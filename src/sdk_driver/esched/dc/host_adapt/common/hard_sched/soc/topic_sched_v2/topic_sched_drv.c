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

#include <asm/io.h>
#include <linux/slab.h>
#include <linux/preempt.h>

#include "securec.h"

#include "esched.h"
#include "topic_sched_drv.h"
#include "esched_drv_adapt.h"
#include "esched_fops.h"

#include "tsdrv_interface.h"
#include "trs_chan.h"
#include "comm_kernel_interface.h"
#include "hwts_task_info.h"
#include "topic_sched_drv_common.h"

static u8 sched_sqe_type[TOPIC_TYPE_MAX] = {
    [TOPIC_TYPE_AICPU_DEVICE_ONLY] = TOPIC_SCHED_SQE_TYPE_DEVICE,
    [TOPIC_TYPE_AICPU_HOST_ONLY] = TOPIC_SCHED_SQE_TYPE_HOST,
};

static int esched_drv_fill_task_pid(u32 chip_id, u32 topic_type, u32 dst_pid, u32 *pid)
{
    u32 slave_pid;
    u32 master_pid;
    int ret;

    if (topic_type == TOPIC_TYPE_AICPU_DEVICE_ONLY) {
        *pid = dst_pid;
        return 0;
    }

    /* If dst_pid is host slave pid, repace it with cp1. */
#ifndef CFG_ENV_HOST
    ret = devdrv_query_master_pid_by_host_slave(dst_pid, &master_pid);
#else
    ret = devdrv_query_master_pid_by_device_slave(chip_id, dst_pid, &master_pid);
#endif
    if (ret != 0) {
        sched_err("query master pid failed. (host slave pid=%u)\n", dst_pid);
        return DRV_ERROR_NO_PROCESS;
    }

    ret = devdrv_query_process_by_host_pid(master_pid, chip_id, DEVDRV_PROCESS_CP1, 0, &slave_pid);
    if (ret != 0) {
        sched_err("query cp1 failed. (host_pid=%u)\n", dst_pid);
        return DRV_ERROR_NO_PROCESS;
    }

    *pid = slave_pid;
    return 0;
}

int esched_drv_fill_sqe(u32 chip_id, u32 event_src, struct topic_sched_sqe *sqe,
    struct sched_published_event_info *event_info)
{
    int ret;

    sqe->ie = 0;
    sqe->pre_p = 0;
    sqe->post_p = 0;
    sqe->wr_cqe = 0;
    sqe->ptr_mode = 0;
    sqe->ptt_mode = 0;
    sqe->head_update = 0;
    sqe->blk_dim = 1;
    sqe->rt_streamid = 0;
    sqe->task_id = 0;

    sqe->kernel_type = TOPIC_SCHED_DEFAULT_KERNEL_TYPE;
    sqe->batch_mode = 0;           /* no use. */
    sqe->topic_type = esched_drv_get_topic_type(event_info->policy, event_info->dst_engine);
    sqe->type = sched_sqe_type[sqe->topic_type];
    ret = esched_drv_fill_sqe_qos(chip_id, event_info, sqe);
    if (ret != 0) {
        sched_err("Failed to fill qos. (pid=%d; gid=%u; eventid=%u; ret=%d)\n",
            event_info->pid, event_info->gid, event_info->event_id, ret);
        return ret;
    }

    sqe->kernel_credit = TOPIC_SCHED_SQE_KERNEL_CREDIT;
    sqe->sqe_len = 0;

    ret = esched_drv_fill_task_msg(chip_id, event_src, (void *)&sqe->user_data[0], event_info);
    if (ret != 0) {
        return ret;
    }

    sqe->subtopic_id = event_info->subevent_id;
    sqe->topic_id = event_info->event_id;
    sqe->gid = event_info->gid;
    sqe->user_data_len = event_info->msg_len;

    ret = esched_drv_fill_task_pid(chip_id, sqe->topic_type, event_info->pid, &(sqe->pid));
    if (ret != 0) {
        return ret;
    }

    sched_debug("Fill sqe event info. (pid=%d; gid=%u; eventid=%u; dst_engine=%d; policy=%u; topic_type=%u)\n",
        event_info->pid, event_info->gid, event_info->event_id, event_info->dst_engine, event_info->policy,
        sqe->topic_type);
    return 0;
}

