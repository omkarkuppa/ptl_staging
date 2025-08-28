/** @file
  Implementation file for Heci Message functionality

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2006 Intel Corporation.

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
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PerformanceLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TimerLib.h>
#include <Library/DxeMeLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/MeInfoLib.h>
#include <Library/RngLib.h>

#include <Protocol/Wdt.h>
#include <Protocol/HeciControlProtocol.h>

#include <Register/HeciRegs.h>

#include <IntelRcStatusCode.h>

/**
  Checks if there were errors in HECI MKHI communication.

  @param[in] Status       Status returned from Heci
  @param[in] Response     Mkhi Header returned from Heci
  @param[in] Command      Command Id that was sent to Heci, to see if response is for the right command
  @param[in] CheckResult  If true, function will check Result from Heci Msg; if false, it will not

  @retval True            Errors detected
  @retval False           No errors present
**/
BOOLEAN
HeciMkhiError (
  IN EFI_STATUS          Status,
  IN MKHI_MESSAGE_HEADER Response,
  IN UINT32              Command,
  IN BOOLEAN             CheckResult
  )
{
  if (EFI_ERROR (Status) ||
      Response.Fields.Command != Command ||
      Response.Fields.IsResponse != 1 ||
      (CheckResult && (Response.Fields.Result != 0))) {
    return TRUE;
  }
  return FALSE;
}

//
// Interface functions of HeciMsgLib
//

//
// ME Client - HBM
//

