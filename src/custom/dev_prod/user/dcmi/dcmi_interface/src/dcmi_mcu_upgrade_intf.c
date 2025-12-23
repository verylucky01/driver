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
#include "dcmi_product_judge.h"
#include "dcmi_virtual_intf.h"
#include "dcmi_common.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_product_judge.h"
#include "dcmi_environment_judge.h"
#include "dcmi_mcu_intf.h"

#if defined DCMI_VERSION_1

int dcmi_mcu_get_upgrade_statues(int card_id, int *status, int *progress)
{
    int err;

    if (dcmi_is_in_phy_machine_root() == false) {
        gplog(LOG_ERR, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    err = dcmi_check_card_id(card_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, err);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_ERR, "This device does not support dcmi_mcu_get_upgrade_statues.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_get_mcu_upgrade_status(card_id, status, progress);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "get mcu upgrade statues failed. card_id=%d, err=%d", card_id, err);
        return err;
    }
    return err;
}

int dcmi_mcu_get_upgrade_status(int card_id, int *status, int *progress)
{
    int err;
    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_ERR, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    err = dcmi_check_card_id(card_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, err);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_ERR, "This device does not support dcmi_mcu_get_upgrade_status.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_get_mcu_upgrade_status(card_id, status, progress);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "get mcu upgrade status failed. card_id=%d, err=%d", card_id, err);
        return err;
    }
    return err;
}

int dcmi_mcu_upgrade_control(int card_id, int upgrade_type)
{
    int err;
    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    err = dcmi_set_mcu_upgrade_stage(card_id, upgrade_type);
    if (err != DCMI_OK) {
        gplog(LOG_OP, "mcu upgrade control failed. card_id=%d, upgrade_type=%d, err=%d", card_id, upgrade_type, err);
        return err;
    }
    gplog(LOG_OP, "mcu upgrade control success. card_id=%d, upgrade_type=%d", card_id, upgrade_type);
    return err;
}

int dcmi_mcu_upgrade_transfile(int card_id, const char *file)
{
    int err;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    err = dcmi_set_mcu_upgrade_file(card_id, file);
    if (err != DCMI_OK) {
        gplog(LOG_OP, "mcu upgrade transfile failed. card_id=%d, err=%d", card_id, err);
        return err;
    }
    gplog(LOG_OP, "mcu upgrade transfile success. card_id=%d", card_id);
    return err;
}

