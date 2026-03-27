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
#ifndef KA_PCI_PUB_H
#define KA_PCI_PUB_H

#include <linux/fs.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/pci.h>
#include <linux/version.h>
#include <linux/iommu.h>
#include <linux/pm_runtime.h>

#include "ka_common_pub.h"
#include "ka_task_pub.h"

#define ka_pci_power_t      pci_power_t
#define KA_PCI_D0            PCI_D0
#define KA_PCI_D1            PCI_D1
#define KA_PCI_D2            PCI_D2
#define KA_PCI_D3hot         PCI_D3hot
#define KA_PCI_D3cold        PCI_D3cold
#define KA_PCI_UNKNOWN       PCI_UNKNOWN
#define KA_PCI_POWER_ERROR   PCI_POWER_ERROR
#define KA_PCI_SET_RUNTIME_PM_OPS SET_RUNTIME_PM_OPS
#define KA_PCI_SET_SYSTEM_SLEEP_PM_OPS SET_SYSTEM_SLEEP_PM_OPS

typedef struct bus_type ka_bus_type_t;
typedef struct msix_entry ka_msix_entry_t;
typedef struct irq_affinity ka_irq_affinity_t;
typedef struct pci_bus ka_pci_bus_t;
typedef struct pci_device_id ka_pci_device_id_t;
typedef struct pci_driver ka_pci_driver_t;
typedef struct pci_error_handlers ka_pci_error_handlers_t;
typedef struct dev_iommu ka_dev_iommu_t;
typedef struct iommu_fwspec ka_iommu_fwspec_t;
typedef struct iommu_domain ka_iommu_domain_t;
typedef struct dev_pm_ops ka_dev_pm_ops_t;
#define KA_IOMMU_DOMAIN_IDENTITY IOMMU_DOMAIN_IDENTITY
#define ka_pci_domain_nr pci_domain_nr
#define KA_PCI_DEVFN PCI_DEVFN
#define KA_PCI_ERS_RESULT_CAN_RECOVER   PCI_ERS_RESULT_CAN_RECOVER
#define KA_PCI_ERS_RESULT_NEED_RESET    PCI_ERS_RESULT_NEED_RESET
#define KA_PCI_ERS_RESULT_RECOVERED     PCI_ERS_RESULT_RECOVERED
#define KA_PCI_ERS_RESULT_DISCONNECT    PCI_ERS_RESULT_DISCONNECT
#define KA_PCI_EXP_LNKSTA_CLS           PCI_EXP_LNKSTA_CLS
#define KA_PCI_EXP_LNKSTA_NLW           PCI_EXP_LNKSTA_NLW
#define KA_PCI_EXP_LNKSTA_NLW_SHIFT     PCI_EXP_LNKSTA_NLW_SHIFT
#define KA_PCI_CAP_ID_EXP               PCI_CAP_ID_EXP
#define KA_PCI_EXP_LNKSTA               PCI_EXP_LNKSTA
#define KA_PCI_EXP_LNKCTL               PCI_EXP_LNKCTL
#define KA_PCI_EXP_LNKCTL_LD            PCI_EXP_LNKCTL_LD
#define KA_PCI_CAP_ID_VNDR              PCI_CAP_ID_VNDR
#define KA_PCI_EXP_LNKSTA_DLLLA         PCI_EXP_LNKSTA_DLLLA

#define ka_pci_channel_io_normal        pci_channel_io_normal
#define ka_pci_channel_io_frozen        pci_channel_io_frozen
#define ka_pci_channel_io_perm_failure  pci_channel_io_perm_failure
#define ka_pci_channel_state_t          pci_channel_state_t
#define ka_pci_ers_result_t             pci_ers_result_t

