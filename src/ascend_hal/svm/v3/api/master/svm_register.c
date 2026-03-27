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

#include "drv_user_common.h" /* user list */
#include "pbl_uda_user.h"

#include "svm_user_adapt.h"
#include "svm_log.h"
#include "svm_pagesize.h"
#include "svm_dbi.h"
#include "va_allocator.h"
#include "malloc_mng.h"
#include "svm_vmm.h"
#include "svmm.h"
#include "smm_client.h"
#include "svm_share_type.h"
#include "svm_share_align.h"
#include "svm_pipeline.h"
#include "svm_addr_desc.h"

struct svm_register_node {
    struct svm_share_priv_head head;
    struct list_head node;
    void *svmm_inst[SVM_MAX_DEV_NUM];
    u64 va;
};

static struct list_head register_head = LIST_HEAD_INIT(register_head);
static pthread_mutex_t register_mutex = PTHREAD_MUTEX_INITIALIZER;

static void svm_register_src_recycle(u32 devid, u32 udevid, void *svmm_inst)
{
    u32 recyle_num = 0;

    /* recycle dst devid match */
    while (1) {
        struct svm_global_va src_info;
        struct svm_dst_va dst_info;
        u64 va, svm_flag;
        u32 smm_flag = 0;
        int ret;

        ret = svm_svmm_get_first_seg(svmm_inst, &devid, &va, &svm_flag, &src_info);
        if (ret != 0) {
            break;
        }

        svm_dst_va_pack(devid, PROCESS_CP1, va, src_info.size, &dst_info);
        (void)svm_smm_client_unmap(&dst_info, &src_info, smm_flag);
        (void)svm_svmm_del_seg(svmm_inst, devid, va, src_info.size, true);
        recyle_num++;
    }

    if (recyle_num > 0) {
        svm_info("Recycle success. (devid=%u; udevid=%u; recyle_num=%u)\n", devid, udevid, recyle_num);
    }
}

void svm_register_recycle(u32 devid)
{
    struct list_head *node = NULL, *tmp = NULL;
    u32 udevid;

    if (uda_get_udevid_by_devid_ex(devid, &udevid) != 0) {
        svm_err("Get udevid failed. (devid=%u)\n", devid);
        return;
    }

    (void)pthread_mutex_lock(&register_mutex);
    list_for_each_safe(node, tmp, &register_head) {
        struct svm_register_node *register_node = list_entry(node, struct svm_register_node, node);
        void *va_handle = svm_handle_get(register_node->va);
        if (va_handle != NULL) { /* get va handle to access its priv svmm_inst in node */
            svm_register_src_recycle(devid, udevid, register_node->svmm_inst[devid]);
            svm_handle_put(va_handle);
        }
    }
    (void)pthread_mutex_unlock(&register_mutex);
}

static void svm_register_svmm_destroy_insts(struct svm_register_node *register_node)
{
    u32 devid;

    for (devid = 0; devid < SVM_MAX_DEV_NUM; devid++) {
        svm_svmm_destroy_inst(register_node->svmm_inst[devid]);
    }
}

static int svm_register_svmm_create_insts(struct svm_register_node *register_node,
    u64 start, u64 size, u64 svm_flag)
{
    void *svmm_inst = NULL;
    u32 devid, tmp_devid;

    for (devid = 0; devid < SVM_MAX_DEV_NUM; devid++) {
        svmm_inst = svm_svmm_create_inst(start, size, SVMM_NON_OVERLAP, svm_flag);
        if (svmm_inst == NULL) {
            svm_err("Create inst priv failed. (va=0x%llx)\n", start);
            goto destroy_insts;
        }

        register_node->svmm_inst[devid] = svmm_inst;
    }

    return DRV_ERROR_NONE;
destroy_insts:
    for (tmp_devid = 0; tmp_devid < devid; tmp_devid++) {
        svm_svmm_destroy_inst(register_node->svmm_inst[devid]);
        register_node->svmm_inst[devid] = NULL;
    }
    return DRV_ERROR_OUT_OF_MEMORY;
}

static void svm_register_svmm_destroy_locked(struct svm_register_node *register_node)
{
    svm_register_svmm_destroy_insts(register_node);
    drv_user_list_del(&register_node->node);
    svm_ua_free(register_node);
}

static void svm_register_svmm_destroy(struct svm_register_node *register_node)
{
    (void)pthread_mutex_lock(&register_mutex);
    svm_register_svmm_destroy_locked(register_node);
    (void)pthread_mutex_unlock(&register_mutex);
}

