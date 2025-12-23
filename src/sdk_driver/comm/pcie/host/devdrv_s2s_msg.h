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

#ifndef __DEVDRV_S2S_MSG_H
#define __DEVDRV_S2S_MSG_H

#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/mutex.h>
#include <linux/jiffies.h>
#include <linux/semaphore.h>

#include "devdrv_dma.h"
#include "devdrv_pci.h"
#include "devdrv_common_msg.h"
#include "comm_kernel_interface.h"
#include "devdrv_msg_def.h"
#include "res_drv_cloud_v2.h"

#define DEVDRV_S2S_MAX_SERVER_NUM 48
#define DEVDRV_S2S_DIE_NUM 2
#define DEVDRV_S2S_SUPPORT_MAX_CHAN_NUM_DEV (DEVDRV_S2S_MAX_SERVER_NUM * DEVDRV_S2S_MAX_CHIP_NUM * DEVDRV_S2S_DIE_NUM)
#define DEVDRV_S2S_SUPPORT_MAX_CHAN_NUM_HOST \
    (DEVDRV_S2S_MAX_SERVER_NUM * DEVDRV_S2S_MAX_CHIP_NUM * DEVDRV_S2S_DIE_NUM * 2)
#define DEVDRV_S2S_SUPPORT_MAX_CHAN_NUM (DEVDRV_S2S_SUPPORT_MAX_CHAN_NUM_DEV + DEVDRV_S2S_SUPPORT_MAX_CHAN_NUM_HOST)
#define DEVDRV_S2S_MAX_DEV_NUM (DEVDRV_S2S_MAX_CHIP_NUM * DEVDRV_S2S_DIE_NUM)

#define DEVDRV_S2S_HOST_MSG_SIZE           0x800 /* 2K */
#define DEVDRV_S2S_DEVICE_MSG_SIZE         0x1000 /* 4K */
#define DEVDRV_S2S_NON_TRANS_MSG_DESC_SIZE 0x1000 /* 4K */

#define DEVDRV_S2S_SEND_MSG_ADDR_OFFSET 0 /* send queue offset, which is zero */
#define DEVDRV_S2S_SEND_MSG_ADDR_SIZE (DEVDRV_S2S_HOST_MSG_SIZE * DEVDRV_S2S_SUPPORT_MAX_CHAN_NUM_HOST + \
    DEVDRV_S2S_DEVICE_MSG_SIZE * DEVDRV_S2S_SUPPORT_MAX_CHAN_NUM_DEV)
#define DEVDRV_S2S_RECV_MSG_ADDR_OFFSET DEVDRV_S2S_SEND_MSG_ADDR_SIZE /* recv queue is next to send queue */
#define DEVDRV_S2S_RECV_MSG_ADDR_SIZE DEVDRV_S2S_SEND_MSG_ADDR_SIZE
#define DEVDRV_S2S_MSG_ADDR_TOTAL_SIZE (DEVDRV_S2S_SEND_MSG_ADDR_SIZE + DEVDRV_S2S_RECV_MSG_ADDR_SIZE)

#define DEVDRV_S2S_DB_NUM_FOR_DEV (DEVDRV_S2S_MAX_CHIP_NUM * DEVDRV_S2S_MAX_SERVER_NUM)

/* use for calculate remote server addr */
#define DEVDRV_S2S_DEV_MEM_SIZE 0x1000000000 /* 64G */
#define DEVDRV_S2S_CHIP_MEM_SIZE (DEVDRV_S2S_DEV_MEM_SIZE * DEVDRV_S2S_DIE_NUM)
#define DEVDRV_S2S_SERVER_MEM_SIZE 0x10000000000 /* 1T */

/* use for calculate local server addr */
#define DEVDRV_S2S_DEV_MEM_SIZE_LOCAL 0x10000000000 /* 1T */
#define DEVDRV_S2S_CHIP_MEM_SIZE_LOCAL (DEVDRV_S2S_DEV_MEM_SIZE_LOCAL * DEVDRV_S2S_DIE_NUM)

#define DOORBELL_REG_BITS 32
#define PCIE_H2H_DOORBELL_MAX 512
#define PCIE_DOORBELL_SLAVE_BASE_ADDR 0x4001e0000ULL
#define PCIE_DOORBELL_SLAVE_VF_REG_OFFSET 0x1000
#define PCIE_DOORBELL_SLAVE_VF_BIT_OFFSET 0x4
#define PCIE_DOORBELL_SLAVE_VF_ADDR_START 0x2000

