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

#ifdef CONFIG_GENERIC_BUG
#undef CONFIG_GENERIC_BUG
#endif
#ifdef CONFIG_BUG
#undef CONFIG_BUG
#endif
#ifdef CONFIG_DEBUG_BUGVERBOSE
#undef CONFIG_DEBUG_BUGVERBOSE
#endif

#include <linux/errno.h>
#include <linux/ethtool.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/jiffies.h>
#include <linux/version.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>
#include <linux/sysfs.h>
#include "pcivnic_mem_alloc.h"
#include "pbl/pbl_spod_info.h"
#include "pcivnic_main.h"

#define PCIVNIC_COUNT 64
#define CHAR_ARRAY_MAX_LEN 20

const char g_pcivnic_name[CHAR_ARRAY_MAX_LEN] = "pcivnic_driver";

static struct {
    const char str[ETH_GSTRING_LEN];
} pcivnic_ethtool_stats[] = {
    {"pcivnic_rx_packets"}, {"pcivnic_tx_packets"}, {"pcivnic_rx_bytes"}, {"pcivnic_tx_bytes"}, {"pcivnic_rx_errors"},
    {"pcivnic_tx_errors"},  {"pcivnic_rx_dropped"}, {"pcivnic_tx_dropped"}, {"pcivnic_multicast"},
    {"pcivnic_collisions"}, {"pcivnic_rx_length_errors"}, {"pcivnic_rx_over_errors"}, {"pcivnic_rx_crc_errors"},
    {"pcivnic_rx_frame_errors"}, {"pcivnic_rx_fifo_errors"}, {"pcivnic_rx_missed_errors"},
    {"pcivnic_tx_aborted_errors"}, {"pcivnic_tx_carrier_errors"}, {"pcivnic_tx_fifo_errors"},
    {"pcivnic_tx_heartbeat_errors"}, {"pcivnic_tx_window_errors"}, {"pcivnic_rx_compressed"}, {"pcivnic_tx_compressed"}
};

STATIC void pcivnic_net_get_drvinfo(struct net_device *net_dev, struct ethtool_drvinfo *info)
{
    char version[CHAR_ARRAY_MAX_LEN] = "pcivnic v1.0";
    char bus_info[CHAR_ARRAY_MAX_LEN] = "pcivnic";

    if (strncpy_s(info->driver, CHAR_ARRAY_MAX_LEN, g_pcivnic_name, CHAR_ARRAY_MAX_LEN - 1) == EOK &&
        strncpy_s(info->version, CHAR_ARRAY_MAX_LEN, version, CHAR_ARRAY_MAX_LEN - 1) == EOK &&
        strncpy_s(info->bus_info, CHAR_ARRAY_MAX_LEN, bus_info, CHAR_ARRAY_MAX_LEN - 1) == EOK) {
        return;
    }

    devdrv_err("strncpy_s fail\n");
    return;
}

STATIC unsigned int pcivnic_net_get_link(struct net_device *net_dev)
{
    struct pcivnic_netdev *vnic_dev = netdev_priv(net_dev);
    return (vnic_dev->status & BIT_STATUS_LINK);
}

STATIC int pcivnic_get_sset_count(struct net_device *net_dev, int sset)
{
    switch (sset) {
        case ETH_SS_STATS:
            return ARRAY_SIZE(pcivnic_ethtool_stats);
        default:
            return -EOPNOTSUPP;
    }
}

STATIC void pcivnic_get_strings(struct net_device *net_dev, u32 strset, u8 *buf)
{
    if (strset == ETH_SS_TEST) {
        devdrv_warn("strset equal with ETH_SS_TEST\n");
        return;
    }
    if (memcpy_s(buf, sizeof(pcivnic_ethtool_stats), pcivnic_ethtool_stats, sizeof(pcivnic_ethtool_stats)) != 0) {
        devdrv_err("memcpy_s fail\n");
    }
    return;
}

STATIC void pcivnic_get_ethtool_stats(struct net_device *net_dev, struct ethtool_stats *estats, u64 *stats)
{
    struct pcivnic_netdev *vnic_dev = netdev_priv(net_dev);
    int i = 0;

    devdrv_info("%s: pcivnic_get_ethtool_stats\n", vnic_dev->ndev->name);
    stats[i++] = net_dev->stats.rx_packets;
    stats[i++] = net_dev->stats.tx_packets;
    stats[i++] = net_dev->stats.rx_bytes;
    stats[i++] = net_dev->stats.tx_bytes;
    stats[i++] = net_dev->stats.rx_errors;
    stats[i++] = net_dev->stats.tx_errors;
    stats[i++] = net_dev->stats.rx_dropped;
    stats[i++] = net_dev->stats.tx_dropped;
    stats[i++] = net_dev->stats.multicast;
    stats[i++] = net_dev->stats.collisions;
    stats[i++] = net_dev->stats.rx_length_errors;
    stats[i++] = net_dev->stats.rx_over_errors;
    stats[i++] = net_dev->stats.rx_crc_errors;
    stats[i++] = net_dev->stats.rx_frame_errors;
    stats[i++] = net_dev->stats.rx_fifo_errors;
    stats[i++] = net_dev->stats.rx_missed_errors;
    stats[i++] = net_dev->stats.tx_aborted_errors;
    stats[i++] = net_dev->stats.tx_carrier_errors;
    stats[i++] = net_dev->stats.tx_fifo_errors;
    stats[i++] = net_dev->stats.tx_heartbeat_errors;
    stats[i++] = net_dev->stats.tx_window_errors;
    stats[i++] = net_dev->stats.rx_compressed;
    stats[i++] = net_dev->stats.tx_compressed;
}

STATIC void pcivnic_irqs_enable(struct pcivnic_netdev *vnic_dev)
{
    skb_queue_head_init(&vnic_dev->skbq);
    napi_enable(&vnic_dev->napi);
}

STATIC void pcivnic_irqs_disable(struct pcivnic_netdev *vnic_dev)
{
    napi_disable(&vnic_dev->napi);
    skb_queue_purge(&vnic_dev->skbq);
}

STATIC int pcivnic_net_open(struct net_device *dev)
{
    struct pcivnic_netdev *vnic_dev = netdev_priv(dev);

    netif_carrier_off(dev);
    pcivnic_irqs_enable(vnic_dev);
    netif_carrier_on(dev);

    netif_start_queue(dev);

    vnic_dev->status |= BIT_STATUS_LINK;

    devdrv_info("%s: pcivnic_net_open\n", vnic_dev->ndev->name);

    /* Notify the other side that we're open will be next iteration */
    return 0;
}

STATIC int pcivnic_net_close(struct net_device *dev)
{
    struct pcivnic_netdev *vnic_dev = netdev_priv(dev);

    pcivnic_irqs_disable(vnic_dev);
    netif_carrier_off(dev);
    netif_stop_queue(dev);
    vnic_dev->status &= ~BIT_STATUS_LINK;

    devdrv_info("%s: pcivnic_net_close\n", vnic_dev->ndev->name);

    /* Notify the other side that we're closed will be next iteration */
    return 0;
}

STATIC int pcivnic_get_next_valid_pcidev(struct pcivnic_netdev *vnic_dev, int begin)
{
    struct pcivnic_pcidev *pcidev = NULL;
    int i;

    for (i = begin; i < NETDEV_PCIDEV_NUM; i++) {
        pcidev = vnic_dev->pcidev[i];
        if (pcidev == NULL) {
            continue;
        }
        if ((pcidev->status & BIT_STATUS_LINK) != 0) {
            if (pcidev->msg_chan != NULL) {
                return i;
            }
        }
    }

    return -EINVAL;
}

STATIC void pcivnic_netdev_completed_queue(struct pcivnic_pcidev *pcidev, u32 *bytes_compl, u32 *pkts_compl)
{
    struct pcivnic_netdev *vnic_dev = (struct pcivnic_netdev *)pcidev->netdev;

    spin_lock_bh(&vnic_dev->lock);
    if ((*pkts_compl) || (*bytes_compl)) {
        netdev_completed_queue(vnic_dev->ndev, *pkts_compl, *bytes_compl);
    }

    if (unlikely(netif_queue_stopped(vnic_dev->ndev)) && (*pkts_compl)) {
        netif_wake_queue(vnic_dev->ndev);
    }
    spin_unlock_bh(&vnic_dev->lock);
}

#ifdef CFG_FEATURE_S2S
STATIC int pcivnic_s2s_sk_buff_enqueue(struct vnic_s2s_queue *s2s_send_queue, struct sk_buff *val)
{
    spin_lock_bh(&s2s_send_queue->s2s_queue_lock);
    if ((s2s_send_queue->rear + 1) % VNIC_S2S_MAX_BUFF_DEPTH == s2s_send_queue->front) {
        spin_unlock_bh(&s2s_send_queue->s2s_queue_lock);
        return -ENOMEM;
    }

    s2s_send_queue->skb[s2s_send_queue->rear] = val;
    s2s_send_queue->rear = (s2s_send_queue->rear + 1) % VNIC_S2S_MAX_BUFF_DEPTH;
    spin_unlock_bh(&s2s_send_queue->s2s_queue_lock);
    return 0;
}

STATIC struct sk_buff *pcivnic_s2s_sk_buff_dequeue(struct vnic_s2s_queue *s2s_send_queue)
{
    struct sk_buff *val = NULL;

    spin_lock_bh(&s2s_send_queue->s2s_queue_lock);
    if (s2s_send_queue->front == s2s_send_queue->rear) {
        spin_unlock_bh(&s2s_send_queue->s2s_queue_lock);
        return NULL;
    }

    val = s2s_send_queue->skb[s2s_send_queue->front];
    s2s_send_queue->skb[s2s_send_queue->front] = NULL;
    s2s_send_queue->front = (s2s_send_queue->front + 1) % VNIC_S2S_MAX_BUFF_DEPTH;

    spin_unlock_bh(&s2s_send_queue->s2s_queue_lock);
    return val;
}

