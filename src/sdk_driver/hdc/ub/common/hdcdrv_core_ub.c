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

#include "ka_ioctl_pub.h"
#include "ka_compiler_pub.h"
#include "ka_fs_pub.h"
#include "ka_kernel_def_pub.h"
#include "pbl/pbl_uda.h"
#include "pbl/pbl_feature_loader.h"
#include "pbl/pbl_runenv_config.h"
#include "dpa_kernel_interface.h"
#include "kernel_version_adapt.h"
#include "pbl/pbl_davinci_api.h"
#include "comm_kernel_interface.h"
#include "hdcdrv_core_com_ub.h"
#include "hdcdrv_cmd.h"
#include "hdcdrv_cmd_ioctl.h"
#include "hdcdrv_proc_fs_ub.h"
#include "hdcdrv_mem_pool_ub.h"

struct hdcdrv_ctrl g_hdc_ctrl = {0};
ka_vm_operations_struct_t hdcdrv_ub_vm_ops_managed;

static struct {
    char str[HDCDRV_STR_NAME_LEN];
} hdcdrv_service_type_str[HDCDRV_SUPPORT_MAX_SERVICE + 1] = {
    { "service_dmp" },                      /* HDCDRV_SERVICE_TYPE_DMP */
    { "service_profiling" },                /* HDCDRV_SERVICE_TYPE_PROFILING */
    { "service_IDE1" },                     /* HDCDRV_SERVICE_TYPE_IDE1 */
    { "service_file_trans" },               /* HDCDRV_SERVICE_TYPE_FILE_TRANS */
    { "service_IDE2" },                     /* HDCDRV_SERVICE_TYPE_IDE2 */
    { "service_log" },                      /* HDCDRV_SERVICE_TYPE_LOG */
    { "service_rdma" },                     /* HDCDRV_SERVICE_TYPE_RDMA */
    { "service_bbox" },                     /* HDCDRV_SERVICE_TYPE_BBOX */
    { "service_framework" },                /* HDCDRV_SERVICE_TYPE_FRAMEWORK */
    { "service_TSD" },                      /* HDCDRV_SERVICE_TYPE_TSD */
    { "service_TDT" },                      /* HDCDRV_SERVICE_TYPE_TDT */
    { "service_PROF" },                     /* HDCDRV_SERVICE_TYPE_PROF */
    { "service_IDE_file_trans" },           /* HDCDRV_SERVICE_TYPE_IDE_FILE_TRANS */
    { "service_dump" },                     /* HDCDRV_SERVICE_TYPE_DUMP */
    { "service_user3" },                    /* HDCDRV_SERVICE_TYPE_USER3 */
    { "service_DVPP" },                     /* HDCDRV_SERVICE_TYPE_DVPP */
    { "service_queue" },                    /* HDCDRV_SERVICE_TYPE_QUEUE */
    { "service_upgrade" },                  /* HDCDRV_SERVICE_TYPE_UPGRADE */
    { "service_rdma_v2" },                  /* HDCDRV_SERVICE_TYPE_RDMA_V2 */
    { "service_test"},                      /* HDCDRV_SERVICE_TYPE_TEST */
};

STATIC const char *hdcdrv_sevice_str(int service_type)
{
    if ((service_type < HDCDRV_SERVICE_TYPE_DMP) || (service_type >= HDCDRV_SUPPORT_MAX_SERVICE)) {
        return "service_invalid";
    } else if ((service_type < HDCDRV_SERVICE_TYPE_USER_START) && (service_type >= HDCDRV_SERVICE_TYPE_APPLY_MAX)) {
        return "service_reserved";
    } else {
        return hdcdrv_service_type_str[service_type].str;
    }
}

static void hdcdrv_fill_service_type_str(void)
{
    int i, ret;

    for (i = HDCDRV_SERVICE_TYPE_USER_START; i < HDCDRV_SUPPORT_MAX_SERVICE; i++) {
        ret = snprintf_s(hdcdrv_service_type_str[i].str, HDCDRV_STR_NAME_LEN, HDCDRV_STR_NAME_LEN - 1,
            "service_user_%d", i - HDCDRV_SERVICE_TYPE_USER_START);
        if (ret < 0) {
            hdcdrv_err("snprintf_s error, service %d can not print error string in log.\n",
                i - HDCDRV_SERVICE_TYPE_USER_START);
        }
    }
}

void *hdcdrv_ub_kvmalloc(size_t size)
{
    void *addr = NULL;

    if (size == 0) {
        return NULL;
    }

    addr = ka_mm_kmalloc(size, KA_GFP_NOWAIT | __KA_GFP_NOWARN | __KA_GFP_ACCOUNT | __KA_GFP_ZERO);
    if (addr == NULL) {
        addr = ka_vmalloc(size, KA_GFP_KERNEL | __KA_GFP_ACCOUNT | __KA_GFP_ZERO, KA_PAGE_KERNEL);
        if (addr == NULL) {
            return NULL;
        }
    }

    return addr;
}

void hdcdrv_ub_kvfree(const void *addr)
{
    if (ka_mm_is_vmalloc_addr(addr)) {
        ka_mm_vfree(addr);
    } else {
        ka_mm_kfree(addr);
    }
    addr = NULL;
}

struct hdcdrv_dev *hdcdrv_ub_get_dev(u32 dev_id)
{
    struct hdcdrv_dev *hdc_dev = NULL;

    if (dev_id >= HDCDRV_SUPPORT_MAX_DEV) {
        return NULL;
    }

    ka_task_mutex_lock(&g_hdc_ctrl.dev_lock[dev_id]);
    hdc_dev = g_hdc_ctrl.dev[dev_id];
    if ((hdc_dev != NULL) && (hdc_dev->valid == HDCDRV_VALID)) {
        ka_base_atomic_add(1, &hdc_dev->ref);
    } else {
        hdc_dev = NULL;
    }
    ka_task_mutex_unlock(&g_hdc_ctrl.dev_lock[dev_id]);

    return hdc_dev;
}

void hdcdrv_put_dev(u32 dev_id)
{
    ka_base_atomic_sub(1, &g_hdc_ctrl.dev[dev_id]->ref);
}

STATIC struct hdcdrv_session *hdcdrv_get_session(u32 dev_id, u32 session_id)
{
    struct hdcdrv_dev *hdc_dev = NULL;
    hdc_dev = hdcdrv_ub_get_dev(dev_id);
    if (hdc_dev == NULL) {
        hdcdrv_err("Cannot find hdcdrv dev. (dev_id=%u)\n", dev_id);
        return NULL;
    }
    if (session_id >= HDCDRV_UB_SINGLE_DEV_MAX_SESSION) {
        hdcdrv_err("Invalid session id. (dev_id=%u;session_id=%u)\n", dev_id, session_id);
        return NULL;
    }
    return &hdc_dev->sessions[session_id];
}

STATIC void hdcdrv_put_session(u32 dev_id)
{
    hdcdrv_put_dev(dev_id);
}

STATIC int hdcdrv_service_level_init(int service_type)
{
    if ((service_type == HDCDRV_SERVICE_TYPE_RDMA) || (service_type == HDCDRV_SERVICE_TYPE_FRAMEWORK) ||
        (service_type == HDCDRV_SERVICE_TYPE_TSD) || (service_type == HDCDRV_SERVICE_TYPE_TDT)) {
        return HDCDRV_SERVICE_HIGH_LEVEL;
    }

    return HDCDRV_SERVICE_LOW_LEVEL;
}

STATIC int hdcdrv_service_conn_feature_init(int service_type)
{
    if (service_type == HDCDRV_SERVICE_TYPE_DMP) {
        return HDCDRV_SERVICE_SHORT_CONN;
    }

    return HDCDRV_SERVICE_LONG_CONN;
}

STATIC void hdcdrv_service_attr_init(struct hdcdrv_ctrl *hdc_ctrl)
{
    int i = 0;

    for (i = 0; i < HDCDRV_SUPPORT_MAX_SERVICE; i++) {
        hdc_ctrl->service_attr[i].level = hdcdrv_service_level_init(i);
        hdc_ctrl->service_attr[i].conn_feature = hdcdrv_service_conn_feature_init(i);
        hdc_ctrl->service_attr[i].service_scope = hdcdrv_service_scope_init_ub(i);
        hdc_ctrl->service_attr[i].log_limit = hdcdrv_service_log_limit_init_ub(i);
    }
}

STATIC int hdcdrv_bind_server_ctx(struct hdcdrv_ctx *ctx, struct hdcdrv_service *service,
    int dev_id, int service_type)
{
    ka_task_mutex_lock(&ctx->ctx_lock);
    if (((ctx->service_type != HDCDRV_SERVICE_TYPE_INVALID) || (ctx->session_fd != HDCDRV_SESSION_FD_INVALID))) {
        hdcdrv_err("ctx has been bind by other server or session.(dev_id=%d; session_id=%d; server_type=%d; "
            "pid=%llu)\n", ctx->dev_id, ctx->session_fd, ctx->service_type, ctx->pid);
        ka_task_mutex_unlock(&ctx->ctx_lock);
        return HDCDRV_ERR;
    }
    ctx->dev_id = dev_id;
    ctx->pid = service->listen_pid;
    ctx->service_type = service_type;

    service->ctx = ctx;
    ka_task_mutex_unlock(&ctx->ctx_lock);
    return HDCDRV_OK;
}

STATIC void hdcdrv_unbind_server_ctx(struct hdcdrv_service *server)
{
    struct hdcdrv_ctx *ctx = server->ctx;

    if (ctx != NULL) {
        ka_task_mutex_lock(&ctx->ctx_lock);
        server->ctx->service_type = HDCDRV_SERVICE_TYPE_INVALID;
        server->ctx = NULL;
        ka_task_mutex_unlock(&ctx->ctx_lock);
    }
}

STATIC int hdcdrv_bind_session_ctx(struct hdcdrv_ctx *ctx, struct hdcdrv_session *session)
{
    ka_task_mutex_lock(&ctx->ctx_lock);
    if (((ctx->service_type != HDCDRV_SERVICE_TYPE_INVALID) || (ctx->session_fd != HDCDRV_SESSION_FD_INVALID))) {
        hdcdrv_err("ctx has been bind by other server or session.(dev_id=%d; session_id=%d; server_type=%d; "
            "pid=%llu)\n", ctx->dev_id, ctx->session_fd, ctx->service_type, ctx->pid);
        ka_task_mutex_unlock(&ctx->ctx_lock);
        return HDCDRV_ERR;
    }

    ctx->session_fd = session->local_session_fd;
    ctx->pid = (u64)ka_task_get_current_tgid();
    ctx->dev_id = session->dev_id;
    session->ctx = ctx;
    ka_task_mutex_unlock(&ctx->ctx_lock);

    return HDCDRV_OK;
}

STATIC void hdcdrv_unbind_session_ctx(struct hdcdrv_session *session)
{
    struct hdcdrv_ctx *ctx = session->ctx;

    if (ctx != NULL) {
        ka_task_mutex_lock(&ctx->ctx_lock);
        ka_task_spin_lock_bh(&session->lock);
        session->ctx->session_fd = HDCDRV_SESSION_FD_INVALID;
        session->ctx = NULL;
        ka_task_spin_unlock_bh(&session->lock);
        ka_task_mutex_unlock(&ctx->ctx_lock);
    }
}

STATIC long hdcdrv_session_para_check(int session_fd, int device_id)
{
    struct hdcdrv_session *session = NULL;

    if ((session_fd >= HDCDRV_UB_SINGLE_DEV_MAX_SESSION) || (session_fd < 0)) {
        hdcdrv_err_limit("session_fd is illegal. (session_fd=%d; dev_id=%d)\n", session_fd, device_id);
        return -HDCDRV_PARA_ERR;
    }

    session = hdcdrv_get_session(device_id, session_fd);
    if (session == NULL) {
        hdcdrv_err_limit("session_fd is illegal. (session_fd=%d; dev_id=%d)\n", session_fd, device_id);
        return -HDCDRV_PARA_ERR;
    }

    if (hdcdrv_get_session_status(session) != HDCDRV_SESSION_STATUS_CONN) {
        hdcdrv_warn_limit("session has closed. (session_fd=%d; dev_id=%d; service_type=\"%s\";"
            "local_session_fd=%d; remote_session_fd=%d)\n",
            session_fd, session->dev_id, hdcdrv_sevice_str(session->service_type),
            session->local_session_fd, session->remote_session_fd);
        hdcdrv_put_session(device_id);
        return -HDCDRV_SESSION_HAS_CLOSED;
    }
    hdcdrv_put_session(device_id);
    return HDCDRV_OK;
}

STATIC long hdcdrv_cmd_client_destroy(struct hdcdrv_ctx *ctx, union hdcdrv_cmd *cmd_data, bool *copy_flag)
{
    return 0;
}

