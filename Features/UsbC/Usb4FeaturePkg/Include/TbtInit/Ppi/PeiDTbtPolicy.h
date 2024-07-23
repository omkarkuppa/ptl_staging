/** @file
  TBT PEI Policy

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

@par Specification
**/

#ifndef _PEI_DTBT_POLICY_H_
#define _PEI_DTBT_POLICY_H_

#include <PeiDTbtGenericStructure.h>
#include <DTbtDefinition.h>

#pragma pack(push, 1)

#define PEI_DTBT_POLICY_REVISION 1

/**
 DTBT PEI configuration\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct _PEI_DTBT_POLICY {
  DTBT_GENERIC_CONFIG    DTbtGenericConfig;                                 ///< dTbt Common Configuration
  DTBT_CONTROLLER_CONFIG DTbtControllerConfig [MAX_DTBT_CONTROLLER_NUMBER]; ///< dTbt Controller Configuration
} PEI_DTBT_POLICY;

#pragma pack(pop)

#endif
