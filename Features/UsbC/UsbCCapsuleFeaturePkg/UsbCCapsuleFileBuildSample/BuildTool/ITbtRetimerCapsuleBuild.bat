@REM @file
@REM Integrated Thunderbolt Retimer Capsule build script.
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

@REM Usage:
@REM ITbtRetimerCapsuleBuild.bat [Work Space Path] [ITBT Retimer version]
@REM   Work Space Path       : The work space should be have edk2 module.
@REM   ITBT Retimer version  : Set Integrated Thunderbolt Retimer version.
@REM

@REM
@REM Define environment variables.
@REM

@REM Set WORKSPACE environment.
@set WORKSPACE=%1
@if "%1" == "" (
  @echo ERROR!! Work space directory did not assigned via argument.
  goto :WrongParametersStop
)
@if not exist %WORKSPACE%\Edk2 (
  @echo ERROR!! The Edk2 module does not exist at work space : %WORKSPACE%
  goto :WrongParametersStop
)

@set RETIMER_VERSION_DEC=%2
@if "%2" == "" (
  @echo ERROR!! Retimer version did not assigned via argument.
  goto :WrongParametersStop
)

@if not defined PYTHON_COMMAND (
  set PYTHON_COMMAND=py -3
)

@set WORKSPACE_CORE=%WORKSPACE%\Edk2
@set WORKSPACE_CORE_PLATFORM=%WORKSPACE%\Edk2Platforms\Platform\Intel

@set PACKAGES_PATH=
@for /f %%i in (
  '%PYTHON_COMMAND% %WORKSPACE_CORE_PLATFORM%\Tools\AppendPackagesPath\GetPackagesPath.py ^
  %WORKSPACE_CORE% ^
') do (
  @set "PACKAGES_PATH=%%i"
)
@if not defined PACKAGES_PATH (
  @echo.
  @echo !!! ERROR !!! Failed to set PACKAGES_PATH. !!!
  @echo.
  set SCRIPT_ERROR=1
  goto :EndPreBuild
)

@REM Generic path and files.
@set USBC_CAPSULE_BUILD_PATH=%CD%
@set USBC_CAPSULE_GEN_TOOLS_PATH=%USBC_CAPSULE_BUILD_PATH%\Tools

@cd ..
@set USBC_CAPSULE_FILE_BUILD_SAMPLE_PATH=%CD%
@if not defined USBC_CAPSULE_CONFIG_PATH (
  set USBC_CAPSULE_CONFIG_PATH=%USBC_CAPSULE_FILE_BUILD_SAMPLE_PATH%\ConfigSample
)
set USBC_CAPSULE_OUTPUT_PATH=%WORKSPACE%\CapsuleOutput
set USBC_CAPSULE_BUILD_RESULT=%USBC_CAPSULE_OUTPUT_PATH%\result.log

@if not defined CAP_PLATFORM_PREFIX (
  set CAP_PLATFORM_PREFIX=UsbC
)
@if not defined OPEN_SSL_SIGN_TOOL_PATH (
  set OPEN_SSL_SIGN_TOOL_PATH=%WORKSPACE%\Binaries\Tools\OpenSSL
)

@REM Integrated Thunderbolt Retimer Payload
set ITBT_RETIMER=ITbtRetimer
@if not defined ITBT_RETIMER_PAYLOAD_CONFIG_FILE (
  set ITBT_RETIMER_PAYLOAD_CONFIG_FILE=%USBC_CAPSULE_CONFIG_PATH%\ITbtRetimerPayloadConfig.ini
)
set ITBT_RETIMER_CAPSULE_PAYLOAD=%USBC_CAPSULE_CONFIG_PATH%\ITbtRetimerPayload.bin

@echo Set basic environment.
@echo.
@echo Prebuild:  Run edksetup.bat batch file.
@echo.
echo %CD%
@cd %WORKSPACE_CORE%
echo %CD%
@call edksetup.bat Rebuild
@if %ERRORLEVEL% NEQ 0 (
  @echo !!! ERROR !!! Failed to run edksetup.bat Rebuild. !!!
  goto :EndPreBuild
)

