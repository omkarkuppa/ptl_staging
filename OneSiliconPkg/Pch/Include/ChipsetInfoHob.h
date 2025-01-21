/** @file
  This file contains definitions of Chipset Info HOB.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _CHIPSET_INFO_HOB_H_
#define _CHIPSET_INFO_HOB_H_

/**
  <b>Revision 1</b>:  - Initial version.
  <b>Revision 2</b>:  - Added ChipsetInit version 3.
  The ChipsetInit Info structure provides the information of ME ChipsetInit CRC and BIOS ChipsetInit CRC.
**/
typedef struct {
  UINT8             Revision;
  UINT8             BaseVersion;
  UINT8             OemVersion;
  UINT8             Rsvd;
  UINT16            BaseCrc;
  UINT16            SusCrc;
  UINT16            OemCrc;
  UINT16            IntelMajor;  // ChipsetInit Version 3
  UINT16            IntelMinor;  // ChipsetInit Version 3
  UINT16            IntelHotFix; // ChipsetInit Version 3
  UINT16            IntelBuild;  // ChipsetInit Version 3
  UINT16            OemMajor;    // ChipsetInit Version 3
  UINT16            OemMinor;    // ChipsetInit Version 3
  UINT16            OemHotFix;   // ChipsetInit Version 3
  UINT16            OemBuild;    // ChipsetInit Version 3
  UINT8             Rsvd1[2];
} CHIPSET_INIT_INFO;

extern EFI_GUID gChipsetInitHobGuid;

#define CHIPSET_INIT_INFO_REVISION  2

#endif // _CHIPSET_INFO_HOB_H_

