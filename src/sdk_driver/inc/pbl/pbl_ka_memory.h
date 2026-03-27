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
#ifndef PBL_KA_MEMORY_H
#define PBL_KA_MEMORY_H

#include <linux/types.h>
#include "ka_base_pub.h"
#include "ka_memory_pub.h"
#include "ka_common_pub.h"

enum ka_mem_node_type {
    KA_MEM_NODE_TYPE_OS,
    KA_MEM_NODE_TYPE_OTHER,
    KA_MEM_NODE_TYPE_MAX
};

enum ka_sub_module_type {
    KA_SUB_MODULE_TYPE_0,
    KA_SUB_MODULE_TYPE_1,
    KA_SUB_MODULE_TYPE_2,
    KA_SUB_MODULE_TYPE_3,
    KA_SUB_MODULE_TYPE_4,
    KA_SUB_MODULE_TYPE_MAX
};

#define KA_MODULE_TYPE_BIT 16U
#define KA_ALLOC_MEM_BY_HUGE_PAGE 0
#define KA_ALLOC_MEM_BY_PAGE_NODE 1

static inline void *ka_vmalloc_ex(size_t size, ka_gfp_t gfp_mask, ka_pgprot_t prot)
{
    return __ka_mm_vmalloc(size, gfp_mask, prot);
}

#ifndef CFG_FEATURE_KA_ALLOC_INTERFACE
#define ka_get_module_id(module_type, sub_module_type)

#define ka_kmalloc(size, flags, module_id) ka_mm_kmalloc(size, flags)
#define ka_kzalloc(size, flags, module_id) ka_mm_kzalloc(size, flags)
#define ka_kcalloc(n, size, flags, module_id) ka_mm_kcalloc(n, size, flags)
#define ka_kzalloc_node(size, flags, node, module_id) ka_mm_kzalloc_node(size, flags, node)
#define ka_kfree(addr, module_id) ka_mm_kfree(addr)

#define ka_vzalloc(size, module_id) ka_mm_vzalloc(size)
#define __ka_vmalloc(size, gfp_mask, prot, module_id) ka_vmalloc_ex(size, gfp_mask, prot)
#define ka_vfree(addr, module_id) ka_mm_vfree(addr)

#define ka_get_free_pages(gfp_mask, order, module_id) ka_mm_get_free_pages(gfp_mask, order)
#define ka_alloc_pages(gfp_mask, order, module_id) ka_mm_alloc_pages(gfp_mask, order)
#define ka_free_pages(addr, order, module_id) ka_mm_free_pages(addr, order)

#define ka_alloc_pages_exact(size, gfp_mask, module_id) ka_mm_alloc_pages_exact(size, gfp_mask)
#define ka_free_pages_exact(virt, size, module_id) ka_mm_free_pages_exact(virt, size)

#define ka_alloc_pages_node(nid, gfp_mask, order, module_id) ka_mm_alloc_pages_node(nid, gfp_mask, order)
#define __ka_alloc_pages_node(nid, gfp_mask, order) ka_mm_alloc_pages_node(nid, gfp_mask, order)
#define __ka_free_pages(page, order, module_id) __ka_mm_free_pages(page, order)

#define ka_kvzalloc(size, flags, module_id) ka_mm_kvzalloc(size, flags)
#define ka_kvmalloc_node(size, flags, node, module_id) ka_mm_kvmalloc_node(size, flags, node)
#define ka_kvmalloc(size, flags, module_id) ka_mm_kvmalloc(size, flags)
#define ka_kvfree(addr, module_id) ka_mm_kvfree(addr)

#define ka_dma_alloc_coherent(dev, size, dma_handle, gfp, module_id) ka_mm_dma_alloc_coherent(dev, size, dma_handle, gfp)
#define ka_dma_free_coherent(dev, size, cpu_addr, dma_handle, module_id) ka_mm_dma_free_coherent(dev, size, cpu_addr, dma_handle)

#define ka_hugetlb_alloc_hugepage(nid, flag, module_id) hugetlb_alloc_hugepage(nid, flag)
#define ka_hugetlb_free_hugepage(page, module_id) ka_mm_put_page(page)

#else

/*
 * module_type: devdrv_module_type; sub_module_type: ka_sub_module_type
 */
#define ka_get_module_id(module_type, sub_module_type) ((module_type << KA_MODULE_TYPE_BIT) | sub_module_type)

