# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

ifeq ($(DAVINCI_HIAI_DKMS),y)
    EXTRA_CFLAGS += -I$(HIAI_DKMS_DIR)/libc_sec/include -I$(HIAI_DKMS_DIR)/libc_sec/src
else
    CUR_MAKEFILE_PATH := $(strip \
                            $(eval LOCAL_MODULE_MAKEFILE := $$(lastword $$(MAKEFILE_LIST))) \
                            $(patsubst %/,%, $(dir $(LOCAL_MODULE_MAKEFILE))) \
                          )
    EXTRA_CFLAGS += -I$(CUR_MAKEFILE_PATH)/include -I$(CUR_MAKEFILE_PATH)/src
endif

EXTRA_CFLAGS += -isystem $(shell $(CC) -print-file-name=include)
EXTRA_CFLAGS += -Iinclude/linux
EXTRA_CFLAGS += -Wno-implicit-fallthrough -Wno-missing-prototypes -Wno-missing-declarations

obj-m := drv_seclib_host.o
drv_seclib_host-objs := src/memcpy_s.o src/memmove_s.o src/memset_s.o  src/securecutil.o  src/strcat_s.o \
    src/strcpy_s.o src/strncat_s.o src/strncpy_s.o src/sprintf_s.o src/vsprintf_s.o src/snprintf_s.o src/vsnprintf_s.o \
    src/secureprintoutput_a.o src/sscanf_s.o src/vsscanf_s.o src/secureinput_a.o src/securecmodule.o src/strtok_s.o \

