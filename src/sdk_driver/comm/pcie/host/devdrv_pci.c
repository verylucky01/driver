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

#include <asm/io.h>
#include <linux/delay.h>
#if defined(__sw_64__)
#include <linux/irqdomain.h>
#endif
#include <linux/msi.h>

#include <linux/time.h>
#include <linux/timer.h>
#include <linux/timex.h>
#include <linux/rtc.h>
#include <linux/version.h>
#include <linux/semaphore.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
#include <linux/irq.h>
#include <linux/errno.h>
#endif
#include <linux/aer.h>
#include <linux/gfp.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0)
#include <linux/namei.h>
#endif
#if LINUX_VERSION_CODE > KERNEL_VERSION(6, 6, 0)
#include <linux/pci_regs.h>
#endif

#include "pbl/pbl_uda.h"
#ifdef CFG_FEATURE_PCIE_PROTO_DIP /* Dependency Inversion Principle */
#include "pbl/pbl_soc_res_sync.h"
#endif
#include "pbl/pbl_soc_res.h"
#include "devdrv_dma.h"
#include "devdrv_ctrl.h"
#include "devdrv_msg.h"
#include "devdrv_pci.h"
#include "devdrv_common_msg.h"
#include "devdrv_util.h"
#include "res_drv.h"
#include "devdrv_vpc.h"
#include "devdrv_smmu.h"
#include "devdrv_pasid_rbtree.h"
#ifdef CFG_FEATURE_S2S
#include "devdrv_s2s_msg.h"
#endif
#include "devdrv_pcie_link_info.h"
#include "devdrv_pci.h"
#include "devdrv_mem_alloc.h"
#include "devdrv_adapt.h"

#ifdef CFG_FEATURE_PCIE_SENTRY
#include "devdrv_sentry.h"
#endif

#ifdef DRV_UT
#define STATIC
#else
#define STATIC static
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 17, 0)
enum msi_desc_filter filter = MSI_DESC_ASSOCIATED;
#define for_each_msi_entry(desc, dev) \
    msi_for_each_desc((desc), (dev), (filter))
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 6, 0)
MODULE_IMPORT_NS(CXL);
#define DEVDRV_PCI_EXP_AER_FLAGS (PCI_EXP_DEVCTL_CERE | PCI_EXP_DEVCTL_NFERE | \
    PCI_EXP_DEVCTL_FERE | PCI_EXP_DEVCTL_URRE)
int pci_enable_pcie_error_reporting(struct pci_dev *dev)
{
    int rc;

    if (pcie_aer_is_native(dev) == 0) {
        return -EIO;
    }
    rc = pcie_capability_set_word(dev, PCI_EXP_DEVCTL, DEVDRV_PCI_EXP_AER_FLAGS);
    return pcibios_err_to_errno(rc);
}

int pci_disable_pcie_error_reporting(struct pci_dev *dev)
{
    int rc;

    if (pcie_aer_is_native(dev) == 0) {
        return -EIO;
    }
    rc = pcie_capability_clear_word(dev, PCI_EXP_DEVCTL, DEVDRV_PCI_EXP_AER_FLAGS);
    return pcibios_err_to_errno(rc);
}
#endif

static const char g_devdrv_driver_name[] = "devdrv_device_driver";

#define GEAR_STR_LEN              4

STATIC int g_pci_manage_device_num = 0;
struct devdrv_res_gear g_res_gear;

int g_host_type;
int g_host_product;

/* 3559 add para */
STATIC char *type = "normal";
STATIC char *product = "dc";
module_param(type, charp, S_IRUGO);
module_param(product, charp, S_IRUGO);
STATIC void devdrv_uinit_3559_interrupt(struct devdrv_pci_ctrl *pci_ctrl);

int devdrv_get_host_type(void)
{
    return g_host_type;
}
EXPORT_SYMBOL(devdrv_get_host_type);

STATIC void devdrv_pci_ctrl_pre_init(struct devdrv_pci_ctrl *pci_ctrl, struct pci_dev *pdev,
    const struct pci_device_id *data, int dev_index)
{
    u8 slot_id = PCI_SLOT(pdev->devfn);
    u8 func_num = PCI_FUNC(pdev->devfn);
    u32 chip_type = (u32)data->driver_data;

    pci_ctrl->dev_id = (u32)-1;
    pci_ctrl->load_half_probe = 0;
    pci_ctrl->add_davinci_flag = 0;
    pci_ctrl->load_vector = -1;
    pci_ctrl->chip_type = chip_type;
    pci_ctrl->dev_id_in_pdev = (u32)dev_index;
    pci_ctrl->slot_id = (u32)slot_id;
    pci_ctrl->func_id = (u32)func_num;
    pci_ctrl->pdev = pdev;

    pci_ctrl->dma_desc_rbtree = RB_ROOT;
    spin_lock_init(&pci_ctrl->dma_desc_rblock);
}

STATIC void devdrv_pci_ctrl_work_queue_uninit(struct devdrv_pci_ctrl *pci_ctrl)
{
    int max_msg_chan_cnt;
    int i;

    if (pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_VF) {
        max_msg_chan_cnt = pci_ctrl->ops.get_vf_max_msg_chan_cnt();
    } else {
        max_msg_chan_cnt = pci_ctrl->ops.get_pf_max_msg_chan_cnt();
    }

    if (max_msg_chan_cnt > DEVDRV_MAX_MSG_CHAN_NUM) {
        devdrv_err("Real cnt greater than max num. (dev_id=%u, chan_cnt=%d)\n", pci_ctrl->dev_id, max_msg_chan_cnt);
        return;
    }

    for (i = 0; i < max_msg_chan_cnt; i++) {
        if (pci_ctrl->work_queue[i] != NULL) {
            destroy_workqueue(pci_ctrl->work_queue[i]);
            pci_ctrl->work_queue[i] = NULL;
        }
    }
}

/* not each msg chan create one workqueue, host will create too much; so each msg_client create one workqueue */
STATIC void devdrv_pci_ctrl_work_queue_init(struct devdrv_pci_ctrl *pci_ctrl)
{
    int max_msg_chan_cnt;
    int i;

    if (pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_VF) {
        max_msg_chan_cnt = pci_ctrl->ops.get_vf_max_msg_chan_cnt();
    } else {
        max_msg_chan_cnt = pci_ctrl->ops.get_pf_max_msg_chan_cnt();
    }

    if (max_msg_chan_cnt > DEVDRV_MAX_MSG_CHAN_NUM) {
        devdrv_err("Real cnt greater than max num. (dev_id=%u, chan_cnt=%d)\n", pci_ctrl->dev_id, max_msg_chan_cnt);
        return;
    }

    for (i = 0; i < max_msg_chan_cnt; i++) {
        pci_ctrl->work_queue[i] = create_workqueue("pcie_msg_workqueue");
        if (pci_ctrl->work_queue[i] == NULL) {
            devdrv_warn("Not create msg work_queue[%d]. (dev_id=%u)\n", i, pci_ctrl->dev_id);
        }
    }
}

void devdrv_pcictrl_shr_para_init(struct devdrv_pci_ctrl *pci_ctrl)
{
    if (pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_VF) {
        pci_ctrl->shr_para->ep_pf_index = 0;
    } else {
        pci_ctrl->shr_para->ep_pf_index = (u32)PCI_FUNC(pci_ctrl->pdev->devfn);
    }
    pci_ctrl->shr_para->hot_reset_pcie_flag = 0;
    pci_ctrl->shr_para->connect_protocol = pci_ctrl->connect_protocol;
    pci_ctrl->ep_pf_index = pci_ctrl->shr_para->ep_pf_index;
}

STATIC void devdrv_mdev_sriov_capacity_init(struct devdrv_pci_ctrl *pci_ctrl)
{
    u32 boot_mode;
    int pf_devid;
    int ret;

    if (pci_ctrl->pdev->revision == DEVDRV_REVISION_TYPE_MDEV_SRIOV_VF) {
        /* vf mdev, not full spec */
        pci_ctrl->virtfn_flag = DEVDRV_SRIOV_TYPE_VF;
        pci_ctrl->env_boot_mode = DEVDRV_MDEV_VF_VM_BOOT;
        return;
    }

    if (pci_ctrl->pdev->is_physfn == 0) {
        if (pci_ctrl->pdev->is_virtfn == DEVDRV_SRIOV_TYPE_VF) {
            /* vf container, or vf mdev */
            pci_ctrl->virtfn_flag = DEVDRV_SRIOV_TYPE_VF;
        } else if ((pci_ctrl->chip_type == HISI_CLOUD_V2) && (PCI_FUNC(pci_ctrl->pdev->devfn) != 0)) {
            /* vf through, bdf's function should not be 0 */
            pci_ctrl->virtfn_flag = DEVDRV_SRIOV_TYPE_VF;
            pci_ctrl->env_boot_mode = DEVDRV_SRIOV_VF_BOOT;
            return;
        } else {
            /* pf through */
            pci_ctrl->virtfn_flag = DEVDRV_SRIOV_TYPE_PF;
        }
    } else {
        pci_ctrl->virtfn_flag = DEVDRV_SRIOV_TYPE_PF;
    }

    pf_devid = devdrv_sriov_get_pf_devid_by_vf_ctrl(pci_ctrl);
    if (pf_devid < 0) {
        pci_ctrl->env_boot_mode = DEVDRV_PHY_BOOT;
        devdrv_info("Can not get pf_devid, set DEVDRV_PHY_BOOT as default mode.\n");
        return;
    }
    ret = devdrv_get_sriov_and_mdev_mode((u32)pf_devid, &boot_mode);
    if (ret != 0) {
        devdrv_err("Get get_sriov_and_mdev_mode fail.(devid = %d)\n", pf_devid);
        return;
    }

    devdrv_info("Get cap info. (rev=%u; is_physfn=%u; is_virtfn=%u; devfn=%u; vf_flag=%u; pf_devid=%u; boot_mode=%u)\n",
        pci_ctrl->pdev->revision, pci_ctrl->pdev->is_physfn, pci_ctrl->pdev->is_virtfn, pci_ctrl->pdev->devfn,
        pci_ctrl->virtfn_flag, pf_devid, boot_mode);

    if ((pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_VF) && (boot_mode == DEVDRV_BOOT_MDEV_AND_SRIOV)) {
        pci_ctrl->env_boot_mode = DEVDRV_MDEV_VF_PM_BOOT;
        return;
    }
    if ((pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_VF) && (boot_mode != DEVDRV_BOOT_MDEV_AND_SRIOV)) {
        pci_ctrl->env_boot_mode = DEVDRV_SRIOV_VF_BOOT;
        return;
    }
    if ((pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_PF) && (boot_mode != DEVDRV_BOOT_MDEV_AND_SRIOV)) {
        pci_ctrl->env_boot_mode = DEVDRV_PHY_BOOT;
        return;
    }
}

STATIC void devdrv_connect_protocol_init(struct devdrv_pci_ctrl *pci_ctrl,
    const struct pci_device_id *data)
{
    if (data->device == CLOUD_V2_HCCS_IEP_DEVICE) {
        pci_ctrl->connect_protocol = CONNECT_PROTOCOL_HCCS;
        pci_ctrl->addr_mode = DEVDRV_ADMODE_FULL_MATCH;
        pci_ctrl->multi_die = DEVDRV_MULTI_DIE_ONE_CHIP;
    } else {
        pci_ctrl->connect_protocol = CONNECT_PROTOCOL_PCIE;
        pci_ctrl->addr_mode = DEVDRV_ADMODE_NOT_FULL_MATCH;
        pci_ctrl->multi_die = 0;
    }
}

int devdrv_get_pdev_davinci_dev_num(u32 device, u16 subsystem_device)
{
    if (((device == MINI_V2_DEVICE) || (device == MINI_V2_2P_DEVICE)) &&
        ((subsystem_device >> DEVDRV_PCI_SUBSYS_DEV_MASK_BIT) ==
        (DEVDRV_1PF2P_PCI_SUBSYS_DEV >> DEVDRV_PCI_SUBSYS_DEV_MASK_BIT))) {
        return DEVDRV_DAVINCI_DEV_NUM_1PF2P;
    } else {
        return DEVDRV_DAVINCI_DEV_NUM_1PF1P;
    }
}

int devdrv_get_davinci_dev_num_by_pdev(struct pci_dev *pdev)
{
    struct devdrv_pdev_ctrl *pdev_ctrl = NULL;

    if (pdev == NULL) {
        devdrv_err("pdev is invalid.\n");
        return -EINVAL;
    }

    pdev_ctrl = (struct devdrv_pdev_ctrl *)pci_get_drvdata(pdev);
    if (pdev_ctrl == NULL) {
        devdrv_err("Input parameter is invalid.\n");
        return -EINVAL;
    }

    return pdev_ctrl->dev_num;
}
EXPORT_SYMBOL(devdrv_get_davinci_dev_num_by_pdev);

struct devdrv_pci_ctrl *devdrv_get_dev_by_index_in_pdev(struct pci_dev *pdev, int dev_index)
{
    struct devdrv_pdev_ctrl *pdev_ctrl = (struct devdrv_pdev_ctrl *)pci_get_drvdata(pdev);

    if (pdev_ctrl == NULL) {
        devdrv_err("pdev_ctrl is invalid.\n");
        return NULL;
    }

    if (dev_index >= pdev_ctrl->dev_num) {
        devdrv_err("Input parameter is invalid. (dev_index=%d)\n", dev_index);
        return NULL;
    }

    return pdev_ctrl->pci_ctrl[dev_index];
}

struct devdrv_pci_ctrl *devdrv_get_pdev_main_davinci_dev(struct pci_dev *pdev)
{
    return devdrv_get_dev_by_index_in_pdev(pdev, 0);
}

u32 devdrv_get_main_davinci_devid_by_pdev(struct pci_dev *pdev)
{
    struct devdrv_pdev_ctrl *pdev_ctrl = (struct devdrv_pdev_ctrl *)pci_get_drvdata(pdev);

    if (pdev_ctrl == NULL) {
        devdrv_err("The pdev_ctrl is invalid.\n");
        return -EINVAL;
    }

    return pdev_ctrl->main_dev_id;
}

bool devdrv_is_pdev_main_davinci_dev(struct devdrv_pci_ctrl *pci_ctrl)
{
    return (devdrv_get_pdev_main_davinci_dev(pci_ctrl->pdev) == pci_ctrl);
}

STATIC void devdrv_add_davinci_dev_to_pdev_list(struct devdrv_pci_ctrl *pci_ctrl, int dev_index)
{
    struct devdrv_pdev_ctrl *pdev_ctrl = (struct devdrv_pdev_ctrl *)pci_get_drvdata(pci_ctrl->pdev);
    pdev_ctrl->pci_ctrl[dev_index] = pci_ctrl;
}

