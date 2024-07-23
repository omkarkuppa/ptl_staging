/** @file
  Pei Memory Health Insights Feature Implementation.

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
#include <PiPei.h>
#include "PeiMemoryHealthInsights.h"

#include <MemoryConfig.h>
#include <Ppi/SiPolicy.h>
#include <Library/PeiLib.h>
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/AmtPprEnableVariable.h>

/**
  Reads the AmtPprEnable UEFI variable and returns the variable data.

  @param[out] AmtPprVariableData  AMT and PPR enablement variable data

  @retval     EFI_SUCCESS         UEFI variable was found
  @retval     Others              UEFI variable not found
**/
EFI_STATUS
GetAmtPprEnableVar (
  OUT AMT_PPR_ENABLE    *AmtPprVariableData
  )
{
  EFI_STATUS      Status;
  UINTN           VariableSize;

  VariableSize = sizeof (AMT_PPR_ENABLE);
  Status = PeiGetVariable (
             AMT_PPR_ENABLE_VARIABLE_NAME,
             &gAmtPprEnableVariableGuid,
             (VOID *) &AmtPprVariableData,
             &VariableSize
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] AMT PPR variable not found.\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_ERROR, "[%a] AMT PPR variable found.\n", __FUNCTION__));
  return EFI_SUCCESS;
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

  // Obtain policy settings.
  SiPreMemPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicyPpi);
  if (EFI_ERROR (Status) || (SiPreMemPolicyPpi == NULL)) {
    return Status;
  }

  // Get the current MRC policy
  MemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigGuid, (VOID *) &MemConfig);
  if (EFI_ERROR (Status) || (MemConfig == NULL)) {
    return Status;
  }

  // Enable AMT if variable enabled
  MemConfig->ExternalInputs.TrainingEnables3.PPR = AmtPprVariable->Bits.AmtEnabled;

  // Update the PPR enabled and PPR Test policies
  if (AmtPprVariable->Bits.AmtEnabled && AmtPprVariable->Bits.PprEnabled) {
    MemConfig->ExternalInputs.PprRepairType = 2;
  } else {
    MemConfig->ExternalInputs.PprRepairType = 0;
  }
  return EFI_SUCCESS;
}

/**
  Entry point of the Memory Health Insights PEIM
  Checks the AMT PPR enable variable status, will update memory config accordingly.

  @param[in]  FileHandle        The file handle of the file, Not used.
  @param[in]  PeiServices       General purpose services available to every PEIM.

  @retval     EFI_SUCCESS       PEI MHI executed as expected
  @retval     Others            PEI MHI failed to get and set AmtPprEnable variable
**/
EFI_STATUS
EFIAPI
PeiMemoryHealthInsightsEntryPoint (
  IN  EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES   **PeiServices
  )
{
  EFI_STATUS          Status;
  AMT_PPR_ENABLE      *AmtPprVariable;

  AmtPprVariable = (AMT_PPR_ENABLE *) AllocateZeroPool (sizeof (AMT_PPR_ENABLE));
  if (AmtPprVariable) {
    return EFI_OUT_OF_RESOURCES;
  }

  // Read UEFI Variable AmtPprEnable. Avoid ASSERT/Hang if not exist
  Status = GetAmtPprEnableVar (AmtPprVariable);
  if (EFI_ERROR(Status)) {
    // wait for variable to be created in DXE phase for next boot
    DEBUG((DEBUG_WARN, "[%a] exit with status %r", __FUNCTION__, Status));
    FreePool (AmtPprVariable);
    return Status;
  }

  // Update MRC policy based on UEFI variable
  Status = UpdatePprMrcPolicy (AmtPprVariable);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_WARN, "[%a] failed to get and set MRC PPR policy with status %r", __FUNCTION__, Status));
  }


  DEBUG((DEBUG_INFO, "[%a] exit with status %r", __FUNCTION__, Status));
  FreePool (AmtPprVariable);
  return Status;
}
