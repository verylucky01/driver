/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_INNER_INFO_GET_H__
#define __DCMI_INNER_INFO_GET_H__

#include <stdbool.h>
#include "dcmi_interface_api.h"
#include "dcmi_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifdef DT_FLAG
#define STATIC
#else
#define STATIC static
#endif

#define MAX_PCIE_INFO_LENTH      512
#define MAX_STR_LENTH            256

#define DCMI_ELABEL_SN    0x23
#define DCMI_ELABEL_MF    0x21
#define DCMI_ELABEL_PN    0x22
#define DCMI_ELABEL_MODEL 0x24

#define DCMI_ELABEL_PN_MODEL_TYPE    0x31
#define DCMI_ELABEL_MODEL_MODEL_TYPE 0x32

#define A200_A2_MODEL_20T_ID_MIN 0x96
#define A200_A2_MODEL_20T_ID_MAX 0xf9    /* 310b模组场景20T BoardID范围 */
#define A200_A2_XP_20T_ID_MIN    0x10e
#define A200_A2_XP_20T_ID_MAX    0x12c   /* 310b芯片场景20T BoardID范围 */

// 自定hccs互联状态
#define HCCS_OFF 0
#define HCCS_ON  1

#define HEALTH_UNKNOWN      4

#define DCMI_PCIE_EP_MODE (1)
#define DCMI_PCIE_RC_MODE (0)

#define DCMI_DEVICE_NOT_EXIST    0xffffffff

#define DRV_VERSION_INFO_PATH       "/driver/version.info"
#define DRV_INSTALL_PATH_FIELD      "Driver_Install_Path_Param"

#define DCMI_910B_BIN0 0
#define DCMI_910B_BIN1 1
#define DCMI_910B_BIN2 2
#define DCMI_910B_BIN3 3

#define DCMI_MEM_SIZE_16G 16
#define DCMI_MEM_SIZE_24G 24
#define DCMI_MEM_SIZE_48G 48
#define DCMI_MEM_SIZE_32G 32

#define DCMI_MEM_16G_MB_VALUE 16384
#define DCMI_MEM_24G_MB_VALUE 24576
#define DCMI_MEM_32G_MB_VALUE 32768
#define DCMI_MEM_48G_MB_VALUE 49152

#define DEVELOP_A_BOM_PCB_MASK (0xF)
#define DEVELOP_C_BOM_PCB_MASK (0x7)

#define PAGE_SIZE 0x1000
#define PAGE_SIZE_MASK 0xfffffffffffff000

#define CPU_ALARM_SECTION_LEN        256

#define MEMORY_SIZE_7GB_TO_MB      (7 * 1024)
#define MEMORY_SIZE_7_5GB_TO_MB    (15 * 512)
#define MEMORY_SIZE_14GB_TO_MB     (14 * 1024)
#define MEMORY_SIZE_15GB_TO_MB     (15 * 1024)
#define MEMORY_SIZE_30GB_TO_MB     (30 * 1024)
#define NOISE_7GB_TO_MB            ((8 - 7) * 1024)
#define NOISE_7_5GB_TO_MB          ((16 - 15) * 512)
#define NOISE_14GB_TO_MB           ((16 - 14) * 1024)
#define NOISE_15GB_TO_MB           ((16 - 15) * 1024)
#define NOISE_30GB_TO_MB           ((32 - 30) * 1024)

#define DIGITAL_NUM_TO_PER      100

#define DCMI_DEVELOP_BOARD_ID_I2C_ADDR    0x21
#define DCMI_310B_BOARD_ID_I2C_ADDR       0x20 // I2C地址是0x40，7bit的地址模式，地址是0x20

#define DCMI_USAGE_MAX          100
#define DCMI_AICORE_NUM_MAX         32

#define DCMI_SEC_SUB_CMD_CUST_SIGN_FLAG 2

/* 单bit错误隔离页数量 */
#define SINGLE_BIT_ERROR 0

enum dcmi_tops_type {
    IS_310B_8TOPS_TYPE = 0,
    IS_310B_20TOPS_TYPE,
};

