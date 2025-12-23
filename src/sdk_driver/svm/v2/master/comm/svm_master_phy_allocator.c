/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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
#include <linux/version.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/numa.h>

#include "devmm_common.h"
#include "svm_phy_addr_blk_mng.h"
#include "svm_proc_mng.h"

static DEFINE_MUTEX(g_host_hgage_lock);

int devmm_obmm_get(struct devmm_host_obmm_info *info)
{
    ka_task_down_write(&info->rw_sem);
    if (info->obmm_alloc_func == NULL) {
        info->alloced_cnt = 0U;
    
        info->obmm_alloc_func = __symbol_get("obmm_alloc");
        if (info->obmm_alloc_func == NULL) {
            ka_task_up_write(&info->rw_sem);
            return -EINVAL;
        }

        info->obmm_free_func = __symbol_get("obmm_free");
        if (info->obmm_free_func == NULL) {
            __symbol_put("obmm_alloc");
            info->obmm_alloc_func = NULL;
            ka_task_up_write(&info->rw_sem);
            return -EINVAL;
        }
    }

    info->alloced_cnt++;
    ka_task_up_write(&info->rw_sem);
    return 0;
}

void devmm_obmm_put(struct devmm_host_obmm_info *info)
{
    ka_task_down_write(&info->rw_sem);
    if (info->alloced_cnt > 0) {
        info->alloced_cnt--;
    }

    if (info->alloced_cnt == 0U) {
        if (info->obmm_alloc_func != NULL) {
            __symbol_put("obmm_alloc");
            info->obmm_alloc_func = NULL;
        }
        if (info->obmm_free_func != NULL) {
            __symbol_put("obmm_free");
            info->obmm_free_func = NULL;
        }
    }
    ka_task_up_write(&info->rw_sem);
}

#if (defined CFG_SOC_PLATFORM_CLOUD_V2) && (defined __aarch64__)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
/* This func is time-consuming operation, may cause performance problem. */
bool devmm_pa_is_local_mem(u64 pa)
{
    return page_is_ram(PFN_DOWN(pa));
}

static int devmm_master_get_next_numa(int numa)
{
    int next_numa;
    next_numa = next_online_node(numa);
    if (next_numa >= MAX_NUMNODES) {
        next_numa = first_online_node;
    }
    return next_numa;
}

static void devmm_master_get_pfn_range_for_nid(int nid, u64 *start_pfn, u64 *end_pfn)
{
    if (nid < 0 || nid >= num_possible_nodes() || !node_online(nid)) {
        *start_pfn = *end_pfn = 0U;
        return;
    }

    if (NODE_DATA(nid) == NULL) {
        *start_pfn = *end_pfn = 0U;
        return;
    }

    *start_pfn = NODE_DATA(nid)->node_start_pfn;
    *end_pfn = NODE_DATA(nid)->node_start_pfn + NODE_DATA(nid)->node_spanned_pages;
}

static u64 devmm_master_alloc_interleaving_huge_pages(ka_page_t **pages, u64 pg_num, u32 gfp_mask)
{
    u64 alloced;
    u64 pa;
    u64 node_offset[DEVMM_S2S_HOST_NODE_NUM] = { 0 };
    u32 stamp = (u32)ka_jiffies;
    u32 i;

    for (alloced = 0; alloced < pg_num;) {
        for (i = 0; i < DEVMM_S2S_HOST_NODE_NUM && alloced < pg_num; i++) {
            u32 node_id = (i + alloced) % DEVMM_S2S_HOST_NODE_NUM;
            u64 hccs_start = devmm_get_host_node_local_addr(node_id);
            u64 hccs_end = hccs_start + DEVMM_S2S_HOST_NODE_MEM_SIZE;

            hccs_start += node_offset[node_id];
            hccs_start = ALIGN(hccs_start, SVM_MASTER_HUGE_PAGE_SIZE);
            hccs_end = ALIGN_DOWN(hccs_end, SVM_MASTER_HUGE_PAGE_SIZE);
            mutex_lock(&g_host_hgage_lock);
            for (pa = hccs_start; pa < hccs_end; pa += SVM_MASTER_HUGE_PAGE_SIZE) {
                u64 start_pfn = __phys_to_pfn(pa);
                u64 end_pfn = __phys_to_pfn(pa + SVM_MASTER_HUGE_PAGE_SIZE);
                devmm_try_cond_resched(&stamp);
                if (!devmm_pa_is_local_mem(pa)) {
                    continue;
                }

                if (alloc_contig_range(start_pfn, end_pfn, MIGRATE_MOVABLE, gfp_mask) == 0) {
                    pages[alloced] = pfn_to_page(start_pfn);
                    (void)memset_s(page_address(pages[alloced++]), SVM_MASTER_HUGE_PAGE_SIZE, 0, SVM_MASTER_HUGE_PAGE_SIZE);
                    devmm_drv_debug("alloc interleaving huge pages. (cpu_id:%u; pfn:%pk)\n", node_id, (void *)start_pfn);
                    // set DEVMM_S2S_HOST_NODE_NUM + 1U, indicates that 'alloced' has changed
                    i = DEVMM_S2S_HOST_NODE_NUM + 1U;
                    break;
                }
            }
            node_offset[node_id] = pa - devmm_get_host_node_local_addr(node_id);
            mutex_unlock(&g_host_hgage_lock);
        }

        if (i == DEVMM_S2S_HOST_NODE_NUM) { // break if 'alloced' is not changed
            break;
        }
    }

    return alloced;
}

