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
#include "securec.h"
#include "urma_api.h"
#include "urma_log.h"
#include "svm_user_interface.h"
#include "trs_dev_drv.h"
#include "trs_uk_msg.h"
#include "esched_user_interface.h"
#include "trs_user_pub_def.h"
#include "trs_urma.h"
#include "trs_res.h"
#include "trs_sqcq.h"
#include "trs_master_async.h"
#include "trs_master_event.h"
#include "trs_master_urma.h"

#define STARS_ASYNC_DMA_WQE_SIZE 64

uint64_t __attribute__((weak)) halSvmRegister(uint32_t dev_id, uint64_t va, uint32_t size)
{
    return va;
}

int __attribute__((weak)) halSvmUnRegister(uint32_t dev_id, uint64_t va, uint32_t size)
{
    return DRV_ERROR_NONE;
}

int __attribute__((weak)) halSvmAccess(uint32_t dev_id, uint64_t dst, uint64_t src, uint32_t size, uint32_t flag)
{
    return DRV_ERROR_NONE;
}

drvError_t trs_register_reg(uint32_t dev_id, uint64_t va, uint32_t size)
{
    if ((va != 0) && (size != 0)) {
        uint64_t align_va = align_down(va, getpagesize());
        uint32_t align_len = align_up(va - align_va + size, getpagesize());
        uint64_t addr = halSvmRegister(dev_id, align_va, align_len);
        return (addr == 0) ? DRV_ERROR_INNER_ERR : DRV_ERROR_NONE;
    }
    return DRV_ERROR_NONE;
}

void trs_unregister_reg(uint32_t dev_id, uint64_t va, uint32_t size)
{
    if ((va != 0) && (size != 0)) {
        uint64_t align_va = align_down(va, getpagesize());
        uint32_t align_len = align_up(va - align_va + size, getpagesize());
        (void)halSvmUnRegister(dev_id, align_va, align_len);
    }
}

static drvError_t trs_urma_res_id_config(uint32_t dev_id, uint32_t ts_id, drvIdType_t type, uint32_t resource_id,
    uint32_t value)
{
    struct res_id_usr_info *info = NULL;
    int ret;

    info = trs_get_res_id_info(dev_id, ts_id, type, resource_id);
    if (info == NULL) {
        return DRV_ERROR_NOT_EXIST;
    }

    ret = halSvmAccess(dev_id, (uint64_t)info->res_addr, (uint64_t)(uintptr_t)&value, info->res_len, SVM_MEM_WRITE);
    if (ret != DRV_ERROR_NONE) {
        trs_err("Failed to config res. (dev_id=%u; ts_id=%u; res_type=%d; res_id=%u; va=0x%llx; len=%u; ret=%d)\n",
            dev_id, ts_id, type, resource_id, info->res_addr, info->res_len, ret);
    }
    return ret;
}

drvError_t trs_urma_res_config(uint32_t dev_id, struct halResourceIdInputInfo *in, struct halResourceConfigInfo *para)
{
    drvError_t ret;

    if (in->resourceId == UINT_MAX) {
        uint32_t res_id_num = trs_get_res_id_num(dev_id, in->tsId, in->type);
        uint32_t id;
        for (id = 0; id < res_id_num; id++) {
            ret = trs_urma_res_id_config(dev_id, in->tsId, in->type, id, para->value[0]);
            if ((ret != DRV_ERROR_NONE) && (ret != DRV_ERROR_NOT_EXIST)) {
                return ret;
            }
        }
        return DRV_ERROR_NONE;
    }

    return trs_urma_res_id_config(dev_id, in->tsId, in->type, in->resourceId, para->value[0]);
}

uint32_t trs_get_jfs_depth(void)
{
    return 5000U; /* 2048: normal task depth */
}

uint32_t trs_get_jfc_depth(void)
{
    return 5000U; /* 64: minimum jfc depth in urma */
}

uint32_t trs_get_jfr_depth(void)
{
    return 64U; /* 64: minimum jfr depth in urma */
}

drvError_t trs_master_register_segment(uint32_t dev_id, struct sqcq_usr_info *sq_info)
{
    struct trs_urma_ctx *urma_ctx = NULL;
    urma_seg_cfg_t seg_cfg = {0};
    urma_reg_seg_flag_t flag = {
        .bs.token_policy = URMA_TOKEN_PLAIN_TEXT,
        .bs.cacheable = URMA_NON_CACHEABLE,
        .bs.access = URMA_ACCESS_READ | URMA_ACCESS_WRITE | URMA_ACCESS_ATOMIC,
        .bs.non_pin = 1, /* alloc_pages memory need non pin, malloc need pin */
        .bs.token_id_valid = 1,
        .bs.reserved = 0
    };

    urma_ctx = (struct trs_urma_ctx *)sq_info->urma_ctx;
    seg_cfg.va = (uint64_t)(uintptr_t)sq_info->sq_map.que.addr;
    seg_cfg.len = (uint64_t)(sq_info->sq_map.que.len);
    seg_cfg.token_value = urma_ctx->token;
    seg_cfg.token_id = urma_ctx->token_id;
    seg_cfg.flag = flag;

    urma_ctx->local_tseg = urma_register_seg(urma_ctx->urma_ctx, &seg_cfg);
    if (urma_ctx->local_tseg == NULL) {
        trs_err("Failed to register sq queue segment.\n");
        return DRV_ERROR_INNER_ERR;
    }

    return DRV_ERROR_NONE;
}

