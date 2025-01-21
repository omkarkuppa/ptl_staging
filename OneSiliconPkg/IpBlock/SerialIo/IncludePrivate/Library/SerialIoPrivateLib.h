/** @file
  Header file for Serial IO Private Lib implementation.

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

@par Specification
**/
#ifndef _SERIAL_IO_PRIVATE_LIB_H_
#define _SERIAL_IO_PRIVATE_LIB_H_

#include <SerialIoDevices.h>
#include <Library/SerialIoAccessLib.h>

/**
  Serial Io Pci Device State structure.
  Used to preserve current information about the device when it is configured in Pci mode prior to Pch Initialization.
**/
typedef struct {
  UINT64 PciCfgBar0;       ///< Pci Config Space Base Address Register
  UINT8  PciCfgCommand;    ///< Pci Config Space Command Register
  UINT8  PciCfgPmeCtrlSts; ///< Pci Config Space Pme Control Status
  UINT8  PprReset;         ///< MMIO Proprietary Reset Register
} SERIAL_IO_PCI_DEVICE_STATE;

// 0xA0, D0i3 And Power Control Enable Register
typedef union {
  struct {
    UINT32  PwrLatencyValue     :10;    //[RW][0-9] Power Latency Value Field
    UINT32  PwrLatencyScale     :3;     //[RW][10:12] Power Latency Scale Field
    UINT32  Reserved0           :3;     //[RW][13-15] Reserved
    UINT32  PmcRequestEn        :1;     //[RO][16] Pmc Request Enable Field
    UINT32  DeviceI3En          :1;     //[RO][17] Device Idle En Field
    UINT32  PwrGateEn           :1;     //[RO][18] D3-Hot Enable (D3HEN) / Power Gate Enable
    UINT32  SleepEn             :1;     //[RO][19] Sleep Enable Field
    UINT32  Reserved1           :1;     //[RW][20] Reserved
    UINT32  HwAutoEn            :1;     //[RW][21] Hardware Autonomous Enable
    UINT32  Reserved2           :10;    //[RW][22-31] Reserved
  } Fields;
  UINT32 Data32;
} SERIAL_IO_D0I3_AND_POWER_CONTROL;

// 0x84, Power Management Control And Status Register
typedef union {
  struct {
    UINT32  PwrState            :2;     //[RW][0-1] Power State Field
    UINT32  Reserved0           :1;     //[RW][2] Reserved
    UINT32  NoSoftReset         :1;     //[RW][3] No Soft Reset Field
    UINT32  Reserved1           :4;     //[RW][4-7] Reserved
    UINT32  PmeEn               :1;     //[RW][8] Pme Enable Field
    UINT32  Reserved2           :6;     //[RW][9-14] Reserved
    UINT32  PmeSts              :1;     //[R][15] Pme Status Field
    UINT32  Reserved3           :16;    //[RW][16-31] Reserved
  } Fields;
  UINT32 Data32;
} SERIAL_IO_PWRMG_CONTROL_AND_STATUS;

// 0x204, Host Controller reset.
typedef union {
  struct {
    UINT32  HostControlReset    :2;     //[RW][0-1] Host Controller reset.
                                        /*
                                         * 00 = Host Controller is in reset(Reset Asserted)
                                         * 01 = Reserved
                                         * 10 = Reserved
                                         * 11 = Host Controller is NOT at reset(Reset Released)
                                         */
    UINT32  DmaReset            :1;     //[RW][2] reset the dma controller
                                        /*
                                         * 0 = IP is in reset (Reset Asserted)
                                         * 1 = IP is NOT at reset (Reset Released)
                                         */
    UINT32  Reserved1           :29;    //[RW][3-31] Reserved
  } Fields;
  UINT32 Data32;
} SERIAL_IO_HOST_CONTROL_RESET;

