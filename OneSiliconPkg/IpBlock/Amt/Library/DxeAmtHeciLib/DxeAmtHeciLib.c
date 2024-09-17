/** @file
  This is a library for Amt Heci Message functionality.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/TimerLib.h>
#include <Library/DxeMeLib.h>
#include <Library/RngLib.h>
#include <Library/MeUtilsLib.h>
#include <MeState.h>
#include <AsfMsgs.h>
#include <AmthiMsgs.h>
#include <AmtConfig.h>
#include <WifiProfileSyncAsfMsgs.h>

//
// ME Client - AMT/ASF
//

//
// Management Control Command
//

/**
  Start ASF Watchdog Timer.
  The WDT will be started only if AMT Watchdog policy is enabled and corresponding timer value is not zero.

  @param[in] WatchDogType         ASF Watchdog Type: 0 - BIOS Watchdog
                                                     1 - OS Watchdog
**/
VOID
AsfStartWatchDog (
  IN  UINT8                       WatchDogType
  )
{
  EFI_STATUS        Status;
  ASF_START_WDT     AsfStartWdt;
  UINT32            MeStatus;
  UINT32            MeMode;
  UINT16            WaitTimer;
  EFI_HOB_GUID_TYPE *GuidHob;
  AMT_PEI_CONFIG    *AmtPeiConfig;

  GuidHob = GetFirstGuidHob (&gAmtPolicyHobGuid);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_INFO, "Get AMT Watchdog policy fail, don't start ASF WDT\n"));
    return;
  }
  AmtPeiConfig = (AMT_PEI_CONFIG *) GET_GUID_HOB_DATA (GuidHob);

  if (!AmtPeiConfig->WatchDogEnabled) {
    DEBUG ((DEBUG_INFO, "AMT Watchdog policy is not enabled, don't start ASF WDT\n"));
    return;
  }

  if (WatchDogType == ASF_START_BIOS_WDT) {
    WaitTimer = AmtPeiConfig->WatchDogTimerBios;
  } else {
    WaitTimer = AmtPeiConfig->WatchDogTimerOs;
  }
  if (WaitTimer == 0) {
    DEBUG ((DEBUG_INFO, "Timeout value is 0, unable to start ASF WDT\n"));
    return;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    DEBUG ((DEBUG_ERROR, "MeMode is %x, unable to start ASF WDT\n", MeMode));
    return;
  }

  ///
  /// Send WDT message when ME is ready.  Do not care about if ME FW INIT is completed.
  ///
  Status = MeBiosGetMeStatus (&MeStatus);
  if (EFI_ERROR (Status) || (ME_STATUS_ME_STATE_ONLY (MeStatus) != ME_READY)) {
    DEBUG ((DEBUG_ERROR, "MeStatus is %x, unable to start ASF WDT\n", MeStatus));
    return;
  }

  DEBUG ((DEBUG_INFO, "%a () - Starting ASF WDT with timeout %d seconds \n", __FUNCTION__, WaitTimer));

  AsfStartWdt.AsfHeader.Fields.Command           = ASF_MESSAGE_COMMAND_MANAGEMENT_CONTROL;
  AsfStartWdt.AsfHeader.Fields.ByteCount         = ASF_MESSAGE_BYTE_COUNT_MAP (ASF_START_WDT);
  AsfStartWdt.AsfHeader.Fields.SubCommand        = ASF_MESSAGE_SUBCOMMAND_START_WATCH_DOG_TIMER;
  AsfStartWdt.AsfHeader.Fields.VersionNumber     = ASF_VERSION;
  AsfStartWdt.EventSensorType                    = ASF_EVENT_SENSOR_TYPE_WATCHDOG2;
  AsfStartWdt.EventType                          = ASF_EVENT_TYPE_SENSOR_SPECIFIC;
  AsfStartWdt.EventOffset                        = ASF_EVENT_OFFSET_TIMER_EXPIRED;
  AsfStartWdt.EventSeverity                      = ASF_EVENT_SEVERITY_CODE_CRITICAL;
  AsfStartWdt.SensorDevice                       = ASF_SENSOR_DEVICE;
  AsfStartWdt.SensorNumber                       = ASF_SENSOR_NUMBER;
  AsfStartWdt.Entity                             = ASF_ENTITY_UNSPECIFIED;
  AsfStartWdt.EntityInstance                     = ASF_ENTITY_INSTANCE_UNSPECIFIED;
  AsfStartWdt.EventData[0]                       = ASF_WD_EVENT_DATA1;
  if (WatchDogType == ASF_START_BIOS_WDT) {
    AsfStartWdt.EventSourceType                  = ASF_EVENT_SOURCE_TYPE_PLATFORM_FIRMWARE;
    AsfStartWdt.EventData[1]                     = ASF_WD_EVENT_DATA2_BIOS_TIMEOUT;
  } else {
    AsfStartWdt.EventSourceType                  = ASF_EVENT_SOURCE_TYPE_OS;
    AsfStartWdt.EventData[1]                     = ASF_WD_EVENT_DATA2_OS_TIMEOUT;
  }

  AsfStartWdt.TimeoutLow                         = (UINT8) WaitTimer;
  AsfStartWdt.TimeoutHigh                        = (UINT8) (WaitTimer >> 8);

  Status = HeciWrapperSend (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &AsfStartWdt,
             sizeof (ASF_START_WDT)
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to start ASF WDT, Status = %r\n", Status));
  }

}

/**
  Stop ASF Watchdog Timer HECI message.
**/
VOID
AsfStopWatchDog (
  VOID
  )
{
  EFI_STATUS           Status;
  ASF_STOP_WDT         AsfStopWdt;
  UINT32               MeStatus;
  UINT32               MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    DEBUG ((DEBUG_ERROR, "MeMode is %x, unable to stop ASF WDT\n", MeMode));
    return;
  }

  ///
  /// Send WDT message when ME is ready. Do not care about if ME FW INIT is completed.
  ///
  Status = MeBiosGetMeStatus (&MeStatus);
  if (EFI_ERROR (Status) || (ME_STATUS_ME_STATE_ONLY (MeStatus) != ME_READY)) {
    DEBUG ((DEBUG_ERROR, "MeStatus is %x, unable to stop ASF WDT\n", MeStatus));
    return;
  }

  DEBUG ((DEBUG_INFO, "%a () - Stopping ASF WDT...\n", __FUNCTION__));
  AsfStopWdt.AsfHeader.Fields.Command        = ASF_MESSAGE_COMMAND_MANAGEMENT_CONTROL;
  AsfStopWdt.AsfHeader.Fields.ByteCount      = ASF_MESSAGE_BYTE_COUNT_MAP (ASF_STOP_WDT);
  AsfStopWdt.AsfHeader.Fields.SubCommand     = ASF_MESSAGE_SUBCOMMAND_STOP_WATCH_DOG_TIMER;
  AsfStopWdt.AsfHeader.Fields.VersionNumber  = ASF_VERSION;

  Status = HeciWrapperSend (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &AsfStopWdt,
             sizeof (ASF_STOP_WDT)
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to stop ASF WDT, Status = %r\n", Status));
  }
}

