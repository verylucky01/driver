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

#ifndef __DEVDRV_ADMIN_MSG_H_
#define __DEVDRV_ADMIN_MSG_H_

#include "comm_kernel_interface.h"
#include "comm_cmd_msg.h"

#if defined(CFG_PLATFORM_ESL) || defined(CFG_PLATFORM_FPGA)
#define DEVDRV_P2P_SUPPORT_MAX_DEVNUM 4U
#define DEVDRV_H2D_SUPPORT_MAX_DEVNUM 4U
#else
#ifdef CFG_FEATURE_P2P_EXTEND
#define DEVDRV_P2P_SUPPORT_MAX_DEVNUM 64U
#define DEVDRV_H2D_SUPPORT_MAX_DEVNUM 64U
#else
#define DEVDRV_P2P_SUPPORT_MAX_DEVNUM 16U
#define DEVDRV_H2D_SUPPORT_MAX_DEVNUM 16U
#endif
#endif

enum msg_queue_type {
    TRANSPARENT_MSG_QUEUE = 0,
    NON_TRANSPARENT_MSG_QUEUE,
    MSG_QUEUE_TYPE_MAX
};

#define DEVDRV_MSG_QUEUE_MEM_BASE 0x40000 /* 256KB */
#define DEVDRV_MSG_QUEUE_MEM_ALIGN 0x400U /* 1KB */

#if defined(CFG_PLATFORM_ESL) || defined(CFG_PLATFORM_FPGA)
#define DEVDRV_MAX_MSG_TIMEOUT 200000000   /* 200s for fpga  */
#define DEVDRV_S2S_MSG_TMOUT       200000000 /* 200s for fpga  */
#define DEVDRV_S2S_MSG_TMOUT_SHORT 20000000  /* 20s for fpga */
#define DEVDRV_S2S_MSG_TIMEOUT 200000000   /* 200s */
#define DEVDRV_MSG_TIMEOUT_LOG 30000000    /* 30s */
#define DEVDRV_MSG_IRQ_TIMEOUT 50000000    /* 50s */
#define DEVDRV_MSG_IRQ_TIMEOUT_LOG 30000000 /* 30s */
#define DEVDRV_MSG_SCHED_STATUS_CHECK_TIME 10 /* 10s for fpga */
#else
#ifdef CFG_BUILD_ASAN
#define DEVDRV_MAX_MSG_TIMEOUT 60000000    /* 60s for asan */
#define DEVDRV_S2S_MSG_TMOUT       100000000 /* 100s for asan */
#define DEVDRV_S2S_MSG_TIMEOUT 100000000  /* 100s */
#define DEVDRV_S2S_MSG_TMOUT_SHORT 3000000  /* 3s for asan */
#else
#define DEVDRV_MAX_MSG_TIMEOUT 35000000   /* 35s */
#define DEVDRV_S2S_MSG_TMOUT       30000000 /* 30s */
#define DEVDRV_S2S_MSG_TMOUT_SHORT 1000000  /* 1s */
#define DEVDRV_S2S_MSG_TIMEOUT 30000000   /* 30s */
#endif

#define DEVDRV_MSG_TIMEOUT_LOG 1000000    /* 1s */
#define DEVDRV_MSG_IRQ_TIMEOUT 1000000    /* 1s */
#define DEVDRV_MSG_IRQ_TIMEOUT_LOG 100000 /* 100ms */
#define DEVDRV_MSG_SCHED_STATUS_CHECK_TIME 3
#endif
#define DEVDRV_MSG_WAIT_DMA_FINISH_TIMEOUT 100 /* 100ms */
#define DEVDRV_MSG_TIME_VOERFLOW 5000          /* 5s */

#define DEVDRV_NON_TRANS_CB_TIME 1000          /* 1s */

#define DEVDRV_DMA_READ_RESPONSE_ERROR 0x20
#define DEVDRV_DMA_WRITE_RESPONSE_ERROR 0x40
#define DEVDRV_DMA_DATA_POISON_RECEIVED 0x80
#define DEVDRV_DMA_COPY_RETRY_DELAY 3000000          /* 3s */
#define DEVDRV_S2S_MSG_RETRY_LIMIT 3

/*
 * pcie use first 1MB DDR space
 * base       length        use
 * 0          1k            reserved
 * 1k         1k            share_param
 * 2k         64k           resved space for 310P, MUST only used by CPU
 * 2k         16k           p2p msg
 * 18k-120k                 not used
 * 120k       4k            for tsdrv use
 * 124k       4k            for devmng use
 * 128k       1M-128k-4k    pcie msg chan
 * 1M-4k      4k            test
 */
