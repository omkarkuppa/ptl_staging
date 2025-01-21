/**@file

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

@par Specification
**/

#include <SmbiosTpmDeviceInfo.h>

///
/// TPM Device (Type 43) data
///
GLOBAL_REMOVE_IF_UNREFERENCED SMBIOS_TABLE_TYPE43 SmbiosTableType43Data = {
  { EFI_SMBIOS_TYPE_TPM_DEVICE, sizeof (SMBIOS_TABLE_TYPE43), 0 },
  {
    TO_BE_FILLED, ///< VendorID0
    TO_BE_FILLED, ///< VendorID1
    TO_BE_FILLED, ///< VendorID2
    TO_BE_FILLED  ///< VendorID3
  },
  TO_BE_FILLED, ///< MajorSpecVersion
  TO_BE_FILLED, ///< MinorSpecVersion
  TO_BE_FILLED, ///< FirmwareVersion1
  TO_BE_FILLED, ///< FirmwareVersion2
  TO_BE_FILLED, ///< Description
  TO_BE_FILLED, ///< Characteristics
  TO_BE_FILLED, ///< OEM Defined
};

GLOBAL_REMOVE_IF_UNREFERENCED SMBIOS_TYPE43_STRING_ARRAY SmbiosTableType43Strings = {
  TO_BE_FILLED_STRING   ///< Description
};