//
// Messaging Command
//

/**
  Send Remote Platform Erase operation status using PET

  @param[in]    RpeOperationCode    PET Alert Code for the RPE Operation.
  @param[in]    RpeDevice           Device for which the RPE OperationCode is reported.
  @param[in]    RpeOperationStatus  Reports if RPE Operation is in progress or failure.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
SendRpePetAlert (
  IN UINT8                        RpeOperationCode,
  IN UINT8                        RpeDevice,
  IN UINT8                        RpeOperationStatus
  )
{
  EFI_STATUS                           Status;
  PET_ALERT                            RpePetAlert;
  UINT32                               MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  RpePetAlert.AsfHeader.Fields.Command         = ASF_MESSAGE_COMMAND_MESSAGE;
  RpePetAlert.AsfHeader.Fields.ByteCount       = ASF_MESSAGE_BYTE_COUNT_MAP (PET_ALERT);
  RpePetAlert.AsfHeader.Fields.SubCommand      = ASF_MESSAGE_SUBCOMMAND_NORETRANSMIT;
  RpePetAlert.AsfHeader.Fields.VersionNumber   = ASF_VERSION;
  RpePetAlert.EventSensorType                  = ASF_EVENT_SENSOR_TYPE_SYS_FW_ERR_PROG;
  RpePetAlert.EventType                        = ASF_EVENT_TYPE_SENSOR_SPECIFIC;
  RpePetAlert.EventSourceType                  = ASF_EVENT_SOURCE_TYPE_ASF10;
  RpePetAlert.EventSeverity                    = ASF_EVENT_SEVERITY_CODE_NONCRITICAL;
  RpePetAlert.SensorDevice                     = ASF_SENSOR_DEVICE;
  RpePetAlert.Sensornumber                     = ASF_SENSOR_NUMBER;
  RpePetAlert.Entity                           = ASF_ENTITY_BIOS;
  RpePetAlert.EntityInstance                   = 0xFF;
  RpePetAlert.EventData1                       = ASF_RPE_EVENT_DATA0_EVENT_DATA_SET_BY_OEM;
  RpePetAlert.EventData2                       = ASF_RPE_EVENT_DATA2_REMOTE_PLATFORM_ERASE;
  RpePetAlert.EventData5                       = 0x00;

  //
  // Update EventData values for sending RPE Alert to ASF.
  //
  RpePetAlert.EventData3                       = RpeOperationCode;
  RpePetAlert.EventData4                       = RpeDevice;
  RpePetAlert.EventOffset                      = RpeOperationStatus;

  Status = HeciWrapperSend (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &RpePetAlert,
             sizeof (PET_ALERT)
             );

  return Status;
}

//
// LAN Command
//

/**
  This message is sent to switch active LAN interface.

  @param[in] ActiveInterface      Active Lan Interface
                                    0: As defined in FIT
                                    1: Integrated LAN
                                    2: Discrete LAN

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
AmtSetActiveLanInterface (
  IN UINT8                        ActiveInterface
  )
{
  EFI_STATUS                          Status;
  AMT_SET_ACTIVE_LAN_INTERFACE_BUFFER SetActiveLanInterface;
  UINT32                              MeMode;
  UINT32                              RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  SetActiveLanInterface.Request.AsfHeader.Fields.Command       = ASF_MESSAGE_COMMAND_LAN;
  SetActiveLanInterface.Request.AsfHeader.Fields.ByteCount     = ASF_MESSAGE_BYTE_COUNT_MAP (AMT_SET_ACTIVE_LAN_INTERFACE);
  SetActiveLanInterface.Request.AsfHeader.Fields.SubCommand    = ASF_MESSAGE_SUBCOMMAND_SET_ACTIVE_LAN;
  SetActiveLanInterface.Request.AsfHeader.Fields.VersionNumber = ASF_VERSION;
  SetActiveLanInterface.Request.ActiveInterface                = ActiveInterface;
  RecvLength                                                   = sizeof (AMT_SET_ACTIVE_LAN_INTERFACE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &SetActiveLanInterface.Request,
             (UINT32) sizeof (AMT_SET_ACTIVE_LAN_INTERFACE),
             (UINT32 *) &SetActiveLanInterface.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Set Active Lan Interface failed %r\n", Status));
    return Status;
  }

  if (SetActiveLanInterface.Response.Status != AmtStatusSuccess) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  This message is sent to get active LAN interface.

  @param[out] ActiveInterface     Active Lan Interface
                                    0: As defined in FIT
                                    1: Integrated LAN
                                    2: Discrete LAN

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
AmtGetActiveLanInterface (
  OUT UINT8                          *ActiveInterface
  )
{
  EFI_STATUS                          Status;
  AMT_GET_ACTIVE_LAN_INTERFACE_BUFFER GetActiveLanInterface;
  UINT32                              MeMode;
  UINT32                              RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  GetActiveLanInterface.Request.AsfHeader.Fields.Command       = ASF_MESSAGE_COMMAND_LAN;
  GetActiveLanInterface.Request.AsfHeader.Fields.ByteCount     = ASF_MESSAGE_BYTE_COUNT_MAP (AMT_GET_ACTIVE_LAN_INTERFACE);
  GetActiveLanInterface.Request.AsfHeader.Fields.SubCommand    = ASF_MESSAGE_SUBCOMMAND_GET_ACTIVE_LAN;
  GetActiveLanInterface.Request.AsfHeader.Fields.VersionNumber = ASF_VERSION;
  RecvLength                                                   = sizeof (AMT_GET_ACTIVE_LAN_INTERFACE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &GetActiveLanInterface.Request,
             (UINT32) sizeof (AMT_GET_ACTIVE_LAN_INTERFACE),
             (UINT32 *) &GetActiveLanInterface.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get Active Lan Interface failed %r\n", Status));
    return Status;
  }

  if (GetActiveLanInterface.Response.Status != AmtStatusSuccess) {
    return EFI_DEVICE_ERROR;
  }

  *ActiveInterface = GetActiveLanInterface.Response.ActiveInterface;
  return EFI_SUCCESS;
}

//
// KVM Command
//

/**
  This is used to send KVM request message to Intel ME. When
  Bootoptions indicate that a KVM session is requested then BIOS
  will send this message before any graphical display output to
  ensure that FW is ready for KVM session.

  @param[in] QueryType            0 - Query Request
                                  1 - Cancel Request
  @param[out] ResponseCode        1h - Continue, KVM session established.
                                  2h - Continue, KVM session cancelled.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmtQueryKvm (
  IN  UINT32                      QueryType,
  OUT UINT32                      *ResponseCode
  )
{
  EFI_STATUS                      Status;
  AMT_QUERY_KVM_BUFFER            QueryKvm;
  UINT32                          RecvLength;
  UINT16                          TimeOut;
  UINT32                          MeMode;
  UINT8                           HostAddress;
  UINT8                           EngineAddress;

  TimeOut       = 0;
  HostAddress   = 0;
  EngineAddress = 0;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  QueryKvm.Request.AsfHeader.Data                 = 0;
  QueryKvm.Request.AsfHeader.Fields.Command       = ASF_MESSAGE_COMMAND_KVM;
  QueryKvm.Request.AsfHeader.Fields.ByteCount     = ASF_MESSAGE_BYTE_COUNT_MAP (AMT_QUERY_KVM);
  QueryKvm.Request.AsfHeader.Fields.SubCommand    = ASF_MESSAGE_SUBCOMMAND_KVM_QUERY;
  QueryKvm.Request.AsfHeader.Fields.VersionNumber = ASF_VERSION;
  QueryKvm.Request.QueryType                      = QueryType;
  RecvLength                                      = sizeof (AMT_QUERY_KVM_RESPONSE);

  if (QueryType == QueryRequest) {
    Status = HeciWrapperSend (
               BIOS_ASF_HOST_ADDR,
               HECI_ASF_MESSAGE_ADDR,
               (UINT32 *) &QueryKvm,
               sizeof (AMT_QUERY_KVM)
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Send Query KVM failed %r\n", Status));
      return Status;
    }

    //
    // BIOS starts counting up to 8 minutes to wait for Query Request response.
    //
    TimeOut = 0;
    do {
      RecvLength = sizeof (AMT_QUERY_KVM_RESPONSE);
      Status = HeciWrapperReceive (
                 NON_BLOCKING,
                 &HostAddress,
                 &EngineAddress,
                 (UINT32 *) &QueryKvm,
                 &RecvLength
                 );

      MicroSecondDelay (KVM_STALL_1_SECOND);
      TimeOut++;
      if (TimeOut > KVM_MAX_WAIT_TIME) {
        DEBUG ((DEBUG_ERROR, "Read Query KVM timeout\n"));
        return EFI_TIMEOUT;
      }
    } while (EFI_ERROR (Status));
  } else {
    //
    // BIOS follows common requirement (wait 5 seconds with max 3 retries) for Cancel Request.
    //
    Status = HeciWrapperSendWithAck (
               BIOS_ASF_HOST_ADDR,
               HECI_ASF_MESSAGE_ADDR,
               (UINT32 *) &QueryKvm.Request,
               (UINT32) sizeof (AMT_QUERY_KVM),
               (UINT32 *) &QueryKvm.Response,
               &RecvLength
               );
  }

  *ResponseCode = QueryKvm.Response.ResponseCode;

  return Status;
}

//
// MAC Commands
//

/**
  This message is sent by the BIOS in order to pass MAC Address
  which should be used for LAN in a dock.

  @param[in] Enabled              Determines if MAC Passthrough should be used
  @param[in] MacAddress          MAC Address to be used

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmtSetMacPassthrough (
  IN UINT8                       Enabled,
  IN UINT8                       *MacAddress
  )
{
  EFI_STATUS                     Status;
  AMT_SET_MAC_PASSTHROUGH_BUFFER SetMacPassthrough;
  UINT32                         MeMode;
  UINT32                         RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  SetMacPassthrough.Request.AsfHeader.Data                 = 0;
  SetMacPassthrough.Request.AsfHeader.Fields.Command       = ASF_MESSAGE_COMMAND_MAC;
  SetMacPassthrough.Request.AsfHeader.Fields.ByteCount     = ASF_MESSAGE_BYTE_COUNT_MAP (AMT_SET_MAC_PASSTHROUGH);
  SetMacPassthrough.Request.AsfHeader.Fields.SubCommand    = ASF_MESSAGE_SUBCOMMAND_SET_MAC_PASSTHROUGH;
  SetMacPassthrough.Request.AsfHeader.Fields.VersionNumber = ASF_VERSION;

  SetMacPassthrough.Request.Enabled                        = Enabled;
  CopyMem (&SetMacPassthrough.Request.MacAddress, MacAddress, MAC_ADDRESS_SIZE);

  RecvLength = sizeof (AMT_SET_MAC_PASSTHROUGH_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &SetMacPassthrough.Request,
             (UINT32) sizeof (AMT_SET_MAC_PASSTHROUGH),
             (UINT32 *) &SetMacPassthrough.Response,
             &RecvLength
             );

  return Status;
}

/**
  Check Amt Response message header to see if it was valid

  @param[in] SubCommand   Sub Command for message
  @param[in] AsfHeader    ASF message header to check

  @retval EFI_SUCCESS            Header is valid
  @retval EFI_DEVICE_ERROR       Asf Header is invalid
**/
EFI_STATUS
CheckAmtResponseHeader (
  IN UINT8                                  SubCommand,
  IN ASF_MESSAGE_HEADER                     AsfHeader
  )
{
  if ((AsfHeader.Fields.Command != ASF_MESSAGE_COMMAND_ASF_CONFIGURATION) ||
      (AsfHeader.Fields.SubCommand != SubCommand) ||
      (AsfHeader.Fields.VersionNumber != ASF_VERSION)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Decode Amt Status to EFI Status

  @param[in] AmtStatus           AMT Status to decode

  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_INVALID_PARAMETER  Invalid Boot Options State
  @retval EFI_NOT_FOUND          HECI Communication failed
**/
EFI_STATUS
DecodeAmtStatusResponse (
  IN AMT_STATUS                    AmtStatus
  )
{
  EFI_STATUS                       Status;

  switch (AmtStatus) {
    case AmtStatusSuccess:
      Status = EFI_SUCCESS;
      break;
    case AmtStatusInternalError:
    case AmtStatusInvalidMessageLength:
    case AmtStatusInvalidParameter:
      Status = EFI_INVALID_PARAMETER;
      break;
    default:
      Status = EFI_NOT_FOUND;
      break;
  }

  return Status;
}

/**
  Set UEFI Boot Options States that are available for AMT to use by sending ASF
  command through HECI

  @param[in] AmtBootControl      AMT boot control

  @retval EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_INVALID_PARAMETER  Invalid Boot Options State
  @retval EFI_NOT_FOUND          HECI Communication failed
  @retval EFI_DEVICE_ERROR       ASF Response is invalid
**/
EFI_STATUS
AsfSetUefiBootOptionsState (
  IN AMT_BOOT_CONTROL      AmtBootControl
  )
{
  EFI_STATUS                          Status;
  SET_UEFI_BOOT_OPTIONS_STATE_BUFFER  SetBootOptionsState;
  UINT32                              MeMode;
  UINT32                              RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&SetBootOptionsState, sizeof (SET_UEFI_BOOT_OPTIONS_STATE_BUFFER));
  SetBootOptionsState.Request.AsfHeader.Fields.Command       = ASF_MESSAGE_COMMAND_ASF_CONFIGURATION;
  SetBootOptionsState.Request.AsfHeader.Fields.ByteCount     = ASF_MESSAGE_BYTE_COUNT_MAP (SET_UEFI_BOOT_OPTIONS_STATE);
  SetBootOptionsState.Request.AsfHeader.Fields.SubCommand    = ASF_MESSAGE_SUBCOMMAND_SET_UEFI_BOOT_OPTIONS_STATE;
  SetBootOptionsState.Request.AsfHeader.Fields.VersionNumber = ASF_VERSION;
  SetBootOptionsState.Request.AmtBootControl                 = AmtBootControl;
  RecvLength                                                 = sizeof (SET_UEFI_BOOT_OPTIONS_STATE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &SetBootOptionsState.Request,
             (UINT32) sizeof (SET_UEFI_BOOT_OPTIONS_STATE),
             (UINT32 *) &SetBootOptionsState.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = CheckAmtResponseHeader (ASF_MESSAGE_SUBCOMMAND_SET_UEFI_BOOT_OPTIONS_STATE, SetBootOptionsState.Response.AsfHeader);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = DecodeAmtStatusResponse (SetBootOptionsState.Response.Status);

  return Status;
}

/**
  Get UEFI Boot Options States that are available for AMT to use by sending ASF
  proper HECI Command.

  @param[out] AmtBootControl     AMT boot control

  @retval EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_INVALID_PARAMETER  Invalid Boot Options State
  @retval EFI_NOT_FOUND          HECI Communication failed
  @retval EFI_DEVICE_ERROR       ASF Response is invalid
**/
EFI_STATUS
AsfGetUefiBootOptionsState (
  OUT AMT_BOOT_CONTROL      *AmtBootControl
  )
{
  EFI_STATUS                           Status;
  GET_UEFI_BOOT_OPTIONS_STATE_BUFFER   GetUefiBootOptionsState;
  UINT32                               MeMode;
  UINT32                               RecvLength;

  if (AmtBootControl == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&GetUefiBootOptionsState, sizeof (GET_UEFI_BOOT_OPTIONS_STATE_BUFFER));
  GetUefiBootOptionsState.Request.AsfHeader.Fields.Command       = ASF_MESSAGE_COMMAND_ASF_CONFIGURATION;
  GetUefiBootOptionsState.Request.AsfHeader.Fields.ByteCount     = ASF_MESSAGE_BYTE_COUNT_MAP (GET_UEFI_BOOT_OPTIONS_STATE);
  GetUefiBootOptionsState.Request.AsfHeader.Fields.SubCommand    = ASF_MESSAGE_SUBCOMMAND_GET_UEFI_BOOT_OPTIONS_STATE;
  GetUefiBootOptionsState.Request.AsfHeader.Fields.VersionNumber = ASF_VERSION;
  RecvLength                                                     = sizeof (GET_UEFI_BOOT_OPTIONS_STATE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &GetUefiBootOptionsState.Request,
             (UINT32) sizeof (GET_UEFI_BOOT_OPTIONS_STATE),
             (UINT32 *) &GetUefiBootOptionsState.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = CheckAmtResponseHeader (ASF_MESSAGE_SUBCOMMAND_GET_UEFI_BOOT_OPTIONS_STATE, GetUefiBootOptionsState.Response.AsfHeader);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *AmtBootControl = GetUefiBootOptionsState.Response.AmtBootControl;

  return Status;
}

/**
  Update UEFI Boot Options available to ASF by sending proper HECI Command

  @param[in] UefiBootOptions      UEFI Boot Options
  @param[in] NumOfUefiBootOptions Number of UEFI Boot Options

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_INVALID_PARAMETER   Invalid Boot Options State
  @retval EFI_NOT_FOUND           HECI Communication failed
  @retval EFI_OUT_OF_RESOURCES    Ran out of resources
  @retval EFI_DEVICE_ERROR        ASF Response is invalid
**/
EFI_STATUS
AsfUpdateUefiBootOptions (
  IN UEFI_BOOT_OPTION      *UefiBootOptions,
  IN UINT16                NumOfUefiBootOptions
  )
{
  EFI_STATUS                       Status;
  UPDATE_UEFI_BOOT_OPTIONS_BUFFER  *UpdateUefiBootOptions;
  UINT32                           MeMode;
  UINT32                           Length;
  UINT32                           RecvLength;

  if ((NumOfUefiBootOptions > MAX_UEFI_BOOT_OPTIONS)  || (UefiBootOptions == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  Length = sizeof (UPDATE_UEFI_BOOT_OPTIONS) + (NumOfUefiBootOptions * sizeof (UEFI_BOOT_OPTION));

  UpdateUefiBootOptions = AllocateZeroPool (Length);
  if (UpdateUefiBootOptions == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  UpdateUefiBootOptions->Request.AsfHeader.Fields.Command       = ASF_MESSAGE_COMMAND_ASF_CONFIGURATION;
  UpdateUefiBootOptions->Request.AsfHeader.Fields.ByteCount     = ASF_MESSAGE_BYTE_COUNT (Length);
  UpdateUefiBootOptions->Request.AsfHeader.Fields.SubCommand    = ASF_MESSAGE_SUBCOMMAND_UPDATE_UEFI_BOOT_OPTIONS;
  UpdateUefiBootOptions->Request.AsfHeader.Fields.VersionNumber = ASF_VERSION;

  UpdateUefiBootOptions->Request.NumOfUefiBootOptions           = NumOfUefiBootOptions;
  CopyMem (UpdateUefiBootOptions->Request.UefiBootOptions, UefiBootOptions, (NumOfUefiBootOptions * sizeof (UEFI_BOOT_OPTION)));

  RecvLength                                                    = sizeof (UPDATE_UEFI_BOOT_OPTIONS_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &UpdateUefiBootOptions->Request,
             Length,
             (UINT32 *) &UpdateUefiBootOptions->Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    FreePool (UpdateUefiBootOptions);
    return Status;
  }

  Status = CheckAmtResponseHeader (ASF_MESSAGE_SUBCOMMAND_UPDATE_UEFI_BOOT_OPTIONS, UpdateUefiBootOptions->Response.AsfHeader);
  if (EFI_ERROR (Status)) {
    FreePool (UpdateUefiBootOptions);
    return EFI_DEVICE_ERROR;
  }

  Status = DecodeAmtStatusResponse (UpdateUefiBootOptions->Response.Status);

  FreePool (UpdateUefiBootOptions);

  return Status;
}

/**
  Get Boot Option with Parameters that being requested from ASF by sending proper
  HECI Command

  @param[out] UefiBootOptionWithParm    UEFI Boot Option with parameters

  @retval EFI_UNSUPPORTED               Current ME mode doesn't support this function
  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_INVALID_PARAMETER         Invalid Boot Options State
  @retval EFI_NOT_FOUND                 HECI Communication failed
  @retval EFI_DEVICE_ERROR              ASF Response is invalid
**/
EFI_STATUS
AsfGetUefiBootParameters (
  OUT UEFI_BOOT_OPTION_PARAMETER  *UefiBootOptionWithParm
  )
{
  EFI_STATUS                              Status;
  UINT32                                  MeMode;
  GET_UEFI_BOOT_OPTION_PARAMETERS_BUFFER  GetUefiBootOptions;
  UINT32                                  RecvLength;
  UINT32                                  Nonce;

  if (UefiBootOptionWithParm == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&GetUefiBootOptions, sizeof (GET_UEFI_BOOT_OPTION_PARAMETERS_BUFFER));
  GetUefiBootOptions.Request.AsfHeader.Fields.Command       = ASF_MESSAGE_COMMAND_ASF_CONFIGURATION;
  GetUefiBootOptions.Request.AsfHeader.Fields.ByteCount     = ASF_MESSAGE_BYTE_COUNT_MAP (GET_UEFI_BOOT_PARAMETERS);
  GetUefiBootOptions.Request.AsfHeader.Fields.SubCommand    = ASF_MESSAGE_SUBCOMMAND_GET_UEFI_BOOT_PARAMETERS;
  GetUefiBootOptions.Request.AsfHeader.Fields.VersionNumber = ASF_VERSION;
  if (!GetRandomNumber32 (&Nonce)) {
    return EFI_NOT_FOUND;
  }
  GetUefiBootOptions.Request.Nonce = Nonce;

  RecvLength = sizeof (GET_UEFI_BOOT_OPTION_PARAMETERS_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &GetUefiBootOptions.Request,
             (UINT32) sizeof (GET_UEFI_BOOT_PARAMETERS),
             (UINT32 *) &GetUefiBootOptions.Response,
             &RecvLength
             );

  Status = CheckAmtResponseHeader (ASF_MESSAGE_SUBCOMMAND_GET_UEFI_BOOT_PARAMETERS, GetUefiBootOptions.Response.AsfHeader);
  if (EFI_ERROR (Status) || GetUefiBootOptions.Response.Nonce != Nonce) {
    return EFI_DEVICE_ERROR;
  }

  CopyMem (UefiBootOptionWithParm, &GetUefiBootOptions.Response.UefiBootOptionWithParams, sizeof (UEFI_BOOT_OPTION_PARAMETER));

  return Status;
}

/**
  Get Root Certificate Authority(CA) Certificate from ASF at specified Index by sending
  proper HECI Command

  @param[in]   Index             Index
  @param[out]  Cert              Root CA Certificate

  @retval EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_INVALID_PARAMETER  Invalid Boot Options State
  @retval EFI_NOT_FOUND          HECI Communication failed
  @retval EFI_DEVICE_ERROR       ASF Response is invalid
**/
EFI_STATUS
AsfGetRootCaCertificate (
  IN UINT32                   Index,
  OUT ROOT_CA_CERTIFICATE     *Cert
  )
{
  EFI_STATUS                        Status;
  UINT32                            MeMode;
  GET_ROOT_CA_CERTIFICATE_BUFFER    GetRootCaCert;
  UINT32                            RecvLength;
  UINT32                            Nonce;

  if (Cert == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  GetRootCaCert.Request.AsfHeader.Fields.Command       = ASF_MESSAGE_COMMAND_ASF_CONFIGURATION;
  GetRootCaCert.Request.AsfHeader.Fields.ByteCount     = ASF_MESSAGE_BYTE_COUNT_MAP (GET_ROOT_CA_CERTIFICATE);
  GetRootCaCert.Request.AsfHeader.Fields.SubCommand    = ASF_MESSAGE_SUBCOMMAND_GET_ROOT_CA_CERTIFICATE;
  GetRootCaCert.Request.AsfHeader.Fields.VersionNumber = ASF_VERSION;

  if (!GetRandomNumber32 (&Nonce)) {
    return EFI_NOT_FOUND;
  }
  GetRootCaCert.Request.Nonce = Nonce;
  GetRootCaCert.Request.RootCACertIndex = Index;
  RecvLength = sizeof (GET_ROOT_CA_CERTIFICATE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &GetRootCaCert.Request,
             (UINT32) sizeof (GET_ROOT_CA_CERTIFICATE),
             (UINT32 *) &GetRootCaCert.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = CheckAmtResponseHeader (ASF_MESSAGE_SUBCOMMAND_GET_ROOT_CA_CERTIFICATE, GetRootCaCert.Response.AsfHeader);
  if (EFI_ERROR (Status) || GetRootCaCert.Response.Nonce != Nonce) {
    return EFI_DEVICE_ERROR;
  }

  CopyMem (Cert, &GetRootCaCert.Response.Cert, sizeof (ROOT_CA_CERTIFICATE));

  return Status;
}

/**
  Send One Click Recovery progress or error event using PET

  @param[in] EventOffset          ASF Event Data Codes for OCR Fw Progress/Error Events
  @param[in] EventData3           OCR Additional Event Data 3
  @param[in] EventData4           OCR Additional Event Data 4

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
**/
EFI_STATUS
SendOcrPetEvent (
  IN UINT8              EventOffset,
  IN UINT8              EventData3,
  IN UINT8              EventData4
  )
{
  EFI_STATUS            Status;
  PET_ALERT             OcrPetAlert;
  UINT32                Length;
  UINT32                MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  OcrPetAlert.AsfHeader.Fields.Command         = ASF_MESSAGE_COMMAND_MESSAGE;
  OcrPetAlert.AsfHeader.Fields.ByteCount       = ASF_MESSAGE_BYTE_COUNT_MAP (PET_ALERT);
  OcrPetAlert.AsfHeader.Fields.SubCommand      = ASF_MESSAGE_SUBCOMMAND_NORETRANSMIT;
  OcrPetAlert.AsfHeader.Fields.VersionNumber   = ASF_VERSION;
  OcrPetAlert.EventSensorType                  = ASF_EVENT_SENSOR_TYPE_SYS_FW_ERR_PROG;
  OcrPetAlert.EventType                        = ASF_EVENT_TYPE_SENSOR_SPECIFIC;
  OcrPetAlert.EventSourceType                  = ASF_EVENT_SOURCE_TYPE_ASF10;
  OcrPetAlert.EventSeverity                    = ASF_EVENT_SEVERITY_CODE_NONCRITICAL;
  OcrPetAlert.SensorDevice                     = ASF_SENSOR_DEVICE;
  OcrPetAlert.Sensornumber                     = ASF_SENSOR_NUMBER;
  OcrPetAlert.Entity                           = ASF_ENTITY_BIOS;
  OcrPetAlert.EntityInstance                   = 0xFF;

  OcrPetAlert.EventOffset = EventOffset;
  OcrPetAlert.EventData1 = ASF_OCR_EVENT_DATA1_EVENT_DATA_SET_BY_OEM;
  OcrPetAlert.EventData2 = ASF_OCR_EVENT_DATA2_EVENT_DATA_REASON;
  OcrPetAlert.EventData3 = EventData3;
  OcrPetAlert.EventData4 = EventData4;
  OcrPetAlert.EventData5 = 0x00;

  Length = sizeof (PET_ALERT);

  Status = HeciWrapperSend (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &OcrPetAlert,
             Length
             );

  return Status;
}

//
// WiFi profile Command
//

/**
  Get the wireless profile name

  @param[in, out] Profile         Profile data defined in wifi profile protocol

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @return Others                  Error happened
**/
EFI_STATUS
AsfWifiGetProfileName (
  IN OUT WIFI_PROFILE_DATA        *Profile
  )
{
  EFI_STATUS                      Status;
  UINT32                          MeMode;
  UINT32                          RecvLength;
  GET_WIFI_PROFILE_NAME_BUFFER    *ProfileNameBuffer;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  // Set up the ASF message commands for profile info transfer
  ProfileNameBuffer = (GET_WIFI_PROFILE_NAME_BUFFER *) AllocateZeroPool (sizeof (GET_WIFI_PROFILE_NAME_BUFFER));
  if (ProfileNameBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ProfileNameBuffer->Request.Data                   = 0;
  ProfileNameBuffer->Request.Fields.Command         = ASF_MESSAGE_COMMAND_ASF_CONFIGURATION;
  ProfileNameBuffer->Request.Fields.ByteCount       = ASF_MESSAGE_BYTE_COUNT_MAP (ASF_MESSAGE_HEADER);
  ProfileNameBuffer->Request.Fields.SubCommand      = ASF_MESSAGE_SUBCOMMAND_GET_WIFI_PROFILE_NAME;
  ProfileNameBuffer->Request.Fields.VersionNumber   = ASF_VERSION;
  RecvLength                                        = sizeof (GET_WIFI_PROFILE_NAME_BUFFER);

  Status = HeciWrapperSendWithAck (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &ProfileNameBuffer->Request,
             (UINT32) sizeof (ASF_MESSAGE_HEADER),
             (UINT32 *) &ProfileNameBuffer->Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    FreePool (ProfileNameBuffer);
    return Status;
  }

  Profile->Status = ProfileNameBuffer->Response.Status;
  if (!EFI_ERROR (Status) && (ProfileNameBuffer->Response.Status == AmtStatusSuccess)) {
    CopyMem (&Profile->ProfileName, &ProfileNameBuffer->Response.ProfileName, PROFILE_NAME_SIZE);
    FreePool (ProfileNameBuffer);
    return Status;
  }

  DEBUG ((DEBUG_ERROR, "Failed to get profile name with status 0x%X\n", Profile->Status));
  FreePool (ProfileNameBuffer);
  return Status;
}

/**
  Get the wireless profile data

  @param[in, out] Profile         Profile data defined in wifi profile protocol

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @return Others                  Error happened
**/
EFI_STATUS
AsfWifiGetProfileData (
  IN OUT WIFI_PROFILE_DATA        *Profile
  )
{
  EFI_STATUS                      Status;
  UINT32                          MeMode;
  UINT32                          RecvLength;
  GET_WIFI_PROFILE_DATA_BUFFER    *ProfileDataBuffer;

  if (Profile == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  // Set up the ASF message commands for profile info transfer
  ProfileDataBuffer = (GET_WIFI_PROFILE_DATA_BUFFER *) AllocateZeroPool (sizeof (GET_WIFI_PROFILE_DATA_BUFFER));
  if (ProfileDataBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (&ProfileDataBuffer->Request.Data, Profile->ProfileName, PROFILE_NAME_SIZE);
  ProfileDataBuffer->Request.AsfHeader.Fields.Command          = ASF_MESSAGE_COMMAND_ASF_CONFIGURATION;
  ProfileDataBuffer->Request.AsfHeader.Fields.ByteCount        = ASF_MESSAGE_BYTE_COUNT_MAP (ASF_PROFILE_MESSAGE_HEADER);
  ProfileDataBuffer->Request.AsfHeader.Fields.SubCommand       = ASF_MESSAGE_SUBCOMMAND_GET_WIFI_PROFILE_DATA;
  ProfileDataBuffer->Request.AsfHeader.Fields.VersionNumber    = ASF_VERSION;
  RecvLength                                                   = sizeof (GET_WIFI_PROFILE_DATA_BUFFER);

  Status = HeciWrapperSendWithAck (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &ProfileDataBuffer->Request,
             (UINT32) sizeof (ASF_PROFILE_MESSAGE_HEADER),
             (UINT32 *) &ProfileDataBuffer->Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    FreePool (ProfileDataBuffer);
    return Status;
  }

  if (ProfileDataBuffer->Response.Profile.Status == AmtStatusSuccess) {
    CopyMem (Profile, &ProfileDataBuffer->Response.Profile, sizeof (WIFI_PROFILE_DATA));
    FreePool (ProfileDataBuffer);
    return Status;
  }

  Profile->Status = ProfileDataBuffer->Response.Profile.Status;
  DEBUG ((DEBUG_ERROR, "%a failed request with status 0x%X\n", __FUNCTION__, Profile->Status));
  FreePool (ProfileDataBuffer);
  return Status;
}


/**
  Get the wireless profile specific 8021X client certificate

  @param[in, out] Profile         Profile data defined in wifi profile protocol

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @return Others                  Error happened
**/
EFI_STATUS
AsfWifiGetClientCertificate (
  IN OUT WIFI_PROFILE             *Profile
  )
{
  EFI_STATUS                      Status;
  UINT32                          MeMode;
  UINT32                          RecvLength;
  GET_WIFI_CLIENT_CERT_BUFFER     *CertBuffer;

  if (Profile == NULL || Profile->ProfileData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  // Set up the ASF message commands for profile info transfer
  CertBuffer = (GET_WIFI_CLIENT_CERT_BUFFER *) AllocateZeroPool (sizeof (GET_WIFI_CLIENT_CERT_BUFFER));
  if (CertBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (&CertBuffer->Request.Data, &Profile->ProfileData->ProfileName, PROFILE_NAME_SIZE);
  CertBuffer->Request.AsfHeader.Fields.Command         = ASF_MESSAGE_COMMAND_ASF_CONFIGURATION;
  CertBuffer->Request.AsfHeader.Fields.ByteCount       = ASF_MESSAGE_BYTE_COUNT_MAP (ASF_PROFILE_MESSAGE_HEADER);
  CertBuffer->Request.AsfHeader.Fields.SubCommand      = ASF_MESSAGE_SUBCOMMAND_GET_WIFI_8021X_CLIENT_CERT;
  CertBuffer->Request.AsfHeader.Fields.VersionNumber   = ASF_VERSION;
  RecvLength                                           = sizeof (GET_WIFI_CLIENT_CERT_BUFFER);

  Status = HeciWrapperSendWithAck (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &CertBuffer->Request,
             (UINT32) sizeof (ASF_PROFILE_MESSAGE_HEADER),
             (UINT32 *) &CertBuffer->Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    FreePool (CertBuffer);
    return Status;
  }

  if (CertBuffer->Response.Cert.Status == AmtStatusSuccess) {
    CopyMem (Profile->Cert, &CertBuffer->Response.Cert, sizeof (WIFI_8021X_CLIENT_CERT));
    FreePool (CertBuffer);
    return Status;
  }

  Profile->Cert->Status = CertBuffer->Response.Cert.Status;
  DEBUG ((DEBUG_ERROR, "%a failed request with status 0x%X\n", __FUNCTION__, Profile->Cert->Status));
  FreePool (CertBuffer);
  return Status;
}

/**
  Get the wireless profile specific 8021X client key

  @param[in, out] Profile         Profile data defined in wifi profile protocol

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @return Others                  Error happened
**/
EFI_STATUS
AsfWifiGetClientKey (
  IN OUT WIFI_PROFILE             *Profile
  )
{
  EFI_STATUS                      Status;
  UINT32                          MeMode;
  UINT32                          RecvLength;
  GET_WIFI_CLIENT_KEY_BUFFER      *KeyBuffer;

  if (Profile == NULL || Profile->ProfileData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  // Set up the ASF message commands for profile info transfer
  KeyBuffer = (GET_WIFI_CLIENT_KEY_BUFFER *) AllocateZeroPool (sizeof (GET_WIFI_CLIENT_KEY_BUFFER));
  if (KeyBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (&KeyBuffer->Request.Data, &Profile->ProfileData->ProfileName, PROFILE_NAME_SIZE);
  KeyBuffer->Request.AsfHeader.Fields.Command          = ASF_MESSAGE_COMMAND_ASF_CONFIGURATION;
  KeyBuffer->Request.AsfHeader.Fields.ByteCount        = ASF_MESSAGE_BYTE_COUNT_MAP (ASF_PROFILE_MESSAGE_HEADER);
  KeyBuffer->Request.AsfHeader.Fields.SubCommand       = ASF_MESSAGE_SUBCOMMAND_GET_WIFI_8021X_CLIENT_KEY;
  KeyBuffer->Request.AsfHeader.Fields.VersionNumber    = ASF_VERSION;
  RecvLength                                           = sizeof (GET_WIFI_CLIENT_KEY_BUFFER);

  Status = HeciWrapperSendWithAck (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &KeyBuffer->Request,
             (UINT32) sizeof (ASF_PROFILE_MESSAGE_HEADER),
             (UINT32 *) &KeyBuffer->Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    FreePool (KeyBuffer);
    return Status;
  }

  if (KeyBuffer->Response.Key.Status == AmtStatusSuccess) {
    CopyMem (Profile->Key, &KeyBuffer->Response.Key, sizeof (WIFI_8021X_CLIENT_KEY));
    FreePool (KeyBuffer);
    return Status;
  }

  Profile->Key->Status = KeyBuffer->Response.Key.Status;
  DEBUG ((DEBUG_ERROR, "%a failed request with status 0x%X\n", __FUNCTION__, Profile->Key->Status));
  FreePool (KeyBuffer);
  return Status;
}

/**
  Write storage erase report to a file with given file name.

  @param[in] FileName            File name of the written data.
  @param[in] DataSize            Size of the written data.
  @param[in] Data                The data to be written to a file.

  @retval EFI_SUCCESS                  Write data to the AMT WEB successfully
  @retval EFI_INVALID_PARAMETER        The parameter is invalid
  @retval EFI_DEVICE_ERROR             Failed to initialize HECI
  @retval EFI_TIMEOUT                  HECI is not ready for communication
  @retval EFI_UNSUPPORTED              Current ME mode doesn't support send this function
**/
EFI_STATUS
WriteStorageEraseReport (
  IN CHAR8   *FileName,
  IN UINT32  DataSize,
  IN UINT8   *Data
  )
{
  EFI_STATUS                 Status;
  UINT32                     Length;
  UINT32                     RecvLength;
  ASF_WRITE_FILE_BUFFER      WriteFileBuffer;
  UINT32                     MeMode;
  CHAR8                      ApplicationName[] = "BIOS";

  if ((FileName == NULL) || (Data == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if ((AsciiStrLen (FileName) > (MAX_FILE_NAME_LENGTH + 1)) || DataSize > MAX_FILE_SIZE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&WriteFileBuffer, sizeof (ASF_WRITE_FILE_BUFFER));
  WriteFileBuffer.Request.AsfHeader.Fields.Command        = ASF_MESSAGE_COMMAND_ASF_CONFIGURATION;
  WriteFileBuffer.Request.AsfHeader.Fields.ByteCount      = 0;
  WriteFileBuffer.Request.AsfHeader.Fields.SubCommand     = ASF_MESSAGE_SUBCOMMAND_WRITE_FILE;
  WriteFileBuffer.Request.AsfHeader.Fields.VersionNumber  = ASF_VERSION;
  WriteFileBuffer.Request.FileSize                        = DataSize;
  CopyMem (WriteFileBuffer.Request.File, Data, WriteFileBuffer.Request.FileSize);
  CopyMem (WriteFileBuffer.Request.ApplicationName, ApplicationName, AsciiStrLen (ApplicationName));
  CopyMem (WriteFileBuffer.Request.FileName, FileName, AsciiStrLen (FileName));

  Length                                                  = sizeof (ASF_WRITE_FILE_REQUEST);
  RecvLength                                              = sizeof (ASF_WRITE_FILE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &WriteFileBuffer.Request,
             Length,
             (UINT32 *) &WriteFileBuffer.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a : %r\n", __FUNCTION__, Status));
    return Status;
  }

  if (WriteFileBuffer.Response.AsfHeader.Fields.Command       != ASF_MESSAGE_COMMAND_ASF_CONFIGURATION ||
      WriteFileBuffer.Response.AsfHeader.Fields.ByteCount      > ASF_MESSAGE_BYTE_COUNT_MAP (ASF_WRITE_FILE_RESPONSE) ||
      WriteFileBuffer.Response.AsfHeader.Fields.SubCommand    != ASF_MESSAGE_SUBCOMMAND_WRITE_FILE ||
      WriteFileBuffer.Response.AsfHeader.Fields.VersionNumber != ASF_VERSION) {
    ZeroMem (&WriteFileBuffer.Response, sizeof (ASF_WRITE_FILE_RESPONSE));
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "%a - return status:%d.\n", __FUNCTION__, WriteFileBuffer.Response.Status));
  return EFI_SUCCESS;
}

/**
  Get HTTP Proxy information by sending a proper ASF command through HECI

  @param[out] AsfHttpProxy        HTTP Proxy information returned from HECI
  @param[out] NumberOfProxies     Number of Proxy servers returned from HECI

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
GetHttpProxy (
  OUT ASF_HTTP_PROXY            *AsfHttpProxy,
  OUT UINT32                    *NumberOfProxies
  )
{
  EFI_STATUS                Status;
  UINT32                    RecvLength;
  UINT32                    MeMode;
  GET_HTTP_PROXY_BUFFER     GetHttpProxy;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&GetHttpProxy, sizeof (GET_HTTP_PROXY_BUFFER));
  GetHttpProxy.Request.AsfHeader.Data                 = 0;
  GetHttpProxy.Request.AsfHeader.Fields.Command       = ASF_MESSAGE_COMMAND_ASF_CONFIGURATION;
  GetHttpProxy.Request.AsfHeader.Fields.ByteCount     = ASF_MESSAGE_BYTE_COUNT_MAP (GET_HTTP_PROXY_REQUEST);
  GetHttpProxy.Request.AsfHeader.Fields.SubCommand    = ASF_MESSAGE_SUBCOMMAND_GET_HTTP_PROXY;
  GetHttpProxy.Request.AsfHeader.Fields.VersionNumber = ASF_VERSION;
  RecvLength                                          = sizeof (GET_HTTP_PROXY_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_ASF_HOST_ADDR,
             HECI_ASF_MESSAGE_ADDR,
             (UINT32 *) &GetHttpProxy.Request,
             (UINT32) sizeof (GET_HTTP_PROXY_REQUEST),
             (UINT32 *) &GetHttpProxy.Response,
             &RecvLength
             );
  if (!EFI_ERROR (Status)) {
    *NumberOfProxies = GetHttpProxy.Response.NumberOfProxies;
    CopyMem (AsfHttpProxy, &GetHttpProxy.Response.Proxies, sizeof (ASF_HTTP_PROXY) * *NumberOfProxies);
  }

  return Status;
}
