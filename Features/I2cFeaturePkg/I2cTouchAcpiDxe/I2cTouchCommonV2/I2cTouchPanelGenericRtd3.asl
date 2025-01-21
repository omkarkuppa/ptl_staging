/** @file
  ACPI RTD3 I2C Touch Panel Power resource and PRxx, PSxx
  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.
**/

External(TPIN)
External(TPBR)
External(TPCT)
External(TPTY)
//
// Touch panel Pins
//
External(TPDI)
External(TPDP)
External(TPLS)
External (VRRD,FieldUnitObj)

External(\_SB.GHPO, MethodObj)
External(\_SB.SHPO, MethodObj)

// GPIO methods
External (\_SB.SGRA, MethodObj)
External (\_SB.GGOV, MethodObj)
External (\_SB.SGOV, MethodObj)
External (\_SB.SGII, MethodObj)
External (TPRD,FieldUnitObj)
External (VRRD,FieldUnitObj)

If (LAnd( LAnd( LNotEqual (TPTY,0), LLess (TPTY,0xFF)), LEqual (TRT3,1))) {
// Variable:
Name(ONTM, 0) ///ONTM: 0 - Not in Speculative ON ; Non-Zero - elapsed time in Nanosecs after Physical ON
Name (TPPE, 0)  // Gpio for touchPaneL power enable
Name (TPPR, 0)  // Gpio for touchPaneL Reset
Name (TPEP, 0)  // TouchPaneL power enable Gpio pin polarity
Name (TPRP, 0)  // TouchPaneL Reset Gpio pin polarity

Store(DeRefOf(Index (\_SB.TLD3, (TPL_POWER_GPIO_INDEX + TPLD_INDEX * 4))), TPPE)
Store(DeRefOf(Index (\_SB.TLD3, (TPL_POWER_POLARITY_INDEX + TPLD_INDEX * 4))), TPEP)
Store(DeRefOf(Index (\_SB.TLD3, (TPL_RESET_GPIO_INDEX + TPLD_INDEX * 4))), TPPR)
Store(DeRefOf(Index (\_SB.TLD3, (TPL_RESET_POLARITY_INDEX + TPLD_INDEX * 4))), TPRP)

Alias(TPDL, TD_D)   // TD_D - Touch Device power on delay


// Timing of Power and Reset for I2C Touch Panel
//  _____________________________________________________________________________________________________
// | minimum required time |  Reset de-assert from Power On (ms)   |     Power Off from Reset assert (ms)|
// |_______________________|_______________________________________|_____________________________________|
// | current setting       |                2                      |                3                    |
// |_______________________|_______________________________________|_____________________________________|
// * no maximum time limitation.

  PowerResource(PTPL, 0, 0){
     Method(_STA){
      ADBG ("_STA PowerResource")

      ADBG (Concatenate ("STA TPL Bridge= ", ToHexString (TPBR)))
      ADBG (Concatenate ("TPL Controller= ", ToHexString (TPCT)))

      If(LEqual(\_SB.GGOV(TPPE), 1)) {
        ADBG (Concatenate ("TPL _STA ON I2C controller= ", ToHexString (TPCT)))
        Return(0x01)
      } Else {
        ADBG (Concatenate ("TPL _STA OFF I2C controller= ", ToHexString (TPCT)))
        Return(0x00)
      }
    }

    Method(_ON){
      ADBG (Concatenate ("Touch PWR ON I2C controller= ", ToHexString (TPCT)))
      // drive pwr high
      \_SB.SGOV(TPPE, TPEP)
      Sleep(2)                  // time for Reset de-assert from Power On
      // De-Assert GPIO RST
      \_SB.SGOV(TPPR, TPRP)
      // update ONTM
      Store(Timer(), ONTM)
      // enable int line
      If (LEqual (TPLM,1)) {
        \_SB.SGRA(TPIN, 1)
      }
    }

    Method(_OFF){
      ADBG (Concatenate ("Touch PWR OFF I2C controller= ", ToHexString (TPCT)))
      // disable int line
      If (LEqual (TPLM,1)) {
        \_SB.SGRA(TPIN, 0)
      }
      // Assert GPIO RST
      Xor(TPRP, 1, Local0)
      \_SB.SGOV(TPPR, Local0)
      Sleep(3)                   // time for Power Off from Reset assert
      // drive pwr low
      Xor(TPEP, 1, Local0)
      \_SB.SGOV(TPPE, Local0)
      // update ONTM
      Store(Zero , ONTM)  ///- Clear ONTM
    }
  }

  /// PS0X Method
  Method (PS0X,0,Serialized)
  {
    ADBG (Concatenate (Concatenate("TPL",TPLD_INDEX), " D0"))

    If (LEqual (ONTM,Zero))               /// Check if I2C controller is powered on
    {
      Return ()
    }

    ///
    /// Make sure "D0 delay" (TD_D) delay is elapsed before returning _PS0 \n
    /// Variables:
    ///- Local0: Elapse time since the _ON method \n
    ///- VRRD: VR Rampup Delay \n
    ///- TD_D: Time required for device to be ready after power on \n
    ///- Local1 = TD_D + VRRD: Need to incorporate VRRD since the _ON method no longer has VR Rampup Delay \n
    ///- So only need sleep for (Local1 - Local0), the amount of time remaining since the _ON method \n
    ///
    Divide (Subtract (Timer (), ONTM), 10000, , Local0) /// Store Elapsed time in ms, ignore remainder \n
    Add (TD_D, VRRD, Local1)              /// Incorporate VR Rampup Delay \n
    If (LLess (Local0, Local1))           /// Do not sleep if already past the delay requirement (TD_D + VRRD) for Touch Device \n
    {
      ///- Else, delay for power ramp
      Sleep (Subtract (Local1, Local0))   ///- Sleep (TD_D + VRRD - time elapsed)
    }

  ///@defgroup i2c_touchdev_ps0    I2C1 Touch Device _PS0 Method
  }

  Method (PS3X,0,Serialized)
  {
    ADBG (Concatenate (Concatenate("TPL",TPLD_INDEX), " D3"))
  ///@defgroup i2c_touchdev_ps3        I2C1 Touch Device _PS3 Method
  }

  Method (_PR0, Zero, NotSerialized, 0, PkgObj)    /// _PR0 Method
  {
     ADBG ("_STA PR0")

    If (LEqual (_STA (),0xF))             /// Check if Touch Device is enabled
    {
      Return (Package (){PTPL})                       ///- Return Device Specific Power Package (PTPL) if enabled
    } Else {
      Return (Package () {})              ///- Return NULL Power Package if disabled
    }

  ///@defgroup i2c_touchdev_pr0        I2C1 Touch Device _PR0 Method
  }

  Method (_PR3, Zero, NotSerialized, 0, PkgObj)    /// _PR3 Method
  {
     ADBG ("_STA PR3")
    If (LEqual (_STA (),0xF))             /// Check if Touch Device is enabled
    {
      Return (Package (){PTPL})                       ///- Return Device Specific Power Package (PTPL) if enabled
    } Else {
      Return (Package () {})              ///- Return NULL Power Package if disabled
    }

  ///@defgroup i2c_touchdev_pr3        I2C1 Touch Device _PR3 Method
  }

  Method(_PS0) { PS0X() }
  Method(_PS3) { PS3X() }
}