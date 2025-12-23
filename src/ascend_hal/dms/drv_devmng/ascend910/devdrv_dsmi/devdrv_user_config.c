/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>

#include "devmng_common.h"
#include "devdrv_user_common.h"
#include "user_cfg_public.h"
#include "dms/dms_devdrv_info_comm.h"
#include "config.h"
#include "dms/dms_devdrv_user_config.h"

#ifdef STATIC_SKIP
#define STATIC
#else
#define STATIC static
#endif

#define DEV_USER_CFG_NAME "/dev/user_config"

#define P2P_MEM_LEVEL_MAX 3
#define P2P_MEM_1024M_PER_G 1024
#define P2P_MEM_DEFAULT_SIZE 512

STATIC int dev_user_cfg_open(const char *path, int flags)
{
    int fd = -1;
    char *real_path = NULL;
    int errno_tmp;

    if (path == NULL) {
        DEVDRV_DRV_ERR("path is null\n");
        return -EINVAL;
    }

    if (strnlen(path, PATH_MAX) >= PATH_MAX) {
        DEVDRV_DRV_ERR("path length is invalid\n");
        return -EINVAL;
    }

    real_path = malloc(PATH_MAX);
    if (real_path == NULL) {
        DEVDRV_DRV_ERR("malloc fail\n");
        return -ENOMEM;
    }

    if (memset_s(real_path, PATH_MAX, 0, PATH_MAX) != 0) {
        DEVDRV_DRV_ERR("memset_s fail\n");
        free(real_path);
        real_path = NULL;
        return -ENOMEM;
    }

    if (realpath((const char *)path, real_path) == NULL) {
        DEVDRV_DRV_ERR("realpath fail, %s\n", path);
        free(real_path);
        real_path = NULL;
        return -ENOENT;
    }

    fd = open((const char *)real_path, flags);
    if (fd < 0) {
        errno_tmp = errno;
        DEVDRV_DRV_ERR("open file fail, (file=%s, errno=%d).\n", real_path, errno_tmp);
    }
    free(real_path);
    real_path = NULL;
    return fd;
}

/*
 * upgrade ioctl operation
 */
STATIC int dev_user_cfg_ioctl_op(int cmd, char *msg)
{
    int dev_fd = -1;
    int ret;

    dev_fd = dev_user_cfg_open(DEV_USER_CFG_NAME, O_RDWR);
    if (dev_fd < 0) {
        DEVDRV_DRV_ERR("can't open fail %s\n", DEV_USER_CFG_NAME);
        return DRV_ERROR_INVALID_DEVICE;
    }

    ret = ioctl(dev_fd, cmd, msg);
    if (ret < 0) {
        DEVDRV_DRV_WARN("ioctl warn %d\n", ret);
        (void)close(dev_fd);
        dev_fd = -1;
        return DRV_ERROR_IOCRL_FAIL;
    }

    (void)close(dev_fd);
    dev_fd = -1;
    return DRV_ERROR_NONE;
}

