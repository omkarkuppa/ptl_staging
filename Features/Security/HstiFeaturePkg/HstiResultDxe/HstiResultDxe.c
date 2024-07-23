/** @file
  This file is SampleCode for a DXE driver to allow for caching of HSTI
  test results to eliminate the need for testing on every boot.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include "HstiResultDxe.h"
#include <Library/VariablePolicyHelperLib.h>

DXE_SI_POLICY_PROTOCOL      *mSiPolicyData;
CONFIGURATION_CHANGE        mConfigChanged;

/**
  Handler to Save HSTI results for publishing on subsequent boots

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context

**/
VOID
EFIAPI
SaveConfigurationChange (
  EFI_EVENT                               Event,
  VOID                                    *Context
  )
{
  UINT32                        VarAttributes;
  UINTN                         VarSize;
  CONFIGURATION_CHANGE          ConfigChanged;
  VOID                          *Interface;
  EFI_STATUS                    Status;

  //
  // Check whether we are running for first time or actually when signaled
  //
  Status = gBS->LocateProtocol (&gPlatformConfigChangeProtocolGuid, NULL, &Interface);
  if (EFI_ERROR (Status)) {
    return;
  }

  ConfigChanged.ConfigChangeType = SETUP_OPTION_CHANGE;
  VarSize                        = sizeof(CONFIGURATION_CHANGE);
  VarAttributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;

  gRT->SetVariable (
            PLATFORM_CONFIGURATION_CHANGE,
            &gPlatformConfigChangeGuid,
            VarAttributes,
            VarSize,
            &ConfigChanged
            );
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

}

/**
  Handler to Save HSTI results for publishing on subsequent boots

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context
**/
VOID
EFIAPI
SaveResults (
  EFI_EVENT                               Event,
  VOID                                    *Context
  )
{

  UINT32                        VarAttributes;
  UINTN                         VarSize;
  EFI_STATUS                    Status;
  VOID                          *Interface;

  Status = EFI_SUCCESS;

  //
  // Check whether we are running for first time or actually when signaled
  //
  Status = gBS->LocateProtocol (&gHstiPublishCompleteProtocolGuid, NULL, &Interface);
  if (EFI_ERROR (Status)) {
    return;
  }

  if (mSiPolicyData->Hsti == NULL) {
    return;
  }
  if(( mConfigChanged.ConfigChangeType != NO_CONFIG_CHANGE)){

    VarAttributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;
    VarSize       = mSiPolicyData->HstiSize;

    gRT->SetVariable (
                  HSTI_STORAGE_NAME,
                  &gHstiStoredResultsGuid,
                  VarAttributes,
                  VarSize,
                  mSiPolicyData->Hsti
                  );
    //
    // Need to clear out the previous change so we can use cached results again
    //
    mConfigChanged.ConfigChangeType = NO_CONFIG_CHANGE;
    VarSize       = sizeof(CONFIGURATION_CHANGE);
    VarAttributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;

    Status = gRT->SetVariable (
                PLATFORM_CONFIGURATION_CHANGE,
                &gPlatformConfigChangeGuid,
                VarAttributes,
                VarSize,
                &mConfigChanged
                );
  }
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
}

