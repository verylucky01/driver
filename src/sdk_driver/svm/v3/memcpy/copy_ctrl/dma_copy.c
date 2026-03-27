/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
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
#include "ka_base_pub.h"
#include "ka_system_pub.h"
#include "ka_compiler_pub.h"

#include "kernel_version_adapt.h"
#include "comm_kernel_interface.h"

#include "svm_pub.h"
#include "svm_kern_log.h"
#include "dma_copy.h"

#define SVM_DMA_DATA_PCIE_MSG_THRES_SIZE    SVM_BYTES_PER_KB
#define SVM_DMA_DATA_TRAFFIC_THRES_SIZE     (256ULL * SVM_BYTES_PER_KB)

#define SVM_DMA_WAIT_QUREY_THREAD_NUM       8U

#define SVM_DMA_WAIT_MIN_TIME               100U
#define SVM_DMA_WAIT_MAX_TIME               200U
#define SVM_DMA_RETRY_CNT                   5000U

static inline u32 svm_get_total_dma_node_size(struct devdrv_dma_node *dma_node, u32 node_cnt)
{
    u64 size = 0;
    u32 i;

    for (i = 0; i < node_cnt; i++) {
        size += dma_node[i].size;
    }

    return size;
}

static void svm_get_dma_copy_type(u32 size, enum devdrv_dma_data_type *type, int *wait_type)
{
    if (size < SVM_DMA_DATA_PCIE_MSG_THRES_SIZE) {
        *type = DEVDRV_DMA_DATA_PCIE_MSG;
        *wait_type = DEVDRV_DMA_WAIT_QUREY;
    } else if (size < SVM_DMA_DATA_TRAFFIC_THRES_SIZE) {
        *type = DEVDRV_DMA_DATA_TRAFFIC;
        *wait_type = DEVDRV_DMA_WAIT_QUREY;
    } else {
        *type = DEVDRV_DMA_DATA_TRAFFIC;
        *wait_type = DEVDRV_DMA_WAIT_INTR;
    }
}

static int _svm_dma_sync_cpy(u32 udevid, struct devdrv_dma_node *dma_nodes, u32 cnt,
    enum devdrv_dma_data_type type, int wait_type, u32 instance)
{
    int retry_cnt = 0;
    int ret;

    while (1) {
        ret = hal_kernel_devdrv_dma_sync_link_copy_plus(udevid, type, wait_type, (int)instance, dma_nodes, cnt);
        if (ka_likely(((ret == -ENOSPC) && (retry_cnt < SVM_DMA_RETRY_CNT)) == false)) {
            break;
        }

        /* dma queue is full, delay resubmit */
        ka_system_usleep_range(SVM_DMA_WAIT_MIN_TIME, SVM_DMA_WAIT_MAX_TIME);
        retry_cnt++;
    }
    if (ret != 0) {
        svm_err("hal_kernel_devdrv_dma_sync_link_copy_plus failed. (ret=%d; udevid=%u; node_cnt=%u)\n", ret, udevid, cnt);
    }

    return ret;
}

int svm_dma_sync_cpy(u32 udevid, struct devdrv_dma_node *dma_nodes, u32 cnt, u32 instance)
{
    enum devdrv_dma_data_type type;
    int ret, wait_type;
    static ka_atomic_t copy_num = KA_BASE_ATOMIC_INIT(0);

    svm_get_dma_copy_type(svm_get_total_dma_node_size(dma_nodes, cnt), &type, &wait_type);
    if (wait_type == DEVDRV_DMA_WAIT_QUREY) {
        if (ka_base_atomic_inc_return(&copy_num) >= SVM_DMA_WAIT_QUREY_THREAD_NUM) {
            wait_type = DEVDRV_DMA_WAIT_INTR;
            ka_base_atomic_dec(&copy_num);
        }
    }

    ret = _svm_dma_sync_cpy(udevid, dma_nodes, cnt, type, wait_type, instance);
    if (wait_type == DEVDRV_DMA_WAIT_QUREY) {
        ka_base_atomic_dec(&copy_num);
    }

    return ret;
}

#define SVM_MAX_UDEV_NUM    1140U   /* todo */

int svm_dma_async_cpy(u32 udevid, struct devdrv_dma_node *dma_nodes, u32 cnt,
    dma_finish_notify call_back, void *priv, u32 instance)
{
    static bool sync_flag[SVM_MAX_UDEV_NUM] = {0};
    struct devdrv_asyn_dma_para_info para_info;
    int ret;

    if (sync_flag[udevid]) {
        /* dma is very busy, direct call sync link cpy */
        ret = _svm_dma_sync_cpy(udevid, dma_nodes, cnt, DEVDRV_DMA_DATA_TRAFFIC, DEVDRV_DMA_WAIT_INTR, instance);
        if (ret == 0) {
            call_back(priv, (u32)instance, 0);
        }
        sync_flag[udevid] = false;
    } else {
        para_info.priv = priv;
        para_info.remote_msi_vector = 0;
        para_info.trans_id = (u32)instance;
        para_info.finish_notify = call_back;
        para_info.interrupt_and_attr_flag = DEVDRV_LOCAL_IRQ_FLAG;
        ret = hal_kernel_devdrv_dma_async_link_copy_plus(udevid, DEVDRV_DMA_DATA_TRAFFIC, (int)instance, dma_nodes, cnt, &para_info);
        if (ret == -ENOSPC) {
            sync_flag[udevid] = true;
            ret = _svm_dma_sync_cpy(udevid, dma_nodes, cnt, DEVDRV_DMA_DATA_TRAFFIC, DEVDRV_DMA_WAIT_INTR, instance);
            if (ret == 0) {
                call_back(priv, (u32)instance, 0);
            }
        }
    }

    return ret;
}

