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

#ifndef MOCK_TBT_NVM_DRV_DMA_H_
#define MOCK_TBT_NVM_DRV_DMA_H_

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/PciIo.h>
#include <Protocol/TbtNvmDrvDmaProtocol.h>
#include <Protocol/TbtNvmDrvCioProtocol.h>
#include <TbtNvmRetimer.h>

extern TBT_DMA  LocalDmaPtr;
}

struct MockTbtNvmDrvDma {
  MOCK_INTERFACE_DECLARATION (MockTbtNvmDrvDma);

  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    TbtNvmDrvTxCfgPkt,
    (IN TBT_DMA    *This,
     IN PDF_VALUE  Eof,
     IN UINT16     Length,
     IN UINT8      *Data)
    );
  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    TbtNvmDrvRxCfgPkt,
    (IN  TBT_DMA      *This,
     IN  PDF_VALUE    Eof,
     IN  UINT16       *Length,
     OUT UINT8        *Data)
    );
  MOCK_FUNCTION_DECLARATION (
    VOID,
    TbtNvmDrvDmaWriteMmio,
    (IN TBT_DMA *This,
     IN UINT32  RegOffset,
     IN UINT32  Data)
    );
  MOCK_FUNCTION_DECLARATION (
    UINT32,
    TbtNvmDrvDmaReadMmio,
    (IN TBT_DMA *This,
     IN UINT32  RegOffset)
    );
  MOCK_FUNCTION_DECLARATION (
    VOID,
    TbtNvmDrvDmaDebugPrint,
    (TBT_DMA *This)
    );
  MOCK_FUNCTION_DECLARATION (
    VOID,
    TbtNvmDrvDmaDtor,
    (TBT_DMA *This)
    );
};

#endif
