/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef BBOX_HDR_DATA_MILAN_H
#define BBOX_HDR_DATA_MILAN_H

#include "bbox_ddr_data.h"

/**
 *  the whole space is 512k, used for history data record
 *  the struct distribution is as follows:
 *  +-------------------+
 *  | head info(1k)     |     region:                    area:                   module block:
 *  +-------------------+     +--------------------+     +-----------------+     +-----------------+
 *  | boot region       |---->| first area         |---->| module block    |---->| block head      |
 *  +-------------------+     +--------------------+     +-----------------+     +-----------------+
 *  | run region        |     | second area        |     | module block    |     | block data      |
 *  +-------------------+     +--------------------+     +-----------------+     +-----------------+
 *  | reserved          |     | ......             |     | ......          |
 *  +-------------------+     +--------------------+     +-----------------+
 */
#define DATA_MODEL_HDR_BOOT_BIOS MODEL_VECTOR(HDR_BOOT_BIOS) = { \
    {"magic",            ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version",          ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id",        ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if",               ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used",          ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"err code",         ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason",           ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index",  ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"bsbc point",       ELEM_OUTPUT_INT, {0x1C}, {0x4}}, \
    {"bsbc exc point",   ELEM_OUTPUT_INT, {0x20}, {0x4}}, \
    {"hboot1 point",     ELEM_OUTPUT_INT, {0x24}, {0x4}}, \
    {"hboot1 exc point", ELEM_OUTPUT_INT, {0x28}, {0x4}}, \
    {"hboot2 point",     ELEM_OUTPUT_INT, {0x2C}, {0x4}}, \
    {"hboot2 exc point", ELEM_OUTPUT_INT, {0x30}, {0x4}}, \
    {"[BIOS info]",      ELEM_OUTPUT_STR_NL, {0x480}, {0x2780}}, \
}

