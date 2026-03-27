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
#ifndef ASSCEND_UB_UNIT_ADAPT_H
#define ASSCEND_UB_UNIT_ADAPT_H

#include "ka_fs_pub.h"
#include "comm_kernel_interface.h"
#include "ascend_ub_common.h"

#define UBDRV_NONTRANS_TYPE_CNT devdrv_msg_client_max

int ubdrv_device_rescan(u32 dev_id);
int ubdrv_device_pre_reset(u32 dev_id);
int ubdrv_hot_reset_device(u32 dev_id);

int ubdrv_query_host_mem_decoder_info(u32 dev_id, enum ubdrv_log_level log_level);
int ubdrv_procfs_mem_host_cfg_dfx_show(ka_seq_file_t *seq, void *offset);
int ubdrv_mem_host_cfg_ctrl_init(void);
void ubdrv_mem_host_cfg_ctrl_uninit(void);

int ubdrv_rao_read(u32 dev_id, enum devdrv_rao_client_type type, u64 offset, u64 len);
int ubdrv_rao_write(u32 dev_id, enum devdrv_rao_client_type type, u64 offset, u64 len);

#endif