/** @file
  SPD data format header file for DDR5.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _MrcSpdDataDdr5_h_
#define _MrcSpdDataDdr5_h_
#pragma pack (push, 1)

#include "CMrcTypes.h"

#define SPD5_MR0                (0)
#define SPD5_MR2                (2)
#define SPD5_MR3                (3)
#define SPD5_MR4                (4)
#define SPD5_MR5                (5)
#define SPD5_MR11               (11)
#define SPD5_MR5_HUB_SUPPORT    (1)
#define SPD5_MR0_SPD5_HUB_DEVICE (0x51)
#define SPD5_MEMREG_REG(Offset)   ((UINT8) ((~BIT7) & (Offset)))
#define SPD5_MEMREG_NVM(Offset)   ((UINT8) (BIT7 | (Offset)))
#define PMIC0_LID_CODE                            (0x09)
#define PMIC1_LID_CODE                            (0x08)
#define PMIC2_LID_CODE                            (0x0C)
// Base Configuration and DRAM Parameters

// Byte 0
// Number of Bytes in SPD Device
// SPD_DEVICE_DESCRIPTION_STRUCT common

// Byte 1
// SPD Revision for Base Configuration Parameters
// SPD_REVISION_STRUCT common

// Byte 2
// Key Byte / Host Bus Command Protocol Type
// SPD_DRAM_DEVICE_TYPE_STRUCT common

// Byte 3
// Key Byte / Module Type
typedef union {
  struct {
    UINT8  ModuleType                          :  4; ///< Bits 3:0
    UINT8  HybridMedia                         :  3; ///< Bits 6:4
    UINT8  Hybrid                              :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD5_MODULE_TYPE_STRUCT;

// Byte 4
// Primary SDRAM Density and Package
typedef union {
  struct {
    UINT8  Density                             :  5; ///< Bits 4:0
    UINT8  DiePerPackage                       :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD5_SDRAM_DENSITY_PACKAGE_STRUCT;

// Byte 5
// Primary SDRAM Addressing
typedef union {
  struct {
    UINT8  RowAddress                          :  5; ///< Bits 4:0
    UINT8  ColumnAddress                       :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD5_SDRAM_ADDRESSING_STRUCT;

// Byte 6
// Primary SDRAM I/O Width
typedef union {
  struct {
    UINT8                                      :  5; ///< Bits 4:0
    UINT8  SdramIoWidth                        :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD5_SDRAM_IO_WIDTH_STRUCT;

// Byte 7
// Primary SDRAM Bank Groups & Banks Per Bank Group
typedef union {
  struct {
    UINT8  BanksPerBankGroup                   :  3; ///< Bits 2:0
    UINT8                                      :  2; ///< Bits 4:3
    UINT8  BankGroups                          :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD5_SDRAM_BANK_GROUP_STRUCT;

// Byte 8
// Secondary SDRAM Density and Package
// SPD5_SDRAM_DENSITY_PACKAGE_STRUCT common

// Byte 9
// Secondary SDRAM Addressing
// SPD5_SDRAM_ADDRESSING_STRUCT common

// Byte 10
// Secondary SDRAM I/O Width
// SPD5_SDRAM_IO_WIDTH_STRUCT common

// Byte 11
// Secondary SDRAM Bank Groups & Banks Per Bank Group
// SPD5_SDRAM_BANK_GROUP_STRUCT common

// Byte 12
// SDRAM Optional Features
typedef union {
  struct {
    UINT8                                      :  4; ///< Bits 3:0
    UINT8  BL32                                :  1; ///< Bits 4:4
    UINT8  SoftPPR                             :  1; ///< Bits 5:5
    UINT8  PostPackageRepair                   :  2; ///< Bits 7:6
  } Bits;
  UINT8  Data;
} SPD5_SDRAM_OPTION_FEATURES_STRUCT;

// Byte 13
// Thermal and Refresh Options
typedef union {
  struct {
    UINT8  SdramTemperatureGrade               :  1; ///< Bit  0
    UINT8                                      :  7; ///< Bits 7:1
  } Bits;
  UINT8  Data;
} SPD5_SDRAM_THERMAL_REFRESH_STRUCT;

// Byte 14 - Byte 15
// Reserved

// Byte 16
// SDRAM Nominal Voltage, VDD
typedef union {
  struct {
    UINT8  Endurant                            :  2; ///< Bits 1:0
    UINT8  Operable                            :  2; ///< Bits 3:2
    UINT8  Nominal                             :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD5_SDRAM_NOMINAL_VOLTAGE_STRUCT;

// Byte 17
// SDRAM Nominal Voltage, VDDQ
// SPD5_SDRAM_NOMINAL_VOLTAGE_STRUCT common

// Byte 18
// SDRAM Nominal Voltage, VPP
// SPD5_SDRAM_NOMINAL_VOLTAGE_STRUCT common

// Byte 19
// Reserved

// Byte 20-21
// SDRAM Minimum Cycle Time (tCKmin\tCKAVGmin)
typedef union {
  struct {
    UINT16 tCKmin                             :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD5_TCK_MIN_PS_STRUCT;

// Byte 22-23
// SDRAM Minimum Cycle Time (tCKmax\tCKAVGmax)
typedef union {
  struct {
    UINT16 tCKmax                             :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD5_TCK_MAX_PS_STRUCT;

// Byte 24-28
// CAS Latencies Supported
typedef union {
  struct {
    UINT8  CL20                                :  1; ///< Bits 0:0
    UINT8  CL22                                :  1; ///< Bits 1:1
    UINT8  CL24                                :  1; ///< Bits 2:2
    UINT8  CL26                                :  1; ///< Bits 3:3
    UINT8  CL28                                :  1; ///< Bits 4:4
    UINT8  CL30                                :  1; ///< Bits 5:5
    UINT8  CL32                                :  1; ///< Bits 6:6
    UINT8  CL34                                :  1; ///< Bits 7:7
    UINT8  CL36                                :  1; ///< Bits 8:8
    UINT8  CL38                                :  1; ///< Bits 9:9
    UINT8  CL40                                :  1; ///< Bits 10:10
    UINT8  CL42                                :  1; ///< Bits 11:11
    UINT8  CL44                                :  1; ///< Bits 12:12
    UINT8  CL46                                :  1; ///< Bits 13:13
    UINT8  CL48                                :  1; ///< Bits 14:14
    UINT8  CL50                                :  1; ///< Bits 15:15
    UINT8  CL52                                :  1; ///< Bits 16:16
    UINT8  CL54                                :  1; ///< Bits 17:17
    UINT8  CL56                                :  1; ///< Bits 18:18
    UINT8  CL58                                :  1; ///< Bits 19:19
    UINT8  CL60                                :  1; ///< Bits 20:20
    UINT8  CL62                                :  1; ///< Bits 21:21
    UINT8  CL64                                :  1; ///< Bits 22:22
    UINT8  CL66                                :  1; ///< Bits 23:23
    UINT8  CL68                                :  1; ///< Bits 24:24
    UINT8  CL70                                :  1; ///< Bits 25:25
    UINT8  CL72                                :  1; ///< Bits 26:26
    UINT8  CL74                                :  1; ///< Bits 27:27
    UINT8  CL76                                :  1; ///< Bits 28:28
    UINT8  CL78                                :  1; ///< Bits 29:29
    UINT8  CL80                                :  1; ///< Bits 30:30
    UINT8  CL82                                :  1; ///< Bits 31:31
    UINT8  CL84                                :  1; ///< Bits 32:32
    UINT8  CL86                                :  1; ///< Bits 33:33
    UINT8  CL88                                :  1; ///< Bits 34:34
    UINT8  CL90                                :  1; ///< Bits 35:35
    UINT8  CL92                                :  1; ///< Bits 36:36
    UINT8  CL94                                :  1; ///< Bits 37:37
    UINT8  CL96                                :  1; ///< Bits 38:38
    UINT8  CL98                                :  1; ///< Bits 39:39
  } Bits;
  UINT8  Data8[5];
} SPD5_CAS_LATENCIES_SUPPORTED_STRUCT;

// Byte 29
// Reserved

// Byte 30-31
// SDRAM Minimum CAS Latency Time (tAAmin)
typedef union {
  struct {
    UINT16 tAAmin                             :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD5_TAA_MIN_PS_STRUCT;

// Byte 32-33
// SDRAM Minimum RAS# to CAS# Delay Time (tRCDmin)
typedef union {
  struct {
    UINT16 tRCDmin                             :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD5_TRCD_MIN_PS_STRUCT;

// Byte 34-35
// SDRAM Minimum Row Precharge Delay Time (tRPmin)
typedef union {
  struct {
    UINT16 tRPmin                             :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD5_TRP_MIN_PS_STRUCT;

// Byte 36-37
// SDRAM Minimum Active to Precharge Delay Time (tRASmin)
typedef union {
  struct {
    UINT16 tRASmin                             :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD5_TRAS_MIN_PS_STRUCT;

// Byte 38-39
// SDRAM Minimum Active to Active/Refresh Delay Time (tRCmin)
typedef union {
  struct {
    UINT16 tRCmin                             :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD5_TRC_MIN_PS_STRUCT;

// Byte 40-41
// SDRAM Minimum Write Recovery Time (tWRmin)
typedef union {
  struct {
    UINT16 tWRmin                             :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD5_TWR_MIN_PS_STRUCT;

// Byte 42-43
// SDRAM Minimum Refresh Recovery Delay Time (tRFC1min\tRFC1_slrmin)
typedef union {
  struct {
    UINT16 tRFC1min                             :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD5_TRFC1_MIN_NS_STRUCT;

// Byte 44-45
// SDRAM Minimum Refresh Recovery Delay Time (tRFC2min\tRFC2_slrmin)
typedef union {
  struct {
    UINT16 tRFC2min                             :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD5_TRFC2_MIN_NS_STRUCT;

// Byte 46-47
// SDRAM Minimum Refresh Recovery Delay Time (tRFCsbmin\tRFCsb_slrmin)
typedef union {
  struct {
    UINT16 tRFCsbmin                             :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD5_TRFCSB_MIN_NS_STRUCT;

// Byte 48-49
// SDRAM Minimum Refresh Recovery Delay Time (tRFC1_dlrmin), 3DS Different Logical Rank
// SPD5_TRFC1_MIN_PS_STRUCT common

// Byte 50-51
// SDRAM Minimum Refresh Recovery Delay Time (tRFC2_dlrmin), 3DS Different Logical Rank
// SPD5_TRFC2_MIN_PS_STRUCT common

// Byte 52-53
// SDRAM Minimum Refresh Recovery Delay Time (tRFCsb_dlrmin), 3DS Different Logical Rank
// SPD5_TRFCSB_MIN_PS_STRUCT common

// Byte 54/56
// SDRAM Refresh Management First Byte
typedef union {
  struct {
    UINT8  RfmRequired                         :  1; // Bits 0:0
    UINT8  RaaImt                              :  4; // Bits 4:1
    UINT8  RaaMmt                              :  3; // Bits 7:5
  } Bits;
  UINT8  Data;
} SPD5_RFM_BYTE_1_STRUCT;

// Byte 55/57
// SDRAM Refresh Management Second Byte
typedef union {
  struct {
    UINT8  DrfmSupported                       :  1; // Bits 0:0
    UINT8  RecommendedBrc                      :  2; // Bits 2:1
    UINT8  BrcSupportLevel                     :  1; // Bits 3:3
    UINT8  ArfmLevel                           :  2; // Bits 5:4
    UINT8  RfmRaaCounter                       :  2; // Bits 7:6
  } Bits;
  UINT8  Data;
} SPD5_RFM_BYTE_2_STRUCT;

// Byte 58/60/62/64/66/68
// SDRAM Adaptive Refresh Management First Byte
typedef union {
  struct {
    UINT8  ArfmLevel                           :  1; // Bits 0:0
    UINT8  RaaImt                              :  4; // Bits 4:1
    UINT8  RaaMmt                              :  3; // Bits 7:5
  } Bits;
  UINT8  Data;
} SPD5_ARFM_BYTE_1_STRUCT;

// Byte 59/61/63/65/67/69
// SDRAM Adaptive Refresh Management Second Byte
typedef SPD5_RFM_BYTE_2_STRUCT SPD5_ARFM_BYTE_2_STRUCT;

// Byte 70-125
// Reserved

// Byte 126-127
// Cyclical Redundancy Code (CRC)
// SPD_CYCLIC_REDUNDANCY_CODE common

// Byte 128-191
// Reserved

// DDR5 Common SPD Bytes for All Module Types

// Byte 192
// SPD Revision for SPD bytes 192-447
// SPD_REVISION_STRUCT common

// Byte 193
// Reserved

// Byte 194-197
// SPD Device Information
typedef union {
  struct {
    UINT16 ContinuationCount                   :  7; ///< Bits 6:0
    UINT16 ContinuationParity                  :  1; ///< Bits 7:7
    UINT16 LastNonZeroByte                     :  8; ///< Bits 15:8
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD5_MANUFACTURER_ID_CODE;

typedef union {
  struct {
    UINT8  DeviceType                          :  4; ///< Bits 3:0
    UINT8                                      :  3; ///< Bits 6:4
    UINT8  DevicesInstalled                    :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD5_DEVICE_TYPE;

typedef struct {
  SPD5_MANUFACTURER_ID_CODE ManufacturerId;
  SPD5_DEVICE_TYPE          DeviceType;
  UINT8                     DeviceRevision;
} SPD5_DEVICE_INFO;

// Byte 198-209
// PMIC Device Information
// SPD5_DEVICE_INFO common

// Byte 210-213
// Thermal Sensor Device Information
typedef union {
  struct {
    UINT8  DeviceType                          :  4; ///< Bits 3:0
    UINT8                                      :  2; ///< Bits 5:4
    UINT8  DevicesInstalledTS1                 :  1; ///< Bits 6:6
    UINT8  DevicesInstalledTS0                 :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD5_THERM_DEVICE_TYPE;

typedef struct {
  SPD5_MANUFACTURER_ID_CODE ManufacturerId;
  SPD5_THERM_DEVICE_TYPE    DeviceType;
  UINT8                     DeviceRevision;
} SPD5_THERM_DEVICE_INFO;

// Byte 214-229
// Reserved

// Byte 230
// Module Nominal Height
typedef union {
  struct {
    UINT8  Height                              :  5; ///< Bits 4:0
    UINT8                                      :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD5_MODULE_NOMINAL_HEIGHT;

// Byte 231
// Module Maximum Thickness
typedef union {
  struct {
    UINT8  FrontThickness                      :  4; ///< Bits 3:0
    UINT8  BackThickness                       :  4; ///< Bits 7:4
  } Bits;
  UINT8 Data;
} SPD5_MODULE_MAXIMUM_THICKNESS;

// Byte 232
// Reference Raw Card Used
typedef union {
  struct {
    UINT8  Card                                :  5; ///< Bits 4:0
    UINT8  Revision                            :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD5_REFERENCE_RAW_CARD;

// Byte 233
// DIMM Attributes
typedef union {
  struct {
    UINT8  DramRowCount                        :  2; ///< Bits 1:0
    UINT8  HeatSpreader                        :  1; ///< Bits 2:2
    UINT8                                      :  5; ///< Bits 7:3
  } Bits;
  UINT8  Data;
} SPD5_DIMM_ATTRIBUTES;

// Byte 234
// Module Organization
typedef union {
  struct {
    UINT8  SdramDeviceWidth                    :  3; ///< Bits 2:0
    UINT8  RankCount                           :  3; ///< Bits 5:3
    UINT8  RankMix                             :  1; ///< Bits 6:6
    UINT8                                      :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD5_MODULE_ORGANIZATION;

// Byte 235
// Memory Channel Bus Width
typedef union {
  struct {
    UINT8  PrimaryBusWidth                     :  3; ///< Bits 2:0
    UINT8  BusWidthExtension                   :  2; ///< Bits 4:3
    UINT8  NumberOfChannels                    :  2; ///< Bits 6:5
    UINT8                                      :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD5_MODULE_MEMORY_BUS_WIDTH;

// Byte 236-239
// Reserved

// Byte 240-445
// Module Type Specific Information
typedef struct {
  UINT8                                   Reserved[445 - 240 + 1];  ///< 240-445 Reserved
} SPD5_MODULE_SOLDER_DOWN;

typedef struct {
  SPD5_DEVICE_INFO                        DeviceInfoRegister;       ///< 240-243 Registering Clock Driver (RCD) Device Information
  UINT8                                   Reserved[445 - 244 + 1];  ///< 244-445 Reserved
} SPD5_MODULE_UNBUFFERED;

typedef struct {
  SPD5_DEVICE_INFO                        DeviceInfoRegister;       ///< 240-243 Registering Clock Driver (RCD) Device Information
  UINT8                                   Reserved[445 - 244 + 1];  ///< 244-445 Reserved
} SPD5_MODULE_REGISTERED;

typedef struct {
  SPD5_DEVICE_INFO                        DeviceInfoRegister;       ///< 240-243 Registering Clock Driver (RCD) Device Information
  SPD5_DEVICE_INFO                        DeviceInfoDataBuffer;     ///< 244-247 Data Buffer Device Information
  UINT8                                   Reserved[445 - 248 + 1];  ///< 248-445 Reserved
} SPD5_MODULE_LOADREDUCED;

typedef struct {
  SPD5_DEVICE_INFO                        DeviceInfoMemBuffer;      ///< 240-243 Differential Memory Buffer Device Information
  UINT8                                   Reserved[445 - 244 + 1];  ///< 244-445 Reserved
} SPD5_MODULE_DIFFERENTIAL;

typedef struct {
  SPD5_DEVICE_INFO                        DeviceInfoRegister;       ///< 240-243 Registering Clock Driver (RCD) Device Information
  SPD5_DEVICE_INFO                        DeviceInfoDataBuffer;     ///< 244-247 Data Buffer Device Information
  UINT8                                   Reserved[445 - 248 + 1];  ///< 248-445 Reserved
} SPD5_MODULE_NVDIMM_N;

typedef struct {
  SPD5_DEVICE_INFO                        DeviceInfoRegister;       ///< 240-243 Registering Clock Driver (RCD) Device Information
  SPD5_DEVICE_INFO                        DeviceInfoDataBuffer;     ///< 244-247 Data Buffer Device Information
  UINT8                                   Reserved[445 - 248 + 1];  ///< 248-445 Reserved
} SPD5_MODULE_NVDIMM_P;

typedef union {
  SPD5_MODULE_SOLDER_DOWN                 SolderDown;               ///< 240-445 Module Specific Bytes for Solder Down
  SPD5_MODULE_UNBUFFERED                  Unbuffered;               ///< 240-445 Module Specific Bytes for Unbuffered Memory Module types
  SPD5_MODULE_REGISTERED                  Registered;               ///< 240-445 Module Specific Bytes for Registered Memory Module types
  SPD5_MODULE_LOADREDUCED                 LoadReduced;              ///< 240-445 Module Specific Bytes for Load Reduced Memory Module types
  SPD5_MODULE_DIFFERENTIAL                Differential;             ///< 240-445 Module Specific Bytes for Differential Memory Module types
  SPD5_MODULE_NVDIMM_N                    NvdimmN;                  ///< 240-445 Module Specific Bytes for Non-Volatile (NVDIMM-N) Memory Module types
  SPD5_MODULE_NVDIMM_N                    NvdimmP;                  ///< 240-445 Module Specific Bytes for Non-Volatile (NVDIMM-P) Memory Module types
} SPD5_MODULE_SPECIFIC;

// Byte 446-447
// Cyclical Redundancy Code (CRC)
// SPD_CYCLIC_REDUNDANCY_CODE common

// Byte 448-511
// Reserved

// Manufacturing Information

// Byte 512-520
// Unique Module ID
// SPD_UNIQUE_MODULE_ID common

// Byte 521-550
// Module Part Number
typedef struct {
  UINT8                               ModulePartNumber[550 - 521 + 1]; ///< 521-550 Module Part Number
} SPD5_MODULE_PART_NUMBER;

// Byte 551
// Module Revision Code
typedef UINT8                         SPD5_MODULE_REVISION_CODE;

// Byte 552-553
// Dram Manufacturer ID Code
// SPD_MANUFACTURER_ID_CODE common

// Byte 554
// Dram Stepping
typedef UINT8                         SPD5_DRAM_STEPPING;

// Byte 555-637
// Manufacturer's Specific Data
typedef struct {
  UINT8                               ManufactureSpecificData[637 - 555 + 1]; ///< 555-637 Manufacturer's Specific Data
} SPD5_MANUFACTURE_SPECIFIC;

// Byte 638-639
// Reserved

// Byte 640-1023
// End User Programmable
typedef union {
  struct {
    UINT16 tCCD_Lmin                           :  16; // Bits 15:0
    UINT8  LowerClockLimit                     :  8;  // Bits 23:16
  } Bits;
  UINT8  Data[3];
} SPD5_TCCD_L_PS_STRUCT;

typedef union {
  struct {
    UINT16 tCCD_L_WRmin                        :  16; // Bits 15:0
    UINT8  LowerClockLimit                     :  8;  // Bits 23:16
  } Bits;
  UINT8  Data[3];
} SPD5_TCCD_L_WR_PS_STRUCT;
typedef union {
  struct {
    UINT16 tCCD_L_WR2min                       :  16; // Bits 15:0
    UINT8  LowerClockLimit                     :  8;  // Bits 23:16
  } Bits;
  UINT8  Data[3];
} SPD5_TCCD_L_WR2_PS_STRUCT;
typedef union {
  struct {
    UINT16 tWTR_Lmin                           :  16; // Bits 15:0
    UINT8  LowerClockLimit                     :  8;  // Bits 23:16
  } Bits;
  UINT8  Data[3];
} SPD5_TWTR_L_PS_STRUCT;
typedef union {
  struct {
    UINT16 tWTR_Smin                           :  16; // Bits 15:0
    UINT8  LowerClockLimit                     :  8;  // Bits 23:16
  } Bits;
  UINT8  Data[3];
} SPD5_TWTR_S_PS_STRUCT;
typedef union {
  struct {
    UINT16 tRRD_Lmin                           :  16; // Bits 15:0
    UINT8  LowerClockLimit                     :  8;  // Bits 23:16
  } Bits;
  UINT8  Data[3];
} SPD5_TRRD_L_PS_STRUCT;
typedef union {
  struct {
    UINT16 tRTPmin                             :  16; // Bits 15:0
    UINT8  LowerClockLimit                     :  8;  // Bits 23:16
  } Bits;
  UINT8  Data[3];
} SPD5_TRTP_PS_STRUCT;
typedef union {
  struct {
    UINT16 tFAWmin                             :  16; // Bits 15:0
    UINT8  LowerClockLimit                     :  8;  // Bits 23:16
  } Bits;
  UINT8  Data[3];
} SPD5_TFAW_PS_STRUCT;
#pragma pack (pop)
#endif // _MrcSpdDataDdr5_h_

