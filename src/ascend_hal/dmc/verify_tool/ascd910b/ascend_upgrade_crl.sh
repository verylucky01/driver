#!/bin/bash

# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------
set -e

CRL_FILE="$1"
CHECK_TOOL_PATH="/root/ascend_check"
INSTALL_INFO="/etc/ascend_install.info"
CHECK_TOOL_PATH_CRL="$CHECK_TOOL_PATH/ascend_cloud_v2.crl"
CHECK_TOOL_FILE="$CHECK_TOOL_PATH/ascend_check.bin"
CRL_TMP_FILE="$CHECK_TOOL_PATH/ascend_tmp.crl"
LOG_FILE="/var/log/ascend_crl_upgrade.log"
CHECK_IMG_FILE="ascend_910b_device_sw.img"

log() {
    if [ ! -f "$LOG_FILE" ];then
        touch $LOG_FILE
        chmod 600 $LOG_FILE
    fi

    chmod 600 $LOG_FILE
    cur_date=$(date +"%Y-%m-%d %H:%M:%S")
    echo "[crl upgrade] [$cur_date] "$1 >> $LOG_FILE
}

# check upgrade crl's integrity
check_crl_integrity() {
    local result=0
    if [ ! -f "$CRL_FILE" ];then
        log "[ERROR] crl file $CRL_FILE is not exist, please input valid file name"
        echo "upgrade fail, input crl file path is illegal, find crl file fail."
        return 1
    fi

    if [ ! -f "$CHECK_TOOL_FILE" ]; then
        log "[ERROR] crl check tool is not exist."
        echo "upgrade fail, crl check tool is not exist."
        return 1
    fi

    # use crl check tool to get crl file is valid or not, para 2 means check integrity
    result=$(echo "$($CHECK_TOOL_FILE 2 $CRL_FILE)$?")
    if [ "$result" -ne 0 ]; then
        log "[ERROR] input crl file is not a valid file, ret=$result."
        echo "upgrade fail, input crl file is illegal, please choose legal file to upgrade."
        return 1
    else
        log "[INFO] crl file integrity check pass."
    fi
}

# update crl file
crl_file_update() {
    if [ ! -d "$CHECK_TOOL_PATH" ]; then
        mkdir -p $CHECK_TOOL_PATH
        chmod 500 $CHECK_TOOL_PATH
        log "[INFO] mkdir $CHECK_TOOL_PATH success"
    fi

    if [ ! -e "$CHECK_TOOL_PATH_CRL" ]; then
        touch $CHECK_TOOL_PATH_CRL
        chmod 400 $CHECK_TOOL_PATH_CRL
        log "[INFO] touch $CHECK_TOOL_PATH_CRL success"
    fi

    cp -f $CRL_FILE $CHECK_TOOL_PATH_CRL
}

# check check
upgrade_crl() {
    local img_file=0
    local drv_install_path=0
    local result=0

    log "[INFO] start to upgrade crl file, file_path: $CRL_FILE, ip_addr: 127.0.0.1, user: root"

    # check upgrade crl file's integrity, if not valid, return fail
    check_crl_integrity
    if [ "$?" -ne 0 ];then
        log "[ERROR] check crl file intgrity fail"
        return 1
    fi

    # check upgrade crl file and old crl file, if upgrade crl is old, return fail
    if [ -e "$CHECK_TOOL_PATH_CRL" ]; then
        log "[INFO] old crl file is exist"
        result=$(echo "$($CHECK_TOOL_FILE 2 $CHECK_TOOL_PATH_CRL)$?")
        if [ "$result" -eq 0 ]; then
            result=$(echo "$($CHECK_TOOL_FILE 1 $CRL_FILE $CHECK_TOOL_PATH_CRL)$?")
            if [ "$result" -gt 1 ]; then
                log "[ERROR] upgrade crl file is old, upgrade failed"
                echo "The uploaded CRL is older than the one on the device, and the CRL file on the device is not updated."
                return 1
            fi
        else
            log "[ERROR] $CHECK_TOOL_PATH_CRL is illegal."
            echo "upgrade CRL fail, $CHECK_TOOL_PATH_CRL is illegal, please delete this file first."
            return 1
        fi
    else
        log "[INFO] $CHECK_TOOL_PATH_CRL is not exist."
    fi

    # parse image crl content and compare
    . $INSTALL_INFO
    drv_install_path=$Driver_Install_Path_Param
    IMG_PATH="$drv_install_path/driver/device"
    img_file="$IMG_PATH/$CHECK_IMG_FILE"
    if [ -f "$img_file" ]; then
        touch $CRL_TMP_FILE
        chmod 600 $CRL_TMP_FILE

        # get img crl content and save to crl temp file
        result=$(echo "$($CHECK_TOOL_FILE 0 $img_file $CRL_TMP_FILE)$?")
        if [ "$result" -ne 0 ]; then
            log "[INFO] image do not have crl file, upgrade crl file directly"
        else
            result=$(echo "$($CHECK_TOOL_FILE 3 $CRL_FILE $img_file)$?")
            if [ $result -ne 0 ]; then
                rm -f $CRL_TMP_FILE
                log "[ERROR] image verify failed, need to upgrade image file first"
                echo "The signature of the current image is insecure. Upgrade the image and then upgrade the CRL."
                return 1
            fi

            log "[INFO] image verify success"
        fi

        rm -f $CRL_TMP_FILE
    else
        log "[INFO] $img_file is not exist."
    fi

    crl_file_update
    log "[INFO] upgrade crl file success"
    echo "upgrade CRL success."
    return 0
}

# upgrade process
upgrade_crl