STATIC void pcivnic_sdid_info_init(unsigned char *dmac, struct sdid_parse_info *sdid_info)
{
    u32 host_devid;
    u32 chip_id, die_id;

    host_devid = dmac[PCIVNIC_MAC_5];
    chip_id = host_devid / PCIVNIC_DIE_NUM_ONE_CHIP;
    die_id = host_devid % PCIVNIC_DIE_NUM_ONE_CHIP;

    sdid_info->server_id = dmac[PCIVNIC_MAC_4];
    sdid_info->chip_id = chip_id;
    sdid_info->die_id = die_id;
    sdid_info->udevid = host_devid;
}

STATIC int pcivnic_data_info_init(struct pcivnic_s2s_data_info *s2s_data_info, struct sk_buff *skb,
    struct data_input_info *data_info)
{
    int ret;

    ret = memcpy_s(&s2s_data_info->data, skb->len, skb->data, skb->len);
    if (ret != 0) {
        devdrv_err("Memcpy skb data fail.(ret=%d)\n", ret);
        return ret;
    }
    s2s_data_info->msg_len = skb->len;

    data_info->data = s2s_data_info;
    data_info->data_len = sizeof(struct pcivnic_s2s_data_info) + skb->len;
    data_info->in_len = sizeof(struct pcivnic_s2s_data_info) + skb->len;
    data_info->out_len = 0;
    data_info->msg_mode = DEVDRV_S2S_SYNC_MODE;

    return 0;
}

STATIC int pcivnic_s2s_send_proc(struct sk_buff *skb, struct pcivnic_pcidev *pcidev, u32 *sdid_list, u32 sdid_len)
{
    struct data_input_info data_info = {0};
    struct sdid_parse_info sdid_info = {0};
    struct pcivnic_s2s_data_info *s2s_data_info = NULL;
    u32 sdid = 0;
    int ret;
    u32 i;

    if ((sdid_list == NULL) || (sdid_len > VNIC_S2S_QUEUE_BUDGET)) {
        devdrv_err("Sdid list is null, or len is invaild.(ret=%d, devid=%u, sdid_len=%u)\n",
            ret, pcidev->dev_id, sdid_len);
        return -EINVAL;
    }

    pcivnic_sdid_info_init((unsigned char *)skb_mac_header(skb), &sdid_info);
    ret = dbl_make_sdid(&sdid_info, &sdid);
    if (ret != 0) {
        devdrv_err("Get sdid fail.(ret=%d, devid=%u)\n", ret, pcidev->dev_id);
        return -EINVAL;
    }

    for (i = 0; i < sdid_len; i++) {
        /* no s2s abnormal, break to improve performance */
        if (sdid_list[i] == U32_MAX) {
            break;
        }

        /* dst sdid may abnormal, please check or retry */
        if (sdid_list[i] == sdid) {
            return -ENODEV;
        }
    }

    s2s_data_info = (struct pcivnic_s2s_data_info *)pcivnic_kvzalloc(skb->len + sizeof(struct pcivnic_s2s_data_info),
        GFP_KERNEL | __GFP_ACCOUNT);
    if (s2s_data_info == NULL) {
        devdrv_err("Alloc skb data buf fail.(devid=%u)\n", pcidev->dev_id);
        return -ENOMEM;
    }

    ret = pcivnic_data_info_init(s2s_data_info, skb, &data_info);
    if (ret != 0) {
        pcivnic_kvfree(s2s_data_info);
        devdrv_err("s2s msg data init fail.(ret=%d, devid=%u)\n", ret, pcidev->dev_id);
        return -EINVAL;
    }

    ret = agentdrv_s2s_msg_send(pcidev->dev_id, sdid, AGENTDRV_S2S_MSG_PCIVNIC, AGENTDRV_S2S_TO_DEVICE, &data_info);
    if (ret != 0) {
        /* Record abnormal sdid, direct return next time */
        for (i = 0; i < sdid_len; i++) {
            if (sdid_list[i] == sdid) {
                break;
            }

            if (sdid_list[i] == U32_MAX) {
                sdid_list[i] = sdid;
                break;
            }
        }
        pcivnic_kvfree(s2s_data_info);
        devdrv_err("s2s msg send fail.(ret=%d, devid=%u, dst_sdid=%u)\n", ret, pcidev->dev_id, sdid);
        return -EINVAL;
    }

    pcivnic_kvfree(s2s_data_info);
    return 0;
}

STATIC void pcivnic_s2s_send_work(struct work_struct *p_work)
{
    struct vnic_s2s_queue *s2s_send_queue = container_of(p_work, struct vnic_s2s_queue, s2s_send_work);
    struct pcivnic_pcidev *pcidev = s2s_send_queue->pcidev;
    struct pcivnic_netdev *vnic_dev = NULL;
    u32 sdid_list[VNIC_S2S_QUEUE_BUDGET];
    struct sk_buff *skb = NULL;
    u32 bytes_compl = 0;
    u32 pkts_compl = 0;
    int cnt = 0;
    int ret;

    (void)memset_s(sdid_list, VNIC_S2S_QUEUE_BUDGET * sizeof(u32), U32_MAX, VNIC_S2S_QUEUE_BUDGET * sizeof(u32));

    if (pcidev == NULL) {
        return;
    }
    vnic_dev = pcivnic_get_netdev(pcidev->dev_id);
    if (vnic_dev == NULL || vnic_dev->ndev_register != PCIVNIC_VALID) {
        return;
    }

    while (1) {
        if ((pcidev == NULL) || ((pcidev->status & BIT_STATUS_LINK) == 0)) {
            return;
        }

        if ((vnic_dev == NULL) || ((vnic_dev->status & BIT_STATUS_LINK) == 0)) {
            return;
        }

        /* Reach the threshold and schedule out */
        if ((cnt >= VNIC_S2S_QUEUE_BUDGET) && (s2s_send_queue->s2s_send_workqueue != NULL)) {
            queue_work(s2s_send_queue->s2s_send_workqueue, &s2s_send_queue->s2s_send_work);
            break;
        }
        skb = pcivnic_s2s_sk_buff_dequeue(s2s_send_queue);
        if (skb == NULL) {
            break;
        }

        if (skb->len > PCIVNIC_MAX_PKT_SIZE) {
            dev_kfree_skb_any(skb);
            devdrv_err("Len is too big.(len=%u, devid=%u)\n", skb->len, pcidev->dev_id);
            continue;
        }

        ret = pcivnic_s2s_send_proc(skb, pcidev, sdid_list, VNIC_S2S_QUEUE_BUDGET);
        if (ret != 0) {
            dev_kfree_skb_any(skb);
            spin_lock_bh(&vnic_dev->lock);
            vnic_dev->ndev->stats.tx_dropped++;
            vnic_dev->ndev->stats.tx_fifo_errors++;
            spin_unlock_bh(&vnic_dev->lock);
            continue;
        }

        /* update netdev */
        if (vnic_dev->ndev != NULL) {
            spin_lock_bh(&vnic_dev->lock);
            vnic_dev->ndev->stats.tx_packets++;
            vnic_dev->ndev->stats.tx_bytes += skb->len;
            netdev_sent_queue(vnic_dev->ndev, skb->len);
            spin_unlock_bh(&vnic_dev->lock);
        }

        bytes_compl += skb->len;
        pkts_compl++;
        dev_consume_skb_any(skb);

        cnt++;
    }
    pcivnic_netdev_completed_queue(pcidev, &bytes_compl, &pkts_compl);
}

STATIC int pcivnic_pciedev_s2s_send(struct sk_buff *skb, struct pcivnic_pcidev *pcidev, struct net_device *dev)
{
    struct sdid_parse_info sdid_info = {0};
    u32 chan_idx;
    int ret;

    if ((pcidev == NULL) || ((pcidev->status & BIT_STATUS_LINK) == 0)) {
        devdrv_warn("pcidev is not useable\n");
        return NETDEV_TX_OK;
    }

    if ((skb == NULL) || (dev == NULL)) {
        devdrv_warn("skb or dev is null.(devid=%u)\n", pcidev->dev_id);
        return NETDEV_TX_OK;
    }

    pcivnic_sdid_info_init((unsigned char *)skb_mac_header(skb), &sdid_info);
    if ((sdid_info.server_id >= PCIVNIC_S2S_SERVER_NUM) || (sdid_info.chip_id >= PCIVNIC_S2S_CHIP_NUM) ||
        (sdid_info.die_id >= PCIVNIC_S2S_DIE_NUM) || (sdid_info.udevid >= PCIVNIC_S2S_ONE_SERVER_DEV_NUM)) {
        devdrv_warn("skb mac is invaild. (devid=%u, server_id=%u, chip_id=%u, die_id=%u)\n", pcidev->dev_id,
            sdid_info.server_id, sdid_info.chip_id, sdid_info.die_id);
        return NETDEV_TX_OK;
    }

    chan_idx = sdid_info.server_id * PCIVNIC_S2S_ONE_SERVER_DEV_NUM + sdid_info.udevid;
    ret = pcivnic_s2s_sk_buff_enqueue(&pcidev->s2s_send_queue[chan_idx], skb);
    if (ret != 0) {
        dev->stats.tx_dropped++;
        dev->stats.tx_fifo_errors++;
        return NETDEV_TX_BUSY;
    }

    if (pcidev->s2s_send_queue[chan_idx].s2s_send_workqueue != NULL) {
        queue_work(pcidev->s2s_send_queue[chan_idx].s2s_send_workqueue,
            &pcidev->s2s_send_queue[chan_idx].s2s_send_work);
    }

    return NETDEV_TX_OK;
}

STATIC void pcidev_s2s_send_queue_uninit(struct pcivnic_pcidev *pcidev)
{
    u32 i, j;

    if (pcivnic_get_addr_mode() != AGENTDRV_ADMODE_FULL_MATCH) {
        return;
    }

    for (i = 0; i < PCIVNIC_S2S_MAX_CHAN_NUM; i++) {
        if (pcidev->s2s_send_queue[i].s2s_send_workqueue != NULL) {
            destroy_workqueue(pcidev->s2s_send_queue[i].s2s_send_workqueue);
        }
        pcidev->s2s_send_queue[i].pcidev = NULL;
        pcidev->s2s_send_queue[i].queue_index = 0;

        for (j = 0; j < VNIC_S2S_MAX_BUFF_DEPTH; j++) {
            pcidev->s2s_send_queue[i].skb[j] = NULL;
        }
        pcidev->s2s_send_queue[i].front = 0;
        pcidev->s2s_send_queue[i].rear = 0;
    }
}