STATIC void devdrv_del_davinci_dev_from_pdev_list(struct devdrv_pci_ctrl *pci_ctrl, int dev_index)
{
    struct devdrv_pdev_ctrl *pdev_ctrl = (struct devdrv_pdev_ctrl *)pci_get_drvdata(pci_ctrl->pdev);
    pdev_ctrl->pci_ctrl[dev_index] = NULL;
}

#ifndef writeq
static inline void writeq(u64 value, volatile void *addr)
{
    *(volatile u64 *)addr = value;
}
#endif

#ifndef readq
static inline u64 readq(volatile unsigned char *addr)
{
    return readl(addr) + ((u64)readl(addr + DEVDRV_ADDR_ADD) << DEVDRV_ADDR_MOVE_32);
}
#endif

__attribute__((unused)) STATIC int devdrv_runtime_suspend(struct device *dev)
{
#ifndef CFG_FEATURE_PCIE_SENTRY
    struct pci_dev *pdev = NULL;
    int ret;

    devdrv_info("Enter devdrv_runtime_suspend.\n");

    pdev = container_of(dev, struct pci_dev, dev);
    ret = pci_set_power_state(pdev, PCI_D3hot);
    if (ret != 0) {
        devdrv_err("Call pci_set_power_state error. (ret=%d)\n", ret);
        return ret;
    }

    return 0;
#else
    return drv_pcie_suspend(dev);
#endif
}

__attribute__((unused)) STATIC int devdrv_runtime_resume(struct device *dev)
{
#ifndef CFG_FEATURE_PCIE_SENTRY
    struct pci_dev *pdev = NULL;
    int ret;

    devdrv_info("enter devdrv_runtime_resume!\n");

    pdev = container_of(dev, struct pci_dev, dev);
    ret = pci_set_power_state(pdev, PCI_D0);
    if (ret != 0) {
        devdrv_err("Call pci_set_power_state error. (ret=%d)\n", ret);
        return ret;
    }
#endif
    return 0;
}

static const struct dev_pm_ops g_devdrv_pm_ops = {
    SET_RUNTIME_PM_OPS(devdrv_runtime_suspend, devdrv_runtime_resume, NULL)
    SET_SYSTEM_SLEEP_PM_OPS(drv_pcie_suspend, drv_pcie_resume_notify)
};

int devdrv_check_dl_dlcmsm_state(void *drvdata)
{
    return 0;
}
EXPORT_SYMBOL(devdrv_check_dl_dlcmsm_state);

int devdrv_register_irq_func(void *drvdata, int vector_index, irqreturn_t (*callback_func)(int, void *), void *para,
                             const char *name)
{
    struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)drvdata;
    int vector;
    int rtn = 0;

    if (pci_ctrl == NULL) {
        devdrv_err("drvdata is NULL.\n");
        return -EINVAL;
    }

    if (vector_index < 0) {
        devdrv_err("vector_index is error. (vector_index=%d)\n", vector_index);
        return -EINVAL;
    }

    if ((devdrv_get_host_type() == HOST_TYPE_ARM_3559) || (devdrv_get_host_type() == HOST_TYPE_ARM_3519)) {
        vector = vector_index;
        if (vector >= DEVDRV_MSI_MAX_VECTORS) {
            devdrv_err("Invalid vector. (vector=%d)\n", vector);
            return -EINVAL;
        }
        pci_ctrl->msi_info[vector].callback_func = callback_func;
        pci_ctrl->msi_info[vector].data = para;
    } else {
        if (vector_index >= pci_ctrl->msix_irq_num) {
            devdrv_err("Invalid vector. (vector=%d)\n", vector_index);
            return -EINVAL;
        }

        vector = (int)pci_ctrl->msix_ctrl.entries[vector_index].vector;
        rtn = request_irq((unsigned int)vector, callback_func, 0, name, para);
    }

    return rtn;
}
EXPORT_SYMBOL(devdrv_register_irq_func);

int devdrv_unregister_irq_func(void *drvdata, int vector_index, void *para)
{
    struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)drvdata;
    int vector;

    if (pci_ctrl == NULL) {
        return 0;
    }

    if (vector_index < 0) {
        devdrv_err("Invalid vector. (dev_id=%d; vector=%d)\n", pci_ctrl->dev_id, vector_index);
        return -EINVAL;
    }

    if ((devdrv_get_host_type() == HOST_TYPE_ARM_3559) || (devdrv_get_host_type() == HOST_TYPE_ARM_3519)) {
        vector = vector_index;
        if (vector >= DEVDRV_MSI_MAX_VECTORS) {
            devdrv_err("Invalid vector. (dev_id=%d; vector=%d)\n", pci_ctrl->dev_id, vector);
            return -EINVAL;
        }
        pci_ctrl->msi_info[vector].callback_func = NULL;
    } else {
        if (vector_index >= pci_ctrl->msix_irq_num) {
            devdrv_err("Invalid vector. (dev_id=%d; vector=%d)\n", pci_ctrl->dev_id, vector_index);
            return -EINVAL;
        }

        vector = (int)pci_ctrl->msix_ctrl.entries[vector_index].vector;
        (void)irq_set_affinity_hint((unsigned int)vector, NULL);
        (void)free_irq((unsigned int)vector, para);
    }

    return 0;
}
EXPORT_SYMBOL(devdrv_unregister_irq_func);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
STATIC void devdrv_rc_irq_set_mask(void __iomem *io_base, u32 val)
{
    writel(val, io_base + DEVDRV_RC_MSI_MASK);
}

STATIC void devdrv_rc_rd_status(const void __iomem *io_base, u32 *val, u32 i)
{
    u32 sts;

    sts = readl(io_base + DEVDRV_RC_MSI_STATUS);
    *val = ((sts >> i) & 0x1);
}

STATIC void devdrv_rc_wr_status(void __iomem *io_base, u32 i)
{
    u32 val;

    val = 1U << i;
    writel(val, io_base + DEVDRV_RC_MSI_STATUS);
}

STATIC irqreturn_t devdrv_irq_main_func(int irq, void *data)
{
    struct devdrv_pci_ctrl *pci_ctrl = data;
    u32 int_status;
    u32 i;

    if ((devdrv_get_host_type() == HOST_TYPE_ARM_3519) || (devdrv_get_host_type() == HOST_TYPE_ARM_3559)) {
        devdrv_rc_irq_set_mask(pci_ctrl->rc_reg_base, 0xFFFFFFFFU);
        for (i = 0; i < DEVDRV_MSI_MAX_VECTORS; i++) {
            devdrv_rc_rd_status(pci_ctrl->rc_reg_base, &int_status, i);
            if (int_status == 0)
                continue;

            if (pci_ctrl->msi_info[i].callback_func != NULL) {
                (void)pci_ctrl->msi_info[i].callback_func((int)i, pci_ctrl->msi_info[i].data);
            }
            devdrv_rc_wr_status(pci_ctrl->rc_reg_base, i);
        }
        devdrv_rc_irq_set_mask(pci_ctrl->rc_reg_base, 0x0);
    }

    return IRQ_HANDLED;
}
#endif

/* 3559 use */
int devdrv_pci_irq_vector(struct pci_dev *dev, unsigned int nr)
{
    if (dev == NULL) {
        devdrv_err("Input parameter is invalid.\n");
        return -EINVAL;
    }
    return (int)(dev->irq + nr);
}
EXPORT_SYMBOL(devdrv_pci_irq_vector);

STATIC int devdrv_init_3559_alloc_irq(struct devdrv_pci_ctrl *pci_ctrl)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
    int irq, i;
    irq = devdrv_pci_irq_vector(pci_ctrl->pdev, 0);
    if (irq < 0) {
        devdrv_err("Alloc irq interrupt failed. (dev_id=%u; ret=%d)\n", pci_ctrl->dev_id, irq);
        return -ENODEV;
    }

    for (i = 0; i < DEVDRV_MSI_MAX_VECTORS; i++) {
        pci_ctrl->msi_info[i].data = NULL;
        pci_ctrl->msi_info[i].callback_func = NULL;
    }

    if (request_irq((unsigned int)irq, devdrv_irq_main_func, IRQF_SHARED, "devdrv_irq_main_func", pci_ctrl) != 0) {
        devdrv_err("Request irq interrupt failed. (dev_id=%u)\n", pci_ctrl->dev_id);
        return -EIO;
    }

    pci_ctrl->msix_irq_num = DEVDRV_MSI_MAX_VECTORS;

    return 0;
#else
    devdrv_err("Request irq interrupt not supported. (dev_id=%u)\n", pci_ctrl->dev_id);

    return -ENODEV;
#endif
}

STATIC int devdrv_init_3559_interrupt(struct devdrv_pci_ctrl *pci_ctrl)
{
    void __iomem *msix_base = NULL;
    int i;
    int ret;

    /* ioremap product reg */
    if (devdrv_get_host_type() == HOST_TYPE_ARM_3559) {
        pci_ctrl->rc_reg_base = ioremap(DEVDRV_RC_REG_BASE_3559, 0x1000);
    } else {
        pci_ctrl->rc_reg_base = ioremap(DEVDRV_RC_REG_BASE_3519, 0x1000);
    }

    if (pci_ctrl->rc_reg_base == NULL) {
        devdrv_err("Ioremap 3559 reg failed. (dev_id=%u)\n", pci_ctrl->dev_id);
        return -ENODEV;
    }
    pci_ctrl->msi_addr = devdrv_ka_dma_alloc_coherent(&pci_ctrl->pdev->dev, 0x10, &pci_ctrl->msi_dma_addr, GFP_KERNEL);

    if (pci_ctrl->msi_addr == NULL) {
        devdrv_err("Dma alloc failed.\n");
        iounmap(pci_ctrl->rc_reg_base);
        pci_ctrl->rc_reg_base = NULL;
        return -ENODEV;
    }

    writel((u32)pci_ctrl->msi_dma_addr, pci_ctrl->rc_reg_base + DEVDRV_RC_MSI_ADDR);
    writel((u32)(((u64)pci_ctrl->msi_dma_addr) >> DEVDRV_ADDR_MOVE_32),
           pci_ctrl->rc_reg_base + DEVDRV_RC_MSI_UPPER_ADDR);
    writel(0xFFFFFFFFU, pci_ctrl->rc_reg_base + DEVDRV_RC_MSI_EN);

    /* write msi-x table */
    msix_base = pci_ctrl->msi_base + DEVDRV_MINI_MSI_X_OFFSET;
    for (i = 0; i < DEVDRV_MSI_MAX_VECTORS; i++) {
        writel((u32)(pci_ctrl->msi_dma_addr & DEVDRV_MSIX_ADDR_BIT), (msix_base + (long)i * DEVDRV_MSIX_TABLE_SPAN));
        writel((u32)(((u64)pci_ctrl->msi_dma_addr) >> DEVDRV_ADDR_MOVE_32),
               (msix_base + (long)i * DEVDRV_MSIX_TABLE_SPAN + DEVDRV_MSIX_TABLE_ADDRH));
        writel((unsigned int)i, (msix_base + (long)i * DEVDRV_MSIX_TABLE_SPAN + DEVDRV_MSIX_TABLE_NUM));
        writel(0, (msix_base + (long)i * DEVDRV_MSIX_TABLE_SPAN + DEVDRV_MSIX_TABLE_MASK));
    }
    ret = devdrv_init_3559_alloc_irq(pci_ctrl);
    if (ret != 0) {
        devdrv_err("Call devdrv_init_3559_alloc_irq failed. (dev_id=%u)\n", pci_ctrl->dev_id);
        devdrv_uinit_3559_interrupt(pci_ctrl);
    }

    return ret;
}

STATIC void devdrv_uinit_3559_interrupt(struct devdrv_pci_ctrl *pci_ctrl)
{
    devdrv_ka_dma_free_coherent(&pci_ctrl->pdev->dev, 0x10, pci_ctrl->msi_addr, pci_ctrl->msi_dma_addr);
    pci_ctrl->msi_addr = NULL;
    iounmap(pci_ctrl->rc_reg_base);
    pci_ctrl->rc_reg_base = NULL;
}

bool devdrv_is_tsdrv_irq(const struct devdrv_intr_info *intr, int irq)
{
    if (((irq >= intr->tsdrv_irq_vector2_base) && (irq < (intr->tsdrv_irq_vector2_base + intr->tsdrv_irq_vector2_num)))
        || ((irq >= intr->tsdrv_irq_base) && (irq < (intr->tsdrv_irq_base + intr->tsdrv_irq_num)))) {
        return true;
    }

    return false;
}

void devdrv_bind_irq(struct devdrv_pci_ctrl *pci_ctrl)
{
    int i;
    u32 cpu_id, irq;

    for (i = 0; i < pci_ctrl->msix_irq_num; i++) {
        if (devdrv_is_tsdrv_irq(&pci_ctrl->res.intr, i) == false) {
            cpu_id = cpumask_local_spread(i + pci_ctrl->dev_id, dev_to_node(&pci_ctrl->pdev->dev));
            irq = pci_ctrl->msix_ctrl.entries[i].vector;
            (void)irq_set_affinity_hint(irq, get_cpu_mask(cpu_id));
        }
    }
}

void devdrv_unbind_irq(struct devdrv_pci_ctrl *pci_ctrl)
{
    int i;
    u32 irq;

    for (i = 0; i < pci_ctrl->msix_irq_num; i++) {
        if (devdrv_is_tsdrv_irq(&pci_ctrl->res.intr, i) == false) {
            irq = pci_ctrl->msix_ctrl.entries[i].vector;
            (void)irq_set_affinity_hint(irq, NULL);
        }
    }
}

STATIC int devdrv_slave_dev_init_interrupt_normal(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_pci_ctrl *main_pci_ctrl = devdrv_get_pdev_main_davinci_dev(pci_ctrl->pdev);
    int davinci_dev_num = devdrv_get_davinci_dev_num_by_pdev(pci_ctrl->pdev);
    u32 offset;
    int i;

    if (main_pci_ctrl == NULL) {
        devdrv_err("main_pci_ctrl is invalid. (dev_id=%u)\n", pci_ctrl->dev_id);
        return -ENODEV;
    }
    if (davinci_dev_num <= 0) {
        devdrv_err("davinci_dev_num is invalid. (dev_id=%u)\n", pci_ctrl->dev_id);
        return -ENODEV;
    }
    pci_ctrl->msix_irq_num = main_pci_ctrl->vector_num / davinci_dev_num;
    offset = (u32)pci_ctrl->msix_irq_num * pci_ctrl->dev_id_in_pdev;

    for (i = 0; i < pci_ctrl->msix_irq_num; i++) {
        pci_ctrl->msix_ctrl.entries[i].vector = main_pci_ctrl->msix_ctrl.entries[offset + (u32)i].vector;
        pci_ctrl->msix_ctrl.entries[i].entry = main_pci_ctrl->msix_ctrl.entries[offset + (u32)i].entry;
    }
    pci_ctrl->shr_para->msix_offset = offset;
    pci_ctrl->msix_offset = offset;

    devdrv_info("Slave dev init interrupt. (dev_id=%u; msix num=%d)\n", pci_ctrl->dev_id, pci_ctrl->msix_irq_num);

    return 0;
}

