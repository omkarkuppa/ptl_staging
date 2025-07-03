/** @file
  PEI FUSA Core CTC Header File

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
#ifndef _FUSA_CORE_CTC_H_
#define _FUSA_CORE_CTC_H_

#include "PeiFusaResultReportingLib.h"
#include <Protocol/MpService.h>
#include <Register/Ptl/Msr/MsrRegs.h>
#include "PeiFusaPrivateLibInternal.h"

typedef enum {
  CacheArray,
  RomCrc
} CoreCtc;

typedef enum {
  PerrAddr,
  Slice0Cone0PerrData,
  Slice0Cone1PerrData,
  Slice1Cone0PerrData,
  Slice1Cone1PerrData
} CoreIdiParityCtc;

typedef struct
{
  FUSA_TEST_RESULT        *pFusaTestResult;
  MSR_CORE_BIST_REGISTER  MsrCoreBistReg;
  UINT8                   CoreNumber;
} CORE_CTC_TEST_PARAM;

typedef struct
{
  FUSA_TEST_RESULT        *pFusaTestResult;
  UINT64                  Msr64;
  UINT64                  Payload;
} CORE_IDI_CTC_TEST_PARAM;

/**
  Perform Core Cache Array Start-up BIST
**/
VOID
FspDxCheckCoreCacheArrayStartupBist (
  IN FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform Core ROM CRC Start-up BIST
**/
VOID
FspDxCheckCoreRomCrcStartupBist (
  IN FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform Core Idi Parity Ctc
**/
VOID
FspDxCheckCoreIdiParityCtc (
  IN FUSA_TEST_RESULT *pFusaTestResult
  );
#endif // _FUSA_CORE_CTC_H_
