/** @file
  This file Implement the public interface for  Fusa
   E2E CTC implementation

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
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Uefi/UefiMultiPhase.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>
#include <Library/HobLib.h>
#include <PiPei.h>
#include <Ppi/MpServices2.h>
#include <Library/PeiServicesLib.h>
#include <Library/PmcLib.h>
#include <Library/MemoryAllocationLib.h>
#include "PeiFusaPrivateLibInternal.h"
#include "PeiFusaResultReportingLib.h"
#include "PeiFusaE2eCtcLibInternal.h"

UINT8 *gpTestData = NULL;

//map unique core to processor number used by MP services, processor number 0 is being ignored as it refers to the BSP
//use array size of 128 so that we do not need to do array index check, although we expect the initial APIC ID max at
STATIC UINT32                   mUniqueCoreList[128];

extern EFI_GUID                   gEdkiiPeiMpServices2PpiGuid;
STATIC CONST EFI_PEI_SERVICES     **mPeiServices;
STATIC EDKII_PEI_MP_SERVICES2_PPI *mMpServices2Ppi = NULL;


/**
  To retrieve  mUniqueCoreList

  @retval mUniqueCoreList
**/
CONST UINT32 *
UniqueCoreListGet (
  VOID
  )
{
  return mUniqueCoreList;
}

/**
  To execute routine "Routine" with parameters "Param" at AP
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
  )
{
  EFI_STATUS Status;
  Status = mMpServices2Ppi->StartupThisAP (
                             mMpServices2Ppi,
                             (EFI_AP_PROCEDURE) Routine,
                             ProcessorNumber,
                             0,
                             Param);

  DEBUG ((DEBUG_INFO, "AP number %d calling status = 0x%x\n", ProcessorNumber, Status));
}

/**
  To populate the mUniqueCoreList array with the processor
  number used for multi-processor calling. Array index implies
  core number. Array value zero implies related core being not
  available. This list does not track the BSP because BSP core
  is always available and tested without MP calling.

  @param[in] PeiServices

  @return error status if MpService PPI was not installed

**/
STATIC
EFI_STATUS
CoreInfoPopulate (
    IN  CONST EFI_PEI_SERVICES  **PeiServices
    )
{
  UINTN NumberOfProcessors;
  UINTN NumberOfEnabledProcessors;
  UINT32 ProcessorNumber;
  EFI_STATUS Status;

  mPeiServices = PeiServices;  //store for later RunAtAp usage

  // Locate CpuMpCpu MpService Ppi
  Status = PeiServicesLocatePpi (
             &gEdkiiPeiMpServices2PpiGuid,
             0,
             NULL,
             (VOID **) &mMpServices2Ppi
             );

  if (EFI_SUCCESS != Status) {
    ASSERT_EFI_ERROR (Status);
  } else {
    mMpServices2Ppi->GetNumberOfProcessors (
                      mMpServices2Ppi,
                      &NumberOfProcessors,
                      &NumberOfEnabledProcessors
                      );

    for (ProcessorNumber = 0; ProcessorNumber < NumberOfProcessors; ProcessorNumber++) {
      Status = mMpServices2Ppi->GetProcessorInfo (
                                 mMpServices2Ppi,
                                 ProcessorNumber,
                                 &(ProcessorInfoBuffer[ProcessorNumber])
                                 );
      ASSERT_EFI_ERROR (Status);
    }

    SetMem ((UINT8 *)mUniqueCoreList, sizeof(mUniqueCoreList), 0 );

    //search for unique core, ProcessorNumber 0 is BSP so we skip it
    for (ProcessorNumber = 1; ProcessorNumber < NumberOfProcessors; ProcessorNumber++)
    {
      UINT8 CoreIndex = (UINT8) (ProcessorInfoBuffer[ProcessorNumber].ProcessorId) >> 1;
      if (CoreIndex != (UINT8) (ProcessorInfoBuffer[0].ProcessorId) >> 1) //skip the SMT sharing the same core with BSP
      {
        //if there were 2 active SMT belong to the same core, then we only remember the last one
        mUniqueCoreList[CoreIndex] = ProcessorNumber;
        DEBUG ((DEBUG_INFO, "UniqueCoreList[%d] = %d\n", CoreIndex, ProcessorNumber));
      }
    }
    //now all non-zero UniqueCoreList[] contains the unique Cpu number not the same as BSP
  }

  return Status;
}

#define R_PMC_PWRM_FUSA_STS_CTL                     0x1F30U
#define B_PMC_PWRM_FUSA_STS_CTL_DIAGTEST_EN         BIT2
#define B_PMC_PWRM_FUSA_STS_CTL_DIAGTEST_PCHMODE    BIT1
#define B_PMC_PWRM_FUSA_STS_CTL_DIAGTEST_FEATURE_EN BIT0

