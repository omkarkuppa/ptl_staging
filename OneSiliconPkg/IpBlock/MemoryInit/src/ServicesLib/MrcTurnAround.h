/** @file
    This file contains all the turnaround training algorithm definitions.

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
#ifndef _MrcTurnaround_h_
#define _MrcTurnaround_h_

//
// Include files
//
#include "MrcCommon.h"
#include "MrcDdrIoUtils.h"

#define PI_PER_TCK       (128)

#define TAT_COUNT        (16)
#define TAT_SG_DG_MAX    (8)
#define TAT_SAFE_OFFSET_DR (8)
#define TAT_SAFE_OFFSET_SR (4)

/// CK2CK (pS)
/// Guardband any flight time between 2 ranks
#define MRC_LP5_TCK2CK (50)

/// Structure to store turnaround timings
typedef struct {
  UINT32 tRdRdsg;
  UINT32 tRdRddg;
  UINT32 tWrWrsg;
  UINT32 tWrWrdg;
  UINT32 tRdWrsg;
  UINT32 tRdWrdg;
  UINT32 tWrRdsg;
  UINT32 tWrRddg;
  UINT32 tRdRddr;
  UINT32 tRdRddd;
  UINT32 tWrWrdr;
  UINT32 tWrWrdd;
  UINT32 tRdWrdr;
  UINT32 tRdWrdd;
  UINT32 tWrRddr;
  UINT32 tWrRddd;
} TurnAroundTimings;

/**
  This function calculates the tRdRd (sg,dg,dr,dd) for LPDDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[in] DeltaRcvEnPiCode  - Largest Delta of RcvEnPiCode across Bytes across the Channel
  @param[out] tRdRdsg          - Return calculated tRdRdsg Timing
  @param[out] tRdRddg          - Return calculated tRdRddg Timing
  @param[out] tRdRddr          - Return calculated tRdRddr Timing
  @param[out] tRdRddd          - Return calculated tRdRddd Timing
**/
VOID
GetLpddr5tRdRd (
  IN   MrcParameters *const MrcData,
  IN   UINT32               DeltaRcvEnPiCode,
  OUT  UINT32               *tRdRdsg,
  OUT  UINT32               *tRdRddg,
  OUT  UINT32               *tRdRddr,
  OUT  UINT32               *tRdRddd
  );

/**
  This function calculates the tWRWR (sg,dg,dr,dd) for LPDDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[out] tWrWrsg          - Return calculated tWRWRdg Timing
  @param[out] tWrWrdg          - Return calculated tWRWRdg Timing
  @param[out] tWrWrdr          - Return calculated tWRWRdr Timing
  @param[out] tWrWrdd          - Return calculated tWRWRdd Timing
**/
VOID
GetLpddr5tWrWr (
  IN   MrcParameters *const MrcData,
  OUT  UINT32               *tWrWrsg,
  OUT  UINT32               *tWrWrdg,
  OUT  UINT32               *tWrWrdr,
  OUT  UINT32               *tWrWrdd
  );

/**
  This function calculates the tRDWR (sg,dg,dr,dd) for LPDDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[out] tRdWrsg          - Return calculated tRDWRsg Timing
  @param[out] tRdWrdg          - Return calculated tRDWRdg Timing
  @param[out] tRdWrdr          - Return calculated tRDWRdr Timing
  @param[out] tRdWrdd          - Return calculated tRDWRdd Timing
**/
VOID
GetLpddr5tRdWr (
  IN   MrcParameters *const MrcData,
  OUT  UINT32               *tRdWrsg,
  OUT  UINT32               *tRdWrdg,
  OUT  UINT32               *tRdWrdr,
  OUT  UINT32               *tRdWrdd
  );

/**
  This function calculates the tWRRD (sg,dg,dr,dd) for LPDDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[out] tWrRdsg          - Return calculated tWRRDsg Timing
  @param[out] tWrRddg          - Return calculated tWRRDdg Timing
  @param[out] tWrRddr          - Return calculated tWRRDdr Timing
  @param[out] tWrRddd          - Return calculated tWRRDdd Timing
**/
VOID
GetLpddr5tWrRd (
  IN   MrcParameters *const MrcData,
  OUT  UINT32               *tWrRdsg,
  OUT  UINT32               *tWrRddg,
  OUT  UINT32               *tWrRddr,
  OUT  UINT32               *tWrRddd
  );