STATIC struct hdcdrv_service *hdcdrv_alloc_service(struct hdcdrv_dev *dev, int service_type, u64 host_pid)
{
    struct hdcdrv_serv_list_node *node;
    struct hdcdrv_service *service;
    ka_list_head_t *pos = NULL;
    ka_list_head_t *n = NULL;

    if ((service_type >= HDCDRV_SUPPORT_MAX_SERVICE) || (service_type < 0)) {
        return NULL;
    }

    service = &dev->service[service_type];
    if (g_hdc_ctrl.service_attr[service_type].service_scope == HDCDRV_SERVICE_SCOPE_GLOBAL) {
        return service;
    }
    ka_task_mutex_lock(&g_hdc_ctrl.dev_lock[dev->dev_id]);
    if (!ka_list_empty_careful(&service->serv_list)) {
        ka_list_for_each_safe(pos, n, &service->serv_list) {
            node = ka_list_entry(pos, struct hdcdrv_serv_list_node, list);
            if (node->service.listen_pid == host_pid) {
                ka_task_mutex_unlock(&g_hdc_ctrl.dev_lock[dev->dev_id]);
                return &node->service;
            }
        }
    }
#ifndef DRV_UT
    if (!ka_list_empty_careful(&service->serv_list)) {
        ka_list_for_each_safe(pos, n, &service->serv_list) {
            node = ka_list_entry(pos, struct hdcdrv_serv_list_node, list);
            if (node->service.listen_pid == HDCDRV_INVALID) {
                node->service.listen_pid = host_pid;
                ka_task_mutex_unlock(&g_hdc_ctrl.dev_lock[dev->dev_id]);
                return &node->service;
            }
        }
    }
#endif
    ka_task_mutex_unlock(&g_hdc_ctrl.dev_lock[dev->dev_id]);
    return NULL;
}

STATIC struct hdcdrv_service *hdcdrv_search_service(struct hdcdrv_dev *dev, int service_type, u64 host_pid)
{
    struct hdcdrv_serv_list_node *node;
    struct hdcdrv_service *service;
    ka_list_head_t *pos = NULL;
    ka_list_head_t *n = NULL;

    if ((service_type >= HDCDRV_SUPPORT_MAX_SERVICE) || (service_type < 0)) {
        return NULL;
    }

    service = &dev->service[service_type];
    if (g_hdc_ctrl.service_attr[service_type].service_scope == HDCDRV_SERVICE_SCOPE_GLOBAL) {
        return service;
    }
    ka_task_mutex_lock(&g_hdc_ctrl.dev_lock[dev->dev_id]);
    if (!ka_list_empty_careful(&service->serv_list)) {
        ka_list_for_each_safe(pos, n, &service->serv_list) {
            node = ka_list_entry(pos, struct hdcdrv_serv_list_node, list);
            if (node->service.listen_pid == host_pid) {
                ka_task_mutex_unlock(&g_hdc_ctrl.dev_lock[dev->dev_id]);
                return &node->service;
            }
        }
    }
    ka_task_mutex_unlock(&g_hdc_ctrl.dev_lock[dev->dev_id]);
    return service;
}

STATIC long hdcdrv_cmd_server_create(struct hdcdrv_ctx *ctx, union hdcdrv_cmd *cmd_data, bool *copy_flag)
{
    struct hdcdrv_service *service;
    struct hdcdrv_dev *dev;
    struct hdcdrv_cmd_server_create *cmd = &cmd_data->server_create;
    u32 dev_id = (u32)cmd->dev_id;

    dev = hdcdrv_ub_get_dev(dev_id);
    if (dev == NULL) {
        hdcdrv_err("Cannot find hdcdrv dev. (dev_id=%u)\n", dev_id);
        return -HDCDRV_PARA_ERR;
    }
    service = hdcdrv_alloc_service(dev, cmd->service_type, cmd->pid);
    if (service == NULL) {
        hdcdrv_err("Alloc service failed.(dev_id=%u;type=%d;pid=%llu)\n", dev_id, cmd->service_type, cmd->pid);
        hdcdrv_put_dev(cmd->dev_id);
        return -HDCDRV_PARA_ERR;
    }
    ka_task_mutex_lock(&service->mutex);
    if (service->listen_status == HDCDRV_VALID) {
        ka_task_mutex_unlock(&service->mutex);
        hdcdrv_err("server has create.(dev_id=%u;type=%d;pid=%llu)\n", dev_id, cmd->service_type, cmd->pid);
        hdcdrv_put_dev(cmd->dev_id);
        return -HDCDRV_SERVICE_LISTENING;
    }

    service->listen_pid = cmd->pid;
    if (hdcdrv_bind_server_ctx(ctx, service, dev_id, cmd->service_type) != HDCDRV_OK) {
        service->listen_pid = HDCDRV_INVALID;
        ka_task_mutex_unlock(&service->mutex);
        hdcdrv_put_dev(cmd->dev_id);
        return -HDCDRV_ERR;
    }

    service->listen_status = HDCDRV_VALID;
    service->gid = cmd->gid;
    ka_task_mutex_unlock(&service->mutex);
    hdcdrv_info("Server create success.(dev_id=%d; service_type=\"%s\"; pid=%llu; gid=%u)\n",
        dev_id, hdcdrv_sevice_str(cmd->service_type), cmd->pid, cmd->gid);
    hdcdrv_put_dev(cmd->dev_id);
    return 0;
}

// This func must be used in lock
STATIC void hdcdrv_clear_server_conn_list(struct hdcdrv_service *service)
{
    struct hdcdrv_conn_info_list *connect_t = NULL;

    while (service->conn_list != NULL) {
        connect_t = service->conn_list;
        service->conn_list = service->conn_list->next;
        hdcdrv_ub_kvfree(connect_t);
        connect_t = NULL;
    }
}

STATIC long hdcdrv_cmd_server_destroy(struct hdcdrv_ctx *ctx, union hdcdrv_cmd *cmd_data, bool *copy_flag)
{
    struct hdcdrv_service *service;
    struct hdcdrv_dev *dev;
    struct hdcdrv_cmd_server_destroy *cmd = &cmd_data->server_destroy;
    u32 dev_id = (u32)cmd->dev_id;

    dev = hdcdrv_ub_get_dev(dev_id);
    if (dev == NULL) {
        hdcdrv_err("Cannot find hdcdrv dev. (dev_id=%u)\n", dev_id);
        return -HDCDRV_PARA_ERR;
    }
    service = hdcdrv_search_service(dev, cmd->service_type, cmd->pid);
    if (service == NULL) {
        hdcdrv_err_limit("service invalid. (devid=%u; server_type=%d).\n", dev_id, cmd->service_type);
        hdcdrv_put_dev(cmd->dev_id);
        return -HDCDRV_PARA_ERR;
    }

    ka_task_mutex_lock(&service->mutex);
    if (service->listen_pid != cmd->pid) {
        ka_task_mutex_unlock(&service->mutex);
        hdcdrv_info("Server has already been destroyed.(dev_id=%d;service_type=\"%s\";pid=%llu)\n",
            dev_id, hdcdrv_sevice_str(cmd->service_type), cmd->pid);
        hdcdrv_put_dev(cmd->dev_id);
        return 0;
    }

    if (service->ctx != ctx) {
        ka_task_mutex_unlock(&service->mutex);
        hdcdrv_info("service_ctx not match.(dev_id=%d;service_type=\"%s\";pid=%llu)\n",
            dev_id, hdcdrv_sevice_str(cmd->service_type), cmd->pid);
        hdcdrv_put_dev(cmd->dev_id);
        return 0;
    }
    hdcdrv_clear_server_conn_list(service);
    hdcdrv_unbind_server_ctx(service);
    service->listen_pid = HDCDRV_INVALID;
    service->listen_status = HDCDRV_INVALID;
    ka_task_mutex_unlock(&service->mutex);
    hdcdrv_info("Server destroy success.(dev_id=%d;service=\"%s\";pid=%llu)\n",
        cmd->dev_id, hdcdrv_sevice_str(cmd->service_type), cmd->pid);
    hdcdrv_put_dev(cmd->dev_id);
    return 0;
}

STATIC int hdcdrv_local_session_check(struct hdcdrv_session *session, int *status, u64 *owner_pid)
{
    *status = hdcdrv_get_session_status(session);

    if (*status != HDCDRV_SESSION_STATUS_CONN) {
        return -HDCDRV_SESSION_HAS_CLOSED;
    }

    if (session->owner_pid != (u64)ka_task_get_current_tgid()) {
        *owner_pid = session->owner_pid;
        return -HDCDRV_NO_PERMISSION;
    }

    return HDCDRV_OK;
}

STATIC long hdcdrv_cmd_get_uid_state(struct hdcdrv_ctx *ctx, union hdcdrv_cmd *cmd_data, bool *copy_flag)
{
    struct hdcdrv_cmd_get_uid_stat *cmd = &cmd_data->get_uid_stat;
    struct hdcdrv_session *session = NULL;
    u32 session_id = (u32)cmd->session;
    u32 dev_id = (u32)cmd->dev_id;
    u64 owner_pid = HDCDRV_INVALID_OWNER_PID;
    long ret;
    int status;

    session = hdcdrv_get_session(dev_id, session_id);
    if (session == NULL) {
        hdcdrv_err("Cannot find hdcdrv session. (dev_id=%u;session_id=%u)\n", dev_id, session_id);
        return -HDCDRV_PARA_ERR;
    }

    ka_task_spin_lock_bh(&session->lock);
    ret = (long)hdcdrv_local_session_check(session, &status, &owner_pid);
    if (ret != HDCDRV_OK) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_put_session(cmd->dev_id);
        hdcdrv_warn("session has been closed or pid has problem.(session_fd=%d; dev_id=%d; status=%d; owner_pid=%llu; "
            "current_pid=%u; ret=%ld)\n", cmd->session, cmd->dev_id, status, owner_pid, ka_task_get_current_tgid(), ret);
        return ret;
    }

    cmd->euid = (u32)session->euid;
    cmd->uid = (u32)session->uid;
    cmd->root_privilege = session->root_privilege;

    *copy_flag = true;
    ka_task_spin_unlock_bh(&session->lock);
    hdcdrv_put_session(cmd->dev_id);
    return 0;
}

STATIC int hdcdrv_get_service_conn_feature(int service_type)
{
    return g_hdc_ctrl.service_attr[service_type].conn_feature;
}

STATIC void hdcdrv_uid_privilege_get(int *euid, int *uid, int *root_privilege, int dev_id)
{
    if ((devdrv_get_pfvf_type_by_devid(dev_id) == DEVDRV_SRIOV_TYPE_VF) || (ka_task_get_current_cred() == NULL)) {
        *euid = -1;
        *uid =  -1;
    } else {
        *euid = ka_task_get_current_cred_euid();
        *uid = ka_task_get_current_cred_uid();
    }

    *root_privilege = (*euid == 0) ? 1 : 0;
}

STATIC int hdcdrv_alloc_session(int service_type, int run_env, int root_privilege, u64 check_pid,
    struct hdcdrv_dev *hdc_dev)
{
    int i, max, base, fd;
    int session_fd = -1;
    int conn_feature = hdcdrv_get_service_conn_feature(service_type);
    int *cur = NULL;

    ka_task_mutex_lock(&g_hdc_ctrl.dev_lock[hdc_dev->dev_id]);
again:
    if (conn_feature == HDCDRV_SERVICE_LONG_CONN) {
        max = (int)256; // max long session num for one device is 256
        base = 0;   // long session start from 0
        cur = &hdc_dev->cur_alloc_session;
    } else {
        max = (int)8;   // max short session num for one device is 8
        base = (int)256;    // short session start from 256
        cur = &hdc_dev->cur_alloc_short_session;
    }

    for (i = 0; i < max; i++) {
        fd = (*cur + i) % max + base;
        if ((hdc_dev->sessions[fd].create_pid != check_pid) &&
            (hdcdrv_get_session_status(&hdc_dev->sessions[fd]) == HDCDRV_SESSION_STATUS_IDLE)) {
                session_fd = fd;
                hdcdrv_set_session_status(&hdc_dev->sessions[fd], HDCDRV_SESSION_STATUS_CONN);
                *cur = (fd + 1) % max + base;
                break;
        }
    }

    if ((session_fd == -1) && (conn_feature == HDCDRV_SERVICE_SHORT_CONN) &&
        (run_env != HDCDRV_SESSION_RUN_ENV_PHYSICAL_CONTAINER) &&
        (run_env != HDCDRV_SESSION_RUN_ENV_VIRTUAL_CONTAINER) && root_privilege) {
        hdcdrv_info_limit("No session, try another. (service=\"%s\"; conn_feature=%d)",
            hdcdrv_sevice_str(service_type), conn_feature);
        conn_feature = HDCDRV_SERVICE_LONG_CONN;
        goto again;
    }