static void trs_master_unregister_segment(struct trs_urma_ctx *urma_ctx)
{
    (void)urma_unregister_seg(urma_ctx->local_tseg);
}

int trs_remote_urma_info_init(struct trs_urma_ctx *urma_ctx, struct trs_remote_sync_info *sync_info)
{
    urma_import_seg_flag_t flag = trs_get_seg_import_flag();
    struct trs_urma_proc_ctx *proc_ctx = trs_get_urma_proc_ctx(urma_ctx->devid);
    if (proc_ctx->tjetty == NULL) {
#ifdef SSAPI_USE_MAMI
        urma_get_tp_cfg_t tpid_cfg = {
            .trans_mode = URMA_TM_RM,
            .local_eid = urma_ctx->eid,  // local_eid
            .peer_eid = sync_info->eid,  // remote_eid
            .flag.bs.ctp = true,
        };
        uint32_t tp_cnt = 1;
        urma_tp_info_t tpid_info = {0};
        urma_status_t status;
        urma_active_tp_cfg_t active_tp_cfg = {0};
#endif
        urma_rjfr_t rjfr = {0};

        rjfr.jfr_id = sync_info->jfr_id;
        rjfr.trans_mode = URMA_TM_RM;
        rjfr.flag.bs.token_policy = URMA_TOKEN_PLAIN_TEXT;
#ifdef SSAPI_USE_MAMI
        status = urma_get_tp_list(urma_ctx->urma_ctx, &tpid_cfg, &tp_cnt, &tpid_info);
        if ((status != 0) || (tp_cnt == 0)) {
            trs_err("Failed to get tp list. (status=%d; tp_cnt=%u)\n", status, tp_cnt);
            return DRV_ERROR_INNER_ERR;
        }
        active_tp_cfg.tp_handle = tpid_info.tp_handle;
        rjfr.tp_type = URMA_CTP;
        proc_ctx->tjetty = urma_import_jfr_ex(urma_ctx->urma_ctx, &rjfr, &sync_info->token, &active_tp_cfg);
#else
        proc_ctx->tjetty = urma_import_jfr(urma_ctx->urma_ctx, &rjfr, &sync_info->token);
#endif
        if (proc_ctx->tjetty == NULL) {
            trs_err("Failed to import remote jfr.\n");
            return DRV_ERROR_INNER_ERR;
        }
    }

    urma_ctx->sq_que_tseg = urma_import_seg(urma_ctx->urma_ctx, &sync_info->sq_que_seg,
        &sync_info->token, 0, flag);
    if (urma_ctx->sq_que_tseg == NULL) {
        trs_err("Failed to import remote sq queue segment.\n");
        return DRV_ERROR_INNER_ERR;
    }
    urma_ctx->sq_tail_tseg = urma_import_seg(urma_ctx->urma_ctx, &sync_info->sq_tail_seg,
        &sync_info->token, 0, flag);
    if (urma_ctx->sq_tail_tseg == NULL) {
        trs_err("Failed to import remote sq tail segment.\n");
        (void)urma_unimport_seg(urma_ctx->sq_que_tseg);
        return DRV_ERROR_INNER_ERR;
    }

    urma_ctx->tjetty = proc_ctx->tjetty;
    urma_ctx->async_ctx.src_jetty_id = sync_info->jetty_id;
    urma_ctx->async_ctx.tpn = sync_info->tpn;

    return DRV_ERROR_NONE;
}

void trs_remote_urma_info_uninit(struct trs_urma_ctx *urma_ctx)
{
    (void)urma_unimport_seg(urma_ctx->sq_tail_tseg);
    (void)urma_unimport_seg(urma_ctx->sq_que_tseg);
    urma_ctx->tjetty = NULL;
}

int trs_master_urma_ctx_init(uint32_t dev_id, struct trs_urma_ctx *master_ctx)
{
    struct trs_urma_proc_ctx *urma_proc_ctx = NULL;
    int ret;

    urma_proc_ctx = trs_get_urma_proc_ctx(dev_id);
    if (urma_proc_ctx == NULL) {
        trs_err("Not open.\n");
        return DRV_ERROR_INVALID_DEVICE;
    }
    master_ctx->urma_ctx = urma_proc_ctx->urma_ctx;
    master_ctx->die_id = urma_proc_ctx->die_id;
    master_ctx->func_id = urma_proc_ctx->func_id;
    master_ctx->jfce = urma_proc_ctx->jfce;
    master_ctx->token = urma_proc_ctx->token;
    master_ctx->eid = urma_proc_ctx->eid;
    master_ctx->devid = dev_id;
    master_ctx->trs_jfr = urma_proc_ctx->trs_jfr;
    INIT_LIST_HEAD(&master_ctx->d2d_ctx_list.head);
    (void)pthread_mutex_init(&master_ctx->d2d_ctx_list.d2d_mutex, NULL);

    master_ctx->token_id = urma_alloc_token_id(master_ctx->urma_ctx);
    if (master_ctx->token_id == NULL) {
        trs_err("Urma alloc token id failed.\n");
        return DRV_ERROR_INNER_ERR;
    }

    ret = trs_create_jfs(master_ctx->urma_ctx, master_ctx->jfce, &master_ctx->trs_jfs);
    if (ret != 0) {
        (void)urma_free_token_id(master_ctx->token_id);
        trs_err("Failed to create master jfs. (ret=%d)\n", ret);
        return ret;
    }

    return ret;
}

