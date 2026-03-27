/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_ELABEL_OPERATE_H__
#define __DCMI_ELABEL_OPERATE_H__

#define ELABEL_OFFSET 128
#define ELABEL_310B_OFFSET 0x0200
#define ELABEL_TOTAL_SIZE 4096  // 4k

#define MAC_MAX_LEN 64
#define MAC_HEAD_LEN 32
#define MAC_SIZE 8

#define MAC_VERSION 11
#define MAC_MAGIC 11
#define MAC_FLAG0 32
#define MAC_FLAG1 64

#define FRU_EXTERN_LABEL_AREA_LEN 268

#define DCMI_ELABEL_LOCK_TIMEOUT 1000

#define ELABEL_MAX_LEN_910_95 512

// 用户态输入，只计算数据段长度
#define HEAD_LENGTH                    (24 - 4 -4)
#define NPU_BOARD_TYPE_LENGTH          (8 - 4)
#define MAIN_BOARD_ID_LENGTH           (8 - 4)
#define BOARD_PRODUCT_NAME_LENGTH      (68 - 4)
#define BOARD_MODEL_LENGTH             (68 - 4)
#define BOARD_MANUFACTURER_LENGTH      (68 - 4)
#define BOARD_SERIAL_NUMBER_LENGTH     (68 - 4)
#define BOARD_ID_LENGTH                (8 - 4)
#define PCB_ID_BOARD_HARD_VER_LENGTH   (8 - 4)
#define BOM_ID_LENGTH                  (8 - 4)
#define DESCRIPTION_LENGTH             (272 - 4)
#define MANUFACTURED_LENGTH            (36 - 4)
#define CLEI_CODE_LENGTH               (36 - 4)
#define BOM_LENGTH                     (68 - 4)
#define PRODUCT_NAME_LENGTH            (68 - 4)
#define PRODUCT_MANUFACTURE_LENGTH     (68 - 4)
#define PRODUCT_SERIAL_NUMBER_LENGTH   (68 - 4)
#define PRODUCT_PART_LENGTH            (68 - 4)
#define ITEM_LENGTH                    (34 - 4)
#define ISSUENUMBER_LENGTH             (24 - 4)
#define EXPAND_INFO_LENGTH             (266 - 4)

#define ELABEL_LOCK_FILE_NAME "/run/elabel_lock_flag"
#define ELABEL_MUTEX_FIRST_TRY_TIMES 100 /* 获取超时锁，首先尝试调用trylock的次数 */
#define ELABEL_MUTEX_SLEEP_TIMES_1MS 1000

enum elabel_item_type_910_95 {
    ELABEL_ITEM_ID_910_95_HEAD = 0x65,                    /* 电子标签版本号 */
    ELABEL_ITEM_ID_910_95_NPU_BOARD_TYPE = 0x66,          /* NPU分bin使用的ID */
    ELABEL_ITEM_ID_910_95_MAIN_BOARD_ID = 0x67,           /* 主板ID，AO区分标卡/模组 */
    ELABEL_ITEM_ID_910_95_BOARD_PRODUCT_NAME = 0x22,      /* 单板名称，继承老命令字 */
    ELABEL_ITEM_ID_910_95_BOARD_MODEL = 0x69,             /* 单板型号 */
    ELABEL_ITEM_ID_910_95_BOARD_MANUFACTURER = 0x6a,      /* 单板生产厂家 */
    ELABEL_ITEM_ID_910_95_BOARD_SERIAL_NUMBER = 0x23,     /* 单板序列号，继承 */
    ELABEL_ITEM_ID_910_95_BOARD_ID = 0x6c,                /* 单板ID，区分VRD固件 */
    ELABEL_ITEM_ID_910_95_PCB_ID_BOARD_HARD_VER = 0x6d,   /* PCB版本编号 */
    ELABEL_ITEM_ID_910_95_BOM_ID = 0x6e,                  /* BOM版本编号，区分VRD固件 */
    ELABEL_ITEM_ID_910_95_DESCRIPTION = 0x6f,             /* 单板描述 */
    ELABEL_ITEM_ID_910_95_MANUFACTURED = 0x21,            /* 生产日期，继承 */
    ELABEL_ITEM_ID_910_95_CLEI_CODE = 0x71,               /* 预留，遵循北美规范 */
    ELABEL_ITEM_ID_910_95_BOM = 0x72,                     /* 更精细粒度的item，销售编码 */
    ELABEL_ITEM_ID_910_95_PRODUCT_NAME = 0x31,            /* 产品名称，继承 */
    ELABEL_ITEM_ID_910_95_PRODUCT_MANUFACTURER = 0x74,    /* 产品厂家 */
    ELABEL_ITEM_ID_910_95_PRODUCT_SERIAL_NUMBER = 0x75,   /* 产品序列号 */
    ELABEL_ITEM_ID_910_95_PRODUCT_PART = 0x76,            /* 产品编号 */
    ELABEL_ITEM_ID_910_95_ITEM = 0x77,                    /* 预留 */
    ELABEL_ITEM_ID_910_95_ISSUENUMBER = 0x78,             /* 预留 */
    ELABEL_ITEM_ID_910_95_EXPAND_INFO = 0x79,             /* 扩展域段 */
    ELABEL_ITEM_ID_910_95_INVALID,                        /* 非法值 */
};

