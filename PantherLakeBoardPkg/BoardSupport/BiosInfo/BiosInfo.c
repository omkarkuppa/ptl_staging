/** @file
  PEIM to provide BiosInfo structure listing up all firmware volume's base addresses, sizes,
  attributes, and information associated to the firmware volume.
  Primarily the structure is used on FIT table creation and Bpm.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

#include <PiPei.h>
#include <Guid/BiosInfo.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <IndustryStandard/FirmwareInterfaceTable.h>
#include <Guid/SysFwUpdateProgress.h>
#include <Ppi/FirmwareVolumeInfoMeasurementExcluded.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SpiAccessLib.h>
#include <Guid/MigratedFvInfo.h>
#include <Library/BootGuardLib.h>
#include <Library/BaseBpmAccessLib.h>
#include <Library/IoLib.h>

EFI_STATUS
EFIAPI
InstallMeasurementExcludedFvList (
  VOID
  );

#if FixedPcdGetBool (PcdMultiIbbFeatureEnable) == 1
  #define BASE_FV_SIZE 12
#else
#if FixedPcdGetBool (PcdSignedFspEnable) == 0
  #define BASE_FV_SIZE 10
#else
  #define BASE_FV_SIZE 9
#endif
#endif

#if FixedPcdGetBool (PcdFspWrapperResetVectorInFsp) == 1
  #define FSP_WRAPPER_FV_SIZE 4
#else
  #define FSP_WRAPPER_FV_SIZE 3
#endif

#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  #define TSN_MAC_ADDRESS_FV_SIZE 1
#else
  #define TSN_MAC_ADDRESS_FV_SIZE 0
#endif

#if FixedPcdGetBool(PcdExtendedBiosRegionSupport) == 1
// #define EXTENDED_REGION_FV_SIZE 2
  #define EXTENDED_REGION_FV_SIZE 0
#else
  #define EXTENDED_REGION_FV_SIZE 0
#endif

#define BIOS_INFO_STRUCT_SIZE (BASE_FV_SIZE + FSP_WRAPPER_FV_SIZE + TSN_MAC_ADDRESS_FV_SIZE + EXTENDED_REGION_FV_SIZE)

#if FixedPcdGetBool (PcdSignedFspEnable) == 1
  #define SIGNED_FSP_EXCLUDE_LIST_SIZE (4)
#endif


/*
  BIOS_INFO structure is the base of the firmware volume layout for Intel platform BIOS implementation
  so security checker module can run based on the structure and throw warnings, error or deadloop
  when any unexpected firmware volumes are detected.

  BIOS_INFO is recommended to support full entries of firmware volumes present in a flash
  with right type, attribute, version, flash map base address and size,
  all associated information which is defined by BIOS_INFO_STRUCT structure.
  - IBB firmware volumes, which are expected to be measured or/and verified
    by hardware base security solution to meet SecureBoot chain of trust
    (Intel BootGuard for example), have attribute 0x0.
  - Post IBB firmware volumes, which are expected to be measured or/and verified
    by BIOS (TCG code for measurement, RSA2048SHA256Sign algorithm for verification for example),
    have attribute BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB.
  - Else, follows Firmware Interface Table specification.
*/
#pragma pack (1)
typedef struct {
  BIOS_INFO_HEADER  Header;
  BIOS_INFO_STRUCT  Entry[BIOS_INFO_STRUCT_SIZE];
} BIOS_INFO;
#pragma pack ()

#if FixedPcdGetBool (PcdSignedFspEnable) == 1
EFI_PHYSICAL_ADDRESS SignedFspExcludeList[SIGNED_FSP_EXCLUDE_LIST_SIZE] = {
    /*FSP-T*/
    FixedPcdGet32 (PcdFlashFvFspTBase),
    /*FSP-O*/
    FixedPcdGet32 (PcdFlashFvFspOBase),
    /*FSP-M*/
    FixedPcdGet32 (PcdFlashFvFspMBase),
    /*BSP-PreMem*/
    FixedPcdGet32 (PcdFlashFvPreMemoryBase)
};
#endif