#define DATA_MODEL_HDR_BOOT_DDR MODEL_VECTOR(HDR_BOOT_DDR) = { \
    {"magic",              ELEM_OUTPUT_INT,        {0x0},  {0x4}}, \
    {"version",            ELEM_OUTPUT_INT,        {0x4},  {0x4}}, \
    {"module id",          ELEM_OUTPUT_INT,        {0x8},  {0x4}}, \
    {"if",                 ELEM_CTRL_COMPARE,      {0xC},  {0x4}}, \
    {"is used",            ELEM_CTRL_CMP_JUMP_NE,  {0x1},  {0xFF}}, \
    {"err code",           ELEM_OUTPUT_INT,        {0x10}, {0x4}}, \
    {"reason",             ELEM_OUTPUT_INT,        {0x14}, {0x4}}, \
    {"hot reset index",    ELEM_OUTPUT_INT,        {0x18}, {0x4}}, \
    {"boot point",         ELEM_OUTPUT_INT,        {0x1C}, {0x4}}, \
    {"hbmc module id",     ELEM_OUTPUT_INT,        {0x20}, {0x4}}, \
    {"0 s0 p0 intSts",     ELEM_OUTPUT_INT,        {0x24}, {0x4}}, \
    {"0 s0 p1 intSts",     ELEM_OUTPUT_INT,        {0x28}, {0x4}}, \
    {"0 s0 p2 intSts",     ELEM_OUTPUT_INT,        {0x2C}, {0x4}}, \
    {"0 s0 p3 intSts",     ELEM_OUTPUT_INT,        {0x30}, {0x4}}, \
    {"0 s0 p4 intSts",     ELEM_OUTPUT_INT,        {0x34}, {0x4}}, \
    {"0 s0 p5 intSts",     ELEM_OUTPUT_INT,        {0x38}, {0x4}}, \
    {"0 s0 p6 intSts",     ELEM_OUTPUT_INT,        {0x3C}, {0x4}}, \
    {"0 s0 p7 intSts",     ELEM_OUTPUT_INT,        {0x40}, {0x4}}, \
    {"0 s0 p8 intSts",     ELEM_OUTPUT_INT,        {0x44}, {0x4}}, \
    {"0 s0 p9 intSts",     ELEM_OUTPUT_INT,        {0x48}, {0x4}}, \
    {"0 s0 p10 intSts",    ELEM_OUTPUT_INT,        {0x4C}, {0x4}}, \
    {"0 s0 p11 intSts",    ELEM_OUTPUT_INT,        {0x50}, {0x4}}, \
    {"0 s0 p12 intSts",    ELEM_OUTPUT_INT,        {0x54}, {0x4}}, \
    {"0 s0 p13 intSts",    ELEM_OUTPUT_INT,        {0x58}, {0x4}}, \
    {"0 s0 p14 intSts",    ELEM_OUTPUT_INT,        {0x5C}, {0x4}}, \
    {"0 s0 p15 intSts",    ELEM_OUTPUT_INT,        {0x60}, {0x4}}, \
    {"0 s1 p0 intSts",     ELEM_OUTPUT_INT,        {0x64}, {0x4}}, \
    {"0 s1 p1 intSts",     ELEM_OUTPUT_INT,        {0x68}, {0x4}}, \
    {"0 s1 p2 intSts",     ELEM_OUTPUT_INT,        {0x6C}, {0x4}}, \
    {"0 s1 p3 intSts",     ELEM_OUTPUT_INT,        {0x70}, {0x4}}, \
    {"0 s1 p4 intSts",     ELEM_OUTPUT_INT,        {0x74}, {0x4}}, \
    {"0 s1 p5 intSts",     ELEM_OUTPUT_INT,        {0x78}, {0x4}}, \
    {"0 s1 p6 intSts",     ELEM_OUTPUT_INT,        {0x7C}, {0x4}}, \
    {"0 s1 p7 intSts",     ELEM_OUTPUT_INT,        {0x80}, {0x4}}, \
    {"0 s1 p8 intSts",     ELEM_OUTPUT_INT,        {0x84}, {0x4}}, \
    {"0 s1 p9 intSts",     ELEM_OUTPUT_INT,        {0x88}, {0x4}}, \
    {"0 s1 p10 intSts",    ELEM_OUTPUT_INT,        {0x8C}, {0x4}}, \
    {"0 s1 p11 intSts",    ELEM_OUTPUT_INT,        {0x90}, {0x4}}, \
    {"0 s1 p12 intSts",    ELEM_OUTPUT_INT,        {0x94}, {0x4}}, \
    {"0 s1 p13 intSts",    ELEM_OUTPUT_INT,        {0x98}, {0x4}}, \
    {"0 s1 p14 intSts",    ELEM_OUTPUT_INT,        {0x9C}, {0x4}}, \
    {"0 s1 p15 intSts",    ELEM_OUTPUT_INT,        {0xA0}, {0x4}}, \
    {"0 s2 p0 intSts",     ELEM_OUTPUT_INT,        {0xA4}, {0x4}}, \
    {"0 s2 p1 intSts",     ELEM_OUTPUT_INT,        {0xA8}, {0x4}}, \
    {"0 s2 p2 intSts",     ELEM_OUTPUT_INT,        {0xAC}, {0x4}}, \
    {"0 s2 p3 intSts",     ELEM_OUTPUT_INT,        {0xB0}, {0x4}}, \
    {"0 s2 p4 intSts",     ELEM_OUTPUT_INT,        {0xB4}, {0x4}}, \
    {"0 s2 p5 intSts",     ELEM_OUTPUT_INT,        {0xB8}, {0x4}}, \
    {"0 s2 p6 intSts",     ELEM_OUTPUT_INT,        {0xBC}, {0x4}}, \
    {"0 s2 p7 intSts",     ELEM_OUTPUT_INT,        {0xC0}, {0x4}}, \
    {"0 s2 p8 intSts",     ELEM_OUTPUT_INT,        {0xC4}, {0x4}}, \
    {"0 s2 p9 intSts",     ELEM_OUTPUT_INT,        {0xC8}, {0x4}}, \
    {"0 s2 p10 intSts",    ELEM_OUTPUT_INT,        {0xCC}, {0x4}}, \
    {"0 s2 p11 intSts",    ELEM_OUTPUT_INT,        {0xD0}, {0x4}}, \
    {"0 s2 p12 intSts",    ELEM_OUTPUT_INT,        {0xD4}, {0x4}}, \
    {"0 s2 p13 intSts",    ELEM_OUTPUT_INT,        {0xD8}, {0x4}}, \
    {"0 s2 p14 intSts",    ELEM_OUTPUT_INT,        {0xDC}, {0x4}}, \
    {"0 s2 p15 intSts",    ELEM_OUTPUT_INT,        {0xE0}, {0x4}}, \
    {"0 s3 p0 intSts",     ELEM_OUTPUT_INT,        {0xE4}, {0x4}}, \
    {"0 s3 p1 intSts",     ELEM_OUTPUT_INT,        {0xE8}, {0x4}}, \
    {"0 s3 p2 intSts",     ELEM_OUTPUT_INT,        {0xEC}, {0x4}}, \
    {"0 s3 p3 intSts",     ELEM_OUTPUT_INT,        {0xF0}, {0x4}}, \
    {"0 s3 p4 intSts",     ELEM_OUTPUT_INT,        {0xF4}, {0x4}}, \
    {"0 s3 p5 intSts",     ELEM_OUTPUT_INT,        {0xF8}, {0x4}}, \
    {"0 s3 p6 intSts",     ELEM_OUTPUT_INT,        {0xFC}, {0x4}}, \
    {"0 s3 p7 intSts",     ELEM_OUTPUT_INT,        {0x100}, {0x4}}, \
    {"0 s3 p8 intSts",     ELEM_OUTPUT_INT,        {0x104}, {0x4}}, \
    {"0 s3 p9 intSts",     ELEM_OUTPUT_INT,        {0x108}, {0x4}}, \
    {"0 s3 p10 intSts",    ELEM_OUTPUT_INT,        {0x10C}, {0x4}}, \
    {"0 s3 p11 intSts",    ELEM_OUTPUT_INT,        {0x110}, {0x4}}, \
    {"0 s3 p12 intSts",    ELEM_OUTPUT_INT,        {0x114}, {0x4}}, \
    {"0 s3 p13 intSts",    ELEM_OUTPUT_INT,        {0x118}, {0x4}}, \
    {"0 s3 p14 intSts",    ELEM_OUTPUT_INT,        {0x11C}, {0x4}}, \
    {"0 s3 p15 intSts",    ELEM_OUTPUT_INT,        {0x120}, {0x4}}, \
    {"rasc module id",     ELEM_OUTPUT_INT,        {0x124}, {0x4}}, \
    {"1 s0 p0 intSts",     ELEM_OUTPUT_INT,        {0x128}, {0x4}}, \
    {"1 s0 p1 intSts",     ELEM_OUTPUT_INT,        {0x12C}, {0x4}}, \
    {"1 s0 p2 intSts",     ELEM_OUTPUT_INT,        {0x130}, {0x4}}, \
    {"1 s0 p3 intSts",     ELEM_OUTPUT_INT,        {0x134}, {0x4}}, \
    {"1 s0 p4 intSts",     ELEM_OUTPUT_INT,        {0x138}, {0x4}}, \
    {"1 s0 p5 intSts",     ELEM_OUTPUT_INT,        {0x13C}, {0x4}}, \
    {"1 s0 p6 intSts",     ELEM_OUTPUT_INT,        {0x140}, {0x4}}, \
    {"1 s0 p7 intSts",     ELEM_OUTPUT_INT,        {0x144}, {0x4}}, \
    {"1 s0 p8 intSts",     ELEM_OUTPUT_INT,        {0x148}, {0x4}}, \
    {"1 s0 p9 intSts",     ELEM_OUTPUT_INT,        {0x14C}, {0x4}}, \
    {"1 s0 p10 intSts",    ELEM_OUTPUT_INT,        {0x150}, {0x4}}, \
    {"1 s0 p11 intSts",    ELEM_OUTPUT_INT,        {0x154}, {0x4}}, \
    {"1 s0 p12 intSts",    ELEM_OUTPUT_INT,        {0x158}, {0x4}}, \
    {"1 s0 p13 intSts",    ELEM_OUTPUT_INT,        {0x15C}, {0x4}}, \
    {"1 s0 p14 intSts",    ELEM_OUTPUT_INT,        {0x160}, {0x4}}, \
    {"1 s0 p15 intSts",    ELEM_OUTPUT_INT,        {0x164}, {0x4}}, \
    {"1 s1 p0 intSts",     ELEM_OUTPUT_INT,        {0x168}, {0x4}}, \
    {"1 s1 p1 intSts",     ELEM_OUTPUT_INT,        {0x16C}, {0x4}}, \
    {"1 s1 p2 intSts",     ELEM_OUTPUT_INT,        {0x170}, {0x4}}, \
    {"1 s1 p3 intSts",     ELEM_OUTPUT_INT,        {0x174}, {0x4}}, \
    {"1 s1 p4 intSts",     ELEM_OUTPUT_INT,        {0x178}, {0x4}}, \
    {"1 s1 p5 intSts",     ELEM_OUTPUT_INT,        {0x17C}, {0x4}}, \
    {"1 s1 p6 intSts",     ELEM_OUTPUT_INT,        {0x180}, {0x4}}, \
    {"1 s1 p7 intSts",     ELEM_OUTPUT_INT,        {0x184}, {0x4}}, \
    {"1 s1 p8 intSts",     ELEM_OUTPUT_INT,        {0x188}, {0x4}}, \
    {"1 s1 p9 intSts",     ELEM_OUTPUT_INT,        {0x18C}, {0x4}}, \
    {"1 s1 p10 intSts",    ELEM_OUTPUT_INT,        {0x190}, {0x4}}, \
    {"1 s1 p11 intSts",    ELEM_OUTPUT_INT,        {0x194}, {0x4}}, \
    {"1 s1 p12 intSts",    ELEM_OUTPUT_INT,        {0x198}, {0x4}}, \
    {"1 s1 p13 intSts",    ELEM_OUTPUT_INT,        {0x19C}, {0x4}}, \
    {"1 s1 p14 intSts",    ELEM_OUTPUT_INT,        {0x1A0}, {0x4}}, \
    {"1 s1 p15 intSts",    ELEM_OUTPUT_INT,        {0x1A4}, {0x4}}, \
    {"1 s2 p0 intSts",     ELEM_OUTPUT_INT,        {0x1A8}, {0x4}}, \
    {"1 s2 p1 intSts",     ELEM_OUTPUT_INT,        {0x1AC}, {0x4}}, \
    {"1 s2 p2 intSts",     ELEM_OUTPUT_INT,        {0x1B0}, {0x4}}, \
    {"1 s2 p3 intSts",     ELEM_OUTPUT_INT,        {0x1B4}, {0x4}}, \
    {"1 s2 p4 intSts",     ELEM_OUTPUT_INT,        {0x1B8}, {0x4}}, \
    {"1 s2 p5 intSts",     ELEM_OUTPUT_INT,        {0x1BC}, {0x4}}, \
    {"1 s2 p6 intSts",     ELEM_OUTPUT_INT,        {0x1C0}, {0x4}}, \
    {"1 s2 p7 intSts",     ELEM_OUTPUT_INT,        {0x1C4}, {0x4}}, \
    {"1 s2 p8 intSts",     ELEM_OUTPUT_INT,        {0x1C8}, {0x4}}, \
    {"1 s2 p9 intSts",     ELEM_OUTPUT_INT,        {0x1CC}, {0x4}}, \
    {"1 s2 p10 intSts",    ELEM_OUTPUT_INT,        {0x1D0}, {0x4}}, \
    {"1 s2 p11 intSts",    ELEM_OUTPUT_INT,        {0x1D4}, {0x4}}, \
    {"1 s2 p12 intSts",    ELEM_OUTPUT_INT,        {0x1D8}, {0x4}}, \
    {"1 s2 p13 intSts",    ELEM_OUTPUT_INT,        {0x1DC}, {0x4}}, \
    {"1 s2 p14 intSts",    ELEM_OUTPUT_INT,        {0x1E0}, {0x4}}, \
    {"1 s2 p15 intSts",    ELEM_OUTPUT_INT,        {0x1E4}, {0x4}}, \
    {"1 s3 p0 intSts",     ELEM_OUTPUT_INT,        {0x1E8}, {0x4}}, \
    {"1 s3 p1 intSts",     ELEM_OUTPUT_INT,        {0x1EC}, {0x4}}, \
    {"1 s3 p2 intSts",     ELEM_OUTPUT_INT,        {0x1F0}, {0x4}}, \
    {"1 s3 p3 intSts",     ELEM_OUTPUT_INT,        {0x1F4}, {0x4}}, \
    {"1 s3 p4 intSts",     ELEM_OUTPUT_INT,        {0x1F8}, {0x4}}, \
    {"1 s3 p5 intSts",     ELEM_OUTPUT_INT,        {0x1FC}, {0x4}}, \
    {"1 s3 p6 intSts",     ELEM_OUTPUT_INT,        {0x200}, {0x4}}, \
    {"1 s3 p7 intSts",     ELEM_OUTPUT_INT,        {0x204}, {0x4}}, \
    {"1 s3 p8 intSts",     ELEM_OUTPUT_INT,        {0x208}, {0x4}}, \
    {"1 s3 p9 intSts",     ELEM_OUTPUT_INT,        {0x20C}, {0x4}}, \
    {"1 s3 p10 intSts",    ELEM_OUTPUT_INT,        {0x210}, {0x4}}, \
    {"1 s3 p11 intSts",    ELEM_OUTPUT_INT,        {0x214}, {0x4}}, \
    {"1 s3 p12 intSts",    ELEM_OUTPUT_INT,        {0x218}, {0x4}}, \
    {"1 s3 p13 intSts",    ELEM_OUTPUT_INT,        {0x21C}, {0x4}}, \
    {"1 s3 p14 intSts",    ELEM_OUTPUT_INT,        {0x220}, {0x4}}, \
    {"1 s3 p15 intSts",    ELEM_OUTPUT_INT,        {0x224}, {0x4}}, \
    {"1 s0 p0 rcdTyp",     ELEM_OUTPUT_INT,        {0x228}, {0x4}}, \
    {"1 s0 p1 rcdTyp",     ELEM_OUTPUT_INT,        {0x22C}, {0x4}}, \
    {"1 s0 p2 rcdTyp",     ELEM_OUTPUT_INT,        {0x230}, {0x4}}, \
    {"1 s0 p3 rcdTyp",     ELEM_OUTPUT_INT,        {0x234}, {0x4}}, \
    {"1 s0 p4 rcdTyp",     ELEM_OUTPUT_INT,        {0x238}, {0x4}}, \
    {"1 s0 p5 rcdTyp",     ELEM_OUTPUT_INT,        {0x23C}, {0x4}}, \
    {"1 s0 p6 rcdTyp",     ELEM_OUTPUT_INT,        {0x240}, {0x4}}, \
    {"1 s0 p7 rcdTyp",     ELEM_OUTPUT_INT,        {0x244}, {0x4}}, \
    {"1 s0 p8 rcdTyp",     ELEM_OUTPUT_INT,        {0x248}, {0x4}}, \
    {"1 s0 p9 rcdTyp",     ELEM_OUTPUT_INT,        {0x24C}, {0x4}}, \
    {"1 s0 p10 rcdTyp",    ELEM_OUTPUT_INT,        {0x250}, {0x4}}, \
    {"1 s0 p11 rcdTyp",    ELEM_OUTPUT_INT,        {0x254}, {0x4}}, \
    {"1 s0 p12 rcdTyp",    ELEM_OUTPUT_INT,        {0x258}, {0x4}}, \
    {"1 s0 p13 rcdTyp",    ELEM_OUTPUT_INT,        {0x25C}, {0x4}}, \
    {"1 s0 p14 rcdTyp",    ELEM_OUTPUT_INT,        {0x260}, {0x4}}, \
    {"1 s0 p15 rcdTyp",    ELEM_OUTPUT_INT,        {0x264}, {0x4}}, \
    {"1 s1 p0 rcdTyp",     ELEM_OUTPUT_INT,        {0x268}, {0x4}}, \
    {"1 s1 p1 rcdTyp",     ELEM_OUTPUT_INT,        {0x26C}, {0x4}}, \
    {"1 s1 p2 rcdTyp",     ELEM_OUTPUT_INT,        {0x270}, {0x4}}, \
    {"1 s1 p3 rcdTyp",     ELEM_OUTPUT_INT,        {0x274}, {0x4}}, \
    {"1 s1 p4 rcdTyp",     ELEM_OUTPUT_INT,        {0x278}, {0x4}}, \
    {"1 s1 p5 rcdTyp",     ELEM_OUTPUT_INT,        {0x27C}, {0x4}}, \
    {"1 s1 p6 rcdTyp",     ELEM_OUTPUT_INT,        {0x280}, {0x4}}, \
    {"1 s1 p7 rcdTyp",     ELEM_OUTPUT_INT,        {0x284}, {0x4}}, \
    {"1 s1 p8 rcdTyp",     ELEM_OUTPUT_INT,        {0x288}, {0x4}}, \
    {"1 s1 p9 rcdTyp",     ELEM_OUTPUT_INT,        {0x28C}, {0x4}}, \
    {"1 s1 p10 rcdTyp",    ELEM_OUTPUT_INT,        {0x290}, {0x4}}, \
    {"1 s1 p11 rcdTyp",    ELEM_OUTPUT_INT,        {0x294}, {0x4}}, \
    {"1 s1 p12 rcdTyp",    ELEM_OUTPUT_INT,        {0x298}, {0x4}}, \
    {"1 s1 p13 rcdTyp",    ELEM_OUTPUT_INT,        {0x29C}, {0x4}}, \
    {"1 s1 p14 rcdTyp",    ELEM_OUTPUT_INT,        {0x2A0}, {0x4}}, \
    {"1 s1 p15 rcdTyp",    ELEM_OUTPUT_INT,        {0x2A4}, {0x4}}, \
    {"1 s2 p0 rcdTyp",     ELEM_OUTPUT_INT,        {0x2A8}, {0x4}}, \
    {"1 s2 p1 rcdTyp",     ELEM_OUTPUT_INT,        {0x2AC}, {0x4}}, \
    {"1 s2 p2 rcdTyp",     ELEM_OUTPUT_INT,        {0x2B0}, {0x4}}, \
    {"1 s2 p3 rcdTyp",     ELEM_OUTPUT_INT,        {0x2B4}, {0x4}}, \
    {"1 s2 p4 rcdTyp",     ELEM_OUTPUT_INT,        {0x2B8}, {0x4}}, \
    {"1 s2 p5 rcdTyp",     ELEM_OUTPUT_INT,        {0x2BC}, {0x4}}, \
    {"1 s2 p6 rcdTyp",     ELEM_OUTPUT_INT,        {0x2C0}, {0x4}}, \
    {"1 s2 p7 rcdTyp",     ELEM_OUTPUT_INT,        {0x2C4}, {0x4}}, \
    {"1 s2 p8 rcdTyp",     ELEM_OUTPUT_INT,        {0x2C8}, {0x4}}, \
    {"1 s2 p9 rcdTyp",     ELEM_OUTPUT_INT,        {0x2CC}, {0x4}}, \
    {"1 s2 p10 rcdTyp",    ELEM_OUTPUT_INT,        {0x2D0}, {0x4}}, \
    {"1 s2 p11 rcdTyp",    ELEM_OUTPUT_INT,        {0x2D4}, {0x4}}, \
    {"1 s2 p12 rcdTyp",    ELEM_OUTPUT_INT,        {0x2D8}, {0x4}}, \
    {"1 s2 p13 rcdTyp",    ELEM_OUTPUT_INT,        {0x2DC}, {0x4}}, \
    {"1 s2 p14 rcdTyp",    ELEM_OUTPUT_INT,        {0x2E0}, {0x4}}, \
    {"1 s2 p15 rcdTyp",    ELEM_OUTPUT_INT,        {0x2E4}, {0x4}}, \
    {"1 s3 p0 rcdTyp",     ELEM_OUTPUT_INT,        {0x2E8}, {0x4}}, \
    {"1 s3 p1 rcdTyp",     ELEM_OUTPUT_INT,        {0x2EC}, {0x4}}, \
    {"1 s3 p2 rcdTyp",     ELEM_OUTPUT_INT,        {0x2F0}, {0x4}}, \
    {"1 s3 p3 rcdTyp",     ELEM_OUTPUT_INT,        {0x2F4}, {0x4}}, \
    {"1 s3 p4 rcdTyp",     ELEM_OUTPUT_INT,        {0x2F8}, {0x4}}, \
    {"1 s3 p5 rcdTyp",     ELEM_OUTPUT_INT,        {0x2FC}, {0x4}}, \
    {"1 s3 p6 rcdTyp",     ELEM_OUTPUT_INT,        {0x300}, {0x4}}, \
    {"1 s3 p7 rcdTyp",     ELEM_OUTPUT_INT,        {0x304}, {0x4}}, \
    {"1 s3 p8 rcdTyp",     ELEM_OUTPUT_INT,        {0x308}, {0x4}}, \
    {"1 s3 p9 rcdTyp",     ELEM_OUTPUT_INT,        {0x30C}, {0x4}}, \
    {"1 s3 p10 rcdTyp",    ELEM_OUTPUT_INT,        {0x310}, {0x4}}, \
    {"1 s3 p11 rcdTyp",    ELEM_OUTPUT_INT,        {0x314}, {0x4}}, \
    {"1 s3 p12 rcdTyp",    ELEM_OUTPUT_INT,        {0x318}, {0x4}}, \
    {"1 s3 p13 rcdTyp",    ELEM_OUTPUT_INT,        {0x31C}, {0x4}}, \
    {"1 s3 p14 rcdTyp",    ELEM_OUTPUT_INT,        {0x320}, {0x4}}, \
    {"1 s3 p15 rcdTyp",    ELEM_OUTPUT_INT,        {0x324}, {0x4}}, \
    {"1 s0 p0 uCrAdL",     ELEM_OUTPUT_INT,        {0x328}, {0x4}}, \
    {"1 s0 p1 uCrAdL",     ELEM_OUTPUT_INT,        {0x32C}, {0x4}}, \
    {"1 s0 p2 uCrAdL",     ELEM_OUTPUT_INT,        {0x330}, {0x4}}, \
    {"1 s0 p3 uCrAdL",     ELEM_OUTPUT_INT,        {0x334}, {0x4}}, \
    {"1 s0 p4 uCrAdL",     ELEM_OUTPUT_INT,        {0x338}, {0x4}}, \
    {"1 s0 p5 uCrAdL",     ELEM_OUTPUT_INT,        {0x33C}, {0x4}}, \
    {"1 s0 p6 uCrAdL",     ELEM_OUTPUT_INT,        {0x340}, {0x4}}, \
    {"1 s0 p7 uCrAdL",     ELEM_OUTPUT_INT,        {0x344}, {0x4}}, \
    {"1 s0 p8 uCrAdL",     ELEM_OUTPUT_INT,        {0x348}, {0x4}}, \
    {"1 s0 p9 uCrAdL",     ELEM_OUTPUT_INT,        {0x34C}, {0x4}}, \
    {"1 s0 p10 uCrAdL",    ELEM_OUTPUT_INT,        {0x350}, {0x4}}, \
    {"1 s0 p11 uCrAdL",    ELEM_OUTPUT_INT,        {0x354}, {0x4}}, \
    {"1 s0 p12 uCrAdL",    ELEM_OUTPUT_INT,        {0x358}, {0x4}}, \
    {"1 s0 p13 uCrAdL",    ELEM_OUTPUT_INT,        {0x35C}, {0x4}}, \
    {"1 s0 p14 uCrAdL",    ELEM_OUTPUT_INT,        {0x360}, {0x4}}, \
    {"1 s0 p15 uCrAdL",    ELEM_OUTPUT_INT,        {0x364}, {0x4}}, \
    {"1 s1 p0 uCrAdL",     ELEM_OUTPUT_INT,        {0x368}, {0x4}}, \
    {"1 s1 p1 uCrAdL",     ELEM_OUTPUT_INT,        {0x36C}, {0x4}}, \
    {"1 s1 p2 uCrAdL",     ELEM_OUTPUT_INT,        {0x370}, {0x4}}, \
    {"1 s1 p3 uCrAdL",     ELEM_OUTPUT_INT,        {0x374}, {0x4}}, \
    {"1 s1 p4 uCrAdL",     ELEM_OUTPUT_INT,        {0x378}, {0x4}}, \
    {"1 s1 p5 uCrAdL",     ELEM_OUTPUT_INT,        {0x37C}, {0x4}}, \
    {"1 s1 p6 uCrAdL",     ELEM_OUTPUT_INT,        {0x380}, {0x4}}, \
    {"1 s1 p7 uCrAdL",     ELEM_OUTPUT_INT,        {0x384}, {0x4}}, \
    {"1 s1 p8 uCrAdL",     ELEM_OUTPUT_INT,        {0x388}, {0x4}}, \
    {"1 s1 p9 uCrAdL",     ELEM_OUTPUT_INT,        {0x38C}, {0x4}}, \
    {"1 s1 p10 uCrAdL",    ELEM_OUTPUT_INT,        {0x390}, {0x4}}, \
    {"1 s1 p11 uCrAdL",    ELEM_OUTPUT_INT,        {0x394}, {0x4}}, \
    {"1 s1 p12 uCrAdL",    ELEM_OUTPUT_INT,        {0x398}, {0x4}}, \
    {"1 s1 p13 uCrAdL",    ELEM_OUTPUT_INT,        {0x39C}, {0x4}}, \
    {"1 s1 p14 uCrAdL",    ELEM_OUTPUT_INT,        {0x3A0}, {0x4}}, \
    {"1 s1 p15 uCrAdL",    ELEM_OUTPUT_INT,        {0x3A4}, {0x4}}, \
    {"1 s2 p0 uCrAdL",     ELEM_OUTPUT_INT,        {0x3A8}, {0x4}}, \
    {"1 s2 p1 uCrAdL",     ELEM_OUTPUT_INT,        {0x3AC}, {0x4}}, \
    {"1 s2 p2 uCrAdL",     ELEM_OUTPUT_INT,        {0x3B0}, {0x4}}, \
    {"1 s2 p3 uCrAdL",     ELEM_OUTPUT_INT,        {0x3B4}, {0x4}}, \
    {"1 s2 p4 uCrAdL",     ELEM_OUTPUT_INT,        {0x3B8}, {0x4}}, \
    {"1 s2 p5 uCrAdL",     ELEM_OUTPUT_INT,        {0x3BC}, {0x4}}, \
    {"1 s2 p6 uCrAdL",     ELEM_OUTPUT_INT,        {0x3C0}, {0x4}}, \
    {"1 s2 p7 uCrAdL",     ELEM_OUTPUT_INT,        {0x3C4}, {0x4}}, \
    {"1 s2 p8 uCrAdL",     ELEM_OUTPUT_INT,        {0x3C8}, {0x4}}, \
    {"1 s2 p9 uCrAdL",     ELEM_OUTPUT_INT,        {0x3CC}, {0x4}}, \
    {"1 s2 p10 uCrAdL",    ELEM_OUTPUT_INT,        {0x3D0}, {0x4}}, \
    {"1 s2 p11 uCrAdL",    ELEM_OUTPUT_INT,        {0x3D4}, {0x4}}, \
    {"1 s2 p12 uCrAdL",    ELEM_OUTPUT_INT,        {0x3D8}, {0x4}}, \
    {"1 s2 p13 uCrAdL",    ELEM_OUTPUT_INT,        {0x3DC}, {0x4}}, \
    {"1 s2 p14 uCrAdL",    ELEM_OUTPUT_INT,        {0x3E0}, {0x4}}, \
    {"1 s2 p15 uCrAdL",    ELEM_OUTPUT_INT,        {0x3E4}, {0x4}}, \
    {"1 s3 p0 uCrAdL",     ELEM_OUTPUT_INT,        {0x3E8}, {0x4}}, \
    {"1 s3 p1 uCrAdL",     ELEM_OUTPUT_INT,        {0x3EC}, {0x4}}, \
    {"1 s3 p2 uCrAdL",     ELEM_OUTPUT_INT,        {0x3F0}, {0x4}}, \
    {"1 s3 p3 uCrAdL",     ELEM_OUTPUT_INT,        {0x3F4}, {0x4}}, \
    {"1 s3 p4 uCrAdL",     ELEM_OUTPUT_INT,        {0x3F8}, {0x4}}, \
    {"1 s3 p5 uCrAdL",     ELEM_OUTPUT_INT,        {0x3FC}, {0x4}}, \
    {"1 s3 p6 uCrAdL",     ELEM_OUTPUT_INT,        {0x400}, {0x4}}, \
    {"1 s3 p7 uCrAdL",     ELEM_OUTPUT_INT,        {0x404}, {0x4}}, \
    {"1 s3 p8 uCrAdL",     ELEM_OUTPUT_INT,        {0x408}, {0x4}}, \
    {"1 s3 p9 uCrAdL",     ELEM_OUTPUT_INT,        {0x40C}, {0x4}}, \
    {"1 s3 p10 uCrAdL",    ELEM_OUTPUT_INT,        {0x410}, {0x4}}, \
    {"1 s3 p11 uCrAdL",    ELEM_OUTPUT_INT,        {0x414}, {0x4}}, \
    {"1 s3 p12 uCrAdL",    ELEM_OUTPUT_INT,        {0x418}, {0x4}}, \
    {"1 s3 p13 uCrAdL",    ELEM_OUTPUT_INT,        {0x41C}, {0x4}}, \
    {"1 s3 p14 uCrAdL",    ELEM_OUTPUT_INT,        {0x420}, {0x4}}, \
    {"1 s3 p15 uCrAdL",    ELEM_OUTPUT_INT,        {0x424}, {0x4}}, \
    {"1 s0 p0 uCrAdH",     ELEM_OUTPUT_INT,        {0x428}, {0x4}}, \
    {"1 s0 p1 uCrAdH",     ELEM_OUTPUT_INT,        {0x42C}, {0x4}}, \
    {"1 s0 p2 uCrAdH",     ELEM_OUTPUT_INT,        {0x430}, {0x4}}, \
    {"1 s0 p3 uCrAdH",     ELEM_OUTPUT_INT,        {0x434}, {0x4}}, \
    {"1 s0 p4 uCrAdH",     ELEM_OUTPUT_INT,        {0x438}, {0x4}}, \
    {"1 s0 p5 uCrAdH",     ELEM_OUTPUT_INT,        {0x43C}, {0x4}}, \
    {"1 s0 p6 uCrAdH",     ELEM_OUTPUT_INT,        {0x440}, {0x4}}, \
    {"1 s0 p7 uCrAdH",     ELEM_OUTPUT_INT,        {0x444}, {0x4}}, \
    {"1 s0 p8 uCrAdH",     ELEM_OUTPUT_INT,        {0x448}, {0x4}}, \
    {"1 s0 p9 uCrAdH",     ELEM_OUTPUT_INT,        {0x44C}, {0x4}}, \
    {"1 s0 p10 uCrAdH",    ELEM_OUTPUT_INT,        {0x450}, {0x4}}, \
    {"1 s0 p11 uCrAdH",    ELEM_OUTPUT_INT,        {0x454}, {0x4}}, \
    {"1 s0 p12 uCrAdH",    ELEM_OUTPUT_INT,        {0x458}, {0x4}}, \
    {"1 s0 p13 uCrAdH",    ELEM_OUTPUT_INT,        {0x45C}, {0x4}}, \
    {"1 s0 p14 uCrAdH",    ELEM_OUTPUT_INT,        {0x460}, {0x4}}, \
    {"1 s0 p15 uCrAdH",    ELEM_OUTPUT_INT,        {0x464}, {0x4}}, \
    {"1 s1 p0 uCrAdH",     ELEM_OUTPUT_INT,        {0x468}, {0x4}}, \
    {"1 s1 p1 uCrAdH",     ELEM_OUTPUT_INT,        {0x46C}, {0x4}}, \
    {"1 s1 p2 uCrAdH",     ELEM_OUTPUT_INT,        {0x470}, {0x4}}, \
    {"1 s1 p3 uCrAdH",     ELEM_OUTPUT_INT,        {0x474}, {0x4}}, \
    {"1 s1 p4 uCrAdH",     ELEM_OUTPUT_INT,        {0x478}, {0x4}}, \
    {"1 s1 p5 uCrAdH",     ELEM_OUTPUT_INT,        {0x47C}, {0x4}}, \
    {"1 s1 p6 uCrAdH",     ELEM_OUTPUT_INT,        {0x480}, {0x4}}, \
    {"1 s1 p7 uCrAdH",     ELEM_OUTPUT_INT,        {0x484}, {0x4}}, \
    {"1 s1 p8 uCrAdH",     ELEM_OUTPUT_INT,        {0x488}, {0x4}}, \
    {"1 s1 p9 uCrAdH",     ELEM_OUTPUT_INT,        {0x48C}, {0x4}}, \
    {"1 s1 p10 uCrAdH",    ELEM_OUTPUT_INT,        {0x490}, {0x4}}, \
    {"1 s1 p11 uCrAdH",    ELEM_OUTPUT_INT,        {0x494}, {0x4}}, \
    {"1 s1 p12 uCrAdH",    ELEM_OUTPUT_INT,        {0x498}, {0x4}}, \
    {"1 s1 p13 uCrAdH",    ELEM_OUTPUT_INT,        {0x49C}, {0x4}}, \
    {"1 s1 p14 uCrAdH",    ELEM_OUTPUT_INT,        {0x4A0}, {0x4}}, \
    {"1 s1 p15 uCrAdH",    ELEM_OUTPUT_INT,        {0x4A4}, {0x4}}, \
    {"1 s2 p0 uCrAdH",     ELEM_OUTPUT_INT,        {0x4A8}, {0x4}}, \
    {"1 s2 p1 uCrAdH",     ELEM_OUTPUT_INT,        {0x4AC}, {0x4}}, \
    {"1 s2 p2 uCrAdH",     ELEM_OUTPUT_INT,        {0x4B0}, {0x4}}, \
    {"1 s2 p3 uCrAdH",     ELEM_OUTPUT_INT,        {0x4B4}, {0x4}}, \
    {"1 s2 p4 uCrAdH",     ELEM_OUTPUT_INT,        {0x4B8}, {0x4}}, \
    {"1 s2 p5 uCrAdH",     ELEM_OUTPUT_INT,        {0x4BC}, {0x4}}, \
    {"1 s2 p6 uCrAdH",     ELEM_OUTPUT_INT,        {0x4C0}, {0x4}}, \
    {"1 s2 p7 uCrAdH",     ELEM_OUTPUT_INT,        {0x4C4}, {0x4}}, \
    {"1 s2 p8 uCrAdH",     ELEM_OUTPUT_INT,        {0x4C8}, {0x4}}, \
    {"1 s2 p9 uCrAdH",     ELEM_OUTPUT_INT,        {0x4CC}, {0x4}}, \
    {"1 s2 p10 uCrAdH",    ELEM_OUTPUT_INT,        {0x4D0}, {0x4}}, \
    {"1 s2 p11 uCrAdH",    ELEM_OUTPUT_INT,        {0x4D4}, {0x4}}, \
    {"1 s2 p12 uCrAdH",    ELEM_OUTPUT_INT,        {0x4D8}, {0x4}}, \
    {"1 s2 p13 uCrAdH",    ELEM_OUTPUT_INT,        {0x4DC}, {0x4}}, \
    {"1 s2 p14 uCrAdH",    ELEM_OUTPUT_INT,        {0x4E0}, {0x4}}, \
    {"1 s2 p15 uCrAdH",    ELEM_OUTPUT_INT,        {0x4E4}, {0x4}}, \
    {"1 s3 p0 uCrAdH",     ELEM_OUTPUT_INT,        {0x4E8}, {0x4}}, \
    {"1 s3 p1 uCrAdH",     ELEM_OUTPUT_INT,        {0x4EC}, {0x4}}, \
    {"1 s3 p2 uCrAdH",     ELEM_OUTPUT_INT,        {0x4F0}, {0x4}}, \
    {"1 s3 p3 uCrAdH",     ELEM_OUTPUT_INT,        {0x4F4}, {0x4}}, \
    {"1 s3 p4 uCrAdH",     ELEM_OUTPUT_INT,        {0x4F8}, {0x4}}, \
    {"1 s3 p5 uCrAdH",     ELEM_OUTPUT_INT,        {0x4FC}, {0x4}}, \
    {"1 s3 p6 uCrAdH",     ELEM_OUTPUT_INT,        {0x500}, {0x4}}, \
    {"1 s3 p7 uCrAdH",     ELEM_OUTPUT_INT,        {0x504}, {0x4}}, \
    {"1 s3 p8 uCrAdH",     ELEM_OUTPUT_INT,        {0x508}, {0x4}}, \
    {"1 s3 p9 uCrAdH",     ELEM_OUTPUT_INT,        {0x50C}, {0x4}}, \
    {"1 s3 p10 uCrAdH",    ELEM_OUTPUT_INT,        {0x510}, {0x4}}, \
    {"1 s3 p11 uCrAdH",    ELEM_OUTPUT_INT,        {0x514}, {0x4}}, \
    {"1 s3 p12 uCrAdH",    ELEM_OUTPUT_INT,        {0x518}, {0x4}}, \
    {"1 s3 p13 uCrAdH",    ELEM_OUTPUT_INT,        {0x51C}, {0x4}}, \
    {"1 s3 p14 uCrAdH",    ELEM_OUTPUT_INT,        {0x520}, {0x4}}, \
    {"1 s3 p15 uCrAdH",    ELEM_OUTPUT_INT,        {0x524}, {0x4}}, \
    {"1 s0 p0 corAdL",     ELEM_OUTPUT_INT,        {0x528}, {0x4}}, \
    {"1 s0 p1 corAdL",     ELEM_OUTPUT_INT,        {0x52C}, {0x4}}, \
    {"1 s0 p2 corAdL",     ELEM_OUTPUT_INT,        {0x530}, {0x4}}, \
    {"1 s0 p3 corAdL",     ELEM_OUTPUT_INT,        {0x534}, {0x4}}, \
    {"1 s0 p4 corAdL",     ELEM_OUTPUT_INT,        {0x538}, {0x4}}, \
    {"1 s0 p5 corAdL",     ELEM_OUTPUT_INT,        {0x53C}, {0x4}}, \
    {"1 s0 p6 corAdL",     ELEM_OUTPUT_INT,        {0x540}, {0x4}}, \
    {"1 s0 p7 corAdL",     ELEM_OUTPUT_INT,        {0x544}, {0x4}}, \
    {"1 s0 p8 corAdL",     ELEM_OUTPUT_INT,        {0x548}, {0x4}}, \
    {"1 s0 p9 corAdL",     ELEM_OUTPUT_INT,        {0x54C}, {0x4}}, \
    {"1 s0 p10 corAdL",    ELEM_OUTPUT_INT,        {0x550}, {0x4}}, \
    {"1 s0 p11 corAdL",    ELEM_OUTPUT_INT,        {0x554}, {0x4}}, \
    {"1 s0 p12 corAdL",    ELEM_OUTPUT_INT,        {0x558}, {0x4}}, \
    {"1 s0 p13 corAdL",    ELEM_OUTPUT_INT,        {0x55C}, {0x4}}, \
    {"1 s0 p14 corAdL",    ELEM_OUTPUT_INT,        {0x560}, {0x4}}, \
    {"1 s0 p15 corAdL",    ELEM_OUTPUT_INT,        {0x564}, {0x4}}, \
    {"1 s1 p0 corAdL",     ELEM_OUTPUT_INT,        {0x568}, {0x4}}, \
    {"1 s1 p1 corAdL",     ELEM_OUTPUT_INT,        {0x56C}, {0x4}}, \
    {"1 s1 p2 corAdL",     ELEM_OUTPUT_INT,        {0x570}, {0x4}}, \
    {"1 s1 p3 corAdL",     ELEM_OUTPUT_INT,        {0x574}, {0x4}}, \
    {"1 s1 p4 corAdL",     ELEM_OUTPUT_INT,        {0x578}, {0x4}}, \
    {"1 s1 p5 corAdL",     ELEM_OUTPUT_INT,        {0x57C}, {0x4}}, \
    {"1 s1 p6 corAdL",     ELEM_OUTPUT_INT,        {0x580}, {0x4}}, \
    {"1 s1 p7 corAdL",     ELEM_OUTPUT_INT,        {0x584}, {0x4}}, \
    {"1 s1 p8 corAdL",     ELEM_OUTPUT_INT,        {0x588}, {0x4}}, \
    {"1 s1 p9 corAdL",     ELEM_OUTPUT_INT,        {0x58C}, {0x4}}, \
    {"1 s1 p10 corAdL",    ELEM_OUTPUT_INT,        {0x590}, {0x4}}, \
    {"1 s1 p11 corAdL",    ELEM_OUTPUT_INT,        {0x594}, {0x4}}, \
    {"1 s1 p12 corAdL",    ELEM_OUTPUT_INT,        {0x598}, {0x4}}, \
    {"1 s1 p13 corAdL",    ELEM_OUTPUT_INT,        {0x59C}, {0x4}}, \
    {"1 s1 p14 corAdL",    ELEM_OUTPUT_INT,        {0x5A0}, {0x4}}, \
    {"1 s1 p15 corAdL",    ELEM_OUTPUT_INT,        {0x5A4}, {0x4}}, \
    {"1 s2 p0 corAdL",     ELEM_OUTPUT_INT,        {0x5A8}, {0x4}}, \
    {"1 s2 p1 corAdL",     ELEM_OUTPUT_INT,        {0x5AC}, {0x4}}, \
    {"1 s2 p2 corAdL",     ELEM_OUTPUT_INT,        {0x5B0}, {0x4}}, \
    {"1 s2 p3 corAdL",     ELEM_OUTPUT_INT,        {0x5B4}, {0x4}}, \
    {"1 s2 p4 corAdL",     ELEM_OUTPUT_INT,        {0x5B8}, {0x4}}, \
    {"1 s2 p5 corAdL",     ELEM_OUTPUT_INT,        {0x5BC}, {0x4}}, \
    {"1 s2 p6 corAdL",     ELEM_OUTPUT_INT,        {0x5C0}, {0x4}}, \
    {"1 s2 p7 corAdL",     ELEM_OUTPUT_INT,        {0x5C4}, {0x4}}, \
    {"1 s2 p8 corAdL",     ELEM_OUTPUT_INT,        {0x5C8}, {0x4}}, \
    {"1 s2 p9 corAdL",     ELEM_OUTPUT_INT,        {0x5CC}, {0x4}}, \
    {"1 s2 p10 corAdL",    ELEM_OUTPUT_INT,        {0x5D0}, {0x4}}, \
    {"1 s2 p11 corAdL",    ELEM_OUTPUT_INT,        {0x5D4}, {0x4}}, \
    {"1 s2 p12 corAdL",    ELEM_OUTPUT_INT,        {0x5D8}, {0x4}}, \
    {"1 s2 p13 corAdL",    ELEM_OUTPUT_INT,        {0x5DC}, {0x4}}, \
    {"1 s2 p14 corAdL",    ELEM_OUTPUT_INT,        {0x5E0}, {0x4}}, \
    {"1 s2 p15 corAdL",    ELEM_OUTPUT_INT,        {0x5E4}, {0x4}}, \
    {"1 s3 p0 corAdL",     ELEM_OUTPUT_INT,        {0x5E8}, {0x4}}, \
    {"1 s3 p1 corAdL",     ELEM_OUTPUT_INT,        {0x5EC}, {0x4}}, \
    {"1 s3 p2 corAdL",     ELEM_OUTPUT_INT,        {0x5F0}, {0x4}}, \
    {"1 s3 p3 corAdL",     ELEM_OUTPUT_INT,        {0x5F4}, {0x4}}, \
    {"1 s3 p4 corAdL",     ELEM_OUTPUT_INT,        {0x5F8}, {0x4}}, \
    {"1 s3 p5 corAdL",     ELEM_OUTPUT_INT,        {0x5FC}, {0x4}}, \
    {"1 s3 p6 corAdL",     ELEM_OUTPUT_INT,        {0x600}, {0x4}}, \
    {"1 s3 p7 corAdL",     ELEM_OUTPUT_INT,        {0x604}, {0x4}}, \
    {"1 s3 p8 corAdL",     ELEM_OUTPUT_INT,        {0x608}, {0x4}}, \
    {"1 s3 p9 corAdL",     ELEM_OUTPUT_INT,        {0x60C}, {0x4}}, \
    {"1 s3 p10 corAdL",    ELEM_OUTPUT_INT,        {0x610}, {0x4}}, \
    {"1 s3 p11 corAdL",    ELEM_OUTPUT_INT,        {0x614}, {0x4}}, \
    {"1 s3 p12 corAdL",    ELEM_OUTPUT_INT,        {0x618}, {0x4}}, \
    {"1 s3 p13 corAdL",    ELEM_OUTPUT_INT,        {0x61C}, {0x4}}, \
    {"1 s3 p14 corAdL",    ELEM_OUTPUT_INT,        {0x620}, {0x4}}, \
    {"1 s3 p15 corAdL",    ELEM_OUTPUT_INT,        {0x624}, {0x4}}, \
    {"1 s0 p0 corAdH",     ELEM_OUTPUT_INT,        {0x628}, {0x4}}, \
    {"1 s0 p1 corAdH",     ELEM_OUTPUT_INT,        {0x62C}, {0x4}}, \
    {"1 s0 p2 corAdH",     ELEM_OUTPUT_INT,        {0x630}, {0x4}}, \
    {"1 s0 p3 corAdH",     ELEM_OUTPUT_INT,        {0x634}, {0x4}}, \
    {"1 s0 p4 corAdH",     ELEM_OUTPUT_INT,        {0x638}, {0x4}}, \
    {"1 s0 p5 corAdH",     ELEM_OUTPUT_INT,        {0x63C}, {0x4}}, \
    {"1 s0 p6 corAdH",     ELEM_OUTPUT_INT,        {0x640}, {0x4}}, \
    {"1 s0 p7 corAdH",     ELEM_OUTPUT_INT,        {0x644}, {0x4}}, \
    {"1 s0 p8 corAdH",     ELEM_OUTPUT_INT,        {0x648}, {0x4}}, \
    {"1 s0 p9 corAdH",     ELEM_OUTPUT_INT,        {0x64C}, {0x4}}, \
    {"1 s0 p10 corAdH",    ELEM_OUTPUT_INT,        {0x650}, {0x4}}, \
    {"1 s0 p11 corAdH",    ELEM_OUTPUT_INT,        {0x654}, {0x4}}, \
    {"1 s0 p12 corAdH",    ELEM_OUTPUT_INT,        {0x658}, {0x4}}, \
    {"1 s0 p13 corAdH",    ELEM_OUTPUT_INT,        {0x65C}, {0x4}}, \
    {"1 s0 p14 corAdH",    ELEM_OUTPUT_INT,        {0x660}, {0x4}}, \
    {"1 s0 p15 corAdH",    ELEM_OUTPUT_INT,        {0x664}, {0x4}}, \
    {"1 s1 p0 corAdH",     ELEM_OUTPUT_INT,        {0x668}, {0x4}}, \
    {"1 s1 p1 corAdH",     ELEM_OUTPUT_INT,        {0x66C}, {0x4}}, \
    {"1 s1 p2 corAdH",     ELEM_OUTPUT_INT,        {0x670}, {0x4}}, \
    {"1 s1 p3 corAdH",     ELEM_OUTPUT_INT,        {0x674}, {0x4}}, \
    {"1 s1 p4 corAdH",     ELEM_OUTPUT_INT,        {0x678}, {0x4}}, \
    {"1 s1 p5 corAdH",     ELEM_OUTPUT_INT,        {0x67C}, {0x4}}, \
    {"1 s1 p6 corAdH",     ELEM_OUTPUT_INT,        {0x680}, {0x4}}, \
    {"1 s1 p7 corAdH",     ELEM_OUTPUT_INT,        {0x684}, {0x4}}, \
    {"1 s1 p8 corAdH",     ELEM_OUTPUT_INT,        {0x688}, {0x4}}, \
    {"1 s1 p9 corAdH",     ELEM_OUTPUT_INT,        {0x68C}, {0x4}}, \
    {"1 s1 p10 corAdH",    ELEM_OUTPUT_INT,        {0x690}, {0x4}}, \
    {"1 s1 p11 corAdH",    ELEM_OUTPUT_INT,        {0x694}, {0x4}}, \
    {"1 s1 p12 corAdH",    ELEM_OUTPUT_INT,        {0x698}, {0x4}}, \
    {"1 s1 p13 corAdH",    ELEM_OUTPUT_INT,        {0x69C}, {0x4}}, \
    {"1 s1 p14 corAdH",    ELEM_OUTPUT_INT,        {0x6A0}, {0x4}}, \
    {"1 s1 p15 corAdH",    ELEM_OUTPUT_INT,        {0x6A4}, {0x4}}, \
    {"1 s2 p0 corAdH",     ELEM_OUTPUT_INT,        {0x6A8}, {0x4}}, \
    {"1 s2 p1 corAdH",     ELEM_OUTPUT_INT,        {0x6AC}, {0x4}}, \
    {"1 s2 p2 corAdH",     ELEM_OUTPUT_INT,        {0x6B0}, {0x4}}, \
    {"1 s2 p3 corAdH",     ELEM_OUTPUT_INT,        {0x6B4}, {0x4}}, \
    {"1 s2 p4 corAdH",     ELEM_OUTPUT_INT,        {0x6B8}, {0x4}}, \
    {"1 s2 p5 corAdH",     ELEM_OUTPUT_INT,        {0x6BC}, {0x4}}, \
    {"1 s2 p6 corAdH",     ELEM_OUTPUT_INT,        {0x6C0}, {0x4}}, \
    {"1 s2 p7 corAdH",     ELEM_OUTPUT_INT,        {0x6C4}, {0x4}}, \
    {"1 s2 p8 corAdH",     ELEM_OUTPUT_INT,        {0x6C8}, {0x4}}, \
    {"1 s2 p9 corAdH",     ELEM_OUTPUT_INT,        {0x6CC}, {0x4}}, \
    {"1 s2 p10 corAdH",    ELEM_OUTPUT_INT,        {0x6D0}, {0x4}}, \
    {"1 s2 p11 corAdH",    ELEM_OUTPUT_INT,        {0x6D4}, {0x4}}, \
    {"1 s2 p12 corAdH",    ELEM_OUTPUT_INT,        {0x6D8}, {0x4}}, \
    {"1 s2 p13 corAdH",    ELEM_OUTPUT_INT,        {0x6DC}, {0x4}}, \
    {"1 s2 p14 corAdH",    ELEM_OUTPUT_INT,        {0x6E0}, {0x4}}, \
    {"1 s2 p15 corAdH",    ELEM_OUTPUT_INT,        {0x6E4}, {0x4}}, \
    {"1 s3 p0 corAdH",     ELEM_OUTPUT_INT,        {0x6E8}, {0x4}}, \
    {"1 s3 p1 corAdH",     ELEM_OUTPUT_INT,        {0x6EC}, {0x4}}, \
    {"1 s3 p2 corAdH",     ELEM_OUTPUT_INT,        {0x6F0}, {0x4}}, \
    {"1 s3 p3 corAdH",     ELEM_OUTPUT_INT,        {0x6F4}, {0x4}}, \
    {"1 s3 p4 corAdH",     ELEM_OUTPUT_INT,        {0x6F8}, {0x4}}, \
    {"1 s3 p5 corAdH",     ELEM_OUTPUT_INT,        {0x6FC}, {0x4}}, \
    {"1 s3 p6 corAdH",     ELEM_OUTPUT_INT,        {0x700}, {0x4}}, \
    {"1 s3 p7 corAdH",     ELEM_OUTPUT_INT,        {0x704}, {0x4}}, \
    {"1 s3 p8 corAdH",     ELEM_OUTPUT_INT,        {0x708}, {0x4}}, \
    {"1 s3 p9 corAdH",     ELEM_OUTPUT_INT,        {0x70C}, {0x4}}, \
    {"1 s3 p10 corAdH",    ELEM_OUTPUT_INT,        {0x710}, {0x4}}, \
    {"1 s3 p11 corAdH",    ELEM_OUTPUT_INT,        {0x714}, {0x4}}, \
    {"1 s3 p12 corAdH",    ELEM_OUTPUT_INT,        {0x718}, {0x4}}, \
    {"1 s3 p13 corAdH",    ELEM_OUTPUT_INT,        {0x71C}, {0x4}}, \
    {"1 s3 p14 corAdH",    ELEM_OUTPUT_INT,        {0x720}, {0x4}}, \
    {"1 s3 p15 corAdH",    ELEM_OUTPUT_INT,        {0x724}, {0x4}}, \
}

