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
#include <linux/swap.h>
#include <linux/swapops.h>

#include "kernel_adapt_init.h"
#include "ka_system_pub.h"
#include "ka_memory_pub.h"
#include "ka_task_pub.h"
#include "ka_compiler_pub.h"
#include "ka_sched_pub.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 5, 0)
static unsigned long pmdp_get_lockless_start(void)
{
    return 0;
}

static void pmdp_get_lockless_end(unsigned long irqflags)
{
}

static ka_pte_t *_ka_pte_offset_map(ka_pmd_t *pmd, u64 addr, ka_pmd_t *pmdvalp)
{
    unsigned long irqflags;
    ka_pmd_t pmdval;

    rcu_read_lock();
    irqflags = pmdp_get_lockless_start();
    pmdval = ka_mm_pmdp_get_lockless(pmd);
    pmdp_get_lockless_end(irqflags);

    if (pmdvalp) {
        *pmdvalp = pmdval;
    }

    if (ka_unlikely(ka_mm_pmd_none(pmdval) || ka_mm_is_pmd_migration_entry(pmdval))) {
        goto nomap;
    }
    if (ka_unlikely(ka_mm_pmd_trans_huge(pmdval) || ka_mm_pmd_devmap(pmdval))) {
        goto nomap;
    }
    if (ka_unlikely(ka_mm_pmd_bad(pmdval))) {
        ka_mm_pmd_ERROR(*pmd);
        ka_mm_pmd_clear(pmd);
        goto nomap;
    }
    return __ka_mm_pte_map(&pmdval, addr);

nomap:
    rcu_read_unlock();
    return NULL;
}

static ka_pte_t *_ka_pte_offset_map_lock(ka_mm_struct_t *mm, ka_pmd_t *pmd, u64 addr, ka_spinlock_t **ptlp)
{
    ka_spinlock_t *ptl;
    ka_pmd_t pmdval;
    ka_pte_t *pte;
again:
    pte = _ka_pte_offset_map(pmd, addr, &pmdval);
    if (unlikely(!pte)) {
        return pte;
    }

    ptl = ka_mm_pte_lockptr(mm, &pmdval);
    ka_task_spin_lock(ptl);
    if (likely(ka_mm_pmd_same(pmdval, ka_mm_pmdp_get_lockless(pmd)))) {
        *ptlp = ptl;
        return pte;
    }
    ka_mm_pte_unmap_unlock(pte, ptl);
    goto again;
}
#endif

static ka_pte_t *ka_pte_offset_map(ka_pmd_t *pmd, u64 addr)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 5, 0)
    return _ka_pte_offset_map(pmd, addr, NULL);
#else
    return ka_mm_pte_offset_map(pmd, addr);
#endif
}

static void ka_pte_unmap(ka_pte_t *pte)
{
    ka_mm_pte_unmap(pte);
}

static ka_pte_t *ka_pte_offset_map_lock(ka_mm_struct_t *mm, ka_pmd_t *pmd, u64 addr, ka_spinlock_t **ptlp)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 5, 0)
    return _ka_pte_offset_map_lock(mm, pmd, addr, ptlp);
#else
    return ka_mm_pte_offset_map_lock(mm, pmd, addr, ptlp);
#endif
}

static void ka_pte_unmap_unlock(ka_pte_t *pte, ka_spinlock_t *ptl)
{
    ka_mm_pte_unmap_unlock(pte, ptl);
}

