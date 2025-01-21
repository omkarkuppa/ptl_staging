/** @file
  Implements the TBT DMA

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

@par Specification Reference:

**/

#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TbtNvmDrvDma.h>
#include <Library/BaseLib.h>
#include <Library/Usb4PlatformHob.h>
#include <Library/HobLib.h>

/**
  Get TX data buffer, which is pointed by the producer index.

  @param[in]  This    This Pointer to the TBT_DMA instance on which the function is activated
  @param[out] ProdPtr A Pointer to producer index in TX ring

  @retval Pointer to the data buffer

**/
STATIC
VOID*
TbtNvmDrvGetTxBuf (
  IN TBT_DMA *This,
  OUT UINT16 *ProdPtr
  )
{
  UINT32 ProdConsReg;
  UINT32 Prod;
  UINT32 Cons;

  ProdConsReg = This->ReadMmio (This, REG_TX_RING_BASE + REG_RING_CONS_PROD_OFFSET);
  Prod = (ProdConsReg & REG_RING_PROD_MASK) >> REG_RING_PROD_SHIFT;
  Cons = (ProdConsReg & REG_RING_CONS_MASK) >> REG_RING_CONS_SHIFT;
  if (Prod >= FW_RING_NUM_TX_BUFS) {
    DEBUG ((DEBUG_ERROR, "TbtDrvGetTxBuf: DMA is not functional, producer %u out of range\n", Prod));
    return NULL;
  }
  if (TBT_TX_RING_FULL (Prod, Cons, FW_RING_NUM_TX_BUFS)) {
    DEBUG ((DEBUG_ERROR, "TbtDrvGetTxBuf: DMA is not functional, TX ring is full\n"));
    return NULL;
  }
  *ProdPtr = (UINT16)Prod;
  return This->Impl->pSharedMem->txBuf[Prod];
}

