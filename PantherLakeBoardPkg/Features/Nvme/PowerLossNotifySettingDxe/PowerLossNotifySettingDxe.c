 /**@file
  Power Loss Notification Setting get the PLN setting and set the EC and GPIO for PLN

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include "PowerLossNotifySettingDxe.h"
#include <Library/EcMiscLib.h>
#include <NvmeSetup.h>

POWER_LOSS_NOTIFY_SETTING_PROTOCOL  mPowerLossNotifySettingProtocol = {
  GetPlnSetting,
  SetPlnController
};

/**
  Enable or disable PLN with GPIO on CS or non-CS mode.

  @param[in]  EnablePln        Enable / Disable PLN feature.
  @param[in]  Cs               Connected Standby, 1: enabled, 0: disabled.

**/
VOID
GpioControlPln (
  IN  UINT8                     EnablePln,
  IN  UINT8                     Cs
  )
{
  UINT32                            PlnDelaySelGpio;
  UINT32                            PlnDelayEnGpio;

  PlnDelayEnGpio = PcdGet32 (PcdPlnDelayEnableGpio);
  PlnDelaySelGpio = PcdGet32 (PcdPlnDelaySelectionGpio);

  //
  // There is no discrete logic to control PLN.
  //
  if (PlnDelayEnGpio == 0 || PlnDelaySelGpio == 0) {
    return;
  }

  if (EnablePln == PLN_DISABLE) {
    GpioV2SetTx (PlnDelayEnGpio, GPIO_PLN_DISABLED);
  } else if(EnablePln == PLN_ENABLE){
    GpioV2SetTx (PlnDelayEnGpio, GPIO_PLN_ENABLED);
    if (Cs == 0) {
      GpioV2SetTx (PlnDelaySelGpio, GPIO_PLN_PB_OVR_NO_CS);
    } else {
      GpioV2SetTx (PlnDelaySelGpio, GPIO_PLN_PB_OVR_CS);
    }
  }
}

/**
  Get the PLN Setting.

  @param[in]  This                 A pointer to the POWER_LOSS_NOTIFY_SETTING_PROTOCOL instance.
  @param[out] Setting              The PLN Setting.

  @retval EFI_SUCCESS              The operation succeeds.

**/
EFI_STATUS
EFIAPI
GetPlnSetting (
  IN  POWER_LOSS_NOTIFY_SETTING_PROTOCOL    *This,
  OUT UINTN                                 *Setting
  )
{
  EFI_STATUS                         Status;
  UINTN                              VarDataSize;
  NVME_SETUP                         NvmeSetup;

  VarDataSize = sizeof (NVME_SETUP);
  Status = gRT->GetVariable (
                  L"NvmeSetup",
                  &gNvmeSetupVariableGuid,
                  NULL,
                  &VarDataSize,
                  &NvmeSetup
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  *Setting = NvmeSetup.PlnEnable;
  return EFI_SUCCESS;
}

/**
  Set the EC data for PLN and set the GPIO for PLN.

  @param[in]  This                 A pointer to the POWER_LOSS_NOTIFY_SETTING_PROTOCOL instance.
  @param[in]  Setting              Enable/Disable of PLN Setting.

  @retval EFI_SUCCESS              The operation succeeds.

**/
EFI_STATUS
EFIAPI
SetPlnController (
  IN  POWER_LOSS_NOTIFY_SETTING_PROTOCOL    *This,
  IN  UINTN                                 Setting
  )
{
  EFI_STATUS                         Status;
  UINTN                              VarDataSize;
  SETUP_DATA                         SystemConfiguration;
  UINT8                              EnablePln;

  VarDataSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &VarDataSize,
                  &SystemConfiguration
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if((Setting == PLN_DISABLE) || (Setting == PLN_ENABLE)) {
    EnablePln = (Setting==PLN_ENABLE)? 1:0;
    //
    // 1. Send command to EC to notify the power button override time if go with EC path
    // 2. Set GPIO
    //
    EcControlPln (EnablePln, SystemConfiguration.LowPowerS0Idle);
    GpioControlPln (EnablePln, SystemConfiguration.LowPowerS0Idle);
  }
  return EFI_SUCCESS;
}

/**
  Driver entry point to PLN Setting.

  @param[in] ImageHandle      The firmware allocated handle for the EFI image.
  @param[in] SystemTable      A pointer to the EFI System Table.

  @retval EFI_SUCCESS         Notify event has been created
**/
EFI_STATUS
EFIAPI
PowerLossNotifySettingEntryPoint (
  IN EFI_HANDLE                       ImageHandle,
  IN EFI_SYSTEM_TABLE                 *SystemTable
  )
{
  EFI_STATUS                         Status;
  UINTN                              VarDataSize;
  NVME_SETUP                         NvmeSetup;
  EFI_HANDLE                         ProtocolHandle;

  //
  // Check PLN SETUP option
  //
  VarDataSize = sizeof (NVME_SETUP);
  Status = gRT->GetVariable (
                  L"NvmeSetup",
                  &gNvmeSetupVariableGuid,
                  NULL,
                  &VarDataSize,
                  &NvmeSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PLN :: %a: Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "PLN :: %a: PlnEnable=%d\n", __FUNCTION__, NvmeSetup.PlnEnable));
  if((NvmeSetup.PlnEnable == PLN_DISABLE)
    || (NvmeSetup.PlnEnable == PLN_ENABLE)) {
    //
    // Install Protocol.
    //
    ProtocolHandle = NULL;
    return gBS->InstallMultipleProtocolInterfaces (
                  &ProtocolHandle,
                  &gPowerLossNotifySettingProtocolGuid,
                  (VOID*)&mPowerLossNotifySettingProtocol,
                  NULL
                  );
  }

  return EFI_SUCCESS;
}
