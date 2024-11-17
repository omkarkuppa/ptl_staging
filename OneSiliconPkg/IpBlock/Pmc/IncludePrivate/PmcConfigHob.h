/** @file
  PMC config HOB definition.

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

#ifndef _PMC_CONFIG_HOB_H_
#define _PMC_CONFIG_HOB_H_

#include <Uefi/UefiBaseType.h>

/**
  The PMC_CONFIG_HOB describes settings for PCH.
**/
typedef struct {
  UINT32 PsOnEnable             :  1; ///< Indicates if PS_ON support has been enabled, <b>0: Disable</b>; 1: Enable.
  UINT32 EnableTimedGpio0       :  1; ///< Enable Bit for Timed GPIO 0 <b>0 = disable</b>; 1 = enable
  UINT32 EnableTimedGpio1       :  1; ///< Enable Bit for Timed GPIO 1 <b>0 = disable</b>; 1 = enable
  UINT32 EnableDtrSci           :  1; ///< Enable Bit for DTR SCI <b>0 = disable</b>; 1 = enable
  UINT32 RsvdBits1              : 29;
} PMC_CONFIG_HOB;

#endif
