/** @file
  PEI Fusa CCF E2E IDI Parity Ctc

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
#include "PeiFusaCcfE2eIdiParityCtc.h"

/**
  Error injection and corresponding traffic routine for LLC Data

  @param[in] IpIndex - IP index vale for MSR MSR_PERRINJ_AT_IP
  @param[in] InjectionPayload - Payload value for MSR MSR_PERRINJ_CTRL

  @return injection control value after traffic generation
**/
STATIC
UINT32
InjectionAndTraffic(
  IN UINT32 IpIndex,
  IN UINT64 Sts_Ctl_Payload,
  IN UINT64 Ctc_Ctl_Payload
  )
{
  UINT32                                 MemoryData;
  EFI_PHYSICAL_ADDRESS                   MemoryAddress;
  UINT64                                 MsrReg;

  DEBUG ((DEBUG_INFO, "ctc_ctl            : IpIndex 0x%llx Payload 0x%llx\n", IpIndex      , Ctc_Ctl_Payload));
  DEBUG ((DEBUG_INFO, "sts_ctl            : IpIndex 0x%llx Payload 0x%llx\n", (IpIndex + 1), Sts_Ctl_Payload));

  // Parity check en
  AsmWriteMsr64 (MSR_PERRINJ_AT_IP, (IpIndex + 1) | BIT31);
  MsrReg = AsmReadMsr64 (MSR_PERRINJ_CTRL);
  DEBUG ((DEBUG_INFO, "read  sts_ctl      : MsrReg 0x%llx\n", MsrReg));
  MsrReg = MsrReg | Sts_Ctl_Payload;
  DEBUG ((DEBUG_INFO, "write sts_ctl      : MsrReg 0x%llx\n", MsrReg));
  AsmWriteMsr64 (MSR_PERRINJ_CTRL,  MsrReg);

  // Injection
  AsmWriteMsr64 (MSR_PERRINJ_AT_IP, IpIndex | BIT31);
  MsrReg = AsmReadMsr64 (MSR_PERRINJ_CTRL);
  DEBUG ((DEBUG_INFO, "read  ctc_ctl      : MsrReg 0x%llx\n", MsrReg));
  MsrReg = MsrReg | Ctc_Ctl_Payload;
  DEBUG ((DEBUG_INFO, "write ctc_ctl      : MsrReg 0x%llx\n", MsrReg));
  AsmWriteMsr64 (MSR_PERRINJ_CTRL,  MsrReg);

  // Traffic
  MemoryAddress = 0x2000000;
  MemoryData = 0xF0F0F0F0;
  *(volatile UINT32*) (UINTN) MemoryAddress = MemoryData;
  AsmWbinvd();
  MemoryData = *(volatile UINT32*) (UINTN) MemoryAddress;

  return 0;
}

STATIC
UINT32
InjectionAndTrafficNCU(
  IN UINT32 IpIndex,
  IN UINT64 Sts_Ctl_Payload,
  IN UINT64 Ctc_Ctl_Payload
  )
{
  UINT32                                 MemoryData;
  EFI_PHYSICAL_ADDRESS                   MemoryAddress;
  UINT64                                 MsrReg;

  DEBUG ((DEBUG_INFO, "ctc_ctl            : IpIndex 0x%llx Payload 0x%llx\n", IpIndex      , Ctc_Ctl_Payload));
  DEBUG ((DEBUG_INFO, "sts_ctl            : IpIndex 0x%llx Payload 0x%llx\n", (IpIndex + 1), Sts_Ctl_Payload));
  
  // Parity check en
  AsmWriteMsr64 (MSR_PERRINJ_AT_IP, (IpIndex + 1) | BIT31);
  MsrReg = AsmReadMsr64 (MSR_PERRINJ_CTRL);
  DEBUG ((DEBUG_INFO, "read  sts_ctl      : MsrReg 0x%llx\n", MsrReg));
  MsrReg = MsrReg | Sts_Ctl_Payload;
  DEBUG ((DEBUG_INFO, "write sts_ctl      : MsrReg 0x%llx\n", MsrReg));
  AsmWriteMsr64 (MSR_PERRINJ_CTRL,  MsrReg);

  // Injection
  AsmWriteMsr64 (MSR_PERRINJ_AT_IP, IpIndex | BIT31);
  MsrReg = AsmReadMsr64 (MSR_PERRINJ_CTRL);
  DEBUG ((DEBUG_INFO, "read  ctc_ctl      : MsrReg 0x%llx\n", MsrReg));
  MsrReg = MsrReg | Ctc_Ctl_Payload;
  DEBUG ((DEBUG_INFO, "write ctc_ctl      : MsrReg 0x%llx\n", MsrReg));
  AsmWriteMsr64 (MSR_PERRINJ_CTRL,  MsrReg);

  // Traffic
  MemoryAddress = 0x2000000;
  MemoryData = 0xF0F0F0F0;
  *(volatile UINT32*) (UINTN) MemoryAddress = MemoryData;
  AsmWbinvd();
  MemoryData = *(volatile UINT32*) (UINTN) MemoryAddress;

  return 0;
}


