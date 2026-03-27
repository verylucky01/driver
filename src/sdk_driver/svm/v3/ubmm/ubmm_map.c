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
#include <linux/ummu_core.h>

#include "ka_pci_pub.h"
#include "ka_task_pub.h"
#include "ka_memory_pub.h"
#include "ka_common_pub.h"

#include "pbl/pbl_soc_res.h"

#include "svm_kern_log.h"
#include "ubmm_map.h"

static ka_device_t *ubmm_device = NULL;

int _ubmm_init_map(u32 udevid)
{
    ka_device_t *device = NULL;
    struct tdev_attr attr;
    u64 ub_tid;
    int ret;

    if (ubmm_device != NULL) {
        return 0;
    }

    ret = soc_resmng_dev_get_key_value(udevid, "UB_TID", &ub_tid);
    if (ret != 0) {
        return -EAGAIN;
    }

    tdev_attr_init(&attr);
    device = ummu_core_alloc_tdev(&attr, (uint32_t *)&ub_tid);
    if (device == NULL) {
        svm_err("ummu_core_alloc_tdev failed.\n");
        return -ENODEV;
    }

    ubmm_device = device;
    return 0;
}

int ubmm_init_map(u32 udevid)
{
    static KA_TASK_DEFINE_MUTEX(mutex);
    int ret;

    ka_task_mutex_lock(&mutex);
    ret = _ubmm_init_map(udevid);
    ka_task_mutex_unlock(&mutex);
    return ret;
}

void ubmm_uninit_map(u32 udevid)
{
    if (ubmm_device != NULL) {
        ummu_core_free_tdev(ubmm_device);
        ubmm_device = NULL;
    }
}

static ka_iommu_domain_t *ubmm_get_iommu_domain(void)
{
    if (ubmm_device == NULL) {
        return NULL;
    }

    return ka_mm_iommu_get_domain_for_dev(ubmm_device);
}

static int ubmm_map_check_align(u64 uba, u64 size, struct svm_pa_seg *pa_seg, u64 seg_num)
{
    u64 i;

    if ((SVM_IS_ALIGNED(uba, KA_MM_PAGE_SIZE) == false) || (SVM_IS_ALIGNED(size, KA_MM_PAGE_SIZE) == false)) {
        svm_err("Uba should be aligned by PAGE_SIZE.\n");
        return -EINVAL;
    }
#ifndef EMU_ST /* todo: emu_st support alloc hpage size */
    for (i = 0; i < seg_num; i++) {
        if ((SVM_IS_ALIGNED(pa_seg[i].pa, KA_MM_PAGE_SIZE) == false) || (SVM_IS_ALIGNED(pa_seg[i].size, KA_MM_PAGE_SIZE) == false)) {
            svm_err("Pa seg should be aligned by PAGE_SIZE.\n");
            return -EINVAL;
        }
    }
#endif
    return 0;
}

static int _ubmm_unmap(ka_iommu_domain_t *domain, u64 uba, u64 size)
{
    u64 unmapped_size;

    unmapped_size = ka_mm_iommu_unmap(domain, uba, size);
    return (unmapped_size != size) ? -EINVAL : 0;
}

static int _ubmm_map(ka_iommu_domain_t *domain, u64 uba, u64 size, struct svm_pa_seg *pa_seg, u64 seg_num)
{
    u64 i, map_size = 0;
    int ret;

    for (i = 0; i < seg_num; i++) {
        ret = ka_mm_iommu_map(domain, uba + map_size, pa_seg[i].pa, pa_seg[i].size,
            KA_IOMMU_READ | KA_IOMMU_WRITE, KA_GFP_KERNEL | __KA_GFP_NOWARN | __KA_GFP_ACCOUNT);
        if (ret != 0) {
            svm_err("Iommu_map failed. (ret=%d)\n", ret);
            (void)_ubmm_unmap(domain, uba, map_size);
            return ret;
        }
        map_size += pa_seg[i].size;
    }

    return 0;
}

int ubmm_map(u32 udevid, u64 uba, u64 size, struct svm_pa_seg *pa_seg, u64 seg_num)
{
    ka_iommu_domain_t *domain = NULL;
    int ret;

    ret = ubmm_map_check_align(uba, size, pa_seg, seg_num);
    if (ret != 0) {
        return ret;
    }

    domain = ubmm_get_iommu_domain();
    if (domain == NULL) {
        svm_err("Get iommu domain failed.\n");
        return -EINVAL;
    }

    return _ubmm_map(domain, uba, size, pa_seg, seg_num);
}

int ubmm_unmap(u32 udevid, u64 uba, u64 size)
{
    ka_iommu_domain_t *domain = NULL;

    domain = ubmm_get_iommu_domain();
    if (domain == NULL) {
        svm_err("Get iommu domain failed.\n");
        return -EINVAL;
    }

    return _ubmm_unmap(domain, uba, size);
}