enum elabel_ret {
    ELABEL_RET_OK = 0,
    ELABEL_RET_FLASH_ERR = -1,
    ELABEL_RET_MEM_ERR = -4,
    ELABEL_RET_ARG_INVALID = -6,
    ELABEL_RET_BUSY = -8,
};

enum elabel_item_id {
    ELABEL_ITEM_ID_CHASSIS_TYPE = 0x10,
    ELABEL_ITEM_ID_CHASSIS_PN = 0x11,
    ELABEL_ITEM_ID_CHASSIS_SN = 0x12,
    ELABEL_ITEM_ID_MFG_DATE = 0x20,
    ELABEL_ITEM_ID_BOARD_MANF = 0x21,
    ELABEL_ITEM_ID_BOARD_PRD_NAME = 0x22,
    ELABEL_ITEM_ID_BOARD_SN = 0x23,
    ELABEL_ITEM_ID_BOARD_PN = 0x24,
    ELABEL_ITEM_ID_BOARD_FRU_FILE = 0x25,
    ELABEL_ITEM_ID_PRD_MANF = 0x30,
    ELABEL_ITEM_ID_PRD_NAME = 0x31,
    ELABEL_ITEM_ID_PRD_PN = 0x32,
    ELABEL_ITEM_ID_PRD_VER = 0x33,
    ELABEL_ITEM_ID_PRD_SN = 0x34,
    ELABEL_ITEM_ID_ASSET_TAG = 0x35,
    ELABEL_ITEM_ID_PRD_FRU_FILE = 0x36,
    ELABEL_ITEM_ID_SYS_MANF = 0x60,
    ELABEL_ITEM_ID_SYS_PRD_NAME = 0x61,
    ELABEL_ITEM_ID_SYS_VER = 0x62,
    ELABEL_ITEM_ID_SYS_SN = 0x63,
    ELABEL_ITEM_DESCRIPTION = 0x64,
    ELABEL_ITEM_ISSUE_NUMBER = 0x65,
    ELABEL_ITEM_CLEI_CODE = 0x66,
    ELABEL_ITEM_BOM = 0x67,
    ELABEL_ITEM_MODEL = 0x68,
    ELABEL_ITEM_ID_EXTEND = 0x50
};

struct elabel_data_info_910_95 {
    unsigned char op_code;
    unsigned int elabel_length;
};

struct dcmi_elabel_item {
    unsigned char item_id;
    unsigned short item_size;
    unsigned short max_len;
    unsigned long offset;
    unsigned char *def_value;
};

