@REM @file
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

@REM #
@REM #  Module Name:
@REM #
@REM #    postbuild.bat
@REM #
@REM #  Abstract:
@REM #
@REM #    Post build script.
@REM #
@REM #--*/

echo on

@set SCRIPT_ERROR=0

setlocal ENABLEDELAYEDEXPANSION
@echo Flash Map File %FLASHMAP_FDF%
@set PREMEM_FV_ADDRESS=

@if not defined PYTHON_COMMAND (
  set PYTHON_COMMAND=py -3
)

@if %IBBSIGN% EQU TRUE (
  @if "%ClientBios%" == "" goto EndPostBuild
  @if not exist %ClientBios% goto EndPostBuild
  echo ClientBios %ClientBios% File size is %~z1.
)

setlocal ENABLEDELAYEDEXPANSION
@if %RESILIENCY_BUILD% EQU TRUE (
  if %EXTENDEDREGION_BUILD% EQU TRUE (
    @set FLASHMAP_FDF=%WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Include\Fdf\FlashMapIncludeExtended_autogen.fdf
  ) else (
    @set FLASHMAP_FDF=%WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Include\Fdf\FlashMapIncludeRes.fdf
  )
) else if %FSP_VALIDATION_BUILD% EQU TRUE (
    @set FLASHMAP_FDF=%WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Include\Fdf\FlashMapIncludeFspV.fdf
) else if %EXTENDEDREGION_BUILD% EQU TRUE (
  @set FLASHMAP_FDF=%WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Include\Fdf\FlashMapIncludeExtended_autogen.fdf
) else (
    @set FLASHMAP_FDF=%WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Include\Fdf\FlashMapInclude.fdf
)
@echo Flash Map File %FLASHMAP_FDF%

@set PREMEM_FV_ADDRESS=

@if not defined WORKSPACE (
    echo.
    echo !!! ERROR !!! WORKSPACE must be defined before executing postbuild.bat !!!
    echo.
    set SCRIPT_ERROR=1
    goto :EndPostBuild
)

@if not defined WORKSPACE_PLATFORM (
    echo.
    echo !!! ERROR !!! WORKSPACE_PLATFORM must be defined before executing postbuild.bat !!!
    echo.
    set SCRIPT_ERROR=1
    goto :EndPostBuild
)

pushd %WORKSPACE%

@if not %CD% == %WORKSPACE% (
    echo.
    echo !!! ERROR !!! postbuild.bat is not executed under %WORKSPACE%
    echo.
    set SCRIPT_ERROR=1
    goto :EndPostBuild
)

@set PATH=%PATH%;%BUILD_DIR_PATH%\FV
copy /y %BUILD_DIR%\FV\GuidedSectionTools.txt %BUILD_DIR%\FV\GuidToolDefinitionConf.ini

@REM
@REM FMMT tool search the "FmmtConf.ini" file in PATH environment variable.
@REM Build folder is one of the possible path.
@REM
copy /y %BUILD_DIR%\FV\GuidedSectionTools.txt %BUILD_DIR%\FV\FmmtConf.ini

@rem
@rem Get the prefixes for ROM Images
@rem

for /f "usebackq tokens=3" %%i in (`FINDSTR /b /c:"VERSION_MAJOR" %WORKSPACE%\%BIOS_ID_FILE%`) do (
  for /f "usebackq tokens=3" %%j in (`FINDSTR /b /c:"VERSION_MINOR" %WORKSPACE%\%BIOS_ID_FILE%`) do (
      set BIOS_MAJOR_VERSION=%%i
      set BIOS_MIN_VERSION=%%j
    )
  )

set BIOS_ROM_FILENAME_INTEXT=External
@set BUILD_TYPE=%TARGET%
@if %CATALOG_RELEASE% EQU TRUE (
  set BUILD_TYPE=%BUILD_TYPE%_CATALOG
)

@if not "%TOOL_CHAIN_TAG%"=="%TOOL_CHAIN_TAG:VS=%" (
  set ROM_FILENAME_COMPILER=VS
) else if not "%TOOL_CHAIN_TAG%"=="%TOOL_CHAIN_TAG:GCC=%" (
  set ROM_FILENAME_COMPILER=GCC
) else if not "%TOOL_CHAIN_TAG%"=="%TOOL_CHAIN_TAG:CLANG=%" (
  set ROM_FILENAME_COMPILER=CLANGPDB
) else (
  set ROM_FILENAME_COMPILER=UNKNOWN
)

set BIOS_PREFIX=PTL

@if %ATOM_BUILD% EQU TRUE (
  set BIOS_PREFIX=%BIOS_PREFIX%-A
)

set BIOS_PREFIX=%BIOS_PREFIX%_P


@REM
REM Post-build FitGen
@REM
@set BIOS_INFO_GUID=4A4CA1C6-871C-45bb-8801-6910A7AA5807
@set STARTUP_AC_MODULE_GUID=26FDAA3D-B7ED-4714-8509-EECF1593800D
@set MICROCODE_ARRAY_FFS_GUID=197DB236-F856-4924-90F8-CDF12FB875F3
@if %IBBSIGN% EQU TRUE (
  @set STARTUP_AC_PARA=-S %STARTUP_AC_MODULE_GUID%
  goto CreatePostIbbDigestHash
)

@REM
@REM UPL Post Build Process Begin
@REM
if "%UNIVERSAL_PAYLOAD%" NEQ "TRUE" goto SkipUefiPayload
@REM +-----------------+
@REM | NV_Storage      |
@REM +-----------------+
@REM | FvPostMemory    |
@REM +-----------------+
@REM | FvSecurity      |
@REM +-----------------+
@REM | FvAdvanced      |
@REM +-----------------+
@REM | FvOsBoot        |                                                               +------------------+
@REM +-----------------+                                                        +------| PayloadEntry.elf |
@REM | FvUefiBoot      |                                                        |      +------------------+
@REM +-----------------+  FMMT   +---------+           +---------+ objcopy      |      +------------------+
@REM | FvUpl (Empty)   |<--------| UPL.ffs |<----------| UPL.elf |<-------------+------| uefi_fv.fv       |
@REM +-----------------+         +---------+           +---------+              |      +------------------+
@REM | FvFwBinaries    |                                                        |            ...
@REM +-----------------+                                                        |      +------------------+
@REM | FvMicrocode     |                                                        +------| other.fv         |
@REM +-----------------+                                                               +------------------+
@REM | FvPostMemory    |
@REM +-----------------+
@REM | FSP-S           |
@REM +-----------------+
@REM | FSP-M           |
@REM +-----------------+
@REM | FvPreMemory     |
@REM +-----------------+
@REM | FSP-Top (T+O)   |
@REM +-----------------+
@set UPL_BUILD_DIR=Build\UefiPayloadPkgX64

call %PYTHON_COMMAND% %WORKSPACE_CORE%\UefiPayloadPkg\UniversalPayloadBuild.py ^
  -s %UPL_SPEC_REVISION% ^
  --SkipBuild ^
  --AddFv bds_fv=%WORKSPACE%\%BUILD_DIR%\FV\FVPPLBDSUNCOMPACT.Fv ^
  -pb %WORKSPACE%\%UPL_BUILD_DIR%\UniversalPayload.elf

@if %errorlevel% NEQ 0 (
  echo UniversalPayloadBuild: AddFv failure
  @set SCRIPT_ERROR=1
  goto EndPostBuild
)

call %EDK_TOOLS_BIN%\GenSec ^
  -s EFI_SECTION_RAW ^
  -o %WORKSPACE%\%UPL_BUILD_DIR%\UniversalPayload.raw ^
     %WORKSPACE%\%UPL_BUILD_DIR%\UniversalPayload.elf

@if %errorlevel% NEQ 0 (
  echo GenSec: Generate RAW failure
  @set SCRIPT_ERROR=1
  goto EndPostBuild
)

