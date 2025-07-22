@REM  @file
@REM
@REM  @copyright
@REM  INTEL CONFIDENTIAL
@REM  Copyright (C) 2022 Intel Corporation.
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
@REM  @par Specification Reference:
@REM

:: Usage: bld [/s] [/f <FEATURE_PCD_NAME> <FALSE or TRUE>] [/r]
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
@setlocal EnableDelayedExpansion
@set BLDTIMESTAMP=%time%

@if not defined PYTHON_COMMAND (
  set PYTHON_COMMAND=py -3
)

@set FLASHMAP_FDF=
@if %RESILIENCY_BUILD% EQU TRUE (
  if %WCL_BUILD% EQU TRUE (
    @set FLASHMAP_FDF=%WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Include\Fdf\FlashMapIncludeResWcl.fdf
  ) else (
    @set FLASHMAP_FDF=%WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Include\Fdf\FlashMapIncludeRes.fdf
  )
) else (
    @set FLASHMAP_FDF=%WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Include\Fdf\FlashMapInclude.fdf
@REM Multi-IBB support
@REM
  if %MULTI_IBB_BUILD% EQU TRUE (
    @set FLASHMAP_FDF=%WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Include\Fdf\FlashMapIncludeMultiIbb.fdf
  )
)

@if %IBBSIGN% EQU TRUE (
  @mkdir %WORKSPACE%\%BUILD_DIR%\FV
  @set ClientBios=%WORKSPACE%\%BUILD_DIR%\FV\ClientBios.fd
  goto IbbSignSetBiosPath
)

@REM
@REM Generating Flashmap FDF for Extended BIOS Region
@REM Note : the flashmap is being generated based on the default flashmap specificed by build option
@REM
@if %EXTENDEDREGION_BUILD% NEQ TRUE @goto NoExtendedBiosRegionGeneration
  @if %EMBEDDED_BUILD% EQU TRUE (
    @set EXTENDEDREGION_TEMPLATE_FDF=%WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Include\Fdf\FlashMapIncludeExtendedEmbedded.fdf.template
  ) else (
    @set EXTENDEDREGION_TEMPLATE_FDF=%WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Include\Fdf\FlashMapIncludeExtended.fdf.template
  )
@set EXTENDEDREGION_FLASHMAP_FDF=%WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Include\Fdf\FlashMapIncludeExtended_autogen.fdf
@%PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Tools\GenFlashmap\GenFlashmap.py ^
  -b %FLASHMAP_FDF% ^
  -t %EXTENDEDREGION_TEMPLATE_FDF% ^
  -o %EXTENDEDREGION_FLASHMAP_FDF% ^
  extended
@if %ERRORLEVEL% NEQ 0 (
  @echo ERROR: Failure in generating %EXTENDEDREGION_FLASHMAP_FDF%
  set SCRIPT_ERROR=1
  goto :BldFail
)
@set FLASHMAP_FDF=%EXTENDEDREGION_FLASHMAP_FDF%
:NoExtendedBiosRegionGeneration

@echo ***********************************
@echo.
@echo Parsing FlashMapInclude FDFs and checking if all offset and size requirements are met
@echo.
  @REM FSP build
  @set BB_CHECK=TRUE
  @if %ATOM_BUILD% EQU TRUE @set BB_CHECK=FALSE
  @if /I "%TARGET%" == "DEBUG" @set BB_CHECK=FALSE
  @REM Topswap for PTL is 8MB. Setting  build specific limit to 6.2MB after arch discussion
  @set TOPSWAP_LIMIT=0x00650000
  @if %BB_CHECK% EQU TRUE (
      @call %PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Tools\FvAlignment\FvAlignment.py --flashmap %FLASHMAP_FDF% --topswap_size %TOPSWAP_LIMIT% -cl all fv_alignment_check
  )
  @if %ERRORLEVEL% NEQ 0 goto :BldFail
@echo.
@echo PASS all flash map quick check successfully!
@echo ***********************************

@REM
@REM Definition for BtGACM Support.
@REM
@SET BTG_ACM_BASE_PATH=%WORKSPACE_BINARIES%\%PLATFORM_BIN_PACKAGE%\Binaries\BootGuard
@SET BTG_ACM_OUTPUT_PATH=%WORKSPACE%\%BUILD_DIR%\BootGuard
@SET BTG_ACM_SLOT_SIZE=0x48000