#define DATA_MODEL_HDR_BOOT_TEE MODEL_VECTOR(HDR_BOOT_TEE) = { \
    {"magic",  ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"[BOOT CRITICAL INFO SIZE]", ELEM_OUTPUT_INT, {0x1C}, {0x4}}, \
    {"[BOOT CRITICAL INFO]", ELEM_OUTPUT_STR_NL,   {0x20}, {0x7E0}}, \
    {"[run point tail]", ELEM_OUTPUT_INT,      {0x800}, {0x4}}, \
    {"[boot point info]", ELEM_OUTPUT_HEX,     {0x804}, {0x20}}, \
    {"[run point info]", ELEM_OUTPUT_HEX,      {0x884}, {0x20}}, \
    {"[last log size]", ELEM_OUTPUT_INT,       {0xC00}, {0x4}}, \
    {"[last log data]", ELEM_OUTPUT_STR_NL,    {0xC04}, {0x3FC}}, \
}

#define DATA_MODEL_HDR_BOOT_HSM MODEL_VECTOR(HDR_BOOT_HSM) = { \
    {"magic",  ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"[HSM info]", ELEM_OUTPUT_STR_NL, {0x1C}, {0xFE4}}, \
}

#define DATA_MODEL_HDR_BOOT_ATF MODEL_VECTOR(HDR_BOOT_ATF) = { \
    {"magic",  ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"[ATF info]", ELEM_OUTPUT_STR_NL, {0x1C}, {0xFE4}}, \
}

