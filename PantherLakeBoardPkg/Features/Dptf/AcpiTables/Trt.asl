/** @file
  Intel ACPI Reference Code for Intel Dynamic Power Performance Management

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

    Name(TRT0, Package ()    // TRT package when TFN1 is at 100% speed
    {
      // Source                             Target          Influence  Period    Reserved
      Package () {\_SB.PC00.TCPU,           \_SB.IETM.SEN2, 40,        100,      0, 0, 0, 0},
      Package () {\_SB.IETM.CHRG,           \_SB.IETM.SEN4, 20,        200,      0, 0, 0, 0}
    })

    // _TRT (Thermal Relationship Table)
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   A variable-length Package containing a list of Thermal Relationship Packages as described below.
    //
    // Return Value Information
    //   Package {
    //   ThermalRelationship[0] // Package
    //    ...
    //   ThermalRelationship[n] // Package
    //   }
    //
    Method (PTRT,,,,PkgObj)
    {
      Return (TRT0)
    } // End PTRT
