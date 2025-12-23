/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_COMMON_H__
#define __DCMI_COMMON_H__
#include <stdbool.h>
#include "dcmi_interface_api.h"

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

#define ENV_PHYSICAL                         1
#define ENV_PHYSICAL_PRIVILEGED_CONTAINER    2
#define ENV_PHYSICAL_PLAIN_CONTAINER         3
#define ENV_VIRTUAL                          4
#define ENV_VIRTUAL_PRIVILEGED_CONTAINER     5
#define ENV_VIRTUAL_PLAIN_CONTAINER          6

#define DCMI_ERROR_CODE_MAX_COUNT        128

#define DCMI_D_CHIP_VENDER_ID     0x19e5
#define DCMI_D_310_DEVICE_ID      0xd100
#define DCMI_D_310B_EP_DEVICE_ID  0xd105
#define DCMI_D_310P_DEVICE_ID     0xd500
#define DCMI_D_910_DEVICE_ID      0xd801
#define DCMI_D_910B_DEVICE_ID     0xd802
#define DCMI_D_910_93_DEVICE_ID   0xd803

#define DCMI_310_CARD_DEVICE_COUNT         4
#define DCMI_310P_1P_CARD_DEVICE_COUNT     1
#define DCMI_310P_2P_CARD_DEVICE_COUNT     2
#define DCMI_310B_EP_CARD_DEVICE_COUNT     1
#define DCMI_910_CARD_DEVICE_COUNT         1
#define DCMI_910_93_CARD_DEVICE_COUNT      2

#define DCMI_INVALID_BOARD_ID          0xFFFF
#define DCMI_INVALID_BOM_ID            0xFFFF
#define DCMI_INVALID_VALUE             (-1)

#define BOARD_CONFIG_FILE   "/run/board_cfg.ini"
#define BOARD_TYPE_KEY      "board_type="
#define BOARD_TYPE_HILENS   "hilens"
#define BOARD_INFO_LINE_LEN 256

#define DCMI_CFG_LINE_MAX_LEN    1024

#define MAX_PCIE_DEVICE_NUM 64
#define MAX_PCIE_INFO_LENTH 512

#define IP_INDEX_0 (0)
#define IP_INDEX_1 (1)
#define IP_INDEX_2 (2)
#define IP_INDEX_3 (3)

#define MINI_MCU_CONNECT       1     /* miniD与MCU之间的IIC通信正常 */
#define MINI_MCU_DISCONNECT    0     /* miniD与MCU之间的IIC通信异常 */
#define MAIN_CHANNEL_MINI2MCU  1     /* miniD与MCU之间的主IIC通道 */
#define SPARE_CHANNEL_MINI2MCU 3     /* miniD与MCU之间的备IIC通道 */
#define NUM_CHANNEL_MINI2MCU   2
#define MAX_RETRY_CNT  5
#define MAX_CHIP_NUM_IN_CARD   4
#define RESULT_LEN_UNIT        2
#define MAX_DATA_LEN           512
#define RESET_WAIT_SECOND      20    /* 带外热复位后等待npu建链时间 20s */
#define MIN_CPLD_VERSION       "3.13"  /* 带外热复位要求CPLD版本号最小值 */
#define CPLD_VERSION_SIZE      5

#define BYTE_TO_KB_TRANS_VALUE  1024
#define KB_TO_MB_TRANS_VALUE    1024

#define DCMI_HEX_TO_STR_BASE        16
#define DCMI_NUMBER_BASE            10

#define DCMI_COMPUTING_TEMPLATE_NAME_LEN 16
#define DCMI_SPLITTING_SPECIFICATIONS_LEN 8

#define MAX_DEVICE_NUM_IN_CARD   16
#define MAX_CARD_NUM_IN_BROAD    MAX_CARD_NUM

#define MAX_DEVICE_NUM           (MAX_DEVICE_NUM_IN_CARD * MAX_CARD_NUM_IN_BROAD)
#ifndef INVALID_DEVICE_ID
#define INVALID_DEVICE_ID        0xFF
#endif

#define DCMI_PCIE_ID_MIN_LEN        12
#define DCMI_DOMAIN_LEN             4
#define DCMI_BUS_ID_LEN             2
#define DCMI_BUS_ID_POS             5
#define DCMI_RC_ID_LEN              2
#define DCMI_RC_ID_POS              8
#define DCMI_FUNC_ID_LEN            1
#define DCMI_FUNC_ID_POS            11

#define DCMI_DOMAIN_SHIFT           16
#define DCMI_BUS_ID_SHIFT           8
#define DCMI_RC_ID_SHIFT            3

#define DCMI_MODEL_PRESENT          1
#define DCMI_MODEL_NOT_PRESENT      0