/**
  The driver's entry point.

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS     The entry point is executed successfully.
  @retval other           Some error occurs when executing this entry point.
**/
EFI_STATUS
EFIAPI
DriverEntryPoint (
  IN    EFI_HANDLE                  ImageHandle,
  IN    EFI_SYSTEM_TABLE            *SystemTable
  )
{
  VOID                            *Registration;
  EFI_STATUS                      Status;
  UINT32                          VarAttributes;
  ADAPTER_INFO_PLATFORM_SECURITY  *Hsti;
  UINTN                           VarSize;
  EDKII_VARIABLE_POLICY_PROTOCOL  *VariablePolicy;
  MEMORY_PLATFORM_DATA_HOB        *MemoryPlatformData;
  EFI_HOB_GUID_TYPE               *GuidHob;

  MemoryPlatformData = NULL;
  Hsti = NULL;
  ZeroMem(&mConfigChanged,sizeof(CONFIGURATION_CHANGE));

  //
  // Locate DxeSiPolicyProtocolGuid protocol instance and assign it to a global variable
  //
  Status = gBS->LocateProtocol (&gDxeSiPolicyProtocolGuid, NULL, (VOID **) &mSiPolicyData);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR,"Failed to locate DxeSiPolicyProtocolGuid Protocol\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Register protocol notification function for saving results
  //
  EfiCreateProtocolNotifyEvent (
           &gHstiPublishCompleteProtocolGuid,
           TPL_NOTIFY,
           SaveResults,
           NULL,
           &Registration
           );

  //
  // Register protocol notification function for saving configuration change
  //
  EfiCreateProtocolNotifyEvent (
           &gPlatformConfigChangeProtocolGuid,
           TPL_NOTIFY,
           SaveConfigurationChange,
           NULL,
           &Registration
           );

  VarAttributes = 0;
  VarSize       = sizeof(CONFIGURATION_CHANGE);
  Status = gRT->GetVariable (
                    PLATFORM_CONFIGURATION_CHANGE,
                    &gPlatformConfigChangeGuid,
                    &VarAttributes,
                    &VarSize,
                    &mConfigChanged
                    );
  if (EFI_ERROR(Status)) {
    mConfigChanged.ConfigChangeType = FIRST_BOOT;
  }

  VarAttributes = 0;
  VarSize       = 0;
  Status = gRT->GetVariable (
                  HSTI_STORAGE_NAME,
                  &gHstiStoredResultsGuid,
                  &VarAttributes,
                  &VarSize,
                  Hsti
                  );
  if (EFI_ERROR(Status)) {
    Hsti   = (ADAPTER_INFO_PLATFORM_SECURITY *) AllocatePool(VarSize + 1);
    if (((UINTN) Hsti & BIT0) == 0){
      Hsti = (ADAPTER_INFO_PLATFORM_SECURITY*)((UINT8 *) Hsti + 1);
    }

    Status = gRT->GetVariable (
                    HSTI_STORAGE_NAME,
                    &gHstiStoredResultsGuid,
                    &VarAttributes,
                    &VarSize,
                    Hsti
                    );
  }

  GuidHob = NULL;
  MemoryPlatformData = NULL;
  GuidHob = GetFirstGuidHob (&gSiMemoryPlatformDataGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob != NULL) {
    MemoryPlatformData = (MEMORY_PLATFORM_DATA_HOB *) (GuidHob);
  }
  if (MemoryPlatformData != NULL) {
    DEBUG ((DEBUG_INFO, "Hsti MrcBootMode = %x\n", MemoryPlatformData->Data.BootMode));
  } else {
    DEBUG ((DEBUG_ERROR, "Hsti get gSiMemoryPlatformDataGuid HOB fails\n"));
  }

  //
  // Need to not use cached results if any setup option changed on previous boot or any major
  // HW change happened that is significant enough to trigger MRC retraining
  //
  if(!EFI_ERROR (Status) &&
     (mConfigChanged.ConfigChangeType == NO_CONFIG_CHANGE) &&
     ((MemoryPlatformData != NULL) && (MemoryPlatformData->Data.BootMode != bmCold))) {
    mSiPolicyData->Hsti = Hsti;
    mSiPolicyData->HstiSize = VarSize;

    //
    // Mark the read-only variables if the Variable Lock protocol exists
    //
    Status = gBS->LocateProtocol (&gEdkiiVariablePolicyProtocolGuid, NULL, (VOID**)&VariablePolicy);
    DEBUG ((DEBUG_INFO, "Locate Variable Policy protocol - %r\n", Status));
    if (!EFI_ERROR (Status)) {
      Status = RegisterBasicVariablePolicy (VariablePolicy,
                 &gHstiStoredResultsGuid,
                 HSTI_STORAGE_NAME,
                 VARIABLE_POLICY_NO_MIN_SIZE,
                 VARIABLE_POLICY_NO_MAX_SIZE,
                 VARIABLE_POLICY_NO_MUST_ATTR,
                 VARIABLE_POLICY_NO_CANT_ATTR,
                 VARIABLE_POLICY_TYPE_LOCK_NOW
                 );

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "HSTI cached results were unable to be locked: Status = %r\n", Status));
        return Status;
      }
    }
  }
  return EFI_SUCCESS;
}
