/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCN_TOOL_H
#define HCCN_TOOL_H

#include "ossl_user_linux.h"
#include "ds_net.h"

#define DRIVER_VERSION_INFO     "1.0.4.0"
#define FIRMWARE_VERSION_INFO_LENGTH     16

#define BOARDID_AI_SERVER_MODULE  0x0
#define BOARDID_ARM_SERVER_AG     0x20

#define NUMBER_BASE               10
#define NUMBER_HEX                16

/* pcie card boardid rule: GPIO[75:73]=0x000 */
#define BOARDID_PCIE_CARD_MASK        0xE00
#define BOARDID_PCIE_CARD_MASK_VALUE  0x0

#define TOOL_ARGC_MIN_LEN 3
#define TOOL_DEVICE_CHIP_ID_MAX  4
#define TOOL_HOST_LOGIC_ID_MAX    16
#define TOOL_MIN_MTU_SIZE       68
#define TOOL_MIN_MTU_SIZE_IPV6      1280
#define TOOL_ETH_MAX_MTU_SIZE       9702
#define TOOL_ROH_MAX_MTU_SIZE       4192
#define TOOL_MIN_TM_SHAPING_BW_LIMIT 10000
#define TOOL_MAX_TM_SHAPING_BW_LIMIT 200000
#define TOOL_MAC_STR_LEN        17
#define TOOL_MAC_FILTER_PRE_UPPER  "01:80:C2"
#define TOOL_MAC_FILTER_PRE_LOWER  "01:80:c2"
#define TOOL_MAC_FILTER_SIZE    18
#define TOOL_DEFAULT_ROUTE_ADDRESS "0.0.0.0"
#define TOOL_DEFAULT_ROUTE_NETMASK "0.0.0.0"
#define TOOL_MAX_ROUTE_ROWS     5000

#define TOOL_NUM_TEN            10
#define TOOL_NUM_SIXTEEN        16
#define DEV_NO_EXIST 0xFFFFFFFF

#define TOOL_XSFP_PRESENT               0x1
#define TOOL_XSFP_HIGH_POWER_ENABLED    0x5
#define TOOL_PARA_BASE          10
#define TOOL_MAX_QPCINDEX       0x100000
#define TOOL_PARA_BITSWIDTH     8
#define TOOL_INDEX_TWO         2
#define TOOL_INDEX_THREE       3
#define TOOL_INDEX_FOUR        4
#define TOOL_INDEX_SIX         6

#define TOOL_SNR_SUPPORT_BIT_HOST           4
#define TOOL_SNR_SUPPORT_BIT_MEDIA          5
#define TOOL_SNR_SUPPORT_BIT_REAR_OFFSET    4
#define TOOL_SNR_SUPPORT_BIT_FRONT_OFFSET   2
#define XSFP_SNR_SUPPORT_INVALID            0xc0

#define TOOL_TLS_CMD_MAX_LEN 37
#define TOOL_TLS_CMD_MIN_LEN 4            /* update at least the private key and device certificate or just crl */
#define TOOL_TLS_SET_CMD_MIN_LEN 2
#define TOOL_TLS_CA_IMPORT_CMD_LEN 4
#define TOOL_TLS_CA_CMD_LEN_ONE 1
#define TOOL_TLS_CA_LOCAL_PATH_LEN 256
#define TOOL_EVEN_CRIT 2
#define TOOL_TLS_ALARM_MAX_LEN   3
#define TOOL_TLS_ALARM_MAX_DAYS  180
#define TOOL_TLS_ALARM_MIN_DAYS  7
#define TOOL_TLS_ALARM_DISABLE   0

#define TLS_CRL_FLAG 1
#define TLS_CA_FLAG 2
#define TLS_HOST_FLAG 3

#define TLS_HOST_ARGC_NUM 2

#define MAX_FRONT_NPU_NUM 8
#define EVEN_NUM          2

#define MAX_HEALTH_CHECK_CNT (5 * 60)

#define ERROR_CODE_MAX_NUM 128
#define BUFF_SIZE 256

#define INVALID_GW      0xFF
#define INVALID_PORT    0xFF

#define IP_BROADCAST    0xFFFFFFFF

