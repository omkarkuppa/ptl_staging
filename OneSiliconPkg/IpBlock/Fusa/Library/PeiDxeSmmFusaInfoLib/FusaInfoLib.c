/** @file
  Source file for common Fusa Info Lib.

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
#include <Library/DebugLib.h>
#include <Ppi/SiPolicy.h>
#include <Register/Ptl/Msr/MsrRegs.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Protocol/CpuNvsArea.h>
#include <Protocol/BiosGuardNvsArea.h>
#include <Library/PmcLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/FusaInfoLib.h>

#define R_PMC_PWRM_FUSA_STS_CTL                     0x1F30U
#define B_PMC_PWRM_FUSA_STS_CTL_DIAGTEST_EN         BIT2
#define B_PMC_PWRM_FUSA_STS_CTL_DIAGTEST_PCHMODE    BIT1
#define B_PMC_PWRM_FUSA_STS_CTL_DIAGTEST_FEATURE_EN BIT0
#define MSR_FUSA_CAPABILITIES_A                 0x000002D9
/**
  Check is the Core is supporting fusa.

  @retval BOOLEAN TRUE/FALSE
**/
BOOLEAN
IsFusaCoreSupported (
  VOID
  )
{
#if FixedPcdGetBool(PcdFusaSupport) == 1
  MSR_CORE_CAPABILITIES_REGISTER                Msr;

  Msr.Uint64 = AsmReadMsr64 (MSR_CORE_CAPABILITIES);
  if (Msr.Bits.FusaSupported) {
    DEBUG ((DEBUG_INFO, "FuSA Core Capability detected\n"));
    return TRUE;
  } else {
    DEBUG ((DEBUG_INFO, "non-FuSA Core Capability detected\n"));
    return FALSE;
  }
#else
  DEBUG ((DEBUG_INFO, "non-FuSA Core Capability detected\n"));
  return FALSE;
#endif
}

/**
  Check is the silicon is supporting fusa.

  @retval BOOLEAN TRUE/FALSE
**/
BOOLEAN
IsFusaSupported (
  VOID
  )
{
#if FixedPcdGetBool(PcdFusaSupport) == 1
  MSR_CORE_CAPABILITIES_REGISTER                Msr;
  UINT64 FusaCapMsr;

  Msr.Uint64 = AsmReadMsr64 (MSR_CORE_CAPABILITIES);
  if (Msr.Bits.FusaSupported) {
    FusaCapMsr = AsmReadMsr64 (MSR_FUSA_CAPABILITIES_A);

    // check if MSR is not 0
    if (FusaCapMsr) {
      return TRUE;
    }
  }
  return FALSE;
#else
  return FALSE;
#endif
}

/**
  Detect Fusa diagnostic mode
  The existence of gSiFusaInfoGuid HOB implies the mode is on

  @return TRUE/FALSE of Fusa Diagnostic Mode

**/
BOOLEAN
IsInFusaDiagnosticMode (
  VOID
  )
{
  EFI_HOB_GUID_TYPE   *GuidHob;
  BOOLEAN             FusaDiagnosticMode;

  GuidHob = GetFirstGuidHob (&gSiFusaInfoGuid);
  if (GuidHob != NULL) {
    FusaDiagnosticMode = TRUE;
  }
  else
  {
    FusaDiagnosticMode = FALSE;
  }

  return FusaDiagnosticMode;
}
