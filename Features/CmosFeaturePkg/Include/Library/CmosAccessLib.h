/** @file
  CmosAccessLib header file.

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
#ifndef _CMOS_ACCESS_LIB_H_
#define _CMOS_ACCESS_LIB_H_

/**
  Read a byte value from a CMOS address.

  @param [in] Address   Location to read from CMOS

  @return The byte value read from the CMOS address.
**/
UINT8
EFIAPI
CmosRead8 (
  IN  UINT8 Address
  );

/**
  Write a byte value to a CMOS address.

  @param [in] Address Location to write to CMOS.
  @param [in] Data    The byte value write to the CMOS address.
**/
VOID
EFIAPI
CmosWrite8 (
  IN UINT8 Address,
  IN UINT8 Data
  );

/**
  Read a word value from a CMOS address.

  @param [in] Address   Location to read from CMOS

  @return The word value read from the CMOS address.
**/
UINT16
EFIAPI
CmosRead16 (
  IN  UINT8  Address
  );

/**
  Write a word value to a CMOS address.

  @param [in] Address Location to write to CMOS.
  @param [in] Data    The word value write to the CMOS address.
**/
VOID
EFIAPI
CmosWrite16 (
  IN UINT8  Address,
  IN UINT16 Data
  );

/**
  Read a dword value from a CMOS address.

  @param [in] Address   Location to read from CMOS

  @return The dword value read from the CMOS address.
**/
UINT32
EFIAPI
CmosRead32 (
  IN  UINT8  Address
  );

/**
  Write a dword value to a CMOS address.

  @param [in] Address Location to write to CMOS.
  @param [in] Data    The dword value write to the CMOS address.
**/
VOID
EFIAPI
CmosWrite32 (
  IN UINT8  Address,
  IN UINT32 Data
  );

/**
  Initialize the CMOS.

  It initialize the CMOS area when Force is TRUE or the checksum is incorrect.

  @param[in]  Force  TRUE indicating initializing the CMOS area without checking the checksum.

  @retval TRUE  The CMOS is initialized to default value.
  @retval FALSE The CMOS isn't initialized to default value.
**/
BOOLEAN
EFIAPI
CmosInit (
  IN  BOOLEAN     Force
  );

#endif // _CMOS_ACCESS_LIB_H_
