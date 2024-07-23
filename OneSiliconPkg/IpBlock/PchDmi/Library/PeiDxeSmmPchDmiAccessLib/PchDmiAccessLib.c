/** @file
  PCH DMI Access Lib.

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
#include <Library/IoLib.h>
#include <Library/PchPcrLib.h>
#include <Register/PchDmiRegs.h>
#include <Register/PchPcrRegsDeprecated.h>

/**
  Check DMI BIOS interface is locked

  @retval TRUE  DMI BIOS interface is locked
          FALSE DMI BIOS interface is not locked
**/
BOOLEAN
PchDmiIsBiosInterfaceLock (
  VOID
  )
{
  return !!(PchPcrRead32 (PID_DMI_DEPRECATED, R_PCH_DMI_PCR_GCS) & B_PCH_DMI_PCR_BILD);
}

/**
  Gets the extended BIOS region base and limit addresses

  @param[out] UINT64  Extended BIOS Region Range Base address
  @param[out] UINT64  Extended BIOS Region Range Limit address
**/
VOID
PchDmiGetExtendedBiosRegionRange (
  OUT  UINT64   *RangeBase,
  OUT  UINT64   *RangeLimit
  )
{
  UINT32 Data32;

  Data32 = PchPcrRead32 (PID_DMI_DEPRECATED, R_PCH_DMI_PCR_GPMR1);

  *RangeBase  = (Data32 & 0xFFFF) << 16;
  *RangeLimit = Data32 & 0xFFFF0000;
}

/**
  Checks lock of extended BIOS Region Limit address

  @retval TRUE    Extended BIOS Region limit address is locked
  @retval FALSE   Extended BIOS Region limit address is unlocked
**/
BOOLEAN
PchDmiIsExtendedBiosRegionLimitLocked (
  VOID
  )
{
  return !!(PchPcrRead32 (PID_DMI_DEPRECATED, R_PCH_DMI_PCR_GPMR1DID) & B_PCH_DMI_PCR_GPMRXDID_GPMR1DE);
}
