/**@file
    Storage security command protocol mock.

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
#include <setjmp.h>
#include <cmocka.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TcgStorageCoreLib.h>
#include "StorageSecurityProtocolMock.h"

EFI_STORAGE_SECURITY_COMMAND_PROTOCOL  *mSscp = NULL;
UINT8  mSscpBuffer[512];

/**

  Creates ComPacket with EndSession.
  This assumes a start session has already been opened.

  @param  [in/out]    CreateStruct        Structure used to add Endsession
  @param  [in/out]    Size                Describes the size of the entire ComPacket (header and payload). Filled out by function.
  @param  [in]        ComId               ComID for the ComPacket
  @param  [in]        ComIdExtension      Extended ComID for the ComPacket
  @param  [in]        HostSessionId         Host Session ID for the Packet
  @param  [in]        TpSessionId         Tper Session ID for the Packet

**/
TCG_RESULT
EFIAPI
TcgCreateMethodStatusMock (
  UINT32             *Size
  )
{
  TCG_PARSE_STRUCT   ParseStruct;
  TCG_CREATE_STRUCT  CreateStruct;
  UINT16  ParseComId;
  UINT16  ParseComIdExtension;
  TCG_UID           InvokingUID;
  TCG_UID           MethodUID;
  TCG_TOKEN  Tok;

  TcgInitTcgParseStruct (&ParseStruct, &mSscpBuffer[0], sizeof (mSscpBuffer));
  ERROR_CHECK (TcgGetComIds (&ParseStruct, &ParseComId, &ParseComIdExtension));
  TcgGetNextToken (&ParseStruct, &Tok);
  if (Tok.Type == TcgTokenTypeEndOfSession) {
    *Size = sizeof (mSscpBuffer);
    return TcgResultSuccess;
  } else if (Tok.Type == TcgTokenTypeCall) {
    ERROR_CHECK (TcgGetNextTcgUid (&ParseStruct, &InvokingUID));
    ERROR_CHECK (TcgGetNextTcgUid (&ParseStruct, &MethodUID));
  }

  ERROR_CHECK (TcgInitTcgCreateStruct (&CreateStruct, &mSscpBuffer[0], sizeof (mSscpBuffer)));
  ERROR_CHECK (TcgStartComPacket (&CreateStruct, ParseComId, ParseComIdExtension));
  ERROR_CHECK (TcgStartPacket (&CreateStruct, 0x0, 0x0, 0x0, 0x0, 0x0));
  ERROR_CHECK (TcgStartSubPacket (&CreateStruct, 0x0));
  ERROR_CHECK (TcgStartMethodCall (&CreateStruct, InvokingUID, MethodUID));
  ERROR_CHECK (TcgStartParameters (&CreateStruct));
  ERROR_CHECK (TcgAddUINT32 (&CreateStruct, 1));   //RecvHostSessionId
  ERROR_CHECK (TcgAddUINT32 (&CreateStruct, 1));   //TperSessionId
  ERROR_CHECK (TcgEndParameters (&CreateStruct));
  ERROR_CHECK (TcgEndMethodCall (&CreateStruct));
  ERROR_CHECK (TcgEndSubPacket (&CreateStruct));
  ERROR_CHECK (TcgEndPacket (&CreateStruct));
  ERROR_CHECK (TcgEndComPacket (&CreateStruct, Size));

  return TcgResultSuccess;
}

