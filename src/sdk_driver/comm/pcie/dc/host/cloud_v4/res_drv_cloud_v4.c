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
#include <linux/dmi.h>
#include <linux/delay.h>

#include "res_drv.h"
#include "devdrv_util.h"
#include "devdrv_ctrl.h"
#include "res_drv_cloud_v4.h"
#include "pbl/pbl_uda.h"

#ifndef DRV_UT

#define DEVDRV_PCIE_LINK_DOWN_STATE           0xFFFFFFFF
#define DEVDRV_PCIE_LINK_INFO_OFFSET          0x50
#define DEVDRV_CLOUD_V4_LINK_INFO_SPEED_SHIFT 16
#define DEVDRV_CLOUD_V4_LINK_INFO_SPEED_MASK  0xF
#define DEVDRV_CLOUD_V4_LINK_INFO_WIDTH_SHIFT 20
#define DEVDRV_CLOUD_V4_LINK_INFO_WIDTH_MASK  0x3F
#define DEVDRV_CLOUD_V4_LINK_STATUS_NORMAL    0x10

#define DEVDRV_HCCS_CACHELINE_SIZE 128
#define DEVDRV_HCCS_CACHELINE_MASK (DEVDRV_HCCS_CACHELINE_SIZE - 1)

#define DEVDRV_WAIT_MODE_SWITCH_RETRY_COUNT 300
#define DEVDRV_WAIT_MODE_SWITCH_TIME 100 /* 100ms */

#define DEVDRV_CLOUD_V4_P2P_SUPPORT_MAX_DEVICE 8

#define DEVDRV_CLOUD_V4_2DIE_DEVID_STRIDE_2  2
#define DEVDRV_CLOUD_V4_2DIE_BOARD_ID        0x80
#define DEVDRV_CLOUD_V4_2DIE_2PCIE_MAINBOARD 0xE
#define DEVDRV_CLOUD_V4_HW_INFO_SRAM_OFFSET  0x5800
#define DEVDRV_CLOUD_V4_PCIE_BOARD_ID        0x1
#define DEVDRV_CLOUD_V4_JUDGE_BOARD_ID       0x7
#define DEVDRV_CLOUD_V4_BOARD_TYPE_BIT       4
#define DEVDRV_CLOUD_V4_INFER_PCIE_BOARD_ID  0x28
#define INVALID_HW_INFO_VERSION   0xFF
#define INVALID_HW_INFO_MAINBOARD_ID 0xFF
#define DEVDRV_INIT_HWINFO_RETRY_TIME 60   /* wait 60 times */
#define DEVDRV_INIT_HWINFO_WAIT_TIME  1000 /* wait 1000ms */

typedef union {
    struct {
        u8 master_slave_type : 1; // bit0    : 0: master-slave, 1: master-master
        u8 sub_machine_form : 4;  // bit1-4  : sub type of machine form
        u8 machine_form : 3;      // bit5-7  : EVB/SERVER/POD/CARD
    } bits;
    u8 id;
} devdrv_hw_mainboard_id_t;

/* bios set, host use */
typedef struct devdrv_hw_info {
    // struct version 3 for cloud v2
    u8 chip_id;
    u8 multi_chip;
    u8 multi_die;
    u8 mainboard_id;
    u16 addr_mode;
    u16 board_id;
    u8 version;
    u8 connect_type;
    u16 hccs_hpcs_bitmap;
    u16 server_id;
    u16 scale_type;
    u32 super_pod_id;
    u32 reserved;
    // struct version 4 for cloud v4
    u32 superpod_id;
    u16 superpod_size;
    u16 chassis_id;
    u8 share_mem_size;
    u8 super_pod_type;
    devdrv_hw_mainboard_id_t mainboard_id0;
    u8 mainboard_id1;
    u8 npu_slot_id;
    u8 npu_module_id;
    u8 superpod_intertype;
    u8 rsv1;
    u16 server_index;
    u16 npu_board_type;
    u8 npu_boot_type;
    u8 ub_remote_port[UB_REMOTE_PORT_NUM];
    u8 ub_remote_device[UB_REMOTE_DEVICE_NUM];
    u8 ub_manage_mode;
    u8 rsv2[UB_HW_INFO_RSV2_NUM];
} devdrv_hw_info_t;
STATIC devdrv_hw_info_t g_hw_info = {0};

enum devdrv_machine_form {
    DEVDRV_MACHINE_FORM_CLOUD_V4_POD = 0,
    DEVDRV_MACHINE_FORM_CLOUD_V4_AK_SERVER = 1,
    DEVDRV_MACHINE_FORM_CLOUD_V4_AX_SERVER = 2,
    DEVDRV_MACHINE_FORM_CLOUD_V4_PCIE_CARD = 3,
    /* 4-6: reserved */
    DEVDRV_MACHINE_FORM_CLOUD_V4_EVB = 7,
};

#define BOARD_CLOUD_V4_8P_MAX_DEV_NUM 16

#define PCI_BAR_RSV_MEM 0
#define PCI_BAR_IO 2
#define PCI_BAR_MEM 4

#define DEVDRV_IO_LOAD_SRAM_OFFSET 0x8C28000
#define DEVDRV_IO_LOAD_SRAM_SIZE 0x40000

#define DEVDRV_SOC_DOORBELL_OFFSET 0x8C08000

#define DEVDRV_IEP_DMA_OFFSET 0
#define DEVDRV_IEP_DMA_SIZE 0x8000
#define DEVDRV_IEP_DMA_CHAN_OFFSET 0x2000
#define DEVDRV_VF_DMA_OFFSET 0

/* mem base */
#define DEVDRV_PF_MDEV_VPC_SIZE 0x4020000

#define DEVDRV_RESERVE_MEM_MSG_OFFSET 0
#define DEVDRV_RESERVE_MEM_MSG_SIZE (1 * 1024 * 1024)

STATIC u64 g_reserver_mem_msg_offset = DEVDRV_RESERVE_MEM_MSG_OFFSET;
#define DEVDRV_HBOOT_L3D_SRAM_OFFSET 0x8C68000
#define DEVDRV_HBOOT_L3D_SRAM_SIZE 0x400000

#define DEVDRV_RESERVE_MEM_TOTAL_SIZE (24 * 1024 * 1024)

#define DEVDRV_RESERVE_MEM_TS_SQ_OFFSET 0
#define DEVDRV_RESERVE_MEM_TS_SQ_SIZE   0

#define DEVDRV_RESERVE_MEM_DEVMANAGER_OFFSET DEVDRV_RESERVE_MEM_TOTAL_SIZE
#define DEVDRV_RESERVE_MEM_DEVMANAGER_SIZE 0x100000
#define DEVDRV_RESERVE_MEM_TOPIC_SCHED_SIZE 0x100000

/* kernel log ring buff */
#define DEVDRV_RESERVE_BBOX_OFFSET    (g_reserver_mem_msg_offset + DEVDRV_RESERVE_MEM_TOTAL_SIZE + \
    DEVDRV_RESERVE_MEM_DEVMANAGER_SIZE + DEVDRV_RESERVE_MEM_TOPIC_SCHED_SIZE)
#define DEVDRV_RESERVE_BBOX_SIZE      0x2000000

#define DEVDRV_RESERVE_BBOX_HDR_ADDR  0x900000
#define DEVDRV_RESERVE_HDR_OFFSET (DEVDRV_RESERVE_BBOX_OFFSET + DEVDRV_RESERVE_BBOX_SIZE + DEVDRV_RESERVE_BBOX_HDR_ADDR)
#define DEVDRV_RESERVE_HDR_SIZE 0x80000

