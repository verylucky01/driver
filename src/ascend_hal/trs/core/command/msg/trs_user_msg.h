/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef TRS_USER_MSG_H
#define TRS_USER_MSG_H

#include "drv_type.h"

#if defined(CFG_SOC_PLATFORM_CLOUD_V4) || defined(EMU_ST)
#include "urma_types.h"
#endif

struct trs_resid_config_msg {
    struct halResourceIdInputInfo in;
    struct halResourceConfigInfo para;
};

/* trs_res_cache.h */
struct trs_res_addr_info {
    uint64_t addr;
    uint32_t len;
    uint32_t res;
};

#if defined(CFG_SOC_PLATFORM_CLOUD_V4) || defined(EMU_ST)
/* trs_urma.h */
struct trs_remote_sync_info {
    struct halSqCqOutputInfo out;
    uint32_t host_pid;
    urma_jfs_id_t jfs_id;
    urma_jfr_id_t jfr_id;
    urma_jetty_id_t jetty_id;
    urma_seg_t sq_que_seg;
    urma_seg_t sq_tail_seg;
    urma_eid_t eid;
    urma_token_t token;
    uint32_t tpn;
};

struct trs_d2d_sync_info {
    urma_jetty_id_t jetty_id;
    uint32_t tpn;
    uint32_t token_value;
    uint32_t die_id;
    uint32_t func_id;
};
#endif

#endif /* TRS_USER_MSG_H */
