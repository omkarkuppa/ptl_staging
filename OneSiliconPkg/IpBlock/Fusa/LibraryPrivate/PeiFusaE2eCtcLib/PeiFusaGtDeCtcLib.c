/** @file

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
**/

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/CpuLib.h>
#include <Register/Intel/ArchitecturalMsr.h>
#include <Library/PciSegmentLib.h>
#include "PeiFusaResultReportingLib.h"
#include "PeiFusaGtDeCtcLib.h"

/**
  Perform GT L3 ECC Ctc
**/
VOID
FspDxCheckGtL3EccCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  UINT8                 TotalChecks;
  UINT8                 CheckResult = 0;
  UINT8                 TestIndex;
  FUSA_TEST_RESULT      *TestResultPtr;
  UINT64                BaseAddress;
  UINT32                GtMmioAddr;
  UINT32                Val32;
  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;
  UINT32                InjectionValue;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  // Init Test Results
  TotalChecks = 2; // Single Bit ECC; Double Bit ECC

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumGtL3EccCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (UINT8 i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Read PCI 00:02:00 for GT MMIO Address
  BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, 0, SA_GT_DEV, 0, 0x10);
  GtMmioAddr = (PciSegmentRead32 (BaseAddress)) & 0xFFFFFFF0;
  if (GtMmioAddr == 0x0) {
    DEBUG ((DEBUG_INFO, "GtMmioAddr is not programmed!\n"));
    return;
  }
  DEBUG ((DEBUG_INFO, "GtMmioAddr 0x%x\n", GtMmioAddr));

  // Check whether lbcflockmsgreg_lock is set
  Val32 = MmioRead32 (GtMmioAddr + L3BANKLOCK_LBCF_GFX_GT_REG);
  DEBUG ((DEBUG_INFO, "lbcflockmsgreg_lock 0x%x\n", (Val32 & BIT16)));
  if (Val32 & BIT16) {
    DEBUG ((DEBUG_INFO, "Test Aborted\n"));
    return;
  }

  McaNum = MCA_IOP;
  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  for (UINT8 Check = 0; Check < TotalChecks; Check++) {
    DEBUG ((DEBUG_INFO, "%a Bit\n", (Check == 0) ? "Single" : "Double"));

    // Wake GT
    MmioWrite32 (GtMmioAddr + FORCE_WAKE_GPM_GRP_GCD, 0xFFFFFFFF);

    ExpectedMcaStatus = 0;
    switch (Check) {
      case 0: // Single Bit
        ExpectedMcaStatus = ((0x0003ULL) << 16 | 0x0E0BULL);
        InjectionValue = 0xC000C;
        Val32 = MmioRead32 (GtMmioAddr + DEV_ERR_ROUTING_CTRL);
        MmioWrite32 (GtMmioAddr + DEV_ERR_ROUTING_CTRL, (Val32 & (~BIT0)));
        break;
      case 1: // Double Bit
        ExpectedMcaStatus = ((0x0001ULL) << 16 | 0x0E0BULL);
        InjectionValue = 0x1C001C;
        Val32 = MmioRead32 (GtMmioAddr + DEV_ERR_ROUTING_CTRL);
        MmioWrite32 (GtMmioAddr + DEV_ERR_ROUTING_CTRL, (Val32 & (~BIT2)));
        break;
      default:
        break;
    }
    PciSegmentWrite16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, 0, SA_GT_DEV, 0, DEVICECTL_0_2_0_PCI), 0x7);
    MicroSecondDelay (5 * 1000);

    // Error Injection
    MmioWrite32 (GtMmioAddr + LBCFLOCKMSGREG_LBCF_GFX, InjectionValue);
    MicroSecondDelay (5 * 1000);

    // Check Error
    if (Check == 0) {
      Val32 = (MmioRead32 (GtMmioAddr + ERR_STAT_GT_CORRECTABLE_2));
      DEBUG ((DEBUG_INFO, "ERR_STAT_GT_CORRECTABLE_2 0x%x\n", Val32));
    } else {
      Val32 = (MmioRead32 (GtMmioAddr + ERR_STAT_GT_FATAL_2));
      DEBUG ((DEBUG_INFO, "ERR_STAT_GT_FATAL_2 0x%x\n", Val32));
    }

    // Check MCA
    CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
    DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

    // Update CheckResults
    if ((Val32 == 0x000000FF) && (CheckResult == FUSA_TEST_PASS)) {
      pFusaTestResult[TestIndex].CheckResults[Check] = FUSA_TEST_PASS;
    } else {
      pFusaTestResult[TestIndex].CheckResults[Check] = FUSA_TEST_FAIL;
    }

    DEBUG ((DEBUG_INFO, "CheckResult       : %a\n", (CheckResult == FUSA_TEST_PASS) ? "PASS" : "FAIL"));

    // Update TestResult
    if (CheckResult == FUSA_TEST_FAIL) {
      pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
    } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
      pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
    }

    // Clean up
    McaBankInfoClear (McaNum);
  }

  // Generate Crc32
  TestResultPtr = &pFusaTestResult[TestIndex];
  pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
    (UINT8 *) TestResultPtr,
    sizeof (FUSA_TEST_RESULT) - 4,
    0xffffffffU
  );

  // Dump results
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}