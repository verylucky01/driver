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

#ifndef _VFIO_OPS_H_
#define _VFIO_OPS_H_

#include "dvt.h"

struct vdavinci_pin_info {
    unsigned long gfn;
    unsigned long *gfns;
    unsigned long *pfns;
    int npage;
    struct page **pages;
};

struct device *get_mdev_parent(struct mdev_device *mdev);
bool device_is_mdev(struct device *dev);
struct mdev_device *get_mdev_device(struct device *dev);
void *get_mdev_drvdata(struct device *dev);
void set_mdev_drvdata(struct device *dev, void *data);
struct device *vdavinci_get_device(struct hw_vdavinci *vdavinci);
struct device *vdavinci_to_dev(struct hw_vdavinci *vdavinci);

void vdavinci_iommu_unmap(struct device *dev, unsigned long iova, size_t size);
int vdavinci_iommu_map(struct device *dev, unsigned long iova,
                       phys_addr_t paddr, size_t size, int prot);
void vdavinci_unpin_pages(struct hw_vdavinci *vdavinci, struct vdavinci_pin_info *pin_info);
int vdavinci_pin_pages(struct hw_vdavinci *vdavinci, struct vdavinci_pin_info *pin_info);

#endif
