/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <stdlib.h>
#include <pthread.h>

#include "ascend_hal_error.h"
#include "queue.h"
#include "que_compiler.h"
#include "que_urma.h"
#include "que_uma.h"
#include "que_jetty.h"

static pthread_rwlock_t g_jetty_lock[MAX_DEVICE];

struct que_jetty_pool {
    unsigned long long total_len[QUE_DATA_RW_JETTY_POOL_DEPTH];
    struct que_jfs *qjfs[QUE_DATA_RW_JETTY_POOL_DEPTH];
    struct que_jfs_rw_wr *rw_wr[QUE_DATA_RW_JETTY_POOL_DEPTH];
};

static struct que_jetty_pool g_jetty_pool[MAX_DEVICE][TRANS_TYPE_MAX];

static inline void que_jetty_wrlock(unsigned int devid)
{
    (void)pthread_rwlock_wrlock(&g_jetty_lock[devid]);
}

static inline void que_jetty_rdlock(unsigned int devid)
{
    (void)pthread_rwlock_rdlock(&g_jetty_lock[devid]);
}

static inline void que_jetty_unlock(unsigned int devid)
{
    (void)pthread_rwlock_unlock(&g_jetty_lock[devid]);
}

static inline void que_jetty_rwlock_init(unsigned int devid)
{
    (void)pthread_rwlock_init(&g_jetty_lock[devid], NULL);
}

static inline void _que_jetty_pool_init(unsigned int devid, unsigned int d2d_flag)
{
    for (unsigned int i = 0; i < QUE_DATA_RW_JETTY_POOL_DEPTH; i++) {
        g_jetty_pool[devid][d2d_flag].total_len[i] = 0;
        g_jetty_pool[devid][d2d_flag].qjfs[i] = NULL;
        g_jetty_pool[devid][d2d_flag].rw_wr[i] = NULL;
    }
}

unsigned int que_idle_jetty_find(unsigned int devid, unsigned int d2d_flag)
{
    unsigned int num = 0;

    que_jetty_rdlock(devid);
    for (unsigned int i = 0; i < QUE_DATA_RW_JETTY_POOL_DEPTH; i++) {
        if (g_jetty_pool[devid][d2d_flag].total_len[i] == 0) {
            num++;
        }
    }
    que_jetty_unlock(devid);
    return num;
}

unsigned int que_rw_jetty_alloc(unsigned int devid, unsigned int d2d_flag)
{
    que_jetty_wrlock(devid);
    for (unsigned int i = 0; i < QUE_DATA_RW_JETTY_POOL_DEPTH; i++) {
        if (g_jetty_pool[devid][d2d_flag].total_len[i] == 0) {
            g_jetty_pool[devid][d2d_flag].total_len[i] = 1;
            que_jetty_unlock(devid);
            return i;
        }
    }
    que_jetty_unlock(devid);
    return QUE_DATA_RW_JETTY_POOL_DEPTH;
}

void que_rw_jetty_free(unsigned int devid, unsigned int jetty_idx, unsigned int d2d_flag)
{
    que_jetty_wrlock(devid);
    g_jetty_pool[devid][d2d_flag].total_len[jetty_idx] = 0;
    que_jetty_unlock(devid);
}

struct que_jfs *que_qjfs_get(unsigned int devid, unsigned int jetty_idx, unsigned int d2d_flag)
{
    return g_jetty_pool[devid][d2d_flag].qjfs[jetty_idx];
}

struct que_jfs_rw_wr *que_send_wr_get(unsigned int devid, unsigned int jetty_idx, unsigned int d2d_flag)
{
    return g_jetty_pool[devid][d2d_flag].rw_wr[jetty_idx];
}

