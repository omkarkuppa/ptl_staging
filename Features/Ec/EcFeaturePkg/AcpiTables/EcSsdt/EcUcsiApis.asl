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

/**
  RPOI        Get PPM->OPM message in and CCI indicator.

  @param      None.
  @retval     Return the PPM->OPM message in and CCI indicator in buffer object (20 bytes):
              Index0      - MGI0
              Index1      - MGI1
              Index2      - MGI2
              Index3      - MGI3
              Index4      - MGI4
              Index5      - MGI5
              Index6      - MGI6
              Index7      - MGI7
              Index8      - MGI8
              Index9      - MGI9
              Index10     - MGIA
              Index11     - MGIB
              Index12     - MGIC
              Index13     - MGID
              Index14     - MGIE
              Index15     - MGIF
              Index16     - CCI0
              Index17     - CCI1
              Index18     - CCI2
              Index19     - CCI3

**/
Method (RPOI)
{
  Name (PMBF, Buffer (20) {})
  Store (ECRD (RefOf (MGI0)), Index (PMBF, 0))
  Store (ECRD (RefOf (MGI1)), Index (PMBF, 1))
  Store (ECRD (RefOf (MGI2)), Index (PMBF, 2))
  Store (ECRD (RefOf (MGI3)), Index (PMBF, 3))
  Store (ECRD (RefOf (MGI4)), Index (PMBF, 4))
  Store (ECRD (RefOf (MGI5)), Index (PMBF, 5))
  Store (ECRD (RefOf (MGI6)), Index (PMBF, 6))
  Store (ECRD (RefOf (MGI7)), Index (PMBF, 7))
  Store (ECRD (RefOf (MGI8)), Index (PMBF, 8))
  Store (ECRD (RefOf (MGI9)), Index (PMBF, 9))
  Store (ECRD (RefOf (MGIA)), Index (PMBF, 10))
  Store (ECRD (RefOf (MGIB)), Index (PMBF, 11))
  Store (ECRD (RefOf (MGIC)), Index (PMBF, 12))
  Store (ECRD (RefOf (MGID)), Index (PMBF, 13))
  Store (ECRD (RefOf (MGIE)), Index (PMBF, 14))
  Store (ECRD (RefOf (MGIF)), Index (PMBF, 15))
  Store (ECRD (RefOf (CCI0)), Index (PMBF, 16))
  Store (ECRD (RefOf (CCI1)), Index (PMBF, 17))
  Store (ECRD (RefOf (CCI2)), Index (PMBF, 18))
  Store (ECRD (RefOf (CCI3)), Index (PMBF, 19))
  Return (PMBF)
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