call %EDK_TOOLS_BIN%\GenSec ^
  --sectionalign 16 ^
  -o %WORKSPACE%\%UPL_BUILD_DIR%\UniversalPayload.align ^
     %WORKSPACE%\%UPL_BUILD_DIR%\UniversalPayload.raw

@if %errorlevel% NEQ 0 (
  echo GenSec: section alignment failure
  @set SCRIPT_ERROR=1
  goto EndPostBuild
)

call %EDK_TOOLS_BIN%\LzmaCompress ^
  -e %WORKSPACE%\%UPL_BUILD_DIR%\UniversalPayload.align ^
  -o %WORKSPACE%\%UPL_BUILD_DIR%\UniversalPayload.lzma

@if %errorlevel% NEQ 0 (
  echo LzmaCompress: failure to compress
  @set SCRIPT_ERROR=1
  goto EndPostBuild
)

call %EDK_TOOLS_BIN%\GenSec ^
  -s EFI_SECTION_GUID_DEFINED ^
  -g EE4E5898-3914-4259-9D6E-DC7BD79403CF ^
  -r PROCESSING_REQUIRED ^
  -o %WORKSPACE%\%UPL_BUILD_DIR%\UniversalPayload.guid ^
     %WORKSPACE%\%UPL_BUILD_DIR%\UniversalPayload.lzma

@if %errorlevel% NEQ 0 (
  echo GenSec: failure for processing EFI_SECTION_GUID_DEFINED
  @set SCRIPT_ERROR=1
  goto EndPostBuild
)

call %EDK_TOOLS_BIN%\GenFfs ^
  -t EFI_FV_FILETYPE_DXE_CORE ^
  -g 728c3e86-88e7-447a-a146-8f99915ebafa ^
  -o %WORKSPACE%\%UPL_BUILD_DIR%\UniversalPayload.ffs ^
  -i %WORKSPACE%\%UPL_BUILD_DIR%\UniversalPayload.guid

@if %errorlevel% NEQ 0 (
  echo GenFfs: failure for processing EFI_SECTION_GUID_DEFINED
  @set SCRIPT_ERROR=1
  goto EndPostBuild
)

call %PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Features\Upl\Tools\FvUplSizeCheck.py -c^
     %WORKSPACE%\%BUILD_DIR%\FV\FVUPL.Fv.map ^
     %WORKSPACE%\%UPL_BUILD_DIR%\UniversalPayload.ffs

@if %errorlevel% NEQ 0 (
  echo FMMT: failure to FvUplSizeCheck
  @set SCRIPT_ERROR=1
  goto EndPostBuild
)

call %EDK_TOOLS_BINWRAPPERS%\FMMT -a ^
  %BUILD_DIR%\FV\ClientBios.fd ^
  F69584D4-56F0-4D8E-A387-072A836CDD4E ^
  %WORKSPACE%\%UPL_BUILD_DIR%\UniversalPayload.ffs ^
  %BUILD_DIR%\FV\ClientBios.fd

@if %errorlevel% NEQ 0 (
  echo FMMT: failure to add UniversalPayload.ffs
  @set SCRIPT_ERROR=1
  goto EndPostBuild
)

:SkipUefiPayload
@REM
@REM UPL Post Build Process End
@REM

:PayloadBuild

echo.
echo  Call thess scripts to generate FIT/NON FIT Payloads
echo.
@set PAYLOAD_BUILD_FLAGS=IntegrateBuild
@if %RESILIENCY_BUILD% EQU TRUE (
  @set PAYLOAD_BUILD_FLAGS=%PAYLOAD_BUILD_FLAGS% res
)
if "%TARGET%" == "RELEASE" (
  call %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\PayloadManagement\NonFitPayloads\NonFitPayloadBuild.bat %PAYLOAD_BUILD_FLAGS% r
) else (
  call %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\PayloadManagement\NonFitPayloads\NonFitPayloadBuild.bat %PAYLOAD_BUILD_FLAGS%
)
@if %errorlevel% NEQ 0 (
   @echo ERROR: Failure in generating NonFitPayload
   @set SCRIPT_ERROR=1
   goto EndPostBuild
)
  call %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\PayloadManagement\FitPayloads\FitPayloadBuild.bat %PAYLOAD_BUILD_FLAGS%

@if %errorlevel% NEQ 0 (
  @echo ERROR: Failure in generating FitPayload
  @set SCRIPT_ERROR=1
  goto EndPostBuild
)

@REM
@REM Stitch Payloads and ClientBios together
@REM
@set EXTBIOS_REGION_SIZE=
@set EXTEND_PAYLOAD_REGION_SIZE=
@set PAYLOADS_BUILD_DIR=Build\FitPayloadsPkg\%TARGET%_%TOOL_CHAIN_TAG%
@set NONFIT_PAYLOAD_BUILD_DIR=Build\NonFitPayloadsPkg\%TARGET%_%TOOL_CHAIN_TAG%
copy %BUILD_DIR%\FV\ClientBios.fd %BUILD_DIR%\FV\ClientBiosPrePayload.fd

setlocal ENABLEDELAYEDEXPANSION
@if %EXTENDEDREGION_BUILD% EQU FALSE (
  @for /f "tokens=4" %%i in ('@findstr /c:"gMinPlatformPkgTokenSpaceGuid\.PcdFlashNvStorageOffset" %FLASHMAP_FDF%') do @set PAYLOAD_REGION_SIZE=%%i
  call split -f %BUILD_DIR%\FV\ClientBios.fd -s !PAYLOAD_REGION_SIZE! -t %BUILD_DIR%\FV\ClientBios_ExcludePayload.fd -o %BUILD_DIR%\FV\Buffer.bin
  copy /y /b %PAYLOADS_BUILD_DIR%\FV\FITPAYLOADS.fd + %NONFIT_PAYLOAD_BUILD_DIR%\FV\NONFITPAYLOADS.fd + %BUILD_DIR%\FV\ClientBios_ExcludePayload.fd %BUILD_DIR%\FV\ClientBios.fd
  @del %BUILD_DIR%\FV\Buffer.bin
) else (
  @for /f "tokens=4" %%i in ('@findstr /c:"gBoardModuleTokenSpaceGuid\.PcdFlashAllExtendAreaSize" %FLASHMAP_FDF%') do @set EXTBIOS_REGION_SIZE=%%i
  @for /f "tokens=4" %%i in ('@findstr /c:"gIntelSiliconPkgTokenSpaceGuid\.PcdFlashMicrocodeFvSize" %FLASHMAP_FDF%') do @set FIT_PAYLOAD_SIZE=%%i
  @for /f "tokens=4" %%i in ('@findstr /c:"gCapsuleFeaturePkgTokenSpaceGuid\.PcdFlashNonFitPayloadSize" %FLASHMAP_FDF%') do @set NON_FIT_PAYLOAD_SIZE=%%i
  set /a PAYLOAD_REGION_SIZE="!FIT_PAYLOAD_SIZE!" + "!NON_FIT_PAYLOAD_SIZE!"
  call split -f %BUILD_DIR%\FV\ClientBios.fd -s !EXTBIOS_REGION_SIZE! -t %BUILD_DIR%\FV\ClientBios16M.fd -o %BUILD_DIR%\FV\ExtBiosRemaining.bin
  call split -f %BUILD_DIR%\FV\ClientBios16M.fd -s !PAYLOAD_REGION_SIZE! -t %BUILD_DIR%\FV\ClientBios_ExcludePayload.fd -o %BUILD_DIR%\FV\Buffer.bin
  copy /y /b %BUILD_DIR%\FV\ExtBiosRemaining.bin + %PAYLOADS_BUILD_DIR%\FV\FITPAYLOADS.fd + %NONFIT_PAYLOAD_BUILD_DIR%\FV\NONFITPAYLOADS.fd + %BUILD_DIR%\FV\ClientBios_ExcludePayload.fd %BUILD_DIR%\FV\ClientBios.fd
  @del %BUILD_DIR%\FV\ExtBiosRemaining.bin
  @del %BUILD_DIR%\FV\Buffer.bin
)
@if %errorlevel% NEQ 0 (
  echo Error: Stitch Fit Payload into ClientBios failure
  @set SCRIPT_ERROR=1
  goto EndPostBuild
)