void trs_master_urma_ctx_uninit(struct trs_urma_ctx *master_ctx)
{
    trs_destroy_jfs(&master_ctx->trs_jfs);
    (void)urma_free_token_id(master_ctx->token_id);
}

static struct trs_urma_ctx *_trs_master_urma_ctx_init(uint32_t dev_id)
{
    struct trs_urma_ctx *master_ctx = trs_urma_ctx_create();
    if (master_ctx == NULL) {
        trs_err("Failed to create master urma ctx.\n");
        return NULL;
    }

    if (trs_master_urma_ctx_init(dev_id, master_ctx) != 0) {
        trs_urma_ctx_destroy(master_ctx);
        trs_err("Failed to init master urma ctx.\n");
        return NULL;
    }
    return master_ctx;
}

static void trs_master_urma_ctx_un_init(struct trs_urma_ctx *master_ctx)
{
    trs_master_urma_ctx_uninit(master_ctx);
    trs_urma_ctx_destroy(master_ctx);
}

static int trs_sqcq_remote_alloc_with_urma(uint32_t dev_id, struct halSqCqInputInfo *in, struct trs_urma_ctx *urma_ctx,
    struct trs_remote_sync_info *sync_info)
{
    struct event_reply reply;
    void *sync_msg = NULL;
    UINT64 sync_msg_len;
    int ret, result;

    reply.buf_len = sizeof(struct trs_remote_sync_info) + sizeof(int);
    reply.buf = (char *)malloc(reply.buf_len);
    if (reply.buf == NULL) {
        trs_err("Malloc reply buffer failed.\n");
        return DRV_ERROR_OUT_OF_MEMORY;
    };

    sync_msg_len = sizeof(struct halSqCqInputInfo) + sizeof(urma_jfr_id_t) + sizeof(urma_token_t) + in->ext_info_len;
    sync_msg = malloc(sync_msg_len);
    if (sync_msg == NULL) {
        free(reply.buf);
        trs_err("Malloc sync_msg failed.\n");
        return DRV_ERROR_OUT_OF_MEMORY;
    }
    ret = memcpy_s(sync_msg, sync_msg_len, in, sizeof(struct halSqCqInputInfo));
    ret |= memcpy_s(sync_msg + sizeof(struct halSqCqInputInfo), sync_msg_len - sizeof(struct halSqCqInputInfo),
        &urma_ctx->trs_jfr.jfr->jfr_id, sizeof(urma_jfr_id_t));
    ret |= memcpy_s(sync_msg + sizeof(struct halSqCqInputInfo) + sizeof(urma_jfr_id_t),
        sync_msg_len - sizeof(struct halSqCqInputInfo) - sizeof(urma_jfr_id_t), &urma_ctx->token, sizeof(urma_token_t));
    if ((in->ext_info != NULL) && (in->ext_info_len != 0)) {
        ret |= memcpy_s(sync_msg + sizeof(struct halSqCqInputInfo) + sizeof(urma_jfr_id_t) + sizeof(urma_token_t),
            sync_msg_len - sizeof(struct halSqCqInputInfo) - sizeof(urma_jfr_id_t) - sizeof(urma_token_t),
            in->ext_info, in->ext_info_len);
    }
    if (ret != 0) {
        free(reply.buf);
        free(sync_msg);
        trs_err("Failed to memcpy_s. (ret=%d)\n", ret);
        return DRV_ERROR_PARA_ERROR;
    }
    ret = trs_svm_mem_event_sync(dev_id, sync_msg, sync_msg_len, DRV_SUBEVENT_TRS_ALLOC_SQCQ_WITH_URMA_MSG, &reply);
    result = DRV_EVENT_REPLY_BUFFER_RET(reply.buf);
    if ((ret != DRV_ERROR_NONE) || (result != 0)) {
        trs_err("Fail to sync event. (dev_id=%u; ret=%d; result=%d)\n", dev_id, ret, result);
        ret = (result == DRV_ERROR_NO_RESOURCES) ? result : ret;
        free(reply.buf);
        free(sync_msg);
        return ret;
    }

    (void)memcpy_s(sync_info, sizeof(struct trs_remote_sync_info), DRV_EVENT_REPLY_BUFFER_DATA_PTR(reply.buf),
        sizeof(struct trs_remote_sync_info));

    free(reply.buf);
    free(sync_msg);

    return 0;
}

static int trs_sqcq_remote_free_with_urma(uint32_t dev_id, struct halSqCqFreeInfo *in)
{
    struct event_reply reply;
    int result, ret;

    reply.buf_len = sizeof(result);
    reply.buf = (char *)&result;
    ret = trs_local_mem_event_sync(dev_id, in, sizeof(struct halSqCqFreeInfo),
        DRV_SUBEVENT_TRS_FREE_SQCQ_WITH_URMA_MSG, &reply);
    if ((ret != 0) || (result != 0)) {
        trs_err("Failed to sync sqcq free event. (ret=%d; result=%d)\n", ret, result);
        return result;
    }

    return 0;
}

static drvError_t trs_remote_sqcq_alloc_with_urma(uint32_t dev_id, struct trs_urma_ctx *master_ctx,
    struct halSqCqInputInfo *in, struct halSqCqOutputInfo *out)
{
    struct trs_remote_sync_info sync_info = {0};
    struct halSqCqFreeInfo free_in;
    drvError_t ret;

