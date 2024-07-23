/** @file
  This module contains functions to configure and use Memory Controller
  maintenance features.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _MrcMaintenance_h_
#define _MrcMaintenance_h_

#include "MrcDdrCommon.h"

/**
  This function configures probeless config register.

  @param[in, out] MrcData - All the MRC global data.
**/
extern
void
MrcProbelessConfig (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function will disable the DQS Oscillator maintenance feature in the Memory Controller.

  @params[in] MrcData - Pointer to MRC global data.
**/
VOID
MrcDisableDqsOscillatorMc (
  IN  MrcParameters * const MrcData
  );

/**
  This function provides the initial configuration of the Memory Controller's
  maintenance services.  Some items are disabled during MRC training, and will need
  to be configured at the end of MRC.

  @params[in] MrcData - Pointer to MRC global data.
**/
VOID
MrcMaintenanceConfig (
  IN  MrcParameters * const MrcData
  );

#endif // _MrcMaintenance_h_
