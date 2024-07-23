/**@file
  Private mock for the I2C Power Meter Driver.

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

#include <PrivateMock.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseCryptLib.h>

/**
  Generate random data.

  @param[in, out] Buffer      The buffer to return data.
  @param[in]      BufferSize  The number of bytes in Buffer to fill with random data.
  @param[in]      IsWord      If random data with [0-9], [A-Z], [a-z] only.

  @retval TRUE                Generate random data success.
  @retval FALSE               Generate random data fail.
**/
BOOLEAN
RandomData (
  IN OUT VOID     *Buffer,
  IN     UINTN    BufferSize,
  IN     BOOLEAN  IsWord
  )
{
  UINTN  Index;
  UINT8  Temp;
  UINTN  Length;

  if (Buffer == NULL || BufferSize == 0) {
    return FALSE;
  }

  if (!RandomSeed (NULL, 0)) {
    return FALSE;
  }

  ZeroMem (Buffer, BufferSize);
  Length = IsWord ? BufferSize - 1 : BufferSize;

  for (Index = 0; Index < Length; Index++) {
    while (TRUE) {
      if (!RandomBytes (&Temp, sizeof (UINT8))) {
        return FALSE;
      }
      if (!IsWord) {
        break;
      }
      if ((Temp >= '0' && Temp <= '9') || (Temp >= 'A' && Temp <= 'Z') || (Temp >= 'a' && Temp <= 'z')) {
        break;
      }
      if (Temp == 0) {
        return TRUE;
      }
    }
    SetMem ((UINT8*) Buffer + Index, sizeof (UINT8), Temp);
  }

  return TRUE;
}

/**
  Generate random Power Mtr Data for unit test.

  @param[in, out] Output  The buffer to return data.

  @retval TRUE            Generate random data success.
  @retval FALSE           Generate random data fail.
**/
BOOLEAN
GenerateRandomPowerMtrData (
  IN OUT POWER_METER_PKG  **Output
  )
{
  POWER_METER_PKG  *PowerMtrBuffer;
  UINT16           DevCount;
  UINTN            Index;
  CHAR8            *RailName;

  PowerMtrBuffer = AllocateZeroPool (sizeof (POWER_METER_PKG));
  if (PowerMtrBuffer == NULL) {
    return FALSE;
  }

  if (!RandomData (PowerMtrBuffer, sizeof (POWER_METER_PKG), FALSE)) {
    return FALSE;
  }

  PowerMtrBuffer->DeviceSize %= POWERMETER_MAX_DEVICES;

  for (DevCount = 0; DevCount < POWERMETER_MAX_DEVICES; DevCount++) {
    for (Index = 0; Index < 4; Index++) {
      RailName = PowerMtrBuffer->PmConfigPkgArray[DevCount].RailDataPkg[Index].RailName;
      if (!RandomData (RailName, MAX_PM_DEVICE_STRING_LENGTH * sizeof (CHAR8), TRUE)) {
        return FALSE;
      }
    }
  }

  *Output = PowerMtrBuffer;
  return TRUE;
}

/**
  Get ACPI Table raw data by file path

  @param[out] FileData          ACPI Table RAW data
  @param[out] FileSize          ACPI Table size

  @retval EFI_SUCCESS           Get ACPI table successfully.
  @retval EFI_OUT_OF_RESOURCES  Don't have any resource to allocate memory.
  @retval EFI_NOT_FOUND         File path not found.

**/

