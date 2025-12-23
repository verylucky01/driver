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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>
#include "ts_agent_log.h"
#include "ts_agnet_ccpu.h"
#include "trs_adapt.h"

#define PCI_VENDOR_ID_HUAWEI 0x19e5
#define DEVDRV_DIVERSITY_PCIE_VENDOR_ID 0xFFFF
static const struct pci_device_id ts_agent_adapt_tbl[] = {
    { PCI_VDEVICE(HUAWEI, 0xd105),           0 },
    { PCI_VDEVICE(HUAWEI, 0xd802),           0 },
    { PCI_VDEVICE(HUAWEI, 0xd803),           0 },
    { PCI_VDEVICE(HUAWEI, 0xd804),           0 },
    { PCI_VDEVICE(HUAWEI, 0xd806),           0 },   // david pci vdevice id
    { DEVDRV_DIVERSITY_PCIE_VENDOR_ID, 0xd500, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0 },
    {}
};
MODULE_DEVICE_TABLE(pci, ts_agent_adapt_tbl);

STATIC int __init ts_agent_init(void)
{
    struct trs_sqcq_agent_ops ops = {
        .device_init = NULL,
        .device_uninit = NULL,
        .sqe_update = tsagent_sqe_update,
        .cqe_update = NULL,
        .mb_update = NULL,
        .sqe_update_src_check = tsagent_sqe_update_src_check,
    };

    init_task_convert_func();
    trs_sqcq_agent_ops_register(&ops);
    ts_agent_info("ts_agent_init end.");
    return 0;
}

STATIC void __exit ts_agent_exit(void)
{
    ts_agent_info("ts_agent_exit begin.");
    trs_sqcq_agent_ops_unregister();
    ts_agent_info("ts_agent_exit end.");
}

module_init(ts_agent_init);
module_exit(ts_agent_exit);

MODULE_LICENSE("GPL v2");
MODULE_INFO(supported, "ts agent");
MODULE_VERSION("v0.1");
MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
