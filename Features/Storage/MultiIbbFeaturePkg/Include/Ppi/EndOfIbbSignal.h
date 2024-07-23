/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _END_OF_IBB_SIGNAL_PPI_H_
#define _END_OF_IBB_SIGNAL_PPI_H_

//
// Global ID for the EFI_END_OF_IBB_SIGNAL_PPI
//
#define EFI_END_OF_IBB_SIGNAL_PPI_GUID \
  { \
    0xeed52bb, 0xfa2e, 0x4b7e, { 0x96, 0x7b, 0xe5, 0x83, 0x41, 0x8d, 0xd2, 0x83 } \
  }

extern EFI_GUID gEfiEndOfIbbSignalPpiGuid;

#endif // _END_OF_IBB_SIGNAL_PPI_H_
