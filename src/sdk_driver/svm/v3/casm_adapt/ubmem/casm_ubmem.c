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
#include "pbl_feature_loader.h"

#include "svm_kern_log.h"
#include "framework_task.h"
#include "casm_kernel.h"
#include "svm_smp.h"
#include "ubmem_client.h"
#include "casm_ubmem.h"

static int casm_ubmem_add_src(u32 udevid, struct svm_global_va *src_va, struct casm_src_ex *src_ex)
{
    int ret;

    /* ub map src va to ubmmu with a spectial tid, so here we should pin src va */
    ret = svm_smp_pin_mem(udevid, ka_task_get_current_tgid(), src_va->va, src_va->size);
    if (ret != 0) {
        svm_err("Smm pin failed. (udevid=%u; va=0x%llx; size=%llu)\n", udevid, src_va->va, src_va->size);
        return ret;
    }

    ret = ubmem_map_client(udevid, src_va, &src_ex->updated_va);
    if ((ret != 0) || (src_ex->updated_va == 0)) { /* no ubmm map */
        svm_smp_unpin_mem(udevid, ka_task_get_current_tgid(), src_va->va, src_va->size);
        if (ret != 0) {
            svm_err("Ubmm map failed. (udevid=%u; va=0x%llx; size=%llu)\n", udevid, src_va->va, src_va->size);
        }
    }

    return ret;
}

static void casm_ubmem_del_src(u32 udevid, int tgid, struct svm_global_va *src_va, struct casm_src_ex *src_ex)
{
    if (src_ex->updated_va != 0) {
        /* Device task recycle will call ubmem unmap */
        if (!svm_task_is_exiting(udevid, tgid)) {
            int ret = ubmem_unmap_client(udevid, src_va);
            if (ret != 0) {
                svm_err("Ubmm unmap failed. (udevid=%u; va=0x%llx; size=%llu)\n", udevid, src_va->va, src_va->size);
            }
        }

        src_ex->updated_va = 0;
        svm_smp_unpin_mem(udevid, tgid, src_va->va, src_va->size);
    }
}

static const struct svm_casm_src_ops casm_ubmem_ops = {
    .add_src = casm_ubmem_add_src,
    .del_src = casm_ubmem_del_src
};

int casm_ubmem_init(void)
{
    svm_casm_register_src_ops(&casm_ubmem_ops);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(casm_ubmem_init, FEATURE_LOADER_STAGE_6);

