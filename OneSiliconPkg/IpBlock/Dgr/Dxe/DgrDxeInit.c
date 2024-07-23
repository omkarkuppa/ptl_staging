/** @file
  DGR driver, which initializes ACPI and SMBIOS

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

@par Specification
**/

#include <Uefi.h>
#include <Protocol/DgrNvsArea.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>
#include <Pi/PiFirmwareFile.h>
#include <Pi/PiFirmwareVolume.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DgrInfoLib.h>
#include <MeSmbios.h>
#include <Library/PcdLib.h>

#if FixedPcdGetBool (PcdLedgerIslandEnable) == 1
DGR_NVS_AREA_PROTOCOL mDgrNvsAreaProtocol;

/**
  Initialize DGR Nvs Area Protocol

  @param[in] ImageHandle - Image handle of the loaded driver

  @retval EFI_SUCCESS    - Protocol successfully installed
**/
EFI_STATUS
EFIAPI
InstallDgrNvsAreaProtocol (
  IN EFI_HANDLE       ImageHandle
  )
{
  EFI_STATUS                    Status;
  UINTN                         Pages;
  EFI_PHYSICAL_ADDRESS          Address;

  ///
  /// Allocated ACPI NVS type memory for DGR Nvs Configuration.
  ///
  Pages = EFI_SIZE_TO_PAGES (sizeof (DGR_NVS_AREA));
  Address = 0xffffffff; // allocate address below 4G.

  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiACPIMemoryNVS,
                  Pages,
                  &Address
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate EfiACPIMemoryNVS pages for DGR NVS\n"));
    return Status;
  }

  mDgrNvsAreaProtocol.Area = (DGR_NVS_AREA *)(UINTN) Address;
  ZeroMem ((VOID *)mDgrNvsAreaProtocol.Area, sizeof (DGR_NVS_AREA));

  ///
  /// Install DGR NVS Area protocol
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gDgrNvsAreaProtocolGuid,
                  &mDgrNvsAreaProtocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to install DGR NVS Area protocol\n"));
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Locate DGR SSDT table defined in DgrAcpiTables

  @param[out] DgrSsdtTable - Pointer to located ACPI table

  @retval     EFI_SUCCESS    DGR SSDT table located succesfully
  @retval     EFI_NOT_FOUND  Failed to locate DGR SSDT table
