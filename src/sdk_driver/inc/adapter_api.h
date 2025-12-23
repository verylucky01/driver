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
#ifndef __ASCEND_ADAPTER_API_H__
#define __ASCEND_ADAPTER_API_H__
#ifndef CFG_FEATURE_USE_ADPATER_FUNC

#define adap_dev_state_notifier_register drvdrv_dev_state_notifier_register

#define adap_dev_state_notifier_unregister devdrv_dev_state_notifier_unregister

/* p2p */

#define adap_enable_p2p devdrv_enable_p2p

#define adap_disable_p2p devdrv_disable_p2p

#define adap_is_p2p_enabled devdrv_is_p2p_enabled

#define adap_flush_p2p devdrv_flush_p2p

#define adap_get_p2p_capability devdrv_get_p2p_capability

#define adap_get_p2p_access_status devdrv_get_p2p_access_status

/* dma */
#define adap_dma_alloc_coherent hal_kernel_devdrv_dma_alloc_coherent

#define adap_dma_free_coherent hal_kernel_devdrv_dma_free_coherent

#define adap_dma_map_single hal_kernel_devdrv_dma_map_single

#define adap_dma_unmap_single hal_kernel_devdrv_dma_unmap_single

#define adap_dma_link_free devdrv_dma_link_free

#define adap_dma_link_prepare devdrv_dma_link_prepare

#define adap_dma_sync_copy hal_kernel_devdrv_dma_sync_copy

/* pcie */
#define adap_get_pci_dev_info devdrv_get_pci_dev_info

#define adap_get_pcie_id_info devdrv_get_pcie_id_info

#define adap_get_dev_topology devdrv_get_dev_topology

#define adap_hot_reset_device devdrv_hot_reset_device

#define adap_pcie_read_proc devdrv_pcie_read_proc

#define adap_pcie_prereset devdrv_hot_pre_reset

#define adap_get_addr_info devdrv_get_addr_info

#define adap_get_bbox_reservd_mem devdrv_get_bbox_reservd_mem

#define adap_get_hccs_link_status_and_group_id devdrv_get_hccs_link_status_and_group_id

#define adap_register_black_callback devdrv_register_black_callback

#define adap_unregister_black_callback devdrv_unregister_black_callback

#define adap_set_module_init_finish devdrv_set_module_init_finish

#define adap_dev_startup_register drvdrv_dev_startup_register

#define adap_get_host_type devdrv_get_host_type

#define adap_get_master_devid_in_the_same_os  devdrv_get_master_devid_in_the_same_os

#define adap_pcie_reinit devdrv_pcie_reinit

#define adap_get_pci_enabled_vf_num devdrv_get_pci_enabled_vf_num
#else
#include "comm_kernel_interface.h"
void adap_dev_state_notifier_register(devdrv_dev_state_notify state_callback);
void adap_dev_state_notifier_unregister(void);
/* p2p */
int adap_enable_p2p(int pid, u32 dev_id, u32 peer_dev_id);
int adap_disable_p2p(int pid, u32 dev_id, u32 peer_dev_id);
bool adap_is_p2p_enabled(u32 dev_id, u32 peer_dev_id);
void adap_flush_p2p(int pid);
int adap_get_p2p_capability(u32 dev_id, u64 *capability);
int adap_get_p2p_access_status(u32 devid, u32 peer_devid, int *status);

/* dma */
void *adap_dma_alloc_coherent(struct device *dev, size_t size, dma_addr_t *dma_addr, gfp_t gfp);
void adap_dma_free_coherent(struct device *dev, size_t size, void *addr, dma_addr_t dma_addr);
dma_addr_t adap_dma_map_single(struct device *dev, void *ptr, size_t size, enum dma_data_direction dir);
void adap_dma_unmap_single(struct device *dev, dma_addr_t addr, size_t size, enum dma_data_direction dir);
dma_addr_t adap_dma_map_page(struct device *dev, struct page *page,
    size_t offset, size_t size, enum dma_data_direction dir);
void adap_dma_unmap_page(struct device *dev, dma_addr_t addr, size_t size, enum dma_data_direction dir);
int adap_dma_link_free(struct devdrv_dma_prepare *dma_prepare);
struct devdrv_dma_prepare *adap_dma_link_prepare(u32 devid, enum devdrv_dma_data_type type,
    struct devdrv_dma_node *dma_node, u32 node_cnt, u32 fill_status);
int adap_dma_sync_copy(u32 dev_id, enum devdrv_dma_data_type type, u64 src, u64 dst, u32 size,
    enum devdrv_dma_direction direction);

/* pcie */
int adap_get_pci_dev_info(u32 devid, struct devdrv_pci_dev_info *dev_info);
int adap_get_pcie_id_info(u32 devid, struct devdrv_pcie_id_info  *pcie_id_info);
int adap_get_dev_topology(u32 devid, u32 peer_devid, int *topo_type);
int adap_hot_reset_device(u32 dev_id);
int adap_pcie_read_proc(u32 dev_id, enum devdrv_addr_type type, u32 offset, unsigned char *value, u32 len);
int adap_pcie_prereset(u32 dev_id);
int adap_get_addr_info(u32 devid, enum devdrv_addr_type type, u32 index, u64 *addr, size_t *size);
int adap_get_bbox_reservd_mem(unsigned int devid, unsigned long long *dma_addr,
    struct page **dma_pages, unsigned int *len);
int adap_get_hccs_link_status_and_group_id(u32 devid, u32 *hccs_status, u32 hccs_group_id[], u32 group_id_num);
int adap_register_black_callback(struct devdrv_black_callback *black_callback);
void adap_unregister_black_callback(const struct devdrv_black_callback *black_callback);
int adap_set_module_init_finish(int dev_id, int module);
void adap_dev_startup_register(devdrv_dev_startup_notify startup_callback);
unsigned int adap_get_host_type(void);
int adap_get_master_devid_in_the_same_os(u32 dev_id, u32 *master_dev_id);
int adap_pcie_reinit(u32 dev_id);
int adap_get_pci_enabled_vf_num(u32 dev_id, int *vf_num);
#endif

#endif

