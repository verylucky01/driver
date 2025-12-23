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

#include <linux/module.h>
#include "module_host_init.h"

STATIC int __init asdrv_vmngh_init_module(void)
{
    int ret = 0;

    ret = vmngh_init_module();
    if (ret != 0) {
        vmng_err("Call vmngh_init_module failed. (ret=%d)\n", ret);
        return ret;
    }
    return 0;
}

STATIC void __exit asdrv_vmngh_exit_module(void)
{
    vmngh_exit_module();
}

module_init(asdrv_vmngh_init_module);
module_exit(asdrv_vmngh_exit_module);

MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
MODULE_DESCRIPTION("asdrv vmng driver");
MODULE_LICENSE("GPL");
