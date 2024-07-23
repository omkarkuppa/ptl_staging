/** @file
  CKD Control Word header file.

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

  @par Specification Reference: DDR5CK01 Rev0.7
**/

#ifndef MRC_DDR5_CKD_H_
#define MRC_DDR5_CKD_H_
#pragma pack (push, 1)

// CKD Register Control Word

// RW00 CKD Configuration
#define CKD_CONFIGURATION_REG                                        (0x00000000)
#define CKD_CONFIGURATION_DEF                                        (0xF0)
  #define CKD_CONFIGURATION_PllMode_MIN                              (0)
  #define CKD_CONFIGURATION_PllMode_MAX                              (2)
  #define CKD_CONFIGURATION_PllMode_HSH                              (0x02000000)

  #define CKD_CONFIGURATION_IctInputClkTermination_MIN               (0)
  #define CKD_CONFIGURATION_IctInputClkTermination_MAX               (3)
  #define CKD_CONFIGURATION_IctInputClkTermination_HSH               (0x02040000)

  #define CKD_CONFIGURATION_DisQck0AClk_MIN                          (0)
  #define CKD_CONFIGURATION_DisQck0AClk_MAX                          (1)
  #define CKD_CONFIGURATION_DisQck0AClk_HSH                          (0x01080000)

  #define CKD_CONFIGURATION_DisQck1AClk_MIN                          (0)
  #define CKD_CONFIGURATION_DisQck1AClk_MAX                          (1)
  #define CKD_CONFIGURATION_DisQck1AClk_HSH                          (0x010A0000)

  #define CKD_CONFIGURATION_DisQck0BClk_MIN                          (0)
  #define CKD_CONFIGURATION_DisQck0BClk_MAX                          (1)
  #define CKD_CONFIGURATION_DisQck0BClk_HSH                          (0x010C0000)

  #define CKD_CONFIGURATION_DisQck1BClk_MIN                          (0)
  #define CKD_CONFIGURATION_DisQck1BClk_MAX                          (1)
  #define CKD_CONFIGURATION_DisQck1BClk_HSH                          (0x010E0000)

typedef union {
  struct {
    UINT8  PllMode                             :  2; // Bits 1:0
    UINT8  IctInputClkTermination              :  2; // Bits 3:2
    UINT8  DisQck0AClk                         :  1; // Bits 4:4
    UINT8  DisQck1AClk                         :  1; // Bits 5:5
    UINT8  DisQck0BClk                         :  1; // Bits 6:6
    UINT8  DisQck1BClk                         :  1; // Bits 7:7
  } Bits;
  UINT8  Data;
} CKD_CONFIGURATION_STRUCT;

// RW01 Output Delay Control Enable
#define CKD_OUTPUT_DELAY_REG                                         (0x00000001)
  #define CKD_OUTPUT_DELAY_Qck0AOutputDelay_MIN                      (0)
  #define CKD_OUTPUT_DELAY_Qck0AOutputDelay_MAX                      (1)
  #define CKD_OUTPUT_DELAY_Qck0AOutputDelay_HSH                      (0x01000001)

  #define CKD_OUTPUT_DELAY_Qck1AOutputDelay_MIN                      (0)
  #define CKD_OUTPUT_DELAY_Qck1AOutputDelay_MAX                      (1)
  #define CKD_OUTPUT_DELAY_Qck1AOutputDelay_HSH                      (0x01020001)

  #define CKD_OUTPUT_DELAY_Qck0BOutputDelay_MIN                      (0)
  #define CKD_OUTPUT_DELAY_Qck0BOutputDelay_MAX                      (1)
  #define CKD_OUTPUT_DELAY_Qck0BOutputDelay_HSH                      (0x01040001)

  #define CKD_OUTPUT_DELAY_Qck1BOutputDelay_MIN                      (0)
  #define CKD_OUTPUT_DELAY_Qck1BOutputDelay_MAX                      (1)
  #define CKD_OUTPUT_DELAY_Qck1BOutputDelay_HSH                      (0x01060001)

typedef union {
  struct {
    UINT8  Qck0AOutputDelay                    :  1; // Bits 0:0
    UINT8  Qck1AOutputDelay                    :  1; // Bits 1:1
    UINT8  Qck0BOutputDelay                    :  1; // Bits 2:2
    UINT8  Qck1BOutputDelay                    :  1; // Bits 3:3
    UINT8                                      :  4; // Bits 7:4
  } Bits;
  UINT8  Data;
} CKD_OUTPUT_DELAY_STRUCT;

