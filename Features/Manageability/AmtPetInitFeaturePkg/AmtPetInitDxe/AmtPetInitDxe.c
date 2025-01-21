/** @file
  AMT PET Initialization feature driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeAsfLib.h>
#include <Library/MeInfoLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/PciSegmentLib.h>
#include <Protocol/ReportStatusCodeHandler.h>
#include <Protocol/HeciControlProtocol.h>
#include <Register/HeciRegs.h>
#include <AmtConfig.h>
#include <AmtForcePushPetHob.h>
#include <AmtPetInitConfigVariable.h>
#include <AmtPetQueueHob.h>
#include <MeBiosPayloadHob.h>
#include <MeState.h>
#include "AmtPetInitDxe.h"

GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN     mProgressEventEnabled = FALSE;

/**
  Is AMT PET Initialization feature driver supported.

  @retval TRUE   AMT PET Initialization feature driver is supported.
  @retval FALSE  AMT PET Initialization feature driver is not supported.
**/
BOOLEAN
IsAmtPetInitSupported (
  VOID
  )
{
  ME_BIOS_PAYLOAD_HOB  *MbpHob;

  if (PciSegmentRead32 (MeHeciPciCfgBase (HECI1)) == 0xFFFFFFFF) {
    return FALSE;
  }

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: No MBP HOB Data Protocol available\n", __FUNCTION__));
    return FALSE;
  }

  if (MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType != IntelMeCorporateFw) {
    return FALSE;
  }

  return TRUE;
}

/**
  Send ASF Message through HECI.

  @param[in] AsfMessage           Pointer to ASF message

  @retval EFI_SUCCESS             Boot options copied
  @retval EFI_INVALID_PARAMETER   Invalid pointer
  @retval EFI_NOT_READY           No controller
**/
EFI_STATUS
SendAsfMessage (
  IN  ASF_MESSAGE                          *AsfMessage
  )
{
  EFI_STATUS                  Status;
  HECI_ASF_PUSH_PROGRESS_CODE HeciAsfPushProgressCode;
  UINT32                      MeStatus;
  UINT32                      MeMode;

  Status = MeBiosGetMeStatus (&MeStatus);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// Get ME Operation Mode
  ///
  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// Only send ASF Push Progress code when ME is ready and ME is in normal mode.  Ignore FW Init Status.
  ///
  if (ME_STATUS_ME_STATE_ONLY (MeStatus) == ME_READY && MeMode == ME_MODE_NORMAL) {
    ZeroMem ((VOID *) &HeciAsfPushProgressCode, sizeof (HECI_ASF_PUSH_PROGRESS_CODE));
    HeciAsfPushProgressCode.Command   = ASF_MESSAGE_COMMAND_MESSAGE;
    HeciAsfPushProgressCode.ByteCount = ASF_MESSAGE_BYTE_COUNT_MAP (HECI_ASF_PUSH_PROGRESS_CODE);
    CopyMem ((VOID *) &(HeciAsfPushProgressCode.AsfMessage), (VOID *) AsfMessage, sizeof (ASF_MESSAGE));

    Status = HeciWrapperSend (
               BIOS_ASF_HOST_ADDR,
               HECI_ASF_MESSAGE_ADDR,
               (UINT32 *) &HeciAsfPushProgressCode,
               sizeof (HECI_ASF_PUSH_PROGRESS_CODE)
               );
  }

  return Status;
}

/**
  This routine checks whether current message is ForcePush message.

  @param[in] MessageType          AMT PET Message Type.

  @retval TRUE                    It is ForcePush message.
  @retval FALSE                   It is not ForcePush message.
**/
BOOLEAN
IsForcePushErrorEvent (
  IN  ASF_FRAMEWORK_MESSAGE_TYPE  MessageType
  )
{
  UINTN                       Index;
  UINTN                       Number;
  VOID                        *GuidHob;
  ASF_FRAMEWORK_MESSAGE_TYPE  *RecordPtr;

  GuidHob = GetFirstGuidHob (&gAmtForcePushPetPolicyGuid);
  if (GuidHob == NULL) {
    return FALSE;
  }
  Number    = GET_GUID_HOB_DATA_SIZE (GuidHob) / sizeof (ASF_FRAMEWORK_MESSAGE_TYPE);
  RecordPtr = (ASF_FRAMEWORK_MESSAGE_TYPE *) GET_GUID_HOB_DATA (GuidHob);
  for (Index = 0; (Index < Number) && (*RecordPtr != AsfMessageTermination); Index++, RecordPtr++) {
    if (*RecordPtr == MessageType) {
      return TRUE;
    }
  }

  return FALSE;
}



