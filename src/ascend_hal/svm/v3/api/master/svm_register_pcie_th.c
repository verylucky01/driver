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

#include "svm_user_adapt.h"
#include "svm_log.h"
#include "svm_pagesize.h"
#include "svm_register_to_master.h"
#include "svmm.h"
#include "va_allocator.h"
#include "malloc_mng.h"
#include "smm_client.h"
#include "svm_pipeline.h"
#include "svm_addr_desc.h"
#include "svm_dbi.h"
#include "svm_apbi.h"
#include "svm_sys_cmd.h"
#include "svm_register_pcie_th.h"

#define MAX_VA_SIZE (256 * SVM_BYTES_PER_TB)

struct svm_show_para {
    char *buf;
    u32 buf_len;
};

struct svm_pcie_th_priv {
    long ref;
    u64 va;
    u64 size;
    u64 align;
};

static void *pcie_th_svmm[SVM_MAX_DEV_AGENT_NUM] = {NULL};
static pthread_mutex_t pcie_th_mutex = PTHREAD_MUTEX_INITIALIZER;

static int svm_register_pcie_th_inst_create(u32 devid)
{
    (void)pthread_mutex_lock(&pcie_th_mutex);
    if ((devid < SVM_MAX_DEV_AGENT_NUM) && (pcie_th_svmm[devid] == NULL)) {
        pcie_th_svmm[devid] = svm_svmm_create_inst(0, MAX_VA_SIZE, SVMM_NON_OVERLAP, 0);
        if (pcie_th_svmm[devid] == NULL) {
            (void)pthread_mutex_unlock(&pcie_th_mutex);
            return DRV_ERROR_OUT_OF_MEMORY;
        }
    }
    (void)pthread_mutex_unlock(&pcie_th_mutex);
    return 0;
}

static int svm_register_pcie_th_src_recycle(void *seg_handle, u64 start, struct svm_global_va *src_info, void *priv)
{
    struct svm_dst_va dst_info;
    u32 devid = *(u32 *)priv;
    void *va_handle = NULL;
    SVM_UNUSED(seg_handle);

    svm_dst_va_pack(devid, PROCESS_CP1, start, src_info->size, &dst_info);
    (void)svm_smm_client_unmap(&dst_info, src_info, 0);
    if (svm_va_is_in_range(src_info->va, src_info->size)) {
        va_handle = svm_handle_get(src_info->va);
        if (va_handle != NULL) {
            svm_handle_put(va_handle); /* pair with register */
            svm_handle_put(va_handle);
        }
    }
    return 0;
}

void svm_register_pcie_th_recycle(u32 devid)
{
    (void)pthread_mutex_lock(&pcie_th_mutex);
    if (pcie_th_svmm[devid] != NULL) {
        (void)svm_svmm_for_each_seg_handle(pcie_th_svmm[devid], svm_register_pcie_th_src_recycle, &devid);
        svm_svmm_destroy_inst(pcie_th_svmm[devid]);
        pcie_th_svmm[devid] = NULL;
    }
    (void)pthread_mutex_unlock(&pcie_th_mutex);
}

static int __attribute__ ((constructor)) svm_register_pcie_th_init(void)
{
    int ret;

    ret = svm_register_ioctl_dev_init_post_handle(svm_register_pcie_th_inst_create);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Register ioctl dev init post handle failed.\n");
    }

    return 0;
}

static int svm_register_pcie_th_show_node(void *seg_handle, u64 start, struct svm_global_va *src_info, void *priv)
{
    struct svm_show_para *para = (struct svm_show_para *)priv;
    char *buf = para->buf;
    u32 buf_len = para->buf_len;

    int len = snprintf_s(buf, buf_len, buf_len - 1, "0x%llx   0x%llx   %u   0x%llx\n",
        src_info->va, src_info->size, svm_svmm_get_seg_devid(seg_handle), start);
    if (len >= 0) {
        para->buf += len;
        para->buf_len -= (u32)len;
    }

    return 0;
}