/**
  Add an SMBIOS table entry using EFI_SMBIOS_PROTOCOL.
  Create the full table record using the formatted section plus each non-null string, plus the terminating (double) null.

  @param[in]  Entry                 The data for the fixed portion of the SMBIOS entry.
                                    The format of the data is determined by EFI_SMBIOS_TABLE_HEADER.
                                    Type. The size of the formatted area is defined by
                                    EFI_SMBIOS_TABLE_HEADER. Length and either followed by a
                                    double-null (0x0000) or a set of null terminated strings and a null.
  @param[in]  TableStrings          Set of string pointers to append onto the full record.
                                    If TableStrings is null, no strings are appended. Null strings
                                    are skipped.
  @param[in]  NumberOfStrings       Number of TableStrings to append, null strings are skipped.
  @param[in]  SmbiosProtocol        Instance of Smbios Protocol
  @param[out] SmbiosHandle          A unique handle will be assigned to the SMBIOS record.

  @retval     EFI_SUCCESS           Table was added.
  @retval     EFI_OUT_OF_RESOURCES  Table was not added due to lack of system resources.
  @retval     EFI_INVALID_PARAMETER The pointer to the input SMBIOS entry is NULL.
**/
EFI_STATUS
AddSmbiosEntry (
  IN  EFI_SMBIOS_TABLE_HEADER *Entry,
  IN  CHAR8                   **TableStrings,
  IN  UINT8                   NumberOfStrings,
  IN  EFI_SMBIOS_PROTOCOL     *SmbiosProtocol
  )
{
  EFI_STATUS              Status;
  EFI_SMBIOS_TABLE_HEADER *Record;
  CHAR8                   *StringPtr;
  UINTN                   Size;
  UINTN                   i;
  EFI_SMBIOS_HANDLE       SmbiosHandle;

  if (Entry == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Calculate the total size of the full record
  //
  Size = Entry->Length;

  //
  // Add the size of each non-null string
  //
  if (TableStrings != NULL) {
    for (i = 0; i < NumberOfStrings; i++) {
      if (TableStrings[i] != NULL) {
        Size += AsciiStrSize (TableStrings[i]);
      }
    }
  }

  //
  // Add the size of the terminating double null
  // If there were any strings added, just add the second null
  //
  if (Size == Entry->Length) {
    Size += 2;
  } else {
    Size += 1;
  }

  //
  // Initialize the full record
  //
  Record = (EFI_SMBIOS_TABLE_HEADER *) AllocateZeroPool (Size);
  if (Record == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (Record, Entry, Entry->Length);

  //
  // Copy the strings to the end of the record
  //
  StringPtr = ((CHAR8 *) Record) + Entry->Length;
  if (TableStrings != NULL) {
    for (i = 0; i < NumberOfStrings; i++) {
      if (TableStrings[i] != NULL) {
        AsciiStrCpyS (StringPtr, Size - Entry->Length, TableStrings[i]);
        StringPtr += AsciiStrSize (TableStrings[i]);
        Size -= AsciiStrSize (TableStrings[i]);
      }
    }
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status = SmbiosProtocol->Add (SmbiosProtocol, NULL, &SmbiosHandle, Record);

  FreePool (Record);
  return Status;
}

/**
   Installs the SMBIOS TPM Device Information type 43

   @retval EFI_UNSUPPORTED      - Could not locate SMBIOS protocol
   @retval EFI_OUT_OF_RESOURCES - Failed to allocate memory for SMBIOS TPM Device Information type.
   @retval EFI_SUCCESS          - Successfully installed SMBIOS TPM Device Information type.
**/
EFI_STATUS
EFIAPI
AddSmbiosTpmDeviceTable (
  VOID
  )
{
  EFI_STATUS          Status;
  UINT8               TpmDeviceType;
  CHAR8               *StringBuffer;
  UINTN               StringBufferLength;
  UINT32              ManufacturerID;
  UINT32              TempManufacturerIDValue;
  UINT32              FirmwareVersion1;
  UINT32              FirmwareVersion2;
  EFI_SMBIOS_PROTOCOL *SmbiosProtocol;
  EFI_GUID            *TpmInstanceGuid;

  DEBUG ((DEBUG_INFO, "[SMBIOS] AddSmbiosTpmDeviceTable ()\n"));

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &SmbiosProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error locating gEfiSmbiosProtocolGuid. Status = %r\n", Status));
    return Status;
  }

  //
  // Check for TPM presence and type.
  //
  TpmDeviceType = TpmDeviceNone;
  TpmInstanceGuid = PcdGetPtr (PcdTpmInstanceGuid);
  if (TpmInstanceGuid != NULL) {
    if (CompareGuid (TpmInstanceGuid, &gEfiTpmDeviceInstanceTpm20DtpmGuid)) {
      TpmDeviceType = TpmDevice20TPM;
    }
  }

  //
  // TPM Description string buffer allocation
  //
  StringBufferLength = SMBIOS_STRING_MAX_LENGTH;
  StringBuffer = AllocateZeroPool (StringBufferLength * sizeof (CHAR8));

  if (StringBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    return Status;
  }

  //
  // TPM Type specific fields
  //
  switch (TpmDeviceType) {

  case TpmDevice20TPM:
    //
    // ManufacturerID defined in TCG Vendor ID Registry.
    //
    Status = Tpm2GetCapabilityManufactureID (&ManufacturerID);
    SmbiosTableType43Data.VendorID[0] = (ManufacturerID >> 0)  & 0xFF;
    SmbiosTableType43Data.VendorID[1] = (ManufacturerID >> 8)  & 0xFF;
    SmbiosTableType43Data.VendorID[2] = (ManufacturerID >> 16) & 0xFF;

    //
    //   May be tailed with 0x00 (ASCII_NULL_CHAR) or 0x20 (ASCII_SPACE_CHAR)
    //
    TempManufacturerIDValue = (ManufacturerID >> 24);
    if ((TempManufacturerIDValue) == ASCII_NULL_CHAR || ((TempManufacturerIDValue) == ASCII_SPACE_CHAR)) {
      //  HID containing PNP ID "NNN####"
      SmbiosTableType43Data.VendorID[3] = ASCII_NULL_CHAR;
    } else {
      //  HID containing ACP ID "NNNN####"
      SmbiosTableType43Data.VendorID[3] = (TempManufacturerIDValue) & 0xFF;
    }

    //
    // Spec version for TPM 2.0 devices.
    //
    SmbiosTableType43Data.MajorSpecVersion = 2;
    SmbiosTableType43Data.MinorSpecVersion = 0;

    //
    // Firmware Version
    //
    Status = Tpm2GetCapabilityFirmwareVersion (&FirmwareVersion1, &FirmwareVersion2);
    SmbiosTableType43Data.FirmwareVersion1 = FirmwareVersion1;
    SmbiosTableType43Data.FirmwareVersion2 = FirmwareVersion2;

    //
    // Format TPM details into Description string
    //
    AsciiSPrint (
      StringBuffer,
      StringBufferLength,
      "TPM %d.%d, ManufacturerID: %c%c%c%c, Firmware Version: 0x%08x.0x%08x",
      SmbiosTableType43Data.MajorSpecVersion,
      SmbiosTableType43Data.MinorSpecVersion,
      SmbiosTableType43Data.VendorID[0],
      SmbiosTableType43Data.VendorID[1],
      SmbiosTableType43Data.VendorID[2],
      SmbiosTableType43Data.VendorID[3],
      SmbiosTableType43Data.FirmwareVersion1,
      SmbiosTableType43Data.FirmwareVersion2
      );

    SmbiosTableType43Data.Description = STRING_1;
    SmbiosTableType43Strings.Description = StringBuffer;

    //
    // TPM Characteristics
    //   Set bit 2 (TPM Characteristics are not supported)
    //
    SmbiosTableType43Data.Characteristics = BIT2;
    break;

  default:
    DEBUG ((DEBUG_ERROR, "[SMBIOS] No supported TPM present. Skipping installation of Smbios TPM Device (record 43) table.\n"));
    Status = EFI_UNSUPPORTED;
    break;
  }

  if (Status != EFI_SUCCESS) {
    return Status;
  }

  //
  // Generate SMBIOS TPM Device Record (Type 43)
  //
  Status = AddSmbiosEntry (
             (EFI_SMBIOS_TABLE_HEADER *) &SmbiosTableType43Data,
             (CHAR8 **) &SmbiosTableType43Strings,
             SMBIOS_TYPE43_NUMBER_OF_STRINGS,
             SmbiosProtocol);

  FreePool (StringBuffer);
  return Status;
}


/**
   Standard EFI driver point. This driver creates the SMBIOS TPM device information (Type 43) record
   for the detected device.

   @param[in] ImageHandle     Handle for the image of this driver
   @param[in] SystemTable     Pointer to the EFI System Table

   @retval EFI_SUCCESS        The data was successfully stored.

**/
EFI_STATUS
EFIAPI
SmbiosTpmDeviceInfoEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS           Status;
  DEBUG ((DEBUG_INFO, "SmbiosTpmDeviceInfoEntryPoint(): Entry\n"));

  //
  // Add Smbios type 43 table.
  //
  Status = AddSmbiosTpmDeviceTable ();

  DEBUG ((DEBUG_INFO, "SmbiosTpmDeviceInfoEntryPoint(): Exit - Status = %r\n", Status));
  return Status;
}
