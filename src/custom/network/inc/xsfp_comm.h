/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef XSFP_COMM_H
#define XSFP_COMM_H
#define XSFP_NAME_LEN             17
#define XSFP_OUI_LEN              3
#define XSFP_PN_LEN               17
#define XSFP_WAVE_LEN             2
#define XSFP_SN_LEN               17
#define XSFP_DATACODE_LEN         11
#define XSFP_POWER_LEN            16
#define XSFP_SNR_LEN              8
#define XSFP_MEDIA_LEN            4
#define XSFP_SHOW_SNR_LEN         4
#define XSFP_PHYSICAL_CODE_LEN    16
#define XSFP_MAX_OPTICAL_MODULE_NUM 1
#define XSFP_SPEC_COMPLIANCE_LEN  9
#define XSFP_VENDOR_REV_LENGTH    2
#define MEDIA_LANE0               0
#define MEDIA_LANE1               1
#define MEDIA_LANE2               2
#define MEDIA_LANE3               3

#define DUMP_PAGE_LEN             128
#define SHOW_ONE_ROW              16
#define TOOL_DUMP_REG_MARGIN      8
#define DUMP_CMD_IIC_BUSY         3
#define DUMP_CMD_NOTSUPP          95
#define OPTICAL_NOTSUPP           96
#define PAGE_INDEX_NOTSUPP        97
#define DUMP_ALL_SLOG_CMD         0
#define DUMP_PAGE_INFO_CMD        1

struct xsfp_base_info {
    int wavelength;
    int xsfp_identifier;
    char vendor_sn[XSFP_SN_LEN];
    char vendor_pn[XSFP_PN_LEN];
    char vendor_oui[XSFP_OUI_LEN];
    char vendor_name[XSFP_NAME_LEN];
    char date_code[XSFP_DATACODE_LEN];
};

#pragma pack(1)
struct xsfp_additional_info {
    int voltage;
    unsigned char tx_power[XSFP_POWER_LEN];
    unsigned char rx_power[XSFP_POWER_LEN];
    int vcc_high_threshold;
    int vcc_low_threshold;
    int vcc_high_alarm_threshold;
    int vcc_low_alarm_threshold;
    int temp_high_threshold;
    unsigned int temp_low_threshold;
    int temp_high_alarm_threshold;
    int temp_low_alarm_threshold;
    int tx_power_high_threshold;
    int tx_power_low_threshold;
    int tx_power_high_alarm_threshold;
    int tx_power_low_alarm_threshold;
    int rx_power_high_threshold;
    int rx_power_low_threshold;
    int rx_power_high_alarm_threshold;
    int rx_power_low_alarm_threshold;
    int tx_bias_high_threshold;
    int tx_bias_low_threshold;
    int tx_bias_high_alarm_threshold;
    int tx_bias_low_alarm_threshold;
    unsigned char tx_bias[XSFP_POWER_LEN];
    unsigned char media_info[XSFP_MEDIA_LEN];
    int tx_los;
    int rx_los;
    int tx_lol;
    int rx_lol;
    unsigned char optical_type;
    int temperature;
    int tx_fault;
    float host_snr[XSFP_SNR_LEN];
    float media_snr[XSFP_SNR_LEN];
    unsigned char access_failed;
    unsigned char snr_support;
    unsigned char physical_code[XSFP_PHYSICAL_CODE_LEN];
    unsigned char optical_lane_cnt;
    unsigned char specification_compliance[XSFP_SPEC_COMPLIANCE_LEN];
    unsigned char vendor_rev[XSFP_VENDOR_REV_LENGTH];
    int loopback_mode;
    int tx_disable_status;
    unsigned int optical_speed;
    unsigned char electrical_lane_cnt;
};

struct additional_optical_info {
    unsigned short optical_run_time;                // 2字节, 模块运行时间
    unsigned short optical_power_on_time;           // 2字节, 模块上电运行时间, 光模块累计上电时间, 掉电保存; 单位: 天
    unsigned short optical_power_on_count;          // 2字节, 模块上电次数, 记录包括光模块正常工作下复位,插拔导致的光模块重新上下电的事件统计次数, 掉电保存; 单位: 次
    unsigned short optical_power_on_status;         // 2字节, 不支持
    unsigned short odsp_temp;                       // 2字节, ODSP结温上报, 单位:1/256 degree
    unsigned short odsp_high_heat_time;             // 2字节, 不支持
    unsigned short laser_run_time;                  // 2字节, 激光器运行时间, 激光器开光累计工作时间, 掉点保存; 单位: 天
    unsigned short laser_temp;                      // 2字节, 激光器温度, 单位:1/256 degree
    unsigned short laser_core_temp;                 // 2字节, 不支持
    unsigned int hard_bad;                          // 4字节, 模块本体诊断Error, 说明模块硬件有一般隐患，可能造成业务性能劣化
    unsigned int hard_err;                          // 4字节, 模块本体诊断Bad, 说明硬件硬件有严重故障，业务会受到影响或者已经产生影响
    unsigned short electrical_link_detection;       // 2字节, 电链路诊断告警, 表征当前模块电链路HRX有信号幅度/告警
    unsigned short optical_link_detection;          // 2字节, 光链路诊断告警, 表征光口性能相比出厂有劣化(SNR或者光功率跌落)
    unsigned char severe_optical_link_detection;    // 1字节, 光链路严重告警, 表征光口性能已经严重劣化，处于逼近或已有纠后误码状态
};

struct optical_module_op_result {
    unsigned char optical_module_num;
    unsigned char optical_module_id[XSFP_MAX_OPTICAL_MODULE_NUM];
};

struct prbs_state_result {
    unsigned char optical_module_num;
    unsigned char supported_test_type[XSFP_MAX_OPTICAL_MODULE_NUM];
    unsigned char test_state[XSFP_MAX_OPTICAL_MODULE_NUM];
};

struct optical_module_prbs_result {
    unsigned char optical_module_num;
    unsigned char optical_module_id[XSFP_MAX_OPTICAL_MODULE_NUM];
    unsigned short error_bit_rate[8];
    unsigned char rx_lost_of_lock;      // rx失锁统计
    unsigned char tx_lost_of_lock;      // tx失锁统计
};

struct prbs_result_out_band {
    unsigned char optical_module_num;
    unsigned char optical_module_id[XSFP_MAX_OPTICAL_MODULE_NUM];
    float error_bit_rate[8];
    unsigned char rx_lost_of_lock;      // rx失锁统计
    unsigned char tx_lost_of_lock;      // tx失锁统计
};

struct thread_para_stu {
    unsigned int flag;
    unsigned int time;
    unsigned int count;
    int dev_id;
    unsigned char module_num;
    unsigned char module_id[XSFP_MAX_OPTICAL_MODULE_NUM];
    unsigned char op_type;
};

struct fmea_info_out_band {
    unsigned char qdd_fail;
    unsigned char cdr_fail;
    unsigned char cpld_fail;
};

#pragma pack()
#endif
