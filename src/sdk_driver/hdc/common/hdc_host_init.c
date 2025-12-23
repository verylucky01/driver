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
#include "hdc_host_init.h"

STATIC int __init init_hdc_host(void)
{
    int ret;

	ret = hdcdrv_host_init_module();
	if (ret != 0) {
		return ret;
	}

#ifdef CFG_FEATURE_UB_COMM
	ret = hdcdrv_ub_init_module();
	if (ret != 0) {
		hdcdrv_host_exit_module();
	}
#endif

    return ret;
}

STATIC void __exit exit_hdc_host(void)
{
#ifdef CFG_FEATURE_UB_COMM
	hdcdrv_ub_exit_module();
#endif
	hdcdrv_host_exit_module();
}

module_init(init_hdc_host);
module_exit(exit_hdc_host);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huawei Tech. Co., Ltd.");