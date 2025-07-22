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
@REM #    postbuildBpmGen.bat
@REM #
@REM #  Abstract:
@REM #
@REM #    Post build BpmGen script.
@REM #
@REM #--*/


@set SCRIPT_ERROR=0

@set KM_REVOCATION=0
@set KM_ID=0x01

if not exist %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\BpmGen\*.pem (
  @echo "#### Can not find *.pem, bypass BpmGen ####"
  @exit /b 0
)

if not exist %WORKSPACE_BINARIES%\Tools\InternalOnly\BpmGen2\BpmGen2.exe (
  @echo "#### Can not find BpmGen2.exe, bypass BpmGen ####"
  @exit /b 0
)

if not exist %WORKSPACE%\RomImages mkdir %WORKSPACE%\RomImages
@set BPM_OUT_DIR=%WORKSPACE%\RomImages

echo "#### BpmGen2:  Generating KeyManifest.bin ####"
pushd .
echo %CD%

set BPMGEN2=%WORKSPACE_BINARIES%\Tools\InternalOnly\BpmGen2\BpmGen2.exe

cd %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\BpmGen
echo %CD%
@REM BPMGEN2 KMGEN -KEY pubkey.pem BPM -KM %WORKSPACE%\%BUILD_DIR%\FV\KeyManifest.bin -SIGNKEY keyprivkey.pem -SIGNPUBKEY keypubkey.pem -KMID %KM_ID% -SVN %KM_REVOCATION% -d:2 >bpmgen2_km.txt
%BPMGEN2% KMGEN -KEY pubkey.pem BPM -KM %WORKSPACE%\%BUILD_DIR%\FV\KeyManifest.bin -SIGNKEY 3k_key_private.pem  -SIGHASHALG SHA384 -SCHEME RSAPSS -KMKHASH SHA384 -KMID %KM_ID% -SVN %KM_REVOCATION% -d:2 >%BPM_OUT_DIR%\bpmgen2_km.txt
@if %ERRORLEVEL% NEQ 0 (
  popd
  echo "#### Error generating KM file #####"
  set SCRIPT_ERROR=1
  goto Cleanup
)
@if %FSP_SIGNED% EQU FALSE (
  @set BPM_PARAMS_FILE=bpmgen2_fsp.params
) else (
  @set BPM_PARAMS_FILE=bpmgen2_signed_fsp.params
)

@if %FSP_SIGNED% EQU TRUE (
  @REM
  @REM For Signed Fsp, BPM manifest need to toggle the Boot componenet bit to 1 (ColdBoot & S3Resume).
  @REM Only MSVC build support the Signed Fsp, doing this patch action on batch file only.
  @REM
  %PYTHON_COMMAND% %WORKSPACE_COMMON%\%PLATFORM_BOARD_PACKAGE%\BoardSupport\Tools\ModifyBpmManifest\ModifyBpmManifest.py -F %BPM_PARAMS_FILE% -BC True
  @if !ERRORLEVEL! NEQ 0 (
    @echo "#### Failed to patch the BPM manifest. ####"
    exit 1
  )
  @set BPM_PARAMS_FILE=%BPM_PARAMS_FILE%.patched
)

@if %RESILIENCY_BUILD% EQU TRUE (
  @REM
  @REM For Resiliency BIOS, BPM manifest need to toggle the TopSwap Supported bit to 1 (ColdBoot).
  @REM
  call %PYTHON_COMMAND% %WORKSPACE_COMMON%\Features\FirmwareGuard\CapsuleFeaturePkg\Tools\ToolScripts\ModifyBpmManifest\ModifyBpmManifest.py -F %BPM_PARAMS_FILE% -TS True
  @if !ERRORLEVEL! NEQ 0 (
    @echo "#### Failed to patch the BPM manifest. ####"
    exit 1
  )
  @set BPM_PARAMS_FILE=%BPM_PARAMS_FILE%.patched
)

echo "Use the BPM PARAMS File: %BPM_PARAMS_FILE%"

echo "#### BpmGen2:  Generating Manifest.bin ####"
  %BPMGEN2% GEN %WORKSPACE%\%BUILD_DIR%\FV\%1.fd %BPM_PARAMS_FILE% -BPM %WORKSPACE%\%BUILD_DIR%\FV\Manifest.bin -U %WORKSPACE%\%BUILD_DIR%\FV\%1_MBIOS.fd -KM %WORKSPACE%\%BUILD_DIR%\FV\KeyManifest.bin -d:2 >%BPM_OUT_DIR%\bpmgen2_bpm.txt
@if %ERRORLEVEL% NEQ 0 (
  popd
  echo "#### Error generating BPM file #####"
  set SCRIPT_ERROR=1
  goto Cleanup
)
popd
echo %CD%


copy %BUILD_DIR%\FV\%1_MBIOS.fd %BUILD_DIR%\FV\%2.fd /Y

:Cleanup
@set KM_REVOCATION=
@set KM_ID=
@if %RESILIENCY_BUILD% EQU TRUE (
  del %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\BpmGen\%BPM_PARAMS_FILE%
)
