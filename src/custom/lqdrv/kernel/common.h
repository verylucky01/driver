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

#ifndef COMMON_H

#define COMMON_H

#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/version.h>
#include <linux/spinlock.h>
#include <linux/timer.h>        // 定义 struct timer_list
#include <linux/workqueue.h>    // 定义 struct work_struct 和 struct delayed_work
#include <linux/hrtimer.h>      // 定义 struct hrtimer
#include <linux/ktime.h>        // 定义 ktime_t
#include <linux/cdev.h>         // 定义 struct cdev
#include <linux/semaphore.h>    // 定义 struct semaphore
#include <linux/interrupt.h>    // 定义 struct tasklet_struct
#include <linux/rbtree.h>       // 定义 struct rb_root
extern struct mutex g_kernel_table_mutex;
#define DEVDRV_MAX_TS_NUM                   (1)
#define AICPU_MAX_NUM 16
#define DEVMNG_SHM_INFO_HEAD_LEN 32
#define DEVMNG_SHM_INFO_ERROR_CODE_LEN 32
#define DEVMNG_SHM_INFO_EVENT_CODE_LEN 128

#define DEVMNG_SHM_INFO_DIE_ID_NUM 5
#define VMNG_VDEV_MAX_PER_PDEV 17
#define DEVDRV_MAX_COMPUTING_POWER_TYPE 1
#define DEVMNG_SHM_INFO_RANDOM_SIZE 24

struct shm_event_code {
    unsigned int event_code;
    unsigned char fid;
};

typedef union shm_info_head {
    struct {
        unsigned int magic; /* 标识功能区是否有效，有效时值为0x5a5a5a5a */
        unsigned int offset_soc;
        unsigned int offset_board;
        unsigned int offset_status;
        unsigned long long version;
    } head_info;
    char s8_union[DEVMNG_SHM_INFO_HEAD_LEN];
} U_SHM_INFO_HEAD;

typedef struct shm_info_soc {
    unsigned short die_id[DEVMNG_SHM_INFO_DIE_ID_NUM];
    unsigned short chip_info;
    unsigned short aicore_count;
    unsigned short cpu_count;
} U_SHM_INFO_SOC;

typedef struct shm_info_board {
    unsigned short board_id;
    unsigned short pcb_ver;
    unsigned short board_type;
    unsigned short slot_id;
    unsigned short venderid;    /* 厂商id */
    unsigned short subvenderid; /* 厂商子id */
    unsigned short deviceid;    /* 设备id */
    unsigned short subdeviceid; /* 设备子id */
    unsigned short bus;         /* 总线号 */
    unsigned short device;      /* 设备物理号 */
    unsigned short fn;          /* 设备功能号 */
    unsigned short davinci_id;  /* device id */
} U_SHM_INFO_BOARD;

typedef struct shm_info_status {
    unsigned short os_status;
    unsigned short health_status;
    int error_cnt;
    unsigned int error_code[DEVMNG_SHM_INFO_ERROR_CODE_LEN];
    unsigned short dms_health_status[VMNG_VDEV_MAX_PER_PDEV];
    int event_cnt;
    struct shm_event_code event_code[DEVMNG_SHM_INFO_EVENT_CODE_LEN];
} U_SHM_INFO_STATUS;

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 5, 0)
#ifndef _STRUCT_TIMESPEC
#define _STRUCT_TIMESPEC
struct timespec {
    __kernel_old_time_t          tv_sec;         /* seconds */
    long                    tv_nsec;        /* nanoseconds */
};
#endif
#endif

struct devdrv_container_info {
    unsigned int tflop_mode;
    unsigned int total_tflop;
    unsigned int alloc_unit;
    unsigned int tflop_num;
    struct mutex lock; /* used for container process */
    void *alloc_table;
};

