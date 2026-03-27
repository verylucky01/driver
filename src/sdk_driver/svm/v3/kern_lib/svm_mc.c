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

#include "ka_system_pub.h"
#include "ka_common_pub.h"
#include "ka_sched_pub.h"

#include "securec.h"

#include "svm_kern_log.h"
#include "svm_gfp.h"
#include "svm_pub.h"
#include "svm_mc.h"

void svm_clear_single_page(ka_page_t *page, u64 page_size)
{
    if (ka_mm_PagePoisoned(page) == 0) {
        (void)memset_s(ka_mm_page_address(page), page_size, 0, page_size);
    }
}

void svm_clear_pages(ka_page_t **pages, u64 page_num, u64 page_size)
{
    unsigned long stamp = (unsigned long)ka_jiffies;
    u64 i;

    for (i = 0; i < page_num; ++i) {
        svm_clear_single_page(pages[i], page_size);
        ka_try_cond_resched(&stamp);
    }
}

static int (* clr_mem_by_uva)(u32 udevid, int tgid, u64 va, u64 size) = NULL;
void svm_register_mc_handle(int (* mc_handle)(u32 udevid, int tgid, u64 va, u64 size))
{
    clr_mem_by_uva = mc_handle;
}

int svm_clear_mem_by_uva(u32 udevid, int tgid, u64 va, u64 size)
{
    if (clr_mem_by_uva == NULL) {
        return -EFAULT;
    }

    return clr_mem_by_uva(udevid, tgid, va, size);
}