**/
EFI_STATUS
LocateDgrSsdtTable (
  OUT EFI_ACPI_DESCRIPTION_HEADER** DgrSsdtTable
  )
{
  EFI_STATUS                     Status;
  UINTN                          Index;
  UINTN                          Size;
  UINT32                         FvStatus;
  EFI_FV_FILETYPE                FileType;
  EFI_FV_FILE_ATTRIBUTES         Attributes;
  UINTN                          Instance;
  EFI_ACPI_COMMON_HEADER         *CurrentTable;
  EFI_HANDLE                     *HandleBuffer;
  UINTN                          NumberOfHandles;
  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol;

  FwVol = NULL;
  HandleBuffer = NULL;

  ///
  /// Locate Firmware volume protocol.
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate Firmware volume protocol\n"));
    if (HandleBuffer != NULL) {
      FreePool (HandleBuffer);
    }
    return Status;
  }

  ///
  /// Look for FV with ACPI storage file
  ///
  FwVol = NULL;
  for (Index = 0; Index < NumberOfHandles; Index++) {

    ///
    /// Get the protocol on this handle
    /// This should not fail because of LocateHandleBuffer
    ///
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **) &FwVol
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to retrieve Firmware volume from handle\n"));
      return Status;
    }

    if (FwVol == NULL) {
      DEBUG ((DEBUG_ERROR, "Failed to locate Firmware volume containing DGR SSDT table\n"));
      return EFI_NOT_FOUND;
    }

    ///
    /// See if it has the ACPI storage file
    ///
    Size      = 0;
    FvStatus  = 0;
    Status = FwVol->ReadFile (
                      FwVol,
                      &gDgrAcpiTableStorageGuid,
                      NULL,
                      &Size,
                      &FileType,
                      &Attributes,
                      &FvStatus
                      );

    ///
    /// If we found it, then we are done
    ///
    if (Status == EFI_SUCCESS) {
      break;
    }
  }

  ///
  /// Our exit status is determined by the success of the previous operations
  /// If the protocol was found, Instance already points to it.
  /// Free any allocated buffers
  ///
  FreePool (HandleBuffer);

  ///
  /// Read table from the storage file.
  ///
  Instance      = 0;
  CurrentTable  = NULL;
  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                      FwVol,
                      &gDgrAcpiTableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &CurrentTable,
                      &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR (Status)) {
      if ( (((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->OemTableId) == SIGNATURE_64 ('D', 'g', 'r', 'S', 's', 'd', 't', 0) ) {
        *DgrSsdtTable = ((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable);
        return EFI_SUCCESS;
      }

      Instance++;
      CurrentTable = NULL;
    }
  }

  DEBUG ((DEBUG_ERROR, "Failed to locate DGR SSDT table within the Firmware volume\n"));
  return EFI_NOT_FOUND;
}

/**
  Update DGR NVS area variables in DgrSsdtTable

  @param[in] DgrSsdtTable - pointer to DGR ACPI table

  @retval EFI_SUCCESS             DGR NVS area updated succesfully
  @retval EFI_INVALID_PARAMETER   Ptr to DgrSsdtTable is NULL
  @retval EFI_NOT_FOUND           Failed to locate DGNV operating region
  @retval EFI_VOLUME_CORRUPTED     NV operating region corrupted
**/
EFI_STATUS
PatchDgrSsdtTable (
  IN EFI_ACPI_DESCRIPTION_HEADER*   DgrSsdtTable
  )
{
  UINT8       *CurrPtr;
  UINT32      *Signature;

  if (DgrSsdtTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ///
  /// Locate the SSDT package
  /// OperationRegion space (14 bytes) = 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1 + sizeof (UINT16)
  ///
  for (CurrPtr = (UINT8 *) DgrSsdtTable; CurrPtr <= ((UINT8 *) DgrSsdtTable + DgrSsdtTable->Length - 14); CurrPtr++) {
    Signature = (UINT32 *) (CurrPtr + 1);

    ///
    /// Update the DGR NVS area
    ///
    if ((*CurrPtr == AML_EXT_REGION_OP) &&
        (*Signature == SIGNATURE_32 ('D', 'G', 'N', 'V'))) {
      if ((*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) != 0xFFFF0000) ||
          (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) != 0xAA55)) {
        DEBUG ((DEBUG_ERROR, "Invalid DGR DGNV operating region size or offset\n"));
        return EFI_VOLUME_CORRUPTED;
      }

      ///
      /// DGR Nvs Area address
      ///
      *(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) = (UINT32) (UINTN) mDgrNvsAreaProtocol.Area;

      ///
      /// DGR Nvs Area size
      ///
      *(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) = sizeof (DGR_NVS_AREA);
      return EFI_SUCCESS;
    }
  }

  DEBUG ((DEBUG_ERROR, "Failed to locate DGR DGNV operating region in DgrSsdt\n"));
  return EFI_NOT_FOUND;
}

