/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <sys/types.h>
#include <unistd.h>

#include "drv_buff_common_mempool.h"
#include "drv_usr_buff_mempool.h"
#include "drv_buff_common.h"
#include "drv_buff_adp.h"
#include "buff_mng.h"
#include "buff_recycle.h"
#include "buff_range.h"
#include "buff_mempool_adapt.h"

static uint64 mp_get_pool_mng_size(uint32 obj_num)
{
    size_t bm_size = bitmap_size(obj_num);
    uint64 pool_sz = (uint64)ALIGN_UP(sizeof(struct mempool_t), sizeof(bitmap_t));
    uint64 pool_tmp;
    uint32 size = (uint32)ALIGN_UP(bm_size, sizeof(uint64));

    pool_sz += size;
    pool_tmp = pool_sz + ((uint64)sizeof(uint64) * obj_num);
    if (pool_tmp <= pool_sz) {
        buff_err("pool_sz out of limit, (pool_sz=%llu, pool_tmp=%llu, obj_num=%u)\n", pool_sz, pool_tmp, obj_num);
        return 0;
    }

    return pool_tmp;
}

static void mp_trace_init(struct mp_trace *trace)
{
    trace->print_flag = 0;
    trace->last_alloc_fail_timestamp = 0;
}

static void mp_init_name(struct mempool_t *mp, char *name)
{
    int len;

    len = (int)strnlen(name, BUFF_POOL_NAME_LEN);
    if ((len >= BUFF_POOL_NAME_LEN) || (len == 0)) {
#ifndef USER_BUFF_MANAGE_UT
        (void)memset_s(mp->pool_name, BUFF_POOL_NAME_LEN, 0, BUFF_POOL_NAME_LEN);
#endif
    } else {
        if (strncpy_s(mp->pool_name, (uint32)BUFF_POOL_NAME_LEN, name, (uint32)len) != 0) {
#ifndef USER_BUFF_MANAGE_UT
            (void)memset_s(mp->pool_name, BUFF_POOL_NAME_LEN, 0, BUFF_POOL_NAME_LEN);
#endif
        }
    }
}

static drvError_t buff_usr_mp_init(struct mempool_t *mp, struct buff_req_mp_create *info)
{
    int ret;

    ret = pthread_mutex_init(&mp->mutex, NULL);
    if (ret != 0) {
        buff_err("pthread_mutex_init failed\n");
        return DRV_ERROR_INNER_ERR;
    }

    mp->head.status = MP_S_NORMAL;
    mp->head.type   = UNI_TYPE_MP;
    mp->head.devid = (uint32_t)info->devid;
    mp->head.ref = 0;
    mp->head.parent = NULL;
    mp->type = info->type;

    mp->blk_available = info->obj_num;
    mp->blk_num = info->obj_num;
    mp->free_index = 0;
    mp->curr_index = 0;
    mp->bit_num = ALIGN_UP(mp->blk_num, BITS_PER_LONG); //lint !e502
    mp->blk_size = info->obj_size;
    mp->scan_in_alloc = 0;
    mp->owner = buff_get_current_pid();

    bitmap_clear(mp->bitmap, 0, (int)mp->blk_available);
    bitmap_set(mp->bitmap, (int)(mp->blk_available), (int)(mp->bit_num - mp->blk_available));

    mp_init_name(mp, info->pool_name);
    mp_trace_init(&mp->trace);
    return DRV_ERROR_NONE;
}

static void buff_mp_mbuf_init(uint64 blk_uva)
{
    struct share_mbuf *mbuf = NULL;
    mbuf = (struct share_mbuf *)(uintptr_t)(blk_uva);
    mbuf->record_cur_idx = 0;
    mbuf->record_total_num = 0;
}

static void buff_mp_buff_init(struct mempool_t *mp, uint64 blk_uva)
{
    struct uni_buff_trace_t *trace = NULL;
    int ret;

    trace = buff_mempool_get_trace((void *)(uintptr_t)(blk_uva));
    ret = memset_s(trace, sizeof(struct uni_buff_trace_t), 0, sizeof(struct uni_buff_trace_t));
    if (ret != 0) {
        buff_warn("Memset_s not success. (ret=%d)\n", ret);
    }

    trace->blk_size = mp->blk_size;
    trace->total_blk_num = mp->blk_num;
    ret = strcpy_s(trace->pool_name, BUFF_POOL_NAME_LEN, mp->pool_name);
    if (ret != 0) {
        buff_warn("Strcpy_s not success. (ret=%d)\n", ret);
    }
}

