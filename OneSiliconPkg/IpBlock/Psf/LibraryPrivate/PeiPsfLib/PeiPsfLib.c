/** @file
  Initilize PSF device in PEI

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
#include <Library/DebugLib.h>
#include <Ppi/SiPolicy.h>
#include <Register/PchRegsPsf.h>
#include <Library/PsfLib.h>

/**
  Configure PSF power management.
  Must be called after all PSF configuration is completed.

  @param[in] PsfTable           Table of PSF elements
**/
VOID
PsfConfigurePowerManagement (
  IN  PSF_SEGMENT_TABLE *PsfTable
  )
{
  UINT32          PsfTableIndex;
  PSF_DEV         *PsfDev;
  UINT32          DataAnd32;
  UINT32          DataOr32;

  DataAnd32 = ~0u;
  DataOr32 = (B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENLCG | B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENPCG);

  //
  // There in an ordering requirement to program enTCG before enLCG.
  //
  for (PsfTableIndex = 0; PsfTableIndex < PsfTable->Size; PsfTableIndex++) {
    PsfDev = PsfTable->Data[PsfTableIndex].PsfDev;
    PsfDev->Access->AndThenOr32 (PsfDev->Access, R_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG, ~0u, B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENTCG);
    PsfDev->Access->AndThenOr32 (PsfDev->Access, R_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG, DataAnd32, DataOr32);
  }
}
