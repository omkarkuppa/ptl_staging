@REM  @file
@REM  Clean Fit Payload on PantherLake platform.
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

@echo off
echo.
echo Run build cleanall...
echo.
echo Build Clean from FitPayloads Start

pushd ..\..\..\..\

if not defined WORKSPACE set WORKSPACE=%CD%
@REM
@REM In order to avoid dual backslash marks, remove the ending backslash mark for file path concatenation.
@REM
if %WORKSPACE:~-1%==\ (
  set WORKSPACE_ROOT=%WORKSPACE:~0,-1%
) else (
  set WORKSPACE_ROOT=%WORKSPACE%
)
if not defined WORKSPACE_CORE set WORKSPACE_CORE=%WORKSPACE_ROOT%\Edk2
if not defined WORKSPACE_COMMON set WORKSPACE_COMMON=%WORKSPACE_ROOT%\Intel
if not defined WORKSPACE_PLATFORM set WORKSPACE_PLATFORM=%WORKSPACE_ROOT%\Intel
if not defined WORKSPACE_SILICON set WORKSPACE_SILICON=%WORKSPACE_ROOT%\Intel
if not defined WORKSPACE_BINARIES set WORKSPACE_BINARIES=%WORKSPACE_ROOT%\Binaries
where git.exe > NUL
if %ERRORLEVEL% EQU 0 (
    set GIT_FOUND=1
) else (
    set GIT_FOUND=0
)
if exist %WORKSPACE_SILICON%\.git (
  if %GIT_FOUND% NEQ 0 (
    set SILICON_GIT=1
  ) else (
    set SILICON_GIT=0
  )
) else (
  set SILICON_GIT=0
)

REM build cleanall

echo.
echo Directories to clean...
echo.

if exist %WORKSPACE%\Build rmdir /q /s %WORKSPACE%\Build
if exist %WORKSPACE%\Conf\.cache rmdir /q /s %WORKSPACE%\Conf\.cache
if exist %WORKSPACE%\BaseToolsTmp rmdir /q /s %WORKSPACE%\BaseToolsTmp
if exist %WORKSPACE%\__pycache__ rmdir /q /s %WORKSPACE%\__pycache__

echo.
echo Files to clean...
echo.

if exist %WORKSPACE%\RomImages\FITPAYLOADS.fd del %WORKSPACE%\RomImages\FITPAYLOADS.fd
if exist %WORKSPACE_ROOT%\edk2.log del %WORKSPACE_ROOT%\edk2.log
if exist %WORKSPACE_ROOT%\Conf\build_rule.txt del %WORKSPACE_ROOT%\Conf\build_rule.txt
if exist %WORKSPACE_ROOT%\Conf\FrameworkDatabase.db del %WORKSPACE_ROOT%\Conf\FrameworkDatabase.db
if exist %WORKSPACE_ROOT%\Conf\target.txt del %WORKSPACE_ROOT%\Conf\target.txt
if exist %WORKSPACE_ROOT%\Conf\tools_def.txt del %WORKSPACE_ROOT%\Conf\tools_def.txt

popd

echo Build Clean from FitPayloads End
echo.
echo All done...
echo.
@echo on
