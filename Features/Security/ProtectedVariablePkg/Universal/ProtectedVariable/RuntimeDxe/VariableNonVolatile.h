/** @file
  Common variable non-volatile store routines.

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

#ifndef _VARIABLE_NON_VOLATILE_H_
#define _VARIABLE_NON_VOLATILE_H_

#include "Variable.h"

/**
  Get non-volatile maximum variable size.

  @return Non-volatile maximum variable size.

**/
UINTN
GetNonVolatileMaxVariableSize (
  VOID
  );

/**
  Init emulated non-volatile variable store.

  @param[out] VariableStoreBase Output pointer to emulated non-volatile variable store base.

  @retval EFI_SUCCESS           Function successfully executed.
  @retval EFI_OUT_OF_RESOURCES  Fail to allocate enough memory resource.

**/
EFI_STATUS
InitEmuNonVolatileVariableStore (
  EFI_PHYSICAL_ADDRESS  *VariableStoreBase
  );

/**
  Init real non-volatile variable store.

  @param[out] VariableStoreBase Output pointer to real non-volatile variable store base.

  @retval EFI_SUCCESS           Function successfully executed.
  @retval EFI_OUT_OF_RESOURCES  Fail to allocate enough memory resource.
  @retval EFI_VOLUME_CORRUPTED  Variable Store or Firmware Volume for Variable Store is corrupted.

**/
EFI_STATUS
InitRealNonVolatileVariableStore (
  OUT EFI_PHYSICAL_ADDRESS  *VariableStoreBase
  );

/**
  Init non-volatile variable store.

  @retval EFI_SUCCESS           Function successfully executed.
  @retval EFI_OUT_OF_RESOURCES  Fail to allocate enough memory resource.
  @retval EFI_VOLUME_CORRUPTED  Variable Store or Firmware Volume for Variable Store is corrupted.

**/
EFI_STATUS
InitNonVolatileVariableStore (
  VOID
  );

#endif
