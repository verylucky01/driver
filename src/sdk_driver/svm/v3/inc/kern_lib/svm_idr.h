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
#ifndef SVM_IDR_H
#define SVM_IDR_H

#include "ka_base_pub.h"

struct svm_idr {
    ka_idr_t idr;
    int next_id;
    int max_id;
};

void svm_idr_init(int max_id, struct svm_idr *idr);
void svm_idr_uninit(struct svm_idr *idr, void (*release_priv)(void *priv), bool try_sched);
int svm_idr_alloc(struct svm_idr *idr, void *ptr, int *id_out);
void *svm_idr_remove(struct svm_idr *idr, int id);

#endif
