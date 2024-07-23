/** @file
  Header file for GPIO PWM Ppi and Protocol definitions.
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
#ifndef _GPIOV2_PWM_
#define _GPIOV2_PWM_

#include <Uefi/UefiBaseType.h>
#include <Base.h>
#include <GpioV2Services.h>

typedef struct _GPIOV2_PWM          GPIOV2_PWM;


/**
  This procedure starts output of PWM signal. Before this function 
  call GPIO pad should be already set to "Blink" pad mode.
  @param[in] GpioPwm              Gpio Pwm Services

  @retval EFI_SUCCESS             The function completed successfully
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_PWM_ON) (
  GPIOV2_PWM  *GpioPwm
  );


/**
  This procedure stops output of PWM signal.
  @param[in] GpioPwm              Gpio Pwm Services

  @retval EFI_SUCCESS             The function completed successfully
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_PWM_OFF) (
  GPIOV2_PWM  *GpioPwm
  );


/**
  This procedure sets frequency of PWM signal.
  @param[in] GpioPwm              Gpio Pwm Services
  @param[in] Frequency            Fpwm = Frequency * 7.8125 [mHz], max 32.767 [kHz]

  @retval EFI_SUCCESS             The function completed successfully
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_PWM_SET_FREQ) (
  GPIOV2_PWM  *GpioPwm,
  UINT32      Frequency
  );


/**
  This procedure sets duty cycle of PWM signal.
  @param[in] GpioPwm              Gpio Pwm Services
  @param[in] DutyCycle            Percentage value of PWM duty Cycle, max 99%
  
  @retval EFI_SUCCESS             The function completed successfully
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_PWM_SET_DUTY) (
  GPIOV2_PWM  *GpioPwm,
  UINT32      DutyCycle
  );


struct _GPIOV2_PWM {
  UINT8                  Hid[GPIOV2_CONTROLLER_HID_LENGTH];
  UINT32                 Uid;

  GPIO_PWM_ON            PwmOn;
  GPIO_PWM_OFF           PwmOff;
  GPIO_PWM_SET_FREQ      PwmSetFreq;
  GPIO_PWM_SET_DUTY      PwmSetDuty;
};


#endif // _GPIOV2_PWM_
