/** @file
  FSP WrapperPeim implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#include <Uefi.h>
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SmmAccessLib.h>
#include <Library/PchSmmControlLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/MtrrLib.h>
#include <Library/SiMtrrLib.h>
#include <Library/PrintLib.h>
#include <Ppi/MpServices2.h>
#include <IndustryStandard/FirmwareVersionInfo.h>
#include <Library/FspInfoLib.h>

#define  FSP_BIN_VERSION_STRING       "FSP Binary Version"

EFI_STATUS
EFIAPI
FspWrapperMtrrEndOfPeiEvent (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDesc,
  IN VOID                       *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR  mFspWrapperMtrrEndofPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  FspWrapperMtrrEndOfPeiEvent
};

#if FixedPcdGet8(PcdFspModeSelection) == 1
EFI_PEI_PPI_DESCRIPTOR mFspReadyToInstallMp = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiReadyToInstallMpPpiGuid,
  NULL
};
#endif

/**
  A minimal wrapper function that allows MtrrSetAllMtrrs() to be passed to
  EDKII_PEI_MP_SERVICES2_PPI.StartupAllAPs() as Procedure.

  @param[in] Buffer  Pointer to an MTRR_SETTINGS object, to be passed to
                     MtrrSetAllMtrrs().
**/
VOID
EFIAPI
SetMtrrsFromBuffer (
  IN VOID *Buffer
  )
{
  MtrrSetAllMtrrs (Buffer);
}

