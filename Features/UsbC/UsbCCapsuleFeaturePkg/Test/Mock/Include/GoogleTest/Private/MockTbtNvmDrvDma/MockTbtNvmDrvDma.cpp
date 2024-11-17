/** @file
  Google Test mocks for TbtNvmDrvDma

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

#include "MockTbtNvmDrvDma.h"

MOCK_INTERFACE_DEFINITION (MockTbtNvmDrvDma);

MOCK_FUNCTION_DEFINITION (MockTbtNvmDrvDma, TbtNvmDrvTxCfgPkt, 4, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockTbtNvmDrvDma, TbtNvmDrvRxCfgPkt, 4, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockTbtNvmDrvDma, TbtNvmDrvDmaWriteMmio, 3, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockTbtNvmDrvDma, TbtNvmDrvDmaReadMmio, 2, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockTbtNvmDrvDma, TbtNvmDrvDmaDebugPrint, 1, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockTbtNvmDrvDma, TbtNvmDrvDmaDtor, 1, EFIAPI);

TBT_DMA LocalDmaPtr = {
  NULL,                                // TBT_DMA_IMPL
  TbtNvmDrvTxCfgPkt,                   // TX_CFG_PKT
  TbtNvmDrvRxCfgPkt,                   // RX_CFG_PKT
  TbtNvmDrvDmaWriteMmio,               // WRITE_MMIO
  TbtNvmDrvDmaReadMmio,                // READ_MMIO
  TbtNvmDrvDmaDebugPrint,              // DEBUG_PRINT
  TbtNvmDrvDmaDtor,                    // DMA_DTOR
  NULL,                                // *TBTControllerWasPowered
};

TBT_DMA *gDmaPtrMock = &LocalDmaPtr;
