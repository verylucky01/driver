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

#ifndef APM_KERNEL_IOCTL_H
#define APM_KERNEL_IOCTL_H

#include "ascend_hal_define.h"

/* all *pid means:
   If handled by the local os, it represents the pid from getpid(), otherwise it represents the kernel's tgid.
   mode                  bind/unbind   query_slave(device)  query_slave(host)  query_master(device)  query_master(host)
                         master slave  master      slave    master      slave  master      slave     master    slave
   offline               pid     pid   pid         pid                         pid         pid
   online(device slave)  tgid    pid   tgid        pid      pid         tgid   tgid        pid
   online(host slave)    pid     pid                        pid         pid                          pid       pid

   The kernel uses the tgid as the key to store binding information.
   input devid is logic devid
   output devid is udevid

   bind sequence: Update the slave tables and then the master tables. Update the local before updating the remote.
       offline: slave tables ---> master tables
       online(device slave): device slave tables ---> host slave proxy tables ---> host master tables
                             ---> device master proxy tables
       online(host slave): host slave tables ---> host master tables ---> device slave proxy tables
                             ---> device master proxy tables
   query: consider the bind sequence, if the query in the proxy tables fails, we will query the original
          tables at the remote end.
*/

#define APM_CHAR_DEV_NAME "apm"

#define APM_LOGIC_DEV_MAX_NUM 64

struct apm_cmd_get_sign {
    unsigned int cur_tgid; /* output. global tgid */
    unsigned int rsv[4]; /* reserve */
};

struct apm_cmd_query_slave_pid {
    unsigned int devid;     /* input */
    int proc_type;          /* input */
    int master_pid;         /* input */
    unsigned int query_in_all_stage; /* input */
    int slave_pid;          /* output */
    int slave_tgid;         /* output */
    unsigned int rsv[4];    /* reserve */
};

struct apm_cmd_bind {
    unsigned int devid;     /* input */
    int proc_type;          /* input */
    int mode;               /* input */
    int master_pid;         /* input */
    int slave_pid;          /* input */
    unsigned int rsv[4];    /* reserve */
};

struct apm_cmd_query_master_info {
    unsigned int slave_pid; /* input */
    unsigned int master_pid; /* output */
    unsigned int master_tgid; /* output */
    unsigned int udevid;    /* output (APM_QUERY_MASTER_INFO_BY_DEVICE_SLAVE cmd is reused as input logic devid) */
    int mode;               /* output */
    unsigned int proc_type_bitmap; /* output */
    unsigned int rsv[4];    /* reserve */
};

struct apm_cmd_res_map {
    unsigned int devid;     /* input */
    struct res_map_info_in res_info; /* input */
    unsigned long va;       /* output */
    unsigned int len;       /* output */
    unsigned int rsv[8];    /* reserve */
};

struct apm_cmd_res_unmap {
    unsigned int devid;     /* input */
    struct res_map_info_in res_info; /* input */
    unsigned int rsv[8];    /* reserve */
};

enum apm_cmd_slave_status_type {
    CMD_SLAVE_STATUS_TYPE_OOM,
    CMD_SLAVE_STATUS_TYPE_MAX
};

struct apm_cmd_slave_status {
    unsigned int devid;     /* input */
    int proc_type;          /* input */
    enum apm_cmd_slave_status_type type; /* input */
    int status;             /* output */
    unsigned int rsv[4];    /* reserve */
};

struct apm_cmd_slave_ssid {
    unsigned int devid;
    int proc_type;
    int ssid;
    unsigned int rsv[4];    /* reserve */
};

typedef enum tagProcMemType {
    PROC_MEM_TYPE_ALL = 0,
    PROC_MEM_TYPE_VMRSS,
    PROC_MEM_TYPE_SP,
    PROC_MEM_MAX
} processMemType_t;

struct apm_cmd_slave_meminfo {
    unsigned int devid;     /* input */
    int proc_type;          /* input */
    int master_pid;         /* input */
    processMemType_t type;  /* input */
    unsigned long long size;          /* output */
    unsigned int rsv[4];    /* reserve */
};

struct apm_cmd_bind_cgroup {
    BIND_CGROUP_TYPE bind_type;     /* input */
    unsigned int rsv[4];            /* reserve */
};

/* bind master cmd */
#define APM_GET_SIGN            _IOR('U', 0, struct apm_cmd_get_sign)
/* query slave from user app master tables.
   rc: device master tables; ep: host master tables(from host query), host master proxy tables(from device query) */
#define APM_QUERY_SLAVE_PID     _IOWR('U', 1, struct apm_cmd_query_slave_pid)
/* query slave from local(tsd) master tables.
   rc: device master tables; ep: host master tables(from host query), device master tables(from device query) */
#define APM_QUERY_SLAVE_PID_BY_LOCAL_MASTER     _IOWR('U', 2, struct apm_cmd_query_slave_pid)

/* bind slave cmd */
#define APM_BIND                _IOW('U', 3, struct apm_cmd_bind)
#define APM_UNBIND              _IOW('U', 4, struct apm_cmd_bind)
/* query master from local slave tables.
   rc: device slave tables; ep: host slave tables(from host query), device slave tables(from device query) */
#define APM_QUERY_MASTER_INFO   _IOWR('U', 5, struct apm_cmd_query_master_info)
/* query master from remote slave proxy tables.
   rc: not support; ep: device slave proxy tables(from host query), host slave proxy tables(from device query) */
#define APM_QUERY_MASTER_INFO_BY_DEVICE_SLAVE _IOWR('U', 6, struct apm_cmd_query_master_info) /* from host query */
#define APM_QUERY_MASTER_INFO_BY_HOST_SLAVE   _IOWR('U', 7, struct apm_cmd_query_master_info) /* from device query */

/* resource map cmd */
#define APM_RES_ADDR_MAP        _IOWR('U', 8, struct apm_cmd_res_map)
#define APM_RES_ADDR_UNMAP      _IOW('U', 9, struct apm_cmd_res_unmap)

/* query task status */
#define APM_QUERY_SLAVE_STATUS  _IOWR('U', 12, struct apm_cmd_slave_status)

/* query slave ssid */
#define APM_QUERY_SSID          _IOWR('U', 13, struct apm_cmd_slave_ssid)

/* query task meminfo */
#define APM_QUERY_SLAVE_MEMINFO _IOWR('U', 14, struct apm_cmd_slave_meminfo)

/* task bind cgroup */
#define APM_BIND_CGROUP         _IOW('U', 15, struct apm_cmd_bind_cgroup)

#define APM_MAX_CMD             16

#endif

