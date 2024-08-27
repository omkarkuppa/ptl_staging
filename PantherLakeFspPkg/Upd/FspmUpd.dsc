## @file
#  Platform description.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2023 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.
#
# @par Specification
##

  ################################################################################
  #
  # UPDs consumed in FspMemoryInit Api
  #
  ################################################################################
  # !BSF FIND:{PTLUPD_M}
  # !HDR COMMENT:{FSP_UPD_HEADER:FSP UPD Header}
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:START}
  # FspmUpdSignature: {PTLUPD_M}
  gPlatformFspPkgTokenSpaceGuid.Signature                   | * | 0x08 | 0x4D5F4450554C5450
  # !BSF NAME:{FspmUpdRevision}  TYPE:{None}
  gPlatformFspPkgTokenSpaceGuid.Revision                    | * | 0x01 | 0x02
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:END}
  gPlatformFspPkgTokenSpaceGuid.Reserved                    | * | 0x17 | {0x00}

  # !HDR COMMENT:{FSPM_ARCH2_UPD:Fsp M Architecture UPD}
  # !HDR EMBED:{FSPM_ARCH2_UPD:FspmArchUpd:START}
  gPlatformFspPkgTokenSpaceGuid.Revision                    | * | 0x01 | 0x03

  gPlatformFspPkgTokenSpaceGuid.Reserved                    | * | 0x03 | {0x00}

  gPlatformFspPkgTokenSpaceGuid.Length                      | * | 0x04 | 0x40

  # !HDR STRUCT:{EFI_PHYSICAL_ADDRESS}
  gPlatformFspPkgTokenSpaceGuid.NvsBufferPtr                | * | 0x08 | 0x00000000

  # !HDR STRUCT:{EFI_PHYSICAL_ADDRESS}
  gPlatformFspPkgTokenSpaceGuid.StackBase                   | * | 0x08 | 0xFEF17F00

  gPlatformFspPkgTokenSpaceGuid.StackSize                   | * | 0x08 | 0x28000

  gPlatformFspPkgTokenSpaceGuid.BootLoaderTolumSize         | * | 0x04 | 0x00000000

  gPlatformFspPkgTokenSpaceGuid.BootMode                    | * | 0x04 | 0x00

  # !HDR STRUCT:{EFI_PHYSICAL_ADDRESS}
  gPlatformFspPkgTokenSpaceGuid.FspEventHandler             | * | 0x08 | 0x00

  # !HDR EMBED:{FSPM_ARCH2_UPD:FspmArchUpd:END}
  gPlatformFspPkgTokenSpaceGuid.Reserved1                   | * | 0x10 | {0x00}

  # !HDR COMMENT:{FSP_M_CONFIG:FSP M Configuration}
  # !HDR EMBED:{FSP_M_CONFIG:FspmConfig:START}

  ## SERIAL_IO_UART_CONFIG Start

  # !BSF NAME:{Serial Io Uart Debug Mode} TYPE:{Combo}
  # !BSF OPTION:{0:SerialIoUartDisabled, 1:SerialIoUartPci, 2:SerialIoUartHidden, 3:SerialIoUartCom, 4:SerialIoUartSkipInit}
  # !BSF HELP:{Select SerialIo Uart Controller mode}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugMode               | * | 0x01 | 0x02

  # !BSF NAME:{Serial Io Uart Debug Auto Flow} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables UART hardware flow control, CTS and RTS lines.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugAutoFlow           | * | 0x01 | 0x0

  # !BSF NAME:{SerialIoUartDebugRxPinMux - FSPT} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Select RX pin muxing for SerialIo UART used for debug}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugRxPinMux           | * | 0x04 | 0x0

  # !BSF NAME:{SerialIoUartDebugTxPinMux - FSPM} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Select TX pin muxing for SerialIo UART used for debug}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugTxPinMux           | * | 0x04 | 0x0

  # !BSF NAME:{SerialIoUartDebugRtsPinMux - FSPM} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select SerialIo Uart used for debug Rts pin muxing. Refer to GPIO_*_MUXING_SERIALIO_UARTx_RTS* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugRtsPinMux          | * | 0x04 | 0x0

  # !BSF NAME:{SerialIoUartDebugCtsPinMux - FSPM} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select SerialIo Uart used for debug Cts pin muxing. Refer to GPIO_*_MUXING_SERIALIO_UARTx_CTS* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugCtsPinMux          | * | 0x04 | 0x0
  # !BSF NAME:{Serial Io Uart Debug Pci Base} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Specify PciCfgBase address. Allows for SerialIO UART functionality outside Bus 0}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugPciCfgBase         | * | 0x04 | 0x0

  # !BSF NAME:{SerialIo Uart PowerGating} TYPE:{Combo}
  # !BSF OPTION:{0:Disabled, 1:Enabled, 2:Auto}
  # !BSF HELP:{Select SerialIo Uart Powergating mode}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartPowerGating             | * | 0x01 | 0x02

  ## SERIAL_IO_UART_CONFIG End

  #
  # Pch Pre-Mem Block End
  #

  #
  # SI Pre-Mem Block Start
  #
  # !BSF PAGE:{SI1}

  ## SI_PREMEM_CONFIG Start

  # !BSF NAME:{Platform Debug Option} TYPE:{Combo} OPTION:{0:Disabled, 2:Enabled Trace Active, 4:Enabled Trace Ready, 6:Enable Trace Power-Off, 7:Manual}
  # !BSF HELP:{Enabled Trace active: TraceHub is enabled and trace is active, blocks s0ix.\n\nEnabled Trace ready: TraceHub is enabled and allowed S0ix.\n\nEnabled Trace power off: TraceHub is powergated, provide setting close to functional low power state\n\nManual: user needs to configure Advanced Debug Settings manually, aimed at advanced users}
  gPlatformFspPkgTokenSpaceGuid.PlatformDebugOption                 | * | 0x01 | 0x00

  ## SI_PREMEM_CONFIG End

  #
  # SI Pre-Mem Block End
  #

  #
  # Debug Pre-Mem Block Start

  # !BSF PAGE:{DEBUG}
  # !BSF NAME:{Debug Interfaces} TYPE:{EditNum, HEX, (0x00,0x3F)}
  # !BSF HELP:{Debug Interfaces. BIT0-RAM, BIT1-UART, BIT3-USB3, BIT4-Serial IO, BIT5-TraceHub, BIT2 - Not used.}
  gPlatformFspPkgTokenSpaceGuid.PcdDebugInterfaceFlags                  | * | 0x01 | 0x32
  # !BSF NAME:{TdxActmModuleAddr}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Base address of Tdx Actm module, used for launching the Actm}
  gPlatformFspPkgTokenSpaceGuid.TdxActmModuleAddr                   | * | 0x08 | 0x0000000000000000

  # !BSF NAME:{TdxActmModuleSize}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{size of Tdx Actm module, used for launching the Actm}
  gPlatformFspPkgTokenSpaceGuid.TdxActmModuleSize                   | * | 0x04 | 0x00000000

  ## CPU_SECURITY_PREMEM_CONFIG End

  # !BSF NAME:{PcdSerialDebugLevel} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Error Only, 2:Error and Warnings, 3:Load Error Warnings and Info, 4:Load Error Warnings and Info & Event, 5:Load Error Warnings Info and Verbose}
  # !BSF HELP:{Serial Debug Message Level. 0:Disable, 1:Error Only, 2:Error & Warnings, 3:Load, Error, Warnings & Info, 4:Load, Error, Warnings, Info & Event, 5:Load, Error, Warnings, Info & Verbose.}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialDebugLevel       | * | 0x01 | 0x03

  # !BSF NAME:{SerialDebugMrcLevel} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Error Only, 2:Error and Warnings, 3:Load Error Warnings and Info, 4:Load Error Warnings and Info & Event, 5:Load Error Warnings Info and Verbose}
  # !BSF HELP:{MRC Serial Debug Message Level. 0:Disable, 1:Error Only, 2:Error & Warnings, 3:Load, Error, Warnings & Info, 4:Load, Error, Warnings, Info & Event, 5:Load, Error, Warnings, Info & Verbose.}
  gPlatformFspPkgTokenSpaceGuid.SerialDebugMrcLevel      | * | 0x01 | 0x03

  # !BSF NAME:{Serial Io Uart Debug Controller Number} TYPE:{Combo}
  # !BSF OPTION:{0:SerialIoUart0, 1:SerialIoUart1, 2:SerialIoUart2}
  # !BSF HELP:{Select SerialIo Uart Controller for debug. Note: If UART0 is selected as CNVi BT Core interface, it cannot be used for debug purpose.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugControllerNumber   | * | 0x01 | 0

  # !BSF NAME:{Serial Io Uart Debug Parity} TYPE:{Combo}
  # !BSF OPTION:{0: DefaultParity, 1: NoParity, 2: EvenParity, 3: OddParity}
  # !BSF HELP:{Set default Parity.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugParity             | * | 0x01 | 0x1

  # !BSF NAME:{Serial Io Uart Debug BaudRate} TYPE:{EditNum, DEC, (0,6000000)}
  # !BSF HELP:{Set default BaudRate Supported from 0 - default to 6000000. Recommended values 9600, 19200, 57600, 115200, 460800, 921600, 1500000, 1843200, 3000000, 3686400, 6000000}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugBaudRate           | * | 0x4 | 115200

  # !BSF NAME:{Serial Io Uart Debug Stop Bits} TYPE:{Combo}
  # !BSF OPTION:{0: DefaultStopBits, 1: OneStopBit, 2: OneFiveStopBits, 3: TwoStopBits}
  # !BSF HELP:{Set default stop bits.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugStopBits           | * | 0x01 | 0x1

  # !BSF NAME:{Serial Io Uart Debug Data Bits} TYPE:{EditNum, HEX, (0x0,0x08)}
  # !BSF OPTION:{5:5BITS, 6:6BITS, 7:7BITS, 8:8BITS}
  # !BSF HELP:{Set default word length. 0: Default, 5,6,7,8}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugDataBits           | * | 0x01 | 0x8


  ## DEBUG_CONFIG_DATA_HOB End

  # !BSF NAME:{Serial Io Uart Debug Mmio Base} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select SerialIo Uart default MMIO resource in SEC/PEI phase when PcdSerialIoUartMode = SerialIoUartPci.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugMmioBase           | * | 0x04 | 0xFE02C000

  # !BSF NAME:{ISA Serial Base selection} TYPE:{Combo}
  # !BSF OPTION:{0:0x3F8, 1:0x2F8}
  # !BSF HELP:{Select ISA Serial Base address. Default is 0x3F8.}
  gPlatformFspPkgTokenSpaceGuid.PcdIsaSerialUartBase                | * | 0x01 | 0x00

  # !BSF NAME:{PcdSerialDebugBaudRate} TYPE:{Combo}
  # !BSF OPTION:{3:9600, 4:19200, 6:56700, 7:115200}
  # !BSF HELP:{Baud Rate for Serial Debug Messages. 3:9600, 4:19200, 6:56700, 7:115200.}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialDebugBaudRate              | * | 0x01 | 0x07

  # !BSF NAME:{Post Code Output Port}
  # !BSF TYPE:{EditNum, HEX, (0x0, 0xFFFF)}
  # !BSF HELP:{This option configures Post Code Output Port}
  gPlatformFspPkgTokenSpaceGuid.PostCodeOutputPort                  | * | 0x02 | 0x80

  # !BSF NAME:{Enable/Disable I2cPostcode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable (Default): Postcode via I2C, Disable: Postcode via Port80}
  gPlatformFspPkgTokenSpaceGuid.I2cPostCodeEnable                   | * | 0x01 | 0x0

  #
  # Debug Pre-Mem Block End
  #

  #
  # SA Pre-Mem Block Start
  #
  # !BSF PAGE:{SA1}

  ## TCSS_PEI_PREMEM_CONFIG Start

  # !BSF NAME:{TCSS USB HOST (xHCI) Enable}}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{Set TCSS XHCI. 0:Disabled  1:Enabled - Must be enabled if xDCI is enabled below}
  gPlatformFspPkgTokenSpaceGuid.TcssXhciEn                | * | 0x01 | 0x01

  ## TCSS_PEI_PREMEM_CONFIG End

  # !HDR EMBED:{FSP_M_CONFIG:FspmConfig:END}
  gPlatformFspPkgTokenSpaceGuid.ReservedFspmUpd             | * | 0x03 | {0x00}


  # Note please keep "UpdTerminator" at the end of each UPD region.
  # The tool will use this field to determine the actual end of the UPD data
  # structure.

  gPlatformFspPkgTokenSpaceGuid.UpdTerminator               | * | 0x02 | 0x55AA
