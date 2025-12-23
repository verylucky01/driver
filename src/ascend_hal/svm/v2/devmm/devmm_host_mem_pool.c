/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "devmm_svm.h"
#include "devmm_virt_interface.h"
#include "devmm_host_mem_pool.h"

#define DEVMM_HOST_MEM_POOL_LEVEL_CNT 3

struct devmm_host_mem_pool_cfg {
    uint64_t size;
    uint64_t num;
};

struct devmm_host_mem_node {
    struct devmm_virt_list_head list;
    void *va;
    uint64_t size;
};

struct devmm_host_mem_pool {
    struct devmm_host_mem_pool_cfg cfg;
    struct devmm_virt_list_head head;
    pthread_mutex_t mutex;
    DVdeviceptr pool_start_va;
    void *node;
};

static struct devmm_host_mem_pool g_pool[DEVMM_MAX_PHY_DEVICE_NUM][DEVMM_HOST_MEM_POOL_LEVEL_CNT] = {0};
static uint32_t g_used_node_num[DEVMM_MAX_PHY_DEVICE_NUM][DEVMM_HOST_MEM_POOL_LEVEL_CNT] = {{0}};
static bool g_pool_inited[DEVMM_MAX_PHY_DEVICE_NUM] = {false};
pthread_mutex_t g_pool_init_mutex[DEVMM_MAX_PHY_DEVICE_NUM];

static void devmm_host_mem_pool_list_uninit(uint32_t devid, struct devmm_host_mem_pool *mng)
{
    (void)devid;
    SVM_INIT_LIST_HEAD(&mng->head);
    if ((mng->node != NULL) && (mng->pool_start_va != 0)) {
        free(mng->node);
        mng->node = NULL;
        (void)devmm_free_managed(mng->pool_start_va);
        mng->pool_start_va = 0;
    }
}

DVresult devmm_host_mem_pool_uninit(uint32_t devid)
{
    uint32_t i;

#ifndef EMU_ST
    if (devid >= DEVMM_MAX_PHY_DEVICE_NUM) {
        return DRV_ERROR_NONE;
    }
#endif

    (void)pthread_mutex_lock(&g_pool_init_mutex[devid]);
    if (g_pool_inited[devid] == false) {
        (void)pthread_mutex_unlock(&g_pool_init_mutex[devid]);
        return DRV_ERROR_NONE;
    }

    for (i = 0; i < DEVMM_HOST_MEM_POOL_LEVEL_CNT; ++i) {
        (void)pthread_mutex_lock(&g_pool[devid][i].mutex);
        if (g_used_node_num[devid][i] != 0) {
            (void)pthread_mutex_unlock(&g_pool[devid][i].mutex);
#ifndef EMU_ST
            (void)pthread_mutex_unlock(&g_pool_init_mutex[devid]);
#endif
            DEVMM_DRV_ERR("devmm host mem pool uninit failed. (devid=%u)\n", devid);
            return DRV_ERROR_BUSY;
        }
        devmm_host_mem_pool_list_uninit(devid, &g_pool[devid][i]);
        (void)pthread_mutex_unlock(&g_pool[devid][i].mutex);
    }

    g_pool_inited[devid] = false;
    (void)pthread_mutex_unlock(&g_pool_init_mutex[devid]);

    DEVMM_RUN_INFO("devmm host mem pool uninit success. (devid=%u)\n", devid);
    return DRV_ERROR_NONE;
}

