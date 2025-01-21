/** @file
  @todo ADD DESCRIPTION

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _PEI_PLATFORM_LIB_H_
#define _PEI_PLATFORM_LIB_H_



#define PEI_DEVICE_DISABLED 0
#define PEI_DEVICE_ENABLED  1

typedef struct {
  UINT8   Register;
  UINT32  Value;
} PCH_GPIO_DEV;

//
// GPIO Initialization Data Structure
//
typedef struct{
  PCH_GPIO_DEV Use_Sel;
  PCH_GPIO_DEV Use_Sel2;
  PCH_GPIO_DEV Use_Sel3;
  PCH_GPIO_DEV Io_Sel;
  PCH_GPIO_DEV Io_Sel2;
  PCH_GPIO_DEV Io_Sel3;
  PCH_GPIO_DEV Lvl;
  PCH_GPIO_DEV Lvl2;
  PCH_GPIO_DEV Lvl3;
  PCH_GPIO_DEV Inv;
  PCH_GPIO_DEV Blink;
  PCH_GPIO_DEV Rst_Sel;
  PCH_GPIO_DEV Rst_Sel2;
} GPIO_INIT_STRUCT;

#endif
