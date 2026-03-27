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
#include <ascend_hal.h>

#include "utils.h"

#define ST_SIDE_HOST                   0ULL
#define ST_SIDE_DEVICE                 1ULL

#define ST_NPAGE_TYPE                  0ULL
#define ST_HPAGE_TYPE                  1ULL
#define ST_GPAGE_TYPE                  2ULL

static inline char *get_side_str(uint64_t side)
{
    static char *side_str[MEM_MAX_SIDE] = {
        [ST_SIDE_HOST] = "HOST",
        [ST_SIDE_DEVICE] = "DEV"
    };

    return side_str[side];
}

static inline char *get_pg_type_str(uint64_t pg_type)
{
    static char *pg_type_str[MEM_MAX_PAGE_TYPE] = {
        [ST_NPAGE_TYPE] = "NORMAL_PAGE",
        [ST_HPAGE_TYPE] = "HUGE_PAGE",
        [ST_GPAGE_TYPE] = "GIANT_PAGE"
    };

    return pg_type_str[pg_type];
}

static inline char *get_mem_type_str(uint64_t mem_type)
{
    static char *mem_type_str[MEM_MAX_TYPE] = {
        [MEM_HBM_TYPE] = "HBM",
        [MEM_DDR_TYPE] = "DDR",
        [MEM_P2P_HBM_TYPE] = "P2P_HBM",
        [MEM_P2P_DDR_TYPE] = "P2P_DDR",
        [MEM_TS_DDR_TYPE] = "TS_DDR"
    };

    return mem_type_str[mem_type];
}

typedef uint32_t (*FUNC_TDT_OPEN)(uint32_t, uint32_t);
typedef uint32_t (*FUNC_TDT_CLOSE)(uint32_t);
int hlt_devmm_tsd_pull_cp_process(DVdevice device_id)
{
 
    void *handle = dlopen("libtsdclient.so", RTLD_LAZY);
    if (!handle) {
        char *dlError = dlerror();
        LOG_ERR("open libtsdclient.so failed, dlerror() = %s\n", dlError);
        return EOF;
    }
    FUNC_TDT_OPEN func = (FUNC_TDT_OPEN)dlsym(handle, "TsdOpen");
    if (!func) {
        LOG_ERR("TsdOpen is null \n");
        dlclose(handle);
        return EOF;
    }
    uint32_t tdtStatus = func(device_id, 0);
    if (tdtStatus != 0) {
        LOG_ERR("TsdOpen is null,tdtStatus:%d, device_id:%d \n", tdtStatus, device_id);
        dlclose(handle);
        return EOF;
    }
 
    dlclose(handle);
 
    int ret = drvMemDeviceOpen(device_id, 0);
    if(ret != 0) {
        LOG_ERR("devmm_drvMemDeviceOpen fail, devid = %u, ret = %d\n", device_id, ret);
        return ret;
    }
 
    return 0;
}

static int st_svm_open_cp(uint32_t cp_mode, uint32_t devid, HDC_SESSION *session)
{
    return hlt_devmm_tsd_pull_cp_process(devid);
}

int hlt_devmm_tsd_close_cp_process(DVdevice device_id)
{
    int ret = drvMemDeviceClose(device_id);
    if(ret != 0) {
        LOG_ERR("drvMemDeviceClose fail, devid = %u, ret = %d\n", device_id, ret);
        return ret;
    }
 
    void *handle = dlopen("libtsdclient.so", RTLD_LAZY);
    if (!handle) {
        char *dlError = dlerror();
        LOG_ERR("open libtsdclient.so failed, dlerror() = %s\n", dlError);
        return -1;
    }
 
    FUNC_TDT_CLOSE func = (FUNC_TDT_CLOSE)dlsym(handle, "TsdClose");
    if (!func) {
        LOG_ERR("TsdClose is null \n");
        dlclose(handle);
        return -1;
    }
    uint32_t tdtStatus = func(device_id);
    if (tdtStatus != 0) {
        LOG_ERR("TsdOpen is null,tdtStatus:%d \n", tdtStatus);
        dlclose(handle);
        return -1;
    }
    dlclose(handle);

    return 0;
}

void st_svm_close_cp(uint32_t cp_mode, uint32_t devid)
{
    hlt_devmm_tsd_close_cp_process(devid);
}

