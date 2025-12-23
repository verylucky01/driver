/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <securec.h>
#include <sys/mman.h>

#include "ascend_hal.h"
#include "dpa_user_interface.h"
#include "svm_adapt.h"
#include "svm_mem_statistics.h"

#ifndef EMU_ST
#define DEVMM_DRV_ERR(fmt, ...)         DRV_ERR(HAL_MODULE_TYPE_DEVMM, fmt, ##__VA_ARGS__)
#define DEVMM_DRV_DEBUG_ARG(fmt, ...)   DRV_DEBUG(HAL_MODULE_TYPE_DEVMM, fmt, ##__VA_ARGS__)
/* run log, the default log level is LOG_INFO. */
#define DEVMM_RUN_INFO(fmt, ...)        DRV_RUN_INFO(HAL_MODULE_TYPE_DEVMM, fmt, ##__VA_ARGS__)
#else
#define DEVMM_DRV_ERR(fmt, ...)
#define DEVMM_DRV_EVENT(fmt, ...)
#define DEVMM_DRV_DEBUG_ARG(fmt, ...)
#define DEVMM_RUN_INFO(fmt, ...)
#endif

SVM_DECLARE_MODULE_NAME(svm_module_name);

#define SVM_MEM_STATS_SHOW(mem_val, page_type, phy_memtype, devid, fmt, ...) do {                                    \
    if (mem_val != MEM_DEV_VAL) {                                                                                    \
        DEVMM_RUN_INFO("%s "fmt, svm_get_mem_type_str(mem_val, page_type, phy_memtype), ##__VA_ARGS__);              \
    } else {                                                                                                         \
        DEVMM_RUN_INFO("%s dev%d "fmt, svm_get_mem_type_str(mem_val, page_type, phy_memtype), devid, ##__VA_ARGS__); \
    }                                                                                                                \
} while (0)

static THREAD struct svm_mem_stats *g_mem_stats[MEM_STATS_DEVICE_CNT] = {NULL};
static pthread_mutex_t g_mem_stats_mutex[MEM_STATS_DEVICE_CNT];

__attribute__((constructor)) static void svm_mem_stats_mutex_init(void)
{
    uint32_t i;

    for (i = 0; i < MEM_STATS_DEVICE_CNT; ++i) {
        pthread_mutex_init(&g_mem_stats_mutex[i], NULL);
    }
}

void svm_init_mem_stats_mng(uint32_t devid)
{
    int err;
#ifndef EMU_ST
    if (devid >= MEM_STATS_DEVICE_CNT) {
        return;
    }
#endif
    (void)pthread_mutex_lock(&g_mem_stats_mutex[devid]);
    if (g_mem_stats[devid] == NULL) {
        struct svm_mem_stats *mmap_addr = (struct svm_mem_stats *)mmap(NULL, MEM_STATS_MNG_SIZE,
            PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (mmap_addr == MAP_FAILED) {
            (void)pthread_mutex_unlock(&g_mem_stats_mutex[devid]);
            err = errno;
            DEVMM_RUN_INFO("g_mem_stats mmap check. (errno=%d; errstr=%s)\n", err, strerror(err));
            return;
        }
        (void)memset_s(mmap_addr, MEM_STATS_MNG_SIZE, 0, MEM_STATS_MNG_SIZE);
        g_mem_stats[devid] = mmap_addr;
    }
    (void)pthread_mutex_unlock(&g_mem_stats_mutex[devid]);
    return;
}

void svm_uninit_mem_stats_mng(uint32_t devid)
{
    if (devid >= MEM_STATS_DEVICE_CNT) {
        return;
    }

    (void)pthread_mutex_lock(&g_mem_stats_mutex[devid]);
    if (g_mem_stats[devid] != NULL) {
        munmap(g_mem_stats[devid], MEM_STATS_MNG_SIZE);
        g_mem_stats[devid] = NULL;
    }
    (void)pthread_mutex_unlock(&g_mem_stats_mutex[devid]);
}

uint64_t svm_get_mem_stats_va(uint32_t devid)
{
    return (uint64_t)(uintptr_t)g_mem_stats[devid];
}

static struct svm_mem_stats *_svm_get_mem_stats_mng(struct svm_mem_stats_type *type, uint32_t devid)
{
    return g_mem_stats[devid] + (type->mem_val * MEM_STATS_MAX_PAGE_TYPE + type->page_type) *
        MEM_STATS_MAX_PHY_MEMTYPE + type->phy_memtype;
}

static struct svm_mem_stats *svm_get_mem_stats_mng(struct svm_mem_stats_type *type, uint32_t devid)
{
#ifndef EMU_ST
    if (devid >= MEM_STATS_DEVICE_CNT) {
        return NULL;
    }
#endif

    if (g_mem_stats[devid] != NULL) {
        return _svm_get_mem_stats_mng(type, devid);
    }

    svm_init_mem_stats_mng(devid);
    return (g_mem_stats[devid] != NULL) ? _svm_get_mem_stats_mng(type, devid) : NULL;
}

#ifdef EMU_ST
__attribute__((destructor)) static void svm_destory_mem_stats_mng(void)
{
    uint32_t i;

    for (i = 0; i < MEM_STATS_DEVICE_CNT; ++i) {
        if (g_mem_stats[i] != NULL) {
            munmap(g_mem_stats[i], MEM_STATS_MNG_SIZE);
        }
    }
}
#endif

static void _svm_mem_stats_show(uint32_t mem_val, uint32_t page_type, uint32_t phy_memtype, uint32_t devid)
{
    struct svm_mem_stats *mem_stats = NULL;
    struct svm_mem_stats_type type;
    uint64_t mapped_size;
    uint64_t total_alloced_size = 0, cached_size;
    uint32_t module_id;
    bool show_stats = false;

    svm_mem_stats_type_pack(&type, mem_val, page_type, phy_memtype);
    mem_stats = svm_get_mem_stats_mng(&type, devid);
    if (mem_stats == NULL) {
        return;
    }
    mapped_size = mem_stats->mapped_size;
    for (module_id = 0; module_id < SVM_MAX_MODULE_ID; module_id++) {
        if (mem_stats->alloced_peak_size[module_id] != 0) {
            show_stats = true;
            SVM_MEM_STATS_SHOW(mem_val, page_type, phy_memtype, devid, "Mem stats (Bytes). (module_name=%s; module_id=%u; "
                "current_alloced_size=%llu; allocated_peak_size=%llu; alloc_cnt=%llu; free_cnt=%llu)\n",
                SVM_GET_MODULE_NAME(svm_module_name, module_id), module_id, mem_stats->current_alloced_size[module_id],
                mem_stats->alloced_peak_size[module_id], mem_stats->alloc_cnt[module_id],
                mem_stats->free_cnt[module_id]);
        }
        total_alloced_size += mem_stats->current_alloced_size[module_id];
    }
    cached_size = (mapped_size >= total_alloced_size) ? (mapped_size - total_alloced_size) : 0;
    if ((mem_val != MEM_SVM_VAL) && (show_stats == true) && (cached_size != 0)) {
        SVM_MEM_STATS_SHOW(mem_val, page_type, phy_memtype, devid, "Cached_size:%lluBytes\n", cached_size);
    }
}

void svm_mem_stats_show_all_svm_mem(uint32_t devid)
{
    uint32_t page_type, phy_memtype;

    _svm_mem_stats_show(MEM_SVM_VAL, 0, 0, 0);
    _svm_mem_stats_show(MEM_HOST_VAL, 0, 0, 0);
    for (page_type = 0; page_type < MEM_STATS_MAX_PAGE_TYPE; page_type++) {
        for (phy_memtype = 0; phy_memtype < MEM_STATS_MAX_PHY_MEMTYPE; phy_memtype++) {
            _svm_mem_stats_show(MEM_DEV_VAL, page_type, phy_memtype, devid);
        }
    }
}

void svm_mem_stats_show_host(void)
{
    _svm_mem_stats_show(MEM_SVM_VAL, 0, 0, 0);
    _svm_mem_stats_show(MEM_HOST_VAL, 0, 0, 0);
}

#if defined(DRV_HOST) || defined(DEVMM_UT)
static void svm_mem_stats_update_type_and_devid(struct svm_mem_stats_type *type, uint32_t *devid)
{
    type->mem_val = (type->mem_val >= MEM_STATS_MAX_MEM_VAL) ? 0 : type->mem_val;
    type->page_type = (type->mem_val != MEM_DEV_VAL) ? 0 : type->page_type;
    type->phy_memtype = (type->mem_val != MEM_DEV_VAL) ? 0 : type->phy_memtype;
    /* MEM_HOST_VAL not care about devid, MEM_SVM_VAL only care about virt_mem, so reuse the space of dev0 index. */
    *devid = ((type->mem_val != MEM_DEV_VAL) || (*devid >= MEM_STATS_DEVICE_CNT)) ? 0 : *devid;
}
#else
static void svm_mem_stats_update_type_and_devid(struct svm_mem_stats_type *type, uint32_t *devid)
{
    type->mem_val = 0;
    *devid = (*devid >= MEM_STATS_DEVICE_CNT) ? 0 : *devid;
}
#endif

static void svm_module_used_size_update(uint32_t devid, uint32_t module_id, uint64_t size, bool add_or_sub)
{
    static uint64_t dppg_sample_size[MEM_STATS_DEVICE_CNT][SVM_MAX_MODULE_ID] = {{0}};

    if (drv_mem_support_prof_sample()) {
        if (dp_proc_mng_get_prof_start_sample_flag() == false) {
            return;
        }
    }

    if (add_or_sub) {
        (void)__sync_add_and_fetch((volatile long long *)(uintptr_t)&dppg_sample_size[devid][module_id], (long long)size);
    } else {
        (void)__sync_sub_and_fetch((volatile long long *)(uintptr_t)&dppg_sample_size[devid][module_id], (long long)size);
    }

    dp_proc_mng_module_used_size_update(devid, module_id, dppg_sample_size[devid][module_id]);
}

void svm_module_alloced_size_inc(struct svm_mem_stats_type *type, uint32_t devid, uint32_t module_id, uint64_t size)
{
    uint32_t module_id_tmp = (module_id >= SVM_MAX_MODULE_ID) ? UNKNOWN_MODULE_ID : module_id;
    struct svm_mem_stats *mem_stats = NULL;
    volatile long long *alloced_peak_size = NULL;
    volatile long long tmp;
    uint32_t in_mem_val = type->mem_val;

    if (drv_mem_support_prof_sample()) {
        if (devid >= MEM_STATS_DEVICE_CNT) {
            DEVMM_DRV_DEBUG_ARG("Invalid devid. (devid=%u)\n", devid);
            return;
        }

        if (dp_proc_mng_get_prof_start_sample_flag() == false) {
            return;
        }
    }

    svm_mem_stats_update_type_and_devid(type, &devid);
    mem_stats = svm_get_mem_stats_mng(type, devid);
    if (mem_stats == NULL) {
        return;
    }

    tmp = __sync_add_and_fetch((volatile long long *)(uintptr_t)&mem_stats->current_alloced_size[module_id_tmp], (long long)size);
    alloced_peak_size = (volatile long long *)(uintptr_t)&mem_stats->alloced_peak_size[module_id_tmp];

    while (1) {
        volatile long long peak_size = *alloced_peak_size;

        if (tmp <= peak_size) {
            break;
        }

        if (__sync_bool_compare_and_swap(alloced_peak_size, peak_size, tmp) == true) {
            break;
        }
    }

    if (in_mem_val == MEM_DEV_VAL) {
        svm_module_used_size_update(devid, module_id_tmp, size, true);
    }

    if (drv_mem_support_alloc_cnt_stats()) {
        (void)__sync_add_and_fetch((volatile long long *)(uintptr_t)&mem_stats->alloc_cnt[module_id_tmp], 1);
    }

    DEVMM_DRV_DEBUG_ARG("Alloc. (devid=%u; module_id=%u; allocated_size=%llu; peak_size=%llu; size=%llu; mem_val=%u; "
        "page_type=%u; phy_memtype=%u; alloc_cnt=%llu; free_cnt=%llu)\n", devid, module_id_tmp,
        mem_stats->current_alloced_size[module_id_tmp], mem_stats->alloced_peak_size[module_id_tmp], size,
        type->mem_val, type->page_type, type->phy_memtype,
        mem_stats->alloc_cnt[module_id_tmp], mem_stats->free_cnt[module_id_tmp]);
}

void svm_module_alloced_size_dec(struct svm_mem_stats_type *type, uint32_t devid, uint32_t module_id, uint64_t size)
{
    uint32_t module_id_tmp = (module_id >= SVM_MAX_MODULE_ID) ? UNKNOWN_MODULE_ID : module_id;
    struct svm_mem_stats *mem_stats = NULL;
    uint32_t in_mem_val = type->mem_val;

    if (drv_mem_support_prof_sample()) {
        if (devid >= MEM_STATS_DEVICE_CNT) {
            DEVMM_DRV_DEBUG_ARG("Invalid devid. (devid=%u)\n", devid);
            return;
        }

        if (dp_proc_mng_get_prof_start_sample_flag() == false) {
            return;
        }
    }

    svm_mem_stats_update_type_and_devid(type, &devid);
    mem_stats = svm_get_mem_stats_mng(type, devid);
    if (mem_stats == NULL) {
        return;
    }

    drv_mem_current_alloc_size_stats(mem_stats, module_id_tmp, size);

    if (in_mem_val == MEM_DEV_VAL) {
        svm_module_used_size_update(devid, module_id_tmp, size, false);
    }

    if (drv_mem_support_alloc_cnt_stats()) {
        (void)__sync_add_and_fetch((volatile long long *)(uintptr_t)&mem_stats->free_cnt[module_id_tmp], 1);
    }

    DEVMM_DRV_DEBUG_ARG("Free. (devid=%u; module_id=%u; allocated_size=%llu; peak_size=%llu; size=%llu; mem_val=%u; "
        "page_type=%u; phy_memtype=%u; alloc_cnt=%llu; free_cnt=%llu)\n", devid, module_id_tmp,
        mem_stats->current_alloced_size[module_id_tmp], mem_stats->alloced_peak_size[module_id_tmp], size,
        type->mem_val, type->page_type, type->phy_memtype,
        mem_stats->alloc_cnt[module_id_tmp], mem_stats->free_cnt[module_id_tmp]);
}

void svm_mapped_size_inc(struct svm_mem_stats_type *type, uint32_t devid, uint64_t size)
{
    struct svm_mem_stats *mem_stats = NULL;

    svm_mem_stats_update_type_and_devid(type, &devid);
    mem_stats = svm_get_mem_stats_mng(type, devid);
    if (mem_stats == NULL) {
        return;
    }
    __sync_add_and_fetch((volatile long long *)(uintptr_t)&mem_stats->mapped_size, (long long)size);
}

void svm_mapped_size_dec(struct svm_mem_stats_type *type, uint32_t devid, uint64_t size)
{
    struct svm_mem_stats *mem_stats = NULL;

    svm_mem_stats_update_type_and_devid(type, &devid);
    mem_stats = svm_get_mem_stats_mng(type, devid);
    if (mem_stats == NULL) {
        return;
    }
    __sync_sub_and_fetch((volatile long long *)(uintptr_t)&mem_stats->mapped_size, (long long)size);
}

static void svm_mem_stats_show_device_svm_mem(uint32_t devid)
{
    uint32_t page_type, phy_memtype;

#if defined(DRV_HOST) || defined(DEVMM_UT)
    _svm_mem_stats_show(MEM_SVM_VAL, 0, 0, 0);
#endif

    for (page_type = 0; page_type < MEM_STATS_MAX_PAGE_TYPE; page_type++) {
        for (phy_memtype = 0; phy_memtype < MEM_STATS_MAX_PHY_MEMTYPE; phy_memtype++) {
#if defined(DRV_HOST) || defined(DEVMM_UT)
            _svm_mem_stats_show(MEM_DEV_VAL, page_type, phy_memtype, devid);
#else
            _svm_mem_stats_show(0, page_type, phy_memtype, devid);
#endif
        }
    }
}

#define DEVMM_DEV_PROC_MEM_STATS_TYPE_NUM  4
static void svm_mem_stats_show_device_proc_mem(uint32_t devid)
{
    u32 dev_proc_mem_type[DEVMM_DEV_PROC_MEM_STATS_TYPE_NUM] = {AICPU_SCHE_MODULE_ID, CUSTOM_SCHE_MODULE_ID,
        HCCP_SCHE_MODULE_ID, MBUFF_MODULE_ID};
    struct module_mem_info *mem_info = NULL;
    u32 i;
    int ret;
#ifndef EMU_ST

    mem_info = (struct module_mem_info *)calloc(MEM_STATS_MAX_MODULE_ID, sizeof(struct module_mem_info));
    if (mem_info == NULL) {
        return;
    }
    ret = dp_proc_mng_mem_stats_sample(mem_info, MEM_STATS_MAX_MODULE_ID, devid);
    if (ret != 0) {
        free(mem_info);
        return;
    }

    for (i = 0; i < DEVMM_DEV_PROC_MEM_STATS_TYPE_NUM; i++) {
        u32 index = dev_proc_mem_type[i];

        if (mem_info[index].total_size != 0) {
            DEVMM_RUN_INFO("DEV_PROC_MEM dev%u Mem stats (Bytes). (module_name=%s; module_id=%u; total_size=%lu)\n",
                devid, SVM_GET_MODULE_NAME(svm_module_name, mem_info[index].module_id),
                mem_info[index].module_id, mem_info[index].total_size);
        }
    }

    free(mem_info);
#endif
}

void svm_mem_stats_show_device(uint32_t devid)
{
    if (devid >= MEM_STATS_DEVICE_CNT) {
        DEVMM_DRV_ERR("Devid is invalid. (devid=%u)\n", devid);
        return;
    }

    svm_mem_stats_show_device_svm_mem(devid);
    svm_mem_stats_show_device_proc_mem(devid);
}

static void svm_mem_module_stats_get(uint32_t mem_val, uint32_t devid, uint32_t module_id, uint64_t *cur_size, uint64_t *peak_size)
{
    struct svm_mem_stats *mem_stats = NULL;
    struct svm_mem_stats_type type;
    uint64_t total_peak_size = 0;
    uint64_t total_cur_size = 0;
    uint32_t page_type, phy_memtype;
    uint32_t max_page_type, max_phy_memtype;

    /* host only needs to count one page_type */
    max_page_type = (mem_val != MEM_DEV_VAL) ? 1 : MEM_STATS_MAX_PAGE_TYPE;
    max_phy_memtype = (mem_val != MEM_DEV_VAL) ? 1 : MEM_STATS_MAX_PHY_MEMTYPE;

    for (page_type = 0; page_type < max_page_type; page_type++) {
        for (phy_memtype = 0; phy_memtype < max_phy_memtype; phy_memtype++) {
            svm_mem_stats_type_pack(&type, mem_val, page_type, phy_memtype);
            /* adapt host and device svm implementation difference statistics */
            svm_mem_stats_update_type_and_devid(&type, &devid);
            mem_stats = svm_get_mem_stats_mng(&type, devid);
            if (mem_stats == NULL) {
                break;
            }
            total_peak_size += mem_stats->alloced_peak_size[module_id];
            total_cur_size += mem_stats->current_alloced_size[module_id];
        }
    }

    *cur_size = total_cur_size;
    *peak_size = total_peak_size;
}

static void svm_mem_module_usage_info_pack(uint32_t module_id, uint64_t cur_size, uint64_t peak_size, struct mem_module_usage *usage_info)
{
    usage_info->cur_mem_size = cur_size;
    usage_info->mem_peak_size = peak_size;
    (void)strcpy_s(usage_info->name, sizeof(usage_info->name), SVM_GET_MODULE_NAME(svm_module_name, module_id));
}

static void svm_mem_usage_info_sort_insert(struct mem_module_usage new_info, struct mem_module_usage *mem_info, size_t cur_num, size_t max_num)
{
    size_t i, tmp_index;

    for (i = 0; i < cur_num; i++) {
        if (new_info.cur_mem_size > mem_info[i].cur_mem_size) {
            break;
        }
    }
    /* Discard data exceeding the maximum array */
    if ((i == max_num) || (cur_num > max_num)) {
        return;
    }
    tmp_index = i;
    /* exceeding array max num remove the last value */
    i = (cur_num == max_num) ? (cur_num - 1) : cur_num;
    for (; i > tmp_index; i--) {
        mem_info[i] = mem_info[i - 1];
    }
    mem_info[tmp_index] = new_info;
}

drvError_t halGetMemUsageInfo(uint32_t dev_id, struct mem_module_usage *mem_usage, size_t in_num, size_t *out_num)
{
    struct mem_module_usage tem_usage_info;
    uint64_t cur_size, peak_size;
    uint32_t tmp_dev_id, module_id, mem_val;
    uint32_t cur_module_num = 0;
    uint32_t host_id = 65;  /* 65 host id */

    if (dev_id >= MEM_STATS_DEVICE_CNT || mem_usage == NULL || out_num == NULL || in_num == 0) {
        return DRV_ERROR_INVALID_VALUE;
    }

    (void)halGetHostID(&host_id);
    tmp_dev_id = (dev_id == host_id) ? 0: dev_id;
    mem_val = (dev_id == host_id) ? MEM_HOST_VAL : MEM_DEV_VAL;

    for (module_id = 0; module_id < SVM_MAX_MODULE_ID; module_id++) {
        svm_mem_module_stats_get(mem_val, tmp_dev_id, module_id, &cur_size, &peak_size);
        if (peak_size == 0) {
            continue;
        }
        svm_mem_module_usage_info_pack(module_id, cur_size, peak_size, &tem_usage_info);
        svm_mem_usage_info_sort_insert(tem_usage_info, mem_usage, cur_module_num, in_num);
        /* If the in_num provided by the user is less than the number of modules that occupy non-zero memory,
        need continue to get the memory usage information of in_num modules in descending order based on current memory usage. */
        if (cur_module_num < in_num) {
            cur_module_num++;
        }
    }
    *out_num = cur_module_num;
    return DRV_ERROR_NONE;
}