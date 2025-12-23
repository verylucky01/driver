/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef BBOX_CDR_DATA_MILAN_H
#define BBOX_CDR_DATA_MILAN_H

#include "bbox_ddr_data.h"

/**
 *  the whole space is 512k, used for history data record
 *  the struct distribution is as follows:
 *  +-------------------+     +-------------------+
 *  | head info(1k)     |---->| magic             |
 *  +-------------------+     +-------------------+
 *  | MIN region(48k)   |     | version           |
 *  +-------------------+     +-------------------+
 *  | reserved(1k)      |     | reserved[24]      |
 *  +-------------------+     +-------------------+
 *  | FULL region(4M)   |     | area head[6]      |
 *  +-------------------+     +-------------------+
 */
#define DATA_MODEL_CDR_MIN MODEL_VECTOR(CDR_MIN) = { \
    {"AA KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0xC}, {0x220}}, \
    {"MULTI RING KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x22C}, {0xC80}}, \
    {"L2 BUF KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0xEAC}, {0x600}}, \
    {"L3D KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x14AC}, {0x60}}, \
    {"L3T KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x150C}, {0x70}}, \
    {"MATA KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x157C}, {0x1C0}}, \
    {"MN KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x173C}, {0x20}}, \
    {"SCHE KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x175C}, {0xD0}}, \
    {"DISPATCH KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x182C}, {0x330}}, \
    {"SIOE KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x1B5C}, {0x28}}, \
    {"SLLC KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x1B84}, {0x10}}, \
    {"SMMU KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x1B94}, {0x7F8}}, \
    {"GIC KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x238C}, {0x364}}, \
    {"PA KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x26F0}, {0x68}}, \
    {"HDLC KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x2758}, {0x120}}, \
    {"HPCS KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x2878}, {0x420}}, \
    {"HILINK X36 KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x2C98}, {0x0}}, \
    {"HBMC KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x2C98}, {0x7300}}, \
    {"RAS KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0x9F98}, {0x300}}, \
    {"ARER KeyInfo", ELEM_OUTPUT_R4_BLOCK, {0xA298}, {0xC00}}, \
    {"HBM Info", ELEM_OUTPUT_R4_BLOCK, {0xAE98}, {0xCB0}}, \
}

#define DATA_MODEL_CDR_FULL MODEL_VECTOR(CDR_FULL) = { \
    {"AA FullInfo", ELEM_OUTPUT_R4_BLOCK, {0xC}, {0x3DA0}}, \
    {"MULTI RING FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x3DAC}, {0x21020}}, \
    {"L2 Buf FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x24DCC}, {0x97280}}, \
    {"L3D FullInfo", ELEM_OUTPUT_R4_BLOCK, {0xbc04c}, {0x9638}}, \
    {"L3T FullInfo", ELEM_OUTPUT_R4_BLOCK, {0xc5684}, {0xa550}}, \
    {"MATA FullInfo", ELEM_OUTPUT_R4_BLOCK, {0xcfbd4}, {0x3eb40}}, \
    {"MN FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x10e714}, {0xac20}}, \
    {"SCHE FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x119334}, {0x98C}}, \
    {"DISP FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x119cc0}, {0x3300}}, \
    {"SMMU FUllInfo", ELEM_OUTPUT_R4_BLOCK, {0x11cfc0}, {0xa92b8}}, \
    {"PA FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x1c6278}, {0xbf70}}, \
    {"HBMC FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x1d21e8}, {0x22b00}}, \
    {"CPU FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x1f4ce8}, {0x4bf0}}, \
    {"SDMA FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x1f98d8}, {0x10e0}}, \
    {"SDMA COMMON FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x1fa9b8}, {0x2de0}}, \
    {"AICORE FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x1fd798}, {0x36e20}}, \
    {"PCIE FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x2345b8}, {0x430}}, \
    {"STARS FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x2349e8}, {0x5c54}}, \
    {"SMMU EVENT FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x23a63c}, {0x8e60}}, \
    {"SMMU CMD FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x24349c}, {0x7f8}}, \
    {"SIOE FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x243c94}, {0xfc60}}, \
    {"HDLC FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x2538f4}, {0x4d90}}, \
    {"SLLC FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x258684}, {0x5910}}, \
    {"HPCS FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x25df94}, {0x4a40}}, \
    {"HILINK32 FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x2629d4}, {0x3df0}}, \
    {"HILINK60 FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x2667c4}, {0x5480}}, \
    {"PA ONLINE FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x26bc44}, {0x4528}}, \
    {"AA FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x27016c}, {0x3DA0}}, \
    {"MULTI RING FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x273f0c}, {0x21020}}, \
    {"L2 Buf FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x294f2c}, {0x97280}}, \
    {"L3D FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x32c1ac}, {0x9638}}, \
    {"L3T FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x3357e4}, {0xa550}}, \
    {"MATA FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x33fd34}, {0x3eb40}}, \
    {"MN FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x37e874}, {0xac20}}, \
    {"SCHE FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x389494}, {0x98C}}, \
    {"DISP FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x389e20}, {0x3300}}, \
    {"SMMU FUllInfo", ELEM_OUTPUT_R4_BLOCK, {0x38d120}, {0xa92b8}}, \
    {"PA FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x4363d8}, {0xbf70}}, \
    {"HBMC FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x442348}, {0x22b00}}, \
    {"CPU FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x464e48}, {0x4bf0}}, \
    {"SDMA FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x469a38}, {0x10e0}}, \
    {"SDMA COMMON FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x46ab18}, {0x2de0}}, \
    {"AICORE FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x46d8f8}, {0x36e20}}, \
    {"PCIE FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x4a4718}, {0x430}}, \
    {"STARS FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x4a4b48}, {0x5c54}}, \
    {"SMMU EVENT FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x4aa79c}, {0x8e60}}, \
    {"SMMU CMD FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x4b35fc}, {0x7f8}}, \
    {"SIOE FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x4b3df4}, {0xfc60}}, \
    {"HDLC FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x4c3a54}, {0x4d90}}, \
    {"SLLC FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x4c87e4}, {0x5910}}, \
    {"HPCS FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x4ce0f4}, {0x4a40}}, \
    {"HILINK32 FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x4d2b34}, {0x3df0}}, \
    {"HILINK60 FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x4d6924}, {0x5480}}, \
    {"PA ONLINE FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x4dbda4}, {0x4528}}, \
}

