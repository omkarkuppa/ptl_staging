/** @file
  This file contains the definitions common to the MRC API and other APIs.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/
#ifndef _BlueMrcCommonTypes_h_
#define _BlueMrcCommonTypes_h_
#include "CMrcCommonTypes.h" // for definitions common to blue and green

// This is used to define a section attribute for some Green MRC functions.
// It is used in common files as well, hence define as empty for Blue MRC.
#define MRC_IRAM0_FUNCTION
#define MRC_IRAM1_FUNCTION

///
/// GetSet Group Types.
///
typedef enum {
  RecEnDelay,                   ///< Linear delay (PI ticks), where the positive increment moves the RCVEN sampling window later in time relative to the RX DQS strobes.
  RxDqsDelay,                   ///< Linear delay (PI ticks), where the positive increment moves the RX DQS strobe later in time relative to the RX DQ signal (i.e. toward the hold side of the eye).
  RxDqDelay,                    ///< Linear delay (PI ticks), where the positive increment moves the RX DQ byte/nibble/bitlane later in time relative to the RX DQS signal (i.e.closing the gap between DQ and DQS in the setup side of the eye).
  RxDqsPDelay,                  ///< Linear delay (PI ticks), where the positive increment moves the RX DQS strobe for "even" chunks later in time relative to the RX DQ signal. Even chunks are 0, 2, 4, 6 within the 0 to 7 chunks of an 8 burst length cacheline, for example.
  RxDqsNDelay,                  ///< Linear delay (PI ticks), where the positive increment moves the RX DQS strobe for "odd" chunks later in time relative to the RX DQ signal. Odd chunks are 1, 3, 5, 7 within the 0 to 7 chunks of an 8 burst length cacheline, for example.
  RxVref,                       ///< Linear increment (Vref ticks), where the positive increment moves the strobe RX Vref to a higher voltage.
  RxVrefDQS,                    ///< Read Vref Code for DQS: unsigned Integer with range 0-512. Does not affect DQ lanes
  RxDqVref,                     ///< Linear increment (Vref ticks), where the positive increment moves the bitlane RX Vref to a higher voltage.
  RxDbiVref,                    ///< Linear increment (Vref ticks), where the positive increment moves the DBI bitlane RX Vref to a higher voltage.
  RxDqVrefByte,                 ///< Linear increment (Vref ticks), where the positive increment moves the byte RX Vref to a higher voltage.
  RxDqVrefBit,                  ///< Linear increment (Vref ticks), where the positive increment moves the bitlane RX Vref to a higher voltage. It's an offset.
  RxEq,                         ///< RX CTLE setting indicating a set of possible resistances, capacitance, current steering, etc. values, which may be a different set of values per product. The setting combinations are indexed by integer values.
  RxDqBitDelay,                 ///< Linear delay (PI ticks), where the positive increment moves the RX DQ bitlane later in time relative to the RX DQS signal (i.e.closing the gap between DQ and DQS in the setup side of the eye).
  RxVocRise,                    ///< Monotonic increment (Sense Amp setting), where the positive increment moves the byte/nibble/bitlane's effective switching point to a lower Vref value.
  RxVocRiseDbi,
  RxOdt,                        ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  RxOdtUp,                      ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  RxOdtDn,                      ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  DramDrvStr,                   ///< Drive strength setting resistance setting within a set of possible resistances (or currents), which may be a different set of values per product. Indexed by integer values.
  McOdtDelay,                   ///< Dq Odt Control
  McOdtDuration,                ///< Dq Odt Control
  SenseAmpDelay,                ///< This may be used to indicate CmdToDiffAmpEn for SoC's.
  SenseAmpDuration,             ///<
  RoundTripDelay,               ///< This may be used to indicate CmdToRdDataValid for SoC's.
  RxDqsBitDelay,                ///< Linear delay (PI ticks), where the positive increment moves the RX DQS within the bitlane later in time relative to the RX DQ signal (i.e.closing the gap between DQ and DQS in the hold side of the eye).
  RxDbiDelay,                   ///< Linear delay (PI ticks), where the positive increment moves the RX DBI within the bitlane later in time relative to the RX DQ signal
  RxDqDqsDelay,                 ///< Linear delay (PI ticks), where the positive increment moves the RX DQS per strobe later in time relative to the RX DQ signal (i.e. closing the gap between DQS and DQ in the hold side of the eye. The difference between this parameter and RxDqsDelay is that both the DQ and DQS timings may be moved in order to increase the total range of DQDQS timings.
  WrLvlDelay,                   ///< Linear delay (PI ticks), where the positive increment moves both the TX DQS and TX DQ signals later in time relative to all other bus signals.
  TxDqsDelay,                   ///< Linear delay (PI ticks), where the positive increment moves the TX DQS strobe later in time relative to all other bus signals.
  TxDqDelay,                    ///< Linear delay (PI ticks), where the positive increment moves the TX DQ byte/nibble/bitlane later in time relative to all other bus signals.
  CtlDrvVrefUp,
  CtlDrvVrefDn,
  TxVref,                       ///< Linear increment (Vref ticks), where the positive increment moves the byte/nibble/bitlane TX Vref to a higher voltage. (Assuming this will abstract away from the range specifics for DDR4, for example.)
  TxDqBitDelay,                 ///< Linear delay (PI ticks), where the positive increment moves the TX DQ bitlane later in time relative to all other bus signals.
  TxDbiDelay,                   ///< Linear delay (PI ticks), where the positive increment moves the TX DBI bitlane later in time relative to all other bus signals.
  TxRon,                        ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  TxRonUp,                      ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  TxRonDn,                      ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  TxImode,                      ///< TX I-Mode Boost setting indicating a set of possible current boost levels, which may be a different set of values per product. The setting combinations are indexed by integer values.
  WrOdt,                        ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  NomOdt,                       ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  ParkOdt,                      ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  TxDqTco,                      ///< TCO Comp Code for DQ
  TxXtalk,                      ///<
  RxCtleR,                      ///< Rx Equalization Resistance control.
  RxCtleC,                      ///< Rx Equalization Capacitance control.
  RxCtleEn,                     ///< Rx CTLE En control.
  RxDqsPBitDelay,               ///< Linear delay (PI ticks), where the positive increment moves the RX DQS bitlane timing for "even" chunks later in time relative to the RX DQ bitlane signal. Even chunks are 0, 2, 4, 6 within the 0 to 7 chunks of an 8 burst length cacheline, for example.
  RxDqsNBitDelay,               ///< Linear delay (PI ticks), where the positive increment moves the RX DQS bitlane timing for "odd" chunks later in time relative to the RX DQ bitlane signal. Odd chunks are 1, 3, 5, 7 within the 0 to 7 chunks of an 8 burst length cacheline, for example.
  CmdAll,                       ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CMD_ALL category later in time relative to all other signals on the bus.
  CmdGrp0,                      ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CMD_GRP0 category later in time relative to all other signals on the bus.
  CmdGrp1,                      ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CMD_GRP1 category later in time relative to all other signals on the bus.
  CmdGrp2,                      ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CMD_GRP2 category later in time relative to all other signals on the bus.
  CtlAll,                       ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_ALL category later in time relative to all other signals on the bus.
  CtlGrp0,                      ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP0 category later in time relative to all other signals on the bus.
  CtlGrp1,                      ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP1 category later in time relative to all other signals on the bus.
  CtlGrp2,                      ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP2 category later in time relative to all other signals on the bus.
  CtlGrp3,                      ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP3 category later in time relative to all other signals on the bus.
  CtlGrp4,                      ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP4 category later in time relative to all other signals on the bus.
  CtlGrp5,                      ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRP5 category later in time relative to all other signals on the bus.
  CmdCtlAll,                    ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CMD_CTL_ALL category later in time relative to all other signals on the bus.
  CkAll,                        ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CK_ALL category later in time relative to all other signals on the bus.
  AlertVref,                    ///< Linear increment (Vref ticks), where the positive increment moves the ALERT Vref to a higher voltage.
  CmdRon,                       ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  CmdGrpPi,                     ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CMD_GRPx (Specified by Strobe index) category later in time relative to all other signals on the bus.
  CtlGrpPi,                     ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CTL_GRPx (Specified by Strobe index) category later in time relative to all other signals on the bus.
  ClkGrpPi,                     ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the CLK_GRPx (Specified by Strobe index) category later in time relative to all other signals on the bus.
  TxCycleDelay,                 ///< Clock Cycle delay, where the positive increment increases the delay
  EridDelay,                    ///< Linear delay (PI ticks), where the positive increment moves the ERID signals later in time relative to the internal sampling clock (i.e.closing the gap between ERID and internal sampling clock in the setup side of the eye). This group is applicable for DDRT DIMMs.
  EridVref,                     ///< Linear increment (Vref ticks), where the positive increment moves the ERID Vref to a higher voltage. This group is applicable for DDRT DIMMs.
  ErrorVref,                    ///< Linear increment (Vref ticks), where the positive increment moves the ERROR Vref to a higher voltage. This group is applicable for DDRT DIMMs.
  ReqVref,                      ///< Linear increment (Vref ticks), where the positive increment moves the REQ Vref to a higher voltage. This group is applicable for DDRT DIMMs.
  RecEnOffset,                  ///< Linear delay (PI ticks), where the positive increment moves the RCVEN sampling window later in time relative to the RX DQS strobes.
  RxDqsOffset,                  ///< Linear delay (PI ticks), where the positive increment moves the RX DQS strobe later in time relative to the RX DQ signal (i.e. toward the hold side of the eye).
  RxVrefOffset,                 ///< Linear increment (Vref ticks), where the positive increment moves the byte/nibble/bitlane RX Vref to a higher voltage.
  TxDqsOffset,                  ///< Linear delay (PI ticks), where the positive increment moves the TX DQS strobe later in time relative to all other bus signals.
  TxDqOffset,                   ///< Linear delay (PI ticks), where the positive increment moves the TX DQ byte/nibble/bitlane later in time relative to all other bus signals.
  RxDqsBitOffset,               ///< Linear delay (PI ticks), read timing offset between Strobe P/N per lane t for bit[x] of the DQ byte
  DqsTxEq,                      ///< Unsigned number where positive numbers delay the output time if bus toggled in the previous UI
  DqTxEq,                       ///< 2s complement number where positive numbers delay the output time if bus toggled in the previous UI
  WckGrpPi,                     ///< Linear delay (PI ticks), where the positive increment moves all signals assigned to the WCK_GRPx (Specified by Strobe index) category later in time relative to all other signals on the bus.
  DqsOdtDelay,                  ///< Dqs ODT Control
  DqsOdtDuration,               ///< Dqs ODT Control
  RxBiasCtl,                    ///< Control Bias current DAC which determines 1st and 2nd stage amp current.  Iref = 14% * BiasIrefAdj[3:0] where default is (0xA, 0x7) for NPath & PPath.
  RxDqsAmpOffset,               ///< RX DQS Amplifier Offset Cancellation (Voltage)
  TxRankMuxDelay,               ///< Tx rank mux delay for DQ/DQS to account for the I/O setting latching time vs. the drive enable
  TxRankMuxDelayDq,             ///< Tx rank mux delay for DQ to account for the I/O setting latching time vs. the drive enable
  TxRankMuxDelayDqs,            ///< Tx rank mux delay for DQS to account for the I/O setting latching time vs. the drive enable
  TxR2RDqEq,                    ///< Offset added to TxRankMuxDelay for DqEqCode
  TxR2RDqsEq,                   ///< Offset added to TxRankMuxDelay for (Dqs{PrNf,PfNr}TcoDelay, DqsEqCode)
  TxR2RDqPi,                    ///< Offset added to TxRankMuxDelay for TxDqPiCode
  TxR2RDqsPi,                   ///< Offset added to TxRankMuxDelay for TxDqsPiCode
  RxR2RDqsOff,                  ///< Offset added to RxRankMuxDelay for DQS Offset
  RxR2RDqOff,                   ///< Offset added to RxRankMuxDelay for DQ Offset & DFE
  RxR2RRxPi,                    ///< Offset added to RxRankMuxDelay for RxDqs{P,N}PICode
  RxR2RRcvEn,                   ///< Offset added to RxRankMuxDelay for RcvEnPICode
  RxRankMuxDelay,               ///< Rx rank mux delay to account for the I/O setting latching time vs. the receive enable
  RxIoTclDelay,                 ///< Per Channel value that defines the additive delay on the Receive Enable signal related to the tCL of the DRAM.  (Number of Clocks per step vary by design) ?CH_SUB
  RoundTripIoComp,              ///< Per Channel offset between Receive Enable is required and when data is ready to go from DDRIO to MC.
  RxFifoRdEnFlybyDelay,         ///< Per Rank, per Channel offset between Read FIFO read enable is required and when data is ready to go from DDRIO to MC. ?CH_SUB
  RxFifoRdEnTclDelay,           ///< Per Channel offset between Read FIFO read enable is required and when data is ready to go from DDRIO to MC. ?CH_SUB
  TxDqFifoWrEnTcwlDelay,        ///< ?CH_SUB
  TxDqFifoRdEnTcwlDelay,        ///< ?CH_SUB
  TxDqsTcoPFallNRise,           ///< Provides an unsigned delay control to DqsP-Fall / DqsN-Rise TcoDelay.  Lower values slows down rise delay and higher values slow down fall delay.
  TxDqsTcoPRiseNFall,           ///< Provides an unsigned delay control to DqsP-Rise / DqsN-Fall TcoDelay.  Lower values slows down rise delay and higher values slow down fall delay.
  TxDqsTcoCode,
  DefDrvEnLow,                  ///< Defined TX behavior when DrvEn is low. {0: Force On, 1: HiZ, 2: Return to Termination (See CR_RTO), 3: Non-Return to Zero}
  CmdTxEq,                      ///< CA Equalization codes: {4} Controls ConstantZ (1) vs. NonConstantZ (0), {3:0}={0: 0 Static Legs / 12 Eq Legs, ... 12: 12 Static Legs / 0 Eq Legs, 13-15: rsvd}
  CtlTxEq,                      ///< CTL Equalization codes: {4} Controls ConstantZ (1) vs. NonConstantZ (0), {3:0}={0: 0 Static Legs / 12 Eq Legs, ... 12: 12 Static Legs / 0 Eq Legs, 13-15: rsvd}
  GsmIntCkOn,                   ///< Force PI enables in CCC
  GsmIocDllWeakLock,
  GsmIocTxDllWeakLock,
  RxVrefVddqDecap,              ///< Control Rx Vref coupling to Vddq.  0: No cap to Vddq  1-7 is cap control with 12.5% step size. Remaining cap will be to VSS
  RxVocFall,
  WrRetrainDeltaPiCode,
  RdRetrainDeltaPiCode,
  RxCompDqsDelayP,
  RxCompDqsDelayN,
  RxCompDqsOffset,
  CompRcompOdtUp,               ///< The resistive value of the pull-up block of on die termination
  CompRcompOdtDn,               ///< The resistive value of the pull-dn block of on die termination
  WrDSCodeUpCmd,                ///< RcompUp Code for Write Drive Strength-Cmd
  WrDSCodeUpCtl,                ///< RcompUp Code for Write Drive Strength-Ctl
  WrDSCodeUpClk,                ///< RcompUp Code for Write Drive Strength-Clk
  WrDSCodeDnCmd,                ///< RcompDown Code for Write Drive Strength-Cmd
  WrDSCodeDnCtl,                ///< RcompDown Code for Write Drive Strength-Ctl
  WrDSCodeDnClk,                ///< RcompDown Code for Write Drive Strength-Clk
                                ///< -----------------------------------------------------------------
  EndOfPhyMarker,               ///< Marker for end of phy groups.
                                ///< -----------------------------------------------------------------
  GsmIocIoReset,                ///< Used to reset the DDR IO.
  GsmIocDccClkGateDisable,
  GsmIocNoDqInterleave,
  GsmIocScramDdr5Mode,          ///< Set the DDRIO DDRSCRAM to DDR5 Mode
  GsmIocScramGearMode,          ///< Program gear in DDRSCRAM: 1 = Gear4, 0 = Gear2
  GsmTrainWCkEn,
  GsmTrainWCkPulse,             ///< Program TrainWCkPulse
  GsmTrainWCkBL,
  GsmtWckHalfRate,
  GsmTrainWCkMask,
  GsmWCkDiffLowInIdle,
  GsmIocCccPiEn,
  GsmIocCccPiEnOverride,
  GsmIocDisClkGate,
  GsmIocDisDataIdlClkGate,      ///< Data Idle Clock Gating Disable switch.
  GsmIocScramLp5Mode,           ///< Set the DDRIO DDRSCRAM to LPDDR5 Mode
  GsmIocLp5Wck2CkRatio,
  GsmIocChNotPop,               ///< Channel Not Populated Mask.
  GsmIocDfiCmdDecoderEn,
  GsmIocDisIosfSbClkGate,
  GsmIocEccEn,                  ///< ECC Enable/Disable for the DDRIO ?
  GsmIocWrite0En,
  GsmIocScramEn,
  GsmIocWrRetrainOvrd,          ///< Override WriteRetrain check for initcomplete_write. This should be set when write retrain is not supported or not enabled.
  GsmIocRdRetrainOvrd,          ///< Override ReadRetrain check for initcomplete_read. This should be set when read retrain is not supported or not enabled.
  GsmIocDdr52NMode,
  GsmDdrReset,
  GsmIocDdr5CkdMode,                ///< Forces CLK to Toggle by forcing into DDR5 CKD Mode
  GsmIocDisableTxDqs,
  GsmIocForceRxAmpOn,
  GsmIocForceRxOnDqs,
  GsmIocForceRxOnDqsMux,
  GsmIocForceRxOnDq,
  GsmIocForceOdtOn,
  GsmIocTxPiPwrDnDis,
  GsmIocInternalClocksOn,           ///< Force on all the PI enables
  GsmIocDqsMaskPulseCnt,            ///< Controls the number of masked DQS pulses sent by the DDR IO during CMD VREF update in LP4 CBT
  GsmIocDqsPulseCnt,                ///< Controls the number of unmasked DQS pulses sent by the DDR IO during CMD VREF update in LP4 CBT
  GsmIocDqOverrideData,             ///< DQ value override to latch CMD VREF to the DRAM during LP4 CBT. This field is used to drive DQ7 High during LP5 ECT.
  GsmIocDqOverrideEn,               ///< Enables the DDR IO to force static DQ value to latch CMD VREF to the DRAM during LP4 CBT. This field is used to drive DQ7 High during LP5 ECT.
  GsmIocDqsOvrdToggle,
  GsmIocRankOverrideEn,             ///< Configures the DDRIO to override the delay values used for the target rank.
  GsmIocRankOverrideVal,            ///< Specified which rank to pull the delay values from.
  GsmIocDataDisableTxDqs,
  GsmIocDataDqsOdtParkMode,
  GsmIocDataInvertNibble,
  GsmIocEnableLpMode4,
  GsmIocLpCtrlEn,
  GsmIocWrEnViewDrv,
  GsmDataDccRankEn,
  GsmDataDccLaneEn,
  GsmClkDccRankEn,
  GsmWckDccRankEn,
  GsmWckDccLaneEn,
  GsmCompVccDdqTarget,
  GsmInitCompleteOvrd,
  GsmInitCompleteOvrdVal,
  GsmIocVccClkFFCRWait,
  GsmPChannelEn,
  GsmSkipRestoreCR,
  GsmSkipRetentionCR,
  GsmSaxgEnable,
  GsmSaxgReady,
  GsmSaxgPwrGood,
  GsmPostCode,
  GsmWorkPointAuxClk,
  GsmWorkPointRatio,
  GsmWorkPointGear4,
  GsmWorkPointLCPLLWPSel,
  GsmWorkPointPhClkRefClk,
  GsmWorkPointWP2LCPLL,
  GsmTrimClkOvrd,
  GsmDccPHClkPhaseEn,
  GsmDccPHClkDutyCycleEn,
  GsmCompCtrlPeriodicComp,
  GsmCompCtrlDVFSComp,
  GsmCompCtrlCompDist,
  GsmCompCtrlCompUpdate,
  GsmIocRetrainSwizzleCtlRetrainEn,
  GsmIocRetrainSwizzleCtlSerialMrr,
  GsmIocRetrainInitPiCode,
  GsmIocRetrainRoCount,
  GsmIocRetrainCtlInitTrain,
  GsmIocRetrainCtlDuration,
  GsmIocRetrainCtlResetStatus,
  GsmIocAuxClkOffAuxClk,
  GsmIocQClkCalOff,
  GsmIocPHClkOffsetFastCal,
  GsmIocAuxOffsetFastCal,
  GsmMiscPmMessage,
  GsmMiscPmMessageRunBusy,
  GsmMiscPmMessageWpSel,
  GsmIocCs2NReset,
  GsmIocCsGearDownEnable,
  GsmIocCsGearDownForce,
  GsmIocCsGearDownSrDramTrack,
  GsmIocITerdcdselovrval,
  GsmIocITerdcdselovren,
  GsmDccDllDcdTargetSel,
  GsmLvrCurrentSensorCntStart,
  GsmLvrCurrentSensorAmpHyster,
  GsmLvrCurrentSensorEnChop,
  GsmLvrCurrentSensorEnSensor,
  GsmLvrCurrentSensorDuration,
  GsmLvrCurrentSensorSelLVR,
  GsmCompLvrCurrentSensorCntStart,
  GsmCompLvrCurrentSensorAmpHyster,
  GsmCompLvrCurrentSensorEnChop,
  GsmCompLvrCurrentSensorEnSensor,
  GsmCompLvrCurrentSensorDuration,
  GsmCompLvrCurrentSensorSelLVR,
  GsmCccVccClkFFCurrentSensorCntStart,
  GsmCccVccClkFFCurrentSensorDuration,
  GsmDataVccClkFFCurrentSensorCntStart,
  GsmDataVccClkFFCurrentSensorDuration,
  GsmCccVccClkFFCurrentSensorSelCnt,
  GsmDataVccClkFFCurrentSensorSelCnt,
  GsmLvrCurrentSensorEnCnt,
  GsmCompLvrCurrentSensorEnCnt,
  GsmCccVccClkFFCurrentSensorEnCnt,
  GsmDataVccClkFFCurrentSensorEnCnt,
  GsmFFCodePhDrvG,
  GsmLvrCurrentSensorCount,
  GsmDataVccClkFFCurrentSensorCount,
  GsmCccVccClkFFCurrentSensorCount,
  GsmLvrAutoTrimOffsetClkRx0,
  GsmLvrAutoTrimOffsetClkRx1,
  GsmLvrAutoTrimOffsetClkTx,
  GsmLvrAutoTrimOffsetIOG,
  GsmLvrAutoTrimOffsetNbiasFF,
  GsmLvrAutoTrimCccOffsetClkTx,
  GsmLvrAutoTrimCccOffsetIOG,
  GsmLvrAutoTrimOffsetClkQ,
  GsmLvrAutoTrimOffsetDIST,
  GsmLvrAutoTrimOffsetPLL,
  GsmLvrWakeIncClkRx,
  GsmLvrWakeIncClkTx,
  GsmLvrWakeIncIOG,
  GsmLvrCccWakeIncClkTx,
  GsmLvrCccWakeIncIOG,
  GsmLvrCompWakeIncClkQ,
  GsmLvrCompWakeIncIOG,
  GsmLvrCompWakeIncDIST,
  GsmLvrCompWakeIncPLL,
  GsmLvrEnDiv,
  GsmLvrChop,
  GsmLvrUsePSAL,
  GsmLvrVoltageSensorCntStart,
  GsmLvrVoltageSensorDuration,
  GsmLvrAutoTrimDelay,
  GsmLvrEnSensor,
  GsmLvrTarget0,
  GsmLvrTarget1,
  GsmLvrVinDivider,
  GsmLvrSelCount,
  GsmLvrSelLVR,
  RcvEnFfMuxSelByte0,
  GsmLvrVoltageSensorSelLvr,
  GsmLvrAutoTrimSelLvr,
  GsmVccIoVccF0,
  GsmVccIoVccF1,
  GsmVccIoVccF2,
  GsmVccIoOCSlope,
  GsmVccClkVccF0,
  GsmVccClkVccF1,
  GsmVccClkVccF2,
  GsmVccClkOCSlope,
  GsmVccClkRxCtlB0FbDivider,
  GsmVccClkRxCtlB0Target,
  GsmVccClkRxCtlB0BiasAdj,
  GsmVccClkRxCtlB0PMBiasAdj,
  GsmVccClkRxCtlB1FbDivider,
  GsmVccClkRxCtlB1Target,
  GsmVccClkRxCtlB1BiasAdj,
  GsmVccClkRxCtlB1PMBiasAdj,
  GsmVccClkFFBiasFbDivider,
  GsmVccClkFFBiasNbTarget,
  GsmVccPllControlFbDivider,
  GsmVccPllControlTarget,
  GsmVccDistControlFbDivider,
  GsmVccDistControlTarget,
  GsmVccIogControlFbDivider,
  GsmVccIogControlTarget,
  GsmVccIogControlBiasAdj,
  GsmVccIogControlPMBiasAdj,
  GsmVccClkTxControlFbDivider,
  GsmVccClkTxControlTarget,
  GsmVccClkTxControlBiasAdj,
  GsmVccClkTxControlPMBiasAdj,
  GsmVccClkqControlFbDivider,
  GsmVccClkqControlTarget,
  GsmVccClkqControlBiasAdj,
  GsmVccClkqControlPMBiasAdj,
  GsmVccDdqControlFbDivider,
  GsmVccDdqControlTarget,
  GsmVccDdqControlBiasAdj,
  GsmVccDdqControlPMBiasAdj,
  GsmLvrAutoTrimRunBusy,
  GsmLvrAutoTrimStatusVsenseOffset,
  GsmLvrPwrGoodOnVrefChange,
  GsmLvrRstCount,
  GsmLvrVoltageSensorCount0,
  GsmLvrVoltageSensorCount1,
  GsmDataLvrAutoTrimRunBusy,
  GsmCccLvrAutoTrimRunBusy,
  GsmCompLvrAutoTrimRunBusy,
  GsmIocCompForceLvrOn,
  GsmIOGLVREnableOvrd,
  GsmIOGLVRVrefEnOvrd,
  GsmIOGPwrGoodOvrd,
  GsmIOGGVLVROvrdModeEn,
  GsmIOGLVROvrdModeEn,
  GsmDataLvrAutoTrimOffsetClk,
  GsmDataLvrAutoTrimOffsetIOG,
  GsmCccLvrAutoTrimOffsetClk,
  GsmCccLvrAutoTrimOffsetIOG,
  GsmCompLvrAutoTrimOffsetIOG,
  GsmCompLvrAutoTrimOffsetIOGGV,
  GsmCompLvrAutoTrimOffsetDist,
  GsmIogLvrVrefSel,
  GsmIogLvrSelVdd2Ladder,
  GsmClkLvrVrefSel,
  GsmClkLvrRxDllB0VrefSel,
  GsmClkLvrRxDllB1VrefSel,
  GsmClkLvrSelVdd2Ladder,
  GsmIogGvLvrVrefSel,
  GsmIogGvLvrSelVdd2Ladder,
  GsmDistGLvrVrefSel,
  GsmDistGLvrSelVdd2Ladder,
  GsmVsBonusIn,
  GsmVsSelVdd2Ladder,
                                    ///< -----------------------------------------------------------------
  EndOfIocMarker,                   ///< End of IO Config Marker
                                    ///< -----------------------------------------------------------------
  GsmMctRCD,
  GsmMctRCDw,
  GsmMctRDA2ACT,
  GsmMctWRA2ACT,
  GsmMctRP,
  GsmMctRPab,
  GsmMctRAS,
  GsmMctnWRDiff,
  GsmMctRDPRE,
  GsmMctPPD,
  GsmMctWRPRE,
  GsmMctFAW,
  GsmMctRRDsg,
  GsmMctRRDdg,
  GsmMctVrcgEnable,
  GsmMctVrcgDisable,
  GsmMctREFSbRd,
  GsmMctLpDeratingExt,
  GsmMctRDRDsg,
  GsmMctRDRDdg,
  GsmMctRDRDdr,
  GsmMctRDRDdd,
  GsmMctRDWRsg,
  GsmMctRDWRdg,
  GsmMctRDWRdr,
  GsmMctRDWRdd,
  GsmMctWRRDsg,
  GsmMctWRRDdg,
  GsmMctWRRDdr,
  GsmMctWRRDdd,
  GsmMctWRWRsg,
  GsmMctWRWRdg,
  GsmMctWRWRdr,
  GsmMctWRWRdd,
  GsmMctCKE,
  GsmMctXP,
  GsmMctPRPDEN,
  GsmMctRDPDEN,
  GsmMctWRPDEN,
  GsmMctCA2CS,
  GsmMctCSL,
  GsmMctCSH,
  GsmtccdCasDelta,
  GsmMctCL,
  GsmMctCWL,
  GsmMctCWLAdd,
  GsmMctCWLDec,
  GsmMctXSDLL,
  GsmMctXSR,                    ///< Self Refresh exit to commands not requiring a locked DLL (tXS) in DDR3/4(For SKL, this maps to the tXS_offset) or Self Refresh exit to next valid command delay (tXSR) in LPDDR3/4.
  GsmMctLpMode4SrxDelay,
  GsmMctSR,                     ///< tSR: minimum time that we are in self refresh
  GsmMctXSRDs,
  GsmMctMRD,                    ///< Mode Register Set command to next valid command delay.
  GsmMctSrIdleTimer,                 ///< The number of cycles with no transactions in order to enter Self Refresh.
  GsmMctSrefDelayQsync,         ///< Defines the delay to qsync signal used to allign Dclk and Qclk
  GsmMctREFI,                   ///< Delay from Start of Refresh to next ACT or Refresh.
  GsmMctRFC,                    ///< Time of refresh.
  GsmMcDdr5Rir,
  GsmMctRefreshHpWm,            ///< tREFI count level that turns refresh priority to high.
  GsmMctRefreshPanicWm,         ///< tREFI count level for panic priority.
  GsmMctZQCalPeriod,
  GsmMcttZQLatch,
  GsmMctZQCAL,
  GsmMctCPDED,                  ///< Defines the delay required from power down before the command bus can be tri-stated.
  GsmMctCKCKEH,                 ///< Defines the number of cycles CS must be driven low before asserting CKE for power down exit.
  GsmMctRFCpb,                  ///< Refresh cycle time per bank in DCLKS. How long the per bank refresh command takes to complete.
  GsmMctRFM,
  GsmMctDRFM,
  GsmMcttOSCO,
  GsmMcttPREMRR,
  GsmMctWrOsclRuntime,
  GsmMctRdOsclRuntime,
  GsmMcttMRR,
  GsmMcttCSLCK,
  GsmMcttWckStop,
  GSmMcttWckOff,
  GsmMcttCkFspX,
  GsmMctWckEnlFs,
  GsmMctWckEnlRd,
  GsmMctWckEnlWr,
  GsmMctWckPreToggleFs,
  GsmMctWckPreToggleRd,
  GsmMctWckPreToggleWr,
  GsmMctWckDfiStop,
  GsmMctWckDfiOffset,
  GsmMctMrhAfterCommandDelay,
  EndOfMctMarker,               ///< Marks the end of the MC Timing Group.
  GsmMccWCKDiffLowInIdle,       ///< This is a shaow copy of the DDRPHY register and should match the POR value of the DDRPHY register
  GsmMccDramType,               ///< Defines what DRAM technology is being used.
  GsmMccCmdStretch,             ///< Defines the Mode of Command Stretch.
  GsmMccAddrMirror,             ///< This field is used to inform the MC which DIMMs are mirrored.
  GsmMccCmdTriStateDisTrain,    ///< Disable Command bus tri-state during CA bus training
  GsmMccCmdTriStateDis,         ///< Disable Command bus tri-state
  GsmMccFreqPoint,              ///< Defines the current frequency point the MC is in during training.
  GsmMccEnableOdtMatrix,        ///< Enables custom mapping of ODT assertion on Ranks.
  GsmMccGearMode,               ///< Indicates Gear: 0=Gear2 1=Gear4
  GsmMccNoGear4ParamDivide,     ///< Don't do RU[param/4] for DRAM timing parameters,divide only by 2
  GsmMccOneDpc,                 ///< 1dpc performance feature
  GsmMccMultiCycCmd,            ///< Setting this field to 1 allows MC to stretch DDR5 MPC and VREF commands. CS Setup and Hold times are enforced along with CS being asserted(active low) for multiple cycles.
  GsmMccEnhancedInterleave,
  GsmMccHashEnabled,
  GsmMccHashLsb,
  GsmMccHashZone1Start,
  GsmMccEccMode,
  GsmMccExtendedBankHash,
  GsmMccBg0Hash,
  GsmMccAddrDecodeDdrType,
  GsmMccCh0Size,
  GsmMccCh1Size,
  GsmMccCpgcActive,
  GsmMccInOrderIngress,
  GsmMccSubch0SdramWidth,         ///< This field defines the L DIMM device width: x4, x8, x16, x32, x64
  GsmMccSubch0RankCnt,
  GsmMccSubch0Density,
  GsmMccSubch1SdramWidth,         ///< This field defines the S DIMM device width: x4, x8, x16, x32, x64
  GsmMccSubch1RankCnt,
  GsmMccSubch1Density,
  GsmMccFspOp,
  GsmMccFspRdWr,
  GsmMccFspMrsOp,
  GsmMccEnableRefresh,
  GsmMccSrState,
  GsmMccForceSreWithFreqChange,
  GsmMccEnableDclk,
  GsmMccPureSrx,                ///< Tell MC to only exit Self Refresh.  Blocks other maintenance FSMs: ZQ, Temp, DQS Oscillator.
  GsmMccIgnoreCke,
  GsmMccMaskCs,
  GsmMccCpgcInOrder,
  GsmMccBlockXarb,
  GsmMccBlockCke,
  GsmMccRankOccupancy,
  GsmMccMcSrx,
  GsmMccEnRefTypeDisplay,       ///< Displays refresh type on BA address pins.
  GsmMccHashMask,               ///< Bits set for channel XOR function.
  GsmMccLsbMaskBit,             ///< MC channel interleave bit.
  GsmMccPbrDis,                 ///< Boolean control for Per Bank Refresh.  1 = Disable, 0 = Enable.
  GsmMccRefreshAbrRelease,      ///< ABR is activated until tREFI count is below this parameter, when tREFI is above HP water mark
  GsmMccCkeOverride,            ///< Override enable
  GsmMccCkeOn,                  ///< Override value
  GsmMccRrdValidTrigger,        ///< Trigger RRDVALID FSM (Roundtrip Matching) per rank
  GsmMccRrdValidOverflow,       ///< RRDVALID FSM Overflow indication per rank
  GsmMccRrdValidValue,          ///< RRDVALID FSM result - gap value, per rank
  GsmMccRrdValidSign,           ///< RRDVALID FSM result - gap sign, per rank
  GsmMccCsOverride0,            ///< Override enable
  GsmMccCsOverrideVal0,         ///< Override value
  GsmMccCsOverride1,            ///< Override enable
  GsmMccCsOverrideVal1,         ///< Override value
  GsmMccMinRefRate,
  GsmMccMaxRefRate,
  GsmMccMR4Period,
  GsmMccLpddrCurrentFsp,
  GsmMccLpddrCurrentFspTrk,
  GsmMccZqSerialize,
  GsmMccDeswizzleByte,          ///< Data Strobe deswizzle mapping
  GsmMccDeswizzleBit,           ///< Data bits deswizzle mapping
  GsmMccMrrResult,
  GsmMccLp5BankMode,
  GsmMccLp5WckMode,
  GsmMccLp5WckFastMode,
  GsmMccRefreshRankMask,
  GsmDisAllCplInterleave,
  GsmDisVc1CplInterleave,
  GsmMccRhLfsr0,
  GsmMccRhLfsr1,
  GsmMccRhTRRDimmEnable,
  GsmMccRhDrfmEnable,
  GsmMccRhLfsr0Mask,
  GsmMccRhLfsr1Mask,
  GsmMccRhMA1Swizzle,
  GsmMccRhMA2Swizzle,
  GsmMccMicronFSwizzle,
  GsmMccMicronRSwizzle,
  GsmMccBrc,
  GsmMccRhEnableDimm0,
  GsmMccRhEnableDimm1,
  GsmMccRhHighWM,
  GsmMccRhLowWM,
  GsmMccRhNormalRefSub,
  GsmMccRhRefmSub,
  GsmMccRhRefmEn,
  GsmMccAllowRHDebtSR,
  GsmMccOppSrefEnable,
  GsmMccForceSRState,
  GsmMccVC0ReadEntries,
  GsmMccVC1ReadEntries,
  GsmMccSpineGateDis,
  GsmMccMainGlbDrvGateDis,
  GsmMccSchedGlbDrvGateDis,
  GsmMccOdtMatrixRd,
  GsmMccOdtMatrixWr,
  GsmMccCasStopAdditionalGap,
  GsmMccRdWckAsyncGap,
  GsmMccWrWckAsyncGap,
  GsmMccEnSrxWrRetraining,
  GsmMccEnWrRetraining,
  GsmMccEnRdRetraining,
  GsmMccRetrainPeriod,
  GsmMccVc0LatencyGaurdTimerx16,
  GsmMccVc1LatencyGuardTimerx8,
  GsmMccVc1IsocLatencyGuardTimerx8,
  GsmMccLatencySensitiveTimerx16,
  GsmMccSelfRefreshEnable,
  GsmMccSelfRefreshLength,
  GsmMccPowerDownEnable,
  GsmMccIdleEnable,
  GsmMccDis2cByp,
  GsmMccSchedDisClkGateLocal,
  GsmMccDisAsyncOdt,
  GsmMccDisStarvedPriorityOnNewReq,
  GsmMccMainDisClkGate,
  GsmMccMainDisRegClkGate,
  GsmMccDisGlbDrvClkGate,
  GsmMccDisIosfSbClkGate,
  GsmMccPtrSepOspid,
  GsmMccPtrSepIspid,
  GsmMccCmdBgfAlwaysOn,
  GsmMccCkDisTristate,
  GsmMccPageIdleTimerDisable,
  GsmMccDisSchedsClkGate,
  GsmMccDisMntClkGate,
  GsmMccDisableSplitAct,
  GsmMccAllowOppRefBelowWrtThreshold,
  GsmMccWrtThreshold,
  GsmMccPdwnIdleCounterSubch0,
  GsmMccPdwnIdleCounterSubch1,
  GsmMccPdwnLpmodeEnable,
  GsmMccDisCkett,
  GsmMccTTIdleCounter,
  GsmMccAPD,
  GsmMccPPD,
  GsmMccDisWdbClkGate,
  GsmMccLpMode4En,
  GsmMccCmiSourceId0,
  GsmMccCmiSourceId1,
  GsmMccCmiSourceId2,
  GsmMccCmiSourceId3,
  GsmMccDelayPMAckCycles,
  GsmMccEnSrxTempRead,
  GsmMccSrxZqcal,
  GsmMccEcDis,
  GsmMccEccGranularity,
  GsmMccAutoPrechargeEn,
  GsmMccEnCsGearDown,
  GsmMccCfiE2eParityEnRdata,
  GsmMccCfiE2eParityEnRdCmplHeader,
  GsmMccCfiE2eParityEn,
  GsmScPbrEcsRefabEnable,
  GsmScPbrEcsRefabPeriod,
  GsmMccDdr5CkdEnable,
  GsmMccDisLpddr5RdwrInterleaving,
  GsmMccEccCorrectionDisable,
  GsmMccMcMntsSpareRw,
  GsmMccMcMntsRfFastSleepDisable,
  GsmMccMcCbRowPressDisAutoPre,
  GsmMccPageOpenPolicyMaxCount,
  GsmMccPageOpenPolicyEn,
  GsmMccRd2rdEchoMask,
  GsmMccWr2wrEchoMask,
  GsmMccDdr1rSplitBgOnSubch,
  EndOfMccMarker,               ///< End of MC Configuration Group.
  GsmPmaEnableMc,
  GsmPmaEnableIbecc,
  GsmPmaEnableCce,
  GsmPmaEnableTme,
  GsmPmaDdrType,
  GsmPmaMaxBandwidth,
  GsmPmaQclkRatioReq,
  GsmPmaGearTypeReq,
  GsmPmaDvfsVddq,
  GsmPmaVddqChangeOnly,
  GsmPmaMaxBwGbps,
  GsmPmaQclkWpIdx,
  GsmPmaRunBusy,
  GsmPmaMrcSave,
  GsmPmaMemConfigDone,
  GsmPmaMemConfigDoneAck,
  GsmPmaQclkRatioData,
  GsmPmaGearTypeData,
  GsmPmaErrorCode,
  GsmPmaBiosMailboxReserved,
  EndOfPmaMarker,               ///< End of Pma Group.
  GsmCkdPllMode,
  GsmCkdIctInputClkTerm,
  GsmCkdQckClkDisable,
  GsmCkdQckOutputDelayEnable,
  GsmCkdQckDrive,
  GsmCkdQckSlewRate,
  GsmCkdQckOutputDelay,
  GsmCkdErrorParityCheck,
  GsmCkdErrorPacketCheck,
  GsmCkdClearParityErrSts,
  GsmCkdClearPacketErrSts,
  EndOfCkdMarker,               ///< End of Ckd Group.
  GsmComplexRcvEn,
  GsmComplexRxBias,             ///< Combines the control of RxBiasRComp and RxBiasIComp into one linear field.
  GsmGtMax,                     ///< GSM_GT enumeration maximum value.
  GsmDebugStringMax,            ///< Used to control Debug string array size.
  GsmGtDelim = MRC_INT32_MAX        ///< This value ensures the enum size is consistent on both sides of the PPI.
} GSM_GT;
#endif // _BlueMrcCommonTypes_h_

