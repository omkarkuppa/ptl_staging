/** @file
  This header file provides memory test function.

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

#ifndef _Memory_TEST_H_
#define _Memory_TEST_H_

#include <Base.h>
#include <MrcDebugPrint.h>
#include <Library/DebugLib.h>

///
/// Used in memory test routines
///
#define  MEMORY_TEST_COVER_SPAN         0x40000
#define  MEMORY_TEST_PATTERN            0x5A5A5A5A

typedef enum {
  Ignore,
  Quick,
  Sparse,
  Extensive,
  MemTestMax
} MEMORY_TEST_LEVEL;

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
  );

/**
  A small memory test to quickly point out severe memory issues.

  @param[in]  MrcData Pointer to the MRC global data structure

  @retval mrcFail on failure, otherwise mrcSuccess.
**/
MrcStatus
BasicMemoryTest (
  IN CONST MrcParameters * CONST MrcData
  );

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
  );

#endif
