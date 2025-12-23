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

#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/version.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/nsproxy.h>
#include <linux/version.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0))
#include <linux/sched/task.h>
#endif

#include "securec.h"

#include "queue_module.h"
#include "queue_proc_fs.h"

#define PROC_FS_NAME_LEN 32
#define PROC_FS_R_MODE 0444
#define PROC_FS_RW_MODE 0644

static struct proc_dir_entry *queue_top_entry = NULL;
static struct proc_dir_entry *queue_process_entry = NULL;

static int queue_normal_status_show(struct seq_file *seq, void *offset)
{
#ifndef EMU_ST
    struct queue_qid_status *status = (struct queue_qid_status *)seq->private;

    queue_show_one_qid_status(seq, status);
    return 0;
#endif
}

static int queue_normal_status_open(struct inode *inode, struct file *file)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 17, 0)
    return single_open(file, queue_normal_status_show, pde_data(inode));
#else
    return single_open(file, queue_normal_status_show, PDE_DATA(inode));
#endif
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 5, 0)
static const struct file_operations normal_status_ops = {
    .owner   = THIS_MODULE,
    .open    = queue_normal_status_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = single_release,
};
#else
static const struct proc_ops normal_status_ops = {
    .proc_open    = queue_normal_status_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};
#endif

static int queue_abnormal_status_show(struct seq_file *seq, void *offset)
{
    seq_printf(seq, "abnormal_queue_status_show\n");
    queue_show_all_qid_status(seq, RECORD_EXCEPT);

    return 0;
}

static int queue_abnormal_status_open(struct inode *inode, struct file *file)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 17, 0)
    return single_open(file, queue_abnormal_status_show, pde_data(inode));
#else
    return single_open(file, queue_abnormal_status_show, PDE_DATA(inode));
#endif
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 5, 0)
static const struct file_operations abnormal_status_ops = {
    .owner   = THIS_MODULE,
    .open    = queue_abnormal_status_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = single_release,
};
#else
static const struct proc_ops abnormal_status_ops = {
    .proc_open    = queue_abnormal_status_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};
#endif

#ifndef EMU_ST
static int queue_perf_switch_show(struct seq_file *seq, void *offset)
{
    seq_printf(seq, "perf_queue_switch_show\n");
    queue_show_perf_switch(seq);

    return 0;
}

static int queue_perf_switch_open(struct inode *inode, struct file *file)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 17, 0)
    return single_open(file, queue_perf_switch_show, pde_data(inode));
#else
    return single_open(file, queue_perf_switch_show, PDE_DATA(inode));
#endif
}

#define PERF_SWITCH_MAX_LEN 10
static ssize_t queue_perf_switch_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos)
{
    char kstr[PERF_SWITCH_MAX_LEN] = {0};
    char *time_threshold_ptr = NULL;
    char *perf_switch_ptr = NULL;
    u32 time_threshold = 0;
    bool set_perf_switch = false;
    int ret;

    if (current->nsproxy->mnt_ns != init_task.nsproxy->mnt_ns) {
        queue_err("No permission to set perf switch.\n");
        return -EINVAL;
    }

    if ((count == 0) || (count > PERF_SWITCH_MAX_LEN)) {
        queue_err("Input count out of range. (count=%lu; max=%u)\n", count, PERF_SWITCH_MAX_LEN);
        return -EINVAL;
    }

    if (copy_from_user(kstr, (void *)buffer, count)) {
        queue_err("Copy from user fail.\n");
        return -EINVAL;
    }
    kstr[count - 1] = '\0';

    perf_switch_ptr = strtok_s(kstr, " ", &time_threshold_ptr);
    if (perf_switch_ptr != NULL) {
        ret = kstrtobool(perf_switch_ptr, &set_perf_switch);
        if (ret) {
            queue_err("Kstr to bool failed. (ret=%d)\n", ret);
            return -EINVAL;
        }
        ret = kstrtouint(time_threshold_ptr, 10, &time_threshold);
        if (ret != 0) {
            time_threshold = 0;
        }
    }

    if (set_perf_switch == false) {
        queue_free_one_type_qid_status(RECORD_PERF);
        time_threshold = 0;
    }
    queue_set_perf_switch(set_perf_switch, time_threshold);

    return (ssize_t)count;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 5, 0)
static const struct file_operations perf_switch_ops = {
    .owner   = THIS_MODULE,
    .open    = queue_perf_switch_open,
    .read    = seq_read,
    .write   = queue_perf_switch_write,
    .llseek  = seq_lseek,
    .release = single_release,

};
#else
static const struct proc_ops perf_switch_ops = {
    .proc_open    = queue_perf_switch_open,
    .proc_read    = seq_read,
    .proc_write   = queue_perf_switch_write,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};
#endif

static int queue_perf_status_show(struct seq_file *seq, void *offset)
{
    seq_printf(seq, "perf_queue_status_show\n");
    queue_show_all_qid_status(seq, RECORD_PERF);

    return 0;
}

static int queue_perf_status_open(struct inode *inode, struct file *file)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 17, 0)
    return single_open(file, queue_perf_status_show, pde_data(inode));
#else
    return single_open(file, queue_perf_status_show, PDE_DATA(inode));
