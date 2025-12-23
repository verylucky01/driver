/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DSMI_COMMON_INTERFACE_CUSTOM_H
#define DSMI_COMMON_INTERFACE_CUSTOM_H

#include "dsmi_common_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define DEV_DAVINCI_NOT_EXIST    0x68022001
#define HOST_HDC_NOT_EXIST       0x68022002
#define HOST_MANAGER_NOT_EXIST   0x68022003
#define HOST_SVM_NOT_EXIST       0x68022004
#define BOARD_TYPE_RC            1
#define DSMI_HEALTH_ERROR_LEVEL  3
#define DEV_PATH_MAX             128
#define EP_MODE                  "0xd100"
#define DSMI_MAIN_CMD_EX_COMPUTING 0x8000
#define DSMI_MAIN_CMD_EX_CONTAINER 0x8001
#define DSMI_MAIN_CMD_GPIO         0x8002
#define DSMI_MAIN_CMD_EX_CERT      0x8003
#define VDEV_VM_CONFIG_ITEM        0x8004
#define DSMI_MAIN_CMD_EN_DECRYPTION 0x8005
#define DSMI_MAIN_CMD_PCIE_BANDWIDTH      0x8006
#define DSMI_MAIN_CMD_HCCS_BANDWIDTH      0X8007
#define DSMI_MAIN_CMD_HCCS_LINKERR        0X8008
#define DSMI_MAIN_CMD_PCIE_LINKERR        0X8009

typedef enum {
    DSMI_CERT_SUB_CMD_INIT_TLS_PUB_KEY = 0,
    DSMI_CERT_SUB_CMD_INIT_RESERVE,
    DSMI_CERT_SUB_CMD_TLS_CERT_INFO,
    DSMI_CERT_SUB_CMD_MAX,
} DSMI_EX_CERT_SUB_CMD;

#define MAX_CERT_COUNT 15
#define NPU_CERT_MAX_SIZE 2048
typedef struct _certs_chain_data {
    unsigned int count;
    unsigned int data_len[MAX_CERT_COUNT];
    unsigned char data[0];
} CERTS_CHAIN_DATA;

#define CERT_COMMON_NAME_LEN 64
#define CERT_ITEM_NAME_LEN    16
#define CERT_TIME_LEN 32
typedef struct _cert_info {
    unsigned int alarm_stat;
    unsigned int reserve;
    char start_time[CERT_TIME_LEN];
    char end_time[CERT_TIME_LEN];
    char country_name[CERT_ITEM_NAME_LEN];
    char province_name[CERT_ITEM_NAME_LEN];
    char city_name[CERT_ITEM_NAME_LEN];
    char organization_name[CERT_ITEM_NAME_LEN];
    char department_name[CERT_ITEM_NAME_LEN];
    char reserve_name[CERT_COMMON_NAME_LEN];
    char common_name[CERT_COMMON_NAME_LEN];
} CERT_CHIEF_INFO;

typedef struct _csr_info {
    char country_name[CERT_ITEM_NAME_LEN];
    char province_name[CERT_ITEM_NAME_LEN];
    char city_name[CERT_ITEM_NAME_LEN];
    char organization_name[CERT_ITEM_NAME_LEN];
    char department_name[CERT_ITEM_NAME_LEN];
    char reserve_name[CERT_COMMON_NAME_LEN];
    int reserve;
    int csr_len;
    char csr_data[NPU_CERT_MAX_SIZE];
} CSR_INFO;

typedef struct dsmi_computing_token_stru {
    float value;
    unsigned char type;
    unsigned char reserve_c;
    unsigned short reserve_s;
} COMPUTING_TOKEN_STRU;
typedef enum {
    DSMI_EX_COMPUTING_SUB_CMD_TOKEN = 0,
    DSMI_EX_COMPUTING_SUB_CMD_MAX,
} DSMI_EX_COMPUTING_SUB_CMD;

typedef struct tag_pcie_bdfinfo {
    unsigned int bdf_deviceid;
    unsigned int bdf_busid;
    unsigned int bdf_funcid;
} TAG_PCIE_BDFINFO, tag_pcie_bdfinfo;