#define DATA_MODEL_CDR_SRAM MODEL_VECTOR(CDR_SRAM) = { \
    {"chip dfx min info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"if", ELEM_CTRL_COMPARE, {0x4}, {0x4}}, \
    {"magic", ELEM_CTRL_CMP_JUMP_NE, {0x63686970}, {0xFF}}, \
    {"version", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0x0}, {0x1}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"CDR_MIN", ELEM_CTRL_TABLE_GOTO, {0x0}, {0xC000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_CDR_SRAM_MIN}, {0x1}}, \
}

#define DATA_MODEL_CDR MODEL_VECTOR(CDR) = { \
    {"chip dfx info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"if", ELEM_CTRL_COMPARE, {0x0}, {0x4}}, \
    {"magic", ELEM_CTRL_CMP_JUMP_NE, {0x63686970}, {0xFF}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"trigType", ELEM_OUTPUT_INT, {0x32}, {0x1}}, \
    {"dump_version", ELEM_OUTPUT_INT, {0x33}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"min info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"if", ELEM_CTRL_COMPARE, {0x20}, {0x1}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0x7}}, \
    {"flag", ELEM_OUTPUT_INT, {0x20}, {0x1}}, \
    {"type", ELEM_OUTPUT_INT, {0x21}, {0x1}}, \
    {"offset", ELEM_OUTPUT_INT, {0x28}, {0x4}}, \
    {"length", ELEM_OUTPUT_INT, {0x2C}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"CDR_MIN", ELEM_CTRL_TABLE_GOTO, {0x400}, {0xC000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_CDR_MIN}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"full info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"if", ELEM_CTRL_COMPARE, {0x30}, {0x1}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0x7}}, \
    {"flag", ELEM_OUTPUT_INT, {0x30}, {0x1}}, \
    {"type", ELEM_OUTPUT_INT, {0x31}, {0x1}}, \
    {"offset", ELEM_OUTPUT_INT, {0x38}, {0x4}}, \
    {"length", ELEM_OUTPUT_INT, {0x3C}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"CDR_FULL", ELEM_CTRL_TABLE_GOTO, {0xC800}, {0x800000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_CDR_FULL}, {0x1}}, \
}

#define DATA_MODEL_CDR_SRAM_LOOSE MODEL_VECTOR(CDR_SRAM_LOOSE) = { \
    {"chip dfx min info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"if", ELEM_CTRL_COMPARE, {0x4}, {0x4}}, \
    {"magic", ELEM_CTRL_CMP_JUMP_NE, {0x63686970}, {0xFF}}, \
    {"version", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"flag", ELEM_OUTPUT_INT, {0x0}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"CDR_MIN", ELEM_CTRL_TABLE_GOTO, {0x0}, {0xC000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_CDR_SRAM_MIN}, {0x1}}, \
}

#define DATA_MODEL_CDR_LOOSE MODEL_VECTOR(CDR_LOOSE) = { \
    {"chip dfx info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"if", ELEM_CTRL_COMPARE, {0x0}, {0x4}}, \
    {"magic", ELEM_CTRL_CMP_JUMP_NE, {0x63686970}, {0xFF}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"trigType", ELEM_OUTPUT_INT, {0x32}, {0x1}}, \
    {"dump_version", ELEM_OUTPUT_INT, {0x33}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"min info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"flag", ELEM_OUTPUT_INT, {0x20}, {0x1}}, \
    {"type", ELEM_OUTPUT_INT, {0x21}, {0x1}}, \
    {"offset", ELEM_OUTPUT_INT, {0x28}, {0x4}}, \
    {"length", ELEM_OUTPUT_INT, {0x2C}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"CDR_MIN", ELEM_CTRL_TABLE_GOTO, {0x400}, {0xC000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_CDR_MIN}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"full info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"flag", ELEM_OUTPUT_INT, {0x30}, {0x1}}, \
    {"type", ELEM_OUTPUT_INT, {0x31}, {0x1}}, \
    {"offset", ELEM_OUTPUT_INT, {0x38}, {0x4}}, \
    {"length", ELEM_OUTPUT_INT, {0x3C}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"CDR_FULL", ELEM_CTRL_TABLE_GOTO, {0xC800}, {0x800000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_CDR_FULL}, {0x1}}, \
}
#endif
