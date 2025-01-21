/**@file
  I2C Device Common Driver
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

@par Specification
**/

#include <PiDxe.h>
#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/AcpiTable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <I2cCommon.h>
#include <Library/PcdLib.h>


GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_DESCRIPTION_HEADER       *mI2CAcpiTable;
GLOBAL_REMOVE_IF_UNREFERENCED  EFI_ACPI_TABLE_PROTOCOL          *mAcpiTable;

EFI_ACPI_DESCRIPTION_HEADER *
EFIAPI
GetDevTablePointer (
  VOID
  );

TABLE_POINTER_ADDR_PROTOCOL  mTablePointer = {
  GetDevTablePointer
};

/**
  Call Back function to Install Table at EndofDxe.
  @param  Event                 Event whose notification function is being invoked.
  @param  Context               The pointer to the notification function's context,
                                which is implementation-dependent.
**/
VOID
EFIAPI
InstallI2cTable
(
  IN EFI_EVENT                Event,
  IN VOID                     *Context
  )
{


  UINTN                         AcpiTableKey;
  EFI_STATUS                    Status;
  AcpiTableKey = 0;
  Status = mAcpiTable->InstallAcpiTable (
                                mAcpiTable,
                                mI2CAcpiTable,
                                mI2CAcpiTable->Length,
                                &AcpiTableKey
                                );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "RTD3 Tables updated \n"));
}

/**
  Parses the Acpi File to return the current table pointer.

  @retval [mI2CAcpiTable] I2c Acpi TabLE Pointer
**/
EFI_ACPI_DESCRIPTION_HEADER *
EFIAPI
GetDevTablePointer (
  VOID
  )
{

  EFI_STATUS                    Status;
  UINTN                         Index;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         NumberOfHandles;
  EFI_FV_FILETYPE               FileType;
  UINT32                        FvStatus;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  UINTN                         Size;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  INTN                          Instance;
  EFI_ACPI_COMMON_HEADER        *CurrentTable;
  EFI_EVENT                     EndOfDxeEvent;

  FwVol       = NULL;
  mI2CAcpiTable = NULL;

  ///
  /// Locate ACPI Table protocol
  ///
  DEBUG ((DEBUG_INFO, "Init Rtd3 SSDT table\n"));
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &mAcpiTable);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_WARN, "Fail to locate EfiAcpiTableProtocol.\n"));
    goto ERROR;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "[I2c AcpiTable] Fail to locate EfiFirmwareVolume2Protocol.\n"));
    goto ERROR;
  }
  ///
  /// Looking for FV with ACPI storage file
  ///
  for (Index = 0; Index < NumberOfHandles; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **) &FwVol
                    );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "[I2c AcpiTable] Fail to get EfiFirmwareVolume2Protocol on handle.\n"));
      goto ERROR;
    }
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
    DEBUG ((DEBUG_INFO, "Readfile status =%x \n",Status));
    if (Status == EFI_SUCCESS) {
      break;
    }
  }
  ///
  /// Free any allocated buffers
  ///
  FreePool (HandleBuffer);

  if (FwVol == NULL) {
    DEBUG ((DEBUG_ERROR, "Rtd3 Global NVS table not found\n"));
    ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    goto ERROR;
  }

  Instance      = 0;
  CurrentTable  = NULL;

  while (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Readfile success status\n"));
    Status = FwVol->ReadSection (
                      FwVol,
                      &gEfiCallerIdGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &CurrentTable,
                      &Size,
                      &FvStatus
                      );
    DEBUG ((DEBUG_INFO, "Status of Readsection =%x \n",Status));
    DEBUG ((DEBUG_INFO, "Current table = %x \n",((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->OemTableId));

    if (!EFI_ERROR (Status)) {
      ///
      /// Check the table ID to modify the table
      ///
      DEBUG ((DEBUG_INFO, "Current table oem id =%016llx\n",((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->OemTableId));
      if (((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->OemTableId == PcdGet64 (PcdI2cDevTableName)) {
        mI2CAcpiTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
        ///
        /// Locate the SSDT package
        ///
        break;
      }
    }
  ///
  /// Increment the instance
  ///
  Instance++;
  CurrentTable = NULL;
  }
  if(mI2CAcpiTable != NULL){
      DEBUG ((DEBUG_INFO, "RTD3 Call Back Function \n"));
      Status = gBS->CreateEventEx (
                      EVT_NOTIFY_SIGNAL,
                      TPL_CALLBACK,
                      InstallI2cTable,
                      NULL,
                      &gEfiEndOfDxeEventGroupGuid,
                      &EndOfDxeEvent
                );
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        goto ERROR;
      }
  }

// On Error returning NULL pointer.
ERROR:
  return mI2CAcpiTable;

}

/**
  Function used to fill data into I2c Buffer from I2c Rtd3 structure.

  @param[in] Count            Maximum Pch I2C ports.
  @param[in] I2C_ACPI_DEV     I2C ACPI device structure.

**/
EFI_STATUS
EFIAPI
I2CTouchDevEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{

  EFI_STATUS  Status;
  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gI2cDeviceProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mTablePointer
                );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
  }
  return Status;
}
