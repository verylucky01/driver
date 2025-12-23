/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <string.h>

#include "securec.h"
#include "ascend_hal.h"
#include "ascend_hal_define.h"

#include "svm_ioctl.h"
#include "devmm_svm.h"
#include "svm_user_msg.h"

#define DEVMM_SYNC_TIMEOUT 30000 // 30 s

static int g_svm_devpid[DEVMM_MAX_PHY_DEVICE_NUM][DEVDRV_PROCESS_CPTYPE_MAX] = {0};

static pid_t devmm_get_devpid(uint32_t devid, enum devdrv_process_type proc_type)
{
    /* DEVDRV_PROCESS_CP2 may exit, so query every time */
    if ((g_svm_devpid[devid][proc_type] == 0) || (proc_type == DEVDRV_PROCESS_CP2)) {
        struct halQueryDevpidInfo hostpidinfo = {0};
        drvError_t ret;
        int devpid;

        hostpidinfo.proc_type = proc_type;
        hostpidinfo.hostpid = getpid();
        hostpidinfo.devid = devid;
        hostpidinfo.vfid = 0;
        ret = halQueryDevpid(hostpidinfo, &devpid);
        if (ret == 0) {
            /* if cp2 not exsit, query will return cp1 pid */
            if ((proc_type == DEVDRV_PROCESS_CP2) && (devpid == g_svm_devpid[devid][DEVDRV_PROCESS_CP1])) {
                return 0;
            }
            g_svm_devpid[devid][proc_type] = devpid;
            DEVMM_DRV_INFO("Query devpid succ. (devId=%u; hostpid=%d; proc_type=%u; devpid=%d).\n",
                devid, hostpidinfo.hostpid, hostpidinfo.proc_type, devpid);
        }
    }

    return g_svm_devpid[devid][proc_type];
}

void devmm_reset_event_devpid(uint32_t devid)
{
    if (devid < DEVMM_MAX_PHY_DEVICE_NUM) {
        enum devdrv_process_type proc_type;
        for (proc_type = DEVDRV_PROCESS_CP1; proc_type < DEVDRV_PROCESS_CPTYPE_MAX; proc_type++) {
            g_svm_devpid[devid][proc_type] = 0;
        }
    }
}

static drvError_t devmm_get_event_grp_id(uint32_t devid, int pid, uint32_t *grp_id)
{
    struct esched_query_gid_output gid_out = {0};
    struct esched_query_gid_input gid_in = {0};
    struct esched_output_info out_put = {0};
    struct esched_input_info in_put = {0};
    size_t grp_name_len;
    drvError_t ret;

    gid_in.pid = pid;
    grp_name_len = strlen(EVENT_DRV_MSG_GRP_NAME);
    (void)memcpy_s(gid_in.grp_name, EVENT_MAX_GRP_NAME_LEN, EVENT_DRV_MSG_GRP_NAME, grp_name_len);
    in_put.inBuff = &gid_in;
    in_put.inLen = sizeof(struct esched_query_gid_input);
    out_put.outBuff = &gid_out;
    out_put.outLen = sizeof(struct esched_query_gid_output);
    ret = halEschedQueryInfo(devid, QUERY_TYPE_REMOTE_GRP_ID, &in_put, &out_put);
    if (ret == DRV_ERROR_NONE) {
        *grp_id = gid_out.grp_id;
        return DRV_ERROR_NONE;
    } else if (ret == DRV_ERROR_UNINIT) {
        *grp_id = 0; // PROXY_HOST_GRP_NAME grp not exist, use default grpid 0.
        return DRV_ERROR_NONE;
    }

    DEVMM_DRV_ERR("Query grpid failed. (ret=%d; devid=%u; devpid=%d).\n", ret, devid, gid_in.pid);
    return ret;
}

static drvError_t devmm_fill_event(struct event_summary *event_info, uint32_t devid, enum devdrv_process_type proc_type,
    uint32_t subevent_id, const char *msg, uint32_t msg_len)
{
    struct event_sync_msg *msg_head = NULL;
    drvError_t ret;
    int pid;

    pid = (int)devmm_get_devpid(devid, proc_type);
    if (pid == 0) {
        return DRV_ERROR_NO_PROCESS;
    }

    ret = devmm_get_event_grp_id(devid, pid, &event_info->grp_id);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }
    event_info->dst_engine = CCPU_DEVICE;
    event_info->policy = ONLY;
    event_info->event_id = EVENT_DRV_MSG;
    event_info->subevent_id = subevent_id;
    event_info->pid = pid;
    event_info->msg_len = (uint32_t)sizeof(struct event_sync_msg) + msg_len;
    event_info->msg = malloc(event_info->msg_len);
    if (event_info->msg == NULL) {
        DEVMM_DRV_ERR("Malloc failed. (size=%u)\n", event_info->msg_len);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    msg_head = (struct event_sync_msg *)event_info->msg;
    msg_head->dst_engine = CCPU_HOST;

    if (msg_len == 0) {
        return DRV_ERROR_NONE;
    }

    ret = (drvError_t)memcpy_s(event_info->msg + sizeof(struct event_sync_msg), msg_len, msg, msg_len);
    if (ret != DRV_ERROR_NONE) {
#ifndef EMU_ST
        DEVMM_DRV_ERR("Memcpy_s failed. (ret=%d)\n", ret);
        free(event_info->msg);
        event_info->msg = NULL;
        return DRV_ERROR_PARA_ERROR;
#endif
    }

    return DRV_ERROR_NONE;
}

