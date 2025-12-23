/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef UDA_INTER_H
#define UDA_INTER_H

#include <stdint.h>
#include <stdbool.h>

int uda_get_udevid_by_devid(uint32_t devid, uint32_t *udevid);
int uda_get_devid_by_udevid(uint32_t udevid, uint32_t *devid);
int uda_get_devid_by_mia_dev(uint32_t phy_devid, uint32_t sub_devid, uint32_t *devid);

int uda_get_udevid_by_devid_ex(uint32_t devid, uint32_t *udevid);
int uda_get_devid_by_udevid_ex(uint32_t udevid, uint32_t *devid);

int uda_dev_is_exist(unsigned int udevid, bool *is_exist);

#endif
