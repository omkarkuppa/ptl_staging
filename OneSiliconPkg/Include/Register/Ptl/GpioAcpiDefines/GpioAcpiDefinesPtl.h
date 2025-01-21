/** @file
  GPIO ASL header for PTL_PCD.

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

#ifndef _GPIOV2_ACPI_DEFINES_PTL_PCD_H_
#define _GPIOV2_ACPI_DEFINES_PTL_PCD_H_

#define GPIO_HID_PTL_PCD_H "INTC10BB" // PTL PCD-H GPIO
#define GPIO_HID_PTL_PCD_P "INTC10BC" // PTL PCD-P GPIO

//
// PCH GPIO Community address
//
#define PTL_PCD_GPIOV2_COM0                               0x590000 // PID: 0xF259
#define PTL_PCD_GPIOV2_COM1                               0x5A0000 // PID: 0xF25A
#define PTL_PCD_GPIOV2_COM3                               0x5B0000 // PID: 0xF25B
#define PTL_PCD_GPIOV2_COM4                               0x5C0000 // PID: 0xF25C
#define PTL_PCD_GPIOV2_COM5                               0x5D0000 // PID: 0xF25D

#endif // _GPIOV2_ACPI_DEFINES_PTL_PCD_H_
