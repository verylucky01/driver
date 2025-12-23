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

#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/atomic.h>
#include <linux/delay.h>
#include <linux/export.h>

#include "securec.h"
#include "ascend_hal_error.h"
#include "pbl/pbl_uda.h"
#include "urd_define.h"
#include "urd_feature.h"
#include "urd_kv.h"
#include "urd_acc_ctrl.h"
#include "urd_init.h"
#include "pbl/pbl_urd.h"

STATIC DMS_FEATURE_NODE_S* feature_get_key_node(const char* key)
{
    DMS_FEATURE_NODE_S* node = NULL;
    int ret;
    ret = dms_kv_get(key, (void*)&node, sizeof(DMS_FEATURE_NODE_S*));
    if ((ret != 0) && (node == NULL)) {
        return NULL;
    }
    return node;
}

STATIC DMS_FEATURE_NODE_S* feature_get_key_node_ex(const char* key)
{
    DMS_FEATURE_NODE_S* node = NULL;
    int ret;
    ret = dms_kv_get_ex(key, (void*)&node, sizeof(DMS_FEATURE_NODE_S*));
    if ((ret != 0) && (node == NULL)) {
        return NULL;
    }
    return node;
}

STATIC int feature_add_key_node(const char* key, DMS_FEATURE_NODE_S* node)
{
    return dms_kv_set(key, (void*)&node, sizeof(DMS_FEATURE_NODE_S*));
}

STATIC int feature_del_key_node(const char* key)
{
    return dms_kv_del(key);
}

STATIC int feature_check_para(DMS_FEATURE_S* feature)
{
    if (feature == NULL) {
        dms_err("Invalid Parameter. (feature=NULL)\n");
        return -EINVAL;
    }

    if (feature->call_fun == NULL) {
        dms_err("Invalid Parameter. (call_fun=NULL)\n");
        return -EINVAL;
    }

    if ((feature->handler_type != URD_HANDLER) && (feature->handler_type != URD_DEV_HANDLER)) {
        dms_err("Invalid Parameter handler_type invalid. (handler_type=%u))\n", feature->handler_type);
        return -EINVAL;
    }

    if (feature->owner_name == NULL) {
        dms_err("Invalid Parameter. (owner_name=NULL)\n");
        return -EINVAL;
    }
    return 0;
}

STATIC DMS_FEATURE_NODE_S* feature_alloc_feature_node(DMS_FEATURE_S* feature)
{
    DMS_FEATURE_NODE_S* node = NULL;
    int ret;
    node = (DMS_FEATURE_NODE_S*)kzalloc(sizeof(DMS_FEATURE_NODE_S), GFP_KERNEL | __GFP_ACCOUNT);
    if (node == NULL) {
        dms_err("kzalloc failed.\n");
        return NULL;
    }
    node->feature = feature;

    /* parse the process list */
    ret = dms_feature_parse_proc_ctrl(feature->proc_ctrl_str, &node->proc_buf, &node->proc_ctrl, &node->proc_num);
    if (ret != 0) {
        dms_err("Parse proc failed. (ret=%d)\n", ret);
        kfree(node);
        node = NULL;
        return NULL;
    }
    return node;
}

STATIC void feature_free_feature_node(DMS_FEATURE_NODE_S* node)
{
    if (node->proc_ctrl != NULL) {
        kfree(node->proc_ctrl);
        node->proc_ctrl = NULL;
    }
    if (node->proc_buf != NULL) {
        kfree(node->proc_buf);
        node->proc_buf = NULL;
    }

    kfree(node);
    return;
}

static inline void feature_set_state(DMS_FEATURE_NODE_S* node, u32 state)
{
    node->state = state;
}

int feature_inc_work(DMS_FEATURE_NODE_S* node)
{
    /* If state was not "STATE_ACTIVE",no more work are allowed. */
    if (node->state != STATE_ACTIVE) {
        dms_err("Not support in the status. (state=%u)\n", node->state);
        return -EBUSY;
    }
    atomic_inc(&node->count);
    return 0;
}

