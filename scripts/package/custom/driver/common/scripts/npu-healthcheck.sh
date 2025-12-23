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

# NPU health check script.

check_log="/${HOME}/npu-healthcheck.log"
work_dir="/${HOME}/npu-healthcheck"

#目前没有风险大的版本，若有后续在补充
dangerous_driver_list=()

#检测工具
npu_tool=npu-smi
driver_tool="/usr/local/Ascend/driver/tools/upgrade-tool"

# 错误码定义
OS_VERSION_ERROR=1
DRIVER_VERSION_ERROR=2
SERVER_TYPE_ERROR=4
DCHIP_HEALTH_ERROR=8
PCIE_STATUS_ERROR=16
NO_D_CHIP=128

#*****************************************************************************
# Prototype    : usage
# Description  : 提示升级输入参数
# Parameter:
#   input:  NA.
#   output: NA
# Return Value : NA
#  History        :
#  1.Date         : 2019/4/30
#    Modification : Created function
#*****************************************************************************
function usage()
{
    echo "*****************************************************************************"
    echo "parameters 1(check type):"
    echo "    1--OS 2--driver 4--server_type 8--Dchip 16--PCIE 31--all"
    echo "*****************************************************************************"
}

#*****************************************************************************
# Prototype    : log
# Description  : 封装写日志接口
# Parameter:
#   input:   写日志时的参数
#   output: NA
# Return Value : NA
#  History        :
#  1.Date         : 2019/4/30
#    Modification : Created function
#*****************************************************************************
function log()
{
    #检查升级文件是否存在
    if [ ! -f "$check_log" ];then
        touch $check_log
        chmod 640 $check_log
    fi
    cur_date=$(date +"%Y-%m-%d %H:%M:%S")
    echo "$cur_date "$1 | tee -a $check_log
}

