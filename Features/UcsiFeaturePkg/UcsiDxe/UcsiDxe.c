/** @file
  Implements Ucsi Dxe driver and publish Ucsi Acpi table

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

#include "UcsiDxe.h"

GLOBAL_REMOVE_IF_UNREFERENCED UCSI_NVS_AREA_PROTOCOL mUcsiNvsAreaProtocol;

EFI_GUID UcsiVarGuid = { 0xfc876842, 0xd8f0, 0x4844, {0xae, 0x32, 0x1f, 0xf8, 0x43, 0x79, 0x7b, 0x17} };

/**
  Ucsi NVS Area Initialization

  @param[in] mUcsiNvsAreaPtr      Pointer to UCSI NVS Area.
**/
VOID
UcsiNvsAreaInit (
  IN  VOID              **mUcsiNvsAreaPtr
  )
{
  UINTN                         Pages;
  EFI_PHYSICAL_ADDRESS          Address;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_INFO, "UcsiNvsAreaInit Start\n"));

  Pages = EFI_SIZE_TO_PAGES (sizeof (UCSI_NVS_AREA));
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

  *mUcsiNvsAreaPtr = (VOID *) (UINTN) Address;
  SetMem (*mUcsiNvsAreaPtr, sizeof (UCSI_NVS_AREA), 0);

  DEBUG ((DEBUG_INFO, "UcsiNvsAreaInit End\n"));
}

/**
  Allocate MemoryType below 4G memory address.

  @param[in] Size           Size of memory to allocate.
  @param[in] Buffer         Allocated address for output.

  @retval EFI_SUCCESS       Memory successfully allocated.
  @retval Other             Other errors occur.
**/
static EFI_STATUS
AllocateMemoryBelow4G (
  IN   EFI_MEMORY_TYPE MemoryType,
  IN   UINTN           Size,
  OUT  VOID           **Buffer
  )
{
  UINTN                 Pages;
  EFI_PHYSICAL_ADDRESS  Address;
  EFI_STATUS            Status;

  Pages = EFI_SIZE_TO_PAGES (Size);
  Address = 0xffffffff;

  Status  = (gBS->AllocatePages) (
                   AllocateMaxAddress,
                   MemoryType,
                   Pages,
                   &Address
                   );

  *Buffer = (VOID *) (UINTN) Address;

  return Status;
};



