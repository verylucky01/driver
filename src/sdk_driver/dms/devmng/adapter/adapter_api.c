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

#include "adapter_api.h"
#include "adpater_def.h"

void adap_dev_state_notifier_register(devdrv_dev_state_notify state_callback)
{
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(0); /* use device 0 get connect type */
    if (adap->pcie.notifier_register != NULL) {
        adap->pcie.notifier_register(state_callback);
    }
    put_adapter(adap);
}
EXPORT_SYMBOL(adap_dev_state_notifier_register);

void adap_dev_state_notifier_unregister(void)
{
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(0); /* use device 0 get connect type */
    if (adap->pcie.notifier_unregister != NULL) {
        adap->pcie.notifier_unregister();
    }
    put_adapter(adap);
}
EXPORT_SYMBOL(adap_dev_state_notifier_unregister);

/* p2p */
int adap_enable_p2p(int pid, u32 dev_id, u32 peer_dev_id)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(dev_id); /* use device 0 get connect type */
    if (adap->p2p.enable != NULL) {
        ret = adap->p2p.enable(pid, dev_id, peer_dev_id);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_enable_p2p);

int adap_disable_p2p(int pid, u32 dev_id, u32 peer_dev_id)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(dev_id); /* use device 0 get connect type */
    if (adap->p2p.disable != NULL) {
        ret = adap->p2p.disable(pid, dev_id, peer_dev_id);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_disable_p2p);

