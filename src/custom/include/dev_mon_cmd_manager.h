/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DEV_MON_CMD_MANAGER_H__
#define __DEV_MON_CMD_MANAGER_H__

#include "dm_common.h"
#include "device_monitor_type.h"
#include "dsmi_inner_interface.h"
#include "dev_mon_cmd_manager_ext.h"

#define MAX_CHIP_MINI_V2        2
#define TX_BIAS_MULTI_2         2
#define IMU_IPC_COMPUTING_DATA_LEN    19
#define IMU_IPC_COMPUTING_BOM_MAX_LEN 15

#define PACKAGE_INFO_COUNT 3
#define DEV_MON_VRD_TEMP               0xFE
#define DM_REP_CHIP_VRDTEMP_DATA_LEN   4
#define STAT_BUF_LEN 256
#define OUT_BAND_MAC_ADDRESS_LEN 6
#define ROCE_PORT                1
#define DEFAULT_PORT_ID             0
#define TMP_STR_LEN 64
#define ARGUE_NUM_TWO 2
#define STAT_RETURN_VALUE "__returned_value_"
#define BYTES_TO_KB   1024
#define BIT_TO_BYTES  8

#define DMP_MSG_HEAD_LEN 12
#define DMP_SMBUS_MSG_MAX_LEN 32 /* according to SMBUS */

#define DDR_BASE_INFO_LEN 5
#define LINK_STAT_MAX_IDX 10

#define XSFP_WIRE_TYPE_OPTICAL_COPPER   4

#define VCC_HIGH_THR_QSFP56       36300
#define VCC_LOW_THR_QSFP56        29700
#define TEMP_HIGH_THR_QSFP56      80
#define TEMP_LOW_THR_QSFP56       (-10)
#define RX_POWER_HIGH_THR_QSFP56  50119
#define RX_POWER_LOW_THR_QSFP56   724
#define TX_POWER_HIGH_THR_QSFP56  50119
#define TX_POWER_LOW_THR_QSFP56   1413
#define BAIS_HIGH_THR_QSFP56      7500
#define BAIS_LOW_THR_QSFP56       0

#define VCC_HIGH_THR_QSFP         35000
#define VCC_LOW_THR_QSFP          31000
#define TEMP_HIGH_THR_QSFP        80
#define TEMP_LOW_THR_QSFP         (-10)
#define RX_POWER_HIGH_THR_QSFP    34700
#define RX_POWER_LOW_THR_QSFP     500
#define TX_POWER_HIGH_THR_QSFP    34700
#define TX_POWER_LOW_THR_QSFP     700
#define BAIS_HIGH_THR_QSFP        7500
#define BAIS_LOW_THR_QSFP         250
#define SUPPORT_INFO_LEN          7

struct dmp_smbus_req_msg {
    unsigned char lun;  /* bit0-bit3: 0 local, 1-15 dev id; bit 7: 1 last frame, 0 more data */
    unsigned char arg;
    unsigned char op_cmd;
    unsigned char op_fun;
    unsigned int offset;
    unsigned int length;
    unsigned char msg[DMP_SMBUS_MSG_MAX_LEN - DMP_MSG_HEAD_LEN];
};

