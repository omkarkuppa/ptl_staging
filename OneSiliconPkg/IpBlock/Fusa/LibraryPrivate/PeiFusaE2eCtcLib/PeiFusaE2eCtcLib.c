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
#include <Library/PcdLib.h>
#include <Uefi/UefiMultiPhase.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>
#include <Library/HobLib.h>
#include <PiPei.h>
#include <Ppi/MpServices2.h>
#include <Library/PeiServicesLib.h>
#include <Library/LocalApicLib.h>
#include <Library/PmcLib.h>
#include <Library/MemoryAllocationLib.h>
#include "PeiFusaPrivateLibInternal.h"
#include "PeiFusaResultReportingLib.h"
#include "PeiFusaE2eCtcLibInternal.h"
#include "PeiFusaCfiParity.h"
#include "PeiFusaE2eCtcLlcDoubleParity.h"
#include "PeiFusaCcfE2eIdiParityCtc.h"
#include "PeiFusaCoreCtc.h"
#include "PeiFusaIbecc.h"
#include "PeiFusaGtDeCtcLib.h"
#include <Library/FusaInfoLib.h>
#include <Register/Cpuid.h>
#include <Register/Ptl/Msr/MsrRegs.h>

UINT8 *gpTestData = NULL;
UINT32 mCrc32cTable[256];

//map unique core to processor number used by MP services, processor number 0 is being ignored as it refers to the BSP
//use array size of 128 so that we do not need to do array index check, although we expect the initial APIC ID max at
//(MAX_TGL_CORE_COUNT*2) -1
STATIC UINT64                   mUniqueCoreList[64];


STATIC FUSA_INFO  Fusa_info;

#define MSR_INTEGRITY_CAPABILITIES             0x000002D9

extern EFI_GUID                   gEdkiiPeiMpServices2PpiGuid;
STATIC CONST EFI_PEI_SERVICES     **mPeiServices;
STATIC EDKII_PEI_MP_SERVICES2_PPI *mMpServices2Ppi = NULL;

#define CRC32C_POLY_NORMAL        0x1EDC6F41

/**
  This internal function reverses bits for 32bit data.

  @param  Value The data to be reversed.

  @return       Data reversed.

**/
UINT32
ReverseBits (
  UINT32  Value
  )
{
  UINTN  Index;
  UINT32 NewValue;

  NewValue = 0;
  for (Index = 0; Index < 32; Index++) {
    if ((Value & (1 << Index)) != 0) {
      NewValue = NewValue | (1 << (31 - Index));
    }
  }

  return NewValue;
}

/**
  Generate a CRC-32C table that can speed up CRC calculation by table look up.
**/
VOID
Crc32cInit(
  VOID
  )
{
  UINT32 TableEntry;
  UINT32 Index;
  UINT32 CrcVal;

  for (TableEntry = 0; TableEntry < 256; TableEntry++) {
    CrcVal = ReverseBits ((UINT32) TableEntry);
    for (Index = 0; Index < 8; Index++) {
      if ((CrcVal & 0x80000000) != 0) {
        CrcVal = (CrcVal << 1) ^ CRC32C_POLY_NORMAL;
      } else {
        CrcVal = CrcVal << 1;
      }
    }

    mCrc32cTable[TableEntry] = ReverseBits (CrcVal);
  }
}

/**
  Calculate CRC32 value of a buffer.

  @param[in] *pBuffer   - input buffer for the CRC32 calculation
  @param[in] Len        - length of the buffer to be
        CRC32-calculated
  @param[in] InitVal    - init value use for the CRC32
        calculation

  @retval calculated CRC32 value
**/
UINT32
AsmCrc32Calc (
  IN UINT8 *pBuffer,
  IN UINT32 Len,
  IN UINT32 InitVal
  )
{
  UINT32  Crc;
  UINTN   Index;
  UINT8   *Ptr;

  if ((Len == 0) || (pBuffer == NULL)) {
    return 0;
  }

  Crc = 0xffffffff;
  for (Index = 0, Ptr = pBuffer; Index < Len; Index++, Ptr++) {
    Crc = (Crc >> 8) ^ mCrc32cTable[(UINT8) Crc ^ *Ptr];
  }

  return (Crc);
}

