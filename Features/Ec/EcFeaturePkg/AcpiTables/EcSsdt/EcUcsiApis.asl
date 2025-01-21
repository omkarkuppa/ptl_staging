/** @file
  ACPI EC type-c APIs that consumed by Ucsi.

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

@par Specification Reference:
**/

External (UCMS, IntObj)

/**
  RPOI        Get PPM->OPM message in and CCI indicator.

  @param      None.
  @retval     Return the PPM->OPM message in and CCI indicator in buffer object (24 bytes):
              Index0      - MI00
              Index1      - MI01
              Index2      - MI02
              Index3      - MI03
              Index4      - MI04
              Index5      - MI05
              Index6      - MI06
              Index7      - MI07
              Index8      - MI08
              Index9      - MI09
              Index10     - MI0A
              Index11     - MI0B
              Index12     - MI0C
              Index13     - MI0D
              Index14     - MI0E
              Index15     - MI0F
              Index16     - MI10
              Index17     - MI11
              Index18     - MI12
              Index19     - MI13
              Index20     - CCI0
              Index21     - CCI1
              Index22     - CCI2
              Index23     - CCI3

**/
Method (RPOI)
{
  Name (PMBF, Buffer (24) {})
  Store (ECRD (RefOf (MI00)), Index (PMBF, 0))
  Store (ECRD (RefOf (MI01)), Index (PMBF, 1))
  Store (ECRD (RefOf (MI02)), Index (PMBF, 2))
  Store (ECRD (RefOf (MI03)), Index (PMBF, 3))
  Store (ECRD (RefOf (MI04)), Index (PMBF, 4))
  Store (ECRD (RefOf (MI05)), Index (PMBF, 5))
  Store (ECRD (RefOf (MI06)), Index (PMBF, 6))
  Store (ECRD (RefOf (MI07)), Index (PMBF, 7))
  Store (ECRD (RefOf (MI08)), Index (PMBF, 8))
  Store (ECRD (RefOf (MI09)), Index (PMBF, 9))
  Store (ECRD (RefOf (MI0A)), Index (PMBF, 10))
  Store (ECRD (RefOf (MI0B)), Index (PMBF, 11))
  Store (ECRD (RefOf (MI0C)), Index (PMBF, 12))
  Store (ECRD (RefOf (MI0D)), Index (PMBF, 13))
  Store (ECRD (RefOf (MI0E)), Index (PMBF, 14))
  Store (ECRD (RefOf (MI0F)), Index (PMBF, 15))
  If (CondRefOf (UCMS)) {
    If (LEqual (UCMS, 1)) {
      Store (ECRD (RefOf (CCI0)), Index (PMBF, 16))
      Store (ECRD (RefOf (CCI1)), Index (PMBF, 17))
      Store (ECRD (RefOf (CCI2)), Index (PMBF, 18))
      Store (ECRD (RefOf (CCI3)), Index (PMBF, 19))
      Return (PMBF) // UCSI 1.2
    }
  }
  Store (ECRD (RefOf (MI10)), Index (PMBF, 16))
  Store (ECRD (RefOf (MI11)), Index (PMBF, 17))
  Store (ECRD (RefOf (MI12)), Index (PMBF, 18))
  Store (ECRD (RefOf (MI13)), Index (PMBF, 19))
  Store (ECRD (RefOf (CCI0)), Index (PMBF, 20))
  Store (ECRD (RefOf (CCI1)), Index (PMBF, 21))
  Store (ECRD (RefOf (CCI2)), Index (PMBF, 22))
  Store (ECRD (RefOf (CCI3)), Index (PMBF, 23))
  Return (PMBF) // UCSI 2.x
}

/**
  WOPM      Write to ECOPM.

  @param    Arg0  - 24 bytes OPM data to write to EC.
              Index0    - MGO0
              Index1    - MGO1
              Index2    - MGO2
              Index3    - MGO3
              Index4    - MGO4
              Index5    - MGO5
              Index6    - MGO6
              Index7    - MGO7
              Index8    - MGO8
              Index9    - MGO9
              Index10   - MGOA
              Index11   - MGOB
              Index12   - MGOC
              Index13   - MGOD
              Index14   - MGOE
              Index15   - MGOF
              Index16   - CTL0
              Index17   - CTL1
              Index18   - CTL2
              Index19   - CTL3
              Index20   - CTL4
              Index21   - CTL5
              Index22   - CTL6
              Index23   - CTL7
  @retval   None.

**/
Method (WOPM, 1, Serialized)
{
  ECWT (DeRefOf (Index (Arg0, 0)), RefOf (MGO0))
  ECWT (DeRefOf (Index (Arg0, 1)), RefOf (MGO1))
  ECWT (DeRefOf (Index (Arg0, 2)), RefOf (MGO2))
  ECWT (DeRefOf (Index (Arg0, 3)), RefOf (MGO3))
  ECWT (DeRefOf (Index (Arg0, 4)), RefOf (MGO4))
  ECWT (DeRefOf (Index (Arg0, 5)), RefOf (MGO5))
  ECWT (DeRefOf (Index (Arg0, 6)), RefOf (MGO6))
  ECWT (DeRefOf (Index (Arg0, 7)), RefOf (MGO7))
  ECWT (DeRefOf (Index (Arg0, 8)), RefOf (MGO8))
  ECWT (DeRefOf (Index (Arg0, 9)), RefOf (MGO9))
  ECWT (DeRefOf (Index (Arg0, 10)), RefOf (MGOA))
  ECWT (DeRefOf (Index (Arg0, 11)), RefOf (MGOB))
  ECWT (DeRefOf (Index (Arg0, 12)), RefOf (MGOC))
  ECWT (DeRefOf (Index (Arg0, 13)), RefOf (MGOD))
  ECWT (DeRefOf (Index (Arg0, 14)), RefOf (MGOE))
  ECWT (DeRefOf (Index (Arg0, 15)), RefOf (MGOF))
  ECWT (DeRefOf (Index (Arg0, 16)), RefOf (CTL0))
  ECWT (DeRefOf (Index (Arg0, 17)), RefOf (CTL1))
  ECWT (DeRefOf (Index (Arg0, 18)), RefOf (CTL2))
  ECWT (DeRefOf (Index (Arg0, 19)), RefOf (CTL3))
  ECWT (DeRefOf (Index (Arg0, 20)), RefOf (CTL4))
  ECWT (DeRefOf (Index (Arg0, 21)), RefOf (CTL5))
  ECWT (DeRefOf (Index (Arg0, 22)), RefOf (CTL6))
  ECWT (DeRefOf (Index (Arg0, 23)), RefOf (CTL7))
  ECMD (EC_C_UCSI_CMD_HNDLR)
}