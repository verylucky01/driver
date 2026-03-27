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

#ifndef DBI_MSG_H
#define DBI_MSG_H

#include "svm_pub.h"
#include "dbi_def.h"
#include "kmc_msg.h"

struct svm_query_dbi_msg {
    struct svm_kmc_msg_head head;
    struct svm_device_basic_info dbi;
};

struct svm_update_dbi_msg {
    struct svm_kmc_msg_head head;
    struct svm_device_basic_info dbi;
};

#endif

