@REM @file
@REM
@REM  @copyright
@REM  INTEL CONFIDENTIAL
@REM  Copyright (C) 2018 Intel Corporation.
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

@echo ON
@set PREPTIMESTAMP=%time%
@REM TODO move these definitions to SetEdk2Env.bat and rename to WORKSPACE_*
@echo Define platform specific environment variables.
@set PLATFORM_PACKAGE=MinPlatformPkg
@set PLATFORM_BOARD_PACKAGE=PantherLakeBoardPkg
@set PLATFORM_BIN_PACKAGE=PantherLakeBinPkg
@set PLATFORM_FULL_PACKAGE=PantherLakePlatSamplePkg
@REM
@REM The following environment variables is used for Micorocode update.
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
@set FSP_PDB_RELEASE=FALSE
@set PrepRELEASE=DEBUG
@set SECURE_BOOT_ENABLE=FALSE
@set STARTUP_ACM_ENABLE=FALSE
@set UNIVERSAL_PAYLOAD=FALSE
@REM BOOT_STAGE is set to stage 6 by default
@set BOOT_STAGE=6
@rem
@rem BUILD_OPTION_PCD is used in build. It will be cleared here.
@rem EXT_BUILD_FLAGS is used by prep.bat and bld.bat. It will be cleared in the
@rem beginning of prep.bat and not cleared after bld.bat. It is not supposed
@rem to be modified by bld.bat. This will ensure bld.bat can have incremental
@rem build with all correct flags intact.
@rem
@set "SI_BUILD_OPTION_PCD="
@set "FSP_BUILD_OPTION_PCD="
@set "BUILD_OPTION_PCD="

@REM
@REM Set Build env variable default value
@REM
@set BUILD=X
@set ATOM_BUILD=FALSE
@set IBBSIGN=FALSE
@set ROM_FILENAME_SPECIAL_BUILD_TYPE=
@set PTL_BUILD=TRUE
@set WCL_BUILD=FALSE
@set MULTI_IBB_BUILD=FALSE
@set FSP_BINARY_BUILD=TRUE
@set FSP_BINARY_BUILD_ONLY=FALSE
@set PERFORMANCE_BUILD=FALSE
@set EMBEDDED_BUILD=FALSE
@set FSP_VALIDATION_BUILD=FALSE
@set FSPM_COMPRESSED=TRUE
@set RESILIENCY_BUILD=FALSE
@set RPMC_BUILD=FALSE
@set EXTENDEDREGION_BUILD=FALSE
@set SPECIAL_POOL_BUILD=FALSE

@set EXT_BUILD_FLAGS=
@set NOTIMESTAMP=
@set TOOL_CHAIN_TAG=
@set FV_SPARE_SPACE_THRESHOLD=

@REM
@REM Set 64-bit PEI for FSP as default build.
@REM
@set FSP_ARCH=X64
@set FSP64_BUILD=TRUE
@set FSP_RESET=TRUE
@set FSP_MODE=Dispatch
@set FSP_SIGNED=TRUE
@set SYMBOL_PREFIX=
@set BUILD_OPTION_PCD=--pcd gFspWrapperFeaturePkgTokenSpaceGuid.PcdFspWrapperResetVectorInFsp=TRUE

@set BUILD_DATE=

@ for /f %%G in ('powershell -Command "(Get-Date).Year"') do set BUILD_DATE=%%G
@set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% --pcd gPcAtChipsetPkgTokenSpaceGuid.PcdMinimalValidYear=%BUILD_DATE%
:CmdLineParse

