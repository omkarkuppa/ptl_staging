@REM @file
@REM
@REM  @copyright
@REM  INTEL CONFIDENTIAL
@REM  Copyright (C) 2013 Intel Corporation.
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

if "%~1" == "/?" goto USAGE
if "%~1" == "/h" goto USAGE
@set NOTIMESTAMP=

setlocal enableextensions enabledelayedexpansion
set BUILD_ARGS=%*
@if not defined WORKSPACE (
  set WORKSPACE=%CD%
)
@if not defined WORKSPACE_CORE (
  set WORKSPACE_CORE=%WORKSPACE%\Edk2
)
@if not defined WORKSPACE_CORE_SILICON (
  set WORKSPACE_CORE_SILICON=%WORKSPACE%\Edk2Platforms\Silicon\Intel
)
@if not defined WORKSPACE_COMMON (
  set WORKSPACE_COMMON=%WORKSPACE%\Intel
)
@if not defined WORKSPACE_SILICON (
  set WORKSPACE_SILICON=%WORKSPACE%\Intel
)
@if not defined WORKSPACE_BINARIES (
  @if exist %WORKSPACE%\Binaries (
    set WORKSPACE_BINARIES=%WORKSPACE%\Binaries
  )
)
set WORKSPACE_PLATFORM=%WORKSPACE%\Intel
set PACKAGES_PATH=%WORKSPACE%;%WORKSPACE_CORE_SILICON%;%WORKSPACE_CORE%;%WORKSPACE_COMMON%;%WORKSPACE_SILICON%;%WORKSPACE_PLATFORM%
@if exist %WORKSPACE%\Binaries (
  set PACKAGES_PATH=%PACKAGES_PATH%;%WORKSPACE_BINARIES%
)

@if not defined NASM_PREFIX_EXT (
  echo NASM_PREFIX_EXT not defined, Default path set to "%SystemDrive%\nasm\"
  @set NASM_PREFIX_EXT=%SystemDrive%\nasm\
)
@if not defined NASM_PREFIX (
  @set NASM_PREFIX=%NASM_PREFIX_EXT%
)

@if not defined PACKAGES_PATH (
  set PACKAGES_PATH=%WORKSPACE%;%WORKSPACE_CORE_SILICON%;%WORKSPACE_CORE%;%WORKSPACE_COMMON%;%WORKSPACE_SILICON%
)
set EDK_TOOLS_PATH=%WORKSPACE_CORE%\BaseTools
set EDK_TOOLS_BIN=%WORKSPACE_CORE%\BaseTools\Bin\Win32

@if not exist %WORKSPACE%\Conf mkdir %WORKSPACE%\Conf
@if not defined PYTHON_COMMAND (
  set PYTHON_COMMAND=py -3
)

for %%a in (%*) do (
  if /i "%%a"=="fspsigned" (
    @set FSP_SIGNED =TRUE
  ) else (
    @set FSP_SIGNED=FALSE
  )
)

@if not defined FSP_RESET (
  @set FSP_RESET=TRUE
)

rem Perform pre-build
rem -----------------

