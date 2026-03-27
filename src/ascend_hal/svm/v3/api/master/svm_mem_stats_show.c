/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <sys/mman.h>
#include <securec.h>

#include "ascend_inpackage_hal.h"
#include "ascend_hal_define.h"
#include "dpa/dpa_apm.h"

#include "svm_log.h"
#include "svm_sys_cmd.h"
#include "mms.h"
#include "cache_malloc.h"
#include "svm_dbi.h"

SVM_DECLARE_MODULE_NAME(svm_module_name);

static void module_mem_stats_show(u32 devid, u32 mms_type, const char *type_str)
{
    u32 module_id;
    int ret;

    for (module_id = 0; module_id < MMS_MODULE_ID_MAX; module_id++) {
        struct mms_type_stats stats = {0};
        ret = svm_mms_get(devid, module_id, mms_type, &stats);
        if ((ret == 0) && (stats.alloced_peak_size != 0)) {
#ifndef EMU_ST /* Too much, not print. */
            svm_run_info("%s Mem stats (Bytes). (module_name=%s; module_id=%u; "
                "current_alloced_size=%llu; allocated_peak_size=%llu; alloc_cnt=%llu; free_cnt=%llu)\n",
                type_str, SVM_GET_MODULE_NAME(svm_module_name, module_id), module_id,
                stats.alloced_size, stats.alloced_peak_size, stats.alloc_cnt, stats.free_cnt);
#endif
        }
    }
}

static inline bool mms_type_is_hpage(u32 type)
{
    return ((type == MMS_TYPE_HPAGE) || (type == MMS_TYPE_P2P_HPAGE));
}

static inline bool mms_type_is_p2p(u32 type)
{
    return ((type == MMS_TYPE_P2P_NPAGE) || (type == MMS_TYPE_P2P_HPAGE));
}

static inline u32 mms_type_to_cache_flag(u64 mms_type)
{
    u32 cache_flag;

    cache_flag = mms_type_is_hpage((u32)mms_type) ? (u32)SVM_CACHE_MALLOC_FLAG_PA_HPAGE : 0;
    cache_flag |= mms_type_is_p2p((u32)mms_type) ? (u32)SVM_CACHE_MALLOC_FLAG_PA_P2P : 0;
    return cache_flag;
}

static void cache_mem_stats_show(u32 devid, u32 mms_type, const char *type_str)
{
    u64 cached_size;

    cached_size = svm_cache_get_stats_idle_size(devid, mms_type_to_cache_flag(mms_type));
    if (cached_size != 0) {
#ifndef EMU_ST /* Too much, not print. */
        svm_run_info("%s Cached_size:%lluBytes\n", type_str, cached_size);
#endif
    }
}

#define TYPE_STR_MAX_LEN  50
static void mem_stats_show_host_svm_mem(u32 devid)
{
    char type_str[TYPE_STR_MAX_LEN] = "MEM_HOST";

    module_mem_stats_show(devid, MMS_TYPE_NPAGE, type_str);
    cache_mem_stats_show(devid, MMS_TYPE_NPAGE, type_str);
}

static void mem_stats_show_get_type_str(u32 devid, u32 flag, char *str, u32 str_len)
{
    static const char *type_str[MMS_TYPE_MAX] = {
        "MEM_DEV_SMALL_HBM", "MEM_DEV_HUGE_HBM", "MEM_DEV_SMALL_P2P_HBM", "MEM_DEV_HUGE_P2P_HBM"};

    (void)sprintf_s(str, str_len, "%s dev%d", type_str[flag], devid);
}

static void mem_stats_show_dev_svm_mem(u32 devid)
{
    u32 mms_type;

    for (mms_type = 0; mms_type < MMS_TYPE_MAX; mms_type++) {
        char type_str[TYPE_STR_MAX_LEN] = {0};

        mem_stats_show_get_type_str(devid, mms_type, type_str, TYPE_STR_MAX_LEN);
        module_mem_stats_show(devid, mms_type, type_str);
        cache_mem_stats_show(devid, mms_type, type_str);
    }
}

static void _mem_stats_show_dev_proc_mem(u32 devid, processType_t processType,
    processMemType_t memType, u32 module_id)
{
    u64 size = 0;
    int ret;

    ret = halQuerySlaveProcMeminfo(getpid(), devid, processType, memType, &size);
    if (ret != 0) {
        return;
    }

    if (size != 0) {
#ifndef EMU_ST /* Too much, not print. */
        svm_run_info("DEV_PROC_MEM dev%u Mem stats (Bytes). (module_name=%s; module_id=%u; total_size=%lu)\n",
            devid, SVM_GET_MODULE_NAME(svm_module_name, module_id), module_id, size);
#endif
    }
}

static void mem_stats_show_dev_proc_mem(u32 devid)
{
    _mem_stats_show_dev_proc_mem(devid, PROCESS_CP1, PROC_MEM_TYPE_VMRSS, AICPU_SCHE_MODULE_ID);
    _mem_stats_show_dev_proc_mem(devid, PROCESS_CP2, PROC_MEM_TYPE_VMRSS, CUSTOM_SCHE_MODULE_ID);
    _mem_stats_show_dev_proc_mem(devid, PROCESS_HCCP, PROC_MEM_TYPE_VMRSS, HCCP_SCHE_MODULE_ID);
    _mem_stats_show_dev_proc_mem(devid, PROCESS_CP1, PROC_MEM_TYPE_SP, MBUFF_MODULE_ID);
}

void svm_mem_stats_show(u32 devid)
{
    if (devid == svm_get_host_devid()) {
        mem_stats_show_host_svm_mem(devid);
    } else {
        mem_stats_show_dev_svm_mem(devid);
        mem_stats_show_dev_proc_mem(devid);
    }
}

int mem_stats_show_svm_mem(u32 devid)
{
    u32 host_devid = svm_get_host_devid();

    if (devid == host_devid) {
        return 0;
    }

    mem_stats_show_host_svm_mem(host_devid);
    mem_stats_show_dev_svm_mem(devid);
    return 0;
}

int svm_mem_stats_get_dev_svm_mem(uint32_t devid, uint32_t module_id, uint64_t *alloced_size)
{
    u64 mms_alloced_size = 0;
    u32 mms_type;
    int ret;

    for (mms_type = 0; mms_type < MMS_TYPE_MAX; mms_type++) {
        struct mms_type_stats stats = {0};
        ret = svm_mms_get(devid, module_id, mms_type, &stats);
        if (ret != 0) {
            return ret;
        }
        mms_alloced_size += stats.alloced_size;
    }
    *alloced_size = (uint64_t)mms_alloced_size;
    return 0;
}

__attribute__((constructor)) void svm_mem_stats_show_init(void)
{
    svm_register_ioctl_dev_uninit_pre_handle(mem_stats_show_svm_mem);
    mem_prof_register_get_module_stats_func(svm_mem_stats_get_dev_svm_mem);
}

