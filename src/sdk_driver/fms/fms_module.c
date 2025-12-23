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

#include "fms_module.h"
#include "pbl_feature_loader.h"

struct submodule_ops {
    int (*init) (void);
    void (*uninit)(void);
};

STATIC struct submodule_ops g_sub_table[] = {
    {dms_dtm_init, dms_dtm_exit},
    {dms_smf_init, dms_smf_exit},
#ifndef CFG_HOST_ENV
    {fpdc_receiver_init, fpdc_receiver_exit},
#endif
    {module_feature_auto_init, module_feature_auto_uninit},
};

STATIC int __init init_fms_base(void)
{
    int index, ret;
    int table_size = sizeof(g_sub_table) / sizeof(struct submodule_ops);
 
    for (index = 0; index < table_size; index++) {
        ret = g_sub_table[index].init();
        if  (ret != 0) {
            goto out;
        }
    }
    return 0;
out:
    for (; index > 0; index--) {
        g_sub_table[index - 1].uninit();
    }
    return ret;
}

STATIC void __exit exit_fms_base(void)
{
    int index;
    int table_size = sizeof(g_sub_table) / sizeof(struct submodule_ops);

    for (index = table_size; index > 0; index--) {
        g_sub_table[index - 1].uninit();
    }
}

module_init(init_fms_base);
module_exit(exit_fms_base);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
MODULE_DESCRIPTION("DAVINCI driver");