/**
  Perform CCF E2E IDI Parity Double Parity.
**/
VOID
FspDxCheckCcfE2eIdiParityCtcGeneric (
  IN FUSA_TEST_RESULT *pFusaTestResult,
  IN UINT8            Slice
  )
{
  UINT8                 TestIndex;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT8                 CheckNum;
  UINT8                 CheckResult = 0;
  UINT8                 McaResult;
  FUSA_TEST_RESULT      *TestResultPtr;
  UINT8                 McaNum;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK;
  UINT64                McaStatus;
  UINT8                 IpIndex;
  UINT64                Ctc_Ctl_Payload;
  UINT64                Sts_Ctl_Payload;

  static const UINT8  TestList[] = {CcfE2eIdiParityCtc_Addr, CcfE2eIdiParityCtc_DataH0, CcfE2eIdiParityCtc_DataH1};

  DEBUG ((DEBUG_INFO, "\nSlice              : %d\n", Slice));

  // Init Test Results
  /*
    CheckNum
    0 Addr
    1 Data H0
    2 Data H1
  */
  TotalChecks = sizeof (TestList);

  switch (Slice) {
    case 0:
      TestIndex = FusaTestNumCCFE2EIDIParityCtC0 - FusaTestNumCCFE2EIDIParityCtC0;
      pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCCFE2EIDIParityCtC0;
      McaNum = MCA_CBO0;
      break;
    case 1:
      TestIndex = FusaTestNumCCFE2EIDIParityCtC1 - FusaTestNumCCFE2EIDIParityCtC0;
      pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCCFE2EIDIParityCtC1;
      McaNum = MCA_CBO1;
      break;
    case 2:
      TestIndex = FusaTestNumCCFE2EIDIParityCtC2 - FusaTestNumCCFE2EIDIParityCtC0;
      pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCCFE2EIDIParityCtC2;
      McaNum = MCA_CBO2;
      break;
    case 3:
      TestIndex = FusaTestNumCCFE2EIDIParityCtC3 - FusaTestNumCCFE2EIDIParityCtC0;
      pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCCFE2EIDIParityCtC3;
      McaNum = MCA_CBO3;
      break;
    case 4:
      TestIndex = FusaTestNumCCFE2EIDIParityCtC4 - FusaTestNumCCFE2EIDIParityCtC0;
      pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCCFE2EIDIParityCtC4;
      McaNum = MCA_CBO4;
      break;
    case 5:
      TestIndex = FusaTestNumCCFE2EIDIParityCtC5 - FusaTestNumCCFE2EIDIParityCtC0;
      pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCCFE2EIDIParityCtC5;
      McaNum = MCA_CBO5;
      break;
    default:
      break;
  }

  IpIndex = 2 + (4 * TestIndex);
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  for (CheckNum = 0; CheckNum < TotalChecks; CheckNum++) {
    DEBUG ((DEBUG_INFO, "CheckNum           : %d\n", CheckNum));

    switch (CheckNum) {
      case CcfE2eIdiParityCtc_Addr:
        DEBUG ((DEBUG_INFO, "Injection          : Addr\n"));
        Ctc_Ctl_Payload =         BIT16;
        Sts_Ctl_Payload = BIT22 | BIT16;
        ExpectedMcaStatus = ((0x0800ULL) << 16); // [MSCOD << 16]
        break;
      case CcfE2eIdiParityCtc_DataH0:
        DEBUG ((DEBUG_INFO, "Injection          : Data H0\n"));
        Ctc_Ctl_Payload =         BIT17;
        Sts_Ctl_Payload = BIT23 | BIT17;
        ExpectedMcaStatus = ((0x2000ULL) << 16); // [MSCOD << 16]
        break;
      case CcfE2eIdiParityCtc_DataH1:
        DEBUG ((DEBUG_INFO, "Injection          : Data H1\n"));
        Ctc_Ctl_Payload =         BIT18;
        Sts_Ctl_Payload = BIT24 | BIT18;
        ExpectedMcaStatus = ((0x2000ULL) << 16); // [MSCOD << 16]
        break;
      default:
        break;
    }

    // Injection
    InjectionAndTraffic(IpIndex, Sts_Ctl_Payload, Ctc_Ctl_Payload);

    // Check MCA
    McaResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaStatus);
    DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaStatus));

    // Check Result
    AsmWriteMsr64 (MSR_PERRINJ_AT_IP, IpIndex     | BIT31);
    Ctc_Ctl_Payload =  AsmReadMsr64 (MSR_PERRINJ_CTRL);
    DEBUG ((DEBUG_INFO, "Ctc_Ctl_Payload    : 0x%llx\n", Ctc_Ctl_Payload));

    AsmWriteMsr64 (MSR_PERRINJ_AT_IP, (IpIndex + 1) | BIT31);
    Sts_Ctl_Payload =  AsmReadMsr64 (MSR_PERRINJ_CTRL);
    DEBUG ((DEBUG_INFO, "Sts_Ctl_Payload    : 0x%llx\n", Sts_Ctl_Payload));

    switch (CheckNum) {
      case CcfE2eIdiParityCtc_Addr:
        CheckResult = ((McaResult == FUSA_TEST_PASS) && \
                      (Ctc_Ctl_Payload & BIT0) && \
                      (Sts_Ctl_Payload & BIT0)) ? \
                      FUSA_TEST_PASS : FUSA_TEST_FAIL;
        break;
      case CcfE2eIdiParityCtc_DataH0:
        CheckResult = ((McaResult == FUSA_TEST_PASS) && \
                      (Ctc_Ctl_Payload & BIT1) && \
                      (Sts_Ctl_Payload & BIT1)) ? \
                      FUSA_TEST_PASS : FUSA_TEST_FAIL;
        break;
      case CcfE2eIdiParityCtc_DataH1:
        CheckResult = ((McaResult == FUSA_TEST_PASS) && \
                      (Ctc_Ctl_Payload & BIT2) && \
                      (Sts_Ctl_Payload & BIT2)) ? \
                      FUSA_TEST_PASS : FUSA_TEST_FAIL;
        break;
      default:
        break;
    }

    // Update CheckResults
    pFusaTestResult[TestIndex].CheckResults[CheckNum] = (CheckResult == FUSA_TEST_PASS) ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
    DEBUG ((DEBUG_INFO, "CheckResult        : %a\n\n", (CheckResult == FUSA_TEST_PASS) ? "PASS" : "FAIL"));

    // Update TestResult
    if (CheckResult == FUSA_TEST_FAIL) {
      pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
    } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
      pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
    }

    // Cleanup
    Ctc_Ctl_Payload = 0x0 | BIT2 | BIT1 | BIT0;
    AsmWriteMsr64 (MSR_PERRINJ_AT_IP, IpIndex     | BIT31);
    AsmWriteMsr64 (MSR_PERRINJ_CTRL,  Ctc_Ctl_Payload);

    Sts_Ctl_Payload = 0x0 | BIT2 | BIT1 | BIT0;
    AsmWriteMsr64 (MSR_PERRINJ_AT_IP, (IpIndex + 1) | BIT31);
    AsmWriteMsr64 (MSR_PERRINJ_CTRL,  Sts_Ctl_Payload);

    Sts_Ctl_Payload = 0x0 | BIT24 | BIT23 | BIT22;
    AsmWriteMsr64 (MSR_PERRINJ_AT_IP, (IpIndex + 1) | BIT31);
    AsmWriteMsr64 (MSR_PERRINJ_CTRL,  Sts_Ctl_Payload);

    McaBankInfoClear (McaNum);
  }

  // Generate Crc32
  TestResultPtr = &pFusaTestResult[TestIndex];
  pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
    (UINT8 *) TestResultPtr,
    sizeof (FUSA_TEST_RESULT) - 4,
    0xffffffffU
  );

  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);
}

