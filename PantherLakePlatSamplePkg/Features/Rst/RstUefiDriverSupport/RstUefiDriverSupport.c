/** @file
  RST UEFI driver support driver. The purpose of this driver is to provide all
  platform dependencies to RST UEFI driver.

#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2023 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.
**/

#include <Uefi.h>
#include <Uefi/UefiMultiPhase.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>

#include <SetupVariable.h>

#include <RstFeaturesVariable.h>
#include <Library/VariablePolicyHelperLib.h>

#include <Protocol/FormBrowser2.h>

#if (FixedPcdGetBool (PcdVmdEnable) == 1)
#include <Library/VmdInfoLib.h>
#endif

typedef struct _RST_STATUS_PROTOCOL  RST_STATUS_PROTOCOL;

typedef enum _RST_STATUS_CODE {
    RstStatusCodeGood = 0,
    RstStatusCodeAbnormalRepairable,
    RstStatusCodeAbnormalUnrepairable,
    RstStatusCodeFailRepairable,
    RstStatusCodeFailUnrepairable
} RST_STATUS_CODE;

typedef
RST_STATUS_CODE
(EFIAPI *RST_STATUS_GET_OVERALL_STATUS) (
    IN RST_STATUS_PROTOCOL* This,
    OUT EFI_HII_HANDLE*     FormHandle
);

typedef struct _RST_STATUS_PROTOCOL {
    UINT64                             Revision;
    RST_STATUS_GET_OVERALL_STATUS      GetOverallStatus;
} RST_STATUS_PROTOCOL;

/**
  Search for RstStatusProtocol and check returned status.
  If Status != RstStatusCodeGood show message to user and display returned Form.
**/
VOID
EFIAPI
CheckRstStatusProtocol (
  VOID
  )
{
  EFI_STATUS                  Status;
  RST_STATUS_PROTOCOL         *RstStatusProtocol;
  RST_STATUS_CODE             RstStatus;
  UINT64                      Revision;
  EFI_HII_HANDLE              FormHandle;
  EFI_BROWSER_ACTION_REQUEST  ActionRequest;
  EFI_FORM_BROWSER2_PROTOCOL  *FormBrowser2;
  PCH_SETUP                   PchSetup;
  UINTN                       VariableSize;

  DEBUG ((DEBUG_INFO, "CheckRstStatusProtocol() Start.\n"));

  //
  // Find RstStatusProtocol
  //
  Status = gBS->LocateProtocol (
                  &gRstStatusProtocolGuid,
                  NULL,
                  (VOID **) &RstStatusProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Failed to locate RstStatusProtocol.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "Located RstStatusProtocol.\n"));

    Revision = RstStatusProtocol->Revision;
    RstStatus = RstStatusProtocol->GetOverallStatus(RstStatusProtocol,(VOID **) &FormHandle);
    DEBUG ((DEBUG_INFO, "Revision : %x \n Status   : %x \n",Revision,RstStatus));

    if (RstStatus != RstStatusCodeGood) {
      DEBUG ((DEBUG_ERROR, "Abnormal status reported by Rapid Storage Technology UEFI driver \n"));

      //
      // Display message to user
      //
      gST->ConOut->ClearScreen (gST->ConOut);
      gST->ConOut->SetCursorPosition(gST->ConOut,2,2);
      gST->ConOut->OutputString (
                  gST->ConOut,
                  L"Abnormal status reported by Rapid Storage Technology UEFI driver.\r\n"
                  );
      gBS->Stall (3000000);
      gST->ConOut->ClearScreen (gST->ConOut);

      VariableSize = sizeof (PCH_SETUP);
      Status = gRT->GetVariable (
                      L"PchSetup",
                      &gPchSetupVariableGuid,
                      NULL,
                      &VariableSize,
                      &PchSetup
                      );
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return;
      }

      if (PchSetup.SataRstForceForm) {
        //
        // Find FormBrowser2
        //
        Status = gBS->LocateProtocol (&gEfiFormBrowser2ProtocolGuid, NULL, (VOID **) &FormBrowser2);

        ActionRequest = EFI_BROWSER_ACTION_REQUEST_NONE;

        Status = FormBrowser2->SendForm (
                           FormBrowser2,
                           &FormHandle,
                           1,
                           NULL,
                           0,
                           NULL,
                           &ActionRequest
                           );

        if (ActionRequest == EFI_BROWSER_ACTION_REQUEST_RESET) {
          gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
        }
      }

    }
  }
  DEBUG ((DEBUG_INFO, "CheckRstStatusProtocol() End.\n"));
  return;
}