static u64 devmm_master_alloc_numa_huge_pages(u32 numa_id, ka_page_t **pages, u64 pg_num, u32 gfp_mask)
{
    u64 alloced = 0;
    u64 pa;
    u32 stamp = (u32)ka_jiffies;
    u32 node_id;
    int numa_total, cur_numa;
    int i;
    u64 numa_start_pfn, numa_end_pfn;

    cur_numa = (numa_id == 0) ? numa_node_id() : (numa_id - 1U);
    numa_total = num_online_nodes();
    if (cur_numa < 0 || cur_numa >= MAX_NUMNODES || !node_online(cur_numa)) {
        devmm_drv_err("invalid numa id. (numa:%u)\n", cur_numa);
        return 0;
    }

    for (i = 0; i < numa_total && alloced < pg_num; i++) {
        devmm_master_get_pfn_range_for_nid(cur_numa, &numa_start_pfn, &numa_end_pfn);
        for (node_id = 0; node_id < DEVMM_S2S_HOST_NODE_NUM; node_id++) {
            u64 numa_start = __pfn_to_phys(numa_start_pfn);
            u64 numa_end = __pfn_to_phys(numa_end_pfn);
            u64 hccs_start = devmm_get_host_node_local_addr(node_id);
            u64 hccs_end = hccs_start + DEVMM_S2S_HOST_NODE_MEM_SIZE;

            hccs_start = max(numa_start, hccs_start);
            hccs_end = min(numa_end, hccs_end);
            hccs_start = ALIGN(hccs_start, SVM_MASTER_HUGE_PAGE_SIZE);
            hccs_end = ALIGN_DOWN(hccs_end, SVM_MASTER_HUGE_PAGE_SIZE);
            if (hccs_start >= hccs_end) {
                continue;
            }

            mutex_lock(&g_host_hgage_lock);
            for (pa = hccs_start; pa < hccs_end && alloced < pg_num; pa += SVM_MASTER_HUGE_PAGE_SIZE) {
                u64 start_pfn = __phys_to_pfn(pa);
                u64 end_pfn = __phys_to_pfn(pa + SVM_MASTER_HUGE_PAGE_SIZE);
                devmm_try_cond_resched(&stamp);
                if (!devmm_pa_is_local_mem(pa)) {
                    continue;
                }

                if (alloc_contig_range(start_pfn, end_pfn, MIGRATE_MOVABLE, gfp_mask) == 0) {
                    pages[alloced] = pfn_to_page(start_pfn);
                    (void)memset_s(page_address(pages[alloced++]), SVM_MASTER_HUGE_PAGE_SIZE, 0, SVM_MASTER_HUGE_PAGE_SIZE);
                    devmm_drv_debug("alloc normal huge pages. (numa:%u; pfn:%pk)\n", cur_numa, (void *)start_pfn);
                }
            }
            mutex_unlock(&g_host_hgage_lock);
        }

        if (numa_id > 0) { // alloc in this numa only, if numa is designated
            break;
        }
        cur_numa = devmm_master_get_next_numa(cur_numa);
    }
    return alloced;
}

static u64 devmm_master_alloc_huge_page_by_cma(u32 numa_id, ka_page_t **pages, u64 pg_num, u32 gfp_mask)
{
    if (numa_id == U32_MAX) {
        return devmm_master_alloc_interleaving_huge_pages(pages, pg_num, gfp_mask);
    } else {
        return devmm_master_alloc_numa_huge_pages(numa_id, pages, pg_num, gfp_mask);
    }
}

static void devmm_master_free_one_huge_page_by_cma(u64 pa)
{
#ifndef EMU_ST
    unsigned long start_pfn = __phys_to_pfn(pa);
    unsigned long end_pfn = __phys_to_pfn(pa + SVM_MASTER_HUGE_PAGE_SIZE);
    unsigned long nr_pages = end_pfn - start_pfn;
    mutex_lock(&g_host_hgage_lock);
    free_contig_range(start_pfn, nr_pages);
    mutex_unlock(&g_host_hgage_lock);
#endif
}