int dcmi_mcu_get_version(int card_id, char *version_str, int max_version_len, int *len)
{
    int err;

    if (version_str == NULL || len == NULL) {
        gplog(LOG_ERR, "version_str or len is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_ERR, "This device does not support dcmi_mcu_get_version.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_get_mcu_version(card_id, version_str, max_version_len);
    if (err == DCMI_OK) {
        *len = (int)strlen(version_str);
    }
    return err;
}
#endif

int dcmi_set_mcu_upgrade_stage(int card_id, enum dcmi_upgrade_type input_type)
{
    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (dcmi_check_card_is_split_phy(card_id) == TRUE) {
        // 算力切分场景下，不支持该命令
        gplog(LOG_OP, "In the vNPU scenario, this device does not support dcmi_set_mcu_upgrade_stage.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if ((input_type != MCU_UPGRADE_START) && (input_type != MCU_UPGRADE_VALIDETE)) {
        gplog(LOG_ERR, "input type error. input_type=%d", input_type);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    int err = dcmi_check_card_id(card_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, err);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!dcmi_is_has_mcu()) {
        gplog(LOG_OP, "mcu is not present, this function is not supported. card_id=%d", card_id);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_mcu_set_info_simple(card_id, DCMI_MCU_UPGRADE_COMMAND_OPCODE, DCMI_MCU_UPGRADE_COMMAND_LEN,
        (char *)&input_type);
    if (err != DCMI_OK) {
        gplog(LOG_OP, "set mcu upgrade stage failed. card_id=%d, upgrade_type=%d, err=%d", card_id, input_type, err);
        return err;
    }

    gplog(LOG_OP, "set mcu upgrade stage success. card_id=%d, upgrade_type=%d", card_id, input_type);
    return DCMI_OK;
}

#ifndef _WIN32
static int dcmi_mcu_upload_file_linux(int card_id, FILE *fp, unsigned int file_len)
{
    int fd, ret;
    unsigned int num_id, block_num;
    size_t count;
    char buff[BUFSIZE] = {0};
    unsigned int send_msg_data_max_len = (unsigned int)dcmi_mcu_get_send_data_max_len();
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    mcu_req.opcode = DCMI_MCU_UPGRADE_OPCODE;
    mcu_req.offset = 0;
    mcu_req.req_data = buff;

    block_num = file_len / send_msg_data_max_len;
    block_num = (file_len % send_msg_data_max_len != 0) ? (block_num + 1) : block_num;

    printf("\nfile_len(%u)--offset(%u) [0].", file_len, mcu_req.offset);
    (void)fflush(stdout);

    ret = dcmi_mcu_set_lock_up(&fd, 1000);      // dcmi mcu lock timeout 1000
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_set_lock failed. ret is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    for (num_id = 0; num_id < block_num; num_id++) {
        mcu_req.lenth = (mcu_req.offset + send_msg_data_max_len >= file_len) ?
            (file_len - mcu_req.offset) : send_msg_data_max_len;
        mcu_req.lun = (mcu_req.offset + send_msg_data_max_len >= file_len) ? DCMI_MCU_MSG_LUN : 0x0;

        count = fread(mcu_req.req_data, 1, mcu_req.lenth, fp);
        if ((count == 0) || (count != mcu_req.lenth)) {
            dcmi_mcu_set_unlock_up(fd);
            gplog(LOG_ERR, "count is %u err. mcu_req.lenth is %u.", count, mcu_req.lenth);
            return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
        }

        ret = dcmi_mcu_set_info(card_id, &mcu_req);
        if (ret != DCMI_OK) {
            dcmi_mcu_set_unlock_up(fd);
            gplog(LOG_ERR, "call dcmi_mcu_set_info fail.ret is %d.", ret);
            return ret;
        }

        mcu_req.offset += mcu_req.lenth;

        if ((mcu_req.offset % 1500) == 0) {         // print offset interval 1500
            printf("\rfile_len(%u)--offset(%u) [%u].", file_len, mcu_req.offset,
                mcu_req.offset * 100 / file_len);      // progress percentage base 100
            fflush(stdout);
        }

        if (num_id % 50 == 0) {     // task upload delay interval 50
            usleep(1);              // task delay 1s
        }
    }

    dcmi_mcu_set_unlock_up(fd);

    printf("\rfile_len(%u)--offset(%u) [100].\n", file_len, mcu_req.offset);
    return DCMI_OK;
}

static int dcmi_set_mcu_upgrade_file_linux(int card_id, const char *file)
{
    int ret;
    FILE *fp = NULL;
    unsigned int file_len = 0;
    char path[PATH_MAX + 1] = {0x00};

    ret = dcmi_file_realpath_disallow_nonexist(file, path, sizeof(path));
    if (ret != DCMI_OK) {
        printf("\t%-30s : Upgrade file path is illegal.\n", "Message");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    /* 计算升级包长度 */
    ret = dcmi_get_file_length(path, &file_len);
    if ((ret != DCMI_OK) || (file_len <= 0) || (file_len > DCMI_MCU_FILE_MAX_SIZE)) {
        gplog(LOG_ERR, "call get_file_length %u failed.", file_len);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    /* 打开文件 */
    if ((fp = fopen((const char *)path, "r")) == NULL) {
        gplog(LOG_ERR, "open upgrade file failed errno is %d.", errno);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = dcmi_mcu_upload_file_linux(card_id, fp, file_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_mcu_upload_file_linux failed. ret is %d.", ret);
        (void)fclose(fp);
        return ret;
    }
    (void)fclose(fp);
    return DCMI_OK;
}
#else
static int dcmi_mcu_upload_file_win(int card_id, HANDLE hFile, UINT32 file_len)
{
    int ret, block_num, num_id;
    DWORD dwStatus;
    DWORD cbRead = 0;
    char buff[BUFSIZE] = { 0 };
    int send_msg_data_max_len = dcmi_mcu_get_send_data_max_len();
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    mcu_req.opcode = DCMI_MCU_UPGRADE_OPCODE;
    mcu_req.offset = 0;
    mcu_req.req_data = buff;

    if (send_msg_data_max_len == 0) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    
    block_num = file_len / send_msg_data_max_len;
    if (file_len % send_msg_data_max_len != 0) {
        block_num++;
    }

    printf("\nfile_len(%d)--offset(%d) [0].", file_len, mcu_req.offset);
    (void)fflush(stdout);
    for (num_id = 0; num_id < block_num; num_id++) {
        if (mcu_req.offset + send_msg_data_max_len >= file_len) {
            mcu_req.lenth = file_len - mcu_req.offset;
            mcu_req.lun = DCMI_MCU_MSG_LUN;
        } else {
            mcu_req.lenth = send_msg_data_max_len;
            mcu_req.lun = 0x0;
        }

        if (ReadFile(hFile, mcu_req.req_data, mcu_req.lenth, &cbRead, NULL) == FALSE) {
            dwStatus = GetLastError();
            gplog(LOG_ERR, "call ReadFile Error: %d.", dwStatus);
            return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
        }
        if ((cbRead == 0) || (cbRead != mcu_req.lenth)) {
            gplog(LOG_ERR, "count is %d err. mcu_req.lenth is %d.", cbRead, mcu_req.lenth);
            return DCMI_ERR_CODE_INNER_ERR;
        }

        ret = dcmi_mcu_set_info(card_id, &mcu_req);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_mcu_set_info fail.ret is %d.", ret);
            return DCMI_ERR_CODE_INNER_ERR;
        }

        mcu_req.offset += mcu_req.lenth;

        if ((mcu_req.offset % PRINT_PROCESS_CNT) == 0 && file_len != 0) {
            printf("\rfile_len(%d)--offset(%d) [%d].", file_len, mcu_req.offset,
                mcu_req.offset * PERCENTAGE_FACTOR / file_len);
            fflush(stdout);
        }
    }
    return DCMI_OK;
}

static int dcmi_set_mcu_upgrade_file_win(int card_id, const char *file)
{
    int ret;
    unsigned int file_len = 0;
    char path[MAX_PATH + 1] = {0x00};
    HANDLE hFile;
    DWORD  status;
    wchar_t pcStr[MAX_PATH + 1] = {0x00};

    ret = dcmi_file_realpath_disallow_nonexist(file, path, sizeof(path));
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = MultiByteToWideChar(CP_ACP, 0, path, -1, pcStr, 0);
    status = MultiByteToWideChar(CP_ACP, 0, path, -1, pcStr, ret);
    if (status == FALSE) {
        status = GetLastError();
        gplog(LOG_ERR, "call MultiByteToWideChar Error: %d", status);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }
    /* 计算升级包长度 */
    ret = dcmi_get_file_length(file, &file_len);
    if ((ret != DCMI_OK) || (file_len <= 0) || (file_len > DCMI_MCU_FILE_MAX_SIZE)) {
        gplog(LOG_ERR, "call get_file_length failed. %d.", file_len);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    hFile = CreateFile(pcStr, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        status = GetLastError();
        gplog(LOG_ERR, "Error opening mcu upgrade file Error: %d", status);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = dcmi_mcu_upload_file_win(card_id, hFile, file_len);
    CloseHandle(hFile);
    return ret;
}
#endif

int dcmi_set_mcu_upgrade_file(int card_id, const char *file)
{
    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (file == NULL) {
        gplog(LOG_ERR, "file is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    int err = dcmi_check_card_id(card_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, err);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_is_has_mcu()) {
#ifndef _WIN32
        err = dcmi_set_mcu_upgrade_file_linux(card_id, file);
#else
        err = dcmi_set_mcu_upgrade_file_win(card_id, file);
#endif
        if (err != DCMI_OK) {
            gplog(LOG_OP, "set mcu upgrade file failed. card_id=%d, err=%d", card_id, err);
            return err;
        }

        gplog(LOG_OP, "set mcu upgrade file success. card_id=%d", card_id);
        return DCMI_OK;
    }

    gplog(LOG_OP, "set mcu upgrade file is not support. card_id=%d", card_id);
    return DCMI_ERR_CODE_NOT_SUPPORT;
}

int dcmi_get_mcu_upgrade_status(int card_id, int *status, int *progress)
{
    int ret;
    char req_data = 0x1;              // 请求参数1表示获取MCU升级状态
    unsigned char data_info[DCMI_MCU_RECV_MSG_DATA_LEN_FOR_DEFAULT] = {0};

    MCU_SMBUS_REQ_MSG mcu_req = {0};
    MCU_SMBUS_RSP_MSG mcu_rsp = {0};

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_ERR, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (dcmi_check_card_is_split_phy(card_id) == TRUE) {
        // 算力切分场景下，不支持该命令
        gplog(LOG_OP, "In the vNPU scenario, this device does not support dcmi_get_mcu_upgrade_status.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if ((status == NULL) || (progress == NULL)) {
        gplog(LOG_ERR, "dcmi_mcu_get_upgrade_statues input para error.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_check_card_id(card_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, ret);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!dcmi_is_has_mcu()) {
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

int dcmi_get_mcu_version(int card_id, char *version, int len)
{
    int ret;
    char str_version[MAX_LENTH] = {0};
    MCU_SMBUS_REQ_MSG mcu_req = {0};
    MCU_SMBUS_RSP_MSG mcu_rsp = {0};
    mcu_req.arg = 0;
    mcu_req.opcode = DCMI_MCU_VERSION_OPCODE;
    mcu_req.lenth = DCMI_MCU_VERSION_LEN;
    mcu_rsp.len = 0;
    mcu_rsp.data_info = str_version;

    if ((version == NULL) || (len <= 0)) {
        gplog(LOG_ERR, "dcmi_mcu_get_version input para error.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_check_card_id(card_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, ret);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!dcmi_is_has_mcu()) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_mcu_get_info_fix(card_id, &mcu_req, &mcu_rsp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_get_info_fix fail.ret is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    return dcmi_mcu_version_parse(version, len, &mcu_rsp);
}