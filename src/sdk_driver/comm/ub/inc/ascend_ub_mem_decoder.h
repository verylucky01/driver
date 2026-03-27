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
#ifndef ASCEND_UB_MEM_DECODER_H
#define ASCEND_UB_MEM_DECODER_H

#include "ascend_ub_common.h"
#include "ascend_ub_unit_adapt.h"

#define ASCEND_UB_MEM_NONLINEAR_NUM 32u
#define UB_MEM_KEY_UB_BASE          "UBA_BASE"
#define UB_MEM_KEY_UB_TID           "UB_TID"
#define UB_MEM_KEY_UB_MEM_ID        "UB_MEM_ID"

struct ubdrv_ubmem_set_decoder_info {
    u32 dev_id;
    u64 uba;
    u64 max_share_mem_size;
    u64 tid;
    u64 mem_id;
};

struct ubmem_nonlinear_pa_info {
    u64 remote_id:16;
    u64 to_host:1;
    u64 rsv:47;
    u64 pa_base;
    u64 pa_size;
};

struct ubmem_decoder_msg_data {
    u32 version;
    u16 super_pod_size;  // npu pod size
    u16 mem_id;  // only use in ub mem_decoder info
    u32 tid;
    u64 max_share_mem_size;
    u64 uba;
    u64 linear_pa_base;
    u64 linear_pa_size;
    u64 nonlinear_pa_num : 8;
    u64 rsv : 56;
    struct ubmem_nonlinear_pa_info nolinear_pa_info[ASCEND_UB_MEM_NONLINEAR_NUM];
} __attribute__((packed));

struct ubdrv_ubmem_decoder_info {
    ka_rw_semaphore_t rw_sem;
    u32 valid;
    struct ubmem_decoder_msg_data *decoder_info;
};

struct ubmem_host_cfg_msg_data {
    u32 version;
    u16 mem_id;
    u16 rsv;
    u64 uba;
    u64 max_share_mem_size;
    u32 tid;
    u32 rsv1;
} __attribute__((packed));

struct ubdrv_ubmem_host_cfg_info {
    ka_rw_semaphore_t rw_sem;
    u32 valid;
    struct ubmem_host_cfg_msg_data *cfg_info;
};

int ubdrv_set_mem_decoder_info(struct ubdrv_ubmem_set_decoder_info *info);
#endif // ASCEND_UB_MEM_DECODER_H