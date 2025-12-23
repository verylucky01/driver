/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#define _GNU_SOURCE
#include <sched.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/prctl.h>

#include "dms_user_interface.h"
#include "drv_buff_adp.h"
#include "drv_user_common.h"
#include "buff_event.h"
#include "drv_user_common.h"
#include "buff_memzone_adapt.h"
#include "buff_large_buf_adapt.h"
#include "grp_mng.h"

#include "buff_manage_base.h"
#include "buff_mng.h"
#include "buff_user_interface.h"
#include "buff_recycle.h"
#include "drv_buff_memzone.h"

#define ATOMIC_SET(x, y) __sync_lock_test_and_set((x), (y))

static unsigned int g_mz_cfg_num = 0;

static memZoneCfg g_mz_cfg[BUFF_MAX_MZ_NODE_NUM] = {};

static memZoneCfg g_mz_cfg_default[] = {
    { 0, 2 * 1024 * 1024, 256, 8 * 1024, BUFF_SP_NORMAL, 0, 0, 0, 0, 0, {0}},
    { 1, 32 * 1024 * 1024, 8 * 1024, 8 * 1024 * 1024, BUFF_SP_NORMAL, 0, 0, 0, 0, 0, {0}},
#ifdef CFG_FEATURE_SURPORT_HUGE_PAGE
    { 2, 2 * 1024 * 1024, 256, 8 * 1024, BUFF_SP_HUGEPAGE_ONLY, 0, 0, 0, 0, 0, {0}},
    { 3, MZ_DEFAULT_CFG_MAX_SIZE, 8 * 1024, 64 * 1024 * 1024, BUFF_SP_HUGEPAGE_ONLY, 0, 0, 0, 0, 0, {0}}
#endif
};


/* dvpp mem will be configed when user doesn't config dvpp mem. */
// dvpp mem cfg same to non-dvpp cfg
#ifdef CFG_FEATURE_NO_SURPORT_DVPP_MZ
static memZoneCfg g_mz_cfg_dvpp_default[] = {
    { 0, 2 * 1024 * 1024, 256, 8 * 1024, BUFF_SP_NORMAL | BUFF_SP_DVPP, 0, 0, 0, 0, 0, {0}},
    { 1, 32 * 1024 * 1024, 8 * 1024, 8 * 1024 * 1024, BUFF_SP_NORMAL | BUFF_SP_DVPP, 0, 0, 0, 0, 0, {0}},
#ifdef CFG_FEATURE_SURPORT_HUGE_PAGE
    { 2, 2 * 1024 * 1024, 256, 8 * 1024, BUFF_SP_HUGEPAGE_ONLY | BUFF_SP_DVPP, 0, 0, 0, 0, 0, {0}},
    { 3, MZ_DEFAULT_CFG_MAX_SIZE, 8 * 1024, 64 * 1024 * 1024, BUFF_SP_HUGEPAGE_ONLY | BUFF_SP_DVPP, 0, 0, 0, 0, 0, {0}}
#endif
};
#else
static memZoneCfg g_mz_cfg_dvpp_default[] = {
    { 0, 2 * 1024 * 1024, 4 * 1024, 384 * 1024, BUFF_SP_NORMAL | BUFF_SP_DVPP, 0, 0, 0, 0, 0, {0}},
#ifdef CFG_FEATURE_SURPORT_HUGE_PAGE
    { 1, 2 * 1024 * 1024, 4 * 1024, 384 * 1024, BUFF_SP_HUGEPAGE_ONLY | BUFF_SP_DVPP, 0, 0, 0, 0, 0, {0}},
#endif
};
#endif
// dvpp mem cfg same to non-dvpp cfg
#ifdef CFG_FEATURE_NO_SURPORT_DVPP_MZ
static memZoneCfg g_mz_cfg_cache_default[] = {
    { 0, 2 * 1024 * 1024, 256, 8 * 1024, BUFF_SP_NORMAL, 1, 0, 0, 0, 6, {0}},
    { 1, 32 * 1024 * 1024, 8 * 1024, 8 * 1024 * 1024, BUFF_SP_NORMAL, 1, 0, 0, 0, 6, {0}},
#ifdef CFG_FEATURE_SURPORT_HUGE_PAGE
    { 2, 2 * 1024 * 1024, 256, 8 * 1024, BUFF_SP_HUGEPAGE_ONLY, 1, 0, 0, 0, 6, {0}},
    { 3, MZ_DEFAULT_CFG_MAX_SIZE, 8 * 1024, 64 * 1024 * 1024, BUFF_SP_HUGEPAGE_ONLY, 1, 0, 0, 0, 6, {0}},
#endif
    { 4, 2 * 1024 * 1024, 256, 8 * 1024, BUFF_SP_NORMAL | BUFF_SP_DVPP, 1, 0, 0, 0, 6, {0}},
    { 5, 32 * 1024 * 1024, 8 * 1024, 8 * 1024 * 1024, BUFF_SP_NORMAL | BUFF_SP_DVPP, 1, 0, 0, 0, 6, {0}},
#ifdef CFG_FEATURE_SURPORT_HUGE_PAGE
    { 6, 2 * 1024 * 1024, 256, 8 * 1024, BUFF_SP_HUGEPAGE_ONLY | BUFF_SP_DVPP, 1, 0, 0, 0, 6, {0}},
    { 7, MZ_DEFAULT_CFG_MAX_SIZE, 8 * 1024, 64 * 1024 * 1024, BUFF_SP_HUGEPAGE_ONLY | BUFF_SP_DVPP, 1, 0, 0, 0, 6, {0}},
#endif
};
#else
static memZoneCfg g_mz_cfg_cache_default[] = {
    { 0, 2 * 1024 * 1024, 256, 8 * 1024, BUFF_SP_NORMAL, 1, 0, 0, 0, 6},
    { 1, 32 * 1024 * 1024, 8 * 1024, 8 * 1024 * 1024, BUFF_SP_NORMAL, 1, 0, 0, 0, 6},
#ifdef CFG_FEATURE_SURPORT_HUGE_PAGE
    { 2, 2 * 1024 * 1024, 256, 8 * 1024, BUFF_SP_HUGEPAGE_ONLY, 1, 0, 0, 0, 6},
    { 3, MZ_DEFAULT_CFG_MAX_SIZE, 8 * 1024, 64 * 1024 * 1024, BUFF_SP_HUGEPAGE_ONLY, 1, 0, 0, 0, 6},
#endif
    { 4, 2 * 1024 * 1024, 4 * 1024, 384 * 1024, BUFF_SP_NORMAL | BUFF_SP_DVPP, 1, 0, 0, 0, 6},
#ifdef CFG_FEATURE_SURPORT_HUGE_PAGE
    { 5, 2 * 1024 * 1024, 4 * 1024, 384 * 1024, BUFF_SP_HUGEPAGE_ONLY | BUFF_SP_DVPP, 1, 0, 0, 0, 6},
#endif
};
#endif


