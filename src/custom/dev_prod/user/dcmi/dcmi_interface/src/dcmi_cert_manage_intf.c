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
 
#include "securec.h"
#include "dsmi_common_interface_custom.h"
#include "dcmi_interface_api.h"
#include "dcmi_init_basic.h"
#include "dcmi_fault_manage_intf.h"
#include "dcmi_log.h"
#include "dcmi_virtual_intf.h"
#include "dcmi_common.h"
#include "dcmi_inner_cfg_manage.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_product_judge.h"
#include "dcmi_environment_judge.h"
#include "dcmi_cert_manage_intf.h"

#if defined DCMI_VERSION_2
int dcmi_set_device_sec_revocation(int card_id, int device_id, enum dcmi_revo_type input_type,
    const unsigned char *file_data, unsigned int file_size)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (dcmi_check_card_is_split_phy(card_id) == TRUE) {
        // 910B算力切分场景下，不支持该命令
        gplog(LOG_OP, "In the vNPU scenario, this device does not support dcmi_set_device_sec_revocation.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (file_data == NULL) {
        gplog(LOG_ERR, "file_data is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    bool check_result = ((input_type >= DCMI_REVOCATION_TYPE_MAX) || (file_size != SEC_REVOCATION_FILE_LEN));
    if (check_result) {
        gplog(LOG_ERR, "input para is invalid. input_type=%d file_size=%u", input_type, file_size);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (!dcmi_board_chip_type_is_ascend_910() && !dcmi_board_chip_type_is_ascend_910b() &&
        !dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_ERR, "This device does not support setting sec revocation.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if ((dcmi_board_chip_type_is_ascend_910_93()) && (device_id != 0)) {
        gplog(LOG_OP, "This product is only supported on chip 0.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_set_npu_device_sec_revocation(card_id, device_id, input_type, file_data, file_size);
        if (err != DCMI_OK) {
            gplog(LOG_OP, "set sec revocation failed. card_id=%d, device_id=%d, revo_type=%d, err=%d", card_id,
                device_id, input_type, err);
            return err;
        }

        gplog(LOG_OP, "set sec revocation success. card_id=%d, device_id=%d, revo_type=%d", card_id, device_id,
            input_type);
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

static int dcmi_sm_data_copy(struct sm_alg_param *buf, struct dcmi_sm_parm *parm, struct dcmi_sm_data *data)
{
    int ret;
    buf->key_type = parm->key_type;
    buf->key_len = parm->key_len;
    buf->iv_len = parm->iv_len;
    buf->data_len = data->in_len;

    if ((buf->key_type == SM4_CBC_ENCRYPT) || (buf->key_type == SM4_CBC_DECRYPT)) {
        ret = memcpy_s(buf->alg_data, ALG_KDATA_LEN, parm->key, parm->key_len);
        if (ret != 0) {
            gplog(LOG_ERR, "memcpy_s key failed. err is  %d.", ret);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
        ret = memcpy_s(buf->alg_data + parm->key_len, ALG_KDATA_LEN - buf->key_len, parm->iv, parm->iv_len);
        if (ret != 0) {
            gplog(LOG_ERR, "memcpy_s iv failed. err is  %d.", ret);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
    }
    ret = memcpy_s(buf->alg_data + parm->key_len + parm->iv_len, ALG_KDATA_LEN - buf->key_len - buf->iv_len,
        data->in_buf, data->in_len);
    if (ret != 0) {
        gplog(LOG_ERR, "memcpy_s data failed. err is  %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    return DCMI_OK;
}

static int dcmi_encrypt_param_valid(struct dcmi_sm_parm *parm, struct dcmi_sm_data *data)
{
    bool invalid_param;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    invalid_param = ((parm == NULL) || (data == NULL) || (data->in_buf == NULL) || (data->out_buf == NULL) ||
        (data->out_len == NULL));
    if (invalid_param) {
        gplog(LOG_ERR, "para or in_data is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (data->in_len > ALG_DATA_MAX_LEN) {
        gplog(LOG_ERR, "in_data len is fault.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    switch (parm->key_type) {
        case SM3_NORMAL_SUMMARY:
            if ((parm->key_len != 0) || (parm->iv_len != 0) || (*(data->out_len) < SM3_OUTPUT_DATA_LEN)) {
                gplog(LOG_ERR, "SM3_NORMAL_SUMMARY param is fault.");
                return DCMI_ERR_CODE_INVALID_PARAMETER;
            }
            break;
        case SM4_CBC_ENCRYPT:
            if ((parm->key_len != SM4_ENCRYPT_KEY_LEN) || (parm->iv_len != SM4_ENCRYPT_IV_LEN) ||
                (*(data->out_len) < data->in_len)) {
                gplog(LOG_ERR, "SM4_CBC_ENCRYPT param is fault.");
                return DCMI_ERR_CODE_INVALID_PARAMETER;
            }
            break;
        default:
            gplog(LOG_ERR, "encrypt key_type is fault or not support.");
            return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    bool support_chip_type = (dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_910());
    if (!support_chip_type) {
        gplog(LOG_OP, "The device does not support this api.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    return DCMI_OK;
}

int dcmi_sm_encrypt(int card_id, int device_id, struct dcmi_sm_parm *parm, struct dcmi_sm_data *data)
{
    int err = 0;
    int device_logic_id = 0;
    unsigned int buf_len = MAX_BUF_SIZE;
    struct sm_alg_param buf = { 0 };

    err = dcmi_encrypt_param_valid(parm, data);
    if (err != 0) {
        gplog(LOG_ERR, "call dcmi_encrypt_param_valid failed. err is %d.", err);
        return err;
    }
    err = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", err);
        return err;
    }
    err = dcmi_sm_data_copy(&buf, parm, data);
    if (err != 0) {
        gplog(LOG_ERR, "call dcmi_sm_data_copy failed. err is %d.", err);
        return err;
    }

    err = dsmi_get_device_info(device_logic_id, DSMI_MAIN_CMD_EN_DECRYPTION, DSMI_CERT_SUB_CMD_ENCRYPT, (void *)(&buf),
        &buf_len);
    if (err != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_get_device_info vdec failed. err is %d.", err);
        if (err == DSMI_ERR_OPER_NOT_PERMITTED) {
            gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        } else if (err == DSMI_ERR_NOT_SUPPORT) {
            gplog(LOG_OP, "The device does not support this api.");
        }
        (void)memset_s(&buf, sizeof(buf), 0, sizeof(buf));
        return dcmi_convert_error_code(err);
    }

    if (buf_len != sizeof(struct sm_alg_param)) {
        gplog(LOG_ERR, "call dsmi_get_device_info back fault, buf_len is %u.", sizeof(buf));
        (void)memset_s(&buf, sizeof(buf), 0, sizeof(buf));
        return DCMI_ERR_CODE_INNER_ERR;
    }
    // out_len此处为用户申请大小
    err = memcpy_s(data->out_buf, *(data->out_len), ((struct sm_alg_param *)(&buf))->alg_data,
        ((struct sm_alg_param *)(&buf))->data_len);
    if (err != 0) {
        gplog(LOG_ERR, "memcpy_s data failed. err is  %d.", err);
        (void)memset_s(&buf, sizeof(buf), 0, sizeof(buf));
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    *(data->out_len) = ((struct sm_alg_param *)(&buf))->data_len;
    (void)memset_s(&buf, sizeof(buf), 0, sizeof(buf));
    gplog(LOG_INFO, "call dcmi_sm_encrypt success.");
    return DCMI_OK;
}

static int dcmi_decrypt_param_valid(struct dcmi_sm_parm *parm, struct dcmi_sm_data *data)
{
    bool invalid_param;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    invalid_param = ((parm == NULL) || (data == NULL) || (data->in_buf == NULL) || (data->out_buf == NULL) ||
        (data->out_len == NULL));
    if (invalid_param) {
        gplog(LOG_ERR, "para or in_data in is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (data->in_len > ALG_DATA_MAX_LEN) {
        gplog(LOG_ERR, "in_data len is fault.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    switch (parm->key_type) {
        case SM4_CBC_DECRYPT:
            if ((parm->key_len != SM4_DECRYPT_KEY_LEN) || (parm->iv_len != SM4_DECRYPT_IV_LEN) ||
                (*(data->out_len) < data->in_len)) {
                gplog(LOG_ERR, "SM4_CBC_DECRYPT para is fault.");
                return DCMI_ERR_CODE_INVALID_PARAMETER;
            }
            break;
        default:
            gplog(LOG_ERR, "decrypt key_type is fault or not support.");
            return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_get_board_chip_type() != DCMI_CHIP_TYPE_D310P && dcmi_get_board_chip_type() != DCMI_CHIP_TYPE_D910) {
        gplog(LOG_OP, "The device does not support this api.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    return DCMI_OK;
}

int dcmi_sm_decrypt(int card_id, int device_id, struct dcmi_sm_parm *parm, struct dcmi_sm_data *data)
{
    int err = 0;
    int device_logic_id = 0;
    unsigned int buf_len = MAX_BUF_SIZE;
    struct sm_alg_param buf = { 0 };

    err = dcmi_decrypt_param_valid(parm, data);
    if (err != 0) {
        gplog(LOG_ERR, "call dcmi_decrypt_param_valid failed. err is %d.", err);
        return err;
    }

    err = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", err);
        return err;
    }

    err = dcmi_sm_data_copy(&buf, parm, data);
    if (err != 0) {
        gplog(LOG_ERR, "call dcmi_sm_data_copy failed. err is %d.", err);
        return err;
    }

    err = dsmi_get_device_info(device_logic_id, DSMI_MAIN_CMD_EN_DECRYPTION, DSMI_CERT_SUB_CMD_DECRYPT, (void *)(&buf),
        &buf_len);
    if (err != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_get_device_info vdec failed. err is %d.", err);
        if (err == DSMI_ERR_OPER_NOT_PERMITTED) {
            gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        } else if (err == DSMI_ERR_NOT_SUPPORT) {
            gplog(LOG_OP, "The device does not support this api.");
        }
        (void)memset_s(&buf, sizeof(buf), 0, sizeof(buf));
        return dcmi_convert_error_code(err);
    }

    if (buf_len != sizeof(struct sm_alg_param)) {
        gplog(LOG_ERR, "call dsmi_get_device_info back fault, buf_len is %u.", sizeof(buf));
        (void)memset_s(&buf, sizeof(buf), 0, sizeof(buf));
        return DCMI_ERR_CODE_INNER_ERR;
    }
    // out_len此处为用户申请大小
    err = memcpy_s(data->out_buf, *(data->out_len), ((struct sm_alg_param *)(&buf))->alg_data,
        ((struct sm_alg_param *)(&buf))->data_len);
    if (err != 0) {
        gplog(LOG_ERR, "memcpy_s data failed. err is  %d.", err);
        (void)memset_s(&buf, sizeof(buf), 0, sizeof(buf));
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    *(data->out_len) = ((struct sm_alg_param *)(&buf))->data_len;
    (void)memset_s(&buf, sizeof(buf), 0, sizeof(buf));
    gplog(LOG_INFO, "call dcmi_sm_decrypt success.");
    return DCMI_OK;
}
#endif