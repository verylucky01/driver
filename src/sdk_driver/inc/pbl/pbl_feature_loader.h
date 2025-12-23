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

#ifndef PBL_FEATURE_LOADER_H
#define PBL_FEATURE_LOADER_H

#include <linux/module.h>
#include <linux/version.h>
#include <linux/seq_file.h>

#include "securec.h"

#ifndef AUTO_INIT_MODULE_NAME
#define AUTO_INIT_MODULE_NAME      DEFAULT
#endif

#ifndef AUTO_INIT_MODULE_NAME_STR
#define AUTO_INIT_MODULE_NAME_STR "DEFAULT"
#endif

/*
   usage : surport feature global/device/task scope auto init and uninit
           feature can define init or uninit function use this macros:
               global: DECLAER_FEATURE_AUTO_INIT / DECLAER_FEATURE_AUTO_UNINIT
               device: DECLAER_FEATURE_AUTO_INIT_DEV / DECLAER_FEATURE_AUTO_UNINIT_DEV
               task: DECLAER_FEATURE_AUTO_INIT_TASK / DECLAER_FEATURE_AUTO_UNINIT_TASK
           global scope init/uninit call module_feature_auto_init/module_feature_auto_uninit in module_init/module_exit
           device scope init/uninit call module_feature_auto_init_dev/module_feature_auto_uninit_dev
                  when device online or offline
           task scope init/uninit call module_feature_auto_init_task/module_feature_auto_uninit_task
                  when proc add or remove
       When there are multiple auto-initialization features in the module and you want to contrl the sequence,
       you can use stage param.
           stage : scope 0-9, stage 0 has the highest priority.
                  init: functions are invoked in the sequence of stages.
                  uninit: functions are invoked in the oppside sequence of stages.
*/

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 5, 0)
struct kernel_symbol {
#ifdef CONFIG_HAVE_ARCH_PREL32_RELOCATIONS
    int value_offset;
    int name_offset;
    int namespace_offset;
#else
    unsigned long value;
    const char *name;
    const char *namespace;
#endif
};
#endif

#define PREFIX_MAX_LEN 128

enum {
    FEATURE_SCOPE_GLOBAL = 0,
    FEATURE_SCOPE_DEV,
    FEATURE_SCOPE_TASK,
    FEATURE_SCOPE_SELFDEF,
    FEATURE_SCOPE_MAX
};

enum {
    FEATURE_LOADER_STAGE_0 = 0,
    FEATURE_LOADER_STAGE_1,
    FEATURE_LOADER_STAGE_2,
    FEATURE_LOADER_STAGE_3,
    FEATURE_LOADER_STAGE_4,
    FEATURE_LOADER_STAGE_5,
    FEATURE_LOADER_STAGE_6,
    FEATURE_LOADER_STAGE_7,
    FEATURE_LOADER_STAGE_8,
    FEATURE_LOADER_STAGE_9,
    FEATURE_LOADER_STAGE_MAX,
};

struct feature_task_para {
    u32 udevid;
    int pid;
    void *start_time;
};

struct feature_dev_para {
    u32 udevid;
};

struct feature_selfdef_para {
    const char *scope;
    void *priv;
};

typedef int (*_fearture_init_func)(void);
typedef void (*_fearture_uninit_func)(void);

typedef int (*_fearture_init_dev_func)(u32 udevid);
typedef void (*_fearture_uninit_dev_func)(u32 udevid);

typedef int (*_fearture_init_task_func)(u32 udevid, int pid, void *start_time);
typedef void (*_fearture_uninit_task_func)(u32 udevid, int pid, void *start_time);

typedef int (*_fearture_init_selfdef_scope_func)(void *priv);
typedef void (*_fearture_uninit_selfdef_scope_func)(void *priv);


/* the function decleared will be auto call in module insert
   init_fn : shoule like "int _fearture_init_func(void)" */
