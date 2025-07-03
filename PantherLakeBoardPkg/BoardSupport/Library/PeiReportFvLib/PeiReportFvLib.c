/** @file

    Source code file for the Report Firmware Volume (FV) library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include <Base.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ReportFvLib.h>
#include <Guid/FirmwareFileSystem2.h>
#include <Ppi/FirmwareVolumeInfo.h>
#include <Library/PeiMeLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/MemoryAllocationLib.h>
#include <IndustryStandard/Pci30.h>
#include <IndustryStandard/Tpm20.h>
#include "FitAccess.h"
#include <Library/BootGuardLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <SetupVariable.h>
#include <Setup.h>
#include <Register/HeciRegs.h>
#include <Library/PciSegmentLib.h>
#include <Library/MeInfoLib.h>
#include <Library/ResiliencySupportLib.h>

#ifndef MAX_NUMBER_OF_OBB_FIRMWARE_VOLUMES
  #define MAX_NUMBER_OF_OBB_FIRMWARE_VOLUMES 8
#endif

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_PPI_DESCRIPTOR mPpiListBeforePostMemFvReport = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiBeforePostMemFvReportPpiGuid,
  NULL
};

EFI_STATUS
EFIAPI
InstallFvExtendedPostMemoryCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

EFI_STATUS
EFIAPI
InstallFvExtendedAdvancedCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  Notify list for FV installation to the memory for Extended BIOS Region.
  Each entry of the notify list may need to be registered based on a boot path.
  Make sure what FV is installed by each callback and notify them per needs.
**/
STATIC EFI_PEI_NOTIFY_DESCRIPTOR mExtendedBiosDecodeReadyNotifyList [] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gExtendedBiosDecodeReadyPpiGuid,
    InstallFvExtendedPostMemoryCallback
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gExtendedBiosDecodeReadyPpiGuid,
    InstallFvExtendedAdvancedCallback
  },
};

/**
  Check current boot is during the capsule update.
  (In the middle of seamless recovery/resiliency restore.)

  @retval TRUE   Current boot is capsule update in progress.
  @retval FALSE  Current boot is not capsule update in progress.
**/
BOOLEAN
IsCapsuleUpdateRecoveryBoot (
  VOID
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;

  GuidHob = GetFirstGuidHob (&gBiosInfoRecoveryGuid);

  if (GuidHob == NULL) {
    return FALSE;
  } else {
    return TRUE;
  }
}

VOID
PrintFvHeaderInfo (
  EFI_FIRMWARE_VOLUME_HEADER  *FvHeader
  );

