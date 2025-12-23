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

#include <linux/workqueue.h>

#include "pbl/pbl_uda.h"
#include "pbl/pbl_runenv_config.h"
#include "hdcdrv_core.h"
#ifdef CFG_FEATURE_PFSTAT
#include "hdcdrv_pfstat.h"
#endif
#include "hdcdrv_host.h"
#include "hdcdrv_host_adapt.h"

bool hdcdrv_is_phy_dev(u32 devid)
{
    return uda_is_phy_dev(devid);
}

int hdcdrv_check_in_container(void)
{
    return (int)run_in_normal_docker();
}

u32 hdcdrv_get_container_id(void)
{
    u32 docker_id = HDCDRV_PHY_HOST_ID;
    int ret;
#ifndef CFG_FEATURE_NOT_SUPPORT_UDA
    if (hdcdrv_check_in_container() != 1) {
        return docker_id;
    }

    ret = uda_get_cur_ns_id(&docker_id);
#else
    if (devdrv_manager_container_is_in_container() != 1) {
        return docker_id;
    }
    ret = devdrv_manager_container_get_docker_id(&docker_id);
#endif
    if (ret != 0) {
        docker_id = HDCDRV_DOCKER_MAX_NUM;
    }

    return docker_id;
}

#define HDCDRV_HOST_NOTIFIER "hdc_host"
STATIC int hdcdrv_host_notifier_func(u32 udevid, enum uda_notified_action action)
{
    int ret = 0;

    if (udevid >= HDCDRV_SUPPORT_MAX_DEV) {
        hdcdrv_err("Invalid para. (udevid=%u)\n", udevid);
        return -EINVAL;
    }

    if (action == UDA_INIT) {
        ret = hdcdrv_init_instance(udevid, uda_get_device(udevid));
    } else if (action == UDA_UNINIT) {
        ret = hdcdrv_uninit_instance(udevid);
    } else if (action == UDA_SUSPEND) {
        ret = hdcdrv_suspend(udevid);
    } else if (action == UDA_RESUME) {
        ret = hdcdrv_resume(udevid, uda_get_device(udevid));
    } else {
#ifndef DRV_UT
        return 0;
#endif
    }

    hdcdrv_info("notifier action. (udevid=%u; action=%d; ret=%d)\n", udevid, (u32)action, ret);

    return ret;
}

int hdcdrv_host_init_module(void)
{
    struct uda_dev_type type;
    int ret;

    hdcdrv_module_param_init();
    hdcdrv_init_hotreset_param();

    ret = hdcdrv_init();
    if (ret != 0) {
        hdcdrv_err("Calling hdcdrv_init failed.\n");
        return ret;
    }

    hdcdrv_set_segment(hdcdrv_get_packet_segment());

    uda_davinci_near_real_entity_type_pack(&type);
    ret = uda_notifier_register(HDCDRV_HOST_NOTIFIER, &type, UDA_PRI1, hdcdrv_host_notifier_func);
    if (ret != HDCDRV_OK) {
        hdcdrv_uninit();
        hdcdrv_err("Calling uda_notifier_register failed.\n");
        return ret;
    }

#ifdef CFG_FEATURE_VFIO
    ret = vhdch_init();
    if (ret != HDCDRV_OK) {
        (void)uda_notifier_unregister(HDCDRV_HOST_NOTIFIER, &type);
        hdcdrv_uninit();
        hdcdrv_err("Calling vhdch_vmngh_init failed. (ret=%d)\n", ret);
        return ret;
    }
#endif

#ifdef CFG_FEATURE_PFSTAT
    (void)hdcdrv_pfstat_init();
#endif

#ifdef CFG_FEATURE_HDC_REG_MEM
    if (!try_module_get(THIS_MODULE)) {
        hdcdrv_warn("can not get module.\n");
    }
#endif
    return HDCDRV_OK;
}

void hdcdrv_host_exit_module(void)
{
    struct uda_dev_type type;
#ifdef CFG_FEATURE_PFSTAT
	hdcdrv_pfstat_exit();
#endif
    uda_davinci_near_real_entity_type_pack(&type);

#ifdef CFG_FEATURE_VFIO
    vhdch_uninit();
#endif
    (void)hdcdrv_unregister_own_common_msg();
    (void)uda_notifier_unregister(HDCDRV_HOST_NOTIFIER, &type);
    hdcdrv_uninit_hotreset_param();
    hdcdrv_uninit();
}