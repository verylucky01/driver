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
Driver_Install_Mode="$(getInstallParam "Driver_Install_Mode" "${installInfo}")"
# used by dkms source compile
export Driver_Install_Mode
[ "${Driver_Install_Mode}" = "vnpu_guest" ] && feature_hot_reset=n
username="$UserName"
usergroup="$UserGroup"
do_dkms=n
no_kernel_flag=n
first_time=y
connect_type=pcie

log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Driver] [$cur_date] $1" >> $logFile
    return 0
}
drvEcho() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[Driver] [$cur_date] $1"
}
drvColorEcho() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo -e "[Driver] [$cur_date] $1"
}
chattrDriver() {
    if [ $installType = "docker" ]; then
        log "[INFO]no need chattr for driver package"
        return 0
    fi
    chattr +i "$Driver_Install_Path_Param"/*.sh > /dev/null 2>&1
    chattr -R +i "$Driver_Install_Path_Param"/driver/* > /dev/null 2>&1
    log "[INFO]add chattr for driver package"
}

# update the base version number
updateVersionInfoVersion() {
    if [ -f "$Driver_Install_Path_Param"/driver/version.info ]; then
        rm -f "$Driver_Install_Path_Param"/driver/version.info
    fi
    cp ./version.info "$Driver_Install_Path_Param"/driver
    setFileChmod -f 444 "$Driver_Install_Path_Param"/driver/version.info
}
# to change files' mode or owner
changeMode(){
    if [ -d "${Driver_Install_Path_Param}" ]; then
        # /etc/hccn.conf
        if [ ! -f "/etc/hccn.conf" ]; then
            touch "/etc/hccn.conf"
        fi
        setFileChmod -f 644 /etc/hccn.conf
        chown -f root:root /etc/hccn.conf

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

drv_vermagic_check() {
    [ $no_kernel_flag = y ] && return 0
    if [ "${Driver_Install_Mode}" = "vnpu_guest" ]; then
        pcie_hdc_ko="$sourcedir"/host/${vnpu_guest_ko}
    else
        pcie_hdc_ko="$sourcedir"/host/${basic_ko}
    fi

    kernel_ver=$(uname -r)
    ko_vermagic=$(modinfo "$pcie_hdc_ko" | grep vermagic | awk '{print $2}')

    primary_kernel_ver=$(echo $kernel_ver | awk -F [-+] '{print $1}')
    primary_ko_vermagic=$(echo $ko_vermagic | awk -F [-+] '{print $1}')
    if [ "$primary_kernel_ver"x != "$primary_ko_vermagic"x ];then
        log "[ERROR]current kernel primary version($kernel_ver) is different from the driver($ko_vermagic)."
        return 1
    fi

    config_modversions=$(cat /boot/config-$kernel_ver | grep CONFIG_MODVERSIONS)
    log "[INFO]config_modversions is : $config_modversions"
    if [ "$config_modversions"x != "CONFIG_MODVERSIONS=y"x ];then
        # full check vermagic
        if [ "$kernel_ver"x != "$ko_vermagic"x ];then
            log "[ERROR]driver ko vermagic($ko_vermagic) is different from the os($kernel_ver), need rebuild driver"
            return 1
        fi

        log "[INFO]current kernel version($kernel_ver) is equal to the driver."
    else
        # don't insmod drv_pcie_host.ko, it will be failed when load boot files
        # test insmod the driver
        log "[INFO]drv_pcie_hdc_host.ko might not load, try to insmod it"
        insmod "$pcie_hdc_ko" >drv_vermagic_check_tmp_log 2>&1
        cat drv_vermagic_check_tmp_log | grep -i "Invalid module format" >>/dev/null
        if [ $? -eq 0 ];then
            log "[ERROR]driver in run packet test load failed, need rebuild driver"
            log "[INFO]test load info :"
            cat drv_vermagic_check_tmp_log >>$logFile
            rm -f drv_vermagic_check_tmp_log
            return 1
        fi

        rm -f drv_vermagic_check_tmp_log
        log "[INFO]driver in run packet could load ok, no need rebuild"
    fi

    return 0
}

device_crl_create() {
    if [ -e $crldir/$crlFile ]; then
        ln -sf $crldir/$crlFile "$targetdir"/device
        log "[INFO]create softlink for $crldir/$crlFile"
    fi
}

drv_dkms_dependent_check() {
    log "[INFO]dkms environment check..."
    local keywords="Status: install ok installed" all_softwares="" ret=1

    # Notice: some OSs have both the dpkg-query and rpm tools.
    all_softwares=$(rpm -qa 2>/dev/null)
    dpkg-query -s dkms 2>/dev/null | grep -q "${keywords}" && ret=$? || ret=$?
    if [ ${ret} -ne 0 ]; then
        echo "${all_softwares}" | grep -q dkms || { log "[WARNING]The program 'dkms' is currently not installed." && return 1; }
    fi

    dpkg-query -s gcc 2>/dev/null | grep -q "${keywords}" && ret=$? || ret=$?
    if [ ${ret} -ne 0 ]; then
        echo "${all_softwares}" | grep -q gcc || { log "[WARNING]The program 'gcc' is currently not installed." && return 1; }
    fi

    dpkg-query -s linux-headers-$(uname -r) 2>/dev/null | grep -q "${keywords}" && ret=$? || ret=$?
    if [ ${ret} -ne 0 ]; then
        echo "${all_softwares}" | grep -q kernel-.*headers-$(uname -r) || { log "[WARNING]linux headers is currently not installed." && return 1; }
        echo "${all_softwares}" | grep -q kernel-.*devel-$(uname -r) || { log "[WARNING]kernel devel is currently not installed." && return 1; }
    fi

    do_dkms=y
    log "[INFO]dkms environment check success"
    return 0
}

drv_dkms_env_check() {
    [ "${no_kernel_flag}" = "y" ] && return 0

    # to check whether EVB environment.
    if [ ! -f "$sourcedir"/script/run_driver_dkms_install.sh ];then
        drv_vermagic_check
        if [ $? -ne 0 ];then
            log "[ERROR]driver vermagic check failed."
            return 1
        fi
        return 0
    else
        drv_dkms_dependent_check
        if [ $? -ne 0 ];then
            log "[INFO]not detect driver dkms dependence "

            # to check whether drivers' version is equal to kernel.
            drv_vermagic_check
            if [ $? -ne 0 ];then
                log "[ERROR]driver vermagic check failed."
                return 1
            fi
            return 0
        fi
        return 0
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

filelist_write_one() {
    echo "[$1] $2" >>$filelist
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
                chcon -t modules_object_t ${sys_path}/${single_ko}* >>$logFile 2>&1
                setFileChmod -f 440 ${sys_path}/${single_ko}*
           fi
        done
        log "[INFO]restorecon -Rv start" 
        restorecon -Rv "$targetdir"/lib64/* >>/dev/null 2>&1
        log "[INFO]restorecon -Rv end"

    fi
    log "[INFO]chcon_for_selinux success"
    return 0
}

delete_tmp_ko_dirs() {
    rm -rf ${targetdir}/host
    rm -rf ${targetdir}/vnpu_host
    rm -rf ${targetdir}/vnpu_guest
}

driver_auto_insmod() {
    local ko=""

    if [ -d ${sys_path} ];then
        filelist_remove driver_ko
        depmod -a > /dev/null 2>&1
    else
        mkdir -p ${sys_path} && log "[INFO]create path: ${sys_path} success" || { log "[ERROR]mkdir -p ${sys_path} failed" && return 1; }
        setFileChmod -f 755 ${sys_path}
    fi

    for ko in ${g_kos}
    do
        cp -f ${ko} ${sys_path}/
        filelist_write_one driver_ko ${sys_path}/${ko##*/}
    done
    depmod -a > /dev/null 2>&1

    log "[INFO]driver ko auto insmod config success"
    return 0
}

