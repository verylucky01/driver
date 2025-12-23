/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_OS_ADAPTER_H__
#define __DCMI_OS_ADAPTER_H__

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <setupapi.h>
#include <locale.h>
#include <shlwapi.h>
#include <bcrypt.h>
#include <wtsapi32.h>

#define access _access
#define chmod _chmod
#define open _open
#define write _write
#define close _close
#define unlink _unlink
#define R_OK 4   /* Test for read permission.  */
#define W_OK 2   /* Test for write permission.  */
#define F_OK 0   /* Test for existence.  */
#define ROOT_UID 0
#define FILE_PERMISSION_755 (_S_IREAD | _S_IWRITE)
#define MCU_LOG_PATH "C:\\Program Files\\Huawei\\npu-smi\\log\\mcu_log\\"
#define MCU_UPDATE_TEMP_PATH "C:\\Program Files\\Huawei\\npu-smi\\update\\mcu_update\\"
#define CRL_PATH  "C:\\Program Files\\Huawei\\npu-smi\\hwsipcrl\\ascendsip.crl"
#define CRL_PATH_PSS "C:\\Program Files\\Huawei\\npu-smi\\hwsipcrl\\ascendsip_new.crl"
#define CRL_SAVE_PATH  "C:\\Program Files\\Huawei\\npu-smi\\hwsipcrl\\"
#define pid_t DWORD
#define GETTID GetCurrentThreadId
#define S_IRUSR 0400
#define S_IRGRP (S_IRUSR >> 3)
#define S_IWUSR 0200
#define S_IROTH 0004

#define D_CARD_HARDWAREID_STR L"PCI\\VEN_19E5&DEV_D100"
#define BUFSIZE 1024
#define BDF_STR_CNT 3
#define DBDF_STR_CNT 4
#define WIN_MOD_CN 0x0809
#define SLEEP_ADAPT 1000

#define PATH_MAX 260

struct dcmi_win_pcie_node {
    int pci_root;
    int bus_id;
    int dev_id;
    int fun_id;
};

void sleep(int x);
void usleep(int x);
errno_t *localtime_r(const time_t *timep, struct tm *result);
int chown(const char *path, int owner, int group);

#else
#include <unistd.h>
#include <sys/stat.h>

#define BUFSIZE 1024
#define FILE_PERMISSION_755 (S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)
#define MCU_LOG_PATH "/run/mcu_log/"
#define MCU_UPDATE_TEMP_PATH "/run/mcu_update"
#define CRL_PATH "/etc/hwsipcrl/ascendsip.crl"
#define CRL_PATH_PSS "/etc/hwsipcrl/ascendsip_new.crl"
#define CRL_SAVE_PATH "/etc/hwsipcrl/"
#define CSR_SAVE_PATH "/run/csr/"
#define PATH_MAX 4096

pid_t GETTID(void);
#endif
#endif      /* __DCMI_OS_ADAPTER_H__ */