/* memtype definition */
#define MEMTYPE_NORMAL 0
#define MEMTYPE_DVPP 1
#define MEMTYPE_MAX_NUM 2

STATIC THREAD bool buff_already_init = false;
static THREAD int g_self_pid = -1;
static THREAD pthread_mutex_t g_buff_grp_init_mutex = PTHREAD_MUTEX_INITIALIZER;

int halBuffInit(BuffCfg *cfg)
{
    drvError_t ret;

    ret = buff_is_support();
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    if (cfg == NULL) {
        buff_err("Cfg is NULL.\n");
        return (int)DRV_ERROR_INVALID_VALUE;
    }

    (void)pthread_mutex_lock(&g_buff_grp_init_mutex);
    if ((buff_already_init == true) && (g_self_pid == getpid())) {
        (void)pthread_mutex_unlock(&g_buff_grp_init_mutex);
        buff_warn("Buff already init.\n");
        return (int)DRV_ERROR_REPEATED_INIT;
    }

    buff_already_init = false;

    ret = memzone_cfg(cfg->cfg);
    if (ret != DRV_ERROR_NONE) {
        (void)pthread_mutex_unlock(&g_buff_grp_init_mutex);
        buff_err("Memzone config err. (ret=%d)\n", ret);
        return (int)ret;
    }

    buff_already_init = true;
    g_self_pid = getpid();
    (void)pthread_mutex_unlock(&g_buff_grp_init_mutex);
    return (int)DRV_ERROR_NONE;
}

void memzone_elastic_cfg_init(struct buff_memzone_list_node* mz_list_node)
{
    uint32_t i;

    for (i = 0; i < g_mz_cfg_num; i++) {
        mz_list_node[i].elastic_enable = (uint32_t)g_mz_cfg[i].elasticEnable;
        mz_list_node[i].elastic_low_level = (uint32_t)g_mz_cfg[i].elasticLowLevel;
    }
}

THREAD struct memzone_cfg_max_buf_info g_mz_max_buf[MEMTYPE_MAX_NUM] = {{0}};

uint64_t* memzone_get_buff_head_by_index(struct memzone_user_mng_t *memzone, unsigned int idx)
{
    uint64_t *head = NULL;

    head = (uint64_t *)((uintptr_t)memzone->bitmap + bitmap_size(memzone->bitnum));

    return (head + idx);
}

void memzone_delete(struct memzone_user_mng_t *mz)
{
    struct buff_req_mz_delete arg;
    drvError_t ret = 0;

    arg.mz_user_mng_uva = (uintptr_t)mz;
    buff_scale_in(mz->area.mz_mem_uva, mz->area.mz_mem_total_size);

    ret = buff_usr_mz_delete(&arg);
    if (ret != DRV_ERROR_NONE) {
        buff_err("mz delete failed, ret:0x%x\n", ret);
    }

    return;
}

static drvError_t memzone_create(memZoneCfg *cfg_info, int grp_id,
    struct buff_memzone_list_node *mz_list_node, struct memzone_user_mng_t **memzone)
{
    struct buff_req_mz_create arg;
    drvError_t ret;

    arg.devid = buff_get_devid_by_mz_list(mz_list_node);
    arg.total_size = cfg_info->total_size;
    arg.blk_size = cfg_info->blk_size;
    arg.page_type = cfg_info->page_type;
    arg.cfg_id = cfg_info->cfg_id;
    arg.grp_id = grp_id;
    arg.sp_flag = cfg_info->page_type;
    arg.mz_list_node = mz_list_node;
    ret = buff_usr_mz_create(&arg);
    if (ret != DRV_ERROR_NONE) {
        buff_event("Can not create mz. (ret=0x%x; errno=%d)\n", ret, errno);
        return ret;
    }

    *memzone = (struct memzone_user_mng_t *)(uintptr_t)(arg.mz_uva);

    buff_scale_out(arg.start, arg.total_size);

    return DRV_ERROR_NONE;
}

STATIC uint32_t memzone_get_memtype(uint32 pg_type)
{
    if ((pg_type & BUFF_SP_DVPP) != 0) {
        return MEMTYPE_DVPP;
    } else {
        return MEMTYPE_NORMAL;
    }
}

STATIC uint64_t memzone_get_max_buf_size(uint32 pg_type)
{
    uint64_t max_buf_size = 0;
    uint32_t memtype;

    memtype = memzone_get_memtype(pg_type);
    if ((pg_type & BUFF_SP_HUGEPAGE_PRIOR) == BUFF_SP_HUGEPAGE_PRIOR) {
        max_buf_size = g_mz_max_buf[memtype].huge_prior;
    } else if ((pg_type & BUFF_SP_HUGEPAGE_ONLY) == BUFF_SP_HUGEPAGE_ONLY) {
        max_buf_size = g_mz_max_buf[memtype].huge_only;
    } else {
        max_buf_size = g_mz_max_buf[memtype].normal;
    }

    return max_buf_size;
}

