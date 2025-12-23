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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <limits.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "securec.h"
#include "dcmi_interface_api.h"
#include "dcmi_log.h"
#include "dcmi_common.h"
#include "dcmi_product_judge.h"
#include "dcmi_i2c_operate.h"

int g_i2c_dev_fd = -1;

STATIC int dcmi_i2c_init(const char *i2c_dev_name)
{
    int ret;
    int time_out;
    char path[PATH_MAX + 1] = {0x00};

#define DCMI_DEVELOP_I2C_TIMEOUT 100
#define DCMI_NORMAL_I2C_TIMEOUT 1

    if (i2c_dev_name == NULL) {
        gplog(LOG_ERR, "i2c_dev_name is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (realpath(i2c_dev_name, path) == NULL) {
        gplog(LOG_ERR, "realpath error. errno is %d", errno);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    g_i2c_dev_fd = open(path, O_RDWR);
    if (g_i2c_dev_fd < 0) {
        gplog(LOG_ERR, "Failed to open %s.\n", path);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = ioctl(g_i2c_dev_fd, I2C_RETRIES, 1);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to set i2c retry.\n");
        return DCMI_ERR_CODE_IOCTL_FAIL;
    }

    /* 开发者板 nputest读写夹具ID读写失败 */
    /* 内核4.19的i2c驱动更改了设置定值的方式 */
    if (dcmi_board_type_is_soc_develop()) {
        time_out = DCMI_DEVELOP_I2C_TIMEOUT;
    } else {
        time_out = DCMI_NORMAL_I2C_TIMEOUT;
    }

    ret = ioctl(g_i2c_dev_fd, I2C_TIMEOUT, time_out);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to set i2c timeout.\n");
        return DCMI_ERR_CODE_IOCTL_FAIL;
    }

    return ret;
}

static void dcmi_close_i2c_fd(void)
{
    if (g_i2c_dev_fd >= 0) {
        close(g_i2c_dev_fd);
        g_i2c_dev_fd = -1;
    }
}

STATIC int dcmi_i2c_read(unsigned char slave_addr, char *in_buffer, int in_len, char *out_buffer, int out_len)
{
    int ret = 0;
    int retry_num = I2C_MAX_RETRY_NUM;
    struct dcmi_i2c_msg msgs[2] = { { 0 } };
    struct dcmi_i2c_rdwr_ioctl_data rdwr_arg = {0};

    if (in_buffer == NULL || out_buffer == NULL) {
        gplog(LOG_ERR, "input para in_buffer or out_buffer is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (in_len < 0 || out_len < 0) {
        gplog(LOG_ERR, "input para in_len or out_len is invalid, in_len=%d, out_len=%d.", in_len, out_len);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    while (retry_num) {
        retry_num--;

        /* write in_buffer */
        msgs[0].addr = slave_addr;
        msgs[0].flags = 0;
        msgs[0].buf = (unsigned char *)in_buffer;
        msgs[0].len = (unsigned short)in_len;

        /* read to out_buffer */
        msgs[1].addr = slave_addr;
        msgs[1].flags = I2C_M_RD;
        msgs[1].buf = (unsigned char *)out_buffer;
        msgs[1].len = (unsigned short)out_len;

        rdwr_arg.msgs = msgs;
        rdwr_arg.nmsgs = (int)(sizeof(msgs) / sizeof(msgs[0]));

        ret = ioctl(g_i2c_dev_fd, I2C_RDWR, &rdwr_arg);

        (void)usleep(DCMI_TASK_DELAY_5_MS);

        /* write read success */
        if (ret >= DCMI_OK) {
            ret = out_len;
            break;
        } else {
            if (retry_num > 0) {
                (void)usleep(DCMI_TASK_DELAY_500_MS * (I2C_MAX_RETRY_NUM - retry_num));
            }
        }
    }

    if (ret < DCMI_OK) {
        gplog(LOG_ERR, "dcmi_i2c_read: ioctl error.\n");
        return DCMI_ERR_CODE_IOCTL_FAIL;
    }
    return ret;
}

STATIC int dcmi_i2c_eep_read(unsigned char slave_addr, unsigned long offset, unsigned char *out_buffer, int out_len)
{
    char in_buffer[2] = {0};
    char tmp_obuffer[128] = {0};
    unsigned long read_offset = 0;
    unsigned long read_addr;
    int size;
    int size_index;
    int err = DCMI_OK;
    const unsigned char addr_hi8_offset = 8;

    if (out_buffer == NULL) {
        gplog(LOG_ERR, "the out_buffer paramter is invalid.\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if ((offset + (unsigned long)out_len) >= EEP_MAX_SIZE) {
        gplog(LOG_ERR, "offset invalid.(%lx,%x)\r\n", offset, out_len);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    size = out_len;
    size_index = I2C_READ_LEN_MAX;

    while (size) {
        if (size < I2C_READ_LEN_MAX) {
            size_index = size;
        }

        read_addr = offset + read_offset;

        err = memset_s(tmp_obuffer, sizeof(tmp_obuffer), 0, sizeof(tmp_obuffer));
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "call memset_s failed.%d", err);
        }

        in_buffer[1] = (char)(read_addr & 0xff);
        in_buffer[0] = (char)((read_addr >> addr_hi8_offset) & 0xff);

        err = dcmi_i2c_read(
            slave_addr, in_buffer, (sizeof(in_buffer) / sizeof(in_buffer[0])), (char *)tmp_obuffer, size_index);
        if (err < DCMI_OK) {
            gplog(LOG_ERR, "read i2c failed(0x%x),ret is (0x%x).\r\n", slave_addr, err);
            return DCMI_ERR_CODE_INNER_ERR;
        }

        err = memcpy_s((out_buffer + read_offset), (unsigned long)out_len - read_offset, tmp_obuffer, size_index);
        if (err != EOK) {
            gplog(LOG_ERR, "call memcpy_s failed.%d", err);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
        read_offset += (unsigned long)size_index;
        size -= size_index;
    }
    return err;
}

STATIC int dcmi_i2c_write(unsigned char slave_addr, char *in_buffer, int in_len)
{
    int ret = 0;
    struct dcmi_i2c_msg msgs[2];
    struct dcmi_i2c_rdwr_ioctl_data rdwr_arg;
    int retry_num = I2C_MAX_RETRY_NUM;

    if (in_buffer == NULL) {
        gplog(LOG_ERR, "in_buffer is invalid.\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (in_len < 0) {
        gplog(LOG_ERR, "in_len is invalid.\r\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    while (retry_num) {
        retry_num--;

        /* write in_buffer */
        msgs[0].addr = slave_addr;
        msgs[0].flags = 0;
        /* 地址为buf[0],buf[1],由调用者给定,后面的buf为要写的内容 */
        msgs[0].buf = (unsigned char *)in_buffer;
        msgs[0].len = (unsigned short)in_len;

        rdwr_arg.msgs = msgs;
        rdwr_arg.nmsgs = 1;

        ret = ioctl(g_i2c_dev_fd, I2C_RDWR, &rdwr_arg);

        /* 延时一下,以免频繁写导致失败 */
        (void)usleep(DCMI_TASK_DELAY_5_MS);

        /* write read success */
        if (ret >= DCMI_OK) {
            ret = in_len;
            break;
        } else {
            if (retry_num > 0) {
                (void)usleep(DCMI_TASK_DELAY_500_MS * (I2C_MAX_RETRY_NUM - retry_num));
            }
        }
    }

    if (ret < DCMI_OK) {
        gplog(LOG_ERR, "dcmi_i2c_read: ioctl error\n");
        return DCMI_ERR_CODE_IOCTL_FAIL;
    }

    return ret;
}

STATIC int dcmi_i2c_eep_write_inner(
    unsigned char slave_addr, unsigned long addr_offset, const unsigned char *in_buffer, int in_len)
{
    char tmp_ibuffer[I2C_BUFFER_SIZE] = {0};
    char tmp_obuffer[I2C_BUFFER_SIZE] = {0};
    int ret;
    char readcn;
    const int addr_size = 2;
    const unsigned char addr_hi8_offset = 8;

    tmp_ibuffer[1] = (char)(addr_offset & 0xff);
    tmp_ibuffer[0] = (char)((addr_offset >> addr_hi8_offset) & 0xff);

    ret = memcpy_s((tmp_ibuffer + addr_size), sizeof(tmp_ibuffer) - addr_size, in_buffer, in_len);
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed.%d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    for (readcn = 0; readcn < I2C_MAX_RETRY_NUM; readcn++) {
        ret = dcmi_i2c_write(slave_addr, tmp_ibuffer, (in_len + addr_size));
        if (ret >= DCMI_OK) {
            (void)dcmi_i2c_read(slave_addr, tmp_ibuffer, addr_size, (char *)tmp_obuffer, in_len);
            if (!memcmp(&tmp_ibuffer[addr_size], tmp_obuffer, in_len)) {
                break;
            } else {
                gplog(LOG_ERR, "\r\nwrite and read not same.ret = %d\r\n", ret);
            }
        }
    }
    if (readcn == I2C_MAX_RETRY_NUM) {
        return DCMI_ERR_CODE_INNER_ERR;
    }
    return ret;
}

STATIC int dcmi_i2c_eep_write(
    unsigned char slave_addr, unsigned long offset, const unsigned char *in_buffer, int in_len)
{
    unsigned long addr_offset = offset; /* 用于记录当前写入数据的地址偏移 */
    int in_buffer_offset = 0;
    int write_size_max;
    int remain_size = in_len;
    int ret;

    if (in_buffer == NULL) {
        gplog(LOG_ERR, "parameter in_buffer invalid.\r\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    write_size_max = I2C_WRITE_LEN_MAX - ((int)offset % I2C_WRITE_LEN_MAX);

    while (remain_size > 0) {
        if (remain_size <= write_size_max) {
            ret = dcmi_i2c_eep_write_inner(slave_addr, addr_offset, in_buffer + in_buffer_offset, remain_size);
            if (ret < DCMI_OK) {
                gplog(LOG_ERR,
                    "write i2c failed remain_size(%d) <= write_size_max(%d), slv=0x%x\r\n",
                    remain_size,
                    write_size_max,
                    slave_addr);
                gplog(LOG_ERR, "offset=0x%lx, in_len=0x%x, in_buffer=%s", offset, in_len, in_buffer);
                return DCMI_ERR_CODE_INNER_ERR;
            }

            in_buffer_offset += remain_size;
            addr_offset += (unsigned long)remain_size;
            remain_size = 0;
        } else {
            ret = dcmi_i2c_eep_write_inner(slave_addr, addr_offset, in_buffer + in_buffer_offset, write_size_max);
            if (ret < DCMI_OK) {
                gplog(LOG_ERR, "write i2c failed(%x). remain_size > write_size_max, slv=0x%x\r\n", ret, slave_addr);
                return DCMI_ERR_CODE_INNER_ERR;
            }

            in_buffer_offset += write_size_max;
            remain_size -= write_size_max;
            addr_offset += (unsigned long)write_size_max;
            write_size_max = I2C_WRITE_LEN_MAX;
        }
    }

    return DCMI_OK;
}

int dcmi_i2c_get_data(
    const char *dev_name, unsigned char slave_addr, unsigned long reg_addr, unsigned char *out_buffer, int out_len)
{
    int ret;
    ret = dcmi_i2c_init(dev_name);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to init i2c.%d\n", ret);
        dcmi_close_i2c_fd();
        return ret;
    }

    usleep(DCMI_TASK_DELAY_100_MS);

    ret = dcmi_i2c_eep_read(slave_addr, reg_addr, out_buffer, out_len);
    if (ret < DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_i2c_eep_read failed!.%d \n", ret);
    }

    dcmi_close_i2c_fd();
    return ret;
}

int dcmi_i2c_set_data(const char *dev_name, unsigned char slave_addr,
    unsigned long reg_addr, const char *value, int len)
{
    int ret;

    ret = dcmi_i2c_init(dev_name);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to init i2c.%d\n", ret);
        dcmi_close_i2c_fd();
        return ret;
    }

    usleep(DCMI_TASK_DELAY_100_MS);

    ret = dcmi_i2c_eep_write(slave_addr, reg_addr, (unsigned char *)value, len);
    if (ret < DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_i2c_eep_write failed!. %d\n", ret);
    }

    dcmi_close_i2c_fd();
    return ret;
}

/* 开发者底板上，获取board id 和pcb id */
int dcmi_i2c_read_board_id(unsigned char addr, unsigned char *buf, int len)
{
    int ret;
    if (dcmi_board_chip_type_is_ascend_310b()) {
        ret = dcmi_i2c_get_data(I2C8_DEV_NAME, addr, I2C_REG_ADDR, buf, len);
    } else {
        ret = dcmi_i2c_get_data(I2C0_DEV_NAME, addr, I2C_REG_ADDR, buf, len);
    }
    return ret;
}

int dcmi_i2c_get_data_9555(const char *dev_name, unsigned char slave_addr, unsigned char reg_addr, char *value, int len)
{
    char req[2] = {0};
    int ret;

    ret = dcmi_i2c_init(dev_name);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to init i2c.\n");
        dcmi_close_i2c_fd();
        return ret;
    }

    usleep(DCMI_TASK_DELAY_100_MS);

    req[0] = (char)reg_addr;
    req[1] = 0;

    ret = dcmi_i2c_read(slave_addr, req, 1, value, len);
    if (ret < DCMI_OK) {
        gplog(LOG_ERR, "dcmi_i2c_read error!\n");
    }
    dcmi_close_i2c_fd();

    return ret;
}

#endif