/**
  Perform CCF E2E IDI Parity Double Parity.
**/
VOID
FspDxCheckCcfE2eIdiParityCtc (
  IN FUSA_TEST_RESULT *pFusaTestResult
  )
{
  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  FspDxCheckCcfE2eIdiParityCtcGeneric(pFusaTestResult, 0);
  FspDxCheckCcfE2eIdiParityCtcGeneric(pFusaTestResult, 1);
  FspDxCheckCcfE2eIdiParityCtcGeneric(pFusaTestResult, 2);
  FspDxCheckCcfE2eIdiParityCtcGeneric(pFusaTestResult, 3);
  FspDxCheckCcfE2eIdiParityCtcGeneric(pFusaTestResult, 4);
  FspDxCheckCcfE2eIdiParityCtcGeneric(pFusaTestResult, 5);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

// CCF cNCU Injection 
VOID
FspDxCheckCCFE2eNCUParityCtcGeneric (
  IN FUSA_TEST_RESULT *pFusaTestResult
  )
{
  UINT8                 TestIndex;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT8                 CheckNum;
  UINT8                 CheckResult = 0;
  UINT8                 McaResult;
  FUSA_TEST_RESULT      *TestResultPtr;
  UINT8                 McaNum;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaStatus;
  UINT8                 IpIndex;
  UINT64                Ctc_Ctl_Payload;
  UINT64                Sts_Ctl_Payload;
  
  // Init Test Results
  /*
    CheckNum
    0 Rx_Data_Chk_en
    1 Tx_Data_Chk_en
  */
  TotalChecks = 2; // RX and TX
  McaNum = MCA_IOP;
  IpIndex = 26;
  TestIndex = 0; 
  pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCCFE2ECNCUParityCtC0;
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL); 
  McaBankInfoClear (McaNum);

  for (CheckNum = 0; CheckNum < TotalChecks; CheckNum++) {
    DEBUG ((DEBUG_INFO, "CheckNum           : %d\n", CheckNum));

    switch (CheckNum) {
      case CCFE2eNCUParityCtc_Rx_Data_Chk_en:
        DEBUG ((DEBUG_INFO, "Injection          : Rx_Data_Chk_en\n"));
        Ctc_Ctl_Payload =         BIT16;
        Sts_Ctl_Payload =  BIT31 |BIT16;
        ExpectedMcaStatus = ((0x0B01ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]
        break;
      case CCFE2eNCUParityCtc_Tx_Data_Chk_en:
        DEBUG ((DEBUG_INFO, "Injection          : Tx_Data_Chk_en\n"));
        Ctc_Ctl_Payload =         BIT18;
        Sts_Ctl_Payload =  BIT31 |BIT18;
        ExpectedMcaStatus = ((0x0B01ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]
        break;
      default:
        break;
    }

    // Injection
    InjectionAndTrafficNCU(IpIndex, Sts_Ctl_Payload, Ctc_Ctl_Payload);

    // Check MCA
    McaResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaStatus);
    DEBUG ((DEBUG_INFO, "MC%d_STATUS         : 0x%llx\n", McaNum, McaStatus));
    

    // Check Result
    AsmWriteMsr64 (MSR_PERRINJ_AT_IP, IpIndex     | BIT31);
    Ctc_Ctl_Payload =  AsmReadMsr64 (MSR_PERRINJ_CTRL);
    DEBUG ((DEBUG_INFO, "Ctc_Ctl_Payload    : 0x%llx\n", Ctc_Ctl_Payload));

    AsmWriteMsr64 (MSR_PERRINJ_AT_IP, (IpIndex + 1) | BIT31);
    Sts_Ctl_Payload =  AsmReadMsr64 (MSR_PERRINJ_CTRL);
    DEBUG ((DEBUG_INFO, "Sts_Ctl_Payload    : 0x%llx\n", Sts_Ctl_Payload));

    switch (CheckNum) {
      case CCFE2eNCUParityCtc_Rx_Data_Chk_en:
        CheckResult = ((McaResult == FUSA_TEST_PASS) && \
                      (Ctc_Ctl_Payload & BIT0) && \
                      (Sts_Ctl_Payload & BIT0)) ? \
                      FUSA_TEST_PASS : FUSA_TEST_FAIL;
        break;
      case CCFE2eNCUParityCtc_Tx_Data_Chk_en:
        CheckResult = ((McaResult == FUSA_TEST_PASS) && \
                      (Ctc_Ctl_Payload & BIT2) && \
                      (Sts_Ctl_Payload & BIT2)) ? \
                      FUSA_TEST_PASS : FUSA_TEST_FAIL;
        break;
      default:
        break;
    }

    // Update CheckResults
    pFusaTestResult[TestIndex].CheckResults[CheckNum] = (CheckResult == FUSA_TEST_PASS) ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
    DEBUG ((DEBUG_INFO, "CheckResult        : %a\n\n", (CheckResult == FUSA_TEST_PASS) ? "PASS" : "FAIL"));

    // Update TestResult
    if (CheckResult == FUSA_TEST_FAIL) {
      pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
    } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
      pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
    }

    // Cleanup
    Ctc_Ctl_Payload = 0x0 | BIT2 | BIT1 | BIT0;
    AsmWriteMsr64 (MSR_PERRINJ_AT_IP, IpIndex     | BIT31);
    AsmWriteMsr64 (MSR_PERRINJ_CTRL,  Ctc_Ctl_Payload);

    Sts_Ctl_Payload = 0x0 | BIT2 | BIT1 | BIT0;
    AsmWriteMsr64 (MSR_PERRINJ_AT_IP, (IpIndex + 1) | BIT31);
    AsmWriteMsr64 (MSR_PERRINJ_CTRL,  Sts_Ctl_Payload);

    Sts_Ctl_Payload = 0x0 | BIT24 | BIT23 | BIT22;
    AsmWriteMsr64 (MSR_PERRINJ_AT_IP, (IpIndex + 1) | BIT31);
    AsmWriteMsr64 (MSR_PERRINJ_CTRL,  Sts_Ctl_Payload);

    McaBankInfoClear (McaNum);
  }

  // Generate Crc32
  TestResultPtr = &pFusaTestResult[TestIndex];
  pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
    (UINT8 *) TestResultPtr,
    sizeof (FUSA_TEST_RESULT) - 4,
    0xffffffffU
  );

  DumpResults (TestResultPtr);

  McaReportingEnable (McaNum);
}

