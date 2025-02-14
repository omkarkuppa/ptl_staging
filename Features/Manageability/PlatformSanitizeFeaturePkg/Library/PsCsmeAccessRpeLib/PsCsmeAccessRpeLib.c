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
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PlatformSanitizeServices.h>
#include <PsParameter.h>
#include <Guid/PlatformSanitizeVariable.h>
#include <MeBiosPayloadHob.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeAsfLib.h>
#include <Library/DxeMeLib.h>
#include <Library/HobLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Library/DxeAsfHeciLib.h>
#include <AsfMsgs.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PsCsmeAccessRpeLib.h>

#define V_PCH_INTEL_VENDOR_ID                   0x8086 ///< Default Intel PCH Vendor ID

GLOBAL_REMOVE_IF_UNREFERENCED RPE_SETUP         mRpeSetupData;

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
  if (AsfIsConfigurationDataResetEnabled ()) {
    DEBUG ((DEBUG_INFO, "PS-%a: CSME Unconfig triggered via Config Data Reset.\n", __FUNCTION__));
    return TRUE;
  }

  return FALSE;
}

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
  )
{
  DEBUG ((DEBUG_INFO, "PS-%a.\n", __FUNCTION__));

  if (PsBootParameters == NULL) {
    DEBUG ((DEBUG_ERROR, "PS: NULL Parameter structure.\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Cross check for CSME Unconfig trigger
  //
  if (AsfIsConfigurationDataResetEnabled ()) {
    PsBootParameters->PsRequestedList = PS_CSME_UNCONFIGURE;
  }

  return EFI_SUCCESS;
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
  ME_BIOS_PAYLOAD_HOB *MbpHob;

  MbpHob = NULL;

  //
  // Get Mbp Data HOB
  //
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL) {
    DEBUG ((DEBUG_ERROR, "PS-%a: MBP HOB not found\n", __FUNCTION__));
    return FALSE;
  }

  //
  // If FwCapSku is available, then check for AMT & RPE capabilities bits and
  // AMT features bit. If they are enabled and return TRUE.
  //
  if (MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.Amt == 1 && MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.RPE == 1) {
    if (MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.Amt) {
      DEBUG ((DEBUG_INFO, "PS-%a: Intel Brand Identity Enabled RPE\n", __FUNCTION__));
      return TRUE;
    }
  }

  DEBUG ((DEBUG_INFO, "PS-%a: Intel Brand Identity Not Enabled RPE\n", __FUNCTION__));

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
  EFI_STATUS                      Status;
  UINTN                           VariableSize;

  ZeroMem (&mRpeSetupData, sizeof (RPE_SETUP));

  VariableSize = sizeof (RPE_SETUP);
  Status = gRT->GetVariable (
                  RPE_SETUP_VARIABLE_NAME,
                  &gPsRpeSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &mRpeSetupData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PS-%a: Status = %r\n", __FUNCTION__, Status));
    ZeroMem (&mRpeSetupData, sizeof (RPE_SETUP));
  }
  return Status;
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
  EFI_STATUS              Status;
  AMT_BOOT_CONTROL        AmtBootControl;
  BOOLEAN                 IsAmtRpeTriggered;
  ME_BIOS_PAYLOAD_HOB     *MbpHob;

  DEBUG ((DEBUG_INFO, "PS: %a - enter\n", __FUNCTION__));

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL) {
    return FALSE;
  }

  if (MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.Amt == 0) {
    return FALSE;
  }

  Status = PsGetRpeSetupData ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "PS: Failed to get RPE setup Data\n"));
    return FALSE;
  }

  //
  // Read the Amt Boot Control Structure
  //
  Status = AsfGetUefiBootOptionsState (&AmtBootControl);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  //
  // BIOS passes the state of user enabled boot option to AMT, if AMT is out of sync with BIOS.
  //
  if (AmtBootControl.Bits.RemotePlatformErase != (BOOLEAN) mRpeSetupData.RpeEnable) {
    AmtBootControl.Bits.RemotePlatformErase = (BOOLEAN) mRpeSetupData.RpeEnable;
    AsfSetUefiBootOptionsState (AmtBootControl);
  }

  //
  // Check whether RPE is triggered from remote via AMT
  //
  IsAmtRpeTriggered = FALSE;
  if (AsfIsRemotePlatformEraseEnabled ()) {
    DEBUG ((DEBUG_INFO, "PS: RPE requested from remote access.\n"));
    IsAmtRpeTriggered = TRUE;
  }

  if (mRpeSetupData.RpeEnable == 1) {
    return IsAmtRpeTriggered;
  } else if (IsAmtRpeTriggered) {
    //
    // RPE is disabled in BIOS Setup but still triggered remotely.
    // BIOS to Notify AMT with failure info via BIOSLastStatus & PET Alert
    //
    DEBUG ((DEBUG_ERROR, "PS: AMT initiated RPE, but feature is disabled from BIOS. Reporting failure to AMT\n"));
    Status = PsReportBiosStatus (AsfRbsGeneralFailure);
    //SendRpePetAlert (ASF_RPE_EVENT_DATA3_BOOT_PARAMETER_RECEIVE_FAILED, 0, ASF_RPE_EVENT_OFFSET_FAILURE);
  }
  return FALSE;
}

