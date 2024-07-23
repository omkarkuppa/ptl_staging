/** @file
  This file defines functions for setting up test control
  registers for CPGC 2.0.

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
#ifndef _CPGC_20_TEST_CTL_H_
#define _CPGC_20_TEST_CTL_H_
#include "../CPGC/MrcCpgcApi.h"
#include "Cpgc20.h"

/**
  This function programs the masks that enable error checking on the
  requested cachelines and chunks.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  McChBitMask   - Controller / Channel Mask to program
  @param[in]  CachelineMask - Bit Mask of cachelines to enable.
  @param[in]  ChunkMask     - Bit Mask of chunks to enable.

  @retval Nothing
**/
void
Cpgc20SetChunkClErrMsk (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask,
  IN  UINT32                CachelineMask,
  IN  UINT32                ChunkMask
  );

/**
  This function programs the masks that enable error checking on the
  requested bytes (as per McChBitMask).

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  McChBitMask   - Controller / Channel Mask to program
  @param[in]  WordMask      - Which Word (Lower or Upper) to apply ErrMask to.
  @param[in]  ErrMask       - Error Masking Value to apply.

  @retval Nothing
**/
void
Cpgc20SetDataErrMsk (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask,
  IN  UINT32                WordMask,
  IN  UINT32                ErrMask
  );

/**
  This function programs the masks that enable ecc error checking on the
  requested bytes.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  EccValue      - Error Masking Value to apply.

  @retval Nothing
**/
void
Cpgc20SetEccErrMsk (
  IN  MrcParameters *const MrcData,
  IN  UINT32               EccValue
  );

/**
  This function returns a specific channel error status

  @param[in]      MrcData       - Pointer to MRC global data.
  @param[in]      Controller    - Desired Memory Controller.
  @param[in]      Channel       - Desired Memory Channel to read error status.
  @param[in out]  ErrorStatus     - returned value of error status on the Desired Channel.

  @retval Nothing
**/
void
Cpgc20GetChannelError (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  OUT UINT8            *ErrorStatus
);

/**
  This function returns a consolidated channel error status

  @param[in]      MrcData       - Pointer to MRC global data.
  @param[in]      McChBitMask   - Valid Channel bit mask

  @retval AllChError   - returns value of error status ORed between all channels
**/
UINT8
Cpgc20GetAllChannelsError (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask
);

/**
  This function programs the CPGC chicken internal config's Read Request doubler

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  Controller    - Desired Memory Controller.
  @param[in]  IsEnabled     - Determines if we are enabling or disabling chicken internal config, if enabled, read request
                              combines two returns into the same TID

  @retval Nothing
**/
void
Cpgc20SetChickenInternalConfig (
  IN  MrcParameters *const  MrcData,
  IN  INT8                  Controller,
  IN  BOOLEAN               IsEnabled
 );

/**
  This function programs the error conditions to stop the CPGC engine on.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  StopType      - Stop type for CPGC engine.
  @param[in]  NumOfErr      - Number of Stop Type errors to wait on before stopping CPGC engine.

  @retval Nothing
**/
void
Cpgc20SetupTestErrCtl (
  IN  MrcParameters       *const  MrcData,
  IN  MRC_TEST_STOP_TYPE          StopType,
  IN  UINT32                      NumOfErr
  );

/**
  This function will Setup REUT Error Counters to count errors for specified type.

  @param[in]  MrcData         - Pointer to MRC global data.
  @param[in]  CounterPointer  - Specifies in register which counter to setup. Each Channel has 9 counters including ECC.
  @param[in]  ErrControlSel   - Specifies which type of error counter read will be executed.
  @param[in]  CounterSetting  - Specifies in register which Lane/Byte/Chunk to track in specified counter,
                                based on ErrControlSel value.
  @param[in]  CounterScope    - Specifies if the Pointer is used or not.
  @param[in]  CounterUI       - Specifies which UI will be considered when counting errors.
                                  00 - All UI; 01 - Even UI; 10 - Odd UI; 11 - Particular UI (COUNTER_CONTROL_SEL = ErrCounterCtlPerUI)

  @retval mrcWrongInputParameter if CounterSetting is incorrect for the ErrControl selected, otherwise mrcSuccess.
**/
MrcStatus
Cpgc20SetupErrCounterCtl (
  IN MrcParameters *const      MrcData,
  IN UINT8                     CounterPointer,
  IN MRC_ERR_COUNTER_CTL_TYPE  ErrControlSel,
  IN UINT32                    CounterSetting,
  IN UINT8                     CounterScope,
  IN UINT8                     CounterUI
  );

