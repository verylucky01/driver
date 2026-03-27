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
#include "udma_ctl.h"
#include "ascend_ub_mem_decoder.h"
#include "ascend_ub_main.h"

struct ubdrv_ubmem_host_cfg_info g_host_cfg_info = {0};

int ubdrv_mem_host_cfg_ctrl_init(void)
{
    size_t len = sizeof(struct ubmem_host_cfg_msg_data);
    int ret;
    u32 host_id;

    ka_task_init_rwsem(&g_host_cfg_info.rw_sem);
    ka_task_down_write(&g_host_cfg_info.rw_sem);
    g_host_cfg_info.valid = ASCEND_UB_INVALID;
    g_host_cfg_info.cfg_info = ubdrv_kzalloc(len, KA_GFP_KERNEL);
    if (g_host_cfg_info.cfg_info == NULL) {
        ubdrv_err("Failed to ubdrv_kzalloc mem cfg info (len=%zu)\n", len);
        ka_task_up_write(&g_host_cfg_info.rw_sem);
        return -ENOMEM;
    }
    ka_task_up_write(&g_host_cfg_info.rw_sem);

    host_id = uda_get_host_id();

    ret = ubdrv_query_host_mem_decoder_info(host_id, UBDRV_INFO_LEVEL);
    if (ret != 0) {
        ubdrv_warn("Query ub mem decoder from control plane unsuccessful. (ret=%d)\n", ret);
    }
    return 0;
}

void ubdrv_mem_host_cfg_ctrl_uninit(void)
{
    ka_task_down_write(&g_host_cfg_info.rw_sem);
    g_host_cfg_info.valid = ASCEND_UB_INVALID;
    if (g_host_cfg_info.cfg_info != NULL) {
        ubdrv_kfree(g_host_cfg_info.cfg_info);
        g_host_cfg_info.cfg_info = NULL;
    }
    ka_task_up_write(&g_host_cfg_info.rw_sem);
    return;
}

int ubdrv_procfs_mem_host_cfg_dfx_show(ka_seq_file_t *seq, void *offset)
{
    struct ubmem_host_cfg_msg_data *cfg_info = NULL;

    ka_task_down_read(&g_host_cfg_info.rw_sem);
    if (g_host_cfg_info.valid == ASCEND_UB_INVALID) {
        ka_fs_seq_printf(seq, "Ub mem host cfg info is invalid.\n");
        ka_task_up_read(&g_host_cfg_info.rw_sem);
        return 0;
    }

    cfg_info = g_host_cfg_info.cfg_info;
    ka_fs_seq_printf(seq, "-------------- ub host mem_cfg info --------------.\n");
    ka_fs_seq_printf(seq, "version: %hu\n", cfg_info->version);
    ka_fs_seq_printf(seq, "mem_id: %hu\n", cfg_info->mem_id);
    ka_fs_seq_printf(seq, "tid: %u\n", cfg_info->tid);
    ka_fs_seq_printf(seq, "max_share_mem_size: 0x%llx\n", cfg_info->max_share_mem_size);
    ka_fs_seq_printf(seq, "uba: 0x%pK\n", (void*)(uintptr_t)cfg_info->uba);
    ka_fs_seq_printf(seq, "rsv: %hu\n", cfg_info->rsv);
    ka_fs_seq_printf(seq, "rsv1: %u\n", cfg_info->rsv1);
    ka_task_up_read(&g_host_cfg_info.rw_sem);
    return 0;
}

int ubdrv_query_host_mem_decoder_info(u32 dev_id, enum ubdrv_log_level log_level)
{
    struct ubcore_user_ctl user_ctl = {0};
    struct ubcore_device *ubc_dev = NULL;
    struct ubdrv_ubmem_set_decoder_info set_decoder_info;
    int ret = 0;

    ubc_dev = ubdrv_get_default_user_ctrl_urma_dev();
    if (ubc_dev == NULL) {
        UBDRV_LOG_LEVEL(log_level, "Get ubc_dev unsuccessful. (dev_id=%u)\n", dev_id);
        return -ENODEV;
    }
    user_ctl.uctx = NULL;
    user_ctl.in.opcode = UDMA_USER_CTL_QUERY_HOST_UBMEM_INFO;
    user_ctl.out.addr = (u64)(uintptr_t)(g_host_cfg_info.cfg_info);
    user_ctl.out.len = sizeof(struct ubmem_host_cfg_msg_data);
    ka_task_down_write(&g_host_cfg_info.rw_sem);
    ret = ubcore_user_control(ubc_dev, &user_ctl);
    if (ret != 0) {
        UBDRV_LOG_LEVEL(log_level, "Get ub host mem cfg info unsuccessful. (dev_id=%u;ret=%d)\n", dev_id, ret);
        ka_task_up_write(&g_host_cfg_info.rw_sem);
        return ret;
    }

    set_decoder_info.dev_id = dev_id;
    set_decoder_info.uba = g_host_cfg_info.cfg_info->uba;
    set_decoder_info.max_share_mem_size = g_host_cfg_info.cfg_info->max_share_mem_size;
    set_decoder_info.mem_id = (u64)g_host_cfg_info.cfg_info->mem_id;
    set_decoder_info.tid = (u64)g_host_cfg_info.cfg_info->tid;

    ret = ubdrv_set_mem_decoder_info(&set_decoder_info);
    if (ret != 0) {
        ubdrv_err("Set host ub mem cfg info to soc_res fail. (dev_id=%u;ret=%d)\n", dev_id, ret);
        ka_task_up_write(&g_host_cfg_info.rw_sem);
        return ret;
    }
    g_host_cfg_info.valid = ASCEND_UB_VALID;
    ka_task_up_write(&g_host_cfg_info.rw_sem);
    return 0;
}