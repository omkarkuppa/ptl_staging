/** @file
  This file declares the interface between the MRC and the different
  versions of CPGC.

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
#ifndef _MRC_CPGC_API_H_
#define _MRC_CPGC_API_H_
#include "MrcCommon.h"

/// Defines and Enumerations
#define CADB_20_MAX_CHUNKS  (8)
#define MRC_MUX_PB_LENGTH   (24)

#define RDCPL_CREDITS_DEFAULT (32)
#define RDCPL_CREDITS_SCRUB   (10)

// Mask for MCx_CHx_CPGC_ERR_TEST_ERR_STAT.ERROR_STATUS
// Each bit is one nibble
#define CPGC_23_ERROR_STATUS_MASK_DDR5  (0x100FF)       // 32 data bits + ECC nibble
#define CPGC_23_ERROR_STATUS_MASK_LPDDR (0x0000F)       // 16 data bits

#define MT_ADDR_DIR_UP      (0)
#define MT_ADDR_DIR_DOWN    (1)

///
/// Structs and Types
///
typedef union {
  struct {
    UINT32  CmdAddr : 24; ///<  Bits 0:23
    UINT32  Spare   : 8;  ///<  Bits 24:31
  } Bits;
  struct {
    UINT32  Ca1   : 7;  ///< Bits 0:6   For Selects this is CA 1.  For DeSelects, this is Sub Channel 0
    UINT32  Ca2   : 7;  ///< Bits 7:13  For Selects this is CA 2.  For DeSelects, this is Sub Channel 1
    UINT32  Ca3   : 7;  ///< Bits 14:20 For Selects this is CA 3.  For DeSelects, this is unused.
    UINT32  Ca4   : 7;  ///< Bits 21:27 For Selects this is CA 4.  For DeSelects, this is unused.
    UINT32  Spare : 4;  ///< Bits 28:31
  } Lpddr5;
  struct {
    UINT32  Ca    : 14; ///< Bits 0:13
    UINT32        : 18; ///< Bits 14:31
  } Ddr5;
  UINT32  Data32;
} MRC_CA_MAP_TYPE;

typedef enum {
  MrcLfsr8,
  MrcLfsr15,
  MrcLfsr31,
  MrcLfsr23b, //x23 + x18
  MrcLfsr7,
  MrcLfsr16,
  MrcLfsr23a, //x23 + x21 + x18 + x15 + x7 + x2
  MrcLfsr32,
  MrcLfsrTypeMax
} MRC_PG_LFSR_TYPE;

/// Public Function Declaration

/**
  This function activates and initializes the CPGC Engine.
  The McChBitMask and CpgcChAssign variables will be initialized
  based on the populated channels.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess
**/
MrcStatus
Cpgc20Init (
  IN  MrcParameters *const  MrcData
  );

/**
  This function programs the Error Mask for Cacheline and UI comparisons.
  A value of 1 enables error checking.

  @param[in]  MrcData       - Pointer to global MRC data.
  @param[in]  McChBitMask   - Controller / Channel Mask to program
  @param[in]  CachelineMask - Bit mask of cachelines to enable.
  @param[in]  ChunkMask     - Bit mask of chunks to enable.

  @retval MrcStatus - mrcSuccess.
**/
MrcStatus
MrcSetChunkAndClErrMsk (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask,
  IN  UINT32                CachelineMask,
  IN  UINT32                ChunkMask
  );

/**
  This function programs the Error Mask for Data and ECC bit lanes as per McChBitMask.
  A value of 1 enables error checking for that bit.

  @param[in]  MrcData    - Pointer to global MRC data.
  @param[in]  McChBitMask   - Controller / Channel Mask to program
  @param[in]  DataMask   - Bit mask of Data bits to check.
  @param[in]  EccMask    - Bit mask of ECC bits to check.

  @retval MrcStatus - mrcSuccess.
**/
MrcStatus
MrcSetDataAndEccErrMsk (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask,
  IN  UINT64                DataMask,
  IN  UINT8                 EccMask
  );
/**
  This function programs the test loop count.

  @param[in]  MrcData      - Pointer to MRC global data.
  @param[in]  McChBitMask  - Controller / Channel Mask to program
  @param[in]  LoopCount    - Number of sequence iterations. 0 means infinite test.

  @retval MrcStatus - mrcSuccess.
**/
MrcStatus
MrcSetLoopcount (
  IN MrcParameters *const  MrcData,
  IN UINT8                 McChBitMask,
  IN UINT32                LoopCount
  );