/**
  This function returns the Error status results for specified MRC_ERR_STATUS_TYPE.

  @param[in]  MrcData     - Pointer to MRC global data.
  @param[in]  Controller  - Desired Controller.
  @param[in]  Channel     - Desired Channel.
  @param[in]  Param       - Specifies which type of error status read will be executed.
  @param[out] Buffer      - Pointer to buffer which register values will be read into.
                              Error status bits will be returned starting with bit zero.

  @retval Returns mrcWrongInputParameter if Param value is not supported by this function, otherwise mrcSuccess.
**/
MrcStatus
Cpgc20GetErrEccChunkRankByteStatus (
  IN  MrcParameters   *const  MrcData,
  IN  UINT32                  Controller,
  IN  UINT32                  Channel,
  IN  MRC_ERR_STATUS_TYPE     Param,
  OUT UINT64          *const  Buffer
  );

/**
  This function accesses the Sequence loop count.

  @param[in]  MrcData      - Pointer to MRC global data.
  @param[in]  McChBitMask  - Controller / Channel Mask to program
  @param[in]  LoopCount    - Pointer to variable to store or set.

  @retval Nothing.
**/
void
Cpgc20SetLoopCount (
  IN      MrcParameters *const  MrcData,
  IN      UINT8                 McChBitMask,
  IN OUT const UINT32   *const  LoopCount
  );

/**
  This function overrides the defualt Bank Address Swizzle to account for differences for LP5
  Cpgc Address bit 3 should be mapped to Cpgc Address bit 4, and Cpgc Address bit 3 should be left unused.

  @param[in]  MrcData    - Pointer to MRC global data.
  @param[in]  Controller - Desired Controller.
  @param[in]  Channel    - Desired Channel.

  @retval Nothing.
**/
VOID
Cpgc20LpBankAddrSwizzleUpdate (
  IN      MrcParameters *const  MrcData,
  IN      UINT32                Controller,
  IN      UINT32                Channel
  );

/**
  This function accesses the Logical to Physical Bank Mapping.

  @param[in]  MrcData    - Pointer to MRC global data.
  @param[in]  Controller - Desired Controller.
  @param[in]  Channel    - Desired Channel.
  @param[in]  Banks      - Pointer to buffer to logical-to-physical bank mapping.
  @param[in]  Count      - Length of the Banks buffer.
  @param[in]  IsGet      - TRUE: Get.  FALSE: Set.

  @retval Nothing.
**/
void
Cpgc20GetSetBankMap (
  IN      MrcParameters *const  MrcData,
  IN      UINT32                Controller,
  IN      UINT32                Channel,
  IN OUT  UINT8 *const          Banks,
  IN      UINT32                Count,
  IN      BOOLEAN               IsGet
  );

/**
  This function returns the Bit Group Error status results.

  @param[in]  MrcData     - Pointer to MRC global data.
  @param[in]  Controller  - Desired Controller.
  @param[in]  Channel     - Desired Channel.
  @param[out] Status      - Pointer to array where the lane error status values will be stored.

  @retval Nothing.
**/
void
Cpgc20GetBitGroupErrStatus (
  IN  MrcParameters   *const  MrcData,
  IN  UINT32                  Controller,
  IN  UINT32                  Channel,
  OUT UINT8                   *Status
  );

