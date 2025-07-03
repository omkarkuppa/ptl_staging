/** @file
  PEI FUSA Core CTC

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
#include <Register/Cpuid.h>
#include <Library/LocalApicLib.h>
#include "PeiFusaE2eCtcLibInternal.h"
#include "PeiFusaCoreCtc.h"
#include <Register/Ptl/Msr/MsrRegs.h>
#include <Register/Intel/ArchitecturalMsr.h>

STATIC UINT32 mBspApicId = 0;

/**
 * Perform Core Cache Array Start-up BIST Generic Internal Function
 **/
VOID
FspDxCheckCoreCtcStartupBistGenericInternal (
  IN CORE_CTC_TEST_PARAM *CoreCtcTestParam
  )
{
  MSR_CORE_BIST_REGISTER  MsrCoreBistReg;

  // Trigger Bist
  AsmWriteMsr64 (MSR_CORE_BIST, CoreCtcTestParam->MsrCoreBistReg.Uint64);

  // Delay
  MicroSecondDelay (1000000);

  // Get results
  MsrCoreBistReg.Uint64 = AsmReadMsr64 (MSR_CORE_BIST);
  CoreCtcTestParam->MsrCoreBistReg.Uint64 = MsrCoreBistReg.Uint64;
  return;
}

/**
  Perform Core Cache Array Start-up BIST Generic Function
**/
VOID
FspDxCheckCoreCtcStartupBistGeneric (
  IN FUSA_TEST_RESULT *pFusaTestResult,
  IN UINT8            CoreIndex,
  IN UINT8            Test,
  IN BOOLEAN          Bsp
  )
{
  UINT8                   CheckResult;
  MSR_CORE_BIST_REGISTER  MsrCoreBistReg;
  CORE_CTC_TEST_PARAM     CoreCtcTestParam;

  // Trigger BIST
  if (Test == CacheArray) {
    MsrCoreBistReg.Bits.BistArray = 0x05FF;
  } else {
    MsrCoreBistReg.Bits.BistArray = 0x0200;
  }
  MsrCoreBistReg.Bits.CtrlResult = 0x1;

  DEBUG ((DEBUG_INFO, "\nCoreIndex               : %d\n", CoreIndex));
  DEBUG ((DEBUG_INFO, "MSR 0x%x Write: 0x%llx\n", MSR_CORE_BIST, (UINTN) MsrCoreBistReg.Uint64));
  
  CoreCtcTestParam.CoreNumber            = CoreIndex;
  CoreCtcTestParam.pFusaTestResult       = pFusaTestResult;
  CoreCtcTestParam.MsrCoreBistReg.Uint64 = MsrCoreBistReg.Uint64;

  if (Bsp) {
    FspDxCheckCoreCtcStartupBistGenericInternal(&CoreCtcTestParam);
  } else {
    RunAtAp (
      (AP_PROCEDURE) FspDxCheckCoreCtcStartupBistGenericInternal,
        CoreIndex,
        &CoreCtcTestParam
      );
  }
  DEBUG ((DEBUG_INFO, "MSR 0x%x Read : 0x%llx\n", MSR_CORE_BIST, CoreCtcTestParam.MsrCoreBistReg.Uint64));
  CheckResult = ((CoreCtcTestParam.MsrCoreBistReg.Uint64 & BIT63) == 0x0ULL) ? FUSA_TEST_PASS : FUSA_TEST_FAIL;

  // Update CheckResults
  DEBUG ((DEBUG_INFO, "CheckResult       : %a\n\n", CheckResult ? "PASS" : "FAIL"));
  pFusaTestResult[CoreIndex].CheckResults[0] = CheckResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;

  // Update TestResult
  if (CheckResult == FUSA_TEST_FAIL) {
    pFusaTestResult[CoreIndex].TestResult = FUSA_TEST_FAIL;
  } else if (pFusaTestResult[CoreIndex].TestResult != FUSA_TEST_FAIL) {
    pFusaTestResult[CoreIndex].TestResult = FUSA_TEST_PASS;
  }
}

#if 0

