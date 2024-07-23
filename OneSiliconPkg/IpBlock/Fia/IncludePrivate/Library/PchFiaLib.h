/** @file
  Header file for PchFiaLib.

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
#ifndef _PCH_FIA_LIB_H_
#define _PCH_FIA_LIB_H_

#define PCH_FIA_NO_CLKREQ  0xFF

#include <RegisterAccess.h>

typedef enum {
  PchFiaOwnerUsb3 = 0,
  PchFiaOwnerDmi,
  PchFiaOwnerPcie,
  PchFiaOwnerSata,
  PchFiaOwnerGbe,
  PchFiaOwnerUfs,
  PchFiaOwnerTsn,
  PchFiaOwnerInvalid
} PCH_FIA_LANE_OWNER;

typedef enum {
  FiaClkOutputDelay0us,
  FiaClkOutputDelay5us,
  FiaClkOutputDelay10us,
  FiaClkOutputDelay15us
} FIA_CLOCK_OUTPUT_DELAY;

typedef struct {
  REGISTER_ACCESS       *RegisterAccess;
} FIA_INSTANCE;

#endif // _PCH_FIA_LIB_H_
