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
#include "pbl_feature_loader.h"
#include "pbl_uda.h"

#include "svm_kern_log.h"
#include "framework_dev.h"
#include "svm_host_dev.h"

static u32 host_udevid;

int svm_host_dev_init(void)
{
    int ret;

    host_udevid = uda_get_host_id();
    ret = svm_add_dev(host_udevid);
    if (ret != 0) {
        svm_err("Add dev failed. (host_udevid=%u)\n", host_udevid);
        return ret;
    }

    svm_info("Add host device instance. (host_udevid=%u)\n", host_udevid);

    return 0;
}
DECLAER_FEATURE_AUTO_INIT(svm_host_dev_init, FEATURE_LOADER_STAGE_8);

void svm_host_dev_uninit(void)
{
    int ret;

    ret = svm_del_dev(host_udevid);
    if (ret != 0) {
        svm_err("Del dev failed. (host_udevid=%u)\n", host_udevid);
    }
}
DECLAER_FEATURE_AUTO_UNINIT(svm_host_dev_uninit, FEATURE_LOADER_STAGE_8);

