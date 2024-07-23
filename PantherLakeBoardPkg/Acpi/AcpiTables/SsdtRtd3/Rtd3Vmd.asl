/** @file
  VMD Device Discription

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
#include <Include/AcpiDebug.h>

DefinitionBlock (
    "Rtd3.aml",
    "SSDT",
    2,
    "Rtd3",
    "Rtd3_Vmd",
    0x1000
    )
{
  External (\_SB.PC00.VMD0, DeviceObj)
  ACPI_DEBUG_EXTERNAL_REFERENCE
  External (VMDE)

  If (LAnd (CondRefOf (VMDE),CondRefOf (\_SB.PC00.VMD0))) {
    Scope (\_SB.PC00)
    {
      If(LEqual(VMDE,1)) {
        ADBG("VMD is Enabled")
        Scope(VMD0) {
          // PCH PCIe Ports
          External (PP01, DeviceObj)
          External (PP02, DeviceObj)
          External (PP03, DeviceObj)
          External (PP04, DeviceObj)
          External (PP05, DeviceObj)
          External (PP06, DeviceObj)
          External (PP07, DeviceObj)
          External (PP08, DeviceObj)
          External (PP09, DeviceObj)
          External (PP10, DeviceObj)
          External (PP11, DeviceObj)
          External (PP12, DeviceObj)
          External (PP13, DeviceObj)
          External (PP14, DeviceObj)
          External (PP15, DeviceObj)
          External (PP16, DeviceObj)
          External (PP17, DeviceObj)
          External (PP18, DeviceObj)
          External (PP19, DeviceObj)
          External (PP20, DeviceObj)
          External (PP21, DeviceObj)
          External (PP22, DeviceObj)
          External (PP23, DeviceObj)
          External (PP24, DeviceObj)

          // PCD PCIe Ports
          External (SP01, DeviceObj)
          External (SP02, DeviceObj)
          External (SP03, DeviceObj)
          External (SP04, DeviceObj)
          External (SP05, DeviceObj)
          External (SP06, DeviceObj)
          External (SP07, DeviceObj)
          External (SP08, DeviceObj)
          External (SP09, DeviceObj)
          External (SP10, DeviceObj)
          External (SP11, DeviceObj)
          External (SP12, DeviceObj)
          External (SP13, DeviceObj)
          External (SP17, DeviceObj)
          External (SP21, DeviceObj)

          External (PRT0, DeviceObj)
          External (PRT1, DeviceObj)
          External (PRT2, DeviceObj)
          External (PRT3, DeviceObj)
          External (PRT4, DeviceObj)
          External (PRT5, DeviceObj)
          External (PRT6, DeviceObj)
          External (PRT7, DeviceObj)

          External (VMSP) // PCD PCIe RPs mapped under VMD
          External (VMPP) // PCH PCIe RPs mapped under VMD

          // PCH PCIe RPs mapped under VMD
          If (CondRefOf (VMPP)) {
            If (VMPP & 0x01) {
              Scope (PP01) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x02) {
              Scope (PP02) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x04) {
              Scope (PP03) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x08) {
              Scope (PP04) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x10) {
              Scope (PP05) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x20) {
              Scope (PP06) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x40) {
              Scope (PP07) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x80) {
              Scope (PP08) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x100) {
              Scope (PP09) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x200) {
              Scope (PP10) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x400) {
              Scope (PP11) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x800) {
              Scope (PP12) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x1000) {
              Scope (PP13) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x2000) {
              Scope (PP14) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x4000) {
              Scope (PP15) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x8000) {
              Scope (PP16) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x10000) {
              Scope (PP17) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x20000) {
              Scope (PP18) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x40000) {
              Scope (PP19) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x80000) {
              Scope (PP20) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x100000) {
              Scope (PP21) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x200000) {
              Scope (PP22) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x400000) {
              Scope (PP23) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMPP & 0x800000) {
              Scope (PP24) {
                Include ("Rtd3VmdPciePort.asl")
              }
            }
          }

          // PCD PCIe Ports mapped under VMD
          If (CondRefOf (VMSP)) {
            If (VMSP & 0x100) {
              Scope (SP01) {      // B0/28/F0
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMSP & 0x200) {
              Scope (SP02) {      // B0/28/F1
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMSP & 0x400) {
              Scope (SP03) {      // B0/28/F2
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMSP & 0x800) {
              Scope (SP04) {      // B0/28/F3
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMSP & 0x1000) {
              Scope (SP05) {      // B0/28/F4
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMSP & 0x2000) {
              Scope (SP06) {      // B0/28/F5
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMSP & 0x4000) {
              Scope (SP07) {      // B0/28/F6
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMSP & 0x8000) {
              Scope (SP08) {      // B0/28/F7
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMSP & 0x1) {
              Scope (SP09) {      // B0/D6/F0
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMSP & 0x2) {
              Scope (SP10) {       // B0/D6/F1
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMSP & 0x4) {
              Scope (SP11) {       // B0/D6/F2
                Include ("Rtd3VmdPciePort.asl")
              }
            }

            If (VMSP & 0x08) {
              Scope (SP12) {        // B0/D6/F3
                Include ("Rtd3VmdPciePort.asl")
              }
            }
          }

          // VMD RAID
          Include ("Rtd3VmdRaid.asl")
        } // scope(VMD0)
      } // VMDE check
    } //  scope(\_SB.PC00)
  } // condref(vmde and VMD0)
}
