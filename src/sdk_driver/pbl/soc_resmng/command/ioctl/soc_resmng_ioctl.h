/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef SOC_RESMNG_IOCTL_H
#define SOC_RESMNG_IOCTL_H

#define SOC_RESMNG_PATH "soc_resmng"
#define SOC_RESMNG_GET_VER_MAGIC 'v'
#define SOC_RESMNG_GET_DEV_VER _IOR(SOC_RESMNG_GET_VER_MAGIC, 0, int)
#define SOC_RESMNG_GET_HOST_VER _IOR(SOC_RESMNG_GET_VER_MAGIC, 1, int)

#define SOC_RESMNG_MODULE_NAME "RESMNG"

enum soc_ver_type {
    VER_TYPE_DEV = 0,
    VER_TYPE_HOST,
    VER_TYPE_MAX
};

int soc_resnmg_get_version(enum soc_ver_type type);

#endif