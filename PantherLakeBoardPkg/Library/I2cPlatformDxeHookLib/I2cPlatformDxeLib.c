/** @file
  I2CPlatformDxe constructor to initialize the PCDs consumed in I2CPlatformDxe driver.

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

@par Specification Reference:
**/

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <SetupVariable.h>
#include <I2cPlatformDxe.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PchInfoLib.h>
#include <Library/LpssI2cSocLib.h>

#define I2C_TARGET_ADDRESS_SENSOR_HUB_1             0x40
#define I2C_TARGET_ADDRESS_SENSOR_HUB_2             0x41
#define I2C_TARGET_ADDRESS_AUDIO_CODEC_1            0x1C   // Realtek ALC298

#define I2C_TARGET_ADDRESS_ATMEL_3432_TOUCHPANEL    0x4C
#define I2C_TARGET_ADDRESS_ATMEL_2952_TOUCHPANEL    0x4A
#define I2C_TARGET_ADDRESS_ELAN_9048_TOUCHPANEL     0x10
#define I2C_TARGET_ADDRESS_NTRIG_SAMSUNG_TOUCHPANEL 0x07
#define I2C_TARGET_ADDRESS_NTRIG_SHARP_TOUCHPANEL   0x64
#define I2C_TARGET_ADDRESS_WACOM_PANEL              0x0A
#define I2C_TARGET_ADDRESS_ELAN_8E18_TOUCHPANEL     0x16

#define DEVICE_ID_TOUCHPANEL        1
#define TOUCHPANEL_HW_REVISION      1
#define DEVICE_ID_PSS_MONZA         2
#define PSS_MONZA_HW_REVISION       1
#define BUS_CONFIG_0                0
#define BUS_CONFIG_4                4
#define BUS_CONFIG_5                5
#define I2C_PLATFORM_DRIVER_VERSION 0
#define DEVICE_END                  0

/**
  Retrieves I2c address of the touchpanel model selected in BIOS setup
**/
UINT16
GetTouchpanelI2cAddress (
  VOID
  )
{
  SETUP_DATA              SetupData;
  EFI_STATUS              Status;
  UINTN                   Size;

  Size = sizeof (SETUP_DATA);
  ZeroMem (&SetupData, sizeof (SETUP_DATA));
  Status = gRT->GetVariable (L"Setup", &gSetupVariableGuid, NULL, &Size, &SetupData);
  if (EFI_ERROR (Status)) {
    return 0xFFFF;
  }
  switch(SetupData.PchI2cTouchPanelType) {
    case 1:
      return I2C_TARGET_ADDRESS_ATMEL_3432_TOUCHPANEL;
    case 2:
      return I2C_TARGET_ADDRESS_ATMEL_2952_TOUCHPANEL;
    case 3:
      return I2C_TARGET_ADDRESS_ELAN_9048_TOUCHPANEL;
    case 4:
      return I2C_TARGET_ADDRESS_NTRIG_SAMSUNG_TOUCHPANEL;
    case 5:
      return I2C_TARGET_ADDRESS_NTRIG_SHARP_TOUCHPANEL;
    case 6:
      return I2C_TARGET_ADDRESS_WACOM_PANEL;
    case 7:
      return SetupData.PchI2cTouchPanelBusAddress;
    case 8:
      return I2C_TARGET_ADDRESS_ELAN_8E18_TOUCHPANEL;
    default:
      return 0xFFFF;
  }
}

