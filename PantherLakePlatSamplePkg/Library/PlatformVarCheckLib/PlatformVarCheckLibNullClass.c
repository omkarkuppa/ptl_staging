/** @file
  Platform variable check implementation.

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

  @par Specification Reference:
**/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/VarCheckLib.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmExitBootServices.h>
#include <Protocol/Variable.h>
#include <Protocol/SmmVariable.h>
#include <Guid/EventGroup.h>
#include <Guid/AuthenticatedVariableFormat.h>

#include <SetupVariable.h>

typedef struct {
  EFI_GUID                      *Guid;
  CHAR16                        *Name;
} INTERNAL_SETUP_VARIABLE_ENTRY;

//
// Setup related variables.
//
INTERNAL_SETUP_VARIABLE_ENTRY mSetupVariableList[] = {
  //
  // gSetupVariableGuid
  //
  {&gSetupVariableGuid, L"Setup"},
  {&gSetupVariableGuid, L"SetupVolatileData"},
  {&gSetupVariableGuid, L"BootTime"},
  {&gSetupVariableGuid, L"InitSetupVariable"},
  {&gSetupVariableGuid, L"PciBusSetup"},
  {&gSetupVariableGuid, L"ComAttributes"},
  {&gSetupVariableGuid, L"SetupCpuFeatures"},
  {&gSetupVariableGuid, L"UsbMassDevNum"},
  {&gSetupVariableGuid, L"UsbSupport"},
  //
  // gBoardInfoVariableGuid
  //
  {&gBoardInfoVariableGuid, L"BoardInfoSetup"},
  //
  // gSiSetupVariableGuid
  //
  {&gSiSetupVariableGuid, L"SiSetup"},
  //
  // gSaSetupVariableGuid
  //
  {&gSaSetupVariableGuid, L"SaSetup"},
  //
  //
  // gMeSetupVariableGuid
  //
  {&gMeSetupVariableGuid, L"MeSetup"},
  {&gMeSetupVariableGuid, L"MeSetupStorage"},
  //
  // gCpuSetupVariableGuid
  //
  {&gCpuSetupVariableGuid, L"CpuSetup"},
  {&gCpuSetupVariableGuid, L"CpuSetupVolatileData"},
  //
  // gPchSetupVariableGuid
  //
  {&gPchSetupVariableGuid, L"PchSetup"},
  //
  // gOsProfileGuid
  //
  {&gOsProfileGuid, L"OsProfile"},
  //
  // gTcg2ConfigFormSetGuid
  //
  {&gTcg2ConfigFormSetGuid, L"TCG2_CONFIGURATION"},
  //
  // gEfiSecureBootEnableDisableGuid
  //
  {&gEfiSecureBootEnableDisableGuid, L"SecureBootEnable"},
  //
  // gSystemAccessGuid
  //
  {&gSystemAccessGuid, L"PasswordStatus"},
  //
  // gLpeSetupVariableGuid
  //
  {&gLpeSetupVariableGuid, L"LpeSetup"},
#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
  //
  // gDptfConfigVariableGuid
  //
  {&gDptfConfigVariableGuid, L"DptfConfig"},
#endif
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  //
  //
  // gOcSetupVariableGuid
  //
  {&gOcSetupVariableGuid, L"OcSetup"},
#endif
};

EFI_SMM_SYSTEM_TABLE2 *mPlatformVarCheckSmst = NULL;
BOOLEAN mPlatformVarCheckAtRuntime           = FALSE;

