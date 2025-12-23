/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef BBOX_DDR_DATA_H
#define BBOX_DDR_DATA_H

#include <stdbool.h>
#include "bbox_data_parser.h"

#define ELEM_OUTPUT_CHAR_LEN       1
#define ELEM_OUTPUT_SHORT_LEN      2
#define ELEM_OUTPUT_INT_LEN        4
#define ELEM_OUTPUT_LONG_LEN       8
#define ELEM_OUTPUT_HEX_MAX_LEN    32
#define ELEM_OUTPUT_DIVIDE_MAX_LEN 15
#define ELEMENT_NAME_MAX_LEN       32

enum model_element_type {
    /* output class */
    ELEM_OUTPUT_TYPE        = 0x0,
    ELEM_OUTPUT_BIN         = 0x1,  /* name = func(offset, size);    二进制bin文件保存 */
    ELEM_OUTPUT_STR         = 0x2,  /* name = func(offset, max_size); 换行字符串输出 */
    ELEM_OUTPUT_STR_NL      = 0x3,  /* name = func(offset, max_size); 不换行字符串输出 */
    ELEM_OUTPUT_HEX         = 0x4,  /* name = func(offset, size);    每个字节按hex输出，最多输出16个字节 */
    ELEM_OUTPUT_INT         = 0x5,  /* name = func(offset, size);    1,2,4,8字节整型输出 */
    ELEM_OUTPUT_CHAR        = 0x6,  /* name = func(offset, size);    按长度，字符输出 */
    ELEM_OUTPUT_INT_CONST   = 0x7,  /* name = value;                 整型:value(size) */
    ELEM_OUTPUT_STR_CONST   = 0x8,  /* name;                         字符串:value(0) size(0) */
    ELEM_OUTPUT_NL          = 0x9,  /* \n */
    ELEM_OUTPUT_DIVIDE      = 0xa,  /* ==========name========== */
    ELEM_OUTPUT_REG         = 0x20, /* name = func(offset, size);    1,2,4,8字节整型输出 */
    ELEM_OUTPUT_R4_BLOCK    = 0x23, /* name = func(offset, size);    4字节寄存器块，寄存器个数：size / 4 */
    ELEM_OUTPUT_MAX         = 0xFFF,
    /* feature class */
    ELEM_FEATURE_TYPE       = 0x1000,
    ELEM_FEATURE_TABLE      = 0x1001,
    ELEM_FEATURE_COMPARE    = 0x1002,
    ELEM_FEATURE_LOOPBUF    = 0x1003,
    ELEM_FEATURE_CHARLOG    = 0x1004,
    ELEM_FEATURE_LISTLOG    = 0x1005,
    ELEM_FEATURE_MAX        = 0x1FFF,
    /* control class */
    ELEM_CTRL_TYPE          = 0x2000,
    /* ELEM_FEATURE_TABLE 控制类 */
    ELEM_CTRL_TABLE         = 0x2001,
    ELEM_CTRL_TABLE_GOTO    = 0x2002, /* (table_enum_type, 0);跳转表的plaintext_table_type enum类型值，非显示项 */
    ELEM_CTRL_TABLE_RANGE   = 0x2003, /* (index_offset, index_cnt);子表开始地址和长度，非显示项 */
    /* ELEM_FEATURE_SWITH 控制类 */
    ELEM_CTRL_SWITCH        = 0x2050, /* (offset, size); switch key : switch value，最多10个case，非显示项 */
    ELEM_CTRL_OUT_CASE      = 0x2060, /* (value, 0); 显示项 */
                                      /* 当switch value == case value，则显示switch key : case key */
    ELEM_CTRL_OUT_DCASE     = 0x2061, /* (value, 0); 显示项 */
                                      /* 当switch value == default value，则显示switch key : case key */
    /* ELEM_FEATURE_COMPARE 控制类 */
    ELEM_CTRL_COMPARE       = 0x2100, /* (offset, size);需要比较的值所在位置和长度，非显示项 */
    ELEM_CTRL_CMP_JUMP_NE   = 0x2101, /* (compare_value, jump_index);如果不等于则跳转，非显示项 */
    ELEM_CTRL_CMP_JUMP_LE   = 0x2102, /* (compare_value, jump_index);如果不大于则跳转，非显示项 */
    ELEM_CTRL_CMP_JUMP_LT   = 0x2103, /* (compare_value, jump_index);如果小于则跳转，非显示项 */
    ELEM_CTRL_CMP_JUMP_GE   = 0x2104, /* (compare_value, jump_index);如果不小于则跳转，非显示项 */
    ELEM_CTRL_CMP_JUMP_GT   = 0x2105, /* (compare_value, jump_index);如果大于则跳转，非显示项 */
    ELEM_CTRL_CMP_JUMP_EQ   = 0x2106, /* (compare_value, jump_index);如果等于则跳转，非显示项 */
    ELEM_CTRL_CMP_MAX       = 0x2119,
    /* ELEM_FEATURE_LOOP 控制类 */
    ELEM_CTRL_LOOP          = 0x2150,
    /* ELEM_FEATURE_LOOP_BLOCK 控制类 */
    ELEM_CTRL_LOOP_BLOCK    = 0x2170, /* (offset, size);循环块 */
    ELEM_CTRL_BLOCK_VALUE   = 0x2171, /* (block num, block size);每块的大小和总块数，非显示项 */
    ELEM_CTRL_BLOCK_TABLE   = 0x2172, /* (table_enum_type, 0);跳转表的plaintext_table_type enum类型值，非显示项 */
    /* ELEM_FEATURE_LOOPBUF 控制类 */
    ELEM_CTRL_LOOPBUF       = 0x2200,
    ELEM_CTRL_LPBF_READ     = 0x2201, /* name: out_put_func(offset, size); 循环buffer读指针在结构体中偏移位置 */
    ELEM_CTRL_LPBF_WRITE    = 0x2202, /* name: out_put_func(offset, size); 循环buffer写指针在结构体中偏移位置 */
    ELEM_CTRL_LPBF_SIZE     = 0x2203, /* name: out_put_func(offset, size); 循环buffer总大小在结构体中偏移位置 */
    ELEM_CTRL_LPBF_D_SIZE   = 0x2204, /* name: value; 循环buffer总长度，以固定值设置 */
    ELEM_CTRL_LPBF_ROLLBK   = 0x2205, /* (offset, size); roll-back标记位，标记buffer是否翻转，非显示项 */
    /* ELEM_FEATURE_LISTLOG 控制类 */
    ELEM_CTRL_LSTLOG        = 0x2300,
    ELEM_CTRL_LSTLOG_START  = 0x2301, /* (offset, size)或(value, 0); LISTLOG结构体的起始偏移位置，非显示项 */
    ELEM_CTRL_LSTLOG_MAGIC  = 0x2302, /* (value); magic标记位，标记LISTLOG_HEAD合法性，非显示项 */
    ELEM_CTRL_LSTLOG_CHECK  = 0x2303, /* (offset, size); 检查magic标记的位置，非显示项 */
    ELEM_CTRL_LSTLOG_STRPTR = 0x2304, /* (offset, size)或(value, 0); LISTLOG实际字符串的起始位置，非显示项 */
    ELEM_CTRL_LSTLOG_STRLEN = 0x2305, /* (offset, size); LISTLOG实际字符串的长度（不含\0），非显示项 */
    ELEM_CTRL_LSTLOG_BUFLEN = 0x2306, /* (offset, size)或(value, 0); LISTLOG实际字符串buffer长度（字节对齐），非显示项 */
    ELEM_CTRL_LSTLOG_NEXT   = 0x2307, /* (offset, size)或(value, 0); 下一个LISTLOG的偏移位置（STRPTR+BUFLEN），非显示项 */
    ELEM_CTRL_LSTLOG_STEP   = 0x2308, /* value; LISTLOG_HEAD偏移的单位长度（HDRLEN和BUFLEN的最大公约数），非显示项 */
    ELEM_CTRL_MAX           = 0x2FFF,
};