struct dcmi_device_info {
    int chip_slot;                             // 芯片在卡上的物理位号
    int logic_id;                              // 硬件逻辑标号，对应dsmi的编号
    unsigned int phy_id;                       // 芯片物理ID
    char chip_pcieinfo[MAX_PCIE_INFO_LENTH];   // 本级pcie信息
    char switch_pcieinfo[MAX_PCIE_INFO_LENTH]; // 前一级pcie信息
};

struct dcmi_card_info {
    int card_id;                   // 用pcie的busid来表示
    int slot_id;                   // 对应的pcie槽位
    int device_count;              // 一块板卡上NPU芯片个数
    int mcu_id;                    // mcu的编号，在NPU芯片后面
    int cpu_id;                    // cpu的编号，没有就为-1，在mcu芯片后面
    int elabel_pos;                // 电子标签在哪个芯片上，D卡在mcu上、小站在3559上，开发者底板(实际在NPU上)、模块在模块上
    int board_id_pos;
    int device_loss;               // 没有找到的芯片个数
    int card_board_id;
    struct dcmi_pcie_info_all str_pci_info;
    char pcie_info_pre[MAX_PCIE_INFO_LENTH];        // 前两级pcie信息
    struct dcmi_device_info device_info[MAX_DEVICE_NUM_IN_CARD];
};

struct dcmi_board_details_info {
    int board_type;   // 硬件形态，模组、标卡、单板
    int sub_board_type;
    int product_type;
    int chip_type;
    int board_id;
    int bom_id;       // 区分小站C板供应1.5改板单板
    int card_count;   // 标卡个数 对模块，这个为1就行
    int device_count; // npu芯片总个数
    int device_count_in_one_card;
    int is_has_mcu;
    int is_has_npu;
    int mcu_access_chan;
    struct dcmi_card_info card_info[MAX_CARD_NUM_IN_BROAD];
};

struct dcmi_mainboard_info {
    unsigned int mainboard_id;
    unsigned int reserve[31]; // 预留字段
};

struct dcmi_product_computing_template {
    unsigned int chip_type;
    unsigned int mem_max_size; // 单位 GB
    unsigned int template_num;
    struct dcmi_computing_template *split_template;
};

struct dcmi_product_computing_template_for_910B {
    int bin_type_910b;
    unsigned int template_num;
    struct dcmi_computing_template *split_template;
};

extern struct dcmi_board_details_info g_board_details;
extern struct dcmi_mainboard_info g_mainboard_info;

int get_card_board_info(struct dcmi_card_info *card_info, unsigned int *slot_id);

int dcmi_get_card_info(int card_id, struct dcmi_card_info **card_info);

int dcmi_get_board_id_inner();

int dcmi_get_board_chip_type();

int dcmi_get_board_type(void);

int dcmi_get_sub_board_type(void);

unsigned int dcmi_get_maindboard_id_inner(void);

int dcmi_get_device_count_in_one_card(void);

int dcmi_get_mcu_access_chan(void);

int dcmi_get_card_board_id(int card_id, int *board_id);

int dcmi_get_pcie_slot(int card_id, int *pcie_slot);

int dcmi_get_board_id_pos_in_card(int card_id, int *board_id_pos);

int dcmi_get_elabel_pos_in_card(int card_id, int *elabel_pos);

void dcmi_get_elabel_item_it(int *sn_id, int *mf_id, int *pn_id, int *model_id);

int dcmi_get_product_type_inner(void);

int dcmi_get_310b_tops_type(void);

int dcmi_is_has_mcu(void);

int dcmi_get_hccs_status(int card_id, int device_id, int *hccs_status);

int dcmi_get_rc_ep_mode(unsigned int *mode);

int dcmi_get_boot_status(unsigned int mode, int device_id);

int dcmi_version_info_of_drv_by_field(const char* field, unsigned char* item_out, unsigned int len);

int dcmi_get_firmware_version(int card_id, int device_id, unsigned char* firmware_version, int len_firmware_version);

int dcmi_get_rootkey_status(int card_id, int device_id, unsigned int key_type, unsigned int *rootkey_status);

int dcmi_get_template_info_by_name(int card_id, const char *name, struct dcmi_computing_template *computing_template);