STATIC int pcidev_s2s_send_queue_init(struct pcivnic_pcidev *pcidev)
{
    u32 i, j;

    if (pcivnic_get_addr_mode() != AGENTDRV_ADMODE_FULL_MATCH) {
        return 0;
    }

    for (i = 0; i < PCIVNIC_S2S_MAX_CHAN_NUM; i++) {
        pcidev->s2s_send_queue[i].pcidev = pcidev;
        pcidev->s2s_send_queue[i].queue_index = i;
        spin_lock_init(&(pcidev->s2s_send_queue[i].s2s_queue_lock));
        for (j = 0; j < VNIC_S2S_MAX_BUFF_DEPTH; j++) {
            pcidev->s2s_send_queue[i].skb[j] = NULL;
        }
        pcidev->s2s_send_queue[i].front = 0;
        pcidev->s2s_send_queue[i].rear = 0;

        pcidev->s2s_send_queue[i].s2s_send_workqueue = create_singlethread_workqueue("s2s-send-work");
        if (pcidev->s2s_send_queue[i].s2s_send_workqueue == NULL) {
            return -EINVAL;
        }
        INIT_WORK(&pcidev->s2s_send_queue[i].s2s_send_work, pcivnic_s2s_send_work);
    }

    return 0;
}
#endif

STATIC int pcivnic_pciedev_send(struct sk_buff *skb, struct pcivnic_pcidev *pcidev, struct net_device *dev)
{
    struct pcivnic_netdev *vnic_dev = (struct pcivnic_netdev *)pcidev->netdev;
    struct pcivnic_sq_desc *sq_desc = NULL;
    u64 addr;
    u32 tail = 0;
    int ret;

    if (pcivnic_device_status_abnormal(pcidev->msg_chan) != 0) {
        devdrv_warn("device %d maybe offline\n", pcidev->dev_id);
        ret = NETDEV_TX_OK;
        goto error;
    }
    spin_lock_bh(&pcidev->lock);
    if ((pcidev->status & BIT_STATUS_LINK) == 0) {
        spin_unlock_bh(&pcidev->lock);
        devdrv_warn("device %d is unlink\n", pcidev->dev_id);
        ret = NETDEV_TX_OK;
        goto error;
    }

    /* tx queue full */
    if (unlikely(pcidev->status & BIT_STATUS_TQ_FULL)) {
        if (dev != NULL) {
            netif_stop_queue(dev);
            dev->stats.tx_dropped++;
            dev->stats.tx_fifo_errors++;
        } else {
            dev_kfree_skb_any(skb);
            skb = NULL;
        }
        spin_unlock_bh(&pcidev->lock);
        pcidev->stat.tx_full++;
        devdrv_info("device %d tx queue full, flag %d, stop tx queue\n",
            pcidev->dev_id, (dev == NULL) ? 0 : 1);
        return NETDEV_TX_BUSY;
    }

    /* update netdev */
    if (dev != NULL) {
        spin_lock_bh(&vnic_dev->lock);
#if LINUX_VERSION_CODE <= KERNEL_VERSION(4, 2, 0)
#ifndef RHEL_RELEASE_CODE
        dev->trans_start = jiffies;
#endif
#endif
        dev->stats.tx_packets++;
        dev->stats.tx_bytes += skb->len;
        netdev_sent_queue(dev, skb->len);
        spin_unlock_bh(&vnic_dev->lock);
    }

    sq_desc = pcivnic_get_w_sq_desc(pcidev->msg_chan, &tail);
    if (sq_desc == NULL || tail >= PCIVNIC_DESC_QUEUE_DEPTH) {
        spin_unlock_bh(&pcidev->lock);
        devdrv_err("sq_desc is NULL\n");
        ret = NETDEV_TX_OK;
        goto error;
    }

#ifdef USE_DMA_ADDR
    addr = pcivnic_dma_map_single(pcidev, skb, PCIVNIC_DESC_QUEUE_TX, tail);
    if (addr == (~(dma_addr_t)0)) {
        spin_unlock_bh(&pcidev->lock);
        devdrv_err("device %d dma map is error\n", pcidev->dev_id);
        ret = NETDEV_TX_OK;
        goto error;
    }
#else
    addr = virt_to_phys(skb->data);
#endif

    sq_desc->data_buf_addr = addr;
    sq_desc->data_len = skb->len;
    wmb();
    sq_desc->valid = PCIVNIC_VALID;

    /* stored locally for release after subsequent sending */
    spin_lock_bh(&pcidev->tx[tail].skb_lock);
    pcidev->tx[tail].addr = addr;
    pcidev->tx[tail].skb = skb;
    pcidev->tx[tail].len = (int)skb->len;
    pcidev->tx[tail].netdev = (void *)dev;
    if (dev != NULL) {
        spin_lock_bh(&vnic_dev->lock);
        pcidev->tx[tail].tx_seq = dev->stats.tx_packets;
        pcidev->tx[tail].timestamp = (unsigned long)(jiffies / HZ);
        spin_unlock_bh(&vnic_dev->lock);
    }
    spin_unlock_bh(&pcidev->tx[tail].skb_lock);

    pcidev->stat.tx_pkt++;
    pcidev->stat.tx_bytes += skb->len;

    pcivnic_copy_sq_desc_to_remote(pcidev, sq_desc);

    if (pcivnic_w_sq_full_check(pcidev->msg_chan)) {
        pcidev->status |= BIT_STATUS_TQ_FULL;
    }

    spin_unlock_bh(&pcidev->lock);

    return NETDEV_TX_OK;

error:
    if (dev != NULL) {
        spin_lock_bh(&vnic_dev->lock);
        dev->stats.tx_dropped++;
        dev->stats.tx_fifo_errors++;
        spin_unlock_bh(&vnic_dev->lock);
    }
    dev_kfree_skb_any(skb);

    return ret;
}

STATIC netdev_tx_t pcivnic_net_xmit(struct sk_buff *skb, struct net_device *dev)
{
    struct pcivnic_netdev *vnic_dev = netdev_priv(dev);
    struct sk_buff *skb_cp = NULL;
    int ret = 0;
    int next_hop;
    int begin = 0;
    vnic_dev->stat.send_pkt++;

    /* bad pkt */
    if (skb->len < ETH_HLEN) {
        goto free_skb;
    }

    next_hop = pcivnic_down_get_next_hop((unsigned char *)skb_mac_header(skb));
    if (next_hop == PCIVNIC_NEXT_HOP_S2S) {
#ifdef CFG_FEATURE_S2S
        ret = pcivnic_pciedev_s2s_send(skb, vnic_dev->pcidev[0], dev);
            if (ret != 0) {
            goto free_skb;
        }
#endif
    } else if (next_hop != PCIVNIC_NEXT_HOP_BROADCAST) {
        if (vnic_dev->pcidev[next_hop] == NULL) {
            devdrv_info("next_hop %d dmac: %pM no pciedev\n", next_hop, skb_mac_header(skb));
            goto free_skb;
        }

        if (vnic_dev->pcidev[next_hop]->msg_chan == NULL) {
            devdrv_info("next_hop %d dmac: %pM no msg chan\n", next_hop, skb_mac_header(skb));
            goto free_skb;
        }

        ret = pcivnic_pciedev_send(skb, vnic_dev->pcidev[next_hop], dev);
    } else {
        next_hop = pcivnic_get_next_valid_pcidev(vnic_dev, begin);
        if (next_hop < 0) {
            goto free_skb;
        }

        begin = next_hop + 1;

        /* Broadcast transmission only counts once */
        skb_cp = skb_copy(skb, GFP_ATOMIC);
        ret = pcivnic_pciedev_send(skb, vnic_dev->pcidev[next_hop], dev);
        while (skb_cp != NULL) {
            next_hop = pcivnic_get_next_valid_pcidev(vnic_dev, begin);
            if (next_hop < 0) {
                break;
            }
            begin = next_hop + 1;

            skb = skb_copy(skb_cp, GFP_ATOMIC);
            if (skb != NULL) {
                (void)pcivnic_pciedev_send(skb, vnic_dev->pcidev[next_hop], NULL);
            }
        }

        if (skb_cp != NULL) {
            dev_kfree_skb_any(skb_cp);
            skb_cp = NULL;
        }
    }

    return (netdev_tx_t)ret;

free_skb:
    spin_lock_bh(&vnic_dev->lock);
    dev->stats.tx_errors++;
    dev->stats.tx_dropped++;
    spin_unlock_bh(&vnic_dev->lock);
    dev_kfree_skb_any(skb);
    return NETDEV_TX_OK;
}

STATIC int pcivnic_tx_finish_para_check(const struct pcivnic_cq_desc *cq_desc)
{
    if ((cq_desc == NULL) || (cq_desc->valid != PCIVNIC_VALID) ||
        (cq_desc->sq_head >= PCIVNIC_DESC_QUEUE_DEPTH)) {
        return -EINVAL;
    }
    return 0;
}

