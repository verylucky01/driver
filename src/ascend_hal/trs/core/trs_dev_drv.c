/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "securec.h"
#include "trs_stl.h"
#include "ascend_hal.h"
#include "davinci_interface.h"
#include "uda_inner.h"
#include "pbl_user_interface.h"
#include "pbl_urd_main_cmd_def.h"
#include "pbl_urd_sub_cmd_common.h"

#include "trs_shr_id_fd.h"
#include "trs_ioctl.h"
#include "trs_res.h"
#include "trs_sqcq.h"
#include "trs_cb_event.h"
#include "trs_user_pub_def.h"
#include "trs_master_event.h"
#include "trs_dev_drv.h"
#include "trs_user_interface.h"

#ifdef EMU_ST
int trs_file_open(const char *pathname, int flags);
int trs_file_close(int fd);
int trs_ioctl_user(int fd, unsigned long cmd, void *para);
void *trs_mmap_user(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int trs_munmap(void *addr, size_t length);

#else
#define trs_file_open open
#define trs_file_close close
#define trs_ioctl_user(fd, cmd, ...) ioctl((fd), (cmd), ##__VA_ARGS__)
#define trs_mmap_user mmap
#define trs_munmap munmap
#endif

static int trs_dev_fd[TRS_DEV_NUM] = {-1, };
static int trs_dev_mmap_fd[TRS_DEV_NUM] = {-1, };
static pthread_mutex_t trs_dev_mutex = PTHREAD_MUTEX_INITIALIZER;

struct trs_dev_init_ops g_dev_init_ops = {NULL, };
void trs_register_dev_init_ops(struct trs_dev_init_ops *ops)
{
    g_dev_init_ops = *ops;
}

static struct trs_dev_init_ops *trs_get_dev_init_ops(void)
{
    return &g_dev_init_ops;
}

signed int __attribute__((constructor)) trs_core_init_user(void)  //lint !e527
{
    int i;

    for (i = 0; i < TRS_DEV_NUM; i++) {
        trs_dev_fd[i] = -1;
        trs_dev_mmap_fd[i] = -1;
    }

    return 0;
}

static void trs_set_dev_fd(uint32_t dev_id, int fd)
{
    trs_dev_fd[dev_id] = fd;
}

static int trs_get_dev_fd(uint32_t dev_id)
{
    return trs_dev_fd[dev_id];
}

static void trs_set_dev_mmap_fd(uint32_t dev_id, int fd)
{
    trs_dev_mmap_fd[dev_id] = fd;
}

static int trs_get_dev_mmap_fd(uint32_t dev_id)
{
    return trs_dev_mmap_fd[dev_id];
}

void *trs_dev_mmap(uint32_t dev_id, void *addr, size_t length, int prot, int flags)
{
    int fd = trs_get_dev_mmap_fd(dev_id);
    if (fd < 0) {
        trs_err("Dev not open. (dev_id=%u; fd=%d)\n", dev_id, fd);
        return MAP_FAILED;
    }

    return trs_mmap_user(addr, length, prot, flags, fd, 0);
}

int trs_dev_munmap(void *addr, size_t length)
{
    return trs_munmap(addr, length);
}

#define MAX_TRS_OPEN_DEV_RETRY_TIME 2400
#define TRS_OPEN_DEV_RETRY_INTERVAL 100000 // 100ms
static int _trs_char_dev_open(uint32_t v_dev_id, int *fd, const char* intf_module_name)
{
    struct davinci_intf_open_arg arg;
    int ret, cnt = 0;
    bool retry = false;

    arg.device_id = (int)v_dev_id;
    ret = strcpy_s(arg.module_name, DAVINIC_MODULE_NAME_MAX, intf_module_name);
    if (ret < 0) {
        trs_err("Strcpy_s failed. (v_dev_id=%u).\n", v_dev_id);
        return ret;
    }

    *fd = trs_file_open(davinci_intf_get_dev_path(), O_RDWR | O_CLOEXEC);
    if (*fd < 0) {
        ret = errno;
        trs_err("Open device failed. (v_dev_id=%u; fd=%d; errno=%d, error=%s)\n", v_dev_id, *fd, errno, strerror(errno));
        return ret;
    }

    do {
        ret = trs_ioctl_user(*fd, DAVINCI_INTF_IOCTL_OPEN, &arg);
        cnt++;
        retry = ((ret != 0) && (errno == EBUSY) && (cnt < MAX_TRS_OPEN_DEV_RETRY_TIME));
        if (retry) {
            usleep(TRS_OPEN_DEV_RETRY_INTERVAL);
        }
    } while (retry);
    if (ret != 0) {
        trs_err("Ioctrl failed. (v_dev_id=%u; fd=%d; errno=%d, error=%s)\n", v_dev_id, *fd, errno, strerror(errno));
        share_log_read_err(HAL_MODULE_TYPE_TS_DRIVER);
        trs_file_close(*fd);
        return ret;
    }

    return DRV_ERROR_NONE;
}

static int trs_char_dev_open(uint32_t v_dev_id, int *fd)
{
    return _trs_char_dev_open(v_dev_id, fd, DAVINCI_INTF_MODULE_TSDRV);
}

static int trs_char_dev_mmap_open(uint32_t v_dev_id, int *fd)
{
    return _trs_char_dev_open(v_dev_id, fd, DAVINCI_INTF_MODULE_TSDRV_MMAP_NAME);
}

static int _trs_char_dev_close(uint32_t v_dev_id, int fd, const char* intf_module_name)
{
    struct davinci_intf_close_arg arg;
    int ret;

    arg.device_id = (int32_t)v_dev_id;
    ret = strcpy_s(arg.module_name, DAVINIC_MODULE_NAME_MAX, intf_module_name);
    if (ret < 0) {
        trs_warn("Strcpy_s warn. (v_dev_id=%u).\n", v_dev_id);
        return ret;
    }

    ret = trs_ioctl_user(fd, DAVINCI_INTF_IOCTL_CLOSE, &arg);
    if (ret != 0) {
        trs_warn("Ioctrl warn. (v_dev_id=%u; fd=%d; errno=%d, error=%s)\n", v_dev_id, fd, errno, strerror(errno));
        share_log_read_err(HAL_MODULE_TYPE_TS_DRIVER);
    }

    trs_file_close(fd);

    return ret;
}

static int trs_char_dev_close(uint32_t v_dev_id, int fd)
{
    return _trs_char_dev_close(v_dev_id, fd, DAVINCI_INTF_MODULE_TSDRV);
}

static int trs_char_dev_mmap_close(uint32_t v_dev_id, int fd)
{
    return _trs_char_dev_close(v_dev_id, fd, DAVINCI_INTF_MODULE_TSDRV_MMAP_NAME);
}

drvError_t __attribute__((weak)) trs_urma_proc_ctx_init_by_devid(uint32_t dev_id)
{
    (void)dev_id;
    return DRV_ERROR_NONE;
}

void __attribute__((weak)) trs_urma_proc_ctx_uninit_by_devid(uint32_t dev_id)
{
    (void)dev_id;
}

static int trs_dev_init(uint32_t dev_id)
{
    int ret = 0;

    ret = trs_hw_info_init(dev_id);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = trs_d2d_info_init(dev_id);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = trs_urma_proc_ctx_init_by_devid(dev_id);
    if (ret != 0) {
        trs_err("Failed to init urma proc ctx. (devid=%u; ret=%d)\n", dev_id, ret);
        goto urma_ctx_init_fail;
    }

    ret = trs_dev_res_id_init(dev_id);
    if (ret != DRV_ERROR_NONE) {
        trs_err("Failed to init res id. (devid=%u; ret=%d)\n", dev_id, ret);
        goto res_id_init_fail;
    }

    ret = trs_dev_sq_cq_init(dev_id);
    if (ret != DRV_ERROR_NONE) {
        trs_err("Sqcq init failed. (devid=%u; ret=%d)\n", dev_id, ret);
        ret = DRV_ERROR_INVALID_DEVICE;
        goto sqcq_init_fail;
    }

    ret = trs_cb_event_init(dev_id);
    if (ret != DRV_ERROR_NONE) {
        trs_err("Cb event init failed. (devid=%u; ret=%d)\n", dev_id, ret);
        ret = DRV_ERROR_INVALID_DEVICE;
        goto cb_event_init_fail;
    }

    if (trs_get_dev_init_ops()->dev_init != NULL) {
        ret = trs_get_dev_init_ops()->dev_init(dev_id);
        if (ret != DRV_ERROR_NONE) {
            trs_err("Dev init failed. (devid=%u; ret=%d)\n", dev_id, ret);
            goto dev_ops_init_fail;
        }
    }

    return DRV_ERROR_NONE;

dev_ops_init_fail:
    trs_cb_event_uninit(dev_id);
cb_event_init_fail:
    trs_dev_sq_cq_uninit(dev_id, DEV_CLOSE_HOST_DEVICE);
sqcq_init_fail:
    trs_dev_res_id_uninit(dev_id);
res_id_init_fail:
    trs_urma_proc_ctx_uninit_by_devid(dev_id);
urma_ctx_init_fail:
    trs_d2d_info_uninit(dev_id);
    return ret;
}

static void trs_dev_uninit(uint32_t dev_id, uint32_t close_type)
{
    if (trs_get_dev_init_ops()->dev_uninit != NULL) {
        trs_get_dev_init_ops()->dev_uninit(dev_id);
    }

    trs_cb_event_uninit(dev_id);
    trs_dev_sq_cq_uninit(dev_id, close_type);
    trs_dev_res_id_uninit(dev_id);
    trs_urma_proc_ctx_uninit_by_devid(dev_id);
    trs_d2d_info_uninit(dev_id);
}

static drvError_t drvDeviceOpenComm(void **dev_info, uint32_t dev_id)
{
    uint32_t udev_id;
    int fd, mmap_fd, ret;

    if ((dev_info == NULL) || (dev_id >= TRS_DEV_NUM)) {
        trs_err("Invalid ptr or dev_id. (dev_info_Null=%d; devid=%u; max=%d)\n", (dev_info == NULL), dev_id, TRS_DEV_NUM);
        return DRV_ERROR_INVALID_VALUE;
    }

    (void)pthread_mutex_lock(&trs_dev_mutex);
    if ((trs_get_dev_fd(dev_id) >= 0) || (trs_get_dev_mmap_fd(dev_id) >= 0)) {
        (void)pthread_mutex_unlock(&trs_dev_mutex);
        return DRV_ERROR_REPEATED_INIT;
    }

    ret = uda_get_udevid_by_devid(dev_id, &udev_id);
    if (ret != DRV_ERROR_NONE) {
        (void)pthread_mutex_unlock(&trs_dev_mutex);
        trs_err("Get phys failed. (devid=%u; ret=%d)\n", dev_id, ret);
        return DRV_ERROR_INVALID_DEVICE;
    }

    share_log_create(HAL_MODULE_TYPE_TS_DRIVER, SHARE_LOG_MAX_SIZE);
    ret = trs_char_dev_open(udev_id, &fd);
    if (ret != DRV_ERROR_NONE) {
        (void)pthread_mutex_unlock(&trs_dev_mutex);
        trs_err("Open failed. (devid=%u; ret=%d)\n", dev_id, ret);
        share_log_destroy(HAL_MODULE_TYPE_TS_DRIVER);
        return DRV_ERROR_NO_DEVICE;
    }

    ret = trs_char_dev_mmap_open(udev_id, &mmap_fd);
    if (ret != DRV_ERROR_NONE) {
        (void)trs_char_dev_close(udev_id, fd);
        (void)pthread_mutex_unlock(&trs_dev_mutex);
        share_log_destroy(HAL_MODULE_TYPE_TS_DRIVER);
#ifndef EMU_ST
        return DRV_ERROR_NO_DEVICE;
#endif
    }

    trs_set_dev_fd(dev_id, fd);
    trs_set_dev_mmap_fd(dev_id, mmap_fd);

    ret = trs_dev_init(dev_id);
    if (ret != DRV_ERROR_NONE) {
        trs_set_dev_mmap_fd(dev_id, -1);
        trs_set_dev_fd(dev_id, -1);
        (void)trs_char_dev_mmap_close(udev_id, mmap_fd);
        (void)trs_char_dev_close(udev_id, fd);
        (void)pthread_mutex_unlock(&trs_dev_mutex);
        share_log_destroy(HAL_MODULE_TYPE_TS_DRIVER);
        return ret;
    }

    ((struct drvDevInfo *)dev_info)->fd = fd;
    (void)pthread_mutex_unlock(&trs_dev_mutex);

    trs_info("Open success. (devid=%u; fd=%d; mmap_fd=%d)\n", dev_id, fd, mmap_fd);
    return DRV_ERROR_NONE;
}

drvError_t drvTrsDeviceCloseUserResInner(uint32_t dev_id, halDevCloseIn *in)
{
    shrid_set_fd(-1);
    trs_set_dev_mmap_fd(dev_id, -1);
    trs_set_dev_fd(dev_id, -1);
    share_log_destroy(HAL_MODULE_TYPE_TS_DRIVER);
    (void)in;
    return DRV_ERROR_NONE;
}

drvError_t drvDeviceOpenInner(uint32_t dev_id, halDevOpenIn *in, halDevOpenOut *out)
{
    struct drvDevInfo dev_info;
	(void)in;
    (void)out;

    return drvDeviceOpenComm((void **)(&dev_info), dev_id);
}

drvError_t drvDeviceOpen(void **devInfo, uint32_t devId)
{
    return drvDeviceOpenComm(devInfo, devId);
}

static int trs_set_close_type(uint32_t dev_id, uint32_t ts_id, uint32_t close_type)
{
    struct trs_set_close_para para;

    if (close_type != DEV_CLOSE_ONLY_HOST) {
        return DRV_ERROR_NONE;
    }

    para.close_type = close_type;
    para.tsid = ts_id;
    return trs_dev_io_ctrl(dev_id, TRS_SET_CLOSE_TYPE, &para);
}

static drvError_t drvDeviceCloseComm(uint32_t dev_id, halDevCloseIn *in)
{
    uint32_t udev_id;
    int fd, mmap_fd, ret;

    if (dev_id >= TRS_DEV_NUM) {
        trs_err("Invalid para. (devid=%u)\n", dev_id);
        return DRV_ERROR_INVALID_VALUE;
    }

    (void)pthread_mutex_lock(&trs_dev_mutex);
    fd = trs_get_dev_fd(dev_id);
    mmap_fd = trs_get_dev_mmap_fd(dev_id);
    if ((fd < 0) || (mmap_fd < 0)) {
        (void)pthread_mutex_unlock(&trs_dev_mutex);
        trs_err("Repeated close. (devid=%u)\n", dev_id);
        return DRV_ERROR_INVALID_DEVICE;
    }

    trs_dev_uninit(dev_id, in->close_type);

    ret = trs_set_close_type(dev_id, 0, in->close_type);
    if (ret != DRV_ERROR_NONE) {
        (void)pthread_mutex_unlock(&trs_dev_mutex);
        if (ret != DRV_ERROR_NOT_SUPPORT) {
            trs_err("Set close type failed. (devid=%u; ret=%d)\n", dev_id, ret);
        }
        return ret;
    }

    ret = uda_get_udevid_by_devid(dev_id, &udev_id);
    if (ret != DRV_ERROR_NONE) {
        (void)pthread_mutex_unlock(&trs_dev_mutex);
        trs_err("Get phys failed. (devid=%u)\n", dev_id);
        return DRV_ERROR_INVALID_DEVICE;
    }

    trs_set_dev_mmap_fd(dev_id, -1);
    trs_set_dev_fd(dev_id, -1);

    ret = trs_char_dev_mmap_close(udev_id, mmap_fd);
    if (ret != DRV_ERROR_NONE) {
        (void)pthread_mutex_unlock(&trs_dev_mutex);
        trs_err("Close mmap_fd failed. (devid=%u; mmap_fd=%d)\n", dev_id, mmap_fd);
        return DRV_ERROR_INVALID_DEVICE;
    }

    ret = trs_char_dev_close(udev_id, fd);
    if (ret != DRV_ERROR_NONE) {
        (void)pthread_mutex_unlock(&trs_dev_mutex);
        trs_err("Close failed. (devid=%u)\n", dev_id);
        return DRV_ERROR_INVALID_DEVICE;
    }

    (void)pthread_mutex_unlock(&trs_dev_mutex);

    share_log_destroy(HAL_MODULE_TYPE_TS_DRIVER);
    trs_info("Close success. (devid=%u; fd=%d; mmap_fd=%d)\n", dev_id, fd, mmap_fd);

    return DRV_ERROR_NONE;
}

drvError_t drvDeviceCloseInner(uint32_t dev_id, halDevCloseIn *in)
{
    halDevCloseIn close_in;

    if (in != NULL) {
        if (in->close_type >= DEV_CLOSE_TYPE_MAX) {
            trs_err("Invalid para. (close_type=%u)\n", in->close_type);
            return DRV_ERROR_INVALID_VALUE;
        }
        close_in.close_type = in->close_type;
    } else {
        close_in.close_type = DEV_CLOSE_HOST_DEVICE;
    }

    return drvDeviceCloseComm(dev_id, &close_in);
}

drvError_t drvDeviceClose(uint32_t devId)
{
    halDevCloseIn in;

    in.close_type = DEV_CLOSE_HOST_DEVICE;
    return drvDeviceCloseComm(devId, &in);
}

int trs_dev_ioctl(int fd, uint32_t cmd, void *para)
{
    int ret, errno_tmp;

    do {
        ret = trs_ioctl_user(fd, cmd, para);
    } while ((ret == -1) && (errno == EINTR));

    errno_tmp = errno;

    if (ret < 0) {
        share_log_read_err(HAL_MODULE_TYPE_TS_DRIVER);
        if (errno_tmp == ETIMEDOUT) {
            return DRV_ERROR_WAIT_TIMEOUT;
        }

        if (errno_tmp == EBUSY) {
            return DRV_ERROR_BUS_DOWN;
        }

        if (errno_tmp == ENOSPC) { /* no res */
            return DRV_ERROR_NO_RESOURCES;
        }

        if (errno_tmp == EOPNOTSUPP) {
            return DRV_ERROR_NOT_SUPPORT;
        }

        if (errno_tmp == ESHUTDOWN) {
            return DRV_ERROR_STATUS_FAIL;
        }

        trs_warn("Cmd %d warn, fd %d errno %d.\n", _IOC_NR(cmd), fd, errno_tmp);
        return DRV_ERROR_IOCRL_FAIL;
    }

    return DRV_ERROR_NONE;
}

static bool trs_can_open_alone(uint32_t cmd)
{
    return (cmd == TRS_RES_ID_NUM_QUERY) || (cmd == TRS_RES_ID_MAX_QUERY) || (cmd == TRS_RES_ID_USED_NUM_QUERY) ||
        (cmd == TRS_RES_ID_REG_OFFSET_QUERY) || (cmd == TRS_RES_ID_REG_SIZE_QUERY) || (cmd == TRS_RES_ID_ENABLE) ||
        (cmd == TRS_RES_ID_DISABLE) || (cmd == TRS_RES_ID_AVAIL_NUM_QUERY) || (cmd == TRS_TS_CMDLIST_MAP_UNMAP);
}

int trs_dev_open_ioctl(uint32_t dev_id, uint32_t cmd, void *para)
{
    uint32_t udev_id;
    int ret, fd;

    (void)pthread_mutex_lock(&trs_dev_mutex);
    fd = trs_get_dev_fd(dev_id);
    if (fd >= 0) {
        (void)pthread_mutex_unlock(&trs_dev_mutex);
        return trs_dev_ioctl(fd, cmd, para);
    }

    ret = drvDeviceGetPhyIdByIndex(dev_id, &udev_id);
    if (ret != DRV_ERROR_NONE) {
        (void)pthread_mutex_unlock(&trs_dev_mutex);
        trs_err("Get phys failed. (devid=%u)\n", dev_id);
        return DRV_ERROR_INVALID_DEVICE;
    }

    ret = trs_char_dev_open(udev_id, &fd);
    if (ret != DRV_ERROR_NONE) {
        (void)pthread_mutex_unlock(&trs_dev_mutex);
        trs_err("Open failed. (devid=%u)\n", dev_id);
        return DRV_ERROR_INVALID_DEVICE;
    }

    ret = trs_dev_ioctl(fd, cmd, para);
    if (ret != DRV_ERROR_NONE) {
        (void)trs_char_dev_close(udev_id, fd);
        (void)pthread_mutex_unlock(&trs_dev_mutex);
        return ret;
    }

    ret = trs_char_dev_close(udev_id, fd);
    if (ret != DRV_ERROR_NONE) {
        (void)pthread_mutex_unlock(&trs_dev_mutex);
        return ret;
    }
    (void)pthread_mutex_unlock(&trs_dev_mutex);

    return DRV_ERROR_NONE;
}

int trs_dev_io_ctrl(uint32_t dev_id, uint32_t cmd, void *para)
{
    int fd = trs_get_dev_fd(dev_id);
    if (fd >= 0) {
        return trs_dev_ioctl(fd, cmd, para);
    }

    if (trs_can_open_alone(cmd)) {
        return trs_dev_open_ioctl(dev_id, cmd, para);
    }

    trs_err("Dev not open. (dev_id=%u; fd=%d; cmd=%u)\n", dev_id, fd, cmd);
    return DRV_ERROR_OPEN_FAILED;
}

int trs_svm_proc_bind(void)
{
    struct trs_svm_process_info info;
    int svmfd, ret;

    svmfd = open(SVM_DEV_NAME, O_RDWR);
    if (svmfd < 0) {
        trs_err("Open svm device failed.\n");
        return DRV_ERROR_FILE_OPS;
    }

    info.flags = 0;
    ret = ioctl(svmfd, SVM_IOCTL_PROCESS_BIND, &info);
    (void)close(svmfd);
    if (ret != 0) {
        trs_err("Ioctl svm device failed. (errno=%d)\n", errno);
        return DRV_ERROR_IOCRL_FAIL;
    }

    return DRV_ERROR_NONE;
}

/* This interface must be triggered by the RTS, because the host scenario depends on the CP process startup. */
drvError_t __attribute__((weak)) drvMemSmmuQuery(uint32_t device, uint32_t *SSID)
{
    struct trs_ssid_query_para para;
    int ret = 0;

    if ((device >= TRS_DEV_NUM) || (SSID == NULL)) {
        trs_err("Invalid para. (dev_id=%u)\n", device);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = trs_dev_io_ctrl(device, TRS_SSID_QUERY, &para);
    if (ret == DRV_ERROR_NONE) {
        *SSID = (uint32_t)para.ssid;
        return DRV_ERROR_NONE;
    }

    ret = trs_svm_proc_bind();
    if (ret != 0) {
        trs_err("Proc bind failed. (ret=%d)\n", ret);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = trs_dev_io_ctrl(device, TRS_SSID_QUERY, &para);
    if (ret == DRV_ERROR_NONE) {
        *SSID = (uint32_t)para.ssid;
    }
    return ret;
}

/* Sunset interface: external statement has been removed from ascend_hal.h. */
drvError_t drvCustomCall(uint32_t devid, uint32_t cmd, void *para)
{
    unsigned int *out = (unsigned int *)((uintptr_t)para);

    (void)devid;
    (void)cmd;
    if (para == NULL) {
        trs_err("Null ptr\n");
        return DRV_ERROR_INVALID_HANDLE;
    }

    *out = 0;
    return DRV_ERROR_NONE;
}

static drvError_t dev_etrs_get_cb_group_num(void *out, size_t *out_size)
{
    if (out == NULL || out_size == NULL) {
        trs_err("Null ptr\n");
        return DRV_ERROR_INVALID_VALUE;
    }
    trs_get_cb_group_num((unsigned int *)out);
    *out_size = sizeof(unsigned int);
    return DRV_ERROR_NONE;
}

static drvError_t trs_mbox_msg_ctrl(uint32_t dev_id, void *param, size_t param_size, void *out, size_t *out_size)
{
    struct tsdrv_ctrl_msg *in = (struct tsdrv_ctrl_msg *)param;
    struct trs_ctrl_msg_para para;
    int ret;

    if (dev_id >= TRS_DEV_NUM) {
        trs_err("Invalid dev_id. (dev_id=%u)\n", dev_id);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((param == NULL) || (param_size != sizeof(struct tsdrv_ctrl_msg)) ||
        (in->msg == NULL) || (out == NULL) || (out_size == NULL)) {
        trs_err("Invalid param. (dev_id=%u; param_size=%u)\n", dev_id, param_size);
        return DRV_ERROR_INVALID_VALUE;
    }

    para.tsid = in->tsid;
    para.msg_len = in->msg_len;
    ret = memcpy_s(para.msg, sizeof(para.msg), in->msg, in->msg_len);
    if (ret != 0) {
        trs_err("Memcpy_s failed. (dev_id=%u; dest_size=%u; src_size=%u)\n", dev_id, sizeof(para.msg), in->msg_len);
        return ret;
    }

    ret = trs_dev_io_ctrl(dev_id, TRS_MSG_CTRL, &para);
    if (ret != DRV_ERROR_NONE) {
        trs_err("Ioctl failed. (dev_id=%u)\n", dev_id);
        return ret;
    }

    ret = memcpy_s(out, *out_size, para.msg, para.msg_len);
    if (ret != 0) {
        trs_err("Memcpy_s failed. (dev_id=%u; dest_size=%u; src_size=%u)\n", dev_id, *out_size, para.msg_len);
        return ret;
    }

    *out_size = para.msg_len;
    return DRV_ERROR_NONE;
}

drvError_t halTsdrvCtl(uint32_t devId, int cmd, void *param, size_t paramSize, void *out, size_t *outSize)
{
    tsDrvCtlCmdType_t cmd_type = (tsDrvCtlCmdType_t)cmd;

    switch (cmd_type) {
        case TSDRV_CTL_CMD_CB_GROUP_NUM_GET:
            return dev_etrs_get_cb_group_num(out, outSize);
        case TSDRV_CTL_CMD_BIND_STL:
            return trs_bind_stl(devId);
        case TSDRV_CTL_CMD_LAUNCH_STL:
            return trs_launch_stl(devId, param, paramSize);
        case TSDRV_CTL_CMD_QUERY_STL:
            return trs_query_stl(devId, out, outSize);
        case TSDRV_CTL_CMD_CTRL_MSG:
            return trs_mbox_msg_ctrl(devId, param, paramSize, out, outSize);
        default:
            trs_warn("Cmd_type is not support, (devid=%u, type=%d).\n", devId, cmd_type);
            return DRV_ERROR_NOT_SUPPORT;
    }
}

static drvError_t trs_mode_op_by_urd(struct trs_mode_info *info, enum urd_main_cmd urd_cmd)
{
    struct urd_cmd cmd = {0};
    struct urd_cmd_para cmd_para = {0};
    int ret = 0;

    if (info == NULL) {
        trs_err("The info para is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((info->dev_id >= TRS_DEV_NUM) || (info->ts_id >= TRS_TS_NUM) || (info->mode_type >= TRS_MODE_TYPE_MAX)) {
        trs_err("Invalid para. (dev_id=%u; ts_id=%u)\n", info->dev_id, info->ts_id, info->mode_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    urd_usr_cmd_fill(&cmd, urd_cmd, ZERO_CMD, NULL, 0);
    urd_usr_cmd_para_fill(&cmd_para, (void *)info, sizeof(struct trs_mode_info), (void *)(&info->mode), sizeof(int));

    (void)pthread_mutex_lock(&trs_dev_mutex);
    ret = urd_usr_cmd(&cmd, &cmd_para);
    if ((ret != 0) && (ret != DRV_ERROR_NOT_SUPPORT)) {
        trs_err("Op mode by urd failed. (dev_id=%u; ts_id=%u; ret=%d)\n", info->dev_id, info->ts_id, ret);
    }

    (void)pthread_mutex_unlock(&trs_dev_mutex);
    return ret;
}

drvError_t trs_mode_config(struct trs_mode_info *info)
{
    return trs_mode_op_by_urd(info, URD_TRS_MODE_CONFIG_CMD);
}

drvError_t trs_mode_query(struct trs_mode_info *info)
{
    return trs_mode_op_by_urd(info, URD_TRS_MODE_QUERY_CMD);
}
