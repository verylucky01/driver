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

filename="/etc/npu_dev_syslog.cfg"

if [ ! -f $filename ]; then
    exit 1
fi
npu_id=$1
# 区分是否是npu-smi命令下发的参数，如果是device_boot_init.sh传入的id需要通过npu-smi info -t phyid-remap -p转换
is_npu_smi_id=$2

uid=$(ls -n "$filename" | awk '{print $3}')

# 检查Uid是否为0，即root用户, 非root用户退出
if [ "$uid" -ne 0 ]; then
    exit 1
fi

# 判断命令为npusmi开头的命令或者为Y（确认下发）
function check_cmd_legal()
{
    string=$1
    # 删除换行符
    new_string=$(echo "$string" | tr -d '\n')
    # 命令的确认行
    if [[ $new_string == "y" ]]; then
        return 0
    fi
    # 命令中不能有分号
    if echo "$new_string" | grep -q ';'; then
        return 1
    fi
        # npu-smi命令
    if [[ $new_string =~ ^(/usr/local/bin/npu-smi) ]]; then
        return 0
    fi
    return 1
}

mode=""

while IFS= read -r line; do
  if [[ $line == 'syslog_persistence_config_mode:'* ]]; then
    mode=${line#*:}
    break
  fi
done < "$filename"
# 判断并输出内容

wait_server_ok() {
    local count=0
    local phy_npuid="$npu_id"
    local max_attempts=300
    if [[ -z ${npu_id} ]]; then
        return
    fi
    while [ $count -lt $max_attempts ]; do
        if [[ -z ${is_npu_smi_id} ]]; then
            phy_npuid=$(/usr/local/bin/npu-smi info -t phyid-remap -p ${npu_id} | grep "NPU ID" |  awk -F ':' '{print $2}')
        fi
        output=$(/usr/local/bin/npu-smi info -t board -i $phy_npuid -c 0)
        if [[ $output == *"NPU ID"* ]]; then
            break
        fi
        count=$((count+1))
        sleep 5
    done
}

start_server() {
  if [[ $mode == *enable* ]]; then
    wait_server_ok
    sleep 20
    while IFS= read -r line; do
      # 判断命令合法后执行
      check_cmd_legal "$line"
      res=$?
        if [ $res -eq 0 ]; then
          eval "$line > /dev/null 2>&1"
      fi
    done < "$filename"
  fi
}

start_server &