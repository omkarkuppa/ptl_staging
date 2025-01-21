/** @file
  PCH SMM private lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/IoLib.h>
#include <Register/PttPtpRegs.h>

STATIC UINT32 mBiosGuardEnabled = ~0u;

/**
  Set InSmm.Sts bit
**/
VOID
PchSetInSmmSts (
  VOID
  )
{
  UINT32      Data32;

  //
  // When BIOS GUARD is enabled, the MSR 1FEh is invalid.
  //
  if (mBiosGuardEnabled != 0) {
    if (mBiosGuardEnabled == 1) {
      return;
    }
    mBiosGuardEnabled = IsBiosGuardEnabled () ? 1 : 0;
    if (mBiosGuardEnabled == 1) {
      return;
    }
  }

  ///
  /// If platform disables TXT_PTLEN strap, NL socket(s) will target abort
  /// when trying to access LT register space below, and writes to
  /// NL's MSR 0x1FE will GP fault. Check straps enabled first.
  ///

  Data32 = MmioRead32 (R_LT_EXISTS);

  if (Data32 == 0xFFFFFFFF) {
    return;
  }
  ///
  /// Read memory location FED30880h OR with 00000001h, place the result in EAX,
  /// and write data to lower 32 bits of MSR 1FEh (sample code available)
  ///
  Data32 = MmioRead32 (R_LT_UCS);
  WriteSpclChipsetUsageMsr (Data32 | BIT0);
  ///
  /// Read FED30880h back to ensure the setting went through.
  ///
  Data32 = MmioRead32 (R_LT_UCS);
}

/**
  Clear InSmm.Sts bit
**/
VOID
PchClearInSmmSts (
  VOID
  )
{
  UINT32      Data32;

  //
  // When BIOS GUARD is enabled, the MSR 1FEh is invalid.
  //
  if (mBiosGuardEnabled != 0) {
    if (mBiosGuardEnabled == 1) {
      return;
    }
    mBiosGuardEnabled = IsBiosGuardEnabled () ? 1 : 0;
    if (mBiosGuardEnabled == 1) {
      return;
    }
  }

  ///
  /// If platform disables TXT_PTLEN strap, NL socket(s) will target abort
  /// when trying to access LT register space below, and writes to
  /// NL's MSR 0x1FE will GP fault. Check straps enabled first.
  ///

  Data32 = MmioRead32 (R_LT_EXISTS);
  if (Data32 == 0xFFFFFFFF) {
    return;
  }

  ///
  /// Read memory location FED30880h AND with FFFFFFFEh, place the result in EAX,
  /// and write data to lower 32 bits of MSR 1FEh (sample code available)
  ///
  Data32 = MmioRead32 (R_LT_UCS);
  WriteSpclChipsetUsageMsr (Data32 & (UINT32) (~BIT0));
  ///
  /// Read FED30880h back to ensure the setting went through.
  ///
  Data32 = MmioRead32 (R_LT_UCS);
}
