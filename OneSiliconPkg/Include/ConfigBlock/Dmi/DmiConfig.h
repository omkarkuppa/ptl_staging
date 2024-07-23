/** @file
  Config block definition for PCH DMI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef __DMI_CONFIG_H__
#define __DMI_CONFIG_H__

#include <ConfigBlock.h>
#include <PchPcieRpConfig.h>

extern EFI_GUID gDmiConfigGuid;
#define DMI_CONFIG_REVISION 1

#pragma pack (push,1)

typedef struct {
  CONFIG_BLOCK_HEADER        Header;
  PCH_PCIE_ROOT_PORT_CONFIG  DmiConfig;
} DMI_CONFIG;

#pragma pack (pop)

#endif // __DMI_CONFIG_H__
