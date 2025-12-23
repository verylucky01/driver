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

#include <linux/aer.h>
#include "res_drv.h"
#include "res_drv_mini_v2.h"
#include "res_drv_cloud_v1.h"
#include "res_drv_cloud_v2.h"
#include "res_drv_mini_v3.h"
#include "res_drv_cloud_v4.h"
#include "comm_kernel_interface.h"
#include "devdrv_util.h"
#include "devdrv_pci.h"
#include "devdrv_adapt.h"

const struct pci_device_id g_devdrv_tbl[] = {
    { PCI_VDEVICE(HUAWEI, CLOUD_V1_DEVICE), HISI_CLOUD_V1 },
    { PCI_VDEVICE(HUAWEI, MINI_V2_DEVICE), HISI_MINI_V2 },
    { PCI_VDEVICE(HUAWEI, MINI_V2_2P_DEVICE), HISI_MINI_V2 },
    { PCI_VDEVICE(HUAWEI, CLOUD_V2_DEVICE), HISI_CLOUD_V2 },
    { PCI_VDEVICE(HUAWEI, CLOUD_V2_2P_DEVICE), HISI_CLOUD_V2 },
    { PCI_VDEVICE(HUAWEI, CLOUD_V2_HCCS_IEP_DEVICE), HISI_CLOUD_V2 },
    { PCI_VDEVICE(HUAWEI, CLOUD_V2_VF_DEVICE), HISI_CLOUD_V2 },
    { DEVDRV_DIVERSITY_PCIE_VENDOR_ID, MINI_V2_DEVICE, PCI_ANY_ID,
      PCI_ANY_ID, 0, 0, HISI_MINI_V2 },
    { PCI_VDEVICE(HUAWEI, MINI_V3_DEVICE), HISI_MINI_V3 },
    { PCI_VDEVICE(HUAWEI, CLOUD_V4_DEVICE), HISI_CLOUD_V4 },
    { PCI_VDEVICE(HUAWEI, CLOUD_V5_DEVICE), HISI_CLOUD_V5 },
    { DEVDRV_PCI_SUBSYS_PRIVATE_VENDOR_HK, MINI_V2_DEVICE, PCI_ANY_ID, PCI_ANY_ID, 0, 0, HISI_MINI_V2 },
    { DEVDRV_PCI_SUBSYS_PRIVATE_VENDOR_KL, MINI_V2_DEVICE, PCI_ANY_ID, PCI_ANY_ID, 0, 0, HISI_MINI_V2 },
    { DEVDRV_PCI_SUBSYS_PRIVATE_VENDOR_HK, CLOUD_V2_DEVICE, PCI_ANY_ID, PCI_ANY_ID, 0, 0, HISI_CLOUD_V2 },
    { DEVDRV_PCI_SUBSYS_PRIVATE_VENDOR_KL, CLOUD_V2_DEVICE, PCI_ANY_ID, PCI_ANY_ID, 0, 0, HISI_CLOUD_V2 },
    {}};
MODULE_DEVICE_TABLE(pci, g_devdrv_tbl);

int devdrv_get_device_id_tbl_num(void)
{
    return (int)(sizeof(g_devdrv_tbl) / sizeof(struct pci_device_id));
}

int (*devdrv_res_init_func[HISI_CHIP_NUM])(struct devdrv_pci_ctrl *pci_ctrl) = {
    [HISI_CLOUD_V1] = devdrv_cloud_v1_res_init,
    [HISI_MINI_V2] = devdrv_mini_v2_res_init,
    [HISI_CLOUD_V2] = devdrv_cloud_v2_res_init,
    [HISI_MINI_V3] = devdrv_mini_v3_res_init,
    [HISI_CLOUD_V4] = devdrv_cloud_v4_res_init,
    [HISI_CLOUD_V5] = devdrv_cloud_v4_res_init,
};

void devdrv_peer_ctrl_init(void)
{
    return;
}

/* devdrv_pci.c */
int devdrv_get_product(void)
{
    return HOST_PRODUCT_DC;
}

void devdrv_shutdown(struct pci_dev *pdev)
{
    if (devdrv_get_host_type() == HOST_TYPE_NORMAL) {
        devdrv_info("Shutdown, pci remove driver.\n");
        drv_pcie_remove(pdev);
    }
    return;
}

static u64 g_aer_critical_cnt = 0;
static u64 g_aer_noncritical_cnt = 0;

