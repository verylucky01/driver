/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <securec.h>

#include "ascend_inpackage_hal.h"
#include "ascend_hal_define.h"

#include "mms.h"

static int svm_mem_stats_get_value(u32 devid, u32 module_id, u64 *alloc_size, u64 *peak_size)
{
    u64 mms_peak_size = 0;
    u64 mms_cur_size = 0;
    u32 mms_type;

    for (mms_type = 0; mms_type < MMS_TYPE_MAX; mms_type++) {
        struct mms_type_stats stats = {0};
        int ret;
        ret = svm_mms_get(devid, module_id, mms_type, &stats);
        if (ret != 0) {
            return ret;
        }
        if (stats.alloced_peak_size != 0) {
            mms_peak_size += stats.alloced_peak_size;
            mms_cur_size += stats.alloced_size;
        }
    }

    *alloc_size = mms_cur_size;
    *peak_size = mms_peak_size;
    return 0;
}

static void svm_mem_module_usage_info_pack(u32 module_id, u64 cur_size, u64 peak_size, struct mem_module_usage *usage_info)
{
    SVM_DECLARE_MODULE_NAME(svm_module_name);
    usage_info->cur_mem_size = cur_size;
    usage_info->mem_peak_size = peak_size;
    (void)strcpy_s(usage_info->name, sizeof(usage_info->name), SVM_GET_MODULE_NAME(svm_module_name, module_id));
}

static u32 svm_mem_usage_find_pos(struct mem_module_usage new_info, struct mem_module_usage *mem_info, size_t max_num)
{
    u32 i;

    for (i = 0; i < max_num; i++) {
        if ((mem_info[i].cur_mem_size == 0) || (new_info.cur_mem_size > mem_info[i].cur_mem_size)) {
            break;
        }
    }

    return i;
}

static void svm_mem_usage_insert(struct mem_module_usage new_info, struct mem_module_usage *mem_info, u32 pos, u32 max_num)
{
    u32 i;

    for (i = max_num - 1; i > pos; i--) {
        mem_info[i] = mem_info[i - 1];
    }
    mem_info[pos] = new_info;
    return;
}

static void svm_mem_usage_sort_insert(struct mem_module_usage new_info, struct mem_module_usage *mem_info, u32 cur_num, size_t max_num)
{
    u32 pos;

    pos = svm_mem_usage_find_pos(new_info, mem_info, cur_num);
    if (pos == max_num) {
        return;
    }
    
    svm_mem_usage_insert(new_info, mem_info, pos, ((cur_num < max_num) ? (cur_num + 1) : cur_num));
    return;
}

drvError_t halGetMemUsageInfo(uint32_t dev_id, struct mem_module_usage *mem_usage, size_t in_num, size_t *out_num)
{
    size_t cur_module_num = 0;
    u32 module_id;

    if (dev_id >= SVM_MAX_DEV_NUM || mem_usage == NULL || out_num == NULL || in_num == 0) {
        return DRV_ERROR_INVALID_VALUE;
    }
    (void)memset_s(mem_usage, sizeof(struct mem_module_usage) * in_num, 0, sizeof(struct mem_module_usage) * in_num);
    for (module_id = 0; module_id < MMS_MODULE_ID_MAX; module_id++) {
        struct mem_module_usage cur_usage_info;
        u64 cur_size, peak_size;
        int ret;

        ret = svm_mem_stats_get_value(dev_id, module_id, &cur_size, &peak_size);
        if ((ret != 0) || (peak_size == 0)) {
            continue;
        }
        svm_mem_module_usage_info_pack(module_id, cur_size, peak_size, &cur_usage_info);
        svm_mem_usage_sort_insert(cur_usage_info, mem_usage, (u32)cur_module_num, in_num);
        /* If the in_num provided by the user is less than the number of modules that occupy non-zero memory,
        need continue to get the memory usage information of in_num modules in descending order based on current memory usage. */
        if (cur_module_num < in_num) {
            cur_module_num++;
        }
    }
    *out_num = cur_module_num;
    return DRV_ERROR_NONE;
}