struct out_band_ds_port_stat_info {
    unsigned long long mac_tx_mac_pause_num;
    unsigned long long mac_rx_mac_pause_num;
    unsigned long long mac_tx_pfc_pkt_num;
    unsigned long long mac_tx_pfc_pri0_pkt_num;
    unsigned long long mac_tx_pfc_pri1_pkt_num;
    unsigned long long mac_tx_pfc_pri2_pkt_num;
    unsigned long long mac_tx_pfc_pri3_pkt_num;
    unsigned long long mac_tx_pfc_pri4_pkt_num;
    unsigned long long mac_tx_pfc_pri5_pkt_num;
    unsigned long long mac_tx_pfc_pri6_pkt_num;
    unsigned long long mac_tx_pfc_pri7_pkt_num;
    unsigned long long mac_rx_pfc_pkt_num;
    unsigned long long mac_rx_pfc_pri0_pkt_num;
    unsigned long long mac_rx_pfc_pri1_pkt_num;
    unsigned long long mac_rx_pfc_pri2_pkt_num;
    unsigned long long mac_rx_pfc_pri3_pkt_num;
    unsigned long long mac_rx_pfc_pri4_pkt_num;
    unsigned long long mac_rx_pfc_pri5_pkt_num;
    unsigned long long mac_rx_pfc_pri6_pkt_num;
    unsigned long long mac_rx_pfc_pri7_pkt_num;
    unsigned long long mac_tx_total_pkt_num;
    unsigned long long mac_tx_total_oct_num;
    unsigned long long mac_tx_bad_pkt_num;
    unsigned long long mac_tx_bad_oct_num;
    unsigned long long mac_rx_total_pkt_num;
    unsigned long long mac_rx_total_oct_num;
    unsigned long long mac_rx_bad_pkt_num;
    unsigned long long mac_rx_bad_oct_num;
    unsigned long long roce_rx_rc_pkt_num;
    unsigned long long roce_rx_all_pkt_num;
    unsigned long long roce_rx_err_pkt_num;
    unsigned long long roce_tx_rc_pkt_num;
    unsigned long long roce_tx_all_pkt_num;
    unsigned long long roce_tx_err_pkt_num;
    unsigned long long roce_cqe_num;
    unsigned long long roce_rx_cnp_pkt_num;
    unsigned long long roce_tx_cnp_pkt_num;
};

/* out band memory info */
#define D_DDR_BITS  16
enum DDR_TYPE {
    DDR4 = 0,
    DDR5 = 1
};
enum DDR_FREQ {
    DDR4_FREQ = 2133,
    DDR5_FREQ = 2745,
    INVALID_DDR_FREQ = 0
};

struct outband_mem_info {
    unsigned int ddr_type;          // 0:3
    unsigned int ddr_cap;           // 4:7
    unsigned int ddr_channel;       // 8:11
    unsigned int ddr_rank;          // 12:15
    unsigned int ddr_ecc_enable;    // 16:19
    unsigned char ddr_bits[2];      // 20:21
    unsigned char ddr_min_v[2];     // 22:23
    unsigned int ddr_freq;          // 24:27 main freq
    unsigned int ddr_cur_freq;      // 28:31
    unsigned char ddr_manufac[24];  // 32:55
};

struct ioctl_get_rootkey {
    unsigned int dev_id;
    unsigned int key_type;
    unsigned int rootkey_status;
};

#if defined(CFG_SOC_PLATFORM_CLOUD_V2)

#define DATA_INVALLID_VALUE 0xff
#define XSFP_DATA_BASE_LEN 32
#define MAX_IPC_MAX_TRANSLEN_EXPAND 220
#define MAX_IPC_MAX_TRANSLEN 32

typedef struct outband_xspf_base_info {
    char vendor_name[XSFP_DATA_BASE_LEN];
    char vendor_sn[XSFP_DATA_BASE_LEN];
    char vendor_pn[XSFP_DATA_BASE_LEN];
    char date_code[XSFP_DATA_BASE_LEN];
    unsigned char transceiver_type;
    unsigned char optical_type;
    unsigned char optical_lane_cnt;
    unsigned char reserve;
    unsigned int optical_speed;
}OUTBAND_XSPF_BASE_INFO;

typedef struct outband_xsfp_threshold_info {
    int vcc_high_warning_threshold;
    int vcc_low_warning_threshold;
    int temp_high_warning_threshold;
    int temp_low_warning_threshold;
    int tx_power_warning_high_threshold;
    int tx_power_warning_low_threshold;
    int rx_power_warning_high_threshold;
    int rx_power_warning_low_threshold;
    int bais_warning_high_threshold;
    int bais_warning_low_threshold;
    int vcc_high_critical_threshold;
    int vcc_low_critical_threshold;
    int temp_high_critical_threshold;
    int temp_low_critical_threshold;
    int tx_power_critical_high_threshold;
    int tx_power_critical_low_threshold;
    int rx_power_critical_high_threshold;
    int rx_power_critical_low_threshold;
    int bais_critical_high_threshold;
    int bais_critical_low_threshold;
}OUTBAND_XSPF_THRESHOLD_INFO;