@REM
@REM XmlCli: Post Build Process Begin
@REM
set WORKSPACE_BUILD_DIR=%WORKSPACE%\%BUILD_DIR%
set XML_CLI_COMMON_BUILD_PATH=%WORKSPACE_BUILD_DIR%\X64\XmlCliFeaturePkg\XmlCliCommon
set XML_CLI_COMMON_DIR=%WORKSPACE_COMMON%\Features\XmlCliFeaturePkg\XmlCliCommon
set INPUT_VFR_DIRS=%WORKSPACE_BUILD_DIR%\X64\%PLATFORM_FULL_PACKAGE%\Setup\Setup\OUTPUT %WORKSPACE_BUILD_DIR%\X64\%PLATFORM_FULL_PACKAGE%\Features\UiApp\D89A7D8B-D016-4D26-93E3-EAB6B4D3B0A2\OUTPUT %WORKSPACE_BUILD_DIR%\X64\SecurityPkg\Tcg\Tcg2Config\Tcg2ConfigDxe\OUTPUT %XML_CLI_COMMON_BUILD_PATH%\Dxe\OUTPUT
@if exist %XML_CLI_COMMON_BUILD_PATH% (
  @echo Create and insert Bios Knobs Data Bin file for XmlCli
  @%PYTHON_COMMAND% %XML_CLI_COMMON_DIR%\Tool\ParseSetup.py post_build -b %WORKSPACE_BUILD_DIR% -xb %XML_CLI_COMMON_BUILD_PATH% -iv %INPUT_VFR_DIRS% -edk %EDK_TOOLS_BIN% -ets %EDK_TOOLS_BINWRAPPERS%
  @echo ==== XmlCliPostBuild: BiosKnobsData.bin File Created =====

  @if not exist %BUILD_DIR%\FV\ClientBios_XmlCli.fd (
    @echo === XmlCliPostBuild: Trying to generate fd file once again... ===
    call %EDK_TOOLS_BINWRAPPERS%\FMMT -a %WORKSPACE%\%BUILD_DIR%\FV\ClientBios.fd A881D567-6CB0-4EEE-8435-2E72D33E45B5 %XML_CLI_COMMON_BUILD_PATH%\BiosKnobsData.ffs %WORKSPACE%\%BUILD_DIR%\FV\ClientBios_XmlCli.fd
    @if not exist %BUILD_DIR%\FV\ClientBios_XmlCli.fd (
      @echo !!! XmlCliPostBuild-ERROR:Build Fail !!!
      @echo *** XmlCliPostBuild: %WORKSPACE%\%BUILD_DIR%\FV\ClientBios_XmlCli.fd does not exist. ***
      @echo -- XmlCliPostBuild: FMMT insertion process may have failed, please check logs above --
      set SCRIPT_ERROR=1
      goto EndPostBuild
    )
  )
  @REM backing up ClientBios.fd before overwriting
  @if not exist %BUILD_DIR%\FV\ClientBios.fd.bak (
    copy %BUILD_DIR%\FV\ClientBios.fd %BUILD_DIR%\FV\ClientBios.fd.bak
  )
  copy /y /b %BUILD_DIR%\FV\ClientBios_XmlCli.fd %BUILD_DIR%\FV\ClientBios.fd
  @REM del %BUILD_DIR%\FV\ClientBios_XmlCli.fd
  del %BUILD_DIR%\FV\ClientBios.fd.bak
  @echo ==== XmlCliPostBuild: BiosKnobsData.bin File Inserted =====
)
@REM
@REM XmlCli: Post Build Process End
@REM

findstr /i %STARTUP_AC_MODULE_GUID% %BUILD_DIR%\FV\FvFwBinaries.inf > NUL
@if %errorlevel% NEQ 0 goto NoStartupAcm
set STARTUP_AC_PARA=-S %STARTUP_AC_MODULE_GUID%

@REM
@REM Support the Type 2 FIT entry version 0x200 format.
@REM
@set STARTUP_ACM_PARA=
@set ACM_FIT_ENTRY_DATA_FILE=%BUILD_DIR%\FV\AcmFitGenPara.txt

@REM Parsing the BtGAcm binary in BIOS rom image.
@call %PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Tools\BtGAcmParser\AcmFitEntryGenerator.py ^
  -F %BUILD_DIR%\FV\ClientBios.fd ^
  -A 0x40000 ^
  -O !ACM_FIT_ENTRY_DATA_FILE!
@if %errorlevel% NEQ 0 (
  echo Error: AcmFitEntryGenerator failure
  @set SCRIPT_ERROR=1
  goto EndPostBuild
)

@REM Get the parameter used for FitGen tool.
for /f "tokens=*" %%i in (!ACM_FIT_ENTRY_DATA_FILE!) do (
  @set STARTUP_ACM_PARA=!STARTUP_ACM_PARA!%%i
)

@set STARTUP_AC_PARA=%STARTUP_AC_PARA% !STARTUP_ACM_PARA!

:NoStartupAcm

@REM
@REM Cutting off unused part of Extended BIOS Region out of the FD image
@REM
@if %EXTENDEDREGION_BUILD% EQU FALSE goto SkipExtendedRegionPostBuildProcess
@if not defined FLASHMAP_FDF goto SkipExtendedRegionPostBuildProcess
  @set EXTENDED_REGION_SIZE=
  @set EXTENDED_REGION_IN_USE=
  @for /f "tokens=4" %%i in ('@findstr /c:"DEFINE EXTENDED_REGION_SIZE " %FLASHMAP_FDF%') do @set EXTENDED_REGION_SIZE=%%i
  @for /f "tokens=4" %%j in ('@findstr /c:"DEFINE EXTENDED_REGION_IN_USE" %FLASHMAP_FDF%') do @set EXTENDED_REGION_IN_USE=%%j
  @echo Extended BIOS Region size        : %EXTENDED_REGION_SIZE%
  @echo Extended BIOS Region size in use : %EXTENDED_REGION_IN_USE%
  @set /a CUTOFF_SIZE="%EXTENDED_REGION_SIZE%-%EXTENDED_REGION_IN_USE%"
  @echo Cutting off unused %CUTOFF_SIZE% in size
  @copy /y /b %BUILD_DIR%\FV\ClientBios.fd %BUILD_DIR%\FV\ClientBios_ExtSplitPre.fd
  call split -f %BUILD_DIR%\FV\ClientBios.fd -s %CUTOFF_SIZE% -t %BUILD_DIR%\FV\ClientBios_ExtSplitPost.fd -o %BUILD_DIR%\FV\Garbage.bin
  @copy /y /b %BUILD_DIR%\FV\ClientBios_ExtSplitPost.fd %BUILD_DIR%\FV\ClientBios.fd
  @del %BUILD_DIR%\FV\Garbage.bin
  @REM
  @REM  Cut off extended region from ClientBios.fd
  @REM  We will append extended region to ClientBios.fd in last step of postbuild.bat
  @REM
  call split -f %BUILD_DIR%\FV\ClientBios.fd -s %EXTENDED_REGION_IN_USE% -o %BUILD_DIR%\FV\ExtendedRegion.bin -t %BUILD_DIR%\FV\ClientBios.fd
:SkipExtendedRegionPostBuildProcess

@REM
@REM Remove MinPlatformPkg module instances that are overridden in PantherLakeBoardPkg
@REM
copy %BUILD_DIR%\FV\ClientBios.fd %BUILD_DIR%\FV\ClientBios_FmmtPre.fd /Y

