/** @file
  PEI Fusa CCF E2E IDI Parity Ctc Header File

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
#ifndef _FUSA_CCF_E2E_IDI_PARITY_H_
#define _FUSA_CCF_E2E_IDI_PARITY_H_

#include "PeiFusaResultReportingLib.h"
#include <Protocol/MpService.h>
#include <Register/Ptl/Msr/MsrRegs.h>
#include "PeiFusaPrivateLibInternal.h"

typedef enum {
  CcfE2eIdiParityCtc_Addr,
  CcfE2eIdiParityCtc_DataH0,
  CcfE2eIdiParityCtc_DataH1
} CcfE2eIdiParityCtc;

typedef enum {
  CCFE2eNCUParityCtc_Rx_Data_Chk_en, 
  CCFE2eNCUParityCtc_Tx_Data_Chk_en
} CCFE2eNCUParityCtc;

typedef enum {
  IDIBE2eNCUParityCtc_Rx_Data_Chk_en, 
  IDIBE2eNCUParityCtc_Tx_Data_Chk_en
} IDIBE2eNCUParityCtc;

/**
  Perform CCF E2E IDI Parity Double Parity.
**/
VOID
FspDxCheckCcfE2eIdiParityCtc (
  IN FUSA_TEST_RESULT *pFusaTestResult
  );

#endif // _FUSA_CCF_E2E_IDI_PARITY_H_
