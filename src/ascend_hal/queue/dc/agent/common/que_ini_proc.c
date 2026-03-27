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

#ifndef EMU_ST
#include "svm_user_interface.h"
#endif
#include "queue.h"
#include "que_ub_msg.h"
#include "que_uma.h"
#include "que_compiler.h"
#include "queue_interface.h"
#include "que_comm_agent.h"
#include "queue_agent.h"
#include "que_jetty.h"
#include "que_mem_merge.h"
#include "que_ini_proc.h"

#define US_PER_SECOND 1000000
#ifndef EMU_ST
static int g_ini_level = 0;
static uint64_t ini_base_time[MAX_DEVICE] = {0};

int que_get_ini_log_level(void)
{
    return g_ini_level;
}

uint64_t que_get_ini_basetime(unsigned int devid)
{
    return ini_base_time[devid];
}

void que_update_ini_basetime(unsigned int devid)
{
    uint64_t cur_time = que_get_cur_time_ns();
    ini_base_time[devid] = cur_time;
}

void que_ini_time_stamp(struct que_ini_proc *ini_proc, QUE_TRACE_INI_TIMESTAMP type)
{
    int ini_log_level = que_get_ini_log_level();
    unsigned int num = TRACE_INI_LEVLE0_BUTT + ini_proc->total_iovec_num * (TRACE_INI_LEVLE1_BUTT - TRACE_INI_LEVLE1_START) * (unsigned int)ini_log_level;
    if ((unsigned int)type >= num) {
        return;
    }

    if (ini_proc->timestamp == NULL) {
        return;
    }
    ini_proc->timestamp[type] = que_get_cur_time_ns();
}

struct que_ini_proc *que_ini_proc_create()
{
    struct que_ini_proc *ini_proc = NULL;

    ini_proc = (struct que_ini_proc *)calloc(QUEUE_ENQUE_BUTT, sizeof(struct que_ini_proc));
    if (que_unlikely(ini_proc == NULL)) {
        return NULL;
    }
    return ini_proc;
}

void que_ini_proc_destroy(struct que_ini_proc *ini_proc)
{
    free(ini_proc);
}

struct que_pkt *que_tx_get_first_pkt(struct que_tx *tx)
{
    unsigned long pkt_base = (unsigned long)(uintptr_t)tx->pkt;

    return (struct que_pkt *)(pkt_base);
}

struct que_pkt *que_tx_get_other_pkts(struct que_tx *tx)
{
    unsigned long pkt_base = (unsigned long)(uintptr_t)tx->pkt;

    return (struct que_pkt *)(pkt_base + QUE_UMA_MAX_SEND_SIZE);
}

static inline QUE_MEM_TYPE que_get_buff_memtype(struct buff_iovec *vector)
{
#ifdef DRV_HOST
    struct DVattribute attr;
    drvError_t ret;
    DVdeviceptr vptr = (DVdeviceptr)(uintptr_t)vector->ptr[0].iovec_base;
    ret = drvMemGetAttribute(vptr, &attr);
    if (ret != DRV_ERROR_NONE) {
        return MEM_NOT_SVM;
    } else {
        if (attr.memType == DV_MEM_LOCK_DEV) {
            return MEM_DEVICE_SVM;
        } else if (attr.memType == DV_MEM_USER_MALLOC) {
            return MEM_NOT_SVM;
        } else {
            return MEM_OTHERS_SVM;
        }
    }
#else
    return MEM_NOT_SVM;
#endif
}

