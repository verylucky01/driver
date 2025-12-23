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

#include "esched_adapt.h"
#include "dms/dms_devdrv_manager_comm.h"
#include "pbl/pbl_runenv_config.h"
#include "esched_ioctl.h"
#include "esched_fops.h"
#if !defined CFG_ENV_HOST && defined CFG_FEATURE_HARDWARE_SCHED
#include "esched_drv_adapt_common.h"
#include "esched_drv_adapt.h"
#endif

#ifdef CFG_SOC_PLATFORM_CLOUD_V2
#include "ts_drv_init.h"
#endif

int32_t sched_fop_query_sync_msg_trace(u32 devid, unsigned long arg)
{
#ifndef EMU_ST
    int32_t ret;
    struct sched_ioctl_para_trace para_info = {{0}};
    struct sched_trace_input *para = &para_info.input;

    if (copy_from_user_safe(para, (void *)(uintptr_t)arg, sizeof(struct sched_trace_input)) != 0) {
        return DRV_ERROR_COPY_USER_FAIL;
    }

    ret = sched_query_sync_event_trace(sched_ioctl_devid(devid, para->dev_id), para->dev_pid, para->gid, para->tid, &para_info.trace);
    if (unlikely(ret != 0)) {
        return ret;
    }

    if (copy_to_user_safe((void *)((uintptr_t)arg + sizeof(struct sched_trace_input)),
        &para_info.trace, sizeof(struct sched_sync_event_trace)) != 0) {
        return DRV_ERROR_COPY_USER_FAIL;
    }
#endif
    return 0;
}

int sched_check_cp2_dest_pid(struct sched_published_event_info *event_info)
{
#ifndef EMU_ST
#ifndef CFG_ENV_HOST
    int ret;
    int cust_mode = DBL_CUST_OP_ENHANCE_DISABLE;
    int pid = current->tgid;
    u32 dev_id, vfid, host_pid;
    enum devdrv_process_type cp_type;

    ret = hal_kernel_devdrv_query_process_host_pid(pid, &dev_id, &vfid, &host_pid, &cp_type);
    if (ret != 0) {
        return 0;
    }
    (void)dbl_get_cust_op_enhance_mode(dev_id, &cust_mode);
    if ((cp_type == DEVDRV_PROCESS_CP2)
        && (event_info->pid != pid)
        && (cust_mode == DBL_CUST_OP_ENHANCE_DISABLE)) {
        sched_warn("Custom process is not allowed. (pid=%u; cp2_pid=%d; custom_mode=%u)\n", event_info->pid, pid, cust_mode);
        return DRV_ERROR_INNER_ERR;
    }
#endif
#endif
    return 0;
}

STATIC int32_t esched_get_trace_index(struct sched_event *event,
    u32 *gid, u32 *event_id, u32 *subevent_id)
{
    struct event_sync_msg *sync_msg = NULL;

    if (event->event_id == EVENT_DRV_MSG) {
        if (event->msg_len < sizeof(struct event_sync_msg)) {
            return DRV_ERROR_PARA_ERROR;
        }
        sync_msg = (struct event_sync_msg *)event->msg;
        *gid = sync_msg->gid;
        *event_id = sync_msg->event_id;
        *subevent_id = sync_msg->subevent_id;
    } else {
        *gid = event->gid;
        *event_id = event->event_id;
        *subevent_id = event->subevent_id;
    }
    return 0;
}

void esched_wait_trace_update(struct sched_proc_ctx *proc_ctx, struct sched_event *event)
{
    int32_t ret;
    int dfx_gid, dfx_tid;
    u32 gid, event_id, subevent_id;

    if ((proc_ctx== NULL) || (event == NULL)) {
        sched_warn("wait event abnormal.\n");
        return;
    }
    ret = esched_get_trace_index(event, &gid, &event_id, &subevent_id);
    if (ret != 0) {
        return;
    }

    dfx_gid = gid - SCHED_MAX_DEFAULT_GRP_NUM;
    dfx_tid = event_id - SCHED_SYNC_START_EVENT_ID;
    if ((dfx_gid < 0) || (dfx_gid >= SCHED_MAX_EX_GRP_NUM) ||
        (dfx_tid < 0) || (dfx_tid >= SCHED_MAX_SYNC_THREAD_NUM_PER_GRP)) {
        return;
    }

#ifdef CFG_ENV_HOST
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].src_wait_start_timestamp = event->timestamp.subscribe_in_kernel;
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].src_wait_end_timestamp = event->timestamp.subscribe_out_kernel;
#else
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].dst_wait_start_timestamp = event->timestamp.subscribe_in_kernel;
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].dst_wait_end_timestamp = event->timestamp.subscribe_out_kernel;
#endif
    return;
}


void esched_publish_trace_update(struct sched_proc_ctx *proc_ctx, struct sched_event *event)
{
    int32_t ret;
    int dfx_gid, dfx_tid;
    u32 gid, event_id, subevent_id;

    if ((proc_ctx== NULL) || (event == NULL)) {
        sched_warn("wait event abnormal.\n");
        return;
    }
    ret = esched_get_trace_index(event, &gid, &event_id, &subevent_id);
    if (ret != 0) {
        return;
    }

    dfx_gid = gid - SCHED_MAX_DEFAULT_GRP_NUM;
    dfx_tid = event_id - SCHED_SYNC_START_EVENT_ID;
    if ((dfx_gid < 0) || (dfx_gid >= SCHED_MAX_EX_GRP_NUM) ||
        (dfx_tid < 0) || (dfx_tid >= SCHED_MAX_SYNC_THREAD_NUM_PER_GRP)) {
        return;
    }
#ifdef CFG_ENV_HOST
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].src_publish_timestamp = sched_get_cur_timestamp();
#else
    (void)memset_s(&proc_ctx->sched_dfx[dfx_gid][dfx_tid],
        sizeof(struct sched_sync_event_trace), 0, sizeof(struct sched_sync_event_trace));
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].dst_publish_timestamp = sched_get_cur_timestamp();
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].gid = gid;
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].event_id = event_id;
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].subevent_id = subevent_id;
#endif
    return;
}

