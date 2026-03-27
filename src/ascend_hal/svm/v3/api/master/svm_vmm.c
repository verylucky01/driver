/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <pthread.h>

#include "ascend_hal.h"
#include "pbl_uda_user.h"

#include "svm_atomic.h"
#include "va_allocator.h"
#include "svm_pub.h"
#include "svm_user_adapt.h"
#include "svm_log.h"
#include "svm_master_init.h"
#include "svm_pagesize.h"
#include "malloc_mng.h"
#include "svm_alloc.h"
#include "svmm.h"
#include "casm.h"
#include "casm_cs.h"
#include "svm_vmm.h"
#include "smm_client.h"
#include "svm_share_type.h"
#include "svm_share_align.h"
#include "svm_pipeline.h"
#include "svm_addr_desc.h"
#include "svm_dbi.h"

#define VMM_ALLOC_GRANULARITY_4K                (4ULL * SVM_BYTES_PER_KB)
#define VMM_ALLOC_GRANULARITY_2M                (2ULL * SVM_BYTES_PER_MB)
#define VMM_ALLOC_GRANULARITY_1G                (1ULL * SVM_BYTES_PER_GB)

#define VMM_ALLOC_MINIMUN_GRANULARITY           VMM_ALLOC_GRANULARITY_2M
#define VMM_ALLOC_RECOMMENDED_GRANULARITY       VMM_ALLOC_MINIMUN_GRANULARITY
#define VMM_ALLOC_GIANT_PAGE_GRANULARITY        SVM_BYTES_PER_GB

struct svm_vmm_node {
    struct svm_share_priv_head head;
    void *svmm_inst;
};

struct svm_vmm_access_node {
    struct svm_share_priv_head head; /* should be at first */
    u64 va;
    u64 size;
    u32 owner_devid;
    drv_mem_handle_t *handle;
    struct svm_global_va src_info;
    drv_mem_access_type access_type[SVM_MAX_DEV_NUM];
    pthread_rwlock_t rwlock;
};

struct svm_vmm_set_access_para {
    u64 va;
    u64 size;
    struct drv_mem_access_desc *desc;
    u32 count;
};

struct svm_vmm_get_access_para {
    struct drv_mem_location *location;
    drv_mem_access_type type;
};

enum SVM_VMM_HANDLE_TYPE {
    SVM_VMM_HANDLE_NORMAL_TYPE = 0U,
    SVM_VMM_HANDLE_EXPORT_TYPE,
    SVM_VMM_HANDLE_IMPORT_TYPE,
    SVM_VMM_HANDLE_MAX_TYPE
};

typedef struct drv_mem_handle {
    enum SVM_VMM_HANDLE_TYPE type;
    u32 devid;
    int clr_cs_flag; /* used for import handle to clr cs info when destroy handle */
    u64 ref; /* create and import is 1, map and retain +1, unmap and release -1, handle will be freed when ref is 0  */
    u64 key; /* valid when export or import */
    u64 va;
    int src_prop_valid;
    struct drv_mem_prop src_prop;
    struct svm_global_va src_info; /* used for import handle, query from casm */
} drv_mem_handle_t;

struct svm_share_handle {
    drv_mem_handle_type handle_type;
    u64 key;
    int cs_valid;
    int owner_pid;
    int src_prop_valid;
    struct drv_mem_prop src_prop;
    struct svm_global_va src_va;
};

#define SVM_VMM_OPS_MAX_NUM 2
struct svm_vmm_ops *vmm_ops[SVM_VMM_OPS_MAX_NUM] = {NULL};

static void vmm_recycle_pa_handle(drv_mem_handle_t *handle);

#ifdef EMU_ST /* for emu st, do not delete */
u64 ut_exp_vmm_get_handle_va(drv_mem_handle_t *handle)
{
    return handle->va;
}
#endif

void svm_vmm_set_ops(struct svm_vmm_ops *ops)
{
    static u32 i = 0;

    if (i >= SVM_VMM_OPS_MAX_NUM) {
        svm_err("Vmm ops is out of range.\n");
        return;
    }

    vmm_ops[i++] = ops;
}

static void vmm_handle_free(drv_mem_handle_t *handle)
{
    free(handle);
}

static drv_mem_handle_t *vmm_handle_alloc(void)
{
    return malloc(sizeof(drv_mem_handle_t));
}

static drv_mem_handle_t *vmm_normal_handle_create(u32 devid, u64 va, const struct drv_mem_prop *src_prop)
{
    drv_mem_handle_t *handle = NULL;

    handle = vmm_handle_alloc();
    if (handle != NULL) {
        handle->type = SVM_VMM_HANDLE_NORMAL_TYPE;
        handle->ref = 1ULL;
        handle->devid = devid;
        handle->clr_cs_flag = 0;
        handle->key = 0;
        handle->va = va;
        handle->src_prop_valid = 1;
        handle->src_prop = *src_prop;
    }

    return handle;
}

static void vmm_normal_handle_destroy(drv_mem_handle_t *handle)
{
    vmm_handle_free(handle);
}

static void vmm_normal_handle_no_ref_destroy(drv_mem_handle_t *handle)
{
    if (svm_atomic64_sub(&handle->ref, 1ULL) == 0ULL) {
        vmm_normal_handle_destroy(handle);
    }
}

static drv_mem_handle_t *vmm_import_handle_create(u32 devid, u64 key, struct drv_mem_prop *src_prop,
    struct svm_global_va *src_info)
{
    drv_mem_handle_t *handle = NULL;

    handle = vmm_handle_alloc();
    if (handle != NULL) {
        handle->type = SVM_VMM_HANDLE_IMPORT_TYPE;
        handle->ref = 1ULL;
        handle->devid = devid;
        handle->clr_cs_flag = 0;
        handle->key = key;
        handle->va = src_info->va;
        handle->src_prop_valid = (src_prop != NULL);
        handle->src_prop = (src_prop != NULL) ? *src_prop : (struct drv_mem_prop){0};
        handle->src_info = *src_info;
    }

    return handle;
}

static void vmm_import_handle_destroy(drv_mem_handle_t *handle)
{
    if (handle->clr_cs_flag != 0) {
        (void)svm_casm_cs_clr_src_info(handle->devid, handle->key);
    }
    vmm_handle_free(handle);
}

static void vmm_import_handle_no_ref_destroy(drv_mem_handle_t *handle)
{
    if (svm_atomic64_sub(&handle->ref, 1ULL) == 0ULL) {
        vmm_import_handle_destroy(handle);
    }
}

static size_t vmm_get_granularity_by_pg_type(enum drv_mem_pg_type pg_type)
{
    return (pg_type == MEM_GIANT_PAGE_TYPE) ? VMM_ALLOC_GIANT_PAGE_GRANULARITY : VMM_ALLOC_RECOMMENDED_GRANULARITY;
}

