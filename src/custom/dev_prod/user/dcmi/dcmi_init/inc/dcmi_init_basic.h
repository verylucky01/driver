/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_INIT_BASIC_H__
#define __DCMI_INIT_BASIC_H__

#include "dcmi_common.h"

void dcmi_run_env_init(void);

void dcmi_init_ok(void);
 
void dcmi_init_board_details_default(void);
 
int dcmi_init_for_model(const int *device_id_list, int device_count);
 
int dcmi_init_for_soc(const int *device_id_list, int device_count);
 
int dcmi_init_for_server(int *device_id_list, int device_count);
 
void dcmi_310B_trans_baseboard_id(unsigned int *board_id);
 
int dcmi_init_chip_board_product_type(struct tag_pcie_idinfo_all *pcie_id_info);
 
int dcmi_init_board_type(const int *device_logic_id, int device_count);

int dcmi_trans_pcie_common_id(const char *bus_id_str, int common_len, int common_pos, unsigned int *common_num);

int dcmi_get_board_info_handle(int device_logic_id, struct dsmi_board_info_stru *board_info);

int dcmi_init_for_card(const int *device_id_list, int device_count);

int dcmi_flush_device_id(void);

int dcmi_pcie_slot_map_init(void);

void dcmi_card_info_sort(void);

#endif /* __DCMI_INIT_BASIC_H__ */