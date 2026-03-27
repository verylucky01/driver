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
#ifndef FRAMEWORK_DEV_H
#define FRAMEWORK_DEV_H

#include "pbl/pbl_davinci_api.h"
#include "davinci_interface.h"

#include "svm_pub.h"

/* for host def device */
int svm_add_dev(u32 udevid);
int svm_del_dev(u32 udevid);

void *svm_dev_ctx_get(u32 udevid);
void svm_dev_ctx_put(void *dev_ctx);

u32 svm_dev_obtain_feature_id(void); /* return feature_id */
int svm_dev_set_feature_priv(void *dev_ctx, u32 feature_id, const char *feature_name, void *priv);
void *svm_dev_get_feature_priv(void *dev_ctx, u32 feature_id);

static inline bool svm_device_status_is_matched(u32 udevid, u32 tar_state)
{
    struct ascend_intf_get_status_para status_para = {0};
    unsigned int status = 0;
    int ret;

    status_para.type = DAVINCI_STATUS_TYPE_DEVICE;
    status_para.para.device_id = udevid;
    ret = ascend_intf_get_status(status_para, &status);
    return ((ret != 0) || ((status & tar_state) == tar_state));
}

static inline bool svm_dev_status_is_link_abnormal(u32 udevid)
{
    return svm_device_status_is_matched(udevid, DAVINCI_INTF_DEVICE_STATUS_LINK_ABNORMAL);
}
#endif
