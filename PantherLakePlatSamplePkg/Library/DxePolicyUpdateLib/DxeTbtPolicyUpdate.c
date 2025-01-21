/** @file
  This file is the library for TBT DXE Policy initialization.

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
#include "DxeTbtPolicyUpdate.h"
#include <Library/CmosAccessLib.h>
#include <CmosMap.h>
#include <Library/Usb4CmMode.h>
#include <MeSetup.h>
#include <MeBiosPayloadHob.h>
#if FixedPcdGetBool (PcdDTbtEnable) == 1
#include <Library/DxeDTbtPolicyLib.h>
#endif

/**
  Get data for platform policy from setup options.

  @param[in] ITbtPolicy                The pointer to get ITBT Policy protocol instance

  @retval EFI_SUCCESS                  Operation success.

**/
EFI_STATUS
EFIAPI
UpdateDxeTbtPolicy (
  IN OUT  ITBT_POLICY_PROTOCOL   *ITbtPolicy
  )
{
  EFI_STATUS                     Status;
  UINTN                          DataSize;
  SETUP_DATA                     SetupData;
  ME_SETUP                       MeSetup;
  ME_BIOS_PAYLOAD_HOB            *MbpHob;
  BOOLEAN                        VproTbtDockSupport;
  SA_SETUP                       SaSetup;
  UINT8                          Index;
  DXE_ITBT_CONFIG                *DxeITbtConfig;
  UINT8                          CmosUsb4CmModeValue;
#if FixedPcdGetBool (PcdDTbtEnable) == 1
  DXE_DTBT_POLICY_PROTOCOL       *DxeDTbtConfig;
  DxeDTbtConfig = NULL;
#endif

  DxeITbtConfig = NULL;
  Status = EFI_NOT_FOUND;
  MbpHob = NULL;
  VproTbtDockSupport = FALSE;
  DEBUG ((DEBUG_INFO, "UpdateTbtPolicyCallback\n"));

  DataSize = sizeof (SetupData);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &DataSize,
                  &SetupData
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DataSize = sizeof (ME_SETUP);
  Status = gRT->GetVariable (
                  L"MeSetup",
                  &gMeSetupVariableGuid,
                  NULL,
                  &DataSize,
                  &MeSetup
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DataSize = sizeof (SA_SETUP);
  Status = gRT->GetVariable (
                  L"SaSetup",
                  &gSaSetupVariableGuid,
                  NULL,
                  &DataSize,
                  &SaSetup
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  if (ITbtPolicy != NULL) {
    Status = GetConfigBlock ((VOID *)ITbtPolicy, &gDxeITbtConfigGuid, (VOID *)&DxeITbtConfig);
  }

  if (DxeITbtConfig == NULL) {
    DEBUG ((DEBUG_ERROR, " DxeITbtConfig is not created!!!\n"));
  } else {
    for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
      DxeITbtConfig->ITbtResourceConfig[Index].TbtPcieExtraBusRsvd   = SetupData.ITbtPcieExtraBusRsvd[Index];
      DxeITbtConfig->ITbtResourceConfig[Index].TbtPcieMemRsvd        = SetupData.ITbtPcieMemRsvd[Index];
      DxeITbtConfig->ITbtResourceConfig[Index].TbtPcieMemAddrRngMax  = SetupData.ITbtPcieMemAddrRngMax[Index];
      DxeITbtConfig->ITbtResourceConfig[Index].TbtPciePMemRsvd       = SetupData.ITbtPciePMemRsvd[Index];
      DxeITbtConfig->ITbtResourceConfig[Index].TbtPciePMemAddrRngMax = SetupData.ITbtPciePMemAddrRngMax[Index];
    }

    CmosUsb4CmModeValue = CmosRead8 (CMOS_USB4_CM_MODE_REG);
    if ((SetupData.Usb4CmMode == USB4_SETUP_CM_MODE_FW) ||
        ((SetupData.Usb4CmMode == USB4_SETUP_CM_MODE_OS) && (CmosUsb4CmModeValue == USB4_CM_MODE_FW_CM))) {
      DxeITbtConfig->ITbtGenericConfig.ITbtRtd3 = SetupData.ITbtRtd3;
    } else {
      DxeITbtConfig->ITbtGenericConfig.ITbtRtd3 = 1;
    }

    DxeITbtConfig->ITbtGenericConfig.ITbtRtd3ExitDelay  = SetupData.ITbtRtd3ExitDelay;
    DxeITbtConfig->OsNativeResourceBalance              = SetupData.OsNativeResourceBalance;
    DxeITbtConfig->PcieRtd3LinkActiveTimeout            = SetupData.PcieRtd3LinkActiveTimeout;
    //
    // Get Mbp Data HOB
    //
    MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
    if (MbpHob != NULL) {
      //
      // If FwCapSku is available, then check for capabilities field whether
      // TBT Dock is enabled or not and update TBT Dock support accordingly
      //
      if (MbpHob->MeBiosPayload.FwCapsSku.Available == TRUE) {
        if (MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.TbtDock == 1) {
          DEBUG ((DEBUG_INFO, "%a: Intel Brand Identity Enabled TBT Dock\n", __FUNCTION__));
          VproTbtDockSupport = TRUE;
        }
      }
    }
    DxeITbtConfig->ITbtGenericConfig.ITbtVproDockSupport = (UINT8) ((MeSetup.vProTbtDock != 0)  && VproTbtDockSupport);
  }

#if FixedPcdGetBool (PcdDTbtEnable) == 1
  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  gBS->LocateProtocol (
       &gDxeDTbtPolicyProtocolGuid,
       NULL,
       (VOID **) &DxeDTbtConfig
       );

  if (DxeDTbtConfig == NULL) {
    DEBUG ((DEBUG_ERROR, " DxeDTbtConfig is not created!!!\n"));
  } else {
    for (Index = 0; Index < PcdGet8 (PcdBoardDTbtControllerNumber); Index++) {
      DxeDTbtConfig->DTbtResourceConfig[Index].TbtPcieExtraBusRsvd   = SetupData.DTbtPcieExtraBusRsvd[Index];
      DxeDTbtConfig->DTbtResourceConfig[Index].TbtPcieMemRsvd        = SetupData.DTbtPcieMemRsvd[Index];
      DxeDTbtConfig->DTbtResourceConfig[Index].TbtPcieMemAddrRngMax  = SetupData.DTbtPcieMemAddrRngMax[Index];
      DxeDTbtConfig->DTbtResourceConfig[Index].TbtPciePMemRsvd       = SetupData.DTbtPciePMemRsvd[Index];
      DxeDTbtConfig->DTbtResourceConfig[Index].TbtPciePMemAddrRngMax = SetupData.DTbtPciePMemAddrRngMax[Index];
    }
    DxeDTbtConfig->TbtCommonConfig.TbtVtdBaseSecurity    = (SetupData.DisableTbtPcieTreeBme && SaSetup.EnableVtd);
    DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3            = SetupData.DTbtRtd3;
    DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3OffDelay    = SetupData.DTbtRtd3OffDelay;
    DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3ClkReq      = SetupData.DTbtRtd3ClkReq;
    DxeDTbtConfig->DTbtGenericConfig.DTbtRtd3ClkReqDelay = SetupData.DTbtRtd3ClkReqDelay;
  }
  DTbtPrintDxePolicyConfig();
#endif

  return EFI_SUCCESS;
}

