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
#ifdef CFG_FEATURE_SUPPORT_UB
#include "urma_types.h"
#include "urma_api.h"
#endif
#include "securec.h"
#include "devmng_common.h"
#include "dms_user_common.h"
#include "dms/dms_misc_interface.h"
#include "ascend_hal_error.h"
#include "ascend_hal.h"
#include "dms/dms_devdrv_info_comm.h"
#include "dsmi_common_interface.h"
#include "dms_cmd_def.h"
#include "dmc_user_interface.h"
#include "dms_device_info.h"

#ifndef STATIC
#define STATIC static
#endif

#define DMS_MAX_DEV_NUM (64 + 2)    // 64 means pf, but for 64+1, dev_num may add 2
struct dms_eid_query_info g_ub_dev_info[DMS_MAX_DEV_NUM] = {0};

pthread_mutex_t ub_dev_info_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ub_token_val_lock = PTHREAD_MUTEX_INITIALIZER;

int dms_get_urma_name_by_devid(u32 dev_id, char *name, u32 len)
{
    int ret;
    struct dms_ioctl_arg ioarg = {0};

    if (!name || len == 0) {
        DMS_ERR("Name is null or len is 0. (len=%u)\n", len);
        return DRV_ERROR_PARA_ERROR;
    }

    ioarg.main_cmd = ASCEND_UB_CMD_BASIC;
    ioarg.sub_cmd = ASCEND_UB_SUBCMD_GET_URMA_NAME;
    ioarg.filter_len = 0;
    ioarg.input = (void *)&dev_id;
    ioarg.input_len = sizeof(u32);
    ioarg.output = (void *)name;
    ioarg.output_len = len;

    ret = errno_to_user_errno(DmsIoctl(DMS_IOCTL_CMD, &ioarg));
    if (ret == DRV_ERROR_NO_DEVICE) {
        DMS_WARN("IOCTL no dev, please retry. (ret=%d)\n", ret);
        return DRV_ERROR_NO_DEVICE;
    } else if(ret != 0) {
        DMS_EX_NOTSUPPORT_ERR(ret, "IOCTL failed. (ret=%d)\n", ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

#ifdef CFG_FEATURE_SUPPORT_UB
void dms_copy_urma_eid_from_addr_info(urma_eid_t *eid, union dms_urma_addr_info *addr_info)
{
    int i;

    for (i = 0; i < URMA_EID_SIZE; i++) {
        eid->raw[i] = addr_info->eid[i];
    }

    return;
}

STATIC int dms_cmp_urma_eid(urma_eid_t *eid1, urma_eid_t *eid2)
{
    int i;

    for (i = 0; i < URMA_EID_SIZE; i++) {
        if (eid1->raw[i] != eid2->raw[i]) {
            return DRV_ERROR_INVALID_VALUE;
        }
    }

    return 0;
}

STATIC int dms_fill_eid_index_and_urma_dev(struct dms_eid_query_info *query_info, struct dms_ub_dev_info *eid_info,
    uint32_t dev_id)
{
    urma_eid_t eid;
    urma_eid_info_t *eid_list;
    uint32_t eid_cnt, i;
    urma_device_t *urma_dev = NULL;

    dms_copy_urma_eid_from_addr_info(&eid, &query_info->local_eid[query_info->min_idx]);
    urma_dev = urma_get_device_by_eid(eid, URMA_TRANSPORT_UB);
    if (urma_dev == NULL) {
        DMS_ERR("urma_get_device_by_eid failed.(devid=%u)\n", dev_id);
        return DRV_ERROR_NO_RESOURCES;
    }

    eid_list = urma_get_eid_list(urma_dev, &eid_cnt);
    if (eid_list == NULL) {
        DMS_ERR("urma_get_eid_list failed.(devid=%u)\n", dev_id);
        return DRV_ERROR_NO_RESOURCES;
    }

    for (i = 0; i < eid_cnt; i++) {
        if (dms_cmp_urma_eid(&eid_list[i].eid, &eid) == 0) {
            eid_info->eid_index[0] = eid_list[i].eid_index;
            break;
        }
    }

    if (i == eid_cnt) {
        DMS_ERR("urma_dev find eid_index failed.(devid=%u; eid="EID_FMT")\n", dev_id, EID_ARGS(eid));
        urma_free_eid_list(eid_list);
        return DRV_ERROR_NO_RESOURCES;
    }

    eid_info->urma_dev[0] = (void *)urma_dev;
    urma_free_eid_list(eid_list);
    return 0;
}
#endif

drvError_t dms_get_ub_dev_info(unsigned int dev_id, struct dms_ub_dev_info *eid_info, int *num)
{
#ifdef CFG_FEATURE_SUPPORT_UB
    struct urd_cmd cmd = {0};
    struct urd_cmd_para cmd_para = {0};
    int ret, i;
    struct dms_eid_query_info query_info = {0};

    if ((eid_info == NULL) || (num == NULL)) {
        DMS_ERR("eid_info or num is Null.(devid=%u)\n", dev_id);
        return DRV_ERROR_PARA_ERROR;
    }

    if (dev_id >= DMS_MAX_DEV_NUM) {
        DMS_ERR("dev_id is invalid.(devid=%u)\n", dev_id);
        return DRV_ERROR_PARA_ERROR;
    }

    pthread_mutex_lock(&ub_dev_info_lock);
    if (g_ub_dev_info[dev_id].num != 0) {
        pthread_mutex_unlock(&ub_dev_info_lock);
        // Resource has been initialized, only need to fill in eid_info
        goto FILL_EID_INFO;
    }

    urd_usr_cmd_fill(&cmd, ASCEND_UB_CMD_BASIC, ASCEND_UB_SUBCMD_GET_EID_INDEX, NULL, 0);
    urd_usr_cmd_para_fill(&cmd_para, (void *)&dev_id, sizeof(unsigned int),
        (void *)&query_info, sizeof(struct dms_eid_query_info));
    ret = urd_dev_usr_cmd(dev_id, &cmd, &cmd_para);
    if ((ret != 0) || (query_info.num > DMS_MAX_EID_PAIR_NUM) || (query_info.num <= 0) ||
        (query_info.min_idx >= query_info.num)) {
        pthread_mutex_unlock(&ub_dev_info_lock);
        DMS_EX_NOTSUPPORT_ERR(ret, "Get ub dev info failed. (dev_id=%u; ret=%d; num=%d; min_idx=%d)\n", dev_id, ret,
            query_info.num, query_info.min_idx);
        *num = 0;
        return DRV_ERROR_IOCRL_FAIL;
    }

    g_ub_dev_info[dev_id].num = query_info.num;
    g_ub_dev_info[dev_id].min_idx = query_info.min_idx;
    for (i = 0; i < query_info.num; i++) {
        (void)memcpy_s(&g_ub_dev_info[dev_id].local_eid[i].eid, sizeof(union dms_urma_addr_info),
            &query_info.local_eid[i].eid, sizeof(union dms_urma_addr_info));
    }
    pthread_mutex_unlock(&ub_dev_info_lock);
FILL_EID_INFO:
    ret = dms_fill_eid_index_and_urma_dev(&g_ub_dev_info[dev_id], eid_info, dev_id);
    ret == 0 ? (*num = 1) : (*num = 0);
    return ret;
#else
    (void)dev_id;
    (void)eid_info;
    (void)num;
    return DRV_ERROR_NOT_SUPPORT;
#endif
}

unsigned int g_share_token_val[DMS_MAX_DEV_NUM] = {0};
#define DMS_INVALID_TOKEN_VAL 0
drvError_t dms_get_token_val(unsigned int dev_id, unsigned int type, unsigned int *val)
{
    struct urd_cmd cmd = {0};
    struct urd_cmd_para cmd_para = {0};
    int ret;
    unsigned int tmp_val;

    if ((type >= TOKEN_VAL_TYPE_MAX) || (val == NULL)) {
        DMS_ERR("Type is invalid or val is Null.(type=%u; devid=%u)\n", type, dev_id);
        return DRV_ERROR_PARA_ERROR;
    }

    if (dev_id >= DMS_MAX_DEV_NUM) {
        DMS_ERR("dev_id is invalid.(devid=%u)\n", dev_id);
        return DRV_ERROR_PARA_ERROR;
    }

    pthread_mutex_lock(&ub_token_val_lock);
    if ((g_share_token_val[dev_id] != DMS_INVALID_TOKEN_VAL) && (type == SHARED_TOKEN_VAL)) {
        *val = g_share_token_val[dev_id];
        pthread_mutex_unlock(&ub_token_val_lock);
        return DRV_ERROR_NONE;
    }

    urd_usr_cmd_fill(&cmd, DMS_MAIN_CMD_BASIC, DMS_SUBCMD_GET_TOKEN_VAL, NULL, 0);
    urd_usr_cmd_para_fill(&cmd_para, (void *)&dev_id, sizeof(unsigned int), (void *)&tmp_val, sizeof(unsigned int));
    ret = urd_dev_usr_cmd(dev_id, &cmd, &cmd_para);
    if (ret != 0) {
        pthread_mutex_unlock(&ub_token_val_lock);
        DMS_EX_NOTSUPPORT_ERR(ret, "Get token_val failed. (dev_id=%u; ret=%d)\n", dev_id, ret);
        *val = 0;
        return DRV_ERROR_IOCRL_FAIL;
    }

    if ((type == SHARED_TOKEN_VAL) && (g_share_token_val[dev_id] == DMS_INVALID_TOKEN_VAL)) {
        g_share_token_val[dev_id] = tmp_val;
    }

    *val = tmp_val;
    pthread_mutex_unlock(&ub_token_val_lock);
    return DRV_ERROR_NONE;
}

drvError_t DmsGetUbInfo(unsigned int dev_id, int module_type, int info_type,
    void *buf, unsigned int *size)
{
    int ret;
    struct dms_filter_st filter = {0};
    struct urd_cmd cmd = {0};
    struct urd_cmd_para cmd_para = {0};
    struct dms_hal_device_info_stru in = {0};
    
    if ((buf == NULL) || (size == NULL) || (*size == 0)) {
        return DRV_ERROR_PARA_ERROR;
    }

    DMS_MAKE_UP_FILTER_HAL_DEV_INFO_EX(&filter, module_type, info_type);

    in.dev_id = dev_id;
    in.module_type = module_type;
    in.info_type = info_type;
    in.buff_size = 0;

    urd_usr_cmd_fill(&cmd, DMS_GET_GET_DEVICE_INFO_CMD, ZERO_CMD, &filter.filter[0], filter.filter_len);
    urd_usr_cmd_para_fill(&cmd_para, (void *)&in, DMS_HAL_DEV_INFO_HEAD_LEN, buf, *size);
    ret = urd_dev_usr_cmd(dev_id, &cmd, &cmd_para);
    if (ret != 0) {
        DMS_ERR("Ioctl failed. (ret=%d; dev_id=%u)\n", ret, dev_id);
        return ret;
    }
    return ret;
}

drvError_t dms_get_ub_info(uint32_t devId, int32_t infoType, void *buf, int32_t *size)
{
    int ret;

    switch (infoType) {
        case INFO_TYPE_UB_STATUS:
            ret = DmsHalGetDeviceInfoEx(devId, MODULE_TYPE_UB, infoType, buf, (unsigned int *)size);
            break;

        case INFO_TYPE_UB_PACKET_STATISTICS:
            ret = DmsGetUbInfo(devId, MODULE_TYPE_UB, infoType, buf, (unsigned int *)size);
            break;

        case INFO_TYPE_UB_QOS_INFO:
            ret = DmsGetUbInfo(devId, MODULE_TYPE_UB, infoType, buf, (unsigned int *)size);
            break;

        case INFO_TYPE_UB_CONFIG_INFO:
            ret = DmsGetUbInfo(devId, MODULE_TYPE_UB, infoType, buf, (unsigned int *)size);
            break;

        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    if (ret != 0) {
        DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret, "Failed to get ub info. (dev_id=%u, infoType=%d, ret=%d)\n",
            devId, infoType, ret);
        return ret;
    }
    return DRV_ERROR_NONE;
}