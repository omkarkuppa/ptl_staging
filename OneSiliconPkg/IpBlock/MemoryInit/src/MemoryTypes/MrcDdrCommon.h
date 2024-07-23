/** @file
  Defines functions and constants that are used to interact with various
  DDR technologies.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#ifndef MRC_DDR_COMMON_H_
#define MRC_DDR_COMMON_H_

#include "CMrcInterface.h"  // for MrcParameters

///
/// LPDDR De-rate Timing addition 1.875ns
///
#define DERATE_TIMING_PS 1875
#define MAX_DERATE_VALUE 8

///
/// Defines and Enumeration
///
typedef enum {
  MRC_ZQ_LATCH, ///< LPDDR: ZQ Latch with tZQLAT                 DDR5: Not used
  MRC_ZQ_CAL,   ///< LPDDR: ZQ Cal and ZQ Latch (tZQCAL, tZQLAT) DDR5: ZQ Cal and ZQ Latch (tZQCAL, tZQLAT)
  MRC_ZQ_MAX
} MrcZqType;

typedef enum {
  oi1DPC1R  = 0,
  oi1DPC2R,
  oi1DPC0R1R,
  oi1DPC0R2R,
  oi2DPC1R1R,
  oi2DPC1R2R,
  oi2DPC2R1R,
  oi2DPC2R2R,
  oiNotValid
} TOdtIndex;

/// Common LPDDR5 Definitions
/// CA, DQ, and SOC ODT fields share the same encoding.
typedef enum {
  OdtOff,  ///< Disabled, Reset default
  Odt240,  ///< RZQ/1
  Odt120,  ///< RZQ/2
  Odt80,   ///< RZQ/3
  Odt60,   ///< RZQ/4
  Odt48,   ///< RZQ/5
  Odt40,   ///< RZQ/6
  OdtMax
} LPDDR_ODT_ENCODING;

#define MAX_ODT_VALUE 2

typedef enum {
  OdtCs80,   ///< RZQ/3, Reset default
  OdtCs120,  ///< RZQ/2
  OdtCs240,  ///< RZQ/1
  OdtCsMax
} LPDDR_CS_ODT_ENCODING;

typedef struct {
  UINT16  RttWr;
  UINT16  RttWck;
  UINT16  RttCa;
  UINT16  RttCa2RByteMode;
  UINT16  RttCs;
  UINT16  RttCs2RByteMode;
  UINT16  RttNT;
} TOdtValueLpddr;

///
/// Common Defines and Macros
///
/// tFC_Long = 250ns.  Common for LPDDR5
#define MRC_LP_tFC_LONG_NS (250)

#define LP5_READ_LATENCY_VALUES       15
#define LP5_READ_LATENCY_VALUES_DVFSC 6

///
/// Function Declarations
///

/**
  Calculate the tCL value for LPDDR5.
  JEDEC Spec Table 220 - Read Latencies for Read Link ECC off case (DVFSC disabled)

    Lower Clk   Upper Clk        Read Latency
    Freq Limit  Freq Limit     Set0  Set1  Set2
    -------------------------------------------
      5            67           3      3      3
     67           133           4      4      4
    133           200           5      5      6
    200           267           6      7      7
    267           344           8      8      9
    344           400           9     10     10
    400           467          10     11     12
    467           533          12     13     14
    533           600          13     14     15
    600           688          15     16     17
    688           750          16     17     19
    750           800          17     18     20
    800           937.5        20     22     24
    937.5        1066.5        23     25     26

    Set0 - x16, No DBI
    Set1 - x8 and No DBI, or x16 and DBI
    Set2 - x8 and DBI

    @param[in] tCK          - The memory tCK in femtoseconds.
    @param[in] SdramWidth   - SDRAM width (8 or 16)
    @param[in] IsDbiEnabled - TRUE if DBI is enabled
    @param[in] IsDvfscEnabled - TRUE if Dvfsc is enabled

    @retval LPDDR5 tCL in tCK units
**/
UINT32
GetLpddr5tCL (
  IN const UINT32     tCK,
  IN UINT8            SdramWidth,
  IN BOOLEAN          IsDbiEnabled,
  IN BOOLEAN          IsDvfscEnabled
  );

