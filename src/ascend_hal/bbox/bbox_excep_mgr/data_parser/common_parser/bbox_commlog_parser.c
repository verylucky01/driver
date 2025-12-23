/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "bbox_commlog_parser.h"

#include "securec.h"

#include "bbox_int.h"
#include "bbox_print.h"
#include "bbox_ddr_int.h"
#include "bbox_common_parser.h"

enum Loop_buf_bit_map {
    LPBF_BIT_INVAL  = 0, // invalid
    LPBF_BIT_RD_PTR = 1 << 0, // read ptr
    LPBF_BIT_WR_PTR = 1 << 1, // write ptr
    LPBF_BIT_BUFLEN = 1 << 2, // buffer length
    // last one, use as map mask
    LPBF_BIT_MASK   = (1 << 3) - 1 // 3 bits mask
};

typedef struct loop_buf_head {
    u64 read_ptr;
    u64 write_ptr;
    u64 buf_len;
    u64 data_len;
    u64 rollback;
    u64 bit_map;
    u64 total_len;
    const char *buffer;
} loop_buf_t;

enum list_log_bit_map {
    LSTLOG_BIT_INVAL = 0,
    LSTLOG_BIT_MAGIC = 1 << 0,
    LSTLOG_BIT_CHECK = 1 << 1,
    LSTLOG_BIT_HEAD_STEP = 1 << 2,
    LSTLOG_BIT_STR_START = 1 << 3,
    LSTLOG_BIT_SBUF_LENG = 1 << 4,
    LSTLOG_BIT_NEXT_HEAD = LSTLOG_BIT_SBUF_LENG,
    // last one, use as map mask
    LSTLOG_BIT_MASK = (1 << 5) - 1 // 5 bits mask
};

struct val_pos {
    u32 offset;
    u32 size;
    u64 val;
};

typedef struct list_log_head {
    u64 magic;     // check wether a valid list log struct head (const)
    u64 head_step;  // jump head_step * n for looking for next head when rollback (const)
    u64 str_ptr;    // start offset of printable log string (normally equal to list log head struct length)
    struct val_pos chk_point;  // the value get from the parsed data to be checked with the magic
    struct val_pos str_len;    // printable log string length
    struct val_pos str_buf_len; // printable log string buffer length (aligned)
    struct val_pos next_head;  // the start offset of the next list log struct head (alternative to str_ptr + strbuf_len)
    u64 bit_map;
    u64 total_len;
    const char *buffer;
} list_log_t;

STATIC_INLINE s32 bbox_comm_log_get_val_sz(const char *data, u64 length, u32 offset, u32 size, u64 *number)
{
    if (((u64)offset + size) > length) {
        return BBOX_FAILURE;
    }
    return bbox_get_val_with_size(data + offset, size, number);
}

static inline s32 bbox_comm_log_get_val(const char *data, u64 length, u32 offset, u32 size, u64 *number)
{
    if (size == 0) {
        *number = offset;
        return BBOX_SUCCESS;
    }
    return bbox_comm_log_get_val_sz(data, length, offset, size, number);
}

static inline s32 bbox_comm_log_get_val_pos(const char *data, u64 length, u32 offset, u32 size, struct val_pos *number)
{
    if (((u64)offset + size) > length) {
        return BBOX_FAILURE;
    }
    number->offset = offset;
    number->size = size;
    return bbox_get_val_with_size(data + number->offset, number->size, &number->val);
}

/**
 * @brief       check list log header parameter
 * @param [in]  head:       the log header
 * @return      0: success, other: failed
 */
static bbox_status bbox_list_log_chk_head(const list_log_t *head)
{
    if ((head->str_ptr >= head->total_len) || (head->head_step == 0) || (head->head_step > head->total_len)) {
#ifndef BBOX_UT_TEST
        BBOX_WAR("Invalid list_log header, (str_ptr_over_len=%d, head_step_is_null=%d, head_step_over_len=%d.",
            (int)(head->str_ptr >= head->total_len), (int)(head->head_step == 0),
            (int)(head->head_step > head->total_len));
#endif
        return BBOX_FAILURE;
    }
    return BBOX_SUCCESS;
}

