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

#ifndef __COMM_MSG_CHAN_H__
#define __COMM_MSG_CHAN_H__

#include <linux/interrupt.h>
#include <linux/mutex.h>
#include <linux/types.h>
#include <linux/device.h>
#include "pbl/pbl_soc_res_attr.h"
#include "addr_trans.h"

/* ******************** non trans for host and device ******************** */
/* **************************** host ************************************* */

/* host msg client type */
enum devdrv_msg_client_type {
    devdrv_msg_client_pcivnic = 0,
    devdrv_msg_client_pasid,
    devdrv_msg_client_devmm,
    devdrv_msg_client_common,
    devdrv_msg_client_devmanager,
    devdrv_msg_client_tsdrv,
    devdrv_msg_client_hdc,
    devdrv_msg_client_queue,
    devdrv_msg_client_s2s,
    devdrv_msg_client_tablesync,
    devdrv_msg_client_max
};

/* host non-trans chan info */
struct devdrv_non_trans_msg_chan_info {
    enum devdrv_msg_client_type msg_type;
    u32 flag; /* sync or async:bit0 */
    u32 level;
    u32 s_desc_size;
    u32 c_desc_size;
    int (*rx_msg_process)(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);
};

/* alloc non-trans msg chan */
void *devdrv_pcimsg_alloc_non_trans_queue(u32 dev_id, struct devdrv_non_trans_msg_chan_info *chan_info);
void *devdrv_pcimsg_alloc_non_trans_queue_inner_msg(u32 index_id, struct devdrv_non_trans_msg_chan_info *chan_info);
/* free non-trans msg chan */
int devdrv_pcimsg_free_non_trans_queue(void *msg_chan);
/* non-trans msg sync send */
int devdrv_sync_msg_send(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);

/* **************************** host **************************** */
/* device msg client type */
enum agentdrv_msg_client_type {
    agentdrv_msg_client_pcivnic = 0,
    agentdrv_msg_client_pasid,
    agentdrv_msg_client_devmm,
    agentdrv_msg_client_common,
    agentdrv_msg_client_devmanager,
    agentdrv_msg_client_tsdrv,
    agentdrv_msg_client_hdc,
    agentdrv_msg_client_queue,
    agentdrv_msg_client_s2s,
    agentdrv_msg_client_tablesync,
    agentdrv_msg_client_max
};

/* device non-trans msg client */
struct agentdrv_non_trans_msg_client {
    enum agentdrv_msg_client_type type;
    u32 flag;
    int (*init_non_trans_msg_chan)(void *msg_chan);
    void (*uninit_non_trans_msg_chan)(void *msg_chan);
    int (*non_trans_msg_process)(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);
    /* irq top half, can not sleep or time-consuming operation */
    int (*non_trans_msg_irq_notify)(void *msg_chan, void *data, u32 in_data_len);
};

/* register non-trans msg client */
int agentdrv_register_non_trans_msg_client(const struct agentdrv_non_trans_msg_client *msg_client);

/* unregister non-trans msg client */
int agentdrv_unregister_non_trans_msg_client(const struct agentdrv_non_trans_msg_client *msg_client);

/* non-trans msg sync send */
int agentdrv_sync_msg_send(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);

/* ********************* common for host and device ********************** */
/* *************************** device ************************************ */
/* device common msg */
enum agentdrv_common_msg_type {
    AGENTDRV_COMMON_MSG_PCIVNIC = 0,
    AGENTDRV_COMMON_MSG_SMMU,
    AGENTDRV_COMMON_MSG_DEVMM,
    AGENTDRV_COMMON_MSG_VMNG,
    AGENTDRV_COMMON_MSG_PROFILE = 4,
    AGENTDRV_COMMON_MSG_DEVDRV_MANAGER,
    AGENTDRV_COMMON_MSG_DEVDRV_TSDRV,
    AGENTDRV_COMMON_MSG_HDC,
    AGENTDRV_COMMON_MSG_SYSFS,
    AGENTDRV_COMMON_MSG_ESCHED,
    AGENTDRV_COMMON_MSG_DP_PROC_MNG,
    AGENTDRV_COMMON_MSG_TEST,
    AGENTDRV_COMMON_MSG_UDIS,
    AGENTDRV_COMMON_MSG_TYPE_MAX
};

struct agentdrv_common_msg_client {
    enum agentdrv_common_msg_type type;
    int (*common_msg_recv)(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);
    void (*init_notify)(u32 devid);
};

