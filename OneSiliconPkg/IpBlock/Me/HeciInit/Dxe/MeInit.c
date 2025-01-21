/** @file
  ME Initialize implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#include <IndustryStandard/Pci22.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseLib.h>
#include <Library/DxeMeLib.h>
#include <Library/HobLib.h>
#include <Library/HeciInitLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/MeInfoLib.h>
#include <Protocol/Wdt.h>
#include <Protocol/AmtReadyToBoot.h>
#include <Register/HeciRegs.h>
#include <PchResetPlatformSpecific.h>
#include "HeciInit.h"
#include "MeInit.h"
#include <MeBiosPayloadHob.h>

GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN               mEopSentInFsp;

/**
  Perform Arb SVN commit when any of CSE managed SVN have a mismatch with minimum allowed SVN.
**/
VOID
PerformAutoArbSvnCommit (
  VOID
  )
{
  ME_BIOS_PAYLOAD_HOB             *MbpHob;

  DEBUG ((DEBUG_INFO, "%a ()\n", __FUNCTION__));

  MbpHob = NULL;
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob != NULL) {
    if (MbpHob->MeBiosPayload.ArbSvnState.Available == 1) {
      if ((MbpHob->MeBiosPayload.ArbSvnState.ArbSvnData.Flags & ARB_SVN_MISMATCH) ||
          (MbpHob->MeBiosPayload.ArbSvnState.ArbSvnData.MinCseArbSvn < MbpHob->MeBiosPayload.ArbSvnState.ArbSvnData.CurrCseArbSvn)) {
        HeciArbSvnCommitMsg ();
      }
    }
  }
}

/**
  Signal a event to save Me relevant registers and check if EOP is sent inside FSP Dispatch.
  This event must be run before ExitPmAuth.

  @param[in] Event                The event that triggered this notification function
  @param[in] Context              Pointer to the notification functions context

**/
VOID
EFIAPI
MeEndOfDxeEvent (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  )
{
  EFI_STATUS                      Status;
  VOID                            *Interface;
  UINT32                          MeMode;
  HECI_FWS_REGISTER               MeFirmwareStatus;

  DEBUG ((DEBUG_INFO, "%a () Start\n", __FUNCTION__));

  ///
  /// Check ME Status
  ///
  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status)) {
    return;
  }

  MeFirmwareStatus.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS);

  ///
  /// PCH BIOS Spec Rev 0.9 Section 18.4  Additional Power Management Programming
  /// Step 2
  ///   Set "Power Management Initialization Register (ETR3) Field 1", D31:F2:ACh[31] = 1b
  ///   for production machine according to "RS - PCH Intel Management Engine
  ///  (Intel(r) ME) BIOS Writer's Guide".
  ///
  /// PCH ME BWG section 4.5.1
  /// The IntelR FPT tool /GRST option uses CF9GR bit to trigger global reset.
  /// Based on above reason, the BIOS should not lock down CF9GR bit during Manufacturing and
  /// Re-manufacturing environment if HFSTS1[4] is set.
  ///
  /// ETR3 is a resume well register and has no script save for it.
  /// System may go through S3 resume path from G3 if DeepSleep is enabled,
  /// that means all resume well registers will be reset to defaults.
  /// Save boot script for ETR3 register if DeepSleep is enabled.
  ///
  if (((MeMode == ME_MODE_NORMAL) || (MeMode == ME_MODE_TEMP_DISABLED)) && !(MeFirmwareStatus.r.SpiProtectionMode)) {
    ///
    /// PCH ME BWG section 4.4.1
    /// BIOS must also ensure that CF9GR is cleared and locked (via bit31 of the same register) before
    /// handing control to the OS in order to prevent the host from issuing global resets and resetting
    /// Intel Management Engine.
    ///
    PmcDisableCf9GlobalResetWithLockInS3BootScript ();
  } else {
    PmcDisableCf9GlobalResetInS3BootScript ();
  }

  //
  // Terminate Heci communication if CSME is in Debug Mode.
  //
  if (MeMode == ME_MODE_DEBUG) {
    Status = HeciTerminateHeciMsg ();
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Failed to send Terminate HECI command. Status = %r\n", Status));
    }
  }

  if (MeAutoArbSvnCommitEnabled ()) {
    PerformAutoArbSvnCommit ();
  }

  //
  // Sending notification that BIOS is starting to run 3rd party code.
  //
  if (MeCoreBiosDoneEnabled ()) {
    HeciCoreBiosDoneMsg ();
  }


  ///
  /// The protocol needs to be checked in EndOfDxe to prevent 3rd party installing it to skip EOP
  /// If the protocol located successfully, it indicates EOP should be sent in FSP Dispatch; otherwise, in wrapper.
  ///
  Status = gBS->LocateProtocol (&gMeSendEopInFspProtocolGuid, NULL, (VOID **)&Interface);
  mEopSentInFsp = (!EFI_ERROR (Status))? TRUE: FALSE;

  gBS->CloseEvent (Event);
}

/**
  Signal Me Ready To Boot Event
  Perform tasks below in the following order:
  1. Signal AmtReadyToBoot events for AMT platform hooks.
  2. Signal MePlatformReadyToBoot event for platform hooks.
  3. Report FWSTS and save to SMBIOS.
  4. Send EOP message.
  5. Lock config for manufacturing mode.
  6. Disable ME devices if particular ME-BIOS boot path requires this
     and this has not been done in PEI phase.

  @param[in] Event            The event that triggered this notification function
  @param[in] Context          Pointer to the notification functions context
**/
VOID
EFIAPI
MeReadyToBootEvent (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  )
{
  DEBUG ((DEBUG_INFO, "%a () Start\n", __FUNCTION__));

  if (!mEopSentInFsp) {
    MeReadyToBoot ();
  }

  gBS->CloseEvent (Event);
}