static ka_page_t *devmm_master_alloc_one_giant_page_by_cma(u32 numa_id, u32 gfp_mask, int *last_numa)
{
    int numa_total;
    int i;
    int cur_numa;

    if (numa_id == U32_MAX) {
        cur_numa = devmm_master_get_next_numa(*last_numa);
    } else {
        cur_numa = (numa_id == 0) ? numa_node_id() : (numa_id - 1U);
    }
    if (cur_numa < 0 || cur_numa >= MAX_NUMNODES || !node_online(cur_numa)) {
        devmm_drv_err("invalid numa id. (numa:%u)\n", cur_numa);
        return NULL;
    }

    if (devmm_obmm_get(&devmm_svm->obmm_info) == 0) {
        numa_total = num_online_nodes();
        for (i = 0; i < numa_total; i++) {
            u64 pa = (u64)devmm_svm->obmm_info.obmm_alloc_func(cur_numa, SVM_MASTER_GIANT_PAGE_SIZE, 0);
            if (pa != 0) {
                devmm_drv_debug("alloc giant pages. (numa:%u; pfn:%pk)\n", cur_numa, (void *)PFN_DOWN(pa));
                *last_numa = cur_numa;
                return pfn_to_page(PFN_DOWN(pa));
            }
            if (numa_id > 0 && numa_id != U32_MAX) { // alloc in this numa only, if numa is designated
                break;
            }
            
            cur_numa = devmm_master_get_next_numa(cur_numa);
        }
        return NULL;
    } else {
        devmm_drv_err("not install obmm ko, can't alloc giant page.\n");
        return NULL;
    }
}

static void devmm_master_free_one_giant_page_by_cma(u64 pa)
{
#ifndef EMU_ST
    if (devmm_svm->obmm_info.obmm_free_func != NULL) {
        devmm_svm->obmm_info.obmm_free_func((void *)pa, 0);
        devmm_obmm_put(&devmm_svm->obmm_info);
    } else {
        devmm_drv_err("not install obmm ko, unknown error.\n");
    }
#endif
}

static bool devmm_is_support_fabric_page(void)
{
    static int record_val = -1;
    u32 dev_id = 0; // use 0 check
    u32 host_flag;
    int ret;

    if (record_val >= 0) {
        return (record_val > 0);
    }

    ret = devmm_get_host_phy_mach_flag(dev_id, &host_flag);
    if (ret != 0) {
        devmm_drv_err("get host flag failed!\n");
        return false;
    }
    record_val = (devmm_is_hccs_connect(dev_id) && devmm_is_hccs_vm_scene(dev_id, host_flag) == false);
    return (record_val > 0);
}

#else
static u64 devmm_master_alloc_huge_page_by_cma(u32 numa_id, ka_page_t **pages, u64 pg_num, u32 gfp_mask)
{
    return NULL;
}

static void devmm_master_free_one_huge_page_by_cma(u64 pa)
{
}

static ka_page_t *devmm_master_alloc_one_giant_page_by_cma(u32 numa_id, u32 gfp_mask, int *last_numa)
{
    return NULL;
}

static void devmm_master_free_one_giant_page_by_cma(u64 pa)
{
}

static bool devmm_is_support_fabric_page(void)
{
    return false;
}

#endif
#endif

static void devmm_master_put_one_huge_pages(ka_page_t *page)
{
#ifndef EMU_ST
    put_page(page);
#else
    __ka_mm_free_pages(page, ka_mm_get_order(SVM_MASTER_HUGE_PAGE_SIZE));
#endif
}

static void devmm_master_free_one_huge_pages(struct devmm_phy_addr_attr *attr, ka_page_t *page)
{
#if (defined CFG_SOC_PLATFORM_CLOUD_V2) && (defined __aarch64__)
    u64 pa = page_to_phys(page);
    if (attr->mem_type == MEM_P2P_DDR_TYPE && devmm_is_support_fabric_page()) {
        devmm_master_free_one_huge_page_by_cma(pa);
    } else {
        devmm_master_put_one_huge_pages(page);
    }
#else
    devmm_master_put_one_huge_pages(page);
#endif
}

void devmm_master_free_huge_pages(struct devmm_phy_addr_attr *attr, ka_page_t **pages, u64 pg_num)
{
    u32 stamp = (u32)ka_jiffies;
    u64 i;

    for (i = 0; i < pg_num; i++) {
        devmm_master_free_one_huge_pages(attr, pages[i]);
        pages[i] = NULL;
        devmm_try_cond_resched(&stamp);
    }
}

