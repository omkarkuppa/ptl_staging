/** @file
  This file includes definitions needed for the MRC to call management
  functions which cause Blue/Green interaction.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#ifndef _MrcUcManagement_h_
#define _MrcUcManagement_h_
#include "CMrcInterface.h"  // for MrcParameters

// #define MRC_SIMICS_XTENSA_SUPPORT 1

/**
  Soft-reset the UC.

  @param[in] MrcData - Include all MRC global data.

**/
MrcStatus
MrcUcReset (
  IN MrcParameters *const MrcData
  );

/**
  Load the executable Green MRC payload into the UC's memory

  @param[in] MrcData - Include all MRC global data.

**/
MrcStatus
MrcUcLoadGreenCode (
  IN MrcParameters *const MrcData
  );

/**
  Load the MRC data structure used by Green MRC into the UC's memory

  @param[in] MrcData - Include all MRC global data.

**/
MrcStatus
MrcUcLoadGreenData (
  IN MrcParameters *const MrcData
  );

/**
  Execute the Green MRC payload currently loaded in the UC.

  @param[in] MrcData - Include all MRC global data.

**/
MrcStatus
MrcUcExecuteGreen (
  IN MrcParameters *const MrcData
  );

/**
  Copy the Green MRC's MrcData updates back to Blue MRC.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess.
**/
MrcStatus
MrcUpdateDataFromUc (
  IN MrcParameters *const MrcData
  );

#endif // _MrcUcManagement_h_
