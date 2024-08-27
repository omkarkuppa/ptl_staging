/** @file
  This file provides functionality for ITSS Policy specific to PTL SOC

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PeiItssPolicyCommonLib.h>
#include <Ppi/SiPolicy.h>
#include <InterruptConfig.h>
#include <IoApicConfig.h>
#include <ConfigBlock.h>

/**
  The IRQ values for devices which use ITSS INTx->PIRQx mapping need to be set in a way
  that for each multifunctional Dxx:Fy, the same interrupt pins must map to the same IRQs.
  Those IRQ values will be used to update the ITSS.PIRx register.
  In APIC, the relationship between PIRQs and IRQs is:
  PIRQA -> IRQ16
  PIRQB -> IRQ17
  PIRQC -> IRQ18
  PIRQD -> IRQ19
  PIRQE -> IRQ20
  PIRQF -> IRQ21
  PIRQG -> IRQ22
  PIRQH -> IRQ23

  Devices which use INTx->PIRQy mapping are: cAVS(in PCI mode), SMBus, GbE, TraceHub, PCIe,
  SATA, HECI, IDE-R, KT Redirection, xHCI, Thermal Subsystem, Camera IO Host Controller, THC

  Configured IRQ values are not used if an OS chooses to be in PIC instead of APIC mode
**/

//
// mPcdPDevIntConfig table contains data pertaining to the INTx->PIRQy mapping for devices that exist on PCD-P
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_DEVICE_INTERRUPT_CONFIG mPcdDevIntConfig [] = {
  // {31, 0, PchNoInt, 0},  // LPC/eSPI Interface
  // {31, 1, PchNoInt, 0},  // P2SB
  // {31, 2, PchNoInt, 0},  // PMC
  { 31, 3, PchIntB, 17}, // ACE
  { 31, 4, PchIntC, 18}, // SMBus Controller
  // {31, 5, PchNoInt, 0},  // SPI
  { 31, 6, PchIntD, 19}, // GbE Controller
  { 31, 7, PchIntA, 16}, // TraceHub
  { 30, 0, PchIntA, 16}, // SerialIo: UART #0
  { 30, 1, PchIntB, 17}, // SerialIo: UART #1
  { 30, 2, PchIntC, 18}, // SerialIo: SPI #0
  { 30, 3, PchIntD, 19}, // SerialIo: SPI #1
  // {30, 6, PchNoInt, 0},  // Reserved for HPET
  // {30, 7, PchNoInt, 0},  // Reserved for IOAPIC
  { 28, 0, PchIntA, 16}, // PCI Express Port 1
  { 28, 1, PchIntB, 17}, // PCI Express Port 2
  { 28, 2, PchIntC, 18}, // PCI Express Port 3
  { 28, 3, PchIntD, 19}, // PCI Express Port 4
  { 28, 4, PchIntA, 16}, // PCI Express Port 5
  { 28, 5, PchIntB, 17}, // PCI Express Port 6
  { 28, 6, PchIntC, 18}, // PCI Express Port 7
  { 28, 7, PchIntD, 19}, // PCI Express Port 8
  { 25, 0, PchIntA, 20}, // SerialIo I2C Controller #4
  { 25, 1, PchIntB, 21}, // SerialIo I2C Controller #5
  { 25, 2, PchIntC, 24}, // SerialIo UART #2
  // {24, 0, PchNoInt, 0},  // SSE: HECI #1
  // {24, 1, PchNoInt, 0},  // SSE: HECI #2
  // {24, 2, PchNoInt, 0},  // SSE: HECI #3
  { 22, 0, PchIntA, 16}, // CSME: HECI #1
  { 22, 1, PchIntB, 17}, // CSME: HECI #2
  { 22, 2, PchIntC, 18}, // CSME: IDE-Redirection (IDE-R)
  { 22, 3, PchIntD, 19}, // CSME: Keyboard and Text (KT) Redirection
  { 22, 4, PchIntA, 16}, // CSME: HECI #3
  { 22, 5, PchIntB, 17}, // CSME: HECI #4
  // {22, 7, PchNoInt, 0},  // CSME: WLAN
  { 21, 0, PchIntA, 27}, // SerialIo I2C Controller #0
  { 21, 1, PchIntB, 28}, // SerialIo I2C Controller #1
  { 21, 2, PchIntC, 29}, // SerialIo I2C Controller #2
  { 21, 3, PchIntD, 30}, // SerialIo I2C Controller #3
  { 20, 0, PchIntA, 16}, // USB 3.0 xHCI Controller
  { 20, 1, PchIntB, 17}, // USB Device Controller (OTG)
  // {20, 2, PchNoInt, 0},  // Shared SRAM
  { 20, 3, PchIntC, 18}, // CNVi WiFi
  // {20, 4, PchNoInt, 0},  // TraceHub Phantom (ACPI) Function
  // {20, 5, PchNoInt, 0},  // IEH 8-bit
  // {20, 6, PchNoInt, 0},  // DBC.DBC Phantom (ACPI) Function
  // {20, 7, PchNoInt, 0}, // CNVi Bluetooth
  // {19, 0, PchNoInt, 0},  // PSE: HECI #1
  // {19, 1, PchNoInt, 0},  // PSE: HECI #1
  // {19, 2, PchNoInt, 0},  // PSE: HECI #1
  { 18, 0, PchIntA, 26}, // Integrated Sensor Hub
  // {18, 1, PchNoInt, 0},  // P2SB 16-bit
  // {18, 2, PchNoInt, 0},  // CSME: PMT Phantom (ACPI) function
  // {18, 3, PchNoInt, 0},  // IEH 16-bit
  // {18, 4, PchNoInt, 0},  // CSME: fTPM DMA Phantom (ACPI) function
  // {18, 6, PchNoInt, 0},  // SerialIo: SPI #2 - disabled
  { 17, 0, PchIntA, 31}, // SerialIo I3C Controller #1
  { 17, 1, PchIntB, 32}, // SerialIo I3C Controller #2
  // {17, 2, PchNoInt, 0},  // TraceHub Phantom (ACPI) Function
  { 16, 0, PchIntA, 23}, // THC #0
  { 16, 1, PchIntB, 22}, // THC #1
  //{ 13, 0, PchNoInt, 0}, // TCSS xHCI Controller
  //{ 13, 1, PchNoInt, 0}, // TCSS XDCI
  //{ 13, 2, PchNoInt, 0}, // TCSS TBT DMA0
  //{ 13, 3, PchNoInt, 0}, // TCSS TBT DMA1
  // {13, 7, PchNoInt, 0},  // TCSS XHCI VTIO Phantom (ACPI) Function
  // {  7, 0, PchNoInt, 0}, // TCSS PCI Express Port 21
  // {  7, 1, PchNoInt, 0}, // TCSS PCI Express Port 22
  // {  7, 2, PchNoInt, 0}, // TCSS PCI Express Port 23
  // {  7, 3, PchNoInt, 0}, // TCSS PCI Express Port 24
  {  6, 0, PchIntA, 16}, // PCI Express Root Port #9
  {  6, 1, PchIntB, 17}, // PCI Express Root Port #10
};

