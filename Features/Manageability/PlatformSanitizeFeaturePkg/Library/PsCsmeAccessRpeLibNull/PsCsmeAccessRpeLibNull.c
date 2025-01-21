/**@file
  Platform Sanitize library provides APIs for Sanitization which are
  specific to RPE method.

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
#include <Library/PlatformSanitizeServices.h>
#include <Library/PsCsmeAccessRpeLib.h>

/**
  Check whether Config Data Reset is set. This bit gets set to perform
  CSME Configuration Data Reset (CSME Unconfiguration)

  @retval TRUE      CSME Unconfig triggered via Config Data Reset.
  @retval FALSE     CSME Unconfig not Triggered.
**/
BOOLEAN
IsConfigDataResetTriggered (
  VOID
  )
{
  return FALSE;
}

/**
  Update PsBootParameters structure when CSME Unconfiguration Triggered via Config Data Reset

  @param[in out] PsBootParameters   Pointer to Boot Parameter structure.

  @retval EFI_SUCCESS               The operation succeeds.
  @retval Others                    Failed to update Boot Parameter structure.
**/
EFI_STATUS
EFIAPI
PsGetRpeConfigDataReset (
  IN OUT PS_BOOT_PARAMETERS *PsBootParameters
  )
{
  return EFI_UNSUPPORTED;
}

/**
  This function checks Brand Identity support for RPE via Mbp data Hob

  @retval TRUE    If RPE feature is supported
  @retval FALSE   If RPE feature is not supported
**/
BOOLEAN
IsBrandIdentityEnabledForRpe (
  VOID
  )
{
  return FALSE;
}

/**
  Get RPE Setup Variable from NVM

  @retval EFI_SUCCESS      Data retrieved
  @retval Others           Data was not retrieved
**/
EFI_STATUS
PsGetRpeSetupData (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Check whether RPE is Enabled in the setup options and Triggered remotely.
  If RPE is disabled in setup, then notify CSME.

  @retval TRUE       RPE is enabled and triggered remotely for erase actions
  @retval FALSE      RPE is disabled / failed to retrive or not Triggered from remote.
**/
BOOLEAN
PsIsRpeEnabledAndTriggeredByAmt (
  VOID
  )
{
  return FALSE;
}

/**
  Get Rpe Storage Erase password from AMT.

  @param[in out] StorageDevPassword    A pointer to the Storage Erase Password

  @retval EFI_SUCCESS                  The operation succeeds.
  @retval Others                       Status depends on Reload config operation
**/
EFI_STATUS
EFIAPI
PsGetRpeAdditionalParameters (
  IN OUT CHAR8                  StorageDevPassword[STORAGE_DEV_PSW_MAX_LEN],
  IN OUT PS_OEM_HOOK_TLV_DATA   *PsOemHookTlvData
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get Rpe Sanitize PsBootParameters details.

  @param[in out] PsBootParameters     A pointer to the Boot Parameter Structure to update
                                      RPE Triggered list

  @retval EFI_SUCCESS                 The operation succeeds.
  @retval Others                      Status depends on Reload config operation
**/
EFI_STATUS
EFIAPI
PsGetRpeSanitizeParameter (
  IN OUT PS_BOOT_PARAMETERS     *PsBootParameters,
  IN OUT CHAR8                  StorageDevPassword[STORAGE_DEV_PSW_MAX_LEN],
  IN OUT PS_OEM_HOOK_TLV_DATA   *PsOemHookTlvData
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Send RPE operation status using PET alerts.

  @param[in]    PsStatus           Operation status either in Progress or Failure
  @param[in]    PsEraseAction      Erase action details
  @param[in]    PsOperation        Erase operation details

  @retval EFI_SUCCESS              Function executed successfully
  @retval Others                   Return HECI status in case of RPE
**/
EFI_STATUS
EFIAPI
PsRpeAlertInfo (
  IN PS_PET_STATUS                PsStatus,
  IN PS_PET_ERASE_ACTION          PsEraseAction,
  IN PS_PET_OPCODE                PsOperation
  )
{
  return EFI_UNSUPPORTED;
}

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
  return EFI_UNSUPPORTED;
}

/**
  Inform CSME to clear RPE Boot.

  @retval EFI_UNSUPPORTED  Not supported on Null Lib
**/
EFI_STATUS
EFIAPI
PsClearRpeBoot (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Storage erase report to send to CSME to store in AMT-WEB storage

  @param[in] FileName            File name of the written data.
  @param[in] DataSize            Size of the written data.
  @param[in] Data                The data to be written to a file.

  @retval EFI_UNSUPPORTED        Not supported on Null Lib
**/
EFI_STATUS
EFIAPI
PsStorageEraseReportToAmt (
  IN CHAR8   *FileName,
  IN UINT32  DataSize,
  IN UINT8   *Data
  )
{
  return EFI_UNSUPPORTED;
}