#define DATA_MODEL_HDR_BOOT_AREA MODEL_VECTOR(HDR_BOOT_AREA) = { \
    {"BIOS INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_BOOT_BIOS", ELEM_CTRL_TABLE_GOTO, {0x0}, {0x3000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_BIOS}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"DDR INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_BOOT_DDR", ELEM_CTRL_TABLE_GOTO, {0x3000}, {0x1000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_DDR}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"HSM INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_BOOT_HSM", ELEM_CTRL_TABLE_GOTO, {0x4000}, {0x1000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_HSM}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"ATF INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_BOOT_ATF", ELEM_CTRL_TABLE_GOTO, {0x5000}, {0x1000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_ATF}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
}

#define DATA_MODEL_HDR_RUN_OS MODEL_VECTOR(HDR_RUN_OS) = { \
    {"magic", ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"[OS info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"event_flag", ELEM_OUTPUT_INT, {0x1C}, {0x4}}, \
    {"dump_flag", ELEM_OUTPUT_INT, {0x20}, {0x4}}, \
    {"err num", ELEM_OUTPUT_INT, {0x24}, {0x4}}, \
    {"[OS log]", ELEM_OUTPUT_STR_NL, {0x100}, {0xF00}}, \
}

#define DATA_MODEL_HDR_RUN_LPM MODEL_VECTOR(HDR_RUN_LPM) = { \
    {"magic",  ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0x200}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"[LPM log]", ELEM_OUTPUT_STR_NL, {0x40}, {0x400}}, \
}

