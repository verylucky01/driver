/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef _WIN32
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include "dcmi_interface_api.h"
#include "dcmi_log.h"
#include "dcmi_smbus_operate.h"

static char g_smbus_dev_name[I2C_MAX][30] = {
    "/dev/i2c-0",
    "/dev/i2c-1",
    "/dev/i2c-2",
    "/dev/i2c-3",
    "/dev/i2c-4",
    "/dev/i2c-5",
    "/dev/i2c-6",
    "/dev/i2c-7",
    "/dev/i2c-8",
    "/dev/i2c-9",
    "/dev/i2c-10",
    "/dev/i2c-11",
};

static int dcmi_smbus_init(unsigned int id)
{
    int fd = -1;
    if (id >= I2C_MAX) {
        gplog(LOG_ERR, "dcmi_smbus_init invalid id %u", id);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    fd = open((char *)(g_smbus_dev_name[id]), O_RDWR);
    if (fd < 0) {
        gplog(LOG_ERR, "open %s dev error!", g_smbus_dev_name[id]);
    }
    return fd;
}

static int dcmi_smbus_access(
    int fd, unsigned char read_write, unsigned char reg, unsigned long size, union i2c_smbus_data *data)
{
    struct dcmi_smbus_ioctl_data32 args;
    int ret;
    args.read_write = read_write;
    args.command = reg;
    args.size = size;
    args.data = data;

    ret = ioctl(fd, I2C_SMBUS, &args);
    if (ret < DCMI_OK) {
        return DCMI_ERR_CODE_IOCTL_FAIL;
    } else {
        return DCMI_OK;
    }
}


int dcmi_smbus_write_block(
    unsigned int id, unsigned int addr, unsigned char reg, unsigned long len, const unsigned char *values)
{
    unsigned int i;
    union i2c_smbus_data data = {0};
    int fd_i2c;
    int ret;
    unsigned long length;

    fd_i2c = dcmi_smbus_init(id);
    if (fd_i2c < 0) {
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = ioctl(fd_i2c, I2C_SLAVE, addr);
    if (ret < DCMI_OK) {
        gplog(LOG_INFO, "set slave address failed:0x%x, err is %d", addr, ret);
        close(fd_i2c);
        return DCMI_ERR_CODE_IOCTL_FAIL;
    }

    length = (len > I2C_SMBUS_BLOCK_MAX) ? I2C_SMBUS_BLOCK_MAX : len;

    for (i = 1; i <= length; i++) {
        data.block[i] = values[i - 1];
    }

    data.block[0] = (unsigned char)length;
    ret = dcmi_smbus_access(fd_i2c, I2C_SMBUS_WRITE, reg, I2C_SMBUS_BLOCK_DATA, &data);
    if (ret != DCMI_OK) {
        gplog(LOG_INFO, "i2c smbus access failed:0x%x", addr);
        close(fd_i2c);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    close(fd_i2c);
    return DCMI_OK;
}

int dcmi_smbus_read_block(
    unsigned int id, unsigned int addr, unsigned char reg, unsigned long len, unsigned char *values)
{
    int i;
    union i2c_smbus_data data;
    int fd_i2c;
    int ret;
    unsigned long length;

    fd_i2c = dcmi_smbus_init(id);
    if (fd_i2c < 0) {
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = ioctl(fd_i2c, I2C_SLAVE, addr);
    if (ret < DCMI_OK) {
        gplog(LOG_INFO, "set slave address failed:0x%x, err is %d", addr, ret);
        close(fd_i2c);
        return DCMI_ERR_CODE_IOCTL_FAIL;
    }

    length = (len > I2C_SMBUS_BLOCK_MAX + 1) ? I2C_SMBUS_BLOCK_MAX : len;

    data.block[0] = (unsigned char)length;
    ret = dcmi_smbus_access(fd_i2c, I2C_SMBUS_READ, reg, I2C_SMBUS_I2C_BLOCK_DATA, &data);
    if (ret != DCMI_OK) {
        gplog(LOG_INFO, "i2c smbus access failed:0x%x", addr);
        close(fd_i2c);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    for (i = 1; i < data.block[0]; i++) {
        values[i - 1] = data.block[i + 1];
    }

    close(fd_i2c);
    return DCMI_OK;
}
#endif