#define DEVDRV_RESERVE_IMU_LOG_SIZE 0x20000
/* RDR + AP + UBitem = 0x320000 */
#define DEVDRV_RESERVE_IMU_LOG_OFFSET (DEVDRV_RESERVE_BBOX_OFFSET + DEVDRV_RESERVE_BBOX_SIZE + 0x320000)

#define DEVDRV_IEP_SDI0_DB_OFFSET 0x1000
#define DEVDRV_DB_IOMAP_SIZE  0x1000

/* vf mem */
#define DEVDRV_VF_MDEV_RESERVE_MEM_SIZE 0x6000000
#define DEVDRV_VF_MDEV_VPC_OFFSET 0
#define DEVDRV_VF_MDEV_VPC_SIZE 0x4020000
#define DEVDRV_VF_MDEV_DVPP_OFFSET DEVDRV_PF_MDEV_VPC_SIZE
#define DEVDRV_VF_MDEV_DVPP_SIZE 0x1800000

#define DEVDRV_VF_MEM_MSG_OFFSET 0
#define DEVDRV_VF_MEM_MSG_SIZE   (1 * 1024 * 1024)

/* VF IO BASE */

#define DEVDRV_VF_IEP_DMA_OFFSET 0X2508000
#define DEVDRV_VF_IEP_DMA_SIZE   0X100

#define DEVDRV_VF_IEP_SDI0_OFFSET 0X2608000
#define DEVDRV_VF_IEP_SDI0_SIZE   0x10000

#define DEVDRV_VF_IO_TS_SRAM_OFFSET 0X2708000
#define DEVDRV_VF_IO_TS_SRAM_SIZE   (2 * 1024)

#define DEVDRV_VF_IO_SOC_DOORBELL_OFFSET 0X8000
#define DEVDRV_VF_IO_SOC_DOORBELL_SIZE   (4 * 1024)

#define DEVDRV_VF_IO_LOAD_SRAM_OFFSET 0Xa000
#define DEVDRV_VF_IO_LOAD_SRAM_SIZE   (2 * 1024)

/* ************ interrupt defined for normal host ************* */
#define DEVDRV_MSI_X_MAX_VECTORS 256
#define DEVDRV_MSI_X_MIN_VECTORS 128

/* device os load notify use irq vector 0, later 0 alse use to admin msg chan */
#define DEVDRV_LOAD_MSI_X_VECTOR_NUM 0

/* irq used to msg trans, a msg chan need two vector. one for tx finish, the other for rx msg.
   msg chan 0 is used to admin(chan 0) role */
#define DEVDRV_MSG_MSI_X_VECTOR_BASE 0
#define DEVDRV_MSG_MSI_X_VECTOR_NUM 48

/* irq used to dma, a dma chan need 22 vector. one for cq, the other for err.
  host surport 11 dma chan witch is related to enum devdrv_dma_data_type */
#define DEVDRV_DMA_MSI_X_VECTOR_BASE 48
#define DEVDRV_DMA_MSI_X_VECTOR_NUM 32

/* msg chan irq section2 */
#define DEVDRV_MSG_MSI_X_VECTOR_2_BASE 128

/* ************ interrupt defined for normal host ************* */
#define DEVDRV_MSI_X_MDEV_VF_MAX_VECTORS 256
#define DEVDRV_MSI_X_MDEV_VF_MIN_VECTORS 256
#define DEVDRV_MSI_X_VF_MAX_VECTORS 64
#define DEVDRV_MSI_X_VF_MIN_VECTORS 64

/* device os load notify use irq vector 0, later 0 alse use to admin msg chan */
#define DEVDRV_LOAD_MSI_X_VF_VECTOR_NUM 0

/* irq used to msg trans, a msg chan need two vector. one for tx finish, the other for rx msg.
   msg chan 0 is used to admin(chan 0) role */
#define DEVDRV_MSG_MSI_X_VF_VECTOR_BASE 0
#define DEVDRV_MSG_MSI_X_VF_VECTOR_NUM 16

/* irq used to dma, a dma chan need 22 vector. one for cq, the other for err.
  host surport 11 dma chan witch is related to enum devdrv_dma_data_type */
#define DEVDRV_DMA_MSI_X_VF_VECTOR_BASE 16
#define DEVDRV_DMA_MSI_X_VF_VECTOR_NUM 24

/* msg chan irq section2 */
#define DEVDRV_MSG_MSI_X_VF_VECTOR_2_BASE 64

#if defined(CFG_PLATFORM_ESL) || defined(CFG_PLATFORM_FPGA)
#define DMA_CHAN_REMOTE_USED_NUM            16
#define DMA_CHAN_REMOTE_USED_START_INDEX    30
#define DMA_CHAN_BAR_SPACE_USED_START_INDEX 0
#else
#define DMA_CHAN_REMOTE_USED_NUM            16
#define DMA_CHAN_REMOTE_USED_START_INDEX    30
#define DMA_CHAN_BAR_SPACE_USED_START_INDEX 0
#endif

#define HOST_VF_DMA_MASK 0x7FF80000000ULL /* channal 31~42 for host VF use, 30 for host PF only */

#define DEVDRV_MAX_DMA_CH_SQ_DEPTH 0x10000
#define DEVDRV_MAX_DMA_CH_CQ_DEPTH 0x10000
#define DEVDRV_DMA_CH_SQ_DESC_RSV  0x400

#define CLOUD_V4_MODULE_NUM        8
#define CLOUD_V4_VF_VM_MODULE_NUM  6
#define DEVDRV_MAX_MSG_PF_CHAN_CNT 46
#define DEVDRV_MAX_MSG_VF_CHAN_CNT 16
/* msg chan cnt for modules */
#define DEVDRV_PCIVNIC_DEV_MSG_CHAN_CNT_MAX       1   /* non_trans:1 */
#define DEVDRV_SMMU_DEV_MSG_CHAN_CNT_MAX          1   /* non_trans:1 */
#define DEVDRV_DEVMM_DEV_MSG_CHAN_CNT_MAX         1   /* non_trans:1 */
#define DEVDRV_COMMON_DEV_MSG_CHAN_CNT_MAX        1   /* non_trans:1 */
#define DEVDRV_DEV_MAMAGER_MSG_CHAN_CNT_MAX       1   /* non_trans:1 */
#define DEVDRV_TSDRV_MSG_CHAN_CNT_MAX             1   /* non_trans:1 */
#define DEVDRV_DEV_HDC_PF_MSG_CHAN_CNT_MAX        32  /* trans:31 non_trans:1 */
#define DEVDRV_QUEUE_MSG_CHAN_CNT_MAX             1   /* non_trans:1 */
#define DEVDRV_S2S_MSG_CHAN_CNT_MAX               1   /* non_trans:1 */

#define DEVDRV_DEV_HDC_VF_MSG_CHAN_CNT_MAX        3   /* trans:2 non_trans:1 */

#define CLOUD_V4_NVME_LOW_LEVEL_DB_IRQ_NUM 2
#define CLOUD_V4_NVME_DB_IRQ_STRDE 8

STATIC unsigned int devdrv_pf_msg_chan_cnt_cloud_v4[devdrv_msg_client_max] = {
    DEVDRV_PCIVNIC_DEV_MSG_CHAN_CNT_MAX,    /* used for pcivnic */
    DEVDRV_SMMU_DEV_MSG_CHAN_CNT_MAX,       /* used for test */
    DEVDRV_DEVMM_DEV_MSG_CHAN_CNT_MAX,      /* used for svm */
    DEVDRV_COMMON_DEV_MSG_CHAN_CNT_MAX,     /* used for common */
    DEVDRV_DEV_MAMAGER_MSG_CHAN_CNT_MAX,    /* used for device manager */
    DEVDRV_TSDRV_MSG_CHAN_CNT_MAX,          /* used for tsdrv */
    DEVDRV_DEV_HDC_PF_MSG_CHAN_CNT_MAX,     /* used for hdc */
    DEVDRV_QUEUE_MSG_CHAN_CNT_MAX,          /* used for queue */
    DEVDRV_S2S_MSG_CHAN_CNT_MAX,            /* used for s2s */
};

