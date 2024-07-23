/** @file
  Default platform setup configuration settings.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2009 Intel Corporation.

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

#ifndef _SETUP_CONFIGURATION_H_
#define _SETUP_CONFIGURATION_H_

#define SERIAL_SETUP_OPTION  1
#define TOTAL_SIO_SERIAL_PORTS  0x4
#define TOTAL_PCI_SERIAL_PORTS  0x1
#define TOTAL_SERIAL_PORTS  0x5
#define MAX_SIO_SERIAL_PORTS  0x4
#define MAX_PCI_SERIAL_PORTS  0x4

#define DEFAULT_CONSOLE_REDIRECTION_ENABLE  1
#define DEFAULT_ACPI_SPCR_COM_PORT  0x0
#define DEFAULT_TERMINAL_TYPE  0x3
#define DEFAULT_RESOLUTION_TYPE  0x0
#define DEFAULT_ACPI_SPCR_CONSOLE_REDIRECTION_ENABLE  1
#define DEFAULT_ACPI_SPCR_TABLE_TERMINAL_TYPE  0x2
#define HARDWARE_FLOW_CONTROL_SETUP_OPTION  0x1
#define SOFTWARE_FLOW_CONTROL_SETUP_OPTION  0x2
#define TIMEOUT_FOR_DETERMINING_LONE_ESC_CHAR  0xf4240

#define DEFAULT_BOOT_TIMEOUT  0x1
#define DEFAULT_FAST_BOOT  0
#define DEFAULT_QUIET_BOOT  0
#define FORCE_USER_TO_SETUP_ON_FIRST_BOOT  0
#define SHOW_ADVANCED_FORMSET  1
#define FORCE_USER_TO_SETUP_IF_BOOT_WITH_DEFAULT  0
#define LOAD_DEFAULTS_IF_SECONDARY_NVRAM_INIT     1

#define UART_SERIAL_PORT_0_INDEX  0x0
#define UART_SERIAL_PORT_1_INDEX  0x1
#define UART_SERIAL_PORT_2_INDEX  0x2
#define UART_SERIAL_PORT_3_INDEX  0x3
#define PCI_SERIAL_PORT_0_INDEX  0x4
#define PCI_SERIAL_PORT_1_INDEX  0x5
#define PCI_SERIAL_PORT_2_INDEX  0x6
#define PCI_SERIAL_PORT_3_INDEX  0x7

#define OFFSET_0  0x0
#define OFFSET_1  0x1
#define OFFSET_2  0x2
#define OFFSET_3  0x3
#define OFFSET_4  0x4
#define OFFSET_5  0x5
#define OFFSET_6  0x6
#define OFFSET_7  0x7
#define OFFSET_8  0x8
#define OFFSET_9  0x9
#define OFFSET_10  0xa
#define OFFSET_11  0xb
#define OFFSET_12  0xc
#define OFFSET_13  0xd
#define OFFSET_14  0xe
#define OFFSET_15  0xf
#define OFFSET_16  0x10
#define OFFSET_17  0x11
#define OFFSET_18  0x12
#define OFFSET_19  0x13
#define OFFSET_20  0x14
#define OFFSET_21  0x15
#define OFFSET_22  0x16
#define OFFSET_23  0x17
#define OFFSET_24  0x18
#define OFFSET_25  0x19
#define OFFSET_26  0x1a
#define OFFSET_27  0x1b
#define OFFSET_28  0x1c
#define OFFSET_29  0x1d
#define OFFSET_30  0x1e
#define OFFSET_31  0x1f
#define OFFSET_32  0x20
#define OFFSET_33  0x21
#define OFFSET_34  0x22
#define OFFSET_35  0x23
#define OFFSET_36  0x24
#define OFFSET_37  0x25
#define OFFSET_38  0x26
#define OFFSET_39  0x27
#define ENGLISH  en-US

#endif
