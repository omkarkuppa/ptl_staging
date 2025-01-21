/** @file
  SMBUS config HOB definition.

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

#ifndef _SMBUS_CONFIG_HOB_H_
#define _SMBUS_CONFIG_HOB_H_

#include <Uefi/UefiBaseType.h>
#include <SmbusConfig.h>

/**
  PCH SMBUS config HOB settings.
**/
typedef struct {
  UINT8 NumRsvdSmbusAddresses; ///< The number of elements in the RsvdSmbusAddressTable.
  UINT8 RsvdSmbusAddressTable[PCH_MAX_SMBUS_RESERVED_ADDRESS];
} SMBUS_CONFIG_HOB;

#endif
