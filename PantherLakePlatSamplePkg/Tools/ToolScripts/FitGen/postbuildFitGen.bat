@REM @file
@REM
@REM  @copyright
@REM  INTEL CONFIDENTIAL
@REM  Copyright (C) 2010 Intel Corporation.
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
@REM #    postbuildFitGen.bat
@REM #
@REM #  Abstract:
@REM #
@REM #    Post build FitGen script.
@REM #
@REM #--*/

@set SCRIPT_ERROR=0

@set BIOS_INFO_GUID=4A4CA1C6-871C-45bb-8801-6910A7AA5807
@set STARTUP_AC_MODULE_GUID=26FDAA3D-B7ED-4714-8509-EECF1593800D

@findstr /i %STARTUP_AC_MODULE_GUID% %BUILD_DIR%\FV\FW_BINARIES_FV_SIGNED.inf > NUL

if errorlevel 1 goto NoStartupAcm
@set STARTUP_AC_PARA=-S %STARTUP_AC_MODULE_GUID%
:NoStartupAcm

copy %BUILD_DIR%\FV\ClientBios.fd %BUILD_DIR%\FV\ClientBios_ORG.fd /Y

@REM
@REM FitGen parameter usage reference:
@REM   -D It is FD file instead of FV file.
@REM   -F <FitTablePointerOffset>
@REM   -I <BiosInfoGuid>
@REM   -NA No 0x800 aligned Microcode requirement. No -NA means Microcode is 0x800 aligned.
@REM   -O RecordType <RecordDataAddress RecordDataSize>|<RESERVE RecordDataSize>
@REM   -P RecordType <IndexPort DataPort Width Bit Index>
@REM

%EDK_TOOLS_BIN%\FitGen.exe -D %BUILD_DIR%\FV\ClientBios.fd %BUILD_DIR%\FV\ClientBios_FIT.fd -F 0x40 -NA -I %BIOS_INFO_GUID% %STARTUP_AC_PARA% -O 0x0C RESERVE 0x600 -O 0x0B RESERVE 0x400 -P 0xA 0x70 0x71 0x1 0x4 0x2A
@if errorlevel 1 (
  echo Error: ClientBios_FIT not found
  set SCRIPT_ERROR=1
  goto :EOF
)

@if not errorlevel 1 (
  @REM Work around to incorporate Bootguard ACM since fit gen does not support new header structure
  @if "%TargetOption%" == "" (
    @echo TargetOption is not found!
    @set SCRIPT_ERROR=1
    goto :EOF
  ) else (
    @if /I "%TargetOption%" == "adl" (
      echo Using KBL ACM
      copy %BUILD_DIR%\FV\ClientBios_FIT.fd %BUILD_DIR%\FV\CLIENTBIOS.fd /Y
    ) else (
      echo Using PO ACM
      %WORKSPACE_BINARIES%\%PLATFORM_BIN_PACKAGE%\Tools\InternalOnly\BootGuard\anc_bios_gen.exe -b %BUILD_DIR%\FV\ClientBios_FIT.fd -a  %WORKSPACE_BINARIES%\%PLATFORM_BIN_PACKAGE%\Binaries\BootGuard\ACM\StartupAcm.bin -o %BUILD_DIR%\FV\ClientBios_FIT_PO.fd
      copy %BUILD_DIR%\FV\ClientBios_FIT_PO.fd %BUILD_DIR%\FV\ClientBios_FIT.fd /Y
    )
  )
)


@set STARTUP_AC_MODULE_GUID=
@set BIOS_INFO_GUID=
@set STARTUP_AC_PARA=
