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
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "securec.h"
#include "dsmi_common_interface_custom.h"
#include "dcmi_i2c_operate.h"
#include "dcmi_interface_api.h"
#include "dcmi_log.h"
#include "dcmi_common.h"
#include "dcmi_fault_manage_intf.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_product_judge.h"
#include "dcmi_elabel_operate.h"

#ifndef _WIN32

struct dcmi_elabel_env g_elabel_env = {0};
struct dcmi_elabel_data g_elabel_data_tmp = {0};
const char *g_i2c_dev_name = I2C8_DEV_NAME; // 默认底板e2prom的i2c名称

void dcmi_set_i2c_dev_name(const char *i2c_dev_name)
{
    g_i2c_dev_name = i2c_dev_name;
    return;
}

int dcmi_elabel_set_lock(int *fd, unsigned int timeout)
{
    int ret;
    unsigned int try_index;
    int fd_curr = -1;
    unsigned int time_curr = 0;
    struct flock f_lock = {0};

    fd_curr = open(ELABEL_LOCK_FILE_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd_curr < 0) {
        gplog(LOG_ERR, "elabel_get_lock:open file %s failed.", ELABEL_LOCK_FILE_NAME);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    *fd = fd_curr;
    f_lock.l_type = F_WRLCK;
    f_lock.l_whence = 0;
    f_lock.l_len = 0;

    while (time_curr < timeout) {
        /* 首先循环一定次数尝试不阻塞方式获取锁 */
        for (try_index = 0; try_index < ELABEL_MUTEX_FIRST_TRY_TIMES; try_index++) {
            ret = fcntl(fd_curr, F_SETLK, &f_lock);
            if (ret == DCMI_OK) {
                return DCMI_OK;
            }
        }

        /* 未获取到锁，等待1ms，再次尝试获取 */
        (void)usleep(ELABEL_MUTEX_SLEEP_TIMES_1MS);
        time_curr++;
    }

    close(fd_curr);

    return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
}

// 进程间文件锁，当做进程间锁
void dcmi_elabel_set_unlock(int fd)
{
    struct flock f_lock = {0};
    if (fd < 0) {
        gplog(LOG_ERR, "dcmi_elabel_set_unlock:fd is invalid. fd = %d", fd);
        return;
    }

    f_lock.l_type = F_UNLCK;
    f_lock.l_whence = 0;
    f_lock.l_len = 0;

    fcntl(fd, F_SETLK, &f_lock);
    close(fd);
    return;
}

static int dcmi_elabel_item_set_default(unsigned int i)
{
    int ret;
    g_elabel_env.elabel_data->magic_num = ELABEL_MAGIC_NUMBEER;

    struct dcmi_elabel_field_bytes *data =
        (struct dcmi_elabel_field_bytes *)((unsigned char *)(g_elabel_env.elabel_data) + elabel_items[i].offset);
    ret = memcpy_s(data->data,
        elabel_items[i].max_len,
        (char const *)elabel_items[i].def_value,
        strlen((char const *)elabel_items[i].def_value) + 1);
    if (ret != EOK) {
        gplog(LOG_ERR, "[dcmi_elabel_item_set_default] memcpy_s failed. ret is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    data->len = strlen((char const *)elabel_items[i].def_value);
    data->crc = crc16(data->data, data->len);

    return DCMI_OK;
}

static const struct dcmi_elabel_item *dcmi_elabel_find_item(unsigned char item_id)
{
    unsigned int elabel_index;
    for (elabel_index = 0; elabel_index < sizeof(elabel_items) / sizeof(elabel_items[0]); elabel_index++) {
        if (elabel_items[elabel_index].item_id == item_id) {
            return &elabel_items[elabel_index];
        }
    }
    return NULL;
}

static int dcmi_elabel_verify(struct dcmi_elabel_data *elabel_data)
{
    unsigned int elabel_index;

    for (elabel_index = 0; elabel_index < sizeof(elabel_items) / sizeof(elabel_items[0]); elabel_index++) {
        struct dcmi_elabel_field_bytes *data =
            (struct dcmi_elabel_field_bytes *)((unsigned char *)elabel_data + elabel_items[elabel_index].offset);

        if (data->len > elabel_items[elabel_index].max_len) {
            dcmi_elabel_item_set_default(elabel_index);
            gplog(LOG_ERR,
                "item(%u) data length %hu > max_len %hu ",
                elabel_index,
                data->len,
                elabel_items[elabel_index].max_len);
            return DCMI_ERR_CODE_INNER_ERR;
        }

        if (data->crc != crc16(data->data, data->len)) {
            dcmi_elabel_item_set_default(elabel_index);
            gplog(LOG_ERR, "item(%u) crc error,set default", elabel_index);
            return DCMI_ERR_CODE_INNER_ERR;
        }
    }

    return DCMI_OK;
}

static void dcmi_elabel_set_default(void)
{
    unsigned int elabel_index;
    int ret;
    g_elabel_env.elabel_data->magic_num = ELABEL_MAGIC_NUMBEER;

    for (elabel_index = 0; elabel_index < sizeof(elabel_items) / sizeof(elabel_items[0]); elabel_index++) {
        struct dcmi_elabel_field_bytes *data =
            (struct dcmi_elabel_field_bytes *)((unsigned char *)(g_elabel_env.elabel_data) +
                                               elabel_items[elabel_index].offset);

        ret = memcpy_s(data->data,
            elabel_items[elabel_index].max_len,
            (char const *)elabel_items[elabel_index].def_value,
            strlen((char const *)elabel_items[elabel_index].def_value) + 1);
        if (ret != EOK) {
            gplog(LOG_ERR, "call memcpy_s failed. ret is %d", ret);
            return;
        }

        data->len = strlen((char const *)elabel_items[elabel_index].def_value);

        data->crc = crc16(data->data, data->len);
    }

    return;
}

int dcmi_flash_read_elabel(unsigned char *obuffer, int begin, int read_len)
{
    int ret;
    int offset = I2C_REG_ADDR + ELABEL_OFFSET;

    if (dcmi_board_chip_type_is_ascend_310b()) {
        offset = I2C_REG_ADDR + ELABEL_310B_OFFSET;
    }
 
    ret = dcmi_i2c_get_data(g_i2c_dev_name, I2C_ELABEL_ADDR, offset, obuffer, read_len);
    if (ret < DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_i2c_get_data error. ret is %d.", ret);
        return ret;
    }

    return DCMI_OK;
}

static int dcmi_elabel_flash_read_verify(struct dcmi_elabel_data *elabel_data)
{
    int ret;
    unsigned int read_cnt = 0;

    while (read_cnt++ < ELABEL_FLASH_MAX_RD_TIMES) {
        ret = dcmi_flash_read_elabel((unsigned char *)elabel_data, 0, sizeof(struct dcmi_elabel_data));
        if (ret < 0) {
            continue;
        } else {
            if (dcmi_elabel_verify(elabel_data) != 0) {
                continue;
            } else {
                return DCMI_OK;
            }
        }
    }
    return DCMI_ERR_CODE_INNER_ERR;
}

static int dcmi_get_elabel_magic_num(unsigned int *magic_num)
{
    int ret;
    unsigned int i = 0;

    while (i++ < ELABEL_FLASH_MAX_RD_TIMES) {
        ret = dcmi_flash_read_elabel((unsigned char *)magic_num, 0, sizeof(unsigned int));
        if (ret == DCMI_OK) {
            break;
        }
        usleep(1);
    }
    return ret;
}

int dcmi_elabel_init(void)
{
    int ret;
    unsigned int magic_num = 0;
    int fd = 0;

    g_elabel_env.elabel_status = 0;
    g_elabel_env.elabel_data = &g_elabel_data_tmp;

    ret = dcmi_elabel_set_lock(&fd, DCMI_ELABEL_LOCK_TIMEOUT);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "driver_elabel bus busy.");
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dcmi_get_elabel_magic_num(&magic_num);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "read elabel magic num failed");
        dcmi_elabel_set_unlock(fd);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    if (magic_num != ELABEL_MAGIC_NUMBEER) {
        g_elabel_env.elabel_status &= ~ELABEL_STATUS_FLASH_HAS_DATA;
        dcmi_elabel_set_default();
        gplog(LOG_ERR, "dcmi_elabel_init: magic number is valid :set elabel to default.");

        ret = dcmi_elabel_update();
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_elabel_init: fail to write flash.");
            dcmi_elabel_set_unlock(fd);
            return DCMI_ERR_CODE_INNER_ERR;
        }
    } else {
        ret = dcmi_elabel_flash_read_verify(g_elabel_env.elabel_data);
        if (ret != DCMI_OK) {
            dcmi_elabel_set_default();
            gplog(LOG_ERR, "dcmi_elabel_init:fail to read and verify elabel,set elabel to default");
            ret = dcmi_elabel_update();
            if (ret != DCMI_OK) {
                gplog(LOG_ERR, "dcmi_elabel_init: fail to write flash");
            }

            dcmi_elabel_set_unlock(fd);
            return DCMI_ERR_CODE_INNER_ERR;
        }

        g_elabel_env.elabel_status |= ELABEL_STATUS_FLASH_HAS_DATA;
    }

    dcmi_elabel_set_unlock(fd);

    return DCMI_OK;
}

