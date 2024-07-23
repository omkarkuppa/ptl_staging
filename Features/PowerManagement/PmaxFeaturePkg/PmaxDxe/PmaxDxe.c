/** @file
  PMAX table install and update driver

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

@par Specification
**/
#include <Uefi.h>
#include <Pi/PiFirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>
#include <IndustryStandard/AcpiAml.h>
#include <PmaxDevDef.h>

/**
  Calculate the string length, will include NULL-terminated.

  @param[in] StrPtr     Point to the string.

  @retval String length.
**/
UINTN
GetStrLength (
  IN     CHAR8 *StrPtr
  )
{
  UINTN StrLength;

  StrLength = AsciiStrLen (StrPtr);

  return (StrLength + 1);

}

/**
  Install the PMAX table.

  @param[in] Table         Point to PMAX table..

  @retval EFI_SUCCESS      Install table success.
  @retval Other            The function encountered an error and could not complete process.
**/
EFI_STATUS
InstallTable (
  IN     EFI_ACPI_DESCRIPTION_HEADER *Table
  )
{
  EFI_STATUS              Status;
  EFI_ACPI_TABLE_PROTOCOL *mAcpiTable;
  UINTN                   TableKey;

  if (Table == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  mAcpiTable = NULL;
  TableKey   = 0;

  //
  // Locate ACPI Table protocol.
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &mAcpiTable);
  //
  //  Install table.
  //
  if (!EFI_ERROR (Status)) {
    Status = mAcpiTable->InstallAcpiTable (mAcpiTable, Table, Table->Length, &TableKey);
  }

  FreePool (Table);

  return Status;
}

/**
  Get the Pmax Configuration.

  @param[out] PmaxCfg      Point to PMAX configuration..

  @retval EFI_SUCCESS      Was able to get the required pmax configuration.
  @retval Other            The function encountered an error and could not complete process.
**/
EFI_STATUS
CheckPmaxConfig (
  IN VPD_PMAX_DEV *PmaxCfg
)
{
  EFI_STATUS              Status;
  UINT8                   Index;

  Status = EFI_SUCCESS;

  for (Index = 0; Index < PMAX_DEVICE_NUMBER; Index ++) {
    if ((GetStrLength ((PmaxCfg)[Index].DevStr) > MAX_PMAX_DEVICE_STRING_LENGTH) ||
        ((PmaxCfg)[Index].DevD0 > MAX_PEAK_POWER_VALUE) || ((PmaxCfg)[Index].DevDx > MAX_PEAK_POWER_VALUE) ||
        (AsciiStrLen ((PmaxCfg)[Index].DevStr) == 0)) {
      return EFI_INVALID_PARAMETER;
    }
  }
  return Status;
}

