/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef TRS_SHR_ID_IOCTL_H
#define TRS_SHR_ID_IOCTL_H
#include <asm/ioctl.h>

#include "drv_type.h"

#define DAVINCI_INTF_MODULE_TRS_SHR_ID "TRS_SHR_ID"

#define SHR_ID_NSM_NAME_SIZE    65
#define SHR_ID_PID_MAX_NUM  16
struct shr_id_ioctl_info {
    u32 opened_devid; /* out:logic devid */
    u32 devid;
    u32 tsid;
    u32 shr_id;
    u32 id_type;

    u64 dev_addr;
    u64 host_addr;
    char name[SHR_ID_NSM_NAME_SIZE];
    pid_t pid[SHR_ID_PID_MAX_NUM];
    u32 flag; /* in:remote flag, out:event flag */
    u32 enable_flag;
    u32 rsv[4];
};

struct shr_id_pod_pid_ioctl_info {
    char name[SHR_ID_NSM_NAME_SIZE];

    unsigned int sdid;
    int pid;
    u32 rsv[4];
};

#define SHR_ID_NOTIFY_MAGIC 'N'
#define SHR_ID_CREATE _IOWR(SHR_ID_NOTIFY_MAGIC, 1, struct shr_id_ioctl_info)
#define SHR_ID_OPEN _IOWR(SHR_ID_NOTIFY_MAGIC, 2, struct shr_id_ioctl_info)
#define SHR_ID_CLOSE _IOWR(SHR_ID_NOTIFY_MAGIC, 3, struct shr_id_ioctl_info)
#define SHR_ID_DESTROY _IOW(SHR_ID_NOTIFY_MAGIC, 4, struct shr_id_ioctl_info)
#define SHR_ID_SET_PID _IOW(SHR_ID_NOTIFY_MAGIC, 5, struct shr_id_ioctl_info)
#define SHR_ID_RECORD _IOW(SHR_ID_NOTIFY_MAGIC, 6, struct shr_id_ioctl_info)
#define SHR_ID_SET_POD_PID _IOW(SHR_ID_NOTIFY_MAGIC, 7, struct shr_id_pod_pid_ioctl_info)
#define SHR_ID_SET_ATTR _IOW(SHR_ID_NOTIFY_MAGIC, 8, struct shr_id_ioctl_info)
#define SHR_ID_GET_ATTR _IOWR(SHR_ID_NOTIFY_MAGIC, 9, struct shr_id_ioctl_info)
#define SHR_ID_GET_INFO _IOWR(SHR_ID_NOTIFY_MAGIC, 10, struct shr_id_ioctl_info)

#define SHR_ID_MAX_CMD   11

#endif

