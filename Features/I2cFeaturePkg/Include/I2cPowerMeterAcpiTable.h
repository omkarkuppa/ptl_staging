/**@file
  I2C Common driver file

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

#ifndef _I2C_POWER_METER_ACPI_TABLE_H
#define _I2C_POWER_METER_ACPI_TABLE_H

// Power meter structure
#define MAX_PM_DEVICE_STRING_LENGTH               31
#define POWERMETER_MAX_DEVICES                     8

#pragma pack(1)

typedef struct  {
  CHAR8    RailName[MAX_PM_DEVICE_STRING_LENGTH];          // Rail Name
  UINT16   ResistorValue;                                  // Rail Resistence value
} RAIL_DATA_PKG;

typedef struct  {
  UINT8         DeviceAddress;   // 7-bit Device Address
  RAIL_DATA_PKG RailDataPkg[4];  // Package info
} POWER_METER_CONFIG_PKG;

typedef struct  {
  UINT8   DeviceSize;               // No. of Power Meter Device
  UINT8   I2cBusNumber;             // I2C Bus Number
  POWER_METER_CONFIG_PKG PmConfigPkgArray[POWERMETER_MAX_DEVICES];
} POWER_METER_PKG;

/**
  Function used to fill I2c Buffer with I2C Rtd3 Structure.
  @param[in] Count            Maximum Pch I2C ports.
  @param[in] I2C_ACPI_DEV     I2C ACPI device structure.
**/
typedef
EFI_STATUS
(EFIAPI *I2C_PM_CONFIG) (
  POWER_METER_PKG             *PowerMtrData,
  UINT8                       PowerMeterEnable
);

struct _I2C_PM_CONFIG_PROTOCOL {
  I2C_PM_CONFIG   I2cPmData;
};

typedef struct _I2C_PM_CONFIG_PROTOCOL I2C_PM_CONFIG_PROTOCOL;

extern EFI_GUID gI2cPmUpdateProtocolGuid;

#pragma pack()

#endif // _I2C_POWER_METER_ACPI_TABLE_H
