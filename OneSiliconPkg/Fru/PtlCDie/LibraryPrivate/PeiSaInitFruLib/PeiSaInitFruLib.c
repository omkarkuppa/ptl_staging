/** @file
  This file is the implementation for PeiSaInitFru library.

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

#include <Library/PeiHostBridgeIpStatusLib.h>

/**
  This function performs SA internal devices enabling/disabling

  @param[in] HostBridgePeiConfig - Instance of HOST_BRIDGE_PEI_CONFIG

**/
VOID
DeviceConfigure (
  IN    HOST_BRIDGE_PEI_CONFIG  *HostBridgePeiConfig
  )
{

  ///
  /// Enable/Disable Thermal device (B0,D4,F0).
  ///
  if (HostBridgePeiConfig->Device4Enable) {
    UncoreDevEnWrite (Camarillo, 0, Enable);
  } else {
    UncoreDevEnWrite (Camarillo, 0, Disable);
  }
  return;
}