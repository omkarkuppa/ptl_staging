/**@file
  Pep Override Dxe driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Uefi/UefiSpec.h>
#include <SetupVariable.h>
#include <Protocol/PepNvsArea.h>
#include <Protocol/FirmwareVolume2.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/AslUpdateLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED PEP_NVS_AREA_PROTOCOL               mPepNvsAreaProtocol;

/**
  Initialize Pep SSDT ACPI tables.

  @retval EFI_SUCCESS    ACPI tables are initialized successfully
  @retval EFI_NOT_FOUND  ACPI tables not found
**/
EFI_STATUS
InitializePepSsdtAcpiTables (
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
  EFI_ACPI_DESCRIPTION_HEADER   *PepAcpiTable;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;

  FwVol       = NULL;
  PepAcpiTable = NULL;
  ///
  /// Locate ACPI Table protocol
  ///
  DEBUG ((DEBUG_INFO, "Init PEP SSDT table\n"));
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_WARN, "Fail to locate EfiAcpiTableProtocol.\n"));
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
  ASSERT (FwVol != NULL);
  if (FwVol == NULL) {
    DEBUG ((DEBUG_INFO, "PEP Global NVS table not found\n"));
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
      if (((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->OemTableId == SIGNATURE_64 ('P','e', 'p', 'o', 'S', 's', 'd', 't')) {
        PepAcpiTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
        ///
        /// Locate the SSDT package
        ///
        CurrPtr = (UINT8 *) PepAcpiTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER);
        EndPtr = (UINT8 *) PepAcpiTable + PepAcpiTable->Length;

        for (; CurrPtr <= EndPtr; CurrPtr++) {
          Signature = (UINT32 *) (CurrPtr + 1);
          if (*Signature == SIGNATURE_32 ('P', 'T', 'N', 'V')) {
            ASSERT (*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) == 0xFFFF0000);
            if (*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) != 0xFFFF0000) {
              return EFI_NOT_FOUND;
            }
            ASSERT (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) == 0xAA55);
            if (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) != 0xAA55) {
              return EFI_NOT_FOUND;
            }
            ///
            /// PEP Global NVS Area address
            ///
            *(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) = (UINT32) (UINTN) mPepNvsAreaProtocol.Area;
            ///
            /// PEP Global NVS Area size
            ///
            *(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) = sizeof (PEP_NVS_AREA);

            AcpiTableKey = 0;
            Status = AcpiTable->InstallAcpiTable (
                                  AcpiTable,
                                  PepAcpiTable,
                                  PepAcpiTable->Length,
                                  &AcpiTableKey
                                  );
            ASSERT_EFI_ERROR (Status);
            gBS->FreePool (PepAcpiTable);
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
  PEP NVS Area Initialize
**/
VOID
PepNvsAreaInit (
  IN  VOID              **mPepNvsAreaPtr
  )
{
  UINTN                         Pages;
  EFI_PHYSICAL_ADDRESS          Address;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_INFO, "PepNvsAreaInit Start\n"));

  Pages = EFI_SIZE_TO_PAGES (sizeof (PEP_NVS_AREA));
  Address = 0xffffffff; // allocate address below 4G.

  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   Pages,
                   &Address
                   );
  ASSERT_EFI_ERROR (Status);

  *mPepNvsAreaPtr = (VOID *) (UINTN) Address;
  SetMem (*mPepNvsAreaPtr, sizeof (PEP_NVS_AREA), 0);

  DEBUG ((DEBUG_INFO, "PepNvsAreaInit End\n"));
}

