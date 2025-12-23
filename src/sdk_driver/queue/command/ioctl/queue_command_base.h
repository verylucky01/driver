/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef QUEUE_COMMAND_BASE_H
#define QUEUE_COMMAND_BASE_H

struct queue_ctrl_data_search {
    unsigned long long serial_num;
    unsigned int ctrl_data_len;
    unsigned int host_timestamp;
};

#endif
