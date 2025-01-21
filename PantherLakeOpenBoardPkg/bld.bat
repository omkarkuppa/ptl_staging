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
:: Useage: bld [/s] [/f <FEATURE_PCD_NAME> <FALSE or TRUE>] [/r]
::
:: For a given build command, 3 options may be passed into this batch file via command prompt:
:: 1) /s = Redirects all output to a file called EDK2.log(Prep.log must be existed), which will be located at the root.
:: 2) /f = Defines the passing in of a single override to a feature PCD that is used in the platform
::    DSC file.  If this parameter is used, it is to be followed immediately after by both the feature
::    pcd name and value. FeaturePcd is the full PCD name, like gMinPlatformPkgTokenSpaceGuid.PcdOptimizeCompilerEnable
:: 3) /r = Useful for faster rebuilds when no changes have been made to .inf files. Passes -u to
::    build.exe to skip the generation of makefiles.
:: 4) rom = Build Bios.rom only and building SPIs will be skipped.
::
@set BLDTIMESTAMP=%time%

@set FLASHMAP_FDF=%WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Include\Fdf\FlashMapInclude.fdf

@echo on
@echo ********************************************************************
@echo ******************    BLD.BAT    ***********************************
@echo ********************************************************************

@rem cd %WORKSPACE%
@echo %CD%

@SET SILENT_MODE=FALSE
@SET REBUILD_MODE=
@SET BUILD_ROM_ONLY=

@if not defined PYTHON_COMMAND (
  set PYTHON_COMMAND=py -3
)

:: Loop through arguements until all are processed
:BUILD_FLAGS_LOOP
@if "%~1" == "" goto BUILD_FLAGS_LOOP_DONE

@if "%~1" == "/f" (
  shift
  goto BUILD_FLAGS_LOOP
)
@if "%~1" == "/s" (
  SET SILENT_MODE=TRUE
  @if exist %WORKSPACE%\Build.log del %WORKSPACE%\Build.log
  shift
  goto BUILD_FLAGS_LOOP
)
@if "%~1" == "/r" (
  SET REBUILD_MODE=-u
  shift
  goto BUILD_FLAGS_LOOP
)
@if "%~1" == "rom" (
  SET BUILD_ROM_ONLY=rom
  shift
  goto BUILD_FLAGS_LOOP
)
:: Unknown build flag.
shift
goto BUILD_FLAGS_LOOP
:BUILD_FLAGS_LOOP_DONE

:: Output the build variables the user has selected.
@echo.
@echo  User Selected build options:
@echo    SILENT_MODE = %SILENT_MODE%
@echo    REBUILD_MODE = %REBUILD_MODE%
@echo    BUILD_ROM_ONLY = %BUILD_ROM_ONLY%
@echo.

del /f %WORKSPACE_SILICON%\%PLATFORM_FSP_BIN_PACKAGE%\Fsp_Rebased*.fd

cd %WORKSPACE%
@echo %CD%
@echo ********************* BLD.BAT *****************************

@echo WORKSPACE_PLATFORM     = %WORKSPACE_PLATFORM%
@echo WORKSPACE_SILICON      = %WORKSPACE_SILICON%
@echo PROJECT                = %PROJECT%
@echo PLATFORM_PACKAGE       = %PLATFORM_PACKAGE%
@echo PLATFORM_BOARD_PACKAGE = %PLATFORM_BOARD_PACKAGE%
@echo WORKSPACE_FSP_BIN      = %WORKSPACE_FSP_BIN%

@if exist %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\OpenBoardPkgPcdInit.dsc attrib -r %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\OpenBoardPkgPcdInit.dsc
@echo .

@REM Split Fsp.fd and generate FspTopAt4G.fd, then add padding data before this binary to FspTopAt4G.fd for alignment
@REM FspTopAt4G.fd contains Free space(at least 8KB) + Fsp_T.fd + Fsp_O.fd
@if %FSP_RESET% EQU TRUE (
  @echo call FspTopGen.py to generate FspTopAt4G.fd.
  @call %PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\BoardSupport\Tools\FspTopGen\FspTopGen.py -SplitBin %EDK_TOOLS_PATH%\Source\Python\Split\Split.py
  @if %errorlevel% NEQ 0 (
    @echo !!! ERROR:generate FspTopAt4G.fd failed!!!
    set SCRIPT_ERROR=1
    goto :BldFail
  )
)

@call %PYTHON_COMMAND% %WORKSPACE_CORE_PLATFORM%\%PLATFORM_PACKAGE%\Tools\Fsp\RebaseFspBinBaseAddress.py %FLASHMAP_FDF% %WORKSPACE_FSP_BIN%\PantherLakeFspBinPkg Fsp_Tmp.fd 0x0
@echo .
@if %ERRORLEVEL% NEQ 0 (
  @echo !!! ERROR:RebaseFspBinBaseAddress failed!!!
  set SCRIPT_ERROR=1
  goto :BldFail
)

