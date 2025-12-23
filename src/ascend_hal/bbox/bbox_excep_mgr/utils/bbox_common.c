/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "bbox_common.h"
#include <stddef.h>
#include "bbox_int.h"
#include "bbox_rdr_pub.h"

static struct bbox_core_map {
    const char *cname;  // core name in history.log
    u8 core;            // core id
} g_core_map[] = {
    {"DRIVER",          BBOX_DRIVER},
    {"AP",              BBOX_OS},
    {"TS",              BBOX_TS},
    {"AICPU",           BBOX_AICPU},
    {"DVPP",            BBOX_DVPP},
    {"LPM",             BBOX_LPM},
    {"REGDUMP",         BBOX_REGDUMP},
    {"MICROWATT",       BBOX_MICROWATT},
    {"BIOS",            BBOX_BIOS},
    {"TEEOS",           BBOX_TEEOS},
    {"NETWORK",         BBOX_NETWORK},
    {"ATF",             BBOX_ATF},
    {"LPFW",            BBOX_LPFW},
    {"HSM",             BBOX_HSM},
    {"ISP",             BBOX_ISP},
    {"SAFETYISLAND",    BBOX_SIL},
    {"DSS",             BBOX_DSS},
    {"COMISOLATOR",     BBOX_COMISOLATOR},
    {"AOS-SD",          BBOX_AOS_SD},
    {"AOS-DP",          BBOX_AOS_DP},
    {"AOS-LINUX",       BBOX_AOS_LINUX},
    {"AOS-CORE",        BBOX_AOS_CORE},
    {"COMMON",          BBOX_COMMON},
    {"IMU",             BBOX_IMU},
    {"QOS",             BBOX_QOS},
    {"UB",              BBOX_UB},
    {NULL,              BBOX_CORE_MAX},
};

/*
 * @brief       : get module name by core_id
 * @param [in]  : u8 core_id                 module id
 * @return      : module name
 */
const char *bbox_get_core_name(u8 core_id)
{
    s32 i;
    for (i = 0; g_core_map[i].cname != NULL; i++) {
        if (core_id == g_core_map[i].core) {
            return g_core_map[i].cname;
        }
    }
    return NULL;
}

// Add, please keep the same as definition in reboot_reason.c in fastboot !!!!
static struct bbox_reboot_reason_map {
    const char *reason;
    u8 code;
} g_except_type_map[] = {
    {"DEVICE_COLDBOOT",         DEVICE_COLDBOOT},
    {"BIOS_EXCEPTION",          BIOS_EXCEPTION},
    {"DEVICE_HOTBOOT",          DEVICE_HOTBOOT},
    {"ABNORMAL_EXCEPTION",      ABNORMAL_EXCEPTION},
    {"TSENSOR_EXCEPTION",       TSENSOR_EXCEPTION},
    {"PMU_EXCEPTION",           PMU_EXCEPTION},
    {"DDR_CRITICAL_EXCEPTION",  DDR_CRITICAL_EXCEPTION},
    {"OS_PANIC",                OS_PANIC},
    {"OS_OOM",                  OS_OOM},
    {"OS_COMM",                 OS_COMM},
    {"STARTUP_EXCEPTION",       STARTUP_EXCEPTION},
    {"HEARTBEAT_EXCEPTION",     HEARTBEAT_EXCEPTION},
    {"RUN_EXCEPTION",           RUN_EXCEPTION},
    {"LPM_EXCEPTION",           LPM_EXCEPTION},
    {"MICROWATT_EXCEPTION",     MICROWATT_EXCEPTION},
    {"TS_EXCEPTION",            TS_EXCEPTION},
    {"DVPP_EXCEPTION",          DVPP_EXCEPTION},
    {"DRIVER_EXCEPTION",        DRIVER_EXCEPTION},
    {"TEE_EXCEPTION",           TEE_EXCEPTION},
    {"LPFW_EXCEPTION",          LPFW_EXCEPTION},
    {"NETWORK_EXCEPTION",       NETWORK_EXCEPTION},
    {"HSM_EXCEPTION",           HSM_EXCEPTION},
    {"ATF_EXCEPTION",           ATF_EXCEPTION},
    {"ISP_EXCEPTION",           ISP_EXCEPTION},
    {"SAFETYISLAND_EXCEPTION",  SAFETYISLAND_EXCEPTION},
    {"TOOLCHAIN_EXCEPTION",     TOOLCHAIN_EXCEPTION},
    {"DSS_EXCEPTION",           DSS_EXCEPTION},
    {"COMISOLATOR_EXCEPTION",   COMISOLATOR_EXCEPTION},
    {"SD_EXCEPTION",            SD_EXCEPTION},
    {"DP_EXCEPTION",            DP_EXCEPTION},
    {"SUSPEND_FAIL",            SUSPEND_FAIL},
    {"RESUME_FAIL",             RESUME_FAIL},
    {"CPUCORE_EXCEPTION",       CPUCORE_EXCEPTION},
    {"HDR_EXCEPTION",           HDR_EXCEPTION},
    {"DEVICE_LTO_EXCEPTION",    DEVICE_LTO_EXCEPTION},
    {"DEVICE_HBL_EXCEPTION",    DEVICE_HBL_EXCEPTION},
    {"BOOT_DOT_INFO",           BOOT_DOT_INFO},
    {NULL,                      BBOX_EXCEPTION_REASON_INVALID},
};

