/** @file
  Header file for USB4 control packets transmit and receive.

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

#ifndef _USB4_CTRL_PKT_H_
#define _USB4_CTRL_PKT_H_

#include <Uefi.h>
#include <Library/Usb4HiCoreDefs.h>

/**
  Send the USB4 control packet to the host router Tx ring.

  @param[in] HiCore    - Pointer to the USB4 Host Interface Core instance.
  @param[in] PktBuf    - Pointer to packet buffer.
  @param[in] Pdf       - PDF value.
  @param[in] PktLen    - Packet length in bytes.

  @retval EFI_SUCCESS           - Send control packet successfully.
  @retval EFI_OUT_OF_RESOURCES  - No Tx buffer to send the control packet.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4CtrlPktSend (
  IN PUSB4_HI_CORE    HiCore,
  IN UINT8            *PktBuf,
  IN USB4_CTRL_PDF    Pdf,
  IN UINT16           PktLen
  );

/**
  Receive the USB4 control response packet from the host router Rx ring.

  @param[in]  HiCore         - Pointer to the USB4 Host Interface Core instance.
  @param[out] PktBuf         - Pointer to packet buffer.
  @param[in]  Pdf            - PDF value.
  @param[in]  ExpectedPktLen - Expected response packet length in bytes.

  @retval EFI_SUCCESS           - Send control packet successfully.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_DEVICE_ERROR      - Error notification is received or the received data length is not expected.
  @retval EFI_NOT_FOUND         - ERR_ADDR notification event is received from Rx ring.
  @retval EFI_TIMEOUT           - The expected PDF packet is not received after timeout period.
**/
EFI_STATUS
Usb4CtrlPktRcv (
  IN PUSB4_HI_CORE    HiCore,
  IN UINT8            *PktBuf,
  IN USB4_CTRL_PDF    Pdf,
  IN UINT16           ExpectedPktLen
  );

/**
  Send Notification Packet to ack the events from USB4 Routers.

  @param[in] HiCore       - Pointer to the USB4 Host Interface Core instance.
  @param[in] TopologyId   - Pointer to the Topology ID of the target router.
  @param[in] AdpNum       - Adapter number.
  @param[in] Event        - Notification event code
  @param[in] Plug         - Differentiate Hot Plug and Hot Unplug Events in a Hot Plug Acknowledgment Packet
                            0 = Not a Hot Plug Acknowledgment Packet
                            1 = Reserved
                            2 = Hot Plug Acknowledgment
                            3 = Hot Unplug Acknowledgment

  @retval EFI_SUCCESS           - Notification packet sent success
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_OUT_OF_RESOURCES  - No Tx buffer to send the control packet.
**/
EFI_STATUS
SendNotification (
  IN PUSB4_HI_CORE              HiCore,
  IN PTOPOLOGY_ID               TopologyId,
  IN UINT8                      AdpNum,
  IN USB4_NOTIFICATION_EVENT    Event,
  IN USB4_NOTIFICATION_PG       Plug
  );

/**
  Free Rx buffer descriptor for later use

  @param[in] HiCore    - Pointer to the USB4 Host Interface Core instance.
  @param[in] RxDesc    - Pointer to Rx descriptor ring.
  @param[in] ConsIndex - Index of Rx buffer descriptor that should be released.
**/
VOID
FreeRxDesc (
  IN PUSB4_HI_CORE    HiCore,
  IN USB4_RING_DESC   *RxDesc,
  IN UINT32           ConsIndex
  );

/**
  Handle Notification Event packet based on event code
  Notification events consists of Error Events, Notifications, Protocol Adapter Events.
  Some notification events require Notification Ack from CM.
  Different event status is returned for the event code.
  Error status is returned for Error Events.
  EFI_SUCCESS is returned for Notifications and Protocol Adapter Events.

  Note:
  So far, we don't get the case that further processing is required for notification events except sending Ack.
  Current implementation only sends Notification Ack if necessary and returns event status.
  Notification Ack should be sent as soon as possible whenever CM receives Notification event requiring Ack.
  If the further processing is required for the notification events, report the event to the Rx event queue.
  New event handler needs to be implemented in event library for handling the notification events.
  Only hot plug event handler is implemented in event library now.

  @param[in] HiCore          - Pointer to the USB4 Host Interface Core instance.
  @param[in] NotificationPkt - Pointer to USB4 Notification Packet structure.

  @retval EFI_SUCCESS      - Notification event is not Error Event.
  @retval EFI_NOT_READY    - Notification event is error event ERR_CONN.
  @retval EFI_NOT_FOUND    - Notification event is error event ERR_ADDR.
  @retval EFI_DEVICE_ERROR - Notification event is error event other than ERR_CONN and ERR_ADDR.
  @retval EFI_INVALID_PARAMETER - Invalid function parameter.
**/
EFI_STATUS
HandleNotificationEvent (
  IN USB4_HI_CORE               *HiCore,
  IN USB4_NOTIFICATION_PACKET   *NotificationPkt
  );
#endif
