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

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
#include <linux/dma-map-ops.h>
#include <linux/vfio.h>
#elif ((LINUX_VERSION_CODE >= KERNEL_VERSION(4,19,36)) || (defined(DRV_UT)))
#include <linux/dma-noncoherent.h>
#include <linux/vfio.h>
#elif ((LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)) || (defined(DRV_UT)))
#include <linux/vfio.h>
#endif

#include <linux/kvm_host.h>
#include <linux/vmalloc.h>
#include <linux/iova.h>
#include <linux/dma-mapping.h>
#include "dvt.h"
#include "vfio_ops.h"
#include "dma_pool.h"
#include "dma_pool_map.h"

#define IOMMU_MAPPING_ERROR     0
#define DMA_BIT_MASK_WIDTH      32

enum iommu_dma_cookie_type {
    IOMMU_DMA_IOVA_COOKIE,
    IOMMU_DMA_MSI_COOKIE,
};

struct iommu_dma_cookie {
    enum iommu_dma_cookie_type    type;
    union {
        /* Full allocator for IOMMU_DMA_IOVA_COOKIE */
        struct iova_domain    iovad;
        /* Trivial linear page allocator for IOMMU_DMA_MSI_COOKIE */
        dma_addr_t        msi_iova;
    };
    struct list_head        msi_page_list;
    spinlock_t            msi_lock;

    /* Domain for flush queue callback; NULL if flush queue not in use */
    struct iommu_domain        *fq_domain;
};

#ifndef __ASM_DEVICE_H
STATIC bool is_dev_dma_coherent(struct device *dev)
{
        return true;
}

#else
#if ((LINUX_VERSION_CODE == KERNEL_VERSION(4,19,36)) || (LINUX_VERSION_CODE == KERNEL_VERSION(4,19,90)))
STATIC bool is_dev_dma_coherent(struct device *dev)
{
    return dev->archdata.dma_coherent;
}
#else
STATIC bool is_dev_dma_coherent(struct device *dev)
{
        return true;
}
#endif
#endif

STATIC int dev_get_prot(struct hw_vdavinci *vdavinci)
{
    struct device *dev = vdavinci_get_device(vdavinci);
    bool coherent = is_dev_dma_coherent(dev);
    int prot = coherent ? IOMMU_CACHE : 0;

    return prot | IOMMU_READ | IOMMU_WRITE;
}

#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(4,19,36)) || (defined(DRV_UT)))
STATIC dma_addr_t dev_dma_alloc_iova(struct device *dev, size_t size)
{
    /* default_domain & domain are the same here */
    struct iommu_domain *domain = iommu_get_domain_for_dev(dev);
    struct iommu_dma_cookie *cookie = domain->iova_cookie;
    struct iova_domain *iovad = &cookie->iovad;
    unsigned long shift, iova_len, iova = 0;
    unsigned long dma_limit = dma_get_mask(dev);

    if (cookie->type == IOMMU_DMA_MSI_COOKIE) {
        cookie->msi_iova += size;
        return cookie->msi_iova - size;
    }

    shift = iova_shift(iovad);
    iova_len = size >> shift;
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(5,16,0))
    /*
     * Freeing non-power-of-two-sized allocations back into the IOVA caches
     * will come back to bite us badly, so we have to waste a bit of space
     * rounding up anything cacheable to make sure that can't happen. The
     * order of the unadjusted size will still match upon freeing.
     */
    if (iova_len < (1 << (IOVA_RANGE_CACHE_MAX_SIZE - 1))) {
        iova_len = roundup_pow_of_two(iova_len);
    }
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,5,0))
    dma_limit = min_not_zero(dma_limit, (unsigned long)dev->bus_dma_limit);
#else
    if (dev->bus_dma_mask) {
        dma_limit &= dev->bus_dma_mask;
    }
#endif

    if (domain->geometry.force_aperture) {
        dma_limit = min(dma_limit, (unsigned long)domain->geometry.aperture_end);
    }

    /* Try to get PCI devices a SAC address */
    if (dma_limit > DMA_BIT_MASK(DMA_BIT_MASK_WIDTH) && dev_is_pci(dev)) {
        iova = alloc_iova_fast(iovad, iova_len,
                               DMA_BIT_MASK(DMA_BIT_MASK_WIDTH) >> shift, false);
    }

    if (iova == 0) {
        iova = alloc_iova_fast(iovad, iova_len, dma_limit >> shift, true);
    }

    return (dma_addr_t)iova << shift;
}

