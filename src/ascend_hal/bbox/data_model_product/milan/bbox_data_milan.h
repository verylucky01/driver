/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef BBOX_DATA_MILAN_H
#define BBOX_DATA_MILAN_H

#include "bbox_ddr_data.h"
#include "bbox_hdr_data_milan.h"
#include "bbox_cdr_data_milan.h"

#define DATA_MODEL_LPM_START MODEL_VECTOR(LPM_START) = { \
    {"startup_point", ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
}

/* each Module need define as follows */
#define DATA_MODEL_LPM MODEL_VECTOR(LPM) = { \
    {"excep_reason",        ELEM_OUTPUT_INT,    {0x0000},   {0x0004}}, \
    {"excep_recovery",      ELEM_OUTPUT_INT,    {0x0004},   {0x0004}}, \
    {"excep_time_sec_l",    ELEM_OUTPUT_INT,    {0x0008},   {0x0004}}, \
    {"excep_time_sec_h",    ELEM_OUTPUT_INT,    {0x000c},   {0x0004}}, \
    {"excep_time_usec_l",   ELEM_OUTPUT_INT,    {0x0010},   {0x0004}}, \
    {"excep_time_usec_h",   ELEM_OUTPUT_INT,    {0x0014},   {0x0004}}, \
    {"aic_freq_curr",       ELEM_OUTPUT_INT,    {0x1400},   {0x0002}}, \
    {"aic_freq_limit",      ELEM_OUTPUT_INT,    {0x1402},   {0x0002}}, \
    {"cpu_freq_curr",       ELEM_OUTPUT_INT,    {0x1404},   {0x0002}}, \
    {"cpu_freq_limit",      ELEM_OUTPUT_INT,    {0x1406},   {0x0002}}, \
    {"ring_freq_curr",      ELEM_OUTPUT_INT,    {0x1408},   {0x0002}}, \
    {"ring_freq_limit",     ELEM_OUTPUT_INT,    {0x140a},   {0x0002}}, \
    {"mata_freq_curr",      ELEM_OUTPUT_INT,    {0x140c},   {0x0002}}, \
    {"mata_freq_limit",     ELEM_OUTPUT_INT,    {0x140e},   {0x0002}}, \
    {"l2buf_freq_curr",     ELEM_OUTPUT_INT,    {0x1410},   {0x0002}}, \
    {"l2buf_freq_limit",    ELEM_OUTPUT_INT,    {0x1412},   {0x0002}}, \
    {"log",                 ELEM_OUTPUT_STR_NL, {0x1800},   {0x0400}}, \
}

/* HSM module */
#define DATA_MODEL_HSM MODEL_VECTOR(HSM) = { \
    {"hsm info", ELEM_OUTPUT_STR_NL, {0x0}, {0x1000}}, \
}

/* HSM module, start */
#define DATA_MODEL_HSM_START MODEL_VECTOR(HSM_START) = { \
    {"hsm start info", ELEM_OUTPUT_STR_NL, {0x0}, {0x1000}}, \
}

/* HSM module, log */
#define DATA_MODEL_HSM_LOG MODEL_VECTOR(HSM_LOG) = { \
    {"log level",       ELEM_OUTPUT_INT,      {0x84},     {0x4}}, \
    {"hsm log info",    ELEM_OUTPUT_STR_NL,   {0x100}, {0xFFF00}}, \
}

/* TEE module */
#define DATA_MODEL_TEE MODEL_VECTOR(TEE) = { \
    {"tee info", ELEM_OUTPUT_STR_NL, {0x0}, {0x10000}}, \
}