struct aicpu_dts_config {
    unsigned int flag;
    unsigned int fw_cpu_id_base;        /* firmware cpu start id */
    unsigned int fw_cpu_num;            /* firmware cpu num */
    unsigned int system_cnt_freq;       /* timer frequency */
    unsigned int ts_int_start_id;       /* interrupt start id, send int to ts when task finish runing or log */
    unsigned int ctrl_cpu_int_start_id; /* interrupt start id, send int to control cpu when page missing */
    unsigned int ipc_cpu_int_start_id;  /* ipc interrupt start id, send int to control cpu when page missing */
    unsigned int ipc_mbx_int_start_id;
};

struct firmware_info {
    /* 内存信息 */
    void *aicpu_fw_mem;
    unsigned int aicpu_fw_mem_size;
    void *alg_mem[AICPU_MAX_NUM];
    unsigned int alg_mem_size;

    /* 启动地址 */
    unsigned long long aicpu_boot_addr; /* 此地址是对齐后的虚拟地址 */
    unsigned long long ts_boot_addr;    /* 此地址是对齐后的物理地址 */
    unsigned long long ts_boot_addr_virt;

    unsigned long long ts_blackbox_base;
    unsigned long long ts_blackbox_size;

    /* TS 信息 */
    unsigned long long ts_start_log_base; /* TS log 物理地址 */
    unsigned long long ts_start_log_size; /* TS log 大小 */

    unsigned char enable_bbox;
};

struct devdrv_heart_beat {
    struct list_head queue;
    spinlock_t spinlock;
    struct timer_list timer;
    unsigned int sq;
    unsigned int cq;
    volatile unsigned int cmd_inc_counter; /* increment counter for sendind heart beat cmd */
    struct work_struct work;
    const void *exception_info;
    volatile unsigned int stop;     /* use in host manager heart beat to device,
                        * avoid access null ptr to heart beat node
                        * when heart beat is stop */
    volatile unsigned int too_much; /* flag if too much heart beat waiting in queue to be sent */
    volatile unsigned int broken;
    volatile unsigned int working;

    /* new heart solution */
    struct hrtimer hrtimer;
    ktime_t kt;
    volatile unsigned int lost_count;
    volatile unsigned long long old_count;
    volatile unsigned long long total_lost_count;
    volatile unsigned long long total_15s_count;
    volatile unsigned long long total_10s_count;
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 5, 0)
    struct timespec old_time;
#endif
    volatile int debug_count;

    unsigned int tsid;
    unsigned int reserve;
};

struct devdrv_cce_ops {
    struct cdev devdrv_cdev;
    struct device *cce_dev;
};

struct devdrv_ts_mng {
    /* TS working Flag */
    volatile unsigned int *ts_work_status;
    volatile unsigned int ts_work_status_shadow;

    /* spinlock for read write ts status */
    spinlock_t ts_spinlock;
};

struct devdrv_hardware_inuse {
    unsigned int devid;
    unsigned int ai_core_num;
    unsigned int ai_core_error_bitmap;
    unsigned int ai_cpu_num;
    unsigned int ai_cpu_error_bitmap;
};

struct devdrv_info {
    unsigned char plat_type;
    unsigned char status;
    atomic_t occupy_ref;
    unsigned int env_type;
    unsigned int board_id;
    unsigned int slot_id;
    unsigned int dev_id; /* device id assigned by local device driver */
    unsigned int chip_id; /* chip id */
    unsigned int die_id; /* die id */
    unsigned int ts_mem_restrict_valid;

    /*
     * device id assigned by pcie driver
     * not used in device side
     */
    unsigned int pci_dev_id;

    /*
     * indicates whether the device supports
     * the corresponding feature.
     */
    unsigned int capability;

    struct semaphore sem;

    /* devdrv_drv_register is called successfully */
    struct mutex lock;
    volatile unsigned int driver_flag;
    struct device *dev;

    unsigned int ctrl_cpu_ip;
    unsigned int ctrl_cpu_id;
    unsigned int ctrl_cpu_core_num;
    unsigned int ctrl_cpu_occupy_bitmap;
    unsigned int ctrl_cpu_endian_little;

