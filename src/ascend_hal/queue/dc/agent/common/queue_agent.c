/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DRV_HOST
#include "drv_buff_unibuff.h"
#include "buff_range.h"
#include "queue.h"
#include "queue_agent.h"

#endif

#ifndef DRV_HOST
static int queue_copy(char *dst, unsigned long long dst_max, const char *src,
    unsigned long long src_max, unsigned long long *copy_len)
{
    unsigned long long remain_len, per_copy_len;
    unsigned long long copied_len = 0;
    int ret = -1;

    *copy_len = (dst_max < src_max) ? dst_max : src_max;
    remain_len = *copy_len;

    while (remain_len > 0) {
        per_copy_len = (remain_len > SECUREC_MEM_MAX_LEN) ? SECUREC_MEM_MAX_LEN : remain_len;
        ret = memcpy_s((void *)(dst + copied_len), per_copy_len,
            (void *)(src + copied_len), per_copy_len);
        if (ret != 0) {
            return ret;
        }
        copied_len += per_copy_len;
        remain_len -= per_copy_len;
    }

    return ret;
}

drvError_t queue_mbuf_copy(struct iovec_info *ptr, unsigned int count, Mbuf *mbuf,
    QUEUE_MEM_COPY_DIRECTION direction)
{
    unsigned long long copied_len = 0;
    unsigned long long copy_len;
    unsigned int i;
    int ret;

    for (i = 0; i < count; i++) {
        if (direction == QUEUE_COPY_FROM_MBUF) {
            ret = queue_copy((char *)ptr[i].iovec_base, ptr[i].len, mbuf->data + copied_len,
                mbuf->data_len - copied_len, &copy_len);
        } else {
            ret = queue_copy(mbuf->data + copied_len, mbuf->total_len - copied_len,
                ptr[i].iovec_base, ptr[i].len, &copy_len);
        }
        if (ret != 0) {
            QUEUE_LOG_ERR("memcpy mbuf data failed. (i=%u; ret=%d)\n", i, ret);
            return DRV_ERROR_INNER_ERR;
        }
        copied_len += copy_len;
    }

    return DRV_ERROR_NONE;
}
#ifndef EMU_ST
int que_bare_copy(unsigned long long src, unsigned long long dst, unsigned long long size)
{
    unsigned long long remain_size = size;
    unsigned long long copied_size = 0;
 
    while (remain_size > 0) {
        unsigned long long copy_size = (remain_size > SECUREC_MEM_MAX_LEN) ? SECUREC_MEM_MAX_LEN : remain_size;
        int ret = memcpy_s((void *)(dst + copied_size), copy_size, (void *)(src + copied_size), copy_size);
        if (ret != EOK) {
            return DRV_ERROR_BAD_ADDRESS;
        }
        copied_size += copy_size;
        remain_size -= copy_size;
    }
 
    return DRV_ERROR_NONE;
}
#endif
#endif