/**
  ReadyToBoot callback to check RST Status Protocol

  @param  Event   Pointer to this event
  @param  Context Event handler private data

  @retval None.
**/
VOID
EFIAPI
OnAllProtocolsInstalledRstStatusProtocolCallBack (
  IN  EFI_EVENT                 Event,
  IN  VOID                      *Context
  )
{
  CheckRstStatusProtocol ();
}

/**
  Creates an instance of EFI_RST_FEATURE_DATA.

  @retval EFI_STATUS  Returns status of SetVariable runtime service
**/
STATIC
EFI_STATUS
CreateRstFeaturesVariable (
  SA_SETUP  *SaSetup
  )
{
  EFI_RST_FEATURE_DATA  RstFeaturesVariable;
  EFI_RST_FEATURE_DATA  RstFeaturesVariableOrg;
  EFI_STATUS            Status;
  UINTN                 Size;

  ZeroMem (&RstFeaturesVariable, sizeof (EFI_RST_FEATURE_DATA));

  RstFeaturesVariable.RevisionId = EFI_RST_FEATURE_VARIABLE_REVISION_ID;

  RstFeaturesVariable.Raid0         = SaSetup->RaidR0;
  RstFeaturesVariable.Raid1         = SaSetup->RaidR1;
  RstFeaturesVariable.Raid5         = SaSetup->RaidR5;
  RstFeaturesVariable.Raid10        = SaSetup->RaidR10;

  Size = sizeof (RstFeaturesVariableOrg);

  Status = gRT->GetVariable (
                  EFI_RST_FEATURE_VARIABLE_NAME,
                  &gRstFeaturesVariableGuid,
                  NULL,
                  &Size,
                  &RstFeaturesVariableOrg
                  );

  if (Status != EFI_NOT_FOUND) {
    if (CompareMem (&RstFeaturesVariable, &RstFeaturesVariableOrg, sizeof (EFI_RST_FEATURE_DATA)) == 0) {
      return EFI_SUCCESS;
    }
  }

  DEBUG ((DEBUG_INFO, "CreateRstFeaturesVariable - Updating RST feature data\n"));
  return gRT->SetVariable (
                EFI_RST_FEATURE_VARIABLE_NAME,
                &gRstFeaturesVariableGuid,
                (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS),
                sizeof (EFI_RST_FEATURE_DATA),
                &RstFeaturesVariable
                );
}

/**
  Locks an instance of EFI_RST_FEATURE_DATA.
  Variable must exist.

  @retval EFI_STATUS Returns status of locking EFI_RST_FEATURE_DATA
**/
STATIC
EFI_STATUS
LockRstFeaturesVariable (
  VOID
  )
{
  EDKII_VARIABLE_POLICY_PROTOCOL  *VariablePolicy = NULL;
  EFI_STATUS  Status;

  Status = gBS->LocateProtocol (&gEdkiiVariablePolicyProtocolGuid, NULL, (VOID**)&VariablePolicy);
  DEBUG ((DEBUG_INFO, "Locate Variable Policy protocol - %r\n", Status));
  if (!EFI_ERROR (Status)) {
    Status = RegisterBasicVariablePolicy (VariablePolicy,
               &gRstFeaturesVariableGuid,
               EFI_RST_FEATURE_VARIABLE_NAME,
               VARIABLE_POLICY_NO_MIN_SIZE,
               VARIABLE_POLICY_NO_MAX_SIZE,
               VARIABLE_POLICY_NO_MUST_ATTR,
               VARIABLE_POLICY_NO_CANT_ATTR,
               VARIABLE_POLICY_TYPE_LOCK_NOW
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error: Failed to lock RstFeaturesVariable \n"));
      ASSERT_EFI_ERROR(Status);
    }
  }
  return Status;
}

