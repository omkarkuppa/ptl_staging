/** @file
  LPSS I3C Handle definition.

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

#ifndef _I3C_HANDLE_H_
#define _I3C_HANDLE_H_

#include <LpssI3cController.h>
#include <ConfigBlock.h>
#include <LpssI3cConfig.h>
#include <Ppi/SiPolicy.h>

typedef struct _I3C_HANDLE LPSS_I3C_HANDLE;

/**
  Internal I3C policy options
**/
typedef struct {
  UINT8      InterruptPin;
  UINT8      Irq;
} I3C_PRIVATE_CONFIG;

/**
  I3C Handle structure
  Stores all data necessary to initialize LPSS I3C IP Block
**/
struct _I3C_HANDLE {
  LPSS_I3C_CONTROLLER    *Controller;
  LPSS_I3C_DEVICE_CONFIG *I3cConfig;
  I3C_PRIVATE_CONFIG     PrivateConfig;
  UINT8                  ControllerId;
};

#endif //_I3C_HANDLE_H_
