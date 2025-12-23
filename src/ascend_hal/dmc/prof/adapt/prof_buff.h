/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef PROF_BUFF_H
#define PROF_BUFF_H
#include "ascend_hal_define.h"

drvError_t prof_buff_init(uint32_t chan_id, uint8_t **buff);
void prof_buff_uninit(uint8_t **buff);

drvError_t prof_buff_write(uint8_t *buff, void *data, uint32_t data_len);
int prof_buff_read(uint8_t *buff, char *out_buf, uint32_t buf_size);

uint32_t prof_buff_get_data_len(uint8_t *buff);
uint32_t prof_buff_get_avail_len(uint8_t *buff);

void *prof_buff_get_buf_addr(uint8_t *buff);
uint32_t prof_buff_get_buf_size(uint8_t *buff);

void *prof_buff_get_readptr_addr(uint8_t *buff);
uint32_t prof_buff_get_readptr_size(uint8_t *buff);

uint32_t prof_buff_get_writeptr(uint8_t *buff);
void prof_buff_update_writeptr(uint8_t *buff, uint32_t write_ptr);

void prof_buff_wait_read_empty(uint8_t *buff, uint32_t dev_id, uint32_t chan_id);

#endif
