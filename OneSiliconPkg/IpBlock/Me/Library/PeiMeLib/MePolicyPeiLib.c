/** @file
  Implementation file for Me Policy functionality for PEIM

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <PiPei.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PeiMeLib.h>
#include <Library/ConfigBlockLib.h>
#include <Pi/PiHob.h>
#include <MePeiConfig.h>
#include <Library/MeUtilsLib.h>
#include <Library/MeInitLib.h>
#include <Library/HeciInitLib.h>

extern GUID gMeEopDoneHobGuid;

/**
  Initialize ME PEI PreMem Config Block.

  @param[in, out] MePeiPreMemConfig    Pointer of ME PEI Pre-Mem Config.

  @retval EFI_SUCCESS                  ME PEI PreMem Config Block is initialized.
  @retval All other error conditions encountered when ME PEI PreMem Config Block initialized fail.
**/
STATIC
EFI_STATUS
MePeiPreMemConfigInit (
  IN OUT ME_PEI_PREMEM_CONFIG **MePeiPreMemConfig
  )
{
  EFI_STATUS                      Status;
  SI_PREMEM_POLICY_PPI            *SiPreMemPolicyPpi;

  ///
  /// Get Policy settings through the SiPreMemPolicy PPI
  ///
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  if (!EFI_ERROR (Status)) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMePeiPreMemConfigGuid, (VOID *) MePeiPreMemConfig);
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**
  Initialize ME PEI Config Block.

  @param[in, out] MePeiConfig     Pointer of ME PEI Config.

  @retval EFI_SUCCESS             ME PEI Config Block is initialized.
  @retval All other error conditions encountered when ME PEI Config Block initialized fail.
**/
STATIC
EFI_STATUS
MePeiConfigInit (
  IN OUT ME_PEI_CONFIG **MePeiConfig
  )
{
  EFI_STATUS                      Status;
  SI_POLICY_PPI                   *SiPolicyPpi;

  ///
  /// Get Policy settings through the SiPolicy PPI
  ///
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicyPpi
             );
  if (!EFI_ERROR (Status)) {
    Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gMePeiConfigGuid, (VOID *) MePeiConfig);
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}


/**
  Read the ME policy to see if HECI timeouts are enabled.

  @retval TRUE             Timeout is enabled
  @retval FALSE            Timeout is disabled
**/
BOOLEAN
MeHeciTimeoutsEnabled (
  VOID
  )
{
  EFI_STATUS                      Status;
  ME_PEI_PREMEM_CONFIG            *MePeiPreMemConfig;

  Status = MePeiPreMemConfigInit (&MePeiPreMemConfig);
  if (!EFI_ERROR (Status)) {
    if (MePeiPreMemConfig->HeciTimeouts == 0) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
  Read the ME policy to see if HECI message check is enabled for Bios Boot Path.

  @retval TRUE             HECI message check is enabled
  @retval FALSE            HECI message check is disabled
**/
BOOLEAN
MeHeciMessageCheckEnabled (
  VOID
  )
{
  EFI_STATUS                      Status;
  ME_PEI_PREMEM_CONFIG            *MePeiPreMemConfig;

  Status = MePeiPreMemConfigInit (&MePeiPreMemConfig);
  if (!EFI_ERROR (Status)) {
    if (MePeiPreMemConfig->DisableMessageCheck == 1) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
  Read the ME policy to see if MBP HOB is skipped.

  @retval TRUE             MBP HOB is skipped
  @retval FALSE            MBP HOB is not skipped
**/
BOOLEAN
MeMbpHobSkipped (
  VOID
  )
{
  EFI_STATUS                      Status;
  ME_PEI_PREMEM_CONFIG            *MePeiPreMemConfig;

  Status = MePeiPreMemConfigInit (&MePeiPreMemConfig);
  if (!EFI_ERROR (Status)) {
    if (MePeiPreMemConfig->SkipMbpHob == 1) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Read the ME policy to see if D0I3 for HECI devices are enabled.

  @retval TRUE             D0I3 for HECI devices are enabled
  @retval FALSE            D0I3 for HECI devices are disabled
**/
BOOLEAN
MeHeciD0I3Enabled (
  VOID
  )
{
  EFI_STATUS                      Status;
  ME_PEI_CONFIG                   *MePeiConfig;

  Status = MePeiConfigInit (&MePeiConfig);
  if (!EFI_ERROR (Status)) {
    if (MePeiConfig->DisableD0I3SettingForHeci == 1) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
  Check if End of Post Message is enabled in setup options.

  @retval FALSE                   EndOfPost is disabled.
  @retval TRUE                    EndOfPost is enabled.
**/
BOOLEAN
MeEndOfPostEnabled (
  VOID
  )
{
  EFI_STATUS      Status;
  ME_PEI_CONFIG   *MePeiConfig;

  Status = MePeiConfigInit (&MePeiConfig);
  if (!EFI_ERROR (Status)) {
    if (MePeiConfig->EndOfPostMessage != EOP_SEND_IN_PEI) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
  Checks if EndOfPost event already happened

  @retval TRUE                    End of post already happened
  @retval FALSE                   End of post did not happen yet
**/
BOOLEAN
MeIsAfterEndOfPost (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS            HobPtr;
  BOOLEAN                         Done;

  Done = FALSE;
  HobPtr.Guid  = GetFirstGuidHob (&gMeEopDoneHobGuid);
  if (HobPtr.Guid != NULL) {
    Done = TRUE;
  }

  DEBUG ((DEBUG_INFO, "Is EndOfPost sent in PEI: %a\n", Done ? "TRUE" : "FALSE"));
  return Done;
}

/**
  Set EndOfPost done
**/
VOID
MeSetEndOfPostDone (
  VOID
  )
{
  VOID                            *HobPtr;
  UINT32                          EopDone;

  DEBUG ((DEBUG_INFO, "MeSetEndOfPostDone in PEI\n"));
  EopDone = 1;
  HobPtr = BuildGuidDataHob (&gMeEopDoneHobGuid, &EopDone, sizeof (EopDone));
  ASSERT (HobPtr != 0);
}

/**
  Checks if ME is available for HSIO

  @retval FALSE         Can't send HSIO messages
  @retval TRUE          HSIO messages can be send

**/
BOOLEAN
IsMeAvailableForHsio (
  VOID
  )
{
  // When ME is in recovery mode do not sent HSIO
  if (MeClientInRecovery ()) {
    DEBUG ((DEBUG_WARN, "(HSIO) WARNING: ME in recovery mode does not support HSIO sync\n"));
    return FALSE;
  }

  return TRUE;
}

/**
  Check if HECI1 should be force enabled for debug purpose.

  @retval FALSE                   HECI1 should follow POR setting.
  @retval TRUE                    HECI1 should be force enabled.
**/
BOOLEAN
MeIsHeci1ForceEnabled (
  VOID
  )
{
  EFI_STATUS              Status;
  ME_PEI_PREMEM_CONFIG    *MePeiPreMemConfig;

  Status = MePeiPreMemConfigInit (&MePeiPreMemConfig);
  if (!EFI_ERROR (Status) && (MePeiPreMemConfig->HeciCommunication == FORCE_ENABLE)) {
    return TRUE;
  }

  return FALSE;
}
