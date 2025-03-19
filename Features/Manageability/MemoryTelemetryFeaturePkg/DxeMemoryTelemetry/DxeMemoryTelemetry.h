/** @file
  Intel Dxe Memory Telemetry functional definitions.

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

#ifndef _DXE_MEMORY_TELEMETRY_H_
#define _DXE_MEMORY_TELEMETRY_H_

#include <PiDxe.h>
#include "MemTelemetryTables.h"
#include <Library/HobLib.h>

/**
  Handles AMT PPR variable access and clearing/creating of variable for subsequent use
**/
VOID
AmtPprVarHandler (
  VOID
  );

/**
  Entry point of the Dxe Memory Telemetry
  Checks for support, and begins the PHAT table information update.

  @param[in] ImageHandle        Handle for this drivers loaded image protocol.
  @param[in] SystemTable        EFI system table.

  @retval     EFI_SUCCESS       The protocol has installed successfully
  @retval     EFI_UNSUPPORTED   The protocol is not supported
  @retval     Others            The protocol did not install successfully
**/
EFI_STATUS
EFIAPI
DxeMemoryTelemetryEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

#endif // _DXE_MEMORY_TELEMETRY_H_