/**
  Checks if CSME supports HBM protocol version 2.2.

  @param[in] HeciDev                HECI device number

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
  @retval EFI_INCOMPATIBLE_VERSION  CSME does not support protocol version required by BIOS
**/
EFI_STATUS
HbmHostVersion (
  IN HECI_DEVICE HeciDev
  )
{
  EFI_STATUS                Status;
  HBM_HOST_VERSION_BUFFER   HbmHostVersion;
  UINT32                    RecvLength;

  ZeroMem (&HbmHostVersion, sizeof (HBM_HOST_VERSION_BUFFER));
  HbmHostVersion.Request.Command.Data             = HOST_VERSION_REQUEST;
  HbmHostVersion.Request.HostVersion.MajorVersion = HBM_MAJOR_VERSION_2;
  HbmHostVersion.Request.HostVersion.MinorVersion = HBM_MINOR_VERSION_2;
  RecvLength                                      = sizeof (HBM_HOST_VERSION_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_HBM_MESSAGE_ADDR,
             (UINT32 *) &HbmHostVersion.Request,
             (UINT32) sizeof (HbmHostVersion.Request),
             (UINT32 *) &HbmHostVersion.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (HbmHostVersion.Response.Command.Fields.Command != HOST_VERSION_REQUEST || !HbmHostVersion.Response.Command.Fields.IsResponse) {
    return EFI_ABORTED;
  } else if (HbmHostVersion.Response.HostVersionSupported != 0x01) {
    return EFI_INCOMPATIBLE_VERSION;
  }

  return Status;
}

/**
  Instructs CSME to stop all communication on given Heci device

  @param[in] HeciDev                HECI device number

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HbmHostStop (
  IN HECI_DEVICE HeciDev
  )
{
  EFI_STATUS              Status;
  HBM_HOST_STOP_BUFFER    HbmHostStop;
  UINT32                  RecvLength;

  ZeroMem (&HbmHostStop, sizeof (HBM_HOST_STOP_BUFFER));
  HbmHostStop.Request.Command.Data  = HOST_STOP_REQUEST;
  HbmHostStop.Request.Reason        = STOP_REASON_REQUEST_FROM_DRIVER;
  RecvLength                        = sizeof (HBM_HOST_STOP_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_HBM_MESSAGE_ADDR,
             (UINT32 *) &HbmHostStop,
             (UINT32) sizeof (HBM_HOST_STOP_REQUEST),
             (UINT32 *) &HbmHostStop,
             &RecvLength
             );

  return Status;
}

/**
  Checks at which CSME addresses there are any clients

  @param[in]  HeciDev               HECI device number
  @param[out] ValidAddresses        Mask of addresses that have a client behind them

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
**/
EFI_STATUS
HbmHostEnumRequest (
  IN HECI_DEVICE    HeciDevice,
  OUT UINT8         *ValidAddresses
  )
{
  EFI_STATUS                    Status;
  HBM_HOST_ENUMERATION_BUFFER   HostEnum;
  UINT32                        RecvLength;

  ZeroMem (&HostEnum, sizeof (HBM_HOST_ENUMERATION_BUFFER));
  HostEnum.Request.Command.Data = HOST_ENUMERATE_REQUEST;
  RecvLength                    = sizeof (HBM_HOST_ENUMERATION_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_HBM_MESSAGE_ADDR,
             (UINT32 *) &HostEnum.Request,
             (UINT32) sizeof (HBM_HOST_ENUMERATION_REQUEST),
             (UINT32 *) &HostEnum.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (HostEnum.Response.Command.Fields.Command != HOST_ENUMERATE_REQUEST || !HostEnum.Response.Command.Fields.IsResponse) {
    return EFI_ABORTED;
  }

  CopyMem (ValidAddresses, &HostEnum.Response.ValidAddresses, sizeof (HostEnum.Response.ValidAddresses));

  return Status;
}

/**
  Retrieves GUID of a single CSME client

  @param[in]  HeciDev               HECI device number
  @param[in]  ClientAddr            Address of the CSME client
  @param[out] ProtocolGuid          GUID of the client

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
**/
EFI_STATUS
HbmGetClientProperties (
  IN  HECI_DEVICE HeciDevice,
  IN  UINT8       ClientAddr,
  OUT EFI_GUID    *ProtocolGuid
  )
{
  EFI_STATUS                          Status;
  HBM_HOST_CLIENT_PROPERTIES_BUFFER   HostClientProperties;
  UINT32                              RecvLength;

  HostClientProperties.Request.Command.Data = HOST_CLIENT_PROPERTIES_REQUEST;
  HostClientProperties.Request.Address      = ClientAddr;
  RecvLength                                = sizeof (HBM_HOST_CLIENT_PROPERTIES_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_HBM_MESSAGE_ADDR,
             (UINT32 *) &HostClientProperties.Request,
             (UINT32) sizeof (HBM_HOST_CLIENT_PROPERTIES_REQUEST),
             (UINT32 *) &HostClientProperties.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (HostClientProperties.Response.Status ||
      HostClientProperties.Response.Command.Fields.Command != HOST_CLIENT_PROPERTIES_REQUEST ||
      !HostClientProperties.Response.Command.Fields.IsResponse) {
    DEBUG ((DEBUG_ERROR, "Identify failed\n"));
    return EFI_ABORTED;
  }

  CopyMem (ProtocolGuid, &HostClientProperties.Response.ClientProperties.ProtocolName, sizeof (EFI_GUID));

  return EFI_SUCCESS;
}

/**
  Creates a connection between given host and CSME addresses

  @param[in] HeciDev                HECI device number
  @param[in] MeAddress              Address of the CSME client
  @param[in] HostAddress            Address of the Host client

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
**/
EFI_STATUS
HbmHostClientConnect (
  IN HECI_DEVICE HeciDev,
  IN UINT8       MeAddress,
  IN UINT8       HostAddress
  )
{
  EFI_STATUS                  Status;
  HBM_CLIENT_CONNECT_BUFFER   HbmHostClientConnect;
  UINT32                      RecvLength;

  ZeroMem (&HbmHostClientConnect, sizeof (HBM_CLIENT_CONNECT_BUFFER));
  HbmHostClientConnect.Request.Command.Data = CLIENT_CONNECT_REQUEST;
  HbmHostClientConnect.Request.MeAddress    = MeAddress;
  HbmHostClientConnect.Request.HostAddress  = HostAddress;
  RecvLength                                = sizeof (HBM_CLIENT_CONNECT_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_HBM_MESSAGE_ADDR,
             (UINT32 *) &HbmHostClientConnect.Request,
             (UINT32) sizeof (HBM_CLIENT_CONNECT_REQUEST),
             (UINT32 *) &HbmHostClientConnect.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (HbmHostClientConnect.Response.Command.Fields.Command != CLIENT_CONNECT_REQUEST || !HbmHostClientConnect.Response.Command.Fields.IsResponse) {
    DEBUG ((
      DEBUG_ERROR,
      "HbmHostClientConnect (%x:%x) %r %x %x\n",
      MeAddress,
      HostAddress,
      Status,
      HbmHostClientConnect.Response.Command.Fields.Command,
      HbmHostClientConnect.Response.Command.Fields.IsResponse
      ));
    return EFI_ABORTED;
  } else if (HbmHostClientConnect.Response.Status) {
    DEBUG ((DEBUG_ERROR, "HbmHostClientConnect (%x:%x) %x\n", MeAddress, HostAddress, HbmHostClientConnect.Response.Status));
    return EFI_ABORTED;
  }

  return Status;
}

/**
  Disconnects an established connection between a Host and CSME clients

  @param[in] HeciDev                HECI device number
  @param[in] HostAddr               Host Address of the connection
  @param[in] MeAddr                 CSME Address of the connection

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
**/
EFI_STATUS
HbmClientDisconnect (
  IN HECI_DEVICE HeciDev,
  IN UINT8       HostAddr,
  IN UINT8       MeAddr
  )
{
  EFI_STATUS                      Status;
  HBM_CLIENT_DISCONNECT_BUFFER    ClientDisconnect;
  UINT32                          RecvLength;

  ZeroMem ((UINT8 *) &ClientDisconnect, sizeof (HBM_CLIENT_DISCONNECT_BUFFER));
  ClientDisconnect.Request.MeAddress    = MeAddr;
  ClientDisconnect.Request.HostAddress  = HostAddr;
  ClientDisconnect.Request.Command.Data = CLIENT_DISCONNECT_REQUEST;
  RecvLength                            = sizeof (HBM_CLIENT_DISCONNECT_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_HBM_MESSAGE_ADDR,
             (UINT32 *) &ClientDisconnect.Request,
             (UINT32) sizeof (HBM_CLIENT_DISCONNECT_REQUEST),
             (UINT32 *) &ClientDisconnect.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (ClientDisconnect.Response.Status ||
      ClientDisconnect.Response.Command.Fields.Command != CLIENT_DISCONNECT_REQUEST ||
      !ClientDisconnect.Response.Command.Fields.IsResponse) {
    Status = EFI_ABORTED;
  }

  return Status;
}

/**
  Sends a flow control token from BIOS to CSME that allows CSME to send one message over connection
  identified by host:Me address pair

  @param[in] HeciDev                HECI device number
  @param[in] HostAddr               Address of the Host client
  @param[in] MeAddr                 Address of the CSME client

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HbmSendFlowControlToken (
  IN HECI_DEVICE   HeciDev,
  IN UINT8         HostAddr,
  IN UINT8         MeAddr
  )
{
  HBM_FLOW_CONTROL    FlowControl;

  ZeroMem (&FlowControl, sizeof (HBM_FLOW_CONTROL));
  FlowControl.Command.Fields.Command = FLOW_CONTROL;
  FlowControl.HostAddress            = HostAddr;
  FlowControl.MeAddress              = MeAddr;

  return HeciWrapperSend (
           BIOS_FIXED_HOST_ADDR,
           HECI_HBM_MESSAGE_ADDR,
           (UINT32 *) &FlowControl,
           (UINT32) sizeof (HBM_FLOW_CONTROL)
           );
}

/**
  Attempts to receive a flow control token from CSME. This token allows BIOS to send one message over commection
  identified by Host:Me address pair

  @param[in] HeciDev                HECI device number
  @param[in] HostAddr               Address of the Host client
  @param[in] MeAddr                 Address of the CSME client

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HbmGetFlowControlToken (
  IN HECI_DEVICE HeciDev,
  IN UINT8       HostAddr,
  IN UINT8       MeAddr
  )
{
  EFI_STATUS          Status;
  HBM_FLOW_CONTROL    FlowControl;
  UINT32              TimeLeft;
  UINT32              RecvLength;
  UINT8               HostAddress;
  UINT8               EngineAddress;

  Status = EFI_SUCCESS;

  TimeLeft = 1000;
  while (TimeLeft != 0) {
    MicroSecondDelay (1000);
    TimeLeft--;
    RecvLength = sizeof (HBM_FLOW_CONTROL);

    Status = HeciWrapperReceive (
               NON_BLOCKING,
               &HostAddress,
               &EngineAddress,
               (UINT32 *) &FlowControl,
               &RecvLength
               );
    if (Status == EFI_NO_RESPONSE) {
      continue;
    } else if (EFI_ERROR (Status)) {
      return Status;
    } else {
      if (FlowControl.Command.Fields.Command == FLOW_CONTROL &&
          FlowControl.HostAddress == HostAddr &&
          FlowControl.MeAddress == MeAddr) {
        return EFI_SUCCESS;
      } else {
        DEBUG ((DEBUG_ERROR, "Wrong Token received\n"));
        return EFI_DEVICE_ERROR;
      }
    }
  }

  DEBUG ((DEBUG_ERROR, "Token not received %r\n", Status));
  return Status;
}

/**
  Informs about Host and CSME capabilities over HBM

  @param[in]      HeciDev           HECI device number
  @param[in, out] HbmCaps             On input:  Host HBM capabilities
                                      On output: CSME HBM capabilities

  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_INVALID_PARAMETER     NULL parameter
  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
**/
EFI_STATUS
HbmCapabilitiesRequest (
  IN     HECI_DEVICE      HeciDev,
  IN OUT HBM_CAPS_DATA    *HbmCaps
  )
{
  EFI_STATUS                Status;
  HBM_CAPABILITIES_BUFFER   HbmCapabilities;
  UINT32                    RecvLength;

  if (HbmCaps == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem ((UINT8 *) &HbmCapabilities, sizeof (HBM_CAPABILITIES_BUFFER));
  CopyMem (&HbmCapabilities.Request.Capabilities, HbmCaps, sizeof (HBM_CAPS_DATA));
  HbmCapabilities.Request.Command.Data = CAPABILITIES_REQUEST;
  RecvLength                           = sizeof (HBM_CAPABILITIES_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_HBM_MESSAGE_ADDR,
             (UINT32 *) &HbmCapabilities.Request,
             (UINT32) sizeof (HBM_CAPABILITIES_REQUEST),
             (UINT32 *) &HbmCapabilities.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (HbmCapabilities.Response.Command.Fields.Command != CAPABILITIES_REQUEST || !HbmCapabilities.Response.Command.Fields.IsResponse) {
    Status = EFI_ABORTED;
  } else {
    CopyMem (HbmCaps, &HbmCapabilities.Response.Capabilities, sizeof (HBM_CAPS_DATA));
  }

  return Status;
}

/**
  Requests mapping CSME HECI client MMIO to the given physcial memory addresses.

  @param[in] HeciDev                HECI device number
  @param[in] ClientDmaBufferId      Identifies Client Id to which physical address should be mapped
  @param[in] AddressLow             Physical address for DMA buffer (Low DWORD)
  @param[in] AddressHigh            Physical address for DMA buffer (High DWORD)
  @param[in] Size                   Size of the physical memory to be mapped

  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
**/
EFI_STATUS
HbmMapRequest (
  IN HECI_DEVICE            HeciDev,
  IN UINT8                  ClientDmaBufferId,
  IN UINT32                 AddressLow,
  IN UINT32                 AddressHigh,
  IN UINT32                 Size
  )
{
  EFI_STATUS                  Status;
  HBM_CLIENT_DMA_MAP_BUFFER   ClientDmaMap;
  UINT32                      RecvLength;

  ZeroMem ((UINT8 *) &ClientDmaMap, sizeof (HBM_CLIENT_DMA_MAP_BUFFER));
  ClientDmaMap.Request.Command.Data   = CLIENT_DMA_MAP_REQUEST;
  ClientDmaMap.Request.ClientBufferId = ClientDmaBufferId;
  ClientDmaMap.Request.AddressLow     = AddressLow;
  ClientDmaMap.Request.AddressHigh    = AddressHigh;
  ClientDmaMap.Request.Size           = Size;
  RecvLength                          = sizeof (HBM_CLIENT_DMA_MAP_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_HBM_MESSAGE_ADDR,
             (UINT32 *) &ClientDmaMap.Request,
             (UINT32) sizeof (HBM_CLIENT_DMA_MAP_REQUEST),
             (UINT32 *) &ClientDmaMap.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (ClientDmaMap.Response.Status ||
      ClientDmaMap.Response.Command.Fields.Command != CLIENT_DMA_MAP_REQUEST ||
      !ClientDmaMap.Response.Command.Fields.IsResponse) {
    Status = EFI_ABORTED;
  }

  return Status;
}

/**
  Requests unmapping CSME HECI client MMIO from the given physcial memory addresses.

  @param[in] HeciDev                HECI device number
  @param[in] ClientDmaBufferId      Identifies Client Id from which physical address should be unampped

  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
**/
EFI_STATUS
HbmUnmapRequest (
  IN HECI_DEVICE HeciDev,
  IN UINT8       ClientDmaBufferId
  )
{
  EFI_STATUS                    Status;
  HBM_CLIENT_DMA_UNMAP_BUFFER   ClientDmaUnmap;
  UINT32                        RecvLength;

  ZeroMem ((UINT8 *) &ClientDmaUnmap, sizeof (HBM_CLIENT_DMA_UNMAP_BUFFER));
  ClientDmaUnmap.Request.Command.Data   = CLIENT_DMA_UNMAP_REQUEST;
  ClientDmaUnmap.Request.ClientBufferId = ClientDmaBufferId;
  RecvLength                            = sizeof (HBM_CLIENT_DMA_UNMAP_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_HBM_MESSAGE_ADDR,
             (UINT32 *) &ClientDmaUnmap.Request,
             (UINT32) sizeof (HBM_CLIENT_DMA_UNMAP_REQUEST),
             (UINT32 *) &ClientDmaUnmap.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (ClientDmaUnmap.Response.Status ||
      ClientDmaUnmap.Response.Command.Fields.Command != CLIENT_DMA_UNMAP_REQUEST ||
      !ClientDmaUnmap.Response.Command.Fields.IsResponse) {
    Status = EFI_ABORTED;
  }

  return Status;
}

//
// ME Client - ISH Service
//

/**
  This message is sent by the BIOS when it wants to send
  PDT Unlock Message to ISH.

  @param[in] FileName             File name of bios2ish file.

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function or EOP was sent
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_BAD_BUFFER_SIZE     The size of FileName is larger than maximum allowed value
**/
EFI_STATUS
HeciPdtUnlockMsg (
  IN CONST CHAR8*               FileName
  )
{
  EFI_STATUS            Status;
  PDT_MESSAGE_BUFFER    PdtUnlock;
  UINT32                MeMode;
  UINT32                RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  PdtUnlock.Request.Command           = ISH_SET_FILE_CMD;
  PdtUnlock.Request.MessageLength     = sizeof (PDT_MESSAGE_REQUEST) - 4;
  PdtUnlock.Request.NumberOfElements  = 0x1;
  PdtUnlock.Request.Data.DataType     = PDT_UNLOCK_DATA_DATA_TYPE;
  PdtUnlock.Request.Data.PayloadSize  = PDT_UNLOCK_DATA_PAYLOAD_SIZE;
  RecvLength                          = sizeof (PDT_MESSAGE_RESPONSE);

  if (AsciiStrLen (FileName) > (sizeof (PdtUnlock.Request.FileName))) {
    ASSERT (FALSE);
    return EFI_BAD_BUFFER_SIZE;
  }

  ZeroMem (PdtUnlock.Request.FileName, sizeof (PdtUnlock.Request.FileName));
  CopyMem (PdtUnlock.Request.FileName, FileName, AsciiStrLen (FileName));

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_ISH_SERVICE_MESSAGE_ADDR,
             (UINT32 *) &PdtUnlock.Request,
             (UINT32) sizeof (PDT_MESSAGE_REQUEST),
             (UINT32 *) &PdtUnlock.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (PdtUnlock.Response.Status != BiosToIshStatusSuccess) {
    DEBUG ((DEBUG_ERROR, "BIOS2ISH Response Status: %x\n", PdtUnlock.Response.Status));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  This message is sent by the BIOS when it wants to send
  PDT Update Message

  @param[in] FileName             File name of bios2ish file.
  @param[in] PdtFileBuffer        Contains ISH PDT file
  @param[in] PdtBufferSize        Size of PDT file

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function or EOP was sent
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_ABBORTED            Unable to allocate memory
  @retval EFI_BAD_BUFFER_SIZE     The size of FileName is larger than maximum allowed value
**/
EFI_STATUS
HeciPdtUpdateMsg (
  IN CONST CHAR8*  FileName,
  IN UINT8         *PdtFileBuffer,
  IN UINT16        PdtBufferSize
  )
{
  EFI_STATUS            Status;
  PDT_MESSAGE_BUFFER    *BiosToIshBuffer;
  UINT32                MeMode;
  UINT32                Length;
  UINT32                RecvLength;

  if ((PdtFileBuffer == NULL) || (PdtBufferSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  RecvLength = sizeof (PDT_MESSAGE_RESPONSE);
  //
  // Get 4 byte aligned Length
  //
  Length = sizeof (PDT_MESSAGE_REQUEST) + PdtBufferSize;
  if (Length % 4) {
    Length += (4 - (Length % 4));
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  BiosToIshBuffer = (PDT_MESSAGE_BUFFER *) AllocateZeroPool (Length);
  if (BiosToIshBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "BIOS2ISH Could not allocate Memory\n"));
    return EFI_ABORTED;
  }

  BiosToIshBuffer->Request.Command          = ISH_SET_FILE_CMD;
  BiosToIshBuffer->Request.MessageLength    = (UINT16) (Length - 4); // Command and MessageLength should not be counted here
  BiosToIshBuffer->Request.NumberOfElements = 0x1;
  BiosToIshBuffer->Request.Data.DataType    = PDT_UPDATE_DATA_DATA_TYPE;
  BiosToIshBuffer->Request.Data.PayloadSize = PdtBufferSize;

  if (AsciiStrLen (FileName) > (sizeof (BiosToIshBuffer->Request.FileName))) {
    ASSERT (FALSE);
    FreePool (BiosToIshBuffer);
    return EFI_BAD_BUFFER_SIZE;
  }

  ZeroMem (BiosToIshBuffer->Request.FileName, sizeof (BiosToIshBuffer->Request.FileName));
  CopyMem (BiosToIshBuffer->Request.FileName, FileName, AsciiStrLen (FileName));

  CopyMem ((VOID *)((UINTN) BiosToIshBuffer + sizeof (PDT_MESSAGE_REQUEST)), PdtFileBuffer, (UINTN) PdtBufferSize);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_ISH_SERVICE_MESSAGE_ADDR,
             (UINT32 *) &BiosToIshBuffer->Request,
             Length,
             (UINT32 *) &BiosToIshBuffer->Response,
             &RecvLength
             );
  if (!EFI_ERROR (Status) && (BiosToIshBuffer->Response.Status != BiosToIshStatusSuccess)) {
    DEBUG ((DEBUG_ERROR, "BIOS2ISH Response Status: %x\n", BiosToIshBuffer->Response.Status));
    Status = EFI_DEVICE_ERROR;
  }

  FreePool (BiosToIshBuffer);

  return Status;
}

/**
  This message is sent by the BIOS when it wants to send
  Sensor Control Message to ISH.

  @param[in] FileName                   File name of bios2ish file.
  @param[in] SensorExposurePayload      Table of Sensor Exposure settings given as buffer
  @param[in] SensorExposurePayloadSize  SensorExposurePayload size

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function or EOP was sent
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate memory
  @retval EFI_BAD_BUFFER_SIZE     The size of FileName is larger than maximum allowed value
**/
EFI_STATUS
HeciSensorControlMsg (
  IN CONST CHAR8*               FileName,
  IN UINT8                      *SensorExposurePayload,
  IN UINT16                     SensorExposurePayloadSize
  )
{
  EFI_STATUS            Status;
  PDT_MESSAGE_BUFFER    *SensorControl;
  UINT32                MeMode;
  UINT16                Length;
  UINT32                RecvLength;

  if ((SensorExposurePayload == NULL) || (SensorExposurePayloadSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  Length      = sizeof (PDT_MESSAGE_REQUEST) + SensorExposurePayloadSize;
  RecvLength  = sizeof (PDT_MESSAGE_RESPONSE);

  SensorControl = (PDT_MESSAGE_BUFFER *) AllocateZeroPool (Length);
  if (SensorControl == NULL) {
    DEBUG ((DEBUG_ERROR, "BIOS2ISH Could not allocate Memory\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  SensorControl->Request.Command          = ISH_SET_FILE_CMD;
  SensorControl->Request.MessageLength    = Length - 4; // Command and MessageLength should not be counted here
  SensorControl->Request.NumberOfElements = 0x1;
  SensorControl->Request.Data.DataType    = PDT_SENSOR_CONTROL_DATA_TYPE;
  SensorControl->Request.Data.PayloadSize = SensorExposurePayloadSize;

  if (AsciiStrLen (FileName) > (sizeof (SensorControl->Request.FileName))) {
    ASSERT (FALSE);
    FreePool (SensorControl);
    return EFI_BAD_BUFFER_SIZE;
  }

  ZeroMem (SensorControl->Request.FileName, sizeof (SensorControl->Request.FileName));
  CopyMem (SensorControl->Request.FileName, FileName, AsciiStrLen (FileName));

  CopyMem ((VOID *)((UINTN) SensorControl + sizeof (PDT_MESSAGE_REQUEST)), SensorExposurePayload, (UINTN) SensorExposurePayloadSize);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_ISH_SERVICE_MESSAGE_ADDR,
             (UINT32 *) &SensorControl->Request,
             Length,
             (UINT32 *) &SensorControl->Response,
             &RecvLength
             );
  if (!EFI_ERROR (Status) && (SensorControl->Response.Status != BiosToIshStatusSuccess)) {
    DEBUG ((DEBUG_ERROR, "BIOS2ISH Response Status: %x\n", SensorControl->Response.Status));
    Status = EFI_DEVICE_ERROR;
  }

  FreePool (SensorControl);

  return Status;
}

//
// ME Client - PSR
//

/**
  Generate User Nonce as an array of "NonceSize" bytes

  @param[in]  NonceSize           Size of nonce array to be generated
  @param[out] NonceArray          User Nonce array

**/
STATIC
VOID
GenerateUserNonce (
  IN  UINT32  NonceSize,
  OUT UINT8   *NonceArray
  )
{
  UINT16 Nonce;
  UINT32 Index;

  Nonce = 0;
  Index = 0;

  if (NonceArray != NULL) {
    DEBUG ((DEBUG_INFO, "User Nonce:\t"));
    for (Index = 0; Index < NonceSize; Index++) {
      GetRandomNumber16 (&Nonce);
      NonceArray[Index] = (UINT8) Nonce;
      DEBUG ((DEBUG_INFO, "%x\t", NonceArray[Index]));
    }
  }
}

/**
  Inform CSME of Psr Capability Set.

  @param[in] CapabilityMask       Bit mask of all capabilities supported by PSR feature.

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current CSME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciSendPsrCapabilitySet (
  IN UINT32   CapabilityMask
  )
{
  EFI_STATUS                          Status;
  PSR_SUPPORTED_CAPABILITIES_BUFFER   CapabilitiesBuffer;
  UINT32                              MeMode;
  UINT32                              RecvLength;

  DEBUG ((DEBUG_INFO, "(PSR) %a() enter\n", __FUNCTION__));

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&CapabilitiesBuffer, sizeof (PSR_SUPPORTED_CAPABILITIES_BUFFER));
  CapabilitiesBuffer.Request.Header.Command   = PSR_HECI_CAPABILITIES_UPDATE;
  CapabilitiesBuffer.Request.Header.ByteCount = sizeof (PSR_SUPPORTED_CAPABILITIES_UPDATE_REQUEST) - sizeof (PSR_HECI_HEADER);
  CapabilitiesBuffer.Request.Capability       = CapabilityMask;
  RecvLength                                  = sizeof (PSR_SUPPORTED_CAPABILITIES_UPDATE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_PSR_MESSAGE_ADDR,
             (UINT32 *) &CapabilitiesBuffer.Request,
             (UINT32) sizeof (PSR_SUPPORTED_CAPABILITIES_UPDATE_REQUEST),
             (UINT32 *) &CapabilitiesBuffer.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  } else if (PsrStatusSuccess != CapabilitiesBuffer.Response.Status) {
    DEBUG ((DEBUG_ERROR, "Response Status Error: %x\n", CapabilitiesBuffer.Response.Status));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Get Platform Service Record from CSME

  @param[in, out] PsrData         Pointer to PSR Data struct

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current CSME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetPsrRecord (
  IN OUT PSR_DATA   *PsrData
  )
{
  EFI_STATUS          Status;
  GET_PSR_BUFFER      PsrBuffer;
  UINT32              MeMode;
  UINT32              RecvLength;

  DEBUG ((DEBUG_INFO, "(PSR) %a() enter\n", __FUNCTION__));

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&PsrBuffer, sizeof (GET_PSR_BUFFER));
  GenerateUserNonce (NONCE_MAXSIZE, PsrBuffer.Request.UserNonce);
  PsrBuffer.Request.Header.Command    = PSR_HECI_PLATFORM_SERVICE_RECORD_GET;
  PsrBuffer.Request.Header.ByteCount  = sizeof (PSR_REQUEST) - sizeof (PSR_HECI_HEADER);
  RecvLength                          = sizeof (PSR_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_PSR_MESSAGE_ADDR,
             (UINT32 *) &PsrBuffer.Request,
             (UINT32) sizeof (PSR_REQUEST),
             (UINT32 *) &PsrBuffer.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (PsrBuffer.Response.Status != PsrStatusSuccess) {
    DEBUG ((DEBUG_ERROR, "Error, Status: %x\n", PsrBuffer.Response.Status));
    Status = EFI_DEVICE_ERROR;
  } else {
    // Copy PSR Data from PSR Response for display to user
    CopyMem (PsrData, &PsrBuffer.Response.PsrData, sizeof (PSR_DATA));
  }

  return Status;
}

/**
  PSR Report Event

  @param[in] EventId       Different event ID for each event
  @param[in] EventData     Data payload

  @retval EFI_UNSUPPORTED  Current ME mode doesn't support this function
  @retval EFI_SUCCESS      Command succeeded
**/
EFI_STATUS
HeciEventReport (
  IN UINT32 EventId,
  IN UINT32 EventData
  )
{
  EFI_STATUS                   Status;
  PSR_HECI_EVENT_REPORT_BUFFER EventReportBuf;
  UINT32                       MeMode;
  UINT32                       RecvLength;

  DEBUG ((DEBUG_INFO, "(PSR) %a() enter\n", __FUNCTION__));

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&EventReportBuf, sizeof (PSR_HECI_EVENT_REPORT_BUFFER));
  EventReportBuf.Request.Header.Command   = PSR_HECI_EVENT_REPORT;
  EventReportBuf.Request.Header.ByteCount = sizeof (PSR_HECI_EVENT_REPORT_REQUEST) - sizeof (PSR_HECI_HEADER);
  RecvLength                              = sizeof (PSR_HECI_EVENT_REPORT_RESPONSE);
  EventReportBuf.Request.Id               = EventId;
  EventReportBuf.Request.Data             = EventData;

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_PSR_MESSAGE_ADDR,
             (UINT32 *) &EventReportBuf.Request,
             (UINT32) sizeof (PSR_HECI_EVENT_REPORT_REQUEST),
             (UINT32 *) &EventReportBuf.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (EventReportBuf.Response.Status != PsrStatusSuccess) {
    DEBUG ((DEBUG_ERROR, "Error, Status: %x\n", EventReportBuf.Response.Status));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Send Chassis Intrusion Detection State to CSME

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current CSME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciSendChassisIntrDetEvent (
  VOID
  )
{
  EFI_STATUS                                Status;
  PSR_CHASSIS_INTRUSION_DET_EVENT_BUFFER    IntrDetBuffer;
  UINT32                                    MeMode;
  UINT32                                    RecvLength;

  DEBUG ((DEBUG_INFO, "(PSR) %a() enter\n", __FUNCTION__));

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&IntrDetBuffer, sizeof (PSR_CHASSIS_INTRUSION_DET_EVENT_BUFFER));
  IntrDetBuffer.Request.Header.Command   = PSR_HECI_CHASSIS_INTRUSION_REPORT;
  IntrDetBuffer.Request.Header.ByteCount = sizeof (PSR_CHASSIS_INTRUSION_DET_EVENT_REQUEST) - sizeof (PSR_HECI_HEADER);
  RecvLength                             = sizeof (PSR_CHASSIS_INTRUSION_DET_EVENT_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_PSR_MESSAGE_ADDR,
             (UINT32 *) &IntrDetBuffer.Request,
             (UINT32) sizeof (PSR_CHASSIS_INTRUSION_DET_EVENT_REQUEST),
             (UINT32 *) &IntrDetBuffer.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IntrDetBuffer.Response.Status != PsrStatusSuccess) {
    DEBUG ((DEBUG_ERROR, "Error, Status: %x\n", IntrDetBuffer.Response.Status));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Report Critical Temperature Shutdown to CSME

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current CSME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciSendCriticalTempShutdownEvent (
  VOID
  )
{
  EFI_STATUS                          Status;
  PSR_CRITICAL_TEMP_SHUTDOWN_BUFFER   CriticalTempShutdownBuffer;
  UINT32                              MeMode;
  UINT32                              RecvLength;

  DEBUG ((DEBUG_INFO, "(PSR) %a() enter\n", __FUNCTION__));

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&CriticalTempShutdownBuffer, sizeof (PSR_CRITICAL_TEMP_SHUTDOWN_BUFFER));
  CriticalTempShutdownBuffer.Request.Header.Command   = PSR_HECI_EXCESSIVE_TEMPERATURE_REPORT;
  CriticalTempShutdownBuffer.Request.Header.ByteCount = sizeof (PSR_CRITICAL_TEMPERATURE_REPORT_REQUEST) - sizeof (PSR_HECI_HEADER);
  RecvLength                                          = sizeof (PSR_CRITICAL_TEMPERATURE_REPORT_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_PSR_MESSAGE_ADDR,
             (UINT32 *) &CriticalTempShutdownBuffer.Request,
             (UINT32) sizeof (PSR_CRITICAL_TEMPERATURE_REPORT_REQUEST),
             (UINT32 *) &CriticalTempShutdownBuffer.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  } else if (PsrStatusSuccess != CriticalTempShutdownBuffer.Response.Status) {
    DEBUG ((DEBUG_ERROR, "Response Status Error: %x\n", CriticalTempShutdownBuffer.Response.Status));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Report Device Erase Event

  @param[in] DeviceEraseReport    Device Erase Report Data

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
**/
EFI_STATUS
HeciDeviceEraseReport (
  IN PSR_DEVICE_ERASE_REPORT      *DeviceEraseReport
  )
{
  EFI_STATUS                            Status;
  PSR_HECI_DEVICE_ERASE_REPORT_BUFFER   EraseReportBuf;
  UINT32                                MeMode;
  UINT32                                RecvLength;

  DEBUG ((DEBUG_INFO, "(PSR) %a() enter \n", __FUNCTION__));

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&EraseReportBuf, sizeof (PSR_HECI_DEVICE_ERASE_REPORT_BUFFER));
  EraseReportBuf.Request.Header.Command   = PSR_HECI_DEVICE_ERASE_REPORT;
  EraseReportBuf.Request.Header.ByteCount = sizeof (PSR_HECI_DEVICE_ERASE_REPORT_REQUEST) - sizeof (PSR_HECI_HEADER);
  RecvLength                              = sizeof (PSR_HECI_DEVICE_ERASE_REPORT_RESPONSE);
  CopyMem (&EraseReportBuf.Request.EraseReport, DeviceEraseReport, sizeof (PSR_DEVICE_ERASE_REPORT));

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_PSR_MESSAGE_ADDR,
             (UINT32 *) &EraseReportBuf.Request,
             (UINT32) sizeof (PSR_HECI_DEVICE_ERASE_REPORT_REQUEST),
             (UINT32 *) &EraseReportBuf.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (EraseReportBuf.Response.Status != PsrStatusSuccess) {
    DEBUG ((DEBUG_ERROR, "%a: Response Status Failed\n", __FUNCTION__));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

//
// ME Client - MKHI
//

//
// MKHI_CBM_GROUP_ID = 0x00
//

/**
  Send Core BIOS Reset Request Message through HECI to reset the system.

  @param[in] ResetOrigin          Reset source
  @param[in] ResetType            Global or Host reset

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciSendCbmResetRequest (
  IN  UINT8                      ResetOrigin,
  IN  UINT8                      ResetType
  )
{
  EFI_STATUS                  Status;
  PLATFORM_ME_HOOK_PROTOCOL   *PlatformMeHook;
  CBM_RESET_BUFFER            CbmResetRequest;
  WDT_PROTOCOL                *WdtProtocol;
  UINT32                      MeMode;
  UINT32                      RecvLength;
  UINT32                      Result;
  CHAR8                       *ResetString;

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

  DEBUG ((DEBUG_INFO, "ME-BIOS: %a Reset Request Entry.\n", ResetString));

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_RESETREQ_ENTRY_START); // PostCode (0xE06)

  if (MeIsAfterEndOfPost ()) {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: %a Reset Request Exit - Error by message is not allowed.\n", ResetString));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_MSGNA); // PostCode (0xE86)
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (
                  &gPlatformMeHookProtocolGuid,
                  NULL,
                  (VOID **) &PlatformMeHook
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Unable to Locate PlatformMeHook Protocol for %a Reset Hook, so skip instead.- %r\n", ResetString, Status));
  } else {
    PlatformMeHook->PreGlobalReset ();
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) ||
      ((MeMode != ME_MODE_NORMAL) && (MeMode != ME_MODE_TEMP_DISABLED))) {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: %a Reset Request Exit - Error by message is not allowed.\n", ResetString));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_MSGNA); // PostCode (0xE86)
    return EFI_UNSUPPORTED;
  }

  CbmResetRequest.Request.MkhiHeader.Data           = 0;
  CbmResetRequest.Request.MkhiHeader.Fields.GroupId = MKHI_CBM_GROUP_ID;
  CbmResetRequest.Request.MkhiHeader.Fields.Command = CBM_RESET_CMD;
  CbmResetRequest.Request.Data.RequestOrigin        = ResetOrigin;
  CbmResetRequest.Request.Data.ResetType            = ResetType;
  RecvLength                                        = sizeof (CBM_RESET_RESPONSE);

  Status = gBS->LocateProtocol (&gWdtProtocolGuid, NULL, (VOID **) &WdtProtocol);
  if (!EFI_ERROR (Status)) {
    WdtProtocol->AllowKnownReset ();
  } else {
    DEBUG ((DEBUG_WARN, "Could not locate WDT protocol!\n"));
  }

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &CbmResetRequest.Request,
             (UINT32) sizeof (CBM_RESET_REQUEST),
             (UINT32 *) &CbmResetRequest.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    if (Status == EFI_NOT_FOUND) {
      DEBUG ((DEBUG_ERROR, "ME-BIOS: %a Reset Request Exit - Error by HECI service not found.\n", ResetString));
      REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_NOHECI); // PostCode (0xEA6)
    } else {
      DEBUG ((DEBUG_ERROR, "ME-BIOS: %a Reset Request Exit - Error by message sent - %r.\n", ResetString, Status));
      REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_MSG_SENTFAIL); // PostCode (0xEC6)
    }
    return Status;
  }

  Result = CbmResetRequest.Response.MkhiHeader.Fields.Result;
  if (Result != 0) {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: %a Reset Request Exit - Error by message ack error. Result: %x\n", ResetString, Result));
    Status = EFI_DEVICE_ERROR;
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_MSG_ACKERR); // PostCode (0xEE6)
  } else {
    DEBUG ((DEBUG_INFO, "ME-BIOS: %a Reset Request Exit - Success.\n", ResetString));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_SUCCESS); // PostCode (0xE26)
  }

  return Status;
}

/**
  Retrieve invocation codes from Firmware.

  @param[out] InvocationCode        Bitmask of invocation codes set by Firmware.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetInvocationCode (
  OUT UINT32 *InvocationCode
  )
{
  EFI_STATUS                    Status;
  GET_INVOCATION_CODE_BUFFER    GetInvocationCode;
  UINT32                        RecvLength;

  GetInvocationCode.Request.MkhiHeader.Data           = 0;
  GetInvocationCode.Request.MkhiHeader.Fields.GroupId = MKHI_CBM_GROUP_ID;
  GetInvocationCode.Request.MkhiHeader.Fields.Command = CBM_GET_INVOCATION_CODE;
  RecvLength                                          = sizeof (GET_INVOCATION_CODE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetInvocationCode.Request,
             (UINT32) sizeof (GET_INVOCATION_CODE_REQUEST),
             (UINT32 *) &GetInvocationCode.Response,
             &RecvLength
             );

  if (HeciMkhiError (Status, GetInvocationCode.Response.MkhiHeader, CBM_GET_INVOCATION_CODE, TRUE)) {
    return EFI_DEVICE_ERROR;
  }

  *InvocationCode = GetInvocationCode.Response.InvocationCode;

  return Status;
}

/**
  Set invocation codes in Firmware.

  @param[in] InvocationCode         Bitmask of invocation codes to be set by Firmware.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetInvocationCode (
  IN UINT32 InvocationCode
  )
{
  EFI_STATUS                    Status;
  SET_INVOCATION_CODE_BUFFER    SetInvocationCode;
  UINT32                        RecvLength;

  SetInvocationCode.Request.MkhiHeader.Data           = 0;
  SetInvocationCode.Request.MkhiHeader.Fields.GroupId = MKHI_CBM_GROUP_ID;
  SetInvocationCode.Request.MkhiHeader.Fields.Command = CBM_SET_INVOCATION_CODE;
  SetInvocationCode.Request.InvocationCode            = InvocationCode;
  RecvLength                                          = sizeof (SET_INVOCATION_CODE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &SetInvocationCode.Request,
             (UINT32) sizeof (SET_INVOCATION_CODE_REQUEST),
             (UINT32 *) &SetInvocationCode.Response,
             &RecvLength
             );

  if (HeciMkhiError (Status, SetInvocationCode.Response.MkhiHeader, CBM_SET_INVOCATION_CODE, TRUE)) {
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Clear invocation codes in Firmware.

  @param[in] InvocationCode         Bitmask of invocation codes to be cleared by Firmware.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciClearInvocationCode (
  IN UINT32 InvocationCode
  )
{
  EFI_STATUS                      Status;
  CLEAR_INVOCATION_CODE_BUFFER    ClearInvocationCode;
  UINT32                          RecvLength;

  ClearInvocationCode.Request.MkhiHeader.Data           = 0;
  ClearInvocationCode.Request.MkhiHeader.Fields.GroupId = MKHI_CBM_GROUP_ID;
  ClearInvocationCode.Request.MkhiHeader.Fields.Command = CBM_CLR_INVOCATION_CODE;
  ClearInvocationCode.Request.InvocationCode            = InvocationCode;
  RecvLength                                            = sizeof (CLEAR_INVOCATION_CODE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &ClearInvocationCode.Request,
             (UINT32) sizeof (CLEAR_INVOCATION_CODE_REQUEST),
             (UINT32 *) &ClearInvocationCode.Response,
             &RecvLength
             );

  if (HeciMkhiError (Status, ClearInvocationCode.Response.MkhiHeader, CBM_CLR_INVOCATION_CODE, TRUE)) {
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

//
// MKHI_PWD_GROUP_ID = 0x02
//

/**
  This function logs in with current password and updates to new password.

  @param[in] OldPwdSize             Size of the old password
  @param[in] OldPwd                 Buffer holding old password
  @param[in] NewPwdSize             Size of the new password
  @param[in] NewPwd                 Buffer holding new password

  @retval ME_PWD_STATUS             Password management Status Code:
                                      @see ME_PWD_STATUS
**/
ME_PWD_STATUS
HeciModifyMebxPassword (
  IN UINT8    OldPwdSize,
  IN UINT8    *OldPwd,
  IN UINT8    NewPwdSize,
  IN UINT8    *NewPwd
  )
{
  EFI_STATUS          Status;
  PWD_CHANGE_BUFFER   ChangePassword;
  ME_PWD_STATUS       Result;
  UINT32              RecvLength;

  if (OldPwd == NULL ||
      NewPwd == NULL ||
      OldPwdSize > MEPWDMGMT_PWD_MAX_SIZE ||
      NewPwdSize > MEPWDMGMT_PWD_MAX_SIZE) {
    return ME_PWDSTS_FAILURE;
  }

  ZeroMem (&ChangePassword, sizeof (PWD_CHANGE_BUFFER));
  ChangePassword.Request.MkhiHeader.Fields.Command = PWD_CHANGE_CMD;
  ChangePassword.Request.MkhiHeader.Fields.GroupId = MKHI_PWD_GROUP_ID;
  ChangePassword.Request.PwdId                     = MEPWDID_ME_BIOS_EXT;
  ChangePassword.Request.OldPwdSize                = OldPwdSize;
  ChangePassword.Request.NewPwdSize                = NewPwdSize;
  RecvLength                                       = sizeof (PWD_CHANGE_RESPONSE);
  CopyMem (ChangePassword.Request.OldPwd, OldPwd, OldPwdSize);
  CopyMem (ChangePassword.Request.NewPwd, NewPwd, NewPwdSize);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &ChangePassword.Request,
             (UINT32) sizeof (PWD_CHANGE_REQUEST),
             (UINT32 *) &ChangePassword.Response,
             &RecvLength
             );

  Result = ChangePassword.Response.MkhiHeader.Fields.Result;
  if (HeciMkhiError (Status, ChangePassword.Response.MkhiHeader, PWD_CHANGE_CMD, FALSE)) {
    Result = ME_PWDSTS_FAILURE;
  }

  // Clear the buffer to clear password - Security concern
  ZeroMem (&ChangePassword, sizeof (PWD_CHANGE_BUFFER));

  return Result;
}

/**
  This function uses the User entered password to login into FW.

  @param[in]  PasswordSize          Size of the user entered password
  @param[in]  Password              Buffer holding user enetered password
  @param[out] MaxRetries            Informs if maximum number of login retries have been reached

  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_INVALID_PARAMETER     Null buffer
  @retval EFI_DEVICE_ERROR          Command failed
**/
EFI_STATUS
HeciValidateMebxPassword (
  IN     UINT8      PasswordSize,
  IN     UINT8      *Password,
     OUT BOOLEAN    *MaxRetries
  )
{
  EFI_STATUS            Status;
  PWD_VALIDATE_BUFFER   ValidatePassword;
  UINT32                RecvLength;

  if (Password == NULL || MaxRetries == NULL || PasswordSize > MEPWDMGMT_PWD_MAX_SIZE) {
    return EFI_INVALID_PARAMETER;
  }

  *MaxRetries = FALSE;

  ZeroMem (&ValidatePassword, sizeof (PWD_VALIDATE_BUFFER));
  ValidatePassword.Request.MkhiHeader.Fields.Command = PWD_VALIDATE_CMD;
  ValidatePassword.Request.MkhiHeader.Fields.GroupId = MKHI_PWD_GROUP_ID;
  ValidatePassword.Request.PwdId                     = MEPWDID_ME_BIOS_EXT;
  ValidatePassword.Request.PwdSize                   = (UINT8) PasswordSize;
  RecvLength                                         = sizeof (PWD_VALIDATE_RESPONSE);
  CopyMem (ValidatePassword.Request.Pwd, Password, PasswordSize);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &ValidatePassword.Request,
             (UINT32) sizeof (PWD_VALIDATE_REQUEST),
             (UINT32 *) &ValidatePassword.Response,
             &RecvLength
             );

  if (HeciMkhiError (Status, ValidatePassword.Response.MkhiHeader, PWD_VALIDATE_CMD, FALSE)) {
    Status = EFI_DEVICE_ERROR;
    goto MebxLoginExit;
  }

  if (ValidatePassword.Response.MaxCountExpired == TRUE) {
    *MaxRetries = TRUE;
    Status = EFI_DEVICE_ERROR;
    goto MebxLoginExit;
  }

  if (ValidatePassword.Response.MkhiHeader.Fields.Result != ME_PWDSTS_PWD_SUCCESS) {
    Status = EFI_DEVICE_ERROR;
  }

MebxLoginExit:
  // Clear the buffer to clear password - Security concern
  ZeroMem (&ValidatePassword, sizeof (PWD_VALIDATE_BUFFER));
  return Status;
}

/**
  Checks if password was modified.

  @param[out] Modified              Password modification status

  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_INVALID_PARAMETER     Parameter invalid
**/
EFI_STATUS
HeciIsMebxPasswordModified (
  OUT UINT32  *Modified
  )
{
  EFI_STATUS                Status;
  PWD_IS_MODIFIED_BUFFER    IsPasswordModified;
  UINT32                    RecvLength;

  if (Modified == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  IsPasswordModified.Request.MkhiHeader.Data           = 0;
  IsPasswordModified.Request.MkhiHeader.Fields.Command = PWD_IS_MODIFIED_CMD;
  IsPasswordModified.Request.MkhiHeader.Fields.GroupId = MKHI_PWD_GROUP_ID;
  RecvLength                                           = sizeof (PWD_IS_MODIFIED_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &IsPasswordModified.Request,
             (UINT32) sizeof (PWD_IS_MODIFIED_REQUEST),
             (UINT32 *) &IsPasswordModified.Response,
             &RecvLength
             );

  if (HeciMkhiError (Status, IsPasswordModified.Response.MkhiHeader, PWD_IS_MODIFIED_CMD, TRUE)) {
    return EFI_DEVICE_ERROR;
  }

  *Modified = IsPasswordModified.Response.IsModified;

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
HeciFwCapsGetRuleData (
  IN  UINT32                      RuleId,
  OUT UINT32                      *RuleData
  )
{
  EFI_STATUS          Status;
  GET_RULE_BUFFER     GetRuleMsg;
  UINT32              MeMode;
  UINT32              RecvLength;

  if (RuleData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&GetRuleMsg, sizeof (GET_RULE_BUFFER));

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  GetRuleMsg.Request.MkhiHeader.Data           = 0;
  GetRuleMsg.Request.MkhiHeader.Fields.GroupId = MKHI_FWCAPS_GROUP_ID;
  GetRuleMsg.Request.MkhiHeader.Fields.Command = FWCAPS_GET_RULE_CMD;
  GetRuleMsg.Request.RuleId                    = RuleId;
  RecvLength                                   = sizeof (GET_RULE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetRuleMsg.Request,
             (UINT32) sizeof (GET_RULE_REQUEST),
             (UINT32 *) &GetRuleMsg.Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status) && ((GetRuleMsg.Response.MkhiHeader.Fields.Command) == FWCAPS_GET_RULE_CMD) &&
      ((GetRuleMsg.Response.MkhiHeader.Fields.IsResponse) == 1) &&
      (GetRuleMsg.Response.MkhiHeader.Fields.Result == 0)) {
    *RuleData = GetRuleMsg.Response.RuleData;
  }

  return Status;
}

/**
  Send Get Firmware Capabilities Request to CSME

  @param[out] *FwCapsSku            Firmware Capabilities

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetFwCapsSkuMsg (
  OUT MEFWCAPS_SKU         *RuleData
  )
{
  return HeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, FwCapsRuleTypeId), (UINT32 *) RuleData);
}

/**
  Send Get Platform State Request to CSME
  @param[out] RuleData              Platform State
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetPlatformStateMsg (
  OUT UINT32                      *RuleData
  )
{
  return HeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, MePlatformStateRuleTypeId), (UINT32 *) RuleData);
}

/**
  This message is sent by the BIOS prior to the End of Post (EOP) on the boot
  where host wants to query the local firmware update interface status.

  @param[out] RuleData            2 - Partial and full firmware update is disabled
                                  1 - Firmware update interface enable
                                  0 - Firmware update interface disable

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetFwUpdateState (
  OUT UINT32               *RuleData
  )
{
  return HeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, MeFwUpdateRuleTypeId), (UINT32 *) RuleData);
}


/**
  Send Get User Capabilities State Request to ME.

  @param[out] RuleData           User Capabilities State

  @retval EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_DEVICE_ERROR       HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT            HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL   Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetUserCapabilitiesState (
  OUT MEFWCAPS_SKU *RuleData
  )
{
  return HeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, UserCapsRuleTypeId), (UINT32 *) RuleData);
}

/**
  This message is sent by the BIOS or IntelR MEBX prior to the End of Post (EOP)
  on the boot where host wants to get Ibex Peak platform type.
  One of usages is to utilize this command to determine if the platform runs in
  Consumer or Corporate SKU size firmware.

  @param[out] RuleData             PlatformBrand,
                                   IntelMeFwImageType,
                                   SuperSku,
                                   PlatformTargetUsageType

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetPlatformTypeMsg (
  OUT PLATFORM_TYPE_RULE_DATA     *RuleData
  )
{
  return HeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, PlatTypeRuleTypeId), (UINT32 *) RuleData);
}

/**
  The Get FW Feature Status message is based on MKHI interface.
  This command is used by BIOS/IntelR MEBX to get firmware runtime status.
  The GET FW RUNTIME STATUS message doesn't need to check the HFS.FWInitComplete
  value before sending the command.
  It means this message can be sent regardless of HFS.FWInitComplete.

  @param[out] RuleData              MEFWCAPS_SKU message

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetFwFeatureStateMsg (
  OUT MEFWCAPS_SKU                *RuleData
  )
{
  return HeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, FeatureStateRuleTypeId), (UINT32 *) RuleData);
}

/**
  Get available ME features.

  @param[out] FwFeatures            Mask of available ME Features

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetFwAvailableFeatures (
  OUT UINT32 *FwFeatures
  )
{
  return HeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, AvailableBitmapRuleTypeId), FwFeatures);
}

/**
  The firmware will respond to GET OEM TAG message even after the End of Post (EOP).

  @param[in] RuleData             Default is zero. Tool can create the OEM specific OEM TAG data.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetOemTagMsg (
  OUT UINT32                      *RuleData
  )
{
  return HeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, OemTagRuleTypeId), RuleData);
}

/**
  This message is sent by the BIOS when it wants to query
  ME Unconfig on RTC Clear Disable state.

  @param[out] RuleData            1 - Unconfig on RTC clear is disabled
                                  0 - Unconfig on RTC clear is not disabled

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetUnconfigOnRtcClearDisableMsg (
  OUT UINT32               *RuleData
  )
{
  return HeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, UnconfigOnRtcClarRuleTypeId), RuleData);
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
HeciGetAmtBiosSyncInfo (
  OUT AMT_BIOS_SYNC_INFO *RuleData
  )
{
  return HeciFwCapsGetRuleData (FWCAPS_RULE_ID (AmtFeatureId, AmtBiosSyncInfoRuleTypeId), (UINT32 *) RuleData);
}

/**
  Get User Consent requirement.

  @param[out] IsUserConsentRequired     User consent requirement:
                                          TRUE  - User Consent required
                                          FALSE - User Consent not required

  @retval EFI_UNSUPPORTED               Current ME mode doesn't support this function
  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_DEVICE_ERROR              HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                   HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL          Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetIsUserConsentRequired (
  OUT BOOLEAN *IsUserConsentRequired
  )
{
  EFI_STATUS    Status;
  UINT32        UserConsentRequiredData;

  UserConsentRequiredData = 0;
  *IsUserConsentRequired  = FALSE;

  // Get User Consent state from AMT
  Status = HeciFwCapsGetRuleData (FWCAPS_RULE_ID (AmtFeatureId, DisplayUserConsentRuleTypeId), &UserConsentRequiredData);

  if (UserConsentRequiredData == 1) {
    *IsUserConsentRequired = TRUE;
  }

  return Status;
}

/**
  Return information if Client Initiated Remote Access (CIRA) is available.

  @retval TRUE    CIRA is available
  @retval FALSE   CIRA isn't available or cannot get infomation
**/
BOOLEAN
IsCiraAvailable (
  VOID
  )
{
  EFI_STATUS            Status;
  AMT_BIOS_SYNC_INFO    CckCiraData;

  ZeroMem (&CckCiraData, sizeof (AMT_BIOS_SYNC_INFO));

  Status = HeciGetAmtBiosSyncInfo (&CckCiraData);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return (BOOLEAN) CckCiraData.CiraAvailable;
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
HeciFwCapsSetRuleData (
  IN  UINT32                      RuleId,
  IN  UINT8                       RuleDataLength,
  IN  UINT32                      RuleData
  )
{
  EFI_STATUS          Status;
  SET_RULE_BUFFER     SetRuleMsg;
  UINT32              MeMode;
  UINT32              RecvLength;

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
  RecvLength                                   = sizeof (SET_RULE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &SetRuleMsg.Request,
             (UINT32) sizeof (SET_RULE_REQUEST),
             (UINT32 *) &SetRuleMsg.Response,
             &RecvLength
             );

  return Status;
}

/**
  This message is sent by the BIOS or IntelR MEBX prior to the End of Post (EOP)
  on the boot where host wants to disable the ME State. The firmware allows a single
  update once it receives the disable command Once firmware receives this message,
  the firmware will work in "Soft Temporary Disable" mode (HFS[19:16] = 3) after a
  global reset. Note, this message is not allowed when AT firmware is enrolled/configured.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetMeDisableMsg (
  VOID
  )
{
  EFI_STATUS          Status;
  UINT32              MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (MeMode == ME_MODE_TEMP_DISABLED) {
    return EFI_SUCCESS;
  }

  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  Status = HeciFwCapsSetRuleData (FWCAPS_RULE_ID (KernelFeatureId, MePlatformStateRuleTypeId), RULE_DATA_LENGTH, ME_DISABLE);

  return Status;
}

/**
  This message is sent by the BIOS or IntelR MEBX prior to the End of Post (EOP) on the boot
  where host wants to enable or disable the local firmware update interface.
  The firmware allows a single update once it receives the enable command

  @param[in] RuleData             1 - local firmware update interface enable
                                  0 - local firmware update interface disable

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetFwUpdateState (
  IN UINT8                 RuleData
  )
{
  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  return HeciFwCapsSetRuleData (FWCAPS_RULE_ID (KernelFeatureId, MeFwUpdateRuleTypeId), RULE_DATA_LENGTH, (UINT32) RuleData);
}

/**
  Send Set User Capabilities State Request to ME.

  @param[in] MEFWCAPS_SKU        User Capabilities State

  @retval EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_DEVICE_ERROR       HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT            HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL   Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetUserCapabilitiesState (
  IN MEFWCAPS_SKU RuleData
  )
{
  return HeciFwCapsSetRuleData (FWCAPS_RULE_ID (KernelFeatureId, UserCapsRuleTypeId), RULE_DATA_LENGTH, RuleData.Data);
}

/**
  Set FW Enabled Features.

  @param[in] NewFeatures          New Fw Enabled Features
  @param[in] FeaturesMask         Bitmask with new Fw Enabled Features

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetFwEnabledFeatures (
  IN UINT32 NewFeatures,
  IN UINT32 FeaturesMask
  )
{
  EFI_STATUS    Status;
  UINT32        CurrentFeatures;

  CurrentFeatures = 0;

  Status = HeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, StateForAllFeaturesRuleTypeId), &CurrentFeatures);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CurrentFeatures &= ~FeaturesMask;
  CurrentFeatures |= (NewFeatures & FeaturesMask);

  Status = HeciFwCapsSetRuleData (FWCAPS_RULE_ID (KernelFeatureId, StateForAllFeaturesRuleTypeId), RULE_DATA_LENGTH, CurrentFeatures);

  return Status;
}

/**
  This message is sent by the BIOS to inform ME FW whether or not to take the
  TPM 1.2 Deactivate flow

  @param[in] UINT8 TpmDeactivate  0 - ME FW should not take the
                                      deactivate flow.
                                  1 - ME FW should take the deactivate
                                      flow.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetAcmTpmData (
  IN UINT8                 TpmDeactivate
  )
{
  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  return HeciFwCapsSetRuleData (FWCAPS_RULE_ID (KernelFeatureId, AcmTpmRuleTypeId), RULE_DATA_LENGTH, (UINT32) TpmDeactivate);
}

/**
  This message is sent by the BIOS when it wants to set
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
HeciSetUnconfigOnRtcClearDisableMsg (
  IN UINT32                RuleData
  )
{
  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  return HeciFwCapsSetRuleData (FWCAPS_RULE_ID (KernelFeatureId, UnconfigOnRtcClarRuleTypeId), RULE_DATA_LENGTH, RuleData);
}

//
// MKHI_HMRFPO_GROUP_ID = 0x05
//

/**
  Sends a message to ME to unlock a specified SPI Flash region for writing and receiving a response message.
  It is recommended that HMRFPO_ENABLE MEI message needs to be sent after all OROMs finish their initialization.

  @param[out] Result              HMRFPO_ENABLE response

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciHmrfpoEnable (
  OUT UINT8                       *Result
  )
{
  EFI_STATUS              Status;
  HMRFPO_ENABLE_BUFFER    HmrfpoEnable;
  UINT32                  MeMode;
  UINT32                  RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&HmrfpoEnable, sizeof (HMRFPO_ENABLE_BUFFER));
  HmrfpoEnable.Request.MkhiHeader.Data           = 0;
  HmrfpoEnable.Request.MkhiHeader.Fields.GroupId = MKHI_HMRFPO_GROUP_ID;
  HmrfpoEnable.Request.MkhiHeader.Fields.Command = HMRFPO_ENABLE_CMD_ID;
  RecvLength                                     = sizeof (HMRFPO_ENABLE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &HmrfpoEnable.Request,
             (UINT32) sizeof (HMRFPO_ENABLE_REQUEST),
             (UINT32 *) &HmrfpoEnable.Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status) && (HmrfpoEnable.Response.MkhiHeader.Fields.Result == 0)) {
    *Result = HmrfpoEnable.Response.Status;
  }

  DEBUG ((DEBUG_INFO, "Send HMRFPO_ENABLE_CMD_ID Result - %r\n", Status));
  return Status;
}

/**
  Sends a message to ME to lock a specified SPI Flash region for writing and receiving a response message.

  @param[out] Result              Status report

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_NOT_READY           HECI Device hasn't ready yet
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciHmrfpoLock (
  OUT UINT8                       *Result
  )
{
  EFI_STATUS            Status;
  HMRFPO_LOCK_BUFFER    HmrfpoLock;
  UINT32                MeMode;
  UINT32                RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&HmrfpoLock, sizeof (HMRFPO_LOCK_BUFFER));
  HmrfpoLock.Request.MkhiHeader.Data           = 0;
  HmrfpoLock.Request.MkhiHeader.Fields.GroupId = MKHI_HMRFPO_GROUP_ID;
  HmrfpoLock.Request.MkhiHeader.Fields.Command = HMRFPO_LOCK_CMD_ID;
  RecvLength                                   = sizeof (HMRFPO_LOCK_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &HmrfpoLock.Request,
             (UINT32) sizeof (HMRFPO_LOCK_REQUEST),
             (UINT32 *) &HmrfpoLock.Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status) && (HmrfpoLock.Response.MkhiHeader.Fields.Result == 0)) {
    *Result = HmrfpoLock.Response.Status;
  }

  DEBUG ((DEBUG_INFO, "Send HMRFPO_LOCK_CMD_ID Result - %r\n", Status));
  return Status;
}

/**
  System BIOS sends this message to get status for HMRFPO_LOCK message.

  @param[out] Result              HMRFPO_GET_STATUS response

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciHmrfpoGetStatus (
  OUT UINT8                       *Result
  )
{
  EFI_STATUS                  Status;
  HMRFPO_GET_STATUS_BUFFER    HmrfpoGetStatus;
  UINT32                      MeMode;
  UINT32                      RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&HmrfpoGetStatus, sizeof (HMRFPO_GET_STATUS_BUFFER));
  HmrfpoGetStatus.Request.MkhiHeader.Data           = 0;
  HmrfpoGetStatus.Request.MkhiHeader.Fields.GroupId = MKHI_HMRFPO_GROUP_ID;
  HmrfpoGetStatus.Request.MkhiHeader.Fields.Command = HMRFPO_GET_STATUS_CMD_ID;
  RecvLength                                        = sizeof (HMRFPO_GET_STATUS_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &HmrfpoGetStatus.Request,
             (UINT32) sizeof (HMRFPO_GET_STATUS_REQUEST),
             (UINT32 *) &HmrfpoGetStatus.Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status) && (HmrfpoGetStatus.Response.MkhiHeader.Fields.Result == 0)) {
    *Result = HmrfpoGetStatus.Response.Status;
  }

  DEBUG ((DEBUG_INFO, "Send HMRFPO_GET_STATUS_CMD_ID Result - %r\n", Status));
  return Status;
}

//
// MKHI_COMMON_GROUP_ID = 0xF0
//

/**
  This message is sent by the BIOS or IntelR MEBX prior to the End of Post (EOP)
  on the boot where host wants to enable the ME State. The firmware allows a single
  update once it receives the enable command. Once firmware receives this message,
  the firmware will be in normal mode after a global reset.

  @retval EFI_SUCCESS             ME enabled message sent
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout or transition failed
                                  within allowed time limit
**/
EFI_STATUS
HeciSetMeEnableMsg (
  VOID
  )
{
  EFI_STATUS              Status;
  HECI_FWS_REGISTER       MeFirmwareStatus;
  ME_ENABLE_CMD_BUFFER    MeEnableReq;
  UINT64                  HeciBaseAddress;
  UINTN                   Timeout;
  UINT32                  MeMode;
  UINT32                  RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (MeMode == ME_MODE_NORMAL) {
    return EFI_SUCCESS;
  }

  if (EFI_ERROR (Status) || (MeMode != ME_MODE_TEMP_DISABLED)) {
    return EFI_UNSUPPORTED;
  }

  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  MeEnableReq.Request.MkhiHeader.Data           = 0;
  MeEnableReq.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  MeEnableReq.Request.MkhiHeader.Fields.Command = ME_ENABLE_CMD;
  RecvLength                                    = sizeof (ME_ENABLE_CMD_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &MeEnableReq.Request,
             (UINT32) sizeof (ME_ENABLE_CMD_REQUEST),
             (UINT32 *) &MeEnableReq.Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status) && (MeEnableReq.Response.MkhiHeader.Fields.Result != 0)) {
    //
    // Command succeeded but ME was not in disabled state
    //
    Status = EFI_UNSUPPORTED;
  }

  Timeout = 0;
  HeciBaseAddress = MeHeciPciCfgBase (HECI1);
  while (TRUE) {
    ///
    /// Read ME status and check for Current State
    ///
    MeFirmwareStatus.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS);
    if (MeFirmwareStatus.r.CurrentState == ME_STATE_NORMAL) {
      break;
    }

    MicroSecondDelay (ME_STATE_STALL_1_SECOND);
    Timeout++;

    if (Timeout >= MSG_MAX_WAIT_TIMEOUT) {
      DEBUG ((DEBUG_ERROR, "Timeout occurred, MeFirmwareStatus: %08x.\n", MeFirmwareStatus.ul));
      return EFI_TIMEOUT;
    }
  }

  return Status;
}

/**
  Send HMRFPO disable MKHI to CSME.

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciHmrfpoDisable (
  VOID
  )
{
  EFI_STATUS                  Status;
  HMRFPO_DISABLE_CMD_BUFFER   HmrfpoDisable;
  UINT32                      RecvLength;
  UINT32                      MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_SECOVER)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&HmrfpoDisable, sizeof (HMRFPO_DISABLE_CMD_BUFFER));
  HmrfpoDisable.Request.MkhiHeader.Data           = 0;
  HmrfpoDisable.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  HmrfpoDisable.Request.MkhiHeader.Fields.Command = HMRFPO_DISABLE_CMD;
  RecvLength                                      = sizeof (HMRFPO_DISABLE_CMD_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &HmrfpoDisable.Request,
             (UINT32) sizeof (HMRFPO_DISABLE_CMD_REQUEST),
             (UINT32 *) &HmrfpoDisable.Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status) && (HmrfpoDisable.Response.MkhiHeader.Fields.Result != 0)) {
    //
    // Command succeeded but ME is in wrong state to process it
    //
    DEBUG ((DEBUG_ERROR, "HMRFPO_DISABLE ACK returns unsuccessful result - %r\n", Status));
    Status = EFI_UNSUPPORTED;
  }

  return Status;
}

/**
  Send command to terminate HECI communication in ME Debug Mode.

  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciTerminateHeciMsg (
  VOID
  )
{
  EFI_STATUS                    Status;
  TERMINATE_HECI_CMD_REQUEST    TerminateHeci;
  UINT32                        MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  //
  // CSME supports Terminate HECI command only in Debug mode
  //
  if (MeMode != ME_MODE_DEBUG) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&TerminateHeci, sizeof (TERMINATE_HECI_CMD_REQUEST));
  TerminateHeci.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  TerminateHeci.MkhiHeader.Fields.Command = TERMINATE_HECI_CMD;

  Status = HeciWrapperSend (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &TerminateHeci,
             (UINT32) sizeof (TERMINATE_HECI_CMD_REQUEST)
             );

  return Status;
}

/**
  This function sends a command to CSME to get Log information of Measurements provided in
  CSME Extend Registers for BIOS to extend to TPM-PCR registers.

  @param[out]     EventLogBuffer    Pointer to received Event log buffer
  @param[in]      RemainingBufSize  Size of available buffer to collect event log
  @param[out]     ReceivedLogSize   Size of received event log
  @param[in, out] LogEntryIndex     On input - Index of first / starting log entry in the buffer. On output - Index of last log entry returned
  @param[out]     RemainingEntries  Pointer to Remaining Entries (0 - no more entries in a log; 1 - additional iteration(s) is(are) required)

  @retval EFI_SUCCESS             Log received successfully
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate required resources
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout or transition failed
                                  within allowed time limit
**/
EFI_STATUS
HeciGetErLog (
  OUT    UINT8     *EventLogBuffer,
  IN     UINT32    RemainingBufSize,
  IN OUT UINT32    *ReceivedLogSize,
  IN OUT UINT8     *LogEntryIndex,
  OUT    UINT8     *RemainingEntries
  )
{
  EFI_STATUS          Status;
  GET_ER_LOG_BUFFER   *GetErLog;
  UINT32              RecvLength;
  UINT32              MeMode;

  GetErLog = NULL;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  RecvLength = sizeof (GET_ER_LOG_CMD_RESPONSE) + RemainingBufSize; // Size of response structure + available buffer size

  GetErLog = AllocateZeroPool (RecvLength);
  if (GetErLog == NULL) {
    DEBUG ((DEBUG_ERROR, "%a Error: Could not allocate Memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  GetErLog->Request.FirstLogEntryIndex = *LogEntryIndex;
  GetErLog->Request.MkhiHeader.Data = 0;
  GetErLog->Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  GetErLog->Request.MkhiHeader.Fields.Command = GET_ER_LOG_CMD;
  GetErLog->Request.Flags = 0;

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetErLog->Request,
             (UINT32) sizeof (GET_ER_LOG_CMD_REQUEST),
             (UINT32 *) &GetErLog->Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status)) {
    if ((GetErLog->Response.MkhiHeader.Fields.Command == GET_ER_LOG_CMD) &&
        (GetErLog->Response.MkhiHeader.Fields.IsResponse == 1) &&
        (GetErLog->Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
      if (GetErLog->Response.DataLength > RemainingBufSize) {
        Status = EFI_BUFFER_TOO_SMALL;
      } else {
        CopyMem (EventLogBuffer, GetErLog->Response.LogData, GetErLog->Response.DataLength);
        *ReceivedLogSize = GetErLog->Response.DataLength;
        *RemainingEntries = GetErLog->Response.RemainingEntries;
        *LogEntryIndex = GetErLog->Response.LastLogEntryIndex;
      }
    } else {
      Status = EFI_DEVICE_ERROR;
    }
  }
  FreePool (GetErLog);

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
HeciDataClear (
  VOID
  )
{
  EFI_STATUS                 Status;
  DATA_CLEAR_BUFFER          DataClear;
  UINT32                     Length;
  UINT32                     RespLength;

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

//
// MKHI_GEN_GROUP_ID = 0xFF
//

/**
  Get MKHI (Management Engine Kernel Host Interface) Version.

  @param[out] MkhiVersion           MKHI Version

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetMkhiVersion (
  OUT MKHI_VERSION        *MkhiVersion
  )
{
  EFI_STATUS                Status;
  GET_MKHI_VERSION_BUFFER   GetMkhiVersion;
  UINT32                    RecvLength;

  GetMkhiVersion.Request.MkhiHeader.Data           = 0;
  GetMkhiVersion.Request.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  GetMkhiVersion.Request.MkhiHeader.Fields.Command = GEN_GET_MKHI_VERSION_CMD;
  RecvLength                                       = sizeof (GET_MKHI_VERSION_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetMkhiVersion.Request,
             (UINT32) sizeof (GET_MKHI_VERSION_REQUEST),
             (UINT32 *) &GetMkhiVersion.Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status) && (GetMkhiVersion.Response.MkhiHeader.Fields.Command == GEN_GET_MKHI_VERSION_CMD) &&
      (GetMkhiVersion.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (GetMkhiVersion.Response.MkhiHeader.Fields.Result == 0)) {
    *MkhiVersion = GetMkhiVersion.Response.MkhiVersion;
  }

  return Status;
}

/**
  Send Get Firmware Version Request to ME

  @param[out] GetFwVersionAckData  Return the message of FW version

  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL     Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetFwVersionMsg (
  OUT GET_FW_VER_ACK_DATA  *GetFwVersionAckData
  )
{
  EFI_STATUS          Status;
  GET_FW_VER_BUFFER   GetFwVersion;
  UINT32              MeMode;
  UINT32              RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ///
  /// Allocate MsgGenGetFwVersion data structure
  ///
  GetFwVersion.Request.MkhiHeader.Data            = 0;
  GetFwVersion.Request.MkhiHeader.Fields.GroupId  = MKHI_GEN_GROUP_ID;
  GetFwVersion.Request.MkhiHeader.Fields.Command  = GEN_GET_FW_VERSION_CMD;
  RecvLength                                      = sizeof (GET_FW_VER_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetFwVersion.Request,
             (UINT32) sizeof (GET_FW_VER_REQUEST),
             (UINT32 *) &GetFwVersion.Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status) && (GetFwVersion.Response.MkhiHeader.Fields.Command == GEN_GET_FW_VERSION_CMD) &&
      (GetFwVersion.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (GetFwVersion.Response.MkhiHeader.Fields.Result == 0)) {
    *GetFwVersionAckData = GetFwVersion.Response.Data;
  }

  return Status;
}

/**
  This message is sent by the BIOS to notify ME firmware that the BIOS is starting
  to run 3rd party code.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function or EOP was sent
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
**/
EFI_STATUS
HeciCoreBiosDoneMsg (
  VOID
  )
{
  EFI_STATUS              Status;
  CORE_BIOS_DONE_BUFFER   CoreBiosDone;
  UINT32                  RecvLength;
  UINT32                  MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  CoreBiosDone.Request.MkhiHeader.Data            = 0;
  CoreBiosDone.Request.MkhiHeader.Fields.GroupId  = MKHI_GEN_GROUP_ID;
  CoreBiosDone.Request.MkhiHeader.Fields.Command  = GEN_CORE_BIOS_DONE_CMD;
  RecvLength                                      = sizeof (CORE_BIOS_DONE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &CoreBiosDone.Request,
             (UINT32) sizeof (CORE_BIOS_DONE_REQUEST),
             (UINT32 *) &CoreBiosDone.Response,
             &RecvLength
             );

  return Status;
}

/**
  Send CPU brand class fuse to the CSME (only for corporate SKU).

  This message is required for BIOS running on CPUs which don't support SID message to obtain CPU information.

  @param[in] CpuBrandClass        CPU brand class

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSendCpuBrandClassFuse (
  IN UINT8 CpuBrandClass
  )
{
  EFI_STATUS                  Status;
  GEN_SEND_CPU_BRAND_BUFFER   SendCpuBrand;
  UINT32                      MeMode;
  UINT32                      RecvLength;

  if (!MeClientIsCorporate ()) {
    return EFI_UNSUPPORTED;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  SendCpuBrand.Request.MkhiHeader.Data           = 0;
  SendCpuBrand.Request.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  SendCpuBrand.Request.MkhiHeader.Fields.Command = GEN_SEND_CPU_BRAND_CLASS_FUSE_CMD;
  SendCpuBrand.Request.CpuBrandClass             = CpuBrandClass;
  RecvLength                                     = sizeof (GEN_SEND_CPU_BRAND_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &SendCpuBrand.Request,
             (UINT32) sizeof (GEN_SEND_CPU_BRAND_REQUEST),
             (UINT32 *) &SendCpuBrand.Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status) &&
     ((SendCpuBrand.Response.MkhiHeader.Fields.GroupId != MKHI_GEN_GROUP_ID) ||
      (SendCpuBrand.Response.MkhiHeader.Fields.Command != GEN_SEND_CPU_BRAND_CLASS_FUSE_CMD) ||
      (SendCpuBrand.Response.MkhiHeader.Fields.IsResponse == 0) ||
      (SendCpuBrand.Response.MkhiHeader.Fields.Result != MkhiStatusSuccess))) {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Get CPU Type Change User Message.

  @param[in] CpuTypeChangeInfo    CPU Type Change User Message

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetCpuTypeChange (
  OUT GEN_GET_CPU_TYPE_CHANGE_DATA *CpuTypeChangeInfo
  )
{
  EFI_STATUS                        Status;
  GEN_GET_CPU_TYPE_CHANGE_BUFFER    GetCpuTypeChange;
  UINT32                            RecvLength;

  GetCpuTypeChange.Request.MkhiHeader.Data           = 0;
  GetCpuTypeChange.Request.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  GetCpuTypeChange.Request.MkhiHeader.Fields.Command = GEN_GET_CPU_TYPE_CHANGE_USER_MESSAGE_CMD;
  RecvLength                                         = sizeof (GEN_GET_CPU_TYPE_CHANGE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetCpuTypeChange.Request,
             (UINT32) sizeof (GEN_GET_CPU_TYPE_CHANGE_REQUEST),
             (UINT32 *) &GetCpuTypeChange.Response,
             &RecvLength
             );

  if (HeciMkhiError (Status, GetCpuTypeChange.Response.MkhiHeader, GEN_GET_CPU_TYPE_CHANGE_USER_MESSAGE_CMD, TRUE)) {
    return EFI_DEVICE_ERROR;
  }

  CopyMem (CpuTypeChangeInfo, &GetCpuTypeChange.Response.Data, sizeof (GEN_GET_CPU_TYPE_CHANGE_DATA));

  return Status;
}

/**
  Send CPU Type Change User Response.

  @param[in] UserResponse         CPU Type Change User Response:
                                    0 - Change Rejected
                                    1 - Change Accepted

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetCpuTypeChangeUserResponse (
  IN UINT8 UserResponse
  )
{
  EFI_STATUS                        Status;
  GEN_SET_CPU_TYPE_CHANGE_BUFFER    SetCpuTypeChange;
  UINT32                            RecvLength;

  SetCpuTypeChange.Request.MkhiHeader.Data           = 0;
  SetCpuTypeChange.Request.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  SetCpuTypeChange.Request.MkhiHeader.Fields.Command = GEN_SET_CPU_TYPE_CHANGE_USER_RESPONSE_CMD;
  SetCpuTypeChange.Request.UserResponse              = UserResponse;
  RecvLength                                         = sizeof (GEN_SET_CPU_TYPE_CHANGE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &SetCpuTypeChange.Request,
             (UINT32) sizeof (GEN_SET_CPU_TYPE_CHANGE_REQUEST),
             (UINT32 *) &SetCpuTypeChange.Response,
             &RecvLength
             );

  if (HeciMkhiError (Status, SetCpuTypeChange.Response.MkhiHeader, GEN_SET_CPU_TYPE_CHANGE_USER_RESPONSE_CMD, TRUE)) {
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Unconfigure ME without password

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciUnconfigWithoutPwd (
  VOID
  )
{
  EFI_STATUS                    Status;
  UNCONFIG_WO_PASSWORD_BUFFER   UnconfigWithoutPassword;
  UINT32                        RecvLength;

  UnconfigWithoutPassword.MkhiHeader.Data           = 0;
  UnconfigWithoutPassword.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  UnconfigWithoutPassword.MkhiHeader.Fields.Command = GEN_UNCFG_WO_PWD_CMD;
  RecvLength                                        = sizeof (UNCONFIG_WO_PASSWORD_BUFFER);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &UnconfigWithoutPassword,
             (UINT32) sizeof (UNCONFIG_WO_PASSWORD_BUFFER),
             (UINT32 *) &UnconfigWithoutPassword,
             &RecvLength
             );

  if (HeciMkhiError (Status, UnconfigWithoutPassword.MkhiHeader, GEN_UNCFG_WO_PWD_CMD, TRUE)) {
    DEBUG ((DEBUG_ERROR, "PS: HeciMkhiError Status = %r\n", Status));
    return EFI_DEVICE_ERROR;
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
HeciGetUnconfigureStatus (
  OUT UINT8              *UnconfigStatus
  )
{
  EFI_STATUS                Status;
  GEN_GET_ME_UNCFG_STATE    UnconfigureStatus;
  UINT32                    MeMode;
  UINT32                    RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  UnconfigureStatus.MkhiHeader.Data           = 0;
  UnconfigureStatus.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  UnconfigureStatus.MkhiHeader.Fields.Command = GEN_GET_ME_UNCFG_STATE_CMD;
  RecvLength                                  = sizeof (GEN_GET_ME_UNCFG_STATE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &UnconfigureStatus,
             (UINT32) sizeof (GEN_GET_ME_UNCFG_STATE),
             (UINT32 *) &UnconfigureStatus,
             &RecvLength
             );

  if (!EFI_ERROR (Status) && (UnconfigureStatus.MkhiHeader.Fields.Command == GEN_GET_ME_UNCFG_STATE_CMD) &&
      (UnconfigureStatus.MkhiHeader.Fields.IsResponse == 1)) {
    *UnconfigStatus = (UINT8) UnconfigureStatus.MkhiHeader.Fields.Result;
  }

  return Status;
}

/**
  Send Set Manufacturing Me Reset and Halt Request to ME

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetManufacturingMeResetAndHalt (
  VOID
  )
{
  EFI_STATUS                  Status;
  SET_ME_RESET_HALT_REQUEST   SetMeResetAndHalt;
  HECI_FWS_REGISTER           MeFirmwareStatus;
  UINT64                      HeciBaseAddress;
  UINTN                       TimeOut;
  UINT32                      MeMode;

  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  HeciBaseAddress = MeHeciPciCfgBase (HECI1);
  DEBUG ((DEBUG_INFO, "%a(): MeFirmwareStatus: %08x before messaging.\n", __FUNCTION__, PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS)));

  ///
  /// Allocate MsgGenSetMeResetAndHalt data structure
  ///
  SetMeResetAndHalt.MkhiHeader.Data           = 0;
  SetMeResetAndHalt.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  SetMeResetAndHalt.MkhiHeader.Fields.Command = GEN_SET_ME_RESET_HALT_CMD;

  Status = HeciWrapperSend (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &SetMeResetAndHalt,
             (UINT32) sizeof (SET_ME_RESET_HALT_REQUEST)
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  TimeOut = 0;
  while (TRUE) {
    ///
    /// Read ME status and check for operation mode
    ///
    MeFirmwareStatus.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS);
    if (MeFirmwareStatus.r.MeOperationMode == ME_OPERATION_MODE_SOFT_TEMP_DISABLE) {
      break;
    }

    MicroSecondDelay (ME_STATE_STALL_1_SECOND);
    TimeOut++;

    if (TimeOut >= MSG_MAX_WAIT_TIMEOUT) {
      DEBUG ((DEBUG_ERROR, "Timeout occured, MeFirmwareStatus: %08x.\n", MeFirmwareStatus.ul));
      return EFI_TIMEOUT;
    }
  }

  DEBUG ((DEBUG_INFO, "%a() succeeds.\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Sends the MKHI Enable/Disable FW Features.

  @param[in] EnableBitMask        Bitmask of features to enable
  @param[in] DisableBitMask       Bitmask of features to disable

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
  @retval EFI_ACCESS_DENIED       Command request is rejected
**/
EFI_STATUS
HeciFwFeatureStateOverride (
  IN UINT32                       EnableBitmask,
  IN UINT32                       DisableBitmask
  )
{
  EFI_STATUS                                        Status;
  FW_FEATURE_SHIPMENT_TIME_STATE_OVERRIDE_BUFFER    FwFeatureStateOverride;
  UINT32                                            RecvLength;
  UINT32                                            MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  FwFeatureStateOverride.Request.MkhiHeader.Data             = 0;
  FwFeatureStateOverride.Request.MkhiHeader.Fields.GroupId   = MKHI_GEN_GROUP_ID;
  FwFeatureStateOverride.Request.MkhiHeader.Fields.Command   = GEN_FW_FEATURE_STATE_OVERRIDE_CMD;
  FwFeatureStateOverride.Request.FeatureState.EnableFeature  = EnableBitmask;
  FwFeatureStateOverride.Request.FeatureState.DisableFeature = DisableBitmask;
  RecvLength                                                 = sizeof (FW_FEATURE_SHIPMENT_TIME_STATE_OVERRIDE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &FwFeatureStateOverride.Request,
             (UINT32) sizeof (FW_FEATURE_SHIPMENT_TIME_STATE_OVERRIDE_REQUEST),
             (UINT32 *) &FwFeatureStateOverride.Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status) && (FwFeatureStateOverride.Response.Status == SetFeatureStateRejected)) {
    Status = EFI_ACCESS_DENIED;
  }

  return Status;
}

/**
  Send Get Image Firmware Version Request to ME

  @param[in]  PartitionId          Partition ID
  @param[out] NumOfModules         Return the number of modules in the partition
  @param[out] PartitionIdData      Return the partition data structure containing all the version information

  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL     Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetImageFwVersionMsg (
  IN  UINT32                    PartitionId,
  OUT UINT32                    *NumOfModules,
  OUT FLASH_PARTITION_DATA      *PartitionIdData
  )
{
  EFI_STATUS                    Status;
  GET_IMAGE_FW_VERSION_BUFFER   GetImageFwVersion;
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
  RecvLength                                          = sizeof (GET_IMAGE_FW_VER_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetImageFwVersion.Request,
             (UINT32) sizeof (GET_IMAGE_FW_VER_REQUEST),
             (UINT32 *) &GetImageFwVersion.Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status) &&
      (GetImageFwVersion.Response.MkhiHeader.Fields.Command == GEN_GET_IMAGE_FW_VERSION) &&
      (GetImageFwVersion.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (GetImageFwVersion.Response.MkhiHeader.Fields.Result == 0) &&
      (GetImageFwVersion.Response.ManifestData.PartitionId == PartitionId)) {
    *NumOfModules = GetImageFwVersion.Response.NumOfModules;
    *PartitionIdData = GetImageFwVersion.Response.ManifestData;
  }

  return Status;
}

/**
  Send Set FIPS Mode to Enabled or Disabled

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetFipsMode (
  IN UINT32 FipsMode
  )
{
  EFI_STATUS              Status;
  SET_FIPS_MODE_BUFFER    SetFipsMode;
  UINT32                  RecvLength;
  UINT32                  MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  SetFipsMode.Request.MkhiHeader.Data              = 0;
  SetFipsMode.Request.MkhiHeader.Fields.GroupId    = MKHI_GEN_GROUP_ID;
  SetFipsMode.Request.MkhiHeader.Fields.Command    = GEN_SET_FIPS_MODE_CMD;
  SetFipsMode.Request.Data.FipsMode                = FipsMode;
  RecvLength                                       = sizeof (SET_FIPS_MODE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &SetFipsMode.Request,
             (UINT32) sizeof (SET_FIPS_MODE_REQUEST),
             (UINT32 *) &SetFipsMode.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (SetFipsMode.Request.MkhiHeader.Fields.Result != MkhiStatusSuccess) {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Send Get Current FIPS Mode and Crypto Driver version

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetFipsMode (
  OUT GET_FIPS_MODE_DATA  *GetFipsModeData
  )
{
  EFI_STATUS              Status;
  GET_FIPS_MODE_BUFFER    GetFipsMode;
  UINT32                  MeMode;
  UINT32                  RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  GetFipsMode.Request.MkhiHeader.Data              = 0;
  GetFipsMode.Request.MkhiHeader.Fields.GroupId    = MKHI_GEN_GROUP_ID;
  GetFipsMode.Request.MkhiHeader.Fields.Command    = GEN_GET_FIPS_MODE_CMD;
  RecvLength                                       = sizeof (GET_FIPS_MODE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetFipsMode.Request,
             (UINT32) sizeof (GET_FIPS_MODE_REQUEST),
             (UINT32 *) &GetFipsMode.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((GetFipsMode.Response.MkhiHeader.Fields.Command == GEN_GET_FIPS_MODE_CMD) &&
      (GetFipsMode.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (GetFipsMode.Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
    *GetFipsModeData = GetFipsMode.Response.Data;
  } else {
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Send Set Measured Boot State to Enable or Disable CSME Measurement

  @param[in]  SetMeasuredBootState  Set Measured boot state to Enable/Disable CSME Extended Measured boot

  @retval EFI_UNSUPPORTED         Current CSME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetMeasuredBootState (
  IN UINT8 SetMeasuredBootState
  )
{
  EFI_STATUS                  Status;
  SET_MEASURED_BOOT_BUFFER    SetMeasureBoot;
  UINT32                      RecvLength;
  UINT32                      MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  SetMeasureBoot.Request.MkhiHeader.Data = 0;
  SetMeasureBoot.Request.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  SetMeasureBoot.Request.MkhiHeader.Fields.Command = GEN_SET_MEASURED_BOOT_STATE_CMD;
  SetMeasureBoot.Request.Data = SetMeasuredBootState & 0x01;
  RecvLength = sizeof (SET_MEASURED_BOOT_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &SetMeasureBoot.Request,
             (UINT32) sizeof (SET_MEASURED_BOOT_REQUEST),
             (UINT32 *) &SetMeasureBoot.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (SetMeasureBoot.Request.MkhiHeader.Fields.Result != MkhiStatusSuccess) {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Send Get Current CSME Measured Boot State

  @param[out] GetMeasuredBootState    Pointer to get CSME Measured boot status.

  @retval EFI_UNSUPPORTED         Current CSME mode/version doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetMeasuredBootState (
  OUT UINT8  *GetMeasuredBootState
  )
{
  EFI_STATUS                  Status;
  GET_MEASURED_BOOT_BUFFER    GetMeasuredBoot;
  UINT32                      RecvLength;
  UINT32                      MeMode;

  if (GetMeasuredBootState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  GetMeasuredBoot.Request.MkhiHeader.Data = 0;
  GetMeasuredBoot.Request.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  GetMeasuredBoot.Request.MkhiHeader.Fields.Command = GEN_GET_MEASURED_BOOT_STATE_CMD;
  RecvLength = sizeof (GET_MEASURED_BOOT_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetMeasuredBoot.Request,
             (UINT32) sizeof (GET_MEASURED_BOOT_REQUEST),
             (UINT32 *) &GetMeasuredBoot.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((GetMeasuredBoot.Response.MkhiHeader.Fields.Command == GEN_GET_MEASURED_BOOT_STATE_CMD) &&
      (GetMeasuredBoot.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (GetMeasuredBoot.Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
    *GetMeasuredBootState = GetMeasuredBoot.Response.Data;
  } else {
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  This message is sent by the BIOS to retrieve data from SPI

  @param[in] SpiReadType           Requested SPI read type
  @param[in] SpiDataLength         Length of the requested SPI data
  @param[out] SpiData              Retrieved SPI data

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES     Unable to allocate required resources
**/
EFI_STATUS
HeciReadFromSpi (
  IN  UINT32                    SpiReadType,
  IN  UINT32                    SpiDataLength,
  OUT UINT32                    *SpiData
  )
{
  EFI_STATUS              Status;
  READ_FROM_SPI_BUFFER    *ReadFromSpi;
  UINT32                  MeMode;
  UINT32                  RecvLength;

  if (SpiData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ReadFromSpi = AllocateZeroPool (sizeof (READ_FROM_SPI_BUFFER));
  if (ReadFromSpi == NULL) {
    DEBUG ((DEBUG_ERROR, "%a Error: Could not allocate Memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  ReadFromSpi->Request.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  ReadFromSpi->Request.MkhiHeader.Fields.Command = GEN_READ_FROM_SPI;
  ReadFromSpi->Request.ReadType                  = SpiReadType;
  RecvLength = sizeof (READ_FROM_SPI_RESPONSE) + SpiDataLength;

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &ReadFromSpi->Request,
             (UINT32) sizeof (READ_FROM_SPI_REQUEST),
             (UINT32 *) &ReadFromSpi->Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    FreePool (ReadFromSpi);
    return Status;
  }

  if ((ReadFromSpi->Response.MkhiHeader.Fields.Command == GEN_READ_FROM_SPI) &&
      (ReadFromSpi->Response.MkhiHeader.Fields.IsResponse == 1) &&
      (ReadFromSpi->Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
     CopyMem (SpiData, &ReadFromSpi->Response.SpiData, SpiDataLength);
  } else {
    Status = EFI_DEVICE_ERROR;
  }

  FreePool (ReadFromSpi);

  return Status;
}

/**
  This message is sent by the BIOS to retrieve Master Access data from SPI

  @param[out] SpiData              Retrieved Master Access data

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES     Unable to allocate required resources
**/
EFI_STATUS
HeciSpiReadPrimaryAccess (
  OUT SPI_DESC_PRIMARY_DATA *SpiData
  )
{
  return HeciReadFromSpi (SpiReadTypePrimaryAccess, sizeof (SPI_DESC_PRIMARY_DATA), (UINT32 *) SpiData);
}

/**
  This message is sent by the BIOS to retrieve GBE Version data from SPI

  @param[out] SpiData              Retrieved GBE Version data

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES     Unable to allocate required resources
**/
EFI_STATUS
HeciSpiReadGbeVersion (
  OUT SPI_GBE_VERSION *SpiData
  )
{
  return HeciReadFromSpi (SpiReadTypeGbeVersion, sizeof (SPI_GBE_VERSION), (UINT32 *) SpiData);
}

/**
  This message is sent by the BIOS to retrieve Flash Info data from SPI

  @param[out] SpiData              Retrieved Flash Info data

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES     Unable to allocate required resources
**/
EFI_STATUS
HeciSpiReadFlashInfo (
  OUT SPI_FLASH_INFO *SpiData
  )
{
  return HeciReadFromSpi (SpiReadTypeFlashInfo, sizeof (SPI_FLASH_INFO), (UINT32 *) SpiData);
}

/**
  This message is sent by the BIOS to retrieve Descriptor Resilience Info data from SPI

  @param[out] SpiData              Retrieved Flash Info data

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES     Unable to allocate required resources
**/
EFI_STATUS
HeciSpiReadDescriptorResilienceInfo (
  OUT SPI_DESCRIPTOR_RESILIENCE_INFO *SpiData
  )
{
  return HeciReadFromSpi (SpiReadTypeDescriptorResilienceInfo, sizeof (SPI_DESCRIPTOR_RESILIENCE_INFO), (UINT32 *) SpiData);
}

//
// ME Client - Hardware Asset
//

/**
  Send Hardware Asset Tables to Firmware

  @param[in] AssetTableData       Hardware Asset Table Data
  @param[in] TableDataSize        Size of Asset table

  @retval EFI_SUCCESS             Table sent
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_NOT_READY           ME not ready
  @retval EFI_OUT_OF_RESOURCES    Could not allocate Memory
**/
EFI_STATUS
HeciAssetUpdateFwMsg (
  IN HWA_TABLE *AssetTableData,
  IN UINT16    TableDataSize
  )
{
  EFI_STATUS            Status;
  HWA_PUSH_TABLES_MSG   *SendAssetTableDataMsg;
  UINT32                MeMode;
  UINT32                MeStatus;
  UINT32                ReqLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    DEBUG ((DEBUG_WARN, "%a: Could not read FW Mode or unsupported ME Mode\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  Status = MeBiosGetMeStatus (&MeStatus);
  if (EFI_ERROR (Status) || (ME_STATUS_ME_STATE_ONLY (MeStatus) != ME_READY)) {
    DEBUG ((DEBUG_WARN, "%a: Could not read FW Status or ME not ready", __FUNCTION__));
    return EFI_NOT_READY;
  }

  if (TableDataSize > MAX_ASSET_TABLE_ALLOCATED_SIZE) {
    return EFI_OUT_OF_RESOURCES;
  }

  SendAssetTableDataMsg = AllocateZeroPool (sizeof (HWA_PUSH_TABLES_MSG) + TableDataSize);
  if (SendAssetTableDataMsg == NULL) {
    DEBUG ((DEBUG_ERROR, "%a Error: Could not allocate Memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  SendAssetTableDataMsg->Header.Data                 = 0;
  SendAssetTableDataMsg->Header.Fields.MessageLength = sizeof (SendAssetTableDataMsg->Data) + TableDataSize;
  SendAssetTableDataMsg->Header.Fields.Command       = HWA_TABLE_PUSH_CMD;

  if (AssetTableData->Table[HWA_TABLE_TYPE_INDEX_FRU_DEVICE].Length != sizeof (SINGLE_TABLE_POSITION)) {
    SendAssetTableDataMsg->Header.Fields.PciFruTablePresent = 1;
  }
  if (AssetTableData->Table[HWA_TABLE_TYPE_INDEX_MEDIA_DEVICE].Length != sizeof (SINGLE_TABLE_POSITION)) {
    SendAssetTableDataMsg->Header.Fields.MediaTablePresent  = 1;
  }
  if (AssetTableData->Table[HWA_TABLE_TYPE_INDEX_SMBIOS].Length != 0) {
    SendAssetTableDataMsg->Header.Fields.SmbiosTablePresent = 1;
  }
  if (AssetTableData->Table[HWA_TABLE_TYPE_INDEX_ASF].Length != 0) {
    SendAssetTableDataMsg->Header.Fields.AsfTablePresent    = 1;
  }
  CopyMem (&SendAssetTableDataMsg->Data, AssetTableData, SendAssetTableDataMsg->Header.Fields.MessageLength);

  ReqLength = SendAssetTableDataMsg->Header.Fields.MessageLength + sizeof (HWA_MESSAGE_HEADER);

  PERF_INMODULE_BEGIN ("HeciSendAssetTable");

  Status = HeciWrapperSend (
            BIOS_FIXED_HOST_ADDR,
            HECI_HWA_MESSAGE_ADDR,
            (UINT32 *) SendAssetTableDataMsg,
            ReqLength
            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to SendAssetTableDataMsg\n", __FUNCTION__));
  }

  PERF_INMODULE_END ("HeciSendAssetTable");

  FreePool (SendAssetTableDataMsg);

  return Status;
}

//
// ME Client - Manufacturing and Configuration Host Interface (MCHI)
//

//
// MCHI_MCA_GROUP_ID = 0x0A
//

/**
  This message is sent by the BIOS to retrieve file stored in ME firmware NVM.

  @param[in]      FileId          Id number of file to read
  @param[in]      Offset          File offset
  @param[in, out] DataSize        On input - size of data to read, on output - size of read data
  @param[in]      Flags           Flags
  @param[out]     Buffer          Pointer to the data buffer

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciReadFileExMsg (
  IN UINT32           FileId,
  IN UINT32           Offset,
  IN OUT UINT32       *DataSize,
  IN UINT8            Flags,
  OUT UINT8           *DataBuffer
  )
{
  EFI_STATUS            Status;
  READ_FILE_EX_BUFFER   *ReadFileEx;
  UINT32                MeMode;
  UINT32                ReqLength;
  UINT32                RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ReqLength  = sizeof (READ_FILE_EX_REQUEST);
  RecvLength = sizeof (READ_FILE_EX_RESPONSE) + *DataSize;
  ReadFileEx = AllocateZeroPool (MAX (ReqLength, RecvLength));
  ZeroMem (DataBuffer, *DataSize);

  if (ReadFileEx == NULL) {
    DEBUG ((DEBUG_ERROR, "%a Error: Could not allocate Memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  ReadFileEx->Request.MchiHeader.Fields.GroupId = MCHI_MCA_GROUP_ID;
  ReadFileEx->Request.MchiHeader.Fields.Command = McaReadFileExCmd;
  ReadFileEx->Request.FileId                    = FileId;
  ReadFileEx->Request.Offset                    = Offset;
  ReadFileEx->Request.DataSize                  = *DataSize;
  ReadFileEx->Request.Flags                     = Flags;

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MCHI_MESSAGE_ADDR,
             (UINT32 *) &ReadFileEx->Request,
             (UINT32) sizeof (READ_FILE_EX_REQUEST),
             (UINT32 *) &ReadFileEx->Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status)) {
    if (ReadFileEx->Response.MchiHeader.Fields.Command == McaReadFileExCmd &&
       (ReadFileEx->Response.MchiHeader.Fields.IsResponse == 1) &&
       (ReadFileEx->Response.MchiHeader.Fields.Result == McaStatusSuccess) &&
       (ReadFileEx->Response.DataSize <= *DataSize)) {
      CopyMem (DataBuffer, ReadFileEx->Response.Data, ReadFileEx->Response.DataSize);
      *DataSize = ReadFileEx->Response.DataSize;
    } else {
      Status = EFI_DEVICE_ERROR;
    }
  }

  FreePool (ReadFileEx);

  return Status;
}

/**
  Read Delayed Authentication Mode from ME NVM

  @param[out] DamState            Delayed Authentication Mode State

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetDamState (
  OUT UINT8 *DamState
  )
{
  UINT32    DataSize;

  DataSize = MCA_DAM_FILE_SIZE;

  return HeciReadFileExMsg (McaDamFileId, 0, &DataSize, 0, DamState);
}

/**
  Read UPID Platform Id feature supported state from ME NVM

  @param[out] PlatIdSupported     Platform Id feature supported State

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetPlatIdSupportedState (
  OUT UINT8  *PlatIdSupported
  )
{
  UINT32    DataSize;

  DataSize = MCA_PLAT_ID_SUPPORT_FILE_SIZE;

  return HeciReadFileExMsg (McaPlatIdSupportFileId, 0, &DataSize, 0, PlatIdSupported);
}

/**
  Read Qualified Brand Identity support from ME NVM

  @param[out] QBEState            Qualified Brand Identity State

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES    Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetQualifiedBrandEntitlements (
  OUT UINT8 *QBEState
  )
{
  UINT32    DataSize;

  DataSize = sizeof (BRAND_ENTITLEMENT);

  return HeciReadFileExMsg (McaBrandEntitlementFileId, 0, &DataSize, 0, QBEState);
}

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
HeciSetFileExMsg (
  IN UINT32          FileId,
  IN UINT32          Offset,
  IN UINT32          DataSize,
  IN UINT8           Flags,
  IN UINT8           *DataBuffer
  )
{
  EFI_STATUS            Status;
  SET_FILE_EX_BUFFER    *SetFileEx;
  UINT32                MeMode;
  UINT32                ReqLength;
  UINT32                RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ReqLength  = sizeof (SET_FILE_EX_REQUEST) + DataSize;
  RecvLength = sizeof (SET_FILE_EX_RESPONSE);

  SetFileEx = AllocateZeroPool (ReqLength);
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
             (UINT32 *) &SetFileEx->Request,
             ReqLength,
             (UINT32 *) &SetFileEx->Response,
             &RecvLength
             );

  if (SetFileEx->Response.MchiHeader.Fields.Result != McaStatusSuccess) {
    Status = EFI_DEVICE_ERROR;
  }

  FreePool (SetFileEx);

  return Status;
}

/**
  Set Delayed Authentication Mode in ME NVM

  @param[in] DamState             New Delayed Authentication Mode State:
                                    0 - DAM Disable
                                    1 - DAM Enable

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetDamState (
  IN UINT8 DamState
  )
{
  EFI_STATUS    Status;

  Status = HeciSetFileExMsg (McaDamFileId, 0, MCA_DAM_FILE_SIZE, 0, &DamState);
  if EFI_ERROR (Status) {
    return Status;
  }

  return HeciCommitFilesMsg ();
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
HeciCommitFilesMsg (
  VOID
  )
{
  EFI_STATUS          Status;
  COMMIT_FILES        CommitFiles;
  UINT32              MeMode;
  UINT32              RecvLength;

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
  RecvLength                            = sizeof (COMMIT_FILES);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MCHI_MESSAGE_ADDR,
             (UINT32 *) &CommitFiles,
             (UINT32) sizeof (COMMIT_FILES),
             (UINT32 *) &CommitFiles,
             &RecvLength
             );

  if (CommitFiles.MchiHeader.Fields.Result != McaStatusSuccess) {
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
HeciGetRpmcStatus (
  OUT RP_STATUS_3         *RpStatus
  )
{
  EFI_STATUS                Status;
  GET_RPMC_STATUS_BUFFER    RpmcStatus;
  UINT32                    RecvLength;
  UINT32                    MeMode;

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
  RecvLength                                   = sizeof (GET_RPMC_STATUS_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MCHI_MESSAGE_ADDR,
             (UINT32 *) &RpmcStatus.Request,
             (UINT32) sizeof (GET_RPMC_STATUS_REQUEST),
             (UINT32 *) &RpmcStatus.Response,
             &RecvLength
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

/**
  This command indicates to the FW that it shall commit ARBSVN to fuse.

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
**/
EFI_STATUS
HeciArbSvnCommitMsg (
  VOID
  )
{
  EFI_STATUS              Status;
  ARB_SVN_COMMIT_BUFFER   ArbSvnCommit;
  UINT32                  RecvLength;
  UINT32                  MeMode;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&ArbSvnCommit, sizeof (ARB_SVN_COMMIT_BUFFER));
  ArbSvnCommit.Request.MchiHeader.Data           = 0;
  ArbSvnCommit.Request.MchiHeader.Fields.GroupId = MCHI_MCA_GROUP_ID;
  ArbSvnCommit.Request.MchiHeader.Fields.Command = McaArbSvnCommitCmd;
  ArbSvnCommit.Request.UsageId                   = ARB_SVN_COMMIT_ALL;
  RecvLength                                     = sizeof (ARB_SVN_COMMIT_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MCHI_MESSAGE_ADDR,
             (UINT32 *) &ArbSvnCommit.Request,
             (UINT32) sizeof (ARB_SVN_COMMIT_REQUEST),
             (UINT32 *) &ArbSvnCommit.Response,
             &RecvLength
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((ArbSvnCommit.Response.MchiHeader.Fields.Command != McaArbSvnCommitCmd) ||
      (ArbSvnCommit.Response.MchiHeader.Fields.GroupId != MCHI_MCA_GROUP_ID) ||
      (ArbSvnCommit.Response.MchiHeader.Fields.IsResponse != 1) ||
      (ArbSvnCommit.Response.MchiHeader.Fields.Result != McaStatusSuccess)) {
    DEBUG ((
      DEBUG_ERROR,
      "ERROR: Error in MCA_ARB_SVN_COMMIT_CMD response (0x%X)\n",
      ArbSvnCommit.Response.MchiHeader.Fields.Result
      ));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  The command retrieves anti-replay SVN information.
  Caller can set Entries as 0 to get the correct number of entries CSME contains.

  @param[in, out] Entries         On input, it is the number of entries caller expects.
                                  On output, it indicates the number of entries CSME contains.
  @param[in, out] ArbSvnInfo      Anti-Rollback SVN Information

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate required resources
  @retval EFI_BUFFER_TOO_SMALL    The Entries is too small for the result
**/
EFI_STATUS
HeciArbSvnGetInfoMsg (
  IN OUT UINT32                *Entries,
  IN OUT ARB_SVN_INFO_ENTRY    *ArbSvnInfo
  )
{
  EFI_STATUS                Status;
  ARB_SVN_GET_INFO_BUFFER   *ArbSvnGetInfo;
  UINT32                    MeMode;
  UINT32                    RecvLength;
  UINT32                    NumberOfEntries;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  if (Entries == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  NumberOfEntries = *Entries;
  ArbSvnGetInfo   = AllocateZeroPool (NumberOfEntries * sizeof (ARB_SVN_INFO_ENTRY) + sizeof (ARB_SVN_GET_INFO_BUFFER));
  if (ArbSvnGetInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ArbSvnGetInfo->Request.MchiHeader.Fields.GroupId = MCHI_MCA_GROUP_ID;
  ArbSvnGetInfo->Request.MchiHeader.Fields.Command = McaArbSvnGetInfoCmd;
  RecvLength                                       = NumberOfEntries * sizeof (ARB_SVN_INFO_ENTRY) + sizeof (ARB_SVN_GET_INFO_BUFFER);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MCHI_MESSAGE_ADDR,
             (UINT32 *) &ArbSvnGetInfo->Request,
             (UINT32) sizeof (ARB_SVN_GET_INFO_REQUEST),
             (UINT32 *) &ArbSvnGetInfo->Response,
             &RecvLength
             );

  if (EFI_ERROR (Status) && (Status == EFI_BUFFER_TOO_SMALL)) {
    *Entries = ArbSvnGetInfo->Response.NumEntries;
    DEBUG ((DEBUG_INFO, "NumEntries: %08x.\n", ArbSvnGetInfo->Response.NumEntries));
  }

  if (!EFI_ERROR (Status) &&
      ((ArbSvnGetInfo->Response.MchiHeader.Fields.Command) == McaArbSvnGetInfoCmd) &&
      ((ArbSvnGetInfo->Response.MchiHeader.Fields.IsResponse) == 1) &&
      (ArbSvnGetInfo->Response.MchiHeader.Fields.Result == McaStatusSuccess)) {
    ASSERT (sizeof (ArbSvnInfo) <= (sizeof (ArbSvnGetInfo)));
    *Entries = ArbSvnGetInfo->Response.NumEntries;
    CopyMem (ArbSvnInfo, ArbSvnGetInfo->Response.ArbSvnEntry, ArbSvnGetInfo->Response.NumEntries * sizeof (ARB_SVN_INFO_ENTRY));
  }

  FreePool (ArbSvnGetInfo);
  return Status;
}

/**
  Send Get Manufacturing Info Request to CSME

  @param[out] *ManufacturingInfo    Retrieved manufacturing info data.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_INVALID_PARAMETER     Parameter invalid.
**/
EFI_STATUS
HeciGetManufacturingInfo (
  OUT UINT32                    *ManufacturingInfo
  )
{
  EFI_STATUS                      Status;
  GET_MANUFACTURING_INFO_BUFFER   GetManufacturingInfo;
  UINT32                          MeMode;
  UINT32                          RecvLength;

  if (ManufacturingInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&GetManufacturingInfo, sizeof (GET_MANUFACTURING_INFO_BUFFER));

  GetManufacturingInfo.Request.MchiHeader.Fields.GroupId = MCHI_MCA_GROUP_ID;
  GetManufacturingInfo.Request.MchiHeader.Fields.Command = McaGetManufacturingInfoCmd;
  RecvLength                                             = sizeof (GET_MANUFACTURING_INFO_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MCHI_MESSAGE_ADDR,
             (UINT32 *) &GetManufacturingInfo.Request,
             (UINT32) sizeof (GET_MANUFACTURING_INFO_REQUEST),
             (UINT32 *) &GetManufacturingInfo.Response,
             &RecvLength
             );

  if (!EFI_ERROR (Status) && ((GetManufacturingInfo.Response.MchiHeader.Fields.Command) == McaGetManufacturingInfoCmd) &&
      ((GetManufacturingInfo.Response.MchiHeader.Fields.IsResponse) == 1) &&
      (GetManufacturingInfo.Response.MchiHeader.Fields.Result == McaStatusSuccess)) {
    *ManufacturingInfo = GetManufacturingInfo.Response.ManufacturingInfo.Data;
  }

  return Status;
}

//
// ME Client - Unique Platform Id (UPID)
//

/**
  This message is sent by the BIOS to check if given UPID feature is enabled in CSME

  @param[in]  UpidFeature          UPID feature
  @param[out] UpidFeatureState     Determines if feature is enabled

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL     Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetUpidFeatureState (
  IN  UINT32                      UpidFeature,
  OUT UINT8                       *UpidFeatureState
  )
{
  EFI_STATUS                  Status;
  GET_FEATURE_STATE_BUFFER    GetFeatureState;
  UINT32                      MeMode;
  UINT32                      RecvLength;

  if (UpidFeatureState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&GetFeatureState, sizeof (GET_FEATURE_STATE_BUFFER));
  GetFeatureState.Request.UpidHeader.Fields.Feature = UpidFeature;
  GetFeatureState.Request.UpidHeader.Fields.Command = UpidCommonGetFeatureState;
  GetFeatureState.Request.UpidHeader.Fields.Length  = UPID_MESSAGE_BYTE_COUNT_MAP (GET_FEATURE_STATE_REQUEST);
  RecvLength                                        = sizeof (GET_FEATURE_STATE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_UPID_MESSAGE_ADDR,
             (UINT32 *) &GetFeatureState.Request,
             (UINT32) sizeof (GET_FEATURE_STATE_REQUEST),
             (UINT32 *) &GetFeatureState.Response,
             &RecvLength
             );

  if (EFI_ERROR (Status)) {
    return Status;
  } else if (GetFeatureState.Response.Status != UpidStatusSuccess) {
    DEBUG ((DEBUG_ERROR, "%a: UpidStatus Error, Status: %x\n", __FUNCTION__, GetFeatureState.Response.Status));
    Status = EFI_DEVICE_ERROR;
  } else {
    *UpidFeatureState = GetFeatureState.Response.PlatIdFeatureState;
  }

  return Status;
}

/**
  This message is sent by the BIOS to set given UPID feature state in CSME

  @param[in] UpidFeature           UPID feature
  @param[in] UpidFeatureState      New UPID feature state

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL     Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetUpidFeatureState (
  IN UINT32                UpidFeature,
  IN UINT8                 UpidFeatureState
  )
{
  EFI_STATUS                  Status;
  SET_FEATURE_STATE_BUFFER    SetFeatureState;
  UINT32                      MeMode;
  UINT32                      RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&SetFeatureState, sizeof (SET_FEATURE_STATE_BUFFER));
  SetFeatureState.Request.UpidHeader.Fields.Feature = UpidFeature;
  SetFeatureState.Request.UpidHeader.Fields.Command = UpidCommonSetFeatureState;
  SetFeatureState.Request.UpidHeader.Fields.Length  = UPID_MESSAGE_BYTE_COUNT_MAP (SET_FEATURE_STATE_REQUEST);
  SetFeatureState.Request.PlatIdFeatureState        = UpidFeatureState;
  RecvLength                                        = sizeof (SET_FEATURE_STATE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_UPID_MESSAGE_ADDR,
             (UINT32 *) &SetFeatureState.Request,
             (UINT32) sizeof (SET_FEATURE_STATE_REQUEST),
             (UINT32 *) &SetFeatureState.Response,
             &RecvLength
             );

  if (EFI_ERROR (Status)) {
    return Status;
  } else if (SetFeatureState.Response.Status != UpidStatusSuccess) {
    DEBUG ((DEBUG_ERROR, "%a: UpidStatus Error, Status: %x\n", __FUNCTION__, SetFeatureState.Response.Status));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  This message is sent by the BIOS to check if given UPID feature state can be
  modified by the OS

  @param[in]  UpidFeature               UPID feature
  @param[out] UpidFeatureOsControlState Determines if OS can modify the feature state

  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_UNSUPPORTED               Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER         Invalid input parameter
  @retval EFI_DEVICE_ERROR              HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                   HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL          Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetUpidOsControlState (
  IN  UINT32                          UpidFeature,
  OUT UINT8                           *UpidFeatureOsControlState
  )
{
  EFI_STATUS                            Status;
  GET_OS_CONTROL_FEATURE_STATE_BUFFER   GetOsControlFeatureState;
  UINT32                                MeMode;
  UINT32                                RecvLength;

  if (UpidFeatureOsControlState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&GetOsControlFeatureState, sizeof (GET_OS_CONTROL_FEATURE_STATE_BUFFER));

  GetOsControlFeatureState.Request.UpidHeader.Fields.Feature = UpidFeature;
  GetOsControlFeatureState.Request.UpidHeader.Fields.Command = UpidCommonGetOsControlFeatureState;
  GetOsControlFeatureState.Request.UpidHeader.Fields.Length  = UPID_MESSAGE_BYTE_COUNT_MAP (GET_OS_CONTROL_FEATURE_STATE_REQUEST);
  RecvLength                                                 = sizeof (GET_OS_CONTROL_FEATURE_STATE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_UPID_MESSAGE_ADDR,
             (UINT32 *) &GetOsControlFeatureState.Request,
             (UINT32) sizeof (GET_OS_CONTROL_FEATURE_STATE_REQUEST),
             (UINT32 *) &GetOsControlFeatureState.Response,
             &RecvLength
             );

  if (EFI_ERROR (Status)) {
    return Status;
  } else if (GetOsControlFeatureState.Response.Status != UpidStatusSuccess) {
    DEBUG ((DEBUG_ERROR, "%a: UpidStatus Error, Status: %x\n", __FUNCTION__, GetOsControlFeatureState.Response.Status));
    Status = EFI_DEVICE_ERROR;
  } else {
    *UpidFeatureOsControlState = GetOsControlFeatureState.Response.PlatIdFeatureOsControlState;
  }

  return Status;
}

/**
  This message is sent by the BIOS to set if given UPID feature state can be
  modified by the OS

  @param[in] UpidFeature                UPID feature
  @param[in] UpidFeatureOsControlState  Determines if OS can modify the feature state

  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_UNSUPPORTED               Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR              HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                   HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL          Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetUpidOsControlState (
  IN UINT32                           UpidFeature,
  IN UINT8                            UpidFeatureOsControlState
  )
{
  EFI_STATUS                            Status;
  SET_OS_CONTROL_FEATURE_STATE_BUFFER   SetOsControlFeatureState;
  UINT32                                MeMode;
  UINT32                                RecvLength;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&SetOsControlFeatureState, sizeof (SET_OS_CONTROL_FEATURE_STATE_BUFFER));
  SetOsControlFeatureState.Request.UpidHeader.Fields.Feature     = UpidFeature;
  SetOsControlFeatureState.Request.UpidHeader.Fields.Command     = UpidCommonSetOsControlFeatureState;
  SetOsControlFeatureState.Request.UpidHeader.Fields.Length      = UPID_MESSAGE_BYTE_COUNT_MAP (SET_OS_CONTROL_FEATURE_STATE_REQUEST);
  SetOsControlFeatureState.Request.PlatIdFeatureOsControlState   = UpidFeatureOsControlState;
  RecvLength                                                     = sizeof (SET_OS_CONTROL_FEATURE_STATE_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_UPID_MESSAGE_ADDR,
             (UINT32 *) &SetOsControlFeatureState.Request,
             (UINT32) sizeof (SET_OS_CONTROL_FEATURE_STATE_REQUEST),
             (UINT32 *) &SetOsControlFeatureState.Response,
             &RecvLength
             );

  if (EFI_ERROR (Status)) {
    return Status;
  } else if (SetOsControlFeatureState.Response.Status != UpidStatusSuccess) {
    DEBUG ((DEBUG_ERROR, "%a: UpidStatus Error, Status: %x\n", __FUNCTION__, SetOsControlFeatureState.Response.Status));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  This message is sent by the BIOS to read the Platform Id

  @param[out] OemPlatformIdType    Determines format of the OemPlatformId field
  @param[out] OemPlatformId        Platform Id set by the OEM
  @param[out] CsmePlatformId       CSME Platfrom Id generated from CSME fuses

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL     Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetUpidPlatformId (
  OUT UINT32                      *OemPlatformIdType,
  OUT UINT8                       *OemPlatformId,
  OUT UINT8                       *CsmePlatformId
  )
{
  EFI_STATUS                Status;
  GET_PLATFORM_ID_BUFFER    GetPlatformId;
  UINT32                    MeMode;
  UINT32                    RecvLength;

  if (OemPlatformIdType == NULL || OemPlatformId == NULL || CsmePlatformId == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&GetPlatformId, sizeof (GET_PLATFORM_ID_BUFFER));
  GetPlatformId.Request.UpidHeader.Fields.Feature = UpidFeaturePlatId;
  GetPlatformId.Request.UpidHeader.Fields.Command = PlatIdGetPlatformId;
  GetPlatformId.Request.UpidHeader.Fields.Length  = UPID_MESSAGE_BYTE_COUNT_MAP (GET_PLATFORM_ID_REQUEST);
  RecvLength                                      = sizeof (GET_PLATFORM_ID_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_UPID_MESSAGE_ADDR,
             (UINT32 *) &GetPlatformId.Request,
             (UINT32) sizeof (GET_PLATFORM_ID_REQUEST),
             (UINT32 *) &GetPlatformId.Response,
             &RecvLength
             );

  if (EFI_ERROR (Status)) {
    return Status;
  } else if (GetPlatformId.Response.Status != UpidStatusSuccess) {
    DEBUG ((DEBUG_ERROR, "%a: UpidStatus Error, Status: %x\n", __FUNCTION__, GetPlatformId.Response.Status));
    Status = EFI_DEVICE_ERROR;
  } else {
    *OemPlatformIdType = GetPlatformId.Response.OemPlatIdType;
    CopyMem (OemPlatformId, &GetPlatformId.Response.OemPlatId, MAX_PLAT_ID_SIZE);
    CopyMem (CsmePlatformId, &GetPlatformId.Response.CsmePlatId, MAX_PLAT_ID_SIZE);
  }

  return Status;
}

/**
  This message is sent by the BIOS to read the current Platform Id Refurbish Counter

  @param[out] PlatformIdRefurbishCounter  Current value of the Refurbish Counter


  @retval EFI_SUCCESS                     Command succeeded
  @retval EFI_UNSUPPORTED                 Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER           Invalid input parameter
  @retval EFI_DEVICE_ERROR                HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                     HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL            Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetUpidRefurbishCounter (
  OUT UINT8                       *PlatformIdRefurbishCounter
  )
{
  EFI_STATUS                      Status;
  GET_REFURBISH_COUNTER_BUFFER    GetRefurbishCounter;
  UINT32                          MeMode;
  UINT32                          RecvLength;

  if (PlatformIdRefurbishCounter == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&GetRefurbishCounter, sizeof (GET_REFURBISH_COUNTER_BUFFER));
  GetRefurbishCounter.Request.UpidHeader.Fields.Feature = UpidFeaturePlatId;
  GetRefurbishCounter.Request.UpidHeader.Fields.Command = PlatIdGetRefurbishCounter;
  GetRefurbishCounter.Request.UpidHeader.Fields.Length  = UPID_MESSAGE_BYTE_COUNT_MAP (GET_REFURBISH_COUNTER_REQUEST);
  RecvLength                                            = sizeof (GET_REFURBISH_COUNTER_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_UPID_MESSAGE_ADDR,
             (UINT32 *) &GetRefurbishCounter.Request,
             (UINT32) sizeof (GET_REFURBISH_COUNTER_REQUEST),
             (UINT32 *) &GetRefurbishCounter.Response,
             &RecvLength
             );

  if (EFI_ERROR (Status)) {
    return Status;
  } else if (GetRefurbishCounter.Response.Status != UpidStatusSuccess) {
    DEBUG ((DEBUG_ERROR, "%a: UpidStatus Error, Status: %x\n", __FUNCTION__, GetRefurbishCounter.Response.Status));
    Status = EFI_DEVICE_ERROR;
  } else {
    *PlatformIdRefurbishCounter = GetRefurbishCounter.Response.RefurbishCounter;
  }

  return Status;
}

/**
  This message is sent by the BIOS to update the OEM Platform Id

  @param[in] OemPlatformIdType     Determines format of the OemPlatformId field
  @param[in] OemPlatformId         OEM Platform Id to be set, up to 32 bytes
  @param[in] ManufacturingType     Manufacturing event

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL     Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciUpdateUpidOemPlatformId (
  IN UINT32                     OemPlatformIdType,
  IN UINT8                      *OemPlatformId,
  IN UINT32                     ManufacturingType
  )
{
  EFI_STATUS                      Status;
  UPDATE_OEM_PLATFORM_ID_BUFFER   UpdateOemPlatformId;
  UINT32                          MeMode;
  UINT32                          RecvLength;

  if (OemPlatformId == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&UpdateOemPlatformId, sizeof (UPDATE_OEM_PLATFORM_ID_BUFFER));
  CopyMem (&UpdateOemPlatformId.Request.OemPlatId, OemPlatformId, MAX_PLAT_ID_SIZE);
  UpdateOemPlatformId.Request.UpidHeader.Fields.Feature = UpidFeaturePlatId;
  UpdateOemPlatformId.Request.UpidHeader.Fields.Command = PlatIdUpdateOemPlatformId;
  UpdateOemPlatformId.Request.UpidHeader.Fields.Length  = UPID_MESSAGE_BYTE_COUNT_MAP (UPDATE_OEM_PLATFORM_ID_REQUEST);
  UpdateOemPlatformId.Request.OemPlatIdType             = OemPlatformIdType;
  UpdateOemPlatformId.Request.ManufacturingType         = ManufacturingType;
  RecvLength                                            = sizeof (UPDATE_OEM_PLATFORM_ID_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_UPID_MESSAGE_ADDR,
             (UINT32 *) &UpdateOemPlatformId.Request,
             (UINT32) sizeof (UPDATE_OEM_PLATFORM_ID_REQUEST),
             (UINT32 *) &UpdateOemPlatformId.Response,
             &RecvLength
             );

  if (EFI_ERROR (Status)) {
    return Status;
  } else if (UpdateOemPlatformId.Response.Status != UpidStatusSuccess) {
    DEBUG ((DEBUG_ERROR, "%a: UpidStatus Error, Status: %x\n", __FUNCTION__, UpdateOemPlatformId.Response.Status));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}