GLOBAL_REMOVE_IF_UNREFERENCED BIOS_INFO  mBiosInfo = {
  {
    BIOS_INFO_SIGNATURE,
    BIOS_INFO_STRUCT_SIZE,
    0,
  },
  {
    /*
    Extended Bios region may not be mapped yet when BiosInfoChecker
    reads FvHeader from flash. Extended FVs are excluded from
    BiosInfo so BiosInfoChecker skips checking on the Security requirements.
    @todo the region wants to be dynamically checked on FV installation.
    Use EXTENDED_REGION_FV_SIZE macro to increase BiosInfo struct entries,
    once the dynamic check solution is implemented.
#if FixedPcdGetBool(PcdExtendedBiosRegionSupport) == 1
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvExtendedPostMemorySize),
      FixedPcdGet32 (PcdFlashFvExtendedPostMemoryBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvExtendedAdvancedSize),
      FixedPcdGet32 (PcdFlashFvExtendedAdvancedBase)
    },
#endif
    */
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_GENERAL_EXCLUDE_FROM_FIT,
      0x0100,
      FixedPcdGet32 (PcdFlashNvStorageVariableSize) + FixedPcdGet32 (PcdFlashNvStorageFtwWorkingSize) + FixedPcdGet32 (PcdFlashNvStorageFtwSpareSize),
      FixedPcdGet32 (PcdFlashNvStorageVariableBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvUplSize),
      FixedPcdGet32 (PcdFlashFvUplBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvAdvancedSize),
      FixedPcdGet32 (PcdFlashFvAdvancedBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvSecuritySize),
      FixedPcdGet32 (PcdFlashFvSecurityBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvOsBootSize),
      FixedPcdGet32 (PcdFlashFvOsBootBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvUefiBootSize),
      FixedPcdGet32 (PcdFlashFvUefiBootBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvPostMemorySize),
      FixedPcdGet32 (PcdFlashFvPostMemoryBase)
    },
    {
      /*
        Note :
        Startup ACM is one of the binaries in FvFirmwareBinaries,
        so put type 07 but not type 02.
        FIT table will contain a type 02 entry with actual address
        of ACM binary (it is passed as an input to FitGen tool).
      */
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_GENERAL_EXCLUDE_FROM_FIT,
      0x0100,
      FixedPcdGet32 (PcdFlashFvFirmwareBinariesSize),
      FixedPcdGet32 (PcdFlashFvFirmwareBinariesBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvFspSSize),
      FixedPcdGet32 (PcdFlashFvFspSBase)
    },

#if FixedPcdGetBool (PcdMultiIbbFeatureEnable) == 1
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_GENERAL_EXCLUDE_FROM_FIT,    // IBB FV [additional IBBs (other than IBB0 a.k.a. common IBB) used in Multi-IBB mode]
      0x0100,
      FixedPcdGet32 (PcdFlashFvIbb1Size),
      FixedPcdGet32 (PcdFlashFvIbb1Base)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_GENERAL_EXCLUDE_FROM_FIT,    // IBB FV [additional IBBs (other than IBB0 a.k.a. common IBB) used in Multi-IBB mode]
      0x0100,
      FixedPcdGet32 (PcdFlashFvIbbnp1Size),
      FixedPcdGet32 (PcdFlashFvIbbnp1Base)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_GENERAL_EXCLUDE_FROM_FIT,    // IBB FV [additional IBBs (other than IBB0 a.k.a. common IBB) used in Multi-IBB mode]
      0x0100,
      FixedPcdGet32 (PcdFlashFvFspMSize),
      FixedPcdGet32 (PcdFlashFvFspMBase)
    },
