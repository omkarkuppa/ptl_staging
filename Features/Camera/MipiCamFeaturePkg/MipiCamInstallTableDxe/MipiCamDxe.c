/** @file
  This DXE driver configures and supports Mipi Camera.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include <Library/BaseMemoryLib.h>
#include "MipiCamDxe.h"
#include <MipiCamConfig.h>
#include <Protocol/MipiCamNvsArea.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Guid/MipiCamConfigHob.h>

#include <Library/PcdLib.h>
#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 0x1
    #include <NhltEndpointsConfigurationVariable.h>
#endif

GLOBAL_REMOVE_IF_UNREFERENCED  MIPICAM_NVS_AREA_PROTOCOL  mMipiCamNvsAreaProtocol;

EFI_STATUS
EFIAPI
InitializeMipiCam (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  )
/**
@brief
  This procedure does all the Mipi Camera initialization and loads the ACPI tables.

  @param[in] ImageHandle   - The firmware allocated handle to the Driver Image
  @param[in] SystemTable   - Pointer to the EFI System Table

  @retval EFI_SUCCESS - The driver installed/initialized correctly.
**/
{
  EFI_STATUS             Status;
  UINTN                  Index;
  EFI_PHYSICAL_ADDRESS   Area;
  UINTN                  Pages;
  UINT32                 MipiVariableAttributes;
  UINTN                  VariableSize;
  UINT16                 MipiModuleName[MIPICAM_MODULE_NAME_LENGTH];
  UINT16                 MipiHidName[MIPICAM_HID_LENGTH];
  #if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 0x1
  UINT16                        AudioHidName[MIPICAM_HID_LENGTH];
  UINT16                        InvalidHidName[MIPICAM_HID_LENGTH] = { 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x0 };
  NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE  NhltConfigurationEfiVariable;
#endif
  MIPICAM_CONFIG         MipiCamSetupData;
  MIPICAM_CONFIG_HOB     *MipiCamConfigDxeHob;
  EFI_HOB_GUID_TYPE      *GuidHob;

  DEBUG ((DEBUG_INFO, "Mipi Camera: EntryPoint\n"));

  VariableSize = sizeof (MIPICAM_CONFIG);
  ZeroMem ((VOID *)&mMipiCamNvsAreaProtocol, sizeof (MIPICAM_NVS_AREA_PROTOCOL));
  ZeroMem ((VOID *)&MipiCamSetupData,        sizeof (MIPICAM_CONFIG));

  Status = gRT->GetVariable (
                  MIPICAM_CONFIG_VARIABLE_NAME,
                  &gMipiCamConfigVariableGuid,
                  &MipiVariableAttributes,
                  &VariableSize,
                  &MipiCamSetupData
                  );

  if (Status == EFI_NOT_FOUND) {

    MipiVariableAttributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;
    VariableSize = sizeof (MIPICAM_CONFIG);
    ZeroMem (&MipiCamSetupData, sizeof (MIPICAM_CONFIG));

    Status = gRT->SetVariable (
                    MIPICAM_CONFIG_VARIABLE_NAME,
                    &gMipiCamConfigVariableGuid,
                    MipiVariableAttributes,
                    VariableSize,
                    &MipiCamSetupData
                    );
    DEBUG ((DEBUG_INFO, "Mipi Camera: Creating Mipi Camera Config Variable\n"));

    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_INFO, "Mipi Camera: Creating Mipi Camera Config Variable Error Status = %r.\n", Status));
      return Status;
    }
  }

  ///
  /// Allocated ACPI NVS type memory for MipiCam Nvs Configuration.
  ///
  Pages  = EFI_SIZE_TO_PAGES (sizeof (MIPICAM_NVS_AREA));
  Area   = MAX_UINT32; // allocate address below 4G.
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiACPIMemoryNVS,
                  Pages,
                  &Area
                  );

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  mMipiCamNvsAreaProtocol.Area = (MIPICAM_NVS_AREA *) (UINTN) Area;
  ZeroMem ((VOID *)mMipiCamNvsAreaProtocol.Area, sizeof (MIPICAM_NVS_AREA));

  //
  // MipiCam sensor configuration
  //
  mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic0 = MipiCamSetupData.MipiCam_ControlLogic0;
  mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic1 = MipiCamSetupData.MipiCam_ControlLogic1;
  mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic2 = MipiCamSetupData.MipiCam_ControlLogic2;
  mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic3 = MipiCamSetupData.MipiCam_ControlLogic3;
  mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic4 = MipiCamSetupData.MipiCam_ControlLogic4;
  mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic5 = MipiCamSetupData.MipiCam_ControlLogic5;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0Enabled  = MipiCamSetupData.MipiCam_Link0;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1Enabled  = MipiCamSetupData.MipiCam_Link1;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2Enabled  = MipiCamSetupData.MipiCam_Link2;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3Enabled  = MipiCamSetupData.MipiCam_Link3;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4Enabled  = MipiCamSetupData.MipiCam_Link4;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5Enabled  = MipiCamSetupData.MipiCam_Link5;

  mMipiCamNvsAreaProtocol.Area->MipiCamLanesClkDiv   = MipiCamSetupData.MipiCam_LanesClkDiv;

  // MipiCam Control Logic 0 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Version              = 1;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Type                 = MipiCamSetupData.MipiCam_ControlLogic0_Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_CrdVersion           = MipiCamSetupData.MipiCam_ControlLogic0_CrdVersion;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_InputClock           = MipiCamSetupData.MipiCam_ControlLogic0_InputClock;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioPinsEnabled      = MipiCamSetupData.MipiCam_ControlLogic0_GpioPinsEnabled;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_I2cBus               = MipiCamSetupData.MipiCam_ControlLogic0_I2cChannel;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_I2cAddress           = MipiCamSetupData.MipiCam_ControlLogic0_I2cAddress;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Pld                  = MipiCamSetupData.MipiCam_ControlLogic0_Pld;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled1FlashMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic0_Wled1FlashMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled1TorchMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic0_Wled1TorchMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled2FlashMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic0_Wled2FlashMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled2TorchMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic0_Wled2TorchMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled1Type            = MipiCamSetupData.MipiCam_ControlLogic0_Wled1Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled2Type            = MipiCamSetupData.MipiCam_ControlLogic0_Wled2Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_PchClockSource       = MipiCamSetupData.MipiCam_ControlLogic0_PchClockSource;

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    if (Index < MipiCamSetupData.MipiCam_ControlLogic0_GpioPinsEnabled) {
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioGroupPadNumber[Index] = MipiCamSetupData.MipiCam_ControlLogic0_GpioGroupPadNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioComNumber[Index]      = MipiCamSetupData.MipiCam_ControlLogic0_GpioComNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioGroupNumber[Index]    = MipiCamSetupData.MipiCam_ControlLogic0_GpioGroupNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioFunction[Index]       = MipiCamSetupData.MipiCam_ControlLogic0_GpioFunction[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioActiveValue[Index]    = MipiCamSetupData.MipiCam_ControlLogic0_GpioActiveValue[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioInitialValue[Index]   = MipiCamSetupData.MipiCam_ControlLogic0_GpioInitialValue[Index];
      if (MipiCamSetupData.MipiCam_ControlLogic0_GpioFunction[Index] == 0xFF) {
        mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioFunction[Index]     = MipiCamSetupData.MipiCam_ControlLogic0_GpioFunction_User[Index];
      }
    } else {
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioGroupPadNumber[Index] = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioGroupNumber[Index]    = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioComNumber[Index]      = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioFunction[Index]       = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioActiveValue[Index]    = 1;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioInitialValue[Index]   = 0;
    }
  }

  // MipiCam Control Logic 1 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Version              = 1;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Type                 = MipiCamSetupData.MipiCam_ControlLogic1_Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_CrdVersion           = MipiCamSetupData.MipiCam_ControlLogic1_CrdVersion;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_InputClock           = MipiCamSetupData.MipiCam_ControlLogic1_InputClock;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioPinsEnabled      = MipiCamSetupData.MipiCam_ControlLogic1_GpioPinsEnabled;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_I2cBus               = MipiCamSetupData.MipiCam_ControlLogic1_I2cChannel;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_I2cAddress           = MipiCamSetupData.MipiCam_ControlLogic1_I2cAddress;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Pld                  = MipiCamSetupData.MipiCam_ControlLogic1_Pld;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled1FlashMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic1_Wled1FlashMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled1TorchMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic1_Wled1TorchMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled2FlashMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic1_Wled2FlashMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled2TorchMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic1_Wled2TorchMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled1Type            = MipiCamSetupData.MipiCam_ControlLogic1_Wled1Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled2Type            = MipiCamSetupData.MipiCam_ControlLogic1_Wled2Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_PchClockSource       = MipiCamSetupData.MipiCam_ControlLogic1_PchClockSource;

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    if (Index < MipiCamSetupData.MipiCam_ControlLogic1_GpioPinsEnabled) {
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioGroupPadNumber[Index] = MipiCamSetupData.MipiCam_ControlLogic1_GpioGroupPadNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioComNumber[Index]      = MipiCamSetupData.MipiCam_ControlLogic1_GpioComNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioGroupNumber[Index]    = MipiCamSetupData.MipiCam_ControlLogic1_GpioGroupNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioFunction[Index]       = MipiCamSetupData.MipiCam_ControlLogic1_GpioFunction[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioActiveValue[Index]    = MipiCamSetupData.MipiCam_ControlLogic1_GpioActiveValue[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioInitialValue[Index]   = MipiCamSetupData.MipiCam_ControlLogic1_GpioInitialValue[Index];
      if (MipiCamSetupData.MipiCam_ControlLogic1_GpioFunction[Index] == 0xFF) {
        mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioFunction[Index]     = MipiCamSetupData.MipiCam_ControlLogic1_GpioFunction_User[Index];
      }
    } else {
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioGroupPadNumber[Index] = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioGroupNumber[Index]    = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioComNumber[Index]      = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioFunction[Index]       = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioActiveValue[Index]    = 1;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioInitialValue[Index]   = 0;
    }
  }

  // MipiCam Control Logic 2 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Version              = 1;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Type                 = MipiCamSetupData.MipiCam_ControlLogic2_Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_CrdVersion           = MipiCamSetupData.MipiCam_ControlLogic2_CrdVersion;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_InputClock           = MipiCamSetupData.MipiCam_ControlLogic2_InputClock;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioPinsEnabled      = MipiCamSetupData.MipiCam_ControlLogic2_GpioPinsEnabled;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_I2cBus               = MipiCamSetupData.MipiCam_ControlLogic2_I2cChannel;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_I2cAddress           = MipiCamSetupData.MipiCam_ControlLogic2_I2cAddress;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Pld                  = MipiCamSetupData.MipiCam_ControlLogic2_Pld;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled1FlashMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic2_Wled1FlashMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled1TorchMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic2_Wled1TorchMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled2FlashMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic2_Wled2FlashMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled2TorchMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic2_Wled2TorchMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled1Type            = MipiCamSetupData.MipiCam_ControlLogic2_Wled1Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled2Type            = MipiCamSetupData.MipiCam_ControlLogic2_Wled2Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_PchClockSource       = MipiCamSetupData.MipiCam_ControlLogic2_PchClockSource;

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    if (Index < MipiCamSetupData.MipiCam_ControlLogic2_GpioPinsEnabled) {
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioGroupPadNumber[Index] = MipiCamSetupData.MipiCam_ControlLogic2_GpioGroupPadNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioComNumber[Index]      = MipiCamSetupData.MipiCam_ControlLogic2_GpioComNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioGroupNumber[Index]    = MipiCamSetupData.MipiCam_ControlLogic2_GpioGroupNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioFunction[Index]       = MipiCamSetupData.MipiCam_ControlLogic2_GpioFunction[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioActiveValue[Index]    = MipiCamSetupData.MipiCam_ControlLogic2_GpioActiveValue[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioInitialValue[Index]   = MipiCamSetupData.MipiCam_ControlLogic2_GpioInitialValue[Index];
      if (MipiCamSetupData.MipiCam_ControlLogic2_GpioFunction[Index] == 0xFF) {
        mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioFunction[Index]     = MipiCamSetupData.MipiCam_ControlLogic2_GpioFunction_User[Index];
      }
    } else {
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioGroupPadNumber[Index] = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioGroupNumber[Index]    = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioComNumber[Index]      = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioFunction[Index]       = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioActiveValue[Index]    = 1;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioInitialValue[Index]   = 0;
    }
  }

  // MipiCam Control Logic 3 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Version              = 1;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Type                 = MipiCamSetupData.MipiCam_ControlLogic3_Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_CrdVersion           = MipiCamSetupData.MipiCam_ControlLogic3_CrdVersion;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_InputClock           = MipiCamSetupData.MipiCam_ControlLogic3_InputClock;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioPinsEnabled      = MipiCamSetupData.MipiCam_ControlLogic3_GpioPinsEnabled;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_I2cBus               = MipiCamSetupData.MipiCam_ControlLogic3_I2cChannel;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_I2cAddress           = MipiCamSetupData.MipiCam_ControlLogic3_I2cAddress;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Pld                  = MipiCamSetupData.MipiCam_ControlLogic3_Pld;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled1FlashMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic3_Wled1FlashMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled1TorchMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic3_Wled1TorchMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled2FlashMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic3_Wled2FlashMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled2TorchMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic3_Wled2TorchMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled1Type            = MipiCamSetupData.MipiCam_ControlLogic3_Wled1Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled2Type            = MipiCamSetupData.MipiCam_ControlLogic3_Wled2Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_PchClockSource       = MipiCamSetupData.MipiCam_ControlLogic3_PchClockSource;

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    if (Index < MipiCamSetupData.MipiCam_ControlLogic3_GpioPinsEnabled) {
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioGroupPadNumber[Index] = MipiCamSetupData.MipiCam_ControlLogic3_GpioGroupPadNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioComNumber[Index]      = MipiCamSetupData.MipiCam_ControlLogic3_GpioComNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioGroupNumber[Index]    = MipiCamSetupData.MipiCam_ControlLogic3_GpioGroupNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioFunction[Index]       = MipiCamSetupData.MipiCam_ControlLogic3_GpioFunction[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioActiveValue[Index]    = MipiCamSetupData.MipiCam_ControlLogic3_GpioActiveValue[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioInitialValue[Index]   = MipiCamSetupData.MipiCam_ControlLogic3_GpioInitialValue[Index];
      if (MipiCamSetupData.MipiCam_ControlLogic3_GpioFunction[Index] == 0xFF) {
        mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioFunction[Index]     = MipiCamSetupData.MipiCam_ControlLogic3_GpioFunction_User[Index];
      }
    } else {
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioGroupPadNumber[Index] = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioGroupNumber[Index]    = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioComNumber[Index]      = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioFunction[Index]       = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioActiveValue[Index]    = 1;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioInitialValue[Index]   = 0;
    }
  }

  // MipiCam Control Logic 4 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Version              = 1;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Type                 = MipiCamSetupData.MipiCam_ControlLogic4_Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_CrdVersion           = MipiCamSetupData.MipiCam_ControlLogic4_CrdVersion;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_InputClock           = MipiCamSetupData.MipiCam_ControlLogic4_InputClock;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioPinsEnabled      = MipiCamSetupData.MipiCam_ControlLogic4_GpioPinsEnabled;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_I2cBus               = MipiCamSetupData.MipiCam_ControlLogic4_I2cChannel;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_I2cAddress           = MipiCamSetupData.MipiCam_ControlLogic4_I2cAddress;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Pld                  = MipiCamSetupData.MipiCam_ControlLogic4_Pld;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled1FlashMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic4_Wled1FlashMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled1TorchMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic4_Wled1TorchMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled2FlashMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic4_Wled2FlashMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled2TorchMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic4_Wled2TorchMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled1Type            = MipiCamSetupData.MipiCam_ControlLogic4_Wled1Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled2Type            = MipiCamSetupData.MipiCam_ControlLogic4_Wled2Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_PchClockSource       = MipiCamSetupData.MipiCam_ControlLogic4_PchClockSource;

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    if (Index < MipiCamSetupData.MipiCam_ControlLogic4_GpioPinsEnabled) {
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioGroupPadNumber[Index] = MipiCamSetupData.MipiCam_ControlLogic4_GpioGroupPadNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioComNumber[Index]      = MipiCamSetupData.MipiCam_ControlLogic4_GpioComNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioGroupNumber[Index]    = MipiCamSetupData.MipiCam_ControlLogic4_GpioGroupNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioFunction[Index]       = MipiCamSetupData.MipiCam_ControlLogic4_GpioFunction[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioActiveValue[Index]    = MipiCamSetupData.MipiCam_ControlLogic4_GpioActiveValue[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioInitialValue[Index]   = MipiCamSetupData.MipiCam_ControlLogic4_GpioInitialValue[Index];
      if (MipiCamSetupData.MipiCam_ControlLogic4_GpioFunction[Index] == 0xFF) {
        mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioFunction[Index]     = MipiCamSetupData.MipiCam_ControlLogic4_GpioFunction_User[Index];
      }
    } else {
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioGroupPadNumber[Index] = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioGroupNumber[Index]    = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioComNumber[Index]      = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioFunction[Index]       = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioActiveValue[Index]    = 1;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioInitialValue[Index]   = 0;
    }
  }

  // MipiCam Control Logic 5 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Version              = 1;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Type                 = MipiCamSetupData.MipiCam_ControlLogic5_Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_CrdVersion           = MipiCamSetupData.MipiCam_ControlLogic5_CrdVersion;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_InputClock           = MipiCamSetupData.MipiCam_ControlLogic5_InputClock;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioPinsEnabled      = MipiCamSetupData.MipiCam_ControlLogic5_GpioPinsEnabled;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_I2cBus               = MipiCamSetupData.MipiCam_ControlLogic5_I2cChannel;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_I2cAddress           = MipiCamSetupData.MipiCam_ControlLogic5_I2cAddress;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Pld                  = MipiCamSetupData.MipiCam_ControlLogic5_Pld;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled1FlashMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic5_Wled1FlashMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled1TorchMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic5_Wled1TorchMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled2FlashMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic5_Wled2FlashMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled2TorchMaxCurrent = MipiCamSetupData.MipiCam_ControlLogic5_Wled2TorchMaxCurrent;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled1Type            = MipiCamSetupData.MipiCam_ControlLogic5_Wled1Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled2Type            = MipiCamSetupData.MipiCam_ControlLogic5_Wled2Type;
  mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_PchClockSource       = MipiCamSetupData.MipiCam_ControlLogic5_PchClockSource;

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    if (Index < MipiCamSetupData.MipiCam_ControlLogic5_GpioPinsEnabled) {
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioGroupPadNumber[Index] = MipiCamSetupData.MipiCam_ControlLogic5_GpioGroupPadNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioComNumber[Index]      = MipiCamSetupData.MipiCam_ControlLogic5_GpioComNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioGroupNumber[Index]    = MipiCamSetupData.MipiCam_ControlLogic5_GpioGroupNumber[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioFunction[Index]       = MipiCamSetupData.MipiCam_ControlLogic5_GpioFunction[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioActiveValue[Index]    = MipiCamSetupData.MipiCam_ControlLogic5_GpioActiveValue[Index];
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioInitialValue[Index]   = MipiCamSetupData.MipiCam_ControlLogic5_GpioInitialValue[Index];
      if (MipiCamSetupData.MipiCam_ControlLogic5_GpioFunction[Index] == 0xFF) {
        mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioFunction[Index]     = MipiCamSetupData.MipiCam_ControlLogic5_GpioFunction_User[Index];
      }
    } else {
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioGroupPadNumber[Index] = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioGroupNumber[Index]    = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioComNumber[Index]      = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioFunction[Index]       = 0;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioActiveValue[Index]    = 1;
      mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioInitialValue[Index]   = 0;
    }
  }

  // MipiCam Link0 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0SensorModel = MipiCamSetupData.MipiCam_Link0_SensorModel;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0Pld         = MipiCamSetupData.MipiCam_Link0_CameraPhysicalLocation;
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  // Need to copy to temporary aligned buffer due to pack(1) in MipiCamConfig.h
  CopyMem (MipiModuleName,MipiCamSetupData.MipiCam_Link0_ModuleName, sizeof (MipiModuleName));
  UnicodeStrToAsciiStrS (MipiModuleName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink0ModuleName, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamLink0ModuleName));
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  CopyMem (MipiHidName,MipiCamSetupData.MipiCam_Link0_UserHid, sizeof (MipiHidName));
  UnicodeStrToAsciiStrS (MipiHidName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink0UserHid, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamLink0UserHid));
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cDevicesEnabled = MipiCamSetupData.MipiCam_Link0_I2cDevicesEnabled;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cBus            = MipiCamSetupData.MipiCam_Link0_I2cChannel;

  #if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 0x1
  //
  // Audio Dynamic HID Link 0
  //
  ZeroMem(AudioHidName, sizeof(AudioHidName));
  if (MipiCamSetupData.Audio_Link0_UserHid[0] == '\0') {
      CopyMem(AudioHidName, InvalidHidName, sizeof(AudioHidName));
  }
  else {
      CopyMem(AudioHidName, MipiCamSetupData.Audio_Link0_UserHid, sizeof(AudioHidName));
  }
  UnicodeStrToAsciiStrS(AudioHidName, (CHAR8*)mMipiCamNvsAreaProtocol.Area->AudioLink0UserHid, sizeof(mMipiCamNvsAreaProtocol.Area->AudioLink0UserHid));
#endif

  // MipiCam Link0 I2C Devices
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cAddrDev[Index]    = MipiCamSetupData.MipiCam_Link0_I2cAddress[Index];
    switch (MipiCamSetupData.MipiCam_Link0_I2cDeviceType[Index]) {
      case MIPICAM_CUSTOMIZE_DEVICE1:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link0_Customize_Id[0];
        break;
      case MIPICAM_CUSTOMIZE_DEVICE2:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link0_Customize_Id[1];
        break;
      case MIPICAM_CUSTOMIZE_DEVICE3:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link0_Customize_Id[2];
        break;
      default:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link0_I2cDeviceType[Index];
    }
  }

  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_Version    = 1;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_CrdVersion = MipiCamSetupData.MipiCam_Link0_DriverData_CrdVersion;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_LinkUsed   = MipiCamSetupData.MipiCam_Link0_DriverData_LinkUsed;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_LaneUsed   = MipiCamSetupData.MipiCam_Link0_DriverData_LaneUsed;
  if (MipiCamSetupData.MipiCam_Link0_DriverData_EepromType == 0xFF) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_EepromType = MipiCamSetupData.MipiCam_Link0_DriverData_EepromTypeCustom;
  } else {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_EepromType = MipiCamSetupData.MipiCam_Link0_DriverData_EepromType;
  }
  if (MipiCamSetupData.MipiCam_Link0_DriverData_VcmType == 0xFF) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_VcmType = MipiCamSetupData.MipiCam_Link0_DriverData_VcmTypeCustom;
  } else {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_VcmType = MipiCamSetupData.MipiCam_Link0_DriverData_VcmType;
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_Mclk               = MipiCamSetupData.MipiCam_Link0_DriverData_Mclk;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_ControlLogic       = MipiCamSetupData.MipiCam_Link0_DriverData_ControlLogic;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_FlashSupport       = MipiCamSetupData.MipiCam_Link0_DriverData_FlashSupport;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_PrivacyLed         = MipiCamSetupData.MipiCam_Link0_DriverData_PrivacyLed;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_Degree             = MipiCamSetupData.MipiCam_Link0_DriverData_Degree;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_PmicPosition       = MipiCamSetupData.MipiCam_Link0_DriverData_PmicPosition;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_VoltageRail        = MipiCamSetupData.MipiCam_Link0_DriverData_VoltageRail;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_PhyConfiguration   = MipiCamSetupData.MipiCam_Link0_DriverData_PhyConfiguration;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_LaneConfiguration  = MipiCamSetupData.MipiCam_Link0_DriverData_LaneConfiguration;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0_FlashDriverSelection = MipiCamSetupData.MipiCam_Link0_FlashDriverSelection;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_FlashID            = MipiCamSetupData.MipiCam_Link0_DriverData_FlashID;

#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 0x1
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_PprValue = MipiCamSetupData.MipiCam_Link0_DriverData_PprValue;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_PprUnit = MipiCamSetupData.MipiCam_Link0_DriverData_PprUnit;
#endif

  // MipiCam Link1 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1SensorModel = MipiCamSetupData.MipiCam_Link1_SensorModel;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1Pld         = MipiCamSetupData.MipiCam_Link1_CameraPhysicalLocation;
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  // Need to copy to temporary aligned buffer due to pack(1) in MipiCamConfig.h
  CopyMem (MipiModuleName,MipiCamSetupData.MipiCam_Link1_ModuleName, sizeof (MipiModuleName));
  UnicodeStrToAsciiStrS (MipiModuleName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink1ModuleName, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamLink1ModuleName));
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  CopyMem (MipiHidName,MipiCamSetupData.MipiCam_Link1_UserHid, sizeof (MipiHidName));
  UnicodeStrToAsciiStrS (MipiHidName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink1UserHid, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamLink1UserHid));
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cDevicesEnabled = MipiCamSetupData.MipiCam_Link1_I2cDevicesEnabled;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cBus = MipiCamSetupData.MipiCam_Link1_I2cChannel;

  #if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 0x1
  //
  // Audio Dynamic HID Link 0
  //
  ZeroMem(AudioHidName, sizeof(AudioHidName));
  if (MipiCamSetupData.Audio_Link1_UserHid[0] == '\0') {
      CopyMem(AudioHidName, InvalidHidName, sizeof(AudioHidName));
  }
  else {
      CopyMem(AudioHidName, MipiCamSetupData.Audio_Link1_UserHid, sizeof(AudioHidName));
  }
  UnicodeStrToAsciiStrS(AudioHidName, (CHAR8*)mMipiCamNvsAreaProtocol.Area->AudioLink1UserHid, sizeof(mMipiCamNvsAreaProtocol.Area->AudioLink1UserHid));
