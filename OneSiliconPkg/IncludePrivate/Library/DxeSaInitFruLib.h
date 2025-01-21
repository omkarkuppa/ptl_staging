/** @file
  Header file for the HybridGraphics Dxe driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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
#ifndef _DXE_SA_INIT_FRU_LIB_H_
#define _DXE_SA_INIT_FRU_LIB_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/MmPciLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/HobList.h>
#include <Library/PciSegmentLib.h>
#include <Defines/HostBridgeDefines.h>
#include <HostBridgeDataHob.h>

#ifndef FORCE_DISABLE
#define FORCE_DISABLE 2
#endif
#ifndef FORCE_ENABLE
#define FORCE_ENABLE  1
#endif
#ifndef PLATFORM_POR
#define PLATFORM_POR  0
#endif

/**
  This function locks the PAM register as part of the SA Security requirements.

**/
VOID
PamLock (
  VOID
  );
#endif
