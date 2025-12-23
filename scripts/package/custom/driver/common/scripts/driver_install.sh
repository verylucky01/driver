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

install_path_should_belong_to_root() {
    local ret=0

    # install drv first time
    if [ ${first_time_install_drv_flag} = "Y" ]; then
        # default dir doesn't exist or exists, but permission not correct
        if [ "${Install_Path_Param}" = "/usr/local/Ascend" ]; then
            mkdir -p "${Install_Path_Param}"
            setFileChmod -f 755 "${Install_Path_Param}"
            chown -fh root:root "${Install_Path_Param}" >& /dev/null
        fi

        parent_dirs_permision_check "${Install_Path_Param}" && ret=$? || ret=$?
        if [ ${ret} -eq 1 ]; then
            log "[ERROR][${short_install_dir}] permision not right, it should belong to root."
            drvColorEcho  "[ERROR]\033[31mThe given directory, including its parents, should belong to root, details in : $logFile \033[0m"
            return 1
        elif [ ${ret} -eq 2 ]; then
            log "[ERROR][${short_install_dir}] permission is too small."
            drvColorEcho  "[ERROR]\033[31mThe given directory, or its parents, permission is too small, details in : $logFile \033[0m"
            return 1
        fi
    # it has already installed before.
    else
        # if upgrade and doesn't give install-path
        if [ "${old_drv_install_path}" = "${Install_Path_Param}" ]; then
            # if upgrade, default path
            if [ "${Install_Path_Param}" = "/usr/local/Ascend" ] || [ "${Install_Path_Param}" = "/usr/local/HiAI" ]; then
                setFileChmod -f 755 "${Install_Path_Param}"
                chown -fh root:root "${Install_Path_Param}" >& /dev/null
            fi
            # dir permission check
            parent_dirs_permision_check "${Install_Path_Param}" && ret=$? || ret=$?
            # --quiet
            [ ${quiet} = y ] && [ ${ret} -ne 0 ] && return 0

            if [ ${ret} -ne 0 ]; then
                echo "You are going to put run-files on a unsecure install-path, do you want to continue? [y/n]"
                while true
                do
                    read yn
                    if [ "$yn" = n ]; then
                        echo "Stop installation!"
                        return 1
                    elif [ "$yn" = y ]; then
                        break;
                    else
                        echo "ERR_NO:0x0002;ERR_DES:input error, please input again!"
                    fi
                done
            fi
        # upgrade and give install-path
        else
            parent_dirs_permision_check "${Install_Path_Param}" && ret=$? || ret=$?
            if [ ${ret} -ne 0 ]; then
                log "[ERROR]the given dir, or its parents, permission is invalid."
                drvColorEcho " [ERROR]\033[31mThe given dir, or its parents, permission is invalid, details in : $logFile \033[0m"
                return 1
            fi
        fi
    fi

    return 0
}

parent_dirs_permision_check(){
    current_dir="$1" parent_dir="" short_install_dir=""
    local owner="" mod_num=""

    parent_dir=$(dirname "${current_dir}")
    short_install_dir=$(basename "${current_dir}")
    log "[INFO]parent_dir value is [${parent_dir}] and children_dir value is [${short_install_dir}]"

    if [ "${current_dir}"x = "/"x ]; then
        log "[INFO]parent_dirs_permision_check success"
        return 0
    else
        owner=$(stat -c %U "${parent_dir}"/"${short_install_dir}")
        if [ "${owner}" != "root" ]; then
            log "[WARNING][${short_install_dir}] permision isn't right, it should belong to root."
            return 1
        fi
        log "[INFO][${short_install_dir}] belongs to root."

        mod_num=$(stat -c %a "${parent_dir}"/"${short_install_dir}")
        mod_num=${mod_num: -3}
        if [ ${mod_num} -lt 755 ] && [ ${input_install_for_all} == n ]; then
            log "[WARNING][${short_install_dir}] permission is too small, it is recommended that the permission be 755 for the root user."
            return 2
        elif [ ${mod_num} -eq 755 ] && [ ${input_install_for_all} == n ]; then
            log "[INFO][${short_install_dir}] permission is ok."
        else
            if [ ${input_install_for_all} == n ]; then
                log "[WARNING][${short_install_dir}] permission is too high, it is recommended that the permission be 755 for the root user."
            fi
        fi
        if [ ${mod_num} -lt 750 ] && [ ${input_install_for_all} == y ]; then
             log "[WARNING][${parent_dir}/${short_install_dir}] permission is too small."
             return 2
        fi

        parent_dirs_permision_check "${parent_dir}"
    fi
}