int agentdrv_register_common_msg_client(const struct agentdrv_common_msg_client *msg_client);
int agentdrv_unregister_common_msg_client(const struct agentdrv_common_msg_client *msg_client);
int agentdrv_common_msg_send(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len,
                             enum agentdrv_common_msg_type msg_type);

/* *************************** device ************************************ */
/* host common msg */
enum devdrv_common_msg_type {
    DEVDRV_COMMON_MSG_PCIVNIC = 0,
    DEVDRV_COMMON_MSG_SMMU,
    DEVDRV_COMMON_MSG_DEVMM,
    DEVDRV_COMMON_MSG_VMNG,
    DEVDRV_COMMON_MSG_PROFILE = 4,
    DEVDRV_COMMON_MSG_DEVDRV_MANAGER,
    DEVDRV_COMMON_MSG_DEVDRV_TSDRV,
    DEVDRV_COMMON_MSG_HDC,
    DEVDRV_COMMON_MSG_SYSFS,
    DEVDRV_COMMON_MSG_ESCHED,
    DEVDRV_COMMON_MSG_DP_PROC_MNG,
    DEVDRV_COMMON_MSG_TEST,
    DEVDRV_COMMON_MSG_UDIS,
    DEVDRV_COMMON_MSG_TYPE_MAX
};

struct devdrv_common_msg_client {
    enum devdrv_common_msg_type type;
    int (*common_msg_recv)(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);
    void (*init_notify)(u32 dev_id, int status);
};

int devdrv_register_common_msg_client(const struct devdrv_common_msg_client *msg_client);
int devdrv_unregister_common_msg_client(u32 devid, const struct devdrv_common_msg_client *msg_client);
int devdrv_common_msg_send(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len,
                           enum devdrv_common_msg_type msg_type);

/* ********************* dev_manager api ********************* */
struct devdrv_base_device_info {
    unsigned int venderid;
    unsigned int subvenderid;
    unsigned int deviceid;
    unsigned int subdeviceid;
    unsigned int bus;
    unsigned int device;
    unsigned int fn;
    int domain;
    unsigned int reserve[32];
};

enum devdrv_base_p2p_attr_op {
    DEVDRV_BASE_P2P_ADD = 0,
    DEVDRV_BASE_P2P_DEL,
    DEVDRV_BASE_P2P_QUERY,
    DEVDRV_BASE_P2P_ACCESS_STATUS_QUERY,
    DEVDRV_BASE_P2P_CAPABILITY_QUERY,
    DEVDRV_BASE_P2P_MAX
};
struct devdrv_base_comm_p2p_attr {
    u32 op;

    int pid;
    u32 devid;
    u32 peer_dev_id;

    u64 *capability;
    int *status;
};

/* device boot status */
enum dsmi_boot_status {
    DSMI_BOOT_STATUS_UNINIT = 0, /* device uninit */
    DSMI_BOOT_STATUS_BIOS,       /* device BIOS starting */
    DSMI_BOOT_STATUS_OS,         /* device OS starting */
    DSMI_BOOT_STATUS_FINISH      /* device boot finish */
};

int devdrv_get_device_boot_status(u32 devid, u32 *boot_status);
int devdrv_get_device_boot_status_inner(u32 index_id, u32 *boot_status);
#define CONNECT_PROTOCOL_PCIE 0
#define CONNECT_PROTOCOL_HCCS 1
#define CONNECT_PROTOCOL_UB 2
#define CONNECT_PROTOCOL_UNKNOWN 3
int devdrv_get_connect_protocol(u32 dev_id);
int agentdrv_get_msg_chan_devid(void *msg_chan);
int devdrv_get_msg_chan_devid(void *msg_chan);
int devdrv_get_msg_chan_devid_inner(void *msg_chan);
int agentdrv_set_msg_chan_priv(void *msg_chan, void *priv);
void *agentdrv_get_msg_chan_priv(void *msg_chan);
int devdrv_set_msg_chan_priv(void *msg_chan, void *priv);
void *devdrv_get_msg_chan_priv(void *msg_chan);

