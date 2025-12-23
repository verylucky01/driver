/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DRV_BUFF_UNIBUFF_H
#define DRV_BUFF_UNIBUFF_H

#include "drv_buff_common.h"
#include "buff_range.h"

#define UNI_STATUS_IDLE     0   /* init or free */
#define UNI_STATUS_ALLOC    1   /* set it by alloc api */
#define UNI_STATUS_RELEASE  2   /* set it when free other proc buff */

#define UNI_HEAD_IMAGE      0xAABCDEFF
#define UNI_TAIL_IMAGE      0xFFBCDEAA

#define UNI_TYPE_ZONE       0
#define UNI_TYPE_MP         1
#define UNI_TYPE_LARGE      2
#define UNI_TYPE_MAX        3

#define UNI_UNIT_SIZE       8
#define UNI_FILE_NAME_LEN       8
#define UNI_RSV_HEAD_LEN       8

#define UNI_ALIGN_FLAG_ENABLE 1
#define UNI_BINARY_BASE  2

#define UNI_ALIGN_MAX       4096 /* must be less zone align */
#define UNI_ALIGN_MIN       32

#define UNI_MBUF_DATA_DISABLE 0
#define UNI_MBUF_DATA_ENABLE 1

#define UNI_BUFF_RECYCLE_FLAG_ENABLE 0
#define UNI_BUFF_RECYCLE_FLAG_DISABLE 1

#define UNI_BUFF_REFERENCE_FLAG_ENABLE 1U
#define UNI_BUFF_REFERENCE_FLAG_DISABLE 0U

#define BUFF_REF_MAX_MBUF_NUM  64

struct common_handle_t {
    uint32 type;
    uint32 status;
    void *parent;      /* pointer to struct block_mem_ctx */
    uint32 ref;
    uint32 devid;
};

/* enum Mbuf_type need to be modified accordingly */
enum uni_buff_type {
    TYPE_NONE = 0,
    BUFF_NORMAL,    // malloc by buff_alloc, in memzone
    BUFF_HUGE,     // huge size buff malloced by buff_alloc, not in memzone
    MBUF_NORMAL,      // malloc by mbuf_alloc
    MBUF_BY_POOL,     // malloc by mbuf_alloc_by_pool
    MBUF_BY_BUILD,     // malloc by mbuf_alloc_by_build
    MBUF_BARE_BUFF,     // malloc by mbuf_build_bare_buff
    MEMZONE_RELEASE_CACHE,
    MEMPOOL_SHARE_MNG,
    MEMPOOL_RECYCLE,
    TYPE_MAX,
};

/*
buffer format:
 | resv_head(if exist) | ext_info(if exist) | head_t | user data space | tail_t |
*/
struct uni_buff_head_t {
    uint32 image;
    uint32 timestamp;

    uint64_t status : 3;       /* UNI_STATUS_* above */
    uint64_t recycle_flag : 1; /* need to recycle? mp,memzone is disable, blk is enable */
    uint64_t ext_flag : 1;     /* indicate uni_buff_ext_info exist */
    uint64_t resv_head : 9;    /* resv head space for alignment, 8B unit, max alignment size is 4k.
                              * include size of resv head and ext info */
    uint64_t align_flag : 1;   /* if equal 1, mean this head is a align_head */
    uint64_t buff_type : 4;    /* the type of buff, see the definition of uni_buff_type */
    uint64_t mbuf_data_flag : 1; /* if queal 1, mean this buff belong to mbuf */
    uint64_t size : 40;           /* total size of buff, include head, user buff, and tail, aligne by 8B */
    uint64_t resv : 4;

    uint32 ref;
    uint32 index;
    uint32 offset; /* store offset between uni_head and align_head */
    uint32 rsv;  /* just for align */
};

struct buff_mbuf_info {
    uint32 pid : 16;
    uint32 opt_type : 4;
    uint32 qid : 12;
};

struct uni_buff_trace_t {
    void *mbuf[BUFF_REF_MAX_MBUF_NUM];
    uint32 alloc_uid[BUFF_REF_MAX_MBUF_NUM];
    uint32 proc_uid[BUFF_REF_MAX_MBUF_NUM];
    struct buff_mbuf_info mbuf_info[BUFF_REF_MAX_MBUF_NUM];
    char pool_name[BUFF_POOL_NAME_LEN];
    unsigned int blk_size;
    unsigned int total_blk_num;
};

struct uni_buff_ext_info {
    int    alloc_pid;
    int    use_pid;
    uint64 process_uni_id;
};

struct uni_buff_tail_t {
    uint32 image;
    uint32 size;           /* offset to head, aligne by 8B */
};

typedef int (*free_handle)(void *parent, void *buff);

drvError_t buff_free(uint32_t blk_id, void *buff, struct uni_buff_head_t *head);

struct uni_buff_head_t *buff_head_init(uintptr_t start, uintptr_t end,
    uint32 align, uint32 ext_info_flag, uint32 trace_flag);
void buff_ext_info_init(struct uni_buff_head_t *head, int pid, uint64 uni_process_id);
void buff_show_info(void *buff, uint32_t blk_id);
drvError_t buff_verify_and_get_head(void *buff, struct uni_buff_head_t **uni_head, uint32_t blk_id);
drvError_t mbuf_verify_and_get_head(void *mbuf, struct uni_buff_head_t **uni_head);
void buff_trace(void *buff, struct share_mbuf *s_mbuf, int pid, int opt_type, int qid);
void buff_trace_print(void *buff, struct mempool_t *mp);
struct uni_buff_head_t *buff_get_head(void *buff, uint32_t blk_id);

