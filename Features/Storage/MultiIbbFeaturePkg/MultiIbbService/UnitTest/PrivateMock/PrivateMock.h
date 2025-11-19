/** @file
  Private mock for the Multi-IBB Service driver.

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

#ifndef _PRIVATE_MOCK_H_
#define _PRIVATE_MOCK_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <PiPei.h>
#include <Library/MtrrLib.h>
#include <Ppi/MultiIbbConfig.h>

VOID                                    *gReturnResult;
MTRR_SETTINGS                           *gRandomMtrrs;
PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI  *gIbbHashInfoPpi;
EFI_PEI_FILE_HANDLE                     *gMockPeiFileHandle;
EFI_PEI_FV_HANDLE                       *gMockVolumeHandle;
UINT8                                   *gSha384Digest;
UINTN                                   gIbbTestIndex;

/**
  Frees a buffer and set the pointer to NULL.

  @param[in, out]  Buffer  Pointer to the buffer.
**/
VOID
FreeSetNull (
  IN OUT VOID  **Buffer
  );

/**
  Generate random data.

  @param[in, out]  Buffer      The buffer to return data.
  @param[in]       BufferSize  The number of bytes in Buffer to fill with random data.
  @param[in]       IsWord      If random data with [0-9], [A-Z], [a-z] only.

  @retval TRUE   - Generate random data success.
  @retval FALSE  - Generate random data fail.
**/
BOOLEAN
RandomData (
  IN OUT UINT8    *Buffer,
  IN     UINTN    BufferSize,
  IN     BOOLEAN  IsWord
  );

/**
  Generate random IbbHashInfoPpi for unit test.

  @param[in, out]  Output  The buffer to return data.

  @retval TRUE   - Generate random data success.
  @retval FALSE  - Generate random data fail.
**/
BOOLEAN
GenerateRandomIbbHashInfoPpi (
  IN OUT PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI  **Output
  );

/**
  Free IbbHashInfoPpi.

  @param[in, out]  IbbHashInfoPpi  The buffer to be freed.
**/
VOID
FreeIbbHashInfoPpi (
  IN OUT PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI  **IbbHashInfoPpi
  );

/**
  Mock of LibPcdGetBool.

  @param[in]  TokenNumber  The PCD token number to retrieve a current value for.

  @return Return the value.
**/
BOOLEAN
EFIAPI
LibPcdGetBool (
  IN UINTN  TokenNumber
  );

#endif