static int que_tx_first_iovec_num_get(QUE_MEM_TYPE mem_type, struct buff_iovec *vector, unsigned int *iovec_num,
    bool *default_wr_flag)
{
    unsigned int i;
    unsigned int max_node_num_in_pkt = _que_get_node_num();
    unsigned long long pkt_read_wr_num, wr_num_tmp, total_rw_wr_num = 0;
    unsigned long long max_pkt_size = _que_get_pkt_size(vector->count);

    *iovec_num = que_min(max_node_num_in_pkt, vector->count);
    if (mem_type == MEM_DEVICE_SVM) {
        return DRV_ERROR_NONE;
    }

    pkt_read_wr_num = que_align_up(max_pkt_size, QUE_URMA_MAX_SIZE) / QUE_URMA_MAX_SIZE;
    if ((pkt_read_wr_num + 1) > QUE_MAX_RW_WR_NUM) { /* 1 for context wr*/
        QUEUE_LOG_ERR("que tx fill iovec size over limit. (max_pkt_size=%llu; pkt_read_wr_num=%llu)\n",
            max_pkt_size, pkt_read_wr_num);
        return DRV_ERROR_PARA_ERROR;
    }

    total_rw_wr_num = pkt_read_wr_num + (vector->context_len != 0);
    for (i = 0; i < *iovec_num; i++) {
        wr_num_tmp = que_align_up(vector->ptr[i].len, QUE_URMA_MAX_SIZE) / QUE_URMA_MAX_SIZE;
        if (((total_rw_wr_num + wr_num_tmp) > QUE_MAX_RW_WR_NUM) ||
            ((total_rw_wr_num + wr_num_tmp) <= total_rw_wr_num)) {
            break;
        }
        total_rw_wr_num += wr_num_tmp;
    }

    *default_wr_flag = (total_rw_wr_num <= QUE_DEFAULT_RW_WR_NUM) ? true : false;
    *iovec_num = i;
    return DRV_ERROR_NONE;
}

static struct que_tx *_que_tx_create(struct que_ini_proc *ini_proc, struct buff_iovec *vector)
{
    unsigned int first_iovec_num;
    unsigned int iovec_num = vector->count;
    QUE_MEM_TYPE mem_type = (ini_proc->que_type == ASYNC_ENQUE) ? MEM_NOT_SVM : que_get_buff_memtype(vector);
    unsigned long long iovec_size_temp, total_iovec_size = 0;
    bool default_wr_flag = true;
    struct que_tx *tx = NULL;
    unsigned int i;

    tx = (struct que_tx *)calloc(1, sizeof(struct que_tx));
    if (que_unlikely(tx == NULL)) {
        QUEUE_LOG_ERR("que tx alloc fail. (devid=%u; qid=%u; size=%ld)\n",
            ini_proc->devid, ini_proc->qid, sizeof(struct que_tx));
        return NULL;
    }

    for (i = 0; i < iovec_num; i++) {
        iovec_size_temp = total_iovec_size + vector->ptr[i].len;
        if (total_iovec_size < iovec_size_temp) {
            total_iovec_size = iovec_size_temp;
        } else {
            QUEUE_LOG_ERR("que tx fill iovec size over limit. (devid=%u; qid=%u; iovec_size_temp=%llu; total_iovec_size=%llu)\n",
                ini_proc->devid, ini_proc->qid, iovec_size_temp, total_iovec_size);
            free(tx);
            return NULL;
        }
    }

    int ret = que_tx_first_iovec_num_get(mem_type, vector, &first_iovec_num, &default_wr_flag);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        free(tx);
        return NULL;
    }

    tx->mem_type = mem_type;
    tx->que_type = ini_proc->que_type;
    tx->total_iovec_num = iovec_num;
    tx->pkt_size = _que_get_pkt_size(iovec_num) + QUE_UMA_MAX_SEND_SIZE;
    tx->total_iovec_size = total_iovec_size;
    tx->first_iovec_num = first_iovec_num;
    tx->remain_iovec_num = iovec_num - first_iovec_num;
    tx->default_wr_flag = default_wr_flag;
    tx->pkt_timestamp = que_get_cur_time_ns();

    ini_proc->sn = ini_proc->sn + 1;
    tx->sn = ini_proc->sn;
    return tx;
}

static void _que_tx_destroy(struct que_tx *tx)
{
    free(tx);
}

