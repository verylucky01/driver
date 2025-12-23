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
#include <linux/mm.h>
#include <linux/dma-mapping.h>
#include "ascend_hal_define.h"
#include "rmo_auto_init.h"
#include "comm_kernel_interface.h"
#include "pbl_kernel_interface.h"
#include "pbl/pbl_uda.h"
#include "pbl/pbl_task_ctx.h"
#include "dpa/dpa_rmo_kernel.h"
#include "rmo_kern_log.h"
#include "rmo_ioctl.h"
#include "rmo_fops.h"
#include "rmo_mem_sharing.h"

#define RMO_MEM_SHARING_MAX_SIZE 4096  /* 4KB */

static int rmo_mem_get_pa_list(u32 devid, u64 addr, u64 size, u64 *pa_list)
{
    u32 pa_num = 1; /* continuous physical memory */
    return hal_kernel_get_mem_pa_list(devid, current->tgid, addr, size, pa_num, pa_list);
}

static int rmo_mem_put_pa_list(u32 devid, u64 addr, u64 size, u64 *pa_list)
{
    u32 pa_num = 1; /* continuous physical memory */
    return hal_kernel_put_mem_pa_list(devid, current->tgid, addr, size, pa_num, pa_list);
}

static int (*const rmo_mem_get_func[ACCESSOR_MAX])(u32 devid, u64 addr, u64 size, u64 *pa_list) = {
    [TS_ACCESSOR] = rmo_mem_get_pa_list,
};

static int (*const rmo_mem_put_func[ACCESSOR_MAX])(u32 devid, u64 addr, u64 size, u64 *pa_list) = {
    [TS_ACCESSOR] = rmo_mem_put_pa_list,
};

static mem_sharing_func g_mem_sharing_func[ACCESSOR_MAX];

void rmo_mem_sharing_register(mem_sharing_func handle, accessMember_t accessor)
{
    if ((accessor >= 0) && (accessor < ACCESSOR_MAX)) {
        g_mem_sharing_func[accessor] = handle;
        rmo_debug("Register mem dispatch func success. (accessor=%d)\n", accessor);
    }
}
EXPORT_SYMBOL_GPL(rmo_mem_sharing_register);

void rmo_mem_sharing_unregister(accessMember_t accessor)
{
    if ((accessor >= 0) && (accessor < ACCESSOR_MAX)) {
        g_mem_sharing_func[accessor] = NULL;
        rmo_debug("Unregister mem dispatch func success. (accessor=%d)\n", accessor);
    }
}
EXPORT_SYMBOL_GPL(rmo_mem_sharing_unregister);

static int rmo_mng_addr_update(u32 devid, u64 *addr, u64 size)
{
    phys_addr_t paddr = (phys_addr_t)(*addr);
    dma_addr_t dma_addr;
    struct device *dev = NULL;

    dev = hal_kernel_devdrv_get_pci_dev_by_devid(devid);
    if (dev == NULL) {
        rmo_err("Get dev failed. (devid=%u)\n", devid);
        return -ENODEV;
    }

    dma_addr = hal_kernel_devdrv_dma_map_page(dev, pfn_to_page(PFN_DOWN(paddr)), 0, size, DMA_BIDIRECTIONAL);
    if (dma_mapping_error(dev, dma_addr)) {
        rmo_err("Dma_map_page failed. (devid=%u; error=%d; size=%llu)\n",
            devid, dma_mapping_error(dev, dma_addr), size);
        return -ENOMEM;
    }
    *addr = (u64)dma_addr;
    return 0;
}

