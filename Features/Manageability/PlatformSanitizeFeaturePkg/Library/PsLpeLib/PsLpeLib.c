/**@file
  Platform Sanitize library provides APIs for Sanitization which are
  specific to LPE Erase method.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <PsParameter.h>
#include <Guid/PlatformSanitizeVariable.h>

GLOBAL_REMOVE_IF_UNREFERENCED LPE_SETUP                   mLpeSetupData = {0};

/**
  Get LPE Setup Variable from NVM

  @retval EFI_SUCCESS      Data retrieved
  @retval Others           Data was not retrieved
**/
EFI_STATUS
PsGetLpeSetupData (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINTN                           VariableSize;

  ZeroMem (&mLpeSetupData, sizeof (LPE_SETUP));

  VariableSize = sizeof (LPE_SETUP);
  Status = gRT->GetVariable (
                  LPE_SETUP_VARIABLE_NAME,
                  &gPsLpeSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &mLpeSetupData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PS-%a: Status = %r\n", __FUNCTION__, Status));
    ZeroMem (&mLpeSetupData, sizeof (LPE_SETUP));
  }

  return Status;
}

/**
  Check whether LPE is Enabled in the setup options

  @retval TRUE       LPE is enabled to triger erase actions
  @retval FALSE      LPE is disabled / failed to retrive.
**/
BOOLEAN
PsIsLpeEnabled (
  VOID
  )
{
  EFI_STATUS                      Status;

  DEBUG ((DEBUG_INFO, "PS-%a = ", __FUNCTION__));

  Status = PsGetLpeSetupData ();
  if (!EFI_ERROR (Status)) {
    if (mLpeSetupData.LpeEnable == 1) {
      DEBUG ((DEBUG_INFO, "TRUE\n"));
      return TRUE;
    }
  }

  DEBUG ((DEBUG_INFO, "FALSE\n"));
  return FALSE;
}

/**
  Get LPE Sanitize Parameter details.

  @param[in out] PsBootParameters    A pointer to the Boot Parameter Structure to update LPE Triggered list

  @retval EFI_SUCCESS                The operation succeeds.
  @retval EFI_INVALID_PARAMETER      The parameters are invalid.
  @retval EFI_UNSUPPORTED            LPE is not enabled.
  @retval Others                     Status depends on Reload config operation
**/
EFI_STATUS
EFIAPI
PsGetLpeSanitizeParameter (
  IN OUT PS_BOOT_PARAMETERS *PsBootParameters
  )
{
  EFI_STATUS      Status;
  UINT32          RequestedEraseMask;

  DEBUG ((DEBUG_INFO, "PS-%a = ", __FUNCTION__));

  if (PsBootParameters == NULL || PsBootParameters->TriggerSource != BootLpe) {
    DEBUG ((DEBUG_ERROR, "Invalid Parameter.\n"));
    return EFI_INVALID_PARAMETER;
  }

  Status = PsGetLpeSetupData ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (mLpeSetupData.LpeEnable != 1) {
    DEBUG ((DEBUG_ERROR, "LPE not enabled.\n"));
    return EFI_UNSUPPORTED;
  }

  RequestedEraseMask = 0;
  if (mLpeSetupData.LpeEraseAll) {
    RequestedEraseMask = PS_PERFORM_ACTION_MASK;
    if (mLpeSetupData.LpeCsmeUnconfigure == 0x0) {
      RequestedEraseMask = RequestedEraseMask & (~PS_CSME_UNCONFIGURE);
    }
    if (mLpeSetupData.LpeStorageEraseVerify == 0x0) {
      RequestedEraseMask = RequestedEraseMask & (~PS_VERIFY_STORAGE_MEDIA);
    }
  } else {
    RequestedEraseMask |= ((mLpeSetupData.LpeEraseSSD ? (PS_ERASE_STORAGE_MEDIA | PS_GENERATE_SANITIZE_REPORT) : 0) |
                           (mLpeSetupData.LpeStorageEraseVerify ? PS_VERIFY_STORAGE_MEDIA : 0) |
                           (mLpeSetupData.LpeClearTPM ? PS_TPM_CLEAR : 0) |
                           (mLpeSetupData.LpeOemCustomAction ? PS_OEM_CUSTOM_ACTION : 0) |
                           (mLpeSetupData.LpeClearNvm ? PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION : 0) |
                           (mLpeSetupData.LpeBiosReloadGoldenConfig ? PS_BIOS_RELOAD_GOLDEN_CONFIG : 0) |
                           (mLpeSetupData.LpeCsmeUnconfigure ? PS_CSME_UNCONFIGURE : 0));
  }

  PsBootParameters->PsRequestedList = RequestedEraseMask;

  return EFI_SUCCESS;
}

/**
  Disable LPE in the Setup menu

  @retval EFI_SUCCESS      Disabled successfully
  @retval Others           Data was not retrieved
**/
EFI_STATUS
PsLpeSetDisabled (
  VOID
  )
{
  EFI_STATUS                      Status;

  Status = PsGetLpeSetupData ();
  if (!EFI_ERROR (Status)) {
    ZeroMem (&mLpeSetupData, sizeof (LPE_SETUP));
    Status = gRT->SetVariable (
                    LPE_SETUP_VARIABLE_NAME,
                    &gPsLpeSetupVariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (LPE_SETUP),
                    &mLpeSetupData
                    );
  }

  return Status;
}