STATIC void pcivnic_tx_free_and_cqsq_update(struct pcivnic_pcidev *pcidev, struct pcivnic_cq_desc *cq_desc,
    u16 tx_head, u32 *bytes_compl, u32 *pkts_compl)
{
    struct pcivnic_netdev *vnic_dev = (struct pcivnic_netdev *)pcidev->netdev;
    struct net_device *ndev = vnic_dev->ndev;
    void *msg_chan = pcidev->msg_chan;
    struct sk_buff *skb = NULL;
    bool skb_been_freed = false;

    spin_lock_bh(&pcidev->tx[tx_head].skb_lock);
    skb = pcidev->tx[tx_head].skb;
    if (skb == NULL) {
        skb_been_freed = true;
        goto next;
    }

    if ((cq_desc != NULL) && (cq_desc->status != 0)) {
        if (pcidev->tx[tx_head].netdev) {
            spin_lock_bh(&vnic_dev->lock);
            ndev->stats.tx_errors++;
            ndev->stats.tx_carrier_errors++;
            spin_unlock_bh(&vnic_dev->lock);
        }
    }

    if (pcidev->tx[tx_head].netdev != NULL) {
        (*pkts_compl)++;
        (*bytes_compl) += (u32)pcidev->tx[tx_head].len;
    }

#ifdef USE_DMA_ADDR
    pcivnic_dma_unmap_single(pcidev, skb, PCIVNIC_DESC_QUEUE_TX, tx_head);
#endif
    dev_consume_skb_any(skb);

next:
    pcidev->tx[tx_head].skb = NULL;
    pcidev->tx[tx_head].addr = (~(dma_addr_t)0);
    pcidev->tx[tx_head].len = 0;
    pcidev->tx[tx_head].netdev = NULL;
    spin_unlock_bh(&pcidev->tx[tx_head].skb_lock);

    spin_lock_bh(&pcidev->lock);
    /* cq_desc != NULL is normal free flow, cq_desc == NULL is timeout flow, timeout flow not move cq */
    if (cq_desc != NULL) {
        cq_desc->valid = PCIVNIC_INVALID;
        pcivnic_move_r_cq_desc(msg_chan);
    }

    /* if skb_been_freed == true, means skb has been freed, no need double update the sq head */
    if (skb_been_freed != true) {
        /* update the sq head pointer to continue send packet */
        tx_head = (tx_head + 1) % PCIVNIC_DESC_QUEUE_DEPTH;
        pcivnic_set_w_sq_desc_head(msg_chan, tx_head);
    }

    if (unlikely(pcidev->status & BIT_STATUS_TQ_FULL)) {
        pcidev->status &= ~BIT_STATUS_TQ_FULL;
    }
    spin_unlock_bh(&pcidev->lock);
}

STATIC void pcivnic_tx_finish_notify_task(unsigned long data)
{
    struct pcivnic_pcidev *pcidev = (struct pcivnic_pcidev *)((uintptr_t)data);
    void *msg_chan = pcidev->msg_chan;
    struct pcivnic_cq_desc *cq_desc = NULL;
    u32 bytes_compl = 0;
    u32 pkts_compl = 0;
    u16 tx_head;
    int cnt = 0;
    int ret;

    pcidev->tx_finish_sched_stat.in++;

    do {
        cq_desc = pcivnic_get_r_cq_desc(msg_chan);
        ret = pcivnic_tx_finish_para_check(cq_desc);
        if (ret != 0) {
            break;
        }

        rmb();

        /* Reach the threshold and schedule out */
        if (cnt >= PCIVNIC_TX_BUDGET) {
            tasklet_schedule(&pcidev->tx_finish_task);
            break;
        }

        tx_head = (u16)cq_desc->sq_head;
        pcivnic_tx_free_and_cqsq_update(pcidev, cq_desc, tx_head, &bytes_compl, &pkts_compl);
        cnt++;
    } while (1);

    pcivnic_netdev_completed_queue(pcidev, &bytes_compl, &pkts_compl);
    pcidev->tx_finish_sched_stat.out++;
}

STATIC void pcivnic_timeout_tx_recycle_proc(struct pcivnic_pcidev *pcidev, u16 tx_head)
{
    u32 bytes_compl = 0;
    u32 pkts_compl = 0;

    pcivnic_tx_free_and_cqsq_update(pcidev, NULL, tx_head, &bytes_compl, &pkts_compl);
    pcivnic_netdev_completed_queue(pcidev, &bytes_compl, &pkts_compl);
}

void pcivnic_tx_finish_notify(void *msg_chan)
{
    struct pcivnic_pcidev *pcidev = pcivnic_get_pcidev(msg_chan);
    if (pcidev != NULL) {
        tasklet_schedule(&pcidev->tx_finish_task);
    }
}

STATIC void pcivnic_response_cq(struct pcivnic_pcidev *pcidev, u32 sq_head, u32 status)
{
    struct pcivnic_cq_desc *cq_desc = NULL;

    /* response cq */
    cq_desc = pcivnic_get_w_cq_desc(pcidev->msg_chan);
    if (cq_desc == NULL) {
        devdrv_err("devid %d cq_desc is NULL, sq_head 0x%x, status 0x%x.\n", pcidev->dev_id, sq_head, status);
        return;
    }
    cq_desc->sq_head = sq_head;
    cq_desc->status = status;
    wmb();
    cq_desc->valid = PCIVNIC_VALID;
    pcivnic_copy_cq_desc_to_remote(pcidev, cq_desc);
}

/* napi receive polling callback function */
static int pcivnic_napi(struct napi_struct *napi, int budget)
{
    struct pcivnic_netdev *vnic_dev = container_of(napi, struct pcivnic_netdev, napi);
    int work_done = 0;
    struct sk_buff *skb = NULL;

    while (1) {
        if (work_done >= budget) {
            break;
        }

        skb = skb_dequeue(&vnic_dev->skbq);
        if (unlikely(skb == NULL)) {
            break;
        }

        (void)napi_gro_receive(&vnic_dev->napi, skb);
        work_done++;
    }

    if (work_done < budget) {
        (void)napi_complete(napi);
    }
    return work_done;
}

void pcivnic_rx_packet(struct sk_buff *skb, struct pcivnic_netdev *vnic_dev, u32 dev_id)
{
    struct net_device *ndev = vnic_dev->ndev;

    spin_lock_bh(&vnic_dev->lock);
    if ((vnic_dev->status & BIT_STATUS_LINK) == 0) {
        ndev->stats.rx_dropped++;
        spin_unlock_bh(&vnic_dev->lock);
	if ((ndev->stats.rx_dropped % PCIVNIC_LINKDOWN_NUM == 0) && (pcivnic_is_register_netdev(dev_id) == true)) {
            devdrv_info("rx drop packet.(len=%d, dev_id=%u)\n", skb->len, dev_id);
        }
        dev_consume_skb_any(skb);
        return;
    }

    /* update stats */
    ndev->stats.rx_packets++;
    ndev->stats.rx_bytes += skb->len;

    spin_unlock_bh(&vnic_dev->lock);

    /* set skb */
    skb->protocol = eth_type_trans(skb, ndev);
    skb->ip_summed = CHECKSUM_NONE;
    skb->dev = ndev;

    /* submit upper level process */
    rmb();

    skb_queue_tail(&vnic_dev->skbq, skb);
    vnic_dev->stat.recv_pkt++;
    if (napi_schedule_prep(&vnic_dev->napi)) {
        __napi_schedule(&vnic_dev->napi);
    }
#ifdef RHEL_RELEASE_CODE
#if (RHEL_RELEASE_VERSION(7, 5) > RHEL_RELEASE_CODE)
    ndev->last_rx = jiffies;
#endif
#else
#if LINUX_VERSION_CODE <= KERNEL_VERSION(4, 2, 0)
    ndev->last_rx = jiffies;
#endif
#endif
}

STATIC int pcivnic_forward_flow_ctrl(struct pcivnic_netdev *vnic_dev, int forward_dev, int next_hop)
{
    struct pcivnic_flow_ctrl *flow_ctrl = NULL;
    struct pcivnic_fwd_stat *fwd_stat = NULL;
    unsigned long timestamp;

    fwd_stat = &vnic_dev->pcidev[forward_dev]->fwd_stat[next_hop];

    fwd_stat->fwd_all++;

    if (pcivnic_is_p2p_enabled((u32)forward_dev, (u32)next_hop) == false) {
        fwd_stat->disable_drop++;
        return PCIVNIC_VALID;
    }

    flow_ctrl = &vnic_dev->pcidev[forward_dev]->flow_ctrl[next_hop];

    flow_ctrl->pkt++;

    /* Threshold not reached */
    if (flow_ctrl->pkt <= flow_ctrl->threshold) {
        fwd_stat->fwd_success++;
        return PCIVNIC_INVALID;
    }

    /* Threshold reached */
    timestamp = (unsigned long)jiffies;
    if (jiffies_to_msecs(timestamp - flow_ctrl->timestamp) < PCIVNIC_FLOW_CTRL_PERIOD) {
        fwd_stat->flow_ctrl_drop++;
        return PCIVNIC_VALID;
    }

    /* new period */
    flow_ctrl->timestamp = timestamp;
    flow_ctrl->pkt = 0;

    fwd_stat->fwd_success++;
    return PCIVNIC_INVALID;
}

STATIC void pcivnic_forward_packet(struct sk_buff *skb, struct pcivnic_netdev *vnic_dev, int forward_dev)
{
    struct sk_buff *skb_cp = NULL;
    int next_hop;
    int begin = 0;

    next_hop = pcivnic_up_get_next_hop((unsigned char *)skb->data);
    if (next_hop == PCIVNIC_NEXT_HOP_LOCAL_NETDEV) {
        pcivnic_rx_packet(skb, vnic_dev, (u32)forward_dev);
    } else if (next_hop != PCIVNIC_NEXT_HOP_BROADCAST) {
        if ((forward_dev == next_hop) || (vnic_dev->pcidev[next_hop] == NULL)) {
            dev_consume_skb_any(skb);
        } else {
            if (pcivnic_forward_flow_ctrl(vnic_dev, forward_dev, next_hop) == PCIVNIC_VALID) {
                dev_consume_skb_any(skb);
                return;
            }

            (void)pcivnic_pciedev_send(skb, vnic_dev->pcidev[next_hop], NULL);
        }
    } else {
        while (1) {
            next_hop = pcivnic_get_next_valid_pcidev(vnic_dev, begin);
            if (next_hop < 0) {
                break;
            }
            begin = next_hop + 1;
            if (forward_dev == next_hop) {
                continue;
            }

            if (pcivnic_forward_flow_ctrl(vnic_dev, forward_dev, next_hop) == PCIVNIC_VALID) {
                continue;
            }

            skb_cp = skb_copy(skb, GFP_ATOMIC);
            if (skb_cp != NULL) {
                (void)pcivnic_pciedev_send(skb_cp, vnic_dev->pcidev[next_hop], NULL);
            }
        }

        pcivnic_rx_packet(skb, vnic_dev, (u32)forward_dev);
    }
}

