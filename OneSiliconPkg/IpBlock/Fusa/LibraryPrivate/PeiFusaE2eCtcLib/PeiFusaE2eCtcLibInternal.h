/** @file
  Header file for Fusa Private Lib Internal functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _FUSA_E2E_CTC_LIB_INTERNAL_H_
#define _FUSA_E2E_CTC_LIB_INTERNAL_H_

#include "PeiFusaResultReportingLib.h"

/**
  Perform Core IDI Parity Error E2E CTC.
  The test targets all active unique core. For core not
  detected, their test result is updated to
  FUSA_TEST_DEVICE_NOTAVAILABLE

  @note The test expects to be run within  Memory Type =
        Writeback (MTRR Encoding = 06h), which is the usual
        case of the memory type covering the code/data region
        of the bootloader.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test from FusaTestNumCpu0Idi
                         to FusaTestNumCpu7Idi

**/
VOID
FspDxCheckCoreIdi (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform LLC ECC Error E2E CTC.
  The test targets all active unique CBO (which in turn maps to
  unique core). For CBO/core not detected, their test result is
  updated to FUSA_TEST_DEVICE_NOTAVAILABLE

  @note The test expects to be run within Memory Type =
        Writeback (MTRR Encoding = 06h), which is the usual
        case of the memory type covering the code/data region
        of the bootloader.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test from
                         FusaTestNumCboSlice0Ingress to
                         FusaTestNumCboSlice7Ingress

**/
VOID
FspDxCheckLlc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform OPI Link Data Parity Error E2E CTC.
  The test targets OPI Link Data path.

  @note The test assumes MchBar and DmiBar have been set up
        with 32 bit addressing and being enabled.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test
                         FusaTestNumOpiLinkIosfData

**/
VOID
FspDxCheckOpiLink (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform IO Port Parity Error E2E CTC.
  The test targets IO Port multiple interfaces.

  @note The test assumes MchBar, GmBar and HpetBase have been
        set up with 32 bit addressing and being enabled.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test FusaTestNumIop

**/
VOID
FspDxCheckIoPort (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  Perform Display Isoch Port (DIP) Parity Error E2E CTC. The
  test targets DIP multiple interfaces.

  @note The test assumes MchBar and GttMmBar have been set up
        with 32 bit addressing and being enabled.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test FusaTestNumDip

**/
VOID
FspDxCheckDip (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  );

/**
  To retrieve  mUniqueCoreList

  @retval mUniqueCoreList
**/
CONST UINT32 *
UniqueCoreListGet (
  VOID
  );

/**
  The function prototype for invoking a function on an Application Processor.

  @param[in,out] Buffer  The pointer to private data buffer.
**/
typedef
VOID
(*AP_PROCEDURE)(
  IN OUT VOID  *Buffer
  );

/**
  To execute routine "Routine" with paramters "Param" at AP
  indicated by ProcessorNumber, in blocking mode.

  @param[in] Routine - routine to be executed
  @param[in] ProcessorNumber - target AP processor number
                         enumerated by the MP services
  @param[in] Param - parameters to be used by the said routine

**/
VOID
RunAtAp (
  IN AP_PROCEDURE Routine,
  IN UINT32       ProcessorNumber,
  IN VOID         *Param
  );

#endif // _FUSA_E2E_CTC_LIB_INTERNAL_H_
