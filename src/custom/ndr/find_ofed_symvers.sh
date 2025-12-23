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
# find ofed symvers

set -e

if [ $# -ne 2 ]; then
    echo "Usage: $0 <kernel_release> <arch>" >&2
    exit 1
fi

KVER="$1"
ARCH="$2"

OFA_DIR="/usr/src/ofa_kernel"

# 用循环代替数组
for d in \
    "$OFA_DIR/$ARCH/$KVER" \
    "$OFA_DIR/$KVER" \
    "$OFA_DIR/default" \
    "/var/lib/dkms/mlnx-ofed-kernel"
do
    if [ -d "$d" ]; then
        symvers="$d/Module.symvers"
        if [ -f "$symvers" ]; then
            if grep -q "ib_register_peer_memory_client" "$symvers" && \
               grep -q "ib_unregister_peer_memory_client" "$symvers"; then
                echo "$d"
                exit 0
            fi
        fi
    fi
done

exit 1