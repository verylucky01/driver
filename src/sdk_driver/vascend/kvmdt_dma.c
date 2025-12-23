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
#include <linux/debugfs.h>
#include "dvt.h"
#include "kvmdt.h"
#include "vfio_ops.h"

#if ((LINUX_VERSION_CODE <= KERNEL_VERSION(5,13,0)))
STATIC struct dvt_dma *__dvt_cache_find_gfn(struct hw_vdavinci *vdavinci, gfn_t gfn)
{
    struct rb_node *node = vdavinci->vdev.gfn_cache.rb_node;
    struct dvt_dma *itr = NULL;

    while (node != NULL) {
        itr = rb_entry(node, struct dvt_dma, gfn_node);

        if (gfn < itr->gfn) {
            node = node->rb_left;
        } else if (gfn > itr->gfn) {
            node = node->rb_right;
        } else {
            return itr;
        }
    }
    return NULL;
}

STATIC int __dvt_cache_add(struct hw_vdavinci *vdavinci, gfn_t gfn,
                           struct sg_table *dma_sgt, struct page_info_list *dma_page_list, unsigned long size)
{
    struct dvt_dma *new = NULL, *itr = NULL;
    struct rb_node **link, *parent = NULL;

    new = kzalloc(sizeof(struct dvt_dma), GFP_KERNEL);
    if (new == NULL) {
        return -ENOMEM;
    }

    new->vdavinci = vdavinci;
    new->gfn = gfn;
    new->dma_addr = sg_dma_address(dma_sgt->sgl);
    new->size = size;
    new->dma_sgt = dma_sgt;
    new->dma_page_list = dma_page_list;
    kref_init(&new->ref);

    /* gfn_cache maps gfn to struct dvt_dma. */
    link = &vdavinci->vdev.gfn_cache.rb_node;
    while (*link) {
        parent = *link;
        itr = rb_entry(parent, struct dvt_dma, gfn_node);

        if (gfn < itr->gfn) {
            link = &parent->rb_left;
        } else {
            link = &parent->rb_right;
        }
    }
    rb_link_node(&new->gfn_node, parent, link);
    rb_insert_color(&new->gfn_node, &vdavinci->vdev.gfn_cache);

    /* dma_cache maps dma addr to struct dvt_dma. */
    parent = NULL;
    link = &vdavinci->vdev.dma_cache.rb_node;
    while (*link) {
        parent = *link;
        itr = rb_entry(parent, struct dvt_dma, dma_node);

        if (new->dma_addr < itr->dma_addr) {
            link = &parent->rb_left;
        } else {
            link = &parent->rb_right;
        }
    }
    rb_link_node(&new->dma_node, parent, link);
    rb_insert_color(&new->dma_node, &vdavinci->vdev.dma_cache);

    vdavinci->vdev.nr_cache_entries++;
    return 0;
}

STATIC void __dvt_cache_remove_entry(struct hw_vdavinci *vdavinci,
                                     struct dvt_dma *entry)
{
    rb_erase(&entry->gfn_node, &vdavinci->vdev.gfn_cache);
    rb_erase(&entry->dma_node, &vdavinci->vdev.dma_cache);
    kfree(entry);
    vdavinci->vdev.nr_cache_entries--;
}

STATIC void dvt_unpin_guest_page(struct hw_vdavinci *vdavinci, unsigned long gfn,
                                 unsigned long size)
{
    unsigned long total_pages = roundup(size, PAGE_SIZE) / PAGE_SIZE;
    unsigned long npage;
    int ret;

    for (npage = 0; npage < total_pages; npage++) {
        unsigned long cur_gfn = gfn + npage;

        ret = vfio_unpin_pages(mdev_dev(vdavinci->vdev.mdev), &cur_gfn, 1);
        WARN_ON(ret != 1);
    }
}

STATIC void dvt_get_base_pfn_from_dma_list(struct page_info_list *dma_page_list,
    unsigned long *base_pfn,
    unsigned long *base_pfn_length)
{
    struct page_info_entry *dma_page_info;

    dma_page_info = list_empty(&(dma_page_list->head)) == true ? NULL :
                list_last_entry(&(dma_page_list->head), struct page_info_entry, list);
    if (dma_page_info) {
        *base_pfn = page_to_pfn(dma_page_info->page);
        *base_pfn_length = dma_page_info->length;
    } else {
        *base_pfn = 0;
        *base_pfn_length = 0;
    }
}

