/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "ka_fs_pub.h"
#include "ka_common_pub.h"
#include "ka_base_pub.h"
#include "ka_errno_pub.h"
#include "comm_kernel_interface.h"
#include "ascend_ub_common.h"
#include "ascend_ub_jetty.h"
#include "ascend_ub_dev.h"
#include "ascend_ub_path.h"
#include "ascend_ub_main.h"
#include "ascend_ub_pair_dev_info.h"
#include "ascend_ub_load.h"

#ifdef CFG_FEATURE_CDMA_LOAD_IMAGE
STATIC int ubdrv_uvb_msg_check_para(const struct cis_message *msg, unsigned long expect_input_len,
    unsigned long expect_output_len)
{
    if (msg == NULL) {
        ubdrv_err("UVB msg is null.\n");
        return -EINVAL;
    }
    if (msg->input == NULL) {
        ubdrv_err("UVB msg input is null.\n");
        return -EINVAL;
    }
    if (msg->output == NULL) {
        ubdrv_err("UVB msg input is null.\n");
        return -EINVAL;
    }

    if (msg->p_output_size == NULL) {
        ubdrv_err("UVB msg p_output_size is null.\n");
        return -EINVAL;
    }

    if (msg->input_size < expect_input_len) {
        ubdrv_err("UVB msg input_size is invalid.(input_size=%u; expect_len=%lu)\n", msg->input_size, expect_input_len);
        return -EINVAL;
    }

    if (*msg->p_output_size < expect_output_len) {
        ubdrv_err("UVB msg output_size is invalid.(input_size=%u; expect_len=%lu)\n", *msg->p_output_size,
            expect_output_len);
        return -EINVAL;
    }

    return 0;
}

STATIC size_t ubdrv_uvb_get_file_trans_size(size_t file_len, u32 offset)
{
    if ((file_len - offset) > UBDRV_MAX_FILE_SIZE) {
        return UBDRV_MAX_FILE_SIZE;
    }

    return (size_t)(file_len - offset);
}

STATIC int ubdrv_uvb_msg_process_part_load(struct ascend_dev *asd_dev, u32 offset, u32 file_id)
{
    struct ubdrv_load_blocks *blocks;
    loff_t pos = offset;
    ka_file_t *p_file;
    size_t file_len;
    ssize_t len;
    int ret;

    p_file = ka_fs_filp_open(asd_dev->loader.file_path, KA_O_RDONLY | KA_O_LARGEFILE, 0);
    if (KA_IS_ERR_OR_NULL(p_file)) {
        ubdrv_err("Open file failed. (dev_id=%u;file_name=%s)\n", asd_dev->dev_id, asd_dev->loader.file_path);
        return -EIO;
    }

    blocks = asd_dev->loader.blocks;
    file_len = ubdrv_uvb_get_file_trans_size(asd_dev->loader.remain_size, offset);
    len = kernel_read(p_file, blocks->blocks_addr[0].addr, file_len, &pos);
    if ((len < 0) || (len != (ssize_t)file_len)) {
        ubdrv_err("Read file failed. (dev_id=%u; file_id=%u; data_size=%llu; read len=%llu)\n",
            asd_dev->dev_id, file_id, (u64)file_len, (u64)len);
        ret = -EIO;
    } else {
        ubdrv_info("Processed load file. (dev_id=%u; file_id=%u; data_size=%llu; total_file_len=%llu)\n", asd_dev->dev_id,
            file_id, (u64)(offset + file_len), asd_dev->loader.remain_size);
        ret = 0;
    }

    ka_fs_filp_close(p_file, NULL);

    return ret;
}

