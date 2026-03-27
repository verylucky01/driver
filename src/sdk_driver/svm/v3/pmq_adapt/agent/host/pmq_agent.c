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

#include "pbl_feature_loader.h"

#include "svm_kern_log.h"
#include "svm_addr_desc.h"
#include "kmc_h2d.h"
#include "pmq_msg.h"
#include "pmq.h"
#include "pmq_agent.h"

static int pmq_agent_query_pa(u32 udevid, void *msg, u32 *reply_len)
{
    struct pmq_query_pa_msg *msg_info = (struct pmq_query_pa_msg *)msg;
    int ret;

    ret = svm_pmq_pa_query(msg_info->tgid, msg_info->va, msg_info->size, msg_info->seg, &msg_info->head.extend_num);
    *reply_len = sizeof(struct pmq_query_pa_msg) + sizeof(struct svm_pa_seg) * msg_info->head.extend_num;
    return ret;
}

static struct svm_kmc_d2h_recv_handle g_d2h_query_pa_handle = {
    .func = pmq_agent_query_pa,
    .raw_msg_size = sizeof(struct pmq_query_pa_msg),
    .extend_gran_size = sizeof(struct svm_pa_seg)
};

int pmq_agent_init(void)
{
    svm_kmc_d2h_recv_handle_register(SVM_KMC_MSG_QUERY_PA, &g_d2h_query_pa_handle);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(pmq_agent_init, FEATURE_LOADER_STAGE_3);

void pmq_agent_uninit(void)
{
}
DECLAER_FEATURE_AUTO_UNINIT(pmq_agent_uninit, FEATURE_LOADER_STAGE_3);

