/** @file
  Definition of private (static) DDR5 Specific functions,
  constants, defines, and data structures private to MrcDdr5.c

  In the future, the functions declared here should be made
  static.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
  DDR5 JEDEC Spec
**/
#ifndef MRC_DDR_5_PRIVATE_H_
#define MRC_DDR_5_PRIVATE_H_

#include "CMrcInterface.h" // For MrcParameters


// DO NOT REMOVE: Ensures this header is only included once in the code
PRIVATE_HEADER_DEFENDER(MrcDdr5Private)

/**
  Set Tx Dimm Vref absolute Value for DDR5.
  Use custom MRC range [-52:+53] :: (45% - 97.5%) * 1.1V where offset zero is 71.0%

  @param[in, out] MrcData       - Include all MRC global data.
  @param[in]      Controller    - Memory Controller Number within the processor (0-based).
  @param[in]      Channel       - Selecting which Channel to talk to
  @param[in]      Rank          - Selecting which Rank to talk to
  @param[in]      Device        - Selecting which Device to talk to (only valid when PDAmode is TRUE)
  @param[in]      VrefType       - Determines the Vref type to change, only CmdV and TxVref are valid.
  @param[in]      TxVrefOffset  - TxVref Offset to set
  @param[in]      UpdateMrcData - update MRC host struct
  @param[in]      PdaMode       - Selecting to use MPC or old method for MRS
**/
VOID
MrcSetTxVrefDdr5 (
  IN OUT MrcParameters *const MrcData,
  IN     UINT32               Controller,
  IN     UINT8                Channel,
  IN     UINT8                Rank,
  IN     UINT8                Device,
  IN     UINT8                VrefType,
  IN     INT32                TxVrefOffset,
  IN     BOOLEAN              UpdateMrcData,
  IN     BOOLEAN              PdaMode
  );

/**
  This function converts from Ohms to MR ODT encoding.

  @param[in]  OdtValue  - ODT Value in Ohms.

  @retval INT8 - Encoding if valid ODT value.  Else, -1.
**/
DDR5_MR32_CKCSCA_ODT
CccOdtEncode_DDR5 (
  IN  UINT32  OdtValue
  );

/**
  This function converts from Ohms to DDR5 MR ODT encoding.

  @param[in]  OdtValue  - ODT Value in Ohms.

  @retval INT8 - Encoding if valid ODT value.  Else, -1.
**/
DDR5_MR33_RTT_PARK
Ddr5OdtEncode (
  IN  UINT16  OdtValue
  );

/**
  This function converts from Odt Offset to DDR5 MR OdtlOn Wr encoding.

  @param[in]  OdttOnWrOffset - Offset to push/pull out/in the WriteRTT or Non-Target Write RTT enable time

  @retval INT8 - Encoding if valid ODT On Wr Offset.  Else, -1.
**/
DDR5_MR37_ODTLON_WR_OFFSET
Ddr5OdttOnWrOffset (
  IN  INT16  OdttOnWrOffset
  );

/**
  This function converts from Odt Offset to DDR5 MR OdtlOff Rd Nt encoding.

  @param[in]  OdttOffRdOffset  - Offset to push/pull out/in the Non-Target Read RTT disable time

  @retval INT8 - Encoding if valid ODT Off Rd Nt Offset.  Else, -1.
**/
INT8
Ddr5OdttOffRdNtOffset (
  IN  INT32  OdttOffRdNtOffset
  );

/**
  This function converts from DFE Tap Value to DDR5 MR encoding.

  @param[in]  DfeValue  - DFE Value

  @retval - Encoding if valid DFE value.  Else, 0.
**/
DDR5_MODE_REGISTER_129_TYPE
Ddr5DfeEncode (
  IN  INT16  DfeValue
  );

/**
  This function returns the DFE Setting for MR111.

  @param[out] DfeGain   - DFE Gain Enable.
  @param[out] DfeTap1   - DFE Tap1 Enable.
  @param[out] DfeTap2   - DFE Tap2 Enable.
  @param[out] DfeTap3   - DFE Tap3 Enable.
  @param[out] DfeTap4   - DFE Tap4 Enable.

  @retval MrcStatus - mrcSuccess
**/
MrcStatus
MrcDdr5GetDFE (
  OUT UINT8          *DfeGain,
  OUT UINT8          *DfeTap1,
  OUT UINT8          *DfeTap2,
  OUT UINT8          *DfeTap3,
  OUT UINT8          *DfeTap4
  );

/**
  Enter / exit Ddr5 CS Training mode:
  If ENABLE:
    - Send DDR5 "Enter CS Training Mode" MPC
  Else // DISABLE
    - Send DDR5 "Exit CS Training Mode" MPC

  @param[in] MrcData      - The MRC global data.
  @param[in] McChBitMask  - McChannels to work on.
  @param[in] Rank         - Ranks to work on.
  @param[in] Enable       - Enable or disable CBT.
  @param[in] IsPrint      - Controls Print output

  @retval mrcSuccess if succeeded
**/
MrcStatus
Ddr5CsTrainMode (
  IN MrcParameters *const  MrcData,
  IN UINT8                 McChBitMask,
  IN UINT32                Rank,
  IN UINT8                 Enable,
  IN BOOLEAN               IsPrint
  );


/**
  Determine the ODT table for the given Memory Controller / Channel / DIMM.

  If Inputs has user-input CCC non-0 values, use user inputs instead of
  from the default table

  @param[in]     MrcData         - Include all the mrc global data.
  @param[in]     Controller      - Controller to work on.
  @param[in]     Channel         - Channel to work on.
  @param[in]     Dimm            - DIMM to work on.
  @param[in out] CCCTable        - CCC structure

  @retval mrcSuccess Data referenced by CCCTable updated
  @retval mrcFail Data referenced by CCCtable not updated
**/
MrcStatus
GetDdr5CCCTableIndex (
  IN MrcParameters *const     MrcData,
  IN const UINT32             Controller,
  IN const UINT32             Channel,
  IN const UINT32             Dimm,
  IN OUT   TOdtValueCccDdr5   *CCCTable
  );


/**
  Determine the Dimm DFE table for the given Memory Controller / Channel / DIMM.

  If Inputs has user-input Dimm DFE non-0 values, use user inputs instead of
  from the default table

  @param[in]     MrcData              - Include all the mrc global data.
  @param[in]     Controller           - Controller to work on.
  @param[in]     Channel              - Channel to work on.
  @param[in]     Dimm                 - DIMM to work on.
  @param[in out] TDFEValueDdr5Table   - Point to TDFEValueDdr5 structure

  @retval Pointer to the relevant CCC structure
**/
MrcStatus
GetDdr5DFETableIndex (
  IN MrcParameters *const     MrcData,
  IN const UINT32             Controller,
  IN const UINT32             Channel,
  IN const UINT32             Dimm,
  IN OUT   TDFEValueDdr5      *TDFEValueDdr5Table
  );

/**
  This function converts from Ohms to DDR5 MR5 PU/PD Drive Strength encoding.

  @param[in]  DrvStrValue  - Ron Value in Ohms.

  @retval INT8 - Encoding if valid Ron value.  Else, -1.
**/
INT8
Ddr5DriveStrengthEncode (
  IN  UINT16  DrvStrValue
  );
#endif // MRC_DDR_5_PRIVATE_H_

