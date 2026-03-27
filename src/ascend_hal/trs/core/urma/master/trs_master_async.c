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
#include <limits.h>
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
#include "trs_res.h"

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
    seg_cfg.token_id = trs_get_urma_proc_ctx(urma_ctx->devid)->token_id[TRS_URMA_SEG_TOKEN_ALLOC_DIVIDE - 1];
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

static void trs_get_async_send_recv_addr(struct trs_async_devid *async_devid,
    struct trs_async_dma_input_para *in, uint64_t *send_addr, uint64_t *recv_addr)
{
    if (in->dir == TRS_ASYNC_HOST_TO_DEVICE) {
        *send_addr = (uint64_t)(uintptr_t)in->async_normal_in->dst;
        *recv_addr = (uint64_t)(uintptr_t)in->async_normal_in->src;
    } else if (in->dir == TRS_ASYNC_DEVICE_TO_HOST) {
        *send_addr = (uint64_t)(uintptr_t)in->async_normal_in->src;
        *recv_addr = (uint64_t)(uintptr_t)in->async_normal_in->dst;
    } else { /* TRS_ASYNC_DEVICE_TO_DEVICE) */
        if (async_devid->local_devid == async_devid->dst_devid) {
            *send_addr = (uint64_t)(uintptr_t)in->async_normal_in->dst;
            *recv_addr = (uint64_t)(uintptr_t)in->async_normal_in->src;
        } else {
            *send_addr = (uint64_t)(uintptr_t)in->async_normal_in->src;
            *recv_addr = (uint64_t)(uintptr_t)in->async_normal_in->dst;
        }
    }
}

static struct trs_async_ctx *trs_get_async_ctx(struct trs_urma_ctx *urma_ctx, uint32_t dir,
    enum trs_async_dma_type async_dma_type)
{
    if (dir == TRS_ASYNC_DEVICE_TO_DEVICE) {
        return &urma_ctx->d2d_async_ctx;
    } else {
        if ((async_dma_type == TRS_ASYNC_DMA_TYPE_2D) || (async_dma_type == TRS_ASYNC_DMA_TYPE_BATCH)) {
            return &urma_ctx->batch_2d_async_ctx;
        } else {
            return &urma_ctx->async_ctx;
        }
    }
}

static void trs_get_async_input_src_dst_len_array(struct trs_async_dma_input_para *in, unsigned long long **src_array,
    unsigned long long **dst_array, unsigned long long **src_len_array, unsigned long long **dst_len_array)
{
    enum trs_async_dma_type async_type = in->async_dma_type;

    if (async_type == TRS_ASYNC_DMA_TYPE_2D) {
        *src_array = (unsigned long long *)&in->async_2d_in->src;
        *dst_array = (unsigned long long *)&in->async_2d_in->dst;
    } else {
        *src_array = (async_type == TRS_ASYNC_DMA_TYPE_BATCH) ? in->async_batch_in->src :
            (unsigned long long *)&in->async_normal_in->src;
        *dst_array = (async_type == TRS_ASYNC_DMA_TYPE_BATCH) ? in->async_batch_in->dst :
            (unsigned long long *)&in->async_normal_in->dst;
    }

    if (async_type == TRS_ASYNC_DMA_TYPE_2D) {
        *src_len_array = &in->async_2d_in->width;
        *dst_len_array = &in->async_2d_in->width;
    } else {
        *src_len_array = (async_type == TRS_ASYNC_DMA_TYPE_BATCH) ? in->async_batch_in->len :
            (unsigned long long *)&in->async_normal_in->len;
        *dst_len_array = *src_len_array;
    }
}

static inline int trs_check_batch_async_devid(unsigned long long va, uint32_t dev_id)
{
    struct DVattribute vaAttr = {0};
    int ret = 0;

    ret = drvMemGetAttribute((DVdeviceptr)va, &vaAttr);
    if (ret != DRV_ERROR_NONE) {
        trs_err("Failed to get va attribute. (ret=%d; va=0x%llx)\n", ret, va);
        return ret;
    }

    return (vaAttr.devId == dev_id) ? 0 : DRV_ERROR_PARA_ERROR;
}

static drvError_t trs_check_d2d_async_devid(uint32_t dev_id, uint32_t src_dev_id, uint32_t dst_dev_id)
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

static int trs_check_async_devid_dir(struct trs_async_dma_input_para *in, uint32_t local_devid, uint32_t src_devid,
    uint32_t dst_devid, uint32_t dir)
{
    enum trs_async_dma_type async_type = in->async_dma_type;
    int ret = 0;

    if (async_type == TRS_ASYNC_DMA_TYPE_BATCH) {
        unsigned long long i;
        for (i = 1; i < in->async_batch_in->count; i++) {
            if ((trs_check_batch_async_devid(in->async_batch_in->src[i], src_devid) != DRV_ERROR_NONE) ||
                (trs_check_batch_async_devid(in->async_batch_in->dst[i], dst_devid) != DRV_ERROR_NONE)) {
                trs_err("Addr invalid. (src_addr=0x%llx; dst_addr=0x%llx; src_devid=%u; dst_devid=%u; index=%u)\n",
                    in->async_batch_in->src[i], in->async_batch_in->dst[i], src_devid, dst_devid, i);
                return DRV_ERROR_PARA_ERROR;
            }
        }
    }

    if (((async_type == TRS_ASYNC_DMA_TYPE_BATCH) || (async_type == TRS_ASYNC_DMA_TYPE_2D)) &&
        (dir == TRS_ASYNC_DEVICE_TO_DEVICE)) {
        trs_debug("Not support 2d/batch d2d cpy");
        return DRV_ERROR_NOT_SUPPORT;
    }

    if ((async_type == TRS_ASYNC_DMA_TYPE_SQE_UPDATE) && (dir != TRS_ASYNC_HOST_TO_DEVICE)) {
        trs_err("Invalid async_type or dir. (async_type=%d; dir=%u)\n", async_type, dir);
        return DRV_ERROR_PARA_ERROR;
    }

    if (dir == TRS_ASYNC_HOST_TO_DEVICE) {
        if (dst_devid != local_devid) {
            trs_err("Invalid devid. (dst_devid=%u; local_devid=%u)\n", dst_devid, local_devid);
            return DRV_ERROR_PARA_ERROR;
        }
    } else if (dir == TRS_ASYNC_DEVICE_TO_HOST) {
        if (src_devid != local_devid) {
            trs_err("Invalid devid. (src_devid=%u; local_devid=%u)\n", src_devid, local_devid);
            return DRV_ERROR_PARA_ERROR;
        }
    } else if (dir == TRS_ASYNC_DEVICE_TO_DEVICE) {
        ret = trs_check_d2d_async_devid(local_devid, src_devid, dst_devid);
        if (ret != 0) {
            return ret;
        }
    }

    return 0;
}