#define DCMI_SPLIT_NUM              2

#define DCMI_BOARD_ID_LEN           2

#define MAC_ADDR_LEN 6          /* MAC地址长度，固定长度6，单位byte */
#define MAC_ADDR_MAX_COUNT 4    /* MAC地址最大支持数量 */
#define MAC_ADDR_STRING_LEN 17      /* MAC地址长度 */

#define DCMI_INVALID_CARD_ID        (-1)

#define RC_EP_MODE_REG 0x1100CE088
#define RC_EP_MODE_REG_MODE_OFFSET 3
#define RC_EP_MODE_REG_MODE_MASK (0x1 << RC_EP_MODE_REG_MODE_OFFSET)

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

#define MAX_PROC_NUM_IN_DEVICE 32

#define MAX_CHAR_LEN 64

#define DCMI_CPU_NUM_CFG_LEN 16    /* cpu_num_cfg参数长度固定为16字节 */

#define DSMI_MAIN_CMD_EN_DECRYPTION 0x8005
#define DSMI_CERT_SUB_CMD_ENCRYPT   0
#define DSMI_CERT_SUB_CMD_DECRYPT   1
#define SM4_ENCRYPT_KEY_LEN         16
#define SM4_DECRYPT_KEY_LEN         16
#define SM4_ENCRYPT_IV_LEN          16
#define SM4_DECRYPT_IV_LEN          16
#define SM3_OUTPUT_DATA_LEN         32
#define MAX_BUF_SIZE                3296
#define ALG_REV_LEN                 80
#define ALG_KDATA_LEN               3200
#define ALG_DATA_MAX_LEN            3072

#ifdef DT_FLAG
#define DRV_INSTALL_PATH_INFO       "./ascend_install.info"
#else
#define DRV_INSTALL_PATH_INFO       "/etc/ascend_install.info"
#endif

#define COMPAT_FIELD_FW             "compatible_version_fw"
#define COMPAT_FIELD_DRV            "compatible_version_drv"
#define COMPAT_LIST_NAME            "compatible_version_drv"
#define COMPAT_ITEM_SIZE_MIN        3
#define COMPAT_VERSION_START_FW     "6.4."

#define ROOTKEY_HUK_ENABLE                   0xA5A55A5A

#define DEVDRV_HOST_VM_MACH_FLAG          0x1a2b3c4d    /* vm mathine flag */

#define CHECK_ONE_BYTE_BIT        8
#define CHECK_TWO_BYTE_BIT        (2 * CHECK_ONE_BYTE_BIT)
#define CHECK_THREE_BYTE_BIT      (3 * CHECK_ONE_BYTE_BIT)

// traceroute入参信息
#define DEVDRV_IPV6_ARRAY_INT_LEN                   4
#define TRACEROUTE_STATUS_ERROR                     2
#define TRACEROUTE_STATUS_RUNNING                   1
#define TRACEROUTE_STATUS_NOT_RUNNING               0
#define MAX_TRACEROUTE_WAITING_TIMES                600
#define TRACEROUTE_HCCN_CMD                         0
#define TRACEROUTE_DCMI_CMD                         1  // 说明不同宏调用区别
#define DEVDRV_IPV6_ARRAY_LEN                       16

#define UDP_PORT_NUMBER_MIN                 0
#define UDP_PORT_NUMBER_MAX                 65535
#define TRACEROUTE_WAITTIME_MIN             1
#define TRACEROUTE_WAITTIME_MAX             60
#define TRACEROUTE_TTL_MIN                  1
#define TRACEROUTE_TTL_MAX                  255
#define TRACEROUTE_TOS_MIN                  0
#define TRACEROUTE_TOS_MAX                  63
#define TRACEROUTE_TC_MAX                   255
#define TRACEROUTE_DEFAULT_VALUE            (-1)

#define TRUE     1
#define FALSE    0

struct sm_alg_param {
    unsigned int key_type;              // 算法类型
    unsigned int key_len;               // 密钥长度
    unsigned int iv_len;                // iv值长度
    unsigned int data_len;              // 传入明文/密文的长度、传入原数据的长度
    unsigned char reserve[ALG_REV_LEN];          // reserverd, 96-20
    unsigned char alg_data[ALG_KDATA_LEN];       // 传入明文/密文、传出明文/密文
};

struct dcmi_single_solt_pcie_map {
    int slot_id;
    char pcie_info[MAX_PCIE_INFO_LENTH];
};

struct dcmi_slot_pcie_map {
    int pcie_num;
    struct dcmi_single_solt_pcie_map single_slot_pcie_map[MAX_PCIE_DEVICE_NUM];
};

