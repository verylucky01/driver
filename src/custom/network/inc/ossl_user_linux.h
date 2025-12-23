/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef OSSL_USER_LINUX_H
#define OSSL_USER_LINUX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>
#include <limits.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#ifndef CONFIG_LLT
#include "slog.h"
#include "dmc_log_user.h"
#endif
#include "user_log.h"

#undef NULL
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif

#ifndef F_OK
#define F_OK 0
#endif
#define TOOL_REAL_PATH_MAX_LEN    512

#define MAX_IP_LEN 20
#define MAX_IP6_LEN 48
#define MAX_DSCP_LEN    384
#define HCCN_MIN(a, b)    (((a) < (b)) ? (a) : (b))

enum {
    UDA_SUCCESS = 0x0,
    UDA_FAIL,
    UDA_ENXIO,
    UDA_ENONMEM,
    UDA_EBUSY,
    UDA_ECRC,
    UDA_EINVAL,
    UDA_EFAULT,
    UDA_ELEN,
    UDA_ECMD,
    UDA_ENODRIVER,
    UDA_EXIST,
    UDA_EOVERSTEP,
    UDA_ENOOBJ,
    UDA_EOBJ,
    UDA_ENOMATCH,
    UDA_ETIMEOUT,
    UDA_HALF_CMD_ERR,
    UDA_HELP_PRINT,
    UDA_FORMATE_ERR,
    UDA_DEV_ERR,

    UDA_REBOOT = 0xFD,
    UDA_CANCEL = 0xFE,
    UDA_KILLED = 0xFF
};

enum {
    UDA_EXE_SUCCESS = 0x0,

    UDA_MAJOR_CMD_ERR = 0x1000,
    UDA_MAJOR_CMD_TYPE_ERR,
    UDA_MAJOR_CMD_NUM_ERR,
    UDA_MAJOR_CMD_INVALID_ERR,
    UDA_MAJOR_CMD_DEV_ID_ERR,
    UDA_MAJOR_CMD_LEN_ERR,

    UDA_PARAM_ERR = 0x2000,
    UDA_PARAM_INVALID_ERR,
    UDA_PARAM_PATH_INVALID_ERR,
    UDA_PARAM_OP_NOT_SUPPORT_ERR,
    UDA_PARAM_CMD_COUNT_ERR,
    UDA_PARAM_MAC_ADDR_INVALID_ERR,
    UDA_PARAM_IP_INVALID_ERR,
    UDA_PARAM_CONFLICT_IP_ADDR_ERR,
    UDA_PARAM_DIFF_SEGMT_GTWY_ERR,
    UDA_PARAM_DSCP_OUT_OF_RANGE_ERR,
    UDA_PARAM_TC_OUT_OF_RANGE_ERR,
    UDA_PARAM_BW_LIMIT_OUT_OF_RANGE_ERR,
    UDA_PARAM_ROCE_CTXT_OUT_OF_RANGE_ERR,
    UDA_PARAM_MTU_OUT_OF_RANGE_ERR,
    UDA_PARAM_TLS_NO_CERT_ERR,
    UDA_PARAM_TLS_INVALID_ALARM_ERR,
    UDA_PARAM_TLS_INVALID_ENABLE_ERR,
    UDA_PARAM_TLS_CA_WTHOT_PRI_ERR,
    UDA_PARAM_TLS_PRI_WITHOUT_PUB_ERR,
    UDA_PARAM_TLS_CERT_DISCONSEQ_ERR,
    UDA_PARAM_TLS_CRL_NOT_LAST_ITEM_ERR,
    UDA_PARAM_TLS_PWD_LEN_INVALID_ERR,
    UDA_PARAM_TLS_PWD_COMPLEXITY_ERR,
    UDA_PARAM_TLS_PWD_TOO_WEAK_ERR,
    UDA_PARAM_TLS_CERT_LEN_INVALID_ERR,
    UDA_PARAM_TLS_NO_WEAK_PWD_DICT,
    UDA_PARAM_TLS_WEAK_PWD_DICT_NULL,
    UDA_PARAM_TLS_WEAK_PWD_DICT_TOO_MANY,
    UDA_PARAM_TLS_WEAK_PWD_DICT_CHECK_FAIL,
    UDA_PARAM_TLS_HOST_NOT_LAST_ITEM_ERR,
    UDA_PARAM_TLS_CA_MEM_FULL,
    UDA_PARAM_TLS_CA_MEM_EMPTY,
    UDA_PARAM_TLS_CA_SAME_ALIAS,
    UDA_PARAM_TLS_HOST_INSERT_CA_ERR,
    UDA_PARAM_TLS_CA_ALIAS_NOT_EXIST,
    UDA_PARAM_DIFF_SEGMT_VIA_ERR,

