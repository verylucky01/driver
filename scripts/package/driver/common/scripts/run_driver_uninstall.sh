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
ASCEND_SECLOG="/var/log/ascend_seclog"
targetdir=/usr/local/Ascend
installInfo="/etc/ascend_install.info"
g_start_time="$(date +"%Y-%m-%d %H:%M:%S")"
sourcedir="$PWD"/driver
SHELL_DIR=$(cd "$(dirname "$0")" || exit; pwd)
COMMON_SHELL="$SHELL_DIR/common.sh"
# load common.sh, get install.info
source "${COMMON_SHELL}"
# read Driver_Install_Path_Param from installInfo
UserName=$(getInstallParam "UserName" "${installInfo}")
UserGroup=$(getInstallParam "UserGroup" "${installInfo}")
# read Driver_Install_Path_Param from installInfo
Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
Driver_Install_Type=$(getInstallParam "Driver_Install_Type" "${installInfo}")
Driver_Install_Mode="$(getInstallParam "Driver_Install_Mode" "${installInfo}")"
# used by dkms source compile
export Driver_Install_Mode
username="$UserName"
usergroup="$UserGroup"
if [ "$username" = "" ]; then
    username=HwHiAiUser
    usergroup=HwHiAiUser
fi

filelist="/etc/ascend_filelist.info"
logFile="${ASCEND_SECLOG}/ascend_install.log"
feature_hot_reset=n
feature_crl_check=n
feature_dkms_compile=n
feature_running_on_device_check=n

log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Driver] [$cur_date] $1" >> $logFile
    return 0
}
drvEcho() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Driver] [$cur_date] $1"
}

chattrDriver() {
    if [ "$Driver_Install_Type" = "docker" ]; then
        log "[INFO]no need chattr for driver package"
        return 0
    fi
    chattr +i "$Driver_Install_Path_Param"/*.sh > /dev/null 2>&1
    chattr -R +i "$Driver_Install_Path_Param"/driver/* > /dev/null 2>&1
    log "[INFO]add chattr for driver package"
}
unchattrDriver() {
    if [ "$Driver_Install_Type" = "docker" ]; then
        log "[INFO]no need unchattr for driver package"
        return 0
    fi
    chattr -i "$Driver_Install_Path_Param"/*.sh > /dev/null 2>&1
    chattr -R -i "$Driver_Install_Path_Param"/driver/* > /dev/null 2>&1
    log "[INFO]unchattr for driver package"
}
get_feature_config() {
    local config
    if [ ! -f "$Driver_Install_Path_Param"/driver/script/feature.conf ];then
        log "[WARNING]$Driver_Install_Path_Param/driver/script/feature.conf does not exist"
        return 0
    fi
    config=$(cat "$Driver_Install_Path_Param"/driver/script/feature.conf | grep FEATURE_HOT_RESET)
    if [ "$config"x = "FEATURE_HOT_RESET=y"x ] && [ "${Driver_Install_Mode}" != "vnpu_guest" ];then
        feature_hot_reset=y
    fi
    config=$(cat "$Driver_Install_Path_Param"/driver/script/feature.conf | grep FEATURE_CRL_CHECK)
    if [ "$config"x = "FEATURE_CRL_CHECK=y"x ];then
        feature_crl_check=y
    fi
    config=$(cat "$Driver_Install_Path_Param"/driver/script/feature.conf | grep FEATURE_NVCNT_CHECK)
    if [ "$config"x = "FEATURE_NVCNT_CHECK=y"x ];then
        feature_nvcnt_check=y
    fi
    config=$(cat "$Driver_Install_Path_Param"/driver/script/feature.conf | grep FEATURE_DKMS_COMPILE)
    if [ "$config"x = "FEATURE_DKMS_COMPILE=y"x ];then
        feature_dkms_compile=y
    fi
    config=$(cat "$Driver_Install_Path_Param"/driver/script/feature.conf | grep FEATURE_RUNNING_ON_DEVICE)
    if [ "$config"x = "FEATURE_RUNNING_ON_DEVICE=y"x ];then
        feature_running_on_device_check=y
    fi

    return 0
}
# our ko put in /lib/modules/`uname -r`/updates
ko_output_absolute_path(){
    local absolute_path="/lib/modules/`uname -r`/updates"
    echo $absolute_path
    return 0
}

show_path() {
    log "[INFO]target path : $targetdir"
    return 0
}

update_targetdir() {
    if [ ! -z "$1" ] && [ -d "$1" ];then
        targetdir="$1"
        targetdir="${targetdir%*/}"
    else
        log "[ERROR]target path($1) is wrong, uninstall failed"
        return 1
    fi

    show_path
    return 0
}

