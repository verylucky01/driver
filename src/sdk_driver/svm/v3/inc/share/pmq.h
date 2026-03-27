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

#ifndef PMQ_H
#define PMQ_H

#include "svm_addr_desc.h"
#include "svm_pub.h"

/*
    PMQ: physical memory query
*/

/* queried pa seg is <= size accord to seg num space */
int svm_pmq_pa_query(int tgid, u64 va, u64 size, struct svm_pa_seg pa_seg[], u64 *seg_num);
int svm_pmq_pa_get(int tgid, u64 va, u64 size, struct svm_pa_seg pa_seg[], u64 *seg_num);
void svm_pmq_pa_put(struct svm_pa_seg pa_seg[], u64 seg_num);
int svm_pmq_query_va(int tgid, u64 pa, u64 start_va, u64 end_va, u64 *matched_va);

#endif
