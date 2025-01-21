/** @file
  Header file for Connection Manager mode information

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

#ifndef _USB4_CM_MODE_H_
#define _USB4_CM_MODE_H_

#include <Uefi.h>

//
// CM mode in Setup menu - 3 bits
//
#define USB4_SETUP_CM_MODE_FW       0
#define USB4_SETUP_CM_MODE_SW       1
#define USB4_SETUP_CM_MODE_OS       2
#define USB4_SETUP_CM_MODE_DEBUG    3

//
// CM mode in Pre-boot - 3 bits
//
#define USB4_CM_MODE_FW_CM                 0x00
#define USB4_CM_MODE_SW_CM                 0x01

//
// CM mode switch unsupported bit
//
#define USB4_CM_MODE_SWITCH_UNSUPPORTED    0x08

//
// CM Mode information for the communication with ASL codes
//
// Bit 7   - Valid bit
// Bit 6:4 - Setup CM value (0 = FW CM, 1 = SW CM, 2 = OS, 3 = Pass Through)
// Bit 3   - Reserved
// Bit 2:0 - Pre-boot CM mode (0 = FW CM, 1 = SW CM)
//
// Note: 8 bits value in NVS
//       NVS variable length needs to be incremented if new bits are added.
//
typedef union _USB4_CM_MODE_INFO {
  struct {
    UINT8   CmMode  : 3;
    UINT8   Rsvd    : 1;
    UINT8   CmSetup : 3;
    UINT8   Valid   : 1;
  } Fields;
  UINT8 Value;
} USB4_CM_MODE_INFO, *PUSB4_CM_MODE_INFO;

#endif
