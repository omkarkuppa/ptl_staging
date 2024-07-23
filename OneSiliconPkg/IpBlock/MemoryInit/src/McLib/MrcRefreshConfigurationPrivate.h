/** @file
  This header exposes private members of MrcAddressDecodeConfiguration such that
  they can be used in Unit Testing

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
#ifndef MrcRefreshConfigurationPrivate_h_
#define MrcRefreshConfigurationPrivate_h_

#include "CMrcInterface.h"  // for MrcParameters

// DO NOT REMOVE: Ensures this header is only included once in the code
PRIVATE_HEADER_DEFENDER(MrcRefreshConfigurationPrivate)

/**
  This function configures the Self Refresh Timing Parameters for the MC:
  tXS-offset, tXSDLL, and tMOD.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Controller to configure.
  @param[in] Channel    - Channel to configure.

  @retval Nothing.
**/
void
SetTcSrftpReg (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  );

#endif // MrcRefreshConfigurationPrivate_h_
