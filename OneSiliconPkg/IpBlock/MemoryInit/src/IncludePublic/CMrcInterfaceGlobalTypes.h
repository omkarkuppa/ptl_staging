/** @file
This file includes all the data structures that the MRC considers "global data".

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
#ifndef _InterfaceGlobalType_h_
#define _InterfaceGlobalType_h_


#ifndef MDEPKG_NDEBUG
#ifndef MRC_DEBUG_PRINT
#define MRC_DEBUG_PRINT (1)
#endif
#endif // MDEPKG_NDEBUG


#ifdef XTENSA
#include <CGreenMrcCommonTypes.h>
#else
#include <CBlueMrcCommonTypes.h>
#endif

#include <Ptl/MrcGlobalDefinitions.h>
#include "Ptl/CMrcExtInputs.h"



#pragma pack (push, 1)

#define CALC_MRC_DDR_TYPE_MAX(x)  ((x) - 1)
#define MAX_COMMAND_GROUPS        (2)       ///< 2 for DDR5. 1 for LPDDR per controller/channel
#define MAX_SUB_CHANNEL           (2)       ///< The maximum number of subchannels per channel.
#define MAX_IP_CHANNEL            (2)
#define MAX_RANK_IN_SUBCHANNEL    (2)       ///< The number of ranks per subchannel, LP specific
#define MAX_SDRAM_IN_DIMM_NO_ECC  (4)       ///< The maximum number of SDRAMs per DIMM when ECC is disabled.

#define NUM_CPU_MODELS            (2)       ///< Number of skus supported (Ulx/Ult, DtHalo)

typedef enum {
  MRC_DDR_TYPE_LPDDR5  = 0,
  MRC_DDR_TYPE_DDR5    = 1,
  MRC_DDR_TYPE_UNKNOWN,  ///< Delimiter
  MAX_MRC_DDR_TYPE
} MrcDdrType;

///
/// UPM PWR and Retrain Limits
///
#define MRC_NUMBER_UPM_PWR_RETRAIN_MARGINS (6)
#define MAX_RCOMP         (3)
#define MAX_RCOMP_TARGETS (5)

#define SWEEP_UP    (0)
#define SWEEP_DOWN  (1)

#define CHANNEL_STROBE_CACHE        (MAX_CHANNEL * MAX_SDRAM_IN_DIMM)
#define CHANNEL_RANK_STROBE_CACHE   (MAX_CHANNEL * MAX_RANK_IN_CHANNEL * MAX_SDRAM_IN_DIMM)
#define CHANNEL_SUB_CHANNEL_CACHE   (MAX_CHANNEL * MAX_SUB_CHANNEL)
#define CRS_ARRAY_ENTRIES           (5 * CHANNEL_RANK_STROBE_CACHE)
#define CS_ARRAY_ENTRIES            (5 * CHANNEL_STROBE_CACHE)
#define CHANNEL_ARRAY_ENTRIES       (10 * MAX_CHANNEL)
#define CHANNEL_SUBCH_ARRAY_ENTRIES (3 * CHANNEL_SUB_CHANNEL_CACHE)
#define ODT_CTL_ENTRIES             (2 * 2 * MAX_CHANNEL)
#define SINGLE_ENTRIES              (13)
#define CACHE_PAD_ENTRIES           (0)

#ifdef MRC_DISABLE_CACHING
  #define REGISTER_CACHE_SIZE         1
  #define MAX_REGISTER_CACHE_ENTRIES  1
#else
    #define MIN_CACHE_ENTRIES_SIZE      1024

  #define MIN_CACHE_ENTRIES_MASK      (MIN_CACHE_ENTRIES_SIZE-1)
  #define REGISTER_CACHE_SIZE   ( \
    CRS_ARRAY_ENTRIES +           \
    CS_ARRAY_ENTRIES +            \
    CHANNEL_ARRAY_ENTRIES +       \
    CHANNEL_SUBCH_ARRAY_ENTRIES + \
    ODT_CTL_ENTRIES +             \
    SINGLE_ENTRIES +              \
    CACHE_PAD_ENTRIES)

    #define MAX_REGISTER_CACHE_ENTRIES ((REGISTER_CACHE_SIZE<MIN_CACHE_ENTRIES_SIZE) ? MIN_CACHE_ENTRIES_SIZE : REGISTER_CACHE_SIZE)
#endif // MRC_DISABLE_CACHING


typedef UINT16 MrcPostCode;


///
/// Enumerations
///
typedef enum {
  CpuCellDelay,
  CpuRon,
  CpuTxEq,
  DimmWrOdtNT,
  MbLengthTx,
  SoDimmLenTx,
  MaxTxPowerParam
} TxPowerOptParam;

typedef enum {
  CpuOdt,
  DimmRdOdtNT,
  MbLengthRx,
  SoDimmLenRx,
  MaxRxPowerParam
} RxPowerOptParam;

typedef enum {
  RefRateLowTempOOS,  // Not safe
  RefRateFourth,      // 4x tREFI - Not safe
  RefRateHalf,        // 2x tREFI
  RefRate1x,          // tREFI
  RefRate2x,          // 1/2 tREFI
  RefRate4x,          // 1/4 tREFI
  RefRate4xDeRateAc,  // 1/4 tREFI de-rate AC timing - Not safe
  RefRateHighTempOOS  // Not safe
} LpddrRefreshRates;

///
/// Structures
///
typedef struct {
  union {
    struct {
      UINT8 IsValid     : 1;  // Bits 0:0
      UINT8 Is64BitReg  : 1;  // Bits 1:1
      UINT8 IsDirty     : 1;  // Bits 2:2
      UINT8 OffsetMsb   : 1;  // Bits 3:3
      UINT8 IsHighDWord : 1;  // Bits 4:4
      UINT8             : 3;  // Bits 7:5
    } Bits;
    UINT8 Data;
  } Flags;
  UINT16 Offset;
  UINT32 Data;
} REGISTER_CACHE_OFFSET_STRUCT;

typedef struct {
//  UINT32                       CurrentSequence;
//  UINT32                       NextSequence;
  UINT16                       MaxValid;
  BOOLEAN                      Overflow;
  REGISTER_CACHE_OFFSET_STRUCT Data[MAX_REGISTER_CACHE_ENTRIES];
} MRC_REGISTER_CACHE;

typedef union {
  struct {
    UINT32 Frequency :  16;
    UINT32 DdrType   :  2;
    UINT32 RttWr     :  8;
    UINT32 Spare     :  6;
  } Bits;
  UINT32 Data;
} MRC_POWER_SYS_CONFIG;

typedef struct {
  INT32 Intercept;
  INT32 FirstOrder;
  INT32 SecondOrder;
  INT32 Interactions[MaxTxPowerParam];
} MRC_POWER_COEFF;

typedef struct {
  MRC_POWER_SYS_CONFIG  Config;
  UINT32                Intercept0;
  MRC_POWER_COEFF       Coeff[MaxTxPowerParam];
} MRC_RSM_POWER_FORMULA;

typedef struct {
  UINT8  Param;
  UINT16 ParamLimit[NUM_CPU_MODELS][CALC_MRC_DDR_TYPE_MAX (MAX_MRC_DDR_TYPE)];
} MrcUpmPwrRetrainLimits;

typedef struct {
  UINT8  Param;
  UINT8 GuardbandLimit[NUM_CPU_MODELS][CALC_MRC_DDR_TYPE_MAX (MAX_MRC_DDR_TYPE)];
} AgingGuardbandLimits;

/*
 * Saved in MrcOptParamMapping and used by TrainDDROptParam
 */
