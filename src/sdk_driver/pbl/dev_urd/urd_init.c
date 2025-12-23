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

#include <linux/io.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/notifier.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/atomic.h>
#include <linux/poll.h>
#include <linux/sort.h>
#include <linux/vmalloc.h>

#include "pbl/pbl_davinci_api.h"
#include "davinci_interface.h"
#include "urd_define.h"
#include "urd_notifier.h"
#include "urd_feature.h"
#include "urd_kv.h"
#include "securec.h"
#include "urd_init.h"
#include "pbl/pbl_urd.h"
#include "urd_cmd.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)
#include <linux/namei.h>
#endif

#define COPY_FUNC_NUM 2
#define FILTER_MAX_LEN 128

STATIC int copy_from_user_local(void *to, unsigned long to_len, const void *from, unsigned long from_len)
{
    return (int)copy_from_user(to, (void __user *)from, from_len);
}

STATIC int copy_to_user_local(void *to, unsigned long to_len, const void *from, unsigned long from_len)
{
    return (int)copy_to_user((void __user *)to, from, from_len);
}

STATIC int memcpy_s_local(void *to, unsigned long to_len, const void *from, unsigned long from_len)
{
    return memcpy_s(to, to_len, from, from_len);
}

int (*copy_func_to[MSG_FROM_TYPE_MAX])(void *to, unsigned long to_len, const void *from, unsigned long from_len) = {
    copy_to_user_local,
    memcpy_s_local,
    copy_to_user_local,
};

int (*copy_func_from[MSG_FROM_TYPE_MAX])(void *to, unsigned long to_len, const void *from, unsigned long from_len) = {
    copy_from_user_local,
    memcpy_s_local,
    copy_from_user_local,
};

STATIC int dms_open(struct inode* inode, struct file* filep)
{
    struct urd_file_private_stru* file_private = NULL;
    if (filep == NULL) {
        return -EINVAL;
    }

    if (!try_module_get(THIS_MODULE)) {
        return -EBUSY;
    }

    file_private =
        (struct urd_file_private_stru*)kzalloc(sizeof(struct urd_file_private_stru), GFP_KERNEL | __GFP_ACCOUNT);
    if (file_private == NULL) {
        dms_err("kzalloc failed. (size=%lu)\n", sizeof(struct urd_file_private_stru));
        module_put(THIS_MODULE);
        return -ENOMEM;
    }
    file_private->owner_pid = current->tgid;
    filep->private_data = (void *)file_private;
    atomic_set(&file_private->work_count, 0);
    return 0;
}

STATIC int dms_release(struct inode* inode, struct file* filep)
{
    struct urd_file_private_stru* file_private = NULL;

    if (filep == NULL) {
        return -EINVAL;
    }
    module_put(THIS_MODULE);

    file_private = filep->private_data;
    if (file_private == NULL) {
        return -EBADFD;
    }
    urd_notifier_call(URD_NOTIFIER_RELEASE, file_private);

    kfree(file_private);
    file_private = NULL;
    return 0;
}

STATIC unsigned int dms_msg_poll(struct file* filep, struct poll_table_struct* wait)
{
    return POLLERR;
}

STATIC int dms_make_feature_key(struct urd_cmd* cmd, DMS_FEATURE_ARG_S* arg)
{
    int ret;
    char* filter = NULL;

    if ((cmd->filter_len > FILTER_MAX_LEN) || ((cmd->filter_len == 0) && (cmd->filter != NULL))) {
        dms_err("filter_len is error. (filter_len=%u)", cmd->filter_len);
        return -EINVAL;
    }
    arg->key = (char*)kzalloc(KV_KEY_MAX_LEN + 1, GFP_KERNEL | __GFP_ACCOUNT);
    if (arg->key == NULL) {
        dms_err("kzalloc failed. (size=%d)\n", KV_KEY_MAX_LEN);
        return -ENOMEM;
    }
    if (cmd->filter != NULL) {
        filter = kzalloc(cmd->filter_len + 1, GFP_KERNEL | __GFP_ACCOUNT);
        if (filter == NULL) {
            dms_err("kzalloc failed. (size=%u)\n", cmd->filter_len + 1);
            kfree(arg->key);
            arg->key = NULL;
            return -ENOMEM;
        }
        ret = copy_func_from[arg->msg_source]((void*)filter, (unsigned long)cmd->filter_len + 1,
            (void*)cmd->filter, (unsigned long)cmd->filter_len);
        if (ret != 0) {
            dms_err("copy_func_from failed. (ret=%d; size=%u)\n", ret, cmd->filter_len);
            kfree(arg->key);
            arg->key = NULL;
            kfree(filter);
            filter = NULL;
            return -EFAULT;
        }
    }
    ret = dms_feature_make_key(cmd->main_cmd, cmd->sub_cmd, filter, arg->key, KV_KEY_MAX_LEN + 1);
    if (ret != 0) {
        dms_err(
            "make feature's key failed. (ret=%d; main_cmd=%u; sub_cmd=%u)\n", ret, cmd->main_cmd, cmd->sub_cmd);
        kfree(arg->key);
        arg->key = NULL;
    }
    if (filter != NULL) {
        kfree(filter);
        filter = NULL;
    }
    return ret;
}

