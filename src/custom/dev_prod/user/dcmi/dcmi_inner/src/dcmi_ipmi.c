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
#include "dcmi_ipmi.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>

#include "securec.h"
#include "dcmi_interface_api.h"
#include "dcmi_log.h"
#include "dcmi_common.h"

#define IPMI_DEV_NO 0
#define IPMI_RSPBUF_SIZE 80
#define IPMI_RSPBUF_SIZE_910_93 255
#define IPMI_BMC_LUN 0
#define IPMI_NETFN_PICMG 0x30
#define IPMI_NETFN_GET_PICMG 0x6
#define IPMI_CMD_DATA_SIZE 11
#define IPMI_CMD_DATA_SIZE_2 5
#define IPMI_CMD_DATA_SLOT_ID_INDEX 8
#define IPMI_CMD_DATA_CHIP_ID_INDEX 9
#define IPMI_CMD_DATA_RESET_CMD_INDEX 10
#define IPMI_FRU_CMD_DATA_SLOT_ID_INDEX 7
#define IPMI_CPLD_CMD_DATA_FRU_ID_INDEX 1

#define PICMG_SET_FRU_LED_STATE_CMD 0x93
#define PICMG_GET_DFT_INFO_CMD 0x90
#define PICMG_GET_OUTBAND_STATE_CMD 0x1

#define IPMI_IOC_MAGIC 'i'
#define IPMICTL_RECEIVE_MSG_TRUNC _IOWR(IPMI_IOC_MAGIC, 11, struct dcmi_ipmi_recv)
#define IPMICTL_SEND_COMMAND _IOR(IPMI_IOC_MAGIC, 13, struct dcmi_ipmi_req)
#define IPMICTL_SEND_COMMAND_SETTIME _IOR(IPMI_IOC_MAGIC, 21, struct dcmi_ipmi_req_settime)
#define IPMICTL_GET_MY_ADDRESS_CMD _IOR(IPMI_IOC_MAGIC, 18, unsigned int)

#define IPMI_BMC_CHANNEL 0x0f
#define IPMI_SYSTEM_INTERFACE_ADDR_TYPE 0x0c

/* IPMI command message sequence */
static long g_msg_seq = 0;

static int dcmi_open_ipmi(char dev_no, int *fd)
{
    int err;
    int count;
    char dev_name[256] = {0};
    char *dev_prex[] = { "/dev/ipmidev", "/dev/ipmi", "/dev/ipmidev/", "/dev/ipmi/" };

    for (count = (int)(sizeof(dev_prex) / sizeof(dev_prex[0]) - 1); count >= 0; count--) {
        err = snprintf_s(dev_name, sizeof(dev_name), sizeof(dev_name) - 1, "%s%d", dev_prex[count], dev_no);
        if (err <= DCMI_OK) {
            gplog(LOG_ERR, "call snprintf_s failed.%d.", err);
        }

        *fd = open(dev_name, O_RDONLY);
        if ((errno == EPERM) || (errno == EACCES)) {
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        }
        if ((*fd) < 0) {
            continue;
        }

        return DCMI_OK;
    }

    return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
}

static int dcmi_close_ipmi(int fd)
{
    if (fd >= 0) {
        return close(fd);
    }
    return DCMI_ERR_CODE_INNER_ERR;
}

