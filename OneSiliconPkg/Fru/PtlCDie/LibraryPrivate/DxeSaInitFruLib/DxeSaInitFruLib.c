/** @file
  Source file for DXE SA Init Fru Lib

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
#include <Library/DxeSaInitFruLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>

/**
  Common function locks the PAM register as part of the SA Security requirements.

**/

VOID
PamLock (
  VOID
  )
{
  HOST_BRIDGE_DATA_HOB  *HostBridgeDataHob;

  ///
  /// Get Host Bridge Data HOB
  ///
  HostBridgeDataHob = NULL;
  HostBridgeDataHob = (HOST_BRIDGE_DATA_HOB *) GetFirstGuidHob (&gHostBridgeDataHobGuid);

  if (HostBridgeDataHob != NULL) {
    if (HostBridgeDataHob->SkipPamLock == FALSE) {
      //
      // Lock PAM by removing POSTBOOT_SAI from the PAM_W_PG WAC/CP
      //
      DEBUG ((DEBUG_INFO, "PAM_LOCK!!\n"));
      SetHostBridgeRegisterData (HostBridgePamWpgWac, HostBridgePamWpgWacWriteData, 0);
      SetHostBridgeRegisterData (HostBridgePamWpgCp, HostBridgePamWpgCpWriteData, 0);
    }
  }
}