/**
  PEP NVS Area Update
**/
EFI_STATUS
PepNvsAreaUpdate (
)
{
  PEP_NVS_AREA_PROTOCOL        *PepNvsAreaProtocol;
  UINTN                         DataSize;
  SETUP_DATA                    *SetupData;
  UINT32                        SetupDataAttributes;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_INFO, "PepNvsAreaUpdate Start\n"));

  //
  // PepNvsAreaProtocol default value init here
  //
  PepNvsAreaProtocol = (PEP_NVS_AREA_PROTOCOL *) &mPepNvsAreaProtocol;

  DataSize = sizeof (SETUP_DATA);
  SetupData = AllocateZeroPool (DataSize);
  if (SetupData == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate setup data size\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  &SetupDataAttributes,
                  &DataSize,
                  SetupData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get Setup Data Status: %r\n", Status));
    goto Exit;
  }

  //
  // Initialize default values for Pep NVS
  //
  PepNvsAreaProtocol->Area->PepPcieStorage = SetupData->PepPcieStorage;
  PepNvsAreaProtocol->Area->PepPcieLan     = SetupData->PepPcieLan;
  PepNvsAreaProtocol->Area->PepPcieWlan    = SetupData->PepPcieWlan;
  PepNvsAreaProtocol->Area->PepPcieGfx     = SetupData->PepPcieGfx;
  PepNvsAreaProtocol->Area->PepPcieOther   = SetupData->PepPcieOther;
  PepNvsAreaProtocol->Area->PepAudio       = SetupData->PepAudio;
  PepNvsAreaProtocol->Area->PepCpu         = SetupData->PepCpu;
  PepNvsAreaProtocol->Area->PepGfx         = SetupData->PepGfx;
  PepNvsAreaProtocol->Area->PepIpu         = SetupData->PepIpu;
  PepNvsAreaProtocol->Area->PepUart        = SetupData->PepUart;

  //
  // I2C related
  //
  PepNvsAreaProtocol->Area->PepI2c0        = SetupData->PepI2c0;
  PepNvsAreaProtocol->Area->PepI2c1        = SetupData->PepI2c1;
  PepNvsAreaProtocol->Area->PepI2c2        = SetupData->PepI2c2;
  PepNvsAreaProtocol->Area->PepI2c3        = SetupData->PepI2c3;
  PepNvsAreaProtocol->Area->PepI2c4        = SetupData->PepI2c4;
  PepNvsAreaProtocol->Area->PepI2c5        = SetupData->PepI2c5;
  PepNvsAreaProtocol->Area->PepI2c6        = SetupData->PepI2c6;
  PepNvsAreaProtocol->Area->PepI2c7        = SetupData->PepI2c7;

  PepNvsAreaProtocol->Area->PepSpi         = SetupData->PepSpi;
  PepNvsAreaProtocol->Area->PepXhci        = SetupData->PepXhci;
  PepNvsAreaProtocol->Area->PepCsme        = SetupData->PepCsme;
  PepNvsAreaProtocol->Area->PepOsse        = SetupData->PepOsse;
  PepNvsAreaProtocol->Area->PepHeci3       = SetupData->PepHeci3;
  PepNvsAreaProtocol->Area->PepGbe         = SetupData->PepGbe;
  PepNvsAreaProtocol->Area->PepThc0        = SetupData->PepThc0;
  PepNvsAreaProtocol->Area->PepThc1        = SetupData->PepThc1;
  PepNvsAreaProtocol->Area->PepUfs0        = SetupData->PepUfs0;
  PepNvsAreaProtocol->Area->PepUfs1        = SetupData->PepUfs1;
  PepNvsAreaProtocol->Area->PepTcss        = SetupData->PepTcss;
  PepNvsAreaProtocol->Area->PepNpu         = SetupData->PepNpu;
  PepNvsAreaProtocol->Area->PepIsh         = SetupData->PepIsh;
  PepNvsAreaProtocol->Area->PepGna         = SetupData->PepGna;
  PepNvsAreaProtocol->Area->PepSata        = SetupData->PepSataContraints;
  PepNvsAreaProtocol->Area->PepSataEnum    = SetupData->PepSataEnumeration;
  PepNvsAreaProtocol->Area->PepEmmc        = SetupData->PepEmmc;
  PepNvsAreaProtocol->Area->PepSdxc        = SetupData->PepSdxc;
  PepNvsAreaProtocol->Area->PepVmd         = SetupData->PepVmd;
Exit:
  DEBUG ((DEBUG_INFO, "PepNvsAreaUpdate End\n"));
  return Status;
}

/**
  Initialize Pep SSDT ACPI tables

  @retval EFI_SUCCESS    ACPI tables are initialized successfully
  @retval EFI_NOT_FOUND  ACPI tables not found
**/

EFI_STATUS
EFIAPI
PepOvrDxeEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                Status;
  EFI_HANDLE                Handle;
  Status                    = EFI_SUCCESS;
  Handle                    = NULL;

  DEBUG ((DEBUG_INFO, "[PEP] PepOvrDxeEntryPoint START\n"));

  //
  // Initialize Pep Nvs Area
  //
  PepNvsAreaInit ((VOID **) &mPepNvsAreaProtocol.Area);

  //
  // Install Pep Global NVS ACPI table
  //
  Status = InitializePepSsdtAcpiTables ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Initialize Pep Ssdt AcpiTables Status: %r\n", Status));
    goto Exit;
  }

  //
  // Update Pep Nvs Area
  //
  PepNvsAreaUpdate ();

  //
  // Install PEP NVS Protocol
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gPepNvsAreaProtocolGuid,
                  &mPepNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

Exit:
  DEBUG ((DEBUG_INFO, "[PEP] PepOvrDxeEntryPoint END\n"));
  return Status;
}