#define DATA_MODEL_HDR_RUN_TEE MODEL_VECTOR(HDR_RUN_TEE) = { \
    {"magic",  ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"[RUN CRITICAL INFO SIZE]", ELEM_OUTPUT_INT, {0x1C}, {0x4}}, \
    {"[RUN CRITICAL INFO]", ELEM_OUTPUT_STR_NL, {0x20},   {0x7E0}}, \
}

#define DATA_MODEL_HDR_RUN_HSM MODEL_VECTOR(HDR_RUN_HSM) = { \
    {"magic",  ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"[RUN CRITICAL INFO SIZE]", ELEM_OUTPUT_INT, {0x1C}, {0x4}}, \
    {"[RUN CRITICAL INFO]", ELEM_OUTPUT_STR_NL, {0x20},   {0x7E0}}, \
}

#define DATA_MODEL_HDR_RUN_ATF MODEL_VECTOR(HDR_RUN_ATF) = { \
    {"magic",  ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"[ATF info]", ELEM_OUTPUT_STR_NL, {0x1C}, {0x7E4}}, \
}

#define DATA_MODEL_HDR_RUN_AREA MODEL_VECTOR(HDR_RUN_AREA) = { \
    {"HSM INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_RUN_HSM", ELEM_CTRL_TABLE_GOTO, {0x0}, {0x800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_HSM}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"ATF INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_RUN_ATF", ELEM_CTRL_TABLE_GOTO, {0x800}, {0x800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_ATF}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"LPM INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_RUN_LPM", ELEM_CTRL_TABLE_GOTO, {0x1000}, {0x1000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_LPM}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"OS INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_RUN_OS", ELEM_CTRL_TABLE_GOTO, {0x2000}, {0x1000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_OS}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
}

