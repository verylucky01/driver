/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <limits.h>
#include "ascend_hal.h"
#include "drv_user_common.h"
#include "trs_user_pub_def.h"
#include "trs_ioctl.h"
#include "trs_res.h"
#include "trs_sqcq.h"
#include "trs_master_urma.h"
#include "trs_master_async.h"
#include "trs_master_event.h"
#include "trs_urma_async.h"

#ifndef EMU_ST
drvError_t __attribute__((weak)) trs_urma_res_config(uint32_t dev_id, struct halResourceIdInputInfo *in,
    struct halResourceConfigInfo *para)
{
    (void)dev_id;
    (void)in;
    (void)para;
    return DRV_ERROR_NOT_SUPPORT;
}

drvError_t __attribute__((weak)) trs_sqcq_urma_alloc(uint32_t dev_id, struct halSqCqInputInfo *in,
    struct halSqCqOutputInfo *out)
{
    (void)dev_id;
    (void)in;
    (void)out;
    return DRV_ERROR_NOT_SUPPORT;
}

drvError_t __attribute__((weak)) trs_sqcq_urma_free(uint32_t dev_id, struct halSqCqFreeInfo *info, bool remote_free_flag)
{
    (void)dev_id;
    (void)info;
    (void)remote_free_flag;
    return DRV_ERROR_NOT_SUPPORT;
}

drvError_t __attribute__((weak)) trs_sq_cq_query_sync(uint32_t dev_id, struct halSqCqQueryInfo *info)
{
    (void)dev_id;
    (void)info;
    return DRV_ERROR_NOT_SUPPORT;
}

drvError_t __attribute__((weak)) trs_sq_cq_config_sync(uint32_t dev_id, struct halSqCqConfigInfo *info)
{
    (void)dev_id;
    (void)info;
    return DRV_ERROR_NOT_SUPPORT;
}

drvError_t __attribute__((weak)) trs_sq_task_args_async_copy(uint32_t dev_id, struct halSqTaskArgsInfo *info,
    struct sqcq_usr_info *sq_info)
{
    (void)dev_id;
    (void)info;
    (void)sq_info;
    return DRV_ERROR_NOT_SUPPORT;
}

drvError_t __attribute__((weak)) trs_async_dma_wqe_create(uint32_t dev_id, struct halAsyncDmaInputPara *in,
    struct halAsyncDmaOutputPara *out)
{
    (void)dev_id;
    (void)in;
    (void)out;
    return DRV_ERROR_NOT_SUPPORT;
}

drvError_t __attribute__((weak)) trs_async_dma_wqe_destory(uint32_t dev_id, struct halAsyncDmaDestoryPara *para)
{
    (void)dev_id;
    (void)para;
    return DRV_ERROR_NOT_SUPPORT;
}

drvError_t __attribute__((weak)) trs_sq_task_send_urma(uint32_t dev_id, struct halTaskSendInfo *info,
    struct sqcq_usr_info *sq_info, unsigned long long *trace_time_stamp, int time_arraylen)
{
    (void)dev_id;
    (void)info;
    (void)sq_info;
    (void)trace_time_stamp;
    (void)time_arraylen;
    return DRV_ERROR_NOT_SUPPORT;
}
#endif

static bool trs_is_remote_res_config_ops(uint32_t flag)
{
    if (drvGetRuntimeApiVer() >= TRS_MC2_BIND_LOGICCQ_FEATURE) {
        if ((flag & TSDRV_FLAG_REMOTE_ID) != 0) {
            return true;
        }
    } else {
        trs_debug("Not support. (runtime_ver=0x%x; drv_ver=0x%x)\n", drvGetRuntimeApiVer(), __HAL_API_VERSION);
    }
    return false;
}

