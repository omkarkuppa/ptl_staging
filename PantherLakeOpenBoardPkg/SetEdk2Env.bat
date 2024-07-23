@REM @file
@REM
@REM  @copyright
@REM  INTEL CONFIDENTIAL
@REM  Copyright (C) 2023 Intel Corporation.
@REM
@REM  This software and the related documents are Intel copyrighted materials,
@REM  and your use of them is governed by the express license under which they
@REM  were provided to you ("License"). Unless the License provides otherwise,
@REM  you may not use, modify, copy, publish, distribute, disclose or transmit
@REM  this software or the related documents without Intel's prior written
@REM  permission.
@REM
@REM  This software and the related documents are provided as is, with no
@REM  express or implied warranties, other than those that are expressly stated
@REM  in the License.
@REM
@REM @par Specification
@REM
@echo off

pushd ..\..\

@REM Set WORKSPACE environment.
@REM %__CD__% appends a backslash to every directory, not just the root as %CD% does
@REM This allows directory concatenation when the workspace is a drive root
@set WORKSPACE=%__CD__%
echo.
echo Set WORKSPACE as: %WORKSPACE%
echo.

@REM Create the Conf directory under WORKSPACE
if not exist %WORKSPACE%\Conf (
  mkdir Conf
)

popd

set WORKSPACE_PLATFORM=%WORKSPACE%\Intel
set WORKSPACE_SILICON=%WORKSPACE%\Intel
set WORKSPACE_COMMON=%WORKSPACE%\Intel
set WORKSPACE_PLATFORM_BIN=%WORKSPACE%\Intel
set WORKSPACE_SILICON_BIN=%WORKSPACE%\Intel
set WORKSPACE_FSP_BIN=%WORKSPACE%\Intel
set WORKSPACE_CORE=%WORKSPACE%\Edk2
set WORKSPACE_CORE_PLATFORM=%WORKSPACE%\Edk2Platforms\Platform\Intel
set WORKSPACE_CORE_SILICON=%WORKSPACE%\Edk2Platforms\Silicon\Intel
set EDK_TOOLS_BIN=%WORKSPACE_CORE%\BaseTools\Bin\Win32
set EDK_TOOLS_BINWRAPPERS=%WORKSPACE_CORE%\BaseTools\BinWrappers\WindowsLike
set EDK_TOOLS_PATH=%WORKSPACE_CORE%\BaseTools
set PLATFORM_PACKAGE=MinPlatformPkg
set PLATFORM_BOARD_PACKAGE=PantherLakeOpenBoardPkg
set SCRIPT_ERROR=0

if not defined NASM_PREFIX set NASM_PREFIX=%SystemDrive%\nasm\
%NASM_PREFIX%\nasm.exe -v
if errorlevel 1 (
  @echo ERROR - NASM is not found at %NASM_PREFIX%
  set SCRIPT_ERROR=1
)

@if not defined PYTHON_COMMAND (
  set PYTHON_COMMAND=py -3
  %PYTHON_COMMAND% --version
  if not %ERRORLEVEL% == 0  (
    @echo ERROR - Python is not found at %PYTHON_COMMAND% and py -3 is invalid
    set SCRIPT_ERROR=1
  )
)
@echo on
