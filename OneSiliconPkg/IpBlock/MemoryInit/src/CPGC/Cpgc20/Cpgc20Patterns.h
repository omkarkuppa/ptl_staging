/** @file
  This file implements functions for setting up the Pattern
  Generators for CPGC 2.0.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#ifndef _CPGC_20_PATTERNS_H_
#define _CPGC_20_PATTERNS_H_
#include "MrcCpgcApi.h"
///
/// Defintions
///
#define CPGC_20_NUM_DATA_PGS     (4)
#define CPGC_20_NUM_DPAT_EXTBUF (16)

/// scrubbing entries size cast
#define CPGC_20_64b_CHUNCKS_IN_32b_ENTRIES  (2)

// Polynomial tap values for address PRBS generation
#define CPGC_PRBS_2   (0x0003)
#define CPGC_PRBS_3   (0x0006)
#define CPGC_PRBS_4   (0x000C)
#define CPGC_PRBS_5   (0x0014)
#define CPGC_PRBS_6   (0x0036)
#define CPGC_PRBS_7   (0x0072)
#define CPGC_PRBS_8   (0x00B2)
#define CPGC_PRBS_9   (0x0152)
#define CPGC_PRBS_10  (0x035A)
#define CPGC_PRBS_11  (0x06E4)
#define CPGC_PRBS_12  (0x0B52)
#define CPGC_PRBS_13  (0x1592)
#define CPGC_PRBS_14  (0x25A4)
#define CPGC_PRBS_15  (0x49A4)
#define CPGC_PRBS_16  (0x9924)
#define CPGC_PRBS_17  (0x12924)
#define CPGC_PRBS_18  (0x25694)

typedef enum {
  Cpgc20InvertMode,
  Cpgc20DcMode,
  Cpgc20InvertDcModeMax,
  Cpgc20InvertDcModeDelim = MRC_INT32_MAX
} CPGC20_INVERT_DC_MODE;

/// CPGC/CADB Mux Control Mode
typedef enum {
  MrcPgMuxLmn,
  MrcPgMuxPatBuffer,
  MrcPgMuxLfsr,          // Legacy Fibonacci LFSR
  MrcPgMuxLfsrGalois,    // New Galois LFSR
  MrcPgMuxMax
} MRC_PG_UNISEQ_TYPE;

/**
  This function sets up the CPGC LFSR configuration per Outputs->McChBitMask.

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  LaneRotate  - Lane Rotate Rate, should be log2(CPGC2_BASE_REPEATS.Base_Repeats+1) for BasicVA

  @retval - MrcStatus: mrcSuccess if McChBitMask are non-zero, mrcFail if McChBitMask = 0.
**/
MrcStatus
Cpgc20LfsrCfg (
  IN  MrcParameters *const   MrcData,
  IN  UINT8                  LaneRotate
  );


/**
  This function sets the Pattern Generators' Invert/DC configuration (per Outputs->McChBitMask).
  The invert mode inverts the selected lanes' values.  The DC mode sets the
  selected lanes to a DC value.

  When rotation is enabled, once the rotation rate is met, the LaneMask is rotated from
  LSb to MSb.  Bit 63 of data is rotated into bit 0 of ECC, and bit 7 of ECC is rotated
  to Bit 0 of data.

  @param[in]  MrcData             - Pointer to global MRC data.
  @param[in]  Mode                - Selects Invert or DC mode.
  @param[in]  DcPolarity          - 0 = DC low. 1 = DC high.
  @param[in]  EnableRotate        - Enables the rotation feature for the Bit Enable Masks of ECC and Data.
  @param[in]  RotateRateExponent  - Programs the counter to rotate every 2^(RotateRateExponenet).
  @param[in]  BytegroupMapping    - Selects between 2 bytes of DPAT pattern generator to be applied to each DQ byte group.  MSB is the ECC byte group.

  @retval - MrcStatus: mrcSuccess if McChBitMask are non-zero, mrcFail if McChBitMask = 0.
**/
MrcStatus
Cpgc20SetPgInvDcCfg (
  IN  MrcParameters *const  MrcData,
  IN  CPGC20_INVERT_DC_MODE Mode,
  IN  UINT8                 DcPolarity,
  IN  BOOLEAN               EnableRotate,
  IN  UINT8                 RotateRateExponent,
  IN  UINT16                BytegroupMapping
  );

/**
  This function sets the Pattern Generators' Alternate Dpat Buffer Control. Useful for static pattern setup.

  @param[in]  MrcData             - Pointer to global MRC data.
  @param[in]  IncScale            - Exponential/Linear
  @param[in]  IncRate             - Pattern Generator's "lines" advancement rate
  @param[in]  Start               - Lane to start from
  @param[in]  Stop                - Lane to wrap ("stop") on

  @retval - MrcStatus: mrcSuccess if McChBitMask are non-zero, mrcFail if McChBitMask = 0.
**/
MrcStatus
Cpgc20SetDpatAltBufCtl (
  IN  MrcParameters *const  MrcData,
  IN  UINT16                IncScale,
  IN  UINT16                IncRate,
  IN  UINT8                 Start,
  IN  UINT8                 Stop
  );

/**
  This function sets the Pattern Generators' Dpat Buffer Control. Useful for static pattern setup.

  @param[in]  MrcData             - Pointer to global MRC data.
  @param[in]  IncScale            - Exponential/Linear
  @param[in]  IncRate             - Pattern Generator's "lines" advancement rate
  @param[in]  Start               - Lane to start from
  @param[in]  Stop                - Lane to wrap ("stop") on

  @retval - MrcStatus: mrcSuccess if McChBitMask are non-zero, mrcFail if McChBitMask = 0.
**/
MrcStatus
Cpgc20SetDpatBufCtl (
  IN  MrcParameters *const  MrcData,
  IN  UINT16                IncScale,
  IN  UINT16                IncRate,
  IN  UINT8                 Start,
  IN  UINT8                 Stop
  );

/**
  This function sets the Data and ECC Lane Mask Enables (per Outputs->McChBitMask).
  If all bits in DataLaneMask and EccLaneMask are 0, then the DC/Invert feature is disabled.

  @param[in]  MrcData       - Pointer to global MRC data.
  @param[in]  DataLaneMask  - Bit-Mask of lanes to enable in this PG.
  @param[in]  EccLaneMask   - Bit-Mask of lanes to enable for ECC in this PG.

  @retval - MrcStatus: mrcSuccess if McChBitMask are non-zero, mrcFail if McChBitMask = 0.
**/
MrcStatus
Cpgc20SetPgInvDcEn (
  IN  MrcParameters *const  MrcData,
  IN  UINT64                DataLaneMask,
  IN  UINT8                 EccLaneMask
  );

/**
  Fill the test engine with the given fixed pattern.
  In Enhanced Channel Mode only entry [0] of each chunk will be used (low 32 bit).

  @param[in] MrcData  - Include all MRC global data.
  @param[in] Pattern  - Array of chunks (64 bit), broken into two 32-bit entries (second dimension).
  Length (first dimension) can be up to NUM_DPAT_EXTBUF
  @retval Nothing
**/
VOID
Cpgc20WriteMATSFixedPattern (
  IN MrcParameters *const  MrcData,
  IN UINT32                Pattern[]
  );
#endif  //_CPGC_20_PATTERNS_H_
