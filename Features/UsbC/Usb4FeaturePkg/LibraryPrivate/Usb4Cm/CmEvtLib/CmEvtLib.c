/** @file
  USB4 event handler implementation.

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
#include <Library/DebugLib.h>
#include <Library/Usb4HiCoreLib.h>
#include <Library/Usb4HrInst.h>
#include <Library/Usb4DebugLib.h>
#include "CmEvtHotPlug.h"

extern EFI_USB4_DEBUG_PROTOCOL   *mUsb4Log;

/**
  CM event handler

  @param[in] Usb4Hr   - Pointer to USB4 host router instance.
  @param[in] RxEvent  - Pointer to RX event.

  @retval EFI_SUCCESS           - Rx event is processed successfully.
  @retval EFI_UNSUPPORTED       - Unexpected process Rx events failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
CmEventHandler (
  IN USB4_HR_INSTANCE    *Usb4Hr,
  IN PRX_EVENT           RxEvent
  )
{
  EFI_STATUS    Status;

  if ((Usb4Hr == NULL) || (RxEvent == NULL)) {
    DEBUG ((DEBUG_ERROR, "CmEventHandler: Invalid parameter (%p, %p)\n", Usb4Hr, RxEvent));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (RxEvent->Pdf == PDF_HOT_PLUG) {
    Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_HOTPLUG_RT, RxEvent->TopologyId.Id.High, RxEvent->TopologyId.Id.Low);
    Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_HOTPLUG_ADP, (UINT32) RxEvent->AdpNum, (UINT32) RxEvent->Unplug);
    Status = CmHotPlugHandler (Usb4Hr, RxEvent);
  } else {
    Status = EFI_UNSUPPORTED;
  }

Exit:
  return Status;
}

/**
  Process Rx events if any pending Rx events are available in the Rx event queue.

  @param[in] Usb4Hr - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS           - Process Rx events successfully.
  @retval EFI_UNSUPPORTED       - Unexpected process Rx events failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
CmProcessRxEvents (
  IN USB4_HR_INSTANCE    *Usb4Hr
  )
{
  EFI_STATUS    Status;
  RX_EVENT      RxEvent;
  UINT32        EvtCount;

  if (Usb4Hr == NULL) {
    DEBUG ((DEBUG_ERROR, "CmProcessRxEvents: Null Usb4Hr\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Status = EFI_SUCCESS;
  EvtCount = 0;

  //
  // Query Rx events until no pending Rx event available.
  //
  while (Usb4HiQueryRxEvent (&(Usb4Hr->HiCore), &RxEvent) == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "RxEvent received - Rt 0x%016llx adp %d Pdf %d Unplug %d\n",
            RxEvent.TopologyId, RxEvent.AdpNum, RxEvent.Pdf, RxEvent.Unplug));
    Status = CmEventHandler (Usb4Hr, &RxEvent);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "CmProcessRxEvents: CmEventHandler failure, %r\n", Status));
    }
    EvtCount++;
    //
    // Stop processing events if the processed event count exceeds the maximum range.
    // Prevent the while loop being stuck by event storm.
    //
    if (EvtCount >= MAX_PROCESSED_RX_EVENTS) {
      break;
    }
  }

Exit:
  return Status;
}
