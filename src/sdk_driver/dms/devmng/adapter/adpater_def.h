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

#ifndef __ASCEND_ADPATER_DEF_H__
#define __ASCEND_ADPATER_DEF_H__

#include "ka_common_pub.h"
#include "ka_memory_pub.h"
#include "ka_base_pub.h"
#include "comm_kernel_interface.h"
#ifdef __cplusplus
extern "C" {
#endif

#define ADAPTER_PCIE_BUS 0
#define ADAPTER_UB_BUS 1

struct symbol_list {
    const char *name;
    unsigned long offset;
};

struct dma_ops_stu {
    void* (*alloc_coherent)(ka_device_t *dev, size_t size, ka_dma_addr_t *dma_addr, ka_gfp_t gfp);
    void (*free_coherent)(ka_device_t *dev, size_t size, void *addr, ka_dma_addr_t dma_addr);
    int (*link_free)(struct devdrv_dma_prepare *dma_prepare);
    struct devdrv_dma_prepare* (*link_prepare)(u32 devid, enum devdrv_dma_data_type type,
                struct devdrv_dma_node *dma_node, u32 node_cnt, u32 fill_status);
    ka_dma_addr_t (*map_single)(ka_device_t *dev, void *ptr, size_t size, ka_dma_data_direction_t dir);
    int (*sync_copy)(u32 dev_id, enum devdrv_dma_data_type type, u64 src, u64 dst, u32 size,
                enum devdrv_dma_direction direction);
    void (*unmap_single)(ka_device_t *dev, ka_dma_addr_t addr, size_t size, ka_dma_data_direction_t dir);
};

struct p2p_ops_stu {
    int (*enable)(int pid, u32 dev_id, u32 peer_dev_id);
    int (*disable)(int pid, u32 dev_id, u32 peer_dev_id);
    bool (*is_enabled)(u32 dev_id, u32 peer_dev_id);
    void (*flush)(int pid);
    int (*getcapability)(u32 dev_id, u64 *capability);
    int (*get_access_status)(u32 devid, u32 peer_devid, int *status);
};
struct hccs_ops_stu {
    int (*get_hccs_link_status_and_group_id)(u32 devid, u32 *hccs_status, u32 hccs_group_id[], u32 group_id_num);
};

struct pcie_ops_stu {
    int (*get_pci_dev_info)(u32 devid, struct devdrv_pci_dev_info *dev_info);
    int (*get_pcie_id_info)(u32 devid, struct devdrv_pcie_id_info *pcie_id_info);
    int (*get_dev_topology)(u32 devid, u32 peer_devid, int *topo_type);
    int (*hot_reset_device)(u32 dev_id);
    int (*prereset)(u32 dev_id);
    int (*read_proc)(u32 dev_id, enum devdrv_addr_type type, u32 offset, unsigned char *value, u32 len);
    int (*get_addr_info)(u32 devid, enum devdrv_addr_type type, u32 index, u64 *addr, size_t *size);
    int (*get_bbox_reservd_mem)(unsigned int devid, unsigned long long *dma_addr,
        ka_page_t **dma_pages, unsigned int *len);
    int (*register_black_callback)(struct devdrv_black_callback *black_callback);
    void (*unregister_black_callback)(const struct devdrv_black_callback *black_callback);
    void (*notifier_unregister)(void);
    void (*notifier_register)(devdrv_dev_state_notify state_callback);
    int (*set_module_init_finish)(int dev_id, int module);
    void (*startup_register)(devdrv_dev_startup_notify startup_callback);
    unsigned int (*get_host_type)(void);
    int (* get_master_devid_in_the_same_os)(u32 dev_id, u32 *master_dev_id);
    int (* reinit)(u32 dev_id);
    int (* get_pci_enabled_vf_num)(u32 dev_id, u32 *vf_num);
};
struct bus_adpater_stu {
    ka_rw_semaphore_t rw_lock;
    struct dma_ops_stu dma;
    struct hccs_ops_stu hccs;
    struct p2p_ops_stu p2p;
    struct pcie_ops_stu pcie;
};

struct module_adapter_cb {
    const char *mod_name;
    const struct symbol_list *sym_list;
    unsigned int sym_count;
    struct bus_adpater_stu *adap;
};

struct bus_adpater_stu *get_adapter_by_dev_id(unsigned int dev_id);
struct bus_adpater_stu *get_adapter_by_module(unsigned int index);
void put_adapter(struct bus_adpater_stu *adap);

void init_module_func(const ka_module_t *mod, const struct symbol_list *fun_name_list,
    unsigned int count, struct bus_adpater_stu *adapt);
void uninit_module_func(const struct symbol_list *fun_name_list, unsigned int count, struct bus_adpater_stu *adapt);

int ascend_adapter_init(void);
void ascend_adapter_exit(void);

#ifdef __cplusplus
}
#endif


#endif