// 0xXXX Pci Configuration Control Register
typedef union {
  struct {
    UINT32  PciCfgDisable       :1;     //[RW][0] Pci Cfg Disable Field
                                        /*
                                         * 1 = PCI configuration accesses return UR response
                                         * 0 = PCI configuration accesses are supported
                                         */
    UINT32  AcpiIntEn           :1;     //[RW][1] reset the dma controller
    UINT32  PmeSupport          :5;      //[RW][2-6] Pme Support Field
    UINT32  BarDisable          :1;      //[RW][7] Bar Disable Field
    UINT32  InterruptPin        :4;      //[RW][8-11] Interrupt Pin Field
                                         /* Interrupt Pin: This register indicates
                                          * the values to be used for Global Interrupts.
                                          */
    UINT32  AcpiIrq             :8;     //[RW][12-19] Acpi Irq Field
    UINT32  PciIrq              :8;     //[RW][20-27] Pci Irq Field
    UINT32  DisPciIdleCap       :1;     //[RW][28] Dis Pci Idle Cap Field
    UINT32  DisMsiBit           :1;     //[RW][29] Dis Msi Cap Field
    UINT32  Reserved0           :2;     //[RW][30-31] Reserved
  } Fields;
  UINT32 Data32;
} SERIAL_IO_PCI_CFG_CONTROL;

/**
  Checks if higher functions are enabled.
  Used for Function 0 Serial Io Device disabling

  @param[in] DeviceNum       Device Number

  @retval TRUE               At least one higher function device is enabled
          FALSE              Higher functions are disabled
**/
BOOLEAN
SerialIoHigherFunctionsEnabled (
  IN UINT8                    DeviceNum
  );

/**
  Places SerialIo device in D3

  @param[in] PciCfgBase       Pci Config Offset

**/
VOID
SerialIoSetD3 (
  IN UINT64                    PciCfgBase
  );

/**
  Places SerialIo device in D0

  @param[in] PciCfgBase       Pci Config Offset

**/
VOID
SerialIoSetD0 (
  IN UINT64                    PciCfgBase
  );

/**
  Allows Memory Access

  @param[in] PciCfgBase       Pci Config Offset
  @param[in] Hidden           Mode that determines access type

**/
VOID
SerialIoEnableMse (
  IN UINT64                    PciCfgBase,
  IN BOOLEAN                   Hidden
  );

/**
  Disable SerialIo memory access

  @param[in] PciCfgBase       Pci Config Offset

**/
VOID
SerialIoDisableMse (
  IN UINT64                    PciCfgBase
  );

/**
  Disable SerialIo memory encoding
  Designated for Pci modes

  @param[in] PciCfgBase       Pci Config Offset
  @param[in] RemoveTempBar    Remove temporary mem base address or not

**/
VOID
SerialIoMmioDisable (
  IN UINT64                    PciCfgBase,
  IN BOOLEAN                   RemoveBar
  );

/**
  Configures Serial IO Controller before control is passd to the OS

  @param[in] SpiNumber         SPI Number
  @param[in] SpiDeviceConfig   SerialIo SPI Config

**/
VOID
SerialIoSpiBootHandler (
  IN UINT8                      SpiNumber,
  IN SERIAL_IO_SPI_CONFIG       *SpiDeviceConfig
  );

/**
  Sets Pme Control Status and Command register values required for S3 Boot Script

  @param[in]     SpiNumber         SPI Number
  @param[in]     SpiDeviceConfig   SerialIo SPI Config
  @param[in/out] S3PciCfgBase      S3 Boot Script Pci Config Base
  @param[in/out] Command           Pci Command register data to save
  @param[in/out] Pme               Pci Pme Control register data to save

**/
VOID
SerialIoSpiS3Handler (
  IN     UINT8                 SpiNumber,
  IN     SERIAL_IO_SPI_CONFIG  *SpiDeviceConfig,
  IN OUT UINT64                *S3PciCfgBase,
  IN OUT UINT32                *Command,
  IN OUT UINT32                *Pme
  );

/**
  Returns UART S3 boot script PCI address

  @param[in] UartNumber         UART Number

  @retval    UART  S3 boot script PCI address
**/
UINT64
GetSerialIoUartS3PciBase (
  IN UINT8        UartNumber
  );

/**
  Returns SPI S3 boot script PCI address

  @param[in] UartNumber         UART Number

  @retval    SPI S3 boot script PCI address
**/
UINT64
GetSerialIoSpiS3PciBase (
  IN UINT8        SpiNumber
  );

