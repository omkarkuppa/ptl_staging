/** @file
  Implementation of USB4 control packets transmit and receive.

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/Usb4HiCoreDefs.h>
#include <Library/Usb4HiRing.h>
#include <Library/Usb4HrSrvLib.h>
#include <Library/CmUtilsLib.h>
#include <Library/Usb4DebugLib.h>
#include "HiCoreEvt.h"
#include "Crc32c.h"

extern EFI_USB4_DEBUG_PROTOCOL   *mUsb4Log;

/**
  Dump Ring Debug Information

  @param[in] RingDbgInfo - Pointer to Ring debug information.
  @param[in] DbgIndex    - The entry index of next debug information
  @param[in] Entries     - Debug information entry count.
**/
VOID
DumpRingDbgInfo (
  IN PRING_DBG_INFO     RingDbgInfo,
  IN UINT8              DbgIndex,
  IN UINT8              Entries
  )
{
  UINT8             Index;
  UINT8             Count;
  PRING_DBG_INFO    Info;

  if ((RingDbgInfo == NULL) || (DbgIndex >= Entries)) {
    DEBUG ((DEBUG_ERROR, "DumpRingDbgInfo: Invalid parameter\n"));
    return;
  }

  DEBUG ((DEBUG_ERROR, "Ring History:\n"));
  Index = (DbgIndex + 1) % Entries;
  for (Count = 0; Count < Entries; Count++) {
    Info = &(RingDbgInfo[Index]);
    DEBUG ((DEBUG_ERROR, "    [%d] Pdf = %d, prod = %d, cons = %d\n", Index, Info->Pdf, Info->Prod, Info->Cons));
    Index = (Index + 1) % Entries;
  }
}

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
  )
{
  if ((HiCore == NULL) || (RxDesc == NULL) || (ConsIndex >= USB4_RX_RING_BUFS)) {
    DEBUG ((DEBUG_ERROR, "FreeRxDesc: Invalid parameter\n"));
  } else {
    //
    // Clear Rx buffer descriptor attributes and update the consumer index register
    //
    DEBUG ((CM_VERBOSE, "      Free Rx Desc %d and update Rx Cons register to %d\n", ConsIndex, ConsIndex));
    RxDesc[ConsIndex].Attribs = DESC_ATTR_REQ_STS;
    HrSrvMmioWrite (HiCore->HrContext, REG_RX_RING_BASE + REG_RING_CONS_PROD_OFFSET, 1, &ConsIndex);
  }
}

