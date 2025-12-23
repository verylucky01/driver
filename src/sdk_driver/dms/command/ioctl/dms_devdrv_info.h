/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef DMS_DEVDRV_INFO_H
#define DMS_DEVDRV_INFO_H

struct dmanage_pcie_id_info {
    unsigned int venderid;    /* vendor id */
    unsigned int subvenderid; /* sub vendor id */
    unsigned int deviceid;
    unsigned int subdeviceid;
    unsigned int bus;         /* bus id */
    unsigned int device;      /* physical id of device */
    unsigned int fn;          /* function id of device */
    unsigned int davinci_id;  /* logical id */
};

#endif
