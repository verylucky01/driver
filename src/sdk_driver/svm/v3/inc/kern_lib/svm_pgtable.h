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
#ifndef SVM_PGTABLE_H
#define SVM_PGTABLE_H

#include "ka_common_pub.h"
#include "ka_memory_pub.h"

#include "framework_vma.h"
#include "svm_addr_desc.h"
#include "svm_gfp.h"

ka_page_t *svm_pa_to_page(u64 pa);
int svm_va_to_pa(ka_vm_area_struct_t *vma, u64 va, u64 *pa, u64 *page_size);
u64 svm_va_to_page_size(u64 tgid, u64 va);
u64 svm_task_va_to_page_size(ka_task_struct_t *task, u64 va);
u64 svm_page_size_to_page_shift(u64 page_size);

struct svm_pgtlb_attr {
    bool is_noncache;
    bool is_rdonly;
    bool is_writecombine;
    u64 page_size;
};

static inline void svm_pgtlb_attr_packet(struct svm_pgtlb_attr *attr, bool is_noncache, bool is_rdonly,
    bool is_writecombine, u64 page_size)
{
    attr->is_noncache = is_noncache;
    attr->is_rdonly = is_rdonly;
    attr->is_writecombine = is_writecombine;
    attr->page_size = page_size;
}

int svm_remap_pages(ka_vm_area_struct_t *vma, u64 va, ka_page_t **pages, u64 page_num,
    struct svm_pgtlb_attr *pgtlb_attr);
int svm_remap_phys(ka_vm_area_struct_t *vma, u64 va, struct svm_pa_seg pa_seg[], u64 seg_num,
    struct svm_pgtlb_attr *pgtlb_attr);
/* remap pages and phys all are used svm_unmap_addr to unmap */
int svm_unmap_addr(ka_vm_area_struct_t *vma, u64 va, u64 size, u64 page_size);
/* return query size, query local page with same pagesize */
u64 svm_query_pages(ka_vm_area_struct_t *vma, u64 va, u64 size, ka_page_t **pages, u64 *page_num);
/* return query size, query pa is all local mem or peer mem, not mixed */
u64 svm_query_phys(ka_vm_area_struct_t *vma, u64 va, u64 size, struct svm_pa_seg pa_seg[], u64 *seg_num);

static inline u64 svm_query_pages_by_task(ka_task_struct_t *task,
    u64 va, u64 size, ka_page_t **pages, u64 *page_num)
{
    ka_vm_area_struct_t *vma = ka_mm_find_vma(ka_task_get_mm(task), va);
    if ((vma == NULL) || (svm_check_vma(vma, va, size) != 0)) {
        *page_num = 0;
        return 0;
    }

    return svm_query_pages(vma, va, size, pages, page_num);
}

static inline u64 smm_query_phys_by_task(ka_task_struct_t *task,
    u64 va, u64 size, struct svm_pa_seg pa_seg[], u64 *seg_num)
{
    ka_vm_area_struct_t *vma = ka_mm_find_vma(ka_task_get_mm(task), va);
    if (vma == NULL) {
        *seg_num = 0;
        return 0;
    }

    return svm_query_phys(vma, va, size, pa_seg, seg_num);
}

/* page table ops */
struct svm_page_table_ops {
    int (*remap)(ka_vm_area_struct_t *vma, u64 va, u64 pa, u64 page_num, ka_mm_pgprot_t pg_prot);
    void (*unmap)(ka_vm_area_struct_t *vma, u64 va, u64 page_num);
};

void svm_register_page_table_ops(enum svm_page_granularity gran, const struct svm_page_table_ops *ops);
void svm_get_page_table_ops(enum svm_page_granularity gran, struct svm_page_table_ops **ops);

/* page table externed ops */
struct svm_page_table_externed_ops {
    void (*pre_remap)(ka_vm_area_struct_t *vma, u64 va, u64 size);
    void (*remap_cancle)(ka_vm_area_struct_t *vma, u64 va, u64 size);
    void (*post_remap)(ka_vm_area_struct_t *vma, u64 va, u64 size);
    void (*post_unmap)(ka_vm_area_struct_t *vma, u64 va, u64 size);
    int (*pte_to_pfn)(ka_pte_t *pte, u64 *pfn);
};

void svm_register_page_table_externed_ops(const struct svm_page_table_externed_ops *ops);

int svm_pte_to_pfn(ka_pte_t *pte, u64 *pfn);

#endif
