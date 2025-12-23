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

isCloud() {
    isCloud_driver=$(echo $device_bdf | grep d801)
    if [ ! -z "$isCloud_driver" ]; then
        sleep 60
    fi
}

is_hot_reset_success()
{
    local bdf
    local pci_dev_path="/sys/bus/pci/devices"

    . $sourcedir/script/specific_func.inc
    get_pci_info
    for bdf in ${g_bdfs[*]}
    do
        if [ ! -e ${pci_dev_path}/${bdf}/chip_id ]; then
            log "[INFO]bdf:${bdf}, dev id: $(cat ${pci_dev_path}/${bdf}/dev_id 2>/dev/null), hot reset not finish"
            return 1
        else
            log "[INFO]bdf:${bdf}, dev id: $(cat ${pci_dev_path}/${bdf}/dev_id 2>/dev/null), hot reset finish"
        fi
    done

    return 0
}

modprobe_host_kos() {
    local ko="" module_full_name="" module_name="" modprobe_kos=""

    modprobe_kos=$(echo "${g_kos}" | grep -Ev "drv_seclib_host.ko|npu_peermem.ko")
    # ko's value is absolute path, eg: /usr/local/Ascend/driver/host/drv_seclib_host.ko.
    for ko in ${modprobe_kos}
    do
        # module_full_name's value only contains ko's full name, eg: drv_seclib_host.ko.
        module_full_name=${ko##*/}
        # module_name's value, which is just like searching with command 'lsmod', is ko's brief name, eg: drv_seclib_host
        module_name=${module_full_name%%.*}
        if [ "${module_name}" = "host_notify" ] || [ "${module_name}" = "drv_vascend" ] || [ "${module_name}" = "tc_pcidev" ]  || [ "${module_name}" = "ascend_dms_mng" ];then
            modprobe ${module_name} >& /dev/null
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

rm_old_seclib() {
    lsmod | grep "drv_seclib_host" > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        log "[INFO]rmmod drv_seclib_host"
        rmmod drv_seclib_host >& /dev/null || { log "[WARNING]rmmod drv_seclib_host failed"; }
    fi
}

isMini() {
    isMini_driver=$(echo $device_bdf | grep d100)
    if [ ! -z "$isMini_driver" ]; then
        rmmod dbl_runenv_config >>/dev/null 2>&1
    fi
}

filelist_write_one() {
    echo "[$1] $2" >>$filelist
}

driver_ko_install_manually() {
    export IS_CALLED_ON_INSTALLING=true
    [ "$no_kernel_flag" = "y" ] && return 0

    if [ $feature_hot_reset = y ];then
        if [ -e "$hotreset_status_file" ];then
            hotreset_status=`cat "$hotreset_status_file"`
            hotreset_time=$(echo ${hotreset_status#*.} | awk '{print $1}')
            hotreset_status=${hotreset_status%.*}
        else
            hotreset_status="unknown"
        fi
    fi

    log "[INFO]install ko, hot reset status: $hotreset_status, first_time:$first_time"
    if [ "$hotreset_status" != "ko_abort" ] && [ "$first_time" = "y" ] || [ "$hotreset_status" = "scan_success" ];then
        # if is 310 Card,rmmod dbl_runenv_config.
        # This is to fix the problem that the previous uninstall script did not uninstall dbl_runenv_config.
        isMini
        rm_old_seclib
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

            is_hot_reset_success
            if [ $? -eq 1 ]; then
                drvEcho "[WARNING]Device startup fail"
                hotreset_status="unknown"
            else
                ko_insert_time=$(date +%s)
                if (($ko_insert_time - $cur_time > 200)); then
                    drvEcho "[WARNING]Device startup timeout"
                    hotreset_status="unknown"
                else
                    drvEcho "[INFO]Device startup success"
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

    isCloud

    delete_tmp_ko_dirs
    log "[INFO]remove ko-put-path success"

    return 0
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

    # some os ksecurec no need insmod
    local current_seclib="ksecurec"
    lsmod | grep ${current_seclib} > /dev/null 2>&1
    if [ $? -eq 0 ];then
        log "[INFO]${current_seclib} is exist, no need insmod drv_seclib_host"
        local no_sec_ko=y
    fi

    for ko in ${g_kos}
    do
        if [ "${no_sec_ko}"x == "yx" ] && [ "${ko##*/}" == "drv_seclib_host.ko" ];then
            continue
        fi
        if [ "${PACKAGE_TYPE}" != "run" ]; then
            if [ ! -f "/etc/uvp-release" ] && [ "${ko##*/}" == "drv_vascend.ko" ];then
                continue
            fi
        fi
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
    $sourcedir/script/run_driver_dkms_install.sh || return 1

    source_compile_path="$(find /lib/modules/$(uname -r) -type f -name "drv_seclib_host.ko*")"
    [ -z "${source_compile_path}" ] && log "[ERROR]driver_dkms_insmod failed" && return 1
    sys_path="${source_compile_path%/*}"

    for ko in ${g_kos}
    do
        if [ "${PACKAGE_TYPE}" != "run" ]; then
            if [ ! -f "/etc/uvp-release" ] && [ "${ko##*/}" == "drv_vascend.ko" ];then
                continue
            fi
        fi
        ls ${source_compile_path%/*} | grep -q ${ko##*/} && filelist_write_one driver_ko ${source_compile_path%/*}/${ko##*/}
    done

    return 0
}

driver_ko_install() {
    [ ${no_kernel_flag} = y ] && return 0

    # to check storing ko directory exist or not.
    [ ! -d "${targetdir}/host" ] && log "[ERROR]${targetdir}/host ko is lost" && return 1

    if [ "${feature_virt_scene}" = "y" ]; then
        # install cmd: --full/--run
        if [ "${Driver_Install_Mode}" = "normal" -o "${Driver_Install_Mode}" = "debug" ]; then
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

    # when compile by dkms, KO will be insmoded by it automatically.
    # drv_pcie_host: Physical machine
    # drv_vpcie: Compute-slicing virtual-machine
    lsmod | grep -E "drv_pcie_host|drv_vpcie|tc_pcidev" >& /dev/null && first_time=n

    sys_path="/lib/modules/$(getKernelVer)/updates" && log "[INFO]ko default put-path ${sys_path}"
    if [ "${do_dkms}" = "n" ]; then
        driver_auto_insmod || { log "[ERROR]driver_auto_insmod failed" && return 1; }
    else
        export g_kos
        driver_dkms_insmod || { log "[ERROR]dkms install failed" && return 1; }
    fi
    return 0
}