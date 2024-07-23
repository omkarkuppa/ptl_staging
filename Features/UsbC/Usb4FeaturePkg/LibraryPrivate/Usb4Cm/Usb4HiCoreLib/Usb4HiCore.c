/** @file
  USB4 host interface core implementation.

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
#include <Library/CmUtilsLib.h>
#include <Library/Usb4HrSrvLib.h>
#include <Library/Usb4HrInst.h>
#include <Library/Usb4HiCoreDefs.h>
#include <Library/Usb4HiRing.h>
#include <Library/Usb4DebugLib.h>
#include "Usb4HiCore.h"
#include "HiCoreEvt.h"
#include "Crc32c.h"

extern EFI_USB4_DEBUG_PROTOCOL   *mUsb4Log;

/**
  Initialize Tx/Rx ring 0 of USB4 host interface.

  @param[in] HiCore - Pointer to USB4 host interface core instance.

  @retval EFI_SUCCESS           - Initialize Tx/Rx ring 0 of USB4 host interface.
  @retval EFI_UNSUPPORTED       - Fail to initialize Tx/Rx ring 0 of USB4 host interface.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HiCoreRing0Init (
  IN USB4_HI_CORE    *HiCore
  )
{
  EFI_STATUS          Status;
  USB4_HI_RING_BUF    *Ring;
  USB4_HI_RING_BUF    *MappedRing;
  UINT32              Index;
  UINT64              PhysAddr;
  UINT32              RegData;

  DEBUG ((DEBUG_INFO, "HiCoreRing0Init entry\n"));

  if (HiCore == NULL) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Init: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Ring = (USB4_HI_RING_BUF *) HiCore->Ring0.RingBuf;
  MappedRing = (USB4_HI_RING_BUF *) HiCore->Ring0.MappedAddr;

  //
  // Configure the TX buffer descriptors
  //
  for (Index = 0; Index < USB4_TX_RING_BUFS; Index++) {
    Ring->TxBufDesc[Index].PhysAddr = (UINT64) (MappedRing->TxBuf[Index]);
    //
    // Need 4 bytes alignment
    //
    if ((Ring->TxBufDesc[Index].PhysAddr & 0x03) != 0) {
      DEBUG ((DEBUG_ERROR, "Tx buffer address 0x%llX is not 4 bytes alignment\n", Ring->TxBufDesc[Index].PhysAddr));
      Status = EFI_UNSUPPORTED;
      ASSERT (0);
      goto Exit;
    }
    //
    // Set Request Status (RS) bit only for buffer descriptor attribute reset
    //
    Ring->TxBufDesc[Index].Attribs = DESC_ATTR_REQ_STS;
  }

  //
  // Configure the RX buffer descriptors
  //
  for (Index = 0; Index < USB4_RX_RING_BUFS; Index++) {
    Ring->RxBufDesc[Index].PhysAddr = (UINT64) (MappedRing->RxBuf[Index]);
    //
    // Need 4 bytes alignment
    //
    if ((Ring->RxBufDesc[Index].PhysAddr & 0x03) != 0) {
      DEBUG ((DEBUG_ERROR, "Rx buffer address 0x%llX is not 4 bytes alignment\n", Ring->RxBufDesc[Index].PhysAddr));
      Status = EFI_UNSUPPORTED;
      ASSERT (0);
      goto Exit;
    }

    //
    // Set Request Status (RS) bit only for buffer descriptor attribute reset
    //
    Ring->RxBufDesc[Index].Attribs = DESC_ATTR_REQ_STS;
  }

  //
  // Configure the TX ring descriptor
  // Writing to ring base registers or ring size register will reset prod/cons index register to 0
  // prod == cons means TX ring empty
  //
  PhysAddr = (UINT64)(MappedRing->TxBufDesc);
  //
  // Ring descriptor needs 16 bytes alignment
  //
  if ((PhysAddr & 0x0F) != 0) {
    DEBUG ((DEBUG_ERROR, "Tx descriptor address 0x%llX is not 16 bytes alignment\n", PhysAddr));
    Status = EFI_UNSUPPORTED;
    ASSERT (0);
    goto Exit;
  }
  Status = HrSrvMmioWrite (HiCore->HrContext, REG_TX_RING_BASE, 2, &PhysAddr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Init: Configure Tx ring base failure, %r\n", Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "Write Tx buf desc addr to offset 0x%x, addr = 0x%ll0x\n", REG_TX_RING_BASE, PhysAddr));

  RegData = (USB4_TX_RING_BUFS << REG_RING_SIZE_SHIFT) & REG_RING_SIZE_MASK;
  Status = HrSrvMmioWrite (HiCore->HrContext, REG_TX_RING_BASE + REG_RING_SIZE_OFFSET, 1, &RegData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Init: Configure Tx ring size failure, %r\n", Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "Write Tx buf number to offset 0x%x, val = 0x%08x\n", REG_TX_RING_BASE + REG_RING_SIZE_OFFSET, RegData));

  //
  // Enable Tx Ring 0 in raw mode
  // Tx Ring 0 shall operate in raw mode only
  //
  RegData = REG_OPTS_RAW | REG_OPTS_VALID;
  Status = HrSrvMmioWrite (HiCore->HrContext, REG_TX_OPTIONS_BASE, 1, &RegData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Init: Configure Tx ring mode failure, %r\n", Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "Write Tx Ring Control - offset 0x%x, value = 0x%08x\n", REG_TX_OPTIONS_BASE, RegData));

  //
  // Configure the RX ring descriptor
  // Writing to ring base registers or ring size register will reset prod/cons index register to 0
  // prod == cons means RX ring full
  // (cons + 1) % ring_size == prod means RX ring empty
  //
  PhysAddr = (UINT64)(MappedRing->RxBufDesc);
  //
  // Ring descriptor needs 16 bytes alignment
  //
  if ((PhysAddr & 0x0F) != 0) {
    DEBUG ((DEBUG_ERROR, "Rx descriptor address 0x%llX is not 16 bytes alignment\n", PhysAddr));
    Status = EFI_UNSUPPORTED;
    ASSERT (0);
    goto Exit;
  }
  Status = HrSrvMmioWrite (HiCore->HrContext, REG_RX_RING_BASE, 2, &PhysAddr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Init: Configure Rx base failure, %r\n", Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "Write Rx buf desc addr to offset 0x%x, addr = 0x%ll0x\n", REG_RX_RING_BASE, PhysAddr));

  RegData = ((USB4_RX_RING_BUFS << REG_RING_SIZE_SHIFT) & REG_RING_SIZE_MASK) |
            ((USB4_FRAME_SIZE_MAX << REG_RING_BUF_SIZE_SHIFT) & REG_RING_BUF_SIZE_MASK);
  Status = HrSrvMmioWrite (HiCore->HrContext, REG_RX_RING_BASE + REG_RING_SIZE_OFFSET, 1, &RegData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Init: Configure Rx ring size failure, %r\n", Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "Write Rx buf number and frame size to offset 0x%x, val = 0x%08x\n",
          REG_RX_RING_BASE + REG_RING_SIZE_OFFSET, RegData));

  //
  // Rewind cons to prod - 1 to indicate Rx ring empty
  //
  RegData = ((USB4_RX_RING_BUFS - 1) << REG_RING_CONS_SHIFT) & REG_RING_CONS_MASK;
  Status = HrSrvMmioWrite (HiCore->HrContext, REG_RX_RING_BASE + REG_RING_CONS_PROD_OFFSET, 1, &RegData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Init: Configure Rx ring ProdCons index failure, %r\n", Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "Write ConsProd index to offset 0x%x, val = 0x%08x\n", REG_RX_RING_BASE + REG_RING_CONS_PROD_OFFSET, RegData));

  //
  // Enable Rx Ring 0 in raw mode
  // Rx Ring 0 shall operate in raw mode only
  //
  RegData = REG_OPTS_RAW | REG_OPTS_VALID;
  Status = HrSrvMmioWrite (HiCore->HrContext, REG_RX_OPTIONS_BASE, 1, &RegData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Init: Configure Rx ring mode failure, %r\n", Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "Write Rx Ring Control - offset 0x%x, value = 0x%08x\n", REG_RX_OPTIONS_BASE, RegData));

  HiCore->State |= HI_CORE_RING0_INIT;
Exit:
  DEBUG ((DEBUG_INFO, "HiCoreRing0Init exit\n"));
  return Status;
}

/**
  Disable Tx/Rx ring 0 of USB4 host interface.

  @param[in] HiCore - Pointer to USB4 host interface core instance.
**/
VOID
HiCoreRing0Uninit (
  IN USB4_HI_CORE    *HiCore
  )
{
  EFI_STATUS    Status;
  UINT32        RegData;

  DEBUG ((DEBUG_INFO, "HiCoreRing0Uninit entry\n"));

  if (HiCore == NULL) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Uninit: Null HiCore\n"));
    goto Exit;
  }

  //
  // Disable Tx Ring 0
  //
  DEBUG ((DEBUG_INFO, "Disable Tx Ring 0\n"));
  Status = HrSrvMmioRead (HiCore->HrContext, REG_TX_OPTIONS_BASE, 1, &RegData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Uninit: Read Tx ring control register failure, %r\n", Status));
    goto Exit;
  }
  RegData &= ~(REG_OPTS_VALID);
  Status = HrSrvMmioWrite (HiCore->HrContext, REG_TX_OPTIONS_BASE, 1, &RegData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Uninit: Write Tx ring control register failure, %r\n", Status));
    goto Exit;
  }

  //
  // Disable Rx Ring 0
  //
  DEBUG ((DEBUG_INFO, "Disable Rx Ring 0\n"));
  Status = HrSrvMmioRead (HiCore->HrContext, REG_RX_OPTIONS_BASE, 1, &RegData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Uninit: Read Rx ring control register failure, %r\n", Status));
    goto Exit;
  }
  RegData &= ~(REG_OPTS_VALID);
  Status = HrSrvMmioWrite (HiCore->HrContext, REG_RX_OPTIONS_BASE, 1, &RegData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Uninit: Write Rx ring control register failure, %r\n", Status));
    goto Exit;
  }

  HiCore->State &= ~(HI_CORE_RING0_INIT|HI_CORE_START);
