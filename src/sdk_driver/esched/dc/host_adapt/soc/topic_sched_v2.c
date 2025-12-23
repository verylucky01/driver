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
#ifndef EMU_ST

#include <asm/io.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/vmalloc.h>

#include "tsdrv_interface.h"
#include "comm_kernel_interface.h"

#include "ascend_hal_define.h"
#include "esched_kernel_interface.h"

#include "esched.h"
#include "esched_host_msg.h"
#include "esched_drv_adapt.h"
#include "topic_sched_common.h"
#include "topic_sched_v2.h"

/* HOST reg offset */
#define SCHED_HOST_TOPIC_ADDR_OFFSET                                 (0x400000U)
#define SCHED_HOST_MB_ADDR_OFFSET                                    (0x40000U)

/* wait/get status and report reg */
#define STARS_TOPIC_VF_HOST_AICPU_WAIT_TOPIC_NS(cpu_id, vf_id)       (0x000C00 + (cpu_id) * 0x20 + (vf_id) * 0x10000)
#define STARS_TOPIC_VF_HOST_AICPU_INT_EN_NS(cpu_id, vf_id)           (0x000C04 + (cpu_id) * 0x20 + (vf_id) * 0x10000)
#define STARS_TOPIC_VF_HOST_AICPU_REPORT_NS(cpu_id, vf_id)           (0x000000 + (cpu_id) * 0x10 + (vf_id) * 0x10000)

#define STARS_TOPIC_HOST_AICPU_INT_STS0_NS(index, vf_id)            (0x000A00 + (vf_id) * 0x10000 + (index) * 0x20)
#define STARS_TOPIC_HOST_AICPU_INT_CLR0_NS(index, vf_id)            (0x000A08 + (vf_id) * 0x10000 + (index) * 0x20)
#define STARS_TOPIC_HOST_AICPU_INT_MASK0_NS(index, vf_id)           (0x000A0C + (vf_id) * 0x10000 + (index) * 0x20)
#define STARS_TOPIC_HOST_AICPU_INT_STS_NS(vf_id)                    (0x000A40 + (vf_id) * 0x10000)
#define STARS_TOPIC_HOST_AICPU_INT_CLR_NS(vf_id)                    (0x000A48 + (vf_id) * 0x10000)

void topic_sched_host_aicpu_intr_mask_set_v2(void __iomem *io_base, u32 mask_index, u32 vf_id, u32 val)
{
    u32 vf_id_tmp = ESCHED_DRV_REASSIGN_VFID(vf_id);
    esched_drv_reg_wr(io_base, STARS_TOPIC_HOST_AICPU_INT_MASK0_NS(mask_index, vf_id_tmp), val);
}

bool topic_sched_host_aicpu_is_mb_valid_v2(const void __iomem *io_base, u32 mb_id, u32 vf_id)
{
    u32 val;

    u32 vf_id_tmp = ESCHED_DRV_REASSIGN_VFID(vf_id);
    esched_drv_reg_rd(io_base, STARS_TOPIC_VF_HOST_AICPU_WAIT_TOPIC_NS(mb_id, vf_id_tmp), &val);

    return ((val & 0x1) == 1);
}

void topic_sched_host_aicpu_intr_clr_v2(void __iomem *io_base, u32 intr_index, u32 vf_id, u32 val)
{
    u32 vf_id_tmp = ESCHED_DRV_REASSIGN_VFID(vf_id);
    esched_drv_reg_wr(io_base, STARS_TOPIC_HOST_AICPU_INT_CLR0_NS(intr_index, vf_id_tmp), val);
}

void topic_sched_host_aicpu_intr_enable_v2(void __iomem *io_base, u32 cpu_index, u32 vf_id)
{
    u32 vf_id_tmp = ESCHED_DRV_REASSIGN_VFID(vf_id);
    esched_drv_reg_wr(io_base, STARS_TOPIC_VF_HOST_AICPU_INT_EN_NS(cpu_index, vf_id_tmp), 0x1);
}

void topic_sched_host_aicpu_int_all_status_v2(const void __iomem *io_base, u32 *val, u32 vf_id)
{
    u32 vf_id_tmp = ESCHED_DRV_REASSIGN_VFID(vf_id);
    esched_drv_reg_rd(io_base, STARS_TOPIC_HOST_AICPU_INT_STS_NS(vf_id_tmp), val);
}

void topic_sched_host_aicpu_intr_all_clr_v2(void __iomem *io_base, u32 val, u32 vf_id)
{
    u32 vf_id_tmp = ESCHED_DRV_REASSIGN_VFID(vf_id);
    esched_drv_reg_wr(io_base, STARS_TOPIC_HOST_AICPU_INT_CLR_NS(vf_id_tmp), val);
}

