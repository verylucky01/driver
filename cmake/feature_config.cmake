# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

set(INPUT_CONFIG cmake/config/feature_config/${PRODUCT}.config)
set(OUTPUT_CONFIG_PATH ${CMAKE_BINARY_DIR}/config/feature_config)

file(MAKE_DIRECTORY "${OUTPUT_CONFIG_PATH}")
execute_process(
    COMMAND bash -c "grep -v \"^#\" ${INPUT_CONFIG} | sed 's/^CONFIG/#define CONFIG/g' > ${OUTPUT_CONFIG_PATH}/feature.h"
    COMMAND bash -c "grep -v \"^#\" ${INPUT_CONFIG} | sed -r 's/^(.*)=(.*)/CONFIG_DEFINES += -D\\1=\\2\\n\\1 := \\2/;1iCONFIG_DEFINES :=' > ${OUTPUT_CONFIG_PATH}/feature.mk"
    COMMAND bash -c "grep -v \"^#\" ${INPUT_CONFIG} | sed -r 's/^(.*)=(.*)/list(APPEND CONFIG_DEFINES \\1=\\2)\\nset(\\1 \\2)/;1iset(CONFIG_DEFINES)' > ${OUTPUT_CONFIG_PATH}/feature.cmake"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error
)

if(NOT result EQUAL 0)
    message(FATAL_ERROR "Faile to create feature config:\r ${output} ${error}")
endif()

set(FEATURE_CMAKE_PATH ${OUTPUT_CONFIG_PATH}/feature.cmake)
set(FEATURE_MK_PATH ${OUTPUT_CONFIG_PATH}/feature.mk)