EFI_STATUS
EFIAPI
MockI2cPmDataAcpiTableFile (
  OUT UINT8  **FileData,
  OUT UINTN  *FileSize
  )
{
  FILE*  FilePointer;
  CHAR8* FileName = mock_ptr_type (CHAR8*);
  UINT8* I2cPmAcpiTableFile;
  CHAR8* AbsoluteFileName;
  UINTN  Index;

  AbsoluteFileName = malloc (AsciiStrSize (__FILE__) + AsciiStrSize (FileName));
  CopyMem (AbsoluteFileName, __FILE__, AsciiStrSize (__FILE__));
  CopyMem (AsciiStrStr (AbsoluteFileName, "Intel\\Features"), FileName, AsciiStrSize (FileName));
  FileName = AbsoluteFileName;

  if (fopen_s (&FilePointer, FileName, "r") != 0) {
    return EFI_NOT_FOUND;
  }
  if (FilePointer == NULL) {
    return EFI_NOT_FOUND;
  }

  fseek (FilePointer, 0L, SEEK_END);

  *FileSize = ftell (FilePointer);

  I2cPmAcpiTableFile = malloc (*FileSize);
  if (I2cPmAcpiTableFile == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  fseek (FilePointer, 0L, SEEK_SET);
  fread (I2cPmAcpiTableFile, *FileSize, 1, FilePointer);

  for (Index = 0; Index < *FileSize; Index++) {
    I2cPmAcpiTableFile [Index] &= 0xFF;
  }

  *FileData = I2cPmAcpiTableFile;
  fclose (FilePointer);
  free (AbsoluteFileName);

  return EFI_SUCCESS;
}

/**
  Mock AcpiTableProtocol InstallAcpiTable: The InstallAcpiTable() function allows a caller to
  install an ACPI table.

  @param[in]  This                 A pointer to a EFI_ACPI_TABLE_PROTOCOL.
  @param[in]  AcpiTableBuffer      A pointer to a buffer containing the ACPI table to be installed.
  @param[in]  AcpiTableBufferSize  Specifies the size, in bytes, of the AcpiTableBuffer buffer.
  @param[out] TableKey             Returns a key to refer to the ACPI table.

  @retval EFI_SUCCESS              The table was successfully inserted
  @retval EFI_INVALID_PARAMETER    Either AcpiTableBuffer is NULL, TableKey is NULL, or
                                   AcpiTableBufferSize and the size field embedded in the ACPI table
                                   pointed to by AcpiTableBuffer are not in sync.
  @retval EFI_OUT_OF_RESOURCES     Insufficient resources exist to complete the request.
  @retval EFI_ACCESS_DENIED        The table signature matches a table already present in the system
                                   and platform policy does not allow duplicate tables of this type.
**/
EFI_STATUS
EFIAPI
MockInstallAcpiTable (
  IN  EFI_ACPI_TABLE_PROTOCOL  *This,
  IN  VOID                     *AcpiTableBuffer,
  IN  UINTN                    AcpiTableBufferSize,
  OUT UINTN                    *TableKey
  )
{
  VOID  *MockTable;

  MockTable = *mock_ptr_type (VOID **);

  //
  // Check for MockTable content.
  //
  if (MockTable != NULL) {
    DEBUG ((DEBUG_ERROR, "MockTable is not NULL!!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  MockTable = AllocateZeroPool (AcpiTableBufferSize);
  if (MockTable == NULL) {
    DEBUG ((DEBUG_ERROR, "Can't locate memory for MockTable!!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Check for invalid input parameters
  //
  if ((AcpiTableBuffer == NULL) || (TableKey == NULL)
    || (((EFI_ACPI_DESCRIPTION_HEADER *) AcpiTableBuffer)->Length != AcpiTableBufferSize)) {
    return EFI_INVALID_PARAMETER;
  }

  CopyMem (MockTable, AcpiTableBuffer, AcpiTableBufferSize);

  return EFI_SUCCESS;
}

/**
  Mock FirmwareVolume2Protocol ReadSection: Locates the requested section within a file and returns
  it in a buffer.

  @param[in]      This                  Indicates the EFI_FIRMWARE_VOLUME2_PROTOCOL instance.
  @param[in]      NameGuid              Pointer to an EFI_GUID, which indicates the file name from
                                        which the requested section will be read.
  @param[in]      SectionType           Indicates the section type to return. SectionType in
                                        conjunction with SectionInstance indicates which section to
                                        return.
  @param[in]      SectionInstance       Indicates which instance of sections with a type of
                                        SectionType to return. SectionType in conjunction with
                                        SectionInstance indicates which section to return.
                                        SectionInstance is zero based.
  @param[in, out] Buffer                Pointer to a pointer to a buffer in which the section
                                        contents are returned, not including the section header.
  @param[in, out] BufferSize            Pointer to a caller-allocated UINTN.
                                        It indicates the size of the memory represented by Buffer.
  @param[out]     AuthenticationStatus  Pointer to a caller-allocated UINT32 in which the
                                        authentication status is returned.

  @retval EFI_SUCCESS                   The call completed successfully.
  @retval EFI_WARN_BUFFER_TOO_SMALL     The caller-allocated buffer is too small to contain the
                                        requested output. The buffer is filled and the output is
                                        truncated.
  @retval EFI_OUT_OF_RESOURCES          An allocation failure occurred.
  @retval EFI_NOT_FOUND                 The requested file was not found in the firmware volume.
                                        EFI_NOT_FOUND The requested section was not found in the
                                        specified file.
  @retval EFI_DEVICE_ERROR              A error occurred when attempting to access the
                                        firmware volume.
  @retval EFI_ACCESS_DENIED             The firmware volume is configured to disallow reads.
                                        EFI_PROTOCOL_ERROR The requested section was not found,
                                        but the file could not be fully parsed because a required
                                        GUIDED_SECTION_EXTRACTION_PROTOCOL was not found. It is
                                        possible the requested section exists within the file and
                                        could be successfully extracted once the required
                                        GUIDED_SECTION_EXTRACTION_PROTOCOL is published.
**/
EFI_STATUS
EFIAPI
MockFwVol2ReadSection (
  IN CONST EFI_FIRMWARE_VOLUME2_PROTOCOL  *This,
  IN CONST EFI_GUID                       *NameGuid,
  IN       EFI_SECTION_TYPE               SectionType,
  IN       UINTN                          SectionInstance,
  IN OUT   VOID                           **Buffer,
  IN OUT   UINTN                          *BufferSize,
  OUT      UINT32                         *AuthenticationStatus
  )
{
  check_expected_ptr (This);
  check_expected_ptr (NameGuid);
  check_expected (SectionType);
  check_expected (SectionInstance);
  check_expected_ptr (Buffer);
  check_expected_ptr (BufferSize);
  check_expected_ptr (AuthenticationStatus);

  *Buffer = mock_ptr_type (VOID*);
  *BufferSize = mock_type (UINTN);

  return mock_type (EFI_STATUS);
}

/**
  Mock FirmwareVolume2Protocol ReadFile: Retrieves a file and/or file information from the firmware
  volume.

  @param[in]      FwVl                  Indicates the EFI_FIRMWARE_VOLUME2_PROTOCOL instance.
  @param[in]      NameGuid              Pointer to an EFI_GUID, which is the file name.
                                        All firmware file names are EFI_GUIDs. A single firmware
                                        volume must not have two valid files with the same file
                                        name EFI_GUID.
  @param[in, out] Buffer                Pointer to a pointer to a buffer in which the file contents
                                        are returned, not including the file header.
  @param[in, out] BufferSize            Pointer to a caller-allocated UINTN. It indicates the size
                                        of the memory represented by Buffer.
  @param[out]     FoundType             Pointer to a caller-allocated EFI_FV_FILETYPE.
  @param[out]     FileAttributes        Pointer to a  caller-allocated EFI_FV_FILE_ATTRIBUTES.
  @param[out]     AuthenticationStatus  Pointer to a caller-allocated UINT32 in which the
                                        authentication status is returned.

  @retval EFI_SUCCESS                   The call completed successfully.
  @retval EFI_WARN_BUFFER_TOO_SMALL     The buffer is too small to contain the requested output.
                                        The buffer is filled and the output is truncated.
  @retval EFI_OUT_OF_RESOURCES          An allocation failure occurred.
  @retval EFI_NOT_FOUND                 Name was not found in the firmware volume.
  @retval EFI_DEVICE_ERROR              A error occurred when attempting to access the firmware volume.
  @retval EFI_ACCESS_DENIED             The firmware volume is configured to disallow reads.
**/
EFI_STATUS
EFIAPI
MockFwVol2ReadFile (
  IN CONST EFI_FIRMWARE_VOLUME2_PROTOCOL  *This,
  IN CONST EFI_GUID                       *NameGuid,
  IN OUT   VOID                           **Buffer,
  IN OUT   UINTN                          *BufferSize,
  OUT      EFI_FV_FILETYPE                *FoundType,
  OUT      EFI_FV_FILE_ATTRIBUTES         *FileAttributes,
  OUT      UINT32                         *AuthenticationStatus
  )
{
  check_expected_ptr (This);
  check_expected_ptr (NameGuid);
  check_expected_ptr (Buffer);
  check_expected_ptr (BufferSize);
  check_expected_ptr (FoundType);
  check_expected_ptr (FileAttributes);
  check_expected_ptr (AuthenticationStatus);

  return mock_type (EFI_STATUS);
}