/* TF module */
#define DATA_MODEL_TF MODEL_VECTOR(TF) = { \
    {"x0",           ELEM_OUTPUT_HEX, {0x0},   {0x8}}, \
    {"x1",           ELEM_OUTPUT_HEX, {0x8},   {0x8}}, \
    {"x30",          ELEM_OUTPUT_HEX, {0x10},  {0x8}}, \
    {"x2",           ELEM_OUTPUT_HEX, {0x18},  {0x8}}, \
    {"x3",           ELEM_OUTPUT_HEX, {0x20},  {0x8}}, \
    {"x4",           ELEM_OUTPUT_HEX, {0x28},  {0x8}}, \
    {"x5",           ELEM_OUTPUT_HEX, {0x30},  {0x8}}, \
    {"x6",           ELEM_OUTPUT_HEX, {0x38},  {0x8}}, \
    {"x7",           ELEM_OUTPUT_HEX, {0x40},  {0x8}}, \
    {"x8",           ELEM_OUTPUT_HEX, {0x48},  {0x8}}, \
    {"x9",           ELEM_OUTPUT_HEX, {0x50},  {0x8}}, \
    {"x10",          ELEM_OUTPUT_HEX, {0x58},  {0x8}}, \
    {"x11",          ELEM_OUTPUT_HEX, {0x60},  {0x8}}, \
    {"x12",          ELEM_OUTPUT_HEX, {0x68},  {0x8}}, \
    {"x13",          ELEM_OUTPUT_HEX, {0x70},  {0x8}}, \
    {"x14",          ELEM_OUTPUT_HEX, {0x78},  {0x8}}, \
    {"x15",          ELEM_OUTPUT_HEX, {0x80},  {0x8}}, \
    {"x16",          ELEM_OUTPUT_HEX, {0x88},  {0x8}}, \
    {"x17",          ELEM_OUTPUT_HEX, {0x90},  {0x8}}, \
    {"x18",          ELEM_OUTPUT_HEX, {0x98},  {0x8}}, \
    {"x19",          ELEM_OUTPUT_HEX, {0xA0},  {0x8}}, \
    {"x20",          ELEM_OUTPUT_HEX, {0xA8},  {0x8}}, \
    {"x21",          ELEM_OUTPUT_HEX, {0xB0},  {0x8}}, \
    {"x22",          ELEM_OUTPUT_HEX, {0xB8},  {0x8}}, \
    {"x23",          ELEM_OUTPUT_HEX, {0xC0},  {0x8}}, \
    {"x24",          ELEM_OUTPUT_HEX, {0xC8},  {0x8}}, \
    {"x25",          ELEM_OUTPUT_HEX, {0xD0},  {0x8}}, \
    {"x26",          ELEM_OUTPUT_HEX, {0xD8},  {0x8}}, \
    {"x27",          ELEM_OUTPUT_HEX, {0xE0},  {0x8}}, \
    {"x28",          ELEM_OUTPUT_HEX, {0xE8},  {0x8}}, \
    {"x29",          ELEM_OUTPUT_HEX, {0xF0},  {0x8}}, \
    {"scr_el3",      ELEM_OUTPUT_HEX, {0xF8},  {0x8}}, \
    {"sctlr_el3",    ELEM_OUTPUT_HEX, {0x100}, {0x8}}, \
    {"cptr_el3",     ELEM_OUTPUT_HEX, {0x108}, {0x8}}, \
    {"tcr_el3",      ELEM_OUTPUT_HEX, {0x110}, {0x8}}, \
    {"daif",         ELEM_OUTPUT_HEX, {0x118}, {0x8}}, \
    {"mair_el3",     ELEM_OUTPUT_HEX, {0x120}, {0x8}}, \
    {"spsr_el3",     ELEM_OUTPUT_HEX, {0x128}, {0x8}}, \
    {"elr_el3",      ELEM_OUTPUT_HEX, {0x130}, {0x8}}, \
    {"ttbr0_el3",    ELEM_OUTPUT_HEX, {0x138}, {0x8}}, \
    {"esr_el3",      ELEM_OUTPUT_HEX, {0x140}, {0x8}}, \
    {"far_el3",      ELEM_OUTPUT_HEX, {0x148}, {0x8}}, \
    {"log info",     ELEM_OUTPUT_STR_NL, {0x208}, {0x77F8}}, \
    {"bak log info", ELEM_OUTPUT_STR_NL, {0x7A08}, {0x77F8}}, \
}

/* DVPP module */
#define DATA_MODEL_DVPP MODEL_VECTOR(DVPP) = { \
    {"dvpp info", ELEM_OUTPUT_STR_NL, {0x0}, {0x10000}}, \
}