static int svm_register_svmm_inst_release(void *svmm_inst, u32 devid, bool force)
{
    u32 recyle_num = 0;

    while (1) {
        struct svm_global_va src_info;
        struct svm_dst_va dst_info;
        u64 va, svm_flag;
        u32 smm_flag = 0;
        u32 tmp_devid;
        int ret;

        ret = svm_svmm_get_first_seg(svmm_inst, &tmp_devid, &va, &svm_flag, &src_info);
        if (ret != 0) {
            break;
        }

        if (force == false) {
            return DRV_ERROR_BUSY;
        }

        svm_dst_va_pack(devid, PROCESS_CP1, va, src_info.size, &dst_info);
        (void)svm_smm_client_unmap(&dst_info, &src_info, smm_flag);
        (void)svm_svmm_del_seg(svmm_inst, devid, va, src_info.size, true);
        recyle_num++;
    }

    if (recyle_num > 0) {
        u64 svmma_start, svmma_size, svm_flag;
        svm_svmm_parse_inst_info(svmm_inst, &svmma_start, &svmma_size, &svm_flag);
        svm_info("Release success. (va=0x%llx; size=0x%llx; devid=%u; recyle_num=%u)\n",
            svmma_start, svmma_size, recyle_num);
    }

    return DRV_ERROR_NONE;
}

static int svm_register_svmm_release(void *priv, bool force)
{
    struct svm_register_node *register_node = (struct svm_register_node *)priv;
    u32 devid;
    int ret;

    for (devid = 0; devid < SVM_MAX_DEV_NUM; devid++) {
        ret = svm_register_svmm_inst_release(register_node->svmm_inst[devid], devid, force);
        if (ret != DRV_ERROR_NONE) {
            return ret;
        }
    }
    svm_register_svmm_destroy(register_node);
    return 0;
}

static u32 _register_show(struct svm_register_node *register_node, char *buf, u32 buf_len)
{
    u32 devid;
    u32 len = 0;

    for (devid = 0; devid < SVM_MAX_DEV_NUM; devid++) {
        svm_info("register show devid[%u]:\n", devid);
        len += svm_svmm_inst_show_detail(register_node->svmm_inst[devid], buf + len, buf_len - len);
    }

    return len;
}

static u32 register_show(void *priv, char *buf, u32 buf_len)
{
    if (priv != NULL) {
        struct svm_register_node *register_node = (struct svm_register_node *)priv;
        return _register_show(register_node, buf, buf_len);
    }

    return 0;
}

static struct svm_priv_ops register_priv_ops = {
    .release = svm_register_svmm_release,
    .get_prop = NULL,
    .show = register_show,
};

static struct svm_svmm_seg_priv_ops register_seg_priv_ops = {
    .release = svm_register_svmm_release,
    .show = register_show,
};

static struct svm_register_node *svm_register_svmm_create(u64 start, u64 size, u64 svm_flag)
{
    struct svm_register_node *register_node = NULL;
    int ret;

    register_node = (struct svm_register_node *)svm_ua_calloc(1, sizeof(*register_node));
    if (register_node == NULL) {
        svm_err("Malloc failed. (va=0x%llx)\n", start);
        return NULL;
    }

    ret = svm_register_svmm_create_insts(register_node, start, size, svm_flag);
    if (ret != DRV_ERROR_NONE) {
        svm_ua_free(register_node);
        return NULL;
    }

    register_node->head.type = SVM_SHARE_TYPE_REGISTER;
    register_node->va = start;
    drv_user_list_add_tail(&register_node->node, &register_head);

    return register_node;
}

static void *svm_normal_get_register_svmm(void *va_handle, u32 devid)
{
    struct svm_register_node *register_node = svm_get_priv(va_handle);
    return ((register_node != NULL) && (register_node->head.type == SVM_SHARE_TYPE_REGISTER)) ?
        register_node->svmm_inst[devid] : NULL;
}

