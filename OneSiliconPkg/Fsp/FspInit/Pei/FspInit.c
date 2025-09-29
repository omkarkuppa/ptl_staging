/** @file
  Source file for FSP Init PEI module

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include "FspInit.h"
#include <Library/PcdLib.h>
#include <Ppi/SecPlatformInformation.h>
#include <Ppi/PeiSiDefaultPolicy.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Register/GenerationMsr.h>
#include <Library/FspSwitchStackLib.h>
#include <Ppi/IGpuPlatformPolicyPpi.h>
#include <Library/PlatformFspMultiPhaseLib.h>

extern EDKII_PEI_MP_SERVICES2_PPI            mMpServices2WrapperPpi;

EDKII_PEI_MP_SERVICES2_PPI     *mCpuMp2Ppi = NULL;
FSP_GLOBAL_DATA               *mFspGlobalData = NULL;

EFI_STATUS
EFIAPI
FspInitOnPciEnumerationComplete (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

EFI_STATUS
EFIAPI
FspApiModePolicyInitUpdateDone (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

EFI_SEC_PLATFORM_INFORMATION2_PPI mFspSecPlatformInformation2 = {
  FspSecPlatformInformation2
};

EFI_PEI_PPI_DESCRIPTOR mFspPeiSecPlatformInformation2 = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiSecPlatformInformation2PpiGuid,
  &mFspSecPlatformInformation2
};

STATIC
EFI_PEI_NOTIFY_DESCRIPTOR  mOnPciEnumerationCompleteNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK  | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPciEnumerationCompleteProtocolGuid,
    FspInitOnPciEnumerationComplete
  }
};

EFI_PEI_PPI_DESCRIPTOR      mS3SmmInitDonePpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiS3SmmInitDoneGuid,
  NULL
};

EFI_PEI_NOTIFY_DESCRIPTOR  mOnDefaultPolicyInitNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK  | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gSiDefaultPolicyInitPpiGuid,
    FspApiModePolicyInitUpdateDone
  }
};

EFI_STATUS
EFIAPI
GetVbtData (
  OUT EFI_PHYSICAL_ADDRESS *VbtAddress,
  OUT UINT32               *VbtSize
)
{
  IGPU_DATA_HOB         *IGpuDataHob;

  IGpuDataHob = (IGPU_DATA_HOB *) GetFirstGuidHob (&gIGpuDataHobGuid);

  if (IGpuDataHob != NULL) {
    *VbtAddress = IGpuDataHob->GraphicsConfigPtr;
    *VbtSize    = IGpuDataHob->VbtSize;
  } else {
    DEBUG ((EFI_D_ERROR, "IGpu Data Hob not found\n"));
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetPeiPlatformLidStatus (
  OUT LID_STATUS  *CurrentLidStatus
)
{
  IGPU_DATA_HOB                *IGpuDataHob;

  IGpuDataHob = (IGPU_DATA_HOB *) GetFirstGuidHob (&gIGpuDataHobGuid);
  if (IGpuDataHob != NULL) {
    *CurrentLidStatus = IGpuDataHob->LidStatus;
  } else {
    DEBUG ((EFI_D_ERROR, "IGpu Data Hob not found\n"));
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetMaxActiveDisplays (
  OUT UINT8  *MaxActiveDisplays
  )
{
  FSPS_UPD  *FspsUpd;

  *MaxActiveDisplays = 0;

  FspsUpd = GetFspSiliconInitUpdDataPointer ();
  if (FspsUpd != NULL) {
    if (FspsUpd->FspsConfig.MaxActiveDisplays > 2) {
      DEBUG ((DEBUG_ERROR, "Invalid MaxActiveDisplays value in FSPS UPD. Supported values are 0-2\n"));
      DEBUG ((DEBUG_ERROR, "Resetting to default value 0\n"));
      return EFI_INVALID_PARAMETER;
    }
    *MaxActiveDisplays = FspsUpd->FspsConfig.MaxActiveDisplays;
  } else {
    DEBUG ((EFI_D_ERROR, "FspsUpd Data not found\n"));
    return EFI_NOT_FOUND;
  }
  DEBUG ((DEBUG_INFO, "MaxActiveDisplays = %d\n", *MaxActiveDisplays));
  return EFI_SUCCESS;
}

PEI_IGPU_PLATFORM_POLICY_PPI PeiIGpuPlatform = {
  PEI_IGPU_PLATFORM_POLICY_REVISION,
  GetPeiPlatformLidStatus,
  GetVbtData,
  GetMaxActiveDisplays
};

EFI_PEI_PPI_DESCRIPTOR  mPeiIGpuPpiDescriptor = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiGraphicsPlatformPpiGuid,
  &PeiIGpuPlatform
};

/**
  This routine is used to get Sec Platform Information Record Pointer.

  @param[in] PeiServices    Pointer to the PEI services table

  @retval GetSecPlatformInformation2 - The pointer of Sec Platform Information Record Pointer.
**/
EFI_SEC_PLATFORM_INFORMATION_RECORD2 *
GetSecPlatformInformationInfoInFormat2 (
  IN EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_STATUS                           Status;
  UINT64                               InformationSize;
  EFI_SEC_PLATFORM_INFORMATION_PPI     *SecPlatformInformationPpi;
  EFI_SEC_PLATFORM_INFORMATION_RECORD  *SecPlatformInformation = NULL;
  EFI_SEC_PLATFORM_INFORMATION_RECORD2 *SecPlatformInformation2;

  //
  // Get BIST information from Sec Platform Information
  //
  Status = PeiServicesLocatePpi (
             &gEfiSecPlatformInformationPpiGuid,    // GUID
             0,                                     // Instance
             NULL,                                  // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &SecPlatformInformationPpi   // PPI
             );

  DEBUG ((DEBUG_INFO, "LocatePpi SecPlatformInformationPpi Status - %r\n", Status));
  if (EFI_ERROR(Status)) {
    return NULL;
  }

  InformationSize = 0;
  Status = SecPlatformInformationPpi->PlatformInformation (
                                        (CONST EFI_PEI_SERVICES **) PeiServices,
                                        &InformationSize,
                                        SecPlatformInformation
                                        );

  ASSERT (Status == EFI_BUFFER_TOO_SMALL);
  if (Status != EFI_BUFFER_TOO_SMALL) {
    return NULL;
  }

  SecPlatformInformation = AllocatePool((UINTN)InformationSize);
  ASSERT (SecPlatformInformation != NULL);
  if (SecPlatformInformation == NULL) {
    return NULL;
  }

  //
  // Retrieve BIST data from SecPlatform
  //
  Status = SecPlatformInformationPpi->PlatformInformation (
                                        (CONST EFI_PEI_SERVICES**) PeiServices,
                                        &InformationSize,
                                        SecPlatformInformation
                                        );
  DEBUG ((DEBUG_INFO, "FSP  SecPlatformInformation2Ppi->PlatformInformation Status - %r\n", Status));
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  SecPlatformInformation2 = AllocatePool (sizeof (EFI_SEC_PLATFORM_INFORMATION_RECORD2));
  ASSERT (SecPlatformInformation2 != NULL);
  if (SecPlatformInformation2 == NULL) {
    return NULL;
  }

  SecPlatformInformation2->NumberOfCpus = 1;
  SecPlatformInformation2->CpuInstance[0].CpuLocation = 0;
  SecPlatformInformation2->CpuInstance[0].InfoRecord.x64HealthFlags.Uint32 = SecPlatformInformation->x64HealthFlags.Uint32;

  FreePool(SecPlatformInformation);

  return SecPlatformInformation2;
}

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4090)
#endif
/**
  Implementation of the FspPlatformInformation2 service in EFI_SEC_PLATFORM_INFORMATION2_PPI.

  @param  PeiServices                The pointer to the PEI Services Table.
  @param  StructureSize              The pointer to the variable describing size of the input buffer.
  @param  PlatformInformationRecord2 The pointer to the EFI_SEC_PLATFORM_INFORMATION_RECORD2.

  @retval EFI_SUCCESS                The data was successfully returned.
  @retval EFI_BUFFER_TOO_SMALL       The buffer was too small. The current buffer size needed to
                                     hold the record is returned in StructureSize.

**/
EFI_STATUS
EFIAPI
FspSecPlatformInformation2 (
  IN CONST EFI_PEI_SERVICES                   **PeiServices,
  IN OUT UINT64                               *StructureSize,
     OUT EFI_SEC_PLATFORM_INFORMATION_RECORD2 *PlatformInformationRecord2
  )
{
  if (GetFspGlobalDataPointer()->FspMode == FSP_IN_API_MODE) { // FSP_MODE_CHECK
    FSP_S_CONFIG                         *FspsConfigUpd;
    UINTN                                InformationDataSize;
    EFI_SEC_PLATFORM_INFORMATION_RECORD2 *PlatformInformationRecord2temp;

    FspsConfigUpd = &((FSPS_UPD *) GetFspSiliconInitUpdDataPointer())->FspsConfig;
    PlatformInformationRecord2temp = (EFI_SEC_PLATFORM_INFORMATION_RECORD2 *)(UINTN) FspsConfigUpd->CpuBistData;
    InformationDataSize = sizeof (EFI_SEC_PLATFORM_INFORMATION_RECORD2) + sizeof (EFI_SEC_PLATFORM_INFORMATION_CPU) * (PlatformInformationRecord2temp->NumberOfCpus - 1);
    if (*StructureSize < InformationDataSize) {
      *StructureSize = InformationDataSize;
      return EFI_BUFFER_TOO_SMALL;
    }

    CopyMem((EFI_SEC_PLATFORM_INFORMATION_RECORD2 *)(UINTN) PlatformInformationRecord2, (VOID*)(UINTN) FspsConfigUpd->CpuBistData, InformationDataSize);

    DEBUG_CODE_BEGIN ();
      UINTN                           Index;
      DEBUG ((DEBUG_INFO, "BIST NumberOfCpus - %x\n", PlatformInformationRecord2->NumberOfCpus));
      for (Index = 0; Index < PlatformInformationRecord2->NumberOfCpus; Index++) {
        DEBUG ((DEBUG_INFO, "BIST IA32HealthFlags Instance(%x)- %x CpuLocation - %x\n", Index, PlatformInformationRecord2->CpuInstance[Index].InfoRecord.x64HealthFlags.Uint32, PlatformInformationRecord2->CpuInstance[Index].CpuLocation));
      }
    DEBUG_CODE_END ();
  }
  return EFI_SUCCESS;
}

