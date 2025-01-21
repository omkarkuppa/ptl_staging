/** @file
  Initializes IEH Device in PEI

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

#include <Library/PeiIehInitLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/IehRegs.h>

/**
  Initialize IEH device.

  @param[in] IehHandle    IEH Handle instance
**/
VOID
IehInit (
  IN  IEH_HANDLE      *IehHandle
  )
{
  //
  // Program IEH bus number
  //
  PciSegmentAndThenOr32 (
    IehHandle->PciCfgBase + R_IEH_CFG_IEHTYPEVER,
    (UINT32)~(B_IEH_CFG_IEHTYPEVER_IEH_BUSNUM),
    (UINT32) (IehHandle->PrivateConfig->BusNum << N_IEH_CFG_IEHTYPEVER_IEH_BUSNUM)
    );

  if (IehHandle->Config->Mode == IEH_MODE_ENABLE) {
    // IEH Mode is set to Enable
    PciSegmentAndThenOr32 (
      IehHandle->PciCfgBase + R_IEH_CFG_MISCCTL1,
      (UINT32) ~(B_IEH_CFG_MISCCTL1_IFD),
      B_IEH_CFG_MISCCTL1_IE);
  } else {
    // IEH Mode is set to Bypass
    PciSegmentAndThenOr32 (
      IehHandle->PciCfgBase + R_IEH_CFG_MISCCTL1,
      (UINT32) ~(B_IEH_CFG_MISCCTL1_IE),
      B_IEH_CFG_MISCCTL1_IFD);
  }
}


