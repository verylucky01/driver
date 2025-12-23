/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DSMI_DFT_DMP_COMMAND_H__
#define __DSMI_DFT_DMP_COMMAND_H__
#include "dev_mon_api.h"
#include "dsmi_dft_interface.h"
#include "dsmi_dft_common.h"

int dsmi_cmd_dft_control_start_test(int device_id, DSMI_DFT_CODE dft_para, int *estimate_time);
int dsmi_cmd_dft_get_aging_flag_time(int device_id, DSMI_AGING_CODE aging_type, DSMI_AGINE_DATA *aging_data);
int dsmi_cmd_dft_get_aging_result(int device_id, DSMI_AGING_CODE aging_type, DSMI_AGING_RESULT_DATA *result);
int dsmi_cmd_dft_get_test_result(int device_id, DSMI_DFT_CODE dft_para, DSMI_GET_DFT_STATE_RES *result);
int dsmi_cmd_dft_get_pmu_die(int device_id, unsigned char pmu_type, struct dsmi_soc_die_stru *pdevice_die);
int dsmi_cmd_dft_load_test_lib(int device_id, unsigned char test_type, const char *test_lib_name, int len);
int dsmi_cmd_dft_set_aging_flag_time(int device_id, DSMI_AGING_CODE aging_type, DSMI_AGINE_DATA aging_data);
int dsmi_cmd_dft_write_elabel(int device_id, DSMI_ELABEL_CODE elabel_code, const char *elabel_data, int len);
int dsmi_cmd_dft_clear_elabel(int device_id, DSMI_ELABEL_CODE elabel_code);

int dsmi_cmd_dft_fw_load(int device_id, unsigned char fw_type);
int dsmi_cmd_dft_stress_ctl(int device_id, unsigned char cmd_type);
int dsmi_cmd_dft_stress_start(int device_id, unsigned char cmd_type, const struct stress_test_start_info *ctl_info);
int dsmi_cmd_dft_stress_get_result(int device_id, unsigned char cmd_type, struct stress_test_result *test_result);
int dsmi_cmd_efuse_burn_or_flash_power(int device_id, unsigned char efuse_info_type);
int dsmi_cmd_efuse_data_check(int device_id, DSMI_EFUSE_TYPE efuse_type, DSMI_EFUSE_REQUEST request,
                              const unsigned int *input, unsigned int efuse_info_len, unsigned int *out_flag);
int dsmi_cmd_write_efuse(int device_id, DSMI_EFUSE_TYPE efuse_type, DSMI_EFUSE_REQUEST request, const unsigned int *src,
                         unsigned int efuse_info_len);
int dsmi_cmd_debug_send_data(int device_id, char trg_type, const char *inbuf, unsigned char size_in, char *outbuf,
                             char *size_out);
int dsmi_cmd_get_cntpct(int device_id, struct dsmi_cntpct_stru *cntpct);
int dsmi_equipment_cmd_set_device_info(unsigned int device_id, DSMI_EQUIP_MAIN_CMD main_cmd, unsigned int sub_cmd,
                                       const void *buf, unsigned int buf_size);
int dsmi_equipment_cmd_get_device_info(unsigned int device_id, DSMI_EQUIP_MAIN_CMD main_cmd, unsigned int sub_cmd,
                                       void *buf, unsigned int *size);
#endif /* __DSMI_DFT_DMP_COMMAND_H__ */