struct dcmi_elabel_field_bytes {
    unsigned short crc;
    unsigned short len;
    unsigned char data[0];
};

struct dcmi_elabel_field_head_info {
    unsigned short crc;
    unsigned short len;
    unsigned char data[16];
    unsigned int data_len;
};

struct dcmi_elabel_field_8_bytes {
    unsigned short crc;
    unsigned short len;
    unsigned char data[4];
};

struct dcmi_elabel_field_24_bytes {
    unsigned short crc;
    unsigned short len;
    unsigned char data[20];
};

struct dcmi_elabel_field_34_bytes {
    unsigned short crc;
    unsigned short len;
    unsigned char data[30];
};

struct dcmi_elabel_field_36_bytes {
    unsigned short crc;
    unsigned short len;
    unsigned char data[32];
};

struct dcmi_elabel_field_52_bytes {
    unsigned short crc;
    unsigned short len;
    unsigned char data[48];
};

struct dcmi_elabel_field_68_bytes {
    unsigned short crc;
    unsigned short len;
    unsigned char data[64];
};

struct dcmi_elabel_field_72_bytes {
    unsigned short crc;
    unsigned short len;
    unsigned char data[68];
};

struct dcmi_elabel_field_266_bytes {
    unsigned short crc;
    unsigned short len;
    unsigned char data[262];
};

struct dcmi_elabel_field_272_bytes {
    unsigned short crc;
    unsigned short len;
    unsigned char data[268];
};

struct dcmi_elabel_field_500_bytes {
    unsigned short crc;
    unsigned short len;
    unsigned char data[496];
};

struct dcmi_elabel_extend_area {
    unsigned short crc;
    unsigned short len;
    unsigned char data[FRU_EXTERN_LABEL_AREA_LEN];
};

struct dcmi_elabel_data {
    unsigned int magic_num;
    struct dcmi_elabel_field_8_bytes chassis_type;
    struct dcmi_elabel_field_72_bytes chassis_part_num;
    struct dcmi_elabel_field_72_bytes chassis_serial_num;
    struct dcmi_elabel_field_72_bytes bd_mfg_time;
    struct dcmi_elabel_field_72_bytes bd_manufacture;
    struct dcmi_elabel_field_72_bytes bd_product_name;
    struct dcmi_elabel_field_72_bytes bd_serial_num;
    struct dcmi_elabel_field_72_bytes bd_part_num;
    struct dcmi_elabel_field_72_bytes bd_file_id;

    struct dcmi_elabel_field_72_bytes prd_manufacture;
    struct dcmi_elabel_field_72_bytes prd_name;
    struct dcmi_elabel_field_72_bytes prd_part_num;
    struct dcmi_elabel_field_72_bytes prd_version;
    struct dcmi_elabel_field_72_bytes prd_serial_num;
    struct dcmi_elabel_field_72_bytes prd_asset_tag;
    struct dcmi_elabel_field_72_bytes prd_file_id;

    struct dcmi_elabel_field_72_bytes sys_manufacture;
    struct dcmi_elabel_field_72_bytes sys_name;
    struct dcmi_elabel_field_72_bytes sys_version;
    struct dcmi_elabel_field_72_bytes sys_serial_num;

    struct dcmi_elabel_field_500_bytes sys_description;
    struct dcmi_elabel_field_52_bytes sys_issuenumber;
    struct dcmi_elabel_field_52_bytes sys_cleicode;
    struct dcmi_elabel_field_52_bytes sys_bom;
    struct dcmi_elabel_field_52_bytes sys_model;

    struct dcmi_elabel_extend_area extend_info;
};