/**
  Get a free TX data buffer, which is pointed by the producer index.

  @param[in]  HiCore      - Pointer to the USB4 host interface core instance.
  @param[out] ProdIndex   - Current producer index of TX ring.
  @param[out] TxBuf       - Pointer of pointer to Tx buffer which is pointed by the producer index

  @retval EFI_SUCCESS      - Get Tx buffer successfully.
  @retval EFI_TIMEOUT      - Timeout failure for waiting free Tx buffer.
  @retval EFI_UNSUPPORTED  - Fail to get free Tx buffer.
  @retval EFI_DEVICE_ERROR - Invalid register value read from host interface.
**/
EFI_STATUS
GetTxBuf (
  IN  PUSB4_HI_CORE    HiCore,
  OUT UINT16           *ProdIndex,
  OUT VOID             **TxBuf
  )
{
  EFI_STATUS    Status;
  UINT32        ProdConsReg;
  UINT32        Prod;
  UINT32        Cons;
  UINT8         DbgIndex;
  UINT8         Retry;

  if ((HiCore == NULL) || (ProdIndex == NULL) || (TxBuf == NULL)) {
    DEBUG ((DEBUG_ERROR, "GetTxBuf: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (HiCore->Ring0.RingBuf == NULL) {
    DEBUG ((DEBUG_ERROR, "GetTxBuf: Null DMA buffer\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  for (Retry = 0; Retry < USB4_TX_RING_TOTAL_WAITS; Retry++)
  {
    //
    // Read Tx Ring Producer/Consumer index to find the next available Tx buffer
    //
    Status = HrSrvMmioRead (HiCore->HrContext, REG_TX_RING_BASE + REG_RING_CONS_PROD_OFFSET, 1, &ProdConsReg);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "GetTxBuf: MMIO read failure, %r\n", Status));
      goto Exit;
    }
    Prod = (ProdConsReg & REG_RING_PROD_MASK) >> REG_RING_PROD_SHIFT;
    Cons = (ProdConsReg & REG_RING_CONS_MASK) >> REG_RING_CONS_SHIFT;
    if (Prod >= USB4_TX_RING_BUFS) {
      DEBUG ((DEBUG_ERROR, "GetTxBuf: Prod %u out of range\n", Prod));
      Status = EFI_DEVICE_ERROR;
      goto Exit;
    }

    if (!USB4_TX_RING_FULL (Prod, Cons, USB4_TX_RING_BUFS)) {
      if (Retry != 0) {
        DEBUG ((DEBUG_INFO, "GetTxBuf: Tx Retry = %d\n", Retry));
      }
      break;
    } else {
      if (Retry == 0) {
        DEBUG ((DEBUG_ERROR, "GetTxBuf: TX ring is full (Prod = %d, Cons = %d), wait for available Tx Buffer....\n", Prod, Cons));
        //
        // Update Tx Ring dbginfo before dump
        //
        DbgIndex = HiCore->TxDbgIndex;
        HiCore->TxDbgInfo[DbgIndex].Prod = (UINT16)Prod;
        HiCore->TxDbgInfo[DbgIndex].Cons = (UINT16)Cons;
        DumpRingDbgInfo (&(HiCore->TxDbgInfo[0]), HiCore->TxDbgIndex, TX_DBG_ENTRIES);
      }
    }
    //
    // Wait for available Tx buffer
    //
    gBS->Stall (USB4_TX_RING_POLL_US);
  }

  if (Retry == USB4_TX_RING_TOTAL_WAITS) {
    DEBUG ((DEBUG_ERROR, "GetTxBuf: TX ring is still full after wait loop timeout ....\n"));
    Status = EFI_TIMEOUT;
    goto Exit;
  }

  //
  // Update Tx Ring debuginfo entry
  //
  DbgIndex = HiCore->TxDbgIndex;
  HiCore->TxDbgInfo[DbgIndex].Prod = (UINT16)Prod;
  HiCore->TxDbgInfo[DbgIndex].Cons = (UINT16)Cons;

  *ProdIndex = (UINT16)Prod;
  *TxBuf = (VOID *)(HiCore->Ring0.RingBuf->TxBuf[Prod]);

Exit:
  return Status;
}

/**
  Initiate the frame transfer operation of Tx Ring
  Operation:
    Increment producer index, which will trigger the DMA to fetch first the
    descriptor and then the data.

  @param[in] HiCore   - Pointer to the USB4 Host Interface Core instance.
  @param[in] Pdf       - PDF value that should be put in EOF_PDF field of packet.
  @param[in] MsgLen    - packet length.
  @param[in] ProdIndex - Current producer index.

  @retval EFI_SUCCESS           - Initiate Tx frame transfer successfully.
  @retval EFI_UNSUPPORTED       - The operation can't be supported.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
InitiateTx (
  IN PUSB4_HI_CORE    HiCore,
  IN USB4_CTRL_PDF    Pdf,
  IN UINT16           MsgLen,
  IN UINT16           ProdIndex
  )
{
  EFI_STATUS        Status;
  UINT32            ProdCons;
  UINT32            NewAttributes;

  if ((HiCore == NULL) || (ProdIndex >= USB4_TX_RING_BUFS)) {
    DEBUG ((DEBUG_ERROR, "InitiateTx: Invalid parameter (%p, %d)\n", HiCore, ProdIndex));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (HiCore->Ring0.RingBuf == NULL) {
    DEBUG ((DEBUG_ERROR, "InitiateTx: Null DMA buffer\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DEBUG ((TX_VERBOSE, "      InitiateTx: pdf=0x%x, length=0x%x, prod=0x%x\n", Pdf, MsgLen, ProdIndex));

  //
  // Update Tx descriptor attributes
  //
  NewAttributes = (MsgLen << DESC_ATTR_LEN_SHIFT) | (Pdf << DESC_ATTR_EOF_SHIFT) | DESC_ATTR_REQ_STS;
  HiCore->Ring0.RingBuf->TxBufDesc[ProdIndex].Attribs = NewAttributes;

  //
  // Increment Producer index to initiate Tx operation
  //
  ProdCons = ((ProdIndex + 1) % USB4_TX_RING_BUFS) << REG_RING_PROD_SHIFT;
  HrSrvMmioWrite (HiCore->HrContext, REG_TX_RING_BASE + REG_RING_CONS_PROD_OFFSET, 1, &ProdCons);

  //
  // Increment Tx dbginfo entry index
  //
  HiCore->TxDbgInfo[HiCore->TxDbgIndex].Pdf = (UINT8)Pdf;
  HiCore->TxDbgIndex = (HiCore->TxDbgIndex + 1) % TX_DBG_ENTRIES;
  DEBUG ((TX_VERBOSE, "      InitiateTx: Update Producer register to 0x%x\n", ProdIndex + 1));

  Status = EFI_SUCCESS;

Exit:
  return Status;
}

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
  )
{
  EFI_STATUS    Status;
  UINT16        Prod;
  VOID          *TxBuf;

  if ((HiCore == NULL) || (PktBuf == NULL) || (PktLen > USB4_FRAME_SIZE_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb4CtrlPktSend: Invalid parameter (%p, %p, %d)\n", HiCore, PktBuf, PktLen));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Get the next Tx buffer for data copy
  //
  Status = GetTxBuf (HiCore, &Prod, &TxBuf);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4CtrlPktSend: Get Tx buffer failure, %r\n", Status));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  CmCopyMem (TxBuf, PktBuf, PktLen);

  //
  // Initiate Tx transfer
  //
  Status = InitiateTx (HiCore, Pdf, PktLen, Prod);

Exit:
  return Status;
}

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
  IN USB4_HI_CORE               *HiCore,
  IN TOPOLOGY_ID                *TopologyId,
  IN UINT8                      AdpNum,
  IN USB4_NOTIFICATION_EVENT    Event,
  IN USB4_NOTIFICATION_PG       Plug
  )
{
  EFI_STATUS                   Status;
  UINT32                       Len;
  UINT32                       Crc32c;
  USB4_NOTIFICATION_PACKET     *CtrlNotification;

  if ((HiCore == NULL) || (TopologyId == NULL)) {
    DEBUG ((DEBUG_ERROR, "SendNotification: Invalid Parameter!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((HOTPLUG_VERBOSE, "SendNotification: Topology ID = 0x%016llX, AdpNum = %d, Event = %d, Plug = %d\n",
          TopologyId->Value,
          AdpNum,
          Event,
          Plug
          ));

  CtrlNotification = (USB4_NOTIFICATION_PACKET *) &HiCore->CtrlNotification;
  CtrlNotification->RouteString.Str.High = TopologyId->Id.High;
  CtrlNotification->RouteString.Str.Low  = TopologyId->Id.Low;
  CtrlNotification->AdpNum               = AdpNum;
  CtrlNotification->EventCode            = Event;
  CtrlNotification->Reserved             = 0;
  CtrlNotification->Plug                 = Plug;

  //
  // Swap byte endianess of each DWORD of the notification packet excluding CRC before transmit
  //
  Len = sizeof (USB4_NOTIFICATION_PACKET) - 4;
  CmSwapEndianness ((UINT32 *) CtrlNotification, Len / 4, (UINT32 *) CtrlNotification);
  CalCrc32c ((UINT8 *) CtrlNotification, Len, &Crc32c);
  CtrlNotification->Crc = SwapBytes32 (Crc32c);

  // Transmit the notification packet
  Status = Usb4CtrlPktSend (HiCore, (UINT8 *)CtrlNotification, PDF_NOTIFICATION_PACKET, sizeof(USB4_NOTIFICATION_PACKET));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Notification sent failure, %r - RouteString = 0x%016llX, AdpNum = %d, Event = %d, Plug = %d\n",
            Status,
            TopologyId->Value,
            AdpNum,
            Event,
            Plug
            ));
    goto Exit;
  }

Exit:
  return Status;
}

/**
  Send Notification Ack Packet for the notification events that require ACK from CM.

  @param[in] HiCore       - Pointer to the USB4 Host Interface Core instance.
  @param[in] TopologyId   - Pointer to the Topology ID of the target router.

  @retval EFI_SUCCESS           - Notification Ack packet sent success
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_OUT_OF_RESOURCES  - No Tx buffer to send the control packet.
**/
EFI_STATUS
SendNotificationAck (
  IN USB4_HI_CORE           *HiCore,
  IN TOPOLOGY_ID            *TopologyId
  )
{
  EFI_STATUS                      Status;
  UINT32                          Len;
  UINT32                          Crc32c;
  USB4_NOTIFICATION_ACK_PACKET    NotificationAck;

  if ((HiCore == NULL) || (TopologyId == NULL)) {
    DEBUG ((DEBUG_ERROR, "SendNotificationAck: Invalid Parameter!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  NotificationAck.RouteString.Str.High = TopologyId->Id.High;
  NotificationAck.RouteString.Str.Low  = TopologyId->Id.Low;

  //
  // Swap byte endianess of each DWORD of the notification ack packet excluding CRC to calculate CRC
  // Swap byte endianess of CRC as well before packet transmission
  //
  Len = sizeof (USB4_NOTIFICATION_ACK_PACKET) - 4;
  CmSwapEndianness ((UINT32 *) &NotificationAck, Len / 4, (UINT32 *) &NotificationAck);
  CalCrc32c ((UINT8 *) &NotificationAck, Len, &Crc32c);
  NotificationAck.Crc = SwapBytes32 (Crc32c);

  //
  // Transmit the notification ack packet
  //
  Status = Usb4CtrlPktSend (HiCore, (UINT8 *)&NotificationAck, PDF_NOTIFICATION_ACK, sizeof (USB4_NOTIFICATION_ACK_PACKET));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Notification Ack sent failure, %r - Rt 0x%016llX\n", Status, TopologyId->Value));
    goto Exit;
  }
Exit:
  return Status;
}

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
  )
{
  EFI_STATUS     Status;
  EFI_STATUS     AckStatus;
  BOOLEAN        SendAck;
  TOPOLOGY_ID    TopologyId;

  if ((HiCore == NULL) || (NotificationPkt == NULL)) {
    DEBUG ((DEBUG_ERROR, "HandleNotificationEvent: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  SendAck = FALSE;
  switch (NotificationPkt->EventCode) {
    case USB4_ERR_CONN:
      Status = EFI_NOT_READY;
      break;
    case USB4_ERR_ADDR:
      Status = EFI_NOT_FOUND;
      break;

    //
    // Notifications that don't require Notification Ack from CM
    //
    case USB4_ROP_CMPLT:       // Router Operation complete notification (USB4 v2)
    case USB4_POP_CMPLT:       // Port Operation complete notification (USB4 v2)
    case USB4_LINK_RECOVERY:   // Link recovery notification (USB4 v2)
    case USB4_ASYM_LINK:       // Asymmetric link notification (USB4 v2)
      Status = EFI_SUCCESS;
      break;

    //
    // Notifications that require Notification Ack from CM
    //
    case USB4_DP_BW:           // DP bandwidth notification (USB4 v2)
    case USB4_PCIE_WAKE:       // PCIE wake notification (USB4 v2)
    case USB4_DP_CON_CHANGE:   // DP connection change notification (USB4 v2)
    case USB4_DPTX_DISCOVERY:  // DPTX discovery notificaiton (USB4 v2)
      SendAck = TRUE;
      Status = EFI_SUCCESS;
      break;

    //
    // Error events that require notification ack from CM
    //
    case USB4_ERR_LINK:
    case USB4_ERR_HEC:
    case USB4_ERR_FC:
    case USB4_ERR_PLUG:
      SendAck = TRUE;
      Status = EFI_DEVICE_ERROR;
      break;

    //
    // All other error events that don't require notification ack from CM
    //
    default:
      Status = EFI_DEVICE_ERROR;
  }

  if (Status != EFI_SUCCESS) {
    Usb4LogWrite (mUsb4Log, USB4_DBG_ERROR, USB4_EVT_CODE_NOTIFICATION_ERROR, (UINT32) Status, 0);
  }

  //
  // Send Notification Ack for the notification events that require Notification Ack from CM
  //
  if (SendAck) {
    TopologyId.Id.High = NotificationPkt->RouteString.Str.High & ROUTE_STRING_HIGH_MASK;
    TopologyId.Id.Low  = NotificationPkt->RouteString.Str.Low & ROUTE_STRING_LOW_MASK;
    DEBUG ((DEBUG_INFO, "Send Notification Ack to Rt 0x%016llX\n", TopologyId.Value));
    AckStatus = SendNotificationAck (HiCore, &TopologyId);
    if (EFI_ERROR (AckStatus)) {
      DEBUG ((DEBUG_ERROR, "Send Notification Ack to Rt 0x%016llX failure, %r\n", TopologyId.Value, AckStatus));
      goto Exit;
    }
  }

Exit:
  return Status;
}

/**
  Waits for a response with given pdf.

  @param[in]  HiCore    - Pointer to the USB4 Host Interface Core instance.
  @param[in]  Pdf       - PDF of a frame to wait for.
  @param[out] ConsIndex - Consumer index to return.
  @param[out] RxBuf     - Pointer of pointer to Rx buffer containing the received packet data
  @param[out] Length    - Length of the received frame.

  @retval EFI_SUCCESS      - Receive response with give PDF successfully.
  @retval EFI_DEVICE_ERROR - Error event is received from Rx ring.
  @retval EFI_NOT_FOUND    - ERR_ADDR notification event is received from Rx ring.
  @retval EFI_TIMEOUT      - The expected PDF packet is not received after timeout period.
  @retval EFI_NOT_READY    - The adapter is disconnected or disabled.
**/
EFI_STATUS
WaitForResponse (
  IN PUSB4_HI_CORE    HiCore,
  IN USB4_CTRL_PDF    Pdf,
  OUT UINT32          *ConsIndex,
  OUT VOID            **RxBuf,
  OUT UINT16          *Length
  )
{
  EFI_STATUS                     Status;
  USB4_HI_RING_BUF               *Ring;
  USB4_RING_DESC                 *RxDesc;
  UINT32                         ProdConsReg;
  UINT32                         Cons;
  UINT32                         Prod;
  UINT8                          ReceivedPdf;
  UINT32                         Index;
  UINT32                         TotalRxPkts;
  UINT32                         *PktDws;
  TOPOLOGY_ID                    TopologyId;
  PUSB4_NOTIFICATION_PACKET      NotificationPkt;
  PUSB4_HOT_PLUG_EVENT_PACKET    HotPlugPkt;

  DEBUG ((CM_VERBOSE, "      WaitForResponse entry - Expected pdf = 0x%x\n", Pdf));

  if ((HiCore == NULL) || (ConsIndex == NULL) || (RxBuf == NULL) || (Length == NULL)) {
    DEBUG ((DEBUG_ERROR, "WaitForResponse: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Ring = HiCore->Ring0.RingBuf;
  if (Ring == NULL) {
    DEBUG ((DEBUG_ERROR, "WaitForResponse: Null DMA buffer\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = HrSrvMmioRead (HiCore->HrContext, REG_RX_RING_BASE + REG_RING_CONS_PROD_OFFSET, 1, &ProdConsReg);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "WaitForResponse: MMIO read failure, %r\n", Status));
    goto Exit;
  }

  Cons = (ProdConsReg & REG_RING_CONS_MASK) >> REG_RING_CONS_SHIFT;
  Prod = (ProdConsReg & REG_RING_PROD_MASK) >> REG_RING_PROD_SHIFT;
  TotalRxPkts = 0;
  DEBUG ((CM_VERBOSE, "      WaitForResponse: Cons = %d, Prod = %d\n", Cons, Prod));

  //
  // Wait for DD(Descriptor Done) bit of Rx buffer descriptor is set
  // Release the current Rx buffer descriptor and process the next one if the received PDF is not excepted
  // Rx ring empty condition is not checked since DD bit of next Rx buffer won't be set if Rx ring is empty
  //
  do {
    Cons = (Cons + 1) % USB4_RX_RING_BUFS;
    RxDesc = &(Ring->RxBufDesc[Cons]);
    DEBUG ((CM_VERBOSE, "      Wait RxDesc[%d], attributes - 0x%x\n", Cons, RxDesc->Attribs));

    //
    // Wait for DD(Descriptor Done) bit of Rx buffer descriptor is set
    // Rx ring empty condition is not checked since DD bit of next Rx buffer won't be set if Rx ring is empty
    //
    for (Index = 0; Index < USB4_RX_RING_TOTAL_WAITS; Index++) {
      if (RxDesc->Attribs & DESC_ATTR_DESC_DONE) {
        break;
      }
      gBS->Stall (USB4_RX_RING_POLL_US);
    }

    if (Index >= USB4_RX_RING_TOTAL_WAITS) {
      DEBUG ((DEBUG_ERROR, "WaitForResponse: Response packet timeout!\n"));
      DEBUG ((DEBUG_ERROR, "The descriptor address - 0x%x, attributes - 0x%x\n", RxDesc, RxDesc->Attribs));
      Status = EFI_TIMEOUT;
      goto Exit;
    }

    ReceivedPdf = (UINT8)((RxDesc->Attribs & DESC_ATTR_EOF_MASK) >> DESC_ATTR_EOF_SHIFT);
    //
    // Process hot plug or error notification event if the received PDF is not expected
    //
    if (ReceivedPdf != Pdf) {
      PktDws = (UINT32 *)(Ring->RxBuf[Cons]);
      CmSwapEndianness (PktDws, (RxDesc->Attribs & DESC_ATTR_LEN_MASK)/4, PktDws);

      if (ReceivedPdf == PDF_NOTIFICATION_PACKET) {
        //
        // Error notification event is received from Rx ring.
        //
        NotificationPkt = (PUSB4_NOTIFICATION_PACKET)PktDws;
        DEBUG ((DEBUG_ERROR, "USB4 Notification Packet: RouteString = 0x%08X%08X, AdpNum = %d, event = %d\n",
                NotificationPkt->RouteString.Str.High,
                NotificationPkt->RouteString.Str.Low,
                NotificationPkt->AdpNum,
                NotificationPkt->EventCode));

        Usb4LogWrite (mUsb4Log,
                      USB4_DBG_INFO,
                      USB4_EVT_CODE_NOTIFICATION_RT,
                      NotificationPkt->RouteString.Str.High & 0x7FFFFFFF,
                      NotificationPkt->RouteString.Str.Low
                      );
        Usb4LogWrite (mUsb4Log,
                      USB4_DBG_INFO,
                      USB4_EVT_CODE_NOTIFICATION_ADP,
                      NotificationPkt->AdpNum,
                      NotificationPkt->EventCode
                      );

        //
        // Handle the notification event based on the event code
        // Error status is returned if the event code indicates error event
        //
        Status = HandleNotificationEvent (HiCore, NotificationPkt);

        FreeRxDesc (HiCore, Ring->RxBufDesc, Cons);

        //
        // Exit the loop if the notification event code indicates error event
        //
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Error event is received, wait response loop exit\n"));
          goto Exit;
        }
      } else if (ReceivedPdf == PDF_HOT_PLUG) {
        //
        // Hot plug event is received from Rx ring.
        //
        HotPlugPkt = (PUSB4_HOT_PLUG_EVENT_PACKET)PktDws;
        DEBUG ((HOTPLUG_VERBOSE, "Hot Plug Packet: RouteString = 0x%08X%08X, AdpNum = %d, Unplug = %d\n",
                HotPlugPkt->RouteString.Str.High,
                HotPlugPkt->RouteString.Str.Low,
                HotPlugPkt->AdpNum,
                HotPlugPkt->Unplug
                ));
        //
        // Report hot plug event to the internal Rx event queue for processing later.
        // Send the ack notification to Router if the hot plug event is reported successfully.
        // Router re-sends hot plug event if it doesn't receive ACK from CM within notification timeout period.
        //
        Status = ReportHotplugEvent (HiCore, HotPlugPkt);
        if (Status == EFI_SUCCESS) {
          TopologyId.Id.High = HotPlugPkt->RouteString.Str.High & ROUTE_STRING_HIGH_MASK;
          TopologyId.Id.Low  = HotPlugPkt->RouteString.Str.Low & ROUTE_STRING_LOW_MASK;
          Status = SendNotification (
                    HiCore,
                    &TopologyId,
                    (UINT8)HotPlugPkt->AdpNum,
                    USB4_HOTPLUG_ACK,
                    (HotPlugPkt->Unplug) ? PG_HOT_UNPLUG_EVENT : PG_HOT_PLUG_EVENT);
          if (EFI_ERROR(Status)) {
            DEBUG ((DEBUG_ERROR, "Send Hotplug Ack failure, %r\n", Status));
            //
            // Keep processing Rx events
            //
            Status = EFI_SUCCESS;
          }
        } else {
          DEBUG ((DEBUG_ERROR, "Report Hot Plug event to the internal Rx queue failure, %r\n", Status));
          //
          // Keep processing Rx events
          //
          Status = EFI_SUCCESS;
        }
      } else {
        DEBUG ((DEBUG_ERROR, "Packet with unexpected PDF %d is received!\n", ReceivedPdf));
        Usb4LogWrite (mUsb4Log, USB4_DBG_WARN, USB4_EVT_CODE_UNEXPECTED_PDF, (UINT32) ReceivedPdf, 0);
      }

      FreeRxDesc (HiCore, Ring->RxBufDesc, Cons);
    }
    TotalRxPkts++;
  } while ((ReceivedPdf != Pdf) && (TotalRxPkts < MAX_PACKETS_TO_RX_WHILE_WAIT_FOR_RESP));

  if (TotalRxPkts >= MAX_PACKETS_TO_RX_WHILE_WAIT_FOR_RESP) {
    DEBUG ((DEBUG_ERROR, "Timeout waiting for the response with given Pdf 0x%x. Exiting...\n", Pdf));
    Status = EFI_TIMEOUT;
    goto Exit;
  }

  *ConsIndex = Cons;
  *RxBuf     = (VOID *)(Ring->RxBuf[Cons]);
  *Length    = (UINT16)(RxDesc->Attribs & DESC_ATTR_LEN_MASK);

Exit:
  DEBUG ((CM_VERBOSE, "      WaitForResponse exit\n"));
  return Status;
}

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
  )
{
  EFI_STATUS    Status;
  UINT32        Cons;
  UINT16        RcvdLen;
  VOID          *RxBuf;

  if ((HiCore == NULL) || (PktBuf == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4CtrlPktRcv: Invalid parameter (%p, %p)\n", HiCore, PktBuf));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (HiCore->Ring0.RingBuf == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CtrlPktRcv: Null DMA buffer\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }
  //
  // Wait for response packet with same PDF value received
  // Consume Rx buffer until the received packet with same PDF is found
  //
  Status = WaitForResponse (HiCore, Pdf, &Cons, &RxBuf, &RcvdLen);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4CtrlPktRcv: Wait for response packet failure, %r\n", Status));
    goto Exit;
  }

  //
  // Report failure if the received data length is not expected.
  //
  if (RcvdLen != ExpectedPktLen) {
    DEBUG ((DEBUG_ERROR, "Usb4CtrlPktRcv: RcvdPktLen mismatches, exp = 0x%x, rcvd = 0x%x\n", ExpectedPktLen, RcvdLen));
    //
    // Release the returned Rx buffer
    //
    FreeRxDesc (HiCore, HiCore->Ring0.RingBuf->RxBufDesc, Cons);
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  }

  CmCopyMem (PktBuf, RxBuf, RcvdLen);

  //
  // Release the returned Rx buffer
  //
  FreeRxDesc (HiCore, HiCore->Ring0.RingBuf->RxBufDesc, Cons);

Exit:
  return Status;
}