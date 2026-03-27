/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "svm_urma_def.h"
#include "svm_pub.h"
#include "svm_addr_desc.h"
#include "svm_log.h"
#include "svm_user_adapt.h"
#include "svm_memcpy.h"
#include "svm_urma_seg_mng.h"
#include "svm_urma_chan.h"
#include "svm_addr_desc.h"
#include "svm_dbi.h"
#include "va_allocator.h"
#include "svm_memcpy_ops_register.h"
#include "svm_register_to_master.h"
#include "svm_memcpy.h"

#define SVM_URMA_CHAN_WAIT_TIMEOUT_MS            60000ULL
#define SVM_URMA_CPY_TRACE_BOUNDARY_SIZE         (2 * SVM_BYTES_PER_MB)
#define SVM_URMA_CPY_TRACE_TIMESTAMP_THRESHOLD   500000ULL

static u64 svm_get_timestamp_ns(void)
{
    struct timespec ts = {0};

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (u64)ts.tv_sec * 1000000000ULL + (u64)ts.tv_nsec; /* 1 sec is 1000000000 nsec. */
}

static bool svm_urma_cpy_time_consume_abnormal(u64 size, u64 time_consume_ns)
{
    u64 threshold = (size < SVM_URMA_CPY_TRACE_BOUNDARY_SIZE) ?
        SVM_URMA_CPY_TRACE_TIMESTAMP_THRESHOLD :
        (size / SVM_URMA_CPY_TRACE_BOUNDARY_SIZE * SVM_URMA_CPY_TRACE_TIMESTAMP_THRESHOLD);
    return (time_consume_ns > threshold);
}

static urma_opcode_t svm_cpy_dir_to_urma_opcode(enum svm_cpy_dir dir)
{
    return (dir == SVM_H2D_CPY) ? URMA_OPC_WRITE : URMA_OPC_READ;
}

static int svm_ub_copy_urma_tseg_get(u32 src_devid, u32 dst_devid, struct svm_urma_chan_submit_para *submit_para)
{
    enum svm_cpy_dir dir = copy_dir_get_by_devid(src_devid, dst_devid);
    u32 user_devid = (dir == SVM_H2D_CPY) ? dst_devid : src_devid;
    struct svm_dst_va dst_va;
    int ret;

    svm_dst_va_pack(src_devid, PROCESS_CP1, submit_para->src, submit_para->size, &dst_va);
    ret = svm_urma_get_tseg(user_devid, &dst_va, &submit_para->src_tseg);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Get src tseg failed. (ret=%d; src_devid=%u; src_va=0x%llx; size=%llu)\n",
            ret, src_devid, submit_para->src, submit_para->size);
        return ret;
    }

    svm_dst_va_pack(dst_devid, PROCESS_CP1, submit_para->dst, submit_para->size, &dst_va);
    ret = svm_urma_get_tseg(user_devid, &dst_va, &submit_para->dst_tseg);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Get dst tseg failed. (ret=%d; dst_devid=%u; dst_va=0x%llx; dst_size=%llu)\n",
            ret, dst_devid, submit_para->dst, submit_para->size);
        return ret;
    }

    return ret;
}

int svm_ub_sync_copy(u32 devid, struct svm_copy_va_info *src_info, struct svm_copy_va_info *dst_info)
{
    enum svm_cpy_dir dir = copy_dir_get_by_devid(src_info->devid, dst_info->devid);
    struct svm_urma_chan_submit_para submit_para = {.src = src_info->va, .dst = dst_info->va, .size = src_info->size,
        .opcode = svm_cpy_dir_to_urma_opcode(dir)};
    u32 out_wr_num, chan_id;
    u64 start, end;
    int ret;

    ret = svm_ub_copy_urma_tseg_get(src_info->devid, dst_info->devid, &submit_para);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_urma_chan_alloc(devid, &chan_id);
    if (ret != DRV_ERROR_NONE) {
        svm_err("svm_urma_chan_alloc failed. (ret=%d)\n", ret);
        return ret;
    }

    start = svm_get_timestamp_ns();
    ret = svm_urma_chan_submit(devid, chan_id, &submit_para, &out_wr_num);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Urma chan submit failed. (ret=%d; chan_id=%u)\n", ret, chan_id);
        goto chan_free;
    }

    ret = svm_urma_chan_wait(devid, chan_id, (int)out_wr_num, SVM_URMA_CHAN_WAIT_TIMEOUT_MS);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Urma chan wait failed. (ret=%d; chan_id=%u)\n", ret, chan_id);
    }

    end = svm_get_timestamp_ns();
    if (svm_urma_cpy_time_consume_abnormal(src_info->size, end - start)) {
        svm_debug("urma memcpy time consume. (size=%llu; time=%llu ns)\n", src_info->size, end - start);
    }

