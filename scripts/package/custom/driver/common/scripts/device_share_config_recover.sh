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

# 1. this script is raised by user root, so can't not open other scripts here.
#    because if using other script here, they also has root authority,this takes 
#    security risk.
set +e

log_file="/var/log/device_boot_init.log"
installInfo="/etc/ascend_install.info"
mode=-1
ChipType=""
CmdLineCheck='^npu-smi +set +-t +device-share +-i +[0-7] +-c +[0-1] +-d +[0-1]$'
function log()
{
    if [ ! -f "$log_file" ]; then
        touch $log_file
        chmod 600 $log_file
    fi
    chmod 600 $log_file
    cur_date=$(date +"%Y-%m-%d %H:%M:%S")
    echo "[aicpu] [$cur_date] "$1 >> $log_file
}

DEVICE_SHARE_CONF="/etc/device_share.cfg"
phy_id=$1

getInstallParam() {
    local _key="$1"
    local _file="$2"
    local _param
    # update "Driver_Install_Type" "UserName" "UserGroup" "Driver_Install_Path_Param" "Driver_Install_For_All"
    install_info_key_array=("Driver_Install_Type" "UserName" "UserGroup" "Driver_Install_Path_Param" "Driver_Install_Mode" "Driver_Install_For_All")
    if [ ! -f "${_file}" ];then
        exit 1
    fi
    for key_param in "${install_info_key_array[@]}"; do
        if [ ${key_param} == ${_key} ]; then
            _param=`grep -r "${_key}=" "${_file}" | cut -d"=" -f2-`
            break;
        fi
    done
    echo "${_param}"
}
UserName=$(getInstallParam "UserName" "${installInfo}")
UserGroup=$(getInstallParam "UserGroup" "${installInfo}")
# read Driver_Install_Path_Param from installInfo
Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")

targetdir="${Driver_Install_Path_Param}"
targetdir="${targetdir%*/}"
username="$UserName"
usergroup="$UserGroup"
if [ x"$username" = x"" ]; then
    username=HwHiAiUser
    usergroup=HwHiAiUser
fi

function check_conf_path_and_phy_id()
{
    if [ -L "$DEVICE_SHARE_CONF" ]; then
        log "DEVICE_SHARE_CONF path is soft link, this has security risk, not take effect."
        return 1
    fi

    local user_id=$(stat -c %u ${DEVICE_SHARE_CONF})
    local group_id=$(stat -c %g ${DEVICE_SHARE_CONF})
    
    if [[ $(lspci | grep "d803") != "" ]]; then
        ChipType="910A3"
        CmdLineCheck='^npu-smi +set +-t +device-share +-i +[0-7] +-d +[0-1]$'
    elif [[ $(lspci | grep "d802") != "" ]]; then
        ChipType="910A2"
    elif [[ $(lspci | grep "d500") != "" ]]; then
        ChipType="310P"
    else
        log "This device does not support device-share command."
        return 1
    fi

    log "The $ChipType chip check is ok."

    if [ ! -n "${user_id}" ] || [ ! -n "${group_id}" ]; then
        log "user or group not exist, this has security risk, not take effect."
        return 1
    fi

    if [ ${user_id} != "0" ] || [ ${group_id} != "0" ]; then
        log "The owner is not root, this has security risk, not take effect."
        return 1
    fi

    if [[ !"$phy_id" =~ ^|0-9|+$ ]];then
        log "The phy_id is not number."
        return 1
    fi

    return 0
}

function wait_device_ready()
{
    local dev=$1
    local timeout=$2

    local counter=0
    while [ ${counter} -lt $timeout ]
    do
        counter=$(($counter+1))
        manager_user=$(ls -l $dev | awk '{print $3}')
        manager_group=$(ls -l $dev | awk '{print $4}')
        if [ x"$username" == x"$manager_user" ] && [ x"$usergroup" == x"$manager_group" ];then
            break
        else
            log "dev $dev $counter times username [${username}] manager_user [${manager_user}] mismatch"
            sleep 5
        fi
    done
    manager_user=$(ls -l $dev | awk '{print $3}')
    manager_group=$(ls -l $dev | awk '{print $4}')
    if [ x"$username" != x"$manager_user" ];then
        log "dev $dev username [${username}] manager_user [${manager_user}] mismatch, exit"
        return 1
    fi

    if [ x"$usergroup" != x"$manager_group" ];then
        log "dev $dev usergroup [${usergroup}] manager_group [${manager_group}] mismatch, exit"
        return 1
    fi

    return 0
}

function check_config_mode()
{
    while IFS= read -r cfg_line; do
        if [ "$cfg_line" == "device-share-recover:enable" ]; then
            mode=1
            return 0
        fi
        if [ "$cfg_line" == "device-share-recover:disable" ]; then
            mode=0
            return 0
        fi
    done < "${DEVICE_SHARE_CONF}"
    log "Warning: error start line or mode, The device-share recover task will not be executed."
    return 0
}

