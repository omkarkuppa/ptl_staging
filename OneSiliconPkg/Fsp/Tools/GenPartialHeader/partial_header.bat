@REM @file
@REM
@REM  @copyright
@REM  INTEL CONFIDENTIAL
@REM  Copyright (C) 2019 Intel Corporation.
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

@echo off
set EDIR=ErrorHeader
set PDIR=PartialHeader
set FSPFILE=fsp_error_log.txt
py -3 %~dp0\partial_header.py %1\FspmUpd.h %2\FSPM.txt Fspm
py -3 %~dp0\partial_header.py %1\FspsUpd.h %2\FSPS.txt Fsps

py -3 %~dp0\name_change.py newFspmUPD.h Fspm
py -3 %~dp0\name_change.py newFspsUPD.h Fsps

py -3 %~dp0\Final.py NCFspmUpd.h %2\FSPM.txt fspm
py -3 %~dp0\Final.py NCFspsUpd.h %2\FSPS.txt fsps

if exist %FSPFILE% (
  DEL /F/Q/S fsp_error_log.txt > NUL

  if not exist %EDIR% mkdir %EDIR%

  copy /Y FinalFspmUpd.h %EDIR%\FspmUpd.h > NUL
  copy /Y FinalFspsUpd.h %EDIR%\FspsUpd.h > NUL

  DEL /F/Q/S newFspmUPD.h newFspsUPD.h NCFspmUPD.h NCFspsUPD.h FinalFspsUpd.h FinalFspmUpd.h Fspm_name_change_list.txt Fsps_name_change_list.txt > NUL
  @echo on
  echo "Error: Please check 'fspm_error.log/fsps_error.log' for more details"
  @echo off
  exit /b 1

) else (
  if not exist %PDIR% mkdir %PDIR%

  copy /Y FinalFspmUpd.h %PDIR%\FspmUpd.h > NUL
  copy /Y FinalFspsUpd.h %PDIR%\FspsUpd.h > NUL

  DEL /F/Q/S newFspmUPD.h newFspsUPD.h NCFspmUPD.h NCFspsUPD.h FinalFspsUpd.h FinalFspmUpd.h Fspm_name_change_list.txt Fsps_name_change_list.txt > NUL
  exit /b 0
)