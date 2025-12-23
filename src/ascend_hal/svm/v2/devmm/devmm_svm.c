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
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>

#include "ascend_hal.h"
#include "ascend_inpackage_hal.h"
#include "dpa_user_interface.h"
#include "dms_user_interface.h"

#include "svm_ioctl.h"
#include "devmm_virt_comm.h"
#include "devmm_virt_interface.h"
#include "devmm_dynamic_addr.h"
#include "devmm_svm_init.h"
#include "devmm_virt_base_heap.h"
#include "devmm_virt_com_heap.h"
#include "devmm_virt_dvpp_heap.h"
#include "devmm_virt_read_only_heap.h"
#include "devmm_cache_coherence.h"
#include "devmm_map_dev_reserve.h"
#include "svm_mem_statistics.h"
#include "svm_user_interface.h"
#include "devmm_host_mem_pool.h"
#include "svm_user_msg.h"
#include "devmm_record.h"
#include "devmm_svm.h"

#define MEM_VIRT_MASK          ((1UL << MEM_VIRT_WIDTH) - 1)

#define DEVMM_MEM_ALLOC_RECOMMENDED_GRANULARITY 0x200000
#define DEVMM_MEM_ALLOC_MINIMUN_GRANULARITY 0x200000

static THREAD uint32_t svm_devid;
static THREAD bool g_get_user_malloc_attr = false;
static THREAD bool g_pro_snapshot_state = false;

struct devmm_advise_record_node {
    DVdeviceptr ptr;
    size_t count;
    struct devmm_virt_list_head node;
};

#define ADVISE_RECORD_MAX_DEV_NUM DEVMM_MAX_PHY_DEVICE_NUM
STATIC struct devmm_virt_list_head advise_record_list[ADVISE_RECORD_MAX_DEV_NUM];
STATIC pthread_mutex_t advise_record_lock[ADVISE_RECORD_MAX_DEV_NUM];

STATIC void devmm_del_mem_advise_record(bool is_del_all, DVdeviceptr free_ptr, size_t free_cnt, DVdevice device);

bool devmm_is_snapshot_state(void)
{
    return g_pro_snapshot_state;
}

bool devmm_is_split_mode(void)
{
    static unsigned int split_mode;
    static bool query_flag = false;
    drvError_t ret;

    if (query_flag == false) {
        ret = halGetDeviceSplitMode(0, &split_mode);
        if (ret != DRV_ERROR_NONE) {
            DEVMM_DRV_WARN("Get split mode not succ. (ret=%d)\n", ret);
            return false;
        }
        DEVMM_RUN_INFO("Get split mode succ. (split_mode=%u)\n", split_mode);
        query_flag = true;
    }

    return (split_mode != VMNG_NORMAL_NONE_SPLIT_MODE);
}

static bool devmm_dev_is_inited(uint32_t devid)
{
    struct devmm_virt_heap_mgmt *mgmt = devmm_virt_get_heap_mgmt();
    return ((mgmt != NULL) && mgmt->is_dev_inited[devid]);
}

static void devmm_set_device_info(struct devmm_virt_heap_mgmt *mgmt, uint32_t devid,
    struct devmm_setup_dev_para *dev_para)
{
    bool is_alloced_by_malloc = (devmm_get_host_mem_alloc_mode() == SVM_HOST_MEM_ALLOCED_BY_MALLOC);

    mgmt->dvpp_mem_size[devid] = dev_para->dvpp_mem_size;
    mgmt->support_bar_mem[devid] = is_alloced_by_malloc ? false : dev_para->support_bar_mem;
    mgmt->support_dev_read_only[devid] = dev_para->support_dev_read_only;
    mgmt->support_dev_mem_map_host[devid] = is_alloced_by_malloc ? false : dev_para->support_dev_mem_map_host;
    mgmt->support_bar_huge_mem[devid] = is_alloced_by_malloc ? false : dev_para->support_bar_huge_mem;
    mgmt->host_support_pin_user_pages_interface = dev_para->host_support_pin_user_pages_interface;
    mgmt->support_host_rw_dev_ro = dev_para->support_host_rw_dev_ro;
    mgmt->double_pgtable_offset[devid] = dev_para->double_pgtable_offset;
    mgmt->support_remote_mmap[devid] = dev_para->support_remote_mmap;
    mgmt->support_host_pin_pre_register = is_alloced_by_malloc ? false : dev_para->support_host_pin_pre_register;
    mgmt->support_host_mem_pool = is_alloced_by_malloc ? false :  dev_para->support_host_mem_pool;
    mgmt->support_agent_giant_page[devid] = dev_para->support_agent_giant_page;
    mgmt->support_shmem_map_exbus[devid] = dev_para->support_shmem_map_exbus;
    DEVMM_RUN_INFO("Device info. (devid=%u; dvpp_size=%llu; support_bar_mem=%u; support_dev_read_only=%u; "
        "support_dev_mem_map_host=%u; support_bar_huge_mem=%u; allocated_by_malloc=%d; host_rw_dev_ro=%u)\n",
        devid, dev_para->dvpp_mem_size, dev_para->support_bar_mem, dev_para->support_dev_read_only,
        dev_para->support_dev_mem_map_host, dev_para->support_bar_huge_mem,
        is_alloced_by_malloc, dev_para->support_host_rw_dev_ro);
    DEVMM_RUN_INFO("Device info. (double_pgtable_offset=%llu; support_host_pin_pre_register=%u; "
        "support_host_mem_pool=%u; is_support_agent_giant_page=%u; is_support_host_giant_page=%u; "
        "support_remote_mmap=%u; support_shmem_map_exbus=%u)\n",
        dev_para->double_pgtable_offset, dev_para->support_host_pin_pre_register, dev_para->support_host_mem_pool,
        dev_para->support_agent_giant_page, mgmt->support_host_giant_page, dev_para->support_remote_mmap,
        dev_para->support_shmem_map_exbus);
}