static int rmo_mem_sharing(struct rmo_cmd_mem_sharing *mem_sharing)
{
    u32 devid = mem_sharing->devid;
    u32 id = (mem_sharing->side == MEM_HOST_SIDE) ? 0 : mem_sharing->devid;
    u64 convert_addr = 0;
    int ret;

    if (mem_sharing->enable_flag == 0) {
        ret = rmo_mem_get_func[mem_sharing->accessor](id, (u64)(uintptr_t)mem_sharing->ptr, mem_sharing->size,
            &convert_addr);
        if (ret != 0) {
            rmo_err("Failed to get addr. (ret=%d; devid=%u; accessor=%u)\n",
                ret, id, mem_sharing->accessor);
            return ret;
        }
        ret = rmo_mng_addr_update(devid, &convert_addr, mem_sharing->size);
        if (ret != 0) {
            rmo_err("Failed to update addr. (ret=%d; devid=%u; accessor=%u)\n",
                ret, devid, mem_sharing->accessor);
            goto err_to_put;
        }
    }

    if (g_mem_sharing_func[mem_sharing->accessor] == NULL) {
        rmo_err("Not register. (devid=%u; accessor=%u)\n", devid, mem_sharing->accessor);
        ret = -ENODEV;
        goto err_to_put;
    }
    ret = g_mem_sharing_func[mem_sharing->accessor](devid, convert_addr, mem_sharing->size);
    if (ret != 0) {
        rmo_err("Failed to dispatch. (ret=%d; devid=%u; accessor=%u; len=%llu; enable_flag=%u)\n",
            ret, devid, mem_sharing->accessor, mem_sharing->size, mem_sharing->enable_flag);
        goto err_to_put;
    }

    if (mem_sharing->enable_flag == 1) {
        ret = rmo_mem_put_func[mem_sharing->accessor](id, (u64)(uintptr_t)mem_sharing->ptr, mem_sharing->size,
            &convert_addr);
        if (ret != 0) {
            rmo_err("Failed to put addr. (ret=%d; devid=%u; accessor=%u)\n",
                ret, id, mem_sharing->accessor);
            return ret;
        }
    }

    rmo_debug("Sharing success. (devid=%u; accessor=%u; len=%llu; enable_flag=%u)\n",
        id, mem_sharing->accessor, mem_sharing->size, mem_sharing->enable_flag);
    return 0;

err_to_put:
    if (mem_sharing->enable_flag == 0) {
        (void)rmo_mem_put_func[mem_sharing->accessor](devid, (u64)(uintptr_t)mem_sharing->ptr, mem_sharing->size,
            &convert_addr);
    }
    return ret;
}

static int rmo_ioctl_mem_sharing(u32 cmd, unsigned long arg)
{
    struct rmo_cmd_mem_sharing *usr_arg = (struct rmo_cmd_mem_sharing __user *)(uintptr_t)arg;
    struct rmo_cmd_mem_sharing mem_sharing;
    int ret;

    ret = (int)copy_from_user(&mem_sharing, usr_arg, sizeof(mem_sharing));
    if (ret != 0) {
        rmo_err("Copy from user failed. (ret=%d)\n", ret);
        return -EFAULT;
    }

    ret = uda_devid_to_udevid(mem_sharing.devid, &mem_sharing.devid);
    if (ret != 0) {
        rmo_err("Invalid devid. (devid=%u)\n", mem_sharing.devid);
        return -ENODEV;
    }

    if (!uda_is_phy_dev(mem_sharing.devid)) {
        return -ENOTSUPP;
    }
    if ((mem_sharing.ptr == NULL) || (mem_sharing.size == 0) || (mem_sharing.size > RMO_MEM_SHARING_MAX_SIZE)) {
        rmo_err("Invalid para. (ptr=%p; size=%llu)\n", mem_sharing.ptr, mem_sharing.size);
        return -EINVAL;
    }

    if ((mem_sharing.accessor < 0) || (mem_sharing.accessor >= ACCESSOR_MAX)) {
        rmo_err("Invalid accessor. (accessor=%d)\n", mem_sharing.accessor);
        return -EINVAL;
    }

    if (mem_sharing.side != MEM_HOST_SIDE) {
        rmo_err("Invalid memory side. (side=%d)\n", mem_sharing.side);
        return -EINVAL;
    }

    if ((mem_sharing.enable_flag != 0) && (mem_sharing.enable_flag != 1)) {
        rmo_err("Invalid enable_flag. (enable_flag=%d)\n", mem_sharing.enable_flag);
        return -EINVAL;
    }

    return rmo_mem_sharing(&mem_sharing);
}

static struct task_ctx_domain *res_mem_sharing_ops_domain = NULL;

int rmo_mem_sharing_init(void)
{
    res_mem_sharing_ops_domain = task_ctx_domain_create("mem_sharing_domain", 0);
    if (res_mem_sharing_ops_domain == NULL) {
        return -ENOMEM;
    }

    rmo_register_ioctl_cmd_func(_IOC_NR(RMO_MEM_SHARING), rmo_ioctl_mem_sharing);

    return 0;
}
DECLAER_FEATURE_AUTO_INIT(rmo_mem_sharing_init, FEATURE_LOADER_STAGE_6);

void rmo_mem_sharing_uninit(void)
{
    task_ctx_domain_destroy(res_mem_sharing_ops_domain);
    res_mem_sharing_ops_domain = NULL;
}
DECLAER_FEATURE_AUTO_UNINIT(rmo_mem_sharing_uninit, FEATURE_LOADER_STAGE_6);