backupKmsData(){
    # Backup kmsagent data to backup_data.
    if [ -f "./driver/script/kmsagent_backup_conf.sh" ]; then
        chown -hR $username:$usergroup $KMSAGENT_WORK_DIR > /dev/null 2>&1
        ./driver/script/kmsagent_backup_conf.sh $KMSAGENT_WORK_DIR $username $usergroup >>$logFile 2>&1
    fi
}



# determine whether the command returns a failure, and report an error if it fails.
# $1:Operation type, $2:ret code,   $3:(Optional)print messages
error() {
    local operation="$1"
    local retcode="$2"
    local msg="$3"
    if [ ${retcode} -ne 0 ]; then
        logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_FAILED}" "${installType}" "${all_parma}"
    fi

    if [ $# -eq 3 ]; then
        if [ ${retcode} -ne 0 ]; then
            drvEcho "[ERROR]${msg}"
            log "[ERROR]${msg}"
            exitLog 1
        fi
    elif [ $# -eq 2 ]; then
        if [ ${retcode} -ne 0 ]; then
            exitLog 1
        fi
    fi
}

# check whether hotreset fails
isHotresetFailed(){
    if [ "${hotreset_status}" = "scan_success"* ] || [ "${hotreset_status}" = "success" ]; then
        echo "false"
    else
        echo "true"
    fi
}



kernel_version_is_upgrade() {
    dev_num=$(lspci -tv | grep d801 | wc -l)
    if [ $dev_num -eq 1 ] || [ $dev_num -eq 2 ];then
        main=$(uname -r | awk -F . '{print $1}')
        minor=$(uname -r | awk -F . '{print $2}')
        if [ $main -lt 4 ];then
            echo "[WARNING]Please upgrade kernel version."
        else
            if [ $minor -le 5 ] && [ $main -eq 4 ];then
                echo "[WARNING]Please upgrade kernel version."
            fi
        fi
    fi
}

# update /etc/ascend_install.info
updateInstallInfo() {
    local installMode_before=""

    if [ -d "$installInfo" ]; then
        rm -rf "$installInfo"
    fi
    if [ -f "$installInfo" ]; then
        UserName=$(getInstallParam "UserName" "${installInfo}")
        UserGroup=$(getInstallParam "UserGroup" "${installInfo}")
        Driver_Install_Type=$(getInstallParam "Driver_Install_Type" "${installInfo}")
        Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
        Driver_Install_For_All=$(getInstallParam "Driver_Install_For_All" "${installInfo}")
        if [ -z "$UserName" ]; then
            updateInstallParam "UserName" "$username" "$installInfo"
        fi
        if [ -z "$UserGroup" ]; then
            updateInstallParam "UserGroup" "$usergroup" "$installInfo"
        fi

    else
        createFile "$installInfo" "root":"root" 644
        updateInstallParam "UserName" "$username" "$installInfo"
        updateInstallParam "UserGroup" "$usergroup" "$installInfo"
    fi
    updateInstallParam "Driver_Install_Type" "$installType" "$installInfo"
    updateInstallParam "Driver_Install_Path_Param" "$Install_Path_Param" "$installInfo"
    updateInstallParam "Driver_Install_For_All" "$install_for_all" "$installInfo"
    if [ "${upgrade}" = "y" ]; then
        installMode_before="$(getInstallParam "Driver_Install_Mode" "${installInfo}")"
        if [ -z "${installMode_before}" -o "${installMode_before}" = "debug" ]; then
            updateInstallParam "Driver_Install_Mode" "${installMode}" "${installInfo}"
        else
            updateInstallParam "Driver_Install_Mode" "${installMode_before}" "${installInfo}"
        fi
    else
        updateInstallParam "Driver_Install_Mode" "${installMode}" "${installInfo}"
    fi
    setFileChmod -f 644 $installInfo
}

initKmsagentService(){
    local new_path="${Install_Path_Param%/}"
    local kms_library_path="${new_path}/driver/lib64/inner:${new_path}/driver/lib64:${new_path}/driver/lib64/driver:${new_path}/driver/lib64/common"
    local kms_restore_path="${new_path}/driver/script/kmsagent_restore_conf.sh"

    if [ $docker != y ] && [ $devel != y ]; then

        if [ ! -f "./driver/script/kmsagent.service" ];then
            # Not need init kmsagent service.
            return 0
        fi
        if [ $1 == "false" ] && [ -f "./driver/script/kmsagent_install.sh" ]; then
            ./driver/script/kmsagent_install.sh ${new_path}/driver/tools/kmsagent ${new_path}/driver/script/kmsagent.conf $KMSAGENT_WORK_DIR $username $usergroup $kms_library_path $kms_restore_path "false" >>$logFile 2>&1
            return $?
        fi
        # 1. Create driver config file
        if [ -d $KMSAGENT_SERVICE_DIR ]; then
            KMSAGENT_SERVICES="${KMSAGENT_SERVICE_DIR}/${KMSAGENT_SERVICE_NAME}"
            source ./driver/script/kmsagent.service
            systemctl daemon-reload 1>/dev/null 2>&1
        elif [ -d $KMSAGENT_USER_SERVICE_DIR ]; then
            KMSAGENT_SERVICES="${KMSAGENT_USER_SERVICE_DIR}/${KMSAGENT_SERVICE_NAME}"
            source ./driver/script/kmsagent.service
            systemctl daemon-reload 1>/dev/null 2>&1
        else
            log "[WARNING]The current environment does not support kmsagent services and can't use the model protection function."
            drvEcho "[WARNING]The current environment does not support kmsagent services and can't use the model protection function."
        fi

        createFile "$ASCEND_DRIVER_CFG_FILE" "root":"root" 644
        sed -i "/${KMSAGENT_CONFIG_ITEM}=/d" "${ASCEND_DRIVER_CFG_FILE}"
        if [ $kmsagent_is_enable = y ];then
            echo "${KMSAGENT_CONFIG_ITEM}=enable" >> "${ASCEND_DRIVER_CFG_FILE}"
        else
            echo "${KMSAGENT_CONFIG_ITEM}=disable" >> "${ASCEND_DRIVER_CFG_FILE}"
        fi
        setFileChmod -f 644 $ASCEND_DRIVER_CFG_FILE

        # Init kmsagent log file.
        local user_home_path=`eval echo "~${username}"`
        chown -fh $username:$usergroup ${user_home_path}/kmsagent_*.log >& /dev/null
        log "[INFO]Switch to the file owner to config the kmsagent_log in the home dir: $?"

        chown -fh $username:$usergroup ${KMSAGENT_WORK_DIR}/kmsagent_*.log >& /dev/null
        log "[INFO]Switch to the file owner to config the kmsagent_log in the work dir: $?"

        # 2. Create kmsagent work dir and restore backup data.
        if [ -f "./driver/script/kmsagent_install.sh" ]; then
        ./driver/script/kmsagent_install.sh ${new_path}/driver/tools/kmsagent ${new_path}/driver/script/kmsagent.conf $KMSAGENT_WORK_DIR $username $usergroup $kms_library_path $kms_restore_path "true" >>$logFile 2>&1
        fi
        # 3. Auto start kms service.
        if [ $kmsagent_is_enable = y ]; then
            ${new_path}/driver/script/${ASCEND_DRIVER_SETUP_SCRIPT} $KMSAGENT_CONFIG_ITEM start >> $runLogFile 2>&1 &
        fi
    fi
}

installationCompletionMessage() {
    local run_install_type="$1"
    local new_path

    new_path="${Install_Path_Param%/}"
    if [ -e "$hotreset_status_file" ]; then
        hotreset_status=`cat "$hotreset_status_file"`
        rm -f "$hotreset_status_file"
    else
        hotreset_status="unknown"
    fi

    # Convert it to lowercase.
    run_install_type="${run_install_type,,}"
    # delete 'e' from install type.
    run_install_type="${run_install_type/e/}ed"

    local start_time=$(date +%s)
    local cnt=0
    local output boot_status_cnt boot_status_oks
    local tool_path="$new_path/driver/tools/upgrade-tool"
    if [[ -e ${tool_path} && "${hotreset_status}" == "success" ]] || \
        [[ "$first_time_install_drv_flag" == "Y" && "$PCIE_INSMOD_STATUS_BEFORE_INSTALL" == "n" && "$force" == "n" ]]; then
        hotreset_status="unknown"
        while (($(date +%s) - start_time < 60)) || (( cnt < 3 ))
        do
            (( cnt += 1 ))
            output=$(timeout 20s ${tool_path} --device_index -1 --boot_status 2>&1)
            if (( $? != 124 )); then  # Error code 124 indicates timeout
                boot_status_cnt=$(echo "${output}" | grep -c 'boot status')
                boot_status_oks=$(echo "${output}" | grep -c 'boot status:16')
                if (( boot_status_cnt > 0 && boot_status_cnt == boot_status_oks )); then
                    # Got boot status and all boot status is 16
                    "${Driver_Install_Path_Param}"/driver/script/run_driver_upgrade_check.sh -s &>/dev/null &
                    hotreset_status="success"
                    break
                fi
            fi
            sleep 10
        done
        if [ -e $davinci_num_file ]; then
            source $davinci_num_file
            log "[INFO]Scan_davinci_num : $scan_davinci_num, all_davinci_num : $all_davinci_num"
        fi
        if [ "$hotreset_status"x = "unknown"x ] || [ -e $davinci_num_file -a $scan_davinci_num -ne $all_davinci_num ]; then
            drvEcho "[WARNING]There are some chips not ready, please check after reboot"
            log "[WARNING]There are some chips not ready, please check after reboot"
            rm -f  $davinci_num_file
        fi
    fi

    if [ "$hotreset_status"x = "success"x ] || [ "$installType"x = "docker"x ] || [ "$installType"x = "devel"x ] ; then
        drvColorEcho  "[INFO]\033[32mDriver package ${run_install_type} successfully! The new version takes effect immediately. \033[0m"
        if [ "$installType"x = "docker"x ];then
            # lib-put-path is different from the others.
            if [ "${lib_put_path}"x = "specific"x ]; then
                echo -e "Please make sure that\n    - LD_LIBRARY_PATH includes $new_path/driver/lib64\n"\
                        "   - Please refer to the instruction manual for specific details"
            else
                echo -e "Please make sure that\n    - LD_LIBRARY_PATH includes $new_path/driver/lib64/common:$new_path/driver/lib64/driver\n"\
                        "   - Please refer to the instruction manual for specific details"
            fi
        fi
    else
        if [ "$hotreset_status"x = "ko_abort"x ]; then
            drvColorEcho "[INFO]\033[32mDriver package ${run_install_type} finished! \033[0m"
            drvColorEcho "[WARNING]\033[33mKernel modules can not be removed, reboot needed for installation/upgrade to take effect! \033[0m"
        else
            drvColorEcho "[INFO]\033[32mDriver package ${run_install_type} successfully! \033[0m\033[31mReboot needed for installation/upgrade to take effect! \033[0m"
        fi
    fi
}

installRun() {
    if [ $full_install = y ] || [ $debug = y ] || [ $upgrade = y ] || [ $run = y ]; then
        [ "$Driver_Install_Type" = "docker" ] || kernel_version_is_upgrade
        if [ ! -f /etc/vnpu.cfg ]; then
            echo "vnpu_config_recover:enable" >> /etc/vnpu.cfg
            echo "[vnpu-config start]" >> /etc/vnpu.cfg
            echo "[vnpu-config end]" >> /etc/vnpu.cfg
            chmod 600 /etc/vnpu.cfg
        fi
    fi
    updateInstallInfo
    ./driver/script/run_driver_install.sh "$Install_Path_Param" $installType $force $norebuild
    driver_install_status=$?
    if [ $driver_install_status -eq 0 ];then
        # Init kmsagent service.
        initKmsagentService "true"
        installationCompletionMessage $1
    else
        initKmsagentService "false"
        drvColorEcho "[INFO]Failed to ${1,,} driver package, please retry after uninstall and reboot!"
    fi
}



# Function to check boot status using upgrade-tool
check_boot_status() {
    local docker_type=$1
    local patch_use_status=$2
    # set hot reset flag
    local phyflag=""
    if [ "$force" = n ]; then
        if [ -f "$Driver_Install_Path_Param"/driver/tools/upgrade-tool ]; then
            systemd-detect-virt -v | grep -E "kvm|vmware|qemu|xen" >> /dev/null 2>&1
            phyflag=$?
            timeout 20s "$Driver_Install_Path_Param"/driver/tools/upgrade-tool --device_index -1 --boot_status 2>> /dev/null | grep -v fail | grep "boot status" | grep -v "boot status:0" >> /dev/null 2>&1
            if [ $? -eq 0 ]; then
                if [ "$phyflag"x = "0"x ]; then
                    log "[INFO]VM scene, continue to uninstall the old software package."
                else
                    if [ "$docker_type"x = "docker"x ]; then
                        if checkUserDocker; then
                           log "[INFO]Check docker process is over, continue to uninstall the old software package."
                        else
                           exitInstallInfo
                        fi
                    else
                        if checkUserDocker && setHotResetFlag; then
                            log "[INFO]Check docker process and set hot reset flag is over, continue to uninstall the old software package."
                        else
                            if (( patch_use_status != 0 )); then
                                 error "Install" 1 "ERR_NO:0x0080;ERR_DES: Uninstallation and recovery failed, so reboot is recommended."
                            else
                                 exitInstallInfo
                            fi
                        fi
                    fi
                fi
            else
                log "[WARNING]The chip can not set the hot reset flag."
            fi
        fi
    fi
}

exitInstallInfo() {
    drvColorEcho "[ERROR]\033[31mThe davinci nodes are occupied by some processes, please stop processes and install or uninstall again, details in : $logFile \033[0m"
    log "[ERROR]The davinci nodes are occupied by some processes, please stop processes and install or uninstall again."
    drvColorEcho "[INFO]If you want to install or uninstall the driver forcibly, add the force parameter. For details, see [--help]."
    logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_FAILED}" "${installType}" "${all_parma}"
    exitLog 1
}

