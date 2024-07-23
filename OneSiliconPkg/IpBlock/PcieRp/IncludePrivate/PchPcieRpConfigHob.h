/** @file
  PCIE RP config HOB definition.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#ifndef _PCIE_RP_CONFIG_HOB_H_
#define _PCIE_RP_CONFIG_HOB_H_

#include <Uefi/UefiBaseType.h>
#include <PchPcieRpConfig.h>
#include <Library/PchLimits.h>

/**
  The PCIE_RP_CONFIG_HOB describes settings for PCH.
**/
typedef struct {
UINT32                       ClkreqIpcCmdSupport      : 1;
UINT32                       Rsvd                     : 31;
PCH_PCIE_ROOT_PORT_CONFIG    RootPort[PCH_MAX_PCIE_ROOT_PORTS];
PCH_PCIE_CLOCK               PcieClock[PCH_MAX_PCIE_CLOCKS];
} PCIE_RP_CONFIG_HOB;

#endif
