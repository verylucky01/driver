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

#include "vfio_ops.h"
#include "dvt.h"
#include "dvt_sysfs.h"

STATIC struct hw_vdavinci_type *get_vdavinci_type(struct device *dev,
                                                  const char *name)
{
    struct hw_vdavinci_type *type = NULL;
    struct vdavinci_priv *priv = NULL;

    if (dev == NULL || name == NULL) {
        return NULL;
    }
    priv = kdev_to_davinci(dev);
    if (priv == NULL || priv->dvt == NULL) {
        return NULL;
    }
    type = g_hw_vdavinci_ops.dvt_find_vdavinci_type(priv->dvt, name);

    return type;
}

unsigned int available_instances_ops(struct device *dev, const char *name)
{
    struct hw_vdavinci_type *type = NULL;

    type = get_vdavinci_type(dev, name);
    if (type == NULL) {
        return 0;
    }

    return type->avail_instance;
}

ssize_t description_ops(struct device *dev, const char *name, char *buf)
{
    struct hw_vdavinci_type *type = NULL;

    if (buf == NULL) {
        return 0;
    }
    type = get_vdavinci_type(dev, name);
    if (type == NULL) {
        return 0;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1,
        "type name: %s\naicore num: %u\nmemory size: %luGB\naicpu num: %u\nvpc num: %u\n"\
        "jpegd num: %u\njpege num: %u\nvenc num: %u\nvdec num: %u\nbar2 size: %dKB\nbar4 size: %dKB\n",
        type->template_name, type->aicore_num, type->mem_size, type->aicpu_num, type->vpc_num,
        type->jpegd_num, type->jpege_num, type->venc_num, type->vdec_num,
        BYTES_TO_KB(type->bar2_size), BYTES_TO_KB(type->bar4_size));
}

ssize_t device_api_ops(char *buf)
{
    int ret;

    if (buf == NULL) {
        return 0;
    }
    ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%s\n",
                     VFIO_DEVICE_API_PCI_STRING);
    return ret == -1 ? 0 : ret;
}

ssize_t vfg_id_store_ops(struct device *dev, const char *name,
                         const char *buf, size_t count)
{
    unsigned int vfg_id;
    struct hw_vdavinci_type *type = NULL;

    if (buf == NULL) {
        return 0;
    }
    type = get_vdavinci_type(dev, name);
    if (type == NULL) {
        return -EINVAL;
    }

    if (kstrtouint(buf, 0, &vfg_id) < 0 || vfg_id >= VDAVINCI_VFG_MAX) {
        vascend_err(dev, "Invalid vfg_id %u\n", vfg_id);
        return -EINVAL;
    }

    type->vfg_id = vfg_id;

    return count;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6,1,0))
unsigned int available_instances_show(struct mdev_type *mtype)
{
    if (mtype == NULL || mtype->parent == NULL) {
        return 0;
    }

    return available_instances_ops(mtype->parent->dev, kobject_name(&mtype->kobj));
}

ssize_t description_show(struct mdev_type *mtype, char *buf)
{
    if (mtype == NULL || mtype->parent == NULL) {
        return 0;
    }

    return description_ops(mtype->parent->dev, kobject_name(&mtype->kobj), buf);
}
#else
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,13,0))
STATIC ssize_t device_api_show(struct mdev_type *mtype, struct mdev_type_attribute *attr,
                               char *buf)
{
    if (buf == NULL) {
        return 0;
    }

    return device_api_ops(buf);
}

STATIC ssize_t available_instances_show(struct mdev_type *mtype,
                                        struct mdev_type_attribute *attr,
                                        char *buf)
{
    unsigned int num = 0;
    struct kobject *kobj = NULL;

    if (buf == NULL) {
        return 0;
    }
    if (mtype == NULL) {
        goto out;
    }
    kobj = (struct kobject *)mtype;
    if (kobj->name == NULL) {
        goto out;
    }

    num = available_instances_ops(mtype_get_parent_dev(mtype),
                                  kobject_name(kobj));
out:
	return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%u\n", num);
}

STATIC ssize_t description_show(struct mdev_type *mtype,
				                struct mdev_type_attribute *attr,
                                char *buf)
{
    struct kobject *kobj = NULL;

    if (mtype == NULL || buf == NULL) {
        return 0;
    }
    kobj = (struct kobject *)mtype;
    if (kobj->name == NULL) {
        return 0;
    }

    return description_ops(mtype_get_parent_dev(mtype),
                           kobject_name(kobj), buf);
}

STATIC ssize_t vfg_id_store(struct mdev_type *mtype,
                            struct mdev_type_attribute *attr,
                            const char *buf, size_t count)
{
    struct kobject *kobj = NULL;

    if (mtype == NULL || buf == NULL) {
        return 0;
    }
    kobj = (struct kobject *)mtype;
    if (kobj->name == NULL) {
        return 0;
    }

    return vfg_id_store_ops(mtype_get_parent_dev(mtype),
                            kobject_name(kobj), buf, count);
}