STATIC int dvt_add_dma_page_list(struct page_info_list *dma_page_list, unsigned long gfn,
                                 unsigned int size, struct page *page)
{
    struct page_info_entry *dma_page_info = NULL;

    dma_page_info = list_empty(&(dma_page_list->head)) == true ? NULL :
                list_last_entry(&(dma_page_list->head), struct page_info_entry, list);
    if (dma_page_info) {
        if (page_to_pfn(dma_page_info->page) == page_to_pfn(page)) {
            dma_page_info->length = size;
            return 0;
        }
    }

    dma_page_info = kzalloc(sizeof(struct page_info_entry), GFP_KERNEL);
    if (dma_page_info == NULL) {
        return -ENOMEM;
    }

    dma_page_info->gfn = gfn;
    dma_page_info->length = size;
    dma_page_info->page = page;
    list_add_tail(&(dma_page_info->list), &(dma_page_list->head));
    dma_page_list->elem_num++;

    return 0;
}

STATIC void dvt_update_last_entry_size(struct page_info_list *dma_page_list,
                                       unsigned long pfn_length)
{
    struct page_info_entry *dma_page_info;

    dma_page_info = list_empty(&(dma_page_list->head)) == true ? NULL :
                list_last_entry(&(dma_page_list->head), struct page_info_entry, list);
    if (dma_page_info) {
        dma_page_info->length -= pfn_length;
    }
}

/**
 * When dealing with pfn, it is necessary to judge whether it is
 * continuous with the last area in dma_page_list. When a discontinuous
 * pfn or the last pfn is found, this area will be added to the
 * dma_page_list. In exceptional cases, the length of pfn added to
 * dma_page_list will be returned.
 */
STATIC int dvt_add_pfn_to_dma_list(struct hw_vdavinci *vdavinci,
                                   struct page_info_list *dma_page_list,
                                   unsigned long *user_pfn,
                                   unsigned long *pfn,
                                   int contiguous_npage)
{
    int i, ret, in_list_npage = 0;
    unsigned long base_pfn = 0;
    unsigned long base_pfn_length = 0;
    unsigned long base_gfn = 0;

    dvt_get_base_pfn_from_dma_list(dma_page_list, &base_pfn, &base_pfn_length);
    for (i = 0; i < contiguous_npage; i++) {
        if (base_pfn == 0 && base_pfn_length == 0) {
            base_pfn = pfn[i];
            base_gfn = user_pfn[i];
            base_pfn_length = PAGE_SIZE;
            continue;
        }

        if (pfn[i] != (base_pfn +
                roundup(base_pfn_length, PAGE_SIZE) / PAGE_SIZE)) {
            ret = dvt_add_dma_page_list(dma_page_list, base_gfn,
                                        base_pfn_length,
                                        pfn_to_page(base_pfn));
            if (ret) {
                return in_list_npage;
            }

            in_list_npage = i;
            base_pfn = pfn[i];
            base_gfn = user_pfn[i];
            base_pfn_length = PAGE_SIZE;
            continue;
        }

        base_pfn_length += PAGE_SIZE;
    }

    ret = dvt_add_dma_page_list(dma_page_list, base_gfn,
                                base_pfn_length,
                                pfn_to_page(base_pfn));
    if (ret) {
        return in_list_npage;
    }

    return contiguous_npage;
}

/**
 * dma page_list saves pinned pages. Abnormal conditions usually release the
 * resources managed by dma_page_list. If the pinned page is not added to
 * dma_page_list, you need to unpin it separately.
 */
STATIC int dvt_pin_guest_contiguous_page(struct hw_vdavinci *vdavinci,
                                         unsigned long *user_pfn,
                                         unsigned long *pfn,
                                         int contiguous_npage,
                                         struct page_info_list *dma_page_list)
{
    int i, ret;