#define ka_pci_disable_msi(dev)    pci_disable_msi(dev)
#define ka_pci_disable_msix(dev)    pci_disable_msix(dev)
#define ka_pci_get_domain_bus_and_slot(domain, bus, devfn)    pci_get_domain_bus_and_slot(domain, bus, devfn)
#define ka_pci_get_device(vendor, device, from)    pci_get_device(vendor, device, from)
#define ka_pci_find_capability(dev, cap)    pci_find_capability(dev, cap)
#define ka_pci_bus_find_capability(bus, devfn, cap)    pci_bus_find_capability(bus, devfn, cap)
#define ka_pci_set_power_state(dev, state)    pci_set_power_state(dev, state)
#define ka_pci_save_state(dev)    pci_save_state(dev)
#define ka_pci_restore_state(dev)    pci_restore_state(dev)
#define ka_pci_enable_device_io(dev)    pci_enable_device_io(dev)
#define ka_pci_enable_device_mem(dev)    pci_enable_device_mem(dev)
#define ka_pci_enable_device(dev)    pci_enable_device(dev)
#define ka_pci_disable_device(dev)    pci_disable_device(dev)
#define ka_pci_request_region(pdev, bar, res_name)    pci_request_region(pdev, bar, res_name)
#define ka_pci_request_selected_regions(pdev, bars, res_name)    pci_request_selected_regions(pdev, bars, res_name)
#define ka_pci_release_selected_regions(pdev, bars)    pci_release_selected_regions(pdev, bars)
#define ka_pci_request_selected_regions_exclusive(pdev, bars, res_name)   \
            pci_request_selected_regions_exclusive(pdev, bars, res_name)
#define ka_pci_release_regions(pdev)    pci_release_regions(pdev)
#define ka_pci_request_regions(pdev, res_name)    pci_request_regions(pdev, res_name)
#define ka_pci_request_regions_exclusive(pdev, res_name)    pci_request_regions_exclusive(pdev, res_name)
#define ka_pci_set_master(dev)    pci_set_master(dev)
#define ka_pci_clear_master(dev)    pci_clear_master(dev)
#define ka_pci_select_bars(dev, flags)    pci_select_bars(dev, flags)
#define ka_pci_match_id(ids, dev)    pci_match_id(ids, dev)
#define ka_pci_register_driver(drv)    pci_register_driver(drv)
#define ka_pci_unregister_driver(drv)    pci_unregister_driver(drv)
#define ka_pci_dev_put(dev)    pci_dev_put(dev)
#define ka_pci_bus_read_config_word(bus, devfn, where, val)    pci_bus_read_config_word(bus, devfn, where, val)
#define ka_pci_bus_read_config_dword(bus, devfn, where, val)    pci_bus_read_config_dword(bus, devfn, where, val)
#define ka_pci_read_config_byte(dev, where, val)    pci_read_config_byte(dev, where, val)
#define ka_pci_read_config_word(dev, where, val)    pci_read_config_word(dev, where, val)
#define ka_pci_read_config_dword(dev, where, val)    pci_read_config_dword(dev, where, val)
#define ka_pci_write_config_word(dev, where, val)    pci_write_config_word(dev, where, val)
#define ka_pci_write_config_dword(dev, where, val)    pci_write_config_dword(dev, where, val)
#define ka_pci_set_drvdata(pdev, data)    pci_set_drvdata(pdev, data)
#define ka_pci_get_drvdata(pdev)    pci_get_drvdata(pdev)
#define ka_pci_sriov_get_totalvfs(pdev)    pci_sriov_get_totalvfs(pdev)
#define ka_pci_to_pci_dev(dev)    to_pci_dev(dev)
#define ka_pci_alloc_irq_vectors_affinity(dev, min_vecs, max_vecs, flags, affd)  \
            pci_alloc_irq_vectors_affinity(dev, min_vecs, max_vecs, flags, affd)
#define ka_pci_free_irq_vectors(dev)    pci_free_irq_vectors(dev)
#define ka_pci_enable_msix_range(dev, entries, minvec, maxvec)    pci_enable_msix_range(dev, entries, minvec, maxvec)
#define ka_pci_resource_flags(dev, bar)    pci_resource_flags(dev, bar)
#define ka_pci_resource_len(dev,bar)    pci_resource_len(dev, bar)
#define ka_pci_resource_start(dev, bar)    pci_resource_start(dev, bar)
#define ka_pci_pm_runtime_get_sync(dev) pm_runtime_get_sync(dev)
#define ka_pci_pm_runtime_put(dev) pm_runtime_put(dev)
#define ka_pci_reset_function(dev) pci_reset_function(dev)
#define ka_pci_try_reset_function(dev) pci_try_reset_function(dev)
#define ka_pci_name(pdev) pci_name(pdev)
#define ka_pci_is_enabled(pdev) pci_is_enabled(pdev)
#define ka_pci_intx(dev, enable) pci_intx(dev, enable)
#define ka_pci_num_vf(dev) pci_num_vf(dev)
#define ka_pci_enable_sriov(dev, nr_virtfn) pci_enable_sriov(dev, nr_virtfn)
#define ka_pci_disable_sriov(dev) pci_disable_sriov(dev)
#define ka_pci_vfs_assigned(dev) pci_vfs_assigned(dev)
#define ka_pci_upstream_bridge(pdev) pci_upstream_bridge(pdev)

