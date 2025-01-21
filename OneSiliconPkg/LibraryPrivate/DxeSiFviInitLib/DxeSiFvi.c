/** @file
  CPU Firmware Version Info implementation.

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

#include <Uefi.h>
#include <ConfigBlock.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/PrintLib.h>
#include <Library/DxeMeLib.h>
#include <Protocol/PciIo.h>
#include <Protocol/Smbios.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/DriverSupportedEfiVersion.h>
#include <IndustryStandard/Pci30.h>
#include <IndustryStandard/SmBios.h>
#include <IndustryStandard/FirmwareVersionInfo.h>
#include <Defines/HostBridgeDefines.h>
#include <SiFvi.h>
#include <Library/PchInfoHob.h>

/**
  Create and update PCH related FVI Records.

  @param[in] PchFviData           A pointer to the INTEL_FIRMWARE_VERSION_INFO
**/
VOID
CreateAndUpdatePchFviRecords (
  IN INTEL_FIRMWARE_VERSION_INFO  *PchFviData
  )
{
  EFI_STATUS                      Status;
  UINTN                           Index;
  UINTN                           NumHandles;
  EFI_HANDLE                      *HandleBuffer;
  EFI_STRING                      DriverName;
  BOOLEAN                         FoundLegacyRaid;
  PCI_DATA_STRUCTURE              *PcirBlockPtr;
  EFI_PCI_IO_PROTOCOL             *PciIo;
  PCI_EXPANSION_ROM_HEADER        *RomImage;
  EFI_COMPONENT_NAME2_PROTOCOL    *ComponentName2;
  CHAR16                          RstDriverName1[] = RST_DRIVER_NAME1;
  CHAR16                          RstDriverName2[] = RST_DRIVER_NAME2;
  EFI_DRIVER_SUPPORTED_EFI_VERSION_PROTOCOL *DriverEfiVersion;

  FoundLegacyRaid = FALSE;
  HandleBuffer = NULL;
  //
  // Get all PCI IO protocols
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &NumHandles,
                  &HandleBuffer
                  );
  if (!EFI_ERROR (Status)) {
    //
    // Find the RAID BIOS by checking each PCI IO handle for RST OPROM
    //
    for (Index = 0; Index < NumHandles; Index++) {
      Status = gBS->HandleProtocol (
                      HandleBuffer[Index],
                      &gEfiPciIoProtocolGuid,
                      (VOID **) &PciIo
                      );
      if (EFI_ERROR (Status) || (PciIo->RomImage == NULL)) {
        //
        // If this PCI device doesn't have a ROM image, skip to the next device.
        //
        continue;
      }

      RomImage = PciIo->RomImage;

      //
      // Get pointer to PCIR structure
      //
      PcirBlockPtr = (PCI_DATA_STRUCTURE *) ((UINTN) RomImage + RomImage->PcirOffset);

      //
      // Check if we have an RAID BIOS OPROM.
      //
      if ((RomImage->Signature == 0xAA55) &&
          (PcirBlockPtr->ClassCode[0] == 0x00) &&
          (PcirBlockPtr->ClassCode[1] == 0x04) &&
          (PcirBlockPtr->ClassCode[2] == 0x01)
          ) {
        PchFviData[RAID_VER].Version.MajorVersion = (UINT8) ((PcirBlockPtr->CodeRevision & 0xFF00) >> 8);
        PchFviData[RAID_VER].Version.MinorVersion = (UINT8)  (PcirBlockPtr->CodeRevision & 0x00FF);
        PchFviData[RAID_VER].Version.Revision     = 0;
        PchFviData[RAID_VER].Version.BuildNumber  = 0;
        FoundLegacyRaid = TRUE;
      }
    }
  }
  if (HandleBuffer != NULL) {
    gBS->FreePool (HandleBuffer);
    HandleBuffer = NULL;
  }
  //
  // Search EFI RST OPROM
  //
  if (FoundLegacyRaid == FALSE) {
    Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gEfiDriverSupportedEfiVersionProtocolGuid,
                    NULL,
                    &NumHandles,
                    &HandleBuffer
                    );
    if (!EFI_ERROR (Status)) {
      for (Index = 0; Index < NumHandles; Index++) {
        Status = gBS->HandleProtocol (
                        HandleBuffer[Index],
                        &gEfiComponentName2ProtocolGuid,
                        (VOID **) &ComponentName2
                        );
        if (EFI_ERROR (Status)) {
          continue;
        }

        Status = ComponentName2->GetDriverName (ComponentName2, "en-US", &DriverName);
        if (EFI_ERROR (Status)) {
          continue;
        }

        if ((StrnCmp (DriverName, RstDriverName1, StrLen(RstDriverName1)) == 0) ||
            (StrnCmp (DriverName, RstDriverName2, StrLen(RstDriverName2)) == 0)) {
          Status = gBS->HandleProtocol (
                          HandleBuffer[Index],
                          &gEfiDriverSupportedEfiVersionProtocolGuid,
                          (VOID **) &DriverEfiVersion
                          );
          if (EFI_ERROR (Status)) {
            continue;
          }
          PchFviData[RAID_VER].Version.MajorVersion = (UINT8) ((DriverEfiVersion->FirmwareVersion & 0x00FF0000) >> 16);
          PchFviData[RAID_VER].Version.MinorVersion = (UINT8)  (DriverEfiVersion->FirmwareVersion & 0x000000FF);
          PchFviData[RAID_VER].Version.Revision     = 0;
          PchFviData[RAID_VER].Version.BuildNumber  = 0;
        }
      }
    }
    if (HandleBuffer != NULL) {
      gBS->FreePool (HandleBuffer);
      HandleBuffer = NULL;
    }
  }
}

