/** @file
  Header file for dump functions of USB4 capabilities and path entries.

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

@par Specification
**/

#ifndef _USB4_DUMP_H_
#define _USB4_DUMP_H_

#include <Uefi.h>
#include <Library/Usb4ConfigSpace.h>
#include <Library/Usb4Capabilities.h>

/**
  Dump adapter TMU capabilities.

  @param[in] TmuCap - Pointer to TMU capabilities.
**/
VOID
DumpLaneAdapterTmuCap (
  IN USB4_ADP_CAP_TMU    *TmuCap
  );

/**
  Dump Lane adapter capabilities.

  @param[in] AdpCap - Pointer to Lane adapter capabilities.
**/
VOID
DumpLaneCap (
  IN USB4_ADP_CAP_LANE    *AdpCap
  );

/**
  Dump DP-IN adapter capability.

  @param[in] DpInCap - Pointer to DP-IN adapter capability.
**/
VOID
DumpAdpDpInCap (
  IN USB4_ADP_CAP_DP_IN    *DpInCap
  );

/**
  Dump DP-OUT adapter capability.

  @param[in] DpOutCap - Pointer to DP-OUT adapter capability.
**/
VOID
DumpAdpDpOutCap (
  IN USB4_ADP_CAP_DP_OUT    *DpOutCap
  );

/**
  Dump USB3 adapter capabilities

  @param[in] AdpCap - Pointer to USB3 adapter capabilities.
**/
VOID
DumpAdpUsb3Cap (
  IN USB4_ADP_CAP_USB3    *AdpCap
  );

/**
  Dump PCIe adapter capabilities

  @param[in] AdpCap - Pointer to PCIe adapter capabilities.
**/
VOID
DumpAdpPcieCap (
  IN USB4_ADP_CAP_PCIE    *AdpCap
  );

/**
  Dump Lane adapter capabilities CS DWORD1

  @param[in] AdpCap - Pointer to Lane adapter capabilities CS DWORD1.
**/
VOID
DumpLaneCapCs1 (
  IN USB4_ADP_CAP_LANE_CS_1    *AdpCapCs1
  );

/**
  Dump path entry

  @param[in] PathEntry - Pointer to the path entry
**/
VOID
DumpPathEntry (
  IN USB4_PATH_CS_ENTRY    *PathEntry
  );
#endif