STATIC void pcivnic_rx_msg_callback(void *data, u32 trans_id, u32 status)
{
    struct pcivnic_pcidev *pcidev = (struct pcivnic_pcidev *)data;
    struct pcivnic_netdev *vnic_dev = (struct pcivnic_netdev *)pcidev->netdev;
    struct sk_buff *skb = NULL;

    if (trans_id >= PCIVNIC_DESC_QUEUE_DEPTH) {
        devdrv_err("dev %d pcivnic rx callback trans id %d error.\n", pcidev->dev_id, trans_id);
        return;
    }

    spin_lock_bh(&pcidev->rx[trans_id].skb_lock);
    skb = pcidev->rx[trans_id].skb;
    if (skb == NULL) {
        spin_unlock_bh(&pcidev->rx[trans_id].skb_lock);
        devdrv_err("dev %d pcivnic rx head %d skb is null.\n", pcidev->dev_id, trans_id);
        return;
    }

#ifdef USE_DMA_ADDR
    pcivnic_dma_unmap_single(pcidev, skb, PCIVNIC_DESC_QUEUE_RX, trans_id);
#endif

    pcidev->stat.rx_pkt++;
    pcidev->stat.rx_bytes += (u64)pcidev->rx[trans_id].len;

    (void)skb_put(skb, (unsigned int)pcidev->rx[trans_id].len);
    spin_unlock_bh(&pcidev->rx[trans_id].skb_lock);
    if (status != 0) {
        devdrv_warn("dev %d rx callback unintended, rx_head=%d, status=%d.\n", pcidev->dev_id, trans_id, status);
        dev_consume_skb_any(skb);
        pcidev->stat.rx_dma_err++;
    } else {
        pcivnic_forward_packet(skb, vnic_dev, (int)pcidev->dev_id);
        pcivnic_response_cq(pcidev, trans_id, status);
    }

    spin_lock_bh(&pcidev->rx[trans_id].skb_lock);
    pcidev->rx[trans_id].skb = NULL;
    pcidev->rx[trans_id].addr = (~(dma_addr_t)0);
    pcidev->rx[trans_id].len = 0;
    spin_unlock_bh(&pcidev->rx[trans_id].skb_lock);
}

/* only [vfio] and [mdev+sriov's vm] use workqueue, others use tasklet */
STATIC void pcivnic_rx_msg_schedule_task(struct pcivnic_pcidev *pcidev)
{
    if (pcidev->is_mdev_vm_boot_mode == true) {
        queue_work(pcidev->rx_workqueue, &pcidev->rx_notify_work);
    } else {
        tasklet_schedule(&pcidev->rx_notify_task);
    }
}

STATIC void pcivnic_rx_msg_notify_handle(struct pcivnic_pcidev *pcidev)
{
    void *msg_chan = pcidev->msg_chan;
    struct sk_buff *skb = NULL;
    struct pcivnic_sq_desc *sq_desc = NULL;
    u64 addr;
    struct devdrv_asyn_dma_para_info dma_para_info;
    u32 head = PCIVNIC_DESC_QUEUE_DEPTH;
    int cnt = 0;
    int ret = 0;

    dma_para_info.interrupt_and_attr_flag = DEVDRV_LOCAL_IRQ_FLAG;
    dma_para_info.priv = (void *)pcidev;
    dma_para_info.finish_notify = pcivnic_rx_msg_callback;

    do {
        sq_desc = pcivnic_get_r_sq_desc(msg_chan, &head);
        if ((sq_desc == NULL) || (sq_desc->valid != PCIVNIC_VALID) ||
            (head >= PCIVNIC_DESC_QUEUE_DEPTH)) {
            break;
        }

        rmb();

        /* Reach the threshold and schedule out */
        if (cnt >= PCIVNIC_RX_BUDGET) {
            pcivnic_rx_msg_schedule_task(pcidev);
            break;
        }

        dma_para_info.trans_id = head;

        /* the receiving end cannot apply for memory, too many receive
         * buffers, and waits for the upper layer to receive packets.
         * Block live send.
         */
        skb = dev_alloc_skb(PCIVNIC_MAX_PKT_SIZE);
        if (unlikely(skb == NULL)) {
            devdrv_err("dev %d rx alloc skb failed!\n", pcidev->dev_id);
            pcivnic_rx_msg_schedule_task(pcidev);
            break;
        }

#ifdef USE_DMA_ADDR
        addr = pcivnic_dma_map_single(pcidev, skb, PCIVNIC_DESC_QUEUE_RX, head);
        if (addr == (~(dma_addr_t)0)) {
            dev_kfree_skb_any(skb);
            devdrv_err("dev %d dma mapping error!\n", pcidev->dev_id);
            pcivnic_rx_msg_schedule_task(pcidev);
            break;
        }
#else
        addr = virt_to_phys(skb->data);
#endif

        spin_lock_bh(&pcidev->rx[head].skb_lock);
        pcidev->rx[head].skb = skb;
        pcidev->rx[head].addr = addr;
        pcidev->rx[head].len = (int)sq_desc->data_len;
        spin_unlock_bh(&pcidev->rx[head].skb_lock);

        /* copy the packet */
        ret = pcivnic_dma_copy(pcidev, sq_desc->data_buf_addr, addr, sq_desc->data_len, &dma_para_info);
        if (ret != 0) {
            devdrv_warn("devid %d pcivnic_dma_copy is fail\n", pcidev->dev_id);
            pcidev->stat.rx_dma_fail++;
        }
        sq_desc->valid = PCIVNIC_INVALID;

        pcivnic_move_r_sq_desc(msg_chan);
        cnt++;
    } while (1);
}

STATIC void pcivnic_rx_msg_notify_work(struct work_struct *p_work)
{
    struct pcivnic_pcidev *pcidev = container_of(p_work, struct pcivnic_pcidev, rx_notify_work);
    pcivnic_rx_msg_notify_handle(pcidev);
}

STATIC void pcivnic_rx_msg_notify_task(unsigned long data)
{
    struct pcivnic_pcidev *pcidev = (struct pcivnic_pcidev *)((uintptr_t)data);
    pcivnic_rx_msg_notify_handle(pcidev);
}

void pcivnic_rx_msg_notify(void *msg_chan)
{
    struct pcivnic_pcidev *pcidev = pcivnic_get_pcidev(msg_chan);
    if ((pcidev != NULL) && (pcidev->status & BIT_STATUS_LINK)) {
        pcivnic_rx_msg_schedule_task(pcidev);
    }
}

STATIC unsigned long pcivnic_findout_smallest_tx_seq(struct pcivnic_netdev *vnic_dev, int *dev_id, u16 *tx_head)
{
    struct pcivnic_pcidev *pcidev = NULL;
    int i, j;
    unsigned long tx_seq = (unsigned long)-1;

    /* Search for the pkt with the smallest tx seq */
    for (i = 0; i < NETDEV_PCIDEV_NUM; i++) {
        pcidev = vnic_dev->pcidev[i];
        if (pcidev == NULL) {
            continue;
        }

        spin_lock_bh(&pcidev->lock);
        if ((pcidev->status & BIT_STATUS_LINK) == 0) {
                spin_unlock_bh(&pcidev->lock);
                continue;
        }
        for (j = 0; j < (int)(pcidev->queue_depth); j++) {
            spin_lock_bh(&pcidev->tx[j].skb_lock);
            if (pcidev->tx[j].netdev == NULL) {
                spin_unlock_bh(&pcidev->tx[j].skb_lock);
                continue;
            }
            if (pcidev->tx[j].tx_seq < tx_seq) {
                tx_seq = pcidev->tx[j].tx_seq;
                *dev_id = i;
                *tx_head = (u16)j;
            }
            spin_unlock_bh(&pcidev->tx[j].skb_lock);
        }
        spin_unlock_bh(&pcidev->lock);
    }

    return tx_seq;
}

STATIC void pcivnic_update_timeout_stat(struct pcivnic_pcidev *pcidev)
{
    if (pcidev->tx_finish_sched_stat.last_in != pcidev->tx_finish_sched_stat.in) {
        pcidev->timeout_cnt++;
    }
    pcidev->tx_finish_sched_stat.triger++;
    pcidev->tx_finish_sched_stat.last_in = pcidev->tx_finish_sched_stat.in;
}

STATIC void pcivnic_pause_free_queue(struct pcivnic_netdev *vnic_dev, struct pcivnic_pcidev *pcidev)
{
    u32 sq_id;

    for (sq_id = 0; sq_id < pcidev->queue_depth; sq_id++) {
        spin_lock_bh(&pcidev->tx[sq_id].skb_lock);
        if (pcidev->tx[sq_id].skb == NULL) {
            spin_unlock_bh(&pcidev->tx[sq_id].skb_lock);
            continue;
        }

        if (pcidev->tx[sq_id].netdev) {
            spin_lock_bh(&vnic_dev->lock);
            netdev_completed_queue(vnic_dev->ndev, 1, pcidev->tx[sq_id].skb->len);
            spin_unlock_bh(&vnic_dev->lock);
        }

#ifdef USE_DMA_ADDR
        hal_kernel_devdrv_dma_unmap_single(pcidev->dev, pcidev->tx[sq_id].addr, pcidev->tx[sq_id].skb->len, DMA_TO_DEVICE);
#endif
        dev_consume_skb_any(pcidev->tx[sq_id].skb);

        pcidev->tx[sq_id].skb = NULL;
        pcidev->tx[sq_id].addr = (~(dma_addr_t)0);
        pcidev->tx[sq_id].len = 0;
        pcidev->tx[sq_id].netdev = NULL;
        spin_unlock_bh(&pcidev->tx[sq_id].skb_lock);
    }
}

