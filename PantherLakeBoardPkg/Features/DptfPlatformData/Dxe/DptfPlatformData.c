/** @file
  This DXE driver configures and supports Intel(R) Dynamic Tuning Technology.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include "DptfPlatformData.h"

EFI_STATUS
EFIAPI
InitializePlatformData (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  )
/**
@brief
  This procedure does all the Intel(R) Dynamic Tuning Technology initialization and loads the ACPI tables.

  @param[in] ImageHandle   - The firmware allocated handle to the Driver Image
  @param[in] SystemTable   - Pointer to the EFI System Table

  @retval EFI_SUCCESS - The driver installed/initialized correctly.
**/
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "Intel(R) Platform Data : LoadAcpiTables\n"));
  Status = LoadAcpiTables ();

  return Status;
}

EFI_STATUS
EFIAPI
LoadAcpiTables(
  VOID
  )
/**
@brief
  This procedure loads the ACPI SSDT tables.

**/
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         NumberOfHandles;
  UINTN                         Index;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  INTN                          Instance;
  EFI_ACPI_COMMON_HEADER        *Table;
  UINTN                         Size;
  EFI_FV_FILETYPE               FileType;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  UINT32                        FvStatus;
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader;
  UINTN                         TableHandle;
  BOOLEAN                       LoadTable;
#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
  DPTF_CONFIG                   DptfConfig;
  UINT32                        VariableAttributes;
  UINTN                         VariableSize;
  SETUP_DATA                    SetupData;
#endif

  FwVol = NULL;
  Table = NULL;

  ///
  /// Locate FV protocol.
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "DPTF: No Efi Firmware Volume Protocol available.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "DPTF: Efi Firmware Volume Protocol is loaded.\n"));
  }
  ASSERT_EFI_ERROR (Status);

  ///
  /// Look for FV with ACPI storage file
  ///
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
    ASSERT_EFI_ERROR (Status);

    if ((Status == EFI_SUCCESS) && (FwVol != NULL)) {
      ///
      /// See if it has the ACPI storage file
      ///
      Size      = 0;
      FvStatus  = 0;
      Status = FwVol->ReadFile (
                      FwVol,
                      &gPlatformDataAcpiTableStorageGuid,
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
        DEBUG ((DEBUG_INFO, "DPTF: Intel(R) Platform Data Acpi Table Storage for RVP is found.\n"));
        break;
      }
    }
  }

  ///
  /// Our exit status is determined by the success of the previous operations
  /// If the protocol was found, Instance already points to it.
  ///
  /// Free any allocated buffers
  ///
  FreePool (HandleBuffer);

  ///
  /// Sanity check that we found our data file
  ///
  ASSERT (FwVol);
  if (FwVol == NULL) {
    return EFI_NOT_FOUND;
  }

  ///
  /// Find the Table protocol
  ///
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);

  ///
  /// Read tables from the storage file.
  ///
  Instance = 0;

  while (Status == EFI_SUCCESS) {
    ///
    /// Read the ACPI tables
    ///
    Status = FwVol->ReadSection (
                          FwVol,
                          &gPlatformDataAcpiTableStorageGuid,
                          EFI_SECTION_RAW,
                          Instance,
                          (VOID **) &Table,
                          &Size,
                          &FvStatus
                          );

    if (!EFI_ERROR (Status)) {
      ///
      /// Check the table size is at least as large as an EFI_ACPI_COMMON_HEADER
      ///
      if (Size < sizeof (EFI_ACPI_COMMON_HEADER)) {
         if(Table != NULL) {
          gBS->FreePool (Table);
          Table = NULL;
         }
        return EFI_BUFFER_TOO_SMALL;
      }

      LoadTable = FALSE;
      TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;

      switch (((EFI_ACPI_DESCRIPTION_HEADER*) TableHeader)->OemTableId) {

      case SIGNATURE_64 ('P', 'D', 'a', 't', 'T', 'a', 'b', 'l'):
        ///
        /// This is Intel(R) Dynamic Tuning Technology SSDT. Intel(R) Dynamic Tuning Technology should be enabled if we reach here so load the table.
        ///
        LoadTable = TRUE;
        DEBUG ((DEBUG_INFO, "Intel(R) Platform Data: Found Intel(R) Platform Data SSDT signature.\n"));
        break;

      default:
        break;
      }

      ///
      /// Add the table
      ///
      if (LoadTable) {
        TableHandle = 0;
        ///
        /// Check the length field isn't larger than the size read in section
        ///
        if (Table->Length > Size) {
          gBS->FreePool (Table);
          Table = NULL;
          return EFI_BAD_BUFFER_SIZE;
        }
        Status = AcpiTable->InstallAcpiTable (
                                  AcpiTable,
                                  Table,
                                  Table->Length,
                                  &TableHandle
                                  );
      }

      ///
      /// Increment the instance
      ///
      Instance++;
      gBS->FreePool (Table);
      Table = NULL;
    }
  }

#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
  VariableSize = sizeof (SETUP_DATA);
  ZeroMem (&SetupData, sizeof (SETUP_DATA));
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  &VariableAttributes,
                  &VariableSize,
                  &SetupData
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  VariableSize = sizeof (DPTF_CONFIG);
  Status = gRT->GetVariable (
                  L"DptfConfig",
                  &gDptfConfigVariableGuid,
                  &VariableAttributes,
                  &VariableSize,
                  &DptfConfig
                  );

  if (EFI_ERROR (Status)) {
    // DptfConfig not created yet. No need to update EnableDptf with EnableIpf value.
    return EFI_SUCCESS;
  }

  DptfConfig.EnableDptf = SetupData.IpfEnable;

  VariableSize = sizeof (DPTF_CONFIG);
  Status = gRT->SetVariable (
                  L"DptfConfig",
                  &gDptfConfigVariableGuid,
                  VariableAttributes,
                  VariableSize,
                  &DptfConfig
                  );

#endif
  return EFI_SUCCESS;
}