static inline void feature_dec_work(DMS_FEATURE_NODE_S* node)
{
    atomic_dec(&node->count);
    return;
}

STATIC int feature_wait_work_finish(DMS_FEATURE_NODE_S* node)
{
    int wait_count = 0;
    /* Check file not in used */
    while ((atomic_read(&node->count) != 0) && (wait_count < FEATURE_WAIT_MAX_TIME)) {
        wait_count++;
        if ((wait_count % FEATURE_CONFIRM_WARN_MASK) == 0) {
            dms_warn("Wait process finish. (main_cmd=%u; sub_cmd=%u; "
                     "work_count=%d)\n",
                node->feature->main_cmd,
                node->feature->sub_cmd,
                atomic_read(&node->count));
        }
        msleep(FEATURE_WAIT_EACH_TIME);
    }
    if (wait_count >= FEATURE_WAIT_MAX_TIME) {
        dms_err("Wait process time out. (main_cmd=%u; sub_cmd=%u; work_count=%d)\n",
            node->feature->main_cmd,
            node->feature->sub_cmd,
            atomic_read(&node->count));
        return -EBUSY;
    }
    return 0;
}

STATIC ssize_t dms_feature_print_feature(void *data, char *buf, ssize_t *offset)
{
    DMS_FEATURE_NODE_S* node = *((DMS_FEATURE_NODE_S **)data);
    ssize_t buf_ret = 0;
    buf_ret = snprintf_s(buf + *offset, PAGE_SIZE - *offset, PAGE_SIZE - 1 - *offset,
        "%s\t0x%x\t0x%x\t%s\t%s\t0x%x\t%u\t%d\t%llu\t%llu\t%llu\t%llu\t%llu\t%d\n",
        node->feature->owner_name, node->feature->main_cmd, node->feature->sub_cmd,
        (node->feature->filter == NULL) ? "null" : node->feature->filter,
        (node->feature->proc_ctrl_str == NULL) ? "null" : node->feature->proc_ctrl_str,
        node->feature->privilege, node->state, node->count,
        node->s.used, node->s.failed, node->s.time_max, node->s.time_min, node->s.time_last, node->s.last_ret);
    if (buf_ret >= 0) {
        *offset += buf_ret;
    }
    return *offset;
}

ssize_t dms_feature_print_feature_list(char *buf)
{
    ssize_t offset = 0;
    offset = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1,
        "owner\tmain\tsub\tfilter\tprocess\tpri\tstate"
        "\tcount\ttotal\tfailed\ts-max\ts-min\ts-last\ts_l_ret\n");
    if (offset >= 0) {
        dms_kv_dump(buf, &offset, dms_feature_print_feature);
    }
    return offset;
}
EXPORT_SYMBOL(dms_feature_print_feature_list);

int dms_feature_register(DMS_FEATURE_S* feature)
{
    char key[KV_KEY_MAX_LEN + 1] = {0};
    DMS_FEATURE_NODE_S* node = NULL;
    int ret;

    ret = feature_check_para(feature);
    if (ret != 0) {
        dms_err("Invalid Parameter. (ret=%d)\n", ret);
        return ret;
    }
    ret = dms_feature_make_key(feature->main_cmd, feature->sub_cmd, feature->filter, key, KV_KEY_MAX_LEN + 1);
    if (ret != 0) {
        dms_err("Make key failed. (main_cmd=%u; sub_cmd=%u)\n", feature->main_cmd, feature->sub_cmd);
        return ret;
    }

    node = feature_alloc_feature_node(feature);
    if (node == NULL) {
        dms_err("Make node failed. (main_cmd=%u; sub_cmd=%u)\n", feature->main_cmd, feature->sub_cmd);
        return -ENOMEM;
    }
    ret = feature_add_key_node(key, node);
    if (ret != 0) {
        dms_err("Add key failed. (key=\"%s\")\n", key);
        feature_free_feature_node(node);
        node = NULL;
        return ret;
    }
    feature_set_state(node, STATE_ACTIVE);

    dms_info("register OK. (owner_name=%s; main_cmd=%u; sub_cmd=%u; filter=%s; proc_ctrl_str=%s; privilege=%u)\n",
        feature->owner_name, feature->main_cmd, feature->sub_cmd,
        (feature->filter == NULL) ? "null" : feature->filter,
        (feature->proc_ctrl_str == NULL) ? "null" : feature->proc_ctrl_str,
        feature->privilege);
    return 0;
}
EXPORT_SYMBOL(dms_feature_register);

