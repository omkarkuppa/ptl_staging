/** @file
  Event report and processing of USB4 host interface implementation.

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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/Usb4HrSrvLib.h>
#include <Library/CmUtilsLib.h>
#include <Library/Usb4HiRing.h>
#include <Library/Usb4HiCoreDefs.h>
#include <Library/Usb4HiCoreLib.h>
#include "Usb4CtrlPkt.h"

/**
  Check if the two events are duplicated events.

  @param[in] RxEvt1 - Pointer to the first Rx Event for comparison.
  @param[in] RxEvt2 - Pointer to the second Rx Event for comparison.

  @retval TRUE  - The two Rx events are duplicated events.
  @retval FALSE - The two Rx events are different events.
**/
BOOLEAN
IsDuplicatedEvent (
  IN PRX_EVENT    RxEvt1,
  IN PRX_EVENT    RxEvt2
 )
{
  BOOLEAN    Duplicated;

  Duplicated = FALSE;

  if ((RxEvt1 != NULL) && (RxEvt2 != NULL)) {
    if ((RxEvt1->TopologyId.Value == RxEvt2->TopologyId.Value) &&
        (RxEvt1->Pdf == RxEvt2->Pdf) &&
        (RxEvt1->AdpNum == RxEvt2->AdpNum) &&
        (RxEvt1->EventCode == RxEvt2->EventCode) &&
        (RxEvt1->Unplug == RxEvt2->Unplug)) {
        Duplicated = TRUE;
    }
  }

  return Duplicated;
}

/**
  Get an empty RxEvent slot for RxEvent report

  @param[in]  HiCore   - Pointer to USB4 Host Interface Core instance.
  @param[out] EmptyEvt - Pointer of pointer to the returned RxEvent.

  @retval EFI_SUCCESS           - Get an empty RxEvent success.
  @retval EFI_OUT_OF_RESOURCES  - No empty RxEvent available.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
GetEmptyRxEvent (
  IN  PUSB4_HI_CORE    HiCore,
  OUT PRX_EVENT        *EmptyEvt
  )
{
  EFI_STATUS    Status;
  LIST_ENTRY    *EvtNode;
  RX_EVENT      *RxEvt;

  if ((HiCore == NULL) || (EmptyEvt == NULL)) {
    DEBUG ((DEBUG_ERROR, "GetEmptyRxEvent: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Get the empty RxEvent from the free event queue.
  //
  RxEvt  = NULL;
  if (!IsListEmpty (&(HiCore->FreeEvtQueue))) {
    EvtNode = GetFirstNode (&(HiCore->FreeEvtQueue));
    if (EvtNode != NULL) {
      RemoveEntryList (EvtNode);
      RxEvt = GET_RXEVT (EvtNode, Entry);
    } else {
      DEBUG ((DEBUG_INFO, "GetEmptyRxEvent: NULL event node from the non-empty free event queue!\n"));
      ASSERT (0);
    }
  }

  if (RxEvt != NULL) {
    CmZeroMem (RxEvt, sizeof(RX_EVENT));
    *EmptyEvt = RxEvt;
    Status = EFI_SUCCESS;
  } else {
    DEBUG ((DEBUG_ERROR, "GetEmptyRxEvent: No free Rx event for use!\n"));
    Status = EFI_OUT_OF_RESOURCES;
  }

Exit:
  return Status;
}

/**
  Free a RxEvent

  @param[in] HiCore  - Pointer to USB4 host interface core instance.
  @param[in] RxEvent - Pointer to RxEvent to be freed.
**/
VOID
FreeRxEvent (
  IN PUSB4_HI_CORE    HiCore,
  IN PRX_EVENT        RxEvent
  )
{
  if ((HiCore != NULL) && (RxEvent != NULL)) {
    //
    // Insert the free RxEvent to the free event queue instead of releasing memory.
    // This avoids the frequent memory alloc/free operation for hot plug event processing.
    //
    InsertHeadList (&(HiCore->FreeEvtQueue), &(RxEvent->Entry));
  }
}