/**
 * @brief       parse list log header data
 * @param [in]  data:       data buffer
 * @param [in]  length:     data buffer length
 * @param [in]  table:      plaintext map table item
 * @param [in]  num:        number of table items
 * @param [in]  args:       custom args used as list_log_t *head
 * @return      0: success, other: failed
 */
STATIC s32 bbox_list_log_parse_head(const char *data, u64 length, const model_elem_t *table, u32 num, void *args)
{
    s32 ret = BBOX_SUCCESS;
    list_log_t *head = (list_log_t *)args;
    u32 i;

    BBOX_CHK_NULL_PTR(head, return BBOX_FAILURE);
    head->buffer = data;
    head->total_len = length;
    head->bit_map = 0;

    for (i = 0; i < num; i++) {
        u32 bit = LSTLOG_BIT_INVAL;
        if (table[i].type == ELEM_CTRL_LSTLOG_MAGIC) {
            ret = bbox_comm_log_get_val(data, length, table[i].offset, table[i].size, &head->magic);
            bit = LSTLOG_BIT_MAGIC;
        } else if (table[i].type == ELEM_CTRL_LSTLOG_CHECK) {
            ret = bbox_comm_log_get_val_pos(data, length, table[i].offset, table[i].size, &head->chk_point);
            bit = LSTLOG_BIT_CHECK;
        } else if (table[i].type == ELEM_CTRL_LSTLOG_STRPTR) {
            ret = bbox_comm_log_get_val(data, length, table[i].offset, table[i].size, &head->str_ptr);
            bit = LSTLOG_BIT_STR_START;
        } else if (table[i].type == ELEM_CTRL_LSTLOG_STRLEN) {
            ret = bbox_comm_log_get_val_pos(data, length, table[i].offset, table[i].size, &head->str_len);
        } else if (table[i].type == ELEM_CTRL_LSTLOG_BUFLEN) {
            ret = bbox_comm_log_get_val_pos(data, length, table[i].offset, table[i].size, &head->str_buf_len);
            bit = LSTLOG_BIT_SBUF_LENG;
        } else if (table[i].type == ELEM_CTRL_LSTLOG_NEXT) {
            ret = bbox_comm_log_get_val_pos(data, length, table[i].offset, table[i].size, &head->next_head);
            bit = LSTLOG_BIT_NEXT_HEAD;
        } else if (table[i].type == ELEM_CTRL_LSTLOG_STEP) {
            ret = bbox_comm_log_get_val(data, length, table[i].offset, table[i].size, &head->head_step);
            bit = LSTLOG_BIT_HEAD_STEP;
        } else {
            ;
        }
        if (ret != BBOX_SUCCESS) {
            BBOX_ERR_CTRL(BBOX_ERR, return ret, "invalid listlog item[%u][%s]: offset[%u], size[%u], length[%llu].",
                i, table[i].name, table[i].offset, table[i].size, length);
        }
        head->bit_map |= bit;
    }
    if (head->bit_map != (u64)LSTLOG_BIT_MASK) {
        BBOX_ERR_CTRL(BBOX_ERR, return BBOX_FAILURE, "list_log header bitmap[%llu] check failed.", head->bit_map);
    }
    return bbox_list_log_chk_head(head);
}

static inline u64 bbox_list_log_get_valid_next_step(u64 next_val, u64 head_step)
{
    return ((next_val != 0) && (head_step != 0) && ((next_val % head_step) == 0)) ? next_val : head_step;
}

/**
 * @brief       parse list log data and write data to file
 * @param [in]  file_hdl:    path struct for write file
 * @param [in]  data:       data buffer
 * @param [in]  length:     data buffer length
 * @param [in]  args:       custom args used as list_log_t *head
 * @return      0: success, other: failed
 */