#define DEVDRV_SHR_PARA_ADDR_OFFSET 0x400
#ifdef CFG_FEATURE_P2P_EXTEND
#define DEVDRV_SHR_PARA_ADDR_SIZE 0x800
#else
#define DEVDRV_SHR_PARA_ADDR_SIZE 0x400
#endif
#define DEVDRV_SHR_MEM_CACHE 1
#define DEVDRV_SHR_MEM_NORMAL 0

/*
 * in VM scene, PHY_MACH_FLAG in share_param is masked 4KB aligned by QEMU,
 * and will make 4KB un-access by DMA for smmu fault. P2P msg use DMA, must skip this 4KB
 */
#ifdef CFG_FEATURE_PHY_MACH_FLAG_SKIP_PAGE
#define DEVDRV_PHY_MACH_FLAG_64K_RESVED  0x10000
#else
#define DEVDRV_PHY_MACH_FLAG_64K_RESVED 0
#endif
#define DEVDRV_P2P_MSG_SIZE 0x400 /* msg 1KB */
#define DEVDRV_P2P_SEND_MSG_ADDR_OFFSET 0
#define DEVDRV_P2P_SEND_MSG_ADDR_SIZE (DEVDRV_P2P_MSG_SIZE * DEVDRV_P2P_SUPPORT_MAX_DEVNUM)
#define DEVDRV_P2P_RECV_MSG_ADDR_OFFSET (DEVDRV_P2P_SEND_MSG_ADDR_OFFSET + DEVDRV_P2P_SEND_MSG_ADDR_SIZE)
#define DEVDRV_P2P_RECV_MSG_ADDR_SIZE (DEVDRV_P2P_MSG_SIZE * DEVDRV_P2P_SUPPORT_MAX_DEVNUM)
#define DEVDRV_P2P_MSG_ADDR_OFFSET (DEVDRV_SHR_PARA_ADDR_OFFSET + DEVDRV_SHR_PARA_ADDR_SIZE + \
    DEVDRV_PHY_MACH_FLAG_64K_RESVED)
#define DEVDRV_P2P_MSG_ADDR_TOTAL_SIZE (DEVDRV_P2P_SEND_MSG_ADDR_SIZE + DEVDRV_P2P_RECV_MSG_ADDR_SIZE)

#define DEVDRV_PCIE_RESV_SPACE_NOT_USED_BASE 0x3B000ULL
#define DEVDRV_RESERVE_TSDRV_RESV_OFFSET DEVDRV_PCIE_RESV_SPACE_NOT_USED_BASE
#define DEVDRV_RESERVE_TSDRV_RESV_SIZE 0x1000ULL
#define DEVDRV_RESERVE_DEVMNG_RESV_OFFSET (DEVDRV_RESERVE_TSDRV_RESV_OFFSET + DEVDRV_RESERVE_TSDRV_RESV_SIZE)
#define DEVDRV_RESERVE_DEVMNG_RESV_SIZE 0x1000ULL
#define DEVDRV_RESERVE_HBM_ECC_OFFSET (DEVDRV_RESERVE_DEVMNG_RESV_OFFSET + DEVDRV_RESERVE_DEVMNG_RESV_SIZE)
#define DEVDRV_RESERVE_HBM_ECC_SIZE 0x3000
#define DEVDRV_VF_BANDWIDTH_OFFSET 0x400 /* TS SRAM 1k~3k resv for bandwidth ctrl */
#define DEVDRV_VF_BANDWIDTH_SIZE 0x800 /* 2K */

/* cmd opreation code, first 8 bits is mudule name, later 8 bits is op type */
enum devdrv_admin_msg_opcode {
    /* msg chan */
    DEVDRV_CREATE_MSG_QUEUE,
    DEVDRV_FREE_MSG_QUEUE,
    DEVDRV_NOTIFY_DMA_ERR_IRQ,
    DEVDRV_NOTIFY_DEV_ONLINE,
    DEVDRV_CFG_P2P_MSG_CHAN,
    DEVDRV_CFG_TX_ATU,
    DEVDRV_GET_RX_ATU,
    DEVDRV_DMA_CHAN_REMOTE_OP,
    DEVDRV_HCCS_HOST_DMA_ADDR_MAP,
    DEVDRV_HCCS_HOST_DMA_ADDR_UNMAP,
    DEVDRV_SRIOV_EVENT_NOTIFY,
    DEVDRV_GET_EP_SUSPEND_STATUS,
    DEVDRV_HCCS_LINK_CHECK,
#ifdef CFG_FEATURE_PCIE_PROTO_DIP
    DEVDRV_SYNC_SOC_RES,
#endif
    DEVDRV_ADMIN_MSG_MAX
};