void esched_submit_trace_update(u32 event_src,
    struct sched_numa_node *node, struct sched_published_event_info *event_info)
{
    int dfx_gid, dfx_tid;
    u32 gid, event_id, subevent_id;
    int pid = current->tgid;
    struct sched_proc_ctx *proc_ctx = NULL;
    struct event_sync_msg *sync_msg = NULL;

    if (event_info->event_id == EVENT_DRV_MSG) {
        if (event_info->msg_len < sizeof(struct event_sync_msg)) {
            return;
        }
        sync_msg = (struct event_sync_msg *)event_info->msg;
        gid = sync_msg->gid;
        event_id = sync_msg->event_id;
        subevent_id = sync_msg->subevent_id;
    } else {
        gid = event_info->gid;
        event_id = event_info->event_id;
        subevent_id = event_info->subevent_id;
    }

    dfx_gid = gid - SCHED_MAX_DEFAULT_GRP_NUM;
    dfx_tid = event_id - SCHED_SYNC_START_EVENT_ID;
    if ((dfx_gid < 0) || (dfx_gid >= SCHED_MAX_EX_GRP_NUM) ||
        (dfx_tid < 0) || (dfx_tid >= SCHED_MAX_SYNC_THREAD_NUM_PER_GRP)) {
        return;
    }

    proc_ctx = esched_proc_get(node, pid);
    if (proc_ctx == NULL) {
        return;
    }

#ifdef CFG_ENV_HOST
    (void)memset_s(&proc_ctx->sched_dfx[dfx_gid][dfx_tid],
        sizeof(struct sched_sync_event_trace), 0, sizeof(struct sched_sync_event_trace));
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].gid = gid;
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].event_id = event_id;
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].subevent_id = subevent_id;
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].src_submit_user_timestamp = event_info->publish_timestamp;
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].src_submit_kernel_timestamp = sched_get_cur_timestamp();
#else
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].dst_submit_user_timestamp = event_info->publish_timestamp;
    proc_ctx->sched_dfx[dfx_gid][dfx_tid].dst_submit_kernel_timestamp = sched_get_cur_timestamp();
#endif
    esched_proc_put(proc_ctx);

    return;
}

STATIC void sched_free_thread_map(struct kref *kref)
{
    struct sched_event_thread_map *thread_map = container_of(kref, struct sched_event_thread_map, ref);

    sched_kfree(thread_map);
}

void sched_get_thread_map(struct sched_event_thread_map *thread_map)
{
    kref_get(&thread_map->ref);
}

void sched_put_thread_map(struct sched_event_thread_map *thread_map)
{
    kref_put(&thread_map->ref, sched_free_thread_map);
}

int sched_event_add_thread(struct sched_event *event, u32 tid)
{
    event->event_thread_map = sched_kzalloc(sizeof(struct sched_event_thread_map) + sizeof(u32),
        GFP_ATOMIC | __GFP_ACCOUNT);
    if (event->event_thread_map == NULL) {
        sched_err("Failed to kzalloc memory for event_thread_map and thread. (size=0x%lx)\n",
                  sizeof(struct sched_event_thread_map) + sizeof(u32));
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    kref_init(&event->event_thread_map->ref);

    event->event_thread_map->thread = (u32 *)(event->event_thread_map + 1);

    event->event_thread_map->thread[0] = tid;
    event->event_thread_map->thread_num = 1;

    return 0;
}

int32_t sched_get_firt_ctrlcpu(void)
{
    return 0;
}

int32_t sched_node_sched_cpu_uda_init(u32 dev_id)
{
    return 0;
}

int32_t sched_node_sched_cpu_module_init(u32 dev_id)
{
    return 0;
}

bool esched_is_support_uda_online()
{
    return true;
}

int esched_pm_shutdown(u32 chip_id)
{
    return 0;
}
EXPORT_SYMBOL_GPL(esched_pm_shutdown);

int esched_ts_platform_init(void)
{
#ifdef CFG_SOC_PLATFORM_CLOUD_V2
    return ts_drv_platform_init();
#else
    return 0;
#endif
}

void esched_ts_platform_uninit(void)
{
#ifdef CFG_SOC_PLATFORM_CLOUD_V2
    return ts_drv_platform_exit();
#else
    return;
#endif
}

#if defined CFG_FEATURE_HARDWARE_SCHED && !defined CFG_ENV_HOST
int esched_drv_init_cpu_port_adapt(u32 chip_id, u32 start_id, u32 chan_num)
{
    return esched_drv_init_all_cpu_port(chip_id, start_id, chan_num);
}
void esched_drv_uninit_cpu_port_adapt(u32 chip_id, u32 start_id, u32 chan_num)
{
    return esched_drv_uninit_all_cpu_port(chip_id, start_id, chan_num);
}

int esched_drv_init_msgq_config_adapt(struct sched_numa_node *node, u32 start_id, u32 aicpu_chan_num, u32 comcpu_chan_num)
{
    return esched_drv_init_msgq_config(node, start_id, aicpu_chan_num, comcpu_chan_num);
}
#endif