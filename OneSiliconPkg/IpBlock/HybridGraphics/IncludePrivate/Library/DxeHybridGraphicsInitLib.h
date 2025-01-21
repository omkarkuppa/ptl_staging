/** @file
  Header file for the HybridGraphics Dxe driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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
#ifndef _HYBRID_GRAPHICS_INIT_LIB_H_
#define _HYBRID_GRAPHICS_INIT_LIB_H_

/**
  Initialize the HybridGraphics support (DXE).

  @retval EFI_SUCCESS          - HybridGraphics initialization complete
  @retval EFI_OUT_OF_RESOURCES - Unable to allocated memory
  @retval EFI_NOT_FOUND        - HG InfoHob not found
  @retval EFI_DEVICE_ERROR     - Error Accessing HG GPIO
**/
EFI_STATUS
DxeHybridGraphicsInit (
  VOID
  );

#endif
