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

#ifndef _PCIVNIC_MAIN_H_
#define _PCIVNIC_MAIN_H_

#include <linux/types.h>
#include <linux/device.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/bitops.h>
#include <linux/interrupt.h>

#include "dmc_kernel_interface.h"
#include "comm_kernel_interface.h"
#include "pcivnic_config.h"
#include "vnic_cmd_msg.h"
#include "pcivnic_adapt.h"

#ifdef DRV_UT
#define STATIC
#else
#define STATIC static
#endif

#define PCIVNIC_RX_MSG_NORIFY_WORK_NAME "pcivnic-rx-msg-notify-work"

#define module_devdrv "pcivnic"
#define devdrv_err(fmt, ...) do { \
    drv_err(module_devdrv, "<%s:%d:%d> " fmt, \
    current->comm, current->tgid, current->pid, ##__VA_ARGS__); \
} while (0)
#define devdrv_warn(fmt, ...) do { \
    drv_warn(module_devdrv, "<%s:%d:%d> " fmt, \
    current->comm, current->tgid, current->pid, ##__VA_ARGS__); \
} while (0)
#define devdrv_info(fmt, ...) do { \
    drv_info(module_devdrv, "<%s:%d:%d> " fmt, \
    current->comm, current->tgid, current->pid, ##__VA_ARGS__); \
} while (0)
#define devdrv_debug(fmt...) drv_debug(module_devdrv, fmt)

#define devdrv_err_spinlock(fmt, ...)
#define devdrv_warn_spinlock(fmt, ...)
#define devdrv_info_spinlock(fmt, ...)
#define devdrv_debug_spinlock(fmt...)

#define USE_DMA_ADDR

#define PCIVNIC_VALID 1
#define PCIVNIC_INVALID 0

#define PCIVNIC_SYSFS_BEEN_CREATE 1

#define PCIVNIC_DESC_QUEUE_TX 0
#define PCIVNIC_DESC_QUEUE_RX 1

#define PCIVNIC_DESC_QUEUE_DEPTH 512
#define PCIVNIC_NET_LINKED 1

/* The kernel will add other fields to ensure no more than 1 page */
#define PCIVNIC_MAX_PKT_SIZE 3840
#define PCIVNIC_MTU_DEFAULT 3000

#define PCIVNIC_RX_BUDGET 64
#define PCIVNIC_TX_BUDGET 64

/* vnic napi poll small weight avoid tcp backlog drop too many pkt */
#define PCIVNIC_NAPI_POLL_WEIGHT 4

#define PCIVNIC_MTU_LOW 68
#define PCIVNIC_MTU_HIGH 3900

#define PCIVNIC_NAME_SIZE 64

#define PCIVNIC_MAC_FILE "/etc/d-pcivnic.conf"
#define PCIVNIC_CONF_FILE_SIZE  4096
#define PCIVNIC_CONF_SSCANF_OK  7

#define BIT_STATUS_LINK BIT(0)
#define BIT_STATUS_TQ_FULL BIT(1)
#define BIT_STATUS_RQ_FULL BIT(2)

#define PCIVNIC_NEXT_HOP_LOCAL_NETDEV 0xd3
#define PCIVNIC_NEXT_HOP_BROADCAST 0xff
#define PCIVNIC_NEXT_HOP_S2S 0xFFFF
#define PCIVNIC_INIT_INSTANCE_TIMEOUT (4 * HZ)

#define PCIVNIC_MAC_0 0
#define PCIVNIC_MAC_1 1
#define PCIVNIC_MAC_2 2
#define PCIVNIC_MAC_3 3
#define PCIVNIC_MAC_4 4
#define PCIVNIC_MAC_5 5
#define PCIVNIC_MAC_VAL_0 0x00
#define PCIVNIC_MAC_VAL_1 0xe0
#define PCIVNIC_MAC_VAL_2 0xfc
#define PCIVNIC_MAC_VAL_3 0xfc
#define PCIVNIC_MAC_VAL_4 0xd3

#define PCIVNIC_LINKDOWN_NUM 1000
#define PCIVNIC_TIMESTAMP_OUT 36
#define PCIVNIC_TIMEOUT_CNT 5
#define PCIVNIC_CQ_STS 0xe0
#define PCIVNIC_SLEEP_CNT 10
#define PCIVNIC_DELAYWORK_TIME 2
#define PCIVNIC_WATCHDOG_TIME 36

#define VNIC_DBG(args...) do { \
    ;                 \
} while (0)

#ifdef CFG_FEATURE_S2S
#define PCIVNIC_S2S_SERVER_NUM 48
#define PCIVNIC_S2S_CHIP_NUM 8
#define PCIVNIC_S2S_DIE_NUM 2
#define PCIVNIC_S2S_ONE_SERVER_DEV_NUM 16
#define PCIVNIC_S2S_MAX_CHAN_NUM 768
#define VNIC_S2S_MAX_BUFF_DEPTH 64
struct vnic_s2s_queue {
    struct pcivnic_pcidev *pcidev;
    struct sk_buff *skb[VNIC_S2S_MAX_BUFF_DEPTH];
    u32 front;
    u32 rear;
    u32 queue_index;
    struct workqueue_struct *s2s_send_workqueue;
    struct work_struct s2s_send_work;
    spinlock_t s2s_queue_lock;
};