int dcmi_get_template_info_all(int card_id, struct dcmi_computing_template *template_out, unsigned int template_size,
    unsigned int *template_num);

int dcmi_get_show_template_info_all(int card_id, struct dcmi_computing_template *template_out,
    unsigned int template_size, unsigned int *template_num);

int dcmi_cpu_get_board_info(int card_id, struct dcmi_board_info *board_info);

int dcmi_cpu_get_board_id(int card_id, unsigned int *board_id);

int dcmi_cpu_get_health(int card_id, unsigned int *health);

int dcmi_cpu_get_device_frequency(int card_id, int device_type, unsigned int *frequency);

int dcmi_cpu_get_device_die(int card_id, int device_id, enum dcmi_die_type input_type, struct dcmi_die_id *die_id);

int dcmi_cpu_get_memory_info(int card_id, struct dcmi_memory_info *memory_info);

int dcmi_cpu_get_chip_info(int card_id, struct dcmi_chip_info_v2 *chip_info);

int dcmi_get_npu_chip_info(int card_id, int device_id, struct dcmi_chip_info_v2 *chip_info);

int dcmi_get_npu_pcie_info(int card_id, int device_id, struct dcmi_pcie_info *pcie_idinfo);

int dcmi_get_pcie_info_win(int device_logic_id, struct dcmi_pcie_info_all *pcie_idinfo);

int dcmi_get_npu_pcie_info_v2(int card_id, int device_id, struct dcmi_pcie_info_all *pcie_idinfo);

int dcmi_get_board_info_for_develop(struct dcmi_board_info *board_info);

int dcmi_get_npu_board_info(int card_id, int device_id, struct dcmi_board_info *board_info);

int dcmi_get_npu_device_power_info(int card_id, int device_id, int *power);

int dcmi_set_npu_device_clear_pcie_error(int card_id, int device_id);

int dcmi_get_npu_device_pcie_error(int card_id, int device_id, struct dcmi_chip_pcie_err_rate *pcie_err_code_info);

int dcmi_get_npu_device_die(int card_id, int device_id, enum dcmi_die_type input_type, struct dcmi_die_id *die_id);

int dcmi_get_npu_device_health(int card_id, int device_id, unsigned int *health);

int dcmi_get_npu_aicore_info(int card_id, int device_id, struct dcmi_aicore_info *aicore_info);

int dcmi_get_npu_aicpu_info(int card_id, int deviceId, struct dcmi_aicpu_info *aicpu_info);

int dcmi_get_npu_device_boot_status(int card_id, int device_id, enum dcmi_boot_status *boot_status);

int dcmi_get_npu_system_time(int card_id, int device_id, unsigned int *time);

int dcmi_get_npu_device_temperature(int card_id, int device_id, int *temprature);

int dcmi_get_npu_device_voltage(int card_id, int device_id, unsigned int *voltage);

int dcmi_get_npu_ecc_info(
    int card_id, int device_id, enum dcmi_device_type input_type, struct dcmi_ecc_info *device_ecc_info);

int dcmi_clear_npu_ecc_statistics_info(int card_id, int device_id);

int dcmi_get_npu_device_frequency(int card_id, int device_id, int device_type, unsigned int *frequency);

int dcmi_get_npu_hbm_info(int card_id, int device_id, struct dcmi_hbm_info *hbm_info);

int dcmi_get_npu_device_memory_info_v2(int card_id, int device_id, struct dcmi_memory_info *memory_info);

int dcmi_get_npu_device_memory_info_v3(int card_id, int device_id, struct dcmi_get_memory_info_stru *memory_info);

int dcmi_get_npu_device_utilization_rate(int card_id, int device_id, int input_type, unsigned int *utilization_rate);

int dcmi_get_npu_soc_sensor_info(
    int card_id, int device_id, enum dcmi_manager_sensor_id sensor_id, union dcmi_sensor_info *sensor_info);

int dcmi_get_npu_device_board_id(int card_id, int device_id, unsigned int *board_id);

int dcmi_get_npu_device_component_count(int card_id, int device_id, unsigned int *component_count);

