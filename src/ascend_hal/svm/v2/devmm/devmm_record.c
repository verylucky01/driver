/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <pthread.h>
#include "rbtree.h"
#include "uref.h"
#include "devmm_svm.h"
#include "devmm_record.h"

typedef int (*record_restore)(uint64_t key1, uint64_t key2, uint32_t devid, void *data);
struct devmm_record_mng {
    struct rbtree_root key1_tree;
    struct rbtree_root key2_tree;
    pthread_mutex_t mutex;
    int (*restore)(uint64_t key1, uint64_t key2, uint32_t devid, void *data);
};

struct devmm_record_node {
    struct rb_node key1_node;
    struct rb_node key2_node;
    uint64_t key1;
    uint64_t key2;

    struct uref ref;
    uint32_t devid;
    enum devmm_record_node_status status;
    enum devmm_record_feature_type type;

    uint64_t data_len;
    void *data;
};

#define container_of(ptr, type, member)                    \
    ({                                                     \
        typeof(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type, member)); \
    })
#define DEVMM_RECORD_MAX_DEVICE_NUM 64U
#define DEVMM_RECORD_TOTAL_DEVICE_NUM (DEVMM_RECORD_MAX_DEVICE_NUM + 2U) /* 64 + 1, host is 65 */
static struct devmm_record_mng record_mng[DEVMM_FEATURE_TYPE_MAX][DEVMM_RECORD_TOTAL_DEVICE_NUM] = {0};

static struct devmm_record_node *devmm_get_record_node(enum devmm_record_feature_type type, uint32_t devid,
    enum devmm_record_key_type key_type, uint64_t key)
{
    struct devmm_record_node *record_node = NULL;
    struct rbtree_root *root_tree = NULL;
    struct rb_node *node = NULL;

    root_tree = (key_type == DEVMM_KEY_TYPE1) ? &record_mng[type][devid].key1_tree : &record_mng[type][devid].key2_tree;
    node = rbtree_get(key, root_tree);
    if (node != NULL) {
        record_node = (key_type == DEVMM_KEY_TYPE1) ? container_of(node, struct devmm_record_node, key1_node) :
            container_of(node, struct devmm_record_node, key2_node);
        return record_node;
    }
    return NULL;
}

static int devmm_record_node_insert(enum devmm_record_feature_type type, uint32_t devid, struct devmm_record_data *data,
    struct devmm_record_node *node)
{
    int ret;

    if (data->key1 != DEVMM_RECORD_KEY_INVALID) {
        ret = rbtree_insert(data->key1, &record_mng[type][devid].key1_tree, &node->key1_node);
        if (ret != 0) {
            return ret;
        }
    }

    if (data->key2 != DEVMM_RECORD_KEY_INVALID) {
        ret = rbtree_insert(data->key2, &record_mng[type][devid].key2_tree, &node->key2_node);
        if (ret != 0) {
            if (data->key1 != DEVMM_RECORD_KEY_INVALID) {
                rbtree_erase(&record_mng[type][devid].key1_tree, &node->key1_node);
            }
            return ret;
        }
    }
    return 0;
}

static void devmm_record_node_erase(struct devmm_record_node *node)
{
    if (node->key1 != DEVMM_RECORD_KEY_INVALID) {
        rbtree_erase(&record_mng[node->type][node->devid].key1_tree, &node->key1_node);
    }
    if (node->key2 != DEVMM_RECORD_KEY_INVALID) {
        rbtree_erase(&record_mng[node->type][node->devid].key2_tree, &node->key2_node);
    }
}

static drvError_t devmm_record_node_init(enum devmm_record_feature_type type, uint32_t devid,
    enum devmm_record_node_status status, struct devmm_record_data *data, struct devmm_record_node *node)
{
    uint64_t len = data->data_len;
    int ret;

    uref_init(&node->ref);
    node->type = type;
    node->devid = devid;
    node->key1 = data->key1;
    node->key2 = data->key2;
    node->status = status;
    node->data = NULL;
    node->data_len = len;
    if (len != 0) {
        node->data = (void *)((uintptr_t)node + sizeof(struct devmm_record_node));
        (void)memcpy_s(node->data, len, data->data, len);
    }

    ret = devmm_record_node_insert(type, devid, data, node);
    if (ret != 0) {
        return DRV_ERROR_REPEATED_USERD;
    }
    return DRV_ERROR_NONE;
}

static int devmm_record_node_create(enum devmm_record_feature_type type, uint32_t devid,
    enum devmm_record_node_status status, struct devmm_record_data *data)
{
    struct devmm_record_node *node = NULL;
    drvError_t ret;

    node = malloc(sizeof(struct devmm_record_node) + data->data_len);
    if(node == NULL) {
        return DRV_ERROR_OUT_OF_MEMORY;
    }
    ret = devmm_record_node_init(type, devid, status, data, node);
    if (ret != DRV_ERROR_NONE) {
        free(node);
        return ret;
    }