int devdrv_init_interrupt_normal(struct devdrv_pci_ctrl *pci_ctrl)
{
    int davinci_dev_num = devdrv_get_davinci_dev_num_by_pdev(pci_ctrl->pdev);
    u32 i;
    int vector_num;

    if (davinci_dev_num <= 0) {
        devdrv_err("davinci_dev_num is invalid. (dev_id=%u)\n", pci_ctrl->dev_id);
        return -ENODEV;
    }
    /* request msix interrupt */
    for (i = 0; i < MSI_X_MAX_VECTORS; i++) {
        pci_ctrl->msix_ctrl.entries[i].entry = (u16)i;
    }
    vector_num = pci_enable_msix_range(pci_ctrl->pdev, pci_ctrl->msix_ctrl.entries,
        pci_ctrl->res.intr.min_vector * davinci_dev_num,
        pci_ctrl->res.intr.max_vector * davinci_dev_num);

    pci_ctrl->vector_num = vector_num;
    pci_ctrl->msix_irq_num = vector_num / davinci_dev_num;
    pci_ctrl->shr_para->msix_offset = 0;
    pci_ctrl->msix_offset = 0;

    devdrv_info("Device init interrupt. (dev_id=%u; davinci dev num=%d; vector_num=%d)\n",
        pci_ctrl->dev_id, davinci_dev_num, vector_num);

    if ((vector_num >= pci_ctrl->res.intr.min_vector * davinci_dev_num) &&
        (vector_num <= pci_ctrl->res.intr.max_vector * davinci_dev_num)) {
        return 0;
    }

    devdrv_err("vector_num is error. (dev_id=%u; vector_num=%d; min_vector=%d; max_vector=%d)\n",
        pci_ctrl->dev_id, vector_num, pci_ctrl->res.intr.min_vector, pci_ctrl->res.intr.max_vector);

    return -EINVAL;
}

STATIC int devdrv_init_interrupt(struct devdrv_pci_ctrl *pci_ctrl)
{
    int ret;

    if ((pci_ctrl->env_boot_mode == DEVDRV_MDEV_VF_PM_BOOT) ||
        (pci_ctrl->env_boot_mode == DEVDRV_MDEV_FULL_SPEC_VF_PM_BOOT)) {
        return 0;
    }
    if ((devdrv_get_host_type() == HOST_TYPE_ARM_3559) || (devdrv_get_host_type() == HOST_TYPE_ARM_3519)) {
        ret = devdrv_init_3559_interrupt(pci_ctrl);
    } else {
        if (devdrv_is_pdev_main_davinci_dev(pci_ctrl)) {
            ret = devdrv_init_interrupt_normal(pci_ctrl);
        } else {
            ret = devdrv_slave_dev_init_interrupt_normal(pci_ctrl);
        }

        if (ret == 0) {
            if (pci_ctrl->ops.bind_irq != NULL) {
                pci_ctrl->ops.bind_irq(pci_ctrl);
            }
        }
    }
    return ret;
}

STATIC bool devdrv_is_msix_can_disable(struct pci_dev *pdev)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 2, 8)
    struct msi_desc *entry = NULL;
    int action_num = 0;
    u32 i;

    for_each_msi_entry(entry, &pdev->dev) {
        if ((entry == NULL) || (entry->irq == 0)) {
            devdrv_err("Irq entry has been freed.\n");
            return false;
        }

        for (i = 0; i < entry->nvec_used; i++) {
            if (irq_has_action(entry->irq + i) != 0) {
                devdrv_warn("Irq action msi index is %d, entry->irq=%d.\n", i, entry->irq);
                action_num++;
            }
        }
    }

    if (action_num > 0) {
        return false;
    }
#endif
    return true;
}

int devdrv_uninit_interrupt(struct devdrv_pci_ctrl *pci_ctrl)
{
    if ((devdrv_get_host_type() == HOST_TYPE_ARM_3559) || (devdrv_get_host_type() == HOST_TYPE_ARM_3519)) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
        pci_disable_msix(pci_ctrl->pdev);
        pci_disable_msi(pci_ctrl->pdev);
#endif
        devdrv_uinit_3559_interrupt(pci_ctrl);
    } else {
        if (pci_ctrl->ops.unbind_irq != NULL) {
            pci_ctrl->ops.unbind_irq(pci_ctrl);
        }

        if (devdrv_is_pdev_main_davinci_dev(pci_ctrl) == false) {
            return 0;
        }

        if (devdrv_is_msix_can_disable(pci_ctrl->pdev) == true) {
            pci_disable_msix(pci_ctrl->pdev);
            pci_intx(pci_ctrl->pdev, 0);
        }
    }

    return 0;
}

STATIC int devdrv_dma_chan_remote_op(struct devdrv_dma_channel *dma_chan, u32 op_type, u32 sriov_flag)
{
    int ret;
    struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)dma_chan->dma_dev->drvdata;
    struct devdrv_msg_dev *msg_dev = pci_ctrl->msg_dev;
    struct devdrv_dma_chan_remote_op cmd_data = {0};

    /* fill the cmd */
    cmd_data.op = op_type;
    cmd_data.sriov_flag = sriov_flag;
    cmd_data.chan_id = dma_chan->chan_id;

    if (op_type == DMA_CHAN_REMOTE_OP_INIT) {
        cmd_data.pf_num = pci_ctrl->shr_para->ep_pf_index;
        cmd_data.vf_num = pci_ctrl->shr_para->vf_id;
        cmd_data.sq_desc_dma = (u64)dma_chan->sq_desc_dma;
        cmd_data.cq_desc_dma = (u64)dma_chan->cq_desc_dma;
        cmd_data.sq_depth = dma_chan->sq_depth;
        cmd_data.cq_depth = dma_chan->cq_depth;
        cmd_data.sqcq_side = dma_chan->dma_dev->sq_cq_side;
    }

    /* send the cmd */
    ret = devdrv_admin_msg_chan_send(msg_dev, DEVDRV_DMA_CHAN_REMOTE_OP, &cmd_data, sizeof(cmd_data), NULL, 0);
    if (ret != 0) {
        devdrv_err("Message send failed. (dev_id=%u; chan=%d; op_type=%d; ret=%d)\n",
                   msg_dev->pci_ctrl->dev_id, dma_chan->chan_id, op_type, ret);
    }
    return ret;
}

int devdrv_dma_chan_init(struct devdrv_dma_channel *dma_chan, u32 sriov_flag)
{
    return devdrv_dma_chan_remote_op(dma_chan, DMA_CHAN_REMOTE_OP_INIT, sriov_flag);
}

int devdrv_dma_chan_reset(struct devdrv_dma_channel *dma_chan, u32 sriov_flag)
{
    return devdrv_dma_chan_remote_op(dma_chan, DMA_CHAN_REMOTE_OP_RESET, sriov_flag);
}

int devdrv_dma_chan_err_proc(struct devdrv_dma_channel *dma_chan)
{
    return devdrv_dma_chan_remote_op(dma_chan, DMA_CHAN_REMOTE_OP_ERR_PROC, DEVDRV_SRIOV_DISABLE);
}

STATIC struct devdrv_dma_dev *devdrv_host_dma_init(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_dma_func_para para_in = {0};
    struct devdrv_dma_res *dma_res = &pci_ctrl->res.dma_res;
    u32 i;

    /* DMA dev init */
    para_in.drvdata = (void *)pci_ctrl;
    para_in.dev = &pci_ctrl->pdev->dev;
    para_in.io_base = dma_res->dma_addr;
    para_in.dma_chan_base = dma_res->dma_chan_addr;
    para_in.dev_id = pci_ctrl->dev_id;
    para_in.chan_num = dma_res->dma_chan_num;
    for (i = 0; i < para_in.chan_num; i++) {
        para_in.use_chan[i] = dma_res->use_chan[i];
    }
    para_in.chan_begin = dma_res->chan_start_id;
    para_in.dma_chan_begin = dma_res->dma_chan_start_id;
    para_in.done_irq_base = (u32)pci_ctrl->res.intr.dma_irq_base;
    para_in.err_irq_base = (u32)pci_ctrl->res.intr.dma_irq_base +
                           (u32)pci_ctrl->res.intr.dma_irq_num / DEVDRV_EACH_DMA_IRQ_NUM;
    para_in.err_flag = 1;
    para_in.dma_pf_num = pci_ctrl->res.dma_res.pf_num;
    para_in.dma_vf_num = pci_ctrl->res.dma_res.vf_id;
    para_in.dma_vf_en = pci_ctrl->virtfn_flag;
    para_in.chip_type = pci_ctrl->chip_type;

    return devdrv_dma_init(&para_in, DEVDRV_DMA_REMOTE_SIDE, pci_ctrl->func_id);
}

STATIC int devdrv_atu_rx_atu_proc(struct devdrv_pci_ctrl *pci_ctrl)
{
    int ret;

    ret = devdrv_get_rx_atu_info(pci_ctrl, PCIE_BAR_0);
    if (ret != 0) {
        devdrv_err("Get mem rx atu failed. (dev_id=%d; ret=%d)\n", pci_ctrl->dev_id, ret);
        return ret;
    }
    ret = devdrv_get_rx_atu_info(pci_ctrl, PCIE_BAR_2);
    if (ret != 0) {
        devdrv_err("Get mem rx atu failed. (dev_id=%d; ret=%d)\n", pci_ctrl->dev_id, ret);
        return ret;
    }
    ret = devdrv_get_rx_atu_info(pci_ctrl, PCIE_BAR_4);
    if (ret != 0) {
        devdrv_err("Get mem rx atu failed. (dev_id=%d; ret=%d)\n", pci_ctrl->dev_id, ret);
        return ret;
    }

    return ret;
}

void devdrv_guard_work_sched_immediate(struct devdrv_pci_ctrl *pci_ctrl)
{
    cancel_delayed_work(&pci_ctrl->guard_work);
    pci_ctrl->guard_work_delay = DEVDRV_GUARD_WORK_DELAY_MIN;
    schedule_delayed_work(&pci_ctrl->guard_work, 0);
}

STATIC void devdrv_guard_work_sched(struct work_struct *p_work)
{
    struct devdrv_pci_ctrl *pci_ctrl =
        container_of(p_work, struct devdrv_pci_ctrl, guard_work.work);
    u32 delay = pci_ctrl->guard_work_delay;

    devdrv_msg_chan_guard_work_sched(pci_ctrl);
    schedule_delayed_work(&pci_ctrl->guard_work, msecs_to_jiffies(delay));

    if (delay != DEVDRV_GUARD_WORK_DELAY_MAX) {
        delay = delay * DEVDRV_GUARD_WORK_DELAY_MULTI;
        delay = delay < DEVDRV_GUARD_WORK_DELAY_MAX ? delay : DEVDRV_GUARD_WORK_DELAY_MAX;
        pci_ctrl->guard_work_delay = delay;
    }
}

void devdrv_guard_work_init(struct devdrv_pci_ctrl *pci_ctrl)
{
    /* msg guard work */
    pci_ctrl->guard_work_delay = DEVDRV_GUARD_WORK_DELAY_MAX;
    INIT_DELAYED_WORK(&pci_ctrl->guard_work, devdrv_guard_work_sched);
    schedule_delayed_work(&pci_ctrl->guard_work, 0);
}

void devdrv_guard_work_uninit(struct devdrv_pci_ctrl *pci_ctrl)
{
    (void)cancel_delayed_work_sync(&pci_ctrl->guard_work);
}

#ifdef CFG_FEATURE_PCIE_PROTO_DIP /* Dependency Inversion Principle */
#define BAR_NUM 3
STATIC int devdrv_get_res_host_bar_addr(struct devdrv_pci_ctrl *pci_ctrl, u32 bar, u64 addr_in_bar, u64 len,
    u64 *bar_addr)
{
    phys_addr_t bar_base[BAR_NUM] = {pci_ctrl->rsv_mem_phy_base, pci_ctrl->io_phy_base, pci_ctrl->mem_phy_base};
    u64 bar_size[BAR_NUM] = {pci_ctrl->rsv_mem_phy_size, pci_ctrl->io_phy_size, pci_ctrl->mem_phy_size};
    u32 asd_bar_num = bar / 2; /* 2 we use 64bit addr bar */

    if (asd_bar_num >= BAR_NUM) {
        devdrv_err("Invalid bar id. (dev_id=%u; bar=%u)\n", pci_ctrl->dev_id, bar);
        return -EINVAL;
    }

    if (len > (bar_base[asd_bar_num] + bar_size[asd_bar_num] - addr_in_bar)) {
        devdrv_err("Invalid addr. (dev_id=%u; bar=%u; addr_in_bar=%llx; len=%llx)\n",
            pci_ctrl->dev_id, asd_bar_num, addr_in_bar, len);
        return -EINVAL;
    }

    *bar_addr = bar_base[asd_bar_num] + addr_in_bar;
    return 0;
}

STATIC int devdrv_soc_res_addr_decode(u32 udevid, u64 encode_addr, u64 len, u64 *addr)
{
    u32 index_id;
    u32 bar = (u32)(encode_addr >> 60); /* 60 bar offset */
    u64 addr_in_bar = encode_addr - ((u64)bar << 60); /* 60 high 4bit bar id */
    struct devdrv_pci_ctrl *pci_ctrl = NULL;

    (void)uda_udevid_to_add_id(udevid, &index_id);
    pci_ctrl = devdrv_pci_ctrl_get(index_id);
    if (pci_ctrl != NULL) {
        int ret = devdrv_get_res_host_bar_addr(pci_ctrl, bar, addr_in_bar, len, addr);
        devdrv_pci_ctrl_put(pci_ctrl);
        return ret;
    }

    devdrv_err("No pci ctrl. (index_id=%u)\n", index_id);
    return -EINVAL;
}

