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

TOP_DIR=$(pwd)
LOG_DIR_NAME="ascend_log"
LOG_DIR="$TOP_DIR/$LOG_DIR_NAME"
RUNNING_LOG="script_running.log"

TARGET_FILE_NAME="ascend_log.tar.gz"
PCI_BASE_PATH="/sys/bus/pci/devices"
INSTALL_PATH="/usr/local/Ascend"

# 0x0001: 310, 0x0010: 310p, 0x0100: 910, 0x1000: 910B
CHIP_TYPE=0
PCIE_BDF_310="19e5:d100"
PCIE_BDF_310P="19e5:d500"
PCIE_BDF_910="19e5:d801"
PCIE_BDF_910B="19e5:d802"
PCIE_BDF_910_93="19e5:d803"
DEVICE_LIST="0"
MAX_NETWORK_ID=7

HOST_INFO_DIR="host_info"
DRIVER_INFO_DIR="driver_info"
HOST_LOG_DIR="host_log"
DUMP_DIR="device_log"
INSTALL_INFO_DIR="install_info"
NETWORK_INFO_DIR="network_info"

HOST_INFO_MODE=1
HOST_LOG_MODE=1
DEVICE_LOG_MODE=1
DRIVER_INFO_MODE=1
INSTALL_INFO_MODE=1
NETWORK_INFO_MODE=1

CHIP_NUM_910=$(lspci | grep -E "d801" | wc -l)
CHIP_NUM_910B=$(lspci | grep -E "d802" | wc -l)
CHIP_NUM_910_93=$(lspci | grep -E "d803" | wc -l)

#additional functions in this list
g_func_list=(
            ""
            )

usage() {
    echo "Usage: $(basename $0)"
    echo "  or   $(basename $0) [OPTION]"
    echo "Example:"
    echo "$(basename $0)                    Collect the default content."
    echo "$(basename $0) -p /home/tmp       Specify a path to store the generated content."
    echo "$(basename $0) --off host_info    Disable host os info."
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

set_off_mode() {
    case $1 in
        host_info )
            HOST_INFO_MODE=0
            ;;
        host_log )
            HOST_LOG_MODE=0
            ;;
        device_log )
            DEVICE_LOG_MODE=0
            ;;
        driver_info )
            DRIVER_INFO_MODE=0
            ;;
        install_info )
            INSTALL_INFO_MODE=0
            ;;
        network_info )
            NETWORK_INFO_MODE=0
            ;;
        * )
            usage
            return 1
    esac
}

parse_args() {
    while [ "$1" != "" ]; do
        case "$1" in
            -p | --path )
                if [ -z "$2" ]; then
                    usage
                    exit 1
                elif [ ! -d "$2" ]; then
                    echo "Warning: The specified path is inexistent, this tool will create the directory." 
                    mkdir -p "$2"
                fi
                cd "$2"
                TOP_DIR=$(pwd)
                shift
                ;;
            --off )
            if [ -z "$2" ]; then
                usage
                exit 1
            fi
            set_off_mode "$2"
            shift
            ;;
            -h | --help )
                usage
                exit
                ;;
            * )
                usage
                exit 1
        esac
        shift
    done
}

show_info() {
    echo
    echo "You are now running "$(basename $0)". This is a tool which will collect necessery information"
    echo "about your system, Linux kernel and ascend kernel module. This will help our support team diagnose"
    echo "the issue you are facing while using the product. The infomation collection process may take several"
    echo "minutes and it will generate the tar file named with timestamp under the assigned directory. Please"
    echo "download and send the file to the support team, thanks."
    echo
}

create_log_path() {
    START_TIME=$(date +"%Y-%m-%d %H:%M:%S")
    echo "Start time: $START_TIME"

    DIR_DATE=$(echo $START_TIME | tr -cd "[0-9]")
    LOG_DIR_NAME="ascend_log_$DIR_DATE"
    TARGET_FILE_NAME="ascend_log_$DIR_DATE.tar.gz"

    LOG_DIR=$TOP_DIR/$LOG_DIR_NAME
    mkdir -p $LOG_DIR 2> /dev/null
    if [ ! -d $LOG_DIR ]; then
        echo "ERROR: The assigned directory is not writable. please cd to a directory"
        echo "       where you have write permission so that the $LOG_DIR_NAME dir"
        echo "       can be created."
        echo
        abnormal_exit
        exit 1
    fi
}