@if "" == "%1" (
  goto Continue
) else if /i "d" == "%1" (
  echo "d"
  set PrepRELEASE=DEBUG
) else if /i "r" == "%1" (
  echo "r"
  set PrepRELEASE=RELEASE
) else if /i "tr" == "%1" (
  echo "tr"
  set PrepRELEASE=TEST_RELEASE
) else if /i "rp" == "%1" (
  echo "rp"
  set PrepRELEASE=RELEASE
  echo gPlatformModuleTokenSpaceGuid.PcdSymbolInReleaseEnable ^|TRUE
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% --pcd gPlatformModuleTokenSpaceGuid.PcdSymbolInReleaseEnable=TRUE
  set FSP_PDB_RELEASE=TRUE
) else if /i "s" == "%1" (
  set SILENT_MODE=TRUE
) else if /i "clang" == "%1" (
  echo "clang"
  set TOOL_CHAIN_TAG=CLANGPDB
) else if /i "non_upl" == "%1" (
  @set UNIVERSAL_PAYLOAD=FALSE
  set ROM_FILENAME_SPECIAL_BUILD_TYPE=_Non_UPL
) else if /i "fsp32" == "%1" (
  echo "fsp32"
  set FSP_ARCH=IA32
  set FSP64_BUILD=FALSE
  set FSP_RESET=FALSE
  set SYMBOL_PREFIX=_
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
--pcd gFspWrapperFeaturePkgTokenSpaceGuid.PcdFspWrapperResetVectorInFsp=FALSE
) else if /i "fsp64" == "%1" (
  echo "fsp64"
  set FSP_ARCH=X64
  set FSP64_BUILD=TRUE
  set FSP_RESET=TRUE
  set SYMBOL_PREFIX=
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
--pcd gFspWrapperFeaturePkgTokenSpaceGuid.PcdFspWrapperResetVectorInFsp=TRUE
) else if /i "fspsigned" == "%1" (
  echo "fspsigned"
  set FSP_ARCH=X64
  set FSP64_BUILD=TRUE
  set FSP_RESET=TRUE
  set FSP_SIGNED=TRUE
  set ROM_FILENAME_SPECIAL_BUILD_TYPE=%ROM_FILENAME_SPECIAL_BUILD_TYPE%_FSPSIGNED
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
--pcd gFspWrapperFeaturePkgTokenSpaceGuid.PcdFspWrapperResetVectorInFsp=TRUE ^
--pcd gIntelFsp2WrapperTokenSpaceGuid.PcdFspMeasurementConfig=0 ^
--pcd gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection=0
  set SI_BUILD_OPTION_PCD=%SI_BUILD_OPTION_PCD% ^
--pcd gSiPkgTokenSpaceGuid.PcdSignedFspEnable=TRUE
) else if /i "fspunsigned" == "%1" (
  echo "fspunsigned"
  set FSP_SIGNED=FALSE
  set ROM_FILENAME_SPECIAL_BUILD_TYPE=_FSPUNSIGNED
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD%
--pcd gIntelFsp2WrapperTokenSpaceGuid.PcdFspMeasurementConfig=0x8000000F \
--pcd gMinPlatformPkgTokenSpaceGuid.PcdFspDispatchModeUseFspPeiMain=TRUE
  set SI_BUILD_OPTION_PCD=%SI_BUILD_OPTION_PCD% ^
--pcd gSiPkgTokenSpaceGuid.PcdSignedFspEnable=FALSE
) else if /i "embedded" == "%1" (
  set EMBEDDED_BUILD=TRUE
  set ROM_FILENAME_SPECIAL_BUILD_TYPE=_Embedded
  set SI_BUILD_OPTION_PCD=%SI_BUILD_OPTION_PCD% ^
--pcd gSiPkgTokenSpaceGuid.PcdEmbeddedEnable=0x1
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD%
  set FSP_BUILD_OPTION_PCD=%FSP_BUILD_OPTION_PCD%
) else if /i "fspb" == "%1" (
  set SI_BUILD_OPTION_PCD=%SI_BUILD_OPTION_PCD% ^
--pcd gSiPkgTokenSpaceGuid.PcdFspBinaryEnable=TRUE
  set FSP_BINARY_BUILD=TRUE
  set FSP_BINARY_BUILD_ONLY=TRUE
) else if /i "fspw" == "%1" (
  set SI_BUILD_OPTION_PCD=%SI_BUILD_OPTION_PCD% ^
--pcd gSiPkgTokenSpaceGuid.PcdFspBinaryEnable=FALSE
  set FSP_BINARY_BUILD=FALSE
) else if /i "fspnoapi" == "%1" (
  set SI_BUILD_OPTION_PCD=%SI_BUILD_OPTION_PCD%
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
--pcd gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection=0
) else if /i "fspapi" == "%1" (
  set FSP_MODE=API
  set FSP_SIGNED=FALSE
  set SI_BUILD_OPTION_PCD=%SI_BUILD_OPTION_PCD%
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
--pcd gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection=1 \
--pcd gIntelFsp2WrapperTokenSpaceGuid.PcdFspMeasurementConfig=0x8000000F
set SI_BUILD_OPTION_PCD=%SI_BUILD_OPTION_PCD%
--pcd gSiPkgTokenSpaceGuid.PcdSignedFspEnable=FALSE
) else if /i "fspmuncompressed" == "%1" (
  set FSPM_COMPRESSED=FALSE
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
--pcd gSiPkgTokenSpaceGuid.PcdEnableFspmCompression=FALSE ^
--pcd gMinPlatformPkgTokenSpaceGuid.PcdFspDispatchModeUseFspPeiMain=TRUE
set FSP_BUILD_OPTION_PCD=%FSP_BUILD_OPTION_PCD% ^
--pcd gSiPkgTokenSpaceGuid.PcdSecondaryDataStackSize=0x0
) else if /i "FspV" == "%1" (
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
--pcd gSiPkgTokenSpaceGuid.PcdFspVEnable=TRUE
  set FSP_BUILD_OPTION_PCD=%FSP_BUILD_OPTION_PCD% ^
--pcd gSiPkgTokenSpaceGuid.PcdFspVEnable=TRUE
  set FSP_VALIDATION_BUILD=TRUE
  set ROM_FILENAME_SPECIAL_BUILD_TYPE=_FSPV
) else if /i "perf" == "%1" (
  set PERFORMANCE_BUILD=TRUE
  set BUILD=P
  set ROM_FILENAME_SPECIAL_BUILD_TYPE=_Performance
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
--pcd gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable=TRUE ^
--pcd gSiPkgTokenSpaceGuid.PcdBootGuardPerformanceEnable=TRUE
  set FSP_BUILD_OPTION_PCD=%FSP_BUILD_OPTION_PCD% ^
--pcd gPantherLakeFspPkgTokenSpaceGuid.PcdFspPerformanceEnable=TRUE
) else if /i "res" == "%1" (
  set RESILIENCY_BUILD=TRUE
  set ROM_FILENAME_SPECIAL_BUILD_TYPE=%ROM_FILENAME_SPECIAL_BUILD_TYPE%_Resiliency
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
--pcd gCapsuleFeaturePkgTokenSpaceGuid.PcdBiosResiliencyEnable=TRUE ^
--pcd gCapsuleFeaturePkgTokenSpaceGuid.PcdMeResiliencyEnable=TRUE
) else if /i "exclfw" == "%1" (
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
--pcd gCommonBinPkgTokenSpaceGuid.PcdRaidDriverEfiEnable=FALSE ^
--pcd gPlatformModuleTokenSpaceGuid.PcdNvmeEnable=FALSE ^
--pcd gCommonBinPkgTokenSpaceGuid.PcdRsteDriverEfiEnable=FALSE ^
--pcd gPantherLakeBinPkgTokenSpaceGuid.PcdIntelGopBinEnable=FALSE ^
--pcd gPantherLakeBinPkgTokenSpaceGuid.PcdStartupAcmBinEnable=FALSE ^
--pcd gPantherLakeBinPkgTokenSpaceGuid.PcdMicrocodeBinEnable=FALSE ^
--pcd gPantherLakeBinPkgTokenSpaceGuid.PcdBiosGuardBinEnable=FALSE ^
--pcd gPantherLakeBinPkgTokenSpaceGuid.PcdTdxBinEnable=FALSE
  set SI_BUILD_OPTION_PCD=%SI_BUILD_OPTION_PCD% ^
--pcd gSiPkgTokenSpaceGuid.PcdBiosGuardEnable=FALSE
) else if /i "specialpool" == "%1" (
  set SPECIAL_POOL_BUILD=TRUE
  set ROM_FILENAME_SPECIAL_BUILD_TYPE=_SpecialPool
  set SI_BUILD_OPTION_PCD=%SI_BUILD_OPTION_PCD% ^
--pcd gSiPkgTokenSpaceGuid.PcdSpecialPoolEnable=TRUE
  set FSP_BUILD_OPTION_PCD=%FSP_BUILD_OPTION_PCD% ^
--pcd gSiPkgTokenSpaceGuid.PcdSpecialPoolEnable=TRUE
) else if /i "phypres" == "%1" (
  @rem Forces physical presence settings. User should not use this flag for production platforms
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
--pcd gPlatformModuleTokenSpaceGuid.PcdPhysicalPresent=TRUE
  @echo *************PHYSICAL PRESENCE PCD SET*****************************
) else if /i "rpmc" == "%1" (
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
--pcd gProtectedVariableFeaturePkgTokenSpaceGuid.PcdProtectedVariableEnable=TRUE
  set RPMC_BUILD=TRUE
  set ROM_FILENAME_SPECIAL_BUILD_TYPE=_RPMC
) else if /i "chksize" == "%1" (
  if "" == "%2" goto InvalidThreshold
  set FV_SPARE_SPACE_THRESHOLD=%2
  SHIFT
) else if /i "stage1" == "%1" (
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% --pcd gMinPlatformPkgTokenSpaceGuid.PcdBootStage=1
  set BOOT_STAGE=1
) else if /i "stage2" == "%1" (
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% --pcd gMinPlatformPkgTokenSpaceGuid.PcdBootStage=2
  set BOOT_STAGE=2
) else if /i "stage3" == "%1" (
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% --pcd gMinPlatformPkgTokenSpaceGuid.PcdBootStage=3
  set BOOT_STAGE=3
) else if /i "stage4" == "%1" (
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% --pcd gMinPlatformPkgTokenSpaceGuid.PcdBootStage=4
  set BOOT_STAGE=4
) else if /i "stage5" == "%1" (
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% --pcd gMinPlatformPkgTokenSpaceGuid.PcdBootStage=5
  set BOOT_STAGE=5
) else if /i "stage6" == "%1" (
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% --pcd gMinPlatformPkgTokenSpaceGuid.PcdBootStage=6
  set BOOT_STAGE=6
) else if /i "extended" == "%1" (
  set EXTENDEDREGION_BUILD=TRUE
  set BIOS_SIZE_OPTION=-DBIOS_SIZE_OPTION=SIZE_170
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
--pcd gPlatformModuleTokenSpaceGuid.PcdExtendedBiosRegionSupport=TRUE ^
--pcd gCapsuleFeaturePkgTokenSpaceGuid.PcdBiosExtenedRegionEnable=TRUE
  set SI_BUILD_OPTION_PCD=%SI_BUILD_OPTION_PCD% ^
--pcd gSiPkgTokenSpaceGuid.PcdExtendedBiosRegionSupport=TRUE
  set ROM_FILENAME_SPECIAL_BUILD_TYPE=%ROM_FILENAME_SPECIAL_BUILD_TYPE%_EXTEND
) else if /i "help" == "%1" (
  goto PrepHelp
) else if /i "notimestamp" == "%1" (
  set NOTIMESTAMP=1
) else if /i "ibbsign" == "%1" (
  set IBBSIGN=TRUE
) else if /i "whp" == "%1" (
  echo "Whp Build"
  set ROM_FILENAME_SPECIAL_BUILD_TYPE=_Whp
  @if /i "r" == "%2" (
    echo "Whp Release Build"
    set PrepRELEASE=RELEASE
    echo gSiPkgTokenSpaceGuid.PcdWhPSupport ^|TRUE
    set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
    --pcd gSiPkgTokenSpaceGuid.PcdWhPSupport=TRUE
  ) else (
    set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% ^
    --pcd gSiPkgTokenSpaceGuid.PcdWhPSupport=TRUE
  )
) else (
  echo Invalid input arguments: %1
  echo.
  goto PrepHelp
)
SHIFT
goto CmdLineParse

