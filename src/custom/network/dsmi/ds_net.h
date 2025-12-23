/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DS_NET_H
#define DS_NET_H

#include <stdbool.h>
#include <unistd.h>
#include <netinet/in.h>
#include "ascend_hal.h"
#ifndef CONFIG_LLT
#endif
#include "securec.h"
#include "dsmi_net_inter.h"
#include "tls.h"
#include "xsfp_comm.h"

int dsmi_cmd_get_network_device_info(int device_id, const char *inbuf,
                                     unsigned int size_in, char *outbuf,
                                     unsigned int *size_out);

#define MAX_CMD_SEND_LEN 2048

#define INFO_PAYLOAD_LEN 1000
#define MAX_CMD_PAYLOAD_LEN 2000
#define MAX_ROUTE_TABLE_LEN 386000
#define MAX_TRANS_DATA_LEN 386000
#define MAX_NETDEV_STATUS_LEN 5000
#define ETH_REG_DUMP_LEN    6144

#define MAX_IP_LEN 20
#define MAX_IPV6_LEN 48
#define MAX_IPV6_NUM 16
#define DS_MAC_ADDR_LEN 6
#define DS_MIN_LOGIC_ID 0
#define DS_MAX_LOGIC_ID 15
#define MAX_PORT_ID 7

#define DS_MAC_STR_LEN        17
#define DS_MAC_ADDR_LEN       6
#define DS_MAC_FILTER_PRE_UPPER  "01:80:C2"
#define DS_MAC_FILTER_PRE_LOWER  "01:80:c2"
#define DS_MAC_FILTER_SIZE    18

#define DSCP_MAX        63
#define TC_MAX    3

#define DSMI_MIN_MTU_SIZE 68
#define DSMI_MAX_MTU_SIZE 9702
#define DS_MTU_LEN 5
#define DS_PORT_LEN 7
#define DSMI_MIN_UDP_PORT 4097
#define DSMI_MIN_UDP_PORT_LEN 4
#define DSMI_MAX_UDP_PORT 65535
#define DSMI_MAX_UDP_PORT_LEN 5
#define DSMI_UDP_RANGE 0xFFFF

#define DSMI_MIN_TM_SHAPING_BW_LIMIT 10000
#define DSMI_MAX_TM_SHAPING_BW_LIMIT 100000

#define DSMI_TLS_ALARM_MAX_DAYS  180
#define DSMI_TLS_ALARM_MIN_DAYS  7
#define DSMI_TLS_ALARM_DISABLE 0

#define NAME_LEN             17
#define OUI_LEN              3
#define PN_LEN               17
#define WAVE_LEN             2
#define SN_LEN               17
#define DATACODE_LEN         11
#define IDENTIFIER_LEN       20

#define TOOL_MAC_ADDR_LEN       6

#define AUTO_ADAPT_INBUFF_LEN       4
#define GENERIC_INFO_INBUFF_LEN     20
#define GENERIC_INFO_OUTBUFF_LEN    1000
#define GENERIC_INFO_SPEED_UNKNOWN  0
#define GENERIC_INFO_PARM_NUM       1
#define GENERIC_INFO_SPEED_100G     100000
#define GENERIC_INFO_SPEED_200G     200000
#define GENERIC_INFO_SPEED_400G     400000

#define ETH_SELF_TEST_OUTBUFF_LEN   500
#define ETH_SELF_TEST_MODE_TYPE_MAX 1
#define UDHCPC_QUIT_INBUFF_LEN      4

#define CHIP_DIE_CNT                    2
#define UNSIGNED_INT_RADIX              10
#define ATLAS_9000_A3_MAINBOARD_ID      0x1C
#define ATLAS_9000_A3_MAINBOARD_ID_2    0x1D
#define ATLAS_900_A3_MAINBOARD_ID_1     0x18
#define ATLAS_900_A3_MAINBOARD_ID_2     0x19
#define ATLAS_900_A3_MAINBOARD_A_K      0x14
#define ATLAS_900_A3_MAINBOARD_A_X      0x15

