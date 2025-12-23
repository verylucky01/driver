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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/device.h>
#include <linux/uio_driver.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/version.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/io.h>

#include "common.h"
#include "ioctl_comm.h"
#include "securec.h"
#include "pci-dev.h"
#include "ioctl_comm_def.h"
#include "ka_fs_pub.h"
#include "ka_pci_pub.h"
#include "ka_dfx_pub.h"
#include "ka_task_pub.h"
#include "ka_list_pub.h"
#include "ka_driver_pub.h"
#include "ka_system_pub.h"
#include "ka_memory_pub.h"
#include "ka_kernel_def_pub.h"

#define PCIE_DEV_NAME "lqdcmi_pcidev"

#define PCIE_TC_VENDOR_ID 0x19e5
#define PCIE_OLD_DEVICE_ID 0X1260
#define PCIE_NEW_DEVICE_ID 0xa526

#define MEM_IO_CMD 100  /* 通用命令字定义 */

#define MEM_OFFSET_HCCS_OVERFLOW_FLAG       9 /* 溢出标志在内存中位置 */
#define MEM_OFFSET_HCCS_STARTTIME_FLAG      6 /* 启动时间在内存中位置 */
#define PCI_BAR_ID 0x4
#define TOTAL_LEN (7 * 1025 * 256)
#define EXIST_FAULT_OFFSET 3145728
#define CPU_BAR_BASE_OFFSET 36864
#define EXIST_FAULT_FLAG_OFFSET 7340784
#define HCCS_BASE_ADDR 0x400020412000
#define HCCS_HEAD_OFFSET 30
#define HCCS_START_TIME_OFFSET 31
#define HCCS_OVERFLOW_FLAG_OFFSET 32
#define HCCS_MAINBOARD_HALF_CONFIG 0x1c
#define HCCS_MAINBOARD_FULL_CONFIG 0x1d
#define HCCS_FILE_PATH "/var/log/hccs_file.log"
#define RETAINED_FAULT_NUMS 50
#define LQ_OVERFLOW_OFFSET  9
#define FAULT_HEAD_OFFSET   4

#ifndef STATIC_SKIP
    #define STATIC static
#else
    #define STATIC
#endif

dev_t g_pci_devid = 0;

dev_t g_device_id = 0;
dev_t g_mainboardid = 0;
unsigned int g_hccs_head = 0;
void __iomem *g_hccs_virt_addr;
STATIC struct task_struct *g_kernel_update_thread = NULL;
bool g_main_board_id_initialized = false;

STATIC struct file *g_hccs_file;
KA_LIST_HEAD(g_drv_uio_list);

#ifndef LLT_LQDCMI
STATIC struct class *g_common_class = NULL; /* 设备类结构体 */
#else
STATIC struct ut_class *g_common_class = NULL; /* 设备类结构体 */
#endif

struct pcidev {
    struct cdev cdev;
    dev_t major;
};

typedef struct PCI_MEM_INFO {
    unsigned long *pmap_addr;
    unsigned int map_len;
} PCI_MEM_INFO_STRU;


PCI_MEM_INFO_STRU g_bmc_pci_mem;
PCI_MEM_INFO_STRU g_exist_fault_mem;
PCI_MEM_INFO_STRU g_bar_flag_mem;

struct pcidev *g_pcidev;

struct pci_dev_info {
    struct pci_dev *pdev;
    struct mutex lock;
    void __iomem *bar_mem_addr;
    unsigned long bar_mem_len; /* 长度为0 说明bar无效 */
    uint32_t bus;                           /* 这条 PCI 总线的总线编号 */
    uint32_t device;
    uint32_t function;
    uint32_t irq_num;
};

struct pci_dev_info g_tn30_info = {0};

SramDescCtlHeader g_fault_event_head = { 0 };
STATIC EventMapping mapping[] = EVENT_MAPPING_INITIALIZER;
FaultEventNodeTable *g_kernel_event_table = NULL;
struct pci_dev_info *g_tcpci_info = NULL;

unsigned int g_proc_id = 0;
unsigned int g_stop_readmem = 0;
struct mutex g_kernel_table_mutex;
struct mutex shared_mem_mutex;
struct mutex kernel_fault_mem_lock;

STATIC long comn_ioctl(__attribute__ ((unused)) struct file *file, uint32_t cmd, unsigned long arg)
{
    int ret = 0;
    struct pci_dev_info *pcidev;

    switch (cmd) {
        case MEM_IO_CMD: /* 通用命令字, 通过arg里面的命令字在做细分 */
            pcidev = &g_tn30_info;
            ret = pcidev_ioctl((void *)arg);
            break;
        default:
            printk(KA_KERN_ERR "[lqdcmi]cmd %d is not support\n", cmd);
            ret = -EINVAL;
            break;
    }

    return ret;
}

struct shared_memory {
    void *mem;               // 用于存储物理内存的指针
    size_t size;             // 内存大小
    struct list_head list;   // 链表节点，用于全局存储
};

STATIC KA_LIST_HEAD(shared_mem_list);

STATIC bool isHccsMode(void)
{
    return (g_mainboardid == HCCS_MAINBOARD_HALF_CONFIG || g_mainboardid == HCCS_MAINBOARD_FULL_CONFIG);
}

struct devdrv_info *(*devdrv_manager_get_devdrv_info_ptr)(u32 dev_id);

STATIC int get_main_board_id(void)
{
    u32 dev_id = 0;  // 假设我们要查询的设备 ID
    struct devdrv_info *info;
    devdrv_manager_get_devdrv_info_ptr = (struct devdrv_info *(*)(u32))__symbol_get("devdrv_manager_get_devdrv_info");
    if (!devdrv_manager_get_devdrv_info_ptr) {
        return -ENOENT;
    }

    // 调用获取的信息
    info = devdrv_manager_get_devdrv_info_ptr(dev_id);
    if (info) {
        g_mainboardid = info->mainboard_id;
        printk(KA_KERN_INFO "[lqdcmi]mainboardid = %x\n", g_mainboardid);
    } else {
        __ka_system_symbol_put("devdrv_manager_get_devdrv_info");
        return -ENOENT;
    }
    __ka_system_symbol_put("devdrv_manager_get_devdrv_info");
    return 0;
}

// 创建每个进程独立的共享内存
STATIC void *create_shared_memory(size_t size)
{
    void *memory = kmalloc(size, KA_GFP_KERNEL);  // 为每个进程分配独立内存
    if (!memory) {
        printk(KA_KERN_ERR "[lqdcmi]Failed to allocate memory\n");
        return NULL;
    }
    return memory;
}

// 映射共享内存到用户空间
STATIC int shared_memory_mmap(struct file *file, struct vm_area_struct *vma)
{
    struct shared_memory *shm = file->private_data;
    unsigned long pfn;
    int ret = 0;

    ka_task_mutex_lock(&shared_mem_mutex);

    if (!shm) {
        if (g_proc_id > MAX_PROCESS_NUM) {
            ret = -ENOSPC;
            printk(KA_KERN_ERR "[lqdcmi]shared_memory_mmap, proc_id is over limit, g_proc_id=%d\n", g_proc_id);
            goto out;
        }

        // 分配共享内存结构体
        shm = kzalloc(sizeof(*shm), KA_GFP_KERNEL);
        if (!shm) {
            ret = -ENOMEM;
            goto out;
        }

        // 分配共享内存区域
        shm->mem = create_shared_memory(SHM_SIZE);
        if (!shm->mem) {
            ka_mm_kfree(shm);
            ret = -ENOMEM;
            goto out;
        }

        // 初始化共享内存内容
        ret = memset_s(shm->mem, SHM_SIZE, 0, SHM_SIZE);
        if (ret != 0) {
            printk(KA_KERN_ERR "[lqdcmi]shared_memory_mmap memset_s fail\n");
            ka_mm_kfree(shm->mem);
            ka_mm_kfree(shm);
            ret = -ENOMEM;
            goto out;
        }

        shm->size = SHM_SIZE;
        file->private_data = shm;  // 将共享内存结构体与文件关联
        list_add(&shm->list, &shared_mem_list);

        g_proc_id++;
        printk(KA_KERN_INFO "[lqdcmi]shared_memory_mmap,g_proc_id = %d\n", g_proc_id);
        ka_mm_writel(g_proc_id, shm->mem + PROC_ID_POINTER_OFFSET * sizeof(int)); // 更新尾指针
    }

    pfn = ka_mm_virt_to_phys(shm->mem) >> KA_MM_PAGE_SHIFT;

    // 将内核的共享内存映射到用户空间
    ret =  ka_mm_remap_pfn_range(vma, vma->vm_start, pfn, shm->size, vma->vm_page_prot);

out:
    ka_task_mutex_unlock(&shared_mem_mutex);
    return ret;
}