#elif FixedPcdGetBool (PcdSignedFspEnable) == 1
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_GENERAL_EXCLUDE_FROM_FIT,    // IBB FV
      0x0100,
      FixedPcdGet32 (PcdFlashFvFspMSize),
      FixedPcdGet32 (PcdFlashFvFspMBase)
    },
#else
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      0x00,    // IBB FV
      0x0100,
      FixedPcdGet32 (PcdFlashFvFspMSize),
      FixedPcdGet32 (PcdFlashFvFspMBase)
    },
#endif

#if FixedPcdGetBool (PcdSignedFspEnable) == 0
#if FixedPcdGetBool (PcdFspWrapperResetVectorInFsp) == 1
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      0x00,    // IBB FV
      0x0100,
      FixedPcdGet32 (PcdFlashFvFspOSize),
      FixedPcdGet32 (PcdFlashFvFspOBase)
    },
#endif
   {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      0x00,    // IBB FV
      0x0100,
      FixedPcdGet32 (PcdFlashFvFspTSize),
      FixedPcdGet32 (PcdFlashFvFspTBase)
    },
#else
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      0x00,    // IBB FV
      0x0100,
      SIZE_4KB,
      SIZE_4GB - SIZE_4KB
    },
#endif
#if FixedPcdGetBool (PcdSignedFspEnable) == 1
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_GENERAL_EXCLUDE_FROM_FIT,    // IBB FV
      0x0100,
      FixedPcdGet32 (PcdFlashFvPreMemorySize),
      FixedPcdGet32 (PcdFlashFvPreMemoryBase)
    },
#else
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      0x00,    // IBB FV
      0x0100,
      FixedPcdGet32 (PcdFlashFvPreMemorySize),
      FixedPcdGet32 (PcdFlashFvPreMemoryBase)
    },
#endif
    {
      FIT_TYPE_01_MICROCODE,
      BIOS_INFO_STRUCT_ATTRIBUTE_MICROCODE_WHOLE_REGION,
      0x0100,
      FixedPcdGet32 (PcdFlashFvMicrocodeSize),
      FixedPcdGet32 (PcdFlashFvMicrocodeBase)
    },

#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_GENERAL_EXCLUDE_FROM_FIT,
      0x0100,
      FixedPcdGet32 (PcdFlashFvTsnMacAddressSize),
      FixedPcdGet32 (PcdFlashFvTsnMacAddressBase)
    },
#endif
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_PPI_DESCRIPTOR  mBiosInfoPpiList = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gBiosInfoGuid,
  &mBiosInfo
};

#if FixedPcdGetBool(PcdCapsuleEnable) == 1
#define BIOS_RECOVERY_INFO_SIGNATURE  SIGNATURE_64 ('$', 'B', 'I', 'O', 'S', 'R', 'I', '$')
#define BIOS_RECOVERY_INFO_STRUCT_SIZE 2
#pragma pack (1)
typedef struct {
  BIOS_INFO_HEADER  Header;
  BIOS_INFO_STRUCT  Entry[BIOS_RECOVERY_INFO_STRUCT_SIZE];
} BIOS_RECOVERY_INFO;
#pragma pack ()
GLOBAL_REMOVE_IF_UNREFERENCED BIOS_RECOVERY_INFO  mBiosInfoRecovery = {
  {
    BIOS_RECOVERY_INFO_SIGNATURE,
    BIOS_RECOVERY_INFO_STRUCT_SIZE,
    0,
  },
  {
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvPostMemorySize),
      FixedPcdGet32 (PcdFlashFvPostMemoryBase)
    },
    {
      FIT_TYPE_07_BIOS_STARTUP_MODULE,
      BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB,
      0x0100,
      FixedPcdGet32 (PcdFlashFvFspSSize),
      FixedPcdGet32 (PcdFlashFvFspSBase)
    },
  }
};
GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_PPI_DESCRIPTOR  mBiosInfoRecoveryPpiList = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gBiosInfoGuid, // Consumed by BiosInfoChecker
  &mBiosInfoRecovery
};

