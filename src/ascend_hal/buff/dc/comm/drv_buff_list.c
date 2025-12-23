/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include "drv_buff_common.h"
#include "drv_buff_unibuff.h"
#include "drv_buff_memzone.h"
#include "drv_buff_adp.h"
#include "ascend_hal.h"
#include "drv_user_common.h"

#ifndef EMU_ST
static struct list_head g_grp_mng_list = LIST_HEAD_INIT(g_grp_mng_list);
#else
struct list_head THREAD g_grp_mng_list;
#endif

pthread_mutex_t g_grp_mng_list_muetx = PTHREAD_MUTEX_INITIALIZER;

/*lint -e454 -e455 -e456*/
struct list_head *buff_get_grp_mng_list(void)
{
#ifdef EMU_ST
    buff_grp_mng_list_init();
#endif

    return &g_grp_mng_list;
}

struct buff_grp_list_node *buff_create_grp_list_node(int devid, int grp_id)
{
    unsigned int i;
    struct buff_grp_list_node *list_node = NULL;

    list_node = malloc(sizeof(struct buff_grp_list_node));
    if (list_node == NULL) {
        buff_err("buff list node alloc failed\n");
        return NULL;
    }

    (void)memset_s(list_node, sizeof(struct buff_grp_list_node), 0, sizeof(struct buff_grp_list_node));

    INIT_LIST_HEAD(&list_node->list);
    for (i = 0; i < (unsigned int)BUFF_MAX_MZ_NODE_NUM; i++) {
        INIT_LIST_HEAD(&list_node->mz_list_node[i].list);
        list_node->mz_list_node[i].cfg_id = i;
        if (pthread_rwlock_init(&list_node->mz_list_node[i].mz_list_mutex, NULL) != 0) {
            free(list_node);
            list_node = NULL;
            buff_err("Pthread mutex init failed. (i=%u)\n", i);
            return NULL;
        }
    }
    memzone_elastic_cfg_init(list_node->mz_list_node);

    list_node->grp_id = grp_id;
    list_node->dev_id = devid;

    drv_user_list_add_tail(&list_node->list, buff_get_grp_mng_list());

    buff_info("create grp list node success\n");

    return list_node;
}

void buff_del_grp_list_node(struct buff_grp_list_node *node)
{
    int i;
    struct buff_memzone_list_node *mz_list_node = NULL;
    struct memzone_user_mng_t *mz = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;

    drv_user_list_del(&node->list);
    for (i = 0; i < BUFF_MAX_MZ_NODE_NUM; i++) {
        mz_list_node = &node->mz_list_node[i];
        if (mz_list_node == NULL) {
            continue;
        }
        (void)pthread_rwlock_wrlock(&mz_list_node->mz_list_mutex);
        list_for_each_safe(pos, n, &(mz_list_node->list)) {
            mz = list_entry(pos, struct memzone_user_mng_t, user_list);
            if (mz != NULL) {
                if (mz_list_node->latest_mz == mz) {
                    mz_list_node->latest_mz = NULL;
                }
                drv_user_list_del(&mz->user_list);
                buff_api_atomic_dec(&mz->mz_list_node->mz_cnt);
                memzone_delete(mz);
            }
        }
        (void)pthread_rwlock_unlock(&mz_list_node->mz_list_mutex);
    }

    free(node);
}

void buff_grp_list_clear(void)
{
    struct buff_grp_list_node *grp_node = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;
    struct list_head *head = buff_get_grp_mng_list();

    (void)pthread_mutex_lock(&g_grp_mng_list_muetx);

    list_for_each_safe(pos, n, head) {
        grp_node = list_entry(pos, struct buff_grp_list_node, list);
        if (grp_node != NULL) {
            buff_del_grp_list_node(grp_node);
        }
    }
    (void)pthread_mutex_unlock(&g_grp_mng_list_muetx);
}

struct buff_memzone_list_node* buff_get_buff_mng_list(int devid, int grp, uint32_t list_idx)
{
    struct buff_grp_list_node *grp_node = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;
    struct buff_memzone_list_node *list_node = NULL;
    struct list_head *head = buff_get_grp_mng_list();

    (void)pthread_mutex_lock(&g_grp_mng_list_muetx);

    list_for_each_safe(pos, n, head)
    {
        grp_node = list_entry(pos, struct buff_grp_list_node, list);
        if ((grp_node != NULL) && (grp_node->grp_id == grp) && (grp_node->dev_id == devid)) {
            list_node = &grp_node->mz_list_node[list_idx];
            goto exit;
        }
    }
    grp_node = buff_create_grp_list_node(devid, grp);
    if (grp_node != NULL) {
        list_node = &grp_node->mz_list_node[list_idx];
    }
exit:
    (void)pthread_mutex_unlock(&g_grp_mng_list_muetx);
    return list_node;
}

int buff_get_devid_by_mz_list(struct buff_memzone_list_node *mz_list_node)
{
    struct buff_grp_list_node *grp_node = container_of(mz_list_node, struct buff_grp_list_node,
        mz_list_node[mz_list_node->cfg_id]);

    return grp_node->dev_id;
}