/**
  Send TX frame
  Operation:
    Increment producer index, which will trigger the DMA to fetch first the
    descriptor and then the data.

**/
STATIC
TBT_STATUS
TbtNvmDrvInitiateTx (
  IN TBT_DMA        *This,
  IN PDF_VALUE      Pdf,
  IN UINT16         MsgLen,
  IN UINT16         Producer
  )
{
  UINT32      ProdCons;
  UINT32      NewAttributes;

  if (Producer < FW_RING_NUM_TX_BUFS) {
    DEBUG ((DEBUG_VERBOSE, "TbtNvmDrvInitiateTx: Sending packet with pdf=0x%x, length=0x%x, prod=0x%x\n", Pdf, MsgLen, Producer));
    NewAttributes = MsgLen << DESC_ATTR_LEN_SHIFT;
    NewAttributes |= Pdf << DESC_ATTR_EOF_SHIFT;
    NewAttributes |= DESC_ATTR_REQ_STS;
    This->Impl->pSharedMem->txBufDesc[Producer].attributes = NewAttributes;
    ProdCons = ((Producer + 1) % FW_RING_NUM_TX_BUFS) << REG_RING_PROD_SHIFT;
    This->WriteMmio (This, REG_TX_RING_BASE + REG_RING_CONS_PROD_OFFSET, ProdCons);
  } else {
    DEBUG ((DEBUG_ERROR, "The producer - consumer is bigger than ring size!\n"));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  return TBT_STATUS_SUCCESS;
}

STATIC
VOID
TbtNvmDrvClearDescriptor (
  IN TBT_BUF_DESC *DescMem,
  IN UINT32   DescPointer
  )
{
  DescMem[DescPointer].attributes = DESC_ATTR_REQ_STS;    // Request status is set
}

/**
  Prepares the descriptor for later use, increments the consumer

  @param[in] This     A Pointer to the DMA data structure
  @param[in] Cons     Current consumer index

**/
STATIC
VOID
TbtNvmDrvFreeRxDesc (
  IN TBT_DMA  *This,
  IN UINT32   Cons
  )
{
  TbtNvmDrvClearDescriptor (This->Impl->pSharedMem->rxBufDesc, Cons);
  This->WriteMmio (This, REG_RX_RING_BASE + REG_RING_CONS_PROD_OFFSET, Cons);
}

/**
  Waits for a response with given pdf
  Operation:
    Poll on buffer descriptor, where consumer index points to.
    If it is done:
      read pdf
      if pdf != Pdf:
        free descriptor, increment consumer
      else:
        return consumer index
  @param[in]  This        Pointer to the DMA instance on which the function is activated
  @param[in]  Pdf         PDF of a frame to wait for
  @param[out] ConsPtr     Consumer index to return
  @param[out] Length      Length of the received frame
  @retval EFI_SUCCESS in case of success. EFI_NO_RESPONSE indicating no response from DMA, stuck.
**/
STATIC
TBT_STATUS
TbtNvmDrvWaitForRxResponse (
  IN  TBT_DMA    *This,
  IN  PDF_VALUE  Pdf,
  OUT UINT32     *ConsPtr,
  OUT UINT16     *Length
  )
{
  EFI_STATUS             Status;
  UINT32                 ProdConsReg;
  UINT32                 ConsIndex;
  volatile UINT8         ReceivedPdf;
  volatile TBT_BUF_DESC  *RxDescPtr;
  UINT32                 Index;
  UINT32                 TotalRxPkts;

  ProdConsReg = This->ReadMmio (This, REG_RX_RING_BASE + REG_RING_CONS_PROD_OFFSET);
  ConsIndex = (ProdConsReg & REG_RING_CONS_MASK) >> REG_RING_CONS_SHIFT;
  TotalRxPkts = 0;
  // TODO improve this using timer
  do {
    ConsIndex = (ConsIndex + 1) % FW_RING_NUM_RX_BUFS;  // Get consumer pointer from HW
    RxDescPtr = &(This->Impl->pSharedMem->rxBufDesc[ConsIndex]);
    DEBUG ((DEBUG_VERBOSE, "Before: RxDescPtr - 0x%x, attributes - 0x%x, data addr - 0x%x\n", RxDescPtr, RxDescPtr->attributes, RxDescPtr->Phys));
    for (Index = 0; ((RxDescPtr->attributes & DESC_ATTR_DESC_DONE) == 0) && (Index < TBT_TOTAL_ACCESSES_WHEN_WAIT_TO_RX); ++Index) {
      Status = gBS->Stall (TBT_TIME_BETWEEN_RX_RING_POLL_US);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Stall had failed, Status %d\n", Status));
      }
    }

    if (Index >= TBT_TOTAL_ACCESSES_WHEN_WAIT_TO_RX) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvWaitForRxResponse: ERROR! Timeout waiting for any response packet from DMA\n"));
      DEBUG ((DEBUG_ERROR, "Waited for %d us. Exiting...\n", TBT_TOTAL_TIME_TO_WAIT_FOR_RX_US));
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvWaitForRxResponse: The descriptor address - 0x%x, attributes - 0x%x\n", RxDescPtr, RxDescPtr->attributes));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    ReceivedPdf = (UINT8)((RxDescPtr->attributes & DESC_ATTR_EOF_MASK) >> DESC_ATTR_EOF_SHIFT);
    DEBUG ((
      DEBUG_VERBOSE,
      "DMA RX frame: pdf 0x%x cmd 0x%x msg_len 0x%x. Consumer index 0x%x\n",
      ReceivedPdf,
      *(((UINT32*)This->Impl->pSharedMem->rxBuf[ConsIndex] + 2)),
      RxDescPtr->attributes & DESC_ATTR_LEN_MASK,
      ConsIndex
      ));

    DEBUG ((DEBUG_VERBOSE, "After: RxDescPtr - 0x%x, attributes - 0x%x, data addr - 0x%x\n", RxDescPtr, RxDescPtr->attributes, RxDescPtr->Phys));

    /* free the descriptors for more receive */
    if (ReceivedPdf != Pdf) {
      TbtNvmDrvFreeRxDesc (This, ConsIndex);
    }
    TotalRxPkts++;
  } while ((ReceivedPdf != Pdf) && (TotalRxPkts < MAX_PACKETS_TO_RX_WHILE_WAIT_FOR_RESP));

  if (TotalRxPkts >= MAX_PACKETS_TO_RX_WHILE_WAIT_FOR_RESP) {
    DEBUG ((DEBUG_ERROR, "Timeout waiting for the response with given Pdf 0x%x. Exiting...\n", Pdf));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }
  *ConsPtr = ConsIndex;
  *Length = (UINT16)(RxDescPtr->attributes & DESC_ATTR_LEN_MASK);
  return TBT_STATUS_SUCCESS;
}