/**
  This function converts from the integer defined Read Latency to the Mode Register
  encoding of the timing in LPDDR5.

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Value   - Requested Read Latency value.
  @param[out] EncVal  - Encoded Mode Register value.

  @retval MrcStatus - mrcSuccess if the latency is supported.  Else mrcWrongInputParameter.
**/
MrcStatus
EncodeReadLatencyLpddr5 (
  IN  MrcParameters *MrcData,
  IN  UINT16        Value,
  OUT UINT8         *EncVal
  );

/**
  Print non-LP MRs

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess
**/
MrcStatus
MrcPrintDdrMrs (
  IN MrcParameters *const MrcData
  );

/**
This function converts from LPDDR5 MR ODT encoding to Ohms.

@param[in]  DecodeVal - Decoded ODT value.

@retval INT16 - ODT Value in Ohms if valid. Else, -1.
**/
INT16
LpddrOdtDecode (
  IN  UINT16 DecodeVal
  );

/**
  This function converts from Ohms to LPDDR5 MR ODT encoding.

  @param[in]  OdtValue  - ODT Value in Ohms.

  @retval INT8 - Encoding if valid ODT value.  Else, -1.
**/
LPDDR_ODT_ENCODING
LpddrOdtEncode (
  IN  UINT16  OdtValue
  );

/**
  This function converts from Ohms to LPDDR5 MR CS ODT encoding.

  @param[in]  OdtValue  - ODT Value in Ohms.

  @retval INT8 - Encoding if valid ODT value.  Else, -1.
**/
LPDDR_CS_ODT_ENCODING
LpddrCsOdtEncode (
  IN  UINT16  OdtValue
  );

/**
  This function searches the list of valid ODT encodings for LPDDR5 to find the closest matching
  ODT.  The SOC ODT and the SOC_ODT programming to the LPDDR5 MR must match or unexpected behavior
  may occur.  It will return this value to the caller.

  @param[in]  MrcData - Pointer to the global MRC data
  @param[in]  LpRzqValues - Pointer to Rzq Value array for this LPDDR type
  @param[in]  LpRzqNumValues - Number of values in the Rzq Value array for this LPDDR type
  @param[in]  PuCalSocOdtValid - Pointer to PuCal SocOdt Valid array for this LPDDR type
  @param[in]  SocOdt  - The ODT requested by the system

  @retval - Closest matching ODT value.
**/
UINT16
MrcCheckSocOdtLpddr (
  IN  MrcParameters *const  MrcData,
  IN  const UINT16         *LpRzqValues,
  IN  UINT32                LpRzqNumValues,
  IN  const BOOLEAN        *PuCalSocOdtValid,
  IN  UINT16                SocOdt
  );

/**
  This function programs the CkeOn field based on population.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval none.
**/
void
MrcCkeOnProgramming (
  IN  MrcParameters *const  MrcData
  );

/**
  This function programs the CkeOverride field based on population.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval none.
**/
void
MrcCkeOverrideProgramming (
  IN  MrcParameters *const  MrcData
  );

/**
  This function will set WCK2DQI (LP5) / DQS (DDR5) Interval Timer Run Time to MR37 (LP5) / MR45 (DDR5).

  @param[in]      MrcData       - Pointer to global MRC data.
  @param[in]      DqioDuration  - WCK2DQI/DQS interval timer run time.

  @retval MrcStatus - mrcSuccess if the value is supported, else mrcWrongInputParameter.
**/
MrcStatus
MrcSetIntervalTimerMr (
  IN      MrcParameters *const  MrcData,
  IN      UINT8                 DqioDuration
  );

#endif // MRC_DDR_COMMON_H_
