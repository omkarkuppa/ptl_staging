/** @file
  Reset scheduling library services

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#ifndef _SI_SCHEDULE_RESET_LIB_H_
#define _SI_SCHEDULE_RESET_LIB_H_

#include <Uefi/UefiMultiPhase.h>
#include <PchResetPlatformSpecific.h>

/**
  This function updates the reset information in SiScheduleResetHob
  @param[in] ResetType        UEFI defined reset type.
  @param[in] ResetData        Optional element used to introduce a platform specific reset.
                               The exact type of the reset is defined by the EFI_GUID that follows
                               the Null-terminated Unicode string.
**/
VOID
SiScheduleResetSetType (
  IN EFI_RESET_TYPE     ResetType,
  IN PCH_RESET_DATA     *ResetData OPTIONAL
  );

/**
  This function returns TRUE or FALSE depending on whether a reset is required based on SiScheduleResetHob

  @retval     BOOLEAN       The function returns FALSE if no reset is required
**/
BOOLEAN
SiScheduleResetIsRequired (
  VOID
  );

/**
  This function performs reset based on SiScheduleResetHob

  @retval     BOOLEAN       The function returns FALSE if no reset is required
**/
BOOLEAN
SiScheduleResetPerformReset (
  VOID
  );

#endif //_SI_SCHEDULE_RESET_LIB_H_
