/** @file
  Header file for USB4 host interface core implementation.

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

#ifndef _USB4_HI_CORE_DEFS_H_
#define _USB4_HI_CORE_DEFS_H_

#include <Uefi.h>
#include <Library/Usb4Common.h>
#include <Library/Usb4ConfigLayer.h>
#include <Library/Usb4ConfigSpace.h>
#include <Library/Usb4CsIo.h>

#pragma pack(push, 1)

//
// Tx/Rx ring descriptor
//
typedef struct {
  UINT64    PhysAddr;
  UINT32    Attribs;
  UINT32    Reserved;
} USB4_RING_DESC;

#pragma pack(pop)

//
// Tx/Rx ring buffer size definition
//
#define USB4_RING_BUF_SIZE    EFI_PAGE_SIZE
#define USB4_FRAME_SIZE_MAX   256
#define USB4_BUF_ENTRY_SIZE   (sizeof (USB4_RING_DESC) + USB4_FRAME_SIZE_MAX)
#define USB4_TX_RING_BUFS     4
#define USB4_RX_RING_BUFS     ((USB4_RING_BUF_SIZE - (USB4_TX_RING_BUFS * USB4_BUF_ENTRY_SIZE)) / USB4_BUF_ENTRY_SIZE)

#define TX_DBG_ENTRIES    (USB4_TX_RING_BUFS * 2)
#define RX_DBG_ENTRIES    (USB4_RX_RING_BUFS * 2)

//
// Tx/Rx ring definition for USB4 host interface implementation
//
typedef struct {
  // Tx ring buffers
  UINT8             TxBuf[USB4_TX_RING_BUFS][USB4_FRAME_SIZE_MAX];
  // Rx ring buffers
  UINT8             RxBuf[USB4_RX_RING_BUFS][USB4_FRAME_SIZE_MAX];
  // Tx ring descriptors
  USB4_RING_DESC    TxBufDesc[USB4_TX_RING_BUFS];
  // Rx ring descriptors
  USB4_RING_DESC    RxBufDesc[USB4_RX_RING_BUFS];
} USB4_HI_RING_BUF, *PUSB4_HI_RING_BUF;

//
// Tx/Rx ring debug information entry
//
typedef struct _RING_DBG_INFO {
  UINT8   Pdf;
  UINT16  Prod;
  UINT16  Cons;
} RING_DBG_INFO, *PRING_DBG_INFO;

//
// DMA buffer information of ring
//
typedef struct _USB4_HI_RING {
  USB4_HI_RING_BUF    *RingBuf;
  UINT32              BufPages;
  PHYSICAL_ADDRESS    MappedAddr;
  VOID *              Mapping;
} USB4_HI_RING, *PUSB4_HI_RING;

#define RX_EVT_MAX    64

//
// Event received from Rx ring
//
typedef struct _RX_EVENT {
  LIST_ENTRY      Entry;
  TOPOLOGY_ID     TopologyId;
  UINT8           AdpNum;
  UINT8           Pdf;
  UINT8           EventCode;
  UINT8           Unplug;
} RX_EVENT, *PRX_EVENT;

#define GET_RXEVT(Addr, Field)  ((PRX_EVENT)((UINT8 *)Addr - (UINTN)(&((PRX_EVENT)0)->Field)))

#define USB4_HI_CORE_SIGNATURE          SIGNATURE_32 ('u', '4', 'h', 'i')
#define USB4_HI_CORE_FROM_CSIO(a)       CR (a, USB4_HI_CORE, Usb4CsIo, USB4_HI_CORE_SIGNATURE)

#define HI_CORE_RING0_INIT    0x00000001
#define HI_CORE_START         0x00000002

//
// Instance for USB4 host interface implementation
//
typedef struct _USB4_HI_CORE{
  UINT32                         Signature;
  // Function table for USB4 config space access
  USB4_CS_IO                     Usb4CsIo;
  // Reference count to USB4_CS_IO function table
  UINT32                         Usb4CsIoRef;
  // Host router context of this host interface instance
  USB4_HR_CONTEXT                HrContext;
  // USB4 Ring0 DMA Buffer
  USB4_HI_RING                   Ring0;
  // Rx event poll
  RX_EVENT                       RxEvts[RX_EVT_MAX];
  // Rx Ring Event queue
  LIST_ENTRY                     RxEvtQueue;
  // Free Event queue
  LIST_ENTRY                     FreeEvtQueue;
  // Packet buffer for Control request / response
  USB4_CONTROL_READ_REQUEST      CtrlReadReq;
  USB4_CONTROL_READ_RESPONSE     CtrlReadRsp;
  USB4_CONTROL_WRITE_REQUEST     CtrlWriteReq;
  USB4_CONTROL_WRITE_RESPONSE    CtrlWriteRsp;
  USB4_NOTIFICATION_PACKET       CtrlNotification;
  // Tx ring debug information - consumer/producer index history
  RING_DBG_INFO                  TxDbgInfo[TX_DBG_ENTRIES];
  UINT8                          TxDbgIndex;
  // Rx Ring debug information - consumer/producer index history
  RING_DBG_INFO                  RxDbgInfo[RX_DBG_ENTRIES];
  UINT8                          RxDbgIndex;
  // Host interface state - init/start
  UINT32                         State;
} USB4_HI_CORE, *PUSB4_HI_CORE;

#endif
