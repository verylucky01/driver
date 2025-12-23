/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DMS_SILS_H
#define DMS_SILS_H

#include "dsmi_common_interface.h"

int dms_get_emu_subsys_status(unsigned int dev_id, struct dsmi_emu_subsys_state_stru *emu_subsys_state_data);
int dms_get_sils_status(unsigned int dev_id, struct dsmi_safetyisland_status_stru *safetyisland_status_data);
int dms_set_sils_info(unsigned int dev_id, unsigned int sub_cmd, void *buf, unsigned int size);
int dms_get_sils_info(unsigned int dev_id, unsigned int vfid, unsigned int sub_cmd, void *buf, unsigned int *size);

int dms_equipment_set_sils_info(unsigned int dev_id, unsigned int sub_cmd, void *buf, unsigned int size);
int dms_equipment_get_sils_info(unsigned int dev_id, unsigned int sub_cmd, void *buf, unsigned int *size);
#endif