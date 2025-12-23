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
#ifndef TRS_SHR_ID_SPOD_EVENT_UPDATE_H
#define TRS_SHR_ID_SPOD_EVENT_UPDATE_H

int shr_id_spod_event_update(unsigned int devid, struct sched_published_event_info *event_info,
                             struct sched_published_event_func *event_func);
#endif