/**
  Perform Core Cache Array Start-up BIST
**/
VOID
FspDxCheckCoreCacheArrayStartupBist (
  IN FUSA_TEST_RESULT *pFusaTestResult
  )
{
  UINT8                   TestIndex;
  UINT8                   TotalChecks;
  UINT8                   CoreIndex;
  UINT8                   i;
  FUSA_INFO               *Local_Fusa_info;
  FUSA_TEST_RESULT        *TestResultPtr;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  Local_Fusa_info = FusaInfoListGet();

  mBspApicId = GetInitialApicId ();
  DEBUG ((DEBUG_INFO, "\nmBspApicId         = %x\n", mBspApicId));
  DEBUG ((DEBUG_INFO, "TotalCoreProcessor = %x\n\n", Local_Fusa_info->TotalCoreProcessor));

  for (CoreIndex = 0; CoreIndex < MAX_BIG_CORE_COUNT; CoreIndex++) {
    // Init TestResult
    TotalChecks = 1;

    switch (CoreIndex) {
      case 0:
        TestIndex = FusaTestNumCoreCacheArrayStartupBistCore0 - FusaTestNumCoreCacheArrayStartupBistCore0;
        pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCoreCacheArrayStartupBistCore0;
        break;
      case 1:
        TestIndex = FusaTestNumCoreCacheArrayStartupBistCore1 - FusaTestNumCoreCacheArrayStartupBistCore0;
        pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCoreCacheArrayStartupBistCore1;
        break;
      case 2:
        TestIndex = FusaTestNumCoreCacheArrayStartupBistCore2 - FusaTestNumCoreCacheArrayStartupBistCore0;
        pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCoreCacheArrayStartupBistCore2;
        break;
      case 3:
        TestIndex = FusaTestNumCoreCacheArrayStartupBistCore3 - FusaTestNumCoreCacheArrayStartupBistCore0;
        pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCoreCacheArrayStartupBistCore3;
        break;
      default:
        break;
    }
    pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
    pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
    for (i = 0; i < TotalChecks; i++ ) {
      pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
    }

    if( Local_Fusa_info->Processor[CoreIndex].CoreType == CPUID_CORE_TYPE_INTEL_CORE
      && Local_Fusa_info->Processor[CoreIndex].ProcessorInfo.Location.Thread == 0)
    {
      if (Local_Fusa_info->Processor[CoreIndex].ProcessorInfo.ProcessorId == mBspApicId) {
      FspDxCheckCoreCtcStartupBistGeneric(pFusaTestResult, CoreIndex, CacheArray, TRUE);
      } else {
      FspDxCheckCoreCtcStartupBistGeneric(pFusaTestResult, CoreIndex, CacheArray, FALSE);
      }
    }

    // Generate Crc32
    TestResultPtr = &pFusaTestResult[CoreIndex];
    pFusaTestResult[CoreIndex].Crc32 = AsmCrc32Calc (
      (UINT8 *) TestResultPtr,
      sizeof (FUSA_TEST_RESULT) - 4,
      0xffffffffU
    );

    DumpResults (TestResultPtr);
  }

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}
#endif


/**
  Perform Core ROM CRC Start-up BIST
**/
VOID
FspDxCheckCoreRomCrcStartupBist (
  IN FUSA_TEST_RESULT *pFusaTestResult
  )
{
  UINT8                   TestIndex;
  UINT8                   TotalChecks;
  UINT8                   CoreIndex;
  UINT8                   i;
  FUSA_INFO               *Local_Fusa_info;
  FUSA_TEST_RESULT        *TestResultPtr;

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  Local_Fusa_info = FusaInfoListGet();

  mBspApicId = GetInitialApicId ();
  DEBUG ((DEBUG_INFO, "\nmBspApicId         = %x\n", mBspApicId));
  DEBUG ((DEBUG_INFO, "TotalCoreProcessor = %x\n\n", Local_Fusa_info->TotalCoreProcessor));

  for (CoreIndex = 0; CoreIndex < MAX_BIG_CORE_COUNT; CoreIndex++) {
    // Init TestResult
    TotalChecks = 1;

    switch (CoreIndex) {
      case 0:
        TestIndex = FusaTestNumCoreRomCrcStartupBistCore0 - FusaTestNumCoreRomCrcStartupBistCore0;
        pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCoreRomCrcStartupBistCore0;
        break;
      case 1:
        TestIndex = FusaTestNumCoreRomCrcStartupBistCore1 - FusaTestNumCoreRomCrcStartupBistCore0;
        pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCoreRomCrcStartupBistCore1;
        break;
      case 2:
        TestIndex = FusaTestNumCoreRomCrcStartupBistCore2 - FusaTestNumCoreRomCrcStartupBistCore0;
        pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCoreRomCrcStartupBistCore2;
        break;
      case 3:
        TestIndex = FusaTestNumCoreRomCrcStartupBistCore3 - FusaTestNumCoreRomCrcStartupBistCore0;
        pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCoreRomCrcStartupBistCore3;
        break;
      default:
        break;
    }
    pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
    pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
    for (i = 0; i < TotalChecks; i++ ) {
      pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
    }

    if( Local_Fusa_info->Processor[CoreIndex].CoreType == CPUID_CORE_TYPE_INTEL_CORE
      && Local_Fusa_info->Processor[CoreIndex].ProcessorInfo.Location.Thread == 0)
    {
      if (Local_Fusa_info->Processor[CoreIndex].ProcessorInfo.ProcessorId == mBspApicId) {
        FspDxCheckCoreCtcStartupBistGeneric(pFusaTestResult, CoreIndex, RomCrc, TRUE);
      } else {
        FspDxCheckCoreCtcStartupBistGeneric(pFusaTestResult, CoreIndex, RomCrc, FALSE);
      }
    }

    // Generate Crc32
    TestResultPtr = &pFusaTestResult[CoreIndex];
    pFusaTestResult[CoreIndex].Crc32 = AsmCrc32Calc (
      (UINT8 *) TestResultPtr,
      sizeof (FUSA_TEST_RESULT) - 4,
      0xffffffffU
    );

    DumpResults (TestResultPtr);
  }

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}

