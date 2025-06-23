/** @file
  This file provides Fusa GRT Mem Bist Test implementation

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
#include <Library/BaseMemoryLib.h>
#include <Library/LocalApicLib.h>
#include "PeiFusaE2eCtcLibInternal.h"
#include "PeiFusaPrivateLibInternal.h"
#include "PeiFusaResultReportingLib.h"
#include <Register/Cpuid.h>
#include <Register/Ptl/Msr/MsrRegs.h>
#include <Library/PeiFusaLib.h>
#include <Library/MemoryAllocationLib.h>

/// Structure of the parameter passed into the Core Idi test routine
typedef struct
{
    FUSA_TEST_RESULT* pFusaTestResult;
    UINT8            FusaPatternFileNumber;
    UINT32           CoreNumber;
} CORE_IDI_TEST_PARAM;

/**
  Retrieve test result in each of the core

  @param[in,out] pCoreIdiTestParam - conttain result buffer
       where the test result to be updated to beside core number
       information
**/
VOID
FspDxCheckGrScanBistReadInternal (
  IN CORE_IDI_TEST_PARAM *pCoreIdiTestParam
  )
{
  FUSA_TEST_RESULT    *pFusaTestResult;
  UINT64              FusaConfigMsr;

  pFusaTestResult = pCoreIdiTestParam->pFusaTestResult;

  FusaConfigMsr = AsmReadMsr64 (MSR_STARTUP_SCAN_BIST_STATUS);
  if ((FusaConfigMsr & BIT0) == 0x0) {
    UpdateResults(FUSA_TEST_PASS, pCoreIdiTestParam->FusaPatternFileNumber, pFusaTestResult);
  } else {
    UpdateResults(FUSA_TEST_FAIL, pCoreIdiTestParam->FusaPatternFileNumber, pFusaTestResult);
  }

}

/**
  Startup Scan that run on each AP

  @param[in] mBuffAddr - buffer address to scan pattern file
**/
VOID
EFIAPI
FspDxCheckGrScanBistSetInternal (
  TEST_CONFIG_SCANBIST  *mTestConfig
  )
{
  CPUID_NATIVE_MODEL_ID_AND_CORE_TYPE_EAX Eax;
  TEST_CONFIG_SCANBIST *TestConfig;
  UINT64 fusa_capability;

  TestConfig = mTestConfig;
  fusa_capability = AsmReadMsr64 (MSR_FUSA_CAPABILITIES_A);

  AsmCpuid (CPUID_HYBRID_INFORMATION, &Eax.Uint32, NULL, NULL, NULL);  
  if(((UINT8)Eax.Bits.CoreType == CPUID_CORE_TYPE_INTEL_ATOM)
    && (fusa_capability & B_MSR_FUSA_CAP_STARTUP_SCAN_DIAGNOGSIS_MASK))
  {
//    _asm
//    {
//      wbinvd
//    }
    AsmWbinvd();
    AsmWriteMsr64 (MSR_TRIGGER_STARTUP_SCAN_BIST, (UINT64)TestConfig->buffAddr);
  }

  return;
}

/**
  Perform GRT Atom PBist.
  The test targets all active unique core. For core not
  detected, their test result is updated to
  FUSA_TEST_DEVICE_NOTAVAILABLE

  @note The test expects to be run within  Memory Type =
        Writeback (MTRR Encoding = 06h), which is the usual
        case of the memory type covering the code/data region
        of the bootloader.
  @param[in] FusaStartupFileAddr - Addr point to Startup Pattern file
  @param[out] pFusaTestResult - pointer to test result
                         buffer for test from FusaTestNumCpu0Idi
                         to FusaTestNumCpu7Idi

**/