/* In training scenarios, one process can be bound to only one device. */
static DVresult devmm_setup_device(uint32_t devid)
{
    struct devmm_virt_heap_mgmt *mgmt = NULL;
    struct devmm_ioctl_arg arg = {0};
    int ret;

    DEVMM_RUN_INFO("DrvMemDeviceOpen. (devid=%u)\n", devid);
    if (devid >= SVM_MAX_AGENT_NUM) {
        DEVMM_DRV_ERR("Devid overflow. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    mgmt = devmm_virt_init_get_heap_mgmt();
    if (mgmt == NULL) {
        DEVMM_DRV_ERR("Init heap mgmt failed. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_HANDLE;
    }

    if (mgmt->can_init_dev[devid] == false) {
#ifndef EMU_ST
        DEVMM_DRV_ERR("Not allowed to initialize device. (devid=%u)\n", devid);
        return DRV_ERROR_PARA_ERROR;
#endif
    }

    ret = svm_da_add_dev(devid);
    if (ret != 0) {
        DEVMM_DRV_ERR("Da mng add dev failed. (devid=%u)\n", devid);
        return ret;
    }

    svm_init_mem_stats_mng(devid);
    arg.head.devid = devid;
    arg.data.setup_dev_para.mem_stats_va = svm_get_mem_stats_va(devid);
    ret = (int)devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_SETUP_DEVICE, &arg);
    share_log_read_run_info(HAL_MODULE_TYPE_DEVMM);
    if (ret != 0) {
        svm_da_del_dev(devid);
        DEVMM_DRV_ERR("Setup device error. (ret=%d; devid=%u)\n", ret, devid);
        return ret;
    }
    if (devid < DEVMM_MAX_PHY_DEVICE_NUM) {
        devmm_set_device_info(mgmt, devid, &arg.data.setup_dev_para);
        (void)devmm_init_dvpp_heap_by_devid(devid);
        if (mgmt->support_dev_read_only[devid] != 0) {
            (void)devmm_init_read_only_heap_by_devid(devid, SUB_READ_ONLY_TYPE);
        }
        (void)devmm_init_read_only_heap_by_devid(devid, SUB_DEV_READ_ONLY_TYPE);
        devmm_init_dev_reserve_addr(devid);
    }

    mgmt->is_dev_inited[devid] = true; /* host_mem_pool need is_dev_inited set */
    if (mgmt->support_host_mem_pool && !devmm_is_snapshot_state()) {
        devmm_host_mem_pool_init(devid);
    }

    svm_devid = devid;
    return 0;
}

static DVresult devmm_free_managed_nomal(DVdeviceptr p)
{
    struct devmm_virt_heap_mgmt *p_heap_mgmt = NULL;
    struct devmm_virt_com_heap *heap = NULL;
    uint32_t heap_list_type;
    uint32_t heap_sub_type;
    uint64_t free_len;
    DVresult ret;

    p_heap_mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (p_heap_mgmt == NULL) {
        DEVMM_DRV_ERR("Please call device_open and alloc_mem api first. (alloc_mem=0x%llx)\n", p);
        return DRV_ERROR_INVALID_HANDLE;
    }

    heap = devmm_va_to_heap(p);
    if ((heap == NULL) || heap->heap_type == DEVMM_HEAP_IDLE) {
        DEVMM_DRV_ERR("Address is not allocated. please check ptr. (offset=%llx)\n", ADDR_TO_OFFSET(p));
        return DRV_ERROR_INVALID_VALUE;
    }

    heap_sub_type = heap->heap_sub_type;
    if (heap_sub_type == SUB_RESERVE_TYPE) {
        DEVMM_DRV_ERR("Addr should be freed by halMemAddressFree api. (va=0x%llx)\n", p);
        return DRV_ERROR_INVALID_VALUE;
    }

    heap_list_type = heap->heap_list_type;
    if (heap_list_type == HOST_AGENT_LIST) {
        ret = halHostUnregister((void *)p, SVM_HOST_AGENT_ID);
        if (ret != DRV_ERROR_NONE) {
            DEVMM_DRV_ERR("Remote_map failed. (free_ptr=0x%llx)\n", p);
            return ret;
        }
    }

    if (devmm_virt_heap_is_primary(heap)) {
        free_len = heap->mapped_size;
        ret = devmm_free_to_base_heap(p_heap_mgmt, heap, p);
    } else {
        ret = devmm_free_to_normal_heap(p_heap_mgmt, heap, p, &free_len);
    }

    if ((ret == DRV_ERROR_NONE) && (heap_sub_type != SUB_SVM_TYPE) && (heap_sub_type != SUB_HOST_TYPE)) {
        uint32_t devid = devmm_heap_device_by_list_type(heap_list_type);
        devmm_del_mem_advise_record(false, p, free_len, devid);
    }

    return ret;
}

/* can not declared static, it patch use */
DVresult devmm_free_managed(DVdeviceptr p)
{
    if (!devmm_va_is_svm(p) &&
        (devmm_get_host_mem_alloc_mode() == SVM_HOST_MEM_ALLOCED_BY_MALLOC)) {
        free((void *)p);
        return 0;
    }

    return devmm_free_managed_nomal(p);
}

static inline bool devmm_should_alloc_from_base(struct devmm_virt_heap_type *heap_type, size_t bytesize,
    DVmem_advise advise)
{
    return ((heap_type->heap_sub_type != SUB_DVPP_TYPE) &&
        (heap_type->heap_sub_type != SUB_READ_ONLY_TYPE) &&
        (heap_type->heap_sub_type != SUB_DEV_READ_ONLY_TYPE) &&
        (bytesize > DEVMM_LARGE_MEM_THRESHOLD_SIZE)) || ((advise & DV_ADVISE_GIANTPAGE) != 0);
}

/* can not declared static, it patch use */
DVresult devmm_alloc_managed(DVdeviceptr *pp, size_t bytesize,
    struct devmm_virt_heap_type *heap_type, DVmem_advise advise)
{
    struct devmm_virt_heap_mgmt *p_heap_mgmt = NULL;
    DVdeviceptr temp_ptr;

    /* asan: host just use libc malloc */
    if ((heap_type->heap_sub_type == SUB_HOST_TYPE) &&
        (devmm_get_host_mem_alloc_mode() == SVM_HOST_MEM_ALLOCED_BY_MALLOC)) {
        *pp = (DVdeviceptr)malloc(bytesize);
        if (*pp == (DVdeviceptr)NULL) {
#ifndef EMU_ST
            return DRV_ERROR_OUT_OF_MEMORY;
#endif
        }
        return 0;
    }

    /* 1.if heap mgmt not inited,init it. */
    p_heap_mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (p_heap_mgmt == NULL) {
        DEVMM_DRV_ERR("Devmm_virt_get_heap_mgmt heap mgmt is NULL.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }

    /* 2.the max size can be allocated is DEVMM_POOL_VM_RANGE */
    if ((pp == NULL) || (bytesize == 0)) {
        DEVMM_DRV_ERR("PP is Null or bytesize is 0 or bytesize is invalid. "
                      "(pp_is_null=%d; bytesize=%lu; max_size=%llu)\n",
                      (pp == NULL), bytesize, p_heap_mgmt->max_conti_size);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (bytesize > p_heap_mgmt->max_conti_size) {
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    /* if alloc size larger than half heap size, alloc mem form base heap  */
    if (devmm_should_alloc_from_base(heap_type, bytesize, advise)) {
        temp_ptr = devmm_alloc_from_base_heap(p_heap_mgmt, bytesize, heap_type, advise, *pp);
    } else {
        temp_ptr = devmm_alloc_from_normal_heap(p_heap_mgmt, bytesize, heap_type, advise, *pp);
    }

    if (temp_ptr < DEVMM_SVM_MEM_START) {
        return ptr_to_errcode(temp_ptr);
    }

    *pp = temp_ptr;
    return DRV_ERROR_NONE;
}

static uint32_t devmm_get_memtype_from_advise(DVmem_advise advise)
{
    if ((advise & DV_ADVISE_P2P_HBM) != 0) {
        return DEVMM_P2P_HBM_MEM;
    } else if ((advise & DV_ADVISE_HBM) != 0) {
        return DEVMM_HBM_MEM;
    } else if ((advise & DV_ADVISE_P2P_DDR) != 0) {
        return DEVMM_P2P_DDR_MEM;
    } else if ((advise & DV_ADVISE_TS_DDR) != 0) {
        return DEVMM_TS_DDR_MEM;
    } else {
        return DEVMM_DDR_MEM;
    }
}

static void devmm_fill_host_heap_type(struct devmm_virt_heap_type *heap_type)
{
    heap_type->heap_type = DEVMM_HEAP_PINNED_HOST;
    heap_type->heap_list_type = HOST_LIST;
    heap_type->heap_sub_type = SUB_HOST_TYPE;
    heap_type->heap_mem_type = DEVMM_DDR_MEM;
}

static void devmm_fill_svm_heap_type(DVmem_advise advise,
    struct devmm_virt_heap_type *heap_type)
{
    heap_type->heap_list_type = SVM_LIST;
    heap_type->heap_sub_type = SUB_SVM_TYPE;
    heap_type->heap_mem_type = devmm_get_memtype_from_advise(advise);
    if ((advise & DV_ADVISE_HUGEPAGE) != 0) {
        heap_type->heap_type = DEVMM_HEAP_HUGE_PAGE;
    } else {
        heap_type->heap_type = DEVMM_HEAP_CHUNK_PAGE;
    }
}

static void devmm_fill_dev_heap_type(uint32_t devid, DVmem_advise advise,
    struct devmm_virt_heap_type *heap_type)
{
    heap_type->heap_list_type = devmm_heap_list_type_by_device(devid);
    heap_type->heap_sub_type = SUB_DEVICE_TYPE;
    heap_type->heap_mem_type = devmm_get_memtype_from_advise(advise);
    if ((advise & DV_ADVISE_HUGEPAGE) != 0) {
        heap_type->heap_type = DEVMM_HEAP_HUGE_PAGE;
    } else {
        heap_type->heap_type = DEVMM_HEAP_CHUNK_PAGE;
    }
}

static void devmm_fill_reserve_heap_type(DVmem_advise advise, struct devmm_virt_heap_type *heap_type)
{
    heap_type->heap_list_type = RESERVE_LIST;
    heap_type->heap_sub_type = SUB_RESERVE_TYPE;
    heap_type->heap_mem_type = 0;       /* not care */
    if ((advise & DV_ADVISE_HUGEPAGE) != 0) {
        heap_type->heap_type = DEVMM_HEAP_HUGE_PAGE;
    } else {
        heap_type->heap_type = DEVMM_HEAP_CHUNK_PAGE;
    }
}

static void devmm_fill_heap_type(uint32_t devid, uint32_t sub_mem_type,
    DVmem_advise advise, struct devmm_virt_heap_type *heap_type)
{
    if (sub_mem_type == SUB_DEVICE_TYPE) {
        devmm_fill_dev_heap_type(devid, advise, heap_type);
    } else if (sub_mem_type == SUB_DVPP_TYPE) {
        devmm_fill_dvpp_heap_type(devid, heap_type);
    } else if ((sub_mem_type == SUB_READ_ONLY_TYPE) || (sub_mem_type == SUB_DEV_READ_ONLY_TYPE)) {
        devmm_fill_read_only_heap_type(devid, sub_mem_type, heap_type);
    } else if (sub_mem_type == SUB_HOST_TYPE) {
        devmm_fill_host_heap_type(heap_type);
    } else if (sub_mem_type == SUB_SVM_TYPE) {
        devmm_fill_svm_heap_type(advise, heap_type);
    } else if (sub_mem_type == SUB_RESERVE_TYPE) {
        devmm_fill_reserve_heap_type(advise, heap_type);
    } else {
        /* impossible branch, just print, not return */
        DEVMM_DRV_ERR("sub_mem_type is invalid. (sub_mem_type=%u)\n", sub_mem_type);
    }
}

static inline bool devmm_is_mem_host_side(uint32_t side)
{
    return (side == MEM_HOST_SIDE) || (side == MEM_HOST_NUMA_SIDE);
}

drvError_t devmm_alloc_proc(uint32_t devid, uint32_t sub_mem_type,
    DVmem_advise advise, size_t size, DVdeviceptr *pp)
{
    struct devmm_virt_heap_type heap_type;
    void *dst_ptr = NULL;
    DVresult result;

    if (devid >= SVM_MAX_AGENT_NUM) {
        DEVMM_DRV_ERR("Devid is invalid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }
    devmm_fill_heap_type(devid, sub_mem_type, advise, &heap_type);
    result = devmm_alloc_managed(pp, size, &heap_type, advise);
    if ((result == DRV_ERROR_NONE) && (heap_type.heap_list_type == HOST_AGENT_LIST)) {
        result = halHostRegister((void *)*pp, size, DEV_SVM_MAP_HOST, devid, &dst_ptr);
        if (result != DRV_ERROR_NONE) {
            DEVMM_DRV_ERR("Register failed. (ptr=%llx; size=%llu)\n", *(DVdeviceptr *)pp, size);
            (void)halMemFree((void *)(uintptr_t)*pp);
        }
    }

    return result;
}

#ifndef EMU_ST
/* because memcpy_s 2nd para must less than(or equal to) 2G, devmm will copy big memory by loop */
STATIC DVresult devmm_h2h_copy(DVdeviceptr dst, size_t dst_max, DVdeviceptr src, size_t count)
{
    size_t rest_count, per_count;
    int ret;

    if (dst_max < count) {
        DEVMM_DRV_ERR("Count bigger than dstMax. (dst=0x%llx; src=0x%llx; count=%lu; dstMax=%lu)\n",
                      dst, src, count, dst_max);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (count <= SECUREC_MEM_MAX_LEN && dst_max <= SECUREC_MEM_MAX_LEN) {
        ret = memcpy_s((void *)(uintptr_t)dst, dst_max, (void *)(uintptr_t)src, count);
        if (ret != 0) {
            DEVMM_DRV_ERR("Memcpy error. (dst=0x%llx; dstMax=%llu; src=0x%llx; count=%llu; ret=%d)\n",
                          dst, dst_max, src, count, ret);
            return DRV_ERROR_INVALID_VALUE;
        }
    } else if (count <= SECUREC_MEM_MAX_LEN && dst_max > SECUREC_MEM_MAX_LEN) {
        ret = memcpy_s((void *)(uintptr_t)dst, SECUREC_MEM_MAX_LEN, (void *)(uintptr_t)src, count);
        if (ret != 0) {
            DEVMM_DRV_ERR("Memcpy error. (dst=0x%llx; dstMax=%llu; src=0x%llx; count=%llu; ret=%d)\n",
                          dst, SECUREC_MEM_MAX_LEN, src, count, ret);
            return DRV_ERROR_INVALID_VALUE;
        }
    } else if (count > SECUREC_MEM_MAX_LEN) {
        for (rest_count = count; rest_count > 0;) {
            per_count = rest_count > SECUREC_MEM_MAX_LEN ? SECUREC_MEM_MAX_LEN : rest_count;
            ret = memcpy_s((void *)(uintptr_t)dst, per_count, (void *)(uintptr_t)src, per_count);
            if (ret != 0) {
                DEVMM_DRV_ERR("Memcpy error. (dst=0x%llx; src=0x%llx; per_count=%lu; ret=%d)\n",
                              dst, src, per_count, ret);
                return DRV_ERROR_INVALID_VALUE;
            }
            dst = dst + per_count;
            src = src + per_count;
            rest_count = rest_count - per_count;
        }
    }

    return DRV_ERROR_NONE;
}
#endif

STATIC DVresult devmm_copy_ioctl(DVdeviceptr dst, size_t dest_max, DVdeviceptr src, size_t count, unsigned long cmd)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    arg.data.copy_para.dst = dst;
    arg.data.copy_para.src = src;
    arg.data.copy_para.ByteCount= count;
    arg.data.copy_para.direction = DEVMM_COPY_INVILED_DIRECTION;
    arg.data.copy_para.is_support_dev_local_addr = false;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, cmd, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NOT_SUPPORT), "Ioctl error. (cmd=%lu; ret=%d; dst=0x%llx; src=0x%llx; size=%lu)\n", cmd, ret, dst, src, count);
        devmm_print_svm_va_info(src, ret);
        devmm_print_svm_va_info(dst, ret);
        return ret;
    }

    if ((cmd == (unsigned int)DEVMM_SVM_MEMCPY) && (arg.data.copy_para.direction == DEVMM_COPY_HOST_TO_HOST)) {
        return devmm_h2h_copy(dst, dest_max, src, count);
    }

    return DRV_ERROR_NONE;
}

#ifdef CFG_HOST_CHIP_HI3559A
DVresult devmm_copy_cache_incoherence(DVdeviceptr dst, size_t dest_max, DVdeviceptr src, size_t byte_count)
{
    DVdeviceptr dst_p;
    DVdeviceptr src_p;
    int ret;
    /*
     * if host dma not support cache consistency
     * svm host mem is 4k per blk, can ensure cache consistency
     * so malloc svm host mem to dma use
     */
    if (!devmm_va_is_svm(dst)) {
        ret = drv_mem_alloc_host((void **)&dst_p, byte_count);
        if (ret != 0) {
            DEVMM_DRV_ERR("DrvMemAllocHost failed. (dst=0x%llx; byte_count=%lu)\n", dst, byte_count);
            return ret;
        }
        ret = devmm_copy_ioctl(dst_p, dest_max, src, byte_count, DEVMM_SVM_MEMCPY);
        if (ret != 0) {
            DEVMM_DRV_ERR("Copy failed. (dst_p=0x%llx; dst=0x%llx; src=0x%llx; byte_count=%lu)\n",
                dst_p, dst, src, byte_count);
            (void)halMemFree((void *)dst_p);
            return ret;
        }
        ret = devmm_h2h_copy(dst, dest_max, dst_p, byte_count);
        (void)halMemFree((void *)dst_p);
        return ret;
    }

    if (!devmm_va_is_svm(src)) {
        ret = drv_mem_alloc_host((void **)&src_p, byte_count);
        if (ret != 0) {
            DEVMM_DRV_ERR("DrvMemAllocHost failed. (src=0x%llx; byte_count=%lu)\n", src, byte_count);
            return ret;
        }
        ret = devmm_h2h_copy(src_p, byte_count, src, byte_count);
        if (ret != 0) {
            DEVMM_DRV_ERR("Trans copy failed. (src_p=0x%llx; src=0x%llx)\n", src_p, src);
            (void)halMemFree((void *)src_p);
            return ret;
        }
        ret = devmm_copy_ioctl(dst, dest_max, src_p, byte_count, DEVMM_SVM_MEMCPY);
        (void)halMemFree((void *)src_p);
        return ret;
    }

    return 0;
}
#endif

STATIC INLINE DVresult devmm_copy_cache_coherence(DVdeviceptr dst, size_t dest_max, DVdeviceptr src, size_t byte_count)
{
    return devmm_copy_ioctl(dst, dest_max, src, byte_count, DEVMM_SVM_MEMCPY);
}

static DVresult devmm_get_devid_by_va(DVdeviceptr va, uint32_t *devid)
{
    struct devmm_virt_com_heap *heap = NULL;

    heap = devmm_va_to_heap((virt_addr_t)va);
    if (heap == NULL) {
        return DRV_ERROR_INVALID_VALUE;
    }
    *devid = devmm_heap_device_by_list_type(heap->heap_list_type);
    return DRV_ERROR_NONE;
}

static DVresult devmm_try_copy_by_host_mem_pool(DVdeviceptr dst, size_t dest_max, DVdeviceptr src, size_t byte_count, bool src_is_svm)
{
    DVdeviceptr svm_va = src_is_svm ? src : dst;
    void *cache_va = NULL;
    void *fd = NULL;
    uint32_t devid;
    DVresult ret;

    ret = devmm_get_devid_by_va(svm_va, &devid);
    if (ret != DRV_ERROR_NONE) {
        return devmm_copy_cache_coherence(dst, dest_max, src, byte_count);
    }

    fd = devmm_host_mem_pool_get(devid, byte_count, &cache_va);
    if (fd == NULL) {
        return devmm_copy_cache_coherence(dst, dest_max, src, byte_count);
    }
#ifndef EMU_ST
    if (src_is_svm) { /* D2H */
        ret = devmm_copy_cache_coherence((DVdeviceptr)(uintptr_t)cache_va, byte_count, src, byte_count);
        if (ret != DRV_ERROR_NONE) {
            DEVMM_DRV_ERR("Devmm_copy_cache_coherence failed. (ret=%d; dst=0x%llx; src=0x%llx; byte_count=%zu)\n",
                ret, (uint64_t)(uintptr_t)cache_va, src, byte_count);
            devmm_host_mem_pool_put(devid, fd);
            return ret;
        }
        if (memcpy_s((void *)(uintptr_t)dst, byte_count, cache_va, byte_count) != 0) {
            DEVMM_DRV_ERR("Memcpy_s failed. (dst=0x%llx; src=0x%llx; byte_count=%zu)\n", dst, (uint64_t)(uintptr_t)cache_va, byte_count);
            devmm_host_mem_pool_put(devid, fd);
            return DRV_ERROR_PARA_ERROR;
        }
    } else { /* H2D */
        if (memcpy_s(cache_va, byte_count, (void *)(uintptr_t)src, byte_count) != 0) {
            DEVMM_DRV_ERR("Memcpy_s failed. (dst=0x%llx; src=0x%llx; byte_count=%zu)\n", (uint64_t)(uintptr_t)cache_va, src, byte_count);
            devmm_host_mem_pool_put(devid, fd);
            return DRV_ERROR_PARA_ERROR;
        }
        ret = devmm_copy_cache_coherence(dst, byte_count, (DVdeviceptr)(uintptr_t)cache_va, byte_count);
        if (ret != DRV_ERROR_NONE) {
            DEVMM_DRV_ERR("Devmm_copy_cache_coherence failed. (ret=%d; dst=0x%llx; src=0x%llx; byte_count=%zu)\n",
                ret, dst, (uint64_t)(uintptr_t)cache_va, byte_count);
            devmm_host_mem_pool_put(devid, fd);
            return ret;
        }
    }
#endif
    devmm_host_mem_pool_put(devid, fd);
    DEVMM_DRV_DEBUG_ARG("Copy by host mem pool success. (dst=0x%llx; src=0x%llx; byte_count=%zu; cache_va=0x%llx)\n",
        dst, src, byte_count, (uint64_t)(uintptr_t)cache_va);
    return DRV_ERROR_NONE;
}

static bool devmm_is_need_host_mem_pool(void *src, void *dst)
{
    bool src_is_svm_range = devmm_va_is_in_svm_range((uint64_t)(uintptr_t)src);
    bool dst_is_svm_range = devmm_va_is_in_svm_range((uint64_t)(uintptr_t)dst);
    void *svm_va = src_is_svm_range ? src : dst;
    struct devmm_virt_heap_mgmt *mgmt = NULL;
    struct devmm_virt_com_heap *heap = NULL;

#ifndef EMU_ST
    if ((src_is_svm_range && dst_is_svm_range) || ((src_is_svm_range == false) && (dst_is_svm_range == false))) {
        return false;
    }
#endif

    mgmt = devmm_virt_get_heap_mgmt();
    if ((mgmt == NULL) || (mgmt->support_host_mem_pool == false)) {
        return false;
    }

    heap = devmm_virt_get_heap_mgmt_virt_heap(devmm_va_to_heap_idx(mgmt, (virt_addr_t)(uintptr_t)svm_va));
    if ((heap == NULL) || (heap->heap_sub_type != SUB_DEVICE_TYPE)) {
        return false;
    }

    return true;
}

static DVresult _drvMemcpyInner(DVdeviceptr dst, size_t dest_max, DVdeviceptr src, size_t byte_count)
{
    bool src_is_svm, dst_is_svm;

    DEVMM_DRV_DEBUG_ARG("Argument. (dst=0x%llx; src=0x%llx; byte_count=%lu)\n", dst, src, byte_count);
    /* Similar to memcpy_s, (byte_count == 0) need to return 0 */
    if ((dst == 0) || (src == 0) || (dest_max == 0) || (dest_max < byte_count)) {
        DEVMM_DRV_ERR("Invalid argument. (dst=0x%llx; src=0x%llx; byte_count=%lu; dest_max=%lu)\n",
            dst, src, byte_count, dest_max);
        return DRV_ERROR_INVALID_VALUE;
    }

    /* Similar to memcpy_s, (byte_count == 0) need to return 0 */
    if (byte_count == 0) {
        return DRV_ERROR_NONE;
    }

    src_is_svm = devmm_va_is_svm(src);
    dst_is_svm = devmm_va_is_svm(dst);
    if ((src_is_svm == false) && (dst_is_svm == false)) {
        return devmm_h2h_copy(dst, dest_max, src, byte_count);
    }
#ifdef CFG_HOST_CHIP_HI3559A
    if (!src_is_svm || !dst_is_svm) {
        return devmm_copy_cache_incoherence(dst, dest_max, src, byte_count);
    }
#endif

    if (devmm_is_need_host_mem_pool((void *)(uintptr_t)src, (void *)(uintptr_t)dst)) {
        return devmm_try_copy_by_host_mem_pool(dst, dest_max, src, byte_count, devmm_va_is_in_svm_range(src));
    }

    return devmm_copy_cache_coherence(dst, dest_max, src, byte_count);
}

static bool devmm_host_support_pin_user_pages(struct devmm_virt_heap_mgmt *p_heap_mgmt)
{
    return p_heap_mgmt->host_support_pin_user_pages_interface;
}

static size_t devmm_get_host_page_size(struct devmm_virt_heap_mgmt *p_heap_mgmt)
{
    return p_heap_mgmt->local_page_size;
}

static DVresult devmm_mlock_first_page(DVdeviceptr va, size_t byte_count)
{
    struct devmm_virt_heap_mgmt *p_heap_mgmt = NULL;
    uint64_t page_size, offset, lock_len;
    int ret;

    p_heap_mgmt = devmm_virt_get_heap_mgmt();
    if (p_heap_mgmt == NULL) {
        DEVMM_DRV_WARN("Get heap mgmt failed, should drvMemDeviceOpen first.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }

    page_size = devmm_get_host_page_size(p_heap_mgmt);
    offset = va % page_size;
    lock_len = (byte_count >= (page_size - offset)) ? (page_size - offset) : byte_count;

    /*
     * If kernel support pin_user_pages*, no need mlock,
     * pin_user_pages* will prevent concurrency with page swap.
     */
    if (devmm_host_support_pin_user_pages(p_heap_mgmt) || devmm_va_is_svm(va) || (offset == 0)) {
        return DRV_ERROR_INVALID_HANDLE;
    }

    ret = mlock((const void *)va, lock_len);
    if (ret != 0) {
        DEVMM_DRV_WARN("Mlock failed. (ret=%d; va=0x%llx; len=%llu; lock_len=%llu)\n",
            ret, va, (uint64_t)byte_count, (uint64_t)lock_len);
        return DRV_ERROR_INVALID_HANDLE;
    }

    return DRV_ERROR_NONE;
}

static void devmm_munlock_first_page(DVdeviceptr va, size_t byte_count)
{
    struct devmm_virt_heap_mgmt *p_heap_mgmt = NULL;
    uint64_t page_size, offset, unlock_len;
    int ret;

    p_heap_mgmt = devmm_virt_get_heap_mgmt();
    if (p_heap_mgmt == NULL) {
        DEVMM_DRV_WARN("Get heap mgmt failed, should drvMemDeviceOpen first.\n");
        return;
    }

    page_size = devmm_get_host_page_size(p_heap_mgmt);
    offset = va % page_size;
    unlock_len = (byte_count >= (page_size - offset)) ? (page_size - offset) : byte_count;

    ret = munlock((const void *)va, unlock_len);
    if (ret != 0) {
        DEVMM_DRV_WARN("Munlock failed. (ret=%d; va=0x%llx; len=%llu; lock_len=%llu)\n",
            ret, va, (uint64_t)byte_count, (uint64_t)unlock_len);
    }
}

DVresult drvMemcpyInner(DVdeviceptr dst, size_t dest_max, DVdeviceptr src, size_t byte_count)
{
    DVresult mlock_ret, ret;

    /* Mlock to prevent page swap, dma copy with page swap may cause data consistency problems. */
    mlock_ret = devmm_mlock_first_page(dst, byte_count);
    ret = _drvMemcpyInner(dst, dest_max, src, byte_count);
    if (mlock_ret == DRV_ERROR_NONE) {
        devmm_munlock_first_page(dst, byte_count);
    }
    return ret;
}

DVresult drvMemcpy(DVdeviceptr dst, size_t dest_max, DVdeviceptr src, size_t byte_count)
{
    return drvMemcpyInner(dst, dest_max, src, byte_count);
}

static drvError_t devmm_mem_cpy_para_check(void *dst, size_t dst_size, void *src, size_t count,
    struct memcpy_info *info)
{
    if (dst == NULL) {
        DEVMM_DRV_ERR("Dst is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (src == NULL) {
        DEVMM_DRV_ERR("Src is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (dst_size == 0) {
        DEVMM_DRV_ERR("Dst size is zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (dst_size < count) {
        DEVMM_DRV_ERR("Dst size larger than cpy size. (dst_size=%lu; count=%lu)\n", dst_size, count);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (info == NULL) {
        DEVMM_DRV_ERR("Info is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (info->dir != DRV_MEMCPY_DEVICE_TO_HOST) {
        DEVMM_DRV_INFO("Invalid dir type. (dir_type=%u)\n", info->dir);
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (info->devid >= DEVMM_MAX_DEVICE_NUM) {
        DEVMM_DRV_ERR("Invalid devid. (devid=%u)\n", info->devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

drvError_t halMemcpy(void *dst, size_t dst_size, void *src, size_t count, struct memcpy_info *info)
{
    struct devmm_ioctl_arg arg = {0};
    void *tmp_dst = dst;
    void *fd = NULL;
    drvError_t ret;

    // drvMemcpy
    if (info == NULL) {
        return drvMemcpyInner((DVdeviceptr)dst, dst_size, (DVdeviceptr)src, count);
    }

    ret = devmm_mem_cpy_para_check(dst, dst_size, src, count, info);
    if (ret != 0) {
        return ret;
    }

    DEVMM_DRV_DEBUG_ARG("Argument. (dst=0x%llx; src=0x%llx; ByteCount=%lu; devid=%u)\n",
        dst, src, count, info->devid);

    /* Similar to memcpy_s, (count == 0) need to return 0 */
    if (count == 0) {
        return DRV_ERROR_NONE;
    }

    if (devmm_is_need_host_mem_pool(src, dst)) {
        fd = devmm_host_mem_pool_get(info->devid, count, &tmp_dst);
    }

    arg.head.devid = info->devid;
    arg.data.copy_para.dst = (uint64_t)(uintptr_t)tmp_dst;
    arg.data.copy_para.src = (uint64_t)(uintptr_t)src;
    arg.data.copy_para.ByteCount= count;
    arg.data.copy_para.direction = (uint32_t)info->dir;
    arg.data.copy_para.is_support_dev_local_addr = true;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEMCPY, &arg);
    if (ret != DRV_ERROR_NONE) {
        if (fd != NULL) {
            devmm_host_mem_pool_put(info->devid, fd);
        }
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NOT_SUPPORT), "Ioctl error. (ret=%d; dst=0x%llx; src=0x%llx; size=%zu; tmp_dst=0x%llx)\n",
            ret, (uint64_t)(uintptr_t)dst, (uint64_t)(uintptr_t)src, count, (uint64_t)(uintptr_t)tmp_dst);
        devmm_print_svm_va_info((uint64_t)(uintptr_t)src, ret);
        devmm_print_svm_va_info((uint64_t)(uintptr_t)dst, ret);
        return ret;
    }

    if (fd != NULL) {
        if (memcpy_s(dst, dst_size, tmp_dst, count) != 0) {
            DEVMM_DRV_ERR("Memcpy_s error. (dst=0x%llx; tmp_dst=0x%llx; size=%zu)\n", dst, tmp_dst, count);
#ifndef EMU_ST
            ret = DRV_ERROR_PARA_ERROR;
#endif
        }
        devmm_host_mem_pool_put(info->devid, fd);
        return ret;
    }

    return DRV_ERROR_NONE;
}

#ifndef USEC_PER_SEC
#define USEC_PER_SEC 1000000UL
#endif
#ifndef NSEC_PER_USEC
#define NSEC_PER_USEC 1000UL
#endif
static inline uint64_t devmm_get_time_us(void)
{
    struct timespec ts = {0};

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * USEC_PER_SEC + (uint64_t)ts.tv_nsec / NSEC_PER_USEC;
}

DVresult halMemCpyAsyncInner(DVdeviceptr dst, size_t dest_max, DVdeviceptr src,
    size_t byte_count, uint64_t *copy_fd)
{
    struct devmm_ioctl_arg *fd_arg = NULL;
    struct devmm_ioctl_arg ioctl_arg;
    DVresult ret;

    if ((dest_max < byte_count) || (copy_fd == NULL)) {
        DEVMM_DRV_ERR("Input parameter is error. (dst=0x%llx; src=0x%llx; byte_count=%lu; dest_max=%lu)\n",
            dst, src, byte_count, dest_max);
        return DRV_ERROR_INVALID_VALUE;
    }
    fd_arg = malloc(sizeof(struct devmm_ioctl_arg));
    if (fd_arg == NULL) {
        return DRV_ERROR_OUT_OF_MEMORY;
    }
    fd_arg->data.async_copy_para.dst = dst;
    fd_arg->data.async_copy_para.src = src;
    fd_arg->data.async_copy_para.byte_count= byte_count;
    fd_arg->data.async_copy_para.cpy_state = 0;
    ioctl_arg = *fd_arg;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_ASYNC_MEMCPY, &ioctl_arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Asynchronous copy error. (ret=%d; dst=0x%llx; src=0x%llx; byte_count=%lu)\n",
                      ret, dst, src, byte_count);
        free(fd_arg);
        return ret;
    }
    fd_arg->data.async_copy_para.task_id = ioctl_arg.data.async_copy_para.task_id;
    fd_arg->data.async_copy_para.start_time = devmm_get_time_us();
    *copy_fd = (uint64_t)(uintptr_t)fd_arg;
    DEVMM_DRV_DEBUG_ARG("Asynchronous copy commited. (dst=0x%llx; src=0x%llx; size=%lu; task_id=%d)\n",
        fd_arg->data.async_copy_para.dst, fd_arg->data.async_copy_para.src,
        fd_arg->data.async_copy_para.byte_count, fd_arg->data.async_copy_para.task_id);

    return DRV_ERROR_NONE;
}

DVresult halMemCpyAsync(DVdeviceptr dst, size_t dest_max, DVdeviceptr src, size_t byte_count,
    uint64_t *copy_fd)
{
    return halMemCpyAsyncInner(dst, dest_max, src, byte_count, copy_fd);
}

#define DEVMM_ASYNC_CPY_FINISH_TRY_TIME 5000000UL /* 5000000us */
DVresult halMemCpyAsyncWaitFinishInner(uint64_t copy_fd)
{
    struct devmm_ioctl_arg *arg = (struct devmm_ioctl_arg *)(uintptr_t)copy_fd;
    uint64_t retry_time, finish_val, start_time, now_time;
    DVresult ret = DRV_ERROR_NONE;

    if (arg == NULL) {
        return DRV_ERROR_NOT_EXIST;
    }

    retry_time = DEVMM_ASYNC_CPY_FINISH_TRY_TIME *
        (arg->data.async_copy_para.byte_count/ 65536UL + 1); /* 65536 64k */
    start_time = devmm_get_time_us();
    now_time = start_time;
    while (arg->data.async_copy_para.cpy_state == 0) {
        now_time = devmm_get_time_us();
        if ((now_time - start_time) > retry_time) {
            break;
        }
        ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEMCPY_RESLUT_REFRESH, arg);
        if (ret != DRV_ERROR_NONE) {
            DEVMM_DRV_ERR("Copy result refresh error, check memory validity. "
                "(dst=0x%llx; src=0x%llx; byte_count=%lu; ret=%d)\n",
                arg->data.async_copy_para.dst, arg->data.async_copy_para.src,
                arg->data.async_copy_para.byte_count, ret);
            break;
        }
    }
    finish_val = arg->data.async_copy_para.cpy_state;
    if (finish_val == 0) {
        DEVMM_DRV_ERR("Asynchronous copy timeout. "
            "(dst=0x%llx; src=0x%llx; size=%lu; wait_time=%lluus; dma_time=%lluus)\n",
            arg->data.async_copy_para.dst, arg->data.async_copy_para.src, arg->data.async_copy_para.byte_count,
            (now_time - start_time), now_time - arg->data.async_copy_para.start_time);
        ret = DRV_ERROR_WAIT_TIMEOUT;
    } else if (finish_val != DEVMM_ASYNC_CPY_FINISH_VALUE) {
        DEVMM_DRV_ERR("Asynchronous copy error. (dst=0x%llx; src=0x%llx; byte_count=%lu; dma_status=%llu)\n",
            arg->data.async_copy_para.dst, arg->data.async_copy_para.src,
            arg->data.async_copy_para.byte_count, finish_val);
        ret = DRV_ERROR_INNER_ERR;
    }
    DEVMM_DRV_DEBUG_ARG("Asynchronous copy success. "
        "(dst=0x%llx; src=0x%llx; size=%lu; wait_time=%llu; dma_time=%llu; task_id=%d)\n",
        arg->data.async_copy_para.dst, arg->data.async_copy_para.src, arg->data.async_copy_para.byte_count,
        (now_time - start_time), now_time - arg->data.async_copy_para.start_time,
        arg->data.async_copy_para.task_id);

    free(arg);
    return ret;
}

DVresult halMemCpyAsyncWaitFinish(uint64_t copy_fd)
{
    return halMemCpyAsyncWaitFinishInner(copy_fd);
}

DVresult halMemcpySumbitInner(struct DMA_ADDR *dma_addr, int flag)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    if ((dma_addr == NULL) || (flag >= (int)MEMCPY_SUMBIT_MAX_TYPE) || (flag < (int)MEMCPY_SUMBIT_SYNC)) {
        DEVMM_DRV_ERR("Input parameter is error, please check. (addr_is_null=%u; flag=%d)\n",
            (dma_addr == NULL), flag);
        return DRV_ERROR_INVALID_VALUE;
    }

    /* host agent not surport convert */
    if (dma_addr->virt_id >= DEVMM_MAX_PHY_DEVICE_NUM) {
        DEVMM_DRV_ERR("Virt_id is out of range. (virt_id=%u; range=0-%u)\n",
                      dma_addr->virt_id, (UINT32)(DEVMM_MAX_PHY_DEVICE_NUM - 1));
        return DRV_ERROR_INVALID_VALUE;
    }

    arg.head.devid = dma_addr->virt_id;
    arg.data.convert_copy_para.dmaAddr = *dma_addr;
    arg.data.convert_copy_para.sync_flag = flag;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_SUMBIT_CONVERT_CPY, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Asynchronous copy error. (ret=%d)\n", ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

DVresult halMemcpySumbit(struct DMA_ADDR *dma_addr, int flag)
{
    return halMemcpySumbitInner(dma_addr, flag);
}

DVresult halMemcpyWaitInner(struct DMA_ADDR *dma_addr)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    if (dma_addr == NULL) {
        DEVMM_DRV_ERR("Ptr is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    /* host agent not surport convert */
    if (dma_addr->virt_id >= DEVMM_MAX_PHY_DEVICE_NUM) {
        DEVMM_DRV_ERR("Virt_id is out of range. (virt_id=%u; range=0-%u)\n",
                      dma_addr->virt_id, (UINT32)(DEVMM_MAX_PHY_DEVICE_NUM - 1));
        return DRV_ERROR_INVALID_VALUE;
    }

    arg.head.devid = dma_addr->virt_id;
    arg.data.convert_copy_para.dmaAddr = *dma_addr;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_WAIT_CONVERT_CPY_RESLUT, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Asynchronous copy wait error. (ret=%d)\n", ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

DVresult halMemcpyWait(struct DMA_ADDR *dma_addr)
{
    return halMemcpyWaitInner(dma_addr);
}

/* Sunset interface: external statement has been removed from ascend_hal.h. */
DVresult drvLoadProgram(DVdevice device_id, void *program, unsigned int offset, size_t byte_count, void **v_ptr)
{
    (void)device_id;
    (void)program;
    (void)offset;
    (void)byte_count;
    (void)v_ptr;
    return DRV_ERROR_NOT_SUPPORT;
}

STATIC INLINE DVresult devmm_convert_proc(struct devmm_ioctl_arg *arg, struct DMA_ADDR *dma_addr)
{
    uint64_t dst = arg->data.convrt_para.pDst;
    uint64_t src = arg->data.convrt_para.pSrc;
    uint64_t dpitch = arg->data.convrt_para.dpitch;
    uint64_t spitch = arg->data.convrt_para.spitch;
    uint64_t width = arg->data.convrt_para.len;
    uint64_t height = arg->data.convrt_para.height;
    uint64_t fixed_size = arg->data.convrt_para.fixed_size;
    uint64_t dir = (uint64_t)arg->data.convrt_para.direction;
    void *priv = NULL;
    DVresult ret;

    ret = devmm_convert_pre(&arg->data.convrt_para, &priv);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Convert_address_pre failed. (dst=0x%llx; src=0x%llx; dpitch=%llu; spitch=%llu; width=%llu; "
                      "height=%llu; fixed_size=%llu; direction=%u; ret=%d)\n", dst, src, dpitch, spitch,
                      width, height, fixed_size, dir, ret);
        priv = NULL;
        return ret;
    }

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_CONVERT_ADDR, arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NOT_SUPPORT), "Ioctl convert error. (dst=0x%llx; src=0x%llx; dpitch=%llu; spitch=%llu; width=%llu; "
                      "height=%llu; fixed_size=%llu; direction=%u; ret=%d)\n", dst, src, dpitch, spitch,
                      width, height, fixed_size, dir, ret);
        (void)devmm_destroy_convert_task_node(priv);
        priv = NULL;
        return ret;
    }

    *dma_addr = arg->data.convrt_para.dmaAddr;
    devmm_convert_post(&arg->data.convrt_para, &priv, (void *)dma_addr);

    DEVMM_DRV_DEBUG_ARG("Convert_proc. (dst=0x%llx; src=0x%llx; dpitch=%llu; spitch=%llu; width=%llu; "
                        "height=%llu; fixed_size=%llu; direction=%u; ret=%d; virt_id=%u; fix_size=%u; "
                        "offset=%llu)\n", dst, src, dpitch, spitch, width, height, fixed_size, dir,
                        ret, dma_addr->virt_id, dma_addr->fixed_size, dma_addr->offsetAddr.offset);
    return DRV_ERROR_NONE;
}


DVresult drvMemConvertAddr(DVdeviceptr p_src, DVdeviceptr p_dst, UINT32 len, struct DMA_ADDR *dma_addr)
{
    struct devmm_ioctl_arg arg = {0};

    DEVMM_DRV_DEBUG_ARG("Convert address. (pSrc=0x%llx; pDst=0x%llx; len=%u)\n", p_src, p_dst, len);

    if (dma_addr == NULL) {
        DEVMM_DRV_ERR("Parameter dmaAddr is NULL. (pSrc=0x%llx; pDst=0x%llx; len=%u)\n",
                      p_src, p_dst, len);
        return DRV_ERROR_INVALID_VALUE;
    }

    /* host agent not surport convert */
    if (dma_addr->offsetAddr.devid >= DEVMM_MAX_PHY_DEVICE_NUM) {
        DEVMM_DRV_ERR("Devid is out of range. (devid=%u; range=0~%u)\n", dma_addr->offsetAddr.devid,
                      (UINT32)(DEVMM_MAX_PHY_DEVICE_NUM - 1));
        return DRV_ERROR_INVALID_VALUE;
    }

    arg.head.devid = dma_addr->offsetAddr.devid;
    arg.data.convrt_para.virt_id = dma_addr->offsetAddr.devid;
    arg.data.convrt_para.pSrc = p_src;
    arg.data.convrt_para.pDst = p_dst;
    arg.data.convrt_para.len = len;
    arg.data.convrt_para.dpitch = len;
    arg.data.convrt_para.spitch = len;
    arg.data.convrt_para.height = 1;
    arg.data.convrt_para.fixed_size = 0;
    arg.data.convrt_para.direction = DEVMM_COPY_INVILED_DIRECTION;

    return devmm_convert_proc(&arg, dma_addr);
}

static DVresult devmm_destroy_para_check(struct DMA_ADDR *ptr[], uint32_t num)
{
    uint32_t i;

    if (ptr == NULL) {
        DEVMM_DRV_ERR("Ptr[] is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (num == 0) {
        DEVMM_DRV_ERR("num is 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    for (i = 0; i < num; i++) {
        if (ptr[i] == NULL) {
            DEVMM_DRV_ERR("Ptr is NULL.\n");
            return DRV_ERROR_INVALID_VALUE;
        }

        /* host agent not surport convert */
        if (ptr[i]->virt_id >= DEVMM_MAX_PHY_DEVICE_NUM) {
            DEVMM_DRV_ERR("Virt_id is out of range. (virt_id=%u; range=0-%u)\n",
                ptr[i]->virt_id, (UINT32)(DEVMM_MAX_PHY_DEVICE_NUM - 1));
            return DRV_ERROR_INVALID_VALUE;
        }
    }

    return DRV_ERROR_NONE;
}

static DVresult devmm_destroy_addr(struct DMA_ADDR *ptr)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    arg.head.devid = ptr->virt_id;
    arg.data.desty_para.dmaAddr = *ptr;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_DESTROY_ADDR, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Destroy address error. (offset=%llu; ret=%d)\n",
            arg.data.desty_para.dmaAddr.offsetAddr.offset, ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

static DVresult devmm_destroy_addr_batch(struct DMA_ADDR *ptr[], uint32_t num)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    arg.head.devid = ptr[0]->virt_id;
    arg.data.destroy_batch_para.dmaAddr = ptr;
    arg.data.destroy_batch_para.num = num;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_DESTROY_ADDR_BATCH, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Destroy address batch error. (ret=%d)\n", ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

DVresult drvMemDestroyAddr(struct DMA_ADDR *ptr)
{
    DVresult ret;

    DEVMM_DRV_DEBUG_ARG("Argument. (ptr=0x%p)\n", ptr);
    ret = devmm_destroy_para_check(&ptr, 1);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = devmm_destroy_addr(ptr);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = devmm_destroy_convert((void *)ptr);
    if (ret != DRV_ERROR_NONE) {
        /* do not need print */
        return ret;
    }

    return DRV_ERROR_NONE;
}

static DVresult devmm_destroy_post(struct DMA_ADDR *ptr[], uint32_t num)
{
    DVresult ret;
    uint32_t i;

    for (i = 0; i < num; i++) {
        ret = devmm_destroy_convert((void *)ptr[i]);
        if (ret != DRV_ERROR_NONE) {
            /* do not need print */
            return ret;
        }
    }

    return DRV_ERROR_NONE;
}

DVresult halMemDestroyAddrBatch(struct DMA_ADDR *ptr[], uint32_t num)
{
    DVresult ret;

    ret = devmm_destroy_para_check(ptr, num);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    if (num == 1) {
        ret = devmm_destroy_addr(ptr[0]);
    } else {
        ret = devmm_destroy_addr_batch(ptr, num);
    }
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return devmm_destroy_post(ptr, num);
}

STATIC void devmm_init_mem_advise_record(void)
{
    uint32_t i;

    for (i = 0; i < ADVISE_RECORD_MAX_DEV_NUM; i++) {
        SVM_INIT_LIST_HEAD(&advise_record_list[i]);
        pthread_mutex_init(&advise_record_lock[i], NULL);
    }
}

STATIC void devmm_add_mem_advise_record(DVdeviceptr ptr, size_t count, DVmem_advise advise, DVdevice device)
{
    struct devmm_advise_record_node *record_node = NULL;
    struct devmm_virt_list_head *pos = NULL;
    struct devmm_virt_list_head *n = NULL;

    if ((device >= ADVISE_RECORD_MAX_DEV_NUM) || (advise != DV_ADVISE_PERSISTENT)) {
        return;
    }

    pthread_mutex_lock(&advise_record_lock[device]);
    devmm_virt_list_for_each_safe(pos, n, &advise_record_list[device]) {
        record_node = devmm_virt_list_entry(pos, struct devmm_advise_record_node, node);
        if ((record_node->ptr == ptr) && (record_node->count == count)) {
            pthread_mutex_unlock(&advise_record_lock[device]);
            return;
        }
    }

    record_node = (struct devmm_advise_record_node *)malloc(sizeof(struct devmm_advise_record_node));
    if (record_node == NULL) {
        DEVMM_DRV_WARN("Malloc not success, not add advise record.\n");
        pthread_mutex_unlock(&advise_record_lock[device]);
        return;
    }

    record_node->ptr = ptr;
    record_node->count = count;
    SVM_INIT_LIST_HEAD(&record_node->node);
    devmm_virt_list_add_tail(&record_node->node, &advise_record_list[device]);
    DEVMM_DRV_DEBUG_ARG("Add mem advise record. (advise=%x; ptr=0x%llx; count=%lu; device=%u)\n",
                    (uint32_t)advise, ptr, count, device);

    pthread_mutex_unlock(&advise_record_lock[device]);
}

STATIC bool devmm_mem_advise_range_is_overlap(DVdeviceptr free_ptr, size_t free_cnt,
    struct devmm_advise_record_node *record_node)
{
    if ((free_ptr + free_cnt <= record_node->ptr) || (free_ptr >= record_node->ptr + record_node->count)) {
        return false;
    }
    return true;
}

STATIC void devmm_del_mem_advise_record(bool is_del_all, DVdeviceptr free_ptr, size_t free_cnt, DVdevice device)
{
    struct devmm_advise_record_node *record_node = NULL;
    struct devmm_virt_list_head *pos = NULL;
    struct devmm_virt_list_head *n = NULL;

    if (device >= ADVISE_RECORD_MAX_DEV_NUM) {
        return;
    }

    pthread_mutex_lock(&advise_record_lock[device]);
    devmm_virt_list_for_each_safe(pos, n, &advise_record_list[device]) {
        record_node = devmm_virt_list_entry(pos, struct devmm_advise_record_node, node);
        if ((is_del_all == true) || devmm_mem_advise_range_is_overlap(free_ptr, free_cnt, record_node)) {
            devmm_virt_list_del(&record_node->node);
            DEVMM_DRV_DEBUG_ARG("Del mem advise record. (ptr=0x%llx; count=%lu; device=%u; is_del_all=%u)\n",
                record_node->ptr, record_node->count, device, is_del_all);
            free(record_node);
        }
    }
    pthread_mutex_unlock(&advise_record_lock[device]);
}

STATIC DVresult devmm_mem_ioctl_advise(DVdeviceptr ptr, size_t count, DVmem_advise advise, DVdevice device)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    arg.head.devid = device;
    arg.data.advise_para.ptr = ptr;
    arg.data.advise_para.count = count;
    arg.data.advise_para.advise = advise;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_ADVISE, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Ioctl device error. (ret=%d; ptr=0x%llx; count=%lu; advise=%x; device=%u; devid=%u)\n",
                      ret, ptr, count, (uint32_t)advise, device, arg.head.devid);
        return ret;
    }
    return ret;
}

STATIC DVresult devmm_mem_advise_restore_per_dev(DVdevice device)
{
    struct devmm_advise_record_node *record_node = NULL;
    struct devmm_virt_list_head *pos = NULL;
    struct devmm_virt_list_head *n = NULL;
    DVresult ret;

    pthread_mutex_lock(&advise_record_lock[device]);
    devmm_virt_list_for_each_safe(pos, n, &advise_record_list[device]) {
        record_node = devmm_virt_list_entry(pos, struct devmm_advise_record_node, node);
        ret = devmm_mem_ioctl_advise(record_node->ptr, record_node->count, DV_ADVISE_PERSISTENT, device);
        if (ret != DRV_ERROR_NONE) {
            pthread_mutex_unlock(&advise_record_lock[device]);
            return ret;
        }

        DEVMM_RUN_INFO("Mem advise restore succ. (ptr=0x%llx; count=%lu; device=%u)\n",
            record_node->ptr, record_node->count, device);
    }
    pthread_mutex_unlock(&advise_record_lock[device]);
    return DRV_ERROR_NONE;
}

STATIC DVresult devmm_mem_advise_restore(void)
{
    DVresult ret;
    uint32_t i;

    for (i = 0; i < ADVISE_RECORD_MAX_DEV_NUM; i++) {
        ret = devmm_mem_advise_restore_per_dev(i);
        if (ret != DRV_ERROR_NONE) {
            DEVMM_DRV_ERR("Mem advise restore failed. (device=%u; ret=%d)\n", i, ret);
            return ret;
        }
    }
    return DRV_ERROR_NONE;
}

STATIC DVresult drv_mem_advise(DVdeviceptr ptr, size_t count, DVmem_advise advise, DVdevice device)
{
    DVresult ret;

    if (!DEVMM_IS_SVM_ADDR(ptr) || (device >= SVM_MAX_AGENT_NUM) || (devmm_dev_is_inited((uint32_t)device) == false)) {
        DEVMM_DRV_ERR("Input paremeter error. (ptr=0x%llx; count=%lu; advise=%x; device=%u)\n",
                      ptr, count, (uint32_t)advise, device);
        return DRV_ERROR_INVALID_VALUE;
    }
    /* DV_ADVISE_DDR and DV_ADVISE_P2P_HBM can just set one */
    if (((advise & DV_ADVISE_DDR) != 0) && ((advise & DV_ADVISE_P2P_HBM) != 0)) {
        DEVMM_DRV_ERR("Advise parameter error. (ptr=0x%llx; count=%lu; advise=%x; device=%u)\n",
                      ptr, count, (uint32_t)advise, device);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = devmm_mem_ioctl_advise(ptr, count, advise, device);
    if (ret) {
        DEVMM_DRV_ERR("Advise ioctl failed. (ptr=0x%llx; count=%lu; advise=%x; device=%u)\n",
                      ptr, count, (uint32_t)advise, device);
        return ret;
    }

    devmm_add_mem_advise_record(ptr, count, advise, device);
    DEVMM_DRV_DEBUG_ARG("Argument. (ptr=0x%llx; count=0x%llx; advise=0x%x; device=%u)\n", ptr, count, (uint32_t)advise,
        device);

    return DRV_ERROR_NONE;
}

DVresult drvMemPrefetchToDevice(DVdeviceptr dev_ptr, size_t len, DVdevice device)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    if (!DEVMM_IS_SVM_ADDR(dev_ptr) || (device >= SVM_MAX_AGENT_NUM)) {
        DEVMM_DRV_ERR("DevPtr is error. (dev_ptr=0x%llx; len=%lu; device=%u)\n", dev_ptr, len, device);
        return DRV_ERROR_INVALID_VALUE;
    }

    arg.data.prefetch_para.ptr = dev_ptr;
    arg.data.prefetch_para.count = len;
    arg.head.devid = device;
    DEVMM_DRV_DEBUG_ARG("Argument. (dev_ptr=0x%llx; len=%lu; device=%u)\n", dev_ptr, len, device);

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_PREFETCH, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NOT_SUPPORT), "Ioctl device error. (ret=%d; dev_ptr=0x%llx; len=%lu; device=%u; arg_head_devid=%u)\n",
            (int)ret, dev_ptr, len, device, arg.head.devid);
        devmm_print_svm_va_info(dev_ptr, ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

#ifndef EMU_ST
/* because memset_s 2nd para must less than(or equal to) 2G, devmm will set big memory by loop */
STATIC DVresult devmm_set_host(DVdeviceptr dst, size_t dst_max, UINT8 value, size_t count)
{
    size_t rest_count, per_count;
    int ret;

    if (dst_max < count) {
        DEVMM_DRV_ERR("Count bigger than dst_max. (dst=0x%llx; count=%lu; dst_max=%lu)\n", dst, count, dst_max);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (count <= SECUREC_MEM_MAX_LEN && dst_max <= SECUREC_MEM_MAX_LEN) {
        ret = memset_s((void *)(uintptr_t)dst, dst_max, value, count);
        if (ret != 0) {
            DEVMM_DRV_ERR("Memset error. (dst=0x%llx; dst_max=%llu; value=%u; count=%llu; ret=%d)\n",
                          dst, dst_max, value, count, ret);
            return DRV_ERROR_INVALID_VALUE;
        }
    } else if (count <= SECUREC_MEM_MAX_LEN && dst_max > SECUREC_MEM_MAX_LEN) {
        ret = memset_s((void *)(uintptr_t)dst, SECUREC_MEM_MAX_LEN, value, count);
        if (ret != 0) {
            DEVMM_DRV_ERR("Memset error. (dst=0x%llx; dst_max=%llu; value=%u; count=%llu; ret=%d)\n",
                          dst, SECUREC_MEM_MAX_LEN, value, count, ret);
            return DRV_ERROR_INVALID_VALUE;
        }
    } else if (count > SECUREC_MEM_MAX_LEN) {
        for (rest_count = count; rest_count > 0;) {
            per_count = rest_count > SECUREC_MEM_MAX_LEN ? SECUREC_MEM_MAX_LEN : rest_count;
            ret = memset_s((void *)(uintptr_t)dst, per_count, value, per_count);
            if (ret != 0) {
                DEVMM_DRV_ERR("Memset error. (dst=0x%llx; value=%u; per_count=%lu; ret=%d)\n",
                              dst, value, per_count, ret);
                return DRV_ERROR_INVALID_VALUE;
            }
            dst = dst + per_count;
            rest_count = rest_count - per_count;
        }
    }

    return DRV_ERROR_NONE;
}
#endif

DVresult drvMemsetD8Inner(DVdeviceptr dst, size_t dest_max, UINT8 value, size_t num)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    DEVMM_DRV_DEBUG_ARG("Show information. (dst=0x%llx; value=%u; num=%lu)\n", dst, value, num);

    if ((dst == (DVdeviceptr)NULL) || (dest_max < num) || (num == 0)) {
        DEVMM_DRV_ERR("Input parameter error. (dst=0x%llx; value=%u; num=%lu)\n", dst, value, num);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (!DEVMM_IS_SVM_ADDR(dst)) {
        return devmm_set_host(dst, dest_max, value, num);
    }

    arg.data.memset_para.dst = dst;
    arg.data.memset_para.value = value;
    arg.data.memset_para.count = num;
    arg.data.memset_para.hostmapped = 0;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEMSET8, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Memset error. (dst=0x%llx; value=%u; num=%lu; ret=%d)\n",
                      dst, value, num, ret);
        return ret;
    }

    if (arg.data.memset_para.hostmapped != 0) {
        return devmm_set_host(dst, dest_max, value, num);
    }

    return DRV_ERROR_NONE;
}

DVresult drvMemsetD8(DVdeviceptr dst, size_t destMax, UINT8 value, size_t num)
{
    return drvMemsetD8Inner(dst, destMax, value, num);
}

DVresult drvMemAddressTranslate(DVdeviceptr vptr, UINT64 *pptr)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    if ((pptr == NULL) || !DEVMM_IS_SVM_ADDR(vptr)) {
        DEVMM_DRV_ERR("Vptr is invalid. (vptr=0x%llx)\n", vptr);
        return DRV_ERROR_INVALID_VALUE;
    }

    arg.data.translate_para.vptr = vptr;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_TRANSLATE, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Ioctl device error. (vptr=0x%llx; ret=%d)\n", vptr, ret);
        return ret;
    }

    *pptr = (UINT64)arg.data.translate_para.pptr;

    DEVMM_DRV_DEBUG_ARG("Argument. (vptr=0x%llx; *pptr=0x%llx)\n", vptr, *pptr);

    return DRV_ERROR_NONE;
}

/* Sunset interface: external statement has been removed from ascend_hal.h. */
DVresult drvMemAllocL2buffAddr(DVdevice device, void **l2buff, UINT64 *pte)
{
    struct AddrMapInPara in_para;
    struct AddrMapOutPara out_para;
    size_t in_size = sizeof(struct AddrMapInPara);
    size_t out_size = sizeof(struct AddrMapOutPara);
    DVresult ret;
#ifndef DEVMM_UT
    /* host agent not surport l2buff */
    if ((l2buff == NULL) || (device >= DEVMM_MAX_PHY_DEVICE_NUM)) {
        DEVMM_DRV_ERR("Input parameter error. (l2buff=%p; pte=%p; device=%u)\n", l2buff, pte, device);
        return DRV_ERROR_INVALID_VALUE;
    }
#endif
    in_para.devid = device;
    in_para.addr_type = ADDR_MAP_TYPE_L2_BUFF;

    ret = devmm_ctrl_map_addr((void *)&in_para, in_size, (void *)&out_para, &out_size);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Map l2buff failed. (ret=%d)\n", ret);
        return ret;
    }

    *l2buff = (void *)(uintptr_t)out_para.ptr;
    DEVMM_DRV_DEBUG_ARG("Argument. (device=%d; *l2buff=%p)\n", device, *l2buff);

    return DRV_ERROR_NONE;
}

/* Sunset interface: external statement has been removed from ascend_hal.h. */
DVresult drvMemReleaseL2buffAddr(DVdevice device, void *l2buff)
{
    uint32_t addr_type = ADDR_MAP_TYPE_L2_BUFF;
    struct AddrUnmapInPara in_para;
    uint64_t p, size;
    DVresult ret;
#ifndef DEVMM_UT
    if (device >= DEVMM_MAX_PHY_DEVICE_NUM) {
        DEVMM_DRV_ERR("Device id error. (l2buff=%p; device=%u)\n", l2buff, device);
        return DRV_ERROR_INVALID_VALUE;
    }
#endif
    devmm_get_dev_reserve_addr(device, addr_type, &p, &size);
    in_para.devid = device;
    in_para.addr_type = addr_type;
    in_para.ptr = (UINT64)(uintptr_t)l2buff;
    in_para.len = size;
    ret = devmm_ctrl_unmap_addr((void *)&in_para, sizeof(struct AddrUnmapInPara), NULL, NULL);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Unmap l2buff failed. (ret=%d)\n", ret);
        return ret;
    }
    DEVMM_DRV_DEBUG_ARG("Argument. (device=%d; l2buff=%p; p=0x%llx; size=%llu)\n", device, l2buff, p, size);

    return DRV_ERROR_NONE;
}

static DVresult devmm_shmem_destroy_handle(const char *name, size_t str_len)
{
    struct devmm_ioctl_arg arg = {0};

    if (memcpy_s(arg.data.ipc_destroy_para.name, sizeof(arg.data.ipc_destroy_para.name), name, str_len) != 0) {
#ifndef EMU_ST
        DEVMM_DRV_ERR("Memcpy_s error. (str_len=%lu)\n", str_len);
        return DRV_ERROR_INVALID_VALUE;
#endif
    }
    return devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_IPC_MEM_DESTROY, &arg);
}

DVresult halShmemCreateHandle(DVdeviceptr vptr, size_t byte_count, char *name, uint32_t name_len)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    if ((name == NULL) || (name_len < DEVMM_MAX_NAME_SIZE)) {
#ifndef EMU_ST
        DEVMM_DRV_ERR("Invalid input. (name_len=%u; minimum_len=%u; vptr=0x%llx)\n", name_len, DEVMM_MAX_NAME_SIZE, vptr);
#endif
        return DRV_ERROR_INVALID_VALUE;
    }

    DEVMM_DRV_DEBUG_ARG("Create share memory. (vptr=0x%llx; size=%lu)\n", vptr, byte_count);
    arg.data.ipc_create_para.vptr = vptr;
    arg.data.ipc_create_para.len = byte_count;
    arg.data.ipc_create_para.name_len = name_len;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_IPC_MEM_CREATE, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Share memory create device error. (ret=%d; vptr=0x%llx)\n", ret, vptr);
        return ret;
    }

    if (memcpy_s(name, name_len, arg.data.ipc_create_para.name, DEVMM_MAX_NAME_SIZE) != 0) {
        DEVMM_DRV_ERR("Memcpy_s error. (name_len=%u; vptr=0x%llx)\n", name_len, vptr);
        (void)devmm_shmem_destroy_handle(arg.data.ipc_create_para.name, DEVMM_MAX_NAME_SIZE);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

static DVresult halShmemSetPid(const char *name, uint32_t sdid, int pid[], int num, unsigned long cmd)
{
    struct devmm_ioctl_arg arg = {0};
    uint64_t str_len;
    DVresult ret;
    int i;

    /* IpcCreate takes one, DEVMM_SVM_MAX_PROCESS_NUM - 1 left */
    if ((pid == NULL) || (num > (DEVMM_SECONDARY_PROCESS_NUM - 1)) || (num <= 0)) {
        DEVMM_DRV_ERR("Invalid input. (pid_null=%d; num=%d)\n", (pid == NULL), num);
        return DRV_ERROR_INVALID_VALUE;
    }

    str_len = (name != NULL) ? strnlen(name, DEVMM_MAX_NAME_SIZE) : 0;
    if ((str_len == 0) || (str_len >= DEVMM_MAX_NAME_SIZE)) {
        DEVMM_DRV_ERR("Invalid input. (str_len=%lu)\n", str_len);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (memcpy_s(arg.data.ipc_set_pid_para.name, sizeof(arg.data.ipc_set_pid_para.name), name, str_len) != 0) {
        DEVMM_DRV_ERR("Memcpy_s error. (str_len=%lu)\n", str_len);
        return DRV_ERROR_INVALID_VALUE;
    }
    arg.data.ipc_set_pid_para.sdid = sdid;
    arg.data.ipc_set_pid_para.num = (uint32_t)num;
    for (i = 0; i < num; i++) {
        DEVMM_DRV_DEBUG_ARG("Shmmem_set_pid. (pid=%d; idx=%d)\n", pid[i], i);
        arg.data.ipc_set_pid_para.set_pid[i] = pid[i];
    }
    ret = devmm_svm_ioctl(g_devmm_mem_dev, cmd, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Shmmem_set_pid error. (ret=%d; num=%d; pid=%d)\n", ret, num, pid[0]);
        return ret;
    }

    return DRV_ERROR_NONE;
}

DVresult halShmemSetPidHandle(const char *name, int pid[], int num)
{
    return halShmemSetPid(name, UINT32_MAX, pid, num, DEVMM_SVM_IPC_MEM_SET_PID);
}

DVresult halShmemSetPodPid(const char *name, uint32_t sdid, int pid[], int num)
{
    return halShmemSetPid(name, sdid, pid, num, DEVMM_SVM_IPC_MEM_SET_PID_POD);
}

DVresult halShmemDestroyHandle(const char *name)
{
    size_t str_len;
    DVresult ret;

    str_len = (name != NULL) ? strnlen(name, DEVMM_MAX_NAME_SIZE) : 0;
    if ((str_len == 0) || (str_len >= DEVMM_MAX_NAME_SIZE)) {
        DEVMM_DRV_ERR("Invalid input. (str_len=%lu)\n", str_len);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = devmm_shmem_destroy_handle(name, str_len);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Destroy share memory error. (ret=%d)\n", (int)ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

STATIC DVresult devmm_ipc_mem_query(struct devmm_ioctl_arg *arg, const char *name, size_t str_len)
{
    DVresult ret;

    if (memcpy_s(arg->data.query_size_para.name, sizeof(arg->data.query_size_para.name), name, str_len)!= 0) {
        DEVMM_DRV_ERR("Memcpy_s error. (str_len=%lu)\n", str_len);
        return DRV_ERROR_INVALID_VALUE;
    }

    arg->data.query_size_para.len = 0;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_IPC_MEM_QUERY, arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Memory query error. (ret=%d)\n", ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

void devmm_set_module_id_to_advise(uint32_t model_id, DVmem_advise *advise)
{
    *advise = *advise | ((model_id & DV_ADVISE_MODULE_ID_MASK) << DV_ADVISE_MODULE_ID_BIT);
}

STATIC DVresult devmm_ipc_mem_open(struct devmm_ioctl_arg *arg,
    DVdeviceptr *ptr, const char *name, size_t str_len, uint32_t devid)
{
    DVmem_advise advise = 0;
    DVresult ret;

    advise |= (arg->data.query_size_para.is_huge != 0) ? DV_ADVISE_HUGEPAGE : 0;
    devmm_set_module_id_to_advise(HCCL, &advise);
    ret = devmm_alloc_proc(0, SUB_SVM_TYPE, advise, arg->data.query_size_para.len, ptr);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Alloc memory failed. (len=%lu; is_huge=%d; ptr=0x%llx)\n", arg->data.query_size_para.len,
            arg->data.query_size_para.is_huge, *ptr);
        return ret;
    }

    if (memcpy_s(arg->data.ipc_open_para.name, sizeof(arg->data.ipc_open_para.name), name, str_len) != 0) {
        DEVMM_DRV_ERR("Memcpy_s error. (str_len=%lu; ptr=0x%llx)\n", str_len, *ptr);
        (void)devmm_free_managed(*ptr);
        return DRV_ERROR_INVALID_VALUE;
    }

    arg->head.devid = devid;
    arg->data.ipc_open_para.vptr = *ptr;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_IPC_MEM_OPEN, arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Open memory error. (ret=%d; *ptr=0x%llx)\n", ret, *ptr);
        (void)devmm_free_managed(*ptr);
        return ret;
    }

    return DRV_ERROR_NONE;
}

#define DEVMM_SERVER_ID_IN_SDID_OFFSET 22
#define DEVMM_SERVER_ID_IN_KEY_OFFSET 54
#define DEVMM_REF_MASK 0x3FFFFFFFFFFFFF
/* key: server_id(10 bit) + ref(54 bit) */
static uint64_t devmm_ipc_name_to_key(const char *name)
{
    uint32_t devid, vfid, sdid, server_id;
    uint64_t ref, key;
    int pid, ret;

    ret = sscanf_s(name, "%08x%016llx%02x%02x%08x", &pid, &ref, &devid, &vfid, &sdid);
    if (ret != 5) { /* 5 for para nums */
        return DEVMM_RECORD_KEY_INVALID;
    }
    server_id = (sdid >> DEVMM_SERVER_ID_IN_SDID_OFFSET);
    key = (((uint64_t)server_id << DEVMM_SERVER_ID_IN_KEY_OFFSET) | (ref & DEVMM_REF_MASK));
    return key;
}

DVresult halShmemOpenHandleByDevId(DVdevice dev_id, const char *name, DVdeviceptr *vptr)
{
    struct devmm_ioctl_arg arg = {0};
    DVdeviceptr ptr = (DVdeviceptr)0;
    struct devmm_record_data data = {0};
    size_t str_len, mem_len;
    DVresult ret;

    str_len = (name != NULL) ? strnlen(name, DEVMM_MAX_NAME_SIZE) : 0;
    if ((str_len == 0) || (str_len >= DEVMM_MAX_NAME_SIZE) || (vptr == NULL)) {
        DEVMM_DRV_ERR("Invalid input. (str_len=%lu; vptr=%p)\n", str_len, vptr);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (dev_id >= DEVMM_MAX_DEVICE_NUM) {
#ifndef EMU_ST
        DEVMM_DRV_ERR("Invalid devid. (devid=%u)\n", dev_id);
#endif
        return DRV_ERROR_INVALID_VALUE;
    }

    data.key1 = devmm_ipc_name_to_key(name);
    data.key2 = DEVMM_RECORD_KEY_INVALID;
    ret = devmm_record_create_and_get(DEVMM_FEATURE_IPC, dev_id, DEVMM_KEY_TYPE1, DEVMM_NODE_INITING, &data);
    if (ret != DRV_ERROR_TRY_AGAIN) {
        if (ret == DRV_ERROR_NONE) {
            *vptr = data.key2;
        }
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NONE), "Record get failed. (devId=%u; ret=%d)\n", dev_id, ret);
        return ret;
    }

    ret = devmm_ipc_mem_query(&arg, name, str_len);
    if (ret != DRV_ERROR_NONE) {
#ifndef EMU_ST
        (void)devmm_record_put(DEVMM_FEATURE_IPC, dev_id, DEVMM_KEY_TYPE1, data.key1, DEVMM_NODE_UNINITED);
#endif
        DEVMM_DRV_ERR("Memory query error. (ret=%d)\n", ret);
        return ret;
    }
    mem_len = arg.data.query_size_para.len;
    ret = devmm_ipc_mem_open(&arg, &ptr, name, str_len, dev_id);
    if (ret != DRV_ERROR_NONE) {
        (void)devmm_record_put(DEVMM_FEATURE_IPC, dev_id, DEVMM_KEY_TYPE1, data.key1, DEVMM_NODE_UNINITED);
        DEVMM_DRV_ERR("Alloc ipc memory and set name failed. (ret=%d)\n", ret);
        return ret;
    }
    /* In the same OS, the service does not invoke the prefetch. */
    ret = drvMemPrefetchToDevice(ptr, mem_len, dev_id);
    if (ret != DRV_ERROR_NONE) {
#ifndef EMU_ST
        (void)halShmemCloseHandle(ptr);
        (void)devmm_record_put(DEVMM_FEATURE_IPC, dev_id, DEVMM_KEY_TYPE1, data.key1, DEVMM_NODE_UNINITED);
        DEVMM_DRV_ERR("Prefetch failed. (devid=%d; ret=%d)\n", dev_id, ret);
#endif
        return ret;
    }

    data.key2 = ptr;
    (void)devmm_record_create_and_get(DEVMM_FEATURE_IPC, dev_id, DEVMM_KEY_TYPE1, DEVMM_NODE_INITED, &data);
    *vptr = ptr;
    DEVMM_DRV_DEBUG_ARG("ShmemOpenHandle succeeded. (devid=%u; open_ptr=0x%llx)\n", dev_id, ptr);

    return DRV_ERROR_NONE;
}

DVresult halShmemOpenHandle(const char *name, DVdeviceptr *vptr)
{
    return halShmemOpenHandleByDevId(svm_devid, name, vptr);
}

DVresult halShmemCloseHandle(DVdeviceptr vptr)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    DEVMM_DRV_DEBUG_ARG("ShmemCloseHandle. (vptr=0x%llx)\n", vptr);

    ret = devmm_record_put(DEVMM_FEATURE_IPC, DEVMM_RECORD_INVALID_DEVID, DEVMM_KEY_TYPE2, vptr, DEVMM_NODE_UNINITING);
    if (ret != DRV_ERROR_NONE) {
        return (ret == DRV_ERROR_INNER_ERR) ? ret : DRV_ERROR_NONE;
    }

    arg.data.ipc_close_para.vptr = vptr;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_IPC_MEM_CLOSE, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Close error. (vptr=0x%llx)\n", vptr);
        return ret;
    }

    ret = devmm_free_managed(vptr);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_WARN("Free memory failed. (vptr=0x%llx)\n", vptr);
    }

    ret = devmm_record_put(DEVMM_FEATURE_IPC, DEVMM_RECORD_INVALID_DEVID, DEVMM_KEY_TYPE2, vptr, DEVMM_NODE_UNINITED);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return DRV_ERROR_NONE;
}