STATIC int ubdrv_uvb_msg_get_final_state(struct cis_message *msg)
{
    struct ubdrv_uvb_get_final_state_input *load_input = NULL;
    struct ascend_dev *asd_dev = NULL;
    int ret = -EINVAL;
    u32 dev_id;

    if ((msg == NULL) || (msg->input == NULL)) {
        ubdrv_err("UVB msg or msg input is null.\n");
        return -EINVAL;
    }

    if (msg->input_size < sizeof(struct ubdrv_uvb_get_final_state_input)) {
        ubdrv_err("UVB msg input_size is invalid.(input_size=%u; expect_len=%lu)\n", msg->input_size,
            sizeof(struct ubdrv_uvb_get_final_state_input));
        return -EINVAL;
    }

    load_input = (struct ubdrv_uvb_get_final_state_input *)msg->input;
    dev_id = ((load_input->slot_id * UBDRV_DEVNUM_PER_SLOT) + load_input->module_id);
    if (dev_id >= ASCEND_UB_PF_DEV_MAX_NUM) {
        ubdrv_err("Get devid by slotid failed. (dev_id=%u; ret=%d)\n", dev_id, ret);
        return -EINVAL;
    }

    asd_dev = ubdrv_get_asd_dev_by_devid(dev_id);
    if (asd_dev == NULL) {
        ubdrv_err("Get asd_dev failed. (dev_id=%u)\n", dev_id);
        return -ENODEV;
    }

    if (load_input->state != 0) {
        ubdrv_err("state has problem(status=%u; dev_id=%u)\n", load_input->state, dev_id);
        return -EINVAL;
    }

    ubdrv_free_load_segment(dev_id, &asd_dev->loader);
    devdrv_ub_set_device_boot_status(dev_id, DSMI_BOOT_STATUS_OS);
    return 0;
}

STATIC int ubdrv_uvb_msg_get_loading_state(struct cis_message *msg)
{
    struct ubdrv_uvb_get_loading_state_input *load_input = NULL;
    struct ascend_dev *asd_dev = NULL;
    int ret = -EINVAL;
    u32 dev_id;

    if ((msg == NULL) || (msg->input == NULL)) {
        ubdrv_err("UVB msg or msg input is null.\n");
        return -EINVAL;
    }

    if (msg->input_size < sizeof(struct ubdrv_uvb_get_loading_state_input)) {
        ubdrv_err("UVB msg input_size is invalid.(input_size=%u; expect_len=%lu)\n", msg->input_size,
            sizeof(struct ubdrv_uvb_get_loading_state_input));
        return -EINVAL;
    }

    load_input = (struct ubdrv_uvb_get_loading_state_input *)msg->input;
    dev_id = ((load_input->slot_id * UBDRV_DEVNUM_PER_SLOT) + load_input->module_id);
    if (dev_id >= ASCEND_UB_PF_DEV_MAX_NUM) {
        ubdrv_err("Get devid by slotid failed. (dev_id=%u; ret=%d)\n", dev_id, ret);
        return -EINVAL;
    }

    asd_dev = ubdrv_get_asd_dev_by_devid(dev_id);
    if (asd_dev == NULL) {
        ubdrv_err("Get asd_dev failed. (dev_id=%u)\n", dev_id);
        return -ENODEV;
    }

    if (load_input->state == 1) {
        // continue trans file
        ret = ubdrv_uvb_msg_process_part_load(asd_dev, load_input->offset, load_input->file_id);
        if (ret != 0) {
            ubdrv_err("Load file failed. (dev_id=%u; ret=%d)\n", dev_id, ret);
        }
    } else if (load_input->state == 0) {
        ubdrv_info("File treansform succ. (dev_id=%u; state=%u; file_id=%d)\n", dev_id, load_input->state,
            load_input->file_id);
        ret = 0;
    } else {
        ubdrv_err("State error. (dev_id=%u; state=%u)\n", dev_id, load_input->state);
    }

    return ret;
}

STATIC u32 ubdrv_uvb_get_file_id_by_name(char *file_name)
{
    int ret;
    u32 val;

    ret = ka_base_kstrtou32(file_name, 10, &val); // 10 is Decimal
    if (ret < 0) {
        ubdrv_err("File name trans id failed.(name_id=\"%s\"; ret=%d)\n", file_name, ret);
        return UBDRV_UVB_INVALID_FILE_ID;
    }

    return val;
}

