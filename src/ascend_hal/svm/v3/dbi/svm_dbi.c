/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <sys/ioctl.h>
#include <stdio.h>

#include "ascend_hal_error.h"
#include "ascend_hal.h"
#include "svm_init_pri.h"
#include "svm_sys_cmd.h"
#include "svm_ioctl_ex.h"
#include "svm_pub.h"
#include "svm_log.h"
#include "svm_dbi.h"

static struct svm_device_basic_info svm_dbi[SVM_MAX_DEV_NUM];
static int svm_dbi_valid[SVM_MAX_DEV_NUM];
static u32 device_connect_type[SVM_MAX_DEV_NUM];

static bool svm_dev_cap_is_support_sva(u32 flag)
{
    return ((flag & SVM_DEV_CAP_SVA) != 0);
}

static bool svm_dev_cap_is_support_assign_gap(u32 flag)
{
    return ((flag & SVM_DEV_CAP_ASSIGN_GAP) != 0);
}

int svm_dbi_query_host_align_page_size(u32 devid, u64 *page_size)
{
    u64 npage_size, host_npage_size;
    int ret = svm_dbi_query_npage_size(devid, &npage_size);
    if (ret == 0) {
        ret = svm_dbi_query_npage_size(svm_get_host_devid(), &host_npage_size);
        if (ret == 0) {
            if (npage_size > host_npage_size) {
                svm_err("Invalid page size. (devid=%u; npage_size=%llu; host_npage_size=%llu)\n",
                    devid, npage_size, host_npage_size);
                return DRV_ERROR_INVALID_VALUE;
            }

            *page_size = host_npage_size;
        }
    }

    return ret;
}

int svm_dbi_query_npage_size(u32 devid, u64 *npage_size)
{
    if ((devid >= SVM_MAX_DEV_NUM) || (svm_dbi_valid[devid] == 0)) {
        svm_err("Invalid device. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    *npage_size = svm_dbi[devid].npage_size;
    return 0;
}

int svm_dbi_query_hpage_size(u32 devid, u64 *hpage_size)
{
    if ((devid >= SVM_MAX_DEV_NUM) || (svm_dbi_valid[devid] == 0)) {
        svm_err("Invalid device. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    *hpage_size = svm_dbi[devid].hpage_size;
    return 0;
}

int svm_dbi_query_gpage_size(u32 devid, u64 *gpage_size)
{
    if ((devid >= SVM_MAX_DEV_NUM) || (svm_dbi_valid[devid] == 0)) {
        svm_err("Invalid device. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    *gpage_size = svm_dbi[devid].gpage_size;
    return 0;
}

int svm_dbi_query_d2h_acc_mask(u32 devid, u64 *acc_mask)
{
    if ((devid >= SVM_MAX_DEV_NUM) || (svm_dbi_valid[devid] == 0)) {
        svm_err("Invalid device. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    *acc_mask = svm_dbi[devid].d2h_acc_mask;
    return 0;
}

bool svm_dbi_is_support_sva(u32 devid)
{
    if ((devid >= SVM_MAX_DEV_NUM) || (svm_dbi_valid[devid] == 0)) {
        svm_err("Invalid device. (devid=%u)\n", devid);
        return false;
    }

    return svm_dev_cap_is_support_sva(svm_dbi[devid].cap_flag);
}

bool svm_dbi_is_support_assign_gap(u32 devid)
{
    if ((devid >= SVM_MAX_DEV_NUM) || (svm_dbi_valid[devid] == 0)) {
        svm_err("Invalid device. (devid=%u)\n", devid);
        return false;
    }

    return svm_dev_cap_is_support_assign_gap(svm_dbi[devid].cap_flag);
}

static int dbi_user_query(u32 devid, struct svm_device_basic_info *dbi)
{
    struct svm_dbi_query_para para;
    int ret;

    ret = svm_cmd_ioctl(devid, SVM_DBI_QUERY, (void *)&para);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Query failed. (devid=%u; ret=%d)\n", devid, ret);
        return ret;
    }

    if ((para.dbi.npage_size == 0) || (para.dbi.hpage_size == 0) || (para.dbi.gpage_size == 0)) {
        svm_err("Invalid page_size. (page_size=0x%llx; hpage_size=0x%llx; gpage_size=0x%llx)\n",
            para.dbi.npage_size, para.dbi.hpage_size, para.dbi.gpage_size);
        return DRV_ERROR_INNER_ERR;
    }

    *dbi = para.dbi;

    return 0;
}

static int svm_init_device_connect_type(u32 devid)
{
    int64_t value;
    int ret;

    ret = halGetDeviceInfo(devid, MODULE_TYPE_SYSTEM, INFO_TYPE_HD_CONNECT_TYPE, &value);
    if (ret != DRV_ERROR_NONE) {
        svm_err("halGetDeviceInfo failed. (devid=%u; ret=%d)\n", devid, ret);
        return ret;
    }

    device_connect_type[devid] = (u32)value;
    return 0;
}

static int svm_dev_dbi_init(u32 devid)
{
    int ret;

    ret = dbi_user_query(devid, &svm_dbi[devid]);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Call halQueryDevpid failed. (ret=%d; devid=%u)\n", ret, devid);
        return ret;
    }

    if (devid != svm_get_host_devid()) {
        ret = svm_init_device_connect_type(devid);
        if (ret != 0) {
            return ret;
        }
    }

    svm_dbi_valid[devid] = 1;

    return 0;
}

static int svm_dev_dbi_uninit(u32 devid)
{
    svm_dbi_valid[devid] = 0;
    return 0;
}

void __attribute__((constructor(SVM_INIT_PRI_FISRT))) svm_dbi_init(void)
{
    int ret;
    u32 i;

    for (i = 0; i < SVM_MAX_DEV_NUM; i++) {
        device_connect_type[i] = HOST_DEVICE_CONNECT_PROTOCOL_UNKNOWN;
    }

    ret = svm_register_ioctl_dev_init_post_handle(svm_dev_dbi_init);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Register ioctl dev init post handle failed.\n");
    }

    ret = svm_register_ioctl_dev_uninit_pre_handle(svm_dev_dbi_uninit);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Register ioctl dev uninit post handle failed.\n");
    }
}

static void svm_update_cur_server_id(u32 *server_id)
{
    int64_t value;
    int ret;

    ret = halGetDeviceInfo(0, MODULE_TYPE_SYSTEM, INFO_TYPE_SERVER_ID, &value);
    *server_id = (ret == DRV_ERROR_NONE) ? (u32)value : SVM_INVALID_SERVER_ID;
}

u32 svm_get_cur_server_id(void)
{
    static u32 server_id = SVM_INVALID_SERVER_ID + 1;

    if (server_id == (SVM_INVALID_SERVER_ID + 1)) {
        svm_update_cur_server_id(&server_id);
    }

    return server_id;
}

u32 svm_get_device_connect_type(u32 devid)
{
    return (devid >= SVM_MAX_DEV_NUM) ? HOST_DEVICE_CONNECT_PROTOCOL_UNKNOWN : device_connect_type[devid];
}
