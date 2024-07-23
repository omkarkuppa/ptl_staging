@REM @file
@REM UsbC capsule build script.
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
@REM @par Specification Reference:
@REM

@set CAP_PLATFORM_PREFIX=UsbC

@REM
@set CAPSULE_CLEAN=FALSE
@set BUILD_ITBT_RETIMER_CAPSULE=FALSE
@set BUILD_DTBT_RETIMER_CAPSULE=FALSE
@set BUILD_DTBT_CONTROLLER_CAPSULE=FALSE

@REM
@set RETIMER_VERSION_DEC=
@set DTBT_VERSION_DEC=
@set VERSION_TYPE=

@if "%1" == "" (
  @echo UsbCCapsuleBuild.bat usage.
  @echo Please execute 'UsbCCapsuleBuild.bat help' for usage information.
  @goto :UsbCCapsuleBuildDone
)

:CmdLineParse
@if "" == "%1" (
  goto Continue
)else if /I "help" == "%1" (
  goto UsbCCapsuleHelp
)else if /I "cln" == "%1" (
  @set CAPSULE_CLEAN=TRUE
)else if /I "itbtretimer" == "%1" (
  set BUILD_ITBT_RETIMER_CAPSULE=TRUE
)else if /I "dtbtretimer" == "%1" (
  set BUILD_DTBT_RETIMER_CAPSULE=TRUE
)else if /I "dtbt" == "%1" (
  set BUILD_DTBT_CONTROLLER_CAPSULE=TRUE
)else if /I "-rtver" == "%1" (
  set RETIMER_VERSION_DEC=%2
  set VERSION_TYPE=-rtver
  SHIFT
)else if /I "-dtbtver" == "%1" (
  set DTBT_VERSION_DEC=%2
  set VERSION_TYPE=-dtbtver
  SHIFT
)else (
  echo.
  echo Invalid input arguments: %1
  echo.
  goto UsbCCapsulePrepFail
)

SHIFT
goto CmdLineParse

:Continue
@REM
@REM Define environment variables for UsbC Capsule build script in BoardPkg.
@REM
@REM Generic path and files
set USBC_CAPSULE_FILE_BUILD_SMAPLE_PATH=%CD%
set USBC_CAPSULE_CONFIG_PATH=%USBC_CAPSULE_FILE_BUILD_SMAPLE_PATH%\ConfigSample

@REM Integrated Thunderbolt Retimer Payload
set ITBT_RETIMER_PAYLOAD_CONFIG_FILE=%USBC_CAPSULE_CONFIG_PATH%\ITbtRetimerPayloadConfig.ini
set ITBT_RETIMER_CAPSULE_PAYLOAD=%USBC_CAPSULE_CONFIG_PATH%\ITbtRetimerPayload.bin

@REM  Discrete Thunderbolt Retimer Payload
set DTBT_RETIMER_PAYLOAD_CONFIG_FILE=%USBC_CAPSULE_CONFIG_PATH%\DTbtRetimerPayloadConfig.ini
set DTBT_RETIMER_CAPSULE_PAYLOAD=%USBC_CAPSULE_CONFIG_PATH%\DTbtRetimerPayload.bin

@REM  Discrete Thunderbolt Controller Payload
set DTBT_PAYLOAD_CONFIG_FILE=%USBC_CAPSULE_CONFIG_PATH%\DTbtPayloadConfig.ini
set DTBT_CAPSULE_PAYLOAD=%USBC_CAPSULE_CONFIG_PATH%\DTbtPayload.bin

@cd ..\..\..\..\..\
@set WORKSPACE=%CD%
@echo %CD%

@REM @REM Open SSL Sign tool path
set OPEN_SSL_SIGN_TOOL_PATH=%WORKSPACE%\Binaries\Tools\OpenSSL

@REM
@REM UsbC Capsule build clean command check.
@REM
@if "%CAPSULE_CLEAN%" == "TRUE"  (
  goto UsbCCapsuleClean
)

@REM ===================================================
@REM =           ITBT Retimer Capsule Build            =
@REM ===================================================
@REM Skip checking ITBT Retimer config image if it's not requested.
@if "%BUILD_ITBT_RETIMER_CAPSULE%" == "FALSE"  (
  goto CheckDTbtRetimerImage
)

@if NOT "%VERSION_TYPE%" == "-rtver" (
  @echo.
  @echo !! Error when ITBT Retimer version is not assigned via -rtver argument
  goto UsbCCapsulePrepFail
)

@REM Generate Retimer FMP Capsule
@cd %WORKSPACE%\Intel\Features\UsbC\UsbCCapsuleFeaturePkg\UsbCCapsuleFileBuildSample\BuildTool
@call ITbtRetimerCapsuleBuild.bat ^
        %WORKSPACE% ^
        %RETIMER_VERSION_DEC%

@if errorlevel 1 (
  @echo.
  @echo !! Error when ITBT Retimer capsule build
  goto UsbCCapsuleNotBuild
)

@REM ===================================================
@REM =           DTBT Retimer Capsule Build            =
@REM ===================================================
:CheckDTbtRetimerImage
@REM Skip checking DTBT Retimer config image if it's not requested.
@if "%BUILD_DTBT_RETIMER_CAPSULE%" == "FALSE"  (
  goto CheckDiscreteThunderboltImage
)

@if NOT "%VERSION_TYPE%" == "-rtver" (
  @echo.
  @echo !! Error when DTBT Retimer version is not assigned via -rtver argument
  goto UsbCCapsulePrepFail
)

