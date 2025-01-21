/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

      External (HOST_BUS.ROOT_PORT_OBJECT.HPSX, FieldUnitObj)
      External (HOST_BUS.ROOT_PORT_OBJECT.L0SE, FieldUnitObj)
      External (HOST_BUS.ROOT_PORT_OBJECT.PDSX, FieldUnitObj)
      External (HOST_BUS.ROOT_PORT_OBJECT.PDCX, FieldUnitObj)
      External (HOST_BUS.ROOT_PORT_OBJECT.VDID, FieldUnitObj)

      //
      // 1. Check if Root Port exists.
      // 2. Check if Root Port is enabled.
      // 2. Check for a Hot Plug Event.
      //
      If (CondRefOf (HOST_BUS.ROOT_PORT_OBJECT)) {
        If (LAnd (LNotEqual (HOST_BUS.ROOT_PORT_OBJECT.VDID,0xFFFFFFFF),HOST_BUS.ROOT_PORT_OBJECT.HPSX))
        {
          If (HOST_BUS.ROOT_PORT_OBJECT.PDCX)
          {
            // Clear all status bits first.
            Store (1,HOST_BUS.ROOT_PORT_OBJECT.PDCX)
            Store (1,HOST_BUS.ROOT_PORT_OBJECT.HPSX)

            If (LNot (HOST_BUS.ROOT_PORT_OBJECT.PDSX)) {
              //
              // The PCI Express slot is empty, so disable L0s on hot unplug
              //
              Store (0,HOST_BUS.ROOT_PORT_OBJECT.L0SE)
            }
            // Perform proper notification
            // to the OS.
            Notify (HOST_BUS.ROOT_PORT_OBJECT,0)
          } Else {
            // False event.  Clear Hot-Plug Status
            // then exit.
            Store (1,HOST_BUS.ROOT_PORT_OBJECT.HPSX)
          }
        }
      }