    UDA_TOOL_ERR = 0x3000,
    UDA_TOOL_NO_MEM_ERR,
    UDA_TOOL_INNER_PARAM_ERR,
    UDA_TOOL_SYS_FOPEN_ERR,
    UDA_TOOL_SYS_WRITE_FILE_ERR,
    UDA_TOOL_SYS_READ_FILE_ERR,
    UDA_TOOL_SYS_RD_FILE_NOT_FOUND,
    UDA_TOOL_SYS_DELETE_FILE_ERR,
    UDA_TOOL_SYS_TIME_OP_ERR,
    UDA_TOOL_SYS_CERT_EXPRD_ERR,
    UDA_TOOL_SYS_TERMIOS_ERR,
    UDA_TOOL_SYS_BUSY_ERR,
    UDA_TOOL_SYS_NOT_ACCESS,
    UDA_TOOL_CMD_UNSUPPORT_ON_PRODUCT_ERR,
    UDA_TOOL_CMD_UNSUPPORT_ON_MEDIA_ERR,
    UDA_TOOL_INSUFFICIENT_FILE_SIZE_ERR,
    UDA_TOOL_PARSE_CONF_FILE_ERR,
    UDA_TOOL_CDR_IS_UPDATING_ERR,
    UDA_TOOL_CDR_UPDATE_FAIL_ERR,
    UDA_TOOL_CDR_NOT_TRIGGERED_ERR,
    UDA_TOOL_ROUTE_ALREADY_EXIST_ERR,
    UDA_TOOL_ROUTE_NOT_EXIST_ERR,
    UDA_TOOL_USER_ABORT_CMD_ERR,
    UDA_TOOL_OP_NOT_SUPPORT_IPV6_ERR,
    UDA_TOOL_OP_NOT_SUPPORT_BOND_ERR,
    UDA_TOOL_OP_PFC_NUMS_OUT_OF_VALUE,
    UDA_TOOL_GATEWAY_NO_IP_CONF_ERR,
    UDA_TOOL_MTU_TOO_SMALL_FOR_IPV6_ERR,
    UDA_TOOL_IP_ROUTE_TABLE_ALREADY_EXIST_ERR,
    UDA_TOOL_ACCESS_CONF_FILE_ERR,
    UDA_TOOL_CONF_FILE_NOT_EXIST_ERR,
    UDA_TOOL_HCCS_PING_RESOURCE_BUSY,
    UDA_TOOL_CONF_NOT_EXIST_ERR,
    UDA_TOOL_CONF_ALREADY_EXIST_ERR,
    UDA_TOOL_IP_MISMATCH_MASK_ERR,
    UDA_TOOL_COFNIG_DEFAULT_ROUTE_ERR,
    UDA_TOOL_ROUTE_TABLE_NOT_EXIST_ERR,
    UDA_TOOL_COFNIG_BROAD_ADDRESS_ERR,

    UDA_DSMI_ERR = 0x4000,
    UDA_DSMI_EXE_ERR,
    UDA_DSMI_GATEWAY_NOT_PRESET_ERR,
    UDA_DSMI_DEV_GET_HEALTH_TMOUT_ERR,
    UDA_DSMI_DEV_NOT_EXIST_ERR,
    UDA_DSMI_TLS_CER_ILLEGAL_ERR,
    UDA_DSMI_TLS_CER_VERIFY_ERR,
    UDA_DSMI_ROUTE_ROW_REACH_MAX_ERR,
    UDA_DSMI_CMD_UPSUPPORT_ON_OPTICAL_ERR,
    UDA_DSMI_CONFIG_NUM_EXCEED_LIMIT_ERR,
    UDA_DSMI_UPDATE_QP_UDP_SPORT_ERR,
    UDA_DSMI_SET_UDP_PORT_IPPAI_ERR,
    UDA_DSMI_GET_I2C_CTRL_ERR,
    UDA_DSMI_IPCONF_NOT_PRESET_ERR,
    UDA_DSMI_LOOPBACK_FAIL_FOR_DOWNGRADE_ERR,
    UDA_DSMI_CONTROL_LINK_UNREACHABLE_ERR,
    UDA_DSMI_CDR_NOT_SUPPORT_ERROR,
    UDA_DSMI_PAGE_INDEX_NOTSUPP,
    UDA_DSMI_OPTICAL_NOT_INIT_ERR,
    UDA_DSMI_XSFP_FAIL_FOR_DOWNGRADE_ERR,
    UDA_DSMI_XSFP_ABSENT,
    UDA_DSMI_NOT_SUPPORT_VM_ERR,
    UDA_DSMI_LINK_IS_DOWN_ERR,
    UDA_DSMI_TLS_CERT_EXPIRED_ERR,
    UDA_DSMI_DIAG_UNENABLED,
    UDA_DSMI_GET_LOCK_FAILED,
    UDA_TOOL_CHECK_PRIO_TC_CONFIG,
    UDA_TOOL_CHECK_TC_QOS_CONFIG,

    UDA_EXE_FAILED = 0x5000,
    UDA_EXE_TIME_OUT_ERR,
};

typedef enum {
    UDA_HELP_CMD = 0x0,
    UDA_NOT_HELP_CMD,
    UDA_TARGET_NOT_FOUND,
    UDA_MAX_ASSIST_CMD
} hccn_assist_cmd;

typedef enum {
    UDA_DEV_CMD = 0x0,
    UDA_GLOBAL_CMD = 0x1,
    UDA_MAX_CMD
} hccn_cmd_attr;

