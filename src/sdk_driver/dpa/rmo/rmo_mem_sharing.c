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
#include "ka_base_pub.h"
#include "ka_common_pub.h"
#include "ka_compiler_pub.h"
#include "ka_ioctl_pub.h"
#include "ka_memory_pub.h"
#include "ka_kernel_def_pub.h"
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
#include "rmo_sched.h"
#include "rmo_mem_sharing_ctx.h"
#include "rmo_mem_sharing.h"

#define RMO_MEM_SHARING_MAX_SIZE 4096  /* 4KB */

static struct task_ctx_domain *res_mem_sharing_ops_domain = NULL;

static inline void rmo_pack_mem_attr(u64 addr, u64 size, struct ka_mem_attr *mem_attr)
{
    mem_attr->addr = addr;
    mem_attr->size = size;
    mem_attr->cp_only_flag = false; /* host mem, not cp */
    mem_attr->raw_pa_flag = true; /* host mem should return raw pa */
}

static int rmo_mem_get_pa_list(u32 devid, u64 addr, u64 size, u64 *pa_list)
{
    struct ka_pa_wraper pa_wraper = {0};
    struct ka_mem_attr mem_attr = {0};
    u64 pa_num = 1; /* continuous physical memory */
    int ret = 0;

    rmo_pack_mem_attr(addr, size, &mem_attr);
    ret = hal_kernel_get_mem_pa_list(devid, ka_task_get_current_tgid(), &mem_attr, &pa_num, &pa_wraper);
    if (ret == 0) {
        *pa_list = pa_wraper.pa;
    }

    return ret;
}

