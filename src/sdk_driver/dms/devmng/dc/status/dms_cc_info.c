/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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

#include <linux/fs.h>

#include "pbl/pbl_feature_loader.h"
#include "ascend_hal_error.h"
#include "dms_define.h"
#include "devdrv_common.h"
#include "comm_kernel_interface.h"
#include "dms_cc_info.h"

BEGIN_DMS_MODULE_DECLARATION(DMS_CC_INFO_CMD_NAME)
BEGIN_FEATURE_COMMAND()
#ifdef CFG_FEATURE_CC_INFO
ADD_DEV_FEATURE_COMMAND(DMS_CC_INFO_CMD_NAME, DMS_MAIN_CMD_BASIC, DMS_SUBCMD_GET_CC_INFO, NULL, NULL,
                    DMS_SUPPORT_ALL, dms_ioctl_get_cc_info)
ADD_DEV_FEATURE_COMMAND(DMS_CC_INFO_CMD_NAME, DMS_MAIN_CMD_BASIC, DMS_SUBCMD_SET_CC_INFO, NULL, "dmp_daemon",
                    DMS_ACC_MANAGE_USER|DMS_ENV_NOT_NORMAL_DOCKER|DMS_VDEV_PHYSICAL, dms_ioctl_set_cc_info)
                    /* Not support vf/normal docker */
#endif
END_FEATURE_COMMAND()
END_MODULE_DECLARATION()

#ifdef CFG_FEATURE_CC_INFO
#define CMDLINE_FILE_PATH "/proc/cmdline"
struct dms_cc_mode g_cc_running_mode = {0};

#ifdef CFG_FEATURE_TEE_BIOS_STUB
struct dms_cc_mode g_cc_cfg_mode_stub= {0};
#endif

#ifndef CFG_FEATURE_TEE_BIOS_STUB
#define CC_CMDLINE_KEY "cc_mode="

/* Pre-code(dms_get_cmdline, dms_parse_cmdline_by_key) before BIOS offer cmdline */
STATIC char* dms_get_cmdline(void)
{
    static char cmdline_buf[DMS_CMDLINE_SIZE] = {0};
    struct file *fp = NULL;
    loff_t pos = 0;

    fp = filp_open(CMDLINE_FILE_PATH, O_RDONLY, 0);
    if (IS_ERR_OR_NULL(fp)) {
        dms_err("Filp open fail! (file=%s)\n", CMDLINE_FILE_PATH);
        return NULL;
    }
    kernel_read(fp, cmdline_buf, DMS_CMDLINE_SIZE - 1, &pos);
    filp_close(fp, NULL);
    return cmdline_buf;
}

STATIC int dms_parse_cmdline_by_key(const char *cmdline, const char *key, char *buf, unsigned int buf_size)
{
    unsigned int i = 0;
    int ret = 0;
    char *value = NULL;

    if (cmdline == NULL || key == NULL || buf == NULL || buf_size == 0) {
        dms_err("Invalid param! (cmdline=%s; key=%s; buf=%s; buf_size=%u)\n", cmdline==NULL ? "NULL" : "OK",
            key==NULL ? "NULL" : "OK", buf==NULL ? "NULL" : "OK", buf_size);
        return -EINVAL;
    }

    value = strstr(cmdline, key);
    if (value == NULL) {
        dms_err("Find key in cmdline failed! (key=%s)\n", key);
        return -EINVAL;
    }

    value += strlen(key); /* In "cc_mode=0", skip "cc_mode=", to get "0" */
    for (i = 0; i < buf_size; i++) {
        if ((value[i] == ' ') || (value[i] == '\0') || (iscntrl(value[i]) != 0)) {
            break; /* find the end of value */
        }
    }

    if (i > buf_size || i == 0) {
        dms_err("Cmdline value out of length limit.(key=%s; value_len=%u; buf_size=%u)\n", key, i, buf_size);
        return -EINVAL;
    }

    ret = strncpy_s(buf, buf_size, value, i);
    if (ret != 0) {
        dms_err("Strncpy failed! (key=%s; ret=%d)\n", key, ret);
        return -ENOMEM;
    }

    return 0;
}
#endif

