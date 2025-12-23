/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef BUFF_MANAGE_BASE_H
#define BUFF_MANAGE_BASE_H

#include "ascend_hal_define.h"

#ifdef __cplusplus
extern "C" {
#endif

drvError_t buff_pool_algo_vma_register(const char *name, unsigned long pool_size,
    unsigned int priv_mbuf_flag, int *pool_id);
drvError_t buff_pool_algo_sp_register(const char *name, unsigned long pool_size,
    unsigned int priv_mbuf_flag, int *pool_id);
drvError_t buff_pool_algo_cache_vma_register(const char *name, unsigned long pool_size,
    unsigned int priv_mbuf_flag, int *pool_id);
drvError_t buff_pool_algo_cache_sp_register(const char *name, unsigned long pool_size,
    unsigned int priv_mbuf_flag, int *pool_id);
drvError_t buff_pool_unregister(int pool_id);
drvError_t buff_pool_cache_create(int pool_id, unsigned int dev_id, unsigned int mem_flag,
    unsigned long long mem_size, unsigned long long alloc_max_size);
drvError_t buff_pool_cache_destroy(int pool_id, unsigned int dev_id);
drvError_t buff_pool_add_proc(int pool_id, int pid, GroupShareAttr attr);
drvError_t buff_pool_del_proc(int pool_id, int pid);
drvError_t buff_pool_attach(int pool_id, int timeout);
drvError_t buff_pool_attach_ex(int pool_id, int timeout, unsigned long long *proc_uid, unsigned int *cache_type);
drvError_t buff_pool_detach(int pool_id);
drvError_t buff_pool_blk_alloc(int pool_id, unsigned long size, unsigned long flag, unsigned long *ptr, uint32_t *blk_id);
drvError_t buff_pool_blk_free(int pool_id, unsigned long ptr);
drvError_t buff_pool_blk_get(unsigned long ptr, int *pool_id, unsigned long *alloc_ptr,
    unsigned long *alloc_size, uint32_t *blk_id);
drvError_t buff_pool_blk_put(int pool_id, unsigned long ptr);

drvError_t buff_pool_id_query(const char *name, int *pool_id);
drvError_t buff_pool_name_query(int pool_id, char *name, int name_len);
drvError_t buff_cache_info_query(int pool_id, unsigned int dev_id, GrpQueryGroupAddrInfo *cache_buff,
    unsigned int cache_cnt, unsigned int *query_cnt);
drvError_t buff_pool_priv_flag_query(int pool_id, unsigned int *flag);
drvError_t buff_pool_va_check(int pool_id, unsigned long va, int *result);
drvError_t buff_pool_task_query(int pool_id, int *pid, int pid_num, int *query_num);
drvError_t buff_pool_task_attr_query(int pool_id, int pid, GroupShareAttr *attr);
drvError_t buff_task_pool_query(int pid, int *pool_id, int pool_num, int *query_num);
drvError_t buff_task_adding_pool_query(int pid, int *pool_id, int pool_num, int *query_num);

drvError_t buff_pool_set_prop(int pool_id, const char *prop_name, unsigned long value);
drvError_t buff_pool_set_grp_prop(int pool_id, const char *prop_name, unsigned long value);
drvError_t buff_pool_get_prop(int pool_id, const char *prop_name, unsigned long *value);
drvError_t buff_pool_del_prop(int pool_id, const char *prop_name);
int buff_pool_poll_exit_task(int pool_id, unsigned long long *proc_uid);

#ifdef __cplusplus
}
#endif
#endif
