/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "drv_buff_common.h"
#include "bitmap.h"

#define BITMAP_ALIGN_MASK(x, mask) (((x) + (mask)) & ~(mask))
#define BITMAP_LAST_WORD_MASK(nbits) \
(                                    \
    (((nbits) % BITS_PER_LONG) != 0) ?      \
        ((1UL<<((nbits) % BITS_PER_LONG)) - 1) : ~0UL  \
)

static unsigned int bitmap_ffs(bitmap_t word, unsigned int start, unsigned int end)
{
    unsigned int idx = start;
    bitmap_t tmp = word >> idx;

    do {
        if ((tmp & 0x1) != 0) {
            return idx;
        }
        tmp >>= 1;
        idx++;
    } while (idx < end);

    return (unsigned int)-1;
}

static unsigned int bitmap_ffz(bitmap_t word, unsigned int start, unsigned int end)
{
    unsigned int idx = start;
    bitmap_t tmp = word >> idx;

    do {
        if ((tmp & 0x1) == 0) {
            return idx;
        }
        tmp >>= 1;
        idx++;
    } while (idx < end);

    return (unsigned int)-1;
}

void bitmap_set(bitmap_t *map, int start, int nr)
{
    unsigned int start_map_line = (unsigned int)BIT_WORD(start);
    unsigned int size = (unsigned int)(start + nr);
    unsigned int end_map_line = ((size & (BITS_PER_LONG - 1)) != 0) ? BIT_WORD(size) : (BIT_WORD(size) - 1);
    bitmap_t *p = map + start_map_line;
    unsigned long set_mask = ~0UL << ((unsigned int)start & (BITS_PER_LONG - 1));

    if ((map == NULL) || (start < 0) || (nr <= 0)) {
        return;
    }

    /* the input para start and start + nr in the same map */
    if (start_map_line == end_map_line) {
        set_mask = set_mask & BITMAP_LAST_WORD_MASK(size);
        *p = *p | set_mask;
    } else {
        /* the first map */
        *p = *p | set_mask;
        p++;

        unsigned int idx;
        /* the 2 ~ n-1 maps  */
        for (idx = start_map_line + 1; idx < end_map_line; idx++) {
            *p = ~0UL;
            p++;
        }

        /* the last map */
        set_mask = BITMAP_LAST_WORD_MASK(size);
        *p = *p | set_mask;
    }

    return;
}

void bitmap_clear(bitmap_t *map, int start, int nr)
{
    unsigned int start_map_line = (unsigned int)BIT_WORD(start);
    unsigned int size = (unsigned int)(start + nr);
    unsigned int end_map_line = ((size & (BITS_PER_LONG - 1)) != 0) ? BIT_WORD(size) : (BIT_WORD(size) - 1);
    bitmap_t *p = map + start_map_line;
    unsigned long clear_mask = (1UL << ((unsigned int)start & (BITS_PER_LONG - 1))) - 1;

    if ((map == NULL) || (start < 0) || (nr <= 0)) {
        return;
    }

    /* the input para start and start + nr in the same map */
    if (start_map_line == end_map_line) {
        clear_mask = clear_mask | (~BITMAP_LAST_WORD_MASK(size));
        *p = *p & clear_mask;
    } else {
        /* the first map */
        *p = *p & clear_mask;
        p++;

        unsigned int idx;
        /* the 2 ~ n-1 maps */
        for (idx = start_map_line + 1; idx < end_map_line; idx++) {
            *p = 0UL;
            p++;
        }

        /* the last map */
        clear_mask = BITMAP_LAST_WORD_MASK(size);
        *p = *p & ~clear_mask;
    }

    return;
}

