/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "bbox_kernel_log_parser.h"
#include "bbox_kernel_rblog_parser.h"

#include "securec.h"
#include "bbox_print.h"

enum bbox_desc_state {
    BBOX_DESC_MISS = -1,       /* mismatch */
    BBOX_DESC_RESERVED = 0,    /* using, reserved */
    BBOX_DESC_COMMITTED = 1,   /* committed, could get reopened */
    BBOX_DESC_FINALIZED = 2,   /* committed, not allowed modification  */
    BBOX_DESC_REUSABLE = 3,    /* free */
};

#define BBOX_LOG_LINE_MAX 1024
#define BBOX_FAILED_LPOS 0x1
#define BBOX_NO_LPOS 0x3

struct bbox_rb_data_block {
    unsigned long id;
    char data[];
};

STATIC unsigned long bbox_data_index(struct rb_data_ring *data_ring, unsigned long lpos)
{
    return (lpos & ((1UL << (data_ring->size_bits)) - 1));
}

STATIC unsigned long bbox_desc_index(struct rb_desc_ring *desc_ring, unsigned long index)
{
    return (index & ((1U << (desc_ring->count_bits)) - 1));
}

STATIC unsigned long bbox_desc_id(unsigned long state)
{
    /* 3 0b11, 8bit, 2bit */
    return (state & (~(3UL << ((sizeof(unsigned long) * 8) - 2))));
}

STATIC unsigned long bbox_data_wraps(struct rb_data_ring *data_ring, unsigned long lpos)
{
    return (lpos >> ((data_ring)->size_bits));
}

STATIC bool bbox_blk_data_less(struct rb_data_blk_lpos *blk)
{
    return ((blk->begin) & 1UL) && ((blk->next) & 1UL);
}

STATIC enum bbox_desc_state bbox_rb_get_desc_state(unsigned long id,
    unsigned long state_val)
{
    if (id != bbox_desc_id(state_val)) {
        return BBOX_DESC_MISS;
    }
    /* 3 0b11, 8bit, 2bit */
    return (enum bbox_desc_state)(3UL & (state_val >> ((sizeof(unsigned long) * 8) - 2)));
}

STATIC enum bbox_desc_state bbox_rb_desc_read(struct rb_desc_ring *desc_ring,
    unsigned long id, struct rb_desc *desc_out, u64 *seq_out)
{
    struct rb_printk_info *info = NULL;
    struct rb_desc *desc = NULL;
    enum bbox_desc_state d_state;
    unsigned long *state_var = NULL;
    unsigned long state_val, desc_index;

    desc_index = bbox_desc_index(desc_ring, id);
    desc = &desc_ring->descs[desc_index];
    info = &desc_ring->infos[desc_index];
    state_var = (unsigned long *)&desc->state_var;
    state_val = *state_var;

    d_state = bbox_rb_get_desc_state(id, state_val);
    if ((d_state == BBOX_DESC_MISS) || (d_state == BBOX_DESC_RESERVED)) {
        desc_out->state_var = state_val;
        return d_state;
    }

    if (seq_out != NULL) {
        *seq_out = info->seq;
    }
    desc_out->text_blk_lpos = desc->text_blk_lpos;
    desc_out->state_var = state_val;

    return d_state;
}

STATIC const char *bbox_rb_get_data(struct rb_data_ring *data_ring,
    struct rb_data_blk_lpos *blk_lpos, u32 *data_size)
{
    struct bbox_rb_data_block *data_block;
    unsigned long index;

    if (bbox_blk_data_less(blk_lpos)) {
        if ((blk_lpos->begin == BBOX_NO_LPOS) && (blk_lpos->next == BBOX_NO_LPOS)) {
            *data_size = 0;
            return "";
        }
        return NULL;
    }

    if ((bbox_data_wraps(data_ring, blk_lpos->begin) == bbox_data_wraps(data_ring, blk_lpos->next)) &&
        (blk_lpos->begin < blk_lpos->next)) {
        index = bbox_data_index(data_ring, blk_lpos->begin);
        data_block = (struct bbox_rb_data_block *)&data_ring->data[index];
        *data_size = (u32)(blk_lpos->next - blk_lpos->begin);
    } else if (bbox_data_wraps(data_ring, blk_lpos->begin + (1UL << (data_ring->size_bits))) ==
        bbox_data_wraps(data_ring, blk_lpos->next)) {
        index = bbox_data_index(data_ring, 0);
        data_block = (struct bbox_rb_data_block *)&data_ring->data[index];
        *data_size = (u32)bbox_data_index(data_ring, blk_lpos->next);
    } else {
        return NULL;
    }