/**
  This function calculates the tRdRd (sg,dg,dr,dd) for DDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[in] DeltaRcvEnPiCode  - Largest Delta of RcvEnPiCode across Bytes across the Channel
  @param[out] tRdRdsg          - Return calculated tRdRdsg Timing
  @param[out] tRdRddg          - Return calculated tRdRddg Timing
  @param[out] tRdRddr          - Return calculated tRdRddr Timing
  @param[out] tRdRddd          - Return calculated tRdRddd Timing
**/
VOID
GetDdr5tRdRd (
  IN MrcParameters *const  MrcData,
  IN  UINT32               DeltaRcvEnPiCode,
  OUT  UINT32              *tRdRdsg,
  OUT  UINT32              *tRdRddg,
  OUT  UINT32              *tRdRddr,
  OUT  UINT32              *tRdRddd
  );

/**
  This function calculates the tWRWR (sg,dg,dr,dd) for DDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[in] MaxOdtWrOff       - Max ODT WR Off Offset across all Ranks of a channel (MR37:OP[5:3])
  @param[in] MinOdtWrOn        - Max ODT WR On Offset across all Ranks of a channel MR37:OP[2:0])
  @param[out] tWrWrsg          - Return calculated tWRWRsg Timing
  @param[out] tWrWrdg          - Return calculated tWRWRdg Timing
  @param[out] tWrWrdr          - Return calculated tWRWRdr Timing
  @param[out] tWrWrdd          - Return calculated tWRWRdd Timing
**/
VOID
GetDdr5tWrWr (
  IN MrcParameters *const  MrcData,
  IN  INT32                MaxOdtWrOff,
  IN  INT32                MinOdtWrOn,
  OUT  UINT32              *tWrWrsg,
  OUT  UINT32              *tWrWrdg,
  OUT  UINT32              *tWrWrdr,
  OUT  UINT32              *tWrWrdd
  );

/**
  This function calculates the tRDWR (sg,dg,dr,dd) for DDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[in] DeltaTxDqsPiCode  - Largest Delta of TxDqsPiCode across Bytes across the Channel
  @param[in] MaxOdtRdOff       - Max ODT Rd Off Offset across all Ranks of a channel (MR39:OP[5:3])
  @param[in] MinOdtWrOn        - Max ODT WR On Offset across all Ranks of a channel MR37:OP[2:0])
  @param[out] NTODTRd          - TRUE if NT RD ODT is enabled on any ranks within the Channel
  @param[out] tRDWRsg          - Return calculated tRDWRsg Timing
  @param[out] tRDWRdg          - Return calculated tRDWRdg Timing
  @param[out] tRDWRdr          - Return calculated tRDWRdr Timing
  @param[out] tRDWRdd          - Return calculated tRDWRdd Timing
**/
VOID
GetDdr5tRdWr (
  IN MrcParameters *const  MrcData,
  IN  UINT32               DeltaTxDqsPiCode,
  IN  INT32                MaxOdtRdOff,
  IN  INT32                MinOdtWrOn,
  IN  BOOLEAN              NTODTRd,
  OUT  UINT32              *tRdWrsg,
  OUT  UINT32              *tRdWrdg,
  OUT  UINT32              *tRdWrdr,
  OUT  UINT32              *tRdWrdd
  );

