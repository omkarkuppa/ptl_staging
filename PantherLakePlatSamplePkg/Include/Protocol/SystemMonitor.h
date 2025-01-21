/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#ifndef _SYSTEM_MONITOR_H_
#define _SYSTEM_MONITOR_H_


// Device Types
#define HECETA6x    0x01

// Company IDs
#define NATIONAL    0x01
#define SMSC        0x5C
#define ADI         0x41

//Number of Monitor Device Supported
#define MAX_MONITOR_DEVICE 0x03

#define EC_REG_TSR1_LSB                  145
#define EC_REG_TSR2_LSB                  147
#define EC_REG_TSR3_LSB                  149
#define EC_REG_TSR4_LSB                  151
#define EC_REG_TSR5_LSB                  153

#define EC_REG_CPU_FAN_SPEED             115
#define EC_REG_PCH_DTS_TEMP              129
#define EC_REG_TS_ON_DIMM0_TEMP          133
#define EC_REG_TS_ON_DIMM1_TEMP          134

typedef struct {
  UINT16    EC_TSR1;
  UINT16    EC_TSR2;
  UINT16    EC_TSR3;
  UINT16    EC_TSR4;
  UINT16    EC_TSR5;
  UINT16    EC_TSR6;
  UINT16    EC_TSR7;
  UINT16    EC_TSR8;

  UINT16    CPUFanSpeed;
  UINT8     PCHDTSTemp;
  UINT8     TSonDimm0Temp;
  UINT8     TSonDimm1Temp;
} MOBILE_SYSTEM_MONITOR_INFO;

typedef struct {
  UINT8   MonitorDeviceType;
  UINT8   CompanyId;
  UINT8   DeviceVersion;
  UINT8   DeviceRevision;
  UINT8   DeviceSmbusAddress;
} EFI_MONITOR_DEVICE_INFO;

typedef
EFI_STATUS
(EFIAPI *EFI_ACCURATE_TACH_SETTINGS) (
  IN UINT8    DeviceSmbusAddress,
  IN BOOLEAN  EnablePws
  );

// Protocol Definition
typedef struct {
  UINT8                         DeviceCount;
  EFI_MONITOR_DEVICE_INFO       DeviceInfoList[MAX_MONITOR_DEVICE];
  EFI_ACCURATE_TACH_SETTINGS    AccurateTachSettings;
} EFI_SYSTEM_MONITOR_PROTOCOL;

#endif
