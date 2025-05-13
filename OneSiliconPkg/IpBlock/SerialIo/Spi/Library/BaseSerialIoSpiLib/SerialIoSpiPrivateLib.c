/** @file
  Basic SPI Library that allows SPI communication through Read/Write/WriteRead operations.

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

#include <Base.h>
#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/Pci22.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/TimerLib.h>
#include <Library/SerialIoAccessLib.h>
#include <Library/SerialIoPrivateLib.h>
#include <SerialIoSpiPrivateLib.h>
#include <Register/SerialIoRegs.h>
#include <Register/SerialIoSpiRegs.h>

/**
  Wait For Serial Io SPI controller to be in Idle state by checking the Busy bit

  @param[in]  SpiBaseAddress - Base Address of SPI Controller
**/
VOID
SerialIoSpiWaitIdle (
  IN UINT64 SpiBaseAddress
  )
{
  UINT8                     Timeout;
  SERIAL_IO_SPI_STATUS_REG  SpiStatusRegister;

  Timeout = SpiWaitIdleTimeout;
  SpiStatusRegister.Data32 = MmioRead32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSSR);
  while (SpiStatusRegister.Fields.Busy && (Timeout > 0)) {
    MicroSecondDelay (SpiSingleOperationDelay);
    Timeout--;
    SpiStatusRegister.Data32 = MmioRead32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSSR);
  }
}

/**
  Return Serial Io SPI controller Tx Fifo Not Full status

  @param[in]  SpiBaseAddress - Base Address of SPI Controller
**/
BOOLEAN
SerialIoSpiTxFifoNotFull (
  IN UINT64 SpiBaseAddress
  )
{
  SERIAL_IO_SPI_STATUS_REG  SpiStatusRegister;
  SpiStatusRegister.Data32 = MmioRead32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSSR);
  return (BOOLEAN) SpiStatusRegister.Fields.TxFifoNotFull;
}

/**
  Return Serial Io SPI controller Rx Fifo Not Empty status

  @param[in]  SpiBaseAddress - Base Address of SPI Controller
**/
BOOLEAN
SerialIoSpiRxFifoNotEmpty (
  IN UINT64 SpiBaseAddress
  )
{
  SERIAL_IO_SPI_STATUS_REG  SpiStatusRegister;
  SpiStatusRegister.Data32 = MmioRead32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSSR);
  return (BOOLEAN) SpiStatusRegister.Fields.RxFifoNotEmpty;
}

/**
  Wait For Serial Io SPI controller to have no interrupts pending

  @param[in]  SpiBaseAddress - Base Address of SPI Controller
**/
VOID
STATIC
SerialIoSpiWaitNoInt (
  IN UINT64 SpiBaseAddress
  )
{
  UINT8                                Count;
  SERIAL_IO_SPI_RW_INTERRUPT_TEST_REG  SpiRWInterruptTest;
  //
  // Clear all interrupts
  //
  SpiRWInterruptTest.Data32 = MmioRead32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSITR);
  //
  // Ensure there are no interrupts in progress.
  //
  Count = 0;
  while (SpiRWInterruptTest.Fields.TestTxFifoServiceRequest &&
         SpiRWInterruptTest.Fields.TestRxFifoServiceRequest &&
         SpiRWInterruptTest.Fields.TestReceiveFxOverrun &&
         (Count < SpiNoIntCounter)) {
    SpiRWInterruptTest.Data32 = MmioRead32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSITR);
    Count ++;
  }
}

