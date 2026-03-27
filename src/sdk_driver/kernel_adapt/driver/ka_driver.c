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

#include "securec.h"
#include "ka_driver_pub.h"

int ka_driver_get_acpi_disabled(void)
{
    return acpi_disabled;
}
EXPORT_SYMBOL_GPL(ka_driver_get_acpi_disabled);

#ifndef __cplusplus
ka_class_t *ka_driver_class_create(ka_module_t *owner, const char *name)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
    return class_create(name);
#else
    return class_create(owner, name);
#endif
}
EXPORT_SYMBOL_GPL(ka_driver_class_create);

#ifndef EMU_ST
typedef char* (*ka_class_devnode_const)(const struct device *dev, umode_t *mode); // typedef function pointer
int ka_driver_class_set_devnode(ka_class_t *cls, ka_class_devnode devnode)
{
    if (cls == NULL || devnode == NULL) {
        return -EINVAL;
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 2, 0)
    cls->devnode = (ka_class_devnode_const)(devnode);
#else
    cls->devnode = devnode;
#endif
    return 0;
}
EXPORT_SYMBOL_GPL(ka_driver_class_set_devnode);
#endif

#endif

int ka_driver_dmi_find_devid(ka_pci_dev_t *pdev, int DMI_DEV_TYPE_DEV_SLOT, int *dev_id)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 5, 0)
#ifdef CONFIG_DMI
    const struct dmi_device *dmi_dev = NULL;
    const struct dmi_device *from = NULL;
    const struct dmi_dev_onboard *dev_data = NULL;
    do {
        from = dmi_dev;
        dmi_dev = dmi_find_device(DMI_DEV_TYPE_DEV_SLOT, NULL, from);
        if (dmi_dev != NULL) {
            dev_data = (struct dmi_dev_onboard *)dmi_dev->device_data;
#ifdef CONFIG_PCI_DOMAINS_GENERIC
            if ((dev_data != NULL) && (dev_data->bus == pdev->bus->number) &&
                (PCI_SLOT(((unsigned int)(dev_data->devfn))) == PCI_SLOT(pdev->devfn)) &&
                (dev_data->segment == pdev->bus->domain_nr)) {
                *dev_id = dev_data->instance;
                break;
            }
#else
            if ((dev_data != NULL) && (dev_data->bus == pdev->bus->number) &&
                (PCI_SLOT(((unsigned int)(dev_data->devfn))) == PCI_SLOT(pdev->devfn))) {
                *dev_id = dev_data->instance;
                break;
            }
#endif
        }
    } while (dmi_dev != NULL);
    return 0;
#endif
#else
    return -EOPNOTSUPP;
#endif
}
EXPORT_SYMBOL_GPL(ka_driver_dmi_find_devid);