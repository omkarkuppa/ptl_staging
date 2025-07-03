/** @file
  PEI FUSA IBECC

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

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/TimerLib.h>
#include <Register/Intel/ArchitecturalMsr.h>
#include "PeiFusaResultReportingLib.h"
#include "PeiFusaIbecc.h"

void
GenerateTraffic (
  IN     UINT8   OpMode,
  IN     UINT64  MemoryAddr
)
{
  UINT32                   MemoryData = 0xF0F0F0F0;
  EFI_PHYSICAL_ADDRESS     MemoryAddress = MemoryAddr;

  if (OpMode == TrafficRead) {
    MemoryData = *(volatile UINT32*) (UINTN) MemoryAddress;
  } else if (OpMode == TrafficWrite) {
    *(volatile UINT32*) (UINTN) MemoryAddress = MemoryData;
  } else if (OpMode == TrafficWriteRead) {
    *(volatile UINT32*) (UINTN) MemoryAddress = MemoryData;
    MemoryData = *(volatile UINT32*) (UINTN) MemoryAddress;
  } else { // TrafficReadWrite
    MemoryData = *(volatile UINT32*) (UINTN) MemoryAddress;
    *(volatile UINT32*) (UINTN) MemoryAddress = MemoryData;
  }
}

BOOLEAN
GetSetMcaStatus (
  IN     UINT8    OpMode
)
{
  UINT64      McaStatus;
  UINT32      McacodStatus;
  UINT32      MscodStatus;
  UINT32      MscodCheck;
  BOOLEAN     MsrResult;

  MscodCheck = ((OpMode == McaCorrectable) ? 0x0004 : 0x0001);

  MsrResult = FALSE;

  switch (OpMode) {
    case McaClear:
      AsmWriteMsr64(MRC_MSR_MC5_STATUS, 0);
      break;
    case McaParity:
    case McaUncorrectable:
    case McaCorrectable:
      McaStatus = AsmReadMsr64 (MRC_MSR_MC5_STATUS);
      McacodStatus = (UINT32) (McaStatus & MRC_MSR_MC5_STATUS_MCACOD_MASK);
      MscodStatus = ((UINT32) (McaStatus & MRC_MSR_MC5_STATUS_MSCOD_MASK)) >> 16;
      MsrResult = (BOOLEAN) ((McacodStatus == 0x008F) && (MscodStatus == MscodCheck));
      DEBUG ((DEBUG_INFO, "McaStatus = %llx, McacodStatus = %x, MscodStatus = %x, MscodCheck = %x, Result = %x\n",\
                           McaStatus, McacodStatus, MscodStatus, MscodCheck, MsrResult));
      break;
    default:
      break;
  }
  return MsrResult;
}

VOID
FspDxCheckIbeccParityGeneric (
  OUT FUSA_TEST_RESULT *pFusaTestResult,
  IN  UINT8            IbeccNum
  )
{
  FUSA_TEST_RESULT                      *TestResultPtr;
  UINT8                                 TotalChecks;
  UINT8                                 McaNum;
  UINT8                                 TestIndex;
  UINT64                                BaseAddress;
  UINT32                                MchBar;
  UINT8                                 TestCount;
  UINT8                                 TrafficMode;
  BOOLEAN                               MsrResult;
  MC0_IBECC_PARITY_ERR_INJ_STRUCT       IbeccParityErrInj;
  MC0_IBECC_PARITY_ERR_LOG_STRUCT       IbeccParityErrLog;
  MC0_IBECC_PARITY_CONTROL_STRUCT       IbeccParityControl;
  EFI_PHYSICAL_ADDRESS                  MemoryAddress;

  static const UINT8  IbeccTestList[] = {WR_DATA_ERR, RD_ADDR_ERR, WR_HDR_ERR, RD_DATA_ERR, RD_HDR_ERR};

  // Init Test Results
  TotalChecks = sizeof (IbeccTestList);
  McaNum = MCA_IOP;

  TestIndex = IbeccNum;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumIbecc0Cmi + IbeccNum;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (UINT8 i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  DEBUG ((DEBUG_INFO, "\nIBECC%d\n", IbeccNum));

  // Get MchBar
  BaseAddress = PCI_SEGMENT_LIB_ADDRESS (0, 0, 0, 0, 0x48);
  MchBar = (PciSegmentRead32 (BaseAddress)) & ~BIT0;
  DEBUG ((DEBUG_INFO, "MCHBAR 0x%lx\n", MchBar));

  // Factor in register offsets for Ibecc1
  if (IbeccNum == 1) {
    MchBar = MchBar + 0x10000;
  }

  // Check Ibecc activate register
  if ((MmioRead32(MchBar + MC0_IBECC_ACTIVATE_REG)) == 0xffffffff) {
    DEBUG ((DEBUG_INFO, "Test aborted: IBECC%d Disabled\n", IbeccNum));

    // Generate Crc32
    TestResultPtr = &pFusaTestResult[TestIndex];
    pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
      (UINT8 *) TestResultPtr,
      sizeof (FUSA_TEST_RESULT) - 4,
      0xffffffffU
    );

    DEBUG ((DEBUG_INFO, "\n"));
    DumpResults (TestResultPtr);

    return;
  }

  // Dump Ibecc Parity Control
  IbeccParityControl.Data = MmioRead32(MchBar + MC0_IBECC_PARITY_CONTROL_REG);
  DEBUG ((DEBUG_INFO, "Parity Control = 0x%lx\n", IbeccParityControl.Data));

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Clear Parity Err Log
  // Clear Parity Err Inj
  MmioWrite64 (MchBar + MC0_IBECC_PARITY_ERR_LOG_REG, BIT63);
  MmioWrite64 (MchBar + MC0_IBECC_PARITY_ERR_INJ_REG, 0);

  // Ibecc Parity Injection
  IbeccParityErrInj.Data = 0;
  for (TestCount = 0; TestCount < sizeof (IbeccTestList); TestCount++) {
    switch (IbeccTestList[TestCount]) {
      case WR_DATA_ERR:
        DEBUG ((DEBUG_INFO, "WR_DATA_ERR\n"));
        IbeccParityErrInj.Bits.WR_DATA_ERR_EN = 0xFF;
        TrafficMode = TrafficWrite;
        break;
      case RD_ADDR_ERR:
        DEBUG ((DEBUG_INFO, "RD_ADDR_ERR\n"));
        IbeccParityErrInj.Bits.RD_ADDR_ERR_EN = 0x1;
        TrafficMode = TrafficRead;
        break;
      case WR_HDR_ERR:
        DEBUG ((DEBUG_INFO, "WR_HDR_ERR\n"));
        IbeccParityErrInj.Bits.WR_HDR_ERR_EN = 0x3;
        TrafficMode = TrafficWrite;
        break;
      case RD_DATA_ERR:
        DEBUG ((DEBUG_INFO, "RD_DATA_ERR\n"));
        IbeccParityErrInj.Bits.RD_DATA_ERR_EN = 0xFF;
        TrafficMode = TrafficRead;
        break;
      case RD_HDR_ERR:
        DEBUG ((DEBUG_INFO, "RD_HDR_ERR\n"));
        IbeccParityErrInj.Bits.RD_HDR_ERR_EN = 0x3;
        TrafficMode = TrafficRead;
        break;
      default:
        break;
    }
    DEBUG ((DEBUG_INFO, "IbeccParityErrInj 0x%llx\n", IbeccParityErrInj.Data));
    MmioWrite64 (MchBar + MC0_IBECC_PARITY_ERR_INJ_REG, IbeccParityErrInj.Data);

    MicroSecondDelay (1000);
    AsmWbinvd();
    // Generate traffic
    MemoryAddress = (IbeccNum == 0) ? 0x2000000 : 0x2013000;
    DEBUG ((DEBUG_INFO, "MemoryAddress 0x%llx\n", MemoryAddress));
    GenerateTraffic (TrafficMode, MemoryAddress);

    // Read Result
    MsrResult = GetSetMcaStatus (McaParity);

    // Dump Parity Err Log
    IbeccParityErrLog.Data = MmioRead64 (MchBar + MC0_IBECC_PARITY_ERR_LOG_REG);
    DEBUG ((DEBUG_INFO, "IbeccParityErrLog %llx\n", IbeccParityErrLog.Data));

    // Populate Result
    pFusaTestResult[TestIndex].CheckResults[TestCount] = MsrResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;

    if (!MsrResult) {
      pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
    } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
      // If there is already other subtest that failed, do not update it to pass.
      pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
    }

    // Clear Parity Err Log ERR_STS; this also clears PERR_OVERFLOW
    // Clear Parity Err Inj
    // Clear MCA Bank
    IbeccParityErrLog.Bits.ERR_STS = 1;
    MmioWrite64 (MchBar + MC0_IBECC_PARITY_ERR_LOG_REG, IbeccParityErrLog.Data);
    IbeccParityErrInj.Data = 0;
    MmioWrite64 (MchBar + MC0_IBECC_PARITY_ERR_INJ_REG, IbeccParityErrInj.Data);
    McaBankInfoClear (McaNum);
  }

  // Generate Crc32
  TestResultPtr = &pFusaTestResult[TestIndex];
  pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
    (UINT8 *) TestResultPtr,
    sizeof (FUSA_TEST_RESULT) - 4,
    0xffffffffU
  );

  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);
}

VOID
FspDxCheckIbeccParity (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  FspDxCheckIbeccParityGeneric (pFusaTestResult, 0); //Ibecc0
  FspDxCheckIbeccParityGeneric (pFusaTestResult, 1); //Ibecc1

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckIbeccEccInjGeneric (
  OUT FUSA_TEST_RESULT *pFusaTestResult,
  IN  UINT8            IbeccNum,
  IN  UINT8            InjErrType
  )
{
  FUSA_TEST_RESULT                      *TestResultPtr;
  UINT8                                 TotalChecks;
  UINT8                                 McaNum;
  UINT8                                 TestIndex;
  UINT64                                BaseAddress;
  UINT32                                MchBar;
  BOOLEAN                               MsrResult;
  MC0_IBECC_ECC_INJ_ADDR_BASE_STRUCT    IbeccEccInjAddrBase;
  MC0_IBECC_ECC_INJ_ADDR_MASK_STRUCT    IbeccEccInjAddrMask;
  MC0_IBECC_ECC_INJ_CONTROL_STRUCT      IbeccEccInjControl;
  MC0_IBECC_ECC_ERROR_LOG_STRUCT        IbeccEccErrLog;
  EFI_PHYSICAL_ADDRESS                  MemoryAddress;

  // Init Test Results
  TotalChecks = 1;
  McaNum = MCA_IOP;
  MemoryAddress = (IbeccNum == 0) ? 0x2000000 : 0x2013000;

  TestIndex = IbeccNum + ((InjErrType == IbeccCorr) ? 0 : 2);
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumIbecc0EccCorrError + IbeccNum + ((InjErrType == IbeccCorr) ? 0 : 2);
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (UINT8 i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  DEBUG ((DEBUG_INFO, "\nIBECC%d\n", IbeccNum));

  // Get MchBar
  BaseAddress = PCI_SEGMENT_LIB_ADDRESS (0, 0, 0, 0, 0x48);
  MchBar = (PciSegmentRead32 (BaseAddress)) & ~BIT0;
  DEBUG ((DEBUG_INFO, "MCHBAR 0x%lx\n", MchBar));

  // Factor in register offsets for Ibecc1
  if (IbeccNum == 1) {
    MchBar = MchBar + 0x10000;
  }

  // Check Ibecc activate register
  if ((MmioRead32(MchBar + MC0_IBECC_ACTIVATE_REG)) == 0xffffffff) {
    DEBUG ((DEBUG_INFO, "Test aborted: IBECC%d Disabled\n", IbeccNum));

    // Generate Crc32
    TestResultPtr = &pFusaTestResult[TestIndex];
    pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
      (UINT8 *) TestResultPtr,
      sizeof (FUSA_TEST_RESULT) - 4,
      0xffffffffU
    );

    DEBUG ((DEBUG_INFO, "\n"));
    DumpResults (TestResultPtr);

    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Clear Ecc Log
  MmioWrite64 (MchBar + MC0_IBECC_ECC_ERROR_LOG_REG, (MmioRead64 (MchBar + MC0_IBECC_ECC_ERROR_LOG_REG)));

  // Set Inj Base
  IbeccEccInjAddrBase.Data = MemoryAddress;
  DEBUG ((DEBUG_INFO, "IbeccEccInjAddrBase %llx\n", IbeccEccInjAddrBase.Data));
  MmioWrite64 (MchBar + MC0_IBECC_ECC_INJ_ADDR_BASE_REG, IbeccEccInjAddrBase.Data);

  // Set Inj Mask
  IbeccEccInjAddrMask.Data = 0xffffffffffffffff;
  DEBUG ((DEBUG_INFO, "IbeccEccInjAddrMask %llx\n", IbeccEccInjAddrMask.Data));
  MmioWrite64 (MchBar + MC0_IBECC_ECC_INJ_ADDR_MASK_REG, IbeccEccInjAddrMask.Data);

  // Set Inj Type
  IbeccEccInjControl.Data = 0;
  IbeccEccInjControl.Bits.ECC_INJECT = ((InjErrType == IbeccCorr) ? 1 : 5);
  DEBUG ((DEBUG_INFO, "%a\n", (InjErrType == IbeccCorr) ? "Correctable" : "Uncorrectable"));
  MmioWrite32 (MchBar + MC0_IBECC_ECC_INJ_CONTROL_REG, IbeccEccInjControl.Data);

  // Generate Traffic
  GenerateTraffic (TrafficWrite, MemoryAddress);
  AsmWbinvd();
  GenerateTraffic (TrafficRead, MemoryAddress);

  // Stop Inj
  MmioWrite32 (MchBar + MC0_IBECC_ECC_INJ_CONTROL_REG, 0);

  // Dump Err Log
  IbeccEccErrLog.Data = MmioRead64 (MchBar + MC0_IBECC_ECC_ERROR_LOG_REG);
  DEBUG ((DEBUG_INFO, "IbeccEccErrLog %llx\n", IbeccEccErrLog.Data));

  // Read Result
  MsrResult = GetSetMcaStatus ((InjErrType == IbeccCorr) ? McaCorrectable : McaUncorrectable);

  // Populate Result
  pFusaTestResult[TestIndex].CheckResults[0] = MsrResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
  if (!MsrResult) {
    pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
  } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
    // If there is already other subtest that failed, do not update it to pass.
    pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
  }

  // Clear Ecc Err Log
  MmioWrite64 (MchBar + MC0_IBECC_ECC_ERROR_LOG_REG, (MmioRead64 (MchBar + MC0_IBECC_ECC_ERROR_LOG_REG)));
  // Clear MCA Bank
  McaBankInfoClear (McaNum);

  // Generate Crc32
  TestResultPtr = &pFusaTestResult[TestIndex];
  pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
    (UINT8 *) TestResultPtr,
    sizeof (FUSA_TEST_RESULT) - 4,
    0xffffffffU
  );

  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);
}

VOID
FspDxCheckIbeccEccInj (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  FspDxCheckIbeccEccInjGeneric (pFusaTestResult, 0, IbeccCorr);
  FspDxCheckIbeccEccInjGeneric (pFusaTestResult, 1, IbeccCorr);
  FspDxCheckIbeccEccInjGeneric (pFusaTestResult, 0, IbeccUncorr);
  FspDxCheckIbeccEccInjGeneric (pFusaTestResult, 1, IbeccUncorr);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}