/**
  To retrieve  mUniqueCoreList

  @retval mUniqueCoreList
**/
CONST UINT64 *
UniqueCoreListGet (
  VOID
  )
{
  return mUniqueCoreList;
}

/**
  To retrieve  mUniqueCoreList

  @retval mUniqueCoreList
**/
FUSA_INFO *
FusaInfoListGet (
  VOID
  )
{
  return &Fusa_info;
}

/**
  To retrieve  TotalCoreProcessor

  @retval TotalCoreProcessor
**/
CONST UINTN
GetNumberofCoreCPU (
  VOID
  )
{
  return Fusa_info.TotalCoreProcessor;
}

/**
  To retrieve  TotalAtomProcessor

  @retval TotalAtomProcessor
**/
CONST UINTN
GetNumberofAtomCPU (
  VOID
  )
{
  return Fusa_info.TotalAtomProcessor;
}

/**
**/
VOID
FspCPUTypeCheckInternal (
  IN PROCESSOR_INFO *Processor_info
  )
{
    PROCESSOR_INFO *pProcessor_info;
    CPUID_NATIVE_MODEL_ID_AND_CORE_TYPE_EAX Eax;

    pProcessor_info = Processor_info;

    AsmCpuid (CPUID_HYBRID_INFORMATION, &Eax.Uint32, NULL, NULL, NULL);
    pProcessor_info->CoreType = Eax.Bits.CoreType;

    pProcessor_info->Fusa_Capabilities.Uint64 = AsmReadMsr64 (MSR_INTEGRITY_CAPABILITIES);

    GetProcessorLocationByApicId (
      (UINT32) pProcessor_info->ProcessorInfo.ProcessorId,
      &(pProcessor_info->ProcessorInfo.Location.Package),
      &(pProcessor_info->ProcessorInfo.Location.Core),
      &(pProcessor_info->ProcessorInfo.Location.Thread)
      );

    GetProcessorLocation2ByApicId (
      (UINT32) pProcessor_info->ProcessorInfo.ProcessorId,
      &(pProcessor_info->ProcessorInfo.ExtendedInformation.Location2.Package),
      &(pProcessor_info->ProcessorInfo.ExtendedInformation.Location2.Die),
      &(pProcessor_info->ProcessorInfo.ExtendedInformation.Location2.Tile),
      &(pProcessor_info->ProcessorInfo.ExtendedInformation.Location2.Module),
      &(pProcessor_info->ProcessorInfo.ExtendedInformation.Location2.Core),
      &(pProcessor_info->ProcessorInfo.ExtendedInformation.Location2.Thread)
      );
}

/**
  This function to update variable
**/
VOID
UpdateBSPInfo (
  VOID
  )
{
  EFI_STATUS Status;

  Status = mMpServices2Ppi->WhoAmI (
                        mMpServices2Ppi,
                        &Fusa_info.BspCpu
                        );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "ERROR who am I! Status: %x\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "Main core's CpuNumber =  %x\n", Fusa_info.BspCpu));
  }
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
  To execute routine "Routine" with parameters "Param" at
  All AP, in blocking mode.

  @param[in] Routine - routine to be executed
  @param[in] ProcessorNumber - target AP processor number
                         enumerated by the MP services
  @param[in] Param - parameters to be used by the said routine

**/
VOID
RunAllAp (
  IN AP_PROCEDURE Routine,
  IN VOID         *Param
  )
{
  EFI_STATUS Status;

  Status = mMpServices2Ppi->StartupAllCPUs (
                     mMpServices2Ppi,
                     (EFI_AP_PROCEDURE) Routine,
                     0,
                     Param);

  DEBUG ((DEBUG_INFO, "AP All calling status  = 0x%x\n", Status));
}