STATIC int dms_get_cc_info_from_cmdline(unsigned int *cc_mode)
{
    unsigned int mode;
#ifdef CFG_FEATURE_TEE_BIOS_STUB
    mode = 0; /* STUB */
#else
    int ret = 0;
    char cc_running_mode[CC_CMDLINE_VALUE_SIZE + 1] = {0};

    ret = dms_parse_cmdline_by_key(dms_get_cmdline(), CC_CMDLINE_KEY, cc_running_mode, CC_CMDLINE_VALUE_SIZE);
    if (ret != 0) {
        return ret;
    }

    ret = sscanf_s(cc_running_mode, "%u", mode);
    if (ret != CC_CMDLINE_VALUE_SIZE) {
        dms_err("Sscanf failed! (cc_running_mode=%s; ret=%d)\n", cc_running_mode, ret);
        return -ENOMEM;
    }

    if (mode >= DMS_CC_MODE_MAX) {
        dms_err("CC mode invalid! (mode=%u)\n", mode);
        return -EINVAL;
    }
#endif

    dms_info("Show cc mode from cmdline. (mode=%u).\n", mode);
    *cc_mode = mode;
    return 0;
}

STATIC int dms_get_cc_running_info(struct dms_cc_mode *cc_info)
{
    if (g_cc_running_mode.cc_mode >= DMS_CC_MODE_MAX || g_cc_running_mode.crypto_mode >= DMS_CRYPTO_MODE_MAX) {
        dms_err("Get cc running info fail! (cc_mode=%u; crypto_mode=%u)\n",
            g_cc_running_mode.cc_mode, g_cc_running_mode.crypto_mode);
        return -EIO;
    }

    cc_info->cc_mode = g_cc_running_mode.cc_mode;
    cc_info->crypto_mode = g_cc_running_mode.crypto_mode;
    return 0;
}

STATIC int dms_get_cc_cfg_info(unsigned int dev_id, struct dms_cc_mode *cc_info)
{
    int ret = 0;

#ifdef CFG_FEATURE_TEE_BIOS_STUB
    (void)dev_id;
    cc_info->cc_mode = g_cc_cfg_mode_stub.cc_mode;
    cc_info->crypto_mode = g_cc_cfg_mode_stub.crypto_mode;
#else
    struct dms_cc_mode cc_info_from_sram = {0};
    ret = hsm_read_sram_data(dev_id, 0, (void *)&cc_info_from_sram, sizeof(struct dms_cc_mode)); /* only stub */
    if (ret != 0) {
        dms_err("Get cc info from sram failed! (ret=%d; dev_id=%u)\n", ret, dev_id);
        return ret;
    }

    if (cc_info_from_sram.cc_mode >= DMS_CC_MODE_MAX || cc_info_from_sram.crypto_mode >= DMS_CRYPTO_MODE_MAX) {
        dms_err("Invalid output from sram! (cc_mode=%u; crypto_mode=%u)\n", mode.cc_mode, mode.crypto_mode);
        return -EIO;
    }

    cc_info->cc_mode = cc_info_from_sram.cc_mode;
    cc_info->crypto_mode = cc_info_from_sram.crypto_mode;
#endif
    return ret;
}

STATIC int dms_ioctl_output_para_check(const struct urd_cmd *cmd, struct urd_cmd_kernel_para *kernel_para,
    struct urd_cmd_para *para, unsigned int output_len)
{
    if ((cmd == NULL) || (kernel_para == NULL) || (para == NULL)) {
        dms_err("Input urd argument is null.\n");
        return -EINVAL;
    }

    if ((para->output == NULL) || (para->output_len != output_len)) {
        dms_err("Output argument is null, or len is wrong. (output_len=%u)\n", para->output_len);
        return -EINVAL;
    }

    return 0;
}

int hal_kernel_get_cc_info(unsigned int dev_id, struct dms_cc_info *cc_info)
{
    int ret = 0;

    if (cc_info == NULL || dev_id > DEVDRV_PF_DEV_MAX_NUM) {
        dms_err("Invalid param! (dev_id=%u; cc_info=%s)\n", dev_id, cc_info==NULL ? "NULL" : "OK");
        return -EINVAL;
    }

    if (devdrv_get_connect_protocol(dev_id) != CONNECT_PROTOCOL_UB) {
        return -EOPNOTSUPP;
    }

    /* cc running mode */
    ret = dms_get_cc_running_info(&cc_info->cc_running_info);
    if (ret != 0) {
        return ret;
    }
    /* cc cfg mode */
    ret = dms_get_cc_cfg_info(dev_id, &cc_info->cc_cfg_info);
    if (ret != 0) {
        return ret;
    }
    return 0;
}
EXPORT_SYMBOL_GPL(hal_kernel_get_cc_info);