// RW02 QCK Signals Driver Characteristics
#define CKD_QCK_SIGNALS_DRIVER_REG                                   (0x00000002)
  #define CKD_QCK_SIGNALS_DRIVER_Qck0ADrive_MIN                      (0)
  #define CKD_QCK_SIGNALS_DRIVER_Qck0ADrive_MAX                      (3)
  #define CKD_QCK_SIGNALS_DRIVER_Qck0ADrive_HSH                      (0x02000002)

  #define CKD_QCK_SIGNALS_DRIVER_Qck1ADrive_MIN                      (0)
  #define CKD_QCK_SIGNALS_DRIVER_Qck1ADrive_MAX                      (3)
  #define CKD_QCK_SIGNALS_DRIVER_Qck1ADrive_HSH                      (0x02040002)

  #define CKD_QCK_SIGNALS_DRIVER_Qck0BDrive_MIN                      (0)
  #define CKD_QCK_SIGNALS_DRIVER_Qck0BDrive_MAX                      (3)
  #define CKD_QCK_SIGNALS_DRIVER_Qck0BDrive_HSH                      (0x02080002)

  #define CKD_QCK_SIGNALS_DRIVER_Qck1BDrive_MIN                      (0)
  #define CKD_QCK_SIGNALS_DRIVER_Qck1BDrive_MAX                      (3)
  #define CKD_QCK_SIGNALS_DRIVER_Qck1BDrive_HSH                      (0x020C0002)

typedef union {
  struct {
    UINT8  Qck0ADrive                          :  2; // Bits 1:0
    UINT8  Qck1ADrive                          :  2; // Bits 3:2
    UINT8  Qck0BDrive                          :  2; // Bits 5:4
    UINT8  Qck1BDrive                          :  2; // Bits 7:6
  } Bits;
  UINT8  Data;
} CKD_QCK_SIGNALS_DRIVER_STRUCT;

// RW03 QCK Output Differential Slew Rate
#define CKD_QCK_SLEW_RATE_REG                                        (0x00000003)
  #define CKD_QCK_SLEW_RATE_Qck01ASlewRate_MIN                       (0)
  #define CKD_QCK_SLEW_RATE_Qck01ASlewRate_MAX                       (2)
  #define CKD_QCK_SLEW_RATE_Qck01ASlewRate_HSH                       (0x02000003)

  #define CKD_QCK_SLEW_RATE_Qck01BSlewRate_MIN                       (0)
  #define CKD_QCK_SLEW_RATE_Qck01BSlewRate_MAX                       (3)
  #define CKD_QCK_SLEW_RATE_Qck01BSlewRate_HSH                       (0x02080003)

typedef union {
  struct {
    UINT8  Qck01ASlewRate                      :  2; // Bits 1:0
    UINT8                                      :  2; // Bits 3:2
    UINT8  Qck01BSlewRate                      :  2; // Bits 5:4
    UINT8                                      :  2; // Bits 7:6
  } Bits;
  UINT8  Data;
} CKD_QCK_SLEW_RATE_STRUCT;

// RW04 - RW07 QCKn Output Delay Range
#define CKD_QCK0_OUTPUT_DELAY_RANGE_REG                              (0x00000004)
  #define CKD_QCK0_OUTPUT_DELAY_RANGE_QckOutputDelay_MIN             (0)
  #define CKD_QCK0_OUTPUT_DELAY_RANGE_QckOutputDelay_MAX             (24)
  #define CKD_QCK0_OUTPUT_DELAY_RANGE_QckOutputDelay_HSH             (0x05000004)

#define CKD_QCK1_OUTPUT_DELAY_RANGE_REG                              (0x00000005)
  #define CKD_QCK1_OUTPUT_DELAY_RANGE_QckOutputDelay_MIN             (0)
  #define CKD_QCK1_OUTPUT_DELAY_RANGE_QckOutputDelay_MAX             (31)
  #define CKD_QCK1_OUTPUT_DELAY_RANGE_QckOutputDelay_HSH             (0x05000005)

#define CKD_QCK2_OUTPUT_DELAY_RANGE_REG                              (0x00000006)
  #define CKD_QCK2_OUTPUT_DELAY_RANGE_QckOutputDelay_MIN             (0)
  #define CKD_QCK2_OUTPUT_DELAY_RANGE_QckOutputDelay_MAX             (31)
  #define CKD_QCK2_OUTPUT_DELAY_RANGE_QckOutputDelay_HSH             (0x05000006)

