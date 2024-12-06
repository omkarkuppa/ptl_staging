/** @file
  Pei Memory Telemetry Feature Implementation.

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

#include "PeiMemoryTelemetry.h"
#include <AmtPprEnableVariable.h>
#include <MemoryConfig.h>
#include <Ppi/SiPolicy.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiLib.h>
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>

/**
  Reads the AmtPprEnable UEFI variable and returns the variable data.

  @param[out] AmtPprVariableData  AMT and PPR enablement variable data

  @retval     EFI_SUCCESS         UEFI variable was found
  @retval     Others              UEFI variable not found
**/
EFI_STATUS
GetAmtPprEnableVar (
  IN OUT AMT_PPR_ENABLE    *AmtPprVariableData
  )
{
  EFI_STATUS                        Status;
  UINTN                             VariableSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;

  VariableSize = sizeof (AMT_PPR_ENABLE);
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status  = VariableServices->GetVariable (
                                VariableServices,
                                AMT_PPR_ENABLE_VARIABLE_NAME,
                                &gAmtPprEnableVariableGuid,
                                NULL,
                                &VariableSize,
                                AmtPprVariableData
                                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Get variable failed with status: %r.\n", __FUNCTION__, Status));
  }
  DEBUG ((DEBUG_INFO, "[%a] AMT PPR variable, AMT bit: %x, PPR bit: %x\n", __FUNCTION__, AmtPprVariableData->Bits.AmtEnabled, AmtPprVariableData->Bits.PprEnabled));
  return Status;
}

/**
  Update MRC PprEnable and and PprTestType policy

  @param[in]  AmtPprVariable    AMT PPR UEFI variable data to update MRC policy

  @retval     EFI_SUCCESS       Success in getting and setting policy data
  @retval     Others            Failed to get Si PreMem or MemConfig Policy
**/
EFI_STATUS
UpdatePprMrcPolicy (
  IN AMT_PPR_ENABLE       *AmtPprVariable
  )
{
  EFI_STATUS              Status;
  MEMORY_CONFIGURATION    *MemConfig;
  SI_PREMEM_POLICY_PPI    *SiPreMemPolicyPpi;

  if (AmtPprVariable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ///
  /// Obtain policy settings.
  ///
  SiPreMemPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicyPpi);
  if (EFI_ERROR (Status) || (SiPreMemPolicyPpi == NULL)) {
    DEBUG ((DEBUG_INFO, "[%a] Failed to locate SiPreMemPolicyPpi with statue: %r\n", __FUNCTION__, Status));
    return Status;
  }

  ///
  /// Get the current MRC policy
  ///
  MemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigGuid, (VOID *) &MemConfig);
  if (EFI_ERROR (Status) || (MemConfig == NULL)) {
    DEBUG ((DEBUG_INFO, "[%a] Failed to get MemoryConfigGuid with statue: %r\n", __FUNCTION__, Status));
    return Status;
  }

  ///
  /// Update and enable AMT, PPR and PPR Test policies
  ///
  if (AmtPprVariable->Bits.AmtEnabled) {
    MemConfig->ExternalInputs.PprTestType           = PPR_TEST;
    MemConfig->ExternalInputs.TrainingEnables3.PPR  = ENABLED;
    if (AmtPprVariable->Bits.PprEnabled) {
      MemConfig->ExternalInputs.PprRepairType       = HARD_PPR;
    } else {
      MemConfig->ExternalInputs.PprRepairType       = NOREPAIR_PPR;
    }
  }
  return EFI_SUCCESS;
}

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
  IN  EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES   **PeiServices
  )
{
  EFI_STATUS          Status;
  AMT_PPR_ENABLE      *AmtPprVariable;

  DEBUG ((DEBUG_INFO, "[%a] Entry\n", __FUNCTION__));

  AmtPprVariable = (AMT_PPR_ENABLE *) AllocateZeroPool (sizeof (AMT_PPR_ENABLE));
  if (AmtPprVariable == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ///
  /// Read UEFI Variable AmtPprEnable. Avoid ASSERT/Hang if not exist
  ///
  Status = GetAmtPprEnableVar (AmtPprVariable);
  if (EFI_ERROR (Status)) {
    ///
    /// wait for variable to be created in DXE phase for next boot
    ///
    goto Exit;
  }

  ///
  /// Update MRC policy based on UEFI variable
  ///
  Status = UpdatePprMrcPolicy (AmtPprVariable);

  Exit:
    DEBUG ((DEBUG_INFO, "[%a] Exit with status %r\n", __FUNCTION__, Status));
    FreePool (AmtPprVariable);
    return Status;
}
