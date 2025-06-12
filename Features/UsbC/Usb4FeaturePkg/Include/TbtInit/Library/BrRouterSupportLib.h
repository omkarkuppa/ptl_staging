/** @file
   BR router support Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
#ifndef _BR_ROUTER_SUPPORT_LIB_H_
#define _BR_ROUTER_SUPPORT_LIB_H_

#include <Library/DxeTbtDisBmeLib.h>

//
// Thunderbolt(TM) Vendor ID
//
#define PCI_VID_INTEL  0x8086

//
// Barlow Ridge device router device IDs
//
#define PCI_DID_BR_HUB_80  0x5786  // Support 80G bps link
#define PCI_DID_BR_HUB_40  0x57A4  // Support 40G bps link

/**
  Recursive BR Hierarchy Configuration

  @param[in]  Sbdf       device's segment:bus:device:function coordinates
**/
VOID
FindBrHierarchyConfiguration  (
  IN SBDF       Sbdf
  );

/**
  EnableBrBmeCallBack

  Enable BME on BR Host and Device Router tree at EfiPciHostBridgeSetResources
  to allow MMIO propagation for loading OPROM from MMIO.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
EnableBrBmeCallBack (
  IN OUT VOID
  );

#endif