/**
  This function does the basic init work for SPI controller.

  @param[in]  SpiBaseAddress  - Base Address of SPI Controller
  @param[in]  Frequency         Bus clock frequency (in kHz)
  @param[in]  ClockPhase        0 - Clock is inactive one cycle at the start of a frame and cycle at the end of a frame
                                     1 - Clock is inactive for one halfcycle at the start of a frame and one cycle at the end of a frame
  @param[in]  ClockIdlePolarity Clock Idle Polarity 0 = Low, 1 = High
  @param[in]  CsActivePolarity  Selects SPI ChipSelect signal polarity, 0 = Low, 1 = High
**/
VOID
STATIC
SerialIoSpiControllerBusInit (
  IN UINT64          SpiBaseAddress,
  IN UINT32          Frequency,
  IN UINT8           ClockPhase,
  IN UINT8           ClockIdlePolarity,
  IN UINT8           CsActivePolarity
  )
{
  SERIAL_IO_SPI_CONTROL0_REG  SpiControl0;
  SERIAL_IO_SPI_CONTROL1_REG  SpiControl1;
  SERIAL_IO_SPI_CLOCK         SpiClock;
  UINT16                      SpiMClock;

  SpiControl0.Data32 = MmioRead32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSCR0);
  SpiControl0.Fields.DataSizeSelect = V_SERIAL_IO_MEM_SPI_SSCR0_DSS_8;
  SpiControl0.Fields.FrameFormat = 0x0; //00 - Motorola Serial Peripheral Interface (SPI)
  SpiControl0.Fields.ExtClockSelect = 0x0;
  SpiControl0.Fields.SynchronousSerialPortEnable = 0x1;
  SpiControl0.Fields.SerialClockRateValue = 0x0; // Use the M/N divider
  SpiControl0.Fields.ExtDataSizeSelect = 0x0; // 4 - 16 bits
  SpiControl0.Fields.NetworkClockSelect = 0x0;
  SpiControl0.Fields.RxFifoOverrunIntMask = 0x1;
  SpiControl0.Fields.TxFifoOverrunIntMask = 0x1;
  MmioWrite32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSCR0, SpiControl0.Data32);

  SpiControl1.Data32 = MmioRead32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSCR1);
  SpiControl1.Fields.RxFifoIntEnable = 0x0;
  SpiControl1.Fields.TxFifoIntEnable = 0x0;
  SpiControl1.Fields.LoopBackMode = 0x0;
  SpiControl1.Fields.ClockIdlePolarity = ClockIdlePolarity;
  SpiControl1.Fields.ClockPhase = ClockPhase;
  SpiControl1.Fields.EnableFifoWriteRead = 0x0;
  SpiControl1.Fields.SelectTxRxFifo = 0x0;
  SpiControl1.Fields.InvertFrameSignal = CsActivePolarity;
  SpiControl1.Fields.PeripheralTrailingByteIntEnable = 0x0;
  SpiControl1.Fields.ReceiveFifoTimeoutintEnable = 0x0;
  SpiControl1.Fields.ReceiveFifoServiceRequestEnable = 0x0;
  SpiControl1.Fields.TransmitFifoServiceRequestEnable = 0x0;
  SpiControl1.Fields.Trail = 0x0;
  SpiControl1.Fields.ReceiveWithoutTransmit = 0x0;
  MmioWrite32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSCR1, SpiControl1.Data32);

  //
  // Configure M/N dividers
  //
  SpiClock.Data32 = MmioRead32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_CLOCKS);
  //M = (spi_clock * 32767) / base_clock;
  SpiMClock = (UINT16) DivU64x64Remainder (MultU64x32 (Frequency, 32767), DEFAULT_SPI_CLOCK_FREQUENCY, NULL);
  SpiClock.Fields.M = SpiMClock;
  SpiClock.Fields.N = 32767;
  SpiClock.Fields.ClockUpdate = 0x0;
  SpiClock.Fields.ClockEnable = 0x1;
  MmioWrite32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_CLOCKS, SpiClock.Data32);
  MicroSecondDelay (SpiClockUpdate);
  SpiClock.Fields.ClockUpdate = 0x1;
  MmioWrite32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_CLOCKS, SpiClock.Data32);
  MicroSecondDelay (SpiClockUpdate);
  SpiClock.Fields.ClockUpdate = 0x0;
  MmioWrite32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_CLOCKS, SpiClock.Data32);
  MicroSecondDelay (SpiClockUpdate);
  SpiClock.Data32 = MmioRead32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_CLOCKS);

  SerialIoSpiWaitNoInt (SpiBaseAddress);
}