STATIC unsigned int devdrv_vf_msg_chan_cnt_cloud_v4[devdrv_msg_client_max] = {
    DEVDRV_PCIVNIC_DEV_MSG_CHAN_CNT_MAX,    /* used for pcivnic */
    DEVDRV_SMMU_DEV_MSG_CHAN_CNT_MAX,       /* used for test */
    DEVDRV_DEVMM_DEV_MSG_CHAN_CNT_MAX,      /* used for svm */
    DEVDRV_COMMON_DEV_MSG_CHAN_CNT_MAX,     /* used for common */
    DEVDRV_DEV_MAMAGER_MSG_CHAN_CNT_MAX,    /* used for device manager */
    DEVDRV_TSDRV_MSG_CHAN_CNT_MAX,          /* used for tsdrv */
    DEVDRV_DEV_HDC_VF_MSG_CHAN_CNT_MAX,     /* used for hdc */
    DEVDRV_QUEUE_MSG_CHAN_CNT_MAX,          /* used for queue */
};

#ifdef CFG_FEATURE_LOAD_TEE_IMAGE
#define CLOUD_V4_BLOCKS_NUM        5
#else
#define CLOUD_V4_BLOCKS_NUM        4
#endif

/* load file adapt */
STATIC struct devdrv_load_file_cfg cloud_v4_file[CLOUD_V4_BLOCKS_NUM] = {
#ifdef CFG_FEATURE_LOAD_TEE_IMAGE
    {
        .file_name = "/driver/device/ascend_910_95_tee.bin",
        .file_type = DEVDRV_CRITICAL_FILE,
        .fail_mode = DEVDRV_NON_NOTICE,
    },
#endif
    {
        .file_name = "/driver/device/ascend_910_95.image",
        .file_type = DEVDRV_CRITICAL_FILE,
        .fail_mode = DEVDRV_NON_NOTICE,
    },
    {
        .file_name = "/driver/device/ascend_910_95.cpio.gz",
        .file_type = DEVDRV_CRITICAL_FILE,
        .fail_mode = DEVDRV_NON_NOTICE,
    },
    {
        .file_name = "/driver/device/ascend_910_95_dt.img",
        .file_type = DEVDRV_CRITICAL_FILE,
        .fail_mode = DEVDRV_NON_NOTICE,
    },
    {
        .file_name = "/driver/device/ascend_cloud_v4.crl",
        .file_type = DEVDRV_NON_CRITICAL_FILE,
        .fail_mode = DEVDRV_NON_NOTICE,
    },
};

#ifdef CFG_FEATURE_LOAD_TEE_IMAGE
#define CLOUD_V5_BLOCKS_NUM        5
#else
#define CLOUD_V5_BLOCKS_NUM        4
#endif

/* load file adapt */
STATIC struct devdrv_load_file_cfg cloud_v5_file[CLOUD_V5_BLOCKS_NUM] = {
#ifdef CFG_FEATURE_LOAD_TEE_IMAGE
    {
        .file_name = "/driver/device/ascend_910_96_tee.bin",
        .file_type = DEVDRV_CRITICAL_FILE,
        .fail_mode = DEVDRV_NON_NOTICE,
    },
#endif
    {
        .file_name = "/driver/device/ascend_910_96.image",
        .file_type = DEVDRV_CRITICAL_FILE,
        .fail_mode = DEVDRV_NON_NOTICE,
    },
    {
        .file_name = "/driver/device/ascend_910_96.cpio.gz",
        .file_type = DEVDRV_CRITICAL_FILE,
        .fail_mode = DEVDRV_NON_NOTICE,
    },
    {
        .file_name = "/driver/device/ascend_910_96_dt.img",
        .file_type = DEVDRV_CRITICAL_FILE,
        .fail_mode = DEVDRV_NON_NOTICE,
    },
    {
        .file_name = "/driver/device/ascend_cloud_v5.crl",
        .file_type = DEVDRV_NON_CRITICAL_FILE,
        .fail_mode = DEVDRV_NON_NOTICE,
    },
};

STATIC void devdrv_cloud_v4_init_pf_bar_addr_info(struct devdrv_pci_ctrl *pci_ctrl)
{
    pci_ctrl->res.msg_db.addr = pci_ctrl->io_phy_base + DEVDRV_SOC_DOORBELL_OFFSET + DEVDRV_IEP_SDI0_DB_OFFSET;
    pci_ctrl->res.msg_db.size = DEVDRV_DB_IOMAP_SIZE;

    pci_ctrl->res.msg_mem.addr = pci_ctrl->rsv_mem_phy_base + g_reserver_mem_msg_offset;
    pci_ctrl->res.msg_mem.size = DEVDRV_RESERVE_MEM_MSG_SIZE;

    pci_ctrl->res.load_sram.addr = pci_ctrl->io_phy_base + DEVDRV_IO_LOAD_SRAM_OFFSET;
    pci_ctrl->res.load_sram.size = DEVDRV_IO_LOAD_SRAM_SIZE;

    pci_ctrl->res.bbox.addr = pci_ctrl->rsv_mem_phy_base + DEVDRV_RESERVE_BBOX_OFFSET;
    pci_ctrl->res.bbox.size = DEVDRV_RESERVE_BBOX_SIZE;

    pci_ctrl->res.imu_log.addr = pci_ctrl->rsv_mem_phy_base + DEVDRV_RESERVE_IMU_LOG_OFFSET;
    pci_ctrl->res.imu_log.size = DEVDRV_RESERVE_IMU_LOG_SIZE;

    pci_ctrl->res.hdr.addr = pci_ctrl->rsv_mem_phy_base + DEVDRV_RESERVE_HDR_OFFSET;
    pci_ctrl->res.hdr.size = DEVDRV_RESERVE_HDR_SIZE;

    pci_ctrl->res.reg_sram.addr = pci_ctrl->io_phy_base + DEVDRV_IO_LOAD_SRAM_OFFSET;
    pci_ctrl->res.reg_sram.size = DEVDRV_IO_LOAD_SRAM_SIZE;

    pci_ctrl->res.l3d_sram.addr = pci_ctrl->io_phy_base + DEVDRV_HBOOT_L3D_SRAM_OFFSET;
    pci_ctrl->res.l3d_sram.size = DEVDRV_HBOOT_L3D_SRAM_SIZE;
    /* not surport */
    pci_ctrl->res.vpc.size = 0;
    pci_ctrl->res.dvpp.size = 0;
    pci_ctrl->res.hwts.size = 0;
    pci_ctrl->res.vf_bandwidth.addr = 0;
    pci_ctrl->res.vf_bandwidth.size = 0;
}