STATIC void dev_dma_free_iova(struct device *dev,
                              dma_addr_t iova, size_t size)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5,17,0))
    struct iommu_domain *domain = iommu_get_domain_for_dev(dev);
    struct iommu_dma_cookie *cookie = domain->iova_cookie;
    struct iova_domain *iovad = &cookie->iovad;

    /* The MSI case is only ever cleaning up its most recent allocation */
    if (cookie->type == IOMMU_DMA_MSI_COOKIE) {
        cookie->msi_iova -= size;
    } else {
        free_iova_fast(iovad, iova_pfn(iovad, iova), size >> iova_shift(iovad));
    }
#endif
}

#else
STATIC dma_addr_t dev_dma_alloc_iova(struct device *dev, size_t size)
{
    return 0;
}

STATIC void dev_dma_free_iova(struct device *dev,
                              dma_addr_t iova, size_t size)
{
}
#endif

STATIC void dev_unmap_2m_x86(struct device *dev, struct sg_table *dma_sgt)
{
    dma_unmap_sg(dev, dma_sgt->sgl, dma_sgt->orig_nents, DMA_BIDIRECTIONAL);
    sg_free_table(dma_sgt);
    kfree(dma_sgt);
}

STATIC int dev_map_2m_x86(struct device *dev, unsigned long gfn,
                          struct sg_table **dma_sgt,
                          struct page_info_list *dma_page_list, unsigned long size)
{
    int ret;
    struct page_info_entry *dma_page_info;
    struct scatterlist *sgl = NULL;

    if (dma_page_list == NULL) {
        return -EINVAL;
    }

    *dma_sgt = kzalloc(sizeof(struct sg_table), GFP_KERNEL);
    if (*dma_sgt == NULL) {
        return -ENOMEM;
    }

    ret = sg_alloc_table(*dma_sgt, dma_page_list->elem_num, GFP_KERNEL);
    if (ret) {
        vascend_err(dev, "sg_alloc_table return error result, ret: %d, sg_len: %u",
                    ret, dma_page_list->elem_num);
        ret = -ENOMEM;
        goto err_free_sgt;
    }

    sgl = (*dma_sgt)->sgl;
    list_for_each_entry(dma_page_info, &dma_page_list->head, list) {
        sg_set_page(sgl, dma_page_info->page, dma_page_info->length, 0);
        sgl = sg_next(sgl);
    }

    (*dma_sgt)->nents = dma_map_sg(dev, (*dma_sgt)->sgl, (*dma_sgt)->orig_nents, DMA_BIDIRECTIONAL);
    if ((*dma_sgt)->nents == 0) {
        ret = -ENOMEM;
        vascend_err(dev, "dma map sg failed, gfn: 0x%lx, size: %lu\n", gfn, size);
        goto err_free_sg;
    }

    return 0;
err_free_sg:
    sg_free_table(*dma_sgt);
err_free_sgt:
    kfree(*dma_sgt);

    return ret;
}

void dev_dma_unmap_ram_range_x86(struct hw_vdavinci *vdavinci,
                                 struct ram_range_info *ram_info)
{
    int i;
    struct dev_dma_sgt *dev_dma_sgt = NULL;
    struct device *dev = vdavinci_get_device(vdavinci);
    struct dev_dma_info *dma_info = NULL;
    struct list_head *pos = NULL, *next = NULL;

    list_for_each_safe(pos, next, &(vdavinci->vdev.dev_dma_info_list_head)) {
        dma_info = list_entry(pos, struct dev_dma_info, list);
        if (ram_info != dma_info->ram_info) {
            continue;
        }
        list_del(&dma_info->list);

        for (i = 0; i < ram_info->dma_array_len; i++) {
            dev_dma_sgt = dma_info->sgt_array[i];
            dev_unmap_2m_x86(dev, dev_dma_sgt->dma_sgt);
            kfree(dev_dma_sgt);
        }
        break;
    }
}

STATIC struct dev_dma_sgt *dev_map_sgt_2m_x86(struct hw_vdavinci *vdavinci,
                                              struct dma_info_2m *dma_array_node)
{
    int ret = 0;
    struct sg_table *dma_sgt = NULL;
    struct dev_dma_sgt *dev_dma_sgt = NULL;
    struct device *dev = vdavinci_get_device(vdavinci);

    ret = dev_map_2m_x86(dev, dma_array_node->gfn, &dma_sgt,
                         &dma_array_node->dma_page_list,
                         dma_array_node->size);
    if (ret) {
        goto out;
    }

    dev_dma_sgt = (struct dev_dma_sgt *)kzalloc(sizeof(struct dev_dma_sgt), GFP_KERNEL);
    if (dev_dma_sgt == NULL) {
        ret = -ENOMEM;
        goto free;
    }

    dev_dma_sgt->gfn = dma_array_node->gfn;
    dev_dma_sgt->dev = dev;
    dev_dma_sgt->dma_sgt = dma_sgt;
    dev_dma_sgt->dma_addr = sg_dma_address(dma_sgt->sgl);