/**
  Check if the system is under Fusa diagnostic mode

  @note Bootloader does not rely on this routine for the
        diagnostic mode detection but the existence of the
        gSiFusaInfoGuid HOB

  @retval TRUE if system is under Fusa diagnostic mode
  @retval FALSE otherwise

**/
BOOLEAN
FspDxDiagnosticModeGet (
  VOID
  )
{
  BOOLEAN FusaDiagTestMode;
  UINT32  FusaStsCtl;
  UINT32  PwrmBaseAddress = PmcGetPwrmBase ();

  FusaStsCtl = MmioRead32 ((UINTN) PwrmBaseAddress + R_PMC_PWRM_FUSA_STS_CTL);
  DEBUG ((DEBUG_INFO, "DIAGTEST_FEATURE_EN: %d\nDIAGTEST_EN: %d\nDIAGTEST_PCHMODE: %d\n",
          FusaStsCtl & B_PMC_PWRM_FUSA_STS_CTL_DIAGTEST_FEATURE_EN,
          (FusaStsCtl & B_PMC_PWRM_FUSA_STS_CTL_DIAGTEST_EN) >> 2,
          (FusaStsCtl & B_PMC_PWRM_FUSA_STS_CTL_DIAGTEST_PCHMODE) >> 1));

  if (((FusaStsCtl & B_PMC_PWRM_FUSA_STS_CTL_DIAGTEST_FEATURE_EN) != 0)
      &&((FusaStsCtl & B_PMC_PWRM_FUSA_STS_CTL_DIAGTEST_EN) != 0)
      &&((FusaStsCtl & B_PMC_PWRM_FUSA_STS_CTL_DIAGTEST_PCHMODE) == 0))
  {
    FusaDiagTestMode = TRUE;
  } else {
    FusaDiagTestMode = FALSE;
  }

  DEBUG ((DEBUG_INFO, "Diagnostic Mode detected is %d\n", FusaDiagTestMode));

  return FusaDiagTestMode;
}

/**
  Init and Install Fusa Info Hob

  @retval EFI_OUT_OF_RESOURCES if the HOB resource allocation
          fails
  @retval EFI_SUCCESS otherwise
**/
EFI_STATUS
InstallFusaInfoHob (
  VOID
  )
{
  EFI_STATUS        Status;
  FUSA_INFO_HOB     FusaInfo;
  FUSA_INFO_HOB     *pFusaInfoHob;

  if (FspDxDiagnosticModeGet ()) {
    ZeroMem ( &FusaInfo, sizeof (FusaInfo));
    FusaInfo.Version = FUSA_INFO_VERSION;

    //
    // Create HOB for Fusa INFO
    //
    pFusaInfoHob = BuildGuidDataHob (
                       &gSiFusaInfoGuid,
                       &FusaInfo,
                       sizeof (FUSA_INFO_HOB)
                       );
    if (NULL == pFusaInfoHob) {
      Status = EFI_OUT_OF_RESOURCES;
    } else {
      Status = EFI_SUCCESS;
    }
  } else {
    Status = EFI_SUCCESS;
  }

  return Status;
}

/**
  Main check the checker routine that will perform all the check
  the checker test (exclude MemSS CTC) and update the test
  result HOB.

  If systematic fault exists in such the MpServices PPI was not
  installed, the test result HOB is still being produced with
  zero content from this test, which the bootloader stage can
  detect the issue.

  @note This routine shall be called before BIOS_DONE or CPU
        PRMRR setup, or another word, prior to CpuInit(). Some
        of the test are using memory reference and thus it shall
        be called after system memory is avaialable.

  @pre  MpService PPI was installed

  @param[in] PeiServices    Pointer to PEI Services Table

**/
VOID
FspDxCheck (
  IN  CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_HOB_GUID_TYPE      *GuidHob;
  FUSA_INFO_HOB        *FusaInfo;
  FUSA_TEST_RESULT     *TestResult;
  EFI_STATUS Status;

  GuidHob = NULL;
  FusaInfo = NULL;

  GuidHob = GetFirstGuidHob (&gSiFusaInfoGuid);
  if (GuidHob != NULL) {
    FusaInfo = (FUSA_INFO_HOB *) GET_GUID_HOB_DATA (GuidHob);
  }

  if (FusaInfo != NULL) {
    TestResult = FusaInfo->FspDxCtcTestResult;

    McaBankStatusDumpAll ();// dump to see if any bank status seen.
    McaBankStatusClearAll ();

    gpTestData = (UINT8 *) AllocatePages (EFI_SIZE_TO_PAGES (1028 * 1024));
    if (gpTestData == NULL) {
      ASSERT(FALSE);
      Status = EFI_OUT_OF_RESOURCES;
    } else {
      Status = CoreInfoPopulate (PeiServices); //init
    }

    if (EFI_SUCCESS == Status) {
      FspDxCheckCoreIdi (&(TestResult[FusaTestNumCpu0Idi]) );
      FspDxCheckLlc (&(TestResult[FusaTestNumCboSlice0Ingress]) );
      FspDxCheckDip (&(TestResult[FusaTestNumDip]));
      FspDxCheckIoPort (&(TestResult[FusaTestNumIop]));
      FspDxCheckOpiLink (&(TestResult[FusaTestNumOpiLinkIosfData]));
    }
  } else {
    ASSERT (FALSE); //expect the existence of the HOB
  }
}

