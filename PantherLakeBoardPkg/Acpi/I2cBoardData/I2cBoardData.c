/**@file
  I2C Board Driver
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

@par Specification
**/

#include <I2cBoardData.h>
#include <I2cDevices.h>
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Register/GpioV2PcdPins.h>
#include <GpioV2Pad.h>
#include <Uefi/UefiBaseType.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <SetupVariable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PchInfoLib.h>

/**
  Printing Vpd Board Data

  @param[in] VPD_I2C_RP_TABLE_PACKAGE    I2C Board Vpd Data
**/
VOID
PrintVpdData (
  VPD_I2C_RP_TABLE_PACKAGE      *VpdI2cRpTableData
  )
{
  UINT8  Index;
  DEBUG ((DEBUG_INFO, "VPD Struct Data\n"));
  for (Index = 0; Index < VpdI2cRpTableData->Count; Index ++) {
    DEBUG ((DEBUG_INFO, "TouchPadInterruptGpioPad = %x\n", VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPadGpioPin.TouchPadInterruptGpioPad));
    DEBUG ((DEBUG_INFO, "TouchPadInterruptGpioPolarity = %x\n", VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPadGpioPin.TouchPadInterruptGpioPolarity));
    DEBUG ((DEBUG_INFO, "TouchPanelInterruptGpioPad = %x\n", VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPanelGpioPin.TouchPanelInterruptGpioPad));
    DEBUG ((DEBUG_INFO, "TouchPanelInterruptGpioPolarity = %x\n", VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPanelGpioPin.TouchPanelInterruptGpioPolarity));
    DEBUG ((DEBUG_INFO, "TouchPanelPowerGpioPad = %x\n", VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPanelGpioPin.TouchPanelPowerGpioPad));
    DEBUG ((DEBUG_INFO, "TouchPanelPowerGpioPolarity = %x\n", VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPanelGpioPin.TouchPanelPowerGpioPolarity));
    DEBUG ((DEBUG_INFO, "TouchPanelResetGpioPad = %x\n", VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPanelGpioPin.TouchPanelResetGpioPad));
    DEBUG ((DEBUG_INFO, "TouchPanelResetGpioPolarity = %x\n", VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPanelGpioPin.TouchPanelResetGpioPolarity));
  }
}