#define VNIC_S2S_RECYCLE_DELAY_TIME 10
#define VNIC_S2S_QUEUE_BUDGET 64
#endif

#define PCIVNIC_SQ_DESC_SIZE sizeof(struct pcivnic_sq_desc)
#define PCIVNIC_CQ_DESC_SIZE sizeof(struct pcivnic_cq_desc)

struct pcivnic_skb_addr {
    struct sk_buff *skb;
    u64 addr;
    int len;
    void *netdev;
    spinlock_t skb_lock;
    unsigned long tx_seq;
    unsigned long timestamp;
};

#define PCIVNIC_MAX_SKB_BUFF_SIZE 0x1000 /* 4K */
struct pcivnic_skb_data_buff {
    void *addr;
    dma_addr_t dma_addr;
};

#define PCIVNIC_FLOW_CTRL_PERIOD 100 /* ms */
#define PCIVNIC_FLOW_CTRL_THRESHOLD 200 /* 2kpps */

struct pcivnic_flow_ctrl {
    unsigned long timestamp;
    u32 pkt;
    u32 threshold;
};

struct pcivnic_dev_stat {
    u64 tx_full;
    u64 fwd_pkt;
    u64 flow_ctrl_drop;
    u64 rx_dma_err;
    u64 rx_dma_fail;
    u64 tx_pkt;
    u64 tx_bytes;
    u64 rx_pkt;
    u64 rx_bytes;
};

struct pcivnic_fwd_stat {
    u64 fwd_all;
    u64 fwd_success;
    u64 disable_drop;
    u64 flow_ctrl_drop;
};

struct pcivnic_sched_stat {
    u64 in;
    u64 out;
    u64 triger;
    u64 last_in;
};

struct pcivnic_pcidev {
    struct device *dev;
    u32 queue_depth;
    u32 dev_id;
    u32 status;
    u32 timeout_cnt;
    struct pcivnic_dev_stat stat;
    struct pcivnic_sched_stat tx_finish_sched_stat;
    struct pcivnic_fwd_stat fwd_stat[NETDEV_PCIDEV_NUM];
    struct pcivnic_flow_ctrl flow_ctrl[NETDEV_PCIDEV_NUM];
    void *msg_chan;
    void *priv;
    void *netdev;
    spinlock_t lock;
    struct tasklet_struct tx_finish_task;
    struct tasklet_struct rx_notify_task;
    struct workqueue_struct *rx_workqueue;
    struct work_struct rx_notify_work;
#ifdef CFG_FEATURE_S2S
    struct delayed_work s2s_recycle;
    struct vnic_s2s_queue s2s_send_queue[PCIVNIC_S2S_MAX_CHAN_NUM];
#endif
    struct pcivnic_skb_addr tx[PCIVNIC_DESC_QUEUE_DEPTH];
    struct pcivnic_skb_addr rx[PCIVNIC_DESC_QUEUE_DEPTH];
#ifdef CFG_FEATURE_AGENT_SMMU
    struct pcivnic_skb_data_buff tx_buff[PCIVNIC_DESC_QUEUE_DEPTH];
    struct pcivnic_skb_data_buff rx_buff[PCIVNIC_DESC_QUEUE_DEPTH];
    u32 host_phy_mach_flag;
#endif
    int sysfs_create_flag;
    /* msg guard work */
    struct delayed_work guard_work;
    bool is_mdev_vm_boot_mode;
};

struct pcivnic_net_dev_stat {
    u64 send_pkt;
    u64 recv_pkt;
};

struct pcivnic_netdev {
    struct net_device *ndev;
    struct napi_struct napi;
    u32 ndev_register;
    u32 status;
    u32 pciedev_num;
    struct pcivnic_net_dev_stat stat;
    spinlock_t lock;
    spinlock_t rx_lock;
    struct delayed_work timeout;
    struct pcivnic_pcidev *pcidev[NETDEV_PCIDEV_NUM];
    struct sk_buff_head skbq;
};

#define PCIVNIC_CTRL_MSG_TYPE_SET_MAC 0
#define PCIVNIC_CTRL_MSG_TYPE_GET_STAT 1
#define PCIVNIC_CTRL_MSG_TYPE_RIGISTER_NETDEV 2
#define PCIVNIC_CTRL_MSG_TYPE_INSTANCE 3
#define PCIVNIC_CTRL_MSG_TYPE_MAX 4

#define PCIVNIC_CTRL_MSG_RESERVE_NUM 4
struct pcivnic_ctrl_msg_head {
    u32 msg_type;
    u32 host_udevid;
    u32 reserve[PCIVNIC_CTRL_MSG_RESERVE_NUM];
};

