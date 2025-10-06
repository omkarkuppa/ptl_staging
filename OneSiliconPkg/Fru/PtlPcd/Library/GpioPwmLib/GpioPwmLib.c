/** @file
  Source code file for GpioPwmLib

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/GpioPwmLib.h>
#include <Library/PchPcrLib.h>
#include <PcrDefine.h>
#include <Register/GpioRegs.h>
#include <PtlPcdSbPortIds.h>
#include <Register/GpioV2PcdRegs.h>
 
 

/**
  Configures the PWM (Pulse Width Modulation) control settings for a GPIO controller.
  This function sets the PWM control parameters for a GPIO controller, including the duty cycle,
  frequency, and the enable state. Depending on the enable state, it either disables the PWM or
  configures the PWM with the specified duty cycle and frequency.
  @param[in]  DutyCycle    The duty cycle for the PWM signal, expressed as a percentage
                           (0-100), where 100 represents a fully on signal.
  @param[in]  Frequency    The frequency for the PWM signal, where the unit is platform
                           dependent and defined by the granularity of the base unit
                           register.
  @param[in]  EnableState  The desired state of the PWM signal, which can be either
                           GPIO_PWM_STATE_OFF to disable PWM or GPIO_PWM_STATE_ON to
                           enable PWM with the specified parameters.
  @retval EFI_SUCCESS      The PWM control settings were successfully applied to the GPIO
                           controller.
  @retval EFI_ERROR        An error occurred while trying to build the GPIO access
                           structure, and the PWM settings could not be applied.
**/
EFI_STATUS
EFIAPI
GpioSetPwmControl (
  IN   UINT8              DutyCycle,
  IN   UINT32             Frequency,
  IN   GPIO_PWM_STATE     EnableState
  )
{
  UINT32                  RegisterOffset;
  P2SB_PORT_16_ID         P2SBPid;

  //
  // WCL_SID_F2_PID_GPCOM5 is same as PTL_SID_F2_PID_GPCOM5 (0xF25D)
  //
  P2SBPid.Pid16bit = PTL_SID_F2_PID_GPCOM5;
  RegisterOffset = R_GPIOV2_PTL_PCD_PCR_PWMC;
 
  if (EnableState == GPIO_PWM_STATE_OFF) {
    PchPcrAndThenOr32 (P2SBPid.PortId.LocalPid, RegisterOffset, (UINT32) ~B_GPIO_PCR_PWMC_EN, (UINT32) B_GPIO_PCR_PWMC_SWUP);
  } else {
    PchPcrAndThenOr32 (
      P2SBPid.PortId.LocalPid,
      RegisterOffset,
      0,
      (((DutyCycle * 256) / 100) & B_GPIO_PCR_PWMC_ONTIMEDIV)                 |
      ((Frequency << N_GPIO_PCR_PWMC_BASEUNIT) & B_GPIO_PCR_PWMC_BASEUNIT)    |
      B_GPIO_PCR_PWMC_SWUP                                                    |
      B_GPIO_PCR_PWMC_EN
    );
  }

  return EFI_SUCCESS;
}