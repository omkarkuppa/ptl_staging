/** @file
  IOC Access Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/IocRegAccessLib.h>
#include <Register/HostBridgeRegs.h>


/**
  Check IOC BIOS interface is locked

  @retval TRUE  IOC BIOS interface is locked
          FALSE IOC BIOS interface is not locked
**/
BOOLEAN
IocIsBiosInterfaceLock (
  VOID
  )
{

  GCS_IOC_MCHBAR_STRUCT  GcsStruct;

  GcsStruct.Data = IocRegAccessRead32 ((UINTN) GCS_IOC_MCHBAR_REG);
  return (BOOLEAN) GcsStruct.Bits.bild;
}

/**
  Gets the extended BIOS region base and limit addresses

  @param[out] UINT64  Extended BIOS Region Range Base address
  @param[out] UINT64  Extended BIOS Region Range Limit address
**/
VOID
IocGetExtendedBiosRegionRange (
  OUT  UINT64   *RangeBase,
  OUT  UINT64   *RangeLimit
  )
{
  UINT32 Data32;

  Data32 = IocRegAccessRead32 ((UINTN) GPMR1_IOC_MCHBAR_REG);

  *RangeBase  = (Data32 & 0xFFFF) << 16;
  *RangeLimit = Data32 & 0xFFFF0000;
}

/**
  Checks lock of extended BIOS Region Limit address

  @retval TRUE    Extended BIOS Region limit address is locked
  @retval FALSE   Extended BIOS Region limit address is unlocked
**/
BOOLEAN
IocIsExtendedBiosRegionLimitLocked (
  VOID
  )
{
  GPMR1DID_IOC_MCHBAR_STRUCT Gpmr1did;

  Gpmr1did.Data = IocRegAccessRead32 ((UINTN) GPMR1DID_IOC_MCHBAR_REG);
  return (BOOLEAN) Gpmr1did.Bits.gpmrde;
}
