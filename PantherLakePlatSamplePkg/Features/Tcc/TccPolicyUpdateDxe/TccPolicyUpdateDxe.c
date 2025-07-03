/** @file
  Updates setup options when TCC mode is enabled

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


#include <PiDxe.h>
#include <SetupVariable.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <TccDataHob.h>
#include <Library/TccPolicyLib.h>
#include <Register/PmcRegs.h>
#include <Library/PmcLib.h>
#include <Library/IoLib.h>

/**
  Set the data for TCC platform policy from setup options.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context.
**/
VOID
EFIAPI
UpdateTccPolicyCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                     Status;
  CPU_SETUP                      CpuSetup;
  PCH_SETUP                      PchSetup;
  SA_SETUP                       SaSetup;
  SETUP_DATA                     SetupData;
  UINTN                          CpuSetupSize;
  UINTN                          PchSetupSize;
  UINTN                          SaSetupSize;
  UINTN                          SetupDataSize;
  UINT32                         CpuSetupAttr;
  UINT32                         PchSetupAttr;
  UINT32                         SaSetupAttr;
  UINT32                         SetupDataAttr;
  VOID                           *Interface;

  DEBUG ((DEBUG_INFO, "UpdateTccPolicyCallback\n"));

  Status = gBS->LocateProtocol (
                  &gEfiVariableWriteArchProtocolGuid,
                  NULL,
                  &Interface
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  CpuSetupSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  &CpuSetupAttr,
                  &CpuSetupSize,
                  &CpuSetup
                  );
  ASSERT_EFI_ERROR(Status);

  PchSetupSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                  L"PchSetup",
                  &gPchSetupVariableGuid,
                  &PchSetupAttr,
                  &PchSetupSize,
                  &PchSetup
                  );
  ASSERT_EFI_ERROR (Status);

  SaSetupSize = sizeof (SA_SETUP);
  Status = gRT->GetVariable (
                  L"SaSetup",
                  &gSaSetupVariableGuid,
                  &SaSetupAttr,
                  &SaSetupSize,
                  &SaSetup
                  );
  ASSERT_EFI_ERROR(Status);

  SetupDataSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  &SetupDataAttr,
                  &SetupDataSize,
                  &SetupData
                  );
  ASSERT_EFI_ERROR (Status);

  if (IsTccModeEnabled (&SetupData)) {
    GetTccModeConfigPolicy (&CpuSetup, &PchSetup, &SaSetup, &SetupData);
  }

  Status = gRT->SetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  CpuSetupAttr,
                  CpuSetupSize,
                  &CpuSetup
                  );
  ASSERT_EFI_ERROR(Status);

  Status = gRT->SetVariable (
                  L"PchSetup",
                  &gPchSetupVariableGuid,
                  PchSetupAttr,
                  PchSetupSize,
                  &PchSetup
                  );
  ASSERT_EFI_ERROR (Status);

  SaSetupSize = sizeof (SA_SETUP);
  Status = gRT->SetVariable (
                  L"SaSetup",
                  &gSaSetupVariableGuid,
                  SaSetupAttr,
                  SaSetupSize,
                  &SaSetup
                  );
  ASSERT_EFI_ERROR(Status);

  Status = gRT->SetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  SetupDataAttr,
                  SetupDataSize,
                  &SetupData
                  );
  ASSERT_EFI_ERROR (Status);
}

/**
  Constructor for the Real Time Tuning Lib

  @param  ImageHandle  ImageHandle of the loaded driver.
  @param  SystemTable  Pointer to the EFI System Table.

  @retval  EFI_SUCCESS            Register successfully.
  @retval  EFI_OUT_OF_RESOURCES   No enough memory to register this handler.
**/
EFI_STATUS
EFIAPI
TccPolicyUpdateEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  VOID          *Registration;

  DEBUG ((DEBUG_INFO, "TccPolicyUpdateEntry Start\n"));

  EfiCreateProtocolNotifyEvent (
    &gEfiVariableWriteArchProtocolGuid,
    TPL_CALLBACK,
    UpdateTccPolicyCallback,
    NULL,
    &Registration
    );

  DEBUG ((DEBUG_INFO, "TccPolicyUpdateEntry End\n"));

  return EFI_SUCCESS;
}
