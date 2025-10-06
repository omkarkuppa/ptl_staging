/** @file
  Header file for GpioPwmLib

@copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.
  This software and the related documents are Intel copyrighted materials, and your
  use of them is governed by the express license under which they were provided to
  you ("License"). Unless the License provides otherwise, you may not use, modify,
  copy, publish, distribute, disclose or transmit this software or the related
  documents without Intel's prior written permission.
  This software and the related documents are provided as is, with no express or
  implied warranties, other than those that are expressly stated in the License.

@par Specification Reference:
**/

#ifndef _GPIO_PWM_LIB_H_
#define _GPIO_PWM_LIB_H_

#include <Base.h>
#include <Uefi.h>

//
// GPIO PWM Constants
//
#define GPIO_PWM_DUTY_CYCLE_MIN     0    ///< Minimum duty cycle (0%)
#define GPIO_PWM_DUTY_CYCLE_MAX     100  ///< Maximum duty cycle (100%)

//
// GPIO PWM State Definitions
//
typedef enum {
  GPIO_PWM_STATE_OFF = 0,  ///< PWM is disabled/off
  GPIO_PWM_STATE_ON  = 1   ///< PWM is enabled/on
} GPIO_PWM_STATE;

/**
  Configures the PWM (Pulse Width Modulation) control settings for a GPIO controller.
  
  This function sets the PWM control parameters for a GPIO controller, including the duty cycle,
  frequency, and the enable state. Depending on the enable state, it either disables the PWM or
  configures the PWM with the specified duty cycle and frequency.
  
  Note: The caller must ensure the target GPIO pad is configured in Native2 mode before calling
  this function.
  
  @param[in]  DutyCycle    The duty cycle for the PWM signal, expressed as a percentage
                           (GPIO_PWM_DUTY_CYCLE_MIN to GPIO_PWM_DUTY_CYCLE_MAX), where 
                           GPIO_PWM_DUTY_CYCLE_MAX represents a fully on signal.
  @param[in]  Frequency    The frequency for the PWM signal, where the unit is platform
                           dependent and defined by the granularity of the base unit
                           register. This value is used as a divisor.
  @param[in]  EnableState  The desired state of the PWM signal, which can be either
                           GPIO_PWM_STATE_OFF to disable PWM or GPIO_PWM_STATE_ON to
                           enable PWM with the specified parameters.
                           
  @retval EFI_SUCCESS           The PWM control settings were successfully applied to the GPIO
                                controller.
  @retval EFI_INVALID_PARAMETER Invalid input parameter (DutyCycle > 100 or invalid EnableState).
  @retval EFI_ERROR             An error occurred while trying to build the GPIO access
                                structure, and the PWM settings could not be applied.
**/
EFI_STATUS
EFIAPI
GpioSetPwmControl (
  IN  UINT8             DutyCycle,
  IN  UINT32            Frequency,
  IN  GPIO_PWM_STATE    EnableState
  );

#endif // _GPIO_PWM_LIB_H_