:Continue

set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% --pcd gBoardModuleTokenSpaceGuid.PcdUplEnable=%UNIVERSAL_PAYLOAD%
set EXT_BUILD_FLAGS=%EXT_BUILD_FLAGS% -D FSP_ARCH=%FSP_ARCH% -D FSP64_BUILD=%FSP64_BUILD%

@if not defined FV_SPARE_SPACE_THRESHOLD goto :SkipThreshold
@set /A THRESHOLD=%FV_SPARE_SPACE_THRESHOLD%
@if not defined THRESHOLD goto:InvalidThreshold
@if %THRESHOLD% GTR 0 (
  set EXT_BUILD_FLAGS=%EXT_BUILD_FLAGS% -D FV_SPARE_SPACE_THRESHOLD=%THRESHOLD%
  set THRESHOLD=
  goto :SkipThreshold
)

:InvalidThreshold
@echo Invalid threshold of chksize: "%FV_SPARE_SPACE_THRESHOLD%"
@echo.
@goto PrepHelp

:SkipThreshold
@echo TIP_DIR             =  %TIP_DIR%
@echo PrepRELEASE         =  %PrepRELEASE%
@echo FSP_PDB_RELEASE     =  %FSP_PDB_RELEASE%
@echo SILENT_MODE         =  %SILENT_MODE%
@echo Current Directory   =  %CD%
@echo WORKSPACE           =  %WORKSPACE%
@echo WORKSPACE_CORE      =  %WORKSPACE_CORE%
@echo WORKSPACE_COMMON    =  %WORKSPACE_COMMON%
@echo WORKSPACE_PLATFORM  =  %WORKSPACE_PLATFORM%
@echo WORKSPACE_SILICON   =  %WORKSPACE_SILICON%
@echo WORKSPACE_BINARIES  =  %WORKSPACE_BINARIES%