#define DATA_MODEL_HDR_BOOT MODEL_VECTOR(HDR_BOOT) = { \
    {"area 0", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_AREA", ELEM_CTRL_TABLE_GOTO, {0x0}, {0x7800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 1", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_AREA", ELEM_CTRL_TABLE_GOTO, {0x7800}, {0x7800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 2", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_AREA", ELEM_CTRL_TABLE_GOTO, {0xF000}, {0x7800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 3", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_AREA", ELEM_CTRL_TABLE_GOTO, {0x16800}, {0x7800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 4", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_AREA", ELEM_CTRL_TABLE_GOTO, {0x1E000}, {0x7800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 5", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_AREA", ELEM_CTRL_TABLE_GOTO, {0x25800}, {0x7800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 6", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_AREA", ELEM_CTRL_TABLE_GOTO, {0x2D000}, {0x7800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_AREA}, {0x1}}, \
}

#define DATA_MODEL_HDR_RUN MODEL_VECTOR(HDR_RUN) = { \
    {"area 0", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_AREA", ELEM_CTRL_TABLE_GOTO, {0x0}, {0x3C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 1", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_AREA", ELEM_CTRL_TABLE_GOTO, {0x3C00}, {0x3C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 2", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_AREA", ELEM_CTRL_TABLE_GOTO, {0x7800}, {0x3C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 3", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_AREA", ELEM_CTRL_TABLE_GOTO, {0xB400}, {0x3C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 4", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_AREA", ELEM_CTRL_TABLE_GOTO, {0xF000}, {0x3C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 5", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_AREA", ELEM_CTRL_TABLE_GOTO, {0x12C00}, {0x3C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 6", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_AREA", ELEM_CTRL_TABLE_GOTO, {0x16800}, {0x3C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_AREA}, {0x1}}, \
}

#define DATA_MODEL_HDR_BOOT_INFO MODEL_VECTOR(HDR_BOOT_INFO) = { \
    {"region offset", ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"region size", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"region config", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"total area", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"history area", ELEM_OUTPUT_INT, {0xC}, {0x4}}, \
    {"error area", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"area config:", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  used module count", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"module config:", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  module 0 offset", ELEM_OUTPUT_INT, {0x1C}, {0x4}}, \
    {"  module 0 size", ELEM_OUTPUT_INT, {0x20}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"  module 1 offset", ELEM_OUTPUT_INT, {0x24}, {0x4}}, \
    {"  module 1 size", ELEM_OUTPUT_INT, {0x28}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"  module 2 offset", ELEM_OUTPUT_INT, {0x2C}, {0x4}}, \
    {"  module 2 size", ELEM_OUTPUT_INT, {0x30}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"  module 3 offset", ELEM_OUTPUT_INT, {0x34}, {0x4}}, \
    {"  module 3 size", ELEM_OUTPUT_INT, {0x38}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"region control", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"area index", ELEM_OUTPUT_INT, {0x6C}, {0x4}}, \
    {"error area count", ELEM_OUTPUT_INT, {0x70}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 0 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0x74}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0x78}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0x7C}, {0x4}}, \
    {"  module id", ELEM_OUTPUT_INT, {0x80}, {0x4}}, \
    {"  exception id", ELEM_OUTPUT_INT, {0x84}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0x88}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 1 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0x8C}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0x90}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0x94}, {0x4}}, \
    {"  module id", ELEM_OUTPUT_INT, {0x98}, {0x4}}, \
    {"  exception id", ELEM_OUTPUT_INT, {0x9C}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xA0}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 2 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xA4}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xA8}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xAC}, {0x4}}, \
    {"  module id", ELEM_OUTPUT_INT, {0xB0}, {0x4}}, \
    {"  exception id", ELEM_OUTPUT_INT, {0xB4}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xB8}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 3 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xBC}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xC0}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xC4}, {0x4}}, \
    {"  module id", ELEM_OUTPUT_INT, {0xC8}, {0x4}}, \
    {"  exception id", ELEM_OUTPUT_INT, {0xCC}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xD0}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 4 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xD4}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xD8}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xDC}, {0x4}}, \
    {"  module id", ELEM_OUTPUT_INT, {0xE0}, {0x4}}, \
    {"  exception id", ELEM_OUTPUT_INT, {0xE4}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xE8}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 5 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xEC}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xF0}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xF4}, {0x4}}, \
    {"  module id", ELEM_OUTPUT_INT, {0xF8}, {0x4}}, \
    {"  exception id", ELEM_OUTPUT_INT, {0xFC}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0x100}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 6 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0x104}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0x108}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0x10C}, {0x4}}, \
    {"  module id", ELEM_OUTPUT_INT, {0x110}, {0x4}}, \
    {"  exception id", ELEM_OUTPUT_INT, {0x114}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0x118}, {0x4}}, \
}