#ifdef _MSC_VER
#pragma warning (pop)
#endif

/**
  This function gets registered as a callback

  @param[in]  PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in]  NotifyDescriptor  Address of the notification descriptor data structure.
  @param[in]  Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
**/
EFI_STATUS
EFIAPI
FspInitOnPciEnumerationComplete (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
{

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP S3 SMM INIT DONE ...\n"));
  PeiServicesInstallPpi (&mS3SmmInitDonePpi);

  return EFI_SUCCESS;
}

/**
  Update SiPolicy from UPD in FSP API mode.

  It's get called when gSiDefaultPolicyInitPpiGuid PPI is installed.

  @param[in]  PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in]  NotifyDescriptor  Address of the notification descriptor data structure.
  @param[in]  Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
**/
EFI_STATUS
EFIAPI
FspApiModePolicyInitUpdateDone (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
{
  EFI_STATUS                     Status;
  FSPS_UPD                       *FspsUpd;
  SI_POLICY_PPI                  *SiPolicyPpi;
  PEI_SI_DEFAULT_POLICY_INIT_PPI *PeiSiDefaultPolicyInitPpi;
  SI_POLICY_PPI                  *PchPolicyPpi;

  DEBUG ((DEBUG_INFO, "Updating SiPolicy according to FSPS_UPD...\n"));
  FspsUpd = GetFspSiliconInitUpdDataPointer ();

  PeiSiDefaultPolicyInitPpi = NULL;
  PchPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (
            &gPchDefaultPolicyInitPpiGuid,
            0,
            NULL,
            (VOID **) &PeiSiDefaultPolicyInitPpi
            );
  if (PeiSiDefaultPolicyInitPpi != NULL) {
    Status = PeiSiDefaultPolicyInitPpi->PeiPolicyInit ();
    ASSERT_EFI_ERROR (Status);
    if (Status == EFI_SUCCESS) {
      Status = PeiServicesLocatePpi (
                &gPchPolicyPpiGuid,
                0,
                NULL,
                (VOID **) &PchPolicyPpi
                );
      ASSERT_EFI_ERROR (Status);
      if ((Status == EFI_SUCCESS) && (PchPolicyPpi != NULL)) {
        FspUpdatePeiAttachedPchPolicy (PchPolicyPpi, FspsUpd);
      }
    }
  }

  //
  // Locate Policy init PPI to install default silicon policy
  //
  PeiSiDefaultPolicyInitPpi = NULL;
  Status = PeiServicesLocatePpi (
              &gSiDefaultPolicyInitPpiGuid,
              0,
              NULL,
              (VOID **) &PeiSiDefaultPolicyInitPpi
              );
  ASSERT_EFI_ERROR (Status);
  if (PeiSiDefaultPolicyInitPpi != NULL) {
    Status = PeiSiDefaultPolicyInitPpi->PeiPolicyInit ();
    ASSERT_EFI_ERROR (Status);
    if (Status == EFI_SUCCESS) {
      Status = PeiServicesLocatePpi (
                  &gSiPolicyPpiGuid,
                  0,
                  NULL,
                  (VOID **) &SiPolicyPpi
                  );
      ASSERT_EFI_ERROR (Status);

      if ((Status == EFI_SUCCESS) && (SiPolicyPpi != NULL)) {
        FspUpdatePeiPchPolicy (SiPolicyPpi, FspsUpd);
        FspUpdatePeiCpuPolicy (SiPolicyPpi, FspsUpd);
        //
        // Update Security Policy before install CpuPolicyPpi
        //
        FspUpdatePeiSecurityPolicy (SiPolicyPpi, FspsUpd);
        FspUpdatePeiMePolicy (SiPolicyPpi, FspsUpd);
        FspUpdatePeiSaPolicy (SiPolicyPpi, FspsUpd);
#if FixedPcdGetBool(PcdAmtEnable) == 1
        FspUpdatePeiAmtPolicy (SiPolicyPpi, FspsUpd);
#endif
        FspUpdatePeiTbtPolicy (SiPolicyPpi, FspsUpd);
        FspUpdatePeiSiPolicy (SiPolicyPpi, FspsUpd);
#if FixedPcdGetBool (PcdFspVEnable) == 1
        FspUpdatePeiFspVPolicy (SiPolicyPpi, FspsUpd);
#endif
      }
    }

  }

  //
  // In FSP API mode, no policy update from boot loader so FSP should install
  // PolicyReady PPI to trigger silicon initialization OnPolicy callbacks.
  //
  Status = SiInstallPolicyReadyPpi ();
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "FspSiliconPolicyInit done\n"));
  return EFI_SUCCESS;
}