static int _que_tx_pkt_init(struct que_ini_proc *ini_proc, struct que_tx *tx)
{
    struct que_pkt *pkt = NULL;
    int ret;
    urma_target_seg_t *tseg = NULL;
    unsigned int urma_devid = que_get_urma_devid(ini_proc->devid, ini_proc->peer_devid);
    unsigned int access = URMA_ACCESS_READ;
    que_ini_time_stamp(ini_proc, TRACE_PKT_SEG_CREATE_START);

    ret = que_mem_alloc((void **)(&pkt), tx->pkt_size);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que pkt mem alloc fail. (devid=%u; qid=%u; size=%llu)\n", ini_proc->devid, ini_proc->qid,
            tx->pkt_size);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    if (que_is_share_mem((unsigned long long)(uintptr_t)pkt) == false) {
        tseg = que_pin_seg_create(urma_devid, (unsigned long long)(uintptr_t)pkt, tx->pkt_size, access, &ini_proc->token_info, ini_proc->d2d_flag);
    } else {
        tseg = que_get_urma_ctx_tseg(urma_devid, ini_proc->d2d_flag);
    }
    
    if (que_unlikely(tseg == NULL)) {
        que_mem_free(pkt);
        QUEUE_LOG_ERR("que seg create fail. (devid=%u; urma_devid=%u; qid=%u; size=%llu)\n", ini_proc->devid, urma_devid,
            ini_proc->qid, tx->pkt_size);
        return DRV_ERROR_QUEUE_INNER_ERROR;
    }
    que_ini_time_stamp(ini_proc, TRACE_PKT_SEG_CREATE_END);
    tx->pkt = pkt;
    tx->pkt_tseg = tseg;

    return DRV_ERROR_NONE;
}

static void _que_tx_pkt_uninit(struct que_tx *tx)
{
    unsigned long long addr = (unsigned long long)(uintptr_t)tx->pkt;
    if (que_likely(tx->pkt_tseg != NULL)) {
        if (que_is_share_mem(addr) == false) {
            que_seg_destroy(tx->pkt_tseg);
            tx->pkt_tseg = NULL;
        }
    }

    if (que_likely(tx->pkt != NULL)) {
        que_mem_free(tx->pkt);
        tx->pkt = NULL;
    }
}

static int _que_tx_vector_merge(unsigned int urma_devid, struct que_ini_proc *ini_proc, struct que_tx *tx, struct buff_iovec *vector)
{
    int ret;
    unsigned int i, j;
    unsigned long long va, size;

    if (vector->context_len != 0) {
        ret = que_mem_merge(&ini_proc->mem_ctx, urma_devid, (unsigned long long)(uintptr_t)vector->context_base, vector->context_len);
        if (que_unlikely(ret != DRV_ERROR_NONE)) {
            QUEUE_LOG_ERR("que ctx merge fail. (devid=%u; urma_devid=%u; qid=%u; va=0x%llx; len=%llu)\n",
                ini_proc->devid, urma_devid, ini_proc->qid, (unsigned long long)(uintptr_t)vector->context_base,
                vector->context_len);
            return ret;
        }
    }

    if (tx->mem_type == MEM_DEVICE_SVM) {
        return DRV_ERROR_NONE;
    }
    for (i = 0; i < tx->total_iovec_num; i++) {
        va = (unsigned long long)(uintptr_t)vector->ptr[i].iovec_base;
        size = vector->ptr[i].len;
        ret = que_mem_merge(&ini_proc->mem_ctx, urma_devid, va, size);
        if (que_unlikely(ret != DRV_ERROR_NONE)) {
            QUEUE_LOG_ERR("que ctx seg init fail. (ret=%d; devid=%u; qid=%u)\n", ret, ini_proc->devid, ini_proc->qid);
            goto err_out;
        }
    }
    return DRV_ERROR_NONE;
err_out:
    for (j = 0; j < i; j++) {
        va = (unsigned long long)(uintptr_t)vector->ptr[j].iovec_base;
        que_mem_seg_unregister(&ini_proc->mem_ctx, urma_devid, va);
    }
    que_mem_seg_unregister(&ini_proc->mem_ctx, urma_devid, (unsigned long long)(uintptr_t)vector->context_base);
    return ret;
}

