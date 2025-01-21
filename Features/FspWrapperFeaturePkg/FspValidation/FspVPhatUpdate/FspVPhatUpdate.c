/** @file
  DXE driver to create FspV PHAT table.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/AcpiTable.h>
#include <Library/HobLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/FspVInfoLib.h>
#include <Protocol/AcpiSystemDescriptionTable.h>

GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_TABLE_PROTOCOL  *mAcpiTableProtocol  = NULL;

/**
  This function calculates and updates an UINT8 checksum.

  @param[in]  Buffer          Pointer to buffer to checksum
  @param[in]  Size            Number of bytes to checksum
**/
VOID
AcpiPlatformChecksum (
  IN UINT8  *Buffer,
  IN UINTN  Size
  )
{
  UINTN ChecksumOffset;
  ChecksumOffset = OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER, Checksum);

  //
  // Set checksum to 0 first
  //
  Buffer[ChecksumOffset] = 0;

  //
  // Update checksum value
  //
  Buffer[ChecksumOffset] = CalculateCheckSum8 (Buffer, Size);
}

/**
  This function scan ACPI table entry point.
  @return ACPI table entry pointer
**/
VOID *
SearchAcpiTablePointer (
  VOID
  )
{
  EFI_ACPI_3_0_ROOT_SYSTEM_DESCRIPTION_POINTER  *Rsdp;
  EFI_ACPI_DESCRIPTION_HEADER                   *Entry;
  EFI_STATUS                                    Status;

  Entry = NULL;

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &mAcpiTableProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "locate gEfiAcpiTableProtocolGuid failed, status[%x].\n", Status));
    return NULL;
  }

  //
  // Find ACPI table RSD_PTR from the system table.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiAcpiTableGuid, (VOID **) &Rsdp);
  if (EFI_ERROR (Status)) {
    Status = EfiGetSystemConfigurationTable (&gEfiAcpi10TableGuid, (VOID **) &Rsdp);
  }

  if (EFI_ERROR (Status) || (Rsdp == NULL)) {
    DEBUG ((DEBUG_INFO, "Can't find RSD_PTR from system table! \n"));
    return NULL;
  } else if (Rsdp->Revision >= EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION && Rsdp->XsdtAddress != 0) {
    Entry = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) Rsdp->XsdtAddress;
  } else if (Rsdp->RsdtAddress != 0) {
    Entry = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) Rsdp->RsdtAddress;
  }

  if (Entry == NULL) {
    DEBUG ((DEBUG_INFO, "Both XsdtAddress and RsdtAddress are NULL! \n"));
    return NULL;
  }

  return Entry;
}

/**
  Get FspV data from GuidHobs.
  @param[in, out] InformationBlock        A pointer to the buffer which follows up with hob record.
                                          May be NULL in order to determine the size buffer needed.
  @param[out]     InformationBlockSize    The size of the InformationBlock in bytes.
**/
EFI_STATUS
GetFspVHobData (
  IN OUT UINT8  **InformationBlock
  )
{
  FSPV_INFO_HOB      *FspVInfo;
  EFI_HOB_GUID_TYPE  *GuidHob;
  UINT8*             TempFspVBuffHeader;
  UINTN              Length;
  UINTN              InformationBlockSize;

  GuidHob = GetFirstGuidHob (&gFspVInfoGuid);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "FspV info Hob was not found\n"));
    return EFI_NOT_FOUND;
  }
  TempFspVBuffHeader = NULL;
  InformationBlockSize = 0;
  while (GuidHob != NULL)
  {
    FspVInfo = (FSPV_INFO_HOB*) GET_GUID_HOB_DATA (GuidHob);
    ASSERT (GuidHob != NULL);
    DEBUG ((DEBUG_INFO, "  PHAT TestName          - %s\n", FspVInfo->TestCaseName));
    DEBUG ((DEBUG_INFO, "  PHAT IsTestPass        - %d\n", FspVInfo->IsTestPassed));
    DEBUG ((DEBUG_INFO, "  PHAT FailureSignature  - %s\n", FspVInfo->FailureSignature));
    DEBUG ((DEBUG_INFO, "       ******\n"));

    TempFspVBuffHeader = *InformationBlock + InformationBlockSize;
    Length = (UINTN) sizeof (FSPV_INFO_HOB);
    InformationBlockSize += Length;
    CopyMem (TempFspVBuffHeader, (UINT8 *) FspVInfo, Length);
    GuidHob = GET_NEXT_HOB (GuidHob);
    GuidHob = GetNextGuidHob (&gFspVInfoGuid, GuidHob);
  }
  return EFI_SUCCESS;
}

