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

@REM
@REM In FSP Reset, place FIT at 0x400 before FSP-T Base, this location need update when
@REM FSP-O or FSP-T sizes change.
@REM FSP-T Size: 0x0001C000 (DEBUG)    0x0000E000 (RELEASE)
@REM FSP-O Size: 0x00012000 (DEBUG)    0x0000E000 (RELEASE)
@REM ------------------------- 4G -------------------------
@REM
@if /I "%TARGET%" == "RELEASE" (
  @set FIXED_FIT_LOCATION=0xFFFE3C00
) else (
  @set FIXED_FIT_LOCATION=0xFFFD1C00
)

@if not defined PYTHON_COMMAND (
  set PYTHON_COMMAND=py -3
)

@if not defined WORKSPACE (
  echo.
  echo !!! ERROR !!! WORKSPACE must be defined before executing postbuild.bat !!!
  echo.
  set SCRIPT_ERROR=1
  goto :EOF
)

@if not defined WORKSPACE_PLATFORM (
  echo.
  echo !!! ERROR !!! WORKSPACE_PLATFORM must be defined before executing postbuild.bat !!!
  echo.
  set SCRIPT_ERROR=1
  goto :EOF
)

pushd %WORKSPACE%

@if not %CD% == %WORKSPACE% (
  echo.
  echo !!! ERROR !!! postbuild.bat is not executed under %WORKSPACE%
  echo.
  set SCRIPT_ERROR=1
  goto :EOF
)

@REM
@REM Set env vars for BIOS ROM filename to align with OWR spec; see PTL RCR.
@REM
@set BIOS_ID_FILE=%WORKSPACE_COMMON%\PantherLakeBoardPkg\BiosId.env
for /f "usebackq tokens=3" %%i in (`FINDSTR /b /c:"VERSION_MAJOR" %BIOS_ID_FILE%`) do (
  for /f "usebackq tokens=3" %%j in (`FINDSTR /b /c:"VERSION_MINOR" %BIOS_ID_FILE%`) do (
      @set BIOS_MAJOR_VERSION=%%i
      @set BIOS_MIN_VERSION=%%j
    )
  )
@set BIOS_ROM_FILENAME_INTEXT=External
@set BUILD_TYPE=%TARGET%

@if not "%TOOL_CHAIN_TAG%"=="%TOOL_CHAIN_TAG:VS=%" (
  @set ROM_FILENAME_COMPILER=VS
) else if not "%TOOL_CHAIN_TAG%"=="%TOOL_CHAIN_TAG:GCC=%" (
  @set ROM_FILENAME_COMPILER=GCC
) else if not "%TOOL_CHAIN_TAG%"=="%TOOL_CHAIN_TAG:CLANG=%" (
  @set ROM_FILENAME_COMPILER=CLANGPDB
) else (
  @set ROM_FILENAME_COMPILER=UNKNOWN
)

@set BIOS_PREFIX=PTL_P_OpenBoard
@set ROM_FILENAME_SPECIAL_BUILD_TYPE=
@if defined FSP_SIGNED (
  @set FSP_SIGNED=TRUE
  @set ROM_FILENAME_SPECIAL_BUILD_TYPE=_FSPSIGNED
) else (
  @set FSP_SIGNED=FALSE
)

@set BIOS_BUILD_TAG=%BIOS_PREFIX%_%BIOS_ROM_FILENAME_INTEXT%_%BIOS_MAJOR_VERSION%.%BIOS_MIN_VERSION%_%FSP_MODE%%ROM_FILENAME_SPECIAL_BUILD_TYPE%_%ROM_FILENAME_COMPILER%_%BUILD_TYPE%


