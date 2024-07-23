/** @file
  Source code file for the SA Platform Init DXE module

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#include "BoardInitAdvancedDxe.h"
#include <Protocol/BlockIo.h>
#include <Protocol/GopPolicy.h>
#include <Library/DebugLib.h>
#include <Protocol/Cpu.h>
#include <Protocol/PciEnumerationComplete.h>
#include <PlatformNvRamHookLib.h>
#include <Library/HostBridgeDataLib.h>
#include <Library/HostBridgeInfoLib.h>
#include <Library/DxeItbtVariableUpdateLib.h>

#define EFI_MAX_ADDRESS       0xFFFFFFFF

VOID
UpdateGmAdrRegionToWriteCombine (
  VOID
  );

VOID
EFIAPI
PciEnumCompleteCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

/**
  System Agent platform Initialization

**/
VOID
SaPlatformInitDxe (
  VOID
  )
{

#if FixedPcdGetBool (PcdTcssSupport) == 1
  //
  // Save Itbt Dma UUID Data
  //
  UpdateItbtDmaUuidVariable();
#endif
  SyncItbtVariables ();

}