    return dev_dma_sgt;

free:
    dev_unmap_2m_x86(dev, dev_dma_sgt->dma_sgt);
out:
    vascend_err(dev, "dma map page failed, ret: %d, size: 0x%lx\n",
                ret, dma_array_node->size);
    return NULL;
}

int dev_dma_map_ram_range_x86(struct hw_vdavinci *vdavinci,
                              struct ram_range_info *ram_info)
{
    int ret = 0;
    int i, j;
    struct dma_info_2m *dma_array_node = NULL;
    struct dev_dma_sgt **sgt_array = NULL;
    struct dev_dma_sgt *dev_dma_sgt = NULL;
    struct device *dev = vdavinci_get_device(vdavinci);
    struct dev_dma_info *dma_info = NULL;

    sgt_array = (struct dev_dma_sgt **)
        kzalloc(sizeof(struct dev_dma_sgt *) * ram_info->dma_array_len, GFP_KERNEL);
    if (sgt_array == NULL) {
        return -ENOMEM;
    }

    dma_info = kzalloc(sizeof(struct dev_dma_info), GFP_KERNEL);
    if (dma_info == NULL) {
        ret = -ENOMEM;
        goto free_sgt;
    }

    for (i = 0; i < ram_info->dma_array_len; i++) {
        dma_array_node = ram_info->dma_array[i];

        dev_dma_sgt = dev_map_sgt_2m_x86(vdavinci, dma_array_node);
        if (dev_dma_sgt == NULL) {
            ret = -EINVAL;
            goto out;
        }
        sgt_array[i] = dev_dma_sgt;
    }
    dma_info->ram_info = ram_info;
    dma_info->sgt_array = sgt_array;
    list_add_tail(&(dma_info->list), &(vdavinci->vdev.dev_dma_info_list_head));

    return 0;
out:
    for (j = 0; j < i; j++) {
        dma_array_node = ram_info->dma_array[j];
        dev_dma_sgt = sgt_array[i];
        dev_unmap_2m_x86(dev, dev_dma_sgt->dma_sgt);
        kfree(dev_dma_sgt);
    }
    kfree(dma_info);
free_sgt:
    kfree(sgt_array);
    return ret;
}

STATIC void dev_unmap_2m(struct device *dev, dma_addr_t base_2m_iova,
                         struct page_info_list *dma_page_list)
{
    struct page_info_entry *dma_page_info = NULL;
    struct list_head *pos = NULL, *next = NULL;
    dma_addr_t iova = base_2m_iova;

    list_for_each_safe(pos, next, &(dma_page_list->head)) {
        dma_page_info = list_entry(pos, struct page_info_entry, list);
        vdavinci_iommu_unmap(dev, iova, dma_page_info->length);
        iova += dma_page_info->length;
    }
}

/* alloc iova. return gfn directly if passthrough */
STATIC dma_addr_t vdev_alloc_iova(struct device *dev,
                                  struct ram_range_info *ram_info,
                                  bool is_passthrough)
{
    if (is_passthrough) {
        return ram_info->base_gfn << PAGE_SHIFT;
    }

    return dev_dma_alloc_iova(dev, ram_info->npages * PAGE_SIZE);
}

/* free iova. do nothing if passthrough */
STATIC void vdev_free_iova(struct device *dev, dma_addr_t iova,
                           size_t size, bool is_passthrough)
{
    if (!is_passthrough) {
        dev_dma_free_iova(dev, iova, size);
    }
}

/**
 * After calling hw_pin_guest_contiguous_page, we get a linked list of consecutive pfn
 * addresses. Then you can use this function to convert the pfn address to the iova
 * address of the corresponding pcie device.
 */
STATIC int dev_map_2m(struct device *dev,
                      dma_addr_t base_2m_iova, struct page_info_list *dma_page_list,
                      int prot)
{
    struct page_info_entry *dma_page_info = NULL;
    struct page_info_entry *dma_page_info_err = NULL;
    struct list_head *pos = NULL, *next = NULL;
    dma_addr_t iova = base_2m_iova;
    phys_addr_t phys;

    if (dma_page_list == NULL) {
        return -EINVAL;
    }

    list_for_each_safe(pos, next, &(dma_page_list->head)) {
        dma_page_info = list_entry(pos, struct page_info_entry, list);

        phys = page_to_phys(dma_page_info->page);
        /* phys is page algned, so there is no iova_offset alignment */
        if (vdavinci_iommu_map(dev, iova, phys, dma_page_info->length, prot) != 0) {
            dma_page_info_err = dma_page_info;
            goto unmap;
        }

#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(4,19,36)) || (defined(DRV_UT)))
        if (!is_dev_dma_coherent(dev)) {
            dma_sync_single_for_device(dev, iova, dma_page_info->length, DMA_BIDIRECTIONAL);
        }
#endif
        iova += dma_page_info->length;
    }

    return 0;

unmap:
    iova = base_2m_iova;
    list_for_each_safe(pos, next, &(dma_page_list->head)) {
        dma_page_info = list_entry(pos, struct page_info_entry, list);

        if (dma_page_info != dma_page_info_err) {
            vdavinci_iommu_unmap(dev, iova, dma_page_info->length);
            iova += dma_page_info->length;
        } else {
            break;
        }
    }

    return -EFAULT;
}