/**
  Install EFI_RST_FEATURE_DATA variable

  @param[in] SaSetup     Sa Setup

  @retval EFI_STATUS  Return status of installing EFI_RST_FEATURE_DATA
**/
STATIC
EFI_STATUS
InstallRstFeaturesVariable (
  SA_SETUP   *SaSetup
  )
{
  EFI_STATUS   Status;

  Status = EFI_UNSUPPORTED;

  //
  // If VMD is enabled install RST features variable. Return EFI_UNSUPPORTED if not.
  //
  if (SaSetup->VmdEnable) {
    Status = CreateRstFeaturesVariable (SaSetup);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error: Failed to create RstFeaturesVariable\n"));
      return Status;
    } else {
      Status = LockRstFeaturesVariable ();

      if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "RstFeaturesVariable locked successfully\n"));
        return EFI_SUCCESS;
      }
    }
  }

  return Status;
}

#if (FixedPcdGetBool (PcdVmdEnable) == 1)
/**
  Creates an instance of EFI_VMD_OS_DATA.

  @retval EFI_STATUS  Returns status of SetVariable runtime service
**/

STATIC
EFI_STATUS
CreateVmdOsVariable (
  )
{
  EFI_VMD_OS_DATA  VmdOsDataVariable;
  EFI_STATUS            Status;
  UINTN                 Size;

  Size = sizeof (EFI_VMD_OS_DATA);

  Status = gRT->GetVariable (
                  EFI_VMD_OS_VARIABLE_NAME,
                  &gEfiVmdFeatureVariableGuid,
                  NULL,
                  &Size,
                  &VmdOsDataVariable
                  );

  if (Status == EFI_NOT_FOUND) {
    DEBUG ((DEBUG_INFO, "Create Vmd OS Variable\n"));
    ZeroMem (&VmdOsDataVariable, sizeof (EFI_VMD_OS_DATA));
  } else {
    DEBUG ((DEBUG_INFO, "Clearing Vmd OS Variable\n"));
    ZeroMem (&VmdOsDataVariable, sizeof (EFI_VMD_OS_DATA));
  }

  return gRT->SetVariable (
              EFI_VMD_OS_VARIABLE_NAME,
              &gEfiVmdFeatureVariableGuid,
              (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS),
              sizeof (EFI_VMD_OS_DATA),
              &VmdOsDataVariable
              );
}
#endif

/**
  Entry point for RST UEFI driver support driver. The point
  of this driver is to provide all dependencies for the RST
  driver.

  @param[in] ImageHandle  Handle for this image
  @param[in] SystemTable  Pointer to system table

  @return Status of the driver dispatch.
**/
EFI_STATUS
EFIAPI
RstUefiDriverSupportEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  PCH_SETUP   PchSetup;
  SA_SETUP    SaSetup;
  UINTN       Size;
  UINT32      PchSetupAttributes;
  UINT32      SaSetupAttributes;
  EFI_EVENT   Event;
  VOID        *Registration;

  Status = EFI_SUCCESS;

  Size = sizeof (SA_SETUP);
  Status = gRT->GetVariable (
                  L"SaSetup",
                  &gSaSetupVariableGuid,
                  &SaSetupAttributes,
                  &Size,
                  &SaSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: VMD Setup variable is not found \n"));
    return Status;
  }

  //
  // Install/Update Rst features Variable
  //
  InstallRstFeaturesVariable (&SaSetup);

#if (FixedPcdGetBool (PcdVmdEnable) == 1)
  //
  // Create Vmd OS Variable
  //
  CreateVmdOsVariable();
#endif

  Size = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                  L"PchSetup",
                  &gPchSetupVariableGuid,
                  &PchSetupAttributes,
                  &Size,
                  &PchSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to fetch PCH_SETUP\n"));
    return Status;
  }

  //
  // Create event to check Rst Status Protocol
  //
  gBS->CreateEvent (
        EVT_NOTIFY_SIGNAL,
        TPL_CALLBACK,
        OnAllProtocolsInstalledRstStatusProtocolCallBack,
        NULL,
        &Event
        );
  gBS->RegisterProtocolNotify (
        &gBdsAllDriversConnectedProtocolGuid,
        Event,
        &Registration
        );

  return Status;
}