// 设备释放操作
STATIC int shared_memory_release(struct inode *inode, struct file *file)
{
    struct shared_memory *shm = file->private_data;

    ka_task_mutex_lock(&shared_mem_mutex);

    if (shm) {
        // 从全局链表中移除
        ka_list_del(&shm->list);

        // 释放共享内存
        if (shm->mem) {
            ka_mm_kfree(shm->mem);  // 释放内存
        }

        // 释放共享内存结构体
        ka_mm_kfree(shm);
        file->private_data = NULL;
        g_proc_id--;

        printk(KA_KERN_INFO "[lqdcmi]shared_memory_release,g_proc_id = %d\n", g_proc_id);
    }
    ka_task_mutex_unlock(&shared_mem_mutex);
    return 0;
}

STATIC int write_shared_memory(SramFaultEventData *sd)
{
    struct shared_memory *shm;
    unsigned int i;
    unsigned int *node_data;

    if (list_empty(&shared_mem_list)) {
        return -1;  // 列表为空
    }
    ka_list_for_each_entry(shm, &shared_mem_list, list) {
        if (shm->mem) {
            int head = ka_mm_readl(shm->mem);  // 从共享内存的偏移 0 处获取头部
            int tail = ka_mm_readl(shm->mem + sizeof(int));   // 从共享内存的偏移 4 处获取尾部
            // 检查队列是否满
            if ((tail + 1) % g_fault_event_head.nodeNum == head) {
                printk(KA_KERN_ERR "[lqdcmi]Queue is full, cannot write to shared memory\n");
                return -1;  // 队列已满
            }

            // 将 SramFaultEventData 转换为 unsigned int 数组并写入共享内存
            node_data = (unsigned int *)sd;  // 将 SramFaultEventData 转换为 unsigned int 数组
            for (i = 0; i < sizeof(SramFaultEventData) / sizeof(unsigned int); i++) {
                ka_mm_writel(node_data[i],
                    shm->mem + sizeof(SramFaultEventData) + tail * sizeof(SramFaultEventData) + i * sizeof(unsigned int));
            }

            // 更新尾部计数
            tail = (tail + 1) % g_fault_event_head.nodeNum;  // 循环更新尾指针
            ka_mm_writel(tail, shm->mem + sizeof(int)); // 更新尾指针
        } else {
            printk(KA_KERN_ERR "[lqdcmi]not have mem\n");
            return -1;
        }
    }

    return 0;
}

STATIC int pcidev_release(struct inode *pinode, struct file* pfile)
{
    shared_memory_release(pinode, pfile);
    return 0;
}

STATIC int pcidev_open(struct inode *pinode, struct file *pfile)
{
    return 0;
}

STATIC struct pci_device_id g_tc_pci_tbl[] = {
    {
        .vendor = PCIE_TC_VENDOR_ID,
        .device = PCIE_OLD_DEVICE_ID,
        .subvendor = KA_PCI_ANY_ID,
        .subdevice = KA_PCI_ANY_ID,
    },
    {
        .vendor = PCIE_TC_VENDOR_ID,
        .device = PCIE_NEW_DEVICE_ID,
        .subvendor = KA_PCI_ANY_ID,
        .subdevice = KA_PCI_ANY_ID,
    },

    { 0, }, /* terminate list */
};

STATIC void copy_fault_from_mem(void)
{
    unsigned int ret = 0;
    unsigned int lqoverflow_flag = 0;
    unsigned int reset_flag = 0;
    unsigned int head, tail, num, diff, steps;
    size_t totalSize;

    if (g_tcpci_info->bar_mem_addr == NULL) {
        printk(KA_KERN_ERR "[lqdcmi]bar_mem_addr is NULL\n");
        return;
    }

    if (g_exist_fault_mem.pmap_addr == NULL) {
        printk(KA_KERN_ERR "[lqdcmi]ka_mm_ioremap failed\n");
        return;
    }

    // overflow_flag表示队列是否有溢出，0表示没有，1表示有、需要修改头和尾
    lqoverflow_flag = ka_mm_readl(g_tcpci_info->bar_mem_addr + sizeof(unsigned int) * LQ_OVERFLOW_OFFSET);
    printk(KA_KERN_INFO "[lqdcmi]copy_fault_from_mem lqoverflowflag: %u\n", lqoverflow_flag);

    if (lqoverflow_flag != 0) {
        ka_mm_memset_io(g_exist_fault_mem.pmap_addr, 0, g_exist_fault_mem.map_len);
        ka_mm_writel(0, g_tcpci_info->bar_mem_addr + sizeof(unsigned int) * LQ_OVERFLOW_OFFSET); // 将标志位0写入寄存器中，表示溢出已经处理完成。
        update_mem_by_map();

        num = g_fault_event_head.nodeNum;
        head = g_fault_event_head.nodeHead;
        tail = g_fault_event_head.nodeTail;
        diff = (tail - head + num) % num;

        if (diff < RETAINED_FAULT_NUMS) {
            steps = RETAINED_FAULT_NUMS - diff;
            head = (head - steps + num) % num;
            ka_mm_writel(head, g_tcpci_info->bar_mem_addr + sizeof(unsigned int) * FAULT_HEAD_OFFSET); // 更新头节点位置地址偏移
        }
    }

    reset_flag = ka_mm_readl(g_bar_flag_mem.pmap_addr);
    printk(KA_KERN_INFO "[lqdcmi]reset flag: %u\n", reset_flag);
    if (reset_flag != 0x5A5A5A) {
        ka_mm_memset_io(g_exist_fault_mem.pmap_addr, 0, g_exist_fault_mem.map_len);
        if (ret != 0) {
            printk(KA_KERN_ERR "[lqdcmi]copy_fault_from_mem ka_mm_memset_io fail\n");
            return;
        }
        ka_mm_writel(0x5A5A5A, g_bar_flag_mem.pmap_addr);
        return;
    }

    totalSize = CAPACITY * sizeof(FaultEventNodeTable);
    ret = memcpy_s(g_kernel_event_table, totalSize, g_exist_fault_mem.pmap_addr, totalSize);
    if (ret != 0) {
        printk(KERN_ERR "[lqdcmi]copy_fault_from_mem memset_s fail\n");
    }
}

STATIC int initAndStartThreadForPcie(void);
STATIC void stopAndCleanupThread(void);

// PCIe设备Probe函数
STATIC int tc_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
    int ret = 0;
    printk(KA_KERN_INFO "[lqdcmi]tc_pci_probe");

    if (g_stop_readmem) {
        copy_fault_from_mem();
        ret = initAndStartThreadForPcie();
        if (ret != 0) {
            printk(KA_KERN_ERR "[lqdcmi]initAndStartThreadForPcie fail\n");
        }
        g_stop_readmem = 0;
        printk(KA_KERN_INFO "[lqdcmi]start to read fault mem\n");
    }
    return ret;
}

STATIC void tc_pci_remove(struct pci_dev *pdev)
{
    printk(KA_KERN_INFO "[lqdcmi]tc_pci_remove");
    stopAndCleanupThread();
    printk(KA_KERN_INFO "[lqdcmi]stop to read fault mem\n");
    g_stop_readmem = 1;
}

STATIC struct pci_driver g_pci_driver = {
    .name = PCIE_DEV_NAME,
    .id_table = g_tc_pci_tbl,
    .probe = tc_pci_probe,
    .remove = tc_pci_remove,
};

struct file_operations g_pcidev_fops = {
    .owner = KA_THIS_MODULE,
    .open = pcidev_open,
    .release = pcidev_release,
    .unlocked_ioctl = comn_ioctl,
    .mmap = shared_memory_mmap,
};

