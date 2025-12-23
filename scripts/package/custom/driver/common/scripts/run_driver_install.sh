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
targetdir=/usr/local/Ascend/driver
crldir="/root/ascend_check"
driverCrlStatusFile="/root/ascend_check/driver_crl_status_tmp"
installInfo="/etc/ascend_install.info"
filelist="/etc/ascend_filelist.info"
logFile="${ASCEND_SECLOG}/ascend_install.log"
hotreset_status_file="/var/log/hotreset_status.log"
SHELL_DIR=$(cd "$(dirname "$0")" || exit; pwd)
PACKAGE_TYPE="run"
if [ "${PACKAGE_TYPE}" = "run" ];then
    sourcedir="$PWD"/driver
else 
    sourcedir="$SHELL_DIR"/..
fi
source $sourcedir/script/log_common.sh
COMMON_SHELL="$SHELL_DIR/common.sh"
KMSAGENT_CONFIG_ITEM="kmsagent"
# load common.sh, get install.info
source "${COMMON_SHELL}"
# read Driver_Install_Path_Param from installInfo
UserName=$(getInstallParam "UserName" "${installInfo}")
UserGroup=$(getInstallParam "UserGroup" "${installInfo}")
# read Driver_Install_Path_Param from installInfo
Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
Driver_Install_Mode="$(getInstallParam "Driver_Install_Mode" "${installInfo}")"
# used by dkms source compile
export Driver_Install_Mode
[ "${Driver_Install_Mode}" = "vnpu_guest" ] && feature_hot_reset=n
username="$UserName"
usergroup="$UserGroup"
do_dkms=n
no_kernel_flag=n
first_time=y

check_is_support_dkms()
{
    if [ ! -e $sourcedir/script/os_adapt.sh ]; then
        export DKMS_SUPPORT=n
        return 1
    fi

    if [ ! -e $sourcedir/script/dkms_check.sh ]; then
        export DKMS_SUPPORT=n
        return 1
    fi
    if [ ! -e $sourcedir/script/deal_ko.sh ]; then
        export DKMS_SUPPORT=n
        return 1
    fi
    if [ ! -e $sourcedir/script/run_driver_dkms_install.sh ]; then
        export DKMS_SUPPORT=n
        return 1
    fi

    if [ ! -e $sourcedir/script/run_driver_map_kernel.sh ]; then
        export DKMS_SUPPORT=n
        return 1
    fi

    if [ ! -e $sourcedir/script/run_driver_ko_rebuild.sh ]; then
        export DKMS_SUPPORT=n
        return 1
    fi

    if [ ! -e $sourcedir/script/binary_os_config.sh ]; then
        export DKMS_SUPPORT=n
        return 1
    fi

    export DKMS_SUPPORT=y
    return 0
}

chattrDriver() {
    chattr +i "$Driver_Install_Path_Param"/*.sh > /dev/null 2>&1
    chattr -R +i "$Driver_Install_Path_Param"/driver/* > /dev/null 2>&1
    if [ $installType = "docker" ]; then
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

updateVersionInfoVersion() {
    if [ "${PACKAGE_TYPE}" = "run" ]; then
        rm -rf "$Driver_Install_Path_Param"/driver/version.info
        cp $sourcedir/../version.info "$Driver_Install_Path_Param"/driver
    fi
    setFileChmod -f 444 "$Driver_Install_Path_Param"/driver/version.info
}

# to change files' mode or owner
changeMode(){
    if [ -d "${Driver_Install_Path_Param}" ]; then
        # /etc/hccn.conf
        if [ ! -f "/etc/hccn.conf" ]; then
            touch "/etc/hccn.conf"
        fi
        setFileChmod -f 640 /etc/hccn.conf
        chown -fh root:root /etc/hccn.conf

        # /lib/
        if [ -f /lib/davinci.conf ];then
            setFileChmod -f 444 /lib/davinci.conf
        fi

        log "[INFO]some files in /etc/ change success"
    fi

    return 0
}

show_path() {
    log "[INFO]target path : $targetdir"
}

update_targetdir() {
    if [ ! -z "$1" ] && [ -d "$1" ];then
        targetdir="$1"
        targetdir="${targetdir%*/}"/driver
    fi
}


