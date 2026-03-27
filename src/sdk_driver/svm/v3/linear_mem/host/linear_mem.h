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
 
#ifndef LINEAR_MEM_H
#define LINEAR_MEM_H

int linear_mem_get_access_va(u32 udevid, struct svm_global_va *src_va, struct casm_src_ex *src_ex, u64 *ex_info);
int linear_mem_register_ops(void);

#endif