cd %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%

copy /y /b %WORKSPACE_SILICON%\%PLATFORM_FSP_BIN_PACKAGE%\Fsp_Rebased_S.fd+%WORKSPACE_SILICON%\%PLATFORM_FSP_BIN_PACKAGE%\Fsp_Rebased_M.fd+%WORKSPACE_SILICON%\%PLATFORM_FSP_BIN_PACKAGE%\Fsp_Rebased_T.fd %WORKSPACE_SILICON%\%PLATFORM_FSP_BIN_PACKAGE%\Fsp_Rebased.fd
:SkipPatchFspBinFvsBaseAddress

@echo Current Directory = %CD%
@echo ********************* BLD.BAT *****************************
@echo SILENT_MODE          = %SILENT_MODE%
@echo NUMBER_OF_PROCESSORS = %NUMBER_OF_PROCESSORS%
@echo REBUILD_MODE         = %REBUILD_MODE%
@echo EXT_BUILD_FLAGS      = %EXT_BUILD_FLAGS%

@if %SILENT_MODE% EQU TRUE goto BldSilent
@set BUILDTIMESTAMP=%time%
call build -n %NUMBER_OF_PROCESSORS% %REBUILD_MODE% %EXT_BUILD_FLAGS%

@echo %CD%
@echo ********************* BLD.BAT *****************************
@echo ERRORLEVEL=%ERRORLEVEL%

@if %ERRORLEVEL% NEQ 0 goto BldFail
@set BUILDTIMESTAMP=%BUILDTIMESTAMP% -%time%

@set POSTBUILDTIMESTAMP=%time%
@echo.
@echo Running postbuild.bat to complete the build process.
@echo.
call %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\postbuild.bat %BUILD_ROM_ONLY%
echo Errorlevel= %errorlevel%
echo SCRIPT_ERROR= %SCRIPT_ERROR%
@if %SCRIPT_ERROR% NEQ 0 goto BldFail
@if %errorlevel% NEQ 0 goto BldFail
@set POSTBUILDTIMESTAMP=%POSTBUILDTIMESTAMP% -%time%
@goto BldSuccess

:BldSilent
@echo.
@echo ********************************************************************
@echo ***********            Build is launched here             **********
@echo ********************************************************************
@echo.
@set BUILDTIMESTAMP=%time%
call build -n %NUMBER_OF_PROCESSORS% %REBUILD_MODE% %EXT_BUILD_FLAGS% 1>>%WORKSPACE%\Build.log 2>&1
@echo ERRORLEVEL=%ERRORLEVEL% 1>>%WORKSPACE%\Build.log 2>&1

@if %ERRORLEVEL% NEQ 0 goto BldFail
@set BUILDTIMESTAMP=%BUILDTIMESTAMP% -%time%

@set POSTBUILDTIMESTAMP=%time%
@echo. >>%WORKSPACE%\Build.log
@echo Running postbuild.bat to complete the build process. >> %WORKSPACE%\Build.log
@echo. >>%WORKSPACE%\Build.log
call %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\postbuild.bat %BUILD_ROM_ONLY% 1>>%WORKSPACE%\Build.log 2>&1
echo Errorlevel= %errorlevel%
echo SCRIPT_ERROR= %SCRIPT_ERROR%
@If %SCRIPT_ERROR% NEQ 0 goto BldFail
@if %errorlevel% NEQ 0 goto BldFail
@set POSTBUILDTIMESTAMP=%POSTBUILDTIMESTAMP% -%time%

:BldSuccess
@echo.
@echo TARGET:               %TARGET%
@echo TOOL_CHAIN_TAG:       %TOOL_CHAIN_TAG%
@echo BIOS location:        %BUILD_DIR%\FV
@echo ROM Images location:  %WORKSPACE%\RomImages
@echo.
@echo The BIOS build has successfully completed!
@echo.
@REM

@goto BldEnd

:BldFail
cd %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%
@echo.
@echo The BIOS Build has failed!
@echo.
@exit /b 1

:BldEnd
@if %SILENT_MODE% EQU TRUE (
  @if exist EDK2.log del EDK2.log
  @if exist Prep.log if exist Build.log copy Prep.log+Build.log EDK2.log
)
@set BLDTIMESTAMP=%BLDTIMESTAMP% -%time%
@echo -- timestamp ------------------------
@echo   prep          %PREPTIMESTAMP%
@echo   bld           %BLDTIMESTAMP%
@echo   - build       %BUILDTIMESTAMP%
@echo   - postbuild   %POSTBUILDTIMESTAMP%
@echo -------------------------------------
cd %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%