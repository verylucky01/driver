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
LOG_NAME="${ASCEND_SECLOG}/ascend_run_servers.log"
INSTALL_LOGFile="${ASCEND_SECLOG}/ascend_install.log"
INSTALL_INFO="/etc/ascend_install.info"
ASCEND_DRIVER_CFG_FILE="/etc/ascend_driver.conf"
ASCEND_DRIVER_SETUP_SCRIPT="ascend_driver_config.sh"
KMSAGENT_CONFIG_ITEM="kmsagent"
KMSAGENT_WORK_DIR="/var/kmsagentd"
KMSAGENT_CONF_FILE="/var/kmsagentd/kmsagent.conf"
KMSAGENT_CONF_MAIN_KSF="/var/kmsagentd/kmsconf.ksf"
KMSAGENT_CONF_BACKUP_KSF="/var/kmsagentd/backup_kmsconf.ksf"
KMSAGENT_SOCKET_FILE="/var/kmsagentd/aiguard.sock"

# services define
MODULE_KMSAGENT_EXEC="kmsagent"
MODULE_KMSAGENT_WDIR="/var/kmsagentd"
MODULE_KMSAGENT_CFGS="kmsagent.conf"
START_WAIT_S=2
STOP_WAIT_S=3

log_record() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    user_id=`id | awk '{printf $1}'`
    echo $1
    if [ -d $ASCEND_SECLOG ]; then
        echo "[Ascend] [$cur_date] [$user_id] $1" >> $LOG_NAME
    fi
}

log_only() {
   cur_date=`date +"%Y-%m-%d %H:%M:%S"`
   user_id=`id | awk '{printf $1}'`
   echo "[Ascend] [$cur_date] [$user_id] $1" >> $LOG_NAME
}

log_hbm() {
   cur_date=`date +"%Y-%m-%d %H:%M:%S"`
   user_id=`id | awk '{printf $1}'`
    echo "[Driver] [$cur_date] [$user_id] "$1 >> $INSTALL_LOGFile
}

