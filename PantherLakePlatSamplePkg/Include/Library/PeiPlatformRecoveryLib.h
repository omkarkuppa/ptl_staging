/** @file
  This library provides platform recovery module.


  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#ifndef _PEI_PLATFORM_RECOVERY_LIB_H_
#define _PEI_PLATFORM_RECOVERY_LIB_H_


/**
  Provide the functionality of the recovery module.

  @retval     EFI_SUCCESS  The function completed successfully.
**/
EFI_STATUS
EFIAPI
InitializeRecovery (
  VOID
  );
#endif
