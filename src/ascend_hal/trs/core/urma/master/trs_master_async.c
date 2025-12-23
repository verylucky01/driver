/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <stdbool.h>
#include "securec.h"
#include "urma_api.h"
#include "urma_log.h"
#include "trs_ioctl.h"
#include "drv_user_common.h"
#include "esched_user_interface.h"
#include "trs_urma.h"
#include "trs_user_pub_def.h"
#include "trs_uk_msg.h"
#include "trs_master_event.h"
#include "trs_master_urma.h"
#include "trs_master_async.h"
#include "trs_urma_async.h"

struct udma_sqe_ctl_tmp {
    uint32_t sqe_bb_idx : 16;
    uint32_t odr : 3;
    uint32_t fence : 1;
    uint32_t se : 1;
    uint32_t cqe : 1;
    uint32_t inline_en : 1;
    uint32_t udf_flag : 1;
    uint32_t rsv : 3;
    uint32_t nf : 1;
    uint32_t token_en : 1;
    uint32_t rmt_jetty_type : 2;
    uint32_t owner : 1;

    uint32_t target_hint : 8;
    uint32_t opcode : 8;
    uint32_t rsv1 : 6;
    uint32_t inline_msg_len : 10;

    uint32_t tpn : 24;
    uint32_t sge_num : 8;

    uint32_t rmt_obj_id : 20;
    uint32_t rsv2 : 12;

    uint32_t rmt_eid[4];
    uint32_t rmt_token_value;
    uint32_t udf;
    uint32_t rmt_addr_l_or_token_id;
    uint32_t rmt_addr_h_or_token_value;
};

struct udma_wqe_sge_tmp {
    uint32_t length;
    uint32_t token_id : 20;
    uint32_t rsv3 : 12;
    uint64_t va;
};

extern int halMemGetSeg(uint32_t dev_id, uint64_t va, uint64_t size, urma_seg_t *seg, urma_token_t *token);
static int trs_get_segment(struct trs_urma_ctx *urma_ctx, uint64_t va, uint64_t size,
    struct trs_async_dma_wqe_info *wqe_info, urma_seg_t *seg, urma_token_t *token)
{
    urma_seg_cfg_t seg_cfg = {0};
    int ret = 0;

    ret = halMemGetSeg(urma_ctx->devid, va, (uint64_t)size, seg, token);
    if (ret == 0) {
        wqe_info->flag = 0;
        trs_debug("Get token_id from svm. (devid=%u)\n", urma_ctx->devid);
        return 0;
    }

#ifndef EMU_ST
    seg_cfg.va = align_down(va, getpagesize());
    seg_cfg.len = align_up(va - seg_cfg.va + size, getpagesize());
    seg_cfg.token_id = urma_ctx->token_id;
    seg_cfg.token_value = urma_ctx->token;
    seg_cfg.flag.bs.token_policy = URMA_TOKEN_PLAIN_TEXT;
    seg_cfg.flag.bs.cacheable = 1;
    seg_cfg.flag.bs.access = URMA_ACCESS_READ | URMA_ACCESS_WRITE | URMA_ACCESS_ATOMIC;
    seg_cfg.flag.bs.non_pin = 0;
    seg_cfg.flag.bs.token_id_valid = 1;
    wqe_info->async_tseg = urma_register_seg(urma_ctx->urma_ctx, &seg_cfg);
    if (wqe_info->async_tseg == NULL) {
        trs_err("Urma register seg failed.\n");
        return DRV_ERROR_INNER_ERR;
    }
    wqe_info->flag = 1;
    trs_debug("Register token_id from tsdrv. (jfs_id=%u; flag=%u)\n", urma_ctx->trs_jfs.jfs->jfs_id.id, wqe_info->flag);

    *seg = wqe_info->async_tseg->seg;
    *token = urma_ctx->token;
    return 0;
#else
    return ret;
#endif
}

void trs_swap_endian(uint8_t dst[], uint8_t src[], uint32_t size)
{
#ifndef EMU_ST
    uint32_t i;
    for (i = 0; i < size; i++) {
        dst[i] = src[size - 1 - i];
    }
#endif
}

static struct trs_async_ctx *trs_get_d2d_async_ctx(uint32_t remote_devid, struct trs_d2d_ctx_list *d2d_ctx_list)
{
    struct trs_async_ctx *ctx = NULL;
    struct list_head *node, *next;

    list_for_each_safe(node, next, &d2d_ctx_list->head) {
        ctx = list_entry(node, struct trs_async_ctx, node);
        if (ctx->recv_devid == remote_devid) {
            return ctx;
        }
    }
    return NULL;
}

static void trs_get_async_send_recv_addr(uint32_t dev_id, struct trs_async_ctx *async_ctx,
    struct halAsyncDmaInputPara *in, uint64_t *send_addr, uint64_t *recv_addr)
{
    if (in->dir == TRS_ASYNC_HOST_TO_DEVICE) {
        *send_addr = (uint64_t)(uintptr_t)in->dst;
        *recv_addr = (uint64_t)(uintptr_t)in->src;
    } else if (in->dir == TRS_ASYNC_DEVICE_TO_HOST) {
        *send_addr = (uint64_t)(uintptr_t)in->src;
        *recv_addr = (uint64_t)(uintptr_t)in->dst;
    } else { /* TRS_ASYNC_DEVICE_TO_DEVICE) */
        if (dev_id == async_ctx->dst_devid) {
            *send_addr = (uint64_t)(uintptr_t)in->dst;
            *recv_addr = (uint64_t)(uintptr_t)in->src;
        } else {
            *send_addr = (uint64_t)(uintptr_t)in->src;
            *recv_addr = (uint64_t)(uintptr_t)in->dst;
        }
    }
}