    ret = trs_sqcq_remote_alloc_with_urma(dev_id, in, master_ctx, &sync_info);
    if (ret != 0) {
        trs_err("Failed to alloc sqcq with urma. (dev_id=%u)\n", dev_id);
        return ret;
    }

    ret = trs_remote_urma_info_init(master_ctx, &sync_info);
    if (ret != 0) {
#ifndef EMU_ST
        free_in.type = in->type;
        free_in.tsId = in->tsId;
        free_in.flag = in->flag;
        free_in.sqId = sync_info.out.sqId;
        free_in.cqId = sync_info.out.cqId;
        trs_sqcq_remote_free_with_urma(dev_id, &free_in);
        trs_err("Failed to init remote urma info. (ret=%d; dev_id=%u)\n", ret, dev_id);
        return ret;
#endif
    }

    (void)memcpy_s(out, sizeof(struct halSqCqOutputInfo), &sync_info.out, sizeof(struct halSqCqOutputInfo));
    trs_debug("Remote alloc sqcq success. (sqid=%u; cqid=%u)\n", sync_info.out.sqId, sync_info.out.cqId);
    return DRV_ERROR_NONE;
}

drvError_t trs_remote_sq_cq_free_with_urma(uint32_t dev_id, struct halSqCqFreeInfo *in, struct trs_urma_ctx *urma_ctx,
    bool remote_free_flag)
{
    drvError_t ret;

    trs_remote_urma_info_uninit(urma_ctx);
    if (remote_free_flag) {
        ret = trs_sqcq_remote_free_with_urma(dev_id, in);
        if (ret != 0) {
            trs_err("Failed free sqcq with urma. (ret=%d; dev_id=%u; sqid=%u; cqid=%u)\n",
                ret, dev_id, in->sqId, in->cqId);
            return ret;
        }
    }

    trs_debug("Remote free success. (dev_id=%u; type=%d; sqid=%u; cqid=%u)\n", dev_id, in->type, in->sqId, in->cqId);

    return DRV_ERROR_NONE;
}

static drvError_t trs_local_sq_cq_alloc_with_urma(uint32_t dev_id, struct trs_urma_ctx *master_ctx,
    struct halSqCqInputInfo *in, struct halSqCqOutputInfo *out)
{
    struct sqcq_usr_info *sq_info = NULL;
    struct halSqCqFreeInfo free_in;
    uint32_t flag = in->flag;
    drvError_t ret;

    in->flag |= TSDRV_FLAG_SPECIFIED_SQ_ID | TSDRV_FLAG_SPECIFIED_CQ_ID;
    in->flag &= (~TSDRV_FLAG_RANGE_ID);
    ret = trs_local_sqcq_alloc(dev_id, in, out);
    if (ret != DRV_ERROR_NONE) {
        trs_err("Failed to alloc sqcq with specified id. (dev_id=%u; sq_id=%u; cq_id=%u)\n",
            dev_id, in->sqId, in->cqId);
        return ret;
    }

    sq_info = trs_get_sq_info(dev_id, in->tsId, DRV_NORMAL_TYPE, out->sqId);
    sq_info->urma_ctx = (void *)master_ctx;

    ret = trs_master_register_segment(dev_id, sq_info);
    if (ret != DRV_ERROR_NONE) {
#ifndef EMU_ST
        trs_err("Failed to register master sq queue. (ret=%d; dev_id=%u)\n", ret, dev_id);
        free_in.type = in->type;
        free_in.tsId = in->tsId;
        free_in.flag = in->flag;
        free_in.sqId = in->sqId;
        free_in.cqId = in->cqId;
        (void)trs_local_sqcq_free(dev_id, &free_in);
#endif
    }

    in->flag = flag;
    trs_debug("Local alloc sqcq success. (sqid=%u; cqid=%u)\n", in->sqId, in->cqId);
    return ret;
}

static drvError_t trs_local_sq_cq_free_with_urma(uint32_t dev_id, struct halSqCqFreeInfo *info, struct trs_urma_ctx *urma_ctx)
{
    uint32_t flag = info->flag;
    drvError_t ret;

    info->flag |= TSDRV_FLAG_SPECIFIED_SQ_ID | TSDRV_FLAG_SPECIFIED_CQ_ID;
#ifndef EMU_ST /* Don't delete because in UB UT scene sqcq has been free in device */
    ret = trs_local_sqcq_free(dev_id, info);
    if (ret != 0) {
        trs_err("Failed to free local sqcq. (dev_id=%d)\n", dev_id);
        info->flag = flag;
        return ret;
    }
#endif
    info->flag = flag;
    trs_master_unregister_segment(urma_ctx); /* Unregister after free to avoid free failed */
    return DRV_ERROR_NONE;
}