STATIC uint32 buff_get_pagetype_from_flags(uint32 flags)
{
    uint32 page_type = 0;

    if ((flags & (BUFF_SP_HUGEPAGE_PRIOR | BUFF_SP_HUGEPAGE_ONLY)) == 0) {
        page_type |= BUFF_SP_NORMAL;
    } else {
        page_type |= BUFF_SP_HUGEPAGE_ONLY;
    }

    if ((flags & BUFF_SP_DVPP) == BUFF_SP_DVPP) {
        page_type |= BUFF_SP_DVPP;
    }
    return page_type;
}

STATIC memZoneCfg *memzone_get_cmpat_cfg_info(uint64_t size, uint32 flags)
{
    uint32 page_type, i;

    page_type = buff_get_pagetype_from_flags(flags);

    for (i = 0; i < g_mz_cfg_num; i++) {
        if (g_mz_cfg[i].page_type != page_type) {
            continue;
        }

        if (g_mz_cfg[i].max_buf_size >= size) {
            break;
        }
    }

    if (i >= g_mz_cfg_num) {
        buff_err("Can't find compatible mz cfg info. (size=%llu; flags=%u; type=%u; i=%u; g_mz_cfg_num=%u)\n",
            size, flags, page_type, i, g_mz_cfg_num);
        return NULL;
    }

    if (g_mz_cfg[i].cfg_id >= g_mz_cfg_num) {
        buff_err("Mz cfg illegal. (i=%u; cfg_id=%u)\n", i, g_mz_cfg[i].cfg_id);
        return NULL;
    }

    return &g_mz_cfg[i];
}

memZoneCfg *memzone_get_cfg_info_by_id(unsigned int cfg_id)
{
    return &g_mz_cfg[cfg_id];
}

static inline bool memzone_is_small_page_type(unsigned int page_type)
{
    return ((page_type == BUFF_SP_NORMAL) || (page_type == (BUFF_SP_NORMAL | BUFF_SP_DVPP)));
}

static inline bool memzone_is_huge_page_type(unsigned int page_type)
{
    return ((page_type == BUFF_SP_HUGEPAGE_ONLY) || (page_type == (BUFF_SP_HUGEPAGE_ONLY | BUFF_SP_DVPP)));
}

static bool memzone_page_type_is_valid(memZoneCfg *cfg_info, unsigned int idx)
{
    if (memzone_is_small_page_type(cfg_info->page_type) == true) {
        if (cfg_info->total_size == 0 || cfg_info->total_size % MZ_PAGE_SIZE_NORMAL != 0) {
            buff_err("Small page total_size is error. (total_size=0x%llx; page_type=%u; idx=%u)\n",
                cfg_info->total_size, cfg_info->page_type, idx);
            return false;
        }
    } else if (memzone_is_huge_page_type(cfg_info->page_type) == true) {
        if (cfg_info->total_size == 0 || cfg_info->total_size % MZ_PAGE_SIZE_HUGE != 0) {
            buff_err("Huge page total_size is error. (total_size=0x%llx; page_type=%u; idx=%u)\n",
                cfg_info->total_size, cfg_info->page_type, idx);
            return false;
        }
    } else {
        buff_err("page_type is error, page_type:%u, idx:%u\n", cfg_info->page_type, idx);
        return false;
    }
    return true;
}

