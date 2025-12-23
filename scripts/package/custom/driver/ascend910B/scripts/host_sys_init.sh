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

### BEGIN INIT INFO
# chkconfig: 123456 80 90
# Provides:        host_sys_init
# Required-Start:    $local_fs  $remote_fs
# Required-Stop:    $local_fs  $remote_fs
# Default-Start:    2 3 4 5
# Default-Stop:        0 1 6
# Short-Description:    Set up davinci servers
# Description:
### END INIT INFO
ASCEND_SECLOG="/var/log/ascend_seclog"
log_name="${ASCEND_SECLOG}/ascend_run_servers.log"
file_info_name="${ASCEND_SECLOG}/ascend_run_file.info"
installInfo="/etc/ascend_install.info"
targetdir=/usr/local/Ascend/driver
ASCEND_DRIVER_CFG_FILE="/etc/ascend_driver.conf"
ASCEND_DRIVER_SETUP_SCRIPT="ascend_driver_config.sh"
KMSAGENT_CONFIG_ITEM="kmsagent"

[ ! -d "${ASCEND_SECLOG}" ] && mkdir -m 750 -p "${ASCEND_SECLOG}"
touch $log_name $file_info_name

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
Driver_Install_For_All=$(getInstallParam "Driver_Install_For_All" "${installInfo}")
# read Driver_Install_Path_Param from installInfo
Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
# read Driver_Install_Mode Param from installInfo
Driver_Install_Mode=$(getInstallParam "Driver_Install_Mode" "${installInfo}")
# read Driver_Install_Type from installInfo
Driver_Install_Type=$(getInstallParam "Driver_Install_Type" "${installInfo}")

targetdir="${Driver_Install_Path_Param}"
targetdir="${targetdir%*/}"
username="$UserName"
usergroup="$UserGroup"
if [ "$username" = "" ]; then
    username=HwHiAiUser
    usergroup=HwHiAiUser
fi

setFileChmod() {
    local option="$1"
    local mod="$2"
    local file_path="$3"

    if [ "${Driver_Install_For_All}" == "yes" ]; then
        local mod=${mod:0:-1}${mod: -2:1}
    fi

    chmod ${option} "${mod}" "${file_path}" >& /dev/null
    if [ $? -ne 0 ]; then
        echo "Error: ${mod} $file_path chmod failed!"
        exit 1
    fi
}

startMode(){
    if [ -d "${Driver_Install_Path_Param}" ]; then
        setFileChmod -f 640 $log_name
    fi
}

endMode(){
    if [ -d "${Driver_Install_Path_Param}" ]; then
        setFileChmod -f 440 $log_name
        setFileChmod -f 640 $file_info_name
    fi
}

add_log(){
    echo "$(date) $*" >> $log_name
}

servers_stop () {
    return 0
}

get_pci_info() {
    local bdf="" i=0

    g_dev_linked_nums=0
    g_bdfs=()
    source "${targetdir}"/driver/script/specific_func.inc

    for bdf in $(ls /sys/bus/pci/devices/)
    do
        cat /sys/bus/pci/devices/${bdf}/vendor 2>/dev/null | grep -Ewq "${vendor_bdf}" || continue
        cat /sys/bus/pci/devices/${bdf}/device 2>/dev/null | grep -Ewq "${device_bdf}" || continue
        [ -e /sys/bus/pci/devices/${bdf}/physfn ] && continue
        # 0x0110: it means that the chip is recognized as 1 on the OS PCI bus but 2 on the device management driver.
        cat /sys/bus/pci/devices/${bdf}/subsystem_device 2>/dev/null | grep -wq "0x0110" && g_dev_linked_nums=$(expr ${g_dev_linked_nums} + 1)
        g_dev_linked_nums=$(expr ${g_dev_linked_nums} + 1)
        # to get all bdfs
        g_bdfs[${i}]="${bdf}"
        i=$(expr ${i} + 1)
    done

    return 0
}