/**
  Send DMA config packet with the given parameters.
  Config packet, meaning packet on DMA ring #0.
  Operation:
    Copy the given data to the TX ring data buffer
    Prepare TX descriptor
    Increment producer

  @param[in] This   Pointer to the DMA instance on which the function is activated
  @param[in] Eof    End of frame identifier
  @param[in] Length The length of the frame
  @param[in] Data   Pointer to the byte data

**/
STATIC
TBT_STATUS
TbtNvmDrvTxCfgPkt (
  IN TBT_DMA    *This,
  IN PDF_VALUE  Eof,
  IN UINT16     Length,
  IN UINT8      *Data
  )
{
  UINT16 Prod;
  VOID *TxBufPtr = TbtNvmDrvGetTxBuf (This, &Prod);
  if (TxBufPtr == NULL) {
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }
  gBS->CopyMem (TxBufPtr, Data, Length);   // Copy the TX frame to the DMA TX buffer
  return TbtNvmDrvInitiateTx (This, Eof, Length, Prod);
}

/**
  DMA will poll it's receive queue until the frame with the given parameters
  is received.

  In case the received frame has length other than expected, the error will be returned.

  @param[in]  This    Pointer to the DMA instance on which the function is activated
  @param[in]  Eof     End of frame identifier
  @param[in]  Length  The length of the expected frame in byte
  @param[out] Data    Pointer to the byte data where to store the received data

**/
STATIC
TBT_STATUS
TbtNvmDrvRxCfgPkt (
  IN  TBT_DMA      *This,
  IN  PDF_VALUE    Eof,
  IN  UINT16       *Length OPTIONAL,
  OUT UINT8        *Data OPTIONAL
  )
{
  TBT_STATUS  Status;
  UINT32      Cons;
  UINT16      Len;

  Status = TbtNvmDrvWaitForRxResponse (This, Eof, &Cons, &Len);
  if (TBT_STATUS_ERR (Status)) {
    return Status;
  }
  if (Length != NULL && Len != *Length) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRxCfgPkt: ERROR! The frame received has length other than expected\n"));
    DEBUG ((DEBUG_ERROR, "                   Expected - 0x%x, Received - 0x%x\n", *Length, Len));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }
  if (Data != NULL) {
    gBS->CopyMem (Data, This->Impl->pSharedMem->rxBuf[Cons], Len);
  }
  TbtNvmDrvFreeRxDesc (This, Cons);
  return TBT_STATUS_SUCCESS;
}

/**
  Write DMA MMIO register.

  @param[in] This          Pointer to the TBT_DMA instance on which the function is activated.
  @param[in] RegOffset     Offset in DMA MMIO to write to.
  @param[in] Data          Data to write.

**/
STATIC
VOID
TbtNvmDrvDmaWriteMmio (
  IN TBT_DMA *This,
  IN UINT32  RegOffset,
  IN UINT32  Data
  )
{
  DEBUG ((DEBUG_VERBOSE, "Write MMIO offset - 0x%x, wdata - 0x%x\n", RegOffset, Data));

  This->Impl->PciIoProtoPtr->Mem.Write (
                                   This->Impl->PciIoProtoPtr,
                                   EfiPciIoWidthUint32,
                                   0,
                                   RegOffset,
                                   1,
                                   &Data
                                   );
}

/**
  Read DMA MMIO register.

  @param[in] This           Pointer to the TBT_DMA instance on which the function is activated.
  @param[in] RegOffset      Offset in DMA MMIO to read from.

**/
STATIC
UINT32
TbtNvmDrvDmaReadMmio (
  IN TBT_DMA *This,
  IN UINT32  RegOffset
  )
{
  UINT32 ReadVal;

  This->Impl->PciIoProtoPtr->Mem.Read (
                                   This->Impl->PciIoProtoPtr,
                                   EfiPciIoWidthUint32,
                                   0,
                                   RegOffset,
                                   1,
                                   &ReadVal
                                   );

  DEBUG ((DEBUG_VERBOSE, "Read MMIO offset - 0x%x, rdata - 0x%x\n", RegOffset, ReadVal));
  return ReadVal;
}