static struct trs_async_ctx *trs_get_async_ctx(struct trs_urma_ctx *urma_ctx, uint32_t dir, uint32_t remote_dev_id)
{
    if (dir == TRS_ASYNC_DEVICE_TO_DEVICE) {
        struct trs_async_ctx *ctx = NULL;
        (void)pthread_mutex_lock(&urma_ctx->d2d_ctx_list.d2d_mutex);
        ctx = trs_get_d2d_async_ctx(remote_dev_id, &urma_ctx->d2d_ctx_list);
        if (ctx == NULL) {
            (void)pthread_mutex_unlock(&urma_ctx->d2d_ctx_list.d2d_mutex);
        }
        return ctx;
    } else {
        return &urma_ctx->async_ctx;
    }
}

static void trs_put_async_ctx(struct trs_urma_ctx *urma_ctx, uint32_t dir, struct trs_async_ctx *ctx)
{
    if ((dir == TRS_ASYNC_DEVICE_TO_DEVICE) && (ctx != NULL)) {
        (void)pthread_mutex_unlock(&urma_ctx->d2d_ctx_list.d2d_mutex);
    }
}

static int trs_get_async_devid(uint32_t dev_id, struct halAsyncDmaInputPara *in, uint32_t *src_devid,
    uint32_t *dst_devid, uint32_t *dir)
{
    struct DVattribute srcAttr, dstAttr;
    uint32_t host_id;
    int ret;

    ret = drvMemGetAttribute((DVdeviceptr)(uintptr_t)in->src, &srcAttr);
    if (ret != DRV_ERROR_NONE) {
        trs_err("Failed to get src attribute. (ret=%d; src=0x%llx)\n", ret, (uint64_t)(uintptr_t)in->src);
        return ret;
    }
    *src_devid = srcAttr.devId;

    ret = drvMemGetAttribute((DVdeviceptr)(uintptr_t)in->dst, &dstAttr);
    if (ret != DRV_ERROR_NONE) {
        trs_err("Failed to get dst attribute. (ret=%d; dst=0x%llx)\n", ret, (uint64_t)(uintptr_t)in->dst);
        return ret;
    }
    *dst_devid = dstAttr.devId;

    (void)halGetHostID(&host_id);
    if ((*src_devid == host_id) && (*dst_devid != host_id)) {
        *dir = TRS_ASYNC_HOST_TO_DEVICE;
    } else if ((*dst_devid == host_id) && (*src_devid != host_id)) {
        *dir = TRS_ASYNC_DEVICE_TO_HOST;
    } else if ((*dst_devid != host_id) && (*src_devid != host_id)) {
        *dir = TRS_ASYNC_DEVICE_TO_DEVICE;
    } else {
        trs_err("Invalid va. (src=0x%llx; srcMemType=%u; dst=0x%llx; dstMemType=%u)\n",
            (uint64_t)(uintptr_t)in->src, srcAttr.memType, (uint64_t)(uintptr_t)in->dst, dstAttr.memType);
        return DRV_ERROR_PARA_ERROR;
    }

    trs_debug("(src=0x%llx; src_devid=%u; srcMemType=%u; dst=0x%llx; dst_devid=%u; dstMemType=%u)\n",
        (uint64_t)(uintptr_t)in->src, *src_devid, srcAttr.memType,
        (uint64_t)(uintptr_t)in->dst, *dst_devid, dstAttr.memType);

    return 0;
}

static urma_opcode_t trs_get_async_urma_opcode(uint32_t dev_id, uint32_t dst_devid, uint32_t dir)
{
    if (dir == TRS_ASYNC_HOST_TO_DEVICE) {
        return URMA_OPC_READ;
    } else if (dir == TRS_ASYNC_DEVICE_TO_HOST) {
        return URMA_OPC_WRITE;
    } else if (dir == TRS_ASYNC_DEVICE_TO_DEVICE) {
        return (dev_id == dst_devid) ? URMA_OPC_READ : URMA_OPC_WRITE;
    } else {
        return URMA_OPC_LAST;
    }
}

