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

#define ELABEL_LOCK_FILE_NAME "/run/elabel_lock_flag"
#define ELABEL_MUTEX_FIRST_TRY_TIMES 100 /* 获取超时锁，首先尝试调用trylock的次数 */
#define ELABEL_MUTEX_SLEEP_TIMES_1MS 1000

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

struct dcmi_elabel_field_8_bytes {
    unsigned short crc;
    unsigned short len;
    unsigned char data[4];
};

struct dcmi_elabel_field_52_bytes {
    unsigned short crc;
    unsigned short len;
    unsigned char data[48];
};

struct dcmi_elabel_field_72_bytes {
    unsigned short crc;
    unsigned short len;
    unsigned char data[68];
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

struct dcmi_elabel_env {
    unsigned char elabel_status;
    struct dcmi_elabel_data *elabel_data;
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

#endif /* __DCMI_ELABEL_OPERATE_H__ */
