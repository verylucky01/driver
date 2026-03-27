/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef DMA_DESC_IOCTL_H
#define DMA_DESC_IOCTL_H

#include "ascend_hal_define.h"

#include "svm_pub.h"

struct svm_dma_desc_convert_para {
    u64 src_va; /* input */
    u64 dst_va; /* input */
    u64 size; /* input */
    u32 src_devid; /* input */
    u32 dst_devid; /* input */
    int src_host_tgid; /* input */
    int dst_host_tgid; /* input */
    struct DMA_ADDR dma_desc; /* output */
    u64 rsv; /* reserve */
};

struct svm_dma_desc_convert_2d_para {
    u64 src_va; /* input */
    u64 dst_va; /* input */
    u64 spitch; /* input */
    u64 dpitch; /* input */
    u64 width; /* input */
    u64 height; /* input */
    u32 src_devid; /* input */
    u32 dst_devid; /* input */
    u64 fixed_size; /* input */
    struct DMA_ADDR dma_desc; /* output */
    u64 rsv; /* reserve */
};

struct svm_dma_desc_submit_para {
    struct DMA_ADDR dma_desc; /* input */
    int sync_flag; /* input */
    u64 rsv; /* reserve */
};

struct svm_dma_desc_wait_para {
    struct DMA_ADDR dma_desc; /* input */
    u64 rsv; /* reserve */
};

struct svm_dma_desc_destroy_para {
    struct DMA_ADDR dma_desc; /* input */
    u64 rsv; /* reserve */
};

#endif