/**
  This function returns the Error Counter status for specified counter.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  Controller    - Desired Controller.
  @param[in]  Channel       - Desired Channel.
  @param[in]  CounterSelect - Desired error counter to read from.
  @param[out] CounterStatus - Pointer to buffer where counter status will be held.
  @param[out] Overflow      - Indicates if counter has reached overflow.

  @retval Nothing.
**/
void
Cpgc20GetErrCounterStatus (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                CounterSelect,
  OUT UINT64        *const  CounterStatus,
  OUT BOOLEAN       *const  Overflow
  );

/**
  This function writes to all enabled CPGC SEQ CTL registers.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  McChBitMask   - Memory Controller Channel Bit mask for which registers should be programmed for.
  @param[in]  CpgcSeqCtl    - Data to be written to all CPGC SEQ CTL registers.

  @retval Nothing.
**/
void
Cpgc20ControlRegWrite (
  IN  MrcParameters *const             MrcData,
  IN  UINT8                            McChBitMask,
  IN  MC0_REQ0_CR_CPGC_SEQ_CTL_STRUCT  CpgcSeqCtl
  );

/**
  This function Start Cpgc Test on all enabled CPGC SEQ CTL registers.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  McChBitMask   - Memory Controller Channel Bit mask for which registers should be programmed for.

  @retval Nothing.
**/
void
Cpgc20StartTest (
  IN  MrcParameters *const              MrcData,
  IN  UINT8                             McChBitMask
  );

/**
  This function Stop Cpgc Test on all enabled CPGC SEQ CTL registers.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  McChBitMask   - Memory Controller Channel Bit mask for which registers should be programmed for.

  @retval Nothing.
**/
void
Cpgc20StopTest (
  IN  MrcParameters *const              MrcData,
  IN  UINT8                             McChBitMask
  );

/**
  This function aggregates the status of STOP_TEST bit for all enabled CPGC engines.

  @param[in]  MrcData       - Pointer to MRC global data.

  @retval UINT8 of the aggregated value of STOP_TEST.
**/
UINT8
Cpgc20ControlRegStopBitStatus (
  IN  MrcParameters *const  MrcData
  );

/**
  This function writes Command Instructions to all enabled CPGC engines.

  @param[in]  MrcData        - Pointer to MRC global data.
  @param[in]  CmdInstruct    - Array of Command Instructions
  @param[in]  NumInstruct    - Number of Instructions

  @retval MrcStatus - mrcSuccess if does not exceed command instruction registers otherwise mrcFail
**/
MrcStatus
Cpgc20CmdInstructWrite (
  IN  MrcParameters *const                            MrcData,
  IN  MC0_REQ0_CR_CPGC2_COMMAND_INSTRUCTION_0_STRUCT  *CmdInstruct,
  IN  UINT8                                           NumInstruct
  );

/**
  This function writes Algorithm Instructions to all enabled CPGC engines.

  @param[in]  MrcData               - Pointer to MRC global data.
  @param[in]  AlgoInstruct          - Array of Algorithm Instructions
  @param[in]  AlgoInstructControl   - Algorithm Instruction Control setting to program based on ValidMask
  @param[in]  AlgoWaitEventControl  - Algorithm Wait Event Control setting to program
  @param[in]  ValidMask             - Bit Mask of which Instructions are valid

  @retval Nothing.
**/
void
Cpgc20AlgoInstructWrite (
  IN  MrcParameters *const                                      MrcData,
  IN  MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_0_STRUCT          *AlgoInstruct,
  IN  MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_STRUCT     AlgoInstructControl,
  IN  MC0_REQ0_CR_CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_STRUCT     AlgoWaitEventControl,
  IN  UINT8                                                     ValidMask
  );

/**
  This function writes Data Instructions to all enabled CPGC engines.

  @param[in]  MrcData        - Pointer to MRC global data.
  @param[in]  DataInstruct   - Array of Data Instructions
  @param[in]  ValidMask      - Bit Mask of which Instructions are valid

  @retval Nothing.
**/
VOID
Cpgc20DataInstructWrite (
  IN  MrcParameters *const                            MrcData,
  IN  MC0_REQ0_CR_CPGC2_DATA_INSTRUCTION_0_STRUCT     *DataInstruct,
  IN  UINT8                                           ValidMask
  );

