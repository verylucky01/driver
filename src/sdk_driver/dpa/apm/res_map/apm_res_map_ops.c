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
#include "ka_base_pub.h"
#include "ka_kernel_def_pub.h"
#include "ka_memory_pub.h"

#include "dpa/dpa_apm_kernel.h"
#include "apm_kern_log.h"
#include "apm_res_map_ops.h"

static struct apm_res_map_ops *res_ops[RES_ADDR_TYPE_MAX] = {NULL, };

int hal_kernel_apm_res_map_ops_register(enum res_addr_type res_type, struct apm_res_map_ops *ops)
{
    if ((res_type < 0) || (res_type >= RES_ADDR_TYPE_MAX)) {
        apm_err("Invalid para. (res_type=%d)\n", res_type);
        return -EINVAL;
    }

    res_ops[res_type] = ops;
    return 0;
}
KA_EXPORT_SYMBOL_GPL(hal_kernel_apm_res_map_ops_register);

void hal_kernel_apm_res_map_ops_unregister(enum res_addr_type res_type)
{
    if ((res_type < 0) || (res_type >= RES_ADDR_TYPE_MAX)) {
        return;
    }
    res_ops[res_type] = NULL;
}
KA_EXPORT_SYMBOL_GPL(hal_kernel_apm_res_map_ops_unregister);

bool apm_res_is_belong_to_proc(int master_tgid, int slave_tgid, u32 udevid, struct res_map_info_in *res_info)
{
    struct apm_res_map_ops *ops = res_ops[res_info->res_type];

    if ((ops == NULL) || (ops->res_is_belong_to_proc == NULL)) {
        return true;
    }

    return ops->res_is_belong_to_proc(master_tgid, slave_tgid, udevid, res_info);
}

int apm_get_res_addr(u32 udevid, struct res_map_info_in *res_info, u64 pa[], u32 num, u32 *len)
{
    struct apm_res_map_ops *ops = res_ops[res_info->res_type];
    int ret = 0;

    if ((ops == NULL) || ((ops->get_res_addr == NULL) && (ops->get_res_addr_array == NULL))) {
        apm_err("No handle. (res_type=%d)\n", res_info->res_type);
        return -EFAULT;
    }

    if (ops->get_res_addr != NULL) {
        ret = ops->get_res_addr(udevid, res_info, pa, len);
        if (ret != 0) {
            apm_err("Get res addr failed. (udevid=%u; res_type=%d; ret=%d)\n", udevid, res_info->res_type, ret);
            return -EFAULT;
        }

        if ((pa[0] == 0) || (*len == 0)) {
            apm_err("Invalid pa or len. (res_type=%d; len=%u; pa_is_NULL=%d)\n", res_info->res_type, *len, pa[0] == 0);
            return -EFAULT;
        }

        if ((pa[0] % KA_MM_PAGE_SIZE) + *len > KA_MM_PAGE_SIZE) {
            apm_err("Invalid pa and len. (res_type=%d; len=%u)\n", res_info->res_type, *len);
            return -EFAULT;
        }
        return 0;
    } else {
        u32 i, query_num;
        ret = ops->get_res_addr_array(udevid, res_info, pa, num, len);
        if (ret != 0) {
            apm_err("Get res addr array failed. (udevid=%u; res_type=%d; num=%u; ret=%d)\n",
                udevid, res_info->res_type, num, ret);
            return -EFAULT;
        }

        query_num = ka_base_round_up((pa[0] % KA_MM_PAGE_SIZE) + *len, KA_MM_PAGE_SIZE) / KA_MM_PAGE_SIZE;
        if ((query_num > num) || (*len == 0)) {
            apm_err("Invalid len. (res_type=%d; len=%u; query_num=%u)\n", res_info->res_type, *len, query_num);
            return -EFAULT;
        }

        /* 1. alloc_pages: pa continue, pa/size page aligned
         * 2. vmalloc: pa not continue, but each continuous pa aligned and size continue(alloc_page)
         * 3. kmalloc: pa continue, but pa/size not page aligned
         */
        for (i = 0; i < query_num; i++) {
            if ((((pa[i] % KA_MM_PAGE_SIZE) != 0) && (i != 0)) || (pa[i] == 0)) {
                apm_err("Invalid pa. (res_type=%d; len=%u; pa_is_NULL=%d)\n", res_info->res_type, *len, (pa[i] == 0));
                return -EFAULT;
            }
        }

        return 0;
    }
}

void apm_put_res_addr(u32 udevid, struct res_map_info_in *res_info, u64 pa[], u32 len)
{
    struct apm_res_map_ops *ops = res_ops[res_info->res_type];

    if ((ops == NULL) || (ops->put_res_addr_array == NULL)) {
        return;
    }

    ops->put_res_addr_array(udevid, res_info, pa, len);
}

int apm_update_res_info(u32 udevid, struct res_map_info_in *res_info)
{
    struct apm_res_map_ops *ops = res_ops[res_info->res_type];

    if ((ops == NULL) || (ops->update_res_info == NULL)) {
        return 0;
    }

    return ops->update_res_info(udevid, res_info);
}