/**
 * Perform Core Idi Parity Ctc Generic Internal Function
 **/
VOID
FspDxCheckCoreIdiParityCtcGenericInternal (
  IN CORE_IDI_CTC_TEST_PARAM *CoreIdiCtcTestParam
  )
{
  UINT8                     IpIndex;
  UINT32                    MemoryData = 0xF0F0F0F0;
  EFI_PHYSICAL_ADDRESS      MemoryAddress;

  IpIndex = 1;
  MemoryAddress = 0;

  // Clear MCA
  McaBankInfoClear(MCA_MLC);

  // Trigger test
  AsmWriteMsr64 (MSR_PERRINJ_AT_IP, BIT31 | IpIndex);
  MicroSecondDelay (10);
  AsmWriteMsr64 (MSR_PERRINJ_CTRL, CoreIdiCtcTestParam->Payload);
  *(volatile UINT32*) (UINTN) MemoryAddress = MemoryData;
  AsmWbinvd();
  MicroSecondDelay (10);

  // Generate Read Traffico
  MemoryData = *(volatile UINT32*) (UINTN) MemoryAddress;
  MicroSecondDelay (10);

  // Check for MCA
  CoreIdiCtcTestParam->Msr64 = AsmReadMsr64 (0x401 + (MCA_MLC * 4));

  return;
}

/**
  Perform Core IDI Parity Ctc Generic Function
**/
VOID
FspDxCheckCoreIdiParityCtcGeneric (
  IN FUSA_TEST_RESULT *pFusaTestResult,
  IN UINT8            CoreIndex,
  IN BOOLEAN          Bsp
  )
{
  UINT8                   CheckNum;
  UINT8                   CheckResult;
  CORE_IDI_CTC_TEST_PARAM CoreIdiCtcTestParam;
  UINT64                  ExpectedMcaStatus;

  DEBUG ((DEBUG_INFO, "\nCoreIndex               : %d\n", CoreIndex));

  for (CheckNum = 0; CheckNum < pFusaTestResult[CoreIndex].TotalChecks; CheckNum++) {
#if 1 // set to 0 to run test
    if (CheckNum == Slice1Cone0PerrData) {
      DEBUG ((DEBUG_INFO, "Skipped %a\n", "Slice1Cone0PerrData"));
      continue;
    }
#endif
    CoreIdiCtcTestParam.Msr64 = 0;
    switch (CheckNum) {
      case PerrAddr:            // PERR_ADDR = 0x1
        CoreIdiCtcTestParam.Payload = BIT10;
        ExpectedMcaStatus = ((0x00C0ULL) << 16 | 0x0405ULL); // [MSCOD << 16 | MCACOD]
        break;
      case Slice0Cone0PerrData: // Slice 0 // Cone 0 // PERR_DATA = 0xFF
        CoreIdiCtcTestParam.Payload = 0xFF;
        ExpectedMcaStatus = ((0x0040ULL) << 16 | 0x0405ULL); // [MSCOD << 16 | MCACOD]
        break;
      case Slice0Cone1PerrData: // Slice 0 // Cone 1 // PERR_DATA = 0xFF
        CoreIdiCtcTestParam.Payload = BIT8 | 0xFF;
        ExpectedMcaStatus = ((0x0040ULL) << 16 | 0x0405ULL); // [MSCOD << 16 | MCACOD]
        break;
      case Slice1Cone0PerrData: // Slice 1 // Cone 0 // PERR_DATA = 0xFF
        CoreIdiCtcTestParam.Payload = BIT9 | 0xFF;
        ExpectedMcaStatus = ((0x0040ULL) << 16 | 0x0405ULL); // [MSCOD << 16 | MCACOD]
        break;
      case Slice1Cone1PerrData: // Slice 1 // Cone 1 // PERR_DATA = 0xFF
        CoreIdiCtcTestParam.Payload = BIT9 | BIT8 | 0xFF;
        ExpectedMcaStatus = ((0x0040ULL) << 16 | 0x0405ULL); // [MSCOD << 16 | MCACOD]
        break;
      default:
        break;
    }

    if (Bsp) {
      FspDxCheckCoreIdiParityCtcGenericInternal (&CoreIdiCtcTestParam);
    } else {
      RunAtAp (
        (AP_PROCEDURE) FspDxCheckCoreIdiParityCtcGenericInternal,
          CoreIndex,
          &CoreIdiCtcTestParam
        );
    }

    // Dump MCA
    DEBUG ((DEBUG_INFO, "MSR 0x%x 0x%llx\n", (0x401 + (MCA_MLC * 4)), CoreIdiCtcTestParam.Msr64));

    // Check Result
    CheckResult = ((CoreIdiCtcTestParam.Msr64 & 0xffffffffULL) == ExpectedMcaStatus) ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
    DEBUG ((DEBUG_INFO, "CheckResult %a\n\n", (CheckResult == FUSA_TEST_PASS) ? "PASS" : "FAIL"));

    // Update Result
    pFusaTestResult[CoreIndex].CheckResults[CheckNum] = CheckResult;

    // Update TestResult
    if (CheckResult == FUSA_TEST_FAIL) {
      pFusaTestResult[CoreIndex].TestResult = FUSA_TEST_FAIL;
    } else if (pFusaTestResult[CoreIndex].TestResult != FUSA_TEST_FAIL) {
      pFusaTestResult[CoreIndex].TestResult = FUSA_TEST_PASS;
    }
  }

  // Clear MCA
  McaBankInfoClear(MCA_MLC);
}