/**
  This function updates Bus speed for each controller
  and provides option to override based on platform setting

  @return The status of the set operation.
**/
EFI_STATUS
EFIAPI
UpdateI2cBusFrequency (
  VOID
  )
{
  SETUP_DATA               SetupData;
  EFI_STATUS               Status;
  UINTN                    Size;
  UINTN                    BusClockHertz;
  UINT8                    Index;
  I2C_FREQUENCY_DATA_TABLE I2cFrequencyTable;

  ZeroMem (&SetupData, sizeof (SETUP_DATA));

  // update total Serial I2C controller count
  I2cFrequencyTable.Count = GetMaxI2cInterfacesNum ();

  BusClockHertz = 100 * 1000;
  for (Index = 0; Index < I2cFrequencyTable.Count; Index ++) {
    I2cFrequencyTable.I2cFrequency[Index] = BusClockHertz;
  }

  // Override frequency value for I2C1 Bus as per Touchpanel setup option
  Size = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (L"Setup", &gSetupVariableGuid, NULL, &Size, &SetupData);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  if (SetupData.PchI2cTouchPanelType >= 6) {
      switch(SetupData.PchI2cTouchPanelSpeed) {
        case 0:
          BusClockHertz = 100 * 1000;
          break;
        case 1:
          BusClockHertz = 400 * 1000;
          break;
        case 2:
          BusClockHertz = 1000 * 1000;
          break;
        default:;
      }
     I2cFrequencyTable.I2cFrequency[1] = BusClockHertz;
    }

  // Override frequency value for I2CX Bus as per PSS Monza
  I2cFrequencyTable.I2cFrequency[PcdGet8 (VpdPcdPssI2cBusNumber)] = 400 * 1000;

  Size = (UINTN) sizeof (I2C_FREQUENCY_DATA_TABLE);

  // Update the PCD with Frequency of all serial I2C controller
  Status = PcdSetPtrS (PcdI2cFrequencyDataMap, &Size, &I2cFrequencyTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[I2CPlatformDxeHook] PcdSetPtrS of I2cFrequencyTable Status = %r  Size = %x\n", Status, Size));
  }

  return Status;
}