#*****************************************************************************
# Prototype    : check_os_version
# Description  : 检查当前的OS是否在兼容范围内
# Parameter:
#   input:
#   output: NA
# Return Value : NA
#  History        :
#  1.Date         : 2019/4/30
#    Modification : Created function
#*****************************************************************************
function check_os_version()
{
    local kernel_version=""
    local release_version=""
    local os_list=()
    local has_find=0

    if [ "$(uname -a | grep -i "ubuntu")" != "" ];then
        kernel_version=$(uname -r)
        release_version=$(cat /etc/issue)
        os_list=("${ubuntu_os_list[@]}")
    elif [ -f "/etc/uvp-release" ] && [ ! -h "/etc/uvp-release" ];then
        if_uvpos=$(cat "/etc/uvp-release" | grep "Virtualization")
        if [ "$if_uvpos" != "" ];then
            kernel_version=$(uname -r)
            release_version=$(cat /etc/uvp-release)
            os_list=("${UvpOS_OS_list[@]}")
        else
            echo "OS type do not exist in the compatibility list."
            echo "OS infomation is : `uname -a`"
            return $OS_VERSION_ERROR
        fi
    elif [ "$(uname -a | grep -i "debian")" != "" ];then
        kernel_version=$(uname -r)
        if [ -f "/etc/debian_version" ] && [ ! -h "/etc/debian_version" ];then
            release_version=$(cat /etc/issue)$(cat /etc/debian_version)
        fi
        os_list=("${debian_os_list[@]}")
    elif [ -f "/etc/centos-release" ] && [ ! -h "/etc/centos-release" ];then
        if_centos=$(cat "/etc/centos-release" | grep -i "CentOS")
        if [ "$if_centos" != "" ];then
            kernel_version=$(uname -r)
            release_version=$(cat /etc/centos-release)
            os_list=("${centos_os_list[@]}")
        else
            echo "OS type do not exist in the compatibility list."
            echo "OS infomation is : $(uname -a)"
            return $OS_VERSION_ERROR
        fi
    elif [ -f "/etc/euleros-release" ] && [ ! -h "/etc/euleros-release" ];then
        if_euleros=$(cat "/etc/euleros-release" | grep -i "EulerOS")
        if [ "$if_euleros" != "" ];then
            kernel_version=$(uname -r)
            release_version=$(cat /etc/euleros-release)
            os_list=("${euleros_os_list[@]}")
        else
            echo "OS type do not exist in the compatibility list."
            echo "OS infomation is : $(uname -a)"
            return $OS_VERSION_ERROR
        fi
    elif [ -f "/usr/lib/os-release" ] && [ ! -h "/usr/lib/os-release" ];then
        if_uos=$(cat "/usr/lib/os-release" | grep "uos")
        if [ "$if_uos" != "" ];then
            kernel_version=$(uname -r)
            release_version=$(cat /usr/lib/os-release)
            os_list=("${UOS_OS_list[@]}")
        else
            echo "OS type do not exist in the compatibility list."
            echo "OS infomation is : `uname -a`"
            return $OS_VERSION_ERROR
        fi
    elif [ -f "/etc/SuSE-release" ] && [ ! -h "/etc/SuSE-release" ];then
        if_SuSE=$(cat "/etc/SuSE-release" | grep "SUSE")
        if [ "$if_SuSE" != "" ];then
            kernel_version=$(uname -r)
            release_version=$(cat /etc/SuSE-release)
            os_list=("${SuSE_OS_list[@]}")
        else
            echo "OS type do not exist in the compatibility list."
            echo "OS infomation is : `uname -a`"
            return $OS_VERSION_ERROR
        fi
    elif [ -f "/etc/tlinux-release" ] && [ ! -h "/etc/tlinux-release" ];then
        if_tlinux=$(cat "/etc/tlinux-release" | grep "tlinux")
        if [ "$if_tlinux" != "" ];then
            kernel_version=$(uname -r)
            release_version=$(cat /etc/tlinux-release)
            os_list=("${Tlinux_OS_list[@]}")
        else
            echo "OS type do not exist in the compatibility list."
            echo "OS infomation is : `uname -a`"
            return $OS_VERSION_ERROR
        fi     
    elif [ -f "/etc/neokylin-release" ] && [ ! -h "/etc/neokylin-release" ];then
        if_neokylin=$(cat "/etc/neokylin-release" | grep "NeoKylin")
        if [ "$if_neokylin" != "" ];then
            kernel_version=$(uname -r)
            release_version=$(cat /etc/neokylin-release)
            os_list=("${NeoKylin_OS_list[@]}")
        else
            echo "OS type do not exist in the compatibility list."
            echo "OS infomation is : $(uname -a)"
            return $OS_VERSION_ERROR
        fi           
    elif [ -f "/etc/kylin-release" ] && [ ! -h "/etc/kylin-release" ];then
        if_kylinos=$(cat "/etc/kylin-release" | grep -i "Kylin")
        if [ "$if_kylinos" != "" ];then
            kernel_version=$(uname -r)
            release_version=$(cat /etc/kylin-release)
            os_list=("${kylinos_os_list[@]}")
        else
            echo "OS type do not exist in the compatibility list."
            echo "OS infomation is : $(uname -a)"
            return $OS_VERSION_ERROR
        fi
    elif [ -f "/etc/bclinux-release" ] && [ ! -h "/etc/bclinux-release" ];then
        if_bclinuxos=$(cat "/etc/bclinux-release" | grep -i "BigCloud Enterprise Linux")
        if [ "$if_bclinuxos" != "" ];then
            kernel_version=$(uname -r)
            release_version=$(cat /etc/bclinux-release)
            os_list=("${bclinux_os_list[@]}")
        else
            echo "OS type do not exist in the compatibility list."
            echo "OS infomation is : $(uname -a)"
            return $OS_VERSION_ERROR
        fi
    else
        echo "OS type do not exist in the compatibility list."
        echo "OS infomation is : $(uname -a)"
        return $OS_VERSION_ERROR
    fi

    # OS版本及内核版本
    for (( j = 1 ; j < ${#os_list[*]} ; j += 2 ))
    do
        if [[ "$release_version" =~ "${os_list[0]}" && "$release_version" =~ "${os_list[$j]}"  && "$kernel_version" =~ "${os_list[$j + 1]}" ]]; then
            has_find=1
            return 0
        fi
    done

    if [[ $has_find -ne 1 ]];then
        echo "Current version information is $release_version, kernel version is $kernel_version. Compatibility list is ${os_list[*]}."
        return $OS_VERSION_ERROR
    fi
}

#*****************************************************************************
# Prototype    : npu_tool_check_version
# Description  : 通过npu-smi工具查询软件和固件版本
# Parameter:
#   input:
#   output: NA
# Return Value : NA
#  History        :
#  1.Date         : 2019/4/30
#    Modification : Created function
#*****************************************************************************
function npu_tool_check_version()
{
    local _out_save_file="$1"
    local has_find=0

    $npu_tool --help > /dev/null
    if [ $? -ne 0 ]; then
        log "[Error]npu_tool does not exist. Can not check $chip_type driver version"
        return $DRIVER_VERSION_ERROR
    fi

    while read card_info
    do
        ### analysis local card relationship
        card_id=$(echo "$card_info" grep "npu_id" | awk -F "npu_id=" '{print $2}' | awk -F ";" '{print $1}')
        chip_num=$(echo "$card_info" grep "npu_id" | awk -F "chip_num=" '{print $2}' | awk -F ";" '{print $1}')
        slot_id=$(echo "$card_info" grep "npu_id" | awk -F "slot_id=" '{print $2}' | awk -F ";" '{print $1}')

        $npu_tool info -t board -i "${card_id}" >board_card${card_id}.txt
        if [ $? -ne 0 ];then
            log "get board_card${card_id} fail"
            continue
        fi
        soft_version=$($npu_tool info -t board -i "${card_id}" | grep "Software Version" | awk -F ": " '{print $2}')

        local i=0
        for ((i=0; i<chip_num; i++))
        do
            $npu_tool info -t board -i "${card_id}" -c "{$i}" >board_card${card_id}_chip${chip_num}.txt
            if [ $? -ne 0 ];then
                log "get board_card${card_id}_chip${chip_num} fail"
                continue
            fi

            firm_version=$(grep -iw "Firmware Version" board_card${card_id}_chip${chip_num}.txt | awk -F ": " '{print $2}')
            for (( j = 0 ; j < ${#dangerous_driver_list[*]}  ; j++ ))
            do
                if [[ "$soft_version" =~ "${dangerous_driver_list[$j]}" ]] || [["$firm_version" =~ "${dangerous_driver_list[$j]}"]]; then
                    echo "card in slot $slot_id chip $chip_num version is in the dangerous driver list. Software is $soft_version, firmware version is $firm_version."
                    has_find=1
                fi
            done
        done
    done < $_out_save_file

    if [ $has_find -eq 1 ];then
        return $DRIVER_VERSION_ERROR
    else
        return 0
    fi
}

#*****************************************************************************
# Prototype    : driver_tool_check_version
# Description  : 通过upgrade-tool工具来获取ascend版本
# Parameter:
#   input:
#   output: NA
# Return Value : NA
#  History        :
#  1.Date         : 2019/4/30
#    Modification : Created function
#*****************************************************************************
function driver_tool_check_version()
{
    #获取ascend版本号
    # 首先判断 upgrade-tool 和 npu-smi工具是否存在
    local card_num=0
    local firm_info
    local soft_info
    local device_id
    local has_find=0

    $driver_tool --device_index -1 --system_version >get_soft_version.txt
    ret_val=$?
    if [ $ret_val -ne 0 ];then
        log "get get software version fail. ret=$ret_val"
        return $DRIVER_VERSION_ERROR
    fi

    $driver_tool --device_index -1 --component -1 --version >get_firm_version.txt
    ret_val=$?
    if [ $ret_val -ne 0 ];then
        log "get get firmware version fail. ret=$ret_val"
        return $DRIVER_VERSION_ERROR
    fi

    # 固件
    while read firm_info
    do
        for (( j = 0 ; j < ${#dangerous_driver_list[*]}  ; j++ ))
        do
            if [[ "$firm_info" =~ "${dangerous_driver_list[$j]}" ]]; then
                echo "firmware version is in the dangerous driver list: $firm_info."
                has_find=1
            fi
        done
    done <get_firm_version.txt

    # 软件
    while read soft_info
    do
        for (( j = 0 ; j < ${#dangerous_driver_list[*]}  ; j++ ))
        do
            if [[ "$soft_info" =~ "${dangerous_driver_list[$j]}" ]]; then
                echo "software version is in the dangerous driver list: $soft_info"
                has_find=1
            fi
        done
    done <get_soft_version.txt

    if [ $has_find -eq 1 ];then
        return $DRIVER_VERSION_ERROR
    else
        return 0
    fi
}

#*****************************************************************************
# Prototype    : check_ascend_driver
# Description  : 检查当前的驱动包是否为高风险或缺陷版本
# Parameter:
#   input:
#   output: NA
# Return Value : NA
#  History        :
#  1.Date         : 2019/4/30
#    Modification : Created function
#*****************************************************************************
function check_ascend_driver()
{
    #获取ascend版本号
    # 首先判断 upgrade-tool 和 npu-smi工具是否存在
    local card_num=0
    $driver_tool --help > /dev/null
    if [ $? -ne 0 ]; then
        log "[Error]upgrade-tool does not exist"
        #如果upgrade-tool不存在，则用npu-smi
        npu_tool_check_version "all_card_info.txt"
        if [ $? -eq 0 ];then
            return 0
        else
            return $DRIVER_VERSION_ERROR
        fi
    else
        driver_tool_check_version
        if [ $? -eq 0 ];then
            return 0
        else
            return $DRIVER_VERSION_ERROR
        fi
    fi
}

#*****************************************************************************
# Prototype    : check_server
# Description  : 检查服务器是否兼容支持Atlas 300标卡
# Parameter:
#   input:
#   output: NA
# Return Value : NA
#  History        :
#  1.Date         : 2019/4/30
#    Modification : Created function
#*****************************************************************************
function check_server()
{
    local has_find=0
    local manufacturer=$(dmidecode -t 1 | grep Manufacturer | awk '{print $2}' )
    local server_type=$(dmidecode -t 1 | grep "Product Name")
    server_type=${server_type#*"Product Name": }

    for (( j = 0 ; j < ${#huawei_server_list[*]}  ; j++ ))
    do
        if [[ "$server_type" =~ "${huawei_server_list[$j]}" ]]; then
            has_find=1
            return 0
        fi
    done

    if [[ $(lspci | grep "d100") != "" ]]
    then
        return 0
    else
        return $SERVER_TYPE_ERROR
    fi
}

#*****************************************************************************
# Prototype    : check_ascend_health
# Description  : 检查各个D芯片的健康状态
# Parameter:
#   input:
#   output: NA
# Return Value : NA
#  History        :
#  1.Date         : 2019/4/30
#    Modification : Created function
#*****************************************************************************
function check_ascend_health()
{
    local _out_save_file="$1"
    local has_find=0
    while read card_info
    do
        local health_status=""

        ### analysis local card relationship
        card_id=$(echo "$card_info" grep "npu_id" | awk -F "npu_id=" '{print $2}' | awk -F ";" '{print $1}')
        chip_num=$(echo "$card_info" grep "npu_id" | awk -F "chip_num=" '{print $2}' | awk -F ";" '{print $1}')
        slot_id=$(echo "$card_info" grep "npu_id" | awk -F "slot_id=" '{print $2}' | awk -F ";" '{print $1}')

        local i=0
        for ((i=0; i<chip_num; i++))
        do
            $npu_tool info -t health -i "${card_id}" -c "$i" >health_card${card_id}_chip${i}.txt
            if [ $? -ne 0 ];then
                log "get slot${slot_id}-chip${i} health fail"
                continue
            fi

            ### check start status
            health_status=$(grep -iw "Status" health_card${card_id}_chip${i}.txt | awk -F ": " '{print $2}' )
            if [ "${health_status}" == "OK" ];then
                continue
            else
                has_find=1
                cat "health_card${card_id}_chip${i}.txt"
                echo "slot${slot_id}_chip${i} health state is $health_status"
            fi

        done
    done < $_out_save_file

    if [ $has_find -eq 1 ];then
        return $DCHIP_HEALTH_ERROR
    else
        return 0
    fi
}

function check_iic_health()
{
	local _out_save_file="$1"
	local has_find=0

	while read card_info
    do
        local iic_status=""

        ### analysis local card relationship
        card_id=$(echo "$card_info" grep "npu_id" | awk -F "npu_id=" '{print $2}' | awk -F ";" '{print $1}')
        slot_id=$(echo "$card_info" grep "npu_id" | awk -F "slot_id=" '{print $2}' | awk -F ";" '{print $1}')

        $npu_tool info -t i2c_check -i "${card_id}" | grep "status" >iic_card${card_id}.txt
        if [ $? -ne 0 ];then
            log "get slot${slot_id} iic status fail"
            continue
        fi
        ### check start status

        while read iic_status_line
        do
            iic_status=$(echo $iic_status_line | awk -F ": " '{print $2}' )
            if [ "${iic_status}" == "OK" ];then
                continue
            else 
                has_find=1
                cat "iic_card${card_id}.txt"
                echo "slot${slot_id}_card${card_id} health state is $iic_status"
                break
            fi
        done < iic_card${card_id}.txt
            
    done < $_out_save_file
	
	if [ $has_find -eq 1 ];then
		return $DCHIP_HEALTH_ERROR
	else
		return 0
	fi
}

#*****************************************************************************
# Prototype    : check_pcie_state
# Description  : 检查Atlas 300标卡的PCIe接口带宽、PCIe错误状态、内部PCIe Switch端口的带宽、状态
# Parameter:
#   input:
#   output: NA
# Return Value : NA
#  History        :
#  1.Date         : 2019/4/30
#    Modification : Created function
#*****************************************************************************
function check_pcie_state()
{
    local bus_has_find=0
    local _out_save_file="$1"
    local bus_min_width=8
    local switch_min_width=4
    switch_has_find=0

    while read card_info
    do
        ### analysis local card relationship
        card_id=$(echo "$card_info" grep "npu_id" | awk -F "npu_id=" '{print $2}' | awk -F ";" '{print $1}')
        chip_num=$(echo "$card_info" grep "npu_id" | awk -F "chip_num=" '{print $2}' | awk -F ";" '{print $1}')
        pci_bus=$(echo "$card_info" grep "npu_id" | awk -F "pci_bus=" '{print $2}' | awk -F ";" '{print $1}')
        slot_id=$(echo "$card_info" grep "npu_id" | awk -F "slot_id=" '{print $2}' | awk -F ";" '{print $1}')

        bus_cap_speed_str=$(lspci -vvv -s $pci_bus 2>/dev/null | grep "LnkCap" | awk -F ',' '{print $2}' | awk -F "Speed " '{print $2}')
        bus_real_speed_str=$(lspci -vvv -s $pci_bus 2>/dev/null | grep "LnkSta" | awk -F ',' '{print $1}' | awk 'NR==1' | awk -F "Speed " '{print $2}')
        bus_cap_width_str=$(lspci -vvv -s $pci_bus 2>/dev/null | grep "LnkCap" | awk -F ',' '{print $3}' | awk -F "Width " '{print $2}')
        bus_real_width_str=$(lspci -vvv -s $pci_bus 2>/dev/null | grep "LnkSta" | awk -F ',' '{print $2}' | awk 'NR==1' | awk -F "Width " '{print $2}' | awk -F " " '{print $1}')

        bus_cap_speed=${bus_cap_speed_str%GT*}
        bus_real_speed=${bus_real_speed_str%GT*}
        bus_cap_width=${bus_cap_width_str#*x}
        bus_real_width=${bus_real_width_str#*x}
        
        $npu_tool info -t product -i "${card_id}" -c 0 | grep "Atlas 300I Model 3000"
        if [ $? -eq 0 ];then
            bus_min_width=2
            switch_min_width=2
        fi

        # 接口PCIE带宽不低于Gen3.0 X8  通过速率判断是3.0（只有3.0的才是8GT/s）
        if [[ $bus_real_width -lt $bus_min_width ]] || [[ $bus_cap_speed -lt 8 ]];then
            bus_has_find=1
            echo "card $card_id: bus_cap_speed $bus_cap_speed_str ;bus_real_speed $bus_real_speed_str; bus_cap_width $bus_cap_width_str; bus_real_width $bus_real_width_str"
        else
            # 检测PCIE 是否有AER 告警
            aer_error=$(lspci -vvv -s $pci_bus 2>/dev/null | grep "First Error Pointer: 00")
            if [ "${aer_error}" == "" ];then
                bus_has_find=1
                echo "PCIE bus $pci_bus has AER error"
                lspci -vvv -s $pci_bus 2>/dev/null | grep "AER"
            fi
        fi
        
        if [[ $(lspci | grep "d100") == "" ]]; then
            continue
        fi

        local i=0
        for ((i=0; i<chip_num; i++))
        do
            $npu_tool info -t board -i "${card_id}" -c "$i" > chipinfo_${card_id}_${i}.txt
            if [ $? -ne 0 ];then
                log "get slot${slot_id}-chip${i} board info fail" 
                continue
            fi

            ### check start status
            pci_chip=$(grep -iw "PCIe Bus Info" chipinfo_${card_id}_${i}.txt | awk -F ": " '{print $2}' )
            
            # 通过芯片的pcie编号找到PCIe switch的编号
            pci_switch=`find /sys -name remove | grep -i $pci_chip | awk -F '/' '{print $(NF-2)}'`
            if [[ "${pci_switch}" = "" ]];then
                log "slot${slot_id}-chip${i}"
                echo "query slot${slot_id}-chip${i} pci_switch failed"
                continue
            else
                switch_cap_speed_str=`lspci -vvv -s $pci_switch | grep "LnkCap" | awk -F ',' '{print $2}' | awk -F "Speed " '{print $2}'`
                switch_real_speed_str=`lspci -vvv -s $pci_switch | grep "LnkSta" | awk -F ',' '{print $1}' | awk 'NR==1' | awk -F "Speed " '{print $2}'`
                switch_cap_width_str=`lspci -vvv -s $pci_switch | grep "LnkCap" | awk -F ',' '{print $3}' | awk -F "Width " '{print $2}'`
                switch_real_width_str=`lspci -vvv -s $pci_switch | grep "LnkSta" | awk -F ',' '{print $2}' | awk 'NR==1' | awk -F "Width " '{print $2}' | awk -F " " '{print $1}'`
                
                switch_cap_speed=${switch_cap_speed_str%GT*}
                switch_real_speed=${switch_real_speed_str%GT*}
                switch_cap_width=${switch_cap_width_str#*x}
                switch_real_width=${switch_real_width_str#*x}
            fi
            # 端口PCIE带宽不低于Gen3.0 X4   通过速率判断是3.0（只有3.0的才是8GT/s）
            if [[ $switch_real_width -lt $switch_min_width ]] || [[ $switch_cap_speed -lt 8 ]];then
                switch_has_find=1
                echo "chip $i: switch_cap_speed $switch_cap_speed_str;switch_real_speed $switch_real_speed_str; switch_cap_width $switch_cap_width_str; switch_real_width $switch_real_width_str"
            else
                # 检测PCIE 是否有AER 告警
                ASR_error=$(lspci -vvv -s $pci_switch | grep "First Error Pointer: 00")
                if [[ "${ASR_error}" == "" ]];then
                    switch_has_find=1
                    echo "PCIE switch $pci_switch has AER error"
                    lspci -vvv -s $pci_bus | grep "AER"
                fi
            fi
        done
    done < $_out_save_file

    if [ $switch_has_find -eq 1 ] || [ $bus_has_find -eq 1 ];then
        return $PCIE_STATUS_ERROR
    else
        return 0
    fi
}
#*****************************************************************************
# Prototype    : get_local_cardinfo
# Description  : 查询所有Atlas 300标卡的信息，并保存供后续使用
# Parameter:
#   input:
#   output: NA
# Return Value : NA
#  History        :
#  1.Date         : 2019/4/30
#    Modification : Created function
#*****************************************************************************
function get_local_cardinfo()
{
    local _out_save_file="$1"
    rm -f "${_out_save_file}"

    ### get total card info
    $npu_tool info -l >total_cardinfo.txt
    if [ $? -ne 0 ]; then
        log "$npu_tool info -l get card info fail"
        return 1
    fi

    ### analysis total number
    grep "NPU ID" total_cardinfo.txt | awk -F ": " '{print $2}' > card_id.txt
    if [ ! -f card_id.txt ]; then
        log "$npu_tool info -l get card info is null"
        return 1
    fi

    ### get every card info
    local chip_num=""
    local bus_info=""
    local slot_id=""
    while read cardid_tmp
    do
        if [ "${cardid_tmp}" = "" ]; then
            continue
        fi

        $npu_tool info -t board -i $cardid_tmp > cardinfo_${cardid_tmp}.txt
        if [ $? -ne 0 ]; then
            log "$npu_tool info -q board -i $cardid_tmp get card info fail"
            continue
        fi

        bus_info=$(grep "PCIe Bus Info" cardinfo_${cardid_tmp}.txt | head -n 1 | awk -F ": " '{print $2}')
        if [ "${bus_info}" = "" ]; then
            log "$npu_tool info -t board -i $cardid_tmp get card bus info is null ,$(grep "PCIe Bus Info" cardinfo_${cardid_tmp}.txt)"
            continue
        fi

        chip_num=$(grep "Chip Count" cardinfo_${cardid_tmp}.txt | head -n 1 | awk -F ": " '{print $2}')
        if [ "${chip_num}" = "" ]; then
            log "npu_tool info -t board -i $cardid_tmp get card Chip Count is null ,$(grep "Chip Count" cardinfo_${cardid_tmp}.txt)"
            continue
        fi

        slot_id=$(grep "Slot ID" cardinfo_${cardid_tmp}.txt | head -n 1 | awk -F ": " '{print $2}' )
        if [ "${slot_id}" = "" ]; then
            log "npu_tool info -t board -i $cardid_tmp get card Chip Count is null ,$(grep "Chip Count" cardinfo_${cardid_tmp}.txt)"
            continue
        fi

        echo "npu_id=${cardid_tmp};pci_bus=${bus_info};chip_num=${chip_num};slot_id=${slot_id};" >>"${_out_save_file}"

    done <card_id.txt

    return 0
}

#*****************************************************************************
# Prototype    : show_success_mesasge
# Description  : 当巡检成功之后，呈现巡检结果
# Parameter:
#   input:
#   output: NA
# Return Value : NA
#  History        :
#  1.Date         : 2019/4/30
#    Modification : Created function
#*****************************************************************************
function show_success_mesasge()
{
    local card_num
    card_num=$(grep "Total Count" total_cardinfo.txt | awk -F ": " '{print $2}' | awk '{print $1}')
    echo "Check result: PASS"
    echo "There are $card_num NPU in total."
    echo "Testing items as follows: "
    echo "     driver version"
    echo "     server information"
    echo "     PCIE status"
    echo "     $chip_type health status"

    return 0
}

function check_init()
{
    if [[ $(lspci | grep "d801") != "" ]]
    then
        chip_type="ascend910"
        # OS兼容性列表
        ubuntu_os_list=("Ubuntu" "18.04" "4.15")
        debian_os_list=("Debian" "9.9" "4.19" "10.0" "4.19")
        centos_os_list=("CentOS" "7.6" "3.10" "7.6" "4.14" "7.6" "4.19" "8.2" "4.18")
        euleros_os_list=("EulerOS" "2.0 (SP8)" "4.19")
        kylinos_os_list=("Kylin" "V10" "4.19")
        bclinux_os_list=("BigCloud Enterprise Linux" "7.6" "4.19" "7.7" "4.19")
        #server兼容性列表
        huawei_server_list=("Atlas 800 (Model 9000)" "Atlas 900 Compute Node" "Atlas 800 (Model 9010)" "2288H V5" "TaiShan 200 (Model 2280)")
    elif [[ $(lspci | grep "d500") != "" ]]
    then
        chip_type="ascend310p"
        # OS兼容性列表
        ubuntu_os_list=("Ubuntu" "18.04" "4.15")
        centos_os_list=("CentOS" "7.6" "3.10")
        euleros_os_list=("EulerOS" "2.0 (SP8)" "4.19")  
        #server兼容性列表
        huawei_server_list=("2288H V5" "TaiShan 200 (Model 2280)")
    elif [[ $(lspci | grep "d100") != "" ]]
    then
        chip_type="miniD"
        # OS兼容性列表
        ubuntu_os_list=("Ubuntu" "16.04" "4.4.0" "16.04" "4.10.0" "18.04" "4.15.0")
        centos_os_list=("CentOS" "7.4" "3.10.0-693" "7.6" "4.14.0" "7.6" "3.10.0"  "8.2" "4.18.0")
        euleros_os_list=("EulerOS" "2.0" "4.19.36" "2.0" "4.19.90" "2.0" "3.10.0" "2.9" "4.19.95")
        UvpOS_OS_list=("EulerOS" "3.0" "4.19.36" "3.0" "3.10.0" "3.0" "4.18.0")
        UOS_OS_list=("UOS" "20" "4.19.0")
        SuSE_OS_list=("SuSE" "12" "4.12.14")
        Tlinux_OS_list=("Tlinux" "2.4" "4.14.105")
        NeoKylin_OS_list=("NeoKylin" "7.0" "4.14.0")
        kylinos_os_list=("Kylin" "V10" "4.19.90")
        #server兼容性列表
        huawei_server_list=("G560" "G530" "2500" "2288H V5" "TaiShan")
    else
        echo "There is no D chip in this server. Not support health check."
        echo "${NO_D_CHIP}"
        return ${NO_D_CHIP}
    fi

    return 0
}

function check_support_i2c_check()
{
    while read cardid_tmp
    do
        if [ "${cardid_tmp}" = "" ]; then
	    continue
        fi

        $npu_tool info -t product -i "${cardid_tmp}" -c 0  | grep "Atlas 300I Model 3000" > /dev/null
        if [ $? -eq 0 ];then
            return 0
        fi        

        $npu_tool info -t product -i "${cardid_tmp}" -c 0 | grep "Atlas 300I Model 3010" > /dev/null
        if [ $? -eq 0 ];then
            return 0
        fi  

        return 1
    done <card_id.txt

    return 1
}

#######################################################
#main
#######################################################

#检查临时工作目录是否存在
if [ ! -d "$work_dir" ];then
    mkdir $work_dir
fi

check_init
result=$?
if [ $result -ne 0 ]; then
    exit $result
fi

cd $work_dir >/dev/null

case $1 in
    1)
        cd - >/dev/null
        [ -d "${work_dir}" ] && rm -rf "${work_dir}"
        echo "0"
        exit 0
        ;;
    2)
        # 保存标卡信息
        get_local_cardinfo "all_card_info.txt"
        check_ascend_driver "all_card_info.txt"
        if [ $? -ne 0 ]; then
            echo "Check $chip_type version failed"
            cd - >/dev/null
            [ -d "${work_dir}" ] && rm -rf "${work_dir}"
            echo "$DRIVER_VERSION_ERROR"
            exit $DRIVER_VERSION_ERROR
        fi

        cd - >/dev/null
        [ -d "${work_dir}" ] && rm -rf "${work_dir}"
        echo "0"
        exit 0
        ;;
    4)
        check_server
        if [ $? -ne 0 ]; then
            echo "Check server failed"
            cd - >/dev/null
            [ -d "${work_dir}" ] && rm -rf "${work_dir}"
            echo "$SERVER_TYPE_ERROR"
            exit $SERVER_TYPE_ERROR
        fi

        cd - >/dev/null
        [ -d "${work_dir}" ] && rm -rf "${work_dir}"
        echo "0"
        exit 0
        ;;
    8)
        # 保存标卡信息
        get_local_cardinfo "all_card_info.txt"
        if [ $? -ne 0 ];then
            echo "Can not query card info. Can not check $chip_type health status."

            cd - >/dev/null
            [ -d "${work_dir}" ] && rm -rf "${work_dir}"
            echo "$DCHIP_HEALTH_ERROR"
            exit $DCHIP_HEALTH_ERROR          
        else
            check_support_i2c_check
            if [ $? -eq 0 ];then
                check_iic_health "all_card_info.txt"
            fi

            check_ascend_health "all_card_info.txt"
            if [ $? -ne 0 ]; then
                echo "Check $chip_type health failed"
                cd - >/dev/null
                [ -d "${work_dir}" ] && rm -rf "${work_dir}"
                echo "$DCHIP_HEALTH_ERROR"
                exit $DCHIP_HEALTH_ERROR
            fi
        fi

        cd - >/dev/null
        [ -d "${work_dir}" ] && rm -rf "${work_dir}"
        echo "0"
        exit 0
        ;;
    16)
        # 保存标卡信息
        get_local_cardinfo "all_card_info.txt"
        if [ $? -ne 0 ];then
            echo "Can not query card info. Can not check PCIE status."
            cd - >/dev/null
            [ -d "${work_dir}" ] && rm -rf "${work_dir}"
            echo "$PCIE_STATUS_ERROR"
            exit $PCIE_STATUS_ERROR
        else
            check_pcie_state "all_card_info.txt"
            if [ $? -ne 0 ]; then
                echo "Check PCIE status failed"
                cd - >/dev/null
                [ -d "${work_dir}" ] && rm -rf "${work_dir}"
                echo "$PCIE_STATUS_ERROR"
                exit $PCIE_STATUS_ERROR
            fi
        fi

        cd - >/dev/null
        [ -d "${work_dir}" ] && rm -rf "${work_dir}"
        echo "0"
        exit 0
        ;;
    31)
        check_server
        ret2=$?
        check_ascend_driver "all_card_info.txt"
        ret3=$?

        get_local_cardinfo "all_card_info.txt"
        if [ $? -ne 0 ];then
            echo "Can not query card info. Can not check $chip_type health and PCIE status."
            cd - >/dev/null
            [ -d "${work_dir}" ] && rm -rf "${work_dir}"
            echo "$[$ret2 + $ret3 + $PCIE_STATUS_ERROR + $DCHIP_HEALTH_ERROR]"
            exit $[$ret2 + $ret3 + $PCIE_STATUS_ERROR + $DCHIP_HEALTH_ERROR]
        fi

        check_support_i2c_check
        if [ $? -eq 0 ];then
            check_iic_health "all_card_info.txt"
            ret4_iic=$?
            check_ascend_health "all_card_info.txt"
            ret4_ascend=$?
            ret4=$[$ret4_iic + $ret4_ascend]
        else
            check_ascend_health "all_card_info.txt"
            ret4=$?
        fi
        
        check_pcie_state "all_card_info.txt"
        ret5=$?

        if [ $[$ret2 + $ret3 + $ret4 + $ret5] -eq 0 ];then
            show_success_mesasge
        fi

        cd - >/dev/null
        [ -d "${work_dir}" ] && rm -rf "${work_dir}"

        echo "$[$ret2 + $ret3 + $ret4 + $ret5]"
        exit $[$ret2 + $ret3 + $ret4 + $ret5]
        ;;
    *)
        echo "input parameter illegal"
        usage
        ;;
esac