/**
  This function will Setup REUT Error Counters to count errors for specified type.

  @param[in]  MrcData           - Include all MRC global data.
  @param[in]  CounterPointer    - Specifies in register which Counter to setup. Valid for ErrControl values:
                                  ErrCounterCtlPerLane, ErrCounterCtlPerByte, ErrCounterCtlPerNibble, or ErrCounterCtlPerUI.
  @param[in]  ErrControl        - Specifies which type of error counter read will be executed.
  @param[in]  CounterUI         - Specifies which UI will be considered when counting errors.
                                  00 - All UI; 01 - Even UI; 10 - Odd UI; 11 - Particular UI (COUNTER_CONTROL_SEL = ErrCounterCtlPerUI)

  @retval mrcWrongInputParameter if ErrControlSel is an incorrect value, otherwise mrcSuccess.
**/
MrcStatus
MrcSetupErrCounterCtl (
  IN  MrcParameters *const      MrcData,
  IN  UINT8                     CounterPointer,
  IN  MRC_ERR_COUNTER_CTL_TYPE  ErrControlSel,
  IN  UINT8                     CounterUI
  );

/**
  This function returns the Bit Group Error status results.

  @param[in]  MrcData     - Include all MRC global data.
  @param[in]  Controller  - 0-based index selecting Controller to adjust.
  @param[in]  Channel     - 0-based index, Channel to configure.
  @param[out] Status      - Pointer to array where the lane error status values will be stored.

  @retval Nothing.
**/
void
MrcGetBitGroupErrStatus (
  IN  MrcParameters   *const  MrcData,
  IN  UINT32                  Controller,
  IN  UINT32                  Channel,
  OUT UINT8                   *Status
  );

/**
  This function returns the Error status results of the specified Error Type.

  @param[in]  MrcData     - Include all MRC global data.
  @param[in]  Controller  - Desired Controller.
  @param[in]  Channel     - Desired Channel.
  @param[in]  Param       - Specifies which type of error status read will be executed.
  @param[out] Buffer      - Pointer to buffer which register values will be read into.
                              Error status bits will be returned starting with bit zero. Logical shifting will not be handled by this function.

  @retval Returns mrcWrongInputParameter if Param value is not supported by this function, otherwise mrcSuccess.
**/
MrcStatus
MrcGetMiscErrStatus (
  IN  MrcParameters   *const  MrcData,
  IN  UINT32                  Controller,
  IN  UINT32                  Channel,
  IN  MRC_ERR_STATUS_TYPE     Param,
  OUT UINT64          *const  Buffer
  );

/**
  This function sets or returns the Logical-to-Physical mapping of Banks.  The index to the array
  is the logical address, and the value at that index is the physical address.  This function
  operates on a linear definition of Banks, even though there may be a hierarchy as BankGroup->Bank.
  For system with X Bank Groups and Y Banks per group, the Banks are indexed in the array as:
  (X * Y + Z) where X is the bank group, Y is the total number of banks per group, and Z is the
  target bank belonging to bank group X.

  Example:
    Bank Group 3, Bank 5, 8 Banks per Bank Group -> Index position (3 * 8) + 5 == 29.

  The function will act upon the array bounded by the param Length.
  The Function always starts at Bank 0.

  @param[in]  MrcData     - Pointer to MRC global data.
  @param[in]  Controller  - Desired Controller.
  @param[in]  Channel     - Desired Channel.
  @param[in]  L2pBankList - Array of logical-to-physical Bank Mapping.
  @param[in]  Length      - Array length of L2pBankList buffer.
  @param[in]  IsGet       - Boolean; if TRUE, request is a Get, otherwise Set.

  @retval MrcStatus.
**/
MrcStatus
Cpgc20GetSetBankSequence (
  IN  MrcParameters *const      MrcData,
  IN  UINT32                    Controller,
  IN  UINT32                    Channel,
  IN  MRC_BG_BANK_PAIR  *const  L2pBankList,
  IN  UINT32                    Length,
  IN  BOOLEAN                   IsGet
  );