#define ENABLE    1
#define DISABLE   0

#define DS_CLEAR_PFC_MODE_MAX          2

#define HCCN_TOOL_GET_LOCK_FAIL        0x100

struct ipv_addr {
    int ip_flag;
    unsigned int ipv4;
    unsigned char ipv6[MAX_IPV6_NUM];
};
struct generic_info {
    unsigned int speed;
};

struct generic_info_parse_param {
    char *param;
    int (*parse_proc)(struct generic_info *generic_info, int *cur, int argc, char **argv);
};


#define DEFAULT_UDHCPC_TIMEOUT  3
#define UDHCPC_OUTBUFF_LEN      400
#define UDHCPC_RUN_INFO_LEN     300
#define UDHCPC_MAX_TIMEOUT      300
#define UDHCPC_MIN_TIMEOUT      1
#define NORMAL_BUF_SIZE         100
 
struct udhcpc_param {
    unsigned int timeout;
    int recovery_flag;
    bool retain_process;
    bool is_ipv6;
};
 
struct udhcpc_parse_param {
    char *param;
    int (*parse_proc)(struct udhcpc_param *param, int *cur, int argc, char **argv);
};

enum ds_net_context_type {
    DS_TYPE_QPC = 0,
    DS_TYPE_AEQC,
    DS_TYPE_CEQC,
    DS_TYPE_CQC,
    DS_TYPE_MPT,
    DS_TYPE_MAX,
};

enum ds_net_opcode_type {
    DS_MIN_CMD = 0x0,
    DS_SET_NET_DETECT   = 0x0001,
    DS_GET_NET_LLDP_NEIGHBOR_INFO   = 0x0002,
    DS_SET_DEFAULT_GATEWAY  = 0x0003,
    DS_DEL_DEFAULT_GATEWAY  = 0x0004,
    DS_GET_DEFAULT_GATEWAY  = 0x0005,
    DS_GET_MAC_ADDRESS  = 0x0006,
    DS_SET_NETDEV_STATUS  = 0x0007,
    DS_INSPECT_ETH_STATUS = 0x0008,
    DS_SET_MTU = 0x0009,
    DS_GET_MTU = 0x000A,
    DS_SET_MAC_ADDRESS = 0x000B,
    DS_SET_DSCP_MAP = 0x000C,
    DS_GET_DSCP_MAP = 0x000D,
    DS_ARP_TABLE_CFG = 0x000E,
    DS_GET_ROUTE_TABLE = 0x000F,
    DS_GET_PCI_INFO = 0x0010,
    DS_GET_ROCE_CONTEXT = 0x0011,
    DS_GET_ETH_REG_INFO = 0x0012,
    DS_GET_OPTICAL_INFO = 0X0013,
    DS_GET_PACKET_STATISTICS = 0x0014,
    DS_SET_TM_SHAPING_PORT = 0x0015,
    DS_GET_TM_SHAPING_PORT = 0x0016,
    DS_SET_TLS_CFG = 0x0017,
    DS_GET_TLS_CFG = 0x0018,
    DS_SET_TLS_ENABLE = 0x0019,
    DS_SET_TLS_ALARM = 0x001A,
    DS_GET_ROCEE_REG_INFO = 0x001B,
    DS_GET_FIRMWARE_VERSION = 0x001C,
    DS_ADD_ROUTE_TABLE = 0x001D,
    DS_DELETE_ROUTE_TABLE = 0x001E,
    DS_GET_VNIC_STATUS = 0x001F,
    DS_GET_DEVICE_PROCESS = 0x0020,
    DS_CLEAR_PACKET_STATISTICS = 0x0021,
    DS_GET_NET_DETECT = 0x0022,
    DS_GET_NETWORK_REGISTER = 0x0023,
    DS_CLEAR_TLS_CFG = 0x0024,
    DS_GET_ENVELOPE_PUB = 0x0025,
    DS_SET_ROCE_PORT = 0x0026,
    DS_GET_ROCE_PORT = 0x0027,
    DS_SET_CFG_STATUS = 0x0028,
    DS_GET_CFG_STATUS = 0x0029,
    DS_GET_NETDEV_STATUS = 0x002A,
    DS_SET_GENERIC_INFO  = 0x002B,
    DS_GET_LINK_HIS_DATA = 0x002C,
    DS_CLEAR_LINK_HIS_DATA = 0x002D,
    DS_GET_ROCE_DFX_LEN = 0x002E,
    DS_GET_ROCE_DFX_DATA = 0x002F,
    DS_SET_OPTICAL_AUTO_ADAPT = 0x0030,
    DS_GET_ETH_TEST_INFO = 0x0031,
    DS_GET_RDMA_HW_STATS_DATA = 0x0032,
    DS_ACQUIRE_DHCP_IP = 0x0033,
    DS_RELEASE_DHCP_IP = 0x0034,
    DS_GET_DCQCN_INFO = 0x0035,
    DS_SET_DCQCN_INFO = 0x0036,
    DS_SET_ARP_TABLE_CFG = 0x0037,
    DS_GET_BANDWIDTH = 0x0038,
    DS_SET_OPTICAL_LOOPBACK = 0x0039,
    DS_GET_LINK_CNT = 0x003A,
    DS_GET_MAC_TYPE = 0x003B,
    DS_GET_NET_LLDP_LOCAL_INFO = 0x003C,
    DS_SET_LLDP_PORT_TYPE = 0x003D,
    DS_GET_TLS_CA_CFG = 0x003E,
    DS_SET_TLS_CA_CFG = 0x003F,
    DS_CLEAR_TLS_CA_CFG = 0x0040,
    DS_MAX_CMD,
};