/**
  Update ME Smbios FVI data

  @param[in] MeFviData            A pointer to the INTEL_FIRMWARE_VERSION_INFO
  @param[in] MeFviStrings         A pointer to the FVI Strings information
**/
VOID
UpdateMeFvi (
  IN INTEL_FIRMWARE_VERSION_INFO  *MeFviData,
  IN CHAR8                        **MeFviStrings
  )
{
  EFI_STATUS                      Status;
  FLASH_PARTITION_DATA            PartitionIdData;
  UINT8                           Index;
  UINT8                           Count;
  CHAR8                           *SseVersionString;

  Status = HeciGetImageFwVersionMsg (FPT_PARTITION_NAME_EFWP, &PartitionIdData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a() Get Image FW Version fail, Status = %r\n", __FUNCTION__, Status));
    return;
  }

  Count = EnumMeSse + 1;
  SseVersionString = *MeFviStrings;
  for (Index = 0; Index < Count * FVI_NUMBER_OF_STRINGS; Index++) {
    if (AsciiStrnCmp (SseVersionString, SSE_FVI_DEFAULT_STRING, AsciiStrLen (SSE_FVI_DEFAULT_STRING)) == 0) {
      break;
    }
    SseVersionString += AsciiStrSize (SseVersionString);
  }

  if (Index == Count * FVI_NUMBER_OF_STRINGS) {
    DEBUG ((DEBUG_INFO, "%a() ESE FW version string not found\n", __FUNCTION__));
    return;
  }

  //
  // Update SSE version
  //
  AsciiValueToStringS (SseVersionString, 5, PREFIX_ZERO | RADIX_HEX, PartitionIdData.Version.Major, 4);

  //
  // Shift 4 digits to fill next field
  //
  SseVersionString += 4;
  *SseVersionString = '.';
  SseVersionString += 1;
  AsciiValueToStringS (SseVersionString, 5, PREFIX_ZERO | RADIX_HEX, PartitionIdData.Version.Minor, 4);

  SseVersionString += 4;
  *SseVersionString = '.';
  SseVersionString += 1;
  AsciiValueToStringS (SseVersionString, 5, PREFIX_ZERO | RADIX_HEX, PartitionIdData.Version.Hotfix, 4);

  SseVersionString += 4;
  *SseVersionString = '.';
  SseVersionString += 1;
  AsciiValueToStringS (SseVersionString, 5, PREFIX_ZERO | RADIX_HEX, PartitionIdData.Version.Build, 4);
}

