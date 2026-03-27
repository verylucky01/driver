/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <sys/mman.h>
#include <errno.h>
#include <securec.h>

#include "ascend_hal_define.h"
#include "ascend_hal.h"
#include "pbl_uda_user.h"
#include "dpa_apm.h"

#include "svm_pub.h"
#include "svm_addr_desc.h"
#include "svm_log.h"
#include "va_allocator.h"
#include "va_mng.h"
#include "svm_user_adapt.h"
#include "malloc_mng.h"
#include "svm_register_to_master.h"
#include "svm_memcpy.h"
#include "svm_memcpy_local_client.h"
#include "svm_dbi.h"
#include "svm_ipc.h"
#include "svm_vmm.h"

enum svm_copy_shared_type {
    SVM_COPY_SHARED_NONE = 0,
    SVM_COPY_SHARED_IPC,
    SVM_COPY_SHARED_VMM,
};

struct svm_copy_shared_info {
    enum svm_copy_shared_type type;
    struct svm_copy_va_info map_info;
    struct svm_copy_va_info real_info;
};

static int svm_copy_check_addr_prop_consistency(struct svm_prop *tar_prop, u64 start, u64 end)
{
    struct svm_prop prop;
    u64 va;
    int ret;

    for (va = start; va < end; va += prop.size) {
        ret = svm_get_prop(va, &prop);
        if (ret != DRV_ERROR_NONE) {
            svm_err("Invalid addr. (ret=%d; va=0x%llx)\n", ret, va);
            return DRV_ERROR_PARA_ERROR; /* Same to OBP. */
        }

        if (((prop.flag & ~SVM_FLAG_ATTR_PA_P2P) != (tar_prop->flag & ~SVM_FLAG_ATTR_PA_P2P)) ||
            (prop.devid != tar_prop->devid)) {
            svm_err("Addr's prop is not consistent.\n");
            return DRV_ERROR_PARA_ERROR;
        }
    }

    return DRV_ERROR_NONE;
}

static int svm_copy_check_addr_prop_cap(u64 va, u64 size, u64 cap_mask)
{
    struct svm_prop prop;
    int va_type;
    int ret;

    ret = svm_get_va_type(va, size, &va_type);
    if (ret != 0) {
        svm_err("Invalid addr. (va=0x%llx; size=0x%llx)\n", va, size);
        return ret;
    }

    if (va_type == VA_TYPE_SVM) {
        ret = svm_get_prop(va, &prop);
        if (ret != DRV_ERROR_NONE) {
            /* The log cannot be modified, because in the failure mode library. */
            svm_err("Invalid addr. (ret=%d; va=0x%llx)\n", ret, va);
            return ret;
        }

        if ((prop.flag & cap_mask) == 0) {
            svm_err("Va not support cur cap. (va=0x%llx; prop.flag=0x%llx)\n", va, prop.flag);
            return DRV_ERROR_PARA_ERROR;
        }

        if ((va + size) > (prop.start + prop.size)) {
            ret = svm_copy_check_addr_prop_consistency(&prop, prop.start + prop.size, va + size);
            if (ret != DRV_ERROR_NONE) {
                return ret;
            }
        }
    }

    return DRV_ERROR_NONE;
}

/* Should check va prop consistency firstly. */
static int svm_set_copy_va_info(u64 va, u64 size, struct svm_copy_va_info *info)
{
    struct svm_prop prop;
    int ret;

    info->va = va;
    info->size = size;
    info->host_tgid = 0;
    info->is_share = false;

    if (svm_va_is_in_range(va, size)) {
        ret = svm_get_prop(va, &prop);
        if (ret != DRV_ERROR_NONE) {
            svm_err("Invalid addr. (ret=%d; va=0x%llx)\n", ret, va);
            return ret;
        }

        info->devid = prop.devid;
    } else {
        info->devid = svm_get_host_devid();
    }

    return DRV_ERROR_NONE;
}

static bool svm_copy_shared_info_is_valid(const struct svm_copy_shared_info *info)
{
    return (info->type != SVM_COPY_SHARED_NONE);
}

static bool svm_copy_need_try_resolve_shared_info(u32 devid)
{
    /* tmp for pcie, ub adapt later */
    return ((devid != svm_get_host_devid()) && (svm_get_device_connect_type(devid) == HOST_DEVICE_CONNECT_TYPE_PCIE));
}

