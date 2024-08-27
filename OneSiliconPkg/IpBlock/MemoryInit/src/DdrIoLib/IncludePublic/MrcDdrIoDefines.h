/** @file
This file contains Memory Controller specific defines.

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
#ifndef _MrcDdrIoDefines_h_
#define _MrcDdrIoDefines_h_

// Partition Instances
#define MRC_CCC_NUM                 (8)   ///< # of CCC Partitions
#define MRC_CCC_SHARED_NUM          (4)   ///< # of CCC_SHARED Partitions
#define MRC_CCC_SBMEM_NUM           (4)   ///< # of CCC_SBMEM Partitions
#define MRC_COMP_NUM                (1)   ///< # of COMP Partitions
#define MRC_DATA_CH_NUM             (2)   ///< # of Channels in the DATA Partitions
#define MRC_CCC_SHARED_MOBILE_NUM   (4)   ///< # of CCC_SHARED Partitions in Mobile
#define MRC_CCC_SHARED_DT_NUM       (4)   ///< # of CCC_SHARED Partitions in Desktop
// DATA_SHARED is a CR partition which is shared across the two bytes in the partition.
// The number of DATA_SHARED is the same as the number of DATA partitions.
// So the same define is used to avoid duplication and proper scaling in DT.
#define MRC_DATA_DT_NUM       (10)  ///< # of DATA Partitions in Desktop
#define MRC_DATA_MOBILE_NUM   (8)   ///< # of DATA Partitions in Mobile
#define MRC_DATA_SHARED_NUM_WITH_ECC   (10)   ///< # of DATA_SHARED Partitions
#define MRC_PG_NUM            (12)  ///< # of PG Partitions: DDRPG[0..9], DDRPGTERM[0..1]
#define MRC_PG_DT_NUM         (13)  ///< # of PG Partitions in Desktop
#define MRC_VCCCLK_DT_NUM     (3)   ///< # of VCCCLK Partitions in Desktop
#define MRC_VCCCLK_MOBILE_NUM (2)   ///< # of VCCCLK Partitions in Mobile
#define MRC_NUM_PAR_PER_SHARED (2)  ///< # of paritions per Shared region

// MRC_CTL_GRP_PER_CH_DT_NUM-2, MRC_CTL_GRP_PER_CH_MOBLIE_NUM-1, MAX-2
#define MRC_CTL_GRP_PER_CH_MAX       (2)

/// DDR Safe Mode
#define DDR_SAFE_BASICPM             (MRC_BIT0)
#define DDR_SAFE_SPINEGATE           (MRC_BIT1)
#define DDR_SAFE_DCC                 (MRC_BIT2)
#define DDR_SAFE_DCC_CLK             (MRC_BIT3)
#define DDR_SAFE_PERIODIC_COMP       (MRC_BIT4)
#define DDR_SAFE_COMP_CLK            (MRC_BIT5)
#define DDR_SAFE_TRANSFORMER         (MRC_BIT6)
#define DDR_SAFE_WRITE_0             (MRC_BIT7)
#define DDR_SAFE_ODT_QUALIFIED       (MRC_BIT8)
#define DDR_SAFE_CCC_PATH            (MRC_BIT9)
#define DDR_SAFE_DQ_DBI              (MRC_BIT10)
#define DDR_SAFE_DQ_CLK              (MRC_BIT11)
#define DDR_SAFE_DQ_RETRAIN          (MRC_BIT12)
#define DDR_SAFE_RX_PATH             (MRC_BIT13)
#define DDR_SAFE_TX_PATH             (MRC_BIT14)
#define DDR_SAFE_PG_CLK              (MRC_BIT15)
#define DDR_SAFE_DIGCLK              (MRC_BIT16)
#define DDR_SAFE_PERIODIC_RXDQSCOMP  (MRC_BIT17)
#define DDR_SAFE_WAKEUP3             (MRC_BIT18)
#define DDR_SAFE_PHCLK               (MRC_BIT19)
#define DDR_SAFE_CLOCKING            (MRC_BIT20)
#define DDR_SAFE_VSXHI               (MRC_BIT21)
#define DDR_SAFE_DLL_PI              (MRC_BIT22)
#define DDR_SAFE_GRACE               (MRC_BIT23)
#define DDR_SAFE_DATAPATH            (MRC_BIT24)
#define DDR_SAFE_LVR                 (MRC_BIT25)
#define DDR_SAFE_PLL                 (MRC_BIT26)
#define DDR_SAFE_MCQCLK              (MRC_BIT27)
#define DDR_SAFE_ROUNDTRIP           (MRC_BIT28)

/// Rank to Rank Mux
#define MRC_PICODELUT_PARTITION (2) // DATASHARED, CCCSHARED
#define MAX_NUM_TX_R2RDELAY (3)     // # of TX R2R Delay (DelayForDqsPi, DelayForDqsEq, DelayForDqPi)
#define MAX_NUM_RX_R2RDELAY (4)     // # of RX R2R Delay (DelayForRxPi, DelayForRcvEn, DelayForDqOffset, DelayForDqsOffset)
#define NUM_TXR2RTYPE (2)           // Eq, Pi
#define NUM_RXR2RTYPE (2)

#define MRC_MAX_MIXERCODE_TC (15)
#define MRC_MAX_MIXERCODE    (16)

// DDRPHY_DDRCOMP_CR_PICODELUT1_MixerCode9_MAX is 31 but we should use 23
#define MRC_PICODELUT1_MixerCode9_MAX (23)

// Delta Pi adjustment between TxDqsDelay and TxDqDelay
#define TX_DELTA_PI_ADJUSTMENT  32

// This defines the PHYs Internal Write FIFO Delay
#define PHY_FIFO_CONSTANT  (12)

// This defines the write fifos range in UI
#define WRT_FIFO_RANGE_UI  (24)

#define CCC_CH_NOT_POP_MASK_LOW      0x0F
#define CCC_CH_NOT_POP_MASK_HIGH     0xF0

/// Vref step size for the following: DqOdt, DqDrv, CmdDrv, CtlDrv, ClkDrv
#define MRC_COMP_VTARG_CALCULATION_VALUE  (384)
#define MRC_COMP_VREF_CALCULATION_VALUE   (386)
#define MRC_COMP_VREF_STEP_SIZE   (191)

/// RCOMP Vref calculation value for the following: DqOdt, DqDrv, CmdDrv, CtlDrv, ClkDrv
#define MRC_RCOMP_VREF_CALCULATION_VALUE  (193)

/// Solving for PadV in formula: XYZ = Rnd(((PadV/VrefRail)*386-1) /2 ) without Rounding
#define PADV_CALC(VREF, RAIL) (((RAIL) * (2 * (VREF) + 1)) / MRC_COMP_VREF_CALCULATION_VALUE)

#define BG_REF_MV                          (800) ///< Banded Gap Reference Voltage

#define MAX_PHASE (8)

#define NUM_PHASE_PLL  (2)
#define NUM_PLL        (2)

typedef enum {
  Off,
  DqDelayForEq,
  DqsDelayForEq,
  DqDelayForPi,
  DqsDelayForPi,
  DelayForDqOffset,
  DelayForDqsOffset,
  DelayForRxPi,
  DelayForRcvEn
} R2RMode;

/// Comp Types
typedef enum {
  FullComp,
  DvfsComp,
  CompDist,
  CompUpdate,
  FullCompDccFix,
  MaxCompTypes
} COMP_CYCLE_TYPE;

typedef enum {
  RdOdt,
  WrDS,
  WrDSCmd,
  WrDSCtl,
  WrDSClk,
  TGlobalCompOffsetMax
} TGlobalCompOffset;

typedef enum {
  Ddr5Nil,
  Lpddr5Nil,
  Ddr5Il,
  MrcPartitionTypeMax
} MRC_PARTITION_TYPE;

#endif // _MrcDdrIoDefines_h_
