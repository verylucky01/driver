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
#ifndef _HDCDRV_ADAPT_UB_HOST_H_
#define _HDCDRV_ADAPT_UB_HOST_H_
#include "securec.h"

struct hdcdrv_dev;
struct hdcdrv_event_connect;
struct hdcdrv_session;
struct sched_published_event;
struct uda_dev_type;
union hdcdrv_cmd;
enum uda_notified_action;

#define HDCDRV_UB_NOTIFIER "hdc_ub"
void hdcdrv_set_msg_env(int *msg_env, int env);
int hdcdrv_service_scope_init_ub(int service_type);
int hdcdrv_service_log_limit_init_ub(int service_type);
void hdcdrv_ub_gen_unique_value_proc(u32 *value);
int hdcdrv_ub_get_session_run_env_proc(u32 dev_id, struct hdcdrv_dev *hdc_dev, struct hdcdrv_event_connect *conn_info);
long hdcdrv_ub_convert_id_from_vir_to_phy(u32 drv_cmd, union hdcdrv_cmd *cmd_data, u32 *vfid);
int hdcdrv_ub_get_session_run_env(u32 dev_id, struct hdcdrv_dev *hdc_dev);
ka_page_t *hdcdrv_ub_alloc_pages_node(u32 dev_id, gfp_t gfp_mask, u32 order);
void hdcdrv_close_remote_session_set_dst_engine(struct sched_published_event *event);
void hdcdrv_notify_msg_release_set_dst_engine(struct sched_published_event *event);
void hdcdrv_set_host_pm_or_vm_flag(struct hdcdrv_dev *hdc_dev);
void uda_davinci_real_entity_type_pack_proc(struct uda_dev_type *type);
void hdcdrv_set_session_run_env_ub(struct hdcdrv_session *session, int run_env);
int uda_notifier_register_proc(struct uda_dev_type *type);
int hdcdrv_ub_notifier_func(u32 udevid, enum  uda_notified_action action);
#endif /* _HDCDRV_ADAPT_UB_HOST_H_ */