/**
  This function creates list of serial i2c device list under each controller
  , and updates it to Struc PCD
  Struc PCD  is consumed by I2cPlatformDxe driver to enumerate the device

  @return The error status of the GetVariable call.
  @return EFI_OUT_OF_RESOURCES  Memory allocation fails
  @return The status of the set operation.

**/
EFI_STATUS
EFIAPI
CreateDeviceList (
  VOID
  )
{
  SETUP_DATA                 SetupData;
  EFI_STATUS                 Status;
  UINTN                      Size;
  I2C_CONTROLLER_DEVICE_LIST *I2cControllerDevList;
  UINT32                     *Buffer;
  UINT8                      PssI2cBusNumber;

  I2cControllerDevList = AllocateZeroPool (sizeof (I2C_CONTROLLER_DEVICE_LIST));

  if (I2cControllerDevList == NULL) {
    DEBUG((DEBUG_ERROR, "[I2CPlatformDxeHook] Failed to allocate memory for I2C Controller Device List.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Size = sizeof (SETUP_DATA);
  ZeroMem (&SetupData, sizeof (SETUP_DATA));

  //
  // Assign I2C1 Settings
  //
  Status = gRT->GetVariable (L"Setup", &gSetupVariableGuid, NULL, &Size, &SetupData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Assign I2C4 Settings
  //
  if ((SetupData.PchI2cTouchPanelType != 0) && (SetupData.PchI2cSensorDevicePort[BUS_CONFIG_4] & SERIAL_IO_I2C_TOUCHPANEL)) {
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_4].I2cDeviceList[0].DeviceAddressCount    = 1;
    Buffer = AllocateZeroPool (sizeof (UINT32) * I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_4].I2cDeviceList[0].DeviceAddressCount);

    if (Buffer == NULL) {
      DEBUG((DEBUG_ERROR, "[I2CPlatformDxeHook] Failed to allocate memory for I2C4 Settings buffer.\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    Buffer[0] = GetTouchpanelI2cAddress ();
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_4].Count                                  = 2;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_4].I2cDeviceList[0].DeviceGuid            = (UINT64) AllocateCopyPool (sizeof (EFI_GUID), &gI2cTouchPanelDeviceGuid);
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_4].I2cDeviceList[0].DeviceIndex           = DEVICE_ID_TOUCHPANEL;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_4].I2cDeviceList[0].HardwareRevision      = TOUCHPANEL_HW_REVISION;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_4].I2cDeviceList[0].I2cBusConfiguration   = BUS_CONFIG_4;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_4].I2cDeviceList[0].DeviceAddressArray    = (UINT64) Buffer;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_4].I2cDeviceList[1].DeviceGuid            = DEVICE_END;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_4].I2cDeviceList[1].DeviceIndex           = DEVICE_END;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_4].I2cDeviceList[1].HardwareRevision      = DEVICE_END;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_4].I2cDeviceList[1].I2cBusConfiguration   = DEVICE_END;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_4].I2cDeviceList[1].DeviceAddressArray    = DEVICE_END;
  }

  //
  // Assign I2C5 Settings
  //
  if ((SetupData.PchI2cTouchPanelType != 0) && (SetupData.PchI2cSensorDevicePort[BUS_CONFIG_5] & SERIAL_IO_I2C_TOUCHPANEL)) {
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_5].I2cDeviceList[0].DeviceAddressCount    = 1;
    Buffer = AllocateZeroPool (sizeof (UINT32) * I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_5].I2cDeviceList[0].DeviceAddressCount);

    if (Buffer == NULL) {
      DEBUG((DEBUG_ERROR, "[I2CPlatformDxeHook] Failed to allocate memory for I2C5 Settings buffer.\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    Buffer[0] = GetTouchpanelI2cAddress ();
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_5].Count                                  = 2;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_5].I2cDeviceList[0].DeviceGuid            = (UINT64) AllocateCopyPool (sizeof (EFI_GUID), &gI2cTouchPanelDeviceGuid);
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_5].I2cDeviceList[0].DeviceIndex           = DEVICE_ID_TOUCHPANEL;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_5].I2cDeviceList[0].HardwareRevision      = TOUCHPANEL_HW_REVISION;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_5].I2cDeviceList[0].I2cBusConfiguration   = BUS_CONFIG_5;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_5].I2cDeviceList[0].DeviceAddressArray    = (UINT64) Buffer;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_5].I2cDeviceList[1].DeviceGuid            = DEVICE_END;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_5].I2cDeviceList[1].DeviceIndex           = DEVICE_END;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_5].I2cDeviceList[1].HardwareRevision      = DEVICE_END;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_5].I2cDeviceList[1].I2cBusConfiguration   = DEVICE_END;
    I2cControllerDevList->I2cControllerDeviceList[BUS_CONFIG_5].I2cDeviceList[1].DeviceAddressArray    = DEVICE_END;
  }

  PssI2cBusNumber = PcdGet8 (VpdPcdPssI2cBusNumber);

  //
  // Assign I2CX Settings for PSS
  //
  I2cControllerDevList->I2cControllerDeviceList[PssI2cBusNumber].I2cDeviceList[0].DeviceAddressCount    = 1;
  Buffer = AllocateZeroPool (sizeof (UINT32) * I2cControllerDevList->I2cControllerDeviceList[PssI2cBusNumber].I2cDeviceList[0].DeviceAddressCount);

  if (Buffer == NULL) {
    DEBUG((DEBUG_ERROR, "[I2CPlatformDxeHook] Failed to allocate memory for PSS I2CX Settings buffer.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Buffer[0] = PcdGet8 (VpdPcdPssI2cDeviceAddress);
  I2cControllerDevList->I2cControllerDeviceList[PssI2cBusNumber].Count                                  = 2;
  I2cControllerDevList->I2cControllerDeviceList[PssI2cBusNumber].I2cDeviceList[0].DeviceGuid            = (UINT64) AllocateCopyPool (sizeof (EFI_GUID), &gI2cPssMonzaDeviceGuid);
  I2cControllerDevList->I2cControllerDeviceList[PssI2cBusNumber].I2cDeviceList[0].DeviceIndex           = DEVICE_ID_PSS_MONZA;
  I2cControllerDevList->I2cControllerDeviceList[PssI2cBusNumber].I2cDeviceList[0].HardwareRevision      = PSS_MONZA_HW_REVISION;
  I2cControllerDevList->I2cControllerDeviceList[PssI2cBusNumber].I2cDeviceList[0].I2cBusConfiguration   = BUS_CONFIG_0;
  I2cControllerDevList->I2cControllerDeviceList[PssI2cBusNumber].I2cDeviceList[0].DeviceAddressArray    = (UINT64) Buffer;

  I2cControllerDevList->I2cControllerDeviceList[PssI2cBusNumber].I2cDeviceList[1].DeviceGuid            = DEVICE_END;
  I2cControllerDevList->I2cControllerDeviceList[PssI2cBusNumber].I2cDeviceList[1].DeviceIndex           = DEVICE_END;
  I2cControllerDevList->I2cControllerDeviceList[PssI2cBusNumber].I2cDeviceList[1].HardwareRevision      = DEVICE_END;
  I2cControllerDevList->I2cControllerDeviceList[PssI2cBusNumber].I2cDeviceList[1].I2cBusConfiguration   = DEVICE_END;
  I2cControllerDevList->I2cControllerDeviceList[PssI2cBusNumber].I2cDeviceList[1].DeviceAddressArray    = DEVICE_END;

  Size = (UINTN) sizeof (I2C_CONTROLLER_DEVICE_LIST);
  // Update the PCD with pointer to Device list for all the I2C controller
  Status = PcdSetPtrS (PcdI2cControllerDevList, &Size, I2cControllerDevList);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[I2CPlatformDxeHook] PcdSetPtrS of PcdI2cControllerDevList Status = %r  Size = %x\n", Status, Size));
  }

  return Status;
}

/**
  This function creates list of serial I2C device id list , and updates it to Structure PCD
  Structure PCD is consumed by I2cPlatformDxe driver for supported check

  @return The status of the set operation.
**/
EFI_STATUS
EFIAPI
UpdateSerialIoI2cDevicePcd (
  VOID
  )
{
  UINT8                   Index;
  EFI_STATUS              Status;
  UINTN                   Size;
  SERIAL_IO_I2C_DEVID     SerialIoI2cDevid;

  // update total Serial I2C controller count
  SerialIoI2cDevid.Count = GetMaxI2cInterfacesNum ();

  for (Index = 0; Index < SerialIoI2cDevid.Count; Index++) {
    SerialIoI2cDevid.SerialIoI2cDevId[Index] = GetLpssI2cDeviceId (Index);
  }

  Size = (UINTN) sizeof (SERIAL_IO_I2C_DEVID);
  // Update the PCD with Device ID of all Serial I2C controller
  Status = PcdSetPtrS (PcdSerialIoI2cDevId, &Size, &SerialIoI2cDevid);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "[I2CPlatformDxeHook] PcdSetPtrS of PcdSerialIoI2cDevId Status = %r  Size = %x\n", Status, Size));
  }

  return Status;
}

/**
  The constructor function to initialize the I2C PCD's consumed in I2cPlatformDxe driver.

  @retval     EFI_SUCCESS  This constructor always return EFI_SUCCESS.
                           It will ASSERT on errors.
**/
EFI_STATUS
EFIAPI
I2cPlatformDxeLibConstructor (
  VOID
  )
{
  EFI_STATUS              Status;

  Status = CreateDeviceList();
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "[I2CPlatformDxeHook] failed to create Device list Status = %r\n", Status));
  }
  ASSERT_EFI_ERROR (Status);

  Status = UpdateI2cBusFrequency();
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "[I2CPlatformDxeHook] failed to updade I2C controller frequency table Status = %r\n", Status));
  }
  ASSERT_EFI_ERROR (Status);

  Status = UpdateSerialIoI2cDevicePcd();
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "[I2CPlatformDxeHook] failed to updade I2c Device table Status = %r\n", Status));
  }
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;

}