Exit:
  DEBUG ((DEBUG_INFO, "HiCoreRing0Uninit exit\n"));
}

/**
  Destroy Tx/Rx ring 0 of USB4 host interface.

  @param[in] HiCore - Pointer to USB4 host interface core instance.
**/
VOID
HiCoreRing0Destroy (
  IN USB4_HI_CORE    *HiCore
  )
{
  USB4_HI_RING   *Ring0;

  DEBUG ((DEBUG_INFO, "HiCoreRing0Destroy entry\n"));

  if (HiCore == NULL) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Destroy: Invalid parameter\n"));
    goto Exit;
  }

  //
  // Disable Tx/Rx ring0 to stop all DMA transfers.
  //
  HiCoreRing0Uninit (HiCore);

  //
  // Release DMA buffer of Tx/Rx ring0.
  //
  Ring0 = &(HiCore->Ring0);
  if (Ring0->RingBuf != NULL) {
    HrSrvDmaBufFree (HiCore->HrContext, Ring0->RingBuf, Ring0->BufPages, Ring0->Mapping);
    Ring0->RingBuf    = NULL;
    Ring0->BufPages   = 0;
    Ring0->MappedAddr = 0;
    Ring0->Mapping    = NULL;
  }

Exit:
  DEBUG ((DEBUG_INFO, "HiCoreRing0Destroy exit\n"));
}

