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
#include "ka_task_pub.h"
#include "ka_common_pub.h"
#include "ka_memory_pub.h"
#include "ka_system_pub.h"
#include "ka_fs_pub.h"
#include "ka_compiler_pub.h"
#include "ka_sched_pub.h"

#include "comm_kernel_interface.h"
#include "comm_msg_chan.h"
#include "pbl_feature_loader.h"
#include "pbl_uda.h"
#include "pbl_spod_info.h"
#include "dpa_kernel_interface.h"

#include "svm_gup.h"
#include "svm_pgtable.h"
#include "svm_slab.h"
#include "svm_mm.h"
#include "svm_smp.h"
#include "svm_kern_log.h"
#include "dbi_kern.h"
#include "pmq.h"
#include "pmq_client.h"
#include "svm_ioctl_ex.h"
#include "framework_cmd.h"
#include "framework_vma.h"
#include "dma_map_flag.h"
#include "dma_map_ioctl.h"
#include "dma_map_ctx.h"
#include "ksvmm.h"
#include "dma_map_core.h"

static bool svm_is_host_va(struct svm_global_va *dst_va)
{
    return (dst_va->udevid == uda_get_host_id());
}

static int _svm_dma_map_query_host_io_addr(ka_mm_struct_t *mm, u64 va, u64 size,
    struct svm_pa_seg pa_seg[], u64 *seg_num)
{
    ka_vm_area_struct_t *vma = NULL;
    u64 query_size;
    int ret;

    vma = ka_mm_find_vma(mm, va);
    if (vma == NULL) {
        svm_err("Find vma failed. (va=0x%llx; size=%llu)\n", va, size);
        return -ENOSYS;
    }

    ret = svm_check_va_range(va, size, ka_mm_get_vm_start(vma), ka_mm_get_vm_end(vma));
    if (ret != 0) {
        svm_err("Vma is invalid. (va=0x%llx; size=0x%llx; vma_start=0x%lx; vma_end=0x%lx)\n",
            va, size, ka_mm_get_vm_start(vma), ka_mm_get_vm_end(vma));
        return -EFAULT;
    }

    if ((ka_mm_get_vm_flags(vma) & KA_VM_PFNMAP) == 0) {
        svm_err("Vma is not pfn-mapped. (va=0x%llx; size=%llu)\n", va, size);
        return -EINVAL;
    }

    query_size = svm_query_phys(vma, va, size, pa_seg, seg_num);
    if (query_size != size) {
        svm_err("Query pa failed. (va=0x%llx; size=0x%llx; query_size=0x%llx)\n", va, size, query_size);
        return -EFAULT;
    }

    /* Only check the first page, svm_query_phys will check mixed mappings. */
    if (svm_pa_is_local_mem(pa_seg[0].pa)) {
        svm_err("Pa shouldn't be local memory. (va=0x%llx; size=%llu)\n", va, size);
        return -EINVAL;
    }

    return 0;
}

static int svm_dma_map_query_host_io_addr(int tgid, u64 va, u64 size, struct svm_pa_seg pa_seg[], u64 *seg_num)
{
    ka_mm_struct_t *mm = NULL;
    int ret;

    mm = svm_mm_get(tgid, false);
    if (mm == NULL) {
        return -ENOSYS;
    }

    ret = _svm_dma_map_query_host_io_addr(mm, va, size, pa_seg, seg_num);
    svm_mm_put(mm, false);
    return ret;
}

