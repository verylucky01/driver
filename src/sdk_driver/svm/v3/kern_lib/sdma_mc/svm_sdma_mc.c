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
#include "ka_base_pub.h"
#include "ka_memory_pub.h"
#include <linux/hisi_sdma.h>

#include "pbl_feature_loader.h"
#include "pbl_uda.h"
#include "dpa_kernel_interface.h"

#include "svm_kern_log.h"

#include "svm_mc.h"

#define SVM_SDMA_STREAM_ID              0x1FU
#define SVM_SDMA_MEMZERO_PER_MAX_SIZE   0x100000000ULL

/* Do not cause page fault, my occur mm_lock deadlock. */
static int sdma_clr_mem_by_uva(u32 udevid, int tgid, u64 va, u64 size)
{
    u64 offset, real_size;
    int ret, ssid;

    if (uda_is_phy_dev(udevid) == false) {
        return -EFAULT;
    }

    ret = apm_query_slave_ssid(udevid, tgid, &ssid);
    if (ret != 0) {
        return ret;
    }

    for (offset = 0; offset < size; offset += real_size) {
        real_size = ka_base_min_t(u64, size - offset, SVM_SDMA_MEMZERO_PER_MAX_SIZE);
        /* devid need set 0, because devid1 is slave SDMA device. */
        ret = sdma_kernel_memset(0, (unsigned long)(va + offset), real_size, ssid, SVM_SDMA_STREAM_ID, 0);
        if (ret != 0) {
            svm_warn("Sdma_kernel_memset invalid, will go to memset_s. (ret=%d; ssid=%d; va=0x%llx; size=%llu)\n",
                ret, ssid, va + offset, real_size);
            return ret;
        }
    }

    return 0;
}

int svm_sdma_mc_init(void)
{
    svm_register_mc_handle(sdma_clr_mem_by_uva);
    return 0;
}
#ifdef CFG_FEATURE_SURPORT_SDMA_MC
DECLAER_FEATURE_AUTO_INIT(svm_sdma_mc_init, FEATURE_LOADER_STAGE_0);
#endif
