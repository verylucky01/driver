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

#include "apm_interface.h"
#include "dp_proc_mng_interface.h"
#include "rmo_interface.h"
#include "udis_interface.h"

struct submodule_ops {
    int (*init) (void);
    void (*uninit)(void);
};

int __attribute__((weak)) apm_init_module(void)
{
    return 0;
}

int __attribute__((weak)) dp_proc_mng_init(void)
{
    return 0;
}

void __attribute__((weak)) apm_exit_module(void)
{
    return;
}

void __attribute__((weak)) dp_proc_mng_exit(void)
{
    return;
}

void __attribute__((weak)) rmo_exit_module(void)
{
    return;
}

int __attribute__((weak)) rmo_init_module(void)
{
    return 0;
}

void __attribute__((weak)) udis_exit(void)
{
    return;
}

int __attribute__((weak)) udis_init(void)
{
    return 0;
}

static struct submodule_ops g_sub_table[] = {
    {apm_init_module, apm_exit_module},
    {dp_proc_mng_init, dp_proc_mng_exit},
    {rmo_init_module, rmo_exit_module},
    {udis_init, udis_exit},
};

static int __init init_dpa(void)
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

static void __exit exit_dpa(void)
{
    int index;
    int table_size = sizeof(g_sub_table) / sizeof(struct submodule_ops);
 
    for (index = table_size; index > 0; index--) {
        g_sub_table[index - 1].uninit();
    }
}

module_init(init_dpa);
module_exit(exit_dpa);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
MODULE_DESCRIPTION("DEVICE PUBLIC ADAPT");