STATIC struct cdev tc_pci_cdev = {
    .owner = KA_THIS_MODULE,
    .ops   = &g_pcidev_fops,
};

STATIC int show_bar_map(struct pci_dev *pdev)
{
    unsigned int i;
    unsigned int bar_flag = 0;
    unsigned int data;

    struct pci_dev_info *tc_pci_dev = ka_pci_get_drvdata(pdev);

    if (g_bmc_pci_mem.pmap_addr == NULL || tc_pci_dev->bar_mem_addr == NULL) {
        printk(KA_KERN_ERR "[lqdcmi]map info is null\n");
        return -1;
    }

    data = ka_mm_readl(tc_pci_dev->bar_mem_addr);
    printk(KA_KERN_INFO "[lqdcmi]data = 0x%x\n", data);
    printk(KA_KERN_INFO "[lqdcmi]bar buf =  \n");
    for (i = 0; i < 64; i++) { // 64 表示，dfx调试时用于分析前 256位信息是否正确
        data = ka_mm_readl(tc_pci_dev->bar_mem_addr + sizeof(unsigned int) * i);
        printk(KA_KERN_INFO "[lqdcmi]0x%x\t", data);
    }

    g_tcpci_info = tc_pci_dev;
    bar_flag = ka_mm_readl(g_tcpci_info->bar_mem_addr + sizeof(unsigned int));
    printk(KA_KERN_INFO "[lqdcmi]bar_flag:0x%x\n", bar_flag);

    if (bar_flag != 0x00000100) {
        printk(KA_KERN_ERR "[lqdcmi]bar_flag is not 0x00000100\n");
        return -1;
    }

    return 0;
}

STATIC void pci_bar_free(struct pci_dev *pdev)
{
    int i;
    struct pci_dev_info *tc_pci_dev = ka_pci_get_drvdata(pdev);

    if (tc_pci_dev->bar_mem_addr != NULL) {
        ka_mm_iounmap(tc_pci_dev->bar_mem_addr);
        tc_pci_dev->bar_mem_addr = NULL;
        tc_pci_dev->bar_mem_len = 0;
        printk(KA_KERN_ERR "[lqdcmi]bar[%d] mem free OK\n", i);
    }
}

STATIC int tc_pci_dev_info_init(struct pci_dev *pdev)
{
    /* 向内核申请资源 */
    int ret;
    struct pci_dev_info *tc_pci_dev = &g_tn30_info;

    tc_pci_dev->bus = pdev->bus->number;
    tc_pci_dev->device = 0;
    tc_pci_dev->function = 0;
    tc_pci_dev->pdev = pdev;
    tc_pci_dev->irq_num = 0;
    tc_pci_dev->bar_mem_addr = g_bmc_pci_mem.pmap_addr;
    tc_pci_dev->bar_mem_len = g_bmc_pci_mem.map_len;

    ka_pci_set_drvdata(pdev, tc_pci_dev);
    /* 向内核申请资源 */
    ret = pci_request_selected_regions(pdev, (1 << PCI_BAR_ID), PCIE_DEV_NAME);
    if (ret) {
        printk(KA_KERN_ERR "[lqdcmi]pci device request regions err %d\n", ret);
        pci_bar_free(pdev);
        return ret;
    }

    printk(KA_KERN_INFO "[lqdcmi]tc_pci_dev_info_init ok\n");
    return 0;
}

STATIC void pci_cleanup(void)
{
    dev_t devid = g_pci_devid;
    struct pci_dev *pdev = NULL;
    struct pci_dev_info *tc_pci_dev;

    pdev = pci_get_device(PCIE_TC_VENDOR_ID, g_device_id, NULL);
    tc_pci_dev = ka_pci_get_drvdata(pdev);

    if (tc_pci_dev->bar_mem_addr != NULL) {
        ka_mm_iounmap(tc_pci_dev->bar_mem_addr);
        tc_pci_dev->bar_mem_addr = NULL;
        tc_pci_dev->bar_mem_len = 0;
        ka_pci_release_selected_regions(pdev, (1 << PCI_BAR_ID));
        printk(KA_KERN_ERR "[lqdcmi]bar[0] mem free OK\n");
    }
    if (g_exist_fault_mem.pmap_addr != NULL) {
        ka_mm_iounmap(g_exist_fault_mem.pmap_addr);
        g_exist_fault_mem.pmap_addr = NULL;
        g_exist_fault_mem.map_len = 0;
        printk(KA_KERN_ERR "[lqdcmi]exist fault mem free OK\n");
    }
    if (g_bar_flag_mem.pmap_addr != NULL) {
        ka_mm_iounmap(g_bar_flag_mem.pmap_addr);
        g_bar_flag_mem.pmap_addr = NULL;
        g_bar_flag_mem.map_len = 0;
        printk(KA_KERN_ERR "[lqdcmi]bar flag mem free OK\n");
    }

    pci_unregister_driver(&g_pci_driver);

    device_destroy(g_common_class, devid);
    ka_driver_class_destroy(g_common_class);

    unregister_chrdev_region(devid, 1);

    g_common_class = NULL;
}

/* 在设备初始化阶段完成设备bar空间地址的映射，后续读写 */
STATIC int tc_pci_map(void)
{
    struct pci_dev *dev = NULL;
    unsigned long resource_flag = 0;
    unsigned int bar_reg_base = 0;
    unsigned long bar_cpu_base_add;
    unsigned int bar_cpu_add_len  = 0;

    unsigned long bar_exist_fault_add;
    unsigned int bar_exist_fault_add_len  = 0;

    unsigned long bar_flag_add;
    unsigned int bar_flag_add_len  = 0;
    unsigned long base_addr;
    unsigned int base_addr_len  = 0;

    dev  = pci_get_device(PCIE_TC_VENDOR_ID, g_device_id, NULL);
    if (NULL == dev) {
        printk(KA_KERN_ERR "[lqdcmi]find TC PCI device fail\n");
        return -EINVAL;
    }

    resource_flag = ka_pci_resource_flags(dev, 0);
    if (!(resource_flag & IORESOURCE_MEM)) {
        printk(KA_KERN_ERR "[lqdcmi]pci space is not I/O space, abort!\n");
        return -ENODEV;
    }

    base_addr = pci_resource_start(dev, 0); /* 获取的地址 cpu不能直接访问，需要映射为虚拟地址 */
    base_addr_len = pci_resource_len(dev, 0);

    if (base_addr_len < TOTAL_LEN || base_addr_len < CAPACITY * sizeof(FaultEventNodeTable) ||
        base_addr_len < sizeof(unsigned long)) {
        printk(KA_KERN_ERR "[lqdcmi]pci have no enough space, abort!\n");
        return -ENODEV;
    }
    /*
     *  内核态获取 bar 空间全量内存信息
     *  memcpy --> bar空间信息 --> g_memmap_info[4096]  -----> 疑问1 ：?? memcpy 方式导致内核卡死
     *  用户态获取队列内容信息 --> ioctl --> cmd_info --> 解析 g_memmap_info --> 封装队列信息 --> outbuffer --> copy_to_user()
     *
     * iotctl 接口 --> 用户态获取虚拟地址信息
     *             --> 获取队列信息
     *             --> 维护一张全局变量表(table)，用户态只读
     */
    bar_cpu_base_add = base_addr + sizeof(unsigned char) * CPU_BAR_BASE_OFFSET;  // 得到CPU bar0的基地址
    printk(KA_KERN_INFO "[lqdcmi]after bar0_cpu_base_add=%lx\n", bar_cpu_base_add);

    bar_exist_fault_add = base_addr + sizeof(unsigned char) * EXIST_FAULT_OFFSET ;  // 得到原有存在的故障信息的基地址
    printk(KA_KERN_INFO "[lqdcmi]after bar_exist_fault_add=%lx\n", bar_exist_fault_add);

    bar_flag_add = base_addr + sizeof(unsigned char) * EXIST_FAULT_FLAG_OFFSET ;  // 得到flag存储地址
    printk(KA_KERN_INFO "[lqdcmi]after bar_flag_add=%lx\n", bar_flag_add);

    bar_cpu_add_len = TOTAL_LEN;
    bar_exist_fault_add_len = CAPACITY * sizeof(FaultEventNodeTable);
    bar_flag_add_len = sizeof(unsigned long);

    g_bmc_pci_mem.pmap_addr = (unsigned long *)ka_mm_ioremap(bar_cpu_base_add, bar_cpu_add_len);
    g_bmc_pci_mem.map_len = TOTAL_LEN;
    if (g_bmc_pci_mem.pmap_addr == NULL) {
        printk(KA_KERN_ERR "[lqdcmi]ioremap BAR[4] reg base[0x%ux], size[0x%ux] failed\n", bar_reg_base, bar_cpu_add_len);
        return -ENOMEM;
    }

    g_exist_fault_mem.pmap_addr = (unsigned long *)ka_mm_ioremap(bar_exist_fault_add, bar_exist_fault_add_len);
    g_exist_fault_mem.map_len = bar_exist_fault_add_len;
    if (g_exist_fault_mem.pmap_addr == NULL) {
        printk(KA_KERN_ERR "[lqdcmi]ioremap BAR[4] reg base[0x%ux], size[0x%ux] failed\n",
               bar_reg_base, bar_exist_fault_add_len);
        goto fault_err;
    }

    g_bar_flag_mem.pmap_addr = (unsigned long *)ka_mm_ioremap(bar_flag_add, bar_flag_add_len);
    g_bar_flag_mem.map_len = bar_flag_add_len;
    if (g_bar_flag_mem.pmap_addr == NULL) {
        printk(KA_KERN_ERR "[lqdcmi]ioremap BAR[4] reg base[0x%ux], size[0x%ux] failed\n", bar_reg_base, bar_flag_add_len);
        goto bar_err;
    }

    printk(KA_KERN_INFO "[lqdcmi]bmc_bar_addr:0x%lx , len:0x%x , virtual_add:%p\n", bar_cpu_base_add,
           TOTAL_LEN, (void *)g_bmc_pci_mem.pmap_addr);
    tc_pci_dev_info_init(dev);
    if (show_bar_map(dev) != 0) {
        return -ENOMEM;
    }
    return 0;
bar_err:
    iounmap((void *)(g_exist_fault_mem.pmap_addr));
    g_exist_fault_mem.pmap_addr = NULL;
fault_err:
    iounmap((void *)(g_bmc_pci_mem.pmap_addr));
    g_bmc_pci_mem.pmap_addr = NULL;

    return -ENOMEM;
}

