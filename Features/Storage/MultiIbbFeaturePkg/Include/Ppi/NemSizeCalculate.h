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

#ifndef _NEM_SIZE_CALCULATE_PPI_H_
#define _NEM_SIZE_CALCULATE_PPI_H_

//
// Global ID for the NEM_SIZE_CALCULATE_PPI
//
#define MTRR_RANGE_CHANGE_PPI_GUID \
  { \
    0x5ec190dc, 0xfe2d, 0x4e79, { 0xbf, 0x43, 0xf0, 0xb0, 0xb3, 0x4f, 0xc9, 0x3b }\
  }

extern EFI_GUID gNemSizeCalculatePpiGuid;

#endif // _NEM_SIZE_CALCULATE_PPI_H_