/**
  This function calculates the tWRRD (sg,dg,dr,dd) for DDR5 based on Jedec specifications

  @param[in] MrcData           - Include all MRC global data.
  @param[in] DeltaTxDqsPiCode  - Largest Delta of TxDqsPiCode across Bytes across the Channel
  @param[in] MinOdtRdOn        - Max ODT Rd On Offset across all Ranks of a channel (MR39:OP[2:0])
  @param[in] MaxOdtWrOff       - Max ODT WR Off Offset across all Ranks of a channel MR37:OP[5:3])
  @param[out] NTODTRd          - TRUE if NT RD ODT is enabled on any ranks within the Channel
  @param[out] tWRRDsg          - Return calculated tWRRDsg Timing
  @param[out] tWRRDdg          - Return calculated tWRRDdg Timing
  @param[out] tWRRDdr          - Return calculated tWRRDdr Timing
  @param[out] tWRRDdd          - Return calculated tWRRDdd Timing
**/
VOID
GetDdr5tWrRd (
  IN MrcParameters *const  MrcData,
  IN  UINT32               DeltaTxDqsPiCode,
  IN  INT8                 MinOdtRdOn,
  IN  INT8                 MaxOdtWrOff,
  IN  BOOLEAN              NTODTRd,
  OUT  UINT32              *tWrRdsg,
  OUT  UINT32              *tWrRddg,
  OUT  UINT32              *tWrRddr,
  OUT  UINT32              *tWrRddd
  );

/**
  This function calculates the Rd2Rd Tunraround timing based on training results
  Phy constraint that tells us what the min value acceptable by phy

  @param[in] MrcData - Include all MRC global data.
  @param[in] RxPathResults - Holds the max/min Rx Path margins for each Channel
  @param[OUT] PHYRd2Rd_PI  - returns the Rd2Rd turnaround timings
**/
VOID
GetPhyRd2RdTA (
  IN  MrcParameters *const MrcData,
  IN  RxPathMinMax  *const RxPathResults,
  OUT UINT32              PHYRd2Rd_PI[MAX_CONTROLLER][MAX_CHANNEL]
  );

/**
  This function calculates the Wr2Wr Tunraround timing based on training results

  @param[in]  MrcData - Include all MRC global data.
  @param[in]  TxPathMinMax - Holds the max/min Tx Path margins for each Channel
  @param[OUT] PHYWr2Wr_PI - returns the Wr2Wr turnaround timings
**/
VOID
GetPhyWr2WrTA (
  IN  MrcParameters *const MrcData,
  IN  TxPathMinMax  *const TxPathResults,
  OUT UINT32               PHYWr2Wr_PI[MAX_CONTROLLER][MAX_CHANNEL]
  );

/**
  This function gets the Pre Training Turnaround Values

  @param[in]  MrcData - Include all MRC global data.
  @param[out] TAT     - return struct which holds TAT values

  @retval None
**/
VOID
GetDramTatPreTraining (
  IN  MrcParameters *const   MrcData,
  OUT TurnAroundTimings      *TAT
  );

/**
  This function gets the Post Training Turnaround Values

  @param[in]  MrcData       - Include all MRC global data.
  @param[in]  RxPathResults - Struct contianing Min/Max Data for Rx Data Path
  @param[in]  TxPathResults - Struct contianing Min/Max Data for Tx Data Path
  @param[in]  Controller    - Current Controller
  @param[in]  Channel       - Current Channel
  @param[out] TAT           - return struct which holds TAT values

  @retval None
**/
VOID
GetDramTatPostTraining (
  IN  MrcParameters *const   MrcData,
  IN  RxPathMinMax  * RxPathResults,
  IN  TxPathMinMax  * TxPathResults,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  OUT TurnAroundTimings    *TAT
  );

/**
  This function gets and sets the Pre Training Turnaround Values

  @param[in]  MrcData - Include all MRC global data.

  @retval None
**/
VOID
MrcTurnAroundTimingsPreTraining (
  IN  MrcParameters *const MrcData
  );

/**
  This function sets the Turnaround Timings based on the incoming Input Struct (TAT)

  @param[in]      MrcData       - Include all MRC global data.
  @param[in]      TAT           - Struct to hold TAT values
  @param[in]      Controller    - Controller to setup
  @param[in]      Channel       - Channel to setup
  @param[in]      IsPreTraining - BOOLEAN to indicate if TAT values should be programmed for Pre Trianing or Post Trianing TAT Optimization

  @retval None
**/
VOID
SetDramTatTimings (
  IN MrcParameters* const  MrcData,
  IN TurnAroundTimings     *TAT,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  BOOLEAN              IsPreTraining
  );

#endif // _MrcTurnAround_h_
