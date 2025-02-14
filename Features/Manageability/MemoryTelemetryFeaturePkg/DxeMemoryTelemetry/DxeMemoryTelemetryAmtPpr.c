/** @file
  Dxe Memory Telemetry Post Packaging Repair Variable Handler.

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

#include "DxeMemoryTelemetry.h"
#include <MemoryConfig.h>
#include <AmtPprEnableVariable.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

/**
  Function to create/clear the AmtPprEnable variable
**/
VOID
AmtPprVarHandler (
  VOID
  )
{
  AMT_PPR_ENABLE  AmtPprVarData;
  EFI_STATUS      Status;

  ///
  /// Create/Clear AMT PPR variable
  ///
  ZeroMem (&AmtPprVarData, sizeof (AmtPprVarData));
  Status = gRT->SetVariable (
                  AMT_PPR_ENABLE_VARIABLE_NAME,
                  &gMemTelAmtPprVariableGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  sizeof (AmtPprVarData),
                  &AmtPprVarData
                  );
  if (EFI_ERROR (Status)){
    DEBUG ((DEBUG_INFO, "[%a] Error created and cleared variable status %r\n", __FUNCTION__, Status));
  }
}
