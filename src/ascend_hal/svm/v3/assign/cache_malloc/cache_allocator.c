/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <stdlib.h>

#include "securec.h"
#include "ascend_hal.h"

#include "svm_pub.h"
#include "svm_log.h"
#include "svm_user_adapt.h"
#include "svm_dbi.h"
#include "gen_allocator.h"
#include "cache_pub.h"
#include "cache_malloc.h"
#include "cache_allocator.h"

static struct cache_allocator *g_ca[SVM_MAX_DEV_NUM][CACHE_TYPE_MAX];

static void __attribute__((constructor)) cache_allocator_init(void)
{
    u32 i, j;

    for (i = 0; i < SVM_MAX_DEV_NUM; i++) {
        for (j = 0; j < CACHE_TYPE_MAX; j++) {
            g_ca[i][j] = NULL;
        }
    }
}

static int cache_strategy_pack(u32 devid, u32 flag, struct cache_strategy *strategy)
{
    u64 page_size;
    int ret;

    if ((flag & SVM_CACHE_MALLOC_FLAG_PA_HPAGE) != 0) {
        ret = svm_dbi_query_hpage_size(devid, &page_size);
    } else {
        ret = svm_dbi_query_npage_size(devid, &page_size);
    }
    if (ret != DRV_ERROR_NONE) {
        svm_err("Dbi query failed. (devid=%u; flag=0x%x)\n", devid, flag);
        return ret;
    }

    if (devid == svm_get_host_devid()) {
        strategy->alloc_thres = 16ull * SVM_BYTES_PER_MB;
        strategy->shrink_thres_default = 16ull * SVM_BYTES_PER_MB;
    } else {
        if ((flag & SVM_CACHE_MALLOC_FLAG_PA_HPAGE) != 0) {
            strategy->alloc_thres = 32ull * SVM_BYTES_PER_MB;
            strategy->shrink_thres_default = 32ull * SVM_BYTES_PER_MB;
        } else {
            strategy->alloc_thres = 16ull * SVM_BYTES_PER_MB;
            strategy->shrink_thres_default = 16ull * SVM_BYTES_PER_MB;
        }
    }
    strategy->shrink_thres_cur = strategy->shrink_thres_default;
    strategy->expand_granularity = 2ull * SVM_BYTES_PER_MB;
    strategy->alloc_gran = page_size;

    return DRV_ERROR_NONE;
}

static struct cache_allocator *cache_allocator_alloc(u32 devid, u32 flag)
{
    struct cache_allocator *ca = NULL;
    struct svm_ga_attr attr;
    struct cache_strategy strategy = {0};

    if (cache_strategy_pack(devid, flag, &strategy) != DRV_ERROR_NONE) {
        return NULL;
    }

    ca = calloc(1, sizeof(struct cache_allocator));
    if (ca == NULL) {
        svm_err("Calloc failed. (size=%llu)\n", (u64)sizeof(struct cache_allocator));
        return NULL;
    }

    svm_ga_attr_pack(strategy.alloc_gran, &attr);
    ca->ga_inst = svm_ga_inst_create(&attr);
    if (ca->ga_inst == NULL) {
        svm_err("Ga inst create failed.\n");
        free(ca);
        return NULL;
    }

    (void)pthread_rwlock_init(&ca->rwlock, NULL);
    ca->strategy = strategy;
    ca->devid = devid;
    ca->flag = flag;
    return ca;
}

static void cache_allocator_free(struct cache_allocator *ca)
{
    svm_ga_inst_destroy(ca->ga_inst);
    free(ca);
}

struct cache_allocator *cache_get_allocator(u32 devid, u32 flag)
{
    u32 cache_type = cache_flag_to_cache_type(devid, flag);

    return g_ca[devid][cache_type];
}

static int cache_allocator_insert(struct cache_allocator *ca)
{
    u32 cache_type = cache_flag_to_cache_type(ca->devid, ca->flag);
    if (g_ca[ca->devid][cache_type] != NULL) {
        svm_err("Repeated init. (devid=%u; flag=0x%x)\n", ca->devid, ca->flag);
        return DRV_ERROR_REPEATED_INIT;
    }

    g_ca[ca->devid][cache_type] = ca;
    return DRV_ERROR_NONE;
}

