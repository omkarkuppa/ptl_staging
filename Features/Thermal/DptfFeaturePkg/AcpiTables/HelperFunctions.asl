/** @file
  Intel ACPI Reference Code for Intel (R) Dynamic Tuning Technology

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

@par Specification Reference:
**/

Scope (\_SB.IETM)
{
    // KTOC (Kelvin to Celsius)
    //
    // This control method converts from 10ths of degree Kelvin to Celsius.
    //
    // Arguments: (1)
    //   Arg0 - Temperature in 10ths of degree Kelvin
    // Return Value:
    //   Temperature in Celsius
    //
    Method (KTOC,1,Serialized)
    {
      If (LGreater (Arg0,2732)) // make sure we have a temperature above zero Celcius
      {
        Return (Divide (Subtract (Arg0, 2732), 10))
      } Else {
        Return (0) // negative temperatures Celcius are changed to 0 degrees Celcius
      }
    }

    // CTOK (Celsius to Kelvin)
    //
    // This control method converts from Celsius to 10ths of degree Kelvin.
    //
    // Arguments: (1)
    //   Arg0 - Temperature in Celsius
    // Return Value:
    //   Temperature in 10ths of degree Kelvin
    //
    Method (CTOK,1,Serialized)
    {
      Return (Add (Multiply (Arg0,10),2732))
    }

    // C10K (Celsius to Kelvin)
    //
    // This control method converts from 10ths of degree Celsius (16 bit signed) to 10ths of degree Kelvin.
    // This method works only for temperatures upto absolute 0 Kelvin.
    //
    // Arguments: (1)
    //   Arg0 - Temperature in 10ths of degree Celsius (16 bit signed value)
    // Return Value:
    //   Temperature in 10ths of degree Kelvin
    //
    Method (C10K,1,Serialized)
    {
      Name (TMP1, Buffer (16) {0})
      CreateByteField (TMP1, 0x00, TMPL)
      CreateByteField (TMP1, 0x01, TMPH)

      Add (Arg0, 2732, Local0)
      Store (And (Local0,0xFF),TMPL)
      Store (ShiftRight (And (Local0,0xFF00),0x8),TMPH)

      ToInteger (TMP1, Local1)
      Return (Local1)

    }

    // K10C (Kelvin to Celsius)
    //
    // This control method converts from 10ths of degree Kelvin to 10ths of degree Celsius (16 bit signed).
    // This method works only for temperatures upto 0 degree celsius. Temperatures below 0 deg celsius are changed to 0 deg C.
    //
    // Arguments: (1)
    //   Arg0 - Temperature in 10ths of degree Kelvin
    // Return Value:
    //   Temperature in 10ths of degree Celsius (16 bit signed value)
    //
    Method (K10C,1,Serialized)
    {
      If (LGreater (Arg0,2732)) // make sure we have a temperature above zero Celcius
      {
        Return (Subtract (Arg0, 2732))
      } Else {
        Return (0) // negative temperatures Celcius are changed to 0 degrees Celcius
      }

    }
}