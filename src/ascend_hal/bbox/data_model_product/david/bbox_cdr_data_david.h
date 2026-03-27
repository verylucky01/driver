/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef BBOX_CDR_DATA_DAVID_H
#define BBOX_CDR_DATA_DAVID_H

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
    {"Aic Key", ELEM_OUTPUT_R4_BLOCK, {0xC}, {0xb40}}, \
    {"Aiv Key", ELEM_OUTPUT_R4_BLOCK, {0xb4C}, {0x1440}}, \
    {"L2b Key", ELEM_OUTPUT_R4_BLOCK, {0x1f8C}, {0x400}}, \
    {"L3d Key", ELEM_OUTPUT_R4_BLOCK, {0x238C}, {0x80}}, \
    {"L3t Key", ELEM_OUTPUT_R4_BLOCK, {0x240C}, {0x80}}, \
    {"Aa Key", ELEM_OUTPUT_R4_BLOCK, {0x248C}, {0x460}}, \
    {"Disp Key", ELEM_OUTPUT_R4_BLOCK, {0x28EC}, {0x508}}, \
    {"Sdma Key", ELEM_OUTPUT_R4_BLOCK, {0x2DF4}, {0x20}}, \
    {"Cpu Key", ELEM_OUTPUT_R4_BLOCK, {0x2E14}, {0x220}}, \
    {"Mn Key", ELEM_OUTPUT_R4_BLOCK, {0x3034}, {0x70}}, \
    {"Sche Key", ELEM_OUTPUT_R4_BLOCK, {0x30A4}, {0x78}}, \
    {"Asmb Key", ELEM_OUTPUT_R4_BLOCK, {0x311C}, {0x820}}, \
    {"Bailu Key", ELEM_OUTPUT_R4_BLOCK, {0x393C}, {0x540}}, \
    {"Smmu Key", ELEM_OUTPUT_R4_BLOCK, {0x3E7C}, {0x1260}}, \
    {"Ha Key", ELEM_OUTPUT_R4_BLOCK, {0x50DC}, {0x300}}, \
    {"Pcie Key", ELEM_OUTPUT_R4_BLOCK, {0x53DC}, {0x368}}, \
    {"Sllc Key", ELEM_OUTPUT_R4_BLOCK, {0x5744}, {0x1080}}, \
    {"Hscb Key", ELEM_OUTPUT_R4_BLOCK, {0x67C4}, {0x130}}, \
    {"BA Key", ELEM_OUTPUT_R4_BLOCK, {0x68F4}, {0x20a0}}, \
    {"MISC Key", ELEM_OUTPUT_R4_BLOCK, {0x8994}, {0xf28}}, \
    {"NL Key", ELEM_OUTPUT_R4_BLOCK, {0x98BC}, {0x1068}}, \
    {"TA Key", ELEM_OUTPUT_R4_BLOCK, {0xA924}, {0x530}}, \
    {"TP Key", ELEM_OUTPUT_R4_BLOCK, {0xAE54}, {0x1358}}, \
    {"UB_CCU Key", ELEM_OUTPUT_R4_BLOCK, {0xC1AC}, {0x9d8}}, \
    {"UMMU Key", ELEM_OUTPUT_R4_BLOCK, {0xCB84}, {0x1b0}}, \
}