/**
  FSP Init PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
FspInitEntryPoint (
  IN       EFI_PEI_FILE_HANDLE    FileHandle,
  IN CONST EFI_PEI_SERVICES       **PeiServices
  )
{
  EFI_STATUS                     Status;
  FSPS_UPD                       *FspsUpd;
  FSP_INFO_HEADER                *FspInfoHeader;
  EFI_PEI_PPI_DESCRIPTOR         *PpiDescriptor;
  EFI_BOOT_MODE                  BootMode;
  MSR_CORE_THREAD_COUNT_REGISTER MsrCoreThreadCount;
  IGPU_DATA_HOB                  *IGpuDataHob;

  //
  // Silicon code will produce Default Policy Init PPI in PrePolicy phase,
  // different owners will consume this PPI to create policy.
  // -----------------------------------------------------------------------
  // |Task                               |Owner for API |Owner for Dispatch|
  // -----------------------------------------------------------------------
  // |Consume Default Policy Init PPI to | FSP          | Boot loader      |
  // |create default policy.             |              |                  |
  // -----------------------------------------------------------------------
  // |Update policy as needed.           | FSP by UPD   | Boot loader      |
  // -----------------------------------------------------------------------
  // |Install PolicyReady PPI to start   | FSP          | Boot loader      |
  // |silicon OnPolicy callbacks.        |              |                  |
  // -----------------------------------------------------------------------
  //
  if (GetFspGlobalDataPointer()->FspMode == FSP_IN_API_MODE) { // FSP_MODE_CHECK

    FspInfoHeader = GetFspInfoHeaderFromApiContext ();
    SetFspInfoHeader (FspInfoHeader);

    FspsUpd = (FSPS_UPD *) GetFspApiParameter ();
    if (FspsUpd == NULL) {
      //
      // Use the UpdRegion as default
      //
      FspsUpd = (FSPS_UPD *)(UINTN) (FspInfoHeader->ImageBase + FspInfoHeader->CfgRegionOffset);
    }
    SetFspUpdDataPointer (FspsUpd);
    SetFspSiliconInitUpdDataPointer (FspsUpd);

    DEBUG_CODE_BEGIN ();
      UINT32  Index;
      DEBUG ((DEBUG_INFO, "Dumping FSPS_UPD - Size: 0x%8X", sizeof(FSPS_UPD)));
      for (Index = 0; Index < sizeof (FSPS_UPD); ++Index) {
        if (Index % 0x10 == 0) {
          DEBUG ((DEBUG_INFO, "\n0x%8X:", Index));
        }
        DEBUG ((DEBUG_INFO, " 0x%02X", *(((UINT8 *)FspsUpd) + Index)));
      }
      DEBUG ((DEBUG_INFO, "\n"));
    DEBUG_CODE_END ();

    ///
    /// Set PcdCpuMaxLogicalProcessorNumber to max number of logical processors enabled
    /// Read MSR_CORE_THREAD_COUNT (0x35) to check the total active Threads
    ///
    MsrCoreThreadCount.Uint64 = AsmReadMsr64 (MSR_CORE_THREAD_COUNT);
    DEBUG ((DEBUG_INFO, "MaxLogicProcessors = %d\n", MsrCoreThreadCount.Bits.Threadcount));
    PcdSet32S (PcdCpuMaxLogicalProcessorNumber, (UINT32) MsrCoreThreadCount.Bits.Threadcount);

    IGpuDataHob = (IGPU_DATA_HOB *) GetFirstGuidHob (&gIGpuDataHobGuid);
    if (IGpuDataHob != NULL) {
      DEBUG ((DEBUG_INFO, "Update LidStatus, VbtData to Hob\n"));
      IGpuDataHob->LidStatus         = FspsUpd->FspsConfig.LidStatus;
      IGpuDataHob->GraphicsConfigPtr = (UINT32) FspsUpd->FspsConfig.GraphicsConfigPtr;
      IGpuDataHob->VbtSize           = FspsUpd->FspsConfig.VbtSize;
    }

    //
    // For FSP API mode, Install PEI_IGPU_PLATFORM_POLICY_PPI.
    //
    Status = PeiServicesInstallPpi (&mPeiIGpuPpiDescriptor);
    ASSERT_EFI_ERROR (Status);

    //
    // Install an instance of SecPlatformInformation2 with BIST data
    //
    if (FspsUpd->FspsConfig.CpuBistData) {
      Status = PeiServicesInstallPpi (&mFspPeiSecPlatformInformation2);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
      }
    }

    //
    // Wrapper can use CpuMpPpi to transfer PPI to FSP, we use it to install MP service. (optional)
    // Otherwise, CpuMpPei will be dispatched.
    //
    if (FspsUpd->FspsConfig.CpuMpPpi) {
      mCpuMp2Ppi = (VOID *)(UINTN) FspsUpd->FspsConfig.CpuMpPpi;
      mFspGlobalData = GetFspGlobalDataPointer();

      PpiDescriptor = AllocatePool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
      if (PpiDescriptor != NULL) {
        PpiDescriptor->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
        PpiDescriptor->Ppi   = (VOID *) &mMpServices2WrapperPpi;
        PpiDescriptor->Guid  = &gEdkiiPeiMpServices2PpiGuid;
        Status = PeiServicesInstallPpi (PpiDescriptor);
        if (EFI_ERROR (Status)) {
          ASSERT_EFI_ERROR (Status);
        }
      }
    }

    Status = PeiServicesGetBootMode (&BootMode);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
    }

    if (BootMode == BOOT_ON_S3_RESUME) {
      Status = PeiServicesNotifyPpi (mOnPciEnumerationCompleteNotifyList);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
      }
    }

    Status = FspMultiPhaseSiInitPlatformWorker ();
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
    }

    Status = PeiServicesNotifyPpi (mOnDefaultPolicyInitNotifyList);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
    }
  }

  //
  // Installs the Report Status Code PPI
  //
  Status = InstallMonoStatusCode (FileHandle, PeiServices);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((DEBUG_INFO, "SI PEI Policy Initialization Done in Post-Memory\n"));

  return Status;
}
