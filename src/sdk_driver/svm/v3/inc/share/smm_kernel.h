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

#ifndef SMM_KERNEL_H
#define SMM_KERNEL_H

#include "svm_addr_desc.h"
#include "svm_pub.h"

enum SMM_PA_LOCATION_TYPE {
    SMM_LOCAL_PA = 0,
    SMM_PEER_HOST_PA,
    SMM_PEER_DEVICE_PA,
};

struct smm_ops {
    int (*pa_get)(u32 udevid, struct svm_global_va *src_info, struct svm_pa_seg pa_seg[], u64 *seg_num); /* must */
    int (*pa_put)(u32 udevid, struct svm_global_va *src_info, struct svm_pa_seg pa_seg[], u64 seg_num); /* must */
    int (*alloc_va)(u32 udevid, int tgid, u64 size, u64 *va); /* not must */
    int (*free_va)(u32 udevid, int tgid, u64 va, u64 size); /* not must */
    int (*remap)(u32 udevid, int tgid, u64 va, u64 pa, u64 size, u64 flag); /* not must */
    int (*unmap)(u32 udevid, int tgid, u64 va, u64 size); /* not must */
    enum SMM_PA_LOCATION_TYPE pa_location;
};

int svm_smm_register_ops(u32 udevid, u32 src_udevid, const struct smm_ops *dev_ops);
int svm_smm_get_ops(u32 udevid, u32 src_udevid, struct smm_ops **dev_ops);

int svm_smm_register_cs_ops(u32 udevid, const struct smm_ops *cs_ops);

/* page table externed handle */
enum smm_external_op_type {
    SMM_EXTERNAL_POST_REMAP,
    SMM_EXTERNAL_POST_UNMAP,
    SMM_EXTERNAL_OP_MAX,
};

void svm_smm_register_external_handle(
    void (*handle)(enum smm_external_op_type op_type, u32 udevid, int tgid, u64 va, u64 size));

#endif