static int rmo_mem_put_pa_list(u32 devid, u64 addr, u64 size, u64 *pa_list)
{
    struct ka_pa_wraper pa_wraper = {.pa = pa_list[0], .size = size};
    struct ka_mem_attr mem_attr = {0};
    u64 pa_num = 1; /* continuous physical memory */

    rmo_pack_mem_attr(addr, size, &mem_attr);
    return hal_kernel_put_mem_pa_list(devid, ka_task_get_current_tgid(), &mem_attr, pa_num, &pa_wraper);
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
KA_EXPORT_SYMBOL_GPL(rmo_mem_sharing_register);

void rmo_mem_sharing_unregister(accessMember_t accessor)
{
    if ((accessor >= 0) && (accessor < ACCESSOR_MAX)) {
        g_mem_sharing_func[accessor] = NULL;
        rmo_debug("Unregister mem dispatch func success. (accessor=%d)\n", accessor);
    }
}
KA_EXPORT_SYMBOL_GPL(rmo_mem_sharing_unregister);

static int rmo_mem_sharing_func_proc(u32 devid, struct rmo_mem_raw_addr *raw_addr,
    struct rmo_cmd_mem_sharing *mem_sharing)
{
    int ret;

    if (g_mem_sharing_func[mem_sharing->accessor] == NULL) {
        rmo_err("Not register. (devid=%u; accessor=%u)\n", devid, mem_sharing->accessor);
        return -ENODEV;
    }

    ret = g_mem_sharing_func[mem_sharing->accessor](devid, raw_addr, mem_sharing->size);
    if (ret != 0) {
        rmo_err("Failed to share. (ret=%d; devid=%u; accessor=%u; len=%llu; enable_flag=%u)\n",
            ret, devid, mem_sharing->accessor, mem_sharing->size, mem_sharing->enable_flag);
    }
    return ret;
}

static int rmo_mem_sharing_enable(struct rmo_cmd_mem_sharing *mem_sharing)
{
    struct rmo_mem_sharing_info info = {0};
    struct rmo_mem_map_addr convert_addr = {0};
    u32 devid = mem_sharing->devid;
    u32 id = uda_get_host_id();
    int tgid = ka_task_get_current_tgid();
    u64 paddr;
    int ret;

    ret = rmo_mem_get_func[mem_sharing->accessor](id, (u64)(uintptr_t)mem_sharing->ptr, mem_sharing->size, &paddr);
    if (ret != 0) {
        rmo_err("Failed to get addr. (ret=%d; devid=%u; accessor=%u; tgid=%d)\n",
            ret, id, mem_sharing->accessor, tgid);
        return ret;
    }

    ret = rmo_mem_addr_map(devid, paddr, mem_sharing->size, &convert_addr);
    if (ret != 0) {
        rmo_err("Failed to update addr. (ret=%d; devid=%u; accessor=%u; tgid=%d)\n",
            ret, devid, mem_sharing->accessor, tgid);
        goto err_to_put;
    }

    ret = rmo_mem_sharing_func_proc(devid, &convert_addr.raw_addr, mem_sharing);
    if (ret != 0) {
        rmo_err("Failed to share. (ret=%d; devid=%u; accessor=%u; len=%llu; tgid=%d)\n",
            ret, devid, mem_sharing->accessor, mem_sharing->size, tgid);
        goto err_to_unmap;
    }

    info.sharing_pa = paddr;
    info.convert_addr = convert_addr;
    info.mem_shr = *mem_sharing;
    ret = rmo_mem_sharing_add_node(res_mem_sharing_ops_domain, tgid, &info);
    if (ret != 0) {
        rmo_err("Failed to add node. (ret=%d; tgid=%d; devid=%u; accessor=%u; tgid=%d)\n",
            ret, tgid, devid, mem_sharing->accessor, tgid);
        goto err_to_func;
    }

    rmo_debug("Enable success. (devid=%u; accessor=%u; len=%llu; enable_flag=%u; tgid=%d)\n",
        devid, mem_sharing->accessor, mem_sharing->size, mem_sharing->enable_flag, tgid);
    return 0;

err_to_func:
    (void)rmo_mem_sharing_func_proc(devid, NULL, mem_sharing);
err_to_unmap:
    (void)rmo_mem_addr_unmap(devid, &convert_addr, mem_sharing->size);
err_to_put:
    (void)rmo_mem_put_func[mem_sharing->accessor](id, (u64)(uintptr_t)mem_sharing->ptr, mem_sharing->size, &paddr);
    return ret;
}

static int rmo_mem_sharing_disable(struct rmo_cmd_mem_sharing *mem_sharing)
{
    struct rmo_mem_sharing_info info;
    int tgid = ka_task_get_current_tgid();
    u32 devid = mem_sharing->devid;
    u32 id = uda_get_host_id();
    int ret;

    info.mem_shr = *mem_sharing;
    ret = rmo_mem_sharing_query_node(res_mem_sharing_ops_domain, tgid, &info);
    if (ret != 0) {
        rmo_err("Failed to find node. (ret=%d; devid=%u; accessor=%u; len=%llu; tgid=%d)\n",
            ret, devid, mem_sharing->accessor, mem_sharing->size, tgid);
        return ret;
    }

    ret = rmo_mem_sharing_del_node(res_mem_sharing_ops_domain, tgid, &info);
    if (ret != 0) {
        rmo_err("Failed to del node. (ret=%d; devid=%u; accessor=%u; len=%llu; tgid=%d)\n",
            ret, devid, mem_sharing->accessor, mem_sharing->size, tgid);
        return ret;
    }

    ret = rmo_mem_sharing_func_proc(devid, NULL, mem_sharing);
    if (ret != 0) {
        (void)rmo_mem_sharing_add_node(res_mem_sharing_ops_domain, tgid, &info);
        rmo_err("Failed to share. (ret=%d; devid=%u; accessor=%u; len=%llu; tgid=%d)\n",
            ret, devid, mem_sharing->accessor, mem_sharing->size, tgid);
        return ret;
    }

    (void)rmo_mem_addr_unmap(devid, &info.convert_addr, mem_sharing->size);
    ret = rmo_mem_put_func[mem_sharing->accessor](id, (u64)(uintptr_t)mem_sharing->ptr, mem_sharing->size,
        &info.sharing_pa);
    if (ret != 0) {
        rmo_warn("Put addr warnning. (ret=%d; devid=%u; accessor=%u)\n",
            ret, id, mem_sharing->accessor);
    } else {
        rmo_debug("Disable success. (devid=%u; accessor=%u; len=%llu; enable_flag=%u; tgid=%d)\n",
            devid, mem_sharing->accessor, mem_sharing->size, mem_sharing->enable_flag, tgid);
    }
    return 0;
}

static int rmo_mem_sharing(struct rmo_cmd_mem_sharing *mem_sharing)
{
    if (mem_sharing->enable_flag == 0) {
        return rmo_mem_sharing_enable(mem_sharing);
    } else {
        return rmo_mem_sharing_disable(mem_sharing);
    }
}

static int rmo_ioctl_mem_sharing(u32 cmd, unsigned long arg)
{
    struct rmo_cmd_mem_sharing *usr_arg = (struct rmo_cmd_mem_sharing __ka_user *)(uintptr_t)arg;
    struct rmo_cmd_mem_sharing mem_sharing;
    int ret;

    ret = (int)ka_base_copy_from_user(&mem_sharing, usr_arg, sizeof(mem_sharing));
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
        return -EOPNOTSUPP;
    }

    if ((mem_sharing.ptr == NULL) || (mem_sharing.size == 0) || (mem_sharing.size > RMO_MEM_SHARING_MAX_SIZE) ||
        (((u64)(uintptr_t)(mem_sharing.ptr) & (KA_MM_PAGE_SIZE - 1)) != 0)) {
        rmo_err("Invalid para. (ptr=%p; size=%llu)\n", mem_sharing.ptr, mem_sharing.size);
        return -EINVAL;
    }

    if ((mem_sharing.accessor < 0) || (mem_sharing.accessor >= ACCESSOR_MAX)) {
        return -EOPNOTSUPP;
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

void rmo_mem_sharing_domain_task_exit(u32 udevid, int tgid, struct task_start_time *start_time)
{
    rmo_mem_sharing_ctx_destroy(res_mem_sharing_ops_domain, tgid);
}
DECLAER_FEATURE_AUTO_UNINIT_TASK(rmo_mem_sharing_domain_task_exit, FEATURE_LOADER_STAGE_1);

int rmo_mem_sharing_init(void)
{
    res_mem_sharing_ops_domain = task_ctx_domain_create("mem_sharing_domain", 0);
    if (res_mem_sharing_ops_domain == NULL) {
        return -ENOMEM;
    }

    rmo_register_ioctl_cmd_func(_KA_IOC_NR(RMO_MEM_SHARING), rmo_ioctl_mem_sharing);

    return 0;
}
DECLAER_FEATURE_AUTO_INIT(rmo_mem_sharing_init, FEATURE_LOADER_STAGE_6);

void rmo_mem_sharing_uninit(void)
{
    task_ctx_domain_destroy(res_mem_sharing_ops_domain);
    res_mem_sharing_ops_domain = NULL;
}
DECLAER_FEATURE_AUTO_UNINIT(rmo_mem_sharing_uninit, FEATURE_LOADER_STAGE_6);