STATIC pci_ers_result_t devdrv_error_detected(struct pci_dev *pdev, pci_channel_state_t state)
{
    struct devdrv_pci_ctrl *pci_ctrl = devdrv_get_pdev_main_davinci_dev(pdev);
    pci_ers_result_t ret = PCI_ERS_RESULT_NEED_RESET;
    if (pci_ctrl == NULL) {
        devdrv_err("pci_ctrl is invalid.\n");
        return PCI_ERS_RESULT_NEED_RESET;
    }
    devdrv_warn("Error detected. (dev_id=%u;  state=%d)\n", pci_ctrl->dev_id, (int)state);
    devdrv_notify_blackbox_err(pci_ctrl->dev_id, DEVDRV_PCIE_AER_ERROR);
    switch (state) {
        /* I/O channel is in normal state */
        case pci_channel_io_normal:
            g_aer_noncritical_cnt++;
            devdrv_err("Channel is in normal state. (dev_id=%d; aer_noncritical_cnt=%llu)\n",
                       pci_ctrl->dev_id, g_aer_noncritical_cnt);
            ret = PCI_ERS_RESULT_CAN_RECOVER;
            break;

        /* I/O to channel is blocked */
        case pci_channel_io_frozen:
            g_aer_critical_cnt++;
            devdrv_err("Channel is blocked. (dev_id=%u; aer_critical_cnt=%llu)\n",
                       pci_ctrl->dev_id, g_aer_critical_cnt);

            if (pci_is_enabled(pdev) != 0) {
                pci_disable_pcie_error_reporting(pdev);
                pci_clear_master(pdev);
                pci_release_regions(pdev);
                pci_disable_device(pdev);
            }
            ret = PCI_ERS_RESULT_NEED_RESET;
            break;

        /* PCI card is dead */
        case pci_channel_io_perm_failure:
            ret = PCI_ERS_RESULT_DISCONNECT;
            break;
        default:
            ret = PCI_ERS_RESULT_NEED_RESET;
            break;
    }

    /* Request a slot reset. */
    return ret;
}

STATIC pci_ers_result_t devdrv_slot_reset(struct pci_dev *pdev)
{
    u8 byte = 0;
    int ret;

    devdrv_info("Enter slot reset.\n");

    if (pci_enable_device(pdev) != 0) {
        devdrv_err("Call pci_enable_device failed.\n");
        return PCI_ERS_RESULT_DISCONNECT;
    }

    pci_read_config_byte(pdev, 0x8, &byte);
    if (byte == 0xff) {
        devdrv_err("Slot_reset failed. got another pci error.\n");
        return PCI_ERS_RESULT_DISCONNECT;
    }

    if (pci_request_regions(pdev, "devdrv") != 0) {
        devdrv_err("Call pci_request_regions failed.\n");
        return PCI_ERS_RESULT_DISCONNECT;
    }

    /*lint -e598 -e648 */
    if (dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(DEVDRV_DMA_BIT_MASK_64)) != 0) {
        /*lint +e598 +e648 */
        devdrv_err("dma_set_mask 64 bit failed.\n");
        ret = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(DEVDRV_DMA_BIT_MASK_32));
        if (ret != 0) {
            devdrv_err("dma_set_mask failed. (ret=%d)\n", ret);
            return PCI_ERS_RESULT_DISCONNECT;
        }
    }

    pci_set_master(pdev);

    devdrv_err("The resume of top business(vnic/hdc/devmm) still need to do.\n");

    return PCI_ERS_RESULT_RECOVERED;
}

STATIC void devdrv_error_resume(struct pci_dev *pdev)
{
    devdrv_info("Enter pcie reporting resume.\n");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 7, 0)
    pci_aer_clear_nonfatal_status(pdev);
#else
    pci_cleanup_aer_uncorrect_error_status(pdev);
#endif
    pci_enable_pcie_error_reporting(pdev);
}

const struct pci_error_handlers g_devdrv_err_handler = {
    .error_detected = devdrv_error_detected,
    .slot_reset = devdrv_slot_reset,
    .resume = devdrv_error_resume,
};

/* devdrv_dma.c */
void devdrv_traffic_and_manage_dma_chan_config(struct devdrv_dma_dev *dma_dev)
{
    dma_dev->data_chan[DEVDRV_DMA_DATA_TRAFFIC].chan_start_id =
        dma_dev->data_chan[DEVDRV_DMA_DATA_PCIE_MSG].chan_start_id +
        dma_dev->data_chan[DEVDRV_DMA_DATA_PCIE_MSG].chan_num;
    if (dma_dev->remote_chan_num <= dma_dev->data_chan[DEVDRV_DMA_DATA_TRAFFIC].chan_start_id) {
        dma_dev->data_chan[DEVDRV_DMA_DATA_TRAFFIC].chan_start_id = 0;
    }
    dma_dev->data_chan[DEVDRV_DMA_DATA_TRAFFIC].chan_num =
        dma_dev->remote_chan_num - dma_dev->data_chan[DEVDRV_DMA_DATA_TRAFFIC].chan_start_id;
    dma_dev->data_chan[DEVDRV_DMA_DATA_TRAFFIC].last_use_chan =
        dma_dev->data_chan[DEVDRV_DMA_DATA_TRAFFIC].chan_start_id;
}

#ifndef CFG_FEATURE_PCIE_SENTRY
bool devdrv_is_sentry_work_mode(void)
{
    return false;
}
#endif

void devdrv_load_half_resume(struct devdrv_pci_ctrl *pci_ctrl)
{
    return;
}

__attribute__((unused)) int drv_pcie_suspend(struct device *dev)
{
    return 0;
}

__attribute__((unused)) int drv_pcie_resume_notify(struct device *dev)
{
    return 0;
}