static int trs_remote_fill_async_dma_wqe(uint32_t dev_id, struct trs_urma_ctx *urma_ctx, struct halAsyncDmaInputPara *in)
{
    struct trs_remote_fill_wqe_info wqe_fill_info = {0};
    urma_opcode_t urma_opcode;
    struct event_reply reply;
    urma_seg_t src_seg = {0};
    urma_seg_t dst_seg = {0};
    urma_token_t src_token, dst_token;
    uint32_t src_dev_id, dst_dev_id, dir;
    int result, ret;

    ret = trs_get_async_devid(dev_id, in, &src_dev_id, &dst_dev_id, &dir);
    if (ret != 0) {
        return ret;
    }

    ret = halMemGetSeg(urma_ctx->devid, (uint64_t)(uintptr_t)in->src, (uint64_t)in->len, &src_seg, &src_token);
    if (ret != 0) {
        trs_err("Failed to get src segment. (vaddr=0x%p; dir=%u; ret=%d)\n", in->src, in->dir, ret);
        return ret;
    }

    ret = halMemGetSeg(urma_ctx->devid, (uint64_t)(uintptr_t)in->dst, (uint64_t)in->len, &dst_seg, &dst_token);
    if (ret != 0) {
        trs_err("Failed to get dst segment. (vaddr=0x%p; dir=%u; ret=%d)\n", in->dst, in->dir, ret);
        return ret;
    }

    src_seg.ubva.va = (uint64_t)(uintptr_t)in->src;
    dst_seg.ubva.va = (uint64_t)(uintptr_t)in->dst;

    urma_opcode = trs_get_async_urma_opcode(dev_id, dst_dev_id, in->dir);
    wqe_fill_info.sq_id = in->sqId;
    wqe_fill_info.recv_dev_id = (urma_opcode == URMA_OPC_READ) ? src_dev_id : dst_dev_id;;
    wqe_fill_info.urma_opcode = urma_opcode;
    wqe_fill_info.dir = in->dir;
    wqe_fill_info.len = in->len;
    wqe_fill_info.src_ubva = (uint64_t)(uintptr_t)in->src;
    wqe_fill_info.dst_ubva = (uint64_t)(uintptr_t)in->dst;
    wqe_fill_info.recv_seg = (urma_opcode == URMA_OPC_READ) ? src_seg : dst_seg;
    wqe_fill_info.send_seg = (urma_opcode == URMA_OPC_READ) ? dst_seg : src_seg;
    wqe_fill_info.recv_token = (urma_opcode == URMA_OPC_READ) ? src_token : dst_token;
    wqe_fill_info.send_token = (urma_opcode == URMA_OPC_READ) ? dst_token : src_token;

    reply.buf_len = sizeof(result);
    reply.buf = (char *)&result;
    ret = trs_svm_mem_event_sync(dev_id, &wqe_fill_info, sizeof(struct trs_remote_fill_wqe_info),
        DRV_SUBEVENT_TRS_FILL_WQE_MSG, &reply);
    if (ret != DRV_ERROR_NONE) {
        trs_err("Remote fill wqe failed. (dev_id=%u; sq_id=%u)\n", dev_id, wqe_fill_info.sq_id);
        return ret;
    }

    return (drvError_t)result;
}

static int _trs_fill_async_dma_direct_wqe(uint32_t dev_id, struct trs_urma_ctx *urma_ctx, struct trs_async_ctx *async_ctx,
    struct trs_async_dma_wqe_info *wqe_info, struct halAsyncDmaInputPara *in)
{
    struct udma_sqe_ctl_tmp *sqe = (struct udma_sqe_ctl_tmp *)(void *)wqe_info->wqe;
    struct udma_wqe_sge_tmp *sge = (struct udma_wqe_sge_tmp *)((void *)wqe_info->wqe + sizeof(struct udma_sqe_ctl_tmp));
    urma_seg_t seg = {0};
    urma_token_t token;
    uint64_t send_addr, recv_addr;
    int ret;

    trs_get_async_send_recv_addr(dev_id, async_ctx, in, &send_addr, &recv_addr);

    ret = trs_get_segment(urma_ctx, recv_addr, (uint64_t)in->len, wqe_info, &seg, &token);
    if (ret != 0) {
        trs_err("Failed to get segment. (vaddr=0x%llx; dir=%u; ret=%d)\n", recv_addr, in->dir, ret);
        return ret;
    }
    sqe->rmt_token_value = token.token;
    sqe->rmt_obj_id = seg.token_id >> 8; // 8: token_id offset

    /* read/write: JFR segment */
    sqe->rmt_addr_l_or_token_id = ((uint32_t)recv_addr) & (0xFFFFFFFF);
    sqe->rmt_addr_h_or_token_value = (uint32_t)(recv_addr >> 32) &(0xFFFFFFFF); // 32: right bits

    /* read/write: JFS segment */
    sge->length = in->len;
    sge->token_id = 0;
    sge->va = send_addr;

    return 0;
}

static uint32_t trs_get_async_dwqe_opcode(uint32_t dev_id, struct trs_async_ctx *async_ctx, uint32_t dir)
{
    if (dir == TRS_ASYNC_HOST_TO_DEVICE) {
        return 0x6; // 0x6:read
    } else if (dir == TRS_ASYNC_DEVICE_TO_HOST) {
        return 0x3; // 0x3:write
    } else if (dir == TRS_ASYNC_DEVICE_TO_DEVICE) {
        return (dev_id == async_ctx->dst_devid) ? 0x6 : 0x3;  // 0x6:read; 0x3:write
    } else {
        return 0xFFFF; // 0xFFFF: invalid opcode
    }
}

static int trs_fill_async_dma_direct_wqe(uint32_t dev_id, struct trs_urma_ctx *urma_ctx, struct trs_async_ctx *async_ctx,
    struct trs_async_dma_wqe_info *wqe_info, struct halAsyncDmaInputPara *in)
{
    struct udma_sqe_ctl_tmp *sqe = (struct udma_sqe_ctl_tmp *)(void *)wqe_info->wqe;
    int ret = 0;