drv_ddr_remove() {
    if [ -f "${targetdir}"/driver/script/specific_func.inc ];then
        . "${targetdir}"/driver/script/specific_func.inc
    elif [ -f  "${sourcedir}"/script/specific_func.inc ];then
        . "${sourcedir}"/script/specific_func.inc
    fi
    if [ -L "${targetdir}"/driver/device/$crlFile ]; then
        rm -f "${targetdir}"/driver/device/$crlFile
    fi

    # delete the temporary compilation file with '.o'
    rm -f $(find ${targetdir}/driver/kernel/ -name "*.o*" 2>/dev/null)

    # delete pss.cfg
    if [ -f /etc/pss.cfg ]; then
        log "[INFO]remove PSS-cfg file"
        rm -f /etc/pss.cfg
    fi
    # remove softlink
    if [ -L "${targetdir}"/driver/lib64/libts.so ];then
       rm -rf "${targetdir}"/driver/lib64/libts.so
       log "[INFO]rm -rf softlink"
    fi
    if [ -L /usr/lib64/libtensorflow.so ];then
       rm -rf /usr/lib64/libtensorflow.so
       log "[INFO]rm -rf softlink tensorflow"
    fi
    if [ -L /usr/lib64/libcpu_kernels_context.so ];then
       rm -rf /usr/lib64/libcpu_kernels_context.so
       log "[INFO]rm -rf softlink cpu_kernels_context"
    fi
    # remove cpio,images,tee and device directory
    if [ -d "${targetdir}"/driver/device ];then
       rm -rf "${targetdir}"/driver/device
       log "[INFO]rm -rf device success"
    fi
    # remove folder "/var/driver/" with 'pid,socket'
    if [ -d /var/driver ];then
       rm -rf /var/driver
       log "[INFO]rm -rf driver success"
    fi
    # remove device side log
    if [ "${uninstallPara}"x = "Uninstall"x ]; then
        if [ "${feature_running_on_device_check}" = "y" ]; then
          if [ -d /var/log/npu/coredump/udf ];then
            umount /var/log/npu/coredump/udf
          fi
          rm -rf /var/log/npu
          log "[INFO]rm -rf log success"
        fi
    fi

    remove_result=n
    BASH_ENV_PATH="${targetdir}/driver/bin/setenv.bash"
    if [ -f "${targetdir}"/driver/script/install_common_parser.sh ] && [ -f "${targetdir}"/driver/script/filelist.csv ]; then
        # restoremod
        "${targetdir}"/driver/script/install_common_parser.sh --restoremod  --username="unknown" --usergroup="unknown" "$Driver_Install_Type" "$targetdir" "${targetdir}"/driver/script/filelist.csv >> /dev/null 2>&1
        if [ $? -ne 0 ];then
            log "[ERROR]install_common_parser.sh restoremod failed"
            return 1
        fi
        # unset bash env
        "${targetdir}"/driver/script/install_common_parser.sh --del-env-rc --package="driver" --username="${username}" "$targetdir" "${BASH_ENV_PATH}" "bash"
        if [ $? -ne 0 ]; then
            log "[ERROR]ERR_NO:0x0089;ERR_DES:failed to unset bash env."
            return 1
        fi
        "${targetdir}"/driver/script/install_common_parser.sh --remove "$Driver_Install_Type" "$targetdir" "${targetdir}"/driver/script/filelist.csv
        if [ $? -eq 0 ]; then
            remove_result=y
        fi
    fi
    if [ $remove_result = n ]; then
        ./install_common_parser.sh --restoremod  --username="unknown" --usergroup="unknown" "$Driver_Install_Type" "$targetdir" ./filelist.csv >> /dev/null 2>&1
        if [ $? -ne 0 ];then
            log "[ERROR]cur install_common_parser.sh restoremod failed"
            return 1
        fi
        # unset bash env
        ./install_common_parser.sh --del-env-rc --package="driver" --username="${username}" "$targetdir" "${BASH_ENV_PATH}" "bash"
        if [ $? -ne 0 ]; then
            log "[ERROR]ERR_NO:0x0089;ERR_DES:failed to unset bash env."
            return 1
        fi
        ./install_common_parser.sh --remove "$Driver_Install_Type" "$targetdir" ./filelist.csv
        if [ $? -eq 0 ];then
            remove_result=y
        fi
    fi
    if [ $remove_result = y ]; then
        log "[INFO]rm -rf ${targetdir}/driver success"
        return 0
    else
        log "[ERROR]rm -rf ${targetdir}/driver failed"
        return 1
    fi
}