@rem
@rem Capsule update fault tolerance support begin
@rem
@rem Below scripts calculate the digest of OBB region and Non Fit Payload region to replace the dummy one in FvPreMemory.
@rem The purpose of OBB/Non Fit Payload digest is to examine the integrity when loading OBB/Non Fit Payload from external storage for recovery.
@rem So this section must be executed when:
@rem   1. After all operations on OBB FVs are done.
@rem   2. Before BootGuard BPM/KM is created.
@rem
@rem gObbSha384HashBinGuid
@set OBB_HASH_FILE_GUID=169BB326-C8E4-4588-A742-6808D7499B47
@set OBB_HASH_TMP_FOLDER=%BUILD_DIR%\FV\ObbDigestTmp
call %EDK_TOOLS_BINWRAPPERS%\FMMT -v %BUILD_DIR%\FV\FVPREMEMORY.FV | findstr /c:"ObbDigest"
@if !errorlevel! NEQ 0 (
  echo No ObbDigest found in FvPreMemory. Skip fault tolerance support process in postbuild
  goto :SkipFaultToleranceSupport
)
@rem gPayloadSha384HashBinGuid
@set PAYLOAD_HASH_FILE_GUID=7B0B85CA-8A43-4520-BA8B-33370F305210
call %EDK_TOOLS_BINWRAPPERS%\FMMT -v %BUILD_DIR%\FV\FVPREMEMORY.FV | findstr /c:"NonFitPayloadDigest"
@if !errorlevel! NEQ 0 (
  echo No Non Fit Payload digest found in FvPreMemory. Skip fault tolerance support process in postbuild
  goto :SkipFaultToleranceSupport
)
@if not exist %OBB_HASH_TMP_FOLDER% mkdir %OBB_HASH_TMP_FOLDER%

:CreatePostIbbDigestHash
@rem
@rem Generate Obb.bin. Calculate Obb size and offset for splitting BIOS ROM
@rem If debug and release BIOS have different flashmap in one file need to call the strip flashmap, otherwise no need.
@rem
@set TempFlashMap=%WORKSPACE_ROOT%\FlashMapInclude_Temp.fdf
@call %PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Tools\StripFlashmap.py "%TARGET%" %FLASHMAP_FDF% !TempFlashMap!
@for /f "tokens=4" %%i in ('@findstr /c:"gCapsuleFeaturePkgTokenSpaceGuid\.PcdFlashObbOffset" %TempFlashMap%') do @set FLASH_OBB_OFFSET=%%i
@for /f "tokens=4" %%i in ('@findstr /c:"gCapsuleFeaturePkgTokenSpaceGuid\.PcdFlashObbSize" %TempFlashMap%') do @set FLASH_OBB_SIZE=%%i

if %EXTENDEDREGION_BUILD% EQU TRUE (
  @rem hard code, need to refine.
  @set NvsOffset=0x02000000
  set /a FLASH_OBB_OFFSET="!FLASH_OBB_OFFSET!"-"!NvsOffset!"
)
@rem
@rem Creat PostIbbDigest.hash
@REM The order of FVs get reported need to match the FV hash digest calculated in CreateStoredHashFvPpiInfo .
@rem
@if %FSP_SIGNED% EQU TRUE (
  @call %PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Tools\CreatePostIbbHash.py ^
  -i %BUILD_DIR%\FV\ClientBios.fd ^
  --fdf %TempFlashMap% ^
  --fv "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvPostMemoryOffset" ^
  --temp %OBB_HASH_TMP_FOLDER%\PostIbbHashTemp ^
  --openssl-tool-path %OPENSSL_PATH% ^
  -o %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\BpmGen\PostIbbDigest.hash
) else (
  @call %PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Tools\CreatePostIbbHash.py ^
  -i %BUILD_DIR%\FV\ClientBios.fd ^
  --fdf %TempFlashMap% ^
  --fv "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspSOffset" ^
  --fv "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvPostMemoryOffset" ^
  --temp %OBB_HASH_TMP_FOLDER%\PostIbbHashTemp ^
  --openssl-tool-path %OPENSSL_PATH% ^
  -o %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\BpmGen\PostIbbDigest.hash
)
@if %ERRORLEVEL% NEQ 0 (
  @echo !!! WARNING: CreatePostIbbHash.py execute with exit code non zero !!!
)

@if %IBBSIGN% EQU TRUE (
  @if %BOOT_STAGE% EQU 6 (
    @if %ATOM_BUILD% EQU FALSE (
      set STARTUP_ACM_ENABLE=TRUE
    )
  )
  goto IbbSignPostbuildBpmGen
)

@rem
@rem Calculate FIT location, which is located FIT_OFFSET_TO_FSP_TOP size before FSP-T and FSP-O
@rem BAT file has size limitation in number calculation, cut off the first "FF" before calculation, and padding "FF" after that
@rem Use exitcode to convert data to HEX value
@rem
@if %FSP_RESET% EQU FALSE goto SkipFitLocation
@set FIXED_FIT_LOCATION=
@set FIT_OFFSET_TO_FSP_TOP=0x400
@for /f "tokens=4" %%i in ('@findstr /c:"gBoardModuleTokenSpaceGuid\.PcdFlashFvFspOSize" %TempFlashMap%') do @set FLASH_FSPO_SIZE=%%i
@for /f "tokens=4" %%i in ('@findstr /c:"gMinPlatformPkgTokenSpaceGuid\.PcdFlashFvFspTSize" %TempFlashMap%') do @set FLASH_FSPT_SIZE=%%i
@set /A FIXED_FIT_LOCATION= 0x1000000 - 0x%FLASH_FSPO_SIZE:~-6% - 0x%FLASH_FSPT_SIZE:~-6% - %FIT_OFFSET_TO_FSP_TOP%
call cmd /c exit /b %FIXED_FIT_LOCATION%
@set FIXED_FIT_LOCATION=%=exitcode:~-6%
@set FIXED_FIT_LOCATION=0xFF%FIXED_FIT_LOCATION:~-6%
@echo FIXED_FIT_LOCATION: %FIXED_FIT_LOCATION%

:SkipFitLocation

@set FLASH_NVS_OBB_SIZE=
@set /a FLASH_NVS_OBB_SIZE="!FLASH_OBB_OFFSET!+!FLASH_OBB_SIZE!"
@REM
@REM A tricky way to covert FW Version from DEC to HEX
@rem
call cmd /c exit /b !FLASH_NVS_OBB_SIZE!
set FLASH_NVS_OBB_SIZE=0x!=exitcode!

@rem
@rem Split ClientBios.fd to get Obb.bin
@rem
call split -f %BUILD_DIR%\FV\ClientBios.fd -s !FLASH_NVS_OBB_SIZE! -o %OBB_HASH_TMP_FOLDER%\NvsObb.bin -t %OBB_HASH_TMP_FOLDER%\IBBx.bin
call split -f %OBB_HASH_TMP_FOLDER%\NvsObb.bin -s !FLASH_OBB_OFFSET! -o %OBB_HASH_TMP_FOLDER%\Nvs.bin -t %OBB_HASH_TMP_FOLDER%\Obb.bin

