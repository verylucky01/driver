/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "pbl_uda.h"
#include "ka_system_pub.h"
#include "uda_access_adapt.h"

void uda_init_phy_dev_num(void)
{
#ifndef DRV_HOST
    u32 node_num = (u32)ka_system_cpu_to_node((int)ka_system_num_online_cpus() - 1) + 1;
    (void)uda_set_detected_phy_dev_num(node_num);
#endif
}