CALL %PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Tools\BtgAcmMisc\BtgAcmMiscScript.py ^
  Padding ^
  -I %BTG_ACM_BASE_PATH% ^
  -S %BTG_ACM_SLOT_SIZE% ^
  -O %BTG_ACM_OUTPUT_PATH%
@IF %ERRORLEVEL% NEQ 0 (
  @ECHO ERROR: Failed to do the padding operation on BtGACM
  GOTO :BldFail
)

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

:: Loop through arguments until all are processed
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

:: Start the FSP build process
@echo FSP_BINARY_BUILD=%FSP_BINARY_BUILD%
@REM
@echo Build FSP Binary
@REM
@if not defined FSP_BINARY_BUILD goto :SkipFspBinaryBuild
@if %FSP_BINARY_BUILD% EQU FALSE goto :SkipFspBinaryBuild
@set FSP_BUILD_PARAMETER=/d
@set FSP_TARGET=%TARGET%
@set FSP_PKG_NAME=PantherLakeFspPkg
@if /I "%TARGET%" == "RELEASE" (
  @if "%FSP_TEST_RELEASE%"=="TRUE" (
    set FSP_BUILD_PARAMETER=/tr
  ) else (
    set FSP_BUILD_PARAMETER=/r
  )
  @if "%FSP_PDB_RELEASE%"=="TRUE" (
    set FSP_BUILD_PARAMETER=/rp
  )
)
@REM
@REM When building resiliency image, always build Release FSP image
@REM
@if /I "%RESILIENCY_BUILD%" == "TRUE" (
  set FSP_TARGET=RELEASE
  set FSP_BUILD_PARAMETER=/r
)
@if /I "%NOTIMESTAMP%" == "1" (
  set FSP_BUILD_PARAMETER=%FSP_BUILD_PARAMETER% notimestamp
)
@set WORKSPACE_FSP_BIN=%WORKSPACE%\Intel

@echo %CD%
@rem
@rem  By Default Build for PTL
@rem

@set FspTargetOption=PantherLake


@if not "%FspTargetOption%" == "" (
  @echo FSP build target is %FspTargetOption%
) else (
  @echo FSP build target is not specified.
  @goto :BldFail
)

@if %FSP_SIGNED% EQU TRUE (
  set FSP_BUILD_PARAMETER=%FSP_BUILD_PARAMETER% fspsigned
)

@set FSPBINARYTIMESTAMP=%time%
@if %SILENT_MODE% EQU TRUE goto FspSilent

call %WORKSPACE_COMMON%\OneSiliconPkg\Fsp\BuildFsp.cmd PantherLake %FspTargetOption% %FSP_BUILD_PARAMETER%

@if %errorlevel% NEQ 0 (
  @goto FspBuildEnd
)
@goto FspBuildEnd

:FspSilent
call %WORKSPACE_COMMON%\OneSiliconPkg\Fsp\BuildFsp.cmd PantherLake %FspTargetOption% %FSP_BUILD_PARAMETER% 1>> %WORKSPACE%\Build.log 2>&1

:FspBuildEnd
@if %ERRORLEVEL% NEQ 0 (
  @echo !!! ERROR:FSP build Failed !!!
  @echo %WORKSPACE_COMMON%\OneSiliconPkg\Fsp\BuildFsp.cmd PantherLake %FspTargetOption% %FSP_BUILD_PARAMETER%
  set SCRIPT_ERROR=1
  goto :BldFail
) else (
  @echo FSP build has succeeded
  @echo FSP build has succeeded >> %WORKSPACE%\Build.log 2>&1
  @set FSPBINARYTIMESTAMP=%FSPBINARYTIMESTAMP% -%time%
)

@if %FSP_BINARY_BUILD_ONLY% EQU TRUE goto :BldEnd

:SkipFspBinaryBuild

@if %FSP_SIGNED% EQU TRUE (
  @if not exist %WORKSPACE_COMMON%\PantherLakeFspBinPkg\Fbm.bin (
    @echo !!! ERROR: FBM binary file is missing !!!
    @set SCRIPT_ERROR=1
    goto :BldFail
  )
)

@if %FSP_BINARY_BUILD_ONLY% EQU TRUE goto :SkipPatchFspBinFvsBaseAddress
del /f %WORKSPACE_SILICON%\%PLATFORM_FSP_BIN_PACKAGE%\Fsp_Rebased*.fd