@REM
@REM Run FitGen tool to generate FIT
@REM
@set BIOS_INFO_GUID=4A4CA1C6-871C-45bb-8801-6910A7AA5807
@set MICROCODE_ARRAY_FFS_GUID=197DB236-F856-4924-90F8-CDF12FB875F3
@REM
@REM FitGen parameter usage reference:
@REM   -D It is FD file instead of FV file.
@REM   -F <FitTablePointerOffset>
@REM   -L <MicrocodeSlotSize> <MicrocodeFfsGuid>
@REM   -I <BiosInfoGuid>
@REM   -NA No 0x800 aligned Microcode requirement. No -NA means Microcode is 0x800 aligned.
@REM   -O RecordType <RecordDataAddress RecordDataSize>|<RESERVE RecordDataSize>
@REM   -P RecordType <IndexPort DataPort Width Bit Index>
@REM
@REM For slot mode, the SLOT_SIZE envir variable already defined in prep.bat script
@REM

%EDK_TOOLS_BIN%\FitGen.exe ^
-D ^
%BUILD_DIR%\FV\%BOARD%.fd ^
%BUILD_DIR%\FV\%BOARD%.fd ^
-F 0x40 ^
-NA ^
-L %SLOT_SIZE% %MICROCODE_ARRAY_FFS_GUID% ^
-I %BIOS_INFO_GUID% ^
-O 0x0C RESERVE 0x600 ^
-O 0x0B RESERVE 0x400 ^
-P 0xA 0x70 0x71 0x1 0x4 0x2a ^
-T %FIXED_FIT_LOCATION%


@if %FSP_RESET% EQU TRUE (
  @REM
  @echo FSPE binary is generated and patched into the BIOS image of 4G - 4K address
  @REM

  @call %PYTHON_COMMAND% %WORKSPACE_COMMON%\PantherLakeOpenBoardPkg\BoardSupport\Tools\BsssGen\BsssGen.py ^
    -Fd %BUILD_DIR%\FV\%BOARD%.fd ^
    -FspmLP 0x0 ^
    -p %BUILD_DIR%\FV\fspe.bin
  @if %errorlevel% NEQ 0 (
    @echo Failure in running BsssGen.py
    set SCRIPT_ERROR=1
    goto :EOF
  )
)


if not exist %WORKSPACE%\RomImages\%PLATFORM_BOARD_PACKAGE%\ (
  MKDIR %WORKSPACE%\RomImages\%PLATFORM_BOARD_PACKAGE%\
)
@set BIOS_ROM_FILE_PATH=%WORKSPACE%\RomImages\%PLATFORM_BOARD_PACKAGE%\BIOS_%BIOS_BUILD_TAG%.rom

copy /y /b %BUILD_DIR%\FV\%BOARD%.fd %BIOS_ROM_FILE_PATH%

@if not exist %BIOS_ROM_FILE_PATH% (
  echo "ERROR - ROM image is not available"
  exit /b 1
)


cd %WORKSPACE_PLATFORM%\%PLATFORM_BOARD_PACKAGE%

@rem
@rem Create Simics image
@rem
@set PLATFORM_BIN_PACKAGE=PantherLakeBinPkg
@set IFWI_PATCHER=%WORKSPACE_BINARIES%\%PLATFORM_BIN_PACKAGE%\Tools\InternalOnly\SimicsImage\IfwiPatcher\IfwiPatcher.py
@set IFWI_BIN=%WORKSPACE_BINARIES%\%PLATFORM_BIN_PACKAGE%\Tools\InternalOnly\SimicsImage\PreSiIfwi\PTL_P_IFWI.bin
@set OUTPUT_SIMICS_BIN=%WORKSPACE%\RomImages\PantherLakeOpenBoardSimics\IFWI_%BIOS_BUILD_TAG%_Simics.bin
@if exist %IFWI_PATCHER% (
  @REM
  @REM PreProduction IFWI
  @REM
  call %PYTHON_COMMAND% %IFWI_PATCHER% --ifwi %IFWI_BIN% --bios %BIOS_ROM_FILE_PATH% --output %OUTPUT_SIMICS_BIN%
  @if not exist %OUTPUT_SIMICS_BIN% (
    echo "ERROR Simics IFWI image generation failed"
    exit /b 1
  )
  @echo =========================================================================
  @echo Simics IFWI image generated successfully !
  @echo =========================================================================
)