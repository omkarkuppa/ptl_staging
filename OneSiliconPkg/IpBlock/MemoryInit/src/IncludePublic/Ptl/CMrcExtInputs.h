/** @file
  This file contains the structure of MRC inputs that can be modified by customers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef MRC_EXT_INPUTS_H_
#define MRC_EXT_INPUTS_H_
#include "CMrcExtTypes.h"

/// Keep in sync with MEMORY_CONFIGURATION_REVISION in MemoryConfig.h
/// Rev 1:  - Initial version.
#define MRC_EXT_INPUTS_REVISION          1

#pragma pack(push, 1)

typedef struct {
  UINT16  Size;                   ///< Offset 0 The size of this structure, in bytes. Must be the first entry in this structure.
  UINT8   HobBufferSize;          ///< Offset 2 Size of HOB buffer for MRC

  UINT8   MemoryProfile;          ///< Offset 3 SPD XMP profile selection - for XMP supported DIMM: <b>0=Default DIMM profile</b>, 1=Customized profile, 2=XMP profile 1, 3=XMP profile 2.

  // The following parameters are used only when SpdProfileSelected is UserDefined (CUSTOM PROFILE)
  UINT16  tCL;                    ///< Offset 4 User defined Memory Timing tCL value,   valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 31=Maximum.
  UINT16  tRCDtRP;                ///< Offset 6 User defined Memory Timing tRCD value (same as tRP), valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 63=Maximum
  UINT16  tRAS;                   ///< Offset 8 User defined Memory Timing tRAS value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 64=Maximum.
  UINT16  tWR;                    ///< Offset 10 User defined Memory Timing tWR value,   valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, legal values: 5, 6, 7, 8, 10, 12, 14, 16, 18, 20, 24.
  UINT16  tRFC;                   ///< Offset 12 User defined Memory Timing tRFC value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 1023=Maximum.
  UINT16  tRRD;                   ///< Offset 14 User defined Memory Timing tRRD value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 15=Maximum.
  UINT16  tWTR;                   ///< Offset 16 User defined Memory Timing tWTR value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 28=Maximum.
  UINT16  tRTP;                   ///< Offset 18 User defined Memory Timing tRTP value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 15=Maximum. legal values: 5, 6, 7, 8, 9, 10, 12
  UINT16  tFAW;                   ///< Offset 20 User defined Memory Timing tFAW value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 63=Maximum.
  UINT16  tCWL;                   ///< Offset 22 User defined Memory Timing tCWL value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 20=Maximum.
  UINT32  tREFI;                  ///< Offset 24 User defined Memory Timing tREFI value, valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>, 131071=Maximum.
  UINT16  VddVoltage;             ///< Offset 28 DRAM voltage (Vdd) in millivolts: <b>0=Platform Default (no override)</b>, 1200=1.2V, 1350=1.35V etc.
  UINT16  Vdd2Mv;                 ///< Offset 30 Vdd2Mv

  UINT32  EccSupport:1;              ///< Offset 32 Bit 0  - DIMM Ecc Support option - for Desktop only: 0=Disable, <b>1=Enable</b>
  UINT32  ForceDIMMRXOFFSET:1;       ///<           Bit 1  - Force DIMM Rx Offset Calibration training for LPDDR5X, frequencies >= 6800: 0 = Disable, 1 = Enable
  UINT32  RemapEnable:1;             ///<           Bit 2  - This option is used to control whether to enable/disable memory remap above 4GB: 0=Disable, <b>1=Enable</b>.
  UINT32  ScramblerSupport:1;        ///<           Bit 3  - Memory scrambler support: 0=Disable, <b>1=Enable</b>
  UINT32  ProbelessTrace:1;          ///<           Bit 4  - Probeless Trace: <b>0=Disabled</b>, <b>1=Enabled</b>
  UINT32  PerBankRefresh:1;          ///<           Bit 5  - Enables and Disables the per bank refresh. FALSE=Disabled, <b>TRUE=Enabled</b>
  UINT32  SimicsFlag:1;              ///<           Bit 6  - Option to Enable SIMICS: 0=Disable, <b>1=Enable</b>
  UINT32  ExtendedBankHashing:1;     ///<           Bit 7  - Enable EBH Extended Bank Hashing: 0=Disabled; <b>1 = Enabled</b>.
  UINT32  SharedZqPin:1;             ///<           Bit 8  - Select if the ZQ resistor is shared between Ranks in LPDDR4 DRAM Packages <b>0=Not Shared</b>, 1=Shared
  UINT32  UserThresholdEnable:1;     ///<           Bit 9  - Flag to manually select the DIMM CLTM Thermal Threshold, 0=Disable,  1=Enable, <b>0=Default</b>
  UINT32  RmtPerTask:1;              ///<           Bit 10 - Rank Margin Tool Per Task. <b>0 = Disabled</b>, 1 = Enabled
  UINT32  TrainTrace:1;              ///<           Bit 11 - Trained state tracing debug. <b>0 = Disabled</b>, 1 = Enabled
  UINT32  UserBudgetEnable:1;        ///<           Bit 12 - Flag to manually select the Budget Registers for CLTM Memory Dimms , 0=Disable,  1=Enable, <b>0=Default</b>
  UINT32  MsHashEnable:1;            ///<           Bit 13 - Controller Hash Enable: 0=Disable, <b>1=Enable</b>
  UINT32  DisPgCloseIdleTimeout:1;   ///<           Bit 14 - Disable Page Close Idle Timeout: 0=Enable, <b>1=Disable</b>
  UINT32  Ibecc:1;                   ///<           Bit 15 - Inband ECC - <b>0=Disable</b>, 1=Enable
  UINT32  IbeccParity:1;             ///<           Bit 16 - Inband ECC Parity Control - <b>0=Disable</b>, 1=Enable
  UINT32  IbeccOperationMode:2;      ///<           Bits 17:18 - Inband ECC Operation Mode: 0=Functional Mode protects requests based on the address range, <b>1=Makes all requests non protected and ignore range checks</b>, 2=Makes all requests protected and ignore range checks
  UINT32  ChHashOverride:1;          ///<           Bit 19 - Select if Channel Hash setting values will be taken from input parameters or automatically taken from POR values depending on DRAM type detected.
  UINT32  SimicsMinDbgMsg:1;         ///<           Bit 20 - Option to disable most MRC debug messages. Only used when SimicsFlag = 1. Options: <b>0=Disable</b>, 1=Enable
  UINT32  RetrainOnFastFail:1;       ///<           Bit 21 - Restart MRC in Cold mode if SW MemTest fails during Fast flow. 0 = Disabled, <b>1 = Enabled</b>
  UINT32  DvfsqEnabled:1;            ///<           Bit 22 - DVFSQ feature (LP5): 0=Disable, <b>1=Enable</b>
  UINT32  DvfscEnabled:1;            ///<           Bit 23 - E-DVFSC feature (LP5): 0=Disable, <b>1=Enable</b>
  UINT32  VoltageReadout:1;          ///<           Bit 24 - Display VCCClk and PBias voltage readings across partitions: 0=Disable, <b>1: Enable</b>
  UINT32  CccHalfFrequency:1;        ///<           Bit 25 - CCC Half Frequency: <b>0=Disabled (Full Frequency)</b>; 1=Enabled (Half Frequency)
  UINT32  Ddr5AutoPrechargeEnable:2; ///<           Bit 26:27 - Auto Precharge Enable for DDR5: <b>O=Auto</b>, 1=Disable, 2=Enable
  UINT32  Lp5SplitACTEnable:2;       ///<           Bit 28:29 - SplitACT enable for LP5: <b>0=Auto</b>, 1=Disable, 2=Enable
  UINT32  RetrainToWorkingChannel:1; ///<           Bit 30 - Disable Failing Channel during Cold Boot and Retrain To Working Channel: <b>0=Disable</b>, 1=Enable
  UINT32  SubChHashOverride:1;       ///<           Bit 31 - Select if SubChannel Hash setting values will be taken from input parameters or automatically taken from POR values depending on DRAM type detected.
  UINT8   DisableChannel[MAX_CONTROLLER][MAX_CHANNEL]; ///< Offset 36-43 Disables a channel
  UINT16  Ratio;                  ///< Offset 44 DDR Frequency ratio, to multiply by 33 MHz <b>0 = Auto</b>
  /**
    Channel Hash Enable.\n
    NOTE: BIT7 will interleave the channels at a 2 cache-line granularity, BIT8 at 4 and BIT9 at 8\n
    0=BIT6, <B>1=BIT7</B>, 2=BIT8, 3=BIT9
  **/
  UINT8   ChHashInterleaveBit;    ///< Offset 46 Option to select interleave Address bit. Valid values are 0 - 3 for BITS 6 - 9 (Valid values for BDW are 0-7 for BITS 6 - 13)
  UINT8   SmramMask;              ///< Offset 47 Reserved memory ranges for SMRAM

  // Training Algorithms
  TrainingStepsEn  TrainingEnables;   ///< Offset 48 Options to Enable individual training steps
  TrainingStepsEn2 TrainingEnables2;  ///< Offset 52 Options to Enable individual training steps
  TrainingStepsEn3 TrainingEnables3;  ///< Offset 56 Options to Enable individual training steps

  UINT32  MrcTimeMeasure:1;                   ///< Offset 60  Bit 0  - Enables serial debug level to display the MRC execution times only: <b>0=Disable</b>, 1=Enable
  UINT32  MrcFastBoot:1;                      ///<            Bit 1  - Enables the MRC fast boot path for faster cold boot execution: 0=Disable, <b>1=Enable</b>
  UINT32  DqPinsInterleaved:1;                ///<            Bit 2  - Interleaving mode of DQ/DQS pins which depends on board routing: <b>0=Disable</b>, 1=Enable
  UINT32  RankInterleave:1;                   ///<            Bit 3  - Rank Interleave Mode: 0=Disable, <b>1=Enable</b>
  UINT32  EnhancedInterleave:1;               ///<            Bit 4  - Enhanced Interleave Mode: 0=Disable, <b>1=Enable</b>
  UINT32  MsHashOverride:1;                   ///<            Bit 5  - Memory Slice (Controller) Hash Mask and LSB Override. <b>0 = Use default memory slice hash mask / lsb</b>, 1 = Use values from MsHashMask and MsHashInterleaveBit
  UINT32  ChHashEnable:1;                     ///<            Bit 6  - Channel Hash Enable: 0=Disable, <b>1=Enable</b>
  UINT32  EnablePwrDn:1;                      ///<            Bit 7  - Enable Power Down control for DDR: 0=PCODE control, <b>1=BIOS control</b>
  UINT32  EnablePwrDnLpddr:1;                 ///<            Bit 8  - Enable Power Down for LPDDR: 0=PCODE control, <b>1=BIOS control</b>
  UINT32  SrefCfgEna:1;                       ///<            Bit 9  - Enable Self Refresh: 0=Disable, <b>1=Enable</b>
  UINT32  ThrtCkeMinDefeatLpddr:1;            ///<            Bit 10 - Throttler CKE min defeature for LPDDR: 0=Disable, <b>1=Enable</b>
  UINT32  ThrtCkeMinDefeat:1;                 ///<            Bit 11 - Throttler CKE min defeature: <b>0=Disable</b>, 1=Enable
  UINT32  AutoSelfRefreshSupport:1;           ///<            Bit 12 - FALSE = No auto self refresh support, <b>TRUE = auto self refresh support</b>
  UINT32  ExtTemperatureSupport:1;            ///<            Bit 13 - FALSE = No extended temperature support, <b>TRUE = extended temperature support</b>
  UINT32  AllowOppRefBelowWriteThrehold:1;    ///<            Bit 14 - Option to allow opportunistic refreshes while we don't exit power down.
  UINT32  Force1Dpc:1;                        ///<            Bit 15 - TRUE means force one DIMM per channel, <b>FALSE means no limit</b>
  UINT32  ForceSingleRank:1;                  ///<            Bit 16 - TRUE means use Rank0 only (in each DIMM): <b>0=Disable</b>, 1=Enable
  UINT32  DynamicMemoryBoost:1;               ///<            Bit 17 - Dynamic Memory Boost: 0 - Disabled, 1 - Enabled. Only valid if SpdProfileSelected is an XMP Profile; otherwise ignored.
  UINT32  RealtimeMemoryFrequency:1;          ///<            Bit 18 - Realtime Memory Frequency: <b>0=Disabled</b>, 1=Enabled. Only valid if SpdProfileSelected is XMP Profile 1; otherwise ignored.
  UINT32  ExitOnFailure:1;                    ///<            Bit 19 - MRC option for exit on failure or continue on failure: 0=Disable, <b>1=Enable</b>
  UINT32  ForceRetrainPath:1;                 ///<            Bit 20 - Force retrain path in RetrainMarginCheck even if margins are good enough
  UINT32  SaGv:1;                             ///<            Bit 21 - SaGv Status: 0 - Disabled, 1 - Enabled
  UINT32  IsDdr5MR7WicaSupported:1;           ///<            Bit 22 - TRUE if DDR5 DRAM Device supports MR7 WICA 0.5 tCK offset alignment <b>0: Disable</b>, 1: Enable
  UINT32  VoltageCurrentSensor:1;             ///<            Bit 23 - Voltage and Current Sensor (deprecated)
  UINT32  DataInvertNibble:1;                 ///<            Bit 24 - DataInvertNibble: 0: Disable, <b>1: Enable</b>
  UINT32  CccPinsInterleaved:1;               ///<            Bit 25 - Interleaving mode of CCC pins which depends on board routing: <b>0=Disable</b>, 1=Enable
  UINT32  EnPeriodicComp:1;                   ///<            Bit 26 - Enable/disable Periodic Compensation
  UINT32  AsyncOdtDis:1;                      ///<            Bit 27 - Option to Enable Asynchronous ODT
  UINT32  OpportunisticRead:1;                ///<            Bit 28 - Option to Enable Opportunistic Read in Write Major Mode.
  UINT32  Disable2CycleBypass:1;              ///<            Bit 29 - Option to disable 2 cycle bypass
  UINT32  OCSafeMode:1;                       ///<            Bit 30 - OverClocking Safe Mode
  UINT32  HsleFlag:1;                         ///<            Bit 31 - Option to Enable HSLE: <b>0=Disable</b>, 1=Enable
  UINT16  SrefCfgIdleTmr;         ///< Offset 64 Self Refresh idle timer: <b>512=Minimal</b>, 65535=Maximum
  UINT16  ChHashMask;             ///< Offset 66 Channel Hash Mask: 0x0001=BIT6 set(Minimal), 0x3FFF=BIT[19:6] set(Maximum), <b>0x30CE= BIT[19:18, 13:12 ,9:7] set</b>
  UINT16  DdrFreqLimit;           ///< Offset 68 Memory Frequency limit: <b>0 = Auto</b>, or memory speed value in MT/s: 1067, 1333, 1600 etc. See the possible values in CMrcInterface.h
  UINT8   ThrtCkeMinTmr;          ///< Offset 70 Throttler CKE min timer: 0=Minimal, 0xFF=Maximum, <b>0x00=Default</b>
  UINT8   ThrtCkeMinTmrLpddr;     ///< Offset 71 Throttler CKE min timer for LPDDR: 0=Minimal, 0xFF=Maximum, <b>0x00=Default</b>
  UINT8   SaGvWpMask;             ///< Offset 72 SA GV: Bitmask of enabled SaGv Points 0x3:Points0_1, 0x7:Points0_1_2, <b>0xF:AllPoints0_1_2_3</b>
  UINT8   NModeSupport;           ///< Offset 73 Memory N Mode Support - Enable user to select Auto, 1N or 2N: <b>0=AUTO</b>, 1=1N, 2=2N.
  UINT8   McRefreshRate;          ///< Offset 74 Refresh Rate: Type of solution to be used for RHP - 0/1/2/3 = RefreshNORMAL/Refresh1x/Refresh2x/Refresh4x
  UINT8   PowerDownMode;          ///< Offset 75 CKE Power Down Mode: <b>0xFF=AUTO</b>, 0=No Power Down, 1= APD mode, 6=PPD-DLL Off mode
  UINT8   PwdwnIdleCounter;       ///< Offset 76 CKE Power Down Mode Idle Counter: 0=Minimal, 255=Maximum, <b>0x80=0x80 DCLK</b>
  UINT8   CmdRanksTerminated;     ///< Offset 77 LPDDR: Bitmask of ranks that have CA bus terminated. <b>0x01=Default, Rank0 is terminating and Rank1 is non-terminating</b>
  UINT16  MsHashMask;             ///< Offset 78 Memory Slice (Controller) Hash Mask: 0x0001=BIT6 set(Minimal), 0x3FFF=BIT[19:6] set(Maximum); used when MsHashOverride is set
  UINT32  Lp5CccConfig;           ///< Offset 80 BitMask where bits [3:0] are controller 0 Channel [3:0] and [7:4] are Controller 1 Channel [3:0].  0 selects Ascending mapping and 1 selects Descending mapping.
  UINT8   RMTLoopCount;           ///< Offset 84 Indicates the Loop Count to be used for Rank Margin Tool Testing: 1=Minimal, 32=Maximum, 0=AUTO, <b>0=Default</b>
  UINT8   MsHashInterleaveBit;    ///< Offset 85 Memory Slice (Controller) Hash LSB bit. Valid values are 0..7 for BITS 6..13; used when MsHashOverride is set
  UINT8   GearRatio;              ///< Offset 86 Gear Ratio when SAGV is disabled: <b>0=Auto</b>, 1=rsvd, 2=Gear2, 3=rsvd, 4=Gear4.
  UINT8   DdrOneDpc;              ///< Offset 87 DDR5 1DPC performance feature: 0 - Disabled; 1 - Enabled on DIMM0 only, 2 - Enabled on DIMM1 only; 3 - Enabled on both DIMMs. (bit [0] - DIMM0, bit [1] - DIMM1)
  UINT16  VddqVoltageOverride;    ///< Offset 88 VccddqVoltage override in # of 1mV, Auto = 0
  UINT16  VccIogVoltageOverride;  ///< Offset 90 VccIogVoltage override in # of 1mV, Auto = 0
  UINT16  VccClkVoltageOverride;  ///< Offset 92 VccClkVoltage override in # of 1mV, Auto = 0
  UINT8   LpddrRttWr;             ///< Offset 94 Initial RttWr for LP5 in Ohms, 0 means Auto
  UINT8   LpddrRttCa;             ///< Offset 95 Initial RttCa for LP5 in Ohms, 0 means Auto
  UINT16  SaGvFreq[MAX_SAGV_POINTS];    ///< Offset 96 Frequency per SAGV point.  0 is Auto, otherwise holds the frequency value expressed as an integer: <b>0=Default</b>, 1067, 1333, 1600, 1800, 1867, etc.
  /**
    Offset 104 Gear ratio per SAGV point.  0 is Auto, otherwise holds the Gear ratio expressed as an integer: <b>0=Default</b>, 1=rsvd, 2=Gear2, 3=rsvd, 4=Gear4.
      Only valid combinations of Gear Ratio per point is:
      | point | set1 | set2 | set3
      | 0     | 1    | 2    | 2
      | 1     | 1    | 2    | 2
      | 2     | 1    | 2    | 2
      | 3     | 1    | 2    | 1
  **/
  UINT8   SaGvGear[MAX_SAGV_POINTS];                      ///< Offset 104
  UINT8   IbeccProtectedRegionEnable[MAX_IBECC_REGIONS];  ///< Offset 108 Enable use of address range for ECC Protection:  <b>0=Default</b>, 1 = Enabled
  UINT16  IbeccProtectedRegionBase[MAX_IBECC_REGIONS];    ///< Offset 116 Base address for address range of ECC Protection:  [0..0x3FFF]
  UINT16  IbeccProtectedRegionMask[MAX_IBECC_REGIONS];    ///< Offset 132 Mask address for address range of ECC Protection:  [1..0x3FFF]
  UINT16  MarginLimitL2;          ///< Offset 148 Margin limit check L2 threshold: <b>100=Default</b>
  UINT8   WriteThreshold;         ///< Offset 150 Option to set number of writes that can be accumulated while CKE is low before CKE is asserted.
  UINT8   MarginLimitCheck;       ///< Offset 151 Margin limit check enable: 0=Disable, <b>1=L1 only</b>, 2=L2 only, 3=Both L1 and L2
  UINT8   Lfsr0Mask;              ///< Offset 152 RH pTRR LFSR0 Mask
  UINT8   Lfsr1Mask;              ///< Offset 153 RH pTRR LFSR1 Mask
  UINT8   RefreshWm;              ///< Offset 154 Refresh Watermarks, 0 = Low, <b>1 = High</b>
  UINT8   DramRfmMode;            ///< Offset 155 Row Hammer Dram Refresh Management Mode runs orthogonally with DRFM/pTRR: <b>0=RFM</b>, 1=ARFM Level A, 2=ARFM Level B, 3=ARFM Level C, 4=Disable RFM and ARFM
  UINT8   TargetedRowRefreshMode; ///< Offset 156 Row Hammer Targeted Row Refresh Mode runs orthogonally with ARFM/RFM: <b>0=DRFM (w/ fallback to pTRR)</b>, 1=pTRR, 2=Disable DRFM and pTRR
  UINT8   DrfmBrc;                ///< Offset 157 Row Hammer DRFM Blast Radius Configuration determines number of victim rows around aggressor row targeted to send the DRFM sequence to: <b>0=BlastRadius 2</b>, 1=BlastRadius 3, 2=BlastRadius 4
  UINT8   CmdMirror;              ///< Offset 158 BitMask where bits [3:0] are controller 0 Channel [3:0] and [7:4] are Controller 1 Channel [3:0].  0 = No Command Mirror and 1 = Command Mirror.
  UINT8   SafeModeOverride;       ///< Offset 159
  UINT32   DdrSafeMode;           ///< Offset 160
  UINT8   McSafeMode;             ///< Offset 164
  UINT8   LpMode;                 ///< Offset 165 Bit mask: Bit[0] - Enable Lpmode0p5 (Idle_enable); Bit[1] - Enable Lpmode2 (Powerdown_enable); Bit[2] - Enable Lpmode3 (Selfrefresh_enable); Default = 7 ('111')
  UINT8   LpMode4;                ///< Offset 166 LpMode4: 0=Disabled; <b>1=Enabled</b>; 2=Dynamic with threshold 2; 3=Dynamic with threshold=3
  UINT8   PprTestType;            ///< Offset 167 Deprecated
  UINT16  tRFCpb;                 ///< Offset 168 User defined Memory Timing tRFCpb value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>
  UINT16  tRFC2;                  ///< Offset 170 User defined Memory Timing tRFC2 value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>
  UINT16  tRFC4;                  ///< Offset 172 User defined Memory Timing tRFC4 value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>
  UINT16  tRRD_L;                 ///< Offset 174 User defined Memory Timing tRRD_L value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>
  UINT16  tRRD_S;                 ///< Offset 176 User defined Memory Timing tRRD_S value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>
  UINT16  tWTR_L;                 ///< Offset 178 User defined Memory Timing tWTR_L value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>
  UINT16  tCCD_L;                 ///< Offset 180 User defined Memory Timing tCCD_L value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>
  UINT16  tWTR_S;                 ///< Offset 182 User defined Memory Timing tWTR_S value,  valid when SpdProfileSelected is CUSTOM_PROFILE: <b>0=AUTO</b>
  UINT16  DimmOdt[EXT_MAX_DIMMS_IN_CHANNEL][MAX_DIMMODT_ENTRY];   ///< Offset 184 Dimm Odt values: <b>0=AUTO</b>, RttWr, RttNomRd, RttNomWr, RttPark, RttParkDqs, RttCa GroupA, RttCs GroupA, RttCk GroupA, RttCa GroupB, RttCs GroupB, RttCk GroupB
  UINT8   DFETap1StepSize;        ///< Offset 228 Dimm DFE Tap1 Step Size
  UINT8   DFETap2StepSize;        ///< Offset 229 Dimm DFE Tap1 Step Size
  UINT8   CsVrefLow;              ///< Offset 230 DDR5 Cs Sweep Low Vref Value
  UINT8   CsVrefHigh;             ///< Offset 231 DDR5 Cs Sweep High Vref Value
  UINT8   CaVrefLow;              ///< Offset 232 DDR5 Ca Sweep Low Vref Value
  UINT8   CaVrefHigh;             ///< Offset 233 DDR5 Ca Sweep High Vref Value
  UINT8   EccCorrectionMode;      ///< Offset 234 0 (Default) = SECDED (Single error correction, double error detection), 1 = ZECTED (Zero error correct triple error detect)
  UINT8   EccGranularity32BEn;    ///< Offset 235 EccGranularity: 0 = 64B(Default), 1 = 32B
  McRegOffsets    OffsetKnobs;    ///< Offset 236 - 247 Deprecated
  UINT8   LowerBasicMemTestSize;  ///< Offset 248 LowerBasicMemTestSize
  UINT8   MCREGOFFSET;            ///< Offset 249 MC Register offset
  UINT8   PcuDdrVoltage;          ///< Offset 250 Override for PCU_CR_DDR_VOLTAGE.

  // TurnAround Timing Delta
  // 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]
  UINT8   Trrsg;                  ///< Offset 251 tRRSG delta
  UINT8   Trrdg;                  ///< Offset 252 tRRDG delta
  UINT8   Trrdr;                  ///< Offset 253 tRRDR delta
  UINT8   Trrdd;                  ///< Offset 254 tRRDD delta
  UINT8   Twrsg;                  ///< Offset 255 tWRSG delta
  UINT8   Twrdg;                  ///< Offset 256 tWRDG delta
  UINT8   Twrdr;                  ///< Offset 257 tWRDR delta
  UINT8   Twrdd;                  ///< Offset 258 tWRDD delta
  UINT8   Twwsg;                  ///< Offset 259 tWWSG delta
  UINT8   Twwdg;                  ///< Offset 260 tWWDG delta
  UINT8   Twwdr;                  ///< Offset 261 tWWDR delta
  UINT8   Twwdd;                  ///< Offset 262 tWWDD delta
  UINT8   Trwsg;                  ///< Offset 263 tRWSG delta
  UINT8   Trwdg;                  ///< Offset 264 tRWDG delta
  UINT8   Trwdr;                  ///< Offset 265 tRWDR delta
  UINT8   Trwdd;                  ///< Offset 266 tRWDD delta

  UINT8   Interpreter;            ///< Offset 267 0 = CMOS location detection, 1 = Ctrl-break detection, 2 = Force into interpreter.
  UINT8   IoOdtMode;              ///< Offset 268 This defines the ODT mode the DDRIO is set to: MRC_ODT_MODE_TYPE.
  INT8    DynamicRangeLow;        ///< Offset 269 Low end of the CPU dynamic temperature range, in degrees C (2's complement). 0 - Auto
  UINT16  VddqVoltage;            ///< Offset 270 DRAM voltage (Vddq) in millivolts: <b>0=Platform Default (no override)</b>, 1200=1.2V, 1350=1.35V etc
  UINT16  VppVoltage;             ///< Offset 272 DRAM voltage (Vpp) in millivolts: <b>0=Platform Default (no override)</b>, 1800=1.8V, 2050=2.05V etc

  UINT16  RcompResistor;          ///< Offset 274 Reference RCOMP resistor on motherboard ~ 100 ohms
  UINT16  RcompTarget[MAX_RCOMP_TARGETS]; ///< Offset 276 RCOMP target values for DqOdt, DqDrv, CmdDrv, CtlDrv, ClkDrv
  UINT16  tCCD_L_WR;                ///< Offset 286 User defined Memory Timing tCCD_L_WR value, valid when SpdProfileSelected is CUSTOM_PROFILE
  UINT8   PprRepairType;            ///< Offset 288 Deprecated
  UINT8   PprRunOnce;               ///< Offset 289 Deprecated
  UINT8   PprRunAtFastboot;         ///< Offset 290 Deprecated
  UINT8   PprErrorInjection;        ///< Offset 291 Deprecated
  UINT32  PprRepairPhysicalAddrLow; ///< Offset 292 Deprecated
  UINT32  PprRepairPhysicalAddrHigh;///< Offset 296 Deprecated
  UINT32  PprRepairRow;             ///< Offset 300 Deprecated
  UINT8   PprRepairController;      ///< Offset 304 Deprecated
  UINT8   PprRepairChannel;         ///< Offset 305 Deprecated
  UINT8   PprRepairDimm;            ///< Offset 306 Deprecated
  UINT8   PprRepairRank;            ///< Offset 307 Deprecated
  UINT8   PprRepairBankGroup;       ///< Offset 308 Deprecated
  UINT8   PprRepairBank;            ///< Offset 309 Deprecated
  UINT8   PprForceRepair;           ///< Offset 310 Deprecated
  UINT8   RunRefPiMaxVoltage;       ///< Offset 311 Run Ref PI Calibration for both min and max Voltage
  MrcBoardInputs BoardDetails;      ///< Offset 312 The details of the board that help MRC determine top memory speed
  UINT16  SubChHashMask;            ///< Offset 316 Channel Hash Mask: 0x0001=BIT6 set(Minimal), 0x3FFF=BIT[19:6] set(Maximum), <b>0x30CE= BIT[19:18, 13:12 ,9:7] set</b>
  UINT8   SubChHashInterleaveBit;   ///< Offset 318 SubCh Option to select interleave Address bit. Valid values are 0 - 3 for BITS 6 - 9 (Valid values for BDW are 0-7 for BITS 6 - 13)
  UINT8   ForceCkdBypass;           ///< Offset 319 Force CKD in Bypass Mode if CKD DIMM is detected: 0 = Single PLL mode (Default), 1 = Bypass mode.

  UINT32  DprLock                   : 2;  ///< Offset 320 Bit 0-1    Lock DPR register. <b>0: Platform POR </b>; 1: Enable; 2: Disable
  UINT32  AllocateSharedMailbox     : 1;  ///<            Bit 2      Allocate shared mailbox for SV module <b>0: Disable </b>; 1: Enable
  UINT32  MimicWcDisaplayInIpq      : 4;  ///<            Bit 3-6    Using for Disable/Enable Mimic WC display pattern in IPQ: <b>0: Disable</b>; <b>1: Enable 1 ACT resources usage</b>; <b>3: Enable 2 ACT resources usage</b>; <b>7: Enable 3 ACT resources usage</b>; <b>0xf: Enable 4 ACT resources usage</b>;
  UINT32  FakeSagv                  : 1;  ///<            Bit 7      Fake SAGV <b>0: Disable</b>; 1: Enable
  UINT32  DisableZq                 : 1;  ///<            Bit 8      Disable Zq Calibration: <b>0: Enable</b>, 1: Disable
  UINT32  ReplicateSagv             : 1;  ///<            Bit 9      Replicate SAGV point0 for 4 points <b>0: Disable</b>; 1: Enable
  UINT32  AdjustWckMode             : 2;  ///<            Bit 10-11  Adjust wck mode: 0 - safe mode; 1 - manual mode; 2 - dynamic mode; 3 - Default
  UINT32  TelemetryControl          : 2;  ///<            Bit 12-13  Control MC/PMA telemetry : 0 - Default, 1 - Enable Telemetry, 2 - Disable Telemetry
  UINT32  SpineAndPhclkGateControl  : 1;  ///<            Bit 14     Enable/Disable PHclk\Qclk SPINE gating Control: 0 - Disable; 1 - Enable
  UINT32  SpineGatePerLpmode        : 4;  ///<            Bit 15-18  SpineGating control per lpmode
                                          ///<                       SpineGatePerLpmode[0] - SpineGating control at Lpmode0.5
                                          ///<                       SpineGatePerLpmode[1]   SpineGating control at Lpmode2
                                          ///<                       SpineGatePerLpmode[2]   SpineGating control at Lpmode3
                                          ///<                       SpineGatePerLpmode[3]   SpineGating control at Lpmode4
  UINT32  PhclkGatePerLpmode        : 5;  ///<            Bit 19-23  PhClkGating control per lpmode
                                          ///<                       PhclkGatePerLpmode[0]  PhClkGating control at lpmode0.5
                                          ///<                       PhclkGatePerLpmode[1]  PhClkGating control at lpmode1
                                          ///<                       PhclkGatePerLpmode[2]  PhClkGating control at lpmode2
                                          ///<                       PhclkGatePerLpmode[3]  PhClkGating control at lpmode3
                                          ///<                       PhclkGatePerLpmode[4]  PhClkGating control at lpmode4
  UINT32  DisableSwitchDfiToMc      : 1;  ///<            Bit 24     keep the control of the DFI with the PHY/MPTU or switching it to MC (after cold boot) : <b>1=Enable, 0=Disable(Default)
  UINT32  DqLoopbackTest            : 1;  ///<            Bit 25     Control Dq Loopback test: 0: Disable, 1: Enable
  UINT32  DunitTatOptimization      : 1;  ///<            Bit 26     Optimize Dunit TAT Timings
  UINT32  Lp5SafeSpeed              : 1;  ///<            Bit 27     LP5 safe speed: 0 = Disabled, 1 = Enabled
  UINT32  ForceInternalClkOn        : 1;  ///<            Bit 28     Force InternalClocksOn and TxPiOn to be set to 1 for frequencies >= 7467. <b>0: Disable</b>, 1: Enable
  UINT32  SvReservedBits            : 3;  ///<            Bit 29-31  Reserved

  UINT8   RowPressEn;                     ///< Offset 324 Enable/disable ROW PRESS feature
  UINT8   WeaklockEn;                     ///< Offset 325 Weak Lock Enable: <b>0=Auto</b>, 1=Enable, 2=Disable
  UINT8   RxDqsDelayCompEn;               ///< Offset 326 Rx DQS Delay Comp Enable: <b>0=Auto</b>, 1=Enable, 2=Disable

  UINT8  RxDqsStepSizeLB            : 2;  ///< Offset 327 Bit 0-1  Control RxDqs step size for Dq Loopback test (actual value = set value * 2)
  UINT8  RxVrefStepSizeLB           : 2;  ///<            Bit 2-3  Control RxVref step size for the Dq Loopback test (actual value = set value * 2)
  UINT8  TxStepSizeLB               : 2;  ///<            Bit 4-5  Control Tx step size for Dq Loopback test (actual value = set value * 2)
  UINT8  ReservedBitsLB             : 2;  ///<            Bit 5-7  Reserved

  UINT8   MrcFailureOnUnsupportedDimm;    ///< Offset 328 Enable/disable MRC failure policy for unsupported memory type.
  UINT8   WckModeOverride;                ///< Offset 329 Overrides the WckMode currently set: 0 = WckModeAutoCtrl, 1 = WckModeAlwaysOn, 2 = WckModeNoOverride (don't override)
  UINT8   Use1p5ReadPostamble;            ///< Offset 330 Enable using 1p5 tCK Read Postamble for higher freqencies.

  UINT8   IbeccEccInjControl;             ///< Offset 331 In-band ECC: Error Injection Control 0: No Error Injection, 1:Inject Correctable Error Address match, 3:Inject Correctable Error on insertion counter, 5: Inject Uncorrectable Error Address match, 7:Inject Uncorrectable Error on insertion counter
  UINT32  IbeccEccInjAddrBase;            ///< Offset 332 Address to match against for ECC error injection, in units of 32MB (Eg. 1 = 32MB, 2 = 64MB, 3 = 96MB)
  UINT8   IbeccEccInjCount;               ///< Offset 336 Number of memory transactions between ECC error injection

  UINT8   IsWckIdleExitEnabled;           ///< Offset 337 Enable/disable WCK Idle Exit: 0 = Disable, 1 = Enable
  UINT16  Vdd2HVoltage;                   ///< Offset 338-339 LP5 DRAM voltage (VDD2H) in millivolts: <b>0=Platform Default (no override)</b>, 1200=1.2V, 1350=1.35V etc.
  UINT16  Vdd1Voltage;                    ///< Offset 340-341 LP5 DRAM voltage (VDD1) in millivolts: <b>0=Platform Default (no override)</b>, 1200=1.2V, 1350=1.35V etc.
  UINT16  Vdd2LVoltage;                   ///< Offset 342-343 LP5 DRAM voltage (VDD2L) in millivolts: <b>0=Platform Default (no override)</b>, 1200=1.2V, 1350=1.35V etc.
  INT8    PhyDtsTemperature;              ///< Offset 344 PHY DTS temperature override, in degrees C (2's complement). 0xFF - platform default (no override).
  BOOLEAN FlexibleAnalogSettings;         ///< Offset 345 Enable/disable usage of flexible analog settings (NN flex feature): 0 = Disable, 1 = Enable
  BOOLEAN PhClkCheck;                     ///< Offset 346 Phase Clock Check.
  BOOLEAN ForceWRDSEQT2400;               ///< Offset 347 Force Enable Write Drive Strength training at 2400: 0 = Disable, 1 = Enable

  UINT32   NnFlexPhyRxEqTap0      : 6;    ///< Offset 348 Bits 0-5   PhyRxEqTap0  - 6 bit 2's complement
  UINT32   NnFlexPhyRxEqTap1      : 6;    ///<            Bits 6-11  PhyRxEqTap1  - 6 bit 2's complement, valid range: [-16..15]
  UINT32   NnFlexPhyDqTcoComp     : 6;    ///<            Bits 12-17 PhyDqTcoComp - 6 bit 2's complement
  UINT32   NnFlexPhyRxCtleR       : 2;    ///<            Bits 18-19 PhyRxCtleR       - [0..3]
  UINT32   NnFlexPhyRxCtleC       : 2;    ///<            Bits 20-21 PhyRxCtleC       - [0..3]
  UINT32   NnFlexPhyRxCtleRcmn    : 2;    ///<            Bits 22-23 PhyRxCtleRcmn    - [0..3]
  UINT32   NnFlexPhyRxCtleEq      : 3;    ///<            Bits 24-26 PhyRxCtleEq      - [0..7]
  UINT32   NnFlexPhyRxCtleTailCtl : 2;    ///<            Bits 27-28 PhyRxCtleTailCtl - [0..3]
  UINT32   NnFlexPhyReserved      : 3;    ///<            Bits 29-31 Reserved

  UINT32   NnFlexLpddr5Dfeq       : 3;    ///< Offset 352 Bits 0-2   Lpddr5Dfeq     - [0..7], MR24 encoding
  UINT32   NnFlexLpddr5PdDrvStr   : 3;    ///<            Bits 3-5   Lpddr5PdDrvStr - [0..6], MR3 encoding
  UINT32   NnFlexLpddr5SocOdt     : 3;    ///<            Bits 6-8   Lpddr5SocOdt   - [0..6], MR17 encoding
  UINT32   NnFlexLpddr5PreEmpDn   : 2;    ///<            Bits 9-10  Lpddr5PreEmpDn - [0..3], MR58 encoding
  UINT32   NnFlexLpddr5PreEmpUp   : 2;    ///<            Bits 11-12 Lpddr5PreEmpUp - [0..3], MR58 encoding
  UINT32   NnFlexLpddr5WckDcaWr   : 4;    ///<            Bits 13-16 Lpddr5WckDcaWr - 4-bit 2's complement, valid range: [-7..7]
  UINT32   NnFlexLpddr5WckDcaRd   : 4;    ///<            Bits 17-20 Lpddr5WckDcaRd - 4-bit 2's complement, valid range: [-7..7]
  UINT32   NnFlexLpddr5RttNT      : 3;    ///<            Bits 21-23 Lpddr5RttNT    - [0..7], MR41 encoding
  UINT32   NnFlexDramOvrdMask     : 8;    ///<            Bits 24-31 Bit mask to enable the use of NnFlexDdr5/Lpddr5 overrides. Use NnFlexMaskLpddr5XXX / NnFlexMaskDdr5XXX for indexing.

  UINT32   NnFlexDdr5DfeTap1      : 8;    ///< Offset 356 Bits 0-7   Ddr5DfeTap1  - 8-bit 2's complement, valid range: [-40..40]
  UINT32   NnFlexDdr5DfeTap2      : 8;    ///<            Bits 8-15  Ddr5DfeTap2  - 8-bit 2's complement, valid range: [-15..15]
  UINT32   NnFlexDdr5RttWr        : 3;    ///<            Bits 16-18 Ddr5RttWr    - [0..7], MR34 encoding
  UINT32   NnFlexDdr5RttNomWr     : 3;    ///<            Bits 19-21 Ddr5RttNomWr - [0..7], MR35 encoding
  UINT32   NnFlexDdr5RttNomRd     : 3;    ///<            Bits 22-24 Ddr5RttNomRd - [0..7], MR35 encoding
  UINT32   NnFlexDdr5RonUp        : 2;    ///<            Bits 25-26 Ddr5RonUp    - [0..2], MR5 encoding
  UINT32   NnFlexDdr5RonDn        : 2;    ///<            Bits 27-28 Ddr5RonDn    - [0..2], MR5 encoding
  UINT32   NnFlexDdr5Reserved     : 3;    ///<            Bits 29-31 Reserved

  UINT8    NnFlexPhyOvrdMask;             ///< Offset 360 Bitmask to enable NnFlexPhyXXX overrides above (offset 348)
  UINT8    NnFlexCmdOvrdMask;             ///< Offset 361 Bitmask to enable the use of NnFlexCmd: CmdDrvVrefUp, CtlDrvVrefUp, CmdCaTxEq, CtlDrvVrefDn
  UINT8    NnFlexCmdDrvVrefUp;            ///< Offset 362 NnFlex CmdDrvVrefUp (0-191)
  UINT8    NnFlexCtlDrvVrefUp;            ///< Offset 363 NnFlex CtlDrvVrefUp (0-191)
  UINT8    NnFlexCmdCaTxEq;               ///< Offset 364 NnFlex CmdCaTxEq (0-31)
  UINT8    NnFlexCtlDrvVrefDn;            ///< Offset 365 NnFlex CtlDrvVrefDn (0-191)

  UINT8   Reserved366[22];                ///< Offset 366 Reserved for future use. Total size of CONFIG_BLOCK_HEADER (28 bytes) + MRC_EXT_INPUTS_TYPE (366 + 22 bytes) = 416 should be a multiple of 8 bytes.
} MRC_EXT_INPUTS_TYPE;

#pragma pack(pop)
#endif // MRC_EXT_INPUTS_H_