#define DSMI_ERR_NOT_SUPPORT            0xfffe

#define SERDES_RESERVED_LEN 64
#define SERDES_INFO_NUM 8

#define TOOL_XSFP_PHYSICAL_CODE_LEN      8
#define TOOL_XSFP_FIBER_FACE_TYPE_LEN    3
#define XSFP_FIBER_FACE_TYPE_APC        "APC"
#define XSFP_FIBER_FACE_TYPE_UPC        "UPC"

struct xsfp_physical_code_map {
    char physical_code[TOOL_XSFP_PHYSICAL_CODE_LEN + 1];
    char face_type[TOOL_XSFP_FIBER_FACE_TYPE_LEN + 1];
};

#define NUMBER_1    1
#define NUMBER_2    2
#define NUMBER_3    3
#define NUMBER_4    4
#define NUMBER_5    5
#define NUMBER_6    6
#define NUMBER_7    7

struct tool_serdes_quality_base {
    unsigned int snr;
    unsigned int heh;
    signed int bottom;
    signed int top;
    signed int left;
    signed int right;
};

typedef enum optical_type {
    PIF_ATTR_OPTICAL_SM     = 0x1,      /* lr */
    PIF_ATTR_OPTICAL_MM     = 0x2,      /* sr */
    PIF_ATTR_ELECTRIC       = 0x3,      /* electric */
    PIF_ATTR_COPPER         = 0x4,      /* copper */
    PIF_ATTR_AOC            = 0x5,      /* aoc */
    PIF_BACKBOARD_INTERFACE = 0x6,      /* MEZZ card */
    PIF_ATTR_BASET          = 0x7,      /* baset */
    PIF_ATTR_UNKNOWN        = 0xffff
} optical_attr_e;

#define IS_COPPER   1

typedef struct tool_serdes_quality_info {
    unsigned int macro_id;
    unsigned int reserved1;     /* reserve for byte alignment */
    struct tool_serdes_quality_base serdes_quality_info[SERDES_INFO_NUM];
    unsigned int reserved2[SERDES_RESERVED_LEN];
} tool_serdes_quality_info;

// The unit of optical voltage reg value is 100uV
#define OPTICAL_VCC_DEVISOR     10
// The unit of optical power reg value is 0.1uW
#define OPTICAL_POWER_DEVISOR   10000
// The unit of optical TX/RX bias reg value is 2uA
#define OPTICAL_BIAS_DEVISOR    500

#define GENERIC_SPEED_100G 100000
#define GENERIC_SPEED_200G 200000
#define INVALID_LOGIC_ID   (-1)

#define LANE_OFFSET                 8

enum board_type {
    EVB_BOARD_TYPE  = 0,
    PCIe_BOARD_TYPE = 1,
    PoD_BOARD_TYPE  = 2,
    A_K_BOARD_TYPE  = 3,
    PoD_BUSINESS_BOARD_TYPE = 4,
    A_X_BOARD_TYPE  = 5,
    SUPERPOD_900_BOARD_TYPE = 6,
    UNKNOWN_BOARD_TYPE = 0xff
};

enum rocee_regs_index {
    /* ERR regs */
    ROCEE_RAS_INT_CFG2_LABEL      = 1,
    ROCEE_RAS_INT_SRC1_LABEL      = 2,
    ROCEE_RAS_INT_SRC2_LABEL      = 3,
    SCC_INT_SRC_LABEL             = 4,
    ROCEE_TDP_STA_LABEL           = 5,
    ROCEE_TDP_ALM_LABEL           = 6,
    ROCEE_TWP_STA_LABEL           = 7,
    ROCEE_TWP_ALM_LABEL           = 8,
    ROCEE_TGP_STA_LABEL           = 9,
    ROCEE_TGP_ALM_LABEL           = 10,
    ROCEE_TMP_STA_LABEL           = 11,
    ROCEE_TMP_ALM_LABEL           = 12,
    ROCEE_TPP_STA_LABEL           = 13,
    ROCEE_TPP_ALM_LABEL           = 14,
    ROCEE_SSU_TC_XOFF_LABEL       = 15,
    ROCEE_TPP_TC_XOFF_LABEL       = 16,