static int dcmi_ipmi_send_req(int fd, struct dcmi_ipmi_msg *ipmiMsg, unsigned char lun)
{
    struct dcmi_ipmi_req ipmi_req = {0};
    struct dcmi_ipmi_system_interface_addr ipmi_bmc_addr = {0};
    struct dcmi_ipmi_req_settime ipmi_req_settime = { { 0 } };
    int ret;

    /* IPMI_SYSTEM_INTERFACE_ADDR_TYPE类型的地址,BMC直接处理返回的消息 */
    ipmi_bmc_addr.addr_type = IPMI_SYSTEM_INTERFACE_ADDR_TYPE;
    ipmi_bmc_addr.channel = IPMI_BMC_CHANNEL;
    ipmi_bmc_addr.lun = lun;
    ipmi_req.req_addr = (unsigned char *)&ipmi_bmc_addr;
    ipmi_req.req_addr_len = sizeof(ipmi_bmc_addr);
    ipmi_req.req_msg.cmd = ipmiMsg->cmd;
    ipmi_req.req_msg.netfn = ipmiMsg->netfn;
    ipmi_req.req_msgid = g_msg_seq;
    ipmi_req.req_msg.data = ipmiMsg->data;
    ipmi_req.req_msg.data_len = ipmiMsg->data_len;

    g_msg_seq++;

    if (ipmi_req.req_msg.data[3] == 0x53) { /* data[3] 为带外复位查询和设置标志，0x53为设置 */
        ipmi_req_settime.req = ipmi_req;
        ipmi_req_settime.retries = 0;
        ipmi_req_settime.retry_time_ms = 0;
        ret = ioctl(fd, IPMICTL_SEND_COMMAND_SETTIME, &ipmi_req_settime);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "ipmi ioctl IPMICTL_SEND_COMMAND_SETTIME failed. ret is %d", ret);
            return DCMI_ERR_CODE_IOCTL_FAIL;
        }
    } else {
        ret = ioctl(fd, IPMICTL_SEND_COMMAND, &ipmi_req);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "ipmi ioctl IPMICTL_SEND_COMMAND failed. ret is %d", ret);
            return DCMI_ERR_CODE_IOCTL_FAIL;
        }
    }
    return ret;
}

static int dcmi_ipmi_recv_rsp(int fd, unsigned char *resp, int resp_size, int *resp_len)
{
    struct dcmi_ipmi_addr ipmi_addr = {0};
    struct dcmi_ipmi_recv ipmi_recv = {0};
    int ret;

    /* Resp data is ready now */
    ipmi_recv.recv_addr = (unsigned char *)&ipmi_addr;
    ipmi_recv.recv_addr_len = (int)sizeof(ipmi_addr);
    ipmi_recv.recv_msg.data = resp;
    ipmi_recv.recv_msg.data_len = (unsigned short)resp_size;

    ret = ioctl(fd, IPMICTL_RECEIVE_MSG_TRUNC, &ipmi_recv);
    if (ret == (-1)) {
        if (ipmi_recv.recv_msg.data_len == resp_size) {
            *resp_len = resp_size;
            return DCMI_OK;
        }
        return DCMI_ERR_CODE_RECV_MSG_FAIL;
    }
    *resp_len = ipmi_recv.recv_msg.data_len;
    return DCMI_OK;
}

static int dcmi_ipmi_cmd(struct dcmi_ipmi_msg *ipmiMsg, unsigned char *presp, int sresp, int *rlen, unsigned char lun)
{
    int ret;
    int fd = 0;
    fd_set read_fd;
    int my_addr;

    ret = dcmi_open_ipmi(IPMI_DEV_NO, &fd);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_open_ipmi failed. err is %d", ret);
        return ret;
    }

    /* Test ipmi device */
    ret = ioctl(fd, IPMICTL_GET_MY_ADDRESS_CMD, &my_addr);
    if (ret) {
        gplog(LOG_ERR, "ipmi ioctl IPMICTL_GET_MY_ADDRESS_CMD failed. ret is %d", ret);
        ret = dcmi_close_ipmi(fd);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_close_ipmi failed. ret is %d", ret);
        }
        return DCMI_ERR_CODE_IOCTL_FAIL;
    }

    FD_ZERO(&read_fd);
    FD_SET(fd, &read_fd);

    ret = dcmi_ipmi_send_req(fd, ipmiMsg, lun);
    if (ret != DCMI_OK) {
        ret = dcmi_close_ipmi(fd);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_close_ipmi failed. ret is %d", ret);
        }
        return DCMI_ERR_CODE_SEND_MSG_FAIL;
    }

    sleep(1);

    ret = dcmi_ipmi_recv_rsp(fd, presp, sresp, rlen);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_ipmi_recv_rsp failed. ret is %d", ret);
    }

    ret = dcmi_close_ipmi(fd);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_close_ipmi failed. ret is %d", ret);
    }

    return DCMI_OK;
}