changeDavinciModeInit() {
    g_change_mode_dev_list=" /dev/hisi_hdc /dev/davinci_manager /dev/devmm_svm /dev/lqdcmi_pcidev "
    g_online_dev_num=0
    get_pci_info
}
changeDavinciModeOnce() {
    for char_dev in $g_change_mode_dev_list; do
        if [ -e $char_dev ]; then
            udevadm settle --timeout=10
            if [ $? -ne 0 ]; then
                echo "Warning: Udev device scanning timed out." >> $log_name
            fi
            setFileChmod -f 660 $char_dev
            chown -f $username:$usergroup $char_dev
            add_log "char dev: $char_dev exist and set file mode owner done."
            g_change_mode_dev_list=${g_change_mode_dev_list/ $char_dev / }
        elif [ "$char_dev" == "/dev/lqdcmi_pcidev" ]; then
            # 只有在 /dev/lqdcmi_pcidev 不存在时，才将其从列表中移除
            add_log "char dev: $char_dev does not exist and has been removed from the list."
            g_change_mode_dev_list=${g_change_mode_dev_list/ $char_dev / }
        fi
    done

    local dev_num=$(ls /dev/davinci[0-9]* 2>/dev/null|wc -l)
    [ $dev_num -eq $g_online_dev_num ] && return
    for i in $(ls /dev/davinci[0-9]*)
    do
        setFileChmod -f 660 $i
        chown -f $username:$usergroup $i
        cat $i >> /dev/null 2>&1
    done
    add_log "online davinci num: $g_online_dev_num to $dev_num."
    g_online_dev_num=$dev_num
}
changeDavinciModeIsComplete() {
    [ "${g_change_mode_dev_list// }" = "" ] || return 1
    [ $g_online_dev_num -eq $g_dev_linked_nums ] || return 1

    local mod_correct_num=0
    local davinci_install_mod=660
    local effective_mod=0
    local effective_username="root"
    local effective_usergroup="root"
    if [ "${Driver_Install_For_All}" == "yes" ]; then
        davinci_install_mod=666
    fi
    for j in $(ls /dev/davinci* 2>/dev/null | grep -E "/dev/davinci[0-9]{1,2}$")
    do
        # open /dev/davinci[0-63] for uda to set default davinci's uid/gid/mode.
        effective_mod=$(stat -c %a ${j})
        effective_username="$(stat -c %U ${j})"
        effective_usergroup="$(stat -c %G ${j})"
        [ ${effective_mod} -eq ${davinci_install_mod} -a \
            "${effective_username}" = "${username}" -a \
            "${effective_usergroup}" = "${usergroup}" ] && mod_correct_num=$(expr $mod_correct_num + 1)
    done
    if [ ${mod_correct_num} -ne ${g_dev_linked_nums} ]; then
        g_online_dev_num=0
        return 1
    fi

    add_log "change mode complete, online_dev_num: $g_online_dev_num."

    timeout 20s "${targetdir}"/driver/tools/upgrade-tool --mini_devices >> $log_name
    if [ $? -ne 0 ]; then
        add_log "upgrade-tool get device count failed"
    else
        add_log "upgrade-tool get device count success"
    fi
    udevadm trigger >> /dev/null 2>&1
    return 0
}
changeDavinciModeBG() {
    sleep 1
    add_log "bg proc in: online_dev_num: $g_online_dev_num, left_devs: $g_change_mode_dev_list"
    for wait_cnt in {1..110};do
        changeDavinciModeOnce
        sleep 1
        changeDavinciModeIsComplete && break
        sleep 1
    done
    add_log "bg proc out: online_dev_num: $g_online_dev_num, left_devs: $g_change_mode_dev_list"
    endMode
}
changeDavinciMode() {
    add_log "change davinci mode begin."
    changeDavinciModeInit
    for wait_cnt in {1..290};do
        changeDavinciModeOnce
        sleep 1
        changeDavinciModeIsComplete && return
        sleep 1
    done
    add_log "start background proc change mode."
    g_change_mode_background=1
    changeDavinciModeBG &
    servers_start_ret=2
}

changeDvppCmdlistMode() {
    add_log "change dvpp_cmdlist mode begin."
    chmod -f 660 /dev/dvpp_cmdlist
    chown -f ${username}:${usergroup} /dev/dvpp_cmdlist
    add_log "change dvpp_cmdlist mode end."
}

clean_servers_log() {
    echo >$log_name
}

servers_log() {
    dmesg >>$log_name
    lsmod >>$log_name
    ps -ef  >>$log_name
}

