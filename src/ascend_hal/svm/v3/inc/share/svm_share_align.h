/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVM_SHARE_ALIGN_H
#define SVM_SHARE_ALIGN_H

#include "ascend_hal_error.h"

#include "svm_pub.h"
#include "svm_pagesize.h"
#include "svm_dbi.h"

static inline int svm_share_dst_align_use_gpage_size(u64 src_va, u64 size, u32 dst_devid, u64 *align)
{
    u64 gpage_size;
    int ret;

    ret = svm_dbi_query_gpage_size(dst_devid, &gpage_size);
    if ((ret == DRV_ERROR_NONE) && SVM_IS_ALIGNED(size, gpage_size) && SVM_IS_ALIGNED(src_va, gpage_size)) {
        *align = gpage_size;
        return DRV_ERROR_NONE;
    }
    return DRV_ERROR_INVALID_VALUE;
}

static inline int svm_share_dst_align_use_hpage_size(u64 src_va, u64 size, u32 dst_devid, u64 *align)
{
    u64 hpage_size;
    int ret;

    ret = svm_dbi_query_hpage_size(dst_devid, &hpage_size);
    if ((ret == DRV_ERROR_NONE) && SVM_IS_ALIGNED(size, hpage_size) && SVM_IS_ALIGNED(src_va, hpage_size)) {
        *align = hpage_size;
        return DRV_ERROR_NONE;
    }
    return DRV_ERROR_INVALID_VALUE;
}

static inline int svm_share_dst_align_use_npage_size(u64 src_va, u64 size, u32 dst_devid, u64 *align)
{
    u64 npage_size;
    int ret;

    ret = svm_dbi_query_npage_size(dst_devid, &npage_size);
    if ((ret == DRV_ERROR_NONE) && SVM_IS_ALIGNED(size, npage_size) && SVM_IS_ALIGNED(src_va, npage_size)) {
        *align = npage_size;
        return DRV_ERROR_NONE;
    }
    return DRV_ERROR_INVALID_VALUE;
}

static inline int svm_share_get_dst_align(u64 src_va, u64 size, u32 dst_devid, u64 *align)
{
    int ret;

    /* For performance, try largest pg_size firstly. */
    ret = svm_share_dst_align_use_gpage_size(src_va, size, dst_devid, align);
    if (ret == DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_share_dst_align_use_hpage_size(src_va, size, dst_devid, align);
    if (ret == DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_share_dst_align_use_npage_size(src_va, size, dst_devid, align);
    if (ret == DRV_ERROR_NONE) {
        return ret;
    }

    return DRV_ERROR_INVALID_VALUE;
}

static inline int svm_share_check_dst_va_align(u64 src_va, u64 size, u32 dst_devid, u64 dst_va)
{
    u64 align;
    int ret;

    ret = svm_share_get_dst_align(src_va, size, dst_devid, &align);
    return ((ret == DRV_ERROR_NONE) && SVM_IS_ALIGNED(dst_va, align)) ? DRV_ERROR_NONE : DRV_ERROR_INVALID_VALUE;
}

static inline int svm_share_get_gpage_src_aligned_size(u32 src_devid, u64 src_va, u64 size,
    u64 *aligned_size)
{
    if (svm_is_va_gpage_align(src_devid, src_va)) {
        return svm_get_hpage_aligned_size(src_devid, size, aligned_size); /* gpage src aligned by hpage_size */
    } else if (svm_is_va_hpage_align(src_devid, src_va)) {
        return svm_get_hpage_aligned_size(src_devid, size, aligned_size);
    } else {
        return DRV_ERROR_INVALID_VALUE;
    }
}

static inline int svm_share_get_hpage_src_aligned_size(u32 src_devid, u64 src_va, u64 size,
    u64 *aligned_size)
{
    if (svm_is_va_hpage_align(src_devid, src_va)) {
        return svm_get_hpage_aligned_size(src_devid, size, aligned_size);
    } else {
        return DRV_ERROR_INVALID_VALUE;
    }
}

static inline int svm_share_get_npage_src_aligned_size(u32 src_devid, u64 src_va, u64 size,
    u64 *aligned_size)
{
    if (svm_is_va_npage_align(src_devid, src_va)) {
        return svm_get_npage_aligned_size(src_devid, size, aligned_size);
    } else {
        return DRV_ERROR_INVALID_VALUE;
    }
}

static inline int svm_share_get_src_aligned_size(u32 src_devid, u64 svm_flag, u64 src_va, u64 size,
    u64 *aligned_size)
{
    if (svm_flag_attr_is_gpage(svm_flag)) {
        return svm_share_get_gpage_src_aligned_size(src_devid, src_va, size, aligned_size);
    } else if (svm_flag_attr_is_hpage(svm_flag)) {
        return svm_share_get_hpage_src_aligned_size(src_devid, src_va, size, aligned_size);
    } else {
        return svm_share_get_npage_src_aligned_size(src_devid, src_va, size, aligned_size);
    }
}

#endif