/**
  Get PCD and update the values.

  @param[in, out] PmaxTable     Point to PMAX table.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval Other                 The function encountered an error and could not complete process.
**/
EFI_STATUS
UpdatePmaxTable (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER *PmaxTable
  )
{
  VPD_PMAX_DEV *PmaxCfg = NULL;
  UINT8        *CurrPtr;
  UINT8        *TableEndPtr;
  UINT32       *NameStrPtr;
  UINT8        *DataPtr;
  EFI_STATUS    Status;

  CurrPtr     = NULL;
  TableEndPtr = NULL;
  NameStrPtr  = NULL;
  DataPtr     = NULL;

  if (PmaxTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PmaxCfg = PcdGetPtr (VpdPcdPmaxDevices);
  if (PmaxCfg == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = CheckPmaxConfig (PmaxCfg);
  if (Status != EFI_SUCCESS) {
    return EFI_INVALID_PARAMETER;
  }

  CurrPtr = (UINT8 *) PmaxTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER);
  TableEndPtr = (UINT8 *) PmaxTable + PmaxTable->Length;

  //
  // Loop through the aml code looking for values that need to be updated.
  //
  for (; CurrPtr < TableEndPtr; CurrPtr ++) {
    //
    // Find name aml op code and determine the values need to be updated.
    //
    if ((*CurrPtr) == AML_NAME_OP) {
      NameStrPtr = (UINT32 *) (CurrPtr + 1);
      DataPtr = CurrPtr + 6;

      switch (*NameStrPtr) {
        case (SIGNATURE_32 ('R','T','K','S')):
          //
          // To keep the PMAX table in fixed size, fill the vacancy of the string buffer with AML NO OP.
          // This will makes PMAX table shows numerous "NoOp" after device name.
          //
          SetMem (DataPtr, MAX_PMAX_DEVICE_STRING_LENGTH, AML_NOOP_OP);
          CopyMem (DataPtr, PmaxCfg[0].DevStr, GetStrLength (PmaxCfg[0].DevStr));
          break;

        case (SIGNATURE_32 ('R','T','K','0')):
          CopyMem (DataPtr, &(PmaxCfg[0].DevD0), sizeof (UINT16));
          break;

        case (SIGNATURE_32 ('R','T','K','X')):
          CopyMem (DataPtr, &(PmaxCfg[0].DevDx), sizeof (UINT16));
          break;

        case (SIGNATURE_32 ('W','F','C','S')):
          //
          // To keep the PMAX table in fixed size, fill the vacancy of the string buffer with AML NO OP.
          // This will makes PMAX table shows numerous "NoOp" after device name.
          //
          SetMem (DataPtr, MAX_PMAX_DEVICE_STRING_LENGTH, AML_NOOP_OP);
          CopyMem (DataPtr, PmaxCfg[1].DevStr, GetStrLength (PmaxCfg[1].DevStr));
          break;

        case (SIGNATURE_32 ('W','F','C','0')):
          CopyMem (DataPtr, &(PmaxCfg[1].DevD0), sizeof (UINT16));
          break;

        case (SIGNATURE_32 ('W','F','C','X')):
          CopyMem (DataPtr, &(PmaxCfg[1].DevDx), sizeof (UINT16));
          break;

        case (SIGNATURE_32 ('U','F','C','S')):
          //
          // To keep the PMAX table in fixed size, fill the vacancy of the string buffer with AML NO OP.
          // This will makes PMAX table shows numerous "NoOp" after device name.
          //
          SetMem (DataPtr, MAX_PMAX_DEVICE_STRING_LENGTH, AML_NOOP_OP);
          CopyMem (DataPtr, PmaxCfg[2].DevStr, GetStrLength (PmaxCfg[1].DevStr));
          break;

        case (SIGNATURE_32 ('U','F','C','0')):
          CopyMem (DataPtr, &(PmaxCfg[2].DevD0), sizeof (UINT16));
          break;

        case (SIGNATURE_32 ('U','F','C','X')):
          CopyMem (DataPtr, &(PmaxCfg[2].DevDx), sizeof (UINT16));
          break;

        case (SIGNATURE_32 ('F','L','D','S')):
          //
          // To keep the PMAX table in fixed size, fill the vacancy of the string buffer with AML NO OP.
          // This will makes PMAX table shows numerous "NoOp" after device name.
          //
          SetMem (DataPtr, MAX_PMAX_DEVICE_STRING_LENGTH, AML_NOOP_OP);
          CopyMem (DataPtr, PmaxCfg[3].DevStr, GetStrLength (PmaxCfg[1].DevStr));
          break;

        case (SIGNATURE_32 ('F','L','D','0')):
          CopyMem (DataPtr, &(PmaxCfg[3].DevD0), sizeof (UINT16));
          break;

        case (SIGNATURE_32 ('F','L','D','X')):
          CopyMem (DataPtr, &(PmaxCfg[3].DevDx), sizeof (UINT16));
          break;

        default:
          break;
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Get the handles containing FV protocol.

  @param[out] NumberOfHandles       The number of handles returned in Buffer.
  @param[out] HandleBuffer          A pointer to the buffer containing the requested.
                                    array of handles that support the protocol.

  @retval EFI_SUCCESS      The result array of handles was returned.
  @retval Other            The function encountered an error and could not complete process.
**/

EFI_STATUS
GetHandlesWithFvProtocol (
  OUT  UINTN       *NumberOfHandles,
  OUT  EFI_HANDLE  **HandleBuffer
  )
{
  EFI_STATUS Status;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  NumberOfHandles,
                  HandleBuffer
                  );
  return Status;
}

/**
  Get Firmware volume contaning the Pmax ACPI file.

  @param[in] NumberOfHandles       The number of handles returned in Buffer.
  @param[in] HandleBuffer          A pointer to the buffer containing the requested.
                                  array of handles that support the protocol.

  @retval FwVol                    Firmware volume contaning the Pmax ACPI file.
**/
EFI_FIRMWARE_VOLUME2_PROTOCOL *
GetPmaxFirmwareVolume (
  IN  UINTN       NumberOfHandles,
  IN  EFI_HANDLE  *HandleBuffer
  )
{
  UINTN                         Index;
  EFI_STATUS                    Status;
  UINTN                         Size;
  EFI_FV_FILETYPE               FileType;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  UINT32                        FvStatus;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;

  Size     = 0;
  FvStatus = 0;

  if (HandleBuffer == NULL) {
    return NULL;
  }

  for (Index = 0; Index < NumberOfHandles; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **) &FwVol
                    );
    if (FwVol != NULL) {
    Status = FwVol->ReadFile (
                      FwVol,
                      &gPmaxAcpiTableStorageGuid,
                      NULL,
                      &Size,
                      &FileType,
                      &Attributes,
                      &FvStatus
                      );
    if (!EFI_ERROR (Status)) {
      break;
      }
    }
  }

  FreePool (HandleBuffer);

  return  FwVol;
}

/**
  Reads the pmax acpi file and gets the pmax table.

  @param[in] FwVol        Firmware volume contaning the Pmax ACPI file.
  @param[out] PmaxTable   Point to PMAX table.

  @retval EFI_SUCCESS     The function completed successfully.
  @retval Other           The function encountered an error and could not complete process.

**/
EFI_STATUS
ReadPmaxAcpiFile (
  IN  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol,
  OUT EFI_ACPI_DESCRIPTION_HEADER   **PmaxTable
)
{
 EFI_STATUS             Status;
 INTN                   Instance;
 UINTN                  Size;
 UINT32                 FvStatus;
 EFI_ACPI_COMMON_HEADER *Table;

 Status   = EFI_SUCCESS;
 Instance = 0;
 Table    = NULL;

  if (FwVol == NULL) {
    return EFI_NOT_FOUND;
  }

  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                      FwVol,
                      &gPmaxAcpiTableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &Table,
                      &Size,
                      &FvStatus
                      );
    if (!EFI_ERROR (Status)) {
      if (((EFI_ACPI_DESCRIPTION_HEADER *) Table)->OemTableId ==
            SIGNATURE_64 ('P', 'm', 'a', 'x', '_', 'D', 'e', 'v')) {
        *PmaxTable = (EFI_ACPI_DESCRIPTION_HEADER *) Table;
        break;
      }

      FreePool(Table);
      Instance++;
      Table = NULL;
    }
  }
 return Status;
}
/**
  Find PMAX SSDT Table.

  @param[out] PmaxTable     Point to PMAX table.

  @retval EFI_SUCCESS       Find PMAX table successfully.
  @retval Other             The function encountered an error and could not complete process.
**/
EFI_STATUS
FindPmaxTable (
  OUT EFI_ACPI_DESCRIPTION_HEADER **PmaxTable
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         NumberOfHandles;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;

  FwVol    = NULL;

  Status = GetHandlesWithFvProtocol (&NumberOfHandles, &HandleBuffer);

  if (Status == EFI_SUCCESS) {
    FwVol  = GetPmaxFirmwareVolume (NumberOfHandles, HandleBuffer);
 }

  Status = ReadPmaxAcpiFile (FwVol, PmaxTable);

  return Status;
}

/**
  PMAX table installation driver.

  @param[in] ImageHandle     Handle for this drivers loaded image protocol.
  @param[in] SystemTable     EFI system table.

  @retval EFI_SUCCESS        The driver installed without error.
  @retval Other              The driver encountered an error and could not complete installation of
                             the ACPI tables.

**/
EFI_STATUS
EFIAPI
InstallPmaxTable (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                  Status;
  EFI_ACPI_DESCRIPTION_HEADER *PmaxTable;

  PmaxTable = NULL;

  Status = FindPmaxTable (&PmaxTable);
  if (EFI_ERROR (Status)){
    return Status;
  }

  Status = UpdatePmaxTable (PmaxTable);
  if (EFI_ERROR (Status)){
    return Status;
  }
  Status = InstallTable (PmaxTable);

  return Status;
}