set DEFAULT_TARGET_FILE=CurrentTarget.txt
  rem Check the first command line parameter for a package name
  rem ---------------------------------------------------------
  if not "%~1" == "" (
    set FIRSTCHAR=%1
    set FIRSTCHAR=!FIRSTCHAR:~0,1!
    if not "!FIRSTCHAR!" == "/" (
      rem First argument is a package name.
      call :UPDATE_DEFAULT_TARGET %~1
      if not !ERRORLEVEL! == 0 goto DIE
      shift
      )
    set FIRSTCHAR=
  )

  rem Check for default target file
  rem -----------------------------
  if not exist %DEFAULT_TARGET_FILE% (
    echo ERROR: Please specify a package to build.
    echo.
    call :USAGE
    goto DIE
    )

  rem Handle Build environment variables in case of fspsigned
  rem --------------------------------------------------------
  for %%a in (%*) do (
    if /i "%%a"=="fspsigned" (
      set FSP_RESET=TRUE
      set FSP_SIGNED=TRUE
      set SI_BUILD_OPTION_PCD=%SI_BUILD_OPTION_PCD% --pcd gSiPkgTokenSpaceGuid.PcdSignedFspEnable=TRUE
      set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% --pcd gFspWrapperFeaturePkgTokenSpaceGuid.PcdFspWrapperResetVectorInFsp=TRUE ^
                                              --pcd gIntelFsp2WrapperTokenSpaceGuid.PcdFspMeasurementConfig=0 ^
                                              --pcd gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection=0
    )
    if /i "%%a"=="perf" (
      set PERFORMANCE_BUILD=TRUE
      set BUILD=P
      set ROM_FILENAME_SPECIAL_BUILD_TYPE=_Performance
      set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
      --pcd gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable=TRUE ^
      --pcd gSiPkgTokenSpaceGuid.PcdBootGuardPerformanceEnable=TRUE
      set FSP_BUILD_OPTION_PCD=%FSP_BUILD_OPTION_PCD%^
      --pcd gPantherLakeFspPkgTokenSpaceGuid.PcdFspPerformanceEnable=TRUE
    )
  )
  rem Remove shifted arguments by reconstructing BUILD_ARGS
  rem -----------------------------------------------------
  set BUILD_ARGS=
  set FV_SPARE_SPACE_THRESHOLD=
  set CLEAN_BUILD=FALSE
  :CONSTRUCT_BUILD_ARGS_LOOP
    if "%~1"=="" goto CONSTRUCT_BUILD_ARGS_CONTINUE
    if /i "notimestamp" == "%1" (
      set NOTIMESTAMP=1
    ) else if /i "chksize" == "%1" (
      if "" == "%2" goto InvalidThreshold
      set FV_SPARE_SPACE_THRESHOLD=%2
      shift
    ) else if /i "/clean" == "%1" (
      set CLEAN_BUILD=TRUE
      set BUILD_ARGS=%BUILD_ARGS% %1
    ) else if /i "embedded" == "%1" (
      set FSP_BUILD_OPTION_PCD=%FSP_BUILD_OPTION_PCD%  --pcd gSiPkgTokenSpaceGuid.PcdEmbeddedEnable=0x1
    ) else if /i "stage4" == "%1" (
      set FSP_BUILD_OPTION_PCD=%FSP_BUILD_OPTION_PCD%  --pcd gSiPkgTokenSpaceGuid.PcdS3Enable=FALSE
    ) else (
      set BUILD_ARGS=%BUILD_ARGS% %1
    )
    shift
    goto CONSTRUCT_BUILD_ARGS_LOOP
  :CONSTRUCT_BUILD_ARGS_CONTINUE

  if not defined FV_SPARE_SPACE_THRESHOLD goto :SkipThreshold
  set /A THRESHOLD=%FV_SPARE_SPACE_THRESHOLD%
  if not defined THRESHOLD goto:InvalidThreshold
  if %THRESHOLD% GTR 0 (
    set EXT_BUILD_FLAGS=%EXT_BUILD_FLAGS% -D FV_SPARE_SPACE_THRESHOLD=%THRESHOLD%
    set THRESHOLD=
    goto :SkipThreshold
  )

  :InvalidThreshold
  echo Invalid threshold of chksize: "%FV_SPARE_SPACE_THRESHOLD%"
  echo.
  call :USAGE
  goto DIE

  :SkipThreshold
  @echo EXT_BUILD_FLAGS      = %EXT_BUILD_FLAGS%

  call :PREBUILD
  if not %ERRORLEVEL% == 0 goto DIE

  rem Build package
  rem -------------
  rem Find build script.
  set /p BUILD_TARGET=<%DEFAULT_TARGET_FILE%
  set BUILD_TARGET=%WORKSPACE_SILICON%\%BUILD_TARGET%FspPkg\BuildFv.cmd
  if not exist %BUILD_TARGET% (
    echo ERROR: Unable to find '%BUILD_TARGET%'.
    goto DIE
    )
  call %BUILD_TARGET% %BUILD_ARGS%
  if not %ERRORLEVEL% == 0 goto DIE

  rem Done
  rem ----
  goto END

