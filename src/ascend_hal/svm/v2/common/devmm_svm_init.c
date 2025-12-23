/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */


#include <sched.h>
#include <pthread.h>

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "svm_ioctl.h"
#include "devmm_svm.h"
#include "drv_type.h"
#include "davinci_interface.h"
#include "dms_user_interface.h"
#include "devmm_svm_init.h"

void *g_devmm_mem_start = ((void *)DEVMM_SVM_MEM_START);

uint32_t g_mmap_seg_num = 0;
struct devmm_mmap_addr_seg g_mmap_segs[DEVMM_MAX_VMA_NUM] = {{0}};
static bool g_is_need_map_nptmv = false;

THREAD int g_devmm_mem_dev = -1;
pthread_mutex_t g_devmm_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_devmm_mmap_mutex = PTHREAD_MUTEX_INITIALIZER;

static THREAD int g_svm_host_mem_alloc_mode = SVM_HOST_MEM_ALLOCED_BY_MMAP;

void devmm_set_host_mem_alloc_mode(int mode)
{
    g_svm_host_mem_alloc_mode = mode;
}

int devmm_get_host_mem_alloc_mode(void)
{
    return g_svm_host_mem_alloc_mode;
}

#ifndef EMU_ST
STATIC DVresult devmm_davinci_open(int fd, const char *davinci_sub_name)
{
    struct davinci_intf_open_arg arg = {0};
    int tmp_errno;
    int ret;

    ret = strcpy_s(arg.module_name, DAVINIC_MODULE_NAME_MAX, davinci_sub_name);
    if (ret != 0) {
        DEVMM_DRV_ERR("Strcpy_s failed. (ret=%d)\n", ret);
        return DRV_ERROR_INNER_ERR;
    }

    ret = ioctl(fd, (unsigned long)DAVINCI_INTF_IOCTL_OPEN, &arg);
    if (ret != 0) {
        tmp_errno = errno;
        DEVMM_DRV_ERR("DAVINCI_INTF_IOCTL_OPEN failed. (ret=%d)\n", ret);
        return DEVMM_KERNEL_ERR(tmp_errno);
    }

    return DRV_ERROR_NONE;
}

STATIC void devmm_davinci_close(int fd, const char *davinci_sub_name)
{
    struct davinci_intf_close_arg arg = {0};
    int ret;

    ret = strcpy_s(arg.module_name, DAVINIC_MODULE_NAME_MAX, davinci_sub_name);
    if (ret != 0) {
        DEVMM_DRV_ERR("Strcpy_s failed. (ret=%d)\n", ret);
        return;
    }

    ret = ioctl(fd, (unsigned long)DAVINCI_INTF_IOCTL_CLOSE, &arg);
    if (ret != 0) {
        DEVMM_DRV_ERR("Davinci close fail. (ret=%d)\n", ret);
    }
}

STATIC int devmm_svm_open_proc(const char *davinci_sub_name)
{
    int fd = -1;

    fd = open(davinci_intf_get_dev_path(), O_RDWR | O_SYNC | O_CLOEXEC);
    if (fd < 0) {
        DEVMM_DRV_ERR("Open device fail. (fd=%d; chardev=%s)\n", fd, davinci_intf_get_dev_path());
        return fd;
    }

    DVresult ret;
    ret = devmm_davinci_open(fd, davinci_sub_name);
    if (ret != DRV_ERROR_NONE) {
        (void)close(fd);
        DEVMM_DRV_ERR("Davinci open memory device fail. (ret=%d; sub_name=%s)\n",
            ret, davinci_sub_name);
        return -1;
    }
    return fd;
}
#endif

DVresult devmm_svm_open(const char *davinci_sub_name)
{
    g_devmm_mem_dev = devmm_svm_open_proc(davinci_sub_name);
    if (g_devmm_mem_dev < 0) {
        DEVMM_DRV_ERR("Open memory device. (g_devmm_mem_dev=%d)\n", g_devmm_mem_dev);
        return DRV_ERROR_INVALID_DEVICE;
    }
    return DRV_ERROR_NONE;
}