/**
  Determine if Bios Check on Obb FVs should be deferred.
  This happens when Obb on SPI is not trusted:
  E.g., BIOS update is interrupted, update is not happened but TopSwap is enabled.

  @retval TRUE   Install mBiosInfoRecoveryPpiList to defer BIOS check on Obb SPI region.
  @retval FALSE  Install mBiosInfoPpiList normally.

**/
BOOLEAN
IsDeferredBiosCheckOnObbFvs (
  VOID
  )
{
  EFI_HOB_GUID_TYPE               *GuidHob;
  SYSTEM_FIRMWARE_UPDATE_PROGRESS *UpdateProgress;

  GuidHob = NULL;
  GuidHob = GetFirstGuidHob (&gSysFwUpdateProgressGuid);
  if (GuidHob != NULL) {
    UpdateProgress = (SYSTEM_FIRMWARE_UPDATE_PROGRESS *) GET_GUID_HOB_DATA (GuidHob);

    if (UpdateProgress->Component == UpdatingBios) {
      DEBUG ((DEBUG_INFO, "BIOS update is in progress, defer Obb FV checks to PlatformRecoveryModule\n"));
      return TRUE;
    }

    if ((UpdateProgress->Component == UpdatingResiliency) && (SpiIsTopSwapEnabled ())) {
      DEBUG ((DEBUG_INFO, "Post-BIOS update while the new BIOS is a bad one\n"));
      return TRUE;
    }
  } else if (SpiIsTopSwapEnabled ()) {
    DEBUG ((DEBUG_INFO, "BIOS update is not happened while TopSwap is enabled somehow\n"));
    return TRUE;
  }

  return FALSE;
}
#endif

/**
  Set migrated FSP-M base address to the PCD.

**/
VOID
SetFspmFvNewBaseAddress (
  )
{
  EFI_PEI_HOB_POINTERS    Hob;
  EDKII_MIGRATED_FV_INFO  *MigratedFvInfo;

    Hob.Raw  = GetFirstGuidHob (&gEdkiiMigratedFvInfoGuid);
    while (Hob.Raw != NULL) {
      MigratedFvInfo = GET_GUID_HOB_DATA (Hob);
      if (PcdGet32 (PcdFspmBaseAddress) == MigratedFvInfo->FvOrgBase) {
        PcdSet32S (PcdFspmBaseAddress, (UINT32)(UINTN)MigratedFvInfo->FvNewBase);
        break;
      }
      Hob.Raw = GET_NEXT_HOB (Hob);
      Hob.Raw = GetNextGuidHob (&gEdkiiMigratedFvInfoGuid, Hob.Raw);
    }
  return;
}

EFI_PEI_NOTIFY_DESCRIPTOR  mSetFspmBaseAfterMemoryDiscovered = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEfiPeiMemoryDiscoveredPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) SetFspmFvNewBaseAddress
};

EFI_PEI_NOTIFY_DESCRIPTOR  mInstallExcludelistAfterMemoryDiscovered = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEfiPeiMemoryDiscoveredPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) InstallMeasurementExcludedFvList
};

/**
  Installs BiosInfo Ppi.

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCCESS   Install the BiosInfo Ppi successfully.

**/
EFI_STATUS
EFIAPI
BiosInfoEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;
  ACM_BIOS_POLICY    AcmPolicySts;
  EFI_STATUS         Status;
  VOID               *HobData;

  GuidHob = NULL;

