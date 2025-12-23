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
#include <linux/sysfs.h>
#include <linux/stat.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>
#include "pbl/pbl_davinci_api.h"
#include "dms_sysfs.h"
#include "pbl_mem_alloc_interface.h"
#include "fms_kernel_interface.h"
#include "urd_feature.h"
#include "dms_sensor.h"
#include "dms_event_dfx.h"
#if defined(CFG_BUILD_DEBUG) && defined(CFG_FEATURE_IMU_CORE)
#include "dms_imu_heartbeat.h"
#endif

#define DMS_ATTR_RD (00400 | 00040)
#define DMS_ATTR_WR (00200 | 00020)
#define DMS_ATTR_RW (DMS_ATTR_RD | DMS_ATTR_WR)
#define MAX_CAT_BUFSIZE 2010
#define PRINTT_LINE 30

STATIC ssize_t dms_sysfs_feature_list_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    if (buf == NULL) {
        dms_err("buf is NULL.\n");
        return 0;
    }

    return dms_feature_print_feature_list(buf);
}

STATIC ssize_t dms_sysfs_node_list_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    if (buf == NULL) {
        dms_err("buf is NULL.\n");
        return 0;
    }

    return dms_devnode_print_node_list(buf);
}

STATIC ssize_t dms_sysfs_sensor_list_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return dms_sensor_print_sensor_list(buf);
}

STATIC ssize_t dms_sysfs_sensor_mask_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return 0;
}

STATIC ssize_t dms_sysfs_sensor_mask_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    return 0;
}

STATIC ssize_t dms_sysfs_channel_flux_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return dms_event_dfx_channel_flux_show(buf);
}

STATIC ssize_t dms_sysfs_channel_flux_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    return dms_event_dfx_channel_flux_store(buf, count);
}

STATIC ssize_t dms_sysfs_convergent_diagrams_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return dms_event_dfx_convergent_diagrams_show(buf);
}

STATIC ssize_t dms_sysfs_convergent_diagrams_store(struct device *dev, struct device_attribute *attr,
    const char *buf, size_t count)
{
    return dms_event_dfx_convergent_diagrams_store(buf, count);
}

STATIC ssize_t dms_sysfs_event_list_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return dms_event_dfx_event_list_show(buf);
}

STATIC ssize_t dms_sysfs_event_list_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    return dms_event_dfx_event_list_store(buf, count);
}

STATIC ssize_t dms_sysfs_mask_list_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return dms_event_dfx_mask_list_show(buf);
}

STATIC ssize_t dms_sysfs_mask_list_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    return dms_event_dfx_mask_list_store(buf, count);
}

STATIC ssize_t dms_sysfs_subscribe_handle_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return dms_event_dfx_subscribe_handle_show(buf);
}

STATIC ssize_t dms_sysfs_subscribe_process_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return dms_event_dfx_subscribe_process_show(buf);
}

#if defined(CFG_BUILD_DEBUG) && !defined(CFG_HOST_ENV) && defined(CFG_FEATURE_IMU_CORE)
ssize_t dms_sysfs_imu_hb_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return dms_imu_hb_show(buf);
}

ssize_t dms_sysfs_imu_hb_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    return dms_imu_hb_store(buf, count);
}
#endif

#define DRV_DMS_FS_ATTR DEVICE_ATTR
#define DRV_FS_ATTR_NAME_POINTER(_name)  &dev_attr_##_name.attr

