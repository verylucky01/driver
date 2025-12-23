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

#ifndef __MODULE_HOST_INIT_H__
#define __MODULE_HOST_INIT_H__

#include "virtmnghost_pci.h"

#ifndef STATIC
#ifdef VIRTMNG_UT
#define STATIC
#else
#define STATIC static
#endif
#endif

#endif