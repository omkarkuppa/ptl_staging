/** @file
  Header of the CRC-32C standard implementation

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

#ifndef _CRC_32C_H_
#define _CRC_32C_H_

#define CRC32C_POLY_REVERSED      0x82F63B78
#define CRC32C_POLY_NORMAL        0x1EDC6F41

#define CRC32C_INITIAL_REMAINDER  0xFFFFFFFF

/**
  Generate a CRC-32C table that can speed up CRC calculation by table look up
**/
VOID
Crc32cInit (
  VOID
  );

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
  );

#endif