    *data_size -= (u32)sizeof(data_block->id);

    return &data_block->data[0];
}

STATIC bbox_status bbox_rb_copy_data(struct rb_data_ring *data_ring,
    struct rb_data_blk_lpos *blk_lpos, u16 len, char *buf, u32 buf_size)
{
    const char *data = NULL;
    u32 data_size;
    int ret;

    if ((buf == NULL) || (buf_size == 0)) {
        return BBOX_SUCCESS;
    }

    data = bbox_rb_get_data(data_ring, blk_lpos, &data_size);
    if (data == NULL) {
        return BBOX_FAILURE;
    }

    if (data_size < (u32)len) {
        return BBOX_FAILURE;
    }

    data_size = (buf_size > data_size) ? data_size : buf_size;
    ret = memcpy_s(&buf[0], buf_size, data, data_size);
    if (ret != 0) {
        BBOX_WAR("Desc memcpy_s unsuccessfully, need export again.");
        return BBOX_FAILURE;
    }

    return BBOX_SUCCESS;
}

STATIC int bbox_rb_desc_read_finalized_seq(struct rb_desc_ring *desc_ring,
    unsigned long id, u64 seq, struct rb_desc *des_out)
{
    enum bbox_desc_state d_state;
    u64 s;

    d_state = bbox_rb_desc_read(desc_ring, id, des_out, &s);
    if ((d_state == BBOX_DESC_MISS) ||
        (d_state == BBOX_DESC_RESERVED) ||
        (d_state == BBOX_DESC_COMMITTED) ||
        (s != seq)) {
        return -EINVAL;
    }
    if ((d_state == BBOX_DESC_REUSABLE) ||
        ((des_out->text_blk_lpos.begin == BBOX_FAILED_LPOS) &&
        (des_out->text_blk_lpos.next == BBOX_FAILED_LPOS))) {
        return -ENOENT;
    }

    return 0;
}

STATIC int bbox_rb_read(struct rb_printk *rb, u64 seq,
    struct rb_printk_record *record)
{
    struct rb_desc_ring *desc_ring = NULL;
    struct rb_printk_info *info = NULL;
    struct rb_desc *rb_desc = NULL;
    struct rb_desc desc;
    unsigned long *state_var = NULL;
    unsigned long desc_index;
    int err;

    desc_ring = &rb->desc_ring;
    desc_index = bbox_desc_index(desc_ring, seq);
    rb_desc = &desc_ring->descs[desc_index];
    state_var = &rb_desc->state_var;

    err = bbox_rb_desc_read_finalized_seq(desc_ring, bbox_desc_id(*state_var), seq, &desc);
    if ((err != 0) || (record == NULL)) {
        return err;
    }

    info = &desc_ring->infos[desc_index];
    if (record->info != NULL) {
        *record->info = *info;
    }
    if (bbox_rb_copy_data(&rb->text_data_ring, &desc.text_blk_lpos, info->text_len,
        record->text_buf, record->text_buf_size) != BBOX_SUCCESS) {
        return -ENOENT;
    }

    return 0;
}

STATIC bbox_status bbox_rb_read_valid(struct rb_printk *rb, u64 *seq,
    struct rb_printk_record *record)
{
    int err;

    while ((err = bbox_rb_read(rb, *seq, record)) != 0) {
        if (err == -ENOENT) {
            (*seq)++;
        } else {
            return BBOX_FAILURE;
        }
    }

    return BBOX_SUCCESS;
}

STATIC u64 bbox_rb_first_seq(struct rb_printk *rb)
{
    struct rb_desc_ring *desc_ring = &rb->desc_ring;
    enum bbox_desc_state d_state;
    struct rb_desc desc;
    unsigned long id, max_id;
    u64 seq = 0;

    id = rb->desc_ring.tail_id;
    max_id = rb->desc_ring.head_id;
    for (; id < max_id; id++) {
        d_state = bbox_rb_desc_read(desc_ring, id, &desc, &seq);
        if ((d_state == BBOX_DESC_FINALIZED) || (d_state == BBOX_DESC_COMMITTED)) {
            return seq;
        }
    }
    BBOX_WAR("Printk in using, kernel log incorrect, try export again.");
    return seq;
}

/**
 * @brief       dump kernel message to file
 * @param [in]  record:  kernel message
 * @param [in]  log_path: path to write log
 * @return      saved message length on success, otherwise -1.
 */
