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
#include "ka_compiler_pub.h"

#include "pbl_feature_loader.h"
#include "pbl_spod_info.h"
#include "pbl_uda.h"

#include "svm_kern_log.h"
#include "framework_cmd.h"
#include "mwl.h"
#include "casm_kernel.h"
#include "svm_ioctl_ex.h"
#include "casm_ioctl.h"
#include "casm_cs_key.h"

#define CASM_KEY_SERVER_ID_OFFSET 48

static u32 local_server_id = SVM_INVALID_SERVER_ID;

static bool casm_cs_is_local_server_id_uninited(void)
{
    return (local_server_id == SVM_INVALID_SERVER_ID);
}

void casm_cs_set_local_server_id(u32 server_id) /* for emu st, not add static */
{
    local_server_id = server_id;
}

static int casm_init_local_server_id(u32 udevid)
{
    struct spod_info info;
    int ret;

    ret = dbl_get_spod_info(udevid, &info);
    if (ret != 0) {
        svm_err("Get server id failed. (udevid=%u)\n", udevid);
        return ret;
    }

    casm_cs_set_local_server_id(info.server_id);

    return 0;
}

static int casm_cs_query_src_info(u64 key, struct svm_global_va *src_va, int *owner_tgid)
{
    struct casm_src_ex src_ex;
    int ret;

    ret = casm_get_src_va(key, src_va, &src_ex);
    if (ret != 0) {
        svm_err("Get src info failed. (key=0x%llx)\n", key);
        return ret;
    }

    casm_try_to_update_src_va(src_va, src_ex.updated_va);
    src_va->server_id = local_server_id;
    *owner_tgid = src_ex.owner_tgid;

    return 0;
}

static int casm_cs_update_key(u64 *key)
{
    if (casm_cs_is_local_server_id_uninited()) {
        (void)casm_init_local_server_id(casm_key_to_udevid(*key));
    }

    *key |= (u64)local_server_id << CASM_KEY_SERVER_ID_OFFSET;
    return 0;
}

static int casm_cs_parse_key(u64 key, u32 *udevid, u32 *id)
{
    *id = casm_key_to_id(key);
    *udevid = casm_key_to_udevid(key);
    return 0;
}

bool casm_cs_is_local_key(u64 key)
{
    u32 server_id = ((u32)(key >> CASM_KEY_SERVER_ID_OFFSET));
    return (server_id == local_server_id);
}

u32 casm_cs_parse_server_id_from_key(u64 key)
{
    u32 server_id = ((u32)(key >> CASM_KEY_SERVER_ID_OFFSET));
    return server_id;
}

int casm_cs_key_init_dev(u32 udevid)
{
    if (casm_cs_is_local_server_id_uninited() && (udevid != uda_get_host_id())) {
        casm_init_local_server_id(udevid);
    }

    return 0;
}
DECLAER_FEATURE_AUTO_INIT_DEV(casm_cs_key_init_dev, FEATURE_LOADER_STAGE_7);

static const struct svm_casm_key_ops g_casm_cs_key_ops = {
    .update_key = casm_cs_update_key,
    .parse_key = casm_cs_parse_key,
    .is_local_key = casm_cs_is_local_key,
};

static int casm_ioctl_cs_query_src(u32 udevid, u32 cmd, unsigned long arg)
{
    struct svm_casm_cs_query_src_para para;
    int ret;

    ret = (int)ka_base_copy_from_user(&para, (void __ka_user *)(uintptr_t)arg, sizeof(para));
    if (ret != 0) {
        svm_err("Copy_from_user fail.\n");
        return -EINVAL;
    }

    ret = casm_cs_query_src_info(para.key, &para.src_va, &para.owner_pid);
    if (ret != 0) {
        return ret;
    }

    if (ka_base_copy_to_user((void __ka_user *)(uintptr_t)arg, &para, sizeof(para)) != 0) {
        svm_err("copy_to_user fail.\n");
        return -EFAULT;
    }

    return ret;
}

int casm_cs_key_init(void)
{
    svm_register_ioctl_cmd_handle(_IOC_NR(SVM_CASM_CS_QUERY_SRC), casm_ioctl_cs_query_src);

    svm_casm_register_key_ops(&g_casm_cs_key_ops);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(casm_cs_key_init, FEATURE_LOADER_STAGE_7);

