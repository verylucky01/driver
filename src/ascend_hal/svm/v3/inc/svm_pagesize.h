/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVM_PAGESIZE_H
#define SVM_PAGESIZE_H

#include "svm_pub.h"
#include "svm_flag.h"
#include "svm_dbi.h"

static inline int svm_query_page_size_by_svm_flag(u32 devid, u64 svm_flag, u64 *page_size)
{
    if (svm_flag_attr_is_gpage(svm_flag)) {
        return svm_dbi_query_gpage_size(devid, page_size);
    } else if (svm_flag_attr_is_hpage(svm_flag)) {
        return svm_dbi_query_hpage_size(devid, page_size);
    } else {
        return svm_dbi_query_npage_size(devid, page_size);
    }
}

static inline bool svm_is_page_align(u32 devid, u64 svm_flag, u64 value)
{
    u64 page_size;

    if (svm_query_page_size_by_svm_flag(devid, svm_flag, &page_size) != 0) {
        return false;
    }

    return SVM_IS_ALIGNED(value, page_size);
}

static inline bool svm_is_va_page_align(u32 devid, u64 svm_flag, u64 va)
{
    return svm_is_page_align(devid, svm_flag, va);
}

static inline bool svm_is_npage_align(u32 devid, u64 value)
{
    return svm_is_page_align(devid, 0, value);
}

static inline bool svm_is_hpage_align(u32 devid, u64 value)
{
    return svm_is_page_align(devid, SVM_FLAG_ATTR_PA_HPAGE, value);
}

static inline bool svm_is_gpage_align(u32 devid, u64 value)
{
    return svm_is_page_align(devid, SVM_FLAG_ATTR_PA_GPAGE, value);
}

static inline bool svm_is_va_npage_align(u32 devid, u64 va)
{
    return svm_is_npage_align(devid, va);
}

static inline bool svm_is_va_hpage_align(u32 devid, u64 va)
{
    return svm_is_hpage_align(devid, va);
}

static inline bool svm_is_va_gpage_align(u32 devid, u64 va)
{
    return svm_is_gpage_align(devid, va);
}

static inline int svm_get_aligned_size(u32 devid, u64 svm_flag, u64 size, u64 *aligned_size)
{
    u64 page_size;
    int ret;

    ret = svm_query_page_size_by_svm_flag(devid, svm_flag, &page_size);
    if (ret != 0) {
        return ret;
    }

    *aligned_size = svm_align_up(size, page_size);
    return 0;
}

static inline int svm_get_npage_aligned_size(u32 devid, u64 size, u64 *aligned_size)
{
    return svm_get_aligned_size(devid, 0, size, aligned_size);
}

static inline int svm_get_hpage_aligned_size(u32 devid, u64 size, u64 *aligned_size)
{
    return svm_get_aligned_size(devid, SVM_FLAG_ATTR_PA_HPAGE, size, aligned_size);
}

static inline int svm_get_gpage_aligned_size(u32 devid, u64 size, u64 *aligned_size)
{
    return svm_get_aligned_size(devid, SVM_FLAG_ATTR_PA_GPAGE, size, aligned_size);
}

static inline u64 svm_calc_order_aligned_size(u64 size, u64 page_size)
{
    u64 aligned_size = svm_align_up(size, page_size);
    u64 page_num = aligned_size / page_size;
    u64 aligned_page_num = 1;

    while (aligned_page_num < page_num) {
        aligned_page_num <<= 1;
    }

    return (aligned_page_num * page_size);
}

static inline int svm_get_order_aligned_size(u32 devid, u64 svm_flag, u64 size, u64 *aligned_size)
{
    u64 page_size;
    int ret;

    ret = svm_query_page_size_by_svm_flag(devid, svm_flag, &page_size);
    if (ret != 0) {
        return ret;
    }

    *aligned_size = svm_calc_order_aligned_size(size, page_size);
    return 0;
}

/* size is page size, va is align to page size */
static inline bool svm_is_va_single_page_align(u32 devid, u64 svm_flag, u64 va, u64 size)
{
    u64 page_size;

    if (svm_query_page_size_by_svm_flag(devid, svm_flag, &page_size) != 0) {
        return false;
    }

    return (SVM_IS_ALIGNED(va, page_size) && (size == page_size));
}

#endif
