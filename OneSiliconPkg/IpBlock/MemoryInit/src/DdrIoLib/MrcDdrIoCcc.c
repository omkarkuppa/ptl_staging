/** @file
  This file contains project independent code related to initializing and
  configuring the DDRIO CCC Partition.

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

**/

#include "MrcDdrIoApi.h"  // for prototypes
#include "MrcCommon.h"  // for MrcChannelExist()
#include "MrcHalRegisterAccess.h"  // for MrcGetSetLimits()
#include "MrcDdrIoPiSerializer.h" // for DCC PI Serializer support functions
#include "MrcDdrIoComp.h"

static const UINT8 CccDdr5IlMap[MRC_CCC_NUM] = {
  0,  // CCC0
  2,  // CCC1
  1,  // CCC2
  3,  // CCC3
  4,  // CCC4
  6,  // CCC5
  5,  // CCC6
  7,  // CCC7
};

/**
  This function translates HW CCC Partition instance to RAL instance based on interleave configuration.

  @param[in] MrcData     - MRC global data.
  @param[in] CccInstance - CCC instance to be translated.

  @retval Translated CCC instance.
**/
UINT8
MrcTranslateCccInstance (
  IN MrcParameters *const MrcData,
  IN UINT8          const CccInstance
  )
{
  MrcInput *Inputs;
  UINT8    CccTranslatedInstance;

  Inputs  = &MrcData->Inputs;

  if (Inputs->ExtInputs.Ptr->CccPinsInterleaved) {
    CccTranslatedInstance = CccDdr5IlMap[CccInstance];
  } else {
    CccTranslatedInstance = CccInstance;
  }
  return CccTranslatedInstance;
}

