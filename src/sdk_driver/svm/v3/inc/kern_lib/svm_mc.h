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
#ifndef SVM_MEM_CLR_H
#define SVM_MEM_CLR_H

#include <linux/types.h>
#include "ka_common_pub.h"

/*
    mc: memory clear
*/

void svm_clear_single_page(ka_page_t *page, u64 page_size);
void svm_clear_pages(ka_page_t **pages, u64 page_num, u64 page_size);
int svm_clear_mem_by_uva(u32 udevid, int tgid, u64 va, u64 size);

void svm_register_mc_handle(int (* mc_handle)(u32 udevid, int tgid, u64 va, u64 size));

#endif