static int _que_jfs_jfce_init(struct que_jfs *qjfs, unsigned int d2d_flag)
{
    urma_jfce_t *jfce_s = NULL;

    if (qjfs->attr.spec_jfce_s) {
        jfce_s = qjfs->attr.jfce;
    } else {
        jfce_s = que_urma_jfce_create(qjfs->devid, d2d_flag);
        if (que_unlikely(jfce_s == NULL)) {
            que_urma_jfce_destroy(jfce_s);
            QUEUE_LOG_ERR("Jfce_s create fail. (devid=%u)\n", qjfs->devid);
            return DRV_ERROR_INNER_ERR;
        }
    }
    qjfs->jfce_s = jfce_s;

    return DRV_ERROR_NONE;
}
 
static void _que_jfs_jfce_uninit(struct que_jfs *qjfs)
{
    if (que_likely((qjfs->jfce_s != NULL) && (!qjfs->attr.spec_jfce_s))) {
        que_urma_jfce_destroy(qjfs->jfce_s);
        qjfs->jfce_s = NULL;
    }
}

static int _que_jfr_jfce_init(struct que_jfr *qjfr, unsigned int d2d_flag)
{
    urma_jfce_t *jfce_r = NULL;
 
    jfce_r = que_urma_jfce_create(qjfr->devid, d2d_flag);
    if (que_unlikely(jfce_r == NULL)) {
        QUEUE_LOG_ERR("jfce_r create fail. (devid=%u)\n", qjfr->devid);
        return DRV_ERROR_INNER_ERR;
    }

    qjfr->jfce_r = jfce_r;
    return DRV_ERROR_NONE;
}

static void _que_jfr_jfce_uninit(struct que_jfr *qjfr)
{
    if (que_likely(qjfr->jfce_r != NULL)) {
        que_urma_jfce_destroy(qjfr->jfce_r);
        qjfr->jfce_r = NULL;
    }
}

static int _que_jfs_jfc_init(struct que_jfs *qjfs, unsigned int d2d_flag)
{
    urma_jfc_t *jfc_s = NULL;
    urma_jfc_cfg_t jfc_cfg = {0};

    if (qjfs->attr.spec_jfc_s) {
        jfc_s = qjfs->attr.jfc;
    } else {
        jfc_cfg.depth = qjfs->attr.jfc_s_depth;
        jfc_cfg.jfce = qjfs->jfce_s;
        jfc_s = que_urma_jfc_create(qjfs->devid, &jfc_cfg, d2d_flag);
        if (que_unlikely(jfc_s == NULL)) {
            QUEUE_LOG_ERR("jfc_r create fail. (devid=%u)\n", qjfs->devid);
            return DRV_ERROR_INNER_ERR;
        }
    }

    qjfs->jfc_s = jfc_s;
    return DRV_ERROR_NONE;
}
 
static void _que_jfs_jfc_uninit(struct que_jfs *qjfs)
{
    if (que_likely((qjfs->jfc_s != NULL) && (!qjfs->attr.spec_jfc_s))) {
        que_urma_jfc_destroy(qjfs->jfc_s);
        qjfs->jfc_s = NULL;
    }
}

static int _que_jfr_jfc_init(struct que_jfr *qjfr, unsigned int d2d_flag)
{
    urma_jfc_t *jfc_r = NULL;
    urma_jfc_cfg_t jfc_cfg = {0};

    jfc_cfg.depth = qjfr->attr.jfc_r_depth;
    jfc_cfg.jfce = qjfr->jfce_r;
    jfc_r = que_urma_jfc_create(qjfr->devid, &jfc_cfg, d2d_flag);
    if (que_unlikely(jfc_r == NULL)) {
        QUEUE_LOG_ERR("jfc_r create fail. (devid=%u)\n", qjfr->devid);
        return DRV_ERROR_INNER_ERR;
    }

    qjfr->jfc_r = jfc_r;
    return DRV_ERROR_NONE;
}
 
static void _que_jfr_jfc_uninit(struct que_jfr *qjfr)
{
    if (que_likely(qjfr->jfc_r != NULL)) {
        que_urma_jfc_destroy(qjfr->jfc_r);
        qjfr->jfc_r = NULL;
    }
}

