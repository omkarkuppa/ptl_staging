/** @file
  This file provides memory test function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#include <CMemoryTest.h>

GLOBAL_REMOVE_IF_UNREFERENCED const CHAR8 NormalModeMemTestStr[] = "Normal mode memory test ";

/**
  This function checks the memory range.

  @param[in]  MrcData           Pointer to the MRC global data structure
  @param[in]  BeginAddress      Beginning of the memory address to be checked.
  @param[in]  MemoryLength      Bytes of memory range to be checked.
  @param[in]  MemoryTestLevel   Type of memory check level to be performed.
  @param[out] ErrorAddress      Return the address of the error memory address.

  @retval     EFI_SUCCESS       The operation completed successfully.
  @retval     EFI_DEVICE_ERROR  Memory test failed. It's not reliable to use this range of memory.
**/
EFI_STATUS
EFIAPI
BaseMemoryTest (
  IN CONST MrcParameters         * CONST MrcData,
  IN       EFI_PHYSICAL_ADDRESS  BeginAddress,
  IN       UINT64                MemoryLength,
  IN       MEMORY_TEST_LEVEL     MemoryTestLevel,
  OUT      EFI_PHYSICAL_ADDRESS  *ErrorAddress
  )
{
  UINT32                                 TestPattern;
  UINT32                                 SpanSize;
  EFI_PHYSICAL_ADDRESS                   TempAddress;


  TestPattern = MEMORY_TEST_PATTERN;
  SpanSize = 0;

  //
  // Make sure we don't try and test anything above the max physical address range
  //
  ASSERT ((BeginAddress + MemoryLength) < MAX_ADDRESS);

  switch (MemoryTestLevel) {
    case Extensive:
      SpanSize = 0x4;
      break;
    case Sparse:
    case Quick:
      SpanSize = MEMORY_TEST_COVER_SPAN;
      break;
    case Ignore:
      goto Done;
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Make sure we don't try and test anything above the max physical address range\n"));
      ASSERT (FALSE);
      break;
  }

  DEBUG ((DEBUG_INFO, "Base memory test started.\n"));

  //
  // Write the test pattern into memory range
  //
  TempAddress = BeginAddress;
  while (TempAddress < BeginAddress + MemoryLength) {
    (*(UINT32*) (UINTN)TempAddress) = TestPattern;
    TempAddress += SpanSize;
  }

  //
  // Read pattern from memory and compare it
  //
  TempAddress = BeginAddress;
  while (TempAddress < BeginAddress + MemoryLength){
    if ((*(UINT32*) (UINTN)TempAddress) != TestPattern) {
      *ErrorAddress = TempAddress;
      DEBUG ((DEBUG_ERROR, "!!! Base memory test FAILED !!!\n"));
      return EFI_DEVICE_ERROR;
    }
    TempAddress += SpanSize;
  }

Done:
  return EFI_SUCCESS;
}

/**
  A small memory test to quickly point out severe memory issues.

  @param[in] MrcData - Pointer to the MRC global data structure

  @retval mrcFail on failure, otherwise mrcSuccess.
**/
MrcStatus
BasicMemoryTest (
  IN CONST MrcParameters * CONST MrcData
  )
{
  UINT32                BlockSize = 0x1000;
  volatile UINT8        * volatile Addr;
  UINT8                 Pattern;
  UINT8                 Value;
  UINTN                 LoopCount;
  const MrcInput        *Inputs;
  const MRC_EXT_INPUTS_TYPE   *ExtInputs;

  Inputs    = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;

  DEBUG ((DEBUG_INFO, "%astarted.\n", NormalModeMemTestStr));

  if ((ExtInputs->LowerBasicMemTestSize) && (Inputs->IsDdrIoUlxUlt)) {
    BlockSize = 0x80;
  }

  Addr    = 0;
  Pattern = 0;
  while ((UINTN) Addr < BlockSize) {
    *Addr = Pattern++;
    Addr++;
  }

  for (LoopCount = 0; LoopCount < 20; LoopCount++) {
    Addr    = 0;
    Pattern = 0;
    while ((UINTN) Addr < BlockSize) {
      Value = *Addr;
      if (Value != Pattern) {
        DEBUG ((DEBUG_ERROR,
          "!!! %aFAILED !!!\nAddress: %Xh, Expected data: %Xh, Actual data: %Xh.\n",
          NormalModeMemTestStr,
          Addr,
          Pattern,
          Value
          ));
        return mrcFail;
      }
      Addr++;
      Pattern++;
    }
  }

  DEBUG ((DEBUG_INFO, "%apassed.\n", NormalModeMemTestStr));
  return mrcSuccess;
}

/**
  A small memory test to quickly point out severe memory issues.
  This test restores each modified memory location,
  so we can run it on S3 resume as well.

  @param[in] MrcData - Pointer to the MRC global data structure

  @retval mrcFail on failure, otherwise mrcSuccess.
**/
MrcStatus
BasicMemoryTestS3 (
  IN CONST MrcParameters * CONST MrcData
  )
{
  UINT32                BlockSize = 0x1000;
  volatile UINT8        * volatile Addr;
  UINT8                 Pattern;
  UINT8                 Value;
  UINT8                 Backup;
  UINTN                 LoopCount;
  const MrcInput        *Inputs;
  const MRC_EXT_INPUTS_TYPE   *ExtInputs;

  Inputs    = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;

  DEBUG ((DEBUG_INFO, "%astarted.\n", NormalModeMemTestStr));

  if ((ExtInputs->LowerBasicMemTestSize) && (Inputs->IsDdrIoUlxUlt)) {
    BlockSize = 0x80;
  }

  Addr    = 0;
  Pattern = 0;
  while ((UINTN) Addr < BlockSize) {
    Backup = *Addr;
    *Addr = Pattern;
    for (LoopCount = 0; LoopCount < 10; LoopCount++) {
      Value = *Addr;
      if (Value != Pattern) {
        DEBUG ((DEBUG_ERROR,
          "!!! %aFAILED !!!\nAddress: %Xh, Expected data: %Xh, Actual data: %Xh.\n",
          NormalModeMemTestStr,
          Addr,
          Pattern,
          Value
          ));
        return mrcFail;
      }
    }
    Pattern++;
    *Addr = Backup;
    for (LoopCount = 0; LoopCount < 10; LoopCount++) {
      Value = *Addr;
      if (Value != Backup) {
        DEBUG ((DEBUG_ERROR,
          "!!! %aFAILED !!!\nAddress: %Xh, Expected data: %Xh, Actual data: %Xh.\n",
          NormalModeMemTestStr,
          Addr,
          Backup,
          Value
          ));
        return mrcFail;
      }
    }
    Addr++;
  }

  DEBUG ((DEBUG_INFO, "%apassed.\n", NormalModeMemTestStr));
  return mrcSuccess;
}