/**
  Set Serial Io SPI controller Active Chip Select

  @param[in]  SpiBaseAddress - Base Address of SPI Controller
  @param[in]  CsOutput       - Default CS used by the SPI HC. 0 = CS0, 1 = CS1, CS2, CS3.
  @param[in]  CsMode         - Sets Chip Select Control mode Hardware or Software. 0 = HW Control, 1 = SW Control
  @param[in]  CsState        - 0 = CS is set to low; 1 = CS is set to high
  @param[in]  CsIdlePolarity   Sets Inactive/Idle polarity of CSn (CsOutput)
**/
VOID
SerialIoSpiSetCsControl (
  IN UINT64 SpiBaseAddress,
  IN UINT8  CsOutput,
  IN UINT8  CsMode,
  IN UINT8  CsState,
  IN UINT8  CsIdlePolarity
  )
{
  SERIAL_IO_SPI_CS_CONTROL_REG  SpiCsControl;

  SpiCsControl.Data32 = MmioRead32 ((UINTN) (SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SPI_CS_CONTROL));
  //
  // Set CS mode
  //
  SpiCsControl.Fields.CsMode = CsMode;
  //
  // Set CS Output Select
  //
  SpiCsControl.Fields.CsOutputSelect = CsOutput;
  //
  // Set CS state
  //
  SpiCsControl.Fields.CsState = CsState;
  if (CsOutput == 0) {
    SpiCsControl.Fields.Cs0Polarity = CsIdlePolarity;
  }
  if (CsOutput == 1) {
    SpiCsControl.Fields.Cs1Polarity = CsIdlePolarity;
  }
  MmioWrite32 ((UINTN) (SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SPI_CS_CONTROL), SpiCsControl.Data32);
}

/**
  Deactivates SPI device
  - Revert CS to Idle state
  - Disable SSP

  @param[in] SpiBaseAddress  - Base Address of SPI Controller
  @param[in] CsOutputSelect    Select which SPI CS Signal is to be driven by the SPI Frame (0 = CS0; 1=CS1 etc.)
  @param[in] CsActivePolarity  Selects SPI ChipSelect signal polarity, 0 = Low, 1 = High
**/
VOID
SerialIoSpiDeactivate (
  IN UINT64 SpiBaseAddress,
  IN UINT8  CsOutputSelect,
  IN UINT8  CsActivePolarity
  )
{
  SERIAL_IO_SPI_CONTROL0_REG  SpiControl0;
  //
  // Deactivate CS
  //
  SerialIoSpiSetCsControl (SpiBaseAddress, CsOutputSelect, SpiCsModeSoftware, SpiCsStateDeAsserted, !(CsActivePolarity & BIT0));
  //
  // Disable SPI Operation
  //
  SpiControl0.Data32 = MmioRead32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSCR0);
  SpiControl0.Fields.SynchronousSerialPortEnable = 0;
  MmioWrite32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSCR0, SpiControl0.Data32);
}

/**
  Enable Spi device when in PCI only if it has assigned Memory resources

  - Enable Memory space decoding
  - Place device in D0
  - Get it out of reset

  @param[in]       PciCfgBase       Pci Config Base Address of specified SPI controller
  @param[in][out]  SpiBaseAddress   MMIO Base Address of specified SPI controller

  @retval   TRUE    SPI successfully enabled
  @retval   FALSE   SPI does not have memory assigned or SDA/SCL are not high
**/
BOOLEAN
STATIC
SerialIoSpiPciEnable (
  IN UINTN          PciCfgBase,
  IN OUT UINT64     *SpiBaseAddress
  )
{
  SERIAL_IO_SPI_RESETS                SpiResets;
  SERIAL_IO_PWRMG_CONTROL_AND_STATUS  SpiPwrMgmControlStsReg;

  *SpiBaseAddress = (UINT64) ((PciSegmentRead32 ((UINTN) (PciCfgBase + 0x10)) & 0xFFFFF000) + LShiftU64 (PciSegmentRead32 ((UINTN) (PciCfgBase + 0x10 + 4)), 32));
  if ((*SpiBaseAddress == 0xFFFFFFFFFFFFF000) || (*SpiBaseAddress == 0x0)) { //Device is still not enabled, or there is no BAR assigned
    DEBUG ((DEBUG_WARN, "SerialIoSpiPciEnable No BAR assigned 0x%X\n", *SpiBaseAddress ));
    return FALSE;
  }

  PciSegmentOr8 (PciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE); // Enable MSE

  SpiPwrMgmControlStsReg.Data32 = PciSegmentRead32 (PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS);
  SpiPwrMgmControlStsReg.Fields.PwrState = 0x0; // D0 State
  PciSegmentWrite32 (PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS, SpiPwrMgmControlStsReg.Data32);
  PciSegmentRead8 (PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS); // Dummy Read after modifying PME

  SpiResets.Data32 = MmioRead32 ((UINTN) *SpiBaseAddress + R_SERIAL_IO_MEM_RESETS);
  SpiResets.Fields.ResetSpi = 3;
  SpiResets.Fields.ResetDma = 0;
  MmioWrite32 ((UINTN) *SpiBaseAddress + R_SERIAL_IO_MEM_RESETS, SpiResets.Data32);

  return TRUE;
}