filelist_remove() {
    if [ ! -f $filelist ];then
        return 0
    fi

    while read file
    do
        file=$(echo $file | grep "\[$1\]" | awk '{print $2}')
        local tmp_file=${file##*/}
        ls $sys_dir | grep $tmp_file > /dev/null 2>&1
        if [ $? -eq 0 ];then
            rm -rf ${sys_dir}/${tmp_file}*
            log "[INFO]rm ${tmp_file}"
        fi
        weak_updates_ko=$(find /lib/modules/$(uname -r)/ -name "${tmp_file}*")
        if [ ! -z "${weak_updates_ko}" ]; then
            rm -f "${weak_updates_ko}" > /dev/null 2>&1
            log "[INFO]rm -f [${weak_updates_ko}] success"
        fi
    done < $filelist

    return 0
}

driver_delete_temp() {
    local sys_dir=/lib/modules/$(uname -r)/kernel/drivers/davinci
    if [ -d $sys_dir ]; then
        rm -rf $sys_dir
        depmod -a > /dev/null 2>&1
        log "[INFO]rm -rf $sys_dir success"
    fi
    return 0
}

driver_auto_insmod_unload() {
    sys_dir=$(ko_output_absolute_path)
    if [ -d $sys_dir ]; then
        filelist_remove driver_ko
        # upgrade the relation
        depmod -a > /dev/null 2>&1

        log "[INFO]driver auto insmod unload success"
    fi

    driver_delete_temp

    return 0
}


pcie_load_path_unset() {
    if [ -f "/lib/davinci.conf" ]; then
        rm -f /lib/davinci.conf
        if [ $? -ne 0 ];then
            log "[ERROR]rm /lib/davinci.conf failed"
            return 1
        fi

        log "[INFO]remove /lib/davinci.conf success"
    fi

    return 0
}

pcivnic_mac_unset()
{
    conf_file=/etc/d-pcivnic.conf
    udev_rule_file=/etc/udev/rules.d/70-persistent-net.rules
    max_dev_num=0

    if [ -f $conf_file ];then
        rm -f $conf_file
    fi

    if [ -f $udev_rule_file ];then
        for dev_id in $(seq 0 $max_dev_num)
        do
            ifname="endvnic"
            # del
            sed -i 's#'$ifname'#PCIVNIC_TMP_VARIABLE_WILL_DEL#;/PCIVNIC_TMP_VARIABLE_WILL_DEL/d' $udev_rule_file
        done
    fi

    log "[INFO]pcivnic mac unset success"
    return 0
}

delTargetdir() {
    if [ "${targetdir}" = "/usr/local/Ascend" ] && [ -d "${targetdir}" ] && [ `ls "${targetdir}" | wc -l` -eq 0 ];then
        [ "${NEW_Driver_Install_Path}" != "${targetdir}" ] && log "[INFO]new-install-path is different from the old one." && rm -rf "${targetdir}"
    fi
    [ -f /usr/bin/device_boot_init.sh ] && chattr -i /usr/bin/device_boot_init.sh >& /dev/null && rm -f /usr/bin/device_boot_init.sh
}

# hotreset device, must be before install file removed
drv_hotreset_check() {
    if [ -f "${targetdir}"/driver/tools/upgrade-tool ];then
        export LD_LIBRARY_PATH="$targetdir"/driver/lib64/common:"$targetdir"/driver/lib64/driver:"${targetdir}"/driver/lib64:${LD_LIBRARY_PATH}
        timeout 10s "${targetdir}"/driver/tools/upgrade-tool --device_index -1 --phymachflag >> /dev/null 2>&1
        ret=$?
        log "[INFO]check if is a physical-machine,ret=$ret"
        if [ $ret -eq 124 ];then
            log "[WARNING]get device phymachflag timeout"
            return 0
        fi
        if [ $ret -eq 2 ];then
            log "[WARNING]not a physical-machine,hot reset not support"
            return 0
        fi
    fi
    if [ $feature_hot_reset = y ];then
        local i=0
        while true
        do
            if [ ! -f "${targetdir}"/driver/tools/upgrade-tool ];then
                log "[WARNING]${targetdir}/driver/tools/upgrade-tool does not exist"
                break
            fi
            # when master and slave synchronize,upgrade-tool status is upgrading, need to wait,max 60s( only wait before hotreset)
            timeout 10s "${targetdir}"/driver/tools/upgrade-tool --device_index -1 --status | grep -E '{"device":|{"device_id":' | grep -q "upgrading"
            if [ $? -eq 0 ];then
                log "[WARNING]master-slave sync,wait $i *10s(max 60s)"
            else
                log "[INFO]master-slave sync finish, continue to install"
                break
            fi
            i=`expr $i + 1` && [ $i -eq 6 ] && log "[ERROR]master-slave sync timeout" && break

            sleep 10
        done
        if [ ! -f "${targetdir}"/driver/script/device_hot_reset.sh ]; then
            log "[INFO]no hotreset file: "${targetdir}/driver/script/device_hot_reset.sh
            if [ ! -f "${sourcedir}"/script/device_hot_reset.sh ]; then
                log "[INFO]no hotreset file: "${sourcedir}/script/device_hot_reset.sh
                drvEcho "[WARNING]no hot reset file"
            else
                "${sourcedir}"/script/device_hot_reset.sh
            fi
        else
            "${targetdir}"/driver/script/device_hot_reset.sh
        fi
    fi
}
unset_conf_for_dracut() {
    if [ -f /etc/dracut.conf.d/Ascend-dracut.conf ]; then
        rm -f /etc/dracut.conf.d/Ascend-dracut.conf
        log "[INFO]unset_conf_for_dracut success"
    fi

    return 0
}

remove_modules_load_conf() {
    source $SHELL_DIR/common_func.inc
    get_os_info
    if [ "${HostOsName}" == "CentOS" ]; then
        if [ -e /etc/modules-load.d/ascend-fat.conf ]; then
            rm -f /etc/modules-load.d/ascend-fat.conf
        fi
    fi
}

livepatch_audit_log_record() {
    local newest_livepatch_version="$1" return_code=$2
    local log_opt="Uninstall" log_level="MAJOR" result="" livepatch_name="Ascend-driver_sph-${newest_livepatch_version}"
    local user_name="root" cur_ip="127.0.0.1" opt_file="${ASCEND_SECLOG}/operation.log" cmdlist="none"

    if [ ! -e "${opt_file}" ]; then
        touch "${opt_file}"
        chmod -f 400 "${opt_file}"
    fi

    if [ ${return_code} -eq 0 ]; then
        result="success"
    else
        result="failed"
    fi

    echo "${log_opt} ${log_level} ${user_name} ${g_start_time} ${cur_ip} ${livepatch_name} ${result} cmdlist=${cmdlist}" >> ${opt_file}
    return 0
}

uninstall_livepatch() {
    local drv_root_dir="$1"
    local livepatch_install_conf="${drv_root_dir}/driver/livepatch/livepatch_install.info"
    local newest_patch_version="" newest_patch_path="" uninstall_sh=""

    newest_patch_version="$(grep -v "^$" "${livepatch_install_conf}" 2>/dev/null | sed -n '$p')"
    if [ -z "${newest_patch_version}" ]; then
        log "[INFO]newest livepatch is NULL and skip here."
        return 0
    fi

    if [ ! -d "${drv_root_dir}/driver/livepatch/${newest_patch_version}" ]; then
        log "[INFO]The livepatch dir doesn't exist and skip here."
        log "[INFO]To remove [${drv_root_dir}/driver/livepatch]."
        rm -rf "${drv_root_dir}/driver/livepatch"
        return 0
    fi

    newest_patch_path="${drv_root_dir}/driver/livepatch/${newest_patch_version}"
    uninstall_sh="${drv_root_dir}/driver/livepatch/${newest_patch_version}/script/run_livepatch_uninstall.sh"

    "${uninstall_sh}" "pre" "y" "${newest_patch_path}" 999 && ret=$? || ret=$?
    if [ ${ret} -ne 0 ]; then
        log "[ERROR]The livepatch's pre-uninstall failed."
        livepatch_audit_log_record "${newest_patch_version}" ${ret}
        return 1
    fi

    "${uninstall_sh}" "post" "y" "${newest_patch_path}" 999 && ret=$? || ret=$?
    if [ ${ret} -ne 0 ]; then
        log "[ERROR]The livepatch's post-uninstall failed."
    fi
    livepatch_audit_log_record "${newest_patch_version}" ${ret}

    unchattrDriver
    return ${ret}
}

# start!

while true
do
    case "$1" in
    --uninstall)
        get_feature_config
        uninstallPath="$2"
        uninstallPara="$3"
        update_targetdir "$uninstallPath"
        if [ $? -ne 0 ];then
            exit 1
        fi
        unchattrDriver
        if [ "$Driver_Install_Type" = "devel" ]; then
            drv_ddr_remove
            if [ $? -ne 0 ];then
                exit 1
            else
                delTargetdir
                log "[INFO]Driver devel package uninstall success."
                exit 0
            fi
        elif [ "$Driver_Install_Type" = "docker" ]; then
            drv_ddr_remove
            if [ $? -ne 0 ];then
                exit 1
            else
                delTargetdir
                log "[INFO]docker mode,Driver package uninstall success."
                exit 0
            fi
        fi
        uninstall_livepatch "${targetdir}"
        remove_modules_load_conf
        if [ $feature_dkms_compile = y ]; then
            if [ -f "${targetdir}"/driver/script/run_driver_dkms_uninstall.sh ]; then
                "${targetdir}"/driver/script/run_driver_dkms_uninstall.sh
                log "[INFO]call run_driver_dkms_uninstall.sh "
            elif [ -f "${sourcedir}"/script/run_driver_dkms_uninstall.sh ]; then
                "${sourcedir}"/script/run_driver_dkms_uninstall.sh
                log "[INFO]call run_driver_dkms_uninstall.sh in runpackage"
            fi
            if [ $? -ne 0 ];then
                chattrDriver
                exit 1
            fi
        fi

        driver_auto_insmod_unload

        drv_hotreset_check
        drv_ddr_remove
        if [ $? -ne 0 ];then
            chattrDriver
            exit 1
        fi

        pcie_load_path_unset
        if [ $? -ne 0 ];then
            chattrDriver
            exit 1
        fi

        pcivnic_mac_unset
        unset_conf_for_dracut
        if [ -f $filelist ]; then
            rm -f $filelist
        fi
        delTargetdir
        exit 0
        ;;
    --*)
        shift
        continue
        ;;
    *)
        log "[ERROR]the arguement doesn't support"
        break
        ;;
    esac
done

exit 1


