/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <sys/mman.h>
#include "ascend_hal.h"
#include "esched_user_interface.h"
#include "hdc_cmn.h"
#include "hdc_ub_drv.h"
#include "hdc_core.h"

void hdc_fill_event_summary_dst_engine(struct event_summary *event_submit)
{
    event_submit->dst_engine = CCPU_DEVICE;
}

void hdc_fill_event_msg_dst_engine(struct hdcdrv_sync_event_msg *msg)
{
    msg->head.dst_engine = CCPU_DEVICE;
}

int hdc_mem_res_init(struct hdc_mem_res_info *mem_info, int service_type)
{
    int fd = -1;
    int map_flag = 0;
    void *user_va;
    int ret = 0;

    mem_info->service_type = service_type;
    mem_info->bind_fd = hdc_ub_open();

    if (mem_info->bind_fd == (mmProcess)EN_ERROR) {
        HDC_LOG_ERR("Input parameter handle is invalid.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }

    if (hdc_use_kernel_pool(mem_info->service_type)) {
#ifndef EMU_ST
        fd = mem_info->bind_fd;
        map_flag = MAP_SHARED;
#else
        ret = hdc_mem_res_init_stub(mem_info->bind_fd, &mem_info->user_va, HDCDRV_UB_MEM_POOL_LEN);
        if (ret != 0) {
            goto va_mmap_failed;
        }
        else {
            return 0;
        }
#endif
    } else {
        fd = -1;
        map_flag = MAP_SHARED | MAP_ANONYMOUS;
    }

    user_va = mmap(NULL, HDCDRV_UB_MEM_POOL_LEN, PROT_READ | PROT_WRITE, map_flag, fd, 0);
    if (user_va == MAP_FAILED) {
        HDC_LOG_ERR("Failed to mmap tx_user memory.\n");
        ret = -HDCDRV_MEM_ALLOC_FAIL;
        goto va_mmap_failed;
    }

    mem_info->user_va = (uint64_t)(uintptr_t)user_va;

    return 0;

va_mmap_failed:
    hdc_ub_close(mem_info->bind_fd);

    return ret;
}

void hdc_mem_res_uninit(struct hdc_mem_res_info *mem_info, unsigned int dev_id)
{
    void *user_va = (void *)(uintptr_t)mem_info->user_va;
    (void)dev_id;

#ifndef EMU_ST
    (void)munmap(user_va, HDCDRV_UB_MEM_POOL_LEN);
#else
    if (hdc_use_kernel_pool(mem_info->service_type)) {
        hdc_mem_res_uninit_stub(mem_info->bind_fd, &mem_info->user_va);
    } else {
        (void)munmap(user_va, HDCDRV_UB_MEM_POOL_LEN);
    }
#endif

    hdc_ub_close(mem_info->bind_fd);

    return;
}

int hdc_register_share_urma_seg(struct hdcConfig *hdc_config, unsigned int dev_id, unsigned int token_val)
{
    (void)hdc_config;
    (void)dev_id;
    (void)token_val;
    return 0;
}

void hdc_unregister_share_urma_seg(urma_target_seg_t *tseg)
{
    (void)tseg;
    return;
}

int hdc_register_own_urma_seg(hdc_ub_context_t *ctx, unsigned long long len, unsigned long long va, int service_type)
{
    urma_seg_cfg_t seg_cfg = {0};

    seg_cfg.flag.bs.token_policy = URMA_TOKEN_PLAIN_TEXT;
    seg_cfg.flag.bs.cacheable = URMA_NON_CACHEABLE;
    seg_cfg.flag.bs.access = URMA_ACCESS_LOCAL_ONLY;
    seg_cfg.flag.bs.reserved = 0;
    seg_cfg.flag.bs.token_id_valid = 1;
    if (hdc_use_kernel_pool(service_type)) {
        seg_cfg.flag.bs.non_pin = 1;    /* alloc_pages memory need non pin, malloc need pin */
    } else {
        seg_cfg.flag.bs.non_pin = 0;
    }

    seg_cfg.va = (uint64_t)(uintptr_t)va;
    seg_cfg.len = len;
    seg_cfg.token_value.token = ctx->token.token;
    seg_cfg.user_ctx = (uintptr_t)NULL;
    seg_cfg.iova = 0;
    seg_cfg.token_id = ctx->token_id;

    ctx->tseg = urma_register_seg(ctx->urma_ctx, &seg_cfg);
    if (ctx->tseg == NULL) {
        HDC_LOG_ERR("Failed to register segment.\n");
        return HDCDRV_ERR;
    }

    return 0;
}

void hdc_unregister_own_urma_seg(urma_target_seg_t *tseg, int service_type)
{
    (void)service_type;
    if (tseg == NULL) {
        return;
    }
    (void)urma_unregister_seg(tseg);
}