STATIC int tc_pci_enable(void)
{
    struct pci_dev *dev = NULL;

    dev = pci_get_device(PCIE_TC_VENDOR_ID, PCIE_OLD_DEVICE_ID, NULL);  // 寻找deviceid为1260的设备
    if (dev == NULL) {
        dev = pci_get_device(PCIE_TC_VENDOR_ID, PCIE_NEW_DEVICE_ID, NULL);  // 寻找deviceid为a526的设备
        if (dev == NULL) {
            printk(KA_KERN_ERR "[lqdcmi]find TC PCI device fail\n");
            return -EINVAL;
        }
    }

    g_device_id = dev->device;
    printk(KA_KERN_INFO "[lqdcmi]Device id is %x\n", g_device_id);

    if (ka_pci_enable_device(dev)) {
        printk(KA_KERN_ERR "[lqdcmi]Not possible to enable PCI Device\n");
        return -ENODEV;
    }

    return 0;
}

/*
* pcie设备初始化流程，1630侧busid分配，枚举由bios完成，启动时已建链，此处的初始化流程仅给pcie控制器使能，保证可读可写
*/
STATIC int pci_init(void)
{
    int res  = tc_pci_enable();
    if (res != 0) {
        printk(KA_KERN_ERR "[lqdcmi]pci init fail\n");
        return res;
    }
    /* 这里不返回结果是由于调试过程中需要反复插入，存在已注册的情况，不影响后续功能的调试 */
    res = ka_pci_register_driver(&g_pci_driver);
    if (res) {
        printk(KA_KERN_ERR "[lqdcmi]pci init register driver fail.\n");
    }
    return 0;
}

void update_mem_by_map(void)
{
    ka_task_mutex_lock(&kernel_fault_mem_lock);

    if (isHccsMode()) {
        if (g_hccs_virt_addr == NULL) {
            printk(KA_KERN_ERR "[lqdcmi]bar_mem_addr is NULL\n");
            ka_task_mutex_unlock(&kernel_fault_mem_lock);
            return;
        }
        /* 解析头部 */
        g_fault_event_head.version = ka_mm_readl(g_hccs_virt_addr);
        g_fault_event_head.length = ka_mm_readl(g_hccs_virt_addr + sizeof(unsigned int));
        g_fault_event_head.nodeSize = ka_mm_readl(g_hccs_virt_addr + sizeof(unsigned int) * 2); // 2表示偏移变量位置
        g_fault_event_head.nodeNum = ka_mm_readl(g_hccs_virt_addr + sizeof(unsigned int) * 3); // 3表示偏移变量位置
        g_fault_event_head.nodeHead = ka_mm_readl(g_hccs_virt_addr + sizeof(unsigned int) * 4); // 4表示偏移变量位置
        g_fault_event_head.nodeTail = ka_mm_readl(g_hccs_virt_addr + sizeof(unsigned int) * 5); // 5表示偏移变量位置
        g_fault_event_head.startTimeMs = ka_mm_readl(g_hccs_virt_addr + sizeof(unsigned int) * 6); // 6表示偏移变量位置
    } else {
        if (g_tcpci_info == NULL || g_tcpci_info->bar_mem_addr == NULL) {
            printk(KA_KERN_ERR "[lqdcmi]bar_mem_addr is NULL\n");
            ka_task_mutex_unlock(&kernel_fault_mem_lock);
            return;
        }

        /* 解析头部 */
        g_fault_event_head.version = ka_mm_readl(g_tcpci_info->bar_mem_addr);
        g_fault_event_head.length = ka_mm_readl(g_tcpci_info->bar_mem_addr + sizeof(unsigned int));
        g_fault_event_head.nodeSize = ka_mm_readl(g_tcpci_info->bar_mem_addr + sizeof(unsigned int) * 2); // 2表示偏移变量位置
        g_fault_event_head.nodeNum = ka_mm_readl(g_tcpci_info->bar_mem_addr + sizeof(unsigned int) * 3); // 3表示偏移变量位置
        g_fault_event_head.nodeHead = ka_mm_readl(g_tcpci_info->bar_mem_addr + sizeof(unsigned int) * 4); // 4表示偏移变量位置
        g_fault_event_head.nodeTail = ka_mm_readl(g_tcpci_info->bar_mem_addr + sizeof(unsigned int) * 5); // 5表示偏移变量位置
    }

    ka_task_mutex_unlock(&kernel_fault_mem_lock);
}

STATIC int get_node_info(SramFaultEventData *node_info, unsigned int index)
{
    unsigned i;
    ka_task_mutex_lock(&kernel_fault_mem_lock);
    if (isHccsMode()) {
        if (g_hccs_virt_addr == NULL) {
            printk(KA_KERN_ERR "[lqdcmi]bar_mem_addr is NULL\n");
            ka_task_mutex_unlock(&kernel_fault_mem_lock);
            return -1;
        }

        // 按照4字节读
        node_info->head.msgId = ka_mm_readl(g_hccs_virt_addr + sizeof(SramDescCtlHeader) +
                                    sizeof(SramFaultEventData) * index);
        node_info->head.devId = ka_mm_readl(g_hccs_virt_addr + sizeof(SramDescCtlHeader) +
                                    sizeof(SramFaultEventData) * index + sizeof(unsigned int));

        /* 读 info 信息, 240 表示 data 总大小为 240 Byte */
        for (i = 0; i < 240; i++) {
            node_info->data[i] = readb(g_hccs_virt_addr + sizeof(SramDescCtlHeader) +
                                    sizeof(SramFaultEventData) * index + sizeof(SramFaultEventHead) + i);
        }
    } else {
        if (g_tcpci_info == NULL || g_tcpci_info->bar_mem_addr == NULL) {
            printk(KA_KERN_ERR "[lqdcmi]bar_mem_addr is NULL\n");
            ka_task_mutex_unlock(&kernel_fault_mem_lock);
            return -1;
        }

        // 按照4字节读
        node_info->head.msgId = ka_mm_readl(g_tcpci_info->bar_mem_addr + sizeof(SramDescCtlHeader) +
                                    sizeof(SramFaultEventData) * index);
        node_info->head.devId = ka_mm_readl(g_tcpci_info->bar_mem_addr + sizeof(SramDescCtlHeader) +
                                    sizeof(SramFaultEventData) * index + sizeof(unsigned int));

        /* 读 info 信息, 240 表示 data 总大小为 240 Byte */
        for (i = 0; i < 240; i++) {
            node_info->data[i] = readb(g_tcpci_info->bar_mem_addr + sizeof(SramDescCtlHeader) +
                                    sizeof(SramFaultEventData) * index + sizeof(SramFaultEventHead) + i);
        }
    }
    ka_task_mutex_unlock(&kernel_fault_mem_lock);

    return 0;
}