static int _que_tx_ctx_seg_init(unsigned int urma_devid, struct que_ini_proc *ini_proc, struct que_tx *tx, struct buff_iovec *vector)
{
    urma_target_seg_t *tseg = NULL;
    unsigned int access;

    if (vector->context_len != 0) {
        access = (ini_proc->que_type == H2D_SYNC_DEQUE) ? DEQUE_INI_ACCESS : ENQUE_INI_ACCESS;
        que_ini_time_stamp(ini_proc, TRACE_CTX_SEG_CREATE_START);
        tseg = que_mem_seg_register(&ini_proc->mem_ctx, ini_proc->d2d_flag, &ini_proc->token_info, QUE_PIN, urma_devid,
            (unsigned long long)(uintptr_t)vector->context_base, access);
        if (que_unlikely(tseg == NULL)) {
            QUEUE_LOG_ERR("que ctx seg create fail. (devid=%u; urma_devid=%u; qid=%u; va=0x%llx; len=%llu)\n",
                ini_proc->devid, urma_devid, ini_proc->qid, (unsigned long long)(uintptr_t)vector->context_base,
                vector->context_len);
            return DRV_ERROR_QUEUE_INNER_ERROR;
        }
        que_ini_time_stamp(ini_proc, TRACE_CTX_SEG_CREATE_END);
    }
    tx->ctx_tseg = tseg;

    return DRV_ERROR_NONE;
}

static void _que_tx_ctx_seg_uninit(unsigned int urma_devid, struct que_tx *tx)
{
    if (que_likely(tx->ctx_tseg != NULL)) {
        tx->ctx_tseg = NULL;
    }
}

static int _que_tx_iovec_seg_init(unsigned int urma_devid, struct que_ini_proc *ini_proc, struct que_tx *tx, struct buff_iovec *vector)
{
    urma_target_seg_t **tseg = NULL;
    unsigned int i, j, access, pin_flg;
    unsigned long long va, size;

    if (tx->mem_type == MEM_DEVICE_SVM) {
        tx->iovec_tseg = NULL;
        return DRV_ERROR_NONE;
    }

    tseg = (urma_target_seg_t **)malloc(sizeof(urma_target_seg_t *) * tx->total_iovec_num);
    if (que_unlikely(tseg == NULL)) {
        QUEUE_LOG_ERR("que tx iovec tseg alloc fail. (devid=%u; qid=%u; total_iovec_num=%u)\n",
            ini_proc->devid, ini_proc->qid, tx->total_iovec_num);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    pin_flg = (tx->mem_type == MEM_OTHERS_SVM) ? QUE_NON_PIN : QUE_PIN;
    access = (ini_proc->que_type == H2D_SYNC_DEQUE) ? DEQUE_INI_ACCESS : ENQUE_INI_ACCESS;
    for (i = 0; i < tx->total_iovec_num; i++) {
        va = (unsigned long long)(uintptr_t)vector->ptr[i].iovec_base;
        size = vector->ptr[i].len;
        que_ini_time_stamp(ini_proc, TRACE_INI_IOVEC_SEG_CREATE_START + i * (TRACE_INI_LEVLE1_BUTT - TRACE_INI_LEVLE1_START));
        tseg[i] = que_mem_seg_register(&ini_proc->mem_ctx, ini_proc->d2d_flag, &ini_proc->token_info, pin_flg, urma_devid, va, access);
        if (que_unlikely(tseg[i] == NULL)) {
            QUEUE_LOG_ERR("que seg create fail. (devid=%u; urma_devid=%u; qid=%u; mem_type=%d; va=0x%llx; size=%llu; i=%u; iovec_num=%u)\n",
                ini_proc->devid, urma_devid, ini_proc->qid, tx->mem_type, va, size, i, tx->total_iovec_num);
            goto err_out;
        }
        que_ini_time_stamp(ini_proc, TRACE_INI_IOVEC_SEG_CREATE_END + i * (TRACE_INI_LEVLE1_BUTT - TRACE_INI_LEVLE1_START));
    }

    tx->iovec_tseg = tseg;
    return DRV_ERROR_NONE;
err_out:
    for (j = 0; j < i; j++) {
        va = (unsigned long long)(uintptr_t)vector->ptr[j].iovec_base;
        que_mem_seg_unregister(&ini_proc->mem_ctx, urma_devid, va);
    }
    free(tseg);
    return DRV_ERROR_QUEUE_INNER_ERROR;
}

static void _que_tx_iovec_seg_uninit(unsigned int urma_devid, struct que_tx *tx)
{
    unsigned int i;

    if (tx->iovec_tseg == NULL) {
        return;
    }

    for (i = 0; i < tx->total_iovec_num; i++) {
        tx->iovec_tseg[i] = NULL;
    }
    free(tx->iovec_tseg);
    tx->iovec_tseg = NULL;
}

static int _que_tx_seg_init(struct que_ini_proc *ini_proc, struct que_tx *tx, struct buff_iovec *vector)
{
    int ret;
    unsigned int urma_devid = que_get_urma_devid(ini_proc->devid, ini_proc->peer_devid);

    ret = _que_tx_vector_merge(urma_devid, ini_proc, tx, vector);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que iovec merge fail. (ret=%d; devid=%u; qid=%u)\n", ret, ini_proc->devid, ini_proc->qid);
        return ret;
    }

    ret = _que_tx_ctx_seg_init(urma_devid, ini_proc, tx, vector);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que ctx seg init fail. (ret=%d; devid=%u; qid=%u)\n", ret, ini_proc->devid, ini_proc->qid);
        goto merge_err;
    }

    ret = _que_tx_iovec_seg_init(urma_devid, ini_proc, tx, vector);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        _que_tx_ctx_seg_uninit(urma_devid, tx);
        QUEUE_LOG_ERR("que iovec seg init fail. (ret=%d; devid=%u; qid=%u)\n", ret, ini_proc->devid, ini_proc->qid);
        goto merge_err;
    }
    return DRV_ERROR_NONE;