static int devdrv_soc_subsys_irq_inject(struct devdrv_pci_ctrl *pci_ctrl, u32 subid, int irq_type, u32 irq_num)
{
    struct res_inst_info inst;
    int ret;
    u32 i, irq, irq_request, udevid;

    uda_add_id_to_udevid(pci_ctrl->dev_id, &udevid);
    soc_resmng_inst_pack(&inst, udevid, TS_SUBSYS, subid);

    ret = soc_resmng_set_irq_num(&inst, irq_type, irq_num);
    if (ret != 0) {
        devdrv_err("Set irq_num failed. (devid=%u; subid=%u)\n", udevid, subid);
        return ret;
    }

    for (i = 0; i < irq_num; i++) {
        irq = pci_ctrl->msix_ctrl.next_entry++;
        if (irq >= (u32)pci_ctrl->vector_num) {
            devdrv_err("Overflow. (devid=%u; irq=%u; vector_num=%u)\n", udevid, irq, pci_ctrl->vector_num);
            return -EINVAL;
        }

        irq_request = pci_ctrl->msix_ctrl.entries[irq].vector;
        ret = soc_resmng_set_irq_by_index(&inst, irq_type, i, irq_request);
        if (ret != 0) {
            devdrv_err("Set irq failed. (devid=%u; tsid=%u; index=%u; irq=%u)\n", udevid, subid, i, irq_request);
            return ret;
        }

        ret = soc_resmng_set_hwirq(&inst, irq_type, irq_request, irq);
        if (ret != 0) {
            devdrv_err("Set irq failed. (devid=%u; tsid=%u)\n", udevid, subid);
            return ret;
        }

        devdrv_info("Set irq. (devid=%u; irq_type=%d; irq=%u; irq_request=%u)\n", udevid, irq_type, irq, irq_request);
    }

    return 0;
}


static int devdrv_soc_irq_inject(struct devdrv_pci_ctrl *pci_ctrl, struct res_sync_target *target, char *buf,
    u32 buf_len)
{
    u32 pos = 0;

    if ((buf_len % sizeof(struct res_sync_irq)) != 0) {
        devdrv_err("Invalid para. (buf_len=%u)\n", buf_len);
        return -EINVAL;
    }

    while (pos < buf_len) {
        struct res_sync_irq *sync = (struct res_sync_irq *)(buf + pos);
        if (target->scope == SOC_TS_SUBSYS) {
            int ret = devdrv_soc_subsys_irq_inject(pci_ctrl, target->id, sync->irq_type, sync->num);
            if (ret != 0) {
                return ret;
            }
        }
        pos += sizeof(*sync);
    }

    return 0;
}

static int _devdrv_sync_soc_res(struct devdrv_pci_ctrl *pci_ctrl, struct res_sync_target *target, char *buf,
    u32 buf_len)
{
    u32 out_len, udevid;
    int ret;

    uda_add_id_to_udevid(pci_ctrl->dev_id, &udevid);
    ret = devdrv_admin_msg_chan_send(pci_ctrl->msg_dev, DEVDRV_SYNC_SOC_RES,
        (const void *)target, sizeof(*target), buf, buf_len);
    if (ret != 0) {
        devdrv_err("Send failed. (index_id=%d; ret=%d)\n", pci_ctrl->dev_id, ret);
        return ret;
    }

    out_len = *(u32 *)buf;
    if (out_len > buf_len - sizeof(u32)) {
        devdrv_err("Outlen error. (index_id=%d; out_len=%u)\n", pci_ctrl->dev_id, out_len);
        return -EFAULT;
    }

    if (out_len == 0) {
        return 0;
    }

    if (target->type == SOC_IRQ_RES) {
        ret = devdrv_soc_irq_inject(pci_ctrl, target, buf + sizeof(u32), out_len);
    } else {
        ret = soc_res_inject(udevid, target, buf + sizeof(u32), out_len, devdrv_soc_res_addr_decode);
        devdrv_info("soc_res_inject set. (udevid=%d; out_len=%u)\n", udevid, out_len);
    }

    return ret;
}

static int devdrv_sync_soc_res(u32 udevid, struct res_sync_target *target, char *buf, u32 buf_len)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    int ret;
    u32 index_id;

    (void)uda_udevid_to_add_id(udevid, &index_id);
    if (index_id >= MAX_DEV_CNT) {
        devdrv_err("Invalid para. (index_id=%u)\n", index_id);
        return -EINVAL;
    }

    devdrv_info("devdrv_sync_soc_res. (udevid=%u)\n", udevid);
    pci_ctrl = devdrv_pci_ctrl_get(index_id);
    if (pci_ctrl != NULL) {
        ret = _devdrv_sync_soc_res(pci_ctrl, target, buf, buf_len);
        devdrv_pci_ctrl_put(pci_ctrl);
        return ret;
    }
    return -EFAULT;
}
#endif

static int devdrv_pack_master_id_to_uda(u32 index_id, struct uda_dev_para *uda_para)
{
    u32 master_id;
    int ret;

    ret = devdrv_get_master_devid_in_the_same_os_inner(index_id, &master_id);
    if (ret != 0) {
        devdrv_err("Get master_devid failed. (index_id=%u; ret=%d)\n", index_id, ret);
        return ret;
    }

    uda_para->master_id = master_id;
    devdrv_info("Pack master_devid. (index_id=%u; master_id=%u)\n", index_id, uda_para->master_id);

    return 0;
}

#ifdef CFG_FEATURE_PCIE_PROTO_DIP
#define DEVDRV_ADD_DAVINCI_NOTIFIER "devdrv_add_davinci"
STATIC int devdrv_add_davinci_notifier_func(u32 udevid, enum uda_notified_action action)
{
    int ret = 0;

    if (udevid >= MAX_DEV_CNT) {
        devdrv_err("Invalid para. (udevid=%u)\n", udevid);
        return -EINVAL;
    }

    if (devdrv_get_connect_protocol(udevid) == CONNECT_PROTOCOL_UB) {
        return 0;
    }

    if (action == UDA_INIT) {
        ret = soc_res_sync_d2h(udevid, devdrv_sync_soc_res);
        if (ret != 0) {
            devdrv_err("Sync fail. (udevid=%u)\n", udevid);
            return ret;
        }
    }
    devdrv_info("add_davinci_notifier_func success. (udevid=%u; action=%d; ret=%d)\n", udevid, (u32)action, ret);

    return 0;
}
#endif

STATIC int devdrv_add_davinci_dev(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct uda_dev_type uda_type = {0};
    struct uda_dev_para uda_para = {0};
    u32 davinci_devid = 0;
    int ret;

    if ((pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_VF) && (pci_ctrl->env_boot_mode != DEVDRV_MDEV_VF_VM_BOOT) &&
        (pci_ctrl->env_boot_mode != DEVDRV_MDEV_FULL_SPEC_VF_VM_BOOT)) {
        return 0;
    }

    uda_type.hw = UDA_DAVINCI;
    uda_type.object = UDA_ENTITY;
    uda_type.location = UDA_NEAR;
    uda_type.prop = UDA_REAL;

    uda_para.udevid = pci_ctrl->dev_id;
    uda_para.remote_udevid = pci_ctrl->remote_dev_id;
    uda_para.chip_type = pci_ctrl->chip_type;
    uda_para.dev = &pci_ctrl->pdev->dev;
    uda_para.pf_flag = (pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_VF) ? 0 : 1;
    ret = devdrv_pack_master_id_to_uda(pci_ctrl->dev_id, &uda_para);
    if (ret != 0) {
        devdrv_err("Add davinci_dev fail. (dev_id=%u; ret=%d)\n", pci_ctrl->dev_id, ret);
        return -EINVAL;
    }

    if (pci_ctrl->ops.set_udevid_reorder_para != NULL) {
        ret = pci_ctrl->ops.set_udevid_reorder_para(pci_ctrl);
        if (ret != 0) {
            devdrv_err("get guid info fail. (dev_id=%u; ret=%d)\n", pci_ctrl->dev_id, ret);
            return -EINVAL;
        }
    }

    ret = uda_add_dev(&uda_type, &uda_para, &davinci_devid);
    if (ret != 0) {
        devdrv_err("Add davinci_dev fail. (dev_id=%u; ret=%d)\n", pci_ctrl->dev_id, ret);
        return -EINVAL;
    }

    return 0;
}

STATIC int devdrv_remove_davinci_dev(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct uda_dev_type uda_type = {0};
    int ret;

    if ((pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_VF) && (pci_ctrl->env_boot_mode != DEVDRV_MDEV_VF_VM_BOOT) &&
        (pci_ctrl->env_boot_mode != DEVDRV_MDEV_FULL_SPEC_VF_VM_BOOT)) {
        /* for reboot uninit all module */
        (void)uda_dev_ctrl(pci_ctrl->dev_id, UDA_CTRL_REMOVE);
        return 0;
    }

    uda_type.hw = UDA_DAVINCI;
    uda_type.object = UDA_ENTITY;
    uda_type.location = UDA_NEAR;
    uda_type.prop = UDA_REAL;

    ret = uda_remove_dev(&uda_type, pci_ctrl->dev_id);
    if (ret != 0) {
        devdrv_err("Remove davinci_dev fail. (dev_id=%u; ret=%d)\n", pci_ctrl->dev_id, ret);
        return -EINVAL;
    }
    devdrv_info("Remove davinci_dev success. (dev_id=%u)\n", pci_ctrl->dev_id);
    return 0;
}

STATIC int devdrv_set_phy_dev_num_to_uda(u32 dev_num)
{
    int ret;

    ret = uda_set_detected_phy_dev_num(dev_num);
    if (ret != 0) {
        devdrv_err("Set_detected_phy_dev_num to uda fail. (ret=%d)\n", ret);
        return -EINVAL;
    }
    return 0;
}

STATIC void devdrv_hccs_link_info_pre_init(struct devdrv_pci_ctrl *pci_ctrl)
{
    int i;

    pci_ctrl->hccs_status = -1;
    for (i = 0; i < HCCS_GROUP_SUPPORT_MAX_CHIPNUM; i++) {
        pci_ctrl->hccs_group_id[i] = -1;
    }
}

STATIC void devdrv_hccs_link_info_post_init(struct devdrv_pci_ctrl *pci_ctrl)
{
    if (pci_ctrl->ops.get_hccs_link_info != NULL) {
        pci_ctrl->ops.get_hccs_link_info(pci_ctrl);
    }
}

STATIC int devdrv_mdev_pm_load_half_probe(struct devdrv_pci_ctrl *pci_ctrl)
{
    if ((pci_ctrl->env_boot_mode != DEVDRV_MDEV_VF_PM_BOOT) &&
        (pci_ctrl->env_boot_mode != DEVDRV_MDEV_FULL_SPEC_VF_PM_BOOT)) {
        return DEVDRV_BOOT_CONTINUE;
    }

    pci_ctrl->dma_dev = devdrv_host_dma_init(pci_ctrl);
    if (pci_ctrl->dma_dev == NULL) {
        devdrv_err("Mdev dma device init fail. (dev_id=%u)\n", pci_ctrl->dev_id);
        return -EINVAL;
    }
    pci_ctrl->dma_dev->dev_id = pci_ctrl->dev_id;
    pci_ctrl->dma_dev->pci_ctrl = pci_ctrl;

    devdrv_pci_ctrl_add(pci_ctrl->dev_id, pci_ctrl);

    pci_ctrl->load_half_probe = 1;
    devdrv_set_startup_status(pci_ctrl, DEVDRV_STARTUP_STATUS_FINISH);
    pci_ctrl->load_status_flag = DEVDRV_LOAD_HALF_PROBE_STATUS;

    devdrv_info("VF half probe success. (devid=%d;bdf=%02x.%02x.%d)\n", pci_ctrl->dev_id,
        pci_ctrl->pdev->bus->number, PCI_SLOT(pci_ctrl->pdev->devfn), PCI_FUNC(pci_ctrl->pdev->devfn));
    return 0;
}

#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0)

#define DEVDRV_MODULE_PATH_MAX_LEN 128UL
#define DEVDRV_MODULE_BASE_PATH "/sys/module/"
STATIC char g_devdrv_module_path[DEVDRV_MODULE_PATH_MAX_LEN] = {0};

STATIC bool devdrv_is_module_insert(const char *name)
{
    struct path module_path;
    bool result = false;
    int ret;

    g_devdrv_module_path[0] = '\0';
    ret = strcpy_s(g_devdrv_module_path, DEVDRV_MODULE_PATH_MAX_LEN, DEVDRV_MODULE_BASE_PATH);
    if (ret != 0) {
        devdrv_err("strcpy_s module base path failed. (ret=%d)\n", ret);
        return false;
    }

    if (strlen(g_devdrv_module_path) + strlen(name) >= DEVDRV_MODULE_PATH_MAX_LEN) {
        devdrv_err("String len error. (base_len=%lu; name_len=%lu)\n", strlen(g_devdrv_module_path),
            strlen(name));
        return false;
    }

    ret = strcat_s(g_devdrv_module_path, DEVDRV_MODULE_PATH_MAX_LEN - strlen(g_devdrv_module_path),
        name);
    if (ret != 0) {
        devdrv_err("strcat_s error. (base_len=%lu; name_len=%lu; ret=%d)\n", strlen(g_devdrv_module_path),
            strlen(name), ret);
        return false;
    }

    if (kern_path(g_devdrv_module_path, LOOKUP_FOLLOW, &module_path) == 0) {
        if (S_ISDIR(module_path.dentry->d_inode->i_mode)) {
            result = true;
        }
        path_put(&module_path);
    }

    return result;
}

#else

STATIC bool devdrv_is_module_insert(const char *name)
{
    if (find_module(name) != NULL) {
        return true;
    }
    return false;
}

#endif

struct mutex g_depend_module_mutex;

STATIC bool devdrv_is_all_module_online(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_depend_module *module_list = pci_ctrl->res.depend_info.module_list;
    struct devdrv_depend_module *module_info = NULL;
    u32 module_num = pci_ctrl->res.depend_info.module_num;
    u32 online_num = 0;
    u32 i;

    if (module_list == NULL) {
        devdrv_info("No need to check depend modules. (dev_id=%u)\n", pci_ctrl->dev_id);
        return true;
    }

    for (i = 0; i < module_num; i++) {
        module_info = &module_list[i];
        if (module_info->status == DEVDRV_MODULE_ONLINE) {
            online_num++;
        } else {
            mutex_lock(&g_depend_module_mutex);
            if (devdrv_is_module_insert(module_info->module_name) == true) {
                module_info->status = DEVDRV_MODULE_ONLINE;
                online_num++;
            }
            mutex_unlock(&g_depend_module_mutex);
        }
    }

    if (online_num == module_num) {
        return true;
    }
    return false;
}