int dcmi_ipmi_reset_npu_910_93(int card_id)
{
    int ret;
    int rsp_len = IPMI_MAX_ADDR_SIZE;
    struct dcmi_ipmi_msg ipmiMsg = {0};
    /* cmd_data[3] 为查询和设置标志，0x53为设置，0x54为查询 */
    unsigned char cmd_data[IPMI_CMD_DATA_SIZE] = { 0xDB, 0x07, 0x00, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    unsigned char rsp_data[IPMI_RSPBUF_SIZE] = {0};

    cmd_data[IPMI_CMD_DATA_SLOT_ID_INDEX] = (unsigned char)(card_id + 1);       /* NPU模组ID */
    cmd_data[IPMI_CMD_DATA_RESET_CMD_INDEX] = 0x06;                       /* 1-NPU芯片复位 */

    ipmiMsg.cmd = PICMG_SET_FRU_LED_STATE_CMD;
    ipmiMsg.netfn = IPMI_NETFN_PICMG;
    ipmiMsg.data = cmd_data;
    ipmiMsg.data_len = (unsigned short)sizeof(cmd_data);

    /* 复位 D-chip 借用PICMG中设置FRU LED的网络功能码和命令字，在请求数据中传递具体槽位号以及查询还是设置 */
    ret = dcmi_ipmi_cmd(&ipmiMsg, rsp_data, sizeof(rsp_data), &rsp_len, IPMI_BMC_LUN);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "reset card %d failed. ret is %d", card_id, ret);
        return ret;
    }

    // Check complete code
    if (rsp_data[0] != 0x00) {
        gplog(LOG_ERR, "wrong complete code[0x%02x].", rsp_data[0]);
        return DCMI_ERR_CODE_RECV_MSG_FAIL;
    }

    return DCMI_OK;
}

int dcmi_ipmi_reset_npu(int slot_id, int chip_id)
{
    int ret;
    int rsp_len = IPMI_MAX_ADDR_SIZE;
    struct dcmi_ipmi_msg ipmiMsg = {0};
    /* cmd_data[3] 为查询和设置标志，0x53为设置，0x54为查询 */
    unsigned char cmd_data[IPMI_CMD_DATA_SIZE] = { 0xDB, 0x07, 0x00, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    unsigned char rsp_data[IPMI_RSPBUF_SIZE] = {0};

    cmd_data[IPMI_CMD_DATA_SLOT_ID_INDEX] = (unsigned char)slot_id;       /* pcie卡的槽位号 */
    cmd_data[IPMI_CMD_DATA_CHIP_ID_INDEX] = (unsigned char)(chip_id + 1); /* 需要复位的芯片编号 */
    cmd_data[IPMI_CMD_DATA_RESET_CMD_INDEX] = 0x01;                       /* 1-NPU芯片复位 */

    ipmiMsg.cmd = PICMG_SET_FRU_LED_STATE_CMD;
    ipmiMsg.netfn = IPMI_NETFN_PICMG;
    ipmiMsg.data = cmd_data;
    ipmiMsg.data_len = (unsigned short)sizeof(cmd_data);

    /* 复位 D-chip 借用PICMG中设置FRU LED的网络功能码和命令字，在请求数据中传递具体槽位号以及查询还是设置 */
    ret = dcmi_ipmi_cmd(&ipmiMsg, rsp_data, sizeof(rsp_data), &rsp_len, IPMI_BMC_LUN);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "reset slot %d chip %d failed. ret is %d", slot_id, chip_id, ret);
        return ret;
    }

    // Check complete code
    if (rsp_data[0] != 0x00) {
        gplog(LOG_ERR, "wrong complete code[0x%02x].", rsp_data[0]);
        return DCMI_ERR_CODE_RECV_MSG_FAIL;
    }

    return DCMI_OK;
}

int dcmi_ipmi_get_npu_outband_channel_state(int *channel_state)
{
    int ret;
    int rsp_len = 0; // 默认返回长度为0，即通道状态失败
    const int CHANNEL_STATUS_SUCCESS = 1;
    /* 通过获取BMC版本号命令判断通道状态 */
    unsigned char cmd_data[IPMI_CMD_DATA_SIZE_2] = { 0x08, 0x00, 0x01, 0x00, 0x04 };
    unsigned char rsp_data[IPMI_RSPBUF_SIZE_910_93] = {0};
    struct dcmi_ipmi_msg ipmiMsg = {0};

    ipmiMsg.cmd = PICMG_GET_DFT_INFO_CMD;
    ipmiMsg.netfn = IPMI_NETFN_PICMG;
    ipmiMsg.data = cmd_data;
    ipmiMsg.data_len = 0;
    ret = dcmi_ipmi_cmd(&ipmiMsg, rsp_data, sizeof(rsp_data), &rsp_len, IPMI_BMC_LUN);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "get channel state failed. ret is %d", ret);
        return ret;
    }

    // 通过响应返回长度来判断通道状态
    if (rsp_len == 0) {
        gplog(LOG_ERR, "rsp_len = %d", rsp_len);
        return DCMI_ERR_CODE_RECV_MSG_FAIL;
    }

    *channel_state = CHANNEL_STATUS_SUCCESS;
    return DCMI_OK;
}