struct dcmi_card_pcie_info {
    struct dcmi_pcie_info_all pcie_info_curr;
    char d_chip_pcie_info[MAX_PCIE_INFO_LENTH];    /* D chip PCIe bdf info */
    char root_pcie_info[MAX_PCIE_INFO_LENTH];      /* bdf information of the previous level of switch */
    char switch_pcie_info[MAX_PCIE_INFO_LENTH];    /* bdf information of PCI switch in card */
};

struct dcmi_device_id_info {
    int device_id_logic;
    int find_device_num;                  // pcie管道内找到的device数量
};

#ifndef _WIN32
/* 新增的内部dsmi接口对应的结构体（未对外暴露），临时定义使用 */
struct dsmi_get_memory_info_stru {
    unsigned long long memory_size;        /* unit:KB */
    unsigned long long memory_available;   /* free + hugepages_free * hugepagesize */
    unsigned int freq;
    unsigned long hugepagesize;             /* unit:KB */
    unsigned long hugepages_total;
    unsigned long hugepages_free;
    unsigned char reserve[32];
};
#endif

struct dcmi_pcie_pre_index {
    int pci_pre_index;
    int switch_pre_index;
};

enum dcmi_pcie_info_index {
    CHIP_PCIE_INFO_INDEX = 0,
    SWITCH_PCIE_INFO_INDEX,
    CARD_PCIE_INFO_INDEX,
};

enum dcmi_mcu_access_type {
    DCMI_MCU_ACCESS_DIRECT = 0,
    DCMI_MCU_ACCESS_BY_NPU,
    DCMI_MCU_ACCESS_INVALID = 0xFF
};

enum dcmi_elabel_access_type {
    DCMI_ELABEL_ACCESS_BY_MCU = 0,
    DCMI_ELABEL_ACCESS_BY_NPU,
    DCMI_ELABEL_ACCESS_BY_CPU,
    DCMI_ELABEL_ACCESS_INVALID = 0xFF
};

enum dcmi_board_id_access_type {
    DCMI_BOARD_ID_ACCESS_BY_MCU = 0,
    DCMI_BOARD_ID_ACCESS_BY_NPU,
    DCMI_BOARD_ID_ACCESS_BY_CPU,
    DCMI_BOARD_ID_ACCESS_INVALID = 0xFF
};

enum dcmi_board_type {
    DCMI_BOARD_TYPE_MODEL = 0,
    DCMI_BOARD_TYPE_SOC,
    DCMI_BOARD_TYPE_CARD,
    DCMI_BOARD_TYPE_SERVER,
    DCMI_BOARD_TYPE_INVALID = 0xFF
};

enum dcmi_board_type_model_sub_type {
    DCMI_BOARD_TYPE_MODEL_BASE = 0,
    DCMI_BOARD_TYPE_MODEL_SEI,
    DCMI_BOARD_TYPE_MODEL_HILENS,
    DCMI_BOARD_TYPE_MODEL_INVALID = 0xFF
};

enum dcmi_board_type_soc_sub_type {
    DCMI_BOARD_TYPE_SOC_BASE = 0,
    DCMI_BOARD_TYPE_SOC_DEVELOP,
    DCMI_BOARD_TYPE_SOC_INVALID = 0xFF
};

enum dcmi_chip_type {
    DCMI_CHIP_TYPE_D310  = 0,
    DCMI_CHIP_TYPE_D310P = 1,
    DCMI_CHIP_TYPE_D910  = 2,
    DCMI_CHIP_TYPE_D310B = 3,
    DCMI_CHIP_TYPE_D910B = 4,
    DCMI_CHIP_TYPE_D910_93 = 5,
    DCMI_CHIP_TYPE_INVALID = 0xFF
};

enum dcmi_product_type {
    DCMI_A200_MODEL_3000 = 0,
    DCMI_A300I_MODEL_3000,
    DCMI_A300I_MODEL_3010,
    DCMI_A300I_PRO,
    DCMI_A300V_PRO,
    DCMI_A300V,
    DCMI_A300I_DUO,
    DCMI_A300I_DUOA,
    DCMI_A300T_MODEL_9000,
    DCMI_A500_MODEL_3000,
    DCMI_A500_MODEL_3010,
    DCMI_A800_SERVER,
    DCMI_A800D_G1,
    DCMI_A800D_G1_CDLS,
    DCMI_A200I_SOC_A1,
    DCMI_A500_A2,
    DCMI_A200_A2_MODEL,
    DCMI_A200_A2_EP,
    DCMI_A900T_POD_A1,
    DCMI_A200T_BOX_A1,
    DCMI_A300T_A1,
    DCMI_A300I_A2,
    DCMI_A900_A3_SUPERPOD,
    DCMI_A200I_PRO,
    DCMI_PRODUCT_TYPE_INVALID = 0xFF
};

