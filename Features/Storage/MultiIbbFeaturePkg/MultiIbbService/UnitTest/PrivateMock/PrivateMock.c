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

#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseCryptLib.h>

/**
  Frees a buffer and set the pointer to NULL.

  @param[in, out]  Buffer  Pointer to the buffer.
**/
VOID
FreeSetNull (
  IN OUT VOID  **Buffer
  )
{
  if (*Buffer != NULL) {
    FreePool (*Buffer);
    *Buffer = NULL;
  }
}

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
  )
{
  UINTN  Index;
  UINT8  Temp;
  UINTN  Length;

  if (Buffer == NULL || BufferSize == 0) {
    return FALSE;
  }

  if (!RandomSeed (NULL, 0)) {
    return FALSE;
  }

  ZeroMem (Buffer, BufferSize);
  Length = IsWord ? BufferSize - 1 : BufferSize;

  for (Index = 0; Index < Length; Index++) {
    while (TRUE) {
      if (!RandomBytes (&Temp, sizeof (UINT8))) {
        return FALSE;
      }
      if (!IsWord) {
        break;
      }
      if ((Temp >= '0' && Temp <= '9') || (Temp >= 'A' && Temp <= 'Z') || (Temp >= 'a' && Temp <= 'z')) {
        break;
      }
      if (Temp == 0) {
        return TRUE;
      }
    }
    SetMem (Buffer + Index, sizeof (UINT8), Temp);
  }

  return TRUE;
}

/**
  Generate random IbbHashInfoPpi for unit test.

  @param[in, out]  Output  The buffer to return data.

  @retval TRUE   - Generate random data success.
  @retval FALSE  - Generate random data fail.
**/
BOOLEAN
GenerateRandomIbbHashInfoPpi (
  IN OUT PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI  **Output
  )
{
  PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI  *IbbHashInfoPpi;
  UINTN                                   Index;
  VOID                                    *Address;
  UINTN                                   Length;

  IbbHashInfoPpi = AllocateZeroPool (sizeof (PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI));
  if (IbbHashInfoPpi == NULL || !RandomData ((UINT8*)IbbHashInfoPpi, sizeof (PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI), FALSE)) {
    return FALSE;
  }

  for (Index = 0; Index < MULTI_IBB_INFO_COUNT; Index++) {
    Length = 0x10000;
    Address = AllocateZeroPool (Length);
    if (Address == NULL) {
      return FALSE;
    }
    SetMem (Address, Length, (UINT8)Index + 1);
    IbbHashInfoPpi->MultiIbbInfo[Index].IbbCacheAgent = IBB_CACHE_AGENT_BIOS;
    IbbHashInfoPpi->MultiIbbInfo[Index].Base = (UINTN) Address;
    IbbHashInfoPpi->MultiIbbInfo[Index].Length = Length;
    IbbHashInfoPpi->MultiIbbInfo[Index].CurrentlyCached = TRUE;
  }

  *Output = IbbHashInfoPpi;
  return TRUE;
}

/**
  Free IbbHashInfoPpi.

  @param[in, out]  IbbHashInfoPpi  The buffer to be freed.
**/
VOID
FreeIbbHashInfoPpi (
  IN OUT PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI  **IbbHashInfoPpi
  )
{
  UINTN  Index;

  if (*IbbHashInfoPpi != NULL) {
    for (Index = 0; Index < MULTI_IBB_INFO_COUNT; Index++) {
      FreeSetNull (&(VOID*)(*IbbHashInfoPpi)->MultiIbbInfo[Index].Base);
    }
    FreeSetNull (IbbHashInfoPpi);
  }
}