STATIC void HandleFault(LqDcmiEvent *event, FaultEventNodeTable *currFault,
                        ChipFaultInfo *currChipFault, PortFaultInfo *currPortFault,
                        unsigned int alarmType)
{
    currFault->alarmFlag = 1;
    currFault->chipId = event->switchChipid;

    if (alarmType == CHIP_ALARM) {
        currChipFault->alarmFlag = 1;
        if (IsPortNodeInfoZero(currChipFault->chipNodeInfo)) {
            currChipFault->chipNodeInfo = *event;
        }
    } else if (alarmType == PORT_ALARM && currPortFault != NULL) {
        currChipFault->alarmFlag = 1;
        currPortFault->alarmFlag = 1;
        if (IsPortNodeInfoZero(currPortFault->portNodeInfo)) {
            currPortFault->portNodeInfo = *event;
        }
    }
}

STATIC void HandleRecovery(LqDcmiEvent *event, FaultEventNodeTable *currFault,
                           ChipFaultInfo *currChipFault, PortFaultInfo *currPortFault,
                           unsigned int alarmType)
{
    unsigned int ret = 0;
    int flag = 0;
    int i = 0;

    if (alarmType == CHIP_ALARM) {
        currChipFault->alarmFlag = 0;
        if (!IsPortNodeInfoZero(currChipFault->chipNodeInfo)) {
            ret = memset_s(&currChipFault->chipNodeInfo, sizeof(currChipFault->chipNodeInfo),
                0, sizeof(currChipFault->chipNodeInfo));
            if (ret != 0) {
                printk(KA_KERN_ERR "[lqdcmi]HandleRecovery memset_s fail\n");
                return;
            }
        }
    } else if (alarmType == PORT_ALARM && currPortFault != NULL) {
        currPortFault->alarmFlag = 0;
        if (!IsPortNodeInfoZero(currPortFault->portNodeInfo)) {
            ret = memset_s(&currPortFault->portNodeInfo, sizeof(currPortFault->portNodeInfo),
                0, sizeof(currPortFault->portNodeInfo));
            if (ret != 0) {
                printk(KA_KERN_ERR "[lqdcmi]HandleRecovery memset_s fail\n");
                return;
            }
        }

        for (i = 0; i < NUM_PORTS; ++i) {
            flag |= currChipFault->portFaultInfo[i].alarmFlag;
        }

        if (flag == 0) {
            currChipFault->alarmFlag = 0;
        }
    }
}

STATIC int write_head_in_file(void)
{
    unsigned int offset;
    ssize_t bytes_written;

    g_hccs_file = filp_open(HCCS_FILE_PATH, O_RDWR | O_CREAT, 0644); // 0644表示文件权限
    if (IS_ERR(g_hccs_file)) {
        printk(KA_KERN_ERR "[lqdcmi]Failed to open g_hccs_file\n");
        return -1;
    }

    // 计算偏移量
    offset = HCCS_HEAD_OFFSET * sizeof(FaultEventNodeTable);

    // 移动文件指针到指定偏移量
    vfs_llseek(g_hccs_file, offset, SEEK_SET);

    // 写入数据
    bytes_written = kernel_write(g_hccs_file, &g_hccs_head, sizeof(unsigned int), &g_hccs_file->f_pos);
    if (bytes_written != sizeof(unsigned int)) {
        printk(KA_KERN_ERR "Failed to write element to g_hccs_file\n");
        filp_close(g_hccs_file, NULL);
        g_hccs_file = NULL;
        return -1;
    }
    filp_close(g_hccs_file, NULL);
    g_hccs_file = NULL;
    return 0;
}
STATIC int write_start_time_in_file(unsigned long startTimeMs)
{
    unsigned int offset;
    ssize_t bytes_written;
    // 计算偏移量
    offset = HCCS_START_TIME_OFFSET * sizeof(FaultEventNodeTable);

    // 移动文件指针到指定偏移量
    vfs_llseek(g_hccs_file, offset, SEEK_SET);

    // 写入数据
    bytes_written = kernel_write(g_hccs_file, &startTimeMs,
                                 sizeof(unsigned long), &g_hccs_file->f_pos);
    if (bytes_written != sizeof(unsigned long)) {
        printk(KA_KERN_ERR "Failed to write element to g_hccs_file\n");
        return -1;
    }
    return 0;
}


STATIC void write_overflow_flag_in_file(unsigned int overflowflag)
{
    unsigned int offset;
    ssize_t bytes_written;
    // 计算偏移量
    offset = HCCS_OVERFLOW_FLAG_OFFSET * sizeof(FaultEventNodeTable);

    // 移动文件指针到指定偏移量
    if (vfs_llseek(g_hccs_file, offset, SEEK_SET) != 0) {
        printk(KA_KERN_ERR "[lqdcmi]Failed to seek g_hccs_file\n");
        return;
    }

    // 写入数据
    bytes_written = kernel_write(g_hccs_file, &overflowflag,
                                 sizeof(unsigned int), &g_hccs_file->f_pos);
    if (bytes_written != sizeof(unsigned int)) {
        printk(KA_KERN_ERR "[lqdcmi]Failed to write element to g_hccs_file\n");
    }
}

STATIC int write_fault_info_in_file(unsigned int index)
{
    unsigned int offset;
    ssize_t bytes_written;

    g_hccs_file = filp_open(HCCS_FILE_PATH, O_RDWR | O_CREAT, 0644); // 0644表示文件权限
    if (IS_ERR(g_hccs_file)) {
        printk(KA_KERN_ERR "[lqdcmi]Failed to open g_hccs_file\n");
        return -1;
    }

    // 计算偏移量
    offset = index * sizeof(FaultEventNodeTable);

    // 移动文件指针到指定偏移量
    vfs_llseek(g_hccs_file, offset, SEEK_SET);

    // 写入数据
    bytes_written = kernel_write(g_hccs_file, &g_kernel_event_table[index],
                                 sizeof(g_kernel_event_table[index]), &g_hccs_file->f_pos);
    if (bytes_written != sizeof(g_kernel_event_table[index])) {
        printk(KA_KERN_ERR "Failed to write element %d to g_hccs_file\n", index);
        filp_close(g_hccs_file, NULL);
        g_hccs_file = NULL;
        return -1;
    }
    filp_close(g_hccs_file, NULL);
    g_hccs_file = NULL;
    return 0;
}

STATIC int read_head_from_file(unsigned int* head)
{
    unsigned int offset;
    ssize_t bytes_read;
    unsigned int index = HCCS_HEAD_OFFSET;

    offset = index * sizeof(FaultEventNodeTable);

        // 移动文件指针到指定偏移量
    vfs_llseek(g_hccs_file, offset, SEEK_SET);

        // 读取数据
    bytes_read = kernel_read(g_hccs_file, head, sizeof(unsigned int), &g_hccs_file->f_pos);
    if (bytes_read != sizeof(unsigned int)) {
        printk(KA_KERN_ERR "Failed to read element %d from g_hccs_file\n", index);
        return -1;
    }

    return 0;
}

