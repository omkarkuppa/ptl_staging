/** @file
  This file contains functions to access the register cacheing information.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#ifndef _MrcRegisterCache_h_
#define _MrcRegisterCache_h_
#include "CMrcInterface.h"  // for MrcParameters

#pragma pack (push, 1)

/**
  Invalidate the specified offset in the register cache.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  Offset  - MCHBAR register offset.

  @retval mrcSuccess if found in the cache, otherwise mrcFail.
**/
extern
MrcStatus
InvalidateRegisterOffsetInCache (
  MrcParameters *const MrcData,
  UINT32  Offset
  );

/**
  Initialize the register cache.

  @param[in] MrcData - Pointer to MRC global data.

  @retval mrcSuccess.
**/
extern
MrcStatus
InitializeRegisterCache (
  MrcParameters *const MrcData
  );

/**
  Invalidate the register cache data.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval mrcSuccess.
**/
MrcStatus
MrcInvalidateRegisterCachedData (
  MrcParameters *const MrcData
  );

/**
  Flush the register data cache entries marked pending to hardware.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval mrcSuccess.
**/
extern
MrcStatus
MrcFlushRegisterCachedData (
  MrcParameters *const MrcData
  );

#if 0 // This code is used for register cache debug only.
#ifdef MRC_DEBUG_PRINT
/**
  Print the entire register data cache.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval mrcSuccess.
**/
extern
MrcStatus
PrintRegisterCachedData (
  MrcParameters *const MrcData
  );
#endif
#endif

/**
  This function sets the current register value and updates corresponding cache location if update cache is enabled.

  @param[in] MrcData    - Pointer to global data.
  @param[in] Offset     - Register offset.
  @param[in] Is64BitReg - TRUE if Offset points to a 64-bit register, FALSE if Offset points to a 32-bit register.
  @param[in] Mode       - Bit-field flags controlling cache access.
  @param[in] Delay      - Delay in microseconds to invoke after hardware register write.
  @param[in] Value      - New register value.

  @retval mrcSuccess or mrcWrongInputParameter if cache update is enabled and there is no cache entry.
**/
extern
MrcStatus
MrcCrCacheWrite (
  IN MrcParameters *const MrcData,
  IN const UINT32  Offset,
  IN const BOOLEAN Is64BitReg,
  IN const UINT32  Mode,
  IN const UINT32  Delay,
  IN const UINT64  Value
  );

/**
  This function gets the current register value, either from the cache or from hardware if either cache reads are disabled or doesn't exist.

  @param[in] MrcData    - Pointer to global data.
  @param[in] Offset     - Register offset.
  @param[in] Is64BitReg - TRUE if Offset points to a 64-bit register, FALSE if Offset points to a 32-bit register.
  @param[in] Mode       - Bit-field flags controlling cache access.

  @retval The current register value, either from the cache or from hardware if either cache reads are disabled or doesn't exist.
**/
extern
UINT64
MrcCrCacheRead (
  IN MrcParameters *const MrcData,
  IN const UINT32  Offset,
  IN const BOOLEAN Is64BitReg,
  IN const UINT32  Mode
  );
#pragma pack (pop)
#endif