    /* node is initing, if ret == DRV_ERROR_TRY_AGAIN,
       should call devmm_record_create_and_get again to complete initialization */
    return (status == DEVMM_NODE_INITING) ? DRV_ERROR_TRY_AGAIN : DRV_ERROR_NONE;
}

static void devmm_record_node_release(struct uref *uref)
{
    struct devmm_record_node *node = container_of(uref, struct devmm_record_node, ref);
    devmm_record_node_erase(node);
    free(node);
}

static void devmm_record_node_get_data(struct devmm_record_node *node, struct devmm_record_data *data)
{
    data->key1 = node->key1;
    data->key2 = node->key2;
    if (node->data_len != 0) {
        (void)memcpy_s(data->data, data->data_len, node->data, node->data_len);
        data->data_len = node->data_len;
    }
}

static drvError_t devmm_record_node_get_process(enum devmm_record_feature_type type, uint32_t devid,
    enum devmm_record_node_status status, struct devmm_record_data *data, struct devmm_record_node *node)
{
    drvError_t ret = DRV_ERROR_INNER_ERR;

    if (node->status == DEVMM_NODE_INITED) {
        ret = (uref_get_unless_zero(&node->ref) == 0) ? DRV_ERROR_NO_RESOURCES : DRV_ERROR_NONE;
        if (ret == DRV_ERROR_NONE) {
            devmm_record_node_get_data(node, data);
        }
    } else if (((node->status == DEVMM_NODE_INITING) || (node->status == DEVMM_NODE_UNINITING)) && (status == DEVMM_NODE_INITING)) {
        ret = DRV_ERROR_WAIT_TIMEOUT;
    } else if ((node->status == DEVMM_NODE_INITING) && (status == DEVMM_NODE_INITED)) {
        ret = devmm_record_node_init(type, devid, status, data, node);
        if (ret != DRV_ERROR_NONE) {
            free(node);
        }
    }
    return ret;
}

drvError_t devmm_record_para_check(enum devmm_record_feature_type type, uint32_t devid,
    enum devmm_record_key_type key_type, struct devmm_record_data *data)
{
    if ((type >= DEVMM_FEATURE_TYPE_MAX) || (key_type >= DEVMM_KEY_TYPE_MAX) || (data->data_len > UINT32_MAX) ||
        (devid >= DEVMM_RECORD_TOTAL_DEVICE_NUM)) {
        DEVMM_DRV_ERR("Invalid para. (type=%u; key_type=%u; len=%llu; devid=%u)\n",
            type, key_type, data->data_len, devid);
        return DRV_ERROR_PARA_ERROR;
    }

    if ((data->key1 == DEVMM_RECORD_KEY_INVALID) ||
        ((key_type == DEVMM_KEY_TYPE2) && (data->key2 == DEVMM_RECORD_KEY_INVALID))) {
        DEVMM_DRV_ERR("Invalid para. (key_type=%u; key1=%llu; key2=%llu)\n", key_type, data->key1, data->key2);
        return DRV_ERROR_INVALID_VALUE;
    }
    return DRV_ERROR_NONE;
}

/**
* @ingroup driver
* @brief Record information and allow it to be queried
* @attention
* 1. Supports up to two key values for querying.
* 2. devmm_record_create_and_get: If the key does not exist, it will be created; if it exists, ref count will be increased.
* 3. devmm_record_put: If the key does not exist, it will fail; if it exists, ref count will be decreased.
* 4. Usage Instructions
*    Method 1: (DEVMM_NODE_INITING->DEVMM_NODE_INITED: prevent concurrency)
*    devmm_record_create_and_get(type, devid, key_type, DEVMM_NODE_INITING, data)
*              Processing flow of the caller
*    devmm_record_create_and_get(type, devid, key_type, DEVMM_NODE_INITED, data)
*    devmm_record_put(type, devid, key_type, key)
*    Method 2:
*    devmm_record_create_and_get(type, devid, key_type, DEVMM_NODE_INITED, data)
*    devmm_record_put(type, devid, key_type, key)
* @param [in] type Record feature type
* @param [in] devid Devid 0~63:devid 65:host devid
* @param [in] key_type Use which key to query
* @param [in] status Record status
* @param [in&out] data Record data
* @return DRV_ERROR_NONE : success
* @return DV_ERROR_XXX : record get fail
*           DRV_ERROR_TRY_AGAIN: node is initing, should call devmm_record_create_and_get again to complete initialization
*           DRV_ERROR_WAIT_TIMEOUT: Timeout waiting for other threads to complete initialization
*/
#define DEVMM_RECORD_RETRY_MAX_TIMES 4320000000ULL   /* 20us * 4320000000 = 24h */
drvError_t devmm_record_create_and_get(enum devmm_record_feature_type type, uint32_t devid,
    enum devmm_record_key_type key_type, enum devmm_record_node_status status, struct devmm_record_data *data)
{
    struct devmm_record_node *node = NULL;
    uint64_t retry_times = 0;
    drvError_t ret;
    uint64_t key;

