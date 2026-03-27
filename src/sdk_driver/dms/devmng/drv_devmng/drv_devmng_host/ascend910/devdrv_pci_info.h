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

#ifndef DEVDRV_PCI_INFO_H
#define DEVDRV_PCI_INFO_H

#include "ka_kernel_def_pub.h"

int devdrv_manager_get_pci_info(ka_file_t *filep, unsigned int cmd, unsigned long arg);
int devdrv_get_pcie_id(unsigned long arg);
int devdrv_manager_get_dev_resource(struct devdrv_info *dev_info);

#endif /* DEVDRV_PCI_INFO_H */