/**
  FSP Mtrr End of PEI callback function.

  @param[in] PeiServices   - Pointer to PEI Services Table.
  @param[in] NotifyDesc    - Pointer to the descriptor for the Notification event that
                             caused this function to execute.
  @param[in] Ppi           - Pointer to the PPI data associated with this function.

  @retval EFI_STATUS       - Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
FspWrapperMtrrEndOfPeiEvent (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDesc,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                 Status;
  MTRR_SETTINGS              MtrrSetting;
  EDKII_PEI_MP_SERVICES2_PPI *MpServices2Ppi;

  Status = MtrrTransfer2DefaultWB (&MtrrSetting);
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  //
  // Synchronize the update with all APs
  //
  Status = PeiServicesLocatePpi (
             &gEdkiiPeiMpServices2PpiGuid,
             0,
             NULL,
             (VOID **) &MpServices2Ppi
             );

  if (!EFI_ERROR (Status)) {
    MtrrGetAllMtrrs (&MtrrSetting);
    Status = MpServices2Ppi->StartupAllAPs (
                              MpServices2Ppi,
                              (EFI_AP_PROCEDURE) SetMtrrsFromBuffer,
                              FALSE,
                              0,
                              (VOID *) &MtrrSetting
                              );
    if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
      DEBUG ((DEBUG_ERROR, "SetMtrrsFromBuffer: StartupAllAps(): %r\n", Status));
      ASSERT (FALSE);
    }
  } else {
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}


/**
  Publish FSP binary version via FVI

  @param[in]     None

  @retval        EFI_SUCCESS
                 EFI_NOT_FOUND
**/
EFI_STATUS
PublishFspVersion (VOID)
{
  SMBIOS_TABLE_TYPE_OEM_INTEL_FVI *FspFviRecord;
  UINT8                           FspFviRecordSize;
  INTEL_FIRMWARE_VERSION_INFO     FspFviData;
  UINT8                           FspFviCount;
  CHAR8                           *DestPtr;
  VOID                            *FspFviHobPtr;
  FSP_INFO_HOB                    *FspInfo;
  CHAR8                           VersionString[15]; // FSP version format: XX.XX.XXXX.XXXX
  EFI_HOB_GUID_TYPE               *GuidHob;

  //
  // Locate FSP_INFO_HOB
  //
  GuidHob = (EFI_HOB_GUID_TYPE *) GetFirstGuidHob (&gFspInfoGuid);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }

  FspInfo = (FSP_INFO_HOB *) GET_GUID_HOB_DATA (GuidHob);

  FspFviCount      = 1;
  FspFviRecordSize = sizeof (SMBIOS_STRUCTURE) + sizeof (UINT8) +
                     (FspFviCount * sizeof (INTEL_FIRMWARE_VERSION_INFO)) +
                     sizeof (FSP_BIN_VERSION_STRING) +
                     (sizeof (VersionString) / sizeof (CHAR8)) + sizeof (UINT16);
  DestPtr          = NULL;
  FspFviHobPtr     = NULL;

  //
  // Initialize FSP binary FVI record.
  //
  FspFviRecord = (SMBIOS_TABLE_TYPE_OEM_INTEL_FVI *) AllocateZeroPool (FspFviRecordSize);
  if (FspFviRecord != NULL) {
    FspFviRecord->Header.Type   = INTEL_FVI_SMBIOS_TYPE;
    FspFviRecord->Header.Length = sizeof (SMBIOS_STRUCTURE) + sizeof (UINT8) + (FspFviCount * sizeof (INTEL_FIRMWARE_VERSION_INFO));
    FspFviRecord->Header.Handle = 0;
    FspFviRecord->Count         = FspFviCount;

    //
    // Initialize FSP binary FVI data.
    //
    FspFviData.ComponentName        = 1; // String#1
    FspFviData.VersionString        = 2; // String#2
    FspFviData.Version.MajorVersion = 0xFF;
    FspFviData.Version.MinorVersion = 0xFF;
    FspFviData.Version.Revision     = 0xFF;
    FspFviData.Version.BuildNumber  = 0xFFFF;

    //
    // Concatenate FVI data to FVI record
    //
    DestPtr = (CHAR8 *)((UINTN)FspFviRecord + sizeof (SMBIOS_STRUCTURE) + sizeof (UINT8));
    CopyMem (DestPtr, &FspFviData, sizeof (INTEL_FIRMWARE_VERSION_INFO));

    //
    // Concatenate FVI strings to FVI record
    //
    DestPtr = (CHAR8 *)((UINTN)DestPtr + (FspFviCount * sizeof (INTEL_FIRMWARE_VERSION_INFO)));
    CopyMem (DestPtr, FSP_BIN_VERSION_STRING, sizeof (FSP_BIN_VERSION_STRING)); // String#1

    DestPtr = (CHAR8 *)((UINTN)DestPtr + sizeof (FSP_BIN_VERSION_STRING));
    AsciiValueToStringS (DestPtr, 3, PREFIX_ZERO | RADIX_HEX, FspInfo->VersionMajor, 2); // String#2.VersionMajor
    DestPtr = (CHAR8 *)((UINTN)DestPtr + 2);
    *DestPtr = 0x2E;
    DestPtr = (CHAR8 *)((UINTN)DestPtr + 1);
    AsciiValueToStringS (DestPtr, 3, PREFIX_ZERO | RADIX_HEX, FspInfo->VersionMinor, 2); // String#2.VersionMinor
    DestPtr = (CHAR8 *)((UINTN)DestPtr + 2);
    *DestPtr = 0x2E;
    DestPtr = (CHAR8 *)((UINTN)DestPtr + 1);
    AsciiValueToStringS (DestPtr, 5, PREFIX_ZERO | RADIX_HEX, FspInfo->VersionRevision, 4);      // String#2.VersionRevision
    DestPtr = (CHAR8 *)((UINTN)DestPtr + 4);
    *DestPtr = 0x2E;
    DestPtr = (CHAR8 *)((UINTN)DestPtr + 1);
    AsciiValueToStringS (DestPtr, 5, PREFIX_ZERO | RADIX_HEX, FspInfo->VersionBuild, 4);         // String#2.VersionBuild

    FspFviHobPtr = BuildGuidDataHob (
                           &gIntelSmbiosDataHobGuid,
                           (VOID *) FspFviRecord,
                           FspFviRecordSize
                             );
    ASSERT (FspFviHobPtr != NULL);

    FreePool (FspFviRecord);
  } else {
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
  }

  return EFI_SUCCESS;
}


/**
  The Entry point of the FSP wrapper PEIM

  @param[in]  FileHandle   Pointer to image file handle
  @param[in]  PeiServices  Pointer to PEI Services Table

  @return     EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
FspWrapperPeimEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                       Status;
  EFI_BOOT_MODE                    BootMode;

  DEBUG ((DEBUG_INFO, "Entered FSP WrapperPeim entry\n"));

  BuildFspInfo ();
  PchSmmControlInit ();
  PublishFspVersion ();

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  if (BootMode == BOOT_ON_S3_RESUME) {
    PeiInstallSmmAccessPpi ();
#if FixedPcdGet8(PcdFspModeSelection) == 1
    //
    // Install mp service to sync BSP AP mtrrs when S3 resume.
    //
    Status = PeiServicesInstallPpi (&mFspReadyToInstallMp);
    ASSERT_EFI_ERROR (Status);
#endif
    //
    // Install Fsp Wrapper Mtrr Notify for S3 resume
    //
    Status = PeiServicesNotifyPpi (&mFspWrapperMtrrEndofPeiNotifyList);
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((DEBUG_INFO, "Exit FSP WrapperPeim\n"));
  return EFI_SUCCESS;
}
