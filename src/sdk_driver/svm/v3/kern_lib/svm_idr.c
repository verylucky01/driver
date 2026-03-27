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
#include "ka_base_pub.h"
#include "ka_task_pub.h"
#include "ka_memory_pub.h"
#include "ka_system_pub.h"
#include "ka_sched_pub.h"

#include "svm_idr.h"

void svm_idr_init(int max_id, struct svm_idr *idr)
{
    ka_base_idr_init(&idr->idr);
    idr->next_id = 0;
    idr->max_id = max_id;
}

void svm_idr_uninit(struct svm_idr *idr, void (*release_priv)(void *priv), bool try_sched)
{
    unsigned long stamp = ka_jiffies;
    void *priv = NULL;
    int id;

    ka_base_idr_for_each_entry(&idr->idr, priv, id) {
         /* Unlikely fail */
        (void)ka_base_idr_remove(&idr->idr, id);
        if (release_priv != NULL) {
            release_priv(priv);
        }

        if (try_sched) {
            ka_try_cond_resched(&stamp);
        }
    }

    ka_base_idr_destroy(&idr->idr);
}

int svm_idr_alloc(struct svm_idr *idr, void *ptr, int *id_out)
{
    int id;

    ka_task_might_sleep();
    id = ka_base_idr_alloc(&idr->idr, ptr, idr->next_id, idr->max_id, KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (id < 0) {
        if (idr->next_id != 0) {
            id = ka_base_idr_alloc(&idr->idr, ptr, 0, idr->next_id, KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
        }

        if (id < 0) {
            return id;
        }
    }

    idr->next_id = (id + 1) % idr->max_id;
    *id_out = id;
    return 0;
}

void *svm_idr_remove(struct svm_idr *idr, int id)
{
    void *priv = NULL;

    priv = ka_base_idr_remove(&idr->idr, id);

    return priv;
}
