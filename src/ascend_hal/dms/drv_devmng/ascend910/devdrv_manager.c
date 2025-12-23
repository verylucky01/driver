/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <unistd.h>
#include <fcntl.h>
#include "securec.h"
#include "mmpa_api.h"
#include "dpa/dpa_apm.h"
#include "devmng_common.h"
#include "devdrv_ioctl.h"
#include "dms/dms_devdrv_info_comm.h"
#include "devdrv_pcie.h"
#ifdef CFG_FEATURE_SUPPORT_DEVMNG_BBOX
#include "devmng_user.h"
#endif
#include "ascend_inpackage_hal.h"
#include "dmc_user_interface.h"
#include "davinci_interface.h"
#include "dsmi_common_interface.h"
#include "devmng_user_common.h"
#include "dms/dms_misc_interface.h"
#include "devdrv_container.h"
#include "ascend_hal.h"
#include "dms_cmd_def.h"
#include "drv_user_common.h"
#include "dms_device_info.h"
#include "dms_user_common.h"
#include "ascend_dev_num.h"
#include "dms/dms_qos_interface.h"
#include "drv_devmng_adapt.h"
#include "hbm_ctrl.h"

#ifndef __linux
    #pragma comment(lib, "libc_sec.lib")
    #include "devdrv_manager_win.h"
    #define PTHREAD_MUTEX_INITIALIZER NULL
    #define DEVDRV_BB_DEVICE_ID_INFORM 0x66020004
    #define DEVDRV_BB_DEVICE_STATE_INFORM 0x66020008
    #define fd_is_invalid(fd) (fd == (mmProcess)DEVDRV_INVALID_FD_OR_INDEX)
#else

    #include <sys/prctl.h>
    #include <errno.h>
    #include <stdio.h>
    #include <stdarg.h>
    #include <syslog.h>
    #include <sys/types.h>
    #include <poll.h>
    #include <stdlib.h>
    #include <sys/ioctl.h>

    #include "devdrv_user_common.h"
    #include "dms/dms_drv_internal.h"
    #define fd_is_invalid(fd) ((fd) < 0)
    #define DAVINCI_COMMON_DRV_NAME "asdrv_pbl"
    #define DAVINCI_COMMON_VDRV_NAME "asdrv_vpbl"
    int devdrv_do_container(int devmng_fd);
#endif

#ifdef STATIC_SKIP
#define STATIC
#else
#define STATIC static
#endif

STATIC mmProcess devdrv_manager_fd = (mmProcess)DEVDRV_INVALID_FD_OR_INDEX;
STATIC mmMutex_t devdrv_manager_fd_mutex = PTHREAD_MUTEX_INITIALIZER;
STATIC mmUserBlock_t func_block = {0};

#define DEVDRV_INVALID_TGID (-1)
STATIC pid_t devdrv_manager_tgid = (pid_t)DEVDRV_INVALID_TGID;

#define DAVINCI_INTF_MODULE_DEVMNG "DEVMNG"
#define DAVINCI_INTF_INVALID_STATUS (-1)
#define PCIE_NUM (3)
#define INVALID_VF_DEVICE_ID 0xFFFFFFFF

struct devdrv_bbox_share_mem_para {
    u32 devid;
    u32 size;
    void *dst_buffer;
};

STATIC mmMutex_t devdrv_black_box_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    drvDeviceExceptionReporFunc exp_report_func;
    drvDeviceStartupNotify start_notifunc;
#ifdef __linux
    drvDeviceStateNotify state_notifunc;
#endif
} drvBlackBoxCallbackFunc_t;

STATIC drvBlackBoxCallbackFunc_t BBoxCallbackFunc = {
    .exp_report_func = NULL,
    .start_notifunc = NULL,
#ifdef __linux
    .state_notifunc = NULL,
#endif
};

typedef struct tag_drv_query_device_info_cmd_st {
    DSMI_MAIN_CMD main_cmd_type;
    unsigned int sub_cmd_type;
    drvError_t (*callback)(unsigned int dev_id, unsigned int main_cmd,
        unsigned int sub_cmd, void *buf, unsigned int *size);
} drv_query_device_info_cmd;

typedef struct tag_drv_set_device_info_cmd_st {
    DSMI_MAIN_CMD main_cmd_type;
    unsigned int sub_cmd_type;
    drvError_t (*callback)(unsigned int dev_id, unsigned int sub_cmd,
        const void *buf, unsigned int size);
} drv_set_device_info_cmd;

STATIC mmThread devdrv_black_box_thread;
STATIC u8 devdrv_black_box_thread_work = 0;

STATIC int devmng_ioctl_open(int fd)
{
    struct davinci_intf_close_arg arg = {0};
    int status = false;
    int ret;

    ret = drvGetCommonDriverInitStatus(&status);
    if (ret != 0) {
        DEVDRV_DRV_ERR("drvGetCommonDriverInitStatus failed, ret(%d).\n", ret);
        return ret;
    }

    if (status == false) {
        return 0;
    }

    ret = strcpy_s(arg.module_name, DAVINIC_MODULE_NAME_MAX, DAVINCI_INTF_MODULE_DEVMNG);
    if (ret < 0) {
        DEVDRV_DRV_ERR("strcpy_s failed, ret(%d).\n", ret);
        return ret;
    }

    ret = ioctl(fd, DAVINCI_INTF_IOCTL_OPEN, &arg);
    if (ret != 0) {
        DEVDRV_DRV_ERR("call devdrv_device ioctl open failed, ret(%d).\n", ret);
        return ret;
    }

    return 0;
}

STATIC void devmng_ioctl_close(int fd)
{
    struct davinci_intf_close_arg arg = {0};
    int status = false;
    int ret;

    ret = drvGetCommonDriverInitStatus(&status);
    if (ret != 0) {
        return;
    }

    if (status == false) {
        return;
    }

    ret = strcpy_s(arg.module_name, DAVINIC_MODULE_NAME_MAX, DAVINCI_INTF_MODULE_DEVMNG);
    if (ret < 0) {
        return;
    }

    (void)ioctl(fd, DAVINCI_INTF_IOCTL_CLOSE, &arg);
}

mmProcess devdrv_open_device_manager(void)
{
    mmProcess fd = -1;
    int err = 0;
#ifdef __linux
    int ret, ret_do_container;
#endif

    /* to improve performance */
    if (!fd_is_invalid(devdrv_manager_fd)) {
        if (devdrv_manager_tgid == getpid()) {
            return devdrv_manager_fd;
        }
    }

#ifndef __linux
    if (devdrv_manager_fd_mutex == NULL) {
        mmMutexInit(&devdrv_manager_fd_mutex);
    }
#endif

    (void)mmMutexLock(&devdrv_manager_fd_mutex);

    if (!fd_is_invalid(devdrv_manager_fd)) {
        if (devdrv_manager_tgid != getpid()) {
            devdrv_manager_fd = (mmProcess)DEVDRV_INVALID_FD_OR_INDEX;
        } else {
            fd = devdrv_manager_fd;
            goto out;
        }
    }

#ifdef __linux
    fd = mmOpen2(davinci_intf_get_dev_path(), M_RDWR|O_CLOEXEC, M_IRUSR);
    err = (__errno_location() != NULL ? errno : 0);
    ret = devmng_ioctl_open(fd);
    dmanage_share_log_create();
#else
    fd = devdrv_open_device_manager_win(&devdrv_manager_fd);
#endif
    if (fd_is_invalid(fd)) {
        DEVDRV_DRV_ERR("Failed to Open device manager. (name=%s; ret_fd=%d; old_fd=%d; err=%d)\n",
                       davinci_intf_get_dev_path(), fd, devdrv_manager_fd, err);
        fd = (mmProcess)DEVDRV_INVALID_FD_OR_INDEX;
        dmanage_share_log_destroy();
        goto out;
    }
#ifdef __linux
    u32 dev_num = 0;
    ret_do_container = drvGetDevNum(&dev_num);
    if ((ret != 0) || (ret_do_container != 0)) {
        devmng_ioctl_close(fd);
        (void)mmClose(fd);
        DEVDRV_DRV_ERR("devdrv_do_container return error. (ret=%d, fd=%d)\n", ret_do_container, fd);
        fd = (mmProcess)DEVDRV_INVALID_FD_OR_INDEX;
        if (ret_do_container == DRV_ERROR_RESOURCE_OCCUPIED) {
            fd = (mmProcess)(-DRV_ERROR_RESOURCE_OCCUPIED);
        }
        dmanage_share_log_destroy();
        goto out;
    }
#endif
    devdrv_manager_fd = fd;
    devdrv_manager_tgid = getpid();
out:
    (void)mmMutexUnLock(&devdrv_manager_fd_mutex);
    return fd;
}

STATIC void devdrv_close_device_manager(void)
{
    (void)mmMutexLock(&devdrv_manager_fd_mutex);
    if (!fd_is_invalid(devdrv_manager_fd)) {
#if defined __linux
        if (devdrv_manager_tgid == getpid()) {
            devmng_ioctl_close(devdrv_manager_fd);
            (void)mmClose(devdrv_manager_fd);
            dmanage_share_log_destroy();
        }
#endif
        devdrv_manager_fd = (mmProcess)DEVDRV_INVALID_FD_OR_INDEX;
        devdrv_manager_tgid = (pid_t)DEVDRV_INVALID_TGID;
    }
    (void)mmMutexUnLock(&devdrv_manager_fd_mutex);
}

mmProcess devdrv_get_dev_manager_fd(void)
{
    mmProcess fd = -1;

    (void)mmMutexLock(&devdrv_manager_fd_mutex);

    fd = devdrv_manager_fd;

    (void)mmMutexUnLock(&devdrv_manager_fd_mutex);

    return fd;
}

void drv_ioctl_param_init(mmIoctlBuf *icotl_buf, void *inbuf, int inbufLen)
{
    icotl_buf->inbuf = inbuf;
    icotl_buf->inbufLen = inbufLen;
    icotl_buf->outbuf = icotl_buf->inbuf;
    icotl_buf->outbufLen = icotl_buf->inbufLen;
    icotl_buf->oa = NULL;
}

int drv_common_ioctl(mmIoctlBuf *icotl_buf, int cmd)
{
    int ret;
    int err_buf;
    mmProcess fd;

    fd = devdrv_open_device_manager();
    if ((int)fd == -DRV_ERROR_RESOURCE_OCCUPIED) {
        DEVDRV_DRV_ERR("open device manager failed, device is busy.\n");
        return DRV_ERROR_RESOURCE_OCCUPIED;
    }
    if (fd_is_invalid(fd)) {
        DEVDRV_DRV_ERR("open device manager failed, fd = %d.\n", fd);
        return DRV_ERROR_INVALID_HANDLE;
    }

    ret = dmanage_mmIoctl(fd, cmd, icotl_buf);
    if (ret == 0) {
        return 0;
    }
    /* to avoid errno be changed by print */
    err_buf = errno;
#ifndef DEVDRV_UT
    if (err_buf == EOPNOTSUPP || err_buf == EAGAIN) {
        DEVDRV_DRV_WARN("cmd = %d, ret = %d\n", cmd, ret);
        ret = errno_to_user_errno(err_buf);
    } else {
        DEVDRV_DRV_ERR_EXTEND(err_buf, EBUSY, "Ioctl info. (cmd=%d; ret=%d)\n", cmd, err_buf);
        ret = errno_to_user_errno(err_buf);
    }
#endif
    return ret;
}