void topic_sched_host_aicpu_int_status_v2(const void __iomem *io_base, u32 intr_index, u32 *val, u32 vf_id)
{
    u32 vf_id_tmp = ESCHED_DRV_REASSIGN_VFID(vf_id);
    esched_drv_reg_rd(io_base, STARS_TOPIC_HOST_AICPU_INT_STS0_NS(intr_index, vf_id_tmp), val);
}

void esched_drv_cpu_report_v2(struct topic_data_chan *topic_chan, u32 vf_id, u32 error_code, u32 status)
{
    u16 cqe[8] = {0}; // 8 * 2 = 16Bytes
    u32 vf_id_tmp = ESCHED_DRV_REASSIGN_VFID(vf_id);

    if (topic_chan->mb_type != ACPU_HOST) {
        return;
    }

    cqe[0] = (u16)error_code;
    cqe[1] = (u16)status;

    esched_drv_reg_mem_wr(topic_chan->hard_res->report_addr,
        STARS_TOPIC_VF_HOST_AICPU_REPORT_NS(topic_chan->mb_id, vf_id_tmp), (void*)&cqe[0], sizeof(cqe));
}

int esched_drv_host_map_addr_v2(u32 dev_id, struct sched_hard_res *res)
{
    struct soc_reg_base_info io_base;
    struct soc_rsv_mem_info rsv_mem;
    struct res_inst_info inst;
    int ret;

    soc_resmng_inst_pack(&inst, dev_id, TS_SUBSYS, 0);

    /* topic sched base */
    ret = soc_resmng_get_reg_base(&inst, "TS_STARS_TOPIC_REG", &io_base);
    if (ret != 0) {
        sched_err("Failed to get TS_STARS_TOPIC_REG. (dev_id=%u)\n", dev_id);
        return ret;
    }

    res->io_base = ioremap(io_base.io_base, io_base.io_base_size);
    if (res->io_base == NULL) {
        sched_err("Failed to invoke the ioremap. (dev_id=%u; size=0x%x)\n", res->dev_id, (u32)io_base.io_base_size);
        return -ENOMEM;
    }

    res->int_io_base = NULL;

    ret = soc_resmng_get_rsv_mem(&inst, "TS_STARS_TOPIC_RSV_MEM", &rsv_mem);
    if (ret != 0) {
        sched_err("Failed to get TS_STARS_TOPIC_RSV_MEM. (dev_id=%u)\n", dev_id);
        goto iounmap_io_base;
    }

    res->rsv_mem_pa = rsv_mem.rsv_mem;
    res->rsv_mem_va = ioremap(res->rsv_mem_pa, rsv_mem.rsv_mem_size);
    if (res->rsv_mem_va == NULL) {
        sched_err("Failed to invoke the ioremap. (dev_id=%u; size=0x%x)\n", res->dev_id, (u32)rsv_mem.rsv_mem_size);
        ret = -ENOMEM;
        goto iounmap_io_base;
    }

    ret = soc_resmng_get_reg_base(&inst, "TS_STARS_TOPIC_CQE", &io_base);
    if (ret != 0) {
        sched_err("Failed to get TS_STARS_TOPIC_CQE. (dev_id=%u)\n", dev_id);
        goto iounmap_rsv_mem;
    }

    res->report_addr = ioremap(io_base.io_base, io_base.io_base_size);
    if (res->report_addr == NULL) {
        sched_err("Failed to invoke the ioremap. (dev_id=%u; size=0x%x)\n", res->dev_id, (u32)io_base.io_base_size);
        ret = -ENOMEM;
        goto iounmap_rsv_mem;
    }

    return 0;
iounmap_rsv_mem:
    iounmap(res->rsv_mem_va);
    res->rsv_mem_va = NULL;
iounmap_io_base:
    iounmap(res->io_base);
    res->io_base = NULL;
    return ret;
}

void esched_host_iounmap_v2(struct sched_hard_res *res)
{
    if (res->report_addr != NULL) {
        iounmap(res->report_addr);
        res->report_addr = NULL;
    }

    if (res->rsv_mem_va != NULL) {
        iounmap(res->rsv_mem_va);
        res->rsv_mem_va = NULL;
    }

    if (res->int_io_base != NULL) {
        iounmap(res->int_io_base);
        res->int_io_base = NULL;
    }

    if (res->io_base != NULL) {
        iounmap(res->io_base);
        res->io_base = NULL;
    }
}
#else
void esched_drv_host_topic_sched_v2(void)
{
}
#endif