static drvError_t memzone_cfg_is_valid(memZoneCfg *cfg_info, unsigned int idx)
{
    /* judge cfg_id  */
    if (cfg_info->cfg_id != idx) {
        buff_err("cfg id is error, cfg_id:%u, idx:%d\n", cfg_info->cfg_id, idx);
        return DRV_ERROR_INVALID_VALUE;
    }

    /* judge blk_size */
    if ((cfg_info->blk_size == 0) || !buff_is_power_of_2(cfg_info->blk_size) ||
        (cfg_info->blk_size > MZ_BLK_SIZE_MAX)) {
        buff_err("blk_size is error, blk_size:0x%x, idx:%u\n", cfg_info->blk_size, idx);
        return DRV_ERROR_INVALID_VALUE;
    }

    /* judge page_type and mz_total_size */
    if (memzone_page_type_is_valid(cfg_info, idx) == false) {
        return DRV_ERROR_INVALID_VALUE;
    }

    /* judge max_buf_size */
    if (cfg_info->max_buf_size >= cfg_info->total_size) {
        buff_err("max_buf_size is error, max_buf_size:0x%llx, mz_total_size:0x%llx, idx:%u\n",
            cfg_info->max_buf_size, cfg_info->total_size, idx);
        return DRV_ERROR_INVALID_VALUE;
    }

    /* judge elastic parameter */
    if ((cfg_info->elasticEnable != 0) && (cfg_info->elasticLowLevel < 0)) {
        buff_err("Elastic Low Level is invalid. (Elastic_low_level=%d; elastic_enable=%d; idx=%u)\n",
            cfg_info->elasticLowLevel, cfg_info->elasticEnable, idx);
        return DRV_ERROR_INVALID_VALUE;
    }

    /* judge mz_total_size and blk_size */
    if (cfg_info->total_size % cfg_info->blk_size != 0) {
        buff_err("mz_total_size and blk_size is error, mz_total_size:0x%llx, blk_size:0x%x, idx:%u\n",
            cfg_info->total_size, cfg_info->blk_size, idx);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (cfg_info->max_buf_size < cfg_info->blk_size) {
        buff_err("max_buf_size is error, cfg_info->max_buf_size: 0x%llx, blk_size:0x%x, idx:%u\n",
            cfg_info->max_buf_size, cfg_info->blk_size, idx);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

#define BUFF_MAX_MEMTYPE_PAGETYPE_NUM 16 /* include normal/dvpp, huge/small page */
static drvError_t memzone_cfg_para_check(memZoneCfg *cfg, uint32_t *cfg_num_out)
{
    uint64_t cur_max_buf[BUFF_MAX_MEMTYPE_PAGETYPE_NUM] = {0};
    uint32_t cfg_num = 0;
    uint32_t i;

    for (i = 0; i < (uint32_t)BUFF_MAX_CFG_NUM; i++) {
        drvError_t ret;

        if ((cfg[i].cfg_id == 0) && (cfg[i].total_size == 0)) {
            break;
        }
        ret = memzone_cfg_is_valid(&cfg[i], i);
        if (ret != DRV_ERROR_NONE) {
            buff_err("Cfg is invalid. (i=%u)\n", i);
            return ret;
        }

        if (cfg[i].max_buf_size <= cur_max_buf[cfg[i].page_type]) {
            buff_err("Max_buf_size must increase. (i=%u; max_buf_size=0x%llx; previous_max_buf=0x%llx)\n",
                i, cfg[i].max_buf_size, cur_max_buf[cfg[i].page_type]);
            return DRV_ERROR_INVALID_VALUE;
        }
        cur_max_buf[cfg[i].page_type] = cfg[i].max_buf_size;

        cfg_num++;
    }

    *cfg_num_out = cfg_num;

    return DRV_ERROR_NONE;
}

static void memzone_set_max_buff(void)
{
    struct memzone_cfg_max_buf_info tmp_info[MEMTYPE_MAX_NUM] = {{0}};
    uint32_t i, memtype;

    for (i = 0; i < g_mz_cfg_num; i++) {
        memtype = memzone_get_memtype(g_mz_cfg[i].page_type);
        if ((g_mz_cfg[i].page_type & BUFF_SP_HUGEPAGE_ONLY) == BUFF_SP_HUGEPAGE_ONLY) {
            if (tmp_info[memtype].huge_only < g_mz_cfg[i].max_buf_size) {
                tmp_info[memtype].huge_only = g_mz_cfg[i].max_buf_size;
            }
        } else {
            if (tmp_info[memtype].normal < g_mz_cfg[i].max_buf_size) {
                tmp_info[memtype].normal = g_mz_cfg[i].max_buf_size;
            }
        }
    }

    for (memtype = 0; memtype < MEMTYPE_MAX_NUM; memtype++) {
        g_mz_max_buf[memtype].huge_prior = (tmp_info[memtype].huge_only > tmp_info[memtype].normal) ?
            tmp_info[memtype].huge_only : tmp_info[memtype].normal;
        g_mz_max_buf[memtype].huge_only = tmp_info[memtype].huge_only;
        g_mz_max_buf[memtype].normal = tmp_info[memtype].normal;
    }
}

static bool is_mz_cfg_dvpp_default(uint32_t cfg_num, memZoneCfg *mz_config)
{
    uint32_t i;

    for (i = 0; i < cfg_num; i++) {
        if ((mz_config[i].page_type & BUFF_SP_DVPP) == BUFF_SP_DVPP) {
            return false;
        }
    }
    return true;
}

static uint32_t memzone_add_default_special_mem(uint32_t cfg_num, memZoneCfg *mz_config)
{
    uint32_t total_cfg_num = cfg_num;
    uint32_t i;

    if (is_mz_cfg_dvpp_default(cfg_num, mz_config) == true) {
        total_cfg_num += (uint32_t)(sizeof(g_mz_cfg_dvpp_default) / sizeof(memZoneCfg));
        for (i = cfg_num; i < total_cfg_num; i++) {
            g_mz_cfg[i] = g_mz_cfg_dvpp_default[i - cfg_num];
            g_mz_cfg[i].cfg_id = i;
        }
    }

    return total_cfg_num;
}

drvError_t memzone_cfg(memZoneCfg *mz_cfg)
{
    memZoneCfg *mz_config = mz_cfg;
    uint32_t cfg_num = 0;
    int ret;

    ret = (int)memzone_cfg_para_check(mz_config, &cfg_num);
    if (ret != DRV_ERROR_NONE) {
        buff_err("hal_buff_cfg para_check failed\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    // use default cfg
    if (cfg_num == 0) {
        if (buff_is_enable_cache()) {
            mz_config = g_mz_cfg_cache_default;
            cfg_num = sizeof(g_mz_cfg_cache_default) / sizeof(memZoneCfg);
        } else {
            mz_config = g_mz_cfg_default;
            cfg_num = sizeof(g_mz_cfg_default) / sizeof(memZoneCfg);
        }
        buff_event("Check no mz cfg, use default. (num=%u)\n", cfg_num);
    }

    /* set new cfg info */
    ret = memcpy_s(g_mz_cfg, sizeof(g_mz_cfg), mz_config, sizeof(memZoneCfg) * cfg_num);
    if (ret != DRV_ERROR_NONE) {
        buff_err("memcpy_s failed, size: %lu, ret:%d\n", sizeof(g_mz_cfg), ret);
        return DRV_ERROR_INNER_ERR;
    }

    /* add g_mz_cfg_special_default */
    g_mz_cfg_num = memzone_add_default_special_mem(cfg_num, mz_config);

    // find max max_buf_size, save to g_mz_max_buf
    memzone_set_max_buff();

    buff_event("Buff_cfg success. (cfg num=%u; huge_prior=%llu; normal=%llu; huge_only=%llu; "
        "dvpp_huge_prior=%llu; dvpp_normal=%llu; dvpp_huge_only=%llu)\n", g_mz_cfg_num,
        g_mz_max_buf[MEMTYPE_NORMAL].huge_prior, g_mz_max_buf[MEMTYPE_NORMAL].normal,
        g_mz_max_buf[MEMTYPE_NORMAL].huge_only, g_mz_max_buf[MEMTYPE_DVPP].huge_prior,
        g_mz_max_buf[MEMTYPE_DVPP].normal, g_mz_max_buf[MEMTYPE_DVPP].huge_only);

    return DRV_ERROR_NONE;
}

static inline void memzone_check_start_recycle(struct memzone_user_mng_t *mz)
{
    if (mz->blk_num_available < mz->recycle_blk_num_level) {
        wake_up_recyle_thread(mz->grp_id);
    }
}

static inline bool memzone_is_idle(struct memzone_user_mng_t *mz)
{
    return (mz->blk_num_available == mz->blk_num_total);
}

STATIC drvError_t memzone_alloc(struct memzone_user_mng_t *mz, uint64_t size, void **buff, uint32_t *blk_id)
{
    struct uni_buff_head_t *head = NULL;
    uint64_t *head_addr = NULL;
    uint64 total;
    uint64 blk_num;
    uint32 idx;
    uintptr_t start;
    uintptr_t end;

    total = buff_calc_size(size, UNI_ALIGN_MIN, 0);
    /*lint -e502 -e647*/
    total = (uint64)ALIGN_UP(total, mz->blk_size);
    /*lint +e502 +e647*/
    blk_num = total / mz->blk_size;

    mz->alloc_flag = 1;
    (void)pthread_mutex_lock(&mz->mutex);
    mz->alloc_flag = 0;
    if (mz->mz_mem_free_size < total) {
        (void)pthread_mutex_unlock(&mz->mutex);
        return DRV_ERROR_INNER_ERR;
    }

    idx = (uint32)bitmap_find_next_zero_area(mz->bitmap, mz->bitnum, 0, (uint32)blk_num, 0);
    if (idx >= mz->bitnum) {
        (void)pthread_mutex_unlock(&mz->mutex);
        return DRV_ERROR_NO_RESOURCES;
    }

    if (memzone_is_idle(mz) == true) {
        buff_api_atomic_inc(&mz->mz_list_node->mz_using_cnt);
    }
    mz->mz_mem_free_size -= total;
    mz->blk_num_available -= (uint32)blk_num;

    start = (uintptr_t)(mz->area.mz_mem_uva) + ((uintptr_t)idx * mz->blk_size);
    end   = start + blk_num * mz->blk_size;

    head = buff_head_init(start, end, UNI_ALIGN_MIN, 1, 0);
    head->buff_type = BUFF_NORMAL;
    head->align_flag = 0;
    head->ref = 1;
    head->timestamp = (unsigned int)buff_api_timestamp();

    buff_ext_info_init(head, mz->pid, buff_get_process_uni_id());
    *buff = (void *)(head + 1);
    *blk_id = mz->area.blk_id;

    head_addr = memzone_get_buff_head_by_index(mz, idx);
    *head_addr = (uint64_t)(uintptr_t)head;

#if defined(__arm__) || defined(__aarch64__)
    wmb();
#endif
    /* in kernel, guard work mz_scan_and_free_buff will get index by bitmap, then use this index
        get head_uva, so we must store head_addr before bitmap_set  */
    bitmap_set(mz->bitmap, (int)idx, (int)blk_num);
    (void)pthread_mutex_unlock(&mz->mutex);

    memzone_check_start_recycle(mz);

    return DRV_ERROR_NONE;
}

static drvError_t memzone_free(void *memzone, void *buff, struct uni_buff_head_t *head)
{
    struct memzone_user_mng_t *mz = (struct memzone_user_mng_t *)memzone;
    uint64_t *head_addr = NULL;
    void *start = NULL;
    void *end = NULL;
    uint64 blk_num;
    uint32 idx;

    start = (void *)((char *)head - (head->resv_head * UNI_RSV_HEAD_LEN));
    end   = (void *)((char *)head + head->size);
    if (!memzone_buff_is_valid(&mz->area, start, end)) {
        buff_err("invalid addr:%lx to free.\n", (uintptr_t)buff);
        return DRV_ERROR_BAD_ADDRESS;
    }

    blk_num = ((uintptr_t)end - (uintptr_t)start) / mz->blk_size;
    idx = (uint32)(((uintptr_t)start - (uintptr_t)(mz->area.mz_mem_uva)) / mz->blk_size);

    head->mbuf_data_flag = UNI_MBUF_DATA_DISABLE;
    head->status = UNI_STATUS_IDLE;

    mz->mz_mem_free_size += blk_num * mz->blk_size;
    mz->blk_num_available += (uint32)blk_num;
    bitmap_clear(mz->bitmap, (int)idx, (int)blk_num);

#if defined(__arm__) || defined(__aarch64__)
    wmb();
#endif

    head_addr = memzone_get_buff_head_by_index(mz, idx);
    *head_addr = 0;

    if (memzone_is_idle(mz) == true) {
        buff_api_atomic_dec(&mz->mz_list_node->mz_using_cnt);
    }

    return DRV_ERROR_NONE;
}

static inline void memzone_atomic_inc(struct memzone_user_mng_t *mz)
{
    buff_api_atomic_inc(&mz->head.ref);
}

static inline void memzone_atomic_dec(struct memzone_user_mng_t *mz)
{
    buff_api_atomic_dec(&mz->head.ref);
}

static inline uint32_t memzone_atomic_read(struct memzone_user_mng_t *mz)
{
    return buff_api_atomic_read(&mz->head.ref);
}

static struct memzone_user_mng_t *memzone_mng_get(struct buff_memzone_list_node *mz_list_node,
    struct memzone_user_mng_t *mz, unsigned int cnt)
{
    struct list_head *head = &mz_list_node->list;
    struct list_head *tmp = NULL;
    struct memzone_user_mng_t *mz_out = NULL;

    (void)pthread_rwlock_rdlock(&mz_list_node->mz_list_mutex);
    if (mz == NULL) {
        tmp = (mz_list_node->latest_mz != NULL) ? &mz_list_node->latest_mz->user_list : head->next;
    } else {
        tmp = mz->user_list.next;
    }
    if ((tmp == head) || (tmp == NULL)) {
        if (cnt >= mz_list_node->mz_cnt) {
            (void)pthread_rwlock_unlock(&mz_list_node->mz_list_mutex);
            return NULL;
        }
        tmp = head->next;
    }
    mz_out = list_entry(tmp, struct memzone_user_mng_t, user_list);

    memzone_atomic_inc(mz_out);
    (void)pthread_rwlock_unlock(&mz_list_node->mz_list_mutex);

    return mz_out;
}

static void memzone_mng_put(struct memzone_user_mng_t *mz)
{
    (void)pthread_rwlock_rdlock(&mz->mz_list_node->mz_list_mutex);
    memzone_atomic_dec(mz);
    (void)pthread_rwlock_unlock(&mz->mz_list_node->mz_list_mutex);
}

void memzone_mng_list_add(struct memzone_user_mng_t *mz)
{
    (void)pthread_rwlock_wrlock(&mz->mz_list_node->mz_list_mutex);
    drv_user_list_add_head(&mz->user_list, &mz->mz_list_node->list);
    buff_api_atomic_inc(&mz->mz_list_node->mz_cnt);
    memzone_atomic_inc(mz);
    (void)pthread_rwlock_unlock(&mz->mz_list_node->mz_list_mutex);
}

bool memzone_need_shrink(struct buff_memzone_list_node *mz_list_node)
{
    uint32_t mz_free_cnt;

    if (mz_list_node->elastic_enable == 0) {
        return false;
    }

    /* for performance: no need lock */
    mz_free_cnt = buff_api_atomic_read(&mz_list_node->mz_cnt) -
        buff_api_atomic_read(&mz_list_node->mz_using_cnt);
    if (mz_free_cnt <= mz_list_node->elastic_low_level) {
        return false;
    }
    return true;
}

static inline bool memzone_is_parent_inited(struct memzone_user_mng_t *mz)
{
    return mz->head.parent != NULL;
}

static bool memzone_is_match_destroy_condition(struct memzone_user_mng_t *mz,
    struct memzone_user_mng_t *specific_mz)
{
    /* Indicates whether to match the release of the specified pool */
    if ((specific_mz != NULL) && (specific_mz != mz)) {
        return false;
    }
    if (memzone_atomic_read(mz) != 1) {
        return false;
    }
    /* The pool that has just been created and added to the linked list may be released.
     * The pool information needs to be initialized. Otherwise, the pool fails to be
     * released and will be leaked.
     */
    if (memzone_is_parent_inited(mz) == false) {
        return false;
    }

    return true;
}

void memzone_scan_free_idle_pool(struct buff_memzone_list_node *mz_list_node,
    struct memzone_user_mng_t *specific_mz)
{
    struct list_head *head = &mz_list_node->list;
    struct memzone_user_mng_t *mz = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;

    (void)pthread_rwlock_wrlock(&mz_list_node->mz_list_mutex);
    list_for_each_safe(pos, n, head) {
        mz = list_entry(pos, struct memzone_user_mng_t, user_list);
        if (memzone_is_match_destroy_condition(mz, specific_mz)) {
            if (mz_list_node->latest_mz == mz) {
                (void)ATOMIC_SET(&mz_list_node->latest_mz, NULL);
            }
            drv_user_list_del(&mz->user_list);
            buff_api_atomic_dec(&mz->mz_list_node->mz_cnt);
            memzone_atomic_dec(mz);
            break;
        }
        mz = NULL;
    }
    (void)pthread_rwlock_unlock(&mz_list_node->mz_list_mutex);

    if (mz != NULL) {
        uint32_t blk_id = mz->area.blk_id;
        memzone_delete(mz);
        idle_buff_range_free_ahead(blk_id);
    }
}

drvError_t memzone_free_normal(void *mz_mng, void *buff, struct uni_buff_head_t *head)
{
    struct memzone_user_mng_t *mz = (struct memzone_user_mng_t *)mz_mng;
    struct buff_memzone_list_node *mz_list_node = mz->mz_list_node;
    drvError_t ret;

    mz->free_flag = 1;
    (void)pthread_mutex_lock(&mz->mutex);
    mz->free_flag = 0;
    ret = memzone_free(mz, buff, head);
    (void)pthread_mutex_unlock(&mz->mutex);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    memzone_mng_put(mz);
    if (memzone_need_shrink(mz_list_node)) {
        memzone_scan_free_idle_pool(mz_list_node, NULL);
    }
    return DRV_ERROR_NONE;
}

/* return free num */
int memzone_scan_free(struct memzone_user_mng_t *mz)
{
    void *buff = NULL, *start = NULL, *end = NULL;
    struct uni_buff_head_t *head = NULL;
    unsigned long i, size;
    int num = 0;

    (void)pthread_mutex_lock(&mz->mutex);
    for (i = 0; i < mz->bitnum;) {
        if ((mz->alloc_flag == 1) || (mz->free_flag == 1)) {
            break;
        }

        i = bitmap_find_next_bit(mz->bitmap, mz->bitnum, i);
        if (i >= mz->blk_num_total) {
            break;
        }

        head = (struct uni_buff_head_t *)(uintptr_t)*memzone_get_buff_head_by_index(mz, (uint32)i);
        if (head == NULL) {
            buff_warn("Mz buff has been free. (mz=%p, index=%lu)\n", mz, i);
            continue;
        }

        buff = (void *)(head + 1);
        if (head->status == UNI_STATUS_RELEASE) {
            (void)memzone_free(mz, buff, head);
            memzone_mng_put(mz);
            num++;
        }

        start = (void *)((char *)head - (head->resv_head * UNI_RSV_HEAD_LEN));
        end   = (void *)((char *)head + head->size);
        size = (unsigned long)end - (unsigned long)start;
        if ((size < mz->blk_size) || (size > mz->area.mz_mem_total_size)) {
            break;
        }
        i += size / mz->blk_size;
    }
    (void)pthread_mutex_unlock(&mz->mutex);

    return num;
}

/*lint -e629 */
static drvError_t memzone_list_alloc(struct buff_memzone_list_node *mz_list_node,
    uint64_t size, void **buff, uint32_t *blk_id)
{
    struct memzone_user_mng_t *mz = NULL;
    struct memzone_user_mng_t *next_mz = NULL;
    drvError_t ret = DRV_ERROR_OUT_OF_MEMORY;
    unsigned int cnt = 0;

    while (ret != DRV_ERROR_NONE) {
        next_mz = memzone_mng_get(mz_list_node, mz, cnt);
        if (mz != NULL) {
            memzone_mng_put(mz);
        }
        if (next_mz == NULL) {
            return ret;
        }
        cnt++;
        ret = memzone_alloc(next_mz, size, buff, blk_id);
        if (ret != DRV_ERROR_NONE) {
            mz = next_mz;
            continue;
        }
    }
    /* do not need lock here, if the value is incorrect, only the performance maybe affected. */
    (void)ATOMIC_SET(&mz_list_node->latest_mz, next_mz);
    return DRV_ERROR_NONE;
}

static drvError_t memzone_alloc_normal(uint64_t size, void **buff, uint32_t *blk_id, struct mem_info_t *mem_info)
{
    int devid = buff_get_devid_from_flags(mem_info->flag);
    struct buff_memzone_list_node* mz_list_node = NULL;
    struct memzone_user_mng_t *mz = NULL;
    memZoneCfg *cfg_info = NULL;
    drvError_t ret, ret_tmp;

    cfg_info = memzone_get_cmpat_cfg_info(size, (uint32_t)(mem_info->flag));
    if (cfg_info == NULL) {
        buff_err("memzone_alloc_normal failed, can't find compatible cfg info\n");
        return DRV_ERROR_INNER_ERR;
    }

    mz_list_node = buff_get_buff_mng_list(devid, mem_info->grp_id, cfg_info->cfg_id);
    if (mz_list_node == NULL) {
        buff_err("get mz list failed\n");
        return DRV_ERROR_INNER_ERR;
    }

    do {
        ret = memzone_list_alloc(mz_list_node, size, buff, blk_id);
        if (ret != DRV_ERROR_NONE) {
            ret_tmp = memzone_create(cfg_info, mem_info->grp_id, mz_list_node, &mz);
            if (ret_tmp != DRV_ERROR_NONE) {
                buff_event("Can not create memzone. (ret=%x)\n", ret_tmp);
                return ret_tmp;
            }
            buff_debug("Memzone create success. (pid=%d, size=%llu, flag=%#llx, grp_id=%d, "
                "cfg_id=%u, blk_size=%u, max_size=%llu)\n",
                buff_api_getpid(), size, mem_info->flag, mem_info->grp_id,
                cfg_info->cfg_id, cfg_info->blk_size, cfg_info->max_buf_size);
        }
    } while (ret != DRV_ERROR_NONE);

    return ret;
}

static drvError_t memzone_alloc_large(uint64_t size, void **buff, uint32_t *blk_id, struct mem_info_t *info)
{
    struct buff_req_mz_alloc_huge_buf arg;
    drvError_t ret;

    arg.buf_uva = 0;
    arg.grp_id = info->grp_id;
    arg.alloc_type = info->alloc_type;
    arg.sp_flag = info->flag;
    arg.size = size;
    arg.devid = buff_get_devid_from_flags(info->flag);

    ret = buff_usr_alloc_large_buf(&arg);
    if (ret != DRV_ERROR_NONE) {
        buff_event("Can not alloc mz huge. (ret=%d; devid=%d; grp_id=%d; pid=%d; size=%llu; page_type=%#llx)\n",
            ret, arg.devid, arg.grp_id, buff_get_current_pid(), arg.size, arg.sp_flag);
        return ret;
    }
    *blk_id = arg.blk_id;
    *buff = (void *)(uintptr_t)arg.buf_uva;

    return DRV_ERROR_NONE;
}

drvError_t memzone_free_huge(void *huge_mng, void *buff)
{
    struct buff_req_mz_free_huge_buf arg;
    drvError_t ret;

    arg.buf_uva = (uintptr_t)buff;
    ret = buff_usr_free_large_buf(huge_mng, &arg);
    if (ret != DRV_ERROR_NONE) {
        buff_err("mz free huge buff failed, ret:%d, buff:0x%lx\n", ret, (uintptr_t)buff);
        return DRV_ERROR_INNER_ERR;
    }

    return DRV_ERROR_NONE;
}

void huge_buff_scan_free(void *huge_mng, void *buff)
{
    (void)memzone_free_huge(huge_mng, buff);
}

static drvError_t buff_align_adapt(void **buff, struct mem_info_t *info)
{
    struct uni_buff_head_t *uni_head = NULL;
    struct uni_buff_head_t *align_head = NULL;
    uint32 head_len = sizeof(struct uni_buff_head_t);
    uint64 org_addr = (uintptr_t)(*buff);
    uint64 adapt_addr;
    uint64 offset = 0;

    info->offset = (uint32)offset;
    if (info->align <= UNI_ALIGN_MIN) {
        return DRV_ERROR_NONE;
    }

    uni_head = (struct uni_buff_head_t *)((char *)*buff - head_len);

    /*lint -e128 -e51*/
    adapt_addr = ALIGN_UP(org_addr, info->align);
    /*lint +e128 +e51*/
    offset = adapt_addr - org_addr;
    if (offset == 0) {
        return DRV_ERROR_NONE;
    } else if (offset > (info->align - UNI_ALIGN_MIN) || offset < head_len) {
        buff_err("adapt buff failed, buff:0x%llx, adapt_buff:0x%llx, offset:0x%llx, align:0x%x\n",
                 org_addr, adapt_addr, offset, info->align);
        return DRV_ERROR_INNER_ERR;
    }

    /* adapt buff */
    *buff = (void *)(uintptr_t)adapt_addr;

    /* init align head */
    align_head = (struct uni_buff_head_t *)((char *)*buff - head_len);
    align_head->image = uni_head->image;
    align_head->align_flag = UNI_ALIGN_FLAG_ENABLE;

    /* store offset between uni_head and align_head */
    align_head->offset = (uint32)offset;
    info->offset = (uint32)offset;

    return DRV_ERROR_NONE;
}

static drvError_t memzone_alloc_para_check(struct mem_info_t *info, void **buff)
{
    uint64_t total;

    if (buff == NULL) {
        buff_err("para is NULL, info:0x%lx, buff:0x%lx\n", (uintptr_t)info, (uintptr_t)buff);
        return DRV_ERROR_INVALID_VALUE;
    }

    /* resv 2M space for buf_mng */
    if (info->size == 0) {
        buff_err("size is invalid, size:0x%llx, size_len:%d, max_size:0x%x\n",
            info->size, sizeof(info->size), BUFF_32_BIT_MASK - MZ_PAGE_SIZE_HUGE);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (buff_get_devid_from_flags(info->flag) >= BUFF_MAX_DEV) {
        buff_err("invalid flag, flag:%#llx\n", info->flag);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (!buff_check_align(info->align)) {
        buff_err("align is invalid:%u\n", info->align);
        return DRV_ERROR_INVALID_VALUE;
    }

    total = buff_calc_size(info->size, info->align, 0);
    if (buff_size_is_invalid(info->size, total) != 0) {
        buff_err("buff_alloc_align size invalid(0~4G), size:0x%llx, total size:0x%llx, align:%u\n", info->size, total,
            info->align);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}
/*lint +e629 */

static drvError_t _memzone_alloc_buff(struct mem_info_t *info, void **buff, uint32_t *blk_id, uint64 sp_flag)
{
    drvError_t ret;

    if (info->alloc_type == MZ_ALLOC_TYPE_DIRECTLY) {
        ret = memzone_alloc_large(info->size, buff, blk_id, info);
    } else if (info->size <= memzone_get_max_buf_size((uint32)sp_flag)) {
        if ((sp_flag & BUFF_SP_HUGEPAGE_PRIOR) == BUFF_SP_HUGEPAGE_PRIOR) {
            info->flag = (sp_flag & (uint64)(~BUFF_SP_HUGEPAGE_PRIOR)) | BUFF_SP_HUGEPAGE_ONLY;
        }

        ret = memzone_alloc_normal(info->size, buff, blk_id, info);
        if ((ret != DRV_ERROR_NONE) && ((sp_flag & BUFF_SP_HUGEPAGE_PRIOR) == BUFF_SP_HUGEPAGE_PRIOR)) {
            info->flag = sp_flag & (uint64)(~(BUFF_SP_HUGEPAGE_PRIOR | BUFF_SP_HUGEPAGE_ONLY));
            ret = memzone_alloc_normal(info->size, buff, blk_id, info);
            buff_event("Can not alloc huge prior, alloc normal page. (ret=0x%x; flag=%#llx; new_flag=%#llx)\n",
                ret, sp_flag, info->flag);
        }
    } else {
        ret = memzone_alloc_large(info->size, buff, blk_id, info);
    }
    return ret;
}

drvError_t memzone_alloc_buff(struct mem_info_t *info, void **buff, uint32_t *blk_id)
{
    uint64 sp_flag = info->flag;
    drvError_t ret;

    ret = memzone_alloc_para_check(info, buff);
    if (ret != DRV_ERROR_NONE) {
        buff_err("para check failed\n");
        return ret;
    }

    if (info->align > UNI_ALIGN_MIN) {
        info->size += info->align - UNI_ALIGN_MIN;
    }

    ret = _memzone_alloc_buff(info, buff, blk_id, sp_flag);
    if (ret != DRV_ERROR_NONE) {
        if (buff_is_enable_cache()) {
            /* wait 200ms to free memory in recycle thread */
            (void)usleep(RECYCLE_THREAD_PERIOD_US);
            buff_event("wait 200ms to free mem. (size=0x%llx; align=0x%x; flag=%#llx; grp_id=%d; pid=%d; ret=0x%x)\n",
                info->size, info->align, info->flag, info->grp_id, buff_get_current_pid(), ret);
            (void)buff_proc_cache_free(BUFF_INVALID_DEV);
            ret = _memzone_alloc_buff(info, buff, blk_id, sp_flag);
        }
        if (ret != DRV_ERROR_NONE) {
            buff_err("Memzone alloc buff failed. (size=0x%llx; align=0x%x; flag=%#llx; grp_id=%d; pid=%d; ret=0x%x)\n",
                info->size, info->align, info->flag, info->grp_id, buff_get_current_pid(), ret);
            return ret;
        }
    }

    return buff_align_adapt(buff, info);
}

static void memzone_init_alloc_info(uint64_t size, unsigned int align,
    unsigned long flag, int grp_id, struct mem_info_t *info)
{
    info->size = size;
    info->align = align;
    info->offset = 0;
    info->flag = flag & (~XSMEM_BLK_ALLOC_FROM_OS);
 
    if ((flag & BUFF_SP_SVM) == BUFF_SP_SVM) {
        info->grp_id = grp_id;
        info->alloc_type = MZ_ALLOC_TYPE_DIRECTLY;
        info->flag = (uint64)(info->flag & (uint64)(~BUFF_SP_SVM));
    } else {
        if (grp_id != buff_get_default_pool_id()) {
            buff_warn("grp_id %d not alloc id %d \n", grp_id, buff_get_default_pool_id());
        }
        info->grp_id = buff_get_default_pool_id();
        info->alloc_type = MZ_ALLOC_TYPE_NORMAL;
    }
}

int halBuffAllocAlignEx(uint64_t size, unsigned int align, unsigned long flag, int grp_id, void **buff)
{
    struct mem_info_t info;
    drvError_t ret;
    uint32_t blk_id;

    memzone_init_alloc_info(size, align, flag, grp_id, &info);
    ret = memzone_alloc_buff(&info, buff, &blk_id);
    if (ret != DRV_ERROR_NONE) {
        buff_err("Hal_buff_alloc_align_ex failed. (size=0x%llx; size_len=%d; align=0x%x; flag=0x%lx; grpid=%d; ret=%d)\n",
            size, sizeof(size), align, flag, info.grp_id, ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

int halBuffAllocEx(uint64_t size, unsigned long flag, int grp_id, void **buff)
{
    return halBuffAllocAlignEx(size, UNI_ALIGN_MIN, flag, grp_id, buff);
}

int halBuffAlloc(uint64_t size, void **buff)
{
    unsigned long flag = 0;

    flag = buff_make_devid_to_flags(buff_get_current_devid(), flag);
#ifdef CFG_FEATURE_SURPORT_HUGE_PAGE
    flag |= BUFF_SP_HUGEPAGE_PRIOR;
    return halBuffAllocAlignEx(size, UNI_ALIGN_MIN, flag, buff_get_default_pool_id(), buff);
#else
    flag |= BUFF_SP_NORMAL;
    return halBuffAllocAlignEx(size, UNI_ALIGN_MIN, flag, buff_get_default_pool_id(), buff);
#endif
}

