/**@file
  I2C Platform DXE driver Setting

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

#ifndef _I2C_PLATFORM_DXE_H_
#define _I2C_PLATFORM_DXE_H_

#include <Protocol/I2cEnumerate.h>
#include <Protocol/I2cBusConfigurationManagement.h>
#include <Protocol/I2cMaster.h>
#include <Protocol/DevicePath.h>

#define I2C_PLATFORM_SIGNATURE          SIGNATURE_32 ('I', '2', 'C', 'M')
#define I2C_PLATFORM_CONTEXT_FROM_ENUMERATION_PROTOCOL(a) CR (a, I2C_PLATFORM_CONTEXT, Enumerate, I2C_PLATFORM_SIGNATURE)
#define I2C_PLATFORM_CONTEXT_FROM_BUS_CFG_PROTOCOL(a) CR (a, I2C_PLATFORM_CONTEXT, BusConfiguration, I2C_PLATFORM_SIGNATURE)
#define I2C_PLATFORM_DRIVER_VERSION 0

#define DIM(x) (sizeof (x) / sizeof (x[0]))
#pragma pack(1)

#define I2C_MAX_PORTS 0x0F

typedef struct  {
  UINT16     Count;
  UINT64     I2cFrequency[I2C_MAX_PORTS];  // Frequency for each I2C port
} I2C_FREQUENCY_DATA_TABLE;

typedef struct  {
  UINT16     Count;
  UINT16     SerialIoI2cDevId[I2C_MAX_PORTS];  // Frequency for each I2C port
} SERIAL_IO_I2C_DEVID;

//
// Creating copy of the EFI_I2C_DEVICE as pointer and UINTN cant be used in Structure PCD
// SPCD limitation is captured in https://bugzilla.tianocore.org/show_bug.cgi?id=3379
// Original EFI_I2C_DEVICE Struc has CONST EFI_GUID *DeviceGuid which occupies 4 bytes if used
// in X32 Arch(PEI phase), but actually in X64 Arch the pointer size should be 8 bytes.
// Same is with the the another structure component CONST UINT32 *PeripheralAddressArray
// As our driver is DXE and will not be used in PEI phase ,modifing both pointer to
// UINT64 type to hold address data which will be copied to the Pointer in I2cPlatformDxe driver
//
typedef struct {
  ///
  /// Unique value assigned by the silicon manufacture or the third
  /// party I2C driver writer for the I2C part.  This value logically
  /// combines both the manufacture name and the I2C part number into
  /// a single value specified as a GUID.
  ///
  UINT64 DeviceGuid;

  ///
  /// Unique ID of the I2C part within the system
  ///
  UINT32 DeviceIndex;

  ///
  /// Hardware revision - ACPI _HRV value.  See the Advanced
  /// Configuration and Power Interface Specification, Revision 5.0
  /// for the field format and the Plug and play support for I2C
  /// web-page for restriction on values.
  ///
  /// http://www.acpi.info/spec.htm
  /// http://msdn.microsoft.com/en-us/library/windows/hardware/jj131711(v=vs.85).aspx
  ///
  UINT32 HardwareRevision;

  ///
  /// I2C bus configuration for the I2C device
  ///
  UINT32 I2cBusConfiguration;

  ///
  /// Number of peripheral addresses for the I2C device.
  ///
  UINT32 DeviceAddressCount;

  ///
  /// Pointer to the array of peripheral addresses for the I2C device.
  ///
  UINT64 DeviceAddressArray;
} EFI_I2C_DEVICE_PCD;

typedef struct  {
  UINT8                  Count;                         // Count of I2C Devices
  EFI_I2C_DEVICE_PCD     I2cDeviceList[I2C_MAX_PORTS];  // List of I2C devices
} I2C_DEVICE_LIST;

typedef struct  {
  I2C_DEVICE_LIST     I2cControllerDeviceList[I2C_MAX_PORTS];  // List of I2C devices in each controller
} I2C_CONTROLLER_DEVICE_LIST;

typedef struct {
  UINTN                                         Signature;
  EFI_HANDLE                                    ControllerHandle;
  EFI_I2C_ENUMERATE_PROTOCOL                    Enumerate;
  EFI_I2C_BUS_CONFIGURATION_MANAGEMENT_PROTOCOL BusConfiguration;
  UINTN                                         I2cNumber;
  I2C_CONTROLLER_DEVICE_LIST                    *I2cConDevList;
} I2C_PLATFORM_CONTEXT;
#pragma pack()

#endif
