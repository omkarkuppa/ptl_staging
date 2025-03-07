/** @file
  Implementation file for Heci Message functionality

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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
#include <BupMsgs.h>
#include <IccMsgs.h>
#include <MchiMsgs.h>
#include <MeBiosPayloadData.h>
#include <MeState.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>
#include <IntelRcStatusCode.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiMeLib.h>
#include <Library/ChipsetInitLib.h>
#include <Library/MeUtilsLib.h>
#include <Ppi/HeciControlPpi.h>
#include <Register/HeciRegs.h>

//
// ME Client - MKHI
//

//
// MKHI_CBM_GROUP_ID = 0x00
//

/**
  Send Core BIOS Reset Request Message through HECI to reset the system.

  @param[in] ResetOrigin         Reset source
  @param[in] ResetType           Global or Host reset

  @retval EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_NOT_FOUND          No ME present
  @retval EFI_DEVICE_ERROR       HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT            HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciSendCbmResetRequest (
  IN  UINT8                      ResetOrigin,
  IN  UINT8                      ResetType
  )
{
  EFI_STATUS                     Status;
  CBM_RESET_REQUEST              CbmResetRequest;
  UINT32                         HeciLength;
  UINT32                         AckLength;
  UINT32                         MeMode;
  UINT32                         Result;
  CHAR8                          *ResetString;

  switch (ResetType) {
    case CBM_HRR_GLOBAL_RESET:
      ResetString = "Global";
      break;

    case CBM_HRR_HOST_RESET:
      ResetString = "Host";
      break;

    case CBM_HRR_CSME_RESET:
      ResetString = "CSME";
      break;

    default:
      return EFI_UNSUPPORTED;
  }

  DEBUG((DEBUG_INFO, "ME-BIOS: ME: %a Reset Request Entry.\n", ResetString));
  if (!MeSkuIsClient ()) {
    return EFI_UNSUPPORTED;
  }

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_RESETREQ_ENTRY_START); //PostCode (0xE06)

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || ((MeMode != ME_MODE_NORMAL) && (MeMode != ME_MODE_TEMP_DISABLED))) {
    return EFI_UNSUPPORTED;
  }

  CbmResetRequest.MkhiHeader.Data           = 0;
  CbmResetRequest.MkhiHeader.Fields.GroupId = MKHI_CBM_GROUP_ID;
  CbmResetRequest.MkhiHeader.Fields.Command = CBM_RESET_CMD;
  CbmResetRequest.Data.RequestOrigin        = ResetOrigin;
  CbmResetRequest.Data.ResetType            = ResetType;
  HeciLength                                = sizeof (CBM_RESET_REQUEST);
  AckLength                                 = sizeof (CBM_RESET_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &CbmResetRequest,
             HeciLength,
             (UINT32 *) &CbmResetRequest,
             &AckLength
             );
  if (EFI_ERROR (Status)) {
    if (Status == EFI_NOT_FOUND) {
      DEBUG ((DEBUG_ERROR, "ME-BIOS: ME: %a Reset Request Exit - Error by HECI service not found.\n", ResetString));
      REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_NOHECI);  //PostCode (0xEA6)
    } else if (Status == EFI_UNSUPPORTED) {
      DEBUG ((DEBUG_ERROR, "ME-BIOS: ME: %a Reset Request Exit - Error by message is not allowed.\n", ResetString));
      REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_MSGNA); //PostCode (0xE86)
    } else {
      DEBUG ((DEBUG_ERROR, "PeiHeciSendCbmResetRequest() - Unable to Send Reset Request - %r\n", Status));
      DEBUG ((DEBUG_ERROR, "ME-BIOS: ME: %a Reset Request Exit - Error by message sent fail.\n", ResetString));
      REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_MSG_SENTFAIL); //PostCode (0xEC6)
    }
    return Status;
  }

  Result = ((CBM_RESET_RESPONSE *) &CbmResetRequest)->MkhiHeader.Fields.Result;
  if (Result != 0) {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: ME: %a Reset Request Exit - Error by message ack error. Result: %x\n", ResetString, Result));
    Status = EFI_DEVICE_ERROR;
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_RESETREQ_EXIT_ERR_MSG_ACKERR); //PostCode (0xEE6)
  } else {
    DEBUG((DEBUG_INFO, "ME-BIOS: ME: %a Reset Request Exit - Success.\n", ResetString));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_SUCCESS); //PostCode (0xE26)
  }

  return Status;
}

//
// MKHI_FWCAPS_GROUP_ID = 0x03
//

/**
  Send Get Rule Data Request to CSME

  @param[in]   RuleId               Identifies the rule which data is requested.
  @param[out]  *RuleData            Pointer to requested rule data.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_INVALID_PARAMETER     Parameter invalid.
**/
EFI_STATUS
PeiHeciFwCapsGetRuleData (
  IN  UINT32                      RuleId,
  OUT UINT32                      *RuleData
  )
{
  EFI_STATUS                      Status;
  GET_RULE_BUFFER                 GetRuleMsg;
  UINT32                          Length;
  UINT32                          RecvLength;
  UINT32                          MeMode;

  if (RuleData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  GetRuleMsg.Request.MkhiHeader.Data           = 0;
  GetRuleMsg.Request.MkhiHeader.Fields.GroupId = MKHI_FWCAPS_GROUP_ID;
  GetRuleMsg.Request.MkhiHeader.Fields.Command = FWCAPS_GET_RULE_CMD;
  GetRuleMsg.Request.RuleId                    = RuleId;
  Length                                       = sizeof (GET_RULE_REQUEST);
  RecvLength                                   = sizeof (GET_RULE_RESPONSE);

  ///
  /// Send Get Rule Data Request to ME
  ///
  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetRuleMsg,
             Length,
             (UINT32 *) &GetRuleMsg,
             &RecvLength
             );

  if (!EFI_ERROR (Status) &&
      (GetRuleMsg.Response.MkhiHeader.Fields.Command == FWCAPS_GET_RULE_CMD) &&
      (GetRuleMsg.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (GetRuleMsg.Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
    *RuleData = GetRuleMsg.Response.RuleData;
  }

  return Status;
}

/**
  Send Get Firmware SKU Request to ME

  @param[out] FwCapsSku             ME Firmware Capability SKU

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetFwCapsSkuMsg (
  OUT MEFWCAPS_SKU             *RuleData
  )
{
  return PeiHeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, FwCapsRuleTypeId), (UINT32 *) RuleData);
}

/**
  This message is sent by the BIOS to retrieve information about CSME firmware
  that platform runs with.

  @param[out] RuleData              Including PlatformBrand, IntelMeFwImageType,
                                    SuperSku, and PlatformTargetUsageType

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetPlatformTypeMsg (
  OUT PLATFORM_TYPE_RULE_DATA     *RuleData
  )
{
  return PeiHeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, PlatTypeRuleTypeId), (UINT32 *) RuleData);
}

/**
  The Get FW Feature Status message is based on MKHI interface.
  This command is used by BIOS to get firmware runtime status.

  @param[out] RuleData              ME Firmware Capability SKU

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetFwFeatureStateMsg (
  OUT MEFWCAPS_SKU                *RuleData
  )
{
  return PeiHeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, FeatureStateRuleTypeId), (UINT32 *) RuleData);
}

/**
  This message is sent by the BIOS in PEI phase to query
  ME Unconfig on RTC Clear Disable state.

  @param[out] RuleData            1 - Unconfig on RTC clear is disabled
                                  0 - Unconfig on RTC clear is enabled

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetUnconfigOnRtcClearDisableMsg (
  OUT UINT32                *RuleData
  )
{
  return PeiHeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, UnconfigOnRtcClarRuleTypeId), RuleData);
}

/**
  This message is sent by the BIOS on the boot where the host wants to get the firmware provisioning state.
  The firmware will respond to AMT BIOS SYNC INFO message even after the End of Post.

  @param[out] RuleData            @see AMT_BIOS_SYNC_INFO

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetAmtBiosSyncInfo (
  OUT AMT_BIOS_SYNC_INFO *RuleData
  )
{
  return PeiHeciFwCapsGetRuleData (FWCAPS_RULE_ID (AmtFeatureId, AmtBiosSyncInfoRuleTypeId), (UINT32 *) RuleData);
}

/**
  Send Set Rule Data Request to CSME

  @param[in]   RuleId             Identifies the rule which data needs to be changed.
  @param[in]   RuleDataLength     Rule Data Length.
  @param[in]   RuleData           Pointer to new rule data.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciFwCapsSetRuleData (
  IN  UINT32                      RuleId,
  IN  UINT8                       RuleDataLength,
  IN  UINT32                      RuleData
  )
{
  EFI_STATUS                      Status;
  SET_RULE_BUFFER                 SetRuleMsg;
  UINT32                          Length;
  UINT32                          RecvLength;
  UINT32                          MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  SetRuleMsg.Request.MkhiHeader.Data           = 0;
  SetRuleMsg.Request.MkhiHeader.Fields.GroupId = MKHI_FWCAPS_GROUP_ID;
  SetRuleMsg.Request.MkhiHeader.Fields.Command = FWCAPS_SET_RULE_CMD;
  SetRuleMsg.Request.RuleId                    = RuleId;
  SetRuleMsg.Request.RuleDataLen               = RuleDataLength;
  SetRuleMsg.Request.RuleData                  = RuleData;
  Length                                       = sizeof (SET_RULE_REQUEST);
  RecvLength                                   = sizeof (SET_RULE_RESPONSE);

  ///
  /// Send Set Rule Data Request to ME
  ///
  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &SetRuleMsg,
             Length,
             (UINT32 *) &SetRuleMsg,
             &RecvLength
             );

  return Status;
}

/**
  This message is sent by the BIOS in PEI phase to set
  ME Unconfig on RTC Clear Disable state.

  @param[in] RuleData             1 - Disable Unconfig on RTC clear
                                  0 - Enable Unconfig on RTC clear

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function or EOP was sent
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciSetUnconfigOnRtcClearDisableMsg (
  IN UINT32                RuleData
  )
{
  return PeiHeciFwCapsSetRuleData (FWCAPS_RULE_ID (KernelFeatureId, UnconfigOnRtcClarRuleTypeId), RULE_DATA_LENGTH, RuleData);
}

//
// MKHI_GEN_GROUP_ID = 0xFF
//

/**
  Get MKHI (Management Engine Kernel Host Interface) Version.

  @param[out] MkhiVersion           MKHI Version

  @retval EFI_UNSUPPORTED           Can't localize HECI protocol
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetMkhiVersion (
  OUT MKHI_VERSION        *MkhiVersion
  )
{
  EFI_STATUS                  Status;
  GET_MKHI_VERSION_REQUEST    Request;
  GET_MKHI_VERSION_RESPONSE   Response;
  UINT32                      Length;
  UINT32                      RecvLength;

  Request.MkhiHeader.Data           = 0;
  Request.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  Request.MkhiHeader.Fields.Command = GEN_GET_MKHI_VERSION_CMD;
  Length                            = sizeof (GET_MKHI_VERSION_REQUEST);
  RecvLength                        = sizeof (GET_MKHI_VERSION_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32*) &Request,
             Length,
             (UINT32*) &Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status) &&
      (Response.MkhiHeader.Fields.Command == GEN_GET_MKHI_VERSION_CMD) &&
      (Response.MkhiHeader.Fields.IsResponse == 1) &&
      (Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
    *MkhiVersion = Response.MkhiVersion;
  }

  return Status;
}

/**
  Get ME Unconfiguration Status.

  @param[out] UnconfigStatus      Unconfiguration Status:
                                    0x01 - Unconfiguration in progress
                                    0x02 - No unconfigure w/o password to be processed
                                    0x03 - CSME finished processing unconfigure w/o password
                                    0x80 - Error encountered by CSME during unconfigure w/o password

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetUnconfigureStatus (
  OUT UINT8* UnconfigStatus
  )
{
  EFI_STATUS             Status;
  GEN_GET_ME_UNCFG_STATE UnconfigureStatus;
  UINT32                 Length;
  UINT32                 MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    *UnconfigStatus = ME_UNCONFIG_NOT_IN_PROGRESS;
    return EFI_UNSUPPORTED;
  }

  UnconfigureStatus.MkhiHeader.Data           = 0;
  UnconfigureStatus.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  UnconfigureStatus.MkhiHeader.Fields.Command = GEN_GET_ME_UNCFG_STATE_CMD;
  Length                                      = sizeof (GEN_GET_ME_UNCFG_STATE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &UnconfigureStatus,
             Length,
             (UINT32 *) &UnconfigureStatus,
             &Length
             );

  if (!EFI_ERROR (Status) &&
      (UnconfigureStatus.MkhiHeader.Fields.Command == GEN_GET_ME_UNCFG_STATE_CMD) &&
      (UnconfigureStatus.MkhiHeader.Fields.IsResponse == 1)) {
    *UnconfigStatus = (UINT8) UnconfigureStatus.MkhiHeader.Fields.Result;
  }

  return Status;
}

/**
  Send Get Image Firmware Version Request to ME

  @param[in]  PartitionId          Partition ID
  @param[out] PartitionIdData      Return the version of the Partition ID

  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL     Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetImageFwVersionMsg (
  IN  UINT32                    PartitionId,
  OUT FLASH_PARTITION_DATA      *PartitionIdData
  )
{
  EFI_STATUS                    Status;
  GET_IMAGE_FW_VERSION_BUFFER   GetImageFwVersion;
  UINT32                        Length;
  UINT32                        RecvLength;
  UINT32                        MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  GetImageFwVersion.Request.MkhiHeader.Data           = 0;
  GetImageFwVersion.Request.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  GetImageFwVersion.Request.MkhiHeader.Fields.Command = GEN_GET_IMAGE_FW_VERSION;
  GetImageFwVersion.Request.PartitionId               = PartitionId;
  Length                                              = sizeof (GET_IMAGE_FW_VER_REQUEST);
  RecvLength                                          = sizeof (GET_IMAGE_FW_VER_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetImageFwVersion.Request,
             Length,
             (UINT32 *) &GetImageFwVersion.Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status) &&
      (GetImageFwVersion.Response.MkhiHeader.Fields.Command == GEN_GET_IMAGE_FW_VERSION) &&
      (GetImageFwVersion.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (GetImageFwVersion.Response.MkhiHeader.Fields.Result == 0) &&
      (GetImageFwVersion.Response.ManifestData.PartitionId == PartitionId)) {
    *PartitionIdData = GetImageFwVersion.Response.ManifestData;
  }

  return Status;
}

/**
  This message is sent by BIOS to set the soft straps override configuration

  @param[in]  StrapGroupsNumber   Number of strap groups
  @param[in]  StrapOverrideData   Strap override data
  @param[out] Flags               Pointer to flags

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciSetStrapOverrideConfig (
  IN  UINT32              StrapGroupsNumber,
  IN  STRAP_OVERRIDE_DATA *StrapOverrideData,
  OUT UINT32              *Flags
  )
{
  EFI_STATUS                     Status;
  GEN_SET_STRAP_OVERRIDE_BUFFER  *SetStrapOverride;
  UINT32                         ReqLength;
  UINT32                         AckLength;

  if (StrapOverrideData == NULL || Flags == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ((StrapGroupsNumber != 1) || (StrapOverrideData->StrapGroupId != StrapGroupModularIoTypeCConfigStraps)) {
    return EFI_UNSUPPORTED;
  }

  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  ReqLength        = sizeof (GEN_SET_STRAP_OVERRIDE_REQUEST) + (StrapGroupsNumber * sizeof (STRAP_OVERRIDE_DATA));
  AckLength        = sizeof (GEN_SET_STRAP_OVERRIDE_RESPONSE);
  SetStrapOverride = AllocateZeroPool (MAX (ReqLength, AckLength));
  if (SetStrapOverride == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  SetStrapOverride->Request.MkhiHeader.Data                         = 0;
  SetStrapOverride->Request.MkhiHeader.Fields.GroupId               = MKHI_GEN_GROUP_ID;
  SetStrapOverride->Request.MkhiHeader.Fields.Command               = GEN_SET_STRAP_OVERRIDE_CMD;
  SetStrapOverride->Request.StrapGroupsNumber                       = StrapGroupsNumber;
  CopyMem (SetStrapOverride->Request.StrapsData, StrapOverrideData, StrapGroupsNumber * sizeof (STRAP_OVERRIDE_DATA));

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &SetStrapOverride->Request,
             ReqLength,
             (UINT32 *) &SetStrapOverride->Response,
             &AckLength
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[PEI] ME-BIOS: Set Strap Override Request Exit - Error by message sent - %r.\n", Status));
  } else {
    if ((SetStrapOverride->Response.MkhiHeader.Fields.IsResponse == 1) &&
        (SetStrapOverride->Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
      *Flags = SetStrapOverride->Response.Flags;
    } else {
      DEBUG ((DEBUG_ERROR, "[PEI] ME-BIOS: Set Strap Override Request Exit, MkhiResult = 0x%02x\n", SetStrapOverride->Response.MkhiHeader.Fields.Result));
      Status = EFI_DEVICE_ERROR;
    }
  }

  FreePool (SetStrapOverride);
  return Status;
}

/**
  This message is sent by BIOS to retrieve the soft straps override configuration

  @param[in]     StrapGroupId        Strap group ID
  @param[in out] StrapGroupsNumber   Number of strap groups
  @param[out]    StrapOverrideData   Strap override data

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER      Invalid input parameter
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetStrapOverrideConfig (
  IN     STRAP_GROUPS        StrapGroupId,
  IN OUT UINT32              *StrapGroupsNumber,
  OUT    STRAP_OVERRIDE_DATA *StrapOverrideData
  )
{
  EFI_STATUS                     Status;
  GEN_GET_STRAP_OVERRIDE_BUFFER  *GetStrapOverride;
  UINT32                         ReqLength;
  UINT32                         AckLength;

  if (StrapGroupsNumber == NULL || StrapOverrideData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ((*StrapGroupsNumber != 1) || (StrapGroupId != StrapGroupModularIoTypeCConfigStraps)) {
    return EFI_UNSUPPORTED;
  }

  ReqLength        = sizeof (GEN_GET_STRAP_OVERRIDE_REQUEST);
  AckLength        = sizeof (GEN_GET_STRAP_OVERRIDE_RESPONSE) + (*StrapGroupsNumber * sizeof (STRAP_OVERRIDE_DATA));
  GetStrapOverride = AllocateZeroPool (MAX (ReqLength, AckLength));
  if (GetStrapOverride == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  GetStrapOverride->Request.MkhiHeader.Data           = 0;
  GetStrapOverride->Request.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  GetStrapOverride->Request.MkhiHeader.Fields.Command = GEN_GET_STRAP_OVERRIDE_CMD;
  GetStrapOverride->Request.StrapGroupId              = StrapGroupId;

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetStrapOverride->Request,
             ReqLength,
             (UINT32 *) &GetStrapOverride->Response,
             &AckLength
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[PEI] ME-BIOS: Get Strap Override Request Exit - Error by message sent - %r.\n", Status));
  } else {
    if ((GetStrapOverride->Response.MkhiHeader.Fields.IsResponse == 1) &&
        (GetStrapOverride->Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
      CopyMem (StrapOverrideData, GetStrapOverride->Response.StrapsData, *StrapGroupsNumber * sizeof (STRAP_OVERRIDE_DATA));
      *StrapGroupsNumber = GetStrapOverride->Response.StrapGroupsNumber;
    } else {
      DEBUG ((DEBUG_ERROR, "[PEI] ME-BIOS: Get Strap Override Request Exit, MkhiResult = 0x%02x\n", GetStrapOverride->Response.MkhiHeader.Fields.Result));
      Status = EFI_DEVICE_ERROR;
    }
  }

  FreePool (GetStrapOverride);
  return Status;
}

//
// BUP_COMMON_GROUP_ID = 0xF0
//

/**
  Send DRAM init done message through HECI to inform ME of memory initialization done.

  @param[in]  ImrBaseLow                Base address for IMR region (Low DWORD)
  @param[in]  ImrBaseHigh               Base address for IMR region (High DWORD)
  @param[in]  MemStatus                 Memory init status
  @param[in]  PchBusNumber              Pch Bus Number
  @param[out] MkhiResult                MKHI Error Code
  @param[out] BiosAction                ME response to DID

  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_DEVICE_ERROR              HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                   HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED               Message is not supported for current ME
**/
EFI_STATUS
PeiHeciSendDid (
  IN  UINT32  ImrBaseLow,
  IN  UINT32  ImrBaseHigh,
  IN  UINT8   MemStatus,
  IN  UINT8   PchBusNumber,
  OUT UINT8   *MkhiResult,
  OUT UINT8   *BiosAction
  )
{
  EFI_STATUS                 Status;
  DRAM_INIT_DONE_CMD_BUFFER  DidBuffer;
  UINT32                     ReqLength;
  UINT32                     RespLength;

  ZeroMem (&DidBuffer, sizeof(DRAM_INIT_DONE_CMD_BUFFER));
  DidBuffer.Request.MkhiHeader.Data           = 0;
  DidBuffer.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  DidBuffer.Request.MkhiHeader.Fields.Command = DRAM_INIT_DONE_CMD;
  DidBuffer.Request.ImrData.BiosImrsBaLow     = ImrBaseLow;
  DidBuffer.Request.ImrData.BiosImrsBaHigh    = ImrBaseHigh;
  DidBuffer.Request.MemStatus                 = MemStatus;
  DidBuffer.Request.Rs3BusNumber              = PchBusNumber;
  ReqLength                                   = sizeof (DRAM_INIT_DONE_CMD_REQUEST);
  RespLength                                  = sizeof (DRAM_INIT_DONE_CMD_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &DidBuffer.Request,
             ReqLength,
             (UINT32 *) &DidBuffer.Response,
             &RespLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((DidBuffer.Response.MkhiHeader.Fields.GroupId != BUP_COMMON_GROUP_ID) ||
      (DidBuffer.Response.MkhiHeader.Fields.Command != DRAM_INIT_DONE_CMD) ||
      (DidBuffer.Response.MkhiHeader.Fields.IsResponse == 0) ||
      (DidBuffer.Response.MkhiHeader.Fields.Result != MkhiStatusSuccess)) {
    DEBUG ((DEBUG_ERROR, "HeciSend DRAM Init Done unsuccessful - no response\n"));
    return EFI_DEVICE_ERROR;
  }

  *MkhiResult     = (UINT8)DidBuffer.Response.MkhiHeader.Fields.Result;
  *BiosAction     = DidBuffer.Response.BiosAction;

  if (*MkhiResult != MkhiStatusSuccess) {
    DEBUG ((DEBUG_WARN, "HeciSend DRAM Init Done processed with MKHI Error Code. Response: \n"));
    DEBUG ((DEBUG_WARN, "  MkhiResult = 0x%02x\n",  *MkhiResult));
  } else {
    DEBUG ((DEBUG_INFO, "HeciSend DRAM Init Done successful. Response:\n"));
  }
  DEBUG ((DEBUG_INFO, "  Pci2Priv Base Low = 0x%08x\n",  DidBuffer.Response.Pci2PrivBase.AdrLow));
  DEBUG ((DEBUG_INFO, "  Pci2Priv Base High = 0x%08x\n", DidBuffer.Response.Pci2PrivBase.AdrHigh));
  DEBUG ((DEBUG_INFO, "  BIOS Action = 0x%02x\n",        DidBuffer.Response.BiosAction));

  return Status;
}

/**
  Send Get MBP from FW

  @param[out]     MbpEntriesNumbe   Number of MBP entries
  @param[in, out] MbpItems          MBP items of the response
  @paran[in]      SkipMbp           Skip MBP

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
                                    or response is incorrect
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_INVALID_PARAMETER     Invalid input parameter
**/
EFI_STATUS
PeiHeciGetMbpMsg (
  OUT UINT32                   *MbpEntriesNumber,
  IN OUT UINT32                *MbpItems,
  IN BOOLEAN                   SkipMbp
  )
{
  EFI_STATUS     Status;
  GET_MBP_BUFFER GetMbp;
  UINT32         Length;
  UINT32         RecvLength;

  if ((MbpEntriesNumber == NULL) || (MbpItems == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  GetMbp.Request.MkhiHeader.Data           = 0;
  GetMbp.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  GetMbp.Request.MkhiHeader.Fields.Command = MBP_REQUEST_CMD;
  GetMbp.Request.SkipMbp                   = SkipMbp;
  Length                                   = sizeof (GET_MBP_REQUEST);
  RecvLength                               = sizeof (GET_MBP_RESPONSE);

  ///
  /// Send Get MBP Request to ME
  ///
  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetMbp.Request,
             Length,
             (UINT32 *) &GetMbp.Response,
             &RecvLength
             );

  DEBUG ((DEBUG_INFO, "ReadMsg returned %r\n", Status));
  DEBUG ((DEBUG_INFO, "MsgGetMbpAck->MkhiHeader.Fields.Command = %d\n", GetMbp.Response.MkhiHeader.Fields.Command));
  DEBUG ((DEBUG_INFO, "MsgGetMbpAck->MkhiHeader.Fields.IsResponse = %d\n", GetMbp.Response.MkhiHeader.Fields.IsResponse));
  DEBUG ((DEBUG_INFO, "MsgGetMbpAck->MkhiHeader.Fields.Result = %d\n", GetMbp.Response.MkhiHeader.Fields.Result));

  *MbpEntriesNumber = 0;
  if (!EFI_ERROR (Status)) {
    if (!SkipMbp &&
        (GetMbp.Response.MkhiHeader.Fields.Command == MBP_REQUEST_CMD) &&
        (GetMbp.Response.MkhiHeader.Fields.IsResponse == 1) &&
        (GetMbp.Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
      *MbpEntriesNumber = GetMbp.Response.MbpHeader.NumEntries;
      CopyMem (MbpItems, &GetMbp.Response.MbpItems, (GetMbp.Response.MbpHeader.MbpSize * sizeof(UINT32) - sizeof (MBP_HEADER)));
    } else {
      Status = EFI_DEVICE_ERROR;
    }
  }

  DEBUG ((DEBUG_INFO, "No. of Mbp Entries: %d\n", GetMbp.Response.MbpHeader.NumEntries));

  //
  // Clear buffers from sensitive data
  //
  ZeroMem (&GetMbp.Response, sizeof (GET_MBP_RESPONSE));
  return Status;
}

/**
  This message is sent by the BIOS to retrieve from CSME total size of IMRs.
  BIOS needs to provide mask of disabled IMRs and requested size for PCIe IMR.

  @param[in]  BiosImrDisableMask0  Low DWORD of BIOS IMR Disable mask
  @param[in]  BiosImrDisableMask1  High DWORD of BIOS IMR Disable mask
  @param[in]  PciImrSize           Requested IMR size for PCI
  @param[out] MkhiResult           MKHI Error Code
  @param[out] ImrsSize             Total IMR size
  @param[out] Alignment            Required address alignment

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciGetImrSizeMsg (
  IN  UINT32     BiosImrDisableMask0,
  IN  UINT32     BiosImrDisableMask1,
  IN  UINT32     PciImrSize,
  OUT UINT8      *MkhiResult,
  OUT UINT32     *ImrsSize,
  OUT UINT32     *Alignment
  )
{
  EFI_STATUS              Status;
  GET_IMR_CMD_BUFFER      GetImrCmd;
  UINT32                  ReqLength;
  UINT32                  RespLength;

  //PostCodeWithDescription (0xE08, "ME-BIOS: Get IMR Size Entry.");
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_HECI_GETIMR_START); //PostCode(0xE08)
  DEBUG ((DEBUG_INFO, "ME-BIOS: Get IMR Size Entry.\n"));

  *ImrsSize  = 0;
  *Alignment = 0;

  GetImrCmd.Request.MkhiHeader.Data           = 0;
  GetImrCmd.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  GetImrCmd.Request.MkhiHeader.Fields.Command = GET_IMR_SIZE_CMD;
  GetImrCmd.Request.Data.BiosImrDisableMask0  = BiosImrDisableMask0;
  GetImrCmd.Request.Data.BiosImrDisableMask1  = BiosImrDisableMask1;
  GetImrCmd.Request.Data.Pci2PrivSize         = (UINT32) PciImrSize;
  ReqLength                                   = sizeof (GET_IMR_CMD_REQUEST);
  RespLength                                  = sizeof (GET_IMR_CMD_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetImrCmd.Request,
             ReqLength,
             (UINT32 *) &GetImrCmd.Response,
             &RespLength
             );
  if (EFI_ERROR (Status)) {
    if (Status == EFI_NOT_FOUND) {
      REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_HECI_GETIMR_EXIT_ERR_NOHECI); //PostCode(0xE88)
      DEBUG ((DEBUG_INFO, "ME-BIOS: Get IMR Size Exit - Error by HECI service not found.\n"));
      return EFI_UNSUPPORTED;
    }
    return Status;
  }

  if ((GetImrCmd.Response.MkhiHeader.Fields.GroupId != BUP_COMMON_GROUP_ID) ||
      (GetImrCmd.Response.MkhiHeader.Fields.Command != GET_IMR_SIZE_CMD) ||
      (GetImrCmd.Response.MkhiHeader.Fields.IsResponse == 0) ||
      (GetImrCmd.Response.MkhiHeader.Fields.Result != MkhiStatusSuccess)) {
    //PostCodeWithDescription (0xEA8, "ME-BIOS: Get IMR Size Exit - Error by no response.");
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_HECI_GETIMR_EXIT_ERR_NORESPONSE); //PostCode(0xEA8)
    DEBUG ((DEBUG_INFO, "ME-BIOS: Get IMR Size Exit - Error by no response.\n"));
    return EFI_DEVICE_ERROR;
  }

  *MkhiResult = (UINT8)GetImrCmd.Response.MkhiHeader.Fields.Result;
  *ImrsSize   = GetImrCmd.Response.Data.ImrsSize;
  *Alignment  = GetImrCmd.Response.Data.Alignment;

  if (*MkhiResult != MkhiStatusSuccess) {
    DEBUG ((DEBUG_WARN, "Heci Get IMR Size processed with MKHI Error Code. Response: \n"));
    DEBUG ((DEBUG_WARN, "  MkhiResult = 0x%02x\n", *MkhiResult));
  } else {
    DEBUG ((DEBUG_INFO, "Heci Get IMR Size Msg successful. Response:\n"));
  }

  DEBUG ((DEBUG_INFO, "  IMR Size = 0x%08x\n",          *ImrsSize));
  DEBUG ((DEBUG_INFO, "  Largest IMR Size = 0x%08x\n",  *Alignment));
  DEBUG ((DEBUG_INFO, "  Flags = 0x%08x\n",             GetImrCmd.Response.Data.Flags));
  //PostCodeWithDescription (0xE28, "ME-BIOS: Get IMR Size Exit - Success.");
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_HECI_GETIMR_EXIT_SUCCESS); //PostCode(0xE28)
  DEBUG ((DEBUG_INFO, "ME-BIOS: Get IMR Size Exit - Success.\n"));

  return Status;
}

/**
  Send Manufacturing Reset and Halt

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciBupManufacturingResetAndHalt (
  VOID
  )
{
  EFI_STATUS                  Status;
  MANUF_RESET_AND_HALT_BUFFER ManufResetAndHalt;
  UINT32                      Length;
  UINT32                      RecvLength;

  ManufResetAndHalt.Request.MkhiHeader.Data           = 0;
  ManufResetAndHalt.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  ManufResetAndHalt.Request.MkhiHeader.Fields.Command = MANUF_RESET_AND_HALT_CMD;
  Length                                              = sizeof (MANUF_RESET_AND_HALT_REQUEST);
  RecvLength                                          = sizeof (MANUF_RESET_AND_HALT_RESPONSE);

  ///
  /// Send Manufacturing Reset and Halt Request to ME
  ///
  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &ManufResetAndHalt.Request,
             Length,
             (UINT32 *) &ManufResetAndHalt.Response,
             &RecvLength
             );

  if (ManufResetAndHalt.Response.MkhiHeader.Fields.Result != MkhiStatusSuccess) {
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  This command is sent by the BIOS to clear CSME data to image creation state.
  After the command succeeds, platform reset is expected.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_UNSUPPORTED         Get ME mode fail.
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout or transition failed
                                  within allowed time limit
**/
EFI_STATUS
PeiHeciDataClear (
  VOID
  )
{
  EFI_STATUS         Status;
  DATA_CLEAR_BUFFER  DataClear;
  UINT32             Length;
  UINT32             RespLength;
  UINT32             MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status)) {
    //
    // CSME should support this command in all modes.
    //
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&DataClear, sizeof (DATA_CLEAR_BUFFER));
  DataClear.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  DataClear.Request.MkhiHeader.Fields.Command = DATA_CLEAR_CMD;
  Length                                      = sizeof (DATA_CLEAR_REQUEST);
  RespLength                                  = sizeof (DATA_CLEAR_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &DataClear,
             Length,
             (UINT32 *) &DataClear,
             &RespLength
             );
  if (!EFI_ERROR (Status) &&
      ((DataClear.Response.MkhiHeader.Fields.Command != DATA_CLEAR_CMD) ||
      (DataClear.Response.MkhiHeader.Fields.IsResponse != 1) ||
      (DataClear.Response.MkhiHeader.Fields.Result != MkhiStatusSuccess))) {
    Status = EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "%a() exit, Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  This message is sent by the BIOS to tell CSME that Buffer has been allocated for payload to be copied.

  @param[in]      LoadRequest            IP information for which firmware has to be loaded
  @param[in]      BufferAddressLow       Lower DRAM address of the buffer
  @param[in]      BufferAddressHigh      Upper DRAM address of the buffer
  @param[in, out] BufferSize             On input, BufferSize is size of Buffer allocated by Bios.On output, actual PayloadSize copied by ME
  @param[out]     IpLoaderStatus         IP Loading status from CSME
  @param[out]     HashType               Hash algorithm used for message digest
  @param[out]     Hash                   Hash Value

  @retval EFI_SUCCESS                    Command succeeded
  @retval EFI_DEVICE_ERROR               HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                    HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED                Current ME mode doesn't support sending this message through this HECI or Input parameters are not valid.
  @retval EFI_BUFFER_TOO_SMALL           Message Buffer is too small for the Acknowledge
  @retval EFI_INVALID_PARAMETER          Parameter invalid
**/
EFI_STATUS
PeiHeciGetFwPayload (
  IN     UINT32        LoadRequest,
  IN     UINT32        BufferAddressLow,
  IN     UINT32        BufferAddressHigh,
  IN OUT UINT32        *BufferSize,
  OUT    UINT32        *IpLoaderStatus,
  OUT    UINT8         *HashType,
  OUT    UINT8         *Hash
  )
{
  EFI_STATUS                 Status;
  IP_LOADING_CMD_BUFFER      *IpLoad;
  UINT32                     ReqLength;
  UINT32                     RespLength;

  DEBUG ((DEBUG_INFO, "%a () enter\n", __FUNCTION__));

  if (((BufferAddressLow == 0) && (BufferAddressHigh == 0)) ||
      (BufferSize == NULL) ||
      (IpLoaderStatus == NULL) ||
      (HashType == NULL) ||
      (Hash == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  ReqLength = sizeof (IP_LOADING_CMD_REQUEST);
  RespLength = sizeof (IP_LOADING_CMD_RESPONSE) + MAX_DIGEST_SIZE;
  IpLoad = AllocateZeroPool (RespLength);
  if (IpLoad == NULL) {
    DEBUG ((DEBUG_ERROR, "%a(): Could not allocate Memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  IpLoad->Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  IpLoad->Request.MkhiHeader.Fields.Command = GET_IP_FIRMWARE_CMD;
  IpLoad->Request.Version                   = IP_LOADING_CMD_VERSION;
  IpLoad->Request.Operation                 = LoadRequest;
  IpLoad->Request.DramBaseLow               = BufferAddressLow;
  IpLoad->Request.DramBaseHigh              = BufferAddressHigh;
  IpLoad->Request.MemorySize                = *BufferSize;

  DEBUG ((DEBUG_INFO, "BufferAddressHigh %x, BufferAddressLow %x\n", BufferAddressHigh, BufferAddressLow));
  ///
  /// Send Ip Load Request to ME
  ///
  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) IpLoad,
             ReqLength,
             (UINT32 *) IpLoad,
             &RespLength
             );
  if (EFI_ERROR (Status)) {
    FreePool (IpLoad);
    return Status;
  }

  if ((IpLoad->Response.MkhiHeader.Fields.GroupId != BUP_COMMON_GROUP_ID) ||
      (IpLoad->Response.MkhiHeader.Fields.Command != GET_IP_FIRMWARE_CMD) ||
      (IpLoad->Response.MkhiHeader.Fields.IsResponse == 0) ||
      (IpLoad->Response.MkhiHeader.Fields.Result != MkhiStatusSuccess)) {
    DEBUG ((DEBUG_ERROR, "Heci Get IP Load Msg Failed\n"));
    FreePool (IpLoad);
    return EFI_DEVICE_ERROR;
  }

  *BufferSize     = IpLoad->Response.PayloadSize;
  *IpLoaderStatus = IpLoad->Response.Status;
  *HashType       = IpLoad->Response.HashType;

  DEBUG ((DEBUG_INFO, "Heci Ip Load Request Msg successful. Response:\n"));
  DEBUG ((DEBUG_INFO, "  PayloadSize = 0x%08x\n", *BufferSize));
  DEBUG ((DEBUG_INFO, "  Hash Type used for Signing Payload = 0x%08x\n", IpLoad->Response.HashType));
  CopyMem (Hash, IpLoad->Response.Hash, MAX_DIGEST_SIZE);

  FreePool (IpLoad);
  DEBUG ((DEBUG_INFO, "%a () exit\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  This message is sent by the BIOS to retrieve debug token data from CSME.

  @param[out] KnobData              Debug Token Knob Data

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_NOT_FOUND             Couldn't locate HeciPpi or debug token data
  @retval EFI_INVALID_PARAMETER     Parameter invalid
**/
EFI_STATUS
PeiHeciGetDebugTokenData (
  OUT UINT32                   *KnobData
  )
{
  EFI_STATUS                   Status;
  GET_DEBUG_TOKEN_DATA_BUFFER  GetDebugTokenData;
  UINT32                       Length;
  UINT32                       RespLength;
  UINT32                       MeMode;

  DEBUG ((DEBUG_INFO, "%a(): Start\n", __FUNCTION__));

  if (KnobData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&GetDebugTokenData, sizeof (GET_DEBUG_TOKEN_DATA_BUFFER));
  GetDebugTokenData.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  GetDebugTokenData.Request.MkhiHeader.Fields.Command = GET_DEBUG_TOKEN_DATA_CMD;
  Length                                              = sizeof (GET_DEBUG_TOKEN_DATA_REQUEST);
  RespLength                                          = sizeof (GET_DEBUG_TOKEN_DATA_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetDebugTokenData,
             Length,
             (UINT32 *) &GetDebugTokenData,
             &RespLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((GetDebugTokenData.Response.MkhiHeader.Fields.Command == GET_DEBUG_TOKEN_DATA_CMD) &&
      (GetDebugTokenData.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (GetDebugTokenData.Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
    CopyMem (KnobData, &GetDebugTokenData.Response.KnobData, sizeof (UINT32));
  } else {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  This message is sent by the BIOS to retrieve device specific seed from CSME.

  @param[in] BiosSeedProperties     Seed properties
  @param[in] LockSeedGeneration     0 - No lock
                                    1 - Explicit lock requested by BIOS
  @param[out] EphemeralKeyCounter   Random counter to be used with ephemeral key for trusted channel
  @param[out] BiosSeedInfo          Seed information
  @param[out] BiosSeed              BIOS seed. If BIOS requested 32 bytes only, then the CSME will
                                    return the seed in first 32 bytes and fill rest of buffer by 0

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_NOT_FOUND             Couldn't locate HeciPpi or debug token data
  @retval EFI_INVALID_PARAMETER     Parameter invalid
**/
EFI_STATUS
PeiHeciGetBiosSeed (
  IN  SEED_PROPERTIES *BiosSeedProperties,
  IN  UINT8           LockSeedGeneration,
  OUT UINT32          *EphemeralKeyCounter,
  OUT SEED_INFO       *BiosSeedInfo,
  OUT UINT8           *BiosSeed
  )
{
  EFI_STATUS                   Status;
  GET_BIOS_SEED_BUFFER         GetBiosSeed;
  UINT32                       Length;
  UINT32                       RespLength;

  DEBUG ((DEBUG_INFO, "%a(): Start\n", __FUNCTION__));

  if (BiosSeedProperties == NULL || EphemeralKeyCounter == NULL || BiosSeedInfo == NULL || BiosSeed == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&GetBiosSeed, sizeof (GET_BIOS_SEED_BUFFER));
  GetBiosSeed.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  GetBiosSeed.Request.MkhiHeader.Fields.Command = GET_BIOS_SEED_CMD;
  GetBiosSeed.Request.LockSeedGeneration        = LockSeedGeneration;
  CopyMem (
    &(GetBiosSeed.Request.BiosSeedProperties),
    BiosSeedProperties,
    sizeof (SEED_PROPERTIES)
  );

  Length      = sizeof (GET_BIOS_SEED_REQUEST);
  RespLength  = sizeof (GET_BIOS_SEED_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetBiosSeed,
             Length,
             (UINT32 *) &GetBiosSeed,
             &RespLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((GetBiosSeed.Response.MkhiHeader.Fields.Command == GET_BIOS_SEED_CMD) &&
      (GetBiosSeed.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (GetBiosSeed.Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
    *EphemeralKeyCounter = GetBiosSeed.Response.EphemeralKeyCounter;
    CopyMem (BiosSeedInfo, &GetBiosSeed.Response.BiosSeedInfo, sizeof (SEED_INFO));
    CopyMem (BiosSeed, &GetBiosSeed.Response.BiosSeed, BOOTLOADER_SEED_LEN_EX * sizeof (UINT8));
  } else {
    Status = EFI_DEVICE_ERROR;
  }

  //
  // Clear security data in infrequently mapped memory.
  //
  ZeroMem (&GetBiosSeed, sizeof (GET_BIOS_SEED_BUFFER));

  return Status;
}

/**
  The function sends a request to load a binary using HECI and waits for an acknowledgment.

  @param[in] IdsCount             The number of binary IDs in the list.
  @param[in] BinaryIdsList        Pointer to the list of binary IDs.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function.
  @retval EFI_INVALID_PARAMETER   BinaryIdsList is NULL.
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate required resources.
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally.
**/
EFI_STATUS
PeiHeciLoadBinaryMsg (
  IN UINT32          IdsCount,
  IN UINT32          *BinaryIdsList
  )
{
  EFI_STATUS         Status;
  LOAD_BINARY_BUFFER *LoadBinary;
  UINT32             MeMode;
  UINT32             Length;
  UINT32             RespLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  if (BinaryIdsList == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  LoadBinary = AllocateZeroPool (IdsCount * sizeof (UINT32) + sizeof (LOAD_BINARY_BUFFER));
  if (LoadBinary == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  LoadBinary->Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  LoadBinary->Request.MkhiHeader.Fields.Command = LOAD_BINARY_CMD;
  LoadBinary->Request.IdsCount                  = IdsCount;
  CopyMem (&LoadBinary->Request.BinaryIds, BinaryIdsList, IdsCount * sizeof (UINT32));

  Length                                        = IdsCount * sizeof (UINT32) + sizeof (LOAD_BINARY_REQUEST);
  RespLength                                    = sizeof (LOAD_BINARY_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) LoadBinary,
             Length,
             (UINT32 *) LoadBinary,
             &RespLength
             );

  if (EFI_ERROR (Status) ||
      ((LoadBinary->Response.MkhiHeader.Fields.Command) != LOAD_BINARY_CMD) ||
      ((LoadBinary->Response.MkhiHeader.Fields.IsResponse) != 1) ||
      (LoadBinary->Response.MkhiHeader.Fields.Result != MkhiStatusSuccess)) {
    Status = EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "%a() exit, Status = %r\n", __FUNCTION__, Status));

  FreePool (LoadBinary);

  return Status;
}

//
// BUP_ICC_GROUP_ID = 0xF1
//

/**
  Send ICC request through HECI to query if CSME FW requires the warm reset flow from a previous boot.

  @param[out] WarmResetRequired   1 - CSME requires a warm reset to complete BCLK ramp en flow

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciIccBclkMsg (
  OUT UINT8    *WarmResetRequired
  )
{
  EFI_STATUS        Status;
  ICC_CMD_BUFFER    IccCmdBuffer;
  UINT32            Length;
  UINT32            RespLength;

  DEBUG ((DEBUG_INFO, "(ICC) %a(): Start\n", __FUNCTION__));

  IccCmdBuffer.Response.Data.FwNeedsWarmResetFlag = 0; //@Todo: remove this after CSME fixes response structure issue.
  IccCmdBuffer.Request.MkhiHeader.Data            = 0;
  IccCmdBuffer.Request.MkhiHeader.Fields.GroupId  = BUP_ICC_GROUP_ID;
  IccCmdBuffer.Request.MkhiHeader.Fields.Command  = ICC_CMD;
  Length                                          = sizeof (ICC_CMD_REQUEST);
  RespLength                                      = sizeof (ICC_CMD_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &IccCmdBuffer.Request,
             Length,
             (UINT32 *) &IccCmdBuffer.Response,
             &RespLength
             );

  if (EFI_ERROR(Status) ||
      (IccCmdBuffer.Response.MkhiHeader.Fields.GroupId != BUP_ICC_GROUP_ID) ||
      (IccCmdBuffer.Response.MkhiHeader.Fields.Command != ICC_CMD) ||
      (IccCmdBuffer.Response.MkhiHeader.Fields.IsResponse != 1) ||
      (IccCmdBuffer.Response.MkhiHeader.Fields.Result != MkhiStatusSuccess)) {
    Status = EFI_DEVICE_ERROR;
  } else {
    *WarmResetRequired = (UINT8) IccCmdBuffer.Response.Data.FwNeedsWarmResetFlag;
  }

  return Status;
}


//
// BUP_MPHY_GROUP_ID = 0xF2
//

/**
  Get the Mphy data from CSME

  @param[out] Data                Pointer to the data buffer.
  @param[in]  DataSize            Pointer to the length of the table in bytes

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES    HECI Could not allocate Memory
  @retval EFI_NOT_FOUND           Couldn't locate HeciPpi.
**/
EFI_STATUS
PeiHeciReadFromMphy (
  OUT UINT8  *Data,
  IN  UINT32 DataSize
  )
{
  EFI_STATUS                        Status;
  BUP_MPHY_READ_FROM_MPHY_BUFFER    *HsioReadMphy;
  UINT32                            Length;
  UINT32                            RecvLength;

  if ((Data == NULL) || (DataSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "%a(): Start\n", __FUNCTION__));

  Length     = sizeof (BUP_MPHY_READ_FROM_MPHY_REQUEST);
  RecvLength = sizeof (BUP_MPHY_READ_FROM_MPHY_RESPONSE) + DataSize;

  HsioReadMphy = AllocateZeroPool (RecvLength);
  if (HsioReadMphy == NULL) {
    DEBUG ((DEBUG_ERROR, "%a(): Could not allocate Memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (HsioReadMphy, sizeof (BUP_MPHY_READ_FROM_MPHY_BUFFER));

  HsioReadMphy->Request.MkhiHeader.Data           = 0;
  HsioReadMphy->Request.MkhiHeader.Fields.GroupId = BUP_MPHY_GROUP_ID;
  HsioReadMphy->Request.MkhiHeader.Fields.Command = MPHY_READ_FROM_MPHY_CMD;
  HsioReadMphy->Request.NumOfBytes                = DataSize;

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) HsioReadMphy,
             Length,
             (UINT32 *) HsioReadMphy,
             &RecvLength
             );

  if (!EFI_ERROR (Status) &&
      (HsioReadMphy->Response.MkhiHeader.Fields.Command == MPHY_READ_FROM_MPHY_CMD) &&
      (HsioReadMphy->Response.MkhiHeader.Fields.IsResponse == 1) &&
      (HsioReadMphy->Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
    CopyMem (Data, HsioReadMphy->Response.Data, DataSize);
  }

  FreePool (HsioReadMphy);

  DEBUG ((DEBUG_INFO, "%a(): End\n", __FUNCTION__));

  return Status;
}


//
// BUP_PM_GROUP_ID = 0xF3
//

/**
  Send Host Reset Notification Message to determine if warm reset is required.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciSendHostResetNotificationMsg (
  VOID
  )
{
  EFI_STATUS             Status;
  UINT32                 Length;
  UINT32                 RecvLength;
  HR_NOTIFICATION_BUFFER HrNotification;

  HrNotification.Request.MkhiHeader.Fields.IsResponse  = 0;
  HrNotification.Request.MkhiHeader.Fields.Result      = 0;
  HrNotification.Request.MkhiHeader.Data               = 0;
  HrNotification.Request.MkhiHeader.Fields.GroupId     = BUP_PM_GROUP_ID;
  HrNotification.Request.MkhiHeader.Fields.Command     = NPCR_NOTIFICATION_CMD;
  Length                                               = sizeof (HR_NOTIFICATION_CMD_REQUEST);
  RecvLength                                           = sizeof (HR_NOTIFICATION_CMD_RESPONSE);

  ///
  /// Send Host Reset Notification Request to ME
  ///
  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &HrNotification.Request,
             Length,
             (UINT32 *) &HrNotification.Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status) &&
      (HrNotification.Response.MkhiHeader.Fields.GroupId == BUP_PM_GROUP_ID) &&
      (HrNotification.Response.MkhiHeader.Fields.Command == NPCR_NOTIFICATION_CMD) &&
      (HrNotification.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (HrNotification.Response.MkhiHeader.Fields.Result == MkhiStatusSuccess) &&
      (HrNotification.Response.ResetRequested == 0x1)) {
    (*GetPeiServicesTablePointer ())->ResetSystem2 (EfiResetWarm, EFI_SUCCESS, 0, NULL);
  } else {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

//
// BUP_BIOS_AR_GROUP_ID = 0xF4
//

/**
  Requests the current monotonic counter from the designated RPMC counter.

  @param[in]    CounterIndex            Counter Index to access.
  @param[out]   CounterValue            A pointer to a buffer to store the RPMC value.

  @retval       EFI_SUCCESS             The operation completed successfully.
  @retval       EFI_DEVICE_ERROR        A device error occurred while attempting to update the counter.
  @retval       EFI_UNSUPPORTED         The operation is un-supported.
**/
EFI_STATUS
PeiHeciRequestMonotonicCounter (
  IN  UINT8   CounterIndex,
  OUT UINT32  *CounterValue
  )
{
  EFI_STATUS              Status;
  BIOS_AR_READ_MC_BUFFER  RpmcRead;
  UINT32                  ReqLength;
  UINT32                  RespLength;

  ///
  /// Set up Read Monotonic Counter Heci message
  ///
  RpmcRead.Request.MkhiHeader.Data           = 0;
  RpmcRead.Request.MkhiHeader.Fields.GroupId = BUP_BIOS_AR_GROUP_ID;
  RpmcRead.Request.MkhiHeader.Fields.Command = BUP_MKHI_READ_MC_CMD;
  ReqLength                                  = sizeof (BIOS_AR_READ_MC_REQUEST);
  RespLength                                 = sizeof (BIOS_AR_READ_MC_RESPONSE);
  RpmcRead.Request.CounterId                 = CounterIndex;

  ///
  /// Send Read Monotonic Counter Heci message to CSME
  ///
  DEBUG ((DEBUG_INFO, "[RPMC] Send Monotonic Counter Request over HECI1, CounterId: 0x%02x\n", RpmcRead.Request.CounterId));
  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &RpmcRead,
             ReqLength,
             (UINT32 *) &RpmcRead,
             &RespLength
             );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ///
  /// Check Read Monotonic Counter message response for errors
  ///
  if ((RpmcRead.Response.MkhiHeader.Fields.Command == BUP_MKHI_READ_MC_CMD) &&
      (RpmcRead.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (RpmcRead.Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
    ///
    /// Retrieve the monotonic counter value
    ///
    *CounterValue = RpmcRead.Response.Counter;
    DEBUG ((DEBUG_INFO, "[RPMC] Send Read Monotonic Counter Request Msg successful. Response:\n"));
    DEBUG ((DEBUG_INFO, "  Monotonic Counter[%d] Value = 0x%08x\n", CounterIndex, *CounterValue));
  } else {
    Status = EFI_DEVICE_ERROR;
    DEBUG ((DEBUG_INFO, "[RPMC] Send Read Monotonic Counter Request Msg processed with MKHI Error Code. Response: \n"));
    DEBUG ((DEBUG_INFO, "  MkhiResult = 0x%02x\n", (UINT8)RpmcRead.Response.MkhiHeader.Fields.Result));
  }

  return Status;
}

/**
  Increments the designated monotonic counter in the SPI flash device by 1.

  @param[in]    CounterIndex            Counter Index to access.

  @retval       EFI_SUCCESS             The operation completed successfully.
  @retval       EFI_DEVICE_ERROR        A device error occurred while attempting to update the counter.
  @retval       EFI_UNSUPPORTED         The operation is un-supported.
**/
EFI_STATUS
PeiHeciIncrementMonotonicCounter (
  IN  UINT8   CounterIndex
  )
{
  EFI_STATUS                  Status;
  BIOS_AR_INCREMENT_MC_BUFFER RpmcIncrement;
  UINT32                      ReqLength;
  UINT32                      RespLength;

  ///
  /// Set up Read Monotonic Counter Heci message
  ///
  RpmcIncrement.Request.MkhiHeader.Data           = 0;
  RpmcIncrement.Request.MkhiHeader.Fields.GroupId = BUP_BIOS_AR_GROUP_ID;
  RpmcIncrement.Request.MkhiHeader.Fields.Command = BUP_MKHI_INCREMENT_MC_CMD;
  ReqLength                                       = sizeof (BIOS_AR_INCREMENT_MC_REQUEST);
  RespLength                                      = sizeof (BIOS_AR_INCREMENT_MC_RESPONSE);
  RpmcIncrement.Request.CounterId                 = CounterIndex;

  ///
  /// Send Read Monotonic Counter Heci message to CSME
  ///
  DEBUG ((DEBUG_INFO, "[RPMC] Send Increment Monotonic Counter Request  HECI1, CounterId: 0x%02x\n", RpmcIncrement.Request.CounterId));
  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &RpmcIncrement,
             ReqLength,
             (UINT32 *) &RpmcIncrement,
             &RespLength
             );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ///
  /// Check Increment Monotonic Counter message response for errors
  ///
  if ((RpmcIncrement.Response.MkhiHeader.Fields.Command == BUP_MKHI_INCREMENT_MC_CMD) &&
      (RpmcIncrement.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (RpmcIncrement.Response.MkhiHeader.Fields.Result == 0)) {
    DEBUG ((DEBUG_INFO, "[RPMC] Send Increment Monotonic Counter Request Msg successful. Response:\n"));
    DEBUG ((DEBUG_INFO, "  Monotonic Counter[%d] Value = 0x%08x\n", CounterIndex, RpmcIncrement.Response.Counter));
  } else {
    Status = EFI_DEVICE_ERROR;
    DEBUG ((DEBUG_ERROR, "[RPMC] Send Increment Monotonic Counter Msg processed with MKHI Error Code. Response: \n"));
    DEBUG ((DEBUG_ERROR, "  MkhiResult = 0x%02x\n", (UINT8)RpmcIncrement.Response.MkhiHeader.Fields.Result));
  }

  return Status;
}

//
// ME Client - MCHI
//

//
// MCHI_MCA_GROUP_ID = 0x0A
//

/**
  This message is sent by the BIOS to save data in a file stored in ME firmware NVM.

  @param[in]   FileId             Id Number of file to write
  @param[in]   Offset             File offset
  @param[in]   DataSize           Size of data to write
  @param[in]   Flags              Flags
  @param[in]   DataBuffer         Pointer to the data buffer

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES    Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciSetFileExMsg (
  IN UINT32          FileId,
  IN UINT32          Offset,
  IN UINT32          DataSize,
  IN UINT8           Flags,
  IN UINT8           *DataBuffer
  )
{
  EFI_STATUS          Status;
  UINT32              Length;
  UINT32              RecvLength;
  SET_FILE_EX_BUFFER  *SetFileEx;
  UINT32              MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  Length     = sizeof (SET_FILE_EX_REQUEST) + DataSize;
  RecvLength = sizeof (SET_FILE_EX_RESPONSE);

  SetFileEx = AllocateZeroPool (Length);
  if (SetFileEx == NULL) {
    DEBUG ((DEBUG_ERROR, "%a Error: Could not allocate Memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  SetFileEx->Request.MchiHeader.Fields.GroupId = MCHI_MCA_GROUP_ID;
  SetFileEx->Request.MchiHeader.Fields.Command = McaSetFileExCmd;
  SetFileEx->Request.FileId                    = FileId;
  SetFileEx->Request.Offset                    = Offset;
  SetFileEx->Request.DataSize                  = DataSize;
  SetFileEx->Request.Flags                     = Flags;

  CopyMem (SetFileEx->Request.Data, DataBuffer, DataSize);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MCHI_MESSAGE_ADDR,
             (UINT32 *) SetFileEx,
             Length,
             (UINT32 *) SetFileEx,
             &RecvLength
             );

  if (SetFileEx->Response.MchiHeader.Fields.Result != McaStatusSuccess) {
    Status = EFI_DEVICE_ERROR;
  }

  FreePool (SetFileEx);

  return Status;
}

/**
  This message is sent by the BIOS after HeciSetFileExMsg to notify ME firmware that it needs to
  commit previously set files. Caller shold invoke global reset afterwards.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciCommitFilesMsg (
  VOID
  )
{
  EFI_STATUS    Status;
  UINT32        Length;
  UINT32        RecvLength;
  COMMIT_FILES  CommitFiles;
  UINT32        MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  CommitFiles.MchiHeader.Data           = 0;
  CommitFiles.MchiHeader.Fields.GroupId = MCHI_MCA_GROUP_ID;
  CommitFiles.MchiHeader.Fields.Command = McaCommitFilesCmd;
  Length                                = sizeof (COMMIT_FILES);
  RecvLength                            = sizeof (COMMIT_FILES);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MCHI_MESSAGE_ADDR,
             (UINT32 *) &CommitFiles,
             Length,
             (UINT32 *) &CommitFiles,
             &RecvLength
             );

  if (CommitFiles.MchiHeader.Fields.Result != McaStatusSuccess && CommitFiles.MchiHeader.Fields.Result != McaStatusGlobalResetRequired) {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  This message is sent by the BIOS to retrieve RPMC status data from CSME.

  @param[out] RpStatus            RPMC status data

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_NOT_FOUND           Couldn't locate HeciPpi
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support send this message through this HECI
  @retval EFI_INVALID_PARAMETER   Parameter invalid
**/
EFI_STATUS
PeiHeciGetRpmcStatus (
  OUT RP_STATUS_3         *RpStatus
  )
{
  EFI_STATUS              Status;
  GET_RPMC_STATUS_BUFFER  RpmcStatus;
  UINT32                  ReqLength;
  UINT32                  RespLength;
  UINT32                  MeMode;

  if (RpStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&RpmcStatus, sizeof (GET_RPMC_STATUS_BUFFER));
  RpmcStatus.Request.MchiHeader.Fields.GroupId = MCHI_MCA_GROUP_ID;
  RpmcStatus.Request.MchiHeader.Fields.Command = McaGetRpmcStatusCmd;
  ReqLength                                    = sizeof (GET_RPMC_STATUS_REQUEST);
  RespLength                                   = sizeof (GET_RPMC_STATUS_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MCHI_MESSAGE_ADDR,
             (UINT32 *) &RpmcStatus,
             ReqLength,
             (UINT32 *) &RpmcStatus,
             &RespLength
             );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// Check Get Rpmc Status message response for errors
  ///
  if ((RpmcStatus.Response.MchiHeader.Fields.Command == McaGetRpmcStatusCmd) &&
      (RpmcStatus.Response.MchiHeader.Fields.IsResponse == 1) &&
      (RpmcStatus.Response.MchiHeader.Fields.Result == McaStatusSuccess)) {
    CopyMem (RpStatus, &RpmcStatus.Response.Status, sizeof (RP_STATUS_3));
  } else {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

//
// ME Client - Integrated Clock Chip (ICC)
//

/**
  Read the ChipsetInit table from CSME

  @param[out]     ChipsetInitTable     Pointer to the required system ChipsetInit Table.
  @param[in, out] ChipsetInitTableLen  Pointer to the length of the table in bytes

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciReadChipsetInitMsg (
  OUT UINT8             *ChipsetInitTable,
  IN OUT UINT32         *ChipsetInitTableLen
  )
{
  EFI_STATUS                  Status;
  ICC_READ_FROM_MPHY_BUFFER   *HsioReadMphy;
  UINT32                      Length;
  UINT32                      RecvLength;

  if ((ChipsetInitTable == NULL) || (ChipsetInitTableLen == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "%a(): Start\n", __FUNCTION__));

  if (!IsMeAvailableForHsio ()) {
    return EFI_UNSUPPORTED;
  }

  Length     = sizeof (ICC_READ_FROM_MPHY_REQUEST);
  RecvLength = sizeof (ICC_READ_FROM_MPHY_RESPONSE) + *ChipsetInitTableLen;

  HsioReadMphy = AllocateZeroPool (RecvLength);
  if (HsioReadMphy == NULL) {
    DEBUG ((DEBUG_ERROR, "(HSIO) %a(): Could not allocate Memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  HsioReadMphy->Request.Header.ApiVersion   = ICC_API_VERSION;
  HsioReadMphy->Request.Header.IccCommand   = ICC_READ_FROM_MPHY_CMD;
  HsioReadMphy->Request.Header.BufferLength = ICC_MESSAGE_BUFFER_LENGTH (Length);
  HsioReadMphy->Request.Offset              = 0;
  HsioReadMphy->Request.NumOfBytes          = *ChipsetInitTableLen;

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_ICC_MESSAGE_ADDR,
             (UINT32 *) HsioReadMphy,
             Length,
             (UINT32 *) HsioReadMphy,
             &RecvLength
             );

  if (!EFI_ERROR (Status)) {
    if (HsioReadMphy->Response.Header.IccResponse == ICC_STATUS_SUCCESS) {
      if (HsioReadMphy->Response.ReadSize > HSIO_CHIPSETINIT_TBL_MAX_SIZE) {
        *ChipsetInitTableLen = 0;
        ASSERT (FALSE);  // ChipsetInit table should not get too large
        Status = EFI_DEVICE_ERROR;
      } else {
        if (*ChipsetInitTableLen < HsioReadMphy->Response.ReadSize) {
          *ChipsetInitTableLen = 0;
          ASSERT (FALSE);  // ChipsetInit buffer size is too small to copy full data
          Status = EFI_BUFFER_TOO_SMALL;
        } else {
          CopyMem (ChipsetInitTable, HsioReadMphy->Response.Payload, HsioReadMphy->Response.ReadSize);
          *ChipsetInitTableLen = HsioReadMphy->Response.ReadSize;
        }
      }
    } else {
      *ChipsetInitTableLen = 0;
      DEBUG ((DEBUG_ERROR, "(HSIO) ERROR: Get HSIO Settings failed!: FW Response=0x%x\n", HsioReadMphy->Response.Header.IccResponse));
      Status = EFI_DEVICE_ERROR;
    }
  }

  FreePool (HsioReadMphy);

  DEBUG ((DEBUG_INFO, "%a(): End\n", __FUNCTION__));

  return Status;
}

/**
  Send the required system ChipsetInit Table to ME FW.

  @param[in] ChipsetInitTable     Pointer to the required system ChipsetInit Table
  @param[in] ChipsetInitTableLen  Length of the table in bytes

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES    HECI Could not allocate Memory
**/
EFI_STATUS
PeiHeciWriteChipsetInitMsg (
  IN  UINT8                       *ChipsetInitTable,
  IN  UINT32                      ChipsetInitTableLen
  )
{
  EFI_STATUS                      Status;
  ICC_SET_MPHY_SETTINGS_BUFFER    *SetMphySettings;
  UINT32                          ReqSize;
  UINT32                          RespSize;

  DEBUG ((DEBUG_INFO, "%a () - Start\n", __FUNCTION__));

  if (ChipsetInitTableLen > HSIO_CHIPSETINIT_TBL_MAX_SIZE) {
    return EFI_DEVICE_ERROR;
  }

  if (!IsMeAvailableForHsio ()) {
    return EFI_UNSUPPORTED;
  }

  ReqSize = sizeof (ICC_SET_MPHY_SETTINGS_BUFFER) + ChipsetInitTableLen;
  SetMphySettings = AllocateZeroPool (ReqSize);
  if (SetMphySettings == NULL) {
    DEBUG ((DEBUG_ERROR, "%a () : Could not allocate Memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  SetMphySettings->Request.Header.ApiVersion   = ICC_API_VERSION;
  SetMphySettings->Request.Header.IccCommand   = ICC_SET_MPHY_SETTINGS_CMD;
  SetMphySettings->Request.Header.BufferLength = ICC_MESSAGE_BUFFER_LENGTH (ReqSize);
  CopyMem (&SetMphySettings->Request.MphyData, ChipsetInitTable, ChipsetInitTableLen);
  RespSize = sizeof (ICC_SET_MPHY_SETTINGS_REQUEST);

  //
  // Send ChipsetInit Table to ME
  //
  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_ICC_MESSAGE_ADDR,
             (UINT32 *) SetMphySettings,
             ReqSize,
             (UINT32 *) SetMphySettings,
             &RespSize
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "(HSIO) ERROR: Write HSIO Settings Message failed! EFI_STATUS = %r\n", Status));
  } else if (SetMphySettings->Response.Header.IccResponse != ICC_STATUS_SUCCESS ||
             SetMphySettings->Response.Header.IccCommand != ICC_SET_MPHY_SETTINGS_CMD) {
    DEBUG ((DEBUG_ERROR, "(HSIO) ERROR: Write HSIO Settings failed!: FW Response = 0x%x\n", SetMphySettings->Response.Header.IccResponse));
    Status = EFI_DEVICE_ERROR;
  }

  FreePool (SetMphySettings);

  DEBUG ((DEBUG_INFO, "%a () - End\n", __FUNCTION__));
  return Status;
}

/**
  Retrieves information on whether Pcie root port configuration SoftStrap override has been done or not
  This command retrieves information from CSME if it performed SoftStrap override in the current boot

  @param[in]      NumberOfControllers    On input, it is the number of controllers caller expects.
  @param[in, out] ControllerConfig       Pointer to the controller config message.

  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_DEVICE_ERROR              HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                   HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL          Message Buffer is too small for the Acknowledge
  @retval EFI_UNSUPPORTED               Current ME mode doesn't support send this message through this HECI
  @retval EFI_OUT_OF_RESOURCES          Unable to allocate required resources
**/
EFI_STATUS
PeiHeciGetSoftStrpConfigMsg (
  IN      UINT16     NumberOfControllers,
  IN OUT  UINT8      *ControllerConfig
  )
{
  EFI_STATUS                             Status;
  UINT32                                 CommandSize;
  UINT32                                 ResponseSize;
  ICC_GET_SOFT_STRAP_CONFIG_CMD_BUFFER   *GetSoftStrapBuffer;

  DEBUG ((DEBUG_INFO, "(ICC) %a(): Start\n", __FUNCTION__));

  //(4 * sizeof (UINT32)) is needed for reserved field
  ResponseSize         = NumberOfControllers * sizeof (UINT8) + sizeof (ICC_GET_SOFT_STRAP_CONFIG_CMD_RESPONSE) + (4 * sizeof (UINT32));
  GetSoftStrapBuffer   = AllocateZeroPool (ResponseSize);
  if (GetSoftStrapBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CommandSize                                     = sizeof (ICC_GET_SOFT_STRAP_CONFIG_CMD_REQUEST);
  GetSoftStrapBuffer->Request.Header.ApiVersion   = ICC_API_VERSION;
  GetSoftStrapBuffer->Request.Header.IccCommand   = ICC_GET_SOFT_STRAP_CONFIG_CMD;
  GetSoftStrapBuffer->Request.Header.BufferLength = ICC_MESSAGE_BUFFER_LENGTH (CommandSize);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_ICC_MESSAGE_ADDR,
             (UINT32 *) GetSoftStrapBuffer,
             CommandSize,
             (UINT32 *) GetSoftStrapBuffer,
             &ResponseSize
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Message failed! Status = %r\n", Status));
  } else if (GetSoftStrapBuffer->Response.Header.IccResponse != ICC_STATUS_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Wrong response! IccHeader.Response = 0x%x\n", GetSoftStrapBuffer->Response.Header.IccResponse));
    Status = EFI_DEVICE_ERROR;
  } else if (GetSoftStrapBuffer->Response.NumOfControllers != NumberOfControllers) {
    DEBUG ((DEBUG_ERROR, "Controller number does not match! Number Of Controllers = 0x%x\n", GetSoftStrapBuffer->Response.NumOfControllers));
    Status = EFI_DEVICE_ERROR;
  } else {
    CopyMem (ControllerConfig, GetSoftStrapBuffer->Response.ControllerConfig, GetSoftStrapBuffer->Response.NumOfControllers);
  }

  FreePool (GetSoftStrapBuffer);
  DEBUG ((DEBUG_INFO, "(ICC) %a(): End, Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Set Pcie Root port configuration SoftStrap override for the specified Pcie Root Port controller

  @param[in] NumberOfControllers         On input, it is the number of controllers caller expects.
  @param[in] OverrideMask                Mask with RpController where Hybrid Storage is present and softstraps need to be overridden

  @retval EFI_SUCCESS                    Command succeeded
  @retval EFI_DEVICE_ERROR               HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                    HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL           Message Buffer is too small for the Acknowledge
  @retval EFI_UNSUPPORTED                Current ME mode doesn't support send this message through this HECI
  @retval EFI_OUT_OF_RESOURCES           Unable to allocate required resources
**/
EFI_STATUS
PeiHeciOverrideSoftStrapMsg (
  IN UINT16                                NumberOfControllers,
  IN UINT8                                 OverrideMask
  )
{
  EFI_STATUS                                  Status;
  UINT32                                      CommandSize;
  UINT32                                      ResponseSize;
  ICC_OVERRIDE_PCIE_SOFT_STRAP_CMD_BUFFER     *OverrideSoftStrapBuffer;
  UINT8                                       *ConfigBuffer;
  UINT16                                      Index;

  DEBUG ((DEBUG_INFO, "(ICC) %a(): Start\n", __FUNCTION__));

  // (4 * sizeof (UINT32)) is needed for reserved field
  CommandSize = NumberOfControllers * sizeof (UINT8) + sizeof (ICC_OVERRIDE_SOFT_STRAP_CMD_REQUEST) + (4 * sizeof (UINT32));
  OverrideSoftStrapBuffer = AllocateZeroPool (CommandSize);
  if (OverrideSoftStrapBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ResponseSize                                         = sizeof (ICC_OVERRIDE_SOFT_STRAP_CMD_RESPONSE);
  OverrideSoftStrapBuffer->Request.Header.ApiVersion   = ICC_API_VERSION;
  OverrideSoftStrapBuffer->Request.Header.IccCommand   = ICC_OVERRIDE_SOFT_STRAP_CMD;
  OverrideSoftStrapBuffer->Request.Header.BufferLength = ICC_MESSAGE_BUFFER_LENGTH (CommandSize);
  OverrideSoftStrapBuffer->Request.NumOfControllers    = NumberOfControllers;
  ConfigBuffer                                         = OverrideSoftStrapBuffer->Request.ControllerConfig;
  for (Index = 0; Index < NumberOfControllers; Index++) {
    if ((OverrideMask & (BIT0 << Index)) != 0) {
      ConfigBuffer[Index] = PcieOverride2x2;
    } else {
      ConfigBuffer[Index] = PcieOverrideNone;
    }
  }

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_ICC_MESSAGE_ADDR,
             (UINT32 *) OverrideSoftStrapBuffer,
             CommandSize,
             (UINT32 *) OverrideSoftStrapBuffer,
             &ResponseSize
             );
  if (EFI_ERROR (Status) || (OverrideSoftStrapBuffer->Response.Header.IccResponse != ICC_STATUS_SUCCESS)) {
    Status = EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "(ICC) %a(): End, Status = %r\n", __FUNCTION__, Status));
  FreePool (OverrideSoftStrapBuffer);
  return Status;
}
