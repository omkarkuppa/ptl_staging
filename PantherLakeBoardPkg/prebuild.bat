@REM @file
@REM  Pre build script.
@REM
@REM  @copyright
@REM  INTEL CONFIDENTIAL
@REM  Copyright (C) 2017 Intel Corporation.
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

@echo Set build capsule flag with default being OFF
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
@set WORKSPACE_CORE_SILICON=%WORKSPACE_ROOT%\Edk2Platforms\Silicon\Intel
@set WORKSPACE_CORE_PLATFORM=%WORKSPACE_ROOT%\Edk2Platforms\Platform\Intel
@set WORKSPACE_CORE_FEATURES=%WORKSPACE_ROOT%\Edk2Platforms\Features\Intel
@set WORKSPACE_COMMON=%WORKSPACE_ROOT%\Intel
@set WORKSPACE_PLATFORM=%WORKSPACE_ROOT%\Intel
@set WORKSPACE_SILICON=%WORKSPACE_ROOT%\Intel
@set WORKSPACE_FSP_BIN=%WORKSPACE%\Intel
@set WORKSPACE_BINARIES=%WORKSPACE%\Binaries
@set EDK_TOOLS_BIN=%WORKSPACE_CORE%\BaseTools\Bin\Win32
@set EDK_TOOLS_PATH=%WORKSPACE_CORE%\BaseTools
@set WORKSPACE_CONF=%WORKSPACE_ROOT%\Conf
@set WORKSPACE_ROM=%WORKSPACE_ROOT%\RomImages


@if not defined WORKSPACE_ADDITIONAL_TOOLS_PATH (
  @set WORKSPACE_ADDITIONAL_TOOLS_PATH=%WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\SignFv
  goto SetAdditionalPath
) else (
  goto SetNormalPath
)

:SetAdditionalPath
@set PATH=%WORKSPACE_ADDITIONAL_TOOLS_PATH%;%PATH%
@set OPENSSL_PATH=%WORKSPACE_BINARIES%\Tools\OpenSSL

:SetNormalPath
@set EFI_SOURCE=%WORKSPACE_CORE%
@set PATH=%NASM_PREFIX%;%PATH%

@echo %WORKSPACE%
@echo %WORKSPACE_ROOT%
@echo %WORKSPACE_CORE%
@echo %WORKSPACE_COMMON%
@echo %WORKSPACE_PLATFORM%
@echo %WORKSPACE_SILICON%
@echo %WORKSPACE_BINARIES%
@echo %PACKAGES_PATH%
@echo %EDK_TOOLS_BIN%
@echo %EDK_TOOLS_PATH%
@echo %WORKSPACE_CONF%
@echo %WORKSPACE_ROM%

@echo %WORKSPACE_FSP_BIN%
@echo %PLATFORM_PACKAGE%
@echo %PLATFORM_BOARD_PACKAGE%

@echo %WORKSPACE_PLATFORM%

@set SCRIPT_ERROR=0

@if %IBBSIGN% EQU TRUE (
  goto Toolchains
)

@if not exist %WORKSPACE_CONF% mkdir %WORKSPACE_CONF%

@if not defined PYTHON_COMMAND (
  set PYTHON_COMMAND=py -3

  %PYTHON_COMMAND% --version
  if not %ERRORLEVEL% == 0 (
    @echo !!! ERROR !!! PYTHON_COMMAND not setting and py -3 is invalid
    set SCRIPT_ERROR=1
    goto :EndPreBuild
  )
)