drvError_t trs_sqcq_urma_alloc(uint32_t dev_id, struct halSqCqInputInfo *in, struct halSqCqOutputInfo *out)
{
    struct halSqCqOutputInfo remote_out = {0};
    struct halSqCqFreeInfo free_in;
    struct trs_urma_ctx *master_ctx = NULL;
    drvError_t ret;

    if ((in->flag & TSDRV_FLAG_ONLY_SQCQ_ID) != 0) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    master_ctx = _trs_master_urma_ctx_init(dev_id);
    if (master_ctx == NULL) {
        trs_err("Failed to init master urma ctx.\n");
        return DRV_ERROR_INNER_ERR;
    }

    ret = trs_remote_sqcq_alloc_with_urma(dev_id, master_ctx, in, &remote_out);
    if (ret != 0) {
#ifndef EMU_ST
        trs_err("Failed to alloc remote sqcq. (ret=%d; dev_id=%u)\n", ret, dev_id);
        goto remote_sqcq_alloc_fail;
#endif
    }

    in->sqId = remote_out.sqId;
    in->cqId = remote_out.cqId;
    ret = trs_local_sq_cq_alloc_with_urma(dev_id, master_ctx, in, out);
    if (ret != 0) {
#ifndef EMU_ST
        trs_err("Failed to alloc sqcq with specified id. (ret=%d; dev_id=%u; sq_id=%u; cq_id=%u)\n",
            ret, dev_id, remote_out.sqId, remote_out.cqId);
        goto local_sqcq_alloc_fail;
#endif
    }

    trs_debug("Alloc sqcq success. (dev_id=%u; sqid=%u; cqid=%u)\n", dev_id, in->sqId, in->cqId);
    return DRV_ERROR_NONE;

#ifndef EMU_ST
local_sqcq_alloc_fail:
    free_in.type = in->type;
    free_in.tsId = in->tsId;
    free_in.sqId = remote_out.sqId;
    free_in.cqId = remote_out.cqId;
    free_in.flag = in->flag;
    (void)trs_remote_sq_cq_free_with_urma(dev_id, &free_in, master_ctx, true);
remote_sqcq_alloc_fail:
    trs_master_urma_ctx_un_init(master_ctx);
    return ret;
#endif
}

drvError_t trs_sqcq_urma_free(uint32_t dev_id, struct halSqCqFreeInfo *info, bool remote_free_flag)
{
    struct sqcq_usr_info *sq_info = NULL;
    struct trs_urma_ctx *urma_ctx = NULL;
    drvError_t ret;

    sq_info = trs_get_sq_info(dev_id, info->tsId, info->type, info->sqId);
    if (sq_info == NULL) {
        return DRV_ERROR_NOT_EXIST;
    }
    urma_ctx = (struct trs_urma_ctx *)sq_info->urma_ctx;

    ret = trs_local_sq_cq_free_with_urma(dev_id, info, urma_ctx);
    if (ret != 0) {
        trs_err("Failed to free local sqcq. (dev_id=%d; sqid=%u; cqid=%u)\n", dev_id, info->sqId, info->cqId);
        return ret;
    }

    ret = trs_remote_sq_cq_free_with_urma(dev_id, info, urma_ctx, remote_free_flag);
    if (ret != 0) {
        trs_err("Failed to free sqcq with urma. (dev_id=%d)\n", dev_id);
        return ret;
    }

    (void)pthread_mutex_lock(&urma_ctx->d2d_ctx_list.d2d_mutex);
    if (!drv_user_list_empty(&urma_ctx->d2d_ctx_list.head)) {
        (void)trs_d2d_async_ctx_destroy(dev_id, info->sqId, &urma_ctx->d2d_ctx_list);
    }
    (void)pthread_mutex_unlock(&urma_ctx->d2d_ctx_list.d2d_mutex);

    trs_master_urma_ctx_un_init(urma_ctx);

    return DRV_ERROR_NONE;
}

int trs_recycle_sq_cq_with_urma(uint32_t dev_id, uint32_t ts_id, uint32_t sq_id, uint32_t cq_id, bool remote_free_flag)
{
    struct halSqCqFreeInfo info = {0};

    info.type = DRV_NORMAL_TYPE;
    info.tsId = ts_id;
    info.sqId = sq_id;
    info.cqId = cq_id;
    return trs_sqcq_urma_free(dev_id, &info, remote_free_flag);
}

