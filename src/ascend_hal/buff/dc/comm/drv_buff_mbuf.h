/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DRV_BUFF_MBUF_H
#define DRV_BUFF_MBUF_H

#include "drv_buff_common.h"
#include "drv_buff_adp.h"
#define MBUF_CHAIN_MAX_LEN  128

enum mbuf_opt_type {
    MBUF_ALLOC_BY_ZONE = 0,
    MBUF_ALLOC_BY_POOL,
    MBUF_ALLOC_BY_COPYREF,
    MBUF_ALLOC_BY_BUILD,
    MBUF_ALLOC_BY_BUILD_BARE_BUFF,
    MBUF_FREE_BEGIN,
    MBUF_FREE_BY_USER = MBUF_FREE_BEGIN,
    MBUF_FREE_BY_RECYCLE,
    MBUF_FREE_BY_QUEUE_OW,
    MBUF_FREE_BY_QUEUE_DQ,
    MBUF_FREE_BY_QUEUE_DS,
    MBUF_FREE_END = MBUF_FREE_BY_QUEUE_DS,
    MBUF_ENQUEUE,
    MBUF_DEQUEUE,
};

drvError_t mbuf_is_invalid(struct Mbuf *mbuf);
drvError_t s_mbuf_free(void *mp, struct share_mbuf *s_mbuf);
drvError_t mbuf_free_with_opt_type(struct Mbuf *mbuf, int type);
drvError_t mbuf_free_for_queue(Mbuf *mbuf, int type);
drvError_t hal_mbuf_alloc_align(uint64_t size, unsigned int align, Mbuf **mbuf);

void mbuf_owner_update_for_enque(struct Mbuf *mbuf, int pid, unsigned int qid);
void mbuf_owner_update_for_deque(struct Mbuf *mbuf, int pid, unsigned int qid);

drvError_t mbuf_build_bare_buff(void *buff, uint64_t len, struct Mbuf **mbuf);
void destroy_priv_mbuf_for_queue(struct Mbuf *mbuf);
drvError_t create_priv_mbuf_for_queue(struct Mbuf **mbuf, void *buff, uint32_t blk_id);

drvError_t buff_set_mbuf_timestamp(struct buff_config_handle_arg *para_in);
unsigned long long mbuf_get_timestamp(struct Mbuf *mbuf);
struct share_mbuf *get_share_mbuf_by_mbuf(Mbuf *mbuf);
void mbuf_set_priv_flag(unsigned int flag);
#endif /* _DRV_BUFF_MBUF_H_ */