/**
  Sends a POST packet across ASF

  @param[in] MessageType          POST Status Code

  @retval EFI_DEVICE_ERROR        No message found
  @retval EFI_SUCCESS             Boot options copied
  @retval EFI_INVALID_PARAMETER   Invalid pointer
  @retval EFI_NOT_READY           No controller
**/
EFI_STATUS
SendPostPacket (
  IN  ASF_FRAMEWORK_MESSAGE_TYPE      MessageType
  )
{
  EFI_STATUS  Status;
  UINTN       Index;

  Status = EFI_DEVICE_ERROR;
  ///
  /// Find the message to send across the wire
  ///
  for (Index = 0; Index < ARRAY_SIZE (mAsfFrameworkMessage); Index++) {
    if (mAsfFrameworkMessage[Index].MessageType == MessageType) {
      Status = SendAsfMessage (&mAsfFrameworkMessage[Index].Message);
      break;
    }
  }

  return Status;
}

/**
  This routine saves current ForcePush ErrorEvent to Variable, which will be sent again.

  @param[in] MessageType          ASF PET message type.

**/
VOID
SaveForcePushErrorEvent (
  IN  ASF_FRAMEWORK_MESSAGE_TYPE  MessageType
  )
{
  EFI_STATUS                      Status;
  UINTN                           Size;
  ASF_FRAMEWORK_MESSAGE_TYPE      *Message;
  UINT32                          VarAttributes;

  ///
  /// Create PET queue variable
  ///
  Message = NULL;
  Size    = 0;
  Status = gRT->GetVariable (
                  AMT_FORCE_PUSH_PET_VARIABLE_NAME,
                  &gAmtForcePushPetVariableGuid,
                  &VarAttributes,
                  &Size,
                  NULL
                  );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    ///
    /// Get the exist message
    ///
    Message = AllocateZeroPool (Size + sizeof (MessageType));
    ASSERT (Message != NULL);
    if (Message == NULL) {
      return;
    }

    Status = gRT->GetVariable (
                    AMT_FORCE_PUSH_PET_VARIABLE_NAME,
                    &gAmtForcePushPetVariableGuid,
                    &VarAttributes,
                    &Size,
                    Message
                    );
    if (EFI_ERROR (Status)) {
      FreePool (Message);
      return;
    }

    ///
    /// Fill new item
    ///
    *(ASF_FRAMEWORK_MESSAGE_TYPE *) ((UINTN) Message + Size) = MessageType;
    Size += sizeof (MessageType);
  } else if (Status == EFI_NOT_FOUND) {
    ///
    /// Create a new one
    ///
    Size    = sizeof (MessageType);
    Message = AllocateZeroPool (sizeof (MessageType));
    ASSERT (Message != NULL);
    if (Message == NULL) {
      return;
    }
    VarAttributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;
    *Message = MessageType;
  } else {
    ASSERT (FALSE);
  }

  ///
  /// Set PET message to variable
  ///
  if (Message != NULL) {
    Status = gRT->SetVariable (
                    AMT_FORCE_PUSH_PET_VARIABLE_NAME,
                    &gAmtForcePushPetVariableGuid,
                    VarAttributes,
                    Size,
                    Message
                    );
    ASSERT_EFI_ERROR (Status);
    FreePool (Message);
  }

  return;
}

/**
  This routine converts Hob ForcePush ErrorEvent to Variable, which will be sent again.

  @retval EFI_SUCCESS             The function completed successfully
  @retval Others                  Fail to get Asf ForcePush PET Hob
**/
EFI_STATUS
SaveForcePushErrorEventFromPeiToDxe (
  VOID
  )
{
  EFI_STATUS              Status;
  AMT_FORCE_PUSH_PET_HOB  *AmtForcePushPetHob;
  EFI_PEI_HOB_POINTERS    Hob;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  ///
  /// Find ASF ForcePush PET Hob
  ///
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, (VOID **) &AmtForcePushPetHob);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Get ASF ForcePush PET Hob Failed\n"));
    return Status;
  }

  while (TRUE) {
    AmtForcePushPetHob = GetNextGuidHob (&gAmtForcePushPetHobGuid, AmtForcePushPetHob);
    if (AmtForcePushPetHob == NULL) {
      break;
    }

    SaveForcePushErrorEvent (AmtForcePushPetHob->MessageType);

    ///
    /// Need find next one
    ///
    Hob.Raw             = (VOID *) AmtForcePushPetHob;
    AmtForcePushPetHob  = (AMT_FORCE_PUSH_PET_HOB *) GET_NEXT_HOB (Hob);
  }

  return EFI_SUCCESS;
}


