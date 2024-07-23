@REM @file
@REM
@REM  @copyright
@REM  INTEL CONFIDENTIAL
@REM  Copyright (C) 2016 Intel Corporation.
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
set PLATFORM_SI_PACKAGE=OneSiliconPkg
set FSP_PKG_NAME=PantherLakeFspPkg
set FSP_BIN_PKG_NAME=PantherLakeFspBinPkg
@if not defined PYTHON_COMMAND (
  set PYTHON_COMMAND=py -3
)

@REM
@REM Parse input parameters
@REM
@set SYMBOL_PREFIX=
@set FspTargetOption=
@set BuildTypeFSP=
@set CatalogOption=
@set BuildHeaderOnly=FALSE
@set OUT_DIR=%WORKSPACE%\Build
:ArgParse
if /I "%1" == "" (
  goto Continue
) else if /I "%1" == "/h" (
  goto Usage
) else if /I "%1" == "/?" (
  goto Usage
) else if /I "%1" == "/clean" (
  goto Clean
) else if /I "%1" == "/r" (
  @set BuildTypeFSP=/r
) else if /I "%1" == "/tr" (
  @set BuildTypeFSP=/tr
) else if /I "%1" == "/rp" (
  @set BuildTypeFSP=/rp
) else if /I "%1" == "/d" (
  @set BuildTypeFSP=/d
) else if /I "%1" == "/catalog" (
  @set CatalogOption=/catalog
) else if /I "%1" == "/header" (
  @set BuildHeaderOnly=TRUE
) else if /I "%1" == "fsp64" (
    echo "Build fsp64"
    @set FSP_ARCH=X64
    @set FSP64_BUILD=TRUE
    set EXT_BUILD_FLAGS=%EXT_BUILD_FLAGS% -D FSP_ARCH=X64 -D FSP64_BUILD=TRUE
)
@shift
goto ArgParse

:Continue
@if not defined FSP_ARCH (
  @echo FSP_ARCH is still not defined, set to X64 as default
  set FSP_ARCH=X64
  set FSP64_BUILD=TRUE
  set EXT_BUILD_FLAGS=%EXT_BUILD_FLAGS% -D FSP_ARCH=X64 -D FSP64_BUILD=TRUE
)

@REM
@REM If any target option is not specified, PantherLake build by default
@REM
@if /I "%FspTargetOption%" == ""  set FspTargetOption=PantherLake

@if not exist %WORKSPACE%\Conf mkdir %WORKSPACE%\Conf
set GET_TIME_COMMAND=%PYTHON_COMMAND% %WORKSPACE_CORE%\BaseTools\Scripts\GetUtcDateTime.py
@if /I "%NOTIMESTAMP%" == "1" (
   echo Set time to zero.
   set YYYY=0
   set MMDD=0
   set HHmm=0
) else (
   echo Get UTC time as build time.
   for /f %%a in ('%GET_TIME_COMMAND% --year') do set YYYY=%%a
   for /f %%b in ('%GET_TIME_COMMAND% --date') do set MMDD=%%b
   for /f %%c in ('%GET_TIME_COMMAND% --time') do set HHmm=%%c
)

echo %YYYY%
echo %MMDD%
echo %HHmm%

@echo off

pushd %WORKSPACE_SILICON%\%FSP_PKG_NAME%

@REM Default PantherLake build
@echo Target CPU is %FspTargetOption%. Build on %FSP_PKG_NAME%



popd

if /I "%CatalogOption%" == "/catalog"  (
  %PYTHON_COMMAND% --version
  if not %ERRORLEVEL% == 0 (
    echo.
    echo !!! ERROR !!! Catalog feature requirement issue: PYTHON_COMMAND is not defined and py -3 is invalid.
    echo.
    goto :PreBuildFail
  ) else (
    call %PYTHON_COMMAND% -c "import sys;print(sys.executable.split('python.exe')[0])" > Output
    set /p PYTHON_DIR= < Output
    del Output
  )

  @if exist %WORKSPACE_COMMON%\%PLATFORM_SI_PACKAGE%\Tools\Catalog\CatalogEncoder.py (
    set CATALOG_ENCODER=%WORKSPACE_COMMON%\%PLATFORM_SI_PACKAGE%\Tools\Catalog\CatalogEncoder.py
  ) else (
    echo.
    echo !!! ERROR !!! Catalog feature requirement issue: CatalogEncoder.py is not found.
    echo.
    goto :PreBuildFail
  )
)

