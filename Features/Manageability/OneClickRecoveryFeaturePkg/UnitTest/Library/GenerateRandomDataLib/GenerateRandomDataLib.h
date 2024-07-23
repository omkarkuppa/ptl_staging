/** @file
  Library generating random data values of size requested

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
#ifndef __GENERATE_RANDOM_DATA_H__
#define __GENERATE_RANDOM_DATA_H__

#include <Uefi.h>

/**
  Generates random data for array or numbers.

  @param[in, out] Data    Address to send generated data to
  @param[in]      Len     Number of bytes to generate
**/
EFI_STATUS
EFIAPI
GenerateRandomData (
  UINT8 *Data,
  UINTN Length
  );

#endif // __GENERATE_RANDOM_DATA_H__