cd %WORKSPACE%
@echo %CD%
@echo ********************* BLD.BAT *****************************
@echo WORKSPACE_BINARIES     = %WORKSPACE_BINARIES%
@echo WORKSPACE_PLATFORM     = %WORKSPACE_PLATFORM%
@echo WORKSPACE_SILICON      = %WORKSPACE_SILICON%
@echo PROJECT                = %PROJECT%
@echo PLATFORM_PACKAGE       = %PLATFORM_PACKAGE%
@echo PLATFORM_BOARD_PACKAGE = %PLATFORM_BOARD_PACKAGE%
@echo WORKSPACE_FSP_BIN      = %WORKSPACE_FSP_BIN%

@if exist %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\BoardPkgPcd.dsc attrib -r %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\BoardPkgPcd.dsc
@echo .

@REM Split Fsp.fd and generate FspTopAt4G.fd, then add padding data before this binary to FspTopAt4G.fd for alignment
@REM FspTopAt4G.fd contains Free space(at least 8KB) + Fsp_T.fd + Fsp_O.fd
@if %FSP_RESET% EQU TRUE (
  @echo call FspTopGen.py to generate FspTopAt4G.fd.
  @call %PYTHON_COMMAND% %WORKSPACE_COMMON%\%PLATFORM_BOARD_PACKAGE%\BoardSupport\Tools\FspTopGen\FspTopGen.py -SplitBin %EDK_TOOLS_PATH%\Source\Python\Split\Split.py
  @if %errorlevel% NEQ 0 (
    @echo !!! ERROR:generate FspTopAt4G.fd failed!!!
    set SCRIPT_ERROR=1
    goto :BldFail
  )
)

@REM Check if debug and release BIOS have different flashmap in one file
@call %PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Tools\StripFlashmap.py "%TARGET%" %FLASHMAP_FDF% %WORKSPACE%\FlashMapInclude_Temp.fdf
@if %FSP_SIGNED% EQU TRUE (
  @call %PYTHON_COMMAND% %WORKSPACE_CORE%\IntelFsp2Pkg\Tools\SplitFspBin.py split -f %WORKSPACE_FSP_BIN%\PantherLakeFspBinPkg\Fsp.fd -o %WORKSPACE_FSP_BIN%\PantherLakeFspBinPkg -n Fsp_Rebased.fd
) else (
  @call %PYTHON_COMMAND% %WORKSPACE_CORE_PLATFORM%\%PLATFORM_PACKAGE%\Tools\Fsp\RebaseFspBinBaseAddress.py %WORKSPACE%\FlashMapInclude_Temp.fdf %WORKSPACE_FSP_BIN%\PantherLakeFspBinPkg Fsp.fd 0x0
)
@if %FSPM_COMPRESSED% EQU TRUE (
  @echo FSP-M is compressed, Rebase FSP-M
  @call %PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Tools\RebaseFspmBinBaseAddress.py %WORKSPACE_FSP_BIN%\PantherLakeFspBinPkg Fsp_Rebased.fd %WORKSPACE_PLATFORM%\PantherLakeBoardPkg\BoardPkgPcdUpdate.dsc
)
@del %WORKSPACE_ROOT%\FlashMapInclude_Temp.fdf

@echo .
@if %ERRORLEVEL% NEQ 0 (
  @echo !!! ERROR:RebaseFspBinBaseAddress failed!!!
  set SCRIPT_ERROR=1
  goto :BldFail
)

@REM FSP binary size check
if defined FSPM_SIZE_LIMIT (
  @call %PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Tools\FvAlignment\FvAlignment.py --fsp_bin_pkg %WORKSPACE_FSP_BIN%\PantherLakeFspBinPkg --fspm_size_limit %FSPM_SIZE_LIMIT% -cl all fsp_size_check
)
@if %ERRORLEVEL% NEQ 0 goto :BldFail

cd %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%

copy /y /b %WORKSPACE_SILICON%\%PLATFORM_FSP_BIN_PACKAGE%\Fsp_Rebased_S.fd+%WORKSPACE_SILICON%\%PLATFORM_FSP_BIN_PACKAGE%\Fsp_Rebased_M.fd+%WORKSPACE_SILICON%\%PLATFORM_FSP_BIN_PACKAGE%\Fsp_Rebased_T.fd %WORKSPACE_SILICON%\%PLATFORM_FSP_BIN_PACKAGE%\Fsp_Rebased.fd
:SkipPatchFspBinFvsBaseAddress

@set MAX_CONCURRENT_THREADS=%NUMBER_OF_PROCESSORS%
@ for /f %%G in ('%PYTHON_COMMAND% %WORKSPACE_COMMON%\OneSiliconPkg\Tools\Containers\MaxConcurrentThreads.py') do @set MAX_CONCURRENT_THREADS=%%G

