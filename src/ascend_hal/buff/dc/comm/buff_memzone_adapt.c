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

#include "drv_buff_common.h"
#include "drv_buff_adp.h"
#include "drv_buff_memzone.h"
#include "buff_mng.h"
#include "buff_recycle.h"
#include "buff_range.h"
#include "buff_memzone_adapt.h"

#define BLK_RECYCLE_NUM(blk_num) ((blk_num) * 20 / 100) /* 20% */

static uint64 mz_get_user_mng_size(uint32 blk_num)
{
    return sizeof(struct memzone_user_mng_t) + bitmap_size(blk_num) + sizeof(uint64) * blk_num;
}

static drvError_t buff_mz_usr_mng_init(struct memzone_user_mng_t *mz, struct buff_req_mz_create *info)
{
    uint32 bitnum_mask, blk_num = (uint32)(info->total_size / info->blk_size);
    int ret;

    ret = memset_s((void *)mz, mz_get_user_mng_size(blk_num), 0, mz_get_user_mng_size(blk_num));
    if (ret != 0) {
        buff_err("Memset_s failed. (mz_size=%llx; ret=%d)\n", mz_get_user_mng_size(blk_num), ret);
        return DRV_ERROR_INNER_ERR;
    }

    ret = pthread_mutex_init(&mz->mutex, NULL);
    if (ret != 0) {
        buff_err("pthread_mutex_init failed\n");
        return DRV_ERROR_INNER_ERR;
    }

    mz->head.type   = UNI_TYPE_ZONE;
    mz->head.status = 1;
    mz->head.devid = (uint32_t)info->devid;
    mz->head.parent = NULL;

    mz->cfg_id = info->cfg_id;
    mz->pid = buff_get_current_pid();
    mz->grp_id = info->grp_id;
    mz->sp_flag = (uint32)info->sp_flag;
    mz->free_flag = 0;
    mz->alloc_flag = 0;

    mz->area.mz_mem_total_size = info->total_size;
    mz->mz_mem_free_size = info->total_size;

    mz->blk_size = info->blk_size;
    mz->blk_num_available = blk_num;
    mz->blk_num_total = blk_num;
    mz->cur_index = 0;
    mz->recycle_blk_num_level = BLK_RECYCLE_NUM(blk_num);

    mz->bitnum = blk_num;

    bitnum_mask = ALIGN_UP(blk_num, BITS_PER_LONG); //lint !e502
    bitmap_clear(mz->bitmap, 0, (int)(mz->bitnum));
    bitmap_set(mz->bitmap, (int)(mz->bitnum), (int)(bitnum_mask));

    return DRV_ERROR_NONE;
}

drvError_t buff_usr_mz_create(struct buff_req_mz_create *info)
{
    uint32 blk_num = (uint32)(info->total_size / info->blk_size);
    unsigned long flags = buff_make_devid_to_flags(info->devid, info->sp_flag);
    struct memzone_user_mng_t *mz = NULL;
    struct block_mem_ctx *block = NULL;
    void *start = NULL;
    drvError_t ret;
    uint32_t blk_id;

    mz = malloc(mz_get_user_mng_size(blk_num));
    if (mz == NULL) {
        buff_err("buff alloc mz mng failed\n");
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    ret = buff_mz_usr_mng_init(mz, info);
    if (ret != 0) {
        free(mz);
        buff_err("buff_mz_usr_mng_init failed\n");
        return ret;
    }

    block = malloc(sizeof(struct block_mem_ctx));
    if (block == NULL) {
        free(mz);
        buff_err("Buff mem ctx alloc failed\n");
        return DRV_ERROR_OUT_OF_MEMORY;
    }
    ret = proc_block_ctx_init(info->grp_id, MEMZONE_LIST, (void *)mz, block);
    if (ret != DRV_ERROR_NONE) {
        buff_err("Buff mem ctx alloc failed\n");
        goto free_mz;
    }

    start = buff_blk_alloc(info->grp_id, info->total_size, flags, &blk_id);
    if (start == NULL) {
        buff_event("Can not alloc. (len=%llx; flag=%#lx)\n", info->total_size, flags);
        ret = DRV_ERROR_OUT_OF_MEMORY;
        goto free_mz;
    }

    ret = add_self_buff_to_range(blk_id, info->grp_id, start, info->total_size, (uint64)(uintptr_t)mz);
    if (ret != 0) {
        buff_err("buff add to range failed, start %p len %llx; ret=%d; blk_id=%u\n",
            start, info->total_size, ret, blk_id);
        goto free_sp_res;
    }

    mz->area.mz_mem_uva = start;
    mz->area.blk_id = blk_id;
    info->mz_uva = (uint64)(uintptr_t)mz;
    info->start = start;
    mz->mz_list_node = info->mz_list_node;

    /* memzone_mng_list_add must before proc_block_add. Otherwise mz may be deleted in recycle thread. */
    memzone_mng_list_add(mz);

    mz->head.parent = block;
    proc_block_add(block);

    buff_debug("Mz create. (cfg_id=%u; blk_size=%u; blk_num=%d; total_size=0x%llx; sp_flag=%#lx; page_type=%u; mz=%llx; "
        "start=0x%llx; devid=%d)\n", info->cfg_id, info->blk_size, blk_num, info->total_size, flags, info->page_type,
        info->mz_uva, (uint64)(uintptr_t)start, info->devid);

    return DRV_ERROR_NONE;
free_sp_res:
    buff_blk_free(info->grp_id, start);
free_mz:
    free(mz);
    free(block);
    return ret;
}

drvError_t buff_usr_mz_delete(struct buff_req_mz_delete *info)
{
    struct memzone_user_mng_t *mz = (struct memzone_user_mng_t *)(uintptr_t)info->mz_user_mng_uva;
    void *start = mz->area.mz_mem_uva;
    void *blk = mz->head.parent;

    buff_debug("Mz delete success addr. (start=0x%llx)\n", (uint64)(uintptr_t)start);

    if (blk == NULL) {
        buff_err("Parent is null\n");
        return DRV_ERROR_INVALID_VALUE;
    }
    proc_set_buff_invalid(blk);

    del_self_buff_from_range(mz->area.blk_id, start);
    proc_block_del(blk);

    return DRV_ERROR_NONE;
}

