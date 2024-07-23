@REM  @file
@REM  Generate Payload for PantherLake platform.
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
@set NONFIT_PAYLOAD_BUILD_FLAGS=
@set INTEGRATE_PAYLOAD_BUILD=FALSE
@set SCRIPT_ERROR=0
@set PLATFORM_BIN_PACKAGE=PantherLakeBinPkg
@set "NONFIT_PAYLOAD_BUILD_OPTION_PCD="
@set TARGET=DEBUG

:CmdLineParse

@if "" == "%1" (
  goto PrepareEdk2Env
) else if /i "r" == "%1" (
  set TARGET=RELEASE
) else if /i "IntegrateBuild" == "%1" (
  set INTEGRATE_PAYLOAD_BUILD=TRUE
) else if /i "res" == "%1" (
  set NONFIT_PAYLOAD_BUILD_OPTION_PCD=%NONFIT_PAYLOAD_BUILD_OPTION_PCD% ^
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

@set BUILD_DIR=Build\NonFitPayloadsPkg\%TARGET%_%TOOL_CHAIN_TAG%
@set BUILD_DIR_PATH=%WORKSPACE%\%BUILD_DIR%
if not exist %BUILD_DIR_PATH% (
  mkdir %BUILD_DIR_PATH%
)

:PayloadBuildBegin
set NONFIT_PAYLOAD_BUILD_FLAGS=-p %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\PayloadManagement\NonFitPayloads\NonFitPayloadsPkg.dsc -b %TARGET% %NONFIT_PAYLOAD_BUILD_OPTION_PCD%
call build %NONFIT_PAYLOAD_BUILD_FLAGS%

if ERRORLEVEL 1 (
  @echo.
  @echo !! Error when generating Non - FIT Payload
  @echo.
  goto :PayloadBuildEnd
)

@if %INTEGRATE_PAYLOAD_BUILD% EQU FALSE (
  @if not exist %WORKSPACE%\RomImages @mkdir %WORKSPACE%\RomImages\
  copy %BUILD_DIR_PATH%\FV\NONFITPAYLOADS.fd %WORKSPACE%\RomImages\NONFITPAYLOADS.fd
)

:PayloadBuildEnd
cd %PAYLOAD_WORKSPACE%