driver_dkms_insmod() {
    local source_compile_path="" ko=""

    log "[INFO]rebuild the hiai kernel driver by dkms..."
    ./driver/script/run_driver_dkms_install.sh || return 1

    source_compile_path="$(find /lib/modules/$(uname -r) -type f -name "drv_seclib_host.ko*")"
    [ -z "${source_compile_path}" ] && log "[ERROR]driver_dkms_insmod failed" && return 1
    sys_path="${source_compile_path%/*}"

    for ko in ${g_kos}
    do
        ls ${source_compile_path%/*} | grep -q ${ko##*/} && filelist_write_one driver_ko ${source_compile_path%/*}/${ko##*/}
    done

    return 0
}

# set dracut-cfg for omit-drivers from initrd-img, when new driver install or new kernel install.
set_conf_for_dracut() {
    local ret="" drivers=""
    [ "${no_kernel_flag}" = "y" ] && return 0
    drivers=$(echo "$g_kos" | grep ko | awk -F [/.] '{print $(NF-1)}')

    if which dracut > /dev/null 2>&1; then
        if [ -e /etc/dracut.conf.d ]; then
            if [ -f /etc/dracut.conf.d/Ascend-dracut.conf ]; then
                rm -f /etc/dracut.conf.d/Ascend-dracut.conf
            fi
            echo "# Ascend davinci drivers shouldn't be repacked to initrd-img." > /etc/dracut.conf.d/Ascend-dracut.conf
            echo omit_drivers+=\"${drivers}\" >> /etc/dracut.conf.d/Ascend-dracut.conf
            chmod -f 440 /etc/dracut.conf.d/Ascend-dracut.conf
            log "[INFO]make drivers in dracut-cfg success"
        fi
    fi

    return 0
}

