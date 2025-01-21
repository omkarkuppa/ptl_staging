/** @file
  Declares TbtNvmDrv common utilities

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

#ifndef TBT_NVM_DRV_UTILS_
#define TBT_NVM_DRV_UTILS_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>

#define BITFIELD_RANGE(startbit, endbit)  ((endbit) - (startbit) + 1)
#define BITFIELD_BIT(bit)                 1
#define BIT(x) (1UL << (x))
#define MASK(endbit, startbit)   (((1 << ((endbit) - (startbit) + 1)) - 1) << \
                                  (startbit))

#define TBT_STATUS_ERR(status) (status != TBT_STATUS_SUCCESS)
#define TBT_STATUS_FATAL_ERR(status) (status == TBT_STATUS_NON_RECOVERABLE_ERROR)

typedef enum {
  TBT_STATUS_SUCCESS,
  TBT_STATUS_NON_RECOVERABLE_ERROR,
  TBT_STATUS_TIMEOUT,
  TBT_STATUS_RETRY,
  TBT_STATUS_GENERIC_ERR,
  TBT_STATUS_INVALID_PARAM
} TBT_STATUS;

typedef enum {
  PORT_A,
  PORT_B
} TBT_PORT;

//
// USB4 LANE is designated as lane in Thunderbolt, per adapter has the specific lane.
// BIOS should configure the YFL_MASTER_LANE number to 1 or 3 corresponding to the board specific configuration.
// If YFL_MASTER_LANE number mismatches the board configuration, BIOS would cause capsule to perform the wrong path then the function might be abnormal.
//
typedef enum {
  FIRST_MASTER_LANE = 1,
  SECOND_MASTER_LANE = 3
} USB4_LANE;

typedef enum {
  TARGET_TBT_HOST,
  TARGET_RETIMER
} UPDATE_TARGET_TYPE;

/**
  Allocates the memory

  @param[in]    AllocationSize The size in bytes of the allocation request
  @retval       Pointer to the allocated memory, NULL in case of failure

**/
VOID*
TbtNvmDrvAllocateMem (
  IN UINTN  AllocationSize
  );

/**
  Deallocates previously allocated memory using TbtNvmDrvAllocateMem interface

  @param[in]    Mem            Pointer to the memory

**/
VOID
TbtNvmDrvDeAllocateMem (
  IN VOID*  Mem
  );


/**
  Swaps the endianess of the given data

  @param[in]  Src    Data to swap
  @param[in]  Length Length of the data to swap in DW
  @param[out] Dst    Swapped data

**/
VOID
TbtNvmDrvSwapEndianess (
  IN  UINT32 *Src,
  IN  UINT32 Length,
  OUT UINT32 *Dst
  );

#endif /* TBT_NVM_DRV_UTILS_ */