STATIC void pcivnic_net_timeout_work(struct work_struct *p_work)
{
    struct delayed_work *delayed_work = container_of(p_work, struct delayed_work, work);
    struct pcivnic_netdev *vnic_dev = container_of(delayed_work, struct pcivnic_netdev, timeout);
    struct net_device *dev = vnic_dev->ndev;
    struct pcivnic_pcidev *pcidev = NULL;
    u16 tx_head = 0;
    int dev_id = 0;
    unsigned long tx_seq;
    unsigned long cur_timestamp = (unsigned long)(jiffies / HZ);
    unsigned long timestamp;
    int timeout_num = 0;

find_next:

    /* Search for the pkt with the smallest tx seq */
    tx_seq = pcivnic_findout_smallest_tx_seq(vnic_dev, &dev_id, &tx_head);

    /* find out */
    if (tx_seq == (unsigned long)-1) {
        devdrv_info("net dev %s watchdog timeout num %d, cur_timestamp %lu, tx finish!\n", dev->name, timeout_num,
                    cur_timestamp);
        return;
    }

    pcidev = vnic_dev->pcidev[dev_id];
    spin_lock_bh(&pcidev->tx[tx_head].skb_lock);
    timestamp = pcidev->tx[tx_head].timestamp;
    spin_unlock_bh(&pcidev->tx[tx_head].skb_lock);
    spin_lock_bh(&pcidev->lock);
    if ((pcidev->status & BIT_STATUS_LINK) == 0) {
        spin_unlock_bh(&pcidev->lock);
        devdrv_info("net dev %s watchdog, pcidev->status=%d.\n", dev->name, pcidev->status);
        return;
    }
    spin_unlock_bh(&pcidev->lock);

    if (cur_timestamp - timestamp > PCIVNIC_TIMESTAMP_OUT) {
        /* set the head cq desc status valid */
        pcivnic_timeout_tx_recycle_proc(pcidev, tx_head);

        /* only host has multi pcie dev in one netdev, clear linkup status */
        if ((pcidev->timeout_cnt > PCIVNIC_TIMEOUT_CNT) && (vnic_dev->pciedev_num > 1)) {
            spin_lock_bh(&pcidev->lock);
            pcidev->status &= ~BIT_STATUS_LINK;
            spin_unlock_bh(&pcidev->lock);
            pcivnic_pause_free_queue(vnic_dev, pcidev);
            devdrv_err("Set device pause, free queue. (netdev=%s; dev=%d; cur_timestamp=%lu)\n",
                dev->name, dev_id, cur_timestamp);
            return;
        }

        timeout_num++;
        pcivnic_update_timeout_stat(pcidev);

        msleep(PCIVNIC_SLEEP_CNT);
        goto find_next;
    }

    devdrv_info("net dev %s watchdog, timeout_num=%d, timeout_cnt=%u, cur_timestamp=%lu, tx_head=%d.\n",
        dev->name, timeout_num, pcidev->timeout_cnt, cur_timestamp, tx_head);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
STATIC void pcivnic_net_timeout(struct net_device *dev, unsigned int txqueue)
#else
STATIC void pcivnic_net_timeout(struct net_device *dev)
#endif
{
    struct pcivnic_netdev *vnic_dev = netdev_priv(dev);
    struct pcivnic_pcidev *pcidev = NULL;
    struct pcivnic_cq_desc *cq_desc = NULL;
    u16 tx_head = 0;
    int dev_id = 0;
    unsigned long tx_seq;
    unsigned long cur_timestamp = (unsigned long)(jiffies / HZ);
    unsigned long timestamp = 0;

    tx_seq = pcivnic_findout_smallest_tx_seq(vnic_dev, &dev_id, &tx_head);
    /* find out */
    if (tx_seq == (unsigned long)-1) {
        goto reset_queue;
    }

    pcidev = vnic_dev->pcidev[dev_id];
    spin_lock_bh(&pcidev->tx[tx_head].skb_lock);
    timestamp = pcidev->tx[tx_head].timestamp;
    spin_unlock_bh(&pcidev->tx[tx_head].skb_lock);
    spin_lock_bh(&pcidev->lock);
    if ((pcidev->status & BIT_STATUS_LINK) != 0) {
        if (cur_timestamp - timestamp > PCIVNIC_TIMESTAMP_OUT) {
            cq_desc = pcivnic_get_r_cq_desc(pcidev->msg_chan);
            if (cq_desc != NULL) {
                devdrv_info_spinlock("dev %d cq id %d valid %d tx_head %d tx_seq %ld cur_timestamp %lu timestamp %lu"
                                     "sched in %llu out %llu triger %llu last sched %llu\n",
                                     dev_id, cq_desc->cq_id, cq_desc->valid, tx_head, tx_seq, cur_timestamp, timestamp,
                                     pcidev->tx_finish_sched_stat.in, pcidev->tx_finish_sched_stat.out,
                                     pcidev->tx_finish_sched_stat.triger, pcidev->tx_finish_sched_stat.last_in);
            }
            /* wakeup tx finish  */
            tasklet_schedule(&pcidev->tx_finish_task);
            pcidev->tx_finish_sched_stat.triger++;
        }
    }
    spin_unlock_bh(&pcidev->lock);

    /* perhaps tx finish tasklet is block, resycle the pkts later */
    (void)schedule_delayed_work(&vnic_dev->timeout, PCIVNIC_DELAYWORK_TIME * HZ);

reset_queue:
    if (netif_queue_stopped(dev)) {
        netif_wake_queue(dev);
    }

    devdrv_warn("net dev %s tx_seq=%ld, cur_timestamp=%lu, timestamp=%lu\n", dev->name, tx_seq, cur_timestamp, timestamp);
}

#ifndef RHEL_RELEASE_CODE
STATIC int pcivnic_net_change_mtu(struct net_device *dev, int new_mtu)
{
    if ((new_mtu < PCIVNIC_MTU_LOW) || (new_mtu > PCIVNIC_MTU_HIGH)) {
        devdrv_err("mtu value is invalid!\n");
        return -EINVAL;
    }
    dev->mtu = (unsigned int)new_mtu;

    return 0;
}
#endif

STATIC struct net_device_stats *pcivnic_net_get_stats(struct net_device *dev)
{
    return &dev->stats;
}

struct ethtool_ops g_pcivnic_ethtools_ops = {
    .get_drvinfo = pcivnic_net_get_drvinfo,
    .get_link = pcivnic_net_get_link,
    .get_strings = pcivnic_get_strings,
    .get_sset_count = pcivnic_get_sset_count,
    .get_ethtool_stats = pcivnic_get_ethtool_stats,
};

struct net_device_ops g_pcivnic_netdev_ops = {
    .ndo_open = pcivnic_net_open,
    .ndo_stop = pcivnic_net_close,
    .ndo_start_xmit = pcivnic_net_xmit,
#ifndef RHEL_RELEASE_CODE
    .ndo_change_mtu = pcivnic_net_change_mtu,
#endif
    .ndo_tx_timeout = pcivnic_net_timeout,
    .ndo_get_stats = pcivnic_net_get_stats,
};

STATIC ssize_t pcivnic_read_file(struct file *file, loff_t *pos, char *addr, size_t count)
{
    ssize_t len;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
    len = kernel_read(file, addr, count, pos);
#else

    char __user *buf = (char __user *)addr;
    mm_segment_t old_fs;

    old_fs = get_fs();
    set_fs(get_ds()); /*lint !e501 */ /* kernel source */
    len = vfs_read(file, buf, count, pos);
    set_fs(old_fs);
#endif

    return len;
}

STATIC char *pcivnic_skip_blank_space(const char *ptr)
{
    while ((*ptr == ' ') || (*ptr == '\t') || (*ptr == '\n')) {
        ptr++;
    }

    return (char *)ptr;
}

STATIC char *pcivnic_skip_line(const char *ptr)
{
    while ((*ptr != '\n') && (*ptr != '\0')) {
        ptr++;
    }

    if (*ptr == '\n') {
        ptr++;
    }

    return pcivnic_skip_blank_space(ptr);
}

STATIC char *pcivnic_skip_notes(const char *ptr)
{
    /* skip blank space  */
    ptr = pcivnic_skip_blank_space(ptr);

    /* skip notes with # */
    while (*ptr == '#') {
        ptr = pcivnic_skip_line(ptr);
    }

    return (char *)ptr;
}

STATIC int pcivnic_is_valid_mac(const unsigned char *mac, int len)
{
    (void)len;

    return is_valid_ether_addr(mac);
}

void pcivnic_get_mac(unsigned char last_byte, unsigned char *mac)
{
    unsigned int tmpmac[ETH_ALEN];
    struct file *file = NULL;
    char *tmpbuf = NULL;
    char *pmac = NULL;
    loff_t offset = 0;
    int tmp_id = -1;
    ssize_t len;
    const int dev_id = 0;
    int ret = 0;

    file = filp_open(PCIVNIC_MAC_FILE, O_RDONLY, 0);
    if (IS_ERR(file)) {
        devdrv_warn("pcivnic mac config file(%s) not existed\n", PCIVNIC_MAC_FILE);
        goto random_mac;
    }

    tmpbuf = (char *)pcivnic_kvzalloc(PCIVNIC_CONF_FILE_SIZE, GFP_KERNEL);
    if (tmpbuf == NULL) {
        devdrv_err("pcivnic mac buffer malloc failed, size %d\n", PCIVNIC_CONF_FILE_SIZE);
        goto close_file;
    }

    len = pcivnic_read_file(file, &offset, tmpbuf, PCIVNIC_CONF_FILE_SIZE - 1);
    if (len <= 0) {
        devdrv_warn("read file(%s) failed\n", PCIVNIC_MAC_FILE);
        goto free_buf;
    }

    tmpbuf[len] = '\0';
    pmac = &tmpbuf[0];
    pmac = pcivnic_skip_notes(pmac);

    /* find the mac address by device id */
    while (*pmac != '\0') {
        tmp_id = -1;
        ret = sscanf_s(pmac, "%d %x:%x:%x:%x:%x:%x", &tmp_id, &tmpmac[PCIVNIC_MAC_0], &tmpmac[PCIVNIC_MAC_1],
                       &tmpmac[PCIVNIC_MAC_2], &tmpmac[PCIVNIC_MAC_3], &tmpmac[PCIVNIC_MAC_4], &tmpmac[PCIVNIC_MAC_5]);
        if (ret != (ETH_ALEN + 1)) {
                devdrv_warn("sscanf_s failed! ret = %d\n", ret);
                goto free_buf;
        }

        if ((ret == PCIVNIC_CONF_SSCANF_OK) && (tmp_id == dev_id)) {
            break;
        }

        pmac = pcivnic_skip_line(pmac);
    }

    if (tmp_id != dev_id) {
        devdrv_warn("can't find device %d fixed mac address\n", dev_id);
        goto free_buf;
    }

    /* mac[4] and mac[5] will be changed in devcie */
    mac[PCIVNIC_MAC_0] = tmpmac[PCIVNIC_MAC_0] & 0xFF;
    mac[PCIVNIC_MAC_1] = tmpmac[PCIVNIC_MAC_1] & 0xFF;
    mac[PCIVNIC_MAC_2] = tmpmac[PCIVNIC_MAC_2] & 0xFF;
    mac[PCIVNIC_MAC_3] = last_byte;
    mac[PCIVNIC_MAC_4] = tmpmac[PCIVNIC_MAC_4] & 0xFF;
    mac[PCIVNIC_MAC_5] = last_byte;

    if (pcivnic_is_valid_mac(mac, ETH_ALEN) == 0) {
        devdrv_warn("The MAC is invalid form file %s\n", PCIVNIC_MAC_FILE);
        goto free_buf;
    }

    pcivnic_kvfree(tmpbuf);
    tmpbuf = NULL;
    (void)filp_close(file, NULL);
    file = NULL;

    devdrv_info("pcivnic device %d, get mac success\n", dev_id);
    return;

free_buf:
    pcivnic_kvfree(tmpbuf);
    tmpbuf = NULL;
close_file:
    (void)filp_close(file, NULL);
    file = NULL;
random_mac:
    mac[PCIVNIC_MAC_0] = PCIVNIC_MAC_VAL_0;
    mac[PCIVNIC_MAC_1] = PCIVNIC_MAC_VAL_1;
    mac[PCIVNIC_MAC_2] = PCIVNIC_MAC_VAL_2;
    mac[PCIVNIC_MAC_3] = PCIVNIC_MAC_VAL_3;
    mac[PCIVNIC_MAC_4] = PCIVNIC_MAC_VAL_4;
    mac[PCIVNIC_MAC_5] = last_byte;
    devdrv_info("pcivnic using default MAC. (dev_id=%d)\n", dev_id);

    return;
}

void pcivnic_set_netdev_mac(struct pcivnic_netdev *vnic_dev, const unsigned char *mac)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 17, 0)
    dev_addr_set(vnic_dev->ndev, mac);
