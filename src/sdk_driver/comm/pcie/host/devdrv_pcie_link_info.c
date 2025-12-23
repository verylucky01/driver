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

#include <linux/io.h>
#include <linux/gpio.h>
#include "devdrv_util.h"
#include "devdrv_pci.h"
#include "devdrv_ctrl.h"
#include "devdrv_pcie_link_info.h"
#include "pbl/pbl_uda.h"

static u32 g_pcie_channel_status = DEVDRV_PCIE_COMMON_CHANNEL_INIT;
static atomic_t g_peer_pcie_status;

#define PCIE_DUMP_LTSSM_TRACER_FN_NAME "pcie_dump_ltssm_tracer"
typedef void (*pcie_dump_ltssm_tracer_fn)(void);
__attribute__((unused)) STATIC pcie_dump_ltssm_tracer_fn g_pcie_dump_ltssm_tracer_fn = NULL;

void devdrv_get_pcie_dump_ltssm_tracer_symbol(void)
{
#ifdef CFG_FEATURE_PCIE_LINK_INFO
    g_pcie_dump_ltssm_tracer_fn = 
        (pcie_dump_ltssm_tracer_fn)(uintptr_t)__symbol_get(PCIE_DUMP_LTSSM_TRACER_FN_NAME);
    if (g_pcie_dump_ltssm_tracer_fn == NULL) {
        devdrv_warn("pcie_dump_ltssm_tracer symbol not find.\n");
    }
#endif
}

void devdrv_put_pcie_dump_ltssm_tracer_symbol(void)
{
#ifdef CFG_FEATURE_PCIE_LINK_INFO
    if (g_pcie_dump_ltssm_tracer_fn == NULL) {
        devdrv_warn("pcie_dump_ltssm_tracer symbol not find.\n");
        return;
    }
    __symbol_put(PCIE_DUMP_LTSSM_TRACER_FN_NAME);
#endif
}

STATIC void devdrv_pcie_dump_ltssm_tracer(void)
{
#ifdef CFG_FEATURE_PCIE_LINK_INFO
    if (g_pcie_dump_ltssm_tracer_fn == NULL) {
        devdrv_warn("pcie_dump_ltssm_tracer symbol not find.\n");
        return;
    }
    g_pcie_dump_ltssm_tracer_fn();
    return;
#endif
}

void devdrv_set_pcie_channel_status(u32 status)
{
#ifdef CFG_FEATURE_PCIE_LINK_INFO
    g_pcie_channel_status = status;
    devdrv_info("set pcie channel status. (status=%u)\n", status);
#endif
    return;
}
EXPORT_SYMBOL(devdrv_set_pcie_channel_status);

u32 devdrv_get_pcie_channel_status(void)
{
    return g_pcie_channel_status;
}

void devdrv_peer_pcie_status_init(void)
{
    atomic_set(&g_peer_pcie_status, DEVDRV_PEER_STATUS_NORMAL);
    return;
}

void devdrv_set_peer_pcie_status(u32 status)
{
#ifdef CFG_FEATURE_PCIE_LINK_INFO
    atomic_set(&g_peer_pcie_status, status);
#endif
    return;
}

u32 devdrv_get_peer_pcie_status(void)
{
    return atomic_read(&g_peer_pcie_status);
}
EXPORT_SYMBOL(devdrv_get_peer_pcie_status);
STATIC int devdrv_get_pcie_mac_link_info(struct devdrv_pcie_link_info_para *pcie_link_info)
{
    u32 mac_reg_link_value;
    u32 ltssm_st;
    void __iomem *apb_base = NULL;

    if (pcie_link_info == NULL) {
        devdrv_err("pcie_link_info is NULL.\n");
        return -EINVAL;
    }
    apb_base = ioremap(PCIE_BASE_ADDR, PCIE_REG_SIZE);
    if (apb_base == NULL) {
        devdrv_err("ioremap fail, apb_base is NULL.\n");
        return -ENOMEM;
    }

    mac_reg_link_value = readl(apb_base + PCIE_MAC_REG_BASE + PCIE_MAC_REG_LINK_ADDR);

    // link_status
    pcie_link_info->link_status = DEVDRV_PCIE_LINK_STATUS_DOWN;
    ltssm_st = (mac_reg_link_value >> PCIE_MAC_REG_LINK_LTSSM_ST_OFFSET) & 0x3F;
    if (ltssm_st == PCIE_MAC_REG_LINK_LTSSM_L0) {
        pcie_link_info->link_status = DEVDRV_PCIE_LINK_STATUS_OK;
    } else {
        iounmap(apb_base);
        apb_base = NULL;
        devdrv_limit_exclusive(warn, DEVDRV_LIMIT_LOG_0x00, "read mac reg link. (reg_value=0x%x)\n",
                               mac_reg_link_value);
        return 0;
    }

    // rate_mode
    pcie_link_info->rate_mode = (mac_reg_link_value >> PCIE_MAC_REG_LINK_SPEED_OFFSET) & 0xF;
    // lane_num
    pcie_link_info->lane_num = mac_reg_link_value & 0x3F;

    iounmap(apb_base);
    apb_base = NULL;
    return 0;
}