STATIC void devdrv_show_unprobed_module(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct devdrv_depend_module *module_list = pci_ctrl->res.depend_info.module_list;
    struct devdrv_depend_module *module_info = NULL;
    u32 module_num = pci_ctrl->res.depend_info.module_num;
    u32 i;

    for (i = 0; i < module_num; i++) {
        module_info = &module_list[i];
        if (module_info->status == DEVDRV_MODULE_UNPROBED) {
            devdrv_err("Depend unprobed module. (dev_id=%d; name=%s)\n", pci_ctrl->dev_id, module_info->module_name);
        }
    }
}

STATIC int devdrv_check_depend_modules(struct devdrv_pci_ctrl *pci_ctrl)
{
    u32 timeout = DEVDRV_CHECK_MODULE_TIMEOUT;

    while (timeout != 0) {
        if (devdrv_is_all_module_online(pci_ctrl) == true) {
            devdrv_info("Wait depend modules finish. (dev_id=%u)\n", pci_ctrl->dev_id);
            break;
        }

        if (pci_ctrl->device_status == DEVDRV_DEVICE_REMOVE) {
            devdrv_warn("Device remove. (dev_id=%u)\n", pci_ctrl->dev_id);
            return -ENODEV;
        }

        msleep(100); /* 100ms */
        timeout--;
    }

    if (timeout == 0) {
        devdrv_err("Wait depend modules timeout. (dev_id=%u)\n", pci_ctrl->dev_id);
        devdrv_show_unprobed_module(pci_ctrl);
    }
    return 0;
}

void devdrv_set_resmng_hccs_link_status_and_group_id(struct devdrv_pci_ctrl *pci_ctrl)
{
    int ret;
    u32 i, hccs_status;
    u32 hccs_group_id[HCCS_GROUP_SUPPORT_MAX_CHIPNUM];

    hccs_status = pci_ctrl->hccs_status;
    for (i = 0; i < HCCS_GROUP_SUPPORT_MAX_CHIPNUM; i++) {
        hccs_group_id[i] = pci_ctrl->hccs_group_id[i];
    }

    ret = soc_resmng_set_hccs_link_status_and_group_id(pci_ctrl->dev_id, hccs_status, hccs_group_id,
        HCCS_GROUP_SUPPORT_MAX_CHIPNUM);
    if (ret != 0) {
        devdrv_err("set hccs link status and group id failed. (dev_id=%u)\n", pci_ctrl->dev_id);
    }
}

void devdrv_set_resmng_host_phy_match_flag(struct devdrv_pci_ctrl *pci_ctrl)
{
    void __iomem *base = NULL;
    int ret;
    u32 host_flag;

    base = pci_ctrl->res.phy_match_flag_addr;
    host_flag = readl(base);

    ret = soc_resmng_set_host_phy_mach_flag(pci_ctrl->dev_id, host_flag);
    if (ret != 0) {
        devdrv_err("set host phy mach flag failed. (dev_id=%u)\n", pci_ctrl->dev_id);
    }
}

void devdrv_set_resmng_pdev_by_devid(struct devdrv_pci_ctrl *pci_ctrl)
{
    int ret;

    ret = soc_resmng_set_pdev_by_devid(pci_ctrl->dev_id, pci_ctrl->pdev);
    if (ret != 0) {
        devdrv_err("set pdev by devid failed. (dev_id=%u)\n", pci_ctrl->dev_id);
    }
}

void devdrv_set_pcie_info_to_dbl(struct devdrv_pci_ctrl *pci_ctrl)
{
    devdrv_set_resmng_hccs_link_status_and_group_id(pci_ctrl);
    devdrv_set_resmng_host_phy_match_flag(pci_ctrl);
    devdrv_set_resmng_pdev_by_devid(pci_ctrl);
}

void devdrv_load_half_probe(struct devdrv_pci_ctrl *pci_ctrl)
{
    int ret;

    devdrv_info("devdrv_load_half_probe start. (dev_id=%d)\n", pci_ctrl->dev_id);

    devdrv_hccs_link_info_post_init(pci_ctrl);

    ret = devdrv_mdev_pm_load_half_probe(pci_ctrl);
    if (ret != DEVDRV_BOOT_CONTINUE) {
        return;
    }

    ret = devdrv_msg_init(pci_ctrl);
    if (ret != 0) {
        devdrv_err("Message dev init failed. (dev_id=%d; ret=%d)\n", pci_ctrl->dev_id, ret);
        return;
    }

    pci_ctrl->dma_dev = devdrv_host_dma_init(pci_ctrl);
    if (pci_ctrl->dma_dev == NULL) {
        devdrv_err("Dma device init. (dev_id=%u)\n", pci_ctrl->dev_id);
        goto msg_exit;
    }
    pci_ctrl->dma_dev->dev_id = pci_ctrl->dev_id;
    pci_ctrl->dma_dev->pci_ctrl = pci_ctrl;

    ret = devdrv_atu_rx_atu_proc(pci_ctrl);
    if (ret != 0) {
        devdrv_err("Rx atu proc failed. (dev_id=%d; ret=%d)\n", pci_ctrl->dev_id, ret);
        goto dma_exit;
    }

    devdrv_register_half_devctrl(pci_ctrl);

    devdrv_set_bar_wc_flag_inner(pci_ctrl->dev_id, 0);

    devdrv_pci_ctrl_add(pci_ctrl->dev_id, pci_ctrl);

    /* alloc common msg queue */
    ret = devdrv_alloc_common_msg_queue(pci_ctrl);
    if (ret != 0) {
        devdrv_err("Alloc common_queue failed. (dev_id=%d; ret=%d)\n", pci_ctrl->dev_id, ret);
        goto remove_pci_ctrl;
    }

    devdrv_guard_work_init(pci_ctrl);
    devdrv_set_host_phy_mach_flag_inner(pci_ctrl->dev_id, DEVDRV_HOST_PHY_MACH_FLAG);

    if (pci_ctrl->ops.init_virt_info != NULL) {
        pci_ctrl->ops.init_virt_info(pci_ctrl);
    }

#ifdef CFG_FEATURE_S2S
    /* if s2s init failed, just s2s func can not be used, continue probe */
    ret = devdrv_s2s_msg_chan_init(pci_ctrl);
    if (ret != 0) {
        devdrv_warn("Init s2s msg chan failed.(dev_id=%d;ret=%d)\n", pci_ctrl->dev_id, ret);
    }
#endif

    ret = devdrv_dev_online(pci_ctrl);
    if (ret != 0) {
        devdrv_err("Online init failed. (dev_id=%d; ret=%d)\n", pci_ctrl->dev_id, ret);
        goto guard_work_uninit;
    }
    if (pci_ctrl->ops.single_fault_init != NULL) {
        (void)pci_ctrl->ops.single_fault_init(pci_ctrl);
    }
    devdrv_set_pcie_channel_status(DEVDRV_PCIE_COMMON_CHANNEL_OK);

    ret = devdrv_pasid_non_trans_init(pci_ctrl);
    if (ret != 0) {
        devdrv_err("Init ammu msg chan failed. (dev_id=%d; ret=%d)\n", pci_ctrl->dev_id, ret);
        goto uda_add_dev_failed;
    }

    ret = devdrv_check_depend_modules(pci_ctrl);
    if (ret != 0) {
        devdrv_err("Check depend modules failed. (dev_id=%d; ret=%d)\n", pci_ctrl->dev_id, ret);
        goto skip_add_davinci_dev;
    }

    devdrv_set_pcie_info_to_dbl(pci_ctrl);
    ret = devdrv_add_davinci_dev(pci_ctrl);
    if (ret != 0) {
        devdrv_err("Add davinci dev failed. (dev_id=%d; ret=%d)\n", pci_ctrl->dev_id, ret);
        goto smmu_init_msg_chan_failed;
    }
    pci_ctrl->add_davinci_flag = 1;

skip_add_davinci_dev:
    pci_ctrl->load_half_probe = 1;

    devdrv_set_startup_status(pci_ctrl, DEVDRV_STARTUP_STATUS_FINISH);
    pci_ctrl->load_status_flag = DEVDRV_LOAD_HALF_PROBE_STATUS;

    devdrv_info("Half probe finish, probe success. (dev_id=%d; bdf=%02x:%02x.%d)\n", pci_ctrl->dev_id,
        pci_ctrl->pdev->bus->number, PCI_SLOT(pci_ctrl->pdev->devfn), PCI_FUNC(pci_ctrl->pdev->devfn));

    return;

smmu_init_msg_chan_failed:
    devdrv_pasid_non_trans_uninit(pci_ctrl);

uda_add_dev_failed:
    devdrv_dev_offline(pci_ctrl);

guard_work_uninit:
    devdrv_guard_work_uninit(pci_ctrl);
    devdrv_free_common_msg_queue(pci_ctrl);

remove_pci_ctrl:
    devdrv_pci_ctrl_del(pci_ctrl->dev_id, pci_ctrl);

dma_exit:
    devdrv_dma_exit(pci_ctrl->dma_dev, DEVDRV_SRIOV_DISABLE);
    pci_ctrl->dma_dev = NULL;

msg_exit:
    devdrv_msg_exit(pci_ctrl);
    return;
}

STATIC void devdrv_half_probe_irq_task(struct work_struct *p_work)
{
    struct devdrv_pci_ctrl *pci_ctrl = container_of(p_work, struct devdrv_pci_ctrl, half_probe_work);
    devdrv_set_pcie_channel_status(DEVDRV_PCIE_COMMON_CHANNEL_HALF_PROBE);
    if (pci_ctrl->device_status == DEVDRV_DEVICE_RESUME) {
        devdrv_set_device_status(pci_ctrl, DEVDRV_DEVICE_ALIVE);
        devdrv_load_half_resume(pci_ctrl);
    } else {
        devdrv_load_half_probe(pci_ctrl);
    }
}

void devdrv_notify_dev_init_status(struct devdrv_pci_ctrl *pci_ctrl)
{
    pci_ctrl->shr_para->host_interrupt_flag = DEVDRV_HOST_RECV_INTERRUPT_FLAG;
}

void devdrv_init_shr_info_after_half_probe(struct devdrv_pci_ctrl *pci_ctrl)
{
    pci_ctrl->shr_para->rc_msix_ready_flag = DEVDRV_MSIX_READY_FLAG;
}

irqreturn_t devdrv_half_probe_irq(int irq, void *data)
{
    struct devdrv_pci_ctrl *pci_ctrl = (struct devdrv_pci_ctrl *)data;

    if (pci_ctrl->shr_para->host_interrupt_flag == 0) {
        pci_ctrl->load_status_flag = DEVDRV_LOAD_SUCCESS_STATUS;
        schedule_work(&pci_ctrl->half_probe_work);
        /* notice device addr info for addr trans */
        devdrv_notify_dev_init_status(pci_ctrl);
        /* ensure interrupt ack written to the device */
        wmb();
    }
    return IRQ_HANDLED;
}

int devdrv_vf_half_probe(u32 index_id)
{
    struct devdrv_pci_ctrl *pci_ctrl = devdrv_get_top_half_pci_ctrl_by_id(index_id);
    if (pci_ctrl == NULL) {
        devdrv_err("Get pci_ctrl failed. (index_id=%u)\n", index_id);
        return -EINVAL;
    }

    if (pci_ctrl->ops.get_vf_dma_info == NULL) {
        devdrv_err("get_vf_dma_info is NULL. (index_id=%u; func_id=%u)\n", index_id, pci_ctrl->func_id);
        return -EINVAL;
    }

    pci_ctrl->ops.get_vf_dma_info(pci_ctrl);
    pci_ctrl->load_status_flag = DEVDRV_LOAD_SUCCESS_STATUS;

    devdrv_load_half_probe(pci_ctrl);
    if (pci_ctrl->load_half_probe == 0) {
        devdrv_err("Load half probe failed. (index_id=%u; func_id=%u)\n", index_id, pci_ctrl->func_id);
        return -EINVAL;
    }

    devdrv_info("Vf half probe finish. (index_id=%u; func_id=%u)\n", index_id, pci_ctrl->func_id);

    return 0;
}

STATIC void devdrv_vf_half_probe_task(struct work_struct *p_work)
{
    struct devdrv_pci_ctrl *pci_ctrl = container_of(p_work, struct devdrv_pci_ctrl, half_probe_work);

    if (devdrv_mdev_vm_init(pci_ctrl) != 0) {
        devdrv_err("mdev vm vpc init failed. (dev_id=%u)\n", pci_ctrl->dev_id);
        return;
    }

    (void)devdrv_vf_half_probe(pci_ctrl->dev_id);
}

#define DEVDRV_DMA_MSI_X_VF_VECTOR_BASE 16
#define DEVDRV_DMA_MSI_X_VF_VECTOR_NUM 24
int devdrv_vf_half_free(u32 index_id)
{
    struct devdrv_pci_ctrl *pci_ctrl = devdrv_get_top_half_pci_ctrl_by_id(index_id);
    if (pci_ctrl == NULL) {
        devdrv_err("Get pci_ctrl failed. (index_id=%u)\n", index_id);
        return -EINVAL;
    }
    devdrv_info("Vf half free start. (index_id=%u; func_id=%u)\n", index_id, pci_ctrl->func_id);
    devdrv_load_half_free(pci_ctrl);
    pci_ctrl->load_half_probe = 0;
    pci_ctrl->msix_ctrl.next_entry = DEVDRV_DMA_MSI_X_VF_VECTOR_BASE + DEVDRV_DMA_MSI_X_VF_VECTOR_NUM;
    pci_ctrl->load_status_flag = DEVDRV_LOAD_INIT_STATUS;
    devdrv_set_devctrl_startup_flag(index_id, DEVDRV_DEV_STARTUP_TOP_HALF_OK);
    devdrv_info("Vf half free end. (index_id=%u; func_id=%u)\n", index_id, pci_ctrl->func_id);

    return 0;
}

STATIC int devdrv_get_os_load_flag(const struct devdrv_pci_ctrl *pci_ctrl)
{
    int load_flag = (int)pci_ctrl->os_load_flag;

    devdrv_info("Get os load flag. (chip_type=%d; host_dev_id=%u; func_id=%u; load_flag=%d)\n",
                pci_ctrl->chip_type, pci_ctrl->dev_id, pci_ctrl->func_id, load_flag);

    return load_flag;
}

