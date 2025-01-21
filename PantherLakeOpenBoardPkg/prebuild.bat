@REM @file
@REM  Pre build script.
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
@echo on
@echo ********************************************************************
@echo *******************     PreBuild.BAT     ***************************
@echo ********************************************************************

@echo %CD%
cd ..\..\
set WORKSPACE=%CD%
@echo %CD%

@REM Set build TARGET.
@if /I "%1" == "" (
  set TARGET=DEBUG
  set TARGET_SHORT=D
  set FSP_BUILD_PARAMETER=/d
) else if /I "%1" == "DEBUG" (
  set TARGET=DEBUG
  set TARGET_SHORT=D
  set FSP_BUILD_PARAMETER=/d
) else if /I "%1" == "RELEASE" (
  set TARGET=RELEASE
  set TARGET_SHORT=R
  set FSP_BUILD_PARAMETER=/r
) else (
  echo.
  echo !!! ERROR !!! Incorrect TARGET option for prebuild.bat. !!!
  echo.
  set SCRIPT_ERROR=1
  goto :EndPreBuild
)

@set CAPSULE_BUILD=0

@if /I "%2" == "TRUE" (
  @set CAPSULE_BUILD=1
  goto StartCapsulePrep
)

:StartCapsulePrep

set WORKSPACE=%CD%
@REM
@REM In order to avoid dual backslash marks, remove the ending backslash mark for file path concatenation.
@REM
if %WORKSPACE:~-1%==\ (
  set WORKSPACE_ROOT=%WORKSPACE:~0,-1%
) else (
  set WORKSPACE_ROOT=%WORKSPACE%
)
@set WORKSPACE_CORE=%WORKSPACE_ROOT%\Edk2
@set WORKSPACE_CORE_PLATFORM=%WORKSPACE_ROOT%\Edk2Platforms\Platform\Intel
@set WORKSPACE_CORE_FEATURES=%WORKSPACE_ROOT%\Edk2Platforms\Features\Intel
@set WORKSPACE_CORE_SILICON=%WORKSPACE_ROOT%\Edk2Platforms\Silicon\Intel
@set WORKSPACE_COMMON=%WORKSPACE_ROOT%\Intel
@set WORKSPACE_PLATFORM=%WORKSPACE_ROOT%\Intel
@set WORKSPACE_SILICON=%WORKSPACE_ROOT%\Intel
@set WORKSPACE_FSP_BIN=%WORKSPACE%\Intel
@set WORKSPACE_BINARIES=%WORKSPACE%\Binaries
@set EDK_TOOLS_BIN=%WORKSPACE_CORE%\BaseTools\Bin\Win32
@set EDK_TOOLS_PATH=%WORKSPACE_CORE%\BaseTools
@set WORKSPACE_CONF=%WORKSPACE_ROOT%\Conf
@set WORKSPACE_ROM=%WORKSPACE_ROOT%\RomImages
if not defined BOARD set BOARD=PantherLakeOpenboard

@set SCRIPT_ERROR=0
@set CATALOG_DEBUG=0

@if not exist %WORKSPACE_CONF% mkdir %WORKSPACE_CONF%

@if not defined PYTHON_COMMAND (
  set PYTHON_COMMAND=py -3

  %PYTHON_COMMAND% --version
  if not %ERRORLEVEL% == 0 (
    @echo !!! ERROR !!! PYTHON_COMMAND not setting and py -3 is invalid
    goto :EndPreBuild
  )
)

@set PACKAGES_PATH=
@for /f %%i in (
  '%PYTHON_COMMAND% %WORKSPACE_CORE_PLATFORM%\Tools\AppendPackagesPath\GetPackagesPath.py ^
  %WORKSPACE_CORE_SILICON% %WORKSPACE_CORE_PLATFORM% %WORKSPACE_CORE_FEATURES% ^
  %WORKSPACE_CORE% %WORKSPACE%\Intel ^
  %WORKSPACE_BINARIES% ^
') do (
  @set "PACKAGES_PATH=%%i"
)

@echo Set basic environment.
@echo.
@echo Prebuild:  Run edksetup.bat batch file.
@echo.
echo %CD%
@if %CATALOG_DEBUG% == 0 (
  @del %WORKSPACE%\Conf\build_rule.txt
)
echo %CD%
cd %WORKSPACE_CORE%
echo %CD%
@call edksetup.bat Rebuild

@echo.
@echo Build tools in Edk2Platforms
@echo.
cd %WORKSPACE_CORE_SILICON%\Tools
nmake

echo %CD%
cd %WORKSPACE%
echo %CD%
@set EFI_SOURCE=%WORKSPACE_CORE%
@set PATH=%NASM_PREFIX%;%PATH%