function config_check_before_reocver()
{
    local cfg_line=$1
    local card_id_cur=$2
    local chip_id_cur=$3
    local device_share_card_id=''
    local device_share_chip_id=''
    local enable_value=''

    if [[ ! $cfg_line =~ $CmdLineCheck ]]; then
        log "The device-share configuration file was modified unexpectedly, please check it!"
        return 1
    fi
    # 命令行格式：npu-smi info -t device-share -i 0 -c 0 -d 1 其中A3没有-c参数
    device_share_card_id=`echo ${cfg_line} | awk -F ' ' '{print $6}'`
    if [ $ChipType != "910A3" ]; then
        device_share_chip_id=`echo ${cfg_line} | awk -F ' ' '{print $8}'`
        enable_value=`echo ${cfg_line} | awk -F ' ' '{print $10}'`
    else
        enable_value=`echo ${cfg_line} | awk -F ' ' '{print $8}'`
    fi

    if [ "$device_share_card_id" -ne "$card_id_cur" ] || [ "${enable_value}" -eq 0 ]; then
        return 1
    fi
    if [ $ChipType != "910A3" ]; then
        if [ "$device_share_chip_id" -ne "$chip_id_cur" ]; then
            return 1
        fi
        echo "npu-smi set -t device-share -i ${device_share_card_id} -c ${device_share_chip_id} -d ${enable_value}"
        return 0
    fi
    echo "npu-smi set -t device-share -i ${device_share_card_id} -d ${enable_value}"
    return 0
}

function config_recover()
{
    local ret
    local cmd_info=""
    local card_id_cur=""
    local chip_id_cur=""
    local start=0
    local end=0
    local max_retries=20 # 定义最大重试次数
    local retry_interval=1  # 每次重试间隔 1 秒
    local retry_count=0 # 初始化计数器

    while [ -z "$card_id_cur" ] || [ -z "$chip_id_cur" ]; do
        # 检查是否达到最大重试次数
        if [ $retry_count -ge $max_retries ]; then
            log "Error: Failed to get card_id_cur or chip_id_cur for phy_id ${phy_id} after ${max_retries} retries."
            break  # 超出最大重试次数，退出 while 循环
        fi

        # 尝试获取 card_id_cur 和 chip_id_cur
        card_id_cur=$(/usr/local/sbin/npu-smi info -t phyid-remap -p ${phy_id} | grep "NPU ID" | awk -F ':' '{print $2}' | xargs)
        chip_id_cur=$(/usr/local/sbin/npu-smi info -t phyid-remap -p ${phy_id} | grep "Chip ID" | awk -F ':' '{print $2}' | xargs)
        # 如果仍然为空，记录警告并等待一段时间后重试
        if [ -z "$card_id_cur" ]; then
            log "Warning: card_id_cur is empty for phy_id ${phy_id}. Retrying... (Attempt ${retry_count}/${max_retries})"
            sleep $retry_interval
        fi

        # 增加重试计数器
        retry_count=$((retry_count + 1))
    done

    while IFS= read -r cfg_line; do
        if [ "$cfg_line" == "[device-share-config start]" ]; then
            start=1
            continue
        fi
        if [ "$cfg_line" == "[device-share-config end]" ]; then
            end=1
            break
        fi
        if [ ${start} -eq 1 ] && [ ${end} -eq 0 ]; then
            if [ -n "$cfg_line" ]; then
                cmd_info=$(config_check_before_reocver "$cfg_line" "$card_id_cur" "$chip_id_cur")
                ret=$?
                if [ $ret -ne 0 ]; then
                    continue
                fi
                wait_device_ready /dev/davinci${phy_id} 12
                ret=$?
                if [ $ret -ne 0 ];then
                    log "wait_device_ready /dev/davinci${phy_id} failed."
                    continue
                fi
                # device侧tsd任务启动比较慢，失败情况，增加重试
                for ((j = 0; j <= 4; j++))
                do
                    echo y | /usr/local/sbin/${cmd_info}
                    ret=0
                    if [ $ret -eq 0 ];then
                        log "$j times phyid ${phy_id} cmd ${cmd_info} exec result is $ret"
                        break
                    fi
                    log "$j times phyid ${phy_id} cmd ${cmd_info} exec err result is $ret"
                    sleep 10
                done
            fi
        fi
    done < "${DEVICE_SHARE_CONF}"
    if [ ${start} -ne 1 ]; then
        log "Warning: error start line or mode, This task will not be executed."
    fi
}

if [ ! -f "$DEVICE_SHARE_CONF" ]; then
    log "DEVICE_SHARE_CONF path is not exist, do not need to recover, exit."
    exit 0
fi

check_conf_path_and_phy_id
ret=$?
if [ $ret -ne 0 ];then
    log "check_conf_path failed. ret is $ret"
    exit 1
fi

wait_device_ready /dev/davinci_manager 37
ret=$?
if [ $ret -ne 0 ];then
    log "wait_device_ready /dev/davinci_manager failed."
    exit 1
fi

check_config_mode
if [ $mode -eq -1 ]; then
    exit 1
fi
if [ $mode -eq 1 ]; then
    config_recover
fi