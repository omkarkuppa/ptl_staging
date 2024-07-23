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

#ifndef TBT_NVM_DRV_DMA_
#define TBT_NVM_DRV_DMA_

#include <Protocol/PciIo.h>
#include <Library/TbtNvmDrvUtils.h>
#include <Protocol/TbtNvmDrvDmaProtocol.h>

#define TBT_ARC_DEBUG_REG_MMIO_OFFSET 0x39944

// R_USBC_TBT_DMA_MEM_TX_RING_DESC_0_INST_0
#define REG_TX_RING_BASE            0x00000

// R_USBC_TBT_DMA_MEM_RX_RING_DESC_0_INST_0
#define REG_RX_RING_BASE            0x08000

#define REG_RING_STEP               16
#define REG_RING_PHYS_LO_OFFSET     0
#define REG_RING_PHYS_HI_OFFSET     4
#define REG_RING_CONS_PROD_OFFSET   8  /* cons - RO, prod - RW */
#define REG_RING_CONS_SHIFT         0
#define REG_RING_CONS_MASK          MASK(15, REG_RING_CONS_SHIFT)
#define REG_RING_PROD_SHIFT         16
#define REG_RING_PROD_MASK          MASK(31, REG_RING_PROD_SHIFT)
#define REG_RING_SIZE_OFFSET        12
#define REG_RING_SIZE_SHIFT         0
#define REG_RING_SIZE_MASK          MASK(15, REG_RING_SIZE_SHIFT)
#define REG_RING_BUF_SIZE_SHIFT     16
#define REG_RING_BUF_SIZE_MASK      MASK(27, REG_RING_BUF_SIZE_SHIFT)

// R_USBC_TBT_DMA_MEM_TX_TABLE_0_INST_0
#define REG_TX_OPTIONS_BASE             0x19800
// R_USBC_TBT_DMA_MEM_RX_TABLE_0_INST_0
#define REG_RX_OPTIONS_BASE             0x29800

#define REG_RX_OPTS_TX_E2E_HOP_ID_SHIFT 12
#define REG_RX_OPTS_TX_E2E_HOP_ID_MASK  MASK(22, REG_RX_OPTS_TX_E2E_HOP_ID_SHIFT)
#define REG_RX_OPTS_MASK_OFFSET         4
#define REG_RX_OPTS_MASK_EOF_SHIFT      0
#define REG_RX_OPTS_MASK_EOF_MASK       MASK(15, REG_RX_OPTS_MASK_EOF_SHIFT)
#define REG_RX_OPTS_MASK_SOF_SHIFT      16
#define REG_RX_OPTS_MASK_SOF_MASK       MASK(31, REG_RX_OPTS_MASK_SOF_SHIFT)
#define REG_OPTS_STEP                   32
#define REG_OPTS_E2E_EN                 BIT(28)
#define REG_OPTS_RAW                    BIT(30)
#define REG_OPTS_VALID                  BIT(31)

#define DESC_ATTR_LEN_SHIFT         0
#define DESC_ATTR_LEN_MASK          MASK(11, DESC_ATTR_LEN_SHIFT)
#define DESC_ATTR_EOF_SHIFT         12
#define DESC_ATTR_EOF_MASK          MASK(15, DESC_ATTR_EOF_SHIFT)
#define DESC_ATTR_SOF_SHIFT         16
#define DESC_ATTR_SOF_MASK          MASK(19, DESC_ATTR_SOF_SHIFT)
#define DESC_ATTR_TX_ISOCH_DMA_EN   BIT(20)  /* TX */
#define DESC_ATTR_RX_CRC_ERR        BIT(20)  /* RX after use */
#define DESC_ATTR_DESC_DONE         BIT(21)
#define DESC_ATTR_REQ_STS           BIT(22)  /* TX and RX before use */
#define DESC_ATTR_RX_BUF_OVRN_ERR   BIT(22)  /* RX after use */
#define DESC_ATTR_INT_EN            BIT(23)
#define DESC_ATTR_OFFSET_SHIFT      24
#define DESC_ATTR_OFFSET_MASK       MASK(31, DESC_ATTR_OFFSET_SHIFT)

#define TBT_TX_RING_FULL(prod, cons, size) ((((prod) + 1) % (size)) == (cons))
#define TBT_TX_RING_EMPTY(prod, cons) ((prod) == (cons))
#define TBT_RX_RING_FULL(prod, cons) ((prod) == (cons))
#define TBT_RX_RING_EMPTY(prod, cons, size) ((((cons) + 1) % (size)) == (prod))

#define TBT_TIME_BETWEEN_TX_RING_POLL_US 50         // 50 microseconds
#define TBT_TOTAL_TIME_TO_WAIT_FOR_TX_US 10000000   // 10s
#define TBT_TOTAL_ACCESSES_WHEN_WAIT_TO_TX  TBT_TOTAL_TIME_TO_WAIT_FOR_TX_US / TBT_TIME_BETWEEN_TX_RING_POLL_US

#define TBT_TIME_BETWEEN_RX_RING_POLL_US 50         // 50us
#define TBT_TOTAL_TIME_TO_WAIT_FOR_RX_US 10000000   // 10s
#define TBT_TOTAL_ACCESSES_WHEN_WAIT_TO_RX  TBT_TOTAL_TIME_TO_WAIT_FOR_RX_US / TBT_TIME_BETWEEN_RX_RING_POLL_US
#define MAX_PACKETS_TO_RX_WHILE_WAIT_FOR_RESP 1000

/**
  Dma constructor. Allocates all the required resources for DMA operation

  @param PciIoProto  Pointer to the PCI Protocol instance.

**/
TBT_DMA*
TbtNvmDrvDmaCtor (
  EFI_PCI_IO_PROTOCOL  *PciIoProto,
  BOOLEAN              *TBTControllerWasPowered,
  UINTN                FunctionNumber
  );

#endif /* TBT_NVM_DRV_DMA_ */