typedef enum {
    IPV4_PROTOCOL = 0x0,
    IPV6_PROTOCOL = 0x1
} hccn_cmd_ip;

typedef struct rx_private_wl {
    unsigned short high;
    unsigned short low;
} rx_private_wl;

#define uda_printf(x, args...) \
    (void) printf(x, ##args)
#define uda_mkdir(path) \
    mkdir(path, S_IRWXU)

#define  PWD_STR  "./"
#define  DIR_BREAK_CHAR  '/'
#define  DIR_BREAK_STRING  "/"
#define U32_MASK ((unsigned int) (~((unsigned int) 0)))   /* 0xFFFFFFFF */
#define NIC_TOOL_MAGIC  'x'

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#define LINUX_DEFAULT_ID  0x1
#define NICTOOL_CMD_TYPE  0x18
#define DEVICE_ONE_STRING  1
#define DEVICE_NAME_OFFSET 3
#ifndef IFNAMSIZ
#define IFNAMSIZ    16
#endif
#define NIC_DCB_UP_MAX       0x8
#define MAX_NUM_OF_PATH_ULOG 128
#define NIC_RSS_INDIR_SIZE   256
#define NIC_RSS_KEY_SIZE     40

#define ONE_VALUE       1
#define TWO_VALUE       2
#define THREE_VALUE     3
#define FIVE_VALUE      5
#define FOUR_VALUE      4
#define SIX_VALUE       6
#define SEVEN_VALUE     7
#define NINE_VALUE      9
#define TEN_VALUE       10
#define THIRTEEN        13

#define ARGC_ID_1   1
#define ARGC_ID_2   2
#define ARGC_ID_3   3
#define ARGC_ID_4   4
#define ARGC_ID_5   5
#define ARGC_ID_6   6
#define ARGC_ID_7   7
#define ARGC_ID_8   8
#define ARGC_ID_9   9
#define ARGC_ID_10  10

#define ARGV_NUM_2  2
#define ARGV_NUM_4  4
#define ARGV_NUM_22  22
#define DCQCN_ARGV_PARAM_NUM 24
#define CHAR_ARGV_MAX 7
#define MAX_DCQCN_PARA_NUM 11
#define MAX_UDP_PORT_LIST_LEN  200
#define MAX_PORT_LEN       10
#define MAX_UDP_MODE_LEN   20
#define COMMA_FORMAT       0
#define HYPHEN_FORMAT      1

#define IP_SPLIT_BIT_0 0
#define IP_SPLIT_BIT_8 8
#define IP_SPLIT_BIT_16 16
#define IP_SPLIT_BIT_24 24
#define DEV_LIST_MAX    16

#define SPLIT_UIP(ip, number) (((ip) >> (number)) & 0xff)

#ifdef CONFIG_LLT
#define TOOL_PRINT_INFO(fmt, args...)  \
        do { \
            fprintf(stdout, fmt "\n", ##args); \
        } while (0)

#define TOOL_PRINT_ERR(fmt, args...)  \
        do { \
            printf_stub(stderr, fmt "\n", ##args); \
        } while (0)

#else
#define TOOL_PRINT_INFO(fmt, args...)  \
    do { \
        fprintf(stdout, fmt "\n", ##args); \
        DRV_INFO(HAL_MODULE_TYPE_NET, fmt, ##args); \
    } while (0)

#define TOOL_PRINT_ERR(fmt, args...)  \
    do { \
        fprintf(stderr, fmt "\n", ##args); \
        DRV_ERR(HAL_MODULE_TYPE_NET, fmt, ##args); \
    } while (0)
#endif

#define MAX_INFO_LEN 128

typedef enum {
    UDA_OPT_EMPTY_CMD = 0x0,
    UDA_OPT_BOND_CMD = 0x1,
    UDA_OPT_PORT_CMD = 0x2, // 预埋
    UDA_OPT_MAX_CMD
} hccn_cmd_mode;

struct tool_param {
    int logic_id;
    int cmd;
    int argc;
    char **argv;
    int cmd_major_offset;
    int cmd_index;
    int phy_id;
    bool host_flag;
    bool inet_protocol_type;
    bool get_cmd_flag; // 表示是否是get类命令行，当前用于执行成功时，判断是否打印回显
    hccn_cmd_attr cmd_attr;
    hccn_cmd_mode option_mode;
    union {
        struct {
            unsigned char is_set;
            unsigned char type;
            unsigned char is_enable;
        } chs_param;
        unsigned char info[MAX_INFO_LEN];
    };
    unsigned int mainboard_id;
};

struct dev_list_info {
    int dev_num;
    int logic_list[DEV_LIST_MAX];
    int phy_list[DEV_LIST_MAX];
};

typedef enum {
    UDA_PRINT_LEVEL_INFO = 0,
    UDA_PRINT_LEVEL_ERR,
} hccn_err_level;

struct hccn_tool_err_info {
    int err_code;
    hccn_err_level err_level;
    void (*print_private_err_str)(struct tool_param *param);
    const char *err_str;
};

extern int create_file(void);
extern void io_ctl(int fd, void *inBuf);
extern void close_file(int fd);
#endif