enum ds_cmd_opetation {
    DS_READ = 0,
    DS_WRITE = 1,
    DS_NONE = 2,
};

struct context_cmd_info {
    enum ds_net_context_type type;
    const char  *cmd;
};

struct ds_cmd_detect {
    unsigned int ip_address;
};

struct ds_cmd_shaping_para {
    unsigned int port_id;
    int bw_limit;
};

struct ds_cmd_tos {
    unsigned int dev_id;
    unsigned int port_id;
    unsigned char dscp;
    unsigned char tc;
};

struct ds_cmd_context {
    enum ds_net_context_type type;
    unsigned int index;
};

struct ds_gateway_addr {
    unsigned int address;
    int port;
    unsigned int default_phy_id;
    bool is_gateway_on_eth;
};

struct ds_shaping_info {
    int IR_B;
    int IR_U;
    int IR_S;
    int BS_B;
    int BS_S;
    int bw_limit;
    int bw_max_cap;
};

struct ds_rocee_reg_info {
    unsigned int rocee_twp_alm;
    unsigned int rocee_tpp_alm;
    unsigned int rocee_trp_err_flg_0;
    unsigned int rocee_trp_err_flg_1;
};

struct ds_addr_info {
    unsigned long address_host;
    unsigned long netmask_host;
    unsigned long address_device;
    unsigned long netmask_device;
};

enum ds_data_frag {
    DS_ONE_PKT,
    DS_FIRST_PKT,
    DS_MIDDLE_PKT,
    DS_LAST_PKT,
};

enum ds_snd_rcv_op {
    DS_SEND_OP,
    DS_RECV_OP,
};

struct ds_common_req_head {
    int logic_id;
    int port_id;
    enum ds_net_opcode_type cmd;
    enum ds_data_frag data_frag;
    enum ds_snd_rcv_op snd_rcv_op;
    unsigned int send_data_len;
    unsigned int recv_data_len;
    unsigned int one_piece_len;
    pid_t host_tid;
    int rsvd;
    pid_t pid;
};

