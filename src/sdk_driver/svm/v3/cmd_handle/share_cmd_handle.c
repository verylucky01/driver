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
#include "ka_ioctl_pub.h"

#include "pbl_uda.h"

#include "svm_ioctl_ex.h"
#include "framework_cmd.h"
#include "svm_kern_log.h"
#include "smm_ioctl.h"
#include "svm_smp.h"
#include "ksvmm.h"
#include "smm_flag.h"
#include "casm_kernel.h"
#include "share_cmd_handle.h"

static int cmd_smm_mmap_pre_handle(u32 udevid, u32 cmd, void *para)
{
    struct svm_smm_map_para *map_para = (struct svm_smm_map_para *)para;
    struct svm_global_va *src_info = &map_para->src_info;

    if (udevid != uda_get_host_id()) {
        svm_err("Invalid para. (udevid=%u; cmd=%u)\n", udevid, cmd);
        return -EINVAL;
    }

    /* try to update casm src info */
    if (svm_smm_get_src_valid_flag((u32)map_para->flag) == 0) {
        int ret = svm_casm_get_src_info(udevid, map_para->dst_va, map_para->dst_size, src_info);
        if (ret != 0) {
            svm_err("Casm get src info failed in cmd pre handle. (udevid=%u; dst_va=0x%llx; dst_size=%llu)\n",
                udevid, map_para->dst_va, map_para->dst_size);
        }
        return ret;
    } else {
        return svm_smp_pin_mem(src_info->udevid, ka_task_get_current_tgid(), src_info->va, src_info->size);
    }
}

static void cmd_smm_mmap_pre_cancle_handle(u32 udevid, u32 cmd, void *para)
{
    struct svm_smm_map_para *map_para = (struct svm_smm_map_para *)para;
    struct svm_global_va *src_info = &map_para->src_info;

    if (udevid != uda_get_host_id()) {
        svm_err("Invalid para. (udevid=%u; cmd=%u)\n", udevid, cmd);
        return;
    }

    if (svm_smm_get_src_valid_flag((u32)map_para->flag) != 0) {
        (void)svm_smp_unpin_mem(src_info->udevid, ka_task_get_current_tgid(), src_info->va, src_info->size);
    }
}

static int cmd_smm_mmap_post_handle(u32 udevid, u32 cmd, void *para)
{
    struct svm_smm_map_para *map_para = (struct svm_smm_map_para *)para;
    int ret;

    if (udevid != uda_get_host_id()) {
        svm_err("Invalid para. (udevid=%u; cmd=%u)\n", udevid, cmd);
        return -EINVAL;
    }

    ret = ksvmm_add_seg(udevid, ka_task_get_current_tgid(), map_para->dst_va, &map_para->src_info);
    if (ret != 0) {
        svm_err("Ksvmm add seg failed in smm um post handle. (udevid=%u; dst_va=0x%llx; dst_size=%llu)\n",
            udevid, map_para->dst_va, map_para->dst_size);
    }

    return ret;
}

static int cmd_smm_munmap_pre_handle(u32 udevid, u32 cmd, void *para)
{
    struct svm_smm_unmap_para *unmap_para = (struct svm_smm_unmap_para *)para;
    struct svm_global_va *src_info = &unmap_para->src_info;
    int ret;

    if (udevid != uda_get_host_id()) {
        svm_err("Invalid para. (udevid=%u; cmd=%u)\n", udevid, cmd);
        return -EINVAL;
    }

    /* try to update casm src info */
    if (svm_smm_get_src_valid_flag((u32)unmap_para->flag) == 0) {
        ret = svm_casm_get_src_info(udevid, unmap_para->dst_va, unmap_para->dst_size, src_info);
        if (ret != 0) {
            svm_err("Casm get src info failed in cmd pre handle. (udevid=%u; dst_va=0x%llx; dst_size=%llu)\n",
                udevid, unmap_para->dst_va, unmap_para->dst_size);
            return ret;
        }
    }

    ret = ksvmm_del_seg(udevid, ka_task_get_current_tgid(), unmap_para->dst_va);
    if (ret != 0) {
        svm_err("Ksvmm add seg failed in smm um post handle. (udevid=%u; dst_va=0x%llx)\n",
            udevid, unmap_para->dst_va);
    }

    return ret;
}

static int cmd_smm_munmap_post_handle(u32 udevid, u32 cmd, void *para)
{
    struct svm_smm_unmap_para *unmap_para = (struct svm_smm_unmap_para *)para;
    struct svm_global_va *src_info = &unmap_para->src_info;

    if (udevid != uda_get_host_id()) {
        svm_err("Invalid para. (udevid=%u; cmd=%u)\n", udevid, cmd);
        return -EINVAL;
    }

    if (svm_smm_get_src_valid_flag((u32)unmap_para->flag) == 0) {
        return 0;
    } else {
        return svm_smp_unpin_mem(src_info->udevid, ka_task_get_current_tgid(), src_info->va, src_info->size);
    }
}

/* for host mem mmap device */
void share_cmd_handle_init(void)
{
    svm_register_ioctl_cmd_pre_handle(_IOC_NR(SVM_SMM_MAP), cmd_smm_mmap_pre_handle);
    svm_register_ioctl_cmd_pre_cancle_handle(_IOC_NR(SVM_SMM_MAP), cmd_smm_mmap_pre_cancle_handle);
    svm_register_ioctl_cmd_post_handle(_IOC_NR(SVM_SMM_MAP), cmd_smm_mmap_post_handle);

    svm_register_ioctl_cmd_pre_handle(_IOC_NR(SVM_SMM_UNMAP), cmd_smm_munmap_pre_handle);
    svm_register_ioctl_cmd_post_handle(_IOC_NR(SVM_SMM_UNMAP), cmd_smm_munmap_post_handle);
}

