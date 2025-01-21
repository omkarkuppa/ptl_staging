/** @file
  THC config HOB definition.

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
  
@par Specification
**/

#ifndef _THC_CONFIG_HOB_H_
#define _THC_CONFIG_HOB_H_

#include <Uefi/UefiBaseType.h>
#include <ThcConfig.h>

/**
  The THC_CONFIG_HOB describes settings for PCH.
**/
typedef struct {
  THC_PORT             ThcPort[2];       ///< Port Configuration
} THC_CONFIG_HOB;

#endif
