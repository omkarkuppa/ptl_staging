/** @file
  Pei Memory Telemetry Amt Ppr PEIM Implementation.

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
#include <PiPei.h>
#include <AmtPprEnableVariable.h>
#include <MemoryConfig.h>
#include <Ppi/SiPolicy.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>

///
/// Notification callback descriptor
///
STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mSiPreMemPolicyNotifyList = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gSiPreMemPolicyPpiGuid,
  OnSiPreMemPolicyAvailable
};

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
                                &gMemTelAmtPprVariableGuid,
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
 Check if PPR is supported on platform

 @retval TRUE   Platform supports PPR
 @retval FALSE  Platform does not support PPR
**/
BOOLEAN
IsPprSupported (
  VOID
  )
{
  if (!FixedPcdGetBool (PcdPprCapability)) {
    DEBUG ((DEBUG_VERBOSE, "[%a] PCD PPR capability disabled\n", __FUNCTION__));
    return FALSE;
  }
  return TRUE;
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
  SI_PREMEM_POLICY_PPI    *SiPreMemPolicyPpi;
  MEMORY_CONFIG_NO_CRC    *MemConfigNoCrc;
  BOOLEAN                 PprPcdEnabled;

  PprPcdEnabled = FALSE;

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

  if (SiPreMemPolicyPpi != NULL) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "Unable to Get gMemoryConfigNoCrcGuid block\n"));
      MemConfigNoCrc = NULL;
      ASSERT_EFI_ERROR (Status);
    }
  }

  ///
  /// Update and enable AMT, PPR and PPR Test policies
  ///
  if (AmtPprVariable->Bits.AmtEnabled && MemConfigNoCrc != NULL) {
    MemConfigNoCrc->PprTestType.Value     = PPR_TEST;

    // Check system PCD for PPR support
    PprPcdEnabled = IsPprSupported ();

    if (AmtPprVariable->Bits.PprEnabled && PprPcdEnabled) {
      MemConfigNoCrc->PprRepairType       = HARD_PPR;
    } else {
      MemConfigNoCrc->PprRepairType       = NOREPAIR_PPR;
    }
  }
  return EFI_SUCCESS;
}

/**
  Notification callback that executes when SiPreMemPolicy PPI becomes available.
  This ensures our AMT PPR processing happens early, Memory Initialization.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The SiPreMemPolicy PPI that became available.

  @retval    EFI_SUCCESS          AMT PPR policy was processed successfully
  @retval    Others               Failed to process AMT PPR policy
**/
EFI_STATUS
EFIAPI
OnSiPreMemPolicyAvailable (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN  VOID                         *Ppi
  )
{
  EFI_STATUS          Status;
  AMT_PPR_ENABLE      *AmtPprVariable;

  DEBUG ((DEBUG_INFO, "[%a] Processing AMT PPR\n", __FUNCTION__));

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

/**
  Entry point of the Memory Telemetry AMT PPR PEIM
  Registers a notification to process AMT PPR when SiPreMemPolicy becomes available.

  @param[in] FileHandle       Handle of the file being invoked.
  @param[in] PeiServices      Describes the list of possible PEI Services.

  @retval     EFI_SUCCESS       Notification callback registered successfully
  @retval     Others            Failed to register notification callback
**/
EFI_STATUS
EFIAPI
PeiMemoryTelemetryEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS    Status;

  DEBUG ((DEBUG_INFO, "[%a] Entry - registering SiPreMemPolicy notification\n", __FUNCTION__));

  ///
  /// Register notification callback to execute when SiPreMemPolicy is available
  ///
  Status = PeiServicesNotifyPpi (&mSiPreMemPolicyNotifyList);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to register SiPreMemPolicy notification: %r\n", __FUNCTION__, Status));
  } else {
    DEBUG ((DEBUG_INFO, "[%a] Successfully registered SiPreMemPolicy notification\n", __FUNCTION__));
  }

  return Status;
}
