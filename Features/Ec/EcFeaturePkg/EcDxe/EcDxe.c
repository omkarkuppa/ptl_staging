/**@file
  EcDxe driver to do Ec intialize and related ACPI table installation.

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
#include <Uefi.h>
#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/AcpiTable.h>
#include <Pi/PiFirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Protocol/FirmwareVolume2.h>
#include <IndustryStandard/Acpi.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/EcNvsArea.h>
#include <IndustryStandard/AcpiAml.h>

static BOOLEAN mEcIdInitalized = FALSE;

static UINT8 mEcId[40] = {0};

/**
  Returns the scope of H_EC devices.

  @param[in]        SignaturePointer        A pointer to H_EC signature
  @param[in]        SsdtTablePointer        A pointer to header of SSDT table.
  @param[in, out]   EcIdBuffer              A pointer to the buffer to hold the EcId data in the EC ACPI SSDT Table.
  @param[in]        EcIdBufferLength        Fixed length of EcId Buffer.

  @retval EFI_SUCCESS                       The function returned successfully.
  @retval EFI_NOT_FOUND                     The section is not found.
  @retval EFI_DEVICE_ERROR                  Cannot find the parent path due to device Error.
  @retval EFI_OUT_OF_RESOURCES              Memory allocation for response failed.
**/
static
EFI_STATUS
FindEcId (
  IN      UINT8       *SignaturePointer,
  IN      UINT8       *SsdtTablePointer,
  IN OUT  UINT8       *EcIdBuffer,
  IN      UINT8       EcIdBufferLength
  )
{
  EFI_STATUS          Status;
  UINT8               Index;
  UINT8               *AmlPointer;
  UINT8               EcIdSize;
  UINT8               PathCount;
  UINT8               ArrayIndex;
  BOOLEAN             FindEcDeviceOpCode;

  Status = EFI_SUCCESS;
  FindEcDeviceOpCode = FALSE;
  AmlPointer = SignaturePointer;

  //
  // SignaturePointer should point to Device OpCode NameString
  // DeviceOp PkgLength NameString TermList
  // PkgLength would be 1 - 4 bytes
  //
  for (Index = 1; Index <= 4; Index++) {
    AmlPointer = SignaturePointer;
    //
    // Move to DeviceOp
    //
    AmlPointer -= (Index + 2);
    if (*(UINT16 *)AmlPointer == SIGNATURE_16 (AML_EXT_OP, AML_EXT_DEVICE_OP)) {
      FindEcDeviceOpCode = TRUE;
      break;
    }
  }

  if (!FindEcDeviceOpCode) {
    return EFI_NOT_FOUND;
  }

  //
  // Find the Scope NameString
  // ScopeOp PkgLength NameString TermList
  // The TermList should be Nothing
  //
  AmlPointer -= 1;
  //
  // NameString:
  // <RootChar NamePath> | <PreFixPath NamePath>
  // NameSeg | DualNamePath | MultiNamePath | NullName
  //
  while ((*AmlPointer != AML_ROOT_CHAR) && (AmlPointer >= SsdtTablePointer)) {
    AmlPointer--;
  }

  if (AmlPointer < SsdtTablePointer) {
    DEBUG ((DEBUG_ERROR, "Cannot find the Parent path of H_EC!\n"));
    return EFI_DEVICE_ERROR;
  }

  AmlPointer += 1;
  if (*AmlPointer == AML_MULTI_NAME_PREFIX ) {
    //
    // MultiNamePath
    //
    AmlPointer += 1;
    PathCount = *AmlPointer;
    AmlPointer += 1;
  } else if (*AmlPointer == AML_DUAL_NAME_PREFIX) {
    //
    // DualNamePath
    //
    PathCount = 2;
    AmlPointer += 1;
  } else {
    PathCount = 1;
  }
  EcIdSize = PathCount * 4 + (PathCount - 1) + 5 + 1;

  if (EcIdSize > EcIdBufferLength) {
    Status = EFI_OUT_OF_RESOURCES;
  } else {
    EcIdBuffer[0] = '\\';
    CopyMem ((EcIdBuffer + 1), AmlPointer, 3);
    EcIdBuffer[4] = 0;
    for (Index = 1; Index < PathCount; Index++) {
      ArrayIndex = 5 * Index;
      EcIdBuffer[ArrayIndex - 1] = '.';
      CopyMem ((EcIdBuffer + ArrayIndex), (AmlPointer + 4 * Index), 4);
    }
    ArrayIndex = 4 * PathCount + (PathCount - 1);
      EcIdBuffer[ArrayIndex] = '.';
      EcIdBuffer[ArrayIndex + 1] = 'H';
      EcIdBuffer[ArrayIndex + 2] = '_';
      EcIdBuffer[ArrayIndex + 3] = 'E';
      EcIdBuffer[ArrayIndex + 4] = 'C';
      EcIdBuffer[ArrayIndex + 5] = 0;
  }

  return Status;
}

