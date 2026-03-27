/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef BASE_DVPP_CMDLIST_INTERFACE_H
#define BASE_DVPP_CMDLIST_INTERFACE_H

#include "dvpp_cmdlist_sqe.h"
#ifndef __KERNEL__
#include <sys/ioctl.h>
#endif

#define IOC_TYPE_CMDLIST 'z'
#define DVPP_CMDLIST_DEVICE_NAME "dvpp_cmdlist"
#define DVPP_CMDLIST_DEVICE_NUM 1
#define DVPP_CMDLIST_DEVICE_CNT 1

typedef enum {
    IOC_NR_CMDLIST_GEN_CMDLIST = 0U,
    IOC_NR_CMDLIST_GET_VERSION = 1U,
    IOC_NR_CMDLIST_DEL_CHN_HLIST = 2U,
    IOC_NR_CMDLIST_BUTT
} ioc_nr_cmdlist;

typedef struct {
    uint32_t devid;   // 与用户使用aclrtSetDevice配置的device id一致，取值[0,63]
    uint32_t phyid;   // 打平后的device id，既可表示物理设备[0,63]，也可表示虚拟设备[100,1123]
    uint32_t sqe_len;
    void *sqe;
} dvpp_gen_cmdlist_user_data;

typedef struct {
    uint32_t major; // 主版本号，范围0x00-0xff，一个字节
    uint32_t minor; // 次版本号，范围0x00-0xff，一个字节
    uint32_t patch; // 补丁版本号，范围0x00-0xff，一个字节
} dvpp_cmdlist_version_user_data;

typedef struct {
    uint32_t chn_id;
} dvpp_cmdlist_hlist_user_data;

// ioctl 命令, 其中dvpp_sqe是用户态传到内核态的参数
#define GEN_CMDLIST _IOWR(IOC_TYPE_CMDLIST, IOC_NR_CMDLIST_GEN_CMDLIST, dvpp_gen_cmdlist_user_data)
#define GET_CMDLIST_VERSION _IOWR(IOC_TYPE_CMDLIST, IOC_NR_CMDLIST_GET_VERSION, dvpp_cmdlist_version_user_data)
#define DEL_CMDLIST_CHN_HLIST _IOWR(IOC_TYPE_CMDLIST, IOC_NR_CMDLIST_DEL_CHN_HLIST, dvpp_cmdlist_hlist_user_data)

#endif // #ifndef BASE_DVPP_CMDLIST_INTERFACE_H