    ka_task_mutex_unlock(&g_hdc_ctrl.dev_lock[hdc_dev->dev_id]);

    return session_fd;
}

STATIC u32 hdcdrv_ub_gen_unique_value(void)
{
    u32 value;

    value = (u32)ka_base_atomic_inc_return(&g_hdc_ctrl.unique_val);
    hdcdrv_ub_gen_unique_value_proc(&value);
    return value;
}

STATIC int hdcdrv_dev_para_check(int dev_id, int service_type)
{
    if ((dev_id >= hdcdrv_get_max_support_dev()) || (dev_id < 0)) {
        hdcdrv_err_limit("Input parameter is error. (dev_id=%d)\n", dev_id);
        return -HDCDRV_PARA_ERR;
    }

    if ((service_type >= HDCDRV_SUPPORT_MAX_SERVICE) || (service_type < 0)) {
        hdcdrv_err_limit("Input parameter is error. (dev_id=%d; service_type=%d)\n", dev_id, service_type);
        return -HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

STATIC long hdcdrv_cmd_alloc_connect_session(struct hdcdrv_ctx *ctx, union hdcdrv_cmd *cmd_data, bool *copy_flag)
{
    struct hdcdrv_cmd_connect *cmd = &cmd_data->connect;
    int fd, ret, run_env, euid, uid, root_privilege;
    struct hdcdrv_session *session = NULL;
    struct hdcdrv_dev *hdc_dev = NULL;

    ret = hdcdrv_dev_para_check(cmd->dev_id, cmd->service_type);
    if (ret != HDCDRV_OK) {
        return ret;
    }

    hdc_dev = hdcdrv_ub_get_dev(cmd->dev_id);
    if (hdc_dev == NULL) {
        hdcdrv_err("Cannot find hdcdrv dev. (dev_id=%u)\n", cmd->dev_id);
        return -HDCDRV_PARA_ERR;
    }

    hdcdrv_uid_privilege_get(&euid, &uid, &root_privilege, cmd->dev_id);
    cmd->pid = (u64)ka_task_get_current_tgid();
    cmd->unique_val = hdcdrv_ub_gen_unique_value();
    run_env = hdcdrv_ub_get_session_run_env(cmd->dev_id, hdc_dev);

    fd = hdcdrv_alloc_session(cmd->service_type, run_env, root_privilege, cmd->pid, hdc_dev);
    if (fd == -1) {
        hdcdrv_err("session alloc failed.\n");
        hdcdrv_put_dev(cmd->dev_id);
        return -HDCDRV_NO_SESSION;
    }

    cmd->session = fd;
    session = &g_hdc_ctrl.dev[cmd->dev_id]->sessions[fd];
    session->local_session_fd = fd;
    session->run_env = run_env;
    session->euid = euid;
    session->uid = uid;
    session->root_privilege = root_privilege;
    session->create_pid = cmd->pid;
    session->peer_create_pid = (u64)cmd->peer_pid;
    session->unique_val = cmd->unique_val;
    session->dev_id = cmd->dev_id;
    session->service_type = cmd->service_type;
    session->remote_close_flag = true;

    if (hdcdrv_bind_session_ctx(ctx, session) != HDCDRV_OK) {
        hdcdrv_set_session_status(&hdc_dev->sessions[fd], HDCDRV_SESSION_STATUS_IDLE);
        hdcdrv_put_dev(cmd->dev_id);
        return -HDCDRV_ERR;
    }

    ret = hdcdrv_alloc_mem_pool_for_session(session, hdc_dev, ctx, cmd->user_va);
    if (ret != 0) {
        hdcdrv_unbind_session_ctx(session);
        hdcdrv_set_session_status(&hdc_dev->sessions[fd], HDCDRV_SESSION_STATUS_IDLE);
        hdcdrv_put_dev(cmd->dev_id);
        return ret;
    }

    *copy_flag = true;
    session->owner_pid = (u64)ka_task_get_current_tgid();
    hdcdrv_put_dev(cmd->dev_id);

    return 0;
}

STATIC struct hdcdrv_conn_info_list *hdcdrv_ub_get_conn_list_node(struct hdcdrv_service *service, unsigned int client_id,
    unsigned int unique_val) {
    struct hdcdrv_conn_info_list *curr = service->conn_list;
    struct hdcdrv_conn_info_list *prev = NULL;
    struct hdcdrv_conn_info_list *node = NULL;

    while (curr != NULL) {
        if ((curr->conn_info.client_session == client_id) && (curr->conn_info.unique_val == unique_val)) {
            if (prev == NULL) {
                service->conn_list = curr->next;
            } else {
                prev->next = curr->next;
            }
            node = curr;
            break;
        }

        prev = curr;
        curr = curr->next;
    }

    return node;
}

STATIC int hdcdrv_get_conn_info_from_server(struct hdcdrv_dev *dev, struct hdcdrv_cmd_accept *cmd,
    struct hdcdrv_event_connect *conn_info)
{
    struct hdcdrv_service *service = NULL;
    struct hdcdrv_conn_info_list *node;

    service = hdcdrv_search_service(dev, cmd->service_type, cmd->pid);
    if (service == NULL) {
        return -HDCDRV_PARA_ERR;
    }

    ka_task_mutex_lock(&service->mutex);
    if (service->listen_status != HDCDRV_VALID) {
        ka_task_mutex_unlock(&service->mutex);
        return -HDCDRV_SERVICE_NO_LISTENING;
    }

    if (service->listen_pid != cmd->pid) {
        ka_task_mutex_unlock(&service->mutex);
        return -HDCDRV_NO_PERMISSION;
    }

    if (service->conn_list == NULL) {
        ka_task_mutex_unlock(&service->mutex);
        return -HDCDRV_SERVICE_NO_LISTENING;
    }

    node = hdcdrv_ub_get_conn_list_node(service, cmd->remote_session, cmd->unique_val);
    if (node == NULL) {
        hdcdrv_err("No expect sessions in conn_list.(service_type=\"%s\"; dev_id=%u)\n",
            hdcdrv_sevice_str(cmd->service_type), dev->dev_id);
        ka_task_mutex_unlock(&service->mutex);
        return -HDCDRV_ERR;
    }

    (void)memcpy_s(conn_info, sizeof(struct hdcdrv_event_connect), &node->conn_info,
        sizeof(struct hdcdrv_event_connect));
    ka_task_mutex_unlock(&service->mutex);

    hdcdrv_ub_kvfree(node);
    return 0;
}

STATIC long hdcdrv_cmd_alloc_accept_session(struct hdcdrv_ctx *ctx, union hdcdrv_cmd *cmd_data, bool *copy_flag)
{
    struct hdcdrv_cmd_accept *cmd = &cmd_data->accept;
    int fd, ret, run_env;
    struct hdcdrv_session *session = NULL;
    u32 dev_id = (u32)cmd->dev_id;
    struct hdcdrv_dev *hdc_dev = NULL;
    struct hdcdrv_event_connect conn_info;

    ret = hdcdrv_dev_para_check(cmd->dev_id, cmd->service_type);
    if (ret != HDCDRV_OK) {
        return ret;
    }

    hdc_dev = hdcdrv_ub_get_dev(dev_id);
    if (hdc_dev == NULL) {
        hdcdrv_err("Cannot find hdcdrv dev. (dev_id=%u)\n", dev_id);
        return -HDCDRV_PARA_ERR;
    }

    ret = hdcdrv_get_conn_info_from_server(hdc_dev, cmd, &conn_info);
    if (ret != 0) {
        hdcdrv_err("service_type not correct for server or server has destroy. (dev_id=%u)\n", dev_id);
        hdcdrv_put_dev(dev_id);
        return ret;
    }

    run_env = hdcdrv_ub_get_session_run_env_proc(dev_id, hdc_dev, &conn_info);
    cmd->pid = (u64)ka_task_get_current_tgid();
    fd = hdcdrv_alloc_session(cmd->service_type, run_env, conn_info.root_privilege, cmd->pid, hdc_dev);
    if (fd == -1) {
        hdcdrv_err("session alloc failed.\n");
        hdcdrv_put_dev(dev_id);
        return -HDCDRV_NO_SESSION;
    }

    cmd->session = fd;
    session = &g_hdc_ctrl.dev[cmd->dev_id]->sessions[fd];
    session->remote_session_fd = (int)conn_info.client_session;
    session->local_session_fd = fd;
    session->unique_val = conn_info.unique_val;
    session->euid = conn_info.euid;
    session->uid = conn_info.uid;
    session->root_privilege = conn_info.root_privilege;
    session->create_pid = cmd->pid;
    session->peer_create_pid = conn_info.client_pid;
    session->dev_id = cmd->dev_id;
    session->service_type = cmd->service_type;
    session->run_env = run_env;
    cmd->remote_gid = conn_info.connect_gid;
    cmd->remote_tid = conn_info.connect_tid;
    cmd->peer_pid = conn_info.client_pid;
    session->remote_close_flag = true;

    if (hdcdrv_bind_session_ctx(ctx, session) != HDCDRV_OK) {
        hdcdrv_set_session_status(&hdc_dev->sessions[fd], HDCDRV_SESSION_STATUS_IDLE);
        hdcdrv_put_dev(dev_id);
        return -HDCDRV_ERR;
    }

    ret = hdcdrv_alloc_mem_pool_for_session(session, hdc_dev, ctx, cmd->user_va);
    if (ret != 0) {
        hdcdrv_unbind_session_ctx(session);
        hdcdrv_set_session_status(&hdc_dev->sessions[fd], HDCDRV_SESSION_STATUS_IDLE);
        hdcdrv_put_dev(dev_id);
        return ret;
    }

    *copy_flag = true;
    session->owner_pid = (u64)ka_task_get_current_tgid();
    hdcdrv_put_dev(dev_id);

    return 0;
}

STATIC void hdcdrv_ub_close_remote_session(struct hdcdrv_sync_event_msg *msg, u32 dev_id)
{
    struct sched_published_event event;
    int ret;

    msg->data.type = HDCDRV_NOTIFY_MSG_CLOSE_RELEASE;

    event.event_info.msg_len = sizeof(struct hdcdrv_sync_event_msg);
    event.event_info.msg = (char *)msg;

    event.event_func.event_ack_func = NULL;
    event.event_func.event_finish_func = NULL;
    event.event_info.dst_devid = dev_id;
    event.event_info.event_id = EVENT_DRV_MSG;
    event.event_info.subevent_id = DRV_SUBEVENT_HDC_CLOSE_LINK_MSG;
    event.event_info.gid = 0;
    event.event_info.pid = 0;

    hdcdrv_close_remote_session_set_dst_engine(&event);
    ret = hal_kernel_sched_submit_event(dev_id, &event);
    if (ret != 0) {
        hdcdrv_err("sched_submit_event failed.(ret=%d)\n", ret);
    }

    return;
}

STATIC long hdcdrv_cmd_close(struct hdcdrv_ctx *ctx, union hdcdrv_cmd *cmd_data, bool *copy_flag)
{
    struct hdcdrv_cmd_close *cmd = &cmd_data->close;
    struct hdcdrv_dev *hdc_dev = NULL;
    struct hdcdrv_session *session = NULL;
    int fd = cmd->session;
    u32 dev_id = (u32)cmd->dev_id;
    int last_status, ret;
    struct hdcdrv_sync_event_msg close_info;

    ka_task_mutex_lock(&g_hdc_ctrl.mutex);
    hdc_dev = hdcdrv_ub_get_dev(dev_id);
    if (hdc_dev == NULL) {
        ka_task_mutex_unlock(&g_hdc_ctrl.mutex);
        return -HDCDRV_PARA_ERR;
    }

    session = hdcdrv_get_session(dev_id, fd);
    if (session == NULL) {
        ka_task_mutex_unlock(&g_hdc_ctrl.mutex);
        hdcdrv_put_dev(cmd->dev_id);
        return -HDCDRV_PARA_ERR;
    }

    ka_task_spin_lock_bh(&session->lock);
    last_status = hdcdrv_get_session_status(session);
    if (last_status != HDCDRV_SESSION_STATUS_CONN) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_warn("session has closed in kernel.(devid=%d; session_fd=%d)\n", dev_id, fd);
        ka_task_mutex_unlock(&g_hdc_ctrl.mutex);
        hdcdrv_put_session(cmd->dev_id);
        hdcdrv_put_dev(cmd->dev_id);
        return -HDCDRV_SESSION_HAS_CLOSED;
    }

    if ((cmd->pid != session->create_pid) && (cmd->pid != session->owner_pid)) {
        ka_task_spin_unlock_bh(&session->lock);
        ka_task_mutex_unlock(&g_hdc_ctrl.mutex);
        hdcdrv_put_session(cmd->dev_id);
        hdcdrv_put_dev(cmd->dev_id);
        return -HDCDRV_NO_PERMISSION;
    }

    hdcdrv_set_session_status(session, HDCDRV_SESSION_STATUS_CLOSING);
    close_info.data.close_msg.local_session = (u32)session->local_session_fd;
    close_info.data.close_msg.remote_session = (u32)session->remote_session_fd;
    close_info.data.close_msg.owner_pid = session->owner_pid;
    close_info.data.close_msg.peer_pid = session->peer_create_pid;
    close_info.data.close_msg.unique_val = session->unique_val;
    close_info.data.close_msg.session_close_state = HDCDRV_CLOSE_TYPE_RELEASE;

    session->create_pid = HDCDRV_INVALID_PID;
    session->peer_create_pid = HDCDRV_INVALID_PID;
    session->owner_pid = HDCDRV_INVALID_PID;
    ka_task_spin_unlock_bh(&session->lock);

    if ((last_status == HDCDRV_SESSION_STATUS_CONN) && (cmd->local_close_state == HDCDRV_CLOSE_TYPE_RELEASE)) {
        /* In case of connection notification peer */
        hdcdrv_ub_close_remote_session(&close_info, dev_id);
    }

    if ((hdcdrv_use_kernel_mem_pool(session->service_type)) && (cmd->local_close_state != HDCDRV_CLOSE_TYPE_RELEASE)) {
        ret = hdcdrv_free_mem_pool_for_session(session, hdc_dev, ctx);
        if (ret == HDCDRV_VA_UNMAP_FAILED) {
            hdcdrv_err("va unmap failed, mem pool will not set free until this process exit.(session=%d; devid=%d)\n",
                session->local_session_fd, session->dev_id);
        }
    }

    hdcdrv_unbind_session_ctx(session);
    ka_task_spin_lock_bh(&session->lock);
    hdcdrv_set_session_status(session, HDCDRV_SESSION_STATUS_IDLE);
    session->remote_close_flag = false;
    ka_task_spin_unlock_bh(&session->lock);
    ka_task_mutex_unlock(&g_hdc_ctrl.mutex);

    *copy_flag = false;
    hdcdrv_put_session(cmd->dev_id);
    hdcdrv_put_dev(cmd->dev_id);
    return 0;
}

#define HDCDRV_UB_MAX_SEG_CAPABILITY (4 * 1024)    /* 4k */
STATIC long hdcdrv_cmd_config(struct hdcdrv_ctx *ctx, union hdcdrv_cmd *cmd_data, bool *copy_flag)
{
    struct hdcdrv_cmd_config *cmd = &cmd_data->config;

    cmd->segment = HDCDRV_UB_MAX_SEG_CAPABILITY;

    *copy_flag = true;
    return HDCDRV_OK;
}

STATIC long hdcdrv_cmd_get_peer_dev_id(struct hdcdrv_ctx *ctx, union hdcdrv_cmd *cmd_data, bool *copy_flag)
{
    struct hdcdrv_cmd_get_peer_dev_id *cmd = &cmd_data->get_peer_dev_id;
    struct hdcdrv_dev *hdc_dev = NULL;
    u32 udevid, remote_udevid;
    int ret;

    hdc_dev = hdcdrv_ub_get_dev(cmd->dev_id);
    if (hdc_dev == NULL) {
        hdcdrv_err("Cannot find hdcdrv dev. (dev_id=%u)\n", cmd->dev_id);
        return -HDCDRV_PARA_ERR;
    }

    // cmd->devid has been transformed from logical devid to phy devid, so can be used to get remote udevid directly
    udevid = (u32)cmd->dev_id;
    ret = uda_dev_get_remote_udevid(udevid, &remote_udevid);
    if (ret != 0) {
        hdcdrv_err("Cannot get remote udevid by devid. (dev_id=%u, udevid=%u)\n", cmd->dev_id, udevid);
        hdcdrv_put_dev(cmd->dev_id);
        return -HDCDRV_PARA_ERR;
    }

    cmd->peer_dev_id = (int)remote_udevid;

    *copy_flag = true;
    hdcdrv_put_dev(cmd->dev_id);
    return HDCDRV_OK;
}

STATIC long hdcdrv_cmd_get_session_info(struct hdcdrv_ctx *ctx, union hdcdrv_cmd *cmd_data, bool *copy_flag)
{
    struct hdcdrv_cmd_get_session_info *cmd = &cmd_data->get_session_info;

    cmd->fid = 0;

    *copy_flag = true;
    return HDCDRV_OK;
}

STATIC long hdcdrv_ub_attr_get_session_status(struct hdcdrv_cmd_get_session_attr *cmd)
{
    int ret = hdcdrv_session_para_check(cmd->session, cmd->dev_id);
    struct hdcdrv_session *session = NULL;

    if (ret != 0) {
        if (ret == -HDCDRV_PARA_ERR) {
            return ret;
        }
        cmd->output = ret;
        return 0;
    }

    session = hdcdrv_get_session(cmd->dev_id, cmd->session);
    if (session == NULL) {
        return -HDCDRV_DEVICE_NOT_READY;
    }

    // true means session has not closed, false means session has been close by remote or close
    if ((!session->remote_close_flag) || (session->create_pid != cmd->pid)) {
        cmd->output = -HDCDRV_SESSION_HAS_CLOSED;
    } else {
        cmd->output = 0;
    }

    return 0;
}

STATIC long hdcdrv_cmd_get_session_attr(struct hdcdrv_ctx *ctx, union hdcdrv_cmd *cmd_data, bool *copy_flag)
{
    struct hdcdrv_cmd_get_session_attr *cmd = &cmd_data->get_session_attr;
    struct hdcdrv_session *session = NULL;
    u64 owner_pid = HDCDRV_INVALID_OWNER_PID;
    long ret;
    int status;

    if (cmd->cmd_type == HDCDRV_SESSION_ATTR_STATUS) {
        *copy_flag = true;
        return hdcdrv_ub_attr_get_session_status(cmd);
    }

    /* get session status should before hdcdrv_session_valid_check */
    ret = hdcdrv_session_para_check(cmd->session, cmd->dev_id);
    if (ret != HDCDRV_OK) {
        return ret;
    }

    session = hdcdrv_get_session(cmd->dev_id, cmd->session);
    if (session == NULL) {
        return -HDCDRV_PARA_ERR;
    }

    ka_task_spin_lock_bh(&session->lock);
    ret = (long)hdcdrv_local_session_check(session, &status, &owner_pid);
    if (ret != HDCDRV_OK) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_put_session(cmd->dev_id);
        hdcdrv_warn("session has been closed or pid has problem.(session_fd=%d; dev_id=%d; status=%d; owner_pid=%llu; "
            "current_pid=%u; ret=%ld)\n", cmd->session, cmd->dev_id, status, owner_pid, ka_task_get_current_tgid(), ret);
        return ret;
    }
    switch (cmd->cmd_type) {
        case HDCDRV_SESSION_ATTR_RUN_ENV:
            cmd->output = session->run_env;
            break;
        case HDCDRV_SESSION_ATTR_DFX:
            ret = HDCDRV_ERR;
            break;
        default:
            ret = HDCDRV_ERR;
            break;
    }
    ka_task_spin_unlock_bh(&session->lock);
    hdcdrv_put_session(cmd->dev_id);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("Not support dfx or input parameter is error.(type=%d; session_fd=%d; dev_id=%d)\n",
            cmd->cmd_type, cmd->session, cmd->dev_id);
    }
    *copy_flag = true;

    return HDCDRV_OK;
}

