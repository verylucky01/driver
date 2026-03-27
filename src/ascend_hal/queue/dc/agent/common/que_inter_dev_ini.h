/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef QUE_INTER_DEV_INI_H
#define QUE_INTER_DEV_INI_H

#include <stdlib.h>

#include "securec.h"
#include "ascend_hal.h"
#include "ascend_hal_error.h"
 
#define QUE_INI_FLAG  0U
#define QUE_TGT_FLAG  1U

drvError_t que_share_que_name_cmp(char *que_share_name_src, char *que_share_name_dst, unsigned int que_inter_dev_flag);
drvError_t que_set_inter_dev_info(unsigned int remote_devid, pid_t remote_devpid, unsigned int remote_grpid, char *share_que_name, struct queue_manages *que_manage);
drvError_t que_set_inter_dev_info_ex(unsigned int remote_devid, char *share_que_name, struct queue_manages *que_manage);
void que_set_import_que_attr(struct queue_manages *que_mng, struct que_inter_dev_import_out_msg que_out_info);
drvError_t que_inter_dev_match(unsigned int dev_id, char *share_queue_name, unsigned int *qid);
void que_clear_inter_dev_info(struct queue_manages *que_mng);
drvError_t que_inter_dev_export_ini_local(unsigned int dev_id, unsigned int qid, struct shareQueInfo *que_info);
drvError_t que_inter_dev_import_ini(unsigned int dev_id, struct shareQueInfo *que_info, unsigned int *qid);
drvError_t que_inter_dev_unexport_ini_local(unsigned int dev_id, unsigned int qid, struct shareQueInfo *que_info);
drvError_t que_inter_dev_unimport_ini_local(unsigned int dev_id, unsigned int qid, struct shareQueInfo *que_info);

#endif