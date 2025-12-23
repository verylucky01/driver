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

#ifndef ADDR_TRANS_H
#define ADDR_TRANS_H

#include <linux/types.h>

struct svm_peer_ub_addr {
    u32 mem_id;
    u64 offset;
};

struct devdrv_addr_desc {
    union {
        struct svm_peer_ub_addr peer_ub;
        u64 pa;
    };
    u64 size;
};

/* h2d, d2h. support latter */
int devdrv_addr_trans_local_to_peer(u32 udevid, struct devdrv_addr_desc *addr_desc, u64 *trans_addr);
int devdrv_addr_trans_peer_to_local(u32 udevid, struct devdrv_addr_desc *addr_desc, u64 *trans_addr);

/* p2p, peer_udevid is host view udevid */
int devdrv_addr_trans_p2p_peer_to_local(u32 udevid, u32 peer_udevid,
    struct devdrv_addr_desc *addr_desc, u64 *trans_addr);

/* p2p, cs: cross server */
int devdrv_addr_trans_cs_p2p_peer_to_local(u32 udevid, u32 peer_sdid,
    struct devdrv_addr_desc *addr_desc, u64 *trans_addr);

#endif