DVresult halShmemSetAttribute(const char *name, uint32_t type, uint64_t attr)
{
    struct devmm_ioctl_arg arg = {0};
    size_t str_len;
    DVresult ret;

    str_len = (name != NULL) ? strnlen(name, DEVMM_MAX_NAME_SIZE) : 0;
    if ((str_len == 0) || (str_len >= DEVMM_MAX_NAME_SIZE)) {
        DEVMM_DRV_ERR("Invalid name. (str_len=%zu)\n", str_len);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (type >= SHMEM_ATTR_TYPE_MAX) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (memcpy_s(arg.data.ipc_set_attr_para.name, sizeof(arg.data.ipc_set_attr_para.name), name, str_len) != 0) {
        DEVMM_DRV_ERR("Memcpy_s error. (str_len=%zu)\n", str_len);
        return DRV_ERROR_INVALID_VALUE;
    }

    arg.data.ipc_set_attr_para.type = type;
    arg.data.ipc_set_attr_para.attr = attr;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_IPC_MEM_SET_ATTR, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NOT_SUPPORT), "Shmem set attribute failed. (ret=%d)\n", ret);
        return ret;
    }

    DEVMM_DRV_DEBUG_ARG("Shmem set attribute successfully. (name=%s; type=%u; attr=%llu)\n", name, type, attr);

    return DRV_ERROR_NONE;
}

DVresult halShmemGetAttribute(const char *name, enum ShmemAttrType type, uint64_t *attr)
{
    struct devmm_ioctl_arg arg = {0};
    size_t str_len;
    DVresult ret;

    str_len = (name != NULL) ? strnlen(name, DEVMM_MAX_NAME_SIZE) : 0;
    if ((str_len == 0) || (str_len >= DEVMM_MAX_NAME_SIZE) || (attr == NULL)) {
        DEVMM_DRV_ERR("Invalid para. (str_len=%zu; attr_is_null=%u)\n", str_len, (attr == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((u32)type >= SHMEM_ATTR_TYPE_MAX) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (memcpy_s(arg.data.ipc_set_attr_para.name, sizeof(arg.data.ipc_set_attr_para.name), name, str_len) != 0) {
        DEVMM_DRV_ERR("Memcpy_s error. (str_len=%zu)\n", str_len);
        return DRV_ERROR_INVALID_VALUE;
    }
    arg.data.ipc_set_attr_para.type = type;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_IPC_MEM_GET_ATTR, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NOT_SUPPORT), "Shmem get attribute failed. (ret=%d; type=%u)\n", ret, type);
        return ret;
    }
    *attr = arg.data.ipc_set_attr_para.attr;

    DEVMM_DRV_DEBUG_ARG("Shmem get attribute successfully. (name=%s; type=%u; attr=%llu)\n", name, type, *attr);

    return DRV_ERROR_NONE;
}

DVresult halShmemInfoGet(const char *name, struct ShmemGetInfo *info)
{
    struct devmm_ioctl_arg arg = {0};
    size_t str_len;
    DVresult ret;

    str_len = (name != NULL) ? strnlen(name, DEVMM_MAX_NAME_SIZE) : 0;
    if ((str_len == 0) || (str_len >= DEVMM_MAX_NAME_SIZE) || (info == NULL)) {
        DEVMM_DRV_ERR("Invalid para. (str_len=%zu; info_is_null=%u)\n", str_len, (info == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = devmm_ipc_mem_query(&arg, name, str_len);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Memory query error. (ret=%d; name=%s)\n", ret, name);
        return ret;
    }
    info->phyDevid = arg.data.query_size_para.phy_devid;
    DEVMM_DRV_DEBUG_ARG("Shmem get info successfully. (name=%s; phyDevid=%u)\n", name, info->phyDevid);

    return DRV_ERROR_NONE;
}

STATIC DVresult devmm_get_attribute(DVdeviceptr vptr, struct DVattribute *attr, struct devmm_virt_com_heap *heap,
    struct devmm_virt_heap_mgmt *p_heap_mgmt)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    arg.data.status_va_info_para.va = vptr;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_DBG_VA_STATUS, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Ioctl device error. (ret=%d)\n", ret);
        return ret;
    }
    attr->memType = arg.data.status_va_info_para.mem_type;
    attr->devId = arg.data.status_va_info_para.devid;
    attr->pageSize = (heap->heap_type == DEVMM_HEAP_HUGE_PAGE) ?
        p_heap_mgmt->huge_page_size : p_heap_mgmt->svm_page_size;
    return DRV_ERROR_NONE;
}

bool svm_support_get_user_malloc_attr(uint32_t dev_id)
{
    (void)dev_id;
    g_get_user_malloc_attr = true;
    return true;
}

STATIC INLINE DVresult devmm_get_host_memory_attribute(struct devmm_virt_com_heap *heap,
    struct devmm_virt_heap_mgmt *p_heap_mgmt, struct DVattribute *attr)
{
    if ((heap == NULL) || (heap->heap_type == DEVMM_HEAP_PINNED_HOST)) {
        if (g_get_user_malloc_attr) {
            attr->memType = (heap == NULL) ? DV_MEM_USER_MALLOC : DV_MEM_LOCK_HOST;
        } else {
            attr->memType = DV_MEM_LOCK_HOST;
        }
    }

    if ((heap != NULL) && (heap->heap_list_type == HOST_AGENT_LIST)) {
        attr->memType = DV_MEM_LOCK_HOST_AGENT;
    }
    attr->pageSize = p_heap_mgmt->local_page_size;
    return DRV_ERROR_NONE;
}

STATIC INLINE bool devmm_heap_is_host_attribute(struct devmm_virt_com_heap *heap)
{
    if ((heap == NULL) || (heap->heap_type == DEVMM_HEAP_PINNED_HOST) ||
        (heap->heap_list_type == HOST_AGENT_LIST)) {
        return true;
    }
    return false;
}

static int devmm_get_reserve_addr_attr(struct devmm_virt_heap_mgmt *p_heap_mgmt,
    struct devmm_virt_com_heap *heap, uint64_t va, struct DVattribute *attr)
{
    (void)heap;
    uint32_t side, devid;
    int ret;

    ret = devmm_get_map_info(va, &side, &devid);
    if (ret != 0) {
        DEVMM_DRV_ERR("Va is invalid. (va=0x%llx; ret=%d)\n", va, ret);
        return ret;
    }

    if ((side != MEM_HOST_SIDE) && (side != MEM_DEV_SIDE)) {
        DEVMM_DRV_ERR("Addr hasn't been mapped. (va=0x%llx)\n", va);
        return DRV_ERROR_INVALID_HANDLE;
    }
    attr->devId = devid;
    attr->pageSize = (side == MEM_HOST_SIDE) ? p_heap_mgmt->local_page_size : p_heap_mgmt->huge_page_size;
    attr->memType = (side == MEM_HOST_SIDE) ? DV_MEM_LOCK_HOST : DV_MEM_LOCK_DEV;
    return DRV_ERROR_NONE;
}

static int devmm_get_dyn_reserve_addr_attr(struct devmm_virt_heap_mgmt *p_heap_mgmt,
    uint64_t va, struct DVattribute *attr)
{
    struct devmm_mem_info mem_info;
    uint32_t host_id = SVM_MAX_AGENT_NUM;
    int ret;

    (void)halGetHostID(&host_id);

    ret = devmm_get_reserve_addr_info(va, &mem_info);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Get add info failed. (va=0x%llx)\n", va);
        return ret;
    }

    attr->pageSize = p_heap_mgmt->huge_page_size;
    attr->devId = mem_info.devid;
    attr->memType = (mem_info.devid == host_id) ? DV_MEM_LOCK_HOST : DV_MEM_LOCK_DEV;
    return DRV_ERROR_NONE;
}

DVresult drvMemGetAttribute(DVdeviceptr vptr, struct DVattribute *attr)
{
    struct devmm_virt_heap_mgmt *p_heap_mgmt = NULL;
    struct devmm_virt_com_heap *heap = NULL;

    if ((vptr == 0) || (attr == NULL)) {
        DEVMM_DRV_ERR("Vptr or attr is NULL. (vptr=0x%llx)\n", vptr);
        return DRV_ERROR_INVALID_VALUE;
    }

    p_heap_mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (p_heap_mgmt == NULL) {
        DEVMM_DRV_ERR("Please call device_open and alloc_mem api first. (alloc_mem=0x%llx)\n", vptr);
        return DRV_ERROR_INVALID_HANDLE;
    }

    if ((vptr >= DEVMM_MAX_DYN_ALLOC_BASE) && (svm_is_dyn_addr(vptr))) {
        return devmm_get_dyn_reserve_addr_attr(p_heap_mgmt, vptr, attr);
    }

    heap = devmm_va_to_heap((virt_addr_t)vptr);
    if ((heap == NULL) && (devmm_va_is_in_svm_range(vptr))) {
        DEVMM_DRV_ERR("Addr isn't allocated. (va=0x%llx)\n", vptr);
        return DRV_ERROR_NOT_EXIST;
    }

    if (devmm_heap_is_host_attribute(heap) == true) {
        return devmm_get_host_memory_attribute(heap, p_heap_mgmt, attr);
    }

    if (heap->heap_sub_type == SUB_SVM_TYPE) {
        return devmm_get_attribute(vptr, attr, heap, p_heap_mgmt);
    } else if (heap->heap_sub_type == SUB_DVPP_TYPE) {
        attr->memType = DV_MEM_LOCK_DEV_DVPP;
    } else if (heap->heap_sub_type == SUB_RESERVE_TYPE) {
        return devmm_get_reserve_addr_attr(p_heap_mgmt, heap, vptr, attr);
    } else {
        attr->memType = DV_MEM_LOCK_DEV;
    }

    attr->devId = devmm_heap_device_by_list_type(heap->heap_list_type);
    attr->pageSize = (heap->heap_type == DEVMM_HEAP_HUGE_PAGE) ?
        p_heap_mgmt->huge_page_size : p_heap_mgmt->svm_page_size;

    return DRV_ERROR_NONE;
}