    sqe->sqe_bb_idx = async_ctx->pi;
    sqe->odr = 0x6; // send:0x4; write:0x6
    sqe->fence = 1;
    sqe->se = 1;
    sqe->cqe = 1;
    sqe->inline_en = 0;
    sqe->udf_flag = 0;
    sqe->nf = 0;
    sqe->token_en = 1;
    sqe->rmt_jetty_type = 0;
    sqe->owner = 0;
    sqe->target_hint = 0;
    sqe->inline_msg_len = 0;
    sqe->sge_num = 1;
    if (in->dir == TRS_ASYNC_DEVICE_TO_DEVICE) {
        struct trs_d2d_dev_ctx *d2d_dev_ctx = NULL;
        trs_dev_ctx_mutex_lock(wqe_info->remote_dev_id);
        d2d_dev_ctx = trs_getd2d_dev_ctx(wqe_info->remote_dev_id, dev_id);
        sqe->tpn = d2d_dev_ctx->tpn;
        trs_swap_endian((uint8_t *)sqe->rmt_eid, d2d_dev_ctx->dst_jetty_id.eid.raw, URMA_EID_SIZE);
        trs_dev_ctx_mutex_un_lock(wqe_info->remote_dev_id);
    } else {
        sqe->tpn = async_ctx->tpn;
        trs_swap_endian((uint8_t *)sqe->rmt_eid, urma_ctx->trs_jfr.jfr->urma_ctx->eid.raw, URMA_EID_SIZE);
    }
    sqe->opcode = trs_get_async_dwqe_opcode(dev_id, async_ctx, in->dir);
    ret = _trs_fill_async_dma_direct_wqe(dev_id, urma_ctx, async_ctx, wqe_info, in);
    if (ret != 0) {
        return ret;
    }

    return 0;
}

static int trs_fill_async_dma_wqe(uint32_t dev_id, struct sqcq_usr_info *sq_info, struct trs_async_dma_wqe_info *wqe_info,
    struct halAsyncDmaInputPara *in, struct halAsyncDmaOutputPara *out)
{
    struct trs_urma_ctx *urma_ctx = sq_info->urma_ctx;
    bool is_wqe_sink = trs_is_async_jetty_wqe_sink(sq_info->flag);
    struct trs_async_ctx *async_ctx = trs_get_async_ctx(urma_ctx, in->dir, wqe_info->remote_dev_id);
    uint32_t pi = async_ctx->pi;
    int ret = 0;

    async_ctx->pi = (async_ctx->pi + 1) % trs_get_async_pi_ci_max(is_wqe_sink);
    if (async_ctx->pi == async_ctx->ci) {
        trs_err("Jetty full. (pi=%u; ci=%u)\n", async_ctx->pi, async_ctx->ci);
        async_ctx->pi = pi;
        trs_put_async_ctx(urma_ctx, in->dir, async_ctx);
        return DRV_ERROR_NO_RESOURCES;
    }

    if (is_wqe_sink == false) {
        ret = trs_fill_async_dma_direct_wqe(dev_id, urma_ctx, async_ctx, wqe_info, in);
    } else {
        ret = trs_remote_fill_async_dma_wqe(dev_id, urma_ctx, in);
    }

    if (ret != 0) {
        async_ctx->pi = pi;
        trs_put_async_ctx(urma_ctx, in->dir, async_ctx);
        return ret;
    }

    out->jettyId = async_ctx->src_jetty_id.id;
    if (in->dir == TRS_ASYNC_DEVICE_TO_DEVICE) {
        out->functionId = async_ctx->func_id;
        out->dieId = async_ctx->die_id;
    } else {
        out->functionId = urma_ctx->func_id;
        out->dieId = urma_ctx->die_id;
    }

    trs_put_async_ctx(urma_ctx, in->dir, async_ctx);
    return 0;
}

static int trs_fill_h2d_async_dma_wqe(struct trs_d2d_async_devid *async_devid, struct sqcq_usr_info *sq_info,
    struct trs_async_dma_wqe_info *wqe_info, struct halAsyncDmaInputPara *in, struct halAsyncDmaOutputPara *out)
{
    if (async_devid->dst_devid != async_devid->local_devid) {
        trs_err("Invalid devid. (local_devid=%u; dst_devid=%u; dst=0x%llx)\n",
            async_devid->local_devid, async_devid->dst_devid, (uint64_t)(uintptr_t)in->dst);
        return DRV_ERROR_PARA_ERROR;
    }
    return trs_fill_async_dma_wqe(async_devid->local_devid, sq_info, wqe_info, in, out);
}

static int trs_fill_d2h_async_dma_wqe(struct trs_d2d_async_devid *async_devid, struct sqcq_usr_info *sq_info,
    struct trs_async_dma_wqe_info *wqe_info, struct halAsyncDmaInputPara *in, struct halAsyncDmaOutputPara *out)
{
    if (async_devid->src_devid != async_devid->local_devid) {
        trs_err("Invalid devid. (local_devid=%u; src_devid=%u; src=0x%llx)\n",
            async_devid->local_devid, async_devid->src_devid, (uint64_t)(uintptr_t)in->src);
        return DRV_ERROR_PARA_ERROR;
    }
    return trs_fill_async_dma_wqe(async_devid->local_devid, sq_info, wqe_info, in, out);
}

