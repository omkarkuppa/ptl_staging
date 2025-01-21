@REM @file
@REM Script to generate Discrete Thunderbolt firmware update capsule.
@REM
@REM Usage:
@REM   GenCapsuleDiscreteTbt.bat [FMP Payload] [Discrete Thunderbolt Version] [Output Path]
@REM   FMP Payload          : The FMP payload used to create the Capsule image.
@REM   Discrete TBT Version : Discrete Thunderbolt FW version in DEC format
@REM   Output Path          : The path for output Capsule image.
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
@echo off
setlocal
echo start

set FMP_CAPSULE_PAYLOAD=%1
set DIS_TBT_VERSION_DEC=%2
set DIS_TBT_VERSION_HEX=%2
set FMP_CAPSULE_OUTPUT_DIR=%3
set CAP_PLATFORM_PREFIX=%4

@REM
@REM A tricky way to covert FW Version from DEC to HEX
@REM
@REM call cmd /c exit /b %DIS_TBT_VERSION_DEC%
@REM set DIS_TBT_VERSION_HEX=0x%=exitcode%

@REM
@REM Setup environment variables
@REM
set FMP_CAPSULE_VENDOR=Intel
set FMP_CAPSULE_GUID=86A885EE-D71E-2ED6-0FC1-9D6CCC9677EB
set FMP_CAPSULE_FILE=%CAP_PLATFORM_PREFIX%_DTbt_%DIS_TBT_VERSION_DEC%.cap
set FMP_CAPSULE_VERSION=0x%DIS_TBT_VERSION_HEX%
set FMP_CAPSULE_STRING=0.0.0.%DIS_TBT_VERSION_DEC%
set FMP_CAPSULE_NAME="Intel Discrete TBT %FMP_CAPSULE_STRING%"
set FMP_CAPSULE_LSV=0x00000000
set WINDOWS_CAPSULE_KEY=SAMPLE_DEVELOPMENT.pfx
set WINDOWS_CAPSULE_OUTPUT_DIR=%FMP_CAPSULE_OUTPUT_DIR%\WindowsCapsule\DTbt

for %%i in ("%FMP_CAPSULE_FILE%") do SET WINDOWS_CAPSULE_NAME=%%~ni

if not exist "%FMP_CAPSULE_PAYLOAD%" exit /b 1

REM
REM Delete files if exist
REM
@del %FMP_CAPSULE_OUTPUT_DIR%\%FMP_CAPSULE_FILE% 1>NUL 2>&1
@del %WINDOWS_CAPSULE_OUTPUT_DIR%\%WINDOWS_CAPSULE_NAME%.* 1>NUL 2>&1

REM
REM Sign capsule using OpenSSL with EDK II Test Certificate
REM
call GenerateCapsule ^
  --encode ^
  -v ^
  --guid %FMP_CAPSULE_GUID% ^
  --fw-version %FMP_CAPSULE_VERSION% ^
  --lsv %FMP_CAPSULE_LSV% ^
  --capflag PersistAcrossReset ^
  --capflag InitiateReset ^
  --signing-tool-path=%OPEN_SSL_SIGN_TOOL_PATH% ^
  --signer-private-cert=%WORKSPACE%\edk2\BaseTools\Source\Python\Pkcs7Sign\TestCert.pem ^
  --other-public-cert=%WORKSPACE%\edk2\BaseTools\Source\Python\Pkcs7Sign\TestSub.pub.pem ^
  --trusted-public-cert=%WORKSPACE%\edk2\BaseTools\Source\Python\Pkcs7Sign\TestRoot.pub.pem ^
  -o %FMP_CAPSULE_FILE% ^
  %FMP_CAPSULE_PAYLOAD%

copy %FMP_CAPSULE_FILE% %FMP_CAPSULE_OUTPUT_DIR%

REM
REM Search for the location of Windows capsule key and setup enviroment variables
REM
set BINARIES_PATH=%WORKSPACE%\Binaries

dir %BINARIES_PATH%\%WINDOWS_CAPSULE_KEY% /s /b > %WORKSPACE%\Conf\WindowKeyPath.txt
set /p WINDOWS_CAPSULE_KEY_PATH=<%WORKSPACE%\Conf\WindowKeyPath.txt

REM
REM Copy windows capsule key file to Gen Tool path for successful signing key, otherwise return sign fail message.
REM
copy %WINDOWS_CAPSULE_KEY_PATH% %CD%

REM
REM Generate Windows Capsule package with test key
REM
if exist "%WINDOWS_CAPSULE_KEY%" (
  @%PYTHON_COMMAND% CreateWindowsCapsule.py ^
    %WINDOWS_CAPSULE_NAME% ^
    %FMP_CAPSULE_STRING% ^
    %FMP_CAPSULE_GUID% ^
    %FMP_CAPSULE_FILE% ^
    %FMP_CAPSULE_VERSION% ^
    %FMP_CAPSULE_VENDOR% ^
    %FMP_CAPSULE_VENDOR% ^
    %FMP_CAPSULE_NAME% %WINDOWS_CAPSULE_KEY%

  xcopy /s/e/v/i/y WindowsCapsule %WINDOWS_CAPSULE_OUTPUT_DIR%
  rmdir /s /q WindowsCapsule
)

REM
REM Delete windows capsule key file.
REM
del %WINDOWS_CAPSULE_KEY%

erase %FMP_CAPSULE_FILE%
