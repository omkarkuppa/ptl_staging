/** @file
  PEIM to initialize IPU device

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#include <Library/IpuInitLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>

/**
  IpuInit: Initialize the IPU device

  @param[in] IPU_PREMEM_CONFIG   IpuPreMemPolicy
**/
VOID
IpuInit (
   VOID
  )
{
  BOOLEAN  IpuDevenStatus;
  //
  // Clear D5EN bit at DEVEN register when PcdIpuEnable is set to FALSE.
  //
  IpuDevenStatus = UncoreDevEnRead (Ipu, 0);
  if (IpuDevenStatus == TRUE) {
    DEBUG ((DEBUG_INFO, "Clear IPU bit at DEVEN regiser in Null library\n"));
    UncoreDevEnWrite (Ipu, 0, Disable);
  }
  return;
}

/**
  IsIpuEnabled: Check the IPU is enabled or not

  @retval FALSE = IPU Disabled by policy or fuse, TRUE = IPU Enabled.
**/
BOOLEAN
IsIpuEnabled (
  VOID
  )
{
  return FALSE;
}

/**
  Check IPU policies are valid for debugging unexpected problem if these
  values are not initialized or assigned incorrect resource.

  @param[in] IpuPreMemPolicy    The IPU PreMem Policy instance

**/
VOID
IpuValidatePolicy (
  IN    IPU_PREMEM_CONFIG    *IpuPreMemPolicy
  )
{
  return;
}
