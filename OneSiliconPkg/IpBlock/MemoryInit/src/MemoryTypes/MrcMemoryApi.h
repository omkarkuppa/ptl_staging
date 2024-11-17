/** @file
This API provides an interface to abstract out memory protocol differences.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _MRC_MEMORY_API_H_
#define _MRC_MEMORY_API_H_

#include "CMrcApi.h"  // Indirectly provides "IN" and "OUT"
#include "MrcChipApi.h"
#include "MrcLpddr5Registers.h"


///
/// HW limits of timing parameters
///
#define MRC_tPRPDEN_MIN 4U

///
/// tWPRE values.
///
#define tWPRE_ALL_FREQ      (1) ///< tWPRE is = 1 tCK for all frequencies up to 1600.

///
/// tVrefDQ in [ps] - delay between each MR10 in DDR5 PDA mode
///
#define tVREF_DQ_PS      500000

///
/// DRAM Temperature/Voltage Drifts
///
#define MAX_TEMP_DRIFT      40
#define MAX_NOISE_PERCENT   4

#define NOREPAIR_PPR        0x0
#define SOFT_PPR            0x1
#define HARD_PPR            0x2

#define PPR_TEST_DETECTION_LOOPS              8
#define PPR_REPAIR_RETRY_LOOPS                10

///
/// Enumeration Types
///
typedef enum {
  LpWrOdt,
  LpNtRdOdt,
  LpOdtTypeMax
} LPDDR_ODT_TYPE;

/// ODTLon Timing Array Index.
typedef enum {
  LpOdtlOn,
  LpOdtlOff,
  LpOdtlMax
} LPDDR_ODTL_PARAM;

typedef enum {
  LpVrcgNormal,
  LpVrcgHighCurrent,
  LpVrcgMax
} LPDDR_VRCG_TYPE;

typedef enum {
  LpFspOpPoint0,
  LpFspOpPoint1, //< LPDDR4 supports only 2 points
  LpFspOpPoint2, //< LPDDR5 only
  LpFspOpMax
} LPDDR_FSP_OP_TYPE;

typedef enum {
  LpFspWePoint0,
  LpFspWePoint1,
  LpFspWePoint2,
  LpFspWeMax
} LPDDR_FSP_WRITE_EN;

typedef struct {
  MrcModeRegister MRnum;
  UINT16 Data;
} MR_DATA_STRUCT;

typedef enum {
  MrcConfigSetZqReset,
  MrcConfigSetZqStop,
  MrcConfigEndZqStop,
  MrcConfigZqMax
} MrcConfigZqType;


typedef enum {
  MrDramCmdMrw, // Must align with GmfCmdMrw  = 0,
  MrDramCmdMpc, // Must align with GmfCmdMpc  = 1,
  MrcDramVref,  // Must align with GmfCmdVref = 2
  MrDramCmdMrr,
  MrDramCmdNop,
  MrDramCmdPrea,
  MrDramCmdWsFs,  // LPDDR5 Only
  MrDramCmdWsOff, // LPDDR5 Only
  MrDramCmdCs2nSyncNop, // DDR5 Only
  MrDramCmdSre,   // LPDDR5 Only
  MrDramCmdMax
} MrDramCmdType;

// This enum is used to map BRC value to Blast Radius Configuration (BRC) in MR75.OP[5:4] (LP5) or MR59.OP[2:1] (DDR5)
typedef enum {
  JedecBrc2 = 0,
  JedecBrc3,
  JedecBrc4
} MrcJedecBrcType;

///
/// Functions
///

/**
 * This functions maps MrcRhDrfmBrcType to MrcJedecBrcType.
 *
 * @param[in] DrfmBrc BRC value as MrcRhDrfmBrcType
 *
 * @returns Convert BRC value as MrcJedecBrcType
**/
MrcJedecBrcType
ConvertDrfmBrcToTMrcJedecBrcType (
  MrcRhDrfmBrcType BrcVal
  );