/**
  Initialize DGR NVS area variables and install ACPI table

  @retval EFI_SUCCESS     DGR NVS area updated succesfully
**/
EFI_STATUS
DgrAcpiTable(
  VOID
  )
{
  EFI_STATUS                     Status;
  UINTN                          AcpiTableHandle;
  EFI_ACPI_DESCRIPTION_HEADER    *DgrSsdtTable;
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTableProtocol;

  Status = LocateDgrSsdtTable (&DgrSsdtTable);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// Update the DGR SSDT table
  ///
  Status = PatchDgrSsdtTable (DgrSsdtTable);
  if (EFI_ERROR (Status)) {
    FreePool (DgrSsdtTable);
    return Status;
  }

  ///
  /// Locate ACPI table protocol
  ///
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTableProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate AcpiTableProtocol\n"));
    FreePool (DgrSsdtTable);
    return Status;
  }

  AcpiTableHandle = 0;
  Status = AcpiTableProtocol->InstallAcpiTable (
                         AcpiTableProtocol,
                         DgrSsdtTable,
                         DgrSsdtTable->Length,
                         &AcpiTableHandle
                         );
  FreePool (DgrSsdtTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to install DgrSsdtTable\n"));
  }

  return Status;
}
#endif
/**
  Returns the SMBIOS table with the specified type.

  @param[in]        Type            The type of the SMBIOS table to be searched.
  @param[out]       Table           On Success, the Table pointer contains the pointer of SMBIOS table of given SMBIOS Type.

  @retval EFI_SUCCESS               Record was found.
  @retval EFI_INVALID_PARAMETER     Table pointer is NULL.
  @retval EFI_NOT_FOUND             Record cannot be found.
**/
EFI_STATUS
GetSmbiosTable (
  IN  SMBIOS_TYPE              Type,
  OUT EFI_SMBIOS_TABLE_HEADER  **Table
  )
{
  EFI_STATUS                 Status;
  EFI_SMBIOS_HANDLE          SmbiosHandle;
  EFI_SMBIOS_PROTOCOL        *Smbios;
  EFI_SMBIOS_TABLE_HEADER    *Record;

  if (Table == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Table = NULL;
  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID**) &Smbios
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  do {
    Status = Smbios->GetNext (
                       Smbios,
                       &SmbiosHandle,
                       NULL,
                       &Record,
                       NULL
                       );
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }
  } while (Record->Type != Type);

  *Table = Record;

  return EFI_SUCCESS;
}

/**
  Call Back function to Update SMBIOS table 131.
  @param  Event                 Event whose notification function is being invoked.
  @param  Context               The pointer to the notification function's context,
                                which is implementation-dependent.
**/
VOID
EFIAPI
UpdateSmbiosType131
(
  IN EFI_EVENT                Event,
  IN VOID                     *Context
  )
{
  EFI_STATUS                   Status;
  EFI_SMBIOS_TABLE_HEADER      *Record;
  SMBIOS_TABLE_TYPE131         *SmbiosTableType131Data;
  UINT8                        NiftyRockSupport;
  
  Status = GetSmbiosTable (MESMBIOS_TYPE_131, &Record);
  DEBUG ((DEBUG_INFO, "UpdateSmbiosType131 GetSmbiosTable %r\n",Status));
  if (!EFI_ERROR (Status)) {
    SmbiosTableType131Data = (SMBIOS_TABLE_TYPE131*) Record;
    NiftyRockSupport = NiftyRockSupportLevel();
    if (NiftyRockSupport >= NR_PPAM_11_SUPPORT) {
      SmbiosTableType131Data->BiosCapabilities.IsrdIssrCapability = 1;
    }
#if FixedPcdGetBool (PcdLedgerIslandEnable) == 1
    if (NiftyRockSupport == NR_PPAM_11_SUPPORT_WITH_LEDGER_ISLAND) {\
      SmbiosTableType131Data->BiosCapabilities.LiCapability = 1;
    }
#endif
  }
}

/**
  Entry point for the DGR Acpi Init driver.

  @param[in] ImageHandle         Image handle of this driver.
  @param[in] SystemTable         Global system service table.

  @retval EFI_SUCCESS     Initialization complete.
  @retval EFI_UNSUPPORTED Feature is disabled
  @retval EFI_ERROR       Driver exits abnormally
**/
EFI_STATUS
EFIAPI
DgrDxeInitEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   EndOfDxeEvent;
  
  DEBUG ((DEBUG_INFO, "DgrDxeInit Entry\n"));
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  UpdateSmbiosType131,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  DEBUG ((DEBUG_INFO, "UpdateSmbiosType131 CreateEventEx %r\n",Status));
  ASSERT_EFI_ERROR (Status);

#if FixedPcdGetBool (PcdLedgerIslandEnable) == 1
  if (NiftyRockSupportLevel () == NR_PPAM_11_SUPPORT_WITH_LEDGER_ISLAND) {

    Status = InstallDgrNvsAreaProtocol (ImageHandle);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to install DgrNvsAreaProtocol\n"));
      ASSERT_EFI_ERROR(Status);
      return Status;
    }

    Status = DgrAcpiTable();
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to initialize DgrAcpiTable\n"));
      ASSERT_EFI_ERROR(Status);
    }
  }
#endif
  return Status;
}
