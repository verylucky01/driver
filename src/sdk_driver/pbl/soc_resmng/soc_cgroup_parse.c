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
#include <linux/vmalloc.h>
#include <linux/version.h>
#include "soc_resmng_log.h"
#include "soc_cgroup_parse.h"

#define CGOUP_CPUSET_PATH "/sys/fs/cgroup/cpuset/cpuset.cpus"
#define CPU_INFO_SIZE 256
#define CPU_BITMAP_LEN 32
#ifndef EMU_ST
STATIC int dbl_get_available_cpumask(const char *file_path, cpumask_var_t *cpumask)
{
    struct file *file = NULL;
    char *buf = NULL;
    loff_t pos = 0;
    int len, ret;

    /* read cpuset info from file */
    file = filp_open(file_path, O_RDONLY, 0);
    if (IS_ERR(file)) {
        soc_err("Failed to filp_open. \n");
        return -ENOENT;
    }
    /* kzalloc */
    buf = kzalloc(CPU_INFO_SIZE, GFP_KERNEL | __GFP_ACCOUNT);
    if (buf == NULL) {
        soc_err("Failed to kzalloc. \n");
        filp_close(file, NULL);
        file = NULL;
        return -ENOMEM;
    }
    /* read cpumask config */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 13, 0)
    len = kernel_read(file, buf, CPU_INFO_SIZE - 1, &pos);
#else
    len = kernel_read(file, pos, buf, CPU_INFO_SIZE - 1);
#endif
    filp_close(file, NULL);
    file = NULL;
    /* if len small or equal 0, return */
    if (len <= 0) {
        soc_err("Failed to len. \n");
        kfree(buf);
        buf = NULL;
        return -ENOENT;
    }
    /* alloc cpumask var */
    if (!zalloc_cpumask_var(cpumask, GFP_KERNEL)) {
        soc_err("Failed to zalloc_cpumask_var. \n");
        kfree(buf);
        buf = NULL;
        return -ENOMEM;
    }
    /* parse the cpumask */
    ret = cpulist_parse(buf, *cpumask);
    if (ret != 0) {
        free_cpumask_var(*cpumask);
    }
    kfree(buf);
    buf = NULL;
    return 0;
}
#endif

void dbl_get_available_cpu(u32 phy_bitmap, u32 *number, u32 *bitmap)
{
    cpumask_var_t available_cpumask;
    unsigned long bitmap_tmp = 0;
    u32 available_bitmap = 0;
    u32 i = 0;
    int ret;

    if (number == NULL) {
        soc_err("Input number NULL.\n");
        return;
    }

    if (bitmap == NULL) {
        soc_err("Input bitmap is NULL.\n");
        return;
    }

#ifndef EMU_ST
    ret = dbl_get_available_cpumask(CGOUP_CPUSET_PATH, &available_cpumask);
    if (ret != 0) {
        soc_err("Failed to get available cpumask. (ret=%d)\n", ret);
        return;
    }

    for_each_cpu(i, available_cpumask) {
        available_bitmap |= (1U << i);
    }
#endif

    (*bitmap) = phy_bitmap & available_bitmap;
    bitmap_tmp = (unsigned long)(*bitmap);
    *number = bitmap_weight(&bitmap_tmp, CPU_BITMAP_LEN);
    return;
}
EXPORT_SYMBOL_GPL(dbl_get_available_cpu);
