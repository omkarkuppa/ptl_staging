/** @file
  Platform Sanitize Services library supports Identifying RPE/LPE boot Header file.

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

#ifndef _PLATFORM_SANITIZE_SERVICES_H_
#define _PLATFORM_SANITIZE_SERVICES_H_

#include "PsParameter.h"
#include <Protocol/StorageSanitizeNotifyProtocol.h>

///
/// Flow Control Services
///

/**
  Storage erase report to send to CSME to store in AMT-WEB storage

  @param[in]  ReportNum     Report count.
  @param[in]  Report        Pointer to an array of report
**/
VOID
EFIAPI
PsStorageEraseReport (
  IN UINT8                   ReportNum,
  IN ERASE_DOCUMENTATION     *Report
  );

/**
  Clear EraseAction Bit in the BootParameter pending list and update the status.

  @param[in] EraseActionBit          EraseAction Bit to be cleared.
  @param[in] Status                  EraseAction exit status.

**/
VOID
PsBootParameterStatusUpdate (
  IN UINT32                EraseActionBit,
  IN EFI_STATUS            Status
  );

/**
  Function to trigger Reset.

  @param[in]  ResetType         The type of reset to perform.
**/
VOID
PsIssueReset (
  IN EFI_RESET_TYPE           ResetType
  );

/**
  Store PS BootParameters structure to NVM.

  @retval EFI_SUCCESS                The firmware has successfully stored the variable and its data as
                                     defined by the Attributes.
  @retval EFI_INVALID_PARAMETER      An invalid combination of attribute bits, name, and GUID was supplied, or the
                                     DataSize exceeds the maximum allowed.
  @retval EFI_INVALID_PARAMETER      VariableName is an empty string.
  @retval EFI_OUT_OF_RESOURCES       Not enough storage is available to hold the variable and its data.
  @retval EFI_DEVICE_ERROR           The variable could not be retrieved due to a hardware error.
  @retval EFI_WRITE_PROTECTED        The variable in question is read-only.
  @retval EFI_WRITE_PROTECTED        The variable in question cannot be deleted.
  @retval EFI_SECURITY_VIOLATION     The variable could not be written due to EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACESS being set,
                                     but the AuthInfo does NOT pass the validation check carried out by the firmware.
  @retval EFI_NOT_FOUND              The variable trying to be updated or deleted was not found.
**/
EFI_STATUS
PsStoreBootParametersAtNvm (
  VOID
  );

/**
  Print Sanitize Alert Information.
  Additionally, In case of RPE, Send operation status using PET alerts.

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
  );


///
/// LPE Services
///

/**
  Check whether LPE is Enabled in the setup options

  @retval TRUE       LPE is enabled to triger erase actions
  @retval FALSE      LPE is disabled / failed to retrive.
**/
BOOLEAN
PsIsLpeEnabled (
  VOID
  );

/**
  Disable LPE in the Setup menu

  @retval EFI_SUCCESS      Disabled successfully
  @retval Others           Data was not retrieved
**/
EFI_STATUS
PsLpeSetDisabled (
  VOID
  );

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
  );

///
/// Storage Services
///

/**
  Register Storage Erase Action Event

  @param[in]  PsBootStorageParameters  The parameter passed in.

  @retval EFI_SUCCESS                  Storage Erase event registered successfully to notify when device connecting.
**/
EFI_STATUS
RegisterStorageEraseEvent (
  IN PS_BOOT_STORAGE_PARAMETERS     *PsBootStorageParameters
  );

#endif  // _PLATFORM_SANITIZE_SERVICES_H_
