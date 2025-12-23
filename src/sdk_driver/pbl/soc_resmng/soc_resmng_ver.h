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
#ifndef SOC_RESMNG_VER_H
#define SOC_RESMNG_VER_H

#define SOC_VERSION_MAJOR_POS 16
#define SOC_VERSION_MINOR_POS 8
/* 
Major version + minor version + sub version. 
0x010000 means: Major version 0x01, minor version: 0x00, sub version: 0x00.
*/
#define SOC_VERSION_DEV_MAJOR 1
#define SOC_VERSION_DEV_MINOR 0
#define SOC_VERSION_DEV_SUB 0
#define SOC_DRIVER_DEV_VERSION (SOC_VERSION_DEV_MAJOR << SOC_VERSION_MAJOR_POS) | \
                               (SOC_VERSION_DEV_MINOR << SOC_VERSION_MINOR_POS) | \
                                SOC_VERSION_DEV_SUB

#define SOC_VERSION_HOST_MAJOR 1
#define SOC_VERSION_HOST_MINOR 0
#define SOC_VERSION_HOST_SUB 0
#define SOC_DRIVER_HOST_VERSION (SOC_VERSION_HOST_MAJOR << SOC_VERSION_MAJOR_POS) | \
                                (SOC_VERSION_HOST_MINOR << SOC_VERSION_MINOR_POS) | \
                                 SOC_VERSION_HOST_SUB     

#endif