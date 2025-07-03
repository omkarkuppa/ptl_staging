/** @file
  PEI FUSA E2E CTC Memory Subsystem

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
#include <MemoryConfig.h>
#include "PeiFusaE2eCtcLibInternal.h"
#include "PeiFusaE2eCtcMemSs.h"
#include "PeiFusaResultReportingLib.h"
#include "PeiFusaPrivateLibInternal.h"
#include <Ppi/SiPolicy.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <MemInfoHob.h>
#include <Register/Intel/ArchitecturalMsr.h>

STATIC UINTN             mMchBase;

/**
  Generate traffic

  @param[in] Controller - Controller to configure.
  @param[in] McCount    - Controller number to calculate address.
  @param[in] Opmode     - Operation Mode: 0 - Read, 1 - Write, 2 - Read then Write, 3 - Write then Read
  @param[in] Loop       - Loop to hit all channels

  @retval Nothing.
**/
void
MemoryGenerateTraffic (
  IN     UINT8                Controller,
  IN     UINT8                McCount,
  IN     MrcGenTrafficMode    OpMode,
  IN     UINT32               McInterleaveSize,
  IN     UINT32               ChInterleaveSize,
  IN     BOOLEAN              Loop
)
{
  UINT8                                  MemLoop;
  UINT32                                 MemoryData;
  EFI_PHYSICAL_ADDRESS                   MemoryAddress;
  EFI_PHYSICAL_ADDRESS                   BaseAddress;

  DEBUG ((DEBUG_INFO, "MemoryGenerateTraffic Entry\n"));
  // Use a random address to test, start at 256MB
  BaseAddress  = 0x10000000;
  MemoryData = 0;

  // Loop to hit targeted address due to address interleave
  for (MemLoop = 0; MemLoop < 8; MemLoop++) {
    MemoryAddress = BaseAddress + McCount * McInterleaveSize + MemLoop * ChInterleaveSize;
    if (OpMode == TrafficRead) {
      MemoryData = *(volatile UINT32*) (UINTN) MemoryAddress;
    } else if (OpMode == TrafficWrite) {
      *(volatile UINT32*) (UINTN) MemoryAddress = MemoryData;
    } else if (OpMode == TrafficWriteRead) {
      *(volatile UINT32*) (UINTN) MemoryAddress = MemoryData;
      MemoryData = *(volatile UINT32*) (UINTN) MemoryAddress;
    } else {
      MemoryData = *(volatile UINT32*) (UINTN) MemoryAddress;
      *(volatile UINT32*) (UINTN) MemoryAddress = MemoryData;
    }
    if (Loop == FALSE) {
      break;
    }
  }
  DEBUG ((DEBUG_INFO, "MemoryGenerateTraffic Complete\n"));
}

/**
  Read MCA Status from MSR

  @param[in] Opmode     - Operation Mode: 0 - Clear, 1 - Parity, 2 - Correctable, 3 - Uncorrectable


  @retval Nothing.
**/
BOOLEAN
MemoryGetSetMcaStatus (
  IN     MrcMcaMode    OpMode
)
{
  UINT64                                 McaStatus;
  UINT32                                 McacodStatus;
  UINT32                                 MscodStatus;
  UINT32                                 MscodCheck;
  BOOLEAN                                MsrResult;

  MscodCheck = (OpMode == McaUncorrectable) ? 0x0002 : ((OpMode == McaCorrectable) ? 0x0010 : 0x0001);

  MsrResult = FALSE;

  switch (OpMode) {
    case McaClear:
      AsmWriteMsr64 (MRC_MSR_MCA5_STATUS, 0);
      break;

    case McaParity:
    case McaUncorrectable:
    case McaCorrectable:
      McaStatus = AsmReadMsr64 (MRC_MSR_MCA5_STATUS);
      McacodStatus = (UINT32) (McaStatus & MRC_MSR_MCA5_STATUS_MCACOD_MASK);
      MscodStatus = ((UINT32) (McaStatus & MRC_MSR_MCA5_STATUS_MSCOD_MASK)) >> 16;
      MsrResult = (BOOLEAN) ((McacodStatus == 0x008F) && (MscodStatus == MscodCheck));
      DEBUG ((DEBUG_INFO, "MrcGetSetMcaStatus() McaStatus = %llx, McacodStatus = %x, MscodStatus = %x, MscodCheck = %x, Result = %x\n"
          , McaStatus, McacodStatus, MscodStatus, MscodCheck, MsrResult));
      break;

    default:
      DEBUG ((DEBUG_INFO, "Invalid mode.\n"));
      break;
  }
  return MsrResult;
}

