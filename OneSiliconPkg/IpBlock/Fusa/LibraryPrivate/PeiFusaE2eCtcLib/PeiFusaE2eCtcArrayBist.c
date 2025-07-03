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


///
/// Fusa Configuration Registers
///
#define MSR_TRIGGER_STARTUP_MEM_BIST        0x000002D4

#define MSR_STARTUP_MEM_BIST_STATUS         0x000002D5
#define B_MSR_STARTUP_MEM_BIST_STATUS_TEST_IN_PROGRESS_MASK BIT2
#define B_MSR_STARTUP_MEM_BIST_STATUS_SELF_TEST_STATUS_MASK BIT1
#define B_MSR_STARTUP_MEM_BIST_STATUS_TEST_RESULT_MASK BIT0

#define MSR_INTEGRITY_CAPABILITIES             0x000002D9
#define B_MSR_FUSA_CAP_STARTUP_ARRAY_DIAGNOGSIS_MASK    BIT1


/// Structure of the parameter passed into the Core Idi test routine
typedef struct
{
  FUSA_TEST_RESULT *pFusaTestResult;
  FUSA_TEST_RESULT *pFusaCRCTestResult;
  UINT32           CoreNumber;
} CORE_IDI_TEST_PARAM;

/**
  Retrieve test result in each of the core

  @param[in,out] pCoreIdiTestParam - conttain result buffer
       where the test result to be updated to beside core number
       information
**/
VOID
FspDxCheckArrayBistReadInternal (
  IN CORE_IDI_TEST_PARAM *pCoreIdiTestParam
  )
{
  //UINT32 CoreNumber;
  FUSA_TEST_RESULT *pFusaTestResult;
  FUSA_TEST_RESULT *pFusaCRCTestResult;
  FUSA_LIB_STATUS LibStatus;
  UINT64 FusaConfigMsr;
  
  pFusaTestResult = pCoreIdiTestParam->pFusaTestResult;
  pFusaCRCTestResult = pCoreIdiTestParam->pFusaCRCTestResult;
  //CoreNumber = pCoreIdiTestParam->CoreNumber;
  FusaConfigMsr = AsmReadMsr64 (MSR_STARTUP_MEM_BIST_STATUS);
  if ((FusaConfigMsr & B_MSR_STARTUP_MEM_BIST_STATUS_TEST_RESULT_MASK) == 0x0) { 
    LibStatus = UpdateResults(FUSA_TEST_PASS, 1U, pFusaCRCTestResult);
  } else {
    LibStatus = UpdateResults(FUSA_TEST_FAIL, 1U, pFusaCRCTestResult);
  }
  if (FusaConfigMsr & B_MSR_STARTUP_MEM_BIST_STATUS_SELF_TEST_STATUS_MASK) { 
    LibStatus = UpdateResults(FUSA_TEST_PASS, 0U, pFusaCRCTestResult);
  } else {
    LibStatus = UpdateResults(FUSA_TEST_FAIL, 0U, pFusaCRCTestResult);
  }
  if (FusaConfigMsr & B_MSR_STARTUP_MEM_BIST_STATUS_TEST_IN_PROGRESS_MASK) { 
    LibStatus = UpdateResults(FUSA_TEST_PASS, 0U, pFusaTestResult);
  } else {
    LibStatus = UpdateResults(FUSA_TEST_FAIL, 0U, pFusaTestResult);
  }

  LibStatus |= GenerateCrc (pFusaTestResult); 
  ASSERT(LibStatus == FusaNoError);

  LibStatus |= GenerateCrc (pFusaCRCTestResult); 
  ASSERT(LibStatus == FusaNoError);

}