static void buff_usr_mp_block_init(struct mempool_t *mp, struct buff_req_mp_create *info, char *addr,
    uint64_t blk_align_size, uint32 type)
{
    uint64 *mp_uva_list = (uint64 *)mp_get_valist_start_addr(mp, mp->bit_num);
    struct uni_buff_head_t *head = NULL;
    uintptr_t start;
    uint32 i;
    uint32 trace_flag = (type == MEMPOOL_MBUF_LIST) ?  0 : 1;

    for (i = 0; i < info->obj_num; i++) {
        start = (uintptr_t)(addr + blk_align_size * i); //lint !e507
        head = buff_head_init(start, start + blk_align_size, info->align, 1, trace_flag);
        head->index = i;
        mp_uva_list[i] = (uint64)(uintptr_t)(head + 1);
        if (type == MEMPOOL_MBUF_LIST) {
            buff_mp_mbuf_init(mp_uva_list[i]);
        } else {
            buff_mp_buff_init(mp, mp_uva_list[i]);
        }
    }
}

drvError_t buff_usr_mp_create(struct buff_req_mp_create *info)
{
    unsigned long flags = buff_make_devid_to_flags(info->devid, info->sp_flag);
    uint64_t block_align_size;
    uint64_t block_org_size;
    uint64_t total_size;
    struct mempool_t *mp = NULL;
    struct block_mem_ctx *block = NULL;
    char *start = NULL;
    uint64 pool_mng_size;
    uint32 trace_flag;
    drvError_t ret;
    uint32_t blk_id;

    pool_mng_size = mp_get_pool_mng_size(info->obj_num);
    if (pool_mng_size == 0) {
        return DRV_ERROR_INVALID_VALUE;
    }

    mp = malloc(pool_mng_size);
    if (mp == NULL) {
        buff_err("buff alloc mp mng failed\n");
        return DRV_ERROR_OUT_OF_MEMORY;
    }
    ret = buff_usr_mp_init(mp, info);
    if (ret != 0) {
        free(mp);
        buff_err("buff mp init failed\n");
        return ret;
    }

    block = malloc(sizeof(struct block_mem_ctx));
    if (block == NULL) {
        free(mp);
        buff_err("Buff mem ctx alloc failed\n");
        return DRV_ERROR_OUT_OF_MEMORY;
    }
    ret = proc_block_ctx_init(info->groupid, (int)(info->type), (void *)mp, block);
    if (ret != DRV_ERROR_NONE) {
        buff_err("Buff mem ctx alloc failed\n");
        goto free_mp;
    }

    trace_flag = (info->type == MEMPOOL_LIST) ? 1 : 0;
    block_org_size = buff_calc_size(info->obj_size, info->align, trace_flag);
    block_align_size = ALIGN_UP(block_org_size, info->align); //lint !e502 !e647 !e666
    total_size = info->obj_num * block_align_size;

    start = (char *)buff_blk_alloc(info->groupid, total_size, flags, &blk_id);
    if (start == NULL) {
        buff_err("buff alloc failed, (len=%llx, flag=%#lx)\n", total_size, flags);
        ret = DRV_ERROR_OUT_OF_MEMORY;
        goto free_mp;
    }

    ret = add_self_buff_to_range(blk_id, info->groupid, start, total_size, (uint64)(uintptr_t)mp);
    if (ret != 0) {
        buff_err("buff add to range failed, start %p len %llx blk_id=%u ret=%d\n", start, total_size, blk_id, ret);
        goto free_sp_res;
    }

    buff_usr_mp_block_init(mp, info, start, block_align_size, info->type);

    mp->blk_total_len = info->obj_num * (uint64)block_align_size;
    mp->blk_start = start;
    mp->align_size = (uint32)(block_align_size - block_org_size);
    mp->blk_id = blk_id;

    info->total_size = mp->blk_total_len;
    info->start = start;
    info->mp_uva = (uint64)(uintptr_t)mp;
    /* mp_mng_list_add must before proc_block_add. Otherwise mp may be deleted in recycle thread. */
    mp_mng_list_add(mp);
    mp->head.parent = block;
    proc_block_add(block);

    buff_debug("Mp create. (start=0x%llx; type=%d; total_size=%llu; block_align_size=%llu; flag=0x%lx; devid=%d; "
        "blk_id=%u)\n", (uint64)(uintptr_t)start, info->type, total_size, block_align_size, flags, info->devid, blk_id);

    return DRV_ERROR_NONE;
free_sp_res:
    buff_blk_free(info->groupid, start);
free_mp:
    free(mp);
    free(block);
    return ret;
}

drvError_t buff_usr_mp_delete(struct buff_req_mp_destroy *info)
{
    struct mempool_t *mp = (struct mempool_t *)(uintptr_t)info->mp;
    void *start = mp->blk_start;
    void *blk = mp->head.parent;
    uint32_t blk_id = mp->blk_id;

    buff_debug("Mp delete .(addr=%p)\n", (uint64)(uintptr_t)start);

    if (blk == NULL) {
        buff_err("Parent is null. (mp=%p, blk_num=%u)\n", mp, mp->blk_num);
        return DRV_ERROR_INVALID_VALUE;
    }
    proc_set_buff_invalid(blk);
    proc_block_del(blk);
    del_self_buff_from_range(mp->blk_id, start);
    idle_buff_range_free_ahead(blk_id);

    return DRV_ERROR_NONE;
}
