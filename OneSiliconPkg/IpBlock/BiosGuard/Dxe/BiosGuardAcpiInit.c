/** @file
  Bios Guard driver, which initializes ACPI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#include <Protocol/BiosGuardNvsArea.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>
#include <Pi/PiFirmwareFile.h>
#include <Pi/PiFirmwareVolume.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/CpuPlatformLib.h>

BIOSGUARD_NVS_AREA_PROTOCOL mBiosGuardNvsAreaProtocol;

/**
  Initialize BIOS Guard Nvs Area Protocol

  @param[in] ImageHandle - Image handle of the loaded driver

  @retval EFI_SUCCESS    - thread can be successfully created
**/
EFI_STATUS
EFIAPI
InstallBiosGuardNvsAreaProtocol (
  IN EFI_HANDLE       ImageHandle
  )
{
  EFI_STATUS                    Status;
  UINTN                         Pages;
  EFI_PHYSICAL_ADDRESS          Address;

  ///
  /// Allocated ACPI NVS type memory for BiosGuard Nvs Configuration.
  ///
  Pages = EFI_SIZE_TO_PAGES (sizeof (BIOSGUARD_NVS_AREA));
  Address = 0xffffffff; // allocate address below 4G.

  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiACPIMemoryNVS,
                  Pages,
                  &Address
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "EfiACPIMemoryNVS pages for BIOS Guard NVS not Allocated\n"));
    return Status;
  }

  mBiosGuardNvsAreaProtocol.Area = (BIOSGUARD_NVS_AREA *)(UINTN) Address;
  ZeroMem ((VOID *)mBiosGuardNvsAreaProtocol.Area, sizeof (BIOSGUARD_NVS_AREA));

  ///
  /// Install BIOS Guard NVS Area protocol
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gBiosGuardNvsAreaProtocolGuid,
                  &mBiosGuardNvsAreaProtocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to install BIOS Guard NVS Area protocol\n"));
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Locate Bios Guard SSDT table defined in BiosGuardAcpiTables

  @param[out] BiosGuardSsdtTable - pointer to located ACPI table

  @retval EFI_SUCCESS     Bios Guard SSDT table located succesfully
  @retval EFI_NOT_FOUND   Failed to locate Bios Guard SSDT table