@rem
@rem Calculate Obb/Non Fit Payload SHA384 digest and replace DummySha384Digest.bin with it.
@rem
call %OPENSSL_PATH%\openssl.exe dgst -binary -sha384 %OBB_HASH_TMP_FOLDER%\Obb.bin > %OBB_HASH_TMP_FOLDER%\ObbDigest.bin
call %OPENSSL_PATH%\openssl.exe dgst -binary -sha384 %NONFIT_PAYLOAD_BUILD_DIR%\FV\NONFITPAYLOADS.fd > %OBB_HASH_TMP_FOLDER%\NonFitPayloadDigest.bin
@rem
@rem Prepare ObbDigest.ffs
@rem
call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_USER_INTERFACE -n "ObbDigest" -o %OBB_HASH_TMP_FOLDER%\ObbDigest.ui
call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_RAW %OBB_HASH_TMP_FOLDER%\ObbDigest.bin -o %OBB_HASH_TMP_FOLDER%\ObbDigest.raw
call %EDK_TOOLS_BIN%\GenFfs.exe -t EFI_FV_FILETYPE_FREEFORM -g %OBB_HASH_FILE_GUID% -o %OBB_HASH_TMP_FOLDER%\ObbDigest.ffs -i %OBB_HASH_TMP_FOLDER%\ObbDigest.raw -i %OBB_HASH_TMP_FOLDER%\ObbDigest.ui
@rem
@rem Prepare NonFitPayloadDigest.ffs
@rem
call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_USER_INTERFACE -n "NonFitPayloadDigest" -o %OBB_HASH_TMP_FOLDER%\NonFitPayloadDigest.ui
call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_RAW %OBB_HASH_TMP_FOLDER%\NonFitPayloadDigest.bin -o %OBB_HASH_TMP_FOLDER%\NonFitPayloadDigest.raw
call %EDK_TOOLS_BIN%\GenFfs.exe -t EFI_FV_FILETYPE_FREEFORM -g %PAYLOAD_HASH_FILE_GUID% -o %OBB_HASH_TMP_FOLDER%\NonFitPayloadDigest.ffs -i %OBB_HASH_TMP_FOLDER%\NonFitPayloadDigest.raw -i %OBB_HASH_TMP_FOLDER%\NonFitPayloadDigest.ui
@rem
@rem Replace ObbDigest.ffs and NonFitPayloadDigest.ffs in ClientBios.fd (FvPreMemory) with the actual one
@rem
call %EDK_TOOLS_BINWRAPPERS%\FMMT -r %BUILD_DIR%\FV\ClientBios.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D ObbDigest %OBB_HASH_TMP_FOLDER%\ObbDigest.ffs %OBB_HASH_TMP_FOLDER%\ClientBios_temp.fd
call %EDK_TOOLS_BINWRAPPERS%\FMMT -r %OBB_HASH_TMP_FOLDER%\ClientBios_temp.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D NonFitPayloadDigest %OBB_HASH_TMP_FOLDER%\NonFitPayloadDigest.ffs %OBB_HASH_TMP_FOLDER%\ClientBios_temp.fd

if %EXTENDEDREGION_BUILD% EQU TRUE (
  @set EXTENDED_HASH_FILE_GUID=151CB3B2-87B0-4A37-8353-ECB59945E9A0
  copy /b %BUILD_DIR%\FV\ExtendedRegion.bin %OBB_HASH_TMP_FOLDER%\ExtendedRegion.bin
  call %OPENSSL_PATH%\openssl.exe dgst -binary -sha384 %OBB_HASH_TMP_FOLDER%\ExtendedRegion.bin > %OBB_HASH_TMP_FOLDER%\ExtendedDigest.bin
  call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_USER_INTERFACE -n "ExtendedDigest" -o %OBB_HASH_TMP_FOLDER%\ExtendedDigest.ui
  call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_RAW %OBB_HASH_TMP_FOLDER%\ExtendedDigest.bin -o %OBB_HASH_TMP_FOLDER%\ExtendedDigest.raw
  call %EDK_TOOLS_BIN%\GenFfs.exe -t EFI_FV_FILETYPE_FREEFORM -g !EXTENDED_HASH_FILE_GUID! -o %OBB_HASH_TMP_FOLDER%\ExtendedDigest.ffs -i %OBB_HASH_TMP_FOLDER%\ExtendedDigest.raw -i %OBB_HASH_TMP_FOLDER%\ExtendedDigest.ui
  call %EDK_TOOLS_BINWRAPPERS%\FMMT -r %OBB_HASH_TMP_FOLDER%\ClientBios_temp.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D ExtendedDigest %OBB_HASH_TMP_FOLDER%\ExtendedDigest.ffs %OBB_HASH_TMP_FOLDER%\ClientBios.fd
) else (
  copy /y /b %OBB_HASH_TMP_FOLDER%\ClientBios_temp.fd %OBB_HASH_TMP_FOLDER%\ClientBios.fd
)
copy /y /b %OBB_HASH_TMP_FOLDER%\ClientBios.fd %BUILD_DIR%\FV\ClientBios.fd
@rem Keep Obb.bin for Resiliency Obb BGUP build below
copy /y /b %OBB_HASH_TMP_FOLDER%\Obb.bin %BUILD_DIR%\FV\Obb.bin
if exist %OBB_HASH_TMP_FOLDER% rmdir /q /s %OBB_HASH_TMP_FOLDER%
@rem
@rem Capsule update fault tolerance support end
@rem
:SkipFaultToleranceSupport

@rem
@rem FvIbbnp1 & Multi-IBB Hash Generator script
@rem
if exist %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Features\MultiIbbConfig\Tools\MultiIbbHashGenerator.bat (
  call %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Features\MultiIbbConfig\Tools\MultiIbbHashGenerator.bat
)

