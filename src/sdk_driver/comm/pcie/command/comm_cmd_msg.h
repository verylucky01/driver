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

#ifndef _COMM_CMD_MSG_H_
#define _COMM_CMD_MSG_H_

struct devdrv_create_queue_command {
    u32 msg_type;   /* enum devdrv_msg_client_type */
    u32 queue_type; /* enum msg_queue_type */
    u32 queue_id;
    u64 sq_dma_base_host;
    u64 cq_dma_base_host;
    u32 sq_desc_size;
    u32 cq_desc_size;
    u16 sq_depth;
    u16 cq_depth;
    u32 sq_slave_mem_offset;
    u32 cq_slave_mem_offset;
    s32 irq_tx_finish_notify;
    s32 irq_rx_msg_notify;
    u32 reserved[8];
};

struct devdrv_free_queue_cmd {
    u32 queue_id;
    u32 reserved[7];
};

struct devdrv_notify_dma_err_irq_cmd {
    u32 dma_chan_id;
    s32 err_irq;
    u32 reserved[6];
};

struct devdrv_notify_dev_online_cmd {
    u32 devid;
    u32 status;
    u32 reserved[6];
};

struct devdrv_p2p_msg_chan_cfg_cmd {
    u32 op;
    u32 devid;
    u32 reserved[6];
};

struct devdrv_tx_atu_cfg_cmd {
    u32 op;
    u32 devid;
    u32 atu_type;
    u64 phy_addr;
    u64 target_addr;
    u64 target_size;
    u64 atu_base_addr;
    u32 reserved[5];
};

struct devdrv_get_rx_atu_cmd {
    u32 devid;
    u32 bar_num;
    u32 reserved[6];
};

struct devdrv_dma_chan_remote_op {
    u32 op;
    u32 chan_id;
    u32 pf_num;
    u32 vf_num;
    u64 sq_desc_dma;
    u64 cq_desc_dma;
    u32 sq_depth;
    u32 cq_depth;
    u32 sqcq_side;
    u32 sriov_flag;
    u32 reserved[8];
};

struct devdrv_sriov_event_notify_cmd {
    u32 devid;
    u32 status;
    u32 reserved[6];
};

/* The maximum size for DMA small packet copy is 32 bytes */
struct devdrv_non_trans_msg_desc {
    u64 seq_num;      /* msg sequence number */
    u32 in_data_len;  /* input real length */
    u32 out_data_len; /* output max length */
    u32 real_out_len; /* output real length */
    u32 msg_type;     /* enum devdrv_common_msg_type */
    u32 status;       /* DEVDRV_MSG_CMD_* */
    u32 reserved;
    char data[];
};

/* agent smmu only can transform 32 addr one time */
struct devdrv_host_dma_addr_to_pa_cmd {
    u32 sub_cmd;
    u32 host_devid;
    u32 cnt;
    u32 reserved[5];
    u64 dma_addr[];
};

struct devdrv_s2s_msg_head_info {
    u32 status;     /* DEVDRV_MSG_CMD_*, this element must be the first one in this struct */
    u32 msg_type;   /* enum agentdrv_s2s_msg_type */
    u32 buf_len;    /* input real length */
    u32 in_len;     /* output max length */
    u32 out_len;    /* output real length */
    u32 send_direction; /* recv side is host/device */
    u32 chan_id;
    u32 sdid;
    phys_addr_t cq_phy_addr;
    dma_addr_t cq_dma_addr;
    u32 version; /* must be here */
    u64 seq_num; /* msg sequence number */
}__attribute__((aligned(128)));

struct devdrv_pasid_msg {
    u64 hash_va;
    u32 op_code;    // operation for node, add or del
    u32 dev_id;     // devid which pasid belongs
    int error_code; // remote process result
}__attribute__((aligned(128)));

struct devdrv_s2s_msg {
    struct devdrv_s2s_msg_head_info head_info;
    char data[];
};

struct devdrv_admin_msg_command {
    u32 opcode;
    u32 status;
    u32 reserved[2];
    char data[];
};

#endif