# Init environment param
get_install_param() {
    local _key="$1"
    local _file="$2"
    local _param

    install_info_key_array=("Driver_Install_Type" "UserName" "UserGroup" "Driver_Install_Path_Param" "Driver_Install_For_All")
    if [ ! -f "${_file}" ];then
        echo "Get install param failed."
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

init_install_param() {
    # Detect miniRC or device environment
    if [ -e "/proc/cmdline" ];then
        local _rc_d=`cat /proc/cmdline | grep boardid=004`
        if [[ $? -eq 0 && "${_rc_d}X" != "X" ]];then
            ENV_IS_RC=Y
            TARGET_DIR=/usr/local/Ascend
            USERNAME=HwHiAiUser
            USERGROUP=HwHiAiUser
            return 0
        fi
    fi

    # read Driver_Install_Path_Param from INSTALL_INFO
    local UserName=$(get_install_param "UserName" "${INSTALL_INFO}")
    local UserGroup=$(get_install_param "UserGroup" "${INSTALL_INFO}")
    local Driver_Install_Path_Param=$(get_install_param "Driver_Install_Path_Param" "${INSTALL_INFO}")

    ENV_IS_RC=N
    TARGET_DIR="${Driver_Install_Path_Param}"
    TARGET_DIR="${TARGET_DIR%*/}"
    USERNAME=$UserName
    USERGROUP=$UserGroup
    if [ "${USERNAME}X" = "X" ]; then
        USERNAME=HwHiAiUser
        USERGROUP=HwHiAiUser
    fi

    KMSAGENT_EXEC="${TARGET_DIR}/driver/tools/${KMSAGENT_CONFIG_ITEM} $KMSAGENT_CONF_FILE $KMSAGENT_CONF_MAIN_KSF"
    KMSAGENT_EXEC_PATH="${TARGET_DIR}/driver/tools/${KMSAGENT_CONFIG_ITEM}"
    KMS_INIT_EXEC="${TARGET_DIR}/driver/script/${ASCEND_DRIVER_SETUP_SCRIPT} $KMSAGENT_CONFIG_ITEM start"
    KMS_INIT_EXEC_PATH="${TARGET_DIR}/driver/script/${ASCEND_DRIVER_SETUP_SCRIPT}"
}

get_kmsagent_config() {
    local kms_config_txt=`grep -r "${KMSAGENT_CONFIG_ITEM}=" "${ASCEND_DRIVER_CFG_FILE}" | cut -d"=" -f2-`
    if [ "${kms_config_txt}X" = "enableX" ];then
        return 0
    fi
    return 1
}

update_kmsagent_config() {
    local kms_enable="disable"
    get_kmsagent_config
    if [ $? -eq 0 ];then
        kms_enable="enable"
    fi

    if [ $1 = $kms_enable ];then
        echo "[INFO]The ${process_name} service has been ${1}d."
        return 0
    fi
    sed -i "/${KMSAGENT_CONFIG_ITEM}=/d" $ASCEND_DRIVER_CFG_FILE
    echo "${KMSAGENT_CONFIG_ITEM}=$1" >> $ASCEND_DRIVER_CFG_FILE

    if [ $? -eq 0 ];then
        log_record "[INFO]Ascend service ${PARAM_MODULE} $1 success."
        return 0
    else
        echo "[ERROR]Ascend service ${PARAM_MODULE} $1 failed."
        return 1
    fi
}

servers_enable() {
    init_install_param
    servers_start $MODULE_KMSAGENT_EXEC $MODULE_KMSAGENT_WDIR
    if [ $? -ne 0 ]; then
        update_kmsagent_config "disable"
        echo "[ERROR]Ascend service ${MODULE_KMSAGENT_EXEC} can't start, enable failed."
        return 1
    fi
    update_kmsagent_config "enable"
    return $?
}

servers_disable() {
    init_install_param
    servers_stop $MODULE_KMSAGENT_EXEC $MODULE_KMSAGENT_WDIR
    if [ $? -ne 0 ]; then
        echo "[ERROR]Ascend service ${MODULE_KMSAGENT_EXEC} can't stop, disable failed."
        return 1
    fi
    update_kmsagent_config "disable"
    return $?
}

servers_runstatus_check() {
    local process_name=$1
    local process_exec_path=$2
    if [ "${process_name}X" = "X" ];then
        echo "[ERROR]Process name can't be null."
        return 1
    fi

    process_info=`ps -ef|grep "${process_name}" | grep -v grep | awk '{print $2}'`
    process_list=(${process_info})
    for pid_nm in "${process_list[@]}"; do
        exec_path=`readlink /proc/${pid_nm}/exe`
        if [ "$exec_path" = "$process_exec_path" ]; then
            pid=$pid_nm
            return 0
        elif [[ $exec_path =~ ^"${process_exec_path} ".* ]];then
            pid=$pid_nm
            return 0
        fi
    done

    return 1
}

conf_kms_array=(
    'KMSAGENT_KEYSTORE KmcMainPath _agentka.ksf'
    'KMSAGENT_KEYSTORE KmcBackupPath _agentkb.ksf'
)

initKmsagentConfigFile(){
    local new_path="${TARGET_DIR}"
    local tool_path="$new_path/driver/tools/kmsagent"
    local tool_conf="${KMSAGENT_CONF_FILE}"
    local tool_depends_lib="$new_path/driver/lib64/inner:$new_path/driver/lib64:$new_path/driver/lib64/driver:$new_path/driver/lib64/common"

    log_record "[INFO]Start init ksf config file."
    touch $KMSAGENT_CONF_MAIN_KSF
    if [ -L $KMSAGENT_CONF_MAIN_KSF ]; then
        log_record "[WARNING]Main ksf cannot be a soft link."
        return 1
    fi
    chown -h $USERNAME:$USERGROUP $KMSAGENT_CONF_MAIN_KSF
    su - ${USERNAME} -s /bin/bash -c "chmod 600 $KMSAGENT_CONF_MAIN_KSF" >> $LOG_NAME
    if [ $? -ne 0 ]; then
        log_record "[WARNING]Unable to configure main ksf."
        return 1
    fi
    log_only "[INFO]switch to the owner to chmod ${KMSAGENT_CONF_MAIN_KSF##*/}."
    if [ -f $KMSAGENT_CONF_BACKUP_KSF ]; then
        if [ -L $KMSAGENT_CONF_BACKUP_KSF ]; then
            log_record "[WARNING]Backup ksf cannot be a soft link."
            return 1
        fi
        chown -h $USERNAME:$USERGROUP $KMSAGENT_CONF_BACKUP_KSF
        su - ${USERNAME} -s /bin/bash -c "chmod 600 $KMSAGENT_CONF_BACKUP_KSF" >> $LOG_NAME
        if [ $? -ne 0 ]; then
            log_record "[WARNING]Unable to configure backup ksf."
            return 1
        fi
        log_only "[INFO]switch to the owner to chmod ${KMSAGENT_CONF_BACKUP_KSF##*/}."
    fi

    for kms_param in "${conf_kms_array[@]}"; do
        sub_param=($kms_param)
        su - ${USERNAME} -s /bin/bash -c "export LD_LIBRARY_PATH=${tool_depends_lib}; $tool_path -c $tool_conf -k $KMSAGENT_CONF_MAIN_KSF -s ${sub_param[0]} -n ${sub_param[1]} -v $KMSAGENT_WORK_DIR/data/${USERNAME}${sub_param[2]}" >> $LOG_NAME
        log_only "[INFO]switch to the owner to execute ${tool_path##*/} to modify ${sub_param[0]}/${sub_param[1]} config."
        grep -q "${USERNAME}${sub_param[2]}" $tool_conf
        if [ $? -ne 0 ]; then
            log_record "[WARNING]Unable to configure ${sub_param[0]}/${sub_param[1]} in the kmsagent file."
            return 1
        fi
    done
    RET=0

    return $RET
}

servers_start() {
    # 1.Check porcess is runing
    local process_name=$1
    servers_runstatus_check "$KMSAGENT_EXEC" "$KMSAGENT_EXEC_PATH"
    if [ $? -eq 0 ];then
        echo "[WARNING]The ${process_name} service has been started and cannot be started again."
        return 0
    fi

    # 2.Creat process work dir
    local process_wdir=$2
    if [ "${process_wdir}X" != "X" ];then
        mkdir -p $process_wdir
        chown -h ${USERNAME}:${USERGROUP} $process_wdir
        if [ $? -ne 0 ];then
            log_record "[ERROR]Set ${process_wdir} dir failed."
            return 1
        fi
    fi
    initKmsagentConfigFile
    if [ $? -ne 0 ]; then
        log_record "[WARNING]Unable to configure kmsagent init file and can't use the model protection function."
    fi

    # 3.Start process
    local service_name="ascend-${process_name}"
    systemctl start $service_name
    local user_pid=`systemctl show --property MainPID ${service_name} | cut -d"=" -f2-`
    if [ $user_pid -eq 0 ];then
        local user_ret=`systemctl show --property ExecMainStatus ${service_name}`
        systemctl stop $service_name

        log_record "[ERROR]Start ${process_name} failed, ${user_ret}"
        return 1
    fi

    # 4.Check start result
    local int=0
    while true
    do
        # Wait 2s to ensure that the process is initialized and started successfully
        servers_runstatus_check "$KMSAGENT_EXEC" "$KMSAGENT_EXEC_PATH"
        on_run=$?
        on_pid=`systemctl show --property MainPID ${service_name} | cut -d"=" -f2-`
        if [[ $on_run -ne 0 && $on_pid -eq 0 ]];then
            # The process does not exist
            local user_ret=`systemctl show --property ExecMainStatus ${service_name}`
            systemctl stop $service_name

            log_record "[ERROR]Start ${process_name} failed, ${user_ret}"
            return 1
        fi

        if [ $int -ge $START_WAIT_S ]; then
            # start process success
            break
        fi
        let "int++"
        sleep 1
    done

    log_record "[INFO]Ascend service ${PARAM_MODULE} start success."
    return 0
}

servers_stop() {
    local process_name=$1
    local process_wdir=$2
    if [ "${process_name}X" = "X" ];then
        echo "[INFO]Process name can't be null."
        return 1
    fi

    # Stop service start sh
    servers_runstatus_check "$KMS_INIT_EXEC" "$KMS_INIT_EXEC_PATH"
    if [ $? -eq 0 ]; then
        kill -15 $pid
    fi

    local service_name="ascend-${process_name}"
    systemctl stop $service_name > /dev/null 2>&1

    servers_runstatus_check "$KMSAGENT_EXEC" "$KMSAGENT_EXEC_PATH"
    if [ $? -eq 0 ]; then
        # Send signal 15 to kill process
        kill -15 $pid

        # Check process status
        local int=1
        while true
        do
            servers_runstatus_check "$KMSAGENT_EXEC" "$KMSAGENT_EXEC_PATH"
            if [ $? -ne 0 ]; then
                # stop process success
                break
            fi
            if [ $int -gt $STOP_WAIT_S ];then
                log_record "[ERROR]Stop ${process_name} failed."
                return 1
            fi
            let "int++"
            sleep 1
        done
    else
        echo "[INFO]The ${process_name} service has been stopped."
        return 0
    fi

    # Clear process work dir
    if [ "${process_wdir}X" != "X" ];then
        if [ -d "${process_wdir}" ] && [ `ls "${process_wdir}" | wc -l` -eq 0 ];then
            rm -rf ${process_wdir}
            log_record "[INFO]Remove process work dir '${process_wdir}' success."
        fi
    fi
    log_record "[INFO]Ascend service ${PARAM_MODULE} stop success."
    return 0
}

get_ppid() { ps -o ppid= $1 | tr -d ' '; }

get_process_cmdline() { cat /proc/$1/cmdline | tr '\0' ' ' && echo; }

get_shell_script_name() {
    local exe=$(readlink -f /proc/$1/exe)
    if [[ ${exe} =~ sh$ ]]; then
        cat /proc/$1/cmdline | tr '\0' '\n' | sed -n 2p
    fi
}

kill_installation_process() {
    local name gpid
    gpid=$(get_ppid ${PPID})
    name=$(get_shell_script_name ${gpid})
    sync
    [[ ${name} =~ \.run$ ]] && {
        if [[ ${PWD} =~ /selfgz${gpid}[0-9]+/?$ ]]; then
            # Remove the tmpdir created by the current run package
            rm -rf ${PWD}
        fi
        # Kill driver.run
        kill -9 ${gpid} &>/dev/null
    }
    # Kill install.sh
    kill -9 ${PPID} &>/dev/null
}

is_executing_override_installation() {
    local cmdline=$(get_process_cmdline ${PPID})
    [[ ${cmdline} =~ /install\.sh ]] || return 1  # Not called by install.sh
    [[ ${cmdline} =~ --uninstall ]] && return 1
    [[ ${cmdline} =~ --(full|run|upgrade) ]] && return 0
    return 1
}

# Execute the generic upgrade check script
do_upgrade_check() {
    local err=0

    is_executing_override_installation || return 0

    [[ -x "${0%/*}"/run_driver_upgrade_check.sh ]] && {
        # Prevent upgrade check messages from being redirected
        exec 3>&1 1>>/proc/${PPID}/fd/1
        "${0%/*}"/run_driver_upgrade_check.sh -c run
        err=$?
        exec 1>&3 3>&-
    }

    (( err == 0 )) || {
        # Upgrade check failed, stop installation
        kill_installation_process
        exit 1
    }
}

########################################## Main function start. ########################################

# 1. Permission check
if [ $(id -u) -ne 0 ]; then
    echo "[ERROR]Do not have root permission, operation failed, please use root permission!"
    exit 1
fi
if [ ! -e $INSTALL_INFO ] && [ ! -e $ASCEND_DRIVER_CFG_FILE ]; then
    echo "[ERROR]Can't find ascend driver config file, may run in docker environment(not support run in docker)."
    exit 1
fi

# 2. Paramters check
if [[ "-$#" = "-0" || "-$1" = "-help" ]]; then
    echo -e "Usage:"
    echo -e "    ${ASCEND_DRIVER_SETUP_SCRIPT} <MODULE> <PARAM> [OPT...]\n"
    echo -e "Options:"
    echo -e "    kmsagent enable        : Enables kmsagent service auto start."
    echo -e "    kmsagent disable       : Disable kmsagent service auto start."
    echo -e "    kmsagent start         : Manually start the kmsagent service."
    echo -e "    kmsagent stop          : Manually Stop the kmsagent service."
    echo -e "    kmsagent cfgstatus     : Show kmsagent service config status."
    echo -e "    kmsagent runstatus     : Show kmsagent service running status."
    echo -e "    help                   : Show help info."
    echo -e "Examples:"
    echo -e "    ./${ASCEND_DRIVER_SETUP_SCRIPT} kmsagent enable"
    exit 0
fi

# 3. Start main process.
main_process() {

    # kmsagent service config.
    if [ "$PARAM_MODULE"X = "${KMSAGENT_CONFIG_ITEM}X" ];then
        case "$PARAM_OPTIONS_0" in
            enable)
            servers_enable
            ;;

            disable)
            servers_disable
            ;;

            start)
            init_install_param
            servers_start $MODULE_KMSAGENT_EXEC $MODULE_KMSAGENT_WDIR $MODULE_KMSAGENT_CFGS
            ;;

            stop)
            do_upgrade_check

            init_install_param
            servers_stop $MODULE_KMSAGENT_EXEC $MODULE_KMSAGENT_WDIR
            ;;

            cfgstatus)
            get_kmsagent_config
            if [ $? -eq 0 ];then
                echo "[INFO]Ascend service ${PARAM_MODULE} automatic startup is on."
                exit 0
            else
                echo "[INFO]Ascend service ${PARAM_MODULE} automatic startup is off."
                exit 1
            fi
            ;;

            runstatus)
            init_install_param
            servers_runstatus_check "$KMSAGENT_EXEC" "$KMSAGENT_EXEC_PATH"
            if [ $? -eq 0 ];then
                echo "[INFO]Ascend service ${PARAM_MODULE} is running."
                exit 0
            else
                echo "[INFO]Ascend service ${PARAM_MODULE} is not running."
                exit 1
            fi
            ;;

            *)
            echo "[ERROR]The parameter doesn't support."
            exit 1
            ;;
        esac

        exit $?
    fi

    echo "[ERROR]The parameter doesn't support."
    exit 1
}

PARAM_MODULE=$1
PARAM_OPTIONS_0=$2
main_process