save_running_log() {
    touch $TOP_DIR/$RUNNING_LOG
    chmod 640 $TOP_DIR/$RUNNING_LOG
    which tee > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        exec &> >(tee $TOP_DIR/$RUNNING_LOG)
    else
        echo "There is not tee in current evnironment." >> $TOP_DIR/$RUNNING_LOG
    fi

}

get_chip_type() {
    CHIP_NUM_310=$(lspci | grep d100 | wc -l)
    if [ $CHIP_NUM_310 -ne 0 ]; then
        let "CHIP_TYPE=$CHIP_TYPE|1"
        return 0
    fi

    CHIP_NUM_310P=$(lspci | grep d500 | wc -l)
    if [ $CHIP_NUM_310P -ne 0 ]; then
        let "CHIP_TYPE=$CHIP_TYPE|2"
        return 0
    fi

    CHIP_NUM_910=$(lspci | grep d801 | wc -l)
    if [ $CHIP_NUM_910 -ne 0 ]; then
        let "CHIP_TYPE=$CHIP_TYPE|4"
        return 0
    fi

    CHIP_NUM_910B=$(lspci | grep d802 | wc -l)
    if [ $CHIP_NUM_910B -ne 0 ]; then
        let "CHIP_TYPE=$CHIP_TYPE|8"
        return 0
    fi

    CHIP_NUM_910_93=$(lspci | grep d803 | wc -l)
    if [ $CHIP_NUM_910_93 -ne 0 ]; then
        let "CHIP_TYPE=$CHIP_TYPE|16"
        return 0
    fi
}

get_install_variable() {
    INSTALL_PATH=$(cat /lib/davinci.conf | awk -F '=' '{print $2}' | head -1)
    DEVICE_LIST=$($INSTALL_PATH/driver/tools/upgrade-tool --mini_devices | tr -cs "[0-9]" " ")
    DEVICE_BDFS_310=$(lspci -D -d $PCIE_BDF_310 | awk '{print $1}')
    DEVICE_BDFS_310P=$(lspci -D -d $PCIE_BDF_310P | awk '{print $1}')
    DEVICE_BDFS_910=$(lspci -D -d $PCIE_BDF_910 | awk '{print $1}')
    DEVICE_BDFS_910B=$(lspci -D -d $PCIE_BDF_910B | awk '{print $1}')
    DEVICE_BDFS_910_93=$(lspci -D -d $PCIE_BDF_910_93 | awk '{print $1}')
}

show_title() {
    echo
    if [ $1 -eq 1 ]; then
        echo "# $2"
    elif [ $1 -eq 2 ]; then
        echo "## $2"
    elif [ $1 -eq 3 ]; then
        echo "### $2"
    fi
    echo "---"
}