STATIC int read_start_time_from_file(unsigned long* starttime)
{
    unsigned int offset;
    ssize_t bytes_read;

    unsigned int index = HCCS_START_TIME_OFFSET;

    offset = index * sizeof(FaultEventNodeTable);

        // 移动文件指针到指定偏移量
    vfs_llseek(g_hccs_file, offset, SEEK_SET);

        // 读取数据
    bytes_read = kernel_read(g_hccs_file, starttime, sizeof(unsigned long), &g_hccs_file->f_pos);
    if (bytes_read != sizeof(unsigned long)) {
        printk(KA_KERN_ERR "Failed to read element %d from g_hccs_file\n", index);
        return -1;
    }

    return 0;
}

STATIC void read_overflow_flag_from_file(unsigned int* overflowflag)
{
    unsigned int offset;
    ssize_t bytes_read;
    unsigned int index = HCCS_OVERFLOW_FLAG_OFFSET;
    offset = index * sizeof(FaultEventNodeTable);

    // 移动文件指针到指定偏移量
    if (vfs_llseek(g_hccs_file, offset, SEEK_SET) != 0) {
        printk(KA_KERN_ERR "[lqdcmi]Failed to seek to element %d in g_hccs_file\n", index);
        return;
    }

    // 读取数据
    bytes_read = kernel_read(g_hccs_file, overflowflag, sizeof(unsigned int), &g_hccs_file->f_pos);
    if (bytes_read != sizeof(unsigned int)) {
        printk(KA_KERN_ERR "[lqdcmi]Failed to read element %d from g_hccs_file\n", index);
    }
}

STATIC void clear_fault_info_file_part(void)
{
    unsigned int offset;
    ssize_t bytes_written;
    char *buffer;
    int ret = 0;
    unsigned int index = 0;

    printk(KA_KERN_INFO "[lqdcmi]clear_fault_info_file\n");
    // 分配缓冲区
    buffer = kmalloc(sizeof(FaultEventNodeTable), KA_GFP_KERNEL);
    if (!buffer) {
        printk(KA_KERN_ERR "[lqdcmi]Failed to allocate memory\n");
        return;
    }

    // 将缓冲区清零
    ret = memset_s(buffer, sizeof(FaultEventNodeTable), 0, sizeof(FaultEventNodeTable));
    if (ret != 0) {
        printk(KA_KERN_ERR "[lqdcmi]clear_fault_info memset_s fail\n");
        ka_mm_kfree(buffer);
        return;
    }

    // 遍历文件，将每个元素清零
    for (index = 0; index < CAPACITY; ++index) {
        // 计算偏移量
        offset = index * sizeof(FaultEventNodeTable);

        // 移动文件指针到指定偏移量
        if (vfs_llseek(g_hccs_file, offset, SEEK_SET) != 0) {
            printk(KA_KERN_ERR "[lqdcmi]Failed to seek to element %d in g_hccs_file\n", index);
            continue;
        }

        // 写入数据
        bytes_written = kernel_write(g_hccs_file, buffer, sizeof(FaultEventNodeTable), &g_hccs_file->f_pos);
        if (bytes_written != sizeof(FaultEventNodeTable)) {
            printk(KA_KERN_ERR "[lqdcmi]Failed to write element %d to g_hccs_file\n", index);
            // 跳过当前索引，继续下一个
            continue;
        }
    }

    // 释放缓冲区
    ka_mm_kfree(buffer);
}

STATIC int read_fault_info_from_file(void)
{
    unsigned int offset;
    ssize_t bytes_read;
    char *buffer;
    unsigned int index = 0;
    int ret = 0;

    printk(KA_KERN_INFO "[lqdcmi]read_fault_info_from_file\n");
    // 分配缓冲区
    buffer = kmalloc(sizeof(FaultEventNodeTable), KA_GFP_KERNEL);
    if (!buffer) {
        printk(KA_KERN_ERR "Failed to allocate memory\n");
        return -ENOMEM;
    }

    // 遍历文件，读取每个元素并填回 g_kernel_event_table
    for (index = 0; index < CAPACITY; ++index) {
        // 计算偏移量
        offset = index * sizeof(FaultEventNodeTable);

        // 移动文件指针到指定偏移量
        vfs_llseek(g_hccs_file, offset, SEEK_SET);

        // 读取数据
        bytes_read = kernel_read(g_hccs_file, buffer, sizeof(FaultEventNodeTable), &g_hccs_file->f_pos);
        if (bytes_read != sizeof(FaultEventNodeTable)) {
            printk(KA_KERN_ERR "Failed to read element %d from g_hccs_file\n", index);
            // 跳过当前索引，继续下一个
            continue;
        }

        // 将读取的数据写入 g_kernel_event_table[index]
        ret = memcpy_s(&g_kernel_event_table[index], sizeof(FaultEventNodeTable), buffer, sizeof(FaultEventNodeTable));
        if (ret != 0) {
            printk(KA_KERN_ERR "[lqdcmi]read_fault_info_from_file memset_s fail\n");
            continue;
        }
    }

    // 释放缓冲区
    ka_mm_kfree(buffer);

    return 0;
}

STATIC void handle_start_time_and_fault_info(unsigned int head_flag)
{
    unsigned long starttime = ka_mm_readl(g_hccs_virt_addr + sizeof(unsigned int) * MEM_OFFSET_HCCS_STARTTIME_FLAG);
    unsigned long pre_starttime = 0;
    int res = 0;

    read_start_time_from_file(&pre_starttime);
    if (starttime == pre_starttime) {
        res = read_fault_info_from_file();
        if (res != 0) {
            printk(KA_KERN_ERR "[lqdcmi]read_fault_info_from_file fail %d\n", res);
        }
        if (head_flag) {
            res = read_head_from_file(&g_hccs_head);
            if (res != 0) {
                printk(KA_KERN_ERR "[lqdcmi]read_head_from_file fail\n");
            }
        }
    } else {
        clear_fault_info_file_part();
    }
    res = write_start_time_in_file(starttime);
    if (res != 0) {
        printk(KA_KERN_ERR "[lqdcmi]write_start_time_in_file fail\n");
    }
}

STATIC void copy_fault_from_file(void)
{
    unsigned int head = 0;
    unsigned int tail = 0;
    unsigned int num = 0;
    unsigned int diff = 0;
    unsigned int steps = 0;
    unsigned int head_flag = 1;
    unsigned int overflowflag = 0;
    unsigned int pre_overflowflag = 0;

    if (g_hccs_virt_addr == NULL) {
        printk(KA_KERN_ERR "[lqdcmi]hccs_virt_addr is NULL\n");
        return;
    }

    g_hccs_file = filp_open(HCCS_FILE_PATH, O_RDWR | O_CREAT, 0644); // 0644表示文件权限
    if (IS_ERR(g_hccs_file)) {
        printk(KA_KERN_ERR "[lqdcmi]Failed to open g_hccs_file\n");
        return;
    }

    overflowflag = ka_mm_readl(g_hccs_virt_addr + sizeof(unsigned int) * MEM_OFFSET_HCCS_OVERFLOW_FLAG);
    printk(KA_KERN_INFO "[lqdcmi]copy_fault_from_file lqoverflowflag: %u\n", overflowflag);
    read_overflow_flag_from_file(&pre_overflowflag);

    if (overflowflag != pre_overflowflag) {
        clear_fault_info_file_part();
        write_overflow_flag_in_file(overflowflag);
        update_mem_by_map();

        num = g_fault_event_head.nodeNum;
        head = g_fault_event_head.nodeHead;
        tail = g_fault_event_head.nodeTail;
        diff = (tail - head + num) % num;

        if (diff < RETAINED_FAULT_NUMS) {
            steps = RETAINED_FAULT_NUMS - diff;
            head = (head - steps + num) % num;
            g_hccs_head = head;
        }
        head_flag = 0;
    }
    handle_start_time_and_fault_info(head_flag);
    filp_close(g_hccs_file, NULL);
    g_hccs_file = NULL;
}

