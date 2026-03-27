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
#include "ka_base_pub.h"
#include "ka_task_pub.h"
#include "ka_common_pub.h"
#include "ka_fs_pub.h"
#include "ka_system_pub.h"
#include "ka_compiler_pub.h"

#include "ascend_hal_define.h"
#include "esched_kernel_interface.h"
#include "pbl_feature_loader.h"
#include "pbl_task_ctx.h"
#include "pbl_uda.h"

#include "svm_kern_log.h"
#include "framework_task.h"
#include "framework_cmd.h"
#include "svm_ioctl_ex.h"
#include "svm_sub_event_type.h"
#include "mem_show_msg.h"
#include "svm_user_feature_show.h"

struct svm_mem_show_msg_sync {
    struct event_sync_msg sync_head;
    struct svm_mem_show_msg msg;
};

struct svm_user_feature {
    char *feature_name;
    u32 task_feature_id;
};

static struct svm_user_feature user_features[] = {
    {USER_FEATURE_MALLOC_MNG, 0},
    {USER_FEATURE_CACHE_MALLOC, 0},
    {USER_FEATURE_PREFETCH, 0},
    {USER_FEATURE_REGISTER, 0},
    {USER_FEATURE_MEM_STAT, 0},
};

static u32 user_feature_num = USER_FEATURE_SHOW_NUM;

static ka_mutex_t user_feature_mutex;
static char feature_show_buf[SVM_MEM_SHOW_BUF_LEN];

static int user_feature_show_ioctl_ack(u32 udevid, u32 cmd, unsigned long arg)
{
    void *task_ctx = NULL;
    int tgid = ka_task_get_current_tgid();
    int ret;

    task_ctx = svm_task_ctx_get(udevid, tgid);
    if (task_ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EINVAL;
    }

    ret = ka_base_copy_from_user(feature_show_buf, (void __ka_user *)arg, SVM_MEM_SHOW_BUF_LEN - 1U);
    svm_task_ctx_put(task_ctx);
    if (ret != 0) {
        svm_err("copy_from_user failed. (ret=%d)\n", ret);
    }

    return ret;
}

int user_feature_init_task(u32 udevid, int tgid, void *start_time)
{
    void *task_ctx = NULL;
    u32 i;

    task_ctx = svm_task_ctx_get(udevid, tgid);
    if (task_ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EINVAL;
    }

    for (i = 0; i < user_feature_num; i++) {
        struct svm_user_feature *feature = &user_features[i];
        int ret = svm_task_set_feature_priv(task_ctx, feature->task_feature_id, feature->feature_name, NULL, NULL);
        if (ret != 0) {
            svm_warn("Set feature priv failed. (udevid=%u; tgid=%d; task_feature_id=%d; feature_name=%s)\n",
                udevid, tgid, feature->task_feature_id, feature->feature_name);
        }
    }

    svm_task_ctx_put(task_ctx);

    return 0;
}
DECLAER_FEATURE_AUTO_INIT_TASK(user_feature_init_task, FEATURE_LOADER_STAGE_6);

void user_feature_uninit_task(u32 udevid, int tgid, void *start_time)
{
    void *task_ctx = NULL;

    task_ctx = svm_task_ctx_get(udevid, tgid);
    if (task_ctx == NULL) {
        return;
    }

    if (!svm_task_is_exit_abort(task_ctx)) {
        int i;
        for (i = 0; i < user_feature_num; i++) {
            struct svm_user_feature *feature = &user_features[i];
            svm_task_set_feature_invalid(task_ctx, feature->task_feature_id);
        }
    }

    svm_task_ctx_put(task_ctx);
}
DECLAER_FEATURE_AUTO_UNINIT_TASK(user_feature_uninit_task, FEATURE_LOADER_STAGE_6);

static char *user_user_feature_get_name(int feature_id)
{
    u32 i;

    for (i = 0; i < user_feature_num; i++) {
        struct svm_user_feature *feature = &user_features[i];
        if (feature->task_feature_id == (u32)feature_id) {
            return feature->feature_name;
        }
    }

    return NULL;
}