/**
  This function returns the timing value for Write Preamble.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval UINT8 - The delay in tCK.
**/
UINT8
MrcGetTwpre (
  IN MrcParameters *const MrcData
  );

/**
  This function looks up the delay timing associated with the input TimingIndex.
  If no timing is associated with the TimingIndex then a value of 0 is returned on
  TimingNckOut pointer.

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[out] TimingIndex  - The delay index to look up
  @param[out] TimingNckOut - Output pointer for the delay timing associated with the input DelayIndex in tCK units

  @retval MrcStatus - mrcSuccess if no errors
**/
MrcStatus
MrcGetGmfDelayTiming (
  IN      MrcParameters *const  MrcData,
  IN      GmfTimingIndex        TimingIndex,
  OUT     UINT16                *TimingNckOut
  );
/**
  This function returns tWCKPRE_Static for both Writes and Reads

  @param[in]  MrcData   - Pointer to MRC global data.

  @retval INT8 - Timing in tCK, or 0 if unsupported for the DDR type.
**/
UINT8
MrcGetWckPreStatic (
  IN  MrcParameters *const  MrcData
  );

/**
  This function returns tWCKENL_FS for the current DRAM configuration.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval UINT8 - Timing in tCK, or 0 if unsupported.
**/
UINT8
MrcGetWckEnlFs (
  IN  MrcParameters *const  MrcData
  );

/**
  This function returns tWCKPRE_total_WR for the current DRAM configuration.

  @param[in]  MrcData   - Pointer to MRC global data.

  @retval UINT8 - Timing in tCK, or 0 if unsupported for the DDR type.
**/
UINT8
MrcGetWckPreWrTotal (
  IN  MrcParameters *const  MrcData
  );

/**
  This function returns tWCKPRE_total_RD for the current DRAM configuration.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval UINT8 - Timing in tCK, or 0 if unsupported for the DDR type.
**/
UINT8
MrcGetWckPreRdTotal (
  IN  MrcParameters *const  MrcData
  );

/**
  This function determines if the SOC ODT requested is valid for the memory technology.
  If, the requested ODT is not supported, it will return the the closest matching ODT.
  If there is no limitation by the memory technology or the ODT value is supported,
  it will return the requested ODT.

  @param[in]  MrcData - Pointer to the global MRC data
  @param[in]  SocOdt  - The ODT requested by the system

  @retval - Valid ODT value.
**/
UINT16
MrcCheckForSocOdtEnc (
  IN  MrcParameters *const  MrcData,
  IN  UINT16                SocOdt
  );

/**
  This function handles calling the correct Init Mrw sequence based on DDR Technology.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus
**/
MrcStatus
MrcMrwInit (
  IN  MrcParameters *MrcData
  );

/**
  This function handles calling the correct JEDEC Reset sequence based on DDR Technology.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus
**/
MrcStatus
MrcJedecReset (
  IN  MrcParameters *MrcData
  );

/**
  This function handles calling the correct JEDEC Init sequence based on DDR Technology.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus
**/
MrcStatus
MrcJedecInit (
  IN  MrcParameters *MrcData
  );

