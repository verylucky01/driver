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

#include <linux/slab.h>
#include <linux/swap.h>
#include <linux/swapops.h>
#include <linux/compiler.h>

#include "securec.h"
#include "ka_memory_pub.h"
#include "ka_mem.h"
#if !defined(EMU_ST)
#ifndef DRV_HOST
#include "linux/share_pool.h"
#endif
#endif

#pragma message(PRINT_MACRO(LINUX_VERSION_CODE))

void ka_si_meminfo(struct ka_sysinfo *val)
{
    struct sysinfo sys_info = {0};
    si_meminfo(&sys_info);
    val->totalram = sys_info.totalram;
    val->freeram = sys_info.freeram;
    val->sharedram = sys_info.sharedram;
}
EXPORT_SYMBOL(ka_si_meminfo);

ka_rw_semaphore_t *ka_mm_get_mmap_sem(ka_mm_struct_t *mm)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 8, 0)
    return &mm->mmap_lock;
#else
    return &mm->mmap_sem;
#endif
}
EXPORT_SYMBOL(ka_mm_get_mmap_sem);

long ka_mm_get_user_pages_remote(ka_task_struct_t *tsk, ka_mm_struct_t *mm,
    unsigned long long va, int write, unsigned int num, ka_page_t **pages)
{
    long got_num;
    int locked;

    locked = 1;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 5, 0)
    got_num = pin_user_pages_remote(mm, va, num, (write != 0) ? FOLL_WRITE : 0, pages, NULL);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
    got_num = pin_user_pages_remote(mm, va, num, (write != 0) ? FOLL_WRITE : 0, pages, NULL, NULL);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(5, 9, 0)
    got_num = get_user_pages_remote(mm, va, num, (write != 0) ? FOLL_WRITE : 0, pages, NULL, NULL);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(4, 10, 0)
    got_num = get_user_pages_remote(tsk, mm, va, num, (write != 0) ? FOLL_WRITE : 0, pages, NULL, NULL);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(4, 9, 0)
    got_num = get_user_pages_remote(tsk, mm, va, num, (write != 0) ? FOLL_WRITE : 0, pages, NULL);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(4, 6, 0)
    got_num = get_user_pages_remote(tsk, mm, va, num, (write != 0) ? FOLL_WRITE : 0, 0, pages, NULL);
#else
    got_num = get_user_pages_locked(tsk, mm, va, num, (write != 0) ? FOLL_WRITE : 0, 0, pages, &locked);
#endif
    return got_num;
}
EXPORT_SYMBOL(ka_mm_get_user_pages_remote);

void *ka_mm_page_to_virt(ka_page_t *page)
{
#ifdef page_to_virt
    return page_to_virt(page);
#else
    return __va(((phys_addr_t)(page_to_pfn(page)) << PAGE_SHIFT));
#endif
}
EXPORT_SYMBOL(ka_mm_page_to_virt);

unsigned long ka_mm_get_vm_flags(ka_vm_area_struct_t *vma)
{
    return vma->vm_flags;
}
EXPORT_SYMBOL(ka_mm_get_vm_flags);

void ka_mm_set_vm_flags(ka_vm_area_struct_t *vma, unsigned long flags)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6, 3, 0))
    vm_flags_set(vma, (vm_flags_t)flags);
#else
    vma->vm_flags |= flags;
#endif
}
EXPORT_SYMBOL(ka_mm_set_vm_flags);

void ka_mm_vm_flags_clear(ka_vm_area_struct_t *vma, unsigned long flags)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 3, 0)
    vm_flags_clear(vma, (vm_flags_t)flags);
#else
    vma->vm_flags &= ~flags;
#endif
}
EXPORT_SYMBOL(ka_mm_vm_flags_clear);

void ka_mm_set_vm_private_data(ka_vm_area_struct_t *vma, void *private_data)
{
    vma->vm_private_data = private_data;
}
EXPORT_SYMBOL(ka_mm_set_vm_private_data);

void *ka_mm_get_vm_private_data(ka_vm_area_struct_t *vma)
{
    return vma->vm_private_data;
}
EXPORT_SYMBOL(ka_mm_get_vm_private_data);

void ka_mm_set_vm_start(ka_vm_area_struct_t *vma, unsigned long start)
{
    vma->vm_start = start;
}
EXPORT_SYMBOL(ka_mm_set_vm_start);

