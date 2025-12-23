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

#ifndef ESCHED_KERNEL_INNERFACE_H
#define ESCHED_KERNEL_INNERFACE_H

#include "ascend_hal_define.h"
#include "esched_kernel_interface_cmd.h"
#define SCHED_SUCCESS 0

#define SCHED_INVALID_PID 0xFFFFFFFFU

#define SCHED_MAX_THREAD_NUM_IN_GRP     1024

#define SCHED_WAIT_TIMEOUT_NO_WAIT 0

#define SCHED_TASK_FINISH_SCENE_NORMAL        0x1U
#define SCHED_TASK_FINISH_SCENE_PROC_EXIT     0x2U
#define SCHED_TASK_FINISH_SCENE_TIME_OUT      0x3U
#define SCHED_TASK_FINISH_SCENE_NORMAL_SOFT_TIMEOUT  0x10U
#define SCHED_TASK_FINISH_SCENE_NORMAL_GETEVENT      0x11U
#define SCHED_TASK_FINISH_SCENE_NORMAL_PROCESS_FREE  0x12U

#define SCHED_EVENT_PRE_PROC_SUCCESS            DRV_ERROR_NONE
#define SCHED_EVENT_PRE_PROC_SUCCESS_RETURN     (DRV_ERROR_RESERVED + 1)

typedef enum sched_pre_proc_pos {
    SCHED_PRE_PROC_POS_LOCAL = 0,
    SCHED_PRE_PROC_POS_REMOTE,
    SCHED_PRE_PROC_POS_MAX
} SCHED_PROC_POS;

struct sched_event_func_info {
    unsigned int devid;
    unsigned int subevent_id;
    const char *msg;
    unsigned int msg_len;
};

struct sched_published_event_func {
    int (*event_ack_func)(unsigned int devid, unsigned int subevent_id, const char *msg,
        unsigned int msg_len, void *priv);
    void (*event_finish_func)(struct sched_event_func_info *finish_info, unsigned int finish_scene, void *priv);
};

struct sched_published_event {
    struct sched_published_event_info event_info;
    struct sched_published_event_func event_func;
};

drvError_t register_esched_trace_record_func(unsigned int grp_id, unsigned int event_id,
    void (*finish_func)(unsigned int grp_id, unsigned int event_id, unsigned int subevent_id,
        sched_trace_time_info *sched_trace_time_info));

int hal_kernel_sched_submit_event(unsigned int chip_id, struct sched_published_event *event);
int sched_query_local_task_gid(unsigned int chip_id, int pid, const char *grp_name, unsigned int *gid);
int sched_query_local_trace(unsigned int chip_id,
    int pid, unsigned int gid, unsigned int tid, struct sched_sync_event_trace *sched_trace);
int sched_query_sync_event_trace(unsigned int chip_id,
    unsigned int dev_pid, unsigned int gid, unsigned int tid, struct sched_sync_event_trace *trace_result);

typedef int (*sched_event_pre_proc)(unsigned int udevid, struct sched_published_event_info *event_info,
    struct sched_published_event_func *event_func);
int hal_kernel_sched_register_event_pre_proc_handle(unsigned int event_id, SCHED_PROC_POS pos, sched_event_pre_proc handle);
void hal_kernel_sched_unregister_event_pre_proc_handle(unsigned int event_id, SCHED_PROC_POS pos, sched_event_pre_proc handle);
int sched_query_sched_mode_by_grpid(unsigned int chip_id, unsigned int pid, unsigned int gid, unsigned int *sched_mode);
int sched_drv_fill_topic_sqe(unsigned int chip_id, struct sched_published_event_info *event_info, void *sqe_t);

#endif

