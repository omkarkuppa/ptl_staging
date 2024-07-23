/** @file

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

#include "Uefi.h"

CHAR16 *mBoardIdIndex[] = {
//L"0123456789ABCDEF0123456789ABCDEF"       // Do not exceed 32 characters
  L"TBD",                                   // 0x00
  L"PTL-UH LP5 T3 RVP1",                    // 0x01
  L"PTL-UH CAMM DTBT T3 RVP2",              // 0x02
  L"PTL-UH LP5 T4 RVP3",                    // 0x03
  L"PTL-UH DDR5 T3 RVP4",                   // 0x04
  L"TBD",                                   // 0x05
  L"TBD",                                   // 0x06
  L"TBD",                                   // 0x07
  L"TBD",                                   // 0x08
  L"PTL-UH HMP RVP",                        // 0x09
  L"TBD",                                   // 0x0A
  L"TBD",                                   // 0x0B
  L"TBD",                                   // 0x0C
  L"TBD",                                   // 0x0D
  L"TBD",                                   // 0x0E
  L"TBD",                                   // 0x0F
  L"TBD",                                   // 0x10
  L"TBD",                                   // 0x11
  L"TBD",                                   // 0x12
  L"TBD",                                   // 0x13
  L"TBD",                                   // 0x14
  L"TBD",                                   // 0x15
  L"TBD",                                   // 0x16
  L"TBD",                                   // 0x17
  L"TBD",                                   // 0x18
  L"TBD",                                   // 0x19
  L"TBD",                                   // 0x1A
  L"TBD",                                   // 0x1B
  L"TBD",                                   // 0x1C
  L"TBD",                                   // 0x1D
  L"TBD",                                   // 0x1E
  L"TBD",                                   // 0x1F
  L"TBD",                                   // 0x20
  L"TBD",                                   // 0x21
  L"TBD",                                   // 0x22
  L"TBD",                                   // 0x23
  L"TBD",                                   // 0x24
  L"TBD",                                   // 0x25
  L"TBD",                                   // 0x26
  L"TBD",                                   // 0x27
  L"TBD",                                   // 0x28
  L"TBD",                                   // 0x29
  L"TBD",                                   // 0x2A
  L"TBD",                                   // 0x2B
  L"TBD",                                   // 0x2C
  L"TBD",                                   // 0x2D
  L"TBD",                                   // 0x2E
  L"TBD",                                   // 0x2F
  L"TBD",                                   // 0x30
  L"PTL-UH Simics Board",                    // 0x31
};

UINT8 mSizeOfmBoardIndex = sizeof(mBoardIdIndex)/sizeof(CHAR16*);
