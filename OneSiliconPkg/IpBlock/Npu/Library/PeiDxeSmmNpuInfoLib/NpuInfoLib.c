/** @file
  This file provides NPU Public Library implementation

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include <Uefi.h>
#include <IndustryStandard/Pci22.h>
#include <Library/NpuInfoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/PeiHostBridgeIpStatusLib.h>

//
// Device 11 Equates
//
#define NPU_BUS_NUM    0x00
#define NPU_DEV_NUM    0x0B
#define NPU_FUN_NUM    0x00

/**
  GetNpuBusNumber: Get NPU Bus Number

  @retval PCI bus number for NPU
**/
UINT8
GetNpuBusNumber (
  VOID
  )
{
  return (UINT8)NPU_BUS_NUM;
}

/**
  GetNpuDevNumber: Get NPU Dev Number

  @retval PCI dev number for NPU
**/
UINT8
GetNpuDevNumber (
  VOID
  )
{
  return (UINT8)NPU_DEV_NUM;
}

/**
  GetNpuFunNumber: Get NPU Fun Number

  @retval PCI fun number for NPU
**/
UINT8
GetNpuFuncNumber (
  VOID
  )
{
  return (UINT8)NPU_FUN_NUM;
}

/**
  IsNpuSupported: Check if NPU is supported or not by Silicon.

  @retval FALSE = NPU is fused off. TRUE = NPU is supported by Sillicon.
**/
BOOLEAN
IsNpuSupported (
  VOID
  )
{
  BOOLEAN  NpuEnable;

  ///
  /// Check if Silicon is NPU capable
  ///
  NpuEnable = UncoreCapIdRead (Npu, 0);
  if (NpuEnable == FALSE) {
    DEBUG ((DEBUG_INFO, "NPU is Fused off\n"));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "NPU is supported by Silicon\n"));
  return TRUE;
}

/**
  IsNpuEnabled: Check if NPU is enabled or not.

  @retval TRUE = NPU is enabled.
  @retval FALSE = NPU is disabled.
**/
BOOLEAN
IsNpuEnabled (
  VOID
  )
{
  UINT64                  NpuBaseAddress;

  NpuBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, NPU_BUS_NUM, NPU_DEV_NUM, NPU_FUN_NUM, 0);

  if (PciSegmentRead16 (NpuBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_WARN, "NPU is unsupported or disabled!\n"));
    return FALSE;
  }

  return TRUE;
}