#else
STATIC ssize_t device_api_show(struct kobject *kobj, struct device *dev,
                               char *buf)
{
    if (buf == NULL) {
        return 0;
    }

    return device_api_ops(buf);
}

STATIC ssize_t available_instances_show(struct kobject *kobj, struct device *dev,
                                        char *buf)
{
    unsigned int num = 0;
    int ret = -1;

    if (buf == NULL) {
        return 0;
    }

    if (kobj == NULL || dev == NULL) {
        goto out;
    }

    num = available_instances_ops(dev, kobject_name(kobj));

out:
    ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%u\n", num);
    return ret == -1 ? 0 : ret;
}

STATIC ssize_t description_show(struct kobject *kobj, struct device *dev,
                                char *buf)
{
    if (kobj == NULL || dev == NULL || buf == NULL) {
        return 0;
    }

    return description_ops(dev, kobject_name(kobj), buf);
}

STATIC ssize_t vfg_id_store(struct kobject *kobj, struct device *dev,
                            const char *buf, size_t count)
{
    if (kobj == NULL || dev == NULL || buf == NULL) {
        return -EINVAL;
    }

    return vfg_id_store_ops(dev, kobject_name(kobj), buf, count);
}
#endif /* KERNEL_VERSION(5,13,0) */
static MDEV_TYPE_ATTR_RO(device_api);
static MDEV_TYPE_ATTR_RO(available_instances);
static MDEV_TYPE_ATTR_RO(description);
static MDEV_TYPE_ATTR_WO(vfg_id);

struct attribute *hw_vdavinci_type_attrs[] = {
    &mdev_type_attr_device_api.attr,
    &mdev_type_attr_available_instances.attr,
    &mdev_type_attr_description.attr,
    &mdev_type_attr_vfg_id.attr,
    NULL
};

struct attribute **get_hw_vdavinci_type_attrs(void)
{
    return hw_vdavinci_type_attrs;
}
#endif /* KERNEL_VERSION(6,1,0) */

STATIC ssize_t
vdavinci_id_show(struct device *dev, struct device_attribute *attr,
                 char *buf)
{
    struct mdev_device *mdev = get_mdev_device(dev);
    int ret;

    if (mdev != NULL) {
        struct hw_vdavinci *vdavinci = (struct hw_vdavinci *)
            get_mdev_drvdata(dev);
        ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%u\n", vdavinci->id);
        return ret == -1 ? 0 : ret;
    }

    ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "\n");
    return ret == -1 ? 0 : ret;
}

STATIC ssize_t
hw_id_show(struct device *dev, struct device_attribute *attr,
           char *buf)
{
    struct mdev_device *mdev = get_mdev_device(dev);
    int ret;

    if (mdev != NULL) {
        struct hw_vdavinci *vdavinci = (struct hw_vdavinci *)
            get_mdev_drvdata(dev);
        ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1,
            "Device [%x:%x]\n", vdavinci->dvt->vendor,
            vdavinci->dvt->device);
        return ret == -1 ? 0 : ret;
    }

    return 0;
}

STATIC ssize_t
vfg_id_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct mdev_device *mdev = get_mdev_device(dev);
    struct hw_vdavinci *vdavinci = NULL;
    int ret;
    if (mdev == NULL) {
        return -ENOENT;
    }
    vdavinci = (struct hw_vdavinci *)get_mdev_drvdata(dev);
    ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%u\n", vdavinci->vfg_id);

    return ret == -1 ? 0 : ret;
}

STATIC ssize_t
vf_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct mdev_device *mdev = get_mdev_device(dev);
    struct hw_vdavinci *vdavinci = NULL;
    struct pci_dev *pdev = NULL;
    struct hw_dvt *dvt;
    int ret;
    if (mdev == NULL) {
        return -ENOENT;
    }

    vdavinci = (struct hw_vdavinci *)get_mdev_drvdata(dev);
    dvt = vdavinci->dvt;
    if (dvt->is_sriov_enabled && dvt->sriov.vf_num > 0) {
        pdev = vdavinci->vf.pdev;
        ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1,
            "%02x:%02x.%d\n", pdev->bus->number, PCI_SLOT(pdev->devfn),
            PCI_FUNC(pdev->devfn));
        return ret == -1 ? 0 : ret;
    }
    ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "\n");
    return ret == -1 ? 0 : ret;
}
static DEVICE_ATTR_RO(vfg_id);
static DEVICE_ATTR_RO(vdavinci_id);
static DEVICE_ATTR_RO(hw_id);
static DEVICE_ATTR_RO(vf);

static struct attribute *hw_vdavinci_attrs[] = {
    &dev_attr_vdavinci_id.attr,
    &dev_attr_hw_id.attr,
    &dev_attr_vfg_id.attr,
    &dev_attr_vf.attr,
    NULL
};

static struct attribute_group hw_vdavinci_group = {
    .name = "hw_vdavinci",
    .attrs = hw_vdavinci_attrs,
};

const struct attribute_group *hw_vdavinci_groups[] = {
    &hw_vdavinci_group,
    NULL,
};

const struct attribute_group **get_hw_vdavinci_groups(void)
{
    return hw_vdavinci_groups;
}
