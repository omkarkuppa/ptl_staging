/** @file
  This file contains memory scrubbing and alias checking related information.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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
#ifndef _MrcMemoryScrub_h_
#define _MrcMemoryScrub_h_
#pragma pack (push, 1)


#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "MrcCommon.h"
#include "MrcPostCodes.h"

/**
  Zero out all of the memory.
  This function is used in the following cases:
  - ECC is enabled (to initialize ECC logic)
  - TXT library is asking to scrub the memory (instead of SCLEAN ACM)
  - Platform code is asking to scrub the memory due to MOR bit being set (Memory Override Request)

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess if the clean succeeded, otherwise an error status.
**/
extern
MrcStatus
MrcEccClean (
  IN     MrcParameters *const MrcData
  );

/**
  This function performs a memory alias check.

  @param[in] MrcData - The global host structure

  @retval mrcSuccess or error value.
**/
extern
MrcStatus
MrcAliasCheck (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function provides MRC core hook to call TXT Alias Check before DRAM Init Done.

  @param[in]  MrcData - Pointer to global MRC structure.

  @retval mrcSuccess.
**/
MrcStatus
MrcTxtAliasCheck (
  IN MrcParameters *const MrcData
  );

/**
  Programs REFRESH ctrl register to run refresh.
  @param[in] MrcData           - Pointer to MRC global data.
  @param[in] Controller        - Controller to enable.
  @param[in] Channel           - Channel to enable.
  @param[in] RankBitMask       - Bit mask of ranks to enable.
  @retval Nothing
**/
void
EnableRefresh (
  IN MrcParameters *const MrcData,
  IN const UINT8          Controller,
  IN const UINT8          Channel,
  IN UINT8                RankBitMask
  );

/**
  This function provides Error Check and Scrub (ECS) implementation.

  DDR5 ECS mode allows the DRAM to internally read, correct single bit errors, and write back corrected data bits to the array (scrub errors)
  while providing transparency to error counts.
  PTL MC only supports the automatic mode (Manual mode is not supported): The operation is done internal to DRAM.

  Enable "error scrubbing" DDR5 feature done by configuring the follwoing registers:
    rfp_0_0_0_mchbar.ecs_refab_enable
    rfp_0_0_0_mchbar.ecs_refab_period

  @param[in] MrcData - Include all MRC global data.

  @retval Nothing.
**/
VOID
MrcEcsClean (
  IN     MrcParameters* const MrcData
);

#pragma pack (pop)
#endif
