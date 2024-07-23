/** @file
  PCH TCO Library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <Base.h>
#include <Library/IoLib.h>
#include <Library/TcoLib.h>
#include <Register/TcoRegs.h>


/**
  Check TCO second timeout status.

  @param[in] BaseAddress TCO Base Address.

  @retval  TRUE   TCO reboot happened.
  @retval  FALSE  TCO reboot didn't happen.
**/
BOOLEAN
TcoSecondToHappened (
  IN UINT16 BaseAddress
  )
{
  ///
  /// Read the Second TO status bit
  ///
  if ((IoRead8 (BaseAddress + R_TCO_IO_TSTS2) & B_TCO_IO_TSTS2_SECOND_TO_STS) != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  This function clears the Second TO status bit

  @param[in] BaseAddress TCO Base Address.

**/
VOID
TcoClearSecondToStatus (
  IN UINT16 BaseAddress
  )
{
  IoWrite8 (BaseAddress + R_TCO_IO_TSTS2, B_TCO_IO_TSTS2_SECOND_TO_STS);
}

/**
  Check TCO SMI ENABLE is locked

  @param[in] BaseAddress TCO Base Address.

  @retval TRUE  TCO SMI ENABLE is locked
          FALSE TCO SMI ENABLE is not locked
**/
BOOLEAN
TcoIsSmiLock (
  IN UINT16 BaseAddress
  )
{
  return !!(IoRead16 (BaseAddress + R_TCO_IO_TCTL1) & B_TCO_IO_TCTL1_TCO_LOCK);
}

/**
  Halts Tco timer

  @param[in] BaseAddress TCO Base Address.

**/
VOID
TcoHaltTimer (
  IN UINT16 BaseAddress
  )
{
  UINT16 Data16;

  Data16 = IoRead16 (BaseAddress + R_TCO_IO_TCTL1);
  Data16 |= B_TCO_IO_TCTL1_TCO_TMR_HALT;
  IoWrite16 (BaseAddress + R_TCO_IO_TCTL1, Data16);
}