STATIC void devdrv_cloud_v4_init_vf_bar_addr_info(struct devdrv_pci_ctrl *pci_ctrl)
{
    pci_ctrl->res.msg_mem.addr = pci_ctrl->rsv_mem_phy_base + DEVDRV_VF_MEM_MSG_OFFSET;
    pci_ctrl->res.msg_mem.size = DEVDRV_VF_MEM_MSG_SIZE;

    pci_ctrl->res.msg_db.addr = pci_ctrl->io_phy_base + DEVDRV_VF_IO_SOC_DOORBELL_OFFSET;
    pci_ctrl->res.msg_db.size = DEVDRV_VF_IO_SOC_DOORBELL_SIZE;

    pci_ctrl->res.load_sram.addr = pci_ctrl->io_phy_base + DEVDRV_VF_IO_LOAD_SRAM_OFFSET;
    pci_ctrl->res.load_sram.size = DEVDRV_VF_IO_LOAD_SRAM_SIZE;

    if (pci_ctrl->env_boot_mode == DEVDRV_MDEV_VF_VM_BOOT) {
        pci_ctrl->res.vpc.addr = pci_ctrl->mdev_rsv_mem_phy_base + DEVDRV_VF_MDEV_VPC_OFFSET;
        pci_ctrl->res.vpc.size = DEVDRV_VF_MDEV_VPC_SIZE;

        pci_ctrl->res.dvpp.addr = pci_ctrl->mdev_rsv_mem_phy_base + DEVDRV_VF_MDEV_DVPP_OFFSET;
        pci_ctrl->res.dvpp.size = DEVDRV_VF_MDEV_DVPP_SIZE;
    } else {
        pci_ctrl->res.vpc.size = 0;
        pci_ctrl->res.dvpp.size = 0;
    }

    /* not surport */

    pci_ctrl->res.bbox.addr = 0;
    pci_ctrl->res.bbox.size = 0;

    pci_ctrl->res.imu_log.addr = 0;
    pci_ctrl->res.imu_log.size = 0;

    pci_ctrl->res.hdr.size = 0;
    pci_ctrl->res.reg_sram.size = 0;
    pci_ctrl->res.vf_bandwidth.addr = 0;
    pci_ctrl->res.vf_bandwidth.size = 0;

    pci_ctrl->res.l3d_sram.addr = 0;
    pci_ctrl->res.l3d_sram.size = 0;
}

STATIC void devdrv_cloud_v4_init_pf_intr_info(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_intr_info *intr = &pci_ctrl->res.intr;

    intr->min_vector = DEVDRV_MSI_X_MIN_VECTORS;
    intr->max_vector = DEVDRV_MSI_X_MAX_VECTORS;
    intr->device_os_load_irq = DEVDRV_LOAD_MSI_X_VECTOR_NUM;
    intr->msg_irq_base = DEVDRV_MSG_MSI_X_VECTOR_BASE;
    intr->msg_irq_num = DEVDRV_MSG_MSI_X_VECTOR_NUM;
    intr->dma_irq_base = DEVDRV_DMA_MSI_X_VECTOR_BASE;
    intr->dma_irq_num = DEVDRV_DMA_MSI_X_VECTOR_NUM;
    intr->msg_irq_vector2_base = DEVDRV_MSG_MSI_X_VECTOR_2_BASE;
    intr->msg_irq_vector2_num = intr->max_vector - DEVDRV_MSG_MSI_X_VECTOR_2_BASE;
    pci_ctrl->msix_ctrl.next_entry = DEVDRV_DMA_MSI_X_VECTOR_BASE + DEVDRV_DMA_MSI_X_VECTOR_NUM;
}

STATIC void devdrv_cloud_v4_init_vf_intr_info(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_intr_info *intr = &pci_ctrl->res.intr;

    if (pci_ctrl->env_boot_mode == DEVDRV_MDEV_VF_VM_BOOT) { // in vm
        intr->min_vector = DEVDRV_MSI_X_MDEV_VF_MIN_VECTORS;
        intr->max_vector = DEVDRV_MSI_X_MDEV_VF_MAX_VECTORS;
    } else {                                                 // vf in pm or container or vm direct
        intr->min_vector = DEVDRV_MSI_X_VF_MIN_VECTORS;
        intr->max_vector = DEVDRV_MSI_X_VF_MAX_VECTORS;
    }

    intr->device_os_load_irq = DEVDRV_LOAD_MSI_X_VF_VECTOR_NUM;
    intr->msg_irq_base = DEVDRV_MSG_MSI_X_VF_VECTOR_BASE;
    intr->msg_irq_num = DEVDRV_MSG_MSI_X_VF_VECTOR_NUM;
    intr->dma_irq_base = DEVDRV_DMA_MSI_X_VF_VECTOR_BASE;
    intr->dma_irq_num = DEVDRV_DMA_MSI_X_VF_VECTOR_NUM;
    intr->msg_irq_vector2_base = DEVDRV_MSG_MSI_X_VF_VECTOR_2_BASE;
    intr->msg_irq_vector2_num = 0;
    pci_ctrl->msix_ctrl.next_entry = DEVDRV_DMA_MSI_X_VF_VECTOR_BASE + DEVDRV_DMA_MSI_X_VF_VECTOR_NUM;
}

STATIC void devdrv_cloud_v4_init_pf_dma_info(struct devdrv_pci_ctrl *pci_ctrl)
{
    int total_func_num = pci_ctrl->shr_para->dev_num;
    u32 i;

    if (total_func_num == 0) {
        devdrv_warn("Input parameter is invalid. (total_func_num=%d)\n", total_func_num);
        total_func_num = 1;
    }

    pci_ctrl->res.dma_res.pf_num = pci_ctrl->func_id;
    pci_ctrl->res.dma_res.dma_addr = pci_ctrl->io_base + DEVDRV_IEP_DMA_OFFSET;
    pci_ctrl->res.dma_res.dma_chan_addr = pci_ctrl->res.dma_res.dma_addr + DEVDRV_IEP_DMA_CHAN_OFFSET;
    pci_ctrl->res.dma_res.dma_chan_num = (u32)(DMA_CHAN_REMOTE_USED_NUM / total_func_num);
    for (i = 0; i < pci_ctrl->res.dma_res.dma_chan_num; i++) {
        pci_ctrl->res.dma_res.use_chan[i] = i + pci_ctrl->func_id * pci_ctrl->res.dma_res.dma_chan_num;
    }
    pci_ctrl->res.dma_res.dma_chan_start_id = DMA_CHAN_REMOTE_USED_START_INDEX;
    pci_ctrl->res.dma_res.chan_start_id = pci_ctrl->res.dma_res.dma_chan_start_id;
    pci_ctrl->res.dma_res.vf_id = 0;
    pci_ctrl->res.dma_res.sq_depth = DEVDRV_MAX_DMA_CH_SQ_DEPTH;
    pci_ctrl->res.dma_res.sq_rsv_num = DEVDRV_DMA_CH_SQ_DESC_RSV;
    pci_ctrl->res.dma_res.cq_depth = DEVDRV_MAX_DMA_CH_CQ_DEPTH;

    devdrv_info("pf dma info. (func_id=%u; func_num=%u; chan_num=%u)\n",
        pci_ctrl->func_id, total_func_num, pci_ctrl->res.dma_res.dma_chan_num);
}