int devdrv_get_pcie_link_info(u32 udevid, struct devdrv_pcie_link_info_para* pcie_link_info)
{
    int ret;

    if (pcie_link_info == NULL) {
        devdrv_err("pcie_link_info is NULL.\n");
        return -EINVAL;
    }
    ret = devdrv_get_pcie_mac_link_info(pcie_link_info);
    if (ret != 0) {
        devdrv_err("get mac link info err. (ret=%d, udevid=%u)\n", ret, udevid);
        return ret;
    }

    // if pcie link up status is down, no need to check channel status
    if (pcie_link_info->link_status == DEVDRV_PCIE_LINK_STATUS_DOWN) {
        devdrv_pcie_dump_ltssm_tracer();
        return 0;
    }

    if (g_pcie_channel_status != DEVDRV_PCIE_COMMON_CHANNEL_OK) {
        pcie_link_info->link_status = DEVDRV_PCIE_LINK_STATUS_CHANNEL_ERR;
    }

    return 0;
}
EXPORT_SYMBOL(devdrv_get_pcie_link_info);

int devdrv_set_err_out_gpio(void)
{
    int ret;

    ret = gpio_request(PCIE_ERR_OUT_GPIO5_00, NULL);
    if (ret != 0) {
        devdrv_err("gpio request fail. (ret=%d)\n", ret);
        return ret;
    }

    ret = gpio_direction_output(PCIE_ERR_OUT_GPIO5_00, 1);
    if (ret != 0) {
        devdrv_err("gpio set direction fail. (ret=%d)\n", ret);
        gpio_free(PCIE_ERR_OUT_GPIO5_00);
        return ret;
    }

    gpio_free(PCIE_ERR_OUT_GPIO5_00);
    return 0;
}

STATIC int devdrv_set_pcie_linkdown(void)
{
    void __iomem *apb_base = NULL;
    u32 tmp;

    apb_base = ioremap(PCIE_BASE_ADDR, PCIE_REG_SIZE);
    if (apb_base == NULL) {
        devdrv_err("ioremap fail, apb_base is NULL.\n");
        return -ENOMEM;
    }

    // mask linkdown interrupt
    tmp = readl(apb_base + PCIE_MAC_REG_BASE + PCIE_MAC_REG_MAC_INT_MASK_OFFSET);
    tmp |= 0x2U;
    writel(tmp, apb_base + PCIE_MAC_REG_BASE + PCIE_MAC_REG_MAC_INT_MASK_OFFSET);
    // disable port
    writel(0U, apb_base + PCIE_CORE_GLOBAL_REG_BASE + PCIE_PORT_EN_OFFSET);

    iounmap(apb_base);
    return 0;
}

int devdrv_force_linkdown_inner(u32 index_id)
{
    struct devdrv_pci_ctrl *pci_ctrl;
    int ret;

    if (index_id >= MAX_DEV_CNT) {
        devdrv_err("index_id invalid\n");
        return -EINVAL;
    }
    ret = devdrv_set_pcie_linkdown();
    if (ret != 0) {
        return ret;
    }
    (void)devdrv_set_err_out_gpio();
    devdrv_peer_fault_notifier(DEVDRV_PEER_STATUS_LINKDOWN);
    devdrv_set_peer_pcie_status(DEVDRV_PEER_STATUS_LINKDOWN);
    devdrv_set_pcie_channel_status(DEVDRV_PCIE_COMMON_CHANNEL_LINKDOWN);

    pci_ctrl = devdrv_pci_ctrl_get(index_id);
    if (pci_ctrl == NULL) {
        devdrv_warn("pcie not ready\n");
        return 0;
    }
    devdrv_set_device_status(pci_ctrl, DEVDRV_DEVICE_DEAD);
    if (pci_ctrl->dma_dev != NULL) {
        devdrv_set_dma_status(pci_ctrl->dma_dev, DEVDRV_DMA_DEAD);
        devdrv_dma_stop_business((unsigned long)(uintptr_t)pci_ctrl->dma_dev);
    }

    devdrv_pci_ctrl_put(pci_ctrl);
    return 0;
}

int devdrv_force_linkdown(u32 udevid)
{
    u32 index_id;

    (void)uda_udevid_to_add_id(udevid, &index_id);
    return devdrv_force_linkdown_inner(index_id);
}
EXPORT_SYMBOL(devdrv_force_linkdown);