/**
  Print to debug Fusa Info

  @param[in] Fusa_info

  @return error status if PrintFusa_info was not success
**/
STATIC
EFI_STATUS
PrintFusa_info (
    IN FUSA_INFO *Fusa_info_ptr
    )
{
  FUSA_INFO *pFusa_info;
  pFusa_info = Fusa_info_ptr;
  UINT32 ProcessorNumber;

  for (ProcessorNumber = 0; ProcessorNumber < pFusa_info->TotalProcessor; ProcessorNumber++) {
    DEBUG((DEBUG_INFO, "The CurrProcessor 0x%x/0x%x ApicId is 0x%x\n",
                pFusa_info->TotalProcessor,
                ProcessorNumber,
                pFusa_info->Processor[ProcessorNumber].ProcessorInfo.ProcessorId));

    DEBUG((DEBUG_INFO, "pFusa_info->Processor[0x%x].Location.Package = %x\n",
                ProcessorNumber,
                pFusa_info->Processor[ProcessorNumber].ProcessorInfo.Location.Package));

    DEBUG((DEBUG_INFO, "pFusa_info->Processor[0x%x].Location.Core = %x\n",
                ProcessorNumber,
                pFusa_info->Processor[ProcessorNumber].ProcessorInfo.Location.Core));

    DEBUG((DEBUG_INFO, "pFusa_info->Processor[0x%x].Location.Thread = %x\n",
                ProcessorNumber,
                pFusa_info->Processor[ProcessorNumber].ProcessorInfo.Location.Thread));

    DEBUG((DEBUG_INFO, "pFusa_info->Processor[0x%x].ExtendedInformation.Module = %x\n",
                ProcessorNumber,
                pFusa_info->Processor[ProcessorNumber].ProcessorInfo.ExtendedInformation.Location2.Module));

    DEBUG((DEBUG_INFO, "pFusa_info->Processor[0x%x].ExtendedInformation.Tile = %x\n",
                ProcessorNumber,
                pFusa_info->Processor[ProcessorNumber].ProcessorInfo.ExtendedInformation.Location2.Tile));

    DEBUG((DEBUG_INFO, "pFusa_info->Processor[0x%x].ExtendedInformation.Die = %x\n",
                ProcessorNumber,
                pFusa_info->Processor[ProcessorNumber].ProcessorInfo.ExtendedInformation.Location2.Die));

    DEBUG((DEBUG_INFO, "pFusa_info->Processor[0x%x].ExtendedInformation.Core = %x\n",
                ProcessorNumber,
                pFusa_info->Processor[ProcessorNumber].ProcessorInfo.ExtendedInformation.Location2.Core));

    DEBUG((DEBUG_INFO, "pFusa_info->Processor[0x%x].ExtendedInformation.Thread = %x\n",
                ProcessorNumber,
                pFusa_info->Processor[ProcessorNumber].ProcessorInfo.ExtendedInformation.Location2.Thread));

    DEBUG((DEBUG_INFO, "pFusa_info->Processor[0x%x].CoreType = %x\n",
                ProcessorNumber,
                pFusa_info->Processor[ProcessorNumber].CoreType));

    DEBUG((DEBUG_INFO, "pFusa_info->Processor[0x%x].Fusa_Capabilities = %x\n",
                ProcessorNumber,
                pFusa_info->Processor[ProcessorNumber].Fusa_Capabilities));
  }

  DEBUG((DEBUG_INFO, "pFusa_info->TotalCoreProcessor = %x\n",
              pFusa_info->TotalCoreProcessor));

  DEBUG((DEBUG_INFO, "pFusa_info->TotalAtomProcessor = %x\n",
              pFusa_info->TotalAtomProcessor));

  DEBUG((DEBUG_INFO, "pFusa_info->TotalProcessor = %x\n",
              pFusa_info->TotalProcessor));

  DEBUG((DEBUG_INFO, "pFusa_info->BspCpu = %x\n",
              pFusa_info->BspCpu));

  return EFI_SUCCESS;
}