/**
  Create Tx/Rx ring 0 of USB4 host interface.

  @param[in] HiCore - Pointer to USB4 host interface core instance.

  @retval EFI_SUCCESS           - Create ring 0 of USB4 host interface successfully.
  @retval EFI_UNSUPPORTED       - Fail to create ring 0 of USB4 host interface.
  @retval EFI_OUT_OF_RESOURCES  - Insufficient resource for creating ring 0.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HiCoreRing0Create (
  IN USB4_HI_CORE    *HiCore
  )
{
  EFI_STATUS      Status;
  USB4_HI_RING    *Ring0;

  DEBUG ((DEBUG_INFO, "HiCoreRing0Create entry\n"));

  if (HiCore == NULL) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Create: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Allocate DMA buffer for Tx/Rx ring 0.
  //
  Ring0 = &(HiCore->Ring0);
  Ring0->BufPages = EFI_SIZE_TO_PAGES (sizeof (USB4_HI_RING_BUF));
  Status = HrSrvDmaBufAlloc (HiCore->HrContext, Ring0->BufPages, (VOID **)&(Ring0->RingBuf), &(Ring0->MappedAddr), &(Ring0->Mapping));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Create: Alloc DMA buffer failure, %r\n", Status));
    Ring0->RingBuf    = NULL;
    Ring0->MappedAddr = 0;
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  //
  // Configure and enable Tx/Rx ring 0.
  //
  Status = HiCoreRing0Init (HiCore);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HiCoreRing0Create: Init Ring0 failure, %r\n", Status));
    goto Exit;
  }

Exit:
  if ((EFI_ERROR (Status)) && (HiCore != NULL)) {
    Usb4LogWrite (mUsb4Log, USB4_DBG_ERROR, USB4_EVT_CODE_RING_ZERO_DESTROY_ERROR, (UINT32) Status, 0);
    HiCoreRing0Destroy (HiCore);
  }
  DEBUG ((DEBUG_INFO, "HiCoreRing0Create exit\n"));
  return Status;
}

/**
  Self-test function for making sure basic operations working fine

  @param[in] HiCore - Pointer to USB4 host interface core instance.

  @retval EFI_SUCCESS           - Self-test success
  @retval EFI_INVALID_PARAMETER - Self-test failure
**/
EFI_STATUS
HiCoreSelfTest (
  IN PUSB4_HI_CORE  HiCore
  )
{
  EFI_STATUS     Status;
  TOPOLOGY_ID    TopologyId;
  UINT16         Index;
  UINT32         RegData;
  USB4_CS_IO     *Usb4CsIo;

  DEBUG ((DEBUG_INFO, "HiCoreSelfTest entry\n"));

  if (HiCore == NULL) {
    DEBUG ((DEBUG_ERROR, "HiCoreSelfTest: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Usb4CsIo = &(HiCore->Usb4CsIo);

  TopologyId.Value = 0;

  DEBUG ((DEBUG_INFO, "Read Router Configuration Space from Host Router:\n"));
  for (Index = 0; Index < 10; Index++) {
    Status = Usb4CsIo->CsRead (Usb4CsIo, &TopologyId, USB4_CS_ROUTER, 0, Index, 1, &RegData, NULL);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "Read USB4 RT CS_%d failure, %r\n", Index, Status));
      goto Exit;
    }
    DEBUG ((DEBUG_INFO, "      USB4 RT CS_%d = 0x%08x\n", Index, RegData));
  }

Exit:
  DEBUG ((DEBUG_INFO, "HiCoreSelfTest exit\n"));
  return Status;
}