static inline void _que_jfs_cfg_fill(struct que_jfs *qjfs, urma_jfs_cfg_t *jfs_cfg)
{
    jfs_cfg->depth = qjfs->attr.jfs_depth;
    jfs_cfg->priority = qjfs->attr.priority;
    jfs_cfg->trans_mode = URMA_TM_RM;
    jfs_cfg->max_sge = 1;
    jfs_cfg->max_rsge = 1;
    jfs_cfg->err_timeout = URMA_TYPICAL_ERR_TIMEOUT;
    jfs_cfg->jfc = qjfs->jfc_s;
}
 
static inline void _que_jfr_cfg_fill(struct que_jfr *qjfr, urma_jfr_cfg_t *jfr_cfg)
{
    jfr_cfg->depth = qjfr->attr.jfr_depth;
    jfr_cfg->trans_mode = URMA_TM_RM;
    jfr_cfg->max_sge = 1;
    jfr_cfg->jfc = qjfr->jfc_r;
    jfr_cfg->id = 0;
    jfr_cfg->flag.bs.token_policy = URMA_TOKEN_PLAIN_TEXT;
}

static int _que_jfs_init(struct que_jfs *qjfs, unsigned int d2d_flag)
{
    urma_jfs_cfg_t jfs_cfg = {0};
    urma_jfs_t *jfs = NULL;

    _que_jfs_cfg_fill(qjfs, &jfs_cfg);
    jfs = que_urma_jfs_create(qjfs->devid, &jfs_cfg, d2d_flag);
    if (que_unlikely(jfs == NULL)) {
        QUEUE_LOG_ERR("Jfs create fail. (devid=%u)\n", qjfs->devid);
        return DRV_ERROR_INNER_ERR;
    }
    qjfs->jfs = jfs;

    return DRV_ERROR_NONE;
}

static void _que_jfs_uninit(struct que_jfs *qjfs)
{
    if (que_likely(qjfs->jfs!= NULL)) {
        que_urma_jfs_destroy(qjfs->jfs);
        qjfs->jfs = NULL;
    }
}

static int _que_jfr_init(struct que_jfr *qjfr, unsigned int d2d_flag)
{
    urma_jfr_cfg_t jfr_cfg = {0};
    urma_jfr_t *jfr = NULL;
 
    _que_jfr_cfg_fill(qjfr, &jfr_cfg);
    jfr = que_urma_jfr_create(qjfr->devid, &jfr_cfg, d2d_flag);
    if (que_unlikely(jfr == NULL)) {
        QUEUE_LOG_ERR("jfr create fail. (devid=%u)\n", qjfr->devid);
        return DRV_ERROR_INNER_ERR;
    }
    qjfr->jfr = jfr;
    return DRV_ERROR_NONE;
}

static void _que_jfr_uninit(struct que_jfr *qjfr)
{
    if (que_likely(qjfr->jfr!= NULL)) {
        que_urma_jfr_destroy(qjfr->jfr);
        qjfr->jfr = NULL;
    }
}

static int que_jfs_init(struct que_jfs *qjfs, unsigned int d2d_flag)
{
    int ret;

    ret = _que_jfs_jfce_init(qjfs, d2d_flag);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que jfce init fail. (ret=%d; devid=%u)\n", ret, qjfs->devid);
        return ret;
    }

    ret = _que_jfs_jfc_init(qjfs, d2d_flag);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que jfc init fail. (ret=%d; devid=%u)\n", ret, qjfs->devid);
        goto err_jfc_init;
    }

    ret = _que_jfs_init(qjfs, d2d_flag);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que jetty init fail. (ret=%d; devid=%u)\n", ret, qjfs->devid);
        goto err_jfs_init;
    }

    return DRV_ERROR_NONE;
err_jfs_init:
    _que_jfs_jfc_uninit(qjfs);

err_jfc_init:
    _que_jfs_jfce_uninit(qjfs);
    return ret;
}

static void que_jfs_uninit(struct que_jfs *qjfs)
{
    _que_jfs_uninit(qjfs);
    _que_jfs_jfc_uninit(qjfs);
    _que_jfs_jfce_uninit(qjfs);
}

