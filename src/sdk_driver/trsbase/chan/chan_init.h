/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef CHAN_INIT_H
#define CHAN_INIT_H

#include "ka_base_pub.h"
#include "ka_task_pub.h"

#include "pbl_kref_safe.h"
#include "chan_ts_inst.h"

struct trs_chan_sq_ctx {
    u32 sqid;
    u32 status;
    u32 sq_head;
    u32 sq_tail;
    u32 pos;
    ka_semaphore_t sem;
    void *sq_addr;
    void *sq_dev_vaddr; /* sq device vaddr, used by host */
    u32 type;
    struct trs_chan_mem_attr mem_attr;
    u64 head_addr;
    u64 tail_addr;
    u64 db_addr;
    struct trs_chan_sq_para para;
    ka_wait_queue_head_t wait_queue;
    ka_task_spinlock_t lock;    /* submit task in tasklet */
    u32 long_sqe_cnt;
};

struct trs_chan_cq_ctx {
    u32 cqid;
    u32 loop;
    u32 cq_head;
    u32 cq_tail;
    u32 cqe_valid;
    void *cq_addr;
    u32 type;
    struct trs_chan_mem_attr mem_attr;
    struct trs_chan_cq_para para;
    ka_mutex_t mutex;
    ka_wait_queue_head_t wait_queue;
};

struct trs_chan_stat {
    u64 tx;
    u64 rx;
    u64 tx_full;
    u64 rx_empty;
    u64 tx_timeout;
    u64 rx_dispatch;
    u64 rx_wakeup;
    u64 hw_err;
    u64 rx_in;
};

struct trs_chan {
    int id;
    struct kref_safe ref;
    u32 flag;
    u32 irq;
    int pid;
    int ssid;
    u32 msg[SQCQ_INFO_LENGTH];
    u32 ext_msg_len;
    void *ext_msg;
    ka_atomic_t chan_status;
    struct trs_chan_ts_inst *ts_inst;
    struct trs_chan_irq_ctx *irq_ctx;
    ka_list_head_t node;
    struct trs_id_inst inst;
    struct trs_chan_type types;
    struct trs_chan_ops ops;
    struct trs_chan_sq_ctx sq;
    struct trs_chan_cq_ctx cq;
    struct trs_chan_stat stat;
    ka_work_struct_t work;
    ka_proc_dir_entry_t *entry;
    ka_task_spinlock_t lock;    /* for cq dispatch */
    int work_running;
    u32 retry_times;
};

static inline bool trs_chan_has_sq(struct trs_chan *chan)
{
    return ((chan->flag & (0x1 << CHAN_FLAG_ALLOC_SQ_BIT)) != 0);
}

static inline bool trs_chan_has_cq(struct trs_chan *chan)
{
    return ((chan->flag & (0x1 << CHAN_FLAG_ALLOC_CQ_BIT)) != 0);
}

static inline bool trs_chan_is_notice_ts(struct trs_chan *chan)
{
    return ((chan->flag & (0x1 << CHAN_FLAG_NOTICE_TS_BIT)) != 0);
}

static inline bool trs_chan_is_auto_update_sq_head(struct trs_chan *chan)
{
    return ((chan->flag & (0x1 << CHAN_FLAG_AUTO_UPDATE_SQ_HEAD_BIT)) != 0);
}

static inline bool trs_chan_is_recv_block(struct trs_chan *chan)
{
    return ((chan->flag & (0x1 << CHAN_FLAG_RECV_BLOCK_BIT)) != 0);
}

static inline void trs_chan_set_not_notice_ts(struct trs_chan *chan)
{
    chan->flag &= ~(0x1 << CHAN_FLAG_NOTICE_TS_BIT);
}

static inline bool trs_chan_has_sq_mem(struct trs_chan *chan)
{
    return ((chan->flag & (0x1 << CHAN_FLAG_NO_SQ_MEM_BIT)) == 0);
}

static inline bool trs_chan_has_cq_mem(struct trs_chan *chan)
{
    return ((chan->flag & (0x1 << CHAN_FLAG_NO_CQ_MEM_BIT)) == 0);
}

static inline bool trs_chan_specified_sq_id(struct trs_chan *chan)
{
    return ((chan->flag & (0x1 << CHAN_FLAG_SPECIFIED_SQ_ID_BIT)) != 0);
}

static inline bool trs_chan_specified_cq_id(struct trs_chan *chan)
{
    return ((chan->flag & (0x1 << CHAN_FLAG_SPECIFIED_CQ_ID_BIT)) != 0);
}

static inline bool trs_chan_reserved_sq_id(struct trs_chan *chan)
{
    return ((chan->flag & (0x1 << CHAN_FLAG_RESERVED_SQ_ID_BIT)) != 0);
}

static inline bool trs_chan_reserved_cq_id(struct trs_chan *chan)
{
    return ((chan->flag & (0x1 << CHAN_FLAG_RESERVED_CQ_ID_BIT)) != 0);
}

static inline bool trs_chan_ranged_sq_id(struct trs_chan *chan)
{
    return ((chan->flag & (0x1 << CHAN_FLAG_RANGE_SQ_ID_BIT)) != 0);
}

static inline bool trs_chan_ranged_cq_id(struct trs_chan *chan)
{
    return ((chan->flag & (0x1 << CHAN_FLAG_RANGE_CQ_ID_BIT)) != 0);
}

static inline bool trs_chan_rts_rsv_sq_id(struct trs_chan *chan)
{
    return ((chan->flag & (0x1 << CHAN_FLAG_RTS_RSV_SQ_ID_BIT)) != 0);
}
 
static inline bool trs_chan_rts_rsv_cq_id(struct trs_chan *chan)
{
    return ((chan->flag & (0x1 << CHAN_FLAG_RTS_RSV_CQ_ID_BIT)) != 0);
}

struct trs_chan *trs_chan_get(struct trs_id_inst *inst, u32 chan_id);
void trs_chan_put(struct trs_chan *chan);
void trs_all_chan_cq_work_cancel(struct trs_chan_ts_inst *ts_inst);

#endif