@if not exist %USBC_CAPSULE_OUTPUT_PATH% mkdir %USBC_CAPSULE_OUTPUT_PATH%
@del %USBC_CAPSULE_BUILD_RESULT% 1>NUL 2>&1
@echo ============= Capsule build result - Begin ============= >> %USBC_CAPSULE_BUILD_RESULT%
@echo. >> %USBC_CAPSULE_BUILD_RESULT%

@REM
@REM Check binaries before building Capsules
@REM
@if not exist %ITBT_RETIMER_PAYLOAD_CONFIG_FILE% (
  @echo.
  @echo ERROR!! ITBT Retimer payload config file does not exist.
  @goto CapsuleNotBuilt
)

@del %ITBT_RETIMER_CAPSULE_PAYLOAD% 1>NUL 2>&1
@%PYTHON_COMMAND% %USBC_CAPSULE_GEN_TOOLS_PATH%\GenerateCapsule\GenRetimerPayloadHeader.py %ITBT_RETIMER_PAYLOAD_CONFIG_FILE%
@if errorlevel 1 (
  @echo.
  @echo ERROR!! Failed to generate ITBT Retimer Capsule payload from %ITBT_RETIMER_PAYLOAD_CONFIG_FILE%.
  @goto CapsuleNotBuilt
)

@if not exist %ITBT_RETIMER_CAPSULE_PAYLOAD% (
  @echo.
  @echo ERROR!! ITBT Retimer binary does not exist.
  @goto CapsuleNotBuilt
)

@REM ===================================================
@REM =            Build ITBT Retimer Capsule           =
@REM ===================================================
@REM Generate Retimer FMP Capsule
@cd %USBC_CAPSULE_GEN_TOOLS_PATH%\GenerateCapsule
@call GenCapsuleTbtRetimer.bat ^
        %ITBT_RETIMER_CAPSULE_PAYLOAD% ^
        %RETIMER_VERSION_DEC% ^
        %USBC_CAPSULE_OUTPUT_PATH% ^
        %CAP_PLATFORM_PREFIX% ^
        %ITBT_RETIMER%

@if errorlevel 1 (
  @echo.
  @echo !! Error when generating ITBT Retimer capsule
  goto CapsuleNotBuilt
)

@REM Write to build result
set USBC_CAPSULE_OUTPUT_NAME=%CAP_PLATFORM_PREFIX%_ITbtRetimer_%RETIMER_VERSION_DEC%.cap
if exist %USBC_CAPSULE_OUTPUT_PATH%\%USBC_CAPSULE_OUTPUT_NAME% (
  @echo Retimer Capsule %USBC_CAPSULE_OUTPUT_NAME% is generated in >> %USBC_CAPSULE_BUILD_RESULT%
  @echo   %USBC_CAPSULE_OUTPUT_PATH% >> %USBC_CAPSULE_BUILD_RESULT%
  @echo. >> %USBC_CAPSULE_BUILD_RESULT%
)

@echo.
@echo.
@echo.
if exist %USBC_CAPSULE_BUILD_RESULT% (
  @echo ============= Capsule build result - End =============== >> %USBC_CAPSULE_BUILD_RESULT%
  type %USBC_CAPSULE_BUILD_RESULT%
  @del %USBC_CAPSULE_BUILD_RESULT% 1>NUL 2>&1
)
@echo.
@echo Capsule build is completed.
@echo.
goto CapsuleBuildDone

@REM
@REM ============ Error handle ============
@REM
:WrongParametersStop
@echo.
@echo !! The Capsule build has failed in prep phase!
@echo.
@goto CapsuleBuildDone

:CapsuleNotBuilt
@echo.
@echo !! CapsuleBuild failed.
@echo.

:CapsuleBuildDone
@if defined USBC_CAPSULE_BUILD_PATH (
    @cd %USBC_CAPSULE_BUILD_PATH%
)