merge_err:
    que_mem_erase_all_node(&ini_proc->mem_ctx);
    return ret;
}

static void _que_tx_seg_uninit(unsigned int urma_devid, struct que_tx *tx, struct que_mem_merge_ctx *mem_ctx)
{
    _que_tx_iovec_seg_uninit(urma_devid, tx);
    _que_tx_ctx_seg_uninit(urma_devid, tx);
    que_mem_erase_all_node(mem_ctx);
}

static struct que_tx *que_tx_create(struct que_ini_proc *ini_proc, struct buff_iovec *vector)
{
    struct que_tx *tx = NULL;
    int ret;

    tx = _que_tx_create(ini_proc, vector);
    if (que_unlikely(tx == NULL)) {
        QUEUE_LOG_ERR("que tx create fail. (devid=%u; qid=%u)\n", ini_proc->devid, ini_proc->qid);
        return NULL;
    }

    ret = _que_tx_pkt_init(ini_proc, tx);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que tx pkt init fail. (ret=%d; devid=%u; qid=%u)\n", ret, ini_proc->devid, ini_proc->qid);
        goto err_que_tx_pkt_init;
    }

    ret = _que_tx_seg_init(ini_proc, tx, vector);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que tx iovec init fail. (ret=%d; devid=%u; qid=%u)\n", ret, ini_proc->devid, ini_proc->qid);
        goto err_que_tx_iovec_init;
    }

    return tx;
err_que_tx_iovec_init:
    _que_tx_pkt_uninit(tx);
err_que_tx_pkt_init:
    _que_tx_destroy(tx);

    return NULL;
}

static void que_tx_destroy(struct que_tx *tx, struct que_mem_merge_ctx *mem_ctx, unsigned int urma_devid)
{
    _que_tx_seg_uninit(urma_devid, tx, mem_ctx);
    _que_tx_pkt_uninit(tx);
    _que_tx_destroy(tx);
}

static void que_tx_pkt_node_fill(struct que_tx *tx, struct buff_iovec *vector,
    unsigned int copied_vec_num, struct que_pkt *pkt)
{
    unsigned int node_id, iovec_id;
    for (node_id = 0; node_id < pkt->iovec_node_num; node_id++) {
        iovec_id = copied_vec_num + node_id;
        pkt->iovec_node[node_id].va = (unsigned long long)vector->ptr[iovec_id].iovec_base;
        pkt->iovec_node[node_id].size = vector->ptr[iovec_id].len;
        if (tx->iovec_tseg != NULL) {
            pkt->iovec_node[node_id].seg = tx->iovec_tseg[iovec_id]->seg;
        }
    }
}