typedef enum {
  OPT_GLOBAL          = MRC_BIT0,
  OPT_PERRANK         = MRC_BIT1,
  OPT_CCC             = MRC_BIT2,
  OPT_CPUCOMP         = MRC_BIT3,
  OPT_DIMMIMPEDANCE   = MRC_BIT4,
  OPT_CHANGE_RXVREF   = MRC_BIT5,
  OPT_CHANGE_TXVREF   = MRC_BIT6,
  OPT_CHANGE_CMDVREF  = MRC_BIT7,
  OPT_1D_AVEN_1       = MRC_BIT8,
  OPT_1D_AVEN_5       = MRC_BIT9,
  OPT_2D_AVEN_3       = MRC_BIT10,
  OPT_MRC_DLL_RESET   = MRC_BIT11,
  OPT_SERIALIZE_RANKS = MRC_BIT13,
  OPT_RESTORE_CAC_VREF = MRC_BIT14
} OptParamFlags;

typedef enum {
  OPT_LIMIT_START = 0,
  OPT_LIMIT_STOP  = 1,
  OPT_LIMIT_MAX   = 2
} OptLimitType;

/*
 * OptParamMapping: defines OptParam => GSM_GT GetSetGroup mapping, StepSize,
 * Limits, and others.
 * Param equals the original OptParam. It's for double-checking in case
 * developers make mistake by adding (or forgetting) OptParam at wrong place.
 *
 * 1) Possible scenarios of Mapping between OptParam and GSM_GT Groups
 *    (1.1) 1:0 : Group value equals GsmGtDelim, no such mapping. Functions need use
 *        special codes to deal with it. Typically, DRAM MR fields are examples, as
 *        their access is not implemented in GetSet yet.
 *    (1.2) 1:1 : OptParam maps to a GSM Group. Usually, MRC will call GetSet to
 *      deal with it.
 *    (1.3) 1:N : OptParam maps to a few GSM Groups.
 *      (1.3.1) Each GSM group uses the same Limits and StepSize. Functions get
 *              Limits and StepSize consistently, but call GetSet by special codes.
 *      (1.3.2) Each GSM Group uses different Limits and StepSize. Current power
 *        training doesn't support it. If there is such case, functions need special
 *        codes to deal with it, usually hard-coded.
 *    (1.4) N:1 : Usually, MRC defines multiple OptParams, and each one can use
 *        different StepSize and Limits.
 *    (1.5) N:N : No such case currently.
 *  2) StepSize: if OptParam uses varying StepSize based on specific scenario,
 *    change function OptParamMapping to deal with such special case
 *  3) Limits[0]:
 *    (3.1) OPT_DIMMIMPEDANCE is not set in Flags: If it equals to MRC_INT16_MAX,
 *  MRC calls MrcGetSetLimits to get the MinVal. Otherwise, MRC uses Limits[0]
 *  as the MIN value.
 *    (3.2) OPT_DIMMIMPEDANCE is set in Flags: Call GetDimmOptParamValues to get
 *    STop value.
 *  4) Limits[1]: Similar to Limits[0], but choose MaxVal.
 *  5) Flags: Control flow in TrainDDROptParam.
 */