if /I "%BuildTypeFSP%"=="/r" goto ReleaseBuild
if /I "%BuildTypeFSP%"=="/tr" goto ReleaseTypeTest
if /I "%BuildTypeFSP%"=="/rp" goto ReleaseBuildPdb
if /I "%BuildTypeFSP%"=="/d" goto DebugBuild
if /I "%BuildTypeFSP%"=="" (
  goto DebugBuild
) else (
  echo.
  echo  ERROR: %BuildTypeFSP% is not valid parameter.
  goto Usage
)


:Clean
echo Build Clean from FspPkg Start
if exist %OUT_DIR% rmdir %OUT_DIR% /s /q
if exist %WORKSPACE%\Conf  rmdir %WORKSPACE%\Conf  /s /q
if exist *.log  del *.log /q /f
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\*.fd del %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\*.fd
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\*.bsf del %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\*.bsf
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\FirmwareVersionInfo.h del %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\FirmwareVersionInfo.h
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\FspProducerDataHeader.h del %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\FspProducerDataHeader.h
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\Fsp*Upd.h del %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\Fsp*Upd.h
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\HobUsageDataHob.h del %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\HobUsageDataHob.h
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\MemInfoHob.h del %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\MemInfoHob.h
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\SmbiosCacheInfoHob.h del %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\SmbiosCacheInfoHob.h
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\SmbiosProcessorInfoHob.h del %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\SmbiosProcessorInfoHob.h
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\FspPkgPcdShare.dsc del %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\FspPkgPcdShare.dsc
if exist %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\Fsp*Upd.h del %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\Fsp*Upd.h
if exist %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\MemInfoHob.h del %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\MemInfoHob.h
set WORKSPACE=
set EDK_TOOLS_PATH=
echo Build Clean from FspPkg End
goto End


:ReleaseTypeTest
set  BD_TARGET=RELEASE
set  BD_MACRO=-D CFG_OUTDIR=%OUT_DIR% -D TARGET=%BD_TARGET% %EXT_BUILD_FLAGS%
set  BD_ARGS=-p %WORKSPACE_SILICON%\%FSP_PKG_NAME%\%FSP_PKG_NAME%.dsc -b RELEASE %BD_MACRO% -a %FSP_ARCH% -a X64 -n %NUMBER_OF_PROCESSORS% -t %TOOL_CHAIN_TAG% -y ReportRelease.log -Y PCD -Y LIBRARY %VAR_BUILD_FLAGS%
set  FSP_BUILD_TYPE=0x0001
set  FSP_RELEASE_TYPE=0x0000
goto Build

:ReleaseBuildPdb
set  FSP_BUILD_OPTION_PCD=%FSP_BUILD_OPTION_PCD% --pcd gPantherLakeFspPkgTokenSpaceGuid.PcdSymbolInReleaseEnable=TRUE
set  BD_TARGET=RELEASE
set  BD_MACRO=-D CFG_OUTDIR=%OUT_DIR% -D TARGET=%BD_TARGET% %EXT_BUILD_FLAGS%
set  BD_ARGS=-p %WORKSPACE_SILICON%\%FSP_PKG_NAME%\%FSP_PKG_NAME%.dsc -b RELEASE %BD_MACRO% -a %FSP_ARCH% -a X64 -n %NUMBER_OF_PROCESSORS% -t %TOOL_CHAIN_TAG% -y ReportRelease.log -Y PCD -Y LIBRARY %VAR_BUILD_FLAGS%
set  FSP_BUILD_TYPE=0x0001
set  FSP_RELEASE_TYPE=0x0002
goto Build
:ReleaseBuild
set  BD_TARGET=RELEASE
set  BD_MACRO=-D CFG_OUTDIR=%OUT_DIR% -D TARGET=%BD_TARGET% %EXT_BUILD_FLAGS%
set  BD_ARGS=-p %WORKSPACE_SILICON%\%FSP_PKG_NAME%\%FSP_PKG_NAME%.dsc -b RELEASE %BD_MACRO% -a %FSP_ARCH% -a X64 -n %NUMBER_OF_PROCESSORS% -t %TOOL_CHAIN_TAG% -y ReportRelease.log -Y PCD -Y LIBRARY %VAR_BUILD_FLAGS%
set  FSP_BUILD_TYPE=0x0001
set  FSP_RELEASE_TYPE=0x0002
goto Build