static int ka_walk_pte_range(ka_pmd_t *pmd, u64 start, u64 end, struct ka_pgwalk *walk)
{
    unsigned long stamp = ka_jiffies;
    struct ka_pgwalk_ops *ops = walk->ops;
    ka_spinlock_t *ptl = NULL;
    ka_pte_t *pte = NULL;
    u64 next = start;
    int ret = 0;

    if (walk->pte_lock_flag == 0) {
        pte = ka_pte_offset_map(pmd, start);
    } else {
        pte = ka_pte_offset_map_lock(walk->vma->vm_mm, pmd, start, &ptl);
    }
    if (pte == NULL) {
        ka_err("Get pte failed.\n");
        return -EFAULT;
    }

    do {
        ka_try_cond_resched(&stamp);
        next = ka_base_min(next + KA_MM_PAGE_SIZE, end);
        if ((pte != NULL) && (ka_mm_pte_none(*pte) == 0) && (ka_mm_pte_present(*pte) != 0)) {
            if (ops->pte_entry != NULL) {
                ret = ops->pte_entry(pte, start, next, KA_FINAL_LEVEL, walk);
                if (ret != 0) {
                    break;
                }
            }
        } else {
            if (ops->pte_hole != NULL) {
                ret = ops->pte_hole(start, next, KA_FINAL_LEVEL, walk);
                if (ret != 0) {
                    break;
                }
            }
        }
        // cppcheck-suppress *
    } while (pte++, start = next, start != end);

    if (walk->pte_lock_flag == 0) {
        ka_pte_unmap(pte);
    } else {
        ka_pte_unmap_unlock(pte, ptl);
    }

    return ret;
}

static int ka_walk_pmd_range(ka_pud_t *pud, u64 start, u64 end, struct ka_pgwalk *walk)
{
    unsigned long stamp = ka_jiffies;
    struct ka_pgwalk_ops *ops = walk->ops;
    ka_pmd_t *pmd = NULL;
    u64 next;
    int ret = 0;

    pmd = ka_mm_pmd_offset(pud, start);
    do {
        ka_try_cond_resched(&stamp);
        next = ka_mm_pmd_addr_end(start, end);
        if (PMD_HUGE(pmd)) { /* is huge page */
            if (ops->pte_entry != NULL) {
                ret = ops->pte_entry((ka_pte_t *)pmd, start, next, KA_PMD_LEVEL, walk);
                if (ret != 0) {
                    break;
                }
            }
            continue;
        } else if (PXD_JUDGE(pmd)) {
            if (ops->pte_hole != NULL) {
                ret = ops->pte_hole(start, next, KA_PMD_LEVEL, walk);
                if (ret != 0) {
                    break;
                }
            }
            continue;
        }

        if (ops->pmd_entry != NULL) {
            ret = ops->pmd_entry(pmd, start, next, walk);
            if (ret != 0) {
                break;
            }
        }

        if ((ops->pte_entry != NULL) || (ops->pte_hole != NULL)) {
            ret = ka_walk_pte_range(pmd, start, next, walk);
            if (ret != 0) {
                break;
            }
        }
        // cppcheck-suppress *
    } while (pmd++, start = next, start != end);

    return ret;
}