/**
  Get Rpe Storage Erase password from AMT.

  @param[in out] StorageDevPassword    A pointer to the Storage Erase Password

  @retval EFI_SUCCESS                  The operation succeeds.
  @retval EFI_INVALID_PARAMETER        The parameters are invalid.
  @retval Others                       Status depends on Reload config operation
**/
EFI_STATUS
EFIAPI
PsGetRpeAdditionalParameters (
  IN OUT CHAR8                  StorageDevPassword[STORAGE_DEV_PSW_MAX_LEN],
  IN OUT PS_OEM_HOOK_TLV_DATA   *PsOemHookTlvData
  )
{
  EFI_STATUS                         Status;
  RPE_PARAMETER_TLV                  *RpeParamTlv;
  UINT32                             Index;
  UEFI_BOOT_OPTION_PARAMETER         *AmtUefiBootOption;

  DEBUG ((DEBUG_INFO, "PS: %a - Enter.\n", __FUNCTION__));

  AmtUefiBootOption = AllocateZeroPool (sizeof (UEFI_BOOT_OPTION_PARAMETER));
  if (AmtUefiBootOption == NULL) {
    DEBUG ((DEBUG_ERROR, "PS: Memory Allocation failed\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Status = AsfGetUefiBootParameters (AmtUefiBootOption);
  if (EFI_ERROR (Status)) {
    FreePool (AmtUefiBootOption);
    DEBUG ((DEBUG_ERROR, "PS: Failed to get ASF UefiBootParameters.\n"));
    //SendRpePetAlert (ASF_RPE_EVENT_DATA3_BOOT_PARAMETER_RECEIVE_FAILED, 0, ASF_RPE_EVENT_OFFSET_FAILURE);
    return Status;
  }

  //
  // Extract BootParameters from the TLV Structure
  // Update StorageDevPassword with the value from ASF which is in PARAMETER_TLV format.
  //
  RpeParamTlv = (RPE_PARAMETER_TLV *) AmtUefiBootOption->ParametersArray;

  for (Index = 0; Index < AmtUefiBootOption->NumberOfParams; Index++) {
    DEBUG ((DEBUG_INFO, "PS: Index: %d, VendorId: %x, ParamTypeId: %d, length: %d\n",
      Index, RpeParamTlv->Header.ParamType.VendorId, RpeParamTlv->Header.ParamType.ParamTypeId, RpeParamTlv->Header.Length));

    if (RpeParamTlv->Header.ParamType.VendorId == V_PCH_INTEL_VENDOR_ID) {
      switch (RpeParamTlv->Header.ParamType.ParamTypeId) {
        case ParamTypeStoragePassword:
          if (RpeParamTlv->Header.Length <= STORAGE_DEV_PSW_MAX_LEN) {
            CopyMem (&StorageDevPassword, &RpeParamTlv->Value, RpeParamTlv->Header.Length);
            //Note: Do not print password in the log
            DEBUG ((DEBUG_INFO, "PS: Storage Erase Password length = %d.\n", RpeParamTlv->Header.Length));
          } else {
            DEBUG ((DEBUG_WARN, "PS: Storage Erase Password length exceeds OPAL/ATA spec.\n"));
          }
          break;

        default:
          break;
        }
    } else if ((RpeParamTlv->Header.ParamType.VendorId != 0) && (RpeParamTlv->Header.ParamType.VendorId != 0xFFFF)) {
      if (RpeParamTlv->Header.Length <= PS_OEM_CUSTOM_ERASE_TLV_LEN) {
        PsOemHookTlvData->OemVid = RpeParamTlv->Header.ParamType.VendorId;
        PsOemHookTlvData->OemParameterTypeId = RpeParamTlv->Header.ParamType.ParamTypeId;
        PsOemHookTlvData->OemDataLength = RpeParamTlv->Header.Length;
        CopyMem (&PsOemHookTlvData->OemData, &RpeParamTlv->Value, RpeParamTlv->Header.Length);
      }
    }

    //
    // Increment to next RpeParamTlv
    //
    RpeParamTlv = (RPE_PARAMETER_TLV *) (((UINTN) RpeParamTlv) + sizeof (RPE_PARAMETER_TLV_HEADER) + RpeParamTlv->Header.Length);
  }

  FreePool (AmtUefiBootOption);
  return EFI_SUCCESS;
}

/**
  Get Rpe Sanitize PsBootParameters details.

  @param[in out] PsBootParameters     A pointer to the Boot Parameter Structure to update
                                      RPE Triggered list

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
  )
{
  EFI_STATUS                         Status;
  RPE_PARAMETER_TLV                  *RpeParamTlv;
  UINT32                             Index;
  UEFI_BOOT_OPTION_PARAMETER         *AmtUefiBootOption;

  DEBUG ((DEBUG_INFO, "PS: %a - Enter.\n", __FUNCTION__));
  if (PsBootParameters == NULL) {
    DEBUG ((DEBUG_ERROR, "PS: PsBootParameters - NULL\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Cross verification for RPE boot
  //
  if (PsBootParameters->TriggerSource != BootRpe) {
    return EFI_INVALID_PARAMETER;
  }

  AmtUefiBootOption = AllocateZeroPool (sizeof (UEFI_BOOT_OPTION_PARAMETER));
  if (AmtUefiBootOption == NULL) {
    DEBUG ((DEBUG_ERROR, "PS: Memory Allocation failed\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Status = AsfGetUefiBootParameters (AmtUefiBootOption);
  if (EFI_ERROR (Status)) {
    FreePool (AmtUefiBootOption);
    DEBUG ((DEBUG_ERROR, "PS: Failed to get ASF UefiBootParameters.\n"));
    return Status;
  }

  //
  // Extract BootParameters from the TLV Structure
  // Update PS_BOOT_PARAMETERS structure with the value from ASF which is in PARAMETER_TLV format.
  //
  RpeParamTlv = (RPE_PARAMETER_TLV *) AmtUefiBootOption->ParametersArray;

  for (Index = 0; Index < AmtUefiBootOption->NumberOfParams; Index++) {
    DEBUG ((DEBUG_INFO, "PS: Index: %d, VendorId: %x, ParamTypeId: %d, length: %d\n",
      Index, RpeParamTlv->Header.ParamType.VendorId, RpeParamTlv->Header.ParamType.ParamTypeId, RpeParamTlv->Header.Length));

    if (RpeParamTlv->Header.ParamType.VendorId == V_PCH_INTEL_VENDOR_ID) {
      switch (RpeParamTlv->Header.ParamType.ParamTypeId) {
        case ParamTypeBitMask:
          if (RpeParamTlv->Header.Length < MAX_UEFI_BOOT_PARAMETERS_ARRAY_SIZE) {
            CopyMem (&PsBootParameters->PsRequestedList, &RpeParamTlv->Value, RpeParamTlv->Header.Length);
            DEBUG ((DEBUG_INFO, "PS: RequestedList bitMask = %x.\n", PsBootParameters->PsRequestedList));
          }
          break;

        case ParamTypeStoragePassword:
          if (RpeParamTlv->Header.Length <= STORAGE_DEV_PSW_MAX_LEN) {
            CopyMem (StorageDevPassword, &RpeParamTlv->Value, RpeParamTlv->Header.Length);
            //Note: Do not print password in the log
            DEBUG ((DEBUG_INFO, "PS: Storage Erase Password length = %d.\n", RpeParamTlv->Header.Length));
          } else {
            DEBUG ((DEBUG_WARN, "PS: Storage Erase Password length exceeds OPAL/ATA spec.\n"));
          }
          break;

        default:
          break;
        }
    } else if ((RpeParamTlv->Header.ParamType.VendorId != 0) && (RpeParamTlv->Header.ParamType.VendorId != 0xFFFF)) {
      if (RpeParamTlv->Header.Length <= PS_OEM_CUSTOM_ERASE_TLV_LEN) {
        PsOemHookTlvData->OemVid = RpeParamTlv->Header.ParamType.VendorId;
        PsOemHookTlvData->OemParameterTypeId = RpeParamTlv->Header.ParamType.ParamTypeId;
        PsOemHookTlvData->OemDataLength = RpeParamTlv->Header.Length;
        CopyMem (PsOemHookTlvData->OemData, &RpeParamTlv->Value, RpeParamTlv->Header.Length);
      }
    } else {
      DEBUG ((DEBUG_INFO, "PS: Invalid Vendor ID sent from Remote User Interface.\n"));
    }

    //
    // Increment to next RpeParamTlv
    //
    RpeParamTlv = (RPE_PARAMETER_TLV *) (((UINTN) RpeParamTlv) + sizeof (RPE_PARAMETER_TLV_HEADER) + RpeParamTlv->Header.Length);
  }

  FreePool (AmtUefiBootOption);
  return EFI_SUCCESS;
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
  EFI_STATUS             Status;
  UINT8                  PsOperationCode;
  UINT8                  PsDevice;
  UINT8                  PsOperationStatus;

  DEBUG ((DEBUG_INFO, "Ps-RPE Alert: Notify PET Alert\n"));

  // Map the input to right definitions as per ASF.
  if (PsStatus == PsProgressInProgress) {
    PsOperationStatus = ASF_RPE_EVENT_OFFSET_PROGRESS;
  } else {
    PsOperationStatus = ASF_RPE_EVENT_OFFSET_FAILURE;
  }

  switch (PsEraseAction) {
    case PsPetEraseGeneral:
      PsDevice = 0;     // Default 0 value assigned
      break;

    case PsPetEraseStorage:
      PsDevice = ASF_RPE_EVENT_DATA4_SSD_ERASE_ACTION;
      break;

    case PsPetEraseTpmClear:
      PsDevice = ASF_RPE_EVENT_DATA4_TPM_ERASE_ACTION;
      break;

    case PsPetEraseClearNvm:
      PsDevice = ASF_RPE_EVENT_DATA4_CLEAR_BIOS_NVM_VARIABLE;
      break;

    case PsPetEraseReloadConfig:
      PsDevice = ASF_RPE_EVENT_DATA4_RELOAD_BIOS_GOLDEN_CONFIG;
      break;

    case PsPetEraseCustomAction:
      PsDevice = ASF_RPE_EVENT_DATA4_OEM_CUSTOM_ACTIONS;
      break;

    case PsPetEraseCsmeUnconfig:
      PsDevice = ASF_RPE_EVENT_DATA4_CSME_UNCONFIGURE;
      break;

    default :
      PsDevice = 0;     // Default 0 value assigned
      break;
  }

  switch (PsOperation) {
    // General Notification Operation codes
    case PsBootParameterReceive:
      PsOperationCode = ASF_RPE_EVENT_DATA3_BOOT_PARAMETER_RECEIVED;
      break;

    case PsBeginPlatformSanitize:
      PsOperationCode = ASF_RPE_EVENT_DATA3_BEGINNING_PLATFORM_ERASE;
      break;

    case PsClearReservedParameters:
      PsOperationCode = ASF_RPE_EVENT_DATA3_CLEAR_RESERVED_PARAMETERS;
      break;

    case PsInvalidBootParameters:
      PsOperationCode = ASF_RPE_EVENT_DATA3_INVALID_BOOT_PARAMETERS;
      break;

    case PsAllSettingsDecremented:
      PsOperationCode = ASF_RPE_EVENT_DATA3_ALL_SETTINGS_DECREMENTED;
      break;

    // Erase Action related Operation codes
    case PsOperationStarted:
      PsOperationCode = ASF_RPE_EVENT_DATA3_STARTED_ERASING_DEVICE;
      break;

    case PsOperationSuccessful:
      PsOperationCode = ASF_RPE_EVENT_DATA3_DEVICE_ERASED_SUCCESSFULLY;
      break;

    case PsOperationFailed:
      PsOperationCode = ASF_RPE_EVENT_DATA3_ERROR_ERASING_DEVICE;
      break;

    case PsOperationNotSupported:
      PsOperationCode = ASF_RPE_EVENT_DATA3_ERASING_DEVICE_UNSUPPORTED;
      break;

    default:
      PsOperationCode = ASF_RPE_EVENT_DATA3_BOOT_PARAMETER_RECEIVED;     // Default 0 value assigned
      break;
  }

  // Send PET Alerts to AMT
  Status = SendRpePetAlert (PsOperationCode, PsDevice, PsOperationStatus);

  return Status;
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
  UINT32       AmtBiosStatus;

  switch (BiosStatus) {
    case PsSuccess:
      AmtBiosStatus = AsfRbsSuccess;
      break;

    case PsInProgress:
      AmtBiosStatus = AsfRbsInProgress;
      break;

    case PsGeneralFailure:
      AmtBiosStatus = AsfRbsGeneralFailure;
      break;

    case PsAccessDenied:
      AmtBiosStatus = AsfRbsAccessDenied;
      break;

    case PsUnsupported:
      AmtBiosStatus = AsfRbsUnsupported;
      break;

    default:
      AmtBiosStatus = 0;
      break;
  }

  return ReportBiosStatus (AmtBiosStatus);
}

/**
  Inform CSME to clear RPE Boot.

  @retval EFI_SUCCESS  Successfully sent Command to CSME to perform Unconfiguration
  @retval Others       Other HECI errors
**/
EFI_STATUS
EFIAPI
PsClearRpeBoot (
  VOID
  )
{
  return ClearBootOptions ();
}

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
  )
{
  //
  // Write erase report to AMT web storage
  //
  return WriteStorageEraseReport (FileName, DataSize, Data);
}