static long (*hdcdrv_operation[HDCDRV_CMD_MAX])(struct hdcdrv_ctx *ctx, union hdcdrv_cmd *cmd_data,
    bool *copy_flag) = {
    [HDCDRV_CMD_CLIENT_DESTROY] = hdcdrv_cmd_client_destroy,
    [HDCDRV_CMD_SERVER_CREATE] = hdcdrv_cmd_server_create,
    [HDCDRV_CMD_SERVER_DESTROY] = hdcdrv_cmd_server_destroy,
    [HDCDRV_CMD_GET_SESSION_UID] = hdcdrv_cmd_get_uid_state,
    [HDCDRV_CMD_CONNECT] = hdcdrv_cmd_alloc_connect_session,
    [HDCDRV_CMD_ACCEPT] = hdcdrv_cmd_alloc_accept_session,
    [HDCDRV_CMD_CLOSE] = hdcdrv_cmd_close,
    [HDCDRV_CMD_CONFIG] = hdcdrv_cmd_config,
    [HDCDRV_CMD_GET_PEER_DEV_ID] = hdcdrv_cmd_get_peer_dev_id,
    [HDCDRV_CMD_GET_SESSION_INFO] = hdcdrv_cmd_get_session_info,
    [HDCDRV_CMD_GET_SESSION_ATTR] = hdcdrv_cmd_get_session_attr,
};
#define HDCDRV_CMD_MAX_OPERATION sizeof(hdcdrv_operation) / sizeof(hdcdrv_operation[0])

STATIC long hdcdrv_ioctl_com(struct hdcdrv_ctx *ctx, unsigned int cmd, union hdcdrv_cmd *cmd_data,
    bool *copy_flag)
{
    u32 index = _KA_IOC_NR(cmd);
    if (index >= HDCDRV_CMD_MAX_OPERATION) {
        hdcdrv_err("Invalid hdc cmd. (cmd=%u)\n", cmd);
        return -HDCDRV_PARA_ERR;
    }
    if (hdcdrv_operation[index] == NULL) {
        hdcdrv_err("Undefine operation. (cmd=%u)\n", index);
        return -HDCDRV_PARA_ERR;
    }
    return hdcdrv_operation[index](ctx, cmd_data, copy_flag);
}

STATIC long hdcdrv_ioctl(ka_file_t *file, unsigned int cmd, unsigned long arg)
{
    void __ka_user *argp = (void __ka_user *)((uintptr_t)arg);
    struct hdcdrv_ctx *ctx = NULL;
    u32 drv_cmd = _KA_IOC_NR(cmd);
    bool copy_flag = false;
    union hdcdrv_cmd cmd_data;
    u32 vfid = 0;
    long ret;
    int link_type;

    if ((file == NULL) || (argp == NULL)) {
        hdcdrv_err("Input parameter is error.\n");
        return -HDCDRV_PARA_ERR;
    }

    if (ka_base_copy_from_user(&cmd_data, argp, sizeof(cmd_data)) != 0) {
        hdcdrv_err("Calling ka_base_copy_from_user failed. (cmd=%u)\n", drv_cmd);
        return -HDCDRV_COPY_FROM_USER_FAIL;
    }

    ctx = (struct hdcdrv_ctx *)ka_fs_get_file_private_data(file);
    cmd_data.cmd_com.pid = (u64)ka_task_get_current_tgid();
    ret = hdcdrv_ub_convert_id_from_vir_to_phy(drv_cmd, &cmd_data, &vfid);
    if (ret != 0) {
        hdcdrv_err("Convert logical id to physical id failed.\n");
        return ret;
    }

    link_type = devdrv_get_global_connect_protocol();
    if (link_type != DEVDRV_COMMNS_UB) {
        hdcdrv_err("Current connection protocol is not ub.(link_type=%d)\n", link_type);
        return -HDCDRV_NOT_SUPPORT;
    }

    ret = hdcdrv_ioctl_com(ctx, cmd, &cmd_data, &copy_flag);
    if (ret != HDCDRV_OK) {
        return ret;
    }

    if ((ret == HDCDRV_OK) && copy_flag) {
        if (ka_base_copy_to_user(argp, &cmd_data, sizeof(cmd_data)) != 0) {
            hdcdrv_err("ka_base_copy_to_user failed. (drv_cmd=%d)\n", drv_cmd);
            return -HDCDRV_COPY_TO_USER_FAIL;
        }
    }

    return HDCDRV_OK;
}

