/** @file
  PEI FUSA E2E CTC LLC Double Parity

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
#include "PeiFusaE2eCtcLlcDoubleParity.h"
#include "PeiFusaE2eCtcLibInternal.h"

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
  IN UINT32 InjectionPayload
  )
{
  UINT32                                 MemoryData;
  EFI_PHYSICAL_ADDRESS                   MemoryAddress;

  DEBUG ((DEBUG_INFO, "Injection          : IpIndex 0x%llx Payload 0x%llx\n", IpIndex, InjectionPayload));

  // Injection
  AsmWriteMsr64(MSR_PERRINJ_AT_IP, IpIndex | BIT31);
  MicroSecondDelay (10);
  AsmWriteMsr64(MSR_PERRINJ_CTRL,  InjectionPayload);

  // Traffic
  MemoryAddress = 0x2000000;
  MemoryData = 0xF0F0F0F0;
  *(volatile UINT32*) (UINTN) MemoryAddress = MemoryData;
  AsmWbinvd();
  MemoryData = *(volatile UINT32*) (UINTN) MemoryAddress;

  return 0;
}


/**
  Perform E2E CTC LLC Double Parity.
**/
VOID
FspDxCheckCboSliceEgressLlcDoubleParityCtCGeneric (
  IN FUSA_TEST_RESULT *pFusaTestResult,
  IN UINT8            Slice
  )
{
  UINT8                 TestIndex;
  UINT8                 TotalChecks;
  UINT8                 i;
  UINT8                 CheckNum;
  UINT8                 CheckResult = 0;
  FUSA_TEST_RESULT      *TestResultPtr;
  UINT8                 McaNum;

  UINT64                ExpectedMcaStatus;
  UINT64                McaStatusMask = MSCOD_MASK;
  UINT64                McaResult;
  UINT8                 IpIndex;
  FUSA_INFO             *Local_Fusa_info;
  BOOLEAN               Module4Available = FALSE;
  BOOLEAN               Module5Available = FALSE;

  DEBUG ((DEBUG_INFO, "\nSlice              : %d\n", Slice));

  ExpectedMcaStatus = ((0x0002ULL) << 16); // [MSCOD << 16]
  // Init Test Results
  /*
    [BIT3]= h1_state_map_even_inject_err
    [BIT2]= h1_state_map_odd_inject_err
    [BIT1]= h0_state_map_even_inject_err
    [BIT0]= h0_state_map_odd_inject_err
  */
  TotalChecks = 4;

  switch (Slice) {
    case 0:
      TestIndex = FusaTestNumCboSlice0EgressLlcDoubleParityCtC - FusaTestNumCboSlice0EgressLlcDoubleParityCtC;
      pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCboSlice0EgressLlcDoubleParityCtC;
      McaNum = MCA_CBO0;
      break;
    case 1:
      TestIndex = FusaTestNumCboSlice1EgressLlcDoubleParityCtC - FusaTestNumCboSlice0EgressLlcDoubleParityCtC;
      pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCboSlice1EgressLlcDoubleParityCtC;
      McaNum = MCA_CBO1;
      break;
    case 2:
      TestIndex = FusaTestNumCboSlice2EgressLlcDoubleParityCtC - FusaTestNumCboSlice0EgressLlcDoubleParityCtC;
      pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCboSlice2EgressLlcDoubleParityCtC;
      McaNum = MCA_CBO2;
      break;
    case 3:
      TestIndex = FusaTestNumCboSlice3EgressLlcDoubleParityCtC - FusaTestNumCboSlice0EgressLlcDoubleParityCtC;
      pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCboSlice3EgressLlcDoubleParityCtC;
      McaNum = MCA_CBO3;
      break;
    case 4:
      TestIndex = FusaTestNumCboSlice4EgressLlcDoubleParityCtC - FusaTestNumCboSlice0EgressLlcDoubleParityCtC;
      pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCboSlice4EgressLlcDoubleParityCtC;
      McaNum = MCA_CBO4;
      break;
    case 5:
      TestIndex = FusaTestNumCboSlice5EgressLlcDoubleParityCtC - FusaTestNumCboSlice0EgressLlcDoubleParityCtC;
      pFusaTestResult[TestIndex].TestNumber  = FusaTestNumCboSlice5EgressLlcDoubleParityCtC;
      McaNum = MCA_CBO5;
      break;
    default:
      break;
  }

  // Disable MCA Reporting
  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (McaNum * 4), 0ULL);
  McaBankInfoClear (McaNum);

  IpIndex = 5 + (4 * TestIndex);
  pFusaTestResult[TestIndex].TotalChecks = TotalChecks;
  pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
  for (i = 0; i < TotalChecks; i++ ) {
    pFusaTestResult[TestIndex].CheckResults[i] = FUSA_TEST_NOTRUN;
  }
  Local_Fusa_info = FusaInfoListGet();
  for (i = 0; i < Local_Fusa_info->TotalProcessor; i++) {
    if (Module4Available) {
      break;
    }
    Module4Available = (Local_Fusa_info->Processor[i].ProcessorInfo.ExtendedInformation.Location2.Module == 4 ) ? TRUE : FALSE;
  }
  for (i = 0; i < Local_Fusa_info->TotalProcessor; i++) {
    if (Module5Available) {
      break;
    }
    Module5Available = (Local_Fusa_info->Processor[i].ProcessorInfo.ExtendedInformation.Location2.Module == 5 ) ? TRUE : FALSE;
  }

  if (( Slice == 0) ||
      ( Slice == 1) ||
      ( Slice == 2) ||
      ( Slice == 3) ||
      ((Slice == 4) && (Module4Available)) ||
      ((Slice == 5) && (Module5Available)))
  {
    for (CheckNum = 0; CheckNum < TotalChecks; CheckNum++) {
      DEBUG ((DEBUG_INFO, "CheckNum           : %d\n", CheckNum));

      // Injection
      InjectionAndTraffic(IpIndex, BIT0 << CheckNum);

      // Check MCA
      CheckResult = McaBankStatusCheck(McaNum, McaStatusMask, ExpectedMcaStatus, &McaResult);
      DEBUG ((DEBUG_INFO, "MC%d_STATUS        : 0x%llx\n", McaNum, McaResult));

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
      McaBankInfoClear (McaNum);
    }
  } else {
    DEBUG ((DEBUG_INFO, "Skipped Slice %d since Atom Module ID %d not available\n", Slice, Slice));
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
  Perform E2E CTC LLC Double Parity.
**/
VOID
FspDxCheckCboSliceEgressLlcDoubleParityCtC (
  IN FUSA_TEST_RESULT *pFusaTestResult
  )
{
  DEBUG ((DEBUG_INFO, "\n%a [Start]\n", __FUNCTION__));

  FspDxCheckCboSliceEgressLlcDoubleParityCtCGeneric(pFusaTestResult, 0);
  FspDxCheckCboSliceEgressLlcDoubleParityCtCGeneric(pFusaTestResult, 1);
  FspDxCheckCboSliceEgressLlcDoubleParityCtCGeneric(pFusaTestResult, 2);
  FspDxCheckCboSliceEgressLlcDoubleParityCtCGeneric(pFusaTestResult, 3);
  FspDxCheckCboSliceEgressLlcDoubleParityCtCGeneric(pFusaTestResult, 4);
  FspDxCheckCboSliceEgressLlcDoubleParityCtCGeneric(pFusaTestResult, 5);

  DEBUG ((DEBUG_INFO, "%a [End]\n\n", __FUNCTION__));
}