static int vmm_prop_check(const struct drv_mem_prop *prop, bool is_from_create)
{
    if (prop == NULL) {
        svm_err("Prop is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (prop->side >= MEM_MAX_SIDE) {
        svm_err("Invalid side type. (side=%u)\n", prop->side);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((prop->side == MEM_DEV_SIDE) && (prop->devid >= SVM_MAX_DEV_AGENT_NUM)) {
        svm_err("Invalid side devid. (side=%u; devid=%u)\n", prop->side, prop->devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (prop->side == MEM_HOST_SIDE) {
        if ((is_from_create) && (prop->devid != 0)) {
            svm_run_info("Devid must be 0 in host side.\n");
            return DRV_ERROR_NOT_SUPPORT;
        } else if ((is_from_create == false) && (prop->devid > svm_get_host_devid())) {
            svm_err("Devid must be less than host devid. (devid=%u)\n", prop->devid);
            return DRV_ERROR_INVALID_VALUE;
        }
    }

    if ((prop->side == MEM_HOST_NUMA_SIDE) && (prop->devid >= SVM_MALLOC_NUMA_NO_NODE)) {
        svm_err("Invalid numa id. (numa_id=%u)\n", prop->devid);
        return DRV_ERROR_PARA_ERROR;
    }

    if (prop->pg_type >= MEM_MAX_PAGE_TYPE) {
        svm_err("Invalid page type. (pg_type=%u)\n", prop->pg_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (prop->mem_type >= MEM_MAX_TYPE) {
        svm_err("Invalid mem type. (mem_type=%u)\n", prop->mem_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (prop->reserve != 0) {
        svm_err("prop reserve should be zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (prop->pg_type == MEM_GIANT_PAGE_TYPE) {
        if (prop->mem_type == MEM_TS_DDR_TYPE) {
            svm_run_info("Giant page not support ts_ddr.\n");
            return DRV_ERROR_NOT_SUPPORT;
        }
        if (prop->side != MEM_DEV_SIDE) {
            svm_run_info("Giant page not support host.\n");
            return DRV_ERROR_NOT_SUPPORT;
        }
    }

    return DRV_ERROR_NONE;
}

static int vmm_get_granularity(drv_mem_granularity_options option, enum drv_mem_pg_type pg_type,
    u32 module_id, u32 devid, size_t *gran)
{
    size_t granularity = vmm_get_granularity_by_pg_type(pg_type);
    u32 host_devid = svm_get_host_devid();
    u64 npage_size;
    int ret;

    ret = svm_dbi_query_npage_size(host_devid, &npage_size);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Dbi query failed. (devid=%u; ret=%d)\n", host_devid, ret);
        return ret;
    }
    granularity = ((pg_type == MEM_NORMAL_PAGE_TYPE) && (option == MEM_ALLOC_GRANULARITY_MINIMUM) &&
        (devid == host_devid) && (module_id == RUNTIME_MODULE_ID)) ? npage_size : granularity;
    *gran = granularity;
    return DRV_ERROR_NONE;
}

drvError_t halMemGetAllocationGranularity(const struct drv_mem_prop *prop,
    drv_mem_granularity_options option, size_t *granularity)
{
    drvError_t ret;

    if (option >= MEM_ALLOC_GRANULARITY_INVALID) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (granularity == NULL) {
        svm_err("granularity is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = vmm_prop_check(prop, false);
    if (ret != 0) {
        return ret;
    }

    ret = svm_master_init();
    if (ret != DRV_ERROR_NONE) {
        svm_err("Svm master init failed. (ret=%d)\n", ret);
        return ret;
    }

    return vmm_get_granularity(option, prop->pg_type, prop->module_id, prop->devid, granularity);
}

static void vmm_recycle_single(void *svmm_inst, u32 udevid)
{
    u32 recyle_num = 0;

    while (1) {
        drv_mem_handle_t *handle = NULL;
        struct svm_global_va src_info;
        struct svm_dst_va dst_info;
        u64 va, svm_flag;
        u32 smm_flag = 0;
        u32 devid;
        int ret;

        src_info.udevid = udevid;
        ret = svm_svmm_get_seg_by_src_udevid(svmm_inst, &devid, &va, &svm_flag, &src_info);
        if (ret != 0) {
            break;
        }

        handle = (drv_mem_handle_t *)(uintptr_t)src_info.va;
        vmm_restore_real_src_va(&src_info);
        vmm_recycle_pa_handle(handle);

        svm_dst_va_pack(devid, PROCESS_CP1, va, src_info.size, &dst_info);
        (void)svm_smm_client_unmap(&dst_info, &src_info, smm_flag);
        (void)svm_svmm_del_seg(svmm_inst, devid, va, src_info.size, true);
        recyle_num++;
    }

    if (recyle_num > 0) {
        svm_info("Recycle success. (udevid=%u; recyle_num=%u)\n", udevid, recyle_num);
    }
}

static int vmm_try_recycle_single(void *va_handle, u64 start, struct svm_prop *prop, void *priv)
{
    void *svmm_inst = vmm_get_svmm(va_handle);
    SVM_UNUSED(start);
    SVM_UNUSED(prop);

    if (svmm_inst != NULL) {
        vmm_recycle_single(svmm_inst, (u32)(uintptr_t)priv);
    }

    return 0;
}

void vmm_recycle(u32 devid)
{
    u32 udevid;

    if (uda_get_udevid_by_devid_ex(devid, &udevid) != 0) {
        svm_err("Get udevid failed. (devid=%u)\n", devid);
        return;
    }

    (void)svm_for_each_handle(vmm_try_recycle_single, (void *)(uintptr_t)udevid);
}

static void svm_vmm_svmm_destroy(struct svm_vmm_node *vmm_node)
{
    svm_svmm_destroy_inst(vmm_node->svmm_inst);
    svm_ua_free(vmm_node);
}

static int vmm_svmm_release(void *priv, bool force)
{
    struct svm_vmm_node *vmm_node = (struct svm_vmm_node *)priv;
    void *svmm_inst = NULL;
    u32 recyle_num = 0;

    if (vmm_node == NULL) {
        return 0;
    }

    svmm_inst = vmm_node->svmm_inst;
    if (svmm_inst == NULL) { /* alloc failed, to free */
        svm_ua_free(vmm_node);
        return 0;
    }

    while (1) {
        struct svm_global_va src_info;
        struct svm_dst_va dst_info;
        u64 va, svm_flag;
        u32 devid;
        u32 smm_flag = 0;
        int ret;

        ret = svm_svmm_get_first_seg(svmm_inst, &devid, &va, &svm_flag, &src_info);
        if (ret != 0) {
            break;
        }

        if (force == false) {
            return DRV_ERROR_BUSY;
        }

        svm_dst_va_pack(devid, PROCESS_CP1, va, src_info.size, &dst_info);
        vmm_restore_real_src_va(&src_info);

        (void)svm_smm_client_unmap(&dst_info, &src_info, smm_flag);
        (void)svm_svmm_del_seg(svmm_inst, devid, va, src_info.size, true);
        recyle_num++;
    }

    if (recyle_num > 0) {
        u64 svmma_start, svmma_size, svm_flag;
        svm_svmm_parse_inst_info(svmm_inst, &svmma_start, &svmma_size, &svm_flag);
        svm_info("Force release success. (va=0x%llx; size=0x%llx; recyle_num=%u)\n",
            svmma_start, svmma_size, recyle_num);
    }

    svm_vmm_svmm_destroy(vmm_node);

    return 0;
}

static int vmm_get_prop(void *priv, u64 va, struct svm_prop *prop)
{
    struct svm_vmm_node *vmm_node = (struct svm_vmm_node *)priv;
    void *svmm_inst = NULL;
    struct svm_global_va src_info;
    u64 svm_flag;
    u32 devid;
    int ret;

    if (vmm_node == NULL) {
        return DRV_ERROR_INVALID_VALUE;
    }

    svmm_inst = vmm_node->svmm_inst;
    if (svmm_inst == NULL) {
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = svm_svmm_get_seg_by_va(svmm_inst, &devid, &va, &svm_flag, &src_info);
    if (ret != 0) { /* not map, use vmm va prop */
        prop->devid = SVM_INVALID_DEVID;
        prop->tgid = 0;
        svm_svmm_parse_inst_info(svmm_inst, &prop->start, &prop->size, &prop->flag);
        prop->aligned_size = prop->size;
        ret = 0;
    } else {
        prop->devid = devid;
        prop->tgid = src_info.tgid; /* should use map dst tgid, fixed it later */
        prop->flag = svm_flag;
        prop->start = va;
        prop->size = src_info.size;
        ret = svm_get_aligned_size(prop->devid, prop->flag, prop->size, &prop->aligned_size);
    }

    return ret;
}

static u32 vmm_show(void *priv, char *buf, u32 buf_len)
{
    struct svm_vmm_node *vmm_node = (struct svm_vmm_node *)priv;
    void *svmm_inst = NULL;

    if (vmm_node == NULL) {
        return 0;
    }

    svmm_inst = vmm_node->svmm_inst;
    if (svmm_inst != NULL) {
        svm_info("vmm show:\n");
        return svm_svmm_inst_show_detail(svmm_inst, buf, buf_len);
    }

    return 0;
}

static struct svm_priv_ops vmm_priv_ops = {
    .release = vmm_svmm_release,
    .get_prop = vmm_get_prop,
    .show = vmm_show,
};

void *vmm_get_svmm(void *va_handle)
{
    struct svm_vmm_node *vmm_node = svm_get_priv(va_handle);
    return ((vmm_node != NULL) && (vmm_node->head.type == SVM_SHARE_TYPE_VMM)) ? vmm_node->svmm_inst : NULL;
}

static int _vmm_create_svmm_inst(void *va_handle, u64 start, u64 size, u64 svm_flag)
{
    struct svm_vmm_node *vmm_node = NULL;
    void *svmm_inst = NULL;

    vmm_node = (struct svm_vmm_node *)svm_ua_calloc(1, sizeof(*vmm_node));
    if (vmm_node == NULL) {
        /* must set priv, to free the va */
        (void)svm_set_priv(va_handle, NULL, &vmm_priv_ops);
        svm_err("Malloc failed. (va=0x%llx)\n", start);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    svmm_inst = svm_svmm_create_inst(start, size, SVMM_NON_OVERLAP, svm_flag);
    if (svmm_inst == NULL) {
        /* must set priv, to free the va, free vmm_node in release */
        (void)svm_set_priv(va_handle, vmm_node, &vmm_priv_ops);
        svm_err("Create svmm failed. (start=%llx)\n", start);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    vmm_node->head.type = SVM_SHARE_TYPE_VMM;
    vmm_node->svmm_inst = svmm_inst;

    (void)svm_set_priv(va_handle, vmm_node, &vmm_priv_ops);

    return 0;
}

static int vmm_create_svmm_inst(u64 start, u64 size, u64 svm_flag)
{
    void *va_handle = NULL;
    int ret;

    va_handle = svm_handle_get(start);
    if (va_handle == NULL) { /* maybe va has been free after get prop */
        svm_err("Get va handle failed. (start=%llx)\n", start);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = _vmm_create_svmm_inst(va_handle, start, size, svm_flag);
    svm_handle_put(va_handle);
    return ret;
}

static int vmm_malloc_va(u64 align, void **va, u64 size, u64 flag)
{
    struct svm_malloc_location location;
    u64 start, svm_flag = 0;
    int ret;

    svm_flag |= SVM_FLAG_CAP_VMM_VA_FREE;
    svm_flag |= SVM_FLAG_CAP_VMM_MAP;
    svm_flag |= SVM_FLAG_ATTR_VA_ONLY;
    svm_flag |= SVM_FLAG_MUST_WITH_PRIV;
    svm_flag |= SVM_FLAG_BY_PASS_CACHE;
    svm_flag_set_module_id(&svm_flag, SVM_FLAG_INVALID_MODULE_ID);

    start = (u64)(uintptr_t)(*va);
    if (start != 0) {
        svm_flag |= SVM_FLAG_ATTR_SPACIFIED_VA;
        if (svm_is_in_dcache_va_range(start, size)) {
            svm_flag |= SVM_FLAG_ATTR_VA_WITHOUT_MASTER;
        }
    }

    if ((flag & MEM_RSV_TYPE_DEVICE_SHARE) != 0) {
        svm_flag |= SVM_FLAG_ATTR_VA_WITHOUT_MASTER;
    }

    svm_malloc_location_pack(SVM_INVALID_DEVID, SVM_MALLOC_NUMA_NO_NODE, &location);

    ret = svm_malloc(&start, size, align, svm_flag, &location);
    if (ret != 0) {
        return ret;
    }

    ret = vmm_create_svmm_inst(start, size, svm_flag);
    if (ret != 0) {
        (void)svm_free(start);
        return ret;
    }

    (*va) = (void *)(uintptr_t)start;

    return 0;
}

static int vmm_free_va(void *va)
{
    struct svm_prop prop;
    u64 start = (u64)(uintptr_t)va;
    int ret;

    ret = svm_get_prop(start, &prop);
    if (ret != 0) {
        svm_err("Get prop failed. (start=%llx)\n", start);
        return ret;
    }

    if (!svm_flag_cap_is_support_vmm_va_free(prop.flag)) {
        svm_err("Addr cap is not support vmm va free. (va=0x%llx)\n", start);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = svm_free(start);
    return (ret == DRV_ERROR_CLIENT_BUSY) ? DRV_ERROR_BUSY : ret;
}

static int vmm_address_reserve_para_check(void **ptr, size_t size, size_t alignment, void *addr, u32 pg_type)
{
    u64 specify_va = (u64)(uintptr_t)addr;

    if (ptr == NULL) {
        svm_err("Ptr is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (size == 0) {
        svm_err("size is zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((specify_va != 0) && ((specify_va != svm_align_up(specify_va, VMM_ALLOC_RECOMMENDED_GRANULARITY)) ||
        (svm_is_valid_range(specify_va, size) == false))) {
        svm_err("Specified addr not aligned up with recommended granularity or mix range. (addr=0x%llx; size=0x%lx)\n",
            specify_va, size);
        return DRV_ERROR_INVALID_VALUE;
    }

    /* reserved para, verify by zero for subsequent compatibility */
    if (alignment != 0) {
        svm_err("Alignment should be zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (pg_type >= MEM_MAX_PAGE_TYPE) {
        svm_err("Invalid pg type. (pg_type=%u)\n", pg_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((specify_va != 0) && (svm_is_in_dcache_va_range(specify_va, size) && (pg_type > MEM_HUGE_PAGE_TYPE))) {
        svm_err("Dcache page_type invalid. (addr=0x%llx; page_type=%u)\n", specify_va, pg_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    return 0;
}

static u64 vmm_get_va_align(u32 pg_type)
{
    static u64 pg_type_to_align[MEM_MAX_PAGE_TYPE] = {
        [MEM_NORMAL_PAGE_TYPE] = VMM_ALLOC_GRANULARITY_2M,
        [MEM_HUGE_PAGE_TYPE] = VMM_ALLOC_GRANULARITY_2M,
        [MEM_GIANT_PAGE_TYPE] = VMM_ALLOC_GRANULARITY_1G
    };

    return pg_type_to_align[pg_type];
}

drvError_t halMemAddressReserve(void **ptr, size_t size, size_t alignment, void *addr, uint64_t flag)
{
    void *malloc_va = addr; /* Application for a specified address */
    u64 len = (u64)size;
    u32 pg_type;
    int ret;

    pg_type = (u32)(flag & 0xFF); /* 0xFF, pgtype mask */
    ret = vmm_address_reserve_para_check(ptr, size, alignment, addr, pg_type);
    if (ret != 0) {
        return (drvError_t)ret;
    }

    if (!svm_is_in_dcache_va_range((u64)malloc_va, size)) { /* Dcache range size isn't aligned by GB */
        /* Inheriting from the old version, the user wants to alloc 1GB aligned va,
           but the passed flag is MEM_HUGE_PAGE_TYPE. */
        if (size >= SVM_BYTES_PER_GB) {
            pg_type = MEM_GIANT_PAGE_TYPE;
        }

        if (((flag & MEM_RSV_TYPE_DEVICE_SHARE) != 0) || (size >= SVM_VA_RESERVE_ALIGN)) {
            len = svm_align_up((u64)size, SVM_VA_RESERVE_ALIGN);
        }
    }

    svm_use_pipeline();
    ret = vmm_malloc_va(vmm_get_va_align(pg_type), &malloc_va, len, flag);
    svm_unuse_pipeline();

    if (ret == DRV_ERROR_NONE) {
        svm_debug("Vmm reserve. (va=0x%llx; size=%llu; flag=0x%llx; pg_type=%u)\n",
            (u64)(uintptr_t)malloc_va, size, flag, pg_type);
    }

    *ptr = (ret == 0) ? malloc_va : *ptr;
    return (drvError_t)ret;
}

drvError_t halMemAddressFree(void *ptr)
{
    drvError_t ret;

    if (ptr == NULL) {
        svm_err("Ptr is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    svm_debug("Vmm free. (Va=0x%llx)\n", (u64)(uintptr_t)ptr);
    svm_use_pipeline();
    ret = vmm_free_va(ptr);
    svm_unuse_pipeline();

    if (ret != 0) {
        svm_err("Vmm free va failed. (ret=%d; ptr=0x%llx)\n", ret, (u64)(uintptr_t)ptr);
    }

    return (drvError_t)ret;
}

static bool vmm_is_normal_pg_type(enum drv_mem_pg_type pg_type, u32 module_id, size_t size)
{
    return (pg_type == MEM_NORMAL_PAGE_TYPE) && (size < VMM_ALLOC_GRANULARITY_2M) && (module_id == RUNTIME_MODULE_ID);
}

static u32 vmm_get_real_pg_type(enum drv_mem_pg_type pg_type, u32 module_id, size_t size)
{
    return vmm_is_normal_pg_type(pg_type, module_id, size) ? MEM_NORMAL_PAGE_TYPE :
        ((pg_type == MEM_GIANT_PAGE_TYPE) ? MEM_GIANT_PAGE_TYPE : MEM_HUGE_PAGE_TYPE);
}

static u64 vmm_get_real_pg_size(enum drv_mem_pg_type pg_type, u32 module_id, size_t size)
{
    u64 npage_size;

    (void)svm_dbi_query_npage_size(svm_get_host_devid(), &npage_size);
    return vmm_is_normal_pg_type(pg_type, module_id, size) ? npage_size :
        ((pg_type == MEM_GIANT_PAGE_TYPE) ? VMM_ALLOC_GRANULARITY_1G : VMM_ALLOC_GRANULARITY_2M);
}

static bool vmm_is_need_pa_continuous(u32 devid, u32 side, u32 pg_type)
{
    u64 host_npage_size, dev_npage_size;

    (void)svm_dbi_query_npage_size(svm_get_host_devid(), &host_npage_size);
    (void)svm_dbi_query_npage_size(devid, &dev_npage_size);
    return ((side == MEM_DEV_SIDE) && (pg_type == MEM_NORMAL_PAGE_TYPE) && (host_npage_size != dev_npage_size));
}

static int vmm_malloc_pa(drv_mem_handle_t **handle, u64 size, const struct drv_mem_prop *prop)
{
    u32 module_id = (prop->module_id == UNKNOWN_MODULE_ID) ? ASCENDCL_MODULE_ID : prop->module_id;
    u32 numa_id = (prop->side == MEM_HOST_NUMA_SIDE) ? prop->devid : SVM_MALLOC_NUMA_NO_NODE;
    u32 pg_type = vmm_get_real_pg_type(prop->pg_type, module_id, size);
    u32 devid = prop->devid;
    u64 start = 0;
    u64 svm_flag = 0;
    int ret;

    if ((prop->mem_type == MEM_P2P_HBM_TYPE) || (prop->mem_type == MEM_P2P_DDR_TYPE)) {
        svm_flag |= SVM_FLAG_ATTR_PA_P2P;
    }
    if ((prop->side == MEM_HOST_SIDE) || (prop->side == MEM_HOST_NUMA_SIDE)) {
        devid = svm_get_host_devid();
    }

    svm_flag |= SVM_FLAG_CAP_VMM_PA_FREE;
    svm_flag |= SVM_FLAG_CAP_VMM_EXPORT;
    svm_flag |= SVM_FLAG_BY_PASS_CACHE;
    svm_flag |= (pg_type == MEM_GIANT_PAGE_TYPE) ? SVM_FLAG_ATTR_PA_GPAGE :
        ((pg_type == MEM_NORMAL_PAGE_TYPE) ? 0 : SVM_FLAG_ATTR_PA_HPAGE);
    svm_flag |= (vmm_is_need_pa_continuous(devid, prop->side, pg_type) ? SVM_FLAG_ATTR_PA_CONTIGUOUS : 0);

    ret = svm_module_mem_malloc(devid, numa_id, svm_flag, &start, size, module_id);
    if (ret != 0) {
        svm_no_err_if((ret == DRV_ERROR_OUT_OF_MEMORY), "Module mem malloc not success. (ret=%d; size=%llu; devid=%u)\n", ret, size, devid);
        return ret;
    }

    *handle = vmm_normal_handle_create(devid, start, prop);
    if (*handle == NULL) {
        svm_module_mem_free(devid, svm_flag, start, size, prop->module_id);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    return 0;
}

static int vmm_free_pa(drv_mem_handle_t *handle)
{
    struct svm_prop prop;
    u64 start = handle->va;
    int ret;

    ret = svm_get_prop(start, &prop);
    if (ret != 0) {
        /* The log cannot be modified, because in the failure mode library. */
        svm_err("Vmm release find handle failed. (start=0x%llx)\n", start);
        return ret;
    }

    if (!svm_flag_cap_is_support_vmm_pa_free(prop.flag)) {
        svm_err("Addr cap is not support vmm pa free. (start=0x%llx)\n", start);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (svm_atomic64_sub(&handle->ref, 1ULL) > 0ULL) {
        return 0;
    }

    ret = svm_module_mem_free(prop.devid, prop.flag, start, prop.size, svm_flag_get_module_id(prop.flag));
    if (ret != 0) {
        svm_atomic64_inc(&handle->ref);
        return ret;
    }

    vmm_normal_handle_destroy(handle);

    return 0;
}

static void vmm_recycle_pa_handle(drv_mem_handle_t *handle)
{
    if (handle->type == SVM_VMM_HANDLE_IMPORT_TYPE) {
        vmm_import_handle_no_ref_destroy(handle);
    } else {
        vmm_normal_handle_no_ref_destroy(handle);
    }
}

static size_t vmm_get_create_granularity(enum drv_mem_pg_type pg_type, u32 module_id, size_t size)
{
    size_t granularity;
    u64 npage_size;

    (void)svm_dbi_query_npage_size(svm_get_host_devid(), &npage_size);
    granularity = vmm_get_granularity_by_pg_type(pg_type);
    granularity = vmm_is_normal_pg_type(pg_type, module_id, size) ? npage_size : granularity;
    return granularity;
}

drvError_t halMemCreateInner(drv_mem_handle_t **handle, size_t size, const struct drv_mem_prop *prop, uint64_t flag);
drvError_t halMemCreateInner(drv_mem_handle_t **handle, size_t size, const struct drv_mem_prop *prop, uint64_t flag)
{
    size_t granularity;
    int ret;

    if (handle == NULL) {
        svm_err("Handle is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    /* reserved para, verify by zero for subsequent compatibility */
    if (flag != 0) {
        svm_err("Flag should be zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = svm_master_init();
    if (ret != DRV_ERROR_NONE) {
        svm_err("Svm master init failed. (ret=%d)\n", ret);
        return ret;
    }

    ret = vmm_prop_check(prop, true);
    if (ret != 0) {
        return (drvError_t)ret;
    }

    granularity = vmm_get_create_granularity(prop->pg_type, prop->module_id, size);
    if ((size == 0) || ((size % granularity) != 0)) {
        /* The log cannot be modified, because in the failure mode library. */
        svm_err("Size is invalid. (size=%llu; granularity=%llu; side=%u; pg_type=%u; module_id=%u)\n",
            (u64)size, (u64)granularity, prop->side, prop->pg_type, prop->module_id);
        return DRV_ERROR_INVALID_VALUE;
    }

    svm_use_pipeline();
    ret = vmm_malloc_pa(handle, (u64)size, prop);
    svm_unuse_pipeline();
    if (ret == 0) {
        svm_debug("Vmm create. (size=%llu; side=%d; devid=%u; module_id=%u; pg_type=%u; mem_type=%u; handle_va=0x%llx)"
            "\n", size, prop->side, prop->devid, prop->module_id, prop->pg_type, prop->mem_type, (*handle)->va);
    }

    return (drvError_t)ret;
}

drvError_t halMemCreate(drv_mem_handle_t **handle, size_t size, const struct drv_mem_prop *prop, uint64_t flag)
{
    return halMemCreateInner(handle, size, prop, flag);
}

drvError_t halMemReleaseInner(drv_mem_handle_t *handle);
drvError_t halMemReleaseInner(drv_mem_handle_t *handle)
{
    int ret;

    if (handle == NULL) {
        svm_err("Handle or prop is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    svm_debug("Vmm release. (handle_va=0x%llx; type=%d; devid=%u; key=%llu; pg_type=%u)\n",
        handle->va, handle->type, handle->devid, handle->key, handle->src_prop.pg_type);
    if (handle->type == SVM_VMM_HANDLE_IMPORT_TYPE) {
        vmm_import_handle_no_ref_destroy(handle);
        return DRV_ERROR_NONE;
    } else {
        if (handle->type == SVM_VMM_HANDLE_EXPORT_TYPE) {
            ret = svm_casm_destroy_key(handle->key);
            if (ret != 0) {
                svm_err("Casm destroy key failed. (ret=%d; key=%llu)\n", ret, handle->key);
                return ret;
            }
        }

        svm_use_pipeline();
        ret = vmm_free_pa(handle);
        svm_unuse_pipeline();
        if (ret != 0) {
            svm_err("Vmm free pa failed. (ret=%d)\n", ret);
        }
        return (drvError_t)ret;
    }
}

drvError_t halMemRelease(drv_mem_handle_t *handle)
{
    return halMemReleaseInner(handle);
}

drvError_t halMemGetAllocationPropertiesFromHandle(struct drv_mem_prop *prop, drv_mem_handle_t *handle)
{
    if ((handle == NULL) || (prop == NULL)) {
        svm_err("Input invalid. (handle=%u; prop=%u)\n", (handle == NULL), (prop == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    if (handle->src_prop_valid == 0) {
        svm_run_info("Src prop invalid, not support.\n");
        return DRV_ERROR_NOT_SUPPORT;
    }

    *prop = handle->src_prop;
    return DRV_ERROR_NONE;
}

static int _vmm_retain_alloction_handle(u32 devid, void *va_handle, u64 va, drv_mem_handle_t **handle)
{
    struct svm_global_va src_info;
    void *svmm_inst = NULL;
    u64 start = va;
    u64 svm_flag;
    int ret;

    svmm_inst = vmm_get_svmm(va_handle);
    if (svmm_inst == NULL) {
        svm_err("Get svmm inst failed. (va=%llx)\n", va);
        return DRV_ERROR_INVALID_VALUE;
    }

    src_info.udevid = SVM_INVALID_UDEVID;
    ret = svm_svmm_get_seg(svmm_inst, &devid, &start, &svm_flag, &src_info);
    if (ret != 0) {
        svm_err("Get seg failed. (ret=%d; devid=%u; va=0x%llx)\n", ret, devid, va);
        return ret;
    }

    *handle = (drv_mem_handle_t *)(uintptr_t)src_info.va;
    svm_atomic64_inc(&(*handle)->ref);

    return 0;
}

static int vmm_retain_alloction_handle(u32 devid, u64 va, drv_mem_handle_t **handle)
{
    void *va_handle = NULL;
    int ret;

    va_handle = svm_handle_get(va);
    if (va_handle == NULL) { /* maybe va has been free after get prop */
        svm_err("Get va handle failed. (va=%llx)\n", va);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = _vmm_retain_alloction_handle(devid, va_handle, va, handle);
    svm_handle_put(va_handle);

    return ret;
}

drvError_t halMemRetainAllocationHandle(drv_mem_handle_t **handle, void *ptr)
{
    struct svm_prop prop;
    u64 start = (u64)(uintptr_t)ptr;
    int ret;

    if ((handle == NULL) || (ptr == NULL)) {
        svm_err("Invalid para. (handle=%d; ptr=%d)\n", (handle == NULL), (ptr == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = svm_get_prop(start, &prop);
    if (ret != 0) {
        svm_err("Get prop failed. (start=0x%llx)\n", start);
        return ret;
    }

    if ((!svm_flag_cap_is_support_vmm_unmap(prop.flag)) && (!svm_flag_cap_is_support_vmm_ipc_unmap(prop.flag))) {
        svm_err("Not vmm map addr. (va=0x%llx)\n", start);
        return DRV_ERROR_PARA_ERROR;
    }

    return vmm_retain_alloction_handle(prop.devid, start, handle);
}

static int vmm_ops_post_map(void *svmm_inst, u32 devid, u64 va, u64 svm_flag, struct svm_global_va *src_info)
{
    u32 i;

    for (i = 0; i < SVM_VMM_OPS_MAX_NUM; ++i) {
        if ((vmm_ops[i] != NULL) && (vmm_ops[i]->post_map != NULL)) {
            int ret = vmm_ops[i]->post_map(svmm_inst, devid, va, svm_flag, src_info);
            if (ret != 0) {
                return ret;
            }
        }
    }
    return 0;
}

 static int vmm_ops_pre_unmap(u32 task_bitmap, u32 devid, u64 va, u64 svm_flag, struct svm_global_va *src_info)
{
    u32 i;

    for (i = 0; i < SVM_VMM_OPS_MAX_NUM; ++i) {
        if ((vmm_ops[i] != NULL) && (vmm_ops[i]->pre_unmap != NULL)) {
            int ret = vmm_ops[i]->pre_unmap(task_bitmap, devid, va, svm_flag, src_info);
            if (ret != 0) {
                return ret;
            }
        }
    }
    return 0;
}

static u64 vmm_get_single_app_mmap_svm_flag(u64 va, u64 size, u64 src_svm_flag)
{
    u64 svm_flag = 0;

    svm_flag |= SVM_FLAG_CAP_VMM_MAP;
    svm_flag |= SVM_FLAG_CAP_VMM_UNMAP;

    if (!svm_is_in_dcache_va_range(va, size)) {
#ifdef EMU_ST /* Simulation ST support test. */
        svm_flag |= SVM_FLAG_CAP_PREFETCH;
        svm_flag |= SVM_FLAG_CAP_REGISTER | SVM_FLAG_CAP_UNREGISTER;
#endif
        svm_flag |= SVM_FLAG_CAP_REGISTER_ACCESS;
        svm_flag |= SVM_FLAG_CAP_SYNC_COPY | SVM_FLAG_CAP_SYNC_COPY_BATCH;
        svm_flag |= SVM_FLAG_CAP_DMA_DESC_CONVERT | SVM_FLAG_CAP_DMA_DESC_DESTROY | SVM_FLAG_CAP_DMA_DESC_SUBMIT |
            SVM_FLAG_CAP_DMA_DESC_WAIT;
        svm_flag |= SVM_FLAG_CAP_ASYNC_COPY_SUBMIT | SVM_FLAG_CAP_ASYNC_COPY_WAIT;
        svm_flag |= SVM_FLAG_CAP_SYNC_COPY_2D | SVM_FLAG_CAP_DMA_DESC_CONVERT_2D;
        svm_flag |= SVM_FLAG_CAP_SYNC_COPY_EX;
        svm_flag |= SVM_FLAG_CAP_MEMSET;
        svm_flag |= SVM_FLAG_CAP_GET_ATTR | SVM_FLAG_CAP_GET_ADDR_CHECK_INFO | SVM_FLAG_CAP_GET_MEM_TOKEN_INFO;
        svm_flag |= SVM_FLAG_CAP_GET_D2D_TRANS_WAY;
    } else {
        svm_flag |= SVM_FLAG_CAP_SYNC_COPY;
    }

    svm_flag |= src_svm_flag & SVM_FLAG_ATTR_PA_GPAGE;
    svm_flag |= src_svm_flag & SVM_FLAG_ATTR_PA_HPAGE;
    svm_flag |= src_svm_flag & SVM_FLAG_ATTR_PA_CONTIGUOUS;
    svm_flag |= src_svm_flag & SVM_FLAG_ATTR_PA_P2P;
    svm_flag_set_module_id(&svm_flag, svm_flag_get_module_id(src_svm_flag));

    return svm_flag;
}

static int _vmm_single_app_mmap(void *va_handle, u64 va, u64 size, struct svm_prop *src_prop, drv_mem_handle_t *handle)
{
    struct svm_dst_va dst_info;
    struct svm_global_va src_info;
    void *svmm_inst = NULL;
    u64 svm_flag = 0;
    u32 smm_flag = 0;
    int ret;

    svmm_inst = vmm_get_svmm(va_handle);
    if (svmm_inst == NULL) { /* vmm map addr first prefetch */
        svm_err("Get svmm inst failed. (va=%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    svm_global_va_pack(0, src_prop->tgid, src_prop->start, size, &src_info);
    ret = uda_get_udevid_by_devid_ex(src_prop->devid, &src_info.udevid);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Get udevid failed. (devid=%u; ret=%d)\n", src_prop->devid, ret);
        return ret;
    }

    svm_flag = vmm_get_single_app_mmap_svm_flag(va, size, src_prop->flag);
    src_info.va = (u64)(uintptr_t)handle; /* use va to store handle, get real va from handle */
    ret = svm_svmm_add_seg(svmm_inst, src_prop->devid, va, svm_flag, &src_info);
    if (ret != 0) {
        svm_err("Add seg failed. (devid=%u; va=%llx; size=%llx)\n", src_prop->devid, va, size);
        return ret;
    }

    src_info.va = src_prop->start; /* map use real va */

    svm_dst_va_pack(src_prop->devid, PROCESS_CP1, va, size, &dst_info);
    ret = svm_smm_client_map(&dst_info, &src_info, smm_flag);
    if (ret != 0) {
        svm_err("Smm map failed. (devid=%u; va=%llx; size=%llx)\n", src_prop->devid, va, size);
        (void)svm_svmm_del_seg(svmm_inst, src_prop->devid, va, size, true);
        return ret;
    }

    ret = vmm_ops_post_map(svmm_inst, src_prop->devid, va, svm_flag, &src_info);
    if (ret != 0) {
        svm_err("Post map failed. (devid=%u; va=%llx; size=%llx)\n", src_prop->devid, va, size);
        (void)svm_smm_client_unmap(&dst_info, &src_info, smm_flag);
        (void)svm_svmm_del_seg(svmm_inst, src_prop->devid, va, size, true);
        return ret;
    }

    return 0;
}

static int vmm_get_task_bitmap(void *svmm_inst, u64 va, u32 *task_bitmap)
{
    void *seg_handle = NULL;

    seg_handle = svm_svmm_seg_handle_get(svmm_inst, va);
    if (seg_handle == NULL) {
        svm_err("Get seg handle failed. (va=0x%llx)\n", va);
        return DRV_ERROR_INVALID_VALUE;
    }

    *task_bitmap = svm_svmm_get_seg_task_bitmap(seg_handle);
    svm_svmm_seg_handle_put(seg_handle);
    return DRV_ERROR_NONE;
}

void vmm_restore_real_src_va(struct svm_global_va *src_info)
{
    drv_mem_handle_t *handle = (drv_mem_handle_t *)(uintptr_t)src_info->va;
    src_info->va = handle->va; /* restore real va */
}

static int _vmm_query_src_info(void *svmm_inst, u64 va, u64 size, struct svm_global_va *src_info)
{
    u64 seg_va, svm_flag, offset;
    u32 seg_devid;
    int ret;

    seg_va = va;
    ret = svm_svmm_get_seg_by_va(svmm_inst, &seg_devid, &seg_va, &svm_flag, src_info);
    if (ret != 0) {
        return ret;
    }

    offset = va - seg_va;
    if ((offset >= src_info->size) || (size > (src_info->size - offset))) {
        svm_err("Is vmm mixed seg. (va=0x%llx; size=0x%llx; seg_va=0x%llx; seg_size=0x%llx)\n",
            va, size, seg_va, src_info->size);
        return DRV_ERROR_INVALID_VALUE;
    }

    vmm_restore_real_src_va(src_info);
    src_info->va += offset;
    src_info->size = size;
    return DRV_ERROR_NONE;
}

int vmm_query_src_info(u64 va, u64 size, struct svm_global_va *src_info)
{
    void *va_handle = NULL;
    void *svmm_inst = NULL;
    int ret;

    va_handle = svm_handle_get(va);
    if (va_handle == NULL) {
        return DRV_ERROR_NOT_EXIST;
    }

    svmm_inst = vmm_get_svmm(va_handle);
    if (svmm_inst == NULL) {
        svm_handle_put(va_handle);
        return DRV_ERROR_NOT_EXIST;
    }

    ret = _vmm_query_src_info(svmm_inst, va, size, src_info);
    svm_handle_put(va_handle);

    return ret;
}

static int _vmm_single_app_unmap(u32 devid, void *va_handle, u64 va, u64 size)
{
    drv_mem_handle_t *handle = NULL;
    struct svm_dst_va dst_info;
    struct svm_global_va src_info;
    void *svmm_inst = NULL;
    u64 start = va;
    u64 svm_flag;
    u32 task_bitmap, smm_flag = 0;
    int ret;

    svmm_inst = vmm_get_svmm(va_handle);
    if (svmm_inst == NULL) { /* vmm map addr first prefetch */
        svm_err("Get svmm inst failed. (va=%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    src_info.udevid = SVM_INVALID_UDEVID;
    ret = svm_svmm_get_seg(svmm_inst, &devid, &start, &svm_flag, &src_info);
    if (ret != 0) {
        svm_err("Get seg failed. (devid=%u; va=%llx; size=%llx)\n", devid, va, size);
        return ret;
    }

    handle = (drv_mem_handle_t *)(uintptr_t)src_info.va;
    vmm_restore_real_src_va(&src_info);

    ret = vmm_get_task_bitmap(svmm_inst, va, &task_bitmap);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = vmm_ops_pre_unmap(task_bitmap, devid, start, svm_flag, &src_info);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = svm_svmm_del_seg(svmm_inst, devid, va, size, false);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Del seg failed. (ret=%d; devid=%u; va=0x%llx; size=0x%llx)\n", ret, devid, va, size);
        return ret;
    }

    (void)vmm_free_pa(handle);

    svm_dst_va_pack(devid, PROCESS_CP1, start, size, &dst_info);
    ret = svm_smm_client_unmap(&dst_info, &src_info, smm_flag);
    if (ret != 0) {
        /* not rollback */
        svm_err("Smm unmap failed. (devid=%u; va=%llx; size=%llx)\n", devid, start, size);
        return ret;
    }

    return 0;
}

static int vmm_single_app_mmap(u64 va, u64 size, drv_mem_handle_t *handle, u64 offset)
{
    u64 handle_start = handle->va;
    struct svm_prop src_prop;
    void *va_handle = NULL;
    int ret;

    ret = svm_get_prop(handle_start, &src_prop);
    if (ret != 0) {
        svm_err("Get src prop failed. (handle_start=%llx)\n", handle_start);
        return ret;
    }

    if ((!svm_is_va_page_align(src_prop.devid, src_prop.flag, va)) ||
        (!svm_is_page_align(src_prop.devid, src_prop.flag, offset)) ||
        (!svm_is_page_align(src_prop.devid, src_prop.flag, size))) {
        svm_err("Not align. (devid=%u; flag=0x%llx; va=0x%llx; size=0x%llx; offset=0x%llx)\n",
            src_prop.devid, src_prop.flag, va, size, offset);
        return DRV_ERROR_PARA_ERROR;
    }

    if (offset >= src_prop.size) {
        svm_err("Map offset size is overflow. (offset=0x%llx; pa_size=0x%llx)\n", offset, src_prop.size);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((size + offset) > src_prop.size) {
        svm_err("Map size is overflow. (size=0x%llx; pa_size=0x%llx; offset=0x%llx)\n", size, src_prop.size, offset);
        return DRV_ERROR_INVALID_VALUE;
    }

    va_handle = svm_handle_get(va);
    if (va_handle == NULL) { /* maybe va has been free after get prop */
        svm_err("Get va handle failed. (va=%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = _vmm_single_app_mmap(va_handle, va, size, &src_prop, handle);
    svm_handle_put(va_handle);

    return ret;
}

static int vmm_single_app_unmap(u32 devid, u64 va, u64 size)
{
    void *va_handle = NULL;
    int ret;

    va_handle = svm_handle_get(va);
    if (va_handle == NULL) { /* maybe va has been free after get prop */
        svm_err("Get va handle failed. (va=%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = _vmm_single_app_unmap(devid, va_handle, va, size);
    svm_handle_put(va_handle);

    return ret;
}

static u64 vmm_get_cross_app_mmap_svm_flag(u64 va, u64 size, u32 module_id)
{
    u64 svm_flag = 0;

    svm_flag |= SVM_FLAG_CAP_VMM_IPC_UNMAP;
    if (!svm_is_in_dcache_va_range(va, size)) {
        svm_flag |= SVM_FLAG_CAP_MEMSET;
        svm_flag |= SVM_FLAG_CAP_SYNC_COPY;
        svm_flag |= SVM_FLAG_CAP_DMA_DESC_CONVERT;
        svm_flag |= SVM_FLAG_CAP_GET_MEM_TOKEN_INFO;
        svm_flag |= SVM_FLAG_CAP_GET_ATTR;
        svm_flag |= SVM_FLAG_CAP_GET_D2D_TRANS_WAY;
    }

    svm_flag_set_module_id(&svm_flag, module_id);

    return svm_flag;
}

static int _vmm_cross_app_mmap(void *va_handle, u64 va, u64 size, drv_mem_handle_t *handle, u64 offset)
{
    struct svm_global_va src_info = handle->src_info;
    u64 key = handle->key;
    u64 dst_devid = handle->devid;
    void *svmm_inst = NULL;
    u64 svm_flag = 0;
    u32 casm_flag = 0;
    int ret;

    svmm_inst = vmm_get_svmm(va_handle);
    if (svmm_inst == NULL) { /* vmm map addr first prefetch */
        svm_err("Get svmm inst failed. (va=%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    if (offset >= src_info.size) {
        svm_err("Map offset size is overflow. (offset=0x%llx; pa_size=0x%llx)\n", offset, src_info.size);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((size + offset) > src_info.size) {
        svm_err("Map size is overflow. (size=0x%llx; pa_size=0x%llx; offset=0x%llx)\n", size, src_info.size, offset);
        return DRV_ERROR_INVALID_VALUE;
    }

    svm_flag = vmm_get_cross_app_mmap_svm_flag(va, size, handle->src_prop.module_id);
    src_info.va = (u64)(uintptr_t)handle; /* cross app mmap src info is not used, use va to store handle */
    src_info.size = size;
    ret = svm_svmm_add_seg(svmm_inst, (u32)dst_devid, va, svm_flag, &src_info);
    if (ret != 0) {
        svm_err("Add seg failed. (devid=%u; va=%llx; size=%llx)\n", dst_devid, va, size);
        return ret;
    }

    ret = svm_casm_mem_map((u32)dst_devid, va, size, key, casm_flag);
    if (ret != 0) {
        svm_err("Casm map failed. (devid=%u; va=%llx; size=%llx)\n", dst_devid, va, size);
        (void)svm_svmm_del_seg(svmm_inst, (u32)dst_devid, va, size, true);
        return ret;
    }

    return 0;
}

static int _vmm_cross_app_unmap(u32 devid, void *va_handle, u64 va, u64 size)
{
    drv_mem_handle_t *handle = NULL;
    struct svm_global_va src_info;
    void *svmm_inst = NULL;
    u64 start = va;
    u64 svm_flag;
    int ret;

    svmm_inst = vmm_get_svmm(va_handle);
    if (svmm_inst == NULL) {
        svm_err("Get svmm inst failed. (va=%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    src_info.udevid = SVM_INVALID_UDEVID;
    ret = svm_svmm_get_seg(svmm_inst, &devid, &start, &svm_flag, &src_info);
    if (ret != 0) {
        svm_err("Get seg failed. (ret=%d; devid=%u; va=0x%llx; size=0x%llx)\n", ret, devid, va, size);
        return ret;
    }

    handle = (drv_mem_handle_t *)(uintptr_t)src_info.va;

    ret = svm_svmm_del_seg(svmm_inst, devid, va, size, false);
    if (ret != 0) {
        svm_err("Del seg failed. (ret=%d; devid=%u; va=0x%llx; size=0x%llx)\n", ret, devid, va, size);
        return ret;
    }

    vmm_import_handle_no_ref_destroy(handle);

    ret = svm_casm_mem_unmap(devid, va, size);
    if (ret != 0) {
        svm_err("Casm unmap failed. (devid=%u; va=%llx; size=%llx)\n", devid, va, size);
        return ret;
    }

    return 0;
}

static int vmm_cross_app_mmap(u64 va, u64 size, drv_mem_handle_t *handle, u64 offset)
{
    void *va_handle = NULL;
    int ret;

    va_handle = svm_handle_get(va);
    if (va_handle == NULL) { /* maybe va has been free after get prop */
        svm_err("Get va handle failed. (va=%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = _vmm_cross_app_mmap(va_handle, va, size, handle, offset);
    svm_handle_put(va_handle);

    return ret;
}

static int vmm_cross_app_unmap(u32 devid, u64 va, u64 size)
{
    void *va_handle = NULL;
    int ret;

    va_handle = svm_handle_get(va);
    if (va_handle == NULL) { /* maybe va has been free after get prop */
        svm_err("Get va handle failed. (va=%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = _vmm_cross_app_unmap(devid, va_handle, va, size);
    svm_handle_put(va_handle);

    return ret;
}

static int vmm_mmap(void *va, u64 size, drv_mem_handle_t *handle, u64 offset)
{
    struct svm_prop prop;
    u64 start = (u64)(uintptr_t)va;
    int ret;

    ret = svm_get_prop(start, &prop);
    if (ret != 0) {
        svm_err("Get prop failed. (start=%llx)\n", start);
        return ret;
    }

    if (!svm_flag_cap_is_support_vmm_map(prop.flag)) {
        svm_err("Addr cap is not support vmm map. (start=0x%llx)\n", start);
        return DRV_ERROR_PARA_ERROR;
    }

    if (handle->type == SVM_VMM_HANDLE_IMPORT_TYPE) {
        ret = vmm_cross_app_mmap(start, size, handle, offset);
    } else {
        ret = vmm_single_app_mmap(start, size, handle, offset);
    }

    if (ret == 0) {
        svm_atomic64_inc(&handle->ref);
    }

    return ret;
}

static int vmm_munmap(void *va)
{
    struct svm_prop prop;
    u64 start = (u64)(uintptr_t)va;
    int ret;

    ret = svm_get_prop(start, &prop);
    if (ret != 0) {
        /* The log cannot be modified, because in the failure mode library. */
        svm_err("Vmm unmap find addr failed. (start=0x%llx)\n", start);
        return ret;
    }

    if (svm_flag_cap_is_support_vmm_unmap(prop.flag)) {
        return vmm_single_app_unmap(prop.devid, start, prop.aligned_size);
    } else if (svm_flag_cap_is_support_vmm_ipc_unmap(prop.flag)) {
        return vmm_cross_app_unmap(prop.devid, start, prop.aligned_size);
    } else {
        /* The log cannot be modified, because in the failure mode library. */
        svm_err("Addr can not unmap. (va=0x%llx)\n", start);
        return DRV_ERROR_PARA_ERROR;
    }
}

static int vmm_mmap_para_check(void *ptr, size_t size, size_t offset, drv_mem_handle_t *handle, uint64_t flag)
{
    int ret;
    u64 aligned, page_size;

    if ((ptr == NULL) || (handle == NULL) || (size == 0)) {
        svm_err("Invalid para. (ptr=%u; handle=%u; size=%u)\n", (ptr == NULL), (handle == NULL), size);
        return DRV_ERROR_INVALID_VALUE;
    }

    /* reserved para, verify by zero for subsequent compatibility */
    if (offset != 0) {
        svm_err("Offset should be zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    /* reserved para, verify by zero for subsequent compatibility */
    if (flag != 0) {
        svm_err("Flag should be zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = svm_share_get_dst_align((u64)(uintptr_t)ptr, size, handle->devid, &aligned);
    if (ret != 0) {
        svm_err("Not aligned. (ptr=0x%llx; size=%llu; devid=%u)\n", (u64)(uintptr_t)ptr, (u64)size, handle->devid);
        return DRV_ERROR_PARA_ERROR;
    }

    if (handle->src_prop_valid == 1) {
        page_size = vmm_get_real_pg_size(handle->src_prop.pg_type, handle->src_prop.module_id, size);
        if (page_size > aligned) {
            svm_err("Not aligned. (ptr=0x%llx; pg_type=%u; size=%llu; module_id=%u; page_size=%llu; aligned=%llu)\n",
                (u64)(uintptr_t)ptr, handle->src_prop.pg_type, (u64)size, handle->src_prop.module_id,
                page_size, aligned);
            return DRV_ERROR_PARA_ERROR;
        }
    } else {
        if (aligned < VMM_ALLOC_GRANULARITY_4K) {
            svm_err("Not aligned. (ptr=0x%llx; pg_type=%u; size=%llu; module_id=%u; aligned=%llu)\n",
                (u64)(uintptr_t)ptr, handle->src_prop.pg_type, (u64)size, handle->src_prop.module_id, aligned);
            return DRV_ERROR_PARA_ERROR;
        }
    }
    return DRV_ERROR_NONE;
}

drvError_t halMemMap(void *ptr, size_t size, size_t offset, drv_mem_handle_t *handle, uint64_t flag)
{
    int ret;

    ret = vmm_mmap_para_check(ptr, size, offset, handle, flag);
    if (ret != 0) {
        return ret;
    }

    svm_use_pipeline();
    ret = vmm_mmap(ptr, (u64)size, handle, (u64)offset);
    svm_unuse_pipeline();
    if (ret == 0) {
        svm_debug("Vmm Map. (va=0x%llx; size=%llu; handle_va=0x%llx; devid=%u; key=%llu)\n",
            (u64)(uintptr_t)ptr, size, handle->va, handle->devid, handle->key);
    }

    return (drvError_t)ret;
}

drvError_t halMemUnmap(void *ptr)
{
    int ret;

    if (ptr == NULL) {
        svm_err("Ptr is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (((uintptr_t)ptr % VMM_ALLOC_RECOMMENDED_GRANULARITY) != 0) {
        svm_err("Ptr is invalid. (ptr=%p)\n", ptr);
        return DRV_ERROR_INVALID_VALUE;
    }
    svm_debug("Vmm Unmap. (va=0x%llx)\n", (u64)(uintptr_t)ptr);

    svm_use_pipeline();
    ret = vmm_munmap(ptr);
    svm_unuse_pipeline();

    return (drvError_t)ret;
}

static void svm_fill_share_handle(struct svm_share_handle *share_handle,
    drv_mem_handle_type handle_type, u64 key, struct drv_mem_prop *src_prop)
{
    struct svm_global_va src_va = {0};
    struct svm_global_va tmp_src_va;
    int owner_pid = 0;
    int tmp_owner_pid;
    int cs_valid = 0;

    share_handle->handle_type = handle_type;
    share_handle->key = key;

    if (handle_type == MEM_HANDLE_TYPE_FABRIC) {
        int ret = svm_casm_cs_query_src_info(key, &tmp_src_va, &tmp_owner_pid);
        if (ret == 0) {
            if (tmp_src_va.server_id != SVM_INVALID_SERVER_ID) {
                cs_valid = 1;
                src_va = tmp_src_va;
                owner_pid = tmp_owner_pid;
            }
        }
    }

    share_handle->cs_valid = cs_valid;
    share_handle->owner_pid = owner_pid;
    share_handle->src_va = src_va;
    share_handle->src_prop_valid = 1;
    share_handle->src_prop = *src_prop;
}

static int vmm_export(drv_mem_handle_t *handle, drv_mem_handle_type handle_type, struct svm_share_handle *share_handle)
{
    struct svm_prop prop;
    struct svm_dst_va dst_va;
    u64 start = handle->va;
    u64 key;
    int ret;

    ret = svm_get_prop(start, &prop);
    if (ret != 0) {
        svm_err("Get prop failed. (start=%llx)\n", start);
        return ret;
    }

    if (!svm_flag_cap_is_support_vmm_export(prop.flag)) {
        svm_err("Addr cap is not support vmm export. (start=0x%llx)\n", start);
        return DRV_ERROR_PARA_ERROR;
    }

    if (prop.devid == svm_get_host_devid()) {
        svm_dst_va_pack(prop.devid, DEVDRV_PROCESS_CPTYPE_MAX, prop.start, prop.aligned_size, &dst_va);
    } else {
        svm_dst_va_pack(prop.devid, DEVDRV_PROCESS_CP1, prop.start, prop.aligned_size, &dst_va);
    }

    svm_use_pipeline();
    ret = svm_casm_create_key(&dst_va, &key);
    if (ret == 0) {
        handle->type = SVM_VMM_HANDLE_EXPORT_TYPE;
        handle->key = key;
        svm_fill_share_handle(share_handle, handle_type, key, &handle->src_prop);
    }
    svm_unuse_pipeline();

    return ret;
}

/* dir: dst device process va -> src device pa */
static int vmm_dir_check(u32 dst_devid, struct svm_global_va *src_va)
{
#define CONNECT_TYPE_NUM (HOST_DEVICE_CONNECT_TYPE_UB + 1)
    u32 host_devid = svm_get_host_devid();

    if (dst_devid == host_devid) {
        /* not support H2rH, H2rD */
        if ((src_va->server_id != SVM_INVALID_SERVER_ID) && (src_va->server_id != svm_get_cur_server_id())) {
            return DRV_ERROR_NOT_SUPPORT;
        }

        if (src_va->udevid == host_devid) {
            /* support local H2H */
            return 0;
        } else { /* local H2D */
            /* not support connect with ub, because host mem-decoder not config */
            int h2d_support_flag[CONNECT_TYPE_NUM] = {
                [HOST_DEVICE_CONNECT_TYPE_PCIE] = 1, /* use pcie bar */
                [HOST_DEVICE_CONNECT_TYPE_HCCS] = 1, /* use global pa */
            };
            u32 src_devid, connect_type;
            int ret = drvDeviceGetIndexByPhyId(src_va->udevid, &src_devid);
            if (ret != 0) {
                svm_err("Get src devid failed. (udevid=%u)\n", src_va->udevid);
                return ret;
            }

            connect_type = svm_get_device_connect_type(src_devid);
            if (connect_type > HOST_DEVICE_CONNECT_TYPE_UB) {
                svm_err("Invalid connect type. (devid=%u; connect_type=%u)\n", dst_devid, connect_type);
                return DRV_ERROR_INVALID_VALUE;
            }

            return (h2d_support_flag[connect_type] == 1) ? 0 : DRV_ERROR_NOT_SUPPORT;
        }
    } else {
        if (src_va->udevid == host_devid) { /* D2H, D2rH */
            /* D2H is same with D2rH */
            /* not support connect with pcie, because pcie th va is not same */
            int d2h_support_flag[CONNECT_TYPE_NUM] = {
                [HOST_DEVICE_CONNECT_TYPE_HCCS] = 1, /* use global pa */
                [HOST_DEVICE_CONNECT_TYPE_UB] = 1, /* use memory decoder */
            };
            u32 connect_type = svm_get_device_connect_type(dst_devid);
            if (connect_type > HOST_DEVICE_CONNECT_TYPE_UB) {
                svm_err("Invalid connect type. (devid=%u; connect_type=%u)\n", dst_devid, connect_type);
                return DRV_ERROR_INVALID_VALUE;
            }

            return (d2h_support_flag[connect_type] == 1) ? 0 : DRV_ERROR_NOT_SUPPORT;
        } else {
            /* support D2D, D2rD */
            return 0;
        }
    }
}

static int vmm_import(struct svm_share_handle *share_handle, u32 devid, drv_mem_handle_t **handle)
{
    struct drv_mem_prop *src_prop = share_handle->src_prop_valid ? &share_handle->src_prop : NULL;
    struct svm_global_va src_va;
    u64 key = share_handle->key;
    int clr_cs_flag = 0;
    int dir_checked_flag = 0;
    int ret;

    if ((share_handle->handle_type == MEM_HANDLE_TYPE_FABRIC) && (share_handle->cs_valid != 0)) {
        ret = vmm_dir_check(devid, &share_handle->src_va);
        if (ret != 0) {
            return ret;
        }
        dir_checked_flag = 1;

        if (share_handle->src_va.server_id != svm_get_cur_server_id()) {
            ret = svm_casm_cs_set_src_info(devid, key, &share_handle->src_va, share_handle->owner_pid);
            if (ret != 0) {
                svm_err("Set cs src failed. (key=0x%llx)\n", key);
                return ret;
            }
            clr_cs_flag = 1;
        }
    }

    ret = svm_casm_get_src_va(devid, key, &src_va);
    if (ret != 0) {
        if (clr_cs_flag == 1) {
            (void)svm_casm_cs_clr_src_info(devid, key);
        }
        svm_err("Get src va failed. (key=0x%llx)\n", key);
        return ret;
    }

    if (dir_checked_flag == 0) {
        ret = vmm_dir_check(devid, &src_va);
        if (ret != 0) {
            return ret;
        }
    }

    *handle = vmm_import_handle_create(devid, key, src_prop, &src_va);
    if (*handle == NULL) {
        if (clr_cs_flag == 1) {
            (void)svm_casm_cs_clr_src_info(devid, key);
        }
        svm_err("Import handle create failed.\n");
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    (*handle)->clr_cs_flag = share_handle->cs_valid;

    return 0;
}

drvError_t halMemExportToShareableHandleV2(drv_mem_handle_t *handle, drv_mem_handle_type handle_type,
    uint64_t flags, struct MemShareHandle *share_handle)
{
    struct svm_share_handle *_share_handle = (struct svm_share_handle *)(void *)share_handle;
    int ret;

    if (share_handle == NULL) {
        svm_err("shareable_handle is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (handle_type >= MEM_HANDLE_TYPE_MAX) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (flags != 0) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (handle == NULL) {
        svm_err("Handle is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (handle->type != SVM_VMM_HANDLE_NORMAL_TYPE) {
        /* The log cannot be modified, because in the failure mode library. */
        svm_err("Handle type should be normal type. (type=%u)\n", handle->type);
        return DRV_ERROR_PARA_ERROR;
    }

    svm_use_pipeline();
    ret = vmm_export(handle, handle_type, _share_handle);
    svm_unuse_pipeline();

    svm_debug("Vmm export. (handle_type=%u; devid=%u; key=%llu; handle_va=0x%llx; cs_valid=%d; "
        "owner_pid=%d; key=%llu)\n", handle_type, handle->devid, handle->key, handle->va,
        _share_handle->cs_valid, _share_handle->owner_pid, _share_handle->key);

    return (drvError_t)ret;
}

drvError_t halMemExportToShareableHandle(drv_mem_handle_t *handle, drv_mem_handle_type handle_type,
    uint64_t flags, uint64_t *shareable_handle)
{
    struct svm_share_handle _share_handle;
    int ret;

    (void)flags;
    if (shareable_handle == NULL) {
        svm_err("shareable_handle is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (handle_type != MEM_HANDLE_TYPE_NONE) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = halMemExportToShareableHandleV2(handle, handle_type, 0, (struct MemShareHandle *)(void *)&_share_handle);
    if (ret == 0) {
        *shareable_handle = _share_handle.key;
    }

    return (drvError_t)ret;
}

static drvError_t vmm_share_mem_set_pid_para_check(uint64_t shareable_handle, int pid[], uint32_t pid_num)
{
    SVM_UNUSED(shareable_handle);

#define SVM_SHARE_MEM_MAX_PID_CNT 65535 /* > 64 process * 384 server node * 2 die */
    if (pid == NULL) {
        svm_err("Pid is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (pid_num == 0) {
        svm_err("Pid num is zero.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (pid_num > SVM_SHARE_MEM_MAX_PID_CNT) {
        svm_err("Pid num is invalid. (pid_num=%u)\n", pid_num);
        return DRV_ERROR_INVALID_VALUE;
    }
    return 0;
}

drvError_t halMemImportFromShareableHandleV2(drv_mem_handle_type handle_type,
    struct MemShareHandle *share_handle, uint32_t devid, drv_mem_handle_t **handle)
{
    struct svm_share_handle *_share_handle = (struct svm_share_handle *)(void *)share_handle;
    int ret;

    if ((share_handle == NULL) || (handle == NULL)) {
        svm_err("Handle is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (devid >= SVM_MAX_DEV_NUM) {
        svm_err("Invalid devid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    if ((handle_type >= MEM_HANDLE_TYPE_MAX) || (handle_type != _share_handle->handle_type)) {
        svm_err("Invalid para. (handle_type=%u)\n", handle_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    svm_use_pipeline();
    ret = vmm_import(_share_handle, devid, handle);
    svm_unuse_pipeline();
    svm_debug("Vmm import. (ret=%d; handle_type=%u; devid=%u; cs_valid=%d; owner_pid=%d; key=%llu; "
        "handle_va=0x%llx)\n", ret, handle_type, devid, _share_handle->cs_valid, _share_handle->owner_pid,
        _share_handle->key, (ret == 0) ? (*handle)->va : 0);

    return (drvError_t)ret;
}

drvError_t halMemImportFromShareableHandle(uint64_t shareable_handle, uint32_t devid, drv_mem_handle_t **handle)
{
    struct svm_share_handle _share_handle = {0};

    _share_handle.handle_type = MEM_HANDLE_TYPE_NONE;
    _share_handle.key = shareable_handle;
    _share_handle.cs_valid = 0;
    _share_handle.src_prop_valid = 0;

    return halMemImportFromShareableHandleV2(MEM_HANDLE_TYPE_NONE, (struct MemShareHandle *)(void *)&_share_handle,
        devid, handle);
}

static inline int vmm_set_mwl_attr(u64 shareable_handle, struct ShareHandleAttr *attr)
{
    u64 key = shareable_handle;
    int tgid = SVM_ANY_TASK_ID;

    if (attr->enableFlag == 0) {
        svm_run_info("Not support disable mwl.\n");
        return DRV_ERROR_NOT_SUPPORT;
    } else if (attr->enableFlag == 1) {
        return svm_casm_add_local_task(key, &tgid, 1);
    } else {
        svm_err("Invalid share handle mwl attr val. (val=%u)\n", attr->enableFlag);
        return DRV_ERROR_PARA_ERROR;
    }
}

static inline int vmm_get_mwl_attr(u64 shareable_handle, struct ShareHandleAttr *attr)
{
    u64 key = shareable_handle;
    int tgid = SVM_ANY_TASK_ID;
    int ret;

    ret = svm_casm_check_local_task(key, &tgid, 1);
    if ((ret != DRV_ERROR_NONE) && (ret != DRV_ERROR_NO_RESOURCES)) {
        svm_err("Check any task failed. (ret=%d; key=0x%llx)\n", ret, key);
        return ret;
    }

    attr->enableFlag = (ret == DRV_ERROR_NONE) ? 1 : 0;
    return DRV_ERROR_NONE;
}

drvError_t halMemShareHandleSetAttribute(uint64_t shareable_handle, enum ShareHandleAttrType type,
    struct ShareHandleAttr attr)
{
    static int (*vmm_set_attr_func[SHR_HANDLE_ATTR_TYPE_MAX])(u64 shareable_handle, struct ShareHandleAttr *attr) = {
        [SHR_HANDLE_ATTR_NO_WLIST_IN_SERVER] = vmm_set_mwl_attr
    };
    int ret;

    if ((type >= SHR_HANDLE_ATTR_TYPE_MAX) || (vmm_set_attr_func[type] == NULL)) {
        svm_debug("Not support type. (type=%d)\n", type);
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = vmm_set_attr_func[type](shareable_handle, &attr);
    if (ret == DRV_ERROR_NONE) {
        svm_debug("Share handle set attr successfully. (shareable_handle=0x%llx; type=%u)\n", shareable_handle, type);
    }

    return ret;
}

drvError_t halMemShareHandleGetAttribute(uint64_t shareable_handle, enum ShareHandleAttrType type,
    struct ShareHandleAttr *attr)
{
    static int (*vmm_get_attr_func[SHR_HANDLE_ATTR_TYPE_MAX])(u64 shareable_handle, struct ShareHandleAttr *attr) = {
        [SHR_HANDLE_ATTR_NO_WLIST_IN_SERVER] = vmm_get_mwl_attr
    };
    int ret;

    if (attr == NULL) {
        svm_err("Attr is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((type >= SHR_HANDLE_ATTR_TYPE_MAX) || (vmm_get_attr_func[type] == NULL)) {
        svm_debug("Not support type. (type=%d)\n", type);
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = vmm_get_attr_func[type](shareable_handle, attr);
    if (ret == DRV_ERROR_NONE) {
        svm_debug("Share handle get attr successfully. (shareable_handle=0x%llx; type=%u)\n", shareable_handle, type);
    }

    return ret;
}

drvError_t halMemShareHandleInfoGet(uint64_t shareable_handle, struct ShareHandleGetInfo *info)
{
    struct svm_global_va src_va;
    u64 key = shareable_handle;
    int ret;

    if (info == NULL) {
        svm_err("Info is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = svm_casm_get_src_va(svm_get_host_devid(), key, &src_va);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Casm get src va failed. (ret=%d; key=0x%llx; attr=%llu)\n", ret, key);
        return ret;
    }

    info->phyDevid = src_va.udevid;

    svm_debug("Share handle get info successfully. (shareable_handle=0x%llx; phy_devid=%u)\n",
        shareable_handle, info->phyDevid);
    return DRV_ERROR_NONE;
}

static u32 vmm_access_location_to_devid(struct drv_mem_location *location)
{
    if (location->side == MEM_DEV_SIDE) {
        return location->id;
    } else {
        return svm_get_host_devid();
    }
}

static int vmm_access_location_check(struct drv_mem_location *location)
{
    if (location->side == MEM_HOST_SIDE) {
        if (location->id != 0) {
            svm_err("Invalid host id. (id=%u)\n", location->id);
            return DRV_ERROR_INVALID_VALUE;
        }
    } else if (location->side == MEM_DEV_SIDE) {
        if (location->id >= SVM_MAX_DEV_AGENT_NUM) {
            svm_err("Invalid dev id. (id=%u)\n", location->id);
            return DRV_ERROR_INVALID_VALUE;
        }
    } else {
        svm_err("Invalid side. (side=%u)\n", location->side);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

static int vmm_access_desc_check(struct drv_mem_access_desc *desc, size_t count)
{
    u32 access_dev_flag[SVM_MAX_DEV_NUM] = {0};
    size_t i;

    if ((desc == NULL) || (count <= 0) || (count > SVM_MAX_DEV_NUM)) {
        svm_err("Invalid desc. (count=%u)\n", (uint32_t)count);
        return DRV_ERROR_INVALID_VALUE;
    }

    for (i = 0; i < count; i++) {
        u32 devid;
        int ret;

        if ((desc[i].type == MEM_ACCESS_TYPE_NONE) || (desc[i].type == MEM_ACCESS_TYPE_READ)) {
            svm_run_info("Not support type. (index=%u; type=%u)\n", i, desc[i].type);
            return DRV_ERROR_NOT_SUPPORT;
        }

        if ((desc[i].type != MEM_ACCESS_TYPE_NONE) && (desc[i].type != MEM_ACCESS_TYPE_READ)
            && (desc[i].type != MEM_ACCESS_TYPE_READWRITE)) {
            svm_err("Invalid type. (index=%u; type=%u)\n", i, desc[i].type);
            return DRV_ERROR_INVALID_VALUE;
        }

        ret = vmm_access_location_check(&desc[i].location);
        if (ret != DRV_ERROR_NONE) {
            svm_err("Invalid location. (index=%u)\n", i);
            return ret;
        }

        devid = vmm_access_location_to_devid(&desc[i].location);
        if (access_dev_flag[devid] == 1) {
            svm_err("Same location. (side=%u; id=%u)\n", desc[i].location.side, desc[i].location.id);
            return DRV_ERROR_INVALID_VALUE;
        }

        access_dev_flag[devid] = 1;
        svm_debug("SetAccess. (i=%llu; count=%llu; type=%u; id=%u; side=%u)\n", 
            i, count, desc[i].type, desc[i].location.id, desc[i].location.side);
    }

    return DRV_ERROR_NONE;
}

static struct svm_vmm_access_node *vmm_get_access_node(void *seg_handle)
{
    struct svm_vmm_access_node *access_node = svm_svmm_get_seg_priv(seg_handle);
    return ((access_node != NULL) && (access_node->head.type == SVM_SHARE_TYPE_VMM_ACCESS)) ?
        access_node : NULL;
}

static struct svm_vmm_access_node *vmm_access_node_create(u32 owner_devid, u64 va, u64 size,
    struct svm_global_va *src_info)
{
    struct svm_vmm_access_node *access_node = NULL;
    u32 i;

    access_node = (struct svm_vmm_access_node *)svm_ua_calloc(1, sizeof(*access_node));
    if (access_node == NULL) {
        svm_err("Malloc failed. (owner_devid=%u; va=0x%llx; size=0x%llx)\n", owner_devid, va, size);
        return NULL;
    }

    access_node->head.type = SVM_SHARE_TYPE_VMM_ACCESS;

    access_node->owner_devid = owner_devid;
    access_node->va = va;
    access_node->size = size;
    access_node->handle = (drv_mem_handle_t *)(uintptr_t)src_info->va;
    access_node->src_info = *src_info;
    vmm_restore_real_src_va(&access_node->src_info);

    for (i = 0; i < SVM_MAX_DEV_NUM; i++) {
        if (i == access_node->owner_devid) {
            access_node->access_type[i] = MEM_ACCESS_TYPE_READWRITE;
        } else {
            access_node->access_type[i] = MEM_ACCESS_TYPE_NONE;
        }
    }

    (void)pthread_rwlock_init(&access_node->rwlock, NULL);

    return access_node;
}

static void vmm_access_node_destroy(struct svm_vmm_access_node *access_node)
{
    svm_ua_free(access_node);
}

static int vmm_access_single_do_mmap(struct svm_vmm_access_node *access_node, u32 devid, drv_mem_access_type type)
{
    drv_mem_handle_t *handle = access_node->handle;
    int ret;

    if (handle->type == SVM_VMM_HANDLE_IMPORT_TYPE) {
        u32 casm_flag = (type == MEM_ACCESS_TYPE_READ) ? SVM_CASM_FLAG_PG_RDONLY : 0;
        ret = svm_casm_mem_map(devid, access_node->va, access_node->size, handle->key, casm_flag);
    } else {
        u32 smm_flag = (type == MEM_ACCESS_TYPE_READ) ? SVM_SMM_FLAG_PG_RDONLY : 0;
        struct svm_dst_va dst_info;

        svm_dst_va_pack(devid, PROCESS_CP1, access_node->va, access_node->size, &dst_info);
        ret = svm_smm_client_map(&dst_info, &access_node->src_info, smm_flag);
    }
    if (ret != 0) { /* not need rollback */
        svm_err("Mmap failed. (devid=%u; va=%llx; size=%llx; handle type=%u)\n",
            devid, access_node->va, access_node->size, handle->type);
        return ret;
    }

    access_node->access_type[devid] = type;

    return 0;
}

static void vmm_access_single_do_unmap(struct svm_vmm_access_node *access_node, u32 devid)
{
    drv_mem_handle_t *handle = access_node->handle;
    int ret;

    if (handle->type == SVM_VMM_HANDLE_IMPORT_TYPE) {
        ret = svm_casm_mem_unmap(devid, access_node->va, access_node->size);
    } else {
        u32 smm_flag = 0;
        struct svm_dst_va dst_info;

        svm_dst_va_pack(devid, PROCESS_CP1, access_node->va, access_node->size, &dst_info);
        ret = svm_smm_client_unmap(&dst_info, &access_node->src_info, smm_flag);
    }
    if (ret != 0) { /* not need rollback */
        svm_warn("Munmap failed. (devid=%u; va=%llx; size=%llx)\n", devid, access_node->va, access_node->size);
    }

    access_node->access_type[devid] = MEM_ACCESS_TYPE_NONE;
}

static int vmm_access_node_release(void *priv, bool force)
{
    struct svm_vmm_access_node *access_node = (struct svm_vmm_access_node *)priv;
    u32 i;
    SVM_UNUSED(force);

    for (i = 0; i < SVM_MAX_DEV_NUM; i++) {
        if ((i == access_node->owner_devid) || (access_node->access_type[i] == MEM_ACCESS_TYPE_NONE)) {
            continue;
        }

        vmm_access_single_do_unmap(access_node, i);
    }

    vmm_access_node_destroy(access_node);

    return 0;
}

static u32 vmm_access_node_show(void *priv, char *buf, u32 buf_len)
{
    struct svm_vmm_access_node *access_node = (struct svm_vmm_access_node *)priv;
    u32 data_len = 0;
    u32 i;

    if (priv == NULL) {
        return 0;
    }

    svm_info("vmm access node show:\n");

    if (buf == NULL) {
        svm_info("owner_devid 0x%llx va 0x%llx size 0x%llx\n",
            access_node->owner_devid, access_node->va, access_node->size);
        for (i = 0; i < SVM_MAX_DEV_NUM; i++) {
            if (access_node->access_type[i] != MEM_ACCESS_TYPE_NONE) {
                svm_info("    devid %u access type %u\n", i, access_node->access_type[i]);
            }
        }
    } else {
        int len = snprintf_s(buf, buf_len, buf_len - 1,
            "owner_devid 0x%llx va 0x%llx size 0x%llx\n",
            access_node->owner_devid, access_node->va, access_node->size);
        if (len < 0) {
            return 0;
        }
        data_len = (u32)len;

        for (i = 0; i < SVM_MAX_DEV_NUM; i++) {
            if (access_node->access_type[i] != MEM_ACCESS_TYPE_NONE) {
                len = snprintf_s(buf + data_len, buf_len - data_len, buf_len - data_len - 1,
                    "    devid %u access type %u\n", i, access_node->access_type[i]);
                if (len < 0) {
                    break;
                }

                data_len += (u32)len;
            }
        }
    }

    return data_len;
}

static struct svm_svmm_seg_priv_ops vmm_access_node_priv_ops = {
    .release = vmm_access_node_release,
    .show = vmm_access_node_show,
};

static struct svm_vmm_access_node *vmm_create_access_node(void *seg_handle, u32 owner_devid, u64 va, u64 size,
    struct svm_global_va *src_info)
{
    struct svm_vmm_access_node *access_node = NULL;

    access_node = vmm_get_access_node(seg_handle);
    /* if access_node is null, but svm_svmm_get_seg_priv(seg_handle) is not null, the priv is occpy by prefetch */
    if ((access_node == NULL) && (svm_svmm_get_seg_priv(seg_handle) == NULL)) {
        access_node = vmm_access_node_create(owner_devid, va, size, src_info);
        if (access_node != NULL) {
            int ret = svm_svmm_set_seg_priv(seg_handle, (void *)access_node, &vmm_access_node_priv_ops);
            if (ret != 0) {
                svm_err("Set seg priv failed. (start=0x%llx)\n", va);
                vmm_access_node_destroy(access_node);
                access_node = NULL;
            }
        }
    }

    return access_node;
}

static int _vmm_access_node_handle(void *va_handle, u64 va,
    int (*func)(struct svm_vmm_access_node *access_node, void *priv), void *priv)
{
    struct svm_vmm_access_node *access_node = NULL;
    void *svmm_inst = NULL;
    void *seg_handle = NULL;
    u32 seg_devid;
    u64 svm_flag, seg_va;
    struct svm_global_va src_info;
    int ret;

    svmm_inst = vmm_get_svmm(va_handle);
    if (svmm_inst == NULL) {
        svm_err("Vmm alloc not finish. (va=0x%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    seg_handle = svm_svmm_seg_handle_get(svmm_inst, va);
    if (seg_handle == NULL) { /* maybe va has been unmap after get prop or not map */
        svm_err("Get seg handle failed. (va=%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    seg_va = va;
    ret = svm_svmm_get_seg_by_va(svmm_inst, &seg_devid, &seg_va, &svm_flag, &src_info);
    if (ret != 0) {
        svm_svmm_seg_handle_put(seg_handle);
        /* should not in, because we has been get seg handle */
        svm_err("Get seg info failed. (va=%llx)\n", va);
        return ret;
    }

    access_node = vmm_get_access_node(seg_handle);
    if (access_node == NULL) { /* vmm map addr first set/get access */
        svm_svmm_inst_occupy_pipeline(svmm_inst);
        access_node = vmm_create_access_node(seg_handle, seg_devid, seg_va, src_info.size, &src_info);
        svm_svmm_inst_release_pipeline(svmm_inst);
        if (access_node == NULL) {
            svm_svmm_seg_handle_put(seg_handle);
            return DRV_ERROR_OUT_OF_MEMORY;
        }
    }

    ret = func(access_node, priv);
    svm_svmm_seg_handle_put(seg_handle);

    return ret;
}

static int vmm_access_node_handle(void *ptr,
    int (*func)(struct svm_vmm_access_node *access_node, void *priv), void *priv)
{
    u64 va = (u64)(uintptr_t)ptr;
    void *va_handle = NULL;
    struct svm_prop prop;
    int ret;

    ret = svm_get_prop(va, &prop);
    if (ret != 0) {
        svm_err("Get prop failed. (va=0x%llx)\n", va);
        return ret;
    }

    if ((!svm_flag_cap_is_support_vmm_unmap(prop.flag)) && (!svm_flag_cap_is_support_vmm_ipc_unmap(prop.flag))) {
        svm_err("Addr cap is not support set access, it is not vmm map address. (va=0x%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    va_handle = svm_handle_get(va);
    if (va_handle == NULL) { /* maybe va has been free after get prop */
        svm_err("Get va handle failed. (va=0x%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = _vmm_access_node_handle(va_handle, va, func, priv);
    svm_handle_put(va_handle);

    return ret;
}

static int vmm_set_access_check(struct svm_vmm_access_node *access_node,
    u64 va, u64 size, struct drv_mem_access_desc *desc, u32 count)
{
    u32 i;

    if ((va != access_node->va) || (size != access_node->size)) {
        svm_err("va and size not same with mmap. (va=0x%llx; size=0x%llx; mmap start=0x%llx; size=0x%llx)\n",
            va, size, access_node->va, access_node->size);
        return DRV_ERROR_PARA_ERROR;
    }

    for (i = 0; i < count; i++) {
        u32 dst_devid = vmm_access_location_to_devid(&desc[i].location);
        int ret;

        if (dst_devid == access_node->owner_devid) {
            /* modify read write prop, support latter */
            continue;
        }

        ret = vmm_dir_check(dst_devid, &access_node->src_info);
        if (ret != DRV_ERROR_NONE) {
            return ret;
        }

        if (access_node->access_type[dst_devid] != MEM_ACCESS_TYPE_NONE) {
            if (access_node->access_type[dst_devid] != desc[i].type) {
                svm_err("Repeat set with diff type. (ptr=0x%llx; size=0x%llx; dst_devid=%u; desc.type=%u; type=%u\n",
                    va, size, dst_devid, desc[i].type, access_node->access_type[dst_devid]);
                return DRV_ERROR_BUSY;
            }
        }
    }

    return 0;
}

static int vmm_set_access_do_mmap(struct svm_vmm_access_node *access_node,
    u64 va, u64 size, struct drv_mem_access_desc *desc, u32 count)
{
    u32 i;

    for (i = 0; i < count; i++) {
        u32 dst_devid = vmm_access_location_to_devid(&desc[i].location);
        int ret;

        if (access_node->access_type[dst_devid] != MEM_ACCESS_TYPE_NONE) {
            continue;
        }

        ret = vmm_access_single_do_mmap(access_node, dst_devid, desc[i].type);
        if (ret != 0) { /* not need rollback */
            svm_err("Smm map failed. (devid=%u; va=%llx; size=%llx)\n", dst_devid, va, size);
            return ret;
        }
    }

    return 0;
}

/* cancel access can only be called by vmm unmap, then we will unmap all access device
   call trace: halMemUnmap->svm_svmm_del_seg->vmm_access_node_release->svm_smm_client_unmap */
static int vmm_set_access(struct svm_vmm_access_node *access_node, void *priv)
{
    struct svm_vmm_set_access_para *para = priv;
    struct drv_mem_access_desc *desc = para->desc;
    u64 va = para->va;
    u64 size = para->size;
    u32 count = para->count;
    int ret;

    pthread_rwlock_wrlock(&access_node->rwlock);

    ret = vmm_set_access_check(access_node, va, size, desc, count);
    if (ret == 0) {
        ret = vmm_set_access_do_mmap(access_node, va, size, desc, count);
    }

    pthread_rwlock_unlock(&access_node->rwlock);

    return ret;
}

static int vmm_get_access(struct svm_vmm_access_node *access_node, void *priv)
{
    struct svm_vmm_get_access_para *para = priv;
    struct drv_mem_location *location = para->location;
    u32 dst_devid = vmm_access_location_to_devid(location);

    (void)pthread_rwlock_rdlock(&access_node->rwlock);

    para->type = access_node->access_type[dst_devid];

    pthread_rwlock_unlock(&access_node->rwlock);

    return 0;
}

drvError_t halMemSetAccess(void *ptr, size_t size, struct drv_mem_access_desc *desc, size_t count)
{
    struct svm_vmm_set_access_para para;
    drvError_t ret;

    if (ptr == NULL) {
        svm_err("Null ptr.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = vmm_access_desc_check(desc, count);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    para.va = (u64)(uintptr_t)ptr;
    para.size  = (u64)size;
    para.desc = desc;
    para.count  = (u32)count;
    svm_debug("SetAccess. (ptr=0x%llx; size=%llu; count=%llu)\n", (u64)(uintptr_t)ptr, size, count);

    return vmm_access_node_handle(ptr, vmm_set_access, &para);
}

drvError_t halMemGetAccess(void *ptr, struct drv_mem_location *location, uint64_t *flags)
{
    struct svm_vmm_get_access_para para;
    drvError_t ret;

    if ((ptr == NULL) || (flags == NULL) || (location == NULL)) {
        svm_err("Null ptr.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = vmm_access_location_check(location);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    para.location = location;
    ret = vmm_access_node_handle(ptr, vmm_get_access, &para);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Get access failed. (side=%u; id=%u)\n", location->side, location->id);
        return ret;
    }

    *flags = (uint64_t)para.type;
    svm_debug("GetAccess. (ptr=0x%llx; side=%u; id=%u; flags=0x%llx)\n",
        (u64)(uintptr_t)ptr, location->side, location->id, *flags);
    return 0;
}

/* trans struct MemShareHandle to u64 shareable_handle(devid + share id), to set/get attribute in local */
drvError_t halMemTransShareableHandle(drv_mem_handle_type handle_type, struct MemShareHandle *share_handle,
    uint32_t *server_id, uint64_t *shareable_handle)
{
    struct svm_share_handle *_share_handle = (struct svm_share_handle *)(void *)share_handle;

    if ((share_handle == NULL) || (server_id == NULL) || (shareable_handle == NULL)) {
        svm_err("handle is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((_share_handle->handle_type != handle_type) || (handle_type >= MEM_HANDLE_TYPE_MAX)) {
        svm_err("Invalid para. (handle_type=%u; share_handle.handle_type=%u)\n",
            handle_type, _share_handle->handle_type);
        return DRV_ERROR_INVALID_VALUE;
    }

    *shareable_handle = _share_handle->key;
    *server_id = _share_handle->src_va.server_id;
    return DRV_ERROR_NONE;
}

drvError_t halMemSetPidToShareableHandle(uint64_t shareable_handle, int pid[], uint32_t pid_num)
{
    int ret;

    ret = vmm_share_mem_set_pid_para_check(shareable_handle, pid, pid_num);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }
    svm_debug("Setpid. (shareable_handle=%llu; pid_num=%u)\n", shareable_handle, pid_num);
    return svm_casm_add_local_task(shareable_handle, pid, pid_num);
}

drvError_t halMemGetAddressReserveRange(void **ptr, size_t *size, drv_mem_addr_reserve_type type, uint64_t flag)
{
    u64 get_va, get_size;

    if ((ptr == NULL) || (size == NULL)) {
        svm_err("Ptr or size is null. (ptr_is_null=%d; size_is_null=%d)\n", (ptr == NULL), (size == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    /* reserved para, verify by zero for subsequent compatibility */
    if (flag != 0) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    switch (type) {
        case MEM_ADDR_RESERVE_TYPE_DCACHE:
            svm_get_dcache_va_range(&get_va, &get_size);
            break;
        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    *ptr = (void *)(uintptr_t)get_va;
    *size = (size_t)get_size;
    return DRV_ERROR_NONE;
}