static int st_svm_sva_copy_para_init(uint32_t devid, uint64_t size, bool is_huge,
    void **master_ptr, void **agent_ptr1, void **agent_ptr2)
{
    uint64_t flag;
    int ret;

    (void)is_huge;
    flag = MEM_HOST;
    ret = halMemAlloc(master_ptr, size, flag);
    if (ret != 0) {
        LOG_ERR("Alloc master ptr err. (ret=%d)\n", ret);
        return ret;
    }

    flag = devid | MEM_DEV | MEM_TYPE_HBM | MEM_PAGE_HUGE;
    ret = halMemAlloc(agent_ptr1, size, flag);
    if (ret != 0) {
        LOG_ERR("Alloc agent ptr err. (ret=%d)\n", ret);
        (void)halMemFree(*master_ptr);
        return ret;
    }

    flag = devid | MEM_DEV | MEM_TYPE_HBM | MEM_PAGE_HUGE;
    ret = halMemAlloc(agent_ptr2, size, flag);
    if (ret != 0) {
        LOG_ERR("Alloc agent ptr err. (ret=%d)\n", ret);
        (void)halMemFree(*agent_ptr1);
        (void)halMemFree(*master_ptr);
    }

    return ret;
}

static void st_svm_sva_copy_para_uninit(void *master_ptr, void *agent_ptr1, void *agent_ptr2)
{
    (void)halMemFree(master_ptr);
    (void)halMemFree(agent_ptr1);
    (void)halMemFree(agent_ptr2);
}

int st_write_addr_by_int(uint64_t addr, uint64_t size, uint32_t value)
{
    uint64_t i;

    //LOG_INFO("Write info. (va=0x%lx; size=%lu; value=%d)\n", addr, size, value);
    for (i = 0; i < (size / sizeof(int)); i++) {
        uint64_t tmp_va = addr + i * sizeof(int);
        *(int *)tmp_va = value;
    }

    return 0;
}

int st_verify_addr_by_int(uint64_t addr, uint64_t size, uint32_t value)
{
    uint64_t i;

    for (i = 0; i < (size / sizeof(int)); i++) {
        uint64_t tmp_va = addr + i * sizeof(int);
        if (*(int *)tmp_va != (int)value) {
            LOG_ERR("Verify data err. (i=%ld; va=0x%lx; expect=%d; truely=%d)\n",
                i, tmp_va, (int)value, *(int *)tmp_va);
            return -1;
        }
    }

    return 0;
}