enum elem_condition_type {
    ELEM_EQUAL   = 1 << 0, // 001
    ELEM_GRATER  = 1 << 1, // 010
    ELEM_LESS    = 1 << 2, // 100
};

struct model_element {
    char name[ELEMENT_NAME_MAX_LEN];
    enum model_element_type type;
    union {
        u32 arg1;           // common
        u32 offset;         // output offset
        u32 value;          // const value
        u32 num;            // blcok num
        enum plain_text_table_type table_enum_type;  // goto another table
        u32 index_offset;    // feature index offset
    };
    union {
        u32 arg2;           // common
        u32 size;           // output size | blcok size
        u32 max_size;        // string max length
        u32 split_char;      // char of split line
        u32 index_cnt;       // feature index count
    };
};

#define MODEL_VECTOR(NAME)           struct model_element MODEL_VECTOR_OBJECT_##NAME[]
#define MODEL_VECTOR_OBJECT(NAME)    (&MODEL_VECTOR_OBJECT_##NAME[0])
#define MODEL_VECTOR_ITEM(NAME, i)   (&MODEL_VECTOR_OBJECT_##NAME[i])
#define MODEL_VECTOR_SIZE(NAME)      (sizeof(MODEL_VECTOR_OBJECT_##NAME) / sizeof(struct model_element))
#define DEFINE_DATA_MODEL(name)      DATA_MODEL_##name

static inline bool compare_condition(enum model_element_type type, u32 condition)
{
    s32 cmp_type = (s32)type - (s32)ELEM_CTRL_COMPARE;
    return ((((u32)cmp_type) & condition) == 0);
}

static inline bool output_class(enum model_element_type type)
{
    return ((type >= ELEM_OUTPUT_TYPE) && (type < ELEM_OUTPUT_MAX));
}

static inline bool Control_class(enum model_element_type type)
{
    return ((type >= ELEM_CTRL_TYPE) && (type < ELEM_CTRL_MAX));
}

static inline bool Feature_class(enum model_element_type type)
{
    return ((type >= ELEM_FEATURE_TYPE) && (type < ELEM_FEATURE_MAX));
}

static inline bool Compare_class(enum model_element_type type)
{
    return ((type >= ELEM_CTRL_COMPARE) && (type < ELEM_CTRL_CMP_MAX));
}

#endif