@if %CATALOG_RELEASE% EQU TRUE (
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
  @if exist %WORKSPACE_COMMON%\OneSiliconPkg\Tools\Catalog\CatalogEncoder.py (
    set BIOS_DECODER_XML_TEMPLATE=%WORKSPACE_COMMON%\OneSiliconPkg\Tools\Catalog\BIOS_Decoder.xml.template
    set BIOS_DECODER_XML_CATALOGBLD=%WORKSPACE%\Build\BIOS_Decoder.xml
    set RELEASEBIOS_DECODER_XML=%WORKSPACE%\Build\ReleaseBIOS_Decoder.xml
    set CATALOGBIOS_DECODER_XML=%WORKSPACE%\Build\CatalogBIOS_Decoder.xml
    set BIOS_DECODER_XML=%WORKSPACE%\Build\BIOS_Decoder.xml
    set CATALOG_ENCODER=%WORKSPACE_COMMON%\OneSiliconPkg\Tools\Catalog\CatalogEncoder.py
    set REPLACECL_PATH=%WORKSPACE%\Intel\OneSiliconPkg\Tools\Catalog
    set EXT_BUILD_FLAGS=%EXT_BUILD_FLAGS% --cmd-len=2048
    set CATALOG_DICT_GENERATOR=%WORKSPACE_COMMON%\OneSiliconPkg\Tools\Catalog\CatalogDictionaryGen.py
    set CATALOG_STATUS_CODE_LIBHDR=%WORKSPACE_COMMON%\OneSiliconPkg\Tools\Catalog\CatalogReportStatusCodeLib.h
    set MRC_DEBUG_HOOK_HEADER_FILE=%WORKSPACE_COMMON%\OneSiliconPkg\IpBlock\MemoryInit\src\IncludePublic\MrcPostCodes.h
  ) else (
    echo.
    echo !!! ERROR !!! Catalog feature requirement issue: CatalogEncoder.py is not found.
    echo.
    set SCRIPT_ERROR=1
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
@if not defined PACKAGES_PATH (
  echo.
  echo !!! ERROR !!! Failed to set PACKAGES_PATH. !!!
  echo.
  set SCRIPT_ERROR=1
  goto :EndPreBuild
)

@echo Set basic environment.
@echo.
@echo Prebuild:  Run edksetup.bat batch file.
@echo.

echo %CD%
cd %WORKSPACE_CORE%
echo %CD%
@call edksetup.bat Rebuild VS2019
@if %ERRORLEVEL% NEQ 0 (
  @echo !!! ERROR !!! Failed to run edksetup.bat Rebuild. !!!
  set SCRIPT_ERROR=1
  goto :EndPreBuild
)

@set TARGET_PLATFORM=PantherLake
@set TARGET_PLATFORM_SHORT=PTL

@echo.
@echo Build tools in Edk2Platforms
@echo.
cd %WORKSPACE_CORE_SILICON%\Tools
nmake
@if %ERRORLEVEL% NEQ 0 (
  @echo !!! ERROR !!! Failed to build tools in Edk2Platforms !!!
  set SCRIPT_ERROR=1
  goto :EndPreBuild
)

echo %CD%
cd %WORKSPACE%
echo %CD%
@set EFI_SOURCE=%WORKSPACE_CORE%

:Toolchains
@REM
@REM Get Visual Studio environment Setting By Edksetup.
@REM Order of precedence is 2019 and then 2017.
@REM
@if not defined TOOL_CHAIN_TAG (
  if defined VS160COMNTOOLS (
    set TOOL_CHAIN_TAG=VS2019
  ) else if defined VS150COMNTOOLS (
    set TOOL_CHAIN_TAG=VS2017
  )
)

REM If no supported version of Visual Studio was detected, return an error.
@if not defined TOOL_CHAIN_TAG (
  echo.
  echo !!! ERROR !!! Preferred Visual Studio not detected!!!
  echo Supported Compilers are: VS2019 or VS2017.
  echo.
  set SCRIPT_ERROR=1
  goto :EndPreBuild
)

@if %IBBSIGN% EQU FALSE (
  @echo Show CL revision
  cl
    @if %ERRORLEVEL% NEQ 0 (
    @echo !!! ERROR !!! Failed to show CL revision !!!
    set SCRIPT_ERROR=1
    goto :EndPreBuild
  )
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

@if %IBBSIGN% EQU TRUE (
  goto EndPreBuild
)

@findstr /V "ACTIVE_PLATFORM TARGET TARGET_ARCH TOOL_CHAIN_TAG BUILD_RULE_CONF" %WORKSPACE%\Conf\target.txt > %BUILD_DIR_PATH%\target.txt
@echo ACTIVE_PLATFORM = %WORKSPACE_PLATFORM%/%PLATFORM_BOARD_PACKAGE%/BoardPkg.dsc        >> %BUILD_DIR_PATH%\target.txt
@echo TARGET          = %TARGET%                                  >> %BUILD_DIR_PATH%\target.txt
@echo TARGET_ARCH     = %FSP_ARCH% X64                            >> %BUILD_DIR_PATH%\target.txt
@echo TOOL_CHAIN_TAG  = %TOOL_CHAIN_TAG%                          >> %BUILD_DIR_PATH%\target.txt
@echo BUILD_RULE_CONF = Conf/build_rule.txt                       >> %BUILD_DIR_PATH%\target.txt
@move /Y %BUILD_DIR_PATH%\target.txt Conf

@echo Current Directory = %CD%
@echo WORKSPACE_FSP_BIN = %WORKSPACE_FSP_BIN%

@if %BOOT_STAGE% EQU 5 (
  set BUILD_OPTION_PCD=%BUILD_OPTION_PCD% --pcd gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable=TRUE
)

@if %BOOT_STAGE% EQU 6 (
  @if %ATOM_BUILD% EQU FALSE (
    set BUILD_OPTION_PCD=%BUILD_OPTION_PCD%^
    --pcd gBoardModuleTokenSpaceGuid.PcdSetupEnable=TRUE^
    --pcd gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable=TRUE^
    --pcd gPantherLakeBinPkgTokenSpaceGuid.PcdStartupAcmBinEnable=TRUE

    set SECURE_BOOT_ENABLE=TRUE
    set STARTUP_ACM_ENABLE=TRUE
  )
)

@REM
@REM
@REM Check %FSP_BINARY_BUILD% and set %FSP_TEST_RELEASE%
@REM
@set FSP_TEST_RELEASE=FALSE
@if %FSP_BINARY_BUILD% EQU TRUE (
  if /I "%1" == "TEST_RELEASE" (
    set FSP_TEST_RELEASE=TRUE
  )
)
@echo FSP_BINARY_BUILD (%FSP_BINARY_BUILD%)
@echo FSP_TEST_RELEASE (%FSP_TEST_RELEASE%)

@if "FSP_BINARY_BUILD"=="TRUE" (
  @if %FSP_BINARY_BUILD_ONLY% EQU TRUE goto :EndPreBuild
)

@echo FSP_BINARY_BUILD = %FSP_BINARY_BUILD%
@echo FSP_TEST_RELEASE = %FSP_TEST_RELEASE%
@echo RESILIENCY_BUILD = %RESILIENCY_BUILD%


@REM
@REM Printing the build type
@REM
@echo.
@echo Printing the build type ([U]nified, [N]on-Restricted, [I]nternal, E[x]ternal, [P]erformance, SL[E], Code-[C]overage)
@echo Build type (%BUILD%)
@echo.

@REM
@REM Create BiosId.env based on the build type

@if %EMBEDDED_BUILD% EQU FALSE goto :SkipEmbeddedBuild
@set BIOS_ID_FILE=%BUILD_DIR%\BiosId.env
@set BIOSID_FILENAME1=BiosId
@copy %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\%BIOSID_FILENAME1%.env %BIOS_ID_FILE% /Y
set BUILD=I
set PCH_TYPE=P

@if %PTL_BUILD% EQU TRUE (
  set PCH_TYPE=P
  set TARGET_PLATFORM_SHORT=PTL
)

echo BOARD_ID      = %TARGET_PLATFORM_SHORT%%PCH_TYPE%FW%BUILD%>> %BIOS_ID_FILE%
echo BOARD_EXT     = %TARGET_SHORT%00>> %BIOS_ID_FILE%
goto :BiosIdBuild

:SkipEmbeddedBuild

set BIOS_ID_FILE=%BUILD_DIR%\BiosId.env
copy %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\BiosId.env %BIOS_ID_FILE% /Y
set BUILD=I
set PCH_TYPE=P

@if %PTL_BUILD% EQU TRUE (
  set PCH_TYPE=P
  set TARGET_PLATFORM_SHORT=PTL
)

echo BOARD_ID      = %TARGET_PLATFORM_SHORT%%PCH_TYPE%FW%BUILD%>> %BIOS_ID_FILE%
echo BOARD_EXT     = %TARGET_SHORT%00>> %BIOS_ID_FILE%

:BiosIdBuild
@if /I "%NOTIMESTAMP%" == "1" (
   SET NoTime=-nt
)else (
   SET NoTime=
)
%PYTHON_COMMAND% %WORKSPACE_CORE_PLATFORM%\Tools\GenBiosId\GenBiosId.py ^
-i %BIOS_ID_FILE% ^
-o %BUILD_IA32%\BiosId.bin %NoTime%
@if %ERRORLEVEL% NEQ 0 (
  @echo !!! ERROR: GenBiosId failure !!!
  set SCRIPT_ERROR=1
  goto :EndPreBuild
)


@if %CAPSULE_BUILD% == 1 (
  goto EndCapsulePrep
)

@REM
@echo Skip BIOS_SIZE_OPTION if it is predefined
@REM
@if NOT "%BIOS_SIZE_OPTION%" == "" goto BiosSizeDone

@set BIOS_SIZE_OPTION=

@REM default size option is 14M
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
@echo   UNIVERSAL_PAYLOAD    = %UNIVERSAL_PAYLOAD%
@echo   WORKSPACE_BINARIES   = %WORKSPACE_BINARIES%
@echo.
:EndPreBuild
cd %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%

@echo TARGET              = %TARGET%
@echo FSP_BUILD_PARAMETER = %FSP_BUILD_PARAMETER%
@echo FSP_BINARY_BUILD    = %FSP_BINARY_BUILD%
@echo CATALOG_RELEASE     = %CATALOG_RELEASE%
@echo FSP_TEST_RELEASE    = %FSP_TEST_RELEASE%
@echo BUILD_X64           = %BUILD_X64%
@echo PERFORMANCE_BUILD   = %PERFORMANCE_BUILD%
@echo BUILD_OPTION_PCD    = %BUILD_OPTION_PCD%