static int trs_jfs_wq_credit_update(struct sqcq_usr_info *sq_info)
{
    struct trs_urma_ctx *master_ctx = (struct trs_urma_ctx *)sq_info->urma_ctx;
    urma_cr_t *cr = NULL;
    urma_status_t urma_ret;
    int cnt, i;

    if (master_ctx->trs_jfs.used_wqe == 0) {
        return DRV_ERROR_NO_RESOURCES;
    }

    cr = (urma_cr_t *)malloc(sizeof(urma_cr_t) * master_ctx->trs_jfs.used_wqe);
    if (cr == NULL) {
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    cnt = urma_poll_jfc(master_ctx->trs_jfs.trs_jfc.jfc, master_ctx->trs_jfs.used_wqe, cr);
    if (cnt < 0) {
        trs_err("Urma poll jfc failed. (cnt=%u; used_wqe=%u)\n", cnt, master_ctx->trs_jfs.used_wqe);
        free(cr);
        return DRV_ERROR_INNER_ERR;
    }
    for (i = 0; i < cnt; i++) {
        if (cr[i].status != URMA_CR_SUCCESS) {
            trs_err("ub cr status abnormal  (cq_handle_num=%d, cr_status=%d;", i, cr[i].status);
        }
    }
    master_ctx->trs_jfs.used_wqe -= cnt * TRS_UB_CQE_LINE;

    urma_ret = urma_rearm_jfc(master_ctx->trs_jfs.trs_jfc.jfc, false);
    if (urma_ret != URMA_SUCCESS) {
        trs_err("Urma rearm jfc failed. (ret=%d)\n", urma_ret);
        free(cr);
        return DRV_ERROR_INNER_ERR;
    }

    free(cr);
    trs_debug("Update jfs credit.\n");
    return DRV_ERROR_NONE;
}

static void trs_jfs_write_src_sge_init(struct sqcq_usr_info *sq_info, urma_sge_t *src_sge_buf,
    uint32_t tail, uint32_t sqe_num)
{
#ifndef EMU_ST
    struct trs_urma_ctx *urma_ctx = (struct trs_urma_ctx *)sq_info->urma_ctx;
    src_sge_buf->addr = urma_ctx->local_tseg->seg.ubva.va + tail * sq_info->e_size;
    src_sge_buf->len = sqe_num * sq_info->e_size;
    src_sge_buf->tseg = urma_ctx->local_tseg;
#endif
}

static void trs_jfs_write_dst_sge_init(struct sqcq_usr_info *sq_info, urma_sge_t *dst_sge_sqe, uint32_t tail,
    uint32_t sqe_num)
{
#ifndef EMU_ST
    struct trs_urma_ctx *urma_ctx = (struct trs_urma_ctx *)sq_info->urma_ctx;
    dst_sge_sqe->addr = urma_ctx->sq_que_tseg->seg.ubva.va + tail * sq_info->e_size;
    dst_sge_sqe->len = sqe_num * sq_info->e_size;
    dst_sge_sqe->tseg = urma_ctx->sq_que_tseg;
#endif
}

static void trs_jfs_write_with_notify_dst_sge_init(struct sqcq_usr_info *sq_info, urma_sge_t *dst_sge_sqe,
    urma_sge_t *dst_sge_notify, uint32_t tail, uint32_t sqe_num)
{
#ifndef EMU_ST
    struct trs_urma_ctx *urma_ctx = (struct trs_urma_ctx *)sq_info->urma_ctx;
    dst_sge_sqe->addr = urma_ctx->sq_que_tseg->seg.ubva.va + tail * sq_info->e_size;
    dst_sge_sqe->len = sqe_num * sq_info->e_size;
    dst_sge_sqe->tseg = urma_ctx->sq_que_tseg;

    dst_sge_notify->addr = urma_ctx->sq_tail_tseg->seg.ubva.va;
    dst_sge_notify->len = 8; /* notify len must be 8 bytes */
    dst_sge_notify->tseg = urma_ctx->sq_tail_tseg;
#endif
}

static void trs_fill_sq_task_wq(struct trs_urma_ctx *urma_ctx, urma_opcode_t opcode, urma_jfs_wr_t *wr)
{
#ifndef EMU_ST
    wr->rw.src.num_sge = 1;
    wr->rw.dst.num_sge = 2; /* 2 sges (0:sqe; 1: notify) */
    wr->opcode = opcode;
    wr->tjetty = urma_ctx->tjetty;
    wr->flag.bs.place_order = 2; /* 2: strong order */
    wr->flag.bs.comp_order = 1;  /* 1: Completion order with previous WR */
#endif
}

static void trs_set_urma_resp_cqe(uint32_t index, urma_jfs_wr_t *wr)
{
    if (index == TRS_UB_CQE_LINE) {
        wr->flag.bs.complete_enable = 1; /* 1: Notify local process after the task is completed. */
    } else {
        wr->flag.bs.complete_enable = 0;
    }
}

static int trs_post_send_wr(struct sqcq_usr_info *sq_info, uint32_t wqe_num, urma_jfs_wr_t *wr,
    unsigned long long *trace_time_stamp, int time_arraylen)
{
    struct trs_urma_ctx *urma_ctx = (struct trs_urma_ctx *)sq_info->urma_ctx;
    urma_jfs_wr_t *bad_wr = NULL;
    bool is_tracing_on = false;
    int trace_index = 0;
    int ret;

    if (trs_is_task_trace_env_set() && (trace_time_stamp != NULL) && (time_arraylen == TRS_TASK_SEND_TRACE_POINT_NUM)) {
        is_tracing_on = true;
    }

    TRS_TRACE_RECORD_TIMESTAMP(is_tracing_on, trace_time_stamp + trace_index);
    if ((urma_ctx->trs_jfs.used_wqe + wqe_num) >= trs_get_jfs_depth()) {
        ret = trs_jfs_wq_credit_update(sq_info);
        if (ret != 0) {
            trs_err("Failed to update jfs wq credit. (ret=%d; used_wqe=%u)\n", ret, urma_ctx->trs_jfs.used_wqe);
            return ret;
        }

#ifndef EMU_ST
        if ((urma_ctx->trs_jfs.used_wqe + wqe_num) >= trs_get_jfs_depth()) {
            trs_warn_limit("Jfs credit not enough. (used_wqe=%u; wqe_num=%u)\n", urma_ctx->trs_jfs.used_wqe, wqe_num);
            return DRV_ERROR_NO_RESOURCES;
        }
#endif
    }

    trace_index++;
    TRS_TRACE_RECORD_TIMESTAMP(is_tracing_on, trace_time_stamp + trace_index);
    ret = urma_post_jfs_wr(urma_ctx->trs_jfs.jfs, wr, &bad_wr);
    if (ret != 0) {
        trs_err("Failed to write with notify. (ret=%d)\n", ret);
        return ret;
    }

    trace_index++;
    TRS_TRACE_RECORD_TIMESTAMP(is_tracing_on, trace_time_stamp + trace_index);
    urma_ctx->trs_jfs.used_wqe += wqe_num;
    urma_ctx->trs_jfs.index = (urma_ctx->trs_jfs.index + wqe_num) % TRS_UB_CQE_LINE;

    return 0;
}

drvError_t trs_sq_task_send_with_urma(uint32_t dev_id, uint32_t sqe_num, struct sqcq_usr_info *sq_info,
    unsigned long long *trace_time_stamp, int time_arraylen)
{
    struct trs_urma_ctx *urma_ctx = (struct trs_urma_ctx *)sq_info->urma_ctx;
    urma_jfs_wr_t wr = {0};
    urma_jfs_wr_t wr_next = {0};
    urma_sge_t src_sge_buf = {0};
    urma_sge_t dst_sge_buf[2] = {{0}};
    urma_sge_t src_sge_buf_next = {0};
    urma_sge_t dst_sge_buf_next[2] = {{0}};
    uint32_t wqe_num;
    int ret;

    if (sq_info->tail + sqe_num <= sq_info->depth) {
        trs_fill_sq_task_wq(urma_ctx, URMA_OPC_WRITE_NOTIFY, &wr);
        trs_set_urma_resp_cqe(urma_ctx->trs_jfs.index + 1, &wr);
        wr.rw.notify_data = (sq_info->tail + sqe_num) % sq_info->depth;
        trs_jfs_write_src_sge_init(sq_info, &src_sge_buf, sq_info->tail, sqe_num);
        trs_jfs_write_with_notify_dst_sge_init(sq_info, &dst_sge_buf[0], &dst_sge_buf[1], sq_info->tail, sqe_num);
        wr.rw.src.sge = &src_sge_buf;
        wr.rw.dst.sge = &dst_sge_buf[0];
        wqe_num = 1;
    } else {
#ifndef EMU_ST
        uint32_t cur_sqe_num, sq_tail;
        /* first, make the sqes at the tail of queue */
        trs_fill_sq_task_wq(urma_ctx, URMA_OPC_WRITE, &wr);
        trs_set_urma_resp_cqe(urma_ctx->trs_jfs.index + 1, &wr);
        cur_sqe_num = sq_info->depth - sq_info->tail;
        trs_jfs_write_src_sge_init(sq_info, &src_sge_buf, sq_info->tail, cur_sqe_num);
        trs_jfs_write_dst_sge_init(sq_info, &dst_sge_buf[0], sq_info->tail, cur_sqe_num);
        wr.rw.src.sge = &src_sge_buf;
        wr.rw.dst.sge = &dst_sge_buf[0];

        /* second, make the sqes at the head of queue */
        trs_fill_sq_task_wq(urma_ctx, URMA_OPC_WRITE_NOTIFY, &wr_next);
        trs_set_urma_resp_cqe(urma_ctx->trs_jfs.index + 2, &wr); /* 2: second wqe */
        sq_tail = (sq_info->tail + sqe_num) % sq_info->depth;
        cur_sqe_num = sq_tail;
        wr_next.rw.notify_data = sq_tail;
        trs_jfs_write_src_sge_init(sq_info, &src_sge_buf_next, 0, cur_sqe_num);
        trs_jfs_write_with_notify_dst_sge_init(sq_info, &dst_sge_buf_next[0], &dst_sge_buf_next[1], 0, cur_sqe_num);
        wr_next.rw.src.sge = &src_sge_buf_next;
        wr_next.rw.dst.sge = &dst_sge_buf_next[0];
        wr.next = &wr_next;
        wqe_num = 2; /* 2: means two wqes */
#endif
    }

    ret = trs_post_send_wr(sq_info, wqe_num, &wr, trace_time_stamp, time_arraylen);
    if (ret != 0) {
        trs_warn_limit("Send sq task warn. (ret=%d; dev_id=%u)\n", ret, dev_id);
    }

    return ret;
}

drvError_t trs_sq_task_send_urma(uint32_t dev_id, struct halTaskSendInfo *info, struct sqcq_usr_info *sq_info,
    unsigned long long *trace_time_stamp, int time_arraylen)
{
    struct trs_urma_ctx *urma_ctx = (struct trs_urma_ctx *)sq_info->urma_ctx;
    int ret;

    (void)pthread_mutex_lock(&sq_info->sq_send_mutex);

    if (sq_info->status == 0) {
        (void)pthread_mutex_unlock(&sq_info->sq_send_mutex);
        trs_err("Invalid status. (dev_id=%u; sq_id=%u; status=%u)\n", dev_id, info->sqId, sq_info->status);
        return DRV_ERROR_STATUS_FAIL;
    }

    ret = trs_sq_task_send_check(dev_id, info, sq_info);
    if (ret != DRV_ERROR_NONE) {
        (void)pthread_mutex_unlock(&sq_info->sq_send_mutex);
        return ret;
    }

    if (trs_get_sq_ctrl_flag(sq_info, info) == TRS_SQ_CTRL_BY_TRS_FLAG) {
        trs_sq_task_fill(info, sq_info);
    }

    info->pos = sq_info->tail;

    ret = trs_sq_task_send_with_urma(dev_id, info->sqe_num, sq_info, trace_time_stamp, time_arraylen);
    if (ret != 0) {
        (void)pthread_mutex_unlock(&sq_info->sq_send_mutex);
        return ret;
    }

    sq_info->tail = (sq_info->tail + info->sqe_num) % sq_info->depth;
    trs_sq_send_ok_stat(sq_info, info->sqe_num);
    (void)pthread_mutex_unlock(&sq_info->sq_send_mutex);
    trs_debug("Send sqe success. (dev_id=%u; sq_id=%u; jfs=%u; jfc=%u; sqe_num=%u; sq_tail=%u)\n", dev_id, info->sqId,
        urma_ctx->trs_jfs.jfs->jfs_id.id, urma_ctx->trs_jfs.trs_jfc.jfc->jfc_id.id, info->sqe_num, sq_info->tail);
    return ret;
}

extern int halMemGetSeg(uint32_t dev_id, uint64_t va, uint64_t size, urma_seg_t *seg, urma_token_t *token);
static void trs_jfs_write_sge_init(uint64_t va, uint64_t len, urma_target_seg_t *tseg, urma_sge_t *src_sge_buf)
{
#ifndef EMU_ST
    src_sge_buf->addr = va;
    src_sge_buf->len = len;
    src_sge_buf->tseg = tseg;
#endif
}

#define TRS_URMA_WR_CPY_MAX_SIZE 0x10000000ULL /* 256MB */
drvError_t trs_sq_task_args_async_copy(uint32_t dev_id, struct halSqTaskArgsInfo *info, struct sqcq_usr_info *sq_info)
{
#ifndef EMU_ST
    struct trs_urma_ctx *urma_ctx = (struct trs_urma_ctx *)sq_info->urma_ctx;
    u32 wr_num = align_up((uint64_t)info->size, TRS_URMA_WR_CPY_MAX_SIZE) / TRS_URMA_WR_CPY_MAX_SIZE;
    urma_target_seg_t src_tseg;
    urma_target_seg_t *dst_tseg = NULL;
    urma_seg_t src_seg, dst_seg;
    urma_token_t src_token, dst_token;
    int i;
    int ret;

    if (wr_num > TRS_URMA_ARGS_CPY_WR_MAX_NUM) {
        trs_err("Size exceed depth. (depth=%u; size=%u; num=%u)\n", TRS_URMA_ARGS_CPY_WR_MAX_NUM, info->size, wr_num);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = halMemGetSeg(dev_id, (uint64_t)info->src, (uint64_t)info->size, &src_seg, &src_token);
    if (ret != 0) {
        trs_err("Failed to get src segment. (ret=%d; va=0x%llx; size=%u)\n", ret, info->src, info->size);
        return ret;
    }
    src_tseg.seg = src_seg;

    ret = halMemGetSeg(dev_id, (uint64_t)info->dst, (uint64_t)info->size, &dst_seg, &dst_token);
    if (ret != 0) {
        trs_err("Failed to get dst segment. (ret=%d; va=0x%llx; size=%u)\n", ret, info->dst, info->size);
        return ret;
    }
    dst_tseg = urma_import_seg(urma_ctx->urma_ctx, &dst_seg, &dst_token, 0, trs_get_seg_import_flag());
    if (dst_tseg == NULL) {
        trs_err("Failed to import dst va.\n");
        return DRV_ERROR_INNER_ERR;
    }

    (void)pthread_mutex_lock(&sq_info->sq_send_mutex);

    for (i = 0; i < wr_num; i++) {
        urma_jfs_wr_t *wr = &urma_ctx->trs_jfs.args_cpy_wr_array[i];
        u32 size = (i == (wr_num - 1)) ? (info->size - i * TRS_URMA_WR_CPY_MAX_SIZE) : TRS_URMA_WR_CPY_MAX_SIZE;

        (void)memset_s(wr, sizeof(urma_jfs_wr_t), 0, sizeof(urma_jfs_wr_t));
        wr->opcode = URMA_OPC_WRITE;
        wr->tjetty = urma_ctx->tjetty;
        wr->flag.bs.place_order = TRS_URMA_WR_PLACE_ORDER; /* 2: strong order */
        wr->flag.bs.comp_order = 1;  /* 1: Completion order with previous WR */
        wr->rw.src.sge = &urma_ctx->trs_jfs.args_cpy_src_sge[i];
        wr->rw.src.num_sge = 1;
        wr->rw.dst.sge = &urma_ctx->trs_jfs.args_cpy_dst_sge[i];;
        wr->rw.dst.num_sge = 1;
        trs_set_urma_resp_cqe(urma_ctx->trs_jfs.index + 1 + i, wr);
        trs_jfs_write_sge_init(info->src + i * TRS_URMA_WR_CPY_MAX_SIZE, (uint64_t)size, &src_tseg, wr->rw.src.sge);
        trs_jfs_write_sge_init(info->dst + i * TRS_URMA_WR_CPY_MAX_SIZE, (uint64_t)size, dst_tseg, wr->rw.dst.sge);
        wr->next = (i == (wr_num - 1)) ? NULL : &urma_ctx->trs_jfs.args_cpy_wr_array[i + 1];
    }

    ret = trs_post_send_wr(sq_info, wr_num, &urma_ctx->trs_jfs.args_cpy_wr_array[0], NULL, 0);
    if (ret != 0) {
        (void)pthread_mutex_unlock(&sq_info->sq_send_mutex);
        urma_unimport_seg(dst_tseg);
        trs_warn_limit("Send sq task warn. (ret=%d; dev_id=%u; sq_id=%u)\n", ret, dev_id, info->sqId);
        return ret;
    }
    (void)pthread_mutex_unlock(&sq_info->sq_send_mutex);

    urma_unimport_seg(dst_tseg);
    trs_debug("Sq args copy success. (dev_id=%u; sq_id=%u)\n", dev_id, info->sqId);

    return DRV_ERROR_NONE;
#endif
}