unsigned long ka_mm_get_vm_start(ka_vm_area_struct_t *vma)
{
    return vma->vm_start;
}
EXPORT_SYMBOL(ka_mm_get_vm_start);

void ka_mm_set_vm_end(ka_vm_area_struct_t *vma, unsigned long end)
{
    vma->vm_end = end;
}
EXPORT_SYMBOL(ka_mm_set_vm_end);

unsigned long ka_mm_get_vm_end(ka_vm_area_struct_t *vma)
{
    return vma->vm_end;
}
EXPORT_SYMBOL(ka_mm_get_vm_end);

void ka_mm_set_vm_pgprot(ka_vm_area_struct_t *vma)
{
    vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
}
EXPORT_SYMBOL(ka_mm_set_vm_pgprot);

ka_mm_pgprot_t *ka_mm_get_vm_pgprot(ka_vm_area_struct_t *vma)
{
    return &(vma->vm_page_prot);
}
EXPORT_SYMBOL(ka_mm_get_vm_pgprot);

int ka_mm_zap_vma_ptes(ka_vm_area_struct_t *vma, unsigned long address, unsigned long size)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 18, 0)
    zap_vma_ptes(vma, address, size);
    return 0;
#else
    return zap_vma_ptes(vma, address, size);
#endif
}
EXPORT_SYMBOL(ka_mm_zap_vma_ptes);

void ka_mm_set_vm_ops(ka_vm_area_struct_t *vma, const ka_vm_operations_struct_t *vm_ops)
{
    vma->vm_ops = vm_ops;
}
EXPORT_SYMBOL(ka_mm_set_vm_ops);

const ka_vm_operations_struct_t *ka_mm_get_vm_ops(ka_vm_area_struct_t *vma)
{
    return vma->vm_ops;
}
EXPORT_SYMBOL(ka_mm_get_vm_ops);

void ka_mm_mmget(ka_mm_struct_t *mm)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0))
    mmget(mm);
#else
    atomic_inc(&mm->mm_users);
#endif
}
EXPORT_SYMBOL(ka_mm_mmget);

int ka_mm_pin_user_pages_fast(unsigned long start, int nr_pages, unsigned int gup_flags, ka_page_t **pages)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
    return pin_user_pages_fast(start, nr_pages, gup_flags | KA_FOLL_LONGTERM, pages);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(5, 2, 0)
    return get_user_pages_fast(start, nr_pages, gup_flags | KA_FOLL_LONGTERM, pages);
#else
    int write = (int)gup_flags;
    return get_user_pages_fast(start, nr_pages, write, pages);
#endif
}
EXPORT_SYMBOL(ka_mm_pin_user_pages_fast);

void ka_mm_unpin_user_page(ka_page_t *page)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
    unpin_user_page(page);
#else
    put_page(page);
#endif
}
EXPORT_SYMBOL(ka_mm_unpin_user_page);

void ka_mm_set_vm_fault_host(ka_vm_operations_struct_t *ops_managed, vmf_fault_func vmf_func, vm_fault_func vm_func)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
    ops_managed->fault = vmf_func;
#else
    ops_managed->fault = vm_func;
#endif
}
EXPORT_SYMBOL_GPL(ka_mm_set_vm_fault_host);

void ka_mm_set_vm_mremap(ka_vm_operations_struct_t *ops_managed, int (*mremap_func)(ka_vm_area_struct_t *))
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 3, 0)
    ops_managed->mremap = mremap_func;
#endif
    return;
}
EXPORT_SYMBOL_GPL(ka_mm_set_vm_mremap);

#ifndef EMU_ST
bool ka_mm_is_svm_addr(struct vm_area_struct *vma, u64 addr) {
#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 2, 0)
#ifndef DRV_HOST
    if (mg_is_sharepool_addr(addr)) {
        return false;
    }
#endif
#endif
    return (((vma->vm_flags) & VM_PFNMAP) != 0);
}
EXPORT_SYMBOL_GPL(ka_mm_is_svm_addr);
#endif

