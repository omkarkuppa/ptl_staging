/** @file
  Declares the interface for TbtNvmDrvRetimerThruHr class.
  This class is in charge of providing the way to access the retimer
  through TBT integrated HR.

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

#ifndef TBT_NVM_DRV_RETIMER_THRU_HR_PROTOCOL_
#define TBT_NVM_DRV_RETIMER_THRU_HR_PROTOCOL_

#include <Library/TbtNvmDrvUtils.h>
#include <Protocol/TbtNvmDrvDevice.h>
#include <Library/TbtNvmDrvHr.h>

/// A struct to store the required fields during the operation
typedef struct {
  USB4_LANE         TbtPort;
  UINT32            CascadedRetimerIndex;
  TBT_HOST_ROUTER   *Hr;
  TBT_RETIMER       *Comm;
} RETIMER_THRU_HR;

#endif /* TBT_NVM_DRV_RETIMER_THRU_HR_PROTOCOL_ */
