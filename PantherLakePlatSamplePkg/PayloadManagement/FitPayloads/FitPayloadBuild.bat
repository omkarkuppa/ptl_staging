@REM  @file
@REM  Generate Fit Payload for PantherLake platform.
@REM
@REM  @copyright
@REM  INTEL CONFIDENTIAL
@REM  Copyright (C) 2024 Intel Corporation.
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
@REM  @par Specification
@REM

@set PAYLOAD_PREP_FLAGS=
@set FIT_PAYLOAD_BUILD_FLAGS=
@set FITPAYLOADS_FD_NAME=FITPAYLOADS
@set INTEGRATE_PAYLOAD_BUILD=FALSE
@set FIT_PAYLOAD_RESILIENCY_BUILD=FALSE
@set "FIT_PAYLOAD_BUILD_OPTION_PCD="
@set "SI_BUILD_OPTION_PCD="
@set SCRIPT_ERROR=0
:CmdLineParse

@if "" == "%1" (
  goto PrepareEdk2Env
) else if /i "IntegrateBuild" == "%1" (
  set INTEGRATE_PAYLOAD_BUILD=TRUE
) else if /i "res" == "%1" (
  set FIT_PAYLOAD_RESILIENCY_BUILD=TRUE
  set FIT_PAYLOAD_BUILD_OPTION_PCD=%FIT_PAYLOAD_BUILD_OPTION_PCD% ^
--pcd gCapsuleFeaturePkgTokenSpaceGuid.PcdBiosResiliencyEnable=TRUE
) else (
  echo Invalid input arguments: %1
  echo.
  goto PayloadBuildEnd
)
SHIFT
goto CmdLineParse

:PrepareEdk2Env
if %INTEGRATE_PAYLOAD_BUILD% EQU TRUE goto PayloadBuildBegin

set PAYLOAD_WORKSPACE=%CD%
@set SLOT_SIZE=0x37000
@set FW_VERSION=0x0001
@set LSV=0x0001
@set FW_VERSION_STRING="Version 0.0.0.1"
@set TARGET=RELEASE
@set TARGET_PLATFORM=PantherLake
@set PLATFORM_BIN_PACKAGE=PantherLakeBinPkg
@set PLATFORM_FULL_PACKAGE=PantherLakePlatSamplePkg

call SetEdk2Env.bat
@if %SCRIPT_ERROR% EQU 1 (
  @echo Failed to execute the SetEdk2Env.bat script.
  goto :PayloadBuildEnd
)
set NASM_PREFIX=%SystemDrive%\nasm\
set PYTHON_COMMAND=py -3

cd %WORKSPACE_CORE%
@call edksetup.bat Rebuild
@if %ERRORLEVEL% NEQ 0 (
  @echo !!! ERROR !!! Failed to run edksetup.bat Rebuild. !!!
  set SCRIPT_ERROR=1
  goto :PayloadBuildEnd
)

@REM
@REM Get Visual Studio environment Setting By Edksetup.
@REM Order of precedence is 2019 and then 2017.
@REM
@if not defined TOOL_CHAIN_TAG (
  if defined VS160COMNTOOLS (
    set TOOL_CHAIN_TAG=VS2019
  ) else if defined VS150COMNTOOLS (
    set TOOL_CHAIN_TAG=VS2017
  )
)

REM If no supported version of Visual Studio was detected, return an error.
@if not defined TOOL_CHAIN_TAG (
  echo.
  echo !!! ERROR !!! Preferred Visual Studio not detected!!!
  echo Supported Compilers are: VS2019 or VS2017.
  echo.
  set SCRIPT_ERROR=1
  goto :PayloadBuildEnd
)

@set BUILD_DIR=Build\FitPayloadsPkg\%TARGET%_%TOOL_CHAIN_TAG%
@set BUILD_DIR_PATH=%WORKSPACE%\%BUILD_DIR%
if not exist %BUILD_DIR_PATH% (
  mkdir %BUILD_DIR_PATH%
)

:PayloadBuildBegin
set ACTIVE_PLATFORM=%WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\PayloadManagement\FitPayloads\FitPayloadsPkg.dsc
if %FIT_PAYLOAD_RESILIENCY_BUILD% EQU FALSE (
    set ACTIVE_MICROCODE_FILE=%WORKSPACE_BINARIES%\%PLATFORM_BIN_PACKAGE%\Include\Fdf\FvMicrocode.fdf
) else (
    set ACTIVE_MICROCODE_FILE=%WORKSPACE_BINARIES%\%PLATFORM_BIN_PACKAGE%\Include\Fdf\FvMicrocodeDbgRes.fdf
)

@echo.
@echo  Call this script to pad each Microcode patch under PantherLakeBinPkg\Binaries\Microcode\
@echo  And also create MicrocodeVersion.data
@echo.
%PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\PayloadManagement\FitPayloads\microcode_padding.py ^
  --opt padding ^
  --fw-version %FW_VERSION% ^
  --lsv %LSV% ^
  --fw-version-string %FW_VERSION_STRING% ^
  --slotsize %SLOT_SIZE% ^
  --fdf %ACTIVE_MICROCODE_FILE%
@if %ERRORLEVEL% NEQ 0 (
  @echo !!! ERROR: microcode_padding.py execute failure !!!
  @echo py -3 microcode_padding.py --opt padding --fw-version %FW_VERSION% --lsv %LSV% --fw-version-string %FW_VERSION_STRING% --slotsize %SLOT_SIZE%
  set SCRIPT_ERROR=1
  goto :PayloadBuildEnd
)

set FIT_PAYLOAD_BUILD_FLAGS=-p %ACTIVE_PLATFORM% -b %TARGET% %FIT_PAYLOAD_BUILD_OPTION_PCD% %SI_BUILD_OPTION_PCD%
call build %FIT_PAYLOAD_BUILD_FLAGS%

if ERRORLEVEL 1 (
  @echo.
  @echo !! Error when generating FIT Payload
  @echo.
  goto :PayloadBuildEnd
)

py -3 %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\PayloadManagement\FitPayloads\microcode_padding.py ^
  --opt revert ^
  --fdf %ACTIVE_MICROCODE_FILE%

@if %INTEGRATE_PAYLOAD_BUILD% EQU FALSE (
  @if not exist %WORKSPACE%\RomImages @mkdir %WORKSPACE%\RomImages\
  copy %BUILD_DIR_PATH%\FV\FITPAYLOADS.fd %WORKSPACE%\RomImages\FITPAYLOADS.fd
)

:PayloadBuildEnd
cd %PAYLOAD_WORKSPACE%
