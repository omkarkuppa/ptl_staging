/** @file
  Register names for Telemetry
  <b>Conventions</b>:
  - Prefixes:
    - Definitions beginning with "R_" are registers
    - Definitions beginning with "B_" are bits within registers
    - Definitions beginning with "V_" are meaningful values of bits within the registers
    - Definitions beginning with "S_" are register sizes
    - Definitions beginning with "N_" are the bit position

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#ifndef _REGS_TELEMETRY_H_
#define _REGS_TELEMETRY_H_

//
// Device 10 Memory Mapped IO Register Offset Equates
//
#define TEL_BUS_NUM     0x00
#define TEL_DEV_NUM     0x0A
#define TEL_FUN_NUM     0x00

//
// PCI Configuration Space Registers
//
#define R_TEL_CFG_BAR0                              0x10
#define R_TEL_CFG_BAR1                              0x14

//
// DVSEC capability Registers
//
#define R_TEL_DVSEC_OFFSET                       0x100

#define R_TEL_DVSEC_PCIE_CAP_ID                  0x0            // PCI Express Capability ID
#define R_TEL_DVSEC_NEXT_CAP                     0x2            // Next Capability Offset
#define R_TEL_DVSEV_ID                           0x8            // DVSEC_ID
#define R_TEL_DVSEV_DISCOVERY_TABLE_OFFSET       0xC            // Discovery Table Offset


#define V_TELEMETRY_EXTENDED_CAP_ID              0x23          // Indicates that this is a Telemetry Aggregator Extended Capability
#define V_CRASHLOG_DVSEC_ID                      0x04          // CrashLog DEVSC ID
#define V_TEL_DVSEC_TBIR_BAR0                    0
#define V_TEL_DVSEC_TBIR_BAR1                    1

//
// CPU CrashLog MMIO Registers
//
#define R_CRASHLOG_HEADER_ADDRESS                          0x6030

#define R_CRASHLOG_MAILBOX_INTERFACE_ADDRESS               0x6038

#define R_CRASHLOG_MAIN_POINTER_OFFSET                     0x6040
#define R_CRASHLOG_TELEMETRY_POINTER_OFFSET                0x6048
#define R_CRASHLOG_TRACE_POINTER_OFFSET                    0x6050

#define R_CRASHLOG_POINTER_BASE_ADDRESS_FIELD_OFFSET       0x00
#define R_CRASHLOG_POINTER_SIZE_FIELD_OFFSET               0x04

//
// CPU CrashLog Mailbox commands
//
#define V_CPU_CRASHLOG_CMD_DISABLE                            0
#define V_CPU_CRASHLOG_CMD_TRIGGER                            1
#define V_CPU_CRASHLOG_CMD_CLEAR                              2
#define V_CPU_CRASHLOG_CMD_DISCOVERY                          3
#define V_CPU_CRASHLOG_CMD_CRASHLOG_ON_ALL_RESET              4
#define V_CPU_CRASHLOG_CMD_REARM                              5
#define V_CPU_CRASHLOG_CMD_CONSUMED                           6

#define CPU_CRASHLOG_MAILBOX_WAIT_STALL                       1
#define CPU_CRASHLOG_MAILBOX_WAIT_TIMEOUT                     1000

#endif // _SA_REGS_TELEMETRY_H_
