/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_VIRTUAL_INTF_H__
#define __DCMI_VIRTUAL_INTF_H__

#include "dcmi_interface_api.h"
 
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define SHARE_AICPU (-1)

#define DCMI_310P_TEMPLATE_NUM       7
#define DCMI_910_TEMPLATE_NUM        4

#define DCMI_910B_BIN0_TEMPLATE_NUM        2
#define DCMI_910B_BIN2_TEMPLATE_NUM        4
#define DCMI_910B_BIN1_TEMPLATE_NUM        2

extern const char *template_name_310p[DCMI_310P_TEMPLATE_NUM];
extern const char *template_name_910[DCMI_910_TEMPLATE_NUM];

extern const char *template_name_910b_bin0[DCMI_910B_BIN0_TEMPLATE_NUM];
extern const char *template_name_910b_bin2[DCMI_910B_BIN2_TEMPLATE_NUM];
extern const char *template_name_910b_bin1[DCMI_910B_BIN1_TEMPLATE_NUM];

#define DCMI_NPU_WORK_MODE_AMP      0
#define DCMI_NPU_WORK_MODE_SMP      1

struct dcmi_bin_type_910b_map {
    int dcmi_board_id;
    int template_num;
    const char **supported_template;
};

typedef enum {
    VDAVINCI_DOCKER,
    VDAVINCI_VM,
    VDAVINCI_MODE_MAX,
} VDAVINCI_MODE;

int dcmi_get_template_conf(int card_id, const char *template_name, struct dcmi_create_vdev_in *vdev_conf);

int dcmi_check_set_vnpu_permission(void);

bool dcmi_check_vnpu_is_docker_mode(void);

int dcmi_find_vnpu_in_docker_mode(void);

int dcmi_npu_create_vdevice(int card_id, int device_id, struct dcmi_create_vdev_res_stru *vdev,
    struct dcmi_create_vdev_out *out);

int dcmi_npu_destroy_vdevice(int card_id, int device_id, int vdev_id);

int dcmi_vnpu_work_mode_is_support(unsigned int *support);

int dcmi_check_910b_template_name(const char *template_name);

int dcmi_find_vnpu_in_vm_mode_for_one_chip(int cardid, int chipid, int *exit_flag);

int dcmi_find_vnpu_in_vm_mode(void);

int dcmi_check_set_vdevice_mode_parameter(int mode);

int set_disable_sriov(int card_id, int device_id, int err);

int dcmi_check_vnpu_chip_is_vir(int card_id, int device_id, int *vir_flag);

int dcmi_all_vnpu_is_vir(int *all_vir_flag);

bool dcmi_check_card_is_split_phy(int card_id);

int dcmi_check_chip_is_in_split_mode(int card_id, int device_id);

int dcmi_set_sriov_cfg(int card_id, int device_id, unsigned int* sriov_cfg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
 
#endif /* __DCMI_VIRTUAL_INTF_H__ */