STATIC s32 bbox_list_log_save_data(const file_hdl_t *file_hdl, const char *data, u32 length, void *args)
{
    s32 ret = BBOX_SUCCESS;
    list_log_t *head = (list_log_t *)args;
    u64 step = 0;
    s64 tmp_len;

    BBOX_CHK_NULL_PTR(head, return BBOX_FAILURE);
    BBOX_CHK_NULL_PTR(data, return BBOX_FAILURE);
    BBOX_CHK_NULL_PTR(file_hdl, return BBOX_FAILURE);

    const char *data_ptr = data;
    struct val_pos *next_head = (head->next_head.size != 0) ? &head->next_head : &head->str_buf_len;
    struct val_pos *str_buf_len = (head->str_buf_len.size != 0) ? &head->str_buf_len : &head->next_head;
    struct val_pos *str_len = (head->str_len.size != 0) ? &head->str_len : str_buf_len;
    u64 addition = (head->next_head.size != 0) ? 0U : head->str_ptr;

    for (tmp_len = (s64)length; tmp_len > (s64)head->str_ptr; tmp_len -= (s64)step) {
        data_ptr += step;
        step = head->head_step;
        ret = bbox_comm_log_get_val_sz(data_ptr, (u64)tmp_len, head->chk_point.offset,
                                  head->chk_point.size, &head->chk_point.val);
        if ((ret != BBOX_SUCCESS) || (head->chk_point.val != head->magic)) {
            continue;
        }
        ret = bbox_comm_log_get_val_sz(data_ptr, (u64)tmp_len, str_len->offset, str_len->size, &str_len->val);
        if ((ret != BBOX_SUCCESS) || (str_len->val >= ((u64)tmp_len - head->str_ptr))) {
#ifndef BBOX_UT_TEST
            BBOX_ERR("listlog get str_len failed, ret %d, offset %u, size %u.",
                (int)ret, str_len->offset, str_len->size);
#endif
            continue;
        }

        u64 slen = strlen(data_ptr + (s64)head->str_ptr);
        u64 len = RDR_MIN(slen, str_len->val);
        ret = bbox_save_buf_to_fs(file_hdl->path, file_hdl->name, data_ptr + head->str_ptr, (u32)len, BBOX_TRUE);
        BBOX_CHK_EXPR_ACTION((ret < 0) || ((u32)ret != len), return BBOX_FAILURE,
            "listlog save data failed(%d!=%llu).", ret, len);

        ret = bbox_comm_log_get_val_sz(data_ptr, (u64)tmp_len, next_head->offset, next_head->size, &next_head->val);
        if (ret != BBOX_SUCCESS) {
#ifndef BBOX_UT_TEST
            BBOX_ERR("listlog get next_head fail, ret %d, offset %u, size %u.", (int)ret, str_len->offset, str_len->size);
#endif
            continue;
        }
        step = bbox_list_log_get_valid_next_step(next_head->val + addition, head->head_step);
    }
    return ret;
}

/**
 * @brief       parse char log data and write data to file
 * @param [in]  file_hdl:    path struct for write file
 * @param [in]  type:       data type
 * @param [in]  data:       data buffer
 * @param [in]  length:     data buffer length
 * @param [in]  args:       custom args not used
 * @return      0: success, other: failed
 */
STATIC bbox_status bbox_char_log_save_data(const file_hdl_t *file_hdl, const char *data, u32 length, void *args)
{
    UNUSED(args);
    if (length == 0) {
        BBOX_INF("charlog data is null, save done.");
        return BBOX_SUCCESS;
    }

    s32 ret = bbox_save_buf_to_fs(file_hdl->path, file_hdl->name, data, length, BBOX_TRUE);
    BBOX_CHK_EXPR_ACTION((u32)ret != length, return BBOX_FAILURE, "charlog save data failed(%d!=%u).", ret, length);
    return BBOX_SUCCESS;
}

typedef s32 (*log_save_func_t)(const file_hdl_t *file, const char *buffer, u32 length, void *args);
typedef s32 (*log_parse_func_t)(const char *buffer, u64 length, const model_elem_t *table, u32 num, void *args);

typedef struct log_parser {
    log_save_func_t save_func;
    log_parse_func_t parse_func;
    void *args;
} log_parser_t;