STATIC struct hdcdrv_ctx *hdcdrv_alloc_ctx(void)
{
    struct hdcdrv_ctx *ctx = ka_mm_kzalloc(sizeof(struct hdcdrv_ctx), KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (ctx != NULL) {
        ctx->service_type = HDCDRV_SERVICE_TYPE_INVALID;
        ctx->session_fd = HDCDRV_SESSION_FD_INVALID;
        ka_task_mutex_init(&ctx->ctx_lock);
    }

    return ctx;
}

STATIC void hdcdrv_free_ctx(const struct hdcdrv_ctx *ctx)
{
    ka_mm_kfree(ctx);
    ctx = NULL;
}

STATIC void hdcdrv_release_close_session(struct hdcdrv_ctx *ctx)
{
    int ret = 0;
    union hdcdrv_cmd close_cmd;
    struct hdcdrv_session *session = NULL;
    bool copy_flag;
    int remote_fd;

    ka_task_mutex_lock(&ctx->ctx_lock);
    if (ctx->session_fd == HDCDRV_SESSION_FD_INVALID) {
        ka_task_mutex_unlock(&ctx->ctx_lock);
        return;
    }
    close_cmd.close.session = ctx->session_fd;
    ka_task_mutex_unlock(&ctx->ctx_lock);

    session = hdcdrv_get_session(ctx->dev_id, ctx->session_fd);
    if (session == NULL) {
        return;
    }
    remote_fd = session->remote_session_fd;
    hdcdrv_info("Release session. (dev=%d; task_pid=%llu; local session_id=%d; remote_id=%d)\n",
        ctx->dev_id, ctx->pid, close_cmd.close.session, remote_fd);
    close_cmd.close.pid = ctx->pid;
    close_cmd.close.unique_val = session->unique_val;
    close_cmd.close.local_close_state = HDCDRV_CLOSE_TYPE_RELEASE;
    close_cmd.close.dev_id = ctx->dev_id;
    ret = (int)hdcdrv_cmd_close(ctx, &close_cmd, &copy_flag);
    if (ret != HDCDRV_OK) {
        hdcdrv_warn("Close session not success. (dev=%d; task_pid=%llu; local_id=%d; remote_id=%d)\n",
            ctx->dev_id, ctx->pid, close_cmd.close.session, remote_fd);
    }
    hdcdrv_put_session(ctx->dev_id);
    return;
}

STATIC void hdcdrv_release_destroy_server(struct hdcdrv_ctx *ctx)
{
    int ret = 0;
    union hdcdrv_cmd destroy_cmd;
    bool copy_flag;

    ka_task_mutex_lock(&ctx->ctx_lock);
    if (ctx->service_type == HDCDRV_SERVICE_TYPE_INVALID) {
        ka_task_mutex_unlock(&ctx->ctx_lock);
        return;
    }
    destroy_cmd.server_destroy.service_type = ctx->service_type;
    ka_task_mutex_unlock(&ctx->ctx_lock);

    /* service free */

    if (g_hdc_ctrl.service_attr[ctx->service_type].log_limit != HDCDRV_SERVICE_LOG_LIMIT) {
        hdcdrv_info("Release server. (task_pid=%llu; dev_id=%d; service_type=\"%s\")\n",
            ctx->pid, ctx->dev_id, hdcdrv_sevice_str(destroy_cmd.server_destroy.service_type));
    }
    destroy_cmd.server_destroy.dev_id = ctx->dev_id;
    destroy_cmd.server_destroy.pid = ctx->pid;
    ret = (int)hdcdrv_cmd_server_destroy(ctx, &destroy_cmd, &copy_flag);
    if (ret != HDCDRV_OK) {
        hdcdrv_warn("Release server. (task_pid=%llu; dev_id=%d; service_type=\"%s\")\n",
            ctx->pid, ctx->dev_id, hdcdrv_sevice_str(destroy_cmd.server_destroy.service_type));
    }
    ctx->service_type = HDCDRV_SERVICE_TYPE_INVALID;

    return;
}

STATIC int hdcdrv_open(ka_inode_t *node, ka_file_t *file)
{
    struct hdcdrv_ctx *ctx = hdcdrv_alloc_ctx();
    if (ctx == NULL) {
        hdcdrv_err("Alloc hdcdrv ctx failed.\n");
        return -HDCDRV_MEM_ALLOC_FAIL;
    }

    ka_fs_set_file_private_data(file, ctx);

    return HDCDRV_OK;
}

STATIC int hdcdrv_release(ka_inode_t *node, ka_file_t *file)
{
    struct hdcdrv_ctx *ctx = ka_fs_get_file_private_data(file);

    if (ctx == NULL) {
        hdcdrv_err("Input parameter is error. (task pid=%llu)", (u64)ka_task_get_current_tgid());
        return -HDCDRV_PARA_ERR;
    }

    hdcdrv_release_close_session(ctx);
    hdcdrv_release_destroy_server(ctx);
    hdcdrv_clear_mem_pool_by_ctx(ctx);
    hdcdrv_free_ctx(ctx);
    ka_fs_set_file_private_data(file, NULL);

    return HDCDRV_OK;
}

STATIC ka_vm_fault_t hdcdrv_vm_ops_fault(ka_vm_fault_struct_t *vmf)
{
    /* Page fault interrupt mapping pa is not supported for hdc mmap */
    hdcdrv_warn("hdc mmap not supported page fault");
    return HDCDRV_FAULT_ERROR;
}

/* mmap not support mremap for safe */
STATIC int hdcdrv_vm_ops_mremap(ka_vm_area_struct_t *vma)
{
    return -EFAULT;
}

STATIC void hdcdrv_mmap_init_vm_struct(ka_vm_operations_struct_t *ops_managed)
{
    ops_managed->fault = hdcdrv_vm_ops_fault;
    ops_managed->mremap = hdcdrv_vm_ops_mremap;
}

STATIC int hdcdrv_mmap_param_check(ka_file_t *filep, ka_vm_area_struct_t *vma)
{
    if (filep == NULL) {
        hdcdrv_err("filep check failed\n");
        return -ENODEV;
    }

    if (ka_fs_get_file_private_data(filep) == NULL) {
        hdcdrv_err("ka_fs_get_file_private_data is NULL\n");
        return -EFAULT;
    }

    if ((vma == NULL) || (ka_mm_get_vm_end(vma) <= ka_mm_get_vm_start(vma))) {
        hdcdrv_err("vm range check failed.\n");
        return -EINVAL;
    }

    if ((ka_mm_get_vm_end(vma) - ka_mm_get_vm_start(vma)) != HDCDRV_UB_MEM_POOL_LEN) {
        hdcdrv_err("vm range should be 128k, now is %ld\n", (ka_mm_get_vm_end(vma) - ka_mm_get_vm_start(vma)));
        return -EINVAL;
    }

    return 0;
}

STATIC int hdcdrv_mmap(ka_file_t *filep, ka_vm_area_struct_t *vma)
{
    int ret;

    ret = hdcdrv_mmap_param_check(filep, vma);
    if (ret != 0) {
        hdcdrv_err("mmap param check failed. (ret=%d)\n", ret);
        return -EINVAL;
    }

    ka_mm_set_vm_private_data(vma, ka_fs_get_file_private_data(filep));
    hdcdrv_mmap_init_vm_struct(&hdcdrv_ub_vm_ops_managed);
    ka_mm_set_vm_ops(vma, &hdcdrv_ub_vm_ops_managed);
    ka_mm_set_vm_flags(vma, KA_VM_DONTEXPAND | KA_VM_DONTDUMP | KA_VM_DONTCOPY | KA_VM_IO | KA_VM_PFNMAP);

    return HDCDRV_OK;
}

STATIC const ka_file_operations_t hdcdrv_fops = {
    ka_fs_init_f_owner(KA_THIS_MODULE)
    ka_fs_init_f_unlocked_ioctl(hdcdrv_ioctl)
    ka_fs_init_f_open(hdcdrv_open)
    ka_fs_init_f_mmap(hdcdrv_mmap)
    ka_fs_init_f_release(hdcdrv_release)
};

STATIC int hdcdrv_register_cdev(void)
{
    int ret;
    ret = drv_davinci_register_sub_module(HDC_MODULE_NAME, &hdcdrv_fops);
    if (ret != HDCDRV_OK) {
        hdcdrv_err("HDC create char device failed. (ret=%d)\n", ret);
        return ret;
    }

    return HDCDRV_OK;
}

STATIC void hdcdrv_unregister_cdev(void)
{
    int ret;
    ret = drv_ascend_unregister_sub_module(HDC_MODULE_NAME);
    if (ret != 0) {
        hdcdrv_err("Unregister sub module failed. (ret=%d)\n", ret);
        return;
    }
    return;
}

static inline void hdcdrv_service_init(u32 dev_id, struct hdcdrv_service *service)
{
    service->dev_id = dev_id;
    service->listen_pid = HDCDRV_INVALID;
    service->ctx = NULL;
    KA_INIT_LIST_HEAD(&service->serv_list);
    ka_task_mutex_init(&service->mutex);
}

STATIC int hdcdrv_init_service_res(u32 dev_id, int service_type, struct hdcdrv_service *service)
{
    struct hdcdrv_serv_list_node *node;
    ka_list_head_t *pos = NULL;
    ka_list_head_t *n = NULL;
    int i;

    for (i = 0; i < HDCDRV_SERVER_PROCESS_MAX_NUM; ++i) {
        node = (struct hdcdrv_serv_list_node *)ka_mm_kzalloc(sizeof(struct hdcdrv_serv_list_node),
            KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
        if (ka_unlikely(node == NULL)) {
            hdcdrv_err("ka_mm_kzalloc process failed. (dev_id=%u;process_num=%d;server_type=%d)\n",
                dev_id, i, service_type);
            goto serv_init_failed;
        }
        hdcdrv_service_init(dev_id, &node->service);
        node->service.service_scope = service->service_scope;   // Inherit the properties of the server
        ka_list_add(&node->list, &service->serv_list);
    }
    return 0;

serv_init_failed:
#ifndef DRV_UT
    if (!ka_list_empty_careful(&service->serv_list)) {
        ka_list_for_each_safe(pos, n, &service->serv_list) {
            node = ka_list_entry(pos, struct hdcdrv_serv_list_node, list);
            ka_list_del(&node->list);
            ka_mm_kfree(node);
        }
    }
#endif
    return -ENOMEM;
}

STATIC void hdcdrv_uninit_service_res(u32 dev_id, int service_type, struct hdcdrv_service *service)
{
    struct hdcdrv_serv_list_node *node;
    ka_list_head_t *pos = NULL;
    ka_list_head_t *n = NULL;

    if (!ka_list_empty_careful(&service->serv_list)) {
        ka_list_for_each_safe(pos, n, &service->serv_list) {
            node = ka_list_entry(pos, struct hdcdrv_serv_list_node, list);
            ka_list_del(&node->list);
            ka_task_mutex_lock(&node->service.mutex);
            if (node->service.ctx != NULL) {
                node->service.ctx->service_type = HDCDRV_SERVICE_TYPE_INVALID;
            }
            hdcdrv_clear_server_conn_list(&node->service);
            ka_task_mutex_unlock(&node->service.mutex);
            ka_mm_kfree(node);
        }
    }

    ka_task_mutex_lock(&service->mutex);
    if (service->ctx != NULL) {
        service->ctx->service_type = HDCDRV_SERVICE_TYPE_INVALID;
    }
    hdcdrv_clear_server_conn_list(service);
    ka_task_mutex_unlock(&service->mutex);
}

STATIC int hdcdrv_init_dev_service(struct hdcdrv_dev *hdc_dev)
{
    struct hdcdrv_service *service;
    int ret;
    int i;

    for (i = 0; i < HDCDRV_SUPPORT_MAX_SERVICE; ++i) {
        service = &hdc_dev->service[i];
        service->dev_id = hdc_dev->dev_id;
        hdcdrv_service_init(hdc_dev->dev_id, service);
        ret = hdcdrv_init_service_res(hdc_dev->dev_id, i, service);
        if (ret != 0) {
            hdcdrv_err("Init service res. (dev_id=%u;server_type=%d)\n", hdc_dev->dev_id, i);
            goto init_service_res_failed;
        }
    }
    return 0;
init_service_res_failed:
#ifndef DRV_UT
    for (i--; i >= 0; --i) {
        service = &hdc_dev->service[i];
        hdcdrv_uninit_service_res(hdc_dev->dev_id, i, service);
    }
#endif
    return ret;
}

STATIC void hdcdrv_uninit_dev_service(struct hdcdrv_dev *hdc_dev)
{
    struct hdcdrv_service *service;
    int i;

    for (i = HDCDRV_SUPPORT_MAX_SERVICE - 1; i >= 0; --i) {
        service = &hdc_dev->service[i];
        hdcdrv_uninit_service_res(hdc_dev->dev_id, i, service);
    }
}

STATIC void hdcdrv_session_pre_init(struct hdcdrv_session *session)
{
    ka_task_spin_lock_init(&session->lock);
    hdcdrv_set_session_status(session, HDCDRV_SESSION_STATUS_IDLE);
    session->owner_pid = HDCDRV_INVALID_PID;
    session->create_pid = HDCDRV_INVALID_PID;
    session->peer_create_pid = HDCDRV_INVALID_PID;
    session->local_close_state = HDCDRV_CLOSE_TYPE_NONE;
    session->remote_close_state = HDCDRV_CLOSE_TYPE_NONE;
    session->remote_close_flag = false;
}

STATIC int hdcdrv_init_instance(u32 dev_id, ka_device_t *dev)
{
    struct hdcdrv_dev *hdc_dev = NULL;
    int ret = 0;
    int i;

    if (dev_id >= HDCDRV_SUPPORT_MAX_DEV) {
        hdcdrv_err("Invalid dev_id. (dev_id=%u)\n", dev_id);
        return -EINVAL;
    }
    if (hdcdrv_ub_get_dev(dev_id) != NULL) {
        hdcdrv_err("Device is already registered. (dev_id=%u)\n", dev_id);
        hdcdrv_put_dev(dev_id);
        return -EINVAL;
    }

    hdc_dev = (struct hdcdrv_dev *)ka_mm_vzalloc(sizeof(struct hdcdrv_dev));
    if (hdc_dev == NULL) {
        hdcdrv_err("Alloc hdc dev failed. (dev_id=%u)\n", dev_id);
        return -ENOMEM;
    }

    ret = hdcdrv_init_dev_service(hdc_dev);
    if (ret != 0) {
        hdcdrv_err("Init dev service failed. (dev_id=%u;ret=%d)\n", dev_id, ret);
        goto init_service_failed;
    }

    ret = hdcdrv_init_dev_mem_pool(hdc_dev);
    if (ret != 0) {
        hdcdrv_err("Init dev mem pool failed. (dev_id=%u;ret=%d)\n", dev_id, ret);
        goto init_mem_pool_failed;
    }

    hdc_dev->sessions = (struct hdcdrv_session *)ka_mm_vzalloc(sizeof(struct hdcdrv_session) *
        HDCDRV_UB_SINGLE_DEV_MAX_SESSION);
#ifndef DRV_UT
    if (hdc_dev->sessions == NULL) {
        hdcdrv_err("Alloc dev session failed. (dev_id=%u)\n", dev_id);
        ret = -ENOMEM;
        goto alloc_session_failed;
    }
#endif

    for (i = 0; i < HDCDRV_UB_SINGLE_DEV_MAX_SESSION; i++) {
        hdcdrv_session_pre_init(&hdc_dev->sessions[i]);
    }

    hdc_dev->dev_id = dev_id;

    hdcdrv_set_host_pm_or_vm_flag(hdc_dev);
    hdc_dev->valid = HDCDRV_VALID;
    ka_task_mutex_lock(&g_hdc_ctrl.mutex);
    g_hdc_ctrl.dev[dev_id] = hdc_dev;
    ka_base_atomic_set(&hdc_dev->ref, 0);
    g_hdc_ctrl.dev_ref[dev_id]++;
    ka_task_mutex_unlock(&g_hdc_ctrl.mutex);
    return 0;
alloc_session_failed:
    hdcdrv_uninit_dev_mem_pool(hdc_dev);
init_mem_pool_failed:
    hdcdrv_uninit_dev_service(hdc_dev);
init_service_failed:
    ka_mm_vfree(hdc_dev);
    hdc_dev = NULL;
    return ret;
}

STATIC int hdcdrv_uninit_instance(u32 dev_id)
{
    struct hdcdrv_dev *hdc_dev = NULL;
    int i;
    int loop_cnt = 0;

    hdc_dev = hdcdrv_ub_get_dev(dev_id);
    if (hdc_dev == NULL) {
        return -EINVAL;
    }

    ka_task_mutex_lock(&g_hdc_ctrl.dev_lock[dev_id]);
    // Prevent new device calls from entering
    hdc_dev->valid = HDCDRV_INVALID;
    ka_task_mutex_unlock(&g_hdc_ctrl.dev_lock[dev_id]);

    hdcdrv_put_dev(dev_id);
    // Wait for all ioctl/event_handle/ctx finish, wait 200s
    while ((ka_base_atomic_read(&hdc_dev->ref) != 0) && (loop_cnt <= HDCDRV_HOTRESET_CHECK_MAX_CNT)) {
        ka_system_msleep(HDCDRV_HOTRESET_CHECK_DELAY_MS);
        loop_cnt++;
    }

    if (loop_cnt > HDCDRV_HOTRESET_CHECK_MAX_CNT) {
        hdcdrv_warn("ref is not zero in 200s.(dev_id=%u)\n", dev_id);
    }

    ka_task_mutex_lock(&g_hdc_ctrl.dev_lock[dev_id]);
    hdcdrv_uninit_dev_mem_pool(hdc_dev);
    for (i = 0; i < HDCDRV_UB_SINGLE_DEV_MAX_SESSION; i++) {
        if (hdcdrv_get_session_status(&hdc_dev->sessions[i]) == HDCDRV_SESSION_STATUS_CONN) {
            hdcdrv_set_session_status(&hdc_dev->sessions[i], HDCDRV_SESSION_STATUS_IDLE);
            if (hdc_dev->sessions[i].ctx != NULL) {
                hdcdrv_info("session %u is not null\n", i);
                hdcdrv_unbind_session_ctx(&hdc_dev->sessions[i]);
            }
        }
    }
    ka_mm_vfree(hdc_dev->sessions);
    hdc_dev->sessions = NULL;
    hdcdrv_uninit_dev_service(hdc_dev);
    g_hdc_ctrl.dev[dev_id] = NULL;
    ka_mm_vfree(hdc_dev);
    ka_task_mutex_unlock(&g_hdc_ctrl.dev_lock[dev_id]);
    return 0;
}

STATIC int hdcdrv_suspend(u32 dev_id)
{
    return 0;
}

STATIC int hdcdrv_resume(u32 dev_id, ka_device_t *dev)
{
    return 0;
}

int hdcdrv_ub_notifier_func(u32 udevid, enum  uda_notified_action action)
{
    int ret = 0;

    if (udevid >= HDCDRV_SUPPORT_MAX_DEV) {
        hdcdrv_err("Invalid para. (udevid=%u)\n", udevid);
        return -EINVAL;
    }

    if (action == UDA_INIT) {
        ret = hdcdrv_init_instance(udevid, uda_get_device(udevid));
    } else if (action == UDA_UNINIT) {
        ret = hdcdrv_uninit_instance(udevid);
#ifndef DRV_UT
    } else if (action == UDA_SUSPEND) {
        ret = hdcdrv_suspend(udevid);
    } else if (action == UDA_RESUME) {
        ret = hdcdrv_resume(udevid, uda_get_device(udevid));
    } else {
        hdcdrv_warn("notifier action unknown. (udevid=%u; action=%d; ret=%d)\n", udevid, (u32)action, ret);
    }
#else
    }
#endif

    hdcdrv_info("notifier action. (udevid=%u; action=%d; ret=%d)\n", udevid, (u32)action, ret);

    return ret;
}

#define HDCDRV_ESCHED_EVENT_ID EVENT_DRV_MSG
static inline bool is_hdcdrv_event(u32 subevent_id)
{
    if ((subevent_id == DRV_SUBEVENT_HDC_CLOSE_LINK_MSG) || (subevent_id == DRV_SUBEVENT_HDC_CREATE_LINK_MSG)) {
        return true;
    }

    return false;
}

STATIC u64 hdcdrv_get_peer_pid(u32 devid, u64 host_pid, u64 peer_pid, int service_type)
{
    u32 master_pid = (u32)host_pid;
    int local_pid = (int)peer_pid;

    // GLOBAL no need to get peer pid, only one server in one device
    if (g_hdc_ctrl.service_attr[service_type].service_scope == HDCDRV_SERVICE_SCOPE_GLOBAL) {
        return peer_pid;
    }

    if (peer_pid != HDCDRV_INVALID_PEER_PID) {
        return peer_pid;
    }

    if (devdrv_query_process_by_host_pid(master_pid, devid, DEVDRV_PROCESS_CP1, 0, &local_pid) != HDCDRV_OK) {
        hdcdrv_warn("devid %u hostpid %llu get localpid not success\n", devid, host_pid);
        return HDCDRV_INVALID_PEER_PID;
    }

    hdcdrv_dbg("after query, master_pid %u, local_pid %d\n", master_pid, local_pid);

    return (u64)local_pid;
}

STATIC int hdcdrv_notify_msg_connect(u32 devid, struct hdcdrv_event_msg *msg, u64 *pid, u32 *gid)
{
    struct hdcdrv_service *service = NULL;
    int service_type = msg->connect_msg.service_type;
    u64 create_pid = msg->connect_msg.peer_create_pid;
    struct hdcdrv_dev *hdc_dev = NULL;
    int ret;
    struct hdcdrv_conn_info_list *node;
    struct hdcdrv_conn_info_list *list_head_t;

    node = (struct hdcdrv_conn_info_list *)hdcdrv_ub_kvmalloc(sizeof(struct hdcdrv_conn_info_list));
    if (node == NULL) {
        hdcdrv_err("no enough mem for hdc connect.(dev_id=%u)\n", devid);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    ret = hdcdrv_dev_para_check(devid, service_type);
    if (ret != 0) {
        goto conn_free_mem;
    }

    hdc_dev = hdcdrv_ub_get_dev(devid);
    if (hdc_dev == NULL) {
        ret = -HDCDRV_PARA_ERR;
        goto conn_free_mem;
    }

    create_pid = hdcdrv_get_peer_pid(devid, msg->connect_msg.client_pid, create_pid, service_type);
    service = hdcdrv_search_service(hdc_dev, service_type, create_pid);
    if (service == NULL) {
        msg->error_code = HDCDRV_PARA_ERR;
        hdcdrv_err_limit("service_type_invalid. (devid=%u; server_type=%d).\n", devid, service_type);
        ret = -HDCDRV_PARA_ERR;
        goto conn_put_dev;
    }

    ka_task_mutex_lock(&service->mutex);
    if (service->listen_status == HDCDRV_INVALID) {
        ka_task_mutex_unlock(&service->mutex);
        /* send to remote to info local service not listen. */
        msg->error_code = HDCDRV_REMOTE_SERVICE_NO_LISTENING;
        hdcdrv_err_limit("server not create. (devid=%u; server_type=%d).\n", devid, service_type);
        ret = -HDCDRV_REMOTE_SERVICE_NO_LISTENING;
        goto conn_put_dev;
    }

    (void)memcpy_s(&node->conn_info, sizeof(struct hdcdrv_event_connect),
        &msg->connect_msg, sizeof(struct hdcdrv_event_connect));
    // New node should add to the end of list, earlier inserted node should be removed first for accept
    if (service->conn_list == NULL) {
        service->conn_list = node;
    } else {
        list_head_t = service->conn_list;
        while (list_head_t->next != NULL) {
            list_head_t = list_head_t->next;
        }
        list_head_t->next = node;
    }

    *pid = service->listen_pid;
    *gid = service->gid;

    ka_task_mutex_unlock(&service->mutex);
    hdcdrv_put_dev(devid);

    return HDCDRV_OK;

conn_put_dev:
    hdcdrv_put_dev(devid);
conn_free_mem:
    hdcdrv_ub_kvfree(node);
    return ret;
}

STATIC int hdcdrv_notify_msg_connect_reply(u32 devid, struct hdcdrv_event_msg *msg)
{
    struct hdcdrv_session *session = NULL;
    u32 session_fd = msg->connect_msg_reply.client_session;

    session = hdcdrv_get_session(devid, msg->connect_msg_reply.client_session);
    if (session == NULL) {
        hdcdrv_err_limit("get session is failed.(session_fd=%u; dev_id=%u)\n", session_fd, devid);
        return DRV_ERROR_PARA_ERROR;
    }

    ka_task_spin_lock_bh(&session->lock);
    if ((hdcdrv_get_session_status(session) != HDCDRV_SESSION_STATUS_CONN) ||
        (session->unique_val != msg->connect_msg_reply.unique_val)) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_put_session(devid);
        hdcdrv_warn("session has been closed.(session_fd=%u; dev_id=%u)\n", session_fd, devid);
        return -HDCDRV_SESSION_HAS_CLOSED;
    }

    session->remote_session_fd = (int)msg->connect_msg_reply.server_session;
    session->peer_create_pid = msg->connect_msg_reply.server_pid;
    hdcdrv_set_session_run_env_ub(session, msg->connect_msg_reply.run_env);
    ka_task_spin_unlock_bh(&session->lock);
    hdcdrv_put_session(devid);
    return HDCDRV_OK;
}

STATIC bool hdcdrv_check_close_session_info(struct hdcdrv_session *session, struct hdcdrv_event_close *close_msg)
{
    if ((u32)session->remote_session_fd != close_msg->local_session) {
        return false;
    }

    if (session->unique_val != close_msg->unique_val) {
        return false;
    }

    if (session->owner_pid != close_msg->peer_pid) {
        return false;
    }

    if (session->peer_create_pid != close_msg->owner_pid) {
        return false;
    }

    return true;
}

STATIC int hdcdrv_notify_msg_close(u32 devid, struct hdcdrv_event_msg *msg)
{
    struct hdcdrv_session *session = NULL;
    u32 session_fd = msg->close_msg.remote_session;

    session = hdcdrv_get_session(devid, session_fd);
    if (session == NULL) {
        hdcdrv_err("session para check fail. (session=%u; dev_id=%u)\n", session_fd, devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    ka_task_spin_lock_bh(&session->lock);
    if (hdcdrv_get_session_status(session) != HDCDRV_SESSION_STATUS_CONN) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_put_session(devid);
        // here notice session has been closed, no need to send event to user
        return SCHED_EVENT_PRE_PROC_SUCCESS_RETURN;
    }

    if (!hdcdrv_check_close_session_info(session, &msg->close_msg)) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_warn("session info check not success, session may have been closed. (msg_info local_id=%u; "
            "remote_id=%u; dev_id=%u; owner_pid=%llu)\n", msg->close_msg.local_session, devid,
            msg->close_msg.remote_session, msg->close_msg.owner_pid);
        hdcdrv_put_session(devid);
        // here notice session has been closed, no need to send event to user
        return SCHED_EVENT_PRE_PROC_SUCCESS_RETURN;
    }
    session->remote_close_flag = false;
    ka_task_spin_unlock_bh(&session->lock);
    hdcdrv_put_session(devid);
    return HDCDRV_OK;
}

STATIC bool hdcdrv_check_dfx_session_info(struct hdcdrv_session *session, struct hdcdrv_session_para_info *para_info,
    unsigned int session_fd)
{
    if ((unsigned int)session->remote_session_fd != session_fd) {
        return false;
    }

    if (session->unique_val != para_info->unique_val) {
        return false;
    }

    if (session->owner_pid != para_info->peer_pid) {
        return false;
    }

    if (session->peer_create_pid != para_info->owner_pid) {
        return false;
    }

    return true;
}

STATIC int hdcdrv_notify_msg_dfx(u32 devid, struct hdcdrv_event_msg *msg)
{
    struct hdcdrv_session *session = NULL;
    u32 session_fd = msg->dfx_msg.r_session_id;

    session = hdcdrv_get_session(devid, session_fd);
    if (session == NULL) {
        hdcdrv_err("session para check fail. (session=%u; dev_id=%u)\n", session_fd, devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    ka_task_spin_lock_bh(&session->lock);
    if (hdcdrv_get_session_status(session) != HDCDRV_SESSION_STATUS_CONN) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_put_session(devid);
        // here notice session has been closed, no need to send event to user
        return -HDCDRV_SESSION_HAS_CLOSED;
    }

    if (!hdcdrv_check_dfx_session_info(session, &msg->dfx_msg.para_info, msg->dfx_msg.l_session_id)) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_warn("session info check not success, session may have been closed. (msg_info local_id=%u; "
            "remote_id=%u; dev_id=%u; owner_pid=%llu)\n", msg->close_msg.local_session, devid,
            msg->close_msg.remote_session, msg->close_msg.owner_pid);
        hdcdrv_put_session(devid);
        // here notice session has been closed, no need to send event to user
        return -HDCDRV_SESSION_HAS_CLOSED;
    }

    ka_task_spin_unlock_bh(&session->lock);
    hdcdrv_put_session(devid);
    return HDCDRV_OK;
}

STATIC int hdcdrv_notify_msg_dfx_reply(u32 devid, struct hdcdrv_event_msg *msg)
{
    struct hdcdrv_session *session = NULL;
    u32 session_fd = msg->dfx_msg_reply.l_session_id;

    session = hdcdrv_get_session(devid, session_fd);
    if (session == NULL) {
        hdcdrv_err("session para check fail. (session=%u; dev_id=%u)\n", session_fd, devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    ka_task_spin_lock_bh(&session->lock);
    if (hdcdrv_get_session_status(session) != HDCDRV_SESSION_STATUS_CONN) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_put_session(devid);
        // here notice session has been closed, no need to send event to user
        return -HDCDRV_SESSION_HAS_CLOSED;
    }

    if (!hdcdrv_check_dfx_session_info(session, &msg->dfx_msg_reply.para_info, msg->dfx_msg_reply.r_session_id)) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_warn("session info check not success, session may have been closed. (msg_info local_id=%u; "
            "remote_id=%u; dev_id=%u; owner_pid=%llu)\n", msg->close_msg.local_session, devid,
            msg->close_msg.remote_session, msg->close_msg.owner_pid);
        hdcdrv_put_session(devid);
        // here notice session has been closed, no need to send event to user
        return -HDCDRV_SESSION_HAS_CLOSED;
    }

    ka_task_spin_unlock_bh(&session->lock);
    hdcdrv_put_session(devid);
    return HDCDRV_OK;
}

#define EVENT_DRV_MSG_GRP_NAME "drv_msg_grp"

STATIC int hdcdrv_notify_msg_release(u32 devid,
    struct hdcdrv_event_msg *msg, struct sched_published_event_info *event_info)
{
    u32 session_id = msg->close_msg.remote_session;
    struct hdcdrv_session *session = NULL;
    struct sched_published_event event;
    struct hdcdrv_sync_event_msg event_msg = {0};
    int ret;

    session = hdcdrv_get_session(devid, session_id);
    if (session == NULL) {
        hdcdrv_err_limit("get session is failed. (session_fd=%u; dev_id=%u)\n", session_id, devid);
        return -HDCDRV_PARA_ERR;
    }

    ka_task_spin_lock_bh(&session->lock);
    if (hdcdrv_get_session_status(session) != HDCDRV_SESSION_STATUS_CONN) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_put_session(devid);
        // here notice session has been closed, no need to send event to user
        return SCHED_EVENT_PRE_PROC_SUCCESS_RETURN;
    }

    if (!hdcdrv_check_close_session_info(session, &msg->close_msg)) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_warn("session info check not success, session may have been closed. (msg_info local_id=%u; "
            "remote_id=%u; dev_id=%u; owner_pid=%llu)\n", msg->close_msg.local_session, devid,
            msg->close_msg.remote_session, msg->close_msg.owner_pid);
        hdcdrv_put_session(devid);
        // here notice session has been closed, no need to send event to user
        return SCHED_EVENT_PRE_PROC_SUCCESS_RETURN;
    }
    session->remote_close_flag = false;
    event.event_info.pid = (int)session->owner_pid;
    ka_task_spin_unlock_bh(&session->lock);
    hdcdrv_put_session(devid);

    event.event_info.msg_len = sizeof(struct hdcdrv_sync_event_msg);
    event.event_info.msg = (char *)&event_msg;
    event.event_func.event_ack_func = NULL;
    event.event_func.event_finish_func = NULL;
    event.event_info.event_id = EVENT_DRV_MSG;
    event.event_info.subevent_id = DRV_SUBEVENT_HDC_CLOSE_LINK_MSG;
    event.event_info.dst_devid = devid;
    ret = sched_query_local_task_gid(devid, event.event_info.pid, EVENT_DRV_MSG_GRP_NAME, &event.event_info.gid);
    if (ret == DRV_ERROR_NO_PROCESS) {
        // process not exit, session must be closed by release, no need to send msg to user
        return SCHED_EVENT_PRE_PROC_SUCCESS_RETURN;
    } else if (ret != 0) {
        hdcdrv_err("sched_query_local_task_gid failed.(ret=%d; devid=%u).\n", ret, devid);
        return ret;
    }

    msg->type = HDCDRV_NOTIFY_MSG_CLOSE;    // proc of drv_event need type to verify close or dfx or invalid value
    (void)memcpy_s(&event_msg.data, sizeof(struct hdcdrv_event_msg), msg, sizeof(struct hdcdrv_event_msg));

    hdcdrv_notify_msg_release_set_dst_engine(&event);
    ret = hal_kernel_sched_submit_event(devid, &event);
    if (ret != 0) {
        hdcdrv_err("sched_submit_event failed.(ret=%d; devid=%u)\n", ret, devid);
        return ret;
    }

    return SCHED_EVENT_PRE_PROC_SUCCESS_RETURN;
}