/**
  This function sets up Address related registers (ADDRESS_INSTRUCTION, REGION_LOW_ROW, REGION_LOW_COL, ADDRESS_SIZE) to all enabled CPGC engines.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  CPGCAddressArray  - 2D Array of Structure that stores address related settings
  @param[in]  EnCADB            - Set up the address ordering for Command stress

  @retval Nothing.
**/
VOID
Cpgc20AddressSetup (
  IN  MrcParameters *const    MrcData,
  IN  MRC_ADDRESS             CPGCAddressArray [MAX_CONTROLLER][MAX_CHANNEL],
  IN  UINT8                   EnCADB
  );

/**
  This function acts as wrapper function and calls Cpgc20UpdateBaseRepeatsForWholeRankExtended().
  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Rank              - Rank to work on
  @param[in]  McChbitMask       - Bit masks of MC channels to enable for the test.
  @param[in]  UseBankAddresses  - Whether to use bank addresses in calculating base repeats. If false, only uses bank groups.
**/
void
Cpgc20UpdateBaseRepeatsForWholeRank (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Rank,
  IN  const UINT8           McChBitMask,
  IN  BOOLEAN               UseBankAddresses
  );

/**
  This function updates BASE_REPEATS to match the required number of Writes.
  This is used in memory scrubbing and PPR.
  The register will be updated on all enabled CPGC engines.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Rank              - Rank to work on
  @param[in]  McChbitMask       - Bit masks of MC channels to enable for the test.
  @param[in]  UseBankAddresses  - Whether to use bank addresses in calculating base repeats. If false, only uses bank groups.
  @param[in]  CapNotPowerOf2    - Whether non-power of 2 capacity found per MC/CH
**/
void
Cpgc20UpdateBaseRepeatsForWholeRankExtended (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Rank,
  IN  const UINT8           McChBitMask,
  IN  BOOLEAN               UseBankAddresses,
  IN  BOOLEAN               CapNotPowerOf2[MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL] OPTIONAL
  );

/**
  This function sets up Base Repeats to all enabled CPGC engines.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  McChBitMask       - Controller / Channel Mask to program
  @param[in]  Burst             - Number of Cachelines (Should be power of 2)
  @param[in]  Ranks             - # of Ranks to test

  @retval Value written to CPGC2_BASE_REPEATS.
**/
UINT32
Cpgc20BaseRepeats (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask,
  IN  UINT32                Burst,
  IN  UINT32                Ranks
  );

/**
  This function sets up ADDRESS_SIZE register per channel according to the Bank/Row/Col size of the given Rank.
  This is used in memory scrubbing and PPR.
  The register will be updated on all enabled CPGC engines.
  This function only supports DDR5 and LP5 BG Mode (Default CPGC settings support LP5 16 Bank Mode therefore it does not need additonal bank mapping)

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Rank              - Rank to work on
  @param[in]  McChbitMask       - Bit masks of MC channels to enable for the test.

**/
void
MrcUpdateL2PAllsBanksMapping (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Rank,
  IN  const UINT8           McChBitMask
  );

/**
  This function sets up Base Repeats for a given CPGC engine (per Channel / SubChannel).
  The function doesn't check for ch/subch presence.

  @param[in]  MrcData     - Pointer to MRC global data.
  @param[in]  Controller  - Controller to work on
  @param[in]  Channel     - Channel to work on
  @param[in]  Burst       - Number of Cachelines (Should be power of 2)
  @param[in]  Ranks       - # of Ranks to test

  @retval Value written to CPGC2_BASE_REPEATS.
**/
UINT32
Cpgc20BaseRepeatsMcCh (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                Burst,
  IN  UINT32                Ranks
  );