int esched_drv_fill_split_task(u32 chip_id, u32 event_src, struct sched_published_event_info *event_info,
    void *split_task)
{
    struct topic_sched_mailbox *mb = (struct topic_sched_mailbox *)split_task;
    int ret;

    mb->vfid = (u8)esched_get_hw_vfid_from_devid(chip_id);
    mb->blk_dim = 1;
    mb->stream_id = 0;
    mb->task_id = 0;

    mb->kernel_type = TOPIC_SCHED_DEFAULT_KERNEL_TYPE;
    mb->batch_mode = 0;
    mb->topic_type = esched_drv_get_topic_type(event_info->policy, event_info->dst_engine);

    ret = esched_drv_fill_task_pid(chip_id, mb->topic_type, event_info->pid, &(mb->pid));
    if (ret != 0) {
        return ret;
    }

    ret = esched_drv_fill_task_msg(chip_id, event_src, (void *)&mb->user_data[0], event_info);
    if (ret != 0) {
        return ret;
    }

    mb->subtopic_id = event_info->subevent_id;
    mb->topic_id = event_info->event_id;
    mb->gid = event_info->gid;
    mb->user_data_len = event_info->msg_len;

    sched_debug("Fill mailbox event info. (pid=%d; gid=%u; eventid=%u; dst_engine=%d; policy=%u)\n",
        event_info->pid, event_info->gid, event_info->event_id, event_info->dst_engine, event_info->policy);
    return 0;
}

int esched_restore_mb_user_data(struct topic_sched_mailbox *mb, u32 *dst_devid, u32 *tid, u32 *pid)
{
    return 0;
}

int esched_drv_map_host_dev_pid(struct sched_proc_ctx *proc_ctx, u32 identity)
{
#ifndef CFG_ENV_HOST
    u32 dev_id, vfid;
    int ret;

    if (proc_ctx->host_pid != 0) {
        return 0;
    }

    ret = hal_kernel_devdrv_query_process_host_pid(proc_ctx->pid, &dev_id, &vfid, (u32 *)&proc_ctx->host_pid, &proc_ctx->cp_type);
    if (ret != 0) {
        sched_warn("Invoke the hal_kernel_devdrv_query_process_host_pid not success. (pid=%d)\n", proc_ctx->pid);
        proc_ctx->host_pid = 0;
        return ret;
    }

    sched_info("Show details. (pid=%d; dev_id=%u; vfid=%u; cp_type=%d; host_pid=%d)\n",
               proc_ctx->pid, dev_id, vfid, (int)(proc_ctx->cp_type), proc_ctx->host_pid);
#endif
    return 0;
}

void esched_drv_unmap_host_dev_pid(struct sched_proc_ctx *proc_ctx, u32 identity)
{
    return;
}

int esched_convert_src_pid_to_cp_pid(struct topic_sched_mailbox *mb, u32 devid, int cp_type)
{
    int ret;
    u32 host_pid = mb->pid;
    u32 cp_pid;

    ret = devdrv_query_process_by_host_pid(host_pid, devid, cp_type, 0, &cp_pid);
    if (ret != 0) {
        return ret;
    }
    mb->pid = cp_pid;
    return 0;
}