struct dcmi_elabel_data_v2 {
    struct dcmi_elabel_field_head_info head;
    struct dcmi_elabel_field_8_bytes npu_board_type;
    struct dcmi_elabel_field_8_bytes main_board_id;
    struct dcmi_elabel_field_68_bytes board_product_name;
    struct dcmi_elabel_field_68_bytes board_model;
    struct dcmi_elabel_field_68_bytes board_manufacture;
    struct dcmi_elabel_field_68_bytes board_serial_num;
    struct dcmi_elabel_field_8_bytes board_id;
    struct dcmi_elabel_field_8_bytes pcb_id_board_hard_ver;
    struct dcmi_elabel_field_8_bytes bom_id;
    struct dcmi_elabel_field_272_bytes description;
    struct dcmi_elabel_field_36_bytes manufactured;
    struct dcmi_elabel_field_36_bytes cleicode;
    struct dcmi_elabel_field_68_bytes bom;
    struct dcmi_elabel_field_68_bytes product_product_name;
    struct dcmi_elabel_field_68_bytes product_manufacture;
    struct dcmi_elabel_field_68_bytes product_serial_num;
    struct dcmi_elabel_field_68_bytes product_part;
    struct dcmi_elabel_field_34_bytes item;
    struct dcmi_elabel_field_24_bytes issue_num;
    struct dcmi_elabel_field_266_bytes reserved;
};

struct dcmi_elabel_env {
    unsigned char elabel_status;
    struct dcmi_elabel_data *elabel_data;
};

static const struct elabel_data_info_910_95 g_elabel_data[] = {
    {ELABEL_ITEM_ID_910_95_HEAD,                        HEAD_LENGTH},
    {ELABEL_ITEM_ID_910_95_NPU_BOARD_TYPE,              NPU_BOARD_TYPE_LENGTH},
    {ELABEL_ITEM_ID_910_95_MAIN_BOARD_ID,               MAIN_BOARD_ID_LENGTH},
    {ELABEL_ITEM_ID_910_95_BOARD_PRODUCT_NAME,          BOARD_PRODUCT_NAME_LENGTH},
    {ELABEL_ITEM_ID_910_95_BOARD_MODEL,                 BOARD_MODEL_LENGTH},
    {ELABEL_ITEM_ID_910_95_BOARD_MANUFACTURER,          BOARD_MANUFACTURER_LENGTH},
    {ELABEL_ITEM_ID_910_95_BOARD_SERIAL_NUMBER,         BOARD_SERIAL_NUMBER_LENGTH},
    {ELABEL_ITEM_ID_910_95_BOARD_ID,                    BOARD_ID_LENGTH},
    {ELABEL_ITEM_ID_910_95_PCB_ID_BOARD_HARD_VER,       PCB_ID_BOARD_HARD_VER_LENGTH},
    {ELABEL_ITEM_ID_910_95_BOM_ID,                      BOM_ID_LENGTH},
    {ELABEL_ITEM_ID_910_95_DESCRIPTION,                 DESCRIPTION_LENGTH},
    {ELABEL_ITEM_ID_910_95_MANUFACTURED,                MANUFACTURED_LENGTH},
    {ELABEL_ITEM_ID_910_95_CLEI_CODE,                   CLEI_CODE_LENGTH},
    {ELABEL_ITEM_ID_910_95_BOM,                         BOM_LENGTH},
    {ELABEL_ITEM_ID_910_95_PRODUCT_NAME,                PRODUCT_NAME_LENGTH},
    {ELABEL_ITEM_ID_910_95_PRODUCT_MANUFACTURER,        PRODUCT_MANUFACTURE_LENGTH},
    {ELABEL_ITEM_ID_910_95_PRODUCT_SERIAL_NUMBER,       PRODUCT_SERIAL_NUMBER_LENGTH},
    {ELABEL_ITEM_ID_910_95_PRODUCT_PART,                PRODUCT_PART_LENGTH},
    {ELABEL_ITEM_ID_910_95_ITEM,                        ITEM_LENGTH},
    {ELABEL_ITEM_ID_910_95_ISSUENUMBER,                 ISSUENUMBER_LENGTH},
    {ELABEL_ITEM_ID_910_95_EXPAND_INFO,                 EXPAND_INFO_LENGTH},
};

