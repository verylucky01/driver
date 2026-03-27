/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "urma_api.h"
#include "urma_types.h"

#include "ascend_hal_error.h"
#include "dms_misc_interface.h"
#include "comm_user_interface.h"

#include "ascend_urma_pub.h"
#include "ascend_urma_log.h"
#include "ascend_urma_dev.h"

static pthread_rwlock_t g_token_val_rwlock = PTHREAD_RWLOCK_INITIALIZER;
static int g_token_is_inited[ASCEND_URMA_MAX_DEV_NUM] = {0};
static u32 g_token_val[ASCEND_URMA_MAX_DEV_NUM];

static int _ascend_urma_get_token_val(u32 devid, u32 *token_val)
{
    u32 got_token_val;
    int ret;

    if (g_token_is_inited[devid] == 1) {
        *token_val = g_token_val[devid];
        return DRV_ERROR_NONE;
    }

    ret = dms_get_token_val(devid, UNIQUE_TOKEN_VAL, &got_token_val);
    if (ret != DRV_ERROR_NONE) {
        ascend_urma_err("Dms get unique token val failed. (devid=%u; ret=%d)\n", devid, ret);
        return ret;
    }

    g_token_val[devid] = got_token_val;
    g_token_is_inited[devid] = 1;
    *token_val = g_token_val[devid];

    return DRV_ERROR_NONE;
}

int ascend_urma_get_token_val(uint32_t devid, uint32_t *token_val)
{
    int ret;

    if (devid >= ASCEND_URMA_MAX_DEV_NUM) {
        ascend_urma_err("Invalid devid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (g_token_is_inited[devid] == 1) {
        *token_val = g_token_val[devid];
        return DRV_ERROR_NONE;
    }

    (void)pthread_rwlock_wrlock(&g_token_val_rwlock);
    ret =_ascend_urma_get_token_val(devid, token_val);
    (void)pthread_rwlock_unlock(&g_token_val_rwlock);

    return ret;
}

#ifdef SSAPI_USE_MAMI
static urma_target_jetty_t *ascend_urma_import_jfr_use_mami(urma_context_t *urma_ctx, urma_rjfr_t *rjfr, urma_token_t *token_value)
{
    urma_get_tp_cfg_t tpid_cfg = {
        .trans_mode = URMA_TM_RM,
        .local_eid = urma_ctx->eid,
        .flag.bs.ctp = true,
    };
    urma_active_tp_cfg_t active_tp_cfg = {0};
    urma_tp_info_t tpid_info = {0};
    uint32_t tp_cnt = 1;
    urma_status_t status;

    tpid_cfg.peer_eid = rjfr->jfr_id.eid;
    status = urma_get_tp_list(urma_ctx, &tpid_cfg, &tp_cnt, &tpid_info);
    if ((status != 0) || (tp_cnt == 0)) {
        ascend_urma_err("urma get tp list failed (status=%d; tp_cnt=%u)\n", status, tp_cnt);
        return NULL;
    }

    active_tp_cfg.tp_handle = tpid_info.tp_handle;
    rjfr->tp_type = URMA_CTP;
    return urma_import_jfr_ex(urma_ctx, rjfr, token_value, &active_tp_cfg);
}
#endif

urma_target_jetty_t *ascend_urma_import_jfr(urma_context_t *urma_ctx, urma_rjfr_t *rjfr, urma_token_t *token_value)
{
#ifdef SSAPI_USE_MAMI
    return ascend_urma_import_jfr_use_mami(urma_ctx, rjfr, token_value);
#else
    return urma_import_jfr(urma_ctx, rjfr, token_value);
#endif
}

