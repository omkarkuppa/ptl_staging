/** @file
  Private RTC Library.

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
#include <Register/RtcRegs.h>
#include <RegisterAccess.h>

/**
  Check RTC BIOS interface is locked.

  @param[in]  RtcPcrAccess  Instance of LPC sideband Register Access Protocol.

  @retval  TRUE   RTC BIOS interface is locked.
  @retval  FALSE  RTC BIOS interface is not locked.

**/
BOOLEAN
RtcIsBiosInterfaceLock (
  IN REGISTER_ACCESS  *RtcPcrAccess
  )
{
  return !!(RtcPcrAccess->Read32 (RtcPcrAccess, R_RTC_PCR_RC) & B_RTC_PCR_RC_BILD);
}

/**
  Get Top Swap status.

  @retval  TRUE   Top Swap is set.
  @retval  FALSE  Top Swap is clear.

  @param[in]  RtcPcrAccess  Instance of LPC sideband Register Access Protocol.

  @note  It is caller's responsibility to ensure the accessibility of Backed Up Control register and TS bit.
         This register might be locked or restricted access in certain phases. (e.g. POSTBOOT_SAI)

**/
BOOLEAN
TopSwapStatus (
  IN REGISTER_ACCESS  *RtcPcrAccess
  )
{
  return (RtcPcrAccess->Read32 (RtcPcrAccess, R_RTC_PCR_BUC) & B_RTC_PCR_BUC_TS) != 0;
}

/**
  Set Top Swap status.

  @param[in]  TopSwapEnable  Enable Top Swap or disable it.
  @param[in]  RtcPcrAccess   Instance of LPC sideband Register Access Protocol.

  @retval  EFI_SUCCESS  Succeed to set Top Swap.
  @retval  Others       Failed to set Top Swap.

  @note  It is caller's responsibility to ensure the accessibility of Backed Up Control register and TS bit.
         This register might be locked or restricted access in certain phases. (e.g. POSTBOOT_SAI)

**/
EFI_STATUS
TopSwapSet (
  IN BOOLEAN          TopSwapEnable,
  IN REGISTER_ACCESS  *RtcPcrAccess
  )
{
  if (TopSwapEnable) {
    RtcPcrAccess->AndThenOr32 (RtcPcrAccess, R_RTC_PCR_BUC, ~0u, B_RTC_PCR_BUC_TS);
  } else {
    RtcPcrAccess->AndThenOr32 (RtcPcrAccess, R_RTC_PCR_BUC, (UINT32)~B_RTC_PCR_BUC_TS, 0);
  }

  return EFI_SUCCESS;
}
