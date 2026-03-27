/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <pthread.h>

#include "ascend_hal.h"

#include "svm_log.h"
#include "svm_pub.h"
#include "mga.h"
#include "svm_dbi.h"
#include "va_mng.h"
#include "va_gap.h"
#include "va_reserve.h"

struct va_pcie_th_allocator {
    int valid;
    void *mga_inst;
};

#define VA_PCIE_TH_RESERVE_GRAN_SIZE     (512ULL * SVM_BYTES_PER_GB)

/* not dynamic expand and shrink */
#define VA_PCIE_TH_EXPAND_FACTOR    1
#define VA_PCIE_TH_SHRINK_FACTOR    1

static struct va_pcie_th_allocator g_pcie_th_allocator = {0};

static struct va_pcie_th_allocator *va_get_pcie_th_allocator(void)
{
    return &g_pcie_th_allocator;
}

void svm_enable_pcie_th(void)
{
    struct va_pcie_th_allocator *allocator = va_get_pcie_th_allocator();
    allocator->valid = 1;
}

bool svm_is_support_pcie_th(void)
{
    struct va_pcie_th_allocator *allocator = va_get_pcie_th_allocator();
    return (allocator->valid == 1);
}

static int va_pcie_th_va_reserve(u64 size, u64 start, u64 end, u64 step, u64 *reserve_va)
{
    u64 va;
    int ret;

    for (va = start; va <= (end - size); va += step) {
        ret = svm_reserve_master_only_va(va, size);
        if (ret == DRV_ERROR_NONE) {
            svm_info("Pcie th reserve va success. (va=0x%llx; size=0x%llx)\n", va, size);
            *reserve_va = va;
            return ret;
        }
    }

    return DRV_ERROR_OUT_OF_MEMORY;
}

static int va_pcie_th_va_expand(void *mga_inst, u64 *expand_size, u64 *va)
{
    u64 start, total_size, step;
    /* expand_size always bigger than total size because expand factor is 1 */
    u64 size = *expand_size - mga_get_total_size(mga_inst);

    svm_get_pcie_th_va_range(&start, &total_size);
    step = VA_PCIE_TH_RESERVE_GRAN_SIZE;

    while (size >= VA_PCIE_TH_RESERVE_GRAN_SIZE) {
        int ret = va_pcie_th_va_reserve(size, start, start + total_size, step, va);
        if (ret == DRV_ERROR_NONE) {
            /* avoid alloc a gap va. */
            *expand_size = size;
            return DRV_ERROR_NONE;
        }

        size -= VA_PCIE_TH_RESERVE_GRAN_SIZE;
    }

    return DRV_ERROR_OUT_OF_MEMORY;
}

static int va_pcie_th_va_shrink(void *mga_inst, u64 va, u64 size)
{
    SVM_UNUSED(mga_inst);
    SVM_UNUSED(size);

    svm_release_master_only_va(va);
    return DRV_ERROR_NONE;
}

static int _va_pcie_th_allocator_init_locked(struct va_pcie_th_allocator *allocator)
{
    u64 start, size;
    void *mga_inst = NULL;
    struct mga_attr attr;

    svm_get_pcie_th_va_range(&start, &size);

    attr.max_align_size = (2ULL * SVM_BYTES_PER_MB);
    attr.expand_gran = size;
    attr.expand_thres = size * VA_PCIE_TH_EXPAND_FACTOR;
    attr.shrink_thres = size * VA_PCIE_TH_SHRINK_FACTOR;
    attr.expand = va_pcie_th_va_expand;
    attr.shrink = va_pcie_th_va_shrink;

    mga_inst = mga_inst_create(&attr);
    if (mga_inst == NULL) {
        svm_err("Create mga inst failed.\n");
        return DRV_ERROR_INNER_ERR;
    }

    allocator->mga_inst = mga_inst;
    return DRV_ERROR_NONE;
}

static int va_pcie_th_allocator_init_locked(struct va_pcie_th_allocator *allocator)
{
    if (allocator->valid == 0) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (allocator->mga_inst != NULL) {
        return 0;
    }

    return _va_pcie_th_allocator_init_locked(allocator);
}

static int va_pcie_th_allocator_init(struct va_pcie_th_allocator *allocator)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int ret;

    (void)pthread_mutex_lock(&mutex);
    ret = va_pcie_th_allocator_init_locked(allocator);
    (void)pthread_mutex_unlock(&mutex);

    return ret;
}

void va_pcie_th_allocator_uninit(void)
{
    struct va_pcie_th_allocator *allocator = va_get_pcie_th_allocator();

    if (allocator->valid != 0) {
        if (allocator->mga_inst != NULL) {
            mga_inst_destroy(allocator->mga_inst);
            allocator->mga_inst = NULL;
        }
        allocator->valid = 0;
    }
}

static u64 va_pcie_th_get_gap_size(void)
{
    /* pcie th use 64k gap */
    return (svm_va_get_gap() == 0ULL) ? 0ULL : (64ULL * SVM_BYTES_PER_KB);
}

int va_pcie_th_alloc(u32 devid, u64 align, u64 size, u64 *va)
{
    struct va_pcie_th_allocator *allocator = va_get_pcie_th_allocator();

    if (devid != svm_get_host_devid()) {
        svm_err("Invalid devid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (allocator->mga_inst == NULL) {
        int ret = va_pcie_th_allocator_init(allocator);
        if (ret != 0) {
            return ret;
        }
    }

    /* add a gap size */
    return mga_va_alloc(allocator->mga_inst, align, size + va_pcie_th_get_gap_size(), va);
}

int va_pcie_th_free(u32 devid, u64 va, u64 size, u64 align)
{
    struct va_pcie_th_allocator *allocator = va_get_pcie_th_allocator();

    if (devid != svm_get_host_devid()) {
        svm_err("Invalid devid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (allocator->mga_inst == NULL) {
        return DRV_ERROR_INVALID_VALUE;
    }

    return mga_va_free(allocator->mga_inst, va, size + va_pcie_th_get_gap_size(), align);
}
