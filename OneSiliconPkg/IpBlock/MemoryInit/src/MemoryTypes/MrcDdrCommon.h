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
  Read LPDDR information from MR5 and MR8 and print to the debug log.
  Also update the Manufacturer's ID in the SPD table, for BIOS Setup and SMBIOS table.

  @param[in] MrcData - include all the MRC general data.

  @retval none
**/
void
ShowLpddrInfo (
  IN  MrcParameters *const MrcData
  );

#endif // MRC_DDR_COMMON_H_