/**
  Initialize the header of the Platform Health Assessment Table.
  @param[out]  Header     The header of the ACPI Table.
  @param[in]   OemId      The OEM ID.
  @param[in]   OemTableId The OEM table ID for the Phat.
**/
VOID
InitFspVPhatTableHeader (
  OUT EFI_ACPI_DESCRIPTION_HEADER   *Header,
  IN  UINT8                         *OemId,
  IN  UINT64                        *OemTableId
  )
{
  ZeroMem (Header, sizeof (EFI_ACPI_DESCRIPTION_HEADER));

  Header->Signature = EFI_ACPI_6_5_PLATFORM_HEALTH_ASSESSMENT_TABLE_SIGNATURE;
  //
  // tatal length (FVI, Driver Health).
  //
  Header->Length    = 0;
  Header->Revision  = EFI_ACPI_6_5_PLATFORM_HEALTH_ASSESSMENT_TABLE_REVISION;
  Header->Checksum  = 0;
  CopyMem (Header->OemId, OemId, sizeof (Header->OemId));
  CopyMem (&Header->OemTableId, OemTableId, sizeof (UINT64));
  Header->OemRevision     = PcdGet32 (PcdAcpiDefaultOemRevision);
  Header->CreatorId       = PcdGet32 (PcdAcpiDefaultCreatorId);
  Header->CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);
}

/**
  Initialize the Platform Health Assessment Table.
  Convert AIP data block to telementry ACPI style, and publish it.
  @param[in]   InfoBlock     Point to FspV data block.
  @param[in]   InfoBlockSize The size of FspV data.
  @retval EFI_SUCCESS           Success
  @retval EFI_OUT_OF_RESOURCES  Status code not supported.
  @retval EFI_INVALID_PARAMETER Either AcpiTableBuffer is NULL,
                                TableKey is NULL, or
                                AcpiTableBufferSize and the size
                                field embedded in the ACPI table
                                pointed to by AcpiTableBuffer
                                are not in sync.
  @retval EFI_ACCESS_DENIED     The table signature matches a table already
                                present in the system and platform policy
                                does not allow duplicate tables of this type.
**/
EFI_STATUS
InstallFspVTelemetryTable (
  IN VOID   *InfoBlock,
  IN UINTN  InfoBlockSize
  )
{
  EFI_STATUS                   Status;
  EFI_ACPI_DESCRIPTION_HEADER  *AcpiEntry;
  EFI_ACPI_SDT_PROTOCOL        *AcpiSdtProtocol;
  EFI_ACPI_DESCRIPTION_HEADER  *PhatHeader;
  UINT8                        *PhatTable;
  UINT32                       PhatLen;
  UINTN                        TableIndex;
  UINT8                        *TableHeader;
  EFI_ACPI_TABLE_VERSION       TableVersion;
  UINTN                        TableKey;

  Status      = EFI_SUCCESS;
  TableIndex  = 0;
  TableKey    = 0;
  TableHeader = NULL;

  AcpiEntry = SearchAcpiTablePointer ();
  if (AcpiEntry == NULL) {
    DEBUG((DEBUG_ERROR, "ACPI table pointer not found\n"));
    return EFI_NOT_FOUND;
  }

  //
  // Locate the EFI_ACPI_SDT_PROTOCOL.
  //
  Status = gBS->LocateProtocol (
                  &gEfiAcpiSdtProtocolGuid,
                  NULL,
                  (VOID **)&AcpiSdtProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to locate AcpiSdt with status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Search ACPI table for PHAT
  while (!EFI_ERROR (Status)) {
    Status = AcpiSdtProtocol->GetAcpiTable (
                                 TableIndex,
                                 (EFI_ACPI_SDT_HEADER **)&TableHeader,
                                 &TableVersion,
                                 &TableKey
                                 );
    if (!EFI_ERROR (Status)) {
      TableIndex++;

      if (((EFI_ACPI_SDT_HEADER *) TableHeader)->Signature ==
          EFI_ACPI_6_5_PLATFORM_HEALTH_ASSESSMENT_TABLE_SIGNATURE)
      {
        DEBUG ((DEBUG_INFO, "[%a] Existing Phat AcpiTable is found.\n", __FUNCTION__));
        break;
      }
    }
  }

  if (!EFI_ERROR (Status)) {
    //
    // A PHAT is already in the ACPI table, update existing table and re-install
    //
    PhatHeader = (EFI_ACPI_DESCRIPTION_HEADER *) TableHeader;
    PhatLen    = PhatHeader->Length + (UINT32) InfoBlockSize;
    PhatTable  = (UINT8 *) AllocateZeroPool (PhatLen);
    if (PhatTable == NULL) {
      DEBUG ((DEBUG_ERROR, "[%a] Failed to allocated new PHAT pool with.\n", __FUNCTION__));
      return EFI_OUT_OF_RESOURCES;
    }

    // Copy original table content to the new PHAT table pool
    CopyMem (PhatTable, TableHeader, PhatHeader->Length);

    // Append InfoBlock in the end of the origin PHAT
    CopyMem (PhatTable + PhatHeader->Length, InfoBlock, InfoBlockSize);

    // Update the PHAT head pointer.
    PhatHeader = (EFI_ACPI_DESCRIPTION_HEADER *) PhatTable;

    // Update the length field to found table plus appended new data
    PhatHeader->Length = PhatLen;

    // Uninstall the origin PHAT from the ACPI table.
    Status = mAcpiTableProtocol->UninstallAcpiTable (
                                    mAcpiTableProtocol,
                                    TableKey
                                    );
    ASSERT_EFI_ERROR (Status);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[%a] Failed to uninstall existing PHAT ACPI table with status: %r\n", __FUNCTION__, Status));
      FreePool (PhatTable);
      return Status;
    }
  } else {
    //
    // PHAT ACPI table does not exist, install new one
    //
    PhatTable = AllocateZeroPool (InfoBlockSize + sizeof (EFI_ACPI_DESCRIPTION_HEADER));
    if (PhatTable == NULL) {
      DEBUG ((DEBUG_ERROR, "[%a] Failed to allocate new PHAT pool.\n", __FUNCTION__));
      return EFI_OUT_OF_RESOURCES;
    }
    PhatHeader = (EFI_ACPI_DESCRIPTION_HEADER *) PhatTable;

    // Initialize the header of the Platform Health Assessment Table.
    InitFspVPhatTableHeader (PhatHeader, AcpiEntry->OemId, &AcpiEntry->OemTableId);

    PhatHeader->Length = sizeof (EFI_ACPI_DESCRIPTION_HEADER) + (UINT32)InfoBlockSize;

    // Connect a telemetry data to ACPI table header.
    CopyMem (PhatTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER), InfoBlock, InfoBlockSize);
  }
  // Update table checksum
  AcpiPlatformChecksum ((UINT8 *) PhatTable, ((EFI_ACPI_DESCRIPTION_HEADER *) PhatHeader)->Length);

  // Install or update the Phat table.
  Status = mAcpiTableProtocol->InstallAcpiTable (
                                 mAcpiTableProtocol,
                                 PhatTable,
                                 PhatHeader->Length,
                                 &TableKey
                                 );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[%a] Install FSPV Phat AcpiTable failed, Status = [%r]. \n", __FUNCTION__, Status));
  }

  if (PhatTable != NULL) {
    FreePool (PhatTable);
  }

  DEBUG ((DEBUG_INFO, "[%a] Install FSPV Phat table, status: %r \n", __FUNCTION__, Status));
  return Status;
}