#if FixedPcdGetBool(PcdCapsuleEnable) == 1
  if (IsDeferredBiosCheckOnObbFvs ()) {
    //
    // Install PPI, so that BiosInfoChecker PEI module can check Signed Section of the table.
    //
    Status = PeiServicesInstallPpi (&mBiosInfoRecoveryPpiList);
    ASSERT_EFI_ERROR (Status);
    //
    // Build BiosInfoRecovery hob to indicate PostMem FV checks on Obb are deferred.
    //
    HobData = BuildGuidHob (&gBiosInfoRecoveryGuid, sizeof (mBiosInfoRecovery));
    ASSERT (HobData != NULL);
    if (HobData == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    CopyMem (HobData, &mBiosInfoRecovery, sizeof (mBiosInfoRecovery));
  } else {
#endif
    //
    // Install PPI, so that other PEI module can add dependency.
    //
    Status = PeiServicesInstallPpi (&mBiosInfoPpiList);
    ASSERT_EFI_ERROR (Status);

    //
    // If Boot Guard or FSP already measured IBB, we do not need let TPM measure it again.
    // If S-CRTM status is set, ACM has done the measurement
    // If gTcgEventDataHobGuid is be found, FSP has done the measurement
    //
    GuidHob = GetFirstGuidHob (&gTcgEventDataHobGuid);
    AcmPolicySts.Data = MmioRead64 (MMIO_ACM_POLICY_STATUS);

    if ((AcmPolicySts.Bits.SCrtmStatus != 0) || GuidHob != NULL) {
      Status = PeiServicesNotifyPpi (&mInstallExcludelistAfterMemoryDiscovered);
      ASSERT_EFI_ERROR (Status);
    }

    //
    // Build hob, so that DXE module can also get the data.
    //
    HobData = BuildGuidHob (&gBiosInfoGuid, sizeof (mBiosInfo));
    ASSERT (HobData != NULL);
    if (HobData == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    CopyMem (HobData, &mBiosInfo, sizeof (mBiosInfo));
#if FixedPcdGetBool(PcdCapsuleEnable) == 1
  }
#endif
  Status = PeiServicesNotifyPpi (&mSetFspmBaseAfterMemoryDiscovered);
  ASSERT_EFI_ERROR (Status);
  return EFI_SUCCESS;
}

/**
  This function call dynamically creates the IBB FV list based on BiosInfo structure,
  and install the measurement excluded PPI. The primary consumer of the PPI is TCG code.
**/
EFI_STATUS
EFIAPI
InstallMeasurementExcludedFvList (
  )
{
  EFI_STATUS                   Status;
#if FixedPcdGetBool (PcdSignedFspEnable) == 0
  BIOS_INFO_HEADER             *BiosInfoHeader;
  BIOS_INFO_STRUCT             *BiosInfoStruct;
#endif
  EFI_FIRMWARE_VOLUME_HEADER   *FvHeader;
  UINT32                       IbbFvCount;
  UINTN                        Index;
  EFI_PEI_PPI_DESCRIPTOR       *PeiFirmwareVolumeInfoMeasurementExcludedPpi;
  EFI_PEI_FIRMWARE_VOLUME_INFO_MEASUREMENT_EXCLUDED_FV  FvListTmp [BIOS_INFO_STRUCT_SIZE];
  EFI_PEI_FIRMWARE_VOLUME_INFO_MEASUREMENT_EXCLUDED_PPI *IbbFvPpi;

  DEBUG ((DEBUG_INFO, "InstallMeasurementExcludedFvList : Creating Exclude FV List from Measurement\n"));

#if FixedPcdGetBool (PcdSignedFspEnable) == 0
  Status = PeiServicesLocatePpi (&gBiosInfoGuid, 0, NULL, (VOID **) &BiosInfoHeader);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallMeasurementExcludedFvList : BiosInfo PPI not found\n"));
    ASSERT_EFI_ERROR (Status);
    return EFI_NOT_FOUND;
  }
  BiosInfoStruct = (BIOS_INFO_STRUCT *) ((UINT8 *) BiosInfoHeader + sizeof (BIOS_INFO_HEADER));
#endif
  /*
   First go through all BiosInfoStructure and store IBB FV to FvListTmp.
   - FvBase is filled with the value read from BiosInfoStruct.
   - FvLength is filled with the value read FV Header, which has the exact installed size information.
   Note: BiosInfo structure's size values may not be the exact size of actually installed FV (example FSP FVs).
  */
#if FixedPcdGetBool (PcdSignedFspEnable) == 1
  IbbFvCount = 0;
  for (Index = 0; Index < SIGNED_FSP_EXCLUDE_LIST_SIZE; Index++) {
    if((SignedFspExcludeList [Index] == PcdGet32 (PcdFlashFvFspMBase))) {
      if(FixedPcdGetBool (PcdEnableFspmCompression) == 1) {
        SetFspmFvNewBaseAddress ();
        SignedFspExcludeList [Index] = PcdGet32 (PcdFspmBaseAddress);
      }
    }
    FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) SignedFspExcludeList [Index];
    FvListTmp [IbbFvCount].FvBase = (EFI_PHYSICAL_ADDRESS) SignedFspExcludeList [Index];
    FvListTmp [IbbFvCount].FvLength = FvHeader->FvLength;
    IbbFvCount++;
  }
