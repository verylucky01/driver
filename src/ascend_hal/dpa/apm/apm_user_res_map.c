/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ascend_hal_define.h"
#include "apm_ioctl.h"
#include "securec.h"

static int apm_res_info_in_check(struct res_map_info_in *res_info_in)
{
    int i;

    if (res_info_in == NULL) {
        apm_err("Null ptr\n");
        return DRV_ERROR_PARA_ERROR;
    }

    if (res_info_in->flag != 0) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    for (i = 0;  i < RES_ADDR_MAP_INFO_IN_RSV_LEN; i++) {
        if (res_info_in->rsv[i] != 0) {
            return DRV_ERROR_NOT_SUPPORT;
        }
    }

    return DRV_ERROR_NONE;
}

static int apm_res_info_out_check(struct res_map_info_out *res_info_out)
{
    int i;

    if (res_info_out == NULL) {
        apm_err("Null ptr\n");
        return DRV_ERROR_PARA_ERROR;
    }

    for (i = 0;  i < RES_ADDR_MAP_INFO_OUT_RSV_LEN; i++) {
        if (res_info_out->rsv[i] != 0) {
            return DRV_ERROR_NOT_SUPPORT;
        }
    }

    return DRV_ERROR_NONE;
}

static int apm_user_res_addr_map(unsigned int devId, struct res_map_info_in *res_info_in,
    struct res_map_info_out *res_info_out)
{
    struct apm_cmd_res_map para;
    int ret;

    ret = apm_res_info_in_check(res_info_in);
    if (ret != 0) {
        return ret;
    }

    ret = apm_res_info_out_check(res_info_out);
    if (ret != 0) {
        return ret;
    }

    para.devid = devId;
    para.res_info = *res_info_in;
    ret = apm_cmd_ioctl(APM_RES_ADDR_MAP, &para);
    if (ret != 0) {
        apm_err("Res addr map fail. (ret=%d; devid=%u; id=%u; proc_type=%d; res_type=%d; res_id=%u; flag=0x%x)\n",
            ret, devId, res_info_in->id, res_info_in->target_proc_type, res_info_in->res_type,
            res_info_in->res_id, res_info_in->flag);
        return ret;
    }

    res_info_out->va = para.va;
    res_info_out->len = para.len;

    return DRV_ERROR_NONE;
}

static int apm_user_res_addr_unmap(unsigned int devId, struct res_map_info_in *res_info_in)
{
    struct apm_cmd_res_unmap para;
    int ret;

    ret = apm_res_info_in_check(res_info_in);
    if (ret != 0) {
        return ret;
    }

    para.devid = devId;
    para.res_info = *res_info_in;
    ret = apm_cmd_ioctl(APM_RES_ADDR_UNMAP, &para);
    if (ret != 0) {
        apm_warn("Res addr unmap warn. (ret=%d; devid=%u; id=%u; proc_type=%d; res_type=%d; res_id=%u; flag=0x%x)\n",
            ret, devId, res_info_in->id, res_info_in->target_proc_type, res_info_in->res_type,
            res_info_in->res_id, res_info_in->flag);
        return ret;
    }

    return DRV_ERROR_NONE;
}

static void apm_user_res_info_in_convert(struct res_addr_info *res_info, struct res_map_info_in *res_info_in)
{
    res_info_in->id = res_info->id;
    res_info_in->target_proc_type = res_info->target_proc_type;
    res_info_in->res_type = res_info->res_type;
    res_info_in->res_id = res_info->res_id;
    res_info_in->flag = res_info->flag;
    (void)memcpy_s(res_info_in->rsv, RES_ADDR_MAP_INFO_IN_RSV_LEN, res_info->rsv, RES_ADDR_INFO_RSV_LEN);
}

drvError_t halResAddrMap(unsigned int devId, struct res_addr_info *res_info, unsigned long *va, unsigned int *len)
{
    struct res_map_info_in res_info_in = {0};
    struct res_map_info_out res_info_out = {0};
    int ret = 0;

    if ((res_info == NULL) || (va == NULL) || (len == NULL)) {
        apm_err("Null ptr\n");
        return DRV_ERROR_PARA_ERROR;
    }

    apm_user_res_info_in_convert(res_info, &res_info_in);
    ret = apm_user_res_addr_map(devId, &res_info_in, &res_info_out);
    if (ret == 0) {
        *va = res_info_out.va;
        *len = res_info_out.len;
    }

    return ret;
}

drvError_t halResAddrUnmap(unsigned int devId, struct res_addr_info *res_info)
{
    struct res_map_info_in res_info_in = {0};

    if (res_info == NULL) {
        apm_err("Null ptr\n");
        return DRV_ERROR_PARA_ERROR;
    }

    apm_user_res_info_in_convert(res_info, &res_info_in);
    return apm_user_res_addr_unmap(devId, &res_info_in);
}

drvError_t halResAddrMapV2(unsigned int devId, struct res_map_info_in *res_info_in,
    struct res_map_info_out *res_info_out)
{
    return apm_user_res_addr_map(devId, res_info_in, res_info_out);
}

drvError_t halResAddrUnmapV2(unsigned int devId, struct res_map_info_in *res_info_in)
{
    return apm_user_res_addr_unmap(devId, res_info_in);
}