@REM Generate Retimer FMP Capsule
@cd %WORKSPACE%\Intel\Features\UsbC\UsbCCapsuleFeaturePkg\UsbCCapsuleFileBuildSample\BuildTool
@call DTbtRetimerCapsuleBuild.bat ^
        %WORKSPACE% ^
        %RETIMER_VERSION_DEC%

@if errorlevel 1 (
  @echo.
  @echo !! Error when DTBT Retimer capsule build
  goto UsbCCapsuleNotBuild
)


@REM ===================================================
@REM =          DTBT Controller Capsule Build          =
@REM ===================================================
:CheckDiscreteThunderboltImage
@if "%BUILD_DTBT_CONTROLLER_CAPSULE%" == "FALSE"  (
  goto UsbCCapsuleBuildDone
)

@if NOT "%VERSION_TYPE%" == "-dtbtver" (
  @echo.
  @echo !! Error when Discrete Thunderbolt version is not assigned via -dtbtver argument
  goto UsbCCapsulePrepFail
)

@REM Generate Retimer FMP Capsule
@cd %WORKSPACE%\Intel\Features\UsbC\UsbCCapsuleFeaturePkg\UsbCCapsuleFileBuildSample\BuildTool
@call DTbtCapsuleBuild.bat ^
        %WORKSPACE% ^
        %DTBT_VERSION_DEC%

@if errorlevel 1 (
  @echo.
  @echo !! Error when DTBT Controller capsule build
  goto UsbCCapsuleNotBuild
)

@REM.
@REM UsbC Capsule build is completed.
@REM.
goto UsbCCapsuleBuildDone

@REM
@REM ============ Error handle ============
@REM
:UsbCCapsulePrepFail
@echo.
@echo !! The Capsule build has failed in prep phase!
@echo.
@goto UsbCCapsuleHelp

:UsbCCapsuleNotBuild
@echo.
@echo !! UsbCCapsuleBuild failed.
@echo.

:UsbCCapsuleHelp
@echo.
@echo Build Capsules based on the binaries in Config
@echo Find more details in ReadMe.txt
@echo.
@echo capsulebuild [target:itbtretimer/dtbtretimer/dtbt]
@echo [-rtver ^<Retimer version^> ][-dtbtver ^<TBT version^>][cln][help]
@echo.
@echo   target   Assign Capsule build target: itbtretimer/dtbtretimer/dtbt
@echo.
@echo            default    : If target is not assigned, that will show usage information.
@echo                         "UsbCCapsuleBuild.bat usage".
@echo                         "Please execute 'UsbCCapsuleBuild.bat help' for usage information".
@echo            cln        : Clean related files and directories generated after executing capsulebuild.
@echo            itbtretimer: Build Integrated TBT Retimer Device FW Capsule
@echo            dtbtretimer: Build Discrete TBT Retimer Device FW Capsule
@echo            dtbt       : Build Discrete TBT Device FW Capsule
@echo.
@echo   -rtver   To assign a virtual Retimer version for Retimer Capsule
@echo            It's mandatory if building capsules with [itbtretimer] or [dtbtretimer] target
@echo            e.g., "UsbCCapsulebuild.bat itbtretimer -rtver 110", "UsbCCapsulebuild.bat dtbtretimer -rtver 119"
@echo   -dtbtver To assign a virtual TBT version for Discrete TBT Capsule
@echo            It's mandatory if building capsules with [distbt] target
@echo            e.g., "UsbCCapsulebuild.bat dtbt -dtbtver 290"
@echo.
@echo   Examples:
@echo      a. UsbCCapsuleBuild.bat cln
@echo           Clean related files and directories generated after executing capsulebuild.
@echo      b. UsbCCapsuleBuild.bat itbtretimer -rtver 110
@echo           Build integrated TBT Retimer Capsule. 110 indicates Retimer build version of integrated TBT Retimer payload
@echo           UsbC_ITbtRetimer_110.cap would be observed in CapsuleOutput directory.
@echo      c. UsbCCapsuleBuild.bat dtbtretimer -rtver 119
@echo           Build discrete TBT Retimer Capsule. 119 indicates Retimer build version of discrete TBT Retimer payload
@echo           UsbC_DTbtRetimer_119.cap would be observed in CapsuleOutput directory.
@echo      d. UsbCCapsuleBuild.bat dtbt -dtbtver 290
@echo           Build discrete TBT Capsule. 290 indicates TBT build version of discrete TBT payload
@echo           UsbC_DTbt_290.cap would be observed in CapsuleOutput directory.
@echo.
@goto UsbCCapsuleBuildDone

:UsbCCapsuleClean
@echo.
@echo Directories to clean...
@echo.
@if exist %WORKSPACE%\Conf rmdir /q /s %WORKSPACE%\Conf
@if exist %USBC_CAPSULE_OUTPUT_PATH%\ rmdir /q /s %USBC_CAPSULE_OUTPUT_PATH%

@echo.
@echo Files to clean...
@echo.
@if exist %ITBT_RETIMER_CAPSULE_PAYLOAD% del %ITBT_RETIMER_CAPSULE_PAYLOAD%
@if exist %DTBT_RETIMER_CAPSULE_PAYLOAD% del %DTBT_RETIMER_CAPSULE_PAYLOAD%
@if exist %DTBT_CAPSULE_PAYLOAD% del %DTBT_CAPSULE_PAYLOAD%

:UsbCCapsuleBuildDone
@if defined USBC_CAPSULE_FILE_BUILD_SMAPLE_PATH (
    @cd %USBC_CAPSULE_FILE_BUILD_SMAPLE_PATH%
)