get_os_info() {
    show_title 1 "PATH"
    echo "\`\`\`"
    echo "PATH=${PATH}"
    echo "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}"
    echo "\`\`\`"

    show_title 1 "OS info"
    echo "\`\`\`"
    uname -a
    uname -r
    cat /etc/*release* 2> /dev/null
    cat /etc/*version* 2> /dev/null
    echo "\`\`\`"

    show_title 1 "last reboot"
    echo "\`\`\`"
    last reboot
    echo "\`\`\`"

    which dmidecode > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        show_title 1 "dmidecode"
        echo "\`\`\`"
        dmidecode -t slot
        echo "\`\`\`"
    fi

    show_title 1 "ps"
    echo "\`\`\`"
    ps -ef
    echo "\`\`\`"

    show_title 1 "/boot"
    echo "\`\`\`"
    ls -l /boot
    echo "\`\`\`"

    cp /proc/meminfo $LOG_DIR/$HOST_INFO_DIR/
    cp /proc/cpuinfo $LOG_DIR/$HOST_INFO_DIR/
    cp /proc/interrupts $LOG_DIR/$HOST_INFO_DIR/
    cp /proc/buddyinfo $LOG_DIR/$HOST_INFO_DIR/
}

get_pcie_info() {
    show_title 1 "ascend chip"
    echo "\`\`\`"
    let "tmp=$CHIP_TYPE&1"
    if [ $tmp -ne 0 ]; then
        echo "Asecnd310 nums: $CHIP_NUM_310"
        lspci | grep d100
    fi

    let "tmp=$CHIP_TYPE&2"
    if [ $tmp -ne 0 ]; then
        echo "Asecnd310P nums: $CHIP_NUM_310P"
        lspci | grep d500
    fi

    let "tmp=$CHIP_TYPE&4"
    if [ $tmp -ne 0 ]; then
        echo "Asecnd910 nums: $CHIP_NUM_910"
        lspci | grep d801
    fi

    let "tmp=$CHIP_TYPE&8"
    if [ $tmp -ne 0 ]; then
        echo "Asecnd910B nums: $CHIP_NUM_910B"
        lspci | grep d802
    fi

    let "tmp=$CHIP_TYPE&16"
    if [ $tmp -ne 0 ]; then
        echo "Asecnd910_93 nums: $CHIP_NUM_910_93"
        lspci | grep d803
    fi
    echo "\`\`\`"

    lspci -tv >> $LOG_DIR/$HOST_INFO_DIR/pcie_topology.log 2> /dev/null
    lspci -vvv >> $LOG_DIR/$HOST_INFO_DIR/pcie_config.log 2> /dev/null
}

get_host_info() {
    if [ $HOST_INFO_MODE -eq 0 ]; then
        return 0
    fi

    echo "Collecting host info..."
    mkdir -p $LOG_DIR/$HOST_INFO_DIR
    cd $LOG_DIR/$HOST_INFO_DIR
    (
        echo [toc]
        get_pcie_info
        get_os_info
    ) >> env_info.md
}

get_host_log() {
    if [ $HOST_LOG_MODE -eq 0 ]; then
        return 0
    fi

    echo "Collecting host log..."
    mkdir -p $LOG_DIR/$HOST_LOG_DIR
    cd $LOG_DIR/$HOST_LOG_DIR

    cp -rf /var/log/messages* ./ 2> /dev/null
    cp -rf /var/log/kern.log* ./ 2> /dev/null
    cp -rf /var/log/syslog* ./ 2> /dev/null
    dmesg >> dmesg.log
    dmesg -T >> dmesg_T.log 
}

get_device_log() {
    if [ $DEVICE_LOG_MODE -eq 0 ]; then
        return 0
    fi

    which msnpureport > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo "Collecting device log..."
        mkdir -p $LOG_DIR/$DUMP_DIR
        cd $LOG_DIR/$DUMP_DIR
        msnpureport -f >> npureportlog.log
        msnpureport -t 5 >> npureportlog1.log
    fi
}

get_install_info() {
    if [ $INSTALL_INFO_MODE -eq 0 ]; then
        return 0
    fi

    echo "Collecting install info..."
    mkdir -p $LOG_DIR/$INSTALL_INFO_DIR
    cd $LOG_DIR/$INSTALL_INFO_DIR

    cp -rf /var/log/ascend_seclog ./ > /dev/null 2>&1
    cp -rf /var/log/upgradetool ./ > /dev/null 2>&1

    (
        echo "[toc]"
        show_title 1 "install path"
        echo "\`\`\`"
        cat /lib/davinci.conf
        echo "\`\`\`"

        show_title 1 "driver info"
        echo "\`\`\`"
        cat /etc/ascend_install.info
        cat /etc/ascend_filelist.info 2>&1
        echo "\`\`\`"

        show_title 1 "driver version"
        echo "\`\`\`"
        $INSTALL_PATH/driver/tools/upgrade-tool --device_index -1 --system_version
        echo "\`\`\`"

        echo "\`\`\`"
        $INSTALL_PATH/driver/tools/upgrade-tool --device_index -1 --component -1 --version
        echo "\`\`\`"

        show_title 1 "module info"
        echo "\`\`\`"
        lsmod | grep drv
        echo "\`\`\`"
        MODULES=$(lsmod | grep drv | awk '{print $1}')
        for i in $MODULES
        do
            echo "\`\`\`"
            modinfo $i 2> /dev/null
            echo "\`\`\`"
        done
    ) >> install_info.md
}

get_file_content() {
    if [ $1 = "config" ]; then
        echo "\`\`\`"
        echo "file: $1"
        which hexdump > /dev/null 2>&1 && hexdump $1
        echo "\`\`\`"
        echo
        return 0
    fi

    TMP_STR=$(cat $1 2> /dev/null)
    if [ $? -eq 0 ]; then
        echo "\`\`\`"
        echo "file: $1"
        echo "$TMP_STR"
        echo "\`\`\`"
        echo
    fi
}

get_path_dir() {
    if [ -L $1 ]; then
        return 0
    fi

    echo "## $1"
    cd $1
    for i in $(ls)
    do
        if [ -f $i ] && [ -r $i ]; then
            get_file_content $i
        fi
    done
    echo
    cd ../
}

get_sysfs_info() {
    echo "[toc]"
    DEVICE_BDFS=${DEVICE_BDFS_310}${DEVICE_BDFS_310P}${DEVICE_BDFS_910}${DEVICE_BDFS_910B}${DEVICE_BDFS_910_93}
    for i in $DEVICE_BDFS
    do
        show_title 1 "pci_bdf --> $i"
        cd $PCI_BASE_PATH/$i
        for j in $(ls)
        do
            if [ -f $j ] && [ -r $j ]; then
                get_file_content $j
            fi
        done
        cd $PCI_BASE_PATH/$i
        for j in $(ls)
        do
            if [ -d $j ]; then
                get_path_dir $j
            fi
        done
    done
}

get_driver_info() {
    if [ $DRIVER_INFO_MODE -eq 0 ]; then
        return 0
    fi

    echo "Collecting driver info..."
    mkdir -p $LOG_DIR/$DRIVER_INFO_DIR
    get_sysfs_info >> $LOG_DIR/$DRIVER_INFO_DIR/driver_info.md
}

run_hccn_tool_get() {
    echo "\`\`\`"
    echo "cmd: hccn_tool -i $1 -$2 -g"
    hccn_tool -i $1 -$2 -g
    echo "\`\`\`"
}

get_hccn_tool_info() {
    HCCN_TOOL_GET_ITEM="ip gateway netdetect lldp mac link dscp_to_tc shaping mtu net_health arp route pci reg
        optical stat tls version vnic process udp speed duplex backpressure serdes fec pcs ip_rule ip_route"

    for i in $HCCN_TOOL_GET_ITEM
    do
        run_hccn_tool_get $1 $i >> network.md 2>&1
    done
}

get_device_network_info() {
    echo [toc]
    show_title 1 "network link info"
    echo "\`\`\`"
    for i in $(seq 0 $MAX_NETWORK_ID)
    do
        echo "hccn_tool -i $i -link -g"
        hccn_tool -i $i -link -g >> network.md 2>&1
        echo
    done
    echo "\`\`\`"

    for i in $(seq 0 $MAX_NETWORK_ID)
    do
        hccn_tool -i $i -link -g 2> /dev/null > /dev/null 2>&1
        
        show_title 1 "devid: $i"
        get_hccn_tool_info $i
        
    done
}

get_build_os_info() {
    #查看驱动构建时间
    echo -e "\n--------------------- v1.70 ---------------------" >> build_info.log
    echo -e "\n-----------build info---------------" >> build_info.log
    cat /usr/local/Ascend/driver/build.info >> build_info.log
    echo -e "\n-----------system up time---------------" >> build_info.log
    who -b >> build_info.log
    echo -e "\n-----------local ip info---------------" >> ifconfig.log
    ifconfig >> ifconfig.log

    #获取os信息
    echo -e "\n-----------OS info---------------" >> npu-smi.log
    cat /etc/os-release &>> npu-smi.log
    cat /etc/centos-release &>> npu-smi.log
    echo "-----------collect OS info info success---------------"
    cat /usr/local/Ascend/firmware/version.info &>> npu-smi.log
    echo "-----------collect npu-smi info success---------------"
}

get_A2A3_network_info() {
    if [ $NETWORK_INFO_MODE -eq 0 ] || [ $CHIP_NUM_910 -eq 0 ]; then
        return 0
    fi

    product=$CHIP_NUM_910B
    product_803=$CHIP_NUM_910_93
    #查询NPU卡的数量， 默认A+X 16张卡，A+K 8张卡
    if [ $product -ne 0 ];then
        npu_nums=$((8+$( uname -a | grep x86 | wc -l )*8));
    elif [ $product_803 -ne 0 ];then
        npu_nums=16
    else
        exit
    fi
    npu_nums=$((npu_nums - 1))

    #创建文件夹并进入
    current_time=$(date +%Y%m%d%H%M%S)
    mkdir -p $LOG_DIR/network_info_data_$current_time
    cd $LOG_DIR/network_info_data_$current_time

    # get_device_error_log
    # 开info前收集一次日志
    ##开启所有卡的imp、roce的info等级日志
    echo "$(date)-----------collect device error log start..."
    (msnpureport -f | grep "Export finished." ;
    for i in $(seq 0 $npu_nums);do msnpureport -m IMP:info -d $i &>> set_log_level.log;done;
    for i in $(seq 0 $npu_nums);do msnpureport -m ROCE:info -d $i &>> set_log_level.log;done;)&
    echo "-----------collect device error log success---------------"

    get_build_os_info

    echo -e "\n-----------link stat---------------" >> extra_network.log
    for i in $(seq 0 $npu_nums);do 
        echo "====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -link_stat -g &>> extra_network.log; 
        echo -e "\n-----------optical reg info---------------" >> extra_network.log
        echo "====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -optical -dump all &>> extra_network.log; 
        echo "-----------collect optical reg info success---------------"
        echo -e "\n-----------optical v2---------------" >> extra_network.log
        echo "====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -optical -g v2 &>> extra_network.log; 

        # get_reg_info
        #查询所有卡的pcs link状态，重定向输出到extra_network.log
        echo -e "\n-----------pcs link---------------" >> extra_network.log
        echo "====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -reg -a 0x106040f0 &>> extra_network.log; 
        echo "-----------collect pcs link info success---------------"
        #查询所有卡的mac link状态，重定向输出到extra_network.log
        echo -e "\n-----------mac link---------------" >> extra_network.log
        echo "====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -reg -a 0x10600420 &>> extra_network.log; 
        echo "-----------collect mac link info success---------------"
        #查询所有卡是否有本端错误，重定向输出到extra_network.log
        echo -e "\n-----------rf lf---------------" >> extra_network.log
        echo "====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -reg -a 0x10600460 &>> extra_network.log; 
        echo "-----------collect reg info success---------------"

        # get_cdr_info
        #dump cdr寄存器
        echo -e "\n-----------scdr snr 1 times---------------" >> extra_network.log
        echo "====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -scdr -t 4 &>> extra_network.log; 
        #查询cdr snr是否有异常，重定向输出到extra_network.log
        echo -e "\n-----------scdr snr 1 times---------------" >> extra_network.log
        echo "====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -scdr -t 5 &>> extra_network.log; 
        echo -e "\n-----------scdr snr 2 times---------------" >> extra_network.log
        echo "====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -scdr -t 5 &>> extra_network.log; 
        echo -e "\n-----------scdr snr 3 times---------------" >> extra_network.log
        echo "====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -scdr -t 5 &>> extra_network.log; 
        echo "-----------scdr snr success---------------"

        #查询hilink serdes info，重定向输出到extra_network.log
        echo -e "\n-----------hilink stable register---------------" >> extra_network.log
        echo -e "\n\n\n====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -hilink_info -t 0 &>> extra_network.log; 
        echo "-----------collect hilink info success---------------"
        #查询hilink serdes info，重定向输出到extra_network.log
        echo -e "\n-----------link down hilink register---------------" >> extra_network.log
        echo -e "\n\n\n====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -hilink_info -t 1 &>> extra_network.log; 
        echo "-----------collect serdes info success---------------"

        #查询serdes的信号质量，重定向输出到extra_network.log
        echo -e "\n-----------serdes quaility---------------" >> extra_network.log
        echo "====> $i" >> extra_network.log; ascend-dmi --sq -t roce -d $i &>> extra_network.log; 

        echo -e "\n-----------TC buffer and PFC threshold---------------" >> extra_network.log
        echo "====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -tc_cfg -g &>> extra_network.log; 
        echo "-----------collect TC buffer and PFC threshold info success---------------"
        #查询tx收发数据，重定向输出到extra_network.log
        echo -e "\n-----------collect TX end receives and transmits data.---------------" >> extra_network.log
        echo "====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -tc_stat -g &>> extra_network.log; 
        echo "-----------collect TX end receives and transmits data info success---------------"

        #查询buffer最大值，重定向输出到extra_network.log
        echo -e "\n-----------Maximum size of the buffer---------------" >> extra_network.log
        
        echo "====> $i" >> extra_network.log
        for tc in 0 1 2 3; do
            timeout 10s hccn_tool -i $i -cur_tc_buf -g tc $tc &>> extra_network.log
        done
        
        #查询pfc的数据包，重定向输出到extra_network.log
        echo -e "\n-----------collect pfc data packet---------------" >> extra_network.log
        echo "====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -pfc_stat -g &>> extra_network.log;
        echo "-----------collect pfc data packet info success---------------"

        echo "-----------collect maximum size of the buffer info success---------------" 
        #查询网卡侧收发包状态，重定向输出到extra_network.log
        echo -e "\n-----------stat_extra---------------" >> extra_network.log
        echo "====> $i" >> extra_network.log;timeout 10s hccn_tool -i $i -stat_extra -g &>> extra_network.log;
        echo "-----------collect stat info success---------------"
    done

    which hccn_tool > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo "Collecting network info..."
        mkdir -p $LOG_DIR/$NETWORK_INFO_DIR
        cd $LOG_DIR/$NETWORK_INFO_DIR
        get_device_network_info >> network.md
    fi

    # get_device_info_log
    #收集device os、host os的日志
    cd $LOG_DIR/network_info_data_$current_time
    echo "-----------collect device info log start..."
    msnpureport -f | grep finish
    echo "-----------collect device info log success---------------"
    ##恢复imp、roce日志等级为error
    echo "" >> set_log_level.log
    for i in $(seq 0 $npu_nums);do msnpureport -m IMP:error -d $i &>> set_log_level.log;done;
    for i in $(seq 0 $npu_nums);do msnpureport -m ROCE:error -d $i &>> set_log_level.log;done;

    #拷贝网口配置类信息
    cp /etc/hccn.conf ./
    #拷贝hccn_tool操作日志
    cp -rf /var/log/hccn_tool ./
}

get_additional_info() {
    cd $LOG_DIR
    for i in ${g_func_list[*]}
    do
        $i
    done
}

collect_init() {
    parse_args
    show_info
    create_log_path
    save_running_log
    get_chip_type
    get_install_variable
}

collect_exit() {
    cd $TOP_DIR
    cp $RUNNING_LOG $LOG_DIR/
    chmod -R 400 $LOG_DIR
    tar -zcPf $TARGET_FILE_NAME $LOG_DIR_NAME
    chmod 640 $TARGET_FILE_NAME
    rm -f $RUNNING_LOG
    rm -rf $LOG_DIR
    echo "Finish."
    echo "End time: $(date +"%Y-%m-%d %H:%M:%S")"
}

abnormal_exit() {
    rm -f $TOP_DIR/$RUNNING_LOG
    echo "End time: $(date +"%Y-%m-%d %H:%M:%S")"
}

main() {
    collect_init
    get_host_info
    get_host_log
    get_device_log
    get_install_info
    get_driver_info
    get_A2A3_network_info
    get_additional_info
    collect_exit
}

if [ `id -u` -ne 0 ]; then
    echo "ERROR: Please run $(basename $0) as root."
    exit 1
fi

parse_args "$@"
main
