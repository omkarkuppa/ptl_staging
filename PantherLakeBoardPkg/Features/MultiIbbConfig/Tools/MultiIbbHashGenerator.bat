@REM @file
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
@REM @par Specification Reference:
@REM

@REM #
@REM #  Module Name:
@REM #
@REM #    MultiIbbHashGenerator.bat
@REM #
@REM #  Abstract:
@REM #
@REM #    Multi-IBB Hash Generator script
@REM #
@REM #--*/

@rem
@rem Generate digest for IBBn+1, then replace the dummy one
@rem
@echo "#### IBBnp1 hash values generation begin ####"
@echo %CD%

@set IBB_HASH_TMP_FOLDER=%BUILD_DIR%\FV\MultiIbbTmp
@if not exist %IBB_HASH_TMP_FOLDER% mkdir %IBB_HASH_TMP_FOLDER%
echo %IBB_HASH_TMP_FOLDER%
@set FV_IBBNP1_HASH_FILE_GUID=87A71A9A-327F-4117-B73C-2B31D7748435

@rem
@rem Calculate SHA384 digest and replace DummySha384Digest.bin with it.
@rem
call %OPENSSL_PATH%\openssl.exe dgst -binary -sha384 %BUILD_DIR%\FV\FvIbbnp1.Fv > %IBB_HASH_TMP_FOLDER%\FvIbbnp1Digest.bin

@rem
@rem Prepare FvIbbnp1Digest.ffs
@rem
call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_USER_INTERFACE -n "FvIbbnp1Digest" -o %IBB_HASH_TMP_FOLDER%\FvIbbnp1Digest.ui
call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_RAW %IBB_HASH_TMP_FOLDER%\FvIbbnp1Digest.bin -o %IBB_HASH_TMP_FOLDER%\FvIbbnp1Digest.raw
call %EDK_TOOLS_BIN%\GenFfs.exe -t EFI_FV_FILETYPE_FREEFORM -g %FV_IBBNP1_HASH_FILE_GUID% -o %IBB_HASH_TMP_FOLDER%\FvIbbnp1Digest.ffs -i %IBB_HASH_TMP_FOLDER%\FvIbbnp1Digest.raw -i %IBB_HASH_TMP_FOLDER%\FvIbbnp1Digest.ui

@rem
@rem Replace FvIbbnp1Diges.ffs in CLIENTBIOS.fd with the actual one
@rem
call %EDK_TOOLS_BINWRAPPERS%\FMMT -r %WORKSPACE%\%BUILD_DIR%\FV\CLIENTBIOS.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D FvIbbnp1Digest %IBB_HASH_TMP_FOLDER%\FvIbbnp1Digest.ffs %IBB_HASH_TMP_FOLDER%\CLIENTBIOS.fd
copy /y /b %IBB_HASH_TMP_FOLDER%\CLIENTBIOS.fd %WORKSPACE%\%BUILD_DIR%\FV\CLIENTBIOS.fd

@rem Keep Digest.bin for checking
copy /y /b %IBB_HASH_TMP_FOLDER%\FvIbbnp1Digest.bin %WORKSPACE%\%BUILD_DIR%\FV\FvIbbnp1Digest.bin