#ifndef EMU_ST
DVresult devmm_svm_ioctl(int fd, unsigned long request, struct devmm_ioctl_arg *arg)
{
    int ret, tmp_errno;

    ret = ioctl(fd, request, arg);
    if (ret != 0) {
        tmp_errno = errno;
        share_log_read_err(HAL_MODULE_TYPE_DEVMM);
        return DEVMM_KERNEL_ERR(tmp_errno);
    }
    return DRV_ERROR_NONE;
}

void *devmm_svm_map_with_flag(void *mem_map_addr, uint64_t size, uint64_t flags, int fixed_va_flag)
{
    void *mem_mapped_addr = NULL;
    int err;
    int ret;

    mem_mapped_addr = mmap(mem_map_addr, size, PROT_READ | PROT_WRITE, (int)(MAP_SHARED | flags), g_devmm_mem_dev, 0);
    if (mem_mapped_addr == MAP_FAILED) {
        err = errno;
        DEVMM_RUN_INFO("Host mem goto malloc. (mapped=0x%lx; start=0x%lx; errno=%d; errstr=%s)\n",
            mem_map_addr, mem_mapped_addr, err, strerror(err));
        return NULL;
    }

    if ((fixed_va_flag == 1) && (mem_mapped_addr != mem_map_addr)) {
        (void)munmap((void *)(uintptr_t)mem_mapped_addr, size);
        return NULL;
    }

    ret = madvise(mem_mapped_addr, size, MADV_DONTDUMP);
    if (ret != 0) {
        (void)munmap(mem_mapped_addr, size);
        return NULL;
    }

    return mem_mapped_addr;
}

void devmm_svm_munmap(void *mem_unmap_addr, uint64_t size)
{
    (void)munmap(mem_unmap_addr, size);
}
#endif

/* DEVMM_MAP_NPTMV is used for distinguish double pgtable by os. */
#define DEVMM_MAP_NPTMV 0x1000000
void *devmm_svm_map_by_size(void *mem_map_addr, uint64_t size)
{
    /* MAP_NPTMV is used for distinguish double pgtable by os. */
    uint64_t flags = (g_is_need_map_nptmv && ((uint64_t)(uintptr_t)mem_map_addr >= (DEVMM_SVM_MEM_START + DEVMM_SVM_MEM_SIZE))) ?
        DEVMM_MAP_NPTMV : 0;

    return devmm_svm_map_with_flag(mem_map_addr, size, flags, 0);
}

STATIC int devmm_svm_get_mmap_para(struct devmm_mmap_addr_seg *segs, uint32_t *seg_num)
{
    struct devmm_ioctl_arg mmap_arg = {0};
    DVresult ret;

    mmap_arg.data.mmap_para.seg_num = *seg_num;
    mmap_arg.data.mmap_para.segs = segs;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_GET_MMAP_INFO, &mmap_arg);
    if ((ret != DRV_ERROR_NONE) || (mmap_arg.data.mmap_para.seg_num > DEVMM_MAX_VMA_NUM)) {
        return DRV_ERROR_IOCRL_FAIL;
    }

    *seg_num = mmap_arg.data.mmap_para.seg_num;
    g_is_need_map_nptmv = mmap_arg.data.mmap_para.is_need_map_nptmv;
    return DRV_ERROR_NONE;
}

static void _devmm_svm_unmap_range(uint64_t mem_unmap_addr, uint64_t size)
{
    (void)devmm_svm_munmap((void *)(uintptr_t)mem_unmap_addr, size);
}

STATIC void devmm_svm_unmap_range(uint32_t seg_num)
{
    u64 i;

    for (i = 0; i < seg_num; i++) {
        _devmm_svm_unmap_range(g_mmap_segs[i].va, g_mmap_segs[i].size);
        g_mmap_segs[i].va = 0;
        g_mmap_segs[i].size = 0;
    }
}

static DVresult devmm_get_svm_map_err_result(int side)
{
    if (side == SVM_MASTER_SIDE) {
        /* When asan is enable or host os not support mmap 8T */
        g_mmap_seg_num = 0;
        devmm_set_host_mem_alloc_mode(SVM_HOST_MEM_ALLOCED_BY_MALLOC);
        return DRV_ERROR_NONE;
    } else {
        return DRV_ERROR_OUT_OF_MEMORY;
    }
}