typedef struct {
  UINT8           Param;
  GSM_GT          GetSetGroup;
  INT8            StepSize;
  INT16           Limits[OPT_LIMIT_MAX]; // Start,Stop in 1 step size.
  OptParamFlags   Flags;
} OptParamMapping;

extern const MrcUpmPwrRetrainLimits InitialLimits[MRC_NUMBER_UPM_PWR_RETRAIN_MARGINS];

typedef union {
  MrcUpmPwrRetrainLimits *Pointer;
  UINT64                 Data;
} MrcUPRLimitPtr;

typedef struct {
  UINT16  RcvEn;
  UINT16  TxDqs;
  UINT16  TxDq;
  UINT16  Wck;
  BOOLEAN Valid;
  UINT8   Dummy8AlignmentBuffer[3];
} MrcIntClkAlignedParam;

typedef struct {
  UINT16 ClkPiCode[MAX_RANK_IN_CHANNEL]; ///< Clk Pi Code
  UINT16 CtlPiCode[MAX_RANK_IN_CHANNEL]; ///< Ctl Pi Code
  UINT16 CmdPiCode[MAX_COMMAND_GROUPS];  ///< CmdPiCode CR, per group
  UINT16 WckPiCode[MAX_SDRAM_IN_DIMM];   ///< WCK Pi Code per byte
} MrcIntCmdTimingOut;
#pragma pack (pop)
#endif
