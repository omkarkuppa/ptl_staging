/** @file
  Discrete Thunderbolt (TBT) Software Connection Manager common ACPI implementation for NHI device

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

@par Specification
**/

#include <TbtMailBoxCmdDefinition.h>

External (\DTMB, MethodObj)
External (DTBT_RPS, FieldUnitObj)
External (DTBT_RPT, FieldUnitObj)
External (DTBT_ROOT_PORT.WAKG, IntObj)
External (\_SB.SHPO, MethodObj)

//
// Windows USB4 host router driver requirements
// Declare NHI device that device name ends with digit 0 - 7
//
Device (DTBT_NHI)
{
  Name (_ADR, 0x00000000)

  If (CondRefOf (\DTRT)) {
    Method (_S0W, 0, Serialized)
    {
      If (LEqual (\DTRT, 0)) {
        ADBG (Concatenate("[dTBT] NHI device _S0W returns 3 - dTBT ", ToDecimalString (DTBT_IDX)))
        Return (3)
      } Else {
        ADBG (Concatenate("[dTBT] NHI device _S0W returns 4 - dTBT ", ToDecimalString (DTBT_IDX)))
        Return (4)
      }
    }

    Method (_PS0, 0, Serialized)
    {
      ADBG (Concatenate ("[dTBT] NHI device _PS0 - dTBT ", ToDecimalString (DTBT_IDX)))
    }

    Method (_PS3, 0, Serialized)
    {
      ADBG (Concatenate ("[dTBT] NHI device _PS3 Start - dTBT ", ToDecimalString (DTBT_IDX)))

      If (LNot (LAnd (CondRefOf (DTBT_RPS), CondRefOf (DTBT_RPT))))
      {
        ADBG ("[dTBT] NHI _PS3 End due to RPS# or RPT# NVS not present!")
        Return ()
      }

      If (CondRefOf (DTMB))
      {
        //
        // Enable wake GPIO before sending Go2Sleep command
        //
        If (CondRefOf (DTBT_WAK))
        {
          If (LNotEqual (DTBT_WAK, 0))
          {
            ADBG (Concatenate ("[dTBT] dTBT enable wake - GPIO pin = ", ToHexString (DTBT_WAK)))
            \_SB.SHPO (DTBT_WAK, 0)
          } Else {
            ADBG ("[dTBT] Wake GPIO pin == 0, skip enabling wake!")
          }
        } Else {
          ADBG ("[dTBT] Wake GPIO pin NVS is not present, skip enabling wake")
        }
        //
        // Send Go2Sleep command and enable wake
        //
        ADBG ("[dTBT] Send Go2Sleep command")
        DTMB (PCIE2TBT_GO_2_SLEEP, 0, DTBT_RPS, DTBT_RPT)
      } Else {
        ADBG ("[dTBT] DTMB method is not present in NHI _PS3")
      }
      ADBG (Concatenate ("[dTBT] NHI device _PS3 End - dTBT ", ToDecimalString (DTBT_IDX)))
    } // Method (_PS3, 0, Serialized)
  } Else {
    ADBG("[dTBT] DTRT NVS not present while declaring NHI device!")
  }
  // BR TBT controller is unremoveable
  Method (_RMV, 0, NotSerialized)  // _RMV: Removal Status
  {
    Return (Zero) // The device can't be removed
  }
}