uninstall_livepatch() {
    local uninstall_type=$1
    local drv_root_dir="${Driver_Install_Path_Param}"
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

    "${uninstall_sh}" "${uninstall_type}" "y" "${newest_patch_path}" 999 && ret=$? || ret=$?
    if (( ret != 0 )); then
        log "[ERROR]The livepatch's ${uninstall_type}-uninstall failed."
    fi
    livepatch_audit_log_record "${newest_patch_version}" ${ret}

    return 1
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


# check whether any script size is zero or not.
check_local_file_size() {
    local filelist="${Driver_Install_Path_Param}"/driver/script/filelist.csv
    local install_type=""

    [ -e "${filelist}" ] && [ $(stat -c %s "${filelist}") -ne 0 ] || { log "[INFO]filelist.csv doesn't exist or its size is zero." && return 1; }

    # check whether any file size is zero, if yes, it will return 1 directly.
    cd "${Driver_Install_Path_Param}" >& /dev/null
    # the full/debug install-type and the run install-type contain the same scripts.
    [ "${Driver_Install_Type}" = "full" ] || [ "${Driver_Install_Type}" = "debug" ] && install_type="run" || install_type="${Driver_Install_Type}"
    if stat -c %s $(grep "\.sh" "${filelist}" | grep -w "${install_type}" | awk -F [,] '{print $4}') | grep -wq "0"; then
        log "[INFO]there is a local script with zero size."
        cd - >& /dev/null
        return 1
    fi
    cd - >& /dev/null

    log "[INFO]check_local_file_size success."
    return 0
}

modifyPreCode() {
    input_path="`dirname $1`/`basename $1`"

    # virtual machines support hot reset
    search_str="\[WARNING\]not a physical-machine,hot reset not support"
    replace_str="\[INFO\]not a physical-machine"
    # unlocking directories and files
    chattr -iR $input_path > /dev/null 2>&1
    chattr -iR $input_path/* > /dev/null 2>&1
    sed -i "/${search_str}/{n;d}" $input_path/run_driver_uninstall.sh
    sed -i "s/${search_str}/${replace_str}/" $input_path/run_driver_uninstall.sh
}

uninstallationCompletionMessage() {
    hotreset_status="unknown"
    if [ -e "$hotreset_status_file" ]; then
        hotreset_status=$(cat "$hotreset_status_file" | sed 's/\..*//')
        rm -f "$hotreset_status_file"
    fi
    if [ "$hotreset_status"x = "scan_success"x ] || [ "$Driver_Install_Type"x = "docker"x ] || [ "$Driver_Install_Type"x = "devel"x ]; then
        drvColorEcho  "[INFO]\033[32mDriver package uninstalled successfully! Uninstallation takes effect immediately. \033[0m"
    else
        if [ "$hotreset_status"x = "ko_abort"x ]; then
            drvColorEcho "[INFO]\033[32mDriver package uninstalled finished! \033[0m"
            drvColorEcho "[WARNING]\033[33mKernel modules can not be removed, reboot needed for uninstallation to take effect! \033[0m"
        else
            drvColorEcho "[INFO]\033[32mDriver package uninstalled successfully! \033[0m\033[31mReboot needed for uninstallation to take effect! \033[0m"
        fi
    fi
}

uninstallRun() {
    if [ -f "$Driver_Install_Path_Param"/driver/script/${ASCEND_DRIVER_SETUP_SCRIPT} ]; then
        "$Driver_Install_Path_Param"/driver/script/${ASCEND_DRIVER_SETUP_SCRIPT} $KMSAGENT_CONFIG_ITEM stop >> $runLogFile
    fi

    operation="${LOG_OPERATION_UNINSTALL}"
    Driver_Install_Type=$(getInstallParam "Driver_Install_Type" "${installInfo}")
    Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
    [ -f /etc/pss.cfg ] && native_pkcs_conf=$(cat /etc/pss.cfg)

    # not check CRL when in docker or devel mode
    if [ $uninstall = n ] && [ ! $Driver_Install_Type = "docker" ] && [ ! $Driver_Install_Type = "devel" ] && [ $feature_crl_check = y ]; then
        [ ${input_path_flag} = "y" ] && export NEW_Driver_Install_Path="${Install_Path_Param}"
        . ./driver/script/device_crl_check.sh
        #check CRL of images
        device_images_crl_check && ret=$? || ret=$?
        if [ ${ret} -ne 0 ];then
            if [ ${ret} -eq 2 ]; then
                drvColorEcho "[ERROR]\033[31mThe software signature verification failed because the signature mode used by the software is inconsistent with the current configuration. Currently configured is [${native_pkcs_conf}], details in : $logFile \033[0m"
            else
                drvColorEcho "[ERROR]\033[31mDevice_images_crl_check failed, details in : $logFile \033[0m"
            fi
            log "[ERROR]new device crl check failed, stop upgrade"
            rm -f $driverCrlStatusFile
            exitLog 1
        fi

        rm -f /etc/pss.cfg >& /dev/null
        log "[INFO]remove /etc/pss.cfg success"
    fi

    # uninstall livepatch
    g_start_time="$(date +"%Y-%m-%d %H:%M:%S")"

    check_boot_status "docker" 0

    local patch_use_status=0
    uninstall_livepatch "pre"
    patch_use_status=$?

    # set hot reset flag
    check_boot_status "all" ${patch_use_status}

    # remove livepatch directory
    uninstall_livepatch "post"

    # remove version.info
    if [ -f "$Driver_Install_Path_Param"/driver/version.info ];then
       chattr -i "$Driver_Install_Path_Param"/driver/version.info > /dev/null 2>&1
       rm -rf "$Driver_Install_Path_Param"/driver/version.info
       log "[INFO]rm -rf version.info success"
    fi
    if [ ! "$Driver_Install_Type" = "devel" ] && [ ! "$Driver_Install_Type" = "docker" ]; then
        # This is for compatibility with earlier versions.
        if [ -f "$Driver_Install_Path_Param"/host_sys_stop.sh ]; then
            "$Driver_Install_Path_Param"/host_sys_stop.sh
        fi
        if [ -f "$Driver_Install_Path_Param"/host_servers_remove.sh ]; then
            bash "$Driver_Install_Path_Param"/host_servers_remove.sh
        else
            ./host_servers_remove.sh
        fi
    fi

    # if new-install-path is the same as the old one, the original directory will not be deleted.
    [ "${uninstall}" = "n" ] && export NEW_Driver_Install_Path="${Install_Path_Param}"
    uninstall_result=n
    if check_local_file_size; then
        if [ -f "$Driver_Install_Path_Param"/driver/script/run_driver_uninstall.sh ]; then
            modifyPreCode "$Driver_Install_Path_Param"/driver/script
            "$Driver_Install_Path_Param"/driver/script/run_driver_uninstall.sh --uninstall "$Driver_Install_Path_Param" $force && uninstall_result=y
        fi
    fi
    if [ $uninstall_result = n ]; then
        ./driver/script/run_driver_uninstall.sh --uninstall "$Driver_Install_Path_Param" $force
        if [ $? -eq 0 ]; then
            uninstall_result=y
        else
            log "[WARNING]uninstall driver failed"
        fi
    fi

    if [ $uninstall = y ] && [ $uninstall_result = n ]; then
        log "[ERROR]ERR_NO:0x0090;ERR_DES:uninstall driver failed"
        drvEcho "[ERROR]ERR_NO:0x0090;ERR_DES:uninstall driver failed, details in : ${ASCEND_SECLOG}/ascend_install.log"
        logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_FAILED}" "${installType}" "${all_parma}"
        exitLog 1
    fi

    if [ $uninstall_result = y ]; then
        log "[INFO]uninstall driver success"
        if [ $uninstall = y ]; then
            # when the installation path is empty, delete it, make sure the directory exists before using ls
            if [ -d "${Driver_Install_Path_Param}" ] && [ `ls "${Driver_Install_Path_Param}" | wc -l` -eq 0 ];then
               rm -rf "${Driver_Install_Path_Param}"
            fi
            sed -i '/Driver_Install_Path_Param=/d' $installInfo
            sed -i '/Driver_Install_Type=/d' $installInfo
            sed -i '/Driver_Install_Mode=/d' ${installInfo}
            sed -i '/Driver_Install_For_All=/d' ${installInfo}
            if [ ` grep -c -i "Install_Path_Param" $installInfo ` -eq 0 ]; then
                rm -f $installInfo
            fi

            logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_SUCCESS}" "${installType}" "${all_parma}"
            uninstallationCompletionMessage
            exitLog 0
        fi
    fi
    unset "Driver_Install_Path_Param"

}

