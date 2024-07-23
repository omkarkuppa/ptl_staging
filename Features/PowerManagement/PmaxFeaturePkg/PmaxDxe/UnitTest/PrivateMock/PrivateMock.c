/**@file

    Private mock of the PmaxFeaturePkg.

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

#include <Uefi.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UnitTestLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <Protocol/FirmwareVolume2.h>
#include <Protocol/AcpiTable.h>


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
MockPmaxAcpiTableFile (
  OUT UINT8  **FileData,
  OUT UINTN  *FileSize
  )
{
  FILE*  FilePointer;
  CHAR8* FileName = mock_ptr_type (CHAR8*);
  UINT8* PmaxAcpiTableFile;
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

  PmaxAcpiTableFile = malloc (*FileSize);
  if (PmaxAcpiTableFile == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  fseek (FilePointer, 0L, SEEK_SET);
  fread (PmaxAcpiTableFile, *FileSize, 1, FilePointer);

  for (Index = 0; Index < *FileSize; Index++) {
    PmaxAcpiTableFile [Index] &= 0xFF;
  }

  *FileData = PmaxAcpiTableFile;
  fclose (FilePointer);
  free (AbsoluteFileName);

  return EFI_SUCCESS;
}


/**
  Mock BootService LocateProtocol: Return the first Protocol Interface that matches the Protocol GUID. If
  Registration is passed in, return a Protocol Instance that was just add
  to the system. If Registration is NULL return the first Protocol Interface
  you find.

  @param  Protocol               The protocol to search for
  @param  Registration           Optional Registration Key returned from
                                 RegisterProtocolNotify()
  @param  Interface              Return the Protocol interface (instance).

  @retval EFI_SUCCESS            If a valid Interface is returned
  @retval EFI_INVALID_PARAMETER  Invalid parameter
  @retval EFI_NOT_FOUND          Protocol interface not found
**/
EFI_STATUS
EFIAPI
MockBootServiceLocateProtocol (
  IN EFI_GUID  *Protocol,
  IN VOID      *Registration  OPTIONAL,
  OUT VOID     **Interface
  )
{
  check_expected_ptr (Protocol);
  check_expected_ptr (Registration);
  check_expected_ptr (Interface);
  *Interface = mock_ptr_type (VOID*);

  return mock_type (EFI_STATUS);
}