@rem
@rem ChasmFalls Resiliency support begin
@rem
@rem Below scripts build two BGUPs and replace the dummy ones in FvPreMemory for resiliency support.
@rem The purpose is to support IBB/IBBR sync up and OBB restore when BiosGuard is enabled.
@rem So this section must be executed when:
@rem   1. After all operations on OBB FVs are done.
@rem   2. Before BootGuard BPM/KM is created.
@rem
@rem gIbbRBgslBinGuid
@set IBB_BGSL_FILE_GUID=f53fc14b-025c-4477-9b48-7a1b19f80f30
@rem gObbRBgslBinGuid
@set OBB_BGSL_FILE_GUID=318d30b7-f669-4af2-ade1-e3f84d097bb3
@rem gUcodeBgslFileGuid
@set UCODE_BGSL_FILE_GUID=a503b9d8-d68c-4436-802d-0ac88af635bb
@REM gFitTableSyncBgslFileGuid
@set FIT_TABLE_SYNC_BGSL_FILE_GUID=55dbdd24-5d2e-53fe-d051-e0dc5e84f540
@REM gNonFitPayloadBgslFileGuid
@set PAYLOAD_BGSL_FILE_GUID=cb25fe33-8b2a-4770-85e7-1ee130e22789
@set BGSL_TMP_FOLDER=%BUILD_DIR%\FV\BgslTmp
@set BIOS_GUARD_UPDATE_PACKAGE_PATH=%WORKSPACE_BINARIES%\%PLATFORM_BIN_PACKAGE%\Tools\ToolScripts\BiosGuard\UpdatePackage\
@rem Get Bios version for Bios Gaurd SVN
@set BIOS_GUARD_SVN=
@for /f "usebackq tokens=3" %%i in (`FINDSTR /b /c:"VERSION_MAJOR" %WORKSPACE_ROOT%\%BIOS_ID_FILE%`) do (set BIOS_GUARD_SVN=%%i)
@if %RESILIENCY_BUILD% EQU TRUE (
  @if not exist %BGSL_TMP_FOLDER% mkdir %BGSL_TMP_FOLDER%
  %PYTHON_COMMAND% %BIOS_GUARD_UPDATE_PACKAGE_PATH%\GenerateBGSL.py ^
    genbgsl ^
    -es %BIOS_GUARD_UPDATE_PACKAGE_PATH%\script_BuildBGUP_OBBR_template.bgsl ^
    -is %BIOS_GUARD_UPDATE_PACKAGE_PATH%\script_BuildBGUP_template_Pri_Sec_Sync.bgsl ^
    -os %BIOS_GUARD_UPDATE_PACKAGE_PATH%\script_BuildBGUP_OBBR_template.bgsl ^
    -us %BIOS_GUARD_UPDATE_PACKAGE_PATH%\script_BuildBGUP_Ucode_template.bgsl ^
    -fs %BIOS_GUARD_UPDATE_PACKAGE_PATH%\script_BuildBGUP_FIT_sync_template.bgsl ^
    -ns %BIOS_GUARD_UPDATE_PACKAGE_PATH%\script_BuildBGUP_OBBR_template.bgsl ^
    -t %PrepRELEASE% -m %FLASHMAP_FDF% -o %BIOS_GUARD_UPDATE_PACKAGE_PATH%

  @rem
  @rem Generate Ibb <-> IbbR sync up BGUP.
  @rem
  call %BIOS_GUARD_UPDATE_PACKAGE_PATH%\BuildBGUP_SPI.bat -p script_ibb_ibbr.bgsl -v !BIOS_GUARD_SVN! -use_ftu false
  call %PYTHON_COMMAND% %BIOS_GUARD_UPDATE_PACKAGE_PATH%\BiosGuardCapsule.py -i update_package.BIOS_Guard -o capsule_update_package.BIOS_Guard
  copy /b capsule_update_package.BIOS_Guard + update_package_bgupc.biosguard %BGSL_TMP_FOLDER%\IbbBgsl.bin
  @REM Delete BGUP related build files
  @del script.bin 1>NUL 2>&1
  @del *update_package* 1>NUL 2>&1
  @rem
  @rem Generate OBB_to_SPI BGUP.
  @rem
  call %BIOS_GUARD_UPDATE_PACKAGE_PATH%\BuildBGUP_SPI.bat -d %BUILD_DIR%\FV\Obb.bin -p script_obb.bgsl -v !BIOS_GUARD_SVN! -use_ftu false
  call %PYTHON_COMMAND% %BIOS_GUARD_UPDATE_PACKAGE_PATH%\BiosGuardCapsule.py -i update_package.BIOS_Guard -o capsule_update_package.BIOS_Guard
  copy /b capsule_update_package.BIOS_Guard + update_package_bgupc.biosguard %BGSL_TMP_FOLDER%\ObbRBgsl.bin
  @REM Delete BGUP related build files
  @del script.bin 1>NUL 2>&1
  @del *update_package* 1>NUL 2>&1
  @rem
  @rem Generate NONFITPAYLOAD_to_SPI BGUP.
  @rem
  call %BIOS_GUARD_UPDATE_PACKAGE_PATH%\BuildBGUP_SPI.bat -d %NONFIT_PAYLOAD_BUILD_DIR%\FV\NONFITPAYLOADS.fd -p script_BuildBGUP_NonFitPayload.bgsl -v !BIOS_GUARD_SVN! -use_ftu false
  call %PYTHON_COMMAND% %BIOS_GUARD_UPDATE_PACKAGE_PATH%\BiosGuardCapsule.py -i update_package.BIOS_Guard -o capsule_update_package.BIOS_Guard
  copy /b capsule_update_package.BIOS_Guard + update_package_bgupc.biosguard %BGSL_TMP_FOLDER%\NonFitPayloadBgsl.bin
  @REM Delete BGUP related build files
  @del script.bin 1>NUL 2>&1
  @del *update_package* 1>NUL 2>&1
if %EXTENDEDREGION_BUILD% EQU TRUE (
  @rem
  @rem Generate Extended_to_SPI BGUP.
  @rem
  call %BIOS_GUARD_UPDATE_PACKAGE_PATH%\BuildBGUP_SPI.bat -d %BUILD_DIR%\FV\ExtendedRegion.bin -p script_extendedbios.bgsl -v %BIOS_SVN%
  @echo Create BGUP for ExtendBios
  %PYTHON_COMMAND% %BIOS_GUARD_UPDATE_PACKAGE_PATH%\BiosGuardCapsule.py -i update_package.BIOS_Guard -o capsule_update_package.BIOS_Guard
  copy /b capsule_update_package.BIOS_Guard + update_package_bgupc.biosguard %BGSL_TMP_FOLDER%\ExtendedBgsl.bin
  @REM Delete BGUP related build files
  @del script.bin 1>NUL 2>&1
  @del *update_package* 1>NUL 2>&1

  @set EXTENDED_HASH_FILE_GUID=6F43F2C6-D0C7-40F2-AB33-3A907CFE7ECB
  @rem
  @rem Prepare ExtendedBgsl.ffs
  @rem
  call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_USER_INTERFACE -n "ExtendedBgsl" -o %BGSL_TMP_FOLDER%\ExtendedBgsl.ui
  call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_RAW %BGSL_TMP_FOLDER%\ExtendedBgsl.bin -o %BGSL_TMP_FOLDER%\ExtendedBgsl.raw
  call %EDK_TOOLS_BIN%\GenFfs.exe -t EFI_FV_FILETYPE_FREEFORM -g !EXTENDED_HASH_FILE_GUID! -o %BGSL_TMP_FOLDER%\ExtendedBgsl.ffs -i %BGSL_TMP_FOLDER%\ExtendedBgsl.raw -i %BGSL_TMP_FOLDER%\ExtendedBgsl.ui
)

  @rem
  @rem Generate Ucode <-> UcodeR BGUP.
  @rem
  call %BIOS_GUARD_UPDATE_PACKAGE_PATH%\BuildBGUP_SPI.bat -p script_BuildBGUP_Ucode.bgsl -v !BIOS_GUARD_SVN! -use_ftu false
  call %PYTHON_COMMAND% %BIOS_GUARD_UPDATE_PACKAGE_PATH%\BiosGuardCapsule.py -i update_package.BIOS_Guard -o capsule_update_package.BIOS_Guard
  copy /b capsule_update_package.BIOS_Guard + update_package_bgupc.biosguard %BGSL_TMP_FOLDER%\UcodeBgsl.bin
  @REM Delete BGUP related build files
  @del script.bin 1>NUL 2>&1
  @del *update_package* 1>NUL 2>&1
  @rem
  @rem Generate Fit <-> FitR BGUP.
  @rem
  call %BIOS_GUARD_UPDATE_PACKAGE_PATH%\BuildBGUP_SPI.bat -p script_BuildBGUP_FIT_table_sync.bgsl -v !BIOS_GUARD_SVN! -use_ftu false
  call %PYTHON_COMMAND% %BIOS_GUARD_UPDATE_PACKAGE_PATH%\BiosGuardCapsule.py -i update_package.BIOS_Guard -o capsule_update_package.BIOS_Guard
  copy /b capsule_update_package.BIOS_Guard + update_package_bgupc.biosguard %BGSL_TMP_FOLDER%\FitTableSyncBgsl.bin
  @REM Delete BGUP related build files
  @del script.bin 1>NUL 2>&1
  @del *update_package* 1>NUL 2>&1

  @rem
  @rem Prepare IbbBgsl.ffs
  @rem
  call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_USER_INTERFACE -n "IbbBgsl" -o %BGSL_TMP_FOLDER%\IbbBgsl.ui
  call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_RAW %BGSL_TMP_FOLDER%\IbbBgsl.bin -o %BGSL_TMP_FOLDER%\IbbBgsl.raw
  call %EDK_TOOLS_BIN%\GenFfs.exe -t EFI_FV_FILETYPE_FREEFORM -g %IBB_BGSL_FILE_GUID% -o %BGSL_TMP_FOLDER%\IbbBgsl.ffs -i %BGSL_TMP_FOLDER%\IbbBgsl.raw -i %BGSL_TMP_FOLDER%\IbbBgsl.ui

  @rem
  @rem Prepare ObbRBgsl.ffs
  @rem
  call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_USER_INTERFACE -n "ObbRBgsl" -o %BGSL_TMP_FOLDER%\ObbRBgsl.ui
  call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_RAW %BGSL_TMP_FOLDER%\ObbRBgsl.bin -o %BGSL_TMP_FOLDER%\ObbRBgsl.raw
  call %EDK_TOOLS_BIN%\GenFfs.exe -t EFI_FV_FILETYPE_FREEFORM -g %OBB_BGSL_FILE_GUID% -o %BGSL_TMP_FOLDER%\ObbRBgsl.ffs -i %BGSL_TMP_FOLDER%\ObbRBgsl.raw -i %BGSL_TMP_FOLDER%\ObbRBgsl.ui

  @rem
  @rem Prepare FitTableSyncBgsl.ffs
  @rem
  call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_USER_INTERFACE -n "FitTableSyncBgsl" -o %BGSL_TMP_FOLDER%\FitTableSyncBgsl.ui
  call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_RAW %BGSL_TMP_FOLDER%\FitTableSyncBgsl.bin -o %BGSL_TMP_FOLDER%\FitTableSyncBgsl.raw
  call %EDK_TOOLS_BIN%\GenFfs.exe -t EFI_FV_FILETYPE_FREEFORM -g %FIT_TABLE_SYNC_BGSL_FILE_GUID% -o %BGSL_TMP_FOLDER%\FitTableSyncBgsl.ffs -i %BGSL_TMP_FOLDER%\FitTableSyncBgsl.raw -i %BGSL_TMP_FOLDER%\FitTableSyncBgsl.ui

  @rem
  @rem Prepare UcodeBgsl.ffs
  @rem
  call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_USER_INTERFACE -n "UcodeBgsl" -o %BGSL_TMP_FOLDER%\UcodeBgsl.ui
  call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_RAW %BGSL_TMP_FOLDER%\UcodeBgsl.bin -o %BGSL_TMP_FOLDER%\UcodeBgsl.raw
  call %EDK_TOOLS_BIN%\GenFfs.exe -t EFI_FV_FILETYPE_FREEFORM -g %UCODE_BGSL_FILE_GUID% -o %BGSL_TMP_FOLDER%\UcodeBgsl.ffs -i %BGSL_TMP_FOLDER%\UcodeBgsl.raw -i %BGSL_TMP_FOLDER%\UcodeBgsl.ui

  @rem Prepare NonFitPayloadBgsl.ffs
  @rem
  call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_USER_INTERFACE -n "NonFitPayloadBgsl" -o %BGSL_TMP_FOLDER%\NonFitPayloadBgsl.ui
  call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_RAW %BGSL_TMP_FOLDER%\NonFitPayloadBgsl.bin -o %BGSL_TMP_FOLDER%\NonFitPayloadBgsl.raw
  call %EDK_TOOLS_BIN%\GenFfs.exe -t EFI_FV_FILETYPE_FREEFORM -g %PAYLOAD_BGSL_FILE_GUID% -o %BGSL_TMP_FOLDER%\NonFitPayloadBgsl.ffs -i %BGSL_TMP_FOLDER%\NonFitPayloadBgsl.raw -i %BGSL_TMP_FOLDER%\NonFitPayloadBgsl.ui

  @rem
  @rem Replace IbbBgsl.ffs and ObbRBgsl.ffs in ClientBios.fd (FvPreMemory) with the actual one
  @rem
if %EXTENDEDREGION_BUILD% EQU TRUE (
  copy /y /b %BUILD_DIR%\FV\ClientBios.fd %BUILD_DIR%\FV\ClientBios_Extended.fd
  call %EDK_TOOLS_BINWRAPPERS%\FMMT -r %BUILD_DIR%\FV\ClientBios_Extended.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D ExtendedBgsl %BGSL_TMP_FOLDER%\ExtendedBgsl.ffs %BUILD_DIR%\FV\ClientBios.fd
)
  call %EDK_TOOLS_BINWRAPPERS%\FMMT -r %BUILD_DIR%\FV\ClientBios.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D IbbBgsl %BGSL_TMP_FOLDER%\IbbBgsl.ffs %BGSL_TMP_FOLDER%\ClientBios_tmp.fd
  call %EDK_TOOLS_BINWRAPPERS%\FMMT -r %BGSL_TMP_FOLDER%\ClientBios_tmp.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D FitTableSyncBgsl %BGSL_TMP_FOLDER%\FitTableSyncBgsl.ffs %BGSL_TMP_FOLDER%\ClientBios_tmp.fd
  call %EDK_TOOLS_BINWRAPPERS%\FMMT -r %BGSL_TMP_FOLDER%\ClientBios_tmp.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D UcodeBgsl %BGSL_TMP_FOLDER%\UcodeBgsl.ffs %BGSL_TMP_FOLDER%\ClientBios_tmp.fd
  call %EDK_TOOLS_BINWRAPPERS%\FMMT -r %BGSL_TMP_FOLDER%\ClientBios_tmp.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D NonFitPayloadBgsl %BGSL_TMP_FOLDER%\NonFitPayloadBgsl.ffs %BGSL_TMP_FOLDER%\ClientBios_tmp.fd
  call %EDK_TOOLS_BINWRAPPERS%\FMMT -r %BGSL_TMP_FOLDER%\ClientBios_tmp.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D ObbRBgsl %BGSL_TMP_FOLDER%\ObbRBgsl.ffs %BGSL_TMP_FOLDER%\ClientBios.fd
  copy /y /b %BGSL_TMP_FOLDER%\ClientBios.fd %BUILD_DIR%\FV\ClientBios.fd

  @REM Clean-Up the intermediate files
  if exist %BGSL_TMP_FOLDER% rmdir /q /s %BGSL_TMP_FOLDER%
  del %BIOS_GUARD_UPDATE_PACKAGE_PATH%\script_ibbr.bgsl
  del %BIOS_GUARD_UPDATE_PACKAGE_PATH%\script_ibb_ibbr.bgsl
  del %BIOS_GUARD_UPDATE_PACKAGE_PATH%\script_obb.bgsl
  del %BIOS_GUARD_UPDATE_PACKAGE_PATH%\script_extendedbios.bgsl
  del %BIOS_GUARD_UPDATE_PACKAGE_PATH%\script_BuildBGUP_Ucode.bgsl
  del %BIOS_GUARD_UPDATE_PACKAGE_PATH%\script_BuildBGUP_FIT_table_sync.bgsl
  del %BIOS_GUARD_UPDATE_PACKAGE_PATH%\script_BuildBGUP_NonFitPayload.bgsl
)
@rem
@rem ChasmFalls Rresiliency support end
@rem

