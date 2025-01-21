/** @file
  FSP DebugPrintErrorLevel library implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include <Base.h>
#include <FspmUpd.h>
#include <FsptUpd.h>
#include <Library/DebugLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/FspCommonLib.h>
#include <Library/PcdLib.h>

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
  UINT32                       DebugPrintErrorLevel;
  UINT32                       FspSerialDebugLevel;
  BOOLEAN                      DebugPrintErrorLevelInited;
  FSPM_UPD                     *FspmUpd;
  FSPT_UPD                     *FsptUpd;
  FSP_GLOBAL_DATA              *FspData;

  DebugPrintErrorLevel = PcdGet32 (PcdDebugPrintErrorLevel);
  DebugPrintErrorLevelInited = FALSE;
  FspSerialDebugLevel = 0;

  FspData = GetFspGlobalDataPointer ();
  if (((UINTN)FspData != 0x00 && (UINTN)FspData != 0xFFFFFFFF)) {
    if (FspData->Signature == FSP_GLOBAL_DATA_SIGNATURE) {
      if ((FspData->FspMode == FSP_IN_API_MODE) && (FspData->MemoryInitUpdPtr != NULL)) {              // FSP_MODE_CHECK
        FspmUpd = FspData->MemoryInitUpdPtr;
        FspSerialDebugLevel = FspmUpd->FspmConfig.PcdSerialDebugLevel;
        DebugPrintErrorLevelInited = TRUE;

      } else if ((FspData->FspMode == FSP_IN_DISPATCH_MODE) && (FspData->TempRamInitUpdPtr != NULL)) { // FSP_MODE_CHECK
        FsptUpd = FspData->TempRamInitUpdPtr;
        FspSerialDebugLevel = FsptUpd->FsptConfig.PcdSerialDebugLevel;
        DebugPrintErrorLevelInited = TRUE;
      }

      if (DebugPrintErrorLevelInited) {
        switch (FspSerialDebugLevel) {
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
      }
    }
  }

  return DebugPrintErrorLevel;
}

/**
  Sets the global debug print error level mask for the entire platform.

  @param   ErrorLevel     Global debug print error level.

  @retval  TRUE           The debug print error level mask was successfully set.
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