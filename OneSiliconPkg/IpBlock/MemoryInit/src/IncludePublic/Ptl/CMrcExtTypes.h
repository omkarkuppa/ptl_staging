/** @file
  This file contains the MRC types that can be exposed to external customer.

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
#ifndef MRC_EXT_TYPES_H_
#define MRC_EXT_TYPES_H_

/// SA GV points
typedef enum {
  MrcSaGvPoint0,
  MrcSaGvPoint1,
  MrcSaGvPoint2,
  MrcSaGvPoint3,
  MrcSaGvPointMax
} MrcSaGvPoint;

#define EXT_MAX_DIMMS_IN_CHANNEL  (2)       ///< The maximum number of DIMMs per channel for external MRC input block
#define MAX_MPTU                  (2)       ///< The maximum number of MPTU blocks in PHY
#define MAX_CONTROLLER            (2)       ///< The maximum number of memory controllers per CPU socket.
#define MAX_CHANNEL               (4)       ///< The maximum number of channels per memory controller.
#define MAX_CHANNEL_DDR5          (2)       ///< The maximum number of channels per memory controller in DDR5
#define MAX_IBECC_REGIONS         (8)       ///< Maximum number of protection regions handled by IBECC
#define MAX_SAGV_POINTS           (MrcSaGvPointMax) ///< Defines the number of frequency points supported.
#define MAX_RANK_IN_DIMM          (2)       ///< The maximum number of ranks per DIMM.
// 11 entries are for RttWr, RttNomRd, RttNomWr, RttPark, RttParkDqs, RttCa GroupA, RttCs GroupA, RttCk GroupA, RttCa GroupB, RttCs GroupB, RttCk GroupB
#define MAX_DIMMODT_ENTRY         (11)

#ifndef MAX_RCOMP_TARGETS
#define MAX_RCOMP_TARGETS         (5)
#endif

typedef struct {
  UINT8 CAVrefCtlOffset;
  UINT8 VrefCtlOffset;
  UINT8 ClkPiCodeOffset;
  UINT8 RcvEnOffset;
  UINT8 RxDqsOffset;
  UINT8 TxDqOffset;
  UINT8 TxDqsOffset;
  UINT8 VrefOffset;
  UINT8 CntrlrMask;
  UINT8 ChMask;
  UINT8 RsvdBytes[2]; ///< Added to keep UINT32 alignment in McRegOffsets
} McRegOffsets;

typedef struct {
  UINT32 ECT:1;             ///< BIT0 - Early Command Training
  UINT32 SOT:1;             ///< BIT1 - Sense Amp Offset Training
  UINT32 ERDMPRTC2D:1;      ///< BIT2 - Early ReadMPR Timing Centering 2D
  UINT32 RDMPRT:1;          ///< BIT3 - Read MPR Training
  UINT32 RCVET:1;           ///< BIT4 - Read Leveling Training (RcvEn)
  UINT32 JWRL:1;            ///< BIT5 - Jedec Write Leveling
  UINT32 EWRTC2D:1;         ///< BIT6 - Early Write Time Centering 2D
  UINT32 ERDTC2D:1;         ///< BIT7 - Early Read Time Centering 2D
  UINT32 UNMATCHEDWRTC1D:1; ///< BIT8 - Unmatched Write Timing Centering 1D
  UINT32 WRTC1D:1;          ///< BIT9 - Write Timing Centering 1D
  UINT32 WRVC1D:1;          ///< BIT10 - Write Voltage Centering 1D
  UINT32 RDTC1D:1;          ///< BIT11 - Read Timing Centering Training
  UINT32 RDVC1D:1;          ///< BIT12 - Read Voltage Centering Training
  UINT32 WRTC2D:1;          ///< BIT13 - Write Timing Centering 2D
  UINT32 RDTC2D:1;          ///< BIT14 - Read Timing Centering 2D
  UINT32 WRVC2D:1;          ///< BIT15 - Write Voltage Centering 2D
  UINT32 RDVC2D:1;          ///< BIT16 - Read Voltage Centering 2D
  UINT32 WRDSEQT:1;         ///< BIT17 - Write Drive Strength / Equalization Training 2D
  UINT32 DQSRF:1;           ///< BIT18 - DQS Rise/Fall
  UINT32 RDDQSODTT:1;       ///< BIT19 - Read DQS ODT Training
  UINT32 RDEQT:1;           ///< BIT20 - Read Equalization Training
  UINT32 DUNITC:1;          ///< BIT21 - Dunit Configuration
  UINT32 CMDVC:1;           ///< BIT22 - Command Voltage Centering
  UINT32 LCT:1;             ///< BIT23 - Late Command Training
  UINT32 RTL:1;             ///< BIT24 - Round Trip latency
  UINT32 TAT:1;             ///< BIT25 - Turn Around Timing
  UINT32 RMT:1;             ///< BIT26 - RMT Tool
  UINT32 RMTEVENODD:1;      ///< BIT27 - RMT Even Odd
  UINT32 ALIASCHK:1;        ///< BIT28 - SPD Alias Check
  UINT32 RCVENC1D:1;        ///< BIT29 - Receive Enable Centering 1D
  UINT32 RMC:1;             ///< BIT30 - Retrain Margin Check
  UINT32 PRETRAIN:1;        ///< BIT31 - PreTraining
} TrainingStepsEn;

typedef struct {
  UINT32 DCCPICODELUT:1;         ///< BIT0 - DCC PI Code LUT Calibration
  UINT32 DIMMODTT:1;             ///< BIT1 - DIMM ODT Training
  UINT32 DIMMRONT:1;             ///< BIT2 - DIMM RON Training
  UINT32 TXTCO:1;                ///< BIT3 - Write TCO Comp Training
  UINT32 CLKTCO:1;               ///< BIT4 - Clock TCO Comp Training
  UINT32 CMDSR:1;                ///< BIT5 - CMD Slew Rate Training
  UINT32 CMDDSEQ:1;              ///< BIT6 - CMD Drive Strength and Tx Equalization
  UINT32 DIMMODTCA:1;            ///< BIT7 - Dimm ODT CA Training
  UINT32 DDR5ODTTIMING:1;        ///< BIT8 - DDR5 ODT Timing Config
  UINT32 DBI:1;                  ///< BIT9 - LPDDR DBI Training
  UINT32 DLLDCC:1;               ///< BIT10 - DLL DCC Calibration
  UINT32 DLLBWSEL:1;             ///< BIT11 - DLL BW Select Calibration
  UINT32 RDVREFDC:1;             ///< BIT12 - Read Vref Decap Training
  UINT32 Reserved2Bit13:1;       ///< BIT13 - Reserved
  UINT32 RMTBIT:1;               ///< BIT14 - Rank Margin Tool Per Bit
  UINT32 DQDQSSWZ:1;             ///< BIT15 - Map Dq Dqs Swizzle
  UINT32 REFPI:1;                ///< BIT16 - RefPi Calibration
  UINT32 DCCLP5READDCA:1;        ///< BIT17 - DRAM DCC for LP5 READ DCA
  UINT32 VCCCLKFF:1;             ///< BIT18 - VccCLK FF Offset Correction
  UINT32 FUNCDCCDQS:1;           ///< BIT19 - Functional Duty Cycle Correction for DDR5 DQS
  UINT32 FUNCDCCCLK:1;           ///< BIT20 - Functional Duty Cycle Correction for DDR5 CLK
  UINT32 FUNCDCCWCK:1;           ///< BIT21 - Functional Duty Cycle Correction for LP5 WCK
  UINT32 FUNCDCCDQ:1;            ///< BIT22 - Functional Duty Cycle Correction for Data DQ
  UINT32 DATAPILIN:1;            ///< BIT23 - Data Pi Linearity Calibration
  UINT32 DDR5XTALK:1;            ///< BIT24 - DDR5 Cross-Talk Cancellation
  UINT32 DCCLP5WCKDCA:1;         ///< BIT25 - DRAM DCC for LP5 WCK DCA
  UINT32 RXUNMATCHEDCAL:1;       ///< BIT26 - Unmatched Rx Calibaration
  UINT32 WRTDIMMDFE:1;           ///< BIT27 - DIMM DFE Training
  UINT32 RMTLVR:1;               ///< BIT28 - LVR RMT
  UINT32 Reserved2Bit29:1;       ///< BIT29 - Reserved
  UINT32 Reserved2Bit30:1;       ///< BIT30 - Reserved
  UINT32 SimicsReservedBit:1;    ///< BIT31 - Simics uses this bit for mrc_mem_flows
} TrainingStepsEn2;

typedef struct {
  UINT32 RXDQSDCC:1;          ///< BIT0 - RXDQS Duty Cycle Correction
  UINT32 DIMMNTODT:1;         ///< BIT1 - DIMM Non-Target ODT training
  UINT32 TXDQSDCC:1;          ///< BIT2 - TXDQS DCC Training
  UINT32 RXVREFPERBIT:1;      ///< BIT3 - RxVref Per-Bit Training
  UINT32 PPR:1;               ///< BIT4 - Post Package Repair
  UINT32 LVRAUTOTRIM:1;       ///< BIT5 - LVR Auto Trim
  UINT32 Reserved3Bit6:1;     ///< BIT6 - Reserved
  UINT32 OPTIMIZECOMP:1;      ///< BIT7 - Comp optimization
  UINT32 WRTRETRAIN:1;        ///< BIT8 - Write Dq/Dqs Retraining
  UINT32 Reserved3Bit9:1;     ///< BIT9 - Reserved
  UINT32 Reserved3Bit10:1;    ///< BIT10 - Reserved
  UINT32 JEDECRESET:1;        ///< BIT11 - Jedec Reset / Init (Don't expose to BIOS setup Menu)
  UINT32 ROUNDTRIPMATCH:1;    ///< BIT12 - Roundtrip Match (Don't expose to BIOS setup Menu)
  UINT32 TLINECLKCAL:1;       ///< BIT13 - Tline Clk Calibration
  UINT32 DCCPISERIALCAL:1;    ///< BIT14 - DCC PI Serializer Calibration
  UINT32 PHASECLKCAL:1;       ///< BIT15 - DCC Phase Clk Calibration
  UINT32 WCKPADDCCCAL:1;      ///< BIT16 - Wck Pad Dcc Calibration
  UINT32 RDCTLET : 1;         ///< BIT17 - Read CTLE Training
  UINT32 RDDQODTT : 1;        ///< BIT18 - Read DQ ODT Training
  UINT32 EMPHASIS : 1;        ///< BIT19 - Pre-Emphasis LP5 Training
  UINT32 DIMMRXOFFSET : 1;    ///< BIT20 - DRAM Rx Offset Calibration Training
  UINT32 VIEWPINCAL : 1;      ///< BIT21 - View Pin Calibration Training
  UINT32 QCLKDCC : 1;         ///< BIT22 - QCLK DCC calibration
  UINT32 WCKCLKPREDCC : 1;    ///< BIT23 - Wck / Clk Pre-Driver DCC
  UINT32 DQSPADDCC : 1;       ///< BIT24 - DQS DCC PAD Optimization
  UINT32 QCLKPHALIGN : 1;     ///< BIT25 - MC QClk Ph QClk Align Calibration
  UINT32 RXDQSVOCC : 1;       ///< BIT26 - RxDqs VOC Centering Training
  UINT32 ISENSERMT : 1;       ///< BIT27 - Current Sensor RMT
  UINT32 Reserved3Bit28 : 1;  ///< BIT28 - Reserved
  UINT32 Reserved3Bit29 : 1;  ///< BIT29 - Reserved
  UINT32 Reserved3Bit30 : 1;  ///< BIT30 - Reserved
  UINT32 Reserved3Bit31 : 1;  ///< BIT31 - Reserved
} TrainingStepsEn3;

typedef struct {
  UINT8 BoardStackUp : 1;    ///< BIT0 - Board Stack Up: <b>0=Typical</b>, 1=Freq Limited.
  UINT8 BoardTopology : 1;   ///< BIT1 - Board Topology: <b>0=Daisy Chain</b>, 1=Tee.
  UINT8 BrdReserved : 6;     ///< BIT[7:2] - Reserved for future use
  UINT8 BrdReserved1[3];     ///< Reserved bytes for struct DWORD alignment
} MrcBoardInputs;

#endif