EFI_STATUS
UpdateAcpiDataTable (
  IN OUT  EFI_ACPI_COMMON_HEADER        *Table
  )
{
  EFI_ACPI_DESCRIPTION_HEADER                 *TableHeader;
  UINT8                                       *SsdtPointer;
  UINT8                                       *CurrPtr;
  UINT32                                      *Signature;
  UINT8                                       *Operation;
  UINT32                                      *Address;
  EC_NVS_AREA_PROTOCOL                        *EcNvsArea;
  UINT16                                      *Size;
  EFI_STATUS                                  Status;
  BOOLEAN                                     IsEcNvsMemUpdated;

  Status = EFI_SUCCESS;
  IsEcNvsMemUpdated = FALSE;

  TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;

  if (Table->Signature == EFI_ACPI_6_5_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
    CopyMem (TableHeader->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (TableHeader->OemId));
  }

  if (Table->Signature != EFI_ACPI_6_5_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
    //
    // Set through patchable PCD value
    //
    TableHeader->OemTableId = PcdGet64 (PcdAcpiDefaultOemTableId);
    TableHeader->OemRevision = PcdGet32(PcdAcpiDefaultOemRevision);

    //
    // Update creator information
    //
    TableHeader->CreatorId        = PcdGet32 (PcdAcpiDefaultCreatorId);
    TableHeader->CreatorRevision  = PcdGet32 (PcdAcpiDefaultCreatorRevision);
  }

  Status = gBS->LocateProtocol (
                  &gEcNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &EcNvsArea
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  switch (Table->Signature) {
    case EFI_ACPI_6_5_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
      CurrPtr = (UINT8 *) TableHeader + sizeof (EFI_ACPI_DESCRIPTION_HEADER);

      //
      // Find and update the EC NVS area.
      //
      for (SsdtPointer = CurrPtr;
        SsdtPointer < ((UINT8 *) TableHeader + TableHeader->Length - sizeof (UINT32));
        SsdtPointer++
        ) {
        Signature = (UINT32 *) SsdtPointer;
        if (!IsEcNvsMemUpdated) {
          if (*Signature == SIGNATURE_32 ('E', 'N', 'V', 'S')) {
            Operation = SsdtPointer - 1;
            if (*Operation == AML_EXT_REGION_OP) {
              Address = (UINT32 *) (SsdtPointer + 6);
              *Address = (UINT32)(UINTN) EcNvsArea->Area;
              Size = (UINT16 *) (SsdtPointer + 11);
              *Size = sizeof (EC_NVS_AREA);
            }
            IsEcNvsMemUpdated = TRUE;
          }
        }
        if (!mEcIdInitalized) {
          if (*Signature == SIGNATURE_32 ('H', '_', 'E', 'C')) {
            Status = FindEcId (SsdtPointer, CurrPtr, mEcId, sizeof (mEcId));
            if (!EFI_ERROR (Status)) {
              DEBUG ((DEBUG_INFO, "Success to init the EcId!\n"));
              DEBUG ((DEBUG_INFO, "Dump EcId: %a\n", mEcId));
              mEcIdInitalized = TRUE;
            } else {
              DEBUG ((DEBUG_ERROR, "Cannot initialize EcId: %r\n", Status));
              mEcIdInitalized = FALSE;
            }
          }
        }
        if (IsEcNvsMemUpdated && mEcIdInitalized) {
           break;
         }
      }
      break;
    case EFI_ACPI_6_5_EMBEDDED_CONTROLLER_BOOT_RESOURCES_TABLE_SIGNATURE:
      //
      // Update GPE Number
      //
      ((EFI_ACPI_6_5_EMBEDDED_CONTROLLER_BOOT_RESOURCES_TABLE *) Table)->GpeBit = (UINT8) EcNvsArea->Area->EcdtGpeNumber;
      if (mEcIdInitalized) {
        CurrPtr = (UINT8 *) Table;
        SsdtPointer = CurrPtr + sizeof (EFI_ACPI_6_5_EMBEDDED_CONTROLLER_BOOT_RESOURCES_TABLE);
        DEBUG ((DEBUG_INFO, "EcId: %a\n", mEcId));
        CopyMem (SsdtPointer, mEcId, 40);
      }
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  return Status;
}

EFI_STATUS
InstallSsdtAcpiTable (
  IN GUID   TableStorageGuid,
  IN UINT32 Signature,
  IN UINT64 OemTableId
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  BOOLEAN                       LoadTable;
  UINTN                         NumberOfHandles;
  UINTN                         Index;
  INTN                          Instance;
  UINTN                         Size;
  UINT32                        FvStatus;
  UINTN                         TableHandle;
  EFI_FV_FILETYPE               FileType;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader;
  EFI_ACPI_COMMON_HEADER        *Table;

  FwVol         = NULL;
  Table         = NULL;

  DEBUG ((DEBUG_INFO, "Loading SSDT Table GUID: %g\n", TableStorageGuid));

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
    if (FwVol == NULL) {
      return EFI_NOT_FOUND;
    }
    ///
    /// See if it has the ACPI storage file
    ///
    Size      = 0;
    FvStatus  = 0;
    Status = FwVol->ReadFile (
                      FwVol,
                      &TableStorageGuid,
                      NULL,
                      &Size,
                      &FileType,
                      &Attributes,
                      &FvStatus
                      );

    ///
    /// If we found it, then we are done
    ///
    if (!EFI_ERROR (Status)) {
      break;
    }
  }
  ///
  /// Our exit status is determined by the success of the previous operations
  /// If the protocol was found, Instance already points to it.
  ///
  ///
  /// Free any allocated buffers
  ///
  FreePool (HandleBuffer);

  ///
  /// Sanity check that we found our data file
  ///
  ASSERT (FwVol);

  ///
  /// Locate ACPI tables
  ///
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);

  ///
  /// Read tables from the storage file.
  ///
  if (FwVol == NULL) {
    ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    return EFI_NOT_FOUND;
  }
  Instance = 0;

  while (Status == EFI_SUCCESS) {
    ///
    /// Read the ACPI tables
    ///
    Status = FwVol->ReadSection (
                      FwVol,
                      &TableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &Table,
                      &Size,
                      &FvStatus
                      );
    if (!EFI_ERROR (Status)) {
      ///
      /// check and load HybridGraphics SSDT table
      ///
      LoadTable   = FALSE;
      TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;

      if (((EFI_ACPI_DESCRIPTION_HEADER *) TableHeader)->Signature == Signature) {
        if (((EFI_ACPI_DESCRIPTION_HEADER *) TableHeader)->OemTableId == OemTableId) {
          UpdateAcpiDataTable (Table);
          ///
          /// This is the SSDT table that match the Signature
          ///
          DEBUG ((DEBUG_INFO, "Found out SSDT Table GUID: %g\n", TableStorageGuid));
          LoadTable = TRUE;
        }
      }

      ///
      /// Add the table
      ///
      if (LoadTable) {
        TableHandle = 0;
        Status = AcpiTable->InstallAcpiTable (
                              AcpiTable,
                              TableHeader,
                              TableHeader->Length,
                              &TableHandle
                              );
      }
      ///
      /// Increment the instance
      ///
      Instance++;
      FreePool (Table);
      Table = NULL;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
InstallEcSsdt (
  void
  )
{
  EFI_STATUS      Status;

  Status =  InstallSsdtAcpiTable (
              gEcSsdtAcpiTableStorageGuid,
              EFI_ACPI_6_5_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE,
              SIGNATURE_64 ('E','c','S','s','d','t',' ', 0)
              );
  ASSERT_EFI_ERROR (Status);

  return InstallSsdtAcpiTable (
            gEcSsdtAcpiTableStorageGuid,
            EFI_ACPI_6_5_EMBEDDED_CONTROLLER_BOOT_RESOURCES_TABLE_SIGNATURE,
            0
            );

}

/**
  Initialization for the Ec device.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
EcDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;

  Status = InstallEcSsdt ();

  return Status;
}