/**
  Report hot plug event to the internal event queue of USB4 Host Interface Core for processing later.

  @param[in] HiCore     - Pointer to USB4 Host Interface Core.
  @param[in] HotPlugPkt - Pointer to the hot plug packet.

  @retval EFI_SUCCESS           - Report hot plug event to the internal event queue success.
  @retval EFI_OUT_OF_RESOURCES  - The internal event queue is full for inserting event.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
ReportHotplugEvent (
  IN PUSB4_HI_CORE                  HiCore,
  IN PUSB4_HOT_PLUG_EVENT_PACKET    HotPlugPkt
  )
{
  EFI_STATUS    Status;
  PRX_EVENT     RxEvent;
  PRX_EVENT     PreRxEvt;
  LIST_ENTRY    *EvtNode;

  if ((HiCore == NULL) || (HotPlugPkt == NULL)) {
    DEBUG ((DEBUG_ERROR, "ReportHotplugEvent: Invalid parameter (%p, %p)\n", HiCore, HotPlugPkt));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Status = GetEmptyRxEvent (HiCore, &RxEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ReportHotplugEvent: Get empty Rx event slot failure, %r\n", Status));
    goto Exit;
  }

  RxEvent->TopologyId.Id.High = HotPlugPkt->RouteString.Str.High & ROUTE_STRING_HIGH_MASK;
  RxEvent->TopologyId.Id.Low  = HotPlugPkt->RouteString.Str.Low & ROUTE_STRING_LOW_MASK;
  RxEvent->AdpNum             = (UINT8)HotPlugPkt->AdpNum;
  RxEvent->Unplug             = (UINT8)HotPlugPkt->Unplug;
  RxEvent->Pdf                = PDF_HOT_PLUG;
  RxEvent->EventCode          = 0;

  //
  // Ignore event if it's a duplicated hot plug event comparing to previous un-processed event in Rx event queue.
  //
  if (!IsListEmpty (&(HiCore->RxEvtQueue))) {
    EvtNode = GetPreviousNode (&(HiCore->RxEvtQueue), &(HiCore->RxEvtQueue));
    if ((EvtNode != NULL) && (EvtNode != &(HiCore->RxEvtQueue))) {
      PreRxEvt = GET_RXEVT (EvtNode, Entry);
    } else {
      PreRxEvt = NULL;
    }
    if ((PreRxEvt != NULL) && IsDuplicatedEvent (RxEvent, PreRxEvt)) {
      DEBUG ((DEBUG_INFO, "ReportHotplugEvent: Duplicated event - Rt 0x%016llX Adp %d Unplug %d\n", RxEvent->TopologyId, RxEvent->AdpNum, RxEvent->Unplug));
      FreeRxEvent (HiCore, RxEvent);
      goto Exit;
    }
  }

  DEBUG ((DEBUG_INFO, "ReportHotplugEvent: Rt 0x%016llX Adp %d Unplug %d\n", RxEvent->TopologyId, RxEvent->AdpNum, RxEvent->Unplug));
  InsertTailList (&(HiCore->RxEvtQueue), &(RxEvent->Entry));

Exit:
  return Status;
}

/**
  Query Rx Ring for Rx event.

  @param[in]  HiCore     - Pointer to USB4 host interface core instance.
  @param[out] RxEvent    - Pointer to the received Rx Event.

  @retval EFI_SUCCESS           - A Rx event is received.
  @retval EFI_NOT_FOUND         - No Rx event is received.
  @retval EFI_UNSUPPORTED       - Fail to query Rx event for unexpected error.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4HiQueryRxEvent (
  IN  USB4_HI_CORE    *HiCore,
  OUT RX_EVENT        *RxEvent
  )
{
  EFI_STATUS                     Status;
  USB4_HI_RING_BUF               *Ring;
  USB4_RING_DESC                 *RxDesc;
  UINT32                         ProdConsReg;
  UINT32                         Cons;
  UINT32                         Prod;
  UINT8                          ReceivedPdf;
  UINT32                         *PktDws;
  PUSB4_NOTIFICATION_PACKET      NotificationPkt;
  PUSB4_HOT_PLUG_EVENT_PACKET    HotPlugPkt;
  PROUTE_STRING                  RouteStr;
  PRX_EVENT                      RxEvt;
  LIST_ENTRY                     *EvtNode;


  if ((HiCore == NULL) || (RxEvent == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4HiQueryRxEvent: Invalid parameter (%p,%p)\n", HiCore, RxEvent));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Check if any Rx event in the internal Rx event queue first
  //
  if (!IsListEmpty (&(HiCore->RxEvtQueue))) {
    EvtNode = GetFirstNode (&(HiCore->RxEvtQueue));
    if (EvtNode != NULL) {
      RxEvt = GET_RXEVT (EvtNode, Entry);
      RemoveEntryList (&(RxEvt->Entry));
      CmCopyMem (RxEvent, RxEvt, sizeof(RX_EVENT));
      FreeRxEvent (HiCore, RxEvt);
      Status = EFI_SUCCESS;
      goto Exit;
    } else {
      DEBUG ((DEBUG_ERROR, "Usb4HiQueryRxEvent: RxEvtQueue is not empty, but the first node returns NULL!\n"));
      Status = EFI_UNSUPPORTED;
      ASSERT (0);
    }
  }

  Ring = HiCore->Ring0.RingBuf;

  //
  // Query Rx Ring if no Rx event is available in the internal Rx event queue.
  //
  Status = HrSrvMmioRead (HiCore->HrContext, REG_RX_RING_BASE + REG_RING_CONS_PROD_OFFSET, 1, &ProdConsReg);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HiQueryRxEvent: MMIO read failure, %r\n", Status));
    goto Exit;
  }

  Cons = (ProdConsReg & REG_RING_CONS_MASK) >> REG_RING_CONS_SHIFT;
  Prod = (ProdConsReg & REG_RING_PROD_MASK) >> REG_RING_PROD_SHIFT;
  DEBUG ((DEBUG_VERBOSE, "Usb4HiQueryRxEvent: HiCore = %p, Cons = %d, Prod = %d\n", HiCore, Cons, Prod));

  if (USB4_RX_RING_EMPTY (Prod, Cons, USB4_RX_RING_BUFS)) {
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  CmZeroMem (RxEvent, sizeof (RX_EVENT));

  Cons = (Cons + 1) % USB4_RX_RING_BUFS;
  RxDesc = &(Ring->RxBufDesc[Cons]);
  if (RxDesc->Attribs & DESC_ATTR_DESC_DONE) {
    ReceivedPdf = (UINT8)((RxDesc->Attribs & DESC_ATTR_EOF_MASK) >> DESC_ATTR_EOF_SHIFT);
    DEBUG ((DEBUG_INFO, "Usb4HiQueryRxEvent: HiCore = %p, Received pdf = 0x%x msg_len = 0x%x. Cons = %d\n",
           HiCore,
           ReceivedPdf,
           RxDesc->Attribs & DESC_ATTR_LEN_MASK,
           Cons));
    RxEvent->Pdf = ReceivedPdf;
    PktDws = (UINT32 *)(Ring->RxBuf[Cons]);
    CmSwapEndianness (PktDws, (RxDesc->Attribs & DESC_ATTR_LEN_MASK)/4, PktDws);

    RouteStr = (PROUTE_STRING)PktDws;
    RxEvent->TopologyId.Id.High = RouteStr->Str.High & ROUTE_STRING_HIGH_MASK;
    RxEvent->TopologyId.Id.Low  = RouteStr->Str.Low & ROUTE_STRING_LOW_MASK;

    if (ReceivedPdf == PDF_HOT_PLUG) {
      HotPlugPkt = (PUSB4_HOT_PLUG_EVENT_PACKET)PktDws;
      DEBUG ((DEBUG_INFO, "Usb4HiQueryRxEvent: Hot Plug Packet - RouteString = 0x%08X%08X, AdpNum = %d, Unplug = %d\n",
              HotPlugPkt->RouteString.Str.High,
              HotPlugPkt->RouteString.Str.Low,
              HotPlugPkt->AdpNum,
              HotPlugPkt->Unplug
              ));
      Status = SendNotification (
                 HiCore,
                 &(RxEvent->TopologyId),
                 (UINT8) HotPlugPkt->AdpNum,
                 USB4_HOTPLUG_ACK,
                 (HotPlugPkt->Unplug) ? PG_HOT_UNPLUG_EVENT : PG_HOT_PLUG_EVENT
                 );
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "Usb4HiQueryRxEvent: Send Hotplug Ack failure, %r\n", Status));
        goto Exit;
      }
      RxEvent->AdpNum = (UINT8)HotPlugPkt->AdpNum;
      RxEvent->Unplug = (UINT8)HotPlugPkt->Unplug;
    } else if (ReceivedPdf == PDF_NOTIFICATION_PACKET) {
      NotificationPkt = (PUSB4_NOTIFICATION_PACKET)PktDws;
      DEBUG ((DEBUG_ERROR, "Usb4HiQueryRxEvent: USB4 Notification Packet - RouteString = 0x%08X%08X, AdpNum = %d, event = %d\n",
              NotificationPkt->RouteString.Str.High,
              NotificationPkt->RouteString.Str.Low,
              NotificationPkt->AdpNum,
              NotificationPkt->EventCode));
      RxEvent->AdpNum = (UINT8)NotificationPkt->AdpNum;
      RxEvent->EventCode = (UINT8)NotificationPkt->EventCode;

      //
      // Handle notification event based on event code
      // Some notification events require CM sends Notification Ack
      // Otherwise, Router will re-transmit the notification packet
      //
      HandleNotificationEvent (HiCore, NotificationPkt);
    } else {
      DEBUG ((DEBUG_INFO, "Usb4HiQueryRxEvent: Packet with unexpected PDF %d is received!\n", ReceivedPdf));
    }
    Status = EFI_SUCCESS;
    FreeRxDesc (HiCore, Ring->RxBufDesc, Cons);
  } else {
    Status = EFI_NOT_FOUND;
  }

Exit:
  return Status;
}

/**
  Initialize the event queues of USB4 Host Interface Core.

  @param[in] HiCore             - Pointer to USB4 host interface core instance.

  @retval EFI_SUCCESS           - Initialize the event queue successfully.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HiCoreEvtQueueInit (
  IN PUSB4_HI_CORE    HiCore
  )
{
  EFI_STATUS    Status;
  UINT32        Index;
  RX_EVENT      *RxEvent;

  if (HiCore != NULL) {
    InitializeListHead (&(HiCore->RxEvtQueue));
    InitializeListHead (&(HiCore->FreeEvtQueue));

    //
    // Insert the Rx events of poll to free Rx event queue.
    //
    for (Index = 0; Index < RX_EVT_MAX; Index++) {
      RxEvent = &(HiCore->RxEvts[Index]);
      InsertTailList (&(HiCore->FreeEvtQueue), &(RxEvent->Entry));
    }
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_INVALID_PARAMETER;
  }
  return Status;
}

/**
  Uninitialize the event queues of USB4 Host Interface Core.

  @param[in] HiCore - Pointer to USB4 Host Interface Core instance.
**/
VOID
HiCoreEvtQueueUninit (
  IN PUSB4_HI_CORE    HiCore
  )
{
  if (HiCore != NULL) {
    InitializeListHead (&(HiCore->RxEvtQueue));
    InitializeListHead (&(HiCore->FreeEvtQueue));
  }
}