/**
  Configure LPDDR5 DRAM ZQ calibration settings in MR28.

  After configuration, wait appropriate delay.
  Finally, if ending ZQ stop, also send ZQ latch and wait tZQLAT.
  Not saving ZQ Stop status in host structure, after ZQ Stop the next call to this function is expected to be MrcConfigEndZqStop.

  @param[in]  MrcData     - Pointer to MRC global data.
  @param[in]  RankMask    - Bit mask of Rank's to configure.
  @param[in]  ZqRequest   - Options are MrcConfigSetZqReset, MrcConfigSetZqStop, or MrcConfigEndZqStop.


  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcConfigureZq (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 RankMask,
  IN  UINT8                 ZqRequest
  );

/**
  Program LPDDR5 FSP-OP and FSP-WR values.
  It will set the ranks specified in the rank mask in all Channels and Controllers populated.
  NOTE:
        Encoding for Vrcg MR field is the same for LPDDR5.
          So we only need to pass through the external type defined in MrcMemoryApi.h.
          If this changes, then this function will need to translate between MRC enumeration to MR enumeration.
        FSP points for LPDDR5 are integer points.  LPDDR5 supports 3 points.

  @param[in]  MrcData     - Pointer to MRC global data.
  @param[in]  RankMask    - Bit mask of Rank's to configure.
  @param[in]  VrcgMode    - VREF Current Generator mode switch.
  @param[in]  FspWePoint  - FSP-WR. Valid Values: LP5 - 0:2
  @param[in]  FspOpPoint  - FSP-OP. Valid Values: LP5 - 0:2

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcSetFspVrcg (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 RankMask,
  IN  UINT8                 VrcgMode,
  IN  UINT8                 FspWePoint,
  IN  UINT8                 FspOpPoint
  );
/**
  GetOptDimmParamMrIndex using Technology specific GetOptDimmParamMrIndex functions

  @param[in]  MrcData     - Include all MRC global data.
  @param[in]  OptParam    - The Dimm Opt Param (e.g., OptDimmRon, OptDimmOdtWr, OptDimmOdtPark, OptDimmOdtNom)
  @param[out] *MrIndex    - Updated Pointer to the MR index.
  @param[out] *MrNum      - Updated Pointer to the MR number.

  @retval MrcStatus - mrcWrongInputParameter if unsupported Technology
                      Status of the Technology specific functions otherwise
**/
MrcStatus
GetOptDimmParamMrIndex (
  IN MrcParameters *const MrcData,
  IN UINT8                OptDimmParam,
  OUT UINT8               *MrIndex,
  OUT UINT16              *MrNum
  );

/**
  Determine the ODT table for the given Controller / Channel / DIMM.

  @param[in]     MrcData         - Include all the mrc global data.
  @param[in]     Profile         - Memory Profile.
  @param[in]     Controller      - Controller to work on.
  @param[in]     Channel         - Channel to work on.
  @param[in]     Dimm            - DIMM to work on.
  @param[in out] OdtTable        - ODT structure - or (TOdtValueDdr5 *) or (TOdtValueLpddr *)

  @retval Pointer to the relevant ODT structure - or (TOdtValueDqDdr5 *) or (TOdtValueLpddr *)
**/
extern
MrcStatus
GetProfileOdtTableIndex (
  IN MrcParameters *const MrcData,
  IN const MrcProfile     Profile,
  IN const UINT32         Controller,
  IN const UINT32         Channel,
  IN const UINT32         Dimm,
  IN OUT   void           *OdtTable
  );

/**
  Determine the ODT table for the given Controller / Channel / DIMM.

  @param[in]     MrcData         - Include all the mrc global data.
  @param[in]     Controller      - Controller to work on.
  @param[in]     Channel         - Channel to work on.
  @param[in]     Dimm            - DIMM to work on.
  @param[in out] OdtTable        - ODT structure - or (TOdtValueDdr5 *) or (TOdtValueLpddr *)

  @retval Pointer to the relevant ODT structure - or (TOdtValueDqDdr5 *) or (TOdtValueLpddr *)
**/
extern
MrcStatus
GetOdtTableIndex (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel,
  IN const UINT32         Dimm,
  IN OUT   void           *OdtTable
  );

/**
  This function returns the tXP value for the specified frequency in tCK.

  @param[in] DdrType   - DDR type
  @param[in] tCK       - Clock period in femtoseconds.

  @retval The tXP value for the specified frequency.
**/
UINT32
MrcGetTxp (
  IN  MrcDdrType  DdrType,
  IN  UINT32      tCK
  );

/**
  This function returns the value of tXSR in tCK based on DdrType.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Timing     - Pointer to timing values.

  @retval UINT32 - tXSR in tCK.
**/
UINT32
MrcGetTxsr (
  IN MrcParameters *const MrcData,
  IN MrcTiming *const     Timing
  );

