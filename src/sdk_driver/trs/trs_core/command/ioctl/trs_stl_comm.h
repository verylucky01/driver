/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef TRS_STL_COMM_H
#define TRS_STL_COMM_H

#include "drv_type.h"

#define TRS_MAX_AIC_STL_NUM 10U
struct trs_stl_launch_para {
    uint32_t tsid;
    uint32_t timeout;
};

struct trs_stl_query_para {
    uint32_t tsid;
    unsigned char aic_num;
    unsigned char aic_status[TRS_MAX_AIC_STL_NUM];
};

#endif