    ret = vfio_pin_pages(mdev_dev(vdavinci->vdev.mdev), user_pfn, contiguous_npage,
                         IOMMU_READ | IOMMU_WRITE, pfn);
    if (ret < 0) {
        vascend_err(vdavinci_to_dev(vdavinci),
            "pin pages failed, vid: %u, gfn : 0x%lx, npage : %d\n",
            vdavinci->id, user_pfn[0], contiguous_npage);
        return ret;
    }

    for (i = 0; i < contiguous_npage; i++) {
        if (!pfn_valid(pfn[i])) {
            vascend_err(vdavinci_to_dev(vdavinci),
                "vid: %u pfn 0x%lx is not mem backed\n",
                vdavinci->id, pfn[i]);
            return -EFAULT;
        }
    }

    ret = dvt_add_pfn_to_dma_list(vdavinci, dma_page_list,
                                  user_pfn, pfn, contiguous_npage);
    if (ret != contiguous_npage) {
        dvt_unpin_guest_page(vdavinci, user_pfn[0] + ret,
                             (contiguous_npage - ret) * PAGE_SIZE);
        return -EINVAL;
    }

        return 0;
}

/**
 * In order to DMA, we need to get the physical page according to gfn.
 * dma_page_list saves a series of continous physical pages. Doing pin pages
 * during DMA will reduce performance. The solution of using memory pool can
 * solve this problem. User will pin the physical memory corresponding to
 * a large section of gfn before DMA
 *
 * Improve performance by reducing the number of calls to vfio_pin_pages.
 * And the step size of the processing page is the same as vfio_pin_pages
 * and both are VFIO_PIN_PAGES_MAX_ENTRIES
 *
 * The length entered by the user is not necessarily aligned with the page
 * and needs to be modified
 */
STATIC int dvt_pin_guest_page_list(struct hw_vdavinci *vdavinci, unsigned long gfn,
                                   struct page_info_list *dma_page_list, unsigned long size)
{
    unsigned long *pfn = NULL;
    unsigned long *user_pfn = NULL;

    int total_pages, contiguous_npage, contiguous_npage_max;
    int ret = 0, i, npage = 0;

    total_pages = roundup(size, PAGE_SIZE) / PAGE_SIZE;
    contiguous_npage_max = min_t(int, total_pages, VFIO_PIN_PAGES_MAX_ENTRIES);
    pfn = kmalloc(sizeof(unsigned long) * contiguous_npage_max, GFP_KERNEL);
    user_pfn = kmalloc(sizeof(unsigned long) * contiguous_npage_max, GFP_KERNEL);
    if (!pfn || !user_pfn) {
        ret = -ENOMEM;
        goto done;
    }

    for (npage = 0; npage < total_pages; npage += contiguous_npage) {
        contiguous_npage = min_t(int, (total_pages - npage), VFIO_PIN_PAGES_MAX_ENTRIES);

        for (i = 0; i < contiguous_npage; i++) {
            user_pfn[i] = gfn + npage + i;
        }

        ret = dvt_pin_guest_contiguous_page(vdavinci, user_pfn, pfn,
                                            contiguous_npage, dma_page_list);
        if (ret < 0) {
            vascend_err(vdavinci_to_dev(vdavinci),
                "pin contiguous page failed, vid: %u, pos : %d, ret : %d\n",
                vdavinci->id, npage, ret);
            break;
        }
    }

    if (ret == 0) {
        dvt_update_last_entry_size(dma_page_list, total_pages * PAGE_SIZE - size);
    }

done:
    kfree(pfn);
    kfree(user_pfn);

    return ret;
}

STATIC int dvt_dma_map_page(struct hw_vdavinci *vdavinci, unsigned long gfn,
                            struct sg_table **dma_sgt, struct page_info_list **dma_page_list, unsigned long size)
{
    struct device *dev = get_mdev_parent(vdavinci->vdev.mdev);
    int ret;
    struct page_info_entry *dma_page_info = NULL;
    struct scatterlist *sgl = NULL;
    struct list_head *pos = NULL, *next = NULL;

    *dma_page_list = kzalloc(sizeof(struct page_info_list), GFP_KERNEL);
    if (*dma_page_list == NULL) {
        return -ENOMEM;
    }
    INIT_LIST_HEAD(&((*dma_page_list)->head));

