/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef ASYNC_COPY_IOCTL_H
#define ASYNC_COPY_IOCTL_H

#include "svm_pub.h"

struct svm_async_copy_submit_para {
    u64 src_va; /* input */
    u64 dst_va; /* input */
    u64 size; /* input */
    u32 src_devid; /* input */
    u32 dst_devid; /* input */
    int src_host_tgid; /* input */
    int dst_host_tgid; /* input */
    int id; /* output */
    u64 rsv; /* reserve */
};

struct svm_async_copy_submit_2d_para {
    u64 src_va; /* input */
    u64 dst_va; /* input */
    u64 spitch; /* input */
    u64 dpitch; /* input */
    u64 width; /* input */
    u64 height; /* input */
    u32 src_devid; /* input */
    u32 dst_devid; /* input */
    int id; /* output */
    u64 rsv; /* reserve */
};

struct svm_async_copy_submit_batch_para {
    u64 *src_va; /* input */
    u64 *dst_va; /* input */
    u64 *size; /* input */
    u64 count; /* input */
    u32 src_devid; /* input */
    u32 dst_devid; /* input */
    int id; /* output */
    u64 rsv; /* reserve */
};

struct svm_async_copy_wait_para {
    int id; /* input */
    u64 rsv; /* reserve */
};

#endif