void dev_dma_unmap_ram_range(struct hw_vdavinci *vdavinci,
                             struct ram_range_info *ram_info)
{
    int j;
    unsigned long count = 0;
    struct dma_info_2m *dma_array_node = NULL;
    struct dev_dma_info *dma_info = NULL;
    struct list_head *pos = NULL, *next = NULL;
    struct device *dev = vdavinci_get_device(vdavinci);

    list_for_each_safe(pos, next, &(vdavinci->vdev.dev_dma_info_list_head)) {
        dma_info = list_entry(pos, struct dev_dma_info, list);
        if (ram_info != dma_info->ram_info) {
            continue;
        }
        list_del(&dma_info->list);
        for (j = 0; j < ram_info->dma_array_len; j++) {
            count++;
            if (hw_vdavinci_scheduled(vdavinci,
                                      count * VFIO_PIN_PAGES_MAX_ENTRIES,
                                      VDAVINCI_MAP_PAGES_OF_SCHEDULE,
                                      VDAVINCI_MAP_TIME_OF_SCHEDULE,
                                      NULL)) {
                count = 0;
            }

            dma_array_node = ram_info->dma_array[j];
            dev_unmap_2m(dev, dma_info->base_iova +
                j * VFIO_PIN_PAGES_MAX_ENTRIES * PAGE_SIZE,
                &(dma_array_node->dma_page_list));
        }
        vdev_free_iova(dev, dma_info->base_iova, ram_info->npages * PAGE_SIZE,
                       vdavinci->is_passthrough);
        kfree(dma_info);
        break;
    }
}

int dev_dma_map_ram_range(struct hw_vdavinci *vdavinci,
                          struct ram_range_info *ram_info)
{
    unsigned long count = 0;
    int i = 0, j = 0, ret = 0;
    struct dma_info_2m *dma_array_node = NULL;
    size_t ram_size = ram_info->npages * PAGE_SIZE;
    struct device *dev = vdavinci_get_device(vdavinci);
    int prot = dev_get_prot(vdavinci);
    dma_addr_t base_iova;
    struct dev_dma_info *dma_info = NULL;

    /* The size is page aligned, there is no need to use iova_align to align */
    base_iova = vdev_alloc_iova(dev, ram_info, vdavinci->is_passthrough);
    if (base_iova == IOMMU_MAPPING_ERROR && !vdavinci->is_passthrough) {
        vascend_err(dev, "alloc iova addr failed\n");
        return -EINVAL;
    }

    dma_info = kzalloc(sizeof(struct dev_dma_info), GFP_KERNEL);
    if (!dma_info) {
        vdev_free_iova(dev, base_iova, ram_size, vdavinci->is_passthrough);
        return -ENOMEM;
    }

    for (i = 0; i < ram_info->dma_array_len; i++) {
        count++;
        if (hw_vdavinci_scheduled(vdavinci,
                                  count * VFIO_PIN_PAGES_MAX_ENTRIES,
                                  VDAVINCI_MAP_PAGES_OF_SCHEDULE,
                                  VDAVINCI_MAP_TIME_OF_SCHEDULE,
                                  NULL)) {
            count = 0;
        }

        dma_array_node = ram_info->dma_array[i];
        ret = dev_map_2m(dev, base_iova + i * VFIO_PIN_PAGES_MAX_ENTRIES * PAGE_SIZE,
                &(dma_array_node->dma_page_list),
                prot);
        if (ret) {
            goto out;
        }
    }

    dma_info->ram_info = ram_info;
    dma_info->base_iova = base_iova;
    list_add_tail(&(dma_info->list), &(vdavinci->vdev.dev_dma_info_list_head));

    return 0;
out:
    for (j = 0; j < i; j++) {
        dma_array_node = ram_info->dma_array[j];

        dev_unmap_2m(dev, base_iova + i * VFIO_PIN_PAGES_MAX_ENTRIES * PAGE_SIZE,
            &(dma_array_node->dma_page_list));
    }
    kfree(dma_info);
    vdev_free_iova(dev, base_iova, ram_size, vdavinci->is_passthrough);
    return ret;
}
