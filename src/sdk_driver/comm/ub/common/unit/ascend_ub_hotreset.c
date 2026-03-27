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

#include "ka_errno_pub.h"
#include "pbl/pbl_uda.h"
#include "ascend_ub_main.h"
#include "ascend_ub_admin_msg.h"
#include "ascend_ub_link_chan.h"
#include "ascend_ub_pair_dev_info.h"
#include "ascend_ub_hotreset.h"

ka_task_struct_t *g_ubdrv_hotreset_task = NULL;

int ubdrv_proc_host_hot_reset(struct ascend_ub_msg_dev *msg_dev, void *data)
{
    ubdrv_debug("Enter hot reset.\n");
    return 0;
}

int ubdrv_set_hot_reset(void)
{
    void __ka_mm_iomem *hotreset_base = ka_mm_ioremap(AO_SUBSYS_SUBCTRL + HOTRESET_REG, UBDRV_UB_PCIE_SEL_REG_SIZE);
    if (hotreset_base == NULL) {
        ubdrv_err("Map device reset base failed.\n");
        return -EINVAL;
    }
    ka_mm_writel(HOTRESET_REG_VALUE, hotreset_base);
    ka_mm_iounmap(hotreset_base);
    hotreset_base = NULL;
    ubdrv_info("Map device reset base success.\n");
    /* os->psci->bios->hot reset */
    kernel_restart(NULL);
    return 0;
}

void ubdrv_hot_reset_process_trigger(void)
{
    (void)ka_task_wake_up_process(g_ubdrv_hotreset_task);
}

STATIC int ubdrv_hotreset_thread_handle(void *data)
{
    if (ubdrv_set_hot_reset() != 0) {
        return -EINVAL;
    }
    return 0;
}

STATIC int ubdrv_hot_reset_thread_init(void)
{
    g_ubdrv_hotreset_task = ka_task_kthread_create(ubdrv_hotreset_thread_handle, NULL, "g_ubdrv_hotreset_task");
    if (KA_IS_ERR_OR_NULL(g_ubdrv_hotreset_task)) {
        ubdrv_err("create kthread ubdrv_hotreset_task failed!\n");
        return -EINVAL;
    }
    return 0;
}

int ubdrv_hot_reset_process(u32 dev_id)
{
    ubdrv_info("Hot reset begin. (uid=%u), (devid=%u)\n", ka_task_get_current_cred_uid(), dev_id);
    if (ubdrv_hot_reset_thread_init() != 0) {
        return -EINVAL;
    }
    ubdrv_hot_reset_process_trigger();
    return 0;
}

int ubdrv_set_hot_reset_msg_finish(struct ascend_ub_msg_dev *msg_dev, void *data)
{
    (void)data;
    return ubdrv_hot_reset_process(msg_dev->dev_id);
}