goto SkipHelp

:PrepHelp
cd %TIP_DIR%
@echo Preparation for BIOS build.
@echo.
@echo prep [ptlp][non_upl][r][rp][s][clang][fsp32][fsp64][tr][perf][notimestamp][whp][help]
@echo.
@echo   r         To do release build. Default is debug build. See note 1
@echo   rp        To do release build with Symbols - For source level debugging. See note 1
@echo   s         To build in silent mode. See note 1
@echo   clang     To build using CLANGPDB tool chain.
@echo   fsp32     To build using 32-bit PEI for FSP.
@echo   fsp64     To build using 64-bit PEI for FSP.
@echo   ptlp         To do build ptlp debug build (this is same as default prep )
@echo   notimestamp To eliminate the effect of timestamp
@echo   fspb      To build *Only* FSP Binary (Default is Debug build, adding r for Release build). See note 1
@echo   fspw      To build *Only* FSP Wrapper         (Default is Debug build, adding r for Release build). See note 1
@echo   fspnoapi  To build FSP Wrapper Dispatch Mode  (Default is Debug build, adding r for Release build). See note 1
@echo   fspapi    To build FSP Wrapper API Mode       (Default is Debug build, adding r for Release build). See note 1
@echo   res       To set gCapsuleFeaturePkgTokenSpaceGuid.PcdBiosResiliencyEnablee^|TRUE
@echo                    gCapsuleFeaturePkgTokenSpaceGuid.PcdMeResiliencyEnable^|TRUE. (Default is Debug build, adding r for Release build). See note 1
@echo   perf      To set gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable^|TRUE. See note 1
@echo   non_upl   To build Non-Upl
@echo   exclfw    To exclude Non-BIOS payloads. See note 1
@echo   specialpool To enable gSiPkgTokenSpaceGuid.PcdSpecialPoolEnable
@echo   chksize=threshold To enable FV_SPARE_SPACE_THRESHOLD check.
@echo             BaseTools will check every FV with the threshold.
@echo             Build tool will report error message to say there is no enough spare space if check fail.
@echo   extended  To enable Extended BIOS Region so a single BIOS image larger than 16MB in size is built.
@echo   ibbsign   To Re-Sign the BIOS Binaries
@echo.
@echo   rpmc  To enable gProtectedVariableFeaturePkgTokenSpaceGuid.PcdProtectedVariableEnable
@echo 1) Re-running prep without these arguments cannot be used for
@echo    incremental build. Hence, these inputs must be supplied each time
@echo    prep are desired to be re-run.
@echo   stage1      To set gMinPlatformPkgTokenSpaceGuid.PcdBootStage^|1. Stage 1 build
@echo   stage2      To set gMinPlatformPkgTokenSpaceGuid.PcdBootStage^|2. Stage 2 build
@echo   stage3      To set gMinPlatformPkgTokenSpaceGuid.PcdBootStage^|3. Stage 3 build
@echo   stage4      To set gMinPlatformPkgTokenSpaceGuid.PcdBootStage^|4. Stage 4 build
@echo   stage5      To set gMinPlatformPkgTokenSpaceGuid.PcdBootStage^|5. Stage 5 build
@echo   stage6      To set gMinPlatformPkgTokenSpaceGuid.PcdBootStage^|6. Stage 6 build
@echo   whp         To set gSiPkgTokenSpaceGuid.PcdWhPSupport TRUE
goto PrepDone

:SkipHelp


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
@echo WORKSPACE_BINARIES  =  %WORKSPACE_BINARIES%
@echo PrepRelease         =  %PrepRelease%
@echo UplBuild            =  %UNIVERSAL_PAYLOAD%

@if %IBBSIGN% EQU TRUE goto BldIbbSign
@if %SILENT_MODE% EQU TRUE goto BldSilent

call prebuild.bat %PrepRelease%
goto PrePrepDone

:BldSilent
cd %TIP_DIR%
call prebuild.bat %PrepRelease% 1>>%WORKSPACE%\Prep.log 2>&1

:PrePrepDone
@If %SCRIPT_ERROR% EQU 1 goto PrepFail
@goto PrepDone

:PrepFail
@echo.
@echo !! The EDKII BIOS build has failed in prep!
@echo.
@exit /b 1

:BldIbbSign
@if %IBBSIGN% EQU TRUE (
  call prebuild.bat
)

:PrepDone
@echo.
@echo Bios Prep succeeded
@echo.
@set PREPTIMESTAMP=%PREPTIMESTAMP% -%time%