:UPDATE_DEFAULT_TARGET
rem ---------------------------------------------------------------------------
rem Update the default target
rem ---------------------------------------------------------------------------
  set TARGET=%~1

  rem Store target for later
  echo Setting default target to '%TARGET%'.
  echo %TARGET%>%DEFAULT_TARGET_FILE%
  set TARGET=

  goto END

:PREBUILD
if /I "%CLEAN_BUILD%" == "TRUE" goto END

rem
rem Determine which version of Visual Studio is installed and configure
rem TOOL_CHAIN_TAG appropriately.
rem Order of precedence is 2019, and then 2017.
rem
if not defined TOOL_CHAIN_TAG (
  if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" (
    set "VS_WHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
    goto VsCheck
  ) else if exist "%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe" (
    set "VS_WHERE=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"
    goto VsCheck
  ) else (
    goto VsCheckDone
  )
) else (
  goto VsCheckDone
)
:VSCheck
for /f "usebackq tokens=1* delims=: " %%i in (`"%VS_WHERE%" -version [16.0^,17.0^) -products * -requires Microsoft.Component.MSBuild`) do (
  if /i "%%i"=="installationPath" set VS_INSTALL_PATH=%%j
)
if defined VS_INSTALL_PATH (
  set TOOL_CHAIN_TAG=VS2019
  goto VsCheckDone
)
for /f "usebackq tokens=1* delims=: " %%i in (`"%VS_WHERE%" -version [15.0^,16.0^) -products * -requires Microsoft.Component.MSBuild`) do (
  @if /i "%%i"=="installationPath" set VS_INSTALL_PATH=%%j
)
if defined VS_INSTALL_PATH (
  set TOOL_CHAIN_TAG=VS2017
  goto VsCheckDone
)
:VsCheckDone
set VS_INSTALL_PATH=
set USE_VS_COMPILER=0
set USE_MINGW=0
if /I "%TOOL_CHAIN_TAG%"=="VS2019" (
  set USE_VS_COMPILER=1
)
if /I "%TOOL_CHAIN_TAG%"=="VS2017" (
  set USE_VS_COMPILER=1
)
if /I "%TOOL_CHAIN_TAG%"=="CLANGPDB" (
  if defined BASETOOLS_MINGW_PATH (
    set USE_MINGW=1
  )
)
if %USE_VS_COMPILER% NEQ 0 (
  call %WORKSPACE_CORE%\edksetup.bat Rebuild %TOOL_CHAIN_TAG%
) else (
  if %USE_MINGW% NEQ 0 (
    call %WORKSPACE_CORE%\edksetup.bat Rebuild Mingw-w64
  )
)
set USE_VS_COMPILER=
set USE_MINGW=

@REM
@REM Check for Python support
@REM
set PYTHON_MIN_VERSION=(2,5)
call :FIND_PYTHON
if not %ERRORLEVEL% == 0 goto DIE

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

@REM If no supported version of Visual Studio was detected, return an error.
@if not defined TOOL_CHAIN_TAG (
  echo.
  echo !!! ERROR !!! Visual Studio not installed correctly!!!
  echo.
  exit /b 1
)

  goto END