static inline void bbox_loop_buf_save_info_name(const file_hdl_t *file, const model_elem_t *table)
{
    char buffer[TMP_BUFF_S_LEN] = {0};
    s32 len = sprintf_s(buffer, TMP_BUFF_S_LEN, "%s:\n", table->name);
    if (len < 0) {
        BBOX_ERR_CTRL(BBOX_ERR, return, "sprintf_s failed.");
    }
    s32 ret = bbox_save_buf_to_fs(file->path, file->name, buffer, (u32)len, BBOX_TRUE);
    BBOX_CHK_EXPR(ret != len, "save [%s] to file failed.", buffer);
}

/**
 * @brief       parse loop buffer log data and write data to file by using save func
 * @param [in]  file_hdl:    path struct for write file
 * @param [in]  table:      plaintext map table item
 * @param [in]  num:        number of table items
 * @param [in]  head:       loop buffer head for parsing
 * @param [in]  sv_func:     func used to save data buffer
 * @return      0: success, other: failed
 */
STATIC s32 bbox_loop_buf_parse_log(const file_hdl_t *file_hdl, const model_elem_t *table, u32 num, const loop_buf_t *head,
    const log_parser_t *parser)
{
    s32 ret;
    s32 result = BBOX_SUCCESS;
    u64 total_len = (head->buf_len > 0) ? head->buf_len : head->total_len;
    u64 length = ((head->data_len > 0) && (head->data_len < (u64)table->size)) ? head->data_len : (u64)table->size;
    const char *buffer = NULL;
    const char *data = NULL;
    u32 len;

    if (((u64)table->offset + length) > total_len) {
#ifndef BBOX_UT_TEST
        BBOX_ERR_CTRL(BBOX_ERR, return BBOX_FAILURE, "log offset %u add len out of buffer length.",
            table->offset);
#endif
    }
    if ((head->read_ptr > length) || (head->write_ptr > length)) {
#ifndef BBOX_UT_TEST
        BBOX_ERR_CTRL(BBOX_ERR, return BBOX_FAILURE, "log read_ptr out of data length %d, "
            "write_ptr out of data length %d",
            (int)(head->read_ptr > length), (int)(head->write_ptr > length));
#endif
    }
    bbox_loop_buf_save_info_name(file_hdl, table);

    buffer = head->buffer + table->offset;
    if (parser->parse_func != NULL) {
        ret = parser->parse_func(buffer, length, table, num, parser->args);
        BBOX_CHK_EXPR_ACTION(ret != BBOX_SUCCESS, return BBOX_FAILURE, "parse log function return failed.");
    }

    if (head->rollback == 0) {
        data = (head->read_ptr < head->write_ptr) ? (buffer + head->read_ptr) : buffer;
        len = (head->read_ptr < head->write_ptr) ? (u32)(head->write_ptr - head->read_ptr) : (u32)head->write_ptr;
        ret = parser->save_func(file_hdl, data, len, parser->args);
    } else {
        data = (head->read_ptr > head->write_ptr) ? (buffer + head->read_ptr) : (buffer + head->write_ptr);
        len = (head->read_ptr > head->write_ptr) ? (u32)(length - head->read_ptr) : (u32)(length - head->write_ptr);
        ret = parser->save_func(file_hdl, data, len, parser->args);
        result = (ret == BBOX_SUCCESS) ? result : BBOX_FAILURE;

        data = buffer;
        len = (u32)head->write_ptr;
        ret = parser->save_func(file_hdl, data, len, parser->args);
    }
    return (ret == BBOX_SUCCESS) ? result : BBOX_FAILURE;
}

static inline s32 bbox_loop_buf_parse_list_log(const file_hdl_t *file_hdl, const model_elem_t *table,
                                          u32 num, const loop_buf_t *head)
{
    log_parser_t parse;
    list_log_t log_head = {0};

    parse.save_func = bbox_list_log_save_data;
    parse.parse_func = bbox_list_log_parse_head;
    parse.args = (void *)&log_head;
    return bbox_loop_buf_parse_log(file_hdl, table, num, head, &parse);
}

