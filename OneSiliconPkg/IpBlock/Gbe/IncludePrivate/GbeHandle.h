/** @file
  SATA routines

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _GBE_HANDLE_H_
#define _GBE_HANDLE_H_

#include <ConfigBlock.h>
#include <GbeController.h>
#include <GbeConfig.h>

/**
  Internal GBE policy options
**/
typedef struct {
  /**
    Corresponds to the "WOL Enable Override" bit in the General PM Configuration B (GEN_PMCON_B) register. 0: Disable; <b>1: Enable</b>.
   **/
  BOOLEAN               WolEnableOverride;

  /**
    Description: Indicates GBE controller interrupt pin is supported
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               InterruptPinSupported;

  /**
    Description: GBE controller interrupt pin
    Possible values: 0 - 255
  **/
  UINT8                 InterruptPin;

  /**
    Description: Lane number that is owned by GbE

    Value 0xFF means no lane assigned
  **/
  UINT8                 LaneNum;

  /**
   * Description: GBE PCIe source clock usage
   */
  BOOLEAN               HasClkReq;

  /**
   * Description: Sle flow enabled
   */
  BOOLEAN               SleEnabled;

  /**
  * Description: Lock Write Flash Supported
  */
  BOOLEAN               LockWriteFlashSupported;
} GBE_PRIVATE_CONFIG;

/**
  This function checks if GbE controller is supported (not disabled by fuse)

  @param[in]  GbeController      Pointer to GBE controller structure

  @retval GbE support state
**/
typedef
BOOLEAN
(*GBE_IS_SUPPORTED) (
  IN  GBE_CONTROLLER *GbeController
  );

/**
  This function checks if GbE is function disabled
  by static power gating

  @param[in]  GbeController      Pointer to GBE controller structure

  @retval GbE device state
**/
typedef
BOOLEAN
(*GBE_IS_DISABLED) (
  IN  GBE_CONTROLLER *GbeController
  );

/**
  This function enables GbE device by disabling static power gating.
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.

  @param[in]  GbeController      Pointer to GBE controller structure
**/
typedef
VOID
(*GBE_ENABLE) (
  IN  GBE_CONTROLLER *GbeController
  );

/**
  This function disables GbE device by static power gating and enables ModPHY SPD gating (PCH-LP only).
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.

  @param[in]  GbeController      Pointer to GBE controller structure
**/
typedef
VOID
(*GBE_STATIC_DISABLE) (
  IN  GBE_CONTROLLER *GbeController
  );

/**
  This function enables GBE ModPHY SPD gating.

  @param[in]  GbeController  Pointer to GBE controller structure
**/
typedef
VOID
(*GBE_MOD_PHY_POWER_GATING) (
  IN  GBE_CONTROLLER *GbeController
  );

/**
  Disables one PCIe clock.

  @param[in] GbeController     Pointer to GBE controller structure
**/
typedef
VOID
(*GBE_DISABLE_CLK) (
  IN  GBE_CONTROLLER *GbeController
  );

/**
  GBE callbacks
  List of function pointers can be passed to IP Block driver
**/
typedef struct {
  GBE_IS_SUPPORTED           IsSupported;
  GBE_IS_DISABLED            IsFunctionDisabled;
  GBE_ENABLE                 Enable;
  GBE_STATIC_DISABLE         Disable;
  GBE_MOD_PHY_POWER_GATING   ModPhyPowerGating;
  GBE_DISABLE_CLK            DisableClk;
} GBE_CALLBACK;

/**
  GBE device structure
  Stores all data necessary to initialize GBE IP block
**/
typedef struct {
  GBE_CONTROLLER             *Controller;
  GBE_CONFIG                 *GbeConfig;
  GBE_PRIVATE_CONFIG         *PrivateConfig;
  GBE_CALLBACK               *Callback;
  /**
    Temporary Bar
  **/
  UINTN                      Mmio;
  UINTN                      PwrmBase;
} GBE_HANDLE;

#endif // _GBE_HANDLE_H_