STATIC void devdrv_cloud_v4_init_vf_dma_info(struct devdrv_pci_ctrl *pci_ctrl)
{
    unsigned long host_bitmap = (unsigned long)pci_ctrl->shr_para->dma_bitmap & HOST_VF_DMA_MASK;
    u32 chan_num = (u32)bitmap_weight(&host_bitmap, DEVDRV_DMA_MAX_CHAN_NUM);
    u32 idx, i;

    pci_ctrl->res.dma_res.dma_chan_num = chan_num;
    idx = DMA_CHAN_REMOTE_USED_START_INDEX;
    for (i = 0; i < chan_num; i++) {
        idx = (u32)find_next_bit(&host_bitmap, DEVDRV_DMA_MAX_CHAN_NUM, idx);
        if (idx >= DMA_CHAN_REMOTE_USED_START_INDEX) {
            pci_ctrl->res.dma_res.use_chan[i] = idx - DMA_CHAN_REMOTE_USED_START_INDEX;
            devdrv_info("vf dma info. (i=%u; use_chan=%u; chan_num=%u; vfid=%u; bitmap=0x%lx)\n",
                i, pci_ctrl->res.dma_res.use_chan[i], pci_ctrl->res.dma_res.dma_chan_num, pci_ctrl->shr_para->vf_id,
                (unsigned long)pci_ctrl->shr_para->dma_bitmap);
        }
        idx++;
    }

    pci_ctrl->res.dma_res.dma_chan_start_id = DMA_CHAN_REMOTE_USED_START_INDEX;
    pci_ctrl->res.dma_res.chan_start_id = pci_ctrl->res.dma_res.dma_chan_start_id;
    pci_ctrl->res.dma_res.pf_num = 0;
    pci_ctrl->res.dma_res.vf_id = pci_ctrl->shr_para->vf_id;
    pci_ctrl->res.dma_res.dma_addr = pci_ctrl->io_base + DEVDRV_VF_DMA_OFFSET;
    pci_ctrl->res.dma_res.dma_chan_addr = pci_ctrl->res.dma_res.dma_addr + DEVDRV_IEP_DMA_CHAN_OFFSET;
    pci_ctrl->res.dma_res.sq_depth = DEVDRV_MAX_DMA_CH_SQ_DEPTH;
    pci_ctrl->res.dma_res.sq_rsv_num = DEVDRV_DMA_CH_SQ_DESC_RSV;
    pci_ctrl->res.dma_res.cq_depth = DEVDRV_MAX_DMA_CH_CQ_DEPTH;
}

STATIC void devdrv_cloud_v4_init_load_file_info(struct devdrv_pci_ctrl *pci_ctrl)
{
    devdrv_info("Get shr_para host_interrupt_flag. (flag=%d).\n", pci_ctrl->shr_para->host_interrupt_flag);
    pci_ctrl->shr_para->host_interrupt_flag = 0;
    if (pci_ctrl->chip_type == HISI_CLOUD_V5) {
        pci_ctrl->res.load_file.load_file_num = CLOUD_V5_BLOCKS_NUM;
        pci_ctrl->res.load_file.load_file_cfg = cloud_v5_file;
    } else {
        pci_ctrl->res.load_file.load_file_num = CLOUD_V4_BLOCKS_NUM;
        pci_ctrl->res.load_file.load_file_cfg = cloud_v4_file;
    }
}

STATIC enum devdrv_load_wait_mode devdrv_cloud_v4_get_load_wait_mode(struct devdrv_pci_ctrl *pci_ctrl)
{
    if (pci_ctrl->ops.pre_cfg != NULL) {
        pci_ctrl->ops.pre_cfg(pci_ctrl);
    }

    if (pci_ctrl->ops.link_speed_slow_to_normal != NULL) {
        pci_ctrl->ops.link_speed_slow_to_normal(pci_ctrl);
    }
    return DEVDRV_LOAD_WAIT_INTERVAL;
}

STATIC int devdrv_cloud_v4_get_pf_msg_chan_cnt(void)
{
    return DEVDRV_MAX_MSG_PF_CHAN_CNT;
}

STATIC int devdrv_cloud_v4_get_vf_msg_chan_cnt(void)
{
    return DEVDRV_MAX_MSG_VF_CHAN_CNT;
}

STATIC int devdrv_get_cloud_v4_devid_by_slotid(void)
{
    int dev_id = -1;
    struct devdrv_ctrl *p_ctrls = get_devdrv_ctrl();

    if (g_hw_info.npu_slot_id < DEVDRV_CLOUD_V4_SLOT_ID_MAX &&
        g_hw_info.npu_module_id < DEVDRV_CLOUD_V4_MODULE_ID_MAX) {
        dev_id = (int)(DEVDRV_CLOUD_V4_MODULE_ID_MAX * g_hw_info.npu_slot_id + g_hw_info.npu_module_id);
        devdrv_info("Get hw info. (mainboard_id0=%hhu; dev_id=%d)\n",
            g_hw_info.mainboard_id0.id, dev_id);
    } else {
        devdrv_err("Input parameter is invalid. (npu_module_id=%hhu; npu_slot_id=%hhu)\n",
            g_hw_info.npu_module_id, g_hw_info.npu_slot_id);
        return dev_id;
    }

    if (p_ctrls[dev_id].priv != NULL) {
        devdrv_err("This dev_id is already registered. (dev_id=%d)\n", dev_id);
        dev_id = -1;
    } else {
        p_ctrls[dev_id].startup_flg = DEVDRV_DEV_STARTUP_PROBED;
    }
    return dev_id;
}

STATIC int devdrv_get_cloud_v4_pcie_card_devid(struct devdrv_pci_ctrl *pci_ctrl)
{
    int dev_id = -1;
#if defined(CFG_FEATURE_DMI) && defined(CONFIG_DMI)
    const struct dmi_device *dmi_dev = NULL;
    const struct dmi_device *from = NULL;
    const struct dmi_dev_onboard *dev_data = NULL;
    struct devdrv_ctrl *p_ctrls = get_devdrv_ctrl();

    if ((pci_ctrl == NULL) || (pci_ctrl->pdev == NULL) || (pci_ctrl->pdev->bus == NULL)) {
        devdrv_err("Input parameter is invalid.\n");
        return dev_id;
    }

    do {
        from = dmi_dev;
        dmi_dev = dmi_find_device(DEVDRV_DMI_DEV_TYPE_DEV_SLOT, NULL, from);
        if (dmi_dev != NULL) {
            dev_data = (struct dmi_dev_onboard *)dmi_dev->device_data;
#ifdef CONFIG_PCI_DOMAINS_GENERIC
            if ((dev_data != NULL) && (dev_data->bus == pci_ctrl->pdev->bus->number) &&
                (PCI_SLOT(((unsigned int)(dev_data->devfn))) == PCI_SLOT(pci_ctrl->pdev->devfn)) &&
                (dev_data->segment == pci_ctrl->pdev->bus->domain_nr)) {
                dev_id = dev_data->instance;
                break;
            }
#else
            if ((dev_data != NULL) && (dev_data->bus == pci_ctrl->pdev->bus->number) &&
                (PCI_SLOT(((unsigned int)(dev_data->devfn))) == PCI_SLOT(pci_ctrl->pdev->devfn))) {
                dev_id = dev_data->instance;
                break;
            }
#endif
        }
    } while (dmi_dev != NULL);

    if ((dev_id >= MAX_PF_DEV_CNT) || (dev_id < 0)) {
        devdrv_info("Dmi not match dev_id, do logical way (dev_id=%d)\n", dev_id);
        dev_id = devdrv_alloc_devid_inturn(0, 1);
        return dev_id;
    }

    if (p_ctrls[dev_id].priv != NULL) {
        devdrv_err("This dev_id is already registered. (dev_id=%u)\n", dev_id);
        dev_id = -1;
    } else {
        p_ctrls[dev_id].startup_flg = DEVDRV_DEV_STARTUP_PROBED;
    }
#else
    dev_id = devdrv_alloc_devid_inturn(0, 1);
#endif

    return dev_id;
}

