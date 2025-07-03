/** @file
  Time Coordinated Compute (TCC) policy library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
#include <Setup.h>
#include <SetupVariable.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PeiDxeSmmCpuPlatformLib/CpuPlatformLibrary.h>
#include <Library/Ptl/PcdInfoLib/PtlPcdInfoLib.h>

/**
  Returns if TCC mode is enabled

  @return TRUE  TCC mode is enabled
  @return FALSE TCC mode is disabled
**/
BOOLEAN
IsTccModeEnabled (
  SETUP_DATA *SetupData
  )
{
  if (SetupData->TccMode == 1) {
    return TRUE;
  }

  return FALSE;
}

/**
  Gets the TCC Mode policy configuration.

  @param  CpuSetup   CPU setup policies with TCC Mode defaults
  @param  PchSetup   PCH setup policies with TCC Mode defaults
  @param  SaSetup    SA setup policies with TCC Mode defaults
  @param  SetupData  System setup policies with TCC Mode defaults

**/
VOID
GetTccModeConfigPolicy (
  CPU_SETUP  *CpuSetup,
  PCH_SETUP  *PchSetup,
  SA_SETUP   *SaSetup,
  SETUP_DATA *SetupData
  )
{
#if FixedPcdGet8(PcdTccSupport) == 0x1
  DEBUG ((DEBUG_INFO, "GetTccModeConfigPolicy - Start\n"));
  // Get TCC Mode Setting
  if (CpuSetup != NULL) {
    // PTL cores support Fast GV so disabling cores GV and Hwp not required
    CpuSetup->EnableGv = CpuSetup->EnableGv;
    CpuSetup->EnableHwp = CpuSetup->EnableHwp;
    CpuSetup->EnableCx = 0;
  }
  if (PchSetup != NULL) {
    PchSetup->PsfTccEnable = 1;
    PchSetup->DmiAspm = 0;
    PchSetup->PchLegacyIoLowLatency = 1;
  }

  if (SaSetup != NULL) {
    // PTL uncore suppor Drainless GV so disabling uncore GV not required
    SaSetup->SaGv = 0;
    SaSetup->DisPgCloseIdleTimeout = 1;
    SaSetup->PowerDownMode = 0;
    SaSetup->EnableRenderStandby = 0;
  }

  if (SetupData != NULL) {
    SetupData->Rtd3Support = 0;
    SetupData->LowPowerS0Idle = 0;
  }
  DEBUG ((DEBUG_INFO, "GetTccModeConfigPolicy - End\n"));

  return;
#else
  return;
#endif
}