static void devmm_clear_event(struct event_summary *event_info)
{
    if (event_info->msg != NULL) {
        free(event_info->msg);
        event_info->msg = NULL;
    }
}

static drvError_t devmm_send_event_sync(uint32_t devid, enum devdrv_process_type proc_type,
    struct devmm_event_msg_info *msg_info)
{
    struct event_summary event_info;
    struct event_reply reply;
    drvError_t ret;

    if (devid >= DEVMM_MAX_PHY_DEVICE_NUM) {
        DEVMM_DRV_ERR("Input devId is invalid. (devId=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    ret = devmm_fill_event(&event_info, devid, proc_type, msg_info->subevent_id, msg_info->msg, msg_info->msg_len);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NO_PROCESS), "FillEvent failed. (ret=%d)\n", ret);
        return ret;
    }
    reply.buf_len = sizeof(struct event_proc_result);
    reply.buf = (char *)&msg_info->result;

    ret = halEschedSubmitEventSync(devid, &event_info, DEVMM_SYNC_TIMEOUT, &reply);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("HalEschedSubmitEventSync failed. (ret=%d)\n", ret);
        devmm_clear_event(&event_info);
        return ret;
    }
    devmm_clear_event(&event_info);

    if (reply.reply_len != reply.buf_len) {
        DEVMM_DRV_ERR("Len is not equal. (reply_len=%u; buf_len=%u)\n", reply.reply_len, reply.buf_len);
        return DRV_ERROR_PARA_ERROR;
    }

    return (drvError_t)msg_info->result.ret;
}

drvError_t devmm_process_task_mmap(uint32_t devid, enum devdrv_process_type proc_type,
    DVdeviceptr *va, size_t size, int fixed_va_flag)
{
    struct devmm_process_cp_mmap mmap_info;
    struct devmm_event_msg_info msg_info;
    drvError_t ret;

    (void)fixed_va_flag;
    
    mmap_info.va = *va;
    mmap_info.size = size;

    msg_info.msg = (char *)&mmap_info;
    msg_info.msg_len = sizeof(struct devmm_process_cp_mmap);
    msg_info.subevent_id = DRV_SUBEVENT_SVM_PROCESS_CP_MMAP_MSG;

    ret = devmm_send_event_sync(devid, proc_type, &msg_info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = (drvError_t)memcpy_s((void *)va, sizeof(DVdeviceptr),
        msg_info.result.data, sizeof(DVdeviceptr));
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Memcpy_s failed. (devId=%u; ret=%d)\n", devid, ret);
        return DRV_ERROR_INNER_ERR;
    }

    return DRV_ERROR_NONE;
}

drvError_t devmm_process_cp_mmap(uint32_t devid, DVdeviceptr *va, size_t size)
{
    return devmm_process_task_mmap(devid, DEVDRV_PROCESS_CP1, va, size, 0);
}

drvError_t devmm_process_task_munmap(uint32_t devid, enum devdrv_process_type proc_type, DVdeviceptr va, size_t size)
{
    struct devmm_process_cp_munmap munmap_info;
    struct devmm_event_msg_info msg_info;
    drvError_t ret;

    munmap_info.va = va;
    munmap_info.size = size;

    msg_info.msg = (char *)&munmap_info;
    msg_info.msg_len = sizeof(struct devmm_process_cp_munmap);
    msg_info.subevent_id = DRV_SUBEVENT_SVM_PROCESS_CP_MUNMAP_MSG;

    ret = devmm_send_event_sync(devid, proc_type, &msg_info);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NO_PROCESS), "Va mmap failed. (devId=%u; vptr=0x%llx;)\n", devid, va);
        return ret;
    }

    return DRV_ERROR_NONE;
}

drvError_t devmm_process_cp_munmap(uint32_t devid, DVdeviceptr va)
{
    return devmm_process_task_munmap(devid, DEVDRV_PROCESS_CP1, va, 1);
}