#endif
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 5, 0)
static const struct file_operations perf_status_ops = {
    .owner   = THIS_MODULE,
    .open    = queue_perf_status_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = single_release,
};
#else
static const struct proc_ops perf_status_ops = {
    .proc_open    = queue_perf_status_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};
#endif
#endif

static void proc_fs_format_qid_dir_name(u32 qid, char *name, u32 len)
{
    if (sprintf_s(name, len, "qid-%u", qid) <= 0) {
        queue_info("Qid sprintf_s unsuccessful. (qid=%u)\n", qid);
    }
}

static struct proc_dir_entry *proc_fs_mk_qid_dir(u32 qid, struct proc_dir_entry *parent)
{
    char name[PROC_FS_NAME_LEN];

    if (parent == NULL) {
        return NULL;
    }
    proc_fs_format_qid_dir_name(qid, name, PROC_FS_NAME_LEN);
    return proc_mkdir((const char *)name, parent);
}

void queue_proc_fs_add_qid(struct queue_qid_status *status, struct proc_dir_entry *parent)
{
    struct proc_dir_entry *entry = NULL;

    if (status == NULL) {
        return;
    }

    if (atomic_cmpxchg(&status->qid_dir_exit, QID_DIR_NO_EXIT, QID_DIR_IS_EXIT) != QID_DIR_NO_EXIT) {
        return;
    }

    entry = proc_fs_mk_qid_dir(status->qid, parent);
    if (entry == NULL) {
        atomic_set(&status->qid_dir_exit, QID_DIR_NO_EXIT);
        queue_info("Create qid entry dir unsuccessful. (qid=%u)\n", status->qid);
        return;
    }

    (void)proc_create_data("queue_status", PROC_FS_R_MODE, entry, &normal_status_ops, status);
}

#ifndef EMU_ST
static void proc_fs_rm_qid_dir(u32 qid, struct proc_dir_entry *parent)
{
    char name[PROC_FS_NAME_LEN];

    if (parent == NULL) {
        return;
    }

    proc_fs_format_qid_dir_name(qid, name, PROC_FS_NAME_LEN);
    (void)remove_proc_subtree((const char *)name, parent);
}

void queue_proc_fs_del_qid(struct queue_qid_status *status, struct proc_dir_entry *parent)
{
    proc_fs_rm_qid_dir(status->qid, parent);
    atomic_set(&status->qid_dir_exit, QID_DIR_NO_EXIT);
}
#endif
static void proc_fs_format_process_dir_name(pid_t pid, char *name, u32 len)
{
    if (sprintf_s(name, len, "%d", pid) <= 0) {
        queue_info("Pid sprintf_s unsuccessful. (pid=%d)\n", pid);
    }
}

static struct proc_dir_entry *proc_fs_mk_process_dir(pid_t pid, struct proc_dir_entry *parent)
{
    char name[PROC_FS_NAME_LEN];

    if (parent == NULL) {
        return NULL;
    }
    proc_fs_format_process_dir_name(pid, name, PROC_FS_NAME_LEN);
    return proc_mkdir((const char *)name, parent);
}

static void proc_fs_rm_process_dir(pid_t pid, struct proc_dir_entry *parent)
{
    char name[PROC_FS_NAME_LEN];

    if (parent == NULL) {
        return;
    }

    proc_fs_format_process_dir_name(pid, name, PROC_FS_NAME_LEN);
    (void)remove_proc_subtree((const char *)name, parent);
}

void queue_proc_fs_add_process(struct queue_context *ctx)
{
    ctx->entry = proc_fs_mk_process_dir(ctx->pid, queue_process_entry);
    if (ctx->entry == NULL) {
        queue_info("Create process entry dir unsuccessful. (pid=%u)\n", ctx->pid);
        return;
    }
}

void queue_proc_fs_del_process(struct queue_context *ctx)
{
    proc_fs_rm_process_dir(ctx->pid, queue_process_entry);
}

void queue_proc_fs_init(void)
{
    struct proc_dir_entry *abnormal_entry = NULL;
    struct proc_dir_entry *perf_entry = NULL;

    queue_status_record_mng_init();
    queue_top_entry = proc_mkdir("queue", NULL);
    if (queue_top_entry != NULL) {
        queue_process_entry = proc_mkdir("process", queue_top_entry);
        abnormal_entry = proc_mkdir("except_collect", queue_top_entry);
        if (abnormal_entry != NULL) {
            (void)proc_create_data("except_queue_status", PROC_FS_R_MODE, abnormal_entry, &abnormal_status_ops, NULL);
        }
        perf_entry = proc_mkdir("perf_collect", queue_top_entry);
        if (perf_entry != NULL) {
#ifndef EMU_ST
            (void)proc_create_data("perf_switch", PROC_FS_RW_MODE, perf_entry, &perf_switch_ops, NULL);
            (void)proc_create_data("perf_queue_status", PROC_FS_R_MODE, perf_entry, &perf_status_ops, NULL);
#endif
        }
    }
}

void queue_proc_fs_uninit(void)
{
    if (queue_top_entry != NULL) {
        (void)remove_proc_subtree("queue", NULL);
    }
    queue_free_all_type_qid_status();
}