    /* TRP regs */
    ROCEE_TRP_EMPTY_0_LABEL       = 101,
    ROCEE_TRP_EMPTY_1_LABEL       = 102,
    ROCEE_TRP_EMPTY_2_LABEL       = 103,
    ROCEE_TRP_EMPTY_3_LABEL       = 104,
    ROCEE_TRP_EMPTY_4_LABEL       = 105,
    ROCEE_TRP_FULL_0_LABEL        = 106,
    ROCEE_TRP_FULL_1_LABEL        = 107,
    ROCEE_TRP_FULL_2_LABEL        = 108,
    ROCEE_TRP_FULL_3_LABEL        = 109,
    ROCEE_TRP_FSM_LABEL           = 110,
    TRP_RX_CNP_CNT_LABEL          = 111,
    TRP_SCC_CNP_CNT_LABEL         = 112,
    TRP_INNER_STA_0_LABEL         = 113,
    TRP_INNER_STA_1_LABEL         = 114,
    TRP_INNER_STA_2_LABEL         = 115,
    TRP_INNER_STA_3_LABEL         = 116,
    TRP_INNER_STA_4_LABEL         = 117,
    TRP_INNER_STA_5_LABEL         = 118,
    TRP_INNER_STA_6_LABEL         = 119,
    TRP_INNER_STA_7_LABEL         = 120,
    TRP_INNER_STA_8_LABEL         = 121,
    TRP_RX_CQE_CNT_0_LABEL        = 122,
    TRP_RX_CQE_CNT_1_LABEL        = 123,
    TRP_RX_CQE_CNT_2_LABEL        = 124,
    TRP_RX_CQE_CNT_3_LABEL        = 125,

    /* TSP regs */
    ROCEE_TPP_STA1_LABEL          = 201,
    ROCEE_TPP_STA2_LABEL          = 202,
    ROCEE_TPP_STA3_LABEL          = 203,
    ROCEE_TPP_STA4_LABEL          = 204,
    ROCEE_TPP_STA5_LABEL          = 205,
    ROCEE_TPP_STA6_LABEL          = 206,
    ROCEE_TPP_STA7_LABEL          = 207,
    ROCEE_TWP_STA1_LABEL          = 208,
    ROCEE_TDP_STA1_LABEL          = 209,
    ROCEE_TPP_STA_RSV0_LABEL      = 210,
    ROCEE_TPP_STA_RSV1_LABEL      = 211,
    ROCEE_TPP_STA_RSV2_LABEL      = 212,
    ROCEE_TPP_STA_RSV3_LABEL      = 213,
    TWP_TC_HDR_XOFF_LABEL         = 214,
    TWP_TC_ATM_XOFF_LABEL         = 215,
    TSP_SGE_ERR_DROP_LEN_LABEL    = 216,
    TSP_SGE_AXI_CNT_LABEL         = 217,
    ROCEE_TDP_TRP_CNT_LABEL       = 218,
    ROCEE_TDP_MDB_CNT_LABEL       = 219,
    ROCEE_TDP_EXT_DEQ_CNT_LABEL   = 220,
    ROCEE_TDP_LP_CNT_LABEL        = 221,
    ROCEE_TDP_QMM_CNT_LABEL       = 222,
    ROCEE_TDP_EXT_ENQ_CNT_LABEL   = 223,
    ROCEE_TDP_TWP_CNT0_LABEL      = 224,
    ROCEE_TDP_TWP_CNT1_LABEL      = 225,
    ROCEE_TDP_TWP_CNT2_LABEL      = 226,
    ROCEE_TDP_SCC_CNT_LABEL       = 227,
    ROCEE_SCC_TDP_CNT_LABEL       = 228,
    ROCEE_TDP_TM_CNT_LABEL        = 229,
    ROCEE_TM_TDP_CNT_LABEL        = 230,