/**
  Triger Setup Mem Bist on each fo the core

  @param[in,out] pCoreIdiTestParam - conttain result buffer
       where the test result to be updated to beside core number
       information
**/
VOID
EFIAPI
FspDxCheckArrayBistSetInternal (
  IN CORE_IDI_TEST_PARAM *pCoreIdiTestParam
  )
{
  CPUID_NATIVE_MODEL_ID_AND_CORE_TYPE_EAX Eax;
  UINT64 fusa_capability;
  

  AsmCpuid (CPUID_HYBRID_INFORMATION, &Eax.Uint32, NULL, NULL, NULL);  
  fusa_capability = AsmReadMsr64 (MSR_INTEGRITY_CAPABILITIES);

  if( ((UINT8)Eax.Bits.CoreType == CPUID_CORE_TYPE_INTEL_ATOM) 
      && (fusa_capability & B_MSR_FUSA_CAP_STARTUP_ARRAY_DIAGNOGSIS_MASK))
  {
    AsmWriteMsr64 (MSR_TRIGGER_STARTUP_MEM_BIST, (UINT64)BIT0);
  }

  return;
}

/**
  Perform Atom PBist.
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
FspDxCheckArrayAndCRCBist (
  OUT FUSA_TEST_RESULT *pFusaTestResult,
  OUT FUSA_TEST_RESULT *pFusaCRCTestResult
  )
{
  UINT32 CoreIndex;
  CORE_IDI_TEST_PARAM CoreIdiTestParam;
  FUSA_INFO *Local_Fusa_info;
  UINT32 CoreCount;
  FUSA_LIB_STATUS LibStatus;

  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  Local_Fusa_info = FusaInfoListGet();

  //intialize the test result buffer as some test may not run because of the device not being available
  CoreCount = 0;
  for (CoreIndex = 0; CoreIndex < MAX_PTL_ATOM_CORE_COUNT ; CoreIndex++)
  {
    if( Local_Fusa_info->Processor[CoreIndex].CoreType == CPUID_CORE_TYPE_INTEL_ATOM )
    {
      DEBUG ((DEBUG_INFO, "Initialize Test Report on CoreNumber[%d] Id = 0x%x\n", CoreIndex,
            Local_Fusa_info->Processor[CoreIndex].ProcessorInfo.ProcessorId ));

      DEBUG ((DEBUG_INFO, "%a on Local_Fusa_info->Processor[%x].CoreType = 0x%x, Fusa_Capabilities =0x%x\n",
          __FUNCTION__,
          CoreIndex,
          Local_Fusa_info->Processor[CoreIndex].CoreType,
          Local_Fusa_info->Processor[CoreIndex].Fusa_Capabilities));

      LibStatus = FusaTestAndReporting (
                    NULL,
                    FusaTestNumArray0Bist + CoreCount,
                    1U,
                    &(pFusaTestResult[CoreCount])
                    );

      ASSERT(LibStatus == FusaNoError);

      LibStatus = FusaTestAndReporting (
                    NULL,
                    FusaTestNumCRCon0PBISTROM + CoreCount,
                    2U,
                    &(pFusaCRCTestResult[CoreCount])
                    );

      ASSERT(LibStatus == FusaNoError);
    } else {
      LibStatus = FusaTestAndReporting (
        NULL,
        FusaTestNumArray0Bist + CoreCount,
        1U,
        &(pFusaTestResult[CoreCount])
        );

      LibStatus = FusaTestAndReporting (
        NULL,
        FusaTestNumCRCon0PBISTROM + CoreCount,
        2U,
        &(pFusaCRCTestResult[CoreCount])
        );
    }
    CoreCount++;
  }

  RunAllAp (
    (AP_PROCEDURE) FspDxCheckArrayBistSetInternal,
    NULL
    );

  CoreCount = 0;
  for (CoreIndex = 0; CoreIndex < Local_Fusa_info->TotalProcessor ; CoreIndex++)
  {
    if( Local_Fusa_info->Processor[CoreIndex].CoreType == CPUID_CORE_TYPE_INTEL_ATOM )
    {
      CoreIdiTestParam.CoreNumber = CoreCount;
      CoreIdiTestParam.pFusaTestResult = &(pFusaTestResult[CoreCount]);
      CoreIdiTestParam.pFusaCRCTestResult = &(pFusaCRCTestResult[CoreCount]);
      RunAtAp (
        (AP_PROCEDURE) FspDxCheckArrayBistReadInternal,
          CoreIndex,
          &CoreIdiTestParam
        );

      DumpResults (CoreIdiTestParam.pFusaTestResult);
      DumpResults (CoreIdiTestParam.pFusaCRCTestResult);
      CoreCount++;
    }
  }
}
