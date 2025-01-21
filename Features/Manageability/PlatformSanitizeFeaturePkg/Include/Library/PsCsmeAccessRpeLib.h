/** @file
  Platform Sanitize Structure definitions Header file.

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

#ifndef _PS_CSME_ACCESS_RPE_LIB_H_
#define _PS_CSME_ACCESS_RPE_LIB_H_

#pragma pack(1)

//
// Intel RPE Boot Parameter Type IDs
//
typedef enum {
  ParamTypeNone,
  ParamTypeBitMask,
  ParamTypePsid = 10,
  ParamTypeStoragePassword = 20,
} RPE_PARAMETER_TYPE_ID;

//
// RPE Boot Option Parameter Type Structure
//
typedef struct {
  UINT16        VendorId;    // Vendor ID
  UINT16        ParamTypeId; // Parameter Type ID
} RPE_VID_PTID_INFO;

//
// RPE Boot Option Parameter TLV Header Stucture
//
typedef struct {
  RPE_VID_PTID_INFO    ParamType;  // RPE Vendor, Parameter Type Information
  UINT32               Length;     // Length of the value array
} RPE_PARAMETER_TLV_HEADER;

//
// RPE Boot Option Parameter TLV Stucture
//
typedef struct {
  RPE_PARAMETER_TLV_HEADER  Header;     // RPE Parameter Header
  UINT8                     Value[0];   // RPE Paramter data
} RPE_PARAMETER_TLV;

#pragma pack()

/**
  Report current BIOS status to AMT

  @param[in]    BiosStatus         BIOS Status Information, refer 'PS_REPORT_BIOS_STATUS'

  @retval EFI_SUCCESS              Function executed successfully
  @retval Others                   Return HECI status in case of RPE
**/
EFI_STATUS
PsReportBiosStatus (
  IN UINT32                       BiosStatus
  );

/**
  Check whether RPE is Enabled in the setup options and Triggered remotely.
  If RPE is disabled in setup, then notify CSME.

  @retval TRUE       RPE is enabled and triggered remotely for erase actions
  @retval FALSE      RPE is disabled / failed to retrieve or not Triggered from remote.
**/
BOOLEAN
PsIsRpeEnabledAndTriggeredByAmt (
  VOID
  );

/**
  Storage erase report to send to CSME to store in AMT-WEB storage

  @param[in] FileName            File name of the written data.
  @param[in] DataSize            Size of the written data.
  @param[in] Data                The data to be written to a file.

  @retval EFI_SUCCESS            Write data to the AMT WEB successfully
**/
EFI_STATUS
EFIAPI
PsStorageEraseReportToAmt (
  IN CHAR8   *FileName,
  IN UINT32  DataSize,
  IN UINT8   *Data
  );

/**
  This function checks Brand Identity support for RPE via Mbp data Hob

  @retval TRUE    If RPE feature is supported
  @retval FALSE   If RPE feature is not supported
**/
BOOLEAN
IsBrandIdentityEnabledForRpe (
  VOID
  );

/**
  Check whether Config Data Reset is set. This bit gets set to perform
  CSME Configuration Data Reset (CSME Unconfiguration)

  @retval TRUE      CSME Unconfig triggered via Config Data Reset.
  @retval FALSE     CSME Unconfig not Triggered.
**/
BOOLEAN
IsConfigDataResetTriggered (
  VOID
  );

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
  );

/**
  Get Rpe Sanitize PsBootParameters details.

  @param[in out] PsBootParameters     A pointer to the Boot Parameter Structure to update
                                      RPE Triggered list
  @param[in out] StorageDevPassword   A pointer to Store Device Password
  @param[in out] PsOemHookTlvData     A pointer to OEM Hook TLV Data structure

  @retval EFI_SUCCESS                 The operation succeeds.
  @retval EFI_INVALID_PARAMETER       The parameters are invalid.
  @retval Others                      Status depends on Reload config operation
**/
EFI_STATUS
EFIAPI
PsGetRpeSanitizeParameter (
  IN OUT PS_BOOT_PARAMETERS     *PsBootParameters,
  IN OUT CHAR8                  StorageDevPassword[STORAGE_DEV_PSW_MAX_LEN],
  IN OUT PS_OEM_HOOK_TLV_DATA   *PsOemHookTlvData
  );

/**
  Get Rpe Storage Erase password from AMT.

  @param[in out] StorageDevPassword     A pointer to the Storage Erase Password
  @param[in out] PsOemHookTlvData       A pointer to OEM Hook TLV Data structure

  @retval EFI_SUCCESS                   The operation succeeds.
  @retval EFI_INVALID_PARAMETER         The parameters are invalid.
  @retval Others                        Status depends on Reload config operation
**/
EFI_STATUS
EFIAPI
PsGetRpeAdditionalParameters (
  IN OUT CHAR8                  StorageDevPassword[STORAGE_DEV_PSW_MAX_LEN],
  IN OUT PS_OEM_HOOK_TLV_DATA   *PsOemHookTlvData
  );

/**
  Inform CSME to clear RPE Boot.

  @retval EFI_SUCCESS    Successfully sent Command to CSME to perform Unconfiguration
  @retval Others         Other HECI errors
**/
EFI_STATUS
EFIAPI
PsClearRpeBoot (
  VOID
  );

/**
  Update PsBootParameters structure when CSME Unconfiguration Triggered via Config Data Reset

  @param[in out] PsBootParameters   Pointer to Boot Parameter structure.

  @retval EFI_SUCCESS               The operation succeeds.
  @retval EFI_INVALID_PARAMETER     The parameters are invalid.
  @retval Others                    Failed to update Boot Parameter structure.
**/
EFI_STATUS
EFIAPI
PsGetRpeConfigDataReset (
  IN OUT PS_BOOT_PARAMETERS *PsBootParameters
  );

#endif //PS_RPE_LIB_H_