@REM
REM Run FitGen tool to generate FIT
@REM
copy %BUILD_DIR%\FV\ClientBios.fd %BUILD_DIR%\FV\ClientBios_FitPre.fd /Y

@REM
@REM FitGen parameter usage reference:
@REM   -D It is FD file instead of FV file.
@REM   -F <FitTablePointerOffset>
@REM   -L <MicrocodeSlotSize> <MicrocodeFfsGuid>
@REM   -I <BiosInfoGuid>
@REM   -NA No 0x800 aligned Microcode requirement. No -NA means Microcode is 0x800 aligned.
@REM   -O RecordType <RecordDataAddress RecordDataSize>|<RESERVE RecordDataSize>
@REM   -P RecordType <IndexPort DataPort Width Bit Index>
@REM   [-T] <FixedFitLocation>
@REM
@REM For slot mode, the SLOT_SIZE envir variable already defined in prep.bat script
@REM
@REM When Reset Vector is in FSP, FIT table will be put at a specific offset to Bios top address
@REM
@set BuildFitArgs=-D ^
  %BUILD_DIR%\FV\ClientBios.fd ^
  %BUILD_DIR%\FV\ClientBios.fd ^
  -F 0x40 ^
  -NA ^
  -L %SLOT_SIZE% %MICROCODE_ARRAY_FFS_GUID% ^
  -I %BIOS_INFO_GUID% ^
  %STARTUP_AC_PARA% ^
  -O 0x0B RESERVE 0x400

@if %FSP_RESET% EQU TRUE (
  @if %FSP_SIGNED% EQU TRUE (
    @rem Calculate FBM location, which is located FIT_OFFSET_TO_FSP_TOP before the start of FSP-T FV
    @set FBM_OFFSET_TO_FIT=0x1C00
    @set /A FIXED_FBM_LOCATION=0x!FIXED_FIT_LOCATION:~-6! - !FBM_OFFSET_TO_FIT!
    call cmd /c exit /b !FIXED_FBM_LOCATION!
    @set FIXED_FBM_LOCATION=!=exitcode:~-6!
    @set FIXED_FBM_LOCATION=0xFF!FIXED_FBM_LOCATION:~-6!
    @echo FIXED_FBM_LOCATION: !FIXED_FBM_LOCATION!

    @set BuildFitArgs=%BuildFitArgs% ^
      -O 0x0C RESERVE 0x800 ^
      -O 0x0D !FIXED_FBM_LOCATION! 0x800 ^
      -P 0xA 0x70 0x71 0x1 0x4 0x2a ^
      -T %FIXED_FIT_LOCATION%
  ) else (
    @set BuildFitArgs=%BuildFitArgs% ^
      -O 0x0C RESERVE 0x800 ^
      -P 0xA 0x70 0x71 0x1 0x4 0x2a ^
      -T %FIXED_FIT_LOCATION%
  )
  goto FitGenCall
)