chan_free:
    svm_urma_chan_free(devid, chan_id);
    return ret;
}

static void svm_unregister_to_master_2d(struct svm_copy_va_2d_info *info, u64 user_devid, u64 count, enum svm_cpy_dir dir, bool *is_register)
{
    struct svm_dst_va register_va;
    u32 host_devid = svm_get_host_devid();
    u32 flag = 0;
    u64 i;

    flag |= (dir == SVM_H2D_CPY) ? 0 : REGISTER_TO_MASTER_FLAG_ACCESS_WRITE;
    flag |= REGISTER_TO_MASTER_FLAG_PIN;

    for (i = 0; i < count; i++) {
        if (is_register[i] == false) {
            continue;
        }

        svm_dst_va_pack(host_devid, 0, info->va + i * info->pitch, info->width, &register_va);
        (void)svm_unregister_to_master(user_devid, &register_va, flag);
    }
}

static int svm_register_to_master_2d(struct svm_copy_va_2d_info *info, u64 user_devid, enum svm_cpy_dir dir, bool *is_register)
{
    struct svm_dst_va register_va;
    u32 host_devid = svm_get_host_devid();
    u32 flag = 0;
    int ret;
    u64 i;

    flag |= (dir == SVM_H2D_CPY) ? 0 : REGISTER_TO_MASTER_FLAG_ACCESS_WRITE;
    flag |= REGISTER_TO_MASTER_FLAG_PIN;

    for (i = 0; i < info->height; i++) {
        u64 va = info->va + i * info->pitch;
        u64 size = info->width;
        if (svm_va_is_in_range(va, size)) {
            continue;
        }

        svm_dst_va_pack(host_devid, 0, va, size, &register_va);
        ret = svm_register_to_master(user_devid, &register_va, flag);
        if (ret != DRV_ERROR_NONE) {
            svm_err("Register local mem to master failed. (ret=%d; host_va=0x%llx; size=%llu)\n", ret, va, size);
            svm_unregister_to_master_2d(info, user_devid, i, dir, is_register);
            return ret;
        }

        is_register[i] = true;
    }

    return DRV_ERROR_NONE;
}

static int svm_copy2d_submit_para_pack(struct svm_copy_va_2d_info *src_info, struct svm_copy_va_2d_info *dst_info,
    u64 height_index, struct svm_urma_chan_submit_para *submit_para)
{
    enum svm_cpy_dir dir = copy_dir_get_by_devid(src_info->devid, dst_info->devid);

    submit_para->src = src_info->va + src_info->pitch * height_index;
    submit_para->dst = dst_info->va + dst_info->pitch * height_index;
    submit_para->opcode = svm_cpy_dir_to_urma_opcode(dir);
    submit_para->size = dst_info->width;

    return svm_ub_copy_urma_tseg_get(src_info->devid, dst_info->devid, submit_para);
}