int dms_ioctl_get_cc_info(const struct urd_cmd *cmd, struct urd_cmd_kernel_para *kernel_para, struct urd_cmd_para *para)
{
    int ret = 0;
    struct dms_cc_info cc_info = {0};

    ret = dms_ioctl_output_para_check(cmd, kernel_para, para, sizeof(struct dms_cc_info));
    if (ret != 0) {
        return ret;
    }

    ret = hal_kernel_get_cc_info(kernel_para->phyid, &cc_info);
    if (ret != 0) {
        return ret;
    }

    dms_info("Get cc info success! (dev_id=%u; cc_cfg=%u; crypto_cfg=%u; cc_running=%u; crypto_running=%u)\n",
        kernel_para->phyid, cc_info.cc_cfg_info.cc_mode, cc_info.cc_cfg_info.crypto_mode,
        cc_info.cc_running_info.cc_mode, cc_info.cc_running_info.crypto_mode);
    *(struct dms_cc_info *)(para->output) = cc_info;

    return 0;
}

STATIC int dms_ioctl_input_para_check(const struct urd_cmd *cmd, struct urd_cmd_kernel_para *kernel_para,
    struct urd_cmd_para *para, unsigned int input_len)
{
    if ((cmd == NULL) || (kernel_para == NULL) || (para == NULL)) {
        dms_err("Input urd argument is null.\n");
        return -EINVAL;
    }

    if ((para->input == NULL) || (para->input_len != input_len)) {
        dms_err("Output argument is null, or len is wrong. (input_len=%u)\n", para->input_len);
        return -EINVAL;
    }

    return 0;
}

int dms_set_cc_mode(unsigned int dev_id, struct dms_cc_mode mode)
{
    int ret = 0;

    if (mode.cc_mode >= DMS_CC_MODE_MAX || mode.crypto_mode >= DMS_CRYPTO_MODE_MAX) {
        dms_err("Invalid param! (cc_mode=%u; crypto_mode=%u)\n", mode.cc_mode, mode.crypto_mode);
        return -EINVAL;
    }

    if (devdrv_get_connect_protocol(dev_id) != CONNECT_PROTOCOL_UB) {
        return -EOPNOTSUPP;
    }

#ifdef CFG_FEATURE_TEE_BIOS_STUB
    g_cc_cfg_mode_stub.cc_mode = mode.cc_mode;
    g_cc_cfg_mode_stub.crypto_mode = mode.crypto_mode;
#else
    ret = hsm_write_sram_data(dev_id, 0, (void *)&mode, sizeof(struct dms_cc_mode)); /* only stub */
    if (ret != 0) {
        dms_err("Set cc mode by tee failed! (ret=%d; dev_id=%u; cc_mode=%u; crypto_mode=%u)\n",
            ret, dev_id, mode.cc_mode, mode.crypto_mode);
        return ret;
    }
#endif

    return ret;
}

int dms_ioctl_set_cc_info(const struct urd_cmd *cmd, struct urd_cmd_kernel_para *kernel_para, struct urd_cmd_para *para)
{
    int ret = 0;

    ret = dms_ioctl_input_para_check(cmd, kernel_para, para, sizeof(struct dms_cc_mode));
    if (ret != 0) {
        return ret;
    }

    ret = dms_set_cc_mode(kernel_para->phyid, *(struct dms_cc_mode *)para->input);
    if (ret != 0) {
        return ret;
    }

    dms_info("Set cc info success! (dev_id=%u)\n", kernel_para->phyid);
    return ret;
}

STATIC void dms_cc_running_mode_init(void)
{
    int ret = 0;
    struct dms_cc_mode cc_info = {0};
    g_cc_running_mode.cc_mode = DMS_CC_MODE_MAX;
    g_cc_running_mode.crypto_mode = DMS_CRYPTO_MODE_MAX;

    /* Get cc running mode from cmdline */
    ret = dms_get_cc_info_from_cmdline(&g_cc_running_mode.cc_mode);
    if (ret != 0) {
        dms_warn("Unable to get cc info from cmdline. (ret=%d)\n", ret);
        return;
    }

    /* Get crypto running mode from sram cfg at ko load stage. */
    ret = dms_get_cc_cfg_info(0, &cc_info);
    if (ret != 0) {
        dms_warn("Unable to get cc info from tee. (ret=%d)\n", ret);
        return;
    }
    g_cc_running_mode.crypto_mode = cc_info.crypto_mode;
    dms_info("Get running mode success! (cc_mode=%u; crypto_mode=%u)\n",
        g_cc_running_mode.cc_mode, g_cc_running_mode.crypto_mode);
    return;
}
#endif

int dms_cc_info_init(void)
{
#ifdef CFG_FEATURE_CC_INFO
    dms_cc_running_mode_init();
#endif
    CALL_INIT_MODULE(DMS_CC_INFO_CMD_NAME);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(dms_cc_info_init, FEATURE_LOADER_STAGE_5);

void dms_cc_info_uninit(void)
{
    CALL_EXIT_MODULE(DMS_CC_INFO_CMD_NAME);
}
DECLAER_FEATURE_AUTO_UNINIT(dms_cc_info_uninit, FEATURE_LOADER_STAGE_5);