/**
  Provides an interface that a software module can call to report an ASF DXE status code.

  @param[in] Type                 Indicates the type of status code being reported.
  @param[in] Value                Describes the current status of a hardware or software entity.
                                  This included information about the class and subclass that is
                                  used to classify the entity as well as an operation.
  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_DEVICE_ERROR        The function should not be completed due to a device error.
**/
EFI_STATUS
AmtReportStatusCode (
  IN  EFI_STATUS_CODE_TYPE        Type,
  IN  EFI_STATUS_CODE_VALUE       Value
  )
{
  UINTN       Index;
  EFI_STATUS  Status;

  if (mProgressEventEnabled) {
    if ((Type & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) {
      for (Index = 0; Index < ARRAY_SIZE (mAsfProgressDataHubMap); Index++) {
        if (mAsfProgressDataHubMap[Index].StatusCodeValue == Value) {
          return SendPostPacket (mAsfProgressDataHubMap[Index].MessageType);
        }
      }
    }
  }

  if ((Type & EFI_STATUS_CODE_TYPE_MASK) == EFI_ERROR_CODE) {
    for (Index = 0; Index < ARRAY_SIZE (mAsfErrorDataHubMap); Index++) {
      if (mAsfErrorDataHubMap[Index].StatusCodeValue == Value) {
        Status = SendPostPacket (mAsfErrorDataHubMap[Index].MessageType);
        if ((Status == EFI_DEVICE_ERROR) && IsForcePushErrorEvent (mAsfErrorDataHubMap[Index].MessageType)) {
          SaveForcePushErrorEvent (mAsfErrorDataHubMap[Index].MessageType);
        }

        return Status;
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  This routine sends PET message in MessageQueue.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_NOT_READY           No controller
**/
EFI_STATUS
SendPetMessageInQueue (
  VOID
  )
{
  EFI_STATUS            Status;
  AMT_PET_QUEUE_HOB     *PetQueueHob;
  EFI_PEI_HOB_POINTERS  Hob;
  UINT32                MeStatus;

  Status = MeBiosGetMeStatus (&MeStatus);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// Only send ASF Push Progress code when ME is ready. Ignore FW Init Status.
  ///
  if (ME_STATUS_ME_STATE_ONLY (MeStatus) != ME_READY) {
    return EFI_NOT_READY;
  }

  ///
  /// Get PetQueueHob
  ///
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, (VOID **) &PetQueueHob);
  while (TRUE) {
    PetQueueHob = GetNextGuidHob (&gAmtPetQueueHobGuid, PetQueueHob);
    if (PetQueueHob == NULL) {
      break;
    }
    ///
    /// Send message
    ///
    AmtReportStatusCode (PetQueueHob->Type, PetQueueHob->Value);

    ///
    /// Mark it as sent
    ///
    PetQueueHob->Type = (UINT32) -1;

    ///
    /// Need find next one
    ///
    Hob.Raw     = (VOID *) PetQueueHob;
    PetQueueHob = (AMT_PET_QUEUE_HOB *) GET_NEXT_HOB (Hob);
  }

  return EFI_SUCCESS;
}

/**
  Filters all the progress and error codes for Asf.

  @param[in]  CodeType         Indicates the type of status code being reported.
  @param[in]  Value            Describes the current status of a hardware or software entity.
                               This included information about the class and subclass that is used to
                               classify the entity as well as an operation.
  @param[in]  Instance         The enumeration of a hardware or software entity within
                               the system. Valid instance numbers start with 1.
  @param[in]  CallerId         This optional parameter may be used to identify the caller.
                               This parameter allows the status code driver to apply different rules to
                               different callers.
  @param[in]  Data             This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS          Reported all the progress and error codes for Asf successfully.
  @retval EFI_DEVICE_ERROR     Sends a POST packet across Asf fail.

**/
EFI_STATUS
EFIAPI
PetStatusCodeReportListener (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId,
  IN EFI_STATUS_CODE_DATA     *Data OPTIONAL
  )
{
  return AmtReportStatusCode (CodeType, Value);
}

/**
  Unregister status code callback functions only available at boot time from
  report status code router when exiting boot services.

  @param[in]  Event         Event whose notification function is being invoked.
  @param[in]  Context       Pointer to the notification function's context, which is
                            always zero in current implementation.

**/
VOID
EFIAPI
PetExitBootServicesEventNotify (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS                      Status;
  EFI_RSC_HANDLER_PROTOCOL        *RscHandlerProtocol;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  Status = gBS->CloseEvent (Event);

  ///
  /// Get ReportStatusCodeHandler Protocol
  ///
  Status = gBS->LocateProtocol (
                  &gEfiRscHandlerProtocolGuid,
                  NULL,
                  (VOID **) &RscHandlerProtocol
                  );
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    Status = RscHandlerProtocol->Unregister (PetStatusCodeReportListener);
    DEBUG ((DEBUG_INFO, "Unregister PetStatusCodeReportListener Status = %r\n", Status));
  }

  return;
}

/**
  Is AMT PET Firmware Progress enabled.

  @retval TRUE   AMT PET Firmware Progress is enabled.
  @retval FALSE  AMT PET Firmware Progress is disabled.
**/
BOOLEAN
PetIsFwProgressEnabled (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINTN                           Size;
  UINT32                          VarAttributes;
  AMT_PET_INIT_CONFIG             AmtPetInitConfig;
  EFI_HOB_GUID_TYPE               *GuidHob;
  AMT_PEI_CONFIG                  *AmtPeiConfig;
  ME_BIOS_PAYLOAD_HOB             *MbpHob;

  //
  // Get Setup Variable
  //
  Size = sizeof (AMT_PET_INIT_CONFIG);
  Status = gRT->GetVariable (
                  AMT_PET_INIT_CONFIG_NAME,
                  &gAmtPetInitConfigGuid,
                  &VarAttributes,
                  &Size,
                  (VOID *) &AmtPetInitConfig
                  );
  if (EFI_ERROR (Status)) {
    ZeroMem (&AmtPetInitConfig, sizeof (AMT_PET_INIT_CONFIG));
    VarAttributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;
    AmtPetInitConfig.FwProgress = 1;
    Status = gRT->SetVariable (
                    AMT_PET_INIT_CONFIG_NAME,
                    &gAmtPetInitConfigGuid,
                    VarAttributes,
                    Size,
                    (VOID *) &AmtPetInitConfig
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: AmtPetInitConfig failed.\n", __FUNCTION__));
      return FALSE;
    }
  }

  //
  // Get AMT Policy HOB Data
  //
  GuidHob = GetFirstGuidHob (&gAmtPolicyHobGuid);
  if (GuidHob != NULL) {
    AmtPeiConfig = (AMT_PEI_CONFIG *) GET_GUID_HOB_DATA (GuidHob);
    if (AmtPeiConfig->AmtEnabled) {
      //
      // Get MBP HOB Data
      //
      MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
      if (MbpHob != NULL) {
        if (AmtPetInitConfig.FwProgress &&
            MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.Amt == 1 &&
            MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.Amt == 1) {
          return TRUE;
        }
      }
    }
  }

  return FALSE;
}

/**
  PET Message and Progress Code initialization.

  @retval EFI_SUCCESS    The PET Message and Progress Code initialized successfully.
  @retval Others         Some errors occur when initializing the PET Message and Progress Code.
**/
EFI_STATUS
PetMessageInit (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_RSC_HANDLER_PROTOCOL        *RscHandlerProtocol;
  EFI_EVENT                       PetExitBootServicesEvent;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  mProgressEventEnabled = AsfIsFwProgressEnabled ();

  ///
  /// If no boot options available, check policy
  ///
  if (!mProgressEventEnabled) {
    mProgressEventEnabled = PetIsFwProgressEnabled ();
  }
  DEBUG ((DEBUG_INFO, "mProgressEventEnabled 0x%x\n", mProgressEventEnabled));

  ///
  /// Save PEI force push error event from hob to variable
  ///
  SaveForcePushErrorEventFromPeiToDxe ();

  ///
  /// Try to send message
  ///
  SendPetMessageInQueue ();

  ///
  /// Get ReportStatusCodeHandler Protocol
  ///
  Status = gBS->LocateProtocol (
                  &gEfiRscHandlerProtocolGuid,
                  NULL,
                  (VOID **) &RscHandlerProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Locate gEfiRscHandlerProtocolGuid Protocol status is %r\n", Status));
    return Status;
  }

  ///
  /// Register our Status Code Report Function.
  /// This function is notified at the highest TPL
  ///
  Status = RscHandlerProtocol->Register (PetStatusCodeReportListener, TPL_HIGH_LEVEL);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "Register PetStatusCodeReportListener status is %r\n", Status));

  ///
  /// Create the notify function to unregister AsfStatusCodeReportListener on ExitBootServices Event.
  ///
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  PetExitBootServicesEventNotify,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &PetExitBootServicesEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Entry point for AMT PET initialization driver.

  @param[in] ImageHandle   Pointer to the loaded image protocol for this driver
  @param[in] SystemTable   Pointer to the EFI System Table

  @retval EFI_SUCCESS      PET initialization driver initialized successfully.
  @retval Others           PET initialization driver unsupported
**/
EFI_STATUS
EFIAPI
AmtPetInitDxeEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS           Status;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  if (!IsAmtPetInitSupported ()) {
    return EFI_UNSUPPORTED;
  }

  ///
  /// PET message and progress code initialization
  ///
  Status = PetMessageInit ();
  ASSERT_EFI_ERROR (Status);

  return Status;
}