#define CKD_QCK3_OUTPUT_DELAY_RANGE_REG                              (0x00000007)
  #define CKD_QCK3_OUTPUT_DELAY_RANGE_QckOutputDelay_MIN             (0)
  #define CKD_QCK3_OUTPUT_DELAY_RANGE_QckOutputDelay_MAX             (31)
  #define CKD_QCK3_OUTPUT_DELAY_RANGE_QckOutputDelay_HSH             (0x05000007)

typedef union {
  struct {
    UINT8  QckOutputDelay                     :  5; // Bits 4:0
    UINT8                                     :  3; // Bits 7:5
  } Bits;
  UINT8  Data;
} CKD_QCKn_OUTPUT_DELAY_RANGE_STRUCT;

// RW08 PLL Status
#define CKD_PLL_STATUS_REG                                           (0x0000008)

typedef union {
  struct {
    UINT8                                      :  8; // Bits 7:0
  } Bits;
  UINT8  Data;
} CKD_PLL_STATUS_STRUCT;

// RW28 I2C & I3C Basic Error Status
#define CKD_I2C_I3C_ERROR_STATUS_REG                                 (0x00000028)
  #define CKD_I2C_I3C_ERROR_STATUS_ErrorParityChk_MIN                (0)
  #define CKD_I2C_I3C_ERROR_STATUS_ErrorParityChk_MAX                (1)
  #define CKD_I2C_I3C_ERROR_STATUS_ErrorParityChk_HSH                (0x01000028)

  #define CKD_I2C_I3C_ERROR_STATUS_ErrorPacketErrChk_MIN             (0)
  #define CKD_I2C_I3C_ERROR_STATUS_ErrorPacketErrChk_MAX             (1)
  #define CKD_I2C_I3C_ERROR_STATUS_ErrorPacketErrChk_HSH             (0x01020028)

  #define CKD_I2C_I3C_ERROR_STATUS_DeviceEventInBandStatus_MIN       (0)
  #define CKD_I2C_I3C_ERROR_STATUS_DeviceEventInBandStatus_MAX       (1)
  #define CKD_I2C_I3C_ERROR_STATUS_DeviceEventInBandStatus_HSH       (0x010E0028)

typedef union {
  struct {
    UINT8 ErrorParityChk                       :  1; // Bits 0:0
    UINT8 ErrorPacketErrChk                    :  1; // Bits 1:1
    UINT8                                      :  5; // Bits 6:2
    UINT8 DeviceEventInBandStatus              :  1; // Bits 7:7
  } Bits;
  UINT8  Data;
} CKD_I2C_I3C_ERROR_STATUS_STRUCT;

// RW29 I2C & I3C Basic Clear Error Status
#define CKD_I2C_I3C_CLEAR_ERROR_STATUS_REG                           (0x00000029)
  #define CKD_I2C_I3C_CLEAR_ERROR_STATUS_ClearParityErrStatus_MIN    (0)
  #define CKD_I2C_I3C_CLEAR_ERROR_STATUS_ClearParityErrStatus_MAX    (1)
  #define CKD_I2C_I3C_CLEAR_ERROR_STATUS_ClearParityErrStatus_HSH    (0x01000029)

  #define CKD_I2C_I3C_CLEAR_ERROR_STATUS_ClearPacketErrStatus_MIN    (0)
  #define CKD_I2C_I3C_CLEAR_ERROR_STATUS_ClearPacketErrStatus_MAX    (1)
  #define CKD_I2C_I3C_CLEAR_ERROR_STATUS_ClearPacketErrStatus_HSH    (0x01020029)

typedef union {
  struct {
    UINT8 ClearParityErrStatus                 :  1; // Bits 0:0
    UINT8 ClearPacketErrStatus                 :  1; // Bits 1:1
    UINT8                                      :  5; // Bits 7:2
  } Bits;
  UINT8  Data;
} CKD_I2C_I3C_CLEAR_ERROR_STATUS_STRUCT;

// RW40 Data Code Byte 0
typedef union {
  struct {
    UINT8 DateCodeDigit0                       :  4; // Bits 3:0
    UINT8 DateCodeDigit1                       :  4; // Bits 7:4
  } Bits;
  UINT8  Data;
} CKD_DATE_CODE_BYTE0_STRUCT;