static void que_tx_pkt_head_fill(unsigned int qid, unsigned int local_qid, unsigned int devid, struct que_tx *tx,
    urma_token_t token, struct buff_iovec *vector, urma_jfr_id_t jfr_id, struct que_pkt *pkt)
{
    uint64_t ini_basetime = que_get_ini_basetime(devid);
    pkt->head.qid = qid;
    pkt->head.peer_qid = local_qid; /* local virqid for peer ack */
    pkt->head.total_iovec_num = tx->total_iovec_num;
    pkt->head.total_iovec_size = tx->total_iovec_size;
    pkt->head.mem_type = tx->mem_type;
    pkt->head.que_type = tx->que_type;
    pkt->head.ctx_node.va = (unsigned long long)(uintptr_t)vector->context_base;
    pkt->head.ctx_node.size = vector->context_len;
    pkt->head.jfr_id = jfr_id;
    pkt->head.token = token;

    if (tx->ctx_tseg != NULL) {
        pkt->head.ctx_node.seg = tx->ctx_tseg->seg;
    }

    pkt->head.pkt_node.seg = tx->pkt_tseg->seg;
    pkt->head.pkt_node.va = (unsigned long long)(uintptr_t)tx->pkt;
    pkt->head.pkt_node.size = tx->pkt_size;

    pkt->head.default_wr_flag = tx->default_wr_flag;
    pkt->head.first_iovec_num = tx->first_iovec_num;
    pkt->head.remain_iovec_num = tx->remain_iovec_num;
    pkt->head.pkt_timestamp = tx->pkt_timestamp;
    pkt->head.ini_base_timestamp = ini_basetime;
    pkt->head.sn = tx->sn;
}

static void que_tx_pkt_fill(unsigned int peer_qid, unsigned int local_qid, unsigned int devid, struct que_tx *tx,
    urma_token_t token, struct buff_iovec *vector, urma_jfr_id_t jfr_id)
{
    unsigned int copied_vec_num = 0;

    struct que_pkt *pkt = que_tx_get_first_pkt(tx);
    que_tx_pkt_head_fill(peer_qid, local_qid, devid, tx, token, vector, jfr_id, pkt);

    pkt->iovec_node_num = pkt->head.first_iovec_num;
    que_tx_pkt_node_fill(tx, vector, copied_vec_num, pkt);
    copied_vec_num += pkt->iovec_node_num;

    if (copied_vec_num == vector->count) {
        return;
    }

    pkt = que_tx_get_other_pkts(tx);
    que_tx_pkt_head_fill(peer_qid, local_qid, devid, tx, token, vector, jfr_id, pkt);

    pkt->iovec_node_num = pkt->head.remain_iovec_num;
    que_tx_pkt_node_fill(tx, vector, copied_vec_num, pkt);
    copied_vec_num += pkt->iovec_node_num;
}

static int _que_tx_pkt_post(struct que_ini_proc *ini_proc, struct que_tx *tx)
{
    struct que_pkt *pkt = NULL;
    urma_sge_t sge;

    pkt = que_tx_get_first_pkt(tx);
    sge.addr = (uint64_t)(uintptr_t)pkt;
    sge.len = _que_get_pkt_size(pkt->iovec_node_num);
    sge.tseg = tx->pkt_tseg;
    return que_uma_send_post_and_wait(ini_proc->pkt_send_jetty, &sge, ini_proc->d2d_flag);
}

static int _que_tx_send(struct que_ini_proc *ini_proc, struct que_tx *tx)
{
    /* only send the first packet */
    int ret = _que_tx_pkt_post(ini_proc, tx);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que tx pkt post fail. (ret=%d; devid=%u; qid=%u)\n", ret, ini_proc->devid, ini_proc->qid);
        return ret;
    }
    return DRV_ERROR_NONE;
}