/**
  Set up the SPI Controller for reading and/or writing

  @param[in]       PciCfgBase        Pci Config Base Address of specified SPI controller
                                     @Note: If SPI Number is provided this parameter is ignored and can be left as 0
  @param[in]       SpiNumber         Optional: SPI controller applicable on bus 0 only
  @param[in][out]  SpiBaseAddress    MMIO Base Address of specified I2C controller
  @param[in]       Frequency         Bus clock frequency (in kHz)
  @param[in]       ClockIdlePolarity Clock Idle Polarity 0 = Low, 1 = High
  @param[in]       ClockPhase        0 - Clock is inactive one cycle at the start of a frame and one cycle at the end of a frame
                                     1 - Clock is inactive for one halfcycle at the start of a frame and one cycle at the end of a frame
  @param[in]       CsOutputSelect    Select which SPI CS Signal is to be driven by the SPI Frame (0 = CS0; 1=CS1 etc.)
  @param[in]       CsActivePolarity  Selects SPI ChipSelect signal polarity, 0 = Low, 1 = High
  @param[in]       AssertCs          FALSE - CS will remain in De-Asserted case. Used for scenario when external CS is used (ex GPIO)
                                     TRUE - CS either CS0/CS1 will be asserted once SPI transaction starts

  @retval   EFI_TIMEOUT        Timeout occurred in SPI disable/enable
  @retval   EFI_SUCCESS        W/R successfully initialized
  @retval   EFI_DEVICE_ERROR   Serial Io Spi Device is still not enabled or there is no BAR assigned
**/
EFI_STATUS
SerialIoSpiInitializeWriteRead (
  IN UINTN        PciCfgBase,
  IN UINT8        *SpiNumber  OPTIONAL,
  IN OUT UINT64   *SpiBaseAddress,
  IN UINTN        Frequency,
  IN UINT8        ClockIdlePolarity,
  IN UINT8        ClockPhase,
  IN UINT8        CsOutputSelect,
  IN UINT8        CsActivePolarity,
  IN BOOLEAN      AssertCs
  )
{
  UINT8  Count;
  SERIAL_IO_SPI_CONTROL0_REG  SpiControl0;
  //
  // If SpiNumber is provided, override PciCfgBase
  //
  if (SpiNumber) {
    PciCfgBase = (UINTN) LpssSpiPciCfgBase (*SpiNumber);
  }
  if (!SerialIoSpiPciEnable (PciCfgBase, SpiBaseAddress)) {
    DEBUG ((DEBUG_WARN, "SerialIoSpiInitializeWriteRead Device is still not enabled or there is no BAR assigned\n"));
    return EFI_DEVICE_ERROR;
  }
  //
  // 1.Set CS mode to SW, deassert CS line before changing CPL, CPH, IFS settings
  //
  SerialIoSpiSetCsControl ((UINTN) *SpiBaseAddress, CsOutputSelect, SpiCsModeSoftware, SpiCsStateDeAsserted, !(CsActivePolarity & BIT0));
  //
  // 2. Disable controller
  //
  SpiControl0.Data32 = MmioRead32 ((UINTN) *SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSCR0);
  SpiControl0.Fields.SynchronousSerialPortEnable = 0;
  MmioWrite32 ((UINTN) *SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSCR0, SpiControl0.Data32);
  //
  // 3. Spi initalization
  //
  SerialIoSpiControllerBusInit ((UINTN) *SpiBaseAddress, (UINT32) Frequency, ClockPhase, ClockIdlePolarity, CsActivePolarity);
  //
  // 4. Make sure SPI is not running
  //
  SerialIoSpiWaitIdle (*SpiBaseAddress);
  Count = 0;
  while ((MmioRead32 ((UINTN) *SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SIRF) > 0) && (Count < SpiNoIntCounter)) {
    MmioRead32 ((UINTN) *SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SIRF);
    Count++;
  }
  if (Count == SpiNoIntCounter) {
    return EFI_TIMEOUT;
  }
  //
  // 5. Activate CS
  //
  if (AssertCs == TRUE) {
    SerialIoSpiSetCsControl ((UINTN) *SpiBaseAddress, CsOutputSelect, SpiCsModeSoftware, SpiCsStateAsserted, !(CsActivePolarity & BIT0));
  }
  return EFI_SUCCESS;
}
