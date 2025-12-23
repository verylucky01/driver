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

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/printk.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/notifier.h>
#include <linux/pci.h>
#include <linux/module.h>

#include "adapter_api.h"
#include "adpater_def.h"

#ifdef CFG_HOST_ENV
#define PCIE_KO_NAME "drv_pcie_host"
#else
#define PCIE_KO_NAME "drv_pcie"
#endif
#define UB_KO_NAME "ascend_ub_drv"

#define PCI_VENDOR_ID_HUAWEI 0x19e5
#define DEVDRV_DIVERSITY_PCIE_VENDOR_ID 0xFFFF

struct bus_adpater_stu g_pcie_adpat = {0};
struct bus_adpater_stu g_ubbus_adpat = {0};

struct symbol_list pcie_ops[] = {
/* dma ops */
{"hal_kernel_devdrv_dma_alloc_coherent", (offsetof(struct bus_adpater_stu, dma)\
    + offsetof(struct dma_ops_stu, alloc_coherent))},
{"hal_kernel_devdrv_dma_free_coherent", (offsetof(struct bus_adpater_stu, dma)\
    + offsetof(struct dma_ops_stu, free_coherent))},
{"devdrv_dma_link_free", (offsetof(struct bus_adpater_stu, dma)\
    + offsetof(struct dma_ops_stu, link_free))},
{"devdrv_dma_link_prepare", (offsetof(struct bus_adpater_stu, dma)\
    + offsetof(struct dma_ops_stu, link_prepare))},
{"hal_kernel_devdrv_dma_map_single", (offsetof(struct bus_adpater_stu, dma)\
    + offsetof(struct dma_ops_stu, map_single))},
{"hal_kernel_devdrv_dma_sync_copy", (offsetof(struct bus_adpater_stu, dma)\
    + offsetof(struct dma_ops_stu, sync_copy))},
{"hal_kernel_devdrv_dma_unmap_single", (offsetof(struct bus_adpater_stu, dma)\
    + offsetof(struct dma_ops_stu, unmap_single))},
/* p2p ops */
{"devdrv_enable_p2p", (offsetof(struct bus_adpater_stu, p2p)\
    + offsetof(struct p2p_ops_stu, enable))},
{"devdrv_disable_p2p", (offsetof(struct bus_adpater_stu, p2p)\
    + offsetof(struct p2p_ops_stu, disable))},
{"devdrv_is_p2p_enabled", (offsetof(struct bus_adpater_stu, p2p)\
    + offsetof(struct p2p_ops_stu, is_enabled))},
{"devdrv_flush_p2p", (offsetof(struct bus_adpater_stu, p2p)\
    + offsetof(struct p2p_ops_stu, flush))},
{"devdrv_get_p2p_capability", (offsetof(struct bus_adpater_stu, p2p)\
    + offsetof(struct p2p_ops_stu, getcapability))},
{"devdrv_get_p2p_access_status", (offsetof(struct bus_adpater_stu, p2p)\
    + offsetof(struct p2p_ops_stu, get_access_status))},

/* hccs ops */
{"devdrv_get_hccs_link_status_and_group_id", (offsetof(struct bus_adpater_stu, hccs)\
    + offsetof(struct hccs_ops_stu, get_hccs_link_status_and_group_id))},

/* pcie ops */
{"devdrv_get_pci_dev_info", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, get_pci_dev_info))},
{"devdrv_get_pcie_id_info", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, get_pcie_id_info))},
{"devdrv_get_dev_topology", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, get_dev_topology))},
{"devdrv_hot_reset_device", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, hot_reset_device))},
{"devdrv_hot_pre_reset", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, prereset))},
{"devdrv_pcie_read_proc", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, read_proc))},
{"devdrv_get_addr_info", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, get_addr_info))},
{"devdrv_get_bbox_reservd_mem", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, get_bbox_reservd_mem))},
{"devdrv_register_black_callback", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, register_black_callback))},
{"devdrv_unregister_black_callback", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, unregister_black_callback))},
{"devdrv_dev_state_notifier_unregister", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, notifier_unregister))},
{"drvdrv_dev_state_notifier_register", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, notifier_register))},
{"devdrv_set_module_init_finish", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, set_module_init_finish))},
{"drvdrv_dev_startup_register", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, startup_register))},
{"devdrv_get_host_type", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, get_host_type))},
{"devdrv_get_master_devid_in_the_same_os", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, get_master_devid_in_the_same_os))},
{"devdrv_pcie_reinit", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, reinit))},
{"devdrv_get_pci_enabled_vf_num", (offsetof(struct bus_adpater_stu, pcie)\
    + offsetof(struct pcie_ops_stu, get_pci_enabled_vf_num))},
};