STATIC int dms_make_feature_input(struct urd_cmd_para* cmd_para, DMS_FEATURE_ARG_S* arg)
{
    int ret;
    arg->input = NULL;
    arg->input_len = 0;
    /* means no input data */
    if ((cmd_para->input_len == 0) && (cmd_para->input == NULL)) {
        return 0;
    }

    if ((cmd_para->input_len != 0) && (cmd_para->input != NULL) && (cmd_para->input_len <= DMS_MAX_INPUT_LEN)) {
        arg->input = kmalloc(cmd_para->input_len, GFP_KERNEL | __GFP_ACCOUNT);
        if (arg->input == NULL) {
            dms_err("kzalloc failed. (size=%u)\n", cmd_para->input_len);
            return -ENOMEM;
        }
        arg->input_len = cmd_para->input_len;
        ret = copy_func_from[arg->msg_source]((void*)arg->input, (unsigned long)arg->input_len,
            (void*)cmd_para->input, (unsigned long)cmd_para->input_len);
        if (ret != 0) {
            dms_err("copy_func_from failed. (ret=%d; size=%u)\n", ret, cmd_para->input_len);
            kfree(arg->input);
            arg->input = NULL;
            return -EFAULT;
        }
        return 0;
    } else {
        dms_err("Input para error. (input_len=%u)\n", cmd_para->input_len);
        return -EINVAL;
    }
}

#define DMS_OUTPUT_2M_LIMIT (2 * 1024 * 1024)
// apply high-capacity memory for output
STATIC int dms_make_feature_max_output(struct urd_cmd_para* cmd_para, DMS_FEATURE_ARG_S* arg)
{
    if (cmd_para->output_len > DMS_OUTPUT_2M_LIMIT) {
        dms_err("Output len is larger than 2M limit. (output_len=%u)\n", cmd_para->output_len);
        return -EINVAL;
    }

    arg->output = kzalloc(cmd_para->output_len, GFP_KERNEL | __GFP_ACCOUNT);
    if (arg->output == NULL) {
        dms_err("kzalloc failed. (size=%u)\n", cmd_para->output_len);
        return -ENOMEM;
    }
    arg->output_len = cmd_para->output_len;
    return 0;
}

// if need high-capacity memory(greater than DMS_MAX_OUTPUT_LEN) return true
static bool dms_is_need_high_capacity_memory(struct urd_cmd* cmd)
{
    //  get history fault event info need high-capacity memory
    if ((cmd->main_cmd == DMS_MAIN_CMD_BASIC && cmd->sub_cmd == DMS_SUBCMD_GET_HISTORY_FAULT_EVENT) ||
        (cmd->main_cmd == DMS_MAIN_CMD_BASIC && cmd->sub_cmd == DMS_SUBCMD_GET_FAULT_EVENT_OBJ) ||
        (cmd->main_cmd == DMS_MAIN_CMD_BASIC && cmd->sub_cmd == DMS_SUBCMD_GET_DEVICE_STATE) ||
        (cmd->main_cmd == DMS_MAIN_CMD_BASIC && cmd->sub_cmd == DMS_SUBCMD_GET_DEV_PROBE_LIST) ||
        (cmd->main_cmd == DMS_MAIN_CMD_BASIC && cmd->sub_cmd == DMS_SUBCMD_GET_ALL_DEV_LIST) ||
        (cmd->main_cmd == DMS_MAIN_CMD_BASIC && cmd->sub_cmd == DMS_SUBCMD_GET_DEVICE_FROM_CHIP)) {
        return true;
    }
    return false;
}