int dcmi_elabel_get_data(unsigned char item_id, unsigned char *data, unsigned short data_size, unsigned short *data_len)
{
    int ret;
    const struct dcmi_elabel_item *item = NULL;
    struct dcmi_elabel_field_bytes *item_data = NULL;

    // 初始状态先进行判断，如果为空，则进行初始化
    if (g_elabel_env.elabel_data == NULL) {
        ret = dcmi_elabel_init();
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_elabel_init failed. ret is %d", ret);
            return DCMI_ERR_CODE_INNER_ERR;
        }
    }

    item = dcmi_elabel_find_item(item_id);
    if (item == NULL) {
        gplog(LOG_ERR, "dcmi_elabel_init: no item %u", item_id);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    if (g_elabel_env.elabel_data == NULL) {
        gplog(LOG_ERR, "dcmi_elabel_get_data: elabel data is NULL");
        return DCMI_ERR_CODE_INNER_ERR;
    }

    item_data = (struct dcmi_elabel_field_bytes *)((unsigned char *)g_elabel_env.elabel_data + item->offset);

    *data_len = item_data->len;

    ret = memcpy_s(data, data_size, item_data->data, item_data->len);
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed. ret is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    return DCMI_OK;
}

int dcmi_flash_erase_elabel(void)
{
    int ret;
    unsigned long offset = I2C_REG_ADDR + ELABEL_OFFSET;
    char tmp[ELABEL_TOTAL_SIZE] = {0};

    if (dcmi_board_chip_type_is_ascend_310b()) {
        offset = I2C_REG_ADDR + ELABEL_310B_OFFSET;
    }
 
    ret = dcmi_i2c_set_data(g_i2c_dev_name, I2C_ELABEL_ADDR, offset, (const char *)tmp,
        sizeof(struct dcmi_elabel_data));
    if (ret < DCMI_OK) {
        gplog(LOG_ERR, "call driver_i2c_set_data failed. ret is %d", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    return DCMI_OK;
}

int dcmi_flash_write_elabel(unsigned char *elabel_data, int begin, int write_len)
{
    int ret = 0;
    unsigned long offset = I2C_REG_ADDR + ELABEL_OFFSET;
    unsigned int i = 0;

    if (dcmi_board_chip_type_is_ascend_310b()) {
        offset = I2C_REG_ADDR + ELABEL_310B_OFFSET;
    }

    while (i++ < ELABEL_FLASH_MAX_RD_TIMES) {
        ret = dcmi_i2c_set_data(g_i2c_dev_name, I2C_ELABEL_ADDR, offset, (const char *)elabel_data, write_len);
        if (ret >= DCMI_OK) {
            return DCMI_OK;
        }

        usleep(1);
    }

    gplog(LOG_ERR, "call dcmi_i2c_set_data failed. ret is %d", ret);

    return DCMI_ERR_CODE_INNER_ERR;
}

int dcmi_elabel_update(void)
{
    int ret;
    unsigned char *elabel_bak = NULL;
    int fd = 0;

    if (g_elabel_env.elabel_data == NULL) {
        return DCMI_ERR_CODE_INNER_ERR;
    }

    elabel_bak = (unsigned char *)malloc(sizeof(struct dcmi_elabel_data));
    if (elabel_bak == NULL) {
        gplog(LOG_ERR, "Card device info malloc failed.");
        return DCMI_ERR_CODE_MEM_OPERATE_FAIL;
    }

    ret = memcpy_s(elabel_bak, sizeof(struct dcmi_elabel_data),
                   g_elabel_env.elabel_data, sizeof(struct dcmi_elabel_data));
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed. ret is %d", ret);
        free(elabel_bak);
        elabel_bak = NULL;
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = dcmi_elabel_set_lock(&fd, DCMI_ELABEL_LOCK_TIMEOUT);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "driver_elabel bus busy.");
        free(elabel_bak);
        elabel_bak = NULL;
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dcmi_flash_erase_elabel();
    if (ret != DCMI_OK) {
        free(elabel_bak);
        elabel_bak = NULL;
        gplog(LOG_ERR, "fail to driver_flash_erase_elabel. ret is  %d", ret);
        dcmi_elabel_set_unlock(fd);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dcmi_flash_write_elabel(elabel_bak, 0, sizeof(struct dcmi_elabel_data));
    if (ret != DCMI_OK) {
        free(elabel_bak);
        elabel_bak = NULL;
        gplog(LOG_ERR, "fail to driver_flash_write_elabel. ret is %d", ret);
        dcmi_elabel_set_unlock(fd);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    free(elabel_bak);
    elabel_bak = NULL;
    dcmi_elabel_set_unlock(fd);

    return DCMI_OK;
}

int dcmi_elabel_clear(void)
{
    int ret;

    if (g_elabel_env.elabel_data != NULL) {
        ret = memset_s(g_elabel_env.elabel_data, sizeof(struct dcmi_elabel_data), 0, sizeof(struct dcmi_elabel_data));
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call memset_s failed. ret is %d", ret);
        }

        dcmi_elabel_set_default();
    }

    ret = dcmi_flash_erase_elabel();
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_elabel_clear: fail to erase flash. ret is %d", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    return DCMI_OK;
}

int item_value_validation(const struct dcmi_elabel_item *item, unsigned char item_id, unsigned short offset,
    unsigned char len)
{
    if (item == NULL) {
        gplog(LOG_ERR, "item_value_validation: no item %u\r\n", item_id);
        return ELABEL_RET_ARG_INVALID;
    }
    if (offset + len > item->max_len) {
        gplog(LOG_ERR, "item_value_validation: data to write is too long \r\n");
        return ELABEL_RET_ARG_INVALID;
    }
    if (g_elabel_env.elabel_data == NULL) {
        gplog(LOG_ERR, "item_value_validation: elabel data is null \r\n");
        return ELABEL_RET_MEM_ERR;
    }
    return DCMI_OK;
}

int dcmi_elabel_set_data(unsigned char item_id, unsigned char *data, unsigned short offset, unsigned char len)
{
    int ret;
    int init_cnt = 0;
    const struct dcmi_elabel_item *item = NULL;
    struct dcmi_elabel_field_bytes *item_data = NULL;

    // 初始状态先进行判断，如果为空，则进行初始化
    if (g_elabel_env.elabel_data == NULL) {
        while (init_cnt++ < ELABEL_FLASH_MAX_INT_TIMES) {
            ret = dcmi_elabel_init();
            if (ret < 0) {
                gplog(LOG_ERR, "init_cnt: %d\n", init_cnt);
                continue;
            } else {
                break;
            }
        }

        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "driver_elabel_init failed. %d\r\n", ret);
            return ret;
        }
    }

    if ((data == NULL) || (len == 0)) {
        gplog(LOG_ERR, "driver_elabel_set_data: argument invalid\r\n");
        return ELABEL_RET_ARG_INVALID;
    }

    item = dcmi_elabel_find_item(item_id);
    ret = item_value_validation(item, item_id, offset, len);
    if (ret != DCMI_OK) {
        return ret;
    }

    item_data = (struct dcmi_elabel_field_bytes *)((unsigned char *)g_elabel_env.elabel_data + item->offset);

    if (offset == 0) {
        ret = memset_s(item_data->data, item->max_len, 0, item->max_len);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call memset_s failed! %d\n", ret);
        }
    }

    ret = memcpy_s((unsigned char *)(item_data->data + offset), item->max_len, data, len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call memcpy_s failed! %d\n", ret);
        return ret;
    }

    item_data->len = offset + len;
    item_data->crc = crc16(item_data->data, item_data->len);

    ret = dcmi_elabel_update();
    if (ret < 0) {
        gplog(LOG_ERR, "elabel_update Error.%d \r\n", ret);
        return ret;
    }

    return ELABEL_RET_OK;
}