typedef enum {
    DSMI_EX_CONTAINER_SUB_CMD_SHARE = 0,
    DSMI_EX_CONTAINER_SUB_CMD_MAX,
} DSMI_EX_CONTAINER_SUB_CMD;

typedef enum {
    DSMI_GPIO_SUB_CMD_GET_VALUE = 0,
    DSMI_GPIO_SUB_CMD_SET_VALUE,
    DSMI_GPIO_SUB_CMD_DIRECT_OUTPUT,
    DSMI_GPIO_SUB_CMD_DIRECT_INPUT,
    DSMI_GPIO_SUB_CMD_MAX,
} DSMI_GPIO_SUB_CMD;

typedef enum {
    DSMI_PCIE_CMD_GET_BANDWIDTH = 0,
    DSMI_PCIE_CMD_GET_LINK_TX_RX,
    DSMI_PCIE_CMD_GET_LINK_CRC,
    DSMI_PCIE_CMD_GET_LINK_RETRY,
    DSMI_PCIE_SUB_CMD_EX_MAX,
} DSMI_PCIE_SUB_CMD_EX;

typedef enum {
    DSMI_HCCS_CMD_GET_BANDWIDTH = 0,
    DSMI_HCCS_CMD_GET_LINKERR,
    DSMI_HCCS_SUB_CMD_EX_MAX,
} DSMI_HCCS_SUB_CMD_EX;

typedef struct dsmi_chip_pcie_err_rate_stru {
    unsigned int reg_deskew_fifo_overflow_intr_status;
    unsigned int reg_symbol_unlock_intr_status;
    unsigned int reg_deskew_unlock_intr_status;
    unsigned int reg_phystatus_timeout_intr_status;
    unsigned int symbol_unlock_counter;
    unsigned int pcs_rx_err_cnt;
    unsigned int phy_lane_err_counter;
    unsigned int pcs_rcv_err_status;
    unsigned int symbol_unlock_err_status;
    unsigned int phy_lane_err_status;
    unsigned int dl_lcrc_err_num;
    unsigned int dl_dcrc_err_num;
} PCIE_ERR_RATE_INFO_STU;

DLLEXPORT int dsmi_get_pcie_bdf(int device_id,struct tag_pcie_bdfinfo *pcie_idinfo);

/**
* @ingroup driver
* @brief  Get the pcie err rate of ascend AI processor Hisilicon SOC
* @attention NULL
* @param [in] device_id  The device id
* @param [out] pcie_err_code_info  Get the pcie err rate of ascend AI processor Hisilicon SOC
* @return  0 for success, others for fail
*/

DLLEXPORT int dsmi_get_pcie_error_rate(int device_id, struct dsmi_chip_pcie_err_rate_stru *pcie_err_code_info);

/**
* @ingroup driver
* @brief  clear the pcie err rate of ascend AI processor Hisilicon SOC
* @attention NULL
* @param [in] device_id  The device id
* @return  0 for success, others for fail
*/
DLLEXPORT int dsmi_clear_pcie_error_rate(int device_id);

#define ALM_NAME_LEN    16
#define ALM_EXTRA_LEN   32
#define ALM_REASON_LEN  32
#define ALM_REPAIR_LEN  32

struct dsmi_alarm_info_stru {
    unsigned int id;
    unsigned int level;
    unsigned int clr_type;            /* 0: automatical clear, 1:manaul clear */
    unsigned int moi;    /* blackbox code */
    unsigned char name[ALM_NAME_LEN];
    unsigned char extra_info[ALM_EXTRA_LEN];
    unsigned char reason_info[ALM_REASON_LEN];
    unsigned char repair_info[ALM_REPAIR_LEN];
};

/**
* @ingroup driver
* @brief  Get the detailed device alarm info
* @attention NULL
* @param [in] device_id  The device id
* @param [out] alarmcount The number of alarms on the device
* @param [out] dsmi_alarm_info_stru The detailed alarm info
* @return  0 for success, others for fail
*/
DLLEXPORT int dsmi_get_device_alarminfo(int device_id, int *alarmcount, struct dsmi_alarm_info_stru *palarminfo);