int devdrv_get_host_phy_mach_flag(u32 devid, u32 *host_flag);
int devdrv_get_env_boot_type(u32 dev_id);
int devdrv_get_env_boot_type_inner(u32 index_id);
int devdrv_get_device_info(u32 devid, struct devdrv_base_device_info *device_info);
int devdrv_p2p_attr_op(struct devdrv_base_comm_p2p_attr *attr);
int devdrv_get_pfvf_type_by_devid_inner(u32 index_id);
int devdrv_get_pfvf_type_by_devid(u32 dev_id);
bool devdrv_is_mdev_vm_boot_mode(u32 dev_id);
bool devdrv_is_mdev_vm_boot_mode_inner(u32 index_id);
int devdrv_get_connect_protocol_inner(u32 index_id);
bool devdrv_is_sriov_support(u32 dev_id);

int devdrv_sriov_enable(u32 dev_id, u32 boot_mode);
int devdrv_sriov_disable(u32 dev_id, u32 boot_mode);
int devdrv_get_ub_urma_info_by_udevid(u32 udevid, struct ascend_urma_dev_info *urma_info);
int devdrv_get_dev_topology(u32 devid, u32 peer_devid, int *topo_type);
struct device *devdrv_base_comm_get_device(u32 devid, u32 vfid, u32 udevid);

/* ********************* remote address operate(RAO) api ********************** */
enum devdrv_rao_client_type {
    DEVDRV_RAO_CLIENT_DEVMNG = 0,
    DEVDRV_RAO_CLIENT_BBOX_DDR,
    DEVDRV_RAO_CLIENT_BBOX_SRAM,
    DEVDRV_RAO_CLIENT_BBOX_KLOG,
    DEVDRV_RAO_CLIENT_BBOX_DEBUG_RUN_OS,
    DEVDRV_RAO_CLIENT_BBOX_SEC_OS,
    DEVDRV_RAO_CLIENT_BBOX_DEBUG_DEV,
    DEVDRV_RAO_CLIENT_BBOX_VMCORE,
    DEVDRV_RAO_CLIENT_TEST,
    DEVDRV_RAO_CLIENT_MAX
};

enum devdrv_rao_permission_type {
    DEVDRV_RAO_PERM_RMT_READ = 0,
    DEVDRV_RAO_PERM_RMT_WRITE,
    DEVDRV_RAO_PERM_RMT_RW,
    DEVDRV_RAO_PERM_MAX
};

int devdrv_register_rao_client(u32 dev_id, enum devdrv_rao_client_type type, u64 va, u64 len,
    enum devdrv_rao_permission_type perm);
int devdrv_unregister_rao_client(u32 dev_id, enum devdrv_rao_client_type type);
int devdrv_rao_read(u32 dev_id, enum devdrv_rao_client_type type, u64 offset, u64 len);
int devdrv_rao_write(u32 dev_id, enum devdrv_rao_client_type type, u64 offset, u64 len);

/* ********************* register communication api ********************** */
enum devdrv_communication_type {
    DEVDRV_COMMNS_PCIE = 0,
    DEVDRV_COMMNS_UB,
    DEVDRV_COMMNS_XLINK,
    DEVDRV_COMMNS_TYPE_MAX
};

enum devdrv_ops_status {
    DEVDRV_COMM_OPS_TYPE_UNINIT,
    DEVDRV_COMM_OPS_TYPE_INIT,
    DEVDRV_COMM_OPS_TYPE_ENABLE,
    DEVDRV_COMM_OPS_TYPE_DISABLE,
    DEVDRV_COMM_OPS_TYPE_MAX
};

#define MAX_EID_NUM_PER_DEV 8
struct devdrv_ub_dev_info {
    u32 eid_index[MAX_EID_NUM_PER_DEV];
    void *udma_dev[MAX_EID_NUM_PER_DEV];
};