/**
  Query USB4_CS_IO associated with USB4 host interface core.

  @param[in]  HiCore   - Pointer to USB4 host interface core instance.
  @param[out] Usb4CsIo - Pointer of pointer to USB4_CS_IO.

  @retval EFI_SUCCESS           - Query USB4_CS_IO successfully.
  @retval EFI_NOT_FOUND         - USB4_CS_IO is not available.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4HiQueryCsIo (
  IN  USB4_HI_CORE    *HiCore,
  OUT USB4_CS_IO      **Usb4CsIo
  )
{
  EFI_STATUS    Status;
  USB4_CS_IO    *CsIo;

  DEBUG ((DEBUG_INFO, "Usb4HiQueryCsIo entry\n"));

  if ((HiCore == NULL) || (Usb4CsIo == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4HiQueryCsIo: Invalid parameter (%p, %p)\n", HiCore, Usb4CsIo));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((HiCore->State & HI_CORE_START) == 0) {
    DEBUG ((DEBUG_ERROR, "Usb4HiQueryCsIo: USB4 host interface is not ready!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  CsIo = &(HiCore->Usb4CsIo);

  //
  // Check if USB4_CS_IO has been initialized.
  //
  if (CsIo->AddRef == NULL) {
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  //
  // Increment reference to indicate the protocol is in use.
  // The caller should call ReleaseRef once the protocol won't be used any more.
  //
  CsIo->AddRef (CsIo);

  *Usb4CsIo = CsIo;
  Status = EFI_SUCCESS;
Exit:
  DEBUG ((DEBUG_INFO, "Usb4HiQueryCsIo exit\n"));
  return Status;
}

/**
  Start USB4 host interface core.
  - Create Ring buffer and enable Tx/Rx Ring0 and release.

  @param[in] HiCore - Pointer to USB4 host interface core instance.
**/
EFI_STATUS
Usb4HiCoreStart (
  IN USB4_HI_CORE    *HiCore
  )
{
  EFI_STATUS    Status;

  DEBUG ((DEBUG_INFO, "Usb4HiCoreStart entry\n"));

  if (HiCore == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4HiCoreStart: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (HiCore->State & (HI_CORE_START | HI_CORE_RING0_INIT)) {
    DEBUG ((DEBUG_ERROR, "Usb4HiCoreStart: Ring buffer has been present!\n"));
    Status = EFI_UNSUPPORTED;
  }

  //
  // Initialize event queue for notification events.
  //
  Status = HiCoreEvtQueueInit (HiCore);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Initialize host interface event queue failure, %r\n", Status));
    goto Exit;
  }

  //
  // Create ring buffer and enable Tx/Rx Ring0.
  //
  Status = HiCoreRing0Create (HiCore);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HiCoreStart: Create ring buffer failure, %r\n", Status));
    goto Exit;
  }

  HiCore->State |= HI_CORE_START;