#define DATA_MODEL_CDR_FULL MODEL_VECTOR(CDR_FULL) = { \
    {"AA FullInfo", ELEM_OUTPUT_R4_BLOCK, {0xc}, {0xa87e0}}, \
    {"AIC FullInfo", ELEM_OUTPUT_R4_BLOCK, {0xa87ec}, {0x49830}}, \
    {"AIV FullInfo", ELEM_OUTPUT_R4_BLOCK, {0xf201c}, {0xde780}}, \
    {"ASMB FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x1d079c}, {0x820}}, \
    {"BAILU FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x1d0fbc}, {0x3c80}}, \
    {"CPU FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x1d4c3c}, {0x7020}}, \
    {"CPU FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x1dbc5c}, {0x7020}}, \
    {"CS FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x1e2c7c}, {0x276e0}}, \
    {"DISP FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x20a35c}, {0x52a8}}, \
    {"DLPHY FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x20f604}, {0xe1c8}}, \
    {" EFUSE FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x21d7cc}, {0x4580}}, \
    {"HILINK FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x221d4c}, {0x13c20}}, \
    {"HSCB FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x23596c}, {0xc98}}, \
    {"L2B FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x236604}, {0x60680}}, \
    {"L3D FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x296c84}, {0xc880}}, \
    {"L3T FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x2a3504}, {0x10680}}, \
    {"MN FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x2b3b84}, {0x14880}}, \
    {"PCIE FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x2c8404}, {0x1070}}, \
    {"PLL FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x2c9474}, {0x624}}, \
    {"PMC FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x2c9a98}, {0x1288}}, \
    {"PPU FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x2cad20}, {0x4af8}}, \
    {"SCHE FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x2cf818}, {0x1e410}}, \
    {"SDMA FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x2edc28}, {0x71e0}}, \
    {"SIOE FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x2f4e08}, {0x5d6e0}}, \
    {"SLLC FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x3524e8}, {0x90780}}, \
    {"SMMU FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x3e2c68}, {0x1892b0}}, \
    {"STARS FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x56bf18}, {0x9e28}}, \
    {"SUBSTRL FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x575d40}, {0xa08}}, \
    {"UB_BA FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x576748}, {0xff88}}, \
    {"UB_CCU FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x5866d0}, {0x6318}}, \
    {"UB_MISC FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x58c9e8}, {0x2360}}, \
    {"UB_NL FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x58ed48}, {0x32408}}, \
    {"UB_TA FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x5c1150}, {0xb40}}, \
    {"UB_TP FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x5c1c90}, {0x29b8}}, \
    {"UB_UMMU FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x5c4648}, {0x1940}}, \
    {"VPC FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x5c5f88}, {0x357e0}}, \
    {"HA FullInfo", ELEM_OUTPUT_R4_BLOCK, {0x5fb768}, {0x1a2400}}, \
}

#define DATA_MODEL_CDR_SRAM MODEL_VECTOR(CDR_SRAM) = { \
    {"chip dfx min info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"if", ELEM_CTRL_COMPARE, {0x4}, {0x4}}, \
    {"magic", ELEM_CTRL_CMP_JUMP_NE, {0x63686970}, {0xFF}}, \
    {"version", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0x0}, {0x1}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"CDR_MIN", ELEM_CTRL_TABLE_GOTO, {0x0}, {0x18000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_CDR_SRAM_MIN}, {0x1}}, \
}

#define DATA_MODEL_CDR MODEL_VECTOR(CDR) = { \
    {"chip dfx info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"if", ELEM_CTRL_COMPARE, {0x0}, {0x4}}, \
    {"magic", ELEM_CTRL_CMP_JUMP_NE, {0x63686970}, {0xFF}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"trigType", ELEM_OUTPUT_INT, {0x32}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"min info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"if", ELEM_CTRL_COMPARE, {0x40}, {0x1}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0x7}}, \
    {"flag", ELEM_OUTPUT_INT, {0x20}, {0x1}}, \
    {"type", ELEM_OUTPUT_INT, {0x21}, {0x1}}, \
    {"offset", ELEM_OUTPUT_INT, {0x28}, {0x4}}, \
    {"length", ELEM_OUTPUT_INT, {0x2C}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"CDR_MIN", ELEM_CTRL_TABLE_GOTO, {0x400}, {0x18300}}, \
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
    {"CDR_FULL", ELEM_CTRL_TABLE_GOTO, {0x18800}, {0x13FFC00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_CDR_FULL}, {0x1}}, \
}

#define DATA_MODEL_CDR_SRAM_LOOSE MODEL_VECTOR(CDR_SRAM_LOOSE) = { \
    {"chip dfx min info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"if", ELEM_CTRL_COMPARE, {0x4}, {0x4}}, \
    {"magic", ELEM_CTRL_CMP_JUMP_NE, {0x63686970}, {0xFF}}, \
    {"version", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"flag", ELEM_OUTPUT_INT, {0x0}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"CDR_MIN", ELEM_CTRL_TABLE_GOTO, {0x0}, {0x18000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_CDR_SRAM_MIN}, {0x1}}, \
}

#define DATA_MODEL_CDR_LOOSE MODEL_VECTOR(CDR_LOOSE) = { \
    {"chip dfx info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"if", ELEM_CTRL_COMPARE, {0x0}, {0x4}}, \
    {"magic", ELEM_CTRL_CMP_JUMP_NE, {0x63686970}, {0xFF}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"trigType", ELEM_OUTPUT_INT, {0x32}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"min info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"flag", ELEM_OUTPUT_INT, {0x20}, {0x1}}, \
    {"type", ELEM_OUTPUT_INT, {0x21}, {0x1}}, \
    {"offset", ELEM_OUTPUT_INT, {0x28}, {0x4}}, \
    {"length", ELEM_OUTPUT_INT, {0x2C}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"CDR_MIN", ELEM_CTRL_TABLE_GOTO, {0x400}, {0x18000}}, \
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