#define DATA_MODEL_HDR_RUN_INFO MODEL_VECTOR(HDR_RUN_INFO) = { \
    {"region offset", ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"region size", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"region config", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"total area", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"history area", ELEM_OUTPUT_INT, {0xC}, {0x4}}, \
    {"error area", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"area config:", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  used module count", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"module config:", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  module 0 offset", ELEM_OUTPUT_INT, {0x1C}, {0x4}}, \
    {"  module 0 size", ELEM_OUTPUT_INT, {0x20}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"  module 1 offset", ELEM_OUTPUT_INT, {0x24}, {0x4}}, \
    {"  module 1 size", ELEM_OUTPUT_INT, {0x28}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"  module 2 offset", ELEM_OUTPUT_INT, {0x2C}, {0x4}}, \
    {"  module 2 size", ELEM_OUTPUT_INT, {0x30}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"  module 3 offset", ELEM_OUTPUT_INT, {0x34}, {0x4}}, \
    {"  module 3 size", ELEM_OUTPUT_INT, {0x38}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"region control", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"area index", ELEM_OUTPUT_INT, {0x6C}, {0x4}}, \
    {"error area count", ELEM_OUTPUT_INT, {0x70}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 0 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0x74}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0x78}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0x7C}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0x88}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 1 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0x8C}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0x90}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0x94}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xA0}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 2 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xA4}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xA8}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xAC}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xB8}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 3 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xBC}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xC0}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xC4}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xD0}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 4 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xD4}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xD8}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xDC}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xE8}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 5 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xEC}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xF0}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xF4}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0x100}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 6 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0x104}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0x108}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0x10C}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0x118}, {0x4}}, \
}

#define DATA_MODEL_HDR_LOG MODEL_VECTOR(HDR_LOG) = { \
    {"if", ELEM_CTRL_COMPARE, {0x0}, {0x4}}, \
    {"", ELEM_CTRL_CMP_JUMP_NE, {0xEAEA2020}, {0xFF}}, \
    {"head info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"magic", ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"reset count", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"boot region", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_INFO", ELEM_CTRL_TABLE_GOTO, {0XC}, {0x168}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_INFO}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"HDR_BOOT", ELEM_CTRL_TABLE_GOTO, {0x400}, {0xA000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"run region", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_INFO", ELEM_CTRL_TABLE_GOTO, {0x170}, {0x164}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_INFO}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"HDR_RUN", ELEM_CTRL_TABLE_GOTO, {0x4B400}, {0xA000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN}, {0x1}}, \
}

#define DATA_MODEL_HDR_STATUS_BLOCK_BOOT MODEL_VECTOR(HDR_STATUS_BLOCK_BOOT) = { \
    {"if",              ELEM_CTRL_COMPARE,      {0x4},          {0x1}}, \
    {"",                ELEM_CTRL_CMP_JUMP_NE,  {0x76},         {0xFF}}, \
    {"magic",           ELEM_OUTPUT_INT,        {0x0},          {0x4}}, \
    {"block id",        ELEM_OUTPUT_INT,        {0x5},          {0x1}}, \
    {"exception id",    ELEM_OUTPUT_INT,        {0x8},          {0x4}}, \
    {"expect status",   ELEM_OUTPUT_INT,        {0xC},          {0x4}}, \
    {"current status",  ELEM_OUTPUT_INT,        {0x10},         {0x4}}, \
    {"arg1",            ELEM_OUTPUT_INT,        {0x6},          {0x1}}, \
    {"arg2",            ELEM_OUTPUT_INT,        {0x7},          {0x1}}, \
    {"NL",              ELEM_OUTPUT_NL,         {0x0},          {0x0}}, \
}

#define DATA_MODEL_HDR_STATUS_BLOCK_RUN MODEL_VECTOR(HDR_STATUS_BLOCK_RUN) = { \
    {"if",              ELEM_CTRL_COMPARE,      {0x4},          {0x1}}, \
    {"",                ELEM_CTRL_CMP_JUMP_NE,  {0x77},         {0xFF}}, \
    {"magic",           ELEM_OUTPUT_INT,        {0x0},          {0x4}}, \
    {"block id",        ELEM_OUTPUT_INT,        {0x5},          {0x1}}, \
    {"exception id",    ELEM_OUTPUT_INT,        {0x8},          {0x4}}, \
    {"expect status",   ELEM_OUTPUT_INT,        {0xC},          {0x4}}, \
    {"current status",  ELEM_OUTPUT_INT,        {0x10},         {0x4}}, \
    {"arg1",            ELEM_OUTPUT_INT,        {0x6},          {0x1}}, \
    {"arg2",            ELEM_OUTPUT_INT,        {0x7},          {0x1}}, \
    {"NL",              ELEM_OUTPUT_NL,         {0x0},          {0x0}}, \
}

#define DATA_MODEL_HDR_STATUS_BLOCK MODEL_VECTOR(HDR_STATUS_BLOCK) = { \
    {"boot block", ELEM_CTRL_TABLE_GOTO, {0x0}, {0x18}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_STATUS_BLOCK_BOOT}, {0x1}}, \
    {"run block", ELEM_CTRL_TABLE_GOTO, {0x0}, {0x18}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_STATUS_BLOCK_RUN}, {0x1}}, \
}

