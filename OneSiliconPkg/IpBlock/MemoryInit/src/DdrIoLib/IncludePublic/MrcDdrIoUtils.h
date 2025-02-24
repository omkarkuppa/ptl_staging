/** @file
  This file contains code related to initializing and configuring the DDRIO.

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
**/

#ifndef _MrcDdrIoUtils_h_
#define _MrcDdrIoUtils_h_

#include "CMrcInterface.h"  // for MrcParameters
#include "MrcDdrIoDefines.h"

extern const INT8 DdrioChDelta[MAX_SYS_CHANNEL];
extern const INT8 DdrioTxFifoChOffset[MAX_SYS_CHANNEL];
extern const INT8 DdrioRxFifoChOffset[MAX_SYS_CHANNEL];
extern const INT8 DdrioChDeltaCccIL[MAX_SYS_CHANNEL];
extern const UINT8 DDRCCC_ADJ_MAX_SREP_CNT_NIL[MAX_SYS_CHANNEL];
extern const UINT8 DDRCCC_ADJ_MAX_SREP_CNT_IL[MAX_SYS_CHANNEL];
extern const UINT8 DDRCCC_MIN_SREP_CNT[MAX_SYS_CHANNEL];
extern const INT8  DDRCCC_tCL4RXDQFIFORDEN_CH_SREP_IL[MAX_SYS_CHANNEL];
extern const INT8  DDRCCC_tCL4RXDQFIFORDEN_CH_SREP_NIL[MAX_SYS_CHANNEL];

/// Defines
#define FREQ_TO_TCK_PS   (2000000)

#define VCCCLK_NOMINAL 725

