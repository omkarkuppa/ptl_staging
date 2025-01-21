/** @file
  ITBT Variable Update Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include <Library/DxeItbtVariableUpdateLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/CmosAccessLib.h>
#include <SetupVariable.h>
#include <ITbtInfoHob.h>
#include <CmosMap.h>
#include <Library/Usb4CmMode.h>

#if FixedPcdGetBool (PcdTcssSupport) == 1
/**
  Update Setup variable for keep DMA UUID
**/
VOID
UpdateItbtDmaUuidVariable (
  VOID
  )
{
  UINTN                         VarSize;
  UINT32                        VariableAttr;
  ITBT_INFO_HOB                 *ITbtInfoHobPtr;
  SETUP_DATA                    SystemConfiguration;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_INFO, "UpdateItbtDmaUuidVariable - Start\n"));
  //
  // Get Setup Data for update ITBT DMA UUID
  //
  VarSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  PLATFORM_SETUP_VARIABLE_NAME,
                  &gSetupVariableGuid,
                  &VariableAttr,
                  &VarSize,
                  &SystemConfiguration
                  );
  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR(Status)){
    DEBUG ((DEBUG_ERROR, "Platform Setup data was not found.\n"));
    return;
  }

  ///
  /// Get ITBT DMA UUID value from HOB of gTcssHobGuid
  ///
  ITbtInfoHobPtr = (ITBT_INFO_HOB *) GetFirstGuidHob (&gITbtInfoHobGuid);

  if (ITbtInfoHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "TcssHobPtr is NULL.\n"));
    return;
  }

  ///
  /// Set ITBT DMA UUID to Setup Data variable with updated values
  ///
  Status = gRT->SetVariable (
                  PLATFORM_SETUP_VARIABLE_NAME,
                  &gSetupVariableGuid,
                  VariableAttr,
                  VarSize,
                  &SystemConfiguration
                  );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "UpdateItbtDmaUuidVariable - End\n"));
}
#endif

/**
  Sync ITBT Variables
**/
VOID
SyncItbtVariables (
  VOID
  )
{
  SA_SETUP                      SaSetup;
  UINTN                         SaSetupVariableSize;
  UINT32                        SaVariableAttributes;
  EFI_STATUS                    Status;
  SETUP_DATA                    SetupData;
  UINTN                         SetupDataVariableSize;
  UINT32                        SetupDataVariableAttributes;
  UINT8                         CmosUsb4CmModeValue;
  UINT8                         EnableSyncSetupData;
  UINT8                         TempITbtRtd3;

  DEBUG ((DEBUG_INFO, "SyncItbtVariables Entry\n"));

  EnableSyncSetupData = 0;

  SetupDataVariableAttributes = 0;
  SetupDataVariableSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  PLATFORM_SETUP_VARIABLE_NAME,
                  &gSetupVariableGuid,
                  &SetupDataVariableAttributes,
                  &SetupDataVariableSize,
                  &SetupData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get Setup variable fail.\n"));
    return;
  }

  SaVariableAttributes = 0;
  SaSetupVariableSize = sizeof (SA_SETUP);
  Status = gRT->GetVariable (
                  L"SaSetup",
                  &gSaSetupVariableGuid,
                  &SaVariableAttributes,
                  &SaSetupVariableSize,
                  &SaSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get SaSetup variable fail.\n"));
    return;
  }

  CmosUsb4CmModeValue = CmosRead8 (CMOS_USB4_CM_MODE_REG);
  if ((SetupData.Usb4CmMode == USB4_SETUP_CM_MODE_FW) ||
      ((SetupData.Usb4CmMode == USB4_SETUP_CM_MODE_OS) && (CmosUsb4CmModeValue == USB4_CM_MODE_FW_CM))) {
    TempITbtRtd3 = SetupData.ITbtRtd3;
  } else {
    TempITbtRtd3 = 1;
  }

  if (SetupData.ITbtRtd3 != TempITbtRtd3) {
    DEBUG ((DEBUG_INFO, "Sync ITbtRtd3 Variables\n"));
    SetupData.ITbtRtd3 = TempITbtRtd3;
    EnableSyncSetupData = 1;
  }

  if (EnableSyncSetupData == 1) {
    Status = gRT->SetVariable (
                    PLATFORM_SETUP_VARIABLE_NAME,
                    &gSetupVariableGuid,
                    SetupDataVariableAttributes,
                    SetupDataVariableSize,
                    &SetupData);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Save SetupData variables fail.\n"));
    }
  }

  //
  // Wake Capability is S3
  //
  if (SetupData.DeepestUSBSleepWakeCapability == 3) {
    if (SaSetup.TcssVccstStatus != 0) {
      DEBUG ((DEBUG_INFO, "Sync TcssIomVccSt Variables\n"));
      SaSetup.TcssVccstStatus = 0;
      Status = gRT->SetVariable (
                      L"SaSetup",
                      &gSaSetupVariableGuid,
                      SaVariableAttributes,
                      SaSetupVariableSize,
                      &SaSetup
                      );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Save SaSetup variables fail.\n"));
      }
    }
  }

  DEBUG ((DEBUG_INFO, "SyncItbtVariables Exit\n"));
}