static int svm_dma_map_pin_host_pa(struct dma_map_node *map_node)
{
    struct svm_global_va *dst_va = &map_node->align_dst_va;
    u64 page_num = dst_va->size / KA_MM_PAGE_SIZE;
    u32 gup_flag = 0;
    int ret;

    map_node->pages = (ka_page_t **)svm_kvzalloc(page_num * sizeof(ka_page_t *), KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (map_node->pages == NULL) {
        svm_err("Malloc pages mng failed. (page_num=%llu)\n", page_num);
        return -ENOMEM;
    }

    gup_flag |= map_node->is_write ? SVM_GUP_FLAG_ACCESS_WRITE : 0;
    gup_flag |= map_node->is_svm_va ? 0 : SVM_GUP_FLAG_CHECK_PA_LOCAL; /* svm va will check local by smp, ksvmm */
    ret = svm_pin_uva_npages(dst_va->va, page_num, gup_flag, map_node->pages, &map_node->is_pfn_map);
    if (ret != 0) {
        svm_err("Pin host addr failed. (ret=%d; va=0x%llx; size=0x%llx)\n", ret, dst_va->va, dst_va->size);
        svm_kvfree(map_node->pages);
        map_node->pages = NULL;
        return ret;
    }

    return 0;
}

static void svm_dma_map_unpin_host_pa(struct dma_map_node *map_node)
{
    struct svm_global_va *dst_va = &map_node->align_dst_va;
    u64 page_num = dst_va->size / KA_MM_PAGE_SIZE;

    svm_unpin_uva_npages(map_node->is_pfn_map, map_node->pages, page_num, page_num);
    svm_kvfree(map_node->pages);
    map_node->pages = NULL;
}

static bool svm_is_local_map(struct svm_global_va *src_info, u32 dst_server_id, u32 dst_udevid, u32 dst_tgid)
{
    return ((src_info->server_id == SVM_INVALID_SERVER_ID) || (src_info->server_id == dst_server_id)) &&
        (src_info->udevid == dst_udevid) && (src_info->tgid == dst_tgid);
}

static void svm_ksvmm_range_unpin_mem(u32 udevid, int tgid, u64 va, u64 size)
{
    struct svm_global_va src_info;
    unsigned long stamp = (unsigned long)ka_jiffies;
    u64 seg_start, cur_va;
    int ret;

    for (cur_va = va; cur_va < (va + size);) {
        ka_try_cond_resched(&stamp);
        ret = ksvmm_get_seg(udevid, tgid, cur_va, &seg_start, &src_info);
        if (ret != 0) {
            return;
        }

        (void)ksvmm_unpin_seg(udevid, tgid, cur_va, 1ULL);
        cur_va = seg_start + src_info.size;
    }
}

static int _svm_ksvmm_range_pin_local_map_mem(u32 udevid, int tgid, u64 va, u64 size,
    u32 local_server_id, int dst_tgid)
{
    struct svm_global_va src_info;
    unsigned long stamp = (unsigned long)ka_jiffies;
    u64 seg_start, cur_va;
    int ret;

    for (cur_va = va; cur_va < (va + size);) {
        ka_try_cond_resched(&stamp);
        ret = ksvmm_pin_seg(udevid, tgid, cur_va, 1ULL);
        if (ret != 0) {
            svm_err("Pin ksvmm seg failed. (ret=%d; udevid=%u; tgid=%d; va=0x%llx)\n", ret, udevid, tgid, cur_va);
            svm_ksvmm_range_unpin_mem(udevid, tgid, va, cur_va - va);
            return ret;
        }

        ret = ksvmm_get_seg(udevid, tgid, cur_va, &seg_start, &src_info);
        if (ka_unlikely(ret != 0)) {
            svm_err("Get ksvmm seg failed. (ret=%d; va=0x%llx)\n", ret, cur_va);
            (void)ksvmm_unpin_seg(udevid, tgid, cur_va, 1ULL);
            svm_ksvmm_range_unpin_mem(udevid, tgid, va, cur_va - va);
            return ret;
        }

        if (ka_unlikely(!svm_is_local_map(&src_info, local_server_id, udevid, dst_tgid))) {
            svm_err("Isn't local map mem.\n");
            (void)ksvmm_unpin_seg(udevid, tgid, cur_va, 1ULL);
            svm_ksvmm_range_unpin_mem(udevid, tgid, va, cur_va - va);
            return -EINVAL;
        }

        cur_va = seg_start + src_info.size;
    }

    return 0;
}

static int svm_ksvmm_range_pin_local_map_mem(u32 udevid, int tgid, u64 va, u64 size)
{
    struct spod_info info;
    u32 local_server_id = SVM_INVALID_SERVER_ID;
    int ret, dst_tgid;

    ret = dbl_get_spod_info(udevid, &info);
    if (ret != 0) {
        svm_debug("Get server id failed. (udevid=%u)\n", udevid);
    } else {
        local_server_id = (u32)info.server_id;
    }

    if (udevid == uda_get_host_id()) {
        dst_tgid = tgid;
    } else {
        ret = hal_kernel_apm_query_slave_tgid_by_master(tgid, udevid, PROCESS_CP1, &dst_tgid);
        if (ret != 0) {
            svm_err("Apm get cp1 tgid failed. (ret=%d; udevid=%u)\n", ret, udevid);
            return ret;
        }
    }

    return _svm_ksvmm_range_pin_local_map_mem(udevid, tgid, va, size, local_server_id, dst_tgid);
}

static int svm_dma_map_pin_svm_pa(struct dma_map_node *map_node)
{
    struct svm_global_va *dst_va = &map_node->align_dst_va;
    int tgid = map_node->tgid;
    int ret;

    map_node->is_mpl_va = true;
    ret = svm_smp_pin_mem(dst_va->udevid, tgid, dst_va->va, dst_va->size); /* For mpl mem. */
    if (ret != 0) {
        ret = svm_ksvmm_range_pin_local_map_mem(dst_va->udevid, tgid, dst_va->va, dst_va->size); /* For smm local map. */
        if (ret == 0) {
            map_node->is_mpl_va = false;
        }
    }
    if (ret != 0) {
        svm_err("Pin mem failed. (udevid=%u; start=0x%llx; size=0x%llx)\n",
            dst_va->udevid, dst_va->va, dst_va->size);
        return ret;
    }
    map_node->is_local_mem = true;

    if (svm_is_host_va(dst_va)) {
        ret = svm_dma_map_pin_host_pa(map_node);
        if (ret != 0) {
            if (map_node->is_mpl_va) {
                (void)svm_smp_unpin_mem(dst_va->udevid, tgid, dst_va->va, dst_va->size);
            } else {
                svm_ksvmm_range_unpin_mem(dst_va->udevid, tgid, dst_va->va, dst_va->size);
            }
            return ret;
        }
    }

    return 0;
}

static int svm_dma_map_unpin_svm_pa(struct dma_map_node *map_node)
{
    struct svm_global_va *dst_va = &map_node->align_dst_va;
    int tgid = map_node->tgid;
    int ret;

    if (svm_is_host_va(dst_va)) {
        svm_dma_map_unpin_host_pa(map_node);
    }

    if (map_node->is_mpl_va) {
        ret = svm_smp_unpin_mem(dst_va->udevid, tgid, dst_va->va, dst_va->size);
        if (ret != 0) {
            svm_err("Smp unpin mem failed. (udevid=%u; start=0x%llx; size=0x%llx)\n",
                dst_va->udevid, dst_va->va, dst_va->size);
            return ret;
        }
    } else {
        svm_ksvmm_range_unpin_mem(dst_va->udevid, tgid, dst_va->va, dst_va->size);
    }

    return 0;
}

static int svm_dma_map_pin_non_svm_pa(struct dma_map_node *map_node)
{
    struct svm_global_va *dst_va = &map_node->align_dst_va;

    if (svm_is_host_va(dst_va) == false) {
        svm_err("Device only support svm addr. (udevid=%u; start=0x%llx; size=0x%llx)\n",
            dst_va->udevid, dst_va->va, dst_va->size);
        return -EINVAL;
    }

    if (svm_dma_map_flag_is_va_io_map(map_node->flag)) {
        map_node->is_local_mem = false;
        return 0;
    }
    map_node->is_local_mem = true;

    return svm_dma_map_pin_host_pa(map_node);
}

static int svm_dma_map_unpin_non_svm_pa(struct dma_map_node *map_node)
{
    if (map_node->is_local_mem) {
        svm_dma_map_unpin_host_pa(map_node);
    }

    return 0;
}

static int svm_dma_map_pin_pa(struct dma_map_ctx *ctx, struct dma_map_node *map_node)
{
    struct svm_global_va *dst_va = &map_node->align_dst_va;
    int va_type = VA_TYPE_SVM;

    if (dst_va->udevid == uda_get_host_id()) {
        int ret = svm_get_current_task_va_type(dst_va->va, dst_va->size, &va_type);
        if (ret != 0) {
            svm_err("Invalid addr. (udevid=%u; va=0x%llx; size=%llu)\n",
                dst_va->udevid, dst_va->va, dst_va->size);
            return ret;
        }
    }

    if (va_type == VA_TYPE_SVM) {
        map_node->is_svm_va = true;
        return svm_dma_map_pin_svm_pa(map_node);
    } else {
        map_node->is_svm_va = false;
        return svm_dma_map_pin_non_svm_pa(map_node);
    }
}

static int svm_dma_map_unpin_pa(struct dma_map_node *map_node)
{
    if (map_node->is_svm_va) {
        return svm_dma_map_unpin_svm_pa(map_node);
    } else {
        return svm_dma_map_unpin_non_svm_pa(map_node);
    }
}

static void svm_dma_map_query_host_pa(struct dma_map_node *map_node, struct svm_pa_seg *pa_seg, u64 *seg_num)
{
    u64 i;

    for (i = 0; i < *seg_num; i++) {
        pa_seg[i].pa = ka_mm_page_to_phys(map_node->pages[i]);
        pa_seg[i].size = KA_MM_PAGE_SIZE;
    }
}

static int svm_dma_map_query_svm_pa(u32 udevid, struct dma_map_node *map_node,
    struct svm_pa_seg *pa_seg, u64 *seg_num)
{
    struct svm_global_va *dst_va = &map_node->align_dst_va;
    int ret = 0;

    if (svm_is_host_va(dst_va)) {
        svm_dma_map_query_host_pa(map_node, pa_seg, seg_num);
    } else {
        if (dst_va->udevid == udevid) { /* device map */
            ret = svm_pmq_client_pa_query(uda_get_host_id(), dst_va, pa_seg, seg_num); /* use pa as dma addr, for esl */
        } else { /* p2p map bar */
            ret = svm_pmq_client_host_bar_query(uda_get_host_id(), dst_va, pa_seg, seg_num);
        }
    }
    if (ret != 0) {
        svm_err("Query pa failed. (start=0x%llx; size=0x%llx)\n", dst_va->va, dst_va->size);
        return ret;
    }

    return ret;
}

static int svm_dma_map_query_non_svm_pa(u32 udevid, struct dma_map_node *map_node,
    struct svm_pa_seg *pa_seg, u64 *seg_num)
{
    if (map_node->is_local_mem) {
        svm_dma_map_query_host_pa(map_node, pa_seg, seg_num);
        return 0;
    } else {
        struct svm_global_va *dst_va = &map_node->align_dst_va;
        return svm_dma_map_query_host_io_addr(map_node->tgid, dst_va->va, dst_va->size, pa_seg, seg_num);
    }
}

static int svm_dma_map_query_pa(u32 udevid, struct dma_map_node *map_node, struct svm_pa_seg *pa_seg, u64 *seg_num)
{
    if (map_node->is_svm_va) {
        return svm_dma_map_query_svm_pa(udevid, map_node, pa_seg, seg_num);
    } else {
        return svm_dma_map_query_non_svm_pa(udevid, map_node, pa_seg, seg_num);
    }
}

static void svm_host_dma_unmap_proc(u32 udevid, struct dma_map_node *map_node)
{
    ka_device_t *dev = uda_get_device(udevid);
    u64 seg_num = map_node->addr_info.dma_addr_seg_num;
    unsigned long stamp = (unsigned long)ka_jiffies;
    u32 i;

    for (i = 0; i < seg_num; i++) {
        struct svm_dma_addr_seg *dma_seg = &map_node->addr_info.seg[i];
        if (dma_seg->size == 0) {
            break;
        }

        ka_try_cond_resched(&stamp);
        if (map_node->is_local_mem) {
            hal_kernel_devdrv_dma_unmap_page(dev, (ka_dma_addr_t)dma_seg->dma_addr, dma_seg->size, KA_DMA_BIDIRECTIONAL);
        } else {
            devdrv_dma_unmap_resource(dev, (ka_dma_addr_t)dma_seg->dma_addr, dma_seg->size, KA_DMA_BIDIRECTIONAL, 0);
        }
        dma_seg->size = 0;
    }
}

static int svm_host_dma_map_proc(u32 udevid, struct dma_map_node *map_node, struct svm_pa_seg *pa_seg)
{
    ka_device_t *dev = uda_get_device(udevid);
    u64 seg_num = map_node->addr_info.dma_addr_seg_num;
    unsigned long stamp = (unsigned long)ka_jiffies;
    u32 i;

    for (i = 0; i < seg_num; i++) {
        ka_dma_addr_t dma_addr;
        ka_page_t *page = NULL;

        ka_try_cond_resched(&stamp);
        if (map_node->is_local_mem) {
            page = svm_pa_to_page(pa_seg[i].pa);
            dma_addr = hal_kernel_devdrv_dma_map_page(dev, page, 0, pa_seg[i].size, KA_DMA_BIDIRECTIONAL);
        } else {
            dma_addr = devdrv_dma_map_resource(dev, (phys_addr_t)pa_seg[i].pa, pa_seg[i].size, KA_DMA_BIDIRECTIONAL, 0);
        }
        if (ka_mm_dma_mapping_error(dev, dma_addr) != 0) {
            svm_host_dma_unmap_proc(udevid, map_node);
            svm_err("Dma mapping error. (error=%d)\n", ka_mm_dma_mapping_error(dev, dma_addr));
            return ka_mm_dma_mapping_error(dev, dma_addr);
        }

        map_node->addr_info.seg[i].dma_addr = (u64)dma_addr;
        map_node->addr_info.seg[i].size = pa_seg[i].size;
    }

    return 0;
}

static void svm_device_dma_unmap_proc(u32 udevid, struct dma_map_node *map_node)
{
    u64 seg_num = map_node->addr_info.dma_addr_seg_num;
    u32 i;

    for (i = 0; i < seg_num; i++) {
        map_node->addr_info.seg[i].size = 0;
    }
}

static int svm_device_dma_map_proc(u32 udevid, struct dma_map_node *map_node, struct svm_pa_seg *pa_seg)
{
    u64 seg_num = map_node->addr_info.dma_addr_seg_num;
    u32 dst_udevid = map_node->align_dst_va.udevid;
    int ret;
    u32 i;

    if (dst_udevid != udevid) {
        if ((uda_is_pf_dev(udevid) == false) || (uda_is_pf_dev(dst_udevid) == false)) {
            svm_err("Vf not support. (src_udevid=%u; dst_udevid=%u)\n", udevid, dst_udevid);
            return -EINVAL;
        }
    }

    for (i = 0; i < seg_num; i++) {
        if (dst_udevid != udevid) {
            ret = devdrv_devmem_addr_bar_to_dma(udevid, dst_udevid,
                (phys_addr_t)pa_seg[i].pa, (ka_dma_addr_t *)&map_node->addr_info.seg[i].dma_addr);
            if (ret != 0) {
                svm_err("Bar to dma failed. (src_udevid=%u; dst_udevid=%u; ret=%d)\n", udevid, dst_udevid, ret);
                return ret;
            }
        } else {
            map_node->addr_info.seg[i].dma_addr = pa_seg[i].pa;
        }

        map_node->addr_info.seg[i].size = pa_seg[i].size;
    }

    return 0;
}

static int svm_dma_map_proc(u32 udevid, struct dma_map_node *map_node, struct svm_pa_seg *pa_seg)
{
    struct svm_global_va *dst_va = &map_node->align_dst_va;

    if (svm_is_host_va(dst_va)) {
        return svm_host_dma_map_proc(udevid, map_node, pa_seg);
    } else {
        return svm_device_dma_map_proc(udevid, map_node, pa_seg);
    }
}

static void svm_dma_unmap_proc(u32 udevid, struct dma_map_node *map_node)
{
    struct svm_global_va *dst_va = &map_node->align_dst_va;

    if (svm_is_host_va(dst_va)) {
        svm_host_dma_unmap_proc(udevid, map_node);
    } else {
        svm_device_dma_unmap_proc(udevid, map_node);
    }
}

static int svm_do_dma_map(struct dma_map_ctx *ctx, struct dma_map_node *map_node, u64 dst_page_size)
{
    struct svm_global_va *dst_va = &map_node->align_dst_va;
    struct svm_pa_seg *pa_seg = NULL;
    u32 udevid = ctx->udevid;
    u64 seg_num;
    int ret;

    ret = svm_dma_map_pin_pa(ctx, map_node);
    if (ret != 0) {
        svm_err("Pin pa failed. (start=0x%llx; size=0x%llx)\n", dst_va->va, dst_va->size);
        return ret;
    }

    seg_num = dst_va->size / dst_page_size;
    pa_seg = svm_kvzalloc(seg_num * sizeof(struct svm_pa_seg), KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (pa_seg == NULL) {
        (void)svm_dma_map_unpin_pa(map_node);
        svm_err("Malloc pa seg failed. (seg_num=%llu)\n", seg_num);
        return -ENOMEM;
    }

    ret = svm_dma_map_query_pa(udevid, map_node, pa_seg, &seg_num);
    if (ret != 0) {
        svm_kvfree(pa_seg);
        (void)svm_dma_map_unpin_pa(map_node);
        svm_err("Query pa failed. (start=0x%llx; size=0x%llx)\n", dst_va->va, dst_va->size);
        return ret;
    }

    map_node->addr_info.dma_addr_seg_num = svm_make_pa_continues(pa_seg, seg_num);
    map_node->addr_info.first_seg_offset = map_node->raw_va - dst_va->va;
    map_node->addr_info.last_seg_len = (pa_seg[map_node->addr_info.dma_addr_seg_num - 1].size
        - (dst_va->size - map_node->raw_size - map_node->addr_info.first_seg_offset));
    if (map_node->addr_info.dma_addr_seg_num == 1) {
        map_node->addr_info.last_seg_len -= map_node->addr_info.first_seg_offset;
    }

    map_node->addr_info.seg = svm_kvzalloc(map_node->addr_info.dma_addr_seg_num * sizeof(struct svm_dma_addr_seg),
        KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (map_node->addr_info.seg == NULL) {
        svm_kvfree(pa_seg);
        (void)svm_dma_map_unpin_pa(map_node);
        svm_err("Malloc dma seg failed. (seg_num=%llu)\n", seg_num);
        return -ENOMEM;
    }

    ret = svm_dma_map_proc(udevid, map_node, pa_seg);
    svm_kvfree(pa_seg); /* end use pa seg */
    if (ret != 0) {
        svm_kvfree(map_node->addr_info.seg);
        map_node->addr_info.seg = NULL;
        (void)svm_dma_map_unpin_pa(map_node);
        svm_err("Dma map failed. (start=0x%llx; size=0x%llx)\n", dst_va->va, dst_va->size);
        return ret;
    }

    return 0;
}

static int svm_do_dma_unmap(struct dma_map_node *map_node)
{
    int ret;

    svm_dma_unmap_proc(map_node->udevid, map_node);
    svm_kvfree(map_node->addr_info.seg);
    map_node->addr_info.seg = NULL;
    ret = svm_dma_map_unpin_pa(map_node);

    return ret;
}

static struct dma_map_node *dma_map_node_create(struct svm_global_va *dst_va, u64 dst_page_size, u32 flag)
{
    struct dma_map_node *map_node = svm_kvzalloc(sizeof(*map_node), KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (map_node == NULL) {
        svm_err("Alloc map_node node failed. (start=0x%llx; size=0x%llx)\n", dst_va->va, dst_va->size);
        return NULL;
    }

    map_node->range_node.start = dst_va->va;
    map_node->range_node.size = dst_va->size;
    map_node->raw_va = dst_va->va;
    map_node->raw_size = dst_va->size;
    map_node->align_dst_va = *dst_va;
    map_node->align_dst_va.va = ka_base_round_down(dst_va->va, dst_page_size);
    map_node->align_dst_va.size = ka_base_round_up(map_node->raw_va - map_node->align_dst_va.va + dst_va->size, dst_page_size);
    map_node->is_write = svm_dma_map_flag_is_access_write(flag) ? true : false;
    map_node->flag = flag;
    ka_base_atomic_set(&map_node->refcnt, 0);

    return map_node;
}

static inline void dma_map_node_destroy(struct dma_map_node *map_node)
{
    svm_kvfree(map_node);
}

static struct dma_map_node *dma_map_node_search(struct dma_map_ctx *ctx, struct svm_global_va *dst_va)
{
    struct dma_map_node *map_node = NULL;
    struct range_rbtree_node *range_node = NULL;

    range_node = range_rbtree_search(&ctx->range_tree, dst_va->va, dst_va->size);
    if (range_node != NULL) {
        map_node = ka_container_of(range_node, struct dma_map_node, range_node);
        if ((map_node->align_dst_va.udevid == dst_va->udevid) && (map_node->align_dst_va.tgid == dst_va->tgid)) {
            return map_node;
        }
    }

    return NULL;
}

static int _svm_dma_map_addr(struct dma_map_ctx *ctx, struct svm_global_va *dst_va,
    u64 dst_page_size, u32 flag, void **handle)
{
    struct dma_map_node *map_node = NULL;
    int ret;

    if (handle == NULL) {
        ka_task_read_lock_bh(&ctx->lock);
        map_node = dma_map_node_search(ctx, dst_va);
        if (map_node != NULL) { /* host_register_post_malloc may insert already, do not print. */
            ka_task_read_unlock_bh(&ctx->lock);
            return -EBUSY;
        }
        ka_task_read_unlock_bh(&ctx->lock);
    }

    map_node = dma_map_node_create(dst_va, dst_page_size, flag);
    if (map_node == NULL) {
        return -ENOMEM;
    }

    map_node->udevid = ctx->udevid;
    map_node->tgid = ctx->tgid;

    ret = svm_do_dma_map(ctx, map_node, dst_page_size);
    if (ret != 0) {
        dma_map_node_destroy(map_node);
        return ret;
    }

    if (handle != NULL) {
        *handle = (void *)map_node;
        return 0;
    }

    ka_task_write_lock_bh(&ctx->lock);
    ret = range_rbtree_insert(&ctx->range_tree, &map_node->range_node);
    ka_task_write_unlock_bh(&ctx->lock);
    if (ret != 0) {
        (void)svm_do_dma_unmap(map_node);
        dma_map_node_destroy(map_node);
        svm_debug("Insert failed. (udevid=%u; tgid=%d; start=0x%llx; size=0x%llx)\n",
            ctx->udevid, ctx->tgid, dst_va->va, dst_va->size);
        ret = -EBUSY;
    }

    return ret;
}

int svm_dma_map_addr(u32 udevid, int tgid, struct svm_global_va *dst_va, u32 flag, void **handle)
{
    struct dma_map_ctx *ctx = NULL;
    u64 page_num;
    u64 npage_size;
    int ret;

    ret = svm_dbi_kern_query_npage_size(dst_va->udevid, &npage_size);
    if (ret != 0) {
        svm_err("Get page size failed. (udevid=%u)\n", dst_va->udevid);
        return ret;
    }

    page_num = svm_get_align_up_num(dst_va->va, dst_va->size, npage_size);
    if ((page_num == 0ULL) || (page_num >= KA_U32_MAX)) {
        svm_err("Invalid size. (size=0x%llx)\n", dst_va->size);
        return -EINVAL;
    }

    if (!svm_is_host_va(dst_va)) {
        if (udevid != dst_va->udevid) {
            if (!uda_proc_can_access_udevid(tgid, dst_va->udevid)) {
                svm_err("Can not access udevid. (udevid=%u)\n", dst_va->udevid);
                return -EINVAL;
            }
        }
    }

    ctx = dma_map_ctx_get(udevid, tgid);
    if (ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EINVAL;
    }

    ret = _svm_dma_map_addr(ctx, dst_va, npage_size, flag, handle);
    dma_map_ctx_put(ctx);

    return ret;
}

void svm_dma_unmap_addr_by_handle(void *handle)
{
    struct dma_map_node *map_node = (struct dma_map_node *)handle;

    (void)svm_do_dma_unmap(map_node);
    dma_map_node_destroy(map_node);
}

static int _svm_dma_unmap_addr(struct dma_map_ctx *ctx, struct svm_global_va *dst_va)
{
    struct dma_map_node *map_node = NULL;
    int ret;

    ka_task_write_lock_bh(&ctx->lock);

    map_node = dma_map_node_search(ctx, dst_va);
    if (map_node == NULL) { /* host_unregister_pre_free may erase already, do not print. */
        ka_task_write_unlock_bh(&ctx->lock);
        return -EINVAL;
    }

    if ((map_node->raw_va != dst_va->va) || (map_node->raw_size != dst_va->size)) {
        ka_task_write_unlock_bh(&ctx->lock);
        svm_err("Not map addr. (udevid=%u; tgid=%d; start=0x%llx; size=0x%llx)\n",
            ctx->udevid, ctx->tgid, dst_va->va, dst_va->size);
        return -EINVAL;
    }

    if (ka_base_atomic_read(&map_node->refcnt) > 0) {
        ka_task_write_unlock_bh(&ctx->lock);
        return -EBUSY;
    }

    range_rbtree_erase(&ctx->range_tree, &map_node->range_node);

    ka_task_write_unlock_bh(&ctx->lock);

    ret = svm_do_dma_unmap(map_node);
    dma_map_node_destroy(map_node);

    return ret;
}

int svm_dma_unmap_addr(u32 udevid, int tgid, struct svm_global_va *dst_va)
{
    struct dma_map_ctx *ctx = NULL;
    int ret;

    ctx = dma_map_ctx_get(udevid, tgid);
    if (ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EINVAL;
    }

    ret = _svm_dma_unmap_addr(ctx, dst_va);
    dma_map_ctx_put(ctx);

    return ret;
}

static void svm_fill_dma_addr_info(struct dma_map_node *map_node,
    struct svm_global_va *dst_va, struct svm_dma_addr_info *dma_info)
{
    struct svm_dma_addr_info *addr_info = &map_node->addr_info;
    u64 pre_first_segid_size = 0;
    u64 pre_last_segid_size;
    u32 first_segid_offset = 0;
    u32 last_segid_offset;

    dma_info->is_write = map_node->is_write;

    while (dst_va->va >= (map_node->align_dst_va.va + pre_first_segid_size + addr_info->seg[first_segid_offset].size)) {
        pre_first_segid_size += addr_info->seg[first_segid_offset].size;
        first_segid_offset++;
    }

    pre_last_segid_size = pre_first_segid_size;
    last_segid_offset = first_segid_offset;
    while ((dst_va->va + dst_va->size) >
        (map_node->align_dst_va.va + pre_last_segid_size + addr_info->seg[last_segid_offset].size)) {
        pre_last_segid_size += addr_info->seg[last_segid_offset].size;
        last_segid_offset++;
    }

    dma_info->dma_addr_seg_num = last_segid_offset - first_segid_offset + 1;
    dma_info->first_seg_offset = dst_va->va - map_node->align_dst_va.va - pre_first_segid_size;
    dma_info->last_seg_len = dst_va->va - map_node->align_dst_va.va + dst_va->size - pre_last_segid_size;
    if (dma_info->dma_addr_seg_num == 1) {
        dma_info->last_seg_len -= dma_info->first_seg_offset;
    }
    dma_info->seg = &addr_info->seg[first_segid_offset];
}

int svm_dma_addr_query_by_handle(void *handle, struct svm_global_va *dst_va, struct svm_dma_addr_info *dma_info)
{
    struct dma_map_node *map_node = (struct dma_map_node *)handle;

    if ((dst_va->va < map_node->raw_va) || ((dst_va->va + dst_va->size) > (map_node->raw_va + map_node->raw_size))) {
        svm_err("Invalid dst va. (map: va=0x%llx; size=0x%llx; dst: va=0x%llx; size=0x%llx)\n",
            map_node->raw_va, map_node->raw_size, dst_va->va, dst_va->size);
        return -EINVAL;
    }

    svm_fill_dma_addr_info(map_node, dst_va, dma_info);
    return 0;
}

static int _svm_dma_addr_get(struct dma_map_ctx *ctx, struct svm_global_va *dst_va, struct svm_dma_addr_info *dma_info)
{
    struct dma_map_node *map_node = NULL;
    int refcnt;

    ka_task_read_lock_bh(&ctx->lock);

    map_node = dma_map_node_search(ctx, dst_va);
    if (map_node == NULL) {
        ka_task_read_unlock_bh(&ctx->lock);
        svm_debug("Search not success. (udevid=%u; tgid=%d; start=0x%llx; size=0x%llx)\n",
            ctx->udevid, ctx->tgid, dst_va->va, dst_va->size);
        return -EINVAL;
    }

    refcnt = ka_base_atomic_inc_return(&map_node->refcnt);
    if (refcnt < 0) {
        ka_base_atomic_dec(&map_node->refcnt);
        ka_task_read_unlock_bh(&ctx->lock);
        svm_err("Get too much. (udevid=%u; tgid=%d; va=%llx; size=%llx; refcnt=%d)\n",
            ctx->udevid, ctx->tgid, dst_va->va, dst_va->size, refcnt);
        return -EINVAL;
    }

    ka_task_read_unlock_bh(&ctx->lock);

    svm_fill_dma_addr_info(map_node, dst_va, dma_info);

    return 0;
}

int svm_dma_addr_get(u32 udevid, int tgid, struct svm_global_va *dst_va, struct svm_dma_addr_info *dma_info)
{
    struct dma_map_ctx *ctx = NULL;
    int ret;

    ctx = dma_map_ctx_get(udevid, tgid);
    if (ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EINVAL;
    }

    ret = _svm_dma_addr_get(ctx, dst_va, dma_info);
    dma_map_ctx_put(ctx);

    return ret;
}

static void _svm_dma_addr_put(struct dma_map_ctx *ctx, struct svm_global_va *dst_va)
{
    struct dma_map_node *map_node = NULL;
    int refcnt;

    ka_task_read_lock_bh(&ctx->lock);

    map_node = dma_map_node_search(ctx, dst_va);
    if (map_node == NULL) {
        ka_task_read_unlock_bh(&ctx->lock);
        svm_info("Search not success. (udevid=%u; tgid=%d; start=%llx)\n", ctx->udevid, ctx->tgid, dst_va->va);
        return;
    }

    refcnt = ka_base_atomic_dec_return(&map_node->refcnt);
    if (refcnt < 0) {
        ka_base_atomic_inc(&map_node->refcnt); /* restore refcnt, hold read lock, del can not access refcnt same time */
        svm_err("No map, can not unmap. (udevid=%u; tgid=%d; va=%llx; size=%llx; refcnt=%d)\n",
            ctx->udevid, ctx->tgid, dst_va->va, dst_va->size, refcnt);
    }

    ka_task_read_unlock_bh(&ctx->lock);
}

void svm_dma_addr_put(u32 udevid, int tgid, struct svm_global_va *dst_va)
{
    struct dma_map_ctx *ctx = NULL;

    ctx = dma_map_ctx_get(udevid, tgid);
    if (ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return ;
    }

    _svm_dma_addr_put(ctx, dst_va);
    dma_map_ctx_put(ctx);
}

void dma_map_addr_show(struct dma_map_ctx *ctx, ka_seq_file_t *seq)
{
    struct range_rbtree_node *range_node, *next;
    int i = 0;

    ka_task_read_lock_bh(&ctx->lock);

    ka_fs_seq_printf(seq, "dma map: udevid %u tgid %d mem num %u\n", ctx->udevid, ctx->tgid, ctx->range_tree.node_num);

    ka_base_rbtree_postorder_for_each_entry_safe(range_node, next, &ctx->range_tree.root, node) {
        struct dma_map_node *map_node = ka_container_of(range_node, struct dma_map_node, range_node);
        struct svm_global_va *dst_va = &map_node->align_dst_va;
        struct svm_dma_addr_info *addr_info = &map_node->addr_info;

        if (i == 0) {
            ka_fs_seq_printf(seq, "   index   va               size      raw(va          size)"
                "     udevid   tgid dma_addr_seg_num first_seg_offset last_seg_len\n");
        }
        ka_fs_seq_printf(seq, "   %d       0x%llx     0x%llx      (0x%llx     0x%llx)     %u    %d     %llu     %llx    %llx\n",
            i++, dst_va->va, dst_va->size, map_node->raw_va, map_node->raw_size, dst_va->udevid, dst_va->tgid,
            addr_info->dma_addr_seg_num, addr_info->first_seg_offset, addr_info->last_seg_len);
    }

    ka_task_read_unlock_bh(&ctx->lock);
}

static int _dma_map_addr_recycle(struct dma_map_ctx *ctx)
{
    unsigned long stamp = (unsigned long)ka_jiffies;
    int recycle_num = 0;

    do {
        struct dma_map_node *map_node = NULL;
        struct range_rbtree_node *range_node = NULL;

        ka_task_write_lock_bh(&ctx->lock);
        range_node = range_rbtree_get_first(&ctx->range_tree);
        if (range_node == NULL) {
            ka_task_write_unlock_bh(&ctx->lock);
            break;
        }
        recycle_num++;
        map_node = ka_container_of(range_node, struct dma_map_node, range_node);
        range_rbtree_erase(&ctx->range_tree, range_node);
        ka_task_write_unlock_bh(&ctx->lock);

        (void)svm_do_dma_unmap(map_node);
        dma_map_node_destroy(map_node);
        ka_try_cond_resched(&stamp);
    } while (1);

    return recycle_num;
}

void dma_map_addr_recycle(struct dma_map_ctx *ctx)
{
    int recycle_num = 0;

    recycle_num = _dma_map_addr_recycle(ctx);
    if (recycle_num > 0) {
        svm_warn("Recycle mem. (udevid=%u; tgid=%d; recycle_num=%d)\n", ctx->udevid, ctx->tgid, recycle_num);
    }
}

static int svm_dst_va_to_global_va(struct svm_dst_va *dst_va, struct svm_global_va *global_va)
{
    int ret;

    svm_global_va_pack(0, 0, dst_va->va, dst_va->size, global_va);

    ret = uda_devid_to_udevid_ex(dst_va->devid, &global_va->udevid);
    if (ret != 0) {
        svm_err("Get udevid failed. (devid=%u)\n", dst_va->devid);
        return ret;
    }

    if (global_va->udevid != uda_get_host_id()) {
        ret = hal_kernel_apm_query_slave_tgid_by_master(ka_task_get_current_tgid(), global_va->udevid, dst_va->task_type, &global_va->tgid);
        if (ret != 0) {
            svm_err("Get slave tgid failed. (udevid=%u; task_type=%u)\n", global_va->udevid, dst_va->task_type);
            return ret;
        }
    } else {
        global_va->tgid = ka_task_get_current_tgid();
    }

    return 0;
}

static int svm_ioctl_dma_map_addr(u32 udevid, u32 cmd, unsigned long arg)
{
    struct svm_dma_map_para para;
    struct svm_global_va global_va;
    int ret;

    ret = ka_base_copy_from_user(&para, (void __ka_user *)(uintptr_t)arg, sizeof(para));
    if (ret != 0) {
        svm_err("Copy_from_user fail.\n");
        return -EINVAL;
    }

    ret = svm_dst_va_to_global_va(&para.dst_va, &global_va);
    if (ret != 0) {
        return ret;
    }

    return svm_dma_map_addr(udevid, ka_task_get_current_tgid(), &global_va, para.flag, NULL);
}

static int svm_ioctl_dma_unmap_addr(u32 udevid, u32 cmd, unsigned long arg)
{
    struct svm_dma_unmap_para para;
    struct svm_global_va global_va;
    int ret;

    ret = ka_base_copy_from_user(&para, (void __ka_user *)(uintptr_t)arg, sizeof(para));
    if (ret != 0) {
        svm_err("Copy_from_user fail.\n");
        return -EINVAL;
    }

    ret = svm_dst_va_to_global_va(&para.dst_va, &global_va);
    if (ret != 0) {
        return ret;
    }

    return svm_dma_unmap_addr(udevid, ka_task_get_current_tgid(), &global_va);
}

int svm_dma_map_core_init(void)
{
    svm_register_ioctl_cmd_handle(_IOC_NR(SVM_DMA_MAP), svm_ioctl_dma_map_addr);
    svm_register_ioctl_cmd_handle(_IOC_NR(SVM_DMA_UNMAP), svm_ioctl_dma_unmap_addr);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(svm_dma_map_core_init, FEATURE_LOADER_STAGE_4);