STATIC int dms_make_feature_output(struct urd_cmd *cmd, struct urd_cmd_para *cmd_para, DMS_FEATURE_ARG_S* arg)
{
    arg->output = NULL;
    arg->output_len = 0;

    /* means no output data */
    if (cmd_para->output_len == 0) {
        return 0;
    }

    // need high-capacity memory, maybe greater than DMS_MAX_OUTPUT_LEN
    if (dms_is_need_high_capacity_memory(cmd)) {
        return dms_make_feature_max_output(cmd_para, arg);
    }

    if (cmd_para->output_len <= DMS_MAX_OUTPUT_LEN) {
        arg->output = kzalloc(cmd_para->output_len, GFP_KERNEL | __GFP_ACCOUNT);
        if (arg->output == NULL) {
            dms_err("kzalloc failed. (size=%u)\n", cmd_para->output_len);
            return -ENOMEM;
        }
        arg->output_len = cmd_para->output_len;
        return 0;
    } else {
        dms_err("Output len error. (output_len=%u)\n", cmd_para->output_len);
        return -EINVAL;
    }
}

STATIC int dms_proc_feature_output(struct urd_cmd_para *cmd_para, DMS_FEATURE_ARG_S* arg)
{
    int ret;

    if (cmd_para->output != NULL) {
        ret = copy_func_to[arg->msg_source]((void*)cmd_para->output, (unsigned long)cmd_para->output_len,
            (void*)arg->output, (unsigned long)cmd_para->output_len);
        if (ret != 0) {
            dms_err("copy_func_to failed. (size=%u)\n", cmd_para->output_len);
            return -EFAULT;
        }
    }

    return 0;
}

STATIC void dms_free_msg(DMS_FEATURE_ARG_S* arg)
{
    if (arg->key != NULL) {
        kfree(arg->key);
        arg->key = NULL;
    }
    if (arg->input != NULL) {
        kfree(arg->input);
        arg->input = NULL;
    }
    if (arg->output != NULL) {
        kfree(arg->output);
        arg->output = NULL;
    }
    return;
}

STATIC int dms_make_up_msg_arg(struct urd_cmd *cmd, struct urd_cmd_para *cmd_para, DMS_FEATURE_ARG_S* arg)
{
    int ret;
    ret = dms_make_feature_key(cmd, arg);
    if (ret != 0) {
        return ret;
    }

    ret = dms_make_feature_input(cmd_para, arg);
    if (ret != 0) {
        dms_free_msg(arg);
        return ret;
    }

    ret = dms_make_feature_output(cmd, cmd_para, arg);
    if (ret != 0) {
        dms_free_msg(arg);
        return ret;
    }
    return 0;
}

STATIC int dms_cmd_process(u32 devid, struct urd_cmd *cmd, struct urd_cmd_para *cmd_para, u32 msg_source)
{
    int ret;
    DMS_FEATURE_ARG_S arg = {0};
    if (msg_source >= MSG_FROM_TYPE_MAX) {
        dms_err("msg_source is wrong. (msg_source=%u)\n", msg_source);
        return -EINVAL;
    }
    arg.msg_source = msg_source;
    arg.devid = devid;

    /* make up call arg */
    ret = dms_make_up_msg_arg(cmd, cmd_para, &arg);
    if (ret != 0) {
        dms_err("Make up message arg failed. (ret=%d)\n", ret);
        return ret;
    }

    /* call feature process */
    ret = dms_feature_process(&arg);
    if (ret != 0) {
        /* do not print err info */
        goto out;
    }

    /* copy out data to user */
    ret = dms_proc_feature_output(cmd_para, &arg);
    if (ret != 0) {
        dms_err("Process output failed. (ret=%d; size=%u)\n", ret, cmd_para->output_len);
    }

out:
    dms_free_msg(&arg);
    return ret;
}

#define CMD_MASK (0x0000FFFFU)
STATIC bool check_cmd_arg_valid(unsigned int cmd, struct urd_ioctl_arg ctl_arg)
{
    if ((cmd & CMD_MASK) == ((ctl_arg.cmd.main_cmd << _IOC_TYPESHIFT) + (ctl_arg.cmd.sub_cmd << _IOC_NRSHIFT))) {
        return true;
    } else {
        return false;
    }
}