driver_ko_install() {
    [ ${no_kernel_flag} = y ] && return 0

    # to check storing ko directory exist or not.
    [ ! -d "${targetdir}/host" ] && log "[ERROR]${targetdir}/host ko is lost" && return 1

    if [ "${feature_virt_scene}" = "y" ]; then
        # install cmd: --full/--run
        if [ "${Driver_Install_Mode}" = "normal" ]; then
            rm -f $(ls ${targetdir}/host/* | grep -vEw "${mode_normal}")
        # install cmd: --full/--run --vnpu_host
        elif [ "${Driver_Install_Mode}" = "vnpu_host" ]; then
            rm -f $(ls ${targetdir}/host/* | grep -vEw "${mode_vnpu_host}")
        # install cmd: --full/--run --vnpu_docker_host
        elif [ "${Driver_Install_Mode}" = "vnpu_docker_host" ]; then
            rm -f $(ls ${targetdir}/host/* | grep -vEw "${mode_vnpu_docker_host}")
        # install cmd: --full/--run --vnpu_guest
        elif [ "${Driver_Install_Mode}" = "vnpu_guest" ]; then
            rm -f $(ls ${targetdir}/host/* | grep -vEw "${mode_vnpu_guest}")
        fi
    fi
    g_kos="$(ls ${targetdir}/host/*)"

    # make drivers not repack in initrd-img.
    set_conf_for_dracut

    # when compile by dkms, KO will be insmoded by it automatically.
    # drv_pcie_host: Physical machine in OBP
    # drv_vpcie: Compute-slicing virtual-machine in OBP
    lsmod | grep -E "drv_pcie_host|drv_vpcie" >& /dev/null && first_time=n

    sys_path="/lib/modules/$(uname -r)/updates" && log "[INFO]ko default put-path ${sys_path}"
    if [ "${do_dkms}" = "n" ];then
        driver_auto_insmod || { log "[ERROR]driver_auto_insmod failed" && return 1; }
    else
        export g_kos
        driver_dkms_insmod || { log "[ERROR]dkms install failed" && return 1; }
    fi

    return 0
}

pcie_load_path_set() {
    [ ${no_kernel_flag} = y ] && return 0

    echo DAVINCI_HOME_PATH="${targetdir%/*}" > /lib/davinci.conf
    if [ $? -ne 0 ];then
        log "[ERROR]echo DAVINCI_HOME_PATH=${targetdir%/*} > /lib/davinci.conf failed"
        return 1
    fi

    log "[INFO]show /lib/davinci.conf :"`cat /lib/davinci.conf` >>$logFile
    log "[INFO]set /lib/davinci.conf success"

    return 0
}

rand() {
    min=$1
    max=$(($2-$min+1))
    num=$(cat /dev/urandom | head -n 10 | cksum | awk -F ' ' '{print $1}')
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

# mac 10:1b:54:xx:xx:xx is allocated to huawei
pcivnic_mac_set() {
    [ $no_kernel_flag = y ] && return 0
    # vnic.ko does not exist in the ub device, just return
    [ "$connect_type" = "ub" ] && return 0
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

driver_file_copy() {
    rm -rf /etc/hdcBasic.cfg
    if [ $input_install_for_all == n ]; then
        ./install_common_parser.sh --makedir --username="$username" --usergroup="$usergroup" "$installType" "$installPathParam" ./filelist.csv
    else
        ./install_common_parser.sh --makedir --install_for_all --username="$username" --usergroup="$usergroup" "$installType" "$installPathParam" ./filelist.csv
    fi
    if [ $? -ne 0 ];then
        log "[ERROR]driver mkdir failed"
        return 1
    fi
    if [ $input_install_for_all == n ]; then
        ./install_common_parser.sh --copy --username="$username" --usergroup="$usergroup" "$installType" "$installPathParam" ./filelist.csv
    else
        ./install_common_parser.sh --copy --install_for_all --username="$username" --usergroup="$usergroup" "$installType" "$installPathParam" ./filelist.csv
    fi
    if [ $? -ne 0 ];then
        log "[ERROR]driver copy failed"
        return 1
    fi
    # set bash env
    BASH_ENV_PATH="${Driver_Install_Path_Param}/driver/bin/setenv.bash"
    if [ -f "install_common_parser.sh" ] && [ -f "${BASH_ENV_PATH}" ]; then
        ./install_common_parser.sh --add-env-rc --package="driver" --username="${username}" --usergroup="${usergroup}" --setenv "$installPathParam" "${BASH_ENV_PATH}" "bash"
        if [ $? -ne 0 ]; then
            log "[ERROR]ERR_NO:0x0089;ERR_DES:failed to set bash env."
            return 1
        fi
    fi
    if [ $input_install_for_all == n ]; then
        ./install_common_parser.sh --chmoddir --username="$username" --usergroup="$usergroup" "$installType" "$installPathParam" ./filelist.csv
    else
        ./install_common_parser.sh --chmoddir --install_for_all --username="$username" --usergroup="$usergroup" "$installType" "$installPathParam" ./filelist.csv
    fi
    if [ $? -ne 0 ];then
        log "[ERROR]driver chmoddir failed"
        return 1
    fi

    # driver/kernel
    if [ -d "${Driver_Install_Path_Param}"/driver/kernel ]; then
        find -L "${Driver_Install_Path_Param}"/driver/kernel -type l -! -exec test -e {} \; -delete
        chmod -Rf 400 "${Driver_Install_Path_Param}"/driver/kernel
        find "${Driver_Install_Path_Param}"/driver/kernel -type d -name "*" | xargs chmod 500
    fi
    return 0
}

progress_bar(){
    local parent_progress=$1 weight=$2 child_progress=$3
    local output_progress
    output_progress=`awk 'BEGIN{printf "%d\n",('$parent_progress'+'$weight'*'$child_progress'/100)}'`
    drvEcho "[INFO]upgradePercentage:${output_progress}%"
    log "[INFO]upgradePercentage:${output_progress}%"
}

is_pci_link_success()
{
    local bdf chip_id
    local pci_dev_path="/sys/bus/pci/devices"
    local times=1 wait_times=9 up_chips=() up_index=0

    . ./driver/script/specific_func.inc
    get_pci_info

    if [ ${#g_bdfs[@]} -eq 0 ]; then
        log "[WARNING]There is no chip matching the driver package."
        return 1
    fi

    # it will wait 2*9=18s for link-up at most.
    for times in $(seq 1 ${wait_times})
    do
        for bdf in ${g_bdfs[*]}
        do
            if echo " ${up_chips[*]} " | grep -q " ${bdf} "; then
                # if the device has been linked up successfully, it will skip here.
                continue
            fi

            if [ ! -e ${pci_dev_path}/${bdf}/chip_id ]; then
                log "[INFO]times=[${times}]. The file [${pci_dev_path}/${bdf}/chip_id] does not exist."
                continue
            fi

            chip_id=$(cat ${pci_dev_path}/${bdf}/chip_id)
            if [ "${chip_id}" = "-1" ]; then
                log "[INFO]times=[${times}]. The bdf=[${bdf}] of chip_id=[${chip_id}] is invalid."
                continue
            fi
            log "[INFO]times=[${times}]. The device bdf=[${bdf}] link up successfully."

            up_chips[${up_index}]="${bdf}"
            up_index=$(expr ${up_index} + 1)

            [ ${g_dev_nums} -eq ${up_index} ] && return 0
        done

        sleep 2
    done

    return 1
}

is_ub_link_success() {
    # 该功能跟随后续通信组维测需求开发，当前打桩
    return 0
}

is_device_link_success() {
    if [ "$connect_type" == "pcie" ]; then
        is_pci_link_success
    else
        is_ub_link_success
    fi
}

modprobe_host_kos() {
    local ko="" module_full_name="" module_name="" modprobe_kos=""

    modprobe_kos=$(echo "${g_kos}" | grep -v "drv_seclib_host.ko")
    # ko's value is absolute path, eg: /usr/local/Ascend/driver/host/drv_seclib_host.ko.
    for ko in ${modprobe_kos}
    do
        # module_full_name's value only contains ko's full name, eg: drv_seclib_host.ko.
        module_full_name=${ko##*/}
        # module_name's value, which is just like searching with command 'lsmod', is ko's brief name, eg: drv_seclib_host
        module_name=${module_full_name%%.*}
        if [ "${module_name}" = "host_notify" ] || [ "${module_name}" = "drv_vascend" ];then
            modprobe ${module_name} >& /dev/null
            if [ $? -ne 0 ]; then
                log "[INFO]${module_name} modprobe skipped"
            else
                log "[INFO]${module_name} modprobe success"
            fi
        else
            modprobe ${module_name} >& /dev/null
            if [ $? -ne 0 ]; then
                log "[ERROR]${module_name} modprobe failed"
                return 1
            fi
            log "[INFO]${module_name} modprobe success"
        fi
    done

    return 0
}