    ret = devmm_record_para_check(type, devid, key_type, data);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    key = (key_type == DEVMM_KEY_TYPE1) ? data->key1 : data->key2;
    do {
        pthread_mutex_lock(&record_mng[type][devid].mutex);
        node = devmm_get_record_node(type, devid, key_type, key);
        if (node == NULL) {
            break;
        }
        ret = devmm_record_node_get_process(type, devid, status, data, node);
        pthread_mutex_unlock(&record_mng[type][devid].mutex);
        if ((ret == DRV_ERROR_WAIT_TIMEOUT) && (retry_times <= DEVMM_RECORD_RETRY_MAX_TIMES)) {
            usleep(20); // sleep 20 us
            retry_times++;
        } else {
            return ret;
        }
    } while (ret == DRV_ERROR_WAIT_TIMEOUT);

    ret = devmm_record_node_create(type, devid, status, data);
    pthread_mutex_unlock(&record_mng[type][devid].mutex);
    return ret;
}

drvError_t devmm_record_get(enum devmm_record_feature_type type, uint32_t devid,
    enum devmm_record_key_type key_type, struct devmm_record_data *data)
{
    struct devmm_record_node *node = NULL;
    drvError_t ret;
    uint64_t key;

    ret = devmm_record_para_check(type, devid, key_type, data);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    key = (key_type == DEVMM_KEY_TYPE1) ? data->key1 : data->key2;
    pthread_mutex_lock(&record_mng[type][devid].mutex);
    node = devmm_get_record_node(type, devid, key_type, key);
    if (node == NULL) {
        pthread_mutex_unlock(&record_mng[type][devid].mutex);
        return DRV_ERROR_NOT_EXIST;
    }
    ret = devmm_record_node_get_process(type, devid, DEVMM_NODE_INITED, data, node);
    pthread_mutex_unlock(&record_mng[type][devid].mutex);
    return ret;
}

static drvError_t devmm_record_node_put_process(struct devmm_record_node *node, enum devmm_record_node_status status)
{
    if (status == DEVMM_NODE_UNINITING) {
        if (node->status == DEVMM_NODE_INITED) {
            if (uref_read(&node->ref) == 1) {
                node->status = DEVMM_NODE_UNINITING;
                return DRV_ERROR_NONE;
            } else {
                return (uref_put(&node->ref, devmm_record_node_release) == 1) ? DRV_ERROR_NONE : DRV_ERROR_BUSY;
            }
        }
    } else if (status == DEVMM_NODE_UNINITED) {
        return (uref_put(&node->ref, devmm_record_node_release) == 1) ? DRV_ERROR_NONE : DRV_ERROR_BUSY;
    }

    return DRV_ERROR_INNER_ERR;
}

static drvError_t _devmm_record_put(enum devmm_record_feature_type type, uint32_t devid,
    uint32_t key_type, uint64_t key, enum devmm_record_node_status status)
{
    struct devmm_record_node *node = NULL;
    drvError_t ret = DRV_ERROR_NOT_EXIST; /* default: need to release */

    pthread_mutex_lock(&record_mng[type][devid].mutex);
    node = devmm_get_record_node(type, devid, key_type, key);
    if (node != NULL) {
        ret = devmm_record_node_put_process(node, status);
    }
    pthread_mutex_unlock(&record_mng[type][devid].mutex);
    return ret;
}

drvError_t devmm_record_put(enum devmm_record_feature_type type, uint32_t devid, uint32_t key_type, uint64_t key,
    enum devmm_record_node_status status)
{
    drvError_t ret;