int dcmi_ipmi_get_npu_reset_state(int slot_id, int chip_id, unsigned char *state)
{
    int ret;
    int rsp_len = IPMI_MAX_ADDR_SIZE;
    /* cmd_data[3] 为查询和设置标志，0x53为设置，0x54为查询 */
    unsigned char cmd_data[IPMI_CMD_DATA_SIZE] = { 0xDB, 0x07, 0x00, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    unsigned char rsp_data[IPMI_RSPBUF_SIZE] = {0};
    struct dcmi_ipmi_msg ipmiMsg = {0};

    cmd_data[IPMI_CMD_DATA_SLOT_ID_INDEX] = (unsigned char)slot_id;       /* pcie卡的槽位号 */
    cmd_data[IPMI_CMD_DATA_CHIP_ID_INDEX] = (unsigned char)(chip_id + 1); /* 需要复位的芯片编号 */
    cmd_data[IPMI_CMD_DATA_RESET_CMD_INDEX] = 0x01;                       /* 1-NPU芯片复位 */

    ipmiMsg.cmd = PICMG_SET_FRU_LED_STATE_CMD;
    ipmiMsg.netfn = IPMI_NETFN_PICMG;
    ipmiMsg.data = cmd_data;
    ipmiMsg.data_len = (unsigned short)sizeof(cmd_data);
    ret = dcmi_ipmi_cmd(&ipmiMsg, rsp_data, sizeof(rsp_data), &rsp_len, IPMI_BMC_LUN);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "get slot %d chip %d reset state failed. ret is %d", slot_id, chip_id, ret);
        return ret;
    }

    // Check complete code
    if (rsp_data[0] != 0x00) {
        gplog(LOG_ERR, "wrong complete code[0x%02x]", rsp_data[0]);
        return DCMI_ERR_CODE_RECV_MSG_FAIL;
    }

    // 查询的结果,1:执行结果返回值; 2~4:0xDB0700; 5: 当前芯片的复位结果
    *state = rsp_data[4];
    return DCMI_OK;
}

int dcmi_ipmi_get_cpld_value(int addr, unsigned char *value)
{
    int ret;
    int rspLen = IPMI_MAX_ADDR_SIZE;
    struct ipmi_get_hardware_reg_info_req reqData = { { 0 } };
    struct ipmi_get_hardware_reg_info_rsp rspData = { 0 };
    struct dcmi_ipmi_msg ipmiMsg = {0};

    reqData.protocol_id[0] = (unsigned char)(IPMI_PROTOCOL_ID >> SHIFT_SIXTEEN_BITS);  // 0:0xDB
    reqData.protocol_id[1] = (unsigned char)(IPMI_PROTOCOL_ID >> SHIFT_EIGHT_BITS);    // 1:0x07
    reqData.protocol_id[2] = (unsigned char)(IPMI_PROTOCOL_ID);                        // 2:0x00
    reqData.sub_cmd = IPMI_GET_HARDWARE_REG_INFO_SUBCMD;
    reqData.hardware_type = IPMI_HARDWARE_TYPE_CPLD;
    reqData.chip_id = IPMI_HARDWARE_TYPE_CPLD_CHIPID;
    reqData.addr = 0x0;
    reqData.offset = (unsigned short)addr;
    reqData.length = IPMI_HARDWARE_REG_SIZE;

    ipmiMsg.cmd = PICMG_GET_HARDWARE_REG_INFO_CMD;
    ipmiMsg.netfn = IPMI_NETFN_PICMG;
    ipmiMsg.data = (unsigned char *)&reqData;
    ipmiMsg.data_len = (unsigned short)sizeof(reqData);

    ret = dcmi_ipmi_cmd(&ipmiMsg, (unsigned char *)&rspData, sizeof(rspData), &rspLen, IPMI_BMC_LUN);
    if (ret != DCMI_OK) {
        gplog(DCMI_OK, "Get cpld value failed. %d\n", ret);
        return ret;
    }

    // Check complete code
    if (rspData.complete_code != 0x00) {
        gplog(LOG_ERR, "Wrong complete code[%02x]\n", rspData.complete_code);
        return DCMI_ERR_CODE_RECV_MSG_FAIL;
    }

    // 当前寄存器结果
    *value = rspData.reg_value;

    return DCMI_OK;
}