u32 svm_show_register_pcie_th(u32 devid, char *buf, u32 buf_len)
{
    struct svm_show_para para = {.buf = buf, .buf_len = buf_len};
    int len;

    if (devid >= SVM_MAX_DEV_AGENT_NUM) {
        return 0;
    }

    len = snprintf_s(buf, buf_len, buf_len - 1, "pcie th register: src va, size, dst devid, dst va\n");
    para.buf += len;
    para.buf_len -= (u32)len;

    (void)pthread_mutex_lock(&pcie_th_mutex);
    if (pcie_th_svmm[devid] != NULL) {
        (void)svm_svmm_for_each_seg_handle(pcie_th_svmm[devid], svm_register_pcie_th_show_node, &para);
    }
    (void)pthread_mutex_unlock(&pcie_th_mutex);

    return buf_len - para.buf_len;
}

static void svm_pcie_th_addr_ref_inc(void *priv)
{
    struct svm_pcie_th_priv *pcie_th_priv = (struct svm_pcie_th_priv *)priv;
    pcie_th_priv->ref += 1;
}

static void svm_pcie_th_addr_ref_dec(void *priv)
{
    struct svm_pcie_th_priv *pcie_th_priv = (struct svm_pcie_th_priv *)priv;
    pcie_th_priv->ref -= 1;
}

static long svm_pcie_th_addr_ref_read(void *priv)
{
    struct svm_pcie_th_priv *pcie_th_priv = (struct svm_pcie_th_priv *)priv;
    return pcie_th_priv->ref;
}

static void svm_init_pcie_th_seg_priv(void *priv, u64 va, u64 size, u64 align)
{
    struct svm_pcie_th_priv *pcie_th_priv = (struct svm_pcie_th_priv *)priv;

    pcie_th_priv->ref = 0;
    pcie_th_priv->va = va;
    pcie_th_priv->size = size;
    pcie_th_priv->align = align;
}