    /* CNT regs */
    CAEP_TRP_AE_CNT_I_LABEL       = 301,
    CAEP_MDB_AE_CNT_I_LABEL       = 302,
    CAEP_QMM_CE_CNT_I_LABEL       = 303,
    CAEP_QMM_CE_VLD_CNT_I_LABEL   = 304,
    CAEP_VFT_ERR_CNT_O_LABEL      = 305,
    CAEP_ACE_DISCARD_CNT_O_LABEL  = 306,
    CAEP_ACE_VLD_CNT_O_LABEL      = 307,
    ROCEE_MBX_ISSUE_CNT_LABEL     = 308,
    ROCEE_MBX_EXEC_CNT_LABEL      = 309,
    ROCEE_DB_ISSUE_CNT_LABEL      = 310,
    ROCEE_DB_EXEC_CNT_LABEL       = 311,
    ROCEE_EQDB_ISSUE_CNT_LABEL    = 312,
    ROCEE_EQDB_EXEC_CNT_LABEL     = 313,
};

#define MAX_REG_NAME_LEN 40

struct cmd_reg_info {
    char reg_name[MAX_REG_NAME_LEN];
    unsigned int reg_offset_label;
};

#define VNIC_MAX_NUM            8
#define VNIC_IP_FIRST           192
#define VNIC_IP_SECOND          168
#define VNIC_IP_THIRD           1
#define VNIC_IP_FOUTH           199
#define RS_DEVICE_NUM           0x3

#define DSCP_STR_LEN    5

struct dscp_tc_data {
    unsigned char dscp;
    unsigned char tc;
};

#define DSCP_AND_TC     2

#define CA_CERT_FLAG_LEN 5  /* eg:caXX, include end flag '\0' */

#define TOOL_TIME_LEN 32
#define MULTI_256   256

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

struct tool_pars_tls_para {
    int argc;
    char **argv;
    unsigned int tls_index;
    unsigned int cfg_file_len;
    unsigned int cert_cnt;
    bool host_flag;
    unsigned int bit_map[MAX_TLS_CFG_COUNT];
    char real_conf_path[PATH_MAX + 1];
    char cfg_file[MAX_TLS_CFG_COUNT + 1][PATH_MAX + 1];
};

struct tool_pars_tls_ca_para {
    int insert_cb;  // 0: 插入到cb3中；1: 插入到cb4中
    int insert_pos;
    int exist_cb;   // 0: 此alias与cb3中的某个证书重名；1：与cb4中的重名
    int exist_pos;
    char alias[TLS_CA_SSL_NEW_CERT_ALIAS_LEN];
    char real_ca_conf_path[PATH_MAX];
};

enum read_route_operation {
    READ_TO_CLEAR = 1,
    READ_TO_RECOVERY,
    READ_TO_DEL_NOT_SAME_SEGMENT,
    READ_TO_DEL_IP_ROUTE_NOT_SAME_SEGMENT,
};

enum {
    TOOL_NET_CFG_NONE = 0,
    TOOL_NET_CFG_START,
    TOOL_NET_CFG_FAIL,
    TOOL_NET_CFG_SUCCESS,
}; // 如新增枚举变量需同步修改 hccn_cdev.h 中的 MAX_NET_CFG_STATUS_SIZE

#define NET_HEALTH_STATUS_LEN   20
struct net_health_status_map {
    int status_code;
    char status_str[NET_HEALTH_STATUS_LEN];
};

struct tool_route_para {
    unsigned int address_ip[FOUR_VALUE];
    unsigned int netmask_ip[FOUR_VALUE];
    unsigned int gateway_ip[FOUR_VALUE];
};

enum device_mac_type {
    MAC_TYPE_ETH = 0,
    MAC_TYPE_ROH = 1,
    INVALID_MAC_TYPE,
};

#define U8_MAX_LEN      3
#define U8_MAX          255
#define DSCP_MAX        63
#define DSCP_TC_CONF    "dscp_tc"

#define TC_MAX    3
#define LLDP_TYPE_INBUFF_LEN 10
#define TIME_CHANGE     1000

struct dcqcn_param_info {
    char *param;
    int (*param_parse_func)(struct ds_dcqcn_info *info, char *val, char *para, unsigned int length);
};

extern int g_recovering;

enum {
    ARG_MUST = 0,   // 必选参数
    ARG_OPT  = 1,   // 可选参数
    ARG_ALRE = 2,   // 参数已选，参数已被录入后，标记会被更新为这个，以防重复输入
};

// 用于解析数字型参数
struct arg_parse_t {
    char *name;     // 参数名
    int mode;       // ARG_MUST/ARG_OPT/ARG_ALRE
    int *dst;       // 参数值的存储位置
    int min_value;
    int max_value;
};