STATIC void devdrv_load_vf_half_probe_task(struct devdrv_pci_ctrl *pci_ctrl)
{
    static atomic_t cpu_start;
    int first_cpu = -1;
    int cpu;

    for_each_online_cpu(cpu) {
        if (first_cpu == -1) {
            first_cpu = cpu;
        }
        if (atomic_read(&cpu_start) <= cpu) {
            atomic_set(&cpu_start, cpu + 1);
            devdrv_info("Start half probe work. (devid=%u; cpu=%d).\n", pci_ctrl->dev_id, cpu);
            schedule_work_on(cpu, &pci_ctrl->half_probe_work);
            return;
        }
    }
    if (first_cpu == -1) {
        schedule_work(&pci_ctrl->half_probe_work);
        return;
    }
    // Schedule on first cpu if not cpu is found after search each online cpu
    // If there are 4 cpus, possible schedule sequence is 0 -> 2 -> 3 -> 4 -> 0 -> 2
    atomic_set(&cpu_start, first_cpu + 1);
    devdrv_info("Start half probe work. (devid=%u; cpu=%d).\n", pci_ctrl->dev_id, first_cpu);
    schedule_work_on(first_cpu, &pci_ctrl->half_probe_work);
    return;
}

STATIC int devdrv_prepare_half_probe(struct devdrv_pci_ctrl *pci_ctrl)
{
    int ret = 0;
    int load_flag = devdrv_get_os_load_flag(pci_ctrl);
    int load_irq = pci_ctrl->res.intr.device_os_load_irq;

    if (load_flag != 0) {
        ret = devdrv_load_device(pci_ctrl);
        if (ret != 0) {
            devdrv_err("Device os load failed. (dev_id=%d; ret=%d)\n", pci_ctrl->dev_id, ret);
            return ret;
        }
        ret = devdrv_register_irq_func((void *)pci_ctrl, load_irq, devdrv_load_irq, pci_ctrl, "devdrv_load_irq");
        if (ret != 0) {
            devdrv_load_exit(pci_ctrl);
            devdrv_err("devdrv_load_irq register failed. (dev_id=%d; ret=%d)\n", pci_ctrl->dev_id, ret);
            return ret;
        }
    } else {
        devdrv_info("Device do not need load os, wait device interrupt to start half probe. (dev_id=%u)\n",
                    pci_ctrl->dev_id);

        if (pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_VF) {
            devdrv_notify_dev_init_status(pci_ctrl);
            /* MDEV_VF_VM need to call devdrv_vf_half_probe, VF_PM wait for sriov init instance */
            if ((pci_ctrl->env_boot_mode == DEVDRV_MDEV_VF_VM_BOOT) ||
                (pci_ctrl->env_boot_mode == DEVDRV_MDEV_FULL_SPEC_VF_VM_BOOT)) {
                INIT_WORK(&pci_ctrl->half_probe_work, devdrv_vf_half_probe_task);
                devdrv_load_vf_half_probe_task(pci_ctrl);
            }
            return 0;
        } else {
            if (pci_ctrl->ops.link_speed_slow_to_normal != NULL) {
                pci_ctrl->ops.link_speed_slow_to_normal(pci_ctrl);
            }
            INIT_WORK(&pci_ctrl->half_probe_work, devdrv_half_probe_irq_task);
            ret = devdrv_register_irq_func((void *)pci_ctrl, load_irq, devdrv_half_probe_irq, pci_ctrl,
                "devdrv_half_probe_irq");
            if (ret != 0) {
                devdrv_err("devdrv_half_probe_irq register failed. (dev_id=%d; ret=%d)\n", pci_ctrl->dev_id, ret);
                return ret;
            }
        }
    }
    pci_ctrl->load_vector = load_irq;
    return ret;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 11, 0)
int devdrv_configure_extended_tags(struct pci_dev *dev)
{
    u32 cap;
    u16 ctl;
    int ret;

    ret = pcie_capability_read_dword(dev, PCI_EXP_DEVCAP, &cap);
    if (ret != 0) {
        return 0;
    }

    if (!(cap & PCI_EXP_DEVCAP_EXT_TAG)) {
        return 0;
    }

    ret = pcie_capability_read_word(dev, PCI_EXP_DEVCTL, &ctl);
    if (ret != 0) {
        return 0;
    }

    if (!(ctl & PCI_EXP_DEVCTL_EXT_TAG)) {
        devdrv_info("Enabling extended tags.\n");
        pcie_capability_set_word(dev, PCI_EXP_DEVCTL, PCI_EXP_DEVCTL_EXT_TAG);
    }

    return 0;
}
#endif

int devdrv_cfg_pdev(struct pci_dev *pdev)
{
    int ret;
#if defined(ASCEND910_93_EX) && defined(ENABLE_BUILD_PRODUCT)
    int pos;
    u32 status;
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 11, 0)
    struct pci_dev *pdev_t = NULL;
#endif

    ret = pci_enable_device_mem(pdev);
    if (ret != 0) {
        devdrv_err("Call pci_enable_device_mem failed. (ret=%d)\n", ret);
        return ret;
    }

    if (dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(DEVDRV_DMA_BIT_MASK_48)) != 0) {
        devdrv_info("dma_set_mask 48 bit not success.\n");
        if (dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(DEVDRV_DMA_BIT_MASK_64)) != 0) {
            devdrv_info("dma_set_mask 64 bit not success.\n");
            ret = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(DEVDRV_DMA_BIT_MASK_32));
            if (ret != 0) {
                devdrv_err("dma_set_mask 32 bit failed. (ret=%d)\n", ret);
                goto disable_device;
            }
        }
    }

    ret = pci_request_regions(pdev, "devdrv");
    if (ret != 0) {
        devdrv_err("Call pci_request_regions failed. (ret=%d)\n", ret);
        goto disable_device;
    }

    pci_set_master(pdev);

#if defined(ASCEND910_93_EX) && defined(ENABLE_BUILD_PRODUCT)
    pos = pdev->aer_cap;
    if (pos){
        devdrv_info("cleanup_aer_error_status.\n");
        pci_read_config_dword(pdev, pos + PCI_ERR_COR_STATUS, &status);
        pci_write_config_dword(pdev, pos + PCI_ERR_COR_STATUS, status);

        pci_read_config_dword(pdev, pos + PCI_ERR_UNCOR_STATUS, &status);
        pci_write_config_dword(pdev, pos + PCI_ERR_UNCOR_STATUS, status);
    }
#endif

    ret = pci_enable_pcie_error_reporting(pdev);
    if (ret != 0) {
        devdrv_info("Pcie reporting. (ret=%d)\n", ret);
    }

    /* pci_configure_device will config this after 4.11.0 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 11, 0)
    (void)devdrv_configure_extended_tags(pdev);

    if ((u32)PCI_FUNC(pdev->devfn) != 0) {
        /* network pf0 */
        pdev_t = devdrv_get_device_pf(pdev, NETWORK_PF_0);
        if (pdev_t) {
            (void)devdrv_configure_extended_tags(pdev_t);
        }

        /* network pf1 */
        pdev_t = devdrv_get_device_pf(pdev, NETWORK_PF_1);
        if (pdev_t) {
            (void)devdrv_configure_extended_tags(pdev_t);
        }
    }

#endif

    return 0;

disable_device:
    pci_disable_device(pdev);

    return ret;
}

void devdrv_uncfg_pdev(struct pci_dev *pdev)
{
    (void)pci_disable_pcie_error_reporting(pdev);
    pci_clear_master(pdev);
    pci_release_regions(pdev);
    pci_disable_device(pdev);
}

int devdrv_get_bbox_reservd_mem_inner(unsigned int index_id, unsigned long long *dma_addr, struct page **dma_pages,
                                unsigned int *len)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_ctrl *ctrl = NULL;

    if (index_id >= MAX_DEV_CNT) {
        devdrv_err("Input parameter is invalid. (index_id=%u)\n", index_id);
        return -EINVAL;
    }
    if ((dma_addr == NULL) || (dma_pages == NULL) || (len == NULL)) {
        devdrv_err("Input parameter is invalid. (index_id=%u)\n", index_id);
        return -EINVAL;
    }
    ctrl = devdrv_get_top_half_devctrl_by_id(index_id);
    if ((ctrl == NULL) || (ctrl->priv == NULL)) {
        devdrv_err("Get dev_ctrl failed. (index_id=%u)\n", index_id);
        return -EINVAL;
    }
    pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;

    *dma_addr = pci_ctrl->bbox_resv_dmaAddr;
    *dma_pages = pci_ctrl->bbox_resv_dmaPages;
    *len = pci_ctrl->bbox_resv_size;

    return 0;
}

int devdrv_get_bbox_reservd_mem(unsigned int udevid, unsigned long long *dma_addr, struct page **dma_pages,
                                unsigned int *len)
{
    u32 index_id;

    (void)uda_udevid_to_add_id(udevid, &index_id);
    return devdrv_get_bbox_reservd_mem_inner(index_id, dma_addr, dma_pages, len);
}
EXPORT_SYMBOL(devdrv_get_bbox_reservd_mem);

STATIC struct device *devdrv_get_pcie_dev(u32 devid, u32 vfid, u32 index_id)
{
    return devdrv_get_pci_dev_by_devid_inner(index_id);
}

STATIC int devdrv_get_pcie_device_info(u32 index_id, struct devdrv_base_device_info *dev_info)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    int ret;

    if (index_id >= MAX_DEV_CNT) {
        devdrv_err("Invalid para. (index_id=%u)\n", index_id);
        return -EINVAL;
    }

    pci_ctrl = devdrv_pci_ctrl_get(index_id);
    if (pci_ctrl == NULL) {
        devdrv_info("Can not get dev_ctrl. (index_id=%u)\n", index_id);
        return -EINVAL;
    }
    ret = devdrv_get_pcie_id_info_inner(index_id, dev_info);
    devdrv_pci_ctrl_put(pci_ctrl);

    return ret;
}

STATIC int devdrv_pcie_p2p_attr_op(struct devdrv_base_comm_p2p_attr *p2p_attr)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    int ret = -EINVAL;
    u32 index_devid;

    (void)uda_udevid_to_add_id(p2p_attr->devid, &index_devid);
    if (index_devid >= MAX_DEV_CNT) {
        devdrv_err("Invalid para. (index_devid=%u)\n", index_devid);
        return -EINVAL;
    }

    pci_ctrl = devdrv_pci_ctrl_get(index_devid);
    if (pci_ctrl == NULL) {
        devdrv_info("Can not get dev_ctrl. (devid=%u)\n", p2p_attr->devid);
        return -EINVAL;
    }

    switch (p2p_attr->op) {
        case DEVDRV_BASE_P2P_ADD:
            ret = devdrv_enable_p2p(current->tgid, p2p_attr->devid, p2p_attr->peer_dev_id);
            break;
        case DEVDRV_BASE_P2P_DEL:
            ret = devdrv_disable_p2p(current->tgid, p2p_attr->devid, p2p_attr->peer_dev_id);
            break;
        case DEVDRV_BASE_P2P_QUERY:
            if (devdrv_is_p2p_enabled(p2p_attr->devid, p2p_attr->peer_dev_id)) {
                *p2p_attr->status = DEVDRV_ENABLE;  // p2p is enable
            } else {
                *p2p_attr->status = DEVDRV_DISABLE; // p2p is disable
            }
            ret = 0;
            break;
        case DEVDRV_BASE_P2P_ACCESS_STATUS_QUERY:
            ret = devdrv_get_p2p_access_status(p2p_attr->devid, p2p_attr->peer_dev_id, p2p_attr->status);
            if (ret) {
                devdrv_err("Devdrv_get_p2p_access_status fail. (devid=%u; ret=%d)\n", p2p_attr->devid, ret);
            }
            break;
        case DEVDRV_BASE_P2P_CAPABILITY_QUERY:
            ret = devdrv_get_p2p_capability(p2p_attr->devid, p2p_attr->capability);
            if (ret) {
                devdrv_err("Devdrv_get_p2p_access_status fail. (devid=%u; ret=%d)\n", p2p_attr->devid, ret);
            }
            break;

        default:
            break;
    }
    devdrv_pci_ctrl_put(pci_ctrl);
    return ret;
}

STATIC int devdrv_pcie_register_rao_client(u32 dev_id, enum devdrv_rao_client_type type, u64 va, u64 len,
    enum devdrv_rao_permission_type perm)
{
    return -EOPNOTSUPP;
}

STATIC int devdrv_pcie_unregister_rao_client(u32 dev_id, enum devdrv_rao_client_type type)
{
    return -EOPNOTSUPP;
}

STATIC int devdrv_pcie_rao_read(u32 dev_id, enum devdrv_rao_client_type type, u64 offset, u64 len)
{
    return -EOPNOTSUPP;
}

STATIC int devdrv_pcie_rao_write(u32 dev_id, enum devdrv_rao_client_type type, u64 offset, u64 len)
{
    return -EOPNOTSUPP;
}

STATIC int devdrv_pcie_get_ub_dev_info(u32 dev_id, struct devdrv_ub_dev_info *eid_info, int *num)
{
    return -EOPNOTSUPP;
}

STATIC int devdrv_pcie_get_token_val(u32 dev_id, u32 *token_val)
{
    return -EOPNOTSUPP;
}

STATIC int devdrv_pcie_process_pasid_add(u32 dev_id, u64 pasid)
{
    return -EOPNOTSUPP;
}

STATIC int devdrv_pcie_process_pasid_del(u32 dev_id, u64 pasid)
{
    return -EOPNOTSUPP;
}

STATIC int devdrv_pcie_get_all_device_count(u32 *count)
{
    u32 index = 0;
    u32 i;

    for (i = 0; i < MAX_DEV_CNT; i++) {
        if (devdrv_check_probe_dev_bitmap(i)) {
            index++;
        }
    }
    *count = index;

    return 0;
}

STATIC int devdrv_pcie_get_device_probe_list(u32 *devids, u32 *count)
{
    u32 index = 0;
    u32 i, udevid;

    for (i = 0; i < MAX_DEV_CNT; i++) {
        devids[i] = DEVDRV_INVALID_PHY_ID;
        if (devdrv_check_probe_dev_bitmap(i)) {
            (void)uda_add_id_to_udevid(i, &udevid);
            devids[index] = udevid;
            index++;
        }
    }
    *count = index;

    return 0;
}

