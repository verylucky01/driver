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
#include "pbl/pbl_feature_loader.h"
#include "adpater_def.h"

static inline void *get_symbol_objective_func(const struct kernel_symbol *sym, const char *synname)
{
    const char *module_sym_name = get_symbol_name(sym);
    if (strcmp(module_sym_name, synname) != 0) {
        return NULL;
    }
 
    return get_symbol_fun(sym);
}

 
void init_module_func(const struct module *mod, const struct symbol_list *fun_name_list,
    unsigned int count, struct bus_adpater_stu *adpat)
{
    unsigned int i, j;
    void *fn = NULL;
    uintptr_t *addr = NULL;
    down_write(&adpat->rw_lock);
    for (i = 0; i < mod->num_syms; i++) {
        for (j = 0; j < count; j++) {
            fn = get_symbol_objective_func(&mod->syms[i], fun_name_list[j].name);
            if (fn != NULL) {
                addr = (uintptr_t *)((uintptr_t)adpat + fun_name_list[j].offset);
                *addr = (uintptr_t)fn;
            }
        }
    }
    up_write(&adpat->rw_lock);
    return;
}
void uninit_module_func(const struct symbol_list *fun_name_list,
    unsigned int count, struct bus_adpater_stu *adpat)
{
    unsigned int i;
    uintptr_t *addr = NULL;
    down_write(&adpat->rw_lock);
    for (i = 0; i < count; i++) {
        addr = (uintptr_t *)((uintptr_t)adpat + fun_name_list[i].offset);
        *addr = (uintptr_t)0;
    }
    up_write(&adpat->rw_lock);
}