/**
  Send a security protocol command to a device that receives data and/or the result
  of one or more commands sent by SendData.
  Mock the SSCP ReceiveData function

  @param  This                         Indicates a pointer to the calling context.
  @param  MediaId                      ID of the medium to receive data from.
  @param  Timeout                      The timeout, in 100ns units, to use for the execution
                                       of the security protocol command. A Timeout value of 0
                                       means that this function will wait indefinitely for the
                                       security protocol command to execute. If Timeout is greater
                                       than zero, then this function will return EFI_TIMEOUT
                                       if the time required to execute the receive data command
                                       is greater than Timeout.
  @param  SecurityProtocolId           The value of the "Security Protocol" parameter of
                                       the security protocol command to be sent.
  @param  SecurityProtocolSpecificData The value of the "Security Protocol Specific" parameter
                                       of the security protocol command to be sent.
  @param  PayloadBufferSize            Size in bytes of the payload data buffer.
  @param  PayloadBuffer                A pointer to a destination buffer to store the security
                                       protocol command specific payload data for the security
                                       protocol command. The caller is responsible for having
                                       either implicit or explicit ownership of the buffer.
  @param  PayloadTransferSize          A pointer to a buffer to store the size in bytes of the
                                       data written to the payload data buffer.

  @retval EFI_SUCCESS                  The security protocol command completed successfully.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                       data from the device. The PayloadBuffer contains the truncated data.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_INVALID_PARAMETER        The PayloadBuffer or PayloadTransferSize is NULL and
                                       PayloadBufferSize is non-zero.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
EFIAPI
SscpReceiveDataMock (
  IN  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL  *This,
  IN  UINT32                                 MediaId,
  IN  UINT64                                 Timeout,
  IN  UINT8                                  SecurityProtocolId,
  IN  UINT16                                 SecurityProtocolSpecificData,
  IN  UINTN                                  PayloadBufferSize,
  OUT VOID                                   *PayloadBuffer,
  OUT UINTN                                  *PayloadTransferSize
  )
{
  EFI_STATUS Status;
  VOID       *Buffer;
  UINT32     Size;

  Status = mock_type (EFI_STATUS);
  if ((PayloadBuffer == NULL) || (PayloadTransferSize == NULL) || (PayloadBufferSize == 0)) {
    Status = EFI_INVALID_PARAMETER;
  }
  if (Status == EFI_SUCCESS) {
    Buffer = mock_type (VOID *);
    if (Buffer != NULL) { //TCG_SP_SPECIFIC_PROTOCOL_LEVEL0_DISCOVERY
      CopyMem (PayloadBuffer, Buffer, PayloadBufferSize);
    } else if (SecurityProtocolSpecificData != 0x0100) {
      TcgCreateMethodStatusMock (&Size);
      CopyMem (PayloadBuffer, &mSscpBuffer[0], sizeof (mSscpBuffer));
    }
  }

  return Status;
}

/**
  Send a security protocol command to a device.
  Mock the SSCP SendData function

  @param  This                         Indicates a pointer to the calling context.
  @param  MediaId                      ID of the medium to receive data from.
  @param  Timeout                      The timeout, in 100ns units, to use for the execution
                                       of the security protocol command. A Timeout value of 0
                                       means that this function will wait indefinitely for the
                                       security protocol command to execute. If Timeout is greater
                                       than zero, then this function will return EFI_TIMEOUT
                                       if the time required to execute the receive data command
                                       is greater than Timeout.
  @param  SecurityProtocolId           The value of the "Security Protocol" parameter of
                                       the security protocol command to be sent.
  @param  SecurityProtocolSpecificData The value of the "Security Protocol Specific" parameter
                                       of the security protocol command to be sent.
  @param  PayloadBufferSize            Size in bytes of the payload data buffer.
  @param  PayloadBuffer                A pointer to a destination buffer to store the security
                                       protocol command specific payload data for the security
                                       protocol command.

  @retval EFI_SUCCESS                  The security protocol command completed successfully.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_INVALID_PARAMETER        The PayloadBuffer is NULL and PayloadBufferSize is non-zero.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
EFIAPI
SscpSendDataMock (
  IN EFI_STORAGE_SECURITY_COMMAND_PROTOCOL  *This,
  IN UINT32                                 MediaId,
  IN UINT64                                 Timeout,
  IN UINT8                                  SecurityProtocolId,
  IN UINT16                                 SecurityProtocolSpecificData,
  IN UINTN                                  PayloadBufferSize,
  IN VOID                                   *PayloadBuffer
  )
{
  EFI_STATUS Status;
  Status = mock_type (EFI_STATUS);
  if ((PayloadBuffer == NULL) && (PayloadBufferSize != 0)) {
    Status = EFI_INVALID_PARAMETER;
  }
  ZeroMem (mSscpBuffer, sizeof (mSscpBuffer));
  if (PayloadBufferSize <= sizeof (mSscpBuffer)) {
    CopyMem (mSscpBuffer, PayloadBuffer, PayloadBufferSize);
  }

  return Status;
}

/**
  Construct the boot service.

  @retval EFI_BOOT_SERVICES  Point to the boot service.

**/
EFI_STORAGE_SECURITY_COMMAND_PROTOCOL*
EFIAPI
SscpConstructorMock (
  )
{
  if (mSscp == NULL) {
    mSscp = AllocateZeroPool (sizeof (EFI_STORAGE_SECURITY_COMMAND_PROTOCOL));
    mSscp->ReceiveData = SscpReceiveDataMock;
    mSscp->SendData = SscpSendDataMock;
  }
  return mSscp;
}

/**
  Destruct the boot service.

**/
VOID
EFIAPI
SscpDestructorMock (
  )
{
  if (mSscp != NULL) {
    FreePool (mSscp);
    mSscp = NULL;
  }
}