@echo Current Directory = %CD%
@echo ********************* BLD.BAT *****************************
@echo SILENT_MODE            = %SILENT_MODE%
@echo MAX_CONCURRENT_THREADS = %MAX_CONCURRENT_THREADS%
@echo REBUILD_MODE           = %REBUILD_MODE%
@echo EXT_BUILD_FLAGS        = %EXT_BUILD_FLAGS%

@set PPL_SETUP_FV_LIST=%BUILD_DIR%\FV\FvSetupData.fv
@set UPLBUILDTIMESTAMP=%time%
if "%UNIVERSAL_PAYLOAD%" NEQ "TRUE" goto SkipUefiPayload

@REM
@REM Universal Payload SpecRevision
@REM
@set UPL_SPEC_REVISION=0.9

if not defined CLANG_BIN (
  echo [Failed] UPL required to install clang compiler.
  echo   Please follow below step to install clang compiler
  echo   Step1. Download https://github.com/llvm/llvm-project/releases/download/llvmorg-12.0.1/LLVM-12.0.1-win64.exe
  echo   Step2. Install https://github.com/llvm/llvm-project/releases/download/llvmorg-12.0.1/LLVM-12.0.1-win64.exe
  goto BldFail
)

@REM
@REM add upl fv or extra fv to PPL_SETUP_FV_LIST
@REM
@set PPL_SETUP_FV_LIST= %PPL_SETUP_FV_LIST% ^
                       %BUILD_DIR%\FV\FvPplBdsUncompact.fv

@REM
@REM Set DXE NX MEMORY PROTECTION POLICY
@REM
@if %SPECIAL_POOL_BUILD% EQU TRUE (
  set PCD_DXE_NX_MEMORY_PROTECTION_POLICY=0x7BD4
) else (
  set PCD_DXE_NX_MEMORY_PROTECTION_POLICY=0x0
)

set UPL_SETTINGS= ^
  %PYTHON_COMMAND% %WORKSPACE_CORE%\UefiPayloadPkg\UniversalPayloadBuild.py ^
  -t %TOOL_CHAIN_TAG% ^
  -s %UPL_SPEC_REVISION% ^
  -b %PrepRELEASE% ^
  -D UNIVERSAL_PAYLOAD=TRUE ^
  -D DISABLE_RESET_SYSTEM=TRUE ^
  -D NETWORK_DRIVER_ENABLE=TRUE ^
  -D PS2_KEYBOARD_ENABLE=TRUE ^
  -D SERIAL_DRIVER_ENABLE=FALSE ^
  -D VARIABLE_SUPPORT=None ^
  -D SECURITY_STUB_ENABLE=FALSE ^
  -D CRYPTO_PROTOCOL_SUPPORT=TRUE ^
  -D CRYPTO_DRIVER_EXTERNAL_SUPPORT=TRUE ^
  -D NVME_ENABLE=FALSE ^
  -D MEMORY_TEST=GENERIC ^
  -D PERFORMANCE_MEASUREMENT_ENABLE=TRUE ^
  -D MULTIPLE_DEBUG_PORT_SUPPORT=TRUE ^
  -D CAPSULE_SUPPORT=TRUE ^
  -p gEfiMdeModulePkgTokenSpaceGuid.PcdPs2KbdExtendedVerification=FALSE ^
  -p gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport=FALSE ^
  -p gEfiMdeModulePkgTokenSpaceGuid.PcdPciDisableBusEnumeration=FALSE ^
  -p gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport=FALSE ^
  -p gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserSubtitleTextColor=0x0 ^
  -p gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserGrayOutTextStatement=TRUE ^
  -p gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserFieldTextColor=0x01 ^
  -p gUefiCpuPkgTokenSpaceGuid.PcdCpuApLoopMode=0x2 ^
  -p gEfiMdeModulePkgTokenSpaceGuid.PcdConOutUgaSupport=TRUE ^
  -p gEfiMdeModulePkgTokenSpaceGuid.PcdSupportUpdateCapsuleReset=TRUE ^
  -p gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosEntryPointProvideMethod=0x01 ^
  -p gPcAtChipsetPkgTokenSpaceGuid.PcdMinimalValidYear=%BUILD_DATE:~0,4% ^
  -p gUefiCpuPkgTokenSpaceGuid.PcdCpuInitIpiDelayInMicroSeconds=100 ^
  -p gEfiMdeModulePkgTokenSpaceGuid.PcdDxeNxMemoryProtectionPolicy=%PCD_DXE_NX_MEMORY_PROTECTION_POLICY%

