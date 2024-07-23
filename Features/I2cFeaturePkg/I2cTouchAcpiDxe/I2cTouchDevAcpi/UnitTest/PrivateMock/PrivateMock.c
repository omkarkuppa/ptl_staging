/**@file
  Private mock for the I2c Touch Device Acpi Driver

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

#include <PrivateMock.h>

/**
  Mock for GetDevTablePointer.

  @retval [I2CAcpiTable] I2c Acpi TabLE Pointer
**/
EFI_ACPI_DESCRIPTION_HEADER *
EFIAPI
MockGetDevTablePointer (
  VOID
  )
{
  return mock_ptr_type (EFI_ACPI_DESCRIPTION_HEADER*);
}

/**
  Get ACPI Table raw data by file path.

  @param[out] FileData          ACPI Table RAW data
  @param[out] FileSize          ACPI Table size

  @retval EFI_SUCCESS           Get ACPI table successfully.
  @retval EFI_OUT_OF_RESOURCES  Don't have any resource to allocate memory.
  @retval EFI_NOT_FOUND         File path not found.

**/
EFI_STATUS
EFIAPI
MockI2cAcpiTableFile (
  OUT UINT8  **FileData,
  OUT UINTN  *FileSize
  )
{
  FILE   *FilePointer;
  CHAR8  *FileName = mock_ptr_type (CHAR8*);
  UINT8  *I2cAcpiTableFile;
  CHAR8  *AbsoluteFileName;
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

  I2cAcpiTableFile = malloc (*FileSize);
  if (I2cAcpiTableFile == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  fseek (FilePointer, 0L, SEEK_SET);
  fread (I2cAcpiTableFile, *FileSize, 1, FilePointer);

  for (Index = 0; Index < *FileSize; Index++) {
    I2cAcpiTableFile [Index] &= 0xFF;
  }

  *FileData = I2cAcpiTableFile;
  fclose (FilePointer);
  free (AbsoluteFileName);

  return EFI_SUCCESS;
}