/**
  Configuring board Vpd data into I2c Acpi Rtd3 Structure

   @param[in] I2C_DEVICES_CONFIG_PROTOCOL    I2C Config Protocol
**/
EFI_STATUS
EFIAPI
BoardConfigData (
  IN I2C_DEVICES_CONFIG_PROTOCOL       *I2cDevicesConfigProtocol
  )
{

  UINT8                         Index;
  VPD_I2C_RP_TABLE_PACKAGE      *VpdI2cRpTableData;
  PCH_SETUP                     *PchSetup;
  SETUP_DATA                    *SetupData;
  UINTN                         VarDataSize;
  EFI_STATUS                    Status;
  UINT8                         Count;
  UINT8                         Rtd3Support;
  I2C_ACPI_DEV                  I2cControllerInfo [MAX_I2C_ROOTPORT_ENTRY_SUPPORT];
  UINT8                         I2cIndex;
  I2C_DEVICES                   *I2cTPTD;
  ///
  /// Acclocate Memory for PCH Setup and Get the Setup Value
  ///
  PchSetup = NULL;
  VarDataSize = sizeof (PCH_SETUP);
  PchSetup = AllocateZeroPool (VarDataSize);
  if (PchSetup == NULL){
    return EFI_OUT_OF_RESOURCES;
  }


  Status = gRT->GetVariable (
                  L"PchSetup",
                  &gPchSetupVariableGuid,
                  NULL,
                  &VarDataSize,
                  PchSetup
                  );
  DEBUG ((DEBUG_ERROR,"Pch Setup Data Status= %r\n",Status));
  ///
  /// Acclocate Memory for  Setup and Get the Setup Value
  ///
  SetupData = NULL;
  VarDataSize = sizeof (SETUP_DATA);
  SetupData = AllocateZeroPool (VarDataSize);
  if (SetupData == NULL){
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &VarDataSize,
                  SetupData
                  );

  VpdI2cRpTableData = NULL;
  VarDataSize = sizeof (VPD_I2C_RP_TABLE_PACKAGE);
  VpdI2cRpTableData = AllocateZeroPool (VarDataSize);
  if (VpdI2cRpTableData == NULL){
    return EFI_OUT_OF_RESOURCES;
  }
  ///
  /// Acclocate Memory for I2c_devices
  ///
  I2cTPTD = NULL;
  VarDataSize = sizeof (I2C_DEVICES);
  I2cTPTD =  AllocateZeroPool (VarDataSize);
  if (I2cTPTD == NULL){
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (VpdI2cRpTableData , PcdGetPtr (VpdI2CRptable), sizeof (VPD_I2C_RP_TABLE_PACKAGE));
  if ((VpdI2cRpTableData->Count > MAX_I2C_ROOTPORT_ENTRY_SUPPORT)){
    return EFI_INVALID_PARAMETER;
  }
  PrintVpdData (VpdI2cRpTableData);

  Count = GetMaxI2cInterfacesNum ();
  Rtd3Support = SetupData->Rtd3Support;

  ZeroMem (&I2cControllerInfo, sizeof (I2cControllerInfo));

  I2cTPTD->TouchPanelNvsVar.TouchPanelType = SetupData->PchI2cTouchPanelType;
  I2cTPTD->TouchPanelNvsVar.TouchPanelIrqMode = SetupData->PchI2cTouchPanelIrqMode;
  I2cTPTD->TouchPanelNvsVar.TouchPanelBusAddress = SetupData->PchI2cTouchPanelBusAddress;
  I2cTPTD->TouchPanelNvsVar.TouchPanelSpeed = SetupData->PchI2cTouchPanelSpeed;
  I2cTPTD->TouchPanelNvsVar.TouchPanelHidAddress = SetupData->PchI2cTouchPanelHidAddress ;
  I2cTPTD->TouchPanelNvsVar.Touchpanel0IrqGpio = PcdGet32(PcdTouchpanel0IrqGpio);
  I2cTPTD->TouchPanelNvsVar.TouchpanelIrqGpio = PcdGet32(PcdTouchpanelIrqGpio);
  I2cTPTD->TouchPadNvsVar.TouchPadType = SetupData->PchI2cTouchPadType;
  I2cTPTD->TouchPadNvsVar.TouchPadIrqMode = SetupData->PchI2cTouchPadIrqMode;
  I2cTPTD->TouchPadNvsVar.TouchPadBusAddress = SetupData->PchI2cTouchPadBusAddress;
  I2cTPTD->TouchPadNvsVar.TouchPadSpeed = SetupData->PchI2cTouchPadSpeed;
  I2cTPTD->TouchPadNvsVar.TouchPadHidAddress = SetupData->PchI2cTouchPadHidAddress;
  I2cTPTD->TouchPadNvsVar.TouchpadIrqGpio = PcdGet32(PcdTouchpadIrqGpio);

  for (Index = 0; Index < VpdI2cRpTableData->Count; Index ++) {
    I2cIndex = VpdI2cRpTableData->I2CRpInfoTable[Index].I2CRpIndex;
    I2cControllerInfo[I2cIndex].I2cControllerTouchIndex = VpdI2cRpTableData->I2CRpInfoTable[Index].I2CRpIndex;
    if (PchSetup->PchSerialIoI2c[I2cIndex] == 1) {
      DEBUG ((DEBUG_INFO, "Controller Enabled = %x\n", PchSetup->PchSerialIoI2c[I2cIndex]));
      I2cControllerInfo[I2cIndex].I2cControllerEnabled = PchSetup->PchSerialIoI2c[I2cIndex];
      if (((SetupData->PchI2cSensorDevicePort[I2cIndex] & 0x1) == TOUCH_PAD_TYPE) && (SetupData->PchI2cTouchPadType)) {
        DEBUG ((DEBUG_INFO, "Touch Pad Type = %x\n",SetupData->PchI2cTouchPadType));
        I2cControllerInfo[I2cIndex].I2cDeviceType =TOUCH_PAD_TYPE;
        I2cControllerInfo[I2cIndex].TouchPadInfo.TouchPadInterruptGpio = VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPadGpioPin.TouchPadInterruptGpioPad;
        I2cControllerInfo[I2cIndex].TouchPadInfo.TouchPadInterruptGpioPolarity = VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPadGpioPin.TouchPadInterruptGpioPolarity;
      }
      if (((SetupData->PchI2cSensorDevicePort[I2cIndex] & TOUCH_PANEL_TYPE) == TOUCH_PANEL_TYPE) && (SetupData->PchI2cTouchPanelType)) {
        DEBUG ((DEBUG_INFO, "Touch Panel Type = %x\n",SetupData->PchI2cTouchPanelType));
        I2cControllerInfo[I2cIndex].I2cDeviceType =TOUCH_PANEL_TYPE;
        I2cControllerInfo[I2cIndex].TouchPanelInfo.TouchPanelInterruptGpio = VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPanelGpioPin.TouchPanelInterruptGpioPad;
        I2cControllerInfo[I2cIndex].TouchPanelInfo.TouchPanelInterruptGpioPolarity = VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPanelGpioPin.TouchPanelInterruptGpioPolarity;
        I2cControllerInfo[I2cIndex].TouchPanelInfo.TouchpanelPwrGpio = VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPanelGpioPin.TouchPanelPowerGpioPad;
        I2cControllerInfo[I2cIndex].TouchPanelInfo.TouchpanelPwrGpioPolarity = VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPanelGpioPin.TouchPanelPowerGpioPolarity;
        I2cControllerInfo[I2cIndex].TouchPanelInfo.TouchPanelResetGpio = VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPanelGpioPin.TouchPanelResetGpioPad;
        I2cControllerInfo[I2cIndex].TouchPanelInfo.TouchPanelResetGpioPolarity = VpdI2cRpTableData->I2CRpInfoTable[Index].TouchPanelGpioPin.TouchPanelResetGpioPolarity;
      }
      if (((SetupData->PchI2cSensorDevicePort[I2cIndex] & TOUCH_PAD_AND_PANEL_SUPPORT) == TOUCH_PAD_AND_PANEL_SUPPORT)) {
        I2cControllerInfo[I2cIndex].I2cDeviceType = TOUCH_PAD_AND_PANEL_SUPPORT;
      }
    }
  }
  Status = I2cDevicesConfigProtocol->I2CBoardData (Count,I2cControllerInfo,Rtd3Support,I2cTPTD);
  FreePool(I2cTPTD);
  FreePool(VpdI2cRpTableData);
  if(Status != EFI_SUCCESS){
    return EFI_NOT_FOUND;
  }
  return Status;
}

/**
  I2c Board driver installation function.

  @param[in] ImageHandle     Handle for this drivers loaded image protocol.
  @param[in] SystemTable     EFI system table.

  @retval EFI_SUCCESS        The protocol was located succesfully.
**/
EFI_STATUS
EFIAPI
I2cBoardInfo (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                      Status;
  I2C_DEVICES_CONFIG_PROTOCOL       *I2cDevicesConfigProtocol;

  DEBUG ((DEBUG_INFO, "BoardInfo\n"));
  Status = gBS->LocateProtocol (&gI2cDeviceInfoProtocolGuid, NULL, (VOID **)&I2cDevicesConfigProtocol);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "The status passed\n"));
    Status = BoardConfigData (I2cDevicesConfigProtocol);
    if (!EFI_ERROR (Status)){
      return Status;
    }else{
      return EFI_UNSUPPORTED;
    }
  } else {
    DEBUG ((DEBUG_INFO, "Not able to find Protocol\n"));
  }
  return Status;
}