STATIC
VOID
TbtNvmDrvDmaDebugPrint (
  TBT_DMA *This
)
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "-------------------------------------------------------------------------\n"));
  DEBUG ((DEBUG_INFO, "Printing the DMA internal state for debug:\n"));
  DEBUG ((DEBUG_INFO, "ARC_DEBUG - 0x%x\n\n", TbtNvmDrvDmaReadMmio(This, TBT_ARC_DEBUG_REG_MMIO_OFFSET)));
  for (Index = 0; Index < 4; Index++) {
    DEBUG ((DEBUG_INFO, "TX ring descriptor DW%d - 0x%x\n", Index, TbtNvmDrvDmaReadMmio(This, REG_TX_RING_BASE + Index*4)));
  }
  DEBUG ((DEBUG_INFO, "\n"));
  for (Index = 0; Index < 5; Index++) {
    DEBUG ((DEBUG_INFO, "TX ring table DW%d - 0x%x\n", Index, TbtNvmDrvDmaReadMmio(This, REG_TX_OPTIONS_BASE + Index*4)));
  }
  DEBUG ((DEBUG_INFO, "\n"));
  for (Index = 0; Index < 4; Index++) {
    DEBUG ((DEBUG_INFO, "RX ring descriptor DW%d - 0x%x\n", Index, TbtNvmDrvDmaReadMmio(This, REG_RX_RING_BASE + Index*4)));
  }
  DEBUG ((DEBUG_INFO, "\n"));
  for (Index = 0; Index < 5; Index++) {
    DEBUG ((DEBUG_INFO, "RX ring table DW%d - 0x%x\n", Index, TbtNvmDrvDmaReadMmio(This, REG_RX_OPTIONS_BASE + Index*4)));
  }
  DEBUG ((DEBUG_INFO, "\n"));
}

/**
  Dma destructor. Deallocates all the resources for DMA operation

  @param[in] This       Pointer to the TBT_DMA instance on which the function is activated.

**/
STATIC
VOID
TbtNvmDrvDmaDtor(
  TBT_DMA *This
  )
{
  UINT32 Data;

  DEBUG ((DEBUG_INFO, "TbtNvmDrvDmaDtor is called.\n"));

  // Release RX ring
  Data = This->ReadMmio (This, REG_RX_OPTIONS_BASE);
  Data &= ~(REG_OPTS_VALID);
  This->WriteMmio (This, REG_RX_OPTIONS_BASE, Data);

  // Release TX ring
  Data = This->ReadMmio (This, REG_TX_OPTIONS_BASE);
  Data &= ~(REG_OPTS_VALID);
  This->WriteMmio (This, REG_TX_OPTIONS_BASE, Data);

  This->Impl->PciIoProtoPtr->Unmap (This->Impl->PciIoProtoPtr, This->Impl->mapping);
  This->Impl->PciIoProtoPtr->FreeBuffer (
                               This->Impl->PciIoProtoPtr,
                               EFI_SIZE_TO_PAGES (sizeof(*This->Impl->pSharedMem)),
                               This->Impl->pSharedMem
                               );
  TbtNvmDrvDeAllocateMem (This->Impl);
  TbtNvmDrvDeAllocateMem (This);
}

