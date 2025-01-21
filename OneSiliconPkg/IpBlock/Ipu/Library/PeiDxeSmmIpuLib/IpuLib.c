/** @file
  This file provides IPU Public Library implementation

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

#include <Library/DebugLib.h>
#include <Defines/IpuDefines.h>
#include <Library/PeiHostBridgeIpStatusLib.h>

/**
  GetIpuBusNumber: Get IPU Bus Number

  @retval PCI bus number for IPU
**/
UINT64
GetIpuBusNumber (
  VOID
  )
{
  return (UINT64)IPU_BUS_NUM;
}

/**
  GetIpuDevNumber: Get IPU Dev Number

  @retval PCI dev number for IPU
**/
UINT64
GetIpuDevNumber (
  VOID
  )
{
  return (UINT64)IPU_DEV_NUM;
}

/**
  GetIpuFunNumber: Get IPU Fun Number

  @retval PCI fun number for IPU
**/
UINT64
GetIpuFuncNumber (
  VOID
  )
{
  return (UINT64)IPU_FUN_NUM;
}

/**
  IsIpuSupported: Check if IPU is supported or not by Silicon.

  @retval FALSE = IPU is fused off. TRUE = IPU is supported by Sillicon.
**/
BOOLEAN
IsIpuSupported (
  VOID
  )
{
  BOOLEAN  IpuEnable;

  ///
  /// Check if Silicon is IPU capable
  ///
  IpuEnable = UncoreCapIdRead (Ipu, 0);
  if (IpuEnable == FALSE) {
    DEBUG ((DEBUG_INFO, "IPU Fused off\n"));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "IPU is supported by Silicon\n"));
  return TRUE;
}