static u64 devmm_master_alloc_normal_huge_pages(ka_page_t **pages, u64 pg_num, u32 gfp_mask)
{
    u32 stamp = (u32)ka_jiffies;
    u64 i;

    for (i = 0; i < pg_num; i++) {
        pages[i] = __ka_alloc_pages_node(KA_NUMA_NO_NODE, gfp_mask, ka_mm_get_order(SVM_MASTER_HUGE_PAGE_SIZE));
        if (pages[i] == NULL) {
            return i;
        }
#ifndef EMU_ST
        (void)memset_s(page_address(pages[i]), SVM_MASTER_HUGE_PAGE_SIZE, 0, SVM_MASTER_HUGE_PAGE_SIZE);
#endif
        devmm_try_cond_resched(&stamp);
    }
    return i;
}

int devmm_master_alloc_huge_pages(struct devmm_phy_addr_attr *attr, ka_page_t **pages, u64 pg_num)
{
    u64 alloced;
    u32 gfp_mask = devmm_get_alloc_mask(true);
#if (defined CFG_SOC_PLATFORM_CLOUD_V2) && (defined __aarch64__)
    if (attr->mem_type == MEM_P2P_DDR_TYPE && devmm_is_support_fabric_page()) {
        alloced = devmm_master_alloc_huge_page_by_cma(attr->numa_id, pages, pg_num, gfp_mask);
    } else {
        alloced = devmm_master_alloc_normal_huge_pages(pages, pg_num, gfp_mask);
    }
#else
    alloced = devmm_master_alloc_normal_huge_pages(pages, pg_num, gfp_mask);
#endif
    if (alloced != pg_num) {
        devmm_master_free_huge_pages(attr, pages, alloced);
        return -ENOMEM;
    }
    return 0;
}

static ka_page_t *devmm_master_alloc_one_giant_pages(struct devmm_phy_addr_attr *attr, u32 gfp_mask, int *last_numa)
{
#if (defined CFG_SOC_PLATFORM_CLOUD_V2) && (defined __aarch64__)
    if (attr->mem_type == MEM_P2P_DDR_TYPE && devmm_is_support_fabric_page()) {
        return devmm_master_alloc_one_giant_page_by_cma(attr->numa_id, gfp_mask, last_numa);
    } else {
        return NULL;
    }
#else
#ifndef EMU_ST
    return NULL;
#else
    return __ka_alloc_pages_node(KA_NUMA_NO_NODE, gfp_mask, ka_mm_get_order(SVM_MASTER_GIANT_PAGE_SIZE));
#endif
#endif
}

static void devmm_master_free_one_giant_pages(struct devmm_phy_addr_attr *attr, ka_page_t *page)
{
#if (defined CFG_SOC_PLATFORM_CLOUD_V2) && (defined __aarch64__)
    u64 pa = page_to_phys(page);
    if (attr->mem_type == MEM_P2P_DDR_TYPE && devmm_is_support_fabric_page()) {
        devmm_master_free_one_giant_page_by_cma(pa);
    }
#else
#ifdef EMU_ST
        __ka_mm_free_pages(page, ka_mm_get_order(SVM_MASTER_GIANT_PAGE_SIZE));
#endif
#endif
}

void devmm_master_free_giant_pages(struct devmm_phy_addr_attr *attr, ka_page_t **pages, u64 pg_num)
{
    u32 stamp = (u32)ka_jiffies;
    u64 i;

    for (i = 0; i < pg_num; i++) {
        devmm_master_free_one_giant_pages(attr, pages[i]);
        pages[i] = NULL;
        devmm_try_cond_resched(&stamp);
    }
}

int devmm_master_alloc_giant_pages(struct devmm_phy_addr_attr *attr, ka_page_t **pages, u64 pg_num)
{
    u32 stamp = (u32)ka_jiffies;
    int last_numa = MAX_NUMNODES;
    u64 i;
    for (i = 0; i < pg_num; i++) {
        pages[i] = devmm_master_alloc_one_giant_pages(attr, devmm_get_alloc_mask(true), &last_numa);
        if (pages[i] == NULL) {
#ifndef EMU_ST
            devmm_master_free_giant_pages(attr, pages, i);
            return -ENOMEM;
#endif
        }
#ifndef EMU_ST
        (void)memset_s(page_address(pages[i]), SVM_MASTER_GIANT_PAGE_SIZE, 0, SVM_MASTER_GIANT_PAGE_SIZE);
#endif
        devmm_try_cond_resched(&stamp);
    }
    return 0;
}