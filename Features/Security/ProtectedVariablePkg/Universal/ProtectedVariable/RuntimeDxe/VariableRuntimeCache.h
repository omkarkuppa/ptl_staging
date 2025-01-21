/** @file
  The common variable volatile store routines shared by the DXE_RUNTIME variable
  module and the DXE_SMM variable module.

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

#ifndef _VARIABLE_RUNTIME_CACHE_H_
#define _VARIABLE_RUNTIME_CACHE_H_

#include "Variable.h"

/**
  Copies any pending updates to runtime variable caches.

  @retval EFI_UNSUPPORTED         The volatile store to be updated is not initialized properly.
  @retval EFI_SUCCESS             The volatile store was updated successfully.

**/
EFI_STATUS
FlushPendingRuntimeVariableCacheUpdates (
  VOID
  );

/**
  Synchronizes the runtime variable caches with all pending updates outside runtime.

  Ensures all conditions are met to maintain coherency for runtime cache updates. This function will attempt
  to write the given update (and any other pending updates) if the ReadLock is available. Otherwise, the
  update is added as a pending update for the given variable store and it will be flushed to the runtime cache
  at the next opportunity the ReadLock is available.

  @param[in] VariableRuntimeCache Variable runtime cache structure for the runtime cache being synchronized.
  @param[in] Offset               Offset in bytes to apply the update.
  @param[in] Length               Length of data in bytes of the update.

  @retval EFI_SUCCESS             The update was added as a pending update successfully. If the variable runtime
                                  cache ReadLock was available, the runtime cache was updated successfully.
  @retval EFI_UNSUPPORTED         The volatile store to be updated is not initialized properly.

**/
EFI_STATUS
SynchronizeRuntimeVariableCache (
  IN  VARIABLE_RUNTIME_CACHE  *VariableRuntimeCache,
  IN  UINTN                   Offset,
  IN  UINTN                   Length
  );

#endif