struct devdrv_comm_ops g_drv_pcie_ops = {
    .comm_type = DEVDRV_COMMNS_PCIE,
    .alloc_non_trans = devdrv_pci_msg_alloc_non_trans_queue,
    .free_non_trans = devdrv_pci_msg_free_non_trans_queue,
    .sync_msg_send = devdrv_pci_sync_msg_send,
    .register_common_msg_client = devdrv_pci_register_common_msg_client,
    .unregister_common_msg_client = devdrv_pci_unregister_common_msg_client,
    .common_msg_send = devdrv_pci_common_msg_send,
    .get_boot_status = devdrv_pci_get_device_boot_status,
    .get_host_phy_mach_flag = devdrv_pci_get_host_phy_mach_flag,
    .get_env_boot_type = devdrv_pci_get_env_boot_type,
    .set_msg_chan_priv = devdrv_pci_set_msg_chan_priv,
    .get_msg_chan_priv = devdrv_pci_get_msg_chan_priv,
    .get_msg_chan_devid = devdrv_pci_get_msg_chan_devid,
    .get_connect_type = devdrv_pci_get_connect_protocol,
    .get_pfvf_type_by_devid = devdrv_pci_get_pfvf_type_by_devid,
    .get_device_info = devdrv_get_pcie_device_info,
    .mdev_vm_boot_mode = devdrv_pci_is_mdev_vm_boot_mode,
    .sriov_support = devdrv_pci_is_sriov_support,
    .sriov_enable = devdrv_pcie_sriov_enable,
    .sriov_disable = devdrv_pcie_sriov_disable,
    .get_device = devdrv_get_pcie_dev,
    .get_dev_topology = devdrv_pcie_get_dev_topology,
    .p2p_attr_op = devdrv_pcie_p2p_attr_op,
    .hotreset_assemble = devdrv_pcie_hotreset_assemble,
    .prereset_assemble = devdrv_pcie_prereset,
    .rescan_atomic = devdrv_pcie_reinit_inner,
    .unbind_atomic = devdrv_pcie_unbind_atomic,
    .reset_atomic = devdrv_pcie_reset_atomic,    
    .remove_atomic = devdrv_pcie_remove_atomic,
    .register_rao_client = devdrv_pcie_register_rao_client,
    .unregister_rao_client = devdrv_pcie_unregister_rao_client,
    .rao_read = devdrv_pcie_rao_read,
    .rao_write = devdrv_pcie_rao_write,
    .get_all_device_count = devdrv_pcie_get_all_device_count,
    .get_device_probe_list = devdrv_pcie_get_device_probe_list,
    .get_ub_dev_info = devdrv_pcie_get_ub_dev_info,
    .get_token_val = devdrv_pcie_get_token_val,
    .add_pasid = devdrv_pcie_process_pasid_add,
    .del_pasid = devdrv_pcie_process_pasid_del,
};

STATIC int devdrv_probe(struct pci_dev *pdev, const struct pci_device_id *data, int dev_index)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_pdev_ctrl *pdev_ctrl = NULL;
    int ret;

    pci_ctrl = devdrv_kzalloc(sizeof(struct devdrv_pci_ctrl), GFP_KERNEL);
    if (pci_ctrl == NULL) {
        devdrv_err("pci_ctrl devdrv_kzalloc failed.\n");
        return -ENOMEM;
    }

    atomic_set(&pci_ctrl->ref_cnt, 0);

    devdrv_pci_ctrl_pre_init(pci_ctrl, pdev, data, dev_index);
    devdrv_hccs_link_info_pre_init(pci_ctrl);
    devdrv_mdev_sriov_capacity_init(pci_ctrl);
    devdrv_connect_protocol_init(pci_ctrl, data);
    devdrv_set_startup_status(pci_ctrl, DEVDRV_STARTUP_STATUS_INIT);
    devdrv_add_davinci_dev_to_pdev_list(pci_ctrl, dev_index);

    devdrv_set_pcie_channel_status(DEVDRV_PCIE_COMMON_CHANNEL_INIT);
    ret = devdrv_res_init(pci_ctrl);
    if (ret != 0) {
        devdrv_err("Call devdrv_res_init failed. (ret=%d)\n", ret);
        goto del_from_pdev_list;
    }

    devdrv_pcictrl_shr_para_init(pci_ctrl);
    devdrv_pci_ctrl_work_queue_init(pci_ctrl);

    /* init pci ctrl and alloc devid */
    ret = devdrv_register_pci_devctrl(pci_ctrl);
    if (ret != 0) {
        devdrv_err("Alloc devid failed. (ret=%d)\n", ret);
        goto res_uninit;
    }
    devdrv_set_communication_ops_status_inner(DEVDRV_COMMNS_PCIE, DEVDRV_COMM_OPS_TYPE_ENABLE, pci_ctrl->dev_id);
    if (dev_index == 0) {
        pdev_ctrl = (struct devdrv_pdev_ctrl *)pci_get_drvdata(pdev);
        pdev_ctrl->main_dev_id = pci_ctrl->dev_id;
    }

    if (pci_ctrl->ops.probe_wait != NULL) {
        pci_ctrl->ops.probe_wait((int)pci_ctrl->dev_id);
    }
    drvdrv_dev_startup_record(pci_ctrl->dev_id);
    drvdrv_dev_startup_report(pci_ctrl->dev_id);
    devdrv_set_probe_dev_bitmap(pci_ctrl->dev_id);
    devdrv_pdev_sid_init(pci_ctrl);
    devdrv_pasid_rbtree_init(pci_ctrl);

    ret = devdrv_init_interrupt(pci_ctrl);
    if (ret != 0) {
        devdrv_err("Init interrupt failed. (dev_id=%u; ret=%d)\n", pci_ctrl->dev_id, ret);
        goto delete_dev;
    }

    ret = devdrv_mdev_pm_init(pci_ctrl);
    if (ret != 0) {
        devdrv_err("Init mdev_vpc failed. (dev_id=%u; ret=%d)\n", pci_ctrl->dev_id, ret);
        goto release_interrupt;
    }

    pci_ctrl->ops.set_dev_shr_info(pci_ctrl);
    pci_ctrl->load_status_flag = DEVDRV_LOAD_INIT_STATUS;
    ret = devdrv_prepare_half_probe(pci_ctrl);
    if (ret != 0) {
        goto mdev_vpc_uninit;
    }
    devdrv_init_shr_info_after_half_probe(pci_ctrl);

    devdrv_set_devctrl_startup_flag(pci_ctrl->dev_id, DEVDRV_DEV_STARTUP_TOP_HALF_OK);

    devdrv_set_device_status(pci_ctrl, DEVDRV_DEVICE_ALIVE);
    pci_ctrl->module_exit_flag = DEVDRV_REMOVE_CALLED_BY_PRERESET;

    if (pci_ctrl->ops.shr_para_rebuild != NULL) {
        pci_ctrl->ops.shr_para_rebuild(pci_ctrl);
    }

#ifdef CFG_FEATURE_PCIE_SENTRY
    if (!try_module_get(THIS_MODULE)) {
        devdrv_warn("can not get module.\n");
    }
#endif
    devdrv_info("Probe driver ends, waiting for device os to start after the second half of initialization."
        "(devid=%u, bootmode=%d, virtfn_flag=%u)\n", pci_ctrl->dev_id, pci_ctrl->env_boot_mode, pci_ctrl->virtfn_flag);

    /* other process in low-half part (devdrv_load_half_probe) after device os loaded */
    return 0;

mdev_vpc_uninit:
    devdrv_mdev_pm_uninit(pci_ctrl);

release_interrupt:
    (void)devdrv_uninit_interrupt(pci_ctrl);

delete_dev:
    devdrv_set_communication_ops_status_inner(DEVDRV_COMMNS_PCIE, DEVDRV_COMM_OPS_TYPE_DISABLE, pci_ctrl->dev_id);
    devdrv_slave_dev_delete(pci_ctrl->dev_id);

res_uninit:
    devdrv_res_uninit(pci_ctrl);
    devdrv_pci_ctrl_work_queue_uninit(pci_ctrl);

del_from_pdev_list:
    devdrv_del_davinci_dev_from_pdev_list(pci_ctrl, dev_index);
    devdrv_kfree(pci_ctrl);
    pci_ctrl = NULL;

    return ret;
}

STATIC int devdrv_remove(struct devdrv_pci_ctrl *pci_ctrl);
STATIC int drv_pcie_probe(struct pci_dev *pdev, const struct pci_device_id *data)
{
    struct devdrv_pdev_ctrl *pdev_ctrl = NULL;
    u8 bus_num = pdev->bus->number;
    u8 device_num = PCI_SLOT(pdev->devfn);
    u8 func_num = PCI_FUNC(pdev->devfn);
    u32 chip_type = (u32)data->driver_data;
    int i, ret;
    int j;
    int davinci_dev_num = devdrv_get_pdev_davinci_dev_num(data->device, pdev->subsystem_device);

    devdrv_info("Probe driver IN. (chip_type=%u; davinci_dev_num=%d; bdf=%02x:%02x.%d)\n",
        chip_type, davinci_dev_num, bus_num, device_num, func_num);

    if ((pdev->subsystem_vendor != DEVDRV_PCI_SUBSYS_VENDOR) &&
        (pdev->subsystem_vendor != DEVDRV_PCI_SUBSYS_PRIVATE_VENDOR) &&
        (pdev->subsystem_vendor != DEVDRV_PCI_SUBSYS_PRIVATE_VENDOR_HX) &&
        (pdev->subsystem_vendor != DEVDRV_PCI_SUBSYS_PRIVATE_VENDOR_HK) &&
        (pdev->subsystem_vendor != DEVDRV_PCI_SUBSYS_PRIVATE_VENDOR_KL)) {
        devdrv_err("subsystem_vendor is invalid. (subsystem_device=0x%x; "
            "subsystem_vendor=0x%x)\n", pdev->subsystem_device, pdev->subsystem_vendor);
        return -EPERM;
    }

    ret = devdrv_cfg_pdev(pdev);
    if (ret != 0) {
        devdrv_err("Call devdrv_cfg_pdev failed. (ret=%d)\n", ret);
        return ret;
    }

    pdev_ctrl = devdrv_kzalloc(sizeof(struct devdrv_pdev_ctrl), GFP_KERNEL);
    if (pdev_ctrl == NULL) {
        devdrv_err("pdev_ctrl devdrv_kzalloc failed.\n");
        ret = -ENOMEM;
        goto uncfg_pdev;
    }

    pdev_ctrl->dev_num = davinci_dev_num;
    pci_set_drvdata(pdev, pdev_ctrl);

    for (i = 0; i < davinci_dev_num; i++) {
        ret = devdrv_probe(pdev, data, i);
        if (ret != 0) {
            devdrv_err("Device probe failed. (index=%d; davinci_dev_num in pdev=%d)\n",
                i, davinci_dev_num);
            goto release_pdev_ctrl;
        }
    }

    ret = devdrv_sysfs_init(pdev);
    if (ret != 0) {
        devdrv_err("Call devdrv_sysfs_init failed. (ret=%d)\n", ret);
    } else {
        pdev_ctrl->sysfs_flag = DEVDRV_SYSFS_ENABLE;
    }

    devdrv_info("Probe driver OUT. (chip_type=%u; device=%x; davinci_dev_num=%d; "
        "bdf=%02x:%02x.%d)\n", chip_type, data->device, davinci_dev_num, bus_num,
        device_num, func_num);

    return 0;

release_pdev_ctrl:
    for (j = i - 1; j >= 0; j--) {
        if (pdev_ctrl->pci_ctrl[j] != NULL) {
            (void)devdrv_remove(pdev_ctrl->pci_ctrl[j]);
            pdev_ctrl->pci_ctrl[j] = NULL;
        }
    }
    pci_set_drvdata(pdev, NULL);
    devdrv_kfree(pdev_ctrl);
    pdev_ctrl = NULL;
uncfg_pdev:
    devdrv_uncfg_pdev(pdev);

    return ret;
}

void devdrv_load_half_free(struct devdrv_pci_ctrl *pci_ctrl)
{
    u32 dev_id = pci_ctrl->dev_id;

    if (devdrv_is_mdev_pm_boot_mode_inner(dev_id) == true) {
        devdrv_info("Enter pm boot mode. (dev_id=%u;boot_type=%d\n", dev_id,
            devdrv_pci_get_env_boot_type(dev_id));
        devdrv_dma_exit(pci_ctrl->dma_dev, DEVDRV_SRIOV_DISABLE);
        pci_ctrl->dma_dev = NULL;
        return;
    }

    /* clear P2P and H2D txatu resource */
    devdrv_clear_p2p_resource(pci_ctrl->dev_id);
    devdrv_clear_h2d_txatu_resource(pci_ctrl->dev_id);

    if (pci_ctrl->add_davinci_flag == 1) {
        (void)devdrv_remove_davinci_dev(pci_ctrl);
        pci_ctrl->add_davinci_flag = 0;
    }

    if ((pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_PF) &&
        (pci_ctrl->module_exit_flag != DEVDRV_REMOVE_CALLED_BY_MODULE_EXIT)) {
        devdrv_set_device_status(pci_ctrl, DEVDRV_DEVICE_UDA_RM);
    }

#ifdef CFG_FEATURE_S2S
    devdrv_s2s_msg_chan_uninit(pci_ctrl);
#endif
    devdrv_pasid_non_trans_uninit(pci_ctrl);
    if (!devdrv_is_sentry_work_mode()) {
        devdrv_set_hccs_link_status(pci_ctrl->dev_id, 0x0);
        devdrv_set_host_phy_mach_flag_inner(pci_ctrl->dev_id, 0x0);
        devdrv_guard_work_uninit(pci_ctrl);
        devdrv_free_common_msg_queue(pci_ctrl);
    }

    if (pci_ctrl->ops.single_fault_uninit != NULL) {
        (void)pci_ctrl->ops.single_fault_uninit(pci_ctrl);
    }
    devdrv_dma_exit(pci_ctrl->dma_dev, DEVDRV_SRIOV_DISABLE);
    pci_ctrl->dma_dev = NULL;

    devdrv_msg_exit(pci_ctrl);
}

STATIC void devdrv_load_probe_free(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct pci_dev *pdev = pci_ctrl->pdev;

    if (pci_ctrl->bbox_resv_dmaPages != NULL) {
        if (pci_ctrl->bbox_resv_dmaAddr != 0) {
            hal_kernel_devdrv_dma_unmap_page(&pdev->dev, (dma_addr_t)pci_ctrl->bbox_resv_dmaAddr, pci_ctrl->bbox_resv_size,
                DMA_BIDIRECTIONAL);
        }
        __devdrv_free_pages(pci_ctrl->bbox_resv_dmaPages, DEVDRV_BBOX_RESVED_MEM_ALLOC_PAGES_ORDER);
        pci_ctrl->bbox_resv_dmaPages = NULL;
        pci_ctrl->bbox_resv_dmaAddr = 0;
        pci_ctrl->bbox_resv_size = 0;
    }
    devdrv_set_devctrl_startup_flag(pci_ctrl->dev_id, DEVDRV_DEV_STARTUP_UNPROBED);

    devdrv_dma_desc_node_uninit(pci_ctrl);
    devdrv_mdev_vpc_uninit(pci_ctrl);
    (void)devdrv_uninit_interrupt(pci_ctrl);
    devdrv_pasid_rbtree_uninit(pci_ctrl);
    devdrv_pdev_sid_uninit(pci_ctrl);
    devdrv_pci_ctrl_work_queue_uninit(pci_ctrl);

    devdrv_set_ctrl_priv(pci_ctrl->dev_id, NULL);
    devdrv_res_uninit(pci_ctrl);
    devdrv_pci_ctrl_del(pci_ctrl->dev_id, pci_ctrl);

    devdrv_kfree(pci_ctrl);
    pci_ctrl = NULL;

    return;
}

