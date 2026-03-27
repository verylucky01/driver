/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
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
#ifndef _ASCEND_UB_PAIR_DEV_INFO_H_
#define _ASCEND_UB_PAIR_DEV_INFO_H_

#include "ubcore_types.h"
#include "ubcore_opcode.h"
#include "ubcore_uapi.h"
#include "ubcore_api.h"
#include "ascend_ub_dev.h"
#include "ascend_ub_main.h"
#include "ascend_ub_dev_online_adapt.h"

#define UBDRV_INVALID_PHY_ID 0xFFFFFFFF
#define UBDRV_PAIR_INFO_WORK_DELAY 5000  // ms
#define UBDRV_PAIR_INFO_QUERY_DELAY 15000  // ms
#define BITS_PER_LONG_LONG 64
#define SET_BIT_64(x, y) ((x) |= ((u64)0x01 << (y)))
#define CLR_BIT_64(x, y) ((x) &= (~((u64)0x01 << (y))))
#define CHECK_BIT_64(x, y) ((x) & ((u64)0x01 << (y)))

#define PHY_MACHEINE_ADDR 0xe8000430

#define UBDRV_H2D_FE_FLAG      KA_BASE_BIT(0)  // 0-false    1-true
#define UBDRV_MIA_FLAG         KA_BASE_BIT(1)  // 0-sia dev  1-mia dev
#define UBDRV_VIRTUAL_FLAG     KA_BASE_BIT(2)  // 0-phy dev  1-virtual dev

struct pair_chan_info{
    union ubcore_eid local_eid;
    union ubcore_eid remote_eid;
    u16 flag;  // bit0: this eid is for h2d, bit1:  sia or mia device
    u16 hop : 4;
    u16 rsv : 12;
}__attribute__((packed));

struct pair_dev_info
{
    union ubcore_eid bus_instance_eid;
    u32 dev_id;  // real phy dev_id, of local view peer, must match dev_id == slot_id*8+module_id
    u32 module_id  : 16;  // peer info, bit0~15:module_id
    u32 slot_id    : 16;  // peer info, bit16~31:slot_id
    u32 chan_num;  // chan num. max is 8
    union ubcore_eid d2d_eid;
    struct pair_chan_info chan_info[PAIR_CHAN_MAX_NUM];
}__attribute__((packed));

enum ubdrv_pair_opcode {
    UBDRV_PAIR_OP_ADD = 1,
    UBDRV_PAIR_OP_DEL,
    UBDRV_PAIR_OP_UPDATE,
    UBDRV_PAIR_OP_MAX,
};

struct pair_notice_dev_info
{
    u32 op        : 8;  // 1:add 2:del 3:update
    u32 version   : 8;
    u32 total_num : 8;
    u32 rsv       : 8;
    struct pair_dev_info notice_dev_info;
}__attribute__((packed));

struct ubdrv_pair_info_node {
    ka_hlist_node_t   hnode; /* hash pair_info link node */
    u32 dev_id;  // key
    struct pair_dev_info dev_info;
};

struct ubdrv_pair_info_work_mng {
    u32 work_magic;
    u32 pair_call_valid;
    ka_delayed_work_t pair_info_work;
};

struct ubdrv_del_dev_work {
    u32 dev_id;
    ka_work_struct_t work;
};

int ubdrv_init_h2d_eid_index(u32 dev_id);
void ubdrv_uninit_h2d_eid_index(u32 dev_id);
bool ubdrv_init_pair_info_h2d_eid_index(void);
int ubdrv_get_all_pair_dev_info(void);
void ubdrv_put_all_pair_dev_info(void);
int ubdrv_query_pair_devinfo_by_remote_eid(u32 *dev_id, struct ubcore_eid_info *local_eid,
    struct ubcore_eid_info *remote_eid);
bool ubdrv_is_valid_devid(u32 dev_id);
int ubdrv_get_all_device_count(u32 *count);
int ubdrv_get_urma_info(u32 udev_id, struct ascend_urma_dev_info *urma_info);
int ubdrv_get_device_probe_list(u32 *devids, u32 *count);
void ubdrv_pair_info_init_work(struct ub_idev *idev);
void ubdrv_pair_info_uninit_work(struct ub_idev *idev);
int ubdrv_pair_info_del_process(struct pair_dev_info *dev_info, struct ubdrv_del_dev_work *del_work, u64 *del_device_bitmap);
void ascend_ub_get_min_eid_from_list_proc(struct dev_eid_info *eid_info);
void ascend_ub_get_min_eid_from_list(u32 *idx, struct ubcore_eid_info *eid_list, u32 eid_list_num);
ka_mutex_t *get_global_pair_info_mutex(void);
struct ubdrv_pair_info_node *ubdrv_pair_info_hash_find(u32 dev_id);
bool ubdrv_check_devid(u32 dev_id);
int ubdrv_query_h2d_pair_chan_info(u32 dev_id, struct ubcore_eid_info *local_eid, struct ubcore_eid_info *remote_eid);
#ifdef CFG_FEATURE_SUPPORT_RUN_INSTALL
int ubdrv_procfs_pair_info_show(ka_seq_file_t *seq, void *offset);
#endif
#endif
