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
MAIN_DIR_ALIASES=(
    "user=0_configure_manager"
    "query=1_query_npuinfo"
    "reset=2_chip_reset"
    "upgrade=3_mcu_upgrade"
)

TOPDIR=${PWD}/../../..
BUILD_CMD="gcc main.c -I/usr/local/dcmi/ -I${TOPDIR}/src/custom/include -I${TOPDIR}/pkg_inc -L/usr/local/dcmi/ -L${LD_LIBRARY_PATH} -ldcmi -o main"
AVAILABLE_TARGETS=()

get_real_main_dir() {
    local alias="$1"
    for map in "${MAIN_DIR_ALIASES[@]}"; do
        local key="${map%%=*}"
        local value="${map#*=}"
        if [ "$key" = "$alias" ]; then
            echo "$value"
            return 0
        fi
    done
    echo ""
    return 1
}

get_all_aliases() {
    local aliases=()
    for map in "${MAIN_DIR_ALIASES[@]}"; do
        aliases+=("${map%%=*}")
    done
    echo "${aliases[*]}"
}

show_help() {
    echo "用法: $0 [选项] <主目录别名> <子目录前缀>"
    echo "选项:"
    echo "  --help        显示此帮助信息并退出"
    echo
    echo "参数说明:"
    echo "  <主目录别名>  主目录的别名（对应实际目录如下）:"
    for map in "${MAIN_DIR_ALIASES[@]}"; do
        local alias="${map%%=*}"
        local real_dir="${map#*=}"
        echo "                $alias → 实际目录: ${real_dir}"
    done
    echo "  <子目录前缀>  子目录的开头字符（如 0、1、test 等，匹配以该前缀开头的子目录）"
    echo "                示例:user 目录下有 0_test 和 1_test,可分别用前缀 0 和 1 匹配"
    echo "示例:"
    echo "  bash $0 user 0      # 运行 user (实际: 0_configure_manager)下以 0 开头的子目录（如 0_test)"
    echo "  bash $0 user 1      # 运行 user (实际: 0_configure_manager)下以 1 开头的子目录（如 1_test)"
    echo "  bash $0 query 1     # 运行 query(实际: 1_query_npuinfo)下以 1 开头的子目录"
}

# 检查参数
if [ $# -eq 0 ] || [ "$1" = "--help" ]; then
    show_help
    exit 0
fi

# 验证参数数量
if [ $# -ne 2 ]; then
    echo "错误: 请提供 主目录别名 和 子目录前缀 两个参数！"
    echo "可用的主目录别名：$(get_all_aliases)"
    echo "使用 --help 查看详细用法"
    exit 1
fi

# 解析参数
ALIAS="$1"
SUB_PREFIX="$2"
REAL_MAIN_DIR=$(get_real_main_dir "$ALIAS")
MATCHED_DIRS=()

# 验证主目录别名是否有效
if [ -z "$REAL_MAIN_DIR" ]; then
    echo "错误: 无效的主目录别名 '${ALIAS}'!"
    echo "可用的主目录别名：$(get_all_aliases)"
    exit 1
fi

# 验证实际主目录是否存在
if [ ! -d "${REAL_MAIN_DIR}" ]; then
    echo "错误: 主目录别名 '${ALIAS}' 对应的实际目录 '${REAL_MAIN_DIR}' 不存在！"
    exit 1
fi

for sub_dir in "${REAL_MAIN_DIR}"/${SUB_PREFIX}*/; do
    sub_dir=$(basename "${sub_dir%/}")
    target_dir="${REAL_MAIN_DIR}/${sub_dir}"
    if [ -d "${target_dir}" ] && [ -f "${target_dir}/main.c" ]; then
        MATCHED_DIRS+=("${target_dir}")
    fi
done

# 验证是否找到匹配的目
if [ ${#MATCHED_DIRS[@]} -eq 0 ]; then
    echo "错误: 未找到符合条件的目录！"
    echo "条件：主目录别名=${ALIAS}（实际: ${REAL_MAIN_DIR}），子目录前缀=${SUB_PREFIX}，且包含 main.c"
    echo "提示：当前目录下的子目录有："
    ls -d "${REAL_MAIN_DIR}"/*/ 2>/dev/null | sed 's/^/  /'
    exit 1
fi

for dir in "${MATCHED_DIRS[@]}"; do
    sub_dir=$(basename "$dir")
    echo "  - [${ALIAS}/${sub_dir}] → 实际路径: ${dir}"
done
echo


for target_dir in "${MATCHED_DIRS[@]}"; do
    sub_dir=$(basename "$target_dir")
    echo "======================================"
    echo "正在处理：[${ALIAS}/${sub_dir}]"
    echo "======================================"
    cd "${target_dir}" || {
        echo "警告：无法进入目录 '${target_dir}'，跳过该目录"
        continue
    }

    if [ -f "main" ]; then
        echo "清理之前的可执行文件..."
        rm -f main
    fi

    echo "开始编译..."
    $BUILD_CMD

    if [ $? -eq 0 ] && [ -f "main" ]; then
        echo "编译成功！"
        echo "开始运行程序..."
        echo "======================================"
        ./main
        echo "======================================"
        echo "[${ALIAS}/${sub_dir}] 运行结束"
    else
        echo "警告：[${ALIAS}/${sub_dir}] 编译失败，跳过该目录"
    fi
    cd - > /dev/null
    echo
done
