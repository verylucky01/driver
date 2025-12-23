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
#include <linux/pci.h>
#include <linux/module.h>

#include "pbl_module.h"

#ifdef CFG_ENV_HOST
#define PCI_VENDOR_ID_HUAWEI 0x19e5
#define DEVDRV_DIVERSITY_PCIE_VENDOR_ID 0xFFFF
#define PCI_DEVICE_CLOUD (0xa126U)

static const struct pci_device_id g_pbl_tbl[] = {
    { PCI_VDEVICE(HUAWEI, 0xd100),           0 },
    { PCI_VDEVICE(HUAWEI, 0xd105),           0 },
    { PCI_VDEVICE(HUAWEI, PCI_DEVICE_CLOUD), 0 },
    { PCI_VDEVICE(HUAWEI, 0xd801),           0 },
    { PCI_VDEVICE(HUAWEI, 0xd500),           0 },
    { PCI_VDEVICE(HUAWEI, 0xd501),           0 },
    { PCI_VDEVICE(HUAWEI, 0xd802),           0 },
    { PCI_VDEVICE(HUAWEI, 0xd803),           0 },
    { PCI_VDEVICE(HUAWEI, 0xd804),           0 },
    { PCI_VDEVICE(HUAWEI, 0xd805),           0 },
    { PCI_VDEVICE(HUAWEI, 0xd806),           0 },
    { PCI_VDEVICE(HUAWEI, 0xd807),           0 },
    { DEVDRV_DIVERSITY_PCIE_VENDOR_ID, 0xd500, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0 },
    {}
};
MODULE_DEVICE_TABLE(pci, g_pbl_tbl);
#endif

int __attribute__((weak)) ka_module_init(void)
{
    return 0;
}

void __attribute__((weak)) ka_module_exit(void)
{
    return;
}

int __attribute__((weak)) prof_framework_init(void)
{
    return 0;
}

void __attribute__((weak)) prof_framework_exit(void)
{
    return;
}

int __attribute__((weak)) ascend_ctl_init(void)
{
    return 0;
}

void __attribute__((weak)) ascend_ctl_exit(void)
{
    return;
}

struct submodule_ops {
    int (*init) (void);
    void (*uninit)(void);
};

static struct submodule_ops g_sub_table[] = {
#ifdef CFG_ENV_HOST
#ifdef CFG_FEATURE_VPBL
    {log_drv_module_init, log_drv_module_exit},
    {ka_module_init, ka_module_exit},
    {drv_ascend_intf_init, drv_davinci_intf_exit},
    {uda_init_module, uda_exit_module},
    {recfg_init, recfg_exit},
#else
    {log_drv_module_init, log_drv_module_exit},
    {ka_module_init, ka_module_exit},
    {drv_ascend_intf_init, drv_davinci_intf_exit},
    {uda_init_module, uda_exit_module},
    {resmng_init_module, resmng_exit_module},
    {recfg_init, recfg_exit},
    {urd_init, urd_exit},
#ifndef CFG_FEATURE_KO_ALONE_COMPILE
    {devdrv_base_comm_init, devdrv_base_comm_exit},
#endif
#endif
#elif defined PKICMS_UT_TEST
    {pkicms_dev_init, pkicms_dev_exit},
#else
    {ka_module_init, ka_module_exit},
    {drv_ascend_intf_init, drv_davinci_intf_exit},
    {uda_init_module, uda_exit_module},
    {resmng_init_module, resmng_exit_module},
    {bdcfg_init, bdcfg_exit},
    {recfg_init, recfg_exit},
    {ccfg_init, ccfg_exit},
    {prof_framework_init, prof_framework_exit},
#ifndef CFG_FEATURE_KO_ALONE_COMPILE
    {dev_user_cfg_module_init, dev_user_cfg_module_exit},
#endif
    {ascend_ctl_init, ascend_ctl_exit},
    {dfm_init, dfm_exit},
    {urd_init, urd_exit},
    {ipcdrv_pbl_init_module, ipcdrv_pbl_exit_module},
#ifndef CFG_FEATURE_KO_ALONE_COMPILE
    {icmdrv_pbl_init_module, icmdrv_pbl_exit_module},
    {pkicms_dev_init, pkicms_dev_exit},
    {devdrv_base_comm_init, devdrv_base_comm_exit},
#endif
#endif
};

STATIC int __init init_pbl_base(void)
{
    int index, ret;
    int table_size = sizeof(g_sub_table) / sizeof(struct submodule_ops);

    for (index = 0; index < table_size; index++) {
        ret = g_sub_table[index].init();
        if  (ret != 0) {
            goto out;
        }
    }
    return 0;
out:
    for (; index > 0; index--) {
        g_sub_table[index - 1].uninit();
    }
    return ret;
}

STATIC void __exit exit_pbl_base(void)
{
    int index;
    int table_size = sizeof(g_sub_table) / sizeof(struct submodule_ops);

    for (index = table_size; index > 0; index--) {
        g_sub_table[index - 1].uninit();
    }
}

module_init(init_pbl_base);
module_exit(exit_pbl_base);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
MODULE_DESCRIPTION("PBL BASE");