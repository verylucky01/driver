/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef MPL_IOCTL_H
#define MPL_IOCTL_H

#include "svm_pub.h"

struct svm_mpl_populate_para {
    u64 va; /* input */
    u64 size; /* input */
    u32 flag; /* input */
    u64 rsv; /* reserve */
};

struct svm_mpl_depopulate_para {
    u64 va; /* input */
    u64 size; /* input */
    u64 rsv; /* reserve */
};

#endif

