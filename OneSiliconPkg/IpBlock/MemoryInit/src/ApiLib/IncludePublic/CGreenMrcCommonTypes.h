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
#ifndef _GreenMrcCommonTypes_h_
#define _GreenMrcCommonTypes_h_
#include "CMrcCommonTypes.h" // for definitions common to blue and green

#ifdef XCC_BUILD
#define FUNCTION_LOCATION_GREEN_IRAM_0 __attribute__ ((section (".iram0.text")))
#define FUNCTION_LOCATION_GREEN_IRAM_1 __attribute__ ((section (".iram1.text")))
#else
#define FUNCTION_LOCATION_GREEN_IRAM_0
#define FUNCTION_LOCATION_GREEN_IRAM_1
#endif // #ifdef XCC_BUILD
#define MRC_IRAM0_FUNCTION FUNCTION_LOCATION_GREEN_IRAM_0
#define MRC_IRAM1_FUNCTION FUNCTION_LOCATION_GREEN_IRAM_1

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
  DqsEqLegs,                    ///<
  DqsEqRes,                     ///<
  DqsEqCap,                     ///<
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
  DisRankSwitchClkGating,
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
  RxVrefVddqDecap,              ///< Control Rx Vref coupling to Vddq.  0: No cap to Vddq  1-7 is cap control with 12.5% step size. Remaining cap will be to VSS
  RxTap0,                       ///< Tap 0 Coefficient control (unsigned). Prior sample of 0 injects +RxEqRankXTapY*dV. dV=3.5% of VccIOG for LP5 RX and dV=7% of VccIOG for DDR5 RX.
  RxTap1,                       ///< Tap 1 Coefficient Control (2s complement). Prior sample of 0 injects +RxEqRankXTapY*dV. dV=3.5% of VccIOG.
  RxTap2,                       ///< Tap 2 Coefficient control (2s complement). Prior sample of 0 injects +RxEqRankXTapY*dV. dV=3.5% of VccIOG.
  RxTap3,                       ///< Tap 3 Coefficient Control (2s complement). Prior sample of 0 injects +RxEqRankXTapY*dV. dV=3.5% of VccIOG.
  DqsOdtCompOffset,             ///< Offset for DQS OdtUp & OdtDn, decoupled from DQ ODT values for better power control in the system
  DqOdtCompOffset,              ///< 2s Complement offset added to both DQ OdtUp and OdtDn Value. Positive # increases termination and each step is ~5%
  RxXtalkDQ10,                  ///< Cross talk cancellation value to apply to DQ[1:0].
  RxXtalkDQ32,                  ///< Cross talk cancellation value to apply to DQ[3:2].
  RxXtalkDQ54,                  ///< Cross talk cancellation value to apply to DQ[5:4].
  RxXtalkDQ76,                  ///< Cross talk cancellation value to apply to DQ[7:6].
  DqSCompOffset,                ///< 2s Complement offset added to DQ Slew Rate Comp Value. Positive # increases slew rate and each step is ~5%
  DqDrvCompOffset,              ///< 2s Complement offset added to DQ Drv Comp Value. Positive # increases drive strength and each step is ~5%
  RloadCompOffset,              ///< 2s Complement offset added to Rload Comp Code. Positive # increases slew rate and each step is ~5%
  CmdRCompDrvOffset,            ///< 2s Complement Cmd offset to drive strength. ?CH_SUB
  CmdSCompOffset,               ///< 2s Complement Cmd offset to slew rate. Positive # decreases slew rate and each step is ~5% ?CH_SUB
  ClkRCompDrvOffset,            ///< 2s Complement Clk offset to drive strength. ?CH_SUB
  CtlRCompDrvOffset,            ///< 2s Complement Ctl offset to drive strength. ?CH_SUB
  CtlSCompOffset,               ///< 2s Complement Ctl offset to slew rate. Positive # decreases slew rate and each step is ~5% ?CH_SUB
  CompOffsetAll,                ///< Temporary holder till comps are enumerated.
  CmdSlewRate,                  ///< Monotonic increment, where the positive increment adds more delay cells in the Command delay line.
  ClkRon,                       ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  CtlSlewRate,                  ///< Monotonic increment, where the positive increment adds more delay cells in the Control delay line.
  CtlRon,                       ///< Resistance setting within a set of possible resistances, which may be a different set of values per product. Indexed by integer values.
  CmdScompPC,                   ///< Phase or Cycle Lock Type for CmdSlewRate
  CtlScompPC,                   ///< Phase or Cycle Lock Type for CtlSlewRate
  SCompCodeCmd,                 ///< Slew Comp Code for Cmd
  SCompCodeCtl,                 ///< Slew Comp Code for Ctl
  SCompBypassCmd,               ///< Bypass CA Slew delay
  SCompBypassCtl,               ///< Bypass Ctl Slew delay
  WrDSCodeUpCmd,                ///< RcompUp Code for Write Drive Strength-Cmd
  WrDSCodeUpCtl,                ///< RcompUp Code for Write Drive Strength-Ctl
  WrDSCodeUpClk,                ///< RcompUp Code for Write Drive Strength-Clk
  WrDSCodeDnCmd,                ///< RcompDown Code for Write Drive Strength-Cmd
  WrDSCodeDnCtl,                ///< RcompDown Code for Write Drive Strength-Ctl
  WrDSCodeDnClk,                ///< RcompDown Code for Write Drive Strength-Clk
  DqOdtVrefUp,
  DqOdtVrefDn,
  DqDrvVrefUp,
  DqDrvVrefDn,
  CmdDrvVrefUp,
  CmdDrvVrefDn,
  CtlDrvVrefUp,
  CtlDrvVrefDn,
  ClkDrvVrefUp,
  ClkDrvVrefDn,
  CompRcompOdtUp,               ///< The resistive value of the pull-up block of on die termination
  CompRcompOdtDn,               ///< The resistive value of the pull-dn block of on die termination
  CompRcompDrvUp,               ///< The resistive value (aka driving strength) of the pull-up block of the transmitter
  CompRcompDrvDn,               ///< The resistive value (aka driving strength) of the pull-down block of the transmitter
  RloadCompCode,
  RxVocFall,
  RxVocFallDbi,
  TxDqDccOffset,
  TxDqsDccOffset,
  CaPerBitCcc,
  CsPerBitCcc,
  CkePerBitCcc,
  OdtPerBitCcc,
  BaPerBitCcc,
  BgPerBitCcc,
  ClkPerBitCcc,
  WckPerBitCcc,
  MiscPerBitCcc,
  WrRetrainDeltaPiCode,
  RdRetrainDeltaPiCode,
  RefPiFsmRun,
  RefPiFsmInvFsmFB,
  RefPiFsmParam,
  RefPiFsmMaxVal,
  RefPiFsmMinVal,
  RefPiFsmMinWidth,
  RefPiFsmTestCycle,
  RefPiFsmSaveModN,
  RefPi,
  RefPiOffset,
  RefPiOffsetIog,
  RefPi4XoverOther,
  RefPi4XoverOtherOffset,
  RefPi4XoverClk,
  RefPi4XoverClkOffset,
  RefPi4Xover,
  RefPi4XoverOffset,
  RefPiCcc,
  RefPiOffsetCcc,
  RefPi4XoverCcc,
  RefPi4XoverOffsetCcc,
  RxCompDqsDelayP,
  RxCompDqsDelayN,
  RxCompDqsTargetP,
  RxCompDqsTargetN,
  RxCompDqsTarget,
  RxCompDqsOffsetP,
  RxCompDqsOffsetN,
  RxCompDqsOffset,
  RxTrainTargetOffsetUI,
  RcvEnWaveShape,
  RXTogglePreAmble,
  RcvEnFfMuxSelByte0,
  RcvEnExtFF,
  RstNumPre,
  RxReadPointer,
  RdEarlyDqs,
  UseDefaultRdPtrCalc,
  RcompVrefVdd2,
  Vdd2PowerReadpJ,
  Vdd2PowerWritepJ,
  Vdd2PowerIdlemW,
  Vdd2PowerCkemW,
                                ///< -----------------------------------------------------------------
  EndOfPhyMarker,               ///< Marker for end of phy groups.
                                ///< -----------------------------------------------------------------
  GsmIocIoReset,                ///< Used to reset the DDR IO.
  GsmIocDccClkGateDisable,
  GsmIocNoDqInterleave,
  GsmIocScramDdr5Mode,          ///< Set the DDRIO DDRSCRAM to DDR5 Mode
  GsmIocScramGearMode,          ///< Program gear in DDRSCRAM: 1 = Gear4, 0 = Gear2
  GsmTrainWCkEn,
  GsmTrainWCkPulse,
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
  GsmIocDataOdtStaticDis,
  GsmIocCompOdtStaticDis,
  GsmIocEnableBwSelComp,
  GsmIocBwSelDone,
  GsmIocBwSelError,
  GsmIocStrobeOdtStaticDis,     ///< 2 bits to control ddrxover cell for mindelay margin improvment [0]: q2q0_sel - controls margin from Qclk to Q0clk[1]: q02pi_sel - controls margin from Q0clk to PIclk
  GsmIocDataODTSingleSegEn,     ///< ODT enables (0: 2 segment, 1: 1 segment) of the 2 DQ Driver segments.  CR is independent of Control0.OdtMode.
  GsmIocDataOdtMode,
  GsmIocDataDqOdtParkMode,
  GsmIocDataEnDQODTParkHi,
  GsmIocRxVrefMFC,              ///< RX Vref Metal Finger Cap. ?Vtt vs Vddq
  GsmIocDisableTxDqs,
  GsmIocDbiEnable,
  GsmIocDbiMode,
  GsmIocForceRxAmpOn,
  GsmIocForceRxOnDqs,
  GsmIocForceRxOnDqsMux,
  GsmIocForceRxOnDq,
  GsmIocEnDqOdtParkMode,
  GsmIocDqsIdleModeDrvEn,
  GsmIocDqDrvStatLegEn,
  GsmIocTxOn,
  GsmIocRxDisable,
  GsmIocSenseAmpMode,
  GsmIocDqsRFMode,                ///< Mode for DQS Rise/Fall testing
  GsmIocClkEnFeedback,            ///< Enable Feedback Mode for Clk P Fall/N Rise and Clk P Rise/N Fall
  GsmIocWckEnFeedback,            ///< Enable Feedback Mode for Wck P Fall/N Rise and Wck P Rise/N Fall
  GsmIocWckEnFeedbackCh0,         ///< Enable Feedback Mode for Wck P Fall/N Rise and Wck P Rise/N Fall
  GsmIocWckEnFeedbackCh1,         ///< Enable Feedback Mode for Wck P Fall/N Rise and Wck P Rise/N Fall
  GsmIocClkPFallNRiseFeedback,    ///< Clk P Fall/N Rise Feedback
  GsmIocClkPRiseNFallFeedback,    ///< Clk P Rise/N Fall Feedback
  GsmIocClkPFallNRise,            ///< Clk P Fall/N Rise Setting
  GsmIocClkPRiseNFall,            ///< Clk P Rise/N Fall Setting
  GsmIocWckPFallNRiseFeedback,    ///< Wck P Fall/N Rise Feedback
  GsmIocWckPFallNRiseFeedbackCh0, ///< Wck P Fall/N Rise Feedback
  GsmIocWckPFallNRiseFeedbackCh1, ///< Wck P Fall/N Rise Feedback
  GsmIocWckPRiseNFallFeedback,    ///< Wck P Rise/N Fall Feedback
  GsmIocWckPRiseNFallFeedbackCh0, ///< Wck P Rise/N Fall Feedback
  GsmIocWckPRiseNFallFeedbackCh1, ///< Wck P Rise/N Fall Feedback
  GsmIocWckPFallNRise,            ///< Wck P Fall/N Rise Setting
  GsmIocWckPFallNRiseCh0,         ///< Wck P Fall/N Rise Setting
  GsmIocWckPFallNRiseCh1,         ///< Wck P Fall/N Rise Setting
  GsmIocWckPRiseNFall,            ///< Wck P Rise/N Fall Setting
  GsmIocWckPRiseNFallCh0,         ///< Wck P Rise/N Fall Setting
  GsmIocWckPRiseNFallCh1,         ///< Wck P Rise/N Fall Setting
  GsmIocCaTrainingMode,
  GsmIocCaParityTrain,
  GsmIocCaParityOdd,
  GsmIocDdr5CsTrainingMode,
  GsmIocReadLevelMode,
  GsmIocWriteLevelMode,
  GsmIocReadDqDqsMode,
  GsmIocRxClkStg,               ///< Number of clock cycles to keep clock on in the RX datapath from the fall of RcvEn signal.
  GsmIocDataRxBurstLen,         ///< Configures the burst length of Read at the data partition.
  GsmIocEnDqsNRcvEn,
  GsmIocBlockSdlWithRcvEn,
  GsmIocForceRcvEnOn,
  GsmIocForceOdtOn,
  GsmIocForceOdtOnWithoutDrvEn,
  GsmIocVccClkCompCbbIBonus,
  GsmIocCbEnTxDll,
  GsmIocCbEnRxDll,
  GsmIocCbEnTxDllCcc,
  GsmIocBwCtrlTxDll,
  GsmIocBwCtrlRxDll,
  GsmIocBwCtrlTxDllCcc,
  GsmIocLeakCodeTxDll,
  GsmIocLeakCodeRxDll,
  GsmIocLeakCodeTxDllCcc,
  GsmIocRxPiPwrDnDis,
  GsmIocTxPiPwrDnDis,
  GsmIocTxDisable,              ///< Ignore Write commands for this data byte
  GsmIocCompClkOn,
  GsmIocRLoadNumSegs,
  GsmIocRLoadDqsVrefCode,
  GsmIocSinStep,
  GsmIocSinStepAdv,
  GsmIocDllSegmentDisable,
  GsmIocCompDllSegmentDisable,
  GsmIocVcdlDccDllDcdTargetSel,
  GsmIocDllWeakLock,
  GsmIocTxDllWeakLock,
  GsmIocDllParkToggle,
  GsmIocInputClkSelect,
  GsmIocDllEnPhaseGating,
  GsmIocEnablePiWhenOff,
  GsmIocDataPhaseDrvPiPMEn,
  GsmIocBypMajVoteForPd,
  GsmIocDataRxPiDis,
  GsmIocDataTxPiDis,
  GsmIocViewPinEqDelay,
  GsmIocWrEnViewDrv,
  GsmIocViewCtlDrvEn,
  GsmIocDrv2xSen,
  GsmIocCccSEClk,
  GsmIocWckSEClk,
  GsmIocWckPIClkDuration,
  GsmIocCccCaVoltageSelect,
  GsmIocCccCtlVoltageSelect,
  GsmIocCccClkVoltageSelect,
  GsmIocWckClkVoltageSelect,
  GsmIocDdr5CkdMode,                ///< Forces CLK to Toggle by forcing into DDR5 CKD Mode
  GsmIocCccTxCANmosOnly,
  GsmIocCccTxCTLNmosOnly,
  GsmIocCccTxCLKNmosOnly,
  GsmIocDataTxDqNmosOnly,
  GsmIocWckTxCLKNmosOnly,
  GsmIocDisWckPupDcc,
  GsmIocCompTxCANmosOnly,
  GsmIocCompTxCTLNmosOnly,
  GsmIocCompTxCLKNmosOnly,
  GsmIocCompTxDqNmosOnly,
  GsmIocCompForceLvrOn,
  GsmIocRptChRepClkOn,              ///< Forces the RPTCH clock on.
  GsmIocTxAnlgEnGraceCnt,           ///< Grace conter for extending the TxAnalogEn counter.
  GsmIocInternalClocksOn,           ///< Force on all the PI enables
  GsmIocInternalTxClocksOn,         ///< Enable Internal Tx Clocks
  GsmIocInternalRxClocksOn,         ///< Enable Internal Rx Clocks
  GsmIocDataRunRefPiFsm,            ///< Start the RefPi FSM in DATA
  GsmIocCccRunRefPiFsm,             ///< Start the RefPi FSM in CCC
  GsmIocDqsMaskPulseCnt,            ///< Controls the number of masked DQS pulses sent by the DDR IO during CMD VREF update in LP4 CBT
  GsmIocDqsPulseCnt,                ///< Controls the number of unmasked DQS pulses sent by the DDR IO during CMD VREF update in LP4 CBT
  GsmIocDqOverrideData,             ///< DQ value override to latch CMD VREF to the DRAM during LP4 CBT. This field is used to drive DQ7 High during LP5 ECT.
  GsmIocDqOverrideEn,               ///< Enables the DDR IO to force static DQ value to latch CMD VREF to the DRAM during LP4 CBT. This field is used to drive DQ7 High during LP5 ECT.
  GsmIocDqsOvrdToggle,
  GsmIocRankOverrideEn,             ///< Configures the DDRIO to override the delay values used for the target rank.
  GsmIocRankOverrideVal,            ///< Specified which rank to pull the delay values from.
  GsmIocDataCtlGearMode,            ///< Program gear in DDRDATA: 1 = Gear4, 0 = Gear2
  GsmIocDataWrPreamble,             ///< The number of Write Preambles to send to the DRAM.  Should match MR encoding.
  GsmIocDccTrainingMode,
  GsmIocDccTrainingDone,
  GsmIocDccDrain,
  GsmIocDccDcoCompEn,
  GsmIocDataDisableTxDqs,
  GsmIocRxVocMode,                  ///< Controls the Sense Amp mode
  GsmIocDataTrainFeedback,          ///< DataTrainFeedback
  GsmIocCsPulseCnt,
  GsmIocCsPulseReset,
  GsmIocDataDqsOdtParkMode,
  GsmIocDataDqsNParkLow,
  GsmIocRxAmpOffsetEn,
  GsmIocBiasPMCtrl,
  GsmIocFFCodeRxWl,
  GsmIocFFCodeRxLock,
  GsmIocFFCodeRxPhEn,
  GsmIocFFCodeRxPi,
  GsmIocFFCodeTxDllCccOffset,
  GsmIocDataInvertNibble,
  GsmIocEnableSpineGate,
  GsmIocForceRstDiv,
  GsmIocCompRunRefPiFsm,
  GsmIocPhaseDrvGBIB,
  GsmIocVrefMuxSel,                ///< Vref ladder Power Mux Select
  GsmIocEnableLpMode4,
  GsmIocLpCtrlEn,
  GsmDataDqDccLaneStatusResult,
  GsmDataDqsDccLaneStatusResult,
  GsmDataDccSaveFullDcc,
  GsmDataDccSkipCRWrite,
  GsmDataDccMeasPoint,
  GsmDataDccRankEn,
  GsmDataDccRankMap,
  GsmDataDccLaneEn,
  GsmDataDccEnRankOvrd,
  GsmDataDccSamples,
  GsmDataDccEnChop,
  GsmDataDccStepSize,
  GsmDataDqTarget,
  GsmDataDqsTarget,
  GsmDataDccStatusLargeChange,
  GsmDataDccLargeChange,
  GsmDataDccLargeChangeEnable,
  GsmDataDccLargeChangeReset,
  GsmCccDccLaneStatusResult,
  GsmWckDccLaneStatusResult,
  GsmClkDccStepSize,
  GsmClkEnLane12,
  GsmClkDccStatusLargeChange,
  GsmClkDccLargeChange,
  GsmClkDccLargeChangeEnable,
  GsmClkDccLargeChangeReset,
  GsmClkDccTargetDutyCycle,
  GsmClkDccRankEn,
  GsmClkDccLaneEn,
  GsmClkDccSamples,
  GsmClkDccUpdateTcoComp,
  GsmClkDccMeasPoint,
  GsmClkDccFsmSaveFullDcc,
  GsmClkDccFsmSkipCrWrite,
  GsmClkDccFsmPiDccEn,
  GsmClkDccFsmEnChop,
  GsmClkTcoCode,
  GsmClkDccCode,
  GsmWckDccEnRankOvrd,
  GsmWckDccRankEn,
  GsmWckDccLaneEn,
  GsmWckDccSamples,
  GsmWckDccUpdateTcoComp,
  GsmWckDccMeasPoint,
  GsmWckDccFsmEnChop,
  GsmWckDccFsmSaveFullDcc,
  GsmWckDccFsmSkipCrWrite,
  GsmIocTcoCode,
  GsmIocDccCode,
  GsmIocLaneEn,
  GsmIocLaneEnCh0,
  GsmIocLaneEnCh1,
  GsmIocPiDivInc,
  GsmIocPiDivEn,
  GsmIocLaneEnCcc,
  GsmIocPiDivEnCcc,
  GsmIocPiEnCcc,
  GsmIocPiCodeCcc,
  GsmWckDccStepSize,
  GsmWckDccStatusLargeChange,
  GsmWckDccLargeChange,
  GsmWckDccLargeChangeEnable,
  GsmWckDccLargeChangeReset,
  GsmWckDccTargetDutyCycle,
  GsmDataVccDdqOCDivider,
  GsmDataLvrAutoTrimRunBusy,
  GsmDataLvrAutoTrimOffsetIOG,
  GsmDataLvrAutoTrimOffsetClk,
  GsmCccLvrAutoTrimRunBusy,
  GsmCccLvrAutoTrimOffsetIOG,
  GsmCccLvrAutoTrimOffsetClk,
  GsmCompVccDdqTarget,
  GsmCompVccIoTarget,
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
  GsmCompLvrAutoTrimRunBusy,
  GsmCompLvrAutoTrimOffsetIOG,
  GsmCompLvrAutoTrimOffsetIOGGV,
  GsmCompLvrAutoTrimOffsetDist,
  GsmLvrAutoTrimRunBusy,
  GsmLvrPwrGoodOnVrefChange,
  GsmLvrAutoTrimSelLvr,
  GsmLvrAutoTrimDelay,
  GsmLvrAutoTrimStatusChop0,
  GsmLvrAutoTrimStatusChop1,
  GsmLvrAutoTrimStatusVsenseOffset,
  GsmLvrAutoTrimOffsetIOG,
  GsmLvrAutoTrimOffsetClkTx,
  GsmLvrAutoTrimCccOffsetIOG,
  GsmLvrAutoTrimCccOffsetClkTx,
  GsmLvrAutoTrimOffsetClkRx0,
  GsmLvrAutoTrimOffsetClkRx1,
  GsmLvrAutoTrimOffsetNbiasFF,
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
  GsmInitCompleteOvrd,
  GsmInitCompleteOvrdVal,
  GsmIOGGVLVROvrdModeEn,
  GsmIOGLVREnableOvrd,
  GsmIOGLVROvrdModeEn,
  GsmIOGLVRVrefEnOvrd,
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
  GsmIOGPwrGoodOvrd,
  GsmCriClkGateDis,
  GsmIocVccClkFFCRWait,
  GsmVccIoVccF0,
  GsmVccIoVccF1,
  GsmVccIoVccF2,
  GsmVccIoOCSlope,
  GsmVccClkVccF0,
  GsmVccClkVccF1,
  GsmVccClkVccF2,
  GsmVccClkOCSlope,
  GsmPostCode,
  GsmDdrPerfMonCountEn,
  GsmVccSagPrampiCtl,
  GsmVccDd2gPrampiCtl,
  GsmVccDdqgPrampiCtl,
  GsmVccSaxgPrampiCtl,
  GsmWorkPointAuxClk,
  GsmWorkPointRatio,
  GsmWorkPointGear4,
  GsmWorkPointLCPLLWPSel,
  GsmWorkPointPhClkRefClk,
  GsmWorkPointWP2LCPLL,
  GsmTrimClkOvrd,
  GsmDccPHClkPhaseEn,
  GsmDccQClkgateDisable,
  GsmDccPHClkDutyCycleEn,
  GsmTxDllDccCodeExtendEn,
  GsmTxDllDccEn,
  GsmDccQclkEn,
  GsmTxDllDccFsmEn,
  GsmIocForceDllReset,        // Reset the Tx DLL in DATA
  GsmIocForceRxDllReset,      // Reset the Rx DLL in DATA
  GsmIocDllReady,
  GsmSaveFullDcc,
  GsmDccSamples,
  GsmDccLargeChange,
  GsmDccLargeChangeEnable,
  GsmDccLargeChangeReset,
  GsmDccTxDllCode,
  GsmDccVcdlCode,
  GsmDccVcdlCodeEn,
  GsmDccDllDcdTargetSel,
  GsmDccSkipCrWrite,
  GsmDccStepSize,
  GsmQClkDccSamples,
  GsmQclkSaveFullDcc,
  GsmQClkSkipCrWrite,
  GsmQClkDccStepSize,
  GsmMcQClkEn,
  GsmPhyQClkEn,
  GsmPhyQClkTarget,
  GsmMcQClkTarget,
  GsmPhyQclkResult,
  GsmMcQclkResult,
  GsmPhyQclkDcc,
  GsmMcQclkDcc,
  GsmMcQclkPiCode,
  GsmMcQclkPhFsmStart,
  GsmMcQclkPhFsmPm,
  GsmMcQclkPhaseOffset,
  GsmMcQclkPhLockSel,
  GsmMcQclkMaxQclkGatedDuration,
  GsmMcQclkPhLock,
  GsmMcQclkPhStickyLock,
  GsmMcQclkPhReady,
  GsmMcQclkPhDlockWakeup3,
  GsmMcQclkPhDlockPm6,
  GsmMcQclkPhAlignCount0,
  GsmMcQclkPhAlignCount1,
  GsmPHClkTarget,
  GsmMcQclkIolbCycles,
  GsmMcQclkRunTest,
  GsmMcQclkLaneError,
  GsmPhDcc,
  GsmDccPhResult,
  GsmDLEn,
  GsmDccPhDelay,
  GsmPhaseDrvGPMEn,
  GsmDccPiLutFine,
  GsmMixerCode,
  GsmCccMixerCode,
  GsmCompMixerCode,
  GsmCompCtrlPeriodicComp,
  GsmCompCtrlDVFSComp,
  GsmCompCtrlCompDist,
  GsmCompCtrlCompUpdate,
  GsmDeltaPiCode,
  GsmFusePllLockSel,
  GsmIocRetrainSwizzleCtlByteSel,
  GsmIocRetrainSwizzleCtlBit,
  GsmIocRetrainSwizzleCtlRetrainEn,
  GsmIocRetrainSwizzleCtlSerialMrr,
  GsmIocRetrainInitPiCode,
  GsmIocRetrainRoCount,
  GsmIocRetrainCtlInitTrain,
  GsmIocRetrainCtlDuration,
  GsmIocRetrainCtlResetStatus,
  GsmIocRdRetrainCtlInitTrain,
  GsmIocRdRetrainCtlResetStatus,
  GsmIocAuxClkOffAuxClk,
  GsmIocAuxClkOffQClkPhClk,
  GsmIocQClkCalOff,
  GsmLvrEnSensor,
  GsmLvrSelCount,
  GsmLvrUsePSAL,
  GsmLvrChop,
  GsmLvrTarget0,
  GsmLvrTarget1,
  GsmLvrVinDivider,
  GsmLvrFusaUndervoltage,
  GsmLvrFusaOvervoltage,
  GsmLvrVoltageSensorSelLvr,
  GsmLvrSelLVR,
  GsmLvrEnDiv,
  GsmLvrRstCount,
  GsmLvrCountMSB,
  GsmLvrVoltageSensorCount0,
  GsmLvrVoltageSensorCount1,
  GsmLvrVoltageSensorCntStart,
  GsmLvrVoltageSensorDuration,
  GsmVsBonusIn,
  GsmVsSelVdd2Ladder,
  GsmViewSel,
  GsmViewFubAddr,
  GsmIocR2RTrainingMode,
  GsmIocPHClkOffsetFastCal,
  GsmIocAuxOffsetFastCal,
  GsmIocOvrdAuxPLLEn,
  GsmIocOvrdAuxClock,
  GsmIocAuxClkFinalLock,
  GsmRcompDrvDownVdd2,
  GsmRcompDrvUpVdd2,
  GsmViewDrvDn,
  GsmViewDrvUp,
  GsmMiscPmMessage,
  GsmMiscPmMessageRunBusy,
  GsmMiscPmMessageWpSel,
  GsmMptuGlobalStart,
  GsmIocDfiMuxSel,
  GsmIocDunitClkReset,
  GsmIocDunitClkDiv,
  GsmIocMptuReset,
  GsmIocMptuCriReset,
  GsmIocMptuEnable,
  GsmFFCodePhDrvG,
  GsmDataVccClkFFCurrentSensorCntStart,
  GsmDataVccClkFFCurrentSensorSelCnt,
  GsmDataVccClkFFCurrentSensorDuration,
  GsmDataVccClkFFCurrentSensorEnCnt,
  GsmDataVccClkFFCurrentSensorCount,
  GsmCccVccClkFFCurrentSensorCntStart,
  GsmCccVccClkFFCurrentSensorSelCnt,
  GsmCccVccClkFFCurrentSensorDuration,
  GsmCccVccClkFFCurrentSensorEnCnt,
  GsmCccVccClkFFCurrentSensorCount,
  GsmLvrCurrentSensorCntStart,
  GsmLvrCurrentSensorEnChop,
  GsmLvrCurrentSensorEnSensor,
  GsmLvrCurrentSensorEnCnt,
  GsmLvrCurrentSensorDuration,
  GsmLvrCurrentSensorAmpHyster,
  GsmLvrCurrentSensorSelLVR,
  GsmLvrCurrentSensorCount,
  GsmCompLvrCurrentSensorCntStart,
  GsmCompLvrCurrentSensorAmpHyster,
  GsmCompLvrCurrentSensorEnChop,
  GsmCompLvrCurrentSensorEnCnt,
  GsmCompLvrCurrentSensorEnSensor,
  GsmCompLvrCurrentSensorDuration,
  GsmCompLvrCurrentSensorSelLVR,
  GsmCompLvrCurrentSensorCount,
  GsmDataWrongPhaseCal,
  GsmDataWrongPhaseHighLimit,
  GsmDataWrongPhaseLowLimit,
  GsmIocITerdcdselovrval,
  GsmIocITerdcdselovren,
  GsmIocRxDllTerDcdSelRefClkByte0,
  GsmIocRxDllTerDcdSelRefClkByte1,
  GsmCccWrongPhaseCal,
  GsmCccWrongPhaseHighLimit,
  GsmCccWrongPhaseLowLimit,
  GsmRefPi4XoverOffsetWckByte0,
  GsmRefPi4XoverOffsetWckByte1,
  GsmRefPi4XoverOffsetDqsByte0,
  GsmRefPi4XoverOffsetDqsByte1,
  GsmIocCccDllDetrmLockStat,
  GsmIocDataDllDetrmLockStat,
  GsmIocCs2NReset,
  GsmIocCsGearDownEnable,
  GsmIocCsGearDownForce,
  GsmIocCsGearDownSrDramTrack,
  GsmIocCsGearDownRankEn,
  GsmIocPhBufBonus,
  GsmIocPiCodeMsbOvrd,
  GsmIocDataEnWrCRC,
  GsmIocMiscEnWrCRC,
  GsmIocMiscTxBurstLen,
  GsmIocMiscRxBurstLen,
  GsmIocDataDisableWriteBackCriClkGating,
  GsmIocCompDisableWriteBackCriClkGating,
  GsmIocScramDisableWriteBackCriClkGating,
  GsmIocCccDisableWriteBackCriClkGating,
  GsmIocDataShrDisableWriteBackCriClkGating,
  GsmIocCccShrDisableWriteBackCriClkGating,
                                    ///< -----------------------------------------------------------------
  EndOfIocMarker,                   ///< End of IO Config Marker
                                    ///< -----------------------------------------------------------------
  GsmMtgLoopCount,
  GsmMptuRd2WckOff,
  GsmMptuWr2WckOff,
  GsmMptuWckCtl,
  GsmMptuWckEnlFs,
  GsmMptuWckEnlRd,
  GsmMptuWckEnlWr,
  GsmMptuWckPreStatic,
  GsmMptuWckStop,
  GsmMptuTckckel,
  GsmMptuTcke,
  GsmMptuTckCkeH,
  GsmMptuTsr,
  GsmMptuTriCa,
  GsmMptuStopCk,
  GsmMptuForceCkStop,
  GsmMptuCkStopDelay,
  GsmMptuIgnorePhyUpdReq,
  GsmMptuTrfi,
  GsmMptuTrfcab,
  GsmMptuTxsdll,
  GsmMptuTzqcal,
  GsmMptuTzqlat,
  GsmMptuChannelWidth,
  GsmMptussChannelWidth,
  GsmMptuDramType,
  GsmMptuDeviceWidth,
  GsmMptuBankOrg,
  GsmMptuRankEn,
  GsmMptuBurstLength,
  GsmMptuEccEn,
  GsmMptuChanEn,
  GsmMptuDfiPhyUpdateEn,
  GsmMptuTxCh0MptuMap,
  GsmMptuTxCh1MptuMap,
  GsmMptuTxCh2MptuMap,
  GsmMptuTxCh3MptuMap,
  GsmMptuTxCh4MptuMap,
  GsmMptuTxCh5MptuMap,
  GsmMptuTxCh6MptuMap,
  GsmMptuTxCh7MptuMap,
  GsmMptuMptu0Ch0RxEn,
  GsmMptuMptu0Ch0RxMap,
  GsmMptuMptu0Ch1RxEn,
  GsmMptuMptu0Ch1RxMap,
  GsmMptuMptu0Ch2RxEn,
  GsmMptuMptu0Ch2RxMap,
  GsmMptuMptu0Ch3RxEn,
  GsmMptuMptu0Ch3RxMap,
  GsmMptuMptu1Ch0RxEn,
  GsmMptuMptu1Ch0RxMap,
  GsmMptuMptu1Ch1RxEn,
  GsmMptuMptu1Ch1RxMap,
  GsmMptuMptu1Ch2RxEn,
  GsmMptuMptu1Ch2RxMap,
  GsmMptuMptu1Ch3RxEn,
  GsmMptuMptu1Ch3RxMap,
  GsmMptuDfiInitStartSel,
  GsmMptuDCrDfiStartCtrl,
  GsmMptuSreWfc,
  GsmMptuDramCmdData,
  GsmMptuDramCmdCs,
  GsmMptuMrrResult,
  GsmMptuMntEn,
  GsmMptuZqCal,
  GsmMptuRefWm,
  GsmMptuRefRate,
  GsmMptuDunitInitDone,
  GsmMptuDunitDdr52nCsPos,
  GsmMptuDunitBgSwapDisable,
  GsmMptuDunitEvenPhaseCmd,
  GsmMptuDCrPdDis,
  GsmMptuDCrPreaBeforePde,
  GsmMptuManualSre,
  GsmMptuManualSrx,
  GsmMptuDynamicSrEn,
  GsmMptuStatusInSr,
  GsmMptuPhyInitComplete,
  GsmMptuCsMode,
  GsmMptuCsVal,
  GsmMptuCsMask,
  GsmMptuMultiCycleCs,
  GsmMptuCbtMode,
  GsmMptuCbtPhase,
  GsmMptuBlockRdData,
  GsmMptuDgbDphaseSel,
  GsmMptuDgbPphaseSel,
  GsmMptuDgbRxRptrInitValue,
  GsmMptuDgbFifoPtrEn,
  GsmMptutRCD,
  GsmMptutRPpb,
  GsmMptutRPab,
  GsmMptutRAS,
  GsmMptutRDPRE,
  GsmMptutWRPRE,
  GsmMptutRRDdg,
  GsmMptutRDRDsg,
  GsmMptutRDRDdg,
  GsmMptutRDRDdr,
  GsmMptutRDRDdd,
  GsmMptutWRWRsg,
  GsmMptutWRWRdg,
  GsmMptutWRWRdr,
  GsmMptutWRWRdd,
  GsmMptutRDWRsr,
  GsmMptutRDWRdr,
  GsmMptutRDWRdd,
  GsmMptutWRRDsg,
  GsmMptutWRRDdg,
  GsmMptutWRRDdr,
  GsmMptutWRRDdd,
  GsmMptutCWL,
  GsmMptutXP,
  GsmMptutXSR,
  GsmMptutCSH,
  GsmMptutCSL,
  GsmMptutRdPdn,
  GsmMptutWrPdn,
  GsmMptutPreMrBlk,
  GsmMptutPstMrrBlk,
  GsmMptutPstMrwBlk,
  GsmMptutMrr,
  GsmMptuDqs2DqCalEn,
  GsmMptuDqs2DqCalMode,
  GsmMptuWriteLevelMode,
  GsmMptuPdaMode,
  GsmMptuTCMD,
  GsmMptuDfi2NModeCtrl,
  GsmMptuWrOdtCtl,
  GsmMptuRdOdtCtl,
  GsmMtgStart,
  GsmMtgNonStop,
  GsmMtgManualPause,
  GsmMtgClrAllErrs,
  GsmMtgErrChkEn,
  GsmMtgReqSize,
  GsmMtgAddressOrder,
  GsmMtgBankIncOrder,
  GsmMtgRowStart,
  GsmMtgColStart,
  GsmMtgBankStart,
  GsmMtgLRankStart,
  GsmMtgRowEnd,
  GsmMtgColEnd,
  GsmMtgBankEnd,
  GsmMtgLRankEnd,
  GsmMtgCmd0Type,
  GsmMtgCmd0Count,
  GsmMtgCmd0Last,
  GsmMtgCmdEntryDelay,
  GsmMtgCmdLoopDelay,
  GsmMtgStopOnErrEn,
  GsmMtgStopAllDqErr,
  GsmMtgStopThreshold,
  GsmMtgChunkMask,
  GsmMtgDqMaskLow,
  GsmMtgDqMaskHi,
  GsmMtgEccMask,
  GsmMtgDqDcCtl,
  GsmMtgDqDcCtlEcc,
  GsmMtgDqInversion,
  GsmMtgDqInversionEcc,
  GsmMtgSerialCharEn,
  GsmMtgSerialPatternConf0,
  GsmMtgSerialPatternConf1,
  GsmMtgDqLfsr0Seed,
  GsmMtgDqLfsr1Seed,
  GsmMtgDqLfsr2Seed,
  GsmMtgDqLfsr3Seed,
  GsmMtgDqLfsr0Poly,
  GsmMtgDqLfsr1Poly,
  GsmMtgDqLfsr2Poly,
  GsmMtgDqLfsr3Poly,
  GsmMtgDqPatMode,
  GsmMtgDqPatRate,
  GsmMtgLfsrVictim,
  GsmMtgLfsrAggressor,
  GsmMtgDm0PatternTypeSelect,
  GsmMtgDqPatternSelect,
  GsmMtgSerialPatternsLength,
  GsmMtgChDataByteErr0,
  GsmMtgChDataByteErr1,
  GsmMtgEccByteErr,
  GsmMtgDqLaneErrEvenUI,
  GsmMtgDqLaneErrOddUI,
  GsmMtgGeneralErr,
  GsmMtgUiErr,
  GsmMtgErrCnt0Type,
  GsmMtgErrCnt0En,
  GsmMtgErrCnt1Type,
  GsmMtgErrCnt1En,
  GsmMtgErrCnt0Val,
  GsmMtgErrCnt1Val,
  GsmMtgByteErrorCntVal,
  GsmMtgByteErrorCntEn,
  GsmMtgL2PRankMapEn,
  GsmMtgL2PRankMap,
  GsmMtgRdErrTmoDly,
  GsmCatgStart,
  GsmCatgNonStop,
  GsmCatgMode,
  GsmCatgSelPeriod,
  GsmCatgSelCs,
  GsmCatgSelPhase,
  GsmCatgCaPatRotateEn,
  GsmCatgVictimCaPatSel,
  GsmCatgAggressorCaPatSel,
  GsmCatgCaPatRotateRate,
  GsmCatgCaPatParityEnable,
  GsmCatgCaPatParityType,
  GsmCatgCalcParityWithCa13,
  GsmCatgCaPatParityPin,
  GsmCatgRunTime,
  GsmCatgLfsr0seed,
  GsmCatgLfsr1seed,
  GsmCatgLfsr0Poly,
  GsmCatgLfsr1Poly,
  GsmMtgDataByteErr,
  GsmCatgSelectPatternEn,
  GsmCatgDeselectPatternEn,
  GsmCatgSelectPattern,
  GsmCatgDeselectPattern,
                                    ///< -----------------------------------------------------------------
  EndOfMptuMarker,                  ///< End of MPTU Config Marker
                                    ///< -----------------------------------------------------------------
  EndOfMemSSMarker,                 ///< MemSS Config Marker
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

  GsmGtMax,                     ///< GSM_GT enumeration maximum value.
  GsmDebugStringMax,            ///< Used to control Debug string array size.
  GsmGtDelim = MRC_INT32_MAX        ///< This value ensures the enum size is consistent on both sides of the PPI.
} GSM_GT;
#endif // _GreenMrcCommonTypes_h_