VOID
FspDxCheckCCFE2eNCUParityCtc (
  IN FUSA_TEST_RESULT *pFusaTestResult
  )
{
  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  FspDxCheckCCFE2eNCUParityCtcGeneric(pFusaTestResult);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

// IDIB cNCU Injection 
VOID
FspDxCheckIDIBE2eNCUParityCtcGeneric (
  IN FUSA_TEST_RESULT *pFusaTestResult
  )
{
  UINT8                 TestIndex;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT8                 CheckNum;
  UINT8                 CheckResult = 0;
  UINT8                 McaResult;
  FUSA_TEST_RESULT      *TestResultPtr;
  UINT8                 McaNum;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK | MCACOD_MASK;
  UINT64                McaStatus;
  UINT8                 IpIndex;
  UINT64                Ctc_Ctl_Payload;
  UINT64                Sts_Ctl_Payload;

  // Init Test Results
  /*
    CheckNum
    0 Rx_Data_Chk_en
    1 Tx_Data_Chk_en
  */
 TotalChecks = 2; // Rx and TX
 McaNum = MCA_IOP;
 IpIndex = 28;
 TestIndex = 0; 
 pFusaTestResult[TestIndex].TestNumber  = FusaTestNumIDIBE2ECNCUParityCtC0;
 pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
 pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
 for (i = 0; i < TotalChecks; i++ ) {
   pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
 }

 // Disable MCA Reporting
 AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL); 
 McaBankInfoClear (McaNum);

 for (CheckNum = 0; CheckNum < TotalChecks; CheckNum++) {
   DEBUG ((DEBUG_INFO, "CheckNum           : %d\n", CheckNum));

   switch (CheckNum) {
     case IDIBE2eNCUParityCtc_Rx_Data_Chk_en:
       DEBUG ((DEBUG_INFO, "Injection          : Rx_Data_Chk_en\n"));
       Ctc_Ctl_Payload =         BIT16;
       Sts_Ctl_Payload =  BIT31 |BIT16;
       ExpectedMcaStatus = ((0x0C01ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]
       break;
     case IDIBE2eNCUParityCtc_Tx_Data_Chk_en:
       DEBUG ((DEBUG_INFO, "Injection          : Tx_Data_Chk_en\n"));
       Ctc_Ctl_Payload =         BIT18;
       Sts_Ctl_Payload =  BIT31 |BIT18;
       ExpectedMcaStatus = ((0x0C01ULL) << 16 | 0x0E0BULL); // [MSCOD << 16 | MCACOD]
       break;
     default:
       break;
   }

   // Injection
   InjectionAndTrafficNCU(IpIndex, Sts_Ctl_Payload, Ctc_Ctl_Payload);

   // Check MCA
   McaResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaStatus);
   DEBUG ((DEBUG_INFO, "MC%d_STATUS         : 0x%llx\n", McaNum, McaStatus));
   
   // Check Result
   AsmWriteMsr64 (MSR_PERRINJ_AT_IP, IpIndex     | BIT31);
   Ctc_Ctl_Payload =  AsmReadMsr64 (MSR_PERRINJ_CTRL);
   DEBUG ((DEBUG_INFO, "Ctc_Ctl_Payload    : 0x%llx\n", Ctc_Ctl_Payload));

   AsmWriteMsr64 (MSR_PERRINJ_AT_IP, (IpIndex + 1) | BIT31);
   Sts_Ctl_Payload =  AsmReadMsr64 (MSR_PERRINJ_CTRL);
   DEBUG ((DEBUG_INFO, "Sts_Ctl_Payload    : 0x%llx\n", Sts_Ctl_Payload));

   switch (CheckNum) {
     case IDIBE2eNCUParityCtc_Rx_Data_Chk_en:
       CheckResult = ((McaResult == FUSA_TEST_PASS) && \
                     (Ctc_Ctl_Payload & BIT0) && \
                     (Sts_Ctl_Payload & BIT0)) ? \
                     FUSA_TEST_PASS : FUSA_TEST_FAIL;
       break;
     case IDIBE2eNCUParityCtc_Tx_Data_Chk_en:
       CheckResult = ((McaResult == FUSA_TEST_PASS) && \
                     (Ctc_Ctl_Payload & BIT2) && \
                     (Sts_Ctl_Payload & BIT2)) ? \
                     FUSA_TEST_PASS : FUSA_TEST_FAIL;
       break;
     default:
       break;
   }

   // Update CheckResults
   pFusaTestResult[TestIndex].CheckResults[CheckNum] = (CheckResult == FUSA_TEST_PASS) ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
   DEBUG ((DEBUG_INFO, "CheckResult        : %a\n\n", (CheckResult == FUSA_TEST_PASS) ? "PASS" : "FAIL"));

   // Update TestResult
   if (CheckResult == FUSA_TEST_FAIL) {
     pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
   } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
     pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
   }

   // Cleanup
   Ctc_Ctl_Payload = 0x0 | BIT2 | BIT1 | BIT0;
   AsmWriteMsr64 (MSR_PERRINJ_AT_IP, IpIndex     | BIT31);
   AsmWriteMsr64 (MSR_PERRINJ_CTRL,  Ctc_Ctl_Payload);

   Sts_Ctl_Payload = 0x0 | BIT2 | BIT1 | BIT0;
   AsmWriteMsr64 (MSR_PERRINJ_AT_IP, (IpIndex + 1) | BIT31);
   AsmWriteMsr64 (MSR_PERRINJ_CTRL,  Sts_Ctl_Payload);

   Sts_Ctl_Payload = 0x0 | BIT24 | BIT23 | BIT22;
   AsmWriteMsr64 (MSR_PERRINJ_AT_IP, (IpIndex + 1) | BIT31);
   AsmWriteMsr64 (MSR_PERRINJ_CTRL,  Sts_Ctl_Payload);

   McaBankInfoClear (McaNum);
 }

 // Generate Crc32
 TestResultPtr = &pFusaTestResult[TestIndex];
 pFusaTestResult[TestIndex].Crc32 = AsmCrc32Calc (
   (UINT8 *) TestResultPtr,
   sizeof (FUSA_TEST_RESULT) - 4,
   0xffffffffU
 );

 DumpResults (TestResultPtr);

 McaReportingEnable (McaNum);
}

VOID
FspDxCheckIDIBE2eNCUParityCtc (
  IN FUSA_TEST_RESULT *pFusaTestResult
  )
{
  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  FspDxCheckIDIBE2eNCUParityCtcGeneric(pFusaTestResult);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}