/**
  This function sets up Registers for Basic Mem Test to all enabled CPGC engines.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Burst             - Number of CL transactions per algorithm instruction within 1 BlockRepeat (outer loop)
  @param[in]  LoopCount         - Total # of CL transactions
  @param[in]  Ranks             - # of Ranks

  @retval Nothing.
**/
void
Cpgc20BasicMemTest (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Burst,
  IN  UINT32                LoopCount,
  IN  UINT8                 Ranks
  );

/**
This function reads base repeats register the channel/subChannel provided.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Controller        - Controller to work on
  @param[in]  Channel           - Channel to work on
  @retval Value read from CPGC2_BASE_REPEATS.
**/
UINT32
Cpgc20GetBaseRepeats (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel
  );

/**
  Wait till CPGC test is done on current Controller/Channel
  Time out if CPGC test takes longer then 10 seconds

  @param[in]  MrcData        - Pointer to MRC global data.
  @param[in]  Controller     - Controller to work on
  @param[in]  Channel        - Channel to work on

  @retval Success if CPGC test finishes without exceeding timeout value
**/
MrcStatus
Cpgc20TestDone (
  IN MrcParameters *const  MrcData,
  IN UINT32                Controller,
  IN UINT32                Channel
  );

/**
  Set CPGC command, algorithm and data instruction registers

  @param[in]  MrcData    - Pointer to MRC global data.
  @param[in] CmdPat      - 0: PatWrRd (Standard Write/Read Loopback),
                           1: PatWr (Write Only),
                           2: PatRd (Read Only),
                           3: PatRdWrTA (ReadWrite Turnarounds),
                           4: PatWrRdTA (WriteRead Turnarounds),
  @param[in] EnCADB      - Enable test to write random deselect packages on bus to create xtalk/isi
  @param[in] SubSeqWait  - # of Dclks to stall at the end of a sub-sequence

  @retval Nothing
**/
void
Cpgc20SetCommandSequence (
  IN MrcParameters *const  MrcData,
  IN const UINT8           CmdPat,
  IN const UINT8           EnCADB,
  IN UINT16                SubSeqWait
  );

/**
  This function disables/enables global bind for a specific channel

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  Controller    - Controller to work on
  @param[in]  Channel       - Channel to work on
  @param[in]  IsEnabled     - Determines if we are enabling or disabling global bind
  @retval Nothing.
**/
void
Cpgc20SetGlobalBind (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                IpChannel,
  IN  BOOLEAN               IsEnabled
  );

/**
  This function adjusts rank mapping register

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Controller        - Controller to work on
  @param[in]  Channel           - Channel to work on
  @param[in, out]  RankCount    - Number of Ranks
  @retval Nothing.
**/
void
Cpgc20PrepareRankMapping (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN UINT32                 RankBitMask,
  IN OUT  UINT8            *RankCount
  );

/**
  This function sets maximum number of ranks to test for a block and region

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Controller        - Controller to work on
  @param[in]  Channel           - Channel to work on
  @param[in]  MaxRank           - Maximum number of Ranks to test for a block and region
  @param[out] OrigRankCount     - Number of Ranks
  @retval Nothing.
**/
void
Cpgc20SetAddrMaxRank (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT8                 MaxRank,
  IN  OUT  UINT8            *OrigRankCount
  );

/**
  This function sets REQUEST_DATA_SIZE in Cpgc2AddrSize, The rest of configuration determined later on, as it depands on the test.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Controller        - Controller to work on
  @param[in]  IpChannel         - Channel to work on
  @param[in]  RequestDataSize   - Request Data Size value to set

  @retval Nothing.
**/
void
Cpgc20SetRequestDataSize (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                IpChannel,
  IN  UINT32                RequestDataSizeVal
  );

#endif // _CPGC_20_TEST_CTL_H_

/**
  This function gets maximum number of ranks to test for a block and region

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Controller        - Controller to work on
  @param[in]  Channel           - Channel to work on

  @retval # programmed into Block_Size_Max_Rank field.
**/
UINT8
Cpgc20GetAddrMaxRank (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel
  );