static int trs_get_check_async_devid_dir(uint32_t dev_id, struct trs_async_dma_input_para *in,
    struct trs_async_devid *async_devid, uint32_t *dir)
{
    enum trs_async_dma_type async_type = in->async_dma_type;
    struct DVattribute srcAttr, dstAttr;
    unsigned long long *src_array, *dst_array;
    unsigned long long *src_len_array, *dst_len_array;
    uint32_t host_id;
    int ret;

    trs_get_async_input_src_dst_len_array(in, &src_array, &dst_array, &src_len_array, &dst_len_array);

    ret = drvMemGetAttribute((DVdeviceptr)src_array[0], &srcAttr);
    if (ret != DRV_ERROR_NONE) {
        trs_err("Failed to get src attribute. (ret=%d; src=0x%llx)\n", ret, src_array[0]);
        return ret;
    }

    if (in->async_dma_type == TRS_ASYNC_DMA_TYPE_SQE_UPDATE) {
        dstAttr.devId = dev_id;
    } else {
        ret = drvMemGetAttribute((DVdeviceptr)dst_array[0], &dstAttr);
        if (ret != DRV_ERROR_NONE) {
            trs_err("Failed to get dst attribute. (ret=%d; dst=0x%llx)\n", ret, dst_array[0]);
            return ret;
        }
    }