/**
  Inject Parity and ECC Error

  @param[in] *pFusaTestResult - Pointer to the FUSA Test Result structure.

  @retval Always returns mrcSuccess.
**/
STATIC
VOID
MemoryErrorInjection (
  IN OUT FUSA_TEST_RESULT *pFusaTestResult
)
{
  EFI_STATUS                             Status;
  UINT32                                 InjOffset;
  UINT32                                 LogOffset;
  UINT8                                  Controller;
  UINT8                                  Channel;
  UINT8                                  TestCount;
  UINT8                                  SubTestCount;
  UINT8                                  TotalSubTest;
  UINT8                                  TestIndex;
  UINT8                                  SubTestIndex;
  BOOLEAN                                MsrResult;
  MC0_PARITY_ERR_INJ_STRUCT              McParityErrInj;
  MC0_PARITYERRLOG_STRUCT                McParityErrLog;
  //UINT32                                 MemoryAddress;
  //BOOLEAN                                TestDone;
  UINT8                                  McCount;
  UINT32                                 McInterleaveSize;
  UINT32                                 ChInterleaveSize;
  UINT8                                  ChannelDIMMPresent = FALSE;
  MEMORY_CONFIGURATION                   *MemConfig;
  UINT32                                 LsbMaskBit;
  SI_PREMEM_POLICY_PPI                   *SiPreMemPolicyPpi;
  FUSA_TEST_RESULT                       *TestResultPtr;
  EFI_HOB_GUID_TYPE                      *GuidHob;
  MEMORY_INFO_DATA_HOB                   *mMemInfo = NULL;
  UINT8 a;
  UINT8 b;

  static const UINT8  McTestList[] = {BeErrEn, DataErrEn, AddrErrEn, DataChErrEn};
  static const UINT8  TotalChecks[] = {11, 11, 8, 8, 8, 8, 8, 8, 8, 8};
  static const UINT8  McSubTestCount[] = {2, 8, 1, 8};

  MemConfig = NULL;
  SiPreMemPolicyPpi = NULL;
  McCount = 0;
  MsrResult = FALSE;
  // Use a random address to test, set ECC Address at 256MB
  //MemoryAddress = 0x10000000;

  TestIndex = 0;
  for (a = 0; a <= FusaTestNumMc1CmiCh3Data; a++ ) {
    pFusaTestResult[TestIndex].TestNumber  = TestIndex + FusaTestNumMc0Cmi;
    pFusaTestResult[TestIndex].TotalChecks = TotalChecks[a];
    pFusaTestResult[TestIndex].TestResult  = FUSA_TEST_DEVICE_NOTAVAILABLE;
    for (b = 0; b < pFusaTestResult[TestIndex].TotalChecks; b++ ) {
      pFusaTestResult[TestIndex].CheckResults[b] = FUSA_TEST_NOTRUN;
    }
    TestIndex++;
  }

  // Get MemConfig
  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigGuid, (VOID *) &MemConfig);
  ASSERT_EFI_ERROR (Status);

  // Get Interleave Bit
  McInterleaveSize = (BIT6 << ((MemConfig->ExternalInputs.MsHashEnable) ? MemConfig->ExternalInputs.MsHashInterleaveBit : 0));
  LsbMaskBit       = MemConfig->ExternalInputs.ChHashInterleaveBit;
  ChInterleaveSize = (BIT6 << LsbMaskBit);

  GuidHob = NULL;
  GuidHob = GetFirstGuidHob (&gSiMemoryInfoDataGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_INFO, "gSiMemoryInfoDataGuid not found!!!\n"));
    return;
  }
  mMemInfo = (MEMORY_INFO_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);

  for ( UINT8 i = 0; i < MAX_NODE; i++) {
    DEBUG ((DEBUG_INFO, "mMemInfo-> Controller[%d].Status = %x\n", i, mMemInfo-> Controller[i].Status));
    DEBUG ((DEBUG_INFO, "mMemInfo-> Controller[%d].ChannelCount = %x\n", i, mMemInfo-> Controller[i].ChannelCount));
    for ( UINT8 j = 0; j < MAX_CH; j++) {
      DEBUG ((DEBUG_INFO, "mMemInfo-> Controller[%d].ChannelInfo[%d].Status = %x\n", i, j, mMemInfo-> Controller[i].ChannelInfo[j].Status));
      DEBUG ((DEBUG_INFO, "mMemInfo-> Controller[%d].ChannelInfo[%d].ChannelId = %x\n", i, j, mMemInfo-> Controller[i].ChannelInfo[j].ChannelId));
      DEBUG ((DEBUG_INFO, "mMemInfo-> Controller[%d].ChannelInfo[%d].DimmCount = %x\n", i, j, mMemInfo-> Controller[i].ChannelInfo[j].DimmCount));
      for ( UINT8 k = 0; k < MAX_DIMM; k++) {
        DEBUG ((DEBUG_INFO, "mMemInfo-> Controller[%d].ChannelInfo[%d].DimmInfo[%d].Status = %x\n", i, j, k, mMemInfo-> Controller[i].ChannelInfo[j].DimmInfo[k].Status));
      }
    }
  }

  AsmWriteMsr64 (MSR_IA32_MC0_CTL + (MCA_IOP * 4), 0ULL);
  McaBankInfoClear (MCA_IOP);
  MemoryGetSetMcaStatus (McaClear);

  for (Controller = 0; Controller < MAX_NODE; Controller++) {
    if ((mMemInfo->Controller[Controller].Status == CHANNEL_PRESENT)) {
      // MC Test
      for (TestCount = 0; TestCount < ARRAY_COUNT (McTestList); TestCount++) {
        //TestDone = FALSE;
        TotalSubTest = McSubTestCount[TestCount];
        InjOffset = OFFSET_CALC_CH (MC0_PARITY_ERR_INJ_REG, MC1_PARITY_ERR_INJ_REG, Controller);
        LogOffset = OFFSET_CALC_CH (MC0_PARITYERRLOG_REG , MC1_PARITYERRLOG_REG , Controller);

        for (Channel = 0; Channel < MAX_CH; Channel++) {
          ChannelDIMMPresent = FALSE;
          for ( UINT8 i = 0; i < MAX_DIMM; i++) {
            if( mMemInfo-> Controller[Controller].ChannelInfo[Channel].DimmInfo[i].Status == DIMM_PRESENT) {
              DEBUG ((DEBUG_INFO, "Controller = %d Channel = %d DIMM = %d DIMM_PRESENT\n", Controller, Channel, i));
              ChannelDIMMPresent = TRUE;
              break;
            }
          }

          if( ChannelDIMMPresent == TRUE ) {
            // CMI test only required to run on one channel
            //TestDone = (McTestList[TestCount] != DataChErrEn) ? TRUE : FALSE;
            for (SubTestCount = 0; SubTestCount < TotalSubTest; SubTestCount++) {
              McParityErrInj.Data = 0;
              // Configure Error Injection Register
              switch (McTestList[TestCount]) {
                case AddrErrEn:
                  McParityErrInj.Bits.REQ_ADDR_ERR_EN = 1;
                  McParityErrInj.Bits.ERR_INJ_MASK = 0x10;
                  break;
                case BeErrEn:
                  McParityErrInj.Bits.RWD_HEADER_ERR_EN = BIT0 << SubTestCount;
                  McParityErrInj.Bits.ERR_INJ_MASK = 0x20;
                  break;
                case DataErrEn:
                  McParityErrInj.Bits.DATA_ERR_EN = BIT0 << (SubTestCount % 8);
                  McParityErrInj.Bits.ERR_INJ_MASK = 0x20;
                  break;
                case DataChErrEn:
                  McParityErrInj.Bits.DATA_ERR_EN = BIT0 << (SubTestCount % 8);
                  McParityErrInj.Bits.ERR_INJ_MASK = BIT0 << Channel;
                  break;
                default:
                  break;
              }
              MmioWrite64 ((mMchBase+InjOffset), McParityErrInj.Data);
              DEBUG ((DEBUG_INFO, "McParityErrInj.Data = %x\n", McParityErrInj.Data));
              DEBUG ((DEBUG_INFO, "McParityErrInj.Data = %x\n", MmioRead64 (mMchBase+InjOffset)));

              // Generate Traffic
              MemoryGenerateTraffic (Controller, McCount, TrafficWrite, McInterleaveSize, ChInterleaveSize, TRUE);
              AsmWbinvd ();

              // Read Msr Result and Store in Fusa test Result
              MsrResult = MemoryGetSetMcaStatus (McaParity);
              if (McTestList[TestCount] == DataChErrEn) {
                TestIndex = FusaTestNumMc0CmiCh0Data + (Controller * MAX_CH) + Channel;
                SubTestIndex = SubTestCount;
              } else {
                TestIndex = FusaTestNumMc0Cmi + Controller;
                if (McTestList[TestCount] == AddrErrEn) {
                  SubTestIndex = McAddrErr0;
                } else if (McTestList[TestCount] == BeErrEn) {
                  SubTestIndex = McBeErr0 + SubTestCount;
                } else {
                  SubTestIndex = McDataErr0 + SubTestCount;
                }
              }

              DEBUG ((DEBUG_INFO, "McParityErrLog.Data = %x\n", MmioRead64 (mMchBase+LogOffset)));

              pFusaTestResult[TestIndex].CheckResults[SubTestIndex] = MsrResult ? FUSA_TEST_PASS : FUSA_TEST_FAIL;
              if (!MsrResult) {
                pFusaTestResult[TestIndex].TestResult = FUSA_TEST_FAIL;
                DEBUG ((DEBUG_INFO, "pFusaTestResult[%x].TestResult = FUSA_TEST_FAIL\n\n", TestIndex));
              } else if (pFusaTestResult[TestIndex].TestResult != FUSA_TEST_FAIL) {
                // If there is already other subtest that failed, do not update it to pass.
                pFusaTestResult[TestIndex].TestResult = FUSA_TEST_PASS;
                DEBUG ((DEBUG_INFO, "pFusaTestResult[%x].TestResult = FUSA_TEST_PASS\n\n", TestIndex));
              }

              // Clear Error Status and Injection Register
              McParityErrLog.Data         = MmioRead64 (mMchBase+LogOffset);
              McParityErrLog.Bits.ERR_STS = 1;
              McParityErrLog.Bits.ERR_STS_OVERFLOW = 1;
              MmioWrite64 ((mMchBase+LogOffset), McParityErrLog.Data);
              McParityErrInj.Data         = 0;
              MmioWrite64 ((mMchBase+LogOffset), McParityErrInj.Data);
              MemoryGetSetMcaStatus (McaClear);
            }
          } else {
              DEBUG ((DEBUG_INFO, "Controller = %d Channel = %d DIMM_NOT_PRESENT\n", Controller, Channel));
          }
        }
      }
      McCount++;
    } else {
      DEBUG ((DEBUG_INFO, "Controller = %d CONTROLLER_NOT_PRESENT\n", Controller));
    }
  }

  McaReportingEnable (MCA_IOP);

  // Generate Crc32 and Print Results
  for (TestCount = 0; TestCount <= FusaTestNumMc1CmiCh3Data; TestCount++) {
    TestResultPtr = &pFusaTestResult[TestCount];
    pFusaTestResult[TestCount].Crc32 = AsmCrc32Calc (
      (UINT8 *) TestResultPtr,
      sizeof (FUSA_TEST_RESULT) - 4,
      0xffffffffU
    );
    DumpResults (TestResultPtr);
  }
}

/**
  Perform Memory Subsystem Parity Error E2E CTC.
  For IP not detected, their test result is updated to
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
FspDxCheckMemSubsystem (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  EFI_STATUS   Status;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  Status = MchBarAddressGet (&mMchBase);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_INFO, "Unable to get MchBar\n"));
  }
  
  MemoryErrorInjection(pFusaTestResult);
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
}