struct ds_common_req_param {
    struct ds_common_req_head req_head;
    char info[MAX_CMD_PAYLOAD_LEN];
};

struct ds_common_rsp_head {
    int logic_id;
    int result;
    enum ds_net_opcode_type cmd;
    enum ds_data_frag data_frag;
    unsigned int one_piece_len;
    pid_t pid;
    int rsvd;
};

struct ds_common_rsp_param {
    struct ds_common_rsp_head rsp_head;
    char info[MAX_CMD_PAYLOAD_LEN];
};

struct ds_trans_data {
    enum ds_net_opcode_type cmd;
    const char *inbuf;
    unsigned int size_in;
    char *outbuf;
    unsigned int *size_out;
    int result;
    /* when size_out>2000 set pid = getpid() */
    pid_t pid;
};

struct ds_arp_table_value {
    unsigned int eth_id;
    unsigned int ip_address;
    unsigned char mac_address[TOOL_MAC_ADDR_LEN];
    char cmd_type;             /* g-get s-add d-del */
};

struct ds_neigh_table_value {
    unsigned int eth_id;
    struct in6_addr ip6_address;
    unsigned char mac_address[TOOL_MAC_ADDR_LEN];
    char cmd_type;             /* g-get s-add d-del */
};

struct ds_route_table_value {
    unsigned int address;
    unsigned int netmask;
    unsigned int gateway;
    bool is_route_on_eth;
};

struct ds_route_table_character {
    char address[MAX_IP_LEN];
    char netmask[MAX_IP_LEN];
    char gateway[MAX_IP_LEN];
};

struct ds_optical_info {
    int temp;
    unsigned int present;
    unsigned char dr4_flag;
    unsigned int high_power_enable;
    struct xsfp_base_info base_info;
    struct xsfp_additional_info additional_info;
};

#define DS_MAX_USER_TOS    64

struct dsmi_shaping_info {
    int IR_B;
    int IR_U;
    int IR_S;
    int BS_B;
    int BS_S;
    int bw_limit;
    int bw_max_cap;
};

struct dsmi_rocee_reg_info {
    unsigned int rocee_twp_alm;
    unsigned int rocee_tpp_alm;
    unsigned int rocee_trp_err_flg_0;
    unsigned int rocee_trp_err_flg_1;
};

struct dsmi_roce_context_factors {
    enum ds_net_context_type type;
    unsigned int index;
    unsigned int len;
};

struct ds_net_cfg_status {
    unsigned int chip_id;
    unsigned int status;
};

#define LINK_STAT_MAX_IDX 10
struct ds_link_his_stat {
    unsigned long long link_up_cnt;
    unsigned long long link_down_cnt;
    struct {
        unsigned int link_status;
        unsigned long long link_tv_sec;
    } stat[LINK_STAT_MAX_IDX];
    unsigned int stat_cnt;
    unsigned long long cur_tv_sec;
};

struct ds_roce_dfx {
    unsigned int buf_len;
};

struct ds_rdma_hw_stats {
    const char * const *names;
    int num_counters;
    unsigned long long value[];
};

#define MAX_DCQCN_PARAM_LEN 30
#define MAX_PARAM_CNT 24

struct ds_enable_dcqcn_param {
    unsigned char enable;
    unsigned char alg_mode1;
};

struct ds_dcqcn_param {
    unsigned short ai;
    unsigned char f;
    unsigned char tkp;
    unsigned short tmp;
    unsigned short alp;
    unsigned int max_speed;
    unsigned char g;
    unsigned char al;
    unsigned char cnp_time;
    unsigned char alp_shift;
    unsigned char alg_mode;
    unsigned char max_des_shift;
};

struct ds_dcqcn_info {
    int chip_id;
    int flag;
    unsigned char cnp_dscp;
    struct ds_dcqcn_param dcqcn_param;
    struct ds_enable_dcqcn_param enable_param;
};

