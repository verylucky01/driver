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

#ifndef SOC_RESMNG_H__
#define SOC_RESMNG_H__

#include <linux/ioctl.h>
#include <linux/string.h>
#include "pbl/pbl_soc_res.h"

#define SOC_IRQ_INVALID_VALUE 0xffffffffU
#define SOC_MAX_DAVINCI_NUM   2048

struct soc_reg_base {
    char name[SOC_RESMNG_MAX_NAME_LEN];
    struct list_head list_node;

    struct soc_reg_base_info info;
};

struct soc_rsv_mem {
    char name[SOC_RESMNG_MAX_NAME_LEN];
    struct list_head list_node;

    struct soc_rsv_mem_info info;
};

struct soc_key_data {
    char name[SOC_RESMNG_MAX_NAME_LEN];
    struct list_head list_node;

    u64 value;
};

struct soc_attr_data {
    char name[SOC_RESMNG_MAX_NAME_LEN];
    struct list_head list_node;

    void *attr;
    u32 size;
};


struct irq_info {
    u32 irq;
    u32 hwirq;
    u32 tscpu_to_taishan_irq;
};

struct soc_irq_info {
    struct irq_info *irqs;
    u32 irq_num;

    bool valid;
    u32 irq;
    u32 hw_irq;
};

static inline void soc_res_name_copy(char *des_name, const char *src_name)
{
    size_t i;

    for (i = 0; i < strnlen(src_name, SOC_RESMNG_MAX_NAME_LEN); i++) {
        des_name[i] = src_name[i];
    }
    des_name[i] = '\0';
}

int resmng_irqs_create(struct soc_irq_info *info, u32 irq_num);
void resmng_irqs_destroy(struct soc_irq_info *info);
int find_irq_index(struct soc_irq_info *info, u32 irq);
struct soc_rsv_mem *rsv_mem_node_find(const char *name, struct list_head *rsv_mems_head);
struct soc_reg_base *io_bases_node_find(const char *name, struct list_head *io_bases_head);
int soc_resmng_set_irq(struct res_inst_info *inst, u32 irq_type, u32 irq);
int soc_resmng_get_irq(struct res_inst_info *inst, u32 irq_type, u32 *irq);

int dev_set_key_value(struct list_head *head, const char *name, u64 value);
int dev_get_key_value(struct list_head *head, const char *name, u64 *value);

int soc_resmng_for_each_res_addr(struct res_inst_info *inst, u32 type,
    int (*func)(char *name, u64 addr, u64 len, void *priv), void *priv);
int soc_resmng_dev_for_each_res_addr(u32 devid, u32 type,
    int (*func)(char *name, u64 addr, u64 len, void *priv), void *priv);
int soc_resmng_for_each_key_value(struct res_inst_info *inst,
    int (*func)(char *key, u64 value, void *priv), void *priv);
int soc_resmng_dev_for_each_key_value(u32 devid, int (*func)(char *key, u64 value, void *priv), void *priv);
int soc_resmng_dev_for_each_attr(u32 devid, int (*func)(const char *name, void *attr, u32 size, void *priv), void *priv);

int resmng_init_module(void);
void resmng_exit_module(void);

#endif /* SOC_RESMNG_H__ */