// RW41 Data Code Byte 1
typedef union {
  struct {
    UINT8 DateCodeDigit2                       :  4; // Bits 3:0
    UINT8 DateCodeDigit3                       :  4; // Bits 7:4
  } Bits;
  UINT8  Data;
} CKD_DATE_CODE_BYTE1_STRUCT;

// RW42 Data Code Byte 2
typedef union {
  struct {
    UINT8 DateCodeDigit4                       :  4; // Bits 3:0
    UINT8 DateCodeDigit5                       :  4; // Bits 7:4
  } Bits;
  UINT8  Data;
} CKD_DATE_CODE_BYTE2_STRUCT;

// RW43 Data Code Byte 0
typedef union {
  struct {
    UINT8 UniqueUnitCode0                      :  8; // Bits 7:0
  } Bits;
  UINT8  Data;
} CKD_UNIT_CODE_BYTE0_STRUCT;

// RW44 Data Code Byte 1
typedef union {
  struct {
    UINT8 UniqueUnitCode1                      :  8; // Bits 7:0
  } Bits;
  UINT8  Data;
} CKD_UNIT_CODE_BYTE1_STRUCT;

// RW45 Data Code Byte 2
typedef union {
  struct {
    UINT8 UniqueUnitCode2                      :  8; // Bits 7:0
  } Bits;
  UINT8  Data;
} CKD_UNIT_CODE_BYTE2_STRUCT;

// RW46 Data Code Byte 3
typedef union {
  struct {
    UINT8 UniqueUnitCode3                      :  8; // Bits 7:0
  } Bits;
  UINT8  Data;
} CKD_UNIT_CODE_BYTE3_STRUCT;

// RW47 Data Code Byte 4
typedef union {
  struct {
    UINT8 UniqueUnitCode4                      :  8; // Bits 7:0
  } Bits;
  UINT8  Data;
} CKD_UNIT_CODE_BYTE4_STRUCT;

// RW48 Data Code Byte 5
typedef union {
  struct {
    UINT8 UniqueUnitCode5                      :  8; // Bits 7:0
  } Bits;
  UINT8  Data;
} CKD_UNIT_CODE_BYTE5_STRUCT;

// RW49 Data Code Byte 6
typedef union {
  struct {
    UINT8 UniqueUnitCode6                      :  8; // Bits 7:0
  } Bits;
  UINT8  Data;
} CKD_UNIT_CODE_BYTE6_STRUCT;

// RW4A Vendor ID Byte 0
typedef union {
  struct {
    UINT8 VendorID0                            :  8; // Bits 7:0
  } Bits;
  UINT8  Data;
} CKD_VENDOR_ID_BYTE0_STRUCT;

// RW4B Vendor ID Byte 1
typedef union {
  struct {
    UINT8 VendorID1                            :  8; // Bits 7:0
  } Bits;
  UINT8  Data;
} CKD_VENDOR_ID_BYTE1_STRUCT;

// RW4C Device ID Byte 0
typedef union {
  struct {
    UINT8 DeviceID0                            :  8; // Bits 7:0
  } Bits;
  UINT8  Data;
} CKD_DEVICE_ID_BYTE0_STRUCT;

// RW4D Device Byte 1
typedef union {
  struct {
    UINT8 DeviceID1                            :  8; // Bits 7:0
  } Bits;
  UINT8  Data;
} CKD_DEVICE_ID_BYTE1_STRUCT;

// RW4E VendorRevID
typedef union {
  struct {
    UINT8 VendorRevID                          :  8; // Bits 7:0
  } Bits;
  UINT8  Data;
} CKD_VENDOR_REVID_STRUCT;

typedef struct {
  CKD_OUTPUT_DELAY_STRUCT            OutputDelayCtlEn;          ///< RW01 Output Delay Control Enable
  CKD_QCK_SIGNALS_DRIVER_STRUCT      QckSignalsDriver;          ///< RW02 QCK Signals Driver Characteristics
  CKD_QCK_SLEW_RATE_STRUCT           QckOutputDiffSlewRate;     ///< RW03 QCK Output Differential Slew Rate
  CKD_QCKn_OUTPUT_DELAY_RANGE_STRUCT OutputDelayQck0A;          ///< RW04 Output Delay Range for QCK0_A_t and QCK0_A_c
  CKD_QCKn_OUTPUT_DELAY_RANGE_STRUCT OutputDelayQck1A;          ///< RW05 Output Delay Range for QCK1_A_t and QCK1_A_c
  CKD_QCKn_OUTPUT_DELAY_RANGE_STRUCT OutputDelayQck0B;          ///< RW06 Output Delay Range for QCK0_B_t and QCK0_B_c
  CKD_QCKn_OUTPUT_DELAY_RANGE_STRUCT OutputDelayQck1B;          ///< RW07 Output Delay Range for QCK1_B_t and QCK1_B_c
} CKD_QCK_OUTPUT;

