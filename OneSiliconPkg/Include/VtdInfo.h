/** @file
  file for Vtd Info

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

@par Specification Reference:
**/
#ifndef _VTD_INFO_H_
#define _VTD_INFO_H_

//
// The order of members in the below enum represents the order of enabling
// bits in VT-d BAR register.
//

typedef enum {
  GFX_VTD     = 0,  // Individual VT-d for GFX device
  IGD_VTD     = 0,  // other name for Gfx VT-d engine
  NON_GFX_VTD = 1,  // Individual VT-d for non-GFX devices (NPU, IPU)
  GLOBAL_VTD  = 2,  // Global VT-d for Segment 0
  IOP_VTD     = 2,  // other name for Global VT-d engine
  VTD_ENGINE_NUMBER
} VTD_ENGINE;

#endif
