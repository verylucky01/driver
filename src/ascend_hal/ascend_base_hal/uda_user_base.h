/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef UDA_USER_BASE_API_H
#define UDA_USER_BASE_API_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int uda_user_get_dev_num(uint32_t *devNum);
int uda_user_get_dev_ids(uint32_t *devices, uint32_t len);
int uda_user_get_dev_num_ex(uint32_t hw_type, uint32_t *devNum);
int uda_user_get_dev_ids_ex(uint32_t hw_type, uint32_t *devices, uint32_t len);
int uda_user_get_vdev_num(uint32_t *devNum);
int uda_user_get_vdev_ids(uint32_t *devices, uint32_t len);
int uda_user_get_phy_id_by_index(uint32_t devid, uint32_t *phyId);
int uda_user_get_index_by_phy_id(uint32_t phyId, uint32_t *devid);
int uda_user_get_device_local_ids(uint32_t *devices, uint32_t len);
int uda_user_get_devid_by_local_devid(uint32_t local_devid, uint32_t *remote_udevid);
int uda_user_get_local_devid_by_host_devid(uint32_t remote_udevid, uint32_t *local_devid);
int uda_user_get_phy_devid_by_logic_devid(unsigned int dev_id, unsigned int *phy_dev_id);
int uda_user_get_host_id(uint32_t *host_id);

int uda_user_get_udevid_by_devid(uint32_t devid, uint32_t *udevid);
int uda_user_get_devid_by_udevid(uint32_t udevid, uint32_t *devid);
int uda_user_get_devid_by_mia_dev(uint32_t phy_devid, uint32_t sub_devid, uint32_t *devid);
int uda_user_get_udevid_by_devid_ex(uint32_t devid, uint32_t *udevid);
int uda_user_get_devid_by_udevid_ex(uint32_t udevid, uint32_t *devid);

#ifdef __cplusplus
}
#endif

#endif
