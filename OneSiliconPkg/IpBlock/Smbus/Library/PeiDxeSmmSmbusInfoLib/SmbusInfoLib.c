/** @file
  Smbus Info Library

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
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/IoLib.h>
#include <Register/SmbusRegs.h>
#include <Register/PchRegs.h>
#include <Library/PchPciBdfLib.h>

/**
Gets Io port base address of Smbus Host Controller.

@retval The Io port base address of Smbus host controller.

**/
UINT16
GetSmbusIoPortBaseAddress (
  VOID
 )
{
  UINT64    SmbusPciBase;
  UINT16    IoPortBaseAddress;

  SmbusPciBase = SmbusPciCfgBase ();
  IoPortBaseAddress = (UINT16)PciSegmentRead32 (SmbusPciBase + R_SMBUS_CFG_SBA);

  //
  // Make sure that the IO port base address has been properly set.
  //
  if ((IoPortBaseAddress == 0) || (IoPortBaseAddress == 0xFFFF)) {
    return 0;
  }

  IoPortBaseAddress &= B_SMBUS_CFG_SBA_BA;

  return IoPortBaseAddress;
}

/**
  This function checks if SMBUS Host Lock is set

  @retval SMBUS host Lock state
**/
BOOLEAN
SmbusIsHostLocked (
  VOID
  )
{
  UINT32 Hostc;
  UINT64 SmbusPciBase;
  SmbusPciBase = SmbusPciCfgBase ();
  Hostc = PciSegmentRead32 (SmbusPciBase + R_SMBUS_CFG_HCFG);
  return !!(Hostc & B_SMBUS_CFG_HCFG_SPDWD);
}

/**
  This function clears the Smbus Status Register
**/
VOID
SmbusClearStatusReg (
  VOID
  )
{
  UINT16 SmbusIoBase;
  UINT64 SmbusPciBase;
  SmbusPciBase = SmbusPciCfgBase ();
  SmbusIoBase = PciSegmentRead16 (SmbusPciBase + R_SMBUS_CFG_SBA) & B_SMBUS_CFG_SBA_BA;
  IoWrite8 (SmbusIoBase + R_SMBUS_IO_HSTS, V_SMBUS_IO_HSTS_ALL);
}
