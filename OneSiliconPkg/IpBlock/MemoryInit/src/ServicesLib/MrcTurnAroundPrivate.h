/** @file
    This file contains all the turnaround training algorithm definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#ifndef MrcTurnaround_h_
#define MrcTurnaround_h_

/**
  This function returns tCCD_L for LPDDR.

  @param[in]  MrcData - Pointer to MRC global data.

  @returns nWR_diff parameter.
**/
UINT32
GetLpddrtCCDL (
  IN MrcParameters *const MrcData
  );

/**
  This function returns tCCD_S for LPDDR.

  @param[in]  MrcData - Pointer to MRC global data.

  @returns nWR_diff parameter.
**/
UINT32
GetLpddrtCCDS (
  IN MrcParameters *const MrcData
  );

#endif // MrcTurnAroundPrivate_h_
