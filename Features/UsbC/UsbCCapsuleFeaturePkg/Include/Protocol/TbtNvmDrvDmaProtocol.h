/** @file
  DMA header.
  DMA serves as abstraction of TBT DMA.

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

#ifndef TBT_NVM_DRV_DMA_PROTOCOL_
#define TBT_NVM_DRV_DMA_PROTOCOL_

#include <Protocol/PciIo.h>
#include <Library/TbtNvmDrvUtils.h>
#include <Protocol/TbtNvmDrvCioProtocol.h>

//
// Convert to index of USB4 Host Router Information from Fun#
//
#define HR_INDEX(FunctionNumber) (FunctionNumber - 2)

typedef struct _TBT_DMA TBT_DMA;

typedef struct _TBT_DMA_IMPL TBT_DMA_IMPL;

/**
  Send DMA frame with the given parameters

  @param[in] This   Pointer to the DMA instance on which the function is activated
  @param[in] Eof    End of frame identifier
  @param[in] Length The length of the frame in byte
  @param[in] Data   Pointer to the byte data

**/
typedef
TBT_STATUS
(*TX_CFG_PKT) (
  IN TBT_DMA    *This,
  IN PDF_VALUE  Eof,
  IN UINT16     Length,
  IN UINT8      *Data
  );

/**
  DMA will poll it's receive queue until the frame with the given parameters
  is received.

  In case the received frame has length bigger than expected, the error will be returned.

  @param[in]     This   Pointer to the DMA instance on which the function is activated
  @param[in]     Eof    End of frame identifier
  @param[in,out] Length In:  The length of the expected frame
                        Out: The length of the received frame
  @param[out]    Data   Pointer to the byte data where to store the received data

**/
typedef
TBT_STATUS
(*RX_CFG_PKT) (
  IN TBT_DMA      *This,
  IN PDF_VALUE    Eof,
  IN OUT UINT16   *Length OPTIONAL,
  OUT UINT8       *Data OPTIONAL
  );

/**
  Write DMA MMIO register.

  @param[in] This        Pointer to the TBT_DMA instance on which the function is activated.
  @param[in] RegOffset   Offset in DMA MMIO to write to.
  @param[in] Data        Data to write.

**/
typedef
VOID
(*WRITE_MMIO) (
  IN TBT_DMA *This,
  IN UINT32  RegOffset,
  IN UINT32  Data
  );

/**
  Read DMA MMIO register.

  @param[in] This        Pointer to the TBT_DMA instance on which the function is activated.
  @param[in] RegOffset   Offset in DMA MMIO to read from.

**/
typedef
UINT32
(*READ_MMIO) (
  IN TBT_DMA *This,
  IN UINT32  RegOffset
  );

/**
  Prints the DMA state for debug

  @param This  Pointer to the instance on which this method is activated.

**/
typedef
VOID
(*DEBUG_PRINT) (
  TBT_DMA *This
  );

/**
  Dma destructor. Deallocates all the required resources.

  @param This  Pointer to the instance on which this method is activated.

**/
typedef
VOID
(*DMA_DTOR) (
  TBT_DMA *This
  );

struct _TBT_DMA {
  TBT_DMA_IMPL      *Impl;
  TX_CFG_PKT        TxCfgPkt;
  RX_CFG_PKT        RxCfgPkt;
  WRITE_MMIO        WriteMmio;
  READ_MMIO         ReadMmio;
  DEBUG_PRINT       DbgPrint;
  DMA_DTOR          Dtor;
  BOOLEAN           *TBTControllerWasPowered;
};

typedef struct {
  UINT64          Phys;
  volatile UINT32 attributes;
  UINT32          TbtBufTime;
} TBT_BUF_DESC;

#define MAX_FW_FRAME_SIZE 256
#define FW_RING_NUM_TX_BUFS 2
#define FW_RING_NUM_RX_BUFS \
   ((EFI_PAGE_SIZE - \
     (FW_RING_NUM_TX_BUFS * (sizeof(TBT_BUF_DESC) + MAX_FW_FRAME_SIZE)))\
    / (sizeof(TBT_BUF_DESC) + MAX_FW_FRAME_SIZE))

typedef struct {
  UINT8 txBuf[FW_RING_NUM_TX_BUFS][MAX_FW_FRAME_SIZE];
  UINT8 rxBuf[FW_RING_NUM_RX_BUFS][MAX_FW_FRAME_SIZE];
  TBT_BUF_DESC txBufDesc[FW_RING_NUM_TX_BUFS];
  TBT_BUF_DESC rxBufDesc[FW_RING_NUM_RX_BUFS];
} SHARED_MEMORY;

struct _TBT_DMA_IMPL {
  EFI_PHYSICAL_ADDRESS  PhyAddr;
  EFI_PCI_IO_PROTOCOL   *PciIoProtoPtr;
  SHARED_MEMORY         *pSharedMem;
  VOID                  *mapping;
};

#endif /* TBT_NVM_DRV_DMA_PROTOCOL_ */