#define ka_pci_error_detected(pci_error_detected) \
    .error_detected = pci_error_detected,
#define ka_pci_slot_reset(pci_slot_reset) \
    .slot_reset = pci_slot_reset,
#define ka_pci_resume(pci_error_resume) \
    .resume = pci_error_resume,

ka_bus_type_t *ka_pci_get_bus_type(void);
unsigned char ka_pci_get_bus_number(ka_pci_dev_t *pdev);
#define KA_PCI_SLOT(devfn) PCI_SLOT(devfn)
#define KA_PCI_FUNC(devfn) PCI_FUNC(devfn)
unsigned short ka_pci_get_vendor_id(ka_pci_dev_t *pdev);
unsigned short ka_pci_get_device_id(ka_pci_dev_t *pdev);
unsigned short ka_pci_get_subsystem_vendor_id(ka_pci_dev_t *pdev);
unsigned short ka_pci_get_subsystem_device_id(ka_pci_dev_t *pdev);

void ka_pci_irq_vector(ka_pci_dev_t *pdev, u32 entry, u32 devid, u32 *irq,
                       int (*irq_vector_func)(u32, u32, unsigned int *));
void ka_pci_aer_clear_status(ka_pci_dev_t *pdev);
int ka_pci_enable_msi_intr(ka_pci_dev_t *pdev, int min, int max, int flags);
void ka_pci_disable_msi_intr(ka_pci_dev_t *pdev);
void ka_pci_stop_and_remove_bus_device_locked(ka_pci_dev_t *pdev, ka_mutex_t *remove_rescan_mutex);
unsigned int ka_pci_rescan_bus_locked(ka_pci_bus_t *bus, ka_mutex_t *remove_rescan_mutex);

unsigned int ka_pci_get_devfn(ka_pci_dev_t *pdev);
ka_pci_bus_t *ka_pci_get_bus(ka_pci_dev_t *pdev);
int ka_pci_get_bus_domain_nr(ka_pci_dev_t *pdev);
ka_pci_dev_t *ka_pci_get_bus_self(ka_pci_dev_t *pdev);
u8 ka_pci_get_revision(ka_pci_dev_t *pdev);
unsigned int ka_pci_get_is_physfn(ka_pci_dev_t *pdev);
unsigned int ka_pci_get_is_virtfn(ka_pci_dev_t *pdev);
ka_device_t *ka_pci_get_dev(ka_pci_dev_t *pdev);
ka_kobject_t *ka_pci_get_dev_kobj(ka_pci_dev_t *pdev);
unsigned short ka_pci_get_pdev_device(ka_pci_dev_t *pdev);
u16 ka_pci_get_aer_cap(ka_pci_dev_t *pdev);
u16 ka_pci_get_msix_entry(ka_msix_entry_t *msix_entry);
u32 ka_pci_get_msix_vector(ka_msix_entry_t *msix_entry);
void ka_pci_set_msix_entry(ka_msix_entry_t *msix_entry, u16 entry);
void ka_pci_set_msix_vector(ka_msix_entry_t *msix_entry, u32 vector);
int ka_pci_get_dev_iommu(ka_device_t *dev);
int ka_pci_get_dev_iommu_fwspec(ka_device_t *dev);
ka_iommu_domain_t *ka_pci_iommu_get_domain_for_dev(ka_device_t *dev);
u32 ka_pci_get_dev_iommu_fwspec_ids0(ka_device_t *dev);
typedef int (*bridge_func)(ka_pci_dev_t *);
bridge_func ka_pci_get_bridge_reset_func(void);
int ka_pci_enable_pcie_error_reporting(ka_pci_dev_t *dev);
int ka_pci_disable_pcie_error_reporting(ka_pci_dev_t *dev);
int ka_pci_configure_extended_capability(ka_pci_dev_t *dev);
#endif
