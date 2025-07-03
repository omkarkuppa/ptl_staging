/** @file
  PEI FUSA CFI Parity

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
#include <Library/TimerLib.h>
#include <Library/CpuLib.h>
#include <Register/CpuGenInfoRegs.h>
#include <Register/Intel/ArchitecturalMsr.h>
#include "PeiFusaResultReportingLib.h"
#include "PeiFusaCfiParity.h"

UINT32 FusaGlobalEnablementReg;
UINT32 FusaParityStsCtl0Reg;
UINT32 FusaParityCtcCtl0Reg;

FUSA_GLOBAL_ENABLEMENT_STRUCT FusaGlobalEnablement;
FUSA_PARITY_STS_CTL_0_STRUCT  FusaParityStsCtl0;
FUSA_PARITY_CTC_CTL_0_STRUCT  FusaParityCtcCtl0;

/**
  Perform CFI Parity Ctc.
**/
VOID
FspDxCheckHbo0E2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  ExpectedMcaStatus = ((0x000BULL) << 16 | 0x010BULL); // [MSCOD << 16 | MCACOD]
  McaNum = MCA_HBO0;

  // Init Test Results
  TotalChecks = 2; // RX0 RX1

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumHbo0E2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_HBO0, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_HBO0, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_HBO0, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_Req_Chk_En        = 1;
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_Req_Fault_inj     = 1;
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX1_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX1_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX0_Req_Chk_Sts       & \
                        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX0_Req_Fault_Res     & \
                        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
          break;
        case RX1:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX1_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX1_Data_Fault_Res;
          break;
        case RX2:
        case RX3:
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x00770000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckHbo1E2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  ExpectedMcaStatus = ((0x000BULL) << 16 | 0x010BULL); // [MSCOD << 16 | MCACOD]
  McaNum = MCA_HBO1;

  // Init Test Results
  TotalChecks = 2; // RX0 RX1

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumHbo1E2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_HBO1, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_HBO1, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_HBO1, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_Req_Chk_En        = 1;
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_Req_Fault_inj     = 1;
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX1_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX1_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX0_Req_Chk_Sts       & \
                        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX0_Req_Fault_Res     & \
                        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
          break;
        case RX1:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX1_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX1_Data_Fault_Res;
          break;
        case RX2:
        case RX3:
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x00770000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckSNCUE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  ExpectedMcaStatus = ((0x0A01ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  TotalChecks = 1; // RX0
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumSNCUE2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_SNCU, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_SNCU, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_SNCU, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
          break;
        case RX1:
        case RX2:
        case RX3:
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x00070000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckSVTUE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  ExpectedMcaStatus = ((0x0501ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  TotalChecks = 1; // RX0
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumSVTUE2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_VTU, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_VTU, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_VTU, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_Req_Chk_En        = 1;
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_Req_Fault_inj     = 1;
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX0_Req_Chk_Sts       & \
                        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX0_Req_Fault_Res     & \
                        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
          break;
        case RX1:
        case RX2:
        case RX3:
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x77770000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckPunitE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  ExpectedMcaStatus = ((0x0701ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  TotalChecks = 1; // RX0 RX1
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumPunitE2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_PUNIT, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_PUNIT, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_PUNIT, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
          break;
        case RX1:
        case RX2:
        case RX3:
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x00070000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckIPU_PB_PIDE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  ExpectedMcaStatus = ((0x0201ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  TotalChecks = 2; // RX0 RX1
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumIPU_PB_PIDE2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_IPU_PB_PID, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_IPU_PB_PID, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_IPU_PB_PID, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }
  FusaParityStsCtl0.Data = MmioRead32 (FusaParityStsCtl0Reg);
  if (FusaParityStsCtl0.Data == 0xffffffff) {
    DEBUG ((DEBUG_INFO, "Test aborted: IPU not available\n"));

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

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX1_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX1_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
          break;
        case RX1:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX1_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX1_Data_Fault_Res;
          break;
        case RX2:
        case RX3:
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x77770000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckVPU_PB_PIDE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  ExpectedMcaStatus = ((0x0901ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  TotalChecks = 4; // RX0 RX1 RX2 RX3
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumVPU_PB_PIDE2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_VPU_PB_PID, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_VPU_PB_PID, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_VPU_PB_PID, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX1_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX1_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX2:
        FusaParityStsCtl0.Bits.RX2_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX2_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX2_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX2_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX3:
        FusaParityStsCtl0.Bits.RX3_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX3_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX3_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX3_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));    

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
            CheckResult = CheckResult                                & \
                        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
            break;
        case RX1:
            CheckResult = CheckResult                                & \
                        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX1_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX1_Data_Fault_Res;
            break;
        case RX2:
            CheckResult = CheckResult                                & \
                        FusaParityStsCtl0.Bits.RX2_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX2_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX2_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX2_Data_Fault_Res;
            break;
        case RX3:
            CheckResult = CheckResult                                & \
                        FusaParityStsCtl0.Bits.RX3_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX3_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX3_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX3_Data_Fault_Res;
            break;
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x77770000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckMEDIA_PBE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  ExpectedMcaStatus = ((0x2101ULL) << 16 | 0x0E03ULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  TotalChecks = 2; // RX0 RX1
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumMEDIA_PBE2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_MEDIA_PB, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_MEDIA_PB, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_MEDIA_PB, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_Req_Chk_En        = 1;
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_Req_Fault_inj     = 1;
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
        FusaParityStsCtl0.Bits.RX1_Req_Chk_En        = 1;
        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX1_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX1_Req_Fault_inj     = 1;
        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX1_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX0_Req_Chk_Sts       & \
                        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX0_Req_Fault_Res     & \
                        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
          break;
        case RX1:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX1_Req_Chk_Sts       & \
                        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX1_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX1_Req_Fault_Res     & \
                        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX1_Data_Fault_Res;
          break;
        case RX2:
        case RX3:
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x77770000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckGT_PBE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  ExpectedMcaStatus = ((0x2201ULL) << 16 | 0x0E03ULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  TotalChecks = 4; // RX0 RX1 RX2 RX3
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumGT_PBE2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_GT_PB, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_GT_PB, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_GT_PB, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_Req_Chk_En        = 1;
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_Req_Fault_inj     = 1;
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
        FusaParityStsCtl0.Bits.RX1_Req_Chk_En        = 1;
        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX1_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX1_Req_Fault_inj     = 1;
        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX1_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX2:
        FusaParityStsCtl0.Bits.RX2_Req_Chk_En        = 1;
        FusaParityStsCtl0.Bits.RX2_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX2_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX2_Req_Fault_inj     = 1;
        FusaParityCtcCtl0.Bits.RX2_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX2_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX3:
        FusaParityStsCtl0.Bits.RX3_Req_Chk_En        = 1;
        FusaParityStsCtl0.Bits.RX3_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX3_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX3_Req_Fault_inj     = 1;
        FusaParityCtcCtl0.Bits.RX3_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX3_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));    

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
            CheckResult = CheckResult                                & \
                        FusaParityStsCtl0.Bits.RX0_Req_Chk_Sts       & \
                        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX0_Req_Fault_Res     & \
                        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
            break;
        case RX1:
            CheckResult = CheckResult                                & \
                        FusaParityStsCtl0.Bits.RX1_Req_Chk_Sts       & \
                        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX1_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX1_Req_Fault_Res     & \
                        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX1_Data_Fault_Res;
            break;
        case RX2:
            CheckResult = CheckResult                                & \
                        FusaParityStsCtl0.Bits.RX2_Req_Chk_Sts       & \
                        FusaParityStsCtl0.Bits.RX2_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX2_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX2_Req_Fault_Res     & \
                        FusaParityCtcCtl0.Bits.RX2_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX2_Data_Fault_Res;
            break;
        case RX3:
            CheckResult = CheckResult                                & \
                        FusaParityStsCtl0.Bits.RX3_Req_Chk_Sts       & \
                        FusaParityStsCtl0.Bits.RX3_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX3_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX3_Req_Fault_Res     & \
                        FusaParityCtcCtl0.Bits.RX3_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX3_Data_Fault_Res;
            break;
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckIAX_PBE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  ExpectedMcaStatus = ((0x2001ULL) << 16 | 0x0E03ULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  TotalChecks = 1; // RX0
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumIAX_PBE2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_IAX_PB, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_IAX_PB, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_IAX_PB, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_Req_Chk_En        = 1;
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_Req_Fault_inj     = 1;
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX0_Req_Chk_Sts       & \
                        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX0_Req_Fault_Res     & \
                        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
          break;
        case RX1:
        case RX2:
        case RX3:
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x77770000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckDNI2CFIE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  ExpectedMcaStatus = ((0x2301ULL) << 16 | 0x0E03ULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  TotalChecks = 3; // RX0 RX1 RX2
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumDNI2CFIE2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_DNI2CFI, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_DNI2CFI, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_DNI2CFI, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX1_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX1_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX2:
        FusaParityStsCtl0.Bits.RX2_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX2_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX2_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX2_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
          CheckResult = CheckResult                                    & \
                          FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                          FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                          FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                          FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
          break;
        case RX1:
          CheckResult = CheckResult                                    & \
                          FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_Sts   & \
                          FusaParityStsCtl0.Bits.RX1_Data_Chk_Sts      & \
                          FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_Res & \
                          FusaParityCtcCtl0.Bits.RX1_Data_Fault_Res;
          break;
        case RX2:
          CheckResult = CheckResult                                    & \
                          FusaParityStsCtl0.Bits.RX2_DataHdr_Chk_Sts   & \
                          FusaParityStsCtl0.Bits.RX2_Data_Chk_Sts      & \
                          FusaParityCtcCtl0.Bits.RX2_DataHdr_Fault_Res & \
                          FusaParityCtcCtl0.Bits.RX2_Data_Fault_Res;
          break;
        case RX3:
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x07770000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

/**
  Perform Idi Parity Ctc.
**/
VOID
FspDxCheckIdiBE2eIdiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 CheckNum;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  FUSA_TEST_RESULT      *TestResultPtr;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;
  UINT8                 PortId;

  FUSA_IDI_PARITY_STS_CTL_0_STRUCT FusaIdiParityStsCtl0;
  FUSA_IDI_PARITY_CTC_CTL_0_STRUCT FusaIdiParityCtcCtl0;

  static const UINT8  TestList[] = {IdiE2eIdiParityCtc_Addr, IdiE2eIdiParityCtc_Be, IdiE2eIdiParityCtc_Nc_Be};

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  ExpectedMcaStatus = ((0x0C01ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  // 1 ADDRESS
  // 2 BE
  // 3 NC_BE
  TotalChecks = sizeof (TestList);
  McaNum = MCA_IOP;
  PortId = PORTID_IDIB;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumIDIBE2EIDIParityCtC;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

    // Init Register Address
  Data64 = 0x420ULL; // IDI_B_FuSa_Parity_Sts_Ctl_0
  Status = (UINT32) CfiRegisterAddress (PortId, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x424ULL; // IDI_B_FuSa_Parity_Ctc_Ctl_0
  Status = (UINT32) CfiRegisterAddress (PortId, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  for (CheckNum = 0; CheckNum < TotalChecks; CheckNum++) {
    DEBUG ((DEBUG_INFO, "CheckNum  : %d\n", CheckNum));

    // Injection
    FusaIdiParityStsCtl0.Data = 0x0;
    FusaIdiParityCtcCtl0.Data = 0x0;

    switch (CheckNum) {
      case IdiE2eIdiParityCtc_Addr:
        DEBUG ((DEBUG_INFO, "Injection : Addr\n"));
        FusaIdiParityStsCtl0.Bits.addr_chk_en = 1;
        FusaIdiParityCtcCtl0.Bits.addr_chk_fault_inj = 1;
        break;
      case IdiE2eIdiParityCtc_Be:
        DEBUG ((DEBUG_INFO, "Injection : Be\n"));
        FusaIdiParityStsCtl0.Bits.be_chk_en = 1;
        FusaIdiParityCtcCtl0.Bits.be_chk_fault_inj = 1;
        break;
      case IdiE2eIdiParityCtc_Nc_Be:
        DEBUG ((DEBUG_INFO, "Injection : Nc_Be\n"));
        FusaIdiParityStsCtl0.Bits.nc_be_chk_en = 1;
        FusaIdiParityCtcCtl0.Bits.nc_be_chk_fault_inj = 1;
        break;
      default:
        break;
    }
    MmioWrite32 (FusaParityStsCtl0Reg, FusaIdiParityStsCtl0.Data);
    MmioWrite32 (FusaParityCtcCtl0Reg, FusaIdiParityCtcCtl0.Data);

    // Check MCA
    CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
    DEBUG ((DEBUG_INFO, "MC%d_STATUS           : 0x%llx\n", McaNum, McaResult));

    // Check Injection Results
    FusaIdiParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
    FusaIdiParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
    DEBUG ((DEBUG_INFO, "FusaIdiParityStsCtl0 : 0x%lx\n", FusaIdiParityStsCtl0.Data));
    DEBUG ((DEBUG_INFO, "FusaIdiParityCtcCtl0 : 0x%lx\n", FusaIdiParityCtcCtl0.Data));

    switch (CheckNum) {
      case IdiE2eIdiParityCtc_Addr:
        CheckResult = CheckResult                                  & \
                      FusaIdiParityStsCtl0.Bits.addr_chk_sts       & \
                      FusaIdiParityCtcCtl0.Bits.addr_fault_res;
        break;
      case IdiE2eIdiParityCtc_Be:
        CheckResult = CheckResult                                  & \
                      FusaIdiParityStsCtl0.Bits.be_chk_sts         & \
                      FusaIdiParityCtcCtl0.Bits.be_fault_res;
        break;
      case IdiE2eIdiParityCtc_Nc_Be:
        CheckResult = CheckResult                                  & \
                      FusaIdiParityStsCtl0.Bits.nc_be_chk_sts      & \
                      FusaIdiParityCtcCtl0.Bits.nc_be_fault_res;
        break;
      default:
        break;
    }

    // Update CheckResults
    pFusaTestResult[TestIndex].CheckResults[CheckNum] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
    DEBUG ((DEBUG_INFO, "CheckResult          : %a\n\n", CheckResult ? "PASS" : "FAIL"));

    // Update TestResult
    if (!CheckResult) {
      pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
    } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
      pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
    }

    // Cleanup
    MmioWrite32 (FusaParityStsCtl0Reg, BIT2 | BIT1 | BIT0);
    //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
    //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
    MmioWrite32 (FusaParityCtcCtl0Reg, BIT2 | BIT1 | BIT0);
    //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
    //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
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

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckCCE0E2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;
  
  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;
  
  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));
  
  ExpectedMcaStatus = ((0x000ULL) << 16 | 0x0444ULL); // [MSCOD << 16 | MCACOD]
  
  // Init Test Results
  TotalChecks = 2; // RX0 RX1
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCCE0E2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_CCE0, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_CCE0, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_CCE0, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_Req_Chk_En        = 1;
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_Req_Fault_inj     = 1;
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX1_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX1_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX0_Req_Chk_Sts       & \
                        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX0_Req_Fault_Res     & \
                        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
          break;
        case RX1:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX1_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX1_Data_Fault_Res;
          break;
        case RX2:
        case RX3:
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));

      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x77770000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckCCE1E2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;
  
  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;
  
  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));
  
  ExpectedMcaStatus = ((0x000ULL) << 16 | 0x0444ULL); // [MSCOD << 16 | MCACOD]
  
  // Init Test Results
  TotalChecks = 2; // RX0 RX1
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCCE1E2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_CCE1, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_CCE1, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_CCE1, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }
  FusaParityStsCtl0.Data = MmioRead32 (FusaParityStsCtl0Reg);
  if (FusaParityStsCtl0.Data == 0xffffffff) {
    DEBUG ((DEBUG_INFO, "Test aborted: CCE1 not available\n"));

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

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_Req_Chk_En        = 1;
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_Req_Fault_inj     = 1;
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX1_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX1_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX0_Req_Chk_Sts       & \
                        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX0_Req_Fault_Res     & \
                        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
          break;
        case RX1:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_Sts   & \
                        FusaParityStsCtl0.Bits.RX1_Data_Chk_Sts      & \
                        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_Res & \
                        FusaParityCtcCtl0.Bits.RX1_Data_Fault_Res;
          break;
        case RX2:
        case RX3:
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x77770000);

      McaBankInfoClear (McaNum);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckCCF_SANTA1E2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;
  
  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;
  
  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));
  
  ExpectedMcaStatus = ((0x0B01ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  TotalChecks = 1; // RX0
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCCF_SANTA1E2EParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_CCF_SANTA1, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_CCF_SANTA1, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_CCF_SANTA1, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res;
          break;
        case RX1:
        case RX2:
        case RX3:
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x77770000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckCCF_SANTA0E2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;
  
  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;
  
  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));
  
  ExpectedMcaStatus = ((0x0B01ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  TotalChecks = 2; // RX0 RX1
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCCF_SANTA0E2EParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_CCF_SANTA0, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_CCF_SANTA0, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_CCF_SANTA0, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_En    = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_inj = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res;
          break;
        case RX1:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_Sts   & \
                        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_Res;
          break;
        case RX2:
        case RX3:
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x77770000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckIOCE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;
  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  ExpectedMcaStatus = ((0x0601ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  TotalChecks = 2; // RX0 RX1
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumIOCE2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_IOC, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_IOC, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_IOC, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_Req_Chk_En        = 1;
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_Req_Fault_inj     = 1;
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
        FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX1_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX1_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));


      if ((GetCpuSteppingId () == EnumPtlHA0) || (GetCpuSteppingId () == EnumPtlUA0)) {
        CheckResult = 1;
      }
      else {
        // Check MCA
        CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
        DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

        switch (Channel) {
          case RX0:
            CheckResult = CheckResult                                  & \
                          FusaParityStsCtl0.Bits.RX0_Req_Chk_Sts       & \
                          FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                          FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                          FusaParityCtcCtl0.Bits.RX0_Req_Fault_Res     & \
                          FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                          FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
            break;
          case RX1:
            CheckResult = CheckResult                                  & \
                          FusaParityStsCtl0.Bits.RX1_DataHdr_Chk_Sts   & \
                          FusaParityStsCtl0.Bits.RX1_Data_Chk_Sts      & \
                          FusaParityCtcCtl0.Bits.RX1_DataHdr_Fault_Res & \
                          FusaParityCtcCtl0.Bits.RX1_Data_Fault_Res;
            break;
          case RX2:
          case RX3:
          default:
            break;
        }
      }
      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x77770000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckIOCCEE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;
  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  ExpectedMcaStatus = ((0x0801ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  TotalChecks = 1; // RX0 RX1
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumIOCCEE2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_IOCCE, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_IOCCE, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_IOCCE, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));


      if ((GetCpuSteppingId () == EnumPtlHA0) || (GetCpuSteppingId () == EnumPtlUA0)){
        CheckResult = 1;
      }
      else{

        // Check MCA
        CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
        DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

        switch (Channel) {
          case RX0:
            CheckResult = CheckResult                                  & \
                          FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                          FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                          FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                          FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
            break;
          case RX1:
          case RX2:
          case RX3:
          default:
            break;
        }
      }
      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x77770000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckIVTUE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  ExpectedMcaStatus = ((0x0401ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  TotalChecks = 1; // RX0
  McaNum = MCA_IOP;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumIVTUE2eCfiParityCtc;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_IVTU, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x300ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_IVTU, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x308ULL;
  Status = (UINT32) CfiRegisterAddress (PORTID_IVTU, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_Req_Chk_En        = 1;
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        FusaParityStsCtl0.Bits.RX0_Data_Chk_En       = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_Req_Fault_inj     = 1;
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        FusaParityCtcCtl0.Bits.RX0_Data_Fault_inj    = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));

      
      if ((GetCpuSteppingId () == EnumPtlHA0) || (GetCpuSteppingId () == EnumPtlUA0)) {
        CheckResult = 1;
      }
      else{
      // Check MCA
        CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
        DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

        switch (Channel) {
          case RX0:
            CheckResult = CheckResult                                  & \
                          FusaParityStsCtl0.Bits.RX0_Req_Chk_Sts       & \
                          FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                          FusaParityStsCtl0.Bits.RX0_Data_Chk_Sts      & \
                          FusaParityCtcCtl0.Bits.RX0_Req_Fault_Res     & \
                          FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res & \
                          FusaParityCtcCtl0.Bits.RX0_Data_Fault_Res;
            break;
          case RX1:
          case RX2:
          case RX3:
          default:
            break;
        }
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Restore default value
      MmioWrite32 (FusaParityStsCtl0Reg, 0x77770000);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

VOID
FspDxCheckIdiBE2eCfiParityCtc (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS            Status;
  UINT8                 TestIndex;
  UINT8                 CheckResult = 0;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT64                Data64;
  UINT8                 Channel;
  FUSA_TEST_RESULT      *TestResultPtr;
  BOOLEAN               SkipChannel = FALSE;
  
  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaResult;
  UINT8                 McaNum;
  UINT8                 PortId;
  
  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));
  
  ExpectedMcaStatus = ((0x0C01ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]

  // Init Test Results
  TotalChecks = 1; // RX0
  McaNum = MCA_IOP;
  PortId = PORTID_IDIB;

  TestIndex = 0;
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumIDIBE2ECfiParityCtC;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Init Register Address
  Data64 = 0x080ULL;
  Status = (UINT32) CfiRegisterAddress (PortId, &Data64);
  FusaGlobalEnablementReg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablementReg %lx\n", FusaGlobalEnablementReg));

  Data64 = 0x420ULL;
  Status = (UINT32) CfiRegisterAddress (PortId, &Data64);
  FusaParityStsCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityStsCtl0Reg    %lx\n", FusaParityStsCtl0Reg));

  Data64 = 0x424ULL;
  Status = (UINT32) CfiRegisterAddress (PortId, &Data64);
  FusaParityCtcCtl0Reg = (UINT32) Data64;
  //DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0Reg    %lx\n", FusaParityCtcCtl0Reg));

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Test aborted\n"));
    return;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  // Enable Parity Check
  // Enable Parity Generation
  FusaGlobalEnablement.Data = 0;
  FusaGlobalEnablement.Bits.CFI_ParityChk_En = 1;
  FusaGlobalEnablement.Bits.CFI_ParityGen_En = 1;
  MmioWrite32 (FusaGlobalEnablementReg, FusaGlobalEnablement.Data);
  //FusaGlobalEnablement.Data = MmioRead32(FusaGlobalEnablementReg);
  //DEBUG ((DEBUG_INFO, "FusaGlobalEnablement %lx\n", FusaGlobalEnablement.Data));

  for (Channel = 0; Channel <= RX3; Channel++) {
    DEBUG ((DEBUG_INFO, "\n[RX%d]\n", Channel));
    FusaParityStsCtl0.Data = 0;
    FusaParityCtcCtl0.Data = 0;
    switch (Channel) {
      // Enable Parity Check
      // Enable Parity Error Injection
      case RX0:
        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_En    = 1;
        MmioWrite32 (FusaParityStsCtl0Reg, FusaParityStsCtl0.Data);
        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_inj = 1;
        MmioWrite32 (FusaParityCtcCtl0Reg, FusaParityCtcCtl0.Data);
        SkipChannel = FALSE;
        break;
      case RX1:
      case RX2:
      case RX3:
      default:
        SkipChannel = TRUE;
        DEBUG ((DEBUG_INFO, "-- None --\n"));
        break;
    }

    if (!SkipChannel) {
      // Check injection result
      FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      DEBUG ((DEBUG_INFO, "FusaParityStsCtl0 : 0x%lx\n", FusaParityStsCtl0.Data));
      DEBUG ((DEBUG_INFO, "FusaParityCtcCtl0 : 0x%lx\n", FusaParityCtcCtl0.Data));

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

      switch (Channel) {
        case RX0:
          CheckResult = CheckResult                                  & \
                        FusaParityStsCtl0.Bits.RX0_DataHdr_Chk_Sts   & \
                        FusaParityCtcCtl0.Bits.RX0_DataHdr_Fault_Res;
          break;
        case RX1:
        case RX2:
        case RX3:
        default:
          break;
      }

      // Update CheckResults
      pFusaTestResult[TestIndex].CheckResults[Channel] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
      DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));

      // Update TestResult
      if (!CheckResult) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
      } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
        pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
      }

      // Cleanup
      MmioWrite32 (FusaParityStsCtl0Reg, 0x0000FFFF);
      //FusaParityStsCtl0.Data = MmioRead32(FusaParityStsCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityStsCtl0 %lx\n", FusaParityStsCtl0.Data));
      MmioWrite32 (FusaParityCtcCtl0Reg, 0x0000FFFF);
      //FusaParityCtcCtl0.Data = MmioRead32(FusaParityCtcCtl0Reg);
      //DEBUG ((DEBUG_INFO, "Cleanup: FusaParityCtcCtl0 %lx\n", FusaParityCtcCtl0.Data));
      McaBankInfoClear (McaNum);

      // Generate Crc32
      TestResultPtr = &pFusaTestResult[TestIndex];
      pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
        (UINT8 *) TestResultPtr,
        sizeof (FUSA_TEST_RESULT) - 4,
        0xffffffffU
      );
    }
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}