static int trs_alloc_device_jetty(uint32_t dev_id, struct trs_d2d_send_info *alloc_info, struct trs_d2d_sync_info *sync_msg)
{
    struct event_reply reply;
    int ret, result;

    reply.buf_len = sizeof(struct trs_d2d_sync_info) + sizeof(int);
    reply.buf = (char *)malloc(reply.buf_len);
    if (reply.buf == NULL) {
        trs_err("Malloc reply buffer failed. (size=%u)\n", reply.buf_len);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    ret = trs_local_mem_event_sync(dev_id, alloc_info, sizeof(struct trs_d2d_send_info),
        DRV_SUBEVENT_TRS_CREATE_ASYNC_JETTY_MSG, &reply);
    result = DRV_EVENT_REPLY_BUFFER_RET(reply.buf);
    if ((ret != DRV_ERROR_NONE) || (result != 0)) {
#ifndef EMU_ST
        trs_err("Failed to sync event. (dev_id=%u; ret=%d; result=%d)\n", dev_id, ret, result);
        free(reply.buf);
        return (ret != DRV_ERROR_NONE) ? ret : result;
#endif
    }
    (void)memcpy_s(sync_msg, sizeof(struct trs_d2d_sync_info), DRV_EVENT_REPLY_BUFFER_DATA_PTR(reply.buf),
        sizeof(struct trs_d2d_sync_info));

    free(reply.buf);
    return 0;
}

static int trs_free_device_jetty(uint32_t dev_id, struct trs_d2d_send_info *free_info)
{
    struct event_reply reply;
    int result, ret;

    reply.buf_len = sizeof(result);
    reply.buf = (char *)&result;
    ret = trs_local_mem_event_sync(dev_id, free_info, sizeof(struct trs_d2d_send_info),
        DRV_SUBEVENT_TRS_DESTROY_ASYNC_JETTY_MSG, &reply);
    if ((ret != 0) || (result != 0)) {
        trs_err("Failed to sync destroy async jetty. (ret=%d; result=%d; dev_id=%u)\n", ret, result, dev_id);
        return (ret != 0) ? ret : result;
    }

    return 0;
}

static int trs_import_remote_jetty(uint32_t dev_id, uint32_t remote_dev_id, uint32_t sq_id,
    struct trs_d2d_sync_info *sync_msg)
{
    struct trs_import_jetty_info info = {0};
    struct event_reply reply;
    int result, ret;

    (void)memcpy_s(info.jetty_raw, URMA_EID_SIZE, sync_msg->jetty_id.eid.raw, URMA_EID_SIZE);
    info.jetty_uasid = sync_msg->jetty_id.uasid;
    info.jetty_id = sync_msg->jetty_id.id;
    info.token_val = sync_msg->token_value;
    info.sq_id = sq_id;
    info.recv_dev_id = remote_dev_id;
    reply.buf_len = sizeof(struct trs_d2d_sync_info) + sizeof(int);
    reply.buf = (char *)malloc(reply.buf_len);
    if (reply.buf == NULL) {
        trs_err("Malloc reply buffer failed.\n");
        return DRV_ERROR_OUT_OF_MEMORY;
    };

    ret = trs_local_mem_event_sync(dev_id, &info, sizeof(struct trs_import_jetty_info),
        DRV_SUBEVENT_TRS_IMPORT_ASYNC_JETTY_MSG, &reply);
    result = DRV_EVENT_REPLY_BUFFER_RET(reply.buf);
    if ((ret != 0) || (result != 0)) {
        free(reply.buf);
        trs_err("Failed to sync import remote jetty. (ret=%d; result=%d; dev_id=%u; sq_id=%u)\n",
            ret, result, dev_id, sq_id);
        return (ret != 0) ? ret : result;
    }
    (void)memcpy_s(sync_msg, sizeof(struct trs_d2d_sync_info), DRV_EVENT_REPLY_BUFFER_DATA_PTR(reply.buf),
        sizeof(struct trs_d2d_sync_info));
    free(reply.buf);

    return 0;
}

static int trs_create_local_d2d_jetty(struct trs_d2d_async_devid *d2d_async_devid, uint32_t sq_id, uint32_t flag,
    struct trs_async_ctx *async_ctx)
{
    struct trs_d2d_sync_info sync_msg;
    struct trs_d2d_send_info alloc_info = {0};
    int ret;
    alloc_info.flag = flag;
    alloc_info.pos = TRS_ASYNC_SEND_SIDE;
    alloc_info.sq_id = sq_id;
    alloc_info.recv_dev_id = d2d_async_devid->remote_devid;
    (void)memcpy_s(alloc_info.eid, sizeof(alloc_info.eid), d2d_async_devid->local_eid, sizeof(d2d_async_devid->local_eid));
    ret = trs_alloc_device_jetty(d2d_async_devid->local_devid, &alloc_info, &sync_msg);
    if (ret != 0) {
        trs_err("Failed to alloc device jetty. (local_devid=%u; sq_id=%u)\n", d2d_async_devid->local_devid, sq_id);
        return ret;
    }
    async_ctx->src_jetty_id = sync_msg.jetty_id;
    async_ctx->func_id = sync_msg.func_id;
    async_ctx->die_id = sync_msg.die_id;
    return 0;
}

static int trs_destroy_local_d2d_jetty(uint32_t local_devid, uint32_t remote_devid, uint32_t sq_id)
{
    struct trs_d2d_send_info free_info = {0};
    free_info.pos = TRS_ASYNC_SEND_SIDE;
    free_info.sq_id = sq_id;
    free_info.recv_dev_id = remote_devid;
    return trs_free_device_jetty(local_devid, &free_info);
}

static int trs_create_remote_d2d_jetty(struct trs_d2d_async_devid *d2d_async_devid, uint32_t sq_id, uint32_t flag,
    struct trs_d2d_dev_ctx *d2d_dev_ctx)
{
    struct trs_d2d_sync_info sync_msg;
    struct trs_d2d_send_info alloc_info = {0};
    int ret;

    alloc_info.flag = flag;
    alloc_info.pos = TRS_ASYNC_RECV_SIDE;
    alloc_info.send_dev_id = d2d_async_devid->local_devid;
    (void)memcpy_s(alloc_info.eid, sizeof(alloc_info.eid), d2d_async_devid->remote_eid, sizeof(d2d_async_devid->remote_eid));
    ret = trs_alloc_device_jetty(d2d_async_devid->remote_devid, &alloc_info, &sync_msg);
    if (ret != 0) {
        trs_err("Failed to alloc remote jetty. (ret=%d; remote_devid=%u; flag=0x%x)\n",
            ret, d2d_async_devid->remote_devid, flag);
        return ret;
    }
    d2d_dev_ctx->dst_jetty_id = sync_msg.jetty_id;
    d2d_dev_ctx->token_value = sync_msg.token_value;

    ret = trs_import_remote_jetty(d2d_async_devid->local_devid, d2d_async_devid->remote_devid, sq_id, &sync_msg);
    if (ret != 0) {
        trs_err("Failed to import remote jetty. (ret=%d; import_devid=%u; sq_id=%u; jetty_id=%u)\n",
            ret, d2d_async_devid->local_devid, sq_id, sync_msg.jetty_id.id);
        (void)trs_free_device_jetty(d2d_async_devid->remote_devid, &alloc_info);
        return ret;
    }
    d2d_dev_ctx->tpn = sync_msg.tpn;

    trs_debug("Create jetty success. (remote_devid=%u; jetty_id=%u; flag=0x%x)\n",
        d2d_async_devid->remote_devid, d2d_dev_ctx->dst_jetty_id.id, flag);
    return 0;
}

int trs_destroy_remote_d2d_jetty(uint32_t dev_id)
{
    struct trs_d2d_send_info info = {0};
    uint32_t local_devid;
    int ret;

    info.pos = TRS_ASYNC_RECV_SIDE;
    trs_dev_ctx_mutex_lock(dev_id);
    for (local_devid = 0; local_devid < TRS_DEV_NUM; local_devid++) {
        struct trs_d2d_dev_ctx *d2d_dev_ctx = trs_getd2d_dev_ctx(dev_id, local_devid);
        if (d2d_dev_ctx == NULL) {
            continue;
        }
        info.send_dev_id = local_devid;
        ret = trs_free_device_jetty(dev_id, &info);
        if (ret != 0) {
            trs_dev_ctx_mutex_un_lock(dev_id);
            trs_err("Failed to free remote jetty. (dev_id=%u; ret=%d)\n", dev_id, ret);
            return ret;
        }
        free(d2d_dev_ctx);
        trs_setd2d_dev_ctx(dev_id, local_devid, NULL);
    }
    trs_dev_ctx_mutex_un_lock(dev_id);
    return 0;
}

static int trs_d2d_async_ctx_create(struct trs_d2d_async_devid *d2d_async_devid, struct trs_d2d_ctx_list *d2d_ctx_list,
    uint32_t sq_id, uint32_t flag)
{
    uint32_t local_devid = d2d_async_devid->local_devid;
    uint32_t remote_devid = d2d_async_devid->remote_devid;
    struct trs_async_ctx *async_ctx = NULL;
    struct trs_async_ctx *async_ctx_tmp = NULL;
    int ret;

    (void)pthread_mutex_lock(&d2d_ctx_list->d2d_mutex);
    async_ctx = trs_get_d2d_async_ctx(remote_devid, d2d_ctx_list);
    if (async_ctx == NULL) {
        async_ctx_tmp = (struct trs_async_ctx *)calloc(1, sizeof(struct trs_async_ctx));
        if (async_ctx_tmp == NULL) {
            (void)pthread_mutex_unlock(&d2d_ctx_list->d2d_mutex);
            trs_err("Alloc async_ctx failed. (local_devid=%u)\n", local_devid);
            return DRV_ERROR_OUT_OF_MEMORY;
        }
        ret = trs_create_local_d2d_jetty(d2d_async_devid, sq_id, flag, async_ctx_tmp);
        if (ret != 0) {
            free(async_ctx_tmp);
            (void)pthread_mutex_unlock(&d2d_ctx_list->d2d_mutex);
            trs_err("Failed to create local jetty. (local_devid=%u; ret=%d; flag=0x%x)\n", local_devid, ret, flag);
            return ret;
        }
        async_ctx_tmp->src_devid = d2d_async_devid->src_devid;
        async_ctx_tmp->dst_devid = d2d_async_devid->dst_devid;
        async_ctx_tmp->recv_devid = remote_devid;
        drv_user_list_add_tail(&async_ctx_tmp->node, &d2d_ctx_list->head);
        trs_debug("Create local jetty success. (local_devid=%u; flag=0x%x; jetty_id=%u; fun_id=%u; die_id=%u)\n",
            local_devid, flag, async_ctx_tmp->src_jetty_id.id, async_ctx_tmp->func_id, async_ctx_tmp->die_id);
    }
    (void)pthread_mutex_unlock(&d2d_ctx_list->d2d_mutex);

    trs_dev_ctx_mutex_lock(remote_devid);
    if (trs_getd2d_dev_ctx(remote_devid, local_devid) == NULL) {
        struct trs_d2d_dev_ctx *d2d_dev_ctx = (struct trs_d2d_dev_ctx *)calloc(1, sizeof(struct trs_d2d_dev_ctx));
        if (d2d_dev_ctx == NULL) {
            trs_err("Alloc d2d_dev_ctx failed. (remote_devid=%u)\n", remote_devid);
            ret = DRV_ERROR_OUT_OF_MEMORY;
            goto free_local;
        }
        ret = trs_create_remote_d2d_jetty(d2d_async_devid, sq_id, flag, d2d_dev_ctx);
        if (ret != 0) {
            trs_err("Failed to create remote jetty. (local_devid=%u; remote_devid=%u; ret=%d; flag=0x%x)\n",
                local_devid, remote_devid, ret, flag);
            free(d2d_dev_ctx);
            goto free_local;
        }
        trs_setd2d_dev_ctx(remote_devid, local_devid, (void *)d2d_dev_ctx);
        trs_debug("Create remote jetty success. (remote_devid=%u; sq_id=%u; flag=0x%x; jetty_id=%u)\n",
            remote_devid, sq_id, flag, d2d_dev_ctx->dst_jetty_id.id);
    }

    (void)trs_dev_ctx_mutex_un_lock(remote_devid);
    return 0;

free_local:
    trs_dev_ctx_mutex_un_lock(remote_devid);
    if (async_ctx_tmp != NULL) {
        free(async_ctx_tmp);
        (void)trs_destroy_local_d2d_jetty(local_devid, remote_devid, sq_id);
    }
    return ret;
}

int trs_d2d_async_ctx_destroy(uint32_t dev_id, uint32_t sq_id, struct trs_d2d_ctx_list *d2d_ctx_list)
{
    struct trs_d2d_send_info info = {0};
    struct trs_async_ctx *async_ctx = NULL;
    struct list_head *node, *next;
    int ret;

    info.pos = TRS_ASYNC_SEND_SIDE;
    info.sq_id = sq_id;
    list_for_each_safe(node, next, &d2d_ctx_list->head) {
        async_ctx = list_entry(node, struct trs_async_ctx, node);
        info.recv_dev_id = async_ctx->recv_devid;
        ret = trs_destroy_local_d2d_jetty(dev_id, async_ctx->recv_devid, sq_id);
        if (ret != 0) {
            trs_err("Failed to destroy local jetty. (dev_id=%u; sq_id=%u; ret=%d)\n", dev_id, sq_id, ret);
            return ret;
        }
        trs_debug("Destroy d2d jetty success. (dev_id=%u; sq_id=%u; recv_devid=%u; jetty_id=%u)\n",
            dev_id, sq_id, info.recv_dev_id, async_ctx->src_jetty_id.id);
        drv_user_list_del(&async_ctx->node);
        free(async_ctx);
    }

    return 0;
}

static drvError_t trs_d2d_check_dev_id(uint32_t dev_id, uint32_t src_dev_id, uint32_t dst_dev_id)
{
    uint32_t host_dev_id;

    if ((dev_id != src_dev_id) && (dev_id != dst_dev_id)) {
        trs_err("Invalid dev_id. (dev_id=%u; src_dev_id=%u; dst_dev_id=%u)\n", dev_id, src_dev_id, dst_dev_id);
        return DRV_ERROR_PARA_ERROR;
    }

    (void)halGetHostID(&host_dev_id);
    if ((src_dev_id == host_dev_id) || (dst_dev_id == host_dev_id)) {
        trs_err("Invalid dev_id. (host_dev_id=%u; src_dev_id=%u; dst_dev_id=%u)\n", host_dev_id, src_dev_id, dst_dev_id);
        return DRV_ERROR_PARA_ERROR;
    }

    if (src_dev_id == dst_dev_id) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    return DRV_ERROR_NONE;
}

static int trs_fill_d2d_async_dma_wqe(struct trs_d2d_async_devid *async_devid, struct sqcq_usr_info *sq_info,
    struct trs_async_dma_wqe_info *wqe_info, struct halAsyncDmaInputPara *in, struct halAsyncDmaOutputPara *out)
{
    struct trs_urma_ctx *urma_ctx = (struct trs_urma_ctx *)sq_info->urma_ctx;
    uint32_t dev_id = async_devid->local_devid;
    int ret;

    ret = trs_d2d_check_dev_id(dev_id, async_devid->src_devid, async_devid->dst_devid);
    if (ret != 0) {
        return ret;
    }

    ret = trs_d2d_async_ctx_create(async_devid, &urma_ctx->d2d_ctx_list, in->sqId, sq_info->flag);
    if (ret != 0) {
        trs_err("Failed to create d2d urma_ctx. (ret=%d; dev_id=%u; sq_id=%u)\n",
            ret, dev_id, in->sqId);
        return ret;
    }

    wqe_info->remote_dev_id = async_devid->remote_devid;
    ret = trs_fill_async_dma_wqe(dev_id, sq_info, wqe_info, in, out);
    if (ret != 0) {
        (void)trs_d2d_async_ctx_destroy(dev_id, in->sqId, &urma_ctx->d2d_ctx_list);
        trs_err("Failed to fill d2d async dma wqe. (ret=%d; dev_id=%u)\n", ret, dev_id);
    }

    return ret;
}

static int (*const trs_async_dma_wqe_fill_handles[TRS_ASYNC_MAX_DIR])(struct trs_d2d_async_devid *async_devid,
    struct sqcq_usr_info *sq_info, struct trs_async_dma_wqe_info *wqe_info, 
    struct halAsyncDmaInputPara *in, struct halAsyncDmaOutputPara *out) = {
    [TRS_ASYNC_HOST_TO_DEVICE] = trs_fill_h2d_async_dma_wqe,
    [TRS_ASYNC_DEVICE_TO_HOST] = trs_fill_d2h_async_dma_wqe,
    [TRS_ASYNC_DEVICE_TO_DEVICE] = trs_fill_d2d_async_dma_wqe,
};

drvError_t trs_async_dma_wqe_create(uint32_t dev_id, struct halAsyncDmaInputPara *in, struct halAsyncDmaOutputPara *out)
{
    struct sqcq_usr_info *sq_info = NULL;
    struct trs_async_dma_wqe_info *wqe_info = NULL;
    struct trs_d2d_async_devid async_devid = {0};
    uint32_t src_devid, dst_devid, remote_devid;
    uint32_t dir;
    int ret;

    if (in->dst == NULL) {
        trs_err("Invalid dst addr. (dev_id=%u; ts_id=%u)\n", dev_id, in->tsId);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = trs_get_async_devid(dev_id, in, &src_devid, &dst_devid, &dir);
    if (ret != 0) {
        return ret;
    }
    remote_devid = (src_devid == dev_id) ? dst_devid : src_devid;
    trs_pack_d2d_async_devid(dev_id, remote_devid, src_devid, dst_devid, &async_devid);
    trs_debug("(local_devid=%u; remote_devid=%u; src_devid=%u; dst_devid=%u)\n",
        dev_id, remote_devid, src_devid, dst_devid);

    wqe_info = (struct trs_async_dma_wqe_info *)calloc(1, sizeof(struct trs_async_dma_wqe_info));
    if (wqe_info == NULL) {
        trs_err("WqeInfo malloc failed. (dev_id=%u)\n", dev_id);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    sq_info = trs_get_sq_info(dev_id, in->tsId, in->type, in->sqId);
    if ((sq_info == NULL) || (sq_info->urma_ctx == NULL)) {
        trs_err("Invalid para. (dev_id=%u; ts_id=%u; type=%d; sq_id=%u)\n", dev_id, in->tsId, in->type, in->sqId);
        free(wqe_info);
        return DRV_ERROR_INVALID_VALUE;
    }

    in->dir = dir;
    ret = trs_async_dma_wqe_fill_handles[in->dir](&async_devid, sq_info, wqe_info, in, out);
    if (ret != 0) {
        trs_err("Failed to fill async dma wqe. (ret=%d; dev_id=%u; sq_id=%u)\n", ret, dev_id, in->sqId);
        free(wqe_info);
        return ret;
    }
    wqe_info->dir = dir;

    if (trs_is_async_jetty_wqe_sink(sq_info->flag) == false) {
        wqe_info->dir = in->dir;
        out->size = STARS_ASYNC_DMA_WQE_SIZE;
        out->wqe = wqe_info->wqe;
    } else {
        free(wqe_info);
    }

    trs_debug("Create async dma wqe. (dev_id=%u; dir=%u; sq_id=%u; dieid=%u; func_id=%u; jetty_id=%u)\n",
        dev_id, in->dir, in->sqId, out->dieId, out->functionId, out->jettyId);

    return DRV_ERROR_NONE;
}

drvError_t trs_async_dma_wqe_destory(uint32_t dev_id, struct halAsyncDmaDestoryPara *para)
{
    struct sqcq_usr_info *sq_info = NULL;
    struct trs_urma_ctx *urma_ctx = NULL;
    struct trs_async_dma_wqe_info *wqe_info = NULL;
    struct trs_async_ctx *async_ctx = NULL;

    if ((para->wqe == NULL) || (para->size != STARS_ASYNC_DMA_WQE_SIZE)) {
        trs_err("Invalid para. (dev_id=%u; wqe=0x%llx; size=%u)\n", dev_id, (uint64_t)(uintptr_t)para->wqe, para->size);
        return DRV_ERROR_INVALID_VALUE;
    }

    sq_info = trs_get_sq_info(dev_id, para->tsId, DRV_NORMAL_TYPE, para->sqId);
    if (sq_info == NULL) {
        trs_err("Invalid para. (dev_id=%u; ts_id=%u; sq_id=%u)\n", dev_id, para->tsId, para->sqId);
        return DRV_ERROR_INVALID_VALUE;
    }

    urma_ctx = (struct trs_urma_ctx *)sq_info->urma_ctx;
    wqe_info = (struct trs_async_dma_wqe_info *)(void *)para->wqe;
    if ((wqe_info->flag == 1) && (wqe_info->async_tseg != NULL)) {
        (void)urma_unregister_seg(wqe_info->async_tseg);
        trs_debug("Unregister seg. (dev_id=%u)\n", dev_id);
    }
    async_ctx = trs_get_async_ctx(urma_ctx, wqe_info->dir, wqe_info->remote_dev_id);
    if (async_ctx == NULL) {
        trs_err("Not create. (dev_id=%u; sq_id=%u)\n", dev_id, para->sqId);
        return DRV_ERROR_UNINIT;
    }
    async_ctx->ci = (async_ctx->ci + 1) % trs_get_async_pi_ci_max(false);
    trs_debug("Async dma wqe destroy. (dev_id=%u; sq_id=%u; ci=%u; dir=%u)\n",
        dev_id, para->sqId, async_ctx->ci, wqe_info->dir);
    trs_put_async_ctx(urma_ctx, wqe_info->dir, async_ctx);
    free(wqe_info);

    return DRV_ERROR_NONE;
}