int _svm_ub_sync_copy_2d(u32 devid, struct svm_copy_va_2d_info *src_info, struct svm_copy_va_2d_info *dst_info)
{
    struct svm_urma_chan_submit_para submit_para;
    u32 out_wr_num, chan_id;
    int ret;
    u64 i;

    ret = svm_urma_chan_alloc(devid, &chan_id);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Svm_urma_chan_alloc failed. (ret=%d)\n", ret);
        return ret;
    }

    for (i = 0; i < src_info->height; i++) {
        ret = svm_copy2d_submit_para_pack(src_info, dst_info, i, &submit_para);
        if (ret != DRV_ERROR_NONE) {
            svm_err("Svm_submit_para_pack failed. (ret=%d; height_index=%llu)\n", ret, i);
            (void)svm_urma_chan_wait(devid, chan_id, -1, SVM_URMA_CHAN_WAIT_TIMEOUT_MS);
            goto chan_free;
        }

        ret = svm_urma_chan_submit(devid, chan_id, &submit_para, &out_wr_num);
        if (ret != DRV_ERROR_NONE) {
            svm_err("Svm_urma_chan_submit failed. (ret=%d; src_start=0x%llx; dst_start=0x%llx; width=%llu; i=%llu)\n",
                ret, src_info->va, dst_info->va, src_info->width, i);
            (void)svm_urma_chan_wait(devid, chan_id, -1, SVM_URMA_CHAN_WAIT_TIMEOUT_MS);
            goto chan_free;
        }
    }

    ret = svm_urma_chan_wait(devid, chan_id, -1, SVM_URMA_CHAN_WAIT_TIMEOUT_MS);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Urma chan wait failed. (ret=%d; chan_id=%u)\n", ret, chan_id);
    }

chan_free:
    svm_urma_chan_free(devid, chan_id);
    return ret;
}

int svm_ub_sync_copy_2d(u32 devid, struct svm_copy_va_2d_info *src_info, struct svm_copy_va_2d_info *dst_info)
{
    enum svm_cpy_dir dir = copy_dir_get_by_devid(src_info->devid, dst_info->devid);
    struct svm_copy_va_2d_info *register_info = (dir == SVM_H2D_CPY) ? src_info : dst_info;
    bool *is_register = NULL;
    int ret;

    is_register = calloc(src_info->height, sizeof(bool));
    if (is_register == NULL) {
        svm_err("Copy 2d calloc failed. (height=%llu)\n", src_info->height);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    ret = svm_register_to_master_2d(register_info, devid, dir, is_register);
    if (ret != DRV_ERROR_NONE) {
        free(is_register);
        return ret;
    }

    ret = _svm_ub_sync_copy_2d(devid, src_info, dst_info);

    svm_unregister_to_master_2d(register_info, devid, register_info->height, dir, is_register);
    free(is_register);

    return ret;
}

static int svm_batch_cpy_submit_para_pack(u64 src, u64 dst, u64 size, u64 src_devid, u64 dst_devid, struct svm_urma_chan_submit_para *submit_para)
{
    enum svm_cpy_dir dir = copy_dir_get_by_devid(src_devid, dst_devid);

    submit_para->src = src;
    submit_para->dst = dst;
    submit_para->opcode = svm_cpy_dir_to_urma_opcode(dir);
    submit_para->size = size;

    return svm_ub_copy_urma_tseg_get(src_devid, dst_devid, submit_para);
}

static void svm_unregister_to_master_batch(u64 batch_va[], u64 batch_size[], u64 user_devid, u64 count, enum svm_cpy_dir dir, bool *is_register)
{
    struct svm_dst_va register_va;
    u32 host_devid = svm_get_host_devid();
    u32 flag = 0;
    u64 i;

    flag |= (dir == SVM_H2D_CPY) ? 0 : REGISTER_TO_MASTER_FLAG_ACCESS_WRITE;
    flag |= REGISTER_TO_MASTER_FLAG_PIN;

    for (i = 0; i < count; i++) {
        if (is_register[i] == false) {
            continue;
        }

        svm_dst_va_pack(host_devid, 0, batch_va[i], batch_size[i], &register_va);
        (void)svm_unregister_to_master(user_devid, &register_va, flag);
    }
}

static int svm_register_to_master_batch(u64 batch_va[], u64 batch_size[], u64 user_devid, u64 count, enum svm_cpy_dir dir, bool *is_register)
{
    struct svm_dst_va register_va;
    u32 host_devid = svm_get_host_devid();
    u32 flag = 0;
    u64 i;
    int ret;

    flag |= (dir == SVM_H2D_CPY) ? 0 : REGISTER_TO_MASTER_FLAG_ACCESS_WRITE;
    flag |= REGISTER_TO_MASTER_FLAG_PIN;

    for (i = 0; i < count; i++) {
        u64 va = batch_va[i];
        u64 size = batch_size[i];
        if (svm_va_is_in_range(va, size)) {
            continue;
        }

        svm_dst_va_pack(host_devid, 0, va, size, &register_va);
        ret = svm_register_to_master(user_devid, &register_va, flag);
        if (ret != DRV_ERROR_NONE) {
            svm_err("Register local mem to master failed. (ret=%d; host_va=0x%llx; size=%llu)\n", ret, va, size);
            svm_unregister_to_master_batch(batch_va, batch_size, user_devid, i, dir, is_register);
            return ret;
        }

        is_register[i] = true;
    }

    return DRV_ERROR_NONE;
}

int _svm_ub_sync_copy_batch(u64 src[], u64 dst[], u64 size[], u64 count, u64 src_devid, u64 dst_devid)
{
    enum svm_cpy_dir dir = copy_dir_get_by_devid(src_devid, dst_devid);
    u32 user_devid = (dir == SVM_H2D_CPY) ? dst_devid : src_devid;
    struct svm_urma_chan_submit_para submit_para;
    u32 out_wr_num, chan_id;
    int ret;
    u64 i;

    ret = svm_urma_chan_alloc(user_devid, &chan_id);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Svm alloc jetty chan failed. (ret=%d)\n", ret);
        return ret;
    }

    for (i = 0; i < count; i++) {
        ret = svm_batch_cpy_submit_para_pack(src[i], dst[i], size[i], src_devid, dst_devid, &submit_para);
        if (ret != DRV_ERROR_NONE) {
            svm_err("Svm batch cpy para get failed. (ret=%d; src=0x%llx; dst=0x%llx; size=%llu; i=%llu)\n",
                ret, src[i], dst[i], size[i], i);
            (void)svm_urma_chan_wait(user_devid, chan_id, -1, SVM_URMA_CHAN_WAIT_TIMEOUT_MS);
            goto chan_free;
        }

        ret = svm_urma_chan_submit(user_devid, chan_id, &submit_para, &out_wr_num);
        if (ret != DRV_ERROR_NONE) {
            svm_err("Svm submit jetty cpy failed. (ret=%d; src=0x%llx; dst=0x%llx; size=%llu; i=%llu)\n",
                ret, src[i], dst[i], size[i], i);
            (void)svm_urma_chan_wait(user_devid, chan_id, -1, SVM_URMA_CHAN_WAIT_TIMEOUT_MS);
            goto chan_free;
        }
    }

    ret = svm_urma_chan_wait(user_devid, chan_id, -1, SVM_URMA_CHAN_WAIT_TIMEOUT_MS);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Svm chan jetty wait failed. (ret=%d; chan_id=%u)\n", ret, chan_id);
    }

