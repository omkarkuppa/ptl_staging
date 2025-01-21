/**@file
  I2C Ssdt Rtd3 File
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



External (IC0D,FieldUnitObj)
External (IC1D,FieldUnitObj)


External (\_SB.GGOV, MethodObj)
External (\_SB.SGOV, MethodObj)
External (\_SB.SGRA, MethodObj)

External (VRRD,FieldUnitObj)

#define SCOPE_I2C \_SB.PC00

Name (I2EN,0)
External (SCOPE_I2C.I2C0, DeviceObj) //I2C0 Controller
External (SCOPE_I2C.I2C0.TPD0, DeviceObj) // Touch pad
External (SCOPE_I2C.I2C0, DeviceObj) //I2C1 Controller
External (SCOPE_I2C.I2C0.TPL1, DeviceObj) // Touch panel

External (SCOPE_I2C.I2C1, DeviceObj) //I2C0 Controller
External (SCOPE_I2C.I2C1.TPD0, DeviceObj) // Touch pad
External (SCOPE_I2C.I2C1, DeviceObj) //I2C1 Controller
External (SCOPE_I2C.I2C1.TPL1, DeviceObj) // Touch panel

External (SCOPE_I2C.I2C2, DeviceObj) //I2C0 Controller
External (SCOPE_I2C.I2C2.TPD0, DeviceObj) // Touch pad
External (SCOPE_I2C.I2C2, DeviceObj) //I2C1 Controller
External (SCOPE_I2C.I2C2.TPL1, DeviceObj) // Touch panel

External (SCOPE_I2C.I2C3, DeviceObj) //I2C0 Controller
External (SCOPE_I2C.I2C3.TPD0, DeviceObj) // Touch pad
External (SCOPE_I2C.I2C3, DeviceObj) //I2C1 Controller
External (SCOPE_I2C.I2C3.TPL1, DeviceObj) // Touch panel

External (SCOPE_I2C.I2C4, DeviceObj) //I2C0 Controller
External (SCOPE_I2C.I2C4.TPD0, DeviceObj) // Touch pad
External (SCOPE_I2C.I2C4, DeviceObj) //I2C1 Controller
External (SCOPE_I2C.I2C4.TPL1, DeviceObj) // Touch panel

External (SCOPE_I2C.I2C5, DeviceObj) //I2C0 Controller
External (SCOPE_I2C.I2C5.TPD0, DeviceObj) // Touch pad
External (SCOPE_I2C.I2C5, DeviceObj) //I2C1 Controller
External (SCOPE_I2C.I2C5.TPL1, DeviceObj) // Touch panel

External (SCOPE_I2C.I2C6, DeviceObj) //I2C0 Controller
External (SCOPE_I2C.I2C6.TPD0, DeviceObj) // Touch pad
External (SCOPE_I2C.I2C6, DeviceObj) //I2C1 Controller
External (SCOPE_I2C.I2C6.TPL1, DeviceObj) // Touch panel

External (SCOPE_I2C.I2C7, DeviceObj) //I2C0 Controller
External (SCOPE_I2C.I2C7.TPD0, DeviceObj) // Touch pad
External (SCOPE_I2C.I2C7, DeviceObj) //I2C1 Controller
External (SCOPE_I2C.I2C7.TPL1, DeviceObj) // Touch panel

External (SCOPE_I2C.I2C0.TPD0._STA, MethodObj)
External (SCOPE_I2C.I2C0.TPL1._STA, MethodObj)
External (SCOPE_I2C.I2C1.TPD0._STA, MethodObj)
External (SCOPE_I2C.I2C1.TPL1._STA, MethodObj)
External (SCOPE_I2C.I2C2.TPD0._STA, MethodObj)
External (SCOPE_I2C.I2C2.TPL1._STA, MethodObj)
External (SCOPE_I2C.I2C3.TPD0._STA, MethodObj)
External (SCOPE_I2C.I2C3.TPL1._STA, MethodObj)
External (SCOPE_I2C.I2C4.TPD0._STA, MethodObj)
External (SCOPE_I2C.I2C4.TPL1._STA, MethodObj)
External (SCOPE_I2C.I2C5.TPD0._STA, MethodObj)
External (SCOPE_I2C.I2C5.TPL1._STA, MethodObj)
External (SCOPE_I2C.I2C6.TPD0._STA, MethodObj)
External (SCOPE_I2C.I2C6.TPL1._STA, MethodObj)
External (SCOPE_I2C.I2C7.TPD0._STA, MethodObj)
External (SCOPE_I2C.I2C7.TPL1._STA, MethodObj)


I2C_ROOTPORT(0,00)
#define I2C_ACPI_DEV  SCOPE_I2C.I2C0
#define CNTL           0
If (I2EN) {
 Include ("I2cRtd3MethodInclusion.asl")
}
#undef CNTL
#undef I2C_ACPI_DEV
I2C_ROOTPORT_END

I2C_ROOTPORT(1,01)
#define I2C_ACPI_DEV  SCOPE_I2C.I2C1
#define CNTL           1
If (I2EN) {
 Include ("I2cRtd3MethodInclusion.asl")
}
#undef CNTL
#undef I2C_ACPI_DEV
I2C_ROOTPORT_END

I2C_ROOTPORT(2,02)
#define I2C_ACPI_DEV  SCOPE_I2C.I2C2
#define CNTL           2
If (I2EN) {
 Include ("I2cRtd3MethodInclusion.asl")
}
#undef CNTL
#undef I2C_ACPI_DEV
I2C_ROOTPORT_END

I2C_ROOTPORT(3,03)
#define I2C_ACPI_DEV  SCOPE_I2C.I2C3
#define CNTL           3
If (I2EN) {
 Include ("I2cRtd3MethodInclusion.asl")
}
#undef CNTL
#undef I2C_ACPI_DEV
I2C_ROOTPORT_END

I2C_ROOTPORT(4,04)
#define I2C_ACPI_DEV  SCOPE_I2C.I2C4
#define CNTL           4
If (I2EN) {
 Include ("I2cRtd3MethodInclusion.asl")
}
#undef CNTL
#undef I2C_ACPI_DEV
I2C_ROOTPORT_END

I2C_ROOTPORT(5,05)
#define I2C_ACPI_DEV  SCOPE_I2C.I2C5
#define CNTL           5
If (I2EN) {
 Include ("I2cRtd3MethodInclusion.asl")
}
#undef CNTL
#undef I2C_ACPI_DEV
I2C_ROOTPORT_END

I2C_ROOTPORT (6,06)
#define I2C_ACPI_DEV  SCOPE_I2C.I2C6
#define CNTL           6
If (I2EN) {
 Include ("I2cRtd3MethodInclusion.asl")
}
#undef CNTL
#undef I2C_ACPI_DEV
I2C_ROOTPORT_END

I2C_ROOTPORT(7,07)
#define I2C_ACPI_DEV  SCOPE_I2C.I2C7
#define CNTL           7
If (I2EN) {
 Include ("I2cRtd3MethodInclusion.asl")
}
#undef CNTL
#undef I2C_ACPI_DEV
I2C_ROOTPORT_END

#undef SCOPE_I2C