#define XSFP_POWER_MAX_LEN 16
#define GET_RUNTIME_INFO    0
#define GET_ALARM_INFO      1
#define OUTBAND_SECONDS_IN_A_DAY   (24 * 60 * 60)

#pragma pack(push, 1)  // 设置结构体的字节对齐为1字节，以防止填充字节
typedef struct outband_xsfp_power_info {
    int voltage;
    unsigned char tx_power[XSFP_POWER_MAX_LEN];
    unsigned char rx_power[XSFP_POWER_MAX_LEN];
    unsigned char tx_bias[XSFP_POWER_MAX_LEN];
    int tx_los;
    int rx_los;
    int tx_lol;
    int rx_lol;
#ifdef CFG_SOC_PLATFORM_CLOUD_V3
    int temperature;
    int tx_fault;
    float host_snr[XSFP_SNR_LEN];
    float media_snr[XSFP_SNR_LEN];
    unsigned char access_failed;
#endif
}OUTBAND_XSPF_POWER_INFO;

typedef struct outband_extend_power_runtime_info {
    STRUCT_HEAD struct_head;
    unsigned int optical_run_time;          // BYTE[8:11], 4字节，光模块运行时间，单位秒
    unsigned int optical_power_on_time;     // BYTE[12:15], 4字节，单位秒
    unsigned short optical_power_on_count;  // BYTE[16:17]
    unsigned short optical_power_on_status; // BYTE[18:19]
    unsigned short odsp_temp;               // BYTE[20:21], 2字节, 单位1/256 degree
    unsigned int odsp_high_heat_time;       // BYTE[22:25], 4字节，单位秒
    unsigned int laser_run_time;            // BYTE[26:29], 4字节，单位秒
    unsigned short laser_temp;              // BYTE[30:31], 2字节, 单位1/256 degree
    unsigned short laser_core_temp;         // BYTE[32:33]
}OUTBAND_EXTEND_POWER_RUNTIME_INFO;
 
typedef struct outband_extend_power_alarm_info {
    STRUCT_HEAD struct_head;
    unsigned int hard_bad;                      // BYTE[8:11], 4字节, hard_bad
    unsigned int hard_err;                      // BYTE[12:15], 4字节, hard_err
    unsigned short electrical_link_detection;   // BYTE[16:17], 2字节, 电链路检测
    unsigned short optical_link_detection;      // BYTE[18:19], 2字节, 光链路检测
}OUTBAND_EXTEND_POWER_ALARM_INFO;
#pragma pack(pop) // 恢复默认的字节对齐

typedef struct outband_xsfp_mac_pkt_info {
    unsigned int mac_rx_total_pkt_num;
    unsigned int mac_rx_fcs_err_pkt_num;
    unsigned int mac_rx_bad_pkt_num;
    unsigned int mac_tx_bad_pkt_num;
    unsigned long long link_up_cnt;
    unsigned long long link_down_cnt;
    unsigned int up_timestamp[LINK_STAT_MAX_IDX];
    unsigned int down_timestamp[LINK_STAT_MAX_IDX];
}OUTBAND_XSPF_MAC_PKT_INFO;

struct rdfx_mac_stat {
    unsigned int mac_rx_total_pkt_num;
    unsigned int mac_rx_fcs_err_pkt_num;
    unsigned int mac_rx_bad_pkt_num;
    unsigned int mac_tx_bad_pkt_num;
    unsigned long long link_up_cnt;
    unsigned long long link_down_cnt;
    unsigned int up_timestamp[LINK_STAT_MAX_IDX];
    unsigned int down_timestamp[LINK_STAT_MAX_IDX];
};

typedef enum {
    IDENTIFIER_UNDEF        = 0x0,
    IDENTIFIER_GBIC         = 0x1,
    IDENTIFIER_SFP          = 0x3,
    IDENTIFIER_QSFP         = 0x0C,
    IDENTIFIER_QSFP_PLUS    = 0x0D,
    IDENTIFIER_QSFP28       = 0x11,
    IDENTIFIER_QSFP_DD      = 0x18,
    IDENTIFIER_QSFP56       = 0x1E
} optical_module_identifier_e;

