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

#include "ascend_hal_define.h"
#include "esched_kernel_interface.h"
#include "svm_sub_event_type.h"
#include "smp_msg.h"
#include "svm_kern_log.h"
#include "smp_event.h"

struct svm_smp_del_msg_sync {
    struct event_sync_msg sync_head;
    struct svm_smp_del_msg msg;
};

void smp_trigger_event(u32 udevid, int tgid, u64 start, u64 size)
{
    struct sched_published_event publish_event;
    struct svm_smp_del_msg_sync del_msg_sync;
    struct svm_smp_del_msg *msg = &del_msg_sync.msg;
    int ret;

    ret = sched_query_local_task_gid(udevid, tgid, EVENT_DRV_MSG_GRP_NAME, &publish_event.event_info.gid);
    if (ret != 0) { /* CP may exited, do not print. */
        return;
    }

    publish_event.event_info.dst_engine = CCPU_LOCAL;
    publish_event.event_info.policy = 0;
    publish_event.event_info.pid = tgid;
    publish_event.event_info.event_id = EVENT_DRV_MSG;
    publish_event.event_info.subevent_id = SVM_SMP_DEL_MEM_EVENT;

    msg->va = start;
    msg->size = size;

    publish_event.event_info.msg = (char *)&del_msg_sync;
    publish_event.event_info.msg_len = sizeof(del_msg_sync);

    publish_event.event_func.event_finish_func = NULL;
    publish_event.event_func.event_ack_func = NULL;

    (void)hal_kernel_sched_submit_event(udevid, &publish_event); /* CP may exited, do not print. */
}

