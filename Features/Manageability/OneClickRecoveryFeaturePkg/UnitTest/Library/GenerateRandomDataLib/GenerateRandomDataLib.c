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

#include "GenerateRandomDataLib.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <Library/DebugLib.h>
#include <Library/UnitTestLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

/**
  Generates random data for array or numbers.

  @param[in, out] Data    Address to send generated data to
  @param[in]      Length  Number of bytes to generate
**/
EFI_STATUS
EFIAPI
GenerateRandomData (
  UINT8 *Data,
  UINTN Length
)
{
  if (Data == NULL || Length == 0) {
    DEBUG ((DEBUG_INFO, "[%a] Data NULL or Length is 0! \n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }
  // Generate random values for the byte array
  for (UINTN i = 0; i < Length; i++) {
    Data[i] = rand() % 256;
  }
  return EFI_SUCCESS;
}
