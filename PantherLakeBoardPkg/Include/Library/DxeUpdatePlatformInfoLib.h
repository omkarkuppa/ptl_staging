/** @file
  Function prototype of DxeUpdatePlatformInfoLib.

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

#ifndef _DXE_UPDATE_PLATFORM_INFO_LIB_H_
#define _DXE_UPDATE_PLATFORM_INFO_LIB_H_

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/PciSegmentLib.h>
#include <OemSetup.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/PchInfoLib.h>
#include <SetupVariable.h>


/**

  This function update the UEFI variable "SetupVolatileData" to reflect current platform values,
  Return out if the variable is NOT found.

**/
VOID
UpdatePlatformInfo (
  VOID
  );

#endif // _DXE_UPDATE_PLATFORM_INFO_LIB_H_