#define UNUSED(x) (void)(x)
int st_svm_sva_copy_normal_test_004(void *hdc_session, uint32_t devid, uint64_t arg[10])
{
    struct memcpy_info info = {.devid = devid};
    void *master_ptr = NULL;
    void *agent_ptr = NULL;
    uint64_t i;
    uint64_t size = 4096;
    uint64_t is_local_host = 0;
    uint64_t is_local_dev = 0;
    uint64_t is_huge = 1;
    uint64_t memset_after_alloc = 1;
    uint64_t loop_cnt = 10;
    uint64_t flag;
    int ret;

    UNUSED(hdc_session);

    if (is_local_host) {
        master_ptr = malloc(size);
        if (master_ptr == NULL) {
            LOG_ERR("malloc failed.\n");
            return -1;
        }
    } else {
        ret = halMemAlloc(&master_ptr, size, MEM_HOST);
        if (ret != 0) {
            LOG_ERR("halMemAlloc host failed. (ret=%d)\n", ret);
            return ret;
        }
    }
    LOG_INFO("Alloc host mem success. (va=0x%lx)\n", (uint64_t)master_ptr);

    if (memset_after_alloc) {
        ret = drvMemsetD8((DVdeviceptr)master_ptr, size, 0, size);
        if (ret != 0) {
            LOG_ERR("drvMemsetD8 host failed. (ret=%d)\n", ret);
            return ret;
        }
        LOG_INFO("drvMemsetD8 host mem success. (va=0x%lx)\n", (uint64_t)master_ptr);
    }


    flag = MEM_DEV | devid;
    flag = (is_huge) ? (flag | MEM_PAGE_HUGE) : flag;
    ret = halMemAlloc(&agent_ptr, size, flag);
    if (ret != 0) {
        LOG_ERR("Alloc dev svm mem failed. (ret=%d)\n", ret);
        return ret;
    }

    LOG_INFO("Alloc dev mem success. (devid=%u; va=0x%lx)\n", devid, (uint64_t)agent_ptr);

    if (memset_after_alloc && (is_local_dev == 0)) {
        ret = drvMemsetD8((DVdeviceptr)agent_ptr, size, 0, size);
        if (ret != 0) {
            LOG_ERR("drvMemsetD8 dev failed. (ret=%d)\n", ret);
            return ret;
        }
        LOG_INFO("drvMemsetD8 dev mem success. (va=0x%lx)\n", (uint64_t)agent_ptr);
    }

    for (i = 0; i < loop_cnt; i++) {

        info.devid = devid;
        info.dir = DRV_MEMCPY_HOST_TO_DEVICE;

        ret = st_write_addr_by_int((DVdeviceptr)master_ptr, size, 2);
        if (ret != 0) {
            return ret;
        }

        ret = drvMemcpy((DVdeviceptr)agent_ptr, size, (DVdeviceptr)master_ptr, size);
        if (ret != 0) {
            LOG_ERR("drvMemcpy failed. (ret=%d)\n", ret);
            return ret;
        }
        LOG_INFO("drvMemcpy h2d success.\n");

        drvMemsetD8((DVdeviceptr)master_ptr, size, 0, size);

        info.devid = devid;
        info.dir = DRV_MEMCPY_DEVICE_TO_HOST;
        ret = drvMemcpy((DVdeviceptr)master_ptr, size, (DVdeviceptr)agent_ptr, size);
        if (ret != 0) {
            LOG_ERR("drvMemcpy failed. (ret=%d)\n", ret);
            return ret;
        }
        LOG_INFO("drvMemcpy d2h success.\n");

        ret = st_verify_addr_by_int((DVdeviceptr)master_ptr, size, 2);
        if (ret != 0) {
            LOG_ERR("data verify failed.(ret=%d)\n", ret);
            return ret;
        }
        LOG_INFO("verify data d2h success.\n");
    }

    if (is_local_host) {
        free(master_ptr);
    } else {
        ret = halMemFree(master_ptr);
        if (ret != 0) {
            LOG_ERR("halMemFree host failed. (ret=%d)\n", ret);
            return ret;
        }
    }
    LOG_INFO("Free host mem success. (va=0x%lx)\n", (uint64_t)master_ptr);

    if (is_local_dev) {
        ret = 0;
    } else {
        ret = halMemFree(agent_ptr);
        if (ret != 0) {
            LOG_ERR("halMemFree dev failed. (ret=%d)\n", ret);
            return ret;
        }
    }
    LOG_INFO("Free dev mem success. (devid=%u; va=0x%lx)\n", devid, (uint64_t)agent_ptr);

    return 0;
}

struct svm_mem_multi_map_para {
      uint64_t map_size;
      uint64_t va_index;
      uint64_t pa_index;
      uint64_t va_offset;
};

static int svm_mem_multi_map_para_create(uint32_t side, uint32_t devid, uint32_t pg_type, uint32_t mem_type,
    uint64_t va_size[], uint32_t va_cnt, uint64_t pa_size[], uint32_t pa_cnt,
    struct svm_mem_multi_map_para multi_map_para[], uint32_t map_cnt,
    uint64_t **va, drv_mem_handle_t ***handle)
{
    drv_mem_handle_t **tmp_handle = NULL;
    uint64_t *tmp_va = NULL;
    uint32_t i, j;
    int ret;

    tmp_va = (uint64_t *)malloc(sizeof(uint64_t) * va_cnt);
    if (tmp_va == NULL) {
        return -1;
    }

    tmp_handle = (drv_mem_handle_t **)malloc(sizeof(drv_mem_handle_t *) * pa_cnt);
    if (tmp_handle == NULL) {
        free(tmp_va);
        return -1;
    }

    for (i = 0; i < va_cnt; i++) {
        ret = halMemAddressReserve((void **)&tmp_va[i], va_size[i], 0, NULL, pg_type);
        if (ret != 0) {
            goto free_reserve_addr;
        }
        LOG_INFO("halMemAddressReserve success.\n");
    }

    for (i = 0; i < pa_cnt; i++) {
        struct drv_mem_prop prop = {
            .side = side,
            .devid = devid,
            .pg_type = pg_type,
            .mem_type = mem_type
        };
        ret = halMemCreate(&tmp_handle[i], pa_size[i], &prop, 0);
        if (ret != 0) {
            LOG_ERR("Mem create failed. (ret=%d; i=%u; size=%lu)\n", ret, i, pa_size[i]);
            goto mem_release;
        }
        LOG_INFO("halMemCreate success.\n");
    }

    for (i = 0; i < map_cnt; i++) {
        uint64_t map_va = tmp_va[multi_map_para[i].va_index] + multi_map_para[i].va_offset;
        drv_mem_handle_t *map_handle = tmp_handle[multi_map_para[i].pa_index];
        uint64_t map_size = multi_map_para[i].map_size;

        ret = halMemMap((void *)map_va, map_size, 0, map_handle, 0);
        if (ret != 0) {
            LOG_ERR("Mem map failed. (ret=%d; i=%u; va=0x%lx; size=%lu)\n", ret, i, map_va, map_size);
            goto mem_unmap;
        }
        LOG_INFO("halMemMap success.\n");
    }

    *va = tmp_va;
    *handle = tmp_handle;
    return 0;

mem_unmap:
    for (j = 0; j < i; j++) {
        uint64_t map_va = tmp_va[multi_map_para[j].va_index] + multi_map_para[j].va_offset;
        (void)halMemUnmap((void *)map_va);
        LOG_INFO("halMemUnmap success.\n");
    }
    i = pa_cnt;
mem_release:
    for (j = 0; j < i; j++) {
        (void)halMemRelease(tmp_handle[j]);
        LOG_INFO("halMemRelease success.\n");
    }
    i = va_cnt;
free_reserve_addr:
    for (j = 0; j < i; j++) {
        (void)halMemAddressFree((void *)tmp_va[j]);
        LOG_INFO("halMemAddressFree success.\n");
    }
    free(tmp_va);
    free(tmp_handle);
    return ret;
}