#endif

void dcmi_set_default_elabel_str(char *elabel, int elabel_size)
{
    int ret;
    ret = strncpy_s(elabel, elabel_size, "NA", strlen("NA"));
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call strncpy_s copy default elable str failed. err is %d.", ret);
    }
}

static int dcmi_soc_cpu_get_elabel_info(struct dcmi_elabel_info *elabel_info)
{
    int err;
    int dcmi_elabel_sn = 0;
    int dcmi_elabel_mf = 0;
    int dcmi_elabel_pn = 0;
    int dcmi_elabel_model = 0;
    int length = 0;
    const int soc_cpu_device_id = 0x10000;

    dcmi_get_elabel_item_it(&dcmi_elabel_sn, &dcmi_elabel_mf, &dcmi_elabel_pn, &dcmi_elabel_model);
    err = dsmi_dft_get_elable(soc_cpu_device_id, dcmi_elabel_sn, (char *)elabel_info->serial_number, &length);
    if (err != DSMI_OK) {
        dcmi_set_default_elabel_str(elabel_info->serial_number, sizeof(elabel_info->serial_number));
    }
    err = dsmi_dft_get_elable(soc_cpu_device_id, dcmi_elabel_mf, elabel_info->manufacturer, &length);
    if (err != DSMI_OK) {
        dcmi_set_default_elabel_str(elabel_info->manufacturer, sizeof(elabel_info->manufacturer));
    }
    err = dsmi_dft_get_elable(soc_cpu_device_id, dcmi_elabel_pn, elabel_info->product_name, &length);
    if (err != DSMI_OK) {
        dcmi_set_default_elabel_str(elabel_info->product_name, sizeof(elabel_info->product_name));
    }
    err = dsmi_dft_get_elable(soc_cpu_device_id, dcmi_elabel_model, elabel_info->model, &length);
    if (err != DSMI_OK) {
        dcmi_set_default_elabel_str(elabel_info->model, sizeof(elabel_info->model));
    }
    return dcmi_convert_error_code(err);
}

