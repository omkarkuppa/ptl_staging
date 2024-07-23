/** @file
  This module includes the power modes definitions.

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
#ifndef _MrcPowerModes_h_
#define _MrcPowerModes_h_
#pragma pack(push, 1)

#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "CMcAddress.h"
#include "MrcCommon.h"

//
// Lookup table definitions
//
#define VDD_120     1
#define VDD_135     2
#define VDD_150     3
#define VDD_OTHER   4
#define ECC_F       0
#define ECC_T       1
#define TYPE_SODIMM 3
#define TYPE_UDIMM  2
#define WIDTH_4     0
#define WIDTH_8     1
#define WIDTH_16    2
#define WIDTH_32    3
#define RANKS_1     1
#define RANKS_2     2
#define DPC_1       1
#define DPC_2       2
#define FREQ_1067   3
#define FREQ_1200   4
#define FREQ_1333   5
#define FREQ_1400   6
#define FREQ_1600   7
#define FREQ_1800   8
#define FREQ_1867   9
#define FREQ_2000   10
#define FREQ_2133   11
#define FREQ_2200   12
#define FREQ_2400   13
#define FREQ_2600   14
#define FREQ_2667   15
#define DENSITY_2   3
#define DENSITY_4   4
#define DENSITY_8   5
#define DIMM_POSITION_1  0
#define DIMM_POSITION_2  1

typedef enum {
  tsmNoThermalSensing = 0,  ///< No thermal sensing in MC
  tsmThermalSensor,         ///< Thermal Sensor (on DIMM) - when set thermal sense is active
  tsmBwEstimation,          ///< BW estimation - when set, PM_SUM_PC_CxRy of this DIMM accumulates command power estimation
  tsmBoth                   ///< Both (1) and (2)
} ThermalSensorModes;

///
/// Power Down mode
///
typedef enum {
  pdmNoPowerDown      = 0,
  pdmAPD              = 1,
  pdmPPD              = 2,
  pdmPPDDLLOFF        = 6,
  pdmAuto             = 0xFF,
} MrcPowerDownMode;

typedef union {
  struct {
    UINT32 Vddq        :  4;
    UINT32 Ecc         :  1;
    UINT32 DimmType    :  4;
    UINT32 DeviceWidth :  3;
    UINT32 NumOfRanks  :  3;
    UINT32 Dpc         :  2;
    UINT32 Frequency   :  4;
    UINT32 DramDensity :  3;
    UINT32 Spare       :  8;
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8 Data8[4];
} PowerWeightInputs;

typedef struct {
  PowerWeightInputs PwInput;
  UINT8             ScaleFactor;
  UINT8             RdCr;
  UINT8             WrCr;
  UINT8             ActCr;
  UINT8             CkeL;
  UINT8             CkeH;
  UINT8             ChPwrFloor;
} DdrPowerWeightEntry;

typedef struct {
  PowerWeightInputs PwInput;
  UINT8             ScaleFactor;
  UINT8             RdCr;
  UINT8             WrCr;
  UINT8             ActCr;
  UINT8             CkeL;
  UINT8             CkeH;
  UINT8             OneChPwrFloor;
  UINT8             TwoChPwrFloor;
} Lpddr3PowerWeightEntry;

#define PDWN_IDLE_COUNTER_ULX_ULT   (0x400) // 1024
#define PDWN_IDLE_COUNTER_DT_HALO   (0x80)

#define MCDECS_CBIT_DEFAULT (0x00000000)

/**
  This function configure the MC power register post training after normal mode before PCU start working.

  @param[in, out] MrcData - Include all MRC global data.

  @retval Nothing.
**/
extern
void
MrcPowerModesPostTraining (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function configures the power down control register.

  @param[in]  - MrcData - The MRC global data.

  @retval - Nothing
**/
extern
void
MrcPowerDownConfig (
  IN MrcParameters *const MrcData
  );

#pragma pack(pop)
#endif // _MrcPowerModes_h_
