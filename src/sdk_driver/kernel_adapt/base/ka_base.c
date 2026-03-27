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
#include <linux/random.h>
#include "securec.h"
#include "ka_memory_pub.h"
#include "ka_base_pub.h"
#include "ka_base.h"

unsigned short const *ka_base_get_crc16_table(void)
{
    return crc16_table;
}
EXPORT_SYMBOL_GPL(ka_base_get_crc16_table);

unsigned char const *_ka_base_get_ctype(void)
{
    return _ctype;
}
EXPORT_SYMBOL_GPL(_ka_base_get_ctype);

void *ka_base_pde_data(const ka_inode_t *inode)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 17, 0))
    return pde_data(inode);
#else
    return PDE_DATA(inode);
#endif
}
EXPORT_SYMBOL_GPL(ka_base_pde_data);

u32 ka_base_get_random_u32(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 12, 0)
    return get_random_u32();
#else
    return get_random_int();
#endif
}
EXPORT_SYMBOL_GPL(ka_base_get_random_u32);

void ka_base_set_cdev_owner(ka_cdev_t *cdev, ka_module_t *owner)
{
    cdev->owner = owner;
}
EXPORT_SYMBOL_GPL(ka_base_set_cdev_owner);

ka_module_t *ka_base_find_module(const char *name)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0)
    return NULL;
#else
    return find_module(name);
#endif
}
EXPORT_SYMBOL_GPL(ka_base_find_module);

func_by_string ka_base_register_func_by_string(bool (*func_high_v)(const char *name),
    bool (*func_low_v)(const char *name))
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0)
    return func_high_v;
#else
    return func_low_v;
#endif
}
EXPORT_SYMBOL_GPL(ka_base_register_func_by_string);

func_by_pdev ka_base_register_func_by_pdev(bool (*func_high_v)(ka_pci_dev_t *pdev),
    bool (*func_low_v)(ka_pci_dev_t *pdev))
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 2, 8)
    return func_high_v;
#else
    return func_low_v;
#endif
}
EXPORT_SYMBOL_GPL(ka_base_register_func_by_pdev);