struct devdrv_comm_ops {
    atomic_t ref_cnt;
    enum devdrv_communication_type comm_type;
#ifdef CFG_ENV_HOST
    void *(*alloc_non_trans)(u32 dev_id, struct devdrv_non_trans_msg_chan_info *chan_info);
    int (*free_non_trans)(void *msg_chan);
    int (*register_common_msg_client)(const struct devdrv_common_msg_client *msg_client);
    int (*unregister_common_msg_client)(u32 devid, const struct devdrv_common_msg_client *msg_client);
    int (*common_msg_send)(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len,
        enum devdrv_common_msg_type msg_type);
    int (*get_boot_status)(u32 dev_id, u32 *boot_status);
    int (*get_host_phy_mach_flag)(u32 devid, u32 *host_flag);
    int (*get_env_boot_type)(u32 dev_id);
    struct device *(*get_device)(u32 dev_id, u32 vfid, u32 udevid);
    int (*get_dev_topology)(u32 devid, u32 peer_devid, int *topo_type);
    int (*p2p_attr_op)(struct devdrv_base_comm_p2p_attr *attr);
    int (*get_device_info)(u32 devid, struct devdrv_base_device_info *device_info);
    int (*hotreset_assemble)(u32 dev_id);
    int (*prereset_assemble)(u32 dev_id);
    int (*rescan_atomic)(u32 dev_id);
    int (*unbind_atomic)(u32 dev_id);
    int (*reset_atomic)(u32 dev_id);    
    int (*remove_atomic)(u32 dev_id);
    int (*rao_read)(u32 dev_id, enum devdrv_rao_client_type type, u64 offset, u64 len);
    int (*rao_write)(u32 dev_id, enum devdrv_rao_client_type type, u64 offset, u64 len);
#else
    int (*register_non_trans_client)(const struct agentdrv_non_trans_msg_client *msg_client);
    int (*unregister_non_trans_client)(const struct agentdrv_non_trans_msg_client *msg_client);
    int (*register_common_msg_client)(const struct agentdrv_common_msg_client *msg_client);
    int (*unregister_common_msg_client)(const struct agentdrv_common_msg_client *msg_client);
    int (*common_msg_send)(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len,
        enum agentdrv_common_msg_type msg_type);
#endif
    int (*sync_msg_send)(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);
    int (*set_msg_chan_priv)(void *msg_chan, void *priv);
    void* (*get_msg_chan_priv)(void* msg_chan);
    int (*get_msg_chan_devid)(void *msg_chan);
    int (*get_connect_type)(u32 dev_id);
    int (*get_pfvf_type_by_devid)(u32 dev_id);
    bool (*mdev_vm_boot_mode)(u32 dev_id);
    bool (*sriov_support)(u32 dev_id);
    int (*sriov_enable)(u32 dev_id, u32 boot_mode);     /* not must */
    int (*sriov_disable)(u32 dev_id, u32 boot_mode);    /* not must */
    int (*register_rao_client)(u32 dev_id, enum devdrv_rao_client_type type, u64 va, u64 len,
        enum devdrv_rao_permission_type perm);
    int (*unregister_rao_client)(u32 dev_id, enum devdrv_rao_client_type type);
    int (*get_all_device_count)(u32 *count);
    int (*get_device_probe_list)(u32 *devids, u32 *count);
    int (*get_urma_info_by_eid)(u32 udevid, struct ascend_urma_dev_info *urma_info);
    int (*get_ub_dev_info)(u32 dev_id, struct devdrv_ub_dev_info *eid_info, int *num);
    int (*addr_trans_p2p)(u32 udevid, u32 peer_udevid, struct devdrv_addr_desc *addr_desc, u64 *trans_addr);
    int (*get_token_val)(u32 dev_id, u32 *token_val);
    int (*add_pasid)(u32 dev_id, u64 pasid);
    int (*del_pasid)(u32 dev_id, u64 pasid);
};

int devdrv_register_communication_ops(struct devdrv_comm_ops *ops);
void devdrv_unregister_communication_ops(struct devdrv_comm_ops *ops);
void devdrv_set_communication_ops_status(u32 type, u32 status, u32 dev_id);
void devdrv_set_communication_ops_status_inner(u32 type, u32 status, u32 index_id);
void devdrv_set_probe_dev_bitmap(u32 devid);
void devdrv_clr_probe_dev_bitmap(u32 devid);
u64 devdrv_check_probe_dev_bitmap(u32 devid);
int devdrv_hot_reset_device(u32 dev_id);
int devdrv_hot_pre_reset(u32 dev_id);
int devdrv_hotreset_atomic_reset(u32 dev_id);
int devdrv_hotreset_atomic_unbind(u32 dev_id);
int devdrv_hotreset_atomic_remove(u32 dev_id);
int devdrv_hotreset_atomic_rescan(u32 dev_id);
int devdrv_get_ub_dev_info(u32 dev_id, struct devdrv_ub_dev_info *eid_info, int *num);
int devdrv_get_token_val(u32 dev_id, u32 *token_val);

int devdrv_process_pasid_add(u32 dev_id, u64 pasid);
int devdrv_process_pasid_del(u32 dev_id, u64 pasid);
#endif /* __COMM_MSG_CHAN_H__ */