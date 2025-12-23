/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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

#ifndef PCI_DEV_H
#define PCI_DEV_H
#include "ioctl_comm_def.h"

extern FaultEventNodeTable g_table[CAPACITY];
extern struct pci_dev_info *g_tcpci_info;
extern FaultEventNodeTable *g_kernel_event_table;
void update_mem_by_map(void);
int initTable(void);
#endif