:DebugBuild
set  BD_TARGET=DEBUG
set  BD_MACRO=-D CFG_OUTDIR=%OUT_DIR% -D TARGET=%BD_TARGET% %EXT_BUILD_FLAGS%
set  BD_ARGS=-p %WORKSPACE_SILICON%\%FSP_PKG_NAME%\%FSP_PKG_NAME%.dsc -b DEBUG  %BD_MACRO% -a %FSP_ARCH% -a X64 -n %NUMBER_OF_PROCESSORS% -t %TOOL_CHAIN_TAG% -y ReportDebug.log -Y PCD -Y LIBRARY %VAR_BUILD_FLAGS%
set  FSP_BUILD_TYPE=0x0000
set  FSP_RELEASE_TYPE=0x0000
goto Build

:Build
call :PreBuild  CALL_RET
if "%CALL_RET%"=="1" exit /b 1
@if /I "%BuildHeaderOnly%" == "TRUE" (
  goto End
)
call build  %BD_ARGS% %SI_BUILD_OPTION_PCD% %FSP_BUILD_OPTION_PCD%
if ERRORLEVEL 1 exit /b 1
call :PostBuild
goto End

:Usage
echo. ================================================================
echo  Usage: "BuildFv.cmd [/h | /? | /r | /rp | /tr | /d | /clean | /catalog | Target Platform] [/header]"
echo
echo     Supported Target Platform :
echo     PantherLake
echo
echo     Note : PantherLake build by default if you don't put any Target Platform option.
echo     Example :
echo        - BuildFv.cmd PantherLake /r
echo
echo. ================================================================
exit /B 1

@REM :CopyBin
@REM @if exist %1\*.efi   xcopy %1\*.efi   %2 /D /U /Y > NUL
@REM @if exist %1\*.inf   xcopy %1\*.inf   %2 /D /U /Y > NUL
@REM @if exist %1\*.depex xcopy %1\*.depex %2 /D /U /Y > NUL
@REM goto:EOF

:PreBuild
echo Start of PreBuild ...

@if not exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include mkdir %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include

if exist %WORKSPACE_SILICON%\%FSP_PKG_NAME%\%FSP_PKG_NAME%.dsc attrib -r %WORKSPACE_SILICON%\%FSP_PKG_NAME%\%FSP_PKG_NAME%.dsc
set %~1=1
set FSP_T_UPD_GUID=34686CA3-34F9-4901-B82A-BA630F0714C6
set FSP_M_UPD_GUID=39A250DB-E465-4DD1-A2AC-E2BD3C0E2385
set FSP_S_UPD_GUID=CAE3605B-5B34-4C85-B3D7-27D54273C40F
%PYTHON_COMMAND% %WORKSPACE_CORE%\IntelFsp2Pkg\Tools\GenCfgOpt.py UPDTXT ^
     %WORKSPACE_SILICON%\%FSP_PKG_NAME%\%FSP_PKG_NAME%.dsc ^
     %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV ^
     %BD_MACRO% %SI_BUILD_OPTION_PCD% %FSP_BUILD_OPTION_PCD%
