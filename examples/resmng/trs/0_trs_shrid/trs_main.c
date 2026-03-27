/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <securec.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <ascend_hal.h>

#include "utils.h"

static int name_fd[2];
static int pid_fd[2];

#define DEVDRV_NAME_LENTH (65)

int sub_get_hccl_type()
{
    return SHR_ID_NOTIFY_TYPE;
}

int sub_get_hccl_alloc_type(uint32_t hccl_type)
{
    if (hccl_type == SHR_ID_EVENT_TYPE) {
        return DRV_EVENT_ID;
    } else {
        return DRV_NOTIFY_ID;
    }
}

// tsdrv open
drvError_t hlt_drvDeviceOpen(void **devInfo, uint32_t devId)
{
    return drvDeviceOpen(devInfo, devId);
}

int devdrv_ChildProcessRet()
{
    int child_ret = 0;
    pid_t pid1;

    pid1 = wait(&child_ret);
    LOG_ERR("wait process's pid=%d,status=0x%d,exit value=%d(0x%d)\n", pid1, child_ret, WEXITSTATUS(child_ret),
           WEXITSTATUS(child_ret));

    // 获取子进程状态码的高8位，如果为0表示子进程return 0,pass
    if (WEXITSTATUS(child_ret) != 0) {
        return -1;
    } else {
        return 0;
    }
}

int sub_get_tgid()
{
    return getpid();
}

struct halResourceIdOutputInfo *sub_getResourceId(unsigned int dev_id, unsigned int ts_id, drvIdType_t type)
{
    drvError_t ret = 0;
    struct halResourceIdInputInfo in = {0};
    struct halResourceIdOutputInfo *out = malloc(sizeof(struct halResourceIdOutputInfo));
    in.type = type;
    in.tsId = ts_id;
    ret = halResourceIdAlloc(dev_id, &in, out);
    if (ret != DRV_ERROR_NONE) {
        LOG_ERR("[%s, %d] halResourceIdAlloc Fail!!! devid:%d, tsid:%d, ret:%d\n", __func__, __LINE__, dev_id, ts_id,
               ret);
        free(out);
        return NULL;
    }
    return out;
}

int sub_freeResourceId(unsigned int dev_id, unsigned int ts_id, drvIdType_t type,
                       struct halResourceIdOutputInfo *resourceinfo)
{
    drvError_t ret = 0;
    struct halResourceIdInputInfo in = {0};
    in.type = type;
    in.tsId = ts_id;
    in.resourceId = resourceinfo->resourceId;

    ret = halResourceIdFree(dev_id, &in);
    if (ret != DRV_ERROR_NONE) {
        LOG_ERR("[%s, %d] halResourceIdFree Fail!!! devid:%d, tsid:%d, ret:%d type:%d\n", __func__, __LINE__, dev_id, ts_id,
               ret, type);
        return -1;
    }
    free(resourceinfo);
    return 0;
}

