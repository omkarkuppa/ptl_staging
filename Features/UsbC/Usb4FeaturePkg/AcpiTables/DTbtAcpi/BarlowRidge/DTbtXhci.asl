/** @file
  Discrete Thunderbolt (TBT) XHCI ACPI implementation

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

#include "DTbtSwcmWrapper.h"

External (DTBT_US_PORT.DTID, IntObj)
External (DTBT_US_PORT.TBDH.NHI0_DEV, DeviceObj)
External (DTBT_US_PORT.TBDH.NHI1_DEV, DeviceObj)
External (\ADBG, MethodObj)

Device(XHCI)
{
  Name(_ADR, 0x00)

  Method (_S0W, 0x0, NotSerialized)
  {
    ADBG (Concatenate ("[dTBT] DS XHCI _S0W returns 4 - dTBT ", ToDecimalString (DTBT_US_PORT.DTID)))
    Return (4)
  }

  Method (_PS0,0,Serialized)
  {
    ADBG (Concatenate ("[dTBT] DS XHCI _PS0 - dTBT ", ToDecimalString (DTBT_US_PORT.DTID)))
  }

  Method (_PS3,0,Serialized)
  {
    ADBG (Concatenate ("[dTBT] DS XHCI _PS3 - dTBT ", ToDecimalString (DTBT_US_PORT.DTID)))
  }

  Device(RHUB)
  {
    Name(_ADR, 0x00)

    //
    // DTBT XHCI controller _DSM method
    //
    Method (_DSM,4,serialized)
    {
      If(LEqual(Arg0, ToUUID("ce2ee385-00e6-48cb-9f05-2edb927c4899"))) {
        ADBG ("[dTBT] XHCI RHUB _DSM")
        If (LEqual (Arg2, 0)) {
          ADBG("[dTBT] XHCI RHUB F0")
          Return (Buffer () {0x81})
        } // function 0: list of supported functions
        If (LEqual (Arg2, 7)) {
          ADBG("[dTBT] XHCI RHUB F7")
          Return (Buffer () {0x01})
        } // function 7: providing _UPC supported or NOT
      }
      Return (Buffer () {0})
    }

    Method (_S0W, 0x0, NotSerialized)
    {
      ADBG (Concatenate ("[dTBT] DS XHCI RHUB _S0W return 4 - dTBT ", ToDecimalString (DTBT_US_PORT.DTID)))
      Return (4)
    }

    Method (_PS0,0,Serialized)
    {
      ADBG (Concatenate ("[dTBT] DS XHCI RHUB _PS0 - dTBT ", ToDecimalString (DTBT_US_PORT.DTID)))
    }

    Method (_PS2,0,Serialized)
    {
      ADBG (Concatenate ("[dTBT] DS XHCI RHUB _PS2 - dTBT ", ToDecimalString (DTBT_US_PORT.DTID)))
    }

    Method (_PS3,0,Serialized)
    {
      ADBG (Concatenate ("[dTBT] DS XHCI RHUB _PS3 - dTBT ", ToDecimalString (DTBT_US_PORT.DTID)))
    }

    //
    // USB ports on Discrete Thunderbolt AIC
    //
    Include ("BarlowRidge/XhciUsbPorts.asl")
  } // End of Device(RHUB)
  // BR XHCI controller is unremoveable
  Method (_RMV, 0, NotSerialized)  // _RMV: Removal Status
  {
    Return (Zero) // The device can't be removed
  }
} // End of Device(XHCI)