    ret = dvt_pin_guest_page_list(vdavinci, gfn, *dma_page_list, size);
    if (ret) {
        vascend_err(dev, "pin guest pages failed, vid: %u, gfn: 0x%lx, size: %lu\n",
            vdavinci->id, gfn, size);
        goto err_unpin_pages;
    }

    *dma_sgt = kzalloc(sizeof(struct sg_table), GFP_KERNEL);
    if (*dma_sgt == NULL) {
        ret = -ENOMEM;
        goto err_unpin_pages;
    }

    ret = sg_alloc_table(*dma_sgt, (*dma_page_list)->elem_num, GFP_KERNEL);
    if (ret) {
        ret = -ENOMEM;
        goto err_free_sgt;
    }

    sgl = (*dma_sgt)->sgl;
    list_for_each_safe(pos, next, &((*dma_page_list)->head)) {
        dma_page_info = list_entry(pos, struct page_info_entry, list);
        sg_set_page(sgl, dma_page_info->page, dma_page_info->length, 0);
        sgl = sg_next(sgl);
    }

    (*dma_sgt)->nents = dma_map_sg(dev, (*dma_sgt)->sgl, (*dma_sgt)->orig_nents, DMA_BIDIRECTIONAL);
    if ((*dma_sgt)->nents == 0) {
        ret = -ENOMEM;
        vascend_err(dev, "dma map sg failed, vid: %u, gfn: 0x%lx, size: %lu\n",
            vdavinci->id, gfn, size);
        goto err_free_sg;
    }

    return 0;

err_free_sg:
    sg_free_table(*dma_sgt);
err_free_sgt:
    kfree(*dma_sgt);
    *dma_sgt = NULL;
err_unpin_pages:
    dvt_unpin_guest_page_list(vdavinci, *dma_page_list);
    return ret;
}

void dvt_unpin_guest_page_list(struct hw_vdavinci *vdavinci, struct page_info_list *dma_page_list)
{
    struct page_info_entry *dma_page_info = NULL;
    struct list_head *pos = NULL, *next = NULL;

    list_for_each_safe(pos, next, &(dma_page_list->head)) {
        dma_page_info = list_entry(pos, struct page_info_entry, list);
        dvt_unpin_guest_page(vdavinci, dma_page_info->gfn, dma_page_info->length);
        list_del(pos);
        kfree(dma_page_info);
    }

    kfree(dma_page_list);
}

STATIC void dvt_dma_unmap_page_list(struct hw_vdavinci *vdavinci, struct sg_table *dma_sgt,
                                    struct page_info_list *dma_page_list)
{
    struct device *dev = get_mdev_parent(vdavinci->vdev.mdev);

    dma_unmap_sg(dev, dma_sgt->sgl, dma_sgt->orig_nents, DMA_BIDIRECTIONAL);
    sg_free_table(dma_sgt);
    kfree(dma_sgt);

    dvt_unpin_guest_page_list(vdavinci, dma_page_list);
}

int kvmdt_dma_map_guest_page(uintptr_t handle, unsigned long gfn,
                             unsigned long size, struct sg_table **dma_sgt)
{
    struct kvmdt_guest_info *info = (struct kvmdt_guest_info *)handle;
    struct hw_vdavinci *vdavinci = NULL;
    struct dvt_dma *entry = NULL;
    struct page_info_list *dma_page_list = NULL;
    int ret = 0;

    if (!handle_valid(handle)) {
        return -EINVAL;
    }

    vdavinci = info->vdavinci;
    mutex_lock(&info->vdavinci->vdev.cache_lock);
    entry = __dvt_cache_find_gfn(info->vdavinci, gfn);
    if (entry == NULL) {
        ret = dvt_dma_map_page(vdavinci, gfn, dma_sgt, &dma_page_list, size);
        if (ret) {
            goto out;
        }
        ret = __dvt_cache_add(vdavinci, gfn, *dma_sgt, dma_page_list, size);
        if (ret) {
            dvt_dma_unmap_page_list(vdavinci, *dma_sgt, dma_page_list);
            *dma_sgt = NULL;
            goto out;
        }
    } else if (entry->size != size) {
        ret = -EINVAL;
        vascend_err(vdavinci_to_dev(vdavinci), "the dma size (%lu) is not equal "
            "the size (%lu) in dma entry cache, gfn is 0x%lx, origin dma may be not unmap, vid: %u\n",
            size, entry->size, gfn, vdavinci->id);
        goto out;
    } else {
        kref_get(&entry->ref);
        *dma_sgt = entry->dma_sgt;
    }

out:
    mutex_unlock(&info->vdavinci->vdev.cache_lock);
    return ret;
}

