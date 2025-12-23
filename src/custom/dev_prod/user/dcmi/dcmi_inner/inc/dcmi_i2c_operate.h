/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_I2C_OPERATE_H__
#define __DCMI_I2C_OPERATE_H__

#include "dcmi_interface_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define I2C_RETRIES     0x0701
#define I2C_TIMEOUT     0x0702
#define I2C_SLAVE       0x0703
#define I2C_RDWR        0x0707
#define I2C_BUS_MODE    0x0780
#ifndef I2C_M_RD
#define I2C_M_RD        0x01
#endif

#define I2C_SLAVE_ADDR     (0x55)

#define I2C_MAX_RETRY_NUM   3
#define I2C_READ_LEN_MAX    64
#define EEP_MAX_SIZE       (0xFFFF)
#define I2C_WRITE_LEN_MAX   16
#define I2C_BUFFER_SIZE     80

#define DCMI_TASK_DELAY_5_MS   5000
#define DCMI_TASK_DELAY_100_MS 100000
#define DCMI_TASK_DELAY_500_MS 500000

#define TEST_SUCCESS        (0)   // 测试成功
#define TEST_FAIL           (1)   // 测试失败
#define TEST_NOT_STARTED    (-1)  // 测试未启动
#define TEST_ING            (2)   // 测试中

#define I2C0_DEV_NAME   "/dev/i2c-0"
#define I2C1_DEV_NAME   "/dev/i2c-1"
#define I2C2_DEV_NAME   "/dev/i2c-2"
#define I2C3_DEV_NAME   "/dev/i2c-3"
#define I2C8_DEV_NAME   "/dev/i2c-8"
#define I2C9_DEV_NAME   "/dev/i2c-9"
#define I2C10_DEV_NAME  "/dev/i2c-10"
#define I2C11_DEV_NAME  "/dev/i2c-11"

#define I2C_REG_ADDR    (0x0)
#define I2C_ELABEL_ADDR  0xa0 >> 1

#define I2C_SLAVE_PCA9555_BOARDINFO 0x20

struct dcmi_i2c_msg {
    unsigned short addr; /* slave address */
    unsigned short flags;
    unsigned short len;
    unsigned char *buf; /* message data pointer */
};

struct dcmi_i2c_rdwr_ioctl_data {
    struct dcmi_i2c_msg *msgs; /* i2c_msg[] pointer */
    int nmsgs;                 /* i2c_msg_str Nums */
};

int dcmi_i2c_get_data(
    const char *dev_name, unsigned char slave_addr, unsigned long reg_addr, unsigned char *buffer, int len);
int dcmi_i2c_set_data(
    const char *dev_name, unsigned char slave_addr, unsigned long reg_addr, const char *value, int len);
int dcmi_i2c_read_board_id(unsigned char addr, unsigned char *buf, int len);
int dcmi_i2c_get_data_9555(
    const char *dev_name, unsigned char slave_addr, unsigned char reg_addr, char *value, int len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
 
#endif /* __DCMI_I2C_OPERATE_H__ */