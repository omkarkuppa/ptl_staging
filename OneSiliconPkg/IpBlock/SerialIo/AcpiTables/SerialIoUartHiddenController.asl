/**@file

  Serial IO UART Hidden Controllers ACPI definitions

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

Scope (\_SB) {
  If (LOr (LEqual (UART_DEVICE_MODE, SERIAL_IO_UART_HIDDEN), LEqual (UART_DEVICE_MODE, SERIAL_IO_UART_COM))) {
    Device (UART_DEVICE_HIDDEN) {
      Name (_DDN, UART_DEVICE_DDN)
      Name (_UID, UART_DEVICE_DDN)
      Method (_HID) { Return (UHID (UART_DEVICE_MODE)) }
      Method (_CRS) { Return (UARH (UART_DEVICE_PCI_BASE, UART_DEVICE_IRQ)) }
      Method (_STA) {
        If (LEqual (UART_DEVICE_MODE, SERIAL_IO_UART_COM)) {
          If (LEqual (UART_DEVICE_PG, UART_PG_AUTO)) {
            Store (UAPG (UART_DEVICE_MODE, UART_DEVICE_PG, UART_DEVICE_PCI_BASE), UART_DEVICE_PG)
          }
          Return (0xF)
        }
        Return (0x8)
      }
      If (LEqual (UART_DEVICE_MODE, SERIAL_IO_UART_COM)) {
        Method (_PSC) { Return (UPSC (UART_DEVICE_PCI_BASE)) }
        Method (_PS3) { UPS3 (UART_DEVICE_PG, UART_DEVICE_PCI_BASE) }
        Method (_PS0) { UPS0 (UART_DEVICE_PG, UART_DEVICE_PCI_BASE) }
      }
    }
  }

} //End Scope (\_SB)