typedef enum {
    XSFP_UNKNOW,
    XSFP_SFP,
    XSFP_QSFP,
    XSFP_QSFP_DD
} XSFP_TYPE;

#define XSFP_CHANNEL_NUM  8
#define LINKDOWN_INFO_NUM 1
#define BYTE_OFFSET       256
#define TEMP_SCAL_UINT    255
#define OUT_TEMP_INVALID      0X7EFF
#define OUT_TEMP_FAULT        0X7FFF
#define DEV_MON_LANE_NUM  8

#pragma pack(push, 1)  // 设置结构体的字节对齐为1字节，以防止填充字节
// 一个网口down的数据包
typedef struct inner_outband_linkdown_info {
    unsigned int time;
    unsigned int reserve;
    unsigned int volt; // 单位0.1mV
    unsigned int tx_power[XSFP_CHANNEL_NUM]; // 单位0.1uW
    unsigned int rx_power[XSFP_CHANNEL_NUM];
    unsigned int bias[XSFP_CHANNEL_NUM]; // 单位uA
    unsigned int tx_los;
    unsigned int rx_los;
    unsigned int tx_lol;
    unsigned int rx_lol;
    unsigned int temperature;
    unsigned int tx_fault;
    float host_snr[XSFP_SNR_LEN];
    float media_snr[XSFP_SNR_LEN];
    unsigned int device_id;
} INNER_OUTBAND_LINKDOWN_INFO;
 
typedef struct outband_port_linkdown_info { // 闪断时端口统计信息
    unsigned int time;                          // 时间戳
    float npu_rx_snr[DEV_MON_LANE_NUM];         // NPU端口SNR, 当前使用4个通道, 需要在xxx函数转换获取到的数据
    unsigned long long  pcs_err_cnt;             // pcs_err_cnt
    unsigned long long  cw_before_correct_cnt;  // fec纠前误码
    unsigned long long  cw_correct_cnt;         // fec纠后误码
    unsigned long long  cw_uncorrect_cnt;       // fec未纠误码统计值
    float cdr_host_snr[DEV_MON_LANE_NUM];       // cdr host snr, qilian和5901都转换成float
    float cdr_media_snr[DEV_MON_LANE_NUM];      // cdr host snr, qilian和5901都转换成float
    unsigned int device_id;                     // 区分cdr snr的闪断信息来自于主die或从die
} outband_port_linkdown_info_t;
 
typedef struct outband_linkdown_info_head {
    unsigned char type;    // 标志数据包类型(如何解析)
    unsigned char all;     // 队列中有多少数据包
    unsigned char num;     // 本帧中有多少数据包
    unsigned char len;     // 一个数据包有多长
} outband_linkdown_info_head_t;
 
typedef struct outband_xsfp_linkdown_info {
    outband_linkdown_info_head_t info_head;
    INNER_OUTBAND_LINKDOWN_INFO inner_info[LINKDOWN_INFO_NUM]; // 一帧最多包含1个数据包
} OUTBAND_XSFP_LINKDOWN_INFO;
 
typedef struct outband_port_linkdown_packet {
    STRUCT_HEAD struct_head;
    outband_linkdown_info_head_t info_head;
    outband_port_linkdown_info_t inner_info[LINKDOWN_INFO_NUM]; // 一帧最多包含1个数据包
} outband_port_linkdown_packet_t;
#pragma pack(pop) // 恢复默认的字节对齐

typedef struct outband_port_statistic_info {
    STRUCT_HEAD struct_head;
    unsigned long long rx_total_pkt_num;
    unsigned long long rx_fec_err_pkt_num;
    unsigned long long rx_bad_pkt_num;
    unsigned long long tx_bad_pkt_num;
    unsigned long long pcs_err_cnt;
    unsigned long long fec_pre;
    unsigned long long fec_succ;
    unsigned long long fec_fail;
    float npu_rx_snr[DEV_MON_LANE_NUM];
    float cdr_host_snr[DEV_MON_LANE_NUM];
    float cdr_media_snr[DEV_MON_LANE_NUM];
} OUTBAND_PORT_STATISTIC_INFO;

