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

#ifndef __STARS_RTSQ_NS_REG_C_DEFINE_H__
#define __STARS_RTSQ_NS_REG_C_DEFINE_H__

/* The base address of the module stars_rtsq_ns_reg */

#define STARS_RTSQ_NS_REG_BASE_ADDR 0x4000  /* module base addr */

/*
 * DEFINE REGISTER UNION
 */
/* Define the union STARS_RTSQ_AXCACHE_SETTING_UNION */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved0               : 16;      /* [15:0] */
        /*
         * arcache for STARS reading static information of RTSQ;
         * refer to AXI 4.0 .
         */
        unsigned int arcache_swapbuf         : 4;       /* [19:16] */
        /* arsnoop for STARS reading static information of RTSQ; refer to AXI 4.0 . */
        unsigned int arsnoop_swapbuf         : 1;       /* [20] */
        unsigned int reserved1               : 11;      /* [31:21] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} STARS_RTSQ_AXCACHE_SETTING_UNION;

#define STARS_RTSQ_AXCACHE_SETTING                   (0x0840) // default val: 0x00120000

/* Define the union STARS_NS_SQ_SWAP_BUF_BASE_ADDR_CFG0_UNION */
typedef union {
    /* Define the struct bits */
    struct {
        /* Non-secure base address of swap buffer0 in system memory, 49 bit. Here is low 32 bits. */
        unsigned int ns_sq_swap_buf_base_addr_l : 32;      /* [31:0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} STARS_NS_SQ_SWAP_BUF_BASE_ADDR_CFG0_UNION;

#define STARS_NS_SQ_SWAP_BUF_BASE_ADDR_CFG0                   (0x0860) // default val: 0x00000000

/* Define the union STARS_NS_SQ_SWAP_BUF_BASE_ADDR_CFG1_UNION */
typedef union {
    /* Define the struct bits */
    struct {
        /* Non-secure swap buffer base address. Whole base address is 49 bits, here is high 17 bits; */
        unsigned int ns_sq_swap_buf_base_addr_h : 17;      /* [16:0] */
        /*
         * Address of individual swap buffer would be
         * STARS_ns_sq_swap_buf_base_addr + (swap_buffer_id << STARS_ns_sq_swap_buf_shift)。
         * The size of every swap buffer is 64byte, s_sq_swap_buf_shift cannot be configured to 0~5.
         */
        unsigned int ns_sq_swap_buf_shift    : 6;       /* [22:17] */
        unsigned int reserved                : 8;       /* [30:23] */
        /* Swap buffer base address is physical. Cannot be set to 1'b1.not used */
        unsigned int ns_sq_swap_buf_is_virtual : 1;       /* [31] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} STARS_NS_SQ_SWAP_BUF_BASE_ADDR_CFG1_UNION;

#define STARS_NS_SQ_SWAP_BUF_BASE_ADDR_CFG1                   (0x0864) // default val: 0x000C0000

/* Define the union STARS_RTSQ_FSM_SEL_UNION */
typedef union {
    /* Define the struct bits */
    struct {
        /* Rtsq id, software configured value, which used to observe rtsq fsm. */
        unsigned int dfx_rtsq_fsm_sel        : 9;       /* [8:0] */
        unsigned int reserved                : 21;      /* [29:9] */
        /*
         * Indicates whether to record the (dfx_rtsq_swapin_num, dfx_rtsq_swapin_time) selected by dfx_rtsq_fsm_sel.
         * The logic for selecting rtsq during exception handling is reused.
         */
        unsigned int dfx_rtsq_record_en      : 1;       /* [30] */
        /* Clear the RTSQ information. (dfx_rtsq_swapin_num, dfx_rtsq_swapin_time) */
        unsigned int dfx_rtsq_record_clr     : 1;       /* [31] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} STARS_RTSQ_FSM_SEL_UNION;

#define STARS_RTSQ_FSM_SEL                   (0x0880) // default val: 0x00000000

/* Define the union STARS_RTSQ_FSM_STATE_UNION */
typedef union {
    /* Define the struct bits */
    struct {
        /*
         * RTSQ fsm state, with the use of dfx_rtsq_fsm_sel.
         * Status codes are as follows:
         * 4'd0: idle
         * 4'd1:wait_SWAPIN waits for the swapin to arbitrate to an acsq.
         * 4'd2: Wait_READY Send ARVALID and wait for ARREADY.
         * 4'd3: Wait_RLAST waiting for the RlAST response.
         * 4'd4: The slot_ENABLE sends the enable signal and configuration information to the corresponding ACSQ to establish the mapping.
         * 4'd5: WORKING
         * 4'd6: Wait_SWAPOUT waits for the busy signal corresponding to the prefetch_buffer to pull down.
         * 4'd7: The slot_DISABLE is used to disassociate acsq.
         * 4'd8: SQCQ_ASYNC_REQ indicates that a software operation occurs and the software operation information is synchronized to the acsq.
         * 4'd9:Wait_FW_RSP waiting for software operation
         * 4'dA:Polling_FLAG rtsq waiting for task synchronization
         * 4'dB: Wait_ACSQ_IDLE Waits for the corresponding ACSQ to return idle.
         */
        unsigned int dfx_rtsq_fsm_state      : 4;       /* [3:0] */
        /*
         * Software operating status.
         * 1: rtsq is processing task resume, debug resume, task terminate, exception handled, prefetch clear.
         * 0: idle.
         */
        unsigned int dfx_rtsq_fwrsp_ost      : 1;       /* [4] */
        unsigned int reserved                : 27;      /* [31:5] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} STARS_RTSQ_FSM_STATE_UNION;

#define STARS_RTSQ_FSM_STATE                   (0x0884) // default val: 0x00000000

/* Define the union STARS_RTSQ_FSM_SWAPIN_STATE_UNION */
typedef union {
    /* Define the struct bits */
    struct {
        /* dfx_rtsq_fsm_sel Time when the selected RTSQ is in SWAPIN */
        unsigned int dfx_rtsq_swapin_time    : 20;      /* [19:0] */
        /* Times of dfx_rtsq_fsm_sel selected RTSQswapin */
        unsigned int dfx_rtsq_swapin_num     : 12;      /* [31:20] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} STARS_RTSQ_FSM_SWAPIN_STATE_UNION;

#define STARS_RTSQ_FSM_SWAPIN_STATE                   (0x0888) // default val: 0x00000000

/* Define the union STARS_SWAPIN_CTRL0_NS_UNION */
typedef union {
    /* Define the struct bits */
    struct {
        /* The max number of ACSQ can be used by ffts. */
        unsigned int rtsq_swapin_max_ffts_ns : 5;       /* [4:0] */
        unsigned int reserved0               : 3;       /* [7:5] */
        /* The max number of ACSQ can be used by pcie. */
        unsigned int rtsq_swapin_max_pciedma_ns : 5;       /* [12:8] */
        unsigned int reserved1               : 3;       /* [15:13] */
        /* The max number of ACSQ can be used by FFTS_AIV */
        unsigned int rtsq_swapin_max_ffts_aiv_only_ns : 5;       /* [20:16] */
        unsigned int reserved2               : 3;       /* [23:21] */
        /* The max number of ACSQ can be used by sdma. */
        unsigned int rtsq_swapin_max_sdma_ns : 5;       /* [28:24] */
        unsigned int reserved3               : 3;       /* [31:29] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} STARS_SWAPIN_CTRL0_NS_UNION;

#define STARS_SWAPIN_CTRL0_NS                   (0x0900) // default val: 0x06010801

/* Define the union STARS_SWAPIN_CTRL1_NS_UNION */
typedef union {
    /* Define the struct bits */
    struct {
        /* The max number of ACSQ can be used by vpc. */
        unsigned int rtsq_swapin_max_vpc_ns  : 5;       /* [4:0] */
        unsigned int reserved0               : 3;       /* [7:5] */
        /* The max number of ACSQ can be used by jpegd. */
        unsigned int rtsq_swapin_max_jpegd_ns : 5;       /* [12:8] */
        unsigned int reserved1               : 3;       /* [15:13] */
        /* The max number of ACSQ can be used by jpege. */
        unsigned int rtsq_swapin_max_jpege_ns : 5;       /* [20:16] */
        unsigned int reserved2               : 3;       /* [23:21] */
        /* The max number of ACSQ can be used by cmo. (ascend310b is used for controlling the acsq swapin of the CMO.) */
        unsigned int rtsq_swapin_max_dsa_ns  : 5;       /* [28:24] */
        unsigned int reserved3               : 3;       /* [31:29] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} STARS_SWAPIN_CTRL1_NS_UNION;

#define STARS_SWAPIN_CTRL1_NS                   (0x0904) // default val: 0x01010202

/* Define the union STARS_SWAPIN_CTRL2_NS_UNION */
typedef union {
    /* Define the struct bits */
    struct {
        /* The max number of ACSQ can be used by condition. */
        unsigned int rtsq_swapin_max_conds_ns : 5;       /* [4:0] */
        unsigned int reserved0               : 3;       /* [7:5] */
        /* The max number of ACSQ can be used by sblk topic task. */
        unsigned int rtsq_swapin_max_sblk_cpu_ns : 5;       /* [12:8] */
        unsigned int reserved1               : 3;       /* [15:13] */
        /* The max number of ACSQ can be used by mblk topic task. */
        unsigned int rtsq_swapin_max_mblk_cpu_ns : 5;       /* [20:16] */
        unsigned int reserved2               : 3;       /* [23:21] */
        /* The max number of ACSQ can be used by FFTS_AIC */
        unsigned int rtsq_swapin_max_ffts_aic_only_ns : 5;       /* [28:24] */
        unsigned int reserved3               : 3;       /* [31:29] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} STARS_SWAPIN_CTRL2_NS_UNION;

#define STARS_SWAPIN_CTRL2_NS                   (0x0908) // default val: 0x01040401

/* Define the union STARS_MODEL_WEIGHT_CTRL_UNION */
typedef union {
    /* Define the struct bits */
    struct {
        /* The weight of model used in WRR. */
        unsigned int model_weight            : 12;      /* [11:0] */
        unsigned int reserved                : 20;      /* [31:12] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} STARS_MODEL_WEIGHT_CTRL_UNION;

#define STARS_MODEL_WEIGHT_CTRL                   (0x0B00) // default val: 0x00000001

/* Define the union STARS_MODEL_FRIENDLY_UNION */
typedef union {
    /* Define the struct bits */
    struct {
        /*
         * model schedule method:
         * 1'b0:not model friendly, RR first;
         * 1'b1:model_friendly,weight count first;
         */
        unsigned int model_friendly          : 1;       /* [0] */
        /*
         * 1'b0:update all weight;
         * 1'b1:update count zero weight;
         */
        unsigned int upd_zero_weight         : 1;       /* [1] */
        unsigned int reserved                : 30;      /* [31:2] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} STARS_MODEL_FRIENDLY_UNION;

#define STARS_MODEL_FRIENDLY                   (0x1600) // default val: 0x00000000

/* Define the union STARS_WAIT_TIMEOUT_CTRL0_UNION */
typedef union {
    /* Define the struct bits */
    struct {
        /* the wait_notify/wait_event timeout limit */
        unsigned int wait_task_runtime_limit_l : 32;      /* [31:0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} STARS_WAIT_TIMEOUT_CTRL0_UNION;

#define STARS_WAIT_TIMEOUT_CTRL0                   (0x1680) // default val: 0x00100000

/* Define the union STARS_WAIT_TIMEOUT_CTRL1_UNION */
typedef union {
    /* Define the struct bits */
    struct {
        /* the wait_notify/wait_event timeout limit */
        unsigned int wait_task_runtime_limit_h : 32;      /* [31:0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} STARS_WAIT_TIMEOUT_CTRL1_UNION;

#define STARS_WAIT_TIMEOUT_CTRL1                   (0x1700) // default val: 0x00000000

/* Define the union STARS_RTSQ_HARDEN_ECO0_UNION */
typedef union {
    /* Define the struct bits */
    struct {
        /* reserved reg for eco,value 0 */
        unsigned int rtsq_rsv_eco0           : 32;      /* [31:0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} STARS_RTSQ_HARDEN_ECO0_UNION;

#define STARS_RTSQ_HARDEN_ECO0                   (0x1800) // default val: 0x00000000

/* Define the union STARS_RTSQ_HARDEN_ECO1_UNION */
typedef union {
    /* Define the struct bits */
    struct {
        /* reserved reg for eco,value 1 */
        unsigned int rtsq_rsv_eco1           : 32;      /* [31:0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} STARS_RTSQ_HARDEN_ECO1_UNION;

#define STARS_RTSQ_HARDEN_ECO1                   (0x1804) // default val: 0xFFFFFFFF

typedef struct {
    volatile STARS_SWAPIN_CTRL0_NS_UNION stars_swapin_ctrl0_ns;
    volatile STARS_SWAPIN_CTRL1_NS_UNION stars_swapin_ctrl1_ns;
    volatile STARS_SWAPIN_CTRL2_NS_UNION stars_swapin_ctrl2_ns;
    unsigned int                         reserved[9];
} REGS_STARS_SWAPIN_CTRL0_NS_TO_STARS_SWAPIN_CTRL2_NS;

/*
 * DEFINE GLOBAL STRUCT
 */
typedef struct {
    unsigned int                         rsr0[528];                           /* 0 ~ 83c */
    volatile STARS_RTSQ_AXCACHE_SETTING_UNION stars_rtsq_axcache_setting;          /* 840 */
    unsigned int                         rsr1[7];                             /* 844 ~ 85c */
    volatile STARS_NS_SQ_SWAP_BUF_BASE_ADDR_CFG0_UNION stars_ns_sq_swap_buf_base_addr_cfg0; /* 860 */
    volatile STARS_NS_SQ_SWAP_BUF_BASE_ADDR_CFG1_UNION stars_ns_sq_swap_buf_base_addr_cfg1; /* 864 */
    unsigned int                         rsr2[6];                             /* 868 ~ 87c */
    volatile STARS_RTSQ_FSM_SEL_UNION    stars_rtsq_fsm_sel;                  /* 880 */
    volatile STARS_RTSQ_FSM_STATE_UNION  stars_rtsq_fsm_state;                /* 884 */
    volatile STARS_RTSQ_FSM_SWAPIN_STATE_UNION stars_rtsq_fsm_swapin_state;         /* 888 */
    unsigned int                         rsr3[29];                            /* 88c ~ 8fc */
    volatile REGS_STARS_SWAPIN_CTRL0_NS_TO_STARS_SWAPIN_CTRL2_NS stars_swapin_ctrl0_ns[8];            /* 900 ~ a7c */
    unsigned int                         rsr4[32];                            /* a80 ~ afc */
    volatile STARS_MODEL_WEIGHT_CTRL_UNION stars_model_weight_ctrl[64];         /* b00 ~ bfc */
    unsigned int                         rsr5[640];                           /* c00 ~ 15fc */
    volatile STARS_MODEL_FRIENDLY_UNION  stars_model_friendly;                /* 1600 */
    unsigned int                         rsr6[31];                            /* 1604 ~ 167c */
    volatile STARS_WAIT_TIMEOUT_CTRL0_UNION stars_wait_timeout_ctrl0[16];        /* 1680 ~ 16bc */
    unsigned int                         rsr7[16];                            /* 16c0 ~ 16fc */
    volatile STARS_WAIT_TIMEOUT_CTRL1_UNION stars_wait_timeout_ctrl1[16];        /* 1700 ~ 173c */
    unsigned int                         rsr8[48];                            /* 1740 ~ 17fc */
    volatile STARS_RTSQ_HARDEN_ECO0_UNION stars_rtsq_harden_eco0;              /* 1800 */
    volatile STARS_RTSQ_HARDEN_ECO1_UNION stars_rtsq_harden_eco1;              /* 1804 */
} STARS_RTSQ_NS_REG_REGS_TYPE_STRU;
#endif /* __STARS_RTSQ_NS_REG_C_DEFINE_H__ */