int dms_cmd_process_from_kernel(u32 devid, struct urd_cmd *cmd, struct urd_cmd_para *cmd_para)
{
    if ((cmd == NULL) || (cmd_para == NULL)) {
        dms_err("Invalid file_private_data.\n");
        return -EINVAL;
    }

    return dms_cmd_process(devid, cmd, cmd_para, MSG_FROM_KERNEL);
}
EXPORT_SYMBOL(dms_cmd_process_from_kernel);

STATIC long dms_ioctl(struct file* filep, unsigned int ioctl_cmd, unsigned long arg)
{
    int ret;
    struct urd_ioctl_arg ctl_arg = {0};
    u32 msg_source;

    if ((filep == NULL) || (arg == 0)) {
        dms_err("Invalid file_private_data.\n");
        return -EINVAL;
    }

    /* copy ioctl arg from user */
    ret = copy_from_user(&ctl_arg, (void*)((uintptr_t)arg), sizeof(struct urd_ioctl_arg));
    if (ret != 0) {
        dms_err("copy_from_user_safe failed.\n");
        return ret;
    }
    if ((ioctl_cmd == URD_IOCTL_CMD) || (check_cmd_arg_valid(ioctl_cmd, ctl_arg))) {
        msg_source = (ascend_intf_is_restrict_access(filep) ? MSG_FROM_USER_REST_ACC : MSG_FROM_USER);
        ret = dms_cmd_process(ctl_arg.devid, &ctl_arg.cmd, &ctl_arg.cmd_para, msg_source);
    } else {
        dms_err("Invalid commmad. (command=%u; main_cmd=%u; sub_cmd=%u)\n", ioctl_cmd, ctl_arg.cmd.main_cmd, ctl_arg.cmd.sub_cmd);
        ret = -ENODEV;
    }
    return ret;
}

const struct file_operations g_dms_file_operations = {
    .owner = THIS_MODULE,
    .open = dms_open,
    .release = dms_release,
    .poll = dms_msg_poll,
    .unlocked_ioctl = dms_ioctl,
};

STATIC int urd_release_prepare(struct file *file_op, unsigned long mode)
{
    if (mode != NOTIFY_MODE_RELEASE_PREPARE) {
        dms_err("Invalid mode. (mode=%lu)\n", mode);
        return -EINVAL;
    }

    if ((file_op == NULL) || (file_op->private_data == NULL)) {
        return -EBADFD;
    }

    urd_notifier_call(URD_NOTIFIER_RELEASE_PREPARE, file_op->private_data);
    return 0;
}

static const struct notifier_operations urd_notifier_ops = {
    .owner = THIS_MODULE,
    .notifier_call = urd_release_prepare,
};

int urd_init(void)
{
    int ret;

    dms_info("dms urd driver init start.\n");
    ret = drv_davinci_register_sub_module(DAVINCI_INTF_MODULE_URD, &g_dms_file_operations);
    if (ret != 0) {
        dms_err("Failed to register DMS module. (ret=%d)\n", ret);
        return ret;
    }

    ret = drv_ascend_register_notify(DAVINCI_INTF_MODULE_URD, &urd_notifier_ops);
    if (ret != 0) {
        dms_err("Failed to register davinci notify. (ret=%d)\n", ret);
        goto unregister_sub_module;
    }

    ret = dms_kv_init();
    if (ret) {
        dms_err("Failed to init dms kv. (ret=%d)\n", ret);
        goto unregister_davinci_notify;
    }

    dms_info("dms urd driver init success.\n");
    return 0;

unregister_davinci_notify:
    (void)drv_ascend_unregister_notify(DAVINCI_INTF_MODULE_URD);
unregister_sub_module:
    (void)drv_ascend_unregister_sub_module(DAVINCI_INTF_MODULE_URD);
    return ret;
}

void urd_exit(void)
{
    dms_info("Dms urd exit start.\n");
    dms_kv_uninit();
    (void)drv_ascend_unregister_notify(DAVINCI_INTF_MODULE_URD);
    (void)drv_ascend_unregister_sub_module(DAVINCI_INTF_MODULE_URD);
    dms_info("Dms urd driver exit success.\n");
    return;
}
