
rem Copyright (c) 2025 Huawei Technologies Co., Ltd.
rem This program is free software, you can redistribute it and/or modify it under the terms and conditions of
rem CANN Open Software License Agreement Version 2.0 (the "License").
rem Please refer to the License for details. You may not use this file except in compliance with the License.
rem THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
rem INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
rem See LICENSE in the root of the software repository for the full text of the License.

@echo off

set topdir=%~dp0

if not exist "%topdir%\..\out\matebook\windows\release_imgs" (
    md "%topdir%\..\out\matebook\windows\release_imgs"
)


set path=%path%;D:\VS2017\MSBuild\15.0\Bin\
msbuild dll-mmpa.sln /t:rebuild /p:configuration=debug /p:platform=x64

copy "%topdir%\x64\Debug\dll-mmpa.dll" "%topdir%\..\out\matebook\windows\release_imgs"
copy "%topdir%\x64\Debug\dll-mmpa.lib" "%topdir%\..\out\matebook\windows\release_imgs"


