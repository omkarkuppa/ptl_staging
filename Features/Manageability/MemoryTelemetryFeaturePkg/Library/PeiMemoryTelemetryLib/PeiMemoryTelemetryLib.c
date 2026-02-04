/**@file
  Memory Telemetry library provides APIs for updating memory configuration
  based on UEFI variable updates

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#include <PiPei.h>
#include <MemoryConfig.h>
#include <AmtPprEnableVariable.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiMemoryTelemetryLib.h>
#include <Library/PcdLib.h>

/**
  Read AMT PPR Enable variable

  @param[out] AmtPprData    AMT PPR variable data

  @retval     EFI_SUCCESS   Variable read successfully
  @retval     Others        Failed to read variable
**/
STATIC
EFI_STATUS
ReadAmtPprVariable (
  OUT AMT_PPR_ENABLE    *AmtPprData
  )
{
  EFI_STATUS                        Status;
  UINTN                             VariableSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  VariableSize = sizeof (AMT_PPR_ENABLE);
  return VariableServices->GetVariable (
                             VariableServices,
                             AMT_PPR_ENABLE_VARIABLE_NAME,
                             &gMemTelAmtPprVariableGuid,
                             NULL,
                             &VariableSize,
                             AmtPprData
                             );
}

/**
  Update MRC PprRepairType and PprTestType based on AMT PPR variable

  Reads AMT PPR variable and updates memory configuration policies directly.
  If AMT is enabled:
    - Sets MemConfigNoCrc->PprTestType.Value to PPR_TEST (6)
    - Sets MemConfigNoCrc->PprRepairType to HARD_PPR if PPR capability enabled, else NOREPAIR_PPR

  @param[in]  MemConfigNoCrc    Pointer to MEMORY_CONFIG_NO_CRC config block

  @retval     EFI_SUCCESS       AMT enabled and policies updated successfully
  @retval     EFI_NOT_FOUND     AMT PPR variable not found or AMT not enabled
  @retval     Others            Failed to locate PPI or get config block
**/
EFI_STATUS
UpdateAmtPprMrcPolicy (
  IN MEMORY_CONFIG_NO_CRC    *MemConfigNoCrc
  )
{
  EFI_STATUS              Status;
  AMT_PPR_ENABLE          AmtPprData;
  BOOLEAN                 PprCapabilityEnabled;

  if (MemConfigNoCrc == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a] MemConfigNoCrc null\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Read AMT PPR variable
  //
  Status = ReadAmtPprVariable (&AmtPprData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[%a] AMT PPR variable not found\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  if (!AmtPprData.Bits.AmtEnabled) {
    DEBUG ((DEBUG_INFO, "[%a] AMT not enabled\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  //
  // Update policies
  //
  MemConfigNoCrc->PprTestType.Value = PPR_TEST;

  PprCapabilityEnabled = FixedPcdGetBool (PcdPprCapability);
  if (AmtPprData.Bits.PprEnabled && PprCapabilityEnabled) {
    MemConfigNoCrc->PprRepairType = HARD_PPR;
  } else {
    MemConfigNoCrc->PprRepairType = NOREPAIR_PPR;
  }
  DEBUG ((DEBUG_INFO, "%a:\n\tAMT Variable Bit: %x\n\tPPR Variable Bit: %x\n\tPPR Test Type:    %x\n\tPPR Repair Type:  %x\n",
                         __FUNCTION__, AmtPprData.Bits.AmtEnabled, AmtPprData.Bits.PprEnabled, MemConfigNoCrc->PprTestType.Value, MemConfigNoCrc->PprRepairType));

  return EFI_SUCCESS;
}