struct ds_mac_type {
    int chip_id;
    unsigned int mac_type;
};

// 测量带宽的默认时间与最大可用时间
#define DEFAULT_TIME_INTERVAL 1000
#define MIN_TIME_INTERVAL 100
#define MAX_TIME_INTERVAL 10000
// 为了生成两位小数的结果需要引入一个值为100的宏
#define PERCENTAGE_BASE 100

#define DSMI_OPTICAL_LOOPBACK_RUNNING 4

int dsmi_network_transmission_channel(int logic_id, struct ds_trans_data *trans_data);
int dsmi_set_net_detect_ip_address(int logic_id, struct ipv_addr *ip_address);
int dsmi_get_net_detect_ip_address(int logic_id, int port_id, struct ipv_addr *ip);
int dsmi_lldp_get_neighbor_info(int logic_id);
int dsmi_inet_ntop_ipv4_address(const unsigned int *address);
int dsmi_inet_ntop_ip_address(struct ipv_addr *address);
int dsmi_mac_filter_check(const unsigned char *mac_addr);
int dsmi_set_mtu(int logic_id, int port_id, unsigned int mtu);
int dsmi_get_mtu(int logic_id, int port_id, unsigned int *mtu);
int dsmi_set_dscp_map(int logic_id, unsigned int port_id, unsigned char dscp_val, unsigned char tc_val);
int dsmi_get_dscp_map(int logic_id, unsigned int port_id, unsigned char dscp_val, unsigned char *tc_val);
int dsmi_set_port_shaping(int logic_id, unsigned int port_id,  int bw_limit);
int dsmi_get_port_shaping(int logic_id, unsigned int port_id,  struct dsmi_shaping_info *shaping_info);
int dsmi_get_route_table(int logic_id, int port_id, char *route, unsigned int len);
int dsmi_get_pci_info(int logic_id, int port_id, char *pci, unsigned int len);
int dsmi_get_roce_context(int logic_id, int port_id,
    struct dsmi_roce_context_factors roce_context_factors, char *context);
int dsmi_get_eth_reg_info(int logic_id, int port_id, char *dump_info, unsigned int len);
int dsmi_get_optical_info(int logic_id, int port_id, struct ds_optical_info *info);
int dsmi_set_tls_cfg(int logic_id, struct tls_cert_ky_crl_info *tls_cfg);
int dsmi_get_tls_cfg(int logic_id, int port_id, struct tls_cert_show_info show_info[], unsigned int num);
int dsmi_set_tls_enable(int logic_id, struct tls_enable_info *enable_info);
int dsmi_set_tls_alarm(int logic_id, struct tls_alarm_info *alarm_info);
int dsmi_clear_tls_cfg(int logic_id, struct tls_clear_info *clear_info);
int dsmi_get_tls_ca_cfg(int logic_id, int port_id, struct tls_ca_new_certs *ca_cert_info, unsigned int num);
int dsmi_set_tls_ca_cfg(int logic_id, int port_id, struct tls_ca_new_certs *ca_cert_info, unsigned int num);
int dsmi_clear_tls_ca_cfg(int logic_id, unsigned int save_mode);
int dsmi_set_default_gateway_address(int logic_id, int port, struct ds_gateway_addr gateway_addr);
int dsmi_get_default_gateway_address(int logic_id, int *port, unsigned int *gateway,
    unsigned int *default_phy_id, bool is_gateway_on_eth);
int dsmi_get_mac_address(int logic_id, int port, unsigned char *mac_addr);
int dsmi_set_mac_address(int logic_id, int port, const unsigned char *mac_addr);
int dsmi_set_netdev_link(int logic_id, int port, int enable_flag);
int dsmi_get_firmware_version(int logic_id, int port_id, char *version, unsigned int length);
int dsmi_add_route_table(int logic_id, int port_id, struct ds_route_table_value *route_param,
    char *outbuf, unsigned int len);
