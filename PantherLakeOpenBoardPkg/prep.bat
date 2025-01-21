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
@echo ON
@set PREPTIMESTAMP=%time%
@REM
@REM The following 4 envir vairable is used for Micorocode update.
@REM The Microcode region layout as the following format:
@REM We will use FW_VERSION, LSV and FW_VERSION_STRING to generate Version.ffs
@REM Use SLOT_SIZE to add padding data for each Microcode patch.
@rem Caution: DO NOT set SLOT_SIZE to 0, otherwise it will build fail.
@REM
@REM --------------------
@REM |   Fv Header      |
@REM --------------------
@REM |   Version.ffs    |
@REM --------------------
@REM |MicrocodeArray.ffs|
@REM --------------------
@REM
@set SLOT_SIZE=0x37000
@set FW_VERSION=0x0001
@set LSV=0x0001
@set FW_VERSION_STRING="Version 0.0.0.1"

@REM Run SetEdk2Env.bat
call SetEdk2Env.bat
@If %SCRIPT_ERROR% EQU 1 goto PrepFail

@set TIP_DIR=%CD%
cd ..\..\

@echo ********************************************************************
@echo ***********           Prep.bat is launched here          ***********
@echo ********************************************************************

@set SILENT_MODE=FALSE
@set CapsuleBuild=FALSE
@set PrepRELEASE=DEBUG

@set EXT_CONFIG_CLEAR=
@set EXT_BUILD_FLAGS=

@REM
@REM Set 64-bit PEI for FSP as default build.
@REM
@set FSP_ARCH=X64
@set FSP64_BUILD=TRUE
@set FSP_RESET=TRUE
@set FSP_MODE=Dispatch
@set SYMBOL_PREFIX=
@set BUILD_OPTION_PCD=--pcd gBoardModuleTokenSpaceGuid.PcdFspWrapperResetVectorInFsp=TRUE

@set EXT_BUILD_FLAGS=%EXT_BUILD_FLAGS% -D FSP_ARCH=%FSP_ARCH%
@set EXT_BUILD_FLAGS=%EXT_BUILD_FLAGS% -D FSP64_BUILD=%FSP64_BUILD%

:CmdLineParse
if "" == "%1" (
  goto Continue
) else if "r" == "%1" (
  set PrepRELEASE=RELEASE
) else if "tr" == "%1" (
  set PrepRELEASE=TEST_RELEASE
) else if "rp" == "%1" (
  set PrepRELEASE=RELEASE
) else if "s" == "%1" (
  set SILENT_MODE=TRUE
) else if "help" == "%1" (
  goto PrepHelp
) else (
  echo Invalid input arguments: %1
  echo.
  goto PrepHelp
)
SHIFT
goto CmdLineParse

:PrepHelp
@echo Preparation for BIOS build.
@echo.
@echo prep [r][rp][s][help]
@echo.
@echo   r         To do release build. Default is debug build. See note 1
@echo   rp        To do release build with Symbols - For source level debugging. See note 1
@echo   s         To build in silent mode. . See note 1
@echo.
@echo 1) Re-running prep without these arguments cannot be used for
@echo    incremental build. Hence, these inputs must be supplied each time
@echo    prep are desired to be re-run.
@echo.
goto PrepDone

:Continue
@echo ==============================================

if exist %WORKSPACE%\Prep.log del %WORKSPACE%\Prep.log

:PrepReleaseCheck

@echo ********************************************************************
@echo ***********        Prebuild.bat is launched here       *************
@echo ********************************************************************
cd %TIP_DIR%
@echo Current Directory   =  %CD%
@echo WORKSPACE           =  %WORKSPACE%
@echo WORKSPACE_CORE      =  %WORKSPACE_CORE%
@echo WORKSPACE_COMMON    =  %WORKSPACE_COMMON%
@echo WORKSPACE_PLATFORM  =  %WORKSPACE_PLATFORM%
@echo WORKSPACE_SILICON   =  %WORKSPACE_SILICON%
@echo PrepRelease         =  %PrepRelease%
@echo CapsuleBuild        =  %CapsuleBuild%

@if %SILENT_MODE% EQU TRUE goto BldSilent

call prebuild.bat %PrepRelease% %CapsuleBuild%
goto PrePrepDone

:BldSilent
cd %TIP_DIR%
call prebuild.bat %PrepRelease% %CapsuleBuild% 1>>%WORKSPACE%\Prep.log 2>&1

:PrePrepDone
@If %SCRIPT_ERROR% EQU 1 goto PrepFail
@goto PrepDone

:PrepFail
@echo.
@echo !! The EDKII BIOS build has failed in prep!
@echo.
@exit /b 1

:PrepDone
@echo.
@echo Bios Prep succeeded
@echo.
@set PREPTIMESTAMP=%PREPTIMESTAMP% -%time%
