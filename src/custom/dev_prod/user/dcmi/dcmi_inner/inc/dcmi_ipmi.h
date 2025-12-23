/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_IPMI_H__
#define __DCMI_IPMI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define IPMI_MAX_ADDR_SIZE 32
#define IPMI_PROTOCOL_ID 0xDB0700 // IPMI协议标识为0xDB0700
#define IPMI_PROTOCOL_ID_SIZE 3
#define PICMG_SET_FRU_LED_STATE_CMD 0x93
#define PICMG_GET_HARDWARE_REG_INFO_CMD 0x92
#define PICMG_GET_DFT_INFO_CMD 0x90
#define IPMI_GET_HARDWARE_REG_INFO_SUBCMD 0x22
#define IPMI_HARDWARE_TYPE_CPLD 0x1
#define IPMI_HARDWARE_TYPE_CPLD_CHIPID 0x1
#define IPMI_HARDWARE_REG_SIZE 0x1

#define SHIFT_FOUR_BITS 4
#define SHIFT_EIGHT_BITS 8
#define SHIFT_SIXTEEN_BITS 16

struct dcmi_ipmi_addr {
    int   addr_type;
    short channel;
    char  data[IPMI_MAX_ADDR_SIZE];
};

struct dcmi_ipmi_msg {
    unsigned char     netfn;
    unsigned char       cmd;
    unsigned short data_len;
    unsigned char     *data;
};

struct dcmi_ipmi_req {
    unsigned char      *req_addr;
    unsigned int    req_addr_len;
    long               req_msgid;
    struct dcmi_ipmi_msg      req_msg;
};

struct dcmi_ipmi_req_settime {
    struct dcmi_ipmi_req    req;
    int                 retries;
    unsigned int  retry_time_ms;
};

struct dcmi_ipmi_recv {
    int       recv_type;
    unsigned char *recv_addr;
    int        recv_addr_len;
    long          recv_msgid;
    struct dcmi_ipmi_msg recv_msg;
};

struct dcmi_ipmi_system_interface_addr {
    int           addr_type;
    short           channel;
    unsigned char       lun;
};

struct dcmi_ipmi_ipmb_addr {
    int            addr_type;
    short            channel;
    unsigned char slave_addr;
    unsigned char        lun;
};

struct ipmi_get_hardware_reg_info_req {
    unsigned char protocol_id[IPMI_PROTOCOL_ID_SIZE]; // IPMI协议标识，固定位0xDB0700
    unsigned char sub_cmd;                            // 子命令=22h
    unsigned char hardware_type; // 硬件协议类型 (1: CPLD 2: GPIO 3: I2C 4: EDMA Other: reserved)
    unsigned char chip_id;       // Chip ID(芯片ID)
    unsigned short addr;   // Addr(器件地址), LS-byte first (hardware_type为3时该字段有效，其余默认为0)
    unsigned short offset; // Offset(偏移), LS-byte first (01 00：表示偏移1个字节)
    unsigned char length;  // Length(读取数据长度，1-32字节）
};

struct ipmi_get_hardware_reg_info_rsp {
    unsigned char complete_code;                      // Completion Code
    unsigned char protocol_id[IPMI_PROTOCOL_ID_SIZE]; // IPMI协议标识，固定位0xDB0700
    unsigned char reg_value; // Data. 实际返回的数据可能小于等于读取长度Length.
};


int dcmi_ipmi_reset_npu(int slot_id, int chip_id);
int dcmi_ipmi_get_npu_outband_channel_state(int *channel_state);
int dcmi_ipmi_reset_npu_910_93(int card_id);
int dcmi_ipmi_get_npu_reset_state(int slot_id, int chip_id, unsigned char *state);
int dcmi_ipmi_get_cpld_value(int addr, unsigned char *value);
int dcmi_ipmi_get_npu_fru_id(int card_id, unsigned char *fru_id);
int dcmi_ipmi_get_cpld_version(unsigned char fru_id, unsigned char *cpld_version);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DCMI_IPMI_H__ */

