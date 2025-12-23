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

# NPU log collect script.

CURRENT_DIR=$(pwd)
TOP_DIR=$(pwd)

ASCEND_INSTALL_PATH="/usr/local/Ascend"

os_name=$(cat /etc/*release | grep -w 'NAME' | awk -F "=" '{ print $2 }')
dev_list=$(npu-smi info -l | grep "NPU ID" | awk -F ":" '{ print $2 }')
dev_id=""
main_flag=0
MCU_LOG_MODE=1

CHIP_NUM_310=$(lspci | grep -E "d100" | wc -l)
CHIP_NUM_310P=$(lspci | grep -E "d500" | wc -l)
CHIP_NUM_910=$(lspci | grep -E "d801" | wc -l)
CHIP_NUM_910B=$(lspci | grep -E "d802" | wc -l)
CHIP_NUM_910_93=$(lspci | grep -E "d803" | wc -l)

usage() {
    echo "Usage: $(basename $0)"
    echo "  or   $(basename $0) [OPTION]"
    echo "Example:"
    echo "$(basename $0)                    Collect the default content."
    echo "$(basename $0) -p /home/tmp       Specify a path to store the generated content."
    echo "$(basename $0) -off host_info     Disable host os info."
    echo "$(basename $0) -m                 Collect the mcu logs."
    echo "$(basename $0) -l                 Collect the link_down logs."
    echo "$(basename $0) -f                 Collect the full logs."
    echo "Options:"
    echo "    -h, --help        Help."
    echo "    -p, --path        Specify a path to store the generated content. If the path is inexistent, this tool will create the directory."
    echo "    --off             Disable collecting specified content."
    echo "                      <host_info>: disable collecting host os info, including environment info, pcie config, proc info, and so on."
    echo "                      <host_log>: disable collecting host log, including dmesg, messages, kern.log, syslog."
    echo "                      <device_log>: disable collecting device dump log."
    echo "                      <driver_info>: disable collecting driver info."
    echo "                      <install_info>: disable collecting ascend driver install info."
    echo "                      <network_info>: disable collecting ascend driver network configuration."
}

function parse_args()
{
    if [ $# -eq 0 ]; then
        echo "No arguments provided. Performing default action..."
        main 
        exit 0
    fi
    while [ "$1" != "" ]; do
        case "$1" in
            -p | --path )
                if [ -z "$2" ]; then
                    usage
                    exit 1
                elif [ ! -d "$2" ] && [ ! -h "$2" ]; then
                    echo "Warning: The specified path is inexistent, this tool will create the directory." 
                    mkdir -p "$2"
                    chmod 440 "$2"
                fi
                cd "$2"
                TOP_DIR=$(pwd)
                main_flag=1
                shift
                ;;
            --off )
                if [ -z "$2" ]; then
                    usage
                    exit 1
                elif [ "$2" = "mcu_log" ]; then
                    MCU_LOG_MODE=0
                fi
                main_flag=1
                shift
                ;;
            -h | --help )
                usage
                exit
                ;;
            -m | --mcu )
                echo "You are now collecting mcu logs."
                get_install_variable
                create_log_collect_dir
                collect_mcu_log
                compress_file
                exit
                ;;
            -l | --linkdown )
                echo "You are now collecting link_down logs."
                network_info_collect
                exit
                ;;
            -f | --full )
                echo "You are now collecting full logs."
                collect_full
                exit
                ;;
            * )
                usage
                exit 1
        esac
        shift
    done
}

function create_log_collect_dir()
{
    LOG_COLLECT_DIR=$TOP_DIR/npu_log_collect
    if [ ! -d "$LOG_COLLECT_DIR" ]; then
        mkdir -p $LOG_COLLECT_DIR
        if [ $? -ne 0 ]; then
            echo "Error: Create log collect directory failed. Please check." 
            exit 1
        fi
    fi
    LOG_COLLECT_RUNING_INFO=$LOG_COLLECT_DIR/collect_scripts_running_log
    LOG_FILE_PATH=$LOG_COLLECT_DIR
    FILE_DRIVER_LOG=$LOG_COLLECT_DIR/driver_info.log
    FILE_VERSION_LOG=$LOG_COLLECT_DIR/version_info.log
    FILE_OS_LOG=$LOG_COLLECT_DIR/OS_info.log
    FILE_PCIE_LOG=$LOG_COLLECT_DIR/pcie_info.log
    FILE_NPU_LOG=$LOG_COLLECT_DIR/npu_card_info.log
    FILE_version_info=$LOG_COLLECT_DIR/version_info.log
}

function show_info()
{
    echo
    echo "You are now running "$(basename $0)". This is a tool which will collect necessery information"
    echo "about your system, Linux kernel, ascend kernel module, nputools and MCU. This will help our support"
    echo "team diagnose the issue you are facing while using the product. The infomation collection process may"
    echo "take several minutes and it will generate the tar file named with timestamp under the assigned directory."
    echo "Please download and send the file to the support team, thanks."
    echo
}

function get_install_variable()
{
    INSTALL_PATH=$(cat /lib/davinci.conf | awk -F '=' '{print $2}' | head -1)
}

function collect_ascend_bug_log()
{
    green_echo "$(date) start collect ascend_bug_report log" | tee -a $LOG_COLLECT_RUNING_INFO
    chmod 440 $LOG_COLLECT_RUNING_INFO
    $INSTALL_PATH/driver/tools/ascend_bug_report.sh "$@" 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    mv $TOP_DIR/ascend_log_* $LOG_COLLECT_DIR/
    chmod 440 $LOG_COLLECT_DIR/ascend_log_*
    green_echo "$(date) End collect ascend_bug_report log" | tee -a $LOG_COLLECT_RUNING_INFO
}

function collect_nputools_run_log()
{
    green_echo "$(date) start collect run log" | tee -a $LOG_COLLECT_RUNING_INFO
    mkdir -p $LOG_COLLECT_DIR/nputools_log/root
    cd $LOG_COLLECT_DIR/nputools_log/root
    cp /var/log/nputools_LOG_ERR.log ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    cp /var/log/nputools_LOG_ERR.log.1 ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    cp /var/log/nputools_LOG_ERR.log-[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9] ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    cp /var/log/nputools_LOG_ERR.log.bak ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    cp /var/log/nputools_LOG_INFO.log ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    cp /var/log/nputools_LOG_INFO.log.1 ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    cp /var/log/nputools_LOG_INFO.log-[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9] ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    cp /var/log/nputools_LOG_INFO.log.bak ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    cp /var/log/nputools_LOG_OP.log ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    cp /var/log/nputools_LOG_OP.log.1 ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    cp /var/log/nputools_LOG_OP.log-[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9] ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    cp /var/log/nputools_LOG_OP.log.bak ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    cd $CURRENT_DIR
    user_name_list=$(dir /home)
    for user_name in $user_name_list
    do
        if [ ! -d "/home/$user_name/var/log" ]
        then
            continue
        else
            mkdir -p $LOG_COLLECT_DIR/nputools_log/$user_name
            cd $LOG_COLLECT_DIR/nputools_log/$user_name
            cp /home/$user_name/var/log/nputools_LOG_ERR.log ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
            cp /home/$user_name/var/log/nputools_LOG_ERR.log.bak ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
            cp /home/$user_name/var/log/nputools_LOG_INFO.log ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
            cp /home/$user_name/var/log/nputools_LOG_INFO.log.bak ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
            cp /home/$user_name/var/log/nputools_LOG_OP.log ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
            cp /home/$user_name/var/log/nputools_LOG_OP.log.bak ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
            cd $CURRENT_DIR
        fi
    done
    green_echo "$(date) End collect run log" | tee -a $LOG_COLLECT_RUNING_INFO
}

function collect_npu_info_log()
{
    green_echo "$(date) start collect npu info log" | tee -a $LOG_COLLECT_RUNING_INFO

    local chip_list=$(npu-smi info -l | grep "Chip Count" | awk -F ":" '{ print $2 }')
    local card_cnt=$(npu-smi info -l | grep -E "Card|Total Count" | awk -F ":" '{ print $2 }')
    mkdir $LOG_COLLECT_DIR/npu_info_log
    touch $LOG_COLLECT_DIR/npu_info_log/card_info.log && chmod 440 $LOG_COLLECT_DIR/npu_info_log/card_info.log
    timeout 10s npu-smi info 1>> $LOG_COLLECT_DIR/npu_info_log/card_info.log 2>> $LOG_COLLECT_RUNING_INFO
    timeout 10s npu-smi info -l 1>> $LOG_COLLECT_DIR/npu_info_log/card_info.log 2>> $LOG_COLLECT_RUNING_INFO

    OLD_IFS="$IFS"
    IFS=" "
    chip_count_list=($chip_list)
    IFS="$OLD_IFS"
    j=0
    for dev_id in $dev_list
    do
        echo "########### collect npu id $dev_id product info ###########" >> $LOG_COLLECT_DIR/npu_info_log/card_info.log 
        echo "npu-smi info -t product -i $dev_id -c 0 1" >> $LOG_COLLECT_DIR/npu_info_log/card_info.log
        timeout 10s npu-smi info -t product -i $dev_id -c 0 1>> $LOG_COLLECT_DIR/npu_info_log/card_info.log 2>> $LOG_COLLECT_RUNING_INFO

        for ((i=0;i<${chip_count_list[j]};i++))
        do
            if [ $j -ge 0 ] && [ $j -lt $card_cnt ]; then
                echo "############ collect npu id $dev_id chip id $i board info ############" >> $LOG_COLLECT_DIR/npu_info_log/card_info.log
                echo "npu-smi info -t board -i $dev_id -c $i" >> $LOG_COLLECT_DIR/npu_info_log/card_info.log
                timeout 10s npu-smi info -t board -i $dev_id -c $i 1>> $LOG_COLLECT_DIR/npu_info_log/card_info.log 2>> $LOG_COLLECT_RUNING_INFO

                echo "########### collect npu id $dev_id chip id $i health info #############" >> $LOG_COLLECT_DIR/npu_info_log/card_info.log
                echo "npu-smi info -t health -i $dev_id -c $i" >> $LOG_COLLECT_DIR/npu_info_log/card_info.log
                timeout 10s npu-smi info -t health -i $dev_id -c $i 1>> $LOG_COLLECT_DIR/npu_info_log/card_info.log 2>> $LOG_COLLECT_RUNING_INFO

                echo "########### collect npu id $dev_id chip id $i ecc info #############" >> $LOG_COLLECT_DIR/npu_info_log/card_info.log
                echo "npu-smi info -t ecc -i $dev_id -c $i" >> $LOG_COLLECT_DIR/npu_info_log/card_info.log
                timeout 10s npu-smi info -t ecc -i $dev_id -c $i 1>> $LOG_COLLECT_DIR/npu_info_log/card_info.log 2>> $LOG_COLLECT_RUNING_INFO

                echo "########### collect npu id $dev_id chip id $i hccs info #############" >> $LOG_COLLECT_DIR/npu_info_log/card_info.log
                echo "npu-smi info -t hccs -i $dev_id -c $i" >> $LOG_COLLECT_DIR/npu_info_log/card_info.log
                timeout 10s npu-smi info -t hccs -i $dev_id -c $i 1>> $LOG_COLLECT_DIR/npu_info_log/card_info.log 2>> $LOG_COLLECT_RUNING_INFO
            else
                break
            fi
        done
        ((j++))
    done
    green_echo "$(date) End collect npu info log" | tee -a $LOG_COLLECT_RUNING_INFO
}

function collect_mcu_log()
{
    if [ $MCU_LOG_MODE -eq 0 ]; then
        return 0
    fi

    green_echo "$(date) start collect mcu_log" | tee -a $LOG_COLLECT_RUNING_INFO

    if [ -d "/run/mcu_log" ]; then
        rm -f /run/mcu_log/error_log*.log /run/mcu_log/maintaince_log*.log /run/mcu_log/operate_log*.log 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    fi

    for dev_id in $dev_list
    do
        (
            npu-smi set -t collect-log -i $dev_id 1>> $LOG_COLLECT_RUNING_INFO 2>&1 
            mkdir -p $LOG_COLLECT_DIR/mcu_log/card_${dev_id}_mcu_log
            cp -f /run/mcu_log/error_log_${dev_id}_*.log /run/mcu_log/maintaince_log_${dev_id}_*.log /run/mcu_log/operate_log_${dev_id}_*.log $LOG_COLLECT_DIR/mcu_log/card_${dev_id}_mcu_log/ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
        ) &
    done
    wait
    green_echo "$(date) End collect mcu_log" | tee -a $LOG_COLLECT_RUNING_INFO
}

function collect_lingqu_log()
{
    green_echo "$(date) start collect lingqu_log" | tee -a $LOG_COLLECT_RUNING_INFO

    mkdir -p $LOG_COLLECT_DIR/lingqu/root
    cd $LOG_COLLECT_DIR/lingqu/root
    cp -f /var/log/lqdcmitools* ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    cp -f /var/log/lingqu/* ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
    cd $CURRENT_DIR
    user_name_list=$(dir /home)
    for user_name in ${user_name_list[@]}
    do
        if [ ! -d "/home/${user_name}/var/log" ]
        then
            continue
        else
            mkdir -p $LOG_COLLECT_DIR/lingqu/${user_name}
            cd $LOG_COLLECT_DIR/lingqu/${user_name}
            cp /home/${user_name}/var/log/lqdcmitools_LOG_* ./ 1>> $LOG_COLLECT_RUNING_INFO 2>&1
            cd $CURRENT_DIR
        fi
    done

    wait
    green_echo "$(date) End collect lingqu_log" | tee -a $LOG_COLLECT_RUNING_INFO
}

function compress_file()
{
    START_TIME=$(date +"%Y-%m-%d %H:%M:%S")
    DIR_DATE=$(echo $START_TIME | tr -cd "[0-9]")
    cd $TOP_DIR
    echo "compressing files into a tar package" | tee -a $LOG_COLLECT_RUNING_INFO
    if [ "$os_name" == "SLES" ];then
        tar  --format=gnu -czPf npu_log_collect_$DIR_DATE.tar.gz npu_log_collect
    else
        tar -czPf npu_log_collect_$DIR_DATE.tar.gz npu_log_collect
    fi
    chmod 440 npu_log_collect_$DIR_DATE.tar.gz
    rm -rf $LOG_COLLECT_DIR
    cd
}

pid=$$
function conflict_check()
{
    PS_INFO=$(ps -ef)
    getpid=$(echo "$PS_INFO" | grep "npu_log_collect.sh" | grep -v "grep" | awk '{print $2}')

    exitpid=0
    for loop in $getpid
    do
        if [ "$loop" = "$pid" ]; then
            exitpid=0
        else
            exitpid=1
            break
        fi
    done

    if [ $exitpid -eq 1 ]; then
        echo "Error: Other npu_log_collect.sh task is running."
        exit 1
    fi
}

function main()
{
    show_info
    get_install_variable
    create_log_collect_dir
    collect_ascend_bug_log "$@" &
    collect_nputools_run_log &
    collect_npu_info_log &
    collect_mcu_log &

    CHIP_NUM_910_93=${CHIP_NUM_910_93:-0}
    if [ "$CHIP_NUM_910_93" -ne 0 ]; then
        collect_lingqu_log &
    fi

    wait
    compress_file
    green_echo "$(date)"
}

function collect_full() 
{
    show_info
    get_install_variable
    create_log_collect_dir
    collect_ascend_bug_log "$@" &
    collect_nputools_run_log &
    collect_npu_info_log &
    collect_mcu_log &

    CHIP_NUM_910_93=${CHIP_NUM_910_93:-0}
    if [ "$CHIP_NUM_910_93" -ne 0 ]; then
        collect_lingqu_log &
    fi
	
    get_version_info 
	get_path_env   &
    get_driver_log &
	get_pcie_log 2>/dev/null &	 
    get_os_info &
	get_qemu_log &

    wait  	  
    get_device_log 
    compress_file
    green_echo "$(date)"
}

function check_run_in_vm()
{
    dmidecode | grep -E "Manufacturer: QEMU|Manufacturer: qemu|xen|Xen|VMware|OpenStack|KVM Virtual Machine"
    if [ $? -eq 0 ]; then
        return 1
    else
        /usr/bin/systemd-detect-virt > /dev/null  2>&1
        if [ $? -eq 0 ]; then
            return 1
        else
            return 0
        fi
    fi
}

function check_run_in_docker()
{
    cat /proc/self/cgroup | grep "docker"
    if [ $? -eq 0 ]; then
        return 1
    else
        return 0
    fi
}

function network_info_collect()
{
    if [ $CHIP_NUM_910 -eq 0 ]; then
        return 0
    fi
    product=$CHIP_NUM_910B
    product_803=$CHIP_NUM_910_93
    #查询NPU卡的数量， 默认A+X 16张卡，A+K 8张卡
    if [ $product -ne 0 ];then
        npu_nums=$[8+$( uname -a | grep x86 | wc -l )*8];
    elif [ $product_803 -ne 0 ];then
        npu_nums=16
    else
        exit
    fi
    npu_nums=$((npu_nums - 1))

    #创建文件夹并进入
    current_time=$(date +%Y%m%d%H%M%S)
    mkdir network_info_data_$current_time
    cd network_info_data_$current_time

    # get_device_error_log
    # 开info前收集一次日志
    ##开启所有卡的imp、roce的info等级日志
    echo "$(date)-----------collect device error log start..."
    (msnpureport -f | grep "Export finished." ;
    for i in $(seq 0 $npu_nums);do msnpureport -m IMP:info -d $i &>> set_log_level.log;done;
    for i in $(seq 0 $npu_nums);do msnpureport -m ROCE:info -d $i &>> set_log_level.log;done;)&
    echo "-----------collect device error log success---------------"

    #查看驱动构建时间
    echo -e "\n--------------------- v1.70 ---------------------" >> build_info.log
    echo -e "\n-----------build info---------------" >> build_info.log
    cat /usr/local/Ascend/driver/build.info >> build_info.log
    echo -e "\n-----------system up time---------------" >> build_info.log
    who -b >> build_info.log
    last reboot | head >> build_info.log
    echo -e "\n-----------local ip info---------------" >> ifconfig.log
    ifconfig >> ifconfig.log

    #获取os信息
    echo -e "\n-----------OS info---------------" >> npu-smi.log
    cat /etc/os-release &>> npu-smi.log
    cat /etc/centos-release &>> npu-smi.log
    uname -a >> npu-smi.log
    echo "-----------collect OS info info success---------------"
    cat /usr/local/Ascend/firmware/version.info &>> npu-smi.log
    (npu-smi info &>> npu-smi.log)&
    echo "-----------collect npu-smi info success---------------"

    # get_optical_info
    echo -e "\n" >> optical.log
    #查询所有卡的网口up信息，重定向输出到optical.log
    echo "-----------link info---------------" >> optical.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> optical.log;timeout 10s hccn_tool -i $i -link -g &>> optical.log;done
    #查询所有卡的网口ip信息，重定向输出到optical.log
    echo "-----------ip info---------------" >> optical.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> optical.log;timeout 10s hccn_tool -i $i -ip -g &>> optical.log;done
    #查询所有卡的光模块信息，重定向输出到optical.log
    echo "-----------optical---------------" >> optical.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> optical.log;timeout 10s hccn_tool -i $i -optical -g &>> optical.log;done
    echo "-----------optical success---------------"
    #查询所有卡的link状态，重定向输出到optical.log
    echo -e "\n-----------link stat---------------" >> optical.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> optical.log;timeout 10s hccn_tool -i $i -link_stat -g &>> optical.log;done
    echo -e "\n-----------optical reg info---------------" >> optical.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> optical.log;timeout 10s hccn_tool -i $i -optical -dump all &>> optical.log;done
    echo "-----------collect optical reg info success---------------"
    echo -e "\n-----------optical v2---------------" >> optical.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> optical.log;timeout 10s hccn_tool -i $i -optical -g v2 &>> optical.log;done
    #查询所有卡的speed、shaping信息，结果重定向输出到optical.log
    echo -e "\n-----------speed info---------------" >> optical.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> optical.log;timeout 10s hccn_tool -i $i -speed -g &>> optical.log;done
    echo "-----------collect speed info success---------------"
    echo -e "\n-----------shaping info---------------" >> optical.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> optical.log;timeout 10s hccn_tool -i $i -shaping -g &>> optical.log;done
    echo "-----------collect shaping info success---------------"

    # get_reg_info
    #查询所有卡的pcs link状态，重定向输出到optical.log
    echo -e "\n-----------pcs link---------------" >> optical.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> optical.log;timeout 10s hccn_tool -i $i -reg -a 0x106040f0 &>> optical.log;done
    echo "-----------collect pcs link info success---------------"
    #查询所有卡的mac link状态，重定向输出到optical.log
    echo -e "\n-----------mac link---------------" >> optical.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> optical.log;timeout 10s hccn_tool -i $i -reg -a 0x10600420 &>> optical.log;done
    echo "-----------collect mac link info success---------------"
    #查询所有卡是否有本端错误，重定向输出到optical.log
    echo -e "\n-----------rf lf---------------" >> optical.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> optical.log;timeout 10s hccn_tool -i $i -reg -a 0x10600460 &>> optical.log;done
    echo "-----------collect reg info success---------------"

    # get_cdr_info
    #dump cdr寄存器
    echo -e "\n-----------scdr snr 1 times---------------" >> cdr_reg.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> cdr_reg.log;timeout 10s hccn_tool -i $i -scdr -t 4 &>> cdr_reg.log;done
    #查询cdr snr是否有异常，重定向输出到cdr.log
    echo -e "\n-----------scdr snr 1 times---------------" >> cdr1.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> cdr1.log;timeout 10s hccn_tool -i $i -scdr -t 5 &>> cdr1.log;done
    echo -e "\n-----------scdr snr 2 times---------------" >> cdr2.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> cdr2.log;timeout 10s hccn_tool -i $i -scdr -t 5 &>> cdr2.log;done
    echo -e "\n-----------scdr snr 3 times---------------" >> cdr3.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> cdr3.log;timeout 10s hccn_tool -i $i -scdr -t 5 &>> cdr3.log;done
    echo "-----------scdr snr success---------------"

    # get_stat_info
    #查询lldp，重定向输出到lldp.log
    echo -e "\n-----------lldp---------------" >> lldp.log
    for i in $(seq 0 $npu_nums);do echo "====> $i" >> lldp.log;timeout 10s hccn_tool -i $i -lldp -g &>> lldp.log;done;
    echo "-----------collect lldp info success---------------"
    #查询网卡侧收发包状态，重定向输出到stat.log
    echo -e "\n-----------stat---------------" >> stat.log
    for i in $(seq 0 $npu_nums); do echo "====> $i" >> stat.log;timeout 10s hccn_tool -i $i -stat -g &>> stat.log;done
    #查询hilink serdes info，重定向输出到hilink.log
    echo -e "\n-----------hilink stable register---------------" >> hilink.log
    for i in $(seq 0 $npu_nums); do echo -e "\n\n\n====> $i" >> hilink.log;timeout 10s hccn_tool -i $i -hilink_info -t 0 &>> hilink.log;done
    echo "-----------collect hilink info success---------------"
    #查询hilink serdes info，重定向输出到hilink.log
    echo -e "\n-----------link down hilink register---------------" >> hilink.log
    for i in $(seq 0 $npu_nums); do echo -e "\n\n\n====> $i" >> hilink.log;timeout 10s hccn_tool -i $i -hilink_info -t 1 &>> hilink.log;done
    echo "-----------collect serdes info success---------------"
    #查询serdes的信号质量，重定向输出到serdes.log
    echo -e "\n-----------serdes quaility---------------" >> serdes.log
    for i in $(seq 0 $npu_nums); do echo "====> $i" >> serdes.log; ascend-dmi --sq -t roce -d $i &>> serdes.log;done
    #考虑到，部分服务器没有ascend-dmi，补充一条-serdes -g
    for i in $(seq 0 $npu_nums); do echo "====> $i" >> serdes.log; timeout 10s hccn_tool -i $i -serdes -g &>> serdes.log;done
    echo "-----------collect serdes quaility info success---------------"
    #查询buffer的水线，重定向输出到roce.log
    echo -e "\n-----------TC buffer and PFC threshold---------------" >> roce.log
    for i in $(seq 0 $npu_nums); do echo "====> $i" >> roce.log;timeout 10s hccn_tool -i $i -tc_cfg -g &>> roce.log;done
    echo "-----------collect TC buffer and PFC threshold info success---------------"
    #查询tx收发数据，重定向输出到roce.log
    echo -e "\n-----------collect TX end receives and transmits data.---------------" >> roce.log
    for i in $(seq 0 $npu_nums); do echo "====> $i" >> roce.log;timeout 10s hccn_tool -i $i -tc_stat -g &>> roce.log;done
    echo "-----------collect TX end receives and transmits data info success---------------"
    #查询buffer最大值，重定向输出到roce.log
    echo -e "\n-----------Maximum size of the buffer---------------" >> roce.log
    for i in $(seq 0 $npu_nums); do
        echo "====> $i" >> roce.log
        for tc in 0 1 2 3; do
            timeout 10s hccn_tool -i $i -cur_tc_buf -g tc $tc &>> roce.log
        done
    done
    #查询pfc的数据包，重定向输出到roce.log
    echo -e "\n-----------collect pfc data packet---------------" >> roce.log
    for i in $(seq 0 $npu_nums); do echo "====> $i" >> roce.log;timeout 10s hccn_tool -i $i -pfc_stat -g &>> roce.log;done
    echo "-----------collect pfc data packet info success---------------"

    echo "-----------collect maximum size of the buffer info success---------------" 
    #查询网卡侧收发包状态，重定向输出到stat.log
    echo -e "\n-----------stat_extra---------------" >> stat.log
    for i in $(seq 0 $npu_nums); do echo "====> $i" >> stat.log;timeout 10s hccn_tool -i $i -stat_extra -g &>> stat.log;done
    echo "-----------collect stat info success---------------"

    # get_device_info_log
    #收集device os、host os的日志
    echo "-----------collect device info log start..."
    msnpureport -f | grep finish
    echo "-----------collect device info log success---------------"
    ##恢复imp、roce日志等级为error
    echo "" >> set_log_level.log
    for i in $(seq 0 $npu_nums);do msnpureport -m IMP:error -d $i &>> set_log_level.log;done;
    for i in $(seq 0 $npu_nums);do msnpureport -m ROCE:error -d $i &>> set_log_level.log;done;

    # get_os_info
    tail -20000 /var/log/messages  &>> host_messages.log
    tail -20000 /var/log/syslog &>> host_syslog.log
    #拷贝网口配置类信息
    cp /etc/hccn.conf ./
    #拷贝hccn_tool操作日志
    cp -rf /var/log/hccn_tool ./
    #拷贝ascend_seclog日志
    cp -rf /var/log/ascend_seclog ./
    #拷贝npu-smi相关日志
    mkdir nputools_log
    cp /var/log/nputools_LOG_* ./nputools_log/

    echo -e "----------- v1.70 collect completed ---------------"

    #返回上一层，打包日志记录
    cd ..
    echo -e "\n$(date) compressing files into a tar package"
    tar -zcvf "network_info_data_$current_time.tar.gz" ./network_info_data_$current_time | grep ok
    rm -rf ./network_info_data_$current_time
    echo "============ tar log info completed ============"
}

function get_path_env() {
    green_echo "$(date)================ Collect path_env ==============" | tee -a $LOG_COLLECT_RUNING_INFO
	echo "PATH=${PATH}"
	echo "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}"
	env > $LOG_FILE_PATH/env.log
    green_echo "$(date) End collect path_env" | tee -a $LOG_COLLECT_RUNING_INFO
}

function get_version_info() {
	date
	
	echo "--------------- cat drvier version.info ---------------"
	which npu-smi > /dev/null 2>&1
	if [ $? -eq 0 ];then
		timeout 20s npu-smi info >> ${FILE_VERSION_LOG}
	else
		echo "npu-smi no exist" >> ${FILE_VERSION_LOG}
	fi
	timeout 20s ${INSTALL_PATH}/driver/tools/upgrade-tool --device_index -1 --system_version >> ${FILE_VERSION_LOG}
	echo "--------------- firmware version ---------------"
	timeout 20s ${INSTALL_PATH}/driver/tools/upgrade-tool --device_index -1 --component -1 --version >> ${FILE_VERSION_LOG}
	bdf_list=$(lspci | egrep "d100|d500|d801|d802|d803" | awk '{print $1}') 
	for bdf in $bdf_list ;do lspci -vvvs $bdf -xxx | grep 4e0 >> ${FILE_VERSION_LOG};done
	for bdf in $bdf_list ;do lspci -vvvs $bdf -xxxx | grep 300  >> ${FILE_VERSION_LOG}; lspci -vvvs $bdf -xxxx | grep 320 >> ${FILE_VERSION_LOG}; lspci -vvvs $bdf -xxxx | grep 450 >> ${FILE_VERSION_LOG};done
	lspci | egrep "d100|d500|d801|d802|d803"  | awk '{print $1}' | xargs -i lspci -xxxx -s {} | grep 300 >> ${FILE_VERSION_LOG}
	lspci | egrep "d100|d500|d801|d802|d803"  | awk '{print $1}' | xargs -i lspci -xxxx -s {} | grep 320 >> ${FILE_VERSION_LOG}
	lspci | egrep "d100|d500|d801|d802|d803"  | awk '{print $1}' | xargs -i lspci -xxxx -s {} | grep 450 >> ${FILE_VERSION_LOG}
	echo "--------------- cat mcu version ---------------"
	for id in $(seq 0 7) ;do echo "====$id===="; npu-smi upgrade -b mcu -i $id >> ${FILE_VERSION_LOG}; done
	echo "--------------- cat vrd version ---------------"
	for id in $(seq 0 7) ;do echo "====$id===="; npu-smi upgrade -b vrd -i $id >> ${FILE_VERSION_LOG}; done
    green_echo "$(date) End collect driver version info" | tee -a $LOG_COLLECT_RUNING_INFO
}

function get_pcie_log() {
	green_echo "================ Collect PCIe log ==============" | tee -a $LOG_COLLECT_RUNING_INFO
	date >> ${FILE_PCIE_LOG}
	
    echo "--------------- lspci ---------------" 		>> ${FILE_PCIE_LOG}
	lspci | egrep "d100|d500|d801|d802|d803" 				>> ${FILE_PCIE_LOG}

	echo "--------------- bdf_to_devid ---------------" 					>> ${FILE_PCIE_LOG}
	bdf_list=($(lspci | egrep "d100|d500|d801|d802|d803" | awk '{print $1}'))
	cat /sys/bus/pci/devices/0000:${bdf_list[0]}/devdrv_sysfs_bdf_to_devid 	>> ${FILE_PCIE_LOG}
	bdf_list=$(lspci | egrep "d100|d500|d801|d802|d803" | awk '{print $1}') 
	
    echo "--------------- firmware version ---------------" 				>> ${FILE_PCIE_LOG}
	for bdf in $bdf_list ;do lspci -vvvs $bdf -xxx | grep 4e0;done			>> ${FILE_PCIE_LOG}
	for bdf in $bdf_list ;do lspci -vvvs $bdf -xxxx | grep 300 >> ${FILE_VERSION_LOG}; lspci -vvvs $bdf -xxxx | grep 320 >> ${FILE_VERSION_LOG}; lspci -vvvs $bdf -xxxx | grep 450 >> ${FILE_PCIE_LOG};done
	lspci | egrep "d100|d500|d801|d802|d803"  | awk '{print $1}' | xargs -i lspci -xxxx -s {} | grep 300 >> ${FILE_PCIE_LOG}
	lspci | egrep "d100|d500|d801|d802|d803"  | awk '{print $1}' | xargs -i lspci -xxxx -s {} | grep 320 >> ${FILE_PCIE_LOG}
	lspci | egrep "d100|d500|d801|d802|d803"  | awk '{print $1}' | xargs -i lspci -xxxx -s {} | grep 450 >> ${FILE_PCIE_LOG}

	echo "--------------- LnkSta ---------------" >> ${FILE_PCIE_LOG}
	date;lspci | egrep "d100|d500|d801|d802|d803" | awk '{print $1}'|xargs -i lspci -vvvvs {}|grep -E 'LnkSta:'  >> ${FILE_PCIE_LOG}
	
	echo "--------------- NUMA ---------------" >> ${FILE_PCIE_LOG}
	for bdf in $bdf_list ;do echo ====$bdf====;lspci -vvvs $bdf | egrep "Lnk|NUMA";done >> ${FILE_PCIE_LOG}
	for bdf in $bdf_list ;do echo ====$bdf====;lspci -vvvs $bdf | egrep "NUMA";done >> ${FILE_PCIE_LOG}

	echo "--------------- lspci -tv ---------------" >> ${FILE_PCIE_LOG}
	lspci -tv >> ${FILE_PCIE_LOG}
	echo "--------------- lspci -d 19e5:xxx -k ---------------" >> ${FILE_PCIE_LOG}
	lspci -d 19e5:d802|d803 -k >> ${FILE_PCIE_LOG}
	lspci -d 19e5:d801 -k >> ${FILE_PCIE_LOG}
	lspci -d 19e5:d500 -k >> ${FILE_PCIE_LOG}
	lspci -d 19e5:d100 -k >> ${FILE_PCIE_LOG}
    green_echo "$(date) End collect PCIe log" | tee -a $LOG_COLLECT_RUNING_INFO
}

function get_os_info() {
	green_echo "================ Collect os info =============="  | tee -a $LOG_COLLECT_RUNING_INFO
    echo "uname -a output:" >> "${FILE_OS_LOG}"
    uname -a >> "${FILE_OS_LOG}"
    echo "uname -r output:" >> "${FILE_OS_LOG}"
    uname -r >> "${FILE_OS_LOG}"
	cat /proc/cmdline >> "${FILE_OS_LOG}"
    hostnamectl	2>/dev/null >> "${FILE_OS_LOG}"
	for file in /etc/*release*; do
        if [ -f "$file" ]; then
            echo "Content of $file:" >> "${FILE_OS_LOG}"
            cat "$file" >> "${FILE_OS_LOG}" 2>> "${FILE_OS_LOG}"
        else
            echo "File $file not found." >> "${FILE_OS_LOG}"
        fi
    done
    for file in /etc/*version*; do
        if [ -f "$file" ]; then
            echo "Content of $file:" >> "${FILE_OS_LOG}"
            cat "$file" >> "${FILE_OS_LOG}" 2>> "${FILE_OS_LOG}"
        else
            echo "File $file not found." >> "${FILE_OS_LOG}"
        fi
    done
    free -g >> "${FILE_OS_LOG}"
	echo "--------------- meminfo ---------------" >> "${FILE_OS_LOG}"
	cat /proc/meminfo >> "${FILE_OS_LOG}"
	free -h >> "${FILE_OS_LOG}"
	echo "--------------- dmidecode ---------------" >> "${FILE_OS_LOG}"
	dmidecode -t slot >> "${FILE_OS_LOG}"
	echo "--------------- ps -ef ---------------" >> "${FILE_OS_LOG}"
	ps -ef >> "${FILE_OS_LOG}"
	echo "--------------- ls -l /boot ---------------" >> "${FILE_OS_LOG}"
	ls -l /boot >> "${FILE_OS_LOG}"
	echo "--------------- cpuinfo ---------------" >> "${FILE_OS_LOG}"
	cat /proc/cpuinfo >> "${FILE_OS_LOG}"
	echo "--------------- interrupts ---------------" >> "${FILE_OS_LOG}"
	cat /proc/interrupts >> "${FILE_OS_LOG}"
    echo "--------------- cmdline ---------------" >> "${FILE_OS_LOG}"
	cat /proc/cmdline >> "${FILE_OS_LOG}"
	echo "--------------- last reboot ---------------" >> "${FILE_OS_LOG}"
	last reboot >> "${FILE_OS_LOG}"
	echo "--------------- lsmod ko info ---------------" >> "${FILE_OS_LOG}"
	lsmod >> "${FILE_OS_LOG}"
	cd ${LOG_FILE_PATH}
	mkdir -p ./mod_info
	cd ./mod_info
	lsmod > lsmod.log
	cp /lib/modules/`uname -r`/modules* ./
	cp -rn /lib/modules/`uname -r`/updates/ ./
	cd ${TOP_PATH}
    green_echo "$(date) End collect os info" | tee -a $LOG_COLLECT_RUNING_INFO
}

function get_qemu_log() {
	green_echo "================ Collect qemu log ==============" | tee -a $LOG_COLLECT_RUNING_INFO
	mkdir -p ${LOG_FILE_PATH}/qemu_log
	cp -rf /var/log/libvirt/qemu ${LOG_FILE_PATH}/qemu_log 2>/dev/null
    green_echo "$(date) end collect qemu_log" | tee -a $LOG_COLLECT_RUNING_INFO
}

function get_driver_log() {
	green_echo "================ Collect driver log ==============" | tee -a $LOG_COLLECT_RUNING_INFO
	cp -rf /var/log/npu "${LOG_FILE_PATH}" > /dev/null 2>&1 
	green_echo "================ Collect plog log ==============" | tee -a $LOG_COLLECT_RUNING_INFO
	cp -rf ~/ascend "${LOG_FILE_PATH}" > /dev/null 2>&1
    green_echo "$(date) End collect driver log" | tee -a $LOG_COLLECT_RUNING_INFO
}

function get_device_log() {
	cd ${LOG_FILE_PATH}
	green_echo "================ Collect device log ==============" | tee -a $LOG_COLLECT_RUNING_INFO
	msnpureport -f
    for d in 0 1 2 3 4 5 6 7; do
        msnpureport -r -d $d 
    done
	cd ${TOP_PATH}
    green_echo "$(date) End collect device log" | tee -a $LOG_COLLECT_RUNING_INFO
}

function green_echo () {
        local what=$*
        echo -e "\e[1;32m ${what} \e[0m"
}

if [ $(id -u) -ne 0 ]; then
    echo "Error: Please run $(basename $0) as root."
    exit 1
fi

check_run_in_vm
if [ $? -eq 1 ]; then
    echo "Error: Please run $(basename $0) on physical machine."
    exit 1
fi

check_run_in_docker
if [ $? -eq 1 ]; then
    echo "Error: Please run $(basename $0) on physical machine."
    exit 1
fi

conflict_check
parse_args "$@"
if [ "$main_flag" -eq 1 ]; then
    main "$@"
fi