static DVresult devmm_get_mem_size_info(uint32_t devid, unsigned int type, struct MemInfo *info)
{
    struct devmm_virt_heap_mgmt *p_heap_mgmt = NULL;
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    p_heap_mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (p_heap_mgmt == NULL) {
        DEVMM_DRV_ERR("Heap_mgmt wan't inited, please call device open api frist. (device=%u)\n", devid);
        return DRV_ERROR_INVALID_HANDLE;
    }

    arg.head.devid = devid;
    arg.data.query_device_mem_usedinfo_para.mem_type = type;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_QUERY_MEM_USEDINFO, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Ioctl error. (device=%u; arg_head_devid=%u; type=%x; ret=%d)\n",
            devid, arg.head.devid, type, ret);
        return ret;
    }

    info->phy_info.total = arg.data.query_device_mem_usedinfo_para.normal_total_size;
    info->phy_info.free = arg.data.query_device_mem_usedinfo_para.normal_free_size;
    info->phy_info.huge_total = arg.data.query_device_mem_usedinfo_para.huge_total_size;
    info->phy_info.huge_free = arg.data.query_device_mem_usedinfo_para.huge_free_size;
    info->phy_info.giant_total = arg.data.query_device_mem_usedinfo_para.giant_total_size;
    info->phy_info.giant_free = arg.data.query_device_mem_usedinfo_para.giant_free_size;

    DEVMM_DRV_DEBUG_ARG("Argument. (ram free=%lu; total=%lu; huge free=%lu; huge total=%lu; "
        "giant free=%lu; giant total=%lu)\n", info->phy_info.free, info->phy_info.total, info->phy_info.huge_free,
        info->phy_info.huge_total, info->phy_info.giant_free, info->phy_info.giant_total);

    return DRV_ERROR_NONE;
}