drvError_t halResourceConfig(uint32_t devId, struct halResourceIdInputInfo *in, struct halResourceConfigInfo *para)
{
    drvError_t ret;

    ret = trs_res_config_para_check(devId, in, para);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }
    if ((trs_is_remote_res_config_ops(in->res[RESOURCEID_RESV_FLAG]))) {
        return trs_remote_res_config(devId, in, para);
    }

    if (((in->type == DRV_NOTIFY_ID) || (in->type == DRV_CNT_NOTIFY_ID)) && 
        ((para->prop == DRV_ID_RECORD) || (para->prop == DRV_ID_RESET))) {
        int connection_type = trs_get_connection_type(devId);
        switch (connection_type) {
            case TRS_CONNECT_PROTOCOL_PCIE:
            case TRS_CONNECT_PROTOCOL_HCCS:
            case TRS_CONNECT_PROTOCOL_RC:
                return trs_local_res_config(devId, in, para);
            case TRS_CONNECT_PROTOCOL_UB:
                return trs_urma_res_config(devId, in, para);
            default:
                trs_err("Invalid connection type. (dev_id=%u; connection_type=%d)\n", devId, connection_type);
                return DRV_ERROR_INVALID_DEVICE;
        }
    } else {
        return trs_local_res_config(devId, in, para);
    }
}

