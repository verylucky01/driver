/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <pthread.h>

#include "ascend_hal.h"
#include "pbl_uda_user.h"

#include "svm_log.h"
#include "svm_user_adapt.h"
#include "svm_sys_cmd.h"
#include "svm_register.h"
#include "svm_register_pcie_th.h"
#include "svm_prefetch.h"
#include "svm_vmm.h"
#include "malloc_mng.h"
#include "svm_dbi.h"
#include "va_allocator.h"
#include "svm_pipeline.h"
#include "svm_user_interface.h"

static int dev_status[SVM_MAX_DEV_NUM];
static int g_master_init_flag = 0;
static pthread_mutex_t master_mutex = PTHREAD_MUTEX_INITIALIZER;

static int svm_device_open_locked(u32 devid)
{
    int ret;

    if (dev_status[devid] != 0) {
        /* Support repeated open. */
        return DRV_ERROR_REPEATED_USERD;
    }

    ret = svm_ioctl_dev_init(devid);
    if (ret != 0) {
        return ret;
    }

    dev_status[devid] = 1;

    svm_info("Open success. (devid=%u)\n", devid);

    return DRV_ERROR_NONE;
}

static void svm_device_res_recycle(u32 devid)
{
    svm_register_recycle(devid);
    svm_register_pcie_th_recycle(devid);
    svm_prefetch_recycle(devid);
    vmm_recycle(devid);
    (void)svm_recycle_mem_by_dev(devid);
}

static int svm_device_close_locked(u32 devid)
{
    int ret;

    if (dev_status[devid] == 0) {
        /* Support repeated close. */
        return DRV_ERROR_REPEATED_USERD;
    }

    svm_occupy_pipeline();
    if (devid < SVM_MAX_AGENT_NUM) {
        svm_device_res_recycle(devid);
    }

    ret = svm_ioctl_dev_uninit(devid); /* Dev uninit pre handle will recycle sub module res.  */
    svm_release_pipeline();
    if (ret != 0) {
        return ret;
    }

    dev_status[devid] = 0;

    svm_info("Close success. (devid=%u)\n", devid);

    return DRV_ERROR_NONE;
}

static int svm_device_open(u32 devid)
{
    int ret;

    (void)pthread_mutex_lock(&master_mutex);
    ret = svm_device_open_locked(devid);
    (void)pthread_mutex_unlock(&master_mutex);

    return ret;
}

int svm_device_close(u32 devid)
{
    u32 udevid;
    int ret;

    if (devid >= SVM_MAX_AGENT_NUM) {
        svm_err("Invalid devid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    ret = uda_get_udevid_by_devid(devid, &udevid);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Get udevid failed. (devid=%u; ret=%d)\n", devid, ret);
        return DRV_ERROR_PARA_ERROR;
    }

    (void)pthread_mutex_lock(&master_mutex);
    ret = svm_device_close_locked(devid);
    (void)pthread_mutex_unlock(&master_mutex);

    return ret;
}

static int svm_master_init_locked(void)
{
    int ret;

    share_log_create(HAL_MODULE_TYPE_DEVMM, SHARE_LOG_MAX_SIZE);

    ret = svm_device_open_locked(svm_get_host_devid());
    if (ret != 0) {
        share_log_destroy(HAL_MODULE_TYPE_DEVMM);
        return ret;
    }
    return DRV_ERROR_NONE;
}

static void svm_master_uninit_locked(void)
{
    (void)svm_device_close_locked(svm_get_host_devid());
}

int svm_master_init(void)
{
    int ret = 0;

    if (g_master_init_flag == 1) {
        return DRV_ERROR_NONE;
    }

    (void)pthread_mutex_lock(&master_mutex);
    if (g_master_init_flag == 0) {
        ret = svm_master_init_locked();
        if (ret == 0) {
            g_master_init_flag = 1;
        }
    }
    (void)pthread_mutex_unlock(&master_mutex);

    return ret;
}

void svm_master_uninit(void)
{
    (void)pthread_mutex_lock(&master_mutex);
    if (g_master_init_flag == 1) {
        g_master_init_flag = 0;
        svm_master_uninit_locked();
    }
    (void)pthread_mutex_unlock(&master_mutex);
}

int svm_dev_open(uint32_t devid, int devfd)
{
    int ret = svm_master_init();
    SVM_UNUSED(devfd);

    if (ret != 0) {
        return ret;
    }
    return svm_device_open(devid);
}

int svm_dev_close(uint32_t devid)
{
    return svm_device_close(devid);
}

static bool devmm_agent_open_close_flag_is_valid(uint32_t flag)
{
    return (flag <= SVM_AGENT_DEVICE);
}