int dcmi_ipmi_get_npu_fru_id(int card_id, unsigned char *fru_id)
{
    int ret;
    int rsp_len = IPMI_MAX_ADDR_SIZE;
    unsigned char cmd_data[IPMI_CMD_DATA_SIZE] = { 0xDB, 0x07, 0x00, 0x3B, 0x00, 0x5C, 0x01, 0x00, 0x00, 0x00, 0x01 };
    unsigned char rsp_data[IPMI_RSPBUF_SIZE] = {0};
    struct dcmi_ipmi_msg ipmiMsg = {0};

    cmd_data[IPMI_FRU_CMD_DATA_SLOT_ID_INDEX] = (unsigned char)(card_id + 1);       /* NPU模组ID */

    ipmiMsg.cmd = PICMG_SET_FRU_LED_STATE_CMD;
    ipmiMsg.netfn = IPMI_NETFN_PICMG;
    ipmiMsg.data = cmd_data;
    ipmiMsg.data_len = (unsigned short)sizeof(cmd_data);
    ret = dcmi_ipmi_cmd(&ipmiMsg, rsp_data, sizeof(rsp_data), &rsp_len, IPMI_BMC_LUN);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "get card %d fru_id failed. ret is %d", card_id, ret);
        return ret;
    }

    // Check complete code
    if (rsp_data[0] != 0x00) {
        gplog(LOG_ERR, "wrong complete code[0x%02x]", rsp_data[0]);
        return DCMI_ERR_CODE_RECV_MSG_FAIL;
    }

    // 查询的结果,0:执行结果返回值; 1~4:0xDB070000; 5: 当前芯片的fru_id
    *fru_id = rsp_data[5];
    return DCMI_OK;
}

int dcmi_ipmi_get_cpld_version(unsigned char fru_id, unsigned char *cpld_version)
{
    int ret;
    int rsp_len = IPMI_MAX_ADDR_SIZE;
    unsigned char cmd_data[IPMI_CMD_DATA_SIZE_2] = { 0x08, 0x00, 0x02, 0x00, 0xFF };
    unsigned char rsp_data[IPMI_RSPBUF_SIZE] = {0};
    struct dcmi_ipmi_msg ipmiMsg = {0};
    int cpld_version_index = 0, start_index = 9;

    cmd_data[IPMI_CPLD_CMD_DATA_FRU_ID_INDEX] = fru_id;       /* NPU模组FRU_ID */

    ipmiMsg.cmd = PICMG_GET_DFT_INFO_CMD;
    ipmiMsg.netfn = IPMI_NETFN_PICMG;
    ipmiMsg.data = cmd_data;
    ipmiMsg.data_len = (unsigned short)sizeof(cmd_data);
    ret = dcmi_ipmi_cmd(&ipmiMsg, rsp_data, sizeof(rsp_data), &rsp_len, IPMI_BMC_LUN);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "get cpld_version failed, fru_id=%u. ret is %d", fru_id, ret);
        return ret;
    }

    // Check complete code
    if (rsp_data[0] != 0x00) {
        gplog(LOG_ERR, "wrong complete code[0x%02x]", rsp_data[0]);
        return DCMI_ERR_CODE_RECV_MSG_FAIL;
    }

    // 查询的结果,从第9位为cpld版本号ascii码值
    for (; start_index < rsp_len; start_index++) {
        if (cpld_version_index >= CPLD_VERSION_SIZE - 1) {
            break;
        }

        cpld_version[cpld_version_index++] = rsp_data[start_index];
    }
    cpld_version[cpld_version_index] = '\0';

    return DCMI_OK;
}
#endif