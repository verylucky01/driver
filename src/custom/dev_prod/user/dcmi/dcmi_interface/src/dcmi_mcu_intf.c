/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "securec.h"
#include "dsmi_common_interface_custom.h"
#include "dcmi_interface_api.h"
#include "dcmi_init_basic.h"
#include "dcmi_fault_manage_intf.h"
#include "dcmi_log.h"
#include "dcmi_smbus_operate.h"
#include "dcmi_os_adapter.h"
#include "dcmi_mcu_health_error_info.h"
#include "dcmi_virtual_intf.h"
#include "dcmi_i2c_operate.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_product_judge.h"
#include "dcmi_environment_judge.h"
#include "dcmi_elabel_operate.h"
#include "dcmi_mcu_intf.h"

struct dcmi_mcu_log_info g_dcmi_mcu_log_info[MCU_LOG_MAX] = {
    {MCU_LOG_ERR,    0xc,  0x32000, "error_log"},
    {MCU_LOG_OPRATE, 0x26, 0x19400, "operate_log"},
    {MCU_LOG_MAINT,  0x27, 0x19C00, "maintaince_log"},
};

/* 来控制dcmi_mcu_set_lock和dcmi_mcu_set_unlock是否需要执行 */
static int g_dcmi_mcu_lock_flag = FALSE;

#if defined DCMI_VERSION_1

int dcmi_mcu_set_license_info(int card_id, char *license, int len)
{
    return dcmi_set_card_customized_info(card_id, license, len);
}

