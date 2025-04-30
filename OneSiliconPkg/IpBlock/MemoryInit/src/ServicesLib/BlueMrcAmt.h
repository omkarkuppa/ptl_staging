/** @file
  This file contains functions to detect and record DRAM Failing addresses via
  Advanced Memory Test.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
#ifndef _BlueMrcAmt_h_
#define _BlueMrcAmt_h_

#include "MrcAmt.h"
#include "MrcCpgcApi.h"
#include "Cpgc20.h"
#include "Cpgc20Patterns.h"

/**
  Cpgc command pattern setup up for memory test

  @param[in] MrcData        - Pointer to MRC global data.
  @param[in] RwMode         - Cpgc read and write mode
  @param[in] UseSubSeq1     - Cpgc subseq1 usage
  @param[in] Direction      - Address direction during memory test
  @param[in] SeqDataInv     - Enables pattern inversion per subsequence
  @param[in] IsUseInvtPat   - Info to indicate whether or not patternQW is inverted by comparing original pattern

  @retval n/a
**/
VOID
MrcConfigurePprCmdPat (
  IN MrcParameters  *const MrcData,
  IN UINT8                 RwMode,
  IN BOOLEAN               UseSubSeq1,
  IN UINT8                 Direction,
  IN UINT8                 SeqDataInv[2],
  IN BOOLEAN               IsUseInvtPat
  );

/**
  This function programs the Raster Repo Configuration register based on input values.

  @param[in]  MrcData         - Pointer to MRC global data.
  @param[in]  McChBitMask     - Bitmask of participating controllers / channels
  @param[in]  Mode3Banks      - Specifies which Bank mode to use in Raster Repo Mode 3 usage.
  @param[in]  UpperBanks      - Indicates the base Bank to store when using Raster Repo Mode 3
  @param[in]  StopOnRaster    - If TRUE, the test will stop after status condition is met in Raster Repo
  @param[in]  RasterRepoClear - If set, Raster Repo entries will be Reset (this bit auto-clears)
  @param[in]  RasterRepoMode  - Indicates the mode of operation for Raster Repo
  @param[in]  Mode3Max        - In Raster Repo Mode 3, sets maximum number of failing address before an additional failing address will set fail_excess for the respective Bank.
**/
void
Cpgc20RasterRepoConfig (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask,
  IN  UINT8         *const  Mode3Banks,
  IN  UINT8         *const  Mode3UpperBanks,
  IN  BOOLEAN       *const  StopOnRaster,
  IN  BOOLEAN       *const  RasterRepoClear,
  IN  UINT8         *const  RasterRepoMode,
  IN  UINT8         *const  Mode3Max
  );

/**
  This function Reads the Repo Content registers, and returns those values in an input array.

  @param[in]  MrcData      - Pointer to MRC global data.
  @param[in]  Controller   - Controller to work on
  @param[in]  Channel      - Channel to work on
  @param[out] RepoStatus   - Contains the data from Repo Content registers, for 8 normal entries and 2 ECC entries

  @retval nothing
**/
void
Cpgc20ReadRasterRepoContent (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  OUT UINT64_STRUCT         RepoStatus[CPGC20_MAX_RASTER_REPO_CONTENT]
  );

/**
  This function reads the Raster Repo Status register and parses the individual fields into the input pointers.

  @param[in]   MrcData        - Pointer to MRC global data.
  @param[in]   Controller     - Controller to work on
  @param[in]   Channel        - Channel to work on
  @param[out]  BitmapValid    - Optional pointer, if provided it will be updated with indication of valid bits in Repo Status
  @param[out]  SummaryValid   - Optional pointer, if provided it will be updated with indication that test summary in Raster Repo is valid
  @param[out]  FailExcessAll  - Optional pointer, if provided it will be updated with indication of any bank that observed a number of failures that exceeded MAX_BANKS setting
  @param[out]  FailAnyAll     - Optional pointer, if provided it will be updated with indication if any bank failed
  @param[out]  NumErrLogged   - Optional pointer, if provided it will be updated with number of errors logged
  @param[out]  RasterRepoFull - Optional pointer, if provided it will be updated with indication of RasterRepo being full

  @retval nothing
**/
void
Cpgc20ReadRasterRepoStatus (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 Controller,
  IN  UINT8                 Channel,
  OUT UINT8         *const  BitmapValid,
  OUT UINT8         *const  SummaryValid,
  OUT UINT8         *const  FailExcessAll,
  OUT UINT8         *const  FailAnyAll,
  OUT UINT8         *const  NumErrLogged,
  OUT UINT8         *const  RasterRepoFull
  );

/**
  This function configures the CPGC Data Control register.

  @param[in]  MrcData                   - Pointer to MRC global data.
  @param[in]  DataSelectRotationRepeats - Number of times to repeat a Data_Instruction with Data_Select_Rotation_Enable
  @param[in]  SplitBackground           - Background mode.

  @retval Nothing.
**/
void
Cpgc20SetDataControl (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                DataSelectRotationRepeats,
  IN  UINT32                SplitBackground
  );

/**
  This function configures the CPGC Base Address Control register based on TestType.

  @param[in] MrcData        - Pointer to MRC global data.
  @param[in] TestType       - Memory test type that is currently being run

  @retval Nothing.
**/
void
AmtSetBaseAddressControl (
  IN  MrcParameters *const      MrcData,
  IN  PprTestTypeOffset         TestType
  );

/**
  This function sets the Pattern Generators' Alternate Dpat Buffer Control for use with AMT tests.
  @param[in]  MrcData             - Pointer to global MRC data.
  @param[in]  Controller          - Desired Memory Controller.
  @param[in]  Channel             - Desired Channel.
  @param[in]  Start               - Lane to start from
  @param[in]  Stop                - Lane to wrap ("stop") on
**/
VOID
AmtSetDpatAltBufCtl (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 Controller,
  IN  UINT8                 Channel,
  IN  UINT8                 Start,
  IN  UINT8                 Stop
  );

/**
  This function programs Bank Logical to Physical mapping for single-bank tests and for LP5 16-bank mode.
  The register will be updated on all enabled CPGC engines.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Rank              - Rank to work on
  @param[in]  UseSingleBank     - Whether to program a single bank; if false, will program all banks in order without BG calculation
  @param[in]  CpgcBank          - Cpgc address of bank to work on, only valid if UseSingleBank = TRUE
**/
VOID
AmtUpdateL2PBankMappingWithoutBG (
  IN  MrcParameters* const  MrcData,
  IN  UINT32                Rank,
  IN  BOOLEAN               UseSingleBank,
  IN  UINT32                CpgcBank
  );

#endif // _BlueMrcAmt_h_
