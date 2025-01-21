/** @file
  Definition of AmtForcePushPetHob data structures

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#ifndef _AMT_FORCE_PUSH_PET_HOB_H_
#define _AMT_FORCE_PUSH_PET_HOB_H_

#include <IntelAsfExtensions.h>

///
/// Data Hob for storing ASF message from PEI to DXE phase, in case sending ASF message command fails in PEI.
///
typedef struct {
  EFI_HOB_GUID_TYPE             EfiHobGuidType; ///< GUID Hob type header.
  ASF_FRAMEWORK_MESSAGE_TYPE    MessageType;    ///< ASF message type.
} AMT_FORCE_PUSH_PET_HOB;

#endif