/* devid = pf_if * 16 + (vf_id - 1) + 100 */
STATIC int devdrv_sriov_cloud_v4_alloc_vf_devid(struct devdrv_pci_ctrl *pci_ctrl)
{
    int pf_dev_id, vf_dev_id;
    int dev_id;

    pf_dev_id = devdrv_sriov_get_pf_devid_by_vf_ctrl(pci_ctrl);
    if (pf_dev_id < 0) {
        return devdrv_alloc_devid_inturn(0, 1);
    }

    vf_dev_id = (int)pci_ctrl->func_id;
    if ((pf_dev_id < 0) || (pf_dev_id >= MAX_PF_DEV_CNT) || (vf_dev_id <= 0) || (vf_dev_id >= MAX_VF_CNT_OF_PF)) {
        devdrv_err("pf_dev_id[%d] or vf_dev_id[%d] is invalid\n", pf_dev_id, vf_dev_id);
        return -1;
    }
    dev_id = pf_dev_id * MAX_VF_CNT_OF_PF + (vf_dev_id - 1) + DEVDRV_SRIOV_VF_DEVID_START;

    devdrv_info("Get vf devid, (pf_dev_id=%d, vf_dev_id=%d, dev_id=%d)\n", pf_dev_id, vf_dev_id, dev_id);
    return dev_id;
}

STATIC int devdrv_cloud_v4_alloc_devid(struct devdrv_ctrl *ctrl_this)
{
    struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)ctrl_this->priv;
    devdrv_hw_mainboard_id_t value = {{0}};
    int dev_id;

    dev_id = devdrv_alloc_devid_check_ctrls(ctrl_this);
    if (dev_id != -1) {
        return dev_id;
    }

    if (pci_ctrl->env_boot_mode == DEVDRV_MDEV_VF_VM_BOOT) {
        return devdrv_alloc_devid_inturn(0, 1);
    }

    if (pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_VF) {
        return devdrv_sriov_cloud_v4_alloc_vf_devid(pci_ctrl);
    }

    devdrv_info("Get share para value. (board_type=%d; node_id=%d; chip_id=%d; slot_id=%d)\n",
        pci_ctrl->shr_para->board_type, pci_ctrl->shr_para->node_id,
        pci_ctrl->shr_para->chip_id, pci_ctrl->shr_para->slot_id);
    value.id = g_hw_info.mainboard_id0.id;
    if (value.bits.machine_form == DEVDRV_MACHINE_FORM_CLOUD_V4_AX_SERVER) {
        return devdrv_get_cloud_v4_devid_by_slotid();
    } else if (value.bits.machine_form == DEVDRV_MACHINE_FORM_CLOUD_V4_PCIE_CARD) {
        dev_id = devdrv_get_cloud_v4_pcie_card_devid(pci_ctrl);
        return dev_id;
    } else {
        return devdrv_alloc_devid_inturn(0, 1);
    }
}

STATIC u32 devdrv_cloud_v4_get_p2p_support_max_devnum(void)
{
    return DEVDRV_CLOUD_V4_P2P_SUPPORT_MAX_DEVICE;
}

STATIC int devdrv_cloud_v4_is_p2p_access_cap(struct devdrv_pci_ctrl *pci_ctrl, struct devdrv_pci_ctrl *peer_pci_ctrl)
{
    if ((pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_VF) || (peer_pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_VF)) {
        devdrv_info("devid and peer_devid is vf, not support p2p. (dev_id=%u; peer_devid=%u)\n",
            pci_ctrl->dev_id, peer_pci_ctrl->dev_id);
        return DEVDRV_P2P_ACCESS_DISABLE;
    }

    if ((PCI_FUNC(pci_ctrl->pdev->devfn) > 0) || (PCI_FUNC(peer_pci_ctrl->pdev->devfn) > 0)) {
        devdrv_info("devid and peer_devid is 1pf2die's die1, not support p2p. (dev_id=%u; peer_devid=%u)\n",
            pci_ctrl->dev_id, peer_pci_ctrl->dev_id);
        return DEVDRV_P2P_ACCESS_DISABLE;
    }

    return DEVDRV_P2P_ACCESS_ENABLE;
}

STATIC bool devdrv_cloud_v4_is_mdev_vm_full_spec(struct devdrv_pci_ctrl *pci_ctrl)
{
    return false;
}

STATIC void devdrv_cloud_v4_set_dev_shr_info(struct devdrv_pci_ctrl *pci_ctrl)
{
    /* vf's vm not set host_dev_id, use host_dev_id on vf's pm */
    if (pci_ctrl->env_boot_mode != DEVDRV_MDEV_VF_VM_BOOT) {
        pci_ctrl->shr_para->host_dev_id = (int)pci_ctrl->dev_id;
    }
    pci_ctrl->shr_para->host_mem_bar_base = (u64)pci_ctrl->mem_phy_base;
    pci_ctrl->shr_para->host_io_bar_base = (u64)pci_ctrl->io_phy_base;
}

STATIC unsigned devdrv_cloud_v4_get_server_id(struct devdrv_pci_ctrl *pci_ctrl)
{
    devdrv_hw_info_t *hw_data = NULL;
    void __iomem *hw_info_addr = pci_ctrl->io_base + DEVDRV_IO_LOAD_SRAM_OFFSET + DEVDRV_CLOUD_V4_HW_INFO_SRAM_OFFSET;

    hw_data = (devdrv_hw_info_t*)hw_info_addr;
    g_hw_info.server_id = hw_data->server_id;

    return g_hw_info.server_id;
}

STATIC unsigned devdrv_cloud_v4_get_max_server_num(struct devdrv_pci_ctrl *pci_ctrl)
{
    devdrv_hw_info_t *hw_data = NULL;
    void __iomem *hw_info_addr = pci_ctrl->io_base + DEVDRV_IO_LOAD_SRAM_OFFSET + DEVDRV_CLOUD_V4_HW_INFO_SRAM_OFFSET;

    hw_data = (devdrv_hw_info_t*)hw_info_addr;
    g_hw_info.scale_type = hw_data->scale_type;

    return (g_hw_info.scale_type / DEVDRV_S2S_MAX_CHIP_NUM);
}

STATIC int devdrv_cloud_v4_set_udevid_reorder_para(struct devdrv_pci_ctrl *pci_ctrl)
{
    void __iomem * para_addr = NULL;
    bool reorder_flag = false;
    int ret;
    devdrv_hw_mainboard_id_t value = {{0}};
    value.id = g_hw_info.mainboard_id0.id;
    devdrv_info("machine_form = %u, sub_machine_form = %u\n", value.bits.machine_form, value.bits.sub_machine_form);
    if (value.bits.machine_form == DEVDRV_MACHINE_FORM_CLOUD_V4_PCIE_CARD) {
        reorder_flag = true;
        para_addr = (void __iomem *)pci_ctrl->shr_para + offsetof(struct devdrv_shr_para, reorder_para);
        ret = uda_set_udevid_reorder_para(pci_ctrl->dev_id, (struct udevid_reorder_para*)para_addr);
        if (ret != 0) {
            devdrv_err("Set udevid reorder para fail. (dev_id=%u; ret=%d)\n", pci_ctrl->dev_id, ret);
            return ret;
        }
    }
    reorder_flag = false;
    uda_set_udevid_reorder_flag(reorder_flag);
    devdrv_info("udevid reorder flag=%u, index_id=%u\n", reorder_flag, pci_ctrl->dev_id);

    return 0;
}

STATIC u32 devdrv_cloud_v4_get_nvme_low_level_db_irq_num(void)
{
    return CLOUD_V4_NVME_LOW_LEVEL_DB_IRQ_NUM;
}

STATIC u32 devdrv_cloud_v4_get_nvme_db_irq_strde(void)
{
    return CLOUD_V4_NVME_DB_IRQ_STRDE;
}

