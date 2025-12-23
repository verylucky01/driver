/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_SMBUS_H__
#define __DCMI_SMBUS_H__

#define READ_MIN_CNT   4
#define WRITE_MIN_CNT  5
#define I2C_MAX        12
#define MCU_SLAVE_ADDR 0x64
#define SEND_REQUEST   0x20
#define SEND_RESPONSE  0x21
#define I2C_NUM_9      9
#define I2C_NUM_6      6
#define SMBUS_DATA_SIZE    20

#define I2C_SLAVE 0x0703 /* Use this slave address */
#define I2C_SMBUS 0x0720 /* SMBus transfer */

struct dcmi_smbus_ioctl_data32 {
    unsigned char read_write;
    unsigned char command;
    unsigned int size;
    void *data; /* union i2c_smbus_data *data */
};

struct dcmi_cpu_req {
    unsigned char lun;
    unsigned char arg;
    unsigned short opcode;
    unsigned int offset;
    unsigned int length;
};

struct dcmi_smbus_std_req {
    unsigned char lun;
    unsigned char arg;
    unsigned short opcode;
    unsigned int offset;
    unsigned int length;
    unsigned char data[SMBUS_DATA_SIZE];
};

struct dcmi_smbus_std_rsp {
    unsigned short error_code;
    unsigned short opcode;
    unsigned int total_length;
    unsigned int length;
    unsigned char data[SMBUS_DATA_SIZE];
};

int dcmi_smbus_read_block(
    unsigned int id, unsigned int addr, unsigned char reg, unsigned long len, unsigned char *values);

int dcmi_smbus_write_block(
    unsigned int id, unsigned int addr, unsigned char reg, unsigned long len, const unsigned char *values);

#endif /* __DCMI_SMBUS_H__ */
