/** @file
  PEI FUSA E2E CTC LLC Double Parity Header File

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

#ifndef _FUSA_E2E_CTC_LLC_DOUBLE_PARITY_H_
#define _FUSA_E2E_CTC_LLC_DOUBLE_PARITY_H_

#include "PeiFusaResultReportingLib.h"
#include <Protocol/MpService.h>
#include <Register/Ptl/Msr/MsrRegs.h>
#include "PeiFusaPrivateLibInternal.h"

/**
  Perform E2E CTC LLC Double Parity.
**/
VOID
FspDxCheckCboSliceEgressLlcDoubleParityCtC (
  IN FUSA_TEST_RESULT *pFusaTestResult
  );

#endif // _FUSA_E2E_CTC_LLC_DOUBLE_PARITY_H_
