/** @file
  Pei/Dxe/Smm DebugPrintErrorLevel Common library implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Pi/PiMultiPhase.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Setup.h>

GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN     mDebugPrintErrorLevelInited = FALSE;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32      mDebugPrintErrorLevel = 0xffffffff;

VOID CacheDebugPrintErrorLevel ();

/**
  Returns the debug print error level mask for the current module.

  @return  Debug print error level mask for the current module.

**/
UINT32
EFIAPI
GetDebugPrintErrorLevel (
  VOID
  )
{
  EFI_HOB_GUID_TYPE            *GuidHob;
  DEBUG_CONFIG_DATA_HOB        *DebugConfigData;
  UINT32                       DebugPrintErrorLevel;

  if (mDebugPrintErrorLevelInited == TRUE) {
    return mDebugPrintErrorLevel;
  }

  GuidHob = NULL;
  DebugConfigData = NULL;
  DebugPrintErrorLevel = 0;

  GuidHob = GetFirstGuidHob (&gDebugConfigHobGuid);
  if (GuidHob != NULL) {
    DebugConfigData = (DEBUG_CONFIG_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
    if (DebugConfigData != NULL) {
      switch (DebugConfigData->SerialDebug) {
      case 0:
        DebugPrintErrorLevel = 0;
        break;
      case 1:
        DebugPrintErrorLevel = DEBUG_ERROR;
        break;
      case 2:
        DebugPrintErrorLevel = DEBUG_ERROR | DEBUG_WARN;
        break;
      case 4:
        DebugPrintErrorLevel = DEBUG_ERROR | DEBUG_WARN | DEBUG_INFO | DEBUG_EVENT | DEBUG_LOAD;
        break;
      case 5:
        DebugPrintErrorLevel = DEBUG_ERROR | DEBUG_WARN | DEBUG_INFO | DEBUG_VERBOSE | DEBUG_LOAD;
        break;
      default:
        DebugPrintErrorLevel = DEBUG_ERROR | DEBUG_WARN | DEBUG_INFO | DEBUG_LOAD;
        break;
      }
      CacheDebugPrintErrorLevel (DebugPrintErrorLevel);
      return DebugPrintErrorLevel;
    }
  }

  return PcdGet32 (PcdDebugPrintErrorLevel);
}

/**
  Sets the global debug print error level mask for the entire platform.

  @param   ErrorLevel     Global debug print error level.

  @retval  TRUE           The debug print error level mask was sucessfully set.
  @retval  FALSE          The debug print error level mask could not be set.

**/
BOOLEAN
EFIAPI
SetDebugPrintErrorLevel (
  UINT32  ErrorLevel
  )
{
  return FALSE;
}