#endif

  // MipiCam Link1 I2C Devices
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cAddrDev[Index] = MipiCamSetupData.MipiCam_Link1_I2cAddress[Index];
    switch (MipiCamSetupData.MipiCam_Link1_I2cDeviceType[Index]) {
      case MIPICAM_CUSTOMIZE_DEVICE1:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link1_Customize_Id[0];
        break;
      case MIPICAM_CUSTOMIZE_DEVICE2:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link1_Customize_Id[1];
        break;
      case MIPICAM_CUSTOMIZE_DEVICE3:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link1_Customize_Id[2];
        break;
      default:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link1_I2cDeviceType[Index];
    }
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_Version     = 1;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_CrdVersion  = MipiCamSetupData.MipiCam_Link1_DriverData_CrdVersion;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_LinkUsed    = MipiCamSetupData.MipiCam_Link1_DriverData_LinkUsed;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_LaneUsed    = MipiCamSetupData.MipiCam_Link1_DriverData_LaneUsed;
  if (MipiCamSetupData.MipiCam_Link1_DriverData_EepromType == 0xFF) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_EepromType = MipiCamSetupData.MipiCam_Link1_DriverData_EepromTypeCustom;
  } else {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_EepromType = MipiCamSetupData.MipiCam_Link1_DriverData_EepromType;
  }
  if (MipiCamSetupData.MipiCam_Link1_DriverData_VcmType == 0xFF) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_VcmType = MipiCamSetupData.MipiCam_Link1_DriverData_VcmTypeCustom;
  } else {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_VcmType = MipiCamSetupData.MipiCam_Link1_DriverData_VcmType;
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_Mclk               = MipiCamSetupData.MipiCam_Link1_DriverData_Mclk;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_ControlLogic       = MipiCamSetupData.MipiCam_Link1_DriverData_ControlLogic;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_FlashSupport       = MipiCamSetupData.MipiCam_Link1_DriverData_FlashSupport;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_PrivacyLed         = MipiCamSetupData.MipiCam_Link1_DriverData_PrivacyLed;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_Degree             = MipiCamSetupData.MipiCam_Link1_DriverData_Degree;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_PmicPosition       = MipiCamSetupData.MipiCam_Link1_DriverData_PmicPosition;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_VoltageRail        = MipiCamSetupData.MipiCam_Link1_DriverData_VoltageRail;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_PhyConfiguration   = MipiCamSetupData.MipiCam_Link1_DriverData_PhyConfiguration;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_LaneConfiguration  = MipiCamSetupData.MipiCam_Link1_DriverData_LaneConfiguration;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1_FlashDriverSelection = MipiCamSetupData.MipiCam_Link1_FlashDriverSelection;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_FlashID            = MipiCamSetupData.MipiCam_Link1_DriverData_FlashID;

#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 0x1
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_PprValue = MipiCamSetupData.MipiCam_Link1_DriverData_PprValue;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_PprUnit = MipiCamSetupData.MipiCam_Link1_DriverData_PprUnit;
#endif

  // MipiCam Link2 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2SensorModel = MipiCamSetupData.MipiCam_Link2_SensorModel;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2Pld         = MipiCamSetupData.MipiCam_Link2_CameraPhysicalLocation;
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  // Need to copy to temporary aligned buffer due to pack(1) in MipiCamConfig.h
  CopyMem (MipiModuleName,MipiCamSetupData.MipiCam_Link2_ModuleName, sizeof (MipiModuleName));
  UnicodeStrToAsciiStrS (MipiModuleName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink2ModuleName, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamLink2ModuleName));
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  CopyMem (MipiHidName,MipiCamSetupData.MipiCam_Link2_UserHid, sizeof (MipiHidName));
  UnicodeStrToAsciiStrS (MipiHidName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink2UserHid, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamLink2UserHid));
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cDevicesEnabled = MipiCamSetupData.MipiCam_Link2_I2cDevicesEnabled;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cBus = MipiCamSetupData.MipiCam_Link2_I2cChannel;

  // MipiCam Link2 I2C Devices
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cAddrDev[Index]    = MipiCamSetupData.MipiCam_Link2_I2cAddress[Index];
    switch (MipiCamSetupData.MipiCam_Link2_I2cDeviceType[Index]) {
      case MIPICAM_CUSTOMIZE_DEVICE1:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link2_Customize_Id[0];
        break;
      case MIPICAM_CUSTOMIZE_DEVICE2:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link2_Customize_Id[1];
        break;
      case MIPICAM_CUSTOMIZE_DEVICE3:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link2_Customize_Id[2];
        break;
      default:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link2_I2cDeviceType[Index];
    }
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_Version     = 1;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_CrdVersion  = MipiCamSetupData.MipiCam_Link2_DriverData_CrdVersion;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_LinkUsed    = MipiCamSetupData.MipiCam_Link2_DriverData_LinkUsed;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_LaneUsed    = MipiCamSetupData.MipiCam_Link2_DriverData_LaneUsed;
  if (MipiCamSetupData.MipiCam_Link2_DriverData_EepromType == 0xFF) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_EepromType = MipiCamSetupData.MipiCam_Link2_DriverData_EepromTypeCustom;
  } else {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_EepromType = MipiCamSetupData.MipiCam_Link2_DriverData_EepromType;
  }
  if (MipiCamSetupData.MipiCam_Link2_DriverData_VcmType == 0xFF) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_VcmType = MipiCamSetupData.MipiCam_Link2_DriverData_VcmTypeCustom;
  } else {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_VcmType = MipiCamSetupData.MipiCam_Link2_DriverData_VcmType;
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_Mclk               = MipiCamSetupData.MipiCam_Link2_DriverData_Mclk;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_ControlLogic       = MipiCamSetupData.MipiCam_Link2_DriverData_ControlLogic;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_FlashSupport       = MipiCamSetupData.MipiCam_Link2_DriverData_FlashSupport;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_PrivacyLed         = MipiCamSetupData.MipiCam_Link2_DriverData_PrivacyLed;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_Degree             = MipiCamSetupData.MipiCam_Link2_DriverData_Degree;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_PmicPosition       = MipiCamSetupData.MipiCam_Link2_DriverData_PmicPosition;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_VoltageRail        = MipiCamSetupData.MipiCam_Link2_DriverData_VoltageRail;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_PhyConfiguration   = MipiCamSetupData.MipiCam_Link2_DriverData_PhyConfiguration;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_LaneConfiguration  = MipiCamSetupData.MipiCam_Link2_DriverData_LaneConfiguration;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2_FlashDriverSelection = MipiCamSetupData.MipiCam_Link2_FlashDriverSelection;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_FlashID            = MipiCamSetupData.MipiCam_Link2_DriverData_FlashID;

#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 0x1
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_PprValue = MipiCamSetupData.MipiCam_Link2_DriverData_PprValue;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_PprUnit = MipiCamSetupData.MipiCam_Link2_DriverData_PprUnit;
#endif

  // MipiCam Link3 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3SensorModel = MipiCamSetupData.MipiCam_Link3_SensorModel;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3Pld         = MipiCamSetupData.MipiCam_Link3_CameraPhysicalLocation;
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  // Need to copy to temporary aligned buffer due to pack(1) in MipiCamConfig.h
  CopyMem (MipiModuleName,MipiCamSetupData.MipiCam_Link3_ModuleName, sizeof (MipiModuleName));
  UnicodeStrToAsciiStrS (MipiModuleName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink3ModuleName, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamLink3ModuleName));
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  CopyMem (MipiHidName,MipiCamSetupData.MipiCam_Link3_UserHid, sizeof (MipiHidName));
  UnicodeStrToAsciiStrS (MipiHidName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink3UserHid, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamLink3UserHid));
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cDevicesEnabled = MipiCamSetupData.MipiCam_Link3_I2cDevicesEnabled;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cBus            = MipiCamSetupData.MipiCam_Link3_I2cChannel;

  // MipiCam Link3 I2C Devices
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cAddrDev[Index]    = MipiCamSetupData.MipiCam_Link3_I2cAddress[Index];
    switch (MipiCamSetupData.MipiCam_Link3_I2cDeviceType[Index]) {
      case MIPICAM_CUSTOMIZE_DEVICE1:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link3_Customize_Id[0];
        break;
      case MIPICAM_CUSTOMIZE_DEVICE2:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link3_Customize_Id[1];
        break;
      case MIPICAM_CUSTOMIZE_DEVICE3:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link3_Customize_Id[2];
        break;
      default:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link3_I2cDeviceType[Index];
    }
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_Version     = 1;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_CrdVersion  = MipiCamSetupData.MipiCam_Link3_DriverData_CrdVersion;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_LinkUsed    = MipiCamSetupData.MipiCam_Link3_DriverData_LinkUsed;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_LaneUsed    = MipiCamSetupData.MipiCam_Link3_DriverData_LaneUsed;
  if (MipiCamSetupData.MipiCam_Link3_DriverData_EepromType == 0xFF) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_EepromType = MipiCamSetupData.MipiCam_Link3_DriverData_EepromTypeCustom;
  } else {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_EepromType = MipiCamSetupData.MipiCam_Link3_DriverData_EepromType;
  }
  if (MipiCamSetupData.MipiCam_Link3_DriverData_VcmType == 0xFF) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_VcmType = MipiCamSetupData.MipiCam_Link3_DriverData_VcmTypeCustom;
  } else {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_VcmType = MipiCamSetupData.MipiCam_Link3_DriverData_VcmType;
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_Mclk               = MipiCamSetupData.MipiCam_Link3_DriverData_Mclk;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_ControlLogic       = MipiCamSetupData.MipiCam_Link3_DriverData_ControlLogic;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_FlashSupport       = MipiCamSetupData.MipiCam_Link3_DriverData_FlashSupport;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_PrivacyLed         = MipiCamSetupData.MipiCam_Link3_DriverData_PrivacyLed;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_Degree             = MipiCamSetupData.MipiCam_Link3_DriverData_Degree;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_PmicPosition       = MipiCamSetupData.MipiCam_Link3_DriverData_PmicPosition;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_VoltageRail        = MipiCamSetupData.MipiCam_Link3_DriverData_VoltageRail;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_PhyConfiguration   = MipiCamSetupData.MipiCam_Link3_DriverData_PhyConfiguration;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_LaneConfiguration  = MipiCamSetupData.MipiCam_Link3_DriverData_LaneConfiguration;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3_FlashDriverSelection = MipiCamSetupData.MipiCam_Link3_FlashDriverSelection;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_FlashID            = MipiCamSetupData.MipiCam_Link3_DriverData_FlashID;

#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 0x1
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_PprValue = MipiCamSetupData.MipiCam_Link3_DriverData_PprValue;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_PprUnit = MipiCamSetupData.MipiCam_Link3_DriverData_PprUnit;
#endif

  // MipiCam Link4 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4SensorModel = MipiCamSetupData.MipiCam_Link4_SensorModel;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4Pld         = MipiCamSetupData.MipiCam_Link4_CameraPhysicalLocation;
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  // Need to copy to temporary aligned buffer due to pack(1) in MipiCamConfig.h
  CopyMem (MipiModuleName,MipiCamSetupData.MipiCam_Link4_ModuleName, sizeof (MipiModuleName));
  UnicodeStrToAsciiStrS (MipiModuleName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink4ModuleName, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamLink4ModuleName));
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  CopyMem (MipiHidName,MipiCamSetupData.MipiCam_Link4_UserHid, sizeof (MipiHidName));
  UnicodeStrToAsciiStrS (MipiHidName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink4UserHid, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamLink4UserHid));
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cDevicesEnabled = MipiCamSetupData.MipiCam_Link4_I2cDevicesEnabled;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cBus = MipiCamSetupData.MipiCam_Link4_I2cChannel;

  // MipiCam Link4 I2C Devices
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cAddrDev[Index]    = MipiCamSetupData.MipiCam_Link4_I2cAddress[Index];
    switch (MipiCamSetupData.MipiCam_Link4_I2cDeviceType[Index]) {
      case MIPICAM_CUSTOMIZE_DEVICE1:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link4_Customize_Id[0];
        break;
      case MIPICAM_CUSTOMIZE_DEVICE2:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link4_Customize_Id[1];
        break;
      case MIPICAM_CUSTOMIZE_DEVICE3:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link4_Customize_Id[2];
        break;
      default:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link4_I2cDeviceType[Index];
    }
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_Version     = 1;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_CrdVersion  = MipiCamSetupData.MipiCam_Link4_DriverData_CrdVersion;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_LinkUsed    = MipiCamSetupData.MipiCam_Link4_DriverData_LinkUsed;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_LaneUsed    = MipiCamSetupData.MipiCam_Link4_DriverData_LaneUsed;
  if (MipiCamSetupData.MipiCam_Link4_DriverData_EepromType == 0xFF) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_EepromType = MipiCamSetupData.MipiCam_Link4_DriverData_EepromTypeCustom;
  } else {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_EepromType = MipiCamSetupData.MipiCam_Link4_DriverData_EepromType;
  }
  if (MipiCamSetupData.MipiCam_Link4_DriverData_VcmType == 0xFF) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_VcmType = MipiCamSetupData.MipiCam_Link4_DriverData_VcmTypeCustom;
  } else {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_VcmType = MipiCamSetupData.MipiCam_Link4_DriverData_VcmType;
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_Mclk               = MipiCamSetupData.MipiCam_Link4_DriverData_Mclk;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_ControlLogic       = MipiCamSetupData.MipiCam_Link4_DriverData_ControlLogic;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_FlashSupport       = MipiCamSetupData.MipiCam_Link4_DriverData_FlashSupport;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_PrivacyLed         = MipiCamSetupData.MipiCam_Link4_DriverData_PrivacyLed;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_Degree             = MipiCamSetupData.MipiCam_Link4_DriverData_Degree;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_PmicPosition       = MipiCamSetupData.MipiCam_Link4_DriverData_PmicPosition;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_VoltageRail        = MipiCamSetupData.MipiCam_Link4_DriverData_VoltageRail;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_PhyConfiguration   = MipiCamSetupData.MipiCam_Link4_DriverData_PhyConfiguration;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_LaneConfiguration  = MipiCamSetupData.MipiCam_Link4_DriverData_LaneConfiguration;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4_FlashDriverSelection = MipiCamSetupData.MipiCam_Link4_FlashDriverSelection;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_FlashID            = MipiCamSetupData.MipiCam_Link4_DriverData_FlashID;

#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 0x1
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_PprValue = MipiCamSetupData.MipiCam_Link4_DriverData_PprValue;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_PprUnit = MipiCamSetupData.MipiCam_Link4_DriverData_PprUnit;
#endif

  // MipiCam Link5 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5SensorModel = MipiCamSetupData.MipiCam_Link5_SensorModel;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5Pld         = MipiCamSetupData.MipiCam_Link5_CameraPhysicalLocation;
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  // Need to copy to temporary aligned buffer due to pack(1) in MipiCamConfig.h
  CopyMem (MipiModuleName,MipiCamSetupData.MipiCam_Link5_ModuleName, sizeof (MipiModuleName));
  UnicodeStrToAsciiStrS (MipiModuleName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink5ModuleName, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamLink5ModuleName));
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  CopyMem (MipiHidName,MipiCamSetupData.MipiCam_Link5_UserHid, sizeof (MipiHidName));
  UnicodeStrToAsciiStrS (MipiHidName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink5UserHid, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamLink5UserHid));
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cDevicesEnabled = MipiCamSetupData.MipiCam_Link5_I2cDevicesEnabled;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cBus = MipiCamSetupData.MipiCam_Link5_I2cChannel;

  // MipiCam Link5 I2C Devices
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cAddrDev[Index]    = MipiCamSetupData.MipiCam_Link5_I2cAddress[Index];
    switch (MipiCamSetupData.MipiCam_Link5_I2cDeviceType[Index]) {
      case MIPICAM_CUSTOMIZE_DEVICE1:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link5_Customize_Id[0];
        break;
      case MIPICAM_CUSTOMIZE_DEVICE2:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link5_Customize_Id[1];
        break;
      case MIPICAM_CUSTOMIZE_DEVICE3:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link5_Customize_Id[2];
        break;
      default:
        mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cDeviceType[Index] = MipiCamSetupData.MipiCam_Link5_I2cDeviceType[Index];
    }
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_Version     = 1;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_CrdVersion  = MipiCamSetupData.MipiCam_Link5_DriverData_CrdVersion;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_LinkUsed    = MipiCamSetupData.MipiCam_Link5_DriverData_LinkUsed;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_LaneUsed    = MipiCamSetupData.MipiCam_Link5_DriverData_LaneUsed;
  if (MipiCamSetupData.MipiCam_Link5_DriverData_EepromType == 0xFF) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_EepromType = MipiCamSetupData.MipiCam_Link5_DriverData_EepromTypeCustom;
  } else {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_EepromType = MipiCamSetupData.MipiCam_Link5_DriverData_EepromType;
  }
  if (MipiCamSetupData.MipiCam_Link5_DriverData_VcmType == 0xFF) {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_VcmType = MipiCamSetupData.MipiCam_Link5_DriverData_VcmTypeCustom;
  } else {
    mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_VcmType = MipiCamSetupData.MipiCam_Link5_DriverData_VcmType;
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_Mclk               = MipiCamSetupData.MipiCam_Link5_DriverData_Mclk;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_ControlLogic       = MipiCamSetupData.MipiCam_Link5_DriverData_ControlLogic;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_FlashSupport       = MipiCamSetupData.MipiCam_Link5_DriverData_FlashSupport;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_PrivacyLed         = MipiCamSetupData.MipiCam_Link5_DriverData_PrivacyLed;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_Degree             = MipiCamSetupData.MipiCam_Link5_DriverData_Degree;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_PmicPosition       = MipiCamSetupData.MipiCam_Link5_DriverData_PmicPosition;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_VoltageRail        = MipiCamSetupData.MipiCam_Link5_DriverData_VoltageRail;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_PhyConfiguration   = MipiCamSetupData.MipiCam_Link5_DriverData_PhyConfiguration;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_LaneConfiguration  = MipiCamSetupData.MipiCam_Link5_DriverData_LaneConfiguration;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5_FlashDriverSelection = MipiCamSetupData.MipiCam_Link5_FlashDriverSelection;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_FlashID            = MipiCamSetupData.MipiCam_Link5_DriverData_FlashID;

#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 0x1
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_PprValue = MipiCamSetupData.MipiCam_Link5_DriverData_PprValue;
  mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_PprUnit = MipiCamSetupData.MipiCam_Link5_DriverData_PprUnit;
#endif

  // MipiCam Flash0 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash0Model                = MipiCamSetupData.MipiCam_Flash0_Model;
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  CopyMem (MipiHidName, MipiCamSetupData.MipiCam_Flash0_UserHid, sizeof (MipiHidName));
  UnicodeStrToAsciiStrS (MipiHidName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash0UserHid, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamFlash0UserHid));
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash0Mode                 = MipiCamSetupData.MipiCam_Flash0_Mode;
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  // Need to copy to temporary aligned buffer due to pack(1) in SetupVariables.h
  CopyMem (MipiModuleName, MipiCamSetupData.MipiCam_Flash0_ModuleName, sizeof (MipiModuleName));
  UnicodeStrToAsciiStrS (MipiModuleName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash0ModuleName, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamFlash0ModuleName));
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash0I2cBus               = MipiCamSetupData.MipiCam_Flash0_I2cChannel;
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash0I2cAddrDev           = MipiCamSetupData.MipiCam_Flash0_I2cAddress;
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioSupport          = MipiCamSetupData.MipiCam_Flash0_GpioSupport;
  if (MipiCamSetupData.MipiCam_Flash0_GpioSupport == 1) {
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioGroupPadNumber = MipiCamSetupData.MipiCam_Flash0_GpioGroupPadNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioComNumber      = MipiCamSetupData.MipiCam_Flash0_GpioComNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioGroupNumber    = MipiCamSetupData.MipiCam_Flash0_GpioGroupNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioActiveValue    = MipiCamSetupData.MipiCam_Flash0_GpioActiveValue;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioInitialValue   = MipiCamSetupData.MipiCam_Flash0_GpioInitialValue;
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash0OperatingMode        = MipiCamSetupData.MipiCam_Flash0_OperatingMode;

  // MipiCam Flash1 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash1Model                = MipiCamSetupData.MipiCam_Flash1_Model;
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  CopyMem (MipiHidName,MipiCamSetupData.MipiCam_Flash1_UserHid, sizeof (MipiHidName));
  UnicodeStrToAsciiStrS (MipiHidName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash1UserHid, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamFlash1UserHid));
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash1Mode                 = MipiCamSetupData.MipiCam_Flash1_Mode;
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  // Need to copy to temporary aligned buffer due to pack(1) in SetupVariables.h
  CopyMem (MipiModuleName, MipiCamSetupData.MipiCam_Flash1_ModuleName, sizeof (MipiModuleName));
  UnicodeStrToAsciiStrS (MipiModuleName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash1ModuleName, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamFlash1ModuleName));
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash1I2cBus               = MipiCamSetupData.MipiCam_Flash1_I2cChannel;
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash1I2cAddrDev           = MipiCamSetupData.MipiCam_Flash1_I2cAddress;
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioSupport          = MipiCamSetupData.MipiCam_Flash1_GpioSupport;
  if (MipiCamSetupData.MipiCam_Flash1_GpioSupport == 1) {
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioGroupPadNumber = MipiCamSetupData.MipiCam_Flash1_GpioGroupPadNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioComNumber      = MipiCamSetupData.MipiCam_Flash1_GpioComNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioGroupNumber    = MipiCamSetupData.MipiCam_Flash1_GpioGroupNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioActiveValue    = MipiCamSetupData.MipiCam_Flash1_GpioActiveValue;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioInitialValue   = MipiCamSetupData.MipiCam_Flash1_GpioInitialValue;
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash1OperatingMode        = MipiCamSetupData.MipiCam_Flash1_OperatingMode;

  // MipiCam Flash2 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash2Model                = MipiCamSetupData.MipiCam_Flash2_Model;
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  CopyMem (MipiHidName,MipiCamSetupData.MipiCam_Flash2_UserHid, sizeof (MipiHidName));
  UnicodeStrToAsciiStrS (MipiHidName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash2UserHid, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamFlash2UserHid));
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash2Mode                 = MipiCamSetupData.MipiCam_Flash2_Mode;
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  // Need to copy to temporary aligned buffer due to pack(1) in SetupVariables.h
  CopyMem (MipiModuleName, MipiCamSetupData.MipiCam_Flash2_ModuleName, sizeof (MipiModuleName));
  UnicodeStrToAsciiStrS (MipiModuleName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash2ModuleName, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamFlash2ModuleName));
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash2I2cBus               = MipiCamSetupData.MipiCam_Flash2_I2cChannel;
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash2I2cAddrDev           = MipiCamSetupData.MipiCam_Flash2_I2cAddress;
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioSupport          = MipiCamSetupData.MipiCam_Flash2_GpioSupport;
  if (MipiCamSetupData.MipiCam_Flash2_GpioSupport == 1) {
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioGroupPadNumber = MipiCamSetupData.MipiCam_Flash2_GpioGroupPadNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioComNumber      = MipiCamSetupData.MipiCam_Flash2_GpioComNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioGroupNumber    = MipiCamSetupData.MipiCam_Flash2_GpioGroupNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioActiveValue    = MipiCamSetupData.MipiCam_Flash2_GpioActiveValue;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioInitialValue   = MipiCamSetupData.MipiCam_Flash2_GpioInitialValue;
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash2OperatingMode        = MipiCamSetupData.MipiCam_Flash2_OperatingMode;

  // MipiCam Flash3 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash3Model                = MipiCamSetupData.MipiCam_Flash3_Model;
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  CopyMem (MipiHidName,MipiCamSetupData.MipiCam_Flash3_UserHid, sizeof (MipiHidName));
  UnicodeStrToAsciiStrS (MipiHidName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash3UserHid, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamFlash3UserHid));
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash3Mode                 = MipiCamSetupData.MipiCam_Flash3_Mode;
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  // Need to copy to temporary aligned buffer due to pack(1) in SetupVariables.h
  CopyMem (MipiModuleName, MipiCamSetupData.MipiCam_Flash3_ModuleName, sizeof (MipiModuleName));
  UnicodeStrToAsciiStrS (MipiModuleName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash3ModuleName, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamFlash3ModuleName));
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash3I2cBus               = MipiCamSetupData.MipiCam_Flash3_I2cChannel;
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash3I2cAddrDev           = MipiCamSetupData.MipiCam_Flash3_I2cAddress;
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioSupport          = MipiCamSetupData.MipiCam_Flash3_GpioSupport;
  if (MipiCamSetupData.MipiCam_Flash3_GpioSupport == 1) {
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioGroupPadNumber = MipiCamSetupData.MipiCam_Flash3_GpioGroupPadNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioComNumber      = MipiCamSetupData.MipiCam_Flash3_GpioComNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioGroupNumber    = MipiCamSetupData.MipiCam_Flash3_GpioGroupNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioActiveValue    = MipiCamSetupData.MipiCam_Flash3_GpioActiveValue;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioInitialValue   = MipiCamSetupData.MipiCam_Flash3_GpioInitialValue;
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash3OperatingMode        = MipiCamSetupData.MipiCam_Flash3_OperatingMode;

  // MipiCam Flash4 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash4Model                = MipiCamSetupData.MipiCam_Flash4_Model;
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  CopyMem (MipiHidName,MipiCamSetupData.MipiCam_Flash4_UserHid, sizeof (MipiHidName));
  UnicodeStrToAsciiStrS (MipiHidName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash4UserHid, sizeof(mMipiCamNvsAreaProtocol.Area->MipiCamFlash4UserHid));
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash4Mode                 = MipiCamSetupData.MipiCam_Flash4_Mode;
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  // Need to copy to temporary aligned buffer due to pack(1) in SetupVariables.h
  CopyMem (MipiModuleName, MipiCamSetupData.MipiCam_Flash4_ModuleName, sizeof (MipiModuleName));
  UnicodeStrToAsciiStrS (MipiModuleName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash4ModuleName, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamFlash4ModuleName));
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash4I2cBus               = MipiCamSetupData.MipiCam_Flash4_I2cChannel;
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash4I2cAddrDev           = MipiCamSetupData.MipiCam_Flash4_I2cAddress;
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioSupport          = MipiCamSetupData.MipiCam_Flash4_GpioSupport;
  if (MipiCamSetupData.MipiCam_Flash4_GpioSupport == 1) {
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioGroupPadNumber = MipiCamSetupData.MipiCam_Flash4_GpioGroupPadNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioComNumber      = MipiCamSetupData.MipiCam_Flash4_GpioComNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioGroupNumber    = MipiCamSetupData.MipiCam_Flash4_GpioGroupNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioActiveValue    = MipiCamSetupData.MipiCam_Flash4_GpioActiveValue;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioInitialValue   = MipiCamSetupData.MipiCam_Flash4_GpioInitialValue;
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash4OperatingMode        = MipiCamSetupData.MipiCam_Flash4_OperatingMode;
  // MipiCam Flash5 specific
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash5Model                = MipiCamSetupData.MipiCam_Flash5_Model;
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  CopyMem (MipiHidName,MipiCamSetupData.MipiCam_Flash5_UserHid, sizeof (MipiHidName));
  UnicodeStrToAsciiStrS (MipiHidName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash5UserHid, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamFlash5UserHid));
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash5Mode                 = MipiCamSetupData.MipiCam_Flash5_Mode;
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  // Need to copy to temporary aligned buffer due to pack(1) in SetupVariables.h
  CopyMem (MipiModuleName, MipiCamSetupData.MipiCam_Flash5_ModuleName, sizeof (MipiModuleName));
  UnicodeStrToAsciiStrS (MipiModuleName, (CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash5ModuleName, sizeof (mMipiCamNvsAreaProtocol.Area->MipiCamFlash5ModuleName));
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash5I2cBus               = MipiCamSetupData.MipiCam_Flash5_I2cChannel;
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash5I2cAddrDev           = MipiCamSetupData.MipiCam_Flash5_I2cAddress;
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioSupport          = MipiCamSetupData.MipiCam_Flash5_GpioSupport;
  if (MipiCamSetupData.MipiCam_Flash5_GpioSupport == 1) {
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioGroupPadNumber = MipiCamSetupData.MipiCam_Flash5_GpioGroupPadNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioGroupNumber    = MipiCamSetupData.MipiCam_Flash5_GpioGroupNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioComNumber      = MipiCamSetupData.MipiCam_Flash5_GpioComNumber;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioActiveValue    = MipiCamSetupData.MipiCam_Flash5_GpioActiveValue;
    mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioInitialValue   = MipiCamSetupData.MipiCam_Flash5_GpioInitialValue;
  }
  mMipiCamNvsAreaProtocol.Area->MipiCamFlash5OperatingMode        = MipiCamSetupData.MipiCam_Flash5_OperatingMode;

  DEBUG ((DEBUG_INFO, "Mipi Camera: Initializing acpi space\n"));

  //
      // HDAudio Configuration
      //
#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 0x1
mMipiCamNvsAreaProtocol.Area->I2SE = MipiCamSetupData.MipiCam_ControlLogic0 || \
MipiCamSetupData.MipiCam_ControlLogic1 || \
MipiCamSetupData.MipiCam_ControlLogic2 || \
MipiCamSetupData.MipiCam_ControlLogic3 || \
MipiCamSetupData.MipiCam_ControlLogic4 || \
MipiCamSetupData.MipiCam_ControlLogic5;
if (mMipiCamNvsAreaProtocol.Area->I2SE)
{
VariableSize = sizeof(NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE);
ZeroMem(&NhltConfigurationEfiVariable, sizeof(NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE)); // Initialize the structure
Status = gRT->GetVariable(
    NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_NAME,
    &gNhltEndpointsTableConfigurationVariableGuid,
    NULL,
    &VariableSize,
    &NhltConfigurationEfiVariable
);
if (!EFI_ERROR(Status))
{
    if ((0 == NhltConfigurationEfiVariable.NhltI2sLontiumI2s0) &&
        (0 == NhltConfigurationEfiVariable.NhltI2sLontiumI2s2))
    {
        mMipiCamNvsAreaProtocol.Area->I2SE = 0;
    }
}
}
#else
mMipiCamNvsAreaProtocol.Area->I2SE = 0;
#endif

  ///
  /// Get MipiCamera Config Hob
  ///
  GuidHob = GetFirstGuidHob (&gMipiCamConfigHobGuid);
  if (GuidHob != NULL) {
    /// Update PlatformId with HOB exist
    MipiCamConfigDxeHob = GET_GUID_HOB_DATA (GuidHob);
    mMipiCamNvsAreaProtocol.Area->MipiCamPlatformId      = MipiCamConfigDxeHob->PlatformId;
    mMipiCamNvsAreaProtocol.Area->MipiCamEmulationConfig = MipiCamConfigDxeHob->EmulationConfig;
    mMipiCamNvsAreaProtocol.Area->MipiCamChipsetId       = MipiCamConfigDxeHob->PlatformChipId;
  }
  ///
  /// Load the SSDT ACPI Tables.
  ///
  DEBUG ((DEBUG_INFO, "Mipi Camera: LoadAcpiTables\n"));
  LoadAcpiTables ();

  DEBUG ((DEBUG_INFO, "Mipi Camera: Exit\n"));
  return Status;

}