    unsigned int ai_cpu_core_num;
    unsigned int ai_core_num;
    unsigned int ai_cpu_core_id;
    unsigned int ai_core_id;
    unsigned int aicpu_occupy_bitmap;
    unsigned int ai_subsys_ip_broken_map;
    unsigned int hardware_version;
    unsigned long long aicore_bitmap;
    cpumask_t ccpumask;

    struct devdrv_hardware_inuse inuse;

    struct work_struct work;
    /* devdrv_manager_register is called successfully */
    volatile int dev_ready;
    spinlock_t spinlock;

    unsigned int ts_num;
    struct devdrv_ts_mng ts_mng[DEVDRV_MAX_TS_NUM];
    struct devdrv_cce_ops cce_ops;

    /* independent ops and information for each device */
    struct tsdrv_drv_ops *drv_ops;
    struct devdrv_platform_data *pdata;

    /*
     * Host Subscript choose 0
     * Device Subscript choose 0 or 1
     */
    struct devdrv_heart_beat heart_beat[DEVDRV_MAX_TS_NUM];

    /*
     * HOST manager use this workqueue to
     * send heart beat msg to device.
     */
    struct workqueue_struct *heart_beat_wq;

    /* for nfe irq, devdrv_device_manager start a tasklet */
    struct tasklet_struct nfe_task;

    /* aicpu & ts upload related parameters */
    struct firmware_info fw_info;
    struct aicpu_dts_config dts_cfg;
    dma_addr_t aicpu_dma_handle;
    unsigned int firmware_hardware_version;

    unsigned int fw_wq_retry;
    struct delayed_work fw_load_wq;

    struct devdrv_container_info container;

    struct rb_root rb_root;
    struct semaphore no_trans_chan_wait_sema;
    unsigned int user_core_distribution;

    unsigned int dump_ddr_size;
    unsigned long long dump_ddr_dma_addr;
    unsigned long long aicore_freq;
    unsigned long long cpu_system_count;
    unsigned long long dev_nominal_osc_freq;
    unsigned long long monotonic_raw_time_ns;

    unsigned int fw_verify;
    unsigned int sec_head_size;
    unsigned char *fw_src_addr;
    unsigned int pos;
    unsigned int fw_len;
    unsigned int vector_core_num;
    unsigned long long vector_core_bitmap;
    unsigned long long vector_core_freq;
    unsigned int localtime_sync_state;

    unsigned long long reg_ddr_dma_addr;
    unsigned int reg_ddr_size;

    unsigned char run_mode; /*
                 * normal mode: online or offline without container
                 * container mode
                 */

    /* share memory info */
    void __iomem *shm_vaddr;
    U_SHM_INFO_HEAD __iomem *shm_head;
    U_SHM_INFO_SOC __iomem *shm_soc;
    U_SHM_INFO_BOARD __iomem *shm_board;
    U_SHM_INFO_STATUS __iomem *shm_status;

    unsigned long long computing_power[DEVDRV_MAX_COMPUTING_POWER_TYPE]; /* computing power level info */
    unsigned int dmp_started;
    unsigned int ffts_type;

    unsigned int ts_irq_init;  /* for destroy ts irq. */

    unsigned int chip_name;
    unsigned int chip_version;
    unsigned int chip_info;
    unsigned int mainboard_id;
    unsigned char multi_chip;
    unsigned char multi_die;
    unsigned short connect_type;
    unsigned short server_id;
    unsigned short scale_type;
    unsigned int super_pod_id;
    unsigned short addr_mode;

#ifdef CFG_FEATURE_PG
    struct devdrv_pg_info pg_info;
#endif
    #define TEMPLATE_NAME_LEN 32
    unsigned char template_name[TEMPLATE_NAME_LEN];

    unsigned char version;
    unsigned short hccs_hpcs_bitmap;
    char random_number[DEVMNG_SHM_INFO_RANDOM_SIZE];
};

#endif