STATIC int update_thread_function_for_hccs(void *data)
{
    unsigned int sramflag;
    printk(KA_KERN_INFO "[lqdcmi]update_thread_function_for_hccs\n");
    while (!ka_task_kthread_should_stop()) {
        int res = 0;
        if (!g_main_board_id_initialized) {
            res = get_main_board_id();
            if (res == 0) {
                if (!isHccsMode()) {
                    printk(KA_KERN_ERR "[lqdcmi]Not HCCS mode\n");
                    g_kernel_update_thread = NULL;
                    return -1;
                }
                g_main_board_id_initialized = true;

                // 在initTable成功后执行额外操作，仅执行一次
                g_hccs_virt_addr = ka_mm_ioremap(HCCS_BASE_ADDR, TOTAL_LEN);
                if (!g_hccs_virt_addr) {
                    printk(KA_KERN_ERR "[lqdcmi]ka_mm_ioremap failed\n");
                    g_kernel_update_thread = NULL;
                    return -1;
                }
                sramflag = ka_mm_readl(g_hccs_virt_addr + sizeof(unsigned int)); // 6表示偏移变量位置
                if (sramflag != 0x00000100) {
                    printk(KA_KERN_ERR "[lqdcmi]sramflag is not 0x00000100\n");
                    ka_mm_iounmap(g_hccs_virt_addr);
                    g_hccs_virt_addr = NULL;
                    g_kernel_update_thread = NULL;
                    return -1;
                }
                copy_fault_from_file();
            }
        } else {
            res = initTable();
            if (res != 0) {
                printk(KA_KERN_ERR "[lqdcmi]update_thread_function_for_hccs error\n");
            }
        }

        // 休眠指定的时间
        ka_task_set_current_state(KA_TASK_INTERRUPTIBLE);
        schedule_timeout(usecs_to_jiffies(THREAD_SLEEP));
    }

    // 清理线程
    __ka_task_set_current_state(TASK_RUNNING);
    return 0;
}

STATIC int update_thread_function_for_pcie(void *data)
{
    printk(KA_KERN_INFO "[lqdcmi]update_thread_function_for_pcie\n");
    while (!ka_task_kthread_should_stop()) {
        int res = initTable();
        if (res != 0) {
            printk(KA_KERN_ERR "[lqdcmi]update_thread_function_for_pcie error\n");
        }

        // 休眠指定的时间
        ka_task_set_current_state(KA_TASK_INTERRUPTIBLE);
        schedule_timeout(usecs_to_jiffies(THREAD_SLEEP));
    }

    // 清理线程
    __ka_task_set_current_state(TASK_RUNNING);
    return 0;
}

STATIC int initAndStartThreadForPcie(void)
{
    printk(KA_KERN_INFO "[lqdcmi]initAndStartThreadForPcie\n");
    // 创建内核态线程
    g_kernel_update_thread = ka_task_kthread_run(update_thread_function_for_pcie, NULL, "lqdcmi_update_thread");
    if (IS_ERR(g_kernel_update_thread)) {
        printk(KA_KERN_ERR "[lqdcmi]Failed to create update thread\n");
        return PTR_ERR(g_kernel_update_thread);
    }

    return 0;
}

STATIC int initAndStartThreadForHccs(void)
{
    printk(KA_KERN_INFO "[lqdcmi]initAndStartThreadForHccs\n");
    // 创建内核态线程
    g_kernel_update_thread = ka_task_kthread_run(update_thread_function_for_hccs, NULL, "lqdcmi_update_thread");
    if (IS_ERR(g_kernel_update_thread)) {
        printk(KA_KERN_ERR "[lqdcmi]Failed to create update thread\n");
        return PTR_ERR(g_kernel_update_thread);
    }

    return 0;
}

STATIC void stopAndCleanupThread(void)
{
    printk(KA_KERN_INFO "[lqdcmi]stopAndCleanupThread\n");
    if (g_kernel_update_thread) {
        ka_task_kthread_stop(g_kernel_update_thread);
        g_kernel_update_thread = NULL;
    }
}

STATIC void EventHandler(LqDcmiEvent *event)
{
    unsigned int index;
    unsigned int assertion;
    unsigned int chipId;
    unsigned int portId;
    unsigned int alarmType;
    FaultEventNodeTable *currFault;
    ChipFaultInfo *currChipFault;
    PortFaultInfo *currPortFault;

    ka_task_mutex_lock(&g_kernel_table_mutex);
    if (find_index_by_sub_type(mapping, sizeof(mapping)/sizeof(mapping[0]), event->subType) == INDEX_NOT_FOUND) {
        printk(KA_KERN_ERR "[lqdcmi]EventHandler index is not found\n");
        ka_task_mutex_unlock(&g_kernel_table_mutex);
        return;
    }

    index = find_index_by_sub_type(mapping, sizeof(mapping)/sizeof(mapping[0]), event->subType);
    assertion = event->assertion;
    chipId = event->switchChipid;
    portId = event->switchPortid;

    alarmType = GetAlarmType(event);

    g_kernel_event_table[index].subType = event->subType;
    currFault = &g_kernel_event_table[index];

    if (chipId >= NUM_CHIP || (portId >= NUM_PORTS && portId != INVALID_PORTID)) {
        printk(KA_KERN_ERR "[lqdcmi]chipId or portId is invalid");
        ka_task_mutex_unlock(&g_kernel_table_mutex);
        return;
    }
    currChipFault = &currFault->chipFaultInfo[chipId];
    currPortFault = (portId == INVALID_PORTID) ? NULL : &currChipFault->portFaultInfo[portId];
    if (assertion == FAULT) {
        HandleFault(event, currFault, currChipFault, currPortFault, alarmType);
    } else if (assertion == RECOVERY) {
        HandleRecovery(event, currFault, currChipFault, currPortFault, alarmType);
    }

    ka_task_mutex_unlock(&g_kernel_table_mutex);
    if (isHccsMode()) {
        int res = write_fault_info_in_file(index);
        if (res != 0) {
            printk(KA_KERN_ERR "[lqdcmi]write_fault_info_in_file fail\n");
            return;
        }
    } else {
        FaultEventNodeTable* g_fault_info_mem = (FaultEventNodeTable*)(g_exist_fault_mem.pmap_addr);
        FaultEventNodeTable *userFault = &g_fault_info_mem[index];
        ChipFaultInfo *userChipFault = &userFault->chipFaultInfo[chipId];
        PortFaultInfo *userPortFault = (portId == INVALID_PORTID) ? NULL : &userChipFault->portFaultInfo[portId];

        g_fault_info_mem[index].subType = event->subType;
        if (assertion == FAULT) {
            HandleFault(event, userFault, userChipFault, userPortFault, alarmType);
        } else if (assertion == RECOVERY) {
            HandleRecovery(event, userFault, userChipFault, userPortFault, alarmType);
        }
    }
}

int initTable(void)
{
    int res;
    unsigned int head;
    unsigned int tail;
    LqDcmiEvent *event;
    SramFaultEventData sd = {0};

    update_mem_by_map();

    if (isHccsMode()) {
        head = g_hccs_head;
    } else {
        head = g_fault_event_head.nodeHead;
    }
    tail = g_fault_event_head.nodeTail;
    if (head == tail) {
        return 0;
    }

    while (head != tail) {
        unsigned int ret = memset_s(&sd, sizeof(SramFaultEventData), 0, sizeof(SramFaultEventData));
        if (ret != 0) {
            printk(KA_KERN_ERR "[lqdcmi]initTable memset_s fail\n");
            return -1;
        }

        if (get_node_info(&sd, head % g_fault_event_head.nodeNum) != 0) {
            printk(KA_KERN_ERR "[lqdcmi]get fault failed at head=%u, tail=%u\n", head, tail);
            return -1;
        }

        event = (LqDcmiEvent *) (sd.data);
        EventHandler(event);
        (void)write_shared_memory(&sd);
        head = (head + 1) % g_fault_event_head.nodeNum;

        if (isHccsMode()) {
            g_hccs_head = head;
            res = write_head_in_file();
            if (res != 0) {
                printk(KA_KERN_ERR "[lqdcmi]write_head_in_file fail\n");
            }
        } else {
            ka_mm_writel(head, g_tcpci_info->bar_mem_addr + sizeof(unsigned int) * 4); // 4表示更新头节点位置地址偏移
        }
    }
    return 0;
}

STATIC void initialize_locks(void)
{
    ka_task_mutex_init(&g_kernel_table_mutex);
    ka_task_mutex_init(&shared_mem_mutex);
    ka_task_mutex_init(&kernel_fault_mem_lock);
}