/**
  Switch to Core CPU as BSP

  @param[in] FUSA_INFO - All the Fusa info
  @param[in] UINTN Core - switching to target Core

  @return error status if SwitchCoreBSP was not success
**/
EFI_STATUS
Switch2BSP (
    IN FUSA_INFO *Fusa_info_ptr,
    IN UINTN Core
  )
{
  EFI_STATUS Status;
  FUSA_INFO *pFusa_info;
  pFusa_info = Fusa_info_ptr;

  pFusa_info->BspCpu =  Core;

  DEBUG ((DEBUG_INFO, "Switching over BSP to =  %x\n", pFusa_info->BspCpu));
  Status = mMpServices2Ppi->SwitchBSP (
                        mMpServices2Ppi,
                        pFusa_info->BspCpu,
                        TRUE
                        );

  Status = mMpServices2Ppi->WhoAmI (
                        mMpServices2Ppi,
                        &pFusa_info->BspCpu
                        );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "ERROR who am I! Status: %x\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "Main core's CpuNumber =  %x\n", pFusa_info->BspCpu));
  }

  return Status;
}

/**
  Switch to Core CPU as BSP

  @param[in] FUSA_INFO - All the Fusa info

  @return error status if SwitchCoreBSP was not success
**/
EFI_STATUS
SwitchCoreBSP (
    IN FUSA_INFO *Fusa_info_ptr
  )
{
  EFI_STATUS Status;
  FUSA_INFO *pFusa_info;
  pFusa_info = Fusa_info_ptr;

  pFusa_info->BspCpu =  0;

  DEBUG ((DEBUG_INFO, "Switching over BSP to =  %x\n", pFusa_info->BspCpu));
  Status = mMpServices2Ppi->SwitchBSP (
                        mMpServices2Ppi,
                        pFusa_info->BspCpu,
                        TRUE
                        );

  Status = mMpServices2Ppi->WhoAmI (
                        mMpServices2Ppi,
                        &pFusa_info->BspCpu
                        );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "ERROR who am I! Status: %x\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "Main core's CpuNumber =  %x\n", pFusa_info->BspCpu));
  }

  return Status;
}

#if 0 //unused function
/**
  Switch to ATOM CPU as BSP

  @return error status if SwitchATOMBSP was not success
**/
STATIC
EFI_STATUS
SwitchATOMBSP (
    IN FUSA_INFO *Fusa_info_ptr
  )
{
  UINT32 ProcessorNumber;
  EFI_STATUS Status;
  FUSA_INFO *pFusa_info;
  pFusa_info = Fusa_info_ptr;

  for (ProcessorNumber = 0; ProcessorNumber < pFusa_info->TotalProcessor; ProcessorNumber++) {
    if( pFusa_info->Processor[ProcessorNumber].CoreType == CPUID_CORE_TYPE_INTEL_ATOM ) {
        pFusa_info->BspCpu =  ProcessorNumber;
        break;
    }
  }

  DEBUG ((DEBUG_INFO, "Switching over BSP to =  %x\n", pFusa_info->BspCpu));
  Status = mMpServices2Ppi->SwitchBSP (
                        mMpServices2Ppi,
                        pFusa_info->BspCpu,
                        TRUE
                        );

  Status = mMpServices2Ppi->WhoAmI (
                        mMpServices2Ppi,
                        &pFusa_info->BspCpu
                        );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "ERROR who am I! Status: %x\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "Main core's CpuNumber =  %x\n", pFusa_info->BspCpu));
  }

  return Status;
}
#endif

