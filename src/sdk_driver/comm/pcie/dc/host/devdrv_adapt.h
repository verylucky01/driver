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

#ifndef DEVDRV_ADAPT_H
#define DEVDRV_ADAPT_H

#include <linux/pci.h>
#include "comm_kernel_interface.h"

#define HOST_PRODUCT_DC 0

/* reserve mem pool for hdc */
#define DEVDRV_RESERVE_MEM_PHY_ADDR (0x0ULL)
#define DEVDRV_RESERVE_MEM_SIZE (0x0ULL)

/* devdrv_dma.h */
#if defined(CFG_PLATFORM_ESL) || defined(CFG_PLATFORM_FPGA) /* for fpga */
#define DEVDRV_DMA_COPY_TIMEOUT (HZ * 1000)
#define DEVDRV_DMA_COPY_MAX_TIMEOUT (HZ * 1000)
#define DEVDRV_DMA_WAIT_CHAN_AVAIL_TIMEOUT 20000
#else
/* Due to the impact of the previously unfinished dma task, the time-out period cannot
  be given based on the amount of data moved by the dma. Consider the influence of
  the PCIE bus bandwidth and the multi-channel of the DMA, giving a larger waiting time  */
#define DEVDRV_DMA_COPY_TIMEOUT (HZ * 50) /* 50s */
#define DEVDRV_DMA_COPY_MAX_TIMEOUT (HZ * 100) /* 100s */
/* wait for dma chan SQ queue when full */
#define DEVDRV_DMA_WAIT_CHAN_AVAIL_TIMEOUT 10000
#endif

/* 10s */
#define DEVDRV_DMA_QUERY_MAX_WAIT_TIME 10000000
/* 36s */
#define DEVDRV_DMA_QUERY_MAX_WAIT_LONG_TIME 36000000

/* devdrv_msg_def.h */
#if defined(CFG_PLATFORM_ESL) || defined(CFG_PLATFORM_FPGA)
#define DEVDRV_MSG_TIMEOUT 200000000       /* 200s for fpga  */
#define DEVDRV_ADMIN_MSG_TIMEOUT 200000000 /* 200s for fpga  */
#define DEVDRV_ADMIN_MSG_IRQ_TIMEOUT 50000000 /* 50s */
#define DEVDRV_MSG_WAIT_MIN_TIME 200       /* 200us */
#define DEVDRV_MSG_WAIT_MAX_TIME 400       /* 400us */
#define DEVDRV_ADMIN_MSG_WAIT_MIN_TIME 200 /* 200us */
#define DEVDRV_ADMIN_MSG_WAIT_MAX_TIME 400 /* 400us */
#else
#ifdef CFG_BUILD_ASAN
#define DEVDRV_MSG_TIMEOUT 60000000        /* 60s for asan */
#define DEVDRV_ADMIN_MSG_TIMEOUT 60000000  /* 60s for asan */
#else
#define DEVDRV_MSG_TIMEOUT 5000000        /* 5s */
#define DEVDRV_ADMIN_MSG_TIMEOUT 15000000 /* 15s for admin */
#endif
#define DEVDRV_ADMIN_MSG_IRQ_TIMEOUT 5000000 /* 5s for admin */
#define DEVDRV_MSG_WAIT_MIN_TIME 1        /* 1us */
#define DEVDRV_MSG_WAIT_MAX_TIME 2        /* 2us */
#define DEVDRV_ADMIN_MSG_WAIT_MIN_TIME 10 /* 10us */
#define DEVDRV_ADMIN_MSG_WAIT_MAX_TIME 11 /* 11us */
#endif

#define DEVDRV_ADMIN_MSG_TIMEOUT_LONG DEVDRV_ADMIN_MSG_TIMEOUT
#define DEVDRV_MSG_RETRY_LIMIT 5

extern int (*devdrv_res_init_func[HISI_CHIP_NUM])(struct devdrv_pci_ctrl *pci_ctrl);

/* devdrv_pcie_link_info.h */
void devdrv_peer_ctrl_init(void);

/* devdrv_pcie.c */
int devdrv_get_product(void);
extern const struct pci_device_id g_devdrv_tbl[];
extern const struct pci_error_handlers g_devdrv_err_handler;
int devdrv_get_device_id_tbl_num(void);
void devdrv_shutdown(struct pci_dev *pdev);

/* devdrv_dma.c */
void devdrv_traffic_and_manage_dma_chan_config(struct devdrv_dma_dev *dma_dev);

/* devdrv_pm.c */
bool devdrv_is_sentry_work_mode(void);
void devdrv_load_half_resume(struct devdrv_pci_ctrl *pci_ctrl);
int drv_pcie_suspend(struct device *dev);
int drv_pcie_resume_notify(struct device *dev);

#endif