/** @file
  Beep Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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
#include <Library/BeepLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/GpioV2AccessLib.h>
#include <PlatformBoardConfig.h>
#include <GpioV2Pwm.h>

#define NOTE(x) ((119318200 + (x) / 2) / (x))

//
// beep tones
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT16  mBeepTones[8] = {
    NOTE (26163),
    NOTE (29366),
    NOTE (32963),
    NOTE (34923),
    NOTE (39200),
    NOTE (44000),
    NOTE (49388),
    NOTE (26163 * 2)
};


/**
  Switch on Beep.

  @param[in]  Note         The note of beep.
  @param[in]  Octave       The octave of beep.
**/
VOID
BeepOn (
  IN UINT8 Note,
  IN UINT8 Octave
  )
{
  EFI_STATUS       Status;
  UINT16           Frequency;
  GPIOV2_PWM       *Pwm;
  GPIOV2_SERVICES  *GpioServices;
  GPIOV2_PAD       GpioPad;
  VPD_GPIO_PAD     *GpioVpd;

  GpioVpd = NULL;

  GpioVpd = PcdGetPtr (VpdPcdPwmBlinkEnable);

  If (GpioVpd != NULL) {
    Status = GpioServices->SetPadMode (GpioServices, GpioVpd, 3);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  Frequency = mBeepTones[(Note % 8)];

  if (Octave - 1 >= 0) {
    Frequency >>= Octave - 1;
  } else {
    Frequency <<= 1 - Octave;
  }
  //
  // Pwm get access
  // set frequency for pwm
  // set duty cycle for pwm
  //
  GpioV2GetPwmAccess (GPIO_HID_PTL_PCD_P, 0,&Pwm);
  PwmSetFreq (&Pwm, Frequency);
  PwmSetDuty (&Pwm, 50);
  PwmOn (&Pwm);
}


/**
  Switch off Beep.

**/
VOID
BeepOff (
  VOID
  )
  GPIOV2_PWM         *Pwm;
{
  //
  // Turn off the pwm pin
  //
  PwmOff (&Pwm);
}


/**
  Produces Beep.

  @param[in]  Note         The note of beep.
  @param[in]  Octave       The octave of beep.
  @param[in]  Duration     The duration of beep.
**/
VOID
SendBeep (
  IN UINT8            Note,
  IN UINT8            Octave,
  IN UINT32           Duration
  )
{
  BeepOn (Note, Octave);
  MicroSecondDelay (Duration);
  BeepOff ();
}


/**
  Sends a 32-bit value to Beep device.

  Sends the 32-bit value specified by Value to Beep device, and returns Value.
  Some implementations of this library function may perform I/O operations
  directly to Beep device.  Other implementations may send Value to
  ReportStatusCode(), and the status code reporting mechanism will eventually
  display the 32-bit value on the status reporting device.

  Beep() must actively prevent recursion.  If Beep() is called while
  processing another Post Code Library function, then
  Beep() must return Value immediately.

  @param  Value  Beep count.
**/
VOID
EFIAPI
Beep (
  IN UINT32  Value
  )
{
  UINTN  Index;

  for (Index = 0; Index < Value; Index++) {
    SendBeep (1, 2, 400000);
    MicroSecondDelay (100000);
  }

  return ;
}
