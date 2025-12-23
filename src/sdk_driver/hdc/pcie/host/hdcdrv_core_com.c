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

#include <linux/device.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/version.h>

#ifdef CFG_FEATURE_PFSTAT
#include "hdcdrv_pfstat.h"
#endif
#include "hdcdrv_core_com.h"
#include "hdcdrv_cmd_ioctl.h"
#include "hdcdrv_cmd_msg.h"
#include "hdcdrv_mem_com.h"
#include "comm_kernel_interface.h"

#ifdef CFG_FEATURE_VFIO
#include "vmng_kernel_interface.h"
#endif

u32 hdcdrv_cmd_size_table[HDCDRV_CMD_MAX] = {0};
STATIC bool hdcdrv_is_kernel_thread(void)
{
    return (current->mm == NULL);
}

u64 hdcdrv_get_pid(void)
{
    if (hdcdrv_is_kernel_thread()) {
        return HDCDRV_KERNEL_DEFAULT_PID;
    }

    return (u64)current->tgid;
}

u64 hdcdrv_get_ppid(void)
{
    if (hdcdrv_is_kernel_thread()) {
        return HDCDRV_KERNEL_DEFAULT_PID;
    }

    return (u64)current->parent->tgid;
}

int hdcdrv_rebuild_raw_pid(u64 pid)
{
    return (int)(pid & HDCDRV_RAW_PID_MASK);
}

u64 hdcdrv_get_task_start_time(void)
{
    if (hdcdrv_is_kernel_thread()) {
        return HDCDRV_KERNEL_DEFAULT_START_TIME;
    }

#ifdef DRV_UT
    return HDCDRV_KERNEL_DEFAULT_START_TIME;
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 17, 0)
    return (u64)current->group_leader->start_time;
#else
    return (u64)current->group_leader->start_time.tv_sec * NSEC_PER_SEC + current->group_leader->start_time.tv_nsec;
#endif
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 2, 0)
STATIC char *hdcdrv_devnode(const struct device *dev, umode_t *mode)
{
    return NULL;
}
#else
STATIC char *hdcdrv_devnode(struct device *dev, umode_t *mode)
{
    return NULL;
}
#endif

