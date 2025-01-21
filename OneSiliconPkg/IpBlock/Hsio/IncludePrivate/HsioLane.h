/** @file
  HSIO Lane definition

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#ifndef _HSIO_LANE_H_
#define _HSIO_LANE_H_

#include <Ppi/SiPolicy.h>
#include <RegisterAccess.h>

/**
  Represents HSIO lane
**/
typedef struct {
  UINT8            Index;       ///< Lane index
  UINT8            Pid;         ///< Sideband ID - It's used for Chipset Init tables entries
  UINT16           Base;        ///< Lane base registers offset
  UINT16           TxBase;      ///< Tx base registers offset
  UINT16           RxBase;      ///< Rx base registers offset
  REGISTER_ACCESS  *P2SbAccess; ///< Sideband access
} HSIO_LANE;

#endif // _HSIO_LANE_H_