STATIC int ubdrv_uvb_msg_get_file_info(struct cis_message *msg)
{
    struct ubdrv_uvb_get_file_info_output *load_output = NULL;
    struct ubdrv_uvb_get_file_info_input *load_input = NULL;
    struct ascend_dev *asd_dev = NULL;
    ka_file_t *p_file = NULL;
    char *file_path = NULL;
    u32 dev_id, file_id;
    int ret = -EINVAL;

    ret = ubdrv_uvb_msg_check_para(msg, sizeof(struct ubdrv_uvb_get_file_info_input),
        sizeof(struct ubdrv_uvb_get_file_info_output));
    if (ret != 0) {
        ubdrv_err("UVB msg callback para check failed.\n");
        return ret;
    }

    load_output = (struct ubdrv_uvb_get_file_info_output *)msg->output;
    load_input = (struct ubdrv_uvb_get_file_info_input *)msg->input;
    dev_id = ((load_input->slot_id * UBDRV_DEVNUM_PER_SLOT) + load_input->module_id);
    ubdrv_info("UVB recv msg info. (dev_id=%u;type=%u;slot_id=%u;module_id=%u;server_eid=%u)\n",
        dev_id, load_input->type, load_input->slot_id, load_input->module_id, load_input->server_eid);

    load_input->file_name[UBDRV_STR_MAX_LEN - 1] = '\0';
    file_id = ubdrv_uvb_get_file_id_by_name(load_input->file_name);
    if (file_id == UBDRV_UVB_INVALID_FILE_ID) {
        ubdrv_err("Find file_id by name failed.\n");
        return -EINVAL;
    }

    if (dev_id >= ASCEND_UB_PF_DEV_MAX_NUM) {
        ubdrv_err("Get devid by slotid failed. (dev_id=%u; ret=%d)\n", dev_id, ret);
        return -EINVAL;
    }

    asd_dev = ubdrv_get_asd_dev_by_devid(dev_id);
    if (asd_dev == NULL) {
        ubdrv_err("Get asd_dev failed. (dev_id=%u)\n", dev_id);
        return -ENODEV;
    }

    ret = ubdrv_alloc_load_segment(dev_id, &asd_dev->loader, UBDRV_MAX_FILE_SIZE);
    if (ret != 0) {
        ubdrv_err("Blocks alloc failed. (dev_id=%u;ret=%d)\n", dev_id, ret);
        return ret;
    }

    devdrv_ub_set_device_boot_status(dev_id, DSMI_BOOT_STATUS_BIOS);

    file_path = asd_dev->loader.file_path;
    (void)memset_s(file_path, UBDRV_STR_MAX_LEN, 0, UBDRV_STR_MAX_LEN);
    ret = ubdrv_single_file_path_init(file_path, UBDRV_STR_MAX_LEN, file_id);
    if (ret != 0) {
        ubdrv_err("Single file path init failed. (dev_id=%u; ret=%d)\n", dev_id, ret);
        goto exit;
    }

    ret = ubdrv_load_get_file_size(dev_id, file_path, file_id, &p_file, &asd_dev->loader.remain_size);
    if (ret != 0) {
        goto exit;
    }
    ka_fs_filp_close(p_file, NULL);

    ret = ubdrv_uvb_msg_process_part_load(asd_dev, 0, file_id);
    if (ret != 0) {
        ubdrv_err("File read err. (dev_id=%u; file_name=%s; ret=%d)\n", dev_id, file_path, ret);
        goto exit;
    }

    load_output->file_address = asd_dev->loader.blocks->blocks_addr[0].dma_addr;
    load_output->file_total_size = (u64)asd_dev->loader.remain_size;
    load_output->file_id = file_id;
    load_output->token_id = asd_dev->loader.tid;
    load_output->section_size = UBDRV_MAX_FILE_SIZE;

    return 0;

exit:
    ubdrv_free_load_segment(dev_id, &asd_dev->loader);
    return ret;
}

STATIC int ubdrv_uvb_msg_get_ability(struct cis_message *msg)
{
    struct ubdrv_uvb_get_ability_output *load_output = NULL;
    struct ubdrv_uvb_get_ability_input *load_input = NULL;
    int ret = -EINVAL;

    ret = ubdrv_uvb_msg_check_para(msg, sizeof(struct ubdrv_uvb_get_ability_input),
        sizeof(struct ubdrv_uvb_get_ability_output));
    if (ret != 0) {
        ubdrv_err("UVB msg callback para check failed.\n");
        return ret;
    }

    load_output = (struct ubdrv_uvb_get_ability_output *)msg->output;
    load_input = (struct ubdrv_uvb_get_ability_input *)msg->input;

    load_output->capability = 0;
    load_output->capability |= UBDRV_UB_MEMORY_MASK;
    *msg->p_output_size = sizeof(struct ubdrv_uvb_get_ability_output);
    return 0;
}