static const struct dcmi_elabel_item elabel_items[] = {
    {ELABEL_ITEM_ID_CHASSIS_TYPE,
        (unsigned char)sizeof(struct dcmi_elabel_field_8_bytes),
        4,
        (unsigned long)&((struct dcmi_elabel_data *)0)->chassis_type,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_CHASSIS_PN,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->chassis_part_num,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_CHASSIS_SN,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->chassis_serial_num,
        (unsigned char *)("")},

    {ELABEL_ITEM_ID_MFG_DATE,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->bd_mfg_time,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_BOARD_MANF,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->bd_manufacture,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_BOARD_PRD_NAME,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->bd_product_name,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_BOARD_SN,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->bd_serial_num,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_BOARD_PN,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->bd_part_num,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_BOARD_FRU_FILE,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->bd_file_id,
        (unsigned char *)("")},

    {ELABEL_ITEM_ID_PRD_MANF,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->prd_manufacture,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_PRD_NAME,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->prd_name,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_PRD_PN,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->prd_part_num,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_PRD_VER,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->prd_version,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_PRD_SN,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->prd_serial_num,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_ASSET_TAG,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->prd_asset_tag,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_PRD_FRU_FILE,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->prd_file_id,
        (unsigned char *)("")},

    {ELABEL_ITEM_ID_SYS_MANF,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->sys_manufacture,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_SYS_PRD_NAME,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->sys_name,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_SYS_VER,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->sys_version,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_SYS_SN,
        (unsigned char)sizeof(struct dcmi_elabel_field_72_bytes),
        68,
        (unsigned long)&((struct dcmi_elabel_data *)0)->sys_serial_num,
        (unsigned char *)("")},

    {ELABEL_ITEM_DESCRIPTION,
        (unsigned char)sizeof(struct dcmi_elabel_field_500_bytes),
        496,
        (unsigned long)&((struct dcmi_elabel_data *)0)->sys_description,
        (unsigned char *)("")},
    {ELABEL_ITEM_ISSUE_NUMBER,
        (unsigned char)sizeof(struct dcmi_elabel_field_52_bytes),
        48,
        (unsigned long)&((struct dcmi_elabel_data *)0)->sys_issuenumber,
        (unsigned char *)("")},
    {ELABEL_ITEM_CLEI_CODE,
        (unsigned char)sizeof(struct dcmi_elabel_field_52_bytes),
        48,
        (unsigned long)&((struct dcmi_elabel_data *)0)->sys_cleicode,
        (unsigned char *)("")},
    {ELABEL_ITEM_BOM,
        (unsigned char)sizeof(struct dcmi_elabel_field_52_bytes),
        48,
        (unsigned long)&((struct dcmi_elabel_data *)0)->sys_bom,
        (unsigned char *)("")},
    {ELABEL_ITEM_MODEL,
        (unsigned char)sizeof(struct dcmi_elabel_field_52_bytes),
        48,
        (unsigned long)&((struct dcmi_elabel_data *)0)->sys_model,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_EXTEND,
        (unsigned char)sizeof(struct dcmi_elabel_extend_area),
        268,
        (unsigned long)&((struct dcmi_elabel_data *)0)->extend_info,
        (unsigned char *)("")}
};

