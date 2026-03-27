/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
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

#undef TRACE_SYSTEM
#define TRACE_SYSTEM trs_chan

#if !defined(CHAN_TRACE_EVENT_H) || defined(TRACE_HEADER_MULTI_READ)
#define CHAN_TRACE_EVENT_H

#include "ka_system_pub.h"
#include <linux/tracepoint.h>

#include "trs_pub_def.h"
#include "trs_chan.h"
#include "chan_trace.h"

KA_DFX_TRACE_EVENT(sqe,
    KA_DFX_TP_PROTO(const char *str, struct trs_id_inst *inst, struct trs_chan_sq_trace *sq_trace),
    KA_DFX_TP_ARGS(str, inst, sq_trace),
    KA_DFX_TP_STRUCT__entry(
        __ka_dfx_string(           str,                        str         )
        __ka_dfx_field_struct(     struct trs_id_inst,         inst        )
        __ka_dfx_field_struct(     struct trs_chan_sq_trace,   sq_trace    )
        __ka_dfx_field_struct(     ka_timespec64_t,          timestamp   )
    ),
    KA_DFX_TP_fast_assign(
        if (str != NULL) {
            __ka_dfx_assign_str(str, str);
        } else  {
            __ka_dfx_assign_str(str, "nop");
        }
        __ka_entry->inst = *inst;
        __ka_entry->sq_trace = *sq_trace;
        ka_system_ktime_get_ts64(&__ka_entry->timestamp);
    ),
    KA_DFX_TP_printk(
        "%s:\t"
        "devid=%u;"
        "tsid=%u;"
        "chan_id=%u;"
        "chan_name=%s;"
        "sqid=%u;"
        "sq_head=%u;"
        "sq_tail=%u;"
        "sqe_type=%u;"
        "task_id=%u;"
        "stream_id=%u;"
        "%lld.%06ld",
        __ka_dfx_get_str(str),
        __ka_entry->inst.devid,
        __ka_entry->inst.tsid,
        __ka_entry->sq_trace.chan_id,
        trs_chan_type_to_name(&__ka_entry->sq_trace.types),
        __ka_entry->sq_trace.sqid,
        __ka_entry->sq_trace.sq_head,
        __ka_entry->sq_trace.sq_tail,
        __ka_entry->sq_trace.type,
        __ka_entry->sq_trace.task_id,
        __ka_entry->sq_trace.stream_id,
        /* tv_sec type is depends on linux version */
        (long long)__ka_entry->timestamp.tv_sec, __ka_entry->timestamp.tv_nsec / KA_NSEC_PER_USEC
    )
);

KA_DFX_TRACE_EVENT(cqe,
    KA_DFX_TP_PROTO(const char *str, struct trs_id_inst *inst, struct trs_chan_cq_trace *cq_trace),
    KA_DFX_TP_ARGS(str, inst, cq_trace),
    KA_DFX_TP_STRUCT__entry(
        __ka_dfx_string(           str,                            str         )
        __ka_dfx_field_struct(     struct trs_id_inst,             inst        )
        __ka_dfx_field_struct(     struct trs_chan_cq_trace,       cq_trace    )
        __ka_dfx_field_struct(     ka_timespec64_t,              timestamp   )
    ),
    KA_DFX_TP_fast_assign(
        if (str != NULL) {
            __ka_dfx_assign_str(str, str);
        } else {
            __ka_dfx_assign_str(str, "nop");
        }
        __ka_entry->inst = *inst;
        __ka_entry->cq_trace = *cq_trace;
        ka_system_ktime_get_ts64(&__ka_entry->timestamp);
    ),
    KA_DFX_TP_printk(
        "%s:\t"
        "devid=%u;"
        "tsid=%u;"
        "chan_id=%u;"
        "chan_name=%s;"
        "cqid=%u;"
        "cq_head=%u;"
        "round=%u;"
        "task_id=%u;"
        "stream_id=%u;"
        "sq_id=%u;"
        "sq_head=%u;"
        "%lld.%06ld",
        __ka_dfx_get_str(str),
        __ka_entry->inst.devid,
        __ka_entry->inst.tsid,
        __ka_entry->cq_trace.chan_id,
        trs_chan_type_to_name(&__ka_entry->cq_trace.types),
        __ka_entry->cq_trace.cqid,
        __ka_entry->cq_trace.cq_head,
        __ka_entry->cq_trace.round,
        __ka_entry->cq_trace.task_id,
        __ka_entry->cq_trace.stream_id,
        __ka_entry->cq_trace.sq_id,
        __ka_entry->cq_trace.sq_head,
        /* tv_sec type is depends on linux version */
        (long long)__ka_entry->timestamp.tv_sec, __ka_entry->timestamp.tv_nsec / KA_NSEC_PER_USEC
    )
)

KA_DFX_TRACE_EVENT(recv,
    KA_DFX_TP_PROTO(const char *str, struct trs_id_inst *inst, struct trs_chan_recv_trace *recv_trace),
    KA_DFX_TP_ARGS(str, inst, recv_trace),
    KA_DFX_TP_STRUCT__entry(
        __ka_dfx_string(           str,                            str         )
        __ka_dfx_field_struct(     struct trs_id_inst,             inst        )
        __ka_dfx_field_struct(     struct trs_chan_recv_trace,     recv_trace  )
        __ka_dfx_field_struct(     ka_timespec64_t,              timestamp   )
    ),
    KA_DFX_TP_fast_assign(
        if (str != NULL) {
            __ka_dfx_assign_str(str, str);
        } else {
            __ka_dfx_assign_str(str, "nop");
        }
        __ka_entry->inst = *inst;
        __ka_entry->recv_trace = *recv_trace;
        ka_system_ktime_get_ts64(&__ka_entry->timestamp);
    ),
    KA_DFX_TP_printk(
        "%s:\t"
        "devid=%u;"
        "tsid=%u;"
        "chan_id=%u;"
        "chan_name=%s;"
        "cqid=%u;"
        "cq_head=%u;"
        "cqe_num=%u;"
        "recv_cqe_num=%u;"
        "timeout=%d;"
        "%lld.%06ld",
        __ka_dfx_get_str(str),
        __ka_entry->inst.devid,
        __ka_entry->inst.tsid,
        __ka_entry->recv_trace.chan_id,
        trs_chan_type_to_name(&__ka_entry->recv_trace.types),
        __ka_entry->recv_trace.cqid,
        __ka_entry->recv_trace.cq_head,
        __ka_entry->recv_trace.cqe_num,
        __ka_entry->recv_trace.recv_cqe_num,
        __ka_entry->recv_trace.timeout,
        /* tv_sec type is depends on linux version */
        (long long)__ka_entry->timestamp.tv_sec, __ka_entry->timestamp.tv_nsec / KA_NSEC_PER_USEC
    )
)
#endif /* CHAN_TRACE_EVENT_H */

/* This part must be outside protection */
#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH .
#define TRACE_INCLUDE_FILE chan_trace_event
#include <trace/define_trace.h>