static const ubdrv_uvb_msg_ctrl_t g_ubdrv_uvb_func_list[] = {
    {UBIOS_CALL_ID_GET_ABILITY, UBIOS_USER_ID_UB_DEVICE, ubdrv_uvb_msg_get_ability},
    {UBIOS_CALL_ID_GET_FILE_INFO, UBIOS_USER_ID_UB_DEVICE, ubdrv_uvb_msg_get_file_info},
    {UBIOS_CALL_ID_GET_FILE_LOADING_STATE, UBIOS_USER_ID_UB_DEVICE, ubdrv_uvb_msg_get_loading_state},
    {UBIOS_CALL_ID_GET_LOADING_STATE, UBIOS_USER_ID_UB_DEVICE, ubdrv_uvb_msg_get_final_state},
};

STATIC int ubdrv_cis_func_register(void)
{
    int ret, ret_tmp, i, len;

    len = sizeof(g_ubdrv_uvb_func_list) / sizeof(ubdrv_uvb_msg_ctrl_t);
    for (i = 0; i < len; i++) {
        ret = register_local_cis_func(g_ubdrv_uvb_func_list[i].call_id, g_ubdrv_uvb_func_list[i].receiver_id,
            g_ubdrv_uvb_func_list[i].func);
        if (ret != 0) {
            ubdrv_err("Register uvb msg callback failed.(ret=%d; call_id=0x%x)\n", ret,
                g_ubdrv_uvb_func_list[i].call_id);
            goto register_failed;
        }
    }

    return 0;

register_failed:
    for (i = i - 1; i >= 0; i--) {
        ret_tmp = unregister_local_cis_func(g_ubdrv_uvb_func_list[i].call_id, g_ubdrv_uvb_func_list[i].receiver_id);
        if (ret_tmp != 0) {
            ubdrv_err("Unregister uvb msg callback failed. (ret=%d; call_id=0x%x)\n", ret_tmp,
                g_ubdrv_uvb_func_list[i].call_id);
        }
    }

    return ret;
}

STATIC void ubdrv_cis_func_unregister(void)
{
    int ret, i, len;

    len = sizeof(g_ubdrv_uvb_func_list) / sizeof(ubdrv_uvb_msg_ctrl_t);
    for (i = len - 1; i >= 0; i--) {
        ret = unregister_local_cis_func(g_ubdrv_uvb_func_list[i].call_id, g_ubdrv_uvb_func_list[i].receiver_id);
        if (ret != 0) {
            ubdrv_warn("Unregister uvb msg callback has problem.(ret=%d; call_id=0x%x)\n", ret,
                g_ubdrv_uvb_func_list[i].call_id);
        }
    }

    return;
}
#endif
int ubdrv_load_device_init(void)
{
    int ret = 0;

    ret = ubdrv_sdk_path_init();
    if (ret != 0) {
        ubdrv_err("Device sdk_path init failed. (ret=%d)\n", ret);
        return ret;
    }
#ifdef CFG_FEATURE_CDMA_LOAD_IMAGE
    ret = ubdrv_cis_func_register();
    if (ret != 0) {
        ubdrv_err("Register uvb msg callback failed. (ret=%d)\n", ret);
    }
#endif
    return ret;
}

void ubdrv_load_device_uninit(void)
{
    struct ascend_dev *asd_dev = NULL;
    int i;

#ifdef CFG_FEATURE_CDMA_LOAD_IMAGE
    /* unregister image load UVB msg callback */
    ubdrv_cis_func_unregister();
#endif
    for (i = 0; i <  ASCEND_UB_DEV_MAX_NUM; i++) {
        asd_dev = ubdrv_get_asd_dev_by_devid(i);
        ubdrv_free_load_segment(i, &asd_dev->loader);
    }
    return;
}