if "%ERRORLEVEL%"=="256" (
  REM  DSC is not changed, no need to recreate MAP and BIN file
) else (
  if ERRORLEVEL 1 goto:PreBuildFail
  echo UPD TXT file was generated successfully !

  echo Generate VPD Header File ...
  del /Q /f %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%FSP_T_UPD_GUID%.bin ^
            %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%FSP_T_UPD_GUID%.map 2>nul

  del /Q /f %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%FSP_M_UPD_GUID%.bin ^
            %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%FSP_M_UPD_GUID%.map 2>nul

  del /Q /f %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%FSP_S_UPD_GUID%.bin ^
            %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%FSP_S_UPD_GUID%.map 2>nul

  call BPDG ^
       %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%FSP_T_UPD_GUID%.txt ^
       -o %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%FSP_T_UPD_GUID%.bin ^
       -m %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%FSP_T_UPD_GUID%.map
  if ERRORLEVEL 1 goto:PreBuildFail

  call BPDG ^
       %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%FSP_M_UPD_GUID%.txt ^
       -o %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%FSP_M_UPD_GUID%.bin ^
       -m %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%FSP_M_UPD_GUID%.map
  if ERRORLEVEL 1 goto:PreBuildFail

  call BPDG ^
       %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%FSP_S_UPD_GUID%.txt ^
       -o %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%FSP_S_UPD_GUID%.bin ^
       -m %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%FSP_S_UPD_GUID%.map
  if ERRORLEVEL 1 goto:PreBuildFail
)

%PYTHON_COMMAND% %WORKSPACE_CORE%\IntelFsp2Pkg\Tools\GenCfgOpt.py HEADER ^
         %WORKSPACE_SILICON%\%FSP_PKG_NAME%\%FSP_PKG_NAME%.dsc ^
         %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV ^
         %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\BootLoaderPlatformData.h ^
         %BD_MACRO% %SI_BUILD_OPTION_PCD% %FSP_BUILD_OPTION_PCD%
if "%ERRORLEVEL%"=="256" (
    REM  No need to recreate header file
) else (
    if ERRORLEVEL 1 goto:PreBuildFail
    echo Vpd header file was generated successfully !

    echo Generate BSF File ...

    if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Fsp.bsf attrib -r %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Fsp.bsf
    %PYTHON_COMMAND% %WORKSPACE_CORE%\IntelFsp2Pkg\Tools\GenCfgOpt.py GENBSF ^
         %WORKSPACE_SILICON%\%FSP_PKG_NAME%\%FSP_PKG_NAME%.dsc ^
         %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV ^
         %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Fsp.bsf ^
         %BD_MACRO% %SI_BUILD_OPTION_PCD% %FSP_BUILD_OPTION_PCD%

    if ERRORLEVEL 1 goto:PreBuildFail
    echo BSF file was generated successfully !
    if exist %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\FspUpd.h attrib -r %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\FspUpd.h
    if exist "%OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspUpd.h" (
      copy /y %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspUpd.h %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\FspUpd.h
      )
    if exist %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\FsptUpd.h attrib -r %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\FsptUpd.h
    if exist "%OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FsptUpd.h" (
      copy /y %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FsptUpd.h %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\FsptUpd.h
      )
    if exist %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\FspmUpd.h attrib -r %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\FspmUpd.h
    if exist "%OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspmUpd.h" (
      copy /y %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspmUpd.h %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\FspmUpd.h
      )
    if exist "%WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Include\MemInfoHob.h" (
      copy /y %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Include\MemInfoHob.h %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\MemInfoHob.h
      )
    if exist %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\FspsUpd.h attrib -r %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\FspsUpd.h
    if exist "%OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspsUpd.h" (
      copy /y %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspsUpd.h %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\FspsUpd.h
      )
    if exist %WORKSPACE_CORE_SILICON%\IntelSiliconPkg\Include\IndustryStandard\FirmwareVersionInfo.h attrib -r %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\FirmwareVersionInfo.h
    if exist "%WORKSPACE_CORE_SILICON%\IntelSiliconPkg\Include\IndustryStandard\FirmwareVersionInfo.h" (
      copy /y %WORKSPACE_CORE_SILICON%\IntelSiliconPkg\Include\IndustryStandard\FirmwareVersionInfo.h %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\
      )
    if exist %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\FspProducerDataHeader.h  attrib -r %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\FspProducerDataHeader.h
    if exist "%WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\FspProducerDataHeader.h" (
      copy /y %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Fsp\Include\FspProducerDataHeader.h %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\
      )
)

:PreBuildRet
set %~1=0
echo End of PreBuild ...
echo.
goto:EOF

