/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DEV_MON_CMD_MANAGER_EXT_H__
#define __DEV_MON_CMD_MANAGER_EXT_H__

#if defined(CFG_SOC_PLATFORM_CLOUD_V2)

typedef struct struct_head {
    char cmd_version;          // BYTE[0], 首次默认版本为0，后续涉及修改递增
    char support_info[7];      // BYTE[1:7], 每个bit代表接下来的结构体中对应顺序的属性是否支持,7表示预留7个字节共56个bit表示结构体元素属性
} STRUCT_HEAD;

#define DMS_HCCS_INFO_RESERVED_BYTES 8
typedef struct hccs_info_struct {
    unsigned int pcs_status;
    unsigned char reserved[DMS_HCCS_INFO_RESERVED_BYTES];
} hccs_status_info_t;

#define DMS_HCCS_MAX_PCS_NUM        (16)
typedef struct {
    unsigned int tx_cnt[DMS_HCCS_MAX_PCS_NUM];
    unsigned int rx_cnt[DMS_HCCS_MAX_PCS_NUM];
    unsigned int crc_err_cnt[DMS_HCCS_MAX_PCS_NUM];
    unsigned int retry_cnt[DMS_HCCS_MAX_PCS_NUM];
    unsigned int reserve[DMS_HCCS_MAX_PCS_NUM * 3];
} hccs_statistic_info_t;

typedef struct hccs_port_lane_details {
    unsigned int hccs_port_pcs_bitmap;
    unsigned int pcs_lane_bitmap[DMS_HCCS_MAX_PCS_NUM];
    unsigned int reserve[DMS_HCCS_MAX_PCS_NUM];
} hccs_lane_info_t;

#define LANE_MODE_OFFSET        9
#define LANE_MODE_MASK          (0x3)
#define LINK_LANE_OFFSET        1
#define HCCS_LANE_SPEED         56
#define SINGLE_PACKETS_BYTES    20
#define FIRST_ERROR_LANE        8
#define MAX_MACRO_NUM           8
#define LANE_NUM               4          // 每个macro口4条lane
#define HCCS_STATUS_OK         0
#define HCCS_STATUS_NOK        1

// 以macro口维度来查询信息
typedef struct outband_hccs_info {
    STRUCT_HEAD struct_head;
    unsigned int pcs_status;    // BYTE[8:11], 4字节，HCCS健康状态，0:ok，1:NOK
    unsigned int lane_mode;     // BYTE[12:15], 4字节, HCCS当前建链lane模式
    unsigned int link_lane_list; // BYTE[16:19], 4字节, HCCS当前建链lane id，一个bit位代表一条lane
    unsigned int link_speed;     // BYTE[20:23], 4字节, 表示链路的标准速率，范围：0~224，单位Gb/s
    unsigned long long tx_packets;     // BYTE[24:31], 8字节, 表示累积发包数量
    unsigned long long tx_bytes;       // BYTE[32:39], 8字节, 表示累积发出字节数量，20*tx packets=tx bytes
    unsigned long long rx_packets;     // BYTE[40:47], 8字节, 表示累积收包数量
    unsigned long long rx_bytes;       // BYTE[48:55], 8字节, 表示累积收到字节数量，20*rx packets=rx bytes
    unsigned long long retry_cnt;      // BYTE[56:63], 8字节，表示数据包的重传次数
    unsigned long long error_cnt;      // BYTE[64:71], 8字节，表示误码数量
    unsigned int first_error_lane;     // BYTE[72:75], 4字节，HCCS当前最小异常的lane id。0XFF代表无异常lane
    unsigned int snr[LANE_NUM];       // BYTE[76:91], 16字节，每条lane的SNR信息
    unsigned int heh[LANE_NUM];       // BYTE[92:107], 16字节，每条lane的半眼高信息
} OUTBAND_HCCS_INFO;

#define XSFP_SNR_LEN        8

#pragma pack(push, 1)  // 设置结构体的字节对齐为1字节，以防止填充字节
// 补充0x068E中因兼容性问题屏蔽A2的字段，其中temperature，tx_fault和access_failed为预留字段
typedef struct outband_xsfp_power_info_extra {
    STRUCT_HEAD struct_head;
    int temperature;
    int tx_fault;
    float host_snr[XSFP_SNR_LEN];
    float media_snr[XSFP_SNR_LEN];
    unsigned char access_failed;
} OUTBAND_XSFP_POWER_INFO_EXTRA;
#pragma pack(pop) // 恢复默认的字节对齐

#define RESERVE_NUMBER_EXT  6
struct rdfx_extra_statistics_info {
    unsigned long long cw_total_cnt;
    unsigned long long cw_before_correct_cnt;
    unsigned long long cw_correct_cnt;
    unsigned long long cw_uncorrect_cnt;
    unsigned long long cw_bad_cnt;
    unsigned long long trans_total_bit;
    unsigned long long cw_total_correct_bit;
    unsigned long long drop_num;
    unsigned long long pcs_err_count;
    unsigned long long rx_send_app_good_pkts;
    unsigned long long rx_send_app_bad_pkts;
    unsigned int reserved[RESERVE_NUMBER_EXT];
};

#define HUYANG_FMEA_LEVEL_MAJOR 2
#define HUYANG_FMEA_LEVEL_MINOR 1
#define HUYANG_FMEA_LEVEL_NONE  0
#define OUTBAND_HUYANG_FMEA_LOG_MAX 10

struct outband_huyang_fmea_log {
    unsigned int id;       // 告警ID
    unsigned char level;    // 告警等级
    unsigned char type;     // 告警类型
    unsigned char source;   // 告警来源
    unsigned char port;     // 告警位置
    unsigned int time;      // 告警时间
    unsigned int value;     // 告警详情
};

typedef struct outband_huyang_fmea_info {
    STRUCT_HEAD struct_head;
    unsigned char cdr_type; // 有必要传cdr_type吗，你给5901查FMEA我直接报错不就完了
    unsigned char count;    // 故障数量
    unsigned char health;   // 当前健康状态
    unsigned char current;  // 就是说当前已经传递了多少告警，对BMC来说，current!=count表示你还得查
    struct outband_huyang_fmea_log log[OUTBAND_HUYANG_FMEA_LOG_MAX]; // 最大传递10个告警/次
} OUTBAND_HUYANG_FMEA_INFO;

#endif /* CFG_SOC_PLATFORM_CLOUD_V2 */

#if defined(CFG_SOC_PLATFORM_CLOUD_V2)
void dev_mon_api_out_band_get_hccs_info(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_xsfp_power_info_extra(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
void dev_mon_api_out_band_get_huyang_fmea(SYSTEM_CB_T *cb, DM_INTF_S *intf, DM_RECV_ST *msg);
#endif /* CFG_SOC_PLATFORM_CLOUD_V2 */

#endif /* __DEV_MON_CMD_MANAGER_EXT_H__ */