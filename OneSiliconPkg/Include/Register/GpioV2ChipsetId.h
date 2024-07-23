/** @file
  General GPIO ChipsetId for all platforms

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

#ifndef _GPIOV2_CHIPSET_ID_H_
#define _GPIOV2_CHIPSET_ID_H_

//
// MTL
//
#define GPIOV2_MTL_SOC_S_CHIPSET_ID     0xA
//
// PTL
//
#define GPIOV2_PTL_SOC_S_CHIPSET_ID     GPIOV2_MTL_SOC_S_CHIPSET_ID
#define GPIOV2_PTL_PCD_CHIPSET_ID     0xD
//
// IBL
//
#define GPIOV2_IBL_CHIPSET_ID           0xA

#endif // _GPIOV2_PAD_DEFINITION_H_