/*
 * @brief       : get reboot reason string
 * @param [in]  : u8 reason         exception type
 * @return      : transformed data
 */
const char *bbox_get_reboot_reason(u8 reason)
{
    s32 i;
    for (i = 0; g_except_type_map[i].reason != NULL; i++) {
        if (reason == g_except_type_map[i].code) {
            return g_except_type_map[i].reason;
        }
    }
    return NULL;
}

STATIC u32 g_dpclk_real = CLOCK_VIRTUAL;
/*
 * @brief       : dpclk config init
 * @return      : success or failure
 */
bbox_status bbox_dpclk_init(void)
{
    char buffer[CMDLINE_BUFFER_SIZE] = {0};
    int fd = FD_INVALID_VAL;
    char *ptr = NULL;
    int readlen;
    int value;

    fd = open(CMDLINE_FILE, O_RDONLY);
    if (fd < 0) {
        BBOX_ERR("Failed to open the command file. (file=\"%s\")", CMDLINE_FILE);
        return BBOX_FAILURE;
    }

    readlen = (int)read(fd, buffer, CMDLINE_BUFFER_SIZE - 1);
    if (readlen <= 0) {
        BBOX_ERR("Failed to read the command file. (file=\"%s\", readLen=%u)", CMDLINE_FILE, (u32)readlen);
        close(fd);
        fd = FD_INVALID_VAL;
        return BBOX_FAILURE;
    }

    buffer[readlen] = '\0';
    ptr = strstr(buffer, CMDLINE_DPCLK);
    if (ptr == NULL) {
        BBOX_INF("Cmdline no has dpclk, use virtual time.");
        close(fd);
        fd = FD_INVALID_VAL;
        return BBOX_SUCCESS;
    }

    ptr += CMDLINE_DPCLK_LEN;
    value = atoi(ptr);
    if (value == CLOCK_VIRTUAL) {
        g_dpclk_real = CLOCK_RELTIME;
    }

    BBOX_INF("Cmdline dpclk config. (value=%u, g_dpclk_real=%u)", (u32)value, g_dpclk_real);
    close(fd);
    fd = FD_INVALID_VAL;
    return BBOX_SUCCESS;
}

/*
 * @brief       : get dpclk
 * @return      : clock_id config
 */
u32 bbox_get_dpclk(void)
{
    return g_dpclk_real;
}