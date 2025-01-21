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
#include <Library/BaseLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/AcpiTable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <I2cPowerMeterAcpiTable.h>

GLOBAL_REMOVE_IF_UNREFERENCED  EFI_ACPI_TABLE_PROTOCOL          *AcpiTable;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_DESCRIPTION_HEADER       *I2cPmAcpiTable;

EFI_STATUS
EFIAPI
I2cPmData (
  IN      POWER_METER_PKG             *PowerMtrData,
  IN      UINT8                       PowerMeterEnable
);

I2C_PM_CONFIG_PROTOCOL  I2cPmConfigData = {
  I2cPmData
};

/**
  Function used to update data in Power Meter ACPI table from I2c Power Meter structure.
  @param[in] POWER_METER_PKG     I2C Power Meter structure.
**/
EFI_STATUS
EFIAPI
I2cPmData (
  IN  POWER_METER_PKG              *PowerMtrData,
  IN  UINT8                        PowerMeterEnable
)
{
  DEBUG ((DEBUG_INFO,"Power Meter Data\n"));

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
  UINTN                         AcpiTableKey;

  UINT8                         *CurrPtr;
  UINT8                         *DataPtr;
  INT16                         Rsense;
  UINT16                        DevCount;
  UINT8                         *TablePointer;
  UINT32                        *Signature;
  UINT8                         *Operation;

  FwVol       = NULL;
  ///
  /// Locate ACPI Table protocol
  ///
  DEBUG ((DEBUG_INFO, "Init Power Meter SSDT table\n"));
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_WARN, "Fail to locate EfiAcpiTableProtocol.\n"));
    return EFI_NOT_FOUND;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "[I2c PowerMeter] Fail to locate EfiFirmwareVolume2Protocol.\n"));
    return Status;
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
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "[I2c PowerMeter] Fail to get EfiFirmwareVolume2Protocol on handle.\n"));
      return Status;
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
    DEBUG ((DEBUG_ERROR, "Power Meter Global NVS table not found\n"));
    return EFI_NOT_FOUND;
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
      if (CompareMem (&((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->OemTableId, "I2Pm_Rvp", 8) == 0) {

        I2cPmAcpiTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
        CurrPtr = (UINT8 *) I2cPmAcpiTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER);
        for (TablePointer = CurrPtr; TablePointer < ((UINT8 *) I2cPmAcpiTable + I2cPmAcpiTable->Length - sizeof (UINT32)); TablePointer++)
        {
          Signature = (UINT32 *) TablePointer;
          switch (*Signature) {
          case (SIGNATURE_32 ('_', 'S', 'B', '_')):
            ///
            /// If we find the _SB_PC00I2C3 scope, save a pointer to the package length
            ///
            Operation = TablePointer - 7;
            if ((*Operation == AML_SCOPE_OP) &&
              (*(Signature) == SIGNATURE_32 ('_', 'S', 'B', '_')) &&
              (*(Signature + 1) == SIGNATURE_32 ('P', 'C', '0', '0')) &&
              (*(Signature + 2) == SIGNATURE_32 ('I', '2', 'C', '3'))
              ) {
              TablePointer = TablePointer + 11;
              *TablePointer = PowerMtrData->I2cBusNumber;
            }
            break;
          case (SIGNATURE_32 ('P','O','M','E')):
            //
            // Conditional match.  For Name Objects, the Operator will always be the byte
            // immediately before the specific name. Therefore, subtract 1 to check the
            // Operator.
            //
            Operation = TablePointer - 1;
            DataPtr = TablePointer + 5;
            if (*Operation == AML_NAME_OP) {
              CopyMem (DataPtr, &PowerMeterEnable, sizeof (UINT8));
            }
            break;
          case (SIGNATURE_32 ('P','A','C','T')):
            //
            // Conditional match.  For Name Objects, the Operator will always be the byte
            // immediately before the specific name. Therefore, subtract 1 to check the
            // Operator.
            //
            Operation = TablePointer - 1;
            DataPtr = TablePointer + 5;
            if (*Operation == AML_NAME_OP) {
              CopyMem (DataPtr, &(PowerMtrData->DeviceSize), sizeof (UINT8));
            }
            break;
          case (SIGNATURE_32 ('P','A','B','N')):
            //
            // Conditional match.  For Name Objects, the Operator will always be the byte
            // immediately before the specific name. Therefore, subtract 1 to check the
            // Operator.
            //
            Operation = TablePointer - 1;
            DataPtr = TablePointer + 5;
            if (*Operation == AML_NAME_OP) {
              CopyMem (DataPtr, &(PowerMtrData->I2cBusNumber), sizeof (UINT8));
            }
            break;
          case (SIGNATURE_32 ('P','A','S','1')):
            //
            // Conditional match.  For Name Objects, the Operator will always be the byte
            // immediately before the specific name. Therefore, subtract 1 to check the
            // Operator.
            //
            Operation = TablePointer - 1;
            DataPtr = TablePointer + 8;
            if (*Operation == AML_NAME_OP) {
              //
              // To keep the PMAX table in fixed size, fill the vacancy of the string buffer with AML NO OP.
              // This will makes PMAX table shows numerous "NoOp" after device name.
              //
              for ( DevCount = 0; DevCount < (PowerMtrData->DeviceSize); DevCount++)
              {
                CopyMem (DataPtr, &(PowerMtrData->PmConfigPkgArray[DevCount].DeviceAddress), sizeof (UINT8));
                DataPtr = (DataPtr + 2);
              }
            }
            break;
          case (SIGNATURE_32 ('P','M','P','0')):
            //
            // Conditional match.  For Name Objects, the Operator will always be the byte
            // immediately before the specific name. Therefore, subtract 1 to check the
            // Operator.
            //
            Operation = TablePointer - 1;
            DataPtr = TablePointer + 13;
            if (*Operation == AML_NAME_OP) {
              //
              // Conditional match.  For Name Objects, the Operator will always be the byte
              // immediately before the specific name. Therefore, subtract 1 to check the
              // Operator.
              //
              for ( DevCount = 0; DevCount < (PowerMtrData->DeviceSize); DevCount++)
              {
                for (Index = 0; Index < 4; Index++) {
                  CopyMem (DataPtr, &(PowerMtrData->PmConfigPkgArray[DevCount].RailDataPkg[Index].RailName), AsciiStrLen(PowerMtrData->PmConfigPkgArray[DevCount].RailDataPkg[Index].RailName));
                  DataPtr = (DataPtr+MAX_PM_DEVICE_STRING_LENGTH+2);
                }
                DataPtr = (DataPtr+4);
              }
            }
            break;
          case (SIGNATURE_32 ('P','M','P','1')):
            //
            // Conditional match.  For Name Objects, the Operator will always be the byte
            // immediately before the specific name. Therefore, subtract 1 to check the
            // Operator.
            //
            Operation = TablePointer - 1;
            DataPtr = TablePointer + 12;
            if (*Operation == AML_NAME_OP) {
              //
              // To keep the PMAX table in fixed size, fill the vacancy of the string buffer with AML NO OP.
              // This will makes PMAX table shows numerous "NoOp" after device name.
              //
              for ( DevCount = 0; DevCount < (PowerMtrData->DeviceSize); DevCount++)
              {
                for (Index=0; Index<4; Index++) {
                  Rsense = PowerMtrData->PmConfigPkgArray[DevCount].RailDataPkg[Index].ResistorValue;
                  Rsense = (Rsense*1000);
                  CopyMem (DataPtr, &Rsense, sizeof (UINT16));
                  DataPtr = DataPtr+3;
                }
                DataPtr = (DataPtr+3);
              }
            }
            break;
          default:
            break;
          }
        }
        break;
      }
    }
  ///
  /// Increment the instance
  ///
  Instance++;
  CurrentTable = NULL;
  }

  AcpiTableKey = 0;
  Status = AcpiTable->InstallAcpiTable (
                                AcpiTable,
                                I2cPmAcpiTable,
                                I2cPmAcpiTable->Length,
                                &AcpiTableKey
                                );
  FreePool (I2cPmAcpiTable);
  if(Status!=EFI_SUCCESS){
    DEBUG ((DEBUG_INFO, "I2c Power Meter Table did not Install\n"));
    return EFI_NOT_FOUND;
  }
  DEBUG ((DEBUG_INFO, "I2c Power Meter Table Installed\n"));
  return EFI_SUCCESS;
}

/**
  Entry point for I2C Power Meter device configuration.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.
**/

EFI_STATUS
EFIAPI
I2cPowerMeterEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status;
  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gI2cPmUpdateProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &I2cPmConfigData
                );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
  }
  return Status;
}