static void *svm_normal_create_register_svmm(void *va_handle, u32 devid, u64 start, u64 size, u64 svm_flag)
{
    void *svmm_inst = NULL;

    (void)pthread_mutex_lock(&register_mutex);
    svmm_inst = svm_normal_get_register_svmm(va_handle, devid);
    if (svmm_inst == NULL) {
        struct svm_register_node *register_node = svm_register_svmm_create(start, size, svm_flag);
        if (register_node != NULL) {
            int ret = svm_set_priv(va_handle, (void *)register_node, &register_priv_ops);
            if (ret == 0) {
                svmm_inst = register_node->svmm_inst[devid];
            } else { /* may be set by prefetch */
                svm_err("Set va priv failed. (start=0x%llx)\n", start);
                svm_register_svmm_destroy_locked(register_node);
            }
        }
    }
    (void)pthread_mutex_unlock(&register_mutex);

    return svmm_inst;
}

static void *svm_vmm_get_register_svmm(void *seg_handle, u32 devid)
{
    struct svm_register_node *register_node = svm_svmm_get_seg_priv(seg_handle);
    return ((register_node != NULL) && (register_node->head.type == SVM_SHARE_TYPE_REGISTER)) ?
        register_node->svmm_inst[devid] : NULL;
}

static void *svm_vmm_create_register_svmm(void *seg_handle, u32 devid, u64 start, u64 size, u64 svm_flag)
{
    void *svmm_inst = NULL;

    (void)pthread_mutex_lock(&register_mutex);
    svmm_inst = svm_vmm_get_register_svmm(seg_handle, devid);
    if (svmm_inst == NULL) {
        struct svm_register_node *register_node = svm_register_svmm_create(start, size, svm_flag);
        if (register_node != NULL) {
            int ret = svm_svmm_set_seg_priv(seg_handle, (void *)register_node, &register_seg_priv_ops);
            if (ret == 0) {
                svmm_inst = register_node->svmm_inst[devid];
            } else { /* may be set by prefetch */
                svm_err("Set seg priv failed. (start=0x%llx)\n", start);
                svm_register_svmm_destroy_locked(register_node);
            }
        }
    }
    (void)pthread_mutex_unlock(&register_mutex);

    return svmm_inst;
}

static int svm_register(void *svmm_inst, u64 va, u64 size, u32 devid, struct svm_global_va *src_info)
{
    struct svm_dst_va dst_info;
    u64 svm_flag = 0;
    u32 smm_flag = 0;
    int ret;

    ret = uda_get_udevid_by_devid_ex(src_info->udevid, &src_info->udevid);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Get udevid failed. (devid=%u; ret=%d)\n", devid, ret);
        return ret;
    }

    ret = svm_svmm_add_seg(svmm_inst, devid, va, svm_flag, src_info);
    if (ret != 0) {
        svm_err("Add seg failed. (devid=%u; va=0x%llx)\n", devid, va);
        return ret;
    }

    svm_dst_va_pack(devid, PROCESS_CP1, va, size, &dst_info);

    ret = svm_smm_client_map(&dst_info, src_info, smm_flag);
    if (ret != 0) {
        svm_err("Smm map failed. (devid=%u; va=0x%llx)\n", devid, va);
        (void)svm_svmm_del_seg(svmm_inst, devid, va, size, true);
        return ret;
    }

    return ret;
}

static int svm_unregister(void *svmm_inst, u64 va, u32 devid)
{
    struct svm_dst_va dst_info;
    struct svm_global_va src_info;
    u64 start = va;
    u64 svm_flag = 0;
    u32 smm_flag = 0;
    int ret;

    ret = svm_svmm_get_seg_by_va(svmm_inst, &devid, &start, &svm_flag, &src_info);
    if (ret != 0) {
        svm_err("Addr is not register. (va=0x%llx; devid=%u)\n", start, devid);
        return DRV_ERROR_PARA_ERROR;
    }
    if (va != start) {
        svm_err("Should be the registered start va. (va=0x%llx; start=0x%llx)\n", va, start);
        return DRV_ERROR_PARA_ERROR;
    }

    svm_dst_va_pack(devid, PROCESS_CP1, start, src_info.size, &dst_info);
    ret = svm_smm_client_unmap(&dst_info, &src_info, smm_flag);
    if (ret != 0) {
        svm_warn("Smm unmap failed. (devid=%u; va=0x%llx)\n", devid, start);
    }

    (void)svm_svmm_del_seg(svmm_inst, devid, start, src_info.size, true);

    return 0;
}