#define DECLARE_DMS_FS_ATTR \
    static DRV_DMS_FS_ATTR(feature_list, DMS_ATTR_RD, dms_sysfs_feature_list_show, NULL); \
    static DRV_DMS_FS_ATTR(node_list, DMS_ATTR_RD, dms_sysfs_node_list_show, NULL); \
    static DRV_DMS_FS_ATTR(sensor_list, DMS_ATTR_RD, dms_sysfs_sensor_list_show, NULL); \
    static DRV_DMS_FS_ATTR(sensor_trace, DMS_ATTR_RW, dms_sysfs_sensor_mask_show, dms_sysfs_sensor_mask_store); \
    static DRV_DMS_FS_ATTR(channel_flux, DMS_ATTR_RW, dms_sysfs_channel_flux_show, dms_sysfs_channel_flux_store); \
    static DRV_DMS_FS_ATTR(convergent_diagrams, DMS_ATTR_RW, dms_sysfs_convergent_diagrams_show, \
                                                             dms_sysfs_convergent_diagrams_store); \
    static DRV_DMS_FS_ATTR(event_list, DMS_ATTR_RW, dms_sysfs_event_list_show, dms_sysfs_event_list_store); \
    static DRV_DMS_FS_ATTR(mask_list, DMS_ATTR_RW, dms_sysfs_mask_list_show, dms_sysfs_mask_list_store); \
    static DRV_DMS_FS_ATTR(subscribe_handle, DMS_ATTR_RD, dms_sysfs_subscribe_handle_show, NULL); \
    static DRV_DMS_FS_ATTR(subscribe_process, DMS_ATTR_RD, dms_sysfs_subscribe_process_show, NULL)

#define DECLARE_DMS_FS_ATTR_LIST(_struct_type, _profix) \
    static struct _struct_type *g_dms_##_profix##_attr_list[] = { \
        DRV_FS_ATTR_NAME_POINTER(feature_list), \
        DRV_FS_ATTR_NAME_POINTER(node_list), \
        DRV_FS_ATTR_NAME_POINTER(sensor_list), \
        DRV_FS_ATTR_NAME_POINTER(sensor_trace), \
        DRV_FS_ATTR_NAME_POINTER(channel_flux), \
        DRV_FS_ATTR_NAME_POINTER(convergent_diagrams), \
        DRV_FS_ATTR_NAME_POINTER(event_list), \
        DRV_FS_ATTR_NAME_POINTER(mask_list), \
        DRV_FS_ATTR_NAME_POINTER(subscribe_handle), \
        DRV_FS_ATTR_NAME_POINTER(subscribe_process), \
        NULL, \
    }


#if (defined CFG_BUILD_DEBUG) && (!defined(CFG_HOST_ENV)) && defined(CFG_FEATURE_IMU_CORE)
#define DECLARE_DEBUG_DMS_FS_ATTR \
    static DRV_DMS_FS_ATTR(imu_hb, DMS_ATTR_RW, dms_sysfs_imu_hb_show, dms_sysfs_imu_hb_store)
#endif

DECLARE_DMS_FS_ATTR;
DECLARE_DMS_FS_ATTR_LIST(attribute, sysfs);

#if (defined CFG_BUILD_DEBUG) && (!defined(CFG_HOST_ENV)) && defined(CFG_FEATURE_IMU_CORE)
DECLARE_DEBUG_DMS_FS_ATTR;
#endif

static const struct attribute_group g_dms_sysfs_group = {
    .attrs = g_dms_sysfs_attr_list,
    .name = "dms"
};

void dms_sysfs_init(void)
{
    int ret;
    struct device *dev = davinci_intf_get_owner_device();
    if (dev == NULL) {
        dms_err("dms dev is NULL, sysfs init failed.\n");
        return;
    }

    ret = sysfs_create_group(&dev->kobj, &g_dms_sysfs_group);
    if (ret != 0) {
        dms_err("dms sysfs create group failed.\n");
        return;
    }
#if (defined CFG_BUILD_DEBUG) && (!defined(CFG_HOST_ENV)) && defined(CFG_FEATURE_IMU_CORE)
    ret = sysfs_add_file_to_group(&dev->kobj, &dev_attr_imu_hb.attr, g_dms_sysfs_group.name);
    if (ret != 0) {
        dms_warn("dms sysfs add imu hb node warn, (ret=%d)\n", ret);
    }
#endif
    dms_info("dms sysfs init.\n");
    return;
}

void dms_sysfs_uninit(void)
{
    struct device *dev = davinci_intf_get_owner_device();
    if (dev == NULL) {
        return;
    }
    sysfs_remove_group(&dev->kobj, &g_dms_sysfs_group);
}
