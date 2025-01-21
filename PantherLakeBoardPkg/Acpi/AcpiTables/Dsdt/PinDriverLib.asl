/** @file
  ACPI functions that drive pin states

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

#define PIN_GPIO_PAD_INDEX  0
#define PIN_GPIO_POLARITY_INDEX  1

//
// This library provides functions that configure pin states.
//
// GPIO package definition:
// Package () {
//   GpioPad : IntObj
//   GpioActiveStatePolarity : IntObj
//   }
//

//
// GPIO library imports
//

Scope (\) {

  //
  // Note this is not a real power resource we simply use this to declare new named scope
  //
  PowerResource (PIN, 0, 0) {
    //
    // Check pin status function
    // @param[in]  Arg0  Pin descriptor package
    //
    // @return GPIO pin status relative to GpioActiveStatePolarity value
    //
    Method (STA, 1, Serialized) {
      If (LEqual (\_SB.GGOV (DeRefOf (Index (Arg0, PIN_GPIO_PAD_INDEX))), DeRefOf (Index (Arg0, PIN_GPIO_POLARITY_INDEX)))) {
        Return (1)
      } Else {
        Return (0)
      }
    }

    //
    // Drive pin to active state function.
    // @param[in]  Arg0  Pin descriptor package
    //
    Method (ON, 1, Serialized) {
      ADBG (Concatenate ("PINH GPIO=", ToHexString (DeRefOf (Index (Arg0, PIN_GPIO_PAD_INDEX)))))
      \_SB.SGOV (DeRefOf (Index (Arg0, PIN_GPIO_PAD_INDEX)), DeRefOf (Index (Arg0, PIN_GPIO_POLARITY_INDEX)))
    }

    //
    // Drive pin to inactive state.
    // @param[in]  Arg0  Pin descriptor package
    //
    Method (OFF, 1, Serialized) {
      ADBG (Concatenate ("PINL GPIO=", ToHexString (DeRefOf (Index (Arg0, PIN_GPIO_PAD_INDEX)))))
      \_SB.SGOV (DeRefOf (Index (Arg0, PIN_GPIO_PAD_INDEX)), Not (DeRefOf (Index (Arg0, PIN_GPIO_POLARITY_INDEX))))
    }

    // Dummy standard methods for OSPM.
    // PowerResource is required to implement those.
    Method (_STA) {Return (0)}
    Method (_ON) {}
    Method (_OFF) {}
  }

  If (LNotEqual (PCHA, 0)) {
    //
    // Note this is not a real power resource we simply use this to declare new named scope
    //
    PowerResource (PPIN, 0, 0) {
      //
      // Check pin status function
      // @param[in]  Arg0  Pin descriptor package
      //
      // @return GPIO pin status relative to GpioActiveStatePolarity value
      //
      Method (STA, 1, Serialized) {
        If (LEqual (\_SB.GGOV (DeRefOf (Index (Arg0, PIN_GPIO_PAD_INDEX))), DeRefOf (Index (Arg0, PIN_GPIO_POLARITY_INDEX)))) {
          Return (1)
        } Else {
          Return (0)
        }
      }

      //
      // Drive pin to active state function.
      // @param[in]  Arg0  Pin descriptor package
      //
      Method (ON, 1, Serialized) {
        ADBG (Concatenate ("PINH GPIO=", ToHexString (DeRefOf (Index (Arg0, PIN_GPIO_PAD_INDEX)))))
        \_SB.SGOV (DeRefOf (Index (Arg0, PIN_GPIO_PAD_INDEX)), DeRefOf (Index (Arg0, PIN_GPIO_POLARITY_INDEX)))
      }

      //
      // Drive pin to inactive state.
      // @param[in]  Arg0  Pin descriptor package
      //
      Method (OFF, 1, Serialized) {
        ADBG (Concatenate ("PINL GPIO=", ToHexString (DeRefOf (Index (Arg0, PIN_GPIO_PAD_INDEX)))))
        \_SB.SGOV (DeRefOf (Index (Arg0, PIN_GPIO_PAD_INDEX)), Not (DeRefOf (Index (Arg0, PIN_GPIO_POLARITY_INDEX))))
      }

      // Dummy standard methods for OSPM.
      // PowerResource is required to implement those.
      Method (_STA) {Return (0)}
      Method (_ON) {}
      Method (_OFF) {}
    }
  }
}