# Script entry

# if hotreset-status file exist, delete it.
[ -e "${hotreset_status_file}" ] && rm -f "${hotreset_status_file}" >& /dev/null

if [ -f ${installInfo} ]; then
    Driver_Install_Path_Param=$(getInstallParam "Driver_Install_Path_Param" "${installInfo}")
    # first installation scenario
    if [ -z "$Driver_Install_Path_Param" ]; then
        # uninstall scenario
        if [ $uninstall = y ]; then
            error "${LOG_OPERATION_UNINSTALL}" 1 "ERR_NO:0x0080;ERR_DES:Driver package is not installed on this device, uninstall failed"
        # installation scenario
        elif [ $full_install = y ] || [ $debug = y ] || [ $upgrade = y ] || [ $run = y ] || [ $docker = y ] || [ $devel = y ]; then
            first_time_install_drv_flag="Y"
            install_path_should_belong_to_root
            if [ $? -ne 0 ]; then
                exitLog 1
            fi
            installRun "Install"
            operation="${LOG_OPERATION_INSTALL}"
        fi
    else
        first_time_install_drv_flag="N"
        old_drv_install_path="${Driver_Install_Path_Param}"

        version1=$(getVersionInRunFile)
        version2=$(getVersionInstalled "$Driver_Install_Path_Param")
        # uninstall scenario
        if [ $uninstall = y ]; then
            uninstallRun
            # Clean up possible residues directory
            if [ -d "/usr/local/Ascend/driver" ];then
               log "[WARNING]/usr/local/Ascend/driver does not clean up! remove it again."
               rm -rf "/usr/local/Ascend/driver"
            fi
        # overwrite installation scenarios
        elif [ $full_install = y ] || [ $debug = y ] || [ $run = y ] || [ $docker = y ] || [ $devel = y ]; then
            install_path_should_belong_to_root
            if [ $? -ne 0 ]; then
                exitLog 1
            fi
            # run-pkg version compatible check.
            ./driver/script/ver_check.sh $check $quiet || exitLog 1
            if [ ! $version2"x" = "x" ] && [ ! "$version2" = "none" ] && [ $quiet = n ]; then
                # determine whether to overwrite the installation
                drvEcho "[INFO]Driver package has been installed on the path $Driver_Install_Path_Param, the version is ${version2}, and the version of this package is ${version1},do you want to continue?  [y/n] "
                while true
                do
                    read yn
                    if [ "$yn"x = nx ]; then
                        drvEcho "[INFO]installation Stoped!"
                        exitLog 0;
                    elif [ "$yn"x = yx ]; then
                        break;
                    else
                        drvEcho "[ERROR]ERR_NO:0x0002;ERR_DES:input error, please input again!"
                    fi
                done
            fi
            backupKmsData
            uninstallRun
            isValidPath
            installRun "Install"
            operation="${LOG_OPERATION_INSTALL}"
        # upgrade scenario
        elif [ $upgrade = y ]; then
            install_path_should_belong_to_root
            if [ $? -ne 0 ]; then
                exitLog 1
            fi
            backupKmsData
            uninstallRun
            isValidPath
            ./driver/script/ver_check.sh $check $quiet || exitLog 1
            installRun "Upgrade"
            operation="${LOG_OPERATION_UPGRADE}"
        fi
    fi
else
    # uninstall scenario
    if [ $uninstall = y ]; then
        error "${LOG_OPERATION_UNINSTALL}" 1 "ERR_NO:0x0080;ERR_DES:Driver package is not installed on this device, uninstall failed"
    # installation scenario
    elif [ $full_install = y ] || [ $debug = y ] || [ $upgrade = y ] || [ $run = y ] || [ $docker = y ] || [ $devel = y ]; then
        first_time_install_drv_flag="Y"
        install_path_should_belong_to_root || exitLog 1
        ./driver/script/ver_check.sh $check $quiet || exitLog 1
        installRun "Install"
        operation="${LOG_OPERATION_INSTALL}"
    fi
fi