#ifndef EMU_ST
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 5, 0)
static ka_pte_t *ka_mm_inner_pte_offset_map(ka_pmd_t *pmd, unsigned long addr, ka_pmd_t *pmdvalp)
{
    ka_pmd_t pmdval;

    /* rcu_read_lock() to be added later */
    pmdval = ka_mm_pmdp_get_lockless(pmd);
    if (pmdvalp) {
        *pmdvalp = pmdval;
    }
    if (unlikely(ka_mm_pmd_none(pmdval) || ka_mm_is_pmd_migration_entry(pmdval))) {
        goto nomap;
    }
    if (unlikely(ka_mm_pmd_trans_huge(pmdval) || ka_mm_pmd_devmap(pmdval))) {
        goto nomap;
    }
    if (unlikely(ka_mm_pmd_bad(pmdval))) {
        ka_mm_pmd_ERROR(*pmd);
        ka_mm_pmd_clear(pmd);
        goto nomap;
    }
    return __ka_mm_pte_map(&pmdval, addr);

nomap:
    /* rcu_read_unlock() to be added later */
    return NULL;
}
#endif

/**
 * @brief get the page table entry of the va
 * @attention
 * kpg_size=KA_MM_PAGE_SIZE(4K) -> pte
 * kpg_size=KA_HPAGE_SIZE(2M) -> pmd
 * kpg_size=PUD_SIZE(1G) -> pud
 * @param [in] vma: ka_vm_area_struct_t
 * @param [in] va: va
 * @param [out] kpg_size: real page size from kernel
 * @return NULL for fail, others for success, means pte pointer
 */
