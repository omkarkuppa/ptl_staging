/** @file
  Connectivity Setup Routines

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#include "SetupPrivate.h"
#include <Library/CnviLib.h>
#include <NhltEndpointsConfigurationVariable.h>
#include <SndwDevTopologyConfigurationVariable.h>
#include <CnvVfrSetupMenuHii.h>
#include <CnvSetupId.h>
#include <Protocol/CnvFormPlatformProtocol.h>

UINT8 mCnvFormChanged = 0;

/**
  Platform CNV setup callback executed when user selects a knob from the CNV browser UI.
  Changes visibility and settings of other options.

  @param[in] Action          Specifies the type of action taken by the browser.
  @param[in] KeyValue        A unique value which is sent to the original exporting driver
                             so that it can identify the type of data to expect.
  @param[in] Value           A pointer to the data being sent to the original exporting driver.
  @retval EFI_SUCCESS        The callback successfully handled the action
  @retval EFI_NOT_FOUND      Failed to retrive protocol/data required for execution
  @retval EFI_UNSUPPORTED    The specified action is not supported by the callback
**/
EFI_STATUS
EFIAPI
CnvFormPlatformCallback (
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN EFI_IFR_TYPE_VALUE                   *Value
  )
{
  EFI_STATUS                                   Status;
  BOOLEAN                                      PchNeedUpdate;
  BOOLEAN                                      NhltNeedUpdate;
  BOOLEAN                                      SndwDevTopologyNeedUpdate;
  UINT32                                       PchVariableAttr;
  UINTN                                        PchVariableVarSize;
  PCH_SETUP                                    PchSetup;
  UINT32                                       NhltConfigurationVariableAttr;
  UINTN                                        NhltConfigurationVariableSize;
  NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE  NhltConfiguration;
  SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE     SndwDevTopologyConfigurationVariable;
  UINT32                                       SndwDevTopologyConfigurationVariableAttr;
  UINTN                                        SndwDevTopologyConfigurationVariableSize;
  CNV_VFR_CONFIG_SETUP                         CnvSetupData;
  UINTN                                        CnvSetupVarSize;

  DEBUG ((DEBUG_INFO, "%a Entry\n", __FUNCTION__));

  Status = EFI_SUCCESS;
  PchNeedUpdate = FALSE;
  NhltNeedUpdate = FALSE;
  SndwDevTopologyNeedUpdate = FALSE;
  PchVariableAttr = 0;
  PchVariableVarSize = sizeof (PCH_SETUP);
  CnvSetupVarSize = sizeof (CNV_VFR_CONFIG_SETUP);
  NhltConfigurationVariableAttr = 0;
  NhltConfigurationVariableSize = sizeof (NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE);
  SndwDevTopologyConfigurationVariableAttr = 0;
  SndwDevTopologyConfigurationVariableSize = sizeof (SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE);
  ZeroMem (&PchSetup, PchVariableVarSize);
  ZeroMem (&CnvSetupData, CnvSetupVarSize);
  ZeroMem (&NhltConfiguration, NhltConfigurationVariableSize);
  ZeroMem (&SndwDevTopologyConfigurationVariable, SndwDevTopologyConfigurationVariableSize);

  if (KeyValue == KEY_CNV_BT_INTERFACE) {
    //
    // Check Bt Interace, and override default vaules.
    //
    if (Action >= EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
      DEBUG ((DEBUG_INFO, "Action: Default , KeyValue: KEY_CNV_BT_INTERFACE\n"));

      if (PcdGetBool (PcdCnviCrfBtUsbOnly) ) {
        DEBUG ((DEBUG_INFO, "Override Bt Interace default to 1\n"));
        Value->u8 = 0x01;
      } else {
        DEBUG ((DEBUG_INFO, "Override Bt Interace default to 2\n"));
        Value->u8 = 0x02;
      }
      return Status;
    }
  }

  if (KeyValue == KEY_CNV_BT_AUDIO_OFFLOAD || KeyValue == KEY_CNV_BT_AUDIO_OFFLOAD_INTERFACE ||
      KeyValue == PCH_HDAUDIO_DBT_OFFLOAD_ACTION_KEY || KeyValue == KEY_CNV_BT_CNVI_MODE) {

    // Inform DSP features submenu about the change
    if (Action == EFI_BROWSER_ACTION_CHANGED) {
      mCnvFormChanged = 1;
    }
    //
    // Update NHLT and PCH setup variables accordingly when the changes in CNV setup are saved.
    //
    if (Action != EFI_BROWSER_ACTION_SUBMITTED) {
      return EFI_UNSUPPORTED;
    }
    mCnvFormChanged = 0;

    Status = gRT->GetVariable (
                    L"CnvSetup",
                    &gCnvFeatureSetupGuid,
                    NULL,
                    &CnvSetupVarSize,
                    &CnvSetupData
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = gRT->GetVariable (
                    L"PchSetup",
                    &gPchSetupVariableGuid,
                    &PchVariableAttr,
                    &PchVariableVarSize,
                    &PchSetup
                    );
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }

    Status = gRT->GetVariable (
                    NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_NAME,
                    &gNhltEndpointsTableConfigurationVariableGuid,
                    &NhltConfigurationVariableAttr,
                    &NhltConfigurationVariableSize,
                    &NhltConfiguration
                    );
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }
    Status = gRT->GetVariable (
                    SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE_NAME,
                    &gSndwDevTopologyConfigurationVariableGuid,
                    &SndwDevTopologyConfigurationVariableAttr,
                    &SndwDevTopologyConfigurationVariableSize,
                    &SndwDevTopologyConfigurationVariable
                  );
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }

    // Make the changes when a integrated/discrete BT module is present and enabled
    if ((PchSetup.HdaDiscBtOffEnabled ||
       (CnviCrfModuleIsPresent () && CnvSetupData.CnviMode != 0)) &&
       CnvSetupData.CnviBtAudioOffload) {
      // BT Sideband support
      if (PchSetup.PchHdAudioFeature[1] != TRUE) {
        PchSetup.PchHdAudioFeature[1] = TRUE;
        PchNeedUpdate = TRUE;
      }
      // BT Intel HFP SCO
      if (PchSetup.PchHdAudioFeature[5] != TRUE) {
        PchSetup.PchHdAudioFeature[5] = TRUE;
        PchNeedUpdate = TRUE;
      }
      // BT Intel A2DP
      if (PchSetup.PchHdAudioFeature[6] != TRUE) {
        PchSetup.PchHdAudioFeature[6] = TRUE;
        PchNeedUpdate = TRUE;
      }
      // BT Intel LE
      if (PchSetup.PchHdAudioFeature[9] != TRUE) {
        PchSetup.PchHdAudioFeature[9] = TRUE;
        PchNeedUpdate = TRUE;
      }
      // SoundWire interface enable
      if (CnvSetupData.CnviBtAudioOffloadInterface == 1) {
        // Audio DSP NHLT Endpoints Configuration - Bluetooth
        if (NhltConfiguration.NhltBluetoothEnabled != FALSE) {
          NhltConfiguration.NhltBluetoothEnabled = FALSE;
          NhltNeedUpdate = TRUE;
        }
        // BT Offload through SoundWire
        if (SndwDevTopologyConfigurationVariable.AcxSndwDevTopologyBtNdaiEnabled != TRUE) {
          SndwDevTopologyConfigurationVariable.AcxSndwDevTopologyBtNdaiEnabled = TRUE;
          SndwDevTopologyNeedUpdate = TRUE;
        }
        // On-die connected SoundWire controller instance
        if (PchSetup.PchHdAudioSndwLinkEnable[4] != TRUE) {
          PchSetup.PchHdAudioSndwLinkEnable[4] = TRUE;
          PchNeedUpdate = TRUE;
        }
      } else { //I2S (legacy) interface is used
        // Audio DSP NHLT Endpoints Configuration - Bluetooth
        if (NhltConfiguration.NhltBluetoothEnabled != TRUE) {
          NhltConfiguration.NhltBluetoothEnabled = TRUE;
          NhltNeedUpdate = TRUE;
        }
        // BT Offload through SoundWire
        if (SndwDevTopologyConfigurationVariable.AcxSndwDevTopologyBtNdaiEnabled != FALSE) {
          SndwDevTopologyConfigurationVariable.AcxSndwDevTopologyBtNdaiEnabled = FALSE;
          SndwDevTopologyNeedUpdate = TRUE;
        }
        // On-die connected SoundWire controller instance
        if (PchSetup.PchHdAudioSndwLinkEnable[4] != FALSE) {
          PchSetup.PchHdAudioSndwLinkEnable[4] = FALSE;
          PchNeedUpdate = TRUE;
        }
      }
    } else { //BT not present or enabled or BT offload disabled
      // Audio DSP NHLT Endpoints Configuration - Bluetooth
      if (NhltConfiguration.NhltBluetoothEnabled != FALSE) {
        NhltConfiguration.NhltBluetoothEnabled = FALSE;
        NhltNeedUpdate = TRUE;
      }
      // BT Offload through SoundWire
      if (SndwDevTopologyConfigurationVariable.AcxSndwDevTopologyBtNdaiEnabled != FALSE) {
        SndwDevTopologyConfigurationVariable.AcxSndwDevTopologyBtNdaiEnabled = FALSE;
        SndwDevTopologyNeedUpdate = TRUE;
      }
      // 5th on-die connected SoundWire controller instance
      if (PchSetup.PchHdAudioSndwLinkEnable[4] != FALSE) {
        PchSetup.PchHdAudioSndwLinkEnable[4] = FALSE;
        PchNeedUpdate = TRUE;
      }
      // BT Sideband support
      if (PchSetup.PchHdAudioFeature[1] != FALSE) {
        PchSetup.PchHdAudioFeature[1] = FALSE;
        PchNeedUpdate = TRUE;
      }
      // BT Intel HFP SCO
      if (PchSetup.PchHdAudioFeature[5] != FALSE) {
        PchSetup.PchHdAudioFeature[5] = FALSE;
        PchNeedUpdate = TRUE;
      }
      // BT Intel A2DP
      if (PchSetup.PchHdAudioFeature[6] != FALSE) {
        PchSetup.PchHdAudioFeature[6] = FALSE;
        PchNeedUpdate = TRUE;
      }
      // BT Intel LE
      if (PchSetup.PchHdAudioFeature[9] != FALSE) {
        PchSetup.PchHdAudioFeature[9] = FALSE;
        PchNeedUpdate = TRUE;
      }
    }

    if (PchNeedUpdate) {
      Status = gRT->SetVariable (
                      L"PchSetup",
                      &gPchSetupVariableGuid,
                      PchVariableAttr,
                      PchVariableVarSize,
                      &PchSetup
                      );
      if (EFI_ERROR (Status)) {
        return EFI_NOT_FOUND;
      }
    }
    if (NhltNeedUpdate) {
      Status = gRT->SetVariable (
                      NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_NAME,
                      &gNhltEndpointsTableConfigurationVariableGuid,
                      NhltConfigurationVariableAttr,
                      NhltConfigurationVariableSize,
                      &NhltConfiguration
                      );
      if (EFI_ERROR (Status)) {
        return EFI_NOT_FOUND;
      }
    }
    if (SndwDevTopologyNeedUpdate) {
      Status = gRT->SetVariable (
                      SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE_NAME,
                      &gSndwDevTopologyConfigurationVariableGuid,
                      SndwDevTopologyConfigurationVariableAttr,
                      SndwDevTopologyConfigurationVariableSize,
                      &SndwDevTopologyConfigurationVariable
                      );
      if (EFI_ERROR (Status)) {
        return EFI_NOT_FOUND;
      }
    }
  }

  return Status;
}

