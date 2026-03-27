/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026. All rights reserved.
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

#ifndef JPEGD_REG_UNION_DEFINE_H
#define JPEGD_REG_UNION_DEFINE_H

#include <linux/types.h>

union FrameSize {
    // Define the struct bits
    struct {
        uint32_t pixWidth : 16;  // [15..0]
        uint32_t pixHeight : 16; // [31..16]
    } bits;

    // Define an unsigned member
    uint32_t u32;
};

union CropHorizontal {
    // Define the struct bits
    struct {
        uint32_t pixStartHor : 16; // [15..0]
        uint32_t pixEndHor : 16;   // [31..16]
    } bits;

    // Define an unsigned member
    uint32_t u32;
};

union CropVertical{
    // Define the struct bits
    struct {
        uint32_t pixStartVer : 16; // [15..0]
        uint32_t pixEndVer : 16;   // [31..16]
    } bits;

    // Define an unsigned member
    uint32_t u32;
};

union FrameLastpageY {
    // Define the struct bits
    struct {
        uint32_t reserved0 : 12; // [11..0]
        uint32_t lastPageY : 19; // [30..12]
        uint32_t reserved1 : 1;  // [31]
    } bits;

    // Define an unsigned member
    uint32_t u32;
};

union FrameLastpageC {
    // Define the struct bits
    struct {
        uint32_t reserved0 : 12; // [11..0]
        uint32_t lastPageC : 19; // [30..12]
        uint32_t reserved1 : 1;  // [31]
    } bits;

    // Define an unsigned member
    uint32_t u32;
};

union FrameStride {
    // Define the struct bits
    struct {
        uint32_t strideY : 16; // [15..0]
        uint32_t strideC : 16; // [31..16]
    } bits;

    // Define an unsigned member
    uint32_t u32;
};

union OutputType {
    // Define the struct bits
    struct {
        uint32_t outputType : 4; // [3..0]
        uint32_t uvSwap : 1;     // [4]
        uint32_t reserved0 : 3;  // [7..5]
        uint32_t rgbAlpha : 8;   // [15..8]
        uint32_t reserved1 : 16; // [31..16]
    } bits;

    // Define an unsigned member
    uint32_t u32;
};

union FreqScale {
    // Define the struct bits
    struct {
        uint32_t freqScale : 2; // [1..0]
        uint32_t reserved : 30; // [31..2]
    } bits;

    // Define an unsigned member
    uint32_t u32;
};

union SamplingFactor {
    // Define the struct bits
    struct {
        uint32_t vFac : 8;     // [7..0]
        uint32_t uFac : 8;     // [15..8]
        uint32_t yFac : 8;     // [23..16]
        uint32_t reserved : 8; // [31..24]
    } bits;

    // Define an unsigned member
    uint32_t u32;
};

union Dri{
    // Define the struct bits
    struct {
        uint32_t dri : 16;      // [15..0]
        uint32_t reserved : 16; // [31..16]
    } bits;

    // Define an unsigned member
    uint32_t u32;
};

union JpegdStart{
    // Define the struct bits
    struct {
        uint32_t decodeStart : 1; // [0]
        uint32_t reserved : 30;   // [30..1]
        uint32_t ckGtEn : 1;      // [31]  跨总线拆分类型 0:跨4K拆分 1:跨256byte拆分
    } bits;

    // Define an unsigned member
    uint32_t u32;
};

#endif // JPEGD_REG_UNION_DEFINE_H