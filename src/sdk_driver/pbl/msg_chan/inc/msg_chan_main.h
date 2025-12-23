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
#ifndef MSG_CHAN_MAIN_H
#define MSG_CHAN_MAIN_H
#include <linux/mutex.h>
#include <linux/rwlock_types.h>
#include "comm_kernel_interface.h"
#include "msg_chan_adapt.h"

#ifndef EMU_ST
#include "dmc_kernel_interface.h"
#else
#include <linux/printk.h>
#endif

#define module_devdrv "devdrv"
#ifndef EMU_ST
#define devdrv_err(fmt, ...) do { \
    drv_err(module_devdrv, "<%s:%d:%d:%d> " fmt, \
        current->comm, current->tgid, current->pid, smp_processor_id(), ##__VA_ARGS__); \
} while (0)
#define devdrv_warn(fmt, ...) do { \
    drv_warn(module_devdrv, "<%s:%d:%d:%d> " fmt, \
        current->comm, current->tgid, current->pid, smp_processor_id(), ##__VA_ARGS__); \
} while (0)
#define devdrv_info(fmt, ...) do { \
    drv_info(module_devdrv, "<%s:%d:%d:%d> " fmt, \
        current->comm, current->tgid, current->pid, smp_processor_id(), ##__VA_ARGS__); \
} while (0)
#define devdrv_debug(fmt, ...) do { \
    drv_pr_debug(module_devdrv, "<%s:%d:%d:%d> " fmt, \
        current->comm, current->tgid, current->pid, smp_processor_id(), ##__VA_ARGS__); \
} while (0)
#else  // EMU_ST
#define devdrv_info(fmt, ...) do {                                      \
    printk(KERN_INFO "[ascend] [%s] [%s %d]" fmt,       \
    module_devdrv, __func__, __LINE__,                                \
    ##__VA_ARGS__);     \
} while (0)
#define devdrv_warn(fmt, ...) do {                                      \
    printk(KERN_WARNING "[ascend] [%s] [%s %d]" fmt,    \
    module_devdrv, __func__, __LINE__,                                \
    ##__VA_ARGS__);     \
} while (0)
#define devdrv_err(fmt, ...) do {                                       \
    printk(KERN_ERR "[ascend] [%s] [%s %d]" fmt,        \
    module_devdrv, __func__, __LINE__,                                \
    ##__VA_ARGS__);     \
} while (0)
#define devdrv_debug(fmt, ...) do {                                     \
    printk(KERN_DEBUG "[ascend] [%s] [%s %d]" fmt,      \
    module_devdrv, __func__, __LINE__,                                \
    ##__VA_ARGS__);     \
} while (0)
#endif  // EMU_ST
#ifdef STATIC_SKIP
#define STATIC
#else
#define STATIC static
#endif

#define COMMU_WAIT_MAX_CNT 10000  // 10s
#define COMMU_WAIT_PER_TIME 1000  // 1ms=1000us

struct devdrv_comm_dev_ops {
    enum devdrv_ops_status status;
    struct devdrv_comm_ops ops;
    rwlock_t rwlock;
    atomic_t dev_cnt;
};

struct devdrv_comm_dev_ops *devdrv_add_ops_ref(void);
struct devdrv_comm_dev_ops *devdrv_add_ops_ref_after_unbind(void);
struct devdrv_comm_dev_ops *devdrv_add_ops_ref_by_type(u32 type);
void devdrv_sub_ops_ref(struct devdrv_comm_dev_ops *dev_ops);
void devdrv_sub_ops_ref_by_type(struct devdrv_comm_dev_ops *dev_ops);
struct devdrv_comm_dev_ops *devdrv_get_comm_ops_ctrl(enum devdrv_communication_type type);
struct devdrv_comm_dev_ops *devdrv_get_comm_ops(void);
int devdrv_hotreset_atomic_rescan(u32 dev_id);
int devdrv_hotreset_atomic_reset(u32 dev_id);
int devdrv_hotreset_atomic_unbind(u32 dev_id);
int devdrv_hotreset_atomic_remove(u32 dev_id);

int devdrv_get_global_connect_protocol(void);

int devdrv_get_device_probe_list_urd(void *feature, char *in, u32 in_len, char *out, u32 out_len);
int devdrv_get_token_val_urd(void *feature, char *in, u32 in_len, char *out, u32 out_len);
int devdrv_get_connect_type_urd(void *feature, char *in, u32 in_len, char *out, u32 out_len);
int devdrv_get_device_probe_num_urd(void *feature, char *in, u32 in_len, char *out, u32 out_len);

int devdrv_sync_msg_send_proc(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);
int devdrv_get_msg_chan_devid_proc(void *msg_chan);
int devdrv_set_msg_chan_priv_proc(void *msg_chan, void *priv);
void *devdrv_get_msg_chan_priv_proc(void *msg_chan);

int devdrv_check_communication_api_proc(struct devdrv_comm_ops *ops);
int devdrv_check_dev_manager_api_proc(struct devdrv_comm_ops *ops);
int devdrv_check_communication_api(struct devdrv_comm_ops *ops);
int devdrv_check_dev_manager_api(struct devdrv_comm_ops *ops);

u32 devdrv_get_index_id_by_devid(u32 dev_id);
struct devdrv_comm_dev_ops *devdrv_get_token_val_ops(void);
void devdrv_register_save_client_info(struct devdrv_comm_dev_ops *dev_ops);
void devdrv_register_save_client_info_proc(struct devdrv_comm_dev_ops *dev_ops);
bool devdrv_is_mdev_vm_boot_mode_inner(u32 index_id);
bool devdrv_is_sriov_support_inner(u32 index_id);
int devdrv_get_connect_protocol_inner(u32 index_id);
#endif