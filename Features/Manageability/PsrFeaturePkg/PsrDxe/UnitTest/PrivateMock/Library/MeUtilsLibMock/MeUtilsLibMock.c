/** @file
  Mock instance of MeUtilsLib library class.

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

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <Uefi.h>
#include <Library/DebugLib.h>

/**
  Mock WaitFirmwareInitComplete: Wait until Firmware Init Complete is set in Firmware Status Register.

  @retval EFI_SUCCESS             Firmware Init Complete bit is set
  @retval EFI_TIMEOUT             Firmware Init Complete bit is not set within specified time
**/
EFI_STATUS
WaitFirmwareInitComplete (
  VOID
  )
{
  return mock_type (EFI_STATUS);
}