/* DRIVE module */
#define DATA_MODEL_DRIVER MODEL_VECTOR(DRIVER) = { \
    {"driver info", ELEM_OUTPUT_STR_NL, {0x0}, {0x20000}}, \
}

/* TS module */
#define DATA_MODEL_TS MODEL_VECTOR(TS) = { \
    {"ts info", ELEM_OUTPUT_CHAR, {0x0}, {0x100000}}, \
}

/* TS module, start */
#define DATA_MODEL_TS_START MODEL_VECTOR(TS_START) = { \
    {"ts start info", ELEM_OUTPUT_STR_NL, {0x0}, {0xC800}}, \
}

/* TS module, ts log */
#define DATA_MODEL_TS_LOG MODEL_VECTOR(TS_LOG) = { \
    {"ts log info",   ELEM_OUTPUT_STR_NL,   {0x100}, {0xFFF00UL}}, \
}

/* LOG module, run or debug device os log : 3 * 1M */
#define DATA_MODEL_DEVICE_OS_LOG MODEL_VECTOR(DEVICE_OS_LOG) = { \
    {"device_os.log.0", ELEM_OUTPUT_STR_NL, {0x50}, {0xFFFB0}}, \
    {"device_os.log.1", ELEM_OUTPUT_STR_NL, {0x100050}, {0xFFFB0}}, \
    {"device_os.log.2", ELEM_OUTPUT_STR_NL, {0x200050}, {0xFFFB0}},  \
}

/* LOG module, debug device id log : 20 * 1M */
#define DATA_MODEL_DEVICE_FW_LOG MODEL_VECTOR(DEVICE_FW_LOG) = { \
    {"device_fw.log.0", ELEM_OUTPUT_STR_NL,  {0x50},    {0xFFFB0}}, \
    {"device_fw.log.1", ELEM_OUTPUT_STR_NL,  {0x100050},  {0xFFFB0}}, \
    {"device_fw.log.2", ELEM_OUTPUT_STR_NL,  {0x200050},  {0xFFFB0}}, \
    {"device_fw.log.3", ELEM_OUTPUT_STR_NL,  {0x300050},  {0xFFFB0}}, \
    {"device_fw.log.4", ELEM_OUTPUT_STR_NL,  {0x400050}, {0xFFFB0}}, \
    {"device_fw.log.5", ELEM_OUTPUT_STR_NL,  {0x500050}, {0xFFFB0}}, \
    {"device_fw.log.6", ELEM_OUTPUT_STR_NL,  {0x600050}, {0xFFFB0}}, \
    {"device_fw.log.7", ELEM_OUTPUT_STR_NL,  {0x700050}, {0xFFFB0}}, \
    {"device_fw.log.8", ELEM_OUTPUT_STR_NL,  {0x800050}, {0xFFFB0}}, \
    {"device_fw.log.9", ELEM_OUTPUT_STR_NL,  {0x900050}, {0xFFFB0}}, \
    {"device_fw.log.10", ELEM_OUTPUT_STR_NL, {0xA00050}, {0xFFFB0}}, \
    {"device_fw.log.11", ELEM_OUTPUT_STR_NL, {0xB00050}, {0xFFFB0}}, \
    {"device_fw.log.12", ELEM_OUTPUT_STR_NL, {0xC00050}, {0xFFFB0}}, \
    {"device_fw.log.13", ELEM_OUTPUT_STR_NL, {0xD00050}, {0xFFFB0}}, \
    {"device_fw.log.14", ELEM_OUTPUT_STR_NL, {0xE00050}, {0xFFFB0}}, \
    {"device_fw.log.15", ELEM_OUTPUT_STR_NL, {0xF00050}, {0xFFFB0}}, \
    {"device_fw.log.16", ELEM_OUTPUT_STR_NL, {0x1000050}, {0xFFFB0}}, \
    {"device_fw.log.17", ELEM_OUTPUT_STR_NL, {0x1100050}, {0xFFFB0}}, \
    {"device_fw.log.18", ELEM_OUTPUT_STR_NL, {0x1200050}, {0xFFFB0}}, \
    {"device_fw.log.19", ELEM_OUTPUT_STR_NL, {0x1300050}, {0xFFFB0}}, \
}

