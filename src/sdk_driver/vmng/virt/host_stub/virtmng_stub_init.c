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

#include "pbl/pbl_uda.h"
#include "pbl/pbl_feature_loader.h"
#include "virtmng_public_def.h"
#include "virtmng_stub_init.h"

STATIC int vmng_stub_notifier_func(u32 udevid, enum uda_notified_action action)
{
    int ret = 0;

    if (action == UDA_INIT) {
        ret = module_feature_auto_init_dev(udevid);
        if (ret != 0) {
            vmng_err("Device auto init failed. (udevid=%u, ret=%d)\n", udevid, ret);
            return ret;
        }
    } else if (action == UDA_UNINIT) {
        module_feature_auto_uninit_dev(udevid);
    }
    vmng_info("notifier action. (udevid=%u; action=%d; ret=%d)\n", udevid, action, ret);
    return ret;
}

#define MIA_MNG_HOST_NOTIFIER "mia_mng"
STATIC int __init vmng_stub_init_module(void)
{
    struct uda_dev_type type;
    int ret;

    uda_davinci_near_real_entity_type_pack(&type);
    ret = uda_notifier_register(MIA_MNG_HOST_NOTIFIER, &type, UDA_PRI3, vmng_stub_notifier_func);
    if (ret != 0) {
        vmng_err("Register UDA notifier function failed. (ret=%d)\n", ret);
        return ret;
    }
    
    ret = module_feature_auto_init();
    if(ret != 0) {
        vmng_err("Feature auto init failed. (ret=%d)\n", ret);
        (void)uda_notifier_unregister(MIA_MNG_HOST_NOTIFIER, &type);
        return ret;
    }
    vmng_info("Init module finish.\n");
    return 0;
}
module_init(vmng_stub_init_module);

STATIC void __exit vmng_stub_exit_module(void)
{
    struct uda_dev_type type;
    int ret;

    module_feature_auto_uninit();

    uda_davinci_near_real_entity_type_pack(&type);
    ret = uda_notifier_unregister(MIA_MNG_HOST_NOTIFIER, &type);
    if(ret != 0) {
        vmng_err("Unregister UDA notifier function failed. (ret=%d)\n", ret);
    }
    vmng_info("Exit module finish.\n");
}
module_exit(vmng_stub_exit_module);

MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
MODULE_DESCRIPTION("virt mng host stub driver");
MODULE_LICENSE("GPL");