#define COMPUTING_POWER_INFO_RESERVE_NUM    3

struct dsmi_computing_power_info {
    unsigned int data1;
    unsigned int reserve[COMPUTING_POWER_INFO_RESERVE_NUM];
};

/**
* @ingroup driver
* @brief Get the DIE ID of the specified device
* @attention NULL
* @param [in] device_id  The device id
* @param [out] schedule  return n die id infomation
* @return  0 for success, others for fail
* @note Support:Ascend910, Ascend910B
*/
DLLEXPORT int dsmi_get_device_ndie(int device_id, struct dsmi_soc_die_stru *pdevice_die);

/**
* @ingroup driver
* @brief Get the aicore number of the device
* @attention NULL
* @param [in] device_id  The device id
* @param [out] aicorenum The number of aicore
* @return  0 for success, others for fail
* @note Support:Ascend910, Ascend910B
*/
DLLEXPORT int dsmi_get_computing_power_info(int device_id, int computing_power_type,
                                  struct dsmi_computing_power_info *computing_power_info);

/**
* @ingroup driver
* @brief Query the overall health status of the driver
* @attention NULL
* @param [out] phealth
* @return  0 for success, others for fail
*/
DLLEXPORT int dsmi_get_driver_health(unsigned int *phealth);

/**
* @ingroup driver
* @brief Query driver fault code
* @attention NULL
* @param [in] device_id  The device id
* @param [out] errorcount  Number of error codes
* @param [out] perrorcode  error codes
* @return  0 for success, others for fail
*/
DLLEXPORT int dsmi_get_driver_errorcode(int *errorcount, unsigned int *perrorcode);

/**
* @ingroup driver
* @pcie hot reset
* @attention NULL
* @param [in] device_id  The device id
* @return  0 for success, others for fail
*/
DLLEXPORT int dsmi_pcie_hot_reset(int device_id);

#define MAX_RECORD_ECC_ADDR_COUNT 64

#pragma pack(1)

struct dsmi_multi_ecc_time_data {
    unsigned int multi_record_count;
    unsigned int multi_ecc_times[MAX_RECORD_ECC_ADDR_COUNT];
};

DLLEXPORT int dsmi_get_multi_ecc_time_info(int device_id, struct dsmi_multi_ecc_time_data *multi_ecc_time_data);

/**
* @ingroup driver
* @brief Query hbm ecc record info
* @attention NULL
* @param [in]
* @param [out] record info  ecc_common_data_s
* @return  0 for success, others for fail
* @note Support:Ascend910B
*/

struct dsmi_ecc_common_data {
    unsigned long long physical_addr;
    unsigned int stack_pc_id;
    unsigned int reg_addr_h;
    unsigned int reg_addr_l;
    unsigned int ecc_count;
    int timestamp;
};

#pragma pack()

DLLEXPORT int dsmi_get_multi_ecc_record_info(int device_id, unsigned int *ecc_count, unsigned char read_type,
    unsigned char module_type, struct dsmi_ecc_common_data *ecc_common_data_s);

/**
 * @ingroup driver
 * @brief: get serdes information
 * @param [in] device_id device id
 * @param [in] main_cmd main command type for serdes information
 * @param [in] sub_cmd sub command type for serdes information
 * @param [in out] buf input and output buffer
 * @param [in out] size input buffer size and output data size
 * @return  0 for success, others for fail
 * @note Support:Ascend910B,Ascend910_93
 */

typedef void (*fault_event_callback)(struct dsmi_event *event);

/**
* @ingroup driver
* @brief Get mcu board id 
* @attention NULL
* @param [in] device_id  The device id
* @param [out] mcu_board_id  mcu board id
* @return  0 for success, others for fail
*/
DLLEXPORT int dsmi_product_get_mcu_board_id (unsigned int device_id, unsigned int *mcu_board_id);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* DSMI_COMMON_INTERFACE_CUSTOM_H */