run_file_info() {
    ls -lR "$1" >$file_info_name
}

set_sock_option() {
        SLOGD_SIZE_2M=2097152
        SLOGD_DGRAM_QLEN=51200
        sysctl -w net.core.wmem_max=$SLOGD_SIZE_2M >/dev/null 2>&1
        sysctl -w net.unix.max_dgram_qlen=$SLOGD_DGRAM_QLEN >/dev/null 2>&1
}

open_debug_switch(){
    if ! timeout 30 bash -c "
        while true; do
            if [ -f "/proc/debug_switch" ];then
                echo 1 > /proc/debug_switch
                break
            fi
            sleep 2
        done
    " 1 > /dev/null 2>&1; then
        echo "[debug_switch][WARNING]debug_switch auto open failed after reboot,please open manually" > /dev/kmsg
    else
        echo "[debug_switch][INFO]debug_switch auto open" > /dev/kmsg
    fi
}

clean_white_proc_cfg() {
    local FILE="/etc/custom_process.cfg"
    local isA2_driver
    local isA3_driver

    source "${targetdir}"/driver/script/specific_func.inc

    isA2_driver=$(echo $device_bdf | grep d802)
    isA3_driver=$(echo $device_bdf | grep d803)
    if [[ -z "$isA2_driver" && -z "$isA3_driver" ]]; then
        log "[INFO] this type driver need not clean white process config"
        return
    fi

    if [[ "$IS_CALLED_ON_INSTALLING" != "true" ]]; then
        if [[ -f "$FILE" ]]; then
            : > /etc/custom_process.cfg
        fi
    fi
}

servers_start() {
    clean_servers_log
    clean_white_proc_cfg
    if [ "$Driver_Install_Type" = "debug" ];then
        open_debug_switch
    fi
    if [ -d "$targetdir"/driver/tools ];then
        changeDavinciMode
        changeDvppCmdlistMode
        # toolchain servers
        set_sock_option
    fi

    run_file_info "$targetdir"
    servers_log
    is_lab=0
    # check whether LogAnalysis server exist
    if [ `which Get_Server_Info.py 2>/dev/null | wc -l` -ne 0 ] && [ -f "/dlyupg/Contral_Plane_IpAddress.xls" ];then
        ip_exist1=`Get_Server_Info.py Log IP 2>/dev/null`
        ip_exist2=`Get_Server_Info.py LogAnalysis IP 2>/dev/null`
        if [ ! -z "$ip_exist1" ] || [ ! -z "$ip_exist2" ];then
            is_lab=1
        fi
    fi

    if [ -f "/sys/kernel/mm/transparent_hugepage/shmem_enabled" ];then
        mount -o remount,huge=advise /dev/shm/
        hugepage_enabled=$(sudo cat /sys/kernel/mm/transparent_hugepage/enabled 2>/dev/null | sed -n 's/.*\[\([^]]*\)\].*/\1/p')
        if [ "${hugepage_enabled}"x == "never"x ]; then
            echo madvise > /sys/kernel/mm/transparent_hugepage/enabled
        fi
        echo advise > /sys/kernel/mm/transparent_hugepage/shmem_enabled
    fi

    [ "$g_change_mode_background" != 1 ] && endMode

    if [ -f $ASCEND_DRIVER_CFG_FILE ];then
        local kms_config_txt=`grep -r "${KMSAGENT_CONFIG_ITEM}=" "${ASCEND_DRIVER_CFG_FILE}" | cut -d"=" -f2-`
        if [ "${kms_config_txt}X" = "enableX" ];then
            if [ -f "$Driver_Install_Path_Param"/driver/script/${ASCEND_DRIVER_SETUP_SCRIPT} ]; then
                "$Driver_Install_Path_Param"/driver/script/${ASCEND_DRIVER_SETUP_SCRIPT} $KMSAGENT_CONFIG_ITEM start >> $log_name &
            fi
        fi
    fi

    return 0
}
servers_start_ret=0
case "$1" in
    start)
    startMode
    servers_start
    "${targetdir}"/driver/script/run_driver_upgrade_check.sh -s &>/dev/null & :
    ;;

    restart)
    startMode
    servers_stop
    servers_start
    ;;

    *)
    # no-operational
    ;;
esac

exit $servers_start_ret