/* LOG module, run event log : 2 * 1M */
#define DATA_MODEL_RUN_EVENT_LOG MODEL_VECTOR(RUN_EVENT_LOG) = { \
    {"run_event.log.0", ELEM_OUTPUT_STR_NL,  {0x50},    {0xFFFB0}}, \
    {"run_event.log.1", ELEM_OUTPUT_STR_NL,  {0x100050},  {0xFFFB0}}, \
}

/* LOG module, sec log : 2 * 1M */
#define DATA_MODEL_SEC_LOG MODEL_VECTOR(SEC_LOG) = { \
    {"sec.log.0", ELEM_OUTPUT_STR_NL,  {0x50},    {0xFFFB0}}, \
    {"sec.log.1", ELEM_OUTPUT_STR_NL,  {0x100050},  {0xFFFB0}}, \
}

/* BIOS module */
#define DATA_MODEL_BIOS MODEL_VECTOR(BIOS) = { \
    {"bios info", ELEM_OUTPUT_STR_NL, {0x0}, {0x50000}}, \
}

/* BIOS module, sram */
#define DATA_MODEL_BIOS_SRAM MODEL_VECTOR(BIOS_SRAM) = { \
    {"LPM3_WAKE_UP_STATUS",             ELEM_OUTPUT_INT, {0x0},   {0x4}}, \
    {"DEBUG_TIME_POWERUP_DONE",         ELEM_OUTPUT_INT, {0x28},  {0x4}}, \
    {"DEBUG_TIME_PERSTHIGH_DONE",       ELEM_OUTPUT_INT, {0x2C},  {0x4}}, \
    {"DEBUG_TIME_PCIEPHY_DONE",         ELEM_OUTPUT_INT, {0x30},  {0x4}}, \
    {"DEBUG_TIME_PHY_FIRMWARE_DONE",    ELEM_OUTPUT_INT, {0x34},  {0x4}}, \
    {"DEBUG_TIME_PCIECTRL_DONE",        ELEM_OUTPUT_INT, {0x38},  {0x4}}, \
    {"DEBUG_TIME_IMG_DONE",             ELEM_OUTPUT_INT, {0x3C},  {0x4}}, \
    {"DEBUG_TIME_SECURE_DONE",          ELEM_OUTPUT_INT, {0x40},  {0x4}}, \
    {"DEBUG_VERSION_ADDR",              ELEM_OUTPUT_HEX, {0x50},  {0x10}}, \
    {"XLOADER_RESET_REG",               ELEM_OUTPUT_INT, {0x200}, {0x4}}, \
    {"XLOADER_KEY_POINT",               ELEM_OUTPUT_INT, {0x204}, {0x4}}, \
    {"XLOADER_TIME_POWERUP_DONE",       ELEM_OUTPUT_INT, {0x228}, {0x4}}, \
    {"XLOADER_TIME_PERSTHIGH_DONE",     ELEM_OUTPUT_INT, {0x22C}, {0x4}}, \
    {"XLOADER_TIME_PCIEPHY_DONE",       ELEM_OUTPUT_INT, {0x230}, {0x4}}, \
    {"XLOADER_TIME_PHY_FIRMWARE_DONE",  ELEM_OUTPUT_INT, {0x234}, {0x4}}, \
    {"XLOADER_TIME_PCIECTRL_DONE",      ELEM_OUTPUT_INT, {0x238}, {0x4}}, \
    {"XLOADER_TIME_PCIE_DETECT_DONE",   ELEM_OUTPUT_INT, {0x23C}, {0x4}}, \
    {"UEFI_LAST_KEYPOINT",              ELEM_OUTPUT_INT, {0x320}, {0x4}}, \
    {"SD_LOAD_FILE_STATUS",             ELEM_OUTPUT_INT, {0x350}, {0x4}}, \
}

#define DATA_MODEL_HBM_SRAM MODEL_VECTOR(HBM_SRAM) = { \
    {"hbm sram info",   ELEM_OUTPUT_STR_NL,   {0x00}, {0x14000}}, \
}

#define DATA_MODEL_SRAM_SNAPSHOT MODEL_VECTOR(SRAM_SNAPSHOT) = { \
    {"sram snapshot info",   ELEM_OUTPUT_BIN,   {0x00}, {0x12000UL}}, \
}

