/** @file
  This file defines the EC NVS Area Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _EC_NVS_AREA_H_
#define _EC_NVS_AREA_H_

//
// EC NVS Area definition
//
#include <EcNvsAreaDef.h>

/**
  This protocol is used to sync EC information from POST to runtime ASL.
  This protocol exposes the pointer of EC NVS Area only. Please refer to
  ASL definition for EC NVS AREA.
**/
typedef struct {
  EC_NVS_AREA                          *Area;
} EC_NVS_AREA_PROTOCOL;

#endif
