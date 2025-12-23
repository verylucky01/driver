/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_INNER_CFG_PERSIST_H__
#define __DCMI_INNER_CFG_PERSIST_H__

#define DCMI_CFG_GET_LOCK_TIMEOUT         5000
#define DCMI_CFG_MUTEX_FIRST_TRY_TIMES   100 /* 获取超时锁，首先尝试调用trylock的次数 */
#define DCMI_CFG_MUTEX_SLEEP_TIMES_1MS   1000
#define DCMI_CFG_CMD_LINE_LEN            20
#define DCMI_CFG_INSERT_OK               0x55
#define DCMI_CFG_NOT_INSERT              0xAA
#define DCMI_CFG_INSERT_COMPLETE         1
#define DCMI_CFG_DELETE_COMPLETE         1
#define DCMI_CFG_CMDLINE_FOUND           0x66
#define DCMI_CFG_CMDLINE_NOT_FOUND       0xBB
#define DCMI_CFG_RECOVER_ENABLE          0x01
#define DCMI_CFG_RECOVER_DISABLE         0x00
#define DCMI_CFG_PERSISTENCE_ENABLE      0x01
#define DCMI_CFG_PERSISTENCE_DISABLE     0x00
#define DCMI_EXIST_VNPU_CFG              0x01
#define DCMI_NO_EXIST_VNPU_CFG           0x00

#define DCMI_VNPU_CONF_DEFAULT_COMMENT   ("vnpu_config_recover:enable\n[vnpu-config start]\n[vnpu-config end]\n")
#define DCMI_VNPU_CONF                   "/etc/vnpu.cfg"
#define DCMI_VNPU_CONF_BAK               "/etc/vnpu.cfg.bak"
#define DCMI_VNPU_CONF_MAX_SIZE          (0x400000) // 文件最大4M,限制申请空间大小
#define DCMI_VNPU_CONF_ONE_LINE_MAX_LEN  256
#define DCMI_VNPU_FLAG_NOT_FIND          0
#define DCMI_CFG_VNPU_LOCK_DIR           "/run/vnpu_cfg_lock"
#define DCMI_CFG_VNPU_LOCK_FILE_NAME     "/run/vnpu_cfg_lock/vnpu_cfg_lock_flag"

#define DCMI_SYSLOG_CONF                     "/etc/npu_dev_syslog.cfg"
#define DCMI_SYSLOG_CONF_BAK                 "/etc/npu_dev_syslog.cfg.bak"
#define DCMI_SYSLOG_CONF_MAX_SIZE            (0x400000)
#define DCMI_SYSLOG_CONF_ONE_LINE_MAX_LEN    512
#define DCMI_SYSLOG_CONF_DISABLE_COMMENT     ("syslog_persistence_config_mode:disable\n")
#define DCMI_SYSLOG_CONF_ENABLE_COMMENT      ("syslog_persistence_config_mode:enable\n")
#define DCMI_SYSLOG_CONF_MAX_LINE            2
#define DCMI_SYSLOG_CONF_MIN_LINE            0
#define DCMI_SYSLOG_DUMP_MAX_GEAR            10
#define DCMI_SYSLOG_DUMP_MIN_GEAR            1
#define DCMI_CFG_SYSLOG_LOCK_FILE_NAME       "/run/npu_dev_syslog_cfg_lock/syslog_cfg_lock_flag"
#define DCMI_CFG_SYSLOG_NPU_SMI_LOCK_FILE_NAME "/run/npu_dev_syslog_cfg_lock/syslog_cfg_npu_smi_lock_flag"
#define DCMI_CFG_SYSLOG_LOCK_DIR              "/run/npu_dev_syslog_cfg_lock"
#define DCMI_ERR_CODE_SYSLOG_CONFIG_ILLEGAL  10

#define DCMI_CUSTOM_OP_CONF                         "/etc/custom_op.cfg"
#define DCMI_CUSTOM_OP_CONF_BAK                     "/etc/custom_op.cfg.bak"
#define DCMI_CUSTOM_OP_CONF_MAX_SIZE                (0x400000) // 文件最大4M,限制申请空间大小
#define DCMI_CUSTOM_OP_CONF_ONE_LINE_MAX_LEN        256
#define DCMI_CUSTOM_OP_CONF_DEFAULT_COMMENT                                  \
    ("custom-op-recover:enable\n[custom-op-config start]\n[custom-op-config end]\n")
#define DCMI_CFG_CUSTOM_OP_DLINE_LEN                4
#define DCMI_CFG_CUSTOM_OP_CMD_LINE_LEN             40
#define DCMI_CUSTOM_OP_FLAG_NOT_FIND                0
#define DCMI_ERR_CODE_CUSTOM_OP_CONFIG_ILLEGAL      10
#define DCMI_CFG_CUSTOM_OP_LOCK_DIR                 "/run/custom_op_cfg_lock"
#define DCMI_CFG_CUSTOM_OP_LOCK_FILE_NAME           "/run/custom_op_cfg_lock/custom_op_cfg_lock_flag"

enum dcmi_cfg_line_type {
    DCMI_CFG_NOT_NEED_INSERT = 0,
    DCMI_CFG_NEED_INSERT,
    DCMI_CFG_HAS_SAME_LINE,
    DCMI_CFG_NOT_NEED_DELETE,
    DCMI_CFG_NEED_DELETE,
    DCMI_CFG_DIFF_LINE,
    DCMI_CFG_COVER_LINE
};

struct cfg_buf_info {
    char *buf;
    unsigned int buf_size;
};

int dcmi_cfg_create_lock_dir(char *path);

int dcmi_cfg_set_lock(int *fd, unsigned int timeout, const char* file_path);

void dcmi_cfg_set_unlock(int fd);

int dcmi_cfg_create_default_syslog_file();

int dcmi_cfg_insert_creat_vnpu_cmdline(unsigned int phy_id, struct dcmi_create_vdev_out *vdev,
    int card_id, int chip_id, const char *vnpu_conf_name);

int dcmi_cfg_insert_destroy_vnpu_cmdline(unsigned int phy_id, unsigned int vnpu_id, int card_id, int chip_id);

int dcmi_cfg_get_create_vnpu_template(unsigned int phy_id, unsigned int vdev_id,
    struct dcmi_create_vdev_res_stru *vdev);

int dcmi_cfg_set_config_recover_mode(unsigned int mode);

int dcmi_cfg_get_config_recover_mode(unsigned int *mode);

int dcmi_get_syslog_cfg_recover_mode(int *mode);

int dcmi_cfg_syslog_clear_file(int mode);

int dcmi_set_syslog_cfg_recover_mode(int mode);

int dcmi_cfg_insert_syslog_persistence_cmdline(char *cmdline);

int dcmi_check_syslog_cfg_legal(char *cfg, int cfg_len);

int dcmi_get_syslog_persistence_info(int *mode, char *cfg, int cfg_len);

int check_msn_is_existed(int device_logic_id, int card_id, int *msn_existed_flag);

int dcmi_cfg_card_exist_vnpu_config(int card_id, int *flag);

int dcmi_cfg_chip_exist_vnpu_config(int card_id, int chip_id, int *flag);

int dcmi_cfg_set_custom_op_config_recover_mode(unsigned int mode);

int dcmi_cfg_get_custom_op_config_recover_mode(unsigned int *mode);

int dcmi_cfg_insert_set_custom_op_cmdline(int card_id, int chip_id, int enable_value);
#endif