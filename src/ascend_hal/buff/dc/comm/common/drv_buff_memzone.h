/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DRV_BUFF_MEMZONE_H
#define DRV_BUFF_MEMZONE_H

#include "drv_buff_common.h"
#include "drv_buff_unibuff.h"
#include "drv_buff_list.h"
#include "buff_recycle.h"

#define MZ_INIT_FLAG    0xAABBCCDD

#define MZ_ALLOC_FROM_SHARE_POOL 0
#define MZ_ALLOC_FROM_NORMAL_MEM 1

#define MZ_STATUS_NORMAL 0
#define MZ_STATUS_DESTROYED 1

#define MZ_MEM_NOT_MAKE_SHARED 0
#define MZ_MEM_MAKE_SHARED 1

#define MZ_PAGE_NORMAL BUFF_SP_NORMAL
#define MZ_PAGE_HUGE BUFF_SP_HUGEPAGE_PRIOR
#define MZ_PAGE_HUGE_ONLY BUFF_SP_HUGEPAGE_ONLY
#define MZ_PAGE_SIZE_NORMAL (4 * 1024)
#define MZ_PAGE_SIZE_HUGE (2 * 1024 * 1024)

#define MZ_DEFAULT_CFG_MAX_SIZE 69206016ul // 66MB

#define MZ_CFG_INVALID 0
#define MZ_CFG_VALID 1

#define MZ_BLK_SIZE_MAX (2 * 1024 * 1024)
#define MZ_TOTAL_SIZE_MAX (256 * 1024 * 1024)

#define MZ_USER_MNG_MAX_RESV_SIZE 1024

#define MZ_RESV_SIZE_BASE 64

#define MZ_ALLOC_TYPE_NORMAL 0
#define MZ_ALLOC_TYPE_DIRECTLY 1

#define MZ_USER_SELF 0
#define MZ_USER_OTHER 1

struct memzone_alloc_share_info {
    uint32 mem_type;
    uint64 size;
    pid_t pid;
    int grp_id;
    uint32 sp_flag;
};

struct memzone_cfg_max_buf_info {
    uint64_t normal;
    uint64_t huge_only;
    uint64_t huge_prior;
};

/* have unibuf head and tail */
struct memzone_buf_release_cache {
    struct common_handle_t common_head;
    void *mz_user_mng_uva;
    uint64 head;
    uint64 tail;
    uint32 make_share_flag;
    int grp_id;
    int sp_id;
    uint64 fast_node_size;
    uint64 total_size;
    uint64 slot_num;
    void *cache_start_uva;
    uint64 slot[];
};

struct memzone_area {
    void *mz_mem_uva;
    uint64 mz_mem_total_size;
    uint32_t blk_id;
};

struct memzone_user_mng_t {
    struct common_handle_t head;
    pthread_mutex_t mutex;
    uint64 start_addr;
    uint32 resv_size;
    pid_t pid;
    int grp_id;
    uint32 sp_flag;
    uint32 cfg_id;

    uint32 make_share_flag;
    struct list_head user_list;

    struct buff_memzone_list_node *mz_list_node;

    struct memzone_area area;
    struct memzone_buf_release_cache *release_cache_uva;
    uint64 mz_mem_free_size;

    int alloc_flag;
    int free_flag;
    uint32 recycle_blk_num_level;
    uint32 blk_size;
    uint32 blk_num_total;
    uint32 blk_num_available;
    uint32 cur_index;

    uint32 bitnum;
    bitmap_t bitmap[];
};

struct memzone_huge_user_mng_t {
    struct common_handle_t head;
    struct memzone_area area;
    uint64 uni_head;
};

struct memzone_kernel_mng_t {
    int devid;
    pid_t pid;
    int grp_id;
    int sp_id;
    uint32 sp_flag;
    uint32 cfg_id;

    struct list_head kernel_list;
    struct buff_pid_list_node *list_node;
    uint32 memzone_status;

    struct memzone_buf_release_cache *cache_uva;
    struct memzone_buf_release_cache *cache_kva_mapped;
    void *cache_start_uva;
    void *cache_start_kva_mapped;
    uint64 cache_slot_num;

    struct memzone_user_mng_t *mz_user_uva_mapped;
    struct memzone_user_mng_t *mz_user_kva;
    void *mz_user_start_uva_mapped;
    void *mz_user_start_kva;
    uint64 mz_user_mng_size;

    void *mz_mem_uva;
    void *mz_mem_kva_mapped;
    uint64 mz_mem_total_size;

    void *mng_fast_node;
    void *mem_u2k_fast_node;
    void *mem_k2u_fast_node;
    void *cache_fast_node;

    uint32 blk_size;
    uint32 blk_num_total;

    uint32 bitnum;
};

void memzone_delete(struct memzone_user_mng_t *mz);
drvError_t memzone_alloc_buff(struct mem_info_t *info, void **buff, uint32_t *blk_id);
drvError_t memzone_free_huge(void *huge_mng, void *buff);
drvError_t memzone_free_normal(void *mz_mng, void *buff, struct uni_buff_head_t *head);
uint64_t* memzone_get_buff_head_by_index(struct memzone_user_mng_t *memzone, unsigned int idx);
int memzone_scan_free(struct memzone_user_mng_t *mz);
void huge_buff_scan_free(void *huge_mng, void *buff);
memZoneCfg *memzone_get_cfg_info_by_id(unsigned int cfg_id);
void memzone_elastic_cfg_init(struct buff_memzone_list_node* mz_list_node);
void memzone_mng_list_add(struct memzone_user_mng_t *mz);
void memzone_scan_free_idle_pool(struct buff_memzone_list_node *mz_list_node,
    struct memzone_user_mng_t *specific_mz);

static inline bool memzone_buff_is_valid(struct memzone_area *mz_area, const void *start, const void *end)
{
    void *mz_end = (void *)((uintptr_t)(mz_area->mz_mem_uva) + (uintptr_t)(mz_area->mz_mem_total_size));

    if (((uintptr_t)start < (uintptr_t)mz_area->mz_mem_uva || (uintptr_t)start >= (uintptr_t)mz_end) ||
        ((uintptr_t)end < (uintptr_t)mz_area->mz_mem_uva || (uintptr_t)end > (uintptr_t)mz_end) ||
        ((uintptr_t)start >= (uintptr_t)end)) {
        buff_err("invalid buff start or end, buff start:%lx, memzone start:%lx, end:%lx\n",
            (uintptr_t)start, (uintptr_t)mz_area->mz_mem_uva, (uintptr_t)mz_end);
        return 0;
    }

    return 1;
}

#endif /* _DRV_BUFF_MEMZONE_H_ */
