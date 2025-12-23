/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <sys/types.h>
#include <unistd.h>

#include "ascend_hal.h"
#include "drv_buff_common.h"
#include "buff_ioctl.h"
#include "buff_manage_base.h"
#include "grp_mng.h"
#include "buff_mng.h"

static drvError_t cache_alloc_para_check(const char *name, GrpCacheAllocPara *para)
{
    int len;

    if ((name == NULL) || (para == NULL)) {
        buff_err("name or alloc_para is null. (name_is_null=%d; para_is_null=%d)\n", (name == NULL), (para == NULL));
        return DRV_ERROR_INVALID_VALUE;
    }

    if (is_cache_size_valid(para->memSize) == false) {
        buff_err("Cache Size is invalid. (cache_memsize=%llu)\n", para->memSize);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (para->allocMaxSize > para->memSize) {
        buff_err("alloc_max_size is larger than mem_size. (alloc_max_size=%u; mem_size=%llu)\n",
            para->allocMaxSize, para->memSize);
        return DRV_ERROR_INVALID_VALUE;
    }

    len = (int)strnlen(name, BUFF_GRP_NAME_LEN);
    if ((len == 0) || (len >= BUFF_GRP_NAME_LEN)) {
        buff_err("Name len err. (len=%d; max=%d)\n", len, BUFF_GRP_NAME_LEN);
        return DRV_ERROR_INVALID_VALUE;
    }
    return DRV_ERROR_NONE;
}

drvError_t halGrpCacheAlloc(const char *name, unsigned int devId, GrpCacheAllocPara *para)
{
    unsigned long long mem_size;
    unsigned long long alloc_max_size;
    drvError_t ret;
    int grp_id;

    ret = cache_alloc_para_check(name, para);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = buff_pool_id_query(name, &grp_id);
    if (ret != DRV_ERROR_NONE) {
        buff_err("Can not find grp_id by name. (name=%s)\n", name);
        return DRV_ERROR_INVALID_VALUE;
    }

    mem_size = buff_kb_to_b(para->memSize);
    alloc_max_size = buff_kb_to_b(para->allocMaxSize);
    ret = buff_pool_cache_create(grp_id, devId, para->memFlag, mem_size, alloc_max_size);
    if (ret != DRV_ERROR_NONE) {
        buff_err("Cache create fail. (ret=%d; name=%s; dev_id=%u; mem_flag=%u; mem_size=%llu; alloc_max_size=%u)\n",
            ret, name, devId, para->memFlag, para->memSize, para->allocMaxSize);
        return ret;
    }

    return DRV_ERROR_NONE;
}

drvError_t halGrpCacheFree(const char *name, unsigned int devId)
{
    drvError_t ret;
    int grp_id;
    int len;

    if (name == NULL) {
        buff_err("Name is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    len = (int)strnlen(name, BUFF_GRP_NAME_LEN);
    if ((len == 0) || (len >= BUFF_GRP_NAME_LEN)) {
        buff_err("Name len err. (len=%d; max=%d)\n", len, BUFF_GRP_NAME_LEN);
        return DRV_ERROR_INVALID_VALUE;
    }

    buff_event("halGrpCacheFree start. (name=%s; dev_id=%u)\n", name, devId);

    ret = buff_pool_id_query(name, &grp_id);
    if (ret != DRV_ERROR_NONE) {
        buff_err("Can not find grp_id by name. (name=%s; ret=%d)\n", name, ret);
        return DRV_ERROR_NO_GROUP;
    }

    ret = buff_pool_cache_destroy(grp_id, devId);
    if ((ret != DRV_ERROR_NONE) && (ret != DRV_ERROR_BUSY) && (ret != DRV_ERROR_NOT_SUPPORT)) {
        buff_err("Cache free fail. (ret=%d; name=%s; dev_id=%u)\n", ret, name, devId);
    }

    return ret;
}