    (void)halGetHostID(&host_id);
    if ((srcAttr.devId == host_id) && (dstAttr.devId != host_id)) {
        *dir = TRS_ASYNC_HOST_TO_DEVICE;
    } else if ((dstAttr.devId == host_id) && (srcAttr.devId != host_id)) {
        *dir = TRS_ASYNC_DEVICE_TO_HOST;
    } else if ((dstAttr.devId != host_id) && (srcAttr.devId != host_id)) {
        *dir = TRS_ASYNC_DEVICE_TO_DEVICE;
    } else {
        trs_err("Invalid va. (src=0x%llx; srcMemType=%u; dst=0x%llx; dstMemType=%u; asyncType=%d)\n",
            src_array[0], srcAttr.memType, dst_array[0], dstAttr.memType, async_type);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = trs_check_async_devid_dir(in, dev_id, srcAttr.devId, dstAttr.devId, *dir);
    if (ret != 0) {
        return ret;
    }

    trs_pack_async_devid(dev_id, srcAttr.devId, dstAttr.devId, async_devid);
    trs_debug("(src=0x%llx; src_devid=%u; srcType=%u; dst=0x%llx; dst_devid=%u; dstType=%u; asyncType=%d; devid=%u)\n",
        src_array[0], async_devid->src_devid, srcAttr.memType, dst_array[0], async_devid->dst_devid, dstAttr.memType,
        async_type, dev_id);
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

static int trs_fill_remote_wr_batch_info(struct trs_urma_ctx *urma_ctx, struct trs_async_dma_input_para *in,
    struct trs_remote_fill_wqe_info *wqe_fill_info)
{
    urma_opcode_t urma_opcode = wqe_fill_info->urma_opcode;
    unsigned long long *src_array, *dst_array;
    unsigned long long *src_len_array, *dst_len_array;
    int ret = 0;
    int i;

    trs_get_async_input_src_dst_len_array(in, &src_array, &dst_array, &src_len_array, &dst_len_array);

    for (i = 0; i < wqe_fill_info->batch_num; i++) {
        enum trs_async_dma_type async_type = in->async_dma_type;
        uint64_t src = (async_type == TRS_ASYNC_DMA_TYPE_2D) ? (src_array[0] + i * in->async_2d_in->spitch) : src_array[i];
        uint64_t dst = (async_type == TRS_ASYNC_DMA_TYPE_2D) ? (dst_array[0] + i * in->async_2d_in->dpitch) : dst_array[i];
        uint64_t len = (async_type == TRS_ASYNC_DMA_TYPE_2D) ? in->async_2d_in->width: src_len_array[i];
        urma_token_t src_token, dst_token;
        urma_seg_t src_seg = {0};
        urma_seg_t dst_seg = {0};

        ret = halMemGetSeg(urma_ctx->devid, src, len, &src_seg, &src_token);
        if (ret != 0) {
            trs_err("Failed to get src segment. (vaddr=0x%p; dir=%u; ret=%d)\n", src, in->dir, ret);
            return ret;
        }

        ret = halMemGetSeg(urma_ctx->devid, dst, len, &dst_seg, &dst_token);
        if (ret != 0) {
            trs_err("Failed to get dst segment. (vaddr=0x%p; dir=%u; ret=%d)\n", dst, in->dir, ret);
            return ret;
        }

        /* svm return the base seg va, need set to real va */
        src_seg.ubva.va = src;
        dst_seg.ubva.va = dst;

        wqe_fill_info->wr_addr[i].len = len;
        wqe_fill_info->wr_addr[i].src_ubva = src;
        wqe_fill_info->wr_addr[i].dst_ubva = dst;
        wqe_fill_info->wr_addr[i].recv_seg = (urma_opcode == URMA_OPC_READ) ? src_seg : dst_seg;
        wqe_fill_info->wr_addr[i].send_seg = (urma_opcode == URMA_OPC_READ) ? dst_seg : src_seg;
        wqe_fill_info->wr_addr[i].recv_token = (urma_opcode == URMA_OPC_READ) ? src_token : dst_token;
    }

    return ret;
}

static int trs_remote_fill_async_dma_wqe(struct trs_async_devid *async_devid, struct trs_urma_ctx *urma_ctx,
    struct trs_async_dma_input_para *in)
{
    struct trs_remote_fill_wqe_info *wqe_fill_info = NULL;
    unsigned long long wqe_fill_info_size = 0;
    uint32_t batch_num = 0;
    urma_opcode_t urma_opcode;
    struct event_reply reply;
    uint32_t dev_id = async_devid->local_devid;
    int result, ret;

    batch_num = (in->async_dma_type == TRS_ASYNC_DMA_TYPE_BATCH) ? in->async_batch_in->count :
        ((in->async_dma_type == TRS_ASYNC_DMA_TYPE_2D) ? in->async_2d_in->height : 1);

    wqe_fill_info_size = (sizeof(struct trs_remote_fill_wqe_info) + sizeof(struct trs_dma_wr_addr) * batch_num);
    wqe_fill_info = (struct trs_remote_fill_wqe_info *)malloc(wqe_fill_info_size);
    if (wqe_fill_info == NULL) {
        trs_err("Malloc wqe fill info failed. (size=%llu)\n", wqe_fill_info_size);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    urma_opcode = trs_get_async_urma_opcode(dev_id, async_devid->dst_devid, in->dir);
    wqe_fill_info->sq_id = in->sqId;
    wqe_fill_info->recv_dev_id = async_devid->remote_devid;
    wqe_fill_info->urma_opcode = urma_opcode;
    wqe_fill_info->dir = in->dir;
    wqe_fill_info->async_dma_type = in->async_dma_type;
    wqe_fill_info->batch_num = batch_num;
    ret = trs_fill_remote_wr_batch_info(urma_ctx, in, wqe_fill_info);
    if (ret != 0) {
        free(wqe_fill_info);
        return ret;
    }

    reply.buf_len = sizeof(result);
    reply.buf = (char *)&result;
    ret = trs_svm_mem_event_sync(dev_id, wqe_fill_info, wqe_fill_info_size, DRV_SUBEVENT_TRS_FILL_WQE_MSG, &reply);
    if (ret != DRV_ERROR_NONE) {
        free(wqe_fill_info);
        trs_err("Remote fill wqe failed. (dev_id=%u; sq_id=%u)\n", dev_id, in->sqId);
        return ret;
    }

    free(wqe_fill_info);
    return (drvError_t)result;
}

static int _trs_fill_async_dma_direct_wqe(struct trs_async_devid *async_devid, struct trs_urma_ctx *urma_ctx,
    struct trs_async_dma_wqe_info *wqe_info, struct trs_async_dma_input_para *in)
{
    struct udma_sqe_ctl_tmp *sqe = (struct udma_sqe_ctl_tmp *)(void *)wqe_info->wqe;
    struct udma_wqe_sge_tmp *sge = (struct udma_wqe_sge_tmp *)((void *)wqe_info->wqe + sizeof(struct udma_sqe_ctl_tmp));
    urma_seg_t seg = {0};
    urma_token_t token;
    uint64_t send_addr, recv_addr;
    int ret;

    trs_get_async_send_recv_addr(async_devid, in, &send_addr, &recv_addr);

    ret = trs_get_segment(urma_ctx, recv_addr, (uint64_t)in->async_normal_in->len, wqe_info, &seg, &token);
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
    sge->length = in->async_normal_in->len;
    sge->token_id = 0;
    sge->va = send_addr;

    return 0;
}

static uint32_t trs_get_async_dwqe_opcode(uint32_t dev_id, struct trs_async_devid *async_devid, uint32_t dir)
{
    if (dir == TRS_ASYNC_HOST_TO_DEVICE) {
        return 0x6; // 0x6:read
    } else if (dir == TRS_ASYNC_DEVICE_TO_HOST) {
        return 0x3; // 0x3:write
    } else if (dir == TRS_ASYNC_DEVICE_TO_DEVICE) {
        return (dev_id == async_devid->dst_devid) ? 0x6 : 0x3;  // 0x6:read; 0x3:write
    } else {
        return 0xFFFF; // 0xFFFF: invalid opcode
    }
}

static int trs_fill_async_dma_direct_wqe(struct trs_async_devid *async_devid, struct trs_urma_ctx *urma_ctx,
    struct trs_async_ctx *async_ctx, struct trs_async_dma_wqe_info *wqe_info, struct trs_async_dma_input_para *in)
{
    struct udma_sqe_ctl_tmp *sqe = (struct udma_sqe_ctl_tmp *)(void *)wqe_info->wqe;
    uint32_t dev_id = async_devid->local_devid;
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
        d2d_dev_ctx = trs_get_d2d_dev_ctx(wqe_info->remote_dev_id);
        sqe->tpn = d2d_dev_ctx->tpn;
        trs_swap_endian((uint8_t *)sqe->rmt_eid, d2d_dev_ctx->dst_jetty_id.eid.raw, URMA_EID_SIZE);
        trs_dev_ctx_mutex_un_lock(wqe_info->remote_dev_id);
    } else {
        sqe->tpn = async_ctx->tpn;
        trs_swap_endian((uint8_t *)sqe->rmt_eid, urma_ctx->trs_jfr.jfr->urma_ctx->eid.raw, URMA_EID_SIZE);
    }
    sqe->opcode = trs_get_async_dwqe_opcode(dev_id, async_devid, in->dir);
    ret = _trs_fill_async_dma_direct_wqe(async_devid, urma_ctx, wqe_info, in);
    if (ret != 0) {
        return ret;
    }

    return 0;
}

static uint32_t trs_get_async_post_wr_num(struct trs_async_dma_input_para *in, bool is_direct_wqe)
{
    uint32_t num = 0;

    if ((in->async_dma_type == TRS_ASYNC_DMA_TYPE_NORMAL) || (in->async_dma_type == TRS_ASYNC_DMA_TYPE_SQE_UPDATE)) {
        /* direct wqe not support 256MB split yet */
        num = (is_direct_wqe == true) ? 1 :
            align_up(in->async_normal_in->len, TRS_URMA_WR_CPY_MAX_SIZE) / TRS_URMA_WR_CPY_MAX_SIZE;
    } else if (in->async_dma_type == TRS_ASYNC_DMA_TYPE_BATCH) {
        uint32_t i;
        for (i = 0; i < in->async_batch_in->count; i++) {
            num += (align_up(in->async_batch_in->len[i], TRS_URMA_WR_CPY_MAX_SIZE) / TRS_URMA_WR_CPY_MAX_SIZE);
        }
    } else if (in->async_dma_type == TRS_ASYNC_DMA_TYPE_2D) {
        num = (align_up(in->async_2d_in->width, TRS_URMA_WR_CPY_MAX_SIZE) / TRS_URMA_WR_CPY_MAX_SIZE) *
                (in->async_2d_in->height);
    } else {
        num = UINT_MAX;
    }

    return num;
}

static int trs_check_async_jetty_credit(struct trs_async_ctx *async_ctx, uint32_t wr_num, bool is_direct_wqe)
{
    uint32_t credit = 0;

    if (is_direct_wqe == true) { /* dwqe not post wr, just pack */
        credit = trs_jetty_get_credit(async_ctx->pi, async_ctx->ci, TRS_UB_PI_CI_DEPTH);
    } else {
        credit = trs_jetty_get_credit(async_ctx->pi % TRS_STARS_NORMAL_JETTY_DEPTH,
                                      async_ctx->ci % TRS_STARS_NORMAL_JETTY_DEPTH, TRS_STARS_NORMAL_JETTY_DEPTH);
    }

    if (credit < wr_num) {
        trs_err("Jetty Credit not enough. (pi=%u; ci=%u; credit=%u; wr_num=%u)\n",
            async_ctx->pi, async_ctx->ci, credit, wr_num);
        return DRV_ERROR_NO_RESOURCES;
    }

    return 0;
}

static int trs_fill_async_dma_wqe(struct trs_async_devid *async_devid, struct sqcq_usr_info *sq_info,
    struct trs_async_dma_wqe_info *wqe_info, struct trs_async_dma_input_para *in, struct halAsyncDmaOutputPara *out)
{
    struct trs_urma_ctx *urma_ctx = sq_info->urma_ctx;
    bool is_direct_wqe = trs_is_async_direct_wqe(sq_info->flag, in->async_dma_type);
    bool is_wqe_sink = trs_is_async_jetty_wqe_sink(sq_info->flag);
    struct trs_async_ctx *async_ctx = trs_get_async_ctx(urma_ctx, in->dir, in->async_dma_type);
    uint32_t wr_num = trs_get_async_post_wr_num(in, is_direct_wqe);
    uint32_t last_pi = async_ctx->pi;
    int ret = 0;

    ret = trs_check_async_jetty_credit(async_ctx, wr_num, is_direct_wqe);
    if (ret != 0) {
        return ret;
    }

    async_ctx->pi = ((async_ctx->pi + wr_num) % TRS_UB_PI_CI_DEPTH);
    if (is_direct_wqe == true) {
        ret = trs_fill_async_dma_direct_wqe(async_devid, urma_ctx, async_ctx, wqe_info, in);
    } else {
        ret = trs_remote_fill_async_dma_wqe(async_devid, urma_ctx, in);
    }

    if (ret != 0) {
        async_ctx->pi = last_pi;
        return ret;
    }

    out->jettyId = async_ctx->src_jetty_id.id;
    out->pi = (is_wqe_sink == false) ? async_ctx->pi : wr_num; /* absolute in single task, relative in task sinking */
    out->functionId = async_ctx->func_id;
    out->dieId = async_ctx->die_id;
    out->fixedSize = (in->async_dma_type == TRS_ASYNC_DMA_TYPE_BATCH) ? in->async_batch_in->count :
        ((in->async_dma_type == TRS_ASYNC_DMA_TYPE_2D) ? in->async_2d_in->width * in->async_2d_in->height : 1);

    return 0;
}

static inline uint64_t trs_get_sq_bind_remote_que_addr(uint32_t dev_id, uint32_t ts_id, struct sqcq_usr_info *sq_info)
{
    if (((struct trs_urma_ctx *)sq_info->urma_ctx)->remote_sq_que_addr != 0) {
        return ((struct trs_urma_ctx *)sq_info->urma_ctx)->remote_sq_que_addr;
    }

    if (sq_info->switch_stream_flag) {
        struct res_id_usr_info *stream_usr_info = trs_get_res_id_info(dev_id, ts_id, DRV_STREAM_ID, sq_info->stream_id);
        if ((stream_usr_info != NULL) && (stream_usr_info->valid != 0)) {
            return stream_usr_info->res_addr;
        }
    }

    return (uintptr_t)NULL;
}

static int trs_fill_h2d_async_dma_wqe(struct trs_async_devid *async_devid, struct sqcq_usr_info *sq_info,
    struct trs_async_dma_wqe_info *wqe_info, struct trs_async_dma_input_para *in, struct halAsyncDmaOutputPara *out)
{
    uint32_t sqid = in->async_normal_in->info.sq_id;
    uint32_t sqe_pos = 0;
    int ret;

    if (in->async_dma_type == TRS_ASYNC_DMA_TYPE_SQE_UPDATE) {
        struct sqcq_usr_info *update_sq_info = NULL;
        uint64_t remote_sq_que_addr = 0;

        update_sq_info = trs_get_sq_info(async_devid->local_devid, in->tsId, in->type, sqid);
        if ((update_sq_info == NULL) || (update_sq_info->urma_ctx == NULL)) {
            trs_err("Invalid para. (devid=%u; tsId=%u; type=%d; sqid=%u)\n",
                async_devid->local_devid, in->tsId, in->type, sqid);
            return DRV_ERROR_INVALID_VALUE;
        }

        sqe_pos = in->async_normal_in->info.sqe_pos;
        if ((sqe_pos >= update_sq_info->depth) ||
            (in->async_normal_in->len > (update_sq_info->depth - sqe_pos) * update_sq_info->e_size)) {
            trs_err("Invalid para. (sqid=%u; sqe_pos=%u; len=%u; depth=%u, sqe_size=%u)\n", sqid, sqe_pos,
                in->async_normal_in->len, update_sq_info->depth, update_sq_info->e_size);
            return DRV_ERROR_PARA_ERROR;
        }

        remote_sq_que_addr = trs_get_sq_bind_remote_que_addr(async_devid->local_devid, 0, update_sq_info);
        if (remote_sq_que_addr == 0) {
            trs_err("Sq queue not inited. (devid=%u; type=%d; sqid=%u)\n", async_devid->local_devid, in->type, sqid);
            return DRV_ERROR_PARA_ERROR;
        }

        in->async_normal_in->dst = (u8 *)(uintptr_t)remote_sq_que_addr + sqe_pos * update_sq_info->e_size;
        trs_debug("(devid=%u; sqid=%u; sqe_pos=%u; sqe_size=%u; len=%u; remote_sq_que_addr=0x%llx; dst=0x%llx)\n",
            async_devid->local_devid, sqid, sqe_pos, update_sq_info->e_size, in->async_normal_in->len,
            remote_sq_que_addr, (u64)(uintptr_t)in->async_normal_in->dst);
    }

    ret = trs_fill_async_dma_wqe(async_devid, sq_info, wqe_info, in, out);
    if (in->async_dma_type == TRS_ASYNC_DMA_TYPE_SQE_UPDATE) {
        in->async_normal_in->info.sq_id = sqid;
        in->async_normal_in->info.sqe_pos = sqe_pos;
    }

    return ret;
}

static int trs_fill_d2h_async_dma_wqe(struct trs_async_devid *async_devid, struct sqcq_usr_info *sq_info,
    struct trs_async_dma_wqe_info *wqe_info, struct trs_async_dma_input_para *in, struct halAsyncDmaOutputPara *out)
{
    return trs_fill_async_dma_wqe(async_devid, sq_info, wqe_info, in, out);
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

static int trs_create_local_d2d_jetty(struct trs_async_devid *d2d_async_devid, uint32_t sq_id, uint32_t flag,
    struct trs_async_ctx *async_ctx)
{
    struct trs_d2d_sync_info sync_msg;
    struct trs_d2d_send_info alloc_info = {0};
    int ret;
    alloc_info.flag = flag;
    alloc_info.pos = TRS_ASYNC_SEND_SIDE;
    alloc_info.sq_id = sq_id;
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

static int trs_destroy_local_d2d_jetty(uint32_t local_devid, uint32_t sq_id)
{
    struct trs_d2d_send_info free_info = {0};
    free_info.pos = TRS_ASYNC_SEND_SIDE;
    free_info.sq_id = sq_id;
    return trs_free_device_jetty(local_devid, &free_info);
}

static int trs_create_remote_d2d_jetty(struct trs_async_devid *d2d_async_devid, uint32_t sq_id, uint32_t flag,
    struct trs_d2d_dev_ctx *d2d_dev_ctx)
{
    struct trs_d2d_sync_info sync_msg;
    struct trs_d2d_send_info alloc_info = {0};
    int ret;

    alloc_info.flag = flag;
    alloc_info.pos = TRS_ASYNC_RECV_SIDE;
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
    struct trs_d2d_dev_ctx *d2d_dev_ctx = trs_get_d2d_dev_ctx(dev_id);
    struct trs_d2d_send_info info = {0};
    int ret;

    if (d2d_dev_ctx == NULL) {
        return 0;
    }

    info.pos = TRS_ASYNC_RECV_SIDE;
    trs_dev_ctx_mutex_lock(dev_id);
    ret = trs_free_device_jetty(dev_id, &info);
    if (ret != 0) {
        trs_dev_ctx_mutex_un_lock(dev_id);
        trs_err("Failed to free remote jetty. (dev_id=%u; ret=%d)\n", dev_id, ret);
        return ret;
    }
    trs_set_d2d_dev_ctx(dev_id, NULL);
    free(d2d_dev_ctx);
    trs_dev_ctx_mutex_un_lock(dev_id);
    return 0;
}

static int trs_d2d_async_ctx_create(struct trs_urma_ctx *urma_ctx, struct trs_async_devid *d2d_async_devid,
    uint32_t sq_id, uint32_t flag)
{
    uint32_t local_devid = d2d_async_devid->local_devid;
    uint32_t remote_devid = d2d_async_devid->remote_devid;
    struct trs_async_ctx *async_ctx = &urma_ctx->d2d_async_ctx;
    int ret, create_local_flag = 0;

    (void)pthread_mutex_lock(&urma_ctx->ctx_mutex);
    if (!async_ctx->init_flag) {
        ret = trs_create_local_d2d_jetty(d2d_async_devid, sq_id, flag, async_ctx);
        if (ret != 0) {
            (void)pthread_mutex_unlock(&urma_ctx->ctx_mutex);
            trs_err("Failed to create local jetty. (local_devid=%u; ret=%d; flag=0x%x)\n", local_devid, ret, flag);
            return ret;
        }
        async_ctx->init_flag = true;
        create_local_flag = 1;
        trs_debug("Create local jetty success. (devid=%u; flag=0x%x; sq_id=%u; jetty_id=%u; fun_id=%u; die_id=%u)\n",
            local_devid, flag, sq_id, async_ctx->src_jetty_id.id, async_ctx->func_id, async_ctx->die_id);
    }
    (void)pthread_mutex_unlock(&urma_ctx->ctx_mutex);

    trs_dev_ctx_mutex_lock(remote_devid);
    if (trs_get_d2d_dev_ctx(remote_devid) == NULL) {
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
        trs_set_d2d_dev_ctx(remote_devid, (void *)d2d_dev_ctx);
        trs_debug("Create remote jetty success. (remote_devid=%u; sq_id=%u; flag=0x%x; jetty_id=%u)\n",
            remote_devid, sq_id, flag, d2d_dev_ctx->dst_jetty_id.id);
    }

    (void)trs_dev_ctx_mutex_un_lock(remote_devid);
    return 0;

free_local:
    trs_dev_ctx_mutex_un_lock(remote_devid);
    if (create_local_flag == 1) {
        (void)pthread_mutex_lock(&urma_ctx->ctx_mutex);
        (void)trs_destroy_local_d2d_jetty(local_devid, sq_id);
        async_ctx->init_flag = false;
        (void)pthread_mutex_unlock(&urma_ctx->ctx_mutex);
    }
    return ret;
}

static int trs_d2d_async_ctx_destroy(uint32_t dev_id, uint32_t sq_id, struct trs_urma_ctx *urma_ctx)
{
    int ret;

    (void)pthread_mutex_lock(&urma_ctx->ctx_mutex);
    if (urma_ctx->d2d_async_ctx.init_flag) {
        ret = trs_destroy_local_d2d_jetty(dev_id, sq_id);
        if (ret != 0) {
            (void)pthread_mutex_unlock(&urma_ctx->ctx_mutex);
            trs_err("Failed to destroy local jetty. (dev_id=%u; sq_id=%u; ret=%d)\n", dev_id, sq_id, ret);
            return ret;
        }
        trs_debug("Destroy d2d jetty success. (dev_id=%u; sq_id=%u)\n", dev_id, sq_id);
        urma_ctx->d2d_async_ctx.init_flag = false;
    }
    (void)pthread_mutex_unlock(&urma_ctx->ctx_mutex);

    return 0;
}

static int trs_fill_d2d_async_dma_wqe(struct trs_async_devid *async_devid, struct sqcq_usr_info *sq_info,
    struct trs_async_dma_wqe_info *wqe_info, struct trs_async_dma_input_para *in, struct halAsyncDmaOutputPara *out)
{
    wqe_info->remote_dev_id = async_devid->remote_devid;
    return trs_fill_async_dma_wqe(async_devid, sq_info, wqe_info, in, out);
}

static int (*const trs_async_dma_wqe_fill_handles[TRS_ASYNC_MAX_DIR])(struct trs_async_devid *async_devid,
    struct sqcq_usr_info *sq_info, struct trs_async_dma_wqe_info *wqe_info, 
    struct trs_async_dma_input_para *in, struct halAsyncDmaOutputPara *out) = {
    [TRS_ASYNC_HOST_TO_DEVICE] = trs_fill_h2d_async_dma_wqe,
    [TRS_ASYNC_DEVICE_TO_HOST] = trs_fill_d2h_async_dma_wqe,
    [TRS_ASYNC_DEVICE_TO_DEVICE] = trs_fill_d2d_async_dma_wqe,
};

static int trs_init_batch_2d_stars_jetty(uint32_t dev_id, struct trs_urma_ctx *urma_ctx, uint32_t sqId)
{
    uint32_t sq_id = sqId;
    struct event_reply reply;
    int ret, result;

    reply.buf_len = sizeof(urma_jetty_id_t) + sizeof(int);
    reply.buf = (char *)malloc(reply.buf_len);
    if (reply.buf == NULL) {
        trs_err("Malloc reply buffer failed. (size=%u)\n", reply.buf_len);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    ret = trs_local_mem_event_sync(dev_id, &sq_id, sizeof(uint32_t), DRV_SUBEVENT_TRS_INIT_BATCH_2D_JETTY_MSG, &reply);
    result = DRV_EVENT_REPLY_BUFFER_RET(reply.buf);

    if ((ret != DRV_ERROR_NONE) || (result != 0)) {
        trs_err("Failed to sync event. (dev_id=%u; ret=%d; result=%d)\n", dev_id, ret, result);
        free(reply.buf);
        return (ret != DRV_ERROR_NONE) ? ret : result;
    }

    (void)memcpy_s(&urma_ctx->batch_2d_async_ctx.src_jetty_id, sizeof(urma_jetty_id_t),
        DRV_EVENT_REPLY_BUFFER_DATA_PTR(reply.buf), sizeof(urma_jetty_id_t));

    free(reply.buf);
    return 0;
}

static int trs_async_init_async_ctx(struct trs_async_devid *async_devid, struct sqcq_usr_info *sq_info,
    struct trs_async_dma_input_para *in)
{
    struct trs_urma_ctx *urma_ctx = sq_info->urma_ctx;
    int ret = 0;

    if (in->dir == TRS_ASYNC_DEVICE_TO_DEVICE) {
        ret = trs_d2d_async_ctx_create(urma_ctx, async_devid, in->sqId, sq_info->flag);
        if (ret != 0) {
            return ret;
        }
    }

    /* 2d/batch jetty would be inited in first remote wqe fill h2d msg */
    if (((in->dir == TRS_ASYNC_HOST_TO_DEVICE) || (in->dir == TRS_ASYNC_DEVICE_TO_HOST)) &&
        ((in->async_dma_type == TRS_ASYNC_DMA_TYPE_2D) || (in->async_dma_type == TRS_ASYNC_DMA_TYPE_BATCH))) {
        if (urma_ctx->batch_2d_async_ctx.init_flag == false) {
            ret = trs_init_batch_2d_stars_jetty(async_devid->local_devid, urma_ctx, in->sqId);
            if (ret != 0) {
                return ret;
            }

            urma_ctx->batch_2d_async_ctx.die_id = urma_ctx->die_id;
            urma_ctx->batch_2d_async_ctx.func_id = urma_ctx->func_id;
            urma_ctx->batch_2d_async_ctx.init_flag = true;
        }
    }

    return 0;
}

/* all async ctx would be destroyed in sq free */
int trs_async_uninit_async_ctx(uint32_t dev_id, uint32_t sq_id, void *master_ctx)
{
    struct trs_urma_ctx *urma_ctx = (struct trs_urma_ctx *)master_ctx;

    if (urma_ctx->batch_2d_async_ctx.init_flag == true) {
        /* batch_2d jetty destroyed in sqcq remote free */
        urma_ctx->batch_2d_async_ctx.init_flag = false;
    }

    return trs_d2d_async_ctx_destroy(dev_id, sq_id, urma_ctx);
}

drvError_t trs_async_dma_wqe_create(uint32_t dev_id, struct trs_async_dma_input_para *in,
    struct halAsyncDmaOutputPara *out)
{
    struct sqcq_usr_info *sq_info = NULL;
    struct trs_async_dma_wqe_info *wqe_info = NULL;
    struct trs_async_devid async_devid = {0};
    int ret;

    ret = trs_get_check_async_devid_dir(dev_id, in, &async_devid, &in->dir);
    if (ret != 0) {
        return ret;
    }

    sq_info = trs_get_sq_info(dev_id, in->tsId, in->type, in->sqId);
    if ((sq_info == NULL) || (sq_info->urma_ctx == NULL)) {
        trs_err("Invalid para. (dev_id=%u; ts_id=%u; type=%d; sq_id=%u)\n", dev_id, in->tsId, in->type, in->sqId);
        return DRV_ERROR_INVALID_VALUE;
    }

    /* d2d and 2d/batch async cpy need init when first submit */
    ret = trs_async_init_async_ctx(&async_devid, sq_info, in);
    if (ret != 0) {
        return ret;
    }

    wqe_info = (struct trs_async_dma_wqe_info *)calloc(1, sizeof(struct trs_async_dma_wqe_info));
    if (wqe_info == NULL) {
        trs_err("WqeInfo malloc failed. (dev_id=%u)\n", dev_id);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    ret = trs_async_dma_wqe_fill_handles[in->dir](&async_devid, sq_info, wqe_info, in, out);
    if (ret != 0) {
        trs_err("Failed to fill async dma wqe. (ret=%d; dev_id=%u; sq_id=%u)\n", ret, dev_id, in->sqId);
        free(wqe_info);
        return ret;
    }

    /* normal dma dwqe cpy need to store tseg when destroy */
    if (trs_is_async_direct_wqe(sq_info->flag, in->async_dma_type) == true) {
        wqe_info->dir = in->dir;
        out->size = STARS_ASYNC_DMA_WQE_SIZE;
        out->wqe = wqe_info->wqe;
    } else {
        out->size = 0;
        out->wqe = NULL;
        free(wqe_info);
    }

    trs_debug("Create async dma wqe. (dev_id=%u; dir=%u; sq_id=%u; dieid=%u; func_id=%u; jetty_id=%u)\n",
        dev_id, in->dir, in->sqId, out->dieId, out->functionId, out->jettyId);
    return DRV_ERROR_NONE;
}

static drvError_t trs_async_dma_wqe_normal_destory(uint32_t dev_id, struct trs_urma_ctx *urma_ctx,
    struct halAsyncDmaDestoryPara *para)
{
    struct trs_async_dma_wqe_info *wqe_info = NULL;
    struct trs_async_ctx *async_ctx = NULL;

    if ((para->wqe == NULL) && (para->size == 0)) {
        return DRV_ERROR_NONE;
    }

    if ((para->wqe == NULL) || (para->size != STARS_ASYNC_DMA_WQE_SIZE)) {
        trs_err("Invalid para. (dev_id=%u; wqe=0x%llx; size=%u)\n", dev_id, (uint64_t)(uintptr_t)para->wqe, para->size);
        return DRV_ERROR_INVALID_VALUE;
    }

    wqe_info = (struct trs_async_dma_wqe_info *)(void *)para->wqe;
    if ((wqe_info->flag == 1) && (wqe_info->async_tseg != NULL)) {
        (void)urma_unregister_seg(wqe_info->async_tseg);
        trs_debug("Unregister seg. (dev_id=%u)\n", dev_id);
    }
    async_ctx = trs_get_async_ctx(urma_ctx, wqe_info->dir, TRS_ASYNC_DMA_TYPE_NORMAL);
    if (async_ctx == NULL) {
        trs_err("Not create. (dev_id=%u; sq_id=%u)\n", dev_id, para->sqId);
        return DRV_ERROR_UNINIT;
    }

    async_ctx->ci = (async_ctx->ci + 1) % trs_get_async_pi_ci_max(false);
    trs_debug("Async dma wqe destroy. (dev_id=%u; sq_id=%u; ci=%u; dir=%u)\n",
        dev_id, para->sqId, async_ctx->ci, wqe_info->dir);
    free(wqe_info);
    return 0;
}

static drvError_t trs_async_dma_wqe_batch_2d_destory(uint32_t dev_id, struct trs_urma_ctx *urma_ctx,
    struct trs_async_dma_destroy_para *para)
{
    struct trs_async_ctx *async_ctx = NULL;

    if (para->async_batch_para->ci >= TRS_UB_PI_CI_DEPTH) {
        trs_err("The ci exceed. (dev_id=%u; sq_id=%u; ci=%u; max=%d)\n", dev_id, para->sqId, para->async_batch_para->ci,
            TRS_UB_PI_CI_DEPTH);
        return DRV_ERROR_PARA_ERROR;
    }

    async_ctx = &urma_ctx->batch_2d_async_ctx;
    if (async_ctx->init_flag == false) {
        trs_err("Not create. (dev_id=%u; sq_id=%u)\n", dev_id, para->sqId);
        return DRV_ERROR_UNINIT;
    }

    async_ctx->ci = para->async_batch_para->ci;
    trs_debug("Async dma wqe destroy. (dev_id=%u; sq_id=%u; ci=%u;)\n", dev_id, para->sqId, async_ctx->ci);

    return 0;
}

drvError_t trs_async_dma_wqe_destory(uint32_t dev_id, struct trs_async_dma_destroy_para *para)
{
    struct sqcq_usr_info *sq_info = NULL;
    struct trs_urma_ctx *urma_ctx = NULL;
    int ret = 0;

    sq_info = trs_get_sq_info(dev_id, para->tsId, DRV_NORMAL_TYPE, para->sqId);
    if (sq_info == NULL) {
        trs_err("Invalid para. (dev_id=%u; ts_id=%u; sq_id=%u)\n", dev_id, para->tsId, para->sqId);
        return DRV_ERROR_INVALID_VALUE;
    }

    urma_ctx = (struct trs_urma_ctx *)sq_info->urma_ctx;
    if (para->async_dma_type == TRS_ASYNC_DMA_TYPE_NORMAL) {
        ret = trs_async_dma_wqe_normal_destory(dev_id, urma_ctx, para->normal_para);
    } else if ((para->async_dma_type == TRS_ASYNC_DMA_TYPE_2D) || (para->async_dma_type == TRS_ASYNC_DMA_TYPE_BATCH)) {
        ret = trs_async_dma_wqe_batch_2d_destory(dev_id, urma_ctx, para);
    } else {
        ret = DRV_ERROR_NOT_SUPPORT;
    }

    return ret;
}

drvError_t trs_async_ctx_pi_ci_reset(uint32_t dev_id, uint32_t sq_id)
{
    struct sqcq_usr_info *sq_info = NULL;
    struct trs_urma_ctx *urma_ctx = NULL;

    sq_info = trs_get_sq_info(dev_id, 0, DRV_NORMAL_TYPE, sq_id);
    if (sq_info == NULL) {
        trs_err("Invalid para. (dev_id=%u; sq_id=%u)\n", dev_id, sq_id);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (trs_is_async_jetty_wqe_sink(sq_info->flag) == false) {
        return DRV_ERROR_NONE;
    }

    urma_ctx = (struct trs_urma_ctx *)sq_info->urma_ctx;
    urma_ctx->async_ctx.pi = 0;
    urma_ctx->async_ctx.ci = 0;

    return DRV_ERROR_NONE;
}

drvError_t trs_sq_jetty_info_query(uint32_t dev_id, struct halSqCqQueryInfo *info)
{
    struct sqcq_usr_info *sq_info = NULL;
    struct trs_async_ctx *async_ctx = NULL;
    struct trs_urma_ctx *urma_ctx = NULL;
    uint32_t dir;

    sq_info = trs_get_sq_info(dev_id, info->tsId, info->type, info->sqId);
    if ((sq_info == NULL) || (sq_info->urma_ctx == NULL)) {
        trs_err("Invalid para. (dev_id=%u; ts_id=%u; type=%d; sq_id=%u)\n", dev_id, info->tsId, info->type, info->sqId);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((sq_info->flag & TSDRV_FLAG_TASK_SINK_SQ) == 0) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    urma_ctx = (struct trs_urma_ctx *)sq_info->urma_ctx;
    dir = (info->prop == DRV_SQCQ_PROP_D2D_ASYNC_JETTY_INFO) ? TRS_ASYNC_DEVICE_TO_DEVICE : TRS_ASYNC_HOST_TO_DEVICE;

    async_ctx = trs_get_async_ctx(urma_ctx, dir, TRS_ASYNC_DMA_TYPE_NORMAL);
    if (async_ctx == NULL) {
        trs_err("Failed to get async ctx. (dev_id=%u; sq_id=%u; prop=%d)\n", dev_id, info->sqId, info->prop);
        return DRV_ERROR_UNINIT;
    }

    info->value[0] = trs_get_async_pi_ci_max(true) - async_ctx->pi; /* 0: NOP number */
    info->value[1] = async_ctx->src_jetty_id.id;                    /* 1: jetty id */
    info->value[2] = async_ctx->func_id;                            /* 2: func id */
    info->value[3] = async_ctx->die_id;                             /* 3: die id*/
    trs_debug("Query jetty info. (devid=%u; sqid=%u; prop=%d; nop_num=%u; jetty_id=%u; func_id=%u; die_id=%u)\n",
        dev_id, info->sqId, info->prop, info->value[0], info->value[1], info->value[2], info->value[3]);

    return DRV_ERROR_NONE;
}