struct symbol_list ubbus_ops[] = {
/* dma ops */
{"ubdrv_flush_p2p", (offsetof(struct bus_adpater_stu, p2p)\
    + offsetof(struct p2p_ops_stu, flush))},
};


struct module_adapter_cb adap_cb[] = {
{PCIE_KO_NAME, pcie_ops, (sizeof(pcie_ops)/sizeof(struct symbol_list)), &g_pcie_adpat},
{UB_KO_NAME, ubbus_ops, (sizeof(ubbus_ops)/sizeof(struct symbol_list)), &g_ubbus_adpat},
};

static const struct pci_device_id devdrv_driver_tbl[] = {
    { PCI_VDEVICE(HUAWEI, 0xd806), 0 },
    { PCI_VDEVICE(HUAWEI, 0xd807), 0 },
    { DEVDRV_DIVERSITY_PCIE_VENDOR_ID, 0xd500, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0 },
    {}};
MODULE_DEVICE_TABLE(pci, devdrv_driver_tbl);

struct bus_adpater_stu *get_adapter_by_dev_id(unsigned int dev_id)
{
    struct bus_adpater_stu *adap[] = {&g_pcie_adpat, &g_ubbus_adpat};
    int connect_type = CONNECT_PROTOCOL_UNKNOWN;

#if (defined CFG_HOST_ENV) && (!defined CFG_FEATURE_ASCEND910_96_STUB)
#ifdef CFG_FEATURE_UB
    connect_type = CONNECT_PROTOCOL_UB;
#else
    connect_type = CONNECT_PROTOCOL_PCIE;
#endif
#else
    connect_type = devdrv_get_connect_protocol(dev_id);
#endif
    if ((connect_type == CONNECT_PROTOCOL_PCIE) || (connect_type == CONNECT_PROTOCOL_HCCS)) {
        down_read(&adap[ADAPTER_PCIE_BUS]->rw_lock);
        return adap[ADAPTER_PCIE_BUS];
    } else {
        down_read(&adap[ADAPTER_UB_BUS]->rw_lock);
        return adap[ADAPTER_UB_BUS];
    }
}

struct bus_adpater_stu *get_adapter_by_module(unsigned int index)
{
    struct bus_adpater_stu *adap[] = {&g_pcie_adpat, &g_ubbus_adpat};
    if (index > ADAPTER_UB_BUS) {
        down_read(&adap[0]->rw_lock);
        return adap[0];
    }
    down_read(&adap[index]->rw_lock);
    return adap[index];
}

void put_adapter(struct bus_adpater_stu *adap)
{
    up_read(&adap->rw_lock);
}

static void init_module_function(struct module *mod)
{
    int i;
    for (i=0; i < (sizeof(adap_cb)/sizeof(struct module_adapter_cb)); i++) {
        if (strcmp(mod->name, adap_cb[i].mod_name) != 0) {
            continue;
        }
        init_module_func(mod, adap_cb[i].sym_list, adap_cb[i].sym_count, adap_cb[i].adap);
    }
}

static void uninit_module_function(struct module *mod)
{
    int i;
    for (i=0; i < (sizeof(adap_cb)/sizeof(struct module_adapter_cb)); i++) {
        if (strcmp(mod->name, adap_cb[i].mod_name) != 0) {
            continue;
        }
        uninit_module_func(adap_cb[i].sym_list, adap_cb[i].sym_count, adap_cb[i].adap);
    }
}

static int adapter_module_callback(struct notifier_block *nb,
    unsigned long val, void *data)
{
    struct module *mod = data;
    if (mod == NULL) {
        return NOTIFY_DONE;
    }
    switch (val) {
        case MODULE_STATE_GOING:
            uninit_module_function(mod);
            break;
        case MODULE_STATE_LIVE:
            init_module_function(mod);
            break;
        default:
            break;
    }
    return NOTIFY_DONE;
}
static struct notifier_block g_adapter_module_nb = {
    .notifier_call = adapter_module_callback,
    .priority = 0
};

int __init ascend_adapter_init(void)
{
    int err, i;
    for (i = 0; i < (sizeof(adap_cb)/sizeof(struct module_adapter_cb)); i++) {
        init_rwsem(&adap_cb[i].adap->rw_lock);
    }
    err = register_module_notifier(&g_adapter_module_nb);
    return err;
}

void __exit ascend_adapter_exit(void)
{
    unregister_module_notifier(&g_adapter_module_nb);
    return;
}

module_init(ascend_adapter_init);
module_exit(ascend_adapter_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
MODULE_DESCRIPTION("DAVINCI DMS Manager driver");
MODULE_SOFTDEP("pre: asdrv_pbl post: drv_pcie_host");