/** @file
  Definition header file of MtrrInitMask PPI for MultiIbb feature.

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

#ifndef _MTRR_INIT_MASK_PPI_H_
#define _MTRR_INIT_MASK_PPI_H_

//
// Global ID for the EFI_END_OF_IBB_SIGNAL_PPI
//
#define MTRR_INIT_MASK_PPI_PPI_GUID \
  { \
    0x4ec0c431, 0x47a7, 0x4594, { 0x98, 0x85, 0xbd, 0xde, 0xe3, 0x35, 0x83, 0xd7 } \
  }

extern EFI_GUID gMtrrInitMaskPpiGuid;

#endif // _MTRR_INIT_MASK_PPI_H_
