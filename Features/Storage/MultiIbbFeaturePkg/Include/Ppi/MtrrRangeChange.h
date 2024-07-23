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

#ifndef _MTRR_RANGE_CHANGE_PPI_H_
#define _MTRR_RANGE_CHANGE_PPI_H_

//
// Global ID for the EFI_END_OF_IBB_SIGNAL_PPI
//
#define MTRR_RANGE_CHANGE_PPI_GUID \
  { \
    0xad35a0af, 0x7a67, 0x4aea, { 0x8b, 0x5a, 0x1a, 0xfc, 0xae, 0x36, 0xda, 0x68 } \
  }

extern EFI_GUID gMtrrRangeChangePpiGuid;

#endif // _MTRR_RANGE_CHANGE_PPI_H_
