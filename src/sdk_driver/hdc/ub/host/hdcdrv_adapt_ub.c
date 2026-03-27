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
#include "comm/comm_pcie.h"
#include "pbl/pbl_runenv_config.h"
#include "pbl/pbl_feature_loader.h"
#include "hdcdrv_log.h"
#include "hdcdrv_cmd_ioctl.h"
#include "hdcdrv_core_ub.h"

void hdcdrv_set_msg_env(int *msg_env, int env)
{
    *msg_env = env;
}

int hdcdrv_service_scope_init_ub(int service_type)
{
    if ((service_type == HDCDRV_SERVICE_TYPE_PROFILING) || (service_type == HDCDRV_SERVICE_TYPE_LOG) ||
        (service_type == HDCDRV_SERVICE_TYPE_DUMP)) {
        return HDCDRV_SERVICE_SCOPE_PROCESS;
    }

    return HDCDRV_SERVICE_SCOPE_GLOBAL;
}

int hdcdrv_service_log_limit_init_ub(int service_type)
{
    return HDCDRV_SERVICE_NO_LOG_LIMIT;
}

void hdcdrv_ub_gen_unique_value_proc(u32 *value)
{
    (*value) = HDCDRV_SESSION_UNIQUE_VALUE_HOST_FLAG | ((*value) & HDCDRV_SESSION_UNIQUE_VALUE_MASK);
}

int hdcdrv_ub_get_session_run_env(u32 dev_id, struct hdcdrv_dev *hdc_dev)
{
    int run_env = HDCDRV_SESSION_RUN_ENV_UNKNOW;
    bool is_in_container = false;

    if (devdrv_get_pfvf_type_by_devid(dev_id) == DEVDRV_SRIOV_TYPE_VF) {
        return HDCDRV_SESSION_RUN_ENV_VIRTUAL;
    }

    is_in_container = run_in_normal_docker();
    if (hdc_dev->host_pm_or_vm_flag == DEVDRV_HOST_PHY_MACH_FLAG) {
        if (is_in_container) {
            run_env = HDCDRV_SESSION_RUN_ENV_PHYSICAL_CONTAINER;
        } else {
            run_env = HDCDRV_SESSION_RUN_ENV_PHYSICAL;
        }
    } else {
        if (is_in_container) {
            run_env = HDCDRV_SESSION_RUN_ENV_VIRTUAL_CONTAINER;
        } else {
            run_env = HDCDRV_SESSION_RUN_ENV_VIRTUAL;
        }
    }

    return run_env;
}

int hdcdrv_ub_get_session_run_env_proc(u32 dev_id, struct hdcdrv_dev *hdc_dev, struct hdcdrv_event_connect *conn_info)
{
    (void)conn_info;
    return hdcdrv_ub_get_session_run_env(dev_id, hdc_dev);
}

STATIC int hdcdrv_container_vir_to_phs_devid(u32 virtual_devid, u32 *physical_devid, u32 *vfid)
{
    return devdrv_manager_container_logical_id_to_physical_id(virtual_devid, physical_devid, vfid);
}

long hdcdrv_ub_convert_id_from_vir_to_phy(u32 drv_cmd, union hdcdrv_cmd *cmd_data, u32 *vfid)
{
    long ret = HDCDRV_OK;

    int *p_devid = NULL;
    switch (drv_cmd) {
        case HDCDRV_CMD_GET_PEER_DEV_ID:
        case HDCDRV_CMD_CLIENT_DESTROY:
        case HDCDRV_CMD_SERVER_CREATE:
        case HDCDRV_CMD_SERVER_DESTROY:
        case HDCDRV_CMD_ACCEPT:
        case HDCDRV_CMD_CONNECT:
        case HDCDRV_CMD_CLOSE:
        case HDCDRV_CMD_GET_SESSION_UID:
        case HDCDRV_CMD_GET_SESSION_ATTR:
            p_devid = &cmd_data->cmd_com.dev_id;
            break;
        default:
            return ret;
    }
    ret = hdcdrv_container_vir_to_phs_devid((u32)(*p_devid), (u32 *)p_devid, vfid);

    return ret;
}

void hdcdrv_close_remote_session_set_dst_engine(struct sched_published_event *event)
{
    event->event_info.dst_engine = CCPU_DEVICE;
}

void hdcdrv_notify_msg_release_set_dst_engine(struct sched_published_event *event)
{
    event->event_info.dst_engine = CCPU_HOST;
}

ka_page_t *hdcdrv_ub_alloc_pages_node(u32 dev_id, gfp_t gfp_mask, u32 order)
{
    ka_page_t *page = NULL;
    page = ka_mm_alloc_pages(gfp_mask, order);
    if (page != NULL) {
        return page;
    }

    return page;
}

void hdcdrv_set_host_pm_or_vm_flag(struct hdcdrv_dev *hdc_dev)
{
    int ret = 0;
    u32 host_pm_or_vm_flag = 0;

    ret = devdrv_get_host_phy_mach_flag(hdc_dev->dev_id, &host_pm_or_vm_flag);
    if (ret != 0) {
        hdcdrv_warn("Get_host_phy_mach_flag has problem. (dev_id=%u; ret=%d)\n", hdc_dev->dev_id, ret);
    }
    hdc_dev->host_pm_or_vm_flag = (ret == 0) ? host_pm_or_vm_flag : 0;
}

void uda_davinci_real_entity_type_pack_proc(struct uda_dev_type *type)
{
    uda_davinci_near_real_entity_type_pack(type);
}

int uda_notifier_register_proc(struct uda_dev_type *type)
{
    (void)type;
#ifndef EMU_ST
    (void)module_feature_auto_init();
#endif

    return 0;
}

void hdcdrv_set_session_run_env_ub(struct hdcdrv_session *session, int run_env)
{
    (void)session;
    (void)run_env;
}