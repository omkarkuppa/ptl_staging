/** @file
  HECI Control driver core operations for use in PEI, DXE, and SMM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <PiDxe.h>
#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/HeciAccessCoreLib.h>
#include <Protocol/HeciTransportProtocol.h>
#include <Protocol/HeciControlProtocol.h>
#include <Library/HeciControlCoreLib.h>
#include <Library/HeciControlFilterLib.h>
#include <Library/HeciTransportCoreLib.h>
#include <Library/HeciUtilsLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/MeInfoLib.h>
#include <Register/HeciRegs.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <IndustryStandard/Pci30.h>
#include "HeciControlCore.h"

#define HECI_CONTROL_DEBUG    "[HECI Control]"

/**
  Returns HECI_TRANSPORT pointer based on Engine and HECI devices

  @param[in]     SecurityEngine  Engine to communicate with
  @param[in]     HeciDevice      Specific HECI device to communicate through

  @retval HECI_TRANSPORT *       On success HECI_TRANSPORT pointer is returned
  @retval NULL                   HECI Transport could not be found
**/
HECI_TRANSPORT *
GetHeciTransportInstance (
  IN  UINT32    SecurityEngine,
  IN  UINT32    HeciDevice
  )
{
  HECI_DEVICE_INFO    HeciDeviceInfo;
  HECI_TRANSPORT      *HeciTransport;

  HeciTransport = GetFirstHeciTransport ();

  while (HeciTransport) {
    HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
    if ((HeciDeviceInfo.SecurityEngine == (UINT32) SecurityEngine) && (HeciDeviceInfo.HeciFunction == (UINTN) HeciDevice)) {
      return HeciTransport;
    }

    HeciTransport = HeciTransport->GetNextHeciTransport (HeciTransport);
  }

  return NULL;
}

