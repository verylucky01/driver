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

#ifndef _DMC_PROFILE_H_
#define _DMC_PROFILE_H_

#define SAMPLE_MASK 0x01
#define SAMPLE_ONLY_DATA 0x0
#define SAMPLE_WITH_HEADER 0x1

#define SAMPLE_NAME_MAX 16

#define PROF_PERI_REV_NUM 6
struct prof_peri_para {
    unsigned int device_id;
    unsigned int vfid;
    void *user_data;                /* sample Configuration information */
    unsigned int user_data_len;     /* sample Length of the configuration information data */
    unsigned int sample_flag;       /* Indicates whether it is the first sample. SAMPLE_ONLY_DATA: not the first sample; SAMPLE_WITH_HEADER: the first sample */
    void *buff;                     /* sample buff address */
    unsigned int buff_len;          /* sample buff total length */
    int target_pid;
    unsigned int reserve[PROF_PERI_REV_NUM];
};
#endif /* _DMC_PROFILE_H_ */