static DVresult devmm_host_mem_pool_list_init(uint32_t devid, struct devmm_host_mem_pool *mng)
{
    struct devmm_host_mem_node *node = NULL;
    DVdeviceptr pool_start_va = 0;
    DVmem_advise advise = 0;
    uint32_t i;
    DVresult ret;

    (void)devid;
    advise |= DV_ADVISE_NOCACHE;
    devmm_set_module_id_to_advise(APP_MODULE_ID, &advise);
    ret = devmm_alloc_proc(0, SUB_HOST_TYPE, advise, mng->cfg.num * mng->cfg.size, &pool_start_va);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    node = malloc(sizeof(struct devmm_host_mem_node) * mng->cfg.num);
    if (node == NULL) {
        DEVMM_DRV_ERR("Alloc memory failed. (cfg.num=%u)\n", mng->cfg.num);
        (void)devmm_free_managed(pool_start_va);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    for (i = 0; i < mng->cfg.num; ++i) {
        node[i].va = (void *)(uintptr_t)(pool_start_va + mng->cfg.size * i);
        node[i].size = mng->cfg.size;
        SVM_INIT_LIST_HEAD(&node[i].list);
        (void)pthread_mutex_lock(&mng->mutex);
        devmm_virt_list_add(&node[i].list, &mng->head);
        (void)pthread_mutex_unlock(&mng->mutex);
    }

    mng->pool_start_va = pool_start_va;
    mng->node = node;
    return DRV_ERROR_NONE;
}

void devmm_host_mem_pool_init(uint32_t devid)
{
    struct devmm_host_mem_pool_cfg info[DEVMM_HOST_MEM_POOL_LEVEL_CNT] = {
        {.size = 128, .num = 10000},
        {.size = 4096, .num = 1000},
        {.size = 2097152, .num = 2}
    };
    uint32_t i, j;
    DVresult ret;

#ifndef EMU_ST
    if (devid >= DEVMM_MAX_PHY_DEVICE_NUM) {
        return;
    }
#endif

    (void)pthread_mutex_lock(&g_pool_init_mutex[devid]);
    if (g_pool_inited[devid] == true) {
        (void)pthread_mutex_unlock(&g_pool_init_mutex[devid]);
        return;
    }

    for (i = 0; i < DEVMM_HOST_MEM_POOL_LEVEL_CNT; ++i) {
        SVM_INIT_LIST_HEAD(&g_pool[devid][i].head);
        g_pool[devid][i].cfg = info[i];
        ret = devmm_host_mem_pool_list_init(devid, &g_pool[devid][i]);
        if (ret != DRV_ERROR_NONE) {
            for (j = 0; j < i; ++j) {
                devmm_host_mem_pool_list_uninit(devid, &g_pool[devid][j]);
            }
            (void)pthread_mutex_unlock(&g_pool_init_mutex[devid]);
            return;
        }
    }

    g_pool_inited[devid] = true;
    (void)pthread_mutex_unlock(&g_pool_init_mutex[devid]);

    DEVMM_RUN_INFO("devmm host mem pool init success. (devid=%u)\n", devid);
}

void *devmm_host_mem_pool_get(uint32_t devid, size_t size, void **cache_va)
{
    struct devmm_host_mem_node *node = NULL;
    uint32_t i;

#ifndef EMU_ST
    if ((devid >= DEVMM_MAX_PHY_DEVICE_NUM) || (g_pool_inited[devid] == false) || (size == 0) || (size > g_pool[devid][DEVMM_HOST_MEM_POOL_LEVEL_CNT - 1].cfg.size)) {
        return NULL;
    }
#endif

    for (i = 0; i < DEVMM_HOST_MEM_POOL_LEVEL_CNT; ++i) {
        if (size <= g_pool[devid][i].cfg.size) {
            (void)pthread_mutex_lock(&g_pool[devid][i].mutex);
            if (devmm_virt_list_empty(&g_pool[devid][i].head) == 0) {
                node = devmm_virt_list_first_entry(&g_pool[devid][i].head, struct devmm_host_mem_node, list);
                devmm_virt_list_del(&node->list);
                *cache_va = node->va;
                g_used_node_num[devid][i]++;
                (void)pthread_mutex_unlock(&g_pool[devid][i].mutex);
                return node;
            }
            (void)pthread_mutex_unlock(&g_pool[devid][i].mutex);
        }
    }

    return NULL;
}

void devmm_host_mem_pool_put(uint32_t devid, void *fd)
{
    struct devmm_host_mem_node *node = (struct devmm_host_mem_node *)fd;
    uint32_t i;

    if (node == NULL) {
        return;
    }

    for (i = 0; i < DEVMM_HOST_MEM_POOL_LEVEL_CNT; ++i) {
        if (node->size == g_pool[devid][i].cfg.size) {
            (void)pthread_mutex_lock(&g_pool[devid][i].mutex);
            devmm_virt_list_add(&node->list, &g_pool[devid][i].head);
            g_used_node_num[devid][i]--;
            (void)pthread_mutex_unlock(&g_pool[devid][i].mutex);
            return;
        }
    }
}

void devmm_restore_host_mem_pool(void)
{
    struct devmm_virt_heap_mgmt *mgmt = devmm_virt_get_heap_mgmt();
    uint32_t i;

    for (i = 0; i < DEVMM_MAX_PHY_DEVICE_NUM; i++) {
        if ((mgmt != NULL) && mgmt->is_dev_inited[i] && mgmt->support_host_mem_pool) {
            devmm_host_mem_pool_init(i);
        }
    }
}

__attribute__((constructor)) static void devmm_host_mem_pool_mutex_init(void)
{
    uint32_t i, j;

    for (i = 0; i < DEVMM_MAX_PHY_DEVICE_NUM; ++i) {
        pthread_mutex_init(&g_pool_init_mutex[i], NULL);
        for (j = 0; j < DEVMM_HOST_MEM_POOL_LEVEL_CNT; ++j) {
            pthread_mutex_init(&g_pool[i][j].mutex, NULL);
        }
    }
}