drvError_t drvDeviceStatus(uint32_t devId, drvStatus_t *status)
{
#ifdef CFG_FEATURE_TRS_HB_REFACTOR
    unsigned int dev_status = DRV_STATUS_RESERVED;
    int ret;

    if (devId >= ASCEND_DEV_MAX_NUM || status == NULL) {
        DEVDRV_DRV_ERR("Parameter is invalid. (devId=%u; status_is_null=%d)\n", devId, (status == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = DmsDeviceInitStatus(devId, &dev_status);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Get device init status failed. (devId=%u, ret=%d)\n", devId, ret);
        return ret;
    }

    if (dev_status == DRV_STATUS_WORK) {
        /* DC has one Ts module, tsid = 0. */
        ret = DmsTsHeartbeatStatus(devId, 0, 0, &dev_status);
        if (ret != 0) {
            DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Get ts hb status failed. (devId=%u, ret=%d)\n", devId, ret);
            return ret;
        }
    }

    if (dev_status >= DRV_STATUS_RESERVED) {
        DEVDRV_DRV_ERR("Invalid status. (devId=%u, dev_status=%d)\n", devId, dev_status);
        return DRV_ERROR_INVALID_VALUE;
    }

    *status = (drvStatus_t)dev_status;
    return DRV_ERROR_NONE;

#else
    int para;
    int ret;

    mmIoctlBuf buf = {0};
    if (devId >= ASCEND_DEV_MAX_NUM || status == NULL) {
        DEVDRV_DRV_ERR("Parameter is invalid. (devId=%u; status_is_null=%d)\n", devId, (status == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    *status = DRV_STATUS_RESERVED;
    para = (int)devId;
    drv_ioctl_param_init(&buf, (void *)&para, sizeof(int));

    ret = drv_common_ioctl(&buf, DEVDRV_MANAGER_DEVICE_STATUS);
    if (ret != 0) {
        DEVDRV_DRV_ERR("ioctl failed devId = %u, ret = %d.\n", devId, ret);
        return ret;
    }

    if (para < 0 || para >= DRV_STATUS_RESERVED) {
        DEVDRV_DRV_ERR("invalid status(%d).\n", para);
        return DRV_ERROR_INVALID_VALUE;
    }
    *status = (drvStatus_t)para;

    return DRV_ERROR_NONE;
#endif
}

drvError_t drvGetDeviceSpec(uint32_t devId, drvSpec_t *drvspec)
{
    struct devdrv_hardware_inuse inuse;
    struct devdrv_hardware_spec spec;
    int ret;
    mmIoctlBuf spec_buf = {0};
    mmIoctlBuf inuse_buf = {0};

    if (devId >= ASCEND_DEV_MAX_NUM || drvspec == NULL) {
        DEVDRV_DRV_ERR("invalid devid = %u, (drvspec == NULL) = %d.\n", devId, (drvspec == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = memset_s((void *)drvspec, sizeof(drvSpec_t), 0, sizeof(drvSpec_t));
    if (ret != EOK) {
        DEVDRV_DRV_ERR("memset_s failed, ret = %d. devid = %u\n", ret, devId);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    spec.devid = (u32)devId;
    drv_ioctl_param_init(&spec_buf, (void *)&spec, sizeof(struct devdrv_hardware_spec));

    ret = drv_common_ioctl(&spec_buf, DEVDRV_MANAGER_GET_CORE_SPEC);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "ioctl failed devId = %u, ret = %d.\n", devId, ret);
        return ret;
    }

    inuse.devid = (u32)devId;
    drv_ioctl_param_init(&inuse_buf, (void *)&inuse, sizeof(struct devdrv_hardware_inuse));

    ret = drv_common_ioctl(&inuse_buf, DEVDRV_MANAGER_GET_CORE_INUSE);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "ioctl failed devId(%u), ret = %d.\n", devId, ret);
        return ret;
    }

    drvspec->aiCoreNum = spec.ai_core_num;
    drvspec->aiCpuNum = spec.ai_cpu_num;
    drvspec->aiCoreNumLevel = spec.ai_core_num_level;
    drvspec->aiCoreFreqLevel = spec.ai_core_freq_level;
    drvspec->aiCoreInuse = inuse.ai_core_num;
    drvspec->aiCoreErrorMap = inuse.ai_core_error_bitmap;
    drvspec->aiCpuInuse = inuse.ai_cpu_num;
    drvspec->aiCpuErrorMap = inuse.ai_cpu_error_bitmap;

    return DRV_ERROR_NONE;
}

drvError_t drvDeviceGetPcieInfo(uint32_t devId, int32_t *bus, int32_t *dev, int32_t *func)
{
    struct devdrv_pci_info devdrv_pci_info;
    mmIoctlBuf dev_info_buf = {0};
    mmIoctlBuf para_buf = {0};
    uint32_t para;
    int ret;

    if (devId >= ASCEND_DEV_MAX_NUM || bus == NULL || dev == NULL || func == NULL) {
        DEVDRV_DRV_ERR("Parameter is invalid. (devid=%u; bus_is_null=%d; dev_is_null=%d; func_is_null=%d)\n",
            devId, (bus == NULL), (dev == NULL), (func == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    drv_ioctl_param_init(&para_buf, (void *)&para, sizeof(uint32_t));
    ret = drv_common_ioctl(&para_buf, DEVDRV_MANAGER_GET_PLATINFO);
    if (ret != 0) {
        DEVDRV_DRV_ERR("ioctl failed devId = %u, ret = %d.\n", devId, ret);
        return ret;
    }

    if (para == DEVDRV_MANAGER_DEVICE_ENV) {
        DEVDRV_DRV_ERR("offline env, no pcie info. devid = %u\n", devId);
        return DRV_ERROR_INVALID_VALUE;
    }

    devdrv_pci_info.dev_id = devId;
    drv_ioctl_param_init(&dev_info_buf, (void *)&devdrv_pci_info, sizeof(struct devdrv_pci_info));
    ret = drv_common_ioctl(&dev_info_buf, DEVDRV_MANAGER_GET_PCIINFO);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "ioctl failed devId = %u, ret = %d.\n", devId, ret);
        return ret != DRV_ERROR_BUSY ? ret : DRV_ERROR_RESOURCE_OCCUPIED;
    }

    *bus = devdrv_pci_info.bus_number;
    *dev = devdrv_pci_info.dev_number;
    *func = devdrv_pci_info.function_number;

    return DRV_ERROR_NONE;
}

drvError_t drvDeviceGetPcieIdInfo(uint32_t devId, struct tag_pcie_idinfo *pcie_idinfo)
{
#ifdef CFG_FEATURE_GET_PCIE_ID_INFO
    dms_pcie_id_info_t id_info = {0};
    int ret;

    if ((devId >= ASCEND_DEV_MAX_NUM) || (pcie_idinfo == NULL)) {
        DEVDRV_DRV_ERR("invalid devid(%u) or pcie_idinfo is NULL.\n", devId);
        return DRV_ERROR_INVALID_VALUE;
    }
    ret = DmsGetPcieIdInfo(devId, &id_info);
    if (ret != 0) {
        DEVDRV_DRV_ERR("dms get pcie id info failed. (devId=%u; ret=%d)", devId, ret);
        return ret;
    }

    pcie_idinfo->venderid = id_info.venderid;
    pcie_idinfo->subvenderid = id_info.subvenderid;
    pcie_idinfo->subdeviceid = id_info.subdeviceid;
    pcie_idinfo->deviceid = id_info.deviceid;
    pcie_idinfo->bdf_busid = id_info.bus;
    pcie_idinfo->bdf_deviceid = id_info.device;
    pcie_idinfo->bdf_funcid = id_info.fn;

    return DRV_ERROR_NONE;
#else
    (void)devId;
    (void)pcie_idinfo;
    return DRV_ERROR_NOT_SUPPORT;
#endif
}

drvError_t vdevmng_get_dev_probe_num(uint32_t *num)
{
    uint32_t dev_num = 0;
    int ret;

    if (num == NULL) {
        DEVDRV_DRV_ERR("Num is null.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }

    ret = dmanage_common_ioctl(DEVDRV_MANAGER_GET_PROBE_NUM, &dev_num);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "dmanage_common_ioctl failed. (ret=%d)\n", ret);
        return ret;
    }

    *num = dev_num;
    return DRV_ERROR_NONE;
}

drvError_t drvGetDevProbeNum(uint32_t *num)
{
    if (DmsGetVirtFlag()) {
        return vdevmng_get_dev_probe_num(num);
    } else {
        return DmsGetDevProbeNum(num);
    }
}

drvError_t halGetDevProbeList(uint32_t *devices, uint32_t len)
{
    return DmsGetDevProbeList(devices, len);
}

STATIC int devdrv_check_vdevid(unsigned int devId)
{
#ifdef CFG_FEATURE_SRIOV
    int ret;
    uint32_t i;
    uint32_t vdev_num = 0;
    uint32_t *vdevids = NULL;

    ret = halGetVdevNum(&vdev_num);
    if (ret != 0) {
        DEVDRV_DRV_ERR("Get vf device num failed. (vdev_num=%u; ret=%d)\n", vdev_num, ret);
        return ret;
    }
    if (vdev_num == 0) {
        DEVDRV_DRV_ERR("Invalid device id. (devId=%d; vdev_num=%u)\n", devId, vdev_num);
        return DRV_ERROR_INVALID_DEVICE;
    }

    vdevids = (uint32_t *)malloc(sizeof(uint32_t) * vdev_num);
    if (vdevids == NULL) {
        DEVDRV_DRV_ERR("malloc failed. (devId=%d)\n", devId);
        return DRV_ERROR_MALLOC_FAIL;
    }
    ret = memset_s(vdevids, vdev_num * sizeof(uint32_t), INVALID_VF_DEVICE_ID, vdev_num * sizeof(uint32_t));
    if (ret != 0) {
        free(vdevids);
        vdevids = NULL;
        DEVDRV_DRV_ERR("vdevids memset fail. (dev_id=%u; ret=%d)\n", devId, ret);
        return ret;
    }

    ret = halGetVdevIDs(vdevids, vdev_num);
    if (ret != 0) {
        free(vdevids);
        vdevids = NULL;
        DEVDRV_DRV_ERR("Get vf device id failed. (vdev_num=%u; ret=%d)\n", vdev_num, ret);
        return ret;
    }
    for (i = 0; i < vdev_num; i++) {
        if (devId == vdevids[i]) {
            free(vdevids);
            vdevids = NULL;
            return DRV_ERROR_NONE;
        }
    }

    free(vdevids);
    vdevids = NULL;
#endif
    DEVDRV_DRV_ERR("Invalid device id. (devId=%d)\n", devId);
    return DRV_ERROR_INVALID_DEVICE;
}

drvError_t drvCheckDevid(unsigned int devId)
{
    int ret;
    unsigned int num_dev = 0, i;
    unsigned int devids[ASCEND_PDEV_MAX_NUM];

    if (devId >= ASCEND_PDEV_MAX_NUM) {
        return devdrv_check_vdevid(devId);
    }

    ret = drvGetDevNum(&num_dev);
    if (ret != 0 || num_dev > ASCEND_PDEV_MAX_NUM) {
        DEVDRV_DRV_ERR("Get device num failed. (num_dev=%u; ret=%d)\n", num_dev, ret);
        return ret;
    }
#ifndef DEVDRV_UT
    ret = drvGetDevIDs(devids, num_dev);
    if (ret != 0) {
        DEVDRV_DRV_ERR("Get device list failed. (num_dev=%u; ret=%d)\n", num_dev, ret);
        return ret;
    }
    for (i = 0; i < num_dev; i++) {
        if (devids[i] == devId) {
            return DRV_ERROR_NONE;
        }
    }
    DEVDRV_DRV_ERR("Invalid device id. (devId=%d; num_dev=%u)\n", devId, num_dev);
#endif
    return DRV_ERROR_INVALID_DEVICE;
}

drvError_t drv_get_container_dev_ids(uint32_t *devices, uint32_t len, uint32_t *num)
{
    struct devdrv_manager_devids dev_info = {0};
    mmIoctlBuf dev_info_buf = {0};
    uint32_t min_len;
    uint32_t i;
    int ret;

    if (devices == NULL || len > ASCEND_DEV_MAX_NUM || num == NULL) {
        DEVDRV_DRV_ERR("Parameter is invalid. (len=%u; devices_is_null=%d; num_is_null=%d)\n",
            len, (devices == NULL), (num == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    drv_ioctl_param_init(&dev_info_buf, (void *)&dev_info, sizeof(struct devdrv_manager_devids));
    ret = drv_common_ioctl(&dev_info_buf, DEVDRV_MANAGER_GET_CONTAINER_DEVIDS);
    if (ret != 0) {
        DEVDRV_DRV_ERR("ioctl failed ret = %d.\n", ret);
        return ret;
    }

    if (dev_info.num_dev > ASCEND_DEV_MAX_NUM) {
        DEVDRV_DRV_ERR("wrong num_dev(%u)\n", dev_info.num_dev);
        return DRV_ERROR_INVALID_DEVICE;
    }

    min_len = dev_info.num_dev > len ? len : dev_info.num_dev;
    for (i = 0; i < min_len; i++) {
        devices[i] = dev_info.devids[i];
    }

    *num = min_len;

    return DRV_ERROR_NONE;
}

drvError_t drvGetDevInfo(uint32_t devId, struct devdrv_device_info *info)
{
    struct devdrv_manager_hccl_devinfo dev_info = {0};
    mmIoctlBuf dev_info_buf = {0};
    int ret;

    if (devId >= ASCEND_DEV_MAX_NUM || info == NULL) {
        DEVDRV_DRV_ERR("Parameter is invalid. (devId=%u; info_is_null=%d)\n", devId, (info == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    dev_info.dev_id = devId;
    drv_ioctl_param_init(&dev_info_buf, (void *)&dev_info, sizeof(struct devdrv_manager_hccl_devinfo));
    ret = drv_common_ioctl(&dev_info_buf, DEVDRV_MANAGER_GET_DEVINFO);
    if (ret != 0) {
        if (ret == DRV_ERROR_RESOURCE_OCCUPIED || ret == DRV_ERROR_BUSY) {
            DEVDRV_DRV_ERR_EXTEND(ret, DRV_ERROR_BUSY, "The device is busy. (ret=%d)\n", ret);
            return DRV_ERROR_RESOURCE_OCCUPIED;
        } else {
            DEVDRV_DRV_ERR("Ioctl failed. (ret=%d)\n", ret);
            return ret;
        }
    }

    info->ai_core_num = dev_info.ai_core_num;
    info->ai_core_freq = dev_info.aicore_freq;
    info->ai_cpu_core_num = dev_info.ai_cpu_core_num;
    info->ctrl_cpu_ip = dev_info.ctrl_cpu_ip;
    info->ctrl_cpu_id = dev_info.ctrl_cpu_id;
    info->ctrl_cpu_core_num = dev_info.ctrl_cpu_core_num;
    info->ctrl_cpu_occupy_bitmap = dev_info.ctrl_cpu_occupy_bitmap;
    info->ctrl_cpu_endian_little = dev_info.ctrl_cpu_endian_little;
    info->ts_cpu_core_num = dev_info.ts_cpu_core_num;
    info->env_type = dev_info.env_type;
    info->aicpu_occupy_bitmap = dev_info.ai_cpu_bitmap;
    info->ai_core_id = dev_info.ai_core_id;
    info->ai_cpu_core_id = dev_info.ai_cpu_core_id;
    info->hardware_version = dev_info.hardware_version;
    info->ts_num = dev_info.ts_num;
    info->cpu_system_count = dev_info.cpu_system_count;
    info->monotonic_raw_time_ns = dev_info.monotonic_raw_time_ns;
    info->chip_id = dev_info.chip_id;
    info->die_id = dev_info.die_id;
    info->vector_core_num = dev_info.vector_core_num;
    info->vector_core_freq = dev_info.vector_core_freq;
    info->addr_mode = dev_info.addr_mode;
    info->mainboard_id = dev_info.mainboard_id;
    info->product_type = dev_info.product_type;
	info->host_device_connect_type = dev_info.host_device_connect_type;
    info->aicore_bitmap[0] = dev_info.aicore_bitmap[0];
    info->aicore_bitmap[1] = dev_info.aicore_bitmap[1];
    ret = strcpy_s(info->soc_version, SOC_VERSION_LEN, dev_info.soc_version);
    if (ret != 0) {
        return ret;
    }
    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_core_utilization(uint32_t dev_id, struct devdrv_core_utilization *utilization_info)
{
    struct devdrv_core_utilization util_info = {0};
    int ret;

#ifdef DRV_HOST
    mmIoctlBuf util_info_buf = {0};
#endif

    if (utilization_info == NULL || utilization_info->core_type >= DEV_DRV_TYPE_MAX) {
        DEVDRV_DRV_ERR("Parameter is invalid. (dev_id=%u)\n", dev_id);
        return DRV_ERROR_INVALID_VALUE;
    }
#ifdef DRV_HOST
    util_info.dev_id = dev_id;
    util_info.core_type = utilization_info->core_type;
    drv_ioctl_param_init(&util_info_buf, (void*)&util_info, sizeof(struct devdrv_core_utilization));
#ifndef CFG_FEATURE_AIC_AIV_UTIL_FROM_TS
    if (utilization_info->core_type == DEV_DRV_TYPE_AIVECTOR) {
        return DRV_ERROR_NOT_SUPPORT;
    }
#endif
    ret = drv_common_ioctl(&util_info_buf, DEVDRV_MANAGER_GET_CORE_UTILIZATION);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Ioctl failed. (dev_id=%u; ret=%d)\n", dev_id, ret);
        return ret;
    }
#else
    if (utilization_info->core_type == DEV_DRV_TYPE_AICPU) {
        ret = dms_get_aicpu_utilization(dev_id, &(util_info.utilization));
        if (ret != 0) {
            DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to get aicpu utilization. (dev_id=%u; ret=%d)\n",
                dev_id, ret);
            return ret;
        }
    } else if (utilization_info->core_type == DEV_DRV_TYPE_AICORE ||
        utilization_info->core_type == DEV_DRV_TYPE_AIVECTOR) {
#ifdef CFG_FEATURE_AIC_AIV_UTIL_FROM_TS
        ret = dms_get_average_util_from_ts(dev_id, 0, utilization_info->core_type, &(util_info.utilization));
#else
        if (utilization_info->core_type == DEV_DRV_TYPE_AICORE) {
            ret = DmsGetTsInfo(dev_id, 0, AICORE0_ID, &util_info.utilization, sizeof(unsigned int));
        } else {
            return DRV_ERROR_NOT_SUPPORT;
        }
#endif
        if (ret != 0) {
            DEVDRV_DRV_ERR("Failed to get utilization. (dev_id=%u; core_type=%u; ret=%d)\n",
                dev_id, utilization_info->core_type, ret);
            return ret;
        }
    }

#endif
    utilization_info->utilization = util_info.utilization;

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_h2d_dev_info(uint32_t devId, struct devdrv_device_info *info)
{
#ifdef CFG_FEATURE_DEVMNG_IOCTL
    mmIoctlBuf dev_info_buf = {0};
#else
    struct urd_cmd cmd = {0};
    struct urd_cmd_para cmd_para = {0};
#endif
    struct devdrv_manager_hccl_devinfo dev_info = {0};
    int ret, i;

    if (devId >= ASCEND_DEV_MAX_NUM || info == NULL) {
        DEVDRV_DRV_ERR("Parameter is invalid. (devId=%u; info_is_null=%d)\n", devId, (info == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

#ifdef CFG_FEATURE_DEVMNG_IOCTL
    dev_info.dev_id = devId;
    drv_ioctl_param_init(&dev_info_buf, (void *)&dev_info, sizeof(struct devdrv_manager_hccl_devinfo));
    ret = drv_common_ioctl(&dev_info_buf, DEVDRV_MANAGER_GET_H2D_DEVINFO);
    if (ret != 0) {
        DEVDRV_DRV_ERR("ioctl failed ret = %d.\n", ret);
        return ret;
    }
#else
    urd_usr_cmd_fill(&cmd, DMS_MAIN_CMD_BASIC, DMS_SUBCMD_GET_H2D_DEV_INFO, NULL, 0);
    urd_usr_cmd_para_fill(&cmd_para, (void *)&devId, sizeof(uint32_t),
        (void *)&dev_info, sizeof(struct devdrv_manager_hccl_devinfo));
    ret = urd_dev_usr_cmd(devId, &cmd, &cmd_para);
    if (ret != 0) {
        DMS_EX_NOTSUPPORT_ERR(ret, "Ioctl failed. (ret=%d; devId=%u)\n", ret, devId);
        return ret;
    }
#endif

    info->ai_core_num = dev_info.ai_core_num;
    info->ai_core_freq = dev_info.aicore_freq;
    info->ai_cpu_core_num = dev_info.ai_cpu_core_num;
    info->ctrl_cpu_ip = dev_info.ctrl_cpu_ip;
    info->ctrl_cpu_id = dev_info.ctrl_cpu_id;
    info->ctrl_cpu_core_num = dev_info.ctrl_cpu_core_num;
    info->ctrl_cpu_occupy_bitmap = dev_info.ctrl_cpu_occupy_bitmap;
    info->ctrl_cpu_endian_little = dev_info.ctrl_cpu_endian_little;
    info->ts_cpu_core_num = dev_info.ts_cpu_core_num;
    info->env_type = dev_info.env_type;
    info->aicpu_occupy_bitmap = dev_info.ai_cpu_bitmap;
    info->ai_core_id = dev_info.ai_core_id;
    info->ai_cpu_core_id = dev_info.ai_cpu_core_id;
    info->hardware_version = dev_info.hardware_version;
    info->ts_num = dev_info.ts_num;
    info->cpu_system_count = dev_info.cpu_system_count;
    info->monotonic_raw_time_ns = dev_info.monotonic_raw_time_ns;
    info->ffts_type = dev_info.ffts_type;
    for (i = 0; i < DEVDRV_MAX_COMPUTING_POWER_TYPE; i++) {
        info->computing_power[i] = dev_info.computing_power[i];
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_osc_freq(uint32_t devId, int32_t info_type, uint64_t *value)
{
#if defined CFG_FEATURE_OSC_FREQ && (defined (DRV_HOST) || defined (CFG_FEATURE_RC_MODE))
    int ret;
    unsigned int sub_cmd;
    int ioctl_cmd;
    struct dms_ioctl_arg ioarg = {0};
#ifndef CFG_FEATURE_SRIOV
    struct dms_osc_freq freq = {0};

    freq.dev_id = devId;
    freq.sub_cmd = (info_type == INFO_TYPE_HOST_OSC_FREQUE) ? DMS_SUBCMD_GET_HOST_OSC_FREQ : DMS_SUBCMD_GET_DEV_OSC_FREQ;
    if (DmsGetVirtFlag() != 0) {
        ret = dmanage_common_ioctl(DEVDRV_MANAGER_GET_OSC_FREQ, &freq);
        if (ret != 0) {
            if (ret != DRV_ERROR_NOT_SUPPORT) {
                DEVDRV_DRV_ERR("Ioctl failed. (dev_id=%u; ret=%d; errno=%d)\n", devId, ret, errno);
            }
        } else {
            *value = freq.value;
        }
        return ret;
    }
#endif

    sub_cmd = (info_type == INFO_TYPE_HOST_OSC_FREQUE) ? DMS_SUBCMD_GET_HOST_OSC_FREQ : DMS_SUBCMD_GET_DEV_OSC_FREQ;
    ioctl_cmd = (info_type == INFO_TYPE_HOST_OSC_FREQUE) ? DMS_GET_OSC_FREQ_INFO_HOST : DMS_GET_OSC_FREQ_INFO_DEVICE;
    ioarg.main_cmd = DMS_MAIN_CMD_BASIC;
    ioarg.sub_cmd = sub_cmd;
    ioarg.filter_len = 0;
    ioarg.filter = NULL;
    ioarg.input = (void *)&devId;
    ioarg.input_len = sizeof(int);
    ioarg.output = (void *)value;
    ioarg.output_len = sizeof(uint64_t);
    ret = DmsIoctl(ioctl_cmd, &ioarg);
    if (ret != 0) {
        if ((ret != EOPNOTSUPP) && ( ret != EAGAIN)) {
            DEVDRV_DRV_ERR("DmsIoctl failed. (devId=%u; ret=%d)\n", devId, ret);
        }
        return errno_to_user_errno(ret);
    }
    return DRV_ERROR_NONE;
#else
    (void)devId;
    (void)info_type;
    (void)value;
    return DRV_ERROR_NOT_SUPPORT;
#endif
}

drvError_t drv_get_info_from_dev_info(uint32_t devId, int32_t info_type, int64_t *value)
{
    int ret;
    struct devdrv_device_info info = { 0 };

    ret = drvGetDevInfo(devId, &info);
    if (ret != 0) {
        DEVDRV_DRV_ERR("Get device info failed. (dev_id=%u; infotype=%d; ret=%d)\n", devId, info_type, ret);
        return ret;
    }

    switch (info_type) {
        case INFO_TYPE_ENV:
            *value = info.env_type;
            break;

        case INFO_TYPE_VERSION:
            *value = info.hardware_version;
            break;

        case INFO_TYPE_CORE_NUM:
            *value = info.ts_num;
            break;

        case INFO_TYPE_PHY_CHIP_ID:
            *value = info.chip_id;
            break;

        case INFO_TYPE_PHY_DIE_ID:
            *value = info.die_id;
            break;

        case INFO_TYPE_ADDR_MODE:
            *value = info.addr_mode;
            break;

        case INFO_TYPE_MAINBOARD_ID:
            *value = info.mainboard_id;
            break;
        case INFO_TYPE_PRODUCT_TYPE:
            *value = info.product_type;
            break;
        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    return 0;
}

STATIC drvError_t drv_get_system_info(uint32_t devId, int32_t info_type, int64_t *value)
{
    int ret;
    uint64_t osc_freq = 0;
    unsigned int tmp = 0;
    unsigned int tmp_len = sizeof(tmp);
    struct devdrv_device_info info = {0};

    switch (info_type) {
        case INFO_TYPE_ENV:
        case INFO_TYPE_CORE_NUM:
        case INFO_TYPE_PHY_CHIP_ID:
        case INFO_TYPE_PHY_DIE_ID:
        case INFO_TYPE_ADDR_MODE:
            return drv_get_info_from_dev_info(devId, info_type, value);

        case INFO_TYPE_MAINBOARD_ID:
#ifdef CFG_FEATURE_HW_INFO_FROM_BIOS
            return drv_get_info_from_dev_info(devId, info_type, value);
#else
            return DRV_ERROR_NOT_SUPPORT;
#endif
        case INFO_TYPE_PRODUCT_TYPE:
#ifdef CFG_FEATURE_PRODUCT_TYPE
            return drv_get_info_from_dev_info(devId, info_type, value);
#else
            return DRV_ERROR_NOT_SUPPORT;
#endif
        case INFO_TYPE_VERSION:
            return drv_get_info_type_version_adapt(devId, info_type, value);

        case INFO_TYPE_MASTERID:
            ret = DmsGetMasterDevInTheSameOs(devId, &tmp);
            if (ret != 0) {
                DEVDRV_DRV_ERR("drvGetMasterDeviceInTheSameOS failed. (dev_id=%u; ret=%d)\n", devId, ret);
                return ret;
            }

            *value = tmp;
            break;

        case INFO_TYPE_SYS_COUNT:
        case INFO_TYPE_MONOTONIC_RAW:
            ret = drv_get_h2d_dev_info(devId, &info);
            if (ret != 0) {
                DEVDRV_DRV_ERR("drvGetDevInfo failed. (dev_id=%u; ret=%d)\n", devId, ret);
                return ret;
            }
            *value = (info_type == INFO_TYPE_SYS_COUNT ? info.cpu_system_count : info.monotonic_raw_time_ns);
            break;

        case INFO_TYPE_HOST_OSC_FREQUE:
        case INFO_TYPE_DEV_OSC_FREQUE:
            ret = drv_get_osc_freq(devId, info_type, &osc_freq);
            if (ret != 0) {
                return ret;
            }

            *value = osc_freq;
            break;
        case INFO_TYPE_SDID:
        case INFO_TYPE_SERVER_ID:
        case INFO_TYPE_SCALE_TYPE:
        case INFO_TYPE_SUPER_POD_ID:
            return dms_get_spod_item(devId, info_type, value);
        case INFO_TYPE_CHASSIS_ID:
#ifdef CFG_FEATURE_SUPPORT_CHASSIS_ID
            return dms_get_spod_item(devId, info_type, value);
#else
            return DRV_ERROR_NOT_SUPPORT;
#endif
        case INFO_TYPE_SUPER_POD_TYPE:
#ifdef CFG_FEATURE_SUPPORT_SPOD_TYPE
            return dms_get_spod_item(devId, info_type, value);
#else
            return DRV_ERROR_NOT_SUPPORT;
#endif
        case INFO_TYPE_RUN_MACH:
            ret = drvCheckDevid(devId);
            if (ret != 0) {
                DEVDRV_DRV_ERR("Check device id failed. (dev_id=%u; ret=%d)\n", devId, ret);
                return ret;
            }
#ifdef DRV_HOST
            ret = drvGetHostPhyMachFlag(devId, &tmp);
            if (ret != 0) {
                DEVDRV_DRV_ERR("Get host physical machine flag fail. (dev_id=%u; ret=%d).\n", devId, ret);
                return ret;
            }

            *value = (tmp == DEVDRV_HOST_PHY_MACH_FLAG) ? RUN_MACHINE_PHYCICAL : RUN_MACHINE_VIRTUAL;
#else
            *value = RUN_MACHINE_PHYCICAL;
#endif
            break;

        case INFO_TYPE_CUST_OP_ENHANCE:
            ret = DmsHalGetDeviceInfoEx(devId, MODULE_TYPE_SYSTEM, info_type, &tmp, &tmp_len);
            if ((ret != 0) || (tmp_len != sizeof(tmp))) {
                DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to get cust op enhance. (dev_id=%u; ret=%d; tmp_len=%u)\n",
                    devId, ret, tmp_len);
                return ret;
            }
            *value = tmp;

            break;

        case INFO_TYPE_HD_CONNECT_TYPE:
#ifdef CFG_FEATURE_HD_COMNNECT_TYPE
            ret = drvGetDevInfo(devId, &info);
            if (ret != 0) {
                DEVDRV_DRV_ERR("Get device info failed. (dev_id=%u; ret=%d)\n", devId, ret);
                return ret;
            }
            *value = info.host_device_connect_type;
            break;
#else
            return DRV_ERROR_NOT_SUPPORT;
#endif
        case INFO_TYPE_BOARD_ID:
            ret = DmsGetBoardId(devId, &tmp);
            if (ret != 0) {
                DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to board id. (dev_id=%u; ret=%d)\n", devId, ret);
                return ret;
            }
            *value = tmp;
            break;

        case INFO_TYPE_VNIC_IP:
            ret = devdrv_get_vnic_ip(devId, &tmp);
            if (ret != 0) {
                DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to vnic ip. (dev_id=%u; ret=%d)\n", devId, ret);
                return ret;
            }
            *value = tmp;
            break;

        case INFO_TYPE_SPOD_VNIC_IP:
            ret = devdrv_get_vnic_ip_by_sdid(devId, &tmp);
            if (ret != 0) {
                DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to vnic ip by sdid. (dev_id=%u; ret=%d)\n", devId, ret);
                return ret;
            }
            *value = tmp;
            break;

        default:
            DEVDRV_DRV_INFO("This version does not support this type. (dev_id=%u; Type=%d)\n", devId, info_type);
            return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_ai_cpu_num_and_bitmap(uint32_t devId, int32_t info_type, int64_t *query_info)
{
    int ret;
    unsigned int query_dev_id;
    struct devdrv_device_info dev_info = {0};

    if ((info_type == INFO_TYPE_PF_CORE_NUM) || (info_type == INFO_TYPE_PF_OCCUPY)) {
#if defined(DRV_HOST) || !defined(CFG_FEATURE_SRIOV)
        return DRV_ERROR_NOT_SUPPORT;
#else
        if (devId >= ASCEND_VDEV_ID_START) {
            /*
             * VF device id is start from ASCEND_VDEV_ID_START, each PF has VDAVINCI_MAX_VFID_NUM of VFs
             */
            query_dev_id = (devId - ASCEND_VDEV_ID_START) / VDAVINCI_MAX_VFID_NUM;
        } else {
            query_dev_id = devId;
        }
#endif
    } else {
        query_dev_id = devId;
    }

    ret = drvGetDevInfo(query_dev_id, &dev_info);
    if (ret != 0) {
        DEVDRV_DRV_ERR("Get device info failed. (ret=%d; dev_id=%u)\n", ret, devId);
        return ret;
    }

    if ((info_type == INFO_TYPE_PF_CORE_NUM) || (info_type == INFO_TYPE_CORE_NUM)) {
        *query_info = dev_info.ai_cpu_core_num;
    } else if ((info_type == INFO_TYPE_OCCUPY) || (info_type == INFO_TYPE_PF_OCCUPY)) {
        *query_info = dev_info.aicpu_occupy_bitmap;
    } else {
#ifdef CFG_FEATURE_AICPU_NOT_EXIST
        if (dev_info.ai_cpu_core_num == 0) {
            return DRV_ERROR_NOT_EXIST;
        }
#endif
        *query_info = dev_info.ai_cpu_core_id;
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_ai_cpu_info(uint32_t devId, int32_t info_type, int64_t *value)
{
    int ret;
    struct tagDrvSpec spc_info = {0};
    struct tagDrvCpuInfo cpu_info = {0};
    struct devdrv_core_utilization aicpu_util = {0};

    if ((info_type == INFO_TYPE_CORE_NUM) || (info_type == INFO_TYPE_PF_CORE_NUM) ||
        (info_type == INFO_TYPE_OCCUPY) || (info_type == INFO_TYPE_PF_OCCUPY) ||
        (info_type == INFO_TYPE_ID)) {
        ret = drv_get_ai_cpu_num_and_bitmap(devId, info_type, value);
        if (ret != 0) {
            DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret,
                "Get aicpu number or bitmap failed. (dev_id=%u; info_type=%d; ret=%d)\n",
                devId, info_type, ret);
            return ret;
        }
    } else if (info_type == INFO_TYPE_OS_SCHED) {
        ret = drvGetCpuInfo(devId, &cpu_info);
        if (ret != 0) {
            DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "drvGetCpuInfo failed ret = %d.\n", ret);
            return ret;
        }
        *value = cpu_info.aicpu_os_sched;
    } else if (info_type == INFO_TYPE_IN_USED) {
        ret = drvGetDeviceSpec(devId, &spc_info);
        if (ret != 0) {
            DEVDRV_DRV_ERR("drvGetDeviceSpec failed ret = %d.\n", ret);
            return ret;
        }
        *value = spc_info.aiCpuInuse;
    } else if (info_type == INFO_TYPE_ERROR_MAP) {
        ret = drvGetDeviceSpec(devId, &spc_info);
        if (ret != 0) {
            DEVDRV_DRV_ERR("drvGetDeviceSpec failed ret = %d.\n", ret);
            return ret;
        }
        *value = spc_info.aiCpuErrorMap;
    } else if (info_type == INFO_TYPE_UTILIZATION) {
        aicpu_util.core_type = DEV_DRV_TYPE_AICPU;
        ret = drv_get_core_utilization(devId, &aicpu_util);
        if (ret != 0) {
            DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to get aicpu utilization. (dev_id=%u; ret=%d)\n", devId, ret);
            return ret;
        }
        *value = (int64_t)aicpu_util.utilization;
    } else if (info_type == INFO_TYPE_WORK_MODE) {
        return DmsGetCpuWorkMode(devId, (long long *)value);
    } else {
        DEVDRV_DRV_INFO("This version does not support this type. (Type=%d)\n", info_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_ctrl_cpu_info(uint32_t devId, int32_t info_type, int64_t *value)
{
    int ret;
    struct devdrv_device_info dev_info = {0};
    struct tagDrvCpuInfo cpu_info = {0};

    if (info_type == INFO_TYPE_CORE_NUM) {
        ret = drvGetDevInfo(devId, &dev_info);
        if (ret != 0) {
            DEVDRV_DRV_ERR("drvGetDevInfo failed ret = %d.\n", ret);
            return ret;
        }
        *value = dev_info.ctrl_cpu_core_num;
    } else if (info_type == INFO_TYPE_ID) {
        ret = drvGetDevInfo(devId, &dev_info);
        if (ret != 0) {
            DEVDRV_DRV_ERR("drvGetDevInfo failed ret = %d.\n", ret);
            return ret;
        }
        *value = dev_info.ctrl_cpu_id;
    } else if (info_type == INFO_TYPE_OCCUPY) {
        ret = drvGetDevInfo(devId, &dev_info);
        if (ret != 0) {
            DEVDRV_DRV_ERR("drvGetDevInfo failed ret = %d.\n", ret);
            return ret;
        }
        *value = dev_info.ctrl_cpu_occupy_bitmap;
    } else if (info_type == INFO_TYPE_IP) {
#ifdef CFG_FEATURE_DMS_ARCH_V1
        ret = drvGetDevInfo(devId, &dev_info);
        if (ret != 0) {
            DEVDRV_DRV_ERR("drvGetDevInfo failed ret = %d.\n", ret);
            return ret;
        }
        *value = dev_info.ctrl_cpu_ip;
#else
        return DRV_ERROR_NOT_SUPPORT;
#endif
    } else if (info_type == INFO_TYPE_ENDIAN) {
        ret = drvGetDevInfo(devId, &dev_info);
        if (ret != 0) {
            DEVDRV_DRV_ERR("drvGetDevInfo failed ret = %d.\n", ret);
            return ret;
        }
        *value = dev_info.ctrl_cpu_endian_little;
    } else if (info_type == INFO_TYPE_OS_SCHED) {
        ret = drvGetCpuInfo(devId, &cpu_info);
        if (ret != 0) {
            DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "drvGetCpuInfo failed ret = %d.\n", ret);
            return ret;
        }
        *value = cpu_info.ccpu_os_sched;
    } else {
        DEVDRV_DRV_INFO("This version does not support this type. (Type=%d)\n", info_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_data_cpu_info(uint32_t devId, int32_t info_type, int64_t *value)
{
    int ret;
    struct tagDrvCpuInfo cpu_info = {0};

    ret = drvGetCpuInfo(devId, &cpu_info);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "drvGetCpuInfo failed ret = %d.\n", ret);
        return ret;
    }

    if (info_type == INFO_TYPE_CORE_NUM) {
        *value = cpu_info.dcpu_num;
    } else if (info_type == INFO_TYPE_OS_SCHED) {
        *value = cpu_info.dcpu_os_sched;
    } else {
        DEVDRV_DRV_INFO("This version does not support this type. (Type=%d)\n", info_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_ai_core_level(uint32_t devId, int32_t info_type, int64_t *value)
{
    struct tagDrvSpec spc_info;
    int ret;
    unsigned char level = 0;

    ret = drvGetDeviceSpec(devId, &spc_info);
    if (ret != 0) {
        return ret;
    }

    if (info_type == INFO_TYPE_CORE_NUM_LEVEL) {
        level = spc_info.aiCoreNumLevel;
    } else if (info_type == INFO_TYPE_FREQUE_LEVEL) {
        level = spc_info.aiCoreFreqLevel;
    }

    if (level == 0) {
#if defined(CFG_SOC_PLATFORM_CLOUD) && !defined(CFG_FEATURE_ERRORCODE_ON_NEW_CHIPS)
        return DRV_ERROR_INVALID_VALUE;
#else
        return DRV_ERROR_NOT_SUPPORT;
#endif
    }
    *value = (int64_t)level;

    return DRV_ERROR_NONE;
}

STATIC drvError_t drvGetAiCoreInfo(uint32_t devId, int32_t info_type, int64_t *value)
{
    struct devdrv_device_info dev_info = {0};
    struct tagDrvSpec spc_info = {0};
    struct devdrv_core_utilization aicore_util = {0};
    int ret = DRV_ERROR_INVALID_VALUE;

    switch (info_type) {
        case INFO_TYPE_UTILIZATION:
            aicore_util.core_type = DEV_DRV_TYPE_AICORE;
            ret = drv_get_core_utilization(devId, &aicore_util);
            if (ret != 0) {
                DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to get aicore utilization. (dev_id=%u; ret=%d)\n",
                    devId, ret);
                return ret;
            }
            *value = (int64_t)aicore_util.utilization;
            break;
        case INFO_TYPE_CORE_NUM:
            ret = drvGetDevInfo(devId, &dev_info);
            if (ret != 0) {
                DEVDRV_DRV_ERR("drvGetDevInfo failed ret = %d.\n", ret);
                return ret;
            }
            *value = dev_info.ai_core_num;
            break;
        case INFO_TYPE_IN_USED:
            ret = drvGetDeviceSpec(devId, &spc_info);
            if (ret != 0) {
                DEVDRV_DRV_ERR("drvGetDeviceSpec failed ret = %d.\n", ret);
                return ret;
            }
            *value = spc_info.aiCoreInuse;
            break;
        case INFO_TYPE_ERROR_MAP:
            ret = drvGetDeviceSpec(devId, &spc_info);
            if (ret != 0) {
                DEVDRV_DRV_ERR("drvGetDeviceSpec failed ret = %d.\n", ret);
                return ret;
            }
            *value = spc_info.aiCoreErrorMap;
            break;
        case INFO_TYPE_ID:
            ret = drvGetDevInfo(devId, &dev_info);
            if (ret != 0) {
                DEVDRV_DRV_ERR("drvGetDevInfo failed ret = %d.\n", ret);
                return ret;
            }
            *value = dev_info.ai_core_id;
            break;
        case INFO_TYPE_FREQUE:
            ret = drvGetDevInfo(devId, &dev_info);
            if (ret != 0) {
                DEVDRV_DRV_ERR("drvGetDevInfo failed ret = %d.\n", ret);
                return ret;
            }
            *value = dev_info.ai_core_freq;
            break;
        case INFO_TYPE_CORE_NUM_LEVEL:
        case INFO_TYPE_FREQUE_LEVEL:
            ret = drv_get_ai_core_level(devId, info_type, value);
            if (ret != 0) {
                if (ret != DRV_ERROR_NOT_SUPPORT) {
                    DEVDRV_DRV_ERR("Failed to invoke drv_get_ai_core_level. (ret=%d)\n", ret);
                }
                return ret;
            }
            break;
        case INFO_TYPE_DIE_NUM:
            return DmsGetAiCoreDieNum(devId, (long long *)value);
        default:
            DEVDRV_DRV_INFO("This version does not support this type. (Type=%d)\n", info_type);
            return ret;
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_vector_core_info(uint32_t devId, int32_t info_type, int64_t *value)
{
    int ret;
    struct devdrv_device_info dev_info = {0};
    struct devdrv_core_utilization aivector_util = {0};

    ret = drvGetDevInfo(devId, &dev_info);
    if (ret != 0) {
        DEVDRV_DRV_ERR("drvGetDevInfo failed ret = %d.\n", ret);
        return ret;
    }

    switch (info_type) {
        case INFO_TYPE_UTILIZATION:
            aivector_util.core_type = DEV_DRV_TYPE_AIVECTOR;
            ret = drv_get_core_utilization(devId, &aivector_util);
            if (ret != 0) {
                DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to get aivector utilization. (dev_id=%u; ret=%d)\n",
                    devId, ret);
                return ret;
            }
            *value = (int64_t)aivector_util.utilization;
            break;
        case INFO_TYPE_CORE_NUM:
            *value = dev_info.vector_core_num;
            break;
        case INFO_TYPE_FREQUE:
#ifdef CFG_FEATURE_VECTOR_CORE_FREQ_IS_ZERO
            if (dev_info.vector_core_freq == 0) {
                return DRV_ERROR_NOT_SUPPORT;
            }
#endif
            *value = dev_info.vector_core_freq;
            break;
        default :
            DEVDRV_DRV_INFO("This version does not support this type. (Type=%d)\n", info_type);
            ret = DRV_ERROR_INVALID_VALUE;
            return ret;
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_ts_cpu_info(uint32_t devId, int32_t info_type, int64_t *value)
{
    int ret;
    struct tagDrvCpuInfo cpu_info = {0};
    struct devdrv_device_info dev_info = {0};

    if (info_type == INFO_TYPE_CORE_NUM) {
        ret = drvGetDevInfo(devId, &dev_info);
        if (ret != 0) {
            DEVDRV_DRV_ERR("drvGetDevInfo failed ret = %d.\n", ret);
            return ret;
        }
        *value = dev_info.ts_cpu_core_num;
    } else if (info_type == INFO_TYPE_OS_SCHED) {
        ret = drvGetCpuInfo(devId, &cpu_info);
        if (ret != 0) {
            DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "drvGetCpuInfo failed ret = %d.\n", ret);
            return ret;
        }
        *value = cpu_info.tscpu_os_sched;
    } else if (info_type == INFO_TYPE_FFTS_TYPE) {
        ret = drv_get_h2d_dev_info(devId, &dev_info);
        if (ret != 0) {
            DEVDRV_DRV_ERR("FFTS drv_get_h2d_dev_info failed ret = %d.\n", ret);
            return ret;
        }
        if (PLAT_GET_CHIP(dev_info.hardware_version) == CHIP_CLOUD_V4) {
            return DRV_ERROR_NOT_SUPPORT;
        }
        *value = dev_info.ffts_type;
    } else {
        DEVDRV_DRV_INFO("This version does not support this type. (Type=%d)\n", info_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_pcieinfo(uint32_t devId, int32_t info_type, int64_t *value)
{
    int ret;
    int32_t bus, dev, func;
    int64_t val;

    if (info_type == INFO_TYPE_ID) {
        ret = drvDeviceGetPcieInfo(devId, &bus, &dev, &func);
        if (ret != 0) {
            DEVDRV_DRV_ERR("drv_device_get_pcie_info failed ret = %d.\n", ret);
            return ret;
        }
        /* obtain the lower eight bits of bus */
        *value = (((((uint32_t)dev) & 0x1f) << PCIE_NUM) | ((uint32_t)(func) & 0x07) | (((uint32_t)(bus) & 0xff) << 8));
    } else if (info_type == INFO_TYPE_P2P_CAPABILITY) {
        ret = DmsGetP2PCapbility(devId, (unsigned long long *)&val);
        if (ret != 0) {
            DEVDRV_DRV_ERR("drvDeviceGetP2PCapbility failed ret = %d.\n", ret);
            return ret;
        }
        *value = val;
    } else {
        DEVDRV_DRV_INFO("This version does not support this type. (Type=%d)\n", info_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

drvError_t drv_get_ts_drv_dev_com_info(struct tsdrv_dev_com_info *dev_com_info)
{
    mmIoctlBuf dev_info_buf = {0};
    int ret;

    drv_ioctl_param_init(&dev_info_buf, (void *)dev_com_info, sizeof(struct tsdrv_dev_com_info));
    ret = drv_common_ioctl(&dev_info_buf, DEVDRV_MANAGER_GET_TSDRV_DEV_COM_INFO);
    if (ret != 0) {
        DEVDRV_DRV_ERR("ioctl failed ret = %d.\n", ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_device_computing_power(uint32_t devId, int32_t info_type, int64_t *value)
{
    struct devdrv_device_info dev_info = {0};
    int ret;

    if (info_type >= DEVDRV_MAX_COMPUTING_POWER_TYPE || info_type < 0) {
        DEVDRV_DRV_ERR("Invalid parameter. (dev=%u; info_type=%d)\n", devId, info_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = drv_get_h2d_dev_info(devId, &dev_info);
    if (ret != 0) {
        DEVDRV_DRV_ERR("Get device info failed. (ret=%d; dev=%u; info_type=%d)\n",
                       ret, devId, info_type);
        return ret;
    }

    if (dev_info.computing_power[info_type] == (uint64_t)DEVDRV_COMPUTING_VALUE_ERROR) {
        DEVDRV_DRV_ERR("Get computing value failed. (dev_id=%u; info_type=%d)\n", devId, info_type);
        return DRV_ERROR_INNER_ERR;
    }

    *value = *(int64_t *)&dev_info.computing_power[info_type];
    return DRV_ERROR_NONE;
}

#if (defined DRV_HOST) && (defined CFG_FEATURE_HOST_AICPU)
STATIC drvError_t drv_get_host_aicpu_info(uint32_t devId, int32_t info_type, int64_t *value)
{
    int ret;
    drvHostAicpuInfo_t info = {0};
    unsigned int size = sizeof(drvHostAicpuInfo_t);

    ret = DmsGetDeviceInfo(devId, DSMI_MAIN_CMD_HOST_AICPU, DSMI_SUB_CMD_HOST_AICPU_INFO, &info, &size);
    if (ret != 0) {
        DEVDRV_DRV_ERR("Failed to obtain the host aicpu info. (dev_id=%u; ret=%d)\n", devId, ret);
        return ret;
    }

    if (info_type == INFO_TYPE_CORE_NUM) {
        *value = (int64_t)info.num;
    } else if (info_type == INFO_TYPE_FREQUE) {
        *value = (int64_t)info.frequency;
    } else if (info_type == INFO_TYPE_OCCUPY) {
        ret = memcpy_s(value, sizeof(unsigned long long) * DSMI_HOST_AICPU_BITMAP_LEN,
            info.bitmap, sizeof(unsigned long long) * DSMI_HOST_AICPU_BITMAP_LEN);
        if (ret != 0) {
            DEVDRV_DRV_ERR("Failed to copy the memory. (dev_id=%u; ret=%d)\n", devId, ret);
            return ret;
        }
    } else if (info_type == INFO_TYPE_WORK_MODE) {
        *value = (int64_t)info.work_mode;
    } else {
        return DRV_ERROR_NOT_SUPPORT;
    }

    return DRV_ERROR_NONE;
}
#endif

STATIC drvError_t drv_get_qos_info(uint32_t devId, int32_t info_type, void *buf, unsigned int *size)
{
#ifdef CFG_FEATURE_QUERY_QOS_CFG_INFO
    int ret;

    if (info_type == INFO_TYPE_CONFIG) {
        ret = drv_get_qos_config(devId, buf, size);
        if (ret != 0) {
            DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to get qos config. (dev_id=%u; ret=%d)\n", devId, ret);
            return ret;
        }
    } else {
        DEVDRV_DRV_INFO("This version does not support this type. (dev_id=%u, Type=%d)\n", devId, info_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
#else
    (void)devId;
    (void)info_type;
    (void)buf;
    (void)size;
    return DRV_ERROR_NOT_SUPPORT;
#endif
}
STATIC drvError_t drv_get_klog_info(uint32_t devId, int32_t info_type, void *path, unsigned int *p_size)
{
#ifdef CFG_FEATURE_QUERY_LOG_INFO
    int ret;

    if (info_type == INFO_TYPE_HOST_KERN_LOG) {
        ret = drvGetKlogBuf(devId, path, p_size);
        if (ret != 0) {
            DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to get log. (dev_id=%u; ret=%d)\n", devId, ret);
            return ret;
        }
    } else {
        return DRV_ERROR_NOT_SUPPORT;
    }

    return DRV_ERROR_NONE;
#else
    (void)devId;
    (void)info_type;
    (void)path;
    (void)p_size;
    return DRV_ERROR_NOT_SUPPORT;
#endif
}

STATIC drvError_t drv_get_ai_core_info_by_buff(uint32_t devId, int32_t info_type, void *buf, unsigned int *size)
{
    int ret = DRV_ERROR_NONE;
#ifdef CFG_FEATURE_QUERY_AICORE_BITMAP
    struct devdrv_device_info dev_info = {0};
    #define AICORE_BITMAP_BUF_SIZE (sizeof(unsigned long long) * 2)  /* 2 * 64 bit, max 128 bit */
#endif

    switch (info_type) {
#ifdef CFG_FEATURE_QUERY_FREQ_INFO
        case INFO_TYPE_CURRENT_FREQ:
            ret = dms_lpm_get_ai_core_curr_freq(devId, buf, size);
            if (ret != 0) {
                DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret,
                    "drv_get_ai_core_info_by_buff unsuccessful. (dev_id=%u; ret=%d; infoType=%d).\n",
                    devId, ret, info_type);
                return ret;
            }

            break;
#endif
#ifdef CFG_FEATURE_QUERY_AICORE_BITMAP
        case INFO_TYPE_OCCUPY:
            ret = drvGetDevInfo(devId, &dev_info);
            if (ret != 0) {
                DEVDRV_DRV_ERR("Get device info failed. (dev_id=%u; ret=%d)\n", devId, ret);
                return ret;
            }

            ret = memcpy_s(buf, *size, &dev_info.aicore_bitmap[0], AICORE_BITMAP_BUF_SIZE);
            if (ret != 0) {
                DEVDRV_DRV_ERR("Copy aicore bitmap failed. (dev_id=%u; ret=%d; src_size=%u; dst_size=%u)\n",
                    devId, ret, AICORE_BITMAP_BUF_SIZE, *size);
                return DRV_ERROR_INVALID_VALUE;
            }

            *size = AICORE_BITMAP_BUF_SIZE;
            break;
#endif
        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    return ret;
}

STATIC drvError_t drv_get_memory_info(uint32_t devId, int32_t info_type, void *buf, unsigned int *size)
{
    int ret = DRV_ERROR_NOT_SUPPORT;

    switch (info_type) {
#ifdef CFG_FEATURE_QUERY_VA_INFO
        case INFO_TYPE_UCE_VA:
            ret = dev_ecc_config_get_va_info(devId, buf, size);
            break;
#endif
        case INFO_TYPE_SYS_COUNT:
            ret = dms_memory_get_hbm_ecc_syscnt(devId, buf, size);
            break;
        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret,
            "drv_get_memory_info unsuccessful. (dev_id=%u; ret=%d; info_type=%d).\n", devId, ret, info_type);
        return ret;
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_lp_info(uint32_t devId, int32_t info_type, void *buf, unsigned int *size)
{
    int ret;

    switch (info_type) {
        case INFO_TYPE_LP_AIC:
        case INFO_TYPE_LP_BUS:
            ret = DmsHalGetDeviceInfoEx(devId, MODULE_TYPE_LP, info_type, buf, size);
            break;

        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to get low power info. (dev_id=%u, infoType=%d, ret=%d)\n",
            devId, info_type, ret);
        return ret;
    }
    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_cc_info(uint32_t devId, int32_t info_type, void *buf, unsigned int *size)
{
    int ret;

    switch (info_type) {
#ifndef DRV_HOST
        case INFO_TYPE_CC:
            ret = dms_get_cc_info(devId, buf, size);
            break;
#else
        (void)buf;
        (void)size;
#endif
        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret,
            "Get cc info unsuccessful. (dev_id=%u; ret=%d; info_type=%d).\n", devId, ret, info_type);
        return ret;
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_l_2_buff_info(uint32_t devId, int32_t info_type, void *buf, unsigned int *size)
{
    int ret = 0;

    switch (info_type) {
        case INFO_TYPE_L2BUFF_RESUME_CNT:
            ret = DmsHalGetDeviceInfoEx(devId, MODULE_TYPE_L2BUFF, info_type, buf, size);
            break;
        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret,
            "Get l2buff info unsuccessful. (dev_id=%u; ret=%d; info_type=%d).\n", devId, ret, info_type);
    }
    return ret;
}

#define DMS_SDK_EX_VERSION_LEN_MAX      128
STATIC drvError_t drv_get_sdk_ex_version(unsigned int dev_id, void *buf, unsigned int *size)
{
    int ret;
    char sdk_ex_ver[DMS_SDK_EX_VERSION_LEN_MAX + 1] = { 0 };
    unsigned int ver_len = DMS_SDK_EX_VERSION_LEN_MAX;

    if (*size == 0) {
        DEVDRV_DRV_ERR("Invalid parameter. (dev_id=%u; size=%u)\n", dev_id, *size);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = DmsHalGetDeviceInfoEx(dev_id, MODULE_TYPE_SYSTEM, INFO_TYPE_SDK_EX_VERSION, sdk_ex_ver, &ver_len);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to get SDK Ex version. (dev_id=%u; ret=%d)\n", dev_id, ret);
        return ret;
    }

    if (ver_len == 0) {
        *size = 0;
        return DRV_ERROR_NONE;
    }

    if (*size < ver_len || ver_len > DMS_SDK_EX_VERSION_LEN_MAX) {
        DEVDRV_DRV_ERR("The buff len is too small. (size=%d; ver_len=%d; buf_len_max=%u)\n",
            *size, ver_len, DMS_SDK_EX_VERSION_LEN_MAX);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = memcpy_s(buf, *size, sdk_ex_ver, ver_len);
    if (ret != 0) {
        DEVDRV_DRV_ERR("Failed to memcpy_s. (ret=%d; dev_id=%u; size=%u)\n", ret, dev_id, *size);
        return DRV_ERROR_INNER_ERR;
    }
    *size = ver_len;

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_system_info_ex(uint32_t devId, int32_t info_type, void *buf, unsigned int *size)
{
    int ret = DRV_ERROR_NOT_SUPPORT;

    switch (info_type) {
        case INFO_TYPE_SDK_EX_VERSION:
            ret = drv_get_sdk_ex_version(devId, buf, size);
            break;
        case INFO_TYPE_UUID:
            ret = DmsHalGetDeviceInfoEx(devId, MODULE_TYPE_SYSTEM, info_type, buf, size);
            break;
        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to set get version info. (devId=%u; infoType=%d; ret=%d)\n",
            devId, info_type, ret);
    }

    return ret;
}

drvError_t halGetDeviceInfoByBuff(uint32_t devId, int32_t module_type, int32_t info_type, void *buf, int32_t *size)
{
    int ret;
    DEV_MODULE_TYPE mtype = module_type;
    DEV_INFO_TYPE itype = info_type;

    if (devId >= ASCEND_DEV_MAX_NUM || buf == NULL || size == NULL) {
        DEVDRV_DRV_ERR("invalid parameter. (dev_id=%u, buf%s, size%s)\n",
            devId, buf==NULL ? "=NULL" : "!=NULL", size==NULL ? "=NULL" : "!=NULL");
        return DRV_ERROR_INVALID_VALUE;
    }

    switch (mtype) {
        case MODULE_TYPE_QOS:
            ret = drv_get_qos_info(devId, itype, buf, (unsigned int *)size);
            break;

        case MODULE_TYPE_AICORE:
            ret = drv_get_ai_core_info_by_buff(devId, itype, buf, (unsigned int *)size);
            break;

        case MODULE_TYPE_MEMORY:
            ret = drv_get_memory_info(devId, itype, buf, (unsigned int *)size);
            break;

        case MODULE_TYPE_LOG:
            ret = drv_get_klog_info(devId, itype, buf, (unsigned int *)size);
            break;

        case MODULE_TYPE_LP:
            ret = drv_get_lp_info(devId, itype, buf, (unsigned int *)size);
            break;

        case MODULE_TYPE_CC:
            ret = drv_get_cc_info(devId, itype, buf, (unsigned int *)size);
            break;

        case MODULE_TYPE_L2BUFF:
            ret = drv_get_l_2_buff_info(devId, itype, buf, (unsigned int *)size);
            break;

        case MODULE_TYPE_SYSTEM:
            ret = drv_get_system_info_ex(devId, itype, buf, (unsigned int *)size);
            break;

        case MODULE_TYPE_UB:
            ret = dms_get_ub_info(devId, itype, buf, size);
            break;

        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret,
            "halGetDeviceInfoByBuff unsuccessful. (dev_id=%u; ret=%d; moduleType=%d; infoType=%d).\n",
            devId, ret, mtype, itype);
        return ret;
    }

    return DRV_ERROR_NONE;
}

#define SET_LP_FREQ_VOLT_MAX 512
#define SET_LP_FREQ_VOLT_PRE_LEN (2 * sizeof(unsigned int)) /* pid, is_offline */
STATIC drvError_t drv_set_lp_freq_volt(unsigned int dev_id, const void *buf, unsigned int size)
{
    unsigned char set_buf[SET_LP_FREQ_VOLT_MAX] = {0};
    int ret;

    ret = memcpy_s(set_buf + SET_LP_FREQ_VOLT_PRE_LEN, sizeof(set_buf) - SET_LP_FREQ_VOLT_PRE_LEN, buf, size);
    if (ret != 0) {
        DEVDRV_DRV_ERR("memcpy_s fail. (dev_id=%u;size=%u)\n", dev_id, size);
        return DRV_ERROR_INVALID_VALUE;
    }
    return DmsHalSetDeviceInfoEx(dev_id, MODULE_TYPE_LP, INFO_TYPE_LP_FREQ_VOLT,
        set_buf, size + SET_LP_FREQ_VOLT_PRE_LEN);
}

STATIC drvError_t drv_set_lp_info(uint32_t devId, int32_t info_type, void *buf, unsigned int size)
{
    int ret;

    switch (info_type) {
        case INFO_TYPE_LP_FREQ_VOLT:
            ret = drv_set_lp_freq_volt(devId, buf, size);
            break;
        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to set low power info. (dev_id=%u, infoType=%d, ret=%d)\n",
            devId, info_type, ret);
        return ret;
    }
    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_set_l_2_buff_info(uint32_t devId, int32_t info_type, void *buf, unsigned int size)
{
    int ret = 0;

    switch (info_type) {
        case INFO_TYPE_L2BUFF_RESUME:
            ret = DmsHalSetDeviceInfoEx(devId, MODULE_TYPE_L2BUFF, INFO_TYPE_L2BUFF_RESUME, buf, size);
            break;
        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to set l2buff info. (devId=%u, infoType=%d, ret=%d)\n",
            devId, info_type, ret);
    }
    return ret;
}

STATIC drvError_t drv_set_sdk_ex_version(unsigned int dev_id, const void *buf, unsigned int size)
{
    int ret;
    char set_buf[DMS_SDK_EX_VERSION_LEN_MAX + 1] = {0};

    if ((size == 0) || (size > DMS_SDK_EX_VERSION_LEN_MAX)) {
        DEVDRV_DRV_ERR("Invalid parameter. (dev_id=%u; size=%u; buf_len_max=%u)\n",
            dev_id, size, DMS_SDK_EX_VERSION_LEN_MAX);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = memcpy_s(set_buf, DMS_SDK_EX_VERSION_LEN_MAX, buf, size);
    if (ret != 0) {
        DEVDRV_DRV_ERR("Failed to invoke memcpy_s. (dev_id=%u; size=%u)\n", dev_id, size);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = DmsHalSetDeviceInfoEx(dev_id, MODULE_TYPE_SYSTEM, INFO_TYPE_SDK_EX_VERSION, set_buf, size);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to set SDK Ex version. (dev_id=%u; ret=%d)\n", dev_id, ret);
        return ret;
    }

    DEVDRV_DRV_EVENT("Set SDK Ex version [%s] success. (dev_id=%u)\n", set_buf, dev_id);

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_set_system_info_ex(uint32_t devId, int32_t info_type, void *buf, unsigned int size)
{
    int ret = DRV_ERROR_NOT_SUPPORT;

    switch (info_type) {
        case INFO_TYPE_SDK_EX_VERSION:
            ret = drv_set_sdk_ex_version(devId, buf, size);
            break;
        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to set extend version info. (dev_id=%u, infoType=%d, ret=%d, size=%u)\n",
            devId, info_type, ret, size);
    }

    return ret;
}

drvError_t halSetDeviceInfoByBuff(uint32_t devId, int32_t module_type, int32_t info_type, void *buf, int32_t size)
{
    int ret;
    DEV_MODULE_TYPE mtype = module_type;
    DEV_INFO_TYPE itype = info_type;

    if (devId >= ASCEND_DEV_MAX_NUM || buf == NULL || size <= 0) {
        DEVDRV_DRV_ERR("invalid parameter. (dev_id=%u;buf=%d;size=%d)\n", devId, buf != NULL, size);
        return DRV_ERROR_INVALID_VALUE;
    }

    switch (mtype) {
        case MODULE_TYPE_LP:
            ret = drv_set_lp_info(devId, itype, buf, size);
            break;

        case MODULE_TYPE_L2BUFF:
            ret = drv_set_l_2_buff_info(devId, itype, buf, (unsigned int)size);
            break;

        case MODULE_TYPE_SYSTEM:
            ret = drv_set_system_info_ex(devId, itype, buf, (unsigned int)size);
            break;

        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret,
            "halSetDeviceInfoByBuff unsuccessful. (dev_id=%u; ret=%d; moduleType=%d; infoType=%d).\n",
            devId, ret, mtype, itype);
        return ret;
    }

    return DRV_ERROR_NONE;
}

static int hal_get_device_info_para_check(uint32_t devId, int32_t module_type, int32_t info_type, int64_t *value)
{
    if ((module_type == MODULE_TYPE_SYSTEM) && (info_type == INFO_TYPE_SPOD_VNIC_IP)) {
        /* input devId is sdid, no need check*/
        return 0;
    }

    if (devId >= ASCEND_DEV_MAX_NUM || value == NULL) {
        DEVDRV_DRV_ERR("Invalid parameter. (devId=%u; max_dev_id=%u; value_is_null=%d)\n",
            devId, ASCEND_DEV_MAX_NUM, (value == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    return 0;
}

drvError_t halGetDeviceInfo(uint32_t devId, int32_t module_type, int32_t info_type, int64_t *value)
{
    int ret;
    DEV_INFO_TYPE itype = info_type;
    DEV_MODULE_TYPE mtype = module_type;

    if (hal_get_device_info_para_check(devId, module_type, info_type, value) != 0) {
        return DRV_ERROR_INVALID_VALUE;
    }

    switch (mtype) {
        case MODULE_TYPE_SYSTEM:
            ret = drv_get_system_info(devId, itype, value);
            break;

        case MODULE_TYPE_AICPU:
            ret = drv_get_ai_cpu_info(devId, itype, value);
            break;

        case MODULE_TYPE_DCPU:
            ret = drv_get_data_cpu_info(devId, itype, value);
            break;

        case MODULE_TYPE_CCPU:
            ret = drv_get_ctrl_cpu_info(devId, itype, value);
            break;

        case MODULE_TYPE_AICORE:
            ret = drvGetAiCoreInfo(devId, itype, value);
            break;

        case MODULE_TYPE_TSCPU:
            ret = drv_get_ts_cpu_info(devId, itype, value);
            break;

        case MODULE_TYPE_PCIE:
            ret = drv_get_pcieinfo(devId, itype, value);
            break;

        case MODULE_TYPE_VECTOR_CORE:
            ret = drv_get_vector_core_info(devId, itype, value);
            break;

        case MODULE_TYPE_COMPUTING:
            ret = drv_get_device_computing_power(devId, info_type, value);
            break;

        case MODULE_TYPE_HOST_AICPU:
#if (defined DRV_HOST) && (defined CFG_FEATURE_HOST_AICPU)
            ret = drv_get_host_aicpu_info(devId, info_type, value);
            break;
#else
            return DRV_ERROR_NOT_SUPPORT;
#endif

        default:
            DEVDRV_DRV_INFO("This version does not support this moduleType. (moduleType=%d)\n", mtype);
            return DRV_ERROR_INVALID_VALUE;
    }

    /*  to retain scalability, no error log here */
    if (ret != 0) {
        DEVDRV_DRV_INFO("Reasult of call halgetdeviceinfo. (ret=%d; module_type=%d; info_type=%d).\n",
            ret, mtype, itype);
        return ret;
    }

    return DRV_ERROR_NONE;
}

STATIC drvError_t drv_get_sys_info_by_phy_id(uint32_t phy_id, int32_t info_type, int64_t *value)
{
    int ret;
    unsigned int tmp_value;

    switch (info_type) {
        case PHY_INFO_TYPE_CHIPTYPE:
            ret = DmsGetChipType(phy_id, &tmp_value);
            if (ret != 0) {
                DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "drvGetDevInfo failed ret = %d.\n", ret);
                return ret;
            }
            *value = (int64_t)tmp_value;
            break;
        case PHY_INFO_TYPE_MASTER_ID:
            ret = DmsGetMasterDevInTheSameOs(phy_id, &tmp_value);
            if (ret != 0) {
                DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to obtain the master device. (phy_id=%u; ret=%d)\n",
                    phy_id, ret);
                return ret;
            }
            *value = (int64_t)tmp_value;
            break;
        default:
            DEVDRV_DRV_ERR("invalid parameter. (info_type=%d; phy_id=%u)\n", info_type, phy_id);
            return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

drvError_t halGetPhyDeviceInfo(uint32_t phy_id, int32_t module_type, int32_t info_type, int64_t *value)
{
    int ret;
    DEV_MODULE_TYPE mtype = module_type;
    DEV_INFO_TYPE itype = info_type;

    if (phy_id >= ASCEND_DEV_MAX_NUM || value == NULL) {
        DEVDRV_DRV_ERR("invalid parameter, value is NULL or phy_id(%u) is invalid\n", phy_id);
        return DRV_ERROR_INVALID_VALUE;
    }

    switch (mtype) {
        case MODULE_TYPE_SYSTEM:
            ret = drv_get_sys_info_by_phy_id(phy_id, itype, value);
            break;

        default:
            DEVDRV_DRV_ERR("invalid parameter. (moduleType=%d)\n", mtype);
            return DRV_ERROR_INVALID_VALUE;
    }
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Get device info failed. (ret=%d; module_type=%d; info_type=%d; phy_id=%u)\n",
                       ret, mtype, itype, phy_id);
        return ret;
    }
    return DRV_ERROR_NONE;
}

drvError_t halGetPairDevicesInfo(uint32_t devId, uint32_t other_dev_id, int32_t info_type, int64_t *value)
{
#ifdef DRV_HOST
    int ret;
    int topology_type = 0;

    if (value == NULL) {
        DEVDRV_DRV_ERR("Input value is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((drvCheckDevid(devId) != 0) || (drvCheckDevid(other_dev_id) != 0)) {
        DEVDRV_DRV_ERR("Device id check failed. (devId=%u; other_dev_id=%u)\n", devId, other_dev_id);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (info_type == DEVS_INFO_TYPE_TOPOLOGY) {
        ret = dms_get_device_topology(devId, other_dev_id, &topology_type);
        if (ret != 0) {
            DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Get device topology failed. (dev_id=%u; dev_id_other=%u; ret=%d)\n",
                devId, other_dev_id, ret);
            return ret;
        }

        *value = topology_type;
        DEVDRV_DRV_DEBUG("Get topology type success. (dev_id=%u; dev_id_other=%u)\n", devId, other_dev_id);
    } else {
        DEVDRV_DRV_ERR("Invalid info type. (info_type=%d)\n", info_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
#else
    (void)devId;
    (void)other_dev_id;
    (void)info_type;
    (void)value;
    return DRV_ERROR_NOT_SUPPORT;
#endif
}

drvError_t halGetPairPhyDevicesInfo(uint32_t phyDevid, uint32_t other_phy_devid, int32_t info_type, int64_t *value)
{
#if (defined CFG_FEATURE_PHY_DEVICES_TOPO) && (defined DRV_HOST)
    int ret;
    int topology_type = 0;

    if (value == NULL) {
        DEVDRV_DRV_ERR("Input value is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (info_type == DEVS_INFO_TYPE_TOPOLOGY) {
        ret = dms_get_phy_devices_topology(phyDevid, other_phy_devid, &topology_type);
        if (ret != 0) {
            DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to get devices topology info."
                "(dev_id=%u; dev_id_other=%u; ret=%d)\n", phyDevid, other_phy_devid, ret);
            return ret;
        }

        *value = topology_type;
    } else {
        return DRV_ERROR_NOT_SUPPORT;
    }

    return DRV_ERROR_NONE;
#else
    (void)phyDevid;
    (void)other_phy_devid;
    (void)info_type;
    (void)value;
    return DRV_ERROR_NOT_SUPPORT;
#endif
}

STATIC bool drv_is_ts_resource(u32 resource_type)
{
#ifdef CFG_FEATURE_TS_RESOURCE_FROM_US
    /* Query TS Information from User Space */
    return (resource_type <= DEVDRV_MAX_TS_INFO_TYPE);
#else
    (void)resource_type;
    return false;
#endif
}

STATIC int drv_resource_info_para_check(u32 devid, struct dsmi_resource_para *para, struct dsmi_resource_info *info)
{
    if (devid > VDAVINCI_MAX_VDEV_ID || info == NULL || para == NULL) {
        DEVDRV_DRV_ERR("Invalid parameter. (device_id=%u; info_is_null=%d; para_is_null=%d)\n",
            devid, (info == NULL), (para == NULL));
        return DRV_ERROR_PARA_ERROR;
    }

    if (info->buf == NULL || info->buf_len > DEVDRV_MAX_PAYLOAD_LEN) {
        DEVDRV_DRV_ERR("Invalid input info. (device_id=%u; buf_is_null=%d; buf_len=%u)\n",
            devid, (info->buf == NULL), info->buf_len);
        return DRV_ERROR_PARA_ERROR;
    }

    if (para->owner_type >= DEVDRV_MAX_OWNER_TYPE) {
        DEVDRV_DRV_ERR("Invalid input resource parameter. (device_id=%u; owner_type=%u)\n",
            devid, para->owner_type);
        return DRV_ERROR_PARA_ERROR;
    }

    return 0;
}

STATIC int drv_ts_resource_info_query(u32 devid, struct dsmi_resource_para *para, struct dsmi_resource_info *info)
{
    drvResourceType_t resource_type[DRV_RESOURCE_NOTIFY_ID + 1] = {DRV_RESOURCE_STREAM_ID, DRV_RESOURCE_EVENT_ID,
        DRV_RESOURCE_NOTIFY_ID, DRV_RESOURCE_MODEL_ID};
    struct halResourceInfo resinfo = {0};
    u32 logic_devid = devid;
    int ret;

    if (((para->owner_type == DSMI_DEV_RESOURCE) && (devid >= VDAVINCI_VDEV_OFFSET)) ||
        ((para->owner_type == DSMI_VDEV_RESOURCE) && (devid < VDAVINCI_VDEV_OFFSET))) {
        DEVDRV_DRV_ERR("Invalid input resource parameter. (device_id=%u; owner_type=%u)\n",
            devid, para->owner_type);
        return DRV_ERROR_PARA_ERROR;
    }

    if (devid >= VDAVINCI_VDEV_OFFSET) {
        ret = drvDeviceGetIndexByPhyId(devid, &logic_devid);
        if (ret != 0) {
            DEVDRV_DRV_ERR("Get logic devid failed. (phyDevid=%u)\n", devid);
            return ret;
        }
    }
    ret = halResourceInfoQuery(logic_devid, para->tsid, resource_type[para->resource_type], &resinfo);
    if (ret != 0) {
        DEVDRV_DRV_ERR("Resource info query failed. (devid=%u; ret=%d)\n", devid, ret);
        return ret;
    }

    ret = memcpy_s(info->buf, info->buf_len, (void *)&(resinfo.capacity), sizeof(resinfo.capacity));
    if (ret != 0) {
        DEVDRV_DRV_ERR("Memcpy to buffer failed. (device_id=%u; ret=%d)\n", devid, ret);
        return DRV_ERROR_INVALID_HANDLE;
    }

    return 0;
}

STATIC int drv_get_resource_info_ioctl(u32 devid, struct dsmi_resource_para *para, struct dsmi_resource_info *info)
{
    struct devdrv_resource_info d_info = {0};
    int ret;

    d_info.devid = devid;
    d_info.owner_type = para->owner_type;
    d_info.owner_id = para->owner_id;
    d_info.resource_type = para->resource_type;
    d_info.tsid = para->tsid;
    d_info.buf_len = info->buf_len;
    ret = dmanage_common_ioctl(DEVDRV_MANAGER_GET_DEV_RESOURCE_INFO, &d_info);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Ioctl failed. (device_id=%u; ret=%d)\n", devid, ret);
        return ret;
    }

    ret = memcpy_s(info->buf, info->buf_len, d_info.buf, d_info.buf_len);
    if (ret != 0) {
        DEVDRV_DRV_ERR("Memcpy to buffer failed. (device_id=%u; ret=%d)\n", devid, ret);
        return DRV_ERROR_INVALID_HANDLE;
    }

    return 0;
}

drvError_t drvGetDeviceResourceInfo(u32 devid, struct dsmi_resource_para *para, struct dsmi_resource_info *info)
{
    int ret = 0;

    ret = drv_resource_info_para_check(devid, para, info);
    if (ret != 0) {
        return ret;
    }

    if (drv_is_ts_resource(para->resource_type) && (para->owner_type < DSMI_PROCESS_RESOURCE)) {
        ret = drv_ts_resource_info_query(devid, para, info);
#ifdef CFG_FEATURE_APM_SUPP_PID
    } else if (para->owner_type == DSMI_PROCESS_RESOURCE) {
        ret = dms_get_process_resource(devid, para, info->buf, info->buf_len);
#endif
    } else {
        ret = drv_get_resource_info_ioctl(devid, para, info);
    }
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret,
            "Get resource info failed. (dev_id=%u; ret=%d; owner_type=%u; resource_type=%u)\n",
            devid, ret, para->owner_type, para->resource_type);
        return ret;
    }

    return DRV_ERROR_NONE;
}

STATIC void devdrv_black_box_exception_process(const struct devdrv_black_box_user *black_box_user,
                                               drvBlackBoxCallbackFunc_t *callback)
{
    drvDeviceExceptionReporFunc exp_rep_func = callback->exp_report_func;
    struct timespec stamp;

    stamp.tv_sec = (long)black_box_user->tv_sec;
    stamp.tv_nsec = (long)black_box_user->tv_nsec;

    if (exp_rep_func == NULL) {
        DEVDRV_DRV_DEBUG("exception callback function is not registered\n");
        return;
    }
    exp_rep_func(black_box_user->devid, black_box_user->exception_code, stamp);
}

STATIC void devdrv_black_box_dev_id_notify_process(struct devdrv_black_box_user *black_box_user,
                                                   drvBlackBoxCallbackFunc_t *callback)
{
    drvDeviceStartupNotify start_noti_func = callback->start_notifunc;

    if (start_noti_func == NULL) {
        DEVDRV_DRV_DEBUG("exception callback function is not registered\n");
        return;
    }

    start_noti_func(black_box_user->priv_data.bbox_devids.dev_num, black_box_user->priv_data.bbox_devids.devids);
}

#ifdef __linux
STATIC void devdrv_black_box_dev_state_notify_process(const struct devdrv_black_box_user *black_box_user,
                                                      drvBlackBoxCallbackFunc_t *callback)
{
    drvDeviceStateNotify state_noti_func = callback->state_notifunc;
    devdrv_state_info_t state_info = {0};
    if (state_noti_func == NULL) {
        DEVDRV_DRV_ERR("exception callback function is not registered.\n");
        return;
    }

    state_info.state = (devdrv_state_t)black_box_user->priv_data.bbox_state.state;
    state_info.devId = black_box_user->priv_data.bbox_state.devId;
    DEVDRV_DRV_INFO("state_notifunc called(devId:%d, state(%d), code(0x%x)).\n", state_info.devId, state_info.state,
                    black_box_user->exception_code);
    state_noti_func(&state_info);
}
#endif

STATIC void devdrv_black_box_notify_process(struct devdrv_black_box_user *black_box_user,
                                            drvBlackBoxCallbackFunc_t *callback)
{
    if (black_box_user->exception_code == 0) {
        return;
    }

    switch (black_box_user->exception_code) {
#ifdef __linux
        case DEVDRV_BB_DEVICE_STATE_INFORM:
            devdrv_black_box_dev_state_notify_process(black_box_user, callback);
            break;
#endif
        case DEVDRV_BB_DEVICE_ID_INFORM:
            devdrv_black_box_dev_id_notify_process(black_box_user, callback);
            break;
        default:
            devdrv_black_box_exception_process(black_box_user, callback);
            break;
    }

    return;
}

STATIC void *devdrv_black_box_notify_thread(void *data)
{
    struct devdrv_black_box_user black_box_user = {0};
    drvBlackBoxCallbackFunc_t *callback = NULL;
    int ret;
    mmIoctlBuf buf = {0};

#ifdef __linux
    (void)prctl(PR_SET_NAME, "devmng_exception_notify");
#endif

    DEVDRV_DRV_DEBUG("thread for black box started.\n");

    callback = (drvBlackBoxCallbackFunc_t *)data;
    if (callback == NULL) {
        DEVDRV_DRV_ERR("invalid callback function, thread stop.\n");
        return NULL;
    }

    while (devdrv_black_box_thread_work != 0) {
        ret = memset_s(&black_box_user, sizeof(struct devdrv_black_box_user), 0, sizeof(struct devdrv_black_box_user));
        if (ret != EOK) {
            DEVDRV_DRV_ERR("memset_s returned error: %d.\n", ret);
            return NULL;
        }

        drv_ioctl_param_init(&buf, (void *)&black_box_user, sizeof(struct devdrv_black_box_user));
        ret = drv_common_ioctl(&buf, DEVDRV_MANAGER_BLACK_BOX_GET_EXCEPTION);
        if (ret != 0) {
            DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "ioctl failed. (ret=%d)\n", ret);
            break;
        }

        if (black_box_user.thread_should_stop != 0) {
            DEVDRV_DRV_ERR("thread for black box should stop.\n");
            break;
        }

        devdrv_black_box_notify_process(&black_box_user, callback);
    }

    DEVDRV_DRV_DEBUG("thread for black box stop.\n");
    return NULL;
}

#ifdef __linux
drvError_t drvDeviceStateNotifierRegister(drvDeviceStateNotify state_callback)
{
    int ret;

    if (state_callback == NULL) {
        DEVDRV_DRV_ERR("invalid input callback func.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }

    (void)mmMutexLock(&devdrv_black_box_mutex);
    if (BBoxCallbackFunc.state_notifunc != NULL) {
        (void)mmMutexUnLock(&devdrv_black_box_mutex);
        DEVDRV_DRV_ERR("already create thread for black box.\n");
        return DRV_ERROR_CLIENT_BUSY;
    }
    BBoxCallbackFunc.state_notifunc = state_callback;

    if (devdrv_black_box_thread_work == 1) {
        (void)mmMutexUnLock(&devdrv_black_box_mutex);
        DEVDRV_DRV_INFO("already create thread for black box.\n");
        return DRV_ERROR_NONE;
    }

    devdrv_black_box_thread_work = 1;
    func_block.procFunc = devdrv_black_box_notify_thread;
    func_block.pulArg = (void *)&BBoxCallbackFunc;

    ret = mmCreateTaskWithDetach(&devdrv_black_box_thread, &func_block);
    if (ret != 0) {
        DEVDRV_DRV_ERR("pthread_create fail.\n");
        BBoxCallbackFunc.exp_report_func = NULL;
        devdrv_black_box_thread_work = 0;
        (void)mmMutexUnLock(&devdrv_black_box_mutex);
        return DRV_ERROR_SOCKET_CREATE;
    }
    (void)mmMutexUnLock(&devdrv_black_box_mutex);
    DEVDRV_DRV_INFO("state callback registered.\n");
    return DRV_ERROR_NONE;
}
#endif

drvError_t drvDeviceStartupRegister(drvDeviceStartupNotify startup_callback)
{
    int ret;

    if (startup_callback == NULL) {
        DEVDRV_DRV_ERR("invalid input callback func.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }
#ifndef __linux
    if (devdrv_black_box_mutex == PTHREAD_MUTEX_INITIALIZER) {
        mmMutexInit(&devdrv_black_box_mutex);
    }
#endif
    (void)mmMutexLock(&devdrv_black_box_mutex);
    if (BBoxCallbackFunc.start_notifunc != NULL) {
        DEVDRV_DRV_ERR("already create thread for black box.\n");
        (void)mmMutexUnLock(&devdrv_black_box_mutex);
        return DRV_ERROR_CLIENT_BUSY;
    }
    BBoxCallbackFunc.start_notifunc = startup_callback;

    if (devdrv_black_box_thread_work == 1) {
        DEVDRV_DRV_INFO("already create thread for black box.\n");
        (void)mmMutexUnLock(&devdrv_black_box_mutex);
        return DRV_ERROR_NONE;
    }

    devdrv_black_box_thread_work = 1;
    func_block.procFunc = devdrv_black_box_notify_thread;
    func_block.pulArg = (void *)&BBoxCallbackFunc;

    ret = mmCreateTaskWithDetach(&devdrv_black_box_thread, &func_block);
    if (ret != 0) {
        DEVDRV_DRV_ERR("pthread_create fail.\n");
        BBoxCallbackFunc.exp_report_func = NULL;
        devdrv_black_box_thread_work = 0;
        (void)mmMutexUnLock(&devdrv_black_box_mutex);
        return DRV_ERROR_SOCKET_CREATE;
    }
    (void)mmMutexUnLock(&devdrv_black_box_mutex);
    DEVDRV_DRV_INFO("Startup callback registered.\n");
    return DRV_ERROR_NONE;
}

drvError_t drvDeviceExceptionHookRegister(drvDeviceExceptionReporFunc exception_callback_func)
{
    int ret;

    if (exception_callback_func == NULL) {
        DEVDRV_DRV_ERR("Invalid input callback func.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }
#ifndef __linux
    if (devdrv_black_box_mutex == PTHREAD_MUTEX_INITIALIZER) {
        mmMutexInit(&devdrv_black_box_mutex);
    }
#endif
    (void)mmMutexLock(&devdrv_black_box_mutex);
    if (BBoxCallbackFunc.exp_report_func != NULL) {
        (void)mmMutexUnLock(&devdrv_black_box_mutex);
        DEVDRV_DRV_ERR("already create thread for black box.\n");
        return DRV_ERROR_CLIENT_BUSY;
    }

    BBoxCallbackFunc.exp_report_func = exception_callback_func;
    if (devdrv_black_box_thread_work == 1) {
        (void)mmMutexUnLock(&devdrv_black_box_mutex);
        DEVDRV_DRV_INFO("already create thread for black box.\n");
        return DRV_ERROR_NONE;
    }

    devdrv_black_box_thread_work = 1;
    func_block.procFunc = devdrv_black_box_notify_thread;
    func_block.pulArg = (void *)&BBoxCallbackFunc;

    ret = mmCreateTaskWithDetach(&devdrv_black_box_thread, &func_block);
    if (ret != 0) {
        DEVDRV_DRV_ERR("pthread_create fail.\n");
        devdrv_black_box_thread_work = 0;
        BBoxCallbackFunc.exp_report_func = NULL;
        (void)mmMutexUnLock(&devdrv_black_box_mutex);
        return DRV_ERROR_SOCKET_CREATE;
    }
    (void)mmMutexUnLock(&devdrv_black_box_mutex);

    DEVDRV_DRV_INFO("Exception callback registered.\n");
    return DRV_ERROR_NONE;
}
#ifdef CFG_FEATURE_SUPPORT_DEVMNG_BBOX
drvError_t drv_device_memory_dump(uint32_t devId, uint64_t bbox_addr_offset, uint32_t size, void *buffer)
{
    struct devdrv_black_box_user black_box_user = {0};
    int ret;
    mmIoctlBuf buf = {0};
    if (buffer == NULL) {
        DEVDRV_DRV_ERR("buffer is NULL. (devid=%u)\n", devId);
        return DRV_ERROR_INVALID_HANDLE;
    }

    if (devId >= ASCEND_DEV_MAX_NUM) {
        DEVDRV_DRV_ERR("invalid devid = %u.\n", devId);
        return DRV_ERROR_INVALID_VALUE;
    }

    black_box_user.devid = devId;
    black_box_user.size = size;
    black_box_user.addr_offset = bbox_addr_offset;
    black_box_user.dst_buffer = buffer;

    drv_ioctl_param_init(&buf, (void *)&black_box_user, sizeof(struct devdrv_black_box_user));
    ret = drv_common_ioctl(&buf, DEVDRV_MANAGER_DEVICE_MEMORY_DUMP);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "ioctl failed ret = %d.\n", ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

drvError_t drv_vmcore_dump(uint32_t devId, uint64_t bbox_addr_offset, uint32_t size, void *buffer)
{
#ifdef CFG_FEATURE_BBOX_KDUMP
    struct devdrv_black_box_user black_box_user = {0};
    int ret;
    mmIoctlBuf buf = {0};

    if (buffer == NULL) {
        DEVDRV_DRV_ERR("buffer is NULL. (devid=%u)\n", devId);
        return DRV_ERROR_PARA_ERROR;
    }

    if (devId >= ASCEND_DEV_MAX_NUM) {
        DEVDRV_DRV_ERR("Invalid device id. (dev_id=%u; max_dev_num=%u)\n", devId, ASCEND_DEV_MAX_NUM);
        return DRV_ERROR_PARA_ERROR;
    }

    black_box_user.devid = devId;
    black_box_user.size = size;
    black_box_user.addr_offset = bbox_addr_offset;
    black_box_user.dst_buffer = buffer;

    drv_ioctl_param_init(&buf, (void *)&black_box_user, sizeof(struct devdrv_black_box_user));
    ret = drv_common_ioctl(&buf, DEVDRV_MANAGER_DEVICE_VMCORE_DUMP);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Ioctl failed. (ret=%d)\n", ret);
        return ret;
    }

    return DRV_ERROR_NONE;
#else
    (void)devId;
    (void)bbox_addr_offset;
    (void)size;
    (void)buffer;
    return DRV_ERROR_NOT_SUPPORT;
#endif
}

drvError_t drv_ts_log_dump(uint32_t devid, uint64_t bbox_addr_offset, uint32_t size, void *buffer)
{
    int ret;
    mmIoctlBuf buf = {0};
    struct devdrv_black_box_user black_box_user = {0};

    if (buffer == NULL) {
        DEVDRV_DRV_ERR("Buffer is NULL. (device id=%u)\n", devid);
        return DRV_ERROR_PARA_ERROR;
    }

    if (devid >= ASCEND_DEV_MAX_NUM) {
        DEVDRV_DRV_ERR("Invalid device id. (device id=%u).\n", devid);
        return DRV_ERROR_PARA_ERROR;
    }

    black_box_user.devid = devid;
    black_box_user.size = size;
    black_box_user.addr_offset = bbox_addr_offset;
    black_box_user.dst_buffer = buffer;

    drv_ioctl_param_init(&buf, (void *)&black_box_user, sizeof(struct devdrv_black_box_user));
    ret = drv_common_ioctl(&buf, DEVDRV_MANAGER_TS_LOG_DUMP);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Ioctl failed. (ret=%d; device id=%u).\n", ret, devid);
        return ret;
    }

    return DRV_ERROR_NONE;
}

int drv_dev_log_dump(uint32_t devid, uint64_t bbox_addr_offset, uint32_t size, void *buffer, uint32_t log_type)
{
    int ret;
    struct dms_ioctl_arg ioarg = { 0 };
    struct devdrv_bbox_logdump bbox_log_dump = {0};

    if (buffer == NULL) {
        DEVDRV_DRV_ERR("Buffer is NULL. (devid=%u)\n", devid);
        return DRV_ERROR_PARA_ERROR;
    }

    if (devid >= ASCEND_DEV_MAX_NUM) {
        DEVDRV_DRV_ERR("Invalid device id. (devid=%u, max_devid=%d).\n", devid, ASCEND_DEV_MAX_NUM);
        return DRV_ERROR_PARA_ERROR;
    }

    bbox_log_dump.bbox_user = (struct devdrv_black_box_user *)malloc(sizeof(struct devdrv_black_box_user));

    if (bbox_log_dump.bbox_user == NULL) {
        DEVDRV_DRV_ERR("Allocate memory for bbox_user failed. (devid=%u).\n", devid);
        return DRV_ERROR_MALLOC_FAIL;
    }

    bbox_log_dump.bbox_user->devid = devid;
    bbox_log_dump.bbox_user->size = size;
    bbox_log_dump.bbox_user->addr_offset = bbox_addr_offset;
    bbox_log_dump.bbox_user->dst_buffer = buffer;

    bbox_log_dump.log_type = log_type;

    ioarg.main_cmd = DMS_MAIN_CMD_BBOX;
    ioarg.sub_cmd = DMS_SUBCMD_GET_LOG_DUMP_INFO;
    ioarg.filter_len = 0;
    ioarg.input = (void *)&bbox_log_dump;
    ioarg.input_len = sizeof(struct devdrv_bbox_logdump);
    ioarg.output = NULL;
    ioarg.output_len = 0;

    ret = errno_to_user_errno(DmsIoctl(DMS_IOCTL_CMD, &ioarg));
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "drv_dev_log_dump failed. (devid=%u, ret=%d)", devid, ret);
    }

    free(bbox_log_dump.bbox_user);
    bbox_log_dump.bbox_user = NULL;
    return ret;
}

drvError_t drv_reg_ddr_read(uint32_t devId, uint64_t reg_addr_offset, uint32_t size, void *buffer)
{
    struct devdrv_black_box_user black_box_user = {0};
    int ret;
    mmIoctlBuf buf = {0};
    if (buffer == NULL) {
        DEVDRV_DRV_ERR("buffer is NULL. (devid=%u)\n", devId);
        return DRV_ERROR_INVALID_HANDLE;
    }

    if (devId >= ASCEND_DEV_MAX_NUM) {
        DEVDRV_DRV_ERR("invalid devid = %u.\n", devId);
        return DRV_ERROR_INVALID_VALUE;
    }

    black_box_user.devid = devId;
    black_box_user.size = size;
    black_box_user.addr_offset = reg_addr_offset;
    black_box_user.dst_buffer = buffer;

    drv_ioctl_param_init(&buf, (void *)&black_box_user, sizeof(struct devdrv_black_box_user));
    ret = drv_common_ioctl(&buf, DEVDRV_MANAGER_REG_DDR_READ);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "ioctl failed ret = %d.\n", ret);
        return DRV_ERROR_IOCRL_FAIL;
    }

    return DRV_ERROR_NONE;
}
#endif

drvError_t drvDeviceResetInform(uint32_t devid)
{
    int ret;
    mmIoctlBuf buf = {0};
    if (devid >= ASCEND_DEV_MAX_NUM) {
        DEVDRV_DRV_ERR("invalid devid(%u).\n", devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    drv_ioctl_param_init(&buf, (void *)&devid, sizeof(uint32_t));
    ret = drv_common_ioctl(&buf, DEVDRV_MANAGER_DEVICE_RESET_INFORM);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "ioctl failed ret = %d.\n", ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

#ifdef CFG_SOC_PLATFORM_CLOUD
drvError_t drvGetDeviceModuleStatus(uint32_t devId, drvModuleStatus_t *module_status)
{
    struct devdrv_module_status status;
    int ret;
    mmIoctlBuf buf = {0};

    if (devId >= ASCEND_DEV_MAX_NUM) {
        DEVDRV_DRV_ERR("invalid devid(%u).\n", devId);
        return DRV_ERROR_INVALID_VALUE;
    }

    status.devid = devId;
#else
drvError_t drvGetDeviceModuleStatus(drvModuleStatus_t *module_status)
{
    struct devdrv_module_status status;
    int ret;
    mmIoctlBuf buf = {0};

    status.devid = 0;
#endif

    if (module_status == NULL) {
        DEVDRV_DRV_ERR("moduleStatus is NULL.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }

    ret = memset_s((void *)module_status, sizeof(drvModuleStatus_t), 0, sizeof(drvModuleStatus_t));
    if (ret != EOK) {
        DEVDRV_DRV_ERR("memset_s failed, ret(%d).\n", ret);
        return DRV_ERROR_INVALID_VALUE;
    }

    drv_ioctl_param_init(&buf, (void *)&status, sizeof(struct devdrv_module_status));
    ret = drv_common_ioctl(&buf, DEVDRV_MANAGER_GET_MODULE_STATUS);
    if (ret != 0) {
        DEVDRV_DRV_ERR("ioctl failed ret = %d.\n", ret);
        return ret;
    }

    /* cloud hasn't lpm3, value about lpm3 always be 0 */
    module_status->ai_core_error_bitmap = status.ai_core_error_bitmap;
    module_status->lpm3_start_fail = status.lpm3_start_fail;
    module_status->lpm3_lost_heart_beat = status.lpm3_lost_heart_beat;
    module_status->ts_start_fail = status.ts_start_fail;
    module_status->ts_lost_heart_beat = status.ts_lost_heart_beat;
    module_status->ts_sram_broken = status.ts_sram_broken;
    module_status->ts_sdma_broken = status.ts_sdma_broken;
    module_status->ts_bs_broken = status.ts_bs_broken;
    module_status->ts_l2_buf0_broken = status.ts_l2_buf0_broken;
    module_status->ts_l2_buf1_broken = status.ts_l2_buf1_broken;
    module_status->ts_spcie_broken = status.ts_spcie_broken;
    module_status->ts_ai_core_broken = status.ts_ai_core_broken;
    module_status->ts_hwts_broken = status.ts_hwts_broken;
    module_status->ts_doorbell_broken = status.ts_doorbell_broken;

    return DRV_ERROR_NONE;
}

drvError_t drvGetCpuInfo(uint32_t devId, drvCpuInfo_t *cpu_info)
{
    int ret;

    if (cpu_info == NULL) {
        DEVDRV_DRV_ERR("cpuInfo is NULL. devid(%u)\n", devId);
        return DRV_ERROR_INVALID_HANDLE;
    }

    if (devId >= ASCEND_DEV_MAX_NUM) {
        DEVDRV_DRV_ERR("invalid devid(%u).\n", devId);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = DmsGetCpuInfo(devId, cpu_info);
    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "dms get cpu info failed. (devid=%u; ret=%d)\n", devId, ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

void init_devdrv_manager_lock(void)
{
    (void)mmMutexInit(&devdrv_manager_fd_mutex);
    return;
}

drvError_t halGetAPIVersion(int *halAPIVersion)
{
    if (halAPIVersion == NULL) {
        return DRV_ERROR_INVALID_VALUE;
    }

    *halAPIVersion = __HAL_API_VERSION;

    return DRV_ERROR_NONE;
}

#ifndef DEVDRV_UT
int g_runtime_api_ver;
void halSetRuntimeApiVer(int Version)
{
    g_runtime_api_ver = Version;
}

int drvGetRuntimeApiVer(void)
{
    return g_runtime_api_ver;
}
#endif

drvError_t halGetOnlineDevList(unsigned int *dev_buf, unsigned int buf_cnt, unsigned int *dev_cnt)
{
    int ret;
    int i = 0;
    mmIoctlBuf buf = {0};
    unsigned int dev_buf_tmp[ASCEND_DEV_MAX_NUM] = {0};
    unsigned int valid_dev_num = 0;

    if ((dev_buf == NULL) || (dev_cnt == NULL)) {
        DEVDRV_DRV_ERR("input para is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    /* set buf to invalid value before ioctl */
    ret = memset_s(dev_buf_tmp, ASCEND_DEV_MAX_NUM, ASCEND_DEV_MAX_NUM, ASCEND_DEV_MAX_NUM);
    if (ret != EOK) {
        DEVDRV_DRV_ERR("memset_s failed, ret=%d.\n", ret);
        return DRV_ERROR_INVALID_VALUE;
    }

    drv_ioctl_param_init(&buf, (void *)dev_buf_tmp, sizeof(unsigned int) * ASCEND_DEV_MAX_NUM);
    ret = drv_common_ioctl(&buf, DEVDRV_MANAGER_GET_BOOT_DEV_ID);
    if (ret != 0) {
        DEVDRV_DRV_ERR("ioctl failed ret = %d.\n", ret);
        return ret;
    }

    for (i = 0; i < ASCEND_DEV_MAX_NUM; i++) {
        if (dev_buf_tmp[i] < ASCEND_DEV_MAX_NUM) {
            DEVDRV_DRV_INFO("online dev id is %u.\n", dev_buf_tmp[i]);
            valid_dev_num++;
        } else {
            break;
        }
    }

    if (buf_cnt < valid_dev_num) {
        DEVDRV_DRV_ERR("buf_cnt[%u] is shorter than actual dev_num[%u].\n", buf_cnt, valid_dev_num);
        return DRV_ERROR_INVALID_VALUE;
    }

    *dev_cnt = valid_dev_num;
    ret = memcpy_s(dev_buf, buf_cnt * sizeof(unsigned int),
                   dev_buf_tmp, *dev_cnt * sizeof(unsigned int));
    if (ret != EOK) {
        DEVDRV_DRV_ERR("buf_cnt[%u] is shorter than actual dev_num[%u].\n", buf_cnt, valid_dev_num);
        return DRV_ERROR_NO_DEVICE;
    }

    return DRV_ERROR_NONE;
}

drvError_t halDevOnlinePollWait(void)
{
    int ret;
    mmProcess fd = -1;
    struct pollfd pollfds;

    fd = devdrv_open_device_manager();
    if (fd_is_invalid(fd)) {
        DEVDRV_DRV_ERR("open device manager failed, fd(%d).\n", fd);
        return DRV_ERROR_INVALID_HANDLE;
    }

    pollfds.fd = fd;
    pollfds.events = POLLIN;

    do {
        ret = poll(&pollfds, 1, -1);
        if (ret < 0) {
            if (errno != EINTR) {
                DEVDRV_DRV_ERR("polling interrupted! ret = %d, errno:%d.\n", ret, errno);
                return DRV_ERROR_INVALID_HANDLE;
            }
        } else if (ret == 0) {
            DEVDRV_DRV_ERR("polling time out! ret = %d\n", ret);
            return DRV_ERROR_WAIT_TIMEOUT;
        }
    } while ((ret < 0) && (errno == EINTR));

    return DRV_ERROR_NONE;
}

#ifndef CFG_FEATURE_APM_SUPP_PID
drvError_t drvBindHostPid(struct drvBindHostpidInfo info)
{
    struct devdrv_ioctl_para_bind_host_pid ioctl_para = { 0 };
    mmIoctlBuf para_buf = { 0 };
    int ret;

    if ((info.len != PROCESS_SIGN_LENGTH) ||
        (info.cp_type >= DEVDRV_PROCESS_CPTYPE_MAX)) {
        DEVDRV_DRV_ERR("sign value or len/cp_type invalid: len:%u cp_type:%d\n", info.len, info.cp_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    ioctl_para.host_pid = info.host_pid;
    ioctl_para.chip_id = info.chip_id;
    ioctl_para.mode = info.mode;
    ioctl_para.cp_type = info.cp_type;
    ioctl_para.len = info.len;
    ioctl_para.vfid = info.vfid;
    ret = memcpy_s(ioctl_para.sign, PROCESS_SIGN_LENGTH, info.sign, info.len);
    if (ret != 0) {
        DEVDRV_DRV_ERR("memcpy_s error: %d, pid:%d, chip id:%u, cp_type:%d, mode:%d, vfid:%u.\n",
                       ret, info.host_pid, info.chip_id, info.cp_type, info.mode, info.vfid);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    para_buf.inbuf = (void *)&ioctl_para;
    para_buf.inbufLen = sizeof(struct devdrv_ioctl_para_bind_host_pid);
    para_buf.outbuf = para_buf.inbuf;
    para_buf.outbufLen = para_buf.inbufLen;
    para_buf.oa = NULL;

    ret = drv_common_ioctl(&para_buf, DEVDRV_MANAGER_BIND_PID_ID);
    if (ret != 0) {
        if (ret == DRV_ERROR_NOT_SUPPORT) {
            return ret;
        }
        DEVDRV_DRV_ERR("Ioctl error. (ret=%d, pid=%d, chip id=%u, cp_type=%d, mode=%d)\n",
                       ret, info.host_pid, info.chip_id, info.cp_type, info.mode);
        return DRV_ERROR_IOCRL_FAIL;
    }

    return DRV_ERROR_NONE;
}

drvError_t drvUnbindHostPid(struct drvBindHostpidInfo info)
{
#ifndef CFG_FEATURE_HOST_UNBIND
    (void)info;
    return DRV_ERROR_NOT_SUPPORT;
#else
    struct devdrv_ioctl_para_bind_host_pid ioctl_para = { 0 };
    mmIoctlBuf para_buf = { 0 };
    int ret;

    if ((info.len != PROCESS_SIGN_LENGTH) ||
        (info.cp_type != DEVDRV_PROCESS_QS)) {
        DEVDRV_DRV_ERR("sign value or len/cp_type invalid: len:%u cp_type:%d\n", info.len, info.cp_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    ioctl_para.host_pid = info.host_pid;
    ioctl_para.chip_id = info.chip_id;
    ioctl_para.mode = info.mode;
    ioctl_para.cp_type = info.cp_type;
    ioctl_para.len = info.len;
    ioctl_para.vfid = info.vfid;
    ret = memcpy_s(ioctl_para.sign, PROCESS_SIGN_LENGTH, info.sign, info.len);
    if (ret != 0) {
        DEVDRV_DRV_ERR("memcpy_s ret: %d, pid:%d, chip_id:%u, cp_type:%d, mode:%d, vfid:%u.\n",
                       ret, info.host_pid, info.chip_id, info.cp_type, info.mode, info.vfid);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    para_buf.inbuf = (void *)&ioctl_para;
    para_buf.inbufLen = sizeof(struct devdrv_ioctl_para_bind_host_pid);
    para_buf.outbuf = para_buf.inbuf;
    para_buf.outbufLen = para_buf.inbufLen;
    para_buf.oa = NULL;

    ret = drv_common_ioctl(&para_buf, DEVDRV_MANAGER_UNBIND_PID_ID);
    if (ret != 0) {
        DEVDRV_DRV_ERR("ioctl failed, ret(%d), pid:%d, chip_id:%u, cp_type:%d, mode:%d.\n",
                       ret, info.host_pid, info.chip_id, info.cp_type, info.mode);
        return DRV_ERROR_IOCRL_FAIL;
    }

    return DRV_ERROR_NONE;
#endif
}

drvError_t halQueryDevpid(struct halQueryDevpidInfo info, pid_t *dev_pid)
{
    int ret;
    struct devdrv_ioctl_para_query_pid ioctl_para = { 0 };
    mmIoctlBuf para_buf = { 0 };
    mmProcess fd = -1;

    if (info.proc_type == DEVDRV_PROCESS_USER) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (info.proc_type >= DEVDRV_PROCESS_CPTYPE_MAX || dev_pid == NULL) {
        DEVDRV_DRV_ERR("proc_type invalid: proc_type:%d or dev_pid is NULL\n", info.proc_type);
        return DRV_ERROR_INVALID_VALUE;
    }
    fd = devdrv_open_device_manager();
    if (fd_is_invalid(fd)) {
        DEVDRV_DRV_ERR("open device error, hostpid:%d, devid:%u, proc_type:%d\n", info.hostpid,
            info.devid, info.proc_type);
        return DRV_ERROR_INVALID_DEVICE;
    }

    ioctl_para.host_pid = info.hostpid;
    ioctl_para.vfid = info.vfid;
    ioctl_para.chip_id = info.devid;
    ioctl_para.cp_type = info.proc_type;

    para_buf.oa = NULL;
    para_buf.inbuf = (void *)&ioctl_para;
    para_buf.inbufLen = sizeof(struct devdrv_ioctl_para_query_pid);
    para_buf.outbuf = para_buf.inbuf;
    para_buf.outbufLen = para_buf.inbufLen;

    ret = dmanage_mmIoctl(fd, DEVDRV_MANAGER_QUERY_DEV_PID, &para_buf);
    if (ret != 0) {
        /* ES, HDC perform the next query based on this return code */
        if (ret == DRV_ERROR_NO_PROCESS) {
            DEVDRV_DRV_WARN("This host pid didn't bind this type device pid. (host_pid=%d)\n", info.hostpid);
            return ret;
        }
        DEVDRV_DRV_WARN("ioctl or query warn, errno(%d), hostpid:%d, devid:%u, proc_type:%d\n", errno,
            info.hostpid, info.devid, info.proc_type);
        return DRV_ERROR_IOCRL_FAIL;
    }

    *dev_pid = ioctl_para.pid;

    return DRV_ERROR_NONE;
}

drvError_t drvQueryProcessHostPid(int pid, unsigned int *chip_id, unsigned int *vfid,
    unsigned int *host_pid, unsigned int *cp_type)
{
    int ret, err_buf;
    struct devdrv_ioctl_para_query_pid ioctl_para = { 0 };
    mmIoctlBuf para_buf = { 0 };
    mmProcess fd = -1;

    if (pid <= 0) {
        DEVDRV_DRV_ERR("invalid input parameter. (pid=%d)\n", pid);
        return DRV_ERROR_INVALID_VALUE;
    }

    fd = devdrv_open_device_manager();
    if ((int)fd == -DRV_ERROR_RESOURCE_OCCUPIED) {
        DEVDRV_DRV_ERR("open device manager failed, device is busy.\n");
        return DRV_ERROR_RESOURCE_OCCUPIED;
    }
    if (fd_is_invalid(fd)) {
        DEVDRV_DRV_ERR("Open device manager failed. (fd=%d)\n", fd);
        return DRV_ERROR_INVALID_HANDLE;
    }

    ioctl_para.pid = pid;

    para_buf.inbuf = (void *)&ioctl_para;
    para_buf.outbuf = para_buf.inbuf;
    para_buf.inbufLen = sizeof(struct devdrv_ioctl_para_query_pid);
    para_buf.outbufLen = para_buf.inbufLen;
    para_buf.oa = NULL;

    ret = dmanage_mmIoctl(fd, DEVDRV_MANAGER_QUERY_HOST_PID, &para_buf);
    if (ret != 0) {
        err_buf = errno;
        if (err_buf == ESRCH) {
            DEVDRV_DRV_DEBUG("Can not find hostpid. (ret=%d; pid=%d)\n", ret, pid);
            return DRV_ERROR_NO_PROCESS;
        } else if (err_buf == EPERM) {
            return DRV_ERROR_IOCRL_FAIL;
        } else if (err_buf == EOPNOTSUPP) {
            return DRV_ERROR_NOT_SUPPORT;
        }
        DEVDRV_DRV_ERR("Ioctl failed. (errno=%d; pid=%d)\n", errno, pid);
        return DRV_ERROR_IOCRL_FAIL;
    }

    if (chip_id != NULL) {
        *chip_id = ioctl_para.chip_id;
    }

    if (vfid != NULL) {
        *vfid = ioctl_para.vfid;
    }

    if (host_pid != NULL) {
        *host_pid = ioctl_para.host_pid;
    }

    if (cp_type != NULL) {
        *cp_type = ioctl_para.cp_type;
    }

    return DRV_ERROR_NONE;
}
#endif

drvError_t drvMngGetConsoleLogLevel(unsigned int *log_level)
{
    int error = 0;
    mmProcess fd;
    int ret;

    fd = devdrv_open_device_manager();
    if (fd_is_invalid(fd)) {
        DEVDRV_DRV_ERR("open device manager failed, fd = %d.\n", fd);
        return DRV_ERROR_INVALID_HANDLE;
    }

    ret = ioctl(fd, DEVDRV_MANAGER_GET_CONSOLE_LOG_LEVEL, log_level);
    if (ret != 0) {
        if (__errno_location() != NULL) {
            error = errno;
            DEVDRV_DRV_ERR("ioctl error, errno = %d.\n", error);
        }

        if (error == ERESTARTSYS) {
            return ERESTARTSYS;
        }

        return DRV_ERROR_IOCRL_FAIL;
    }

    return ret;
}

drvError_t drvGetDeviceStatus(unsigned int devId, unsigned int *device_status)
{
    int ret;
    mmIoctlBuf para_buf = {0};
    struct devdrv_device_work_status device_work_status = {0};

    if (devId >= ASCEND_DEV_MAX_NUM || device_status == NULL) {
        DEVDRV_DRV_ERR("Parameter is invalid. (devId=%u; status_is_null=%d)\n", devId, (device_status == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    device_work_status.device_id = devId;
    drv_ioctl_param_init(&para_buf, (void *)&device_work_status, sizeof(struct devdrv_device_work_status));
    ret = drv_common_ioctl(&para_buf, DEVDRV_MANAGER_GET_STARTUP_STATUS);
    if (ret != 0) {
        DEVDRV_DRV_ERR("Ioctl failed. (dev_id=%u; ret=%d)\n", devId, ret);
        return DRV_ERROR_IOCRL_FAIL;
    }

    *device_status = device_work_status.device_process_status;

    return DRV_ERROR_NONE;
}

drvError_t drvUpdateDeviceStatus(unsigned int dev_id, unsigned int device_status)
{
    int ret;
    mmIoctlBuf para_buf = {0};
    struct devdrv_device_work_status device_work_status = {0};
    device_work_status.device_id = dev_id;
    device_work_status.device_process_status = device_status;
    drv_ioctl_param_init(&para_buf, (void *)&device_work_status, sizeof(struct devdrv_device_work_status));
    ret = drv_common_ioctl(&para_buf, DEVDRV_MANAGER_UPDATE_STARTUP_STATUS);
    if (ret != 0) {
        DEVDRV_DRV_ERR("ioctl error, ret =%d\n", ret);
        return DRV_ERROR_IOCRL_FAIL;
    }

    return DRV_ERROR_NONE;
}

drvError_t drvDeviceHealthStatus(uint32_t devId, unsigned int *health_status)
{
    int ret;
    mmIoctlBuf para_buf = {0};
    struct devdrv_device_health_status para = {0};

    if (devId >= ASCEND_DEV_MAX_NUM || health_status == NULL) {
        DEVDRV_DRV_ERR("Parameter is invalid. (devId=%u; status_is_null=%d)\n", devId, (health_status == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    para.device_id = devId;
    drv_ioctl_param_init(&para_buf, (void *)&para, sizeof(struct devdrv_device_health_status));
    ret = drv_common_ioctl(&para_buf, DEVDRV_MANAGER_GET_DEVICE_HEALTH_STATUS);
    if (ret != 0) {
        DEVDRV_DRV_ERR("ioctl error, ret =%d\n", ret);
        return DRV_ERROR_IOCRL_FAIL;
    }

    *health_status = para.device_health_status;

    return DRV_ERROR_NONE;
}

drvError_t drvGetDmpStarted(uint32_t devId, uint32_t *dmp_started)
{
    int ret;
    mmIoctlBuf para_buf = {0};
    struct devdrv_device_work_status device_work_status = {0};

    if (devId >= ASCEND_DEV_MAX_NUM || dmp_started == NULL) {
        DEVDRV_DRV_ERR("Parameter is invalid. (devId=%u, status_is_null=%d)\n", devId, (dmp_started == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    device_work_status.device_id = devId;
    drv_ioctl_param_init(&para_buf, (void *)&device_work_status, sizeof(struct devdrv_device_work_status));
    ret = drv_common_ioctl(&para_buf, DEVDRV_MANAGER_GET_STARTUP_STATUS);
    if (ret != 0) {
        DEVDRV_DRV_ERR("ioctl error, devid=%u ret=%d\n", devId, ret);
        return DRV_ERROR_IOCRL_FAIL;
    }

    *dmp_started = device_work_status.dmp_started;

    return DRV_ERROR_NONE;
}

drvError_t halGetChipCount(int *chip_count)
{
#ifdef CFG_FEATURE_CHIP_DIE
    int ret;
    int count = 0;
    struct urd_cmd cmd = {0};
    struct urd_cmd_para cmd_para = {0};

    if (chip_count == NULL) {
        DEVDRV_DRV_ERR("Chip count is null.\n");
        return DRV_ERROR_PARA_ERROR;
    }

    urd_usr_cmd_fill(&cmd, DMS_MAIN_CMD_BASIC, DMS_SUBCMD_GET_CHIP_COUNT, NULL, 0);
    urd_usr_cmd_para_fill(&cmd_para, NULL, 0, (void *)&count, sizeof(int));
    ret = urd_usr_cmd(&cmd, &cmd_para);
    if (ret == DRV_ERROR_RESOURCE_OCCUPIED) {
        DEVDRV_DRV_ERR("Ioctl failed, device is busy. (ret=%d)\n", ret);
        return ret;
    }
    if (ret != 0) {
        DEVDRV_DRV_ERR("Ioctl failed. (ret=%d)\n", ret);
        return DRV_ERROR_IOCRL_FAIL;
    }

    if ((count < 0) || (count > DEVDRV_MAX_CHIP_NUM)) {
        DEVDRV_DRV_ERR("Chip count is invalid. (count=%u).\n", count);
        return DRV_ERROR_INNER_ERR;
    }

    *chip_count = count;
    return DRV_ERROR_NONE;
#else
    (void)chip_count;
    return DRV_ERROR_NOT_SUPPORT;
#endif
}

drvError_t halGetChipList(int chip_list[], int count)
{
#ifdef CFG_FEATURE_CHIP_DIE
    int ret, i;
    struct urd_cmd cmd = {0};
    struct urd_cmd_para cmd_para = {0};
    struct devdrv_chip_list chip_list_info = {0};

    if ((count <= 0) || (count > DEVDRV_MAX_CHIP_NUM) || (chip_list == NULL)) {
        DEVDRV_DRV_ERR("Input chip para error. (count=%d; chip_list_is_null=%d)\n", count, chip_list == NULL);
        return DRV_ERROR_PARA_ERROR;
    }

    urd_usr_cmd_fill(&cmd, DMS_MAIN_CMD_BASIC, DMS_SUBCMD_GET_CHIP_LIST, NULL, 0);
    urd_usr_cmd_para_fill(&cmd_para, NULL, 0, (void *)&chip_list_info, sizeof(struct devdrv_chip_list));
    ret = urd_usr_cmd(&cmd, &cmd_para);

    if (ret == DRV_ERROR_RESOURCE_OCCUPIED) {
        DEVDRV_DRV_ERR("Ioctl failed, device is busy. (ret=%d)\n", ret);
        return ret;
    }
    if (ret != 0) {
        DEVDRV_DRV_ERR("Ioctl failed. (ret=%d)\n", ret);
        return DRV_ERROR_IOCRL_FAIL;
    }

    if ((chip_list_info.count <= 0) || (chip_list_info.count > count)) {
        DEVDRV_DRV_ERR("Chip count is invalid. (count=%u)\n", chip_list_info.count);
        return DRV_ERROR_INNER_ERR;
    }

    ret = memcpy_s(chip_list, count * sizeof(int), chip_list_info.chip_list, chip_list_info.count * sizeof(int));
    if (ret != EOK) {
        DEVDRV_DRV_ERR("Memcpy_s fail. (ret=%d)\n", ret);
        return DRV_ERROR_INNER_ERR;
    }

    for (i = 0; i < count; i++) {
        DEVDRV_DRV_DEBUG("Chip list. (list[%d]=%d)\n", i, chip_list[i]);
    }

    return DRV_ERROR_NONE;
#else
    (void)chip_list;
    (void)count;
    return DRV_ERROR_NOT_SUPPORT;
#endif
}

drvError_t halGetDeviceCountFromChip(int chip_id, int *device_count)
{
#ifdef CFG_FEATURE_CHIP_DIE
    int ret;
    struct urd_cmd cmd = {0};
    struct urd_cmd_para cmd_para = {0};
    struct devdrv_chip_dev_list chip_dev_list = {0};

    if ((chip_id < 0) || (chip_id >= DEVDRV_MAX_CHIP_NUM) || (device_count == NULL)) {
        DEVDRV_DRV_ERR("Invalid Parameter. (chip_id=%d; device_count_is_null=%d)\n", chip_id, device_count == NULL);
        return DRV_ERROR_PARA_ERROR;
    }

    chip_dev_list.chip_id = chip_id;
    urd_usr_cmd_fill(&cmd, DMS_MAIN_CMD_BASIC, DMS_SUBCMD_GET_DEVICE_FROM_CHIP, NULL, 0);
    urd_usr_cmd_para_fill(&cmd_para, (void *)&chip_dev_list.chip_id, sizeof(int),
        (void *)&chip_dev_list, sizeof(struct devdrv_chip_dev_list));
    ret = urd_usr_cmd(&cmd, &cmd_para);
    if (ret == DRV_ERROR_RESOURCE_OCCUPIED) {
        DEVDRV_DRV_ERR("Ioctl failed, device is busy. (ret=%d)\n", ret);
        return ret;
    }
    if (ret != 0) {
        DEVDRV_DRV_ERR("Ioctl failed. (ret=%d)\n", ret);
        return DRV_ERROR_IOCRL_FAIL;
    }

    if ((chip_dev_list.count <= 0) || (chip_dev_list.count > ASCEND_DEV_MAX_NUM)) {
        DEVDRV_DRV_ERR("Device count is invalid. (Device count=%u)\n", chip_dev_list.count);
        return DRV_ERROR_INNER_ERR;
    }

    *device_count = chip_dev_list.count;
    DEVDRV_DRV_DEBUG("Device count. (count=%d)\n", *device_count);
    return DRV_ERROR_NONE;
#else
    (void)chip_id;
    (void)device_count;
    return DRV_ERROR_NOT_SUPPORT;
#endif
}

drvError_t halGetDeviceFromChip(int chip_id, int device_list[], int count)
{
#ifdef CFG_FEATURE_CHIP_DIE
    int ret, i;
    struct urd_cmd cmd = {0};
    struct urd_cmd_para cmd_para = {0};
    struct devdrv_chip_dev_list chip_dev_list = {0};

    if ((device_list == NULL) || (chip_id < 0) || (chip_id >= DEVDRV_MAX_CHIP_NUM) || (count <= 0)) {
        DEVDRV_DRV_ERR("Input chip id or count or device list err. (chip id=%d; count=%d; device_list_is_null=%d)\n",
            chip_id, count, device_list == NULL);
        return DRV_ERROR_PARA_ERROR;
    }

    chip_dev_list.chip_id = chip_id;
    urd_usr_cmd_fill(&cmd, DMS_MAIN_CMD_BASIC, DMS_SUBCMD_GET_DEVICE_FROM_CHIP, NULL, 0);
    urd_usr_cmd_para_fill(&cmd_para, (void *)&chip_dev_list.chip_id, sizeof(int),
        (void *)&chip_dev_list, sizeof(struct devdrv_chip_dev_list));
    ret = urd_usr_cmd(&cmd, &cmd_para);
    if (ret == DRV_ERROR_RESOURCE_OCCUPIED) {
        DEVDRV_DRV_ERR("Ioctl failed, device is busy.(ret=%d)\n", ret);
        return ret;
    }

    if (ret != 0) {
        DEVDRV_DRV_ERR("Ioctl failed. (ret=%d)\n", ret);
        return DRV_ERROR_IOCRL_FAIL;
    }

    if ((chip_dev_list.count <= 0) || (chip_dev_list.count > ASCEND_DEV_MAX_NUM) || (chip_dev_list.count > count)) {
        DEVDRV_DRV_ERR("Device count is invalid. (Device count=%u)\n", chip_dev_list.count);
        return DRV_ERROR_INNER_ERR;
    }

    ret = memcpy_s(device_list, count * sizeof(int), chip_dev_list.dev_list, chip_dev_list.count * sizeof(int));
    if (ret != EOK) {
        DEVDRV_DRV_ERR("Memcpy_s fail. (ret=%d)\n", ret);
        return DRV_ERROR_INNER_ERR;
    }

    for (i = 0; i < chip_dev_list.count; i++) {
        DEVDRV_DRV_DEBUG("Device list[%d] is %d.\n", i, device_list[i]);
    }

    return DRV_ERROR_NONE;
#else
    (void)chip_id;
    (void)device_list;
    (void)count;
    return DRV_ERROR_NOT_SUPPORT;
#endif
}

drvError_t halGetChipFromDevice(int device_id, int *chip_id)
{
#ifdef CFG_FEATURE_CHIP_DIE
    int ret;
    struct urd_cmd cmd = {0};
    struct urd_cmd_para cmd_para = {0};
    struct devdrv_get_dev_chip_id chip_id_from_dev = {0};

    if ((chip_id == NULL) || (device_id < 0) || (device_id >= ASCEND_DEV_MAX_NUM)) {
        DEVDRV_DRV_ERR("Input Dev_id or chip_id invalid. (device_id=%d; chip_id_is_null=%d)\n",
            device_id, (chip_id == NULL));
        return DRV_ERROR_PARA_ERROR;
    }

    chip_id_from_dev.dev_id = device_id;
    urd_usr_cmd_fill(&cmd, DMS_MAIN_CMD_BASIC, DMS_SUBCMD_GET_CHIP_FROM_DEVICE, NULL, 0);
    urd_usr_cmd_para_fill(&cmd_para, (void *)&chip_id_from_dev, sizeof(struct devdrv_get_dev_chip_id),
        (void *)&chip_id_from_dev, sizeof(struct devdrv_get_dev_chip_id));
    ret = urd_usr_cmd(&cmd, &cmd_para);
    if (ret == DRV_ERROR_RESOURCE_OCCUPIED) {
        DEVDRV_DRV_ERR("Ioctl failed, device is busy. (ret=%d)\n", ret);
        return ret;
    }
    if (ret != 0) {
        DEVDRV_DRV_ERR("Ioctl failed. (ret=%d)\n", ret);
        return DRV_ERROR_IOCRL_FAIL;
    }

    if ((chip_id_from_dev.chip_id < 0) || (chip_id_from_dev.chip_id >= DEVDRV_MAX_CHIP_NUM)) {
        DEVDRV_DRV_ERR("Output invalid. (device_id=%d; chip_id=%d)\n", device_id, chip_id_from_dev.chip_id);
        return ret;
    }

    *chip_id = chip_id_from_dev.chip_id;
    DEVDRV_DRV_DEBUG("Get chip_id. (device_id=%d; chip_id=%d)\n", device_id, *chip_id);
    return DRV_ERROR_NONE;
#else
    (void)device_id;
    (void)chip_id;
    return DRV_ERROR_NOT_SUPPORT;
#endif
}

STATIC drvError_t hal_get_chip_from_device_cmd(unsigned int dev_id, unsigned int main_cmd,
    unsigned int sub_cmd, void *buf, unsigned int *size)
{
    int ret;
    (void)main_cmd;
    (void)sub_cmd;

    if (*size < sizeof(int)) {
        DEVDRV_DRV_ERR("Parameter is invalid. (dev_id=%u; size=%u)\n", dev_id, *size);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = halGetChipFromDevice(dev_id, (int *)buf);
    if (ret != 0) {
        return ret;
    }

    *size = sizeof(int);
    return DRV_ERROR_NONE;
}

STATIC drvError_t hal_get_ub_status_cmd(unsigned int dev_id, unsigned int main_cmd,
    unsigned int sub_cmd, void *buf, unsigned int *size)
{
    (void)main_cmd;
    (void)sub_cmd;
    return halGetDeviceInfoByBuff(dev_id, MODULE_TYPE_UB, INFO_TYPE_UB_STATUS, buf, (int32_t *)size);
}

STATIC drvError_t hal_get_uuid_cmd(unsigned int dev_id, unsigned int main_cmd,
    unsigned int sub_cmd, void *buf, unsigned int *size)
{
    return halGetDeviceInfoByBuff(dev_id, MODULE_TYPE_SYSTEM, INFO_TYPE_UUID, buf, (int32_t *)size);
}

#ifdef CFG_FEATURE_FFTS
STATIC drvError_t hal_get_device_info_cmd(unsigned int dev_id, unsigned int main_cmd,
    unsigned int sub_cmd, void *buf, unsigned int *size)
{
    int ret;
    int64_t val;
    (void)main_cmd;
    (void)sub_cmd;

    if (*size < sizeof(unsigned int)) {
        DEVDRV_DRV_ERR("Parameter is invalid. (dev_id=%u; size=%u)\n", dev_id, *size);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = halGetDeviceInfo(dev_id, MODULE_TYPE_TSCPU, INFO_TYPE_FFTS_TYPE, &val);
    if (ret != 0) {
        DEVDRV_DRV_ERR("halGetDeviceInfo is fail. (dev_id=%u; ret=%d)\n", dev_id, ret);
        return ret;
    }

    *(unsigned int *)buf = (unsigned int)val;
    *size = sizeof(unsigned int);
    return DRV_ERROR_NONE;
}
#endif

STATIC drvError_t drv_get_svm_vdev_info_cmd(unsigned int dev_id, unsigned int main_cmd,
    unsigned int sub_cmd, void *buf, unsigned int *size)
{
    (void)main_cmd;
    return drvGetSvmVdevInfo(dev_id, sub_cmd, buf, size);
}

drvError_t drvQueryDeviceInfo(unsigned int dev_id, unsigned int main_cmd,
    unsigned int sub_cmd, void *buf, unsigned int *size)
{
    drv_query_device_info_cmd query_info_cmd_func[] = {
        {DSMI_MAIN_CMD_CHIP_INF, DSMI_CHIP_INF_SUB_CMD_CHIP_ID, hal_get_chip_from_device_cmd},
        {DSMI_MAIN_CMD_UB, DSMI_UB_INFO_SUB_CMD_PORT_STATUS, hal_get_ub_status_cmd},
#ifdef CFG_FEATURE_FFTS
        {DSMI_MAIN_CMD_TS, DSMI_TS_SUB_CMD_FFTS_TYPE, hal_get_device_info_cmd},
#endif
        {DSMI_MAIN_CMD_SVM, DSMI_GET_VDEV_CONVERT_LEN, drv_get_svm_vdev_info_cmd},
        {DSMI_MAIN_CMD_VDEV_MNG, DSMI_VMNG_SUB_CMD_GET_VDEV_RESOURCE, drvGetSingleVdevInfo},
        {DSMI_MAIN_CMD_VDEV_MNG, DSMI_VMNG_SUB_CMD_GET_TOTAL_RESOURCE, drvGetVdevTotalInfo},
        {DSMI_MAIN_CMD_VDEV_MNG, DSMI_VMNG_SUB_CMD_GET_FREE_RESOURCE, drvGetVdevFreeInfo},
        {DSMI_MAIN_CMD_VDEV_MNG, DSMI_VMNG_SUB_CMD_GET_VDEV_ACTIVITY, drvGetVdevActivityInfo},
#ifdef CFG_FEATURE_VFIO_SOC
        {DSMI_MAIN_CMD_VDEV_MNG, DSMI_VMNG_SUB_CMD_GET_TOPS_PERCENTAGE, drvGetVdevTopsPercentage},
#endif
#if (defined DRV_HOST) && (defined CFG_FEATURE_HOST_AICPU)
        {DSMI_MAIN_CMD_HOST_AICPU, DSMI_SUB_CMD_HOST_AICPU_INFO, DmsGetHostAicpuInfo},
        {DSMI_MAIN_CMD_HCCS, DSMI_HCCS_CMD_GET_CREDIT_INFO, DmsGetDeviceInfoEx},
        {DSMI_MAIN_CMD_TS, DSMI_TS_SUB_CMD_COMMON_MSG, DmsGetTsInfoEx},
#endif
        {DSMI_MAIN_CMD_CHIP_INF, DSMI_CHIP_INF_SUB_CMD_SPOD_INFO, dms_get_spod_info},
        {DSMI_MAIN_CMD_HCCS, DSMI_HCCS_CMD_GET_PING_INFO, dms_get_spod_ping_info},
#if (defined DRV_HOST)
        {DSMI_MAIN_CMD_CHIP_INF, DSMI_CHIP_INF_SUB_CMD_SPOD_NODE_STATUS, dms_get_spod_node_status},
#endif
        {DSMI_MAIN_CMD_TRS, DSMI_TRS_SUB_CMD_KERNEL_LAUNCH_MODE, DmsGetTrsMode},
        {DSMI_MAIN_CMD_CHIP_INF, DSMI_CHIP_INF_SUB_CMD_UUID, hal_get_uuid_cmd},
    };

    unsigned int i;
    unsigned int query_cmd_max;

    if ((buf == NULL) || (size == NULL) || (dev_id >= ASCEND_DEV_MAX_NUM)) {
        DEVDRV_DRV_ERR("Parameter is invalid. (dev_id=%u; buf_is_null=%d; size_is_null=%d)\n",
            dev_id, (buf == NULL), (size == NULL));
        return DRV_ERROR_PARA_ERROR;
    }

    query_cmd_max = sizeof(query_info_cmd_func) / sizeof(drv_query_device_info_cmd);

    for (i = 0; i < query_cmd_max; i++) {
        if ((query_info_cmd_func[i].main_cmd_type == main_cmd) && (query_info_cmd_func[i].sub_cmd_type == sub_cmd)) {
            if (query_info_cmd_func[i].callback != NULL) {
                return query_info_cmd_func[i].callback(dev_id, main_cmd, sub_cmd, buf, size);
            }
            break;
        }
    }
    return DRV_ERROR_NOT_SUPPORT;
}

drvError_t drvSetDeviceInfo(unsigned int dev_id, unsigned int main_cmd,
    unsigned int sub_cmd, const void *buf, unsigned int size)
{
    unsigned int i;
    unsigned int query_cmd_max;
    drv_set_device_info_cmd set_info_cmd_func[] = {
        {DSMI_MAIN_CMD_SVM, DSMI_SET_VDEV_CONVERT_LEN, drvSetSvmVdevInfo},
#ifdef CFG_FEATURE_SRIOV
        {DSMI_MAIN_CMD_VDEV_MNG, DSMI_VMNG_SUB_CMD_SET_SRIOV_SWITCH, dms_set_sriov_switch},
#endif
#if (defined DRV_HOST) && (defined CFG_FEATURE_HOST_AICPU)
        {DSMI_MAIN_CMD_HOST_AICPU, DSMI_SUB_CMD_HOST_AICPU_INFO, DmsSetHostAicpuInfo},
        {DSMI_MAIN_CMD_TS, DSMI_TS_SUB_CMD_COMMON_MSG, DmsSetTsInfo},
#endif
#if (defined DRV_HOST)
        {DSMI_MAIN_CMD_CHIP_INF, DSMI_CHIP_INF_SUB_CMD_SPOD_NODE_STATUS, dms_set_spod_node_status},
#endif
    };

    query_cmd_max = sizeof(set_info_cmd_func) / sizeof(drv_set_device_info_cmd);
    for (i = 0; i < query_cmd_max; i++) {
        if ((set_info_cmd_func[i].main_cmd_type == main_cmd) && (set_info_cmd_func[i].sub_cmd_type == sub_cmd)) {
            #ifndef DEVDRV_UT
            return set_info_cmd_func[i].callback(dev_id, sub_cmd, buf, size);
            #endif
        }
    }

    return DRV_ERROR_NOT_SUPPORT;
}

drvError_t drvSetDeviceInfoToDmsHal(unsigned int dev_id, unsigned int main_cmd, unsigned int sub_cmd,
    const void *buf, unsigned int size)
{
    int module_type = -1;
    int info_type = -1;

    if (buf == NULL || size == 0) {
        DEVDRV_DRV_ERR("Invaild parameter. (dev_id=%u; buf_is_null=%d; size=%u)\n", dev_id, (buf == NULL), size);
        return DRV_ERROR_PARA_ERROR;
    }

    if ((main_cmd == DSMI_MAIN_CMD_SOC_INFO) && (sub_cmd == DSMI_SOC_INFO_SUB_CMD_CUST_OP_ENHANCE)) {
        module_type = MODULE_TYPE_SYSTEM;
        info_type = INFO_TYPE_CUST_OP_ENHANCE;
    } else {
        return DRV_ERROR_NOT_SUPPORT;
    }

    return DmsHalSetDeviceInfoEx(dev_id, module_type, info_type, buf, size);
}

drvError_t drvGetCommonDriverInitStatus(int *status)
{
    static int davinci_intf_status = DAVINCI_INTF_INVALID_STATUS;
    if (status == NULL) {
        DEVDRV_DRV_ERR("invalid input argument.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }

    if (davinci_intf_status == true) {
        *status = davinci_intf_status;
        return DRV_ERROR_NONE;
    }

    if ((dmanage_check_module_init(DAVINCI_COMMON_DRV_NAME) == 0) ||
        (dmanage_check_module_init(DAVINCI_COMMON_VDRV_NAME) == 0)) {
        davinci_intf_status = true;
    } else {
        davinci_intf_status = false;
    }

    *status = davinci_intf_status;
    return DRV_ERROR_NONE;
}

int halRegisterVmngClient(void)
{
#if (defined CFG_SOC_PLATFORM_MINIV2) || (defined CFG_SOC_PLATFORM_CLOUD)
    mmIoctlBuf buf = {0};
    int ret;
    int para;

    drv_ioctl_param_init(&buf, (void *)&para, sizeof(int));
    ret = drv_common_ioctl(&buf, DEVDRV_MANAGER_REG_VMNG_CLIENT);
    if (ret != 0) {
        DEVDRV_DRV_ERR("ioctl failed ret = %d.\n", ret);
        return ret;
    }
#endif

    return DRV_ERROR_NONE;
}

int halGetDeviceVfMax(unsigned int devId, unsigned int *vf_max_num)
{
    mmIoctlBuf buf = {0};
    unsigned int para;
    int ret;

    if ((vf_max_num == NULL) || drvCheckDevid(devId)) {
        DEVDRV_DRV_ERR("Parameter is invalid. (dev_id=%u)\n", devId);
        return DRV_ERROR_PARA_ERROR;
    }

    para = devId;
    drv_ioctl_param_init(&buf, (void *)&para, sizeof(unsigned int));
    ret = drv_common_ioctl(&buf, DEVDRV_MANAGER_GET_DEVICE_VF_MAX);
    if (ret != 0) {
        DEVDRV_DRV_ERR("Ioctl failed. (ret=%d)\n", ret);
        return ret;
    }

    *vf_max_num = para;

    return DRV_ERROR_NONE;
}

int halGetDeviceVfList(unsigned int devId, unsigned int *vf_list, unsigned int list_len, unsigned int *vf_num)
{
    struct virtmng_vf_list get_vf_list = {0};
    mmIoctlBuf buf = {0};
    int ret;
    u32 i;

    if ((vf_list == NULL) || (vf_num == NULL) || drvCheckDevid(devId)) {
        DEVDRV_DRV_ERR("Parameter is invalid. (dev_id=%u; vf_list=%s; vf_num=%s)\n",
                       devId, vf_list == NULL ? "NULL" : "OK", vf_num == NULL ? "NULL" : "OK");
        return DRV_ERROR_PARA_ERROR;
    }

    get_vf_list.dev_id = devId;
    drv_ioctl_param_init(&buf, (void *)&get_vf_list, sizeof(struct virtmng_vf_list));
    ret = drv_common_ioctl(&buf, DEVDRV_MANAGER_GET_DEVICE_VF_LIST);
    if (ret != 0) {
        DEVDRV_DRV_ERR("Ioctl failed. (ret=%d)\n", ret);
        return ret;
    }

    if ((get_vf_list.vf_num > list_len) || (get_vf_list.vf_num > VDAVINCI_MAX_VFID_NUM)) {
        DEVDRV_DRV_ERR("Parameter is invalid. (dev_id=%u; list_len=%u; vf_num=%u)\n",
                       devId, list_len, get_vf_list.vf_num);
        return DRV_ERROR_PARA_ERROR;
    }

    for (i = 0; i < get_vf_list.vf_num; i++) {
        vf_list[i] = get_vf_list.vf_list[i];
    }
    *vf_num = get_vf_list.vf_num;

    return DRV_ERROR_NONE;
}

drvError_t halGetChipInfo(unsigned int devId, halChipInfo *chip_info)
{
    int ret;
    struct dms_query_chip_info query_info = {0};

    if (chip_info == NULL) {
        DEVDRV_DRV_ERR("Chip info is null. (dev_id=%u)\n", devId);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = (int)drvCheckDevid(devId);
    if (ret != DRV_ERROR_NONE) {
        DEVDRV_DRV_ERR("Check device id failed. (devid=%u; ret=%d)\n", devId, ret);
        if (ret == DRV_ERROR_RESOURCE_OCCUPIED) {
            return DRV_ERROR_RESOURCE_OCCUPIED;
        }
        return DRV_ERROR_PARA_ERROR;
    }

    query_info.dev_id = devId;
    ret = DmsGetChipInfo(&query_info);
    if (ret != DRV_ERROR_NONE) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Get chip info fail. (dev_id=%u)\n", devId);
        return DRV_ERROR_PARA_ERROR;
    }

    *chip_info = *(halChipInfo *)(&query_info.info);
    return DRV_ERROR_NONE;
}

drvError_t halParseSDID(uint32_t sdid, struct halSDIDParseInfo *sdid_parse)
{
    if (sdid_parse == NULL) {
        DEVDRV_DRV_ERR("SDID parse pointer is null. (sdid=%u)\n", sdid);
        return DRV_ERROR_PARA_ERROR;
    }
    return dms_parse_sdid(sdid, sdid_parse);
}

STATIC void __attribute__((constructor)) drv_manager_init(void)
{
    unsigned int container_flag = false;

#ifdef CFG_FEATURE_DRV_DEVMNG_INIT
    if (access(davinci_intf_get_dev_path(), R_OK | W_OK) != 0) {
        return;
    }
    /* davinci_manager has been opened in dmanager_get_container_flag
       have to close davinci_manager when get flag fail */
    if (dmanage_get_container_flag(&container_flag)) {
        devdrv_close_device_manager();
        DEVDRV_DRV_ERR("get container flag failed.\n");
        return;
    }
#endif
}

STATIC void __attribute__((destructor)) drv_manager_un_init(void)
{
#ifdef CFG_FEATURE_DRV_DEVMNG_INIT
    devdrv_close_device_manager();
#endif
}

drvError_t devdrv_close_restore_device_manager(void)
{
#ifdef CFG_FEATURE_DRV_DEVMNG_INIT
    devdrv_close_device_manager();
    unsigned int container_flag = false;
    if (access(davinci_intf_get_dev_path(), R_OK | W_OK) != 0) {
        return DRV_ERROR_NO_DEVICE;
    }
    /* davinci_manager has been opened in dmanager_get_container_flag
       have to close davinci_manager when get flag fail */
    if (dmanage_get_container_flag(&container_flag)) {
        devdrv_close_device_manager();
        DEVDRV_DRV_ERR("Get container flag failed.\n");
        return DRV_ERROR_OPEN_FAILED;
    }
#endif
    return DRV_ERROR_NONE;
}