int tool_ip_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_gateway_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_netdetect_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_lldp_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_local_lldp_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_mac_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_cfg_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_link_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_mtu_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_net_health_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_dscp_cmd_execute(int argc, char **argv, struct tool_param *param, unsigned int port_id);

int tool_arp_table_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_route_table_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_pci_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_roce_context_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_reg_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_tls_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_tls_ca_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_optical_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_stat_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_shaping_cmd_execute(int argc, char **argv, struct tool_param *param, unsigned int port_id);

int tool_check_dev_health(int logic_id);

int tool_get_dev_list_info(struct dev_list_info *info);

int tool_port_shaping_set(int logic_id, int argc, unsigned int port_id, char **argv);

int tool_version_cmd_execute(int argc, char **argv, struct tool_param *param);
int tool_read_hccn_conf_handle_action(struct tool_param *param, enum read_route_operation route_op,
    struct ds_route_table_value ip_param);

int tool_vnic_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_process_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_roce_port_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_cfg_arp_recovery(struct tool_param *param);
int tool_cfg_status_set(struct tool_param *param, unsigned int status);
int tool_cfg_status_get(int argc, char **argv, struct tool_param *param);
int tool_bandwidth_cmd_execute(int argc, char **argv, struct tool_param *param);

int tool_netdev_status_cmd_execute(int argc, char **argv, struct tool_param *param);
int tool_generic_cmd_execute(int argc, char **argv, struct tool_param *param);
int tool_set_static_speed(int argc, char **argv, struct tool_param *param);
int tool_link_stat_cmd_execute(int argc, char **argv, struct tool_param *param);
int tool_eth_test_cmd_execute(int argc, char **argv, struct tool_param *param);
int tool_hw_stats_cmd_execute(int argc, char **argv, struct tool_param *param);
int tool_udhcpc_cmd_execute(int argc, char **argv, struct tool_param *param);
int tool_double_check(char *info);
int tool_check_single_route_cmd_argv(char **argv, char **ip, const char prefix[], char *route_char,
    unsigned int route_char_length);
int tool_dcqcn_cmd_execute(int argc, char **argv, struct tool_param *param);
int tool_set_dcqcn_alg_info(int argc, char **argv, struct tool_param *param, struct ds_dcqcn_info *info);
int tool_set_dcqcn_enable(int argc, char **argv, struct tool_param *param, struct ds_dcqcn_info *info);
int tool_set_dcqcn_cnp_dscp(int argc, char **argv, struct tool_param *param, struct ds_dcqcn_info *info);
int tool_cfg_dcqcn_alg_recovery(struct tool_param *param);
int tool_cfg_dcqcn_enable_recovery(struct tool_param *param);
int tool_cfg_dcqcn_cnp_dscp_recovery(struct tool_param *param);
int tool_get_board_type(struct tool_param *param, int *board_type);
int tool_local_lldp_cmd_execute(int argc, char **argv, struct tool_param *param);
int tool_cfg_lldp_portid_recovery(struct tool_param *param);
int ATLAS_900_A3_SUPERPOD_logic_id_convert(int phy_id);
bool check_udhcpc_write_hccn_conf(struct tool_param *param, int is_ipv6);
int atlas_900_a3_superpod_phy_id_convert(int phy_id);
int atlas_900_a3_superpod_get_cooperative_phy_id(int phy_id);
int is_copper_cable(int logic_id);
void tool_show_additional_dr4_optical_snr_info(struct ds_optical_info *info, const int offset,
    bool host_support, bool media_support);
void tool_show_optical_info_v2(struct ds_optical_info *info, const int sflag);
int tool_get_optical_info_handle(int argc, char **argv, struct tool_param *param);
int tool_get_xsfp_id_map_index(struct ds_optical_info *info, unsigned int *id_map_idx);
void tool_show_optical_info(struct ds_optical_info *info, unsigned int id_map_idx, const int sflag);
bool is_network_segment(unsigned int address, unsigned int netmask);
bool is_address_gateway_same_segment(unsigned int address, unsigned int gateway, unsigned int netmask);
#endif