VOID
ReportPreMemFv (
  VOID
  )
{
  /*
    Note : FSP FVs except FSP-T FV are installed in IntelFsp2Wrapper Pkg or FspPkg in Dispatch mode.
  */
  if (FixedPcdGetBool (PcdFspWrapperBootMode)) {
    DEBUG ((DEBUG_INFO, "Install FlashFvFspT - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvFspTBase), PcdGet32 (PcdFlashFvFspTSize)));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PcdGet32 (PcdFlashFvFspTBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvFspTBase),
      PcdGet32 (PcdFlashFvFspTSize),
      NULL,
      NULL,
      0
      );
  }

  //
  // When FSP Reset is supported, FSP-O FV should be installed
  //
  if (FixedPcdGetBool (PcdFspWrapperResetVectorInFsp)) {
    DEBUG ((DEBUG_INFO, "Install FlashFvFspO - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvFspOBase), PcdGet32 (PcdFlashFvFspOSize)));
    PeiServicesInstallFvInfo2Ppi (
      &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PcdGet32 (PcdFlashFvFspOBase))->FileSystemGuid),
      (VOID *) (UINTN) PcdGet32 (PcdFlashFvFspOBase),
      PcdGet32 (PcdFlashFvFspOSize),
      NULL,
      NULL,
      0
      );
  }

  //
  //
#ifndef SOC_CORE_BOOT
  DEBUG ((DEBUG_INFO, "Install FlashFvFirmwareBinaries - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvFirmwareBinariesBase), PcdGet32 (PcdFlashFvFirmwareBinariesSize)));
  PeiServicesInstallFvInfo2Ppi (
    &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PcdGet32 (PcdFlashFvFirmwareBinariesBase))->FileSystemGuid),
    (VOID *) (UINTN) PcdGet32 (PcdFlashFvFirmwareBinariesBase),
    PcdGet32 (PcdFlashFvFirmwareBinariesSize),
    NULL,
    NULL,
    0
    );
#endif
}

/**
  This functions determines if the Hashed FVs are intended for Verified Boot, Measured or Both.

  @param  None

  @retval EFI_SUCCESS           The applicable type of boot, whether Verified, Measured or Both.

**/
UINT64
GetBaseHashFvFlag (
  VOID
  )
{
  UINT64            MsrBootGuardSacmInfo;
  UINT64            Flag;

  MsrBootGuardSacmInfo = AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO);
  DEBUG ((EFI_D_INFO, "\tMSR_BOOT_GUARD_SACM_INFO[0x%08x] = 0x%016lx\n", MSR_BOOT_GUARD_SACM_INFO, MsrBootGuardSacmInfo));
  Flag = 0;

  // If BootGuard (BTG) is not supported by the CPU, OBB verification can still be enforced with the PCD, PcdObbVerificationEnforcementEnable
#if FixedPcdGet8(PcdObbVerificationEnforcementEnable) == 0x1
  Flag |= HASHED_FV_FLAG_VERIFIED_BOOT;
#endif
  if ((MsrBootGuardSacmInfo & B_BOOT_GUARD_SACM_INFO_VERIFIED_BOOT) != 0) {
    Flag |= HASHED_FV_FLAG_VERIFIED_BOOT;
  }
  if (FeaturePcdGet (PcdTpm2Enable) == TRUE) {
    Flag |= HASHED_FV_FLAG_MEASURED_BOOT;
  }
  // If BootGuard (BTG) is not supported and there is no Measured boot, skip all FVs in the OBB verification
  if (!Flag) {
    Flag |= HASHED_FV_FLAG_SKIP_ALL;
  }
  return Flag;
}