/**
  Update PCH Smbios FVI data

  @param[in] PchFviData           A pointer to the INTEL_FIRMWARE_VERSION_INFO
  @param[in, out] PchFviStrings   A pointer to the FVI Strings information
  @param[in]  Crid                GENERAL HOB Crid value
**/
VOID
UpdatePchFvi (
  IN INTEL_FIRMWARE_VERSION_INFO *PchFviData,
  IN OUT CHAR8                   **PchFviStrings,
  IN UINT32                      Crid
  )
{
  STATIC CONST UINT8              StrEnabled[sizeof (PCH_CRID_ENABLED)] = PCH_CRID_ENABLED;
  STATIC CONST UINT8              StrDisabled[sizeof (PCH_CRID_DISABLED)] = PCH_CRID_DISABLED;
  UINT8                           Index;
  EFI_PEI_HOB_POINTERS            HobPtr;
  PCH_INFO_HOB                    *PchInfoHob;

  //
  // Do Crid programming as late as possible so others can get the true PCH stepping.
  //

  HobPtr.Guid = GetFirstGuidHob (&gPchInfoHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  PchInfoHob = (PCH_INFO_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);

  PchFviData[PCH_CRID_ORIGINAL].Version.BuildNumber = (UINT16) PchInfoHob->CridOrgRid;
  if (PchInfoHob->CridSupport && Crid) {
    PmcSetCrid0WithS3BootScript (PmcGetPwrmBase ());
    PchFviData[PCH_CRID_NEW].Version.BuildNumber = (UINT16) PchInfoHob->CridNewRid;
  } else {
    PchFviData[PCH_CRID_NEW].Version.BuildNumber = (UINT16) PchInfoHob->CridOrgRid;
  }
  PmcLockCridWithS3BootScript(PmcGetPwrmBase ());

  //
  // Search the position of VersionString for PCH_CRID_STATUS
  //
  for (Index = 1; Index < PchFviData[PCH_CRID_STATUS].VersionString; Index++) {
    *PchFviStrings += AsciiStrSize ((CHAR8 *) *PchFviStrings);
  }
  if (PchFviData[PCH_CRID_ORIGINAL].Version.BuildNumber != PchFviData[PCH_CRID_NEW].Version.BuildNumber) {
    DEBUG ((DEBUG_INFO, "PCH_CRID_NEW.BuildNumber = %x\n", PchFviData[PCH_CRID_NEW].Version.BuildNumber));
    DEBUG ((DEBUG_INFO, "PCH_CRID_ORIGINAL.BuildNumber = %x\n", PchFviData[PCH_CRID_ORIGINAL].Version.BuildNumber));
    AsciiStrnCpyS (*PchFviStrings, SMBIOS_STRING_MAX_LENGTH, (CHAR8 *) &StrEnabled, sizeof (StrEnabled));
  } else {
    AsciiStrnCpyS (*PchFviStrings, SMBIOS_STRING_MAX_LENGTH, (CHAR8 *) &StrDisabled, sizeof (StrDisabled));
  }

  CreateAndUpdatePchFviRecords (PchFviData);
}

/**
  Update SA Smbios FVI data

  @param[in] SaFviData            A pointer to the INTEL_FIRMWARE_VERSION_INFO
**/
VOID
UpdateSaFvi (
  IN INTEL_FIRMWARE_VERSION_INFO *SaFviData
  )
{
  UINT8                         Data8;
  UINT64                        McBaseAddress;
  UINT16                        VbiosBuildNum;

  VbiosBuildNum = 0xFFFF;

  McBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);
  Data8 = PciSegmentRead8 (McBaseAddress + PCI_REVISION_ID_OFFSET);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint8,
    (UINTN) (PcdGet64 (PcdSiPciExpressBaseAddress) + McBaseAddress + PCI_REVISION_ID_OFFSET),
    1,
    &Data8
    );

  SaFviData[VBIOS_VER].Version.BuildNumber = VbiosBuildNum;
}

/**
  Update All Smbios FVI OEM Type Data

  @param[in]  SmbiosOemType    - SMBIOS OEM Type
  @param[in]  Crid             - GENERAL HOB Crid value

  @retval EFI_UNSUPPORTED      - Could not locate SMBIOS protocol
  @retval EFI_SUCCESS          - Successfully update FVI data
**/
EFI_STATUS
UpdateFviInfo (
  IN UINT8                    SmbiosOemType,
  IN UINT32                   Crid
  )
{
  EFI_STATUS                  Status;
  EFI_SMBIOS_PROTOCOL         *Smbios;
  EFI_SMBIOS_HANDLE           SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER     *Record;
  INTEL_FIRMWARE_VERSION_INFO *FviData;
  FVI_STRINGS                 *FviString;

  DEBUG ((DEBUG_INFO, "UpdateFviInfo(): Update SMBIOS FVI OEM Type.\n"));

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &Smbios);
  if (Smbios == NULL || EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error locating gEfiSmbiosProtocolGuid failure. Status = %r\n", Status));
    return EFI_UNSUPPORTED;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;

  do {
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);
    if (EFI_ERROR (Status)) {
      break;
    }
    if (Record->Type == SmbiosOemType) {
      FviData   = (INTEL_FIRMWARE_VERSION_INFO *)((UINT8 *)Record + sizeof (EFI_SMBIOS_TABLE_HEADER) + sizeof (UINT8));
      FviString = (FVI_STRINGS *)((UINT8 *)Record + Record->Length);

      if (AsciiStrnCmp ((CHAR8 *) &FviString->ComponentName, PCH_CRID_STATUS_STRING, AsciiStrLen (PCH_CRID_STATUS_STRING)) == 0) {
        UpdatePchFvi (FviData, (CHAR8 **) &FviString, Crid);
      }

      if (AsciiStrnCmp ((CHAR8 *) &FviString->ComponentName, MEM_FVI_STRING, AsciiStrLen (MEM_FVI_STRING)) == 0) {
        UpdateSaFvi (FviData);
      }
      if (AsciiStrnCmp ((CHAR8 *) &FviString->ComponentName, ME_FW_FVI_STRING, AsciiStrLen (ME_FW_FVI_STRING)) == 0) {
        UpdateMeFvi (FviData, (CHAR8 **) &FviString);
      }

    }
  } while (Status == EFI_SUCCESS);

  return EFI_SUCCESS;
}