#else
  IbbFvCount = 0;
  for (Index = 0; Index < BiosInfoHeader->EntryCount; Index++) {
    if (BiosInfoStruct [Index].Type != FIT_TYPE_07_BIOS_STARTUP_MODULE) {
      continue;
    }
    if (BiosInfoStruct [Index].Attributes == 0x0) {   // IBB FV attribute
      FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) BiosInfoStruct [Index].Address;
      FvListTmp [IbbFvCount].FvBase = (EFI_PHYSICAL_ADDRESS) BiosInfoStruct [Index].Address;
      FvListTmp [IbbFvCount].FvLength = FvHeader->FvLength;
      IbbFvCount++;
    }
  }
#endif

  IbbFvPpi = (EFI_PEI_FIRMWARE_VOLUME_INFO_MEASUREMENT_EXCLUDED_PPI *) AllocateZeroPool (sizeof (UINT32) + IbbFvCount * sizeof (EFI_PEI_FIRMWARE_VOLUME_INFO_MEASUREMENT_EXCLUDED_FV));
  if (IbbFvPpi == NULL) {
    DEBUG ((DEBUG_ERROR, "InstallMeasurementExcludedFvList : buffer allocation failure\n"));
    Status = RETURN_OUT_OF_RESOURCES;
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  IbbFvPpi->Count = IbbFvCount;
  CopyMem (IbbFvPpi->Fv, FvListTmp,(IbbFvCount * sizeof (EFI_PEI_FIRMWARE_VOLUME_INFO_MEASUREMENT_EXCLUDED_FV)));
  DEBUG ((DEBUG_INFO, "InstallMeasurementExcludedFvList :\n"));
  for (Index = 0; Index < IbbFvPpi->Count; Index++) {
    DEBUG ((DEBUG_INFO, "     %x FV FvBase   =  0x%x\n", Index, IbbFvPpi->Fv [Index].FvBase));
    DEBUG ((DEBUG_INFO, "           FvLength =  0x%x\n", IbbFvPpi->Fv [Index].FvLength));
  }
  PeiFirmwareVolumeInfoMeasurementExcludedPpi = AllocatePool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (PeiFirmwareVolumeInfoMeasurementExcludedPpi == NULL) {
    DEBUG((DEBUG_ERROR, "InstallMeasurementExcludedFvList : buffer allocation failure\n"));
    Status = RETURN_OUT_OF_RESOURCES;
    ASSERT_EFI_ERROR(Status);
    return Status;
  }
  PeiFirmwareVolumeInfoMeasurementExcludedPpi->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PeiFirmwareVolumeInfoMeasurementExcludedPpi->Guid  = &gEfiPeiFirmwareVolumeInfoMeasurementExcludedPpiGuid;
  PeiFirmwareVolumeInfoMeasurementExcludedPpi->Ppi   = IbbFvPpi;
  Status = PeiServicesInstallPpi (PeiFirmwareVolumeInfoMeasurementExcludedPpi);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
