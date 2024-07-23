/** @file
  SPD data format header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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

#ifndef _MrcSpdData_h_
#define _MrcSpdData_h_
#pragma pack (push, 1)

#include "CMrcTypes.h"
#include "CMrcSpdDataDdr5.h"

#define MAX_XMP3_PROFILES     (5)
#define SPD5_MANUF_SIZE   (SPD5_MANUF_END - SPD5_MANUF_START + 1)   ///< The size of the SPD manufacturing data.
#define SPDLP_MANUF_SIZE  (SPDLP_MANUF_END - SPDLP_MANUF_START + 1) ///< The size of the SPD manufacutring data.
#define XMP_PROFILE1_SUPPORT_MASK             MRC_BIT0
#define XMP_PROFILE2_SUPPORT_MASK             MRC_BIT1
#define XMP_PROFILE3_SUPPORT_MASK             MRC_BIT2
#define XMP_USER_PROFILE4_SUPPORT_MASK        MRC_BIT3
#define XMP_USER_PROFILE5_SUPPORT_MASK        MRC_BIT4
#define SPD_XMP3_GLOBAL_SECTION_START_BYTE    (640)
#define SPD_XMP3_GLOBAL_SECTION_END_BYTE      (701)
#define SPD_XMP3_GLOBAL_SECTION_SIZE          (SPD_XMP3_GLOBAL_SECTION_END_BYTE - SPD_XMP3_GLOBAL_SECTION_START_BYTE + 1)
#define SPD_XMP3_PROFILE1_START_BYTE          (704)
#define SPD_XMP3_PROFILE1_END_BYTE            (765)
#define SPD_XMP3_PROFILE_SIZE                 (SPD_XMP3_PROFILE1_END_BYTE - SPD_XMP3_PROFILE1_START_BYTE + 1)
#define SPD_XMP3_PROFILE2_START_BYTE          (768)
#define SPD_XMP3_PROFILE2_END_BYTE            (829)
#define SPD_XMP3_PROFILE3_START_BYTE          (832)
#define SPD_XMP3_PROFILE3_END_BYTE            (893)
#define SPD_XMP3_USER_PROFILE4_START_BYTE     (896)
#define SPD_XMP3_USER_PROFILE4_END_BYTE       (957)
#define SPD_XMP3_USER_PROFILE5_START_BYTE     (960)
#define SPD_XMP3_USER_PROFILE5_END_BYTE       (1021)
#define SPD_PAGE_ADDRESS_0                    (0x6C)
#define SPD_PAGE_ADDRESS_1                    (0x6E)
#define SPD_SDRAM_TYPE_OFFSET                 (0x02)
#define MRC_SPD_DDR5_SDRAM_TYPE_NUMBER        (0x12)
#define MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER      (0x13)
#define MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER     (0x15)
#define CKD_LID                       (0xB)
#define DIMM_LID_MASK                 (0xF0)
#define DIMM_HID_MASK                 (0x7)

typedef union {
  struct {
    UINT8  BytesUsed                           :  4; ///< Bits 3:0
    UINT8  BytesTotal                          :  3; ///< Bits 6:4
    UINT8  CrcCoverage                         :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_DEVICE_DESCRIPTION_STRUCT;

typedef union {
  struct {
    UINT8  Minor                               :  4; ///< Bits 3:0
    UINT8  Major                               :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_REVISION_STRUCT;

typedef union {
  struct {
    UINT8  Type                                :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_DRAM_DEVICE_TYPE_STRUCT;

typedef union {
  struct {
    UINT8  ModuleType                          :  4; ///< Bits 3:0
    UINT8                                      :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_MODULE_TYPE_STRUCT;

typedef union {
  struct {
    UINT8  Density                             :  4; ///< Bits 3:0
    UINT8  BankAddress                         :  3; ///< Bits 6:4
    UINT8                                      :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_SDRAM_DENSITY_BANKS_STRUCT;

typedef union {
  struct {
    UINT8  ColumnAddress                       :  3; ///< Bits 2:0
    UINT8  RowAddress                          :  3; ///< Bits 5:3
    UINT8                                      :  2; ///< Bits 7:6
  } Bits;
  UINT8  Data;
} SPD_SDRAM_ADDRESSING_STRUCT;

typedef union {
  struct {
    UINT8  OperationAt1_50                     :  1; ///< Bits 0:0
    UINT8  OperationAt1_35                     :  1; ///< Bits 1:1
    UINT8  OperationAt1_25                     :  1; ///< Bits 2:2
    UINT8                                      :  5; ///< Bits 7:3
  } Bits;
  UINT8  Data;
} SPD_MODULE_NOMINAL_VOLTAGE_STRUCT;

typedef union {
  struct {
    UINT8  SdramDeviceWidth                    :  3; ///< Bits 2:0
    UINT8  RankCount                           :  3; ///< Bits 5:3
    UINT8                                      :  2; ///< Bits 7:6
  } Bits;
  UINT8  Data;
} SPD_MODULE_ORGANIZATION_STRUCT;

typedef union {
  struct {
    UINT8  PrimaryBusWidth                     :  3; ///< Bits 2:0,  In LPDDR4 this is ChannelBusWidth
    UINT8  BusWidthExtension                   :  2; ///< Bits 4:3
    UINT8  NumberOfChannels                    :  3; ///< Bits 7:5, only in LPDDR4
  } Bits;
  UINT8  Data;
} SPD_MODULE_MEMORY_BUS_WIDTH_STRUCT;

typedef union {
  struct {
    UINT8  Divisor                             :  4; ///< Bits 3:0
    UINT8  Dividend                            :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_FINE_TIMEBASE_STRUCT;

typedef union {
  struct {
    UINT8  Dividend                            :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_MEDIUM_TIMEBASE_DIVIDEND_STRUCT;

typedef union {
  struct {
    UINT8  Divisor                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_MEDIUM_TIMEBASE_DIVISOR_STRUCT;

typedef struct {
  SPD_MEDIUM_TIMEBASE_DIVIDEND_STRUCT Dividend; ///< Medium Timebase (MTB) Dividend
  SPD_MEDIUM_TIMEBASE_DIVISOR_STRUCT  Divisor;  ///< Medium Timebase (MTB) Divisor
} SPD_MEDIUM_TIMEBASE;

typedef union {
  struct {
    UINT8  tCKmin                              :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TCK_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT16 CL4                                 :  1; ///< Bits 0:0
    UINT16 CL5                                 :  1; ///< Bits 1:1
    UINT16 CL6                                 :  1; ///< Bits 2:2
    UINT16 CL7                                 :  1; ///< Bits 3:3
    UINT16 CL8                                 :  1; ///< Bits 4:4
    UINT16 CL9                                 :  1; ///< Bits 5:5
    UINT16 CL10                                :  1; ///< Bits 6:6
    UINT16 CL11                                :  1; ///< Bits 7:7
    UINT16 CL12                                :  1; ///< Bits 8:8
    UINT16 CL13                                :  1; ///< Bits 9:9
    UINT16 CL14                                :  1; ///< Bits 10:10
    UINT16 CL15                                :  1; ///< Bits 11:11
    UINT16 CL16                                :  1; ///< Bits 12:12
    UINT16 CL17                                :  1; ///< Bits 13:13
    UINT16 CL18                                :  1; ///< Bits 14:14
    UINT16                                     :  1; ///< Bits 15:15
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD_CAS_LATENCIES_SUPPORTED_STRUCT;

typedef union {
  struct {
    UINT8  tAAmin                              :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TAA_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tWRmin                              :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TWR_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tRCDmin                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRCD_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tRRDmin                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRRD_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tCCDLmin                            :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TCCD_L_MIN_MTB_STRUCT;

typedef union {
  struct {
    INT8  tCCDLminFine                         :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TCCD_L_MIN_FTB_STRUCT;

typedef union {
  struct {
    UINT8  tRPmin                              :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRP_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tRPab                               :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRP_AB_MTB_STRUCT;

typedef union {
  struct {
    INT8  tRPabFine                            :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TRP_AB_FTB_STRUCT;

typedef union {
  struct {
    UINT8  tRPpb                               :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRP_PB_MTB_STRUCT;

typedef union {
  struct {
    INT8  tRPpbFine                            :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TRP_PB_FTB_STRUCT;

typedef union {
  struct {
    UINT16 tRFCab                              :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD_TRFC_AB_MTB_STRUCT;

typedef union {
  struct {
    UINT16 tRFCpb                              :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD_TRFC_PB_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tRASminUpper                        :  4; ///< Bits 3:0
    UINT8  tRCminUpper                         :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_TRAS_TRC_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tRASmin                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRAS_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tRCmin                              :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRC_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT16 tRFCmin                             :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD_TRFC_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tWTRmin                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TWTR_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tRTPmin                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRTP_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tFAWminUpper                        :  4; ///< Bits 3:0
    UINT8                                      :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_TFAW_MIN_MTB_UPPER_STRUCT;

typedef union {
  struct {
    UINT8  tFAWmin                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TFAW_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tCWLmin                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TCWL_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  NMode                               :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_SYSTEM_COMMAND_RATE_STRUCT;

typedef union {
  struct {
    UINT16 tREFImin                            :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD_TREFI_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  RealTimeMemoryFreqOC                 :  1; // Bits 0:0
    UINT8  DynamicMemoryBoost                   :  1; // Bits 1:1
    UINT8                                       :  6; // Bits 7:2
  } Bits;
  UINT8  Data;

} SPD_MEMORY_OC_FEATURE_STRUCT;
#define REALTIME_MEMORY_FREQ_OVERCLOCK_SUPPORTED   (BIT0)
#define DYNAMIC_MEMORY_BOOST_SUPPORTED             (BIT1)
typedef enum {
  PMIC_5MVSTEP,
  PMIC_10MVSTEP
} PMICStepSize;

typedef union {
  struct {
    UINT8   OCCapable                           :  1; // Bits 0:0
    UINT8   OCEnabled                           :  1; // Bits 1:1
    UINT8   DefaultStepSize                     :  1; // Bits 2:2
    UINT8                                       :  5; // Bits 7:3
  } Bits;
  UINT8  Data;

} SPD_MEMORY_PMIC_CAPABILITIES_STRUCT;
typedef union {
  struct {
    UINT8  RZQ6                                :  1; ///< Bits 0:0
    UINT8  RZQ7                                :  1; ///< Bits 1:1
    UINT8                                      :  5; ///< Bits 6:2
    UINT8  DllOff                              :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_SDRAM_OPTIONAL_FEATURES_STRUCT;

typedef union {
  struct {
    UINT8  ExtendedTemperatureRange            :  1; ///< Bits 0:0
    UINT8  ExtendedTemperatureRefreshRate      :  1; ///< Bits 1:1
    UINT8  AutoSelfRefresh                     :  1; ///< Bits 2:2
    UINT8  OnDieThermalSensor                  :  1; ///< Bits 3:3
    UINT8                                      :  3; ///< Bits 6:4
    UINT8  PartialArraySelfRefresh             :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_SDRAM_THERMAL_REFRESH_STRUCT;

typedef union {
  struct {
    UINT8  ThermalSensorAccuracy               :  7; ///< Bits 6:0
    UINT8  ThermalSensorPresence               :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_MODULE_THERMAL_SENSOR_STRUCT;

typedef union {
  struct {
    UINT8  NonStandardDeviceDescription        :  7; ///< Bits 6:0
    UINT8  SdramDeviceType                     :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_SDRAM_DEVICE_TYPE_STRUCT;

typedef union {
  struct {
    UINT8                                      :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_AUTO_SELF_REFRESH_PERF_STRUCT;

typedef union {
  struct {
    INT8  tCKminFine                           :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TCK_MIN_FTB_STRUCT;

typedef union {
  struct {
    INT8  tAAminFine                           :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TAA_MIN_FTB_STRUCT;

typedef union {
  struct {
    INT8  tRCDminFine                          :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TRCD_MIN_FTB_STRUCT;

typedef union {
  struct {
    INT8  tRPminFine                           :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TRP_MIN_FTB_STRUCT;

typedef union {
  struct {
    INT8  tRCminFine                           :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TRC_MIN_FTB_STRUCT;

typedef union {
  struct {
    UINT8  tMACencoding                        :  4; ///< Bits 3:0
    UINT8  tMAWencoding                        :  2; ///< Bits 5:4
    UINT8  Reserved                            :  2; ///< Bits 7:6
  } Bits;
  UINT8  Data;
} SPD_PTRR_SUPPORT_STRUCT;

typedef union {
  struct {
    INT8  tRRDminFine                          :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TRRD_MIN_FTB_STRUCT;

typedef union {
  struct {
    UINT8  Height                              :  5; ///< Bits 4:0
    UINT8  RawCardExtension                    :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD_UNBUF_MODULE_NOMINAL_HEIGHT;

typedef union {
  struct {
    UINT8  FrontThickness                      :  4; ///< Bits 3:0
    UINT8  BackThickness                       :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_UNBUF_MODULE_NOMINAL_THICKNESS;

typedef union {
  struct {
    UINT8  Card                                :  5; ///< Bits 4:0
    UINT8  Revision                            :  2; ///< Bits 6:5
    UINT8  Extension                           :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_UNBUF_REFERENCE_RAW_CARD;

typedef union {
  struct {
    UINT8  MappingRank1                        :  1; ///< Bits 0:0
    UINT8                                      :  7; ///< Bits 7:1
  } Bits;
  UINT8  Data;
} SPD_UNBUF_ADDRESS_MAPPING;

typedef union {
  struct {
    UINT8  Height                              :  5; ///< Bits 4:0
    UINT8                                      :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD_RDIMM_MODULE_NOMINAL_HEIGHT;

typedef union {
  struct {
    UINT8  FrontThickness                      :  4; ///< Bits 3:0
    UINT8  BackThickness                       :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_RDIMM_MODULE_NOMINAL_THICKNESS;

typedef union {
  struct {
    UINT8  Card                                :  5; ///< Bits 4:0
    UINT8  Revision                            :  2; ///< Bits 6:5
    UINT8  Extension                           :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_RDIMM_REFERENCE_RAW_CARD;

typedef union {
  struct {
    UINT8  RegisterCount                       :  2; ///< Bits 1:0
    UINT8  DramRowCount                        :  2; ///< Bits 3:2
    UINT8                                      :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_RDIMM_MODULE_ATTRIBUTES;

typedef union {
  struct {
    UINT16 ContinuationCount                   :  7; ///< Bits 6:0
    UINT16 ContinuationParity                  :  1; ///< Bits 7:7
    UINT16 LastNonZeroByte                     :  8; ///< Bits 15:8
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD_MANUFACTURER_ID_CODE;

typedef struct {
  UINT8  Year;                                 ///< Year represented in BCD (00h = 2000)
  UINT8  Week;                                 ///< Year represented in BCD (47h = week 47)
} SPD_MANUFACTURING_DATE;

typedef union {
  UINT32 Data;
  UINT16 SerialNumber16[2];
  UINT8  SerialNumber8[4];
} SPD_MANUFACTURER_SERIAL_NUMBER;

typedef struct {
  UINT8 Location;                              ///< Module Manufacturing Location
} SPD_MANUFACTURING_LOCATION;

typedef struct {
  SPD_MANUFACTURER_ID_CODE            IdCode;                   ///< Module Manufacturer ID Code
  SPD_MANUFACTURING_LOCATION          Location;                 ///< Module Manufacturing Location
  SPD_MANUFACTURING_DATE              Date;                     ///< Module Manufacturing Year, in BCD (range: 2000-2255)
  SPD_MANUFACTURER_SERIAL_NUMBER      SerialNumber;             ///< Module Serial Number
} SPD_UNIQUE_MODULE_ID;

typedef union {
  UINT16 Crc[1];
  UINT8  Data8[2];
} SPD_CYCLIC_REDUNDANCY_CODE;

typedef union {
  struct {
    UINT8  ProfileEnable1                :  1;  // Bits 0:0
    UINT8  ProfileEnable2                :  1;  // Bits 1:1
    UINT8  ProfileConfig1                :  2;  // Bits 3:2
    UINT8  ProfileConfig2                :  2;  // Bits 5:4
    UINT8                                :  2;  // Bits 7:6
  } Bits;
  UINT8  Data;
} SPD_XMP_ORG_CONFIG;

typedef union {
  struct {
    UINT8  ProfileEnable1                :  1;  // Bits 0:0
    UINT8  ProfileEnable2                :  1;  // Bits 1:1
    UINT8  ProfileEnable3                :  1;  // Bits 2:2
    UINT8                                :  5;  // Bits 7:3
  } Bits;
  UINT8  Data;
} SPD_XMP3_PROFILE_ORG;

typedef union {
  struct {
    UINT8  ProfileConfig1                :  2;  // Bits 1:0
    UINT8  ProfileConfig2                :  2;  // Bits 3:2
    UINT8  ProfileConfig3                :  2;  // Bits 5:4
    UINT8                                :  2;  // Bits 7:6
  } Bits;
  UINT8  Data;
} SPD_XMP3_PROFILE_CONFIG;

typedef union {
  struct {
    UINT8  Decimal : 5;
    UINT8  Integer : 2;
    UINT8          : 1;
  } Bits;
  UINT8 Data;
} SPD_VDD_VOLTAGE_LEVEL_STRUCT;

typedef union {
  struct {
    UINT8  Decimal : 7;
    UINT8  Integer : 1;
  } Bits;
  UINT8 Data;
} SPD_VDD_VOLTAGE_LEVEL_STRUCT_2_0;

typedef union {
  struct {
    UINT8  Fine                                :  2; ///< Bits 1:0
    UINT8  Medium                              :  2; ///< Bits 3:2
    UINT8                                      :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD4_TIMEBASE_STRUCT;

typedef union {
  struct {
    UINT32 CL7                                 :  1; ///< Bits 0:0
    UINT32 CL8                                 :  1; ///< Bits 1:1
    UINT32 CL9                                 :  1; ///< Bits 2:2
    UINT32 CL10                                :  1; ///< Bits 3:3
    UINT32 CL11                                :  1; ///< Bits 4:4
    UINT32 CL12                                :  1; ///< Bits 5:5
    UINT32 CL13                                :  1; ///< Bits 6:6
    UINT32 CL14                                :  1; ///< Bits 7:7
    UINT32 CL15                                :  1; ///< Bits 8:8
    UINT32 CL16                                :  1; ///< Bits 9:9
    UINT32 CL17                                :  1; ///< Bits 10:10
    UINT32 CL18                                :  1; ///< Bits 11:11
    UINT32 CL19                                :  1; ///< Bits 12:12
    UINT32 CL20                                :  1; ///< Bits 13:13
    UINT32 CL21                                :  1; ///< Bits 14:14
    UINT32 CL22                                :  1; ///< Bits 15:15
    UINT32 CL23                                :  1; ///< Bits 16:16
    UINT32 CL24                                :  1; ///< Bits 17:17
    UINT32                                     :  14; ///< Bits 31:18
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SPD4_CAS_LATENCIES_SUPPORTED_STRUCT;

typedef struct {
  SPD_VDD_VOLTAGE_LEVEL_STRUCT        Vdd;                      ///< 185, 220 XMP Module VDD Voltage Level
  SPD_TCK_MIN_MTB_STRUCT              tCKmin;                   ///< 186, 221 XMP SDRAM Minimum Cycle Time (tCKmin)
  SPD_TAA_MIN_MTB_STRUCT              tAAmin;                   ///< 187, 222 XMP Minimum CAS Latency Time (tAAmin)
  SPD_CAS_LATENCIES_SUPPORTED_STRUCT  CasLatencies;             ///< 188-189, 223-224 XMP CAS Latencies Supported, Least Significant Byte
  SPD_TCWL_MIN_MTB_STRUCT             tCWLmin;                  ///< 190, 225 XMP Minimum CAS Write Latency Time (tCWLmin)
  SPD_TRP_MIN_MTB_STRUCT              tRPmin;                   ///< 191, 226 XMP Minimum Row Precharge Delay Time (tRPmin)
  SPD_TRCD_MIN_MTB_STRUCT             tRCDmin;                  ///< 192, 227 XMP Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TWR_MIN_MTB_STRUCT              tWRmin;                   ///< 193, 228 XMP Minimum Write Recovery Time (tWRmin)
  SPD_TRAS_TRC_MIN_MTB_STRUCT         tRASMintRCMinUpper;       ///< 194, 229 XMP Upper Nibbles for tRAS and tRC
  SPD_TRAS_MIN_MTB_STRUCT             tRASmin;                  ///< 195, 230 XMP Minimum Active to Precharge Delay Time (tRASmin), Least Significant Byte
  SPD_TRC_MIN_MTB_STRUCT              tRCmin;                   ///< 196, 231 XMP Minimum Active to Active/Refresh Delay Time (tRCmin), Least Significant Byte
  SPD_TREFI_MIN_MTB_STRUCT            tREFImin;                 ///< 197-198, 232-233 XMP Maximum tREFI Time (Average Periodic Refresh Interval), Least Significant Byte
  SPD_TRFC_MIN_MTB_STRUCT             tRFCmin;                  ///< 199-200, 234-235 XMP Minimum Refresh Recovery Delay Time (tRFCmin), Least Significant Byte
  SPD_TRTP_MIN_MTB_STRUCT             tRTPmin;                  ///< 201, 236 XMP Minimum Internal Read to Precharge Command Delay Time (tRTPmin)
  SPD_TRRD_MIN_MTB_STRUCT             tRRDmin;                  ///< 202, 237 XMP Minimum Row Active to Row Active Delay Time (tRRDmin)
  SPD_TFAW_MIN_MTB_UPPER_STRUCT       tFAWMinUpper;             ///< 203, 238 XMP Upper Nibble for tFAW
  SPD_TFAW_MIN_MTB_STRUCT             tFAWmin;                  ///< 204, 239 XMP Minimum Four Activate Window Delay Time (tFAWmin)
  SPD_TWTR_MIN_MTB_STRUCT             tWTRmin;                  ///< 205, 240 XMP Minimum Internal Write to Read Command Delay Time (tWTRmin)
  UINT8                               Reserved1[207 - 206 + 1]; ///< 206-207, 241-242 XMP Reserved
  SPD_SYSTEM_COMMAND_RATE_STRUCT      SystemCmdRate;            ///< 208, 243 XMP System ADD/CMD Rate (1N or 2N mode)
  SPD_AUTO_SELF_REFRESH_PERF_STRUCT   AsrPerf;                  ///< 209, 244 XMP SDRAM Auto Self Refresh Performance (Sub 1x Refresh and IDD6 impact)
  UINT8                               VoltageLevel;             ///< 210, 245 XMP Memory Controller Voltage Level
  SPD_TCK_MIN_FTB_STRUCT              tCKminFine;               ///< 211, 246 XMP Fine Offset for SDRAM Minimum Cycle Time (tCKmin)
  SPD_TAA_MIN_FTB_STRUCT              tAAminFine;               ///< 212, 247 XMP Fine Offset for Minimum CAS Latency Time (tAAmin)
  SPD_TRP_MIN_FTB_STRUCT              tRPminFine;               ///< 213, 248 XMP Minimum Row Precharge Delay Time (tRPmin)
  SPD_TRCD_MIN_FTB_STRUCT             tRCDminFine;              ///< 214, 249 XMP Fine Offset for Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TRC_MIN_FTB_STRUCT              tRCminFine;               ///< 215, 250 XMP Fine Offset for Minimum Active to Active/Refresh Delay Time (tRCmin)
  UINT8                               Reserved2[218 - 216 + 1]; ///< 216-218, 251-253 XMP Reserved
  UINT8                               VendorPersonality;        ///< 219, 254 XMP Vendor Personality
} SPD_EXTREME_MEMORY_PROFILE_DATA;

typedef struct {

  UINT16                              XmpId;                        ///< 384-385 XMP Identification String
  SPD_REVISION_STRUCT                 XmpRevision;                  ///< 387 XMP Revision
  SPD_XMP3_PROFILE_ORG                XmpOrg;                       ///< 643 XMP Org
  SPD_XMP3_PROFILE_CONFIG             XmpConf;                      ///< 644 XMP profile Configuration
  UINT16                              PmicVendorID;                 ///< 645~646 PMIC Vendor ID
  UINT8                               NumOfPmic;                    ///< 647 Number of PMICs
  SPD_MEMORY_PMIC_CAPABILITIES_STRUCT PmicCapabilities;             ///< 648 PMIC Capabilities
  SPD_REVISION_STRUCT                 XmpSpecRevision;              ///< 649 Extreme Memory Profile Spec Revision
  UINT8                               Reserved[653 - 650 + 1];      ///< 650-653 Reserved
  UINT8                               Profile1Name[669 - 654 + 1];  ///< 654-669 Profile1 String Name
  UINT8                               Profile2Name[685 - 670 + 1];  ///< 670-685 Profile2 String Name
  UINT8                               Profile3Name[701 - 686 + 1];  ///< 686-701 Profile3 String Name
  UINT16                              Crc;                          ///< 702-703 CRC for Global Section
} SPD_EXTREME_MEMORY_PROFILE_HEADER_3_0;

typedef struct {
  SPD_VDD_VOLTAGE_LEVEL_STRUCT        Vpp;                      ///< 704 XMP Module VPP Voltage Level
  SPD_VDD_VOLTAGE_LEVEL_STRUCT        Vdd;                      ///< 705 XMP Module VDD Voltage Level
  SPD_VDD_VOLTAGE_LEVEL_STRUCT        Vddq;                     ///< 706 XMP Module VDDQ Voltage Level
  SPD_VDD_VOLTAGE_LEVEL_STRUCT        Vtbd;                     ///< 707 XMP Module TBD Voltage Level
  SPD_VDD_VOLTAGE_LEVEL_STRUCT        Vmc;                      ///< 708 Memory Controller Voltage Level
  SPD5_TCK_MIN_PS_STRUCT              tCKAVGmin;                ///< 709-710 XMP SDRAM Minimum Cycle Time (tCKAVGmin)
  SPD5_CAS_LATENCIES_SUPPORTED_STRUCT CasLatencies;             ///< 711-715, XMP CAS Latencies Supported
  UINT8                               ReservedCASLatency;       ///< 716, Reserved for future CAS Latency Expansion
  SPD5_TAA_MIN_PS_STRUCT              tAAmin;                   ///< 717-718 XMP Minimum CAS Latency Time (tAAmin)
  SPD5_TRCD_MIN_PS_STRUCT             tRCDmin;                  ///< 719-720 XMP Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD5_TRP_MIN_PS_STRUCT              tRPmin;                   ///< 721-722 XMP Minimum Row Precharge Delay Time (tRPmin)
  SPD5_TRAS_MIN_PS_STRUCT             tRASmin;                  ///< 723-724 XMP Minimum Active to Precharge Delay Time (tRASmin)
  SPD5_TRC_MIN_PS_STRUCT              tRCmin;                   ///< 725-726 XMP Minimum Active to Active/Refresh Delay Time (tRCmin)
  SPD5_TWR_MIN_PS_STRUCT              tWRmin;                   ///< 727-728 XMP Minimum Write Recovery Time(tWRmin)
  SPD5_TRFC1_MIN_NS_STRUCT            tRFC1min;                 ///< 729-730 XMP Minimum Refresh Recovery Delay Time (tRFC1min)
  SPD5_TRFC2_MIN_NS_STRUCT            tRFC2min;                 ///< 731-732 XMP Minimum Refresh Recovery Delay Time (tRFC2min)
  SPD5_TRFCSB_MIN_NS_STRUCT           tRFCsbmin;                ///< 733-734 XMP Minimum Refresh Recovery Delay (Same Bank) Time (tRFCsbmin)
  SPD5_TCCD_L_PS_STRUCT               tCCD_Lmin;                ///< 735-737 XMP Minimum Read to Read Command Delay Time, Same Bank Group (tCCD_Lmin)
  SPD5_TCCD_L_WR_PS_STRUCT            tCCD_L_WRmin;             ///< 738-740 XMP Minimum Write to Write Command Delay Time, Same Bank Group (tCCD_L_WRmin)
  SPD5_TCCD_L_WR2_PS_STRUCT           tCCD_L_WR2min;            ///< 741-743 XMP Minimum Write to Write Command Delay Time, Second Write not RMW, Same Bank Group (tCCD_L_WR2min)
  SPD5_TWTR_L_PS_STRUCT               tWTR_Lmin;                ///< 744-746 XMP Minimum Write to Read Command Delay Time, Same Bank Group (tCCD_L_WTRmin)
  SPD5_TWTR_S_PS_STRUCT               tWTR_Smin;                ///< 747-749 XMP Minimum Write to Read Command Delay Time, Different Bank Group (tCCD_S_WTRmin)
  SPD5_TRRD_L_PS_STRUCT               tRRD_Lmin;                ///< 750-752 XMP Minimum Active to Active Command Delay Time, Same Bank Group (tRRD_Lmin)
  SPD5_TRTP_PS_STRUCT                 tRTPmin;                  ///< 753-755 XMP Minimum Read to Precharge Command Delay Time, (tRTP min)
  SPD5_TFAW_PS_STRUCT                 tFAWmin;                  ///< 756-758 XMP Minimum Four Activate Window (tFAW min)
  UINT8                               Reserved1[762 - 759 + 1]; ///< 759-762 XMP Reserved for Enthusiast
  SPD_MEMORY_OC_FEATURE_STRUCT        AdvancedMemoryOCFeat;     ///< 763 Advanced Memory Overclocking Feature Support
  SPD_SYSTEM_COMMAND_RATE_STRUCT      SystemCmdRate;            ///< 764 XMP System ADD/CMD Rate (1N to 3N mode)
  UINT8                               VendorPersonality;        ///< 765 XMP Vendor Personality
  UINT16                              Crc;                      ///< 766-767 Cyclical Redundancy Code
} SPD_EXTREME_MEMORY_PROFILE_DATA_3_0;

typedef struct {
  SPD_EXTREME_MEMORY_PROFILE_HEADER_3_0      Header;                   ///< 640-703 XMP header
  SPD_EXTREME_MEMORY_PROFILE_DATA_3_0        Data[MAX_XMP3_PROFILES];  ///< 704-1023 XMP profiles
} SPD_EXTREME_MEMORY_PROFILE_3_0;

typedef struct {
  SPD_DEVICE_DESCRIPTION_STRUCT       Description;              ///< 0   Number of Serial PD Bytes Written / SPD Device Size / CRC Coverage 1, 2
  SPD_REVISION_STRUCT                 Revision;                 ///< 1   SPD Revision
  SPD_DRAM_DEVICE_TYPE_STRUCT         DramDeviceType;           ///< 2   DRAM Device Type
  SPD_MODULE_TYPE_STRUCT              ModuleType;               ///< 3   Module Type
  SPD_SDRAM_DENSITY_BANKS_STRUCT      SdramDensityAndBanks;     ///< 4   SDRAM Density and Banks
  SPD_SDRAM_ADDRESSING_STRUCT         SdramAddressing;          ///< 5   SDRAM Addressing
  SPD_MODULE_NOMINAL_VOLTAGE_STRUCT   ModuleNominalVoltage;     ///< 6   Module Nominal Voltage, VDD
  SPD_MODULE_ORGANIZATION_STRUCT      ModuleOrganization;       ///< 7   Module Organization
  SPD_MODULE_MEMORY_BUS_WIDTH_STRUCT  ModuleMemoryBusWidth;     ///< 8   Module Memory Bus Width
  SPD_FINE_TIMEBASE_STRUCT            FineTimebase;             ///< 9   Fine Timebase (FTB) Dividend / Divisor
  SPD_MEDIUM_TIMEBASE                 MediumTimebase;           ///< 10-11 Medium Timebase (MTB) Dividend
  SPD_TCK_MIN_MTB_STRUCT              tCKmin;                   ///< 12  SDRAM Minimum Cycle Time (tCKmin)
  UINT8                               Reserved1;                ///< 13  Reserved
  SPD_CAS_LATENCIES_SUPPORTED_STRUCT  CasLatencies;             ///< 14-15 CAS Latencies Supported
  SPD_TAA_MIN_MTB_STRUCT              tAAmin;                   ///< 16  Minimum CAS Latency Time (tAAmin)
  SPD_TWR_MIN_MTB_STRUCT              tWRmin;                   ///< 17  Minimum Write Recovery Time (tWRmin)
  SPD_TRCD_MIN_MTB_STRUCT             tRCDmin;                  ///< 18  Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TRRD_MIN_MTB_STRUCT             tRRDmin;                  ///< 19  Minimum Row Active to Row Active Delay Time (tRRDmin)
  SPD_TRP_MIN_MTB_STRUCT              tRPmin;                   ///< 20  Minimum Row Precharge Delay Time (tRPmin)
  SPD_TRAS_TRC_MIN_MTB_STRUCT         tRASMintRCMinUpper;       ///< 21  Upper Nibbles for tRAS and tRC
  SPD_TRAS_MIN_MTB_STRUCT             tRASmin;                  ///< 22  Minimum Active to Precharge Delay Time (tRASmin), Least Significant Byte
  SPD_TRC_MIN_MTB_STRUCT              tRCmin;                   ///< 23  Minimum Active to Active/Refresh Delay Time (tRCmin), Least Significant Byte
  SPD_TRFC_MIN_MTB_STRUCT             tRFCmin;                  ///< 24-25  Minimum Refresh Recovery Delay Time (tRFCmin)
  SPD_TWTR_MIN_MTB_STRUCT             tWTRmin;                  ///< 26  Minimum Internal Write to Read Command Delay Time (tWTRmin)
  SPD_TRTP_MIN_MTB_STRUCT             tRTPmin;                  ///< 27  Minimum Internal Read to Precharge Command Delay Time (tRTPmin)
  SPD_TFAW_MIN_MTB_UPPER_STRUCT       tFAWMinUpper;             ///< 28  Upper Nibble for tFAW
  SPD_TFAW_MIN_MTB_STRUCT             tFAWmin;                  ///< 29  Minimum Four Activate Window Delay Time (tFAWmin)
  SPD_SDRAM_OPTIONAL_FEATURES_STRUCT  SdramOptionalFeatures;    ///< 30  SDRAM Optional Features
  SPD_SDRAM_THERMAL_REFRESH_STRUCT    ThermalAndRefreshOptions; ///< 31  SDRAMThermalAndRefreshOptions
  SPD_MODULE_THERMAL_SENSOR_STRUCT    ModuleThermalSensor;      ///< 32  Module Thermal Sensor
  SPD_SDRAM_DEVICE_TYPE_STRUCT        SdramDeviceType;          ///< 33  SDRAM Device Type
  SPD_TCK_MIN_FTB_STRUCT              tCKminFine;               ///< 34  Fine Offset for SDRAM Minimum Cycle Time (tCKmin)
  SPD_TAA_MIN_FTB_STRUCT              tAAminFine;               ///< 35  Fine Offset for Minimum CAS Latency Time (tAAmin)
  SPD_TRCD_MIN_FTB_STRUCT             tRCDminFine;              ///< 36  Fine Offset for Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TRP_MIN_FTB_STRUCT              tRPminFine;               ///< 37  Minimum Row Precharge Delay Time (tRPmin)
  SPD_TRC_MIN_FTB_STRUCT              tRCminFine;               ///< 38  Fine Offset for Minimum Active to Active/Refresh Delay Time (tRCmin)
  UINT8                               Reserved2[40 - 39 + 1];   ///< 39 - 40 Reserved
  SPD_PTRR_SUPPORT_STRUCT             pTRRsupport;              ///< 41 - pTRR support with TMAC value
  UINT8                               Reserved3[59 - 42 + 1];   ///< 42 - 59 Reserved
} SPD_GENERAL_SECTION;

typedef struct {
  SPD_UNBUF_MODULE_NOMINAL_HEIGHT     ModuleNominalHeight;      ///< 60 Module Nominal Height
  SPD_UNBUF_MODULE_NOMINAL_THICKNESS  ModuleMaximumThickness;   ///< 61 Module Maximum Thickness
  SPD_UNBUF_REFERENCE_RAW_CARD        ReferenceRawCardUsed;     ///< 62 Reference Raw Card Used
  SPD_UNBUF_ADDRESS_MAPPING           AddressMappingEdgeConn;   ///< 63 Address Mapping from Edge Connector to DRAM
  UINT8                               Reserved[116 - 64 + 1];   ///< 64-116 Reserved
} SPD_MODULE_UNBUFFERED;

typedef struct {
  SPD_RDIMM_MODULE_NOMINAL_HEIGHT     ModuleNominalHeight;      ///< 60 Module Nominal Height
  SPD_RDIMM_MODULE_NOMINAL_THICKNESS  ModuleMaximumThickness;   ///< 61 Module Maximum Thickness
  SPD_RDIMM_REFERENCE_RAW_CARD        ReferenceRawCardUsed;     ///< 62 Reference Raw Card Used
  SPD_RDIMM_MODULE_ATTRIBUTES         DimmModuleAttributes;     ///< 63 DIMM Module Attributes
  UINT8                               Reserved[116 - 64 + 1];   ///< 64-116 Reserved
} SPD_MODULE_REGISTERED;

typedef union {
  SPD_MODULE_UNBUFFERED               Unbuffered;
  SPD_MODULE_REGISTERED               Registered;
} SPD_MODULE_SPECIFIC;

typedef struct {
  UINT8                          ModulePartNumber[145 - 128 + 1];        ///< 128-145 Module Part Number
} SPD_MODULE_PART_NUMBER;

typedef struct {
  UINT8                          ModuleRevisionCode[147 - 146 + 1];      ///< 146-147 Module Revision Code
} SPD_MODULE_REVISION_CODE;

typedef struct {
  UINT8                          ManufactureSpecificData[175 - 150 + 1]; ///< 150-175 Manufacturer's Specific Data
} SPD_MANUFACTURE_SPECIFIC;

typedef union {
  struct {
    UINT8  Density                             :  4; ///< Bits 3:0
    UINT8  BankAddress                         :  2; ///< Bits 5:4
    UINT8  BankGroup                           :  2; ///< Bits 7:6
  } Bits;
  UINT8  Data;
} SPD4_SDRAM_DENSITY_BANKS_STRUCT;

typedef union {
  struct {
    UINT8  tCKmax                              :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD4_TCK_MAX_MTB_STRUCT;

typedef union {
  struct {
    INT8  tCKmaxFine                           :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD4_TCK_MAX_FTB_STRUCT;

typedef union {
  struct {
    UINT8                                      :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD4_SDRAM_THERMAL_REFRESH_STRUCT;

typedef enum {
  SPD_BRC2,
  SPD_BRC3,
  SPD_BRC4,
  BRC_Reserved
} BRCEncoding;
typedef struct {
  SPD_DEVICE_DESCRIPTION_STRUCT       Description;                     ///< 0       Number of Serial PD Bytes Written / SPD Device Size / CRC Coverage 1, 2
  SPD_REVISION_STRUCT                 Revision;                        ///< 1       SPD Revision
  SPD_DRAM_DEVICE_TYPE_STRUCT         DramDeviceType;                  ///< 2       DRAM Device Type
  SPD5_MODULE_TYPE_STRUCT             ModuleType;                      ///< 3       Module Type
  SPD5_SDRAM_DENSITY_PACKAGE_STRUCT   PrimarySdramDensityAndPackage;   ///< 4       Primary SDRAM Density and Package
  SPD5_SDRAM_ADDRESSING_STRUCT        PrimarySdramAddressing;          ///< 5       Primary SDRAM Addressing
  SPD5_SDRAM_IO_WIDTH_STRUCT          PrimarySdramIoWidth;             ///< 6       Primary SDRAM I/O Width
  SPD5_SDRAM_BANK_GROUP_STRUCT        PrimarySdramBankGroups;          ///< 7       Primary SDRAM Bank Groups & Banks Per Bank Group
  SPD5_SDRAM_DENSITY_PACKAGE_STRUCT   SecondarySdramDensityAndPackage; ///< 8       Secondary SDRAM Density and Package
  SPD5_SDRAM_ADDRESSING_STRUCT        SecondarySdramAddressing;        ///< 9       Secondary SDRAM Addressing
  SPD5_SDRAM_IO_WIDTH_STRUCT          SecondarySdramIoWidth;           ///< 10      Secondary SDRAM I/O Width
  SPD5_SDRAM_BANK_GROUP_STRUCT        SecondaryBankGroups;             ///< 11      Secondary SDRAM Bank Groups & Banks Per Bank Group
  SPD5_SDRAM_OPTION_FEATURES_STRUCT   SdramOptionalFeatures;           ///< 12      SDRAM Optional Features
  SPD5_SDRAM_THERMAL_REFRESH_STRUCT   ThermalAndRefreshOptions;        ///< 13      SDRAM Thermal and Refresh Options
  UINT8                               Reserved0[15 - 14 + 1];          ///< 14-15   Reserved
  SPD5_SDRAM_NOMINAL_VOLTAGE_STRUCT   SdramNominalVoltageVdd;          ///< 16      SDRAM Nominal Voltage, VDD
  SPD5_SDRAM_NOMINAL_VOLTAGE_STRUCT   SdramNominalVoltageVddq;         ///< 17      SDRAM Nominal Voltage, VDDQ
  SPD5_SDRAM_NOMINAL_VOLTAGE_STRUCT   SdramNominalVoltageVpp;          ///< 18      SDRAM Nominal Voltage, VPP
  UINT8                               Reserved1;                       ///< 19      Reserved
  SPD5_TCK_MIN_PS_STRUCT              tCKmin;                          ///< 20-21   SDRAM Minimum Cycle Time (tCKmin\tCKAVGmin)
  SPD5_TCK_MAX_PS_STRUCT              tCKmax;                          ///< 22-23   SDRAM Minimum Cycle Time (tCKmax\tCKAVGmax)
  SPD5_CAS_LATENCIES_SUPPORTED_STRUCT CasLatencies;                    ///< 24-28   CAS Latencies Supported
  UINT8                               Reserved2;                       ///< 29      Reserved
  SPD5_TAA_MIN_PS_STRUCT              tAAmin;                          ///< 30-31   SDRAM Minimum CAS Latency Time (tAAmin)
  SPD5_TRCD_MIN_PS_STRUCT             tRCDmin;                         ///< 32-33   SDRAM Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD5_TRP_MIN_PS_STRUCT              tRPmin;                          ///< 34-35   SDRAM Minimum Row Precharge Delay Time (tRPmin)
  SPD5_TRAS_MIN_PS_STRUCT             tRASmin;                         ///< 36-37   SDRAM Minimum Active to Precharge Delay Time (tRASmin)
  SPD5_TRC_MIN_PS_STRUCT              tRCmin;                          ///< 38-39   SDRAM Minimum Active to Active/Refresh Delay Time (tRCmin)
  SPD5_TWR_MIN_PS_STRUCT              tWRmin;                          ///< 40-41   SDRAM Minimum Write Recovery Time (tWRmin)
  SPD5_TRFC1_MIN_NS_STRUCT            tRFC1min;                        ///< 42-43   SDRAM Minimum Refresh Recovery Delay Time (tRFC1min\tRFC1_slrmin)
  SPD5_TRFC2_MIN_NS_STRUCT            tRFC2min;                        ///< 44-45   SDRAM Minimum Refresh Recovery Delay Time (tRFC2min\tRFC2_slrmin)
  SPD5_TRFCSB_MIN_NS_STRUCT           tRFCsbmin;                       ///< 46-47   SDRAM Minimum Refresh Recovery Delay Time (tRFCsbmin\tRFCsb_slrmin)
  SPD5_TRFC1_MIN_NS_STRUCT            tRFC1_dlrmin;                    ///< 48-49   SDRAM Minimum Refresh Recovery Delay Time (tRFC1_dlrmin), 3DS Different Logical Rank
  SPD5_TRFC2_MIN_NS_STRUCT            tRFC2_dlrmin;                    ///< 50-51   SDRAM Minimum Refresh Recovery Delay Time (tRFC2_dlrmin), 3DS Different Logical Rank
  SPD5_TRFCSB_MIN_NS_STRUCT           tRFCsb_dlrmin;                   ///< 52-53   SDRAM Minimum Refresh Recovery Delay Time (tRFCsb_dlrmin), 3DS Different Logical Rank
  SPD5_RFM_BYTE_1_STRUCT              tRFM_Byte1;                      ///< 54      SDRAM Refresh Management First Byte First SDRAM
  SPD5_RFM_BYTE_2_STRUCT              tRFM_Byte2;                      ///< 55      SDRAM Refresh Management Second Byte First SDRAM
  UINT8                               Reserved56;                      ///< 56      SDRAM Refresh Management First Byte Second SDRAM - not used in this MRC
  UINT8                               Reserved57;                      ///< 57      SDRAM Refresh Management Second Byte Second SDRAM - not used in this MRC
  SPD5_ARFM_BYTE_1_STRUCT             tARFM_A_Byte1;                   ///< 58      SDRAM Adaptive Refresh Management Level A First Byte First SDRAM
  SPD5_ARFM_BYTE_2_STRUCT             tARFM_A_Byte2;                   ///< 59      SDRAM Adaptive Refresh Management Level A Second Byte First SDRAM
  UINT8                               Reserved60;                      ///< 60      SDRAM Adaptive Refresh Management Level A First Byte Second SDRAM - not used in this MRC
  UINT8                               Reserved61;                      ///< 61      SDRAM Adaptive Refresh Management Level A Second Byte Second SDRAM - not used in this MRC
  SPD5_ARFM_BYTE_1_STRUCT             tARFM_B_Byte1;                   ///< 62      SDRAM Adaptive Refresh Management Level B First Byte First SDRAM
  SPD5_ARFM_BYTE_2_STRUCT             tARFM_B_Byte2;                   ///< 63      SDRAM Adaptive Refresh Management Level B Second Byte First SDRAM
  UINT8                               Reserved64;                      ///< 64      SDRAM Adaptive Refresh Management Level B First Byte Second SDRAM - not used in this MRC
  UINT8                               Reserved65;                      ///< 65      SDRAM Adaptive Refresh Management Level B Second Byte Second SDRAM - not used in this MRC
  SPD5_ARFM_BYTE_1_STRUCT             tARFM_C_Byte1;                   ///< 66      SDRAM Adaptive Refresh Management Level C First Byte First SDRAM
  SPD5_ARFM_BYTE_2_STRUCT             tARFM_C_Byte2;                   ///< 67      SDRAM Adaptive Refresh Management Level C Second Byte First SDRAM
  UINT8                               Reserved68;                      ///< 68      SDRAM Adaptive Refresh Management Level C First Byte Second SDRAM - not used in this MRC
  UINT8                               Reserved69;                      ///< 69      SDRAM Adaptive Refresh Management Level C Second Byte Second SDRAM - not used in this MRC
  UINT8                               Reserved3[125 - 70 + 1];         ///< 70-125  Reserved
  SPD_CYCLIC_REDUNDANCY_CODE          Crc;                             ///< 126-127 Cyclical Redundancy Code (CRC)
} SPD5_BASE_SECTION;

typedef struct {
  UINT8                               ModulePartNumber[348 - 329 + 1]; ///< 329-348 Module Part Number
} SPD4_MODULE_PART_NUMBER;

typedef struct {
  UINT8                               ManufactureSpecificData[381 - 353 + 1]; ///< 353-381 Manufacturer's Specific Data
} SPD4_MANUFACTURE_SPECIFIC;

typedef UINT8                         SPD4_MODULE_REVISION_CODE;///< 349     Module Revision Code
typedef UINT8                         SPD4_DRAM_STEPPING;       ///< 352     Dram Stepping

typedef struct {
  SPD_UNIQUE_MODULE_ID                ModuleId;                 ///< 512-520 Unique Module ID
  SPD5_MODULE_PART_NUMBER             ModulePartNumber;         ///< 521-550 Module Part Number
  SPD5_MODULE_REVISION_CODE           ModuleRevisionCode;       ///< 551     Module Revision Code
  SPD_MANUFACTURER_ID_CODE            DramIdCode;               ///< 552-553 Dram Manufacturer ID Code
  SPD5_DRAM_STEPPING                  DramStepping;             ///< 554     Dram Stepping
  SPD5_MANUFACTURE_SPECIFIC           ManufactureSpecificData;  ///< 555-637 Manufacturer's Specific Data
  UINT8                               Reserved[639 - 638 + 1];  ///< 638-639 Reserved
} SPD5_MANUFACTURING_DATA;

typedef union {
  struct {
    UINT32 CL3                                 :  1;  ///< Bits 0:0
    UINT32 CL6                                 :  1;  ///< Bits 1:1
    UINT32 CL8                                 :  1;  ///< Bits 2:2
    UINT32 CL9                                 :  1;  ///< Bits 3:3
    UINT32 CL10                                :  1;  ///< Bits 4:4
    UINT32 CL11                                :  1;  ///< Bits 5:5
    UINT32 CL12                                :  1;  ///< Bits 6:6
    UINT32 CL14                                :  1;  ///< Bits 7:7
    UINT32 CL16                                :  1;  ///< Bits 8:8
    UINT32                                     :  1;  ///< Bits 9:9
    UINT32 CL20                                :  1;  ///< Bits 10:10
    UINT32 CL22                                :  1;  ///< Bits 11:11
    UINT32 CL24                                :  1;  ///< Bits 12:12
    UINT32                                     :  1;  ///< Bits 13:13
    UINT32 CL28                                :  1;  ///< Bits 14:14
    UINT32                                     :  1;  ///< Bits 15:15
    UINT32 CL32                                :  1;  ///< Bits 16:16
    UINT32                                     :  1;  ///< Bits 17:17
    UINT32 CL36                                :  1;  ///< Bits 18:18
    UINT32                                     :  1;  ///< Bits 19:19
    UINT32 CL40                                :  1;  ///< Bits 20:20
    UINT32                                     :  1;  ///< Bits 21:21
    UINT32 CL48                                :  1;  ///< Bits 22:22
    UINT32 CL52                                :  1;  ///< Bits 23:23
    UINT32 CL56                                :  1;  ///< Bits 24:24
    UINT32 CL60                                :  1;  ///< Bits 25:25
    UINT32                                     :  1;  ///< Bits 26:26
    UINT32 CL68                                :  1;  ///< Bits 27:27
    UINT32                                     :  4;  ///< Bits 31:28
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SPD_LPDDR_CAS_LATENCIES_SUPPORTED_STRUCT;

typedef union {
  struct {
    UINT8  SignalLoading                       :  2; ///< Bits 1:0
    UINT8  ChannelsPerPkg                      :  2; ///< Bits 3:2
    UINT8  DiePerSdramPkg                      :  3; ///< Bits 6:4
    UINT8  SdramPkgType                        :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_LPDDR_SDRAM_PACKAGE_TYPE_STRUCT;

typedef union {
  struct {
    UINT8  Reserved                            :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_LPDDR_MODULE_NOMINAL_VOLTAGE_STRUCT;

typedef union {
  struct {
    UINT8  ReadLatencyMode                     :  2; ///< Bits 1:0
    UINT8  WriteLatencySet                     :  2; ///< Bits 3:2
    UINT8                                      :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_LPDDR_RW_LATENCY_OPTION_STRUCT;

typedef struct {
  SPD_DEVICE_DESCRIPTION_STRUCT             Description;              ///< 0       Number of Serial PD Bytes Written / SPD Device Size / CRC Coverage 1, 2
  SPD_REVISION_STRUCT                       Revision;                 ///< 1       SPD Revision
  SPD_DRAM_DEVICE_TYPE_STRUCT               DramDeviceType;           ///< 2       DRAM Device Type
  SPD_MODULE_TYPE_STRUCT                    ModuleType;               ///< 3       Module Type
  SPD4_SDRAM_DENSITY_BANKS_STRUCT           SdramDensityAndBanks;     ///< 4       SDRAM Density and Banks
  SPD_SDRAM_ADDRESSING_STRUCT               SdramAddressing;          ///< 5       SDRAM Addressing
  SPD_LPDDR_SDRAM_PACKAGE_TYPE_STRUCT       SdramPackageType;         ///< 6       SDRAM Package Type
  SPD_PTRR_SUPPORT_STRUCT                   pTRRsupport;              ///< 7       pTRR support with TMAC value - SDRAM Optional Features
  SPD4_SDRAM_THERMAL_REFRESH_STRUCT         ThermalAndRefreshOptions; ///< 8       SDRAM Thermal and Refresh Options
  UINT8                                     Reserved0[10 - 9 + 1];    ///< 9-10    Reserved
  SPD_LPDDR_MODULE_NOMINAL_VOLTAGE_STRUCT   ModuleNominalVoltage;     ///< 11      Module Nominal Voltage, VDD
  SPD_MODULE_ORGANIZATION_STRUCT            ModuleOrganization;       ///< 12      Module Organization
  SPD_MODULE_MEMORY_BUS_WIDTH_STRUCT        ModuleMemoryBusWidth;     ///< 13      Module Memory Bus Width
  SPD_MODULE_THERMAL_SENSOR_STRUCT          ModuleThermalSensor;      ///< 14      Module Thermal Sensor
  UINT8                                     Reserved1[16 - 15 + 1];   ///< 15-16   Reserved
  SPD4_TIMEBASE_STRUCT                      Timebase;                 ///< 17      Timebases
  SPD_TCK_MIN_MTB_STRUCT                    tCKmin;                   ///< 18      SDRAM Minimum Cycle Time (tCKmin)
  SPD4_TCK_MAX_MTB_STRUCT                   tCKmax;                   ///< 19      SDRAM Maximum Cycle Time (tCKmax)
  SPD_LPDDR_CAS_LATENCIES_SUPPORTED_STRUCT  CasLatencies;             ///< 20-23   CAS Latencies Supported
  SPD_TAA_MIN_MTB_STRUCT                    tAAmin;                   ///< 24      Minimum CAS Latency Time (tAAmin)
  SPD_LPDDR_RW_LATENCY_OPTION_STRUCT        LatencySetOptions;        ///< 25      Read and Write Latency Set Options
  SPD_TRCD_MIN_MTB_STRUCT                   tRCDmin;                  ///< 26      Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TRP_AB_MTB_STRUCT                     tRPab;                    ///< 27      Minimum Row Precharge Delay Time (tRPab), all banks
  SPD_TRP_PB_MTB_STRUCT                     tRPpb;                    ///< 28      Minimum Row Precharge Delay Time (tRPpb), per bank
  SPD_TRFC_AB_MTB_STRUCT                    tRFCab;                   ///< 29-30   Minimum Refresh Recovery Delay Time (tRFCab), all banks
  SPD_TRFC_PB_MTB_STRUCT                    tRFCpb;                   ///< 31-32   Minimum Refresh Recovery Delay Time (tRFCpb), per bank
  UINT8                                     Reserved2[119 - 33 + 1];  ///< 33-119  Reserved
  SPD_TRP_PB_FTB_STRUCT                     tRPpbFine;                ///< 120     Fine Offset for Minimum Row Precharge Delay Time (tRPpb), per bank
  SPD_TRP_AB_FTB_STRUCT                     tRPabFine;                ///< 121     Fine Offset for Minimum Row Precharge Delay Time (tRPab), all ranks
  SPD_TRCD_MIN_FTB_STRUCT                   tRCDminFine;              ///< 122     Fine Offset for Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TAA_MIN_FTB_STRUCT                    tAAminFine;               ///< 123     Fine Offset for Minimum CAS Latency Time (tAAmin)
  SPD4_TCK_MAX_FTB_STRUCT                   tCKmaxFine;               ///< 124     Fine Offset for SDRAM Maximum Cycle Time (tCKmax)
  SPD_TCK_MIN_FTB_STRUCT                    tCKminFine;               ///< 125     Fine Offset for SDRAM Minimum Cycle Time (tCKmin)
  SPD_CYCLIC_REDUNDANCY_CODE                Crc;                      ///< 126-127 Cyclical Redundancy Code (CRC)
} SPD_LPDDR_BASE_SECTION;

typedef union {
  struct {
    UINT8  FrontThickness                      :  4; ///< Bits 3:0
    UINT8  BackThickness                       :  4; ///< Bits 7:4
  } Bits;
  UINT8 Data;
} SPD_LPDDR_MODULE_MAXIMUM_THICKNESS;

typedef union {
  struct {
    UINT8  Height                              :  5; ///< Bits 4:0
    UINT8  RawCardExtension                    :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD_LPDDR_MODULE_NOMINAL_HEIGHT;

typedef union {
  struct {
    UINT8  Card                                :  5; ///< Bits 4:0
    UINT8  Revision                            :  2; ///< Bits 6:5
    UINT8  Extension                           :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_LPDDR_REFERENCE_RAW_CARD;

typedef struct {
  SPD_LPDDR_MODULE_NOMINAL_HEIGHT         ModuleNominalHeight;      ///< 128     Module Nominal Height
  SPD_LPDDR_MODULE_MAXIMUM_THICKNESS      ModuleMaximumThickness;   ///< 129     Module Maximum Thickness
  SPD_LPDDR_REFERENCE_RAW_CARD            ReferenceRawCardUsed;     ///< 130     Reference Raw Card Used
  UINT8                                   Reserved[253 - 131 + 1];  ///< 131-253 Reserved
  SPD_CYCLIC_REDUNDANCY_CODE              Crc;                      ///< 254-255 Cyclical Redundancy Code (CRC)
} SPD_LPDDR_MODULE_LPDIMM;

typedef union {
  SPD_LPDDR_MODULE_LPDIMM                 LpDimm;                   ///< 128-255 LP-DIMM Memory Module Types
} SPD_LPDDR_MODULE_SPECIFIC;

typedef union {
  SPD_EXTREME_MEMORY_PROFILE_3_0      Xmp;                      ///< 640-1023 Intel(r) Extreme Memory Profile support
} SPD5_END_USER_SECTION;
///
/// DDR5 Common SPD Bytes for All Module Types
///
typedef struct {
  SPD_REVISION_STRUCT                     Revision;                 ///< 192     SPD Revision for SPD bytes 192-447
  UINT8                                   Reserved0;                ///< 193     Reserved
  SPD5_DEVICE_INFO                        DeviceInfoSpd;            ///< 194-197 SPD Device Information
  SPD5_DEVICE_INFO                        DeviceInfoPmic[3];        ///< 198-209 PMIC Device Information
  SPD5_THERM_DEVICE_INFO                  DeviceInfoThermalSensor;  ///< 210-213 Thermal Sensor Device Information
  UINT8                                   Reserved1[229 - 214 + 1]; ///< 214-229 Reserved
  SPD5_MODULE_NOMINAL_HEIGHT              ModuleNominalHeight;      ///< 230     Module Nominal Height
  SPD5_MODULE_MAXIMUM_THICKNESS           ModuleMaximumThickness;   ///< 231     Module Maximum Thickness
  SPD5_REFERENCE_RAW_CARD                 ReferenceRawCardUsed;     ///< 232     Reference Raw Card Used
  SPD5_DIMM_ATTRIBUTES                    DimmAttributes;           ///< 233     DIMM Attributes
  SPD5_MODULE_ORGANIZATION                ModuleOrganization;       ///< 234     Module Organization
  SPD5_MODULE_MEMORY_BUS_WIDTH            ModuleMemoryBusWidth;     ///< 235     Memory Channel Bus Width
  UINT8                                   Reserved2[239 - 236 + 1]; ///< 236-239 Reserved
  SPD5_MODULE_SPECIFIC                    ModuleSpecific;           ///< 240-445 Module Type Specific Information
  SPD_CYCLIC_REDUNDANCY_CODE              Crc;                      ///< 446-447 Cyclical Redundancy Code (CRC)
} SPD5_MODULE_COMMON;

typedef struct {
  SPD_UNIQUE_MODULE_ID                    ModuleId;                 ///< 320-328 Unique Module ID
  SPD4_MODULE_PART_NUMBER                 ModulePartNumber;         ///< 329-348 Module Part Number
  SPD4_MODULE_REVISION_CODE               ModuleRevisionCode;       ///< 349     Module Revision Code
  SPD_MANUFACTURER_ID_CODE                DramIdCode;               ///< 350-351 Dram Manufacturer ID Code
  SPD4_DRAM_STEPPING                      DramStepping;             ///< 352     Dram Stepping
  SPD4_MANUFACTURE_SPECIFIC               ManufactureSpecificData;  ///< 353-381 Manufacturer's Specific Data
  UINT8                                   Reserved[383 - 382 + 1];  ///< 382-383 Reserved
} SPD_LPDDR_MANUFACTURING_DATA;

typedef union {
  UINT8                                   Reserved0[511 - 384 + 1]; ///< 384-511 End User Programmable
} SPD_LPDDR_END_USER_SECTION;

typedef struct {
  SPD_LPDDR_BASE_SECTION                  Base;                     ///< 0-127   Base Configuration and DRAM Parameters
  SPD_LPDDR_MODULE_SPECIFIC               Module;                   ///< 128-255 Module-Specific Section
  UINT8                                   Reserved0[319 - 256 + 1]; ///< 256-319 Reserved
  SPD_LPDDR_MANUFACTURING_DATA            ManufactureInfo;          ///< 320-383 Manufacturing Information
  SPD_LPDDR_END_USER_SECTION              EndUser;                  ///< 384-511 End User Programmable
} MrcSpdLpddr;

///
/// DDR5 Serial Presence Detect structure
///
typedef struct {
  SPD5_BASE_SECTION                   Base;                     ///< 0-127    Base Configuration and DRAM Parameters
  UINT8                               Reserved0[191 - 128 + 1]; ///< 128-191  Reserved
  SPD5_MODULE_COMMON                  ModuleCommon;             ///< 192-447  Module-Specific Section
  UINT8                               Reserved1[511 - 448 + 1]; ///< 448-511  Reserved
  SPD5_MANUFACTURING_DATA             ManufactureInfo;          ///< 512-639  Manufacturing Information
  SPD5_END_USER_SECTION               EndUser;                  ///< 640-1023 End User Programmable
} MrcSpdDdr5;

typedef union {
  MrcSpdDdr5  Ddr5;
  MrcSpdLpddr Lpddr;
} MrcSpd;


#ifndef MAX_SPD_SAVE
#define MAX_SPD_SAVE (sizeof (SPD_MANUFACTURER_ID_CODE) + \
                      sizeof (SPD_MANUFACTURING_LOCATION) + \
                      sizeof (SPD_MANUFACTURING_DATE) + \
                      sizeof (SPD_MANUFACTURER_SERIAL_NUMBER) + \
                      sizeof (SPD4_MODULE_PART_NUMBER))
#endif

#pragma pack (pop)
#endif // _MrcSpdData_h_
