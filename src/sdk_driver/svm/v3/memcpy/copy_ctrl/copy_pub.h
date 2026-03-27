/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef COPY_PUB_H
#define COPY_PUB_H

#include "pbl_uda.h"

#include "svm_kern_log.h"
#include "svm_pub.h"

struct copy_va_info {
    u64 src_va;
    u32 src_udevid;
    int src_host_tgid; /* input defaults to current master tgid; user-side resolve may rewrite it to the owner tgid */

    u64 dst_va;
    u32 dst_udevid;
    int dst_host_tgid; /* input defaults to current master tgid; user-side resolve may rewrite it to the owner tgid */

    u64 size;
};

struct copy_2d_va_info {
    u64 src_va;
    u64 dst_va;
    u64 spitch;
    u64 dpitch;
    u64 width;
    u64 height;
    u32 src_udevid;
    u32 dst_udevid;
};

struct copy_batch_va_info {
    u64 *src_va;
    u64 *dst_va;
    u64 *size;
    u64 count;
    u32 src_udevid;
    u32 dst_udevid;
};

static inline enum svm_cpy_dir copy_dir_get_by_udevid(u32 src_udevid, u32 dst_udevid)
{
    if ((src_udevid == uda_get_host_id()) && (dst_udevid != uda_get_host_id())) {
        return SVM_H2D_CPY;
    } else if ((dst_udevid == uda_get_host_id()) && (src_udevid != uda_get_host_id())) {
        return SVM_D2H_CPY;
    } else if ((dst_udevid != uda_get_host_id()) && (src_udevid != uda_get_host_id())) {
        return SVM_D2D_CPY;
    } else {
        return SVM_MAX_CPY_DIR;
    }
}

static inline u32 copy_va_info_get_exec_udevid(const struct copy_va_info *info)
{
    return (copy_dir_get_by_udevid(info->src_udevid, info->dst_udevid) == SVM_H2D_CPY) ?
        info->dst_udevid : info->src_udevid;
}

static inline int copy_va_info_check(u32 udevid, struct copy_va_info *info)
{
    enum svm_cpy_dir dir = copy_dir_get_by_udevid(info->src_udevid, info->dst_udevid);
    if (dir == SVM_MAX_CPY_DIR) {
        svm_err("Invalid cpy dir. (src_udevid=%u; dst_udevid=%u)\n", info->src_udevid, info->dst_udevid);
        return -EINVAL;
    }

    if ((!uda_can_access_udevid(info->src_udevid) && (info->src_udevid != uda_get_host_id())) ||
        (!uda_can_access_udevid(info->dst_udevid) && (info->dst_udevid != uda_get_host_id()))) {
        svm_info("Can not access device. (src_udevid=%u; dst_udevid=%u)\n", info->src_udevid, info->dst_udevid);
        return -EPERM;
    }

    if ((dir == SVM_H2D_CPY) && (udevid != info->dst_udevid)) {
        svm_err("Invalid h2d addr. (dst_va=0x%llx; size=%llu)\n", info->dst_va, info->size);
        return -EINVAL;
    } else if ((dir == SVM_D2H_CPY) && (udevid != info->src_udevid)) {
        svm_err("Invalid d2h addr. (src_va=0x%llx; size=%llu)\n", info->src_va, info->size);
        return -EINVAL;
    } else if ((dir == SVM_D2D_CPY) && (udevid != info->src_udevid) && (udevid != info->dst_udevid)) {
        svm_err("Invalid d2d addr. (src_va=0x%llx; dst_va=0x%llx; size=%llu)\n",
            info->src_va, info->dst_va, info->size);
        return -EINVAL;
    }

    return 0;
}

#endif