struct pcivnic_ctrl_msg_set_mac {
    struct pcivnic_ctrl_msg_head head;
    unsigned char mac[ETH_ALEN];
};

struct pcivnic_ctrl_msg_register_netdev {
    struct pcivnic_ctrl_msg_head head;
};

struct pcivnic_ctrl_msg_dev_instance {
    struct pcivnic_ctrl_msg_head head;
};

#define VNIC_STAT_MSG_MAX_LEN (60 * 1024)
struct pcivnic_ctrl_msg_get_stat {
    struct pcivnic_ctrl_msg_head head;
    u32 msg_len;
    char msg[PAGE_SIZE];
};

#define PCIVNIC_DIE_NUM_ONE_CHIP 2
#define VNIC_MAX_SERVER_NUM 48
#define VNIC_DEFAULT_IP 168

void pcivnic_rx_packet(struct sk_buff *skb, struct pcivnic_netdev *vnic_dev, u32 dev_id);
void pcivnic_s2s_send_guard_work(struct work_struct *p_work);
struct pcivnic_netdev *pcivnic_get_netdev(u32 dev_id);
void pcivnic_s2s_npi_work(struct work_struct *p_work);
int pcivnic_get_server_id(void);
int pcivnic_get_addr_mode(void);
extern struct pcivnic_pcidev *pcivnic_get_pcidev(void *msg_chan);
extern struct pcivnic_sq_desc *pcivnic_get_w_sq_desc(void *msg_chan, u32 *tail);
extern void pcivnic_set_w_sq_desc_head(void *msg_chan, u32 head);
extern void pcivnic_copy_sq_desc_to_remote(struct pcivnic_pcidev *pcidev, const struct pcivnic_sq_desc *sq_desc);
extern bool pcivnic_w_sq_full_check(void *msg_chan);
extern struct pcivnic_sq_desc *pcivnic_get_r_sq_desc(void *msg_chan, u32 *head);
extern void pcivnic_move_r_sq_desc(void *msg_chan);
extern int pcivnic_dma_copy(const struct pcivnic_pcidev *pcidev, u64 src, u64 dst, u32 size,
                            struct devdrv_asyn_dma_para_info *para_info);
extern struct pcivnic_cq_desc *pcivnic_get_w_cq_desc(void *msg_chan);
extern void pcivnic_copy_cq_desc_to_remote(struct pcivnic_pcidev *pcidev, const struct pcivnic_cq_desc *cq_desc);
extern struct pcivnic_cq_desc *pcivnic_get_r_cq_desc(void *msg_chan);
extern void pcivnic_move_r_cq_desc(void *msg_chan);

extern struct pcivnic_pcidev *pcivnic_get_pciedev(const struct device *dev);
extern ssize_t pcivnic_get_dev_stat(struct device *dev, struct device_attribute *attr, char *buf);
extern ssize_t pcivnic_get_dev_stat_inner(struct device *dev, char *buf);

extern bool pcivnic_is_p2p_enabled(u32 dev_id, u32 peer_dev_id);

extern int pcivnic_up_get_next_hop(const unsigned char *dmac);
extern int pcivnic_down_get_next_hop(const unsigned char *dmac);

void pcivnic_skb_data_buff_uninit(struct pcivnic_pcidev *pcidev);
int pcivnic_skb_data_buff_init(struct pcivnic_pcidev *pcidev);

u64 pcivnic_dma_map_single(struct pcivnic_pcidev *pcidev, struct sk_buff *skb, u32 buff_type, u32 index);
void pcivnic_dma_unmap_single(struct pcivnic_pcidev *pcidev, struct sk_buff *skb, u32 buff_type, u32 index);

extern void pcivnic_get_mac(unsigned char last_byte, unsigned char *mac);
extern void pcivnic_set_netdev_mac(struct pcivnic_netdev *vnic_dev, const unsigned char *mac);

extern void pcivnic_rx_msg_notify(void *msg_chan);
extern void pcivnic_tx_finish_notify(void *msg_chan);
extern struct pcivnic_pcidev *pcivnic_add_dev(struct pcivnic_netdev *vnic_dev, struct device *dev, u32 queue_depth,
                                              int net_dev_id);
extern void pcivnic_del_dev(struct pcivnic_netdev *vnic_dev, int dev_id);
extern struct pcivnic_netdev *pcivnic_alloc_netdev(const char *ndev_name, int ndev_name_len);
extern void pcivnic_free_netdev(struct pcivnic_netdev *vnic_dev);
extern int pcivnic_register_netdev(struct pcivnic_netdev *vnic_dev);
extern bool pcivnic_is_register_netdev(u32 dev_id);

extern void pcivnic_init_msgchan_cq_desc(void *msg_chan);

extern int pcivnic_device_status_abnormal(const void *msg_chan);
extern bool pcivnic_get_sysfs_creat_group_capbility(struct device *dev, int dev_id);
#endif  // _DEVDRV_MAIN_H_