bool devmm_is_in_mmap_segs(uint64_t va, uint64_t size)
{
    uint32_t i;

    if (va == 0) {
        return false;
    }

    (void)pthread_mutex_lock(&g_devmm_mmap_mutex);
    for (i = 0; i < g_mmap_seg_num; i++) {
        if ((g_mmap_segs[i].va == va) && (g_mmap_segs[i].size == size)) {
            (void)pthread_mutex_unlock(&g_devmm_mmap_mutex);
            return true;
        }
    }
    (void)pthread_mutex_unlock(&g_devmm_mmap_mutex);
    return false;
}

DVresult devmm_svm_map(int side)
{
    void *mem_mapped_addr = NULL;
    uint32_t seg_num = DEVMM_MAX_VMA_NUM;
    uint32_t i;
    int ret;

    (void)pthread_mutex_lock(&g_devmm_mmap_mutex);

    ret = devmm_svm_get_mmap_para(g_mmap_segs, &seg_num);
    if (ret != 0) {
        (void)pthread_mutex_unlock(&g_devmm_mmap_mutex);
        DEVMM_DRV_ERR("Get mmap para error. (ret=%d)\n", ret);
        return ret;
    }

    for (i = 0; i < seg_num; i++) {
        mem_mapped_addr = devmm_svm_map_by_size((void *)(uintptr_t)g_mmap_segs[i].va, g_mmap_segs[i].size);
        if (mem_mapped_addr == NULL) {
            devmm_svm_unmap_range(i);
            (void)pthread_mutex_unlock(&g_devmm_mmap_mutex);
            return devmm_get_svm_map_err_result(side);
        }
    }

    g_mmap_seg_num = seg_num;
    (void)pthread_mutex_unlock(&g_devmm_mmap_mutex);

    return DRV_ERROR_NONE;
}

void devmm_svm_unmap(void)
{
    (void)pthread_mutex_lock(&g_devmm_mmap_mutex);
    devmm_svm_unmap_range(g_mmap_seg_num);
    g_mmap_seg_num = 0;
    (void)pthread_mutex_unlock(&g_devmm_mmap_mutex);
}

#ifndef EMU_ST
STATIC void devmm_svm_close_proc(int fd, const char *davinci_sub_name)
{
    devmm_davinci_close(fd, davinci_sub_name);
    (void)close(fd);
}
#endif

STATIC DVresult devmm_svm_alloc_proc_struct(void)
{
    struct devmm_ioctl_arg para_arg = {0};
    DVresult ret;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_ALLOC_PROC_STRUCT, &para_arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Alloc process struct error. (ret=%d)\n", ret);
        return ret;
    }
    return DRV_ERROR_NONE;
}

void devmm_svm_close(const char *davinci_sub_name)
{
    devmm_svm_close_proc(g_devmm_mem_dev, davinci_sub_name);
    g_devmm_mem_dev = -1;
}

STATIC DVresult devmm_svm_init(const char *davinci_sub_name, int side)
{
    DVresult ret;

    ret = devmm_svm_open(davinci_sub_name);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }
    ret = devmm_svm_alloc_proc_struct();
    if (ret != DRV_ERROR_NONE) {
        devmm_svm_close(davinci_sub_name);
        return ret;
    }
    ret = devmm_svm_map(side);
    if (ret != DRV_ERROR_NONE) {
        devmm_svm_close(davinci_sub_name);
        return ret;
    }
    return DRV_ERROR_NONE;
}

STATIC void devmm_svm_uninit(void)
{
    devmm_svm_unmap();
    /* davinci chardev will be released in the relese process, no need to close */
}

DVresult devmm_svm_master_init(void)
{
    return devmm_svm_init(DAVINCI_SVM_SUB_MODULE_NAME, SVM_MASTER_SIDE);
}

DVresult devmm_svm_agent_init(void)
{
    return devmm_svm_init(DAVINCI_SVM_AGENT_SUB_MODULE_NAME, SVM_AGENT_SIDE);
}

void devmm_svm_agent_uninit(void)
{
    devmm_svm_uninit();
}

void devmm_svm_master_uninit(void)
{
    devmm_svm_uninit();
}