STATIC int hdcdrv_event_pre_handle_remote(unsigned int dev_id, struct sched_published_event_info *event_info,
    struct sched_published_event_func *event_func)
{
    struct hdcdrv_event_msg *msg;
    int ret = HDCDRV_OK;
    u64 pid;
    u32 gid;

    if (event_info == NULL) {
        return -HDCDRV_PARA_ERR;
    }

    if (!is_hdcdrv_event(event_info->subevent_id)) {
        return 0;
    }

    if (event_info->msg == NULL) {
        hdcdrv_err("Msg is NULL. (dev_id=%u)\n", dev_id);
        return -HDCDRV_PARA_ERR;
    }
    if (event_info->msg_len < sizeof(struct event_sync_msg) + sizeof(struct hdcdrv_event_msg)) {
        hdcdrv_err("Msg len is invalid. (msg len=%u)\n", event_info->msg_len);
        return -HDCDRV_PARA_ERR;
    }

    pid = (u64)event_info->pid;
    msg = (struct hdcdrv_event_msg *)(event_info->msg + sizeof(struct event_sync_msg));
    if (msg->type >= HDCDRV_NOTIFY_MSG_MAX) {
        hdcdrv_err("Msg type is invalid. (max_type=%u; now_type=%u)\n", HDCDRV_NOTIFY_MSG_MAX, msg->type);
        return EINVAL;
    }