typedef struct {
  CKD_PLL_STATUS_STRUCT                    PllStatus;                ///< RW08    PLL Status Register
  CKD_I2C_I3C_ERROR_STATUS_STRUCT          BasicErrorStatus;         ///< RW28    I2C & I3C Basic Error Status
  CKD_I2C_I3C_CLEAR_ERROR_STATUS_STRUCT    BasicClearErrorStatus;    ///< RW29    I2C & I3C Basic Clear Error Status
} CKD_STATUS;

typedef struct {
  CKD_DATE_CODE_BYTE0_STRUCT          DateCodeByte0;            ///< RW40 Date Code Byte 0
  CKD_DATE_CODE_BYTE1_STRUCT          DateCodeByte1;            ///< RW41 Date Code Byte 1
  CKD_DATE_CODE_BYTE2_STRUCT          DateCodeByte2;            ///< RW42 Date Code Byte 2
  CKD_UNIT_CODE_BYTE0_STRUCT          VendorUnitCodeByte0;      ///< RW43 Vendor Specific Unique Unit Code Byte 0
  CKD_UNIT_CODE_BYTE1_STRUCT          VendorUnitCodeByte1;      ///< RW44 Vendor Specific Unique Unit Code Byte 1
  CKD_UNIT_CODE_BYTE2_STRUCT          VendorUnitCodeByte2;      ///< RW45 Vendor Specific Unique Unit Code Byte 2
  CKD_UNIT_CODE_BYTE3_STRUCT          VendorUnitCodeByte3;      ///< RW46 Vendor Specific Unique Unit Code Byte 3
  CKD_UNIT_CODE_BYTE4_STRUCT          VendorUnitCodeByte4;      ///< RW47 Vendor Specific Unique Unit Code Byte 4
  CKD_UNIT_CODE_BYTE5_STRUCT          VendorUnitCodeByte5;      ///< RW48 Vendor Specific Unique Unit Code Byte 5
  CKD_UNIT_CODE_BYTE6_STRUCT          VendorUnitCodeByte6;      ///< RW49 Vendor Specific Unique Unit Code Byte 6
  CKD_VENDOR_ID_BYTE0_STRUCT          VendorIdByte0;            ///< RW4A Vendor ID Byte 0
  CKD_VENDOR_ID_BYTE1_STRUCT          VendorIdByte1;            ///< RW4B Vendor ID Byte 1
  CKD_DEVICE_ID_BYTE0_STRUCT          DevIdByte0;               ///< RW4C Device ID Byte 0
  CKD_DEVICE_ID_BYTE1_STRUCT          DevIdByte1;               ///< RW4D Device ID Byte 1
  CKD_VENDOR_REVID_STRUCT             VendorRevisionID;         ///< RW4E Vendor Revision ID
} CKD_MANUFACTURING_ID;

///
/// DDR5 CKD structure
///
typedef struct {
  CKD_CONFIGURATION_STRUCT            Configuration;          ///< RW00 Configuration
  CKD_QCK_OUTPUT                      QckOutput;              ///< RW01-07  QCK Output
  CKD_STATUS                          Status;                 ///< RW08-29  Status
  CKD_MANUFACTURING_ID                ManufacturingId;        ///< RW40-4E  Manufacturing ID
} MrcCkdDdr5;

typedef enum {
  BypassMode,
  SinglePllMode,
  DualPllMode
} MRC_CKD_PLLMODE;

typedef enum {
  Ict80Ohm,
  Ict60Ohm,
  Ict120Ohm
} MRC_CKD_ICT_CLKTERM;

typedef enum {
  LightDrive,
  ModerateDrive,
  StrongDrive,
  WeakDrive,
} MRC_CKD_DRIVER;

typedef enum {
  SlewRateModerate,
  SlewRateFast
} MRC_CKD_SLEWRATE;

#pragma pack (pop)
#endif // MRC_DDR5_CKD_H_