@set BuildFitArgs=%BuildFitArgs% ^
  -O 0x0C RESERVE 0x600 ^
  -P 0xA 0x70 0x71 0x1 0x4 0x2a

:FitGenCall
%EDK_TOOLS_BIN%\FitGen.exe %BuildFitArgs%
@if %errorlevel% NEQ 0 (
  echo Error: FitGen failure
  set SCRIPT_ERROR=1
  goto EndPostBuild
)

copy /b %BUILD_DIR%\FV\ClientBios.fd %BUILD_DIR%\FV\ClientBios_Post_Fit.fd /Y

:SkipGenFitTable

@if %SECURE_BOOT_ENABLE% EQU FALSE (
  echo Secure Boot is disable
  goto SecureBootDisable
)

:SecureBootDisable

:IbbSignPostbuildBpmGen

setlocal ENABLEDELAYEDEXPANSION
@set FSPM_LOADING_POLICY=0x0
@if %STARTUP_ACM_ENABLE% EQU TRUE (
  @REM
  @REM Gerneate Bsis binary for both FSP Signing and non FSP Signing cases.
  @REM In FSP Signing case, Bsis binary includes data for FSP verification;
  @REM In non FSP Signing case, Bsis binary only includes FSP-T UPD and BSP entry
  @REM

  @echo generate BSIS binary
  @if %FSP_SIGNED% EQU TRUE (
    @if %MULTI_IBB_BUILD% EQU TRUE (
      @set FSPM_LOADING_POLICY=0x0
    ) else (
      @set FSPM_LOADING_POLICY=0x1
    )
  )
  @if %FSPM_COMPRESSED% EQU TRUE (
    @set /A FSPM_LOADING_POLICY= %FSPM_LOADING_POLICY% + 0x2
  )

  @call %PYTHON_COMMAND% %WORKSPACE_COMMON%\%PLATFORM_BOARD_PACKAGE%\BoardSupport\Tools\BsssGen\BsssGen.py ^
    -Fd %WORKSPACE_ROOT%\%BUILD_DIR%\FV\CLIENTBIOS.fd ^
    -FspmLP !FSPM_LOADING_POLICY! ^
    -s %FSP_SIGNED% ^
    -o %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\BpmGen\bsis.bin ^
    -d %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\BpmGen\bsis.bin
  @if %errorlevel% NEQ 0 (
    @echo Failure in running BsssGen.py
    set SCRIPT_ERROR=1
    goto EndPostBuild
  )
) else (
  @REM
  @REM Gerneate BSPM binary in No ACM case, BSPM binary is generated and patched into the BIOS image at 4G - 4K address
  @REM

  @echo generate BSPM binary
  @call %PYTHON_COMMAND% %WORKSPACE_COMMON%\%PLATFORM_BOARD_PACKAGE%\BoardSupport\Tools\BsssGen\BsssGen.py ^
    -Fd %WORKSPACE_ROOT%\%BUILD_DIR%\FV\CLIENTBIOS.fd ^
    -FspmLP !FSPM_LOADING_POLICY! ^
    -s %FSP_SIGNED% ^
    -p %WORKSPACE_ROOT%\%BUILD_DIR%\FV\fspe.bin
  @if %errorlevel% NEQ 0 (
    @echo Failure in running BsssGen.py
    set SCRIPT_ERROR=1
    goto EndPostBuild
  )
  goto :NoAcm
)

@REM
@echo Create BTG keymanifest and Boot Policy Manifest
@REM

@rem
  if exist %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\BpmGen\postbuildBpmGen.bat (
    call %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\BpmGen\postbuildBpmGen.bat ClientBios  ClientBios
    if !SCRIPT_ERROR! NEQ 0 goto :NoAcm
  )
@if %RESILIENCY_BUILD% EQU TRUE (
  @call %PYTHON_COMMAND% %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%\Tools\GenFitTableBackup\GenFitTableBackup.py ^
  -i %BUILD_DIR%\FV\ClientBios.fd ^
  -o %BUILD_DIR%\FV\ClientBios.fd ^
  -T 0x!FIXED_FIT_LOCATION:~-6!

  @if %errorlevel% NEQ 0 (
    echo Error: GenFitTableBackup failure
    set SCRIPT_ERROR=1
    goto EndPostBuild
  )
)

if exist %TempFlashMap% del %TempFlashMap%

@REM Remove bsis.bin
IF exist %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\BpmGen\bsis.bin (
  del %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\BpmGen\bsis.bin
)

@rem NOTE: postbuildBpmGen.bat will copy the updated .fd with KEYM/BPM as ClientBios.fd (passed as second parameter)
@rem
@rem The invocation to the BpmGen tool should be the last step of the build process, with no further modifications to the BIOS
@rem binary; any change will invalidate the reference hashes computed by BpmGen and required for verified boot flows.
@rem The only exception to this case being: replacing the pre-production ACM binaries with production signed ACM when creating
@rem the Prod_*.rom BIOS binaries.

@if %IBBSIGN% EQU TRUE (
  copy /y /b %BUILD_DIR%\FV\ClientBios.fd %WORKSPACE%\RomImages\BIOS_Resign.rom
  goto EndPostBuild
)

:NoAcm

if not exist %WORKSPACE_ROM% mkdir %WORKSPACE_ROM%

@echo ---Create ROM and Simics images---
call %WORKSPACE_BINARIES%\%PLATFORM_BIN_PACKAGE%\Tools\InternalOnly\RomImage\SetupRomDirs.bat

@if %errorlevel% NEQ 0 (
  @echo !!! ERROR !!! Create ROM and Simics images !!!
  set SCRIPT_ERROR=1
  goto EndPostBuild
)

@echo ---Prepare the BIOS Decoder Dictionaries for the Release Build and Catalog Build BIOS---
@if %CATALOG_RELEASE% EQU TRUE (
  @rem Catalog Dictionary Generation script will parse through the source and preprocessed files to prepare the entries for the RELEASE BIOS decoder.
  call %PYTHON_COMMAND% %CATALOG_DICT_GENERATOR%
  @if %errorlevel% NEQ 0 (
      @echo !!! ERROR !!! BIOS Decoder Dictionary generation failed  !!!
      set SCRIPT_ERROR=1
      goto EndPostBuild
  )
  @rem Placing the Release Build and Catalog Build BIOS Decoder files from the Build to the workspace folder.
  @copy /Y /b %RELEASEBIOS_DECODER_XML% %WORKSPACE%\BIOS_Decoder_Release_%BIOS_PREFIX%_%BIOS_MAJOR_VERSION%_%BIOS_MIN_VERSION%.xml > NUL 2>&1
  @copy /Y /b %CATALOGBIOS_DECODER_XML% %WORKSPACE%\BIOS_Decoder_Catalog_%BIOS_PREFIX%_%BIOS_MAJOR_VERSION%_%BIOS_MIN_VERSION%.xml > NUL 2>&1
)


:EndPostBuild
@set STARTUP_AC_MODULE_GUID=
@set BIOS_INFO_GUID=
@set STARTUP_AC_PARA=
@popd
endlocal && set SCRIPT_ERROR=%SCRIPT_ERROR%
@if %SCRIPT_ERROR% NEQ 0 (
  @echo ERROR: The BIOS PostBuild has failed!
  @exit /b 1
)