/*
 * kmalloc -- ka_kmalloc -- ka_kfree
 * kzalloc -- ka_kzalloc -- ka_kfree
 * kcalloc -- ka_kcalloc -- ka_kfree
 */

void *ka_kmalloc(size_t size, ka_gfp_t flags, unsigned int module_id);
void *ka_kzalloc(size_t size, ka_gfp_t flags, unsigned int module_id);
void *ka_kcalloc(unsigned long n, size_t size, ka_gfp_t flags, unsigned int module_id);
void *ka_kzalloc_node(size_t size, ka_gfp_t flags, unsigned int node, unsigned int module_id);
void ka_kfree(const void *addr, unsigned int module_id);

/*
 * __vmalloc && vmalloc && ka_vmalloc -- __ka_vmalloc --   ka_vfree
 * vzalloc                            -- ka_vzalloc   --   ka_vfree
 */
void *ka_vzalloc(size_t size, unsigned int module_id);
void *__ka_vmalloc(size_t size, ka_gfp_t gfp_mask, ka_pgprot_t prot, unsigned int module_id);
void ka_vfree(const void *addr, unsigned int module_id);

/*
 * __get_free_pages -- ka_get_free_pages -- ka_free_pages
 * alloc_pages --      ka_alloc_pages --    ka_free_pages
 */
unsigned long ka_get_free_pages(ka_gfp_t gfp_mask, unsigned int order, unsigned int module_id);
ka_page_t *ka_alloc_pages(ka_gfp_t gfp_mask, unsigned int order, unsigned int module_id);
void ka_free_pages(unsigned long addr, unsigned int order, unsigned int module_id);

/*
 * alloc_pages_exact   --   ka_alloc_pages_exact     -- ka_free_pages_exact
 */
void *ka_alloc_pages_exact(size_t size, ka_gfp_t gfp_mask, unsigned int module_id);
void ka_free_pages_exact(void *virt, size_t size, unsigned int module_id);

/*
 * alloc_pages_node -- __ka_alloc_pages_node -- ka_alloc_pages_node -- __ka_free_pages
 */
ka_page_t *ka_alloc_pages_node(int nid, ka_gfp_t gfp_mask, unsigned int order, unsigned int module_id);
ka_page_t *__ka_alloc_pages_node(int nid, ka_gfp_t gfp_mask, unsigned int order);
void __ka_free_pages(ka_page_t *page, unsigned int order, unsigned int module_id);

/*
 * kvzalloc --      ka_kvzalloc --      ka_kvfree
 * kvmalloc_node -- ka_kvmalloc_node -- ka_kvfree
 * kvmalloc --      ka_kvmalloc --      ka_kvfree
 */
void *ka_kvzalloc(size_t size, ka_gfp_t flags, unsigned int module_id);
void *ka_kvmalloc_node(size_t size, ka_gfp_t flags, int node, unsigned int module_id);
void *ka_kvmalloc(size_t size, ka_gfp_t flags, unsigned int module_id);
void ka_kvfree(const void *addr, unsigned int module_id);

/*
 * dma_alloc_coherent --      ka_dma_alloc_coherent --      ka_dma_free_coherent
 */
void *ka_dma_alloc_coherent(struct device *dev, size_t size, dma_addr_t *dma_handle,
    ka_gfp_t gfp, unsigned int module_id);
void ka_dma_free_coherent(struct device *dev, size_t size, void *cpu_addr, dma_addr_t dma_handle,
    unsigned int module_id);

/*
 * hugetlb_alloc_hugepage --      ka_hugetlb_alloc_hugepage --   ka_hugetlb_free_hugepage
 * alloc_hugetlb_folio_size    -- ka_alloc_hugetlb_folio_size -- ka_hugetlb_free_hugepage
 */
ka_page_t *ka_hugetlb_alloc_hugepage(int nid, int flag, unsigned int module_id);
void ka_hugetlb_free_hugepage(ka_page_t *page, unsigned int module_id);


#endif

void ka_free_single_page(ka_page_t *page_addr, unsigned int order, int alloc_flag, unsigned int module_type,
                         unsigned int level);
ka_page_t *ka_alloc_hugetlb(int nid, unsigned int size, int flag, unsigned int module_type,
                              unsigned int level);
ka_page_t *ka_alloc_hugepage(int nid, ka_gfp_t gfp_mask, unsigned int order, int *alloc_flag, int flag,
                               unsigned int module_type, unsigned int level);
#endif
