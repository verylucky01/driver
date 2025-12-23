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

#include "vfio_ops.h"

void vdavinci_iommu_unmap(struct device *dev, unsigned long iova, size_t size)
{
    size_t unmapped;
    struct iommu_domain *domain = NULL;

    dma_sync_single_for_cpu(dev, iova, size, DMA_BIDIRECTIONAL);
    domain = iommu_get_domain_for_dev(dev);
    unmapped = iommu_unmap(domain, iova, size);
    WARN_ON(unmapped != size);
}

int vdavinci_iommu_map(struct device *dev, unsigned long iova,
                       phys_addr_t paddr, size_t size, int prot)
{
    struct iommu_domain *domain = iommu_get_domain_for_dev(dev);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6,3,0))
    return iommu_map(domain, iova, paddr, size, prot, GFP_KERNEL);
#else
    return iommu_map(domain, iova, paddr, size, prot);
#endif
}

void vdavinci_unpin_pages(struct hw_vdavinci *vdavinci, struct vdavinci_pin_info *pin_info)
{
    int ret = pin_info->npage;

    if (pin_info->npage <= 0 || pin_info->npage > VFIO_PIN_PAGES_MAX_ENTRIES) {
        vascend_err(vdavinci_to_dev(vdavinci), "vdavinci error npage: %d\n", pin_info->npage);
        return;
    }
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6,0,0))
    if (pin_info->gfn == 0) {
        vascend_err(vdavinci_to_dev(vdavinci), "vdavinci gfn error");
        return;
    }
#else
    if (IS_ERR_OR_NULL(pin_info->gfns)) {
        vascend_err(vdavinci_to_dev(vdavinci), "vdavinci error gfns");
        return;
    }
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6,0,0))
    vfio_unpin_pages(vdavinci->vdev.vfio_device, pin_info->gfn, pin_info->npage);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(5,19,0))
    ret = vfio_unpin_pages(vdavinci->vdev.vfio_device, pin_info->gfns, pin_info->npage);
#else
    ret = vfio_unpin_pages(mdev_dev(vdavinci->vdev.mdev), pin_info->gfns, pin_info->npage);
#endif
    WARN_ON(ret != pin_info->npage);
}

int vdavinci_pin_pages(struct hw_vdavinci *vdavinci, struct vdavinci_pin_info *pin_info)
{
    int ret = -1;

    if (pin_info->npage <= 0 || pin_info->npage > VFIO_PIN_PAGES_MAX_ENTRIES) {
        vascend_err(vdavinci_to_dev(vdavinci), "vdavinci error npage: %d\n", pin_info->npage);
        return -EINVAL;
    }
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6,0,0))
    if (pin_info->gfn == 0 || IS_ERR_OR_NULL(pin_info->pages)) {
        vascend_err(vdavinci_to_dev(vdavinci), "vdavinci pages or gfn error");
        return -EINVAL;
    }
#else
    if (IS_ERR_OR_NULL(pin_info->gfns) || IS_ERR_OR_NULL(pin_info->pfns)) {
        vascend_err(vdavinci_to_dev(vdavinci), "vdavinci pfns or gfns error ");
        return -EINVAL;
    }
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6,0,0))
    ret = vfio_pin_pages(vdavinci->vdev.vfio_device, pin_info->gfn,
                         pin_info->npage, IOMMU_READ | IOMMU_WRITE, pin_info->pages);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(5,19,0))
    ret = vfio_pin_pages(vdavinci->vdev.vfio_device, pin_info->gfns,
                         pin_info->npage, IOMMU_READ | IOMMU_WRITE, pin_info->pfns);
#else
    ret = vfio_pin_pages(mdev_dev(vdavinci->vdev.mdev), pin_info->gfns,
                         pin_info->npage, IOMMU_READ | IOMMU_WRITE, pin_info->pfns);
#endif
    return ret;
}

struct device *get_mdev_parent(struct mdev_device *mdev)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,1,0)
    return mdev->type->parent->dev;
#else
    return mdev_parent_dev(mdev);
#endif
}

bool device_is_mdev(struct device *dev)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,13,0)
    if (dev == NULL) {
        return false;
    }
    return !strcmp(dev->bus->name, "mdev");
#else
    return dev->bus == &mdev_bus_type;
#endif
}

struct mdev_device *get_mdev_device(struct device *dev)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,1,0)
    return to_mdev_device(dev);
#else
    return mdev_from_dev(dev);
#endif
}

void *get_mdev_drvdata(struct device *dev)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,19,0)
    return dev_get_drvdata(dev);
#else
    struct mdev_device *mdev = mdev_from_dev(dev);

    return mdev_get_drvdata(mdev);
#endif
}

void set_mdev_drvdata(struct device *dev, void *data)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,19,0)
    dev_set_drvdata(dev, data);
#else
    struct mdev_device *mdev = mdev_from_dev(dev);

    mdev_set_drvdata(mdev, data);
#endif
}

/**
 * first, return the vf device otherwise return mdev parent device
 */
struct device *vdavinci_get_device(struct hw_vdavinci *vdavinci)
{
    struct device *dev = vdavinci_resource_dev(vdavinci);
    if (!dev) {
        dev = get_mdev_parent(vdavinci->vdev.mdev);
    }

    return dev;
}

struct device *vdavinci_to_dev(struct hw_vdavinci *vdavinci)
{
    struct device *dev = NULL;

    if (vdavinci == NULL) {
        return NULL;
    }

    dev = vdavinci_resource_dev(vdavinci);
    if (dev) {
        return dev;
    }

    dev = vdavinci->dvt->vdavinci_priv->dev;
    if (dev) {
        return dev;
    }

    return get_mdev_parent(vdavinci->vdev.mdev);
}