/**
  This function returns the tMOD value.
  For DDR5 this is for "tRMD"

  @param[in] MrcData   - Include all MRC global data.
  @param[in] tCK       - DCLK period in femtoseconds.

  @retval UINT32 - Timing in tCK.
**/
UINT32
MrcGetTmod (
  IN MrcParameters *const MrcData,
  IN const UINT32         tCK
  );

/**
  This function returns the tMRW value.
  @param[in] MrcData   - Include all MRC global data.
  @param[in] tCK       - DCLK period in femtoseconds.
  @retval UINT32 - Timing in tCK.
**/
UINT32
MrcGettMRW (
  IN MrcParameters *const MrcData,
  IN const UINT32         tCK
  );

/**
  This function returns the tZQCAL value in DCLKs.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] tCK      - DCLK period in femtoseconds.

  @retval UINT32 - Timing in nCK.
**/
UINT32
MrcGetTzqcal (
  IN MrcParameters *const MrcData,
  IN UINT32               tCK
  );

/**
  This function returns the tZQLAT value in DCLKs.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] tCK      - DCLK period in femtoseconds.

  @retval UINT32 - Timing in tCK.
**/
UINT32
MrcGetTzqlat (
  IN MrcParameters *const MrcData,
  IN UINT32               tCK
  );

/**
  This function returns the tZQCS value.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] tCK      - DCLK period in femtoseconds.

  @retval UINT32 - Value in tCK or WCK (LP5).
**/
UINT32
MrcGetTzqcs (
  IN MrcParameters *const MrcData,
  IN UINT32               tCK
  );

/**
  This function returns type of DimmOdt for DDR5 based on OdtMatrix and Read/Write mode

  Returns OptDimmOdtPark if OdtMatrix is 0
          OptDimmOdtNomRd if OdtMatrix is 1 and OdtType is ReadOdt
          OptDimmOdtNomWr if OdtMatrix is 1 and OdtType is WriteOdt

  @param[in]      MrcData       - Include all MRC global data.
  @param[in]      Controller    - Controller to setup
  @param[in]      Channel       - Channel to setup
  @param[in]      Rank          - Rank to setup
  @param[in]      OdtType       - ReadOdt or WriteOdt
  @param[in]      NtRank        - Non Target Rank

  @retval OdtParam
**/
UINT8
MrcGetOdtParam (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT8                OdtType,
  IN UINT8                NtRank
  );

/**
  This function returns the tCKE value for the specified frequency.

  @param[in] DdrType    - DDR type
  @param[in] tCK        - DCLK period in femtoseconds.

  @retval The tCKE value for the specified frequency.
**/
UINT32
MrcGetTcke (
  IN MrcDdrType         DdrType,
  IN UINT32             tCK
  );

/**
  Used to update VrefDQ or VrefCA.
  Uses input offset value to increment/decrement current setting.

  @param[in,out] MrcData        - Include all MRC global data.
  @param[in]     Controller     - Selecting which Memory Controller to talk to.
  @param[in]     Channel        - Selecting which Channel to talk to.
  @param[in]     RankMask       - Selecting which Ranks to talk to.
  @param[in]     DeviceMask     - Selecting which Devices to talk to (only valid for DDR5 and adjusting VrefDQ).
  @param[in]     VrefType       - Determines the Vref type to change, only CmdV and TxVref are valid.
  @param[in]     Offset         - Vref offset value.
  @param[in]     UpdateMrcData  - Used to decide if Mrc host must be updated.
  @param[in]     PdaMode        - Selecting to use MRH or old method for MRS (only valid for DDR5 and adjusting VrefDQ).
  @param[in]     IsCachedOffset - Determines if the parameter is an offset (relative to cache) or absolute value.
**/
void
MrcSetDramVref (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8                Controller,
  IN     UINT8                Channel,
  IN     UINT8                RankMask,
  IN     UINT16               DeviceMask,
  IN     UINT8                VrefType,
  IN     INT32                Offset,
  IN     BOOLEAN              UpdateMrcData,
  IN     BOOLEAN              PdaMode,
  IN     BOOLEAN              IsCachedOffset
  );

