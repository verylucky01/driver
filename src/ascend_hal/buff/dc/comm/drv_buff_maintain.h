/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DRV_BUFF_MAINTAIN_H
#define DRV_BUFF_MAINTAIN_H

#include "ascend_hal_error.h"

#include "drv_buff_adp.h"

drvError_t buff_get_mbuf_use_info(struct buff_get_info_handle_arg *para_in);
drvError_t buff_get_mbuf_type_info(struct buff_get_info_handle_arg *para_in);
drvError_t buff_get_buff_type_info(struct buff_get_info_handle_arg *para_in);
drvError_t buff_get_mempool_info(struct buff_get_info_handle_arg *para_in);
drvError_t buff_get_mempool_blk_available(struct buff_get_info_handle_arg *para_in);

#endif