static int que_tx_send(struct que_ini_proc *ini_proc, struct que_tx *tx, struct buff_iovec *vector)
{
    int ret;
    int virqid = queue_get_virtual_qid(ini_proc->qid, LOCAL_QUEUE);

    if (ini_proc->que_type != ASYNC_ENQUE) {
        que_tx_pkt_fill(ini_proc->qid, ini_proc->qid, ini_proc->devid, tx, ini_proc->token_info.token,
            vector, ini_proc->imm_recv_jetty->jfr->jfr_id);
    } else {
        que_tx_pkt_fill(ini_proc->peer_qid, virqid, ini_proc->devid, tx, ini_proc->token_info.token,
            vector, ini_proc->qjfr->jfr->jfr_id);
    }
    que_ini_time_stamp(ini_proc, TRACE_PKT_FILL);

    ret = _que_tx_send(ini_proc, tx);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que tx send fail. (ret=%d; devid=%u; qid=%u)\n", ret, ini_proc->devid, ini_proc->qid);
    }
    que_ini_time_stamp(ini_proc, TRACE_TX_SEND);
    return ret;
}

int que_ini_pkt_send(struct que_ini_proc *ini_proc, struct buff_iovec *vector)
{
    struct que_tx *tx = NULL;
    int ret;

    tx = que_tx_create(ini_proc, vector);
    if (que_unlikely(tx == NULL)) {
        QUEUE_LOG_ERR("que tx create fail. (devid=%u; qid=%u)\n", ini_proc->devid, ini_proc->qid);
        return DRV_ERROR_QUEUE_INNER_ERROR;
    }
    que_ini_time_stamp(ini_proc, TRACE_TX_CREATE);
    ini_proc->tx = tx;
    ini_proc->vector = vector;

    ret = que_tx_send(ini_proc, tx, vector);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que tx send fail. (ret=%d; devid=%u; qid=%u)\n", ret, ini_proc->devid, ini_proc->qid);
        return DRV_ERROR_QUEUE_INNER_ERROR;
    }
    return DRV_ERROR_NONE;
}

int que_ini_ack_wait(struct que_ini_proc *ini_proc, int timeout)
{
    int ret;
    int timeout_ms_ = timeout;
    struct timeval start, end;
    que_ack_data ack_data = {0};
    que_ini_time_stamp(ini_proc, TRACE_ACK_WAIT_START);

wait:
    que_get_time(&start);
    ret = que_uma_imm_wait(ini_proc->imm_recv_jetty, ini_proc->recv_para, &ack_data.imm_data, timeout_ms_);
    if (que_likely(ret == DRV_ERROR_NONE)) {
        if ((ack_data.ack_msg.sn != ini_proc->tx->sn) ||
            (ini_proc->qid != (unsigned int)ack_data.ack_msg.qid)) {
            QUEUE_LOG_ERR("que ack error. (ret=%d; devid=%u; memtype=%d; exp_qid=%u; act_qid=%u,"
                "exp_sn=%d, act_sn=%u)\n",
                ret, ini_proc->devid, ini_proc->que_type, ini_proc->qid, ack_data.ack_msg.qid,
                ini_proc->tx->sn, ack_data.ack_msg.sn);
           que_get_time(&end);
            queue_agent_update_time(start, end, &timeout_ms_);
            goto wait;
        }
    }

    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que wait fail. (ret=%d; devid=%u; qid=%u; que_type=%d, timeout=%d)\n",
            ret, ini_proc->devid, ini_proc->qid, (int)ini_proc->que_type, timeout);
        return ret;
    }
    if (que_unlikely(ack_data.ack_msg.result == DRV_ERROR_TRANS_LINK_ACK_TIMEOUT_ERR)) {
        QUEUE_LOG_WARN("The packet has already been received in tgt, continue wait ack.\n");
        goto wait;
    }

    que_ini_time_stamp(ini_proc, TRACE_ACK_WAIT_END);
    ini_proc->tgt_time = ack_data.ack_msg.tgt_time;
    return ack_data.ack_msg.result;
}

void que_ini_proc_done(struct que_ini_proc *ini_proc)
{
    unsigned int urma_devid = que_get_urma_devid(ini_proc->devid, ini_proc->peer_devid);

    if (que_likely(ini_proc->tx != NULL)) {
        que_tx_destroy(ini_proc->tx, &ini_proc->mem_ctx, urma_devid);
        ini_proc->tx = NULL;
    }
}

#else   /* EMU_ST */

void que_ini_proc_emu_test(void)
{
}

#endif  /* EMU_ST */
