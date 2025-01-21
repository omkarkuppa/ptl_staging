/** @file
  Header file for event report and processing of USB4 host interface.

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

#ifndef _HI_CORE_EVT_H_
#define _HI_CORE_EVT_H_

#include <Uefi.h>
#include <Library/Usb4HiCoreDefs.h>
#include <Library/Usb4HiCoreLib.h>

/**
  Initialize the event queues of USB4 host interface core instance.

  @param[in] HiCore - Pointer to USB4 Host Interface Core instance.

  @retval EFI_SUCCESS           - Initialize the event queue successfully.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HiCoreEvtQueueInit (
  IN PUSB4_HI_CORE    HiCore
  );

/**
  Uninitialize the event queues of USB4 host interface core instance.

  @param[in] HiCore - Pointer to USB4 Host Interface Core instance.
**/
VOID
HiCoreEvtQueueUninit (
  IN PUSB4_HI_CORE    HiCore
  );

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
  );

/**
  Free a RxEvent

  @param[in] HiCore  - Pointer to USB4 host interface core instance.
  @param[in] RxEvent - Pointer to RxEvent to be freed.
**/
VOID
FreeRxEvent (
  IN PUSB4_HI_CORE    HiCore,
  IN PRX_EVENT        RxEvent
  );

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
 );

/**
  Report hot plug event to the internal event queue of USB4 Host Interface Core for processing later.

  @param[in] HiCore     - Pointer to USB4 host interface core instance.
  @param[in] HotPlugPkt - Pointer to the hot plug packet.

  @retval EFI_SUCCESS           - Report hot plug event to the internal event queue success.
  @retval EFI_OUT_OF_RESOURCES  - The internal event queue is full for inserting event.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
ReportHotplugEvent (
  IN PUSB4_HI_CORE                  HiCore,
  IN PUSB4_HOT_PLUG_EVENT_PACKET    HotPlugPkt
  );

#endif