/**
  Callback executed when user enters the DSP Features submenu of HD-Configuration form.
  Changes visibility and settings of other options according to the changes in CNV setup.

  @param[in] Action             Specifies the type of action taken by the browser.

  @retval EFI_SUCCESS           The callback successfully handled the action
  @retval EFI_UNSUPPORTED       The specified action is not supported by the callback
  @retval EFI_NOT_FOUND         Failed to retrive protocol/data required for execution
  @retval EFI_OUT_OF_RESOURCES  Failed to allocate resources.
**/
EFI_STATUS
DspFeaturesCnvSetupCallback (
  IN EFI_BROWSER_ACTION         Action
  )
{
  PCH_SETUP                                    PchSetup;
  UINTN                                        PchVarSize;
  EFI_STRING                                   RequestString;
  EFI_STRING                                   TmpString;
  UINTN                                        NhltConfigurationVariableSize;
  NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE  NhltConfiguration;
  CNV_VFR_CONFIG_SETUP                         CnvSetupData;
  UINTN                                        CnvSetupVarSize;
  EFI_STATUS                                   Status;

  DEBUG ((DEBUG_INFO, "%a () enter, Action = 0x%x\n", __FUNCTION__, Action));

  PchVarSize      = sizeof (PCH_SETUP);
  CnvSetupVarSize = sizeof (CNV_VFR_CONFIG_SETUP);
  ZeroMem (&CnvSetupData, CnvSetupVarSize);

  // Pull the correct data depending on whether the changes in the CNV setup are saved or not
  if (mCnvFormChanged) {
    if (!HiiGetBrowserData (&gCnvFeatureSetupGuid, CNV_SETUP_VARIABLE_NAME, CnvSetupVarSize, (UINT8 *) &CnvSetupData)) {
      return EFI_NOT_FOUND;
    }
  } else {
    CnvSetupVarSize = sizeof (CNV_VFR_CONFIG_SETUP);
    Status = gRT->GetVariable (
                    L"CnvSetup",
                    &gCnvFeatureSetupGuid,
                    NULL,
                    &CnvSetupVarSize,
                    &CnvSetupData
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  if (!HiiGetBrowserData (&gPchSetupVariableGuid, L"PchSetup", PchVarSize, (UINT8 *) &PchSetup)) {
    return EFI_NOT_FOUND;
  }

  NhltConfigurationVariableSize = sizeof (NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE);
  if (!HiiGetBrowserData (
          &gNhltEndpointsTableConfigurationVariableGuid,
          NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_NAME,
          NhltConfigurationVariableSize,
          (UINT8 *) &NhltConfiguration)) {
    return EFI_NOT_FOUND;
  }

  if ((PchSetup.HdaDiscBtOffEnabled  ||
      (CnviCrfModuleIsPresent () && CnvSetupData.CnviMode != 0)) &&
      CnvSetupData.CnviBtAudioOffload) {
      // BT Sideband support
      PchSetup.PchHdAudioFeature[1] = TRUE;
      // BT Intel HFP SCO
      PchSetup.PchHdAudioFeature[5] = TRUE;
      // BT Intel A2DP
      PchSetup.PchHdAudioFeature[6] = TRUE;
      // BT Intel LE
      PchSetup.PchHdAudioFeature[9] = TRUE;
      // SoundWire interface enable
      if (CnvSetupData.CnviBtAudioOffloadInterface == 1) {
        // Audio DSP NHLT Endpoints Configuration - Bluetooth
        NhltConfiguration.NhltBluetoothEnabled = FALSE;
      } else { //I2S (legacy) interface is used
        // Audio DSP NHLT Endpoints Configuration - Bluetooth
        NhltConfiguration.NhltBluetoothEnabled = TRUE;
      }
  } else {
    // Audio DSP NHLT Endpoints Configuration - Bluetooth
    NhltConfiguration.NhltBluetoothEnabled = FALSE;
    // BT Sideband support
    PchSetup.PchHdAudioFeature[1] = FALSE;
    // BT Intel HFP SCO
    PchSetup.PchHdAudioFeature[5] = FALSE;
    // BT Intel A2DP
    PchSetup.PchHdAudioFeature[6] = FALSE;
    // BT Intel LE
    PchSetup.PchHdAudioFeature[9] = FALSE;
  }

  //
  // Update NHLT browser data
  //
  RequestString = NULL;
  TmpString = NULL;
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE, NhltBluetoothEnabled), sizeof (NhltConfiguration.NhltBluetoothEnabled));
  if (RequestString == NULL) {
    goto ExitNoSpace;
  }

  if (!HiiSetBrowserData (&gNhltEndpointsTableConfigurationVariableGuid, NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_NAME, NhltConfigurationVariableSize, (UINT8 *) &NhltConfiguration, RequestString)) {
    goto ExitNotFound;
  }
  FreePool (RequestString);

  //
  // Update PCH browser data
  //
  RequestString = NULL;
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (PCH_SETUP, PchHdAudioFeature[1]), sizeof (PchSetup.PchHdAudioFeature[1]));
  if (RequestString == NULL) {
    goto ExitNoSpace;
  }

  TmpString = RequestString;
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (PCH_SETUP, PchHdAudioFeature[5]), sizeof (PchSetup.PchHdAudioFeature[5]));
  if (RequestString == NULL) {
    goto ExitNoSpace;
  }

  TmpString = RequestString;
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (PCH_SETUP, PchHdAudioFeature[6]), sizeof (PchSetup.PchHdAudioFeature[6]));
  if (RequestString == NULL) {
    goto ExitNoSpace;
  }

  TmpString = RequestString;
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (PCH_SETUP, PchHdAudioFeature[9]), sizeof (PchSetup.PchHdAudioFeature[9]));
  if (RequestString == NULL) {
    goto ExitNoSpace;
  }

  if (!HiiSetBrowserData (&gPchSetupVariableGuid, L"PchSetup", PchVarSize, (UINT8 *) &PchSetup, RequestString)) {
    goto ExitNotFound;
  }
  FreePool (RequestString);

  return EFI_SUCCESS;

  ExitNoSpace:
    if (TmpString != NULL) {
      FreePool (TmpString);
    }
    return EFI_OUT_OF_RESOURCES;

  ExitNotFound:
    FreePool (RequestString);
    return EFI_NOT_FOUND;
}