static DVresult devmm_get_mem_check_info_para_check(struct MemAddrInfo *info)
{
    if (info->addr == NULL) {
        DEVMM_DRV_ERR("Info addr para is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((info->cnt == 0) || (info->cnt > DEVMM_DEV_ADDR_NUM_MAX)) {
        DEVMM_DRV_ERR("Cnt out of range. (cnt=%u)\n", info->cnt);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((info->mem_type & MEM_SVM_TYPE) || (info->mem_type & MEM_HOST_TYPE) ||
        (info->mem_type & MEM_HOST_AGENT_TYPE)) {
        DEVMM_RUN_INFO("Mem_type not support. (mem_type=%u)\n", info->mem_type);
        return DRV_ERROR_NOT_SUPPORT;
    }
    return DRV_ERROR_NONE;
}

static DVresult devmm_get_mem_check_info(uint32_t devid, uint32_t type, struct MemInfo *info)
{
    (void)type;
    struct MemAddrInfo *addr_info = &info->addr_info;
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    ret = devmm_get_mem_check_info_para_check(addr_info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    arg.head.devid = devid;
    arg.data.check_meminfo_para.va = (uint64_t *)addr_info->addr;
    arg.data.check_meminfo_para.cnt = addr_info->cnt;
    arg.data.check_meminfo_para.heap_subtype_mask = devmm_memtype_mask_to_heap_subtype_mask(addr_info->mem_type);
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_CHECK_MEMINFO, &arg);
    if (ret != DRV_ERROR_NONE) {
        addr_info->flag = false;
        return ret;
    }

    addr_info->flag = true;
    return DRV_ERROR_NONE;
}

static DVresult (*devmm_mem_get_info[MEM_INFO_TYPE_MAX])
    (uint32_t devid, unsigned int type, struct MemInfo *info) = {
        [MEM_INFO_TYPE_DDR_SIZE] = devmm_get_mem_size_info,
        [MEM_INFO_TYPE_HBM_SIZE] = devmm_get_mem_size_info,
        [MEM_INFO_TYPE_DDR_P2P_SIZE] = devmm_get_mem_size_info,
        [MEM_INFO_TYPE_HBM_P2P_SIZE] = devmm_get_mem_size_info,
        [MEM_INFO_TYPE_ADDR_CHECK] = devmm_get_mem_check_info,
};

static DVresult devmm_mem_get_info_para_check(DVdevice devid, unsigned int type, struct MemInfo *info)
{
    /* host agent not surport get meminfo */
    if (devid >= DEVMM_MAX_PHY_DEVICE_NUM) {
        DEVMM_DRV_ERR("Devid out of range. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (info == NULL) {
        DEVMM_DRV_ERR("Info para is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (type >= MEM_INFO_TYPE_MAX) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (devmm_mem_get_info[type] == NULL) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    return DRV_ERROR_NONE;
}

DVresult halMemGetInfo(DVdevice device, unsigned int type, struct MemInfo *info)
{
    DVresult ret;

    DEVMM_DRV_DEBUG_ARG("Enter. (device=%u; type=0x%x)\n", device, type);

    ret = devmm_mem_get_info_para_check(device, type, info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return devmm_mem_get_info[type](device, type, info);
}

static bool devmm_support_pcie_bar_feature(struct devmm_virt_heap_mgmt *p_heap_mgmt, uint32_t devid)
{
    return p_heap_mgmt->support_bar_mem[devid];
}

static bool devmm_support_dev_mem_register_feature(struct devmm_virt_heap_mgmt *p_heap_mgmt, uint32_t devid)
{
    return p_heap_mgmt->support_dev_mem_map_host[devid];
}

static bool devmm_support_pcie_bar_huge_feature(struct devmm_virt_heap_mgmt *p_heap_mgmt, uint32_t devid)
{
    return p_heap_mgmt->support_bar_huge_mem[devid];
}

static uint64_t devmm_get_double_pgtable_offset(struct devmm_virt_heap_mgmt *p_heap_mgmt, uint32_t devid)
{
    return p_heap_mgmt->double_pgtable_offset[devid];
}

static bool devmm_support_agent_giant_page_feature(struct devmm_virt_heap_mgmt *p_heap_mgmt, uint32_t devid)
{
    return p_heap_mgmt->support_agent_giant_page[devid];
}

static bool devmm_support_shmem_map_exbus_feature(struct devmm_virt_heap_mgmt *p_heap_mgmt, uint32_t devid)
{
    return p_heap_mgmt->support_shmem_map_exbus[devid];
}

typedef bool (*devmm_is_feature_support)(struct devmm_virt_heap_mgmt *p_heap_mgmt, uint32_t devid);

#define SUPPORT_FEATURE_MAX_NUM 6
static void devmm_get_support_feature(uint32_t devid, unsigned long long in_support_feature,
    unsigned long long *out_support_feature)
{
    static const devmm_is_feature_support is_feature_support[SUPPORT_FEATURE_MAX_NUM] = {
        [CTRL_SUPPORT_PCIE_BAR_MEM_BIT] = devmm_support_pcie_bar_feature,
        [CTRL_SUPPORT_DEV_MEM_REGISTER_BIT] = devmm_support_dev_mem_register_feature,
        [CTRL_SUPPORT_PCIE_BAR_HUGE_MEM_BIT] = devmm_support_pcie_bar_huge_feature,
        [CTRL_SUPPORT_GIANT_PAGE_BIT] = devmm_support_agent_giant_page_feature,
        [CTRL_SUPPORT_SHMEM_MAP_EXBUS_BIT] = devmm_support_shmem_map_exbus_feature,
    };
    struct devmm_virt_heap_mgmt *p_heap_mgmt = NULL;
    uint32_t i;

    if (devid >= DEVMM_MAX_PHY_DEVICE_NUM) {
        return;
    }
    p_heap_mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (p_heap_mgmt == NULL) {
        return;
    }

    for (i = CTRL_SUPPORT_PCIE_BAR_MEM_BIT; i < SUPPORT_FEATURE_MAX_NUM; i++) {
        uint64_t feature_mask = (1ULL << i);
        if ((in_support_feature & feature_mask) != 0) {
            if (is_feature_support[i](p_heap_mgmt, devid)) {
                *out_support_feature |= feature_mask;
            }
        }
    }
}

static THREAD bool g_support_numa_ts = false;
static DVresult devmm_ctrl_support_feature(void *param_value, size_t param_value_size,
    void *out_value, size_t *out_size_ret)
{
    (void)param_value_size;
    (void)out_size_ret;
    struct supportFeaturePara *in_para = (struct supportFeaturePara *)param_value;
    struct supportFeaturePara *out_para = (struct supportFeaturePara *)out_value;

    if ((in_para == NULL) || (out_para == NULL)) {
        DEVMM_DRV_ERR("Check in_para out_para is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }
    out_para->support_feature = 0;
    if ((in_para->support_feature & CTRL_SUPPORT_NUMA_TS_MASK) != 0) {
        g_support_numa_ts = 1;
        out_para->support_feature |= CTRL_SUPPORT_NUMA_TS_MASK;
    }

    devmm_get_support_feature(in_para->devid, in_para->support_feature, &out_para->support_feature);

    DEVMM_DRV_INFO("RTS note drv support feature. (in_feature=0x%llx; out_feature=0x%llx)\n",
        in_para->support_feature, out_para->support_feature);
    return DRV_ERROR_NONE;
}

static DVresult devmm_get_double_pgtable_offset_para_check(void *param_value, size_t param_value_size,
    void *out_value, size_t *out_size_ret)
{
    if (param_value == NULL) {
        DEVMM_DRV_ERR("Param_value is NULL. \n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (out_value == NULL) {
        DEVMM_DRV_ERR("Out_value is NULL. \n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (out_size_ret == NULL) {
        DEVMM_DRV_ERR("Out_size_ret is NULL. \n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (param_value_size != sizeof(uint32_t)) {
        DEVMM_DRV_ERR("Param_value_size not equal sizeof(uint32_t). \n");
        return DRV_ERROR_INVALID_VALUE;
    }
    return DRV_ERROR_NONE;
}

static DVresult devmm_ctrl_get_double_pgtable_offset(void *param_value, size_t param_value_size,
    void *out_value, size_t *out_size_ret)
{
    struct devmm_virt_heap_mgmt *p_heap_mgmt = NULL;
    uint32_t devid;
    DVresult ret;

    ret = devmm_get_double_pgtable_offset_para_check(param_value, param_value_size, out_value, out_size_ret);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Parameter is invalid. \n");
        return ret;
    }

    devid = *(uint32_t *)param_value;
    if (devid >= DEVMM_MAX_PHY_DEVICE_NUM) {
        DEVMM_DRV_ERR("Devid is invalid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    p_heap_mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (p_heap_mgmt == NULL) {
        return DRV_ERROR_INNER_ERR;
    }

    *(uint64_t *)out_value = devmm_get_double_pgtable_offset(p_heap_mgmt, devid);
    *out_size_ret = sizeof(uint64_t);

    DEVMM_DRV_INFO("RTS note drv double pgtable offset. (offset=0x%llx)\n", *(uint64_t *)out_value);
    return DRV_ERROR_NONE;
}

static DVresult devmm_mem_repair_para_check(void *param_value, size_t param_value_size,
    void *out_value, size_t *out_size_ret)
{
    struct MemRepairInPara *in_para = NULL;

    if (param_value == NULL) {
        DEVMM_DRV_ERR("Param_value is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (param_value_size != sizeof(struct MemRepairInPara)) {
        DEVMM_DRV_ERR("Param_value_size is invalid.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((out_value != NULL) || (out_size_ret != NULL)) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    in_para = (struct MemRepairInPara *)param_value;
    if ((in_para->devid >= DEVMM_MAX_PHY_DEVICE_NUM) ||
        (in_para->count == 0) || (in_para->count > MEM_REPAIR_MAX_CNT)) {
        DEVMM_DRV_ERR("InPara is invalid. (devid=%u; repair_cnt=%u)\n", in_para->devid, in_para->count);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

static DVresult devmm_ctrl_mem_repair(void *para_value, size_t para_value_size, void *out_value, size_t *out_size_ret)
{
    struct MemRepairInPara *in_para = NULL;
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;
    u32 i;

    ret = devmm_mem_repair_para_check(para_value, para_value_size, out_value, out_size_ret);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    in_para = (struct MemRepairInPara *)para_value;
    arg.head.devid = in_para->devid;
    arg.data.mem_repair_para.count = in_para->count;
    (void)memcpy_s(arg.data.mem_repair_para.repair_addrs, MEM_REPAIR_MAX_CNT *sizeof(struct MemRepairAddr),
        in_para->repairAddrs, in_para->count *sizeof(struct MemRepairAddr));
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_REPLAIR, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Mem repair failed. (devid=%u; repair_cnt=%u)\n", in_para->devid, in_para->count);
        return ret;
    }

    DEVMM_RUN_INFO("Mem repair succ. (devid=%u; repair_cnt=%u)\n", in_para->devid, in_para->count);
    for (i= 0; i < in_para->count; i++) {
        DEVMM_RUN_INFO("Repair info. (addr[%u]=0x%llx; size=0x%llx).\n",
            i, in_para->repairAddrs[i].ptr, in_para->repairAddrs[i].len);
    }
    return DRV_ERROR_NONE;
}

static drvError_t devmm_ctrl_get_addr_module_id(void *param_value, size_t param_value_size,
    void *out_value, size_t *out_size_ret)
{
    if ((param_value == NULL) || (out_value == NULL) || (out_size_ret == NULL)) {
        DEVMM_DRV_ERR("Input is invalid. (param_value=%u; out_value=%u; out_size_ret=%u)\n",
            (param_value != NULL), (out_value != NULL), (out_size_ret != NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    if (param_value_size != sizeof(uint64_t)) {
        DEVMM_DRV_ERR("Param_value_size is invalid. (param_value_size=%lu; size=%lu)\n",
            param_value_size, sizeof(uint64_t));
        return DRV_ERROR_INVALID_VALUE;
    }

    devmm_get_addr_module_id(*(uint64_t *)param_value, (uint32_t *)out_value, (uint64_t *)out_size_ret);
    return DRV_ERROR_NONE;
}

#define DEVMM_SPECIFIED_ALLOC_START_ADDR 0x140000000000ULL
#define DEVMM_SPECIFIED_ALLOC_SIZE 0X40000000000ULL
struct devmm_specified_addr_alloc_range {
    uint64_t start_addr;
    uint64_t size;
};

drvError_t devmm_get_specified_addr_alloc_range(void *param_value, size_t param_value_size,
    void *alloc_range_value, size_t *alloc_range_size)
{
    struct devmm_specified_addr_alloc_range *alloc_range = alloc_range_value;

    if ((param_value != NULL) || (param_value_size != 0)) {
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((alloc_range == NULL) || (alloc_range_size == NULL)) {
        DEVMM_DRV_ERR("Out para is invalid. (alloc_range=%u; alloc_range_size=%u)\n",
            (alloc_range != NULL), (alloc_range_size != NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    alloc_range->start_addr = DEVMM_SPECIFIED_ALLOC_START_ADDR;
    alloc_range->size = DEVMM_SPECIFIED_ALLOC_SIZE;
    *alloc_range_size = sizeof(struct devmm_specified_addr_alloc_range);
    return DRV_ERROR_NONE;
}

static DVresult devmm_check_remote_mmap_feature(uint32_t devid)
{
    struct devmm_virt_heap_mgmt *p_heap_mgmt = NULL;

    if (devid >= DEVMM_MAX_PHY_DEVICE_NUM) {
        DEVMM_DRV_ERR("devid is invalid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    p_heap_mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if ((p_heap_mgmt == NULL) || (p_heap_mgmt->is_dev_inited[devid] == false)) {
        DEVMM_DRV_ERR("devid is uninited. (devid=%u)\n", devid);
        return DRV_ERROR_UNINIT;
    }

    return p_heap_mgmt->support_remote_mmap[devid] ? DRV_ERROR_NONE : DRV_ERROR_NOT_SUPPORT;
}

static DVresult devmm_ctrl_cp_process_mmap(void *para_value, size_t para_value_size, void *out_value, size_t *out_size_ret)
{
    struct ProcessCpMmap *mmap_info = (struct ProcessCpMmap *)para_value;
    DVdeviceptr vptr;
    DVresult ret;

    if ((para_value == NULL) || (out_value == NULL) || (out_size_ret == NULL)) {
        DEVMM_DRV_ERR("Input para is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (para_value_size != sizeof(struct ProcessCpMmap)) {
        DEVMM_DRV_ERR("Para_value_size is invalid. (value_size=%lu)\n", para_value_size);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (mmap_info->flag != 0) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (svm_is_dcache_addr(mmap_info->ptr, mmap_info->size) == false) {
        DEVMM_RUN_INFO("Not support not dcache addr. (va=0x%llx; size=0x%llx)\n", mmap_info->ptr, mmap_info->size);
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = devmm_check_remote_mmap_feature(mmap_info->devid);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    vptr = mmap_info->ptr;
    ret = devmm_process_cp_mmap(mmap_info->devid, &vptr, mmap_info->size);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Cp process mmap failed. (ret=%d)\n", ret);
        return ret;
    }

    *(DVdeviceptr *)out_value = vptr;
    *out_size_ret = sizeof(DVdeviceptr);

    DEVMM_DRV_DEBUG_ARG("Mmap_info_ptr=0x%llx; vptr=0x%llx; size=0x%lx.\n", mmap_info->ptr, vptr, mmap_info->size);

    return DRV_ERROR_NONE;
}

static DVresult devmm_ctrl_cp_process_munmap(void *para_value, size_t para_value_size, void *out_value, size_t *out_size_ret)
{
    struct ProcessCpMunmap *munmap_info = (struct ProcessCpMunmap *)para_value;
    DVresult ret;

    if ((para_value == NULL) || (out_value != NULL) || (out_size_ret != NULL)) {
        DEVMM_DRV_ERR("Input param is invalid.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (para_value_size != sizeof(struct ProcessCpMunmap)) {
        DEVMM_DRV_ERR("Para_value_size is invalid. (value_size=%lu)\n", para_value_size);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (munmap_info->size != 0) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (svm_is_dcache_addr(munmap_info->ptr, 1) == false) {
        DEVMM_RUN_INFO("Not support not dcache addr. (va=0x%llx)\n", munmap_info->ptr);
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = devmm_check_remote_mmap_feature(munmap_info->devid);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return devmm_process_cp_munmap(munmap_info->devid, munmap_info->ptr);
}

static DVresult devmm_get_dcache_addr(void *para_value, size_t para_value_size, void *out_value, size_t *out_size_ret)
{
    if ((para_value != NULL) || (para_value_size != 0) || (out_value == NULL) || (out_size_ret == NULL)) {
        DEVMM_DRV_ERR("Input param is invalid.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    *(DVdeviceptr *)out_value = DEVMM_DCACHE_ADDR_START;
    *out_size_ret = sizeof(DVdeviceptr);

    DEVMM_DRV_DEBUG_ARG("dcache addr=0x%llx.\n", *(DVdeviceptr *)out_value);

    return DRV_ERROR_NONE;
}

static DVresult(*devmm_mem_ctrl_handlers[CTRL_TYPE_MAX])
    (void *param, size_t param_size, void *out_param, size_t *out_size) = {
        [CTRL_TYPE_ADDR_MAP] = devmm_ctrl_map_addr,
        [CTRL_TYPE_ADDR_UNMAP] = devmm_ctrl_unmap_addr,
        [CTRL_TYPE_SUPPORT_FEATURE] = devmm_ctrl_support_feature,
        [CTRL_TYPE_GET_DOUBLE_PGTABLE_OFFSET] = devmm_ctrl_get_double_pgtable_offset,
        [CTRL_TYPE_MEM_REPAIR] = devmm_ctrl_mem_repair,
        [CTRL_TYPE_GET_ADDR_MODULE_ID] = devmm_ctrl_get_addr_module_id,
        [CTRL_TYPE_PROCESS_CP_MMAP] = devmm_ctrl_cp_process_mmap,
        [CTRL_TYPE_PROCESS_CP_MUNMAP] = devmm_ctrl_cp_process_munmap,
        [CTRL_TYPE_GET_DCACHE_ADDR] = devmm_get_dcache_addr,
};

drvError_t halMemCtl(int type, void *param_value, size_t param_value_size, void *out_value, size_t *out_size_ret)
{
    drvError_t ret;

    if ((type < 0) || (type >= (int)CTRL_TYPE_MAX)) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = devmm_mem_ctrl_handlers[type](param_value, param_value_size, out_value, out_size_ret);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NOT_SUPPORT), "Mem_ctrl error. (ret=%d)\n", ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

drvError_t drvMemDeviceOpenInner(uint32_t devid, halDevOpenIn *in, halDevOpenOut *out)
{
    (void)in;
    (void)out;
    return devmm_setup_device(devid);
}

int drvMemDeviceOpen(uint32_t devid, int devfd)
{
    (void)devfd;
    return (int)drvMemDeviceOpenInner(devid, NULL, NULL);
}

static void devmm_heap_close(struct devmm_virt_heap_mgmt *p_heap_mgmt,
    uint32_t page_type, uint32_t heap_list_type, uint32_t heap_sub_type)
{
    struct devmm_heap_list *heap_list = NULL;
    struct devmm_virt_list_head *pos = NULL;
    struct devmm_virt_list_head *n = NULL;
    struct devmm_virt_com_heap *heap = NULL;
    struct devmm_virt_heap_type heap_type;
    uint32_t heap_mem_type;

    heap_type.heap_type = page_type;
    heap_type.heap_list_type = heap_list_type;
    heap_type.heap_sub_type = heap_sub_type;

    for (heap_mem_type = DEVMM_HBM_MEM; heap_mem_type < DEVMM_MEM_TYPE_MAX; heap_mem_type++) {
        heap_type.heap_mem_type = heap_mem_type;
        if (devmm_get_heap_list_by_type(p_heap_mgmt, &heap_type, &heap_list) != DRV_ERROR_NONE) {
            return;
        }

        (void)pthread_rwlock_wrlock(&heap_list->list_lock);
        devmm_virt_list_for_each_safe(pos, n, &heap_list->heap_list)
        {
            heap = devmm_virt_list_entry(pos, struct devmm_virt_com_heap, list);
            devmm_virt_list_del_init(&(heap->list));
            (void)devmm_virt_destroy_heap(p_heap_mgmt, heap, true);
            heap_list->heap_cnt--;
        }
        (void)pthread_rwlock_unlock(&heap_list->list_lock);
    }

    return;
}

static void devmm_close_device(UINT32 devid)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    arg.head.devid = devid;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_CLOSE_DEVICE, &arg);
    share_log_read_run_info(HAL_MODULE_TYPE_DEVMM);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Ioctl close device error. (ret=%d; devid=%u))\n", ret, devid);
    }
    return;
}

static int devmm_prepare_close_device(UINT32 devid)
{
    struct devmm_ioctl_arg arg = {0};

    arg.head.devid = devid;
    return (int)devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_PREPARE_CLOSE_DEVICE, &arg);
}

drvError_t drvMemDeviceCloseInner(uint32_t devid, halDevCloseIn *in)
{
    (void)in;
    struct devmm_virt_heap_mgmt *p_heap_mgmt = NULL;
    uint32_t list_type;
    int ret;

    DEVMM_RUN_INFO("DrvMemDeviceClose. (devid=%u)\n", devid);
    p_heap_mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (p_heap_mgmt == NULL) {
        DEVMM_DRV_ERR("Heap_mgmt wasn't inited, please call device open api frist. (devid=%u)\n", devid);
        return (int)DRV_ERROR_INVALID_HANDLE;
    }

    if (devid >= SVM_MAX_AGENT_NUM) {
        DEVMM_DRV_ERR("Devid is invalid. (devid=%u)\n", devid);
        return (int)DRV_ERROR_INVALID_DEVICE;
    }
    svm_mem_stats_show_all_svm_mem(devid);

    if (p_heap_mgmt->support_host_mem_pool) {
        ret = devmm_host_mem_pool_uninit(devid);
        if (ret != DRV_ERROR_NONE) {
            DEVMM_DRV_ERR("Devmm_host_mem_pool_uninit error. (ret=%d; devid=%u))\n", ret, devid);
            return ret;
        }
    }

    ret = devmm_prepare_close_device(devid);
    if (ret != 0) {
        p_heap_mgmt->can_init_dev[devid] = false;
        DEVMM_DRV_ERR("Ioctl prepare close device error. (ret=%d; devid=%u))\n", ret, devid);
        return ret;
    }

    p_heap_mgmt->is_dev_inited[devid] = false;
    p_heap_mgmt->can_init_dev[devid] = true;
    list_type = devmm_heap_list_type_by_device(devid);
    devmm_heap_close(p_heap_mgmt, DEVMM_HEAP_HUGE_PAGE, list_type, SUB_DEVICE_TYPE);
    devmm_heap_close(p_heap_mgmt, DEVMM_HEAP_HUGE_PAGE, list_type, SUB_DVPP_TYPE);
    devmm_heap_close(p_heap_mgmt, DEVMM_HEAP_HUGE_PAGE, list_type, SUB_READ_ONLY_TYPE);
    devmm_heap_close(p_heap_mgmt, DEVMM_HEAP_HUGE_PAGE, list_type, SUB_DEV_READ_ONLY_TYPE);
    devmm_heap_close(p_heap_mgmt, DEVMM_HEAP_CHUNK_PAGE, list_type, SUB_DEVICE_TYPE);

    devmm_record_recycle(devid);
    devmm_del_mem_advise_record(true, 0, 0, devid);
    devmm_reset_dev_reserve_addr(devid);
    devmm_reset_event_devpid(devid);
    devmm_close_device(devid);
    svm_da_del_dev(devid);

    return (int)DRV_ERROR_NONE;
}

int drvMemDeviceClose(uint32_t devid)
{
    return (int)drvMemDeviceCloseInner(devid, NULL);
}

static bool devmm_agent_open_close_flag_is_valid(uint32_t flag)
{
    return (flag <= SVM_AGENT_DEVICE);
}

drvError_t halMemAgentOpen(uint32_t devid, uint32_t flag)
{
    if (devmm_agent_open_close_flag_is_valid(flag) == false) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    return (drvError_t)drvMemDeviceOpen(devid, 0);
}

drvError_t halMemAgentClose(uint32_t devid, uint32_t flag)
{
    if (devmm_agent_open_close_flag_is_valid(flag) == false) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    return (drvError_t)drvMemDeviceClose(devid);
}

static void devmm_restore_func_register(void);
drvError_t drvMemDeviceCloseUserRes(uint32_t devid, halDevCloseIn *in)
{
    (void)in;
    DEVMM_RUN_INFO("CloseUserRes start\n");
    if (devmm_is_split_mode()) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    devmm_virt_uninit_heap_mgmt();
    devmm_reset_event_devpid(devid);
    g_devmm_mem_dev = -1;
    svm_uninit_mem_stats_mng(devid);
    devmm_svm_unmap();
    devmm_restore_func_register();
    g_pro_snapshot_state = true;
    DEVMM_RUN_INFO("CloseUserRes end\n");

    return DRV_ERROR_NONE;
}

drvError_t drvMemProcResBackup(halProcResBackupInfo *info)
{
    (void)info;
    drvError_t ret;

    DEVMM_RUN_INFO("Backup start\n");
    if (devmm_is_split_mode()) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = devmm_virt_backup_heap_mgmt();
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }
    DEVMM_RUN_INFO("Backup end\n");

    return DRV_ERROR_NONE;
}

drvError_t drvMemProcResRestore(halProcResRestoreInfo *info)
{
    (void)info;
    DVresult ret;

    DEVMM_RUN_INFO("Restore start\n");
    if (devmm_is_split_mode()) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = devmm_virt_restore_heap_mgmt();
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("devmm_virt_restore_heap_mgmt fail. (ret=%d)\n", ret);
        return ret;
    }
    devmm_restore_host_mem_pool();

    ret = devmm_record_restore();
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("record_restore fail. (ret=%d)\n", ret);
        return ret;
    }

    ret = devmm_mem_advise_restore();
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Mem advise restore fail. (ret=%d)\n", ret);
        return ret;
    }

    ret = devmm_ctrl_map_mem_restore();
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Ctrl_map_mem restore fail. (ret=%d)\n", ret);
        return ret;
    }
    g_pro_snapshot_state = false;
    DEVMM_RUN_INFO("Restore end\n");

    return DRV_ERROR_NONE;
}

/* Sunset interface: external statement has been removed from ascend_hal.h. */
DVresult drvMbindHbm(DVdeviceptr devPtr, size_t len, uint32_t type, uint32_t dev_id)
{
    (void)devPtr;
    (void)len;
    (void)type;
    (void)dev_id;
    return DRV_ERROR_NONE;
}

DVresult halSdmaCopyInner(DVdeviceptr dst, size_t dst_size, DVdeviceptr src, size_t len)
{
    (void)dst;
    (void)dst_size;
    (void)src;
    (void)len;
    return DRV_ERROR_NOT_SUPPORT;
}

DVresult halSdmaCopy(DVdeviceptr dst, size_t dst_size, DVdeviceptr src, size_t len)
{
    return halSdmaCopyInner(dst, dst_size, src, len);
}

static drvError_t devmm_remote_map_input_check(uint32_t map_type, uint32_t proc_type,
    uint32_t devid, void **dst_ptr)
{
    if (devid >= SVM_MAX_AGENT_NUM) {
        DEVMM_DRV_ERR("Devid is invalid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    if ((map_type >= HOST_REGISTER_MAX_TPYE) || (proc_type >= DEVDRV_PROCESS_CPTYPE_MAX)) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (dst_ptr == NULL) {
        DEVMM_DRV_ERR("Dst_ptr is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

#define SVM_MAP_TYPE_MASK ((1U << MEM_PROC_TYPE_BIT) - 1)
static inline uint32_t devmm_get_register_map_type(uint32_t flag)
{
    return (flag & SVM_MAP_TYPE_MASK);
}

static inline uint32_t devmm_get_register_proc_type(uint32_t flag)
{
    return (flag >> MEM_PROC_TYPE_BIT);
}

static drvError_t devmm_alloc_svm_mem(uint64_t size, uint32_t devid, void **dst_ptr)
{
    DVmem_advise advise = 0;

    devmm_set_module_id_to_advise(HCCL, &advise);
    return devmm_alloc_proc(devid, SUB_SVM_TYPE, advise, size, (DVdeviceptr *)dst_ptr);
}

static void devmm_free_svm_mem(void *dst_ptr)
{
    (void)devmm_free_managed((DVdeviceptr)dst_ptr);
}

static drvError_t _devmm_mem_remote_map(void *src_va, uint64_t size, uint32_t flag, uint32_t devid, void **dst_va)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    arg.head.devid = devid;
    arg.data.map_para.src_va = (UINT64)(uintptr_t)src_va;
    arg.data.map_para.size = size;
    arg.data.map_para.map_type = devmm_get_register_map_type(flag);
    arg.data.map_para.proc_type = devmm_get_register_proc_type(flag);
    arg.data.map_para.dst_va = *(uint64_t *)dst_va;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_REMOTE_MAP, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NOT_SUPPORT), "Mem_remote_map ioctl error. (src_va=0x%llx; size=%llu; devid=%u; ret=%d)\n",
            src_va, size, devid, ret);
        return ret;
    }

    *dst_va = (void *)(uintptr_t)arg.data.map_para.dst_va;
    DEVMM_DRV_DEBUG_ARG("Mem_remote_map. (devid=%u; src_va=0x%llx; size=%llu; dst_va=%p; flag=%u)\n",
        devid, (UINT64)(uintptr_t)src_va, size, *dst_va, flag);
    return DRV_ERROR_NONE;
}

static drvError_t devmm_mem_remote_map(void *src_va, uint64_t size, uint32_t flag, uint32_t devid, void **dst_va)
{
    void *tmp_dst_va = NULL;
    drvError_t ret;

    ret = _devmm_mem_remote_map(src_va, size, flag, devid, &tmp_dst_va);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }
    *dst_va = tmp_dst_va;
    return DRV_ERROR_NONE;
}

static drvError_t devmm_dev_mem_remote_map(void *src_va, uint64_t size, uint32_t flag,
    uint32_t devid, void **dst_va)
{
    void *tmp_dst_va = NULL;
    drvError_t ret;

    ret = devmm_alloc_svm_mem(size, devid, &tmp_dst_va);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Alloc memory failed. (ret=%d; size=%llu; flag=%u; devid=%u)\n",
            ret, size, flag, devid);
        return ret;
    }

    ret = _devmm_mem_remote_map(src_va, size, flag, devid, &tmp_dst_va);
    if (ret != DRV_ERROR_NONE) {
        devmm_free_svm_mem(tmp_dst_va);
        DEVMM_DRV_ERR("Mem remote map fail. (devid=%u; src_va=0x%llx; size=%llu; dst_va=0x%llx; flag=%u; ret=%d)\n",
            devid, (UINT64)(uintptr_t)src_va, size, (UINT64)(uintptr_t)tmp_dst_va, flag, ret);
        return ret;
    }
    *dst_va = tmp_dst_va;
    return DRV_ERROR_NONE;
}

drvError_t devmm_register_mem_to_dma(void *src_va, uint64_t size, uint32_t devid)
{
    struct devmm_ioctl_arg arg = {0};
    drvError_t ret;

    if ((src_va == NULL) || (size == 0) || (size > DEVMM_MAX_MAPPED_RANGE)) {
        DEVMM_DRV_ERR("Src_va is zero or size is invalid. (src_va=0x%llx; size=%llu)\n", (uint64_t)(uintptr_t)src_va, size);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (devid >= SVM_MAX_AGENT_NUM) {
        DEVMM_DRV_ERR("Devid is invalid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

#ifndef EMU_ST
    if ((devmm_va_is_in_svm_range((uint64_t)(uintptr_t)src_va) == false) && devmm_va_is_in_svm_range((uint64_t)(uintptr_t)src_va + size)) {
        DEVMM_DRV_ERR("Start_va and end_va have cross between svm and non svm. (src_va=0x%llx; size=%llu)\n", src_va, size);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (devmm_get_host_mem_alloc_mode() == SVM_HOST_MEM_ALLOCED_BY_MALLOC) {
        return DRV_ERROR_NOT_SUPPORT;
    }
#endif

    arg.head.devid = devid;
    arg.data.register_dma_para.vaddr = (uint64_t)(uintptr_t)src_va;
    arg.data.register_dma_para.size = size;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_REGISTER_DMA, &arg);
    if (ret != DRV_ERROR_NONE) {
        if (ret != DRV_ERROR_NOT_SUPPORT) {
            DEVMM_DRV_ERR("Register mem to dma. ioctl error. (src_va=0x%llx; size=%llu; devid=%u; ret=%d)\n",
                src_va, size, devid, ret);
        }
        return ret;
    }

    DEVMM_DRV_DEBUG_ARG("Register mem to dma. (devid=%u; src_va=0x%llx; size=%llu)\n",
        devid, (uint64_t)(uintptr_t)src_va, size);
    return DRV_ERROR_NONE;
}

typedef drvError_t (*devmm_remote_map_policy)(void *src_va, uint64_t size, uint32_t flag,
    uint32_t devid, void **dst_va);

static drvError_t devmm_remote_map(void *src_ptr, UINT64 size, UINT32 flag, UINT32 devid, void **dst_ptr)
{
    static const devmm_remote_map_policy remote_map_policy[HOST_REGISTER_MAX_TPYE] = {
        [HOST_MEM_MAP_DEV] = devmm_mem_remote_map,
        [HOST_SVM_MAP_DEV] = devmm_mem_remote_map,
        [DEV_SVM_MAP_HOST] = devmm_mem_remote_map,
        [HOST_MEM_MAP_DEV_PCIE_TH] = devmm_mem_remote_map,
        [DEV_MEM_MAP_HOST] = devmm_dev_mem_remote_map,
    };
    uint32_t map_type = devmm_get_register_map_type(flag);
    uint32_t proc_type = devmm_get_register_proc_type(flag);
    uint32_t device_id = devid;
    DVresult ret;

    /*
     * AGENT_SVM_MAP_MASTER doesn't need the user to pass in the devid,
     * but we should ensure it vaild because ioctl_dispatch will verify it.
     */
    if (devmm_va_is_in_svm_range((UINT64)(uintptr_t)src_ptr) && (map_type == DEV_SVM_MAP_HOST)) {
        device_id = 0;
    }

    ret = devmm_remote_map_input_check(map_type, proc_type, device_id, dst_ptr);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return remote_map_policy[map_type](src_ptr, size, flag, device_id, dst_ptr);
}

drvError_t halHostRegister(void *src_ptr, UINT64 size, UINT32 flag, UINT32 devid, void **dst_ptr)
{
    uint32_t map_type = devmm_get_register_map_type(flag);
    if (map_type >= HOST_REGISTER_MAX_TPYE) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (map_type < HOST_MEM_MAP_DMA) {
        return devmm_remote_map(src_ptr, size, flag, devid, dst_ptr);
    } else {
        return devmm_register_mem_to_dma(src_ptr, size, devid);
    }
}

static drvError_t devmm_remote_unmap_input_check(uint32_t map_type, uint32_t proc_type, uint32_t devid)
{
    if (devid >= SVM_MAX_AGENT_NUM) {
        DEVMM_DRV_ERR("Devid is invalid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    if ((map_type >= HOST_REGISTER_MAX_TPYE) || (proc_type >= DEVDRV_PROCESS_CPTYPE_MAX)) {
        return DRV_ERROR_NOT_SUPPORT;
    }
    return DRV_ERROR_NONE;
}

static drvError_t devmm_mem_remote_unmap(void *src_va, uint32_t flag, uint32_t devid, uint64_t *dst_va)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    arg.head.devid = devid;
    arg.data.unmap_para.src_va = (UINT64)(uintptr_t)src_va;
    arg.data.unmap_para.map_type = devmm_get_register_map_type(flag);
    arg.data.unmap_para.proc_type = devmm_get_register_proc_type(flag);

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_REMOTE_UNMAP, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Mem_remote_unmap error. (src_va=0x%llx; flag=%u; devid=%u; ret=%d)\n",
            (UINT64)(uintptr_t)src_va, flag, devid, ret);
        return ret;
    }

    *dst_va = arg.data.unmap_para.dst_va;
    DEVMM_DRV_DEBUG_ARG("Mem_remote_unmap. (devid=%u; src_va=0x%llx; dst_va=0x%llx; flag=%u)\n",
        devid, (UINT64)(uintptr_t)src_va, *dst_va, flag);
    return DRV_ERROR_NONE;
}

static drvError_t devmm_dev_mem_remote_unmap(void *src_va, uint32_t flag, uint32_t devid,
    uint64_t *dst_va)
{
    drvError_t ret;

    ret = devmm_mem_remote_unmap(src_va, flag, devid, dst_va);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    devmm_free_svm_mem((void *)(uintptr_t)(*dst_va));
    return DRV_ERROR_NONE;
}

drvError_t devmm_unregister_mem_to_dma(void *src_va, uint32_t devid)
{
    struct devmm_ioctl_arg arg = {0};
    drvError_t ret;

    if (devid >= SVM_MAX_AGENT_NUM) {
        DEVMM_DRV_ERR("Devid is invalid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    arg.head.devid = devid;
    arg.data.unregister_dma_para.vaddr = (uint64_t)(uintptr_t)src_va;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_UNREGISTER_DMA, &arg);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    DEVMM_DRV_DEBUG_ARG("Unregister mem to dma. (devid=%u; src_va=0x%llx)\n",
        devid, (uint64_t)(uintptr_t)src_va);
    return DRV_ERROR_NONE;
}

typedef drvError_t (*devmm_remote_unmap_policy)(void *src_va, uint32_t devid, uint32_t flag, uint64_t *dst_va);

static drvError_t devmm_remote_unmap(void *src_ptr, UINT32 devid, UINT32 flag)
{
    static const devmm_remote_unmap_policy remote_unmap_policy[HOST_REGISTER_MAX_TPYE] = {
        [HOST_MEM_MAP_DEV] = devmm_mem_remote_unmap,
        [HOST_SVM_MAP_DEV] = devmm_mem_remote_unmap,
        [DEV_SVM_MAP_HOST] = devmm_mem_remote_unmap,
        [HOST_MEM_MAP_DEV_PCIE_TH] = devmm_mem_remote_unmap,
        [DEV_MEM_MAP_HOST] = devmm_dev_mem_remote_unmap,
    };
    uint32_t map_type = devmm_get_register_map_type(flag);
    uint32_t proc_type = devmm_get_register_proc_type(flag);
    uint32_t tmp_devid = devid;
    uint64_t dst_va;
    DVresult ret;

    /*
     * MASTER_SVM_MAP_AGENT and AGENT_SVM_MAP_MASTER doesn't need the user to pass in the devid,
     * but we should ensure it vaild because ioctl_dispatch will verify it.
     */
    if (devmm_va_is_in_svm_range((UINT64)(uintptr_t)src_ptr) &&
        (map_type != HOST_MEM_MAP_DEV_PCIE_TH)) {
        tmp_devid = 0;
    }

    ret = devmm_remote_unmap_input_check(map_type, proc_type, tmp_devid);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return remote_unmap_policy[map_type](src_ptr, flag, tmp_devid, &dst_va);
}

drvError_t halHostUnregisterEx(void *src_ptr, UINT32 devid, UINT32 flag)
{
    uint32_t map_type = devmm_get_register_map_type(flag);
    if (map_type >= HOST_REGISTER_MAX_TPYE) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (map_type < HOST_MEM_MAP_DMA) {
        return devmm_remote_unmap(src_ptr, devid, flag);
    } else {
        return devmm_unregister_mem_to_dma(src_ptr, devid);
    }
}

drvError_t halHostUnregister(void *src_ptr, UINT32 devid)
{
    /* flag = HOST_MEM_MAP_DEV is tmp flag, real flag will get in kernel */
    return halHostUnregisterEx(src_ptr, devid, HOST_MEM_MAP_DEV);
}

static inline bool devmm_is_advise_ts(uint64_t flag)
{
    uint32_t advise_4g = (flag >> MEM_ADVISE_4G_BIT) & 1;
    uint32_t advise_ts = (flag >> MEM_ADVISE_TS_BIT) & 1;
    return ((advise_4g != 0) || (advise_ts != 0));
}

static inline bool devmm_mem_is_readonly(uint64_t flag)
{
    return (((flag >> MEM_READONLY_BIT) & 1) == 1) ? true : false;
}

static inline bool devmm_mem_is_dev_readonly(uint64_t flag)
{
    return (((flag >> MEM_HOST_RW_DEV_RO_BIT) & 1) == 1) ? true : false;
}

static inline bool devmm_mem_is_giant_page(uint64_t flag)
{
    return (((flag >> MEM_PAGE_GIANT_BIT) & 1) == 1) ? true : false;
}

static DVresult devmm_host_set_advise(uint32_t devid, uint32_t virt_mem_type, uint64_t flag,
    DVmem_advise *advise)
{
    (void)devid;
    if (devmm_mem_is_readonly(flag) || devmm_mem_is_dev_readonly(flag)) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (devmm_mem_is_giant_page(flag)) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (virt_mem_type == MEM_HOST_AGENT_VAL) {
        if (devmm_is_advise_ts(flag) == true) {
            return DRV_ERROR_NOT_SUPPORT;
        }
        /* host_agent default alloc normal_page ddr */
        *advise |= DV_ADVISE_DDR;
    }

    if (virt_mem_type == MEM_HOST_VAL) {
        /* host default alloc normal_page ddr */
        *advise |= DV_ADVISE_HOST | DV_ADVISE_DDR;
    }
    return DRV_ERROR_NONE;
}

static inline void devmm_phy_page_type_set_advise(uint32_t phy_page_type,
    DVmem_advise *advise)
{
    *advise |= (phy_page_type != 0) ? DV_ADVISE_HUGEPAGE : 0;
}

static inline void devmm_hbm_ddr_p2p_set_advise(uint64_t flag, DVmem_advise *advise)
{
    uint32_t advise_p2p = (((flag >> MEM_ADVISE_P2P_BIT) & 1) != 0) || (((flag >> MEM_ADVISE_BAR_BIT) & 1) != 0);
    uint32_t hbm_mem_type = (flag >> MEM_PHY_BIT) & 1;

    if (hbm_mem_type != 0) {
        *advise |= (advise_p2p != 0) ? DV_ADVISE_P2P_HBM : 0;
        *advise |= DV_ADVISE_HBM;
    } else {
        *advise |= (advise_p2p != 0) ? DV_ADVISE_P2P_DDR : 0;
        *advise |= DV_ADVISE_DDR;
    }
}

static inline bool devmm_support_dev_readonly(void)
{
    struct devmm_virt_heap_mgmt *p_heap_mgmt = NULL;

    p_heap_mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if ((p_heap_mgmt != NULL) && (p_heap_mgmt->support_host_rw_dev_ro == true)) {
        return true;
    }
    return false;
}

static inline drvError_t devmm_readonly_mem_set_advise(uint64_t flag, uint32_t virt_mem_type, DVmem_advise *advise)
{
    if ((devmm_mem_is_readonly(flag) == true) || (devmm_mem_is_dev_readonly(flag) == true)) {
        if ((virt_mem_type == MEM_DEV_VAL) ||
            ((virt_mem_type == MEM_DVPP_VAL) && (devmm_mem_is_dev_readonly(flag) == false)))  {
            *advise |= DV_ADVISE_READONLY;
        } else {
            return DRV_ERROR_NOT_SUPPORT;
        }

        if ((devmm_mem_is_dev_readonly(flag)) && (devmm_support_dev_readonly() == false)) {
            *advise &= ~((uint32_t)DV_ADVISE_READONLY);
        }
    }
    return DRV_ERROR_NONE;
}

static inline drvError_t devmm_continuty_mem_set_advise(uint64_t flag, uint64_t size, uint32_t virt_mem_type,
    uint32_t phy_page_type, DVmem_advise *advise)
{
    /* dvpp mem ignores 4G and continuty flag */
    if (virt_mem_type != MEM_DVPP_VAL) {
        uint32_t advise_continuty = (flag >> MEM_CONTINUTY_BIT) & 1;
        if (advise_continuty != 0) {
            if ((size > MAX_CONTINUTY_PHYS_SIZE) || (phy_page_type != 0)) {
                return DRV_ERROR_INVALID_VALUE;
            }
            *advise |= DV_ADVISE_CONTINUTY;
        }
    }
    return DRV_ERROR_NONE;
}

static inline uint32_t devmm_get_devid_from_flag(uint64_t flag)
{
    uint32_t virt_mem_type = (flag >> MEM_VIRT_BIT) & MEM_VIRT_MASK;

    if (virt_mem_type == MEM_HOST_AGENT_VAL) {
        return SVM_HOST_AGENT_ID;
    }
    return flag & MEM_DEVID_MASK;
}

static inline uint32_t devmm_get_virt_mem_type_from_flag(uint64_t flag)
{
    return (flag >> MEM_VIRT_BIT) & MEM_VIRT_MASK;
}

static inline uint32_t devmm_get_page_type_from_flag(uint64_t flag)
{
    return (flag >> MEM_PAGE_BIT) & 1;
}

static inline drvError_t devmm_ts_mem_set_advise(uint64_t flag, uint32_t virt_mem_type, DVmem_advise *advise)
{
    (void)virt_mem_type;
    uint32_t advise_p2p = (((flag >> MEM_ADVISE_P2P_BIT) & 1) != 0) || (((flag >> MEM_ADVISE_BAR_BIT) & 1) != 0);
    uint32_t hbm_mem_type = (flag >> MEM_PHY_BIT) & 1;
    bool no_support_numa_ts = (g_support_numa_ts == false) && (hbm_mem_type == 0) && (advise_p2p == 0);

    if (devmm_is_advise_ts(flag) == true) {
        *advise |= DV_ADVISE_TS_DDR;
    } else if (no_support_numa_ts != 0) {
        /* adapt rts old version ddr use hbm to replace */
        *advise &= ~((uint32_t)DV_ADVISE_DDR);
        *advise |= DV_ADVISE_HBM;
    }
    return DRV_ERROR_NONE;
}

static inline void devmm_get_mem_type(uint64_t flag,
    uint32_t *virt_mem_type, uint32_t *phy_page_type)
{
    *virt_mem_type = devmm_get_virt_mem_type_from_flag(flag);
    *phy_page_type = devmm_get_page_type_from_flag(flag);
}

static bool devmm_is_host_advise(uint32_t devid, uint32_t virt_mem_type)
{
    (void)devid;
    if ((virt_mem_type == MEM_HOST_VAL) || (virt_mem_type == MEM_HOST_AGENT_VAL)) {
        return true;
    }
    return false;
}

static uint32_t devmm_get_module_id_by_flag(uint64_t flag)
{
    uint32_t model_id = (uint32_t)((flag >> MEM_MODULE_ID_BIT) & MEM_MODULE_ID_MASK);

    /* RUNTIME will fill invalid model_id in some scenarios */
    model_id = (model_id >= SVM_MAX_MODULE_ID) ? UNKNOWN_MODULE_ID : model_id;
    return model_id;
}

static bool devmm_is_support_agent_giant_page_feature(uint32_t devid)
{
    struct devmm_virt_heap_mgmt *mgmt = NULL;

    mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (mgmt == NULL) {
        DEVMM_DRV_ERR("Init heap mgmt failed. (devid=%u)\n", devid);
        return false;
    }

    if (devid >= DEVMM_MAX_PHY_DEVICE_NUM) {
        DEVMM_DRV_ERR("Invalid devid. (devid=%u)\n", devid);
        return false;
    }
    return mgmt->support_agent_giant_page[devid];
}

static bool devmm_is_support_host_giant_page_feature(void)
{
    struct devmm_virt_heap_mgmt *mgmt = NULL;

    mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (mgmt == NULL) {
        DEVMM_DRV_ERR("Init heap mgmt failed.\n");
        return false;
    }


#ifndef EMU_ST
    return mgmt->support_host_giant_page;
#else
    return true;
#endif
}

static drvError_t devmm_giant_page_set_advise(uint64_t flag, uint32_t virt_mem_type, DVmem_advise *advise)
{
    if (devmm_mem_is_giant_page(flag)) {
        uint32_t devid = devmm_get_devid_from_flag(flag);

        if (virt_mem_type != MEM_DEV_VAL) {
            DEVMM_RUN_INFO("Giant page only support dev mem. (flag=0x%llx; virt_mem_type=%u)\n", flag, virt_mem_type);
            return DRV_ERROR_NOT_SUPPORT;
        }
        if (devmm_is_support_agent_giant_page_feature(devid) == false) {
            DEVMM_RUN_INFO("Not support giant page feature. (devid=%u)\n", devid);
            return DRV_ERROR_NOT_SUPPORT;
        }
        if (devmm_is_advise_ts(flag)) {
            DEVMM_RUN_INFO("Giant page not support 4g and ts. (flag=0x%llx)\n", flag);
            return DRV_ERROR_NOT_SUPPORT;
        }
        if (((flag >> MEM_CONTINUTY_BIT) & 1) != 0) {
            DEVMM_RUN_INFO("Giant page not support continuous phy. (flag=0x%llx)\n", flag);
            return DRV_ERROR_NOT_SUPPORT;
        }
        if (devmm_mem_is_readonly(flag) || devmm_mem_is_dev_readonly(flag)) {
            DEVMM_RUN_INFO("Giant page not support readonly. (flag=0x%llx)\n", flag);
            return DRV_ERROR_NOT_SUPPORT;
        }
        if (((flag >> MEM_PAGE_BIT) & 1) != 0) {
#ifndef EMU_ST
            DEVMM_DRV_ERR("Alloc both giant page and huge page is invalid. (flag=0x%llx)\n", flag);
            return DRV_ERROR_INVALID_VALUE;
#endif
        }
        *advise |= DV_ADVISE_GIANTPAGE;
        *advise |= DV_ADVISE_HUGEPAGE;
    }
    return DRV_ERROR_NONE;
}

static drvError_t devmm_dev_set_advise(uint64_t flag, uint64_t size, uint32_t virt_mem_type,
    uint32_t phy_page_type, DVmem_advise *advise)
{
    if (devmm_readonly_mem_set_advise(flag, virt_mem_type, advise) != DRV_ERROR_NONE) {
        DEVMM_RUN_INFO("Not support mem_readonly flag. (alloc_size=%llu; flag=0x%llx)\n", size, flag);
        return DRV_ERROR_NOT_SUPPORT;
    }
    /* dvpp mem ignores huge/p2p/TS/continuty flag */
    if (virt_mem_type != MEM_DVPP_VAL) {
        devmm_phy_page_type_set_advise(phy_page_type, advise);
        devmm_hbm_ddr_p2p_set_advise(flag, advise);

        if (devmm_continuty_mem_set_advise(flag, size, virt_mem_type, phy_page_type, advise) != DRV_ERROR_NONE) {
            DEVMM_DRV_ERR("Invalid continuty size or huge page. (alloc_size=%llu; flag=0x%llx)\n",
                size, flag);
            return DRV_ERROR_INVALID_VALUE;
        }
        if (devmm_ts_mem_set_advise(flag, virt_mem_type, advise) != DRV_ERROR_NONE) {
            return DRV_ERROR_NOT_SUPPORT;
        }
    }

    return devmm_giant_page_set_advise(flag, virt_mem_type, advise);
}

static DVresult devmm_set_advise(uint32_t devid, uint64_t flag, uint64_t size,
    DVmem_advise *advise)
{
    uint32_t virt_mem_type, phy_page_type;
    DVresult ret;

    devmm_get_mem_type(flag, &virt_mem_type, &phy_page_type);

    if (devmm_is_host_advise(devid, virt_mem_type) != true) {
        ret = devmm_dev_set_advise(flag, size, virt_mem_type, phy_page_type, advise);
    } else {
        ret = devmm_host_set_advise(devid, virt_mem_type, flag, advise);
    }

    devmm_set_module_id_to_advise(devmm_get_module_id_by_flag(flag), advise);
    DEVMM_DRV_DEBUG_ARG("Flag. (flag=0x%llx; size=0x%llx; ad=0x%x; devid=%u; virt=%u; huge=%u; numa_ts=%u)\n",
        flag, size, *advise, devid, virt_mem_type, phy_page_type, g_support_numa_ts);
    return ret;
}

static bool devmm_check_support_dev_read_only(uint32_t devid)
{
    struct devmm_virt_heap_mgmt *mgmt = NULL;

    if (devid >= DEVMM_MAX_PHY_DEVICE_NUM) {
        return false;
    }
    mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if ((mgmt == NULL) || (mgmt->support_dev_read_only[devid] == 0)) {
        return false;
    }
    return true;
}

static drvError_t devmm_get_sub_mem_type(uint32_t devid, uint64_t flag,
    unsigned long long size, uint32_t *sub_mem_type)
{
    (void)size;
    uint32_t virt_mem_type = (flag >> MEM_VIRT_BIT) & MEM_VIRT_MASK;

    if (virt_mem_type == MEM_DVPP_VAL) {
        if (devmm_is_host_agent(devid) == true) {
            return DRV_ERROR_NOT_SUPPORT;
        }
        *sub_mem_type = SUB_DVPP_TYPE;
    } else if ((virt_mem_type == MEM_DEV_VAL) || (virt_mem_type == MEM_HOST_AGENT_VAL)) {
        bool mem_readonly = false, mem_dev_readonly = false;

        *sub_mem_type = SUB_DEVICE_TYPE;
        mem_readonly = devmm_mem_is_readonly(flag);
        if (mem_readonly == true) {
            if (devmm_check_support_dev_read_only(devid) == false) {
                return DRV_ERROR_NOT_SUPPORT;
            }
            *sub_mem_type = SUB_READ_ONLY_TYPE;
        }

        mem_dev_readonly = devmm_mem_is_dev_readonly(flag);
        if ((mem_dev_readonly == true) && devmm_support_dev_readonly()) {
            *sub_mem_type = SUB_DEV_READ_ONLY_TYPE;
        }

        if ((mem_readonly == true) && (mem_dev_readonly == true)) {
            DEVMM_DRV_ERR("Invalid flag, readonly and dev_readonly only support one type. \n");
            return DRV_ERROR_PARA_ERROR;
        }
    } else if (virt_mem_type == MEM_HOST_VAL) {
        *sub_mem_type = SUB_HOST_TYPE;
    } else if (virt_mem_type == MEM_SVM_VAL) {
        if (devmm_get_host_mem_alloc_mode() == SVM_HOST_MEM_ALLOCED_BY_MALLOC) {
            DEVMM_DRV_ERR("Not support svm mem, because asan is opened or os not support mmap 8T.\n");
            return DRV_ERROR_NOT_SUPPORT;
        }

        *sub_mem_type = SUB_SVM_TYPE;
    } else {
        return DRV_ERROR_NOT_SUPPORT;
    }

    return DRV_ERROR_NONE;
}

drvError_t halMemAllocInner(void **pp, unsigned long long size, unsigned long long flag)
{
    DVmem_advise advise = 0;
    uint32_t sub_mem_type, devid;
    DVresult ret;

    if (pp == NULL) {
        DEVMM_DRV_ERR("pp is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    devid = devmm_get_devid_from_flag(flag);
    ret = devmm_get_sub_mem_type(devid, flag, size, &sub_mem_type);
    if (ret != DRV_ERROR_NONE) {
        goto show_mem_stats;
    }

    ret = devmm_set_advise(devid, flag, size, &advise);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NOT_SUPPORT), "Invalid flag. (flag=0x%llx; ret=%d)\n", flag, ret);
        goto show_mem_stats;
    }

    *pp = NULL;
    ret = devmm_alloc_proc(devid, sub_mem_type, advise, size, (DVdeviceptr *)pp);
    if (ret != DRV_ERROR_NONE) {
        goto show_mem_stats;
    }

    DEVMM_DRV_DEBUG_ARG("Alloc. (ptr=%llx; size=%llu; virt_type=%u; flag=0x%llx)\n",
        *(DVdeviceptr *)pp, size, sub_mem_type, flag);

    return 0;

show_mem_stats:
    if (sub_mem_type == SUB_HOST_TYPE) {
        svm_mem_stats_show_host();
    } else {
        svm_mem_stats_show_device(devid);
    }
    return ret;
}

drvError_t halMemAlloc(void **pp, unsigned long long size, unsigned long long flag)
{
    return halMemAllocInner(pp, size, flag);
}

drvError_t halMemFreeInner(void *pp)
{
    DEVMM_DRV_DEBUG_ARG("Free enter. (ptr=0x%llx)\n", (DVdeviceptr)(uintptr_t)pp);

    return devmm_free_managed((DVdeviceptr)(uintptr_t)pp);
}

drvError_t halMemFree(void *pp)
{
    return halMemFreeInner(pp);
}

drvError_t drvDeviceGetTransWay(void *src, void *dest, uint8_t *trans_type)
{
    DVdeviceptr dst_tmp = (DVdeviceptr)(uintptr_t)dest;
    DVdeviceptr src_tmp = (DVdeviceptr)(uintptr_t)src;

    if (trans_type == NULL) {
        DEVMM_DRV_ERR("Trans_type is NULL. (dst=0x%llx; src=0x%llx)\n", dst_tmp, src_tmp);
        return DRV_ERROR_INVALID_VALUE;
    }

    /* 1:pcie dma, 0:sdma */
    *trans_type = 0;
    DEVMM_DRV_DEBUG_ARG("Argument. (dst=0x%llx; src=0x%llx; trans_type=%u)\n", dst_tmp, src_tmp, *trans_type);
    return DRV_ERROR_NONE;
}

DVresult halMemcpy3D(void *pCopy)
{
    (void)pCopy;
    return DRV_ERROR_NOT_SUPPORT;
}

STATIC INLINE DVresult devmm_check_memcpy2d_input_valid(struct drvMem2D *copy2d)
{
    if ((copy2d->dst == NULL) || (copy2d->src == NULL)) {
        DEVMM_DRV_ERR("Input parameter src or dst is NULL. (dst_is_null=%d; src_is_null=%d)\n",
                      (copy2d->dst == NULL), (copy2d->src == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((copy2d->width > copy2d->dpitch) || (copy2d->width > copy2d->spitch)) {
        DEVMM_DRV_ERR("Dpitch and spitch should both larger than width. (dpitch=%llu; spitch=%llu; "
            "width=%llu)\n", copy2d->dpitch, copy2d->spitch, copy2d->width);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((copy2d->width == 0) || (copy2d->height == 0)) {
        DEVMM_DRV_ERR("Width and height should both larger than 0. (width=%llu; height=%llu)\n",
            copy2d->width, copy2d->height);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((!devmm_va_is_svm((virt_addr_t)(uintptr_t)copy2d->src)) && (!devmm_va_is_svm((virt_addr_t)(uintptr_t)copy2d->dst))) {
        DEVMM_RUN_INFO("Not support h2h copy. (dst=0x%llx; src=0x%llx)\n", copy2d->dst, copy2d->src);
        return DRV_ERROR_NOT_SUPPORT;
    }

    if ((copy2d->direction == DRV_MEMCPY_HOST_TO_HOST) || (copy2d->direction >= DRV_MEMCPY_DEVICE_TO_DEVICE)) {
        DEVMM_RUN_INFO("Not support h2h&d2d copy. (direction=%u)\n", copy2d->direction);
        return DRV_ERROR_NOT_SUPPORT;
    }

    if ((copy2d->height != 1) && (((copy2d->src + copy2d->spitch) < copy2d->src) || ((copy2d->dst + copy2d->dpitch) < copy2d->dst))) {
        DEVMM_DRV_ERR("Pitch is invalid. (src=0x%llx; dst=0x%llx; spitch=%llu; dpitch=%llu)\n", copy2d->src, copy2d->dst, copy2d->spitch, copy2d->dpitch);
        return DRV_ERROR_INVALID_VALUE;
    }
    return 0;
}

STATIC INLINE DVresult devmm_memcpy2d_sync(struct drvMem2D *memcopy2d)
{
    struct drvMem2D *copy2d = memcopy2d;
    struct devmm_ioctl_arg arg = {0};
    int incoherence_flag = 0;
    DVresult ret;

    DEVMM_DRV_DEBUG_ARG("Memcpy2d synchronize. (dst=0x%llx; src=0x%llx; dpitch=%llu; spitch=%llu; width=%llu; "
        "height=%llu; direction=%u)\n", copy2d->dst, copy2d->src, copy2d->dpitch, copy2d->spitch,
        copy2d->width, copy2d->height, copy2d->direction);

    ret = devmm_check_memcpy2d_input_valid(copy2d);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    arg.data.copy2d_para.dst = (DVdeviceptr)copy2d->dst;
    arg.data.copy2d_para.src = (DVdeviceptr)copy2d->src;
    arg.data.copy2d_para.dpitch = copy2d->dpitch;
    arg.data.copy2d_para.spitch = copy2d->spitch;
    arg.data.copy2d_para.width = copy2d->width;
    arg.data.copy2d_para.height = copy2d->height;
    arg.data.copy2d_para.direction = copy2d->direction;

    ret = devmm_memcpy2d_cache_incoherence(&arg.data.copy2d_para, &incoherence_flag);
    if (incoherence_flag != 0) {
        return ret;
    }

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEMCPY2D, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NOT_SUPPORT), "Memcpy2d synchronize error. (ret=%d; dst=0x%llx; src=0x%llx; dpitch=%llu; spitch=%llu; "
            "width=%llu; height=%llu; direction=%u)\n", ret, copy2d->dst, copy2d->src, copy2d->dpitch,
            copy2d->spitch, copy2d->width, copy2d->height, copy2d->direction);
        devmm_print_svm_va_info((uint64_t)(uintptr_t)copy2d->src, ret);
        devmm_print_svm_va_info((uint64_t)(uintptr_t)copy2d->dst, ret);
        return ret;
    }
    return DRV_ERROR_NONE;
}

STATIC INLINE DVresult devmm_memcpy2d_convert(struct drvMem2DAsync *copy2d_async)
{
    struct devmm_ioctl_arg arg = {0};
    struct drvMem2D *copy2d = &copy2d_async->copy2dInfo;
    struct DMA_ADDR *dma_addr = copy2d_async->dmaAddr;
    uint64_t len = copy2d->width * copy2d->height;
    DVresult ret;

    DEVMM_DRV_DEBUG_ARG("Memcpy2d_convert. (dst=0x%llx; src=0x%llx; dpitch=%llu; spitch=%llu; width=%llu; "
                        "height=%llu; fixed_size=%llu; direction=%u)\n", copy2d->dst, copy2d->src,
                        copy2d->dpitch, copy2d->spitch, copy2d->width, copy2d->height,
                        copy2d->fixed_size, copy2d->direction);

    ret = devmm_check_memcpy2d_input_valid(copy2d);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }
    if (copy2d->height > DEVMM_CONVERT2D_HEIGHT_MAX) {
        DEVMM_DRV_ERR("Height is larger than 5M. (height=%llu)\n", copy2d->height);
        return DRV_ERROR_INVALID_VALUE;
    }
    if (copy2d->fixed_size >= len) {
        DEVMM_DRV_ERR("Fixed_size should smaller than width*height. (fixed_size=%llu; width=%llu; "
                      "height=%llu)\n", copy2d->fixed_size, copy2d->width, copy2d->height);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (dma_addr == NULL) {
        DEVMM_DRV_ERR("Out parameter dmaAddr is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    /* host agent not surport convert */
    if (dma_addr->offsetAddr.devid >= DEVMM_MAX_PHY_DEVICE_NUM) {
        DEVMM_DRV_ERR("Devid is out of range. (devid=%u; range=0-%u)\n", dma_addr->offsetAddr.devid,
                      (UINT32)(DEVMM_MAX_PHY_DEVICE_NUM - 1));
        return DRV_ERROR_INVALID_VALUE;
    }

    arg.head.devid = dma_addr->offsetAddr.devid;
    arg.data.convrt_para.virt_id = dma_addr->offsetAddr.devid;
    arg.data.convrt_para.pDst = (DVdeviceptr)copy2d->dst;
    arg.data.convrt_para.pSrc = (DVdeviceptr)copy2d->src;
    arg.data.convrt_para.len = copy2d->width;
    arg.data.convrt_para.dpitch = copy2d->dpitch;
    arg.data.convrt_para.spitch = copy2d->spitch;
    arg.data.convrt_para.height = copy2d->height;
    arg.data.convrt_para.fixed_size = copy2d->fixed_size;
    arg.data.convrt_para.direction = copy2d->direction;

    return devmm_convert_proc(&arg, dma_addr);
}

drvError_t halMemcpy2DInner(struct MEMCPY2D *p_copy)
{
    struct MEMCPY2D *memcpy2d = p_copy;
    DVresult ret;

    if (memcpy2d == NULL) {
        DEVMM_DRV_ERR("Input parameter pCopy is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (memcpy2d->type == DEVMM_MEMCPY2D_SYNC) {
        ret = devmm_memcpy2d_sync(&memcpy2d->copy2d);
    } else if (memcpy2d->type == DEVMM_MEMCPY2D_ASYNC_CONVERT) {
        ret = devmm_memcpy2d_convert(&memcpy2d->copy2dAsync);
    } else if (memcpy2d->type == DEVMM_MEMCPY2D_ASYNC_DESTROY) {
        ret = drvMemDestroyAddr(memcpy2d->copy2dAsync.dmaAddr);
    } else {
        DEVMM_DRV_ERR("Input parameter type is invalid. (type=%u)\n", memcpy2d->type);
        ret = DRV_ERROR_INVALID_VALUE;
    }

    return ret;
}

drvError_t halMemcpy2D(struct MEMCPY2D *p_copy)
{
    return halMemcpy2DInner(p_copy);
}

static drvError_t devmm_batch_va_is_svm(uint64_t dst[], uint64_t src[], size_t count)
{
    unsigned int i;

    for(i = 0; i < count; i++) {
        if ((devmm_va_is_svm(dst[i]) == 0) && (devmm_va_is_svm(src[i]) == 0)) {
            DEVMM_RUN_INFO("Memcpy batch not support h2h. (dst=0x%llx; src=0x%llx; index=%u)\n", dst[i], src[i], i);
            return DRV_ERROR_NOT_SUPPORT;
        }
    }
    return DRV_ERROR_NONE;
}

static drvError_t devmm_memcpy_batch_para_check(uint64_t dst[], uint64_t src[], size_t size[], size_t count)
{
    if ((dst == NULL) || (src == NULL) || (size == NULL) || (count == 0)) {
        DEVMM_DRV_ERR("Memcpy batch para check failed. (dst_is_null=%d; src_is_null=%d; size_is_null=%d; count=%lu)\n",
            (dst == NULL), (src == NULL), (size == NULL), count);
        return DRV_ERROR_PARA_ERROR;
    }
    if (count > DEVMM_MEMCPY_BATCH_MAX_COUNT) {
        return DRV_ERROR_NOT_SUPPORT;
    }
    return devmm_batch_va_is_svm(dst, src, count);
}

static drvError_t halMemcpyBatchInner(uint64_t dst[], uint64_t src[], size_t size[], size_t count)
{
    struct devmm_ioctl_arg arg = {0};
    drvError_t ret;

    ret = devmm_memcpy_batch_para_check(dst, src, size, count);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }
    arg.data.copy_batch_para.dst = (uint64_t *)dst;
    arg.data.copy_batch_para.src = (uint64_t *)src;
    arg.data.copy_batch_para.size = size;
    arg.data.copy_batch_para.addr_count = (uint32_t)count;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEMCPY_BATCH, &arg);
    if (ret != DRV_ERROR_NONE) {
        if (ret != DRV_ERROR_NOT_SUPPORT) {
             DEVMM_DRV_ERR("Memcpy batch ioctl error. (ret=%d)\n", ret);
        }
        return ret;
    }
    return DRV_ERROR_NONE;
}

DVresult halMemcpyBatch(uint64_t dst[], uint64_t src[], size_t size[], size_t count)
{
    return halMemcpyBatchInner(dst, src, size, count);
}

static DVresult devmm_set_mem_advise(unsigned int type, DVmem_advise *advise)
{
    if (type == ADVISE_PERSISTENT) {
        *advise = DV_ADVISE_PERSISTENT;
    } else if (type == ADVISE_DEV_MEM) {
        *advise = (DV_ADVISE_POPULATE | DV_ADVISE_LOCK_DEV);
    } else {
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

drvError_t halMemAdvise(DVdeviceptr ptr, size_t count, unsigned int type, DVdevice device)
{
    DVmem_advise mem_advise;
    drvError_t ret;

    ret = devmm_set_mem_advise(type, &mem_advise);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return drv_mem_advise(ptr, count, mem_advise, device);
}

static void devmm_process_status_result_show(DVdevice device, processType_t process_type, processStatus_t status, u32 result)
{
    if (status != STATUS_SVM_PAGE_FALUT_ERR_CNT) {
        if (result == (u32)true) {
            if (status == STATUS_NOMEM) {
                svm_mem_stats_show_device(device);
            }
            DEVMM_RUN_INFO("Process status occurs. (devid=%u; process_type=%d; status=%d)\n",
                device, process_type, status);
        }
    }
}

drvError_t halCheckProcessStatus(
    DVdevice device, processType_t process_type, processStatus_t status, bool *is_matched)
{
    struct drv_process_status_output out = {0};
    drvError_t ret;

    if (is_matched == NULL) {
        DEVMM_DRV_ERR("Input parameter isMatched is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = halCheckProcessStatusEx(device, process_type, status, &out);
    if (ret == 0) {
        if ((status == STATUS_NOMEM) || (status == STATUS_SVM_PAGE_FALUT_ERR_OCCUR)) {
            *is_matched = ((out.result != 0) ? true : false);
        } else {
            ret = DRV_ERROR_NOT_SUPPORT;
        }
    }

    return ret;
}

drvError_t halCheckProcessStatusEx(
    DVdevice device, processType_t process_type, processStatus_t status, struct drv_process_status_output *out)
{
    struct devmm_ioctl_arg arg = {0};
    DVresult ret;

    if (device >= DEVMM_MAX_PHY_DEVICE_NUM || (int)status <= 0) {
        DEVMM_DRV_ERR("Only support devid 0-63, status %d-%d(devid=%u, status=%d)\n",
            STATUS_NOMEM, STATUS_MAX - 1, device, status);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (process_type != PROCESS_CP1 || status >= STATUS_MAX) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (out == NULL) {
        DEVMM_DRV_ERR("Input parameter out is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    arg.head.devid = device;
    arg.data.query_process_status_para.process_type = process_type;
    arg.data.query_process_status_para.pid_status = status;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_PROCESS_STATUS_QUERY, &arg);
    if (ret != 0) {
        DEVMM_DRV_ERR("Query process status error. (ret=%d; devid=%u; processType=%d)\n", ret, device, process_type);
        return ret;
    }

    out->result = arg.data.query_process_status_para.status_result;
    devmm_process_status_result_show(device, process_type, status, out->result);

    return DRV_ERROR_NONE;
}

static pthread_mutex_t g_vmm_handle_lock[DEVMM_MAX_LOGIC_DEVICE_NUM];
static void __attribute__((constructor)) devmm_vmm_handle_lock_init(void)
{
    uint64_t devid;

    for (devid = 0; devid < DEVMM_MAX_LOGIC_DEVICE_NUM; devid++) {
        (void)pthread_mutex_init(&g_vmm_handle_lock[devid], NULL);
    }
}

static drvError_t devmm_giant_page_para_check(const struct drv_mem_prop *prop)
{
    if (prop->side != MEM_DEV_SIDE) {
        if (prop->mem_type != MEM_P2P_DDR_TYPE) {
            DEVMM_RUN_INFO("Only p2p ddr support host giant page. (side=%u; mem_type=%u)\n",
                prop->side, prop->mem_type);
            return DRV_ERROR_NOT_SUPPORT;
        }

        if (devmm_is_support_host_giant_page_feature() == false) {
            DEVMM_RUN_INFO("Not support host giane page feature.\n");
            return DRV_ERROR_NOT_SUPPORT;
        }
    } else {
        if (devmm_is_support_agent_giant_page_feature(prop->devid) == false) {
            DEVMM_RUN_INFO("Not support agent giane page feature. (devid=%u)\n", prop->devid);
            return DRV_ERROR_NOT_SUPPORT;
        }
    }

    if (prop->mem_type == MEM_TS_DDR_TYPE) {
        DEVMM_RUN_INFO("Giant page not support ts_ddr.\n");
        return DRV_ERROR_NOT_SUPPORT;
    }
    return DRV_ERROR_NONE;
}

static drvError_t devmm_mem_prop_check(const struct drv_mem_prop *prop, bool is_chk_host_id)
{
    if (prop == NULL) {
        DEVMM_DRV_ERR("Prop is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (prop->side >= MEM_MAX_SIDE) {
        DEVMM_DRV_ERR("Invalid side type. (side=%u)\n", prop->side);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (is_chk_host_id) {
        uint32_t host_id = SVM_MAX_AGENT_NUM;
        (void)halGetHostID(&host_id);
        if ((prop->side != MEM_HOST_NUMA_SIDE) && (prop->devid >= DEVMM_MAX_PHY_DEVICE_NUM)
            && (prop->devid != host_id)) {
            DEVMM_DRV_ERR("Invalid devid. (devid=%u; host_id=%u)\n", prop->devid, host_id);
            return DRV_ERROR_INVALID_VALUE;
        }
    } else {
        if ((prop->side != MEM_HOST_NUMA_SIDE) && prop->devid >= DEVMM_MAX_PHY_DEVICE_NUM) {
            DEVMM_DRV_ERR("Invalid devid. (devid=%u)\n", prop->devid);
            return DRV_ERROR_INVALID_VALUE;
        }
    }

    if (prop->pg_type >= MEM_MAX_PAGE_TYPE) {
        DEVMM_DRV_ERR("Invalid page type. (page_type=%u)\n", prop->pg_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (prop->mem_type >= MEM_MAX_TYPE) {
        DEVMM_DRV_ERR("Invalid mem type. (mem_type=%u)\n", prop->mem_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (prop->pg_type == MEM_GIANT_PAGE_TYPE) {
        drvError_t ret = devmm_giant_page_para_check(prop);
        if (ret != DRV_ERROR_NONE) {
            return ret;
        }
    }

    if (prop->reserve != 0) {
        DEVMM_DRV_ERR("Prop reserve shoule be zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

static drvError_t devmm_mem_create_para_check(drv_mem_handle_t **handle, size_t size,
    const struct drv_mem_prop *prop, uint64_t flag)
{
    if (handle == NULL) {
        DEVMM_DRV_ERR("Handle is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (size == 0) {
        DEVMM_DRV_ERR("Size is zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    /* reserved para, verify by zero for subsequent compatibility */
    if (flag != 0) {
        DEVMM_DRV_ERR("Flag shoule be zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    return devmm_mem_prop_check(prop, false);
}

static drvError_t devmm_vmm_mem_create(const struct drv_mem_prop *prop, size_t size, int *id, uint64_t *pg_num)
{
    uint32_t logic_devid, module_id, page_type, host_numa_id;
    uint32_t host_id = SVM_MAX_AGENT_NUM;
    struct devmm_ioctl_arg arg = {0};
    size_t real_size;
    drvError_t ret;

    if (devmm_is_mem_host_side(prop->side)) {
        (void)halGetHostID(&host_id);
    }

    page_type = (prop->pg_type == MEM_GIANT_PAGE_TYPE) ? MEM_GIANT_PAGE_TYPE : MEM_HUGE_PAGE_TYPE;
    page_type = (prop->pg_type == MEM_NORMAL_PAGE_TYPE) && (size < DEVMM_MEM_ALLOC_MINIMUN_GRANULARITY) &&
        (prop->module_id == RUNTIME_MODULE_ID) ? MEM_NORMAL_PAGE_TYPE : page_type;
    module_id = (prop->module_id == UNKNOWN_MODULE_ID) ? ASCENDCL_MODULE_ID : prop->module_id;
    logic_devid = devmm_is_mem_host_side(prop->side) ? host_id : prop->devid;
    real_size = (prop->pg_type == MEM_GIANT_PAGE_TYPE) ? align_up(size, DEVMM_GIANT_PAGE_SIZE) : size;
    host_numa_id = (prop->side == MEM_HOST_NUMA_SIDE) ? prop->devid : 0U;

    arg.head.devid = logic_devid;
    arg.data.mem_create_para.size = real_size;
    arg.data.mem_create_para.flag = 0;
    arg.data.mem_create_para.side = prop->side;
    arg.data.mem_create_para.module_id = module_id;
    arg.data.mem_create_para.pg_type = page_type;
    arg.data.mem_create_para.mem_type = prop->mem_type;
    arg.data.mem_create_para.host_numa_id = host_numa_id;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_CREATE, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_RUN_INFO_IF((ret != DRV_ERROR_NOT_SUPPORT), "Mem create is unsuccessful. (ret=%d; size=%u; side=%u; "
            "devid=%u; module_id=%u; pg_type=%u; mem_type=%u)\n",
            ret, real_size, prop->side, logic_devid, module_id, prop->pg_type, prop->mem_type);
        if (devmm_is_mem_host_side(prop->side)) {
            svm_mem_stats_show_host();
        } else {
            svm_mem_stats_show_device(logic_devid);
        }
        return ret;
    }
    *id = arg.data.mem_create_para.id;
    *pg_num = arg.data.mem_create_para.pg_num;   /* Just to control msg num of dev mem_release. */
    return DRV_ERROR_NONE;
}

static int devmm_vmm_create_record_restore(uint64_t key1, uint64_t key2, uint32_t devid, void *data)
{
    (void)key2;
    (void)devid;
    (void)data;
    drv_mem_handle_t *handle = (drv_mem_handle_t *)(uintptr_t)key1;
    struct devmm_virt_heap_mgmt *mgmt = NULL;
    struct drv_mem_prop prop = {0};
    uint64_t pg_num;
    int ret, id;

    mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (mgmt == NULL) {
        DEVMM_DRV_ERR("Get heap mangement error.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }

    prop.side = handle->side;
    prop.pg_type = handle->pg_type;
    prop.mem_type = handle->phy_mem_type;
    prop.devid = handle->devid;
    prop.module_id = handle->module_id;
    DEVMM_RUN_INFO("Create restore. (pg_num=%lu; devid=%u; module_id=%u; pg_type=%u; mem_type=%u; "
        "side=%u; id=%d)\n", handle->pg_num, handle->devid, handle->module_id, handle->pg_type,
        handle->phy_mem_type, handle->side, handle->id);
    ret = devmm_vmm_mem_create(&prop, handle->pg_num * mgmt->huge_page_size, &id, &pg_num);
    if (ret != 0) {
        return ret;
    }
    handle->id = id;
    return 0;
}

static drvError_t devmm_vmm_pa_alloc(const struct drv_mem_prop *prop, size_t size, uint64_t flag,
    int *id, uint64_t *pg_num)
{
    struct devmm_virt_heap_mgmt *mgmt = NULL;
    uint32_t host_id = SVM_MAX_AGENT_NUM;
    uint32_t logic_devid, module_id, mem_val;
    struct svm_mem_stats_type type;
    size_t real_size;
    drvError_t ret;

    mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (mgmt == NULL) {
        DEVMM_DRV_ERR("Get heap mangement error.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }
    if (devmm_is_mem_host_side(prop->side)) {
        (void)halGetHostID(&host_id);
    }
    ret = devmm_vmm_mem_create(prop, size, id, pg_num);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    logic_devid = devmm_is_mem_host_side(prop->side) ? host_id : prop->devid;
    real_size = (prop->pg_type == MEM_GIANT_PAGE_TYPE) ? align_up(size, DEVMM_GIANT_PAGE_SIZE) : size;
    module_id = (prop->module_id == UNKNOWN_MODULE_ID) ? ASCENDCL_MODULE_ID : prop->module_id;
    mem_val = devmm_is_mem_host_side(prop->side) ? MEM_HOST_VAL : MEM_DEV_VAL;
    svm_mem_stats_type_pack(&type, mem_val, MEM_HUGE_PAGE_TYPE, prop->mem_type);
    svm_mapped_size_inc(&type, logic_devid, real_size);
    svm_module_alloced_size_inc(&type, logic_devid, module_id, real_size);
    DEVMM_DRV_DEBUG_ARG("Create. (size=%lu; real_size=%lu; devid=%u; module_id=%u; pg_type=%u; mem_type=%u; "
        "flag=0x%lx; id=%d)\n",
        size, real_size, logic_devid, prop->module_id, prop->pg_type, prop->mem_type, flag, *id);
    return DRV_ERROR_NONE;
}

static drvError_t devmm_vmm_pa_free(const struct drv_mem_prop *prop, int id, uint64_t pg_num)
{
    struct devmm_virt_heap_mgmt *mgmt = NULL;
    struct devmm_ioctl_arg arg = {0};
    struct svm_mem_stats_type type;
    uint32_t host_id = SVM_MAX_AGENT_NUM;
    uint32_t mem_val, logic_devid;
    size_t size;
    drvError_t ret;

    mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (mgmt == NULL) {
        DEVMM_DRV_ERR("Get heap mangement error.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }

    if (devmm_is_mem_host_side(prop->side)) {
        (void)halGetHostID(&host_id);
    }
    logic_devid = devmm_is_mem_host_side(prop->side) ? host_id : prop->devid;
    arg.head.devid = logic_devid;
    arg.data.mem_release_para.id = id;
    arg.data.mem_release_para.side = prop->side;
    arg.data.mem_release_para.pg_num = pg_num;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_RELEASE, &arg);
    if (ret != DRV_ERROR_NONE) {
        /* The log cannot be modified, because in the failure mode library. */
        DEVMM_DRV_ERR("Mem release failed. (ret=%d; id=%d; side=%u; devid=%u)\n",
            ret, id, prop->side, logic_devid);
        return ret;
    }

    if (arg.data.mem_release_para.handle_type != SVM_MEM_HANDLE_IMPORT_TYPE) {
        mem_val = devmm_is_mem_host_side(prop->side) ? MEM_HOST_VAL : MEM_DEV_VAL;
        svm_mem_stats_type_pack(&type, mem_val, MEM_HUGE_PAGE_TYPE, prop->mem_type);
        size = pg_num * mgmt->huge_page_size;
        svm_mapped_size_dec(&type, logic_devid, size);
        svm_module_alloced_size_dec(&type, logic_devid, prop->module_id, size);
    }
    return ret;
}

static drvError_t devmm_vmm_create_record_data_create(drv_mem_handle_t *handle, const struct drv_mem_prop *prop)
{
    struct devmm_record_data data = {0};
    uint32_t logic_devid;
    drvError_t ret;

    logic_devid = devmm_is_mem_host_side(prop->side) ? DEVMM_RECORD_HOST_DEVID : prop->devid;
    data.key1 = (uint64_t)(uintptr_t)handle;
    data.key2 = DEVMM_RECORD_KEY_INVALID;
    ret = devmm_record_create_and_get(DEVMM_FEATURE_VMM_CREATE, logic_devid, DEVMM_KEY_TYPE1, DEVMM_NODE_INITED, &data);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Record get fail. (ret=%d; side=%u; devid=%u; module_id=%u; pg_type=%u; mem_type=%u)\n",
            ret, prop->side, prop->devid, prop->module_id, prop->pg_type, prop->mem_type);
        return ret;
    }
    return DRV_ERROR_NONE;
}

static void devmm_vmm_create_record_data_destroy(drv_mem_handle_t *handle)
{
    drvError_t ret;

    ret = devmm_record_put(DEVMM_FEATURE_VMM_CREATE, handle->devid, DEVMM_KEY_TYPE1, (uint64_t)(uintptr_t)handle,
        DEVMM_NODE_UNINITED);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_WARN("Record put warn. (ret=%d; id=%d; side=%u; devid=%u)\n",
            ret, handle->id, handle->side, handle->devid);
    }
}

static drv_mem_handle_t *devmm_vmm_pa_handle_create(const struct drv_mem_prop *prop, int id, uint64_t pg_num)
{
    drv_mem_handle_t *handle = NULL;
    uint32_t host_id = SVM_MAX_AGENT_NUM;
    drvError_t ret;
    int64_t value;

    ret = halGetDeviceInfo((devmm_is_mem_host_side(prop->side) ? 0 : prop->devid),
        MODULE_TYPE_SYSTEM, INFO_TYPE_SDID, &value);
    if (ret != DRV_ERROR_NONE) {
        if (ret != DRV_ERROR_NOT_SUPPORT) {
            DEVMM_DRV_ERR("Get sdid failed. (ret=%d; devid=%u)\n", ret, prop->devid);
            return NULL;
        }
        value = 0;
    }
    if (devmm_is_mem_host_side(prop->side)) {
        (void)halGetHostID(&host_id);
    }

    handle = malloc(sizeof(drv_mem_handle_t));
    if (handle == NULL) {
        DEVMM_DRV_ERR("Malloc handle failed.\n");
        return NULL;
    }

    ret = devmm_vmm_create_record_data_create(handle, prop);
    if (ret != DRV_ERROR_NONE) {
        free(handle);
        return NULL;
    }

    handle->id = id;
    handle->side = devmm_is_mem_host_side(prop->side) ? MEM_HOST_SIDE : MEM_DEV_SIDE;
    handle->sdid = (uint32_t)value;
    handle->devid = devmm_is_mem_host_side(prop->side) ? host_id : prop->devid;
    handle->pg_num = pg_num;   /* Just to control msg num of dev mem_release. */
    handle->module_id = (prop->module_id == UNKNOWN_MODULE_ID) ? ASCENDCL_MODULE_ID : prop->module_id;
    handle->pg_type = (prop->pg_type == MEM_GIANT_PAGE_TYPE && devmm_is_mem_host_side(prop->side))
        ? MEM_GIANT_PAGE_TYPE : MEM_HUGE_PAGE_TYPE;
    handle->phy_mem_type = prop->mem_type;
    handle->is_shared = false;
    handle->ref = 1;
    return handle;
}

static void devmm_vmm_pa_handle_destroy(drv_mem_handle_t *handle)
{
    devmm_vmm_create_record_data_destroy(handle);
    free(handle);
}

bool svm_support_vmm_normal_granularity(uint32_t dev_id)
{
    (void)dev_id;
    return !devmm_is_split_mode();
}

drvError_t halMemCreate(drv_mem_handle_t **handle, size_t size, const struct drv_mem_prop *prop, uint64_t flag)
{
    drv_mem_handle_t *tmp_handle = NULL;
    uint64_t pg_num;
    drvError_t ret;
    int id;

    ret = devmm_mem_create_para_check(handle, size, prop, flag);
    if (ret != 0) {
        return ret;
    }

    ret = devmm_vmm_pa_alloc(prop, size, flag, &id, &pg_num);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    tmp_handle = devmm_vmm_pa_handle_create(prop, id, pg_num);
    if (tmp_handle == NULL) {
        devmm_vmm_pa_free(prop, id, pg_num);
        return DRV_ERROR_OUT_OF_MEMORY;
    }
    *handle = tmp_handle;
    return DRV_ERROR_NONE;
}

static drvError_t devmm_mem_handle_check(drv_mem_handle_t *handle)
{
    uint32_t host_id = SVM_MAX_AGENT_NUM;

    (void)halGetHostID(&host_id);

    if (handle == NULL) {
        DEVMM_DRV_ERR("Handle is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((handle->side != MEM_HOST_SIDE) && (handle->side != MEM_DEV_SIDE)) {
        /* The log cannot be modified, because in the failure mode library. */
        DEVMM_DRV_ERR("Invalid handle side. (side=%u)\n", handle->side);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((handle->side == MEM_HOST_SIDE) && (handle->devid != host_id)) {
        /* The log cannot be modified, because in the failure mode library. */
        DEVMM_DRV_ERR("Host_side's devid should be zero. (devid=%u)\n", handle->devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((handle->side == MEM_DEV_SIDE) && (handle->devid >= DEVMM_MAX_LOGIC_DEVICE_NUM)) {
        /* The log cannot be modified, because in the failure mode library. */
        DEVMM_DRV_ERR("Invalid handle devid. (devid=%u)\n", handle->devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    if (handle->pg_type >= MEM_MAX_PAGE_TYPE) {
        DEVMM_DRV_ERR("Invalid page type. (page_type=%u)\n", handle->pg_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (handle->phy_mem_type >= MEM_MAX_TYPE) {
        DEVMM_DRV_ERR("Invalid mem type. (mem_type=%u)\n", handle->phy_mem_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

static drvError_t devmm_vmm_handle_ref_inc(drv_mem_handle_t *handle)
{
    drvError_t ret;
    ret = ((handle->ref > UINT32_MAX) || (handle->ref == 0)) ? DRV_ERROR_NO_RESOURCES : DRV_ERROR_NONE;
    if (ret == DRV_ERROR_NONE) {
        handle->ref++;
    }
    return ret;
}

static drvError_t devmm_vmm_handle_ref_dec(drv_mem_handle_t *handle)
{
    if (handle->ref == 0) {
        return DRV_ERROR_INVALID_VALUE;
    }
    handle->ref--;
    return DRV_ERROR_NONE;
}

static uint64_t devmm_vmm_handle_ref_read(drv_mem_handle_t *handle)
{
    return handle->ref;
}

static drvError_t svm_mem_release(drv_mem_handle_t *handle)
{
    struct drv_mem_prop prop = {0};
    drvError_t ret;
    uint64_t ref;

    prop.side = handle->side;
    prop.devid = handle->devid;
    prop.module_id = handle->module_id;
    prop.pg_type = handle->pg_type;
    prop.mem_type = handle->phy_mem_type;
    (void)pthread_mutex_lock(&g_vmm_handle_lock[handle->devid]);
    ref = devmm_vmm_handle_ref_read(handle);
    if ((ref == 0) || (ref > UINT32_MAX)) {
        DEVMM_DRV_ERR("Handle ref is invalid. (ref=%llu)\n", ref);
        (void)pthread_mutex_unlock(&g_vmm_handle_lock[handle->devid]);
        return DRV_ERROR_INVALID_VALUE;
    }
    if (ref > 1) {
        devmm_vmm_handle_ref_dec(handle);
        (void)pthread_mutex_unlock(&g_vmm_handle_lock[handle->devid]);
        return DRV_ERROR_NONE;
    }

    ret = devmm_vmm_pa_free(&prop, handle->id, handle->pg_num);
    if (ret != DRV_ERROR_NONE) {
        (void)pthread_mutex_unlock(&g_vmm_handle_lock[handle->devid]);
        return ret;
    }
    (void)devmm_record_put(DEVMM_FEATURE_IMPORT, handle->devid, DEVMM_KEY_TYPE2, (uint64_t)handle, DEVMM_NODE_UNINITED);
    devmm_vmm_handle_ref_dec(handle);
    (void)pthread_mutex_unlock(&g_vmm_handle_lock[handle->devid]);

    devmm_vmm_pa_handle_destroy(handle);
    return DRV_ERROR_NONE;
}

drvError_t halMemRelease(drv_mem_handle_t *handle)
{
    drvError_t ret;

    ret = devmm_mem_handle_check(handle);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    DEVMM_DRV_DEBUG_ARG("Release. (devid=%u; module_id=%u; pg_type=%u; mem_type=%u; pg_num=%llu; id=%d)\n",
        handle->devid, handle->module_id, handle->pg_type, handle->phy_mem_type, handle->pg_num, handle->id);
    return svm_mem_release(handle);
}

static uint32_t devmm_size_to_order(size_t size)
{
	size_t tmp_size = size;
    uint32_t order = 0;

    while (tmp_size >>= 1) {
        order++;
    }

	if ((size & ~(1UL << order)) != 0) {
		order++;
	}
    return order;
}

static drvError_t devmm_check_addr_is_order_align(uint64_t addr, size_t size)
{
    if (size > DEVMM_LARGE_MEM_THRESHOLD_SIZE) {
        if (IS_ALIGNED(addr, DEVMM_HEAP_SIZE) == false) {
            DEVMM_DRV_ERR("Size over 512M, addr should align with 1G. (addr=0x%llx; size=%lu)\n", addr, size);
            return DRV_ERROR_INVALID_VALUE;
        }
    } else if (size > DEVMM_MEM_ALLOC_RECOMMENDED_GRANULARITY) {
        uint32_t order = devmm_size_to_order(size);
        size_t align_size = 1UL << order;
        if (IS_ALIGNED(addr, align_size) == false) {
            DEVMM_DRV_ERR("Addr is not aligned with size. (addr=0x%llx; size=%lu; align_size=%lu)\n",
                addr, size, align_size);
            return DRV_ERROR_INVALID_VALUE;
        }
    }
    return DRV_ERROR_NONE;
}

static drvError_t devmm_mem_address_reserve_addr_size_check(uint64_t addr, size_t size)
{
    if (addr == 0) {
        return DRV_ERROR_NONE;
    }

    if (IS_ALIGNED(addr, DEVMM_MEM_ALLOC_RECOMMENDED_GRANULARITY) == false) {
        DEVMM_DRV_ERR("Addr should align. (addr=0x%llx; align_size=%lu)\n",
            addr, DEVMM_MEM_ALLOC_RECOMMENDED_GRANULARITY);
        return DRV_ERROR_INVALID_VALUE;
    }

    return devmm_check_addr_is_order_align(addr, size);
}

static drvError_t devmm_mem_address_reserve_para_check(void **ptr, size_t size,
    size_t alignment, uint64_t addr, uint64_t flag)
{
    if (ptr == NULL) {
        DEVMM_DRV_ERR("Ptr is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (size == 0) {
        DEVMM_DRV_ERR("Size is 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    /* reserved para, verify by zero for subsequent compatibility */
    if (alignment != 0) {
        DEVMM_DRV_ERR("Alignment shoule be zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (devmm_mem_address_reserve_addr_size_check(addr, size) != DRV_ERROR_NONE) {
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((flag & 0xFF) >= MEM_MAX_PAGE_TYPE) { /* 0xFF, pgtype mask */
        DEVMM_DRV_ERR("Invalid pg type. (flag=%llu)\n", flag);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

static int svm_vmm_dynamic_reserve(struct devmm_virt_heap_mgmt *mgmt, uint64_t *va, uint64_t size,
    DVmem_advise advise, uint64_t flag)
{
    uint32_t da_flag = ((flag & MEM_RSV_TYPE_DEVICE_SHARE) != 0) ? 0 : SVM_DA_FLAG_WITH_MASTER;
    struct devmm_virt_com_heap heap = {0};
    uint32_t heap_idx;
    int ret;

    ret = svm_da_alloc(va, size, da_flag);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Da alloc failed. (va=0x%llx; size=0x%llx; da_flag=%x)\n", *va, size, da_flag);
        return ret;
    }

    heap_idx = devmm_va_to_heap_idx(mgmt, (virt_addr_t)*va);
    ret = devmm_ioctl_enable_heap(heap_idx, DEVMM_HEAP_HUGE_PAGE, SUB_RESERVE_TYPE, size, RESERVE_LIST);
    if (ret != DRV_ERROR_NONE) {
        (void)svm_da_free(*va);
        DEVMM_DRV_ERR("Enable heap failed. (heap_idx=%u; va=0x%llx; size=0x%llx)\n", heap_idx, *va, size);
        return ret;
    }

    heap.heap_sub_type = SUB_RESERVE_TYPE;
    if (devmm_virt_heap_alloc_ops(&heap, *va, size, advise) < DEVMM_SVM_MEM_START) {
        DEVMM_DRV_ERR("Alloc heap ops failed. (heap_idx=%u; va=0x%llx; size=0x%llx; advise=%u)\n",
            heap_idx, *va, size, advise);
        (void)devmm_ioctl_disable_heap(heap_idx, DEVMM_HEAP_HUGE_PAGE, SUB_RESERVE_TYPE, size);
        (void)svm_da_free(*va);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    return DRV_ERROR_NONE;
}

static int svm_vmm_dynamic_free(struct devmm_virt_heap_mgmt *mgmt, uint64_t va)
{
    struct devmm_virt_com_heap heap = {0};
    uint64_t size;
    uint32_t heap_idx;
    int ret;

    ret = svm_da_query_size(va, &size);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Query size failed. (va=0x%llx)\n", va);
        return ret;
    }

    heap.heap_sub_type = SUB_RESERVE_TYPE;
    ret = devmm_virt_heap_free_ops(&heap, va);
    if (ret != 0) {
        DEVMM_DRV_ERR("Free heap ops failed. (va=0x%llx; size=0x%llx)\n", va, size);
        return DRV_ERROR_IOCRL_FAIL;
    }

    heap_idx = devmm_va_to_heap_idx(mgmt, (virt_addr_t)va);
    ret = devmm_ioctl_disable_heap(heap_idx, DEVMM_HEAP_HUGE_PAGE, SUB_RESERVE_TYPE, size);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Disable heap failed. (va=0x%llx; size=0x%llx)\n", va, size);
        return ret;
    }

    (void)svm_da_free(va);
    return DRV_ERROR_NONE;
}

drvError_t halMemAddressReserve(void **ptr, size_t size, size_t alignment, void *addr, uint64_t flag)
{
    struct devmm_virt_heap_mgmt *mgmt = NULL;
    DVmem_advise advise = {0};
    size_t real_size = size;
    drvError_t ret;

    ret = devmm_mem_address_reserve_para_check(ptr, size, alignment, (uint64_t)(uintptr_t)addr, flag);
    if (ret != 0) {
        return ret;
    }

    mgmt = devmm_virt_get_heap_mgmt();
    if (mgmt == NULL) {
        DEVMM_DRV_ERR("Get heap mangement error.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }

    *ptr = addr;
    advise = (addr != NULL) ? DV_ADVISE_NOCACHE : 0;
    if (size >= DEVMM_MEM_ALLOC_MINIMUN_GRANULARITY) {
        advise |= DV_ADVISE_HUGEPAGE;
    } else {
        real_size = DEVMM_MEM_ALLOC_MINIMUN_GRANULARITY; /* Ensure va alignment according to page_size */
    }

    if ((flag & MEM_RSV_TYPE_REMOTE_MAP) != 0) {
        return svm_vmm_dynamic_reserve(mgmt, (uint64_t *)ptr, (uint64_t)real_size, advise, flag);
    }

    ret = devmm_alloc_proc(0, SUB_RESERVE_TYPE, advise, real_size, (DVdeviceptr *)ptr);
    DEVMM_DRV_DEBUG_ARG("Reserve. (ptr=0x%llx; size=%lu; real_size=%lu; addr=0x%llx; flag=0x%llx; ret=%u)\n",
        *(DVdeviceptr *)ptr, size, real_size, (DVdeviceptr)(uintptr_t)addr, flag, ret);
    return ret;
}

drvError_t halMemAddressFree(void *ptr)
{
    struct devmm_virt_heap_mgmt *p_heap_mgmt = NULL;
    struct devmm_virt_com_heap *heap = NULL;
    u64 va = (DVdeviceptr)(uintptr_t)ptr;
    u64 free_len;

    DEVMM_DRV_DEBUG_ARG("Free. (ptr=0x%llx)\n", va);
    p_heap_mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (p_heap_mgmt == NULL) {
        DEVMM_DRV_ERR("Please call device_open and alloc_mem api first. (alloc_mem=0x%llx)\n", va);
        return DRV_ERROR_INVALID_HANDLE;
    }

    if (va >= DEVMM_MAX_DYN_ALLOC_BASE) {
        return svm_vmm_dynamic_free(p_heap_mgmt, va);
    }

    heap = devmm_va_to_heap(va);
    if ((heap == NULL) || heap->heap_type == DEVMM_HEAP_IDLE) {
        DEVMM_DRV_ERR("Address is not allocated. please check ptr. (offset=%llx)\n", ADDR_TO_OFFSET(va));
        return DRV_ERROR_INVALID_VALUE;
    }

    if (heap->heap_sub_type != SUB_RESERVE_TYPE) {
        DEVMM_DRV_ERR("Is not reserve addr. (va=0x%llx)\n", va);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (devmm_virt_heap_is_primary(heap)) {
        return devmm_free_to_base_heap(p_heap_mgmt, heap, va);
    } else {
        return devmm_free_to_normal_heap(p_heap_mgmt, heap, va, (uint64_t *)(uintptr_t)&free_len);
    }
}

static int devmm_mem_query_owner(void *ptr, size_t size, uint32_t *owner_phy_devid, uint32_t *local_handle_flag)
{
    struct devmm_ioctl_arg arg = {0};
    int ret;

    arg.data.mem_query_owner_para.va = (uint64_t)(uintptr_t)ptr;
    arg.data.mem_query_owner_para.size = size;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_QUERY_OWNER, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Query owner failed. (ret=%d; ptr=%p; size=%llu)\n", ret, ptr, (uint64_t)size);
        return ret;
    }

    *owner_phy_devid = arg.data.mem_query_owner_para.devid;
    *local_handle_flag = arg.data.mem_query_owner_para.local_handle_flag;

    return DRV_ERROR_NONE;
}

static int devmm_mem_set_access(void *ptr, size_t size, struct drv_mem_location location, drv_mem_access_type type)
{
    struct devmm_ioctl_arg arg = {0};
    uint32_t host_id = SVM_MAX_AGENT_NUM;
    int ret;

    (void)halGetHostID(&host_id);

    arg.data.mem_set_access_para.va = (uint64_t)(uintptr_t)ptr;
    arg.data.mem_set_access_para.size = size;
    arg.data.mem_set_access_para.logic_devid = (location.side == MEM_DEV_SIDE) ? location.id : host_id;
    arg.data.mem_set_access_para.type = type;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_SET_ACCESS, &arg);
    if (ret != DRV_ERROR_NONE) {
        /* not rollback */
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NOT_SUPPORT), "Set access failed. (ret=%d; ptr=%p; size=%llu)\n",
            ret, ptr, (uint64_t)size);
        return ret;
    }

    return DRV_ERROR_NONE;
}

static int devmm_mem_get_access(void *ptr, size_t *size, struct drv_mem_location *location, drv_mem_access_type *type)
{
    struct devmm_ioctl_arg arg = {0};
    uint32_t host_id = SVM_MAX_AGENT_NUM;
    int ret;

    (void)halGetHostID(&host_id);

    arg.data.mem_get_access_para.va = (uint64_t)(uintptr_t)ptr;
    arg.data.mem_get_access_para.size = (uint64_t)*size;
    arg.data.mem_get_access_para.logic_devid = (location->side == MEM_DEV_SIDE) ? location->id : host_id;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_GET_ACCESS, &arg);
    if (ret != DRV_ERROR_NONE) {
        /* not rollback */
        DEVMM_DRV_ERR("Get access failed. (ret=%d; ptr=%p; size=%llu)\n", ret, ptr, (uint64_t)(uintptr_t)size);
        return ret;
    }

    *type = arg.data.mem_get_access_para.type;
    *size = (size_t)arg.data.mem_get_access_para.size;

    return DRV_ERROR_NONE;
}

static drvError_t devmm_mem_config_access(void *ptr, size_t size, uint32_t owner_phy_devid,
    struct drv_mem_location location, drv_mem_access_type type)
{
    uint32_t host_id = SVM_MAX_AGENT_NUM;
    drvError_t ret;

    if (type == MEM_ACCESS_TYPE_NONE) {
        return DRV_ERROR_NONE;
    }

    (void)halGetHostID(&host_id);

    if (owner_phy_devid == host_id) {
        if (location.side == MEM_HOST_SIDE) {
            /* h2h map */
            return DRV_ERROR_NOT_SUPPORT; /* support latter */
        } else {
            /* d2h map, map in kernel */
        }
    } else {
        if (location.side == MEM_HOST_SIDE) {
            /* h2d map, map in kernel */
        } else {
            /* d2d map */
            ret = drvMemPrefetchToDevice((DVdeviceptr)(uintptr_t)ptr, size, location.id);
            if (ret != 0) {
                DEVMM_DRV_ERR("Call drvMemPrefetchToDevice failed. (ret=%d; ptr=%p; size=%llu; devid=%u)\n",
                    ret, ptr, (uint64_t)size, location.id);
                return ret;
            }
        }
    }

    return DRV_ERROR_NONE;
}

static void devmm_mem_cancle_access(void *ptr, size_t size, uint32_t owner_phy_devid,
    struct drv_mem_location location, drv_mem_access_type type)
{
    uint32_t host_id = SVM_MAX_AGENT_NUM;

    if (type == MEM_ACCESS_TYPE_NONE) {
        return;
    }

    (void)halGetHostID(&host_id);

    if (owner_phy_devid == host_id) {
        if (location.side == MEM_HOST_SIDE) {
            /* h2h unmap, support latter */
            DEVMM_DRV_DEBUG_ARG("Host side. (ptr=0x%llx; size=0x%lx)\n", (DVdeviceptr)(uintptr_t)ptr, size);
        } else {
            /* d2h unmap, unmap in kernel */
            DEVMM_DRV_DEBUG_ARG("Device side. (ptr=0x%llx; size=0x%lx)\n", (DVdeviceptr)(uintptr_t)ptr, size);
        }
    } else {
        if (location.side == MEM_HOST_SIDE) {
            /* h2d unmap, support latter */
            DEVMM_DRV_DEBUG_ARG("Host side. (ptr=0x%llx; size=0x%lx)\n", (DVdeviceptr)(uintptr_t)ptr, size);
        } else {
            /* d2d unmap, drvMemPrefetchToDevice can not be cancled */
            DEVMM_DRV_DEBUG_ARG("Device side. (ptr=0x%llx; size=0x%lx)\n", (DVdeviceptr)(uintptr_t)ptr, size);
        }
    }
}

static void devmm_mem_cancle_all_access(void *ptr)
{
    size_t size = 1;
    int owner_valid = 0;
    uint32_t i, num_dev;
    int ret;

    ret = drvGetDevNum(&num_dev);
    if (ret != DRV_ERROR_NONE) {
        return;
    }

    for (i = 0;  i < num_dev; i++) {
        struct drv_mem_location location = {.side = MEM_DEV_SIDE, .id = i};
        uint32_t owner_phy_devid, local_handle_flag;
        drv_mem_access_type type;
        ret = devmm_mem_get_access(ptr, &size, &location, &type);
        if (ret != 0) {
            continue;
        }

        if ((type != MEM_ACCESS_TYPE_READ) && (type != MEM_ACCESS_TYPE_READWRITE)) {
            continue;
        }

        if (owner_valid == 0) {
            ret = devmm_mem_query_owner(ptr, size, &owner_phy_devid, &local_handle_flag);
            if (ret != DRV_ERROR_NONE) {
                DEVMM_DRV_ERR("Query owner failed. (ret=%d; ptr=%p; size=%llu; devid=%u)\n",
                    ret, ptr, (uint64_t)size, location.id);
                break;
            }
            owner_valid = 1;
        }

        if (i != owner_phy_devid) {
            devmm_mem_cancle_access(ptr, size, owner_phy_devid, location, type);
        }
    }
}

static int devmm_mem_map_para_check(void *ptr, size_t size,
    size_t offset, drv_mem_handle_t *handle, uint64_t flag)
{
    if (ptr == NULL) {
        DEVMM_DRV_ERR("Ptr is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (size == 0) {
        DEVMM_DRV_ERR("Size is 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    /* reserved para, verify by zero for subsequent compatibility */
    if (offset != 0) {
        DEVMM_DRV_ERR("Offset shoule be zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    /* reserved para, verify by zero for subsequent compatibility */
    if (flag != 0) {
        DEVMM_DRV_ERR("Flag shoule be zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    return devmm_mem_handle_check(handle);
}

struct devmm_record_vmm_map_data {
    uint64_t size;
    uint64_t offset;
    drv_mem_handle_t *handle;
    uint64_t flag;
};

static drvError_t devmm_mem_map(void *ptr, size_t size, size_t offset, drv_mem_handle_t *handle, uint64_t flag)
{
    (void)offset;
    (void)flag;
    struct devmm_ioctl_arg arg = {0};
    drvError_t ret;

    arg.head.devid = handle->devid;
    arg.data.mem_map_para.va = (uint64_t)(uintptr_t)ptr;
    arg.data.mem_map_para.size = size;
    arg.data.mem_map_para.id = handle->id;
    arg.data.mem_map_para.side = handle->side;
    arg.data.mem_map_para.module_id = handle->module_id;
    arg.data.mem_map_para.pg_num = handle->pg_num;
    arg.data.mem_map_para.pg_type = handle->pg_type;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_MAP, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Mem map failed. (ret=%d; ptr=0x%llx; size=%llu; id=%d; side=%u; devid=%u)\n",
            ret, (uint64_t)(uintptr_t)ptr, (uint64_t)size, handle->id, handle->side, handle->devid);
        return ret;
    }
    return DRV_ERROR_NONE;
}

static drvError_t devmm_mem_unmap(void *ptr, uint32_t *devid)
{
    struct devmm_ioctl_arg arg = {0};
    drvError_t ret;

    arg.data.mem_unmap_para.va = (uint64_t)(uintptr_t)ptr;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_UNMAP, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Mem unmap failed. (ret=%d; ptr=0x%llx)\n", ret, (uint64_t)(uintptr_t)ptr);
    }

    *devid = arg.data.mem_unmap_para.logic_devid;
    if (arg.data.mem_unmap_para.side == MEM_DEV_SIDE) {
        uint64_t unmap_size = arg.data.mem_unmap_para.unmap_size;
        devmm_del_mem_advise_record(false, (uint64_t)(uintptr_t)ptr, unmap_size, *devid);
    }

    return ret;
}

static int devmm_vmm_map_record_restore(uint64_t key1, uint64_t key2, uint32_t devid, void *data)
{
    (void)key2;
    (void)devid;
    struct devmm_record_vmm_map_data *map_data = data;
    drv_mem_handle_t *handle = map_data->handle;

    if (handle->is_shared) {
        return 0;
    }

    DEVMM_RUN_INFO("Map restore. (va=0x%llx; size=%lu; offset=%lu; flag=0x%lx; id=%d; devid=%u; side=%u)\n",
        key1, map_data->size, map_data->offset, map_data->flag, handle->id, handle->devid, handle->side);
    return devmm_mem_map((void *)key1, map_data->size, map_data->offset, handle, map_data->flag);
}

static drvError_t devmm_vmm_map_record_data_create(void *ptr, size_t size, size_t offset, drv_mem_handle_t *handle,
    uint64_t flag)
{
    uint64_t data_len = sizeof(struct devmm_record_vmm_map_data);
    struct devmm_record_vmm_map_data map_data = {0};
    struct devmm_record_data data = {0};
    drvError_t ret;

    map_data.size = size;
    map_data.offset = offset;
    map_data.handle = handle;
    map_data.flag = flag;

    data.key1 = (uint64_t)(uintptr_t)ptr;
    data.key2 = DEVMM_RECORD_KEY_INVALID;
    data.data_len = data_len;
    data.data = (void *)&map_data;
    ret = devmm_record_create_and_get(DEVMM_FEATURE_VMM_MAP, handle->devid, DEVMM_KEY_TYPE1, DEVMM_NODE_INITED, &data);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Record get failed. (ret=%d; ptr=0x%llx; size=%llu; id=%d; side=%u; devid=%u)\n",
            ret, (uint64_t)(uintptr_t)ptr, (uint64_t)size, handle->id, handle->side, handle->devid);
        return ret;
    }
    return DRV_ERROR_NONE;
}

static void devmm_vmm_map_record_data_destroy(void *ptr, uint32_t devid)
{
    uint64_t va = (uint64_t)(uintptr_t)ptr;
    drvError_t ret;

    ret = devmm_record_put(DEVMM_FEATURE_VMM_MAP, devid, DEVMM_KEY_TYPE1, va, DEVMM_NODE_UNINITED);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_WARN("Record put warn. (ret=%d; ptr=0x%llx)\n", ret, va);
    }
}

static drvError_t devmm_vmm_map_record_data_get(void *ptr, uint32_t devid, drv_mem_handle_t **handle)
{
    uint64_t data_len = sizeof(struct devmm_record_vmm_map_data);
    struct devmm_record_vmm_map_data map_data = {0};
    struct devmm_record_data data = {0};
    drvError_t ret;

    data.key1 = (uint64_t)(uintptr_t)ptr;
    data.key2 = DEVMM_RECORD_KEY_INVALID;
    data.data_len = data_len;
    data.data = (void *)&map_data;
    ret = devmm_record_get(DEVMM_FEATURE_VMM_MAP, devid, DEVMM_KEY_TYPE1, &data);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Record get failed. (ret=%d; ptr=0x%llx; devid=%u)\n",
            ret, (uint64_t)(uintptr_t)ptr, devid);
        return ret;
    }
    *handle = map_data.handle;
    return DRV_ERROR_NONE;
}

static void devmm_vmm_map_record_data_put(void *ptr, uint32_t devid)
{
    uint64_t va = (uint64_t)(uintptr_t)ptr;
    drvError_t ret;

    ret = devmm_record_put(DEVMM_FEATURE_VMM_MAP, devid, DEVMM_KEY_TYPE1, va, DEVMM_NODE_UNINITED);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_WARN("Record put warn. (ret=%d; ptr=0x%llx)\n", ret, va);
    }
}

drvError_t halMemMap(void *ptr, size_t size, size_t offset, drv_mem_handle_t *handle, uint64_t flag)
{
    drvError_t ret;

    ret = devmm_mem_map_para_check(ptr, size, offset, handle, flag);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    DEVMM_DRV_DEBUG_ARG("Map. (ptr=0x%llx; size=%lu; offset=%u; id=%d; flag=0x%llx)\n",
        (DVdeviceptr)(uintptr_t)ptr, size, offset, handle->id, flag);
    ret = devmm_vmm_map_record_data_create(ptr, size, offset, handle, flag);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    (void)pthread_mutex_lock(&g_vmm_handle_lock[handle->devid]);
    ret = devmm_vmm_handle_ref_inc(handle);
    (void)pthread_mutex_unlock(&g_vmm_handle_lock[handle->devid]);
    if (ret != DRV_ERROR_NONE) {
        devmm_vmm_map_record_data_destroy(ptr, handle->devid);
        return ret;
    }

    ret = devmm_mem_map(ptr, size, offset, handle, flag);
    if (ret != DRV_ERROR_NONE) {
        (void)pthread_mutex_lock(&g_vmm_handle_lock[handle->devid]);
        devmm_vmm_handle_ref_dec(handle);
        (void)pthread_mutex_unlock(&g_vmm_handle_lock[handle->devid]);
        devmm_vmm_map_record_data_destroy(ptr, handle->devid);
        return ret;
    }

    /* For drvMemGetAttribute, quickly get side and devid, won't fail, just save not clear, kernel will verify. */
    if ((uint64_t)(uintptr_t)ptr < DEVMM_MAX_DYN_ALLOC_BASE) {
        (void)devmm_save_map_info((uint64_t)(uintptr_t)ptr, handle->side, handle->devid);
    }
    return DRV_ERROR_NONE;
}

drvError_t halMemUnmap(void *ptr)
{
    drv_mem_handle_t *handle = NULL;
    drvError_t ret;
    uint32_t devid;

    devmm_mem_cancle_all_access(ptr);

    DEVMM_DRV_DEBUG_ARG("Unmap. (ptr=0x%llx)\n", (DVdeviceptr)(uintptr_t)ptr);
    ret = halMemRetainAllocationHandle(&handle, ptr);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = devmm_mem_unmap(ptr, &devid);
    if (ret != DRV_ERROR_NONE) {
        halMemRelease(handle);
        return ret;
    }

    (void)pthread_mutex_lock(&g_vmm_handle_lock[devid]);
    devmm_vmm_map_record_data_destroy(ptr, devid);
    (void)pthread_mutex_unlock(&g_vmm_handle_lock[devid]);
    svm_mem_release(handle);    /* ref dec */
    halMemRelease(handle);
    return DRV_ERROR_NONE;
}

drvError_t halMemRetainAllocationHandle(drv_mem_handle_t **handle, void *ptr)
{
    struct DVattribute attr = {0};
    drv_mem_handle_t *tmp_handle = NULL;
    uint64_t va = (uint64_t)(uintptr_t)ptr;
    drvError_t ret;

    if ((handle == NULL) || (ptr == NULL)) {
        DEVMM_DRV_ERR("Invalid para. (handle=%d; ptr=%d)\n", (handle == NULL), (ptr == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = drvMemGetAttribute(va, &attr);
    if (ret != 0) {
        DEVMM_DRV_ERR("Va is invalid. (va=0x%llx; ret=%d)\n", va, ret);
        return ret;
    }

    (void)pthread_mutex_lock(&g_vmm_handle_lock[attr.devId]);
    ret = devmm_vmm_map_record_data_get(ptr, attr.devId, &tmp_handle);
    if (ret != DRV_ERROR_NONE) {
        (void)pthread_mutex_unlock(&g_vmm_handle_lock[attr.devId]);
        DEVMM_DRV_ERR("Get record data failed. (va=0x%llx; ret=%d)\n", va, ret);
        return ret;
    }
    ret = devmm_vmm_handle_ref_inc(tmp_handle);
    if (ret == DRV_ERROR_NONE) {
        *handle = tmp_handle;
        DEVMM_DRV_DEBUG_ARG("Retain. (ptr=0x%llx; pg_num=%llu; side=%u; id=%d; devid=%u)\n",
            (DVdeviceptr)(uintptr_t)ptr, tmp_handle->pg_num, tmp_handle->side, tmp_handle->id, tmp_handle->devid);
    }
    devmm_vmm_map_record_data_put(ptr, attr.devId);
    (void)pthread_mutex_unlock(&g_vmm_handle_lock[attr.devId]);
    return ret;
}

static bool svm_is_mem_access_type_valid(drv_mem_access_type type)
{
    return ((type == MEM_ACCESS_TYPE_NONE) || (type == MEM_ACCESS_TYPE_READ) || (type == MEM_ACCESS_TYPE_READWRITE));
}

static drvError_t svm_mem_set_access(void *ptr, size_t size, struct drv_mem_location location, drv_mem_access_type type)
{
    uint32_t owner_phy_devid, local_handle_flag, map_phy_devid;
    drvError_t ret;

    if ((ptr == NULL) || (location.side >= MEM_MAX_SIDE) || (!svm_is_mem_access_type_valid(type))) {
        DEVMM_DRV_ERR("Invalid para. (side=%u; type=%u)\n", location.side, type);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = devmm_mem_query_owner(ptr, size, &owner_phy_devid, &local_handle_flag);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    if (local_handle_flag == 1) {
        if (location.side == MEM_HOST_SIDE) {
            map_phy_devid = 0;
            (void)halGetHostID(&map_phy_devid);
        } else {
            ret = drvDeviceGetPhyIdByIndex(location.id, &map_phy_devid);
            if (ret != DRV_ERROR_NONE) {
                DEVMM_DRV_ERR("Query phy devid failed. (ret=%d; devid=%u)\n", ret, location.id);
                return ret;
            }
        }

        if (map_phy_devid == owner_phy_devid) {
            /* modify read write prop, support latter */
            return DRV_ERROR_NONE;
        }
    }

    ret = devmm_mem_config_access(ptr, size, owner_phy_devid, location, type);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = devmm_mem_set_access(ptr, size, location, type);
    if (ret != DRV_ERROR_NONE) {
        devmm_mem_cancle_access(ptr, size, owner_phy_devid, location, type);
        return ret;
    }

    return DRV_ERROR_NONE;
}

static int devmm_access_location_check(struct drv_mem_location *location)
{
    if (location->side == MEM_HOST_SIDE) {
        if (location->id != 0) {
            DEVMM_DRV_ERR("Invalid host id. (id=%u)\n", location->id);
            return DRV_ERROR_INVALID_VALUE;
        }
    } else if (location->side == MEM_DEV_SIDE) {
        if (location->id >= DEVMM_MAX_PHY_DEVICE_NUM) {
            DEVMM_DRV_ERR("Invalid dev id. (id=%u)\n", location->id);
            return DRV_ERROR_INVALID_VALUE;
        }
    } else {
        DEVMM_DRV_ERR("Invalid side. (side=%u)\n", location->side);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

static int devmm_access_desc_check(struct drv_mem_access_desc *desc, size_t count)
{
    size_t i;

    if ((desc == NULL) || (count == 0)) {
        DEVMM_DRV_ERR("Invalid desc. (count=%u)\n", (uint32_t)count);
        return DRV_ERROR_INVALID_VALUE;
    }

    for (i = 0; i < count; i++) {
        int ret;

        if ((desc[i].type == MEM_ACCESS_TYPE_NONE) || (desc[i].type == MEM_ACCESS_TYPE_READ)) {
            DEVMM_RUN_INFO("Not support type. (index=%u; type=%u)\n", i, desc[i].type);
            return DRV_ERROR_NOT_SUPPORT;
        }

        if ((desc[i].type != MEM_ACCESS_TYPE_NONE) && (desc[i].type != MEM_ACCESS_TYPE_READ)
            && (desc[i].type != MEM_ACCESS_TYPE_READWRITE)) {
            DEVMM_DRV_ERR("Invalid type. (index=%u; type=%u)\n", i, desc[i].type);
            return DRV_ERROR_INVALID_VALUE;
        }

        ret = devmm_access_location_check(&desc[i].location);
        if (ret != DRV_ERROR_NONE) {
            DEVMM_DRV_ERR("Invalid location. (index=%u)\n", i);
            return ret;
        }
    }

    return DRV_ERROR_NONE;
}

drvError_t halMemSetAccess(void *ptr, size_t size, struct drv_mem_access_desc *desc, size_t count)
{
    drvError_t ret;
    size_t i;

    if (ptr == NULL) {
        DEVMM_DRV_ERR("Null ptr.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = devmm_access_desc_check(desc, count);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    for (i = 0; i < count; i++) {
        drv_mem_access_type type;
        size_t get_size = size;

        ret = devmm_mem_get_access(ptr, &get_size, &desc[i].location, &type);
        if (ret != DRV_ERROR_NONE) {
            DEVMM_DRV_ERR("Get access failed. (ret=%d; ptr=%p; size=0x%llx)\n", ret, ptr, (uint64_t)size);
            return ret;
        }

        if (type != MEM_ACCESS_TYPE_NONE) {
            if ((desc[i].type != type) || (get_size != size)) {
                DEVMM_DRV_ERR("Repeat set with diff type or size. "
                    "(ptr=%p; size=0x%llx; desc.type=%u; type=%u; get_size=0x%llx)\n",
                    ptr, (uint64_t)size, desc[i].type, type, (uint64_t)get_size);
                return DRV_ERROR_BUSY;
            }
            continue;
        }

        ret = svm_mem_set_access(ptr, size, desc[i].location, desc[i].type);
        if (ret != DRV_ERROR_NONE) { /* not need rollback */
            return ret;
        }
    }

    return DRV_ERROR_NONE;
}

drvError_t halMemGetAccess(void *ptr, struct drv_mem_location *location, uint64_t *flags)
{
    drv_mem_access_type type;
    size_t size = 1;
    drvError_t ret;

    if ((ptr == NULL) || (flags == NULL) || (location == NULL)) {
        DEVMM_DRV_ERR("Null ptr.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = devmm_access_location_check(location);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = devmm_mem_get_access(ptr, &size, location, &type);
    if (ret == DRV_ERROR_NONE) {
        *flags = (uint64_t)type;
    }

    return ret;
}

#define DEVMM_SHARE_HANDLE_DEVID_OFFSET 32
#define DEVMM_SHARE_HANDLE_DEVID_MASK 0xff
#define DEVMM_SHARE_HANDLE_ID_MASK 0xffffffff
/* shareable_handle(64 bit) = reserve(24 bit) + share_devid(8 bit) + share_id(32 bit) */
static void devmm_share_handle_pack(uint64_t *shareable_handle, int share_id, uint32_t share_devid)
{
    *shareable_handle = (uint64_t)share_id;
    *shareable_handle |= ((uint64_t)share_devid << DEVMM_SHARE_HANDLE_DEVID_OFFSET);
}

static int devmm_get_share_id(uint64_t shareable_handle)
{
    return (int)(shareable_handle & DEVMM_SHARE_HANDLE_ID_MASK);
}

static uint32_t devmm_get_share_devid(uint64_t shareable_handle)
{
    return (uint32_t)((shareable_handle >> DEVMM_SHARE_HANDLE_DEVID_OFFSET) & DEVMM_SHARE_HANDLE_DEVID_MASK);
}

static drvError_t devmm_mem_export_para_check(drv_mem_handle_t *handle)
{
    uint32_t host_id = SVM_MAX_AGENT_NUM;

    (void)halGetHostID(&host_id);

    if (devmm_is_split_mode()) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (handle == NULL) {
        DEVMM_DRV_ERR("Handle is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((handle->side != MEM_HOST_SIDE) && (handle->side != MEM_DEV_SIDE)) {
        DEVMM_DRV_ERR("Invalid handle side. (side=%u)\n", handle->side);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (((handle->side == MEM_HOST_SIDE) && (handle->devid != host_id)) ||
        ((handle->side == MEM_DEV_SIDE) && (handle->devid >= SVM_MAX_AGENT_NUM))) {
        DEVMM_DRV_ERR("Invalid handle devid. (devid=%u)\n", handle->devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    return DRV_ERROR_NONE;
}

struct devmm_share_handle {
    drv_mem_handle_type handle_type;
    uint32_t sdid;
    uint32_t devid;
    uint32_t phy_devid; /* cross server use */
    int share_id;
};

static drvError_t devmm_export_to_share_handle(drv_mem_handle_t *handle, drv_mem_handle_type handle_type,
    uint64_t flags, struct devmm_share_handle *share_handle)
{
    (void)flags;
    struct devmm_ioctl_arg arg = {0};
    uint32_t host_id = SVM_MAX_AGENT_NUM;
    uint32_t phy_devid;
    drvError_t ret;

    ret = devmm_mem_export_para_check(handle);
    if (ret != 0) {
        return ret;
    }

    (void)halGetHostID(&host_id);

    if (handle->devid == host_id) {
        phy_devid = host_id;
    } else {
        ret = drvDeviceGetPhyIdByIndex(handle->devid, &phy_devid);
        if (ret != DRV_ERROR_NONE) {
            DEVMM_DRV_ERR("Get phy devid failed. (ret=%d; devid=%u; id=%d)\n", ret, handle->devid, handle->id);
            return ret;
        }
    }

    arg.head.devid = handle->devid;
    arg.data.mem_export_para.side = (int)handle->side;
    arg.data.mem_export_para.id = handle->id;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_EXPORT, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Mem export failed. (ret=%d; devid=%u; id=%d)\n", ret, handle->devid, handle->id);
        return ret;
    }

    handle->is_shared = true;
    share_handle->handle_type = handle_type;
    share_handle->sdid = handle->sdid;
    share_handle->devid = handle->devid;
    share_handle->phy_devid = phy_devid;
    share_handle->share_id = arg.data.mem_export_para.share_id;

    return DRV_ERROR_NONE;
}

drvError_t halMemExportToShareableHandleV2(drv_mem_handle_t *handle, drv_mem_handle_type handle_type,
    uint64_t flags, struct MemShareHandle *share_handle)
{
    drvError_t ret;

    if (share_handle == NULL) {
        DEVMM_DRV_ERR("share_handle is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (handle_type >= MEM_HANDLE_TYPE_MAX) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (flags != 0) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = devmm_export_to_share_handle(handle, handle_type, flags, (struct devmm_share_handle *)(void *)share_handle);
    if (ret != 0) {
        return ret;
    }

    return DRV_ERROR_NONE;
}

drvError_t halMemExportToShareableHandle(drv_mem_handle_t *handle, drv_mem_handle_type handle_type,
    uint64_t flags, uint64_t *shareable_handle)
{
    struct devmm_share_handle share_handle;
    drvError_t ret;

    if (shareable_handle == NULL) {
        DEVMM_DRV_ERR("shareable_handle is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (handle_type != MEM_HANDLE_TYPE_NONE) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = devmm_export_to_share_handle(handle, handle_type, flags, &share_handle);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    devmm_share_handle_pack(shareable_handle, share_handle.share_id, handle->devid);
    DEVMM_DRV_DEBUG_ARG("Mem export. (devid=%u; id=%d; share_id=%d; shareableHandle=%llu)\n",
        handle->devid, handle->id, share_handle.share_id, *shareable_handle);

    return DRV_ERROR_NONE;
}

/* trans struct MemShareHandle to u64 shareable_handle(devid + share id), to set/get attribute in local */
drvError_t halMemTransShareableHandle(drv_mem_handle_type handle_type, struct MemShareHandle *share_handle,
    uint32_t *server_id, uint64_t *shareable_handle)
{
    struct devmm_share_handle *_share_handle = (struct devmm_share_handle *)(void *)share_handle;
    struct halSDIDParseInfo sdid_parse;
    drvError_t ret;

    if ((share_handle == NULL) || (server_id == NULL) || (shareable_handle == NULL)) {
        DEVMM_DRV_ERR("handle is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((_share_handle->handle_type != handle_type) || (handle_type >= MEM_HANDLE_TYPE_MAX)) {
        DEVMM_DRV_ERR("Invalid para. (handle_type=%u; share_handle.handle_type=%u)\n",
            handle_type, _share_handle->handle_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = halParseSDID(_share_handle->sdid, &sdid_parse);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Parse sdid failed. (handle_type=%u; sdid=%u)\n", handle_type, _share_handle->sdid);
        return ret;
    }

    devmm_share_handle_pack(shareable_handle, _share_handle->share_id, _share_handle->devid);
    *server_id = sdid_parse.server_id;
    return DRV_ERROR_NONE;
}

static drvError_t devmm_share_handle_para_check(uint64_t shareable_handle)
{
    uint32_t host_id = SVM_MAX_AGENT_NUM;
    uint32_t devid = devmm_get_share_devid(shareable_handle);

    (void)halGetHostID(&host_id);
    if ((devid >= SVM_MAX_AGENT_NUM) && (devid != host_id)) {
        DEVMM_DRV_ERR("Invalid shareable_handle. (shareable_handle=%llu; devid=%u)\n", shareable_handle, devid);
        return DRV_ERROR_INVALID_HANDLE;
    }

    return DRV_ERROR_NONE;
}

static drvError_t devmm_mem_import_para_check(drv_mem_handle_type handle_type,
    struct devmm_share_handle *share_handle, uint32_t devid, drv_mem_handle_t **handle)
{
    uint32_t host_id = SVM_MAX_AGENT_NUM;
    uint64_t shareable_handle;

    if (devmm_is_split_mode()) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if ((handle == NULL) || (share_handle == NULL)) {
        DEVMM_DRV_ERR("handle is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    (void)halGetHostID(&host_id);
    if ((devid >= DEVMM_MAX_LOGIC_DEVICE_NUM) && (devid != host_id)) {
        DEVMM_DRV_ERR("Invalid devid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    if ((share_handle->handle_type != handle_type) || (handle_type >= MEM_HANDLE_TYPE_MAX)) {
        DEVMM_DRV_ERR("Invalid para. (handle_type=%u; share_handle.handle_type=%u)\n",
            handle_type, share_handle->handle_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    devmm_share_handle_pack(&shareable_handle, share_handle->share_id, share_handle->devid);

    return devmm_share_handle_para_check(shareable_handle);
}

static drvError_t devmm_import_record_create_and_get(uint64_t shareable_handle, uint32_t devid,
    drv_mem_handle_t **handle, enum devmm_record_node_status status)
{
    struct devmm_record_data data = {0};
    drvError_t ret;

    if (status == DEVMM_NODE_INITING) {
        data.key1 = shareable_handle;
        data.key2 = DEVMM_RECORD_KEY_INVALID;
        (void)pthread_mutex_lock(&g_vmm_handle_lock[devid]);
        ret = devmm_record_create_and_get(DEVMM_FEATURE_IMPORT, devid, DEVMM_KEY_TYPE1, DEVMM_NODE_INITING, &data);
        if (ret != DRV_ERROR_TRY_AGAIN) {
            if (ret == DRV_ERROR_NONE) {
                *handle = (drv_mem_handle_t *)(uintptr_t)data.key2;
                ret = devmm_vmm_handle_ref_inc(*handle);
                (void)devmm_record_put(DEVMM_FEATURE_IMPORT, devid, DEVMM_KEY_TYPE1, shareable_handle,
                    DEVMM_NODE_UNINITED);
                (void)pthread_mutex_unlock(&g_vmm_handle_lock[devid]);
                return ret;
            }
            (void)pthread_mutex_unlock(&g_vmm_handle_lock[devid]);
            DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NONE), "Record get failed. (ret=%d; shareable_handle=%llu; devid=%u)\n",
                ret, shareable_handle, devid);
            return ret;
        }
        (void)pthread_mutex_unlock(&g_vmm_handle_lock[devid]);
        return ret;
    } else {
        data.key1 = shareable_handle;
        data.key2 = (uint64_t)(uintptr_t)(*handle);
        (void)devmm_record_create_and_get(DEVMM_FEATURE_IMPORT, devid, DEVMM_KEY_TYPE1, DEVMM_NODE_INITED, &data);
        return DRV_ERROR_NONE;
    }
}

static bool devmm_import_is_support_record(drv_mem_handle_type handle_type, struct devmm_share_handle *share_handle)
{
    struct halSDIDParseInfo sdid_parse;
    int64_t value;
    int ret;
    
    if (handle_type == MEM_HANDLE_TYPE_NONE) {
        return true;
    }

    ret = halParseSDID(share_handle->sdid, &sdid_parse);
    if (ret != DRV_ERROR_NONE) {
        return false;
    }

    ret = halGetDeviceInfo(0, MODULE_TYPE_SYSTEM, INFO_TYPE_SERVER_ID, &value);
    return (ret == DRV_ERROR_NONE) ? (value == sdid_parse.server_id) : false;
}

static drvError_t devmm_import_from_share_handle(drv_mem_handle_type handle_type,
    struct devmm_share_handle *share_handle, uint32_t devid, drv_mem_handle_t **handle)
{
    drv_mem_handle_t *tmp_handle = NULL;
    struct devmm_ioctl_arg arg = {0};
    uint64_t shareable_handle;
    drvError_t ret;

    ret = devmm_mem_import_para_check(handle_type, share_handle, devid, handle);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    tmp_handle = calloc(1, sizeof(drv_mem_handle_t));
    if (tmp_handle == NULL) {
        DEVMM_DRV_ERR("Malloc handle failed.\n");
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    devmm_share_handle_pack(&shareable_handle, share_handle->share_id, share_handle->devid);

    if (devmm_import_is_support_record(handle_type, share_handle)) {
        ret = devmm_import_record_create_and_get(shareable_handle, devid, handle, DEVMM_NODE_INITING);
        if (ret != DRV_ERROR_TRY_AGAIN) {
            free(tmp_handle);
            return ret;
        }
    }

    arg.head.devid = devid;
    arg.data.mem_import_para.share_sdid = share_handle->sdid;
    arg.data.mem_import_para.handle_type = handle_type;
    arg.data.mem_import_para.share_id = share_handle->share_id;
    arg.data.mem_import_para.share_devid = share_handle->devid;
    arg.data.mem_import_para.share_phy_devid = share_handle->phy_devid;

    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_IMPORT, &arg);
    if (ret != DRV_ERROR_NONE) {
        if (devmm_import_is_support_record(handle_type, share_handle)) {
            (void)devmm_record_put(DEVMM_FEATURE_IMPORT, devid, DEVMM_KEY_TYPE1, shareable_handle, DEVMM_NODE_UNINITED);
        }
        free(tmp_handle);
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NOT_SUPPORT), "Mem import failed. (ret=%d; shareable_handle=%llu; devid=%u)\n",
            ret, shareable_handle, devid);
        return ret;
    }

    if (devmm_import_is_support_record(handle_type, share_handle)) {
        (void)devmm_import_record_create_and_get(shareable_handle, devid, &tmp_handle, DEVMM_NODE_INITED);
    }

    /* share: id side and devid is belong to import dev */
    tmp_handle->id = arg.data.mem_import_para.id;
    tmp_handle->side = arg.data.mem_import_para.side;
    tmp_handle->devid = devid;
    tmp_handle->pg_num = arg.data.mem_import_para.pg_num;
    tmp_handle->pg_type = arg.data.mem_import_para.pg_type;
    tmp_handle->module_id = arg.data.mem_import_para.module_id;
    tmp_handle->is_shared = true;
    tmp_handle->ref = 1;

    *handle = tmp_handle;
    DEVMM_DRV_DEBUG_ARG("Mem import. (share_devid=%u; share_id=%d; devid=%u; id=%d)\n",
        arg.data.mem_import_para.share_devid, arg.data.mem_import_para.share_id, devid, tmp_handle->id);
    return DRV_ERROR_NONE;
}

drvError_t halMemImportFromShareableHandleV2(drv_mem_handle_type handle_type,
    struct MemShareHandle *share_handle, uint32_t devid, drv_mem_handle_t **handle)
{
    struct devmm_share_handle *_share_handle = (struct devmm_share_handle *)(void *)share_handle;
    return devmm_import_from_share_handle(handle_type, _share_handle, devid, handle);
}

drvError_t halMemImportFromShareableHandle(uint64_t shareable_handle,
    uint32_t devid, drv_mem_handle_t **handle)
{
    struct devmm_share_handle share_handle;
    uint32_t host_id = SVM_MAX_AGENT_NUM;
    uint32_t tmp_devid = devid;
    int64_t value;
    drvError_t ret;

    if (devid >= DEVMM_MAX_LOGIC_DEVICE_NUM) {
        DEVMM_DRV_ERR("Invalid devid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    (void)halGetHostID(&host_id);

    if (devid == host_id) {
        tmp_devid = 0; /* host import use devid 0 to get sdid */
    }

    ret = halGetDeviceInfo(tmp_devid, MODULE_TYPE_SYSTEM, INFO_TYPE_SDID, &value);
    if (ret != DRV_ERROR_NONE) {
        if (ret != DRV_ERROR_NOT_SUPPORT) {
            DEVMM_DRV_ERR("Get sdid failed. (ret=%d; devid=%u)\n", ret, tmp_devid);
            return ret;
        }
        value = 0;
    }

    share_handle.handle_type = MEM_HANDLE_TYPE_NONE;
    share_handle.sdid = (uint32_t)value;
    share_handle.devid = devmm_get_share_devid(shareable_handle);
    share_handle.phy_devid = 0; /* not care in single app */
    share_handle.share_id = devmm_get_share_id(shareable_handle);

    return devmm_import_from_share_handle(MEM_HANDLE_TYPE_NONE, &share_handle, devid, handle);
}

static drvError_t devmm_set_pid_para_check(uint64_t shareable_handle, int pid[], uint32_t pid_num)
{
    if (pid == NULL) {
        DEVMM_DRV_ERR("Pid is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (pid_num == 0) {
        DEVMM_DRV_ERR("Pid num is zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (pid_num > DEVMM_SHARE_MEM_MAX_PID_CNT) {
        DEVMM_DRV_ERR("Pid num is invalid. (pid_num=%u)\n", pid_num);
        return DRV_ERROR_INVALID_VALUE;
    }
    return devmm_share_handle_para_check(shareable_handle);
}

drvError_t halMemSetPidToShareableHandle(uint64_t shareable_handle, int pid[], uint32_t pid_num)
{
    struct devmm_ioctl_arg arg = {0};
    drvError_t ret;

    if (devmm_is_split_mode()) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = devmm_set_pid_para_check(shareable_handle, pid, pid_num);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    arg.head.devid = devmm_get_share_devid(shareable_handle);
    arg.data.mem_set_pid_para.share_id = devmm_get_share_id(shareable_handle);
    arg.data.mem_set_pid_para.pid_num = pid_num;
    arg.data.mem_set_pid_para.pid_list = pid;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_SET_PID, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Set pid failed. (ret=%d; shareable_handle=%llu; pid_num=%u)\n",
            ret, shareable_handle, pid_num);
    }
    return ret;
}

static drvError_t devmm_mem_set_attr_para_check(uint64_t shareable_handle, enum ShareHandleAttrType type)
{
    if ((u32)type >= SHR_HANDLE_ATTR_TYPE_MAX) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    return devmm_share_handle_para_check(shareable_handle);
}

drvError_t halMemShareHandleSetAttribute(uint64_t shareable_handle, enum ShareHandleAttrType type,
    struct ShareHandleAttr attr)
{
    struct devmm_ioctl_arg arg = {0};
    drvError_t ret;

    if (devmm_is_split_mode()) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = devmm_mem_set_attr_para_check(shareable_handle, type);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    arg.head.devid = devmm_get_share_devid(shareable_handle);
    arg.data.mem_set_attr_para.share_id = devmm_get_share_id(shareable_handle);
    arg.data.mem_set_attr_para.type = type;
    arg.data.mem_set_attr_para.attr = attr;
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_SET_ATTR, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NOT_SUPPORT),
            "Share handle set attr failed. (ret=%d; shareable_handle=%llu; type=%u)\n", ret, shareable_handle, type);
        return ret;
    }

    DEVMM_DRV_DEBUG_ARG("Share handle set attr successfully. (shareable_handle=%llu; type=%u)\n", shareable_handle, type);

    return DRV_ERROR_NONE;
}

static drvError_t devmm_mem_get_attr_para_check(uint64_t shareable_Handle, enum ShareHandleAttrType type,
    struct ShareHandleAttr *attr)
{
    if ((u32)type >= SHR_HANDLE_ATTR_TYPE_MAX) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (attr == NULL) {
        DEVMM_DRV_ERR("attr is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    return devmm_share_handle_para_check(shareable_Handle);
}

drvError_t halMemShareHandleGetAttribute(uint64_t shareable_handle, enum ShareHandleAttrType type,
    struct ShareHandleAttr *attr)
{
    struct devmm_ioctl_arg arg = {0};
    drvError_t ret;

    if (devmm_is_split_mode()) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = devmm_mem_get_attr_para_check(shareable_handle, type, attr);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    arg.data.mem_get_attr_para.type = type;
    arg.data.mem_get_attr_para.share_devid = devmm_get_share_devid(shareable_handle);
    arg.data.mem_get_attr_para.share_id = devmm_get_share_id(shareable_handle);
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_GET_ATTR, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR_IF((ret != DRV_ERROR_NOT_SUPPORT),
            "Share handle get attr failed. (ret=%d; shareable_handle=%llu; type=%u)\n", ret, shareable_handle, type);
        return ret;
    }

    *attr = arg.data.mem_get_attr_para.attr;
    DEVMM_DRV_DEBUG_ARG("Share handle get attr successfully. (shareable_handle=%llu; type=%u)\n", shareable_handle, type);

    return DRV_ERROR_NONE;
}

static drvError_t devmm_mem_info_get_para_check(uint64_t shareable_handle, struct ShareHandleGetInfo *info)
{
    if (info == NULL) {
        DEVMM_DRV_ERR("info is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }
    return devmm_share_handle_para_check(shareable_handle);
}

drvError_t halMemShareHandleInfoGet(uint64_t shareable_handle, struct ShareHandleGetInfo *info)
{
    struct devmm_ioctl_arg arg = {0};
    drvError_t ret;

    if (devmm_is_split_mode()) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = devmm_mem_info_get_para_check(shareable_handle, info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    arg.data.mem_get_info_para.share_devid = devmm_get_share_devid(shareable_handle);
    arg.data.mem_get_info_para.share_id = devmm_get_share_id(shareable_handle);
    ret = devmm_svm_ioctl(g_devmm_mem_dev, DEVMM_SVM_MEM_GET_INFO, &arg);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_ERR("Share handle get info failed. (ret=%d; shareable_handle=%llu)\n", ret, shareable_handle);
        return ret;
    }

    *info = arg.data.mem_get_info_para.info;

    DEVMM_DRV_DEBUG_ARG("Share handle get info successfully. (shareable_handle=%llu; phy_devid=%u)\n",
        shareable_handle, info->phyDevid);

    return DRV_ERROR_NONE;
}

drvError_t halMemGetAllocationGranularity(const struct drv_mem_prop *prop,
    drv_mem_granularity_options option, size_t *granularity)
{
    uint32_t host_id = SVM_MAX_AGENT_NUM;
    drvError_t ret;

    if (option >= MEM_ALLOC_GRANULARITY_INVALID) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (granularity == NULL) {
        DEVMM_DRV_ERR("granularity is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = devmm_mem_prop_check(prop, true);
    if (ret != 0) {
        return ret;
    }
    (void)halGetHostID(&host_id);
    if ((option == MEM_ALLOC_GRANULARITY_MINIMUM) && (prop->pg_type == MEM_NORMAL_PAGE_TYPE) &&
        (prop->devid == host_id) && (prop->module_id == RUNTIME_MODULE_ID)) {
        *granularity = (size_t)getpagesize();
    } else if (prop->pg_type == MEM_GIANT_PAGE_TYPE) {
        *granularity = DEVMM_GIANT_PAGE_SIZE;
    } else {
        *granularity = DEVMM_MEM_ALLOC_RECOMMENDED_GRANULARITY;
    }
    return DRV_ERROR_NONE;
}

drvError_t halMemGetAddressRange(DVdeviceptr ptr, DVdeviceptr *pbase, size_t *psize)
{
    struct devmm_mem_info mem_info = {.start = 0, .end = 0, .module_id = SVM_INVALID_MODULE_ID,
        .devid = SVM_MAX_AGENT_NUM};

    if ((pbase == NULL) && (psize == NULL)) {
        DEVMM_DRV_ERR("Invalid argument. (pbase=NULL; psize=NULL)\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (devmm_va_is_in_svm_range(ptr) == false) {
        DEVMM_DRV_ERR("Invalid argument. (ptr=0x%llx)\n", ptr);
        return DRV_ERROR_INVALID_VALUE;
    }

    devmm_get_svm_va_info(ptr, &mem_info);
    if (mem_info.start == 0) {
        DEVMM_DRV_ERR("ptr is not allocated. (ptr=0x%llx)\n", ptr);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (pbase != NULL) {
        *pbase = mem_info.start;
    }

    if (psize != NULL) {
        *psize = mem_info.end - mem_info.start + 1;
    }

    return DRV_ERROR_NONE;
}

static void devmm_restore_func_register(void)
{
    devmm_record_restore_func_register(DEVMM_FEATURE_VMM_CREATE, devmm_vmm_create_record_restore);
    devmm_record_restore_func_register(DEVMM_FEATURE_VMM_MAP, devmm_vmm_map_record_restore);
}

static void __attribute__((constructor)) devmm_svm_pre_init(void)
{
    devmm_init_mem_advise_record();
}
