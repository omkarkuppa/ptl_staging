/** @file
  Header file for Firmware Status Smbios Table Interface implementation

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _FW_STS_SMBIOS_TABLE_H_
#define _FW_STS_SMBIOS_TABLE_H_

#include <IndustryStandard/SmBios.h>
#include <MeDefines.h>

/**
  BIOS FWSTS Structure SMBIOS Table DEF for OEM Type
  FWSTS structure is defined as SMBIOS OEM type structure (from 128(0x80) through 255 (0xFF)).
  OEM structure type is dynamic type extracted from the "Type 14 - Group Associations Structure"
  to avoid conflicts introduced by hardcoded OEM type.
  The structure can have multiple instance with the same type, each table can specify the name string
  (or PFA) to indicate the FWSTS for any HECI device specified for extension consideration.
  Version 1 -
    The Type14 defined in SMBIOS Specification provides OEMs to specify the arrangement or hierarchy
    of certain components. FWSTS Structure SMBIOS Table type can be extracted from the
    Type 14 "Item Type" field if the "Group Name" is "$MEI". There shall be only one Type14 structure
    with Group Name "$MEI".

    The following is the definition for the SMBIOS table entry Type14 that will contain the
    BIOS FWSTS Structure SMBIOS Table.
    -------------------------------------------------------------------------------------------------
      Offset    Name                  Length          Description
    -------------------------------------------------------------------------------------------------
      00h       Type                  BYTE            14, Group Associations Indicator
    -------------------------------------------------------------------------------------------------
      01h       Length                BYTE            Computed as 5 + (3 bytes for each item in the group).
                                                      The user of this structure determines the number of
                                                      items as (Length -5)/3
    -------------------------------------------------------------------------------------------------
      02h       Handle                WORD            Specify the handle of the structure, a unique
                                                      16-bit number in the range 0 to 0FFFEh
                                                      (for version 2.0) or 0 to 0FEFFh
                                                      (for version 2.1 and later).
    -------------------------------------------------------------------------------------------------
      04h      Group name             BYTE            String number of the string (L "$MEI")
                                                      describing the group
    -------------------------------------------------------------------------------------------------
      05h      Item Type              BYTE            Item (Structure) Type of this member.
                                                      The default value is 0xDB, it can be overrided
                                                      by gSiPkgTokenSpaceGuid.PcdFwStsSmbiosType.
    -------------------------------------------------------------------------------------------------
      06h      Item Handle            WORD            Handle corresponding to this structure
    -------------------------------------------------------------------------------------------------


    The following is the definition for the BIOS FWSTS Structure SMBIOS Table that will contain
    the FWSTS values.
    -------------------------------------------------------------------------------------------------
      Offset      Name                Length          Description
    -------------------------------------------------------------------------------------------------
      00h         Type                BYTE            Specifies the type of the structure Value,
                                                      defined in SMBIOS Type 14 - Group Associations
                                                      Structure
    -------------------------------------------------------------------------------------------------
      01h         Length              BYTE            Specifies the length of the structure, starting
                                                      at the Type field. Length 6 + (25 * N)
    -------------------------------------------------------------------------------------------------
      02h         Handle              WORD            Specify the handle of the structure, a unique
                                                      16-bit number in the range 0 to 0FFFEh
                                                      (for version 2.0) or 0 to 0FEFFh
                                                      (for version 2.1 and later).
    -------------------------------------------------------------------------------------------------
      04h         Version             BYTE            Version of implemented structure.
                                                      Current version is 1.
    -------------------------------------------------------------------------------------------------
      05h         Count               BYTE            Number of MEI device blocks included (n).
                                                      N =1 - One set of FWSTS registers exposed
    -------------------------------------------------------------------------------------------------
      06h + 25    Component Number    BYTE            Component number for which FWSTS is listed.
      * (n-1)

    -------------------------------------------------------------------------------------------------
      07h + 25    FWSTS1              DWORD           The value of 0:22:x PCI offset 0x40
      * (n-1)                                         X : 0, 1, 4, 5
    -------------------------------------------------------------------------------------------------
      0Bh + 25    FWSTS2              DWORD           The value of 0:22:x PCI offset 0x48
      * (n-1)                                         X : 0, 1, 4, 5
    -------------------------------------------------------------------------------------------------
      0Fh + 25    FWSTS3              DWORD           The value of 0:22:x PCI offset 0x60
      * (n-1)                                         X : 0, 1, 4, 5
    -------------------------------------------------------------------------------------------------
      13h + 25    FWSTS4              DWORD           The value of 0:22:x PCI offset 0x64
      * (n-1)                                         X : 0, 1, 4, 5
    -------------------------------------------------------------------------------------------------
      17h + 25    FWSTS5              DWORD           The value of 0:22:x PCI offset 0x68
      * (n-1)                                         X : 0, 1, 4, 5
    -------------------------------------------------------------------------------------------------
      18h + 25    FWSTS6              DWORD           The value of 0:22:x PCI offset 0x6C
      * (n-1)                                         X : 0, 1, 4, 5
    -------------------------------------------------------------------------------------------------
      Length + 5  Component Name      BYTE            (L "MEI1" for 0:22:0, L"MEI2" for 0:22:1,
      * (n-1)                                          L"MEI3" for 0:22:4, L"MEI4" for 0:22:5)
**/

#define MAX_HECI_FUNC             HECI_DEVICE_TO_NUMBER (HECI4)
#define MAX_FWSTS_REG             6
#define T14_FWSTS_STRING          "$MEI"
#define MEI1_FWSTS_STRING         "MEI1"
#define MEI2_FWSTS_STRING         "MEI2"
#define MEI3_FWSTS_STRING         "MEI3"
#define MEI4_FWSTS_STRING         "MEI4"

#pragma pack(1)

///
/// MEI Device Structure
///
typedef struct {
  HECI_DEVICE   HeciDev;
  CHAR8         *String;
} MEI_DEV_STRING;

///
/// FWSTS element structure
///
typedef struct {
  UINT8         ComponentNumber;              ///< Component number
  /**
    The value of 0:22:x PCI offset 0x40
    The value of 0:22:X PCI offset 0x48
    The value of 0:22:X PCI offset 0x60
    The value of 0:22:X PCI offset 0x64
    The value of 0:22:X PCI offset 0x68
    The value of 0:22:X PCI offset 0x6C
  **/
  UINT32        Reg[MAX_FWSTS_REG];
} FWSTS_SET_RECORD;

///
/// FWSTS SMBIOS Record Structure
///
typedef struct {
  SMBIOS_STRUCTURE  SmbiosHeader;
  UINT8             Version;                      ///< Version of implemented structure, current version is 1
  UINT8             Count;                        ///< Number of MEI device groups included
  FWSTS_SET_RECORD  Group[MAX_HECI_FUNC];         ///< Component information
  // CHAR8          ComponentName[MAX_HECI_FUNC]; ///< Component name, size is runtime allocated
} FWSTS_SMBIOS_RECORD;

#pragma pack()

#endif