virt_install_check() {
    local env="unknown"
    local bdf_num=$(echo "${device_bdf//0x/}")
    local dev_unlink_cnt=0
    local dev_rev_msg=($(lspci | grep -E ${bdf_num} | grep -oP 'rev\s+\K.*?(?=\))'))
    local dev_total_num=${#dev_rev_msg[@]}

    for rev in "${dev_rev_msg[@]}"; do
        if [ "${rev}" == "71" ]; then
            env="cgroup"
        fi

        if [ "${rev}" == "ff" ]; then
            dev_unlink_cnt=$((dev_unlink_cnt + 1))
        fi
    done

    if [ "${env}" == "unknown" ] && [ "${dev_unlink_cnt}" -lt "${dev_total_num}" ]; then
        env="passthrough"
    fi

    log "[INFO]Get device status env=${env}, dev_unlink_cnt=${dev_unlink_cnt}, dev_total=${dev_total_num}"

    if [ "${Driver_Install_Mode}" == "vnpu_guest" ]; then
        if [ "${env}" == "passthrough" ]; then
            drvColorEcho "[ERROR]\033[31mNot support install with \"--vnpu_guest\" in pass-through scenario, details in : $logFile \033[0m"
            changeMode
            exit 1
        fi
    fi

    if [ "${env}" == "unknown" ]; then
        log "[WARNING]Unable to obtain device status."
    fi
}

driver_ko_install_manually() {
    [ "$no_kernel_flag" = "y" ] && return 0

    if [ $feature_hot_reset = y ];then
        if [ -e "$hotreset_status_file" ];then
            hotreset_status=`cat "$hotreset_status_file"`
            hotreset_time=${hotreset_status#*.}
            hotreset_status=${hotreset_status%.*}
        else
            hotreset_status="unknown"
        fi
    fi

    log "[INFO]install ko, hot reset status: $hotreset_status, first_time:$first_time"
    if [ "$first_time" = "y" ] || [ "$hotreset_status" = "scan_success" ];then
        if [ $feature_hot_reset = y ];then
            drvEcho "[INFO]Waiting for device startup..."

            cur_time=$(date +%s)
            if [ "$hotreset_status" = "scan_success" ]; then
                if (($cur_time - $hotreset_time < 10)) && (($cur_time - $hotreset_time > 0)); then
                    sleep_time=$((10 + $hotreset_time - $cur_time))
                    log "[INFO]wait "$sleep_time"s for device bios"
                    sleep $sleep_time
                else
                    log "[INFO]time's messed up, it will sleep 10s."
                    sleep 10
                fi
            fi

            # to insmod ko manually
            modprobe_host_kos || return 1

            is_device_link_success
            if [ $? -eq 1 ]; then
                drvEcho "[WARNING]Device startup abnormal"
                hotreset_status="unknown"
            else
                ko_insert_time=$(date +%s)
                if (($ko_insert_time - $cur_time > 200)); then
                    drvEcho "[WARNING]Device startup timeout"
                    hotreset_status="unknown"
                else
                    drvEcho "[INFO]Device link up successfully."
                fi
            fi
        else
            # to insmod ko manually
            modprobe_host_kos || return 1
        fi

        sleep 1
        # startup servers
        if [ ! -f "${targetdir}"/../host_sys_init.sh ];then
            log "[ERROR]host_sys_init.sh is not existed, setup servers failed"
            return 1
        fi
        "${targetdir}"/../host_sys_init.sh start;ret=$?
        log "[INFO]setup servers finish"

        if [ "$hotreset_status" = "scan_success" -a $ret -eq 0 ]; then
            log "[INFO]hot reset success"
            echo "success" > "$hotreset_status_file"
        fi
    fi

    delete_tmp_ko_dirs
    log "[INFO]remove ko-put-path success"

    return 0
}

clr_hdcd_dir() {
    if [ -d /var/log/npu/hdcd ]; then
       rm -rf /var/log/npu/hdcd &
    fi
}

add_modules_load_conf() {
    source "${sourcedir}"/script/common_func.inc
    get_os_info
    if [ "${HostOsName}" == "CentOS" ]; then
        if [ -e /etc/modules-load.d/ascend-fat.conf ]; then
            rm -f /etc/modules-load.d/ascend-fat.conf
        fi
        echo -e "# Load fat and vfat at boot\nfat\nvfat" > /etc/modules-load.d/ascend-fat.conf
        chmod 644 /etc/modules-load.d/ascend-fat.conf
    fi
}

get_connect_type() {
    if [ -d "/sys/bus/ub" ]; then
        connect_type="ub"
    else
        connect_type="pcie"
    fi
}

# start!
installPathParam="$1"
installType="$2"
no_kernel_flag=n
percent=0
weight=100

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
. ./driver/script/specific_func.inc
if [ "${feature_virt_scene}" = "y" ] && [ "$installType"x = "full"x -o "$installType"x = "run"x ]; then
    virt_install_check
fi
drv_dkms_env_check
if [ $? -ne 0 ];then
    drvColorEcho "[ERROR]\033[31mDrv_dkms_env_check failed, details in : $logFile \033[0m"
    changeMode
    exit 1
fi
get_connect_type
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
    if [ $installType = "docker" ]; then
        cp -f ${installPathParam}/driver/tools/device_boot_init.sh /usr/bin/
        setFileChmod -f 550 /usr/bin/device_boot_init.sh
    else
        chattr -i /usr/bin/device_boot_init.sh >& /dev/null
        cp -f ${installPathParam}/driver/tools/device_boot_init.sh /usr/bin/
        setFileChmod -f 550 /usr/bin/device_boot_init.sh
        chattr +i /usr/bin/device_boot_init.sh >& /dev/null
    fi
fi
#check CRL of images
if [ $feature_crl_check = y ]; then
    if [ $no_kernel_flag = n ]; then
        . ./driver/script/device_crl_check.sh
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
driver_ko_install
if [ $? -ne 0 ];then
    drvColorEcho "[ERROR]\033[31mDriver_ko_install failed, details in : $logFile \033[0m"
    delete_tmp_ko_dirs
    changeMode
    exit 1
fi

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
sync

# if kos aren't in kernel, it will insmod them manually
driver_ko_install_manually
if [ $? -ne 0 ];then
    drvColorEcho  "[ERROR]\033[31mDriver_ko_install_manually failed, details in : $logFile \033[0m"
    delete_tmp_ko_dirs
    changeMode
    exit 1
fi
clr_hdcd_dir
updateVersionInfoVersion
chattrDriver
if [ "$installType"x != "docker"x -a "$installType"x != "devel"x ]; then
    add_modules_load_conf
fi
sync
progress_bar $percent $weight 100
exit 0