int esched_get_real_pid(struct topic_sched_mailbox *mb, u32 devid, u32 pid)
{
    int ret;
    u32 host_pid;
#ifndef CFG_ENV_HOST
    int cp_type;
    u32 dst_pid;

    if (mb->topic_type == TOPIC_TYPE_AICPU_HOST_ONLY) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (mb->pid == 0) { /* Notice:only pid=0 return DRV_ERROR_PARA_ERROR */
        if (!esched_log_limited(SCHED_LOG_LIMIT_GET_REAL_PID)) {
            sched_warn("Recv a fak task or mb pid invalid. (dev_id=%u)\n", devid);
        }
        return DRV_ERROR_PARA_ERROR;
    }

    if ((mb->kernel_type == TOPIC_SCHED_CUSTOM_KERNEL_TYPE) || (mb->kernel_type == TOPIC_SCHED_CUSTOM_KFC_KERNEL_TYPE)) {
        cp_type = DEVDRV_PROCESS_CP2;
    } else {
        cp_type = DEVDRV_PROCESS_CP1;
    }

    if (mb->topic_type == TOPIC_TYPE_AICPU_DEVICE_SRC_PID) {
        ret = esched_convert_src_pid_to_cp_pid(mb, devid, cp_type);
        if (ret != 0) {
            if (!esched_log_limited(SCHED_LOG_LIMIT_GET_REAL_PID)) {
                sched_err("Failed to convert src_pid to cp pid. (dev_id=%u; mb->pid=%u; kernel_type=%u; ret=%d)\n",
                    devid, mb->pid, mb->kernel_type, ret);
            }
            return DRV_ERROR_NO_PROCESS;
        }
        return 0;
    }
    if (cp_type == DEVDRV_PROCESS_CP1) {
        sched_debug("CP1 sched(mb->pid=%u)\n", mb->pid);
        return 0;
    }

    /* query host pid by cp1. */
    ret = esched_drv_get_host_pid(devid, mb->pid, &host_pid, &cp_type);
    if (ret != 0) {
        if (!esched_log_limited(SCHED_LOG_LIMIT_GET_REAL_PID)) {
            sched_err("Failed to get host pid. (dev_id=%u; mb->pid=%u; vfid=%u; topic_id=%u; kernel_type=%u; ret=%d)\n",
                devid, mb->pid, mb->vfid + 1U, mb->topic_id, mb->kernel_type, ret);
        }
        return DRV_ERROR_NO_PROCESS;
    }

    /* query cp2 by host pid. */
    ret = devdrv_query_process_by_host_pid(host_pid, devid, DEVDRV_PROCESS_CP2, 0, &dst_pid);
    if (ret != 0) {
        if (!esched_log_limited(SCHED_LOG_LIMIT_GET_REAL_PID)) {
            sched_err("query cp2 failed. (dev_id=%u; vfid=%u; topic_id=%u; kernel_type=%u; cp1=%u; host_pid=%u; ret=%d)\n",
                devid, mb->vfid + 1U, mb->topic_id, mb->kernel_type, mb->pid, host_pid, ret);
        }
        return DRV_ERROR_NO_PROCESS;
    }

    sched_debug("Get CP2 pid(mb->pid=%u; host_pid=%u; cp2 pid=%u; cp_type=%d; kernel_type=%u)\n",
        mb->pid, host_pid, dst_pid, cp_type, mb->kernel_type);

    mb->pid = dst_pid;
    return DRV_ERROR_NONE;
#else
    ret = devdrv_query_master_pid_by_device_slave(devid, mb->pid, &host_pid);
    if (ret != 0) {
        if (!esched_log_limited(SCHED_LOG_LIMIT_GET_REAL_PID)) {
            sched_err("Failed to get host master pid. (mb->pid=%u; ret=%d)\n", mb->pid, ret);
        }
        return DRV_ERROR_NO_PROCESS;
    }

    mb->pid = host_pid;
    sched_debug("Get host slave pid(mb->pid=%u; master_pid=%u)\n", mb->pid, host_pid);
    return DRV_ERROR_NONE;
#endif
}

bool esched_drv_check_dst_is_support(u32 dst_engine)
{
    if ((dst_engine == DCPU_DEVICE) || (dst_engine == DVPP_CPU)) {
        return false;
    }
#ifndef CFG_ENV_HOST
    if (dst_engine == TS_CPU) {
        return false;
    }
#endif
#ifdef CFG_FEATURE_STARS_V2
    if ((dst_engine == CCPU_HOST) || (dst_engine == ACPU_HOST)) {
        return false;
    }
#endif
    return true;
}

int esched_drv_get_ccpu_flag(u32 dst_engine)
{
    if ((dst_engine != CCPU_DEVICE) && (dst_engine != CCPU_HOST) && (dst_engine != CCPU_LOCAL) && (dst_engine != TS_CPU)) {
        return SCHED_INVALID;
    }
    return SCHED_VALID;
}

/* split task mb_id_ptr is not right, and com cpu need to convert mb_id */
void esched_drv_flush_mb_mbid(u8 *mb_id_ptr, u8 mb_id)
{
    *mb_id_ptr = mb_id;
}
