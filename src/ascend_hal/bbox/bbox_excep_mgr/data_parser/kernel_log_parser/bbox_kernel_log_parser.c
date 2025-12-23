/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "bbox_kernel_log_parser.h"
#include "bbox_kernel_klog_parser.h"
#include "bbox_kernel_rblog_parser.h"

#include "securec.h"
#include "bbox_fs_api.h"
#include "bbox_system_api.h"
#include "bbox_print.h"
#include "bbox_file_list.h"

/**
 * @brief       save char to file
 * @param [in]  text_log:  kernel log
 * @param [in]  text_len:  kernel log length
 * @param [in]  ts_usec:   kernel log time
 * @param [in]  log_path: path to write log
 * @return      length of saved message
 */
s32 bbox_klog_save_buf_to_fs(const char *text_log, u32 text_len, u64 ts_usec, const char *log_path)
{
    u32 buf_len = TMP_BUFF_B_LEN;
    char *buf = NULL;

    BBOX_CHK_NULL_PTR(log_path, return BBOX_FAILURE);
    buf = (char *)bbox_malloc(buf_len);
    if (buf == NULL) {
        BBOX_ERR_CTRL(BBOX_ERR, return BBOX_FAILURE, "malloc failed.");
    }

    // collect timestamp
    s32 ret = snprintf_s(buf, buf_len, buf_len - 1U, "[%5llu.%06llu] ", ts_usec / USEC_PER_SEC, ts_usec % USEC_PER_SEC);
    if (ret < 0) {
        bbox_free(buf);
        BBOX_ERR_CTRL(BBOX_ERR, return BBOX_FAILURE, "snprintf_s failed.");
    }

    u32 char_idx = 0;
    u32 num = ((u32)ret >= (buf_len - 2U)) ? (buf_len - 2U) : (u32)ret; // reserve 2 for '\0' and '\n'
    while ((char_idx < text_len) && (text_log[char_idx] != '\0') && (char_idx < (buf_len - num - 2U))) {
        u32 tmp_idx = num + char_idx;
        buf[tmp_idx] = text_log[char_idx];
        char_idx++;
    }

    if (char_idx > 0U) {
        u32 line_idx = num + char_idx;
        u32 end_idx = line_idx + 1U;
        buf[line_idx] = '\n';
        buf[end_idx] = '\0';
        char_idx++;
        (void)bbox_save_log_buf_to_fs(log_path, BBOX_FILE_NAME_KLOG, buf, (u32)strlen(buf), BBOX_TRUE);
    }
    ret += (s32)char_idx;
    bbox_free(buf);
    return ret;
}

void bbox_klog_save_all_log_buf_to_fs(const void *log_buf, u32 len, const char *log_path)
{
    BBOX_CHK_NULL_PTR(log_path, return);

    s32 ret = bbox_save_log_buf_to_fs(log_path, BBOX_FILE_NAME_KLOG, log_buf, len, BBOX_TRUE);
    BBOX_CHK_EXPR(ret < 0, "failed to save all klog into file %s", BBOX_FILE_NAME_KLOG);
}

/**
 * @brief       save klog original log to file
 * @param [in]  log_path: path to write log
 * @param [in]  buffer  The buffer of kernel log data.
 * @param [in]  len     The length of the buffer.
 */
static inline void bbox_klog_save_bin(const char *log_path, const buff *buffer, u32 len)
{
    s32 ret = bbox_save_log_buf_to_fs(log_path, BBOX_FILE_NAME_KLOG_BIN, buffer, len, BBOX_FALSE);
    BBOX_CHK_EXPR(ret < 0, "failed to save klog info into file %s", BBOX_FILE_NAME_KLOG_BIN);
    return;
}

/**
 * @brief       Interface for dump kernel log into file-system.
 *              The kernel log data may be incorrect, resulting in interpretation failure.
 * @param [in]  buffer  The buffer of kernel log data.
 * @param [in]  len     The length of the buffer.
 * @param [in]  log_path The path for DDRDUMP module dump data into.
 * @return      0 on success otherwise -1
 */
static bbox_status bbox_klog_save_fs(const void *buffer, u32 len, const char *log_path)
{
    const struct printk_area_head *head = (const struct printk_area_head *)buffer;
    bbox_status ret = BBOX_FAILURE;

    if (bbox_klog_is_printk_info_log((const char *)head->manage_data)) {
        ret = bbox_klog_save_fs_printk_info_log(buffer, len, log_path);
    } else if (bbox_klog_is_ring_buffer_log((const char *)head->manage_data)) {
        ret = bbox_klog_save_fs_ring_buffer_log(buffer, len, log_path);
    }

    return ret;
}

/**
 * @brief       Interface for dump kernel log into file-system.
 *              The kernel log data may be incorrect, resulting in interpretation failure.
 * @param [in]  buffer  The buffer of kernel log data.
 * @param [in]  len     The length of the buffer.
 * @param [in]  log_path The path for DDRDUMP module dump data into.
 * @return      0 on success otherwise -1
 */
bbox_status bbox_klog_dump(const void *buffer, u32 len, const char *log_path)
{
    BBOX_CHK_NULL_PTR(buffer, return BBOX_FAILURE);
    BBOX_CHK_NULL_PTR(log_path, return BBOX_FAILURE);
    BBOX_CHK_INVALID_PARAM(len <= (u32)sizeof(struct printk_area_head), return BBOX_FAILURE, "%u", len);

    bbox_status ret = bbox_klog_save_fs(buffer, len, log_path);
    if (ret != BBOX_SUCCESS) {
        BBOX_WAR("Can not parse klog struct, the klog message may not save completely. (ret=%d)", ret);
        bbox_klog_save_bin(log_path, buffer, len);
    }
    return BBOX_SUCCESS;
}