static int svm_vmm_register(void *va_handle, u64 va, u64 size, u32 devid)
{
    void *vmm_svmm_inst = NULL, *register_svmm_inst = NULL;
    void *seg_handle = NULL;
    u32 seg_devid;
    u64 svm_flag, seg_va;
    struct svm_global_va src_info;
    int ret;

    vmm_svmm_inst = vmm_get_svmm(va_handle);
    if (vmm_svmm_inst == NULL) {
        svm_err("Vmm alloc not finish. (va=0x%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    seg_handle = svm_svmm_seg_handle_get(vmm_svmm_inst, va);
    if (seg_handle == NULL) { /* maybe va has been unmap after get prop or not map */
        svm_err("Get seg handle failed. (va=%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    /* seg va may be update to seg start va which is small than register va */
    seg_va = va;
    ret = svm_svmm_get_seg_by_va(vmm_svmm_inst, &seg_devid, &seg_va, &svm_flag, &src_info);
    if (ret != 0) {
        svm_svmm_seg_handle_put(seg_handle);
        /* should not in, because we has been get seg handle */
        svm_err("Get seg info failed. (va=%llx)\n", va);
        return ret;
    }

    vmm_restore_real_src_va(&src_info);

    register_svmm_inst = svm_vmm_get_register_svmm(seg_handle, devid);
    if (register_svmm_inst == NULL) { /* vmm map addr first register */
        register_svmm_inst = svm_vmm_create_register_svmm(seg_handle, devid, seg_va, src_info.size, svm_flag);
        if (register_svmm_inst == NULL) {
            svm_svmm_seg_handle_put(seg_handle);
            return DRV_ERROR_OUT_OF_MEMORY;
        }
    }

    /* update register va real src info */
    src_info.va += va - seg_va;
    src_info.size = size;
    ret = svm_register(register_svmm_inst, va, size, devid, &src_info);
    if (ret == 0) {
        u64 flag = svm_flag | SVM_FLAG_CAP_LDST;
        svm_svmm_mod_seg_svm_flag(seg_handle, flag);
    }

    svm_svmm_seg_handle_put(seg_handle);

    return ret;
}

static int svm_vmm_unregister(void *va_handle, u64 va, u32 devid)
{
    void *vmm_svmm_inst = NULL, *register_svmm_inst = NULL;
    void *seg_handle = NULL;
    int ret;

    vmm_svmm_inst = vmm_get_svmm(va_handle);
    if (vmm_svmm_inst == NULL) {
        svm_err("Vmm alloc not finish. (va=0x%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    seg_handle = svm_svmm_seg_handle_get(vmm_svmm_inst, va);
    if (seg_handle == NULL) { /* maybe va has been unmap after get prop or not map */
        svm_err("Get seg handle failed. (va=%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    register_svmm_inst = svm_vmm_get_register_svmm(seg_handle, devid);
    if (register_svmm_inst == NULL) { /* vmm map addr first register */
        svm_svmm_seg_handle_put(seg_handle);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = svm_unregister(register_svmm_inst, va, devid);
    if (ret == 0) {
        u64 flag = svm_svmm_get_seg_svm_flag(seg_handle) & (~SVM_FLAG_CAP_LDST);
        svm_svmm_mod_seg_svm_flag(seg_handle, flag);
    }

    svm_svmm_seg_handle_put(seg_handle);

    return ret;
}

static int svm_normal_register(void *va_handle, u64 va, u64 size, u32 devid, struct svm_prop *prop)
{
    void *svmm_inst = NULL;
    struct svm_global_va src_info;
    int ret;

    svmm_inst = svm_normal_get_register_svmm(va_handle, devid);
    if (svmm_inst == NULL) { /* vmm map addr first register */
        svmm_inst = svm_normal_create_register_svmm(va_handle, devid, prop->start, prop->aligned_size, prop->flag);
        if (svmm_inst == NULL) {
            return DRV_ERROR_OUT_OF_MEMORY;
        }
    }

    /* trans devid to udevid in svm_register */
    svm_global_va_pack(prop->devid, prop->tgid, va, size, &src_info);

    ret = svm_register(svmm_inst, va, size, devid, &src_info);
    if (ret == 0) {
        if (prop->devid < SVM_MAX_DEV_AGENT_NUM) {
            u64 flag = prop->flag | SVM_FLAG_CAP_LDST;
            svm_mod_prop_flag(va_handle, flag);
        }
    }

    return ret;
}

static int svm_normal_unregister(void *va_handle, u64 va, u32 devid, struct svm_prop *prop)
{
    void *svmm_inst = NULL;
    int ret;

    svmm_inst = svm_normal_get_register_svmm(va_handle, devid);
    if (svmm_inst == NULL) { /* vmm map addr first register */
        return DRV_ERROR_PARA_ERROR;
    }

    ret = svm_unregister(svmm_inst, va, devid);
    if (ret == 0) {
        if (prop->devid < SVM_MAX_DEV_AGENT_NUM) {
            u64 flag = prop->flag & (~SVM_FLAG_CAP_LDST);
            svm_mod_prop_flag(va_handle, flag);
        }
    }

    return ret;
}

static int _svm_register_to_peer(u64 va, u64 size, u32 devid)
{
    void *va_handle = NULL;
    struct svm_prop prop;
    u64 aligned_size;
    u32 host_devid = svm_get_host_devid();
    int ret;

    ret = svm_get_prop(va, &prop);
    if (ret != 0) {
        svm_err("Get prop failed. (va=0x%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    if (!svm_flag_cap_is_support_register(prop.flag)) {
        svm_err("Addr cap is not support register. (va=0x%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    if (!(((prop.devid == host_devid) && (devid < SVM_MAX_DEV_AGENT_NUM)) ||
        ((prop.devid < SVM_MAX_DEV_AGENT_NUM) && (devid == host_devid)))) {
        svm_err("Only support h<->d. (register_va_devid=%u; access_devid=%u)\n", prop.devid, devid);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = svm_share_get_src_aligned_size(prop.devid, prop.flag, va, size, &aligned_size);
    if (ret != 0) {
        svm_err("Get aligned size failed. (src_devid=%u; flag=0x%llx; va=0x%llx; size=0x%llx)\n",
            prop.devid, prop.flag, va, size);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = svm_share_check_dst_va_align(va, aligned_size, devid, va);
    if (ret != 0) {
        svm_err("Check dst va align failed. (va=0x%llx; aligned_size=0x%llx; devid=%u)\n",
            va, aligned_size, devid);
        return DRV_ERROR_PARA_ERROR;
    }

    if ((va + size) > (prop.start + prop.size)) {
        svm_err("Size if out of bounds. (va=0x%llx; size=%d; align_size=0x%llx; prop start=0x%llx; prop size=0x%llx)\n",
            va, size, aligned_size, prop.start, prop.size);
        return DRV_ERROR_PARA_ERROR;
    }

    va_handle = svm_handle_get(va);
    if (va_handle == NULL) { /* maybe va has been free after get prop */
        svm_err("Get va handle failed. (va=0x%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    if (svm_flag_cap_is_support_vmm_unmap(prop.flag)) {
        ret = svm_vmm_register(va_handle, va, aligned_size, devid);
    } else {
        ret = svm_normal_register(va_handle, va, aligned_size, devid, &prop);
    }

    svm_handle_put(va_handle);
    return ret;
}

static int _svm_unregister_to_peer(u64 va, u32 devid)
{
    void *va_handle = NULL;
    struct svm_prop prop;
    u32 host_devid = svm_get_host_devid();
    int ret;

    ret = svm_get_prop(va, &prop);
    if (ret != 0) {
        svm_err("Get prop failed. (va=0x%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    if (!svm_flag_cap_is_support_register(prop.flag)) {
        svm_err("Addr cap is not support register. (va=0x%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    if (!(((prop.devid == host_devid) && (devid < SVM_MAX_DEV_AGENT_NUM)) ||
        ((prop.devid < SVM_MAX_DEV_AGENT_NUM) && (devid == host_devid)))) {
        svm_err("Only support h<->d. (register_va_devid=%u; access_devid=%u)\n", prop.devid, devid);
        return DRV_ERROR_PARA_ERROR;
    }

    va_handle = svm_handle_get(va);
    if (va_handle == NULL) { /* maybe va has been free after get prop */
        svm_err("Get va handle failed. (va=0x%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    if (svm_flag_cap_is_support_vmm_unmap(prop.flag)) {
        ret = svm_vmm_unregister(va_handle, va, devid);
    } else {
        ret = svm_normal_unregister(va_handle, va, devid, &prop);
    }

    svm_handle_put(va_handle);
    return ret;
}

int svm_register_to_peer(u64 va, u64 size, u32 devid)
{
    int ret;

    svm_use_pipeline();
    ret = _svm_register_to_peer(va, size, devid);
    svm_unuse_pipeline();

    return ret;
}

int svm_unregister_to_peer(u64 va, u32 devid)
{
    int ret;

    svm_use_pipeline();
    ret = _svm_unregister_to_peer(va, devid);
    svm_unuse_pipeline();

    return ret;
}