STATIC int devdrv_remove(struct devdrv_pci_ctrl *pci_ctrl)
{
    struct pci_dev *pdev = pci_ctrl->pdev;
    struct devdrv_agent_load *agent_loader = NULL;
    u8 bus_num = pdev->bus->number;
    u8 device_num = PCI_SLOT(pdev->devfn);
    u8 func_num = PCI_FUNC(pdev->devfn);
    u32 dev_id = pci_ctrl->dev_id;
    int count = 0;

    agent_loader = pci_ctrl->agent_loader;
    devdrv_info("Remove driver start. (dev_id=%u; bdf:%02x:%02x.%d)\n", dev_id, bus_num, device_num, func_num);

    if (devdrv_get_product() != HOST_PRODUCT_DC) {
        /* shutdown scene doesn't need send offline msg */
        devdrv_info("No need send offline msg. (dev_id=%u)", dev_id);
    } else {
        if (devdrv_is_mdev_pm_boot_mode_inner(pci_ctrl->dev_id) == false) {
            devdrv_dev_offline(pci_ctrl);
        }
    }

    if (pci_ctrl->device_status != DEVDRV_DEVICE_DEAD) {
        devdrv_set_device_status(pci_ctrl, DEVDRV_DEVICE_REMOVE);
    }
    /* unregister device ready interrupt */
    if (pci_ctrl->load_vector >= 0) {
        (void)devdrv_unregister_irq_func((void *)pci_ctrl, pci_ctrl->load_vector, pci_ctrl);
        pci_ctrl->load_vector = -1;
    }
    msleep(10);
    /* wait half probe status */
    if (pci_ctrl->load_status_flag == DEVDRV_LOAD_SUCCESS_STATUS) {
        while (pci_ctrl->load_status_flag != DEVDRV_LOAD_HALF_PROBE_STATUS) {
            count++;
            if (count == DEVDRV_LOAD_HALF_WAIT_COUNT) {
                break;
            }
            msleep(10);
        }
    }
    devdrv_set_startup_status(pci_ctrl, DEVDRV_STARTUP_STATUS_INIT);
    devdrv_set_device_boot_status(pci_ctrl, DSMI_BOOT_STATUS_UNINIT);
    /* cancel work, when driver remove, should cancel device load work as early as possible */
    devdrv_load_exit(pci_ctrl);

    devdrv_set_dma_status(pci_ctrl->dma_dev, DEVDRV_DMA_DEAD);
    /* report remove to dev manager */
    drvdrv_dev_state_notifier(pci_ctrl);
    devdrv_clr_probe_dev_bitmap(pci_ctrl->dev_id);

    if ((pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_PF) &&
        (pci_ctrl->module_exit_flag != DEVDRV_REMOVE_CALLED_BY_MODULE_EXIT)) {
        devdrv_set_device_status(pci_ctrl, DEVDRV_DEVICE_DEAD);
    }
    devdrv_set_pcie_channel_status(DEVDRV_PCIE_COMMON_CHANNEL_DEAD);
    if (pci_ctrl->load_half_probe != 0) {
        devdrv_load_half_free(pci_ctrl);
    }

    devdrv_slave_dev_delete(pci_ctrl->dev_id);

    devdrv_load_probe_free(pci_ctrl);
    devdrv_set_communication_ops_status_inner(DEVDRV_COMMNS_PCIE, DEVDRV_COMM_OPS_TYPE_DISABLE, dev_id);
    devdrv_info("Remove driver exit. (dev_id=%u; bdf=%02x:%02x.%d)\n", dev_id, bus_num, device_num, func_num);

    return 0;
}

void drv_pcie_remove(struct pci_dev *pdev)
{
    struct devdrv_pdev_ctrl *pdev_ctrl = (struct devdrv_pdev_ctrl *)pci_get_drvdata(pdev);
    int i;

    if (pdev_ctrl == NULL) {
        devdrv_info("pdev_ctrl is free\n");
        return;
    }

    if (pdev_ctrl->sysfs_flag == DEVDRV_SYSFS_ENABLE) {
        devdrv_sysfs_exit(pdev);
        pdev_ctrl->sysfs_flag = DEVDRV_SYSFS_DISABLE;
    }

    /* Some resources, such as interrupts, are requested by the main dev and therefore need to remove at last. */
    for (i = pdev_ctrl->dev_num - 1; i >= 0; i--) {
        if (pdev_ctrl->pci_ctrl[i] != NULL) {
            (void)devdrv_remove(pdev_ctrl->pci_ctrl[i]);
            pdev_ctrl->pci_ctrl[i] = NULL;
        }
    }

    pci_set_drvdata(pdev, NULL);
    devdrv_kfree(pdev_ctrl);
    pdev_ctrl = NULL;

    if (!devdrv_is_sentry_work_mode()) {
        devdrv_uncfg_pdev(pdev);
    }
}

struct pci_driver g_devdrv_driver_ver = {
    .name = g_devdrv_driver_name,
    .id_table = g_devdrv_tbl,
    .probe = drv_pcie_probe,
    .remove = drv_pcie_remove,
    .driver = {
        .name = "devdrv_device_driver",
        .pm = &g_devdrv_pm_ops,
    },
    .err_handler = &g_devdrv_err_handler,
    .shutdown = devdrv_shutdown,
};

void devdrv_init_dev_num(void)
{
    struct pci_dev *pdev = NULL;
    int dev_num = 0;
    int dev_num_total = 0;
    int id_num = devdrv_get_device_id_tbl_num();
    int i;

    for (i = 0; i < id_num; i++) {
        pdev = NULL;
        dev_num = 0;

        do {
            pdev = pci_get_device(g_devdrv_tbl[i].vendor, g_devdrv_tbl[i].device, pdev);
            if (pdev == NULL) {
                break;
            }
            dev_num += devdrv_get_pdev_davinci_dev_num(g_devdrv_tbl[i].device, pdev->subsystem_device);
        } while (1);

        devdrv_info("Findout device. (index=%d; device=%x; dev_num=%d)\n",
                    i, g_devdrv_tbl[i].device, dev_num);
        dev_num_total += dev_num;
    }

    g_pci_manage_device_num = dev_num_total;
    if (dev_num_total != 0) {
        (void)devdrv_set_phy_dev_num_to_uda((u32)dev_num_total);
        devdrv_info("Findout total device. (dev_num=%d)\n", dev_num_total);
    } else {
        devdrv_event("No device found. (dev_num=%d)\n", dev_num_total);
    }
}

int devdrv_get_dev_num(void)
{
    return g_pci_manage_device_num;
}
EXPORT_SYMBOL(devdrv_get_dev_num);

int devdrv_get_davinci_dev_num(void)
{
    return g_pci_manage_device_num;
}
EXPORT_SYMBOL(devdrv_get_davinci_dev_num);

STATIC void devdrv_set_irq_res_gear(u32 gear_val)
{
    g_res_gear.irq_res_gear = gear_val;
}

u32 devdrv_get_irq_res_gear(void)
{
    return g_res_gear.irq_res_gear;
}

void devdrv_parse_res_gear(void)
{
    char *config_file = devdrv_get_config_file();
    char gear_str[GEAR_STR_LEN] = {0};
    u32 gear_val = 0;
    u32 ret_tmp;
    int ret;

    ret_tmp = devdrv_get_env_value_from_file(config_file, "IRQ_RES_GEAR", gear_str, (u32)sizeof(gear_str));
    if (ret_tmp != 0) {
        devdrv_info("Cannot get IRQ_RES_GEAR env value, use default gear. (ret=%u)\n", ret_tmp);
        goto default_gear;
    }

    ret = kstrtou32(gear_str, 0, &gear_val);
    if (ret != 0) {
        devdrv_info("gear_str kstrtou32 invalid, use default gear. (ret=%d)\n", ret);
        goto default_gear;
    }

    if (gear_val > DEVDRV_RES_GEAR_MAX_VAL) {
        devdrv_info("Gear is invalid, use default gear. (gear_val=%u)\n", gear_val);
        goto default_gear;
    } else {
        devdrv_set_irq_res_gear(gear_val);
    }

    return;

default_gear:
    devdrv_set_irq_res_gear(DEVDRV_RES_GEAR_MAX_VAL);
}

#define PCIE_HOST_NOTIFIER "pcie_host"
STATIC int pcie_host_notifier_func(u32 udevid, enum uda_notified_action action)
{
#ifdef CFG_FEATURE_S2S
    if (action == UDA_HOTRESET) {
        devdrv_set_s2s_chan_pre_reset(udevid);
    }
#endif
    return 0;
}

STATIC int __init devdrv_init_module(void)
{
    struct uda_dev_type uda_type;
    int ret;

    devdrv_info("Insmod host driver. (type=\"%s\", product=\"%s\", driver_name=\"%s\")\n",
                type, product, g_devdrv_driver_name);
    ret = devdrv_register_communication_ops(&g_drv_pcie_ops);
    if (ret != 0) {
        devdrv_err("Register pcie communication ops failed. (ret=%d)\n", ret);
        return ret;
    }
    mutex_init(&g_depend_module_mutex);

    if (strcmp(type, "3559") == 0) {
        g_host_type = HOST_TYPE_ARM_3559;
    } else if (strcmp(type, "3519") == 0) {
        g_host_type = HOST_TYPE_ARM_3519;
    } else {
        g_host_type = HOST_TYPE_NORMAL;
    }

#ifdef CFG_FEATURE_S2S
    devdrv_s2s_rwsem_init();
#endif

    ret = devdrv_alloc_attr_info();
    if (ret != 0) {
        devdrv_err("Alloc attr info failed. (ret=%d)\n", ret);
        goto unregister_ops;
    }

    devdrv_init_dev_num();
    ret = devdrv_pci_ctrl_mng_init();
    if (ret != 0) {
        devdrv_err("Alloc g_pci_ctrl_mng failed. (ret=%d)\n", ret);
        goto free_attr_info;
    }

    ret = devdrv_ctrl_init();
    if (ret != 0) {
        devdrv_err("Devdrv_ctrl_init failed. (ret=%d)\n", ret);
        goto pci_ctrl_mng_uninit;
    }
    /*lint -e64 */
    ret = pci_register_driver(&g_devdrv_driver_ver);
    if (ret != 0) {
        devdrv_err("Insmod devdrv driver failed. (ret=%d)\n", ret);
        goto ctrl_uninit;
    }
    /*lint +e64 */

#ifdef CFG_FEATURE_PCIE_SENTRY
    ret = devdrv_register_sentry_node();
    if (ret != 0) {
        devdrv_err("register sentry node failed. (ret=%d)\n", ret);
        goto pci_driver_unregister;
    }
    devdrv_init_sentry_waitqueue_status();
#endif

    uda_davinci_near_real_entity_type_pack(&uda_type);
    ret = uda_notifier_register(PCIE_HOST_NOTIFIER, &uda_type, UDA_PRI0, pcie_host_notifier_func);
    if (ret != 0) {
        devdrv_err("Register uda notifier failed. (ret=%d)\n", ret);
        goto unregister_sentry_node;
    }

    devdrv_get_pcie_dump_ltssm_tracer_symbol();
#ifdef CFG_FEATURE_PCIE_PROTO_DIP
    ret = uda_notifier_register(DEVDRV_ADD_DAVINCI_NOTIFIER, &uda_type, UDA_PRI0, devdrv_add_davinci_notifier_func);
    if (ret != 0) {
        devdrv_err("Calling uda_notifier_register failed. (ret=%d)\n", ret);
        goto unregister_pcie_host_notifier;
    }
#endif

    devdrv_info("Get dev_num. (dev_num=%d)\n", devdrv_get_dev_num());
    return 0;

#ifdef CFG_FEATURE_PCIE_PROTO_DIP
unregister_pcie_host_notifier:
    (void)uda_notifier_unregister(PCIE_HOST_NOTIFIER, &uda_type);
#endif
unregister_sentry_node:
#ifdef CFG_FEATURE_PCIE_SENTRY
    devdrv_unregister_sentry_node();
pci_driver_unregister:
#endif
    pci_unregister_driver(&g_devdrv_driver_ver);
ctrl_uninit:
    devdrv_ctrl_uninit();
pci_ctrl_mng_uninit:
    devdrv_pci_ctrl_mng_uninit();
free_attr_info:
    devdrv_free_attr_info();
unregister_ops:
    devdrv_unregister_communication_ops(&g_drv_pcie_ops);
    return ret;
}
module_init(devdrv_init_module);

STATIC void __exit devdrv_exit_module(void)
{
    struct devdrv_pci_ctrl *pci_ctrl = NULL;
    struct devdrv_ctrl *ctrl = NULL;
    struct uda_dev_type uda_type;
    u32 i;

    devdrv_put_pcie_dump_ltssm_tracer_symbol();
    for (i = 0; i < MAX_DEV_CNT; i++) {
        ctrl = devdrv_get_top_half_devctrl_by_id(i);
        if ((ctrl == NULL) || (ctrl->priv == NULL)) {
            continue;
        }
        pci_ctrl = (struct devdrv_pci_ctrl *)ctrl->priv;
        pci_ctrl->module_exit_flag = DEVDRV_REMOVE_CALLED_BY_MODULE_EXIT;
    }

    uda_davinci_near_real_entity_type_pack(&uda_type);
#ifdef CFG_FEATURE_PCIE_PROTO_DIP
    (void)uda_notifier_unregister(DEVDRV_ADD_DAVINCI_NOTIFIER, &uda_type);
#endif
    (void)uda_notifier_unregister(PCIE_HOST_NOTIFIER, &uda_type);

#ifdef CFG_FEATURE_PCIE_SENTRY
    devdrv_unregister_sentry_node();
#endif
    pci_unregister_driver(&g_devdrv_driver_ver);
    devdrv_clients_instance_uninit();
    devdrv_pci_ctrl_mng_uninit();
    devdrv_free_attr_info();
    devdrv_unregister_communication_ops(&g_drv_pcie_ops);
    return;
}
module_exit(devdrv_exit_module);

MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
MODULE_DESCRIPTION("devdrv host pcie driver");
MODULE_LICENSE("GPL");
MODULE_SOFTDEP("pre: ascend_adapter");