@REM
@REM Get Visual Studio environment Setting By Edksetup.
@REM Order of precedence is 2019, 2017, 2015, 2012, 2013, 2010 and then 2008.
@REM
@if not defined TOOL_CHAIN_TAG (
  if defined VS160COMNTOOLS (
    set TOOL_CHAIN_TAG=VS2019
  ) else if defined VS150COMNTOOLS (
    set TOOL_CHAIN_TAG=VS2017
  ) else if defined VS140COMNTOOLS (
    set TOOL_CHAIN_TAG=VS2015
  )
)
@if /I "%TOOL_CHAIN_TAG%"=="VS2015" (
  if /I "%VS140COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 14.0\Common7\Tools\" (
    set TOOL_CHAIN_TAG=VS2015
  ) else (
    set TOOL_CHAIN_TAG=VS2015x86
  )
)

@echo If no supported version of Visual Studio was detected, return an error.
@if not defined TOOL_CHAIN_TAG (
  echo.
  echo !!! ERROR !!! Visual Studio not installed correctly!!!
  echo.
  set SCRIPT_ERROR=1
  goto :EndPreBuild
)

@echo Show CL revision
cl
@if %ERRORLEVEL% NEQ 0 (
  @echo !!! ERROR !!! Failed to show CL revision !!!
  set SCRIPT_ERROR=1
  goto :EndPreBuild
)

@set BUILD_DIR=Build\%PLATFORM_BOARD_PACKAGE%\%TARGET%_%TOOL_CHAIN_TAG%
@set BUILD_DIR_PATH=%WORKSPACE%\%BUILD_DIR%
@set BUILD_X64=%BUILD_DIR_PATH%\X64
@set BUILD_IA32=%BUILD_DIR_PATH%\IA32

@echo.
@echo Prebuild:  Set build environment.
@echo.
@echo BUILD_IA32 = %BUILD_IA32%
@echo BUILD_X64 = %BUILD_X64%
@if not exist %BUILD_DIR_PATH% (
  mkdir %BUILD_DIR_PATH%
)
@if not exist %BUILD_IA32% (
  mkdir %BUILD_IA32%
)
@if not exist %BUILD_X64% (
  mkdir %BUILD_X64%
)

@findstr /V "ACTIVE_PLATFORM TARGET TARGET_ARCH TOOL_CHAIN_TAG BUILD_RULE_CONF" %WORKSPACE%\Conf\target.txt > %BUILD_DIR_PATH%\target.txt
@echo ACTIVE_PLATFORM = %WORKSPACE_PLATFORM%/%PLATFORM_BOARD_PACKAGE%/OpenBoardPkg.dsc        >> %BUILD_DIR_PATH%\target.txt
@echo TARGET          = %TARGET%                                  >> %BUILD_DIR_PATH%\target.txt
@echo TARGET_ARCH     = %FSP_ARCH% X64                                  >> %BUILD_DIR_PATH%\target.txt
@echo TOOL_CHAIN_TAG  = %TOOL_CHAIN_TAG%                          >> %BUILD_DIR_PATH%\target.txt
@echo BUILD_RULE_CONF = Conf/build_rule.txt                       >> %BUILD_DIR_PATH%\target.txt
@move /Y %BUILD_DIR_PATH%\target.txt Conf

@echo Current Directory = %CD%
@echo WORKSPACE_FSP_BIN = %WORKSPACE_FSP_BIN%
@if %CAPSULE_BUILD% == 1 (
  goto EndCapsulePrep
)

@REM
@echo Skip BIOS_SIZE_OPTION if it is predefined
@REM
@if NOT "%BIOS_SIZE_OPTION%" == "" goto BiosSizeDone

@set BIOS_SIZE_OPTION=

@REM default size option is 16M
@set BIOS_SIZE_OPTION=-DBIOS_SIZE_OPTION=SIZE_140

:BiosSizeDone
@echo BIOS_SIZE_OPTION=%BIOS_SIZE_OPTION%

:EndCapsulePrep
@echo.
@echo   Prebuild is complete.
@echo   Current Directory    = %CD%
@echo   EFI_SOURCE           = %EFI_SOURCE%
@echo   TARGET               = %TARGET%
@echo   TARGET_ARCH          = %FSP_ARCH% X64
@echo   TOOL_CHAIN_TAG       = %TOOL_CHAIN_TAG%
@echo   WORKSPACE            = %WORKSPACE%
@echo   WORKSPACE_CORE       = %WORKSPACE_CORE%
@echo   EXT_BUILD_FLAGS      = %EXT_BUILD_FLAGS%
@echo   WORKSPACE_PLATFORM   = %WORKSPACE_PLATFORM%
@echo.
:EndPreBuild
cd %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%

@echo TARGET              = %TARGET%
@echo BUILD_X64           = %BUILD_X64%
@echo BUILD_OPTION_PCD    = %BUILD_OPTION_PCD%
