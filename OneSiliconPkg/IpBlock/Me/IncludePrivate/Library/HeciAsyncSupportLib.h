/** @file
  Definition for Heci Asynchronous communication library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#ifndef _HECI_ASYNC_SUPPORT_LIB_H
#define _HECI_ASYNC_SUPPORT_LIB_H

#include <Uefi.h>

#pragma pack(1)

typedef struct {
  LIST_ENTRY          Link;
  UINT8               HostAddress;
  UINT8               MeAddress;
  UINT32              *MessageBuffer;
  UINT32              MessageLength;
  UINT32              *ReceiveMessageBuffer;
  UINT32              ReceiveMessageLength;
  EFI_EVENT           *Event;
} HECI_MESSAGE_QUEUE_ENTRY;

#pragma pack()

/**
  Function starts the asynchronous event timer.
**/
VOID
StartAsyncTimer (
  VOID
  );

/**
  Function stops the asynchronous event timer.
**/
VOID
StopAsyncTimer (
  VOID
  );

/**
  Function adds one message to the queue holding
  asynchronous messages.
  Caller is responsible for allocating the response buffer and
  registering a response event to handle actions when message ACK is received.

  @param[in] HostAddress          The address of the host processor
  @param[in] MeAddress            Address of the ME subsystem the message is being sent to
  @param[in] Message              Asynchronous message send buffer
  @param[in] Length               Request length of the asynchronous message
  @param[in] RecvMessageBuffer    Pointer to the receive buffer
  @param[in] RecvLength           Response length for the asynchronous message
  @param[in] Event                Pointer to the event called when response message arrives

  @retval EFI_UNSUPPORTED         Function not supported in current phase
**/
EFI_STATUS
AddMessageToQueue (
  IN UINT8                        HostAddress,
  IN UINT8                        MeAddress,
  IN UINT32                       *Message,
  IN UINT32                       Length,
  IN UINT32                       *RecvMessageBuffer,
  IN UINT32                       RecvLength,
  IN EFI_EVENT                    *Event
  );

/**
  Function checks for asynchronous read

  @param[in] HostAddress          Current message Host Client Address
  @param[in] MeAddress            Current message ME Client Address
  @param[in] MessageBody          Current message buffer

  @retval TRUE                    Asynchronous read detected
  @retval FALSE                   Asynchronous read not detected
**/
BOOLEAN
IsHeciAsyncRead (
  IN UINT8                        HostAddress,
  IN UINT8                        MeAddress,
  IN UINT32                       *MessageBody
  );

#endif //_HECI_ASYNC_SUPPORT_LIB_H