:PreBuildFail
del /q /f %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspUpd.h
del /q /f %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FsptUpd.h
del /q /f %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspmUpd.h
del /q /f %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspsUpd.h
echo.
(goto) 2>nul & endlocal & exit /b 1
goto:EOF

:PostBuild
echo Start of PostBuild ...
echo Patch FSP-O Image ...
%PYTHON_COMMAND% %WORKSPACE_CORE%\IntelFsp2Pkg\Tools\PatchFv.py ^
    %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV ^
    FSP-O:FSP  ^
    "0x0000,            _BASE_FSP-O_,                                                                                       @Temporary Base" ^
    "<[0x0000]>+0x00AC, [<[0x0000]>+0x0020],                                                                                @FSP-O Size" ^
    "<[0x0000]>+0x00B0, [0x0000],                                                                                           @FSP-O Base" ^
    "<[0x0000]>+0x00B6, ([<[0x0000]>+0x00B6] & 0xFFFF0FFC) | 0x8000 | %FSP_BUILD_TYPE% | %FSP_RELEASE_TYPE%,                @FSP-O Component Attribute"
:SkipFspO
echo Patch FSP-T Image ...
@REM
@REM @todo Change FSP.fd to Fsp.fd
@REM
%PYTHON_COMMAND% %WORKSPACE_CORE%\IntelFsp2Pkg\Tools\PatchFv.py ^
     %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV ^
     FSP-T:FSP  ^
     "0x0000,            _BASE_FSP-T_,                                                                                       @Temporary Base" ^
     "<[0x0000]>+0x00AC, [<[0x0000]>+0x0020],                                                                                @FSP-T Size" ^
     "<[0x0000]>+0x00B0, [0x0000],                                                                                           @FSP-T Base" ^
     "<[0x0000]>+0x00B4, ([<[0x0000]>+0x00B4] & 0xFFFFFFFF) | 0x0001,                                                        @FSP-T Image Attribute" ^
     "<[0x0000]>+0x00B6, ([<[0x0000]>+0x00B6] & 0xFFFF0FFC) | 0x1000 | %FSP_BUILD_TYPE% | %FSP_RELEASE_TYPE%,                @FSP-T Component Attribute" ^
     "<[0x0000]>+0x00B8, 70BCF6A5-FFB1-47D8-B1AE-EFE5508E23EA:0x1C - <[0x0000]>,                                             @FSP-T CFG Offset" ^
     "<[0x0000]>+0x00BC, [70BCF6A5-FFB1-47D8-B1AE-EFE5508E23EA:0x14] & 0xFFFFFF - 0x001C,                                    @FSP-T CFG Size" ^
     "<[0x0000]>+0x00C4, FspSecCoreT:%SYMBOL_PREFIX%TempRamInitApi - [0x0000],                                               @TempRamInit API" ^
     "<[0x0000]>+0x010C, 0x%YYYY%,                                                                                           @BuildTimeStamp" ^
     "<[0x0000]>+0x0110, 0x%MMDD%,                                                                                           @BuildTimeStamp" ^
     "<[0x0000]>+0x0114, 0x%HHmm%,                                                                                           @BuildTimeStamp" ^
     "0x0000,            0x00000000,                                                                                         @Restore the value" ^
     "FspSecCoreT:%SYMBOL_PREFIX%FspInfoHeaderRelativeOff, FspSecCoreT:%SYMBOL_PREFIX%AsmGetFspInfoHeader - {912740BE-2284-4734-B971-84B027353F0C:0x1C}, @FSP-T Header Offset"
if ERRORLEVEL 1 goto:PreBuildFail