if %MULTI_IBB_BUILD% EQU FALSE goto SkipMultiIbbHashGeneration
    @rem
    @rem Generate digest for IBB1, IBB2, ... IBBn, then replace the dummy one
    @rem
    @echo "#### Multi-IBB hash values generation begin ####"
    @echo %CD%

    @set PLATFORM_FSP_BIN_PACKAGE=PantherLakeFspBinPkg
    @set FLASHMAP_FDF=%WORKSPACE%\Intel\%PLATFORM_BOARD_PACKAGE%\Include\Fdf\FlashMapIncludeMultiIbb.fdf
    @echo %FLASHMAP_FDF%
    @echo %TARGET%

    @set FV_IBB1_HASH_FILE_GUID=1E8B76AC-0621-4725-8951-ACBC72271826
    @set FV_FSPM_HASH_FILE_GUID=6E31A9DA-3548-47FE-9E92-8092DA714446

    @rem
    @rem Multi-IBB: Fsp case, to generate FspM.bin. Calculate FspM size and offset for splitting BIOS ROM
    @rem
    @for /f "tokens=4" %%i in ('@findstr /c:"gMinPlatformPkgTokenSpaceGuid\.PcdFlashFvFspMOffset" %FLASHMAP_FDF%') do (
        @set FLASH_FSPM_OFFSET=%%i
        @if /I "%TARGET%" == "DEBUG" goto :continue
    )
    :continue
    @echo FLASH_FSPM_OFFSET=%FLASH_FSPM_OFFSET%

    @for /f "tokens=4" %%i in ('@findstr /c:"gMinPlatformPkgTokenSpaceGuid\.PcdFlashFvFspMSize" %FLASHMAP_FDF%') do (
        @set FLASH_FSPM_SIZE=%%i
        @if /I "%TARGET%" == "DEBUG" goto :continue
    )
    :continue
    @echo FLASH_FSPM_SIZE=%FLASH_FSPM_SIZE%

    @set TEMP_OFFSET=
    @set /a TEMP_OFFSET="!FLASH_FSPM_OFFSET!+!FLASH_FSPM_SIZE!"
    @rem
    @rem Covert FW Version from DEC to HEX
    @rem
    @call cmd /c exit /b !TEMP_OFFSET!
    @echo %TEMP_OFFSET%
    @set TEMP_OFFSET=0x!=exitcode!

    @rem
    @rem Split ClientBios.fd to get FspM.bin
    @rem
    call split -f %WORKSPACE%\%BUILD_DIR%\FV\ClientBios.fd -s !TEMP_OFFSET! -o %IBB_HASH_TMP_FOLDER%\IncludeFspM.bin -t %IBB_HASH_TMP_FOLDER%\Temp.bin
    call split -f %IBB_HASH_TMP_FOLDER%\IncludeFspM.bin -s !FLASH_FSPM_OFFSET! -o %IBB_HASH_TMP_FOLDER%\Others.bin -t %IBB_HASH_TMP_FOLDER%\FspM.bin

    @rem
    @rem Calculate SHA384 digest and replace DummySha384Digest.bin with it.
    @rem
    call %OPENSSL_PATH%\openssl.exe dgst -binary -sha384 %BUILD_DIR%\FV\FvIbb1.Fv > %IBB_HASH_TMP_FOLDER%\FvIbb1Digest.bin
    call %OPENSSL_PATH%\openssl.exe dgst -binary -sha384 %IBB_HASH_TMP_FOLDER%\FspM.bin > %IBB_HASH_TMP_FOLDER%\FvFspMDigest.bin

    @rem
    @rem Prepare FvIbb1Digest.ffs, FvFspMDigest.ffs
    @rem
    call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_USER_INTERFACE -n "FvIbb1Digest" -o %IBB_HASH_TMP_FOLDER%\FvIbb1Digest.ui
    call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_RAW %IBB_HASH_TMP_FOLDER%\FvIbb1Digest.bin -o %IBB_HASH_TMP_FOLDER%\FvIbb1Digest.raw
    call %EDK_TOOLS_BIN%\GenFfs.exe -t EFI_FV_FILETYPE_FREEFORM -g %FV_IBB1_HASH_FILE_GUID% -o %IBB_HASH_TMP_FOLDER%\FvIbb1Digest.ffs -i %IBB_HASH_TMP_FOLDER%\FvIbb1Digest.raw -i %IBB_HASH_TMP_FOLDER%\FvIbb1Digest.ui

    call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_USER_INTERFACE -n "FvFspMDigest" -o %IBB_HASH_TMP_FOLDER%\FvFspMDigest.ui
    call %EDK_TOOLS_BIN%\GenSec.exe -s EFI_SECTION_RAW %IBB_HASH_TMP_FOLDER%\FvFspMDigest.bin -o %IBB_HASH_TMP_FOLDER%\FvFspMDigest.raw
    call %EDK_TOOLS_BIN%\GenFfs.exe -t EFI_FV_FILETYPE_FREEFORM -g %FV_FSPM_HASH_FILE_GUID% -o %IBB_HASH_TMP_FOLDER%\FvFspMDigest.ffs -i %IBB_HASH_TMP_FOLDER%\FvFspMDigest.raw -i %IBB_HASH_TMP_FOLDER%\FvFspMDigest.ui

    @rem
    @rem Replace FvIbb1Digest.ffs, FvFspMDigest.ffs in ClientBios.fd with the actual one
    @rem
    call %EDK_TOOLS_BINWRAPPERS%\FMMT -r %WORKSPACE%\%BUILD_DIR%\FV\ClientBios.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D FvIbb1Digest %IBB_HASH_TMP_FOLDER%\FvIbb1Digest.ffs %IBB_HASH_TMP_FOLDER%\ClientBios.fd
    copy /y /b %IBB_HASH_TMP_FOLDER%\ClientBios.fd %WORKSPACE%\%BUILD_DIR%\FV\ClientBios.fd
    call %EDK_TOOLS_BINWRAPPERS%\FMMT -r %WORKSPACE%\%BUILD_DIR%\FV\ClientBios.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D FvFspMDigest %IBB_HASH_TMP_FOLDER%\FvFspMDigest.ffs %IBB_HASH_TMP_FOLDER%\ClientBios.fd
    copy /y /b %IBB_HASH_TMP_FOLDER%\ClientBios.fd %WORKSPACE%\%BUILD_DIR%\FV\ClientBios.fd

    @rem Keep Digest.bin for checking
    copy /y /b %IBB_HASH_TMP_FOLDER%\FvIbb1Digest.bin %WORKSPACE%\%BUILD_DIR%\FV\FvIbb1Digest.bin
    copy /y /b %IBB_HASH_TMP_FOLDER%\FvFspMDigest.bin %WORKSPACE%\%BUILD_DIR%\FV\FvFspMDigest.bin
:SkipMultiIbbHashGeneration

if exist %IBB_HASH_TMP_FOLDER% rmdir /q /s %IBB_HASH_TMP_FOLDER%
@rem
@rem Digest generation completed
@rem