static int svm_alloc_pcie_th_seg_priv(void **priv)
{
    struct svm_pcie_th_priv *pcie_th_priv =
        (struct svm_pcie_th_priv *)svm_ua_calloc(1, sizeof(struct svm_pcie_th_priv));
    if (pcie_th_priv == NULL) {
        svm_err("Alloc seg priv failed.\n");
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    *priv = (void *)pcie_th_priv;
    return DRV_ERROR_NONE;
}

static void svm_free_pcie_th_seg_priv(void *priv)
{
    svm_ua_free(priv);
}

static int svm_alloc_pcie_th_va(u64 size, u64 align, u64 *va)
{
    u32 flag = 0;
    int ret;

    flag |= SVM_VA_ALLOCATOR_FLAG_WITH_MASTER;
    flag |= SVM_VA_ALLOCATOR_FLAG_MASTER_UVA;

    ret = svm_alloc_va(0ULL, size, align, svm_get_host_devid(), flag, va);
    if (ret != 0) {
        svm_err("Alloc pcie th va failed. (align=0x%llx; size=0x%llx)\n", align, size);
        return ret;
    }

    return DRV_ERROR_NONE;
}

static void svm_free_pcie_th_va(u64 va, u64 size, u64 align)
{
    u32 flag = 0;
    int ret;

    flag |= SVM_VA_ALLOCATOR_FLAG_WITH_MASTER;
    flag |= SVM_VA_ALLOCATOR_FLAG_MASTER_UVA;

    ret = svm_free_va(va, size, align, svm_get_host_devid(), flag);
    if (ret != 0) {
        svm_err("Free pcie th va failed. (align=0x%llx; size=0x%llx)\n", align, size);
    }
}

static int svm_query_register_dev_pcie_th_addr(u32 devid, u64 va, u64 size,
    u64 *pcie_th_addr, void **priv, struct svm_global_va *src_info)
{
    void *svmm_inst = pcie_th_svmm[devid];
    u64 svm_flag, query_va;

    svm_global_va_pack(svm_get_host_devid(), (int)drvDeviceGetBareTgid(), va, size, src_info);

    if (svm_svmm_get_seg_by_src_udevid_and_va(svmm_inst, &devid, &query_va, &svm_flag, src_info) == 0) {
        void *seg_handle = svm_svmm_seg_handle_get(svmm_inst, query_va);
        if (seg_handle != NULL) {
            *pcie_th_addr = query_va;
            *priv = svm_svmm_get_seg_priv(seg_handle);
            svm_svmm_seg_handle_put(seg_handle);
            return DRV_ERROR_NONE;
        }
    }

    return DRV_ERROR_NOT_EXIST;
}

static int svm_query_register_pcie_th_addr(u64 va, u64 size, u64 *pcie_th_addr, void **priv)
{
    u32 devid;

    for (devid = 0; devid < SVM_MAX_DEV_AGENT_NUM; devid++) {
        if (pcie_th_svmm[devid] != NULL) {
            struct svm_global_va src_info;
            int ret = svm_query_register_dev_pcie_th_addr(devid, va, size, pcie_th_addr, priv, &src_info);
            if (ret == DRV_ERROR_NONE) {
                return ret;
            }
        }
    }

    return DRV_ERROR_NOT_EXIST;
}

static int svm_register_pcie_th_seg_priv_release(void *priv, bool force)
{
    SVM_UNUSED(force);

    svm_pcie_th_addr_ref_dec(priv);

    if (svm_pcie_th_addr_ref_read(priv) <= 0) {
        struct svm_pcie_th_priv *pcie_th_priv = (struct svm_pcie_th_priv *)priv;
        svm_free_pcie_th_va(pcie_th_priv->va, pcie_th_priv->size, pcie_th_priv->align);
        svm_free_pcie_th_seg_priv(priv);
    }

    return 0;
}

static struct svm_svmm_seg_priv_ops pcie_th_seg_priv_ops = {
    .release = svm_register_pcie_th_seg_priv_release,
};

static int svm_register_pcie_th_add_seg(u32 devid, struct svm_global_va *src_info, u64 dst_va, void *seg_priv)
{
    void *svmm_inst = pcie_th_svmm[devid];
    void *seg_handle = NULL;
    u64 svm_flag= 0ULL;
    int ret;

    ret = svm_svmm_add_seg(svmm_inst, devid, dst_va, svm_flag, src_info);
    if (ret != 0) {
        svm_err("Add seg failed. (devid=%u; va=0x%llx)\n", devid, dst_va);
        return ret;
    }

    if (seg_priv != NULL) {
        seg_handle = svm_svmm_seg_handle_get(svmm_inst, dst_va);
        if (seg_handle == NULL) {
            (void)svm_svmm_del_seg(svmm_inst, devid, dst_va, src_info->size, true);
            svm_err("Get seg failed. (devid=%u; va=0x%llx)\n", devid, dst_va);
            return DRV_ERROR_INNER_ERR;
        }

        svm_pcie_th_addr_ref_inc(seg_priv);
        svm_svmm_set_seg_priv(seg_handle, seg_priv, &pcie_th_seg_priv_ops);
        svm_svmm_seg_handle_put(seg_handle);
    }
    return 0;
}

static int svm_register_pcie_th_del_seg(u32 devid, u64 dst_va, u64 size)
{
    void *svmm_inst = pcie_th_svmm[devid];
    int ret;

    ret = svm_svmm_del_seg(svmm_inst, devid, dst_va, size, true);
    if (ret != 0) {
        svm_err("Del seg failed. (devid=%u; va=0x%llx)\n", devid, dst_va);
        return ret;
    }

    return 0;
}

static int svm_register_pcie_th_check_va(u64 va, u64 size, u32 devid)
{
    struct svm_prop prop;
    int ret;

    ret = svm_get_prop(va, &prop);
    if (ret != 0) {
        svm_err("Get prop failed. (va=0x%llx)\n", va);
        return DRV_ERROR_PARA_ERROR;
    }

    if (!svm_flag_cap_is_support_register(prop.flag)) {
        svm_run_info("Addr cap is not support register. (va=0x%llx)\n", va);
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (prop.devid != devid) {
        svm_err("Invalid para. (devid=%u)\n", prop.devid);
        return DRV_ERROR_PARA_ERROR;
    }

    if ((va + size) > (prop.start + prop.size)) {
        svm_err("Size if out of bounds. (va=0x%llx; size=0x%llx; prop start=0x%llx; size=0x%llx)\n",
            va, size, prop.start, prop.size);
        return DRV_ERROR_INVALID_VALUE;
    }

    return 0;
}

static u32 svm_get_register_to_master_flag_by_register_pcie_th_flag(u32 register_pcie_th_flag)
{
    u32 register_to_master_flag = 0;

    register_to_master_flag |= REGISTER_TO_MASTER_FLAG_ACCESS_WRITE;
    register_to_master_flag |= ((register_pcie_th_flag & SVM_REGISTER_PCIE_TH_FLAG_VA_IO_MAP) != 0) ?
        REGISTER_TO_MASTER_FLAG_VA_IO_MAP : 0;

    return register_to_master_flag;
}

static int _svm_register_pcie_th_locked(u64 va, u64 size, u32 flag, u32 devid, bool is_malloc_va, u64 dst_va, void *seg_priv)
{
    u32 register_flag = svm_get_register_to_master_flag_by_register_pcie_th_flag(flag);
    struct svm_dst_va register_va, dst_info;
    struct svm_global_va src_info;
    u64 svm_flag, aligned_size, query_va;
    u32 host_devid = svm_get_host_devid();
    int ret;

    if (!is_malloc_va) {
        ret = svm_register_pcie_th_check_va(va, size, host_devid);
        if (ret != 0) {
            return ret;
        }
    }

    ret = svm_get_aligned_size(devid, 0ULL, size, &aligned_size);
    if (ret != 0) {
        svm_err("Get aligned size failed. (devid=%u; va=0x%llx; size=0x%llx)\n", devid, va, size);
        return ret;
    }

    svm_global_va_pack(host_devid, (int)drvDeviceGetBareTgid(), va, aligned_size, &src_info);
    ret = svm_svmm_get_seg_by_src_udevid_and_va(pcie_th_svmm[devid], &devid, &query_va, &svm_flag, &src_info);
    if (ret == 0) {
        svm_err("Addr is already register to device. (va=0x%llx; devid=%u)\n", va, devid);
        return DRV_ERROR_REPEATED_USERD;
    }

    svm_dst_va_pack(host_devid, 0, va, aligned_size, &register_va);
    ret = svm_register_to_master(devid, &register_va, register_flag);
    if ((ret != 0) && (ret != DRV_ERROR_BUSY)) {
        return ret;
    }

    svm_dst_va_pack(devid, PROCESS_CP1, dst_va, aligned_size, &dst_info);
    ret = svm_smm_client_map(&dst_info, &src_info, 0);
    if (ret != 0) {
        svm_err("Smm map failed. (devid=%u; va=0x%llx)\n", devid, va);
        goto unregister_master;
    }

    ret = svm_register_pcie_th_add_seg(devid, &src_info, dst_va, seg_priv);
    if (ret != 0) {
        goto client_unmap;
    }

    return 0;
client_unmap:
    (void)svm_smm_client_unmap(&dst_info, &src_info, 0);
unregister_master:
    if (is_malloc_va) {
        (void)svm_unregister_to_master(devid, &register_va, 0);
    }
    return ret;
}

static int svm_register_pcie_th_locked(u64 va, u64 size, u32 flag, u32 devid, bool is_malloc_va, u64 *dst_va)
{
    u64 aligned_size, npage_size;
    void *seg_priv = NULL;
    bool alloc_va = false;
    int ret;

    if (!va_is_in_pcie_th_range(va, size)) {
        ret = svm_get_aligned_size(devid, 0ULL, size, &aligned_size);
        if (ret != 0) {
            svm_err("Get aligned size failed. (devid=%u; size=0x%llx)\n", devid, size);
            return ret;
        }

        ret = svm_dbi_query_npage_size(devid, &npage_size);
        if (ret != 0) {
            svm_err("Get npage size failed. (devid=%u)\n", devid);
            return ret;
        }

        ret = svm_query_register_pcie_th_addr(va, aligned_size, dst_va, &seg_priv);
        if (ret != DRV_ERROR_NONE) {
            ret = svm_alloc_pcie_th_seg_priv(&seg_priv);
            if (ret != DRV_ERROR_NONE) {
                return ret;
            }

            ret = svm_alloc_pcie_th_va(aligned_size, npage_size, dst_va);
            if (ret != DRV_ERROR_NONE) {
                svm_free_pcie_th_seg_priv(seg_priv);
                return ret;
            }
            svm_init_pcie_th_seg_priv(seg_priv, *dst_va, aligned_size, npage_size);
            alloc_va = true;
        }
    } else {
        *dst_va = va;
    }

    ret = _svm_register_pcie_th_locked(va, size, flag, devid, is_malloc_va, *dst_va, seg_priv);
    if (ret != DRV_ERROR_NONE) {
        if (alloc_va) {
            svm_free_pcie_th_va(*dst_va, aligned_size, npage_size);
            svm_free_pcie_th_seg_priv(seg_priv);
        }
    }

    return ret;
}

static inline void svm_update_err_code(u32 devid, int *err_code)
{
    int ret, tgid;

    if (*err_code != DRV_ERROR_BUSY) {
        return;
    }

    ret = svm_apbi_query_tgid(devid, PROCESS_CP1, &tgid);
    *err_code = (ret == DRV_ERROR_NO_PROCESS) ? ret : *err_code;
}

static int svm_unregister_pcie_th_locked(u64 va, u32 devid, bool is_malloc_va)
{
    struct svm_dst_va register_va, dst_info;
    struct svm_global_va src_info;
    void *seg_priv = NULL;
    u32 host_devid = svm_get_host_devid();
    u64 dst_va;
    int ret;

    if (!is_malloc_va) {
        ret = svm_register_pcie_th_check_va(va, 1, host_devid);
        if (ret != 0) {
            return ret;
        }
    }

    ret = svm_query_register_dev_pcie_th_addr(devid, va, 0, &dst_va, &seg_priv, &src_info);
    if (ret != 0) {
        svm_err("Addr is not register to device. (va=0x%llx; devid=%u)\n", va, devid);
        return ret;
    }

    ret = svm_register_pcie_th_del_seg(devid, dst_va, src_info.size);
    if (ret != 0) {
        svm_err("Del seg failed. (devid=%u; va=0x%llx)\n", devid, dst_va);
        return ret;
    }

    svm_dst_va_pack(devid, PROCESS_CP1, dst_va, src_info.size, &dst_info);
    ret = svm_smm_client_unmap(&dst_info, &src_info, 0);
    if (ret != 0) {
        svm_err("Smm unmap failed. (devid=%u; va=0x%llx)\n", devid, va);
        return ret;
    }

    if (is_malloc_va) {
        svm_dst_va_pack(host_devid, 0, va, src_info.size, &register_va);
        ret = svm_unregister_to_master(devid, &register_va, 0);
        if (ret != 0) {
            svm_err("Unregister to master failed. (devid=%u; va=0x%llx; ret=%d)\n", devid, va, ret);
            svm_update_err_code(devid, &ret);
            return ret;
        }
    }

    return 0;
}

static bool _svm_va_is_pcie_th_register(u64 va, u32 devid)
{
    struct svm_global_va src_info;
    u64 dst_va, svm_flag;
    int ret;

    svm_global_va_pack(svm_get_host_devid(), (int)drvDeviceGetBareTgid(), va, 0, &src_info);
    ret = svm_svmm_get_seg_by_src_udevid_and_va(pcie_th_svmm[devid], &devid, &dst_va, &svm_flag, &src_info);
    if (ret != DRV_ERROR_NONE) {
        return false;
    }

    return true;
}

bool svm_va_is_pcie_th_register(u64 va, u32 devid)
{
    bool ret;

    if (devid >= SVM_MAX_DEV_AGENT_NUM) {
        svm_err("Invalid para. (devid=%u)\n", devid);
        return false;
    }

    svm_use_pipeline();
    if (pcie_th_svmm[devid] == NULL) {
        svm_unuse_pipeline();
        svm_err("Init failed\n");
        return false;
    }

    (void)pthread_mutex_lock(&pcie_th_mutex);
    ret = _svm_va_is_pcie_th_register(va, devid);
    (void)pthread_mutex_unlock(&pcie_th_mutex);
    svm_unuse_pipeline();

    return ret;
}

static int _svm_register_pcie_th(u64 va, u64 size, u32 flag, u32 devid, u64 *dst_va)
{
    void *va_handle = NULL;
    bool is_malloc_va = true;
    int ret;

    if (svm_va_is_in_range(va, size)) {
        if ((flag & SVM_REGISTER_PCIE_TH_FLAG_VA_IO_MAP) != 0) {
            svm_err("Svm addr not support va io map. (devid=%u; va=0x%llx; size=%llu)\n", devid, va, size);
            return DRV_ERROR_PARA_ERROR;
        }

        va_handle = svm_handle_get(va);
        if (va_handle == NULL) {
            svm_err("Get va handle failed. (va=0x%llx)\n", va);
            return DRV_ERROR_PARA_ERROR;
        }
        is_malloc_va = false;
    }

    ret = svm_register_pcie_th_locked(va, size, flag, devid, is_malloc_va, dst_va);
    if (ret != DRV_ERROR_NONE) {
        if (va_handle != NULL) {
            svm_handle_put(va_handle);
        }
    }

    return ret;
}

int svm_register_pcie_th(u64 va, u64 size, u32 flag, u32 devid, u64 *dst_va)
{
    int ret;

    if (devid >= SVM_MAX_DEV_AGENT_NUM) {
        svm_err("Invalid para. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    if (svm_get_device_connect_type(devid) != HOST_DEVICE_CONNECT_TYPE_PCIE) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    svm_use_pipeline();
    if (pcie_th_svmm[devid] == NULL) {
        svm_unuse_pipeline();
        svm_err("Init failed\n");
        return DRV_ERROR_INNER_ERR;
    }

    (void)pthread_mutex_lock(&pcie_th_mutex);
    ret = _svm_register_pcie_th(va, size, flag, devid, dst_va);
    (void)pthread_mutex_unlock(&pcie_th_mutex);

    svm_unuse_pipeline();

    return ret;
}

int svm_unregister_pcie_th(u64 va, u32 devid)
{
    void *va_handle = NULL;
    bool is_malloc_va = true;
    int ret;

    if (devid >= SVM_MAX_DEV_AGENT_NUM) {
        svm_err("Invalid para. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    svm_use_pipeline();
    if (pcie_th_svmm[devid] == NULL) {
        svm_unuse_pipeline();
        svm_err("Init failed\n");
        return DRV_ERROR_INNER_ERR;
    }

    (void)pthread_mutex_lock(&pcie_th_mutex);

    if (svm_va_is_in_range(va, 1ULL)) {
        is_malloc_va = false;
    }

    ret = svm_unregister_pcie_th_locked(va, devid, is_malloc_va);
    if (ret == DRV_ERROR_NONE) {
        if (!is_malloc_va) {
            va_handle = svm_handle_get(va);
            if (va_handle != NULL) {
                svm_handle_put(va_handle); /* pair with register */
                svm_handle_put(va_handle);
            }
        }
    }
    (void)pthread_mutex_unlock(&pcie_th_mutex);

    svm_unuse_pipeline();

    return ret;
}