enum linux_file_descriptor {
    FD_STD_IN = 0,
    FD_STD_OUT,
    FD_STD_ERR,
    FD_SERVICE_MIN
};

enum dcmi_nve_level {
    DCMI_NVE_LOW,
    DCMI_NVE_MID,
    DCMI_NVE_HIGH,
    DCMI_NVE_FULL,
    DCMI_NVE_LEVEL_INVALID
};

struct dcmi_computing_template {
    char name[DCMI_COMPUTING_TEMPLATE_NAME_LEN];                      // 模板名称
    char splitting_specifications[DCMI_SPLITTING_SPECIFICATIONS_LEN]; // 切分规格
    unsigned int aicore_num;                                          // AICORE 数量
    unsigned int mem_size;                                            // 内存大小
    float aicpu_num;                                                  // AICPU 数量
    float vpc;                                                        // DVPP vpc数量
    float venc;                                                       // DVPP venc数量
    float vdec;                                                       // DVPP vdec数量
    float jpegd;                                                      // DVPP jpegd数量
    float jpege;                                                      // DVPP jpege数量
    float pngd;                                                       // DVPP pngd数量
};

typedef enum {
    MAC_ADDR_OFFSET0 = 0,
    MAC_ADDR_OFFSET1,
    MAC_ADDR_OFFSET2,
    MAC_ADDR_OFFSET3,
    MAC_ADDR_OFFSET4,
    MAC_ADDR_OFFSET5
} MAC_ADDR_LEN_INFO;

#ifndef __TRACEROUTE_INFO
struct dsmi_traceroute_info {
    int max_ttl;
    int tos;
    int waittime;
    int source_port;
    int dest_port;
    unsigned int dip[DEVDRV_IPV6_ARRAY_INT_LEN];
    bool ipv6_flag;
};
#endif
union troute_value {
    int int_value;
};

struct param_value {
    char *operation;
    union troute_value input;
    union troute_value min;
    union troute_value max;
};

struct tag_pcie_idinfo_all {
    unsigned int venderid;          /* 厂商id */
    unsigned int subvenderid;       /* 厂商子id */
    unsigned int deviceid;          /* 设备id */
    unsigned int subdeviceid;       /* 设备子id */
    int domain;                     /* pcie域 */
    unsigned int bdf_busid;         /* 总线号 */
    unsigned int bdf_deviceid;      /* 设备物理号 */
    unsigned int bdf_funcid;        /* 设备功能号 */
    unsigned char reserve[32];
};

unsigned short crc16(unsigned char *buffer, unsigned int length);

int dcmi_gen_rand_num(void);

int safe_exec(const char *cmd_path, char *cmd[]);

int dcmi_file_realpath_disallow_nonexist(const char *file, char *path, int path_len);

int dcmi_file_realpath_allow_nonexist(const char *file, char *path, int path_len);

int dcmi_get_file_length(const char *file, unsigned int *file_len);

int dcmi_check_file_path(const char *tmp_path);

#ifndef _WIN32
/* 新增的内部dsmi接口（未对外暴露），临时定义使用 */
int dsmi_get_memory_info_v2(int device_id, struct dsmi_get_memory_info_stru *pdevice_memory_info);
int dsmi_get_pcie_info_v2(int device_id, struct tag_pcie_idinfo_all *pcie_id_info);
int dsmi_hot_reset_atomic(int device_id, int dsmi_hotreset_subcmd);
int dsmi_get_work_mode(unsigned int *work_mode);
int dsmi_set_prbs_flag(int logic_id, int prbs_flag);
int dsmi_prbs_adapt_in_order(unsigned int mode, unsigned int logic_id, unsigned char master_flag);
int dsmi_set_serdes_info(unsigned int device_id, unsigned int main_cmd, unsigned int sub_cmd, void* buf,
                         unsigned int size);
int dsmi_get_rootkey_status(unsigned int device_id, unsigned int key_type, unsigned int *rootkey_status);
int dsmi_get_mainboard_id(unsigned int device_id, unsigned int *mainboard_id);
int dsmi_get_serdes_info(unsigned int device_id, unsigned int main_cmd, unsigned int sub_cmd,
    void *buf, unsigned int *size);
int dsmi_get_hbm_manufacturer_id(unsigned int device_id, unsigned int *manufacturer_id);
#ifdef SOC
inline int dsmi_get_hbm_manufacturer_id(unsigned int device_id, unsigned int *manufacturer_id)
{
    return NPU_ERR_CODE_NOT_SUPPORT;
}
else
int dsmi_get_hbm_manufacturer_id(unsigned int device_id, unsigned int *manufacturer_id);
#endif  /* SOC */

#endif  /* _WIN32 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DCMI_COMMON_H__ */