echo Patch FSP-M Image ...
@REM
@REM @todo Change FSP.fd to Fsp.fd
@REM
%PYTHON_COMMAND% %WORKSPACE_CORE%\IntelFsp2Pkg\Tools\PatchFv.py ^
     %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV ^
     FSP-M:FSP  ^
     "0x0000,            _BASE_FSP-M_,                                                                                       @Temporary Base" ^
     "<[0x0000]>+0x00AC, [<[0x0000]>+0x0020],                                                                                @FSP-M Size" ^
     "<[0x0000]>+0x00B0, [0x0000],                                                                                           @FSP-M Base" ^
     "<[0x0000]>+0x00B4, ([<[0x0000]>+0x00B4] & 0xFFFFFFFF) | 0x0001,                                                        @FSP-M Image Attribute" ^
     "<[0x0000]>+0x00B6, ([<[0x0000]>+0x00B6] & 0xFFFF0FFC) | 0x2000 | %FSP_BUILD_TYPE% | %FSP_RELEASE_TYPE%,                @FSP-M Component Attribute" ^
     "<[0x0000]>+0x00B8, D5B86AEA-6AF7-40D4-8014-982301BC3D89:0x1C - <[0x0000]>,                                             @FSP-M CFG Offset" ^
     "<[0x0000]>+0x00BC, [D5B86AEA-6AF7-40D4-8014-982301BC3D89:0x14] & 0xFFFFFF - 0x001C,                                    @FSP-M CFG Size" ^
     "<[0x0000]>+0x00D0, Fsp24SecCoreM:%SYMBOL_PREFIX%FspMemoryInitApi - [0x0000],                                           @MemoryInitApi API" ^
     "<[0x0000]>+0x00D4, Fsp24SecCoreM:%SYMBOL_PREFIX%TempRamExitApi - [0x0000],                                             @TempRamExit API" ^
     "<[0x0000]>+0x00E4, Fsp24SecCoreM:%SYMBOL_PREFIX%FspMultiPhaseMemoryInitApi - [0x0000],                                 @FspMultiPhaseMemoryInitApi API" ^
     "<[0x0000]>+0x010C, 0x%YYYY%,                                                                                           @BuildTimeStamp" ^
     "<[0x0000]>+0x0110, 0x%MMDD%,                                                                                           @BuildTimeStamp" ^
     "<[0x0000]>+0x0114, 0x%HHmm%,                                                                                           @BuildTimeStamp" ^
     "Fsp24SecCoreM:%SYMBOL_PREFIX%FspPeiCoreEntryOff, PeiCore:%SYMBOL_PREFIX%_ModuleEntryPoint - [0x0000],                  @PeiCore Entry" ^
     "0x0000,            0x00000000,                                                                                         @Restore the value" ^
     "Fsp24SecCoreM:%SYMBOL_PREFIX%FspInfoHeaderRelativeOff, Fsp24SecCoreM:%SYMBOL_PREFIX%AsmGetFspInfoHeader - {912740BE-2284-4734-B971-84B027353F0C:0x1C}, @FSP-M Header Offset"
if ERRORLEVEL 1 goto:PreBuildFail


echo Patch FSP-S Image ...
@REM
@REM @todo Change FSP.fd to Fsp.fd
@REM
%PYTHON_COMMAND% %WORKSPACE_CORE%\IntelFsp2Pkg\Tools\PatchFv.py ^
     %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV ^
     FSP-S:FSP  ^
     "0x0000,            _BASE_FSP-S_,                                                                                       @Temporary Base" ^
     "<[0x0000]>+0x00AC, [<[0x0000]>+0x0020],                                                                                @FSP-S Size" ^
     "<[0x0000]>+0x00B0, [0x0000],                                                                                           @FSP-S Base" ^
     "<[0x0000]>+0x00B4, ([<[0x0000]>+0x00B4] & 0xFFFFFFFF) | 0x0001,                                                        @FSP-S Image Attribute" ^
     "<[0x0000]>+0x00B6, ([<[0x0000]>+0x00B6] & 0xFFFF0FFC) | 0x3000 | %FSP_BUILD_TYPE% | %FSP_RELEASE_TYPE%,                @FSP-S Component Attribute" ^
     "<[0x0000]>+0x00B8, E3CD9B18-998C-4F76-B65E-98B154E5446F:0x1C - <[0x0000]>,                                             @FSP-S CFG Offset" ^
     "<[0x0000]>+0x00BC, [E3CD9B18-998C-4F76-B65E-98B154E5446F:0x14] & 0xFFFFFF - 0x001C,                                    @FSP-S CFG Size" ^
     "<[0x0000]>+0x00D8, Fsp24SecCoreS:%SYMBOL_PREFIX%FspSiliconInitApi - [0x0000],                                          @SiliconInit API" ^
     "<[0x0000]>+0x00CC, Fsp24SecCoreS:%SYMBOL_PREFIX%NotifyPhaseApi - [0x0000],                                             @NotifyPhase API" ^
     "<[0x0000]>+0x00DC, Fsp24SecCoreS:%SYMBOL_PREFIX%FspMultiPhaseSiInitApi - [0x0000],                                     @FspMultiPhaseSiInitApi API" ^
     "<[0x0000]>+0x010C, 0x%YYYY%,                                                                                           @BuildTimeStamp" ^
     "<[0x0000]>+0x0110, 0x%MMDD%,                                                                                           @BuildTimeStamp" ^
     "<[0x0000]>+0x0114, 0x%HHmm%,                                                                                           @BuildTimeStamp" ^
     "0x0000,            0x00000000,                                                                                         @Restore the value" ^
     "Fsp24SecCoreS:%SYMBOL_PREFIX%FspInfoHeaderRelativeOff, Fsp24SecCoreS:%SYMBOL_PREFIX%AsmGetFspInfoHeader - {912740BE-2284-4734-B971-84B027353F0C:0x1C}, @FSP-S Header Offset"