chan_free:
    svm_urma_chan_free(user_devid, chan_id);
    return ret;
}

int svm_ub_sync_copy_batch(u64 src[], u64 dst[], u64 size[], u64 count, u32 src_devid, u32 dst_devid)
{
    enum svm_cpy_dir dir = copy_dir_get_by_devid(src_devid, dst_devid);
    u32 user_devid = (dir == SVM_H2D_CPY) ? dst_devid : src_devid;
    u64 *register_va = (dir == SVM_H2D_CPY) ? src : dst;
    bool *is_register = NULL;
    int ret;

    is_register = calloc(count, sizeof(bool));
    if (is_register == NULL) {
        svm_err("Batch cpy calloc failed. (addr_count=%llu)\n", count);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    ret = svm_register_to_master_batch(register_va, size, user_devid, count, dir, is_register);
    if (ret != DRV_ERROR_NONE) {
        free(is_register);
        return ret;
    }

    ret = _svm_ub_sync_copy_batch(src, dst, size, count, src_devid, dst_devid);

    svm_unregister_to_master_batch(register_va, size, user_devid, count, dir, is_register);
    free(is_register);

    return ret;
}

static struct svm_copy_ops g_ub_copy_ops = {
    .sync_copy = svm_ub_sync_copy,
    .async_copy_submit = NULL,
    .async_copy_wait = NULL,
    .dma_desc_convert = NULL,
    .dma_desc_submit = NULL,
    .dma_desc_wait = NULL,
    .dma_desc_destroy = NULL,
    .sync_copy_2d = svm_ub_sync_copy_2d,
    .dma_desc_convert_2d = NULL,
    .sync_copy_batch = svm_ub_sync_copy_batch
};

void svm_ub_memcpy_ops_register(u32 devid)
{
    svm_copy_ops_register(devid, &g_ub_copy_ops);
}

