/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <sched.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include "drv_buff_common.h"
#include "ascend_hal.h"
#include "drv_buff_unibuff.h"
#include "drv_buff_mbuf.h"
#include "drv_buff_memzone.h"
#include "buff_manage_base.h"
#include "buff_mng.h"
#include "grp_mng.h"

#include "buff_query_grp.h"

static drvError_t buff_query_grp(void *in_buff, unsigned int in_len, void *out_buff, unsigned int *out_len)
{
    (void)in_len;
    GrpQueryGroupInfo *query_grp_out = (GrpQueryGroupInfo *)out_buff;
    GrpQueryGroup *query_grp = (GrpQueryGroup *)in_buff;
    int grp_id, len, i;
    drvError_t ret;
    int *pid = NULL;
    int pid_num = 0;

    len = (int)strnlen(query_grp->grpName, BUFF_GRP_NAME_LEN);
    if ((len == 0) || (len >= BUFF_GRP_NAME_LEN)) {
        buff_err("name len err, len: %d, max: %d\n", len, BUFF_GRP_NAME_LEN);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = buff_pool_id_query(query_grp->grpName, &grp_id);
    if (ret != DRV_ERROR_NONE) {
        buff_err("Can not find grp_id by name. (name=%s)\n", query_grp->grpName);
        return DRV_ERROR_INVALID_VALUE;
    }

    pid = (int *)malloc(sizeof(int) * BUFF_PROC_IN_GRP_MAX_NUM);
    if (pid == NULL) {
        buff_err("Malloc pid out of memory.\n");
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    ret = buff_pool_task_query(grp_id, pid, BUFF_PROC_IN_GRP_MAX_NUM, &pid_num);
    if (ret != DRV_ERROR_NONE) {
        buff_err("Query task by grp_id failed. (grp_id=%d)\n", grp_id);
        free(pid);
        return DRV_ERROR_INVALID_VALUE;
    }

    for(i = 0; i < pid_num; i++) {
        query_grp_out[i].pid = pid[i];
        ret = buff_pool_task_attr_query(grp_id, pid[i], &query_grp_out[i].attr);
        if (ret != DRV_ERROR_NONE) {
            buff_warn("can not find attr by grp_id:%d, pid:%d\n", grp_id, pid[i]);
        }
    }
    free(pid);

    *out_len = (unsigned int)(pid_num * (int)sizeof(GrpQueryGroupInfo));
    return DRV_ERROR_NONE;
}

static drvError_t buff_query_grp_proc(void *in_buff, unsigned int in_len, void *out_buff, unsigned int *out_len)
{
    (void)in_len;
    GrpQueryGroupsOfProcInfo *query_out = (GrpQueryGroupsOfProcInfo *)out_buff;
    GrpQueryGroupsOfProc *query_in = (GrpQueryGroupsOfProc *)in_buff;
    int pid = query_in->pid;
    int *grp_id = NULL;
    int grp_num1 = 0;
    int grp_num = 0;
    drvError_t ret;
    unsigned int j;
    int i;

    grp_id = (int *)calloc(BUFF_GRP_MAX_NUM, sizeof(int));
    if (grp_id == NULL) {
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    ret = buff_task_pool_query(pid, grp_id, BUFF_GRP_MAX_NUM, &grp_num);
    if (ret != DRV_ERROR_NONE) {
        buff_warn("Query grp_id that had been added to pool not success. (pid=%d, ret=%d)\n", pid, ret);
        grp_num = 0;
    }

    if (grp_num < BUFF_GRP_MAX_NUM) {
        ret = buff_task_adding_pool_query(pid, &grp_id[grp_num], BUFF_GRP_MAX_NUM - grp_num, &grp_num1);
        if (ret != DRV_ERROR_NONE) {
            buff_warn("Query grp_id that is adding to pool not success. (pid=%d, ret=%d)\n", pid, ret);
            grp_num1 = 0;
        }
        grp_num += grp_num1;
    }

    for (i = 0, j = 0; i < grp_num; i++) {
        ret = buff_pool_name_query(grp_id[i], query_out[j].groupName, BUFF_GRP_NAME_LEN);
        if (ret != DRV_ERROR_NONE) {
            buff_warn("Query group name not success. (grp_id=%d, pid=%d, ret=%d)\n", grp_id[i], pid, ret);
            continue;
        }

        buff_event("Query info. (pid=%d, grp_id=%d, group_name=%s)\n", pid, grp_id[i], query_out[j].groupName);
        ret = buff_pool_task_attr_query(grp_id[i], pid, &query_out[j].attr);
        if (ret != DRV_ERROR_NONE) {
            buff_warn("Query attr not success. (grp_id=%d, pid=%d, ret=%d)\n", grp_id[i], pid, ret);
        } else {
            j++;
        }
    }
    *out_len = (unsigned int)(j * sizeof(GrpQueryGroupsOfProcInfo));
    free(grp_id);

    return DRV_ERROR_NONE;
}

static drvError_t buff_query_grp_id(void *in_buff, unsigned int in_len, void *out_buff, unsigned int *out_len)
{
    (void)in_len;
    GrpQueryGroupIdInfo *query_grp_out = (GrpQueryGroupIdInfo *)out_buff;
    GrpQueryGroupId *query_grp = (GrpQueryGroupId *)in_buff;
    int grp_id, len;
    drvError_t ret;

    len = (int)strnlen(query_grp->grpName, BUFF_GRP_NAME_LEN);
    if ((len == 0) || (len >= BUFF_GRP_NAME_LEN)) {
        buff_err("name len err, len: %d, max: %d\n", len, BUFF_GRP_NAME_LEN);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = buff_pool_id_query(query_grp->grpName, &grp_id);
    if (ret != DRV_ERROR_NONE) {
        buff_err("Can not find grp_id by name. (name=%s)\n", query_grp->grpName);
        return DRV_ERROR_INVALID_VALUE;
    }

    query_grp_out->groupId = grp_id;
    *out_len = (unsigned int)sizeof(GrpQueryGroupIdInfo);

    return DRV_ERROR_NONE;
}

static void buff_cache_query_result_process(GrpQueryGroupAddrInfo *query_grp_out, unsigned int cache_cnt)
{
    unsigned long long addr;
    unsigned int i;

    addr = buff_get_base_addr();
    for (i = 0; i < cache_cnt; i++) {
        query_grp_out[i].addr += addr;
    }
}

static drvError_t buff_query_group_addr_info(void *in_buff, unsigned int in_len, void *out_buff, unsigned int *out_len)
{
    (void)in_len;
    GrpQueryGroupAddrInfo *query_grp_out = (GrpQueryGroupAddrInfo *)out_buff;
    GrpQueryGroupAddrPara *query_grp = (GrpQueryGroupAddrPara *)in_buff;
    unsigned int cache_cnt;
    int grp_id, len;
    drvError_t ret;

    len = (int)strnlen(query_grp->grpName, BUFF_GRP_NAME_LEN);
    if ((len == 0) || (len >= BUFF_GRP_NAME_LEN)) {
        buff_err("Name len err. (len=%u; max_len=%u)\n", len, BUFF_GRP_NAME_LEN);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = buff_pool_id_query(query_grp->grpName, &grp_id);
    if (ret != DRV_ERROR_NONE) {
        buff_err("Can not find grp_id by name. (name=%s)\n", query_grp->grpName);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (buff_is_enable_cache() == true) {
        ret = buff_cache_info_query(grp_id, query_grp->devId, query_grp_out, BUFF_GROUP_ADDR_MAX_NUM, &cache_cnt);
        if (ret != DRV_ERROR_NONE) {
            buff_err("Cache addr query fail. (ret=%d; name=%s; dev_id=%u)\n",
                ret, query_grp->grpName, query_grp->devId);
            return ret;
        }
        buff_cache_query_result_process(query_grp_out, cache_cnt);
    } else {
        ret = buff_group_addr_query(query_grp_out, &cache_cnt);
        if (ret != DRV_ERROR_NONE) {
            return ret;
        }
    }

    *out_len = (unsigned int)(cache_cnt * sizeof(GrpQueryGroupAddrInfo));

    return DRV_ERROR_NONE;
}

static buff_grp_query_func g_buff_grp_query[GRP_QUERY_CMD_MAX] = {
    [GRP_QUERY_GROUP] = buff_query_grp,
    [GRP_QUERY_GROUPS_OF_PROCESS] = buff_query_grp_proc,
    [GRP_QUERY_GROUP_ID] = buff_query_grp_id,
    [GRP_QUERY_GROUP_ADDR_INFO] = buff_query_group_addr_info,
};

int halGrpQuery(GroupQueryCmdType cmd,
    void *inBuff, unsigned int inLen, void *outBuff, unsigned int *outLen)
{
    drvError_t ret;

    if ((outBuff == NULL) || (inBuff == NULL) || (outLen == NULL)) {
        buff_err("point is NULL, in_buff: %pk, out_buff: %pk, out_buff: %pk.\n", inBuff, outBuff, outLen);
        return (int)DRV_ERROR_INVALID_VALUE;
    }

    if (((unsigned int)cmd >= GRP_QUERY_CMD_MAX) || (g_buff_grp_query[cmd] == NULL)) {
        return (int)DRV_ERROR_NOT_SUPPORT;
    }

    ret = g_buff_grp_query[cmd](inBuff, inLen, outBuff, outLen);
    if (ret != DRV_ERROR_NONE) {
        buff_err("buff grp query cmd exec err. cmd: %d, ret: %d\n", cmd, ret);
        return (int)ret;
    }

    return (int)DRV_ERROR_NONE;
}