int dms_feature_unregister(DMS_FEATURE_S* feature)
{
    int ret;
    char key[KV_KEY_MAX_LEN + 1] = {0};
    DMS_FEATURE_NODE_S* node = NULL;

    ret = feature_check_para(feature);
    if (ret != 0) {
        dms_err("Invalid Parameter. (ret=%d)\n", ret);
        return ret;
    }

    ret = dms_feature_make_key(feature->main_cmd, feature->sub_cmd, feature->filter, key, KV_KEY_MAX_LEN + 1);
    if (ret != 0) {
        dms_err("Make key failed. (main_cmd=%u; sub_cmd=%u)\n", feature->main_cmd, feature->sub_cmd);
        return ret;
    }

    node = feature_get_key_node(key);
    if (node == NULL) {
        dms_err("Not support feature. (key=\"%s\")\n", key);
        return -EINVAL;
    }

    feature_set_state(node, STATE_STOP);
    ret = feature_wait_work_finish(node);
    if (ret != 0) {
        dms_err("Wait work finish time out.(main_cmd=%u; sub_cmd=%u)\n", feature->main_cmd, feature->sub_cmd);
        return ret;
    }

    ret = feature_del_key_node(key);
    if (ret != 0) {
        dms_err("Delete kv node failed. (key=\"%s\")\n", key);
        return ret;
    }

    feature_free_feature_node(node);
    node = NULL;
    dms_info("unregister OK. (main_cmd=%u; sub_cmd=%u)\n", feature->main_cmd, feature->sub_cmd);
    return 0;
}
EXPORT_SYMBOL(dms_feature_unregister);

STATIC void dms_update_static(FEATURE_STATISTIC_S *s,
    int ret, u64 start, u64 end)
{
    s->last_ret = ret;
    s->used++;
    if (ret) {
        s->failed++;
    }
    if (end >= start) {
        s->time_last = end - start;
    } else {
        s->time_last = end + (ULLONG_MAX - start);
    }
    if (s->time_last > s->time_max) {
        s->time_max = s->time_last;
    }
    if (s->time_last < s->time_min) {
        s->time_max = s->time_last;
    }
}

STATIC void urd_feature_make_handle_para(DMS_FEATURE_ARG_S* arg, DMS_FEATURE_S* feature,
                                         struct urd_cmd *cmd, struct urd_cmd_para *cmd_para)
{
    cmd->filter = feature->filter;
    cmd->filter_len = (feature->filter != NULL) ? strlen(feature->filter) : 0;
    cmd->main_cmd = feature->main_cmd;
    cmd->sub_cmd = feature->sub_cmd;

    cmd_para->input = arg->input;
    cmd_para->input_len = arg->input_len;
    cmd_para->output = arg->output;
    cmd_para->output_len = arg->output_len;
    return;
}

STATIC int urd_feature_make_handle_devid(u32 msg_source, u32 devid, struct urd_cmd_kernel_para *kernel_para)
{
    int ret;

    if (msg_source == MSG_FROM_USER) {
        ret = uda_devid_to_udevid(devid, &kernel_para->udevid);
        if (ret != 0) {
            dms_err("devid to udevid fail. (devid=%u)\n", devid);
            return ret;
        }
    } else {
        /* kernel urd forward devid is udevid */
        kernel_para->udevid = devid;
    }

    if (!uda_is_phy_dev(kernel_para->udevid)) {
        struct uda_mia_dev_para mia_para;
        (void)uda_udevid_to_mia_devid(kernel_para->udevid, &mia_para);
        kernel_para->phyid = mia_para.phy_devid;
    } else {
        kernel_para->phyid = kernel_para->udevid;
    }

    return 0;
}