#define DEVDRV_MSG_CMD_BEGIN             1
#define DEVDRV_MSG_CMD_FINISH_SUCCESS    2
#define DEVDRV_MSG_CMD_FINISH_FAILED     3
#define DEVDRV_MSG_CMD_INVALID_PARA      4
#define DEVDRV_MSG_CMD_NULL_PROCESS_CB   5
#define DEVDRV_MSG_CMD_SEND_HOST_FAILED  6
#define DEVDRV_MSG_CMD_CB_PROCESS_FAILED 7
#define DEVDRV_MSG_CMD_COPY_FAILED       8
#define DEVDRV_MSG_CMD_IRQ_BEGIN  0x69727173U   /* irq handle begin, schedule work */
#define DEVDRV_MSG_CMD_IRQ_FINISH 0x8F7D6C5BU   /* irq notify success, return */
#define DEVDRV_MSG_CMD_IRQ_BUSY   0x1A3B7C9DU   /* irq notify busy, return for host retry */

#define DEVDRV_MSG_HANDLE_STATE_INIT 1
#define DEVDRV_MSG_HANDLE_STATE_SCHEDING 2

#define DEVDRV_OP_ADD 1
#define DEVDRV_OP_DEL 2

#define DEVDRV_MSIX_READY_FLAG 0x72636F6B

#define AGENTDRV_S2S_NON_TRANS_MSG_CHAN_NUM 8 /* host/device must be the same */

#define AGENTDRV_S2S_MAX_SERVER_NUM 48
#define AGENTDRV_S2S_MAX_CHIP_NUM 8
#define AGENTDRV_S2S_MAX_DEV_NUM 2
#define AGENTDRV_S2S_MAX_UDEVID_NUM 16
#define AGENTDRV_S2S_SUPPORT_MAX_CHAN_NUM_DEVICE (AGENTDRV_S2S_MAX_DEV_NUM * AGENTDRV_S2S_MAX_CHIP_NUM * \
    AGENTDRV_S2S_MAX_SERVER_NUM)
#define AGENTDRV_S2S_SUPPORT_MAX_CHAN_NUM_HOST (AGENTDRV_S2S_MAX_DEV_NUM * AGENTDRV_S2S_MAX_CHIP_NUM * \
    AGENTDRV_S2S_MAX_SERVER_NUM * 2)
#define AGENTDRV_S2S_SUPPORT_MAX_CHAN_NUM (AGENTDRV_S2S_SUPPORT_MAX_CHAN_NUM_DEVICE + \
    AGENTDRV_S2S_SUPPORT_MAX_CHAN_NUM_HOST)

struct devdrv_general_interrupt_db_info {
    u32 db_start;
    u32 db_num;
};

struct devdrv_ep_suspend_status {
    u32 status;
    u32 reserved;
};

struct devdrv_ep_suspend_cmd {
    u32 hand_shake_flag;
};

struct devdrv_alloc_msg_chan_reply {
    dma_addr_t sq_rsv_dma_addr_d;
    dma_addr_t cq_rsv_dma_addr_d;
};

#define DMA_CHAN_REMOTE_OP_RESET 0
#define DMA_CHAN_REMOTE_OP_INIT 1
#define DMA_CHAN_REMOTE_OP_ERR_PROC 2

struct devdrv_hccs_host_dma_addr {
    u64 phy_addr;
    u64 dma_addr;
    u64 size;
};

/* DMA single node read and write command */
struct devdrv_dma_single_node_command {
    struct devdrv_dma_node dma_node;
};

/* DMA chained read and write command */
struct devdrv_dma_chain_command {
    u64 dma_node_base;
    u32 node_cnt;
};

struct devdrv_admin_msg_reply {
    u32 len;  // contain 'len' own occupied space
    char data[];
};

#define DEVDRV_SRIOV_ENABLE 1
#define DEVDRV_SRIOV_DISABLE 0

/* The command channel uses a memory synchronous call */
#define DEVDRV_ADMIN_MSG_QUEUE_DEPTH 1
#define DEVDRV_ADMIN_MSG_QUEUE_BD_SIZE 0x400

#define DEVDRV_ADMIN_MSG_HEAD_LEN sizeof(struct devdrv_admin_msg_command)
#define DEVDRV_ADMIN_MSG_DATA_LEN (DEVDRV_ADMIN_MSG_QUEUE_BD_SIZE - DEVDRV_ADMIN_MSG_HEAD_LEN)

#define DEVDRV_NON_TRANS_MSG_HEAD_LEN sizeof(struct devdrv_non_trans_msg_desc)

