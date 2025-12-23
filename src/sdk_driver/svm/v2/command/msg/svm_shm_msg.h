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

#ifndef SVM_SHM_MSG_H
#define SVM_SHM_MSG_H
 
#define DEVMM_DATA_SIZE 40
#pragma pack(1)
struct devmm_share_memory_data {
    u32 image_word;
    u16 id;
    u16 vm_id;
    int host_pid;
    u16 data_type;
    u16 vfid;
    char data[DEVMM_DATA_SIZE];
    u64 va;
};
#pragma pack()
 
#endif