static const struct dcmi_elabel_item elabel_items_v2[] = {
    {ELABEL_ITEM_ID_910_95_HEAD,
        (unsigned char)sizeof(struct dcmi_elabel_field_head_info),
        16,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->head,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_NPU_BOARD_TYPE,
        (unsigned char)sizeof(struct dcmi_elabel_field_8_bytes),
        4,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->npu_board_type,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_MAIN_BOARD_ID,
        (unsigned char)sizeof(struct dcmi_elabel_field_8_bytes),
        4,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->main_board_id,
        (unsigned char *)("")},

    {ELABEL_ITEM_ID_910_95_BOARD_PRODUCT_NAME,
        (unsigned char)sizeof(struct dcmi_elabel_field_68_bytes),
        64,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->board_product_name,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_BOARD_MODEL,
        (unsigned char)sizeof(struct dcmi_elabel_field_68_bytes),
        64,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->board_model,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_BOARD_MANUFACTURER,
        (unsigned char)sizeof(struct dcmi_elabel_field_68_bytes),
        64,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->board_manufacture,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_BOARD_SERIAL_NUMBER,
        (unsigned char)sizeof(struct dcmi_elabel_field_68_bytes),
        64,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->board_serial_num,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_BOARD_ID,
        (unsigned char)sizeof(struct dcmi_elabel_field_8_bytes),
        4,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->board_id,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_PCB_ID_BOARD_HARD_VER,
        (unsigned char)sizeof(struct dcmi_elabel_field_8_bytes),
        4,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->pcb_id_board_hard_ver,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_BOM_ID,
        (unsigned char)sizeof(struct dcmi_elabel_field_8_bytes),
        4,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->bom_id,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_DESCRIPTION,
        (unsigned char)sizeof(struct dcmi_elabel_field_272_bytes),
        268,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->description,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_MANUFACTURED,
        (unsigned char)sizeof(struct dcmi_elabel_field_36_bytes),
        32,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->manufactured,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_CLEI_CODE,
        (unsigned char)sizeof(struct dcmi_elabel_field_36_bytes),
        32,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->cleicode,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_BOM,
        (unsigned char)sizeof(struct dcmi_elabel_field_68_bytes),
        64,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->bom,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_PRODUCT_NAME,
        (unsigned char)sizeof(struct dcmi_elabel_field_68_bytes),
        64,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->product_product_name,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_PRODUCT_MANUFACTURER,
        (unsigned char)sizeof(struct dcmi_elabel_field_68_bytes),
        64,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->product_manufacture,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_PRODUCT_SERIAL_NUMBER,
        (unsigned char)sizeof(struct dcmi_elabel_field_68_bytes),
        64,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->product_serial_num,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_PRODUCT_PART,
        (unsigned char)sizeof(struct dcmi_elabel_field_68_bytes),
        64,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->product_part,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_ITEM,
        (unsigned char)sizeof(struct dcmi_elabel_field_34_bytes),
        30,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->item,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_ISSUENUMBER,
        (unsigned char)sizeof(struct dcmi_elabel_field_24_bytes),
        20,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->issue_num,
        (unsigned char *)("")},
    {ELABEL_ITEM_ID_910_95_EXPAND_INFO,
        (unsigned char)sizeof(struct dcmi_elabel_field_266_bytes),
        262,
        (unsigned long)&((struct dcmi_elabel_data_v2 *)0)->reserved,
        (unsigned char *)("")}
};

#define ELABEL_STATUS_RAM_HAS_DATA   (1 << 0)
#define ELABEL_STATUS_FLASH_HAS_DATA (1 << 1)
#define ELABEL_STATUS_FLASH_WR_ERR   (1 << 2)
#define ELABEL_STATUS_FLASH_RD_ERR   (1 << 3)

#define ELABEL_MAGIC_NUMBEER       0x454C4142  //  0x454C4142  = "ELAB"
#define ELABEL_FLASH_MAX_RD_TIMES  5
#define ELABEL_FLASH_MAX_INT_TIMES 3

void dcmi_set_i2c_dev_name(const char *channel_num);
int dcmi_elabel_update(void);
int dcmi_elabel_clear(void);
int dcmi_elabel_get_data(unsigned char item_id, unsigned char *data, unsigned short data_size,
                         unsigned short *data_len);
int dcmi_elabel_set_data(unsigned char item_id, unsigned char *data, unsigned short offset, unsigned char len);

int dcmi_cpu_get_device_elabel_info(int card_id, struct dcmi_elabel_info *elabel_info);

void dcmi_set_default_elabel_str(char *elabel, int elabel_size);

int dcmi_ao_get_elabel_info(int card_id, int device_id, unsigned char item_id, char *data, unsigned short data_size);

#endif /* __DCMI_ELABEL_OPERATE_H__ */