static int user_feature_trigger_event(u32 udevid, int tgid, u32 show_type, char *feature_name)
{
    struct sched_published_event publish_event;
    struct svm_mem_show_msg_sync show_msg_sync;
    struct svm_mem_show_msg *msg = &show_msg_sync.msg;
    int ret;

    ret = sched_query_local_task_gid(udevid, tgid, EVENT_DRV_MSG_GRP_NAME, &publish_event.event_info.gid);
    if (ret != 0) {
        svm_warn("Get gid failed. (udevid=%u; tgid=%d; feature=%s)\n", udevid, tgid, feature_name);
        return ret;
    }

    publish_event.event_info.dst_engine = CCPU_LOCAL;
    publish_event.event_info.policy = 0;
    publish_event.event_info.pid = tgid;
    publish_event.event_info.event_id = EVENT_DRV_MSG;
    publish_event.event_info.subevent_id = SVM_MEM_SHOW_EVENT;

    strcpy_s(msg->feature_name, FEARURE_NAME_LEN, feature_name);
    msg->type = show_type;

    publish_event.event_info.msg = (char *)&show_msg_sync;
    publish_event.event_info.msg_len = sizeof(show_msg_sync);

    publish_event.event_func.event_finish_func = NULL;
    publish_event.event_func.event_ack_func = NULL;

    ret = hal_kernel_sched_submit_event(udevid, &publish_event);
    if (ret != 0) {
        svm_warn("Submit event failed. (udevid=%u; tgid=%d; feature=%s)\n", udevid, tgid, feature_name);
    }

    return ret;
}

static void user_feature_show(u32 udevid, int tgid, char *feature_name, u32 show_type, ka_seq_file_t *seq)
{
    struct svm_mem_show_buf_head *buf_head = (struct svm_mem_show_buf_head *)feature_show_buf;
    int ret, retry_cnt = 3000; /* wait max 3000 ms */

    buf_head->valid = 0;
    ret = user_feature_trigger_event(udevid, tgid, show_type, feature_name);
    if (ret != 0) {
        ka_fs_seq_printf(seq, "notice user failed. (ret=%d; udevid=%u; tgid=%d; feature=%s)\n", ret, udevid, tgid, feature_name);
        return;
    }

    feature_show_buf[SVM_MEM_SHOW_BUF_LEN - 1U] = '\0';
    while (retry_cnt-- > 0) {
        if (buf_head->valid == 1) {
            ka_fs_seq_printf(seq, "user feature %s:\n", feature_name);
            ka_fs_seq_printf(seq, "%s\n", buf_head->data);
            break;
        }

        ka_system_msleep(1);
    }

    if (retry_cnt <= 0) {
        ka_fs_seq_printf(seq, "read response timeout. (udevid=%u; tgid=%d; feature=%s)\n", udevid, tgid, feature_name);
    }
}

static u32 user_feature_find_first_agent_udevid(int tgid)
{
    u32 udev_max_num = uda_get_udev_max_num();
    u32 udevid;

    for (udevid = 0; udevid < udev_max_num; udevid++) {
        void *task_ctx = svm_task_ctx_get(udevid, tgid);
        if (task_ctx != NULL) {
            svm_task_ctx_put(task_ctx);
            break;
        }
    }

    return udevid;
}

void user_feature_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq)
{
    void *task_ctx = NULL;
    char *feature_name = NULL;
    u32 type = (udevid == uda_get_host_id()) ? SVM_SHOW_SCOPE_ALL : SVM_SHOW_SCOPE_DEV;

    feature_name = user_user_feature_get_name(feature_id);
    if (feature_name == NULL) {
        return;
    }

    if (udevid == uda_get_host_id()) {
        udevid = user_feature_find_first_agent_udevid(tgid); /* host dev no drv event thread, use agent udevid */
        if (udevid == uda_get_udev_max_num()) {
            svm_err("No valid dev. (udevid=%u; tgid=%d)\n", udevid, tgid);
            return;
        }
    }

    task_ctx = svm_task_ctx_get(udevid, tgid); /* user buf store in host dev task ctx priv */
    if (task_ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return;
    }

    ka_task_mutex_lock(&user_feature_mutex);
    user_feature_show(udevid, tgid, feature_name, type, seq);
    ka_task_mutex_unlock(&user_feature_mutex);
    svm_task_ctx_put(task_ctx);
}
DECLAER_FEATURE_AUTO_SHOW_TASK(user_feature_show_task, FEATURE_LOADER_STAGE_6);

int svm_user_feature_init(void)
{
    int i;

    ka_task_mutex_init(&user_feature_mutex);
    for (i = 0; i < user_feature_num; i++) {
        user_features[i].task_feature_id = svm_task_obtain_feature_id();
    }

    svm_register_ioctl_cmd_handle(_IOC_NR(SVM_MEM_SHOW_FEATURE_ACK), user_feature_show_ioctl_ack);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(svm_user_feature_init, FEATURE_LOADER_STAGE_6);
