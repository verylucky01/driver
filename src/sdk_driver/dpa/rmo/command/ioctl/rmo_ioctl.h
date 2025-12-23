/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef RMO_CMD_H
#define RMO_CMD_H

#include <linux/ioctl.h>
#include "ascend_hal_define.h"

#define RMO_CHAR_DEV_NAME "rmo"

#define RMO_LOGIC_DEV_MAX_NUM 64

struct rmo_cmd_res_map {
    unsigned int devid;             /* input */
    struct res_map_info res_info;   /* input */
    unsigned long va;               /* input */
    unsigned int len;               /* input */
    bool repeat_map_flag;           /* output */
    unsigned int rsv[4];            /* reserve */
};

struct rmo_cmd_res_io {
    unsigned int devid;     /* input */
    struct res_map_info res_info; /* input */
    unsigned int len; /* input */
    void *data;       /* output or input*/
    unsigned int rsv[4];    /* reserve */
};

struct rmo_cmd_mem_sharing {
    unsigned int devid;
    enum drv_mem_side side;
    void *ptr;
    unsigned long long size;
    accessMember_t accessor;
    unsigned int pg_prot;
    unsigned int enable_flag; /* 0:enable; 1:disable */
    unsigned int rsv[4];      /* reserve */
};

/* resouce map cmd */
#define RMO_RES_MAP        _IOWR('U', 0, struct rmo_cmd_res_map)
#define RMO_RES_UNMAP      _IOWR('U', 1, struct rmo_cmd_res_map)
#define RMO_GET_RES_LEN    _IOWR('U', 2, struct rmo_cmd_res_map)

/* resouce read write cmd */
#define RMO_RES_READ       _IOWR('U', 3, struct rmo_cmd_res_io)
#define RMO_RES_WRITE      _IOW('U', 4, struct rmo_cmd_res_io)

#define RMO_MEM_SHARING    _IOR('U', 5, struct rmo_cmd_mem_sharing)

#define RMO_MAX_CMD             6

#endif