/**
  This functions installs a PPI containing all Hashed FVs along with their Hash information.

  @param  StoredHashFvPpi       A structure pointer to PPI where hashed FVs along with their Hash information are stored.
  @param  HashInfo              A structure pointer to the Hash of all the FVs Hashes
  @param  BootMode              Current boot mode.
  @param  FvFlag                FV Flag.

  @retval EFI_SUCCESS           PPI was successfully installed.
  @retval EFI_INVALID_PARAMETER The PpiList pointer is NULL.
  @retval EFI_INVALID_PARAMETER Any of the PEI PPI descriptors in the list do not have the
                                EFI_PEI_PPI_DESCRIPTOR_PPI bit set in the Flags field.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space in the PPI database.

**/
EFI_STATUS
InstallStoredHashFvPpi (
  IN EDKII_PEI_FIRMWARE_VOLUME_INFO_STORED_HASH_FV_PPI  *StoredHashFvPpi,
  IN HASH_STRUCTURE                                     *HashInfo,
  IN EFI_BOOT_MODE                                      BootMode,
  IN UINT64                                             FvFlag
  )
{
  EFI_STATUS              Status;
  EFI_PEI_PPI_DESCRIPTOR  *PpiDescriptor;
  Status                  = EFI_SUCCESS;
  PpiDescriptor           = NULL;

  if ((HashInfo != NULL) && (FvFlag !=0)) {
    StoredHashFvPpi->HashInfo.HashFlag   = FV_HASH_FLAG_BOOT_MODE (BootMode);
    StoredHashFvPpi->HashInfo.HashAlgoId = HashInfo->HashAlg;
    StoredHashFvPpi->HashInfo.HashSize   = HashInfo->Size;
    CopyMem (StoredHashFvPpi->HashInfo.Hash, HashInfo + 1, HashInfo->Size);
    //DEBUG_CODE
    UINT16 Index;
    DEBUG ((DEBUG_INFO, "\tStoredHashFvPpi[0x%08x] = 0x%08x\n", StoredHashFvPpi,*(UINT32 *)StoredHashFvPpi));
    DEBUG ((DEBUG_INFO, "\tHashInfo.HashFlag\t= 0x%016lx\n", StoredHashFvPpi->HashInfo.HashFlag));
    DEBUG ((DEBUG_INFO, "\tHashInfo.HashAlgoId= 0x%04x\n", StoredHashFvPpi->HashInfo.HashAlgoId));
    DEBUG ((DEBUG_INFO, "\tHashInfo.HashSize\t= 0x%04x\n", StoredHashFvPpi->HashInfo.HashSize));
    DEBUG ((DEBUG_INFO, "\tHashInfo.Hash\t\t\t=\n\t"));
    for (Index = 0; Index < StoredHashFvPpi->HashInfo.HashSize; Index++) {
      DEBUG ((DEBUG_INFO, "%02x", (UINT8)StoredHashFvPpi->HashInfo.Hash[Index]));
      if (Index == (StoredHashFvPpi->HashInfo.HashSize - 1)) {
        DEBUG ((DEBUG_INFO, "\n"));
      }
    }
    //DEBUG_CODE
  } else {
    StoredHashFvPpi->HashInfo.HashFlag   = 0;
    StoredHashFvPpi->HashInfo.HashAlgoId = TPM_ALG_NULL;
    StoredHashFvPpi->HashInfo.HashSize   = 0;
  }
  PpiDescriptor = AllocatePool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  ASSERT (PpiDescriptor != NULL);
  if (PpiDescriptor == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  PpiDescriptor->Guid  = &gEdkiiPeiFirmwareVolumeInfoStoredHashFvPpiGuid;
  PpiDescriptor->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PpiDescriptor->Ppi   = (VOID *) StoredHashFvPpi;
  Status = PeiServicesInstallPpi (PpiDescriptor);
  if (EFI_ERROR (Status)) {
    if (PpiDescriptor != NULL) {
      FreePool (PpiDescriptor);
      ASSERT_EFI_ERROR (Status);
    }
  }

  return Status;
}

/**
  This function creates Stored Hash Fv PPI Info containing all Hashed FVs along with their Hash information.

  @param  [in][out] StoredHashFvPpi       A structure pointer to PPI where hashed FVs along with their Hash information are stored.
  @param  [in]      FvFlag                Flag used to identify how the FV should be reported to the rest of the boot.
  @param  [in]      CapsuleRecoveryFlag   Flag used to identify how the FV should be reported to the rest of the boot.
  @param  [in]      BootMode              Current boot mode.

  @retval EFI_SUCCESS           PPI Information created properly.
  @retval EFI_INVALID_PARAMETER Detected invalid inputs.

**/
EFI_STATUS
CreateStoredHashFvPpiInfo (
  IN OUT EDKII_PEI_FIRMWARE_VOLUME_INFO_STORED_HASH_FV_PPI  *StoredHashFvPpi,
  IN     UINT64                                             FvFlag,
  IN     UINT64                                             CapsuleRecoveryFlag,
  IN     EFI_BOOT_MODE                                      BootMode
  )
{
  UINTN  FvNumber;

  FvNumber = 0;

  if (StoredHashFvPpi == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // FVs taken into account (or hashed) for the OBB verification, must match the ones used by the BpmGen2 tool in the BPM generation (bpmgen2_XXX.params)
  // The order of FVs get reported need to match the FV hash digest calculated. e.g. ObbDigetHash and PostIbbDigestHash
  //

#if FixedPcdGetBool(PcdSignedFspEnable) == 0
  if (FixedPcdGetBool (PcdFspWrapperBootMode)) {
    // FvFspS
    StoredHashFvPpi->FvInfo[FvNumber].Base    = FixedPcdGet32 (PcdFlashFvFspSBase);
    StoredHashFvPpi->FvInfo[FvNumber].Length  = (UINT64)((EFI_FIRMWARE_VOLUME_HEADER *)(UINTN) (PcdGet32 (PcdFlashFvFspSBase)))->FvLength;
    StoredHashFvPpi->FvInfo[FvNumber].Flag    = FvFlag;
    FvNumber++;
  }
#endif
  // FvPostMemory
  StoredHashFvPpi->FvInfo[FvNumber].Base    = FixedPcdGet32 (PcdFlashFvPostMemoryBase);
  StoredHashFvPpi->FvInfo[FvNumber].Length  = FixedPcdGet32 (PcdFlashFvPostMemorySize);
  StoredHashFvPpi->FvInfo[FvNumber].Flag    = FvFlag |
                                              HASHED_FV_FLAG_REPORT_FV_INFO_PPI;
  FvNumber++;
  // FvUefiBoot
  StoredHashFvPpi->FvInfo[FvNumber].Base    = FixedPcdGet32 (PcdFlashFvUefiBootBase);
  StoredHashFvPpi->FvInfo[FvNumber].Length  = FixedPcdGet32 (PcdFlashFvUefiBootSize);
  StoredHashFvPpi->FvInfo[FvNumber].Flag    = FvFlag |
                                              HASHED_FV_FLAG_REPORT_FV_INFO_PPI |
                                              HASHED_FV_FLAG_SKIP_BOOT_MODE(BOOT_ON_S3_RESUME) |
                                              CapsuleRecoveryFlag;
  FvNumber++;
  // FvUpl
  StoredHashFvPpi->FvInfo[FvNumber].Base    = FixedPcdGet32 (PcdFlashFvUplBase);
  StoredHashFvPpi->FvInfo[FvNumber].Length  = FixedPcdGet32 (PcdFlashFvUplSize);
  StoredHashFvPpi->FvInfo[FvNumber].Flag    = FvFlag |
                                              HASHED_FV_FLAG_REPORT_FV_INFO_PPI |
                                              CapsuleRecoveryFlag;
  FvNumber++;
  // FvOsBoot
  StoredHashFvPpi->FvInfo[FvNumber].Base    = FixedPcdGet32 (PcdFlashFvOsBootBase);
  StoredHashFvPpi->FvInfo[FvNumber].Length  = FixedPcdGet32 (PcdFlashFvOsBootSize);
  StoredHashFvPpi->FvInfo[FvNumber].Flag    = FvFlag |
                                              HASHED_FV_FLAG_REPORT_FV_INFO_PPI |
                                              HASHED_FV_FLAG_SKIP_BOOT_MODE(BOOT_ON_S3_RESUME) |
                                              CapsuleRecoveryFlag;
  FvNumber++;
  // FvAdvanced
  StoredHashFvPpi->FvInfo[FvNumber].Base    = FixedPcdGet32 (PcdFlashFvAdvancedBase);
  StoredHashFvPpi->FvInfo[FvNumber].Length  = FixedPcdGet32 (PcdFlashFvAdvancedSize);
  StoredHashFvPpi->FvInfo[FvNumber].Flag    = FvFlag |
                                              HASHED_FV_FLAG_REPORT_FV_INFO_PPI |
                                              HASHED_FV_FLAG_SKIP_BOOT_MODE(BOOT_ON_S3_RESUME) |
                                              CapsuleRecoveryFlag;
  FvNumber++;
  // FvSecurity
  StoredHashFvPpi->FvInfo[FvNumber].Base    = FixedPcdGet32 (PcdFlashFvSecurityBase);
  StoredHashFvPpi->FvInfo[FvNumber].Length  = FixedPcdGet32 (PcdFlashFvSecuritySize);
  StoredHashFvPpi->FvInfo[FvNumber].Flag    = FvFlag |
                                              HASHED_FV_FLAG_REPORT_FV_INFO_PPI |
                                              CapsuleRecoveryFlag;
  FvNumber++;
  // NonFitPayloads
  StoredHashFvPpi->FvInfo[FvNumber].Base    = FixedPcdGet32 (PcdFlashNonFitPayloadBase);
  StoredHashFvPpi->FvInfo[FvNumber].Length  = FixedPcdGet32 (PcdFlashNonFitPayloadSize);
  StoredHashFvPpi->FvInfo[FvNumber].Flag    = FvFlag |
                                              HASHED_FV_FLAG_REPORT_FV_INFO_PPI |
                                              HASHED_FV_FLAG_SKIP_BOOT_MODE(BOOT_ON_S3_RESUME) |
                                              CapsuleRecoveryFlag;
  FvNumber++;

  ASSERT (FvNumber <= MAX_NUMBER_OF_OBB_FIRMWARE_VOLUMES);

  StoredHashFvPpi->FvNumber = FvNumber;

  if (BootMode != BOOT_ON_S3_RESUME) {
    BuildFvHob (
      (UINTN)FixedPcdGet32 (PcdFlashFvMicrocodeBase),
      (UINTN)FixedPcdGet32 (PcdFlashFvMicrocodeSize)
      );
    DEBUG ((DEBUG_INFO, "Build FlashFvMicrocode Hob - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvMicrocodeBase), PcdGet32 (PcdFlashFvMicrocodeSize)));

    if (GetBiosResiliencyType () == SUPPORT_BIOS_RESILIENCY_RECOVERY) {
      BuildFvHob (
        (UINTN) FixedPcdGet32 (PcdFlashFvPreMemoryBase),
        (UINTN) FixedPcdGet32 (PcdFlashFvPreMemorySize)
        );
      DEBUG ((DEBUG_INFO, "Build PcdFlashFvPreMemory Hob - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvPreMemoryBase), PcdGet32 (PcdFlashFvPreMemorySize)));
    }

  }

  //
  // Build Fv FspS only for FSP DISPATCH mode
  //
#if FixedPcdGet8 (PcdFspModeSelection) == 0
  BuildFvHob (
    (UINTN) FixedPcdGet32 (PcdFlashFvFspSBase),
    (UINTN) FixedPcdGet32 (PcdFlashFvFspSSize)
    );
  DEBUG ((DEBUG_INFO, "Build PcdFlashFvFspS Hob - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvFspSBase), PcdGet32 (PcdFlashFvFspSSize)));
#endif

  //
  // Report resource HOB for flash FV
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_WRITE_PROTECTABLE),
      (UINTN) PcdGet32 (PcdFlashAreaBaseAddress),
    (UINTN) PcdGet32 (PcdFlashAreaSize)
    );
  BuildMemoryAllocationHob (
    (UINTN) PcdGet32 (PcdFlashAreaBaseAddress),
    (UINTN) PcdGet32 (PcdFlashAreaSize),
    EfiMemoryMappedIO
    );

  return EFI_SUCCESS;
}
/**
  Publishes STORED_HASH_FV PPI.

  @param[in]      BootMode     Current BootMode.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others

**/
EFI_STATUS
CreateAndInstallStoredHashFvPpi (
  IN EFI_BOOT_MODE                                  BootMode
  )
{
  EDKII_PEI_FIRMWARE_VOLUME_INFO_STORED_HASH_FV_PPI *StoredHashFvPpi;
  HASH_STRUCTURE                                    *ObbHash;
  UINT64                                            FvFlag;
  UINT64                                            CapsuleRecoveryFlag;
  EFI_STATUS                                        Status;
  SETUP_DATA                                        SetupData;
  UINTN                                             VarSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI                   *VariableServices;
  HECI_FW_STS5_REGISTER                             MeFwSts5;
  UINT64                                            HeciBaseAddress;

  StoredHashFvPpi     = NULL;
  ObbHash             = NULL;
  FvFlag              = 0;
  CapsuleRecoveryFlag = 0;
  Status              = EFI_SUCCESS;
  //
  // Locate Setup variables
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PeiServicesLocatePpi failed\n"));
    return Status;
  }

  VarSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VarSize,
                               &SetupData
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GetVariable (SetupData) failed\n"));
    return Status;
  }
  StoredHashFvPpi = AllocateZeroPool (
                      sizeof (EDKII_PEI_FIRMWARE_VOLUME_INFO_STORED_HASH_FV_PPI)
                      + sizeof (HASHED_FV_INFO) * MAX_NUMBER_OF_OBB_FIRMWARE_VOLUMES
                      );
  ASSERT (StoredHashFvPpi != NULL);
  if (StoredHashFvPpi == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Check if Heci device is enabled
  //
  HeciBaseAddress = MeHeciPciCfgBase (HECI1);
  if (PciSegmentRead32 (HeciBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFFFFFF) {
    DEBUG ((DEBUG_INFO, "ME is disabled\n"));
  } else {
    MeFwSts5.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS_5);
    if (SetupData.SkipObbHashVerification == 2) {
      if ((MeFwSts5.r.BtgProfile != 0) && (MeFwSts5.ul != 0xFFFFFFFF)) {
        FvFlag = GetBaseHashFvFlag ();
      }
    }
  }
  if (SetupData.SkipObbHashVerification == 0) {
    FvFlag = GetBaseHashFvFlag ();
  }

  //
  // Flag to skip FV installation in Capsule recovery boot.
  //
  if ((!IsCapsuleUpdateRecoveryBoot ()) && (BootMode != BOOT_ON_S3_RESUME)) {
    CapsuleRecoveryFlag = 0;
  } else {
    CapsuleRecoveryFlag = HASHED_FV_FLAG_SKIP_BOOT_MODE(BOOT_ON_FLASH_UPDATE);
  }
  Status = CreateStoredHashFvPpiInfo (StoredHashFvPpi, FvFlag, CapsuleRecoveryFlag, BootMode);

  if (!IsCapsuleUpdateRecoveryBoot ()) {
    ObbHash = FindObbHash (BootMode);
  } else {
    ObbHash = FindPostIbbHash (BootMode);
  }

  Status = InstallStoredHashFvPpi (StoredHashFvPpi, ObbHash, BootMode, FvFlag);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

VOID
ReportPostMemFv (
  VOID
  )
{
  EFI_STATUS                             Status;
  EFI_BOOT_MODE                          BootMode;

  //
  // Install the PPI to trigger the notify.
  // Check if need to change boot mode to BOOT_ON_FLASH_UPDATE.
  // Note: This need to be changed before the boot mode to be installed within gEdkiiPeiFirmwareVolumeInfoStoredHashFvPpiGuid.
  //
  Status = PeiServicesInstallPpi (&mPpiListBeforePostMemFvReport);
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  /*
    Note : FSP FVs except FSP-T FV are installed in IntelFsp2WrapperPkg or FspPkg.
  */

  ///
  /// Build HOB for DXE
  ///
  if (BootMode == BOOT_IN_RECOVERY_MODE) {
    ///
    /// Prepare the recovery service
    ///
  } else {
    ///
    /// OBB verification
    ///
    Status = PeiServicesNotifyPpi (&mExtendedBiosDecodeReadyNotifyList [1]);
    ASSERT_EFI_ERROR (Status);
    Status = PeiServicesNotifyPpi (&mExtendedBiosDecodeReadyNotifyList [0]);
    ASSERT_EFI_ERROR (Status);
    Status = CreateAndInstallStoredHashFvPpi (BootMode);
    ASSERT_EFI_ERROR (Status);

  }
}

/**
  Callback on Extended BIOS Decode Ready Ppi so Extended PostMemory FV is installed to the memory
  for Extended BIOS Region. The callback is called regardless of Extended BIOS Region support
  in platform code.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The gExtendedBiosDecodeReady PPI.  Not used.

  @retval EFI_SUCCESS             Always returns EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
InstallFvExtendedPostMemoryCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
#if FixedPcdGetBool(PcdExtendedBiosRegionSupport) == 1
  EFI_FIRMWARE_VOLUME_HEADER  *FvHeader;

  DEBUG ((DEBUG_INFO, "Install FlashFvExtendedPostMemory - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvExtendedPostMemoryBase), PcdGet32 (PcdFlashFvExtendedPostMemorySize)));
  FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) (UINT32) PcdGet32 (PcdFlashFvExtendedPostMemoryBase);
  PeiServicesInstallFvInfo2Ppi (
    &(FvHeader->FileSystemGuid),
    (VOID *) (UINTN) PcdGet32 (PcdFlashFvExtendedPostMemoryBase),
    PcdGet32 (PcdFlashFvExtendedPostMemorySize),
    NULL,
    NULL,
    0
    );
  PrintFvHeaderInfo (FvHeader);

#else
  DEBUG ((DEBUG_INFO, "Extended BIOS Region is not supported by the image. No FV installed here\n"));
#endif
  return EFI_SUCCESS;
}


/**
  Callback on Extended BIOS Decode Ready Ppi so Extended Advanced FV is installed to the memory
  for Extended BIOS Region. The callback is called regardless of Extended BIOS Region support in
  platform code. Extended Advanced FV contains DXE phase drivers only and may want to be skipped
  on S3 path for responsiveness.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The gExtendedBiosDecodeReady PPI.  Not used.

  @retval EFI_SUCCESS             Always returns EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
InstallFvExtendedAdvancedCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
#if FixedPcdGetBool(PcdExtendedBiosRegionSupport) == 1
  EFI_FIRMWARE_VOLUME_HEADER  *FvHeader;

  DEBUG ((DEBUG_INFO, "Install FlashFvExtendedAdvanced - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvExtendedAdvancedBase), PcdGet32 (PcdFlashFvExtendedAdvancedSize)));
  FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) (UINT32) PcdGet32 (PcdFlashFvExtendedAdvancedBase);
  PeiServicesInstallFvInfo2Ppi (
    &(FvHeader->FileSystemGuid),
    (VOID *) (UINTN) PcdGet32 (PcdFlashFvExtendedAdvancedBase),
    PcdGet32 (PcdFlashFvExtendedAdvancedSize),
    NULL,
    NULL,
    0
    );
  PrintFvHeaderInfo (FvHeader);

#else
  DEBUG ((DEBUG_INFO, "Extended BIOS Region is not supported by the image. No FV installed here\n"));
#endif
  return EFI_SUCCESS;
}


/**
  Debug support function to output detailed information on a firmware volume being installed.
  Setting DEBUG_PROPERTY_DEBUG_CODE_ENABLED bit of PcdDebugProperyMask lets the function to be
  included in a module. Refer to DEBUG_CODE macro.

  @param[in] FvHeader   Pointer to firmware volume header
**/
VOID
PrintFvHeaderInfo (
  EFI_FIRMWARE_VOLUME_HEADER  *FvHeader
  )
{
  DEBUG_CODE (
    EFI_FIRMWARE_VOLUME_EXT_HEADER  *FvExtHeader;
    EFI_FFS_FILE_HEADER             *FfsHeader;

      DEBUG ((DEBUG_INFO, "[ FV @ 0x%x ] \n", FvHeader));
      DEBUG ((DEBUG_INFO, " FV File System       :  %g   \n",   &FvHeader->FileSystemGuid));
    if (FvHeader->ExtHeaderOffset != 0) {
      FvExtHeader = (EFI_FIRMWARE_VOLUME_EXT_HEADER *) ((UINT8 *) FvHeader + FvHeader->ExtHeaderOffset);
      FfsHeader   = (EFI_FFS_FILE_HEADER *) ((UINT8 *) FvExtHeader + FvExtHeader->ExtHeaderSize);
      FfsHeader = (EFI_FFS_FILE_HEADER *) ALIGN_POINTER (FfsHeader, 8);
      DEBUG ((DEBUG_INFO, " FV GUID              :  %g   \n",   &FvExtHeader->FvName));
      DEBUG ((DEBUG_INFO, " File GUID            :  %g   \n",   &FfsHeader->Name));
    }
  );
}
