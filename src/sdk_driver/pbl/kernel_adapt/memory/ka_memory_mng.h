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

#ifndef KA_MEMORY_MNG_H
#define KA_MEMORY_MNG_H

#include <linux/seq_file.h>

#ifndef __GFP_ACCOUNT

#ifdef __GFP_KMEMCG
#define __GFP_ACCOUNT __GFP_KMEMCG /* for linux version 3.10 */
#endif

#ifdef __GFP_NOACCOUNT
#define __GFP_ACCOUNT 0 /* for linux version 4.1 */
#endif

#endif

void ka_mem_mng_init(void);
void ka_mem_mng_uninit(void);
int ka_mem_stats_show(struct seq_file *seq, void *offset);
bool ka_is_enable_mem_record(void);
void ka_mem_record_status_reset(bool is_enable);
void ka_mem_alloc_stat_add(unsigned int module_id, size_t size, unsigned long va);
void ka_mem_alloc_stat_del(unsigned long va, unsigned int module_id);

#endif