STATIC void devdrv_cloud_v4_ops_init(struct devdrv_pci_ctrl *pci_ctrl)
{
    pci_ctrl->ops.shr_para_rebuild = NULL;
    pci_ctrl->ops.alloc_devid = devdrv_cloud_v4_alloc_devid;
    pci_ctrl->ops.is_p2p_access_cap = devdrv_cloud_v4_is_p2p_access_cap;
    pci_ctrl->ops.probe_wait = devdrv_probe_wait;
    pci_ctrl->ops.bind_irq = devdrv_bind_irq;
    pci_ctrl->ops.unbind_irq = devdrv_unbind_irq;
    pci_ctrl->ops.get_load_wait_mode = devdrv_cloud_v4_get_load_wait_mode;
    pci_ctrl->ops.get_pf_max_msg_chan_cnt = devdrv_cloud_v4_get_pf_msg_chan_cnt;
    pci_ctrl->ops.get_vf_max_msg_chan_cnt = devdrv_cloud_v4_get_vf_msg_chan_cnt;
    pci_ctrl->ops.get_p2p_support_max_devnum = devdrv_cloud_v4_get_p2p_support_max_devnum;
    pci_ctrl->ops.get_vf_dma_info = devdrv_cloud_v4_init_vf_dma_info;
    pci_ctrl->ops.get_hccs_link_info = NULL;
    pci_ctrl->ops.is_mdev_vm_full_spec = devdrv_cloud_v4_is_mdev_vm_full_spec;
    pci_ctrl->ops.get_server_id = devdrv_cloud_v4_get_server_id;
    pci_ctrl->ops.get_max_server_num = devdrv_cloud_v4_get_max_server_num;
    pci_ctrl->ops.devdrv_deal_suspend_handshake = NULL;
    pci_ctrl->ops.is_all_dev_unified_addr = NULL;
    pci_ctrl->ops.flush_cache = NULL;
    pci_ctrl->ops.get_peh_link_info = NULL;
    pci_ctrl->ops.link_speed_slow_to_normal = NULL;
    pci_ctrl->ops.set_dev_shr_info = devdrv_cloud_v4_set_dev_shr_info;
    pci_ctrl->ops.get_p2p_addr = NULL;
    pci_ctrl->ops.set_udevid_reorder_para = devdrv_cloud_v4_set_udevid_reorder_para;
    pci_ctrl->ops.get_nvme_low_level_db_irq_num = devdrv_cloud_v4_get_nvme_low_level_db_irq_num;
    pci_ctrl->ops.get_nvme_db_irq_strde = devdrv_cloud_v4_get_nvme_db_irq_strde;
    pci_ctrl->ops.pre_cfg = NULL;
}

STATIC void devdrv_cloud_v4_init_pf_msg_cnt(struct devdrv_pci_ctrl *pci_ctrl)
{
    int i;
    for (i = 0; i < devdrv_msg_client_max; i++) {
        pci_ctrl->res.msg_chan_cnt[i] = (int)devdrv_pf_msg_chan_cnt_cloud_v4[i];
    }
}

STATIC void devdrv_cloud_v4_init_vf_msg_cnt(struct devdrv_pci_ctrl *pci_ctrl)
{
    int i;
    for (i = 0; i < devdrv_msg_client_max; i++) {
        pci_ctrl->res.msg_chan_cnt[i] = (int)devdrv_vf_msg_chan_cnt_cloud_v4[i];
    }
}

STATIC int devdrv_init_hw_info(struct devdrv_pci_ctrl *pci_ctrl)
{
    void __iomem *hw_info_addr = pci_ctrl->io_base + DEVDRV_IO_LOAD_SRAM_OFFSET + DEVDRV_CLOUD_V4_HW_INFO_SRAM_OFFSET;
    int retry_times = 0;

retry:
    memcpy_fromio(&g_hw_info, hw_info_addr, sizeof(devdrv_hw_info_t));

    if ((retry_times < DEVDRV_INIT_HWINFO_RETRY_TIME) && (g_hw_info.version == (u8)INVALID_HW_INFO_VERSION) &&
        (g_hw_info.mainboard_id0.id == (u8)INVALID_HW_INFO_MAINBOARD_ID)) {
        msleep(DEVDRV_INIT_HWINFO_WAIT_TIME);
        retry_times++;
        goto retry;
    }

    devdrv_info("Get hw info. (mainboard_id0=%hhu)\n", g_hw_info.mainboard_id0.id);

    if (retry_times == DEVDRV_INIT_HWINFO_RETRY_TIME) {
        return -ENODEV;
    }
    return 0;
}

STATIC int devdrv_cloud_v4_init_pf_bar_info(struct devdrv_pci_ctrl *pci_ctrl)
{
    resource_size_t offset;
    unsigned long size;
    int ret;

    pci_ctrl->io_phy_base = (phys_addr_t)pci_resource_start(pci_ctrl->pdev, PCI_BAR_IO);
    pci_ctrl->io_phy_size = (u64)pci_resource_len(pci_ctrl->pdev, PCI_BAR_IO);

    pci_ctrl->io_base = ioremap(pci_ctrl->io_phy_base, pci_ctrl->io_phy_size);
    if (pci_ctrl->io_base == NULL) {
        devdrv_err("Ioremap io_base failed. (size=0x%llx)\n", pci_ctrl->io_phy_size);
        devdrv_res_uninit(pci_ctrl);
        return -ENOMEM;
    }

    ret = devdrv_init_hw_info(pci_ctrl);
    if (ret != 0) {
        devdrv_err("Init hw info failed.");
        devdrv_res_uninit(pci_ctrl);
        return ret;
    }

    g_reserver_mem_msg_offset = DEVDRV_RESERVE_MEM_MSG_OFFSET;

    pci_ctrl->mem_bar_id = PCI_BAR_MEM;
    pci_ctrl->mem_phy_base = (phys_addr_t)pci_resource_start(pci_ctrl->pdev, PCI_BAR_MEM);
    pci_ctrl->mem_phy_size = (u64)pci_resource_len(pci_ctrl->pdev, PCI_BAR_MEM);

    offset = pci_resource_start(pci_ctrl->pdev, PCI_BAR_RSV_MEM) + g_reserver_mem_msg_offset;
    size = DEVDRV_RESERVE_MEM_MSG_SIZE;
    pci_ctrl->mem_base = ioremap(offset, size);
    if (pci_ctrl->mem_base == NULL) {
        devdrv_err("Ioremap mem_base failed. (size=%lu)\n", size);
        devdrv_res_uninit(pci_ctrl);
        return -ENOMEM;
    }

    pci_ctrl->rsv_mem_phy_base = (phys_addr_t)pci_resource_start(pci_ctrl->pdev, PCI_BAR_RSV_MEM);
    pci_ctrl->rsv_mem_phy_size = (u64)pci_resource_len(pci_ctrl->pdev, PCI_BAR_RSV_MEM);

    offset = pci_resource_start(pci_ctrl->pdev, PCI_BAR_IO) + DEVDRV_SOC_DOORBELL_OFFSET + DEVDRV_IEP_SDI0_DB_OFFSET;
    size = DEVDRV_DB_IOMAP_SIZE;
    pci_ctrl->msi_base = ioremap(offset, size);
    if (pci_ctrl->msi_base == NULL) {
        devdrv_err("Ioremap msi_base failed. (size=%lu)\n", size);
        devdrv_res_uninit(pci_ctrl);
        return -ENOMEM;
    }

    pci_ctrl->shr_para = pci_ctrl->io_base + DEVDRV_IO_LOAD_SRAM_OFFSET + DEVDRV_SHR_PARA_ADDR_OFFSET;
    /* bios will use phy_match_flag to safety check when upgrade firmware */
    pci_ctrl->shr_para->phy_match_flag = DEVDRV_HOST_PHY_MACH_FLAG;

    pci_ctrl->res.phy_match_flag_addr = (u8 *)pci_ctrl->shr_para + PHY_MATCH_FLAG_OFFSET_IN_SHR_MEM;
    pci_ctrl->res.nvme_db_base = pci_ctrl->msi_base;

    pci_ctrl->res.nvme_pf_ctrl_base = NULL;
    pci_ctrl->res.load_sram_base = pci_ctrl->io_base + DEVDRV_IO_LOAD_SRAM_OFFSET;

    return 0;
}