#ifdef CFG_FEATURE_USER_CFG_P2P
STATIC int devdrv_p2p_mem_check_memory_enough(u8 *buf, u32 buf_size)
{
    int ret;
    u32 p2p_mem_size;
    u32 mem_left_size;
    struct sysinfo info = {0};

    ret = memcpy_s(&p2p_mem_size, sizeof(p2p_mem_size), buf, buf_size);
    if (ret != 0) {
        DEVDRV_DRV_ERR("memcpy_s failed, ret: %d.\n", ret);
        return ret;
    }

    ret = sysinfo(&info);
    if (ret != 0) {
        DEVDRV_DRV_ERR("sysinfo failed, ret: %d.\n", ret);
        return ret;
    }
    mem_left_size = (u32)(info.freeram / SZ_1M);

    if (p2p_mem_size > ((int)mem_left_size - (SZ_2G / SZ_1M))) {
        DEVDRV_DRV_ERR("Memory left not enough. (p2p_mem_size=%uM; mem_left_size=%uM)\n", p2p_mem_size, mem_left_size);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    *(u32 *)buf = *(u32 *)buf / SIZE_IN_G;

    return DRV_ERROR_NONE;
}

/*lint -e527*/
STATIC int devdrv_p2p_mem_check_env(u32 dev_id)
{
    if (dev_id != 0) { /* in 2P scenes: p2p mem just support P0 */
        return DRV_ERROR_NOT_SUPPORT;
    }

    return DRV_ERROR_NONE;
}
/*lint +e527*/
#endif

int halGetUserConfig(unsigned int devId, const char *name, unsigned char *buf, unsigned int *bufSize)
{
#ifdef DRV_HOST
    (void)(devId);
    (void)(name);
    (void)(buf);
    (void)(bufSize);
    return DRV_ERROR_NOT_SUPPORT;
#else
    struct user_cfg_ioctl_para drv_flash_cmd_para = {0};
    u32 name_len;
    int err_buf;
    int ret;

    if ((name == NULL) || (buf == NULL) || (bufSize == NULL)) {
        DEVDRV_DRV_ERR("dev_id[%u], input para is NULL.\n", devId);
        return -EINVAL;
    }

    if (devId >= DEVDRV_UC_CHIP_MAX) {
        DEVDRV_DRV_ERR("dev_id[%u] out of range.\n", devId);
        return -EINVAL;
    }

    name_len = (u32)strlen(name) + 1;
    if (name_len > (DEVDRV_USER_CONFIG_NAME_MAX)) {
        DEVDRV_DRV_ERR("dev_id[%u], name len is too long, name_len: %u.\n", devId, name_len);
        return -EINVAL;
    }

#ifndef CFG_FEATURE_SUPPORT_AUTH_ENABLE_SIGN
    if (strcmp(name, AUTH_CONFIG_ENABLE_NAME) == 0) {
        return DRV_ERROR_NOT_SUPPORT;
    }
#endif

    ret = memcpy_s(drv_flash_cmd_para.name, DEVDRV_USER_CONFIG_NAME_MAX, name, name_len);
    if (ret != 0) {
        DEVDRV_DRV_ERR("dev_id[%u], memcpy_s failed, ret: %d.\n", devId, ret);
        return -EINVAL;
    }

    drv_flash_cmd_para.dev_id = devId;
    drv_flash_cmd_para.cmd = DEVDRV_FLASH_CONFIG_READ_CMD;
    drv_flash_cmd_para.buf = (void *)buf;
    drv_flash_cmd_para.buf_size = *bufSize;

    ret = dev_user_cfg_ioctl_op((int)USER_CFG_FLASH_OP, (char *)&drv_flash_cmd_para);
    if (ret != 0) {
        err_buf = errno;
        if (err_buf == ENOENT) {
            DEVDRV_DRV_WARN("dev_id[%u] getting item is not set before, err[%d], ret=%d.\n", devId, err_buf, ret);
        } else {
            DEVDRV_DRV_ERR("dev_id[%u] Ioctl error[%d], ret=%d.\n", devId, err_buf, ret);
        }
        return (-err_buf);
    }
#ifdef CFG_FEATURE_USER_CFG_P2P
    if (strcmp(name, P2P_MEM_CONFIG_NAME) == 0) {
        if (*((u32 *)drv_flash_cmd_para.buf) != P2P_MEM_DEFAULT_SIZE) {
            *((u32 *)drv_flash_cmd_para.buf) = *((u32 *)drv_flash_cmd_para.buf) * P2P_MEM_1024M_PER_G;
        }
    }
#endif

    *bufSize = drv_flash_cmd_para.buf_size;
    return DRV_ERROR_NONE;
#endif
}

int halSetUserConfig(unsigned int devId, const char *name, unsigned char *buf, unsigned int bufSize)
{
#ifdef DRV_HOST
    (void)(devId);
    (void)(name);
    (void)(buf);
    (void)(bufSize);
    return DRV_ERROR_NOT_SUPPORT;
#else
    struct user_cfg_ioctl_para drv_flash_cmd_para = {0};
    u32 name_len;
    int err_buf;
    int ret;

    if (name == NULL) {
        DEVDRV_DRV_ERR("dev_id[%u], input name is NULL.\n", devId);
        return -EINVAL;
    }
    if (buf == NULL) {
        DEVDRV_DRV_ERR("dev_id[%u], input buf handle is NULL.\n", devId);
        return -EINVAL;
    }

    if (devId >= DEVDRV_UC_CHIP_MAX) {
        DEVDRV_DRV_ERR("dev_id[%u] out of range.\n", devId);
        return -EINVAL;
    }

    name_len = (u32)strlen(name) + 1;
    if (name_len > DEVDRV_USER_CONFIG_NAME_MAX) {
        DEVDRV_DRV_ERR("dev_id[%u], name len is too long, name_len: %u.\n", devId, name_len);
        return -EINVAL;
    }

#ifndef CFG_FEATURE_SUPPORT_AUTH_ENABLE_SIGN
    if (strcmp(name, AUTH_CONFIG_ENABLE_NAME) == 0) {
        return DRV_ERROR_NOT_SUPPORT;
    }
#endif

    ret = memcpy_s(drv_flash_cmd_para.name, DEVDRV_USER_CONFIG_NAME_MAX, name, name_len);
    if (ret != 0) {
        DEVDRV_DRV_ERR("dev_id[%u], memcpy_s failed, ret: %d.\n", devId, ret);
        return -EINVAL;
    }

    drv_flash_cmd_para.dev_id = devId;
    drv_flash_cmd_para.cmd = DEVDRV_FLASH_CONFIG_WRITE_CMD;
    drv_flash_cmd_para.buf = (void *)buf;
    drv_flash_cmd_para.buf_size = bufSize;

    ret = dev_user_cfg_ioctl_op((int)USER_CFG_FLASH_OP, (char *)&drv_flash_cmd_para);
    if (ret != 0) {
        err_buf = errno;
        DEVDRV_DRV_ERR("dev_id[%u] Ioctl error[%d], ret=%d.\n", devId, err_buf, ret);
        return (-err_buf);
    }

    return DRV_ERROR_NONE;
#endif
}

int halClearUserConfig(unsigned int devId, const char *name)
{
#ifdef DRV_HOST
    (void)(devId);
    (void)(name);
    return DRV_ERROR_NOT_SUPPORT;
#else
    struct user_cfg_ioctl_para drv_flash_cmd_para = {0};
    u32 name_len;
    int err_buf;
    int ret;

    if (name == NULL) {
        DEVDRV_DRV_ERR("dev_id[%u], input name is NULL.\n", devId);
        return -EINVAL;
    }

    if (devId >= DEVDRV_UC_CHIP_MAX) {
        DEVDRV_DRV_ERR("dev_id[%u] out of range.\n", devId);
        return -EINVAL;
    }

    name_len = (u32)strlen(name) + 1;
    if (name_len > DEVDRV_USER_CONFIG_NAME_MAX) {
        DEVDRV_DRV_ERR("dev_id[%u], name len is too long, name_len: %u.\n", devId, name_len);
        return -EINVAL;
    }

#ifndef CFG_FEATURE_SUPPORT_AUTH_ENABLE_SIGN
    if (strcmp(name, AUTH_CONFIG_ENABLE_NAME) == 0) {
        return DRV_ERROR_NOT_SUPPORT;
    }
#endif

    ret = memcpy_s(drv_flash_cmd_para.name, DEVDRV_USER_CONFIG_NAME_MAX, name, name_len);
    if (ret != 0) {
        DEVDRV_DRV_ERR("dev_id[%u], memcpy_s failed, ret: %d.\n", devId, ret);
        return -EINVAL;
    }

    drv_flash_cmd_para.dev_id = devId;
    drv_flash_cmd_para.cmd = DEVDRV_FLASH_CONFIG_CLEAR_CMD;

    ret = dev_user_cfg_ioctl_op((int)USER_CFG_FLASH_OP, (char *)&drv_flash_cmd_para);
    if (ret != 0) {
        err_buf = errno;
        DEVDRV_DRV_ERR("dev_id[%d] Ioctl error[%d], ret=%d.\n", devId, err_buf, ret);
        return (-err_buf);
    }

    return DRV_ERROR_NONE;
#endif
}

STATIC int devdrv_get_total_cpu_cores(unsigned int *total_cpu_cores)
{
    int ret;
    FILE *fp = NULL;
    char *buf = NULL;
    int errno_tmp;

    if (total_cpu_cores == NULL) {
        DEVDRV_DRV_ERR("total cpu cores is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    buf = (char *)calloc(MACR_CPU_STAT_BUF_LEN, sizeof(char));
    if (buf == NULL) {
        DEVDRV_DRV_ERR("malloc memory error.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }

    fp = fopen(MACR_CPU_INFO_PATH, "r");
    if (fp == NULL) {
        errno_tmp = errno;
        DEVDRV_DRV_ERR("fopen error, (errno=%d).\n", errno_tmp);
        ret = DRV_ERROR_INVALID_HANDLE;
        goto EXIT;
    }

    *total_cpu_cores = 0;
    while (fgets(buf, MACR_CPU_INFO_BUF_LEN, fp) != NULL) {
        if (memcmp(buf, MACR_CPU_INFO_WORD, strlen(MACR_CPU_INFO_WORD)) == 0) {
            *total_cpu_cores = *total_cpu_cores + 1;
        }
    }

    ret = DRV_ERROR_NONE;
    DEVDRV_DRV_INFO("total cpu cores:%u.\n", *total_cpu_cores);

EXIT:
    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }

    if (fp != NULL) {
        (void)fclose(fp);
        fp = NULL;
    }

    return ret;
}

int devdrv_get_boot_cfg(unsigned char *chip_info)
{
    int ret;
    int err_buf;
    int boot_cfg = 0;

    if (chip_info == NULL) {
        DEVDRV_DRV_ERR("chip_info is null.\n");
        return -EINVAL;
    }

    ret = dev_user_cfg_ioctl_op((int)USER_CFG_GET_BOOT_CFG, (char *)&boot_cfg);
    if (ret != 0) {
        err_buf = errno;
        DEVDRV_DRV_ERR("Ioctl error[%d], ret=%d.\n", err_buf, ret);
        return (-err_buf);
    }

    DEVDRV_DRV_INFO("drvtool_get_boot_cfg: %d.\n", boot_cfg);
    if (boot_cfg == 0) {
        *chip_info = CHIP_INFO_SOLO;
    } else {
        *chip_info = CHIP_INFO_DUAL;
    }

    return 0;
}

STATIC int devdrv_cpu_nums_check_para(unsigned char *buf, unsigned int buf_size)
{
    int ret;
    unsigned int dev_num;
    unsigned int cpu_nums_sum;
    unsigned int total_cpu_cores;
    uc_cpu_cfg_t *cpu_cfg = (uc_cpu_cfg_t *)buf;
    unsigned char chip_info = CHIP_INFO_SOLO;
#ifdef CFG_FEATURE_COM_CPU_CONFIG
    uc_cpu_cfg_t cpu_check = {0};
#endif

    if (buf_size != sizeof(uc_cpu_cfg_t)) {
        DEVDRV_DRV_ERR("buf_size[%u] is not valid.\n", buf_size);
        return DRV_ERROR_INVALID_VALUE;
    }

    DEVDRV_DRV_INFO("ctrl_cpu: %u, data_cpu: %u, ai_cpu: %u.\n",
        cpu_cfg->ctrl_cpu_num, cpu_cfg->data_cpu_num, cpu_cfg->ai_cpu_num);
#ifdef CFG_FEATURE_COM_CPU_CONFIG
    cpu_nums_sum = cpu_cfg->ctrl_cpu_num + cpu_cfg->data_cpu_num + cpu_cfg->ai_cpu_num + cpu_cfg->com_cpu_num;
    if (memcmp(cpu_cfg->reserved_2, cpu_check.reserved_2, sizeof(cpu_check.reserved_2)) != 0) {
        DEVDRV_DRV_ERR("The reserved field is not all 0.\n");
        return DRV_ERROR_INVALID_VALUE;
    }
#else
    cpu_nums_sum = cpu_cfg->ctrl_cpu_num + cpu_cfg->data_cpu_num + cpu_cfg->ai_cpu_num;
#endif
#ifdef CFG_FEATURE_AICPU_NUM_CFG_UNLIMIT
    if (cpu_cfg->ctrl_cpu_num < CTRL_CPU_NUM_MIN) {
        DEVDRV_DRV_ERR("cpu number is not valid.\n");
        return DRV_ERROR_INVALID_VALUE;
    }
#else
    if ((cpu_cfg->ctrl_cpu_num < CTRL_CPU_NUM_MIN) || (cpu_cfg->ai_cpu_num < AI_CPU_NUM_MIN) ||
        (cpu_cfg->com_cpu_num > COM_CPU_NUM_MAX)) {
        DEVDRV_DRV_ERR("cpu number is not valid.\n");
        return DRV_ERROR_INVALID_VALUE;
    }
#endif
    ret = devdrv_get_total_cpu_cores(&total_cpu_cores);
    if (ret != 0) {
        DEVDRV_DRV_ERR("devdrv_get_total_cpu_cores failed, ret=%d.\n", ret);
        return ret;
    }

    ret = devdrv_get_boot_cfg(&chip_info);
    if (ret != 0) {
        DEVDRV_DRV_ERR("devdrv_get_boot_cfg failed, ret=%d.\n", ret);
        return ret;
    }

    if (chip_info == CHIP_INFO_SOLO) {
        dev_num = 1; /* 1P */
    } else {
        dev_num = 2; /* 2P */
    }

    DEVDRV_DRV_INFO("dev_num: %u.\n", dev_num);
    if (cpu_nums_sum != (total_cpu_cores / dev_num)) {
        DEVDRV_DRV_ERR("total cpu number[%u] is invalid.\n", cpu_nums_sum);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

/*
 * for host dsmi interface to check authority
 * tls cert data not permit for host user cfg interface to operate
 */
STATIC int devdrv_check_user_config_authority(const char *name)
{
#ifdef CFG_SOC_PLATFORM_CLOUD
    int i;
    const char *verify_item_name[NETWORK_ITEM_NAME_NUM] = {
        CERT_ITEM_NAME_S0, CERT_ITEM_NAME_S1, CERT_ITEM_NAME_S2, CERT_ITEM_NAME_S3,
        DIGITAL_ITEM_NAME_S0, DIGITAL_ITEM_NAME_S1, DIGITAL_ITEM_NAME_S2
    };
    int verify_num = NETWORK_ITEM_NAME_NUM;

    /* cert item for dsmi, not permit to read/write/clear */
    for (i = 0; i < verify_num; i++) {
        if (strcmp(name, verify_item_name[i]) == 0) {
            DEVDRV_DRV_ERR("don't have permission!.\n");
            return -EACCES;
        }
    }
#else
    (void)name;
#endif

    return DRV_ERROR_NONE;
}

STATIC bool is_cpu_num_cfg_in_user_cfg(void)
{
    int cfg_index;

    for (cfg_index = 0; cfg_index < UC_ITEM_MAX_NUM; cfg_index++) {
        if (strcmp(CPU_NUM_CONFIG_NAME, user_cfg_version_1[cfg_index].name) == 0) {
            return true;
        }
    }
    return false;
}

STATIC int devdrv_check_user_config_para(const char *name, u8 *buf, u32 buf_size)
{
    int ret;

    if (strcmp(name, CPU_NUM_CONFIG_NAME) == 0 && is_cpu_num_cfg_in_user_cfg() == true) {
        ret = devdrv_cpu_nums_check_para(buf, buf_size);
        if (ret != 0) {
            DEVDRV_DRV_ERR("devdrv_cpu_nums_check_para failed, ret[%d].\n", ret);
            return ret;
        }
    }

    return DRV_ERROR_NONE;
}

STATIC int devdrv_check_devid_in_smp(u32 *dev_id, const char *name)
{
    if (*dev_id >= DEVDRV_UC_CHIP_MAX) {
        return -EINVAL;
    }
#if ((defined CFG_SOC_PLATFORM_MINIV2) && (!defined CFG_FEATURE_NOT_SUPPORT_SSH_CONFIG))
    /* ssh status set in smp only dev0 effective */
    if ((strcmp(name, SSH_CONFIG_NAME) == 0) && (*dev_id > 0)) {
        *dev_id = 0;
    }
#endif
    (void)(name);
    return DRV_ERROR_NONE;
}

int devdrv_get_user_config_ex(unsigned int dev_id, const char *name, unsigned char *buf, unsigned int *buf_size)
{
    int ret;

    ret = devdrv_check_user_config_authority(name);
    if (ret != 0) {
        DEVDRV_DRV_ERR("dev_id[%d] check authority for get failed[%d]!.\n", dev_id, ret);
        return -EACCES;
    }

    ret = devdrv_check_devid_in_smp(&dev_id, name);
    if (ret != 0) {
        DEVDRV_DRV_ERR("dev_id[%u] is invalid.\n", dev_id);
        return ret;
    }

    return devdrv_get_user_config(dev_id, name, buf, buf_size);
}

#ifdef CFG_FEATURE_USER_CFG_P2P
STATIC int devdrv_p2p_mem_cfg_para_check(unsigned char *buf, unsigned int buf_size)
{
    int i;
    (void)buf_size;
    /* only support for 0M, 1024M, 2048M */
    unsigned int p2p_mem_level[P2P_MEM_LEVEL_MAX] = {0, 1024, 2048};

    for (i = 0; i < P2P_MEM_LEVEL_MAX; i++) {
        if (*(unsigned int *)buf == p2p_mem_level[i]) {
            break;
        }
    }

    if (i >= P2P_MEM_LEVEL_MAX) {
        DEVDRV_DRV_ERR("Unsupport p2p memory size. (p2p_mem_size=%uM)\n", *(unsigned int *)buf);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}
#endif

#define CRC16_LOW_8_BIT 8
STATIC uint16_t CRC16(const uint8_t *pdata, uint16_t datalen)
{
    uint8_t CRC16Lo, CRC16Hi, CL, CH, save_hi, save_lo;
    uint16_t i, Flag;
    CRC16Lo = 0xFF;
    CRC16Hi = 0xFF;
    CL = 0x01;
    CH = 0xA0;

    for (i = 0; i < datalen; i++) {
        CRC16Lo ^= *(pdata + i);

        for (Flag = 0; Flag < CRC16_LOW_8_BIT; Flag++) {
            save_hi = CRC16Hi;
            save_lo = CRC16Lo;
            CRC16Hi >>= 1;
            CRC16Lo >>= 1;

            if ((save_hi & 0x01) == 0x01) {
                CRC16Lo |= 0x80;
            }

            if ((save_lo & 0x01) == 0x01) {
                CRC16Hi ^= CH;
                CRC16Lo ^= CL;
            }
        }
    }

    return (CRC16Hi << CRC16_LOW_8_BIT) | CRC16Lo;
}

#define MAC_INFO_TYPE_INDEX 2
#define MAC_DATA_LENGTH_INDEX 2
#define MAC_ADDR_OFFSET 3
#define MAC_ID_INDEX 3
#define MAC_TYPE_INDEX 4
#define BUF_MAC_ADDR_OFFSET 5
#define MAC_ADDR_LEN 0X06
#define MAC_INFO_CRC_BUF_LEN 9
#define BUF_MIN_LEN 16
#define CRC_CODE_MASK 0xFF
#define MAC_INFO_CONFIG_NAME        "mac_info"
#define MAC_INFO_1_CONFIG_NAME      "mac_info_1"
STATIC int devdrv_check_mac_info_crc(const char *name, unsigned char *buf, unsigned int buf_size)
{
    int ret;
    unsigned char crc_buf[MAC_INFO_CRC_BUF_LEN] = {0};
    unsigned short crc_value;
    unsigned char mac_id;

    if (strcmp(name, MAC_INFO_CONFIG_NAME) == 0) {
        mac_id = 0;
    } else if (strcmp(name, MAC_INFO_1_CONFIG_NAME) == 0) {
        mac_id = 1;
    } else {
        return DRV_ERROR_NONE;
    }

    if (buf_size < BUF_MIN_LEN) {
        DEVDRV_DRV_ERR("buf_size[%u] is invalid.\n", buf_size);
        return DRV_ERROR_PARA_ERROR;
    }

    crc_buf[0] = MAC_INFO_CRC_BUF_LEN;
    crc_buf[1] = mac_id;
    crc_buf[MAC_INFO_TYPE_INDEX] = 0;
    ret = memcpy_s(crc_buf + MAC_ADDR_OFFSET, MAC_INFO_CRC_BUF_LEN - MAC_ADDR_OFFSET,
        buf + BUF_MAC_ADDR_OFFSET, MAC_ADDR_LEN);
    if (ret != DRV_ERROR_NONE) {
        DEVDRV_DRV_ERR("Failed to invoke memcpy_s (ret=%d).\n", ret);
        return DRV_ERROR_MEMORY_OPT_FAIL;
    }
    crc_value = CRC16(crc_buf, sizeof(crc_buf));
    buf[0] = crc_value & CRC_CODE_MASK;
    buf[1] = (crc_value >> CRC16_LOW_8_BIT) & CRC_CODE_MASK;
    buf[MAC_DATA_LENGTH_INDEX] = MAC_INFO_CRC_BUF_LEN;
    buf[MAC_ID_INDEX] = mac_id;
    buf[MAC_TYPE_INDEX] = 0;

    return DRV_ERROR_NONE;
}

int devdrv_set_user_config_ex(unsigned int dev_id, const char *name, unsigned char *buf, unsigned int buf_size)
{
    int ret;

    ret = devdrv_check_user_config_authority(name);
    if (ret != 0) {
        DEVDRV_DRV_ERR("dev_id[%d] check authority for set failed[%d]!.\n", dev_id, ret);
        return -EACCES;
    }

    ret = devdrv_check_user_config_para(name, buf, buf_size);
    if (ret != 0) {
        DEVDRV_DRV_ERR("dev_id[%d] devdrv_check_user_config_para failed[%d].\n", dev_id, ret);
        return ret;
    }

    ret = devdrv_check_mac_info_crc(name, buf, buf_size);
    if (ret != 0) {
        DEVDRV_DRV_ERR("dev_id[%d] devdrv_check_mac_info_crc failed[%d].\n", dev_id, ret);
        return ret;
    }

#ifdef CFG_FEATURE_USER_CFG_P2P
    if (strcmp(name, P2P_MEM_CONFIG_NAME) == 0) {
        ret = devdrv_p2p_mem_cfg_para_check(buf, buf_size);
        if (ret != 0) {
            DEVDRV_DRV_ERR("P2P memory configure parameters check failed. (device_id=%u; ret=%d)\n", dev_id, ret);
            return ret;
        }

        ret = devdrv_p2p_mem_check_memory_enough(buf, buf_size);
        if (ret != 0) {
            DEVDRV_DRV_ERR("Memory is not enough. (dev_id=%u; ret=%d)\n", dev_id, ret);
            return ret;
        }
    }
#endif

    ret = devdrv_check_devid_in_smp(&dev_id, name);
    if (ret != 0) {
        DEVDRV_DRV_ERR("dev_id[%u] is invalid.\n", dev_id);
        return ret;
    }

    return devdrv_set_user_config(dev_id, name, buf, buf_size);
}

int devdrv_clear_user_config_ex(unsigned int dev_id, const char *name)
{
    int ret;

    ret = devdrv_check_user_config_authority(name);
    if (ret != 0) {
        DEVDRV_DRV_ERR("dev_id[%d] check authority for clear failed[%d]!.\n", dev_id, ret);
        return -EACCES;
    }

    ret = devdrv_check_devid_in_smp(&dev_id, name);
    if (ret != 0) {
        DEVDRV_DRV_ERR("dev_id[%u] is invalid.\n", dev_id);
        return ret;
    }

    return devdrv_clear_user_config(dev_id, name);
}

int devdrv_user_config_common_check(unsigned int dev_id, const char *name)
{
#ifdef CFG_FEATURE_USER_CFG_P2P
    int ret;

    if (strcmp(name, P2P_MEM_CONFIG_NAME) == 0) {
        ret = devdrv_p2p_mem_check_env(dev_id);
        if (ret != 0) {
            DEVDRV_DRV_EX_NOTSUPPORT_ERR(ret,
                "In 2P scenes, p2p memory only support for P0. (devid=%u; ret=%d)\n", dev_id, ret);
            return ret;
        }
    }
#endif
    (void)(dev_id);
    (void)(name);
    return 0;
}

int devdrv_get_user_config(u32 devid, const char *name, u8 *buf, u32 *buf_size)
{
    return halGetUserConfig(devid, name, buf, buf_size);
}

int devdrv_set_user_config(u32 devid, const char *name, u8 *buf, u32 buf_size)
{
    return halSetUserConfig(devid, name, buf, buf_size);
}

int devdrv_clear_user_config(u32 devid, const char *name)
{
    return halClearUserConfig(devid, name);
}