struct dev_mon_cdr_snr_info {
    unsigned int retimer_type;
    unsigned int host_snr[DEV_MON_LANE_NUM];
    unsigned int media_snr[DEV_MON_LANE_NUM];
    unsigned int his_host_snr[DEV_MON_LANE_NUM];
    unsigned int his_media_snr[DEV_MON_LANE_NUM];
};

typedef struct outband_fmea_info {
    unsigned char cmd_version;
    unsigned char qdd_fail;
    unsigned char cdr_fail;
    unsigned char cpld_fail;
}OUTBAND_FMEA_INFO;

enum dev_mon_cdr_retimer_type {
    DEV_MON_CDR_RETIMER_TYPE_DEFAULT,
    DEV_MON_CDR_RETIMER_TYPE_5901,
    DEV_MON_CDR_RETIMER_TYPE_HUYANG,
};

#endif

struct flash_rw_buf {
    unsigned char *buff;
    unsigned int size;
};

struct ddr_tempr_ioctl_para {
    unsigned int dev_id;
    unsigned int ddr_type;
    unsigned int buf_size;
    void *buf;
};

#define SSL_RET_SUCCESS 1

#define HTTPS_USER_LEN 512
#define HTTPS_TIME_LEN 32
#define HTTPS_SERIAL_NUM_LEN 64
#define COMMON_CERT_LEN 4096
#define FLASH_CERT_OFFSET 4
#define MAX_LANE_NUM 8

struct https_cert_info {
    char issuer[HTTPS_USER_LEN];
    char user[HTTPS_USER_LEN];
    char start_time[HTTPS_TIME_LEN];
    char end_time[HTTPS_TIME_LEN];
    char serial_num[HTTPS_SERIAL_NUM_LEN];
};

struct secure_boot_info {
    unsigned int flag;
    struct https_cert_info whitelist_cert;
    struct https_cert_info blacklist_cert;
};

struct flash_cert {
    unsigned int dev_id;
    unsigned int cert_len;
    unsigned int flag;
    unsigned char *cer;
};

struct secure_boot_cert {
    struct flash_cert flag;
    struct flash_cert whitelist;
    struct flash_cert blacklist;
};

enum {
    GET_SECURE_BOOT_WHITELIST = 0,
    GET_SECURE_BOOT_BLACKLIST,
    GET_HTTPS_BOOT_CERT,
    GET_HTTPS_CRL_CERT,
    GET_SECURE_BOOT_FLAG
};

// optical prbs
#define MAX_OPTICAL_MODULE_NUM 1
typedef enum {
    OPTICAL_MODULE_PRBS_GET_STATE = 0,
    OPTICAL_MODULE_PRBS_CONFIG,
    OPTICAL_MODULE_PRBS_CLEAR_STATISTICS,
    OPTICAL_MODULE_PRBS_GET_RESULT,
    OPTICAL_MODULE_PRBS_CLOSE,
    OPTICAL_MODULE_PRBS_OP_TYPE_MAX
} OPTICAL_MODULE_PRBS_OPERATION;

typedef enum {
    PRBS31,
    PRBS23,
    PRBS15,
    PRBS13,
    PRBS9,
    PRBS7,
    SSPRQ
} OPTICAL_MODULE_PRBS_TYPR;

typedef enum {
    CONFIG_SUCCESS = 0,
    CONFIG_FAIL
} optical_module_operation_result;      // config,clear_statistics,

#pragma pack(push, 1)  // 设置结构体的字节对齐为1字节，以防止填充字节
struct optical_module_config {
    unsigned char config_item;
    unsigned int test_time;
    unsigned char prbs_type;
    unsigned char optical_module_num;
    unsigned char optical_module_id[MAX_OPTICAL_MODULE_NUM];
};

struct optical_module_clear_statistics {
    unsigned char optical_module_num;
    unsigned char optical_module_id[MAX_OPTICAL_MODULE_NUM];
};