static int svm_reserve_addr_memcpy_test(uint32_t devid, uint64_t reserve_addr, uint64_t size)
{
    uint64_t host_va;
    int ret;

    ret = halMemAlloc((void *)&host_va, size, MEM_HOST);
    if (ret != 0) {
        LOG_ERR("halMemAlloc host va failed. (ret=%d; size=%lu)\n", ret, size);
        return ret;
    }
    LOG_INFO("Sync cpy info. (dev_reserve_addr=0x%lx; host_addr=0x%lx; size=%lu)\n", reserve_addr, host_va, size);

    ret = st_write_addr_by_int(host_va, size, 2);
    if (ret != 0) {
        return ret;
    }

    ret = drvMemcpy((DVdeviceptr)reserve_addr, size, (DVdeviceptr)host_va, size);
    if (ret != 0) {
        LOG_ERR("drvMemcpy failed. (ret=%d; size=%lu; src=0x%lx; dst=0x%lx)\n",
            ret, size, host_va, reserve_addr);
        return ret;
    }

    drvMemsetD8(host_va, size, 0, size);

    ret = drvMemcpy((DVdeviceptr)host_va, size, (DVdeviceptr)reserve_addr, size);
    if (ret != 0) {
        LOG_ERR("drvMemcpy failed. (ret=%d; size=%lu; src=0x%lx; dst=0x%lx)\n",
            ret, size, reserve_addr, host_va);
        return ret;
    }

    ret = st_verify_addr_by_int(host_va, size, 2);
    if (ret != 0) {
        return ret;
    }

    return 0;
}

static int svm_mem_multi_map_para_destroy(uint64_t *va, uint32_t va_cnt,
    drv_mem_handle_t **handle, uint32_t pa_cnt, uint32_t map_cnt, bool is_test_res_recycle)
{
    uint32_t i;
    int ret;

    if (is_test_res_recycle) {
        for (i = 0; i < pa_cnt; i++) {
            /* or asan will report mem leak */
            free(handle[i]);
        }
        free(va);
        free(handle);
        return 0;
    }

    for (i = 0; i < map_cnt; i++) {
        ret = halMemUnmap((void *)va[i]);
        if (ret != 0) {
            LOG_ERR("Mem unmap failed. (ret=%d; i=%u; va=0x%lx)\n", ret, i, va[i]);
            return ret;
        }
        LOG_INFO("halMemUnmap success.\n");
    }

    for (i = 0; i < pa_cnt; i++) {
        ret = halMemRelease(handle[i]);
        if (ret != 0) {
            LOG_ERR("Mem release failed. (ret=%d; i=%u)\n", ret, i);
            return ret;
        }
        LOG_INFO("halMemRelease success.\n");
    }

    for (i = 0; i < va_cnt; i++) {
        ret = halMemAddressFree((void *)va[i]);
        if (ret != 0) {
            LOG_ERR("Reserve addr free failed. (ret=%d; i=%u)\n", ret, i);
            return ret;
        }
        LOG_INFO("halMemAddressFree success.\n");
    }

    free(va);
    free(handle);
    return 0;
}