GLOBAL_REMOVE_IF_UNREFERENCED PCH_DEVICE_INTERRUPT_CONFIG mPcdPDevIntConfig[] = {
  { 23, 0, PchIntA, 25}, // UFS
};

//
// mPcdHDevIntConfig table contains data pertaining to the INTx->PIRQy mapping for devices that exist on PCD-H
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_DEVICE_INTERRUPT_CONFIG mPcdHDevIntConfig[] = {
  // {17, 3, PchNoInt, 0},  // LPSS Reserved #1
  // {17, 4, PchNoInt, 0},  // LPSS Reserved #2
  { 6, 2, PchIntC, 18}, // PCI Express Root Port #11
  { 6, 3, PchIntD, 19}, // PCI Express Root Port #12
};

/**
  Loads the Device Interrupt Configuration settings for the platform

  @param[in][out] NumOfDevIntConfig      Pointer to the length of DevIntConfig
  @param[in][out] DevIntConfig           Array which stores PCH device interrupt settings

  @retval None
**/
VOID
EFIAPI
LoadDeviceInterruptConfig (
  IN OUT UINT8                          *NumOfDevIntConfig,
  IN OUT PCH_DEVICE_INTERRUPT_CONFIG    DevIntConfig[]
  )
{
  UINT8                 IntConfigTableEntries;

      IntConfigTableEntries = ARRAY_SIZE (mPcdDevIntConfig);
      CopyMem (
        DevIntConfig,
        mPcdDevIntConfig,
        sizeof (mPcdDevIntConfig)
      );
    ASSERT (IntConfigTableEntries <= PCH_MAX_DEVICE_INTERRUPT_CONFIG);
    *NumOfDevIntConfig = IntConfigTableEntries;

    if (PtlIsPcdP ()) {
      CopyMem (
        &DevIntConfig[*NumOfDevIntConfig],
        mPcdPDevIntConfig,
        sizeof (mPcdPDevIntConfig)
      );
      *NumOfDevIntConfig += ARRAY_SIZE(mPcdPDevIntConfig);
    } else {
      CopyMem(
        &DevIntConfig[*NumOfDevIntConfig],
        mPcdHDevIntConfig,
        sizeof(mPcdHDevIntConfig)
      );
      *NumOfDevIntConfig += ARRAY_SIZE(mPcdHDevIntConfig);
    }
}