int hdccom_register_cdev(struct hdcdrv_cdev *hcdev, const struct file_operations *fops)
{
    struct device *dev = NULL;
    struct class *hdc_class = NULL;
    int ret;

    ret = alloc_chrdev_region(&hcdev->dev_no, 0, HDCDRV_CDEV_COUNT, HDCDRV_CHAR_DRIVER_NAME);
    if (ret != 0) {
        hdcdrv_err("Calling alloc_chrdev_region failed. (ret=%d)\n", ret);
        return ret;
    }

    /* init and add char device */
    cdev_init(&hcdev->cdev, fops);
    hcdev->cdev.owner = THIS_MODULE;

    ret = cdev_add(&hcdev->cdev, hcdev->dev_no, HDCDRV_CDEV_COUNT);
    if (ret != 0) {
        hdcdrv_err("Calling cdev_add failed. (ret=%d)\n", ret);
        goto cdev_add_failed;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
    hdc_class = class_create(HDCDRV_CHAR_DRIVER_NAME);
#else
    hdc_class = class_create(THIS_MODULE, HDCDRV_CHAR_DRIVER_NAME);
#endif
    if (IS_ERR_OR_NULL(hdc_class)) {
        hdcdrv_err("Class create failed.\n");
        ret = HDCDRV_CHAR_DEV_CREAT_FAIL;
        goto class_create_failed;
    }

    hcdev->cdev_class = hdc_class;
    hcdev->cdev_class->devnode = hdcdrv_devnode;
    dev = device_create(hcdev->cdev_class, NULL, hcdev->dev_no, NULL, HDCDRV_CHAR_DRIVER_NAME);
    if (IS_ERR_OR_NULL(dev)) {
        hdcdrv_err("Device create failed.\n");
        ret = HDCDRV_CHAR_DEV_CREAT_FAIL;
        goto dev_create_failed;
    }

    hcdev->dev = dev;

    return HDCDRV_OK;

dev_create_failed:
    class_destroy(hcdev->cdev_class);
class_create_failed:
    cdev_del(&hcdev->cdev);
cdev_add_failed:
    unregister_chrdev_region(hcdev->dev_no, HDCDRV_CDEV_COUNT);

    return ret;
}

void hdccom_free_cdev(struct hdcdrv_cdev *hcdev)
{
    (void)device_destroy(hcdev->cdev_class, hcdev->dev_no);
    (void)class_destroy(hcdev->cdev_class);
    (void)unregister_chrdev_region(hcdev->dev_no, HDCDRV_CDEV_COUNT);
    (void)cdev_del(&hcdev->cdev);

    hcdev->cdev_class = NULL;
    hcdev->dev = NULL;
}

#ifdef CFG_FEATURE_VFIO
int hdccom_rx_comm_msg_para_check(u32 dev_id, u32 fid, const struct vmng_rx_msg_proc_info *proc_info)
{
    /* if struct vhdc_ctrl_msg add member before enum VHDC_CTRL_MSG_TYPE, here shoule be changed */
    int ctrl_msg_head = sizeof(enum VHDC_CTRL_MSG_TYPE) + sizeof(int);

    if ((proc_info == NULL) || (proc_info->real_out_len == NULL) || (proc_info->data == NULL) ||
        (dev_id >= VMNG_PDEV_MAX) || (fid >= VMNG_VDEV_MAX_PER_PDEV)) {
        hdcdrv_err("Input parameter is error. (dev_id=%u; fid=%u)\n", dev_id, fid);
        return HDCDRV_PARA_ERR;
    }

    if ((proc_info->in_data_len < ctrl_msg_head) || (proc_info->out_data_len < ctrl_msg_head)) {
        hdcdrv_err("Input parameter is error. (in_data_len=%u; out_data_len=%u)\n",
            proc_info->in_data_len, proc_info->out_data_len);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

int hdccom_rx_vpc_msg_para_check(u32 dev_id, u32 fid, const struct vmng_rx_msg_proc_info *proc_info)
{
    if ((proc_info == NULL) || (proc_info->real_out_len == NULL) || (proc_info->data == NULL) ||
        (dev_id >= VMNG_PDEV_MAX) || (fid >= VMNG_VDEV_MAX_PER_PDEV)) {
        hdcdrv_err("Input parameter is error. (dev_id=%u; fid=%u)\n", dev_id, fid);
        return HDCDRV_PARA_ERR;
    }

    if ((proc_info->in_data_len < (sizeof(struct vhdc_ioctl_msg) - sizeof(union hdcdrv_cmd))) ||
        (proc_info->out_data_len < (sizeof(struct vhdc_ioctl_msg) - sizeof(union hdcdrv_cmd)))) {
        hdcdrv_err("Input parameter is error. (in_data_len=%u; out_data_len=%u)\n",
            proc_info->in_data_len, proc_info->out_data_len);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

int hdccom_rx_comm_msg_type_check(unsigned int cmd_min_len, const struct vmng_rx_msg_proc_info *proc_info)
{
    /* if struct vhdc_ctrl_msg change member before enum VHDC_CTRL_MSG_TYPE, here shoule be changed */
    int ctrl_msg_head = sizeof(enum VHDC_CTRL_MSG_TYPE) + sizeof(int);

    if (((cmd_min_len + ctrl_msg_head) > proc_info->in_data_len) ||
        ((cmd_min_len + ctrl_msg_head) > proc_info->out_data_len)) {
        hdcdrv_err("Input parameter is error. (in_data_len=%u; out_data_len=%u)\n",
            proc_info->in_data_len, proc_info->out_data_len);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

int hdccom_rx_vpc_cmd_type_check(unsigned int cmd, const struct vmng_rx_msg_proc_info *proc_info)
{
    u32 drv_cmd = _IOC_NR(cmd);
    u32 vhdc_ioctl_msg_head;

    if (drv_cmd >= HDCDRV_CMD_MAX) {
        hdcdrv_err_limit("Command is illegal. (cmd=%u)\n", drv_cmd);
        return HDCDRV_PARA_ERR;
    }

    vhdc_ioctl_msg_head = sizeof(struct vhdc_ioctl_msg) - sizeof(union hdcdrv_cmd);
    if (((vhdc_ioctl_msg_head + hdcdrv_cmd_size_table[drv_cmd]) > proc_info->in_data_len) ||
        ((vhdc_ioctl_msg_head + hdcdrv_cmd_size_table[drv_cmd]) > proc_info->out_data_len)) {
        hdcdrv_err("Parameter cmd is illegal. (drv_cmd=%d)\n", drv_cmd);
        return HDCDRV_PARA_ERR;
    }

    return HDCDRV_OK;
}

void hdccom_fill_cmd_size_table(void)
{
    hdcdrv_cmd_size_table[HDCDRV_CMD_GET_PEER_DEV_ID] = sizeof(struct hdcdrv_cmd_get_peer_dev_id);
    hdcdrv_cmd_size_table[HDCDRV_CMD_CONFIG] = sizeof(struct hdcdrv_cmd_config);
    hdcdrv_cmd_size_table[HDCDRV_CMD_SET_SERVICE_LEVEL] = sizeof(struct hdcdrv_cmd_set_service_level);
    hdcdrv_cmd_size_table[HDCDRV_CMD_CLIENT_DESTROY] = sizeof(struct hdcdrv_cmd_client_destroy);
    hdcdrv_cmd_size_table[HDCDRV_CMD_SERVER_CREATE] = sizeof(struct hdcdrv_cmd_server_create);
    hdcdrv_cmd_size_table[HDCDRV_CMD_SERVER_DESTROY] = sizeof(struct hdcdrv_cmd_server_destroy);
    hdcdrv_cmd_size_table[HDCDRV_CMD_ACCEPT] = sizeof(struct hdcdrv_cmd_accept);
    hdcdrv_cmd_size_table[HDCDRV_CMD_CONNECT] = sizeof(struct hdcdrv_cmd_connect);
    hdcdrv_cmd_size_table[HDCDRV_CMD_CLOSE] = sizeof(struct hdcdrv_cmd_close);
    hdcdrv_cmd_size_table[HDCDRV_CMD_SEND] = sizeof(struct hdcdrv_cmd_send);
    hdcdrv_cmd_size_table[HDCDRV_CMD_RECV_PEEK] = sizeof(struct hdcdrv_cmd_recv_peek);
    hdcdrv_cmd_size_table[HDCDRV_CMD_RECV] = sizeof(struct hdcdrv_cmd_recv);
    hdcdrv_cmd_size_table[HDCDRV_CMD_SET_SESSION_OWNER] = sizeof(struct hdcdrv_cmd_set_session_owner);
    hdcdrv_cmd_size_table[HDCDRV_CMD_GET_SESSION_ATTR] = sizeof(struct hdcdrv_cmd_get_session_attr);
    hdcdrv_cmd_size_table[HDCDRV_CMD_SET_SESSION_TIMEOUT] = sizeof(struct hdcdrv_cmd_set_session_timeout);
    hdcdrv_cmd_size_table[HDCDRV_CMD_GET_SESSION_UID] = sizeof(struct hdcdrv_cmd_get_uid_stat);
    hdcdrv_cmd_size_table[HDCDRV_CMD_GET_PAGE_SIZE] = sizeof(struct hdcdrv_cmd_get_page_size);
    hdcdrv_cmd_size_table[HDCDRV_CMD_GET_SESSION_INFO] = sizeof(struct hdcdrv_cmd_get_session_info);
    hdcdrv_cmd_size_table[HDCDRV_CMD_ALLOC_MEM] = sizeof(struct hdcdrv_cmd_alloc_mem);
    hdcdrv_cmd_size_table[HDCDRV_CMD_FREE_MEM] = sizeof(struct hdcdrv_cmd_free_mem);
    hdcdrv_cmd_size_table[HDCDRV_CMD_FAST_SEND] = sizeof(struct hdcdrv_cmd_fast_send);
    hdcdrv_cmd_size_table[HDCDRV_CMD_FAST_RECV] = sizeof(struct hdcdrv_cmd_fast_recv);
    hdcdrv_cmd_size_table[HDCDRV_CMD_DMA_MAP] = sizeof(struct hdcdrv_cmd_dma_map);
    hdcdrv_cmd_size_table[HDCDRV_CMD_DMA_UNMAP] = sizeof(struct hdcdrv_cmd_dma_unmap);
    hdcdrv_cmd_size_table[HDCDRV_CMD_DMA_REMAP] = sizeof(struct hdcdrv_cmd_dma_remap);
    hdcdrv_cmd_size_table[HDCDRV_CMD_EPOLL_ALLOC_FD] = sizeof(struct hdcdrv_cmd_epoll_alloc_fd);
    hdcdrv_cmd_size_table[HDCDRV_CMD_EPOLL_FREE_FD] = sizeof(struct hdcdrv_cmd_epoll_free_fd);
    hdcdrv_cmd_size_table[HDCDRV_CMD_EPOLL_CTL] = sizeof(struct hdcdrv_cmd_epoll_ctl);
    hdcdrv_cmd_size_table[HDCDRV_CMD_EPOLL_WAIT] = sizeof(struct hdcdrv_cmd_epoll_wait);
    hdcdrv_cmd_size_table[HDCDRV_CMD_CLIENT_WAKEUP_WAIT] = sizeof(struct hdcdrv_cmd_client_wakeup_wait);
    hdcdrv_cmd_size_table[HDCDRV_CMD_SERVER_WAKEUP_WAIT] = sizeof(struct hdcdrv_cmd_server_wakeup_wait);
}
#endif

int hdcdrv_send_mem_info(struct hdcdrv_fast_mem *mem, int devid, int flag)
{
    int ret;
    int i;
    u32 len = 0;
    u32 msg_size;
    struct hdcdrv_ctrl_msg_sync_mem_info *msg = NULL;
    struct mutex *sync_mem_mutex = hdcdrv_get_sync_mem_lock(devid);
#ifdef CFG_FEATURE_PFSTAT
    u64 time_stamp;
#endif

    if (!hdcdrv_mem_is_notify(mem)) {
        return HDCDRV_OK;
    }

    mutex_lock(sync_mem_mutex);

    msg_size = sizeof(struct hdcdrv_ctrl_msg_sync_mem_info) + mem->phy_addr_num * sizeof(struct hdcdrv_dma_mem);
    msg = (struct hdcdrv_ctrl_msg_sync_mem_info *)hdcdrv_get_sync_mem_buf(devid);
    if (msg == NULL) {
        mutex_unlock(sync_mem_mutex);
        hdcdrv_err("sync mem buf already free. (devid=%d)\n", devid);
        return HDCDRV_SEND_CTRL_MSG_FAIL;
    }

    msg->error_code = HDCDRV_OK;
    msg->type = HDCDRV_CTRL_MSG_TYPE_SYNC_MEM_INFO;
    msg->flag = flag;
    msg->phy_addr_num = mem->phy_addr_num;
    msg->alloc_len = mem->alloc_len;
    msg->mem_type = mem->mem_type;
    msg->pid = (long long)(mem->hash_va & HDCDRV_FRBTREE_PID_MASK);
    msg->hash_va = mem->hash_va;
#ifdef CFG_FEATURE_HDC_REG_MEM
    msg->align_size = mem->align_size;
    msg->register_offset = mem->register_inner_page_offset;
    msg->user_va = mem->user_va;
#endif

    for (i = 0; i < msg->phy_addr_num; i++) {
        msg->mem[i].addr = mem->mem[i].addr;
        msg->mem[i].len = mem->mem[i].len;
        msg->mem[i].resv = 0;
    }

#ifdef CFG_FEATURE_PFSTAT
    hdcdrv_pfstat_get_timestamp(&time_stamp);
#endif
    ret = (int)hdcdrv_non_trans_ctrl_msg_send((u32)devid, (void *)msg, msg_size, msg_size, &len);
    if ((ret != HDCDRV_OK) || (len != sizeof(struct hdcdrv_ctrl_msg_sync_mem_info)) ||
        (msg->error_code != HDCDRV_OK)) {
        hdcdrv_limit_exclusive(err, HDCDRV_LIMIT_LOG_0x0B, "Memory infotmation message send failed. (dev_id=%d; "
            "ret=%d; len=%d; error_code=%d)\n", devid, ret, len, msg->error_code);
        ret = HDCDRV_SEND_CTRL_MSG_FAIL;
    }
#ifdef CFG_FEATURE_PFSTAT
    hdcdrv_pfstat_update_latency(HDCDRV_PFSTATE_NON_TRANS_CHAN_ID, SYNC_MSG_LATENCY, time_stamp);
#endif
    mutex_unlock(sync_mem_mutex);

    return ret;
}

void hdcdrv_node_status_busy(struct hdcdrv_fast_node *node)
{
    node->stamp = jiffies;
    atomic_inc(&node->status);
}

void hdcdrv_node_status_idle(struct hdcdrv_fast_node *node)
{
    u32 cost_time;

    if (node->stamp != 0) {
        cost_time = jiffies_to_msecs(jiffies - node->stamp);
        if (cost_time > node->max_cost) {
            node->max_cost = cost_time;
        }

        if (cost_time > HDCDRV_NODE_BUSY_WARNING) {
            hdcdrv_info_limit_spinlock("cost_time is invalid. (cost_time=%ums; max_cost=%ums)\n",
                cost_time, node->max_cost);
        }
    }

    node->stamp = 0;
    if (atomic_read(&node->status) > HDCDRV_NODE_IDLE) {
        atomic_dec(&node->status);
    }
}

void hdcdrv_node_status_init(struct hdcdrv_fast_node *node)
{
    node->stamp = 0;
    atomic_set(&node->status, HDCDRV_NODE_IDLE);
    node->unregister_flag = 0;
}

void hdcdrv_node_status_idle_by_mem(struct hdcdrv_fast_mem *f_mem)
{
    struct hdcdrv_fast_node *f_node = NULL;

    if (f_mem != NULL) {
        f_node = container_of(f_mem, struct hdcdrv_fast_node, fast_mem);
        hdcdrv_node_status_idle(f_node);
    }
}

bool hdcdrv_node_is_busy(const struct hdcdrv_fast_node *node)
{
    if (atomic_read(&node->status) >= HDCDRV_NODE_BUSY) {
        return true;
    } else {
        return false;
    }
}

bool hdcdrv_node_is_timeout(int node_stamp)
{
    if (jiffies_to_msecs(jiffies - node_stamp) > HDCDRV_NODE_BUSY_TIMEOUT) {
        return true;
    } else {
        return false;
    }
}

void hdcdrv_add_to_async_ctx(struct hdcdrv_ctx_fmem *async_ctx, struct hdcdrv_fast_node *f_node)
{
    spin_lock_bh(&async_ctx->mem_lock);
    list_add(&f_node->mem_fd_node->list, &async_ctx->mlist.list);
    async_ctx->mem_count++;
    spin_unlock_bh(&async_ctx->mem_lock);
}

STATIC void hdcdrv_fill_fast_node_info(struct hdcdrv_mem_node_info *f_info, struct hdcdrv_fast_node *f_node)
{
    f_info->hash_va = f_node->hash_va;
    f_info->pid = f_node->pid;
    f_info->mem_type = f_node->fast_mem.mem_type;
    f_info->alloc_len = f_node->fast_mem.alloc_len;
    f_info->user_va = f_node->fast_mem.user_va;
}

void hdcdrv_bind_mem_ctx(struct hdcdrv_ctx_fmem *ctx_fmem, struct hdcdrv_fast_node *f_node,
    struct hdcdrv_mem_fd_list *new_node)
{
    new_node->f_node = f_node;
    new_node->ctx_fmem = ctx_fmem;

    spin_lock_bh(&ctx_fmem->mem_lock);
    list_add(&new_node->list, &ctx_fmem->mlist.list);
    hdcdrv_fill_fast_node_info(&new_node->f_info, f_node);
    f_node->mem_fd_node = new_node;
    ctx_fmem->mem_count++;
    spin_unlock_bh(&ctx_fmem->mem_lock);
}

void hdcdrv_unbind_mem_ctx(struct hdcdrv_fast_node *f_node)
{
    struct hdcdrv_mem_fd_list *node = f_node->mem_fd_node;
    struct hdcdrv_ctx_fmem *ctx_fmem = NULL;

    if (node != NULL) {
        ctx_fmem = node->ctx_fmem;

        spin_lock_bh(&ctx_fmem->mem_lock);
        list_del(&node->list);
        ctx_fmem->mem_count--;
        f_node->mem_fd_node->ctx_fmem = NULL;
        spin_unlock_bh(&ctx_fmem->mem_lock);
        hdcdrv_kfree(node, KA_SUB_MODULE_TYPE_1);
    }

    f_node->mem_fd_node = NULL;
}

STATIC void hdcdrv_count_mem_info(const struct hdcdrv_mem_node_info *f_info, struct hdcdrv_mem_stat *mem_info)
{
    if ((f_info->mem_type >= 0) && (f_info->mem_type < HDCDRV_FAST_MEM_TYPE_MAX)) {
        mem_info->mem_nums[f_info->mem_type]++;
        mem_info->mem_size[f_info->mem_type] += f_info->alloc_len;
    }
}

struct hdcdrv_mem_fd_list *hdcdrv_release_get_free_mem_entry(struct hdcdrv_ctx_fmem *ctx_fmem)
{
    struct hdcdrv_mem_fd_list *entry = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;

    spin_lock_bh(&ctx_fmem->mem_lock);
    if (!list_empty_careful(&ctx_fmem->mlist.list)) {
        list_for_each_safe(pos, n, &ctx_fmem->mlist.list)
        {
            entry = list_entry(pos, struct hdcdrv_mem_fd_list, list);
            list_del(&entry->list);
            break;
        }
    }
    spin_unlock_bh(&ctx_fmem->mem_lock);

    return entry;
}

void hdcdrv_set_quice_release_flag(int *flag)
{
#ifdef CFG_FEATURE_HDC_REG_MEM
    *flag = HDCDRV_QUICK_RELEASE_FLAG;
#endif
}
bool hdcdrv_release_is_quick(int flag)
{
#ifdef CFG_FEATURE_HDC_REG_MEM
    return (flag == HDCDRV_QUICK_RELEASE_FLAG);
#else
    return false;
#endif
}

void hdcdrv_release_free_mem(struct hdcdrv_ctx_fmem *ctx_fmem)
{
    struct hdcdrv_mem_fd_list *entry = NULL;
    struct hdcdrv_mem_stat mem_info = {{0}};

    spin_lock_bh(&ctx_fmem->mem_lock);
    if ((list_empty_careful(&ctx_fmem->mlist.list)) || (ctx_fmem->mem_count == 0)) {
        spin_unlock_bh(&ctx_fmem->mem_lock);
        return;
    }
    spin_unlock_bh(&ctx_fmem->mem_lock);

    /* memory free */
    hdcdrv_info("Release memory. (task_pid=%llu; count=%llu)\n", hdcdrv_get_pid(), ctx_fmem->mem_count);
    usleep_range(HDCDRV_USLEEP_RANGE_2000, HDCDRV_USLEEP_RANGE_3000);

    while (1) {
        entry = hdcdrv_release_get_free_mem_entry(ctx_fmem);
        if (entry == NULL) {
            return;
        }

        hdcdrv_count_mem_info(&entry->f_info, &mem_info);
        if (hdcdrv_release_is_quick(ctx_fmem->quick_flag)) {
            hdcdrv_fast_mem_quick_proc(&entry->f_info);
        } else {
            hdcdrv_fast_mem_free_abnormal(&entry->f_info);
            hdcdrv_kfree(entry, KA_SUB_MODULE_TYPE_1);
        }
    }
}

int hdcdrv_mmap_param_check(const struct file *filep, const struct vm_area_struct *vma)
{
    if (filep == NULL) {
        hdcdrv_err("filep check failed\n");
        return -ENODEV;
    }

    if (filep->private_data == NULL) {
        hdcdrv_err("filep->private_data is NULL\n");
        return -EFAULT;
    }

    if ((vma == NULL) || (vma->vm_end <= vma->vm_start)) {
        hdcdrv_err("vm range check failed, vma->vm_end = 0x%lx, vma->vm_start = 0x%lx.\n",
            (vma == NULL ? 0x0 : vma->vm_end), (vma == NULL ? 0x0 : vma->vm_start));
        return -EINVAL;
    }

    return 0;
}