EFI_STATUS
EFIAPI
LoadAcpiTables (
  VOID
  )
/**
@brief
  This procedure loads the ACPI SSDT tables.

**/
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         NumberOfHandles;
  UINTN                         Index;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  INTN                          Instance;
  EFI_ACPI_COMMON_HEADER        *Table;
  UINTN                         Size;
  EFI_FV_FILETYPE               FileType;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  UINT32                        FvStatus;
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader;
  UINTN                         TableHandle;
  BOOLEAN                       LoadTable;
  UINT8                         *CurrPtr;
  UINT32                        *Signature;
  UINT8                         PchScopeNumber;
  MIPICAM_CONFIG_HOB_EXT        *MipiCamConfigDxeHobExt;
  EFI_HOB_GUID_TYPE             *GuidHob;
  UINT32                        MipiPchScope;

  FwVol = NULL;
  Table = NULL;
  PchScopeNumber = 0;                                    // Default - Mobile only platform

  GuidHob = GetFirstGuidHob (&gMipiCamConfigHobExtGuid); // Get Extend Hob
  if (GuidHob != NULL) {
    MipiCamConfigDxeHobExt = GET_GUID_HOB_DATA (GuidHob);
    PchScopeNumber = MipiCamConfigDxeHobExt->PchScopeNum;
  }
  MipiPchScope = SIGNATURE_32 ('P', 'C', '0', ('0' + PchScopeNumber));

  ///
  /// Locate FV protocol.
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "MIPICAM: No Efi Firmware Volume Protocol available.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "MIPICAM: Efi Firmware Volume Protocol is loaded.\n"));
  }
  ASSERT_EFI_ERROR (Status);

  ///
  /// Look for FV with ACPI storage file
  ///
  for (Index = 0; Index < NumberOfHandles; Index++) {
    ///
    /// Get the protocol on this handle
    /// This should not fail because of LocateHandleBuffer
    ///
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **) &FwVol
                    );
    ASSERT_EFI_ERROR (Status);

    if ((Status == EFI_SUCCESS) && (FwVol != NULL)) {
      ///
      /// See if it has the ACPI storage file
      ///
      Size      = 0;
      FvStatus  = 0;
      Status = FwVol->ReadFile (
                        FwVol,
                        &gMipiCamAcpiTableStorageGuid,
                        NULL,
                        &Size,
                        &FileType,
                        &Attributes,
                        &FvStatus
                        );

      ///
      /// If we found it, then we are done
      ///
      if (Status == EFI_SUCCESS) {
        DEBUG ((DEBUG_INFO, "MIPICAM: Mipi Camera Acpi Table Storage for RVP is found.\n"));
        break;
      }
    }
  }

  ///
  /// Our exit status is determined by the success of the previous operations
  /// If the protocol was found, Instance already points to it.
  ///
  /// Free any allocated buffers
  ///
  FreePool (HandleBuffer);

  ///
  /// Sanity check that we found our data file
  ///
  ASSERT (FwVol);
  if (FwVol == NULL) {
    return EFI_NOT_FOUND;
  }

  ///
  /// Find the Table protocol
  ///
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);

  ///
  /// Read tables from the storage file.
  ///
  Instance = 0;

  while (Status == EFI_SUCCESS) {
    ///
    /// Read the ACPI tables
    ///
    Status = FwVol->ReadSection (
                      FwVol,
                      &gMipiCamAcpiTableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &Table,
                      &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR (Status)) {
      ///
      /// Check the table size is at least as large as an EFI_ACPI_COMMON_HEADER
      ///
      if (Size < sizeof (EFI_ACPI_COMMON_HEADER)) {
        if(Table != NULL) {
          FreePool (Table);
          Table = NULL;
          }
        return EFI_BUFFER_TOO_SMALL;
      }

      LoadTable = FALSE;
      TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;

      switch (((EFI_ACPI_DESCRIPTION_HEADER*) TableHeader)->OemTableId) {

      case SIGNATURE_64 ('M','i', 'C','a', 'T','a','b','l'):
        ///
        /// This is Mipi Camera table.
        ///
        LoadTable = TRUE;
        DEBUG ((DEBUG_INFO, "Mipi Camera: Found Mipi Camera SSDT signature.\n"));

        // Update the MipiCamNvs.asl Operation Region to the allocated address
        for (CurrPtr = (UINT8 *) TableHeader + sizeof (EFI_ACPI_DESCRIPTION_HEADER); CurrPtr < ((UINT8 *) TableHeader + TableHeader->Length) - sizeof(*Signature) ; CurrPtr++) {
          Signature = (UINT32 *) (CurrPtr + 1);
          if (*Signature == SIGNATURE_32 ('P', 'C', '0', 'X')) {  //Update PchScope
            *Signature = MipiPchScope;
            CurrPtr += sizeof (UINT32);
            continue;
          }
          if ((*CurrPtr == AML_EXT_REGION_OP) && (*Signature == SIGNATURE_32 ('M', 'N', 'V', 'S'))) {
            DEBUG ((DEBUG_INFO, "MipiCam: Update NVS Area %X\n",*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) ));
            DEBUG ((DEBUG_INFO, "MipiCam: Update NVS Area %X\n",*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) ));
            ASSERT (*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) == 0xFFFF0000);
            if (*(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) != 0xFFFF0000) {
              return EFI_NOT_FOUND;
            }
            ASSERT (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) == 0xAA55);
            if (*(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) != 0xAA55) {
              return EFI_NOT_FOUND;
            }
            ///
            /// MipiCam NVS Area address
            ///
            *(UINT32 *) (CurrPtr + 1 + sizeof (*Signature) + 2) = (UINT32) (UINTN) mMipiCamNvsAreaProtocol.Area;
            ///
            /// MipiCam NVS Area size
            ///
            *(UINT16 *) (CurrPtr + 1 + sizeof (*Signature) + 2 + sizeof (UINT32) + 1) = sizeof (MIPICAM_NVS_AREA);
            DEBUG ((DEBUG_INFO, "MipiCam: Update NVS Area\n"));
          }
        }
        break;
      default:
        break;
      }

      ///
      /// Add the table
      ///
      if (LoadTable) {
        TableHandle = 0;
        ///
        /// Check the length field isn't larger than the size read in section
        ///
        if (Table->Length > Size) {
          FreePool (Table);
          Table = NULL;
          return EFI_BAD_BUFFER_SIZE;
        }
        Status = AcpiTable->InstallAcpiTable (
                              AcpiTable,
                              Table,
                              Table->Length,
                              &TableHandle
                              );
      }

      ///
      /// Increment the instance
      ///
      Instance++;
      FreePool (Table);
      Table = NULL;
    }

  }
  return EFI_SUCCESS;
}
