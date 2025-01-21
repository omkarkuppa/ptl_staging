/** @file

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
#ifndef _BOARDPOSTCODE_H_
#define _BOARDPOSTCODE_H_

//
// GENERAL USAGE GUIDELINES
//

/**
[definition]
PostCode = XYZZ
X - "D"=premem, "9"=postmem, "8"=SMM, "7"=DXE
Y - "5"=board driver
ZZ - "00"=entry, "7F"=exit
 - 1 - board init premem: entry/exit (0xD500/0xD57F)
 - 2 - board init postmem: entry/exit (0x9500/0x957F)
 - 3 - board init DXE: entry/exit (0x7500/0x757F)
 - 4 - board init SMM: entry/exit (0x8500/0x857F)
 - 5 - BIOS S3 entry (0xB503) means BioS PC 03, to differentiate with ACPI _PTS PC
 - 6 - BIOS S4 entry (0xB504) means BioS PC 04, to differentiate with ACPI _PTS PC
 - 7 - BIOS S5 entry (0xB505) means BioS PC 05, to differentiate with ACPI _PTS PC
*/

#define BOARD_INIT_PREMEM_ENTRY     0xD500
#define BOARD_INIT_PREMEM_EXIT      0xD57F
#define BOARD_INIT_POSTMEM_ENTRY    0x9500
#define BOARD_INIT_POSTMEM_EXIT     0x957F
#define BOARD_INIT_DXE_ENTRY        0x7500
#define BOARD_INIT_DXE_EXIT         0x757F
#define BOARD_INIT_SMM_ENTRY        0x8500
#define BOARD_INIT_SMM_EXIT         0x857F
#define BIOS_S3_ENTRY               0xB503
#define BIOS_S4_ENTRY               0xB504
#define BIOS_S5_ENTRY               0xB505

#endif
