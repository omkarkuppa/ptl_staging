/** @file
  DPTF APIs that would be used in EC Qevent.

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

/**
  DP30        DPTF notify function for Q30 event
**/
Method (DP30)
{
  If (LAnd (LEqual (\_SB.DTTE, 1), LEqual (\_SB.PWRE, 1))) {
    Notify (\_SB.IETM.TPWR, 0x81) // notify Power participant
  }
}

/**
  DP31        DPTF notify function for Q31 event
  Note:       Same with DP30
**/
Method (DP31)
{
  If (LAnd (LEqual (\_SB.DTTE, 1), LEqual (\_SB.PWRE, 1))) {
    Notify (\_SB.IETM.TPWR, 0x81) // notify Power participant
  }
}

/**
  DP32        DPTF notify function for Q32 event
**/
Method (DP32)
{
  If (LAnd (LEqual (\_SB.DTTE, 1), LEqual (\_SB.BATR, 1))) {
    Notify (\_SB.IETM.BAT1, 0x86) // notify Battery participant
  }
}

/**
  DP33        DPTF notify function for Q33 event
**/
Method (DP33)
{
  If (LAnd (LEqual (\_SB.DTTE, 1), LEqual (\_SB.PWRE, 1))) {
    Notify (\_SB.IETM.TPWR, 0x81) // notify Power participant
  }
}

/**
  DP34        DPTF notify function for Q34 event
**/
Method (DP34)
{
  If (LAnd (LEqual (\_SB.DTTE, 1), LEqual (\_SB.BATR, 1))) {
    Notify (\_SB.IETM.BAT1, 0x80) // notify Battery participant
  }
}

/**
  DP35        DPTF notify function for Q35 event
**/
Method (DP35)
{
  If (LAnd (LEqual (\_SB.DTTE, 1), LEqual (\_SB.BATR, 1))) {
    Notify (\_SB.IETM.BAT1, 0x83) // notify Battery participant
  }
}

/**
  DP37        DPTF notify function for Q37 event
  @param      Arg0  - Power Source Type. Only check the bit2.
                      If BIT2 == 1, Programmable Power Supply Enable
**/
Method (DP37, 1)
{
  If (And (Arg0, 0x04)) {
    ADBG ("PPS adapter plugged-in")
    Store (1, \_SB.ODV1)
    \_SB.IETM.ODVP ()
    ADBG ("Update the OEM variable 1")
    If (LAnd (LEqual (\_SB.ODV0, 1), LEqual (\_SB.ODV1, 1))) {
      ADBG ("Notify DTT OEM variable changed")
      Notify (\_SB.IETM, 0x88)                     // Intel(R) Dynamic Tuning Technology framework driver evaluate ODVP
      If (LEqual (\_SB.CHGE, 1)) {
        ADBG ("Notify DTT P state table changed")
        Notify (\_SB.IETM.CHRG, 0x80)              // PPS re-evaluation after AC/DC transtion has occurred.
      }
    }
  } Else {
    ADBG ("PPS adapter removed")
    Store (0, \_SB.ODV1)
    \_SB.IETM.ODVP ()
    ADBG ("Turn off variable 1")
    If (NOT (LAnd (LEqual (\_SB.ODV0, 1), LEqual (\_SB.ODV1, 1)))) {
      ADBG ("Notify DTT OEM variable changed")
      Notify (\_SB.IETM, 0x88)                     // Intel(R) Dynamic Tuning Technology framework driver evaluate ODVP
      If (LEqual (\_SB.CHGE, 1)) {
        ADBG ("Notify DTT P state table changed")
        Notify (\_SB.IETM.CHRG, 0x80)              // PPS re-evaluation after AC/DC transtion has occurred.
      }
    }
  }

  If (LAnd (LEqual (\_SB.DTTE, 1), LEqual (\_SB.PWRE, 1))) {
    Notify (\_SB.IETM.TPWR, 0x81) // notify Power participant
  }
}

/**
  DP38        DPTF notify function for Q38 event
**/
Method (DP38)
{
  If (LAnd (LEqual (\_SB.DTTE, 1), LEqual (\_SB.BATR, 1))) {
    Notify (\_SB.IETM.BAT1, 0x85) // notify Battery participant
  }
}

/**
  DP39        DPTF notify function for Q32 event
**/
Method (DP39)
{
  If (LAnd (LEqual (\_SB.DTTE, 1), LEqual (\_SB.BATR, 1))) {
    Notify (\_SB.IETM.BAT1, 0x86) // notify Battery participant
  }
}

/**
  DP3A        DPTF notify function for Q3A event
**/
Method (DP3A)
{
  If (LAnd (LEqual (\_SB.DTTE, 1), LEqual (\_SB.BATR, 1))) {
    Notify (\_SB.IETM.BAT1, 0x86) // notify Battery participant
  }
}

/**
  DTTV        Dynamic Tuning Technology validation
**/
Method (DTTV)
{
  If (LEqual (\_SB.DTTE, 1)) {
    Notify (\_SB.IETM, 0xA0)
  }
}

/**
  DTTI      Increment Intel(R) Dynamic Tuning Technology ODVP elements for testing.
**/
Method (DTTI)
{
  If (LGreaterEqual (\_SB.ODV2, 255)) {
    Store (0, \_SB.ODV2) // reset to 0 if max exceeded
  } Else {
    Increment (\_SB.ODV2)
  }

  If (LGreaterEqual (\_SB.ODV3, 255)) {
    Store (0, \_SB.ODV3) // reset to 0 if max exceeded
  } Else {
    Increment (\_SB.ODV3)
  }

  If (LGreaterEqual (\_SB.ODV4, 255)) {
    Store (0, \_SB.ODV4) // reset to 0 if max exceeded
  } Else {
    Increment (\_SB.ODV4)
  }

  If (LGreaterEqual (\_SB.ODV5, 255)) {
    Store (0, \_SB.ODV5) // reset to 0 if max exceeded
  } Else {
    Increment (\_SB.ODV5)
  }

  \_SB.IETM.ODVP ()
  Notify (\_SB.IETM, 0x88) // Intel (R) Dynamic Tuning Technology framework driver evaluate ODVP
}
