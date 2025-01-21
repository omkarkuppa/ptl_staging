/** @file
  Initializes PCH at end of PEI

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

#include "PtlPcdInitPei.h"
#include <IntelRcStatusCode.h>
#include <Library/PeiWdtLib.h>
#include <Library/PeiRtcLib.h>
#include <Library/PeiPcieRpInitLib.h>
#include <Library/IocLib.h>
#include <Library/PeiPchDmiLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/PcdGpioNativeLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdFiaSocLib.h>
#include <Library/P2SbSocLib.h>
#include <LockDownConfig.h>
#include <Library/PerformanceLib.h>

/**
  Configures BIOS Interface Lock Down bit

  @param[in] SiPolicyPpi  The SI Policy PPI instance
**/
STATIC
VOID
PtlPcdBiosInterfaceLockDown (
  IN  SI_POLICY_PPI                    *SiPolicyPpi
  )
{
  PCH_LOCK_DOWN_CONFIG         *LockDownConfig;
  EFI_STATUS                   Status;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gLockDownConfigGuid, (VOID *) &LockDownConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  if (LockDownConfig->BiosInterface == FALSE) {
    return;
  }
  IocSetBiosLockDown ();
}

/**
  Pch End of PEI callback function. This is the last event before entering DXE and OS in S3 resume.
**/
VOID
PtlPcdOnEndOfPei (
  VOID
  )
{
  EFI_STATUS             Status;
  SI_PREMEM_POLICY_PPI   *SiPreMemPolicy;
  SI_POLICY_PPI          *SiPolicy;
  FIA_CONFIG             *FiaConfig;

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_ENDOFPEI_START);
  DEBUG ((DEBUG_INFO, "PchOnEndOfPei after memory PEI module - Start\n"));

  //
  // Get Policy settings through the SiPolicy PPI
  //
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicy
             );
  if (Status != EFI_SUCCESS) {
    //
    // SI_POLICY_PPI must be installed at this point
    //
    ASSERT (FALSE);
    return;
  }

  //
  // Get Policy settings through the SiPreMemPolicy PPI
  //
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  if (Status != EFI_SUCCESS) {
    //
    // SI_PREMEM_POLICY_PPI must be installed at this point
    //
    ASSERT (FALSE);
    return;
  }

  PtlPcdSpiDmaWaEnd();

  //
  // Clear RTC PWR STS in end of PEI so others won't miss the status
  //
  ///
  /// PCH BIOS Spec Section 19.8.1 RTC Resets
  /// The PCH will set the RTC_PWR_STS bit when the RTCRST# pin goes low.
  /// The System BIOS shouldn't rely on the RTC RAM contents when the RTC_PWR_STS bit is set.
  /// BIOS should clear this bit by writing a 0 to this bit position.
  /// This bit isn't cleared by any reset function.
  ///
  PmcClearRtcPowerFailureStatus ();


  //
  // Init thermal device and lock down settings.
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_ENDOFPEI_INIT_THERMAL);
  ThermalInit (SiPolicy, PmcGetPwrmBase ());

  WdtDisableBeforeS3Resume ();

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFiaConfigGuid, (VOID *) &FiaConfig);
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status) && FiaConfig->FiaProgramming) {
    PtlPcdFiaFinalizeConfigurationAndLock ();
  }

  PtlPcdPmcInitEndOfPei (SiPolicy);

  //
  // Lock RTC settings
  //
  PtlPcdRtcLock (SiPolicy);

  //
  // Perform initial IO Standby State related configurations at EndOfPei
  //
  PERF_INMODULE_BEGIN ("PcdGpioConfigIoStandBy");
  PtlPcdGpioConfigureIoStandbyState ();
  PERF_INMODULE_END ("PcdGpioConfigIoStandBy");

  //
  // HdAudio Init
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_ENDOFPEI_INIT_ACE);
  PERF_INMODULE_BEGIN ("PcdAcePostMemInit");
  PtlPcdAcePostMemInit (SiPreMemPolicy);
  PERF_INMODULE_END ("PcdAcePostMemInit");

  //
  // Configure P2SB at the end of EndOfPei
  // This must be done before POSTBOOT_SAI programming.
  //
  PtlPcdP2sbLock (SiPolicy);
  if (!PtlPcdIsSecondP2SbHidden ()) {
    PtlPcdSecondP2sbLock (SiPolicy);
  }

  PtlPcdBiosInterfaceLockDown (SiPolicy);

  //
  // Send IPC Memory Calibration Done Notification to PMC
  //
  PmcSendBiosResetCompletionMemCalDone (PmcGetPwrmBase ());

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_ENDOFPEI_DONE);
  DEBUG ((DEBUG_INFO, "PchOnEndOfPei after memory PEI module - End\n"));
}