int dcmi_get_npu_device_component_list(
    int card_id, int device_id, enum dcmi_component_type *component_table, unsigned int component_count);

int dcmi_get_npu_device_component_static_version(
    int card_id, int device_id, enum dcmi_component_type component_type, unsigned char *version_str, unsigned int len);

int dcmi_get_npu_device_cgroup_info(int card_id, int device_id, struct dcmi_cgroup_info *cg_info);

int dcmi_get_npu_device_llc_perf_para(int card_id, int device_id, struct dcmi_llc_perf *perf_para);

int dcmi_get_npu_device_info(
    int card_id, int device_id, enum dcmi_main_cmd main_cmd, unsigned int sub_cmd, void *buf, unsigned int *size);

int dcmi_get_npu_device_mac_count(int card_id, int device_id, int *count);

int dcmi_get_npu_device_mac(int card_id, int device_id, int mac_id, char *mac_addr, unsigned int len);

int dcmi_get_npu_device_gateway(
    int card_id, int device_id, enum dcmi_port_type input_type, int port_id, struct dcmi_ip_addr *gateway);

int dcmi_get_npu_device_ip(int card_id, int device_id, enum dcmi_port_type input_type, int port_id,
    struct dcmi_ip_addr *ip, struct dcmi_ip_addr *mask);

int dcmi_get_npu_device_network_health(int card_id, int device_id, enum dcmi_rdfx_detect_result *result);

int dcmi_get_npu_device_fan_count(int card_id, int device_id, int *count);

int dcmi_get_npu_device_fan_speed(int card_id, int device_id, int fan_id, int *speed);

int dcmi_get_npu_device_user_config(
    int card_id, int device_id, const char *config_name, unsigned int buf_size, unsigned char *buf);

int dcmi_i2c_get_elable_info(int card_id, int item_type, char *elable_data, int *len);

int dcmi_i2c_get_npu_device_elable_info(int card_id, struct dcmi_elabel_info *elabel_info);

int dcmi_get_npu_device_elable_info(int card_id, int device_id, struct dcmi_elabel_info *elabel_info);

int dcmi_get_npu_device_share_enable(int card_id, int device_id, int *enable_flag);

int dcmi_get_npu_device_ssh_enable(int card_id, int device_id, int *enable_flag);

int dcmi_get_npu_device_aicpu_count_info(int card_id, int device_id, unsigned char *count);

int dcmi_get_npu_device_list(int *device_list, int list_size, int *device_count);

int dcmi_get_npu_fault_event(int card_id, int device_id, int timeout, struct dcmi_event_filter filter,
    struct dcmi_event *event);

int dcmi_get_npu_device_dvpp_ratio_info(int card_id, int device_id, struct dcmi_dvpp_ratio *usage);

int dcmi_get_npu_proc_mem_info(int card_id, int device_id, struct dcmi_proc_mem_info *proc_info, int *proc_num);

int dcmi_get_npu_device_cpu_freq_info(int card_id, int device_id, int *enable_flag);

int dcmi_npu_get_capability_group_info(int card_id, int device_id, int ts_id, int group_id,
    struct dcmi_capability_group_info *group_info, int group_count);

int dcmi_npu_get_capability_group_aicore_usage(int card_id, int device_id, int group_id, int *rate);

int dcmi_get_device_boot_area(int card_id, int device_id, int *status);

int dcmi_get_custom_op_secverify_enable(int card_id, int device_id, unsigned char *enable);

int dcmi_get_custom_op_secverify_mode(int card_id, int device_id, unsigned int *model);

#ifndef _WIN32
/* 新增的内部dsmi接口（未对外暴露），临时定义使用 */
int dsmi_get_hccs_status(unsigned int device_id1, unsigned int device_id2, int *hccs_status);
#ifdef SOC
inline int dsmi_get_hccs_status(unsigned int device_id1, unsigned int device_id2, int *hccs_status)
{
    return NPU_ERR_CODE_NOT_SUPPORT;
}
else
int dsmi_get_hccs_status(unsigned int device_id1, unsigned int device_id2, int *hccs_status);
#endif  /* SOC */

#endif  /* _WIN32 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DCMI_INNER_INFO_GET_H__ */