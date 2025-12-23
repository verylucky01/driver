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

#include "ka_kernel_def_pub.h"

#include "trs_chan.h"
#include "trs_id.h"

struct submodule_ops {
    int (*init) (void);
    void (*uninit)(void);
};
 
static struct submodule_ops g_sub_table[] = {
    {init_trs_id, exit_trs_id},
    {trs_chan_init_module, trs_chan_exit_module},
};
 
static int __init init_trs_base(void)
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
 
static void __exit exit_trs_base(void)
{
    int index;
    int table_size = sizeof(g_sub_table) / sizeof(struct submodule_ops);
 
    for (index = table_size; index > 0; index--) {
        g_sub_table[index - 1].uninit();
    }
}
 
ka_module_init(init_trs_base);
ka_module_exit(exit_trs_base);

KA_MODULE_LICENSE("GPL");
KA_MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
KA_MODULE_DESCRIPTION("TRS BASE");