/**
  Read HECI Firmware status

  This function reads HECI firmware status from given PCH and HECI interface

  @param[in]     This             HeciControl instance
  @param[in]     SecurityEngine   Engine device where HECI is located
  @param[in]     HeciDevice       HECI device to communicate through
  @param[in,out] FirmwareStatus   Pointer to returned firmware status

  @retval EFI_SUCCESS             Returned valid firmware status
  @retval EFI_INVALID_PARAMETER   Input parameters are invalid
  @retval EFI_DEVICE_ERROR        Error occurred
**/
EFI_STATUS
EFIAPI
HeciFirmwareStatus (
  IN OUT HECI_CONTROL_PRIVATE   *This,
  IN     SECURITY_ENGINE        SecurityEngine,
  IN     HECI_DEVICE            HeciDevice,
  IN OUT UINT32                 *FirmwareStatus
  )
{
  HECI_TRANSPORT    *HeciTransport;

  if ((This == NULL) || (FirmwareStatus == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *FirmwareStatus = MAX_UINT32;
  HeciTransport   = GetHeciTransportInstance (SecurityEngine, HeciDevice);

  if (HeciTransport == NULL) {
    return EFI_NOT_FOUND;
  }

  return HeciTransport->GetHeciFirmwareStatus (HeciTransport, FirmwareStatus);
}

/**
  Verify if HECI device is available

  @param[in]  HeciTransport   HECI device handle identifies device function will work on

  @retval TRUE                Device is available
  @retval FALSE               Device is not available
**/
BOOLEAN
EFIAPI
VerifyHeciAvailability (
  IN  HECI_TRANSPORT    *HeciTransport
  )
{
  HECI_DEVICE_INFO    HeciDeviceInfo;
  UINT64              HeciPciCfgAddr;
  UINT16              VendorId;

  if (HeciTransport == NULL) {
    DEBUG ((DEBUG_ERROR, "%a () - NULL pointer passed!\n", __FUNCTION__));
    return FALSE;
  }

  HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
  HeciPciCfgAddr = PCI_SEGMENT_LIB_ADDRESS (
                     HeciDeviceInfo.HeciSegment,
                     HeciDeviceInfo.HeciBus,
                     HeciDeviceInfo.HeciDevice,
                     HeciDeviceInfo.HeciFunction,
                     0
                     );

  VendorId = PciSegmentRead16 (HeciPciCfgAddr + PCI_VENDOR_ID_OFFSET);
  if (VendorId == MAX_UINT16) {
    DEBUG ((DEBUG_WARN, HECI_CONTROL_DEBUG));
    DEBUG ((DEBUG_WARN, " %a () - Device disabled!\n", __FUNCTION__));
    return FALSE;
  }

  return TRUE;
}

/**
  Initialize HECI interface.

  This function initializes host side of HECI interface. If timeout is
  greater than zero it also waits until ME is ready to receive messages.

  @param[in]  This                HeciControl instance
  @param[in]  HeciTransport       HECI device handle identifies device function will work on

  @retval EFI_INVALID_PARAMETER   Input parameters are invalid
  @retval EFI_DEVICE_ERROR        Device not present or HECI MBar can't be initialized
  @retval EFI_SUCCESS             Initialization succeed
**/
EFI_STATUS
EFIAPI
HeciControlInit (
  IN OUT HECI_CONTROL_PRIVATE *This,
  IN     HECI_TRANSPORT       *HeciTransport
  )
{
  UINT32           DefaultTimeout;
  EFI_STATUS       Status;

  Status = EFI_DEVICE_ERROR;

  if (HeciTransport == NULL) {
    DEBUG ((DEBUG_ERROR, "%a () - NULL pointer passed!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  if (VerifyHeciAvailability (HeciTransport)) {
    DefaultTimeout = HECI_INIT_TIMEOUT;
    Status = HeciTransport->HeciInit (HeciTransport, &DefaultTimeout);
  }

  return Status;
}

/**
  Initialize defined HECI communication interface (excluding ICH interface).

  @param[in]  This                HeciControl instance
  @param[in]  HeciTransport       HeciTransport instance

  @retval EFI_SUCCESS             Initialization succeed
  @retval EFI_INVALID_PARAMETER   At least one of input parameter is NULL
**/
EFI_STATUS
EFIAPI
HeciControlInitalizeCommunicationDevice (
  IN  HECI_CONTROL_PRIVATE    *This,
  IN  HECI_TRANSPORT          *HeciTransport
  )
{
  HECI_DEVICE_INFO    HeciDeviceInfo;
  EFI_STATUS          Status;

  if (This == NULL || HeciTransport == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
  if (!HeciTransport->GetInitializationState (HeciTransport)) {
    Status = HeciControlInit (This, HeciTransport);
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, HECI_CONTROL_DEBUG));
      DEBUG ((DEBUG_INFO, " HECI %d initialized\n", HECI_DEVICE_TO_NUMBER (HeciDeviceInfo.HeciFunction)));
    }
  } else {
    DEBUG ((DEBUG_INFO, HECI_CONTROL_DEBUG));
    DEBUG ((DEBUG_INFO, " HECI %d already initialized, skipping\n", HECI_DEVICE_TO_NUMBER (HeciDeviceInfo.HeciFunction)));
  }

  return EFI_SUCCESS;
}

/**
  Initialize all defined HECI communication interfaces.

  @param[in]  This    HeciControl instance
**/
VOID
HeciControlInitalizeAllCommunicationDevices (
  IN  HECI_CONTROL_PRIVATE    *This
  )
{
  HECI_TRANSPORT    *HeciTransport;
  EFI_STATUS        Status;

  DEBUG ((DEBUG_INFO, HECI_CONTROL_DEBUG));
  DEBUG ((DEBUG_INFO, " Initialize HECI Interfaces\n"));

  HeciTransport = GetFirstHeciTransport ();
  while (HeciTransport != NULL) {
    Status = HeciControlInitalizeCommunicationDevice (This, HeciTransport);
    if (EFI_ERROR (Status)) {
      break;
    }
    HeciTransport = HeciTransport->GetNextHeciTransport (HeciTransport);
  }
}

/**
  Initialize Specific HECI communication interface.

  @param[in]  This      HeciControl instance
  @param[in]  HeciNum   Heci Number
**/
VOID
HeciControlInitalizeSpecificCommunicationDevice (
  IN  HECI_CONTROL_PRIVATE  *This,
  IN  UINT8                 HeciNum
  )
{
  HECI_TRANSPORT      *HeciTransport;
  HECI_DEVICE_INFO    HeciDeviceInfo;

  DEBUG ((DEBUG_INFO, HECI_CONTROL_DEBUG));
  DEBUG ((DEBUG_INFO, " Initialize HECI %d Interface\n", HECI_DEVICE_TO_NUMBER (HeciNum)));

  HeciTransport = GetFirstHeciTransport ();
  while (HeciTransport != NULL) {
    HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
    if (HeciDeviceInfo.HeciFunction == HeciNum) {
      HeciControlInitalizeCommunicationDevice (This, HeciTransport);
      break;
    }

    HeciTransport = HeciTransport->GetNextHeciTransport (HeciTransport);
  }
}

/**
  Clear HECI Buffer for given HECI transport

  @param[in]  HeciTransport   Pointer to HECI Transport Instance
**/
VOID
HeciBufferClear (
  IN  HECI_TRANSPORT    *HeciTransport
  )
{
  HECI_ACCESS   *HeciAccess;
  UINT32        HeciCsrHost;
  UINT32        Data;
  UINT8         CBLength;

  if (HeciTransport == NULL) {
     return;
  }

  HeciAccess = GetHeciAccess ((HECI_TRANSPORT_PRIVATE *) HeciTransport);
  if (HeciAccess == NULL) {
    return;
  }

  // Read R_ME_MEM_H_CSR register
  HeciAccess->PciIo.Mem.Read (
                          &(HeciAccess->PciIo),
                          EfiPciIoWidthUint32,
                          0,
                          R_ME_MEM_H_CSR,
                          1,
                          &HeciCsrHost
                          );

  // Verify if HECI is ready
  if (GET_READY (HeciCsrHost)) {
    DEBUG ((DEBUG_INFO, "%a (): CSR: 0x%x (Clear H_RDY)\n", __FUNCTION__, HeciCsrHost));
    // Clear HECI Ready bit, to safely clear HECI buffer
    SET_READY (HeciCsrHost, 0);
    HeciAccess->PciIo.Mem.Write (
                            &(HeciAccess->PciIo),
                            EfiPciIoWidthUint32,
                            0,
                            R_ME_MEM_H_CSR,
                            1,
                            &HeciCsrHost
                            );

    // Read R_ME_MEM_H_CSR register again after READY clear
    HeciAccess->PciIo.Mem.Read (
                            &(HeciAccess->PciIo),
                            EfiPciIoWidthUint32,
                            0,
                            R_ME_MEM_H_CSR,
                            1,
                            &HeciCsrHost
                            );
  }

  // Grab Circular Buffer length
  CBLength = (UINT8) ((HeciCsrHost & 0xFF000000) >> 24);
  DEBUG ((DEBUG_INFO, "%a (): CSR: 0x%x (CBLength = %d, H_RDY = %d)\n", __FUNCTION__, HeciCsrHost, CBLength, GET_READY (HeciCsrHost)));
  for (; CBLength > 0; CBLength--) {
    Data = 0;
    HeciAccess->PciIo.Mem.Write (
                            &(HeciAccess->PciIo),
                            EfiPciIoWidthUint32,
                            0,
                            R_ME_MEM_H_CB_WW,
                            1,
                            &Data
                            );
  }
}

/**
  Reset HECI queue.

  This function triggers HECI queue reset. If timeout is greater than zero it
  also waits until ME is ready to receive messages.

  @param[in,out]  This              HeciControl instance
  @param[in]      SecurityEngine    Engine where HECI device is located
  @param[in]      HeciDevice        HECI device that should be reset
  @param[in,out]  Timeout           On input timeout in us, on exit time left

  @retval EFI_SUCCESS               Reset HECI queue succeeded
  @retval EFI_INVALID_PARAMETER     Input parameters are invalid
  @retval EFI_WRITE_PROTECTED       Can't find suitable HECI Transport
  @retval EFI_DEVICE_ERROR          HECI MBar can't be initialized
  @retval EFI_TIMEOUT               Timeout occurs during operation
**/
EFI_STATUS
EFIAPI
HeciControlReset (
  IN OUT  HECI_CONTROL_PRIVATE    *This,
  IN      UINT32                  SecurityEngine,
  IN      UINT32                  HeciDevice,
  IN OUT  UINT32                  *Timeout
  )
{
  EFI_STATUS         Status;
  HECI_TRANSPORT     *HeciTransport;
  HECI_DEVICE_INFO   HeciDeviceInfo;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_UNSUPPORTED;
  HeciTransport = GetHeciTransportInstance (SecurityEngine, HeciDevice);

  if (HeciTransport == NULL) {
    return EFI_WRITE_PROTECTED;
  }

  if (VerifyHeciAvailability (HeciTransport)) {
    HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
    if (HeciTransport->GetInitializationState (HeciTransport)) {
      DEBUG ((DEBUG_INFO, HECI_CONTROL_DEBUG));
      DEBUG ((
        DEBUG_INFO,
        " %a () - Execute reset for HECI %d [%d:%d:%d:%d]\n",
        __FUNCTION__,
        HECI_DEVICE_TO_NUMBER (HeciDeviceInfo.HeciFunction),
        HeciDeviceInfo.HeciSegment,
        HeciDeviceInfo.HeciBus,
        HeciDeviceInfo.HeciDevice,
        HeciDeviceInfo.HeciFunction
        ));

      HeciBufferClear (HeciTransport);
      Status = HeciTransport->HeciReset (HeciTransport, Timeout);
    } else {
      DEBUG ((DEBUG_WARN, HECI_CONTROL_DEBUG));
      DEBUG ((DEBUG_ERROR, " ERROR: %a () - HECI %d is not initialized\n", __FUNCTION__, HECI_DEVICE_TO_NUMBER (HeciDeviceInfo.HeciFunction)));
      return EFI_DEVICE_ERROR;
    }
  }

  return Status;
}

/**
  Write a message to HECI queue, if necessary perform fragmentation.

  This function puts one message to HECI queue. If the message is longer than
  HECI queue size and timeout is greater than zero the function will split
  the message into several packets and wait for space in the queue to send all
  packets/fragments. Each packet has HECI packet header added.

  For send from SMM check if HECI MBARs has been not changed.
  In such case use configured by UEFI FW and restore after transmission
  On the end wait for send completion on ME side
  and reset HECI Q in addition to clean data.

  @param[in,out]  This              HeciControl instance
  @param[in]      SecurityEngine    Engine to communicate with
  @param[in]      HeciDevice        HECI device to communicate through
  @param[in,out]  Timeout           On input timeout in us, on exit time left
  @param[in]      HostAddress       Host address
  @param[in]      EngineAddress     Engined (ME/IE) address
  @param[in]      MessageBody       Body of the message to send
  @param[in]      MessageLength     Length of the message to send in bytes

  @retval EFI_SUCCESS               Message has been sent successfully
  @retval EFI_INVALID_PARAMETER     Input parameters are invalid
  @retval EFI_WRITE_PROTECTED       Can't find suitable HECI Transport
                                    Possible reason: HECI filter active
  @retval EFI_DEVICE_ERROR          HECI MBar can't be initialized
  @retval EFI_TIMEOUT               Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE       Packet is bigger than queue length
**/
EFI_STATUS
EFIAPI
HeciControlSend (
  IN OUT HECI_CONTROL_PRIVATE   *This,
  IN     UINT32                 SecurityEngine,
  IN     UINT32                 HeciDevice,
  IN OUT UINT32                 *Timeout,
  IN     UINT8                  HostAddress,
  IN     UINT8                  EngineAddress,
  IN     UINT32                 *MessageBody,
  IN     UINT32                 MessageLength
  )
{
  EFI_STATUS          Status;
  HECI_TRANSPORT      *HeciTransport;
  HECI_DEVICE_INFO    HeciDeviceInfo;
  UINT32              DefaultTimeout;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = FilterHeciMessage (SecurityEngine, HeciDevice, HostAddress, EngineAddress, MessageBody);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - Could not send this message!\n", __FUNCTION__));
    return Status;
  }

  HeciTransport = GetHeciTransportInstance (SecurityEngine, HeciDevice);
  if (HeciTransport == NULL) {
    return EFI_WRITE_PROTECTED;
  }

  HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);

  DEBUG ((DEBUG_WARN, HECI_CONTROL_DEBUG));
  DEBUG ((
    DEBUG_VERBOSE,
    " %a () - Execute send for HECI %d [%d:%d:%d:%d]\n",
    __FUNCTION__,
    HECI_DEVICE_TO_NUMBER (HeciDeviceInfo.HeciFunction),
    HeciDeviceInfo.HeciSegment,
    HeciDeviceInfo.HeciBus,
    HeciDeviceInfo.HeciDevice,
    HeciDeviceInfo.HeciFunction
    ));

  if (HeciTransport->GetInitializationState (HeciTransport)) {
    if (Timeout == NULL) {
      DefaultTimeout = HECI_SEND_TIMEOUT;
      Timeout = &DefaultTimeout;
    }

    Status = HeciTransport->HeciSend (HeciTransport, Timeout, HostAddress, EngineAddress, MessageBody, MessageLength);
  } else {
    DEBUG ((DEBUG_WARN, HECI_CONTROL_DEBUG));
    DEBUG ((DEBUG_ERROR, " ERROR: %a () - HECI %d is not initialized\n", __FUNCTION__, HECI_DEVICE_TO_NUMBER (HeciDeviceInfo.HeciFunction)));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Read a message from HECI queue, if necessary perform reassembly.

  This function reads one message from HECI queue. If the message is
  fragmented and timeout is greater than zero the function will wait for
  subsequent packets (fragments) until full message is completed.

  For send from SMM check if HECI MBARs has been not changed.
  In such case use configured by UEFI FW and restore after transmission
  On the end reset HECI Q in addition to clean data.

  @param[in,out]  This              HeciControl instance
  @param[in]      SecurityEngine    Engine to communicate with
  @param[in]      HeciDevice        HECI device to communicate through
  @param[in,out]  Timeout           On input timeout in us, on exit time left
  @param[out]     HostAddress       Host address
  @param[out]     EngineAddress     Engine (ME/IE) address
  @param[out]     MessageBody       Body of the message to send
  @param[in,out]  MessageLength     On input buffer size, on exit message body size in bytes

  @retval EFI_SUCCESS               Message has been received successfully
  @retval EFI_INVALID_PARAMETER     Input parameters are invalid
  @retval EFI_WRITE_PROTECTED       Can't find suitable HECI Transport
  @retval EFI_DEVICE_ERROR          HECI MBar can't be initialized
  @retval EFI_TIMEOUT               Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE       Message exceeds buffer size
**/
EFI_STATUS
EFIAPI
HeciControlReceive (
  IN OUT  HECI_CONTROL_PRIVATE    *This,
  IN      UINT32                  SecurityEngine,
  IN      UINT32                  HeciDevice,
  IN OUT  UINT32                  *Timeout,
     OUT  UINT8                   *HostAddress,
     OUT  UINT8                   *EngineAddress,
     OUT  UINT32                  *MessageBody,
  IN OUT  UINT32                  *MessageLength
  )
{
  EFI_STATUS          Status;
  HECI_TRANSPORT      *HeciTransport;
  HECI_DEVICE_INFO    HeciDeviceInfo;
  UINT32              DefaultTimeout;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HeciTransport = GetHeciTransportInstance (SecurityEngine, HeciDevice);

  if (HeciTransport == NULL) {
    return EFI_WRITE_PROTECTED;
  }

  if (HeciTransport->GetInitializationState (HeciTransport)) {
    if (Timeout == NULL) {
      DefaultTimeout = HECI_READ_TIMEOUT;
      Timeout = &DefaultTimeout;
    }

    Status = HeciTransport->HeciReceive (HeciTransport, Timeout, HostAddress, EngineAddress, MessageBody, MessageLength);
  } else {
    HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
    DEBUG ((DEBUG_WARN, HECI_CONTROL_DEBUG));
    DEBUG ((DEBUG_ERROR, " ERROR: %a () - HECI %d is not initialized\n", __FUNCTION__, HECI_DEVICE_TO_NUMBER (HeciDeviceInfo.HeciFunction)));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Send request message to HECI queue, wait for response if needed.

  This function writes one message to HECI queue and - if receive buffer
  was provided and timeout is greater than zero - waits for response message.
  The request can be retried if Retries > 0 (0 means one attempt, no retry),
  but if retries are requested, the ReqMsg and RspMsg pointers must point
  to different buffers. This is required to make sure request is unchanged.
  Size of receive message buffer is given in bytes in (*RspLen) on input.
  On exit (*RspLen) provides the number of bytes written to the message
  buffer. If buffer is too short the message is truncated.

  For send from SMM check if HECI MBARs has been not changed.
  In such case use configured by UEFI FW and restore after transmission
  On the end reset HECI Q in addition to clean data.

  @param[in,out]  This              HeciControl instance
  @param[in]      SecurityEngine    Engine to communicate with
  @param[in]      HeciDevice        HECI device to communicate through
  @param[in,out]  Timeout           On input timeout in us, on exit time left
  @param[in]      Retries           The number of retries requested (0 => no retry)
  @param[in]      HostAddress       Host address
  @param[in]      EngineAddress     Engine (ME/IE) address
  @param[in]      ReqMsg            Request message
  @param[in]      ReqLen            Request message length in bytes
  @param[out]     RspMsg            Buffer for the response message
  @param[in,out]  RspLen            On input buffer size, on exit message, in bytes
  @param[in,out]  SendStatus        Send operation status
  @param[in,out]  ReceiveStatus     Receive operation status

  @retval EFI_SUCCESS               Message request has been completed successfully
  @retval EFI_INVALID_PARAMETER     Input parameters are invalid
  @retval EFI_WRITE_PROTECTED       Can't find suitable HECI Transport
                                    Possible reason: HECI filter active
  @retval EFI_DEVICE_ERROR          HECI MBar can't be initialized
  @retval EFI_TIMEOUT               Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE       Message is too big
  @retval EFI_ABORTED               This message is not supported for given Engine HECI device or in current boot path
**/
EFI_STATUS
EFIAPI
HeciControlSendAndReceive (
  IN OUT  HECI_CONTROL_PRIVATE    *This,
  IN      UINT32                  SecurityEngine,
  IN      UINT32                  HeciDevice,
  IN OUT  UINT32                  *Timeout,
  IN      UINT8                   Retries,
  IN      UINT8                   HostAddress,
  IN      UINT8                   EngineAddress,
  IN      UINT32                  *ReqMsg,
  IN      UINT32                  ReqLen,
     OUT  UINT32                  *RspMsg,
  IN      UINT32                  *RspLen,
  IN OUT  EFI_STATUS              *SendStatus,
  IN OUT  EFI_STATUS              *ReceiveStatus
  )
{
  EFI_STATUS          Status;
  HECI_DEVICE_INFO    HeciDeviceInfo;
  HECI_TRANSPORT      *HeciTransport;
  UINT32              *ReqMsgInternal;
  UINT32              AllocationReqSize;
  UINT32              DefaultTimeout;

  if ((This == NULL) || (RspLen == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  ReqMsgInternal    = NULL;
  AllocationReqSize = 0;

  Status = FilterHeciMessage (SecurityEngine, HeciDevice, HostAddress, EngineAddress, ReqMsg);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - Could not send this message!\n", __FUNCTION__));
    return Status;
  }

  if ((ReqMsg == RspMsg) && (ReqLen > 0)) {
    AllocationReqSize = ((ReqLen / sizeof (UINT32)) + 1) * sizeof (UINT32);
    ReqMsgInternal = HeciAllocateMemory (AllocationReqSize);
    if (ReqMsgInternal != NULL) {
      CopyMem (ReqMsgInternal, ReqMsg, ReqLen);
    } else {
      AllocationReqSize = 0;
      ReqMsgInternal = ReqMsg;
    }
  } else {
    ReqMsgInternal = ReqMsg;
  }

  Status = EFI_UNSUPPORTED;
  HeciTransport = GetHeciTransportInstance (SecurityEngine, HeciDevice);

  if (HeciTransport == NULL) {
    Status = EFI_WRITE_PROTECTED;
  } else {
    HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
    DEBUG ((DEBUG_VERBOSE, HECI_CONTROL_DEBUG));
    DEBUG ((
      DEBUG_VERBOSE,
      " %a () - Execute send and receive for HECI %d [%d:%d:%d:%d]\n",
      __FUNCTION__,
      HECI_DEVICE_TO_NUMBER (HeciDeviceInfo.HeciFunction),
      HeciDeviceInfo.HeciSegment,
      HeciDeviceInfo.HeciBus,
      HeciDeviceInfo.HeciDevice,
      HeciDeviceInfo.HeciFunction
      ));

    if (HeciTransport->GetInitializationState (HeciTransport)) {
      if (Timeout == NULL) {
        DefaultTimeout = HECI_SEND_TIMEOUT + HECI_READ_TIMEOUT;
        Timeout = &DefaultTimeout;
      }

      Status = HeciTransport->HeciSendAndReceive (
                                HeciTransport,
                                Timeout,
                                Retries,
                                HostAddress,
                                EngineAddress,
                                ReqMsgInternal,
                                ReqLen,
                                RspMsg,
                                RspLen,
                                SendStatus,
                                ReceiveStatus
                                );
    } else {
      DEBUG ((DEBUG_WARN, HECI_CONTROL_DEBUG));
      DEBUG ((DEBUG_ERROR, " ERROR: %a () - HECI %d is not initialized\n", __FUNCTION__, HECI_DEVICE_TO_NUMBER (HeciDeviceInfo.HeciFunction)));
      Status = EFI_DEVICE_ERROR;
    }
  }

  if (AllocationReqSize > 0) {
    HeciFreeMemory (ReqMsgInternal);
  }

  return Status;
}

/**
  Initialize HECI_CONTROL protocol functions

  @param[out]     HeciControl HECI control pointer to be initialized
**/
VOID
SetHeciControlFunctions (
  OUT HECI_CONTROL_PRIVATE *HeciControl
  )
{
  if (HeciControl == NULL) {
    DEBUG ((DEBUG_ERROR, HECI_CONTROL_DEBUG));
    DEBUG ((DEBUG_ERROR, " %a - NULL pointer passed!\n", __FUNCTION__));
    return;
  }

  HeciControl->HeciControl.HeciReset          = (HECI_CONTROL_RESET) HeciControlReset;
  HeciControl->HeciControl.HeciSendAndReceive = (HECI_CONTROL_SEND_AND_RECEIVE) HeciControlSendAndReceive;
  HeciControl->HeciControl.HeciSend           = (HECI_CONTROL_SEND) HeciControlSend;
  HeciControl->HeciControl.HeciReceive        = (HECI_CONTROL_RECEIVE) HeciControlReceive;
  HeciControl->HeciControl.HeciFirmwareStatus = (HECI_CONTROL_READ_FIRMWARE_STATUS) HeciFirmwareStatus;
}