static int que_jfr_init(struct que_jfr *qjfr, unsigned int d2d_flag)
{
    int ret;

    ret = _que_jfr_jfce_init(qjfr, d2d_flag);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("Que jfce init fail. (ret=%d; devid=%u)\n", ret, qjfr->devid);
        return ret;
    }

    ret = _que_jfr_jfc_init(qjfr, d2d_flag);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que jfc init fail. (ret=%d; devid=%u)\n", ret, qjfr->devid);
        goto err_jfc_init;
    }

    ret = _que_jfr_init(qjfr, d2d_flag);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que jetty init fail. (ret=%d; devid=%u)\n", ret, qjfr->devid);
        goto err_jfr_init;
    }

    return DRV_ERROR_NONE;
err_jfr_init:
    _que_jfr_jfc_uninit(qjfr);
 
err_jfc_init:
    _que_jfr_jfce_uninit(qjfr);
    return ret;
}

static void que_jfr_uninit(struct que_jfr *qjfr)
{
    _que_jfr_uninit(qjfr);
    _que_jfr_jfc_uninit(qjfr);
    _que_jfr_jfce_uninit(qjfr);
}

static struct que_jfs *_que_jfs_create(unsigned int devid, struct que_jfs_attr *attr)
{
    struct que_jfs *qjfs = NULL;
 
    qjfs = calloc(1, sizeof(struct que_jfs));
    if (que_unlikely(qjfs == NULL)) {
        QUEUE_LOG_ERR("qjetty alloc fail. (devid=%u; size=%ld)\n", devid, sizeof(struct que_jfs));
        return NULL;
    }
    qjfs->devid = devid;
    qjfs->attr = *attr;
    return qjfs;
}
 
static void _que_jfs_destroy(struct que_jfs *qjfs)
{
    free(qjfs);
}
 
static struct que_jfr *_que_jfr_create(unsigned int devid, struct que_jfr_attr *attr)
{
    struct que_jfr *qjfr = NULL;
 
    qjfr = calloc(1, sizeof(struct que_jfr));
    if (que_unlikely(qjfr == NULL)) {
        QUEUE_LOG_ERR("qjetty alloc fail. (devid=%u; size=%ld)\n", devid, sizeof(struct que_jfr));
        return NULL;
    }
    qjfr->devid = devid;
    qjfr->attr = *attr;
    return qjfr;
}
 
static void _que_jfr_destroy(struct que_jfr *qjfr)
{
    free(qjfr);
}
 
struct que_jfs *que_jfs_create(unsigned int devid, struct que_jfs_attr *attr, unsigned int d2d_flag)
{
    struct que_jfs *qjfs = NULL;
    int ret;
 
    qjfs = _que_jfs_create(devid, attr);
    if (que_unlikely(qjfs == NULL)) {
        QUEUE_LOG_ERR("Que jfs create fail. (devid=%u)\n", devid);
        return NULL;
    }

    ret = que_jfs_init(qjfs, d2d_flag);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        _que_jfs_destroy(qjfs);
        QUEUE_LOG_ERR("Que jfs init fail. (ret=%d; devid=%u)\n", ret, devid);
        return NULL;
    }

    return qjfs;
}
 
void que_jfs_destroy(struct que_jfs *qjfs)
{
    que_jfs_uninit(qjfs);
    _que_jfs_destroy(qjfs);
}

struct que_jfr *que_jfr_create(unsigned int devid, struct que_jfr_attr *attr, unsigned int d2d_flag)
{
    struct que_jfr *qjfr = NULL;
    int ret;
 
    qjfr = _que_jfr_create(devid, attr);
    if (que_unlikely(qjfr == NULL)) {
        QUEUE_LOG_ERR("que jfr create fail. (devid=%u)\n", devid);
        return NULL;
    }