#define __DECLAER_FEATURE_AUTO_INIT(module_name, init_fn, stage) \
    int module_name##_##stage##_init_##init_fn(void); \
    int module_name##_##stage##_init_##init_fn(void) \
    { \
        return init_fn(); \
    } \
    EXPORT_SYMBOL(module_name##_##stage##_init_##init_fn)
#define _DECLAER_FEATURE_AUTO_INIT(module_name, init_fn, stage) \
    __DECLAER_FEATURE_AUTO_INIT(module_name, init_fn, stage)
#define DECLAER_FEATURE_AUTO_INIT(init_fn, stage) \
    _DECLAER_FEATURE_AUTO_INIT(AUTO_INIT_MODULE_NAME, init_fn, stage)

/* the function decleared will be auto call in module remove
   init_fn : shoule like "void _fearture_uninit_func(void)" */
#define __DECLAER_FEATURE_AUTO_UNINIT(module_name, uninit_fn, stage) \
    void module_name##_##stage##_uninit_##uninit_fn(void); \
    void module_name##_##stage##_uninit_##uninit_fn(void) \
    { \
        uninit_fn(); \
    } \
    EXPORT_SYMBOL(module_name##_##stage##_uninit_##uninit_fn)
#define _DECLAER_FEATURE_AUTO_UNINIT(module_name, uninit_fn, stage) \
    __DECLAER_FEATURE_AUTO_UNINIT(module_name, uninit_fn, stage)
#define DECLAER_FEATURE_AUTO_UNINIT(uninit_fn, stage) \
    _DECLAER_FEATURE_AUTO_UNINIT(AUTO_INIT_MODULE_NAME, uninit_fn, stage)

/* the function decleared will be auto call when dev online
   init_fn : shoule like "int _fearture_init_dev_func(u32 udevid)" */
#define __DECLAER_FEATURE_AUTO_INIT_DEV(module_name, init_fn, stage) \
    int module_name##_##stage##_dev_init_##init_fn(u32 udevid); \
    int module_name##_##stage##_dev_init_##init_fn(u32 udevid) \
    { \
        return init_fn(udevid); \
    } \
    EXPORT_SYMBOL(module_name##_##stage##_dev_init_##init_fn)
#define _DECLAER_FEATURE_AUTO_INIT_DEV(module_name, init_fn, stage) \
    __DECLAER_FEATURE_AUTO_INIT_DEV(module_name, init_fn, stage)
#define DECLAER_FEATURE_AUTO_INIT_DEV(init_fn, stage) \
    _DECLAER_FEATURE_AUTO_INIT_DEV(AUTO_INIT_MODULE_NAME, init_fn, stage)

/* the function decleared will be auto call when dev offline
   init_fn : shoule like "void _fearture_uninit_dev_func(u32 udevid)" */
#define __DECLAER_FEATURE_AUTO_UNINIT_DEV(module_name, uninit_fn, stage) \
    void module_name##_##stage##_dev_uninit_##uninit_fn(u32 udevid); \
    void module_name##_##stage##_dev_uninit_##uninit_fn(u32 udevid) \
    { \
        uninit_fn(udevid); \
    } \
    EXPORT_SYMBOL(module_name##_##stage##_dev_uninit_##uninit_fn)
#define _DECLAER_FEATURE_AUTO_UNINIT_DEV(module_name, uninit_fn, stage) \
    __DECLAER_FEATURE_AUTO_UNINIT_DEV(module_name, uninit_fn, stage)
#define DECLAER_FEATURE_AUTO_UNINIT_DEV(uninit_fn, stage) \
    _DECLAER_FEATURE_AUTO_UNINIT_DEV(AUTO_INIT_MODULE_NAME, uninit_fn, stage)

/* the function decleared will be auto call when task init
   init_fn : shoule like "int _fearture_init_task_func(u32 udevid, int pid)" */
#define __DECLAER_FEATURE_AUTO_INIT_TASK(module_name, init_fn, stage) \
    int module_name##_##stage##_task_init_##init_fn(u32 udevid, int pid, void *start_time); \
    int module_name##_##stage##_task_init_##init_fn(u32 udevid, int pid, void *start_time) \
    { \
        return init_fn(udevid, pid, start_time); \
    } \
    EXPORT_SYMBOL(module_name##_##stage##_task_init_##init_fn)
#define _DECLAER_FEATURE_AUTO_INIT_TASK(module_name, init_fn, stage) \
    __DECLAER_FEATURE_AUTO_INIT_TASK(module_name, init_fn, stage)
#define DECLAER_FEATURE_AUTO_INIT_TASK(init_fn, stage) \
    _DECLAER_FEATURE_AUTO_INIT_TASK(AUTO_INIT_MODULE_NAME, init_fn, stage)

/* the function decleared will be auto call when task uninit
   init_fn : shoule like "void _fearture_uninit_task_func(u32 udevid, int pid)" */
#define __DECLAER_FEATURE_AUTO_UNINIT_TASK(module_name, uninit_fn, stage) \
    void module_name##_##stage##_task_uninit_##uninit_fn(u32 udevid, int pid, void *start_time); \
    void module_name##_##stage##_task_uninit_##uninit_fn(u32 udevid, int pid, void *start_time) \
    { \
        uninit_fn(udevid, pid, start_time); \
    } \
    EXPORT_SYMBOL(module_name##_##stage##_task_uninit_##uninit_fn)
#define _DECLAER_FEATURE_AUTO_UNINIT_TASK(module_name, uninit_fn, stage) \
    __DECLAER_FEATURE_AUTO_UNINIT_TASK(module_name, uninit_fn, stage)
#define DECLAER_FEATURE_AUTO_UNINIT_TASK(uninit_fn, stage) \
    _DECLAER_FEATURE_AUTO_UNINIT_TASK(AUTO_INIT_MODULE_NAME, uninit_fn, stage)

/* the function decleared will be auto call when module_feature_auto_init_selfdef called
   init_fn : shoule like "int _fearture_init_selfdef_func(void *priv)" */
#define __DECLAER_FEATURE_AUTO_INIT_BY_SCOPE(module_name, scope, init_fn, stage) \
    int module_name##_##stage##_##scope##_init_##init_fn(void *priv); \
    int module_name##_##stage##_##scope##_init_##init_fn(void *priv) \
    { \
        return init_fn(priv); \
    } \
    EXPORT_SYMBOL(module_name##_##stage##_##scope##_init_##init_fn)
#define _DECLAER_FEATURE_AUTO_INIT_BY_SCOPE(module_name, scope, init_fn, stage) \
    __DECLAER_FEATURE_AUTO_INIT_BY_SCOPE(module_name, scope, init_fn, stage)
#define DECLAER_FEATURE_AUTO_INIT_BY_SCOPE(scope, init_fn, stage) \
    _DECLAER_FEATURE_AUTO_INIT_BY_SCOPE(AUTO_INIT_MODULE_NAME, scope, init_fn, stage)

/* the function decleared will be auto call when module_feature_auto_uninit_selfdef called
   init_fn : shoule like "void _fearture_uninit_selfdef_func(void *priv)" */
#define __DECLAER_FEATURE_AUTO_UNINIT_BY_SCOPE(module, scope, uninit_fn, stage) \
    void module_name##_##stage##_##scope##_uninit_##uninit_fn(void *priv); \
    void module_name##_##stage##_##scope##_uninit_##uninit_fn(void *priv) \
    { \
        uninit_fn(priv); \
    } \
    EXPORT_SYMBOL(module_name##_##stage##_##scope##_uninit_##uninit_fn)
#define _DECLAER_FEATURE_AUTO_UNINIT_BY_SCOPE(module, scope, uninit_fn, stage) \
    __DECLAER_FEATURE_AUTO_UNINIT_BY_SCOPE(module, scope, uninit_fn, stage)
#define DECLAER_FEATURE_AUTO_UNINIT_BY_SCOPE(scope, uninit_fn, stage) \
    _DECLAER_FEATURE_AUTO_UNINIT_BY_SCOPE(AUTO_INIT_MODULE_NAME, scope, uninit_fn, stage)

static inline void *get_symbol_fun(const struct kernel_symbol *sym)
{
#ifdef CONFIG_HAVE_ARCH_PREL32_RELOCATIONS
    return (void *)(uintptr_t)offset_to_ptr(&sym->value_offset);
#else
    return (void *)(uintptr_t)sym->value;
#endif
}

static inline const char *get_symbol_name(const struct kernel_symbol *sym)
{
#ifdef CONFIG_HAVE_ARCH_PREL32_RELOCATIONS
    return offset_to_ptr(&sym->name_offset);
#else
    return sym->name;
#endif
}

static inline void *get_symbol_prefix_func(const struct kernel_symbol *sym, char *prefix)
{
    const char *sym_name = get_symbol_name(sym);
    if (strstr(sym_name, prefix) != sym_name) {
        return NULL;
    }

    return get_symbol_fun(sym);
}

static inline int module_feature_init_call(int feature_scope, void *fn, void *param)
{
    if (feature_scope == FEATURE_SCOPE_GLOBAL) {
        return ((_fearture_init_func)fn)();
    } else if (feature_scope == FEATURE_SCOPE_DEV) {
        struct feature_dev_para *dev_para = (struct feature_dev_para *)param;
        return ((_fearture_init_dev_func)fn)(dev_para->udevid);
    } else if (feature_scope == FEATURE_SCOPE_TASK) {
        struct feature_task_para *task_para = (struct feature_task_para *)param;
        return ((_fearture_init_task_func)fn)(task_para->udevid, task_para->pid, task_para->start_time);
    } else if (feature_scope == FEATURE_SCOPE_SELFDEF) {
        return ((_fearture_init_selfdef_scope_func)fn)(((struct feature_selfdef_para *)param)->priv);
    }
    return 0;
}

static inline int module_feature_try_to_init(int feature_scope, void *param,
    const struct kernel_symbol *sym, char *prefix)
{
    void *fn = get_symbol_prefix_func(sym, prefix);
    return (fn != NULL) ? module_feature_init_call(feature_scope, fn, param) : 0;
}

static inline void module_feature_uninit_call(int feature_scope, void *fn, void *param)
{
    if (feature_scope == FEATURE_SCOPE_GLOBAL) {
        ((_fearture_uninit_func)fn)();
    } else if (feature_scope == FEATURE_SCOPE_DEV) {
        struct feature_dev_para *dev_para = (struct feature_dev_para *)param;
        ((_fearture_uninit_dev_func)fn)(dev_para->udevid);
    } else if (feature_scope == FEATURE_SCOPE_TASK) {
        struct feature_task_para *task_para = (struct feature_task_para *)param;
        ((_fearture_uninit_task_func)fn)(task_para->udevid, task_para->pid, task_para->start_time);
    } else if (feature_scope == FEATURE_SCOPE_SELFDEF) {
        ((_fearture_uninit_selfdef_scope_func)fn)(((struct feature_selfdef_para *)param)->priv);
    }
}

static inline void module_feature_try_to_uninit(int feature_scope, void *param,
    const struct kernel_symbol *sym, char *prefix)
{
    void *fn = get_symbol_prefix_func(sym, prefix);
    if (fn != NULL) {
        module_feature_uninit_call(feature_scope, fn, param);
    }
}

static inline void module_feature_uninit_prefix_range(int feature_scope, void *param,
    struct module *module, char *prefix, u32 num_syms)
{
    u32 i;

    for (i = 0; i < num_syms; i++) {
        module_feature_try_to_uninit(feature_scope, param, &module->syms[i], prefix);
    }
}

static inline int module_feature_init_prefix(int feature_scope, void *param, char *init_prefix, char *uninit_prefix)
{
    struct module *module = THIS_MODULE;
    u32 i;

    for (i = 0; i < module->num_syms; i++) {
        int ret = module_feature_try_to_init(feature_scope, param, &module->syms[i], init_prefix);
        if (ret != 0) {
            module_feature_uninit_prefix_range(feature_scope, param, module, uninit_prefix, i);
            return ret;
        }
    }

    return 0;
}

static inline void module_feature_uninit_prefix(int feature_scope, void *param, char *prefix)
{
    struct module *module = THIS_MODULE;
    module_feature_uninit_prefix_range(feature_scope, param, module, prefix, module->num_syms);
}

static inline void module_feature_format_init_prefix(int feature_scope, void *param, char *prefix, int len, int stage)
{
    if (feature_scope == FEATURE_SCOPE_GLOBAL) {
        (void)sprintf_s(prefix, PREFIX_MAX_LEN, "%s_FEATURE_LOADER_STAGE_%d_init_", AUTO_INIT_MODULE_NAME_STR, stage);
    } else if (feature_scope == FEATURE_SCOPE_DEV) {
        (void)sprintf_s(prefix, PREFIX_MAX_LEN, "%s_FEATURE_LOADER_STAGE_%d_dev_init_", AUTO_INIT_MODULE_NAME_STR, stage);
    } else if (feature_scope == FEATURE_SCOPE_TASK) {
        (void)sprintf_s(prefix, PREFIX_MAX_LEN, "%s_FEATURE_LOADER_STAGE_%d_task_init_", AUTO_INIT_MODULE_NAME_STR, stage);
    } else if (feature_scope == FEATURE_SCOPE_SELFDEF) {
        (void)sprintf_s(prefix, PREFIX_MAX_LEN, "%s_FEATURE_LOADER_STAGE_%d_%s_init_", AUTO_INIT_MODULE_NAME_STR, stage,
            ((struct feature_selfdef_para*)param)->scope);
    }
}

static inline void module_feature_format_uninit_prefix(int feature_scope, void *param, char *prefix, int len, int stage)
{
    if (feature_scope == FEATURE_SCOPE_GLOBAL) {
        (void)sprintf_s(prefix, PREFIX_MAX_LEN, "%s_FEATURE_LOADER_STAGE_%d_uninit_", AUTO_INIT_MODULE_NAME_STR, stage);
    } else if (feature_scope == FEATURE_SCOPE_DEV) {
        (void)sprintf_s(prefix, PREFIX_MAX_LEN, "%s_FEATURE_LOADER_STAGE_%d_dev_uninit_", AUTO_INIT_MODULE_NAME_STR, stage);
    } else if (feature_scope == FEATURE_SCOPE_TASK) {
        (void)sprintf_s(prefix, PREFIX_MAX_LEN, "%s_FEATURE_LOADER_STAGE_%d_task_uninit_", AUTO_INIT_MODULE_NAME_STR, stage);
    } else if (feature_scope == FEATURE_SCOPE_SELFDEF) {
        (void)sprintf_s(prefix, PREFIX_MAX_LEN, "%s_FEATURE_LOADER_STAGE_%d_%s_uninit_", AUTO_INIT_MODULE_NAME_STR, stage,
            ((struct feature_selfdef_para*)param)->scope);
    }
}

static inline int module_feature_init_stage(int feature_scope, void *param, int stage)
{
    char init_prefix[PREFIX_MAX_LEN];
    char uninit_prefix[PREFIX_MAX_LEN];

    module_feature_format_init_prefix(feature_scope, param, init_prefix, PREFIX_MAX_LEN, stage);
    module_feature_format_uninit_prefix(feature_scope, param, uninit_prefix, PREFIX_MAX_LEN, stage);
    return module_feature_init_prefix(feature_scope, param, init_prefix, uninit_prefix);
}

static inline void module_feature_uninit_stage(int feature_scope, void *param, int stage)
{
    char uninit_prefix[PREFIX_MAX_LEN];

    module_feature_format_uninit_prefix(feature_scope, param, uninit_prefix, PREFIX_MAX_LEN, stage);
    module_feature_uninit_prefix(feature_scope, param, uninit_prefix);
}

static inline void module_feature_uninit_stage_range(int feature_scope, void *param, int min_stage, int max_stage)
{
    int stage;

    for (stage = max_stage; stage >= min_stage; stage--) {
        module_feature_uninit_stage(feature_scope, param, stage);
    }
}

static inline int module_feature_unified_init(int feature_scope, void *param)
{
    int stage;

    for (stage = FEATURE_LOADER_STAGE_0; stage <= FEATURE_LOADER_STAGE_9; stage++) {
        int ret = module_feature_init_stage(feature_scope, param, stage);
        if (ret != 0) {
            module_feature_uninit_stage_range(feature_scope, param, FEATURE_LOADER_STAGE_0, stage - 1);
            return ret;
        }
    }

    return 0;
}

static inline void module_feature_unified_uninit(int feature_scope, void *param)
{
    module_feature_uninit_stage_range(feature_scope, param, FEATURE_LOADER_STAGE_0, FEATURE_LOADER_STAGE_9);
}

static inline int module_feature_auto_init(void)
{
    return module_feature_unified_init(FEATURE_SCOPE_GLOBAL, NULL);
}

static inline void module_feature_auto_uninit(void)
{
    module_feature_unified_uninit(FEATURE_SCOPE_GLOBAL, NULL);
}

static inline int module_feature_auto_init_dev(u32 udevid)
{
    struct feature_dev_para dev_para = {.udevid = udevid};
    return module_feature_unified_init(FEATURE_SCOPE_DEV, (void *)&dev_para);
}

static inline void module_feature_auto_uninit_dev(u32 udevid)
{
    struct feature_dev_para dev_para = {.udevid = udevid};
    module_feature_unified_uninit(FEATURE_SCOPE_DEV, (void *)&dev_para);
}

static inline int module_feature_auto_init_task(u32 udevid, int pid, void *start_time)
{
    struct feature_task_para task_para = {.udevid = udevid, .pid = pid, .start_time = start_time};
    return module_feature_unified_init(FEATURE_SCOPE_TASK, (void *)&task_para);
}

static inline void module_feature_auto_uninit_task(u32 udevid, int pid, void *start_time)
{
    struct feature_task_para task_para = {.udevid = udevid, .pid = pid, .start_time = start_time};
    module_feature_unified_uninit(FEATURE_SCOPE_TASK, (void *)&task_para);
}

/* not suggest call this in high performance situation */
static inline int module_feature_auto_init_by_scope(const char *scope, void *priv)
{
    struct feature_selfdef_para para = {scope, priv};
    return module_feature_unified_init(FEATURE_SCOPE_SELFDEF, (void *)&para);
}

/* not suggest call this in high performance situation */
static inline int module_feature_auto_call_by_scope(const char *scope, void *priv)
{
    return module_feature_auto_init_by_scope(scope, priv);
}

/* not suggest call this in high performance situation */
static inline void module_feature_auto_uninit_by_scope(const char *scope, void *priv)
{
    struct feature_selfdef_para para = {scope, priv};
    module_feature_unified_uninit(FEATURE_SCOPE_SELFDEF, (void *)&para);
}

typedef void (*_fearture_show_task_func)(u32 udevid, int pid, int feature_id, struct seq_file *seq);

/* the function decleared will be auto call when show task */
#define __DECLAER_FEATURE_AUTO_SHOW_TASK(module_name, show_task_fn, stage) \
    void module_name##_##stage##_task_show_##show_task_fn(u32 udevid, int pid, int feature_id, struct seq_file *seq); \
    void module_name##_##stage##_task_show_##show_task_fn(u32 udevid, int pid, int feature_id, struct seq_file *seq)  \
    { \
        show_task_fn(udevid, pid, feature_id, seq); \
    } \
    EXPORT_SYMBOL(module_name##_##stage##_task_show_##show_task_fn)
#define _DECLAER_FEATURE_AUTO_SHOW_TASK(module_name, show_task_fn, stage) \
    __DECLAER_FEATURE_AUTO_SHOW_TASK(module_name, show_task_fn, stage)
#define DECLAER_FEATURE_AUTO_SHOW_TASK(show_task_fn, stage) \
    _DECLAER_FEATURE_AUTO_SHOW_TASK(AUTO_INIT_MODULE_NAME, show_task_fn, stage)

static inline void module_feature_show_task_prefix(char *prefix, u32 udevid, int pid, int feature_id,
    struct seq_file *seq)
{
    struct module *module = THIS_MODULE;
    u32 i;

    for (i = 0; i < module->num_syms; i++) {
        void *fn = get_symbol_prefix_func(&module->syms[i], prefix);
        if (fn != NULL) {
            ((_fearture_show_task_func)fn)(udevid, pid, feature_id, seq);
        }
    }
}

static inline void module_feature_format_show_task_prefix(char *prefix, int len, int stage)
{
    (void)sprintf_s(prefix, PREFIX_MAX_LEN, "%s_FEATURE_LOADER_STAGE_%d_task_show_", AUTO_INIT_MODULE_NAME_STR, stage);
}

static inline void module_feature_show_task_stage(int stage, u32 udevid, int pid, int feature_id, struct seq_file *seq)
{
    char show_prefix[PREFIX_MAX_LEN];

    module_feature_format_show_task_prefix(show_prefix, PREFIX_MAX_LEN, stage);
    module_feature_show_task_prefix(show_prefix, udevid, pid, feature_id, seq);
}

/* pid=0, show global info */
static inline void module_feature_auto_show_task(u32 udevid, int pid, int feature_id, struct seq_file *seq)
{
    int stage;

    for (stage = FEATURE_LOADER_STAGE_0; stage <= FEATURE_LOADER_STAGE_9; stage++) {
        module_feature_show_task_stage(stage, udevid, pid, feature_id, seq);
    }
}

typedef void (*_fearture_show_dev_func)(u32 udevid, int feature_id, struct seq_file *seq);

/* the function decleared will be auto call when show dev */
#define __DECLAER_FEATURE_AUTO_SHOW_DEV(module_name, show_dev_fn, stage) \
    void module_name##_##stage##_dev_show_##show_dev_fn(u32 udevid, int feature_id, struct seq_file *seq); \
    void module_name##_##stage##_dev_show_##show_dev_fn(u32 udevid, int feature_id, struct seq_file *seq)  \
    { \
        show_dev_fn(udevid, feature_id, seq); \
    } \
    EXPORT_SYMBOL(module_name##_##stage##_dev_show_##show_dev_fn)
#define _DECLAER_FEATURE_AUTO_SHOW_DEV(module_name, show_dev_fn, stage) \
    __DECLAER_FEATURE_AUTO_SHOW_DEV(module_name, show_dev_fn, stage)
#define DECLAER_FEATURE_AUTO_SHOW_DEV(show_dev_fn, stage) \
    _DECLAER_FEATURE_AUTO_SHOW_DEV(AUTO_INIT_MODULE_NAME, show_dev_fn, stage)

static inline void module_feature_show_dev_prefix(char *prefix, u32 udevid, int feature_id, struct seq_file *seq)
{
    struct module *module = THIS_MODULE;
    u32 i;

    for (i = 0; i < module->num_syms; i++) {
        void *fn = get_symbol_prefix_func(&module->syms[i], prefix);
        if (fn != NULL) {
            ((_fearture_show_dev_func)fn)(udevid, feature_id, seq);
        }
    }
}

static inline void module_feature_format_show_dev_prefix(char *prefix, int len, int stage)
{
    (void)sprintf_s(prefix, PREFIX_MAX_LEN, "%s_FEATURE_LOADER_STAGE_%d_dev_show_", AUTO_INIT_MODULE_NAME_STR, stage);
}

static inline void module_feature_show_dev_stage(int stage, u32 udevid, int feature_id, struct seq_file *seq)
{
    char show_prefix[PREFIX_MAX_LEN];

    module_feature_format_show_dev_prefix(show_prefix, PREFIX_MAX_LEN, stage);
    module_feature_show_dev_prefix(show_prefix, udevid, feature_id, seq);
}

static inline void module_feature_auto_show_dev(u32 udevid, int feature_id, struct seq_file *seq)
{
    int stage;

    for (stage = FEATURE_LOADER_STAGE_0; stage <= FEATURE_LOADER_STAGE_9; stage++) {
        module_feature_show_dev_stage(stage, udevid, feature_id, seq);
    }
}

#endif