/**
  Perform Core Idi Parity Ctc
**/
VOID
FspDxCheckCoreIdiParityCtc (
  IN FUSA_TEST_RESULT *pFusaTestResult
  )
{
  UINT8                   TestIndex;
  UINT8                   TotalChecks;
  UINT8                   CoreIndex;
  UINT8                   i;
  FUSA_INFO               *Local_Fusa_info;
  FUSA_TEST_RESULT        *TestResultPtr;
  UINT8                   McaNum;
  static const UINT8      CoreIdiParityCtcTestList[] = {PerrAddr,
                                                        Slice0Cone0PerrData,
                                                        Slice0Cone1PerrData,
                                                        Slice1Cone0PerrData,
                                                        Slice1Cone1PerrData
                                                      };

  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  Local_Fusa_info = FusaInfoListGet();

  mBspApicId = GetInitialApicId ();
  DEBUG ((DEBUG_INFO, "\nmBspApicId         = %x\n", mBspApicId));
  DEBUG ((DEBUG_INFO, "TotalCoreProcessor = %x\n\n", Local_Fusa_info->TotalCoreProcessor));

  McaNum = MCA_MLC;

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  for (CoreIndex = 0; CoreIndex < MAX_BIG_CORE_COUNT; CoreIndex++) {
    // Init TestResult
    TotalChecks = sizeof(CoreIdiParityCtcTestList);

    switch (CoreIndex) {
      case 0:
        TestIndex = FusaTestNumCoreIdiParityCtcCore0 - FusaTestNumCoreIdiParityCtcCore0;
        pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCoreIdiParityCtcCore0;
        break;
      case 1:
        TestIndex = FusaTestNumCoreIdiParityCtcCore1 - FusaTestNumCoreIdiParityCtcCore0;
        pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCoreIdiParityCtcCore1;
        break;
      case 2:
        TestIndex = FusaTestNumCoreIdiParityCtcCore2 - FusaTestNumCoreIdiParityCtcCore0;
        pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCoreIdiParityCtcCore2;
        break;
      case 3:
        TestIndex = FusaTestNumCoreIdiParityCtcCore3 - FusaTestNumCoreIdiParityCtcCore0;
        pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCoreIdiParityCtcCore3;
        break;
      default:
        break;
    }
    pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
    pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
    for (i = 0; i < TotalChecks; i++ ) {
      pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
    }

    if( Local_Fusa_info->Processor[CoreIndex].CoreType == CPUID_CORE_TYPE_INTEL_CORE
      && Local_Fusa_info->Processor[CoreIndex].ProcessorInfo.Location.Thread == 0)
    {
      if (Local_Fusa_info->Processor[CoreIndex].ProcessorInfo.ProcessorId == mBspApicId) {
        FspDxCheckCoreIdiParityCtcGeneric(pFusaTestResult, CoreIndex, TRUE);
      } else {
        FspDxCheckCoreIdiParityCtcGeneric(pFusaTestResult, CoreIndex, FALSE);
      }
    }

    // Generate Crc32
    TestResultPtr = &pFusaTestResult[CoreIndex];
    pFusaTestResult[CoreIndex].Crc32 = AsmCrc32Calc (
      (UINT8 *) TestResultPtr,
      sizeof (FUSA_TEST_RESULT) - 4,
      0xffffffffU
    );

    DumpResults (TestResultPtr);
  }

  McaReportingEnable (McaNum);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}