Exit:
  DEBUG ((DEBUG_INFO, "Usb4HiCoreStart exit\n"));
  return Status;
}

/**
  Destroy USB4 host interface

  @param[in] HiCore - Pointer to USB4 host interface core instance.
**/
VOID
Usb4HiCoreDestroy (
  IN USB4_HI_CORE    *HiCore
  )
{
  DEBUG ((DEBUG_INFO, "Usb4HiCoreDestroy entry\n"));

  if (HiCore == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4HiCoreDestroy: Invalid parameter\n"));
    goto Exit;
  }

  //
  // Check if all USB_CS_IO_PROTOCOL references have been released.
  //
  if (HiCore->Usb4CsIoRef != 0) {
    DEBUG ((DEBUG_ERROR, "Usb4HiCoreDestroy: Usb4CsIoRef is non-zero!\n"));
    goto Exit;
  }

  //
  // Disable Tx/Rx Ring0 and release Ring buffer.
  //
  HiCoreRing0Destroy (HiCore);

  //
  // Uninit Rx event queue.
  //
  HiCoreEvtQueueUninit (HiCore);

  HiCore->State &= ~HI_CORE_START;

Exit:
  DEBUG ((DEBUG_INFO, "Usb4HiCoreDestroy exit\n"));
}

/**
  Create and initialize USB4 Host Interface for Tx/Rx Ring access.

  @param[in] Usb4Hr - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS           - Initialize USB4 host interface successfully.
  @retval EFI_UNSUPPORTED       - Fail to initialize USB4 host interface.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4HiCoreCreate (
  IN  USB4_HR_INSTANCE    *Usb4Hr
  )
{
  EFI_STATUS       Status;
  PUSB4_HI_CORE    HiCore;

  DEBUG ((DEBUG_INFO, "Usb4HiCoreCreate entry\n"));

  HiCore = NULL;

  if (Usb4Hr == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4HiCoreCreate: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Initialize signature and host router context
  //
  HiCore = &(Usb4Hr->HiCore);
  HiCore->Signature = USB4_HI_CORE_SIGNATURE;
  HiCore->HrContext = Usb4Hr;

  //
  // Initialize event queue for notification events.
  //
  Status = HiCoreEvtQueueInit (HiCore);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HiCoreCreate: Initialize host interface event queue failure, %r\n", Status));
    goto Exit;
  }

  //
  // Create and initialize Tx/Rx Ring0
  //
  Status = HiCoreRing0Create (HiCore);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HiCoreCreate: Create Tx/Rx Ring 0 failure, %r\n", Status));
    goto Exit;
  }

  //
  // Initialize CRC-32C look up table
  //
  Crc32cInit ();

  //
  // Initialize USB4_CS_IO function table for USB4 config space access
  //
  Status = Usb4CsIoInit (&(HiCore->Usb4CsIo));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HiCoreCreate: Initialize USB4_CS_IO failure, %r\n", Status));
    goto Exit;
  }

  //
  // Router config space test
  //
  Status = HiCoreSelfTest (HiCore);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HiCoreCreate: HiCore self test failure, %r\n", Status));
    goto Exit;
  }

  HiCore->State |= HI_CORE_START;

Exit:
  if (EFI_ERROR (Status) && (HiCore != NULL)) {
    Usb4LogWrite (mUsb4Log, USB4_DBG_ERROR, USB4_EVT_CODE_HI_CORE_DESTROY_ERROR, (UINT32) Status, 0);
    Usb4HiCoreDestroy (HiCore);
  }

  DEBUG ((DEBUG_INFO, "Usb4HiCoreCreate exit\n"));
  return Status;
}