/**
  Switch to ATOM CPU as BSP

  @return error status if CpuNumPopulate was not success
**/
STATIC
EFI_STATUS
CpuNumPopulate (
    IN FUSA_INFO *Fusa_info_ptr
  )
{
  UINT32 ProcessorNumber;
  FUSA_INFO *pFusa_info;
  pFusa_info = Fusa_info_ptr;

  pFusa_info->TotalCoreProcessor = 0;
  pFusa_info->TotalAtomProcessor = 0;

  for (ProcessorNumber = 0; ProcessorNumber < pFusa_info->TotalProcessor; ProcessorNumber++) {
    if( pFusa_info->Processor[ProcessorNumber].CoreType == CPUID_CORE_TYPE_INTEL_CORE ) {
      pFusa_info->TotalCoreProcessor++;
    } else {
      pFusa_info->TotalAtomProcessor++;
    }
  }

  return EFI_SUCCESS;
}

/**
  Find the core Number with Thread 0 to calculate the IPIndex.

  @return error status if CpuIDtoCoreNumber was not success
**/
EFI_STATUS
CpuIDtoCoreNumber (
    IN UINT32 ProcessorId,
    OUT UINT8 *CoreIndex
  )
{
  UINT8 ProcessorNumber;
  UINT8 ProcessorCount = 0;

  for (ProcessorNumber = 0; ProcessorNumber < Fusa_info.TotalProcessor; ProcessorNumber++) {
    if( Fusa_info.Processor[ProcessorNumber].ProcessorInfo.Location.Thread == 0 ) {
      if( Fusa_info.Processor[ProcessorNumber].ProcessorInfo.ProcessorId == ProcessorId ) {
        *CoreIndex = ProcessorCount;
        return EFI_SUCCESS;
      }
      ProcessorCount++;
    }
  }
  return EFI_INVALID_PARAMETER;
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
  UINT32 ProcessorNumber;
  EFI_STATUS Status;
  CPUID_NATIVE_MODEL_ID_AND_CORE_TYPE_EAX Eax;

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
                      &Fusa_info.TotalProcessor,
                      &Fusa_info.TotalEnProcessor
                      );


    Status = mMpServices2Ppi->WhoAmI (
                      mMpServices2Ppi,
                      &Fusa_info.BspCpu
                      );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "ERROR who am I! Status: %x\n", Status));
    } else {
      DEBUG ((DEBUG_INFO, "Main core's CpuNumber =  %x\n", Fusa_info.BspCpu));
    }

    for (ProcessorNumber = 0; ProcessorNumber < Fusa_info.TotalProcessor; ProcessorNumber++) {
      Status = mMpServices2Ppi->GetProcessorInfo (
                                 mMpServices2Ppi,
                                 ProcessorNumber,
                                 &(Fusa_info.Processor[ProcessorNumber].ProcessorInfo)
                                 );

      ASSERT_EFI_ERROR (Status);

      RunAtAp (
              (AP_PROCEDURE) FspCPUTypeCheckInternal,
              ProcessorNumber,
              &(Fusa_info.Processor[ProcessorNumber]));

    }
    // Probe BspCPU
    AsmCpuid (CPUID_HYBRID_INFORMATION, &Eax.Uint32, NULL, NULL, NULL);
    Fusa_info.Processor[Fusa_info.BspCpu].CoreType = Eax.Bits.CoreType;
    Fusa_info.Processor[Fusa_info.BspCpu].Fusa_Capabilities.Uint64 = AsmReadMsr64 (MSR_INTEGRITY_CAPABILITIES);

    GetProcessorLocationByApicId (
      (UINT32) Fusa_info.Processor[Fusa_info.BspCpu].ProcessorInfo.ProcessorId,
      &(Fusa_info.Processor[Fusa_info.BspCpu].ProcessorInfo.Location.Package),
      &(Fusa_info.Processor[Fusa_info.BspCpu].ProcessorInfo.Location.Core),
      &(Fusa_info.Processor[Fusa_info.BspCpu].ProcessorInfo.Location.Thread)
      );

    GetProcessorLocation2ByApicId (
      (UINT32) Fusa_info.Processor[Fusa_info.BspCpu].ProcessorInfo.ProcessorId,
      &(Fusa_info.Processor[Fusa_info.BspCpu].ProcessorInfo.ExtendedInformation.Location2.Package),
      &(Fusa_info.Processor[Fusa_info.BspCpu].ProcessorInfo.ExtendedInformation.Location2.Die),
      &(Fusa_info.Processor[Fusa_info.BspCpu].ProcessorInfo.ExtendedInformation.Location2.Tile),
      &(Fusa_info.Processor[Fusa_info.BspCpu].ProcessorInfo.ExtendedInformation.Location2.Module),
      &(Fusa_info.Processor[Fusa_info.BspCpu].ProcessorInfo.ExtendedInformation.Location2.Core),
      &(Fusa_info.Processor[Fusa_info.BspCpu].ProcessorInfo.ExtendedInformation.Location2.Thread)
      );
  }

  CpuNumPopulate(&Fusa_info);
  PrintFusa_info(&Fusa_info);

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
  UINTN   PwrmBaseAddress = PmcGetPwrmBase ();

  if (!IsFusaSupported()) {
    return FALSE;
  }

  FusaStsCtl = MmioRead32 ((UINTN) PwrmBaseAddress + R_PMC_PWRM_FUSA_STS_CTL);
  DEBUG ((DEBUG_INFO, "Fusa DIAGTEST_FEATURE_EN: %d\nDIAGTEST_EN: %d\nDIAGTEST_PCHMODE: %d\n",
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

  DEBUG ((DEBUG_INFO, "Fusa Diagnostic Mode detected is %d\n", FusaDiagTestMode));

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

  if (!IsFusaSupported()) {
    return EFI_SUCCESS;
  }

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
  IN  CONST EFI_PEI_SERVICES  **PeiServices,
  IN  UINT32 FusaStartupFileAddr
  )
{
  EFI_HOB_GUID_TYPE                  *GuidHob;
  FUSA_INFO_HOB                      *FusaInfo;
  FUSA_TEST_RESULT                   *TestResult;
  EFI_STATUS                         Status;

  if (!IsFusaSupported()) {
    return;
  }

  GuidHob = NULL;
  FusaInfo = NULL;

  Crc32cInit();
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
      Status = EFI_SUCCESS;
    }

    if (EFI_SUCCESS == Status) {
      FspDxCheckIbeccEccInj(&(TestResult[FusaTestNumIbecc0EccCorrError]));
      FspDxCheckIbeccParity(&(TestResult[FusaTestNumIbecc0Cmi]));
      FspDxCheckScanBist (FusaStartupFileAddr, (&(TestResult[FusaTestNumScan0Bist])));
      FspDxCheckArrayAndCRCBist ((&(TestResult[FusaTestNumArray0Bist])), (&(TestResult[FusaTestNumCRCon0PBISTROM])));
      FspDxCheckCboSliceEgressLlcDoubleParityCtC(&(TestResult[FusaTestNumCboSlice0EgressLlcDoubleParityCtC]));
      FspDxCheckCcfE2eIdiParityCtc(&(TestResult[FusaTestNumCCFE2EIDIParityCtC0]));
      FspDxCheckMemSubsystem (&(TestResult[FusaTestNumMc0Cmi]) );
      FspDxCheckMemoryMbist(&(TestResult[FusaTestNumMc0Mbist]));
      FspDxCheckHbo0E2eCfiParityCtc(&(TestResult[FusaTestNumHbo0E2eCfiParityCtc]));
      FspDxCheckHbo1E2eCfiParityCtc(&(TestResult[FusaTestNumHbo1E2eCfiParityCtc]));
      FspDxCheckSNCUE2eCfiParityCtc(&(TestResult[FusaTestNumSNCUE2eCfiParityCtc]));
      FspDxCheckSVTUE2eCfiParityCtc(&(TestResult[FusaTestNumSVTUE2eCfiParityCtc]));
      FspDxCheckPunitE2eCfiParityCtc(&(TestResult[FusaTestNumPunitE2eCfiParityCtc]));
      FspDxCheckIdiBE2eIdiParityCtc(&(TestResult[FusaTestNumIDIBE2EIDIParityCtC]));
      FspDxCheckIPU_PB_PIDE2eCfiParityCtc(&(TestResult[FusaTestNumIPU_PB_PIDE2eCfiParityCtc]));
      FspDxCheckVPU_PB_PIDE2eCfiParityCtc(&(TestResult[FusaTestNumVPU_PB_PIDE2eCfiParityCtc]));
      FspDxCheckMEDIA_PBE2eCfiParityCtc(&(TestResult[FusaTestNumMEDIA_PBE2eCfiParityCtc]));
      FspDxCheckGT_PBE2eCfiParityCtc(&(TestResult[FusaTestNumGT_PBE2eCfiParityCtc]));
      FspDxCheckIAX_PBE2eCfiParityCtc(&(TestResult[FusaTestNumIAX_PBE2eCfiParityCtc]));
      FspDxCheckDNI2CFIE2eCfiParityCtc(&(TestResult[FusaTestNumDNI2CFIE2eCfiParityCtc]));
      FspDxCheckCCE0E2eCfiParityCtc(&(TestResult[FusaTestNumCCE0E2eCfiParityCtc]));
      FspDxCheckCCE1E2eCfiParityCtc(&(TestResult[FusaTestNumCCE1E2eCfiParityCtc]));
      FspDxCheckCCF_SANTA1E2eCfiParityCtc(&(TestResult[FusaTestNumCCF_SANTA1E2EParityCtc]));
      FspDxCheckCCF_SANTA0E2eCfiParityCtc(&(TestResult[FusaTestNumCCF_SANTA0E2EParityCtc]));
      FspDxCheckIOCE2eCfiParityCtc(&(TestResult[FusaTestNumIOCE2eCfiParityCtc]));
      FspDxCheckIOCCEE2eCfiParityCtc(&(TestResult[FusaTestNumIOCCEE2eCfiParityCtc]));
      FspDxCheckIVTUE2eCfiParityCtc(&(TestResult[FusaTestNumIVTUE2eCfiParityCtc]));
      FspDxCheckIdiBE2eCfiParityCtc(&(TestResult[FusaTestNumIDIBE2ECfiParityCtC]));
      //FspDxCheckCoreCacheArrayStartupBist(&(TestResult[FusaTestNumCoreCacheArrayStartupBistCore0]));
      FspDxCheckCoreRomCrcStartupBist(&(TestResult[FusaTestNumCoreRomCrcStartupBistCore0]));
      FspDxCheckCoreIdiParityCtc(&(TestResult[FusaTestNumCoreIdiParityCtcCore0]));
      FspDxCheckCCFE2eNCUParityCtc(&(TestResult[FusaTestNumCCFE2ECNCUParityCtC0]));
      FspDxCheckIDIBE2eNCUParityCtc(&(TestResult[FusaTestNumIDIBE2ECNCUParityCtC0]));
    }
  } else {
    ASSERT (FALSE); //expect the existence of the HOB
  }
  FreePages( gpTestData, EFI_SIZE_TO_PAGES (1028 * 1024));
}

VOID
FspDxCheckEndofPei (
  IN  CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_HOB_GUID_TYPE                  *GuidHob;
  FUSA_INFO_HOB                      *FusaInfo;
  FUSA_TEST_RESULT                   *TestResult;
  EFI_STATUS                         Status;

  if (!IsFusaSupported()) {
    return;
  }

  GuidHob = NULL;
  FusaInfo = NULL;

  Crc32cInit();
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
      Status = EFI_SUCCESS;
    }

    if (EFI_SUCCESS == Status) {
      FspDxCheckGtL3EccCtc(&(TestResult[FusaTestNumGtL3EccCtc]));
    }
  } else {
    ASSERT (FALSE); //expect the existence of the HOB
  }
  FreePages( gpTestData, EFI_SIZE_TO_PAGES (1028 * 1024));
}