void test_sleep(unsigned int s)
{
    unsigned int uicnt;
    LOG_INFO("pid(%d) start to sleep for %d s...\n\r", getpid(), s);

    for (uicnt = 0; uicnt < s; uicnt++) {
        printf("%d ", uicnt);
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
    return;
}

/* IPCNotify 全流程长时间测试 */
int test_shr_IPCNotify_all_func(uint32_t devid)
{
    char name[DEVDRV_NAME_LENTH] = {0};
    pid_t pid;
    int child_ret = 0;
    pid_t pid1;
    pid_t pid_array[1] = {0};
    struct drvShrIdInfo ipcnotifyinfo = {0};
    int tsId = 0;
    uint32_t id_type = 0;
    uint32_t alloc_id_type = 0;

    id_type = sub_get_hccl_type();
    alloc_id_type = sub_get_hccl_alloc_type(id_type);
    LOG_INFO("Used shr type is %d and used id type is %d.\n", id_type, alloc_id_type);
    if (pipe(name_fd) < 0) {
        LOG_ERR("[%s, %d] Fail to Create Pipe1\n", __func__, __LINE__);
    }
    if (pipe(pid_fd) < 0) {
        LOG_ERR("[%s, %d] Fail to Create Pipe2\n", __func__, __LINE__);
    }
    pid = fork();

    if (pid < 0) {
        LOG_ERR("err:%s, %d\n\r", __func__, __LINE__);
        return -1;
    } else if (pid > 0) {
        // Main process
        void *info;
        drvError_t ret = 0;
        struct halResourceIdOutputInfo *notifyid = NULL;

        ret = hlt_drvDeviceOpen(&info, devid);
        if (ret != DRV_ERROR_NONE) {
            LOG_ERR("[main process] test_fail hlt_drvDeviceOpen ret:%d, devid:%d \n", ret, devid);
            devdrv_ChildProcessRet();
            return -1;
        }

        notifyid = sub_getResourceId(devid, tsId, alloc_id_type);
        if (notifyid == NULL) {
            LOG_ERR("[main process] test_fail drvNotifyIdAlloc ret:%d, devid:%d \n", ret, devid);
            (void)drvDeviceClose(devid);
            devdrv_ChildProcessRet();
            return -1;
        }

        ipcnotifyinfo.devid = devid;
        ipcnotifyinfo.shrid = notifyid->resourceId;
        ipcnotifyinfo.tsid = tsId;
        ipcnotifyinfo.id_type = id_type;
        ret = halShrIdCreate(&ipcnotifyinfo, name, DEVDRV_NAME_LENTH);
        if (ret != DRV_ERROR_NONE) {
            LOG_ERR("[main process] test_fail halShrIdCreate ret:%d, devid:%d \n", ret, devid);
            (void)sub_freeResourceId(devid, tsId, alloc_id_type, notifyid);
            (void)drvDeviceClose(devid);
            devdrv_ChildProcessRet();
            return -1;
        }
        LOG_INFO("[main process] halShrIdCreate done, devid:%d, Notify_id:%d, ret:%d, pid=%d\n",
            devid, notifyid->resourceId, ret, getpid());
        LOG_INFO("[main process] name : %s\n", name);
        close(name_fd[0]);
        write(name_fd[1], name, sizeof(name));

        close(pid_fd[1]);
        read(pid_fd[0], &pid, sizeof(pid));
        pid_array[0] = pid;
        ret = halShrIdSetPid(name, pid_array, 1);
        LOG_INFO("[debug dev:%d] halShrIdSetPid pid:%d\n", devid, pid_array[0]);
        if (ret != DRV_ERROR_NONE) {
            LOG_ERR("[main process] test_fail halShrIdSetPid ret:%d, devid:%d \n", ret, devid);
            (void)sub_freeResourceId(devid, tsId, alloc_id_type, notifyid);
            (void)drvDeviceClose(devid);
            devdrv_ChildProcessRet();
            return -1;
        }
        test_sleep(10);

        ret = halShrIdDestroy(name);
        if (ret != DRV_ERROR_NONE) {
            LOG_ERR("[main process]test_fail drvNotifyIdAlloc ret:%d, devid:%d \n", ret, devid);
            (void)sub_freeResourceId(devid, tsId, alloc_id_type, notifyid);
            (void)drvDeviceClose(devid);
            devdrv_ChildProcessRet();
            return -1;
        }

        ret = sub_freeResourceId(devid, tsId, alloc_id_type, notifyid);
        if (ret != DRV_ERROR_NONE) {
            LOG_ERR("[main process]test_fail drvNotifyIdFree ret:%d, devid:%d \n", ret, devid);
            (void)drvDeviceClose(devid);
            devdrv_ChildProcessRet();
            return -1;
        }

        ret = drvDeviceClose(devid);
        if (ret != DRV_ERROR_NONE) {
            LOG_ERR("[main process] test_fail drvDeviceClose ret:%d, devid:%d \n", ret, devid);
            devdrv_ChildProcessRet();
            return -1;
        }

        pid1 = wait(&child_ret);
        LOG_INFO("wait process's pid=%d,status=0x%d,exit value=%d(0x%d)\n", pid1, child_ret, WEXITSTATUS(child_ret),
               WEXITSTATUS(child_ret));

        // 获取子进程状态码的高8位，如果为0表示子进程return 0,pass
        if (WEXITSTATUS(child_ret) != 0) {
            return -1;
        }

    } else {
        drvError_t ret = 0;

        pid = sub_get_tgid();
        close(pid_fd[0]);
        write(pid_fd[1], &pid, sizeof(pid));

        test_sleep(5);

        // Child process
        close(name_fd[1]);
        read(name_fd[0], name, sizeof(name));
        LOG_INFO("[follow process] name : %s\n", name);
        ipcnotifyinfo.devid = devid;
        ipcnotifyinfo.tsid = tsId;
        ipcnotifyinfo.id_type = id_type;
        ret = halShrIdOpen(name, &ipcnotifyinfo);
        if (ret != DRV_ERROR_NONE) {
            LOG_ERR("[follow process]test_fail halShrIdOpen ret:%d, devid:%d pid:%d\n", ret, devid, getpid());
            exit(-1);
        }
        LOG_INFO("[follow process]test_success halShrIdOpen recv_id:%d, Notify_id:%d, ret:%d, "
               "pid=%d \n", ipcnotifyinfo.devid, ipcnotifyinfo.shrid, ret, getpid());

        ret = halShrIdClose(name);
        if (ret != DRV_ERROR_NONE) {
            LOG_ERR("[follow process]test_fail halShrIdClose ret:%d, devid:%d \n", ret, devid);
            exit(-1);
        }
        LOG_INFO("[follow process]test_success halShrIdClose recv_id:%d, Notify_id:%d, ret:%d, "
               "pid=%d \n", ipcnotifyinfo.devid, ipcnotifyinfo.shrid, ret, getpid());
        exit(0);
    }

    return 0;
}

int main(int argc, char **argv)
{
    int ret = 0;
    uint32_t i = 0;
    uint32_t num;
    
    // 获取device数量
    ret = drvGetDevNum(&num);
    if (ret != DRV_ERROR_NONE) {
        LOG_ERR("[%s, %d]halGetDevNumEx Fail!!! ret:%d\n", __func__, __LINE__, ret);
        ret = -1;
        goto L_tsdrv_main_exit;
    }
    for (i = 0; i < num; i++) {
        ret = test_shr_IPCNotify_all_func(i);
        if (ret != 0) {
            LOG_ERR("[testcase [test_shr_IPCNotify_all_func] devid:%d test_fail, value = -1 \n", i);
            ret = -1;
            goto L_tsdrv_main_exit;
        }
    }
L_tsdrv_main_exit:
    if (ret == 0) {
        LOG_INFO("test_success, value = 0 \n");
    } else {
        LOG_ERR("test_fail, value = %d \n", ret);
    }
    return ret;
}