STATIC void destroy_locks(void)
{
    mutex_destroy(&shared_mem_mutex);
    mutex_destroy(&kernel_fault_mem_lock);
    mutex_destroy(&g_kernel_table_mutex);
}

STATIC int prepare_for_read_mem(void)
{
    int res;
    initialize_locks();
    copy_fault_from_mem();

    res = initTable();
    if (res != 0) {
        printk(KERN_ERR "[lqdcmi]initTable %s fail\n", PCIE_DEV_NAME);
        pci_cleanup();
        destroy_locks();
        return -1;
    }

    res = initAndStartThreadForPcie();
    if (res != 0) {
        printk(KERN_ERR "[lqdcmi]initAndStartThreadForPcie %s fail\n", PCIE_DEV_NAME);
        pci_cleanup();
        destroy_locks();
        return -1;
    }

    return 0;
}

STATIC void cleanup_resources_common(void)
{
    struct shared_memory *shm;

    device_destroy(g_common_class, g_pci_devid);
    ka_driver_class_destroy(g_common_class);

    unregister_chrdev_region(g_pci_devid, 1);

    g_common_class = NULL;

    ka_task_mutex_lock(&shared_mem_mutex);

    ka_list_for_each_entry(shm, &shared_mem_list, list)
    {
        ka_list_del(&shm->list);
        if (shm->mem) {
            ka_mm_kfree(shm->mem);
        }
        ka_mm_kfree(shm);
    }
    mutex_unlock(&shared_mem_mutex);
    destroy_locks();
}

STATIC int tc_comm_cdev_init(void)
{
    dev_t devid;
    struct device *pdev = NULL;
    int res;

    /*
     * 字符设备初始化流程
     */
    res = alloc_chrdev_region(&g_pci_devid, 0, 1, PCIE_DEV_NAME);
    if (res < 0) {
        printk(KA_KERN_ERR "[lqdcmi]register chrdev %s err \n", PCIE_DEV_NAME);
        return -1;
    }
    devid = g_pci_devid;

    // 填充设备结构体, 注册 pci设备
    // 建立 file_operation
    cdev_init(&tc_pci_cdev, &g_pcidev_fops);

    res = cdev_add(&tc_pci_cdev, devid, 1);
    if (res != 0) {
        printk(KA_KERN_ERR "[lqdcmi]cdev_add 0x%X err \n", devid);
        unregister_chrdev_region(devid, 1);
        return -1;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
    g_common_class = class_create(PCIE_DEV_NAME);
#else
    g_common_class = class_create(KA_THIS_MODULE, PCIE_DEV_NAME);
#endif
    if (g_common_class == NULL) {
        printk(KA_KERN_ERR "[lqdcmi]class create device %s faild\n", PCIE_DEV_NAME);
        ka_fs_cdev_del(&g_pcidev->cdev);
        return -1;
    }

    /* 创建设备 */
    pdev = device_create(g_common_class, NULL, devid, NULL, PCIE_DEV_NAME);
    if (pdev == NULL) {
        printk(KA_KERN_ERR "[lqdcmi]device create %s fail! \n", PCIE_DEV_NAME);
        cdev_del(&g_pcidev->cdev);
        class_destroy(g_common_class);
        g_common_class = NULL;
        return -1;
    }

    printk(KA_KERN_INFO "[lqdcmi]device create %s ok \n", PCIE_DEV_NAME);
    return 0;
}

STATIC void tc_pci_exit(void)
{
    struct pci_dev_info *tc_pci_dev;
    struct pci_dev *pdev = pci_get_device(PCIE_TC_VENDOR_ID, g_device_id, NULL);
    if (pdev == NULL) {
        printk(KA_KERN_ERR "[lqdcmi]PCI device not found\n");
        return;
    }

    tc_pci_dev = ka_pci_get_drvdata(pdev);

    ka_task_mutex_lock(&kernel_fault_mem_lock);

    if (tc_pci_dev->bar_mem_addr != NULL) {
        ka_mm_iounmap(tc_pci_dev->bar_mem_addr);
        tc_pci_dev->bar_mem_addr = NULL;
        tc_pci_dev->bar_mem_len = 0;
        ka_pci_release_selected_regions(pdev, (1 << PCI_BAR_ID));
        printk(KA_KERN_ERR "[lqdcmi]bar[0] mem free OK\n");
    }

    ka_task_mutex_unlock(&kernel_fault_mem_lock);

    pci_unregister_driver(&g_pci_driver);
}

STATIC int __init pcidev_init(void)
{
    int res;

    printk(KA_KERN_ERR "[lqdcmi]pcidev version 2024.06\n");
    g_kernel_event_table = (FaultEventNodeTable*)kmalloc(sizeof(FaultEventNodeTable) * CAPACITY, KA_GFP_KERNEL);
    if (g_kernel_event_table == NULL) {
        printk(KA_KERN_ERR "[lqdcmi]g_kernel_event_table kmalloc fail\n");
        return -1;
    }

    res = pci_init();
    if (res != 0) {
        // 仅当产品为天工，或者其他A3产品出现CPU和1260之间PCI断链的情况才会得到res！=0的结果
        printk(KA_KERN_ERR "[lqdcmi]pci_init %s fail\n", PCIE_DEV_NAME);
        res = tc_comm_cdev_init();
        if (res != 0) {
            printk(KA_KERN_ERR "[lqdcmi]tc_comm_cdev_init %s fail\n", PCIE_DEV_NAME);
            goto err;
        }
        initialize_locks();
        res = initAndStartThreadForHccs();
        if (res != 0) {
            printk(KA_KERN_ERR "[lqdcmi]initAndStartThreadForHccs %s fail\n", PCIE_DEV_NAME);
            cleanup_resources_common();
            goto err;
        }
    } else {
        res = tc_comm_cdev_init();
        if (res != 0) {
            printk(KA_KERN_ERR "[lqdcmi]tc_comm_cdev_init %s fail\n", PCIE_DEV_NAME);
            pci_unregister_driver(&g_pci_driver);
            goto err;
        }

        res = tc_pci_map();
        if (res != 0) {
            printk(KA_KERN_ERR "[lqdcmi]tc_pci_map %s fail\n", PCIE_DEV_NAME);
            pci_cleanup();
            goto err;
        }

        res = prepare_for_read_mem();
        if (res != 0) {
            printk(KA_KERN_ERR "[lqdcmi]preparae_for_read_mem %s fail\n", PCIE_DEV_NAME);
            goto err;
        }
    }

    return res;
err:
    kfree(g_kernel_event_table);
    g_kernel_event_table = NULL;
    return -1;
}

STATIC void __exit pcidev_exit(void)
{
    printk(KA_KERN_INFO "[lqdcmi]pcidev byte !\n");
    // 删除定时器
    if (isHccsMode()) {
        stopAndCleanupThread();
        if (g_hccs_file != NULL) {
            filp_close(g_hccs_file, NULL);
            g_hccs_file = NULL;
        }
        ka_task_mutex_lock(&kernel_fault_mem_lock);
        if (g_hccs_virt_addr != NULL) {
            ka_mm_iounmap(g_hccs_virt_addr);
            g_hccs_virt_addr = NULL;
        }
        ka_task_mutex_unlock(&kernel_fault_mem_lock);
        cleanup_resources_common();
    } else {
        // 注销 pci设备
        dev_t devid;
        devid = g_pci_devid;
        stopAndCleanupThread();
        tc_pci_exit();
        cleanup_resources_common();

        if (g_exist_fault_mem.pmap_addr != NULL) {
            ka_mm_iounmap((void *)(g_exist_fault_mem.pmap_addr));
            g_exist_fault_mem.pmap_addr = NULL;
        }


        if (g_bar_flag_mem.pmap_addr != NULL) {
            ka_mm_iounmap((void *)(g_bar_flag_mem.pmap_addr));
            g_bar_flag_mem.pmap_addr = NULL;
        }
    }
    printk(KA_KERN_INFO "[lqdcmi]pcidev exit !\n");
    kfree(g_kernel_event_table);
    g_kernel_event_table = NULL;
}

ka_module_init(pcidev_init);
ka_module_exit(pcidev_exit);
KA_MODULE_LICENSE("GPL");