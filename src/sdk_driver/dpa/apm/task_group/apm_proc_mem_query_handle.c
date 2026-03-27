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
#include "ka_kernel_def_pub.h"

#include "ascend_hal_define.h"
#include "ascend_kernel_hal.h"
#include "apm_kern_log.h"
#include "dpa/dpa_apm_kernel.h"
#include "apm_proc_mem_query_handle.h"

typedef int (*proc_mem_query)(u32 udevid, int tgid, u64 *out_size);
static proc_mem_query g_proc_mem_query = NULL;

int apm_proc_mem_query_handle_register(int (*proc_mem_query)(u32 udevid, int tgid, u64 *out_size))
{
    g_proc_mem_query = proc_mem_query;
    return 0;
}
KA_EXPORT_SYMBOL_GPL(apm_proc_mem_query_handle_register);

void apm_proc_mem_query_handle_unregister(void)
{
    g_proc_mem_query = NULL;
}
KA_EXPORT_SYMBOL_GPL(apm_proc_mem_query_handle_unregister);

static int _apm_proc_mem_query(int tgid, u64 *out_size)
{
    u32 udevid, proc_type_bitmap;
    int master_tgid, mode;
    int ret;

    ret = apm_query_master_info_by_slave(tgid, &master_tgid, &udevid, &mode, &proc_type_bitmap);
    if (ret != 0) {
        apm_err("Query master info by slave failed. (slave_tgid=%d)\n", tgid);
        return ret;
    }

    return g_proc_mem_query(udevid, tgid, out_size);
}

int apm_proc_mem_query(int tgid, u64 *out_size)
{
    if (g_proc_mem_query != NULL) {
        return _apm_proc_mem_query(tgid, out_size);
    }

    /* return 0 not affect apm; until svm support new version */
    return -EPERM;
}

