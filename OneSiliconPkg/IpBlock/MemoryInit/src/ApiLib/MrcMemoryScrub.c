/** @file
  This file contains the memory scrubbing and alias checking functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.
**/

#include "CMrcTypes.h"
#include "CMrcInterface.h"
#include "MrcGeneral.h"
#include "MrcMemoryScrub.h"

/**
  Checks the MRC inputs and current boot configuration to determine if
  test engine based memory scrubbing is required.

  @param[in] MrcData - Include all MRC global data.
  @param[in] MrcData - Enable the debug print.

  @return TRUE if test engine based memory scrubbing is required, otherwise FALSE.
**/
BOOLEAN
MrcIsTeMemoryScrubbingRequired (
  IN     MrcParameters *const MrcData,
  IN     BOOLEAN              IsDebugPrint
  )
{
  const MrcInput        *Inputs;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  BOOLEAN               EccEnabled;
  BOOLEAN               TxtClean;
  BOOLEAN               CleanMemory;
  BOOLEAN               WarmReset;
#ifdef MRC_DEBUG_PRINT
  static const char *SourceStr[4] = { "ECC", "CleanMemory", "TXT" };
#endif

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;
  EccEnabled  = (Outputs->EccSupport == TRUE);
  TxtClean    = Inputs->TxtClean;
  CleanMemory = (BOOLEAN) Inputs->CleanMemory;
  WarmReset   = (Inputs->BootMode == bmWarm);

  if (!(((EccEnabled || CleanMemory) && (!WarmReset)) || TxtClean)) {
    // Memory clean is not needed if none of the (EccEnabled || CleanMemory || TxtClean) bit is set
    // In WarmRest, only TxtClean needs to preform this EccClean function, not required for EccEnabled or CleanMemory
    return FALSE;
  }

  if (IsDebugPrint) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Scrubbing Memory required due to %s\n", EccEnabled ? SourceStr[0] : (TxtClean ? SourceStr[2] : SourceStr[1]));
  }
  return TRUE;
}