:FIND_PYTHON
rem ---------------------------------------------------------------------------
rem Looks for Python installed on the system.
rem ---------------------------------------------------------------------------
  set PYTHON_DEF_EXE=python.exe
  set PYTHON_EXE=
  set PYTHON_FOUND_IN_PATH=0

  rem Check the PYTHON_COMMAND for PYTHON_EXE
  if defined PYTHON_COMMAND (
    set PYTHON_EXE=%PYTHON_COMMAND%
  )
  if defined PYTHON_EXE goto FIND_PYTHON_FOUND

  rem Check the PATH environment variable
  for %%X in (%PYTHON_DEF_EXE%) do (set PYTHON_EXE=%%~$PATH:X)
  if defined PYTHON_EXE (
    set PYTHON_FOUND_IN_PATH=1
    goto FIND_PYTHON_FOUND
    )

  rem Python not found
  echo Error: %PYTHON_DEF_EXE% could not be located on this system. If^
  Python has been installed to a nonstandard location (the typical installation^
  location is C:\Python25\%PYTHON_DEF_EXE%), ensure that the directory^
  containing the Python executable has been added to the PATH environment^
  variable.
   call :FIND_PYTHON_CLEANUP
  goto DIE

  :FIND_PYTHON_FOUND
  echo Python was found at '%PYTHON_EXE%'.
  call :PYTHON_VERSION_TEST "%PYTHON_EXE%" "%PYTHON_MIN_VERSION%"
  if not %ERRORLEVEL% == 0 (
  echo Error: The version of Python installed on this system at^
  '%PYTHON_EXE%' is too old to run necessary scripts. Please upgrade^
  your Python installation to version !PYTHON_MIN_VERSION! or newer.
    call :FIND_PYTHON_CLEANUP
    goto DIE
    )

  :FIND_PYTHON_CLEANUP
  set PYTHON_DEF_EXE=
  set PYTHON_DEF_DIR=
  set PYTHON_EXE=
  set PYTHON_FOUND_IN_PATH=

  goto END

:PYTHON_VERSION_TEST
rem ---------------------------------------------------------------------------
rem Test the Python executable %1 for version >= %2.
rem ---------------------------------------------------------------------------
  %~1 -c "import sys; sys.exit(sys.version_info<%~2)"
  if not %ERRORLEVEL% == 0 goto DIE
  goto END

:USAGE
rem ------------------------------------------------------------------------------
rem Print usage and exit
rem ------------------------------------------------------------------------------
  echo.
  echo ===============================================================================
  echo = For backward compatibility following usage will still be supported          =
  echo = Usage: BuildFsp [package name]                                              =
  echo =                 [platform name (optional)]                                  =
  echo =                 [/h or /? or /r or /d or /mr or /md or /clean]              =
  echo =                 [/header]                                                   =
  echo =                 [notimestamp]                                               =
  echo =                 [chksize=threshold]                                         =
  echo =                 [fsp64]                                                     =
  echo =                 [fspsigned]                                                 =
  echo =                                                                             =
  echo =     Example build for PantherLakeFspPkg:                                    =
  echo =       .\Intel\OneSiliconPkg\Fsp\BuildFsp.cmd PantherLake /d                 =
  echo =                                                                             =
  echo =     Example build for PantherLakeFspPkg with chksize:                       =
  echo =       .\Intel\OneSiliconPkg\Fsp\BuildFsp.cmd PantherLake /d chksize=512     =
  echo ===============================================================================
  echo = By default BuildFsp assumes below paths when executing:                     =
  echo =   \Edk2\                 : FSP required open source packages                =
  echo =   \Intel\                : Rest of the FSP required packages                =
  echo =   %PYTHON_COMMAND%       : For supporting FSP build scripts                 =
  echo =   nasm.exe               : Should be under NASM_PREFIX_EXT path             =
  echo ===============================================================================
  goto END

:DIE
rem ---------------------------------------------------------------------------
rem Exit returning a failure error code
rem ---------------------------------------------------------------------------
  exit /B 1

:END
rem ---------------------------------------------------------------------------
rem Exit returning a success error code
rem ---------------------------------------------------------------------------
  exit /B 0
