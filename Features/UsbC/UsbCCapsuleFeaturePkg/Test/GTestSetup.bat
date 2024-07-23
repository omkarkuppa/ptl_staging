@REM @file
@REM Google test setup script.
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
@REM Define environment variables.
@REM

@set GOOGLE_TEST_BUILD_PATH=%CD%

@REM Check Edk2 Directory
@REM Variables description
@REM HC (Hierarchy Count), PDH (Parent Directory Hierarchy)
@set /a HC=1,PDH=10
:CheckEdk2Directory
@if not exist edk2 (
  @if %HC% GTR %PDH% (
    @echo !!! ERROR !!! Failed to found edk2 module in %PDH% parent directory hierarchy.
    goto EndGtestSetup
  )
  cd ..
  @set /a HC+=1
  goto CheckEdk2Directory
)

@REM Set WORKSPACE environment.
@set WORKSPACE=%CD%

@set WORKSPACE_CORE=%WORKSPACE%\Edk2
@set WORKSPACE_CORE_PLATFORM=%WORKSPACE%\Edk2Platforms\Platform\Intel
@set WORKSPACE_SILICON=%WORKSPACE%\Intel

@set PACKAGES_PATH=
@for /f %%i in (
  '%PYTHON_COMMAND% %WORKSPACE_CORE_PLATFORM%\Tools\AppendPackagesPath\GetPackagesPath.py ^
  %WORKSPACE%
  %WORKSPACE_CORE% ^
') do (
  @set "PACKAGES_PATH=%%i"
)
@if not defined PACKAGES_PATH (
  @echo.
  @echo !!! ERROR !!! Failed to set PACKAGES_PATH. !!!
  @echo.
  set SCRIPT_ERROR=1
  goto :EndGtestSetup
)

@echo Set basic environment.
@echo.
@echo GtestSetup:  Run edksetup.bat batch file.
@echo.
echo %CD%
@cd %WORKSPACE_CORE%
echo %CD%
@call edksetup.bat Rebuild
@if %ERRORLEVEL% NEQ 0 (
  @echo !!! ERROR !!! Failed to run edksetup.bat Rebuild. !!!
  goto :EndGtestSetup
)

:EndGtestSetup
@if defined GOOGLE_TEST_BUILD_PATH (
    @cd %GOOGLE_TEST_BUILD_PATH%
)