static int _ka_walk_pud_range(ka_pud_t *pud, u64 start, u64 end, struct ka_pgwalk *walk)
{
    unsigned long stamp = ka_jiffies;
    struct ka_pgwalk_ops *ops = walk->ops;
    u64 next;
    int ret = 0;

    do {
        ka_try_cond_resched(&stamp);
        next = pud_addr_end(start, end);
        if (PUD_GIANT(pud)) { /* is giant page */
            if (ops->pte_entry != NULL) {
                ret = ops->pte_entry((ka_pte_t *)pud, start, next, KA_PUD_LEVEL, walk);
                if (ret != 0) {
                    break;
                }
            }
            continue;
        } else if (PXD_JUDGE(pud)) {
            if (ops->pte_hole != NULL) {
                ret = ops->pte_hole(start, next, KA_PUD_LEVEL, walk);
                if (ret != 0) {
                    break;
                }
            }
            continue;
        }

        if ((ops->pte_entry != NULL) || (ops->pte_hole != NULL) || (ops->pmd_entry != NULL)) {
            ret = ka_walk_pmd_range(pud, start, next, walk);
            if (ret != 0) {
                break;
            }
        }

        if (ops->pud_entry_post != NULL) {
            ret = ops->pud_entry_post(pud, start, next, walk);
            if (ret != 0) {
                break;
            }
        }

        // cppcheck-suppress *
    } while (pud++, start = next, start != end);

    return ret;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
static int ka_walk_pud_range(p4d_t *p4d, u64 start, u64 end, struct ka_pgwalk *walk)
{
    return _ka_walk_pud_range(ka_mm_pud_offset(p4d, start), start, end, walk);
}

static int ka_walk_p4d_range(ka_pgd_t *pgd, u64 start, u64 end, struct ka_pgwalk *walk)
{
    unsigned long stamp = ka_jiffies;
    struct ka_pgwalk_ops *ops = walk->ops;
    p4d_t *p4d = NULL;
    u64 next;
    int ret = 0;

    p4d = ka_mm_p4d_offset(pgd, start);
    do {
        ka_try_cond_resched(&stamp);
        next = p4d_addr_end(start, end);
        if (PXD_JUDGE(p4d)) {
            if (ops->pte_hole != NULL) {
                ret = ops->pte_hole(start, next, KA_P4D_LEVEL, walk);
                if (ret != 0) {
                    break;
                }
            }
            continue;
        }

        ret = ka_walk_pud_range(p4d, start, next, walk);
        if (ret != 0) {
            break;
        }
        // cppcheck-suppress *
    } while (p4d++, start = next, start != end);

    return ret;
}
#else
static int ka_walk_pud_range(ka_pgd_t *pgd, u64 start, u64 end, struct ka_pgwalk *walk)
{
    return _ka_walk_pud_range(ka_mm_pud_offset(pgd, start), start, end, walk);
}
#endif

static int ka_walk_pgd_range(u64 start, u64 end, struct ka_pgwalk *walk)
{
    unsigned long stamp = ka_jiffies;
    ka_vm_area_struct_t *vma = walk->vma;
    struct ka_pgwalk_ops *ops = walk->ops;
    ka_pgd_t *pgd = NULL;
    u64 next;
    int ret = 0;

    pgd = ka_mm_pgd_offset(vma->vm_mm, start);
    do {
        ka_try_cond_resched(&stamp);
        next = ka_mm_pgd_addr_end(start, end);
        if (PXD_JUDGE(pgd)) {
            if (ops->pte_hole != NULL) {
                ret = ops->pte_hole(start, next, KA_PGD_LEVEL, walk);
                if (ret != 0) {
                    break;
                }
            }
            continue;
        }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
        ret = ka_walk_p4d_range(pgd, start, next, walk);
#else
        ret = ka_walk_pud_range(pgd, start, next, walk);
#endif
        if (ret != 0) {
            break;
        }
        // cppcheck-suppress *
    } while (pgd++, start = next, start != end);

    return ret;
}

int ka_walk_page_range(ka_vm_area_struct_t *vma, u64 start, u64 end, struct ka_pgwalk_ops *ops, void *priv)
{
    struct ka_pgwalk walk = {
        .vma = vma,
        .ops = ops,
        .pte_lock_flag = 0,
        .priv = priv
    };

    if (start >= end) {
        return -EFAULT;
    }

    return ka_walk_pgd_range(start, end, &walk);
}
EXPORT_SYMBOL_GPL(ka_walk_page_range);

int ka_walk_page_range_pte_lock(ka_vm_area_struct_t *vma, u64 start, u64 end, struct ka_pgwalk_ops *ops, void *priv)
{
    struct ka_pgwalk walk = {
        .vma = vma,
        .ops = ops,
        .pte_lock_flag = 1,
        .priv = priv
    };

    if (start >= end) {
        return -EFAULT;
    }

    return ka_walk_pgd_range(start, end, &walk);
}
EXPORT_SYMBOL_GPL(ka_walk_page_range_pte_lock);