STATIC void devdrv_cloud_v4_vf_boot_mode_rebuild(struct devdrv_pci_ctrl *pci_ctrl)
{
    if (pci_ctrl->pdev->revision == DEVDRV_REVISION_TYPE_MDEV_SRIOV_VF) {
        if ((pci_ctrl->shr_para->dma_bitmap & HOST_VF_DMA_MASK) == HOST_VF_DMA_MASK) {
            /* vf full spec mdev's vm */
            pci_ctrl->env_boot_mode = DEVDRV_MDEV_FULL_SPEC_VF_VM_BOOT;
        } else {
            /* vf not full spec mdev's vm */
            pci_ctrl->env_boot_mode = DEVDRV_MDEV_VF_VM_BOOT;
        }
    } else {
        if (((pci_ctrl->shr_para->dma_bitmap & HOST_VF_DMA_MASK) == HOST_VF_DMA_MASK) &&
            (pci_ctrl->env_boot_mode == DEVDRV_MDEV_VF_PM_BOOT)) {
            /* vf full spec mdev's phy */
            pci_ctrl->env_boot_mode = DEVDRV_MDEV_FULL_SPEC_VF_PM_BOOT;
        }
    }
}

STATIC int devdrv_cloud_v4_init_vf_bar_info(struct devdrv_pci_ctrl *pci_ctrl)
{
    resource_size_t offset;
    unsigned long size;
    unsigned long rsv_vpc_mem_offset = 0;

    pci_ctrl->mem_bar_id = PCI_BAR_MEM;

    pci_ctrl->mem_phy_base = (phys_addr_t)pci_resource_start(pci_ctrl->pdev, PCI_BAR_MEM);
    pci_ctrl->mem_phy_size = (u64)pci_resource_len(pci_ctrl->pdev, PCI_BAR_MEM);

    if (pci_ctrl->env_boot_mode == DEVDRV_MDEV_VF_VM_BOOT) {
        rsv_vpc_mem_offset = DEVDRV_VF_MDEV_RESERVE_MEM_SIZE;
    }

    offset = pci_resource_start(pci_ctrl->pdev, PCI_BAR_RSV_MEM) + DEVDRV_VF_MEM_MSG_OFFSET + rsv_vpc_mem_offset;
    size = DEVDRV_VF_MEM_MSG_SIZE;

    pci_ctrl->mem_base = ioremap(offset, size);
    if (pci_ctrl->mem_base == NULL) {
        devdrv_err("Ioremap mem_base failed. (size=%lu)\n", size);
        devdrv_res_uninit(pci_ctrl);
        return -ENOMEM;
    }

    if (pci_ctrl->env_boot_mode == DEVDRV_MDEV_VF_VM_BOOT) {
        pci_ctrl->mdev_rsv_mem_phy_base = (phys_addr_t)pci_resource_start(pci_ctrl->pdev, PCI_BAR_RSV_MEM) ;
        pci_ctrl->mdev_rsv_mem_phy_size = DEVDRV_VF_MDEV_RESERVE_MEM_SIZE;
    }

    pci_ctrl->rsv_mem_phy_base = (phys_addr_t)pci_resource_start(pci_ctrl->pdev, PCI_BAR_RSV_MEM) + rsv_vpc_mem_offset;
    pci_ctrl->rsv_mem_phy_size = (u64)pci_resource_len(pci_ctrl->pdev, PCI_BAR_RSV_MEM) - rsv_vpc_mem_offset;

    offset = pci_resource_start(pci_ctrl->pdev, PCI_BAR_IO) + DEVDRV_VF_IO_SOC_DOORBELL_OFFSET;
    size = DEVDRV_VF_IO_SOC_DOORBELL_SIZE;
    pci_ctrl->msi_base = ioremap(offset, size);
    if (pci_ctrl->msi_base == NULL) {
        devdrv_err("Ioremap msi_base failed. (size=%lu)\n", size);
        devdrv_res_uninit(pci_ctrl);
        return -ENOMEM;
    }

    pci_ctrl->io_phy_base = (phys_addr_t)pci_resource_start(pci_ctrl->pdev, PCI_BAR_IO);
    pci_ctrl->io_phy_size = (u64)pci_resource_len(pci_ctrl->pdev, PCI_BAR_IO);

    pci_ctrl->io_base = ioremap(pci_ctrl->io_phy_base, pci_ctrl->io_phy_size);
    if (pci_ctrl->io_base == NULL) {
        devdrv_err("Ioremap io_base failed. (size=%llx)\n", pci_ctrl->io_phy_size);
        devdrv_res_uninit(pci_ctrl);
        return -ENOMEM;
    }

    pci_ctrl->shr_para = pci_ctrl->io_base + DEVDRV_VF_IO_LOAD_SRAM_OFFSET + DEVDRV_SHR_PARA_ADDR_OFFSET;
    pci_ctrl->res.phy_match_flag_addr = (u8 *)pci_ctrl->shr_para + PHY_MATCH_FLAG_OFFSET_IN_SHR_MEM;
    pci_ctrl->res.nvme_db_base = pci_ctrl->msi_base;
    pci_ctrl->res.nvme_pf_ctrl_base = NULL;
    pci_ctrl->res.load_sram_base = pci_ctrl->io_base + DEVDRV_IO_LOAD_SRAM_OFFSET;
    pci_ctrl->func_id = pci_ctrl->shr_para->vf_id;

    return 0;
}
#endif

int devdrv_cloud_v4_res_init(struct devdrv_pci_ctrl *pci_ctrl)
{
#ifndef DRV_UT
    int ret;

    if (pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_VF) {
        ret = devdrv_cloud_v4_init_vf_bar_info(pci_ctrl);
        if (ret != 0) {
            devdrv_err("Init vf bar info\n");
            return ret;
        }

        devdrv_cloud_v4_vf_boot_mode_rebuild(pci_ctrl);
        devdrv_cloud_v4_init_vf_bar_addr_info(pci_ctrl);
        devdrv_cloud_v4_init_vf_intr_info(pci_ctrl);
        devdrv_cloud_v4_init_vf_msg_cnt(pci_ctrl);
        pci_ctrl->remote_dev_id = (u32)pci_ctrl->shr_para->vf_id;
        pci_ctrl->os_load_flag = 0;
    } else {
        ret = devdrv_cloud_v4_init_pf_bar_info(pci_ctrl);
        if (ret != 0) {
            devdrv_err("Init pf bar info\n");
            return ret;
        }
        devdrv_cloud_v4_init_pf_bar_addr_info(pci_ctrl);
        devdrv_cloud_v4_init_pf_intr_info(pci_ctrl);
        devdrv_cloud_v4_init_pf_msg_cnt(pci_ctrl);
        pci_ctrl->os_load_flag = (u32)pci_ctrl->shr_para->load_flag;
        pci_ctrl->remote_dev_id = pci_ctrl->os_load_flag == 1 ? 0 : 1;
        devdrv_cloud_v4_init_pf_dma_info(pci_ctrl);
    }
    devdrv_cloud_v4_init_load_file_info(pci_ctrl);
    devdrv_cloud_v4_ops_init(pci_ctrl);
#endif

    return 0;
}