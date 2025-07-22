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
@REM @par Specification Reference:
@REM
@REM #
@REM #  Module Name:
@REM #
@REM #    Rsa2048Sha256SignPlatform.bat
@REM #
@REM #  Abstract:
@REM #
@REM #    Rsa2048Sha256SignPlatform script.
@REM #
@REM #--*/

@echo on
@setlocal

@echo %1
@echo %2
@echo %3
@echo %4

@if "%1"=="" goto End

@if exist %WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\SignFv\privkey.pem (
  set PRIVATE_KEY_PATH=%WORKSPACE_PLATFORM%\%PLATFORM_FULL_PACKAGE%\InternalOnly\ToolScripts\SignFv\privkey.pem
) else (
  @echo "Can not find privkey.pem, use open source TestSigningPrivateKey.pem"
  set PRIVATE_KEY_PATH=%WORKSPACE_CORE%\BaseTools\Source\Python\Rsa2048Sha256Sign\TestSigningPrivateKey.pem
)

Rsa2048Sha256Sign %1 %2 %3 %4 --private-key %PRIVATE_KEY_PATH%
@if errorlevel 1 (
  @exit /b 1
)

:End
