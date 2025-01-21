/** @file
  This module configures the memory controller address decoder.

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
#ifndef _MrcAddressConfiguration_h_
#define _MrcAddressConfiguration_h_


#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "CMcAddress.h"
#include "MrcCommon.h"
#include "MrcDebugPrint.h"

/**
  This function is the main address decoding configuration function.

  @param[in] MrcData - Include all MRC global data.

  @retval Nothing.

**/
extern
void
MrcAdConfiguration (
  IN MrcParameters *const MrcData
  );

/**
  This function configures the MAD_DIMM_CH0/1 register.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Controller to configure.
  @param[in] Channel    - Channel to configure.

  @retval Nothing.
**/
void
ChannelAddressDecodeConfiguration (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  );

/**
  This function checks the possiblity of enabling

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - MC Controller ID.

  @retval True/False    - True: enable EBH, False: disable EBH.
**/

BOOLEAN
MrcEnableExtendedBankHashing (
  IN MrcParameters *const MrcData,
  IN     UINT32           Controller
);

#endif