/**
  Returns I2C S3 boot script PCI address

  @param[in] I2cNumber         I2C Number

  @retval    I2C  S3 boot script PCI address
**/
UINT64
GetSerialIoI2cS3PciBase (
  IN UINT8        I2cNumber
  );

/**
  Returns I3C S3 boot script PCI address

  @param[in] I3cNumber         I3C Number

  @retval    I3C  S3 boot script PCI address
**/
UINT64
GetSerialIoI3cS3PciBase (
  IN UINT8        I3cNumber
  );

/**
  Checks if UART is Hidden, and it's Pci Config space available

  @param[in]      UartNumber     Selects Serial IO UART device

  @retval   TRUE             UART is in hidden mode
  @retval   FALSE            UART is not in hidden mode
**/
BOOLEAN
IsSerialIoUartHidden (
  IN UINT8               UartNumber
  );

/**
  Configures Serial IO Controller before control is passd to the OS

  @param[in] UartNumber         UART Number
  @param[in] UartDeviceConfig   SerialIo UART Config

**/
VOID
SerialIoUartBootHandler (
  IN UINT8                      UartNumber,
  IN SERIAL_IO_UART_CONFIG      *UartDeviceConfig
  );

/**
  Sets Pme Control Status and Command register values required for S3 Boot Script

  @param[in]     UartNumber         UART Number
  @param[in]     UartDeviceConfig   SerialIo UART Config
  @param[in/out] S3PciCfgBase       S3 Boot Script Pci Config Base
  @param[in/out] Command            Pci Command register data to save
  @param[in/out] Pme                Pci Pme Control register data to save

**/
VOID
SerialIoUartS3Handler (
  IN     UINT8                  UartNumber,
  IN     SERIAL_IO_UART_CONFIG  *UartDeviceConfig,
  IN OUT UINT64                 *S3PciCfgBase,
  IN OUT UINT32                 *Command,
  IN OUT UINT32                 *Pme
  );

/**
  Configures Serial IO Controller before control is passd to the OS

  @param[in] I2cNumber         I2C Number
  @param[in] I2cDeviceConfig   SerialIo I2C Config

**/
VOID
SerialIoI2cBootHandler (
  IN UINT8                      I2cNumber,
  IN SERIAL_IO_I2C_CONFIG       *I2cDeviceConfig
  );

/**
  Sets Pme Control Status and Command register values required for S3 Boot Script

  @param[in]     I2cNumber         I2C Number
  @param[in]     I2cDeviceConfig   SerialIo I2C Config
  @param[in/out] S3PciCfgBase      S3 Boot Script Pci Config Base
  @param[in/out] Command           Pci Command register data to save
  @param[in/out] Pme               Pci Pme Control register data to save

**/
VOID
SerialIoI2cS3Handler (
  IN     UINT8                 I2cNumber,
  IN     SERIAL_IO_I2C_CONFIG  *I2cDeviceConfig,
  IN OUT UINT64                *S3PciCfgBase,
  IN OUT UINT32                *Command,
  IN OUT UINT32                *Pme
  );

/**
  Configures Serial IO Controller before control is passd to the OS

  @param[in] I3cNumber         I3c Number
  @param[in] I3cMode           SerialIo I3c Mode

**/
VOID
SerialIoI3cBootHandler (
  IN UINT8                      I3cNumber,
  IN UINT8                      I3cMode
  );

/**
  Sets Pme Control Status and Command register values required for S3 Boot Script

  @param[in]     I3cNumber         I3c Number
  @param[in]     I3cMode           SerialIo I3c Mode
  @param[in/out] S3PciCfgBase      S3 Boot Script Pci Config Base
  @param[in/out] Command           Pci Command register data to save
  @param[in/out] Pme               Pci Pme Control register data to save

**/
VOID
SerialIoI3cS3Handler (
  IN     UINT8                 I3cNumber,
  IN     UINT8                 I3cMode,
  IN OUT UINT64                *S3PciCfgBase,
  IN OUT UINT32                *Command,
  IN OUT UINT32                *Pme
  );

#endif // _SERIAL_IO_PRIVATE_LIB_H_
