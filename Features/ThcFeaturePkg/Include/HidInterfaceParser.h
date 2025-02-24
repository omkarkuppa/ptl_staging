/** @file
  HID Protocol Interface

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef HID_INTERFACE_PARSER_H_
#define HID_INTERFACE_PARSER_H_

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Protocol/HidInterfaceProtocol.h>

/*
  HID's InputReportDescriptor consists of variable length tokens.
  On entry, Descriptor points to InputReportDescriptor's start or a boundary
    between two tokens somewhere inside descriptor. Size means number of bytes
    before end of Descriptor.
  Function returns a single Token.
  On exit, Descriptor points next boundary between tokens, just after the token
    that was returned. Size is decreased by as many bytes as Descriptor pointer
    was moved forward.
  Tokens in Long Item format are only partially supported; they will return
    invalid value but Descriptor pointer and Size will be updated correctly to
    allow further parsing
*/
HID_TOKEN
STATIC
HidGetToken (
  IN OUT UINT8  **Descriptor,
  IN OUT UINT32 *Size
  );

/*
  Stack contains a set of data retrieved from parsing InputReportDescriptor.
  This function checks if that set of data constitutes a valid InputData
  dictionary, and if so then puts it into ReportTable, table of dictionaries.
  Unless this is the 1st dictionary for a particular device, this means
  allocating new bigger table and deallocating old table

  @param[in]     Stack             A pointer to the PARSER_STACK
  @param[in/out] ReportTable       Pointer to the final report table that contains all the Reports

*/
VOID
STATIC
HidExportReport (
  IN     HID_PARSER_STACK       *Stack,
  IN OUT HID_INPUT_REPORT_TABLE *ReportTable
  );

/*
  Stack is pointer to a not-yet-complete InputReport dictionary.
  This function consumes Token retrieved from InputReportDescriptor stream
  and uses it to update the dictionary.
  Once a dictionary is completed, it calls ExportReport() which puts the
  dictionary in a table of dictionaries. Then it clears internal data
  and prepares to build a new one.
  This implementation of Descriptor parser ignores all types of data except for
  information on how to decode Button presses and X/Y touch coordinates, as this
  is the only info relevant for touchpanels.

  @param[in/out] Stack                 A pointer to the PARSER_STACK
  @param[in]     Token                 Current Token
  @param[in/out] ReportTable           Pointer to the final report table that contains all the Reports
*/
VOID
STATIC
HidUpdateStack (
  IN OUT HID_PARSER_STACK       *Stack,
  IN     HID_TOKEN              Token,
  IN OUT HID_INPUT_REPORT_TABLE *ReportTable,
  IN OUT HID_GET_REPORT_FORMAT  *GetReportPacket
  );

/*
  Retrieves single bit from multi-byte stream

  @param[in]  InputStream   Data
  @param[in]  Bit           Bit offset
*/
UINT32
STATIC
HidAccessBit (
  IN UINT8  *InputStream,
  IN UINT32 Bit
  );

/**
  Dump report table
  - shows the amount of parsed Reports along with their collections

  @param[in] ReportTable           Pointer to the final report table that contains all the Reports
**/
VOID
STATIC
HidShowReportTable (
  IN HID_INPUT_REPORT_TABLE *ReportTable
  );

/**
  Convert Output X/Y Data to INT

  @param[in]  BitCount      Number of BITs which indicates the size of Output
  @param[in]  Output        X/Y Data to convert

  @retval INT32             Converted X/Y data
**/
INT32
STATIC
ConvertOutputToInt (
  UINT32 BitCount,
  UINT32 Output
  );

#endif // HID_INTERFACE_PARSER_H_
