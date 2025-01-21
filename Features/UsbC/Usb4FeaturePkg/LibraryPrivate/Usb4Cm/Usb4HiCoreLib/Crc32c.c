/** @file
  CRC-32C standard implementation

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
#include "Crc32c.h"

UINT32 mCrc32cTable[256];

/**
  This internal function reverses bits for 32 bit data.

  @param[in] Value - The data to be reversed.

  @retval Data reversed.
**/
UINT32
ReverseBits (
  UINT32  Value
  )
{
  UINTN   Index;
  UINT32  NewValue;

  NewValue = 0;
  for (Index = 0; Index < 32; Index++) {
    if ((Value & (1 << Index)) != 0) {
      NewValue = NewValue | (1 << (31 - Index));
    }
  }

  return NewValue;
}

/**
  Generate a CRC-32C table that can speed up CRC calculation by table look up
**/
VOID
Crc32cInit (
  VOID
  )
{
  UINT32 TableEntry;
  UINT32 Index;
  UINT32 CrcVal;

  for (TableEntry = 0; TableEntry < 256; TableEntry++) {
    CrcVal = ReverseBits ((UINT32) TableEntry);
    for (Index = 0; Index < 8; Index++) {
      if ((CrcVal & 0x80000000) != 0) {
        CrcVal = (CrcVal << 1) ^ CRC32C_POLY_NORMAL;
      } else {
        CrcVal = CrcVal << 1;
      }
    }

    mCrc32cTable[TableEntry] = ReverseBits (CrcVal);
  }
}

/**
  Calculate CRC value of the input data based on CRC-32C standard

  @param[in]  Data     - Pointer to source data for calculating CRC codes
  @param[in]  DataSize - Data length in bytes of source data
  @param[out] CrcOut   - Calculated CRC codes for source data

  @retval EFI_SUCCESS           - CRC calculation success
  @retval EFI_INVALID_PARAMETER - Invalid input parameter is specified
**/
EFI_STATUS
CalCrc32c (
  IN  UINT8     *Data,
  IN  UINT32    DataSize,
  OUT UINT32    *CrcOut
  )
{
  UINT32  Crc;
  UINT32  Index;
  UINT8 *Ptr;

  if (Data == NULL || DataSize == 0 || CrcOut == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Crc = CRC32C_INITIAL_REMAINDER;
  for (Index = 0, Ptr = Data; Index < DataSize; Index++, Ptr++) {
    Crc = (Crc >> 8) ^ mCrc32cTable[(UINT8)Crc ^ *Ptr];
  }

  *CrcOut = Crc ^ 0xffffffff;
  return EFI_SUCCESS;
}