static int dcmi_host_cpu_get_elabel_info(struct dcmi_elabel_info *elabel_info)
{
    int err;
    int dcmi_elabel_sn = 0;
    int dcmi_elabel_mf = 0;
    int dcmi_elabel_pn = 0;
    int dcmi_elabel_model = 0;
    unsigned short length = 0;

    dcmi_get_elabel_item_it(&dcmi_elabel_sn, &dcmi_elabel_mf, &dcmi_elabel_pn, &dcmi_elabel_model);
    err = dcmi_elabel_get_data(
        dcmi_elabel_sn, (unsigned char *)elabel_info->serial_number, MAX_LENTH, &length);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call driver_elabel_get_data failed. err is %d.", err);
        dcmi_set_default_elabel_str(elabel_info->serial_number, sizeof(elabel_info->serial_number));
    }
    err = dcmi_elabel_get_data(dcmi_elabel_mf, (unsigned char *)(elabel_info->manufacturer),
                               MAX_LENTH, &length);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call driver_elabel_get_data failed. err is %d.", err);
        dcmi_set_default_elabel_str(elabel_info->manufacturer, sizeof(elabel_info->manufacturer));
    }
    err = dcmi_elabel_get_data(dcmi_elabel_pn, (unsigned char *)(elabel_info->product_name),
                               MAX_LENTH, &length);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call driver_elabel_get_data failed. err is %d.", err);
        dcmi_set_default_elabel_str(elabel_info->product_name, sizeof(elabel_info->product_name));
    }
    err = dcmi_elabel_get_data(dcmi_elabel_model, (unsigned char *)(elabel_info->model),
                               MAX_LENTH, &length);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call driver_elabel_get_data failed. err is %d.", err);
        dcmi_set_default_elabel_str(elabel_info->model, sizeof(elabel_info->model));
    }
    return err;
}

int dcmi_cpu_get_device_elabel_info(int card_id, struct dcmi_elabel_info *elabel_info)
{
#ifndef _WIN32
    if (dcmi_board_type_is_soc_develop()) {
        return dcmi_soc_cpu_get_elabel_info(elabel_info);
    } else {
        return dcmi_host_cpu_get_elabel_info(elabel_info);
    }
#else
    return DCMI_OK;
#endif
}