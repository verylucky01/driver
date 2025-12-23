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
#include "buff_event.h"
#include "buff_large_buf_adapt.h"

/*lint -e429*/
drvError_t buff_usr_alloc_large_buf(struct buff_req_mz_alloc_huge_buf *info)
{
    struct memzone_huge_user_mng_t *huge_mz = NULL;
    struct uni_buff_head_t *head = NULL;
    struct block_mem_ctx *block = NULL;
    uint64_t total_size;
    void *start = NULL;
    drvError_t ret;
    uint32_t blk_id;

    huge_mz = malloc(sizeof(*huge_mz));
    if (huge_mz == NULL) {
        buff_err("Buff alloc handle failed\n");
        return DRV_ERROR_OUT_OF_MEMORY;
    }
    huge_mz->head.type = UNI_TYPE_LARGE;
    huge_mz->head.devid = (uint32_t)info->devid;
    huge_mz->head.parent = NULL;

    block = malloc(sizeof(struct block_mem_ctx));
    if (block == NULL) {
        free(huge_mz);
        buff_err("Buff mem ctx alloc failed\n");
        return DRV_ERROR_OUT_OF_MEMORY;
    }
    ret = proc_block_ctx_init(info->grp_id, HUGE_BUF_LIST, (void *)huge_mz, block);
    if (ret != DRV_ERROR_NONE) {
        buff_err("Buff mem ctx alloc failed\n");
        goto free_huge_mz;
    }

    total_size = buff_calc_size(info->size, UNI_ALIGN_MIN, 0);
    start = buff_blk_alloc(info->grp_id, total_size, info->sp_flag, &blk_id); /* align 4k in kernel malloc */
    if (start == NULL) {
        buff_event("Can not alloc. (len=%llx; sp_flag=%#llx)\n", total_size, info->sp_flag);
        ret = DRV_ERROR_OUT_OF_MEMORY;
        goto free_huge_mz;
    }
    huge_mz->area.mz_mem_uva = start;
    huge_mz->area.mz_mem_total_size = total_size;
    huge_mz->area.blk_id = blk_id;

    ret = add_self_buff_to_range(blk_id, info->grp_id, start, total_size, (uint64)(uintptr_t)huge_mz);
    if (ret != 0) {
        buff_err("buff add to range failed, start %p len %llx\n", start, total_size);
        goto free_sp_res;
    }

    head = buff_head_init((uintptr_t)start, (uintptr_t)start + total_size, UNI_ALIGN_MIN, 1, 0);
    head->buff_type = BUFF_HUGE;
    head->align_flag = 0;
    head->ref = 1;

    buff_ext_info_init(head, buff_get_current_pid(), buff_get_process_uni_id());

    info->buf_uva = (uint64)(uintptr_t)(head + 1);
    info->blk_id = blk_id;
    huge_mz->uni_head = (uint64)head;
    huge_mz->head.parent = block;
    proc_block_add(block);
    buff_scale_out(start, total_size);

    buff_debug("Alloc. (large_size=%llu; flag=0x%llx; uva=0x%llx; start=0x%llx; blk_id=%u; devid=%d)\n",
        info->size, info->sp_flag, info->buf_uva, (uint64)(uintptr_t)start, blk_id, info->devid);

    return DRV_ERROR_NONE;

free_sp_res:
    buff_blk_free(info->grp_id, start);
free_huge_mz:
    free(huge_mz);
    free(block);
    return ret;
}
/*lint +e429*/

drvError_t buff_usr_free_large_buf(void *huge_mng, struct buff_req_mz_free_huge_buf *info)
{
    (void)info;
    struct memzone_huge_user_mng_t *huge_mz = (struct memzone_huge_user_mng_t *)huge_mng;
    uint32_t blk_id = huge_mz->area.blk_id;
    void *blk = NULL;

    blk = huge_mz->head.parent;
    if (blk == NULL) {
        buff_err("Parent is null. (blk_id=%u)\n", blk_id);
        return DRV_ERROR_INVALID_VALUE;
    }

    buff_debug("Free. (large_size=%llu; start=0x%llx; blk_id=%u)\n",
        (unsigned long long)huge_mz->area.mz_mem_total_size,
        (unsigned long long)(uintptr_t)huge_mz->area.mz_mem_uva, blk_id);

    buff_scale_in(huge_mz->area.mz_mem_uva, huge_mz->area.mz_mem_total_size);
    proc_set_buff_invalid(blk);
    proc_block_del(blk);

    del_self_buff_from_range(huge_mz->area.blk_id, huge_mz->area.mz_mem_uva);

    return DRV_ERROR_NONE;
}