#define DEVDRV_LOCAL_SUPER_NODE_START_ADDR 0x201054000000
#define DEVDRV_S2S_SERVER_ADDR_BASE 0x300000000000
#define DEVDRV_S2S_SERVER_ADDR_OFFSET 0x010000000000  /* 1T */
#define DEVDRV_S2S_CHIP_ADDR_OFFSET 0x02000000000     /* 128G */
#define DEVDRV_S2S_DIE_ADDR_OFFSET 0x01000000000      /* 64G */

#define DEVDRV_DB_ADD_OFFSET_ADDR 0x1000000
#define DEVDRV_SERVER_DB_BASE_ADDR 0x400100000

#define DEVDRV_S2S_HOST_CHAN_EACH 2  /* 2 channel for each die */

#define DEVDRV_S2S_NORMAL    0
#define DEVDRV_S2S_PRE_RESET 1
#define DEVDRV_S2S_ABORT     2

#define DEVDRV_S2S_CB_TIME 1000 /* ms */

#define DEVDRV_S2S_MSG_VERSION_0 0x5A5D3C21 /* 0x5A5D3 is magic, C21 is version */
#define DEVDRV_S2S_MSG_VERSION_1 0x5A5D3C23 /* 0x5A5D3 is magic, C23 is version */

#define DEVDRV_S2S_MSG_HEAD_LEN sizeof(struct devdrv_s2s_msg)

struct devdrv_msg_s2s_queue_info {
    struct devdrv_s2s_msg *desc;        // map from union address each host for 16 devices, used for cq
    struct devdrv_s2s_msg *data_buf;    // used for service data transfer
    dma_addr_t data_buf_addr;
    struct devdrv_s2s_msg *ack_buf;
    dma_addr_t ack_addr;
};

struct devdrv_s2s_msg_chan {
    u32 valid;
    u32 devid;
    u32 chan_id;
    u32 status;
    u32 version; /* must be here */
    struct devdrv_msg_dev *msg_dev;
    struct device *dev;
    struct devdrv_msg_s2s_queue_info sq;
    phys_addr_t sq_phy_addr;
    phys_addr_t remote_sq_msg_phy_addr;
    phys_addr_t remote_cq_msg_phy_addr;
    struct semaphore sema;
    u32 msg_mode;
    u64 tx_seq_num;
};

struct devdrv_s2s_non_trans_ctrl {
    u32 last_use;
    struct mutex mutex;
    void *chan[DEVDRV_S2S_NON_TRANS_MSG_CHAN_NUM];
};

struct devdrv_s2s_msg_send_data_para {
    void *data;
    u32 data_len;
    u32 in_len;
    u32 out_len;
    u32 direction;
    u32 msg_mode;
};

enum devdrv_s2s_non_trans_msg_type {
    DEVDRV_S2S_NON_TRANS_INIT_TYPE,
    DEVDRV_S2S_NON_TRANS_SDMA_TYPE,
    DEVDRV_S2S_NON_TRANS_TYPE_MAX
};

struct devdrv_s2s_init_msg {
    phys_addr_t host_sq_phy_addr;
    u32 chan_id;
};

struct devdrv_s2s_sdma_msg {
    u32 in_len;
    u32 host_dev_id;
    u32 dst_sdid;
    u32 chan_id;
};

struct devdrv_s2s_non_trans_msg {
    u32 msg_type;
    int ret;
    union {
        struct devdrv_s2s_init_msg init_msg;
        struct devdrv_s2s_sdma_msg sdma_msg;
    } msg_data;
};

int devdrv_s2s_msg_chan_init(struct devdrv_pci_ctrl *pci_ctrl);
void devdrv_s2s_msg_chan_uninit(struct devdrv_pci_ctrl *pci_ctrl);
u64 devdrv_s2s_get_doobell_reg_addr_from_irqpool(u32 server_id, u32 chip_id, u32 die_id, u32 irq_idx);
extern int devdrv_s2s_non_trans_msg_recv(void *msg_chan, void *data, u32 in_data_len,
    u32 out_data_len, u32 *real_out_len);
void devdrv_s2s_rwsem_init(void);
void *devdrv_get_s2s_non_trans_chan(struct devdrv_msg_dev *msg_dev);
void devdrv_set_s2s_chan_pre_reset(u32 dev_id);
#endif