    switch (msg->type) {
        case HDCDRV_NOTIFY_MSG_CONNECT:
            ret = hdcdrv_notify_msg_connect(dev_id, msg, &pid, &gid);
            event_info->pid = (int)pid;
            event_info->gid = gid;
            break;
        case HDCDRV_NOTIFY_MSG_CONNECT_REPLY:
            ret = hdcdrv_notify_msg_connect_reply(dev_id, msg);
            break;
        case HDCDRV_NOTIFY_MSG_CLOSE:
            ret = hdcdrv_notify_msg_close(dev_id, msg);
            break;
        case HDCDRV_NOTIFY_MSG_CLOSE_REPLY:
            break;
        case HDCDRV_NOTIFY_MSG_CLOSE_RELEASE:
            ret = hdcdrv_notify_msg_release(dev_id, msg, event_info);
            break;
        case HDCDRV_NOTIFY_MSG_DFX:
            ret = hdcdrv_notify_msg_dfx(dev_id, msg);
            break;
        case HDCDRV_NOTIFY_MSG_DFX_REPLY:
            ret = hdcdrv_notify_msg_dfx_reply(dev_id, msg);
            break;
        default:
            break;
    }

    return ret;
}

STATIC int hdcdrv_local_event_process_for_connect(u32 devid, struct hdcdrv_event_msg *msg)
{
    u32 session_id = msg->connect_msg.client_session;
    u64 owner_pid = HDCDRV_INVALID_OWNER_PID;
    struct hdcdrv_session *session = NULL;
    int ret, status;

    session = hdcdrv_get_session(devid, session_id);
    if (session == NULL) {
        hdcdrv_err_limit("get session is failed. (session_fd=%d; dev_id=%u)\n", msg->close_msg.remote_session, devid);
        return -HDCDRV_PARA_ERR;
    }

    ka_task_spin_lock_bh(&session->lock);
    ret = hdcdrv_local_session_check(session, &status, &owner_pid);
    if (ret != HDCDRV_OK) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_put_session(devid);
        hdcdrv_warn("session has been closed or pid has problem.(session_fd=%u; dev_id=%u; status=%d; owner_pid=%llu; "
            "current_pid=%u; ret=%d)\n", session_id, devid, status, owner_pid, ka_task_get_current_tgid(), ret);
        return ret;
    }

    msg->connect_msg.root_privilege = session->root_privilege;
    msg->connect_msg.euid = session->euid;
    msg->connect_msg.uid = session->uid;
    msg->connect_msg.client_pid = (unsigned long long)ka_task_get_current_tgid();
    msg->connect_msg.unique_val = session->unique_val;
    // if local side is host, need to get env and privilege in this process, else do nothing
    hdcdrv_set_msg_env(&(msg->connect_msg.run_env), session->run_env);
    ka_task_spin_unlock_bh(&session->lock);
    hdcdrv_put_session(devid);
    return 0;
}

STATIC int hdcdrv_local_event_process_for_connect_reply(u32 devid, struct hdcdrv_event_msg *msg)
{
    // if local side is host, need to get env and privilege in this process, else do nothing
    u32 session_id = msg->connect_msg_reply.server_session;
    u64 owner_pid = HDCDRV_INVALID_OWNER_PID;
    struct hdcdrv_session *session = NULL;
    int ret, status;

    session = hdcdrv_get_session(devid, session_id);
    if (session == NULL) {
        hdcdrv_err_limit("get session is failed. (session_fd=%d; dev_id=%u)\n", session_id, devid);
        return -HDCDRV_PARA_ERR;
    }

    ka_task_spin_lock_bh(&session->lock);
    ret = hdcdrv_local_session_check(session, &status, &owner_pid);
    if (ret != HDCDRV_OK) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_put_session(devid);
        hdcdrv_warn("session has been closed or pid has problem.(session_fd=%u; dev_id=%u; status=%d; owner_pid=%llu; "
            "current_pid=%u; ret=%d)\n", session_id, devid, status, owner_pid, ka_task_get_current_tgid(), ret);
        return ret;
    }

    msg->connect_msg_reply.unique_val = session->unique_val;
    msg->connect_msg_reply.server_pid = (unsigned long long)ka_task_get_current_tgid();
    hdcdrv_set_msg_env(&(msg->connect_msg_reply.run_env), session->run_env);
    ka_task_spin_unlock_bh(&session->lock);
    hdcdrv_put_session(devid);
    return 0;
}