**/
EFI_STATUS
LocateBiosGuardSsdtTable (
  OUT EFI_ACPI_DESCRIPTION_HEADER** BiosGuardSsdtTable
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

  ///
  /// Locate Firmware volume protocol.
  /// There is little chance we can't find an FV protocol
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
    FreePool(HandleBuffer);
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
      FreePool(HandleBuffer);
      return Status;
    }

    ///
    /// See if it has the ACPI storage file
    ///
    Size      = 0;
    FvStatus  = 0;
    Status = FwVol->ReadFile (
                      FwVol,
                      &gBiosGuardAcpiTableStorageGuid,
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
  /// Sanity check that we found our data file
  ///
  if (FwVol == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to locate Firmware volume containing Bios Guard SSDT table\n"));
    return EFI_NOT_FOUND;
  }

  ///
  /// Read table from the storage file.
  ///
  Instance      = 0;
  CurrentTable  = NULL;
  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                      FwVol,
                      &gBiosGuardAcpiTableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &CurrentTable,
                      &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR (Status)) {
      if ( (((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->OemTableId) == SIGNATURE_64 ('B', 'i', 'g', 'S', 's', 'd', 't', 0) ) {
        *BiosGuardSsdtTable = ((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable);
        return EFI_SUCCESS;
      }

      Instance++;
      CurrentTable = NULL;
    }
  }

  DEBUG ((DEBUG_ERROR, "Failed to locate Bios Guard SSDT table within the Firmware volume\n"));
  return EFI_NOT_FOUND;
}

/**
  Update Bios Guard NVS area variables in BiosGuardSsdtTable

  @param[in] BiosGuardSsdtTable - pointer to Bios Guard ACPI table

  @retval EFI_SUCCESS             Bios Guard NVS area updated succesfully
  @retval EFI_INVALID_PARAMETER   Ptr to BiosGuardSsdtTable is NULL
  @retval EFI_NOT_FOUND           Failed to locate Bios Guard BGNV operating region
  @retval EFI_VOLUME_CORRUPTED    Bios Guard NV operating region corrupted
**/
EFI_STATUS
PatchBiosGuardSsdtTable (
  IN EFI_ACPI_DESCRIPTION_HEADER*   BiosGuardSsdtTable
  )
{
  UINT8       *CurrPtr;
  UINT32      *Signature;

  if (BiosGuardSsdtTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ///
  /// Locate the SSDT package
  /// OperationRegion space (14 bytes) = 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1 + sizeof (UINT16)
  ///
  for (CurrPtr = (UINT8 *) BiosGuardSsdtTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER); CurrPtr <= ((UINT8 *) BiosGuardSsdtTable + BiosGuardSsdtTable->Length - 14); CurrPtr++) {
    Signature = (UINT32 *) (CurrPtr + 1);

    ///
    /// Update the BIOS Guard NVS area
    ///
    if ((*CurrPtr == AML_EXT_REGION_OP) &&
        (*Signature == SIGNATURE_32 ('B', 'G', 'N', 'V'))) {
      if ((*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) != 0xFFFF0000) ||
          (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) != 0xAA55)) {
        DEBUG ((DEBUG_ERROR, "Invalid Bios Guard BGNV operating region size or offset\n"));
        return EFI_VOLUME_CORRUPTED;
      }

      ///
      /// BIOS Guard Nvs Area address
      ///
      *(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) = (UINT32) (UINTN) mBiosGuardNvsAreaProtocol.Area;

      ///
      /// BIOS Guard Nvs Area size
      ///
      *(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) = sizeof (BIOSGUARD_NVS_AREA);
      return EFI_SUCCESS;
    }
  }

  DEBUG ((DEBUG_ERROR, "Failed to locate Bios Guard BGNV operating region in BigSsdt\n"));
  return EFI_NOT_FOUND;
}

/**
  Initialize Bios Guard NVS area variables and install ACPI table

  @retval EFI_SUCCESS     Bios Guard NVS area updated succesfully
**/
EFI_STATUS
InitBiosGuardAcpiTable (
  VOID
  )
{
  EFI_STATUS                     Status;
  UINTN                          AcpiTableHandle;
  EFI_ACPI_DESCRIPTION_HEADER    *BiosGuardSsdtTable;
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTableProtocol;

  Status = LocateBiosGuardSsdtTable(&BiosGuardSsdtTable);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// Update the Bios Guard SSDT table
  ///
  Status = PatchBiosGuardSsdtTable(BiosGuardSsdtTable);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// Locate ACPI table protocol
  ///
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTableProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate AcpiTableProtocol\n"));
    return Status;
  }

  AcpiTableHandle = 0;
  Status = AcpiTableProtocol->InstallAcpiTable (
                         AcpiTableProtocol,
                         BiosGuardSsdtTable,
                         BiosGuardSsdtTable->Length,
                         &AcpiTableHandle
                         );
  FreePool (BiosGuardSsdtTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to install BiosGuardSsdtTable\n"));
  }

  return Status;
}

/**
  Entry point for the Bios Guard Acpi Init driver.

  @param[in] ImageHandle         Image handle of this driver.
  @param[in] SystemTable         Global system service table.

  @retval EFI_SUCCESS     Initialization complete.
  @retval EFI_NOT_READY   Bios Guard is disabled
  @retval EFI_ERROR       Driver exits abnormally
**/
EFI_STATUS
EFIAPI
BiosGuardAcpiInitEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS Status;

  if (IsBiosGuardEnabled()) {
    Status = InstallBiosGuardNvsAreaProtocol(ImageHandle);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to install BiosGuardNvsAreaProtocol\n"));
      ASSERT_EFI_ERROR(Status);
      return Status;
    }

    Status = InitBiosGuardAcpiTable();
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to initialize InitBiosGuardAcpiTable\n"));
      ASSERT_EFI_ERROR(Status);
    }

    return Status;
  } else {
    return EFI_NOT_READY;
  }
}
