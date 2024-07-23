/** @file
  Silicon Config HOB is used for gathering platform
  related Intel silicon information and config setting.

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
#ifndef _SI_CONFIG_HOB_H_
#define _SI_CONFIG_HOB_H_

#include <SiPolicyStruct.h>

extern EFI_GUID gSiConfigHobGuid;

// Rename SI_CONFIG_HOB into SI_CONFIG_HOB_DATA for it does not follow HOB structure.
typedef CONST SI_CONFIG SI_CONFIG_HOB_DATA;
#endif
