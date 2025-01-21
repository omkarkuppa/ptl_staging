/** @file
  Library for Fsp Performance functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/

#ifndef _FSP_PERFORMANCE_LIB_H_
#define _FSP_PERFORMANCE_LIB_H_

#include <Guid/Performance.h>
#include <FspPerformance.h>

#define BASE_IDENTIFIER  0x1150

typedef struct {
  FSP_PERFORMANCE    value;
  CONST CHAR8        *String;
} FSP_PERF_DATA;

/**
  Create FSP Performance Hob

  @retval EFI_SUCCESS          FSP Performance HOB is created.
  @retval EFI_OUT_OF_RESOURCES Could not allocate memory.
**/
EFI_STATUS
EFIAPI
CreateFspPerformanceHob (
  VOID
  );

/**
  This function returns the current time.

  @retval           Returns the current timestamp
**/
UINT64
EFIAPI
GetFspCurrentTime (
  VOID
  );

/**
  This function stores the timestamps into FspPerformanceHob and logs corresponding
  FPDT record.

  @param[in]  Identifier          Identifier for the global FSP_PERF_DATA
  @param[in]  StartTime           StartTime of the module
**/
VOID
EFIAPI
LogFspPerformanceData (
  IN FSP_PERFORMANCE  Identifier,
  IN UINT64           StartTime
  );

/**
  This function returns the time taken by FSP module.

  @retval           Returns the time taken by a FSP module
**/
UINT64
EFIAPI
GetFspModuleTime (
  IN FSP_PERFORMANCE  Identifier
  );

#endif // __FSP_PERFORMANCE_LIB_H_