bool adap_is_p2p_enabled(u32 dev_id, u32 peer_dev_id)
{
    bool ret = false;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(dev_id); /* use device 0 get connect type */
    if (adap->p2p.is_enabled != NULL) {
        ret = adap->p2p.is_enabled(dev_id, peer_dev_id);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_is_p2p_enabled);

void adap_flush_p2p(int pid)
{
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(0); /* use device 0 get connect type */
    if (adap->p2p.flush != NULL) {
        adap->p2p.flush(pid);
    }
    put_adapter(adap);
}
EXPORT_SYMBOL(adap_flush_p2p);

int adap_get_p2p_capability(u32 dev_id, u64 *capability)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(dev_id); /* use device 0 get connect type */
    if (adap->p2p.getcapability != NULL) {
        ret = adap->p2p.getcapability(dev_id, capability);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_get_p2p_capability);

int adap_get_p2p_access_status(u32 devid, u32 peer_devid, int *status)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(devid); /* use device 0 get connect type */
    if (adap->p2p.get_access_status != NULL) {
        ret = adap->p2p.get_access_status(devid, peer_devid, status);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_get_p2p_access_status);

/* dma */
void *adap_dma_alloc_coherent(struct device *dev, size_t size, dma_addr_t *dma_addr, gfp_t gfp)
{
    void *addr = NULL;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(0); /* use device 0 get connect type */
    if (adap->dma.alloc_coherent != NULL) {
        addr = adap->dma.alloc_coherent(dev, size, dma_addr, gfp);
    }
    put_adapter(adap);
    return addr;
}
EXPORT_SYMBOL(adap_dma_alloc_coherent);

void adap_dma_free_coherent(struct device *dev, size_t size, void *addr, dma_addr_t dma_addr)
{
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(0); /* use device 0 get connect type */
    if (adap->dma.free_coherent != NULL) {
        adap->dma.free_coherent(dev, size, addr, dma_addr);
    }
    put_adapter(adap);
}
EXPORT_SYMBOL(adap_dma_free_coherent);

dma_addr_t adap_dma_map_single(struct device *dev, void *ptr, size_t size, enum dma_data_direction dir)
{
    dma_addr_t ret = 0;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(0); /* use device 0 get connect type */
    if (adap->dma.map_single != NULL) {
        ret = adap->dma.map_single(dev, ptr, size, dir);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_dma_map_single);

void adap_dma_unmap_single(struct device *dev, dma_addr_t addr, size_t size, enum dma_data_direction dir)
{
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(0); /* use device 0 get connect type */
    if (adap->dma.unmap_single != NULL) {
        adap->dma.unmap_single(dev, addr, size, dir);
    }
    put_adapter(adap);
}
EXPORT_SYMBOL(adap_dma_unmap_single);

int adap_dma_link_free(struct devdrv_dma_prepare *dma_prepare)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(0); /* use device 0 get connect type */
    if (adap->dma.link_free != NULL) {
        ret = adap->dma.link_free(dma_prepare);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_dma_link_free);

struct devdrv_dma_prepare *adap_dma_link_prepare(u32 devid, enum devdrv_dma_data_type type,
    struct devdrv_dma_node *dma_node, u32 node_cnt, u32 fill_status)
{
    struct devdrv_dma_prepare *prepare = NULL;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(devid); /* use device 0 get connect type */
    if (adap->dma.link_prepare != NULL) {
        prepare = adap->dma.link_prepare(devid, type, dma_node, node_cnt, fill_status);
    }
    put_adapter(adap);
    return prepare;
}
EXPORT_SYMBOL(adap_dma_link_prepare);

int adap_dma_sync_copy(u32 dev_id, enum devdrv_dma_data_type type, u64 src, u64 dst, u32 size,
    enum devdrv_dma_direction direction)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(dev_id); /* use device 0 get connect type */
    if (adap->dma.sync_copy != NULL) {
        ret = adap->dma.sync_copy(dev_id, type, src, dst, size, direction);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_dma_sync_copy);

/* pcie */
int adap_get_pci_dev_info(u32 devid, struct devdrv_pci_dev_info *dev_info)
{
    int ret = 0;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(devid);
    if (adap->pcie.get_pci_dev_info != NULL) {
        ret = adap->pcie.get_pci_dev_info(devid, dev_info);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_get_pci_dev_info);

int adap_get_pcie_id_info(u32 devid, struct devdrv_pcie_id_info  *pcie_id_info)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(devid);
    if (adap->pcie.get_pcie_id_info != NULL) {
        ret = adap->pcie.get_pcie_id_info(devid, pcie_id_info);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_get_pcie_id_info);

int adap_get_dev_topology(u32 devid, u32 peer_devid, int *topo_type)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(devid);
    if (adap->pcie.get_dev_topology != NULL) {
        ret = adap->pcie.get_dev_topology(devid, peer_devid, topo_type);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_get_dev_topology);

int adap_hot_reset_device(u32 dev_id)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(dev_id);
    if (adap->pcie.hot_reset_device != NULL) {
        ret = adap->pcie.hot_reset_device(dev_id);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_hot_reset_device);

int adap_pcie_read_proc(u32 dev_id, enum devdrv_addr_type type, u32 offset, unsigned char *value, u32 len)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(dev_id);
    if (adap->pcie.read_proc != NULL) {
        ret = adap->pcie.read_proc(dev_id, type, offset, value, len);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_pcie_read_proc);

int adap_pcie_prereset(u32 dev_id)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(dev_id);
    if (adap->pcie.prereset != NULL) {
        ret = adap->pcie.prereset(dev_id);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_pcie_prereset);

int adap_pcie_reinit(u32 dev_id)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(dev_id);
    if (adap->pcie.reinit != NULL) {
        ret = adap->pcie.reinit(dev_id);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_pcie_reinit);

int adap_get_addr_info(u32 devid, enum devdrv_addr_type type, u32 index, u64 *addr, size_t *size)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(devid);
    if (adap->pcie.get_addr_info != NULL) {
        ret = adap->pcie.get_addr_info(devid, type, index, addr, size);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_get_addr_info);

int adap_get_bbox_reservd_mem(unsigned int devid, unsigned long long *dma_addr, struct page **dma_pages,
    unsigned int *len)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(devid);
    if (adap->pcie.get_bbox_reservd_mem != NULL) {
        ret = adap->pcie.get_bbox_reservd_mem(devid, dma_addr, dma_pages, len);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_get_bbox_reservd_mem);
                                
int adap_get_hccs_link_status_and_group_id(u32 devid, u32 *hccs_status, u32 hccs_group_id[], u32 group_id_num)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(devid);
    if (adap->hccs.get_hccs_link_status_and_group_id != NULL) {
        ret = adap->hccs.get_hccs_link_status_and_group_id(devid, hccs_status,
            hccs_group_id, group_id_num);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_get_hccs_link_status_and_group_id);

int adap_register_black_callback(struct devdrv_black_callback *black_callback)
{
    int ret = 0;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(0); /* use device 0 get connect type */
    if (adap->pcie.register_black_callback != NULL) {
        ret = adap->pcie.register_black_callback(black_callback);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_register_black_callback);

void adap_unregister_black_callback(const struct devdrv_black_callback *black_callback)
{
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(0); /* use device 0 get connect type */
    if (adap->pcie.unregister_black_callback != NULL) {
        adap->pcie.unregister_black_callback(black_callback);
    }
    put_adapter(adap);
}
EXPORT_SYMBOL(adap_unregister_black_callback);

int adap_set_module_init_finish(int dev_id, int module)
{
    int ret = 0;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(dev_id); /* use device 0 get connect type */
    if (adap->pcie.set_module_init_finish != NULL) {
        ret = adap->pcie.set_module_init_finish(dev_id, module);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_set_module_init_finish);

void adap_dev_startup_register(devdrv_dev_startup_notify startup_callback)
{
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(0); /* use device 0 get connect type */
    if (adap->pcie.startup_register != NULL) {
        adap->pcie.startup_register(startup_callback);
    }
    put_adapter(adap);
}
EXPORT_SYMBOL(adap_dev_startup_register);

unsigned int adap_get_host_type(void)
{
    unsigned int ret = 0;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(0); /* use device 0 get connect type */
    if (adap->pcie.get_host_type != NULL) {
        ret = adap->pcie.get_host_type();
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_get_host_type);

int adap_get_master_devid_in_the_same_os(u32 dev_id, u32 *master_dev_id)
{
    int ret = 0;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(dev_id);
    if (adap->pcie.get_master_devid_in_the_same_os != NULL) {
        ret = adap->pcie.get_master_devid_in_the_same_os(dev_id, master_dev_id);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_get_master_devid_in_the_same_os);

int adap_get_pci_enabled_vf_num(u32 dev_id, int *vf_num)
{
    int ret = -EOPNOTSUPP;
    struct bus_adpater_stu *adap = get_adapter_by_dev_id(dev_id);
    if (adap->pcie.get_pci_enabled_vf_num != NULL) {
        ret = adap->pcie.get_pci_enabled_vf_num(dev_id, vf_num);
    }
    put_adapter(adap);
    return ret;
}
EXPORT_SYMBOL(adap_get_pci_enabled_vf_num);