/**
  Install Ucsi ACPI Table
**/
EFI_STATUS
InstallUcsiAcpiTable (
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
  EFI_ACPI_DESCRIPTION_HEADER   *UcsiAcpiTable;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;

  FwVol        = NULL;
  UcsiAcpiTable = NULL;
  ///
  /// Locate ACPI Table protocol
  ///
  DEBUG ((DEBUG_INFO, "Init UCSI SSDT table\n"));
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_WARN, "[UCSI] Fail to locate EfiAcpiTableProtocol.\n"));
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
    DEBUG ((DEBUG_WARN, "[UCSI] Fail to locate EfiFirmwareVolume2Protocol.\n"));
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
      DEBUG ((DEBUG_WARN, "[UCSI] Fail to get EfiFirmwareVolume2Protocol on handle.\n"));
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
    DEBUG ((DEBUG_INFO, "UCSI Global NVS table not found\n"));
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
      if (((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->OemTableId == SIGNATURE_64 ('U','c', 's', 'i', 'T', 'a', 'b', 'l')) {
        UcsiAcpiTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
        ///
        /// Locate the SSDT package
        ///
        CurrPtr = (UINT8 *) UcsiAcpiTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER);
        /// OperationRegion space (14 bytes) = 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1 + sizeof(UINT16)
        EndPtr  = (UINT8 *) UcsiAcpiTable + UcsiAcpiTable->Length - 14;

        for (; CurrPtr <= EndPtr; CurrPtr++) {
          Signature = (UINT32 *) (CurrPtr + 1);
          if (*Signature == SIGNATURE_32 ('U', 'P', 'N', 'V')) {
            ASSERT (*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) == 0xFFFF0000);
            if (*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) != 0xFFFF0000) {
              return EFI_NOT_FOUND;
            }

            ASSERT (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) == 0xAA55);
            if (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) != 0xAA55) {
              return EFI_NOT_FOUND;
            }

            ///
            /// UCSI Global NVS Area address
            ///
            *(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) = (UINT32) (UINTN) mUcsiNvsAreaProtocol.Area;
            ///
            /// UCSI Global NVS Area size
            ///
            *(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) = sizeof (UCSI_NVS_AREA);

            AcpiTableKey = 0;
            Status = AcpiTable->InstallAcpiTable (
                                  AcpiTable,
                                  UcsiAcpiTable,
                                  UcsiAcpiTable->Length,
                                  &AcpiTableKey
                                  );
            ASSERT_EFI_ERROR (Status);
            gBS->FreePool (UcsiAcpiTable);
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
UcsiEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS                    Status;
  UCSI_OPM_BUF                  *OpmBuffer;
  UINT8                         UcmSelection;
  UINT8                         *TcssPortUcxxDriverSupport;
  EFI_HANDLE                    Handle;

  DEBUG ((DEBUG_INFO, "%a enter\n", __FUNCTION__));

  UcsiNvsAreaInit ((VOID **) &mUcsiNvsAreaProtocol.Area);

  Status = InstallUcsiAcpiTable ();
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (PcdGet8 (PcdUsbCUcmMode) == 0) {
    DEBUG ((DEBUG_WARN, "USBC UCM mode is unsupported.\n"));
    return EFI_UNSUPPORTED;
  }

  UcmSelection              = PcdGet8 (PcdUcmSelection);
  TcssPortUcxxDriverSupport = PcdGetPtr (PcdPortUcxxDriverSupport);

  //
  // [ACPI] Ucsi Install NvsAreaProtocol
  //
  Handle = NULL;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gUcsiNvsAreaProtocolGuid,
                  &mUcsiNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Allocate memory in ACPI NVS
  //
  Status = AllocateMemoryBelow4G (EfiACPIMemoryNVS, 0x1000, (VOID **) &OpmBuffer);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  SetMem (OpmBuffer, sizeof (UCSI_OPM_BUF), 0x0);

  //
  //  Get version when UCSI Device is enable
  //
  if (UcmSelection != UCXX_DISABLE){
    OpmBuffer->Version = (UINT16) PcdGet16 (PcdUcsiRevision);
  }

  mUcsiNvsAreaProtocol.Area->UcsiOpBaseAddr = (UINT32) (UINTN) OpmBuffer;
  DEBUG ((DEBUG_INFO, "Ucsi EntryPoint: mUcsiNvsAreaProtocol.Area->UcsiOpBaseAddr = 0x%X\n", mUcsiNvsAreaProtocol.Area->UcsiOpBaseAddr));
  DEBUG ((DEBUG_INFO, "Ucsi Version: 0x%04X\n", OpmBuffer->Version));

  Status = gRT->SetVariable (
    UCSI_DATA_VAR,
    &UcsiVarGuid,
    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
    sizeof (UINT32),
    &(mUcsiNvsAreaProtocol.Area->UcsiOpBaseAddr)
  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Port 1  mapping and properties
  mUcsiNvsAreaProtocol.Area->UsbCPort1UcxxSupport = (TcssPortUcxxDriverSupport[0] == TCSS_FORCE_DISABLE) ?\
                                                    0 : UcmSelection;
  // Port 2  mapping and properties
  mUcsiNvsAreaProtocol.Area->UsbCPort2UcxxSupport = (TcssPortUcxxDriverSupport[1] == TCSS_FORCE_DISABLE) ?\
                                                    0 : UcmSelection;
  // Port 3  mapping and properties
  mUcsiNvsAreaProtocol.Area->UsbCPort3UcxxSupport = (TcssPortUcxxDriverSupport[2] == TCSS_FORCE_DISABLE) ?\
                                                    0 : UcmSelection;
  // Port 4  mapping and properties
  mUcsiNvsAreaProtocol.Area->UsbCPort4UcxxSupport = (TcssPortUcxxDriverSupport[3] == TCSS_FORCE_DISABLE) ?\
                                                    0 : UcmSelection;
  // Port 5  mapping and properties
  mUcsiNvsAreaProtocol.Area->UsbCPort5UcxxSupport = (TcssPortUcxxDriverSupport[4] == TCSS_FORCE_DISABLE) ?\
                                                    0 : UcmSelection;
  // Port 6  mapping and properties
  mUcsiNvsAreaProtocol.Area->UsbCPort6UcxxSupport = (TcssPortUcxxDriverSupport[5] == TCSS_FORCE_DISABLE) ?\
                                                    0 : UcmSelection;
  // Port 7  mapping and properties
  mUcsiNvsAreaProtocol.Area->UsbCPort7UcxxSupport = (TcssPortUcxxDriverSupport[6] == TCSS_FORCE_DISABLE) ?\
                                                    0 : UcmSelection;
  // Port 8  mapping and properties
  mUcsiNvsAreaProtocol.Area->UsbCPort8UcxxSupport = (TcssPortUcxxDriverSupport[7] == TCSS_FORCE_DISABLE) ?\
                                                    0 : UcmSelection;
  // Port 9  mapping and properties
  mUcsiNvsAreaProtocol.Area->UsbCPort9UcxxSupport = (TcssPortUcxxDriverSupport[8] == TCSS_FORCE_DISABLE) ?\
                                                    0 : UcmSelection;
  // Port 10  mapping and properties
  mUcsiNvsAreaProtocol.Area->UsbCPortAUcxxSupport = (TcssPortUcxxDriverSupport[9] == TCSS_FORCE_DISABLE) ?\
                                                    0 : UcmSelection;

  return Status;
}