static int svm_copy_real_info_pack(const struct svm_global_va *src_info, struct svm_copy_va_info *info)
{
    u32 host_tgid, proc_type;
    int ret;

    if ((src_info->server_id != SVM_INVALID_SERVER_ID) && (src_info->server_id != svm_get_cur_server_id())) {
        svm_info("Cross server shared memcpy is not support. (server_id=%u; cur_server_id=%u)\n",
            src_info->server_id, svm_get_cur_server_id());
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = uda_get_devid_by_udevid_ex(src_info->udevid, &info->devid);
    if (ret != 0) {
        svm_err("Get devid by udevid failed. (ret=%d; udevid=%u)\n", ret, src_info->udevid);
        return ret;
    }

    ret = halQueryMasterPidByDeviceSlave(info->devid, src_info->tgid, &host_tgid, &proc_type);
    if (ret != 0) {
        svm_err("Get master pid by device slave failed. (ret=%d; devid=%u; slave_pid=%d)\n",
            ret, info->devid, src_info->tgid);
        return ret;
    }

    info->host_tgid = (int)host_tgid;
    info->va = src_info->va;
    info->size = src_info->size;
    info->is_share = true;
    return DRV_ERROR_NONE;
}

static int svm_copy_try_query_shared_info(struct svm_copy_va_info *info, struct svm_copy_shared_info *shared)
{
    struct svm_global_va src_info;
    enum svm_copy_shared_type type;
    int ret;

    shared->type = SVM_COPY_SHARED_NONE;
    shared->map_info = *info;
    shared->real_info = *info;

    ret = svm_ipc_query_src_info(info->va, info->size, &src_info);
    if (ret == DRV_ERROR_NONE) {
        type = SVM_COPY_SHARED_IPC;
    } else {
        ret = vmm_query_ipc_src_info(info->va, info->size, &src_info);
        if (ret != DRV_ERROR_NONE) {
            return DRV_ERROR_NONE; /* Non shared addr, just return ok to continue copy. */
        }
        type = SVM_COPY_SHARED_VMM;
    }

    ret = svm_copy_real_info_pack(&src_info, &shared->real_info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }
    shared->type = type;

    return DRV_ERROR_NONE;
}

static int svm_copy_try_resolve_shared_info(struct svm_copy_va_info *src_info, struct svm_copy_va_info *dst_info,
    struct svm_copy_va_info *real_src, struct svm_copy_va_info *real_dst, bool *resolved)
{
    struct svm_copy_shared_info src_shared, dst_shared;
    bool src_is_shared, dst_is_shared;
    int ret;

    ret = svm_copy_try_query_shared_info(src_info, &src_shared);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_copy_try_query_shared_info(dst_info, &dst_shared);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    src_is_shared = svm_copy_shared_info_is_valid(&src_shared);
    dst_is_shared = svm_copy_shared_info_is_valid(&dst_shared);
    if (!src_is_shared && !dst_is_shared) {
        *resolved = false;
        return DRV_ERROR_NONE;
    }

    if (src_is_shared && dst_is_shared) {
        svm_info("Shared src and dst are not support. (src=0x%llx; dst=0x%llx)\n", src_info->va, dst_info->va);
        return DRV_ERROR_NOT_SUPPORT;
    }

    *real_src = src_is_shared ? src_shared.real_info : *src_info;
    *real_dst = dst_is_shared ? dst_shared.real_info : *dst_info;
    real_src->is_share = src_is_shared;
    real_dst->is_share = dst_is_shared;
    *resolved = true;
    return DRV_ERROR_NONE;
}

static int svm_mem_sync_copy_local_resolved(struct svm_copy_va_info *src_info, struct svm_copy_va_info *dst_info)
{
    int cur_tgid = (int)drvDeviceGetBareTgid();

    if (((src_info->host_tgid != 0) && (src_info->host_tgid != cur_tgid)) ||
        ((dst_info->host_tgid != 0) && (dst_info->host_tgid != cur_tgid))) {
        svm_info("Resolved local copy tgid not match current process. (src_host_tgid=%d; dst_host_tgid=%d; cur_tgid=%d)\n",
            src_info->host_tgid, dst_info->host_tgid, cur_tgid);
        return DRV_ERROR_NOT_SUPPORT; /* todo: later adapt */
    }

    return svm_memcpy_local_client(dst_info->devid, dst_info->va, dst_info->size, src_info->va, src_info->size);
}

/* Should check va prop consistency firstly. */
static int svm_set_copy_va_2d_info(u64 va, u64 pitch, u64 width, u64 height, struct svm_copy_va_2d_info *info)
{
    struct svm_prop prop;
    int ret;

    info->va = va;
    info->pitch = pitch;
    info->width = width;
    info->height = height;

    if (svm_va_is_in_range(va, pitch * height)) {
        ret = svm_get_prop(va, &prop);
        if (ret != DRV_ERROR_NONE) {
            svm_err("Invalid addr. (ret=%d; va=0x%llx)\n", ret, va);
            return ret;
        }

        info->devid = prop.devid;
    } else {
        info->devid = svm_get_host_devid();
    }

    return DRV_ERROR_NONE;
}
#ifdef CFG_FEATURE_ASYNC_COPY
static int svm_async_copy_para_check(u64 dst, u64 dst_max, u64 src, u64 size, u64 *handle)
{
    int ret;

    if (src == 0) {
        svm_err("src is 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (dst == 0) {
        svm_err("dst is 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (dst_max == 0) {
        svm_err("dst_max is 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (size == 0) {
        svm_err("cnt is 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (dst_max < size) {
        svm_err("Dst_max less than size. (dst_max=%llu; size=%llu)\n", dst_max, size);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (handle == NULL) {
        svm_err("handle is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = svm_copy_check_addr_prop_cap(dst, size, SVM_FLAG_CAP_ASYNC_COPY_SUBMIT);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return svm_copy_check_addr_prop_cap(src, size, SVM_FLAG_CAP_ASYNC_COPY_SUBMIT);
}
#endif
DVresult halMemCpyAsync(DVdeviceptr dst, size_t dest_max, DVdeviceptr src, size_t byte_count,
    uint64_t *copy_fd)
{
#ifdef CFG_FEATURE_ASYNC_COPY
    struct svm_copy_va_info src_info, dst_info;
    struct svm_copy_va_info real_src, real_dst;
    bool resolved = false;
    enum svm_cpy_dir dir;
    int ret;

    ret = svm_async_copy_para_check(dst, dest_max, src, byte_count, (u64 *)copy_fd);
    if (ret != 0) {
        return ret;
    }

    ret = svm_set_copy_va_info((u64)src, (u64)byte_count, &src_info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_set_copy_va_info((u64)dst, (u64)byte_count, &dst_info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_copy_try_resolve_shared_info(&src_info, &dst_info, &real_src, &real_dst, &resolved);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    if (resolved) {
        svm_info("Async memcpy does not support shared addr. (src=0x%llx; dst=0x%llx; size=%llu)\n",
            src_info.va, dst_info.va, src_info.size);
        return DRV_ERROR_NOT_SUPPORT;
    }

    dir = copy_dir_get_by_devid(src_info.devid, dst_info.devid);
    if (dir == SVM_H2H_CPY) {
        svm_err("Invalid input, no support H2H async memcpy.\n");
        return DRV_ERROR_PARA_ERROR;
    }

    return svm_async_copy_submit(&src_info, &dst_info, (u64 *)copy_fd);
#else
    SVM_UNUSED(dst);
    SVM_UNUSED(dest_max);
    SVM_UNUSED(src);
    SVM_UNUSED(byte_count);
    SVM_UNUSED(copy_fd);

    return DRV_ERROR_NOT_SUPPORT;
#endif
}

DVresult halMemCpyAsyncWaitFinish(uint64_t copy_fd)
{
#ifdef CFG_FEATURE_ASYNC_COPY
    return svm_async_copy_wait((u64)copy_fd);
#else
    SVM_UNUSED(copy_fd);

    return DRV_ERROR_NOT_SUPPORT;
#endif
}

static int svm_mem_sync_copy_h2h(struct svm_copy_va_info *src_info, struct svm_copy_va_info *dst_info)
{
    return svm_memcpy_local_client(dst_info->devid, dst_info->va, dst_info->size, src_info->va, src_info->size);
}

static int _svm_mem_sync_copy(enum svm_cpy_dir dir, struct svm_copy_va_info *src_info,
    struct svm_copy_va_info *dst_info)
{
    struct svm_dst_va register_va;
    u64 host_va = (dir == SVM_H2D_CPY) ? src_info->va : dst_info->va;
    u64 size = src_info->size;
    u32 user_devid = (dir == SVM_H2D_CPY) ? dst_info->devid : src_info->devid;
    u32 flag = 0;
    bool is_register = false;
    int ret;

    svm_dst_va_pack(svm_get_host_devid(), 0, host_va, size, &register_va);

    if (svm_va_is_in_range(host_va, size) == false) {
        flag |= (dir == SVM_H2D_CPY) ? 0 : REGISTER_TO_MASTER_FLAG_ACCESS_WRITE;
        flag |= REGISTER_TO_MASTER_FLAG_PIN;

        ret = svm_register_to_master(user_devid, &register_va, flag);
        if ((ret != DRV_ERROR_NONE) && (ret != DRV_ERROR_NOT_SUPPORT) && (ret != DRV_ERROR_BUSY)) {
            svm_err("Register local mem to master failed. (ret=%d; host_va=0x%llx; size=%llu)\n", ret, host_va, size);
            return ret;
        }

        if (ret == DRV_ERROR_NONE) {
            is_register = true;
        }
    }

    ret = svm_sync_copy(src_info, dst_info);
    if (is_register) {
        (void)svm_unregister_to_master(user_devid, &register_va, flag);
    }

    return ret;
}

static int svm_mem_sync_copy_h2d(struct svm_copy_va_info *src_info, struct svm_copy_va_info *dst_info)
{
    return _svm_mem_sync_copy(SVM_H2D_CPY, src_info, dst_info);
}

static int svm_mem_sync_copy_d2h(struct svm_copy_va_info *src_info, struct svm_copy_va_info *dst_info)
{
    return _svm_mem_sync_copy(SVM_D2H_CPY, src_info, dst_info);
}

static int svm_mem_sync_copy_same_d2d(struct svm_copy_va_info *src_info, struct svm_copy_va_info *dst_info)
{
    return svm_memcpy_local_client(dst_info->devid, dst_info->va, dst_info->size, src_info->va, src_info->size);
}

static int svm_ub_sync_copy_diff_d2d(struct svm_copy_va_info *src_info, struct svm_copy_va_info *dst_info)
{
    struct svm_copy_va_info host_info;
    void *host_va = NULL;
    int ret;

    host_va = svm_user_mmap(NULL, src_info->size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (host_va == NULL) {
        svm_warn("Alloc d2d local mem failed.\n");
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    host_info.devid = svm_get_host_devid();
    host_info.va = (u64)(uintptr_t)host_va;
    host_info.size = src_info->size;
    host_info.host_tgid = 0;
    host_info.is_share = false;

    ret = svm_mem_sync_copy_d2h(src_info, &host_info);
    if (ret != DRV_ERROR_NONE) {
        svm_err("D2H cpy failed. (ret=%d)\n", ret);
        goto free_host_va;
    }

    ret = svm_mem_sync_copy_h2d(&host_info, dst_info);
    if (ret != DRV_ERROR_NONE) {
        svm_err("H2D cpy failed. (ret=%d)\n", ret);
        goto free_host_va;
    }

free_host_va:
    svm_user_munmap(host_va, src_info->size);
    return ret;
}

static int svm_mem_sync_copy_diff_d2d(struct svm_copy_va_info *src_info, struct svm_copy_va_info *dst_info)
{
    u32 hd_connect_type = svm_get_device_connect_type(src_info->devid);
    if ((hd_connect_type == HOST_DEVICE_CONNECT_TYPE_PCIE) ||
        (hd_connect_type == HOST_DEVICE_CONNECT_TYPE_HCCS)) {
        return svm_sync_copy(src_info, dst_info);
    } else if (hd_connect_type == HOST_DEVICE_CONNECT_TYPE_UB) {
        return svm_ub_sync_copy_diff_d2d(src_info, dst_info);
    } else {
        return DRV_ERROR_INNER_ERR;
    }
}

static int svm_mem_sync_copy_d2d(struct svm_copy_va_info *src_info, struct svm_copy_va_info *dst_info)
{
    if (src_info->devid == dst_info->devid) {
        return svm_mem_sync_copy_same_d2d(src_info, dst_info);
    } else {
        return svm_mem_sync_copy_diff_d2d(src_info, dst_info);
    }
}

static int (* const g_sync_copy[SVM_MAX_CPY_DIR])
    (struct svm_copy_va_info *src_info, struct svm_copy_va_info *dst_info) = {
    [SVM_H2H_CPY] = svm_mem_sync_copy_h2h,
    [SVM_H2D_CPY] = svm_mem_sync_copy_h2d,
    [SVM_D2H_CPY] = svm_mem_sync_copy_d2h,
    [SVM_D2D_CPY] = svm_mem_sync_copy_d2d
};

int svm_mem_sync_copy(struct svm_copy_va_info *src_info, struct svm_copy_va_info *dst_info)
{
    enum svm_cpy_dir dir = copy_dir_get_by_devid(src_info->devid, dst_info->devid);
    if (g_sync_copy[dir] == NULL) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    return g_sync_copy[dir](src_info, dst_info);
}

static int svm_memcpy_sync(struct svm_copy_va_info *src_info, struct svm_copy_va_info *dst_info)
{
    struct svm_copy_va_info real_src, real_dst;
    enum svm_cpy_dir dir;
    bool resolved = false;
    u32 exec_devid;
    int ret;

    dir = copy_dir_get_by_devid(src_info->devid, dst_info->devid);
    if ((dir == SVM_H2H_CPY) || ((dir == SVM_D2D_CPY) && (src_info->devid == dst_info->devid))) {
        return svm_mem_sync_copy(src_info, dst_info);
    }

    exec_devid = (dir == SVM_H2D_CPY) ? dst_info->devid : src_info->devid;
    if (svm_copy_need_try_resolve_shared_info(exec_devid)) {
        ret = svm_copy_try_resolve_shared_info(src_info, dst_info, &real_src, &real_dst, &resolved);
        if (ret != DRV_ERROR_NONE) {
            return ret;
        }

        if (resolved) {
            dir = copy_dir_get_by_devid(real_src.devid, real_dst.devid);
            if ((dir == SVM_H2H_CPY) || ((dir == SVM_D2D_CPY) && (real_src.devid == real_dst.devid))) {
                return svm_mem_sync_copy_local_resolved(&real_src, &real_dst);
            }
            *src_info = real_src;
            *dst_info = real_dst;
        }
    }

    return svm_mem_sync_copy(src_info, dst_info);
}

DVresult drvMemcpyInner(DVdeviceptr dst, size_t dest_max, DVdeviceptr src, size_t byte_count);
DVresult drvMemcpyInner(DVdeviceptr dst, size_t dest_max, DVdeviceptr src, size_t byte_count)
{
    struct svm_copy_va_info src_info, dst_info;
    int ret;

    if ((dst == 0) || (src == 0) || (dest_max == 0) || (dest_max < byte_count)) {
        svm_err("Invalid argument. (dst=0x%llx; src=0x%llx; byte_count=%lu; dest_max=%lu)\n",
            dst, src, byte_count, dest_max);
        return DRV_ERROR_INVALID_VALUE;
    }

    /* Similar to memcpy_s, (byte_count == 0) need to return 0 */
    if (byte_count == 0) {
        return DRV_ERROR_NONE;
    }

    ret = svm_copy_check_addr_prop_cap(dst, byte_count, SVM_FLAG_CAP_SYNC_COPY);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_copy_check_addr_prop_cap(src, byte_count, SVM_FLAG_CAP_SYNC_COPY);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_set_copy_va_info((u64)src, (u64)byte_count, &src_info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_set_copy_va_info((u64)dst, (u64)byte_count, &dst_info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return svm_memcpy_sync(&src_info, &dst_info);
}

DVresult drvMemcpy(DVdeviceptr dst, size_t dest_max, DVdeviceptr src, size_t byte_count)
{
    return drvMemcpyInner(dst, dest_max, src, byte_count);
}

static int svm_mem_cpy_para_check(u64 dst, u64 dst_size, u64 src, u64 count,
    struct memcpy_info *info)
{
    if (dst == 0) {
        svm_err("Dst is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (src == 0) {
        svm_err("Src is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (dst_size == 0) {
        svm_err("Dst size is zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (dst_size < count) {
        svm_err("Dst size larger than cpy size. (dst_size=%lu; count=%lu)\n", dst_size, count);
        return DRV_ERROR_INVALID_VALUE;
    }

    /* drvMemcpy */
    if (info == NULL) {
        int ret = svm_copy_check_addr_prop_cap(dst, count, SVM_FLAG_CAP_SYNC_COPY_EX);
        if (ret != DRV_ERROR_NONE) {
            return ret;
        }
        return svm_copy_check_addr_prop_cap(src, count, SVM_FLAG_CAP_SYNC_COPY_EX);
    } else {
        if (info->dir != DRV_MEMCPY_DEVICE_TO_HOST) {
            svm_info("Invalid dir type. (dir_type=%u)\n", info->dir);
            return DRV_ERROR_NOT_SUPPORT;
        }

        if (info->devid >= SVM_MAX_AGENT_NUM) {
            svm_err("Invalid devid. (devid=%u)\n", info->devid);
            return DRV_ERROR_INVALID_VALUE;
        }

        if (!svm_va_is_in_range(src, count) && !va_is_in_sp_range(src, count)) {
            return DRV_ERROR_NOT_SUPPORT;
        }

        return svm_copy_check_addr_prop_cap(dst, count, SVM_FLAG_CAP_SYNC_COPY_EX);
    }
}

drvError_t halMemcpy(void *dst, size_t dst_size, void *src, size_t count, struct memcpy_info *info)
{
    struct svm_copy_va_info src_info, dst_info;
    enum svm_cpy_dir dir;
    drvError_t ret;

    /* Similar to memcpy_s, (count == 0) need to return 0 */
    if (count == 0) {
        return DRV_ERROR_NONE;
    }

    ret = svm_mem_cpy_para_check((u64)dst, (u64)dst_size, (u64)src, (u64)count, info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    if (va_is_in_sp_range((u64)src, (u64)count)) {
        svm_copy_va_info_pack((u64)src, (u64)count, info->devid, &src_info);
    } else {
        ret = svm_set_copy_va_info((u64)src, (u64)count, &src_info);
        if (ret != DRV_ERROR_NONE) {
            return ret;
        }
    }

    ret = svm_set_copy_va_info((u64)dst, (u64)count, &dst_info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    if (info == NULL) {
        return svm_mem_sync_copy(&src_info, &dst_info);
    } else {
        /* D2H copy sp addr. */
        dir = copy_dir_get_by_devid(src_info.devid, dst_info.devid);
        if (dir != (enum svm_cpy_dir)info->dir) {
            svm_err("Invalid addr dir. (dir=%d; info.dir=%d)\n", dir, info->dir);
            return DRV_ERROR_INVALID_VALUE;
        }

        return svm_mem_sync_copy_d2h(&src_info, &dst_info);
    }
}

static drvError_t svm_dma_desc_convert_para_check(u64 src, u64 dst, u64 size, struct DMA_ADDR *dma_desc)
{
    int ret;

    if (src == 0) {
        svm_err("src is 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (dst == 0) {
        svm_err("dst is 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (size == 0) {
        svm_err("size is 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (dma_desc == NULL) {
        svm_err("DmaAddr is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (dma_desc->offsetAddr.devid >= SVM_MAX_AGENT_NUM) {
        svm_err("Invalid devid. (devid=%u)\n", dma_desc->offsetAddr.devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = svm_copy_check_addr_prop_cap(dst, size, SVM_FLAG_CAP_DMA_DESC_CONVERT);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return svm_copy_check_addr_prop_cap(src, size, SVM_FLAG_CAP_DMA_DESC_CONVERT);
}

static int svm_dma_desc_dir_check(u32 exec_devid, u32 src_devid, u32 dst_devid)
{
    enum svm_cpy_dir dir = copy_dir_get_by_devid(src_devid, dst_devid);
    int ret;

    if (dir == SVM_H2H_CPY) {
        svm_err("Invalid input, no support H2H convert.\n");
        return DRV_ERROR_PARA_ERROR;
    }

    if ((dir == SVM_D2D_CPY) && (src_devid == dst_devid)) {
        svm_err("Invalid input, Src and dst are same device. (devid=%d)\n", src_devid);
        return DRV_ERROR_PARA_ERROR;
    }

    if (dir == SVM_H2D_CPY) {
        ret = (exec_devid != dst_devid) ? DRV_ERROR_INVALID_VALUE : 0;
    } else if (dir == SVM_D2H_CPY) {
        ret = (exec_devid != src_devid) ? DRV_ERROR_INVALID_VALUE : 0;
    } else if (dir == SVM_D2D_CPY) {
        ret = ((exec_devid != src_devid) && (exec_devid != dst_devid)) ? DRV_ERROR_INVALID_VALUE : 0;
    }
    if (ret != DRV_ERROR_NONE) {
        svm_err("Exec_devid does not match the direction. (exec_devid=%u; src_devid=%u; dst_devid=%u; dir=%d)\n",
            exec_devid, src_devid, dst_devid, dir);
        return ret;
    }

    return DRV_ERROR_NONE;
}

DVresult drvMemConvertAddr(DVdeviceptr p_src, DVdeviceptr p_dst, UINT32 len, struct DMA_ADDR *dma_addr)
{
    struct svm_copy_va_info src_info, dst_info;
    struct svm_copy_va_info real_src, real_dst;
    bool resolved = false;
    int ret;

    ret = svm_dma_desc_convert_para_check(p_src, p_dst, len, dma_addr);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_set_copy_va_info((u64)p_src, (u64)len, &src_info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_set_copy_va_info((u64)p_dst, (u64)len, &dst_info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    if (svm_copy_need_try_resolve_shared_info(dma_addr->offsetAddr.devid)) {
        ret = svm_copy_try_resolve_shared_info(&src_info, &dst_info, &real_src, &real_dst, &resolved);
        if (ret != DRV_ERROR_NONE) {
            return ret;
        }

        if (resolved) {
            src_info = real_src;
            dst_info = real_dst;
        }
    }

    ret = svm_dma_desc_dir_check(dma_addr->offsetAddr.devid, src_info.devid, dst_info.devid);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return svm_dma_desc_convert(&src_info, &dst_info, dma_addr);
}

static drvError_t svm_dma_desc_para_check(struct DMA_ADDR *dma_desc[], uint32_t num)
{
    uint32_t i;

    if (dma_desc == NULL) {
        svm_err("Ptr[] is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (num == 0) {
        svm_err("num is 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    for (i = 0; i < num; i++) {
        if (dma_desc[i] == NULL) {
            svm_err("Ptr is NULL.\n");
            return DRV_ERROR_INVALID_VALUE;
        }

        /* host agent not support convert */
        if (dma_desc[i]->virt_id >= SVM_MAX_DEV_AGENT_NUM) {
            svm_err("Virt_id is out of range. (virt_id=%u; range=0-%u)\n",
                dma_desc[i]->virt_id, (UINT32)SVM_MAX_DEV_AGENT_NUM);
            return DRV_ERROR_INVALID_VALUE;
        }
    }

    return DRV_ERROR_NONE;
}

DVresult drvMemDestroyAddr(struct DMA_ADDR *ptr)
{
    int ret;

    ret = svm_dma_desc_para_check(&ptr, 1);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return svm_dma_desc_destroy(ptr);
}

DVresult halMemDestroyAddrBatch(struct DMA_ADDR *ptr[], uint32_t num)
{
    int ret = DRV_ERROR_NONE;
    u32 i;

    ret = svm_dma_desc_para_check(ptr, num);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    for (i = 0; i < num; i++) {
        ret = drvMemDestroyAddr(ptr[i]);
        if (ret != DRV_ERROR_NONE) {
            break;
        }
    }

    return ret;
}

DVresult halMemcpySumbit(struct DMA_ADDR *dma_addr, int flag)
{
#ifdef CFG_FEATURE_DMA_DESC_SUBMIT
    int ret;

    ret = svm_dma_desc_para_check(&dma_addr, 1);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    if ((flag >= (int)MEMCPY_SUMBIT_MAX_TYPE) || (flag < (int)MEMCPY_SUMBIT_SYNC)) {
        svm_err("Invalid flag. (flag=%d)\n", flag);
        return DRV_ERROR_INVALID_VALUE;
    }

    return svm_dma_desc_submit(dma_addr, flag);
#else
    SVM_UNUSED(dma_addr);
    SVM_UNUSED(flag);

    return DRV_ERROR_NOT_SUPPORT;
#endif
}

DVresult halMemcpyWait(struct DMA_ADDR *dma_addr)
{
#ifdef CFG_FEATURE_DMA_DESC_SUBMIT
    int ret;

    ret = svm_dma_desc_para_check(&dma_addr, 1);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return svm_dma_desc_wait(dma_addr);
#else
    SVM_UNUSED(dma_addr);

    return DRV_ERROR_NOT_SUPPORT;
#endif
}

static drvError_t svm_2d_sync_copy(struct drvMem2D *sync_2d)
{
    struct svm_copy_va_2d_info src_info, dst_info;
    enum svm_cpy_dir dir;
    int ret;

    ret = svm_copy_check_addr_prop_cap((u64)(uintptr_t)sync_2d->src, sync_2d->spitch * (sync_2d->height - 1) + sync_2d->width, SVM_FLAG_CAP_SYNC_COPY_2D);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_copy_check_addr_prop_cap((u64)(uintptr_t)sync_2d->dst, sync_2d->dpitch * (sync_2d->height - 1) + sync_2d->width, SVM_FLAG_CAP_SYNC_COPY_2D);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_set_copy_va_2d_info((u64)(uintptr_t)sync_2d->src, sync_2d->spitch, sync_2d->width, sync_2d->height, &src_info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_set_copy_va_2d_info((u64)(uintptr_t)sync_2d->dst, sync_2d->dpitch, sync_2d->width, sync_2d->height, &dst_info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    dir = copy_dir_get_by_devid(src_info.devid, dst_info.devid);
    if ((dir == SVM_H2H_CPY) || (dir == SVM_D2D_CPY)) {
        svm_info("Not support h2h, d2d copy.\n");
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (dir != sync_2d->direction) {
        svm_err("Act cpy dir is diff from para dir. (act_dir=%u; para_dir=%u; src=0x%llx; dst=0x%llx)\n",
            dir, sync_2d->direction, sync_2d->src, sync_2d->dst);
        return DRV_ERROR_PARA_ERROR;
    }

    return svm_sync_copy_2d(&src_info, &dst_info);
}

static drvError_t svm_2d_convert(struct drvMem2DAsync *convert_2d)
{
    struct drvMem2D *info_2d = &convert_2d->copy2dInfo;
    struct svm_copy_va_2d_info src_info, dst_info;
    enum svm_cpy_dir dir;
    int ret;

    if (convert_2d->dmaAddr == NULL) {
        svm_err("dmaAddr is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (info_2d->fixed_size >= (info_2d->width * info_2d->height)) {
        svm_err("Fixed_size should smaller than width*height. (fixed_size=%llu; width=%llu; height=%llu)\n",
            info_2d->fixed_size, info_2d->width, info_2d->height);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (convert_2d->dmaAddr->offsetAddr.devid >= SVM_MAX_DEV_AGENT_NUM) {
        svm_err("Devid is out of range. (devid=%u; range=0-%u)\n", convert_2d->dmaAddr->offsetAddr.devid,
            (SVM_MAX_DEV_AGENT_NUM - 1));
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = svm_copy_check_addr_prop_cap((u64)(uintptr_t)info_2d->src, info_2d->spitch * (info_2d->height - 1) + info_2d->width, SVM_FLAG_CAP_DMA_DESC_CONVERT_2D);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_copy_check_addr_prop_cap((u64)(uintptr_t)info_2d->dst, info_2d->dpitch * (info_2d->height - 1) + info_2d->width, SVM_FLAG_CAP_DMA_DESC_CONVERT_2D);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_set_copy_va_2d_info((u64)(uintptr_t)info_2d->src, info_2d->spitch, info_2d->width, info_2d->height, &src_info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_set_copy_va_2d_info((u64)(uintptr_t)info_2d->dst, info_2d->dpitch, info_2d->width, info_2d->height, &dst_info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    dir = copy_dir_get_by_devid(src_info.devid, dst_info.devid);
    if ((dir == SVM_H2H_CPY) || (dir == SVM_D2D_CPY)) {
        svm_info("Not support h2h, d2d copy.\n");
        return DRV_ERROR_NOT_SUPPORT;
    }
    if (dir != info_2d->direction) {
        svm_err("Act cpy dir is diff from para dir. (act_dir=%u; para_dir=%u; src=0x%llx; dst=0x%llx)\n",
            dir, info_2d->direction, info_2d->src, info_2d->dst);
        return DRV_ERROR_PARA_ERROR;
    }

    if (((dir == SVM_H2D_CPY) && (convert_2d->dmaAddr->offsetAddr.devid != dst_info.devid)) ||
        ((dir == SVM_D2H_CPY) && (convert_2d->dmaAddr->offsetAddr.devid != src_info.devid))) {
        svm_err("Invalid devid. (dir=%u; devid=%u; src_devid=%u; dst_devid=%u)\n",
            dir, convert_2d->dmaAddr->offsetAddr.devid, src_info.devid, dst_info.devid);
        return DRV_ERROR_PARA_ERROR;
    }

    return svm_dma_desc_convert_2d(&src_info, &dst_info, info_2d->fixed_size, convert_2d->dmaAddr);
}

#define SVM_2D_HEIGHT_MAX         (5ULL * SVM_BYTES_PER_MB)
static int svm_memcpy_2d_para_check(struct MEMCPY2D *p_copy)
{
    struct drvMem2D *handle_2d = NULL;

    if (p_copy == NULL) {
        svm_err("p_copy is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    handle_2d = &p_copy->copy2d;
    if (handle_2d == NULL) {
        svm_err("Input para is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (handle_2d->dst == 0) {
        svm_err("Dst is 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (handle_2d->src == 0) {
        svm_err("Src is 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (handle_2d->width == 0) {
        svm_err("Width is 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (handle_2d->height == 0) {
        svm_err("Height is 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (handle_2d->dpitch < handle_2d->width) {
        svm_err("Dpitch should larger than width\n", handle_2d->dpitch, handle_2d->width);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (handle_2d->spitch < handle_2d->width) {
        svm_err("Spitch should larger than width\n", handle_2d->spitch, handle_2d->width);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (handle_2d->direction > DRV_MEMCPY_DEVICE_TO_DEVICE) {
        svm_info("Direction is invalid. (direction=%u)\n", handle_2d->direction);
        return DRV_ERROR_NOT_SUPPORT;
    }

    if ((handle_2d->direction == DRV_MEMCPY_HOST_TO_HOST) || (handle_2d->direction == DRV_MEMCPY_DEVICE_TO_DEVICE)) {
        svm_info("Not support h2h & d2d copy.\n");
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (handle_2d->height > SVM_2D_HEIGHT_MAX) {
        svm_err("Out of max height. (height=%llu; max=%llu)\n", handle_2d->height, SVM_2D_HEIGHT_MAX);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((handle_2d->height != 1) && (((handle_2d->src + handle_2d->spitch) < handle_2d->src)
            || ((handle_2d->dst + handle_2d->dpitch) < handle_2d->dst))) {
        svm_err("Pitch is invalid. (src=0x%llx; dst=0x%llx; spitch=%llu; dpitch=%llu)\n",
            handle_2d->src, handle_2d->dst, handle_2d->spitch, handle_2d->dpitch);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

drvError_t halMemcpy2DInner(struct MEMCPY2D *p_copy);
drvError_t halMemcpy2DInner(struct MEMCPY2D *p_copy)
{
    int ret;

    ret = svm_memcpy_2d_para_check(p_copy);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    if (p_copy->type == DEVMM_MEMCPY2D_SYNC) {
        ret = svm_2d_sync_copy(&p_copy->copy2d);
    } else if(p_copy->type == DEVMM_MEMCPY2D_ASYNC_CONVERT) {
        ret = svm_2d_convert(&p_copy->copy2dAsync);
    } else if(p_copy->type == DEVMM_MEMCPY2D_ASYNC_DESTROY) {
        ret = drvMemDestroyAddr(p_copy->copy2dAsync.dmaAddr);
    } else {
        svm_err("Input parameter type is invalid. (type=%u)\n", p_copy->type);
        return DRV_ERROR_INVALID_VALUE;
    }

    return ret;
}

drvError_t halMemcpy2D(struct MEMCPY2D *p_copy)
{
    return halMemcpy2DInner(p_copy);
}

static int svm_cpy_batch_para_prop_check(u64 dst[], u64 src[], u64 size[], u64 count)
{
    int ret;
    u64 i;

    for (i = 0; i < count; i++) {
        if (size[i] == 0ULL) {
            svm_err("Size is zero. (index=%u)\n", i);
            return DRV_ERROR_INVALID_VALUE;
        }

        if ((svm_va_is_in_range(src[i], size[i]) == false) && (svm_va_is_in_range(dst[i], size[i]) == false)) {
            svm_debug("Memcpy batch not support h2h. (dst=0x%llx; src=0x%llx; index=%u)\n", dst[i], src[i], i);
            return DRV_ERROR_NOT_SUPPORT;
        }

        ret = svm_copy_check_addr_prop_cap(dst[i], size[i], SVM_FLAG_CAP_SYNC_COPY_BATCH);
        if (ret != DRV_ERROR_NONE) {
            return ret;
        }

        ret = svm_copy_check_addr_prop_cap(src[i], size[i], SVM_FLAG_CAP_SYNC_COPY_BATCH);
        if (ret != DRV_ERROR_NONE) {
            return ret;
        }
    }

    return DRV_ERROR_NONE;
}

#define SVM_CPY_BATCH_MAX_COUNT (4ULL * SVM_BYTES_PER_KB)
static int svm_cpy_batch_para_check(u64 dst[], u64 src[], u64 size[], u64 count)
{
    if ((dst == NULL) || (src == NULL) || (size == NULL) || (count == 0)) {
        svm_err("Memcpy batch para check failed. (dst_is_null=%d; src_is_null=%d; size_is_null=%d; count=%lu)\n",
            (dst == NULL), (src == NULL), (size == NULL), count);
        return DRV_ERROR_PARA_ERROR;
    }

    if (count > SVM_CPY_BATCH_MAX_COUNT) {
        svm_info("Batch cpy addr count over limit. (count=%lu)\n", count);
        return DRV_ERROR_NOT_SUPPORT;
    }

    return svm_cpy_batch_para_prop_check(dst, src, size, count);
}

static int get_copy_va_devid(u64 va, u64 size, u32 *devid)
{
    struct svm_prop prop;
    int ret;

    if (svm_va_is_in_range(va, size)) {
        ret = svm_get_prop(va, &prop);
        if (ret != DRV_ERROR_NONE) {
            svm_err("Invalid addr. (ret=%d; va=0x%llx)\n", ret, va);
            return ret;
        }

        *devid = prop.devid;
    } else {
        *devid = svm_get_host_devid();
    }

    return DRV_ERROR_NONE;
}

static int check_batch_copy_va_devid(u64 va, u64 size, u32 devid)
{
    u32 tmp_devid;
    int ret;

    ret = get_copy_va_devid(va, size, &tmp_devid);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    if (devid != tmp_devid) {
        svm_err("Cpy devid different. (cur_addr=0x%llx; cur_devid=%u; target_devid=%u)\n",
            va, tmp_devid, devid);
        return DRV_ERROR_PARA_ERROR;
    }

    return DRV_ERROR_NONE;
}

static int svm_get_batch_copy_devid(u64 dst[], u64 src[], u64 size[], u64 count, u32 *src_devid, u32 *dst_devid)
{
    int ret;
    u64 i;

    ret = get_copy_va_devid(src[0], size[0], src_devid);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = get_copy_va_devid(dst[0], size[0], dst_devid);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    for (i = 1; i < count; i++) {
        ret = check_batch_copy_va_devid(src[i], size[i], *src_devid);
        if (ret != DRV_ERROR_NONE) {
            return ret;
        }

        ret = check_batch_copy_va_devid(dst[i], size[i], *dst_devid);
        if (ret != DRV_ERROR_NONE) {
            return ret;
        }
    }

    return ret;
}

DVresult halMemcpyBatch(uint64_t dst[], uint64_t src[], size_t size[], size_t count)
{
    u32 src_devid, dst_devid;
    int ret;

    ret = svm_cpy_batch_para_check((u64 *)dst, (u64 *)src, (u64 *)size, (u64)count);
    if (ret != DRV_ERROR_NONE) {
        return (DVresult)ret;
    }

    ret = svm_get_batch_copy_devid((u64 *)dst, (u64 *)src, (u64 *)size, (u64)count, &src_devid, &dst_devid);
    if (ret != DRV_ERROR_NONE) {
        return (DVresult)ret;
    }

    return (DVresult)svm_sync_copy_batch((u64 *)dst, (u64 *)src, (u64 *)size, (u64)count, src_devid, dst_devid);
}
