/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DSMI_INNER_INTERFACE_H__
#define __DSMI_INNER_INTERFACE_H__

#define MAX_LANE 8
typedef enum {
    DSMI_STATE_SUB_CMD_ENCRYPT,
    DSMI_STATE_SUB_CMD_DECRYPT,
    DSMI_STATE_SUB_CMD_MAX,
} DSMI_STATE_SUB_CMD;

struct dsmi_get_memory_info_stru {
    unsigned long long memory_size;        /* unit:KB */
    unsigned long long memory_available;   /* free + hugepages_free * hugepagesize */
    unsigned int freq;
    unsigned long hugepagesize;             /* unit:KB */
    unsigned long hugepages_total;
    unsigned long hugepages_free;
    unsigned char reserve[32];
};

struct tag_pcie_idinfo_all {
    unsigned int venderid;    /* 厂商id */
    unsigned int subvenderid; /* 厂商子id */
    unsigned int deviceid;    /* 设备id */
    unsigned int subdeviceid; /* 设备子id */
    int domain;               /* pcie域 */
    unsigned int bdf_busid;         /* 总线号 */
    unsigned int bdf_deviceid;      /* 设备物理号 */
    unsigned int bdf_funcid;          /* 设备功能号 */
    unsigned char reserve[32];
};

typedef enum {
    DSMI_SERDES_CMD_PRBS = 0,
    DSMI_SERDES_CMD_MAX,
} SERDES_MAIN_CMD;
 
typedef enum {
    DSMI_SERDES_PRBS_SUB_CMD_SET_TYPE,
    DSMI_SERDES_PRBS_SUB_CMD_GET_RESULT,
    DSMI_SERDES_PRBS_SUB_CMD_GET_STATUS,
    DSMI_SERDES_PRBS_SUB_CMD_MAX,
}DSMI_SERDES_PRBS_SUB_CMD;
 
typedef struct {
    unsigned int serdes_prbs_macro_id;
    unsigned int serdes_prbs_start_lane_id;
    unsigned int serdes_prbs_lane_count;
} DSMI_SERDES_PRBS_PARAM_BASE;
 
typedef struct {
    unsigned int check_en;
    unsigned int check_type;
    unsigned int error_status;
    unsigned int error_cnt;
    unsigned long error_rate;
    unsigned int alos_status;
    unsigned long time_val;
} SERDES_PRBS_LANE_RESULT;
 
typedef struct {
    unsigned int lane_prbs_tx_status;
    unsigned int lane_prbs_rx_status;
} SERDES_PRBS_LANE_STATUS;
 
typedef struct {
    union {
        SERDES_PRBS_LANE_RESULT result[MAX_LANE];
        SERDES_PRBS_LANE_STATUS lane_status[MAX_LANE];
    };
} DSMI_SERDES_PRBS_GET_OPERATE;
 
typedef struct {
    DSMI_SERDES_PRBS_PARAM_BASE dsmi_serdes_base;
    unsigned int serdes_prbs_type;
    unsigned int serdes_prbs_direction;
} DSMI_SERDES_PRBS_SET_PARAM;
 
typedef struct {
    union {
        DSMI_SERDES_PRBS_SET_PARAM set_param;
        DSMI_SERDES_PRBS_PARAM_BASE get_param;
    };
} DSMI_PRBS_OPERATE_PARAM;

#pragma pack(1)
/* Force single byte alignment to return to out of band fetch */

enum ECC_INFO_READ {
    MULTI_ECC_TIMES_READ = 0,
    SINGLE_ECC_INFO_READ,
    MULTI_ECC_INFO_READ,
    ECC_ADDRESS_COUNT_READ,
    ECC_MAX_READ_CMD
};

#pragma pack()

/**
* @ingroup driver
* @brief Get memory information v2
* @attention NULL
* @param [in] device_id  The device id
* @param [out] pdevice_memory_info  Return memory information
* @return  0 for success, others for fail
* @note Support:Ascend310,Ascend310B,Ascend910,Ascend910B,Ascend610,Ascend310P
*/
int dsmi_get_memory_info_v2(int device_id, struct dsmi_get_memory_info_stru *pdevice_memory_info);

/**
* @ingroup driver
* @brief Get HBM manufacturer id
* @attention NULL
* @param [in] device_id  The device id
* @param [out] manufacturer_id  Return manufacturer id
* @return  0 for success, others for fail
* @note Support:Ascend910B
*/
int dsmi_get_hbm_manufacturer_id(unsigned int device_id, unsigned int *manufacturer_id);

/**
* @ingroup driver
* @brief Get the burning status of the root key.
* @attention NULL
* @param [in] device_id  The device id
* @param [in] key_type  The key type
* @param [out] rootkey_status  Return status
* @return  0 for success, others for fail
* @note Support:Ascend910B
*/
int dsmi_get_rootkey_status(unsigned int device_id, unsigned int key_type, unsigned int *rootkey_status);

/**
* @ingroup driver
* @brief Get HCCS status.
* @attention NULL
* @param [in] device_id1, device_id2  The device id
* @param [out] hccs_status  Return HCCS status, HCCS_OFF(0) for HCCS interconnection status,
    HCCS_ON(1) for HCCS disconnection status
* @return  0 for success, others for fail
* @note Support:Ascend910B
*/
int dsmi_get_hccs_status(unsigned int device_id1, unsigned int device_id2, int *hccs_status);

/**
* @ingroup driver
* @brief Query PCIe device information v2
* @attention NULL
* @param [in] device_id  The device id
* @param [out] pcie_idinfo  PCIe device information
* @return  0 for success, others for fail
* @note Support:Ascend310,Ascend910,Ascend910B,Ascend310P
*/
int dsmi_get_pcie_info_v2(int device_id, struct tag_pcie_idinfo_all *pcie_idinfo);

/**
* @ingroup driver
* @brief Query Device Work Mode
* @attention NULL
* @param [in]
* @param [out] work_mode  Device_Work_Mode
* @return  0 for success, others for fail
* @note Support:Ascend310,Ascend910,Ascend910B,Ascend310P
*/
int dsmi_get_work_mode(unsigned int *work_mode);

/**
* @ingroup driver
* @brief Query Main Board ID
* @attention NULL
* @param [in] device_id The device id
* @param [out] mainboard_id  Main Board Id
* @return  0 for success, others for fail
* @note Support:Ascend310,Ascend910,Ascend910B,Ascend310P
*/
int dsmi_get_mainboard_id(unsigned int device_id, unsigned int *mainboard_id);
 
/**
* @ingroup driver
* @brief Query hbm ecc time info
* @attention NULL
* @param [in]
* @param [out] time info  multi_ecc_time_data
* @return  0 for success, others for fail
* @note Support:Ascend910B
*/

int dsmi_get_serdes_info(unsigned int device_id, SERDES_MAIN_CMD main_cmd, unsigned int sub_cmd,
    void *buf, unsigned int *size);

/**
 * @ingroup driver
 * @brief: set serdes information
 * @param [in] device_id device id
 * @param [in] main_cmd main command type for serdes information
 * @param [in] sub_cmd sub command type for serdes information
 * @param [in out] buf input and output buffer
 * @param [in out] size input buffer size and output data size
 * @return  0 for success, others for fail
 * @note Support:Ascend910B,Ascend910_93
 */
int dsmi_set_serdes_info(unsigned int device_id, SERDES_MAIN_CMD main_cmd, unsigned int sub_cmd,
    void *buf, unsigned int size);
#endif
