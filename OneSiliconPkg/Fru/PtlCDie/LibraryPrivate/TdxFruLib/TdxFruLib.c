/** @file
  TDX Fru library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Library/TdxFruLib.h>

/**
  Calculate the SEAMRR size

  @param[in] MrcOutput  - MRC Output Parameter Structure

  @retval UINT32        - returns calculated Seamrr Size in MB
**/
UINT32
EFIAPI
CalculateSeamrrSize (
  IN MrcOutput *MrcOutputData
  )
{
  MrcMemoryMap *MemoryMap = NULL;
  UINT64       ActrrSize;
  UINT32       SeamrrSize = 0;
  UINT64       ActTableSize;
  UINT32       ControllerCount = 0;
  UINT8        ControllerId;

  DEBUG ((DEBUG_INFO, "%a \n", __FUNCTION__));

  if (MrcOutputData == NULL) {
    DEBUG ((DEBUG_ERROR, "MrcOutputData is null. exiting!\n"));
    return SeamrrSize;
  }

  MemoryMap = &MrcOutputData->MemoryMapData;

  if (MemoryMap == NULL) {
    DEBUG ((DEBUG_ERROR, "MemoryMap data is null. exiting!\n"));
    return SeamrrSize;
  }

  for (ControllerId = 0; ControllerId < MAX_CONTROLLER; ControllerId++) {
    if (MrcOutputData->Controller[ControllerId].Status == CONTROLLER_PRESENT) {
      ControllerCount++;
    }
  }

  ActTableSize = (ControllerCount * CONVERT_MB_TO_B (MemoryMap->TouudBase))/SIZE_32KB;
  ActrrSize    = GetPowerOfTwo64 (((ActTableSize * 8) + 6) / 7);

  DEBUG ((DEBUG_ERROR, "[TDX] Actrr Size = %d MB \n", CONVERT_B_TO_MB (ActrrSize)));

  if (ActrrSize <= SIZE_16MB) {
      SeamrrSize = CONVERT_B_TO_MB(SIZE_32MB);
      DEBUG ((DEBUG_ERROR, "[TDX] Seamrr Size = %d MB\n", SeamrrSize));
  } else {
    DEBUG ((DEBUG_ERROR, "[TDX] ERROR: Invalid Actrr Size = %d MB \n", CONVERT_B_TO_MB (ActrrSize)));
    DEBUG ((DEBUG_ERROR, "[TDX] Seamrr Size = %d\n", SeamrrSize));
    return SeamrrSize;
  }

  DEBUG ((DEBUG_INFO, "%a END\n", __FUNCTION__));
  return SeamrrSize;
}

/**
  Determine whether the Cache level allows TDX enablement or not.

  @retval EFI_SUCCESS     - Sufficient Cache is Available for enabling TDX
  @retval EFI_UNSUPPORTED - Cache level does not support TDX enablement
**/
EFI_STATUS
EFIAPI
IsSufficientCacheAvailable (
  VOID
  )
{
  return EFI_SUCCESS;
}