#else
    ether_addr_copy(vnic_dev->ndev->dev_addr, mac);
#endif
}

int pcivnic_register_netdev(struct pcivnic_netdev *vnic_dev)
{
    int ret = -1;

    if (vnic_dev->ndev_register == PCIVNIC_INVALID) {
        ret = register_netdev(vnic_dev->ndev);
        if (ret != 0) {
            devdrv_err("%s, dma register_ndev failed!\n", vnic_dev->ndev->name);
            return ret;
        }

        vnic_dev->ndev_register = PCIVNIC_VALID;
    }

    return ret;
}

void pcivnic_init_msgchan_cq_desc(void *msg_chan)
{
    struct pcivnic_cq_desc *cq_desc = NULL;
    struct pcivnic_cq_desc *w_cq_desc = NULL;
    u32 i;

    cq_desc = pcivnic_get_r_cq_desc(msg_chan);
    w_cq_desc = pcivnic_get_w_cq_desc(msg_chan);
    if ((cq_desc == NULL) || (w_cq_desc == NULL)) {
        devdrv_err("cq_desc is NULL\n");
        return;
    }
    for (i = 0; i < (u32)PCIVNIC_DESC_QUEUE_DEPTH; i++) {
        cq_desc->cq_id = i;
        cq_desc++;
        w_cq_desc->cq_id = i;
        w_cq_desc++;
    }
}

ssize_t pcivnic_get_dev_stat_inner(struct device *dev, char *buf)
{
    struct pcivnic_pcidev *pcidev;
    struct pcivnic_netdev *vnic_dev = NULL;
    ssize_t offset = 0;
    int ret;

    pcidev = pcivnic_get_pciedev(dev);
    if (pcidev == NULL) {
        devdrv_err("not find pcidev\n");
        return offset;
    }

    vnic_dev = (struct pcivnic_netdev *)pcidev->netdev;

    ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "dev_id: %u\n", pcidev->dev_id);
    if (ret >= 0) {
        offset += ret;
    }

    ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
        "net dev status: %x, pci dev status: %x\n", vnic_dev->status, pcidev->status);
    if (ret >= 0) {
        offset += ret;
    }

    ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
        "pci dev timeout cnt: %x\n", pcidev->timeout_cnt);
    if (ret >= 0) {
        offset += ret;
    }

    ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
        "\npci dev stat:\n    tx_full: %llu\n    fwd_pkt: %llu\n    flow_ctrl_drop: %llu\n    rx_dma_err: %llu\n"
        "    rx_dma_fail: %llu\n    tx_pkt: %llu\n    tx_bytes: %llu\n    rx_pkt: %llu\n    rx_bytes: %llu\n",
        pcidev->stat.tx_full, pcidev->stat.fwd_pkt, pcidev->stat.flow_ctrl_drop,
        pcidev->stat.rx_dma_err, pcidev->stat.rx_dma_fail,
        pcidev->stat.tx_pkt, pcidev->stat.tx_bytes, pcidev->stat.rx_pkt, pcidev->stat.rx_bytes);
    if (ret >= 0) {
        offset += ret;
    }

    ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1,
        "\nnet dev stat:\n    send_pkt: %llu\n    recv_pkt: %llu\n    tx_packets: %llu\n    tx_dropped: %llu\n"
        "    tx_fifo_errors: %llu\n    tx_errors: %llu\n    tx_carrier_errors: %llu\n    rx_dropped: %llu\n"
        "    rx_packets: %llu\n",
        vnic_dev->stat.send_pkt, vnic_dev->stat.recv_pkt,  vnic_dev->ndev->stats.tx_packets,
        vnic_dev->ndev->stats.tx_dropped, vnic_dev->ndev->stats.tx_fifo_errors, vnic_dev->ndev->stats.tx_errors,
        vnic_dev->ndev->stats.tx_carrier_errors, vnic_dev->ndev->stats.rx_dropped, vnic_dev->ndev->stats.rx_packets);
    if (ret >= 0) {
        offset += ret;
    }

    return offset;
}

#ifdef CFG_FEATURE_S2S
ssize_t pcivnic_sys_get_server_id(struct device *dev, struct device_attribute *attr, char *buf)
{
    ssize_t offset = 0;
    u32 server_id;
    int ret;

    (void)attr;

    server_id = (u32)pcivnic_get_server_id();
    if ((server_id >= VNIC_MAX_SERVER_NUM) || (pcivnic_get_addr_mode() != AGENTDRV_ADMODE_FULL_MATCH)) {
        server_id = VNIC_DEFAULT_IP;
    }

    ret = snprintf_s(buf + offset, PAGE_SIZE - offset, PAGE_SIZE - offset - 1, "%u\n", server_id);
    if (ret >= 0) {
        offset += ret;
    }

    return offset;
}
static DEVICE_ATTR(get_server_id, S_IRUSR | S_IRGRP, pcivnic_sys_get_server_id, NULL);
#endif
static DEVICE_ATTR(stat, S_IRUSR | S_IRGRP, pcivnic_get_dev_stat, NULL);

static struct attribute *g_pcivnic_sysfs_attrs[] = {
    &dev_attr_stat.attr,
#ifdef CFG_FEATURE_S2S
    &dev_attr_get_server_id.attr,
#endif
    NULL,
};

static const struct attribute_group g_pcivnic_sysfs_group = {
    .attrs = g_pcivnic_sysfs_attrs,
    .name = "vnic",
};

static void pcivnic_init_pcidev(struct pcivnic_pcidev *pcidev)
{
    pcidev->timeout_cnt = 0;
    if (memset_s((void *)&pcidev->stat, sizeof(struct pcivnic_dev_stat), 0, sizeof(struct pcivnic_dev_stat)) != 0) {
        devdrv_warn("pcidev clear stat failed!\n");
    }

    if (memset_s((void *)&pcidev->tx_finish_sched_stat, sizeof(struct pcivnic_sched_stat), 0,
        sizeof(struct pcivnic_sched_stat)) != 0) {
        devdrv_warn("pcidev clear tx_finish_sched_stat failed!\n");
    }

    if (memset_s((void *)&pcidev->fwd_stat[0], sizeof(struct pcivnic_fwd_stat) * NETDEV_PCIDEV_NUM, 0,
        sizeof(struct pcivnic_fwd_stat) * NETDEV_PCIDEV_NUM) != 0) {
        devdrv_warn("pcidev clear fwd_stat failed!\n");
    }

    if (memset_s((void *)&pcidev->tx[0], sizeof(struct pcivnic_skb_addr) * PCIVNIC_DESC_QUEUE_DEPTH, 0,
        sizeof(struct pcivnic_skb_addr) * PCIVNIC_DESC_QUEUE_DEPTH) != 0) {
        devdrv_warn("pcidev tx buffer clear failed!\n");
    }

    if (memset_s((void *)&pcidev->rx[0], sizeof(struct pcivnic_skb_addr) * PCIVNIC_DESC_QUEUE_DEPTH, 0,
        sizeof(struct pcivnic_skb_addr) * PCIVNIC_DESC_QUEUE_DEPTH) != 0) {
        devdrv_warn("pcidev rx buffer clear failed!\n");
    }
}

