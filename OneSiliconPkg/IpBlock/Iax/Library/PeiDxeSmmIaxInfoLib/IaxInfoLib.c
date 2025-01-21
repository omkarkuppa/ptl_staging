/** @file
  This file provides IAX Public Library implementation

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include <Library/IaxInfoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Defines/IaxDefines.h>


/**
  IsIaxSupported: Check if IAX is supported or not by Silicon.

  @retval FALSE = IAX is fused off.
          TRUE = IAX is supported by Sillicon.
**/
BOOLEAN
EFIAPI
IsIaxSupported (
  VOID
  )
{
  BOOLEAN  IaxEnable;
  ///
  /// Check if Silicon is IAX capable
  ///
  IaxEnable = UncoreCapIdRead (Iax, 0);
  if (IaxEnable == FALSE) {
    DEBUG ((DEBUG_INFO, "IAX is Fused off\n"));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "IAX is supported by Silicon\n"));
  return TRUE;
}

/**
  IsIaxEnabled: Check if IAX is enabled or not.

  @retval TRUE = IAX is enabled.
  @retval FALSE = IAX is disabled.
**/
BOOLEAN
EFIAPI
IsIaxEnabled (
  VOID
  )
{
  UINT64  IaxBaseAddress;

  IaxBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, IAX_BUS_NUM, IAX_DEV_NUM, IAX_FUN_NUM, 0);

  if (!IsIaxSupported () || (PciSegmentRead16 (IaxBaseAddress + PCI_VENDOR_ID_OFFSET)) == 0xFFFF) {
    DEBUG ((DEBUG_WARN, "IAX is unsupported or disabled!\n"));
    return FALSE;
  }

  return TRUE;
}
