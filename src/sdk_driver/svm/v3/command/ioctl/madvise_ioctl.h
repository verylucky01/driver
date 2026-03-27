/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef MADVISE_IOCTL_H
#define MADVISE_IOCTL_H

#include "svm_pub.h"

struct svm_madvise_para {
    u64 va; /* input */
    u64 size; /* input */
    u32 flag; /* input */
    u64 rsv; /* reserve */
};

#endif
