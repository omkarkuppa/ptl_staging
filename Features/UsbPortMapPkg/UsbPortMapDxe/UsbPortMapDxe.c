/** @file
  Implements UsbPortMap Dxe driver and publish UsbPortMap Acpi table

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

#include "UsbPortMapDxe.h"

GLOBAL_REMOVE_IF_UNREFERENCED USB_PORT_MAP_NVS_AREA_PROTOCOL mUsbPortMapNvsAreaProtocol;

/**
  UsbPortMap NVS Area Initialization

  @param[in] mUsbPortMapNvsAreaPtr      Pointer to USB PORT MAP NVS Area.
**/
VOID
UsbPortMapNvsAreaInit (
  IN  VOID              **mUsbPortMapNvsAreaPtr
  )
{
  UINTN                         Pages;
  EFI_PHYSICAL_ADDRESS          Address;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_INFO, "UsbPortMapNvsAreaInit Start\n"));

  Pages = EFI_SIZE_TO_PAGES (sizeof (USB_PORT_MAP_NVS_AREA));
  Address = 0xffffffff; // allocate address below 4G.

  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   Pages,
                   &Address
                   );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  *mUsbPortMapNvsAreaPtr = (VOID *) (UINTN) Address;
  SetMem (*mUsbPortMapNvsAreaPtr, sizeof (USB_PORT_MAP_NVS_AREA), 0);

  DEBUG ((DEBUG_INFO, "UsbPortMapNvsAreaInit End\n"));
}

/**
  Install UsbPortMap ACPI Table
**/
EFI_STATUS
InstallUsbPortMapAcpiTable (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         NumberOfHandles;
  EFI_FV_FILETYPE               FileType;
  UINT32                        FvStatus;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  UINTN                         Size;
  UINTN                         i;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  INTN                          Instance;
  EFI_ACPI_COMMON_HEADER        *CurrentTable;
  UINTN                         AcpiTableKey;
  UINT8                         *CurrPtr;
  UINT8                         *EndPtr;
  UINT32                        *Signature;
  EFI_ACPI_DESCRIPTION_HEADER   *UsbPortMapAcpiTable;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;

  FwVol = NULL;
  UsbPortMapAcpiTable = NULL;
  ///
  /// Locate ACPI Table protocol
  ///
  DEBUG ((DEBUG_INFO, "Init USB PORT MAP SSDT table\n"));
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_WARN, "[USB PORT MAP] Fail to locate EfiAcpiTableProtocol.\n"));
    return EFI_NOT_FOUND;
  }
  ///
  /// Locate protocol.
  /// There is little chance we can't find an FV protocol
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "[USB PORT MAP] Fail to locate EfiFirmwareVolume2Protocol.\n"));
    return Status;
  }
  ///
  /// Looking for FV with ACPI storage file
  ///
  for (i = 0; i < NumberOfHandles; i++) {
    ///
    /// Get the protocol on this handle
    /// This should not fail because of LocateHandleBuffer
    ///
    Status = gBS->HandleProtocol (
                    HandleBuffer[i],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **) &FwVol
                    );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "[USB PORT MAP] Fail to get EfiFirmwareVolume2Protocol on handle.\n"));
      return Status;
    }

    ///
    /// See if it has the ACPI storage file
    ///
    Size      = 0;
    FvStatus  = 0;
    Status = FwVol->ReadFile (
                      FwVol,
                      &gEfiCallerIdGuid,
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
  /// Free any allocated buffers
  ///
  FreePool (HandleBuffer);
  ///
  /// Sanity check that we found our data file
  ///
  if (FwVol == NULL) {
    DEBUG ((DEBUG_INFO, "USB PORT MAP Global NVS table not found\n"));
    ASSERT (FwVol != NULL);
    return EFI_NOT_FOUND;
  }
  ///
  /// Our exit status is determined by the success of the previous operations
  /// If the protocol was found, Instance already points to it.
  /// Read tables from the storage file.
  ///
  Instance      = 0;
  CurrentTable  = NULL;
  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                      FwVol,
                      &gEfiCallerIdGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &CurrentTable,
                      &Size,
                      &FvStatus
                      );
    if (!EFI_ERROR (Status)) {
      ///
      /// Check the table ID to modify the table
      ///
      if (((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->OemTableId == SIGNATURE_64 ('U','p', 'm', 'p', 'S', 's', 'd', 't')) {
        UsbPortMapAcpiTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
        ///
        /// Locate the SSDT package
        ///
        CurrPtr = (UINT8 *) UsbPortMapAcpiTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER);
        /// OperationRegion space (14 bytes) = 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1 + sizeof(UINT16)
        EndPtr  = (UINT8 *) UsbPortMapAcpiTable + UsbPortMapAcpiTable->Length - 14;

        for (; CurrPtr <= EndPtr; CurrPtr++) {
          Signature = (UINT32 *) (CurrPtr + 1);
          if (*Signature == SIGNATURE_32 ('X', 'H', 'N', 'V')) {
            ASSERT (*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) == 0xFFFF0000);
            if (*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) != 0xFFFF0000) {
              return EFI_NOT_FOUND;
            }

            ASSERT (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) == 0xAA55);
            if (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) != 0xAA55) {
              return EFI_NOT_FOUND;
            }

            ///
            /// USB PORT MAP Global NVS Area address
            ///
            *(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) = (UINT32) (UINTN) mUsbPortMapNvsAreaProtocol.Area;
            ///
            /// USB PORT MAP Global NVS Area size
            ///
            *(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) = sizeof (USB_PORT_MAP_NVS_AREA);

            AcpiTableKey = 0;
            Status = AcpiTable->InstallAcpiTable (
                                  AcpiTable,
                                  UsbPortMapAcpiTable,
                                  UsbPortMapAcpiTable->Length,
                                  &AcpiTableKey
                                  );
            ASSERT_EFI_ERROR (Status);
            gBS->FreePool (UsbPortMapAcpiTable);
            return EFI_SUCCESS;
          }
        }
      }
      ///
      /// Increment the instance
      ///
      Instance++;
      CurrentTable = NULL;
    }
  }
  return Status;
}

/**
  This function is the entry point for this DXE driver.
  It creates ACPI table and publishes it.

  @param[in] ImageHandle       Image handle of this driver.
  @param[in] SystemTable       Global system service table.

  @retval EFI_NOT_FOUND        Initialization fails
  @retval EFI_SUCCESS          Initialization completes successfully.

**/
EFI_STATUS
EFIAPI
UsbPortMapEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    Handle;

  DEBUG ((DEBUG_INFO, "%a enter\n", __FUNCTION__));

  UsbPortMapNvsAreaInit ((VOID **) &mUsbPortMapNvsAreaProtocol.Area);

  Status = InstallUsbPortMapAcpiTable ();
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // [ACPI] UsbPortMap Install NvsAreaProtocol
  //
  Handle = NULL;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gUsbPortMapNvsAreaProtocolGuid,
                  &mUsbPortMapNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