STATIC int urd_feature_handle(DMS_FEATURE_ARG_S* arg, DMS_FEATURE_S* feature)
{
    int ret;

    if (feature->call_fun == NULL) {
        dms_err("Feature handler is null. (key=\"%s\")\n", arg->key);
        return -EINVAL;
    }

    if (feature->handler_type == URD_HANDLER) {
        ret = (feature->handler)((void *)feature, arg->input, arg->input_len, arg->output, arg->output_len);
    } else {
        struct urd_cmd cmd = {0};
        struct urd_cmd_para cmd_para = {0};
        struct urd_cmd_kernel_para kernel_para = {0};

        urd_feature_make_handle_para(arg, feature, &cmd, &cmd_para);
        ret = urd_feature_make_handle_devid(arg->msg_source, arg->devid, &kernel_para);
        if (ret != 0) {
            return ret;
        }

        ret = (feature->dev_handler)(&cmd, &kernel_para, &cmd_para);
        if (ret != 0) {
            return ret;
        }
    }

    return ret;
}

int dms_feature_process(DMS_FEATURE_ARG_S* arg)
{
    DMS_FEATURE_S* feature = NULL;
    DMS_FEATURE_NODE_S* node = NULL;
    u64 start, end;
    int ret;
    if ((arg == NULL) || (arg->key == NULL)) {
        dms_err("Invalid Parameter.\n");
        return -EINVAL;
    }
    node = feature_get_key_node_ex(arg->key);
    if (node == NULL || (node->feature == NULL)) {
        dms_debug("Not support feature. (key=\"%s\")\n", arg->key);
        return -EOPNOTSUPP;
    }
    feature = node->feature;

    if (arg->msg_source == MSG_FROM_USER || arg->msg_source == MSG_FROM_USER_REST_ACC) {
        /* whitelist check */
        ret = dms_feature_whitelist_check((const char**)node->proc_ctrl, node->proc_num);
        if (ret != 0) {
            feature_dec_work(node);
            dms_err("Operation not permitted. (key=\"%s\")\n", arg->key);
            return DRV_ERROR_OPER_NOT_PERMITTED;
        }

        /* privilege check */
        ret = dms_feature_access_identify(feature->privilege, arg->msg_source);
        if (ret != 0) {
            feature_dec_work(node);
            dms_ex_notsupport_err(ret, "Operation not permitted. (key=\"%s\"; privilege=%08x)\n",
                arg->key,
                feature->privilege);
            return ret;
        }
    }

    /* do handler */
    start = dms_get_cur_cpu_tick();
    ret = urd_feature_handle(arg, feature);
    end = dms_get_cur_cpu_tick();
    dms_update_static(&node->s, ret, start, end);
    feature_dec_work(node);
    return ret;
}

int dms_feature_make_key(u32 main_cmd, u32 sub_cmd, const char* filter, char* key, u32 len)
{
    int ret;
    if ((key == NULL) || (len <= 1) || (len > (KV_KEY_MAX_LEN + 1))) {
        dms_err("Invalid Parameter. (len=%u)\n", len);
        return -EINVAL;
    }
    if (filter != NULL) {
        ret = snprintf_s(key, len, len - 1, "fun_0x%x_0x%x_%s", main_cmd, sub_cmd, filter);
    } else {
        ret = snprintf_s(key, len, len - 1, "fun_0x%x_0x%x", main_cmd, sub_cmd);
    }
    if (ret <= 0) {
        dms_err("sprintf_s failed. (ret=%d)\n", ret);
        return ret;
    }
    return 0;
}
