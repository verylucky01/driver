/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "dlog_pub.h"

#ifdef LOG_CPP
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup slog
 * @brief External log interface, which called by modules
 */
void dlog_init(void)
{

}

/**
 * @ingroup slog
 * @brief DlogGetlevelForC: get module debug loglevel and enableEvent
 *
 * @param [in]moduleId: moudule id(see slog.h, eg: CCE), others: invalid
 * @param [out]enableEvent: 1: enable; 0: disable
 * @return: module level(0: debug, 1: info, 2: warning, 3: error, 4: null output)
 */
int DlogGetlevelForC(int moduleId, int *enableEvent)
{
        return 0;
}

/**
 * @ingroup slog
 * @brief DlogSetlevelForC: set module loglevel and enableEvent
 *
 * @param [in]moduleId: moudule id(see slog.h, eg: CCE), -1: all modules, others: invalid
 * @param [in]level: log level(0: debug, 1: info, 2: warning, 3: error, 4: null output)
 * @param [in]enableEvent: 1: enable; 0: disable, others:invalid
 * @return: 0: SUCCEED, others: FAILED
 */
int32_t DlogSetlevelForC(int32_t moduleId, int32_t level, int32_t enableEvent)
{
        return 0;
}

/**
 * @ingroup slog
 * @brief CheckLogLevelForC: check module level enable or not
 * users no need to call it because all dlog interface(include inner interface) has already called
 *
 * @param [in]moduleId: module id, eg: CCE
 * @param [in]logLevel: eg: DLOG_EVENT/DLOG_ERROR/DLOG_WARN/DLOG_INFO/DLOG_DEBUG
 * @return: 1:enable, 0:disable
 */
int32_t CheckLogLevelForC(int32_t moduleId, int32_t logLevel)
{
        return 0;
}

/**
 * @ingroup slog
 * @brief DlogSetAttrForC: set log attr, default pid is 0, default device id is 0, default process type is APPLICATION
 * @param [in]logAttrInfo: attr info, include pid(must be larger than 0), process type and device id(chip ID)
 * @return: 0: SUCCEED, others: FAILED
 */
int32_t DlogSetAttrForC(LogAttr logAttrInfo)
{
        return 0;
}

/**
 * @ingroup slog
 * @brief DlogFlushForC: flush log buffer to file
 */
void DlogFlushForC(void)
{

}

// log interface
void DlogRecordForC(int32_t moduleId, int32_t level, const char *fmt, ...)
{

}


/**
 * @brief           get module debug loglevel and enableEvent
 * @param [in]      moduleId       moudule id(see log_types.h, eg: CCE), others: invalid
 * @param [out]     enableEvent    1: enable; 0: disable
 * @return          module level   0: debug, 1: info, 2: warning, 3: error, 4: null output
 */
int32_t dlog_getlevel(int32_t moduleId, int32_t *enableEvent)
{
        return 0;
}

/**
 * @brief           set module loglevel and enableEvent
 * @param [in]      moduleId       moudule id(see log_types.h, eg: CCE), -1: all modules, others: invalid
 * @param [in]      level          log level, eg: DLOG_ERROR/DLOG_WARN/DLOG_INFO/DLOG_DEBUG
 * @param [in]      enableEvent    1: enable; 0: disable, others:invalid
 * @return          0: SUCCEED, others: FAILED
 */
int32_t dlog_setlevel(int32_t moduleId, int32_t level, int32_t enableEvent)
{
        return 0;
}

/**
 * @brief           check module level enable or not
 * @param [in]      moduleId       module id, eg: CCE
 * @param [in]      logLevel       log level, eg: DLOG_ERROR/DLOG_WARN/DLOG_INFO/DLOG_DEBUG
 * @return          1:enable, 0:disable
 */
int32_t CheckLogLevel(int32_t moduleId, int32_t logLevel)
{
        return 0;
}

/**
 * @brief           set log attr, default pid is 0, default device id is 0, default process type is APPLICATION
 * @param [in]      logAttrInfo    attr info, include pid(must be larger than 0), process type and device id(chip ID)
 * @return          0: SUCCEED, others: FAILED
 */
int32_t DlogSetAttr(LogAttr logAttrInfo)
{
        return 0;
}

/**
 * @brief           print log, need va_list variable, exec CheckLogLevel() before call this function
 * @param[in]       moduleId      module id, eg: CCE
 * @param[in]       level         log level, eg: DLOG_ERROR/DLOG_WARN/DLOG_INFO/DLOG_DEBUG
 * @param[in]       fmt           log content
 * @param[in]       list          variable list of log content
 * @return          NA
 */
void DlogVaList(int32_t moduleId, int32_t level, const char *fmt, va_list list)
{

}

/**
 * @brief           flush log buffer to file
 * @return          NA
 */
void DlogFlush(void)
{

}

/**
 * @brief           record log
 * @param [in]      moduleId   module id, eg: SLOG
 * @param [in]      level      log level, eg: DLOG_ERROR/DLOG_WARN/DLOG_INFO/DLOG_DEBUG
 * @param [in]      fmt        log content
 * @return:         NA
 */
void DlogRecord(int32_t moduleId, int32_t level, const char *fmt, ...)
{

}

#ifdef __cplusplus
}
#endif
#endif // LOG_CPP