/// Structure to store Min/Max Rx Path Values Across Ranks
typedef struct {
  UINT32 MaxRxDqsN[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 MinRxDqsN[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 MaxRxDqsP[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 MinRxDqsP[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 MinRxDqsBit[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 MaxRxDqsBit[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 MinRxDqsPComp[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 MinRxDqsNComp[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 MinRcvEnPi[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 MaxRcvEnPi[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 DeltaPiCode[MAX_CONTROLLER][MAX_CHANNEL];
} RxPathMinMax;

/// Structure to store Min/Max Tx Path Values Across Ranks
typedef struct {
  UINT32 MaxTxDqs[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 MinTxDqs[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 MaxTxDq[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 MinTxDq[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 MaxTxDqBit[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 MinTxDqBit[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 DeltaPiCode[MAX_CONTROLLER][MAX_CHANNEL];
} TxPathMinMax;

/// Structure to store SenseAmp, DqOdt, DqsOdt Delay and Duration
typedef struct {
  INT64 SaDelay;
  INT64 SaDuration;
  INT64 DqDelay;
  INT64 DqDuration;
  INT64 DqsDelay;
  INT64 DqsDuration;
} SaOdtTiming;

/**
  Find initial Cben and BwSel values based on the current Frequency and CCC Half Frequency mode

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[out] BwSel - The BW code based on gear and frequency
  @param[out] Cben  - The CB code based on gear and frequency
*/
VOID
MrcBwSelCbenFreqSet (
  IN  MrcParameters *const MrcData,
  OUT UINT32               *BwSel,
  OUT UINT32               *Cben
  );

/**
  This function calculates Guardband for drift in Rd DQS timing over time

  @param[in]  MrcData - Pointer to MRC global data.

  @retval UINT32 GB - Guardband in Pico Seconds
**/
UINT32
MrcGetGBDrift (
  IN     MrcParameters *const MrcData
  );

/**
  This function configures the Data Invert Nibble feature in the Phy based on the Phy ODT configuration

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus - mrcFail if the ODT mode doesn't have a mapping for Data Invert Nibble.
  @retval MrcStatus - mrcSuccess otherwise.
**/
MrcStatus
MrcDataInvertNibble (
  IN  MrcParameters *const MrcData
  );

/**
  This function sets DDRIO IP Version, Derivative, Segment, Stepping based on DDRPHY_MISC_SAUG_CR_IP_VERSION_REG.

  @param[in]  MrcData     - Include all MRC global data.

  @retval Nothing
**/
MrcStatus
MrcSetupDdrIoIpInfo (
  OUT MrcParameters *const MrcData
  );

/**
  This function returns channel bit mask of the channels that hardware exists even though it may not be populated
  @param[in] MrcData - Pointer to MRC global data.

  @returns channel bit mask that exists in hardware.
**/
UINT8
MrcGetValidHwChBitMask (
  IN MrcParameters *const MrcData
  );

/**
  Get the max CMD Groups per channel associated with the current memory technology

  @param[in] MrcData  - Pointer to global MRC data.

  @return The maximum number of CMD Groups per channel for the current memory technology
**/
UINT8
MrcGetCmdGroupMax (
  MrcParameters *const MrcData
  );

/**
  This function gets the max partition number for DataShared partitions.

  @param[in]  MrcData   - Pointer to global data.

  @retval Correct number of data shared partitions based on ECC and Memory technology.
**/
UINT8
GetDataSharedPartitionMax (
  IN  MrcParameters *const  MrcData
  );

/**
  This function caclulates the IoChNotPop mask based on the PHY
  DdrMiscControl0.Channel_Not_Populated register field definition.

  Assumtions:
    MrcData->Outputs.MaxChannels is never zero

  @param[in] MrcData - Include all MRC global data.

  @return The Channel_Not_Populated bit mask based on the current
          platform memory population.
**/
UINT8
MrcCalcIoChNotPopMask (
  IN MrcParameters* const MrcData
  );


/**
  This function programs the WorkPoint CR, including enforcing any fuse limits to avoid HW from NACKing PLLLock Requests.

  @param[in, out] MrcData - MRC global data.

  @retval MrcStatus - mrcSuccess if Workpoint CR is set correctly, otherwise mrcFail.
**/
MrcStatus
MrcSetWorkpointCR (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function programs PHClkRatio, WP2LCPLL and LCPLLWPSel during LP5 frequency switch operation.

  @param[in, out] MrcData - MRC global data.
  @param[in]      NewFreq - The new frequency to lock MC PLL.
**/
VOID
MrcProgramLCPLLKeysForFreqSwitch (
  IN OUT MrcParameters *const MrcData,
  IN  MrcFrequency            NewFreq
  );

/**
  This function sets workpoint data including Qclk/PHClk ratio, Gear, and AuxClk ratio for PHY.

  @param[in, out] MrcData - MRC global data.
**/
VOID
MrcSetWorkPointDataForPhy (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function calculates the AuxClkRatio based on QClkRatio/PHClkRatio.

  @param[in, out] MrcData - All the MRC global data.
  @param[in]      Ratio33 - QClkRatio/PHClkRatio

  @return Returns AuxClk ratio.
**/
UINT32
GetAuxClkRatio (
  IN MrcParameters *const MrcData,
  IN UINT32               Ratio33
  );

/**
  This function sets XSyncDelay

  @param[in] MrcData  - Include all MRC global data.
**/
VOID
MrcSetXSyncDelay (
  IN MrcParameters *const MrcData
  );

/**
  This function calculates QClkRatio/PHClkRatio.

  @param[in] MrcData  - Include all MRC global data.

  @return Returns QClkRatio/PHClkRatio.
**/
UINT32
MrcCalcRatio33 (
  IN MrcParameters* const MrcData
  );

/**
  This function updates CBMix and CBMux based on the PHClk ratio locked

  @param[in, out] MrcData - MRC global data.
**/
VOID
CbMixMuxConfig (
  IN OUT MrcParameters *const MrcData
  );

/**
  This routine computes the read/write fifo pointer delays

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  tCWL              - Write Latency for current channel
  @param[in]  Controller        - Current Controller
  @param[in]  Channel           - Current Channel
  @param[out] tCWL4TxDqFifoWrEn - Pointer to the write TX DQ fifo delay
  @param[out] tCWL4TxDqFifoRdEn - Pointer to the read TX DQ fifo delay

  @retval N/A.
**/
VOID
MrcGetTxDqFifoDelay (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8         tCWL,
  IN     UINT8         Controller,
  IN     UINT8         Channel,
  OUT    UINT32        *tCWL4TxDqFifoWrEn,
  OUT    UINT32        *tCWL4TxDqFifoRdEn
  );

/**
  This routine computes DDRPHY delay parameters for Read command

  @param[in]  MrcData           - Pointer to MRC global data
  @param[in]  tCL               - Read Latency for current channel
  @param[in]  Index             - Current Index
  @param[out] tCL4RcvEn         - Pointer to tCL delay for RcvEn
  @param[out] tCL4RxDqFifoRdEn  - Pointer to tCL delay for RX DQ FIFO read enable

  @retval N/A.
**/
VOID
MrcGetRxDqFifoDelay(
    IN OUT MrcParameters* const MrcData,
    IN     UINT8          tCL,
    IN     UINT32         Index,
    OUT    UINT32*        tCL4RcvEn,
    OUT    INT32*         tCL4RxDqFifoRdEn
);

/**
  This function Sets InitComplete Override before sending PM message and Restores InitComplete Override after PM message is sent.

  @param[in] MrcData              - All the MRC global data.
  @param[in] LpJedecFreqSwitch    - Boolean indicator that the InitComplete Override is for the LPDDR frequency switch flow.
  @param[in] IsOverrideMode       - Boolean to indicate Set or Restore mode.
  @param[in] InitCompleteValSave  - Pointer to save / restore InitComplete Override Value

  @retval Returns mrcSuccess if InitComple Override is successful, returns mrcFail if InitComplete Override pointer is NULL
**/
MrcStatus
InitCompleteOverride (
  IN MrcParameters *const MrcData,
  IN BOOLEAN       IsLpJedecFreqSwitch,
  IN BOOLEAN       IsOverrideMode,
  OUT INT64        *InitCompleteValSave
  );

/**
  This function caluculates the Min and Max TxFifo Limits

  @param[in, out]    MrcData  - Include all MRC global data.
  @param[out]        MinLimit - Min Fifo Limit
  @param[out]        MaxLimit - Max Fifo Limit
**/
VOID
TxFifoLimitstCK (
  IN OUT MrcParameters *const MrcData,
  OUT    UINT32        *const MinLimit,
  OUT    UINT32        *const MaxLimit
  );

/**
  This function will set the DDRSCRAM_CR_DDRLASTCR_REG and poll for InitComplete bits to be high.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus  - mrcDeviceBusy if poll InitComplete hits timeout.
                     - mrcSuccess otherwise.
**/
MrcStatus
MrcSetLastCR (
  IN MrcParameters *const MrcData
  );

/**
  This function will set the DDRSCRAM_CR_DDRLASTCR_REG and poll for InitComplete bits to be high.

  The periodic Write retraining Override (DDRSCRAM_CR_DDRMISCCONTROL0.WrRetrainOvrd) field is set
  to 1 before the DDRLASTCR_REG bit is set and is restored after the InitComplete bit is polled high.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus  - mrcDeviceBusy if poll InitComplete hits timeout.
                     - mrcSuccess otherwise.
**/
MrcStatus
MrcSetLastCrWithWrRetrainOverride (
  IN MrcParameters *const MrcData
  );

/**
  This function find the Min/Max Rx Path delays within a channel for all populated channels
  The parameters used are RxDqsNDelay, RxDqsPDelay, BitDelayRxDqs, RxDqsPComp, and RecEnDelay

  @param[in] MrcData - Include all MRC global data.
  @param[out] TxPath - Returns Min/Max delays for each channel
**/
VOID
GetRxPathMinMax (
  IN OUT MrcParameters *const MrcData,
  OUT    RxPathMinMax  *const RxPath
  );

/**
  This function find the Min/Max Tx Path delays within a channel for all populated channels
  The parameters used are TxDqsDelay, TxDqDelay, and TxDqBitDelay

  @param[in] MrcData - Include all MRC global data.
  @param[out] TxPath - Returns Min/Max delays for each channel
**/
VOID
GetTxPathMinMax (
  IN OUT MrcParameters *const MrcData,
  OUT    TxPathMinMax  *const TxPath
  );

/**
  This function calculates the DDR5 timings tDQSS which takes into account JEDEC Spec and SOC requirments

  @param[in]  MrcData      - Include all MRC global data.
  @param[out] tDQSSMax     - Max Voltage/Temperature drift window of first rising DQS pre-amble edge relative to CWL CK edge
  @param[out] tDQSSMin     - Min Voltage/Temperature drift window of first rising DQS pre-amble edge relative to CWL CK edge
  @param[in] WrPreambleLow - Write Preamble low time
**/
VOID
GetDdr5tDQSS (
  IN   MrcParameters *const MrcData,
  OUT  INT32                *tDQSSMax,
  OUT  INT32                *tDQSSMin,
  IN   UINT8                WrPreambleLow
  );

/**
  This function returns the current RxDqsDelay for N and P on CompDqsDelay.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Current Controller
  @param[in] Channel    - Current Channel
  @param[in] Byte       - Current Byte
  @param[out] DqsPDelay - Current RxDqsP Comp Delay
  @param[out] DqsNDelay - Current RxDqsP Comp Delay
**/
VOID
MrcCalcCurrentCompDelayRx (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8                Controller,
  IN     UINT8                Channel,
  IN     UINT8                Byte,
  OUT    INT32         *const DqsPDelay,
  OUT    INT32         *const DqsNDelay
  );

/**
  This function returns the delay limit of RxDqsPerBitDeskew and RxDQPerBitDeskewOffset for a specific Byte.
  Based on the input param "Sign", the composite MIN or MAX margin will be found for all bits within a byte

  @param[in] MrcData              - Pointer to MRC global data.
  @param[in] Controller           - Current Controller
  @param[in] Channel              - Current Channel
  @param[in] Rank                 - Current Rank
  @param[in] Byte                 - Current Byte
  @param[out] BitDelayRxDqs       - MIN or MAX RxDqPerBitDeskew across all bits
  @param[out] BitOffsetRxDqs      - MIN or MAX RxDQPerBitDeskewOffset across all bits
  @param[in] Sign                 - Sign of the delay (0 - negative/min, 1 - positive/max).

**/
VOID
MrcCalcCurrentRxBitDelay (
  IN OUT   MrcParameters *const MrcData,
  IN const UINT8                Controller,
  IN const UINT8                Channel,
  IN const UINT8                Rank,
  IN const UINT8                Byte,
  OUT      INT32         *const BitDelayRxDqs,
  OUT      INT32         *const BitOffsetRxDqs,
  IN const UINT8                Sign
  );

/**
  Programming of CCC_CR_DDRCRCCCCLKCONTROLS_BlockTrainRst

  @param[in] MrcData - The MRC global data.
  @param[in] BlockTrainReset - TRUE to BlockTrainReset for most training algos.  FALSE for specific training algos that need PiDivider sync.

**/
VOID
MrcBlockTrainResetToggle (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              BlockTrainReset
  );

/**
  This function performs an ADC readout for the VCCCLK and PBIas voltages before Read MPR training.
  The readout applies across Data, CCC, and Comp partitions

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess
**/
MrcStatus
MrcVoltageReadout (
  IN MrcParameters *const MrcData
  );

/**
  This function programs the current MRC post code to PHYPMSTATUS1 register field.

  @param[in]  MrcData  - Include all MRC global data.
  @param[in]  PostCode - MRC post code to be programmed

**/
VOID
MrcProgramPostCode (
  IN MrcParameters* const MrcData,
  IN INT64          PostCode
  );

/**
  This function calculates CCC Duty Cycle Adjustor (DCA) bandwidth (BW).

  @param[in, out] MrcData - MRC global data.

  @return Returns CCC DCA BW value.
**/
UINT8
GetCCCDCABW (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function calculates Duty Cycle Adjustor (DCA) bandwidth (BW).
  According to CR MAS, DCA BW and DLL DCA BW point to the same value.

  @param[in, out] MrcData - MRC global data.

  @return Returns DCA BW value.
**/
UINT8
GetDCABW (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function updates the DLL VCDL DCC Code

  @param[in, out] MrcData               - Pointer to MRC global data.
  @param[in]      DllVcdlDccCodeEnValue - Dll Vcdl Dcc Code Enable Value.

**/
VOID
DllVcdlDccCodeUpdate (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8         DllVcdlDccCodeEnValue
  );

/**
  This function updates the DATA BUFFER BONUS PER BIT

  @param[in, out] MrcData               - Pointer to MRC global data.
  @param[in]      Set                   - Set or clear bit 3

**/
VOID
DataBufferBonusPerBitUpdate (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN       Set
  );

/**
  This function sets up DDRIO set of registers for CS 1N or 2N modes

  @param[in]  MrcData  - Include all MRC global data.
  @param[in]  IsEnable - Whether it is 2N (MRC_ENABLE) or 1N (MRC_DISABLE).
**/
VOID
DdrIoCs2NEnableOrDisable (
  IN MrcParameters *const MrcData,
  IN BOOLEAN IsEnable
  );

/**
  This function sets the Cs2NReset field for when switching CS NMode or during Jedec Reset w/o PM6/13

  @param[in] MrcData - Include all MRC global data.
**/
VOID
DdrioCs2NReset (
  IN MrcParameters *const MrcData
  );

/**
  Setup Data Dcc Lane En.

  @param[in, out] MrcData       - Include all MRC global data.
  @param[in]      DataDccLaneEn -  The value for GsmDataDccLaneEn.

  @retval none
**/
VOID
MrcDataDccLaneEn (
  IN OUT MrcParameters *const   MrcData,
  IN     INT64                  DataDccLaneEn
  );

/**
  Setup Data Dcc Lane En.

  @param[in, out] MrcData         - Include all MRC global data.
  @param[in]      DccPHClkPhaseEn -  The value for GsmDccPHClkPhaseEn.

  @retval none
**/
VOID
MrcDccPHClkPhaseEn (
  IN OUT MrcParameters *const   MrcData,
  IN     INT64                  DccPHClkPhaseEn
  );

/**
  This function returns CCC Partition configuration (LP5 / DDR5 NIL / DDR5 IL).

  @param[in] MrcData - MRC global data.

  @returns the CCC partition type.
**/
MRC_PARTITION_TYPE
MrcGetCccPartitionConfiguration (
  IN MrcParameters *const MrcData
  );

/**
  Return the channel mask associated with the input Partition type and the input Partition Instance.

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[in]  PartType     - The partition type to look up.
  @param[in]  PartInstance - The partition instance to look up.
  @param[in]  PartChannel  - The partition channel to look up. Only used by the Data
                             Partitions as there are two "bytes" in 1 Data Instance.

  @return channel mask output buffer.
**/
UINT32
MrcGetPartition2ChMask (
  IN     MrcParameters      *MrcData,
  IN     PARTITION_TYPE     PartType,
  IN     UINT32             PartInstance,
  IN     UINT32             PartChannel
  );

/**
  Setup DQ override and enable/disable DQS continuous toggle mode.

  @param[in, out] MrcData         - Include all MRC global data.
  @param[in]      IsEnabled       - If set to TRUE, programs dq pins high and enables DQS continuous toggle mode.
                                    If set to FALSE, disables DQ override and DQS continuous toggle mode.
**/
VOID
MrcConfigureDqPins (
  IN OUT MrcParameters *const   MrcData,
  BOOLEAN                       IsEnabled
 );

/**
  As part of the frequency switching flow, this function generates all the frequency dependent values
  (PI/DCA BW Values, CBMix, CBMux, NBias) for the new frequency.

  @param[in, out] MrcData - MRC global data.
**/
VOID
MrcGenerateFreqDependentValues (
 IN MrcParameters *const MrcData
 );

/**
  This function runs View Pin Calibration

  @param[in, out] MrcData - Include all MRC global data.

  @retval MrcStatus - mrcSuccess if successful or an error status.s
**/
MrcStatus
MrcViewPinCal (
  IN OUT MrcParameters* const MrcData
  );

/**
  Find mapping of Alert0, Alert1 pins to Memory Controllers. They depend on Board Memory Population/Mapping

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[out] Alert0toMcMap - Pointer to the mapping of Alert0 pin to MCs (0x0-disabled; 0x1-MC0; 0x2-MC1; 0x3-MC0,MC1).
  @param[out] Alert1toMcMap - Pointer to the mapping of Alert1 pin to MCs (0x0-disabled; 0x1-MC0; 0x2-MC1; 0x3-MC0,MC1).

  @retval N/A.
**/
VOID
MrcGetAlertToMcMap (
  IN OUT MrcParameters *const MrcData,
  OUT    UINT8         *Alert0toMcMap,
  OUT    UINT8         *Alert1toMcMap
  );

#endif // _MrcDdrIoUtils_h_

