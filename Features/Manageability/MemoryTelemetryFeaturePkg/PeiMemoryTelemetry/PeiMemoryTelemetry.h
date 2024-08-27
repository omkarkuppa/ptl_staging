/** @file
  Pei Memory Telemetry Functional Definitions

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

#ifndef _PEI_MEMORY_TELEMETRY_H_
#define _PEI_MEMORY_TELEMETRY_H_

#include <PiPei.h>
#include <Uefi.h>

/**
  Entry point of the Memory Telemetry PEIM
  Checks the AMT PPR enable variable status, will update memory config accordingly.

  @param[in]  FileHandle        The file handle of the file, Not used.
  @param[in]  PeiServices       General purpose services available to every PEIM.

  @retval     EFI_SUCCESS       PEI Memory Telemetry executed as expected
  @retval     Others            PEI Memory Telemetry failed to get and set AmtPprEnable variable
**/
EFI_STATUS
EFIAPI
PeiMemoryTelemetryEntryPoint (
  IN  EFI_PEI_FILE_HANDLE      FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  );
#endif // _PEI_MEMORY_TELEMETRY_H_
