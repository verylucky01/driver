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
    g_change_mode_dev_list=" /dev/hisi_hdc /dev/davinci_manager /dev/devmm_svm "
    g_online_dev_num=0
    get_pci_info
}
changeDavinciModeOnce() {
    for char_dev in $g_change_mode_dev_list;do
        [ -e $char_dev ] || continue
        setFileChmod -f 660 $char_dev
        chown -f $username:$usergroup $char_dev
        add_log "char dev: $char_dev exist and set file mode owner done."
        g_change_mode_dev_list=${g_change_mode_dev_list/ $char_dev / }
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

    udevadm trigger > /dev/null 2>&1

    add_log "change mode complete, online_dev_num: $g_online_dev_num."

    "${targetdir}"/driver/tools/upgrade-tool --mini_devices >> $log_name
    if [ $? -ne 0 ]; then
        add_log "upgrade-tool get device count failed"
    else
        add_log "upgrade-tool get device count success"
    fi
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
    for wait_cnt in {1..145};do
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

init_signature_configuration() {

    if [ ! -d "/var/asdrv" ]; then
        mkdir -p /var/asdrv
        chmod 755 /var/asdrv
    fi

    devids=({0..63} {100..1123})
    for devid in "${devids[@]}"; do
        if [ ! -f "/var/asdrv/dev${devid}/device-sw-plugin/verify/flag" ];then
            mkdir -p /var/asdrv/dev${devid}
            chmod 755 /var/asdrv/dev${devid}

            mkdir -p /var/asdrv/dev${devid}/device-sw-plugin
            chmod 755 /var/asdrv/dev${devid}/device-sw-plugin

            mkdir -p /var/asdrv/dev${devid}/device-sw-plugin/verify
            chmod 755 /var/asdrv/dev${devid}/device-sw-plugin/verify

            echo "verify_flag=1" > "/var/asdrv/dev${devid}/device-sw-plugin/verify/flag";
            touch "/var/asdrv/dev${devid}/device-sw-plugin/verify/user-certificate";
            chmod 644 "/var/asdrv/dev${devid}/device-sw-plugin/verify/flag";
            chmod 644 "/var/asdrv/dev${devid}/device-sw-plugin/verify/user-certificate";
        fi

        if [ ! -f "/var/asdrv/dev${devid}/device-sw-plugin/verify/user-certificate" ];then
            touch "/var/asdrv/dev${devid}/device-sw-plugin/verify/user-certificate";
            chmod 644 "/var/asdrv/dev${devid}/device-sw-plugin/verify/user-certificate";
        fi
    done
}

servers_start() {
    clean_servers_log
    if [ -d "$targetdir"/driver/tools ];then
        changeDavinciMode

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
        echo madvise > /sys/kernel/mm/transparent_hugepage/enabled
        echo advise > /sys/kernel/mm/transparent_hugepage/shmem_enabled
    fi

    init_signature_configuration

    [ "$g_change_mode_background" != 1 ] && endMode
}

servers_start_ret=0
case "$1" in
    start)
    startMode
    servers_start
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
