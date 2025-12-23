/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef PBL_PROF_INTERFACE_CMD_H
#define PBL_PROF_INTERFACE_CMD_H
#define PROF_OK (0)
#define PROF_ERROR (-1)
#define PROF_STOPPED_ALREADY (-4)
#define PROF_NOT_SUPPORT (-7)
#define PROF_NOT_ENOUGH_SUB_CHANNEL_RESOURCE (-10)
#define PROF_VF_SUB_RESOURCE_FULL (-11)
#define PROF_CHANNEL_NUM 160

struct channels_info {
    unsigned short remote_pid;
    unsigned short channel_id;
};

struct prof_channel_list {
    unsigned int channel_num;
    struct channels_info channel[PROF_CHANNEL_NUM];
};
#endif