device_crl_create() {
    if [ -e $crldir/$crlFile ]; then
        ln -sf $crldir/$crlFile "$targetdir"/device
        log "[INFO]create softlink for $crldir/$crlFile"
    fi
}

load_file_check() {
    local dst_name

    for dst_name in $dst_names
    do
        if [ ! -f "$1"/$dst_name ]; then
            log "[ERROR]host load file("$1"/$dst_name) is missing, failed"
            return 1
        fi
    done
    return 0
}
load_file_rename() {
    if [ -f $1 ] && [ ! -f $2 ];then
        mv $1 $2
    fi
}

device_pcie_load_file_rename() {
    [ ${no_kernel_flag} = y ] && return 0
    [ ${feature_no_device_kernel} = y ] && return 0
    local i
    cd  "$targetdir"/device
    for i in "${!src_names[@]}";
    do
        load_file_rename "${src_names[$i]}" "${dst_names[$i]}"
    done
    cd - >/dev/null

    if [ $feature_crl_check = y ]; then
        device_crl_create
    fi
    load_file_check "$targetdir"/device
    if [ $? -ne 0 ];then
        log "[ERROR]load_file_check $targetdir/device failed"
        return 1
    fi

    log "[INFO]device pcie load files rename success"
    return 0
}

filelist_remove() {
    if [ ! -f $filelist ];then
        return 0
    fi

    while read file
    do
        file=$(echo $file | grep "\[$1\]" | awk '{print $2}')
        local tmp_file=${file##*/}
        ls $sys_path | grep $tmp_file > /dev/null 2>&1
        if [ $? -eq 0 ];then
            rm -rf ${sys_path}/${tmp_file}*
        fi
    done < $filelist
    if [ -e ${sys_path}/drv_vascend.ko ] || [ -e ${sys_path}/drv_vascend.ko.xz ]; then
        rm -rf ${sys_path}/drv_vascend.ko*
    fi
    return 0
}

# set selinux security context for driver ko
chcon_for_selinux(){
    [ $no_kernel_flag = y ] && return 0
    local single_ko
    if getenforce >/dev/null 2>&1;then
        for single_ko in $(echo "${g_kos}" | grep "ko$")
        do
           if [ -e ${sys_path}/${single_ko}* ]; then
                chcon -t modules_object_t ${sys_path}/${single_ko}* >>/dev/null 2>&1 || log "[INFO]chcon modules_object_t for ${single_ko}: $?"
                setFileChmod -f 440 ${sys_path}/${single_ko}*
           fi
        done
        log "[INFO]restorecon -Rv start"
        restorecon -Rv "$targetdir"/lib64/* >>/dev/null 2>&1
        log "[INFO]restorecon -Rv end"

        if [ -e "${Driver_Install_Path_Param}/driver/tools/${KMSAGENT_CONFIG_ITEM}" ]; then
            chcon -t usr_t "${Driver_Install_Path_Param}/driver/tools/${KMSAGENT_CONFIG_ITEM}" >>/dev/null 2>&1
            log "[INFO]chcon usr_t for kmsagent: $?"
        fi
    fi
    log "[INFO]chcon_for_selinux success"
    return 0
}

pcie_load_path_set() {
    [ ${no_kernel_flag} = y ] && return 0

    echo DAVINCI_HOME_PATH="${targetdir%/*}" > /lib/davinci.conf
    if [ $? -ne 0 ];then
        log "[ERROR]echo DAVINCI_HOME_PATH=${targetdir%/*} > /lib/davinci.conf failed"
        return 1
    fi

    # just for sw_64
    if [ "$(arch)" == "sw_64" ]; then
        echo "IRQ_RES_GEAR=4" >> /lib/davinci.conf || \
            { log "[ERROR]echo IRQ_RES_GEAR to /lib/davinci.conf failed" && return 1; }
    fi

    log "[INFO]show /lib/davinci.conf :"`cat /lib/davinci.conf` >>$logFile
    log "[INFO]set /lib/davinci.conf success"

    return 0
}

rand() {
    min=$1
    max=$(($2-$min+1))
    which hexdump >/dev/null 2>&1
    if [ $? -eq 0 ]; then
        num=$(hexdump -n 10 /dev/urandom | cksum | awk -F ' ' '{print $1}')
    else
        num=$(head -n 10 /dev/urandom | cksum | awk -F ' ' '{print $1}')
    fi
    echo $(($num%$max+$min))
    return 0
}

rand_mac() {
    mac=$(rand 0 255)
    mac=$(printf "%02x\n" $mac)
    echo $mac
    return 0
}

pcivnic_mac_conflict_check() {
    conflict=$1
    ifconfig -a | grep "$conflict" >>/dev/null 2>&1
    return $?
}

pcivnic_set_udev_rules() {
    udev_rule_file=/etc/udev/rules.d/70-persistent-net.rules
    ifname=$1
    mac=$2

    if [ ! -f $udev_rule_file ];then
        if [ ! -d /etc/udev/rules.d/ ];then
            mkdir -p /etc/udev/rules.d/
            setFileChmod -f 755 /etc/udev/rules.d/
        fi
        echo '# This is a sample udev rules file that demonstrates how to get udev to' >>$udev_rule_file
        echo '# set the name of net interfaces to whatever you wish.  There is a' >>$udev_rule_file
        echo '# 16 character limit on network device names though, so do not go too nuts' >>$udev_rule_file
        echo '#' >>$udev_rule_file
        echo '# Note: as of rhel7, udev is case sensitive on the address field match' >>$udev_rule_file
        echo '# and all addresses need to be in lower case.' >>$udev_rule_file
        echo '#' >>$udev_rule_file
        echo '# SUBSYSTEM=="net", ACTION=="add", DRIVERS=="?*", ATTR{address}=="00:1f:3c:48:70:b1", ATTR{type}=="1", KERNEL=="eth*", NAME="wlan0"' >>$udev_rule_file
    fi

    # del old
    sed -i 's#'$ifname'#PCIVNIC_TMP_VARIABLE_WILL_DEL#;/PCIVNIC_TMP_VARIABLE_WILL_DEL/d' $udev_rule_file

    # add rule
    echo 'SUBSYSTEM=="net", ACTION=="add", DRIVERS=="?*", ATTR{address}=="'$mac'", ATTR{type}=="1", KERNEL=="end*", NAME="'$ifname'"' >>$udev_rule_file
}

pcivnic_mac_set() {
    [ $no_kernel_flag = y ] && return 0
    conf_file=/etc/d-pcivnic.conf
    mac0="10"
    mac1="1b"
    mac2="54"
    mac3=$(rand_mac)
    mac4=$(rand_mac)

    if [ -f $conf_file ];then
        rm -f $conf_file
    fi

    pcivnic_mac_conflict_check "$mac0:$mac1:$mac2:$mac3:$mac4"
    while [ $? -eq 0 ]
    do
        mac3=$(rand_mac)
        mac4=$(rand_mac)
        pcivnic_mac_conflict_check "$mac0:$mac1:$mac2:$mac3:$mac4"
    done

    echo "# Pcie vnic mac address config file" >>$conf_file
    echo "# Format : device_id mac_address" >>$conf_file

    # mac5 is fixed 0xD3
    mac5="d3"

    dev_id=0
    ifname="endvnic"
    mac_addr="$mac0:$mac1:$mac2:$mac3:$mac4:$mac5"

    echo "$dev_id $mac_addr" >>$conf_file
    pcivnic_set_udev_rules $ifname $mac_addr

    log "[INFO]pcivnic default mac address produce finish"
    return 0
}

progress_bar(){
    local parent_progress=$1 weight=$2 child_progress=$3
    local output_progress
    output_progress=`awk 'BEGIN{printf "%d\n",('$parent_progress'+'$weight'*'$child_progress'/100)}'`
    drvEcho "[INFO]upgradePercentage:${output_progress}%"
    log "[INFO]upgradePercentage:${output_progress}%"
}

delete_tmp_ko_dirs() {
    rm -rf ${targetdir}/host
    if [ "${PACKAGE_TYPE}" != "run" ];then
        rm -rf ${targetdir}/host_driver_binary
    fi
}

# set dracut-cfg for omit-drivers from initrd-img, when new driver install or new kernel install.
set_conf_for_dracut() {
    local ret="" drivers=""
    [ "${no_kernel_flag}" = "y" ] && return 0
    drivers=$(echo $(echo "${g_kos}" | grep ko | awk -F [/.] '{print $(NF-1)}'))

    if which dracut > /dev/null 2>&1; then
        if [ -e /etc/dracut.conf.d ]; then
            echo "# Ascend davinci drivers shouldn't be repacked to initrd-img." > /etc/dracut.conf.d/Ascend-dracut.conf
            echo "omit_drivers+=\" ${drivers} \"" >> /etc/dracut.conf.d/Ascend-dracut.conf
            chmod -f 440 /etc/dracut.conf.d/Ascend-dracut.conf
            log "[INFO]make drivers in dracut-cfg success"
        fi
    fi

    return 0
}

clr_hdcd_dir() {
    if [ -d /var/log/npu/hdcd ]; then
       rm -rf /var/log/npu/hdcd &
    fi
}

create_white_proc_cfg() {
    local isA2_driver
    local isA3_driver
    local CONFIG_FILE

    source "${sourcedir}"/script/specific_func.inc
    isA2_driver=$(echo $device_bdf | grep d802)
    isA3_driver=$(echo $device_bdf | grep d803)
    if [[ -z "$isA2_driver" && -z "$isA3_driver" ]]; then
        log "[INFO] this type driver need not create white process config"
        return 0
    fi
    
    CONFIG_FILE="/etc/custom_process.cfg"
    if [[ ! -f "$CONFIG_FILE" ]]; then
        touch "$CONFIG_FILE"
        chmod 644 "$CONFIG_FILE"
    fi
}

add_modules_load_conf() {
    source "${sourcedir}"/script/common_func.inc
    get_os_info
    if [ "${HostOsName}" == "CentOS" ]; then
        echo -e "# Load fat and vfat at boot\nfat\nvfat\nxfs\next4\next3\nmsdos" > /etc/modules-load.d/ascend-fat.conf
        chmod 640 /etc/modules-load.d/ascend-fat.conf
    fi
    if [ ! -z "$is_a3_driver" ]; then
        echo -e "# Load modules at boot\ntc_pcidev" > /etc/modules-load.d/ascend_modules.conf
        chmod 640 /etc/modules-load.d/ascend_modules.conf
    fi
}

check_uname_kernel() {
    uname -a | grep $(uname -r) >>/dev/null 2>&1
    uname_kernel=$?
}

# start!
installPathParam="$1"
installType="$2"
force="$3"
no_rebuild_flag="$4"
no_kernel_flag=n
percent=0
weight=100

. $sourcedir/script/file_copy.sh
update_targetdir "$installPathParam"
if [ "$installType"x = "devel"x ]; then
    driver_file_copy
    if [ $? -ne 0 ];then
        log "[ERROR]Driver devel package install failed"
        exit 1
    fi
    updateVersionInfoVersion
    log "[INFO]Driver devel package install success!"
    exit 0
fi

if [ "$installType"x = "docker"x ]; then
    no_kernel_flag=y
    log "[INFO]no_kernel_flag value is [$no_kernel_flag]"
fi

show_path
. $sourcedir/script/specific_func.inc

check_is_support_dkms

if [ "${DKMS_SUPPORT}" = "y" ]; then
    . $sourcedir/script/os_adapt.sh
    set_os_custom_compile
fi

. $sourcedir/script/specific_func.inc

if [ "${DKMS_SUPPORT}" = "y" ]; then
    . $sourcedir/script/dkms_check.sh
    drv_dkms_env_check
    if [ $? -ne 0 ];then
        drvColorEcho "[ERROR]\033[31mDrv_dkms_env_check failed, details in : $logFile \033[0m"
        changeMode
        exit 1
    fi
fi

progress_bar $percent $weight 10
driver_file_copy
if [ $? -ne 0 ];then
    drvColorEcho "[ERROR]\033[31mDrv_file_copy failed, details in : $logFile \033[0m"
    changeMode
    exit 1
fi

progress_bar $percent $weight 30
# cp device_boot_init.sh for drv.
if [ -f ${installPathParam}/driver/tools/device_boot_init.sh ];then
    chattr -i /usr/bin/device_boot_init.sh >& /dev/null
    cp -f ${installPathParam}/driver/tools/device_boot_init.sh /usr/bin/
    setFileChmod -f 550 /usr/bin/device_boot_init.sh
    chattr +i /usr/bin/device_boot_init.sh >& /dev/null
fi
#check CRL of images
if [ $feature_crl_check = y ]; then
    if [ $no_kernel_flag = n ]; then
        . $sourcedir/script/device_crl_check.sh
        device_images_crl_check && ret=$? || ret=$?
        if [ ${ret} -ne 0 ];then
            if [ ${ret} -eq 2 ]; then
                drvColorEcho "[ERROR]\033[31mThe software signature verification failed because the signature mode used by the software is inconsistent with the current configuration. Currently configured is [${native_pkcs_conf}], details in : $logFile \033[0m"
            else
                drvColorEcho "[ERROR]\033[31mDevice_images_crl_check failed, details in : $logFile \033[0m"
            fi
            log "[ERROR]cur device crl check fail, stop upgrade"
            rm -f ${driverCrlStatusFile}
            exit 1
        else
            log "[INFO]cur device crl check success"
            if [ -e $driverCrlStatusFile ]; then
                rm -f $driverCrlStatusFile
                log "[INFO]rm -f $driverCrlStatusFile success"
            fi

            rm -f /etc/pss.cfg >& /dev/null
            log "[INFO]remove /etc/pss.cfg success"
        fi
    fi
fi

# device load files rename
device_pcie_load_file_rename
if [ $? -ne 0 ];then
    drvColorEcho  "[ERROR]\033[31mDevice_pcie_load_file_rename failed, details in : $logFile \033[0m"
    changeMode
    exit 1
fi

# save install path to /lib/davinci.conf
pcie_load_path_set
if [ $? -ne 0 ];then
    drvColorEcho "[ERROR]\033[31mPcie_load_path_set failed, details in : $logFile \033[0m"
    changeMode
    exit 1
fi
progress_bar $percent $weight 40

# drvier ko auto insmod
. $sourcedir/script/deal_ko.sh
driver_ko_install
if [ $? -ne 0 ];then
    drvColorEcho "[ERROR]\033[31mDriver_ko_install failed, details in : $logFile \033[0m"
    delete_tmp_ko_dirs
    changeMode
    exit 1
fi

# make drivers not repack in initrd-img.
set_conf_for_dracut
#to set driver ko for selinux
chcon_for_selinux

# to set boot-items for daemons.
if [ $no_kernel_flag = n ]; then
    "${targetdir}"/../host_servers_setup.sh
    if [ $? -ne 0 ]; then
        drvColorEcho  "[ERROR]\033[31mSet boot-items failed, details in : $logFile \033[0m"
        delete_tmp_ko_dirs
        changeMode
        exit 1
    fi
fi

progress_bar $percent $weight 90
# config pcie vnic mac address
pcivnic_mac_set
changeMode

# ensures data flushing
sync
log "[INFO]The sync is executed."
sleep 1

# check whether the uname -a or -r is customized
check_uname_kernel

# if kos aren't in kernel, it will insmod them manually
if [ $force = y ];then
    log "[INFO]The driver is in forcible installation mode."
    delete_tmp_ko_dirs
elif [ $uname_kernel -ne 0 ];then
    log "[INFO]The kernel version of uname -a and uname -r is inconsistent."
    delete_tmp_ko_dirs
else
    driver_ko_install_manually
    if [ $? -ne 0 ];then
        drvColorEcho  "[ERROR]\033[31mDriver_ko_install_manually failed, details in : $logFile \033[0m"
        delete_tmp_ko_dirs
        changeMode
        exit 1
    fi
fi

clr_hdcd_dir
updateVersionInfoVersion
chattrDriver
if [ "$installType"x != "devel"x -a "$installType"x != "docker"x ]; then
    add_modules_load_conf
fi

# ensures data flushing
sync
log "[INFO]The sync is executed."
sleep 1

if [ "${Driver_Install_Mode}" = "debug" ]; then
    if [ -e /proc/debug_switch ]; then
        echo 1 > /proc/debug_switch
    fi
fi

create_white_proc_cfg
progress_bar $percent $weight 100
exit 0