#define DATA_MODEL_HDR_STATUS_DATA MODEL_VECTOR(HDR_STATUS_DATA) = { \
    {"area 0",              ELEM_OUTPUT_DIVIDE,     {0x0},                              {0x3D}}, \
    {"loop block",          ELEM_CTRL_LOOP_BLOCK,   {0x0},                              {0xC00}}, \
    {"block info",          ELEM_CTRL_BLOCK_VALUE,  {0x80},                             {0x18}}, \
    {"block table",         ELEM_CTRL_BLOCK_TABLE,  {PLAINTEXT_TABLE_HDR_STATUS_BLOCK}, {0x0}}, \
    {"area 1",              ELEM_OUTPUT_DIVIDE,     {0x0},                              {0x3D}}, \
    {"loop block",          ELEM_CTRL_LOOP_BLOCK,   {0xC00},                            {0xC00}}, \
    {"block info",          ELEM_CTRL_BLOCK_VALUE,  {0x80},                             {0x18}}, \
    {"block table",         ELEM_CTRL_BLOCK_TABLE,  {PLAINTEXT_TABLE_HDR_STATUS_BLOCK}, {0x0}}, \
    {"area 2",              ELEM_OUTPUT_DIVIDE,     {0x0},                              {0x3D}}, \
    {"loop block",          ELEM_CTRL_LOOP_BLOCK,   {0x1800},                           {0xC00}}, \
    {"block info",          ELEM_CTRL_BLOCK_VALUE,  {0x80},                             {0x18}}, \
    {"block table",         ELEM_CTRL_BLOCK_TABLE,  {PLAINTEXT_TABLE_HDR_STATUS_BLOCK}, {0x0}}, \
    {"area 3",              ELEM_OUTPUT_DIVIDE,     {0x0},                              {0x3D}}, \
    {"loop block",          ELEM_CTRL_LOOP_BLOCK,   {0x2400},                           {0xC00}}, \
    {"block info",          ELEM_CTRL_BLOCK_VALUE,  {0x80},                             {0x18}}, \
    {"block table",         ELEM_CTRL_BLOCK_TABLE,  {PLAINTEXT_TABLE_HDR_STATUS_BLOCK}, {0x0}}, \
    {"area 4",              ELEM_OUTPUT_DIVIDE,     {0x0},                              {0x3D}}, \
    {"loop block",          ELEM_CTRL_LOOP_BLOCK,   {0x3000},                           {0xC00}}, \
    {"block info",          ELEM_CTRL_BLOCK_VALUE,  {0x80},                             {0x18}}, \
    {"block table",         ELEM_CTRL_BLOCK_TABLE,  {PLAINTEXT_TABLE_HDR_STATUS_BLOCK}, {0x0}}, \
}

#define DATA_MODEL_HDR_STATUS MODEL_VECTOR(HDR_STATUS) = { \
    {"if",                      ELEM_CTRL_COMPARE,      {0x0},          {0x4}}, \
    {"",                        ELEM_CTRL_CMP_JUMP_NE,  {0x48445253},   {0xFF}}, \
    {"head info",               ELEM_OUTPUT_DIVIDE,     {0x0},          {0x3D}}, \
    {"magic",                   ELEM_OUTPUT_INT,        {0x0},          {0x4}}, \
    {"version",                 ELEM_OUTPUT_INT,        {0x4},          {0x4}}, \
    {"NL",                      ELEM_OUTPUT_NL,         {0x0},          {0x0}}, \
    {"config info",             ELEM_OUTPUT_DIVIDE,     {0x0},          {0x2D}}, \
    {"region offset",           ELEM_OUTPUT_INT,        {0x8},          {0x4}}, \
    {"region size",             ELEM_OUTPUT_INT,        {0xC},          {0x4}}, \
    {"area total num",          ELEM_OUTPUT_INT,        {0x10},         {0x4}}, \
    {"area used num",           ELEM_OUTPUT_INT,        {0x14},         {0x4}}, \
    {"block total num",         ELEM_OUTPUT_INT,        {0x18},         {0x4}}, \
    {"block used num",          ELEM_OUTPUT_INT,        {0x1C},         {0x4}}, \
    {"NL",                      ELEM_OUTPUT_NL,         {0x0},          {0x0}}, \
    {"control info",            ELEM_OUTPUT_DIVIDE,     {0x0},          {0x2D}}, \
    {"area index",              ELEM_OUTPUT_INT,        {0x20},         {0x4}}, \
    {"NL",                      ELEM_OUTPUT_NL,         {0x0},          {0x0}}, \
    {"[area 0 control info]",   ELEM_OUTPUT_STR_CONST,  {0x0},          {0x0}}, \
    {"  reset num",             ELEM_OUTPUT_INT,        {0x30},         {0x4}}, \
    {"  type",                  ELEM_CTRL_SWITCH,       {0x28},         {0x4}}, \
    {"current",                 ELEM_CTRL_OUT_DCASE,    {0x0},          {0x0}}, \
    {"  rw status",             ELEM_CTRL_SWITCH,       {0x2C},         {0x4}}, \
    {"writing",                 ELEM_CTRL_OUT_DCASE,    {0x0},          {0x0}}, \
    {"NL",                      ELEM_OUTPUT_NL,         {0x0},          {0x0}}, \
    {"[area 1 control info]",   ELEM_OUTPUT_STR_CONST,  {0x0},          {0x0}}, \
    {"  reset num",             ELEM_OUTPUT_INT,        {0x40},         {0x4}}, \
    {"  type",                  ELEM_CTRL_SWITCH,       {0x38},         {0x4}}, \
    {"history",                 ELEM_CTRL_OUT_CASE,     {0x4849},       {0x0}}, \
    {"unused",                  ELEM_CTRL_OUT_DCASE,    {0x0},          {0x0}}, \
    {"  rw status",             ELEM_CTRL_SWITCH,       {0x3C},         {0x4}}, \
    {"wait read",               ELEM_CTRL_OUT_CASE,     {0x5752},       {0x0}}, \
    {"read done",               ELEM_CTRL_OUT_CASE,     {0x5245},       {0x0}}, \
    {"init",                    ELEM_CTRL_OUT_DCASE,    {0x0},          {0x0}}, \
    {"  exception",             ELEM_CTRL_SWITCH,       {0x44},         {0x1}}, \
    {"true",                    ELEM_CTRL_OUT_CASE,     {0x1},          {0x0}}, \
    {"false",                   ELEM_CTRL_OUT_DCASE,    {0x0},          {0x0}}, \
    {"NL",                      ELEM_OUTPUT_NL,         {0x0},          {0x0}}, \
    {"[area 2 control info]",   ELEM_OUTPUT_STR_CONST,  {0x0},          {0x0}}, \
    {"  reset num",             ELEM_OUTPUT_INT,        {0x50},         {0x4}}, \
    {"  type",                  ELEM_CTRL_SWITCH,       {0x48},         {0x4}}, \
    {"history",                 ELEM_CTRL_OUT_CASE,     {0x4849},       {0x0}}, \
    {"unused",                  ELEM_CTRL_OUT_DCASE,    {0x0},          {0x0}}, \
    {"  rw status",             ELEM_CTRL_SWITCH,       {0x4C},         {0x4}}, \
    {"wait read",               ELEM_CTRL_OUT_CASE,     {0x5752},       {0x0}}, \
    {"read done",               ELEM_CTRL_OUT_CASE,     {0x5245},       {0x0}}, \
    {"init",                    ELEM_CTRL_OUT_DCASE,    {0x0},          {0x0}}, \
    {"  exception",             ELEM_CTRL_SWITCH,       {0x54},         {0x1}}, \
    {"true",                    ELEM_CTRL_OUT_CASE,     {0x1},          {0x0}}, \
    {"false",                   ELEM_CTRL_OUT_DCASE,    {0x0},          {0x0}}, \
    {"NL",                      ELEM_OUTPUT_NL,         {0x0},          {0x0}}, \
    {"[area 3 control info]",   ELEM_OUTPUT_STR_CONST,  {0x0},          {0x0}}, \
    {"  reset num",             ELEM_OUTPUT_INT,        {0x60},         {0x4}}, \
    {"  type",                  ELEM_CTRL_SWITCH,       {0x58},         {0x4}}, \
    {"history",                 ELEM_CTRL_OUT_CASE,     {0x4849},       {0x0}}, \
    {"unused",                  ELEM_CTRL_OUT_DCASE,    {0x0},          {0x0}}, \
    {"  rw status",             ELEM_CTRL_SWITCH,       {0x5C},         {0x4}}, \
    {"wait read",               ELEM_CTRL_OUT_CASE,     {0x5752},       {0x0}}, \
    {"read done",               ELEM_CTRL_OUT_CASE,     {0x5245},       {0x0}}, \
    {"init",                    ELEM_CTRL_OUT_DCASE,    {0x0},          {0x0}}, \
    {"  exception",             ELEM_CTRL_SWITCH,       {0x64},         {0x1}}, \
    {"true",                    ELEM_CTRL_OUT_CASE,     {0x1},          {0x0}}, \
    {"false",                   ELEM_CTRL_OUT_DCASE,    {0x0},          {0x0}}, \
    {"NL",                      ELEM_OUTPUT_NL,         {0x0},          {0x0}}, \
    {"[area 4 control info]",   ELEM_OUTPUT_STR_CONST,  {0x0},          {0x0}}, \
    {"  reset num",             ELEM_OUTPUT_INT,        {0x70},         {0x4}}, \
    {"  type",                  ELEM_CTRL_SWITCH,       {0x68},         {0x4}}, \
    {"history",                 ELEM_CTRL_OUT_CASE,     {0x4849},       {0x0}}, \
    {"unused",                  ELEM_CTRL_OUT_DCASE,    {0x0},          {0x0}}, \
    {"  rw status",             ELEM_CTRL_SWITCH,       {0x6C},         {0x4}}, \
    {"wait read",               ELEM_CTRL_OUT_CASE,     {0x5752},       {0x0}}, \
    {"read done",               ELEM_CTRL_OUT_CASE,     {0x5245},       {0x0}}, \
    {"init",                    ELEM_CTRL_OUT_DCASE,    {0x0},          {0x0}}, \
    {"  exception",             ELEM_CTRL_SWITCH,       {0x74},         {0x1}}, \
    {"true",                    ELEM_CTRL_OUT_CASE,     {0x1},          {0x0}}, \
    {"false",                   ELEM_CTRL_OUT_DCASE,    {0x0},          {0x0}}, \
    {"NL",                      ELEM_OUTPUT_NL,         {0x0},          {0x0}}, \
    {"HDR_STATUS_DATA",         ELEM_CTRL_TABLE_GOTO,   {0x400},        {0x7800}}, \
    {"table_index",             ELEM_CTRL_TABLE_RANGE,  {PLAINTEXT_TABLE_HDR_STATUS_DATA}, {0x1}}, \
}

#define DATA_MODEL_HDR MODEL_VECTOR(HDR) = { \
    {"HDR_BOOT", ELEM_CTRL_TABLE_GOTO, {0x0}, {0x70C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_LOG}, {0x1}}, \
    {"HDR_STATUS", ELEM_CTRL_TABLE_GOTO, {0x70C00}, {0x7C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_STATUS}, {0x1}}, \
}

#endif