static struct cache_allocator *cache_allocator_erase(u32 devid, u32 flag)
{
    struct cache_allocator *ca = NULL;
    u32 cache_type = cache_flag_to_cache_type(devid, flag);

    ca = g_ca[devid][cache_type];
    if (ca == NULL) {
        svm_warn("No such cache allocator. (devid=%u; flag=0x%x)\n", devid, flag);
        return NULL;
    }

    g_ca[devid][cache_type] = NULL;
    return ca;
}

int cache_allocator_create(u32 devid, u32 flag)
{
    struct cache_allocator *ca = NULL;
    int ret;

    ca = cache_get_allocator(devid, flag);
    if (ca != NULL) {
        return DRV_ERROR_NONE;
    }

    ca = cache_allocator_alloc(devid, flag);
    if (ca == NULL) {
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    ret = cache_allocator_insert(ca);
    if (ret != DRV_ERROR_NONE) {
        cache_allocator_free(ca);
    }
    return ret;
}

void cache_allocator_destroy(u32 devid, u32 flag)
{
    struct cache_allocator *ca = NULL;

    ca = cache_allocator_erase(devid, flag);
    if (ca != NULL) {
        cache_allocator_free(ca);
    }
}

static u32 _cache_allocator_show(struct cache_allocator *ca, char *buf, u32 buf_len)
{
    if (buf == NULL) {
        svm_info("flag 0x%x: shrink_thres_cur=%llu(Bytes) "
            "stats(total=%llu idle=%llu cur_alloced=%llu peak_alloced=%llu)(Bytes)\n",
            ca->flag, ca->strategy.shrink_thres_cur,
            ca->stats.total, ca->stats.idle, ca->stats.cur_alloced, ca->stats.peak_alloced);
        (void)svm_ga_show(ca->ga_inst, buf, buf_len);
        return 0;
    } else {
        int len;
        u32 ga_format_len;

        len = snprintf_s(buf, buf_len, buf_len - 1, "flag 0x%x: shrink_thres_cur=%llu(Bytes) "
            "stats(total=%llu idle=%llu cur_alloced=%llu peak_alloced=%llu)(Bytes)\n",
            ca->flag, ca->strategy.shrink_thres_cur,
            ca->stats.total, ca->stats.idle, ca->stats.cur_alloced, ca->stats.peak_alloced);
        if (len < 0) {
            return 0;
        }
        ga_format_len = svm_ga_show(ca->ga_inst, buf + len, buf_len - (u32)len);
        return (u32)len + ga_format_len;
    }
}

u32 cache_allocator_show(u32 devid, char *buf, u32 buf_len)
{
    struct cache_allocator *ca = NULL;
    u32 len = 0;
    u32 i;

    for (i = 0; i < CACHE_TYPE_MAX; i++) {
        ca = g_ca[devid][i];
        if (ca != NULL) {
            len += _cache_allocator_show(ca, buf + len, buf_len - len);
        }
    }

    return len;
}

struct cache_for_each_range {
    u32 devid;
    void *priv;
    int (*handle)(u32 devid, u64 start, u64 size, void *priv);
};

static int svm_cache_single_range_proc(u64 start, u64 size, void *priv)
{
    struct cache_for_each_range *para = (struct cache_for_each_range *)priv;

    return para->handle(para->devid, start, size, para->priv);
}

int svm_cache_for_each_range(u32 devid, int (*handle)(u32 devid, u64 start, u64 size, void *priv), void *priv)
{
    struct cache_for_each_range para = {.devid = devid, .priv = priv, .handle = handle};
    struct cache_allocator *ca = NULL;
    u32 i;
    int ret;

    if (devid >= SVM_MAX_DEV_NUM) {
        svm_err("Devid is invalid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    for (i = 0; i < CACHE_TYPE_MAX; i++) {
        ca = g_ca[devid][i];
        if (ca != NULL) {
            ret = svm_ga_for_each_range(ca->ga_inst, svm_cache_single_range_proc, (void *)&para);
            if (ret != 0) {
                return ret;
            }
        }
    }
    return 0;
}