static inline s32 bbox_loop_buf_parse_char_log(const file_hdl_t *file_hdl, const model_elem_t *table,
                                          u32 num, const loop_buf_t *head)
{
    log_parser_t parse = {NULL, NULL, NULL};
    parse.save_func = bbox_char_log_save_data;
    return bbox_loop_buf_parse_log(file_hdl, table, num, head, &parse);
}

static inline s32 bbox_loop_buf_format_head(char *buffer, u32 size, const struct loop_buf_head *head)
{
    s32 ret = sprintf_s(buffer, size,
        "bad loop buffer head info:\n"
        "  read_ptr:  0x%llx\n  write_ptr: 0x%llx\n  buf_len:   0x%llx\n"
        "  data_len:  0x%llx\n  rollback: %llu\n  bit_map:   0x%llx\n"
        "  total_len: 0x%llx\n",
        head->read_ptr, head->write_ptr, head->buf_len, head->data_len, head->rollback, head->bit_map, head->total_len);
    return ((ret <= 0) ? -1 : ret);
}

/**
 * @brief       save loop buffer header to file
 * @param [in]  file:       path struct for write file
 * @param [in]  head:       loop buffer header to save
 * @return      0: success, other: failed
 */
static bbox_status bbox_loop_buf_save_head(const file_hdl_t *file, const loop_buf_t *head)
{
    char buf[TMP_BUFF_S_LEN] = {0};
    s32 len = bbox_loop_buf_format_head(buf, TMP_BUFF_S_LEN, head);
    BBOX_CHK_EXPR_ACTION(len < 0, return BBOX_FAILURE, "bbox_loop_buf_format_head failed.");
    s32 ret = bbox_save_buf_to_fs(file->path, file->name, buf, (u32)len, BBOX_TRUE);
    BBOX_CHK_EXPR_ACTION(ret != len, return BBOX_FAILURE, "return bad saved len %d(!=%d).", ret, len);
    return BBOX_SUCCESS;
}

/**
 * @brief       check loop buffer header parameter
 * @param [in]  head:       the buffer header
 * @return      0: success, other: failed
 */
static inline bbox_status bbox_loop_buf_chk_head(const loop_buf_t *head)
{
    if ((head->data_len > head->total_len) || (head->buf_len > head->total_len) ||
        (head->read_ptr > head->total_len) || (head->write_ptr > head->total_len)) {
#ifndef BBOX_UT_TEST
        BBOX_WAR("Invalid loopbuffer header: data_len over len %d, buf_len over len %d, "
            "read_ptr over len %d, write_ptr over len %d.",
            (int)(head->data_len > head->total_len), (int)(head->buf_len > head->total_len),
            (int)(head->read_ptr > head->total_len), (int)(head->write_ptr > head->total_len));
#endif
        return BBOX_FAILURE;
    }
    return BBOX_SUCCESS;
}

/**
 * @brief       parse loop buffer header data
 * @param [in]  file_hdl:    path struct for write file
 * @param [in]  table:      plaintext map table item
 * @param [in]  num:        number of table items
 * @param [out] head:       parsed log header
 * @return      0: success, other: failed
 */
