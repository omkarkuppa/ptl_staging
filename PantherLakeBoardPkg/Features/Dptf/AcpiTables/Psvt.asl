/** @file
  Intel ACPI Reference Code for Intel Dynamic Power Performance Management

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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


    // Used by passive policy 2.0
    //
    //  Name (PSVT, Package ()
    //  {
    //    0x80000000,     // Revision: 1 = true proportional limit, 2 = depth limit
    //    Package () {
    //      "Source",     // Source device: String
    //      "Target",     // Target device: String
    //      0x80000000,   // Priority: for use by passive policy 2.0
    //      0x80000000,   // Sampling Period: in 1/10 of seconds unit
    //      0x80000000,   // Passive Temp: Threshold(1/10 of K) at which Passive Policy 2.0 will take action
    //      0x80000000,   // Domain: type under the source that is controlled
    //      0x80000000,   // Control Knob: type of control knob that need to be exercised
    //      0x80000000,   // Control Value: Depth Limit or Target State which policy will jump to on a thermal condition(based on revision).
    //      0x80000000,   // Step size for this specific control knob
    //      0x80000000,   // Limit Coefficient: 1/10 of units, used with StepSize to control limiting action
    //      0x80000000,   // UnLimit Coefficient: 1/10 of units, used with StepSize to control unlimiting action
    //      0x80000000    // Reserved1
    //    }
    //  })
    Name (PSVT, Package ()
    {
      2,
      Package () {\_SB.IETM.CHRG, \_SB.IETM.SEN3, 1, 200, 3182, 14, 0x000A0000, "MAX",   1, 10, 10, 0},
      Package () {\_SB.IETM.CHRG, \_SB.IETM.SEN3, 1, 200, 3232, 14, 0x000A0000,     1,   1, 10, 10, 0},
      Package () {\_SB.IETM.CHRG, \_SB.IETM.SEN3, 1, 200, 3282, 14, 0x000A0000,     2,   1, 10, 10, 0},
      Package () {\_SB.IETM.CHRG, \_SB.IETM.SEN3, 1, 200, 3382, 14, 0x000A0000, "MIN",   1, 10, 10, 0}
    })