drvError_t halStreamTaskFill(uint32_t dev_id, uint32_t stream_id, void *stream_mem, void *task_info, uint32_t task_cnt)
{
    if (dev_id >= TRS_DEV_NUM) {
        trs_err("Invalid devId. (dev_id=%u)\n", dev_id);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((stream_mem == NULL) || (task_info == NULL)) {
        trs_err("Null ptr.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    return trs_stream_task_fill(dev_id, stream_id, stream_mem, task_info, task_cnt);
}

drvError_t halSqSwitchStreamBatch(uint32_t dev_id, struct sq_switch_stream_info *info, uint32_t num)
{
    if (dev_id >= TRS_DEV_NUM) {
        trs_err("Invalid devId. (dev_id=%u)\n", dev_id);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (info == NULL) {
        trs_err("Null ptr.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    return trs_sq_switch_stream_batch(dev_id, info, num);
}

drvError_t halSqCqAllocate(uint32_t devId, struct halSqCqInputInfo *in, struct halSqCqOutputInfo *out)
{
    drvError_t ret;

    ret = trs_sqcq_alloc_para_check(devId, in, out);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    if ((in->type == DRV_NORMAL_TYPE) && !trs_is_remote_sqcq_ops(in->type, in->flag)) {
        int connection_type = trs_get_connection_type(devId);
        switch (connection_type) {
            case TRS_CONNECT_PROTOCOL_PCIE:
            case TRS_CONNECT_PROTOCOL_HCCS:
            case TRS_CONNECT_PROTOCOL_RC:
                return trs_sqcq_alloc(devId, in, out);
            case TRS_CONNECT_PROTOCOL_UB:
                return trs_sqcq_urma_alloc(devId, in, out);
            default:
                trs_err("Invalid connection type. (dev_id=%u; connection_type=%d)\n", devId, connection_type);
                return DRV_ERROR_INVALID_DEVICE;
        }
    } else {
        return trs_sqcq_alloc(devId, in, out);
    }
}

drvError_t halSqCqFree(uint32_t devId, struct halSqCqFreeInfo *info)
{
    drvError_t ret;

    ret = trs_sqcq_free_para_check(devId, info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    if ((info->type == DRV_NORMAL_TYPE) && !trs_is_remote_sqcq_ops(info->type, info->flag)) {
        int connection_type = trs_get_connection_type(devId);
        switch (connection_type) {
            case TRS_CONNECT_PROTOCOL_PCIE:
            case TRS_CONNECT_PROTOCOL_HCCS:
            case TRS_CONNECT_PROTOCOL_RC:
                return trs_sqcq_free(devId, info);
            case TRS_CONNECT_PROTOCOL_UB:
                return trs_sqcq_urma_free(devId, info, true);
            default:
                trs_err("Invalid connection type. (dev_id=%u; connection_type=%d)\n", devId, connection_type);
                return DRV_ERROR_INVALID_DEVICE;
        }
    } else {
        return trs_sqcq_free(devId, info);
    }
}

drvError_t halSqCqQuery(uint32_t devId, struct halSqCqQueryInfo *info)
{
    if (info == NULL) {
        trs_err("Null ptr.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((devId >= TRS_DEV_NUM) || (info->tsId >= TRS_TS_NUM)) {
        trs_err("Invalid para. (dev_id=%u; ts_id=%u)\n", devId, info->tsId);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (info->type == DRV_NORMAL_TYPE) {
        int connection_type = trs_get_connection_type(devId);
        switch (connection_type) {
            case TRS_CONNECT_PROTOCOL_PCIE:
            case TRS_CONNECT_PROTOCOL_HCCS:
            case TRS_CONNECT_PROTOCOL_RC:
                return trs_sq_cq_query(devId, info);
            case TRS_CONNECT_PROTOCOL_UB:
                if ((info->type == DRV_NORMAL_TYPE) &&
                    ((info->prop == DRV_SQCQ_PROP_SQ_HEAD) || (info->prop == DRV_SQCQ_PROP_SQ_CQE_STATUS) ||
                    (info->prop == DRV_SQCQ_PROP_SQ_DEPTH) || (info->prop == DRV_SQCQ_PROP_SQ_MEM_ATTR) ||
                    (info->prop == DRV_SQCQ_PROP_SQ_BASE))) {
                    return trs_sq_cq_query(devId, info);
                }
                return trs_sq_cq_query_sync(devId, info);
            default:
                trs_err("Invalid connection type. (dev_id=%u; connection_type=%d)\n", devId, connection_type);
                return DRV_ERROR_INVALID_DEVICE;
        }
    } else {
        return trs_sq_cq_query(devId, info);
    }
}

static bool trs_is_remote_sq_cq_config(struct halSqCqConfigInfo *info)
{
    if (trs_is_remote_sqcq_ops(info->type, info->value[SQCQ_CONFIG_INFO_FLAG])) {
        if (info->prop == DRV_SQCQ_PROP_SQCQ_RESET) {
            return true;
        }
        if ((info->sqId == UINT_MAX) && (info->cqId == UINT_MAX) &&
            ((info->prop == DRV_SQCQ_PROP_SQ_PAUSE) || (info->prop == DRV_SQCQ_PROP_SQ_RESUME))) {
            return true;
        }
    }
    return false;
}

static bool trs_is_support_sq_cq_ops(struct halSqCqConfigInfo *info)
{
#ifndef CFG_FEATURE_SQCQ_RESET
    if (info->prop == DRV_SQCQ_PROP_SQCQ_RESET) {
        return false;
    }
#endif
    return true;
}

drvError_t halSqCqConfig(uint32_t devId, struct halSqCqConfigInfo *info)
{
    if (info == NULL) {
        trs_err("Null ptr.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((devId >= TRS_DEV_NUM) || (info->tsId >= TRS_TS_NUM)) {
        trs_err("Invalid para. (dev_id=%u; ts_id=%u)\n", devId, info->tsId);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (!trs_is_support_sq_cq_ops(info)) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (trs_is_remote_sq_cq_config(info)) {
        return trs_sq_cq_config_sync(devId, info);
    }
    info->value[SQCQ_CONFIG_INFO_FLAG] = 0;

    if (info->type == DRV_NORMAL_TYPE) {
        int connection_type = trs_get_connection_type(devId);
        drvError_t ret;
        switch (connection_type) {
            case TRS_CONNECT_PROTOCOL_PCIE:
            case TRS_CONNECT_PROTOCOL_HCCS:
            case TRS_CONNECT_PROTOCOL_RC:
                return trs_sq_cq_config(devId, info);
            case TRS_CONNECT_PROTOCOL_UB:
                if ((info->prop == DRV_SQCQ_PROP_SQCQ_RESET) || (info->prop == DRV_SQCQ_PROP_SQ_PAUSE) ||
                    (info->prop == DRV_SQCQ_PROP_SQ_RESUME)) {
                    return trs_sq_cq_config(devId, info);
                }
                if (info->prop == DRV_SQCQ_PROP_SQ_HEAD) {
                    ret = trs_set_sq_info_head(devId, info->tsId, info->type, info->sqId, info->value[0]);
                    if (ret != 0) {
                        return ret;
                    }
                }
                if (info->prop == DRV_SQCQ_PROP_SQ_TAIL) {
                    ret = trs_set_sq_info_tail(devId, info->tsId, info->type, info->sqId, info->value[0]);
                    if (ret != 0) {
                        return ret;
                    }
                }
                return trs_sq_cq_config_sync(devId, info); /* UB scene: set sq register should compelete remotely */
            default:
                trs_err("Invalid connection type. (dev_id=%u; connection_type=%d)\n", devId, connection_type);
                return DRV_ERROR_INVALID_DEVICE;
        }
    } else {
        return trs_sq_cq_config(devId, info);
    }
}

drvError_t halSqTaskArgsAsyncCopy(uint32_t devId, struct halSqTaskArgsInfo *info)
{
#ifndef EMU_ST
    struct sqcq_usr_info *sq_info = NULL;
    drvError_t ret;

    if (info == NULL) {
        trs_err("Null ptr.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((devId >= TRS_DEV_NUM) || (info->tsId >= TRS_TS_NUM) ||
        (info->src == 0) || (info->dst == 0) || (info->size == 0) || (info->type != DRV_NORMAL_TYPE)) {
        trs_err("Invalid para. (dev_id=%u; ts_id=%u; src=0x%llx; dst=0x%llx; size=%u; type=%d)\n",
            devId, info->tsId, info->src, info->dst, info->size, info->type);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (trs_get_connection_type(devId) != TRS_CONNECT_PROTOCOL_UB) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    sq_info = trs_get_sq_info(devId, info->tsId, info->type, info->sqId);
    if (sq_info == NULL) {
        trs_err("Invalid para. (dev_id=%u; ts_id=%u; sq_id=%u)\n", devId, info->tsId, info->sqId);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = trs_sq_task_args_async_copy(devId, info, sq_info);
    if (ret != 0) {
        trs_warn_limit("Copy sq args warn. (devid=%u; sqid=%u)\n", devId, info->sqId);
        return ret;
    }
    return DRV_ERROR_NONE;
#endif
}

static drvError_t trs_async_para_check(uint32_t dev_id, struct halAsyncDmaInputPara *in,
    struct halAsyncDmaOutputPara *out)
{
    if ((in == NULL) || (out == NULL)) {
        trs_err("Invalid para.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((dev_id >= TRS_DEV_NUM) || (in->tsId >= TRS_TS_NUM) || (in->src == NULL) || (in->len == 0) ||
        (in->type != DRV_NORMAL_TYPE)) {
        trs_err("Invalid para. (dev_id=%u; ts_id=%u; len=%u; type=%d)\n", dev_id, in->tsId, in->len, in->type);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((in->dir != TRS_ASYNC_HOST_TO_DEVICE) && (in->dir != TRS_ASYNC_DEVICE_TO_HOST) &&
        (in->dir != TRS_ASYNC_DEVICE_TO_DEVICE)) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    return 0;
}

drvError_t halAsyncDmaCreate(uint32_t devId, struct halAsyncDmaInputPara *in, struct halAsyncDmaOutputPara *out)
{
    int connection_type;
    drvError_t ret;

    ret = trs_async_para_check(devId, in, out);
    if (ret != 0) {
        return ret;
    }

    connection_type = trs_get_connection_type(devId);
    switch (connection_type) {
        case TRS_CONNECT_PROTOCOL_PCIE:
            return trs_async_dma_desc_create(devId, in, out);
        case TRS_CONNECT_PROTOCOL_UB:
            return trs_async_dma_wqe_create(devId, in, out);
        default:
            return DRV_ERROR_NOT_SUPPORT;
    }
}

drvError_t halAsyncDmaWqeCreate(uint32_t devId, struct halAsyncDmaInputPara *in, struct halAsyncDmaOutputPara *out)
{
    return halAsyncDmaCreate(devId, in, out);
}

drvError_t halAsyncDmaDestory(uint32_t devId, struct halAsyncDmaDestoryPara *para)
{
    int connection_type;

    if (para == NULL) {
        trs_err("Invalid para.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (devId >= TRS_DEV_NUM || para->tsId >= TRS_TS_NUM) {
        trs_err("Invalid para. (dev_id=%u; ts_id=%u)\n", devId, para->tsId);
        return DRV_ERROR_INVALID_VALUE;
    }

    connection_type = trs_get_connection_type(devId);
    switch (connection_type) {
        case TRS_CONNECT_PROTOCOL_PCIE:
            return trs_async_dma_destory(devId, para);
        case TRS_CONNECT_PROTOCOL_UB:
            return trs_async_dma_wqe_destory(devId, para);
        default:
            return DRV_ERROR_NOT_SUPPORT;
    }
}

drvError_t halAsyncDmaWqeDestory(uint32_t devId, struct halAsyncDmaDestoryPara *para)
{
    return halAsyncDmaDestory(devId, para);
}

drvError_t halSqTaskSend(uint32_t devId, struct halTaskSendInfo *info)
{
    struct sqcq_usr_info *sq_info = NULL;
    unsigned long long trace_time_stamp[TRS_TASK_SEND_TRACE_TOTAL_NUM] = {0};
    int ret = 0;

    TRS_TRACE_TIME_CONSUME_START(trs_is_task_trace_env_set(), trace_time_stamp, TRS_TASK_SEND_TRACE_TOTAL_NUM,
        TRS_TASK_SEND_TIME_LIMIT);

    if (info == NULL) {
        trs_err("Null ptr.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((devId >= TRS_DEV_NUM) || (info->tsId >= TRS_TS_NUM)) {
        trs_err("Invalid para. (dev_id=%u; ts_id=%u)\n", devId, info->tsId);
        return DRV_ERROR_INVALID_VALUE;
    }

    sq_info = trs_get_sq_info(devId, info->tsId, info->type, info->sqId);
    if (sq_info == NULL) {
        trs_err("Invalid para. (dev_id=%u; ts_id=%u; type=%d; sq_id=%u)\n", devId, info->tsId, info->type, info->sqId);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (!trs_is_sq_support_send(sq_info)) {
        trs_warn("Sq is only id type, not support task send. (dev_id=%u; ts_id=%u; sq_id=%u; flag=%u)\n",
            devId, info->tsId, info->sqId, sq_info->flag);
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (info->type == DRV_NORMAL_TYPE) {
        int connection_type = trs_get_connection_type(devId);
        switch (connection_type) {
            case TRS_CONNECT_PROTOCOL_PCIE:
            case TRS_CONNECT_PROTOCOL_HCCS:
            case TRS_CONNECT_PROTOCOL_RC:
                ret = trs_sq_task_send(devId, info, sq_info);
                break;
            case TRS_CONNECT_PROTOCOL_UB:
                ret = trs_sq_task_send_urma(devId, info, sq_info, trace_time_stamp + 1, TRS_TASK_SEND_TRACE_POINT_NUM);
                break;
            default:
                trs_err("Invalid connection type. (dev_id=%u; connection_type=%d)\n", devId, connection_type);
                return DRV_ERROR_INVALID_DEVICE;
        }
    } else {
        ret = trs_sq_task_send(devId, info, sq_info);
    }

    TRS_TRACE_TIME_CONSUME_END;
    return ret;
}
