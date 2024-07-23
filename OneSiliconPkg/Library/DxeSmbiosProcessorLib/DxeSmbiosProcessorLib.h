/** @file
  Header file for SMBIOS related functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
  System Management BIOS (SMBIOS) Reference Specification v3.0.0
  dated 2015-Feb-12 (DSP0134)
  http://www.dmtf.org/sites/default/files/standards/documents/DSP0134_3.0.0.pdf
**/

#ifndef _DXE_SMBIOS_PROCESSOR_LIB_H_
#define _DXE_SMBIOS_PROCESSOR_LIB_H_

///
/// Non-static SMBIOS table data to be filled later with a dynamically generated value
///
#define TO_BE_FILLED  0
#define TO_BE_FILLED_STRING  " "        ///< Initial value should not be NULL

///
/// String references in SMBIOS tables. This eliminates the need for pointers. See spec for details.
///
#define NO_STRING_AVAILABLE  0
#define STRING_1  1
#define STRING_2  2
#define STRING_3  3
#define STRING_4  4
#define STRING_5  5
#define STRING_6  6
#define STRING_7  7

#pragma pack(1)
typedef struct {
  CHAR8 *Socket;
  CHAR8 *ProcessorManufacturer;
  CHAR8 *ProcessorVersion;
  CHAR8 *SerialNumber;
  CHAR8 *AssetTag;
  CHAR8 *PartNumber;
} SMBIOS_TYPE4_STRING_ARRAY;
#define SMBIOS_TYPE4_NUMBER_OF_STRINGS  6
#pragma pack()

#endif
