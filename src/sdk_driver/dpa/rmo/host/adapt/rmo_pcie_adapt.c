/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026. All rights reserved.
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
#include "ka_common_pub.h"
#include "comm_kernel_interface.h"
#include "pbl_kernel_interface.h"

#include "dpa/dpa_rmo_kernel.h"
#include "rmo_kern_log.h"
#include "rmo_mem_sharing.h"

int rmo_mem_addr_map(u32 devid, u64 paddr, u64 size, struct rmo_mem_map_addr *mapped_addr)
{
    ka_dma_addr_t dma_addr;
    ka_device_t *dev = NULL;

    dev = hal_kernel_devdrv_get_pci_dev_by_devid(devid);
    if (dev == NULL) {
        rmo_err("Get dev failed. (devid=%u)\n", devid);
        return -ENODEV;
    }

    dma_addr = hal_kernel_devdrv_dma_map_page(dev, ka_mm_pfn_to_page(KA_MM_PFN_DOWN(paddr)), 0, size,
                                              KA_DMA_BIDIRECTIONAL);
    if (ka_mm_dma_mapping_error(dev, dma_addr)) {
        rmo_err("Dma_map_page failed. (devid=%u; error=%d; size=%llu)\n",
            devid, ka_mm_dma_mapping_error(dev, dma_addr), size);
        return -ENOMEM;
    }

    *((u64 *)mapped_addr->raw_addr.raw_addr) = (u64)dma_addr;
    mapped_addr->raw_addr.raw_addr_len = sizeof(u64);
    return 0;
}

int rmo_mem_addr_unmap(u32 devid, struct rmo_mem_map_addr *mapped_addr, u64 size)
{
    ka_device_t *dev = NULL;

    dev = hal_kernel_devdrv_get_pci_dev_by_devid(devid);
    if (dev == NULL) {
        rmo_err("Get dev failed. (devid=%u)\n", devid);
        return -ENODEV;
    }

    hal_kernel_devdrv_dma_unmap_page(dev, *((u64 *)mapped_addr->raw_addr.raw_addr), size, KA_DMA_BIDIRECTIONAL);
    return 0;
}
