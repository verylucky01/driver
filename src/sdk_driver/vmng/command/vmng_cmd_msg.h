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

#ifndef _VMNG_CMD_MSG_H_
#define _VMNG_CMD_MSG_H_

struct vmng_vdev_res_ele {
    unsigned long bitmap;
};

struct vmng_numa_address {
    u64 start;
    u64 end;
};

struct vmng_soc_res_base {
    u32 memory;
    u32 memory_spec;
    u64 numa_bitmap;
};

struct vmng_vf_group_refresh {
    u32 vfg_mode; /* 0:strick 1:relax */
    u64 token;
    u64 token_max;
    u64 task_timeout;
};

#define VMNG_NUMA_MAX_NUM   25
struct vmng_vf_memory_info {
    u32 number;
    u64 size;
    struct vmng_vdev_res_ele numa_id;
    struct vmng_numa_address address[VMNG_NUMA_MAX_NUM];
};

struct vmng_vf_group_info {
    u32 vfg_id; /* 0~5 */
    u32 vfg_type;
    struct vmng_vf_group_refresh vfg_refresh;
};

struct vmng_stars_res_refresh {
    u32 aiv;
    u32 dsa;
    u32 rtsq;
    u32 cdqm;
    u32 topic_aicpu_slot;
    u32 host_ctrl_cpu;
    u32 device_aicpu;
    u32 host_aicpu;
    u32 jpegd;
    u32 jpege;
    u32 vpc;
    u32 vdec;
    u32 pngd;
    u32 venc;
};

struct vmng_stars_res_static {
    u32 aic;
    u32 c_core;
    u32 ffts;
    u32 sdma;
    u32 pcie_dma;
    u32 acsq;
    u32 event_id;
    u32 notify_id;
    u32 topic_ctrl_cpu_slot;
};

struct vmng_soc_res_info {
    struct vmng_soc_res_base base;
    struct vmng_stars_res_refresh stars_refresh;
    struct vmng_stars_res_static stars_static;
};

#define MAX_DIE_NUM_PER_DEV 2
struct vmng_mia_res_info_ex {
    u64 bitmap;
    u32 unit_per_bit;
    u32 start;
    u32 total_num;
    u32 freq;
};

#define VMNG_VF_TEMP_NAME_LEN 16
struct vmng_vf_res_info {
    char name[VMNG_VF_TEMP_NAME_LEN];
    u32 vm_full_spec_enable;
    u32 dev_id;
    u32 vfid;
    struct vmng_vf_memory_info memory;
    struct vmng_vf_group_info vfg;
    struct vmng_stars_res_refresh stars_refresh;
    struct vmng_stars_res_static stars_static;
};

struct vmng_soc_resource_enquire {
    struct vmng_vf_res_info each;
    struct vmng_soc_res_info remain;
    struct vmng_soc_res_info total;
    struct vmng_soc_res_info vfg;
};

struct vmng_soc_resource_refresh {
    u64 memory;
    struct vmng_stars_res_refresh stars_refresh;
};

struct vmng_mdev_iova_info {
    unsigned int dev_id;
    unsigned int vfid;
    unsigned int action;    // online or offline
    dma_addr_t iova_base;   // vf iova of pm
    size_t size;
};

struct vmng_vf_sync_remote_id {
    u32 udevid;         /* devid in host local */
    u32 remote_udevid;  /* devid in remote device os */
};

struct soc_mia_ub_info {
    u32 eid;
    u32 token;
    u32 jetty_id;
};

struct vmng_group_info {
    u32 dev_id;
    u32 vnpu_id;
    u32 group_id;
    u32 aic;

    // Used when setting up soc
    u32 vfg_id;
    u32 vf_id;
    struct vmng_mia_res_info_ex soc_aic[MAX_DIE_NUM_PER_DEV];
    struct vmng_mia_res_info_ex soc_aiv[MAX_DIE_NUM_PER_DEV];
    struct vmng_mia_res_info_ex soc_rtsq;
    struct vmng_mia_res_info_ex soc_notifyid;
};

struct vmng_ctrl_msg_sync {
    int dev_id;
    unsigned int aicore_num;
    u32 reserved[6];
};

struct vmng_ctrl_msg_info {
    u32 dev_id;
    u32 vfid;
    u32 vfg_type;
    u32 dtype;
    u32 core_num;
    u32 total_core_num;
    u32 reserved[4];
    union {
        struct vmng_vf_res_info vf_cfg;
        struct vmng_soc_resource_enquire enquire;
        struct vmng_soc_resource_refresh refresh;
        int sriov_status;
        struct vmng_mdev_iova_info iova_info;
        struct vmng_vf_sync_remote_id id_info;
        struct soc_mia_ub_info ub_info;
    };
};

struct mia_ctrl_info {
    u32 dev_id;
    u32 vnpu_id;
    union {
        struct vmng_group_info group_info;
    };
    u32 reserved[6];
};

struct vmng_ctrl_msg {
    int type;
    int error_code;
    union {
        struct vmng_ctrl_msg_sync sync_msg;
        struct vmng_ctrl_msg_info info_msg;
        struct mia_ctrl_info mia_msg;
    };
};


#endif