#if 0
FUSA_PATTERN_FILE_HEADER **
GetTotalNumberOfPatternFiles(
    IN   UINT32     FusaStartupFileAddr,
    OUT  UINT8      *NumberOfPatternFiles
)
{
    UINT32                     *FusaPatternFile;
    UINT8                      TotalNumberofPatternFiles = 0;
    FUSA_PATTERN_FILE_HEADER   **StartFuSaHeader;
    UINT32                     Count = 0 ;
    FUSA_PATTERN_FILE_HEADER   *FuSaHeader;
    FUSA_PATTERN_FILE_HEADER  *FuSaPatternArray[FUSA_PATTERN_MAX_FILE];
    UINT8                      IsError = FALSE;
    UINT32                     *SignaturePtr;

    FusaPatternFile = (UINT32*)FusaStartupFileAddr;
    Count = 0;
    do {
        if (NULL == FusaPatternFile)
        {
            DEBUG((DEBUG_INFO, "GetTotalNumberOfPatternFiles: FusaStartupFileAddr is invalid \n"));
            IsError = TRUE;
            break;
        }
        while (Count < 0x1000)
        {
            if (CompareGuid((EFI_GUID*)((UINT32*)(FusaPatternFile)+Count), &gFusaStartupSectionGuid)) {
                DEBUG((DEBUG_INFO, "GetTotalNumberOfPatternFiles: FOUND STARTUP Guid.\n"));
                SignaturePtr = (UINT32*)((UINTN)(FusaPatternFile)+Count * 4 + sizeof(EFI_GUID) +\
                    NUMBER_OF_UNKNOWN_CHARACTER_AFTER_GUID );
                DEBUG((DEBUG_INFO, "SignaturePtr = %x %x\n", *SignaturePtr, *(SignaturePtr + 1)));
                break;
            }
            Count++;
        }
        if (Count >= 0x1000)
        {
            DEBUG((DEBUG_INFO, "GetTotalNumberOfPatternFiles: Failed to find Pattern File with GUID.\n"));
            IsError = TRUE;
            break;
        }
        if (NULL == SignaturePtr)
        {
            DEBUG((DEBUG_INFO, "GetTotalNumberOfPatternFiles: Failed to find Pattern File with GUID.\n"));
            IsError = TRUE;
            break;
        }

        do {

            if (TotalNumberofPatternFiles >= FUSA_PATTERN_MAX_FILE)
            {
                DEBUG((DEBUG_INFO, "TotalNumberofPatternFiles found 0x%x is more than max permitted files 0x%x\n", TotalNumberofPatternFiles, FUSA_PATTERN_MAX_FILE));
                break;
            }
            if (0 != CompareMem(SignaturePtr, FUSA_PATTERN_SIGNATURE_START, AsciiStrLen(FUSA_PATTERN_SIGNATURE_START)))
            {
                DEBUG((DEBUG_INFO, "GetTotalNumberOfPatternFiles: %x Fusa Pattern file do not have a valid signatire\n", TotalNumberofPatternFiles));
                IsError = TRUE;
                break;
            }
            FuSaHeader = (FUSA_PATTERN_FILE_HEADER*)((UINTN)SignaturePtr + AsciiStrLen(FUSA_PATTERN_SIGNATURE_START) + NUMBER_OF_UNKNOWN_CHARACTER_AFTER_START_SIG);
            FuSaPatternArray[TotalNumberofPatternFiles] = FuSaHeader;
            DEBUG((DEBUG_INFO, "FspDxCheckGrtScanBist: Pattern Files Number  =%d\n", TotalNumberofPatternFiles));

            SignaturePtr = (UINT32*)((UINTN)FuSaHeader + FuSaHeader->TotalSize );
            if (0 == CompareMem(SignaturePtr, FUSA_PATTERN_SIGNATURE_END, AsciiStrLen(FUSA_PATTERN_SIGNATURE_END)))
            {
                DEBUG((DEBUG_INFO, "FspDxCheckGrtScanBist: The First Fusa Pattern file Do no have the valid signature.\n"));
                break;
            }
            TotalNumberofPatternFiles++;
        } while (TRUE);
    } while (FALSE);

    if (IsError)
    {
        *NumberOfPatternFiles = 0;
        return NULL;
    }
    TotalNumberofPatternFiles++;
    StartFuSaHeader = AllocatePool(TotalNumberofPatternFiles * sizeof(StartFuSaHeader));
    for (Count = 0; Count < TotalNumberofPatternFiles ; Count++)
    {
        StartFuSaHeader[Count] = FuSaPatternArray[Count];
    }
    *NumberOfPatternFiles = TotalNumberofPatternFiles;
    return StartFuSaHeader;
}
#endif
VOID
FspDxCheckGrtScanBist (
  IN  UINT32            FusaStartupFileAddr,
  OUT FUSA_TEST_RESULT  *pFusaTestResult
  )
{
  UINT32                            CoreIndex;
  CORE_IDI_TEST_PARAM               CoreIdiTestParam;
  FUSA_INFO                         *Local_Fusa_info;
  UINT32                            CoreCount;
//#ifdef FUSA_PATTERN_FILE_ACCESS
//  UINT32                           FusaPatternFileSize;
//#endif
  UINT32                           *TargetAddr;
  TEST_CONFIG_SCANBIST              TestConfig;

  FUSA_PATTERN_FILE_HEADER         *FuSaHeaderPtr;
  UINT8                             FusaPatternFileNumber;
  UINT8                             TotalNumberOfPatternFiles;
  FUSA_PATTERN_FILE_HEADER         **FusaTestpatternHeaderptr;

  /* Initialize Test Rsult */
  for (int i = 0; i < 8; i++)
  {
    pFusaTestResult[i].TestNumber = FusaTestNumGrtScan0Bist + i;
    pFusaTestResult[i].TotalChecks = 0;
    pFusaTestResult[i].TestResult = FUSA_TEST_NOT_SUPPORTED;
    GenerateCrc(&pFusaTestResult[i]);
  }

  //
  //First get all the pattern files in single array.
  //
  //FusaTestpatternHeaderptr = GetTotalNumberOfPatternFiles(FusaStartupFileAddr, &TotalNumberOfPatternFiles);
  FusaTestpatternHeaderptr = (FUSA_PATTERN_FILE_HEADER**) 0; //hardcoding, need relook
  TotalNumberOfPatternFiles = 8; //hardcoding, need relook
  if (0 == TotalNumberOfPatternFiles || NULL == FusaTestpatternHeaderptr)
  {
      DEBUG((DEBUG_INFO, "There is no Pattern file to test \n"));
      return;
  }
  DEBUG((DEBUG_INFO, "Total Number of Pattern Files are [%d] Id = 0x%x\n", TotalNumberOfPatternFiles));
  for (int i = 0; i < TotalNumberOfPatternFiles; i++)
  {
      FuSaHeaderPtr = FusaTestpatternHeaderptr[i];
      DEBUG((DEBUG_INFO, " Fusa FuSaHeaderPtr-> HeaderVersion; =%x\n", FuSaHeaderPtr->HeaderVersion));
      DEBUG((DEBUG_INFO, " Fusa FuSaHeaderPtr-> PatchID; =%x\n", FuSaHeaderPtr->PatchID));
      DEBUG((DEBUG_INFO, " Fusa FuSaHeaderPtr-> Date; =%x\n", FuSaHeaderPtr->Date));
      DEBUG((DEBUG_INFO, " Fusa FuSaHeaderPtr-> CpuId; =%x\n", FuSaHeaderPtr->CpuId));
      DEBUG((DEBUG_INFO, " Fusa FuSaHeaderPtr-> Checksum; =%x\n", FuSaHeaderPtr->Checksum));
      DEBUG((DEBUG_INFO, " Fusa FuSaHeaderPtr-> LoaderVersion; =%x\n", FuSaHeaderPtr->LoaderVersion));
      DEBUG((DEBUG_INFO, " Fusa FuSaHeaderPtr-> PlatformID; =%x\n", FuSaHeaderPtr->PlatformID));
      DEBUG((DEBUG_INFO, " Fusa FuSaHeaderPtr-> DataSize; =%x\n", FuSaHeaderPtr->DataSize));
      DEBUG((DEBUG_INFO, " Fusa FuSaHeaderPtr-> TotalSize; =%x\n", FuSaHeaderPtr->TotalSize));
      DEBUG((DEBUG_INFO, " Fusa FuSaHeaderPtr-> FuSaInfoReserve =%x\n", FuSaHeaderPtr->FuSaInfoReserved));
      DEBUG((DEBUG_INFO, " Fusa FuSaHeaderPtr-> Reserved1; =%x\n", FuSaHeaderPtr->Reserved1));
      DEBUG((DEBUG_INFO, " Fusa FuSaHeaderPtr-> Reserved2; =%x\n", FuSaHeaderPtr->Reserved2));
  }

  Local_Fusa_info = FusaInfoListGet();

  //intialize the test result buffer as some test may not run because of the device not being available
  CoreCount = 0;
  for (CoreIndex = 0; CoreIndex < Local_Fusa_info->TotalProcessor ; CoreIndex++)
  {
    if( Local_Fusa_info->Processor[CoreIndex].CoreType == CPUID_CORE_TYPE_INTEL_ATOM )
    {
      FUSA_LIB_STATUS LibStatus;

      DEBUG ((DEBUG_INFO, "Initializa Test Report on CoreNumber[%d] Id = 0x%x\n", CoreIndex,
            Local_Fusa_info->Processor[CoreIndex].ProcessorInfo.ProcessorId ));

      DEBUG ((DEBUG_INFO, "FspDxCheckGrtScanBist on Local_Fusa_info->Processor[%x].CoreType = 0x%x, Fusa_Capabilities =0x%x\n",
          CoreIndex,
          Local_Fusa_info->Processor[CoreIndex].CoreType,
          Local_Fusa_info->Processor[CoreIndex].Fusa_Capabilities));

      LibStatus = FusaTestAndReporting (
                    NULL,
                    FusaTestNumGrtScan0Bist + CoreCount,
                    TotalNumberOfPatternFiles,
                    &(pFusaTestResult[CoreCount])
                    );

      ASSERT(LibStatus == FusaNoError);
      CoreCount++;
    }
  }

  for (FusaPatternFileNumber  = 0; FusaPatternFileNumber < TotalNumberOfPatternFiles; FusaPatternFileNumber++)
  {
    FuSaHeaderPtr = FusaTestpatternHeaderptr[FusaPatternFileNumber];
    TargetAddr = (UINT32*)FuSaHeaderPtr + FUSA_PATTERN_DATA_OFF;
    TestConfig.buffAddr = (UINT64)TargetAddr | BIT0;
    RunAllAp(
            (AP_PROCEDURE)FspDxCheckGrScanBistSetInternal,
            &TestConfig
            );
    CoreCount = 0;
    for (CoreIndex = 0; CoreIndex < Local_Fusa_info->TotalProcessor; CoreIndex++)
    {
        DEBUG((DEBUG_INFO, "FspDxCheckGrtScanBist:  CoreIndex %x\n", CoreIndex));
        if (Local_Fusa_info->Processor[CoreIndex].CoreType == CPUID_CORE_TYPE_INTEL_ATOM)
        {
            CoreIdiTestParam.CoreNumber = CoreCount;
            CoreIdiTestParam.pFusaTestResult = &(pFusaTestResult[CoreCount]);
            CoreIdiTestParam.FusaPatternFileNumber = FusaPatternFileNumber;
            DEBUG((DEBUG_INFO, "FspDxCheckGrtScanBist:  RunAtAp\n"));
            RunAtAp(
                (AP_PROCEDURE)FspDxCheckGrScanBistReadInternal,
                CoreIndex,
                &CoreIdiTestParam
            );

            CoreCount++;
        }
    }
  }

  CoreCount = 0;
  for (CoreIndex = 0; CoreIndex < Local_Fusa_info->TotalProcessor; CoreIndex++)
  {
      if (Local_Fusa_info->Processor[CoreIndex].CoreType == CPUID_CORE_TYPE_INTEL_ATOM)
      {
          GenerateCrc(&pFusaTestResult[CoreCount]);
          DumpResults(&pFusaTestResult[CoreCount]);
          CoreCount++;
      }
  }
}
