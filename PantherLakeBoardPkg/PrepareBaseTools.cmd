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
@REM @par Specification
@REM

@echo off

REM ---------------------------------------------------------------------------
REM Prepare Tools provided from Python script
REM ---------------------------------------------------------------------------

@if not defined PYTHON_COMMAND (
  set PYTHON_COMMAND=py -3
)

REM ---------------------------------------------------------------------------
REM Prepare workspace environment.
REM ---------------------------------------------------------------------------

setlocal EnableDelayedExpansion EnableExtensions

if defined WORKSPACE goto :WorkSpaceDefined
set LocationOfThisFile=%~dp0
set WORKSPACE=%LocationOfThisFile%..\..\

:WorkSpaceDefined
set WORKSPACE_CORE=%WORKSPACE%\Edk2
set WORKSPACE_COMMON=%WORKSPACE%\Intel
set WORKSPACE_PLATFORM=%WORKSPACE%\Intel
set WORKSPACE_SILICON=%WORKSPACE%\Intel
set WORKSPACE_BINARIES=%WORKSPACE%\Binaries
set BASE_TOOLS_PATH=%WORKSPACE_CORE%\BaseTools
set EDK_TOOLS_PATH=%WORKSPACE_CORE%\BaseTools
set EDK_TOOLS_BIN=%WORKSPACE_CORE%\BaseTools\Bin\Win32
set PATH=%EDK_TOOLS_BIN%;%PATH%

REM ---------------------------------------------------------------------------
REM Get Visual Studio environment Setting By Edksetup.
REM Order of precedence is 2019, 2017, 2015, 2012, 2013, 2010 and then 2008.
REM ---------------------------------------------------------------------------
@if not defined TOOL_CHAIN_TAG (
  if defined VS160COMNTOOLS (
    set TOOL_CHAIN_TAG=VS2019
  ) else if defined VS150COMNTOOLS (
    set TOOL_CHAIN_TAG=VS2017
  )
)

REM If no supported version of Visual Studio was detected, return an error.
if not defined TOOL_CHAIN_TAG (
  echo.
  echo !!! ERROR !!! Preferred Visual Studio not detected!!!
  echo Supported Compilers are: VS2019 or VS2017.
  echo.
  set SCRIPT_ERROR=1
  goto :End
)

echo Show CL revision
cl

REM ---------------------------------------------------------------------------
REM Building Tools from C source code, will take around 30 seconds.
REM ---------------------------------------------------------------------------

if not exist %EDK_TOOLS_BIN% (
  mkdir %EDK_TOOLS_BIN%
)

pushd .
cd %EDK_TOOLS_PATH%
nmake -f Makefile c
popd

:END
endlocal