int drvMemDeviceOpen(uint32_t devid, int devfd)
{
    int ret;

    if (devid >= SVM_MAX_AGENT_NUM) {
        svm_err("Invalid devid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = svm_dev_open(devid, devfd);
    return ((ret == DRV_ERROR_REPEATED_USERD) ? DRV_ERROR_NONE : ret);
}

int drvMemDeviceClose(uint32_t devid)
{
    int ret;

    ret = svm_dev_close(devid);
    return ((ret == DRV_ERROR_REPEATED_USERD) ? DRV_ERROR_NONE : ret);
}

drvError_t halMemAgentOpen(uint32_t devid, uint32_t flag)
{
    int ret;

    if (devmm_agent_open_close_flag_is_valid(flag) == false) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (devid >= SVM_MAX_AGENT_NUM) {
        svm_err("Invalid devid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = halDrvEventThreadInit(devid);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Drv event thread init failed. (ret=%d; devid=%u)\n", ret, devid);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = svm_dev_open(devid, 0);
    return (drvError_t)((ret == DRV_ERROR_REPEATED_USERD) ? DRV_ERROR_NONE : ret);
}

drvError_t halMemAgentClose(uint32_t devid, uint32_t flag)
{
    int ret;

    if (devmm_agent_open_close_flag_is_valid(flag) == false) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = svm_dev_close(devid);
    if (ret != DRV_ERROR_NONE) {
        return (drvError_t)((ret == DRV_ERROR_REPEATED_USERD) ? DRV_ERROR_NONE : ret);
    }

    return halDrvEventThreadUninit(devid);
}

drvError_t drvMemDeviceOpenInner(uint32_t devid, halDevOpenIn *in, halDevOpenOut *out)
{
    SVM_UNUSED(in);
    SVM_UNUSED(out);

    return halMemAgentOpen(devid, SVM_AGENT_DEVICE);
}

drvError_t drvMemDeviceCloseInner(uint32_t devid, halDevCloseIn *in)
{
    SVM_UNUSED(in);

    return halMemAgentClose(devid, SVM_AGENT_DEVICE);
}

drvError_t drvMemDeviceCloseUserRes(uint32_t devid, halDevCloseIn *in)
{
    SVM_UNUSED(devid);
    SVM_UNUSED(in);

    return DRV_ERROR_NOT_SUPPORT;
}

drvError_t drvMemProcResBackup(halProcResBackupInfo *info)
{
    SVM_UNUSED(info);

    return DRV_ERROR_NOT_SUPPORT;
}

drvError_t drvMemProcResRestore(halProcResRestoreInfo *info)
{
    SVM_UNUSED(info);

    return DRV_ERROR_NOT_SUPPORT;
}

/* to-do: adapt later */
DVresult halSdmaCopy(DVdeviceptr dst, size_t dst_size, DVdeviceptr src, size_t len)
{
    SVM_UNUSED(dst);
    SVM_UNUSED(dst_size);
    SVM_UNUSED(src);
    SVM_UNUSED(len);

    return DRV_ERROR_NOT_SUPPORT;
}

DVresult halMemcpy3D(void *p_copy)
{
    SVM_UNUSED(p_copy);

    return DRV_ERROR_NOT_SUPPORT;
}

/* Sunset interface: external statement has been removed from ascend_hal.h. */
DVresult drvLoadProgram(DVdevice device_id, void *program, unsigned int offset, size_t byte_count, void **v_ptr)
{
    SVM_UNUSED(device_id);
    SVM_UNUSED(program);
    SVM_UNUSED(offset);
    SVM_UNUSED(byte_count);
    SVM_UNUSED(v_ptr);

    return DRV_ERROR_NOT_SUPPORT;
}

DVresult drvMemAddressTranslate(DVdeviceptr vptr, UINT64 *pptr)
{
    SVM_UNUSED(vptr);
    SVM_UNUSED(pptr);

    return DRV_ERROR_NOT_SUPPORT;
}

/* Sunset interface: external statement has been removed from ascend_hal.h. */
DVresult drvMemAllocL2buffAddr(DVdevice device, void **l2buff, UINT64 *pte)
{
    SVM_UNUSED(device);
    SVM_UNUSED(l2buff);
    SVM_UNUSED(pte);

    return DRV_ERROR_NOT_SUPPORT;
}

/* Sunset interface: external statement has been removed from ascend_hal.h. */
DVresult drvMemReleaseL2buffAddr(DVdevice device, void *l2buff)
{
    SVM_UNUSED(device);
    SVM_UNUSED(l2buff);

    return DRV_ERROR_NOT_SUPPORT;
}

/* Sunset interface: external statement has been removed from ascend_hal.h. */
DVresult drvMbindHbm(DVdeviceptr devPtr, size_t len, uint32_t type, uint32_t dev_id)
{
    SVM_UNUSED(devPtr);
    SVM_UNUSED(len);
    SVM_UNUSED(type);
    SVM_UNUSED(dev_id);

    return DRV_ERROR_NONE;
}