STATIC int hdcdrv_local_session_check_for_close_dfx(struct hdcdrv_session *session, int *status, u64 *owner_pid,
    u64 remote_pid)
{
    int ret;

    ret = hdcdrv_local_session_check(session, status, owner_pid);
    if (ret != 0) {
        return ret;
    }

    if (remote_pid != session->peer_create_pid) {
        return -HDCDRV_NO_PERMISSION;
    }

    return HDCDRV_OK;
}

STATIC int hdcdrv_local_event_process_for_close(u32 devid, struct hdcdrv_event_msg *msg, int remote_pid)
{
    // if local side is host, need to get env and privilege in this process, else do nothing
    u32 session_id = msg->close_msg.local_session;
    u64 owner_pid = HDCDRV_INVALID_OWNER_PID;
    struct hdcdrv_session *session = NULL;
    int ret, status;

    session = hdcdrv_get_session(devid, session_id);
    if (session == NULL) {
        hdcdrv_err_limit("get session is failed. (session_fd=%d; dev_id=%u)\n", session_id, devid);
        return -HDCDRV_PARA_ERR;
    }

    ka_task_spin_lock_bh(&session->lock);
    ret = hdcdrv_local_session_check_for_close_dfx(session, &status, &owner_pid, remote_pid);
    if (ret != HDCDRV_OK) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_put_session(devid);
        hdcdrv_warn("session has been closed or pid has problem.(session_fd=%u; dev_id=%u; status=%d; owner_pid=%llu; "
            "current_pid=%u; ret=%d)\n", session_id, devid, status, owner_pid, ka_task_get_current_tgid(), ret);
        return ret;
    }

    msg->close_msg.peer_pid = session->peer_create_pid;
    msg->close_msg.owner_pid = session->owner_pid;
    msg->close_msg.unique_val = session->unique_val;
    msg->close_msg.local_session = (unsigned int)session->local_session_fd;
    msg->close_msg.remote_session = (unsigned int)session->remote_session_fd;

    ka_task_spin_unlock_bh(&session->lock);
    hdcdrv_put_session(devid);
    return 0;
}

STATIC int hdcdrv_local_event_process_for_dfx(u32 devid, struct hdcdrv_event_msg *msg, int remote_pid)
{
    /* for dfx and dfx_reply, l_session_id means the side which need to query dfx,
        r_session_id means the side which need to give dfx info to remote */
    u32 session_id = msg->dfx_msg.l_session_id;
    u64 owner_pid = HDCDRV_INVALID_OWNER_PID;
    struct hdcdrv_session *session = NULL;
    int ret, status;

    session = hdcdrv_get_session(devid, session_id);
    if (session == NULL) {
        hdcdrv_err_limit("get session is failed. (session_fd=%d; dev_id=%u)\n", session_id, devid);
        return -HDCDRV_PARA_ERR;
    }

    ka_task_spin_lock_bh(&session->lock);
    ret = hdcdrv_local_session_check_for_close_dfx(session, &status, &owner_pid, remote_pid);
    if (ret != HDCDRV_OK) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_put_session(devid);
        hdcdrv_warn("session has been closed or pid has problem.(session_fd=%u; dev_id=%u; status=%d; owner_pid=%llu; "
            "current_pid=%u; ret=%d)\n", session_id, devid, status, owner_pid, ka_task_get_current_tgid(), ret);
        return ret;
    }

    msg->dfx_msg.r_session_id = (u32)session->remote_session_fd;
    msg->dfx_msg.para_info.peer_pid = session->peer_create_pid;
    msg->dfx_msg.para_info.owner_pid = session->owner_pid;
    msg->dfx_msg.para_info.unique_val = session->unique_val;
    ka_task_spin_unlock_bh(&session->lock);
    hdcdrv_put_session(devid);
    return 0;
}

STATIC int hdcdrv_local_event_process_for_dfx_reply(u32 devid, struct hdcdrv_event_msg *msg, int remote_pid)
{
    /* for dfx and dfx_reply, l_session_id means the side which need to query dfx,
        r_session_id means the side which need to give dfx info to remote */
    u32 session_id = msg->dfx_msg_reply.r_session_id;
    u64 owner_pid = HDCDRV_INVALID_OWNER_PID;
    struct hdcdrv_session *session = NULL;
    int ret, status;

    session = hdcdrv_get_session(devid, session_id);
    if (session == NULL) {
        hdcdrv_err_limit("get session is failed. (session_fd=%d; dev_id=%u)\n", session_id, devid);
        return -HDCDRV_PARA_ERR;
    }

    ka_task_spin_lock_bh(&session->lock);
    ret = hdcdrv_local_session_check_for_close_dfx(session, &status, &owner_pid, remote_pid);
    if (ret != HDCDRV_OK) {
        ka_task_spin_unlock_bh(&session->lock);
        hdcdrv_put_session(devid);
        hdcdrv_warn("session has been closed or pid has problem.(session_fd=%u; dev_id=%u; status=%d; owner_pid=%llu; "
            "current_pid=%u; ret=%d)\n", session_id, devid, status, owner_pid, ka_task_get_current_tgid(), ret);
        return ret;
    }

    msg->dfx_msg_reply.l_session_id = (u32)session->remote_session_fd;
    msg->dfx_msg_reply.para_info.peer_pid = session->peer_create_pid;
    msg->dfx_msg_reply.para_info.owner_pid = session->owner_pid;
    msg->dfx_msg_reply.para_info.unique_val = session->unique_val;
    ka_task_spin_unlock_bh(&session->lock);
    hdcdrv_put_session(devid);
    return 0;
}

STATIC int hdcdrv_event_pre_handle_local(unsigned int dev_id, struct sched_published_event_info *event_info,
    struct sched_published_event_func *event_func)
{
    struct hdcdrv_event_msg *msg;
    int ret = HDCDRV_OK;

    if (event_info == NULL) {
        return EINVAL;
    }

    if (!is_hdcdrv_event(event_info->subevent_id)) {
        return 0;
    }

    if (event_info->msg == NULL) {
        hdcdrv_err("Msg is NULL. (dev_id=%u)\n", dev_id);
        return EINVAL;
    }

    if (event_info->msg_len < (sizeof(struct event_sync_msg) + sizeof(struct hdcdrv_event_msg))) {
        hdcdrv_err("Msg len is invalid. (msg len=%u)\n", event_info->msg_len);
        return EINVAL;
    }

    msg = (struct hdcdrv_event_msg *)(event_info->msg + sizeof(struct event_sync_msg));
    if (msg->type >= HDCDRV_NOTIFY_MSG_MAX) {
        hdcdrv_err("Msg type is invalid. (max_type=%u; now_type=%u)\n", HDCDRV_NOTIFY_MSG_MAX, msg->type);
        return EINVAL;
    }

    switch (msg->type) {
        case HDCDRV_NOTIFY_MSG_CONNECT:
            ret = hdcdrv_local_event_process_for_connect(dev_id, msg);
            break;
        case HDCDRV_NOTIFY_MSG_CONNECT_REPLY:
            ret = hdcdrv_local_event_process_for_connect_reply(dev_id, msg);
            break;
        case HDCDRV_NOTIFY_MSG_CLOSE:
            ret = hdcdrv_local_event_process_for_close(dev_id, msg, event_info->pid);
            break;
        case HDCDRV_NOTIFY_MSG_DFX:
            ret = hdcdrv_local_event_process_for_dfx(dev_id, msg, event_info->pid);
            break;
        case HDCDRV_NOTIFY_MSG_DFX_REPLY:
            ret = hdcdrv_local_event_process_for_dfx_reply(dev_id, msg, event_info->pid);
            break;
        default:
            break;
    }

    return ret;
}

// register handle to esched, local is used when submit, remote is used when wait
STATIC int hdcdrv_init_esched(void)
{
    int ret;

    ret = hal_kernel_sched_register_event_pre_proc_handle(HDCDRV_ESCHED_EVENT_ID,
        SCHED_PRE_PROC_POS_LOCAL, hdcdrv_event_pre_handle_local);
    if (ret != 0) {
        hdcdrv_err("Register local event pre proc handler failed.\n");
        return ret;
    }

    ret = hal_kernel_sched_register_event_pre_proc_handle(HDCDRV_ESCHED_EVENT_ID,
        SCHED_PRE_PROC_POS_REMOTE, hdcdrv_event_pre_handle_remote);
    if (ret != 0) {
        hal_kernel_sched_unregister_event_pre_proc_handle(HDCDRV_ESCHED_EVENT_ID, SCHED_PRE_PROC_POS_LOCAL,
            hdcdrv_event_pre_handle_local);
        hdcdrv_err("Register remote event pre proc handler failed.\n");
        return ret;
    }
    return 0;
}

void hdcdrv_uninit_esched(void)
{
    hal_kernel_sched_unregister_event_pre_proc_handle(HDCDRV_ESCHED_EVENT_ID, SCHED_PRE_PROC_POS_REMOTE,
        hdcdrv_event_pre_handle_remote);
    hal_kernel_sched_unregister_event_pre_proc_handle(HDCDRV_ESCHED_EVENT_ID, SCHED_PRE_PROC_POS_LOCAL,
        hdcdrv_event_pre_handle_local);
    return;
}

STATIC int hdcdrv_ctrl_init(struct hdcdrv_ctrl *hdc_ctrl)
{
    int i;

    ka_task_mutex_init(&hdc_ctrl->mutex);
    hdcdrv_service_attr_init(hdc_ctrl);

    for (i = 0; i < HDCDRV_SUPPORT_MAX_DEV; i++) {
        ka_task_mutex_init(&hdc_ctrl->dev_lock[i]);
        hdc_ctrl->dev[i] = NULL;
        hdc_ctrl->dev_ref[i] = 0;
    }
    return 0;
}

STATIC void hdcdrv_ctrl_uninit(void)
{
    int i;

    for (i = 0; i < HDCDRV_SUPPORT_MAX_DEV; i++) {
        (void)hdcdrv_uninit_instance(i);
    }
}

STATIC int hdcdrv_init(void)
{
    int ret;

    hdcdrv_fill_service_type_str();

    ret = hdcdrv_ctrl_init(&g_hdc_ctrl);
    if (ret != 0) {
        hdcdrv_err("g_hdcdrv_ctrl init failed. (ret=%d)\n", ret);
        return ret;
    }
#ifndef DRV_UT
    if (hdcdrv_register_cdev() != 0) {
        hdcdrv_err("Register cdev failed.\n");
        hdcdrv_ctrl_uninit();
        return -ENOMEM;
    }
#endif
    return HDCDRV_OK;
}

void hdcdrv_uninit_ub(void)
{
    hdcdrv_unregister_cdev();
    hdcdrv_ctrl_uninit();
}

int hdcdrv_ub_init_module(void)
{
    struct uda_dev_type type;
    int ret;

    ret = hdcdrv_init();
    if (ret != 0) {
        hdcdrv_err("hdcdrv init failed. (ret=%d)\n", ret);
        return ret;
    }

    ret = hdcdrv_init_esched();
    if (ret != 0) {
#ifndef DRV_UT
        hdcdrv_err("hdcdrv init esched failed. (ret=%d)\n", ret);
        hdcdrv_uninit_ub();
#endif
        return ret;
    }

    uda_davinci_real_entity_type_pack_proc(&type);
    ret = uda_notifier_register(HDCDRV_UB_NOTIFIER, &type, UDA_PRI1, hdcdrv_ub_notifier_func);
    if (ret != HDCDRV_OK) {
#ifndef DRV_UT
        hdcdrv_uninit_esched();
        hdcdrv_uninit_ub();
        hdcdrv_err("Register uda notifier failed. (ret=%d)\n", ret);
#endif
        return ret;
    }

    return uda_notifier_register_proc(&type);
}

void hdcdrv_ub_exit_module(void)
{
    struct uda_dev_type type;

#ifndef EMU_ST
    (void)module_feature_auto_uninit();
#endif

    uda_davinci_real_entity_type_pack_proc(&type);
    (void)uda_notifier_unregister(HDCDRV_UB_NOTIFIER, &type);
    hdcdrv_uninit_esched();
    hdcdrv_uninit_ub();
}
