/** @file
  DXE phase asynchronous communication helper functions.

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

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HeciAsyncSupportLib.h>
#include <Library/DxeMeLib.h>
#include <Protocol/HeciProtocol.h>

#define HECI_ASYNC_TIMER EFI_TIMER_PERIOD_MILLISECONDS(200)

typedef struct {
  HECI_PROTOCOL  Heci;
  LIST_ENTRY     Queue;
  EFI_EVENT      AsyncMsgTimer;
} HECI_ASYNC_COMM;

/**
  Function starts the asynchronous event timer.
**/
VOID
StartAsyncTimer (
  VOID
  )
{
  EFI_STATUS               Status;
  HECI_PROTOCOL            *Heci;
  HECI_ASYNC_COMM          *Private;

  Status = gBS->LocateProtocol (&gHeciProtocolGuid, NULL, (VOID **) &Heci);
  if (EFI_ERROR (Status)) {
    return;
  }

  Private = (HECI_ASYNC_COMM *) Heci;

  if (IsListEmpty (&Private->Queue)) {
    return;
  }

  Status = gBS->SetTimer (
                  Private->AsyncMsgTimer,
                  TimerPeriodic,
                  HECI_ASYNC_TIMER
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Heci Set Periodic Timer Error, Status = %r\n", Status));
  }
  return;
}

/**
  Function stops the asynchronous event timer.
**/
VOID
StopAsyncTimer (
  VOID
  )
{
  EFI_STATUS               Status;
  HECI_PROTOCOL            *Heci;
  HECI_ASYNC_COMM          *Private;

  Status = gBS->LocateProtocol (&gHeciProtocolGuid, NULL, (VOID **) &Heci);
  if (EFI_ERROR (Status)) {
    return;
  }

  Private = (HECI_ASYNC_COMM *) Heci;

  if (IsListEmpty (&Private->Queue)) {
    return;
  }

  Status = gBS->SetTimer (
                  Private->AsyncMsgTimer,
                  TimerCancel,
                  0
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Heci Set Periodic Timer Error, Status = %r\n", Status));
  }
}

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

  @retval EFI_SUCCESS             Message sent and response received
  @retval EFI_OUT_OF_RESOURCES    Out of memory
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
  )
{
  EFI_STATUS               Status;
  HECI_PROTOCOL            *Heci;
  HECI_ASYNC_COMM          *Private;
  HECI_MESSAGE_QUEUE_ENTRY *QueueEntry;

  Status = gBS->LocateProtocol (&gHeciProtocolGuid, NULL, (VOID **) &Heci);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Private = (HECI_ASYNC_COMM *) Heci;

  QueueEntry = AllocateZeroPool (sizeof (HECI_MESSAGE_QUEUE_ENTRY));
  if (QueueEntry == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  QueueEntry->HostAddress          = HostAddress;
  QueueEntry->MeAddress            = MeAddress;

  QueueEntry->MessageBuffer = AllocateZeroPool (Length);
  if (QueueEntry->MessageBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (QueueEntry->MessageBuffer, Message, Length);

  QueueEntry->MessageLength        = Length;
  QueueEntry->ReceiveMessageBuffer = RecvMessageBuffer;
  QueueEntry->ReceiveMessageLength = RecvLength;
  QueueEntry->Event                = Event;

  InsertTailList (&Private->Queue, &QueueEntry->Link);

  return EFI_SUCCESS;
}

/**
  Function checks if the given message is the queued asynchronous message response

  @param[in] HostAddress   Current message Host Client Address
  @param[in] MeAddress     Current message ME Client Address
  @param[in] MessageBody   Current message buffer
  @param[in] HostAddress   Queued asynchronous message Host Client Address
  @param[in] MeAddress     Queued asynchronous message ME Client Address
  @param[in] QueueMessage  Queued asynchronous message request buffer

  @retval TRUE             Current message is an asynchronous response
  @retval FALSE            Current message is not an asynchronous response
**/
BOOLEAN
STATIC
IsHeciAsyncMessage (
  IN UINT8                 PacketHostAddress,
  IN UINT8                 PacketMeAddress,
  IN UINT32                *MessageBody,
  IN UINT8                 QueueHostAddress,
  IN UINT8                 QueueMeAddress,
  IN UINT32                *QueueMessage
  )
{
  if ((PacketHostAddress != QueueHostAddress ||
      PacketMeAddress != QueueMeAddress) &&
      (QueueMeAddress != 0 && QueueHostAddress != 0)) {
    return FALSE;
  }

  switch (QueueMeAddress) {
    case HECI_MKHI_MESSAGE_ADDR:
    case HECI_MCHI_MESSAGE_ADDR:
      if (((MKHI_MESSAGE_HEADER *)MessageBody)->Fields.Command == ((MKHI_MESSAGE_HEADER *)QueueMessage)->Fields.Command &&
        ((MKHI_MESSAGE_HEADER *)MessageBody)->Fields.GroupId == ((MKHI_MESSAGE_HEADER *)QueueMessage)->Fields.GroupId) {
        return TRUE;
      }
      break;
    case HECI_HBM_MESSAGE_ADDR:
      if (((HBM_FLOW_CONTROL *)MessageBody)->MeAddress == ((HBM_FLOW_CONTROL *)QueueMessage)->MeAddress) {
        return TRUE;
      }
      break;
    default:
      return FALSE;
    }

  return FALSE;
}

/**
  Function checks for asynchronous read

  @param[in] HostAddress   Current message Host Client Address
  @param[in] MeAddress     Current message ME Client Address
  @param[in] MessageBody   Current message buffer

  @retval TRUE             Asynchronous read detected
  @retval FALSE            Asynchronous read not detected
**/
BOOLEAN
IsHeciAsyncRead (
  IN UINT8                 HostAddress,
  IN UINT8                 MeAddress,
  IN UINT32                *MessageBody
  )
{
  EFI_STATUS               Status;
  HECI_PROTOCOL            *Heci;
  HECI_ASYNC_COMM          *Private;
  LIST_ENTRY               *ListEntry;
  HECI_MESSAGE_QUEUE_ENTRY *QueueEntry;

  Status = gBS->LocateProtocol (&gHeciProtocolGuid, NULL, (VOID **) &Heci);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Private = (HECI_ASYNC_COMM *) Heci;

  if (!IsListEmpty (&Private->Queue)) {
    ListEntry = GetFirstNode (&Private->Queue);
    while (!IsNull (&Private->Queue, ListEntry)) {
      QueueEntry = (HECI_MESSAGE_QUEUE_ENTRY*) ListEntry;

      if (IsHeciAsyncMessage (HostAddress, MeAddress, MessageBody, QueueEntry->HostAddress, QueueEntry->MeAddress, QueueEntry->MessageBuffer)) {
        CopyMem (QueueEntry->ReceiveMessageBuffer, MessageBody, QueueEntry->ReceiveMessageLength);
        FreePool (QueueEntry->MessageBuffer);
        RemoveEntryList (&QueueEntry->Link);
        DEBUG ((DEBUG_INFO, "Calling asynch event\n"));
        gBS->SignalEvent (QueueEntry->Event);
        FreePool (QueueEntry);
        return TRUE;
      }
      ListEntry = GetNextNode (&Private->Queue, ListEntry);
    }
  }
  return FALSE;
}