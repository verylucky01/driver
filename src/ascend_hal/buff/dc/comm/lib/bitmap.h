/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BITMAP_H
#define BITMAP_H

typedef unsigned long bitmap_t;

void   bitmap_set(bitmap_t *map, int start, int nr);
void   bitmap_clear(bitmap_t *map, int start, int nr);
unsigned long bitmap_find_next_zero_area(bitmap_t *map, unsigned long size, unsigned long start,
    unsigned int  nr, unsigned long align_mask);
unsigned long bitmap_find_next_bit(const bitmap_t *map, unsigned long size, unsigned long offset);
#endif
