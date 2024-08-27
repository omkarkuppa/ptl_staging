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
#include <Guid/AmtPprEnableVariable.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

/**
  Gets or creates the AmtPprEnable UEFI variable

  @param [in,out] AmtPprVarData   Pointer to the Amt PPR variable value

  @retval         EFI_SUCCESS     UEFI variable found or created
  @retval         Others          UEFI variable failed to create
**/
VOID
CheckAmtPprVariable (
  IN OUT AMT_PPR_ENABLE  *AmtPprVarData
  )
{
  EFI_STATUS      Status;
  UINTN           VarSize;
  UINT32          VarAttr;

  VarSize = sizeof (AMT_PPR_ENABLE);
  Status = gRT->GetVariable (
                  AMT_PPR_ENABLE_VARIABLE_NAME,
                  &gAmtPprEnableVariableGuid,
                  &VarAttr,
                  &VarSize,
                  &AmtPprVarData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[%a] Variable not found, creating now",__FUNCTION__));
  }
}

/**
  Clears the AmtPprEnable UEFI variable

  @param [in,out] AmtPprVarData   Pointer to the Amt PPR variable value

  @retval         EFI_SUCCESS     UEFI variable cleared successfully
  @retval         Others          UEFI variable failed to clear
**/
EFI_STATUS
ClearAmtPprVariable (
  IN AMT_PPR_ENABLE  *AmtPprVarData
  )
{
  EFI_STATUS      Status;
  UINTN           VarSize;

  if (AmtPprVarData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (AmtPprVarData, sizeof (AMT_PPR_ENABLE));
  VarSize = sizeof (AMT_PPR_ENABLE);
  Status = gRT->SetVariable (
                  AMT_PPR_ENABLE_VARIABLE_NAME,
                  &gAmtPprEnableVariableGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  VarSize,
                  &AmtPprVarData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to clear variable\n", __FUNCTION__));
  }
  return Status;
}

/**
  Main control flow for MemoryTelemetry AMT PPR Report access and configure for PHAT publishing
**/
VOID
MemoryTelemetryAmtPprMain (
  VOID
  )
{
  AMT_PPR_ENABLE  *AmtPprVarData;

  // Check for AMT PPR Variable
  AmtPprVarData = (AMT_PPR_ENABLE *) AllocateZeroPool (sizeof (AMT_PPR_ENABLE));
  if (AmtPprVarData == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed in memory allocation\n", __FUNCTION__));
    return;
  }
  CheckAmtPprVariable (AmtPprVarData);

  // Clear AMT PPR variable
  ClearAmtPprVariable(AmtPprVarData);
}