/**
  Dma constructor. Allocates all the required resources for DMA operation

  @param[in] PciIoProto               Pointer to the PCI Protocol instance.
  @param[in] TBTControllerWasPowered  Store the TBT controller power status to DMA structure.

  @retval    TBT_DMA*    Pointer to the TBT_DMA structure

**/
TBT_DMA*
TbtNvmDrvDmaCtor (
  EFI_PCI_IO_PROTOCOL  *PciIoProto,
  BOOLEAN              *TBTControllerWasPowered,
  UINTN                FunctionNumber
  )
{
  EFI_STATUS            Status;
  TBT_DMA_IMPL          *DmaImplPtr;
  UINTN                 NumBytes;
  EFI_PHYSICAL_ADDRESS  PhyAddr;
  UINT32                RegVal;
  UINT32                Index;
  TBT_DMA               *DmaPtr;
  VOID                  *HobPtr;
  USB4_PLATFORM_HOB     *Usb4PlatformHob;

  DmaImplPtr = TbtNvmDrvAllocateMem (sizeof(TBT_DMA_IMPL));
  if (!DmaImplPtr) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvDmaCtor: AllocateRuntimeZeroPool failed, Status = %r\n", EFI_OUT_OF_RESOURCES));
    return NULL;
  }
  DmaImplPtr->PciIoProtoPtr = PciIoProto;
  // Allocate DMA memory
  Status = DmaImplPtr->PciIoProtoPtr->AllocateBuffer (
                                        DmaImplPtr->PciIoProtoPtr,
                                        AllocateAnyPages,
                                        EfiRuntimeServicesData,
                                        EFI_SIZE_TO_PAGES (sizeof(*DmaImplPtr->pSharedMem)),
                                        (VOID**) &DmaImplPtr->pSharedMem,
                                        0
                                        );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvDmaCtor: AllocateBuffer failed, Status = %r\n", Status));
    goto free_dma_impl;
  }
  // Map the memory to the physical location
  // Get physical address of the memory along with mapping to free it later.
  NumBytes = sizeof (*DmaImplPtr->pSharedMem);
  Status = DmaImplPtr->PciIoProtoPtr->Map (
                                        DmaImplPtr->PciIoProtoPtr,
                                        EfiPciIoOperationBusMasterCommonBuffer,
                                        DmaImplPtr->pSharedMem,
                                        &NumBytes,
                                        &DmaImplPtr->PhyAddr,
                                        &DmaImplPtr->mapping
                                        );
  if (EFI_ERROR (Status)) {
    if (Status == EFI_NOT_READY) {
      //
      // If Iommu is enabled and DMAR table is not exist, it means Iommu protocol is not ready and PciIoMap will return EFI_NOT_READY.
      // Need to use the buffer which reserved in PEI phase via Iommu Ppi to access DMA at that time.
      //
      if (FunctionNumber < 2) {
        DEBUG ((DEBUG_ERROR, "TbtNvmDrvDmaCtor: Function number is invalid\n"));
        goto free_shared_mem;
      }
      HobPtr = NULL;
      HobPtr = GetFirstGuidHob (&gUsb4PlatformHobGuid);
      if (HobPtr != NULL) {
        Usb4PlatformHob = GET_GUID_HOB_DATA (HobPtr);
        if (Usb4PlatformHob->Usb4PlatformInfo.Usb4Hr[HR_INDEX (FunctionNumber)].PeiDmaBufBase != 0) {
          DEBUG ((DEBUG_INFO, "TbtNvmDrvDmaCtor: Use PeiDmaBuf to initialize.\n"));
          DmaImplPtr->PciIoProtoPtr->FreeBuffer (
                                       DmaImplPtr->PciIoProtoPtr,
                                       EFI_SIZE_TO_PAGES (sizeof (*DmaImplPtr->pSharedMem)),
                                       DmaImplPtr->pSharedMem
                                       );
          DmaImplPtr->pSharedMem = (VOID *) Usb4PlatformHob->Usb4PlatformInfo.Usb4Hr[HR_INDEX (FunctionNumber)].PeiDmaBufBase;
          DmaImplPtr->PhyAddr = Usb4PlatformHob->Usb4PlatformInfo.Usb4Hr[HR_INDEX (FunctionNumber)].PeiDmaBufBase;
        } else {
          DEBUG ((DEBUG_ERROR, "TbtNvmDrvDmaCtor: PeiDmaBuf is not exist.\n"));
          goto free_shared_mem;
        }
      }
    } else {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvDmaCtor: Map failed, Status=%r\n", Status));
      goto free_shared_mem;
    }
  }

  // Check the mapping is done fully
  if (NumBytes != sizeof (*DmaImplPtr->pSharedMem)) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvDmaCtor: Map returned %u bytes, Status=%r\n", NumBytes, Status));
    goto unmap;
  }

  // Initialize the DMA public object
  DmaPtr = TbtNvmDrvAllocateMem (sizeof(TBT_DMA));
  if (!DmaPtr) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvDmaCtor: AllocateRuntimeZeroPool failed, Status=%r\n", EFI_OUT_OF_RESOURCES));
    goto unmap;
  }
  DmaPtr->Impl                    = DmaImplPtr;
  DmaPtr->RxCfgPkt                = TbtNvmDrvRxCfgPkt;
  DmaPtr->TxCfgPkt                = TbtNvmDrvTxCfgPkt;
  DmaPtr->ReadMmio                = TbtNvmDrvDmaReadMmio;
  DmaPtr->WriteMmio               = TbtNvmDrvDmaWriteMmio;
  DmaPtr->DbgPrint                = TbtNvmDrvDmaDebugPrint;
  DmaPtr->Dtor                    = TbtNvmDrvDmaDtor;
  DmaPtr->TBTControllerWasPowered = TBTControllerWasPowered;

  // Configure the TX descriptors
  for (Index = 0, PhyAddr = DmaImplPtr->PhyAddr;
       Index < FW_RING_NUM_TX_BUFS;
       ++Index, PhyAddr += MAX_FW_FRAME_SIZE) {
    DmaImplPtr->pSharedMem->txBufDesc[Index].Phys = PhyAddr;
    TbtNvmDrvClearDescriptor (DmaImplPtr->pSharedMem->txBufDesc, Index);
  }

  // Configure the RX descriptors
  for (Index = 0;
       Index < FW_RING_NUM_RX_BUFS;
       ++Index, PhyAddr += MAX_FW_FRAME_SIZE) {
    DmaImplPtr->pSharedMem->rxBufDesc[Index].Phys = PhyAddr;
    TbtNvmDrvClearDescriptor (DmaImplPtr->pSharedMem->rxBufDesc, Index);
  }

  // Configure the TX registers for FW ring
  PhyAddr = DmaImplPtr->PhyAddr + OFFSET_OF (SHARED_MEMORY, txBufDesc);
  DmaPtr->WriteMmio (DmaPtr, REG_TX_RING_BASE, (UINT32)PhyAddr);
  DmaPtr->WriteMmio (DmaPtr, REG_TX_RING_BASE+4, (UINT32)RShiftU64 (PhyAddr, 32));
  RegVal = (FW_RING_NUM_TX_BUFS << REG_RING_SIZE_SHIFT) & REG_RING_SIZE_MASK;
  DmaPtr->WriteMmio (DmaPtr, REG_TX_RING_BASE + REG_RING_SIZE_OFFSET, RegVal);
  RegVal = REG_OPTS_RAW | REG_OPTS_VALID;
  DmaPtr->WriteMmio (DmaPtr, REG_TX_OPTIONS_BASE, RegVal);

  // Configure the RX registers for FW ring
  PhyAddr = DmaImplPtr->PhyAddr + OFFSET_OF (SHARED_MEMORY, rxBufDesc);
  DmaPtr->WriteMmio (DmaPtr, REG_RX_RING_BASE, (UINT32)PhyAddr);
  DmaPtr->WriteMmio (DmaPtr, REG_RX_RING_BASE + 4, (UINT32)RShiftU64 (PhyAddr, 32));
  RegVal = ((FW_RING_NUM_RX_BUFS << REG_RING_SIZE_SHIFT) & REG_RING_SIZE_MASK)
         | ((MAX_FW_FRAME_SIZE << REG_RING_BUF_SIZE_SHIFT)
         & REG_RING_BUF_SIZE_MASK);
  DmaPtr->WriteMmio (DmaPtr, REG_RX_RING_BASE + REG_RING_SIZE_OFFSET, RegVal);
  RegVal = ((FW_RING_NUM_RX_BUFS - 1) << REG_RING_CONS_SHIFT) & REG_RING_CONS_MASK;
  DmaPtr->WriteMmio (DmaPtr, REG_RX_RING_BASE + REG_RING_CONS_PROD_OFFSET, RegVal);
  RegVal = REG_OPTS_RAW | REG_OPTS_VALID;
  DmaPtr->WriteMmio (DmaPtr, REG_RX_OPTIONS_BASE, RegVal);

  return DmaPtr;

unmap:
  DmaImplPtr->PciIoProtoPtr->Unmap (DmaImplPtr->PciIoProtoPtr, DmaImplPtr->mapping);
free_shared_mem:
  DmaImplPtr->PciIoProtoPtr->FreeBuffer (
                               DmaImplPtr->PciIoProtoPtr,
                               EFI_SIZE_TO_PAGES (sizeof(*DmaImplPtr->pSharedMem)),
                               DmaImplPtr->pSharedMem
                               );
free_dma_impl:
  TbtNvmDrvDeAllocateMem (DmaImplPtr);
  return NULL;
}
