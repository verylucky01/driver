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
set -e

if [ "$1" = "hdcd" ];then
    pid=$2
    if [ -w "/sys/fs/cgroup/memory/usermemory/cgroup.procs" ]; then
        echo $pid > /sys/fs/cgroup/memory/usermemory/cgroup.procs
    fi
elif [ "$1" = "hccp_service.bin" ];then
    pids=$(pidof $1)
    len=$(echo $pids | awk '{print NF}')
    if [ -w "/sys/fs/cgroup/memory/usermemory/cgroup.procs" ]; then
        while [ $len -gt 0 ]
        do
            pid=$(echo $pids | awk '{print $("'$len'")}')
            echo $pid > /sys/fs/cgroup/memory/usermemory/cgroup.procs
            len=$(($len-1));
        done
    fi
fi
