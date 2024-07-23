/**@file

  ACPI D3Cold/D3hot support for Storage device connected to PCIE Root Port

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

      External(PCIE_ROOT_PORT.PXSX.PNVM, MethodObj, IntObj)
      External(PCIE_ROOT_PORT.PXSX.PAHC, MethodObj, IntObj)
      External(PCIE_ROOT_PORT.VDID, FieldUnitObj)

      //
      // 1. Check if Root Port exists.
      // 2. Check if Root Port is mapped under VMD.
      // 2. Check for a storage device.
      //
      If (CondRefOf(PCIE_ROOT_PORT)) {
        If(LNotEqual(PCIE_ROOT_PORT.VDID,0xFFFFFFFF)) {
          If (CondRefOf(\STD3)) {
            If (CondRefOf(PCIE_ROOT_PORT.PRMV)) {
              If (LEqual (PCIE_ROOT_PORT.PRMV, 1)) {
                ADBG(Concatenate("VMD Storage Runtime D3 status:", ToHexString(PCIE_ROOT_PORT.RD3C)))
                Store(\STD3, PCIE_ROOT_PORT.RD3C)
              }
            }

            If(LOr(PCIE_ROOT_PORT.PXSX.PNVM(),PCIE_ROOT_PORT.PXSX.PAHC())) {
              ADBG(Concatenate("Storage Runtime D3 status:", ToHexString(PCIE_ROOT_PORT.RD3C)))
              Store(\STD3, PCIE_ROOT_PORT.RD3C)
            }
          }
        }
      }