STATIC s32 bbox_loop_buf_parse_head(const file_hdl_t *file_hdl, const model_elem_t table[], u32 num, loop_buf_t *head)
{
    s32 ret = BBOX_SUCCESS;
    const char *data = head->buffer;
    u64 length = head->total_len;
    s32 i;

    head->bit_map = 0;
    head->rollback = 0;

    for (i = 0; i < (s32)num; i++) {
        u32 bit = LPBF_BIT_INVAL;
        const model_elem_t *element = &table[i];
        if (element->type == ELEM_CTRL_LPBF_READ) {
            ret = bbox_comm_log_get_val(data, length, element->offset, element->size, &head->read_ptr);
            bit = LPBF_BIT_RD_PTR;
        } else if (element->type == ELEM_CTRL_LPBF_WRITE) {
            ret = bbox_comm_log_get_val(data, length, element->offset, element->size, &head->write_ptr);
            bit = LPBF_BIT_WR_PTR;
        } else if (element->type == ELEM_CTRL_LPBF_SIZE) {
            ret = bbox_comm_log_get_val(data, length, element->offset, element->size, &head->buf_len);
            bit = LPBF_BIT_BUFLEN;
        } else if (element->type == ELEM_CTRL_LPBF_D_SIZE) {
            ret = bbox_comm_log_get_val(data, length, element->offset, element->size, &head->data_len);
            bit = LPBF_BIT_BUFLEN;
        } else if (element->type == ELEM_CTRL_LPBF_ROLLBK) {
            ret = bbox_comm_log_get_val(data, length, element->offset, element->size, &head->rollback);
        } else if ((element->type > ELEM_OUTPUT_TYPE) && (element->type < ELEM_OUTPUT_MAX)) {
            ret = bbox_plaintext_out_data(file_hdl, element, 1, data, (u32)length);
        } else {
            ;
        }
        if (ret != BBOX_SUCCESS) {
            BBOX_ERR_CTRL(BBOX_ERR, return ret, "invalid loopbuffer item[%d][%s]: offset[%u], size[%u], length[%llu].",
                i, element->name, element->offset, element->size, length);
        }
        head->bit_map |= bit;
    }
    if (head->bit_map != (u64)LPBF_BIT_MASK) {
        BBOX_ERR_CTRL(BBOX_ERR, return BBOX_FAILURE, "loopbuffer header bitmap[%llu] check failed.", head->bit_map);
    }
    return bbox_loop_buf_chk_head(head);
}

/**
 * @brief       parse common log data and write data to file
 * @param [in]  file_hdl:    path struct for write file
 * @param [in]  table:      plaintext map table item
 * @param [in]  num:        number of table items
 * @param [in]  buffer:     data buffer
 * @param [in]  length:     data buffer length
 * @return      >= 0: success, other: failed
 */
s32 bbox_comm_log_parse_data(const file_hdl_t *file_hdl, const model_elem_t table[], u32 num, const char *buffer, u32 length)
{
    s32 i;
    s32 ret = BBOX_SUCCESS;
    loop_buf_t log_head = {0};

    BBOX_CHK_NULL_PTR(file_hdl, return BBOX_FAILURE);
    BBOX_CHK_NULL_PTR(table, return BBOX_FAILURE);
    BBOX_CHK_NULL_PTR(buffer, return BBOX_FAILURE);
    BBOX_CHK_INVALID_PARAM(length == 0, return BBOX_FAILURE, "%u", length);
    BBOX_CHK_INVALID_PARAM(table[0].type != ELEM_FEATURE_LOOPBUF, return BBOX_FAILURE, "%d", (s32)table[0].type);

    log_head.buffer = buffer;
    log_head.total_len = length;

    for (i = 0; i < (s32)num; i++) {
        const model_elem_t *element = &table[i];
        u32 index_offset = element->index_offset;
        u32 index_cnt = element->index_cnt;
        if (((u64)index_offset + (u64)index_cnt) > (u64)num) {
            BBOX_ERR("loop buffer item offset %u, count %u, out of range %u", index_offset, index_cnt, num);
            ret = BBOX_FAILURE;
            break;
        }

        if (element->type == ELEM_FEATURE_LOOPBUF) {
            ret = bbox_loop_buf_parse_head(file_hdl, &table[i] + index_offset, index_cnt, &log_head);
        } else if (element->type == ELEM_FEATURE_CHARLOG) {
            ret = bbox_loop_buf_parse_char_log(file_hdl, &table[i] + index_offset, index_cnt, &log_head);
        } else if (element->type == ELEM_FEATURE_LISTLOG) {
            ret = bbox_loop_buf_parse_list_log(file_hdl, &table[i] + index_offset, index_cnt, &log_head);
        } else {
            break;
        }
        if (ret != BBOX_SUCCESS) {
            (void)bbox_loop_buf_save_head(file_hdl, &log_head);
            break;
        }
        i += ((s32)index_offset + (s32)index_cnt - 1);
    }
    s32 parse_count = (ret == BBOX_SUCCESS) ? i : BBOX_FAILURE;
    return  parse_count;
}