/**
  SetVariable check handler platform.

  @param[in] VariableName       Name of Variable to set.
  @param[in] VendorGuid         Variable vendor GUID.
  @param[in] Attributes         Attribute value of the variable.
  @param[in] DataSize           Size of Data to set.
  @param[in] Data               Data pointer.

  @retval EFI_SUCCESS           The SetVariable check result was success.
  @retval EFI_WRITE_PROTECTED   Write protected to runtime.

**/
EFI_STATUS
EFIAPI
SetVariableCheckHandlerPlatform (
  IN CHAR16     *VariableName,
  IN EFI_GUID   *VendorGuid,
  IN UINT32     Attributes,
  IN UINTN      DataSize,
  IN VOID       *Data
  )
{
  UINTN                             Index;
  INTERNAL_SETUP_VARIABLE_ENTRY     *SetupVariableEntry;

  if (!mPlatformVarCheckAtRuntime) {
    return EFI_SUCCESS;
  }

  for (Index = 0; Index < sizeof (mSetupVariableList) / sizeof (mSetupVariableList[0]); Index++) {
    SetupVariableEntry = &mSetupVariableList[Index];
    if (CompareGuid (VendorGuid, SetupVariableEntry->Guid) && (StrCmp (VariableName, SetupVariableEntry->Name) == 0)) {
      DEBUG ((EFI_D_INFO, "Variable write protected at runtime - %g:%s\n", VendorGuid, VariableName));
      return EFI_WRITE_PROTECTED;
    }
  }

  // Not found, so pass.
  return EFI_SUCCESS;
}

/**
  SMM Exit Boot Services Event notification handler.

  @param[in] Protocol   Points to the protocol's unique identifier
  @param[in] Interface  Points to the interface instance
  @param[in] Handle     The handle on which the interface was installed

  @retval EFI_SUCCESS PlatformVarCheckAtRuntimeNotifySmm runs successfully

**/
EFI_STATUS
EFIAPI
PlatformVarCheckAtRuntimeNotifySmm (
  IN CONST EFI_GUID     *Protocol,
  IN VOID               *Interface,
  IN EFI_HANDLE         Handle
  )
{
  mPlatformVarCheckAtRuntime = TRUE;
  return EFI_SUCCESS;
}

/**
  Exit Boot Services Event notification handler.

  @param[in] Event      Event whose notification function is being invoked.
  @param[in] Context    Pointer to the notification function's context.

**/
VOID
EFIAPI
PlatformVarCheckAtRuntimeNotifyDxe (
  IN EFI_EVENT          Event,
  IN VOID               *Context
  )
{
  mPlatformVarCheckAtRuntime = TRUE;
}

/**
  Check if it is in SMM.

  @return TRUE if it is in SMM, or FALSE if it is not in SMM.

**/
BOOLEAN
PlatformVarCheckIsInSmm (
  VOID
  )
{
  EFI_STATUS                     Status;
  EFI_SMM_BASE2_PROTOCOL         *SmmBase2;
  BOOLEAN                        InSmm;

  //
  // Get InSmm.
  //
  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID**) &SmmBase2);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }
  Status = SmmBase2->InSmm (SmmBase2, &InSmm);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }
  if (!InSmm) {
    return FALSE;
  }
  //
  // Good, we are in SMM
  //
  Status = SmmBase2->GetSmstLocation (SmmBase2, &mPlatformVarCheckSmst);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }
  return TRUE;
}

/**
  Constructor function of PlatformVarCheckLib.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @return Status returned from SmmRegisterProtocolNotify() or CreateEventEx().

**/
EFI_STATUS
EFIAPI
PlatformVarCheckLibNullClassConstructor (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                Status;
  VOID                      *Registration;
  EFI_EVENT                 Event;

  VarCheckLibRegisterSetVariableCheckHandler (SetVariableCheckHandlerPlatform);

  if (PlatformVarCheckIsInSmm ()) {
    //
    // Register SmmExitBootServices notification.
    //
    Registration = NULL;
    Status = mPlatformVarCheckSmst->SmmRegisterProtocolNotify (
                                      &gEdkiiSmmExitBootServicesProtocolGuid,
                                      PlatformVarCheckAtRuntimeNotifySmm,
                                      &Registration
                                      );
    ASSERT_EFI_ERROR (Status);
  } else {
    //
    // Register ExitBootServices notification.
    //
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_NOTIFY,
                    PlatformVarCheckAtRuntimeNotifyDxe,
                    NULL,
                    &gEfiEventExitBootServicesGuid,
                    &Event
                    );
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}