static inline struct uni_buff_head_t *buff_get_head_by_start(uintptr_t start, uintptr_t end, uint32 align,
    uint32 ext_info_flag, uint32 trace_flag)
{
    (void)end;
    uint32 offset = (uint32)sizeof(struct uni_buff_head_t);
    offset += (uint32)((ext_info_flag != 0) ? sizeof(struct uni_buff_ext_info) : 0);
    offset += (uint32)((trace_flag != 0) ? sizeof(struct uni_buff_trace_t) : 0);
    /*lint -e502 -e647*/
    void *data = (void *)(uintptr_t)ALIGN_UP(start + offset, align);
    /*lint +e502 +e647*/
    struct uni_buff_head_t *head  = NULL;

    head = (struct uni_buff_head_t *)((char *)data - sizeof(struct uni_buff_head_t));

    return head;
}

static inline void buff_put_head(struct uni_buff_head_t *head, uint32_t blk_id)
{
    buff_range_put(blk_id, head);
}

static inline struct uni_buff_head_t *buff_mempool_get_head(void *buff)
{
    /* buff - head is verified by Mbuf_get */
    struct uni_buff_head_t *head = (struct uni_buff_head_t *)((char *)buff - sizeof(struct uni_buff_head_t));

    return head;
}

static inline struct uni_buff_tail_t *buff_mempool_get_tail(void *start, uint32 blk_size, uint32 align_size)
{
    return (struct uni_buff_tail_t *)((uintptr_t)start + blk_size + align_size);
}

static inline struct uni_buff_trace_t *buff_mempool_get_trace(void *buff)
{
    struct uni_buff_head_t *head = buff_mempool_get_head(buff);

    return (struct uni_buff_trace_t *)(((char *)head - sizeof(struct uni_buff_ext_info)) -
            sizeof(struct uni_buff_trace_t));
}

static inline uint64_t buff_calc_size(uint64_t size, uint32 align, uint32 trace_flag)
{
    uint64_t head = (uint64_t)sizeof(struct uni_buff_head_t);

    head += (uint64_t)sizeof(struct uni_buff_ext_info);

    if (trace_flag != 0) {
        head += (uint64_t)sizeof(struct uni_buff_trace_t);
    }
    /*lint -e502 -e647*/
    head = ALIGN_UP(head, align);
    /*lint +e502 +e647*/
    return (head + size + sizeof(struct uni_buff_tail_t));
}

static inline struct uni_buff_ext_info *mbuf_get_ext_info(struct share_mbuf *mbuf)
{
    return (struct uni_buff_ext_info *)(((char *)mbuf - sizeof(struct uni_buff_head_t)) -
            sizeof(struct uni_buff_ext_info));
}

static inline uint32 mbuf_get_align_size(void)
{
    uint32 block_org_size = (uint32)buff_calc_size(sizeof(struct share_mbuf), UNI_ALIGN_MIN, 0);
    uint32 block_align_size = ALIGN_UP(block_org_size, UNI_ALIGN_MIN); //lint !e502 !e647 !e666

    return block_align_size - block_org_size;
}

static inline uint32 mbuf_get_verify_size(void)
{
    return (uint32)(sizeof(struct uni_buff_head_t) + sizeof(struct share_mbuf) +
        mbuf_get_align_size() + sizeof(struct uni_buff_tail_t));
}

static inline struct uni_buff_head_t *mbuf_get_head(struct share_mbuf *mbuf)
{
    return (struct uni_buff_head_t *)((char *)mbuf - sizeof(struct uni_buff_head_t));
}

static inline struct uni_buff_tail_t *mbuf_get_tail(struct share_mbuf *mbuf)
{
    return buff_mempool_get_tail(mbuf, sizeof(struct share_mbuf), mbuf_get_align_size());
}

static inline struct uni_buff_ext_info *buff_get_ext_info(struct uni_buff_head_t *head, uint32_t blk_id)
{
    struct uni_buff_ext_info *info = NULL;
    drvError_t ret;

    if (head->ext_flag != 0) {
        info = (struct uni_buff_ext_info *)((char *)head - sizeof(struct uni_buff_ext_info));
    }

    ret = buff_range_get(blk_id, info, sizeof(*info));
    if (ret != DRV_ERROR_NONE) {
        return NULL;
    }

    return info;
}

static inline void buff_put_ext_info(struct uni_buff_ext_info *info, uint32_t blk_id)
{
    buff_range_put(blk_id, info);
}

static inline struct uni_buff_tail_t *buff_get_tail(struct uni_buff_head_t *head, uint32_t blk_id)
{
    struct uni_buff_tail_t *tail = (struct uni_buff_tail_t *)((char *)head + head->size - sizeof(struct uni_buff_tail_t));
    drvError_t ret;

    ret = buff_range_get(blk_id, tail, sizeof(*tail));
    if (ret != DRV_ERROR_NONE) {
        return NULL;
    }

    return tail;
}

static inline void buff_put_tail(struct uni_buff_tail_t *tail, uint32_t blk_id)
{
    buff_range_put(blk_id, tail);
}

/**
 * Returns true if n is a power of 2
 * @param n
 *     Number to check
 * @return 1 if true, 0 otherwise
 */
static inline bool buff_is_power_of_2(uint32 n)
{
    return ((n - 1) & n) == 0;
}

static inline bool buff_check_align(uint32 align)
{
    if (align < UNI_ALIGN_MIN) {
        return 0;
    }
    return (buff_is_power_of_2(align) && (align <= UNI_ALIGN_MAX));
}

static inline int buff_size_is_invalid(uint64_t size, uint64 total)
{
    return (size > total);
}

#endif /* _DRV_BUFF_UNIBUFF_H_ */