/**
  Get Total Size Of FspV GuidHobs.

   @retval  TotalSizeOfFspVInfo  Total Size Of FspV GuidHobs.
**/
UINTN
GetTotalSizeOfFspVGuidHobs (
  VOID
  )
{
  UINT8              HobCount;
  EFI_HOB_GUID_TYPE  *GuidHob;
  UINTN              FspVInfoTotalSize;

  HobCount = 0;
  GuidHob = GetFirstGuidHob (&gFspVInfoGuid);
  while (GuidHob != NULL)
  {
    HobCount += 1;
    GuidHob = GET_NEXT_HOB (GuidHob);
    GuidHob = GetNextGuidHob (&gFspVInfoGuid, GuidHob);
  }
  FspVInfoTotalSize = HobCount * sizeof (FSPV_INFO_HOB);
  DEBUG((DEBUG_INFO, "Total FspV Info Hobs = %d, Total size = %d\n", HobCount, FspVInfoTotalSize));
  return FspVInfoTotalSize;
}

/**
  Collect AIP data including FVI and driver health, then convert them to Acpi format.

  @param[in]  Event   Pointer to this event
  @param[in]  Context Event handler private data.
**/
VOID
EFIAPI
InstallFspVPhatTableCallback (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  EFI_STATUS                   Status;
  UINTN                        FspVGuidHobSize;
  UINT8                        *AcpiFspVBuffer;

  FspVGuidHobSize = 0;
  AcpiFspVBuffer  = NULL;

  //
  // Get Total Size of FspV Guid Hobs
  //
  FspVGuidHobSize = GetTotalSizeOfFspVGuidHobs ();
  AcpiFspVBuffer = AllocateZeroPool (FspVGuidHobSize);

  if (AcpiFspVBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "EFI_OUT_OF_RESOURCES : AcpiFspVBuffer allocate failed.\n"));
    return;
  }

  //
  // Get FspV Info HOB data.
  //
  Status = GetFspVHobData (&AcpiFspVBuffer);
  if (!(EFI_ERROR (Status))) {
    //
    // Initialize the Platform Health Assessment Table for FspV info.
    //
    Status = InstallFspVTelemetryTable (AcpiFspVBuffer, FspVGuidHobSize);
  }

  if (!(EFI_ERROR (Status))) {
    gBS->CloseEvent (Event);
  }

  DEBUG ((DEBUG_INFO, "Install FspV PHAT table, Status = [%r] \n", Status));
  if (AcpiFspVBuffer != NULL) {
    FreePool (AcpiFspVBuffer);
  }
}

/**
  This is driver entry point to register the notification event.
  @param[in] ImageHandle  A handle for the image that is initializing this driver
  @param[in] SystemTable  A pointer to the EFI system table

  @retval    EFI_SUCCESS  The initialization finished successfully.
**/
EFI_STATUS
EFIAPI
FspVPhatUpdate (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_EVENT   Event;
  EFI_STATUS  Status;

  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             InstallFspVPhatTableCallback,
             NULL,
             &Event
             );
  DEBUG ((DEBUG_INFO, "Create ReadyToBoot event to build FspV PHAT table: %r\n", Status));

  return EFI_SUCCESS;
}
