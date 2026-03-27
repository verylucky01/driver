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

#ifndef VA_QUERY_H
#define VA_QUERY_H

#include "pbl_uda.h"
#include "dpa_kernel_interface.h"

#include "svm_pub.h"
#include "svm_kern_log.h"
#include "svm_smp.h"
#include "ksvmm.h"

static inline int svm_query_va_udevid_by_smp(int master_tgid, u64 va, u64 size, u32 *udevid)
{
    u32 devid, tmp_udevid;
    int ret;

    for (devid = 0; devid < SVM_MAX_DEV_NUM; devid++) {
        ret = uda_devid_to_udevid_ex(devid, &tmp_udevid);
        if (ret != 0) {
            break; /* Devid is continuously starting from 0. */
        }

        /* Get devid from smp, for halMemAlloc va. */
        ret = svm_smp_check_mem_exists(tmp_udevid, master_tgid, va, size);
        if (ret == 0) {
            *udevid = tmp_udevid;
            return 0;
        }
    }

    return -EINVAL;
}

static inline int svm_query_va_udevid_by_ksvmm(int master_tgid, u64 va, u64 size, u32 *udevid)
{
    struct svm_global_va src_info;
    u64 start;
    u32 devid, tmp_udevid;
    int cp1_tgid, ret;

    for (devid = 0; devid < SVM_MAX_DEV_NUM; devid++) {
        ret = uda_devid_to_udevid_ex(devid, &tmp_udevid);
        if (ret != 0) {
            break; /* Devid is continuously starting from 0. */
        }

        /* Same (udevid, tgid) is vmm local map. */
        ret = ksvmm_get_seg(tmp_udevid, master_tgid, va, &start, &src_info);
        if (ret != 0) {
            continue;
        }

        if (src_info.udevid == tmp_udevid) {
            ret = hal_kernel_apm_query_slave_tgid_by_master(master_tgid, tmp_udevid, PROCESS_CP1, &cp1_tgid);
            if (ret != 0) {
                continue;
            }

            if (src_info.tgid == cp1_tgid) {
                *udevid = tmp_udevid;
                return 0;
            }
        }
    }

    return -EINVAL;
}

static inline int svm_query_va_udevid(int master_tgid, u64 va, u64 size, u32 *udevid)
{
    int ret;

    ret = svm_query_va_udevid_by_smp(master_tgid, va, size, udevid);
    if (ret != 0) {
        ret = svm_query_va_udevid_by_ksvmm(master_tgid, va, size, udevid);
    }

    return ret;
}

#endif