int dcmi_mcu_get_license_info(int card_id, char *data_info, int *len)
{
    char info[DCMI_MCU_CUSTOMIZED_INFO_MAX_LEN] = {0};
    int err;
    if ((data_info == NULL) || (len == NULL)) {
        gplog(LOG_ERR, "input para invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err =  dcmi_get_card_customized_info(card_id, info, sizeof(info));
    if (err != DCMI_OK) {
        if (err != DCMI_ERR_CODE_NOT_SUPPORT) {
            gplog(LOG_ERR, "dcmi_get_card_customized_info failde err is %d.", err);
        }
        return err;
    }

    err = strncpy_s(data_info, strlen(info) + 1, info, strlen(info));
    if (err != EOK) {
        gplog(LOG_ERR, "call strncpy_s failde err is %d.", err);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    *len = (int)strlen(info);
    return DCMI_OK;
}

#endif

/* 对全局变量的读写应集中封装 */
void dcmi_set_mcu_lock_flag(bool flag)
{
    g_dcmi_mcu_lock_flag = flag;
}

bool dcmi_get_mcu_lock_flag(void)
{
    return g_dcmi_mcu_lock_flag;
}

static int dcmi_mcu_get_file_lock(int fd, unsigned int timeout)
{
#ifndef _WIN32
    int ret;
    unsigned int index;
    unsigned int time_curr = 0;
    struct flock f_lock = {0};

    f_lock.l_type = F_WRLCK;
    f_lock.l_whence = 0;
    f_lock.l_len = 0;

    while (time_curr < timeout) {
        /* 首先循环一定次数尝试不阻塞方式获取锁 */
        for (index = 0; index < DCMI_MCU_MUTEX_FIRST_TRY_TIMES; index++) {
            ret = fcntl(fd, F_SETLK, &f_lock);
            if (ret == DCMI_OK) {
                return DCMI_OK;
            }
        }

        /* 未获取到锁，等待1ms，再次尝试获取 */
        (void)usleep(DCMI_MCU_MUTEX_SLEEP_TIMES_1MS);
        time_curr++;
    }
    return DCMI_ERR_CODE_INNER_ERR;
#else
    return DCMI_OK;
#endif
}

/* 进程间文件锁，当做进程间锁 */
STATIC int dcmi_mcu_set_lock(int *fd, unsigned int timeout)
{
#ifndef _WIN32
    if (dcmi_board_type_is_station() || dcmi_board_type_is_A500_A2()) {
        int fd_curr = -1;

        if (dcmi_get_mcu_lock_flag() == TRUE) {
            return DCMI_OK;
        }

        fd_curr = open(DCMI_MCU_LOCK_FILE_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if (fd_curr < 0) {
            gplog(LOG_ERR, "mcu_get_lock:open file %s failed! ", DCMI_MCU_LOCK_FILE_NAME);
            return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
        }

        *fd = fd_curr;

        if (dcmi_mcu_get_file_lock(fd_curr, timeout) != DCMI_OK) {
            /* 失败需要关闭文件，成功需要调用mcu_set_unlock来关闭 */
            close(fd_curr);
            *fd = -1;
            return DCMI_ERR_CODE_INNER_ERR;
        }
    } else {
        (void)fd;
        (void)timeout;
    }
#endif
    return DCMI_OK;
}

/* 进程间文件锁，当做进程间锁 */
STATIC void dcmi_mcu_set_unlock(int fd)
{
#ifndef _WIN32
    if (dcmi_board_type_is_station() || dcmi_board_type_is_A500_A2()) {
        struct flock f_lock = {0};

        if (dcmi_get_mcu_lock_flag() == TRUE) {
            return;
        }

        if (fd < 0) {
            gplog(LOG_ERR, "mcu_set_unlock:fd invalid. fd = %d", fd);
            return;
        }

        f_lock.l_type = F_UNLCK;
        f_lock.l_whence = 0;
        f_lock.l_len = 0;

        fcntl(fd, F_SETLK, &f_lock);
        close(fd);

        return;
    } else {
        (void)fd;
        return;
    }
#else
    return;
#endif
}

int dcmi_mcu_set_lock_up(int *fd, unsigned int timeout)
{
#ifndef _WIN32
    if (dcmi_board_type_is_station() || dcmi_board_type_is_A500_A2()) {
        int ret;
        dcmi_set_mcu_lock_flag(FALSE);
        ret = dcmi_mcu_set_lock(fd, timeout);
        if (ret == DCMI_OK) {
            dcmi_set_mcu_lock_flag(TRUE);
        }

        return ret;
    } else {
        (void)fd;
        (void)timeout;
        return DCMI_OK;
    }
#else
    return DCMI_OK;
#endif
}

void dcmi_mcu_set_unlock_up(int fd)
{
#ifndef _WIN32
    if (dcmi_board_type_is_station() || dcmi_board_type_is_A500_A2()) {
        dcmi_set_mcu_lock_flag(FALSE);
        dcmi_mcu_set_unlock(fd);
        return;
    } else {
        (void)fd;
        return;
    }
#else
    return;
#endif
}

int dcmi_mcu_release_and_get_lock(int *fd)
{
    int ret;
    dcmi_mcu_set_unlock_up(*fd);
    usleep(DCMI_MCU_TASK_DELAY_10_MS);
    ret = dcmi_mcu_set_lock_up(fd, DCMI_MCU_GET_LOCK_TIMOUT);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call mcu_get_lock failed. err is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    return DCMI_OK;
}

static int dcmi_init_dcmi_smbus_req(struct dcmi_smbus_std_req *req, MCU_SMBUS_REQ_MSG *mcu_req)
{
    int ret = 0;
    int send_msg_data_max_len = dcmi_mcu_get_send_data_max_len();

    /* 发送请求报文 */
    req->lun = DCMI_MCU_MSG_LUN;
    req->arg = mcu_req->arg;
    req->opcode = mcu_req->opcode;
    req->offset = mcu_req->offset;
    req->length = mcu_req->lenth;

    mcu_req->req_data_len = (mcu_req->req_data_len > send_msg_data_max_len) ?
                            send_msg_data_max_len : mcu_req->req_data_len;
    if (mcu_req->req_data_len != 0) {
        ret = memcpy_s(&req->data[0], send_msg_data_max_len, mcu_req->req_data, mcu_req->req_data_len);
        if (ret != EOK) {
            gplog(LOG_ERR, "call memcpy_s failed. ret is %d.", ret);
        }
    }
    return ret;
}

static int dcmi_mcu_get_info_by_i2c(int card_id, MCU_SMBUS_REQ_MSG *mcu_req, MCU_SMBUS_RSP_MSG *mcu_rsp)
{
#ifndef _WIN32
    struct dcmi_smbus_std_req req = {0};
    struct dcmi_smbus_std_rsp rsp = {0};
    int ret, fd;
    unsigned int i2c_num = dcmi_board_chip_type_is_ascend_310b() ? I2C_NUM_6 : I2C_NUM_9;

    (void)card_id;

    ret = dcmi_init_dcmi_smbus_req(&req, mcu_req);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_init_dcmi_smbus_req failed. ret is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dcmi_mcu_set_lock(&fd, DCMI_MCU_GET_LOCK_TIMOUT);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call mcu_get_lock failed. ret is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dcmi_smbus_write_block(i2c_num, MCU_SLAVE_ADDR, SEND_REQUEST,
        DMP_MSG_HEAD_LENGTH + mcu_req->req_data_len, (const unsigned char *)&req);
    if (ret != DCMI_OK) {
        dcmi_mcu_set_unlock(fd);
        gplog(LOG_INFO, "call i2c_smbus_write_block failed. ret is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    /* 发送应答报文 */
    ret = dcmi_smbus_read_block(
        i2c_num, MCU_SLAVE_ADDR, SEND_RESPONSE, DMP_MSG_HEAD_LENGTH + mcu_req->lenth + 1, (unsigned char *)&rsp);
    if (ret != DCMI_OK) {
        dcmi_mcu_set_unlock(fd);
        gplog(LOG_INFO, "call i2c_smbus_read_block failed. ret is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    dcmi_mcu_set_unlock(fd);

    if (rsp.error_code != DCMI_OK) {
        gplog(LOG_ERR, "error_code=%hu.", rsp.error_code);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = memcpy_s(mcu_rsp->data_info, dcmi_mcu_get_recv_data_max_len(), &rsp.data[0], rsp.length);
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed. ret is %d.", ret);
    }

    mcu_rsp->len = (int)rsp.length;
    mcu_rsp->total_len = (int)rsp.total_length;
    return DCMI_OK;
#else
    return DCMI_OK;
#endif
}

static int dcmi_mcu_set_info_by_i2c(int card_id, MCU_SMBUS_REQ_MSG *mcu_req)
{
#ifndef _WIN32
    struct dcmi_smbus_std_req req = {0};
    int ret;
    int fd = 0;
    int send_msg_data_max_len = dcmi_mcu_get_send_data_max_len();
    (void)card_id;
    unsigned int i2c_num = dcmi_board_chip_type_is_ascend_310b() ? I2C_NUM_6 : I2C_NUM_9;

    /* 发送请求报文 */
    req.lun = mcu_req->lun;
    req.opcode = mcu_req->opcode;
    req.offset = mcu_req->offset;
    req.length = mcu_req->lenth;

    ret = memcpy_s(&req.data[0], send_msg_data_max_len, mcu_req->req_data, mcu_req->lenth);
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed. ret is %d.", ret);
    }

    ret = dcmi_mcu_set_lock(&fd, DCMI_MCU_GET_LOCK_TIMOUT);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call mcu_get_lock failed. ret is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dcmi_smbus_write_block(
        i2c_num, MCU_SLAVE_ADDR, SEND_REQUEST, DMP_MSG_HEAD_LENGTH + mcu_req->lenth, (const unsigned char *)&req);
    if (ret != DCMI_OK) {
        dcmi_mcu_set_unlock(fd);
        gplog(LOG_INFO, "call i2c_smbus_write_block failed. ret is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    if (dcmi_board_chip_type_is_ascend_310b()) {
        struct dcmi_smbus_std_rsp rsp = {0};
        /* 发送应答报文 */
        ret = dcmi_smbus_read_block(
            i2c_num, MCU_SLAVE_ADDR, SEND_RESPONSE, DMP_MSG_HEAD_LENGTH + 1, (unsigned char *)&rsp);
        if (ret != DCMI_OK) {
            dcmi_mcu_set_unlock(fd);
            gplog(LOG_INFO, "call i2c_smbus_read_block failed. ret is %d.", ret);
            return DCMI_ERR_CODE_INNER_ERR;
        }

        if (rsp.error_code != DCMI_OK) {
            dcmi_mcu_set_unlock(fd);
            gplog(LOG_ERR, "error_code=%hu.", rsp.error_code);
            return DCMI_ERR_CODE_INNER_ERR;
        }
    }

    dcmi_mcu_set_unlock(fd);
    return DCMI_OK;
#else
    return DCMI_OK;
#endif
}

int dcmi_mcu_get_info_fix(int card_id, MCU_SMBUS_REQ_MSG *mcu_req, MCU_SMBUS_RSP_MSG *mcu_rsp)
{
    mcu_req->req_data = NULL;
    mcu_req->offset = 0;
    mcu_rsp->total_len = 0;

    return dcmi_mcu_get_info(card_id, mcu_req, mcu_rsp);
}

/* 获取固定长度，长度小于或等于4个字节 */
int dcmi_mcu_get_fix_word(int card_id, unsigned char arg,
    unsigned short opcode, unsigned int get_lenth, int *data_info)
{
    int ret;
    char data_curr[64] = {0};
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    MCU_SMBUS_RSP_MSG mcu_rsp = {0};
    mcu_req.arg = arg;
    mcu_req.opcode = opcode;
    mcu_req.lenth = get_lenth;
    mcu_rsp.len = 0;
    mcu_rsp.data_info = data_curr;

    ret = dcmi_mcu_get_info_fix(card_id, &mcu_req, &mcu_rsp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_get_info_fix failed. ret is %d.", ret);
        return ret;
    }

    if (mcu_rsp.len > (int)sizeof(int)) {
        gplog(LOG_ERR, "len(%d) error.", mcu_rsp.len);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    *data_info = *(int *)(void *)&data_curr[0];

    return DCMI_OK;
}

static void mcu_get_info_dynamic_data_handle(MCU_SMBUS_RSP_MSG *mcu_rsp, const char *buffer, int buf_len, int copy_len)
{
    int ret;

    (void)buf_len;
    ret = memcpy_s(mcu_rsp->data_info, mcu_rsp->total_len, buffer, copy_len);
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed.%d.", ret);
    }

    mcu_rsp->len = copy_len;
}

/* 获取变长数据, 超过就出错, 最大支持256 */
int dcmi_mcu_get_info_dynamic(int card_id, MCU_SMBUS_REQ_MSG *mcu_req, MCU_SMBUS_RSP_MSG *mcu_rsp)
{
    int ret, len_total_true, count, num_id;
    char buffer[DCMI_MCU_MSG_MAX_TOTAL_LEN] = {0};
    MCU_SMBUS_RSP_MSG mcu_rsp_tmp = {0};
    mcu_rsp_tmp.data_info = buffer;

    if (mcu_req == NULL || mcu_rsp == NULL) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    mcu_req->offset = 0;
    mcu_req->lenth = (unsigned int)dcmi_mcu_get_recv_data_max_len();

    ret = dcmi_mcu_get_info(card_id, mcu_req, &mcu_rsp_tmp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_get_info fail.ret is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    if (mcu_rsp_tmp.total_len > DCMI_MCU_MSG_MAX_TOTAL_LEN || mcu_rsp_tmp.total_len < 0) {
        gplog(LOG_ERR, "data total len = %d is invalid.", mcu_rsp_tmp.total_len);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    len_total_true = mcu_rsp_tmp.total_len;
    if (mcu_rsp_tmp.total_len <= mcu_rsp_tmp.len) {
        mcu_get_info_dynamic_data_handle(mcu_rsp, buffer, DCMI_MCU_MSG_MAX_TOTAL_LEN, len_total_true);
        return DCMI_OK;
    }

    mcu_rsp_tmp.total_len -= mcu_rsp_tmp.len;
    mcu_req->offset = (unsigned int)mcu_rsp_tmp.len;
    mcu_rsp_tmp.data_info += mcu_rsp_tmp.len;

    count = (mcu_rsp_tmp.total_len % (int)mcu_req->lenth != 0) ?
            ((mcu_rsp_tmp.total_len + (int)mcu_req->lenth) / (int)mcu_req->lenth) :
            ((mcu_rsp_tmp.total_len) / (int)mcu_req->lenth);

    for (num_id = 0; num_id < count; num_id++) {
        mcu_rsp_tmp.len = (mcu_req->offset + mcu_req->lenth > (unsigned int)mcu_rsp_tmp.total_len) ? \
        (mcu_rsp_tmp.total_len - (int)mcu_req->offset) : (int)mcu_req->lenth;
        ret = dcmi_mcu_get_info(card_id, mcu_req, &mcu_rsp_tmp);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_mcu_get_info fail.ret is %d.", ret);
            return DCMI_ERR_CODE_INNER_ERR;
        }

        mcu_req->offset += (unsigned int)mcu_rsp_tmp.len;
        mcu_rsp_tmp.data_info += mcu_rsp_tmp.len;
    }

    mcu_get_info_dynamic_data_handle(mcu_rsp, buffer, DCMI_MCU_MSG_MAX_TOTAL_LEN, len_total_true);

    return DCMI_OK;
}

int dcmi_mcu_set_info_dynamic(int card_id, unsigned short opcode, int len, char *data_info)
{
    int ret, num_id, block_num;
    int send_msg_data_max_len = dcmi_mcu_get_send_data_max_len();
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    mcu_req.opcode = opcode;
    mcu_req.offset = 0;
    mcu_req.req_data = data_info;

    if (send_msg_data_max_len == 0) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    block_num = len / send_msg_data_max_len;
    if (len % send_msg_data_max_len != 0) {
        block_num++;
    }

    for (num_id = 0; num_id < block_num; num_id++) {
        if ((int)mcu_req.offset + send_msg_data_max_len >= len) {
            mcu_req.lenth = (unsigned int)len - mcu_req.offset;
            mcu_req.lun = DCMI_MCU_MSG_LUN;
        } else {
            mcu_req.lenth = (unsigned int)send_msg_data_max_len;
            mcu_req.lun = 0x0;
        }

        ret = dcmi_mcu_set_info(card_id, &mcu_req);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_mcu_set_info fail.ret(%d)", ret);
            return DCMI_ERR_CODE_INNER_ERR;
        }

        mcu_req.offset += mcu_req.lenth;
        mcu_req.req_data += mcu_req.lenth;
    }

    return DCMI_OK;
}

int dcmi_mcu_set_info_simple(int card_id, unsigned short opcode, unsigned int len, char *data_info)
{
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    mcu_req.lun = DCMI_MCU_MSG_LUN;
    mcu_req.opcode = opcode;
    mcu_req.offset = 0;
    mcu_req.lenth = len;
    mcu_req.req_data = data_info;
    return dcmi_mcu_set_info(card_id, &mcu_req);
}

STATIC int dcmi_passthru_mcu(int device_logic_id, struct passthru_message_stru *passthru_message, int opcode)
{
    int i, err;
    struct dmp_rsp_message_stru *ms_rsp = NULL;

    for (i = 0; i < DCMI_MCU_MSG_REPEATE_NUM; i++) {
        err = dsmi_passthru_mcu(device_logic_id, passthru_message);
        if ((err == DSMI_ERR_OPER_NOT_PERMITTED)) {
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        } else if (err == DSMI_OK) {
            ms_rsp = &(passthru_message->dest_message.data.rsp);
            // 增加opcode 返回值校验
            if (ms_rsp->opcode != opcode) {
                gplog(LOG_INFO, "req_opcode = 0x%x, ms_rsp->opcode = 0x%x, rsp_errorcode=%d.",
                    opcode, ms_rsp->opcode, ms_rsp->errorcode);
                usleep((i > DCMI_MCU_MSG_REPEATE_NUM_1MS) ? DCMI_MCU_TASK_DELAY_500_MS : DCMI_MCU_TASK_DELAY_1_MS);
                continue;
            }
            // 电子标签数据为空时，为了让BMC感知，MCU会返回错误码5，此为正常情况，返回ok
            if ((ms_rsp->errorcode == 0) || (ms_rsp->errorcode == DCMI_MCU_DATA_EMPTY)) {
                return DCMI_OK;
            }

            // 旧版本MCU不支持查询mcu typeid，会返回错误码1，为正常情况，此处返回ok
            if ((ms_rsp->errorcode == DCMI_MCU_NO_SUPPORT) && (opcode == DCMI_MCU_MCU_TYPE_ID_OPCODE)) {
                return DCMI_OK;
            }
        }
        usleep((i > DCMI_MCU_MSG_REPEATE_NUM_1MS) ? DCMI_MCU_TASK_DELAY_500_MS : DCMI_MCU_TASK_DELAY_1_MS);
    }

    if (err == DSMI_OK) {
        ms_rsp = &(passthru_message->dest_message.data.rsp);
        gplog(LOG_ERR, "opcode = 0x%x, ms_rsp->opcode = 0x%x, errorcode=%d.",
            opcode, ms_rsp->opcode, ms_rsp->errorcode);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    return dcmi_convert_error_code(err);
}

int dcmi_mcu_get_send_data_max_len(void)
{
    bool support_chip_type = (dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_910b() ||
        dcmi_board_chip_type_is_ascend_910() || dcmi_board_chip_type_is_ascend_910_93());
    if (support_chip_type) {
        return DCMI_MCU_SEND_MSG_DATA_LEN_FOR_310P_AND_910;
    }

    return DCMI_MCU_SEND_MSG_DATA_LEN_FOR_DEFAULT;
}

int dcmi_mcu_get_recv_data_max_len(void)
{
    bool support_chip_type = (dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_910b() ||
        dcmi_board_chip_type_is_ascend_910() || dcmi_board_chip_type_is_ascend_310b() ||
        dcmi_board_chip_type_is_ascend_910_93());
    if (support_chip_type) {
        return DCMI_MCU_RECV_MSG_DATA_LEN_FOR_310P_AND_910;
    }

    return DCMI_MCU_RECV_MSG_DATA_LEN_FOR_DEFAULT;
}

STATIC int dcmi_get_mini2mcu_heartbeat_status(int card_id, int device_id, unsigned char *status, unsigned int *discnt)
{
    int err;
    int device_logic_id = 0;

    err = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", err);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    err = dsmi_get_mini2mcu_heartbeat_status(device_logic_id, status, discnt);
    return dcmi_convert_error_code(err);
}

STATIC int dcmi_get_slotid_connnet_with_mcu(int card_id, unsigned char *status,
    unsigned int *dis_cnt, unsigned int *cnt_index)
{
    /* 都通:以心跳计数小的为主；其中一个通，则选定该通道；都不通，返回错误
       dsmi_get_mini2mcu_heartbeat_status用的是芯片在卡上的槽位号
       目前的版本逻辑位号->物理位号:  0->0;1->2;2->1;3->3
       硬件MCU与miniD连接 的iic为1or3 */
    if ((status[MAIN_CHANNEL_MINI2MCU] == MINI_MCU_DISCONNECT) &&
        (status[SPARE_CHANNEL_MINI2MCU] == MINI_MCU_DISCONNECT)) {
        gplog(LOG_ERR, "Card %d can not connect with MCU.", card_id);
        return DCMI_ERR_CODE_INNER_ERR;
    } else if ((status[MAIN_CHANNEL_MINI2MCU] == MINI_MCU_CONNECT) &&
               (status[SPARE_CHANNEL_MINI2MCU] == MINI_MCU_DISCONNECT)) {
        *cnt_index = MAIN_CHANNEL_MINI2MCU;
    } else if ((status[MAIN_CHANNEL_MINI2MCU] == MINI_MCU_DISCONNECT) &&
               (status[SPARE_CHANNEL_MINI2MCU] == MINI_MCU_CONNECT)) {
        *cnt_index = SPARE_CHANNEL_MINI2MCU;
    } else if ((status[MAIN_CHANNEL_MINI2MCU] == MINI_MCU_CONNECT) &&
               (status[SPARE_CHANNEL_MINI2MCU] == MINI_MCU_CONNECT)) {
        *cnt_index = (unsigned int)((dis_cnt[MAIN_CHANNEL_MINI2MCU] >= dis_cnt[SPARE_CHANNEL_MINI2MCU]) ?
            SPARE_CHANNEL_MINI2MCU : MAIN_CHANNEL_MINI2MCU);
    } else {
        gplog(LOG_ERR, "dcmi get mini 2 mcu heartbeat status error, status is %u.", status[MAIN_CHANNEL_MINI2MCU]);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    return DCMI_OK;
}

/* The validity of the parameter is guaranteed by the caller, and the internal function is not judged */
int dcmi_get_mcu_connect_device_logic_id(int *device_logic_id, int *device_slot_id, int card_id)
{
    int err, slot_id, chip_index;
    struct dcmi_card_info *card_info = NULL;
    unsigned char status[MAX_DEVICE_NUM_IN_CARD] = {0};  // 芯片的当前心跳状态
    unsigned int dis_cnt[MAX_DEVICE_NUM_IN_CARD] = {0};  // 当前心跳失联计数
    int logic_id[MAX_DEVICE_NUM_IN_CARD] = {0};          // 当前卡上芯片的逻辑位号
    unsigned char status_tmp = 0;
    unsigned int dis_cnt_tmp = 0;
    unsigned int cnt_index = 0;                  // 当前与mcu相通的miniD槽位号
    int chip_slot[MAX_DEVICE_NUM_IN_CARD] = {0};  // 芯片在卡上的slot编号

    /* 获取卡的信息 */
    err = dcmi_get_card_info(card_id, &card_info);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_card_info failed. err is %d.", err);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    /* 分别获取卡上每个芯片的通信状态 */
    /* 并将slot id 为1 或3的芯片通信状态记录下来 */
    for (chip_index = 0; chip_index < card_info->device_count; chip_index++) {
        slot_id = card_info->device_info[chip_index].chip_slot;  // 找到芯片在卡上的slot 编号

        if ((slot_id != MAIN_CHANNEL_MINI2MCU) && (slot_id != SPARE_CHANNEL_MINI2MCU)) {
            continue;
        }
        err = dcmi_get_mini2mcu_heartbeat_status(card_id, chip_index, &status_tmp, &dis_cnt_tmp);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_mini2mcu_heartbeat_status failed %d. chip slot is %d.", err, slot_id);
        }

        status[slot_id] = status_tmp;
        dis_cnt[slot_id] = dis_cnt_tmp;
        logic_id[slot_id] = card_info->device_info[chip_index].logic_id;
        chip_slot[slot_id] = slot_id;
    }

    err = dcmi_get_slotid_connnet_with_mcu(card_id, status, dis_cnt, &cnt_index);
    if (err != DCMI_OK) {
        return err;
    }

    *device_logic_id = logic_id[cnt_index];
    *device_slot_id = chip_slot[cnt_index];
    return DCMI_OK;
}

STATIC int dcmi_mcu_get_info_by_npu(int card_id, MCU_SMBUS_REQ_MSG *mcu_req, MCU_SMBUS_RSP_MSG *mcu_rsp)
{
    int err;
    int device_logic_id, device_slot_id;
    struct dmp_req_message_stru *ms_req = NULL;
    struct dmp_rsp_message_stru *ms_rsp = NULL;
    struct passthru_message_stru passthru_message = {0};
    int send_msg_data_max_len = dcmi_mcu_get_send_data_max_len();
    int recv_msg_data_max_len = dcmi_mcu_get_recv_data_max_len();
    bool support_chip_type = (dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_910() ||
                              dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93());
    if (support_chip_type) {
        err = dcmi_get_device_logic_id(&device_logic_id, card_id, 0);
    } else {
        err = dcmi_get_mcu_connect_device_logic_id(&device_logic_id, &device_slot_id, card_id);
    }

    if (err != DCMI_OK) {
        gplog(LOG_ERR, "get mcu connect device  logic_id failed. err is %d.", err);
        return err;
    }

    ms_req = &passthru_message.src_message.data.req;
    ms_rsp = &passthru_message.dest_message.data.rsp;

    ms_req->opcode = mcu_req->opcode;
    ms_req->offset = mcu_req->offset;
    ms_req->length = mcu_req->lenth;
    ms_req->arg = mcu_req->arg;
    ms_req->lun = DCMI_MCU_MSG_LUN;

    mcu_req->req_data_len = (mcu_req->req_data_len > send_msg_data_max_len) ?
                            send_msg_data_max_len : mcu_req->req_data_len;

    if (mcu_req->req_data_len != 0) {
        err = memcpy_s(&ms_req->data[0], send_msg_data_max_len, mcu_req->req_data, mcu_req->req_data_len);
        if (err != EOK) {
            gplog(LOG_ERR, "call memcpy_s failed. err is %d.", err);
        }
    }

    passthru_message.src_len = (unsigned int)(DMP_MSG_HEAD_LENGTH + mcu_req->req_data_len);
    passthru_message.rw_flag = 0;  // 0 read 1 write

    err = dcmi_passthru_mcu(device_logic_id, &passthru_message, mcu_req->opcode);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_passthru_mcu failed. err is %d.", err);
        return err;
    }

    err = memcpy_s(mcu_rsp->data_info, recv_msg_data_max_len, &ms_rsp->data[0], ms_rsp->length);
    if (err != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed. err is %d.", err);
    }

    mcu_rsp->len = (int)ms_rsp->length;
    mcu_rsp->total_len = (int)ms_rsp->total_length;
    mcu_rsp->errorcode = ms_rsp->errorcode;

    return DCMI_OK;
}


STATIC int dcmi_mcu_set_info_by_npu(int card_id, MCU_SMBUS_REQ_MSG *mcu_req)
{
    int err;
    int device_logic_id = 0;
    int device_slot_id = 0;
    struct dmp_req_message_stru *ms_req = NULL;
    struct passthru_message_stru passthru_message = {0};
    int send_msg_data_max_len = dcmi_mcu_get_send_data_max_len();

    bool support_chip_type = (dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_910b() ||
        dcmi_board_chip_type_is_ascend_910() || dcmi_board_chip_type_is_ascend_910_93());
    if (support_chip_type) {
        err = dcmi_get_device_logic_id(&device_logic_id, card_id, 0);
    } else {
        err = dcmi_get_mcu_connect_device_logic_id(&device_logic_id, &device_slot_id, card_id);
    }

    if (err != DCMI_OK) {
        gplog(LOG_ERR, "get mcu connect device logic_id failed. err is %d.", err);
        return err;
    }

    ms_req = &passthru_message.src_message.data.req;

    ms_req->lun = mcu_req->lun;
    ms_req->opcode = mcu_req->opcode;
    ms_req->offset = mcu_req->offset;
    ms_req->length = mcu_req->lenth;

    err = memcpy_s(&ms_req->data[0], send_msg_data_max_len, mcu_req->req_data, mcu_req->lenth);
    if (err != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed. err is %d.", err);
    }

    passthru_message.src_len = DMP_MSG_HEAD_LENGTH + mcu_req->lenth;
    if (support_chip_type) {
        passthru_message.rw_flag = 0;           /* 310p、910、910b标卡设置类消息有响应需要回读 */
    } else {
        passthru_message.rw_flag = 1;
    }

    err = dcmi_passthru_mcu(device_logic_id, &passthru_message, mcu_req->opcode);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dsmi_passthru_mcu failed. err is %d.", err);
        return err;
    }

    return DCMI_OK;
}

STATIC int dcmi_mcu_get_info_inn(int card_id, MCU_SMBUS_REQ_MSG *mcu_req, MCU_SMBUS_RSP_MSG *mcu_rsp)
{
    int ret;
    int num_id;

    if (dcmi_get_mcu_access_chan() == DCMI_MCU_ACCESS_BY_NPU) {
        if (dcmi_check_vnpu_in_docker_or_virtual(card_id)) {
            return DCMI_OK;
        }
        return dcmi_mcu_get_info_by_npu(card_id, mcu_req, mcu_rsp);
    }

    for (num_id = 0; num_id < DCMI_MCU_MSG_REPEATE_NUM; num_id++) {
        ret = dcmi_mcu_get_info_by_i2c(card_id, mcu_req, mcu_rsp);
        if (ret == DCMI_OK) {
            break;
        }
        usleep(DCMI_MCU_TASK_DELAY_500_MS);
    }
    return ret;
}

int dcmi_mcu_get_info(int card_id, MCU_SMBUS_REQ_MSG *mcu_req, MCU_SMBUS_RSP_MSG *mcu_rsp)
{
    int ret;

    ret = dcmi_mcu_get_info_inn(card_id, mcu_req, mcu_rsp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_get_info_inn failed  err is %d.", ret);
    }
    return ret;
}

STATIC int dcmi_mcu_set_info_inn(int card_id, MCU_SMBUS_REQ_MSG *mcu_req)
{
    int ret;
    int num_id;

    if (dcmi_get_mcu_access_chan() == DCMI_MCU_ACCESS_BY_NPU) {
        return dcmi_mcu_set_info_by_npu(card_id, mcu_req);
    }

    for (num_id = 0; num_id < DCMI_MCU_MSG_REPEATE_NUM; num_id++) {
        ret =  dcmi_mcu_set_info_by_i2c(card_id, mcu_req);
        if (ret == DCMI_OK) {
            break;
        }
        usleep(DCMI_MCU_TASK_DELAY_500_MS);
    }
    return ret;
}

/* 失败就重复试两次，这个为对外接口 */
int dcmi_mcu_set_info(int card_id, MCU_SMBUS_REQ_MSG *mcu_req)
{
    int ret;
    int num_id;

    for (num_id = 0; num_id < DCMI_MCU_MSG_REPEATE_NUM; num_id++) {
        ret = dcmi_mcu_set_info_inn(card_id, mcu_req);
        if (ret == DCMI_OK) {
            break;
        }
        usleep(DCMI_MCU_TASK_DELAY_500_MS);
    }

    return ret;
}

int dcmi_mcu_version_parse(char *version, int len, MCU_SMBUS_RSP_MSG *mcu_rsp)
{
    int num_id, ret;
    int len_pre = 0;

    for (num_id = 0; num_id < mcu_rsp->len - 1; num_id++) {
        if (len - len_pre <= 0) {
            gplog(LOG_ERR, "mcu_ver_str buffer not enough,max_version_len is %d len_pre is %d.", len, len_pre);
            return DCMI_ERR_CODE_INNER_ERR;
        }

        ret = sprintf_s(version + len_pre, len - len_pre - 1, "%d.", mcu_rsp->data_info[num_id]);
        if (ret <= 0) {
            gplog(LOG_ERR, "call sprintf_s fail.ret is %d.", ret);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
        len_pre += ret;
    }
    if (len - len_pre <= 0) {
        gplog(LOG_ERR, "mcu_ver_str buffer not enough,max_version_len is %d len_pre is %d.", len, len_pre);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = sprintf_s(version + len_pre, len - len_pre - 1, "%d", mcu_rsp->data_info[num_id]);
    if (ret <= 0) {
        gplog(LOG_ERR, "call sprintf_s fail.ret is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    return DCMI_OK;
}

int dcmi_mcu_get_board_id(int card_id, unsigned int *board_id)
{
    if (board_id == NULL) {
        gplog(LOG_ERR, "dcmi_mcu_get_board_id input para error.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    return dcmi_mcu_get_fix_word(card_id, 0, DCMI_MCU_BOARD_ID_OPCODE, DCMI_MCU_BOARD_ID_LEN, (int *)(void *)board_id);
}

static int dcmi_mcu_get_pcb_id(int card_id, int *pcb_id)
{
    if (pcb_id == NULL) {
        gplog(LOG_ERR, "dcmi_mcu_get_pcb_id input para error.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    return dcmi_mcu_get_fix_word(card_id, 0, DCMI_MCU_PCB_ID_OPCODE, DCMI_MCU_PCB_ID_LEN, pcb_id);
}

int dcmi_mcu_get_bom_id(int card_id, int *bom_id)
{
    if (bom_id == NULL) {
        gplog(LOG_ERR, "dcmi_mcu_get_bom_id input para error.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    return dcmi_mcu_get_fix_word(card_id, 0, DCMI_MCU_BOM_ID_OPCODE, DCMI_MCU_BOM_ID_LEN, bom_id);
}

int dcmi_mcu_get_mcu_type_id(int card_id, unsigned int *mcu_type)
{
    int ret;
    unsigned char data_info[DCMI_MCU_RECV_MSG_DATA_LEN_FOR_DEFAULT] = {0};
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    MCU_SMBUS_RSP_MSG mcu_rsp = {0};
    mcu_req.opcode = DCMI_MCU_MCU_TYPE_ID_OPCODE;
    mcu_req.lenth = DCMI_MCU_MCU_TYPE_ID_LEN;
    mcu_rsp.data_info = (char *)data_info;

    ret = dcmi_mcu_get_info(card_id, &mcu_req, &mcu_rsp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_get_info failed. ret is %d.", ret);
        return ret;
    }

    if (mcu_rsp.errorcode == DCMI_MCU_NO_SUPPORT) {
        gplog(LOG_ERR, "mcu not support query type_id.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    *mcu_type = (data_info[1] << 8) + (data_info[0]);       // 原始数据左移8位，获取无符号整型数据第0位

    return DCMI_OK;
}

int dcmi_mcu_get_first_power_on_date(int card_id, unsigned int *first_power_on_date)
{
    int ret;
    char req_data = 0x1;                                // 请求参数1表示获取首次上电时间
    unsigned char data_info[DCMI_MCU_RECV_MSG_DATA_LEN_FOR_DEFAULT] = {0};
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    MCU_SMBUS_RSP_MSG mcu_rsp = {0};
    mcu_req.arg = 0;
    mcu_req.opcode = DCMI_MCU_DFLC_DATA_OPCODE;
    mcu_req.offset = 0;
    mcu_req.lenth = DCMI_FIRST_POWER_ON_DATE_LEN;
    mcu_req.req_data_len = 1;
    mcu_req.req_data = &req_data;
    mcu_rsp.data_info = (char *)data_info;

    if (first_power_on_date == NULL) {
        gplog(LOG_ERR, "dcmi_mcu_get_first_power_on_date input para error.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_mcu_get_info(card_id, &mcu_req, &mcu_rsp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_get_info failed %d.\n", ret);
        return ret;
    }

    // 将4字节小字节序数据拼成无符号整型数据
    *first_power_on_date = (data_info[3] & 0xff);                               // 获取无符号整型数据第3位最高位
    *first_power_on_date = (*first_power_on_date << 8) + (data_info[2] & 0xff); // 原始数据左移8位，获取无符号整型数据第2位
    *first_power_on_date = (*first_power_on_date << 8) + (data_info[1] & 0xff); // 原始数据左移8位，获取无符号整型数据第1位
    *first_power_on_date = (*first_power_on_date << 8) + (data_info[0] & 0xff); // 原始数据左移8位，获取无符号整型数据第0位

    return DCMI_OK;
}

int dcmi_mcu_get_temperature(int card_id, int *temper)
{
    int ret;
    int value = 0;

    if (temper == NULL) {
        gplog(LOG_ERR, "dcmi_mcu_get_temperature input para error.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    ret = dcmi_mcu_get_fix_word(card_id, 0, DCMI_MCU_TEMPERATURE_OPCODE, DCMI_MCU_TEMPERATURE_LEN, &value);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_INNER_ERR;
    }
    // 实际有效值为2个字节，需要进行强制转换，否则在温度为负值时，结果会变成很大的值。
    *temper = (short)value;
    return DCMI_OK;
}

int dcmi_mcu_get_voltage(int card_id, unsigned int *voltage)
{
    if (voltage == NULL) {
        gplog(LOG_ERR, "dcmi_mcu_get_voltage input para error.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    return dcmi_mcu_get_fix_word(card_id, 0, DCMI_MCU_VOLTAGE_OPCODE, DCMI_MCU_VOLTAGE_LEN, (int *)(void *)voltage);
}

int dcmi_mcu_get_health(int card_id, unsigned int *health)
{
    return dcmi_mcu_get_fix_word(card_id, 0, DCMI_MCU_HEALTH_OPCODE, DCMI_MCU_HEALTH_LEN, (int *)(void *)health);
}

int dcmi_mcu_get_device_errorcode(
    int card_id, int *error_count, unsigned int *error_code_list, unsigned int list_len)
{
    int ret, i;
    int req_data = 0;
    unsigned short mcu_err_code_list[DCMI_MCU_ERR_CODE_MAX] = {0};
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    MCU_SMBUS_RSP_MSG mcu_rsp = {0};
    mcu_req.arg = 0;
    mcu_req.opcode = DCMI_MCU_ERRCODE_OPCODE;
    mcu_req.req_data = (char *)&req_data;
    mcu_req.req_data_len = 0;
    mcu_rsp.data_info = (char *)&mcu_err_code_list[0];
    mcu_rsp.total_len = (int)sizeof(mcu_err_code_list);
    mcu_rsp.len = 0;

    ret = dcmi_mcu_get_info_dynamic(card_id, &mcu_req, &mcu_rsp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_get_info_dynamic fail %d.", ret);
        return ret;
    }

    /* PCIE带外管理规范，若无故障，则填0x0000，小字节序 */
    if ((mcu_rsp.len == (int)sizeof(unsigned short)) && (mcu_err_code_list[0] == 0)) {
        *error_count = 0;
    } else {
        *error_count = mcu_rsp.len / (int)sizeof(unsigned short);  // MCU返回的错误码为unsigned short类型，位宽为2
        if (*error_count > list_len) {
            gplog(LOG_ERR, "*error_count %d is bigger than list_len %u", *error_count, list_len);
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }
        for (i = 0; i < *error_count; i++) {
            error_code_list[i] = mcu_err_code_list[i];
        }
    }
    return DCMI_OK;
}

static int dcmi_mcu_get_error_string_inner(unsigned char *error_info, int buff_size, const char *src_info, int size)
{
    int ret;
    ret = memcpy_s(error_info, buff_size, src_info, DCMI_MCU_ERROR_STRING_LENTH);
    if (ret != EOK) {
        gplog(LOG_ERR, "memcpy_s failed, err is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    return DCMI_OK;
}

static bool is_310p_1p_card(int board_id)
{
    if (board_id == DCMI_310P_1P_CARD_BOARD_ID || board_id == DCMI_310P_1P_CARD_BOARD_ID_V2 ||
        board_id == DCMI_310P_1P_CARD_BOARD_ID_V3) {
        return true;
    }
    return false;
}

bool is_zhuque_board_id(int board_id)
{
    if (board_id == DCMI_A800I_POD_A2_BIN2_BOARD_ID || board_id == DCMI_A800I_POD_A2_BIN2_1_BOARD_ID ||
        board_id == DCMI_A800T_POD_A2_BIN1_BOARD_ID || board_id == DCMI_A800T_POD_A2_BIN0_BOARD_ID) {
        return true;
    }
    return false;
}

static bool is_910b_pod(int board_id)
{
    if (board_id == DCMI_A900T_POD_A1_BIN3_BOARD_ID || board_id == DCMI_A900T_POD_A1_BIN0_BOARD_ID ||
        board_id == DCMI_A900T_POD_A1_BIN1_BOARD_ID || board_id == DCMI_A900T_POD_A1_BIN2_BOARD_ID ||
        board_id == DCMI_A900T_POD_A1_BIN2X_BOARD_ID || board_id == DCMI_A900T_POD_A1_BIN2X_1_BOARD_ID ||
        board_id == DCMI_A800I_POD_A2_BIN4_1_PCIE_BOARD_ID ||
        board_id == DCMI_A900T_POD_A1_BIN3_P3_BOARD_ID || board_id == DCMI_A900T_POD_A1_BIN0_P3_BOARD_ID ||
        board_id == DCMI_A900T_POD_A1_BIN1_P3_BOARD_ID || board_id == DCMI_A900T_POD_A1_BIN2_P3_BOARD_ID ||
        board_id == DCMI_A900T_POD_A1_BIN2X_P3_BOARD_ID || board_id ==DCMI_A900T_POD_A1_BIN2X_1_P3_BOARD_ID ||
        is_zhuque_board_id(board_id) == true) {
        return true;
    }
    return false;
}

static bool is_910b_box(int board_id)
{
    if (board_id == DCMI_A200T_BOX_A1_BIN3_BOARD_ID || board_id == DCMI_A200T_BOX_A1_BIN0_BOARD_ID ||
        board_id == DCMI_A200T_BOX_A1_BIN2_BOARD_ID || board_id == DCMI_A200T_BOX_A1_BIN1_BOARD_ID) {
        return true;
    }
    return false;
}

int dcmi_mcu_get_device_errorcode_string(int card_id, int error_code, unsigned char *error_info, int buff_size)
{
    size_t i, table_size;
    int chip_type, board_id;
    struct dcmi_health_info *health_error_info = NULL;

    if (error_info == NULL || buff_size <= 0) {
        gplog(LOG_ERR, "error_info is NULL or buff_size %d.", buff_size);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    chip_type = dcmi_get_board_chip_type();
    board_id = dcmi_get_board_id_inner();
    switch (chip_type) {
        case DCMI_CHIP_TYPE_D310:
            return DCMI_ERR_CODE_NOT_SUPPORT;
        case DCMI_CHIP_TYPE_D310P:
            if (is_310p_1p_card(board_id)) {
                table_size = (sizeof(g_310p_1p_card_health_error_info) / sizeof(g_310p_1p_card_health_error_info[0]));
                health_error_info = g_310p_1p_card_health_error_info;
            } else {
                table_size = (sizeof(g_310p_2p_card_health_error_info) / sizeof(g_310p_2p_card_health_error_info[0]));
                health_error_info = g_310p_2p_card_health_error_info;
            }
            break;
        case DCMI_CHIP_TYPE_D910:
            table_size = (sizeof(g_910_card_health_error_info) / sizeof(g_910_card_health_error_info[0]));
            health_error_info = g_910_card_health_error_info;
            break;
        case DCMI_CHIP_TYPE_D910B:
            if (is_910b_pod(board_id)) {
                table_size = (sizeof(g_910b_pod_health_error_info) / sizeof(g_910b_pod_health_error_info[0]));
                health_error_info = g_910b_pod_health_error_info;
            } else if (is_910b_box(board_id)) {
                table_size = (sizeof(g_910b_box_health_error_info) / sizeof(g_910b_box_health_error_info[0]));
                health_error_info = g_910b_box_health_error_info;
            } else {
                table_size = (sizeof(g_910b_card_health_error_info) / sizeof(g_910b_card_health_error_info[0]));
                health_error_info = g_910b_card_health_error_info;
            }
            break;
        default:
            return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    for (i = 0; i < table_size; i++) {
        if (error_code == health_error_info[i].error_code) {
            return dcmi_mcu_get_error_string_inner(error_info, buff_size,
                &health_error_info[i].error_info[0], DCMI_MCU_ERROR_STRING_LENTH);
        }
    }

    return DCMI_ERR_CODE_INNER_ERR;
}

int dcmi_mcu_get_board_info(int card_id, struct dcmi_board_info *board_info)
{
    int ret;

    if (board_info == NULL) {
        gplog(LOG_ERR, "board_info pointer is null.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_check_card_id(card_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, ret);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_is_has_mcu()) {
        board_info->bom_id = 0;
        board_info->board_id = 0;
        board_info->pcb_id = 0;

        ret = dcmi_mcu_get_board_id(card_id, &board_info->board_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "get mcu board id fail.ret is %d", ret);
            return ret;
        }

        ret = dcmi_mcu_get_pcb_id(card_id, (int *)(void *)&board_info->pcb_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "get mcu pcb id fail.ret is %d", ret);
            return ret;
        }

        ret = dcmi_mcu_get_bom_id(card_id, (int *)(void *)&board_info->bom_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "get mcu bom id fail.ret is %d", ret);
            return ret;
        }
        return ret;
    }
    return DCMI_ERR_CODE_NOT_SUPPORT;
}

static int dcmi_get_mcu_log_name(int card_id, int log_type, char *log_file_name, int log_file_name_size)
{
    struct tm tm_time;
    time_t now_time;
    int ret;
    const int year_base = 1900;
    const int month_base = 1;

    (void)time(&now_time);
    (void)localtime_r(&now_time, &tm_time);
    ret = sprintf_s(log_file_name, log_file_name_size, "%s%s_%d_%04d%02d%02d%02d%02d%02d.log",
        MCU_LOG_PATH, g_dcmi_mcu_log_info[log_type].file_name, card_id,
        tm_time.tm_year + year_base,
        tm_time.tm_mon + month_base,
        tm_time.tm_mday,
        tm_time.tm_hour,
        tm_time.tm_min,
        tm_time.tm_sec);
    if (ret <= 0) {
        gplog(LOG_ERR, "call sprintf_s failed. ret is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    return DCMI_OK;
}

static void mcu_smbus_req_msg_init(int arg, int opcode, int offset, int lenth, MCU_SMBUS_REQ_MSG *mcu_req)
{
    mcu_req->arg = (unsigned char)arg;
    mcu_req->opcode = (unsigned short)opcode;
    mcu_req->offset = (unsigned int)offset;
    mcu_req->lenth = (unsigned int)lenth;
    return;
}

int dcmi_mcu_get_log_length(int card_id, int log_type, unsigned int *log_info_len)
{
    int ret;
    int req_data = 0;
    char log_info[DCMI_MCU_SEND_MSG_DATA_LEN_FOR_DEFAULT] = {0};
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    MCU_SMBUS_RSP_MSG mcu_rsp = {0};
    mcu_req.req_data_len = 0;
    mcu_req.req_data = (char *)&req_data;
    mcu_rsp.data_info = log_info;

    mcu_smbus_req_msg_init(0, g_dcmi_mcu_log_info[log_type].opcode, 0, dcmi_mcu_get_recv_data_max_len(), &mcu_req);

    ret = dcmi_mcu_get_info(card_id, &mcu_req, &mcu_rsp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_get_info fail.ret is %d", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    *log_info_len = mcu_rsp.total_len;
    return DCMI_OK;
}

static int mcu_get_total_length(int card_id, int log_type, unsigned int *len_total, unsigned int max_length)
{
    int ret;

    ret = dcmi_mcu_get_log_length(card_id, log_type, len_total);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_INNER_ERR;
    }

    if (*len_total > max_length) {
        gplog(LOG_OP, "log_type is %d.len_total(%u) is invalid, max_length is %u. The log was truncated.",
            log_type, *len_total, max_length);
        *len_total = max_length;
    }
    return DCMI_OK;
}

int dcmi_mcu_get_log_info(int card_id, int log_type, char *log_info, int log_info_size, unsigned int *log_info_len)
{
    int fd, ret;
    unsigned int len_curr, len_total, count, num_id, max_length = (unsigned int)g_dcmi_mcu_log_info[log_type].max_lenth;
    int req_data = 0;
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    MCU_SMBUS_RSP_MSG mcu_rsp = {0};
    mcu_req.req_data_len = 0;
    mcu_req.req_data = (char *)&req_data;
    mcu_rsp.data_info = log_info;

    mcu_smbus_req_msg_init(0, g_dcmi_mcu_log_info[log_type].opcode, 0, dcmi_mcu_get_recv_data_max_len(), &mcu_req);

    ret = mcu_get_total_length(card_id, log_type, &len_total, max_length);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_INNER_ERR;
    }

    count = (mcu_req.lenth == 0) ? 0 : (len_total + mcu_req.lenth - 1) / mcu_req.lenth;

    ret = dcmi_mcu_set_lock_up(&fd, DCMI_MCU_GET_LOCK_TIMOUT);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_set_lock failed. ret is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    for (num_id = 0; num_id < count; num_id++) {
        if (dcmi_board_type_is_station() && (access("/run/upgrade_running", F_OK) == DCMI_OK)) {
            gplog(LOG_ERR, "upgrade running");
            dcmi_mcu_set_unlock_up(fd);
            return DCMI_ERR_CODE_IS_UPGRADING;
        }

        len_curr = (mcu_req.offset + mcu_req.lenth > len_total) ? (len_total - mcu_req.offset) : mcu_req.lenth;
        ret = dcmi_mcu_get_info(card_id, &mcu_req, &mcu_rsp);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_mcu_get_info fail.ret(%d) offset(%u) len_cur(%u)", ret, mcu_req.offset, len_curr);
            dcmi_mcu_set_unlock_up(fd);
            return DCMI_ERR_CODE_INNER_ERR;
        }

        mcu_req.offset += len_curr;
        mcu_rsp.data_info += len_curr;

        printf("\rtype(%d): file_len(%u)--offset(%u) [%u].", log_type, len_total, mcu_req.offset,
            ((mcu_req.offset * 100) / len_total));  // 显示百分比，100作为基数
        (void)fflush(stdout);

        if ((num_id % DCMI_MCU_GET_LOCK_MAX_TIME) == 0) {
            ret = dcmi_mcu_release_and_get_lock(&fd);
            if (ret != DCMI_OK) {
                return ret;
            }
        }
    }
    *log_info_len = len_total;
    dcmi_mcu_set_unlock_up(fd);
    return DCMI_OK;
}

#ifndef _WIN32
int write_buf_to_file(char *file_name, const char *buffer, unsigned int len)
{
#define PERMISSION_FILE 0600
    int fd = 0;
    int write_count, ret;
    char path[PATH_MAX + 1] = {0x00};

    ret = dcmi_file_realpath_allow_nonexist(file_name, path, sizeof(path));
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "file path is illegal errno(%d).\n", errno);
        return ret;
    }

    if ((fd = open(path, O_RDWR | O_CREAT | O_APPEND, PERMISSION_FILE)) < 0) {
        gplog(LOG_ERR, "open file failed errno(%d).", errno);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    write_count = write(fd, (char *)buffer, len);
    if (write_count != (int)len) {
        gplog(LOG_ERR, "call write failed.write_count is %d. log_len is %u.", write_count, len);
        (void)fchmod(fd, S_IRUSR);
        (void)fsync(fd);
        (void)close(fd);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }
    (void)fchmod(fd, S_IRUSR);
    (void)fsync(fd);
    (void)close(fd);
    return DCMI_OK;
}
#else
int write_buf_to_file(char *file_name, const char *buffer, unsigned int len)
{
    FILE *fp = NULL;
    size_t write_count;
    char path[MAX_PATH + 1] = {0x00};

    int ret = dcmi_file_realpath_allow_nonexist(file_name, path, sizeof(path));
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "file path is illegal errno(%d).\n", errno);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    if ((fp = fopen(path, "a+")) == NULL) {
        gplog(LOG_ERR, "open file failed errno(%d).", errno);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }
    write_count = fwrite((char *)buffer, sizeof(char), len, fp);
    if (write_count != len) {
        gplog(LOG_ERR, "call fwrite failed.write_count is %u. log_len is %u.", write_count, len);
        (void)fclose(fp);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }
    (void)fflush(fp);
    (void)fclose(fp);
    return DCMI_OK;
}
#endif

int dcmi_mcu_collect_log_once(int card_id, int log_type)
{
    int ret;
    struct dcmi_mcu_log_info *log_info = NULL;
    char file_name[MAX_LENTH] = {0};
    char *buffer = NULL;
    unsigned int log_len = 0;

    log_info = &g_dcmi_mcu_log_info[log_type];
    buffer = (char *)malloc(log_info->max_lenth);
    if (buffer == NULL) {
        return DCMI_ERR_CODE_MEM_OPERATE_FAIL;
    }
    (void)memset_s(buffer, log_info->max_lenth, 0, log_info->max_lenth);

    ret = dcmi_mcu_get_log_info(card_id, log_type, buffer, log_info->max_lenth, &log_len);
    if (ret != DCMI_OK) {
        free(buffer);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    if (access(MCU_LOG_PATH, F_OK) != DCMI_OK) {
#ifndef _WIN32
        if (mkdir(MCU_LOG_PATH, S_IRWXU | S_IRGRP | S_IXGRP) < 0) {
#else
        if (_mkdir(MCU_LOG_PATH) < 0) {
#endif
            printf("mcu log will save in %s. But mkdir %s failed.", MCU_LOG_PATH, MCU_LOG_PATH);
            free(buffer);
            return DCMI_ERR_CODE_INNER_ERR;
        }
    }

    ret = dcmi_get_mcu_log_name(card_id, log_type, file_name, sizeof(file_name));
    if (ret != DCMI_OK) {
        free(buffer);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = write_buf_to_file(file_name, (const char *)buffer, log_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "write_buf_to_file failed. ret is %d.", ret);
        free(buffer);
        return ret;
    }

    free(buffer);
    return DCMI_OK;
}

int dcmi_mcu_collect_log(int card_id, int log_type)
{
    int retry_times = 0;
    int ret;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (dcmi_check_card_is_split_phy(card_id) == TRUE) {
        // 910B算力切分场景下，不支持该命令
        gplog(LOG_OP, "In the vNPU scenario, this device does not support dcmi_mcu_collect_log.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    ret = dcmi_check_card_id(card_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, ret);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if ((log_type < MCU_LOG_ERR) || (log_type >= MCU_LOG_MAX)) {
        gplog(LOG_ERR, "log_type is invalid. logtype=%d", log_type);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!dcmi_is_has_mcu()) {
        gplog(LOG_OP, "MCU is not present, this function is not supported.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    while (retry_times++ < DCMI_MCU_COLLECT_LOG_RETRY_TIMES) {
        ret = dcmi_mcu_collect_log_once(card_id, log_type);
        if ((ret == DCMI_OK) || (ret == DCMI_ERR_CODE_OPER_NOT_PERMITTED)) {
            break;
        }
        sleep(DCMI_MCU_COLLECT_LOG_WAIT_TIME);
        gplog(LOG_INFO, "mcu collect log retry. card_id=%d, log_type=%d, retry_times=%d, ret=%d", card_id, log_type,
            retry_times, ret);
    }

    if (ret != DCMI_OK) {
        gplog(LOG_OP, "mcu collect log failed. card_id=%d, log_type=%d, ret=%d", card_id, log_type, ret);
    } else {
        gplog(LOG_OP, "mcu collect log success. card_id=%d, log_type=%d", card_id, log_type);
    }

    return ret;
}

int dcmi_mcu_get_monitor_enable(int card_id, int device_id, int *enable_flag)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;
    if (enable_flag == NULL) {
        gplog(LOG_ERR, "info enable is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d", err);
        return err;
    }

    if (dcmi_board_type_is_station() && (device_type == MCU_TYPE)) {
        return dcmi_mcu_get_fix_word(card_id, 0, DCMI_MCU_GET_MONITOR_OPCODE, DCMI_MCU_GET_MONITOR_LEN, enable_flag);
    } else {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_mcu_set_monitor_enable(int card_id, int device_id, int enable_flag)
{
    int err;
    enum dcmi_unit_type device_type = INVALID_TYPE;

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (dcmi_board_type_is_station() && (device_type == MCU_TYPE)) {
        err = dcmi_mcu_set_info_simple(card_id, DCMI_MCU_SET_MONITOR_OPCODE, DCMI_MCU_SET_MONITOR_LEN,
            (char *)&enable_flag);
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (err != DCMI_OK) {
        gplog(LOG_OP, "set monitor enable failed. card_id=%d, device_id=%d,enable_flag=%d,err=%d", card_id, device_id,
            enable_flag, err);
        return err;
    }

    gplog(LOG_OP, "set monitor enable success. card_id=%d, device_id=%d,enable_flag=%d", card_id, device_id,
        enable_flag);
    return DCMI_OK;
}

int dcmi_mcu_set_disk_power(int card_id, int device_id, int power_flag)
{
    int err;
    enum dcmi_unit_type device_type = INVALID_TYPE;

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }
    if (device_type == MCU_TYPE) {
        if (dcmi_board_type_is_station() || dcmi_board_chip_type_is_ascend_310b()) {
            char data[RESULT_LEN_UNIT] = {0};
            data[0] = (char)power_flag;
            data[1] = DCMI_MCU_SET_DISK_POWER_STATE;
            err = dcmi_mcu_set_info_simple(card_id, DCMI_MCU_EQUIP_TEST_OPCODE, RESULT_LEN_UNIT, (char *)&data[0]);
            if (err != DCMI_OK) {
                gplog(LOG_OP, "set disk power failed. card_id=%d, device_id=%d, power_flag=%d, err=%d", card_id,
                    device_id, power_flag, err);
                return err;
            }
            gplog(LOG_OP, "set disk power success. card_id=%d, device_id=%d, power_flag=%d", card_id, device_id,
                power_flag);
            return DCMI_OK;
        } else {
            gplog(LOG_ERR, "this function is not supported.");
            return DCMI_ERR_CODE_NOT_SUPPORT;
        }
    } else {
        gplog(LOG_ERR, "device_type %d is error.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_switch_mini2mcu_channel(int card_id, int *channel_old, int *channel_new)
{
    int err;
    char chip_slot;
    int device_logic_id_old = 0;
    int device_logic_id_new = 0;
    const int task_delay_3s = 3;
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    mcu_req.lun = 0;
    mcu_req.opcode = DCMI_MCU_SWITCH_I2C_OPCODE;
    mcu_req.offset = 0;
    mcu_req.lenth = DCMI_MCU_SWITCH_I2C_LEN;
    mcu_req.req_data = &chip_slot;

    err = dcmi_get_mcu_connect_device_logic_id(&device_logic_id_old, channel_old, card_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_mcu_connect_device_logic_id failed. err is %d.", err);
        return err;
    }

    /* 硬件MCU与miniD连接 的iic为1or3(此处的1or3是芯片在卡上的物理位号)
       获取当前卡slot1 的逻辑位号 */
    chip_slot = (*channel_old == MAIN_CHANNEL_MINI2MCU) ? SPARE_CHANNEL_MINI2MCU : MAIN_CHANNEL_MINI2MCU;

    err = dcmi_mcu_set_info(card_id, &mcu_req);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_set_info failed. err is %d.", err);
        printf("\tSwitch iic channel failed.\n");
        return err;
    }

    /* 切完之后等3s重新查通道信息
       20190302 miniD->mcu的心跳是1s更新一次，原来的延时1s会概率性查询到原来的状态 */
    sleep(task_delay_3s);
    err = dcmi_get_mcu_connect_device_logic_id(&device_logic_id_new, channel_new, card_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_mcu_connect_device_logic_id failed. ret is %d.", err);
        printf("\tSwitch iic channel failed.\n");
        return err;
    }

    return DCMI_OK;
}

int dcmi_mcu_get_iic_to_miniD_status(int card_id, MCU_SMBUS_RSP_MSG *mcu_rsp)
{
    int ret;
    char req_data = 0x2;
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    mcu_req.arg = 0;
    mcu_req.opcode = DCMI_MCU_EQUIP_RESULT_OPCODE;
    mcu_req.req_data = &req_data;
    mcu_req.req_data_len = 1;

    ret = dcmi_mcu_get_info_dynamic(card_id, &mcu_req, mcu_rsp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_get_info_dynamic failed. ret is %d.", ret);
        return ret;
    }
    return DCMI_OK;
}

static int dcmi_mcu_get_i2c_peripheral_test_result(int card_id, int *health_status, int max_size)
{
    int ret;
    char data[RESULT_LEN_UNIT] = {0x1, 0x2};
    char data_info[DCMI_MCU_MSG_MAX_TOTAL_LEN] = {0};
    int test_index;
    char *pminiD_list[] = {"MINI0", "MINI1", "MINI2", "MINI3"};
    char *find_flag = NULL;
    const int task_delay_3s = 3;
    const int mcu_2_miniD_offset = 2;
    MCU_SMBUS_RSP_MSG mcu_rsp = {0};
    mcu_rsp.data_info = data_info;
    mcu_rsp.total_len = (int)sizeof(data_info);
    mcu_rsp.len = 0;

    /* 启动外围器件测试 */
    ret = dcmi_mcu_set_info_simple(card_id, DCMI_MCU_EQUIP_TEST_OPCODE, DCMI_MCU_EQUIP_TEST_LEN, (char *)&data[0]);
    if (ret != DCMI_OK) {
        gplog(LOG_OP, "call dcmi_mcu_set_info_simple start peripheral test failed. ret is %d.", ret);
        return ret;
    }

    sleep(task_delay_3s);

    /* 查询测试结果 */
    ret = dcmi_mcu_get_iic_to_miniD_status(card_id, &mcu_rsp);
    if (ret == DCMI_OK) {
        /* 解析四个miniD的状态，匹配关键字 */
        for (test_index = 0; test_index < MAX_CHIP_NUM_IN_CARD; test_index++) {
            find_flag = strstr(data_info, pminiD_list[test_index]);
            if (find_flag == NULL) {
                health_status[test_index + mcu_2_miniD_offset] = TEST_SUCCESS;
            } else {
                health_status[test_index + mcu_2_miniD_offset] = TEST_FAIL;
            }
        }
    } else {
        gplog(LOG_ERR, "call dcmi_mcu_get_peripheral_test_result failed. ret is %d.", ret);
    }

    /* 最后的时候把外围器件测试停掉 */
    data[0] = 0x0;
    ret = dcmi_mcu_set_info_simple(card_id, DCMI_MCU_EQUIP_TEST_OPCODE, DCMI_MCU_EQUIP_TEST_LEN, (char *)&data[0]);
    if (ret != DCMI_OK) {
        gplog(LOG_OP, "call dcmi_mcu_set_info_simple stop peripheral test failed. ret is %d.", ret);
        return ret;
    }

    gplog(LOG_OP, "mcu check i2c success. card_id=%d", card_id);
    return DCMI_OK;
}

int dcmi_mcu_check_i2c(int card_id, int *health_status, int buf_size)
{
    int ret;
    int channel_old = 0;
    int channel_new = 0;
    const int task_delay_10s = 10;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if ((health_status == NULL) || (buf_size < MAX_CHIP_NUM_IN_CARD + NUM_CHANNEL_MINI2MCU)) {
        gplog(LOG_ERR, "input para error.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_check_card_id(card_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, ret);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    bool check_support_result = (dcmi_board_type_is_card() && dcmi_board_chip_type_is_ascend_310());
    if (!check_support_result) {
        gplog(LOG_OP, "card id %d is not supported.", card_id);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    printf("\tChecking... Please do not interrupt!\n");
    /* 切换通道 */
    ret = dcmi_switch_mini2mcu_channel(card_id, &channel_old, &channel_new);
    if (ret == DCMI_OK) {
        if (channel_old != channel_new) {
            health_status[0] = TEST_SUCCESS;
            health_status[1] = TEST_SUCCESS;
        } else {
            /* 主通道ok */
            if (channel_new == MAIN_CHANNEL_MINI2MCU) {
                health_status[0] = TEST_SUCCESS;
                health_status[1] = TEST_FAIL;
            } else {
                health_status[0] = TEST_FAIL;
                health_status[1] = TEST_SUCCESS;
            }
            /* 当有通道是坏的时候，等待MCU主动切回来 */
            sleep(task_delay_10s);
        }
    } else {
        gplog(LOG_OP, "call dcmi_switch_mini2mcu_channel failed. ret is %d.", ret);
        return DCMI_OK;
    }

    gplog(LOG_OP, "start peripheral test, card_id=%d.", card_id);
    (void)dcmi_mcu_get_i2c_peripheral_test_result(card_id, health_status, buf_size);

    return DCMI_OK;
}

int dcmi_mcu_set_boot_area(int card_id)
{
    char value = 0;
    return dcmi_mcu_set_info_simple(card_id, DCMI_MCU_SET_BOOT_AEREA_OPCODE, DCMI_MCU_SET_BOOT_AEREA_LEN, &value);
}

int dcmi_mcu_get_boot_area(int card_id, int *boot_status)
{
    return dcmi_mcu_get_fix_word(card_id, 0, DCMI_MCU_GET_BOOT_AEREA_OPCODE, DCMI_MCU_GET_BOOT_AEREA_LEN, boot_status);
}

int dcmi_mcu_get_chip_temperature(int card_id, char *data_info, int buf_size, int *data_len)
{
    int ret;
    int req_data = 0;
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    MCU_SMBUS_RSP_MSG mcu_rsp = {0};
    mcu_req.arg = 0;
    mcu_req.opcode = DCMI_MCU_DEV_TEMPERATURE_OPCODE;
    mcu_req.req_data = (char *)&req_data;
    mcu_req.req_data_len = 0;
    mcu_rsp.data_info = data_info;
    mcu_rsp.total_len = buf_size;
    mcu_rsp.len = 0;

    if (!dcmi_is_has_mcu()) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if ((data_info == NULL) || (data_len == NULL) || (buf_size <= 0)) {
        gplog(LOG_ERR, "dcmi_mcu_get_chip_temperature para is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_check_card_id(card_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, ret);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_mcu_get_info_dynamic(card_id, &mcu_req, &mcu_rsp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_get_info_dynamic failed. ret is %d.", ret);
        return ret;
    }
    *data_len = mcu_rsp.len;
    return DCMI_OK;
}

int dcmi_mcu_start_bootloader_upgrade(int card_id, char *data_info, int len)
{
    int err;

    if (!dcmi_is_has_mcu()) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_check_card_id(card_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, err);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if ((data_info == NULL) || (len <= 0)) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    err = dcmi_mcu_set_info_dynamic(card_id, DCMI_MCU_UPGRADE_BOOT_OPCODE, len, data_info);
    if (err != DCMI_OK) {
        gplog(LOG_OP, "start mcu upgrade bootloader failed. card_id=%d, err=%d", card_id, err);
        return err;
    }

    gplog(LOG_OP, "start mcu upgrade bootloader success. card_id=%d, err=%d", card_id, err);
    return DCMI_OK;
}

int dcmi_mcu_get_chip_info(int card_id, struct dcmi_chip_info_v2 *chip_info)
{
    int ret;

    ret = memcpy_s(chip_info->chip_type, MAX_CHIP_NAME_LEN, "mcu", strlen("mcu"));
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed. ret is %d", ret);
    }

    ret = memcpy_s(chip_info->chip_name, MAX_CHIP_NAME_LEN, "MCU", strlen("MCU"));
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed. ret is %d.", ret);
    }

    ret = memcpy_s(chip_info->chip_ver, MAX_CHIP_NAME_LEN, "V100", strlen("V100"));
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed.%d.", ret);
    }

    return DCMI_OK;
}

int dcmi_mcu_get_power_info(int card_id, int *power)
{
    int ret;

    if (power == NULL) {
        gplog(LOG_ERR, "power pointer is null.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_check_card_id(card_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, ret);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!dcmi_is_has_mcu() || dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_ERR, "Mcu does not exist or this device does not support");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return dcmi_mcu_get_fix_word(card_id, 0, DCMI_MCU_POWER_OPCODE, DCMI_MCU_POWER_LEN, power);
}

int dcmi_mcu_set_customized_info(int card_id, char *info, int len)
{
    int ret = DCMI_ERR_CODE_NOT_SUPPORT;

    if (dcmi_is_has_mcu()) {
        int send_len = len;
        unsigned char buffer[DCMI_MCU_CUSTOMIZED_INFO_MAX_LEN + DCMI_MCU_CUSTOMIZED_INFO_HEAD_LEN] = {0};
        const int task_delay_3s = 3;

        if (send_len > DCMI_MCU_CUSTOMIZED_INFO_MAX_LEN - 1 || send_len <= 0) {
            gplog(LOG_ERR, "info length %d is invalid.", send_len);
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }

        *(unsigned int *)(void *)buffer = DCMI_MCU_CUSTOMIZED_INFO_VERSION;
        *(unsigned short *)(buffer + DCMI_MCU_CUSTOMIZED_INFO_HEAD_VERSION_LEN) = (unsigned short)send_len;
        *(unsigned short *)(buffer + DCMI_MCU_CUSTOMIZED_INFO_HEAD_SEND_LEN +
                            DCMI_MCU_CUSTOMIZED_INFO_HEAD_VERSION_LEN) = crc16((unsigned char *)info, send_len);

        ret = memcpy_s(buffer + DCMI_MCU_CUSTOMIZED_INFO_HEAD_LEN, DCMI_MCU_CUSTOMIZED_INFO_MAX_LEN, info, send_len);
        if (ret != EOK) {
            gplog(LOG_ERR, "call memcpy_s failed. ret is %d", ret);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }

        ret = dcmi_mcu_set_info_dynamic(
            card_id, DCMI_MCU_SET_CUSTOMIZED_INFO, send_len + DCMI_MCU_CUSTOMIZED_INFO_HEAD_LEN, (char *)buffer);
        if (ret == DCMI_OK) {
            sleep(task_delay_3s);
        }
    }
    return ret;
}

int dcmi_mcu_get_customized_info(int card_id, char *data_info, int data_info_size, int *len)
{
    int ret;
    int req_data = 0;
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    MCU_SMBUS_RSP_MSG mcu_rsp = {0};
    mcu_req.arg = 0;
    mcu_req.opcode = DCMI_MCU_GET_CUSTOMIZED_INFO;
    mcu_req.req_data = (char *)&req_data;
    mcu_req.req_data_len = 0;
    mcu_rsp.data_info = data_info;
    mcu_rsp.total_len = data_info_size;
    mcu_rsp.len = 0;

    ret = dcmi_mcu_get_info_dynamic(card_id, &mcu_req, &mcu_rsp);
    *len = mcu_rsp.len;
    return ret;
}

int dcmi_get_vrd_upgrade_status(int card_id, int *status, int *progress)
{
    int ret;
    char req_data = 0x2;              // 请求参数2表示获取VRD升级状态
    unsigned char data_info[DCMI_MCU_RECV_MSG_DATA_LEN_FOR_DEFAULT] = {0};
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    MCU_SMBUS_RSP_MSG mcu_rsp = {0};

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_ERR, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if ((status == NULL) || (progress == NULL)) {
        gplog(LOG_ERR, "dcmi_get_vrd_upgrade_status input para error.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_check_card_id(card_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, ret);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!(dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_910b()) || !dcmi_is_has_mcu()) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    mcu_req.arg = 0;
    mcu_req.opcode = DCMI_MCU_UPGRADE_STATE_OPCODE;
    mcu_req.lenth = DCMI_MCU_UPGRADE_STATE_LEN;
    mcu_req.req_data = &req_data;
    mcu_req.req_data_len = 1;
    mcu_req.offset = 0;
    mcu_rsp.data_info = (char *)data_info;
    ret = dcmi_mcu_get_info(card_id, &mcu_req, &mcu_rsp);
    if (ret != DCMI_OK) {
        return ret;
    }

    *status = data_info[0] & 0xFF;
    *progress = data_info[1] & 0xFF;
    return DCMI_OK;
}

int dcmi_get_vrd_info(int card_id, char *version, int len)
{
    int ret;
    int req_data = 0;
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    MCU_SMBUS_RSP_MSG mcu_rsp = {0};
    mcu_req.arg = 0;
    mcu_req.opcode = DCMI_MCU_VRD_INFO_OPCODE;
    mcu_req.req_data = (char *)&req_data;
    mcu_req.req_data_len = 0;
    mcu_rsp.len = 0;
    mcu_rsp.total_len = len;
    mcu_rsp.data_info = version;

    if ((version == NULL) || (len <= 0)) {
        gplog(LOG_ERR, "dcmi_get_vrd_info input para error.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_check_card_id(card_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, ret);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!(dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_910b() || \
        dcmi_board_chip_type_is_ascend_910_93()) || !dcmi_is_has_mcu()) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_mcu_get_info_dynamic(card_id, &mcu_req, &mcu_rsp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_get_info_dynamic failed. ret is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    return DCMI_OK;
}

int dcmi_set_vrd_upgrade_stage(int card_id, enum dcmi_upgrade_type input_type)
{
    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (input_type != VRD_UPGRADE_START) {
        gplog(LOG_ERR, "input type error. input_type=%d", input_type);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    int err = dcmi_check_card_id(card_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, err);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!(dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_910b()) || !dcmi_is_has_mcu()) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_mcu_set_info_simple(card_id, DCMI_MCU_UPGRADE_COMMAND_OPCODE, DCMI_MCU_UPGRADE_COMMAND_LEN,
        (char *)&input_type);
    if (err != DCMI_OK) {
        gplog(LOG_OP, "set vrd upgrade stage failed. card_id=%d, upgrade_type=%d, err=%d", card_id, input_type, err);
        return err;
    }

    gplog(LOG_OP, "set vrd upgrade stage success. card_id=%d, upgrade_type=%d", card_id, input_type);
    return DCMI_OK;
}

int dcmi_mcu_get_device_elabel_info(int card_id, struct dcmi_elabel_info *elabel_info)
{
    int req_data = 0;
    int dcmi_elabel_sn = 0;
    int dcmi_elabel_mf = 0;
    int dcmi_elabel_pn = 0;
    int dcmi_elabel_model = 0;
    int err;
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    MCU_SMBUS_RSP_MSG mcu_rsp = {0};
    mcu_req.opcode = DCMI_MCU_ELABEL_OPCODE;
    mcu_req.req_data = (char *)&req_data;
    mcu_req.req_data_len = 0;
    mcu_rsp.total_len = MAX_LENTH;
    mcu_rsp.len = 0;

    dcmi_get_elabel_item_it(&dcmi_elabel_sn, &dcmi_elabel_mf, &dcmi_elabel_pn, &dcmi_elabel_model);

    mcu_req.arg = (unsigned char)dcmi_elabel_sn;
    mcu_rsp.data_info = elabel_info->serial_number;
    err = dcmi_mcu_get_info_dynamic(card_id, &mcu_req, &mcu_rsp);
    if ((err != DCMI_OK) || (mcu_rsp.len == 0)) {
        dcmi_set_default_elabel_str(elabel_info->serial_number, sizeof(elabel_info->serial_number));
    }

    mcu_req.arg = (unsigned char)dcmi_elabel_mf;
    mcu_rsp.data_info = elabel_info->manufacturer;
    err = dcmi_mcu_get_info_dynamic(card_id, &mcu_req, &mcu_rsp);
    if ((err != DCMI_OK) || (mcu_rsp.len == 0)) {
        dcmi_set_default_elabel_str(elabel_info->manufacturer, sizeof(elabel_info->manufacturer));
    }

    mcu_req.arg = (unsigned char)dcmi_elabel_pn;
    mcu_rsp.data_info = elabel_info->product_name;
    err = dcmi_mcu_get_info_dynamic(card_id, &mcu_req, &mcu_rsp);
    if ((err != DCMI_OK) || (mcu_rsp.len == 0)) {
        dcmi_set_default_elabel_str(elabel_info->product_name, sizeof(elabel_info->product_name));
    }

    mcu_req.arg = (unsigned char)dcmi_elabel_model;
    mcu_rsp.data_info = elabel_info->model;
    err = dcmi_mcu_get_info_dynamic(card_id, &mcu_req, &mcu_rsp);
    if ((err != DCMI_OK) || (mcu_rsp.len == 0)) {
        dcmi_set_default_elabel_str(elabel_info->model, sizeof(elabel_info->model));
    }

    return DCMI_OK;
}

int dcmi_set_check_customized_is_exist(int card_id)
{
    char buffer[DCMI_MCU_CUSTOMIZED_INFO_MAX_LEN] = {0};
    int length = 0;

    int err = dcmi_mcu_get_customized_info(card_id, buffer, sizeof(buffer), &length);
    if (err != DCMI_OK) {
        gplog(LOG_OP, "failed to get customized info. card_id=%d", card_id);
        return err;
    }

    if (length != 0) {
        gplog(LOG_OP, "Customized info already exists. It cannot be set repeatedly. card_id=%d", card_id);
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    return DCMI_OK;
}

int dcmi_mcu_get_boot_sel(int card_id, int device_id, int *boot_sel)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;
    if (boot_sel == NULL) {
        gplog(LOG_ERR, "info enable is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d", err);
        return err;
    }

    if (dcmi_board_chip_type_is_ascend_310b() && (device_type == MCU_TYPE)) {
        return dcmi_mcu_get_fix_word(card_id, 0, DCMI_MCU_GET_BOOT_SEL_OPCODE, DCMI_MCU_GET_BOOT_SEL_LEN, boot_sel);
    } else {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_mcu_set_boot_sel(int card_id, int device_id, int boot_sel)
{
    int err;
    enum dcmi_unit_type device_type = INVALID_TYPE;

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (dcmi_board_chip_type_is_ascend_310b() && (device_type == MCU_TYPE)) {
        err = dcmi_mcu_set_info_simple(card_id, DCMI_MCU_SET_BOOT_SEL_OPCODE, DCMI_MCU_SET_BOOT_SEL_LEN,
            (char *)&boot_sel);
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (err != DCMI_OK) {
        gplog(LOG_OP, "set boot select failed. card_id=%d, device_id=%d,boot_select=%d,err=%d", card_id, device_id,
            boot_sel, err);
        return err;
    }

    gplog(LOG_OP, "set boot select success. card_id=%d, device_id=%d,boot_select=%d", card_id, device_id,
        boot_sel);
    return DCMI_OK;
}