/**
  This function returns the Error Counter and Raster Repo status.

  @param[in]  MrcData       - Include all MRC global data.
  @param[in]  Controller    - Desired Controller.
  @param[in]  Channel       - Desired Channel.
  @param[in]  Byte          - Desired Byte.
  @param[in]  EvenOddUIErr  - Selects which Raster Repo register to be read to get Even/Odd UI error status for all lanes.
  @param[in]  RasterRepoStatus        - Pointer to buffer where raster repo status will be held.

  @retval RasterRepoStatus.
**/
void
MrcGetRasterRepoStatus (
  IN  MrcParameters   *const    MrcData,
  IN  UINT32                    Controller,
  IN  UINT32                    Channel,
  IN  UINT8                     Byte,
  IN  UINT8                     EvenOddUI,
  OUT UINT64            *const  RasterRepoStatus
);

/**
  This function will enable or disable CPGC engines on all channels

  @param[in] MrcData    - Global MRC data structure
  @param[in] ActiveMode - If true, enable CPGC engines. If false, set to idle mode.

  @retval - Nothing
**/
extern
void
MrcSetCpgcInitMode (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              ActiveMode
  );

/**
  This function clears the raster repo to get a new error
  status on the next RunIOTest.

  @param[in]  MrcData     - Pointer to MRC global data.
  @param[in]  Controller  - Controller to work on
  @param[in]  Channel     - Channel to work on

  @retval Nothing
**/
VOID
MrcCpgcClearRasterRepo (
  IN MrcParameters *const  MrcData,
  IN UINT32                Controller,
  IN UINT32                Channel
  );

/**
  This function immediately clear all error registers and error status
  except for the Raster Repository.

  @param[in] MrcData     - The MRC global data.
  @param[in] McChBitMask - Controllers and channels to work on.

  @retval none
**/
void
Cpgc20ClearErrors (
  IN MrcParameters *const MrcData,
  IN  UINT8               McChBitMask
  );

/**
  This function configures initial CPGC credits before running any CPGC traffic.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  Controller    - Controller to work on
  @param[in]  IsCpgcActive  - FALSE for Normal mode, TRUE for CPGC mode

  @retval Nothing.
**/
VOID
MrcCpgcConfigInitialCredits (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  BOOLEAN               IsCpgcActive
  );

/**
  This function read the initial CPGC credits if IsSave is set and program new value.

  @param[in]      MrcData       - Pointer to global MRC data
  @param[in]      Controller    - Controller to work on
  @param[in]      IsSave        - Set to save initial CPGC credits
  @param[in,out]  RdCplCredits  - CPGC credits to program

  @retval MrcStatus - mrcFail if ISM is not in IDLE state
                    - mrcSuccess otherwise.
**/
MrcStatus
MrcCpgcModifyReadCredits (
  IN     MrcParameters* const  MrcData,
  IN     UINT32                Controller,
  IN     BOOLEAN               IsSave,
  IN OUT UINT32                *RdCplCredits
  );

/**
  This function return the CPGC ErrStatus Mask based on byte

  ECC bytes only has 4 bit, so return only lower nibble, else return 0xFF

  @param[in]  Byte - Byte to get ErrStatusMask
**/
UINT8
MrcCpgcErrStatusMask (
  UINT32 Byte
  );

/**
  This function return the BitMask based on byte

  ECC byte only has 4 bit, so return bitmask of lower nibble

  @param[in]  Byte - Byte to get BitMask
  @param[in]  Bit  - Bit to get BitMask
**/
UINT8
MrcGetCpgcBitMask (
  UINT32 Byte,
  UINT32 Bit
  );

/**
  This function sets up ADDRESS_SIZE register per channel to the whole rank, Bank/Row/Col size is determined by DIMM.
  This is used in memory scrubbing.
  The register will be updated on all enabled CPGC engines.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[out] CPGCAddressArray  - 2D Array of Structure that stores address related settings per channel.
  @param[in]  AddrDirection     - CPGC Address Direction
  @param[in]  McChMask          - Channel bit mask (can be different than Outputs->McChBitMask)
  @param[in]  Rank              - Rank to work on

**/
VOID
UpdateAddressForWholeRank (
  IN  MrcParameters *const            MrcData,
  OUT MRC_ADDRESS                     CPGCAddressArray[MAX_CONTROLLER][MAX_CHANNEL],
  IN  MRC_ADDRESS_INCREMENT_ORDER     AddrIncOrder,
  IN  UINT8                           McChMask,
  IN  UINT32                          Rank
  );

#endif //_MRC_CPGC_API_H_
