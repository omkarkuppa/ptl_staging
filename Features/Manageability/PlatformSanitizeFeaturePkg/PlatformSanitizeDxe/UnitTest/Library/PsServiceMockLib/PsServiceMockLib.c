/** @file
  PS Mock Library functions for Platform Sanitize Unit Testing

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

@par Specification
**/

#include <Uefi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <Library/DebugLib.h>
#include <PsParameter.h>

GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR16 *mGenOpCodeStr[] = {
    L"Boot Parameters Received",
    L"Started Platform Sanitize",
    L"Cleared Reserved Parameters",
    L"Invalid Boot Parameters",
    L"All Setting Decremented",
  };

GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR16 *mDevEraseOpCodeStr[] = {
    L"Started",
    L"Successful",
    L"Failed",
    L"Not Supported"
  };

/**
  Report current BIOS status to AMT

  @param[in] BiosStatus             BIOS Status Information, refer 'PS_REPORT_BIOS_STATUS'

  @retval EFI_SUCCESS               Function executed successfully
  @retval Others                    Return HECI status in case of RPE
**/
EFI_STATUS
PsReportBiosStatus (
  IN UINT32                       BiosStatus
  )
{
  return EFI_SUCCESS;
}

/**
  Clear Boot option and all related NVM variables. So that next boot will be Normal Boot.
**/
VOID
PsClearBootOption (
  VOID
  )
{
  return;
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
  return EFI_SUCCESS;
}

/**
  Inform CSME to clear RPE Boot.

  @retval EFI_SUCCESS    Successfully sent Command to CSME to perform Unconfiguration
  @retval Others         Other HECI errors
**/
EFI_STATUS
EFIAPI
PsClearRpeBoot (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  Print Sanitize Alert Information.

  @param[in]    PsStatus           Operation status either in Progress or Failure
  @param[in]    PsEraseAction      Erase action details
  @param[in]    PsOperation        Erase operation details

  @retval EFI_SUCCESS               Function executed successfully
  @retval Others                    Return HECI status in case of RPE
**/
EFI_STATUS
PsPetAlert (
  IN PS_PET_STATUS                 PsStatus,
  IN PS_PET_ERASE_ACTION           PsEraseAction,
  IN PS_PET_OPCODE                 PsOperation
  )
{
  EFI_STATUS         Status;

  Status = EFI_SUCCESS;

  DEBUG_CODE_BEGIN ();
  if (PsStatus == PsProgressInProgress) {
    DEBUG ((DEBUG_INFO, "Ps-Alert: In Progress\n"));
  } else if (PsStatus == PsProgressFailure) {
    DEBUG ((DEBUG_INFO, "Ps-Alert: Failed\n"));
  } else {
    return Status;
  }

  if (PsEraseAction == PsPetEraseGeneral && PsOperation >= PsGeneralOpcodeMax) {
    DEBUG ((DEBUG_INFO, "Invalid PET parameter for General Opration\n"));
    return EFI_UNSUPPORTED;
  } else if (PsEraseAction > PsPetEraseGeneral && PsOperation < PsGeneralOpcodeMax) {
    DEBUG ((DEBUG_INFO, "Invalid PET parameter for Erase Action Opration\n"));
    return EFI_UNSUPPORTED;
  }

  if (PsOperation >= PsGeneralOpcodeMax) {
    //
    // Switch to use DevEraseOpCodeStr to print erase action result.
    //
    PsOperation -= PsGeneralOpcodeMax;
  }

  switch (PsEraseAction) {
    case PsPetEraseGeneral:
      DEBUG ((DEBUG_INFO, "    %s\n", mGenOpCodeStr[PsOperation]));
      break;
    case PsPetEraseStorage:
      DEBUG ((DEBUG_INFO, "   Storage Erase %s\n", mDevEraseOpCodeStr[PsOperation]));
      break;
    case PsPetEraseTpmClear:
      DEBUG ((DEBUG_INFO, "   TPM Clear %s\n", mDevEraseOpCodeStr[PsOperation]));
      break;
    case PsPetEraseCustomAction:
      DEBUG ((DEBUG_INFO, "   Custom Action %s\n", mDevEraseOpCodeStr[PsOperation]));
      break;
    case PsPetEraseClearNvm:
      DEBUG ((DEBUG_INFO, "   Clear All NVM Variable %s\n", mDevEraseOpCodeStr[PsOperation]));
      break;
    case PsPetEraseReloadConfig:
      DEBUG ((DEBUG_INFO, "   BIOS Golden Config %s\n", mDevEraseOpCodeStr[PsOperation]));
      break;
    case PsPetEraseCsmeUnconfig:
      DEBUG ((DEBUG_INFO, "   CSME Unconfigure %s\n", mDevEraseOpCodeStr[PsOperation]));
      break;
    default:
      break;
  }
  DEBUG_CODE_END ();

  return Status;
}

/**
  Function to trigger Reset.

  @param[in]  ResetType         The type of reset to perform.
**/
VOID
PsIssueReset (
  IN EFI_RESET_TYPE           ResetType
  )
{
  DEBUG ((DEBUG_INFO, "Performing Reset..\n"));
  return;
}