if ERRORLEVEL 1 goto:PreBuildFail

@REM Copy FSP binary to PantherLakeFspBinPkg
echo on
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Fsp.bsf attrib -r %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Fsp.bsf
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Fsp.fd attrib -r %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Fsp.fd
copy /y %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FSP.fd %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Fsp.fd
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\FspUpd.h attrib -r %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\FspUpd.h
if exist "%OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspUpd.h" (
  copy /y %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspUpd.h %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\
  )
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\FsptUpd.h attrib -r %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\FsptUpd.h
if exist "%OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FsptUpd.h" (
  copy /y %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FsptUpd.h %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\
  )
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\FspmUpd.h attrib -r %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\FspmUpd.h
if exist "%OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspmUpd.h" (
  copy /y %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspmUpd.h %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\
  )
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\FspsUpd.h attrib -r %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\FspsUpd.h
if exist "%OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspsUpd.h" (
  copy /y %OUT_DIR%\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspsUpd.h %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\
  )
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\MemInfoHob.h attrib -r %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\MemInfoHob.h
if exist "%WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Include\MemInfoHob.h" (
  copy /y %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Include\MemInfoHob.h %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\
  )
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\SmbiosCacheInfoHob.h attrib -r %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\SmbiosCacheInfoHob.h
if exist "%WORKSPACE_COMMON%\%PLATFORM_SI_PACKAGE%\Include\SmbiosCacheInfoHob.h" (
  copy /y %WORKSPACE_COMMON%\%PLATFORM_SI_PACKAGE%\Include\SmbiosCacheInfoHob.h %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\
  )
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\SmbiosProcessorInfoHob.h attrib -r %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\SmbiosProcessorInfoHob.h
if exist "%WORKSPACE_COMMON%\%PLATFORM_SI_PACKAGE%\Include\SmbiosProcessorInfoHob.h" (
  copy /y %WORKSPACE_COMMON%\%PLATFORM_SI_PACKAGE%\Include\SmbiosProcessorInfoHob.h %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\
  )
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\HobUsageDataHob.h attrib -r %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\HobUsageDataHob.h
if exist %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Include\HobUsageDataHob.h (
  copy /y %WORKSPACE_SILICON%\%PLATFORM_SI_PACKAGE%\Include\HobUsageDataHob.h %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\Include\
  )
if exist %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\FspPkgPcdShare.dsc attrib -r %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\FspPkgPcdShare.dsc
if exist %WORKSPACE_SILICON%\%FSP_PKG_NAME%\FspPkgPcdShare.dsc (
  copy /y %WORKSPACE_SILICON%\%FSP_PKG_NAME%\FspPkgPcdShare.dsc %WORKSPACE_SILICON%\%FSP_BIN_PKG_NAME%\
  )

echo off

echo Patch is DONE

goto:EOF

:End
echo.
