/** @file
  Specific Interrupt Configuration for PTL-SOC

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include <ItssHandle.h>

//
// mPchTypeDeviceInterruptInfo consist of information for whole range of devices related to PCH. This table must cover
// all possible device and function number (even if they are not a valid device or do not generate an interrupt).
// Data in table must be arranged in asceding order to be correctly consumed by functions which use it.
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_DEV_INT_INFO mSocPDeviceInterruptInfo[] = {
  { 6, 0, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTA , IRQ_UNIQUE_NO  }, // PCI Express Root Port #9
  { 6, 1, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTB , IRQ_UNIQUE_NO  }, // PCI Express Root Port #10
  { 6, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 6, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 6, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 6, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 6, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 6, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {13, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {13, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {13, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {13, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {13, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {13, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {13, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {13, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {14, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {14, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {14, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {14, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {14, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {14, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {14, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {14, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {16, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // THC #0
  {16, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // THC #1
  {16, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {16, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {16, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {16, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {16, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {16, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {17, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I3C Controller #1
  {17, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I3C Controller #2
  {17, 2, VALID_DEV_NO,  0          , 0            , 0            , 0            , 0              }, // TraceHub Phantom (ACPI) Function
  {17, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {17, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {17, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {17, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {17, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {18, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_ISH }, // Integrated Sensor Hub
  {18, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // P2SB 16-bit
  {18, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // CSME: PMT,  doesn't use interrupts
  {18, 3, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // IEH 16-bit
  {18, 4, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // CSME: fTPM DMA
  {18, 5, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // AON Vision (AVB)
  {18, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // SerialIo: SPI #2 - disabled
  {18, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {19, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // Partner Security Engine (PSE): HECI #1
  {19, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // Partner Security Engine (PSE): HECI #2
  {19, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // Partner Security Engine (PSE): HECI #3
  {19, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {19, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {19, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {19, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {19, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {20, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // USB 3.0 xHCI Controller
  {20, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_OTG }, // USB Device Controller (OTG)
  {20, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // Shared SRAM
  {20, 3, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CNVi WiFi
  {20, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // TraceHub Phantom (ACPI) Function
  {20, 5, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // IEH 8-bit
  {20, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // DBC.DBC Phantom (ACPI) Function
  {20, 7, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CNVi: BT
  {21, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I2C Controller #0
  {21, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I2C Controller #1
  {21, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I2C Controller #2
  {21, 3, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I2C Controller #3
  {21, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {21, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {21, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {21, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {22, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CSME: HECI #1
  {22, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CSME: HECI #2
  {22, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CSME: IDE-Redirection (IDE-R)
  {22, 3, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CSME: Keyboard and Text (KT) Redirection
  {22, 4, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CSME: HECI #3
  {22, 5, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CSME: HECI #4
  {22, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {22, 7, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // CSME: WLAN
  {23, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // UFS
  {23, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {23, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {23, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {23, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {23, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {23, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {23, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {24, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // Silicon Security Engine (ESE): HECI #1
  {24, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // Silicon Security Engine (ESE): HECI #2
  {24, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // Silicon Security Engine (ESE): HECI #3
  {24, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {24, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {24, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {24, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {24, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {25, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I2C Controller #4
  {25, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I2C Controller #5
  {25, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo UART Controller #2
  {25, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {25, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {25, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {25, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {25, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 0, VALID_DEV_NO,  0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {28, 0, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTA , IRQ_UNIQUE_NO  }, // PCI Express Port 1
  {28, 1, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTB , IRQ_UNIQUE_NO  }, // PCI Express Port 2
  {28, 2, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTC , IRQ_UNIQUE_NO  }, // PCI Express Port 3
  {28, 3, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTD , IRQ_UNIQUE_NO  }, // PCI Express Port 4
  {28, 4, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTA , IRQ_UNIQUE_NO  }, // PCI Express Port 5
  {28, 5, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTB , IRQ_UNIQUE_NO  }, // PCI Express Port 6
  {28, 6, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTC , IRQ_UNIQUE_NO  }, // PCI Express Port 7
  {28, 7, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTD , IRQ_UNIQUE_NO  }, // PCI Express Port 8
  {29, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {29, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {29, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {29, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {29, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {29, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {29, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {29, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {30, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo: UART #0
  {30, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo: UART #1
  {30, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo: SPI #0
  {30, 3, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo: SPI #1
  {30, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {30, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {30, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // reserved for HPET
  {30, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // reserved for IOAPIC
  {31, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // LPC/eSPI Interface
  {31, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // P2SB
  {31, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // PMC
  {31, 3, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // ACE
  {31, 4, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_MUST_SET, IRQ_UNIQUE_NO  }, // SMBus Controller
  {31, 5, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // SPI
  {31, 6, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_MUST_SET, IRQ_UNIQUE_NO  }, // GbE Controller
  {31, 7, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTA , IRQ_UNIQUE_NO  }  // TraceHub
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mSocPDeviceInterruptInfoSize = ARRAY_SIZE (mSocPDeviceInterruptInfo);
//
// mPchTypeDeviceInterruptInfo consist of information for whole range of devices related to PCH. This table must cover
// all possible device and function number (even if they are not a valid device or do not generate an interrupt).
// Data in table must be arranged in asceding order to be correctly consumed by functions which use it.
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_DEV_INT_INFO mSocHDeviceInterruptInfo[] = {
  { 6, 0, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTA , IRQ_UNIQUE_NO  }, // TCSS PCI Express Port 21
  { 6, 1, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTB , IRQ_UNIQUE_NO  }, // TCSS PCI Express Port 22
  { 6, 2, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTC , IRQ_UNIQUE_NO  }, // TCSS PCI Express Port 23
  { 6, 3, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTD , IRQ_UNIQUE_NO  }, // TCSS PCI Express Port 24
  { 6, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 6, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 6, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 6, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 7, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 8, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  { 9, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {10, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {11, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {12, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {13, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // TCSS xHCI Controller
  {13, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {13, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTA , IRQ_UNIQUE_NO  }, // TypeC Subsystem TBT DMA0
  {13, 3, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTA , IRQ_UNIQUE_NO  }, // TypeC Subsystem TBT DMA1
  {13, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {13, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {13, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {13, 7, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // TCSS XHCI VTIO phantom
  {14, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {14, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {14, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {14, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {14, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {14, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {14, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {14, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {15, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {16, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // THC #0
  {16, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // THC #1
  {16, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {16, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {16, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {16, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {16, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {16, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {17, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I3C Controller #1
  {17, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I3C Controller #2
  {17, 2, VALID_DEV_NO,  0          , 0            , 0            , 0            , 0              }, // TraceHub Phantom (ACPI) Function
  {17, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {17, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {17, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {17, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {17, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {18, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_ISH }, // Integrated Sensor Hub
  {18, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // P2SB 16-bit
  {18, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // CSME: PMT,  doesn't use interrupts
  {18, 3, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // IEH 16-bit
  {18, 4, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // CSME: fTPM DMA
  {18, 5, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // AON Vision (AVB)
  {18, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // SerialIo: SPI #2 - disabled
  {18, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {19, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // Partner Security Engine (PSE): HECI #1
  {19, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // Partner Security Engine (PSE): HECI #2
  {19, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // Partner Security Engine (PSE): HECI #3
  {19, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {19, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {19, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {19, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {19, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {20, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // USB 3.0 xHCI Controller
  {20, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_OTG }, // USB Device Controller (OTG)
  {20, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // Shared SRAM
  {20, 3, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CNVi WiFi
  {20, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // TraceHub Phantom (ACPI) Function
  {20, 5, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // IEH 8-bit
  {20, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // DBC.DBC Phantom (ACPI) Function
  {20, 7, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CNVi: BT
  {21, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I2C Controller #0
  {21, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I2C Controller #1
  {21, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I2C Controller #2
  {21, 3, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I2C Controller #3
  {21, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {21, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {21, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {21, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {22, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CSME: HECI #1
  {22, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CSME: HECI #2
  {22, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CSME: IDE-Redirection (IDE-R)
  {22, 3, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CSME: Keyboard and Text (KT) Redirection
  {22, 4, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CSME: HECI #3
  {22, 5, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // CSME: HECI #4
  {22, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {22, 7, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // CSME: WLAN
  {23, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {23, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {23, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {23, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {23, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {23, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {23, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {23, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {24, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // Silicon Security Engine (SSE): HECI #1
  {24, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // Silicon Security Engine (SSE): HECI #2
  {24, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // Silicon Security Engine (SSE): HECI #3
  {24, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {24, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {24, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {24, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {24, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {25, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I2C Controller #4
  {25, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo I2C Controller #5
  {25, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo UART Controller #2
  {25, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {25, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {25, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {25, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {25, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 0, VALID_DEV_NO,  0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {26, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {27, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {28, 0, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTA , IRQ_UNIQUE_NO  }, // PCI Express Port 1
  {28, 1, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTB , IRQ_UNIQUE_NO  }, // PCI Express Port 2
  {28, 2, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTC , IRQ_UNIQUE_NO  }, // PCI Express Port 3
  {28, 3, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTD , IRQ_UNIQUE_NO  }, // PCI Express Port 4
  {28, 4, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTA , IRQ_UNIQUE_NO  }, // PCI Express Port 5
  {28, 5, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTB , IRQ_UNIQUE_NO  }, // PCI Express Port 6
  {28, 6, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTC , IRQ_UNIQUE_NO  }, // PCI Express Port 7
  {28, 7, VALID_DEV_YES, PCIE_RP_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTD , IRQ_UNIQUE_NO  }, // PCI Express Port 8
  {29, 0, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {29, 1, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {29, 2, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {29, 3, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {29, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {29, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {29, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {29, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {30, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo: UART #0
  {30, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo: UART #1
  {30, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo: SPI #0
  {30, 3, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_SERIALIO}, // SerialIo: SPI #1
  {30, 4, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {30, 5, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // Not a valid PCH device
  {30, 6, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // reserved for HPET
  {30, 7, VALID_DEV_NO , 0          , 0            , 0            , 0            , 0              }, // reserved for IOAPIC
  {31, 0, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // LPC/eSPI Interface
  {31, 1, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // P2SB
  {31, 2, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // PMC
  {31, 3, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // ACE
  {31, 4, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_MUST_SET, IRQ_UNIQUE_NO  }, // SMBus Controller
  {31, 5, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_NO , 0            , 0            , 0              }, // SPI
  {31, 6, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_MUST_SET, IRQ_UNIQUE_NO  }, // GbE Controller
  {31, 7, VALID_DEV_YES, PCIE_RP_NO , INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTA , IRQ_UNIQUE_NO  }  // TraceHub
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mSocHDeviceInterruptInfoSize = ARRAY_SIZE(mSocHDeviceInterruptInfo);