echo %UPL_SETTINGS%
@if %SILENT_MODE% EQU TRUE (
  call %UPL_SETTINGS% 1>>%WORKSPACE%\Build.log 2>&1
) else (
  call %UPL_SETTINGS%
)

if %ERRORLEVEL% NEQ 0 goto BldFail
set UPL_BUILD_OPTION_PCD=
setlocal enableextensions enabledelayedexpansion
@for /f %%i in (
  '@%PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Features\Upl\Tools\GetUplVerFromBin.py ^
  --UplBinFile %WORKSPACE%\Build\UefiPayloadPkgX64\UniversalPayload.elf ^
') do (
 set UPL_BUILD_OPTION_PCD=!UPL_BUILD_OPTION_PCD! --pcd %%i
)
set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% %UPL_BUILD_OPTION_PCD%
@set UPLBUILDTIMESTAMP=%UPLBUILDTIMESTAMP% -%time%
:SkipUefiPayload
@if %SILENT_MODE% EQU TRUE goto BldSilent
@set BUILDTIMESTAMP=%time%
set EXT_BUILD_FLAGS=%EXT_BUILD_FLAGS%
@REM  -y %WORKSPACE%\Build\%PLATFORM_BOARD_PACKAGE%\BoardPkgReport.log
call build  %SI_BUILD_OPTION_PCD% %BUILD_OPTION_PCD% -n %MAX_CONCURRENT_THREADS% %REBUILD_MODE% %EXT_BUILD_FLAGS%

@echo %CD%
@echo ********************* BLD.BAT *****************************
@echo ERRORLEVEL=%ERRORLEVEL%

@if %ERRORLEVEL% NEQ 0 goto BldFail
@set BUILDTIMESTAMP=%BUILDTIMESTAMP% -%time%

if "%UNIVERSAL_PAYLOAD%" NEQ "TRUE" goto SkipPcdCheck
call %PYTHON_COMMAND% %CD%\Features\Upl\Tools\PcdCheck.py
@if %ERRORLEVEL% NEQ 0 goto BldFail
:SkipPcdCheck
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
call build  %SI_BUILD_OPTION_PCD% %BUILD_OPTION_PCD% -n %MAX_CONCURRENT_THREADS% %REBUILD_MODE% %EXT_BUILD_FLAGS% 1>>%WORKSPACE%\Build.log 2>&1
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

:IbbSignSetBiosPath
@if %IBBSIGN% EQU TRUE (
  @if "%1" == "" goto BldFail
  @if not exist %1 goto BldFail
  copy /y /b %1 %ClientBios%
  echo ClientBios %ClientBios% File size is %~z1.
  call %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\postbuild.bat  %ClientBios%
  @rmdir /s /q %WORKSPACE%\Conf
  @rmdir /s /q %WORKSPACE%\Build
  @del %WORKSPACE%\RomImages\*.txt
  goto IbbsignBldSuccess
)

:BldSuccess


:IbbsignBldSuccess

cd %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%
@echo.
@echo Build_pkg:                 %BUILD_DIR%
@echo Fsp_Bin_pkg:               %WORKSPACE_FSP_BIN%
@call %PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\FvReportGen\FirmVolumeGen.py --fsp_bin_pkg %WORKSPACE%\Build\%FSP_PKG_NAME%\%FSP_TARGET%_%TOOL_CHAIN_TAG%\FV --build_pkg %WORKSPACE%\%BUILD_DIR%\FV  --File_path %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\FvReportGen
copy /y %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Firm_Volume_Sizes.xlsx %WORKSPACE%\RomImages
if exist %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Firm_Volume_Sizes.xlsx del %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Firm_Volume_Sizes.xlsx
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
@echo ----------------
@findstr "ACTIVE_PLATFORM" %WORKSPACE%\Conf\target.txt
@echo ACTIVE FLASH MAP FDF = %FLASHMAP_FDF%
@echo ----------------
@echo.
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
@echo   - fsp binary  %FSPBINARYTIMESTAMP%
@if "%UNIVERSAL_PAYLOAD%" EQU "TRUE" (
@echo   - upl binary  %UPLBUILDTIMESTAMP%
)
@if %FSP_BINARY_BUILD_ONLY% EQU FALSE (
@echo   - build       %BUILDTIMESTAMP%
@echo   - postbuild   %POSTBUILDTIMESTAMP%
)
@echo -------------------------------------
cd %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%