static int svm_mem_single_map_test(uint32_t side, uint32_t devid, uint32_t pg_type,
    uint32_t mem_type, uint64_t size, bool is_test_res_recycle)
{
    uint64_t va_size[1] = {
        [0] = 2097152ULL * 1024ULL * 3ULL
    };
    uint64_t pa_size[1] = {
        [0] = 2097152ULL * 1ULL
    };
    struct svm_mem_multi_map_para multi_map_para[1] = {
        /* Map multiple segments in one reserve addr to different handle. */
        [0] = {.map_size = 2097152ULL * 1ULL, .va_index = 0, .pa_index = 0, .va_offset = 0}
    };
    drv_mem_handle_t **handle = NULL;
    uint64_t va_cnt, pa_cnt, map_cnt;
    uint64_t *va = NULL;
    int ret;

    va_cnt = 1;
    pa_cnt = 1;
    map_cnt = 1;
    ret = svm_mem_multi_map_para_create(side, devid, pg_type, mem_type,
        va_size, va_cnt, pa_size, pa_cnt, multi_map_para, map_cnt, &va, &handle);
    if (ret != 0) {
        LOG_ERR("Mulit mem map para create failed. (ret=%d)\n", ret);
        return ret;
    }

    ret = svm_reserve_addr_memcpy_test(devid, va[0], size);
    if (ret != 0) {
        LOG_ERR("Svm_reserve_addr_memcpy_test failed. (ret=%d)\n", ret);
        return ret;
    }

    ret = svm_mem_multi_map_para_destroy(va, va_cnt, handle, pa_cnt, map_cnt, is_test_res_recycle);
    if (ret != 0) {
        LOG_ERR("Mulit mem map para destroy failed. (ret=%d)\n", ret);
        return ret;
    }

    return 0;
}

int st_svm_mem_map_normal_test_001(void *hdc_session, uint32_t devid, uint64_t arg[10])
{
    uint64_t size = 4096;
    uint64_t side = MEM_DEV_SIDE; 
    uint64_t pg_type = MEM_HUGE_PAGE_TYPE;
    uint64_t mem_type = MEM_HBM_TYPE; 
    uint64_t cnt = 1;
    bool is_test_res_recycle = false;
    uint64_t i;
    int ret;

    UNUSED(hdc_session);

    LOG_INFO("Mem map info. (size=%lu; side=%s; devid=%u; pg_type=%s; mem_type=%s)\n",
         size, get_side_str(side), devid, get_pg_type_str(pg_type), get_mem_type_str(mem_type));

    for (i = 0; i < cnt; i++) {
        ret = svm_mem_single_map_test(side, devid, pg_type, mem_type, size, is_test_res_recycle);
        if (ret != 0) {
            LOG_ERR("Single mem map failed. (ret=%d)\n", ret);
            return ret;
        }
    }

    return 0;
}

int hlt_svm_develop_st(int argc, char **arg)
{
    HDC_SESSION session = NULL;
    int i, k;
    uint64_t arg_list[10] = {0};
    int ret;
    uint32_t devid = 0;

    ret = st_svm_open_cp(1, devid, &session);
    if (ret) {
        LOG_ERR("open_failed.\n");
        return ret;
    }

    ret = st_svm_mem_map_normal_test_001(&session, devid, arg_list);
    if (ret) {
        LOG_ERR("st_svm_mem_map_normal_test_001 failed.\n");
        return ret;
    }

    ret = st_svm_sva_copy_normal_test_004(&session, devid, arg_list);
    if (ret) {
        LOG_ERR("st_svm_sva_copy_normal_test_004 failed.\n");
        return ret;
    }

    st_svm_close_cp(1, devid);

exit_st:
    if (ret == 0) {
        LOG_INFO("testcase success.\n");
    } else {
        LOG_ERR("testcase failed.\n");
    }

    return 0;
}

int main(int argc, char **argv)
{
    int ret = 0;
    int choice = 0;

    LOG_INFO("In main.\n");

    ret = hlt_svm_develop_st(argc, argv);

    return ret;
}