#pragma pack(pop) // 恢复默认的字节对齐

struct get_node_info_para {
    unsigned int dev_id;
    unsigned int dev_num;
    unsigned int node_num;
    unsigned int node_value;
};

#define NET_DATA            2048
#define MAX_CMD_PAYLOAD_LEN 2000

void dev_mon_api_get_chip_pcie_err_rate(SYSTEM_CB_T* cb, DM_INTF_S* intf, DM_RECV_ST* msg);
void dev_mon_api_clear_chip_pcie_err_rate(SYSTEM_CB_T* cb, DM_INTF_S* intf, DM_RECV_ST* msg);
void dev_mon_board_passthru_mcu(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);

#if defined(CFG_SOC_PLATFORM_MINIV2) || defined(CFG_SOC_PLATFORM_CLOUD)
void dev_mon_get_vrd_temperature(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
#endif

void dev_mon_get_cdr_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);

#ifdef CFG_SOC_PLATFORM_CLOUD
void dev_mon_api_out_band_get_utilization_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_frequency_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_hbm_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_mac_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_gateway_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_ip_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_package_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
#endif

#if defined(CFG_SOC_PLATFORM_CLOUD_V2)
void dev_mon_api_out_band_get_xsfp_base_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_xsfp_threshold_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_xsfp_power_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_xsfp_mac_pkt_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_xsfp_linkdown_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_extend_power_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_port_statistic_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_optical_module_prbs(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_port_linkdown_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_fmea_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
#endif

#ifdef CFG_FEATURE_HBM_MANUFACTURER_ID
void dev_mon_api_get_hbm_manufacturer_id(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
#endif
#ifdef CFG_FEATURE_ROOTKEY_STATUS
void dev_mon_api_get_rootkey_status(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
#endif
#if (defined(CFG_SOC_PLATFORM_CLOUD) || defined(CFG_SOC_PLATFORM_CLOUD_V2))
void dev_mon_api_set_device_computing_power(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
#endif

#ifdef CFG_SOC_PLATFORM_MINIV3
void testlib_api_set_efuse_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void testlib_api_check_efuse_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
#endif

int devdrv_get_user_config_product(unsigned int devid, const char *name, unsigned char *buf, unsigned int *buf_size);
void dev_mon_api_set_user_config_product(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_get_user_config_product(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_clear_user_config_product(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
int dev_mon_get_freq(unsigned int dev_id, unsigned char dev_type, unsigned int *ret_value);
int dmanage_get_device_ddr_meminfo(unsigned int dev_id, struct dsmi_get_memory_info_stru *memory_info);
void dev_mon_api_get_memory_information(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
int dev_mon_get_memory_info(unsigned int dev_id, unsigned int vfid,
    struct dsmi_get_memory_info_stru *pdevice_memory_info);
int dmanage_get_ddr_info_single(char *node_name, int name_len, struct dsmi_get_memory_info_stru *pdevice_memory_info);
void dev_mon_api_get_system_info_parameters(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
int get_node_info(struct get_node_info_para *para, struct dsmi_get_memory_info_stru *memory_info);
void dev_mon_api_get_ddr_temperature(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_outband_get_mem_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
int devdrv_open_device_manager(void);
void dev_mon_api_get_cpu_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_get_chip_type(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_get_https_cert_info(SYSTEM_CB_T* cb, DM_INTF_S* intf, DM_RECV_ST* msg);
void dev_mon_api_get_secure_boot_cert_info(SYSTEM_CB_T* cb, DM_INTF_S* intf, DM_RECV_ST* msg);

#ifdef CFG_FEATURE_SERDES_INFO
void dev_mon_api_set_serdes_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
int ddmp_get_serdes_info(DM_INTF_S *intf, DM_RECV_ST *data, const unsigned char *buff, unsigned int total_length);
void dev_mon_api_get_serdes_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
#endif

#if defined(CFG_FEATURE_INIT_MCU_BOARD_ID)
void dev_mon_api_get_mcu_board_id(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
#endif

#endif