    if ((type >= DEVMM_FEATURE_TYPE_MAX) || (key_type >= DEVMM_KEY_TYPE_MAX) ||
        ((devid != DEVMM_RECORD_INVALID_DEVID) && (devid >= DEVMM_RECORD_TOTAL_DEVICE_NUM))) {
        DEVMM_DRV_ERR("Invalid para. (type=%u; key_type=%u; devid=%u)\n", type, key_type, devid);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (devid < DEVMM_RECORD_TOTAL_DEVICE_NUM) {
        ret = _devmm_record_put(type, devid, key_type, key, status);
    } else {
        uint32_t devid_tmp;
        for (devid_tmp = 0; devid_tmp < DEVMM_RECORD_TOTAL_DEVICE_NUM; devid_tmp++) {
            ret = _devmm_record_put(type, devid_tmp, key_type, key, status);
            if (ret != DRV_ERROR_NOT_EXIST) {
                return ret;
            }
        }
    }
    /* DRV_ERROR_NOT_EXIST: need to release */
    return (ret == DRV_ERROR_NOT_EXIST) ? DRV_ERROR_NONE : ret;
}

static inline struct devmm_record_node *devmm_get_record_node_from_rb_node(struct rbtree_node *rb_node)
{
    struct rb_node *tmp = container_of(rb_node, struct rb_node, rbtree_node);
    return container_of(tmp, struct devmm_record_node, key1_node);
}

void devmm_record_recycle(uint32_t devid)
{
    enum devmm_record_feature_type type;
    struct devmm_record_node *node = NULL;
    struct rbtree_node *cur = NULL;
    struct rbtree_node *tmp = NULL;

    for (type = DEVMM_FEATURE_IPC; type < DEVMM_FEATURE_TYPE_MAX; type++) {
        pthread_mutex_lock(&record_mng[type][devid].mutex);
        rbtree_node_for_each_prev_safe(cur, tmp, &record_mng[type][devid].key1_tree) {
            node = devmm_get_record_node_from_rb_node(cur);
            if (node != NULL) {
                devmm_record_node_release(&node->ref);
            }
        }
        pthread_mutex_unlock(&record_mng[type][devid].mutex);
    }
}

static void __attribute__((constructor)) devmm_record_init(void)
{
    enum devmm_record_feature_type type;
    uint64_t devid;

    for (type = DEVMM_FEATURE_IPC; type < DEVMM_FEATURE_TYPE_MAX; type++) {
        for (devid = 0; devid < DEVMM_RECORD_TOTAL_DEVICE_NUM; devid++) {
            rbtree_init(&record_mng[type][devid].key1_tree);
            rbtree_init(&record_mng[type][devid].key2_tree);
            pthread_mutex_init(&record_mng[type][devid].mutex, NULL);
        }
    }
}

static void __attribute__((destructor)) devmm_record_uninit(void)
{
    enum devmm_record_feature_type type;
    struct devmm_record_node *node = NULL;
    struct rbtree_node *cur = NULL;
    struct rbtree_node *tmp = NULL;
    uint64_t devid;
    uint64_t i;

    for (type = DEVMM_FEATURE_IPC; type < DEVMM_FEATURE_TYPE_MAX; type++) {
        for (devid = 0; devid < DEVMM_RECORD_TOTAL_DEVICE_NUM; devid++) {
            i = 0;
            pthread_mutex_lock(&record_mng[type][devid].mutex);
            rbtree_node_for_each_prev_safe(cur, tmp, &record_mng[type][devid].key1_tree) {
                node = devmm_get_record_node_from_rb_node(cur);
                if (node != NULL) {
                    DEVMM_DRV_SWITCH("Record info. (type=%u; devid=%u; key1=%llu; key2=%llu)\n",
                        type, devid, node->key1, node->key2);
                    i++;
                }
            }
            pthread_mutex_unlock(&record_mng[type][devid].mutex);
            if (i != 0) {
                DEVMM_RUN_INFO("Record exit info. (type=%u; devid=%u; no_put_node_cnt=%llu)\n", type, devid, i);
            }
        }
    }
}

void devmm_record_restore_func_register(enum devmm_record_feature_type type,
    int (*restore)(uint64_t , uint64_t , uint32_t , void *))
{
    uint32_t devid;

    for (devid = 0; devid < DEVMM_RECORD_TOTAL_DEVICE_NUM; devid++) {
        record_mng[type][devid].restore = restore;
    }
}

int devmm_record_restore(void)
{
    enum devmm_record_feature_type type;
    struct devmm_record_node *node = NULL;
    struct rbtree_node *cur = NULL;
    struct rbtree_node *tmp = NULL;
    uint32_t devid;
    int ret;

    for (type = DEVMM_FEATURE_IPC; type < DEVMM_FEATURE_TYPE_MAX; type++) {
        for (devid = 0; devid < DEVMM_RECORD_TOTAL_DEVICE_NUM; devid++) {
            if (record_mng[type][devid].restore == NULL) {
                break;
            }
            pthread_mutex_lock(&record_mng[type][devid].mutex);
            rbtree_node_for_each_prev_safe(cur, tmp, &record_mng[type][devid].key1_tree) {
                node = devmm_get_record_node_from_rb_node(cur);
                if (node != NULL) {
                    DEVMM_DRV_DEBUG_ARG("Record restore. (type=%u; devid=%u; key1=%lu; key2=%lu)\n",
                        type, devid, node->key1, node->key2);
                    ret = record_mng[type][devid].restore(node->key1, node->key2, node->devid, node->data);
                    if (ret != 0) {
                        pthread_mutex_unlock(&record_mng[type][devid].mutex);
                        return ret;
                    }
                }
            }
            pthread_mutex_unlock(&record_mng[type][devid].mutex);
        }
    }
    return 0;
}
