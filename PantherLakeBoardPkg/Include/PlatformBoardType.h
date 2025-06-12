/** @file
  BoardType used for Platform Info Data entries in the SBIOS/Setup/AcpiASL.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#ifndef _PLATFORM_BOARD_TYPE_H_
#define _PLATFORM_BOARD_TYPE_H_

#define BoardTypeRvp        0x00    // Reference Validation Platform
#define BoardTypeRvpSthi    0x01    // Boards with Special Test Hooks
#define BoardTypeRvpPpv     0x02    // Boards used for Processor Platform Validation
#define BoardTypeFfrd       0x03    // Form factor reference design Boards
#define BoardTypeRvpErb     0x04    // ERB Boards
#define BoardTypeRvpCpv     0x05    // Boards used for Chipset Platform Validation
#define BoardTypeSv         0x06    // Boards used for Silicon Validation
#define BoardTypeUpServer   0x07    // Server Boards
#define BoardTypeFfvs       0x08    // Form factor Validation system
#define BoardTypeTdv        0x09    // Technology Development Vehicle
#define BoardTypeEmbCrb     0x0A    // Embedded CRB Boards
#define BoardTypeMax        0x0B

#endif