#define DATA_MODEL_BIOS_HISS MODEL_VECTOR(BIOS_HISS) = { \
    {"bios hiss info",   ELEM_OUTPUT_BIN,   {0x00}, {0x1000UL}}, \
}

#define DATA_MODEL_HBOOT MODEL_VECTOR(HBOOT) = { \
    {"hboot info",   ELEM_OUTPUT_BIN,   {0x00}, {0x200000UL}}, \
}

/* NETWORK module */
#define DATA_MODEL_NETWORK MODEL_VECTOR(NETWORK) = { \
    {"network info", ELEM_OUTPUT_STR, {0x0}, {0x20000}}, \
}

/* IMU module, boot log */
#define DATA_MODEL_IMU_BOOT_LOG MODEL_VECTOR(IMU_BOOT_LOG) = { \
    {"imu log header",  ELEM_FEATURE_LOOPBUF,     {1},       {5}}, \
    {"buf_read",        ELEM_CTRL_LPBF_READ,    {0x0},     {0x4}}, \
    {"buf_len",         ELEM_CTRL_LPBF_SIZE,    {0x4},     {0x4}}, \
    {"log level",       ELEM_OUTPUT_INT,        {0x8},     {0x4}}, \
    {"rollback",        ELEM_CTRL_LPBF_ROLLBK, {0x10},     {0x4}}, \
    {"buf_write",       ELEM_CTRL_LPBF_WRITE,  {0x40},     {0x4}}, \
    {"imu log",         ELEM_FEATURE_CHARLOG,     {1},       {1}}, \
    {"imu log info",    ELEM_OUTPUT_STR_NL,    {0x80}, {0xFFF80}}, \
}

/* UEFI(BIOS) module, boot log */
#define DATA_MODEL_UEFI_BOOT_LOG MODEL_VECTOR(UEFI_BOOT_LOG) = { \
    {"uefi log header", ELEM_FEATURE_LOOPBUF,     {1},        {5}}, \
    {"buf_read",        ELEM_CTRL_LPBF_READ,    {0x0},      {0x4}}, \
    {"buf_len",         ELEM_CTRL_LPBF_SIZE,    {0x4},      {0x4}}, \
    {"log level",       ELEM_OUTPUT_INT,        {0x8},      {0x4}}, \
    {"rollback",        ELEM_CTRL_LPBF_ROLLBK, {0x10},      {0x4}}, \
    {"buf_write",       ELEM_CTRL_LPBF_WRITE,  {0x40},      {0x4}}, \
    {"uefi log",        ELEM_FEATURE_CHARLOG,     {1},        {1}}, \
    {"uefi log info",   ELEM_OUTPUT_STR_NL,    {0x80}, {0x2FFF80}}, \
}

/* IMU module, run log */
#define DATA_MODEL_IMU_RUN_LOG MODEL_VECTOR(IMU_RUN_LOG) = { \
    {"imu log info",    ELEM_OUTPUT_STR_NL,    {0x100},{ 0x1FF00}}, \
}

/* LPM module, log */
#define DATA_MODEL_LPM_LOG MODEL_VECTOR(LPM_LOG) = { \
    {"lpm log header",  ELEM_FEATURE_LOOPBUF,    {1},       {4}}, \
    {"buf_read",        ELEM_CTRL_LPBF_READ,   {0x0},     {0x4}}, \
    {"buf_len",         ELEM_CTRL_LPBF_SIZE,   {0x4},     {0x4}}, \
    {"buf_write",       ELEM_CTRL_LPBF_WRITE, {0x80},     {0x4}}, \
    {"log level",       ELEM_OUTPUT_INT,      {0x84},     {0x4}}, \
    {"lpm log",         ELEM_FEATURE_CHARLOG,    {1},       {1}}, \
    {"lpm log info",    ELEM_OUTPUT_STR_NL,   {0x100},{0x1FF00}}, \
}

/* OS kbox module */
#define DATA_MODEL_DP MODEL_VECTOR(DP) = { \
    {"os kbox info", ELEM_OUTPUT_STR_NL, {0x3000}, {0x27D000}}, \
}

#endif // BBOX_DATA_MILAN_H