static unsigned long bitmap_find_next_zero_bit(const bitmap_t *map, unsigned long size, unsigned long offset)
{
    unsigned long start_map_line = BIT_WORD(offset);
    unsigned long end_map_line = (((size & (BITS_PER_LONG - 1)) != 0) ? BIT_WORD(size) : (BIT_WORD(size) - 1));
    bitmap_t tmp;
    unsigned long result;
    const bitmap_t *p = map + start_map_line;
    unsigned int end = (unsigned int)(((size & (BITS_PER_LONG - 1)) != 0) ?
                       (size & (BITS_PER_LONG - 1)) : BITS_PER_LONG);

    if (offset >= size) {
        return size;
    }

    /* there is onlyone map input */
    if (start_map_line == end_map_line) {
        tmp = *p;
        result = bitmap_ffz(tmp, offset & (BITS_PER_LONG - 1), end);
        if (result < BITS_PER_LONG) {
            return result + (start_map_line * BITS_PER_LONG);
        }
    } else {
        /* the first map */
        tmp = *(p++);
        result = bitmap_ffz(tmp, offset & (BITS_PER_LONG - 1), BITS_PER_LONG);
        if (result < BITS_PER_LONG) {
            return result + (start_map_line * BITS_PER_LONG);
        }

        unsigned long idx;
        /* the 2 ~ n - 1 maps */
        for (idx = start_map_line + 1; idx < end_map_line; idx++) {
            tmp = *(p++);
            if (tmp != ~0UL) {
                result = bitmap_ffz(tmp, 0, BITS_PER_LONG);
                return result + (idx * BITS_PER_LONG);
            }
        }

        /* the last map */
        tmp = *p;
        result = bitmap_ffz(tmp, 0, end);
        if (result < BITS_PER_LONG) {
            return result + (end_map_line * BITS_PER_LONG);
        }
    }

    return size;
}

unsigned long bitmap_find_next_bit(const bitmap_t *map, unsigned long size, unsigned long offset)
{
    unsigned long start_map_line = BIT_WORD(offset);
    unsigned long end_map_line = (size & (BITS_PER_LONG - 1)) != 0 ? BIT_WORD(size) : (BIT_WORD(size) - 1);
    const bitmap_t *p = map + start_map_line;
    bitmap_t tmp;
    unsigned long result;
    unsigned int end = (unsigned int)(((size & (BITS_PER_LONG - 1)) != 0) ?
                       (size & (BITS_PER_LONG - 1)) : BITS_PER_LONG);

    if (offset >= size) {
        return size;
    }

    /* there is onlyone map input */
    if (start_map_line == end_map_line) {
        tmp = *p;
        result = bitmap_ffs(tmp, offset & (BITS_PER_LONG - 1), end);
        if (result < BITS_PER_LONG) {
            return result + (start_map_line * BITS_PER_LONG);
        }
    } else {
        /* the first map */
        tmp = *(p++);
        result = bitmap_ffs(tmp, offset & (BITS_PER_LONG - 1), BITS_PER_LONG);
        if (result < BITS_PER_LONG) {
            return result + (start_map_line * BITS_PER_LONG);
        }

        unsigned long idx;
        /* the 2 ~ n - 1 maps */
        for (idx = start_map_line + 1; idx < end_map_line; idx++) {
            tmp = *(p++);
            if (tmp != 0UL) {
                result = bitmap_ffs(tmp, 0, BITS_PER_LONG);
                return result + (idx * BITS_PER_LONG);
            }
        }

        /* the last map */
        tmp = *p;
        result = bitmap_ffs(tmp, 0, end);
        if (result < BITS_PER_LONG) {
            return result + (end_map_line * BITS_PER_LONG);
        }
    }

    return size;
}

unsigned long bitmap_find_next_zero_area(bitmap_t *map, unsigned long size, unsigned long start,
    unsigned int nr, unsigned long align_mask)
{
    unsigned long index_zero, end, index_one, start_tmp;

    if (nr > (ULONG_MAX - start)) {
        return size;
    }
    end = start + nr;
    if ((end > size) || (map == NULL) || (size == 0)) {
        return size;
    }

    start_tmp = start;
    while (1) {
        index_zero = bitmap_find_next_zero_bit(map, size, start_tmp);

        /* Align allocation */
        index_zero = BITMAP_ALIGN_MASK(index_zero, align_mask);

        end = index_zero + nr;
        if ((end > size) || (nr > (ULONG_MAX - end))) {
            return size;
        }

        index_one = bitmap_find_next_bit(map, end, index_zero);
        if (index_one >= end) {
            return index_zero;
        }

        start_tmp = index_one + 1;
    }
}