/**
  This function returns the tXSDLL in DCLKs based on DDR Type, and Frequency.
  DDR5 has values per frequency, and LPDDR5 do not have
  a DLL.  From the spec, we must wait tXSR before issuing a command.  Thus,
  we set tXSDLL to tXSR to ensure tXSR before read/write commands.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Frequency  - The memory frequency.
  @param[in] tXSR       - Value of tXSR.
**/
UINT32
MrcGetTxsdll (
  IN MrcParameters *const MrcData,
  IN const MrcFrequency   Frequency,
  IN const INT64          tXSR
  );

/**
  This function returns the tRDPDEN value for the specified Memory type.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] tCL      - CAS Latency in tCK.

  @returns The tRDPDEN value for the specified configuration.
**/
UINT32
MrcGetTrdpden (
  IN MrcParameters *const MrcData,
  IN UINT32               tCL
  );

/**
  This function returns the tWRPDEN value for the specified Memory type.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] tCWL     - WR CAS Latency in tCK.
  @param[in] tWR      - Write recovery time in tCK.

  @returns The tWRPDEN value for the specified configuration.
**/
UINT32
MrcGetTwrpden (
  IN MrcParameters *const MrcData,
  IN UINT32               tCWL,
  IN UINT32               tWR
  );

/**
  This function returns the tCPDED value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tCPDED value for the specified configuration.
**/
UINT32
MrcGetTcpded (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the tCKCKEH value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tCKCKEH value for the specified configuration.
**/
UINT32
MrcGetTckckeh (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the tCACSH value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tCACSH value for the specified configuration.
**/
UINT32
MrcGetTcacsh (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the tWCKOFF value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tWCKOFF value for the specified configuration.
**/
UINT32
MrcGetTwckoff (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the BL/n_min value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The BL/n_min value for the specified configuration.
**/
UINT32
MrcGetBurstLengthNMin (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the BL/n_max value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The BL/n_max value for the specified configuration.
**/
UINT32
MrcGetBurstLengthNMax (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the trd_wck_async_gap value for the specified Memory type.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] tCL      - CAS Latency in tCK.

  @returns The trd_wck_async_gap value for the specified configuration.
**/
UINT32
MrcGetTrdWckAsyncGap (
  IN MrcParameters *const MrcData,
  IN UINT32               tCL
  );

/**
  This function returns the twr_wck_async_gap value for the specified Memory type.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] tCWL     - WR CAS Latency in tCK.

  @returns The twr_wck_async_gap value for the specified configuration.
**/
UINT32
MrcGetTwrWckAsyncGap (
  IN MrcParameters *const MrcData,
  IN UINT32               tCWL
  );

/**
  This function returns the tWCKSTOP value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tWCKSTOP value for the specified configuration.
**/
UINT32
MrcGetTwckstop (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the tCSLCK value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tCSLCK value for the specified configuration.
**/
UINT32
MrcGetTcsclk (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the tCKFSPX value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tCKFSPX value for the specified configuration.
**/
UINT32
MrcGetTckfspx (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the tVRCG_ENABLE value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tVRCG_ENABLE value for the specified configuration.
**/
UINT32
MrcGetTvrcgEnable (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the tVRCG_DISABLE value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tVRCG_DISABLE value for the specified configuration.
**/
UINT32
MrcGetTvrcgDisable (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the tRCDw value.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] tRCD      - RAS to CAS Latency in tCK.

  @returns The tRCDw value for the specified configuration.
**/
UINT32
MrcGetTrcdw (
  IN MrcParameters *const MrcData,
  IN UINT32               tRCD
  );

/**
  This function returns the tMRR value.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] tCL        - CAS Latency in tCK.

  @retval UINT32 - Timing in tCK / wCK (LP5).
**/
UINT32
MrcGetTmrr (
  IN MrcParameters *const MrcData,
  IN const UINT32         tCL
  );

/**
  This function returns the Write to MRR value.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] tCWL       - CAS Latency in tCK.
  @param[in] tWTR       - WRITE-to-READ delay.
  @param[in] tWTR_L     - WRITE-to-READ delay long.

  @retval UINT32 - Timing in tCK / wCK (LP5).
**/
UINT32
MrcGetWrToMrr (
  IN MrcParameters *const MrcData,
  IN const UINT32         tCWL,
  IN const UINT32         tWTR,
  IN const UINT32         tWTR_L
  );

/**
  This function returns the timing value for Precharge to Precharge delay.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval UINT8 - 0 for unsupported technologies, otherwise the delay in tCK
**/
UINT8
MrcGetTppd (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the tCSH value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tCSH value for the specified configuration.
**/
UINT32
MrcGetTcsh (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the tCSL value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tCSL value for the specified configuration.
**/
UINT32
MrcGetTcsl (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the tccd_32_byte_cas_delta value.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] Timing   - Timing Settings.

  @returns The tccd_32_byte_cas_delta value for the specified configuration.
**/
UINT32
MrcGetTccdByteCasDelta (
  IN MrcParameters *const MrcData,
  IN MrcTiming     *Timing
  );

/**
  This function returns the tSR value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tSR value for the specified configuration.
**/
UINT32
MrcGetTsr (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the tOSCO value.

  @param[in] MrcData    - Include all MRC global data.

  @retval UINT32 - Timing in tCK.
**/
UINT32
MrcGetTosco (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the tWRPRE value.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] tCWL     - WR CAS Latency in tCK.
  @param[in] tWR      - Write recovery time in tCK.

  @returns tWRPRE timing in tCK (WCK for LP5)
**/
UINT32
MrcGetTwrpre (
  IN MrcParameters *const MrcData,
  IN const UINT32         tCWL,
  IN const UINT32         tWTR
);

/**
  This function returns tRRFpb in WCK/CK.

  @param[in] MrcData       - Include all MRC global data.
  @param[in] DeviceDensity - Device density in MB.

  @returns tRRFpb timing in WCK/CK.
**/
UINT32
MrcGetTrrfpb (
  IN MrcParameters *const MrcData,
  IN const UINT32         DeviceDensity
  );

/**
  This function returns tRRFsb in WCK/CK.

  @param[in] MrcData       - Include all MRC global data.

  @returns tRRFsb timing in WCK/CK.
**/
UINT32
MrcGetTrrfsb (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the ODTL timing.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  Frequency - Current data rate.
  @param[in]  OdtType   - Selects which command ODT type.
  @param[in]  OdtlParam - Determines if this is for ODT On or Off.

  @retval INT8 - ODT impact to the command timing.
**/
INT8
MrcGetOdtlTiming (
  IN  MrcParameters *const  MrcData,
  IN  MrcFrequency          Frequency,
  IN  LPDDR_ODT_TYPE        OdtType,
  IN  LPDDR_ODTL_PARAM      OdtlParam
  );

/**
  This function determines if NT ODT for WR and/or RD are enabled on any ranks within a channel

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Current Controller
  @param[in] Channel    - Current Channel
  @param[out] NTODTWr   - TRUE if any rank within the channel supports NT WR ODT
  @param[out] NTODTRd   - TRUE if any rank within the channel supports NT RD ODT
**/
VOID
IsNtOdtSupported (
  IN MrcParameters *const  MrcData,
  IN UINT32                Controller,
  IN UINT32                Channel,
  OUT BOOLEAN              *NTODTWr,
  OUT BOOLEAN              *NTODTRd
  );


#endif // _MRC_MEMORY_API_H_
