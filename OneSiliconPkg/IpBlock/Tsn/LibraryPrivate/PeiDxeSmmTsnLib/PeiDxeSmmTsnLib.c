/** @file
  This file provides Tsn Public Library implementation

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <TsnHandle.h>
#include <Register/TsnRegs.h>

/**
  Check whether TSN controller is enabled in the platform.

  @retval TRUE    TSN GbE Controller is present.
  @retval FALSE   TSN GbE Controller is not present.
**/
BOOLEAN
IsTsnDevicePresent (
  IN  TSN_HANDLE    *TsnHandle
  )
{
#if FixedPcdGet8(PcdTsnSupport) == 0x1
  REGISTER_ACCESS *TsnPcrAccess;

  TsnPcrAccess = TsnHandle->Controller->TsnPcrAccess;

  // Check if TSN is available
  if (TsnPcrAccess->Read32 (TsnPcrAccess, R_TSN_PCR_PCICFGCTRL) != 0xFFFFFFFF) {
    DEBUG((DEBUG_INFO, "[TSN] Controller Present!\n"));
    return TRUE;
  }
#endif
  DEBUG((DEBUG_INFO, "[TSN] Controller not Present!\n"));
  return FALSE;
}