    ret = que_jfr_init(qjfr, d2d_flag);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        _que_jfr_destroy(qjfr);
        QUEUE_LOG_ERR("que jfr init fail. (ret=%d; devid=%u)\n", ret, devid);
        return NULL;
    }

    return qjfr;
}
 
void que_jfr_destroy(struct que_jfr *qjfr)
{
    que_jfr_uninit(qjfr);
    _que_jfr_destroy(qjfr);
}

static int que_jetty_pool_wr_init(unsigned int devid, unsigned int d2d_flag)
{
    unsigned int i, j;
    struct que_jfs_rw_wr_attr attr = {.wr_num = QUE_DEFAULT_RW_WR_NUM, .opcode = URMA_OPC_READ};
    struct que_jfs_rw_wr *rw_wr[QUE_DATA_RW_JETTY_POOL_DEPTH] = {NULL};

    for (i = 0; i < QUE_DATA_RW_JETTY_POOL_DEPTH; i++) {
        rw_wr[i] = que_jfs_rw_wr_create(&attr);
        if (que_unlikely(rw_wr[i] == NULL)) {
            QUEUE_LOG_ERR("que jetty pool wr init fail. (jetty_idx=%d)\n", i);
            goto rw_wr_destroy;
        }
    }

    for (i = 0; i < QUE_DATA_RW_JETTY_POOL_DEPTH; i++) {
        g_jetty_pool[devid][d2d_flag].rw_wr[i] = rw_wr[i];
    }
    return DRV_ERROR_NONE;

rw_wr_destroy:
    for (j = 0; j < i; j++) {
        que_jfs_rw_wr_destroy(rw_wr[j]);
    }
    return DRV_ERROR_INNER_ERR;
}

static void que_jetty_pool_wr_uninit(unsigned int devid, unsigned int d2d_flag)
{
    for (unsigned int i = 0; i < QUE_DATA_RW_JETTY_POOL_DEPTH; i++) {
        if (que_likely(g_jetty_pool[devid][d2d_flag].rw_wr[i] != NULL)) {
            que_jfs_rw_wr_destroy(g_jetty_pool[devid][d2d_flag].rw_wr[i]);
            g_jetty_pool[devid][d2d_flag].rw_wr[i] = NULL;
        }
    }
}

int que_jfs_pool_init(unsigned int devid, struct que_jfs_attr *attr, unsigned int d2d_flag)
{
    int i, j, ret;
    _que_jetty_pool_init(devid, d2d_flag);

    for (i = 0; i < QUE_DATA_RW_JETTY_POOL_DEPTH; i++) {
        g_jetty_pool[devid][d2d_flag].qjfs[i] = que_jfs_create(devid, attr, d2d_flag);
        if (que_unlikely(g_jetty_pool[devid][d2d_flag].qjfs[i] == NULL)) {
            goto out;
        }
        attr->spec_jfc_s = 1; /* The jetty pool reuses jfc_s, only alloc jfc_s in the first loop.*/
        attr->jfc = g_jetty_pool[devid][d2d_flag].qjfs[0]->jfc_s;
    }

    ret = que_jetty_pool_wr_init(devid, d2d_flag);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        goto out;
    }
    que_jetty_rwlock_init(devid);
    return DRV_ERROR_NONE;

out:
    for (j = i - 1; j >= 0; j--) {
        que_jfs_destroy(g_jetty_pool[devid][d2d_flag].qjfs[j]);
        g_jetty_pool[devid][d2d_flag].qjfs[j] = NULL;
    }
    return DRV_ERROR_QUEUE_INNER_ERROR;
}

void que_jfs_pool_uninit(unsigned int devid, unsigned int d2d_flag)
{
    int i;
    que_jetty_pool_wr_uninit(devid, d2d_flag);
    for (i = QUE_DATA_RW_JETTY_POOL_DEPTH - 1; i >=0 ; i--) {
        que_jfs_destroy(g_jetty_pool[devid][d2d_flag].qjfs[i]);
        g_jetty_pool[devid][d2d_flag].qjfs[i] = NULL;
    }
}