#define DEVDRV_SHR_PARA_PRE_CFG_LEN 8
#define PHY_MATCH_FLAG_OFFSET_IN_SHR_MEM 0x30
#define DEVDRV_SHR_PARA_RESV 11
struct devdrv_shr_para {
    int load_flag;      /* D2H: device bios notice host to load device os via pcie. 0: no, 1 yes */
    int chip_id;        /* D2H: device bios notice host: cloud ai server, index in one node(4P 0-3); others 0 */
    int node_id;        /* D2H: device bios notice host: cloud ai server has total 8P, one node has 4p, which node */
    int slot_id;        /* D2H: device bios notice host: slot_id (0-7) */
    int board_type;     /* D2H: device bios notice host: cloud pcie card, ai server, evb */
    int chip_type;      /* D2H: mini cloud */
    int driver_version; /* H2D: host notice device bios: driver version */
    int dev_num;        /* D2H: device bios notice host: only cloud v2 set, other chip device driver set */
    int hot_reset_flag; /* CCPU set, bios read */
    int hot_reset_pcie_flag; /* H2D: make sure pcie report hotreset */
    int total_func_num; /* D2H: device driver set */
    /*
     * D2H: device bios capability flag: bit0: imu export reg, other reserved,
     * make sure phy_match_flag offset=0x30
     */
    u32 capability;
    u32 phy_match_flag;      /* HOST set, phy machine flag */
    /* H2D: bios set actual hccs link status, bit[x]=1 means the hccs between this chip and chip[x] is linked
     * D2H: bios set hccs link status, bit[x]=1 means the hccs between this chip and chip[x] is linked
     */
    u32 hccs_status;
    /* H2D: bios set expected hccs grouping of the whole system
     * D2H: bios set expected hccs grouping of the whole system
     */
    u32 hccs_group_id[HCCS_GROUP_SUPPORT_MAX_CHIPNUM];
    int host_dev_id;         /* H2D: the dev id in host side */
    int host_interrupt_flag; /* H2D: host notice device has receive interrupt begin half probe. 0: no, 1 yes */
    u32 ep_pf_index;         /* H2D: EP PF INDEX */
    int connect_protocol;    /* H2D: pcie, hccs */
    u32 admin_msg_status;    /* host set begin, device irq set irq_begin, host continue */
    u64 admin_chan_sq_base;  /* H2D: cloud v2(hccs), host notice phy addr; others, host notice dma addr */
    u64 host_mem_bar_base;   /* H2D: mem bar 4 base addr */
    u64 host_io_bar_base;    /* H2D: io bar 2 base addr */
    u64 tx_atu_base_addr1;   /* D2H: cloud only, high 256GB address space */
    u64 tx_atu_base_size1;   /* D2H: cloud only, high 256GB space size */
    u64 tx_atu_base_addr2;   /* D2H: cloud only, low 128MB address space */
    u64 tx_atu_base_size2;   /* D2H: cloud only, low 128MB space size */
    u32 pre_cfg[DEVDRV_SHR_PARA_PRE_CFG_LEN];          /* H2D: resv 8 */
    u64 resv[DEVDRV_SHR_PARA_RESV]; /* Keep offset constant and reserve for new flags */
    struct udevid_reorder_para reorder_para; /* GUID info for cloud v4 pcie card */
    volatile u64 host_heartbeat_count; /* devmng set in host, get in device */
    volatile u64 heartbeat_count; /* devmng set in device, get in host */
    volatile u64 runtime_runningplat; /* runtime running in device(0xdece) or host(0xa5a5), runtime stop:0xABCD */
    u32 msix_offset;         /* H2D: 1pf2p drv notice msix offset of p1 to device drv and ts */
    u32 vf_id;               /* D2H: device driver set */
    u64 sq_desc_dma;         /* H2D: host set, mdev pm dma desc addr */
    u64 dma_bitmap;          /* D2H: dma channel bitmap for vf, 0-irrelevant, 1-allocated */
    u32 rc_msix_ready_flag;  /* H2D: rc has registered MSI interrupt */
    u32 sid;                 /* H2D: host pdev's sid, agent smmu use it for iova to pa */
    u32 bar_wc_flag;         /* OS set, bit0~5:bar0~bar5, 1 is wc attribute, 0 is not attribute */
    int addr_mode;           /* HOST set, only vf user */
    u64 p2p_msg_base_addr[DEVDRV_P2P_SUPPORT_MAX_DEVNUM]; /* H2D: cloud only, p2p msg base for dma */
    u64 p2p_db_base_addr[DEVDRV_P2P_SUPPORT_MAX_DEVNUM];  /* H2D: cloud only, p2p doorbell base for dma */
    u32 vm_full_spec_flag;   /* D2H: mdev only, is 1:1 vf mdev */
};

#endif
