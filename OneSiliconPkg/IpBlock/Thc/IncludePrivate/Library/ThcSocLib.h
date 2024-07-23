/** @file
  Initializes THC Devices

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
#ifndef _THC_SOC_LIB_H_
#define _THC_SOC_LIB_H_

#include <ThcCtrl.h>
#include <Library/PchLimits.h>
#include <ConfigBlock.h>
#include <ThcConfig.h>
#include <Ppi/SiPolicy.h>

struct THC_HANDLE_;
typedef struct THC_HANDLE_ THC_HANDLE;

/**
  Internal THC policy options
**/
typedef struct {
  UINT16     HidDeviceid[PCH_MAX_THC_CONTROLLERS];
  UINT32     D0i2EntryTimer;
  BOOLEAN    ThcResourceDisabled;
  UINT8      InterruptPin[PCH_MAX_THC_CONTROLLERS];
  BOOLEAN    FrameCoalescingSupported;
  UINT32     PgcbSourceClockFrequency; // Hz Unit required to calculate Ten Microsecond Counter Value
} THC_PRIVATE_CONFIG;

/**
  Performs Device Id update for Hid Over SPI support

  @param[in]  ThcHandle         The THC Handle instance
  @param[in]  ThcIndex          The THC Controiller index
**/
typedef
VOID
(*THC_HID_OVER_SPI_ENABLE) (
  IN THC_HANDLE  *ThcHandle,
  IN UINT8       ThcIndex
  );

/**
  Configures GPIO pins for THC

  @param[in] ThcIndex       THC Index
  @param[in] ThcPort        THC Port
**/
typedef
VOID
(*THC_GPIO_ENABLE) (
  IN UINT32              ThcIndex,
  IN THC_PORT            *ThcPort
  );

/**
  Disables Resource control for THC

  @param[in]  ThcPcrAccess     Pointer to THC Controller structure
**/
typedef
VOID
(*THC_RESOURCE_CONTROL_DISABLE) (
  IN REGISTER_ACCESS     *ThcPcrAccess
  );

/**
  Disables THC controller in power controller and fabric

  @param[in]  ThcController     Pointer to THC Controller structure
**/
typedef
VOID
(*THC_CONTROLLER_DISABLE) (
  IN  THC_CONTROLLER    *ThcController
  );

/**
  THC callbacks
  List of function pointers can passed to IP Block driver
**/
typedef struct {
  THC_GPIO_ENABLE               ThcGpioEnable;
  THC_GPIO_ENABLE               ThcI2cGpioEnable;
  THC_HID_OVER_SPI_ENABLE       ThcHidOverSpiEnable;
  THC_HID_OVER_SPI_ENABLE       ThcHidOverI2cEnable;
  THC_RESOURCE_CONTROL_DISABLE  ThcResourceControlDisable;
  THC_CONTROLLER_DISABLE        ThcControllerDisable;
} THC_CALLBACK;

/**
  THC controller structure
  Stores all data necessary to initialize THC IP block
**/
struct THC_HANDLE_ {
  THC_CONTROLLER             Controller[PCH_MAX_THC_CONTROLLERS];
  THC_PORT                   ThcPortConfig[PCH_MAX_THC_CONTROLLERS];
  THC_PRIVATE_CONFIG         PrivateConfig;
  THC_CALLBACK               *Callback;
  UINT64                     Mmio;
  UINT8                      ThcCount;
};

/**
  Initialize the Intel Touch Host Controller

  @param[in] SiPolicy             Policy
**/
VOID
ThcSocInit (
  IN SI_POLICY_PPI      *SiPolicy
  );

#endif //_THC_SOC_LIB_H_
