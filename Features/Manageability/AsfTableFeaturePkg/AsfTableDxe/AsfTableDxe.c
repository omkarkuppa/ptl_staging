/** @file
  Create and install ASF ACPI table.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AsfTableUpdateProtocol.h>
#include <Protocol/AsfProtocol.h>

EFI_ACPI_DESCRIPTION_HEADER AsfTableHeader =
{
  EFI_ACPI_ASF_DESCRIPTION_TABLE_SIGNATURE,
  0,                          // overwritten after table building
  EFI_ACPI_2_0_ASF_DESCRIPTION_TABLE_REVISION
};

/**
  Calculate ASF Table Info part.

  @param[in]        AsfTable     - Pointer to ASF Table.
  @param[in,out]    TableOffset  - On input  - offset to the beginning of ASF_INFO Table
                                   On output - current size of the ASF Table
**/
VOID
CalculateAsfTableInfo (
  IN      ASF_TABLE        *AsfTable,
  IN OUT  UINT32           *TableOffset
  )
{
  EFI_ACPI_ASF_INFO        *AsfInfo;

  AsfInfo = (EFI_ACPI_ASF_INFO *) &AsfTable->TableData[*TableOffset];
  ZeroMem (AsfInfo, sizeof (EFI_ACPI_ASF_INFO));

  AsfInfo->RecordHeader.Type         = ASF_INFO_TYPE;
  AsfInfo->RecordHeader.RecordLength = sizeof (EFI_ACPI_ASF_INFO);
  AsfInfo->MinWatchDogResetValue     = ASF_INFO_MIN_WATCH_DOG_RESET_VALUE;
  AsfInfo->MinPollingInterval        = ASF_INFO_MIN_POLLING_INTERVAL;
  AsfInfo->SystemID                  = ASF_INFO_SYSTEM_ID;
  AsfInfo->IANAManufactureID         = ASF_INTEL_CONVERTED_IANA;

  *TableOffset += sizeof (EFI_ACPI_ASF_INFO);
}

/**
  Install Alert Standard Format (ASF) Table.

  @param[in] Event         - A pointer to the Event that triggered the callback.
  @param[in] Context       - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
BuildAsfTable (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                      Status;
  UINTN                           AsfTableHandle;
  EFI_ACPI_TABLE_PROTOCOL         *AcpiTable;
  ASF_TABLE                       *AsfTable;
  UINT32                          TableOffset;
  UINT32                          TableSize;
  ASF_ACPI_TABLES_UPDATE_PROTOCOL *AsfAcpiTablesUpdateProtocol;

  AsfTableHandle = 0;
  TableOffset    = 0;

  gBS->CloseEvent (Event);

  Status = gBS->LocateProtocol (&gAsfTableUpdateProtocolGuid, NULL, (VOID**) &AsfAcpiTablesUpdateProtocol);
  if (EFI_ERROR (Status)) {
    return;
  }

  TableSize = AsfAcpiTablesUpdateProtocol->GetAsfTableSize ();
  AsfTable = AllocateZeroPool (TableSize);
  if (AsfTable == NULL) {
    DEBUG ((DEBUG_ERROR, "%a Error: Could not allocate Memory\n", __FUNCTION__));
    return;
  }

  CopyMem (&AsfTable->TableHeader, &AsfTableHeader, sizeof (AsfTableHeader));
  CopyMem (AsfTable->TableHeader.OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (AsfTable->TableHeader.OemId));
  AsfTable->TableHeader.Length          = TableSize;
  AsfTable->TableHeader.OemTableId      = PcdGet64 (PcdAcpiDefaultOemTableId);
  AsfTable->TableHeader.OemRevision     = PcdGet32 (PcdAcpiDefaultOemRevision);
  AsfTable->TableHeader.CreatorId       = PcdGet32 (PcdAcpiDefaultCreatorId);
  AsfTable->TableHeader.CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);

  CalculateAsfTableInfo (AsfTable, &TableOffset);
  AsfAcpiTablesUpdateProtocol->CalculateAsfTable (AsfTable, &TableOffset);

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
  if (!EFI_ERROR (Status)) {
    Status = AcpiTable->InstallAcpiTable (
                          AcpiTable,
                          &AsfTable->TableHeader,
                          AsfTable->TableHeader.Length,
                          &AsfTableHandle
                          );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Install ASF AcpiTable failed, Status = [%r]. \n", Status));
    }
  }
  FreePool (AsfTable);
}

/**
  Entry point for the ACPI ASF Table Driver.
  Build Asf Table when Ready To Boot event is signaled.

  @param[in] ImageHandle   - Pointer to the loaded image protocol for this driver
  @param[in] SystemTable   - Pointer to the EFI System Table

  @retval EFI_SUCCESS      - Build ASF Table successfully.
  @retval other            - Some errors occur when executing this entry point.
**/
EFI_STATUS
EFIAPI
AsfTableDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_EVENT   AsfEvent;
  EFI_STATUS  Status;

  Status = EfiCreateEventReadyToBootEx (
             TPL_NOTIFY,
             BuildAsfTable,
             &ImageHandle,
             &AsfEvent
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
