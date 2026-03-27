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
#ifndef _HDCDRV_MEM_POOL_H_
#define _HDCDRV_MEM_POOL_H_

#include "ka_type.h"
#include "hdcdrv_core_ub.h"

static inline bool hdcdrv_use_kernel_mem_pool(int service_type)
{
    if (service_type == HDCDRV_SERVICE_TYPE_DMP) {
        return true;
    }

    return false;
}

int hdcdrv_init_dev_mem_pool(struct hdcdrv_dev *hdc_dev);
void hdcdrv_uninit_dev_mem_pool(struct hdcdrv_dev *hdc_dev);
int hdcdrv_alloc_mem_pool_for_session(struct hdcdrv_session *session, struct hdcdrv_dev *hdc_dev,
    struct hdcdrv_ctx *ctx, u64 user_va);
int hdcdrv_free_mem_pool_for_session(struct hdcdrv_session *session, struct hdcdrv_dev *hdc_dev,
    struct hdcdrv_ctx *ctx);
void hdcdrv_clear_mem_pool_by_ctx(struct hdcdrv_ctx *ctx);
#endif // _HDCDRV_MEM_POOL_H_