STATIC s32 bbox_printk_record_dump(const struct rb_printk_record *record, const char *log_path)
{
    const char *char_log;
    u64 ts_usec;

    // collect timestamp
    ts_usec = record->info->ts_nsec / (u64)NSEC_PER_USEC;
    char_log = (const char *)record->text_buf;
    return bbox_klog_save_buf_to_fs(char_log, record->info->text_len, ts_usec, log_path);
}

/**
 * @brief       Interface for dump kernel log into file-system.
 *              The kernel log data may be incorrect, resulting in interpretation failure.
 * @param [in]  buffer  The buffer of kernel log data.
 * @param [in]  len     The length of the buffer.
 * @param [in]  log_path The path for DDRDUMP module dump data into.
 * @return      0 on success otherwise -1
 */
bbox_status bbox_klog_save_fs_ring_buffer_log(const void *buffer, u32 len, const char *log_path)
{
    u64 head_size = (u64)sizeof(struct printk_area_head);
    struct printk_area_head *head = (struct printk_area_head *)(uintptr_t)buffer;
    struct rb_printk ring_buffer = *((struct rb_printk *)head->manage_data);
    struct rb_printk_record record;
    struct rb_printk_info info;
    s32 text_len, ring_buffer_size, desc_bits, info_size, desc_size;
    char text_buf[BBOX_LOG_LINE_MAX] = {0};
    u64 seq;

    desc_bits = (s32)(1u << ring_buffer.desc_ring.count_bits);
    info_size = desc_bits * (s32)(sizeof(struct rb_printk_info));
    desc_size = desc_bits * (s32)(sizeof(struct rb_desc));
    ring_buffer_size = (s32)(1u << ring_buffer.text_data_ring.size_bits) + info_size + desc_size;
    if ((len == 0) || (len < (u32)ring_buffer_size)) {
        BBOX_WAR("Invalid param, msg length[%u], text length[%u].", len, (u32)ring_buffer_size);
        return BBOX_FAILURE;
    }

/*
 *      Memory distribution
 *      -----------------------------------------------------
 *      | PAGE_SIZE  RO                                     |
 *      -----------------------------------------------------
 *      | PAGE_SIZE  manage_data  (printk_ringbuffer)       |
 *      -----------------------------------------------------
 *      | PAGE_SIZE  RO                                     |
 *      -----------------------------------------------------
 *      | Remaining  printk_ringbuffer:                     |
 *      |            rb_desc                                 |
 *      |            printk_info                             |
 *      |            text_data_ring                           |
 *      -----------------------------------------------------
 *      | PAGE_SIZE  RO                                     |
 *      -----------------------------------------------------
 */
    ring_buffer.desc_ring.descs = (struct rb_desc *)((char *)(uintptr_t)buffer + head_size);
    ring_buffer.desc_ring.infos = (struct rb_printk_info *)((char *)ring_buffer.desc_ring.descs + desc_size);
    ring_buffer.text_data_ring.data = (char *)((char *)ring_buffer.desc_ring.infos + info_size);

    record.info = &info;
    record.text_buf = text_buf;
    record.text_buf_size = BBOX_LOG_LINE_MAX;

    seq = bbox_rb_first_seq(&ring_buffer);
    text_len = 0;
    while (text_len < ring_buffer_size) {
        s32 tmp_text_len;
        if (bbox_rb_read_valid(&ring_buffer, &seq, &record) != BBOX_SUCCESS) {
            break;
        }
        tmp_text_len = bbox_printk_record_dump((const struct rb_printk_record *)&record, log_path);
        text_len += tmp_text_len;
        // get next printk log seq
        seq = record.info->seq + 1;
        memset_s(record.text_buf, record.text_buf_size, 0, record.text_buf_size);
    }

    if (text_len == 0) {
        u32 log_size;
        BBOX_WAR("Can not parse ringbuff, start to dump all log.");
        log_size = 1u << ring_buffer.text_data_ring.size_bits;
        bbox_klog_save_all_log_buf_to_fs(ring_buffer.text_data_ring.data, log_size, log_path);
    }

    return BBOX_SUCCESS;
}

bool bbox_klog_is_ring_buffer_log(const char *manage_data)
{
    const struct rb_printk *ring_buffer = (struct rb_printk *)(uintptr_t)manage_data;
    const struct rb_printk_area_size *area_size = (const struct rb_printk_area_size *)(ring_buffer + 1);

    if (area_size->end_flag == PRINTK_MANAGEMENT_ENG_FLAG) {
        return true;
    }
    return false;
}

