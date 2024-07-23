/** @file
  HOB definition to be used by DTBT.

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

#ifndef _DTBT_INFO_GUID_H_
#define _DTBT_INFO_GUID_H_
#include <PeiDTbtGenericStructure.h>
#include <DTbtDefinition.h>

#pragma pack(1)
//
// DTBT Info HOB
//
typedef struct _DTBT_INFO_HOB {
  EFI_HOB_GUID_TYPE      EfiHobGuidType;
  DTBT_GENERIC_CONFIG    DTbtGenericConfig;                                 ///< dTbt Generic Configuration
  DTBT_CONTROLLER_CONFIG DTbtControllerConfig [MAX_DTBT_CONTROLLER_NUMBER]; ///< dTbt Controller Configuration
} DTBT_INFO_HOB;
#pragma pack()

#endif