STATIC struct dvt_dma *__dvt_cache_find_dma(struct hw_vdavinci *vdavinci,
                                            dma_addr_t dma_addr)
{
    struct rb_node *node = vdavinci->vdev.dma_cache.rb_node;
    struct dvt_dma *itr = NULL;

    while (node != NULL) {
        itr = rb_entry(node, struct dvt_dma, dma_node);

        if (dma_addr < itr->dma_addr) {
            node = node->rb_left;
        } else if (dma_addr > itr->dma_addr) {
            node = node->rb_right;
        } else {
            return itr;
        }
    }
    return NULL;
}

STATIC void dvt_dma_release(struct kref *ref)
{
    struct dvt_dma *entry = container_of(ref, typeof(*entry), ref);

    dvt_dma_unmap_page_list(entry->vdavinci, entry->dma_sgt, entry->dma_page_list);
    __dvt_cache_remove_entry(entry->vdavinci, entry);
}

void kvmdt_dma_unmap_guest_page(uintptr_t handle, struct sg_table *dma_sgt)
{
    struct kvmdt_guest_info *info = (struct kvmdt_guest_info *)handle;
    struct dvt_dma *entry = NULL;
    dma_addr_t dma_addr;

    if (!handle_valid(handle)) {
        return;
    }

    if (dma_sgt == NULL) {
        return;
    }

    dma_addr = sg_dma_address(dma_sgt->sgl);
    mutex_lock(&info->vdavinci->vdev.cache_lock);
    entry = __dvt_cache_find_dma(info->vdavinci, dma_addr);
    if (entry != NULL) {
        kref_put(&entry->ref, dvt_dma_release);
    }
    mutex_unlock(&info->vdavinci->vdev.cache_lock);
}

void dvt_cache_init(struct hw_vdavinci *vdavinci)
{
    vdavinci->vdev.gfn_cache = RB_ROOT;
    vdavinci->vdev.dma_cache = RB_ROOT;
    vdavinci->vdev.nr_cache_entries = 0;
    mutex_init(&vdavinci->vdev.cache_lock);
    hw_dvt_debugfs_add_cache_info(vdavinci);
}

void dvt_cache_destroy(struct hw_vdavinci *vdavinci)
{
    struct dvt_dma *dma = NULL;
    struct rb_node *node = NULL;

    debugfs_remove(vdavinci->debugfs.debugfs_cache_info);

    for (;;) {
        mutex_lock(&vdavinci->vdev.cache_lock);
        node = rb_first(&vdavinci->vdev.gfn_cache);
        if (node == NULL) {
            mutex_unlock(&vdavinci->vdev.cache_lock);
            break;
        }
        dma = rb_entry(node, struct dvt_dma, gfn_node);
        dvt_dma_unmap_page_list(vdavinci, dma->dma_sgt, dma->dma_page_list);
        __dvt_cache_remove_entry(vdavinci, dma);
        mutex_unlock(&vdavinci->vdev.cache_lock);
    }
}

void dvt_cache_remove_ram(struct hw_vdavinci *vdavinci,
                          unsigned long start_gfn, unsigned long size)
{
    struct dvt_dma *iter = NULL;
    unsigned long i, gfn_len = size / PAGE_SIZE;

    for (i = 0; i < gfn_len; i++) {
        iter = __dvt_cache_find_gfn(vdavinci, start_gfn);
        if (iter == NULL) {
            continue;
        }    

        dvt_dma_unmap_page_list(vdavinci, iter->dma_sgt, iter->dma_page_list);
        __dvt_cache_remove_entry(vdavinci, iter);
    }    
}
#endif
