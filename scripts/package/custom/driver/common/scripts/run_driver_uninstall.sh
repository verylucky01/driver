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
ASCEND_DRIVER_CFG_FILE="/etc/ascend_driver.conf"
KMSAGENT_SERVICES="/lib/systemd/system/ascend-kmsagent.service"
KMSAGENT_USER_SERVICES="/usr/lib/systemd/system/ascend-kmsagent.service"
ASCEND_DRIVER_SETUP_SCRIPT="ascend_driver_config.sh"
KMSAGENT_CONFIG_ITEM="kmsagent"
KMSAGENT_WORK_DIR="/var/kmsagentd"
SHELL_DIR=$(cd "$(dirname "$0")" || exit; pwd)
PACKAGE_TYPE="run"
if [ "${PACKAGE_TYPE}" = "run" ];then
    sourcedir="$PWD"/driver
else 
    sourcedir="$SHELL_DIR"/..
fi
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


log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    user_id=`id | awk '{printf $1}'`
    echo "[Driver] [$cur_date] [$user_id] "$1 >> $logFile
    return 0
}
drvEcho() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Driver] [$cur_date] $1"
}

drvColorEcho() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo -e  "[Driver] [$cur_date] $1"
}

chattrDriver() {
    chattr +i "$Driver_Install_Path_Param"/*.sh > /dev/null 2>&1
    chattr -R +i "$Driver_Install_Path_Param"/driver/* > /dev/null 2>&1
    if [ "$Driver_Install_Type" = "docker" ]; then
        if [ -d "$Driver_Install_Path_Param"/driver/script ];then
            chattr -R +i "$Driver_Install_Path_Param"/driver/script/* > /dev/null 2>&1
        fi
        if [ -d "$Driver_Install_Path_Param"/driver/tools ];then
            chattr -R +i "$Driver_Install_Path_Param"/driver/tools/* > /dev/null 2>&1
        fi
        if [ -d "$Driver_Install_Path_Param"/driver/lib64 ];then
            chattr -R +i "$Driver_Install_Path_Param"/driver/lib64/* > /dev/null 2>&1
        fi
        log "[INFO]add chattr for driver docker mode"
    fi
    log "[INFO]add chattr for driver package"
}
unchattrDriver() {
    chattr -i "$Driver_Install_Path_Param"/*.sh > /dev/null 2>&1
    chattr -R -i "$Driver_Install_Path_Param"/driver/* > /dev/null 2>&1
    if [ "$Driver_Install_Type" = "docker" ]; then
        if [ -d "$Driver_Install_Path_Param"/driver/script ];then
            chattr -R -i "$Driver_Install_Path_Param"/driver/script/* > /dev/null 2>&1
        fi
        if [ -d "$Driver_Install_Path_Param"/driver/tools ];then
            chattr -R -i "$Driver_Install_Path_Param"/driver/tools/* > /dev/null 2>&1
        fi
        if [ -d "$Driver_Install_Path_Param"/driver/lib64 ];then
            chattr -R -i "$Driver_Install_Path_Param"/driver/lib64/* > /dev/null 2>&1
        fi
        log "[INFO]unchattr for driver docker mode"
    fi
    log "[INFO]unchattr  for driver package"
}
get_feature_config() {
    local config
    if [ ! -f "$Driver_Install_Path_Param"/driver/script/feature.conf ];then
        log "[WARNING]$Driver_Install_Path_Param/driver/script/feature.conf does not exist"
        return 0
    fi
    config=$(cat "$Driver_Install_Path_Param"/driver/script/feature.conf | grep FEATURE_HOT_RESET | awk -F'=' '{print $2}')
    if [ "$config"x = "y"x ] && [ "${Driver_Install_Mode}" != "vnpu_guest" ];then
        feature_hot_reset=y
    fi
    config=$(cat "$Driver_Install_Path_Param"/driver/script/feature.conf | grep FEATURE_CRL_CHECK | awk -F'=' '{print $2}')
    if [ "$config"x = "y"x ];then
        feature_crl_check=y
    fi
    config=$(cat "$Driver_Install_Path_Param"/driver/script/feature.conf | grep FEATURE_NVCNT_CHECK | awk -F'=' '{print $2}')
    if [ "$config"x = "y"x ];then
        feature_nvcnt_check=y
    fi
    config=$(cat "$Driver_Install_Path_Param"/driver/script/feature.conf | grep FEATURE_DKMS_COMPILE | awk -F'=' '{print $2}')
    if [ "$config"x = "y"x ];then
        feature_dkms_compile=y
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
       rm -f "${targetdir}"/driver/lib64/libts.so
       log "[INFO]rm -f softlink"
    fi
    empty_softlinks="libtensorflow.so libcpu_kernels_context.so"
    for softlink in ${empty_softlinks[@]}
		do
            if [ -L /usr/lib64/${softlink} ];then
                real_path=`ls -l /usr/lib64/${softlink} | awk '{print $NF}'`
                if [ ! -f "${real_path}" ]; then
                    rm -f /usr/lib64/${softlink}
                    log "[INFO]rm -f softlink ${softlink}"
                fi
            fi
		done
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
        if [ "${PACKAGE_TYPE}" = "run" ];then
            "${targetdir}"/driver/script/install_common_parser.sh --remove "$Driver_Install_Type" "$targetdir" "${targetdir}"/driver/script/filelist.csv
            if [ $? -eq 0 ]; then
                remove_result=y
            fi
        else
            remove_result=y
        fi
    fi
    if [ "$remove_result" = "n" ] && [ "${PACKAGE_TYPE}" = "run" ]; then
        local install_common_parser_path=$sourcedir/..
        "$install_common_parser_path"/install_common_parser.sh --restoremod  --username="unknown" --usergroup="unknown" "$Driver_Install_Type" "$targetdir" "$install_common_parser_path"/filelist.csv >> /dev/null 2>&1
        if [ $? -ne 0 ];then
            log "[ERROR]cur install_common_parser.sh restoremod failed"
            return 1
        fi
        # unset bash env
        "$install_common_parser_path"/install_common_parser.sh --del-env-rc --package="driver" --username="${username}" "$targetdir" "${BASH_ENV_PATH}" "bash"
        if [ $? -ne 0 ]; then
            log "[ERROR]ERR_NO:0x0089;ERR_DES:failed to unset bash env."
            return 1
        fi
        "$install_common_parser_path"/install_common_parser.sh --remove "$Driver_Install_Type" "$targetdir" "$install_common_parser_path"/filelist.csv
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
        file=$(echo "$file" | grep "\[$1\]" | awk '{print $2}')
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

    if [ -e "${sys_dir}"/drv_vascend.ko ] || [ -e "${sys_dir}"/drv_vascend.ko.xz ]; then
        rm -rf "${sys_dir}"/drv_vascend.ko*
    fi

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

del_kms_array=($ASCEND_DRIVER_CFG_FILE )
delTargetdir() {
    if [ "${targetdir}" = "/usr/local/Ascend" ] && [ -d "${targetdir}" ] && [ `ls "${targetdir}" | wc -l` -eq 0 ];then
        [ "${NEW_Driver_Install_Path}" != "${targetdir}" ] && log "[INFO]new-install-path is different from the old one." && rm -rf "${targetdir}"
    fi
    [ -f /usr/bin/device_boot_init.sh ] && chattr -i /usr/bin/device_boot_init.sh >& /dev/null && rm -f /usr/bin/device_boot_init.sh

    if [ -f $KMSAGENT_SERVICES ];then
        rm -f $KMSAGENT_SERVICES
        systemctl daemon-reload 1>/dev/null 2>&1
    fi
    if [ -f $KMSAGENT_USER_SERVICES ];then
        rm -f $KMSAGENT_USER_SERVICES
        systemctl daemon-reload 1>/dev/null 2>&1
    fi
    for kms_param in "${del_kms_array[@]}"; do
        if [ -e $kms_param ]; then
            rm -f $kms_param
        fi
    done
}

# hotreset device, must be before install file removed
drv_hotreset_check() {
    if [ -f "${targetdir}"/driver/tools/upgrade-tool ];then
        export LD_LIBRARY_PATH="$targetdir"/driver/lib64/common:"$targetdir"/driver/lib64/driver:"${targetdir}"/driver/lib64:${LD_LIBRARY_PATH}
        timeout 20s "${targetdir}"/driver/tools/upgrade-tool --device_index -1 --phymachflag >> /dev/null 2>&1
        ret=$?
        log "[INFO]check if is a physical-machine,ret=$ret"
        if [ $ret -eq 2 ];then
            if lspci 2>/dev/null | grep -q 'd80[23]'; then
                log "[INFO]not a physical-machine, hot reset not support"
                return 0
            else
                log "[INFO]not a physical-machine"
            fi
        fi
    fi
    if [ $feature_hot_reset = y ] && [ "$force" = n ];then
        local i=0
        while true
        do
            if [ ! -f "${targetdir}"/driver/tools/upgrade-tool ];then
                log "[WARNING]${targetdir}/driver/tools/upgrade-tool does not exist"
                break
            fi
            # when master and slave synchronize,upgrade-tool status is upgrading, need to wait,max 60s( only wait before hotreset)
            timeout 20s "${targetdir}"/driver/tools/upgrade-tool --device_index -1 --status | grep -E '{"device":|{"device_id":' | grep -q "upgrading" >> /dev/null 2>&1
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
    if [ -e /etc/modules-load.d/ascend_modules.conf ]; then
        rm -f /etc/modules-load.d/ascend_modules.conf
    fi
}

# start!

while true
do
    case "$1" in
    --uninstall)
        if [ "$Driver_Install_Type"x != "devel"x -a "$Driver_Install_Type"x != "docker"x ]; then
            if [ -f $installInfo ]; then
                sed -i '/Driver_Install_Status=/d' $installInfo
            fi
            remove_modules_load_conf
        fi
        get_feature_config
        uninstallPath="$2"
        force="$3"
        if [ -z "$3" ];then
            force=n
        fi
        update_targetdir "$uninstallPath"
        timeout_exec=""
        timeout_second=300
        ret_timeout=0
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
                log "[INFO]Driver devel package uninstalled successfully."
                exit 0
            fi
        elif [ "$Driver_Install_Type" = "docker" ]; then
            drv_ddr_remove
            if [ $? -ne 0 ];then
                exit 1
            else
                delTargetdir
                log "[INFO]docker mode,Driver package uninstalled successfully."
                exit 0
            fi
        fi
        # Clear kmsagent data
        if [ -f ${targetdir}/driver/script/kmsagent_uninstall.sh ]; then
            ${targetdir}/driver/script/kmsagent_uninstall.sh $KMSAGENT_WORK_DIR >>$logFile 2>&1
        fi

        which timeout >/dev/null 2>&1 && timeout_exec="timeout ${timeout_second}"
        if [ $feature_dkms_compile = y ]; then
            if [ -f "${targetdir}"/driver/script/run_driver_dkms_uninstall.sh ]; then
                log "[INFO]call run_driver_dkms_uninstall.sh "
                ${timeout_exec} "${targetdir}"/driver/script/run_driver_dkms_uninstall.sh
                ret_timeout=$?
            elif [ -f "${sourcedir}"/script/run_driver_dkms_uninstall.sh ]; then
                log "[INFO]call run_driver_dkms_uninstall.sh in runpackage"
                ${timeout_exec} "${sourcedir}"/script/run_driver_dkms_uninstall.sh
                ret_timeout=$?
            fi
            if [ "${ret_timeout}" -ne 0 ]; then
                # If the command times out, and --preserve-status is not set, then exit with
                # status 124.  Otherwise, exit with the status of COMMAND.
                if [ "${ret_timeout}" -eq 124 ]; then
                    drvEcho "[ERROR]DKMS check and uninstall time(${timeout_second} seconds) out, please check DKMS and uninstall later!"
                fi
                log "[ERROR]call run_driver_dkms_uninstall.sh failed."
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