int dsmi_del_route_table(int logic_id, int port_id, struct ds_route_table_value *route_param,
    char *outbuf, unsigned int len);
int dsmi_get_vnic_status(int logic_id, int port, int *link);
int dsmi_get_device_process(int logic_id, int port_id, int *found, unsigned int length);
int dsmi_get_tls_digital_envelope_pub_ky(int logic_id, struct envelope_pub_info *pub_info);
int dsmi_set_roce_port(int logic_id, int port_id, unsigned int roce_port);
int dsmi_get_roce_port(int logic_id, int port_id, int *roce_port);
int dsmi_set_cfg_status(int logic_id, unsigned int status);
int dsmi_get_cfg_status(int logic_id, unsigned int *status);
int dsmi_get_netdev_status(int logic_id, int port_id, char *route, unsigned int len);
int dsmi_set_generic_info(int logic_id, char *inbuf, unsigned int inbuf_len);
int dsmi_get_link_his_stat(int logic_id, int port, struct ds_link_his_stat *stat);
int dsmi_clear_link_his_stat(int logic_id, int port);
int dsmi_get_roce_dfx_len(int logic_id, int port_id, struct ds_roce_dfx *roce_dfx);
int dsmi_get_roce_dfx_data(int logic_id, int port_id, char *data, unsigned int len);
int dsmi_get_network_register(int logic_id, int port_id, unsigned long long addr, unsigned long *value);
int dsmi_get_eth_test_info(int logic_id, char mode);
int dsmi_get_rdma_hw_stats(int logic_id, int port, struct ds_rdma_hw_stats *stats);
int dsmi_acquire_dhcp_ip(int logic_id, int port, const struct udhcpc_param *udhcpc_param, char *outbuf,
    unsigned int len);
int dsmi_get_dcqcn_info(int logic_id, struct ds_dcqcn_info *info);
int dsmi_set_dcqcn_info(int logic_id, struct ds_dcqcn_info *info);
int dsmi_release_dhcp_ip(int logic_id, int port, const struct udhcpc_param *udhcpc_param, char *outbuf,
    unsigned int len);
int dsmi_set_optical_loopback(int logic_id, int port_id, int mode, int is_write);
int dsmi_get_link_cnt(int logic_id, int port_id, unsigned int *link_cnt);
int dsmi_get_mac_type(int logic_id, int port_id, unsigned int *mac_type);
int dsmi_lldp_get_local_info(int logic_id);
int dsmi_set_lldp_port_id(int logic_id, int port_id_type);
int dsmi_set_tls_machine_type(int logic_id, struct tls_enable_info *enable_info);
int dsmi_analysis_dsmi_ret_to_uda(int ret);

bool is_ATLAS_900_A3_SUPERPOD(unsigned int mainboard_id);
bool is_ATLAS_9000_A3_SUPERPOD(unsigned int mainboard_id);

#define DSMI_SET_TRANS_DATA(_trans_data, _cmd, _inbuf, _size_in, _outbuf, _size_out) do { \
    (_trans_data).cmd = (_cmd); \
    (_trans_data).inbuf = (_inbuf); \
    (_trans_data).size_in = (_size_in); \
    (_trans_data).outbuf = (_outbuf); \
    (_trans_data).size_out = (_size_out); \
    (_trans_data).result = (-1); \
    (_trans_data).pid = (*(_trans_data).size_out > MAX_CMD_PAYLOAD_LEN) ? getpid() : 0; \
} while (0)

#define DSMI_PRINT_INFO(fmt, args...)  \
    do { \
        fprintf(stdout, fmt "\n", ##args); \
    } while (0)

#define DSMI_CHECK_PTR_VALID_RETURN_VAL(p, ret) do { \
    if ((p) == NULL) { \
        roce_err("ptr is NULL!"); \
        return ret; \
    } \
} while (0)

#define MIN(a, b)   (((a) > (b)) ? (b) : (a))

#endif
