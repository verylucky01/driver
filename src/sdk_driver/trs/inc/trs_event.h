/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef TRS_EVENT_H_
#define TRS_EVENT_H_

#include "ka_system_pub.h"

#include "ascend_hal_define.h"
#include "trs_uk_msg.h"

static inline int trs_event_kerror_to_uerror(int kerror)
{
    switch (kerror) {
        case 0:
            return DRV_ERROR_NONE;
        case -EINVAL:
            return DRV_ERROR_INVALID_VALUE;
        case -EACCES:
            return DRV_ERROR_OPER_NOT_PERMITTED;
        default:
            return DRV_ERROR_IOCRL_FAIL;
    }
}

#endif