struct pcivnic_pcidev *pcivnic_add_dev(struct pcivnic_netdev *vnic_dev, struct device *dev, u32 queue_depth,
                                       int net_dev_id)
{
    struct pcivnic_pcidev *pcidev = NULL;
    int ret;
    u32 i;

    pcidev = vnic_dev->pcidev[net_dev_id];

    if (pcidev == NULL) {
        devdrv_err("dev id %d alloc err.", net_dev_id);
        return NULL;
    }

    pcidev->dev = dev;
    pcidev->netdev = (void *)vnic_dev;
    pcidev->dev_id = (u32)net_dev_id;
    pcidev->is_mdev_vm_boot_mode = devdrv_is_mdev_vm_boot_mode(pcidev->dev_id);

    ret = pcivnic_skb_data_buff_init(pcidev);
    if (ret != 0) {
        devdrv_err("dev_id %d skb_data_buff_init failed!\n", net_dev_id);
        return NULL;
    }

    pcivnic_init_pcidev(pcidev);

    /* init skb queue */
    pcidev->queue_depth = queue_depth;

    for (i = 0; i < queue_depth; i++) {
        pcidev->rx[i].skb = NULL;
        pcidev->tx[i].skb = NULL;
    }

    for (i = 0; i < NETDEV_PCIDEV_NUM; i++) {
        pcidev->flow_ctrl[i].threshold = PCIVNIC_FLOW_CTRL_THRESHOLD;
        pcidev->flow_ctrl[i].timestamp = (unsigned long)jiffies;
        pcidev->flow_ctrl[i].pkt = 0;
    }

    tasklet_init(&pcidev->tx_finish_task, pcivnic_tx_finish_notify_task, (uintptr_t)pcidev);

    /* only [vfio] and [mdev+sriov's vm] use workqueue, others use tasklet */
    if (pcidev->is_mdev_vm_boot_mode == true) {
        pcidev->rx_workqueue = create_singlethread_workqueue(PCIVNIC_RX_MSG_NORIFY_WORK_NAME);
        if (pcidev->rx_workqueue == NULL) {
            pcivnic_skb_data_buff_uninit(pcidev);
            devdrv_err("Create_singlethread_workqueue failed. (name=\"%s\")\n", PCIVNIC_RX_MSG_NORIFY_WORK_NAME);
            return NULL;
        }
        INIT_WORK(&pcidev->rx_notify_work, pcivnic_rx_msg_notify_work);
    } else {
        tasklet_init(&pcidev->rx_notify_task, pcivnic_rx_msg_notify_task, (uintptr_t)pcidev);
    }

#ifdef CFG_FEATURE_S2S
    ret = pcidev_s2s_send_queue_init(pcidev);
    if (ret != 0) {
        pcivnic_skb_data_buff_uninit(pcidev);
        pcidev_s2s_send_queue_uninit(pcidev);
        devdrv_err("S2s queue init failed. (dev_id=%d; name=\"%s\")\n", net_dev_id, PCIVNIC_RX_MSG_NORIFY_WORK_NAME);
        return NULL;
    }
    INIT_DELAYED_WORK(&pcidev->s2s_recycle, pcivnic_s2s_send_guard_work);
    (void)schedule_delayed_work(&pcidev->s2s_recycle, VNIC_S2S_RECYCLE_DELAY_TIME * HZ);
#endif

    vnic_dev->pciedev_num++;
    if (pcivnic_get_sysfs_creat_group_capbility(dev, net_dev_id) == true) {
        if (sysfs_create_group(&dev->kobj, &g_pcivnic_sysfs_group) != 0) {
            devdrv_warn("pcivnic sysfs_create_group abnormal.(dev_id=%d )\n", net_dev_id);
            pcidev->sysfs_create_flag = 0;
        } else {
            pcidev->sysfs_create_flag = PCIVNIC_SYSFS_BEEN_CREATE;
        }
    }

    devdrv_info("pcivnic add dev success.(dev_id=%d)\n", net_dev_id);

    return pcidev;
}

void pcivnic_del_dev(struct pcivnic_netdev *vnic_dev, int dev_id)
{
    struct pcivnic_pcidev *pcidev = NULL;
    u32 i;

    pcidev = vnic_dev->pcidev[dev_id];
    spin_lock_bh(&pcidev->lock);
    pcidev->status &= ~BIT_STATUS_LINK;
    spin_unlock_bh(&pcidev->lock);

    if (pcivnic_get_sysfs_creat_group_capbility(pcidev->dev, dev_id) == true) {
        if (pcidev->sysfs_create_flag == PCIVNIC_SYSFS_BEEN_CREATE) {
            sysfs_remove_group(&pcidev->dev->kobj, &g_pcivnic_sysfs_group);
            pcidev->sysfs_create_flag = 0;
        }
    }

    vnic_dev->pciedev_num--;
    msleep(PCIVNIC_SLEEP_CNT);
#ifdef CFG_FEATURE_S2S
    (void)cancel_delayed_work_sync(&pcidev->s2s_recycle);
    pcidev_s2s_send_queue_uninit(pcidev);
#endif
    tasklet_kill(&pcidev->tx_finish_task);
    if (pcidev->is_mdev_vm_boot_mode == true) {
        destroy_workqueue(pcidev->rx_workqueue);
    } else {
        tasklet_kill(&pcidev->rx_notify_task);
    }

    for (i = 0; i < pcidev->queue_depth; i++) {
        if (pcidev->tx[i].skb) {
            if (pcidev->tx[i].netdev) {
                spin_lock_bh(&vnic_dev->lock);
                netdev_completed_queue(vnic_dev->ndev, 1, pcidev->tx[i].skb->len);
                spin_unlock_bh(&vnic_dev->lock);
            }
#ifdef USE_DMA_ADDR
            hal_kernel_devdrv_dma_unmap_single(pcidev->dev, pcidev->tx[i].addr, pcidev->tx[i].skb->len, DMA_TO_DEVICE);
#endif
            dev_consume_skb_any(pcidev->tx[i].skb);
        }

        if (pcidev->rx[i].skb) {
#ifdef USE_DMA_ADDR
            hal_kernel_devdrv_dma_unmap_single(pcidev->dev, pcidev->rx[i].addr, PCIVNIC_MAX_PKT_SIZE, DMA_FROM_DEVICE);
#endif
            dev_consume_skb_any(pcidev->rx[i].skb);
        }
    }
    pcidev->is_mdev_vm_boot_mode = false;
    pcivnic_skb_data_buff_uninit(pcidev);
    devdrv_info("pcivnic del dev success.(dev_id=%d)\n", dev_id);
}

STATIC struct pcivnic_pcidev *pcivnic_alloc_pcidevs(u32 dev_num)
{
    struct pcivnic_pcidev *pcidev;

    pcidev = (struct pcivnic_pcidev *)pcivnic_vzalloc((unsigned long)sizeof(struct pcivnic_pcidev) * dev_num);
    if (pcidev == NULL) {
        devdrv_err("alloc pcidevs err\n");
        return NULL;
    }

    return pcidev;
}

STATIC void pcivnic_free_pcidevs(const struct pcivnic_pcidev *pcidev)
{
    if (pcidev != NULL) {
        pcivnic_vfree(pcidev);
        pcidev = NULL;
    }
}

struct pcivnic_netdev *pcivnic_alloc_netdev(const char *ndev_name, int ndev_name_len)
{
    struct pcivnic_netdev *vnic_dev = NULL;
    struct pcivnic_pcidev *pcidev = NULL;
    struct net_device *ndev = NULL;
    u32 i, j;

    (void)ndev_name_len;
    pcidev = pcivnic_alloc_pcidevs(NETDEV_PCIDEV_NUM);
    if (pcidev == NULL) {
        devdrv_err("alloc pcidev fail\n");
        return NULL;
    }

    /* init netdev */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 17, 0)
    ndev = alloc_netdev(sizeof(struct pcivnic_netdev), ndev_name, NET_NAME_USER, ether_setup);
#else
    ndev = alloc_netdev(sizeof(struct pcivnic_netdev), ndev_name, ether_setup);
#endif
    if (ndev == NULL) {
        pcivnic_free_pcidevs(pcidev);
        devdrv_err("alloc netdev fail\n");
        return NULL;
    }

    vnic_dev = netdev_priv(ndev);
    vnic_dev->ndev_register = PCIVNIC_INVALID;
    vnic_dev->ndev = ndev;

    ndev->netdev_ops = &g_pcivnic_netdev_ops;
    ndev->ethtool_ops = &g_pcivnic_ethtools_ops;

    ndev->watchdog_timeo = PCIVNIC_WATCHDOG_TIME * HZ;
    ndev->hw_features |= NETIF_F_GRO;
    ndev->features |= NETIF_F_HIGHDMA | NETIF_F_GSO;
#ifdef CFG_FEATURE_S2S
    if (pcivnic_get_addr_mode() == AGENTDRV_ADMODE_FULL_MATCH) {
        ndev->max_mtu = PCIVNIC_MTU_HIGH;
    }
#endif
    vnic_dev->status &= ~BIT_STATUS_LINK;
    vnic_dev->status &= ~BIT_STATUS_TQ_FULL;
    vnic_dev->status &= ~BIT_STATUS_RQ_FULL;

    spin_lock_init(&vnic_dev->lock);
    spin_lock_init(&vnic_dev->rx_lock);

    vnic_dev->pciedev_num = 0;

    INIT_DELAYED_WORK(&vnic_dev->timeout, pcivnic_net_timeout_work);

    for (i = 0; i < NETDEV_PCIDEV_NUM; i++) {
        vnic_dev->pcidev[i] = pcidev + i;
        spin_lock_init(&(vnic_dev->pcidev[i]->lock));
        for (j = 0; j < PCIVNIC_DESC_QUEUE_DEPTH; j++) {
            spin_lock_init(&(vnic_dev->pcidev[i]->tx[j].skb_lock));
            spin_lock_init(&(vnic_dev->pcidev[i]->rx[j].skb_lock));
        }
    }

    /* init napi */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 0, 0)
    netif_napi_add_weight(vnic_dev->ndev, &vnic_dev->napi, pcivnic_napi, PCIVNIC_NAPI_POLL_WEIGHT);
#else
    netif_napi_add(vnic_dev->ndev, &vnic_dev->napi, pcivnic_napi, PCIVNIC_NAPI_POLL_WEIGHT);
#endif
    return vnic_dev;
}

void pcivnic_free_netdev(struct pcivnic_netdev *vnic_dev)
{
    int i;

    netif_napi_del(&vnic_dev->napi);
#ifndef DRV_UT
    if (vnic_dev->ndev_register == PCIVNIC_VALID) {
        unregister_netdev(vnic_dev->ndev);
        vnic_dev->ndev_register = PCIVNIC_INVALID;
    }
#endif
    (void)cancel_delayed_work_sync(&vnic_dev->timeout);

    if (vnic_dev->pcidev[0] != NULL) {
        pcivnic_free_pcidevs(vnic_dev->pcidev[0]);
    }

    for (i = 0; i < NETDEV_PCIDEV_NUM; i++) {
        vnic_dev->pcidev[i] = NULL;
    }

    free_netdev(vnic_dev->ndev);
}
