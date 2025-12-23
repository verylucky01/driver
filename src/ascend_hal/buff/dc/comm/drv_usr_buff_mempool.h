/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DRV_USR_BUFF_MEMPOOL_H
#define DRV_USR_BUFF_MEMPOOL_H

#include "drv_buff_unibuff.h"
#include "drv_buff_list.h"
#include "ascend_hal.h"
#include "drv_buff_common_mempool.h"

#define MP_S_NORMAL         0
#define MP_S_DESTROYED      1
#define MP_S_ABNORMAL       2

#define MP_F_PUBLIC         0
#define MP_F_PRIVATE        1
#define MP_BITMAP_INDEX_INVALID  (-1)
#define MP_BLK_SIZE_MAX (0xffffffff - ((UNI_ALIGN_MAX) * 2))

enum mp_alloc_fail_state {
    MP_ALLOC_NO_BLK = 1,
    MP_ALLOC_NO_BITMAP
};

void *mp_get_buff_uva_by_index(struct mempool_t *mp, unsigned long index);
drvError_t mp_alloc_mbuf_head(uint32_t devid, void **buff, uint32_t *blk_id);
drvError_t mp_create(mpAttr *attr, uint32 type, struct mempool_t **mp);
drvError_t mp_alloc_buff(struct mempool_t *mp, void **buff, uint32_t *blk_id);
void mp_destroy_mbuf_mp(struct mempool_t *mp);
void mp_mng_put(struct mempool_t *mp);

#endif /* _DRV_USR_BUFF_MEMPOOL_H_ */

