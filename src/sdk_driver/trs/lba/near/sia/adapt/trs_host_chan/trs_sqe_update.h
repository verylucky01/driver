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
#ifndef TRS_SQE_UPDATE_H
#define TRS_SQE_UPDATE_H

int trs_sqe_update_desc_create(u32 devid, u32 tsid, struct trs_dma_desc_addr_info *addr_info,
                               struct trs_dma_desc *dma_desc, bool is_src_secure);

#endif
