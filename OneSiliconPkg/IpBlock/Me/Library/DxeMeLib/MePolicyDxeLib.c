/** @file
  Implementation file for Me Policy functionality

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2006 Intel Corporation.

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
#include <PiDxe.h>
#include <MePeiConfig.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeMeLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/HobLib.h>
#include <PchPolicyCommon.h>

extern GUID gMeEopDoneHobGuid;
extern GUID gMeEopDoneProtocolGuid;

//
// Global variables
//
GLOBAL_REMOVE_IF_UNREFERENCED ME_PEI_PREMEM_CONFIG          *mMePeiPreMemConfig = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED ME_PEI_CONFIG                 *mMePeiConfig = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED ME_DXE_CONFIG                 *mMeDxeConfig = NULL;


/**
  Initialize module varialbe - mMePeiPreMemConfig for ME PEI Config Block.

  @retval EFI_SUCCESS             mMePeiPreMemConfig is initialized.
  @retval All other error conditions encountered when mMePeiPreMemConfig initialized fail.
**/
STATIC
EFI_STATUS
MePeiPreMemConfigInit (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS            HobPtr;

  if (mMePeiPreMemConfig != NULL) {
    return EFI_SUCCESS;
  }

  //
  // Get ME PreMem Policy HOB.
  //
  HobPtr.Guid  = GetFirstGuidHob (&gMePreMemPolicyHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  if (HobPtr.Guid != NULL) {
    mMePeiPreMemConfig = (ME_PEI_PREMEM_CONFIG *) GET_GUID_HOB_DATA (HobPtr.Guid);
    return EFI_SUCCESS;
  }

  return EFI_UNSUPPORTED;
}

/**
  Initialize module varialbe - mMePeiConfig for ME PEI Config Block.

  @retval EFI_SUCCESS             mMePeiConfig is initialized.
  @retval All other error conditions encountered when mMePeiConfig initialized fail.
**/
STATIC
EFI_STATUS
MePeiConfigInit (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS            HobPtr;

  if (mMePeiConfig != NULL) {
    return EFI_SUCCESS;
  }

  //
  // Get ME Policy HOB.
  //
  HobPtr.Guid  = GetFirstGuidHob (&gMePolicyHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  if (HobPtr.Guid != NULL) {
    mMePeiConfig = (ME_PEI_CONFIG *) GET_GUID_HOB_DATA (HobPtr.Guid);
    return EFI_SUCCESS;
  }

  return EFI_UNSUPPORTED;
}

/**
  Initialize module varialbe - mMeDxeConfig for ME DXE Config Block.

  @retval EFI_SUCCESS             mMeDxeConfig is initialized.
  @retval All other error conditions encountered when mMeDxeConfig initialized fail.
**/
STATIC
EFI_STATUS
MeDxeConfigInit (
  VOID
  )
{
  EFI_STATUS                      Status;
  ME_POLICY_PROTOCOL              *DxeMePolicy;

  if (mMeDxeConfig != NULL) {
    return EFI_SUCCESS;
  }

  DxeMePolicy = NULL;
  Status = gBS->LocateProtocol (&gDxeMePolicyGuid, NULL, (VOID **) &DxeMePolicy);
  if (EFI_ERROR (Status) || (DxeMePolicy == NULL)) {
    DEBUG ((DEBUG_ERROR, "No ME Policy Protocol available"));
    return EFI_UNSUPPORTED;
  }

  Status = GetConfigBlock ((VOID *) DxeMePolicy, &gMeDxeConfigGuid, (VOID *) &mMeDxeConfig);
  ASSERT_EFI_ERROR (Status);

  return Status;
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
  EFI_STATUS         Status;

  Status = MePeiConfigInit ();
  if (!EFI_ERROR (Status)) {
    if (mMePeiConfig->EndOfPostMessage != EOP_SEND_IN_DXE) {
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

  Status = MePeiPreMemConfigInit ();
  if (EFI_ERROR (Status)) {
    return TRUE;
  }

  if (mMePeiPreMemConfig->DisableMessageCheck == 1) {
    return FALSE;
  }

  return TRUE;
}

/**
  Check if MeFwDowngrade is enabled in setup options.

  @retval FALSE                   MeFwDowngrade is disabled.
  @retval TRUE                    MeFwDowngrade is enabled.
**/
BOOLEAN
MeFwDowngradeSupported (
  VOID
  )
{
  EFI_STATUS                      Status;

  Status = MeDxeConfigInit ();
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  if (mMeDxeConfig->MeFwDowngrade == 1) {
    return TRUE;
  }

  return FALSE;
}

/**
  Check if Core Bios Done Message is enabled in setup options.

  @retval FALSE                   Core Bios Done is disabled.
  @retval TRUE                    Core Bios Done is enabled.
**/
BOOLEAN
MeCoreBiosDoneEnabled (
  VOID
  )
{
  EFI_STATUS                      Status;

  Status = MeDxeConfigInit ();
  if (EFI_ERROR (Status)) {
    return TRUE;
  }

  if (mMeDxeConfig->CoreBiosDoneEnabled == 1) {
    return TRUE;
  }

  return FALSE;
}

/**
  Check if Auto Arb SVN Commit is enabled in setup options.

  @retval FALSE                   Auto Arb SVN Commit is disabled.
  @retval TRUE                    Auto Arb SVN Commit is enabled.
**/
BOOLEAN
MeAutoArbSvnCommitEnabled (
  VOID
  )
{
  EFI_STATUS                      Status;

  Status = MeDxeConfigInit ();
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  if (mMeDxeConfig->AutoArbSvnCommit == 1) {
    return TRUE;
  }

  return FALSE;
}

/**
  Read the ME policy to see if HECI timeouts are enabled.

  @retval FALSE                   HECI timeouts is disabled.
  @retval TRUE                    HECI timeouts is enabled.
**/
BOOLEAN
MeHeciTimeoutsEnabled (
  VOID
  )
{
  EFI_STATUS                      Status;

  Status = MePeiPreMemConfigInit ();
  if (EFI_ERROR (Status)) {
    return TRUE;
  }

  if (mMePeiPreMemConfig->HeciTimeouts == 0) {
    return FALSE;
  }

  return TRUE;
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

  Status = MePeiConfigInit ();
  if (EFI_ERROR (Status)) {
    return TRUE;
  }

  if (mMePeiConfig->DisableD0I3SettingForHeci == 1) {
    return FALSE;
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
  EFI_STATUS              Status;
  EFI_PEI_HOB_POINTERS    HobPtr;
  BOOLEAN                 Done;
  VOID                    *Interface;

  Done = FALSE;
  HobPtr.Guid  = GetFirstGuidHob (&gMeEopDoneHobGuid);
  if (HobPtr.Guid != NULL) {
    Done = TRUE;
  }

  if (Done == FALSE) {
    Status = gBS->LocateProtocol (&gMeEopDoneProtocolGuid, NULL, (VOID **)&Interface);
    if (!EFI_ERROR (Status)) {
      Done = TRUE;
    }
    DEBUG ((DEBUG_INFO, "Locate MeEopDone Protocol, Status: %r\n", Status));
  }

  DEBUG ((DEBUG_INFO, "Is EndOfPost sent in DXE: %a\n", Done ? "TRUE" : "FALSE"));
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
  EFI_STATUS                      Status;
  EFI_HANDLE                      Handle;

  DEBUG ((DEBUG_INFO, "MeSetEndOfPostDone in DXE\n"));
  //
  // Install the Protocol into a new handle.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gMeEopDoneProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT (Status == EFI_SUCCESS);
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
  EFI_STATUS    Status;

  Status = MePeiPreMemConfigInit ();
  if (!EFI_ERROR (Status) && (mMePeiPreMemConfig->HeciCommunication == FORCE_ENABLE)) {
    return TRUE;
  }

  return FALSE;
}