void *ka_mm_get_pte(const ka_vm_area_struct_t *vma, u64 va, u64 *kpg_size)
{
    ka_pgd_t *pgd = NULL;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
    p4d_t *p4d = NULL;
#endif
    ka_pud_t *pud = NULL;
    ka_pmd_t *pmd = NULL;
    ka_pte_t *pte = NULL;

    if ((vma == NULL) || (vma->vm_mm == NULL) || (kpg_size == NULL)) {
        return NULL;
    }
    /* too much log, not print */
    pgd = ka_mm_pgd_offset(vma->vm_mm, va);
    if (PXD_JUDGE(pgd) != 0) {
        return NULL;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
    p4d = ka_mm_p4d_offset(pgd, va);
    if (PXD_JUDGE(p4d) != 0) {
        return NULL;
    }

    /* if kernel version is above 4.11.0,then 5 level pt arrived.
    pud_offset(pgd,va) changed to pud_offset(p4d,va) for x86
    but not changed in arm64 */
    pud = ka_mm_pud_offset(p4d, va);
#else
    pud = ka_mm_pud_offset(pgd, va);
#endif
    if (PUD_GIANT(pud) != 0) {
        *kpg_size = KA_MM_GIANT_PAGE_SIZE;
        return pud;
    }
    if (PXD_JUDGE(pud) != 0) {
        return NULL;
    }

    pmd = ka_mm_pmd_offset(pud, va);
    /* huge page pmd can not judge bad flag */
    if (PMD_HUGE(pmd) != 0) {
        *kpg_size = KA_HPAGE_SIZE;
        return pmd;
    }
    if (PMD_JUDGE(pmd) != 0) {
        return NULL;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 5, 0)
    pte = ka_mm_inner_pte_offset_map(pmd, va, NULL);
#else
    pte = ka_mm_pte_offset_map(pmd, va);
#endif
    if ((pte == NULL) || (ka_mm_pte_none(*pte) != 0) || (ka_mm_pte_present(*pte) == 0)) {
        return NULL;
    }
    *kpg_size = KA_MM_PAGE_SIZE;
    return pte;
}
EXPORT_SYMBOL_GPL(ka_mm_get_pte);

ka_pmd_t *ka_mm_get_va_to_pmd(const ka_vm_area_struct_t *vma, unsigned long va) /* To be deleted */
{
    ka_pgd_t *pgd = NULL;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
    p4d_t *p4d = NULL;
#endif
    ka_pud_t *pud = NULL;
    ka_pmd_t *pmd = NULL;

    if ((vma == NULL) || (vma->vm_mm == NULL)) {
        return NULL;
    }
    /* too much log, not print */
    pgd = ka_mm_pgd_offset(vma->vm_mm, va);
    if (PXD_JUDGE(pgd)) {
        return NULL;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
    p4d = ka_mm_p4d_offset(pgd, va);
    if (PXD_JUDGE(p4d) != 0) {
        return NULL;
    }

    /* if kernel version is above 4.11.0,then 5 level pt arrived.
    pud_offset(pgd,va) changed to pud_offset(p4d,va) for x86
    but not changed in arm64 */
    pud = ka_mm_pud_offset(p4d, va);
    if (PXD_JUDGE(pud) != 0) {
        return NULL;
    }
#else
    pud = ka_mm_pud_offset(pgd, va);
    if (PXD_JUDGE(pud) != 0) {
        return NULL;
    }
#endif

    pmd = ka_mm_pmd_offset(pud, va);
    return pmd;
}
EXPORT_SYMBOL_GPL(ka_mm_get_va_to_pmd);

static int ka_mm_va_to_pa_pmd_range(ka_pmd_t *pmd, u64 start, u64 end, u64 *pas, u64 *num)
{
    ka_pte_t *pte = NULL;
    u64 got_num = 0;
    u64 va = start;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 5, 0)
    pte = ka_mm_inner_pte_offset_map(pmd, va, NULL);
#else
    pte = ka_mm_pte_offset_map(pmd, va);
#endif

    for (; va != end; pte++, va += KA_MM_PAGE_SIZE) {
        if ((ka_mm_pte_none(*pte) != 0) || (ka_mm_pte_present(*pte) == 0)) {
            return -ERANGE;
        }

        pas[got_num] = KA_MM_PFN_PHYS(ka_mm_pte_pfn(*pte));
        got_num++;
    }

    *num = got_num;
    return 0;
}

static int ka_mm_va_to_pa_pud_range(ka_pud_t *pud, u64 start, u64 end, u64 *pas, u64 *num)
{
    ka_pmd_t *pmd = NULL;
    u64 got_num = 0;
    u64 va = start;
    u64 next;

    pmd = ka_mm_pmd_offset(pud, va);
    for (; va != end; pmd++, va = next) {
        int ret;
        u64 n;

        if (PXD_JUDGE(pmd) != 0) {
            return -EDOM;
        }

        next = ka_mm_pmd_addr_end(va, end);
        ret = ka_mm_va_to_pa_pmd_range(pmd, va, next, &pas[got_num], &n);
        if (ret != 0) {
            return ret;
        }
        got_num += n;
    }

    *num = got_num;
    return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
static int ka_mm_va_to_pa_p4d_range(p4d_t *p4d, u64 start, u64 end, u64 *pas, u64 *num)
{
    ka_pud_t *pud = NULL;
    u64 got_num = 0;
    u64 va = start;
    u64 next;

    pud = ka_mm_pud_offset(p4d, va);
    for (; va != end; pud++, va = next) {
        int ret;
        u64 n = 0;

        if (PXD_JUDGE(pud) != 0) {
            return -EDOM;
        }

        next = pud_addr_end(va, end);
        ret = ka_mm_va_to_pa_pud_range(pud, va, next, &pas[got_num], &n);
        if (ret != 0) {
            return ret;
        }
        got_num += n;
    }

    *num = got_num;
    return 0;
}

int ka_mm_va_to_pa_pgd_range(ka_pgd_t *pgd, u64 start, u64 end, u64 *pas, u64 *num)
{
    p4d_t *p4d = NULL;
    u64 got_num = 0;
    u64 va = start;
    u64 next;

    if (pgd == NULL || pas == NULL || num == NULL) {
        return -EINVAL;
    }
    p4d = ka_mm_p4d_offset(pgd, va);
    for (; va != end; p4d++, va = next) {
        int ret;
        u64 n;

        if (PXD_JUDGE(p4d) != 0) {
            return -EDOM;
        }

        next = p4d_addr_end(va, end);
        ret = ka_mm_va_to_pa_p4d_range(p4d, va, next, &pas[got_num], &n);
        if (ret != 0) {
            return ret;
        }
        got_num += n;
    }

    *num = got_num;
    return 0;
}
#else
int ka_mm_va_to_pa_pgd_range(ka_pgd_t *pgd, u64 start, u64 end, u64 *pas, u64 *num)
{
    ka_pud_t *pud = NULL;
    u64 got_num = 0;
    u64 va = start;
    u64 next;

    if (pgd == NULL || pas == NULL || num == NULL) {
        return -EINVAL;
    }
    pud = ka_mm_pud_offset(pgd, va);
    for (; va != end; pud++, va = next) {
        int ret;
        u64 n;

        if (PXD_JUDGE(pud) != 0) {
            return -EDOM;
        }

        next = pud_addr_end(va, end);
        ret = ka_mm_va_to_pa_pud_range(pud, va, next, &pas[got_num], &n);
        if (ret != 0) {
            return ret;
        }
        got_num += n;
    }

    *num = got_num;
    return 0;
}
#endif
EXPORT_SYMBOL_GPL(ka_mm_va_to_pa_pgd_range);
#endif
