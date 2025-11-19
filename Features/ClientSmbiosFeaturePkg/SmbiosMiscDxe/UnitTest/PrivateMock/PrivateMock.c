/** @file
  Private mock for the Smbios misc dxe driver.

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
#include "PrivateMock.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <Library/MemoryAllocationLib.h>

EFI_SMBIOS_PROTOCOL      *gSmbiosProtocol = NULL;
EFI_SMBIOS_TABLE_HEADER  *gSmbiosTable[DEFAULT_SMBIOS_PLATFORM_INFO_SIZE];
UINTN                    gSmbiosTableIndex = 0;

/**

  Get the full size of SMBIOS structure including optional strings that follow the formatted structure.

  @param[in]   This                   The EFI_SMBIOS_PROTOCOL instance.
  @param[in]   Head                   Pointer to the beginning of SMBIOS structure.
  @param[out]  Size                   The returned size.
  @param[out]  NumberOfStrings        The returned number of optional strings that follow the formatted structure.

  @retval EFI_SUCCESS           Size retured in Size.
  @retval EFI_INVALID_PARAMETER Input SMBIOS structure mal-formed or Size is NULL.

**/
EFI_STATUS
EFIAPI
GetSmbiosStructureSize (
  IN   CONST EFI_SMBIOS_PROTOCOL  *This,
  IN   EFI_SMBIOS_TABLE_HEADER    *Head,
  OUT  UINTN                      *Size,
  OUT  UINTN                      *NumberOfStrings
  )
{
  UINTN  FullSize;
  UINTN  StrLen;
  UINTN  MaxLen;
  INT8   *CharInStr;

  if ((Size == NULL) || (NumberOfStrings == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  FullSize         = Head->Length;
  CharInStr        = (INT8 *) Head + Head->Length;
  *Size            = FullSize;
  *NumberOfStrings = 0;
  StrLen           = 0;
  MaxLen           = 0xFFFF;
  //
  // look for the two consecutive zeros, check the string limit by the way.
  //
  while (*CharInStr != 0 || *(CharInStr+1) != 0) {
    if (*CharInStr == 0) {
      *Size += 1;
      CharInStr++;
    }

    for (StrLen = 0; StrLen < MaxLen; StrLen++) {
      if (*(CharInStr+StrLen) == 0) {
        break;
      }
    }

    if (StrLen == MaxLen) {
      return EFI_INVALID_PARAMETER;
    }

    //
    // forward the pointer
    //
    CharInStr        += StrLen;
    *Size            += StrLen;
    *NumberOfStrings += 1;
  }

  //
  // count ending two zeros.
  //
  *Size += 2;
  return EFI_SUCCESS;
}

/**
  Add an SMBIOS record.

  @param[in]       This                  The EFI_SMBIOS_PROTOCOL instance.
  @param[in]       ProducerHandle        The handle of the controller or driver associated with the SMBIOS information. NULL
                                         means no handle.
  @param[in, out]  SmbiosHandle          On entry, the handle of the SMBIOS record to add. If FFFEh, then a unique handle
                                         will be assigned to the SMBIOS record. If the SMBIOS handle is already in use,
                                         EFI_ALREADY_STARTED is returned and the SMBIOS record is not updated.
  @param[in]       Record                The data for the fixed portion of the SMBIOS record. The format of the record is
                                         determined by EFI_SMBIOS_TABLE_HEADER.Type. The size of the formatted area is defined
                                         by EFI_SMBIOS_TABLE_HEADER.Length and either followed by a double-null (0x0000) or
                                         a set of null terminated strings and a null.

  @retval EFI_SUCCESS           Record was added.

**/
EFI_STATUS
EFIAPI
MockSmbiosAdd (
  IN     CONST EFI_SMBIOS_PROTOCOL      *This,
  IN           EFI_HANDLE               ProducerHandle, OPTIONAL
  IN OUT       EFI_SMBIOS_HANDLE        *SmbiosHandle,
  IN           EFI_SMBIOS_TABLE_HEADER  *Record
  )
{
  UINT8  *SmbiosRecord;
  UINTN  RecordFullSize;
  UINTN  NumberOfStrings;

  check_expected (SmbiosHandle);

  GetSmbiosStructureSize (gSmbiosProtocol, Record, &RecordFullSize, &NumberOfStrings);

  SmbiosRecord = AllocateCopyPool (RecordFullSize, Record);

  gSmbiosTable[gSmbiosTableIndex] = (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord;

  gSmbiosTableIndex = gSmbiosTableIndex + 1;

  return mock_type (EFI_STATUS);
}

/**
  Allow the caller to discover all or some of the SMBIOS records.

  @param[in]       This             The EFI_SMBIOS_PROTOCOL instance.
  @param[in, out]  SmbiosHandle     On entry, points to the previous handle of the SMBIOS record. On exit, points to the
                                    next SMBIOS record handle. If it is FFFEh on entry, then the first SMBIOS record
                                    handle will be returned. If it returns FFFEh on exit, then there are no more SMBIOS records.
  @param[in]       Type             On entry it means return the next SMBIOS record of type Type. If a NULL is passed in
                                    this functionally it ignored. Type is not modified by the GetNext() function.
  @param[out]      Record           On exit, points to the SMBIOS Record consisting of the formatted area followed by
                                    the unformatted area. The unformatted area optionally contains text strings.
  @param[out]      ProducerHandle   On exit, points to the ProducerHandle registered by Add(). If no ProducerHandle was passed into Add() NULL is returned.
                                    If a NULL pointer is passed in no data will be returned

  @retval          EFI_SUCCESS      SMBIOS record information was successfully returned in Record.
  @retval          EFI_NOT_FOUND    The SMBIOS record with SmbiosHandle was the last available record.

**/
EFI_STATUS
EFIAPI
MockSmbiosGetNext(
  IN     CONST EFI_SMBIOS_PROTOCOL     *This,
  IN OUT       EFI_SMBIOS_HANDLE       *SmbiosHandle,
  IN           EFI_SMBIOS_TYPE         *Type              OPTIONAL,
  OUT          EFI_SMBIOS_TABLE_HEADER **Record,
  OUT          EFI_HANDLE              *ProducerHandle    OPTIONAL
  )
{
  check_expected (SmbiosHandle);
  *SmbiosHandle = mock_type (SMBIOS_HANDLE);
  *Record       = mock_ptr_type (EFI_SMBIOS_TABLE_HEADER *);

  return mock_type (EFI_STATUS);
}
