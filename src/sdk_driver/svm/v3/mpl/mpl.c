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
#include "ka_base_pub.h"
#include "ka_task_pub.h"
#include "ka_memory_pub.h"
#include "ka_common_pub.h"
#include "ka_compiler_pub.h"
#include "ka_sched_pub.h"

#include "kernel_version_adapt.h"
#include "pbl_feature_loader.h"
#include "dpa_kernel_interface.h"
#include "pbl_uda.h"
#include "pbl_chip_config.h"

#include "framework_cmd.h"
#include "framework_vma.h"
#include "svm_pgtable.h"
#include "svm_gfp.h"
#include "svm_mc.h"
#include "svm_ioctl_ex.h"
#include "svm_kern_log.h"
#include "svm_slab.h"
#include "mpl_flag.h"
#include "mpl_ioctl.h"
#include "pmm.h"
#include "mpl.h"

static int mpl_permission_check(u32 udevid, int tgid)
{
    processType_t proc_type;
    int ret;

    if (udevid == uda_get_host_id()) {
        return 0;
    }

    ret = apm_query_proc_type_by_slave(tgid, &proc_type);
    if (ret != 0) {
        svm_err("Not apm bind proc. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EACCES;
    }

    if (proc_type != PROCESS_CP1) {
        svm_err("No permission to populate. (udevid=%u; tgid=%d; proc_type=%d)\n", udevid, tgid, (int)proc_type);
        return -EACCES;
    }

    return 0;
}

static int mpl_comm_para_check(u32 udevid, u64 va, u64 size, u64 page_size)
{
    if ((va == 0) || (size == 0) || (page_size == 0)) {
        svm_err("Mpl para check failed. (va=0x%llx; size=0x%llx; page_size=%llu)\n", va, size, page_size);
        return -EINVAL;
    }

    if ((SVM_IS_ALIGNED(va, page_size) == false) || (SVM_IS_ALIGNED(size, page_size) == false)) {
        svm_err("Va or size isn't aligned by page_size. (va=0x%llx; size=%llu; page_size=%llu)\n", va, size, page_size);
        return -EINVAL;
    }

    /* does not need to check va, later check vma will check va and size */

    return 0;
}

static int mpl_populate_para_check(u32 udevid, u64 va, u64 size, u64 page_size, u32 flag)
{
    int ret;

    ret = mpl_comm_para_check(udevid, va, size, page_size);
    if (ret != 0) {
        return ret;
    }

    if ((udevid != uda_get_host_id()) && ((flag & SVM_MPL_FLAG_FIXED_NUMA) != 0)) {
        svm_err("No permission to fixed numa. (udevid=%u; flag=0x%x)\n", udevid, flag);
        return -EACCES;
    }

    return 0;
}

static enum svm_page_granularity mpl_get_page_gran_by_flag(u32 flag)
{
    if ((flag & SVM_MPL_FLAG_GPAGE) != 0) {
        return SVM_PAGE_GRAN_GIANT;
    } else if ((flag & SVM_MPL_FLAG_HPAGE) != 0) {
        return SVM_PAGE_GRAN_HUGE;
    } else {
        return SVM_PAGE_GRAN_NORMAL;
    }
}

static int _mpl_populate(u32 udevid, struct svm_mpl_populate_para *para, ka_page_t **pages,
    u64 page_num, u32 page_size)
{
    ka_vm_area_struct_t *vma = NULL;
    struct svm_pgtlb_attr pgtlb_attr;
    u64 va = para->va;
    u64 size = para->size;
    u32 flag = para->flag;
    int ret;

    vma = ka_mm_find_vma(ka_task_get_current_mm(), va);
    if ((vma == NULL) || (svm_check_vma(vma, va, size) != 0)) {
        svm_err("Svm find vma failed. (va=0x%llx)\n", va);
        return -EINVAL;
    }

    ret = pmm_add_seg(udevid, va, size, PMM_SEG_WITH_PA);
    if (ret != 0) {
        svm_err("Add seg failed. (va=0x%llx; va=0x%llx; ret=%d)\n", va, size, ret);
        return ret;
    }

    svm_pgtlb_attr_packet(&pgtlb_attr, ((flag & SVM_MPL_FLAG_PG_NC) != 0), ((flag & SVM_MPL_FLAG_PG_RDONLY) != 0),
        false, page_size);

    svm_set_vma_status(vma, VMA_STATUS_NORMAL_OP);
    ret = svm_remap_pages(vma, va, pages, page_num, &pgtlb_attr);
    svm_set_vma_status(vma, VMA_STATUS_IDLE);
    if (ret != 0) {
        (void)pmm_del_seg(udevid, va, size, PMM_SEG_WITH_PA);
        svm_err("Remap failed. (va=0x%llx; va=0x%llx; ret=%d)\n", va, size, ret);
        return ret;
    }

    if (udevid != uda_get_host_id() && ((flag & SVM_MPL_FLAG_PG_RDONLY) == 0)) {
        /* clear memory to 0 */
        ret = svm_clear_mem_by_uva(udevid, ka_task_get_current_tgid(), va, size);
        if (ret != 0) {
            svm_clear_pages(pages, page_num, page_size);
        }
    }

    return 0;
}

static int _mpl_depopulate(u32 udevid, struct svm_mpl_depopulate_para *para, ka_vm_area_struct_t *vma,
    ka_page_t **pages, u64 page_num, u32 page_size)
{
    u64 va = para->va;
    u64 size = para->size;
    int ret;

    svm_set_vma_status(vma, VMA_STATUS_NORMAL_OP);
    ret = svm_unmap_addr(vma, va, size, page_size);
    svm_set_vma_status(vma, VMA_STATUS_IDLE);
    if (ret == 0) {
        (void)pmm_del_seg(udevid, va, size, PMM_SEG_WITH_PA);
    }

    return ret;
}

static u32 mpl_flag_to_gfp_flag(u32 flag)
{
    u32 gfp_flag = 0;

    gfp_flag |= ((flag & SVM_MPL_FLAG_CONTIGUOUS) != 0) ? SVM_GFP_FLAG_CONTINUOUS : 0;
    gfp_flag |= ((flag & SVM_MPL_FLAG_P2P) != 0) ? SVM_GFP_FLAG_P2P : 0;

    if ((flag & SVM_MPL_FLAG_FIXED_NUMA) != 0) {
        gfp_flag |= SVM_GFP_FLAG_FIXED_NUMA;
        gfp_flag_set_numa_id(&gfp_flag, mpl_flag_get_numa_id(flag));
    }

    return gfp_flag;
}

static int mpl_populate(u32 udevid, struct svm_mpl_populate_para *para, u64 page_size)
{
    ka_page_t **pages = NULL;
    enum svm_page_granularity gran = mpl_get_page_gran_by_flag(para->flag);
    u64 page_num = para->size / page_size;
    u32 gfp_flag = mpl_flag_to_gfp_flag(para->flag);
    int ret;

    pages = (ka_page_t**)svm_kvmalloc(sizeof(ka_page_t *) * page_num, KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (pages == NULL) {
        svm_err("Mpl kvmalloc inpages failed. (page_num=%llu)\n", page_num);
        return -ENOMEM;
    }

    ret = svm_alloc_pages(udevid, gran, pages, page_num, gfp_flag);
    if (ret != 0) {
        svm_kvfree(pages);
        svm_no_err_if((ret == -ENOMEM), "Alloc page not success. (ret=%d; page_num=%llu; gfp_flag=0x%x)\n",
            ret, page_num, gfp_flag);
        return ret;
    }

    ka_task_down_write(get_mmap_sem(ka_task_get_current_mm()));
    ret = _mpl_populate(udevid, para, pages, page_num, page_size);
    ka_task_up_write(get_mmap_sem(ka_task_get_current_mm()));
    if (ret != 0) {
        svm_free_pages(gran, pages, page_num, 0);
    }

    svm_kvfree(pages);

    return ret;
}

static int mpl_depopulate(u32 udevid, struct svm_mpl_depopulate_para *para, u64 page_size)
{
    ka_vm_area_struct_t *vma = NULL;
    ka_page_t **pages = NULL;
    u64 page_num = para->size / page_size;
    u64 query_size;
    int ret;

    pages = (ka_page_t**)svm_kvmalloc(sizeof(ka_page_t *) * page_num, KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (pages == NULL) {
        svm_err("Mpl kvmalloc inpages failed. (page_num=%llu)\n", page_num);
        return -ENOMEM;
    }

    ka_task_down_write(get_mmap_sem(ka_task_get_current_mm()));
    vma = ka_mm_find_vma(ka_task_get_current_mm(), para->va);
    if ((vma == NULL) || (svm_check_vma(vma, para->va, para->size) != 0)) {
        ka_task_up_write(get_mmap_sem(ka_task_get_current_mm()));
        svm_kvfree(pages);
        svm_err("Find vma failed. (va=0x%llx; size=0x%llx)\n", para->va, para->size);
        return -EFAULT;
    }

    query_size = svm_query_pages(vma, para->va, para->size, pages, &page_num);
    if (query_size != para->size) {
        ka_task_up_write(get_mmap_sem(ka_task_get_current_mm()));
        svm_kvfree(pages);
        svm_err("Query page failed. (va=0x%llx; size=0x%llx; query_size=0x%llx)\n", para->va, para->size, query_size);
        return -EFAULT;
    }

    ret = _mpl_depopulate(udevid, para, vma, pages, page_num, page_size);
    ka_task_up_write(get_mmap_sem(ka_task_get_current_mm()));
    if (ret == 0) {
        svm_free_pages(svm_page_size_to_page_gran(page_size), pages, page_num, 0);
    }

    svm_kvfree(pages);

    return ret;
}

static int mpl_ioctl_populate(u32 udevid, u32 cmd, unsigned long arg)
{
    struct svm_mpl_populate_para para;
    enum svm_page_granularity gran;
    u64 page_size;
    int ret;

    ret = ka_base_copy_from_user(&para, (void __ka_user *)(uintptr_t)arg, sizeof(struct svm_mpl_populate_para));
    if (ret != 0) {
        svm_err("Mpl copy arg from user failed. (ret=%d; arg=0x%lx)\n", ret, arg);
        return -EINVAL;
    }

    gran = mpl_get_page_gran_by_flag(para.flag);
    page_size = svm_page_gran_to_page_size(gran);
    ret = mpl_populate_para_check(udevid, para.va, para.size, page_size, para.flag);
    if (ret != 0) {
        return ret;
    }

    ret = svm_call_ioctl_pre_handler(udevid, cmd, (void *)&para);
    if (ret != 0) {
        svm_err("Pre handle failed. (ret=%d; va=0x%llx)\n", ret, para.va);
        return ret;
    }

    ret = mpl_populate(udevid, &para, page_size);
    if (ret != 0) {
        svm_call_ioctl_pre_cancle_handler(udevid, cmd, (void *)&para);
        return ret;
    }

    ret = svm_call_ioctl_post_handler(udevid, cmd, (void *)&para);
    if (ret != 0) {
        struct svm_mpl_depopulate_para depopulate_para = {.va = para.va, .size = para.size};
        (void)mpl_depopulate(udevid, &depopulate_para, page_size);
        svm_err("Post handle failed. (ret=%d; va=0x%llx)\n", ret, para.va);
    }

    return ret;
}

static int mpl_ioctl_depopulate(u32 udevid, u32 cmd, unsigned long arg)
{
    struct svm_mpl_depopulate_para para;
    u64 page_size;
    int ret;

    ret = ka_base_copy_from_user(&para, (void __ka_user *)(uintptr_t)arg, sizeof(struct svm_mpl_depopulate_para));
    if (ret != 0) {
        svm_err("Mpl copy arg from user failed. (ret=%d; arg=0x%lx)\n", ret, arg);
        return -EINVAL;
    }

    page_size = svm_task_va_to_page_size(ka_task_get_current(), para.va);
    if (page_size == 0) {
        svm_err("Mpl va to page size failed. (tgid=%d; va=0x%llx)\n", ka_task_get_current_tgid(), para.va);
        return -EINVAL;
    }

    ret = mpl_comm_para_check(udevid, para.va, para.size, page_size);
    if (ret != 0) {
        svm_err("Mpl depopulate para check failed. (ret=%d; va=0x%llx; size=%llu)\n", ret, para.va, para.size);
        return ret;
    }

    ret = svm_call_ioctl_pre_handler(udevid, cmd, (void *)&para);
    if (ret != 0) {
        svm_err("Pre handle failed. (ret=%d; va=0x%llx)\n", ret, para.va);
        return ret;
    }

    ret = mpl_depopulate(udevid, &para, page_size);
    if (ret != 0) {
        svm_call_ioctl_pre_cancle_handler(udevid, cmd, (void *)&para);
    }

    return ret;
}

static int svm_mpl_ioctl_comm(u32 udevid, u32 cmd, unsigned long arg)
{
    int ret = mpl_permission_check(udevid, ka_task_get_current_tgid());
    if (ret != 0) {
        return ret;
    }

    if (cmd == SVM_MPL_POPULATE) {
        return mpl_ioctl_populate(udevid, cmd, arg);
    } else {
        return mpl_ioctl_depopulate(udevid, cmd, arg);
    }
}

int svm_mpl_feature_init(void)
{
    svm_register_ioctl_cmd_handle(_IOC_NR(SVM_MPL_POPULATE), svm_mpl_ioctl_comm);
    svm_register_ioctl_cmd_handle(_IOC_NR(SVM_MPL_DEPOPULATE), svm_mpl_ioctl_comm);

    return 0;
}
DECLAER_FEATURE_AUTO_INIT(svm_mpl_feature_init, FEATURE_LOADER_STAGE_6);

