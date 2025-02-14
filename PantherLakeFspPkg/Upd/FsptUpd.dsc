## @file
#  Platform description.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2017 Intel Corporation.
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

  #
  # Comments with !BSF will be used to generate BSF file
  # Comments with !HDR will be used to generate H header file
  #

  # Global definitions in BSF
  # !BSF PAGES:{MRC:"Memory Reference Code", OC:"Overclocking", DEBUG:"Debug", SI1:"Silicon(PreMem), SECURITY1:"Security(PreMem)", ME1:"Management Engine(PreMem)", CPU1:"CPU(PreMem)", SA1:"System Agent(PreMem)", PCH1:"PCH(PreMem)", SI2:"Silicon(PostMem), SECURITY2:"Security(PostMem)", ME2:"Management Engine(PostMem)", CPU2:"CPU(PostMem)", SA2:"System Agent(PostMem)", PCH2:"PCH(PostMem)"}
  # !BSF BLOCK:{NAME:"Panther Lake Platform", VER:"0.1"}

  # !BSF FIND:{PTLUPD_T}
  # !HDR COMMENT:{FSP_UPD_HEADER:FSP UPD Header}
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:START}
  # FsptUpdSignature: {PTLUPD_T}
  gPlatformFspPkgTokenSpaceGuid.Signature                   | * | 0x08 | 0x545F4450554C5450
  # !BSF NAME:{FsptUpdRevision}  TYPE:{None}
  gPlatformFspPkgTokenSpaceGuid.Revision                    | * | 0x01 | 0x02
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:END}
  gPlatformFspPkgTokenSpaceGuid.Reserved                    | * | 0x17 | {0x00}

  # !HDR COMMENT:{FSPT_ARCH2_UPD:FSPT_ARCH_UPD}
  # !HDR EMBED:{FSPT_ARCH2_UPD:FsptArchUpd:START}
  gPlatformFspPkgTokenSpaceGuid.Revision                    | * | 0x01 | 0x03
  gPlatformFspPkgTokenSpaceGuid.Reserved                    | * | 0x03 | {0x00}
  gPlatformFspPkgTokenSpaceGuid.Length                      | * | 0x04 | 0x00000020
  gPlatformFspPkgTokenSpaceGuid.FspDebugHandler             | * | 0x08 | 0x00000000
  gPlatformFspPkgTokenSpaceGuid.FspTemporaryRamSize         | * | 0x04 | 0x00100000
  # !HDR EMBED:{FSPT_ARCH2_UPD:FsptArchUpd:END}
  gPlatformFspPkgTokenSpaceGuid.Reserved1                   | * | 0x0C | {0x00}

  # !HDR COMMENT:{FSPT_CORE_UPD:Fsp T Core UPD}
  # !HDR EMBED:{FSPT_CORE_UPD:FsptCoreUpd:START}
  # Base address of the microcode region.
  # !HDR STRUCT:{EFI_PHYSICAL_ADDRESS}
  # !BSF HELP:{For example, 0xFFAD1000 is for Intel RVP BIOS implementation.}
  gPlatformFspPkgTokenSpaceGuid.MicrocodeRegionBase         | * | 0x08 | 0x00000000

  # Length of the microcode region.
  # !BSF HELP:{For example, 0x0009F000 is for Intel RVP BIOS implementation.}
  gPlatformFspPkgTokenSpaceGuid.MicrocodeRegionSize         | * | 0x08 | 0x00000000

  # Base address of the cacheable flash region.
  # !HDR STRUCT:{EFI_PHYSICAL_ADDRESS}
  gPlatformFspPkgTokenSpaceGuid.CodeRegionBase              | * | 0x08 | 0x00000000

  # !HDR EMBED:{FSPT_CORE_UPD:FsptCoreUpd:END}
  # Length of the cacheable flash region.
  # !BSF HELP:{For example, 0x00E00000 is for Intel RVP BIOS implementation.}
  gPlatformFspPkgTokenSpaceGuid.CodeRegionSize              | * | 0x08 | 0x00000000

  # !HDR COMMENT:{FSP_T_CONFIG:Fsp T Configuration}
  # !HDR EMBED:{FSP_T_CONFIG:FsptConfig:START}

  # !BSF NAME:{PcdLpssUartDebugEnable} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Enable and Initialize, 2:Enable without Initializing}
  # !BSF HELP:{Enable SerialIo Uart debug library with/without initializing SerialIo Uart device in FSP. }
!if $(TARGET) == DEBUG
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUartDebugEnable           | * | 0x01 | 0x01
!else
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUartDebugEnable           | * | 0x01 | 0x00
!endif

  # !BSF NAME:{PcdLpssUartNumber} TYPE:{Combo}
  # !BSF OPTION:{0:SerialIoUart0, 1:SerialIoUart1, 2:SerialIoUart2}
  # !BSF HELP:{Select SerialIo Uart Controller for debug. Note: If UART0 is selected as CNVi BT Core interface, it cannot be used for debug purpose.}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUartNumber                | * | 0x01 | 0

  # !BSF NAME:{PcdLpssUartMode - FSPT} TYPE:{Combo}
  # !BSF OPTION:{0:SerialIoUartDisabled, 1:SerialIoUartPci, 2:SerialIoUartHidden, 3:SerialIoUartCom, 4:SerialIoUartSkipInit}
  # !BSF HELP:{Select SerialIo Uart Controller mode}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUartMode                  | * | 0x01 | 0x02

  # !BSF NAME:{PcdLpssUartPowerGating - FSPT} TYPE:{Combo}
  # !BSF OPTION:{0:Disabled, 1:Enabled, 2:Auto}
  # !BSF HELP:{Select SerialIo Uart Controller Powergating mode}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUartPowerGating           | * | 0x01 | 0x02

  # !BSF NAME:{PcdLpssUartBaudRate - FSPT} TYPE:{EditNum, DEC, (0,6000000)}
  # !BSF HELP:{Set default BaudRate Supported from 0 - default to 6000000}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUartBaudRate              | * | 0x4 | 115200

  #
  # NOTE: If offset for PcdPciExpressBaseAddress & PcdPciExpressRegion is changed,
  # then please change offset accordingly in SecHostBridgeInit in SecHostBridgeLib.nasm
  #

  # !BSF NAME:{Pci Express Base Address}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Base address to be programmed for Pci Express }
  gPlatformFspPkgTokenSpaceGuid.PcdPciExpressBaseAddress             | * | 0x08 | 0xE0000000

  # !BSF NAME:{Pci Express Region Length}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Region Length to be programmed for Pci Express }
  gPlatformFspPkgTokenSpaceGuid.PcdPciExpressRegionLength            | * | 0x04 | 0x10000000

  # !BSF NAME:{PcdLpssUartParity - FSPT} TYPE:{Combo}
  # !BSF OPTION:{0: DefaultParity, 1: NoParity, 2: EvenParity, 3: OddParity}
  # !BSF HELP:{Set default Parity.}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUartParity                | * | 0x01 | 0x1

  # !BSF NAME:{PcdLpssUartDataBits - FSPT} TYPE:{EditNum, HEX, (0x0,0x08)}
  # !BSF HELP:{Set default word length. 0: Default, 5,6,7,8}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUartDataBits              | * | 0x01 | 0x08

  # !BSF NAME:{PcdLpssUartStopBits - FSPT} TYPE:{Combo}
  # !BSF OPTION:{0: DefaultStopBits, 1: OneStopBit, 2: OneFiveStopBits, 3: TwoStopBits}
  # !BSF HELP:{Set default stop bits.}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUartStopBits              | * | 0x01 | 0x01

  # !BSF NAME:{PcdLpssUartAutoFlow - FSPT} TYPE:{Combo}
  # !BSF OPTION:{0: Disable, 1:Enable}
  # !BSF HELP:{Enables UART hardware flow control, CTS and RTS lines.}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUartAutoFlow              | * | 0x01 | 0x0

  # !BSF NAME:{PcdLpssUartRxPinMux - FSPT} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Select RX pin muxing for SerialIo UART used for debug}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUartRxPinMux              | * | 0x04 | 0x0

  # !BSF NAME:{PcdLpssUartTxPinMux - FSPT} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Select TX pin muxing for SerialIo UART used for debug}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUartTxPinMux              | * | 0x04 | 0x0

  # !BSF NAME:{PcdLpssUartRtsPinMux - FSPT} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select SerialIo Uart used for debug Rts pin muxing. Refer to GPIO_*_MUXING_SERIALIO_UARTx_RTS* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUartRtsPinMux             | * | 0x04 | 0x0

  # !BSF NAME:{PcdLpssUartCtsPinMux - FSPT} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select SerialIo Uart used for debug Cts pin muxing. Refer to GPIO_*_MUXING_SERIALIO_UARTx_CTS* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUartCtsPinMux             | * | 0x04 | 0x0

  # !BSF NAME:{PcdLpssUartDebugMmioBase - FSPT} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select SerialIo Uart default MMIO resource in SEC/PEI phase when PcdLpssUartMode = SerialIoUartPci.}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUartDebugMmioBase         | * | 0x04 | 0xFE02C000

  # !BSF NAME:{PcdLpssUartDebugPciCfgBase - FSPT} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Specify PciCfgBase address. Allows for SerialIO UART functionality outside Bus 0}
  gSiPkgTokenSpaceGuid.PcdLpssUartDebugPciCfgBase                | * | 0x04| 0x0

  # !BSF NAME:{PcdLpcUartDebugEnable} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Enable}
  # !BSF HELP:{Enable to initialize LPC Uart device in FSP.}
!if $(TARGET) == DEBUG
  gPlatformFspPkgTokenSpaceGuid.PcdLpcUartDebugEnable                | * | 0x01 | 0x01
!else
  gPlatformFspPkgTokenSpaceGuid.PcdLpcUartDebugEnable                | * | 0x01 | 0x00
!endif

  # !BSF NAME:{Debug Interfaces} TYPE:{EditNum, HEX, (0x00,0x3F)}
  # !BSF HELP:{Debug Interfaces. BIT0-RAM, BIT1-UART, BIT3-USB3, BIT4-Serial IO, BIT5-TraceHub, BIT2 - Not used.}
  gPlatformFspPkgTokenSpaceGuid.PcdDebugInterfaceFlags          | * | 0x01 | 0x012

  # !BSF NAME:{PcdSerialDebugLevel} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Error Only, 2:Error and Warnings, 3:Load Error Warnings and Info, 4:Load Error Warnings and Info, 5:Load Error Warnings Info and Verbose}
  # !BSF HELP:{Serial Debug Message Level. 0:Disable, 1:Error Only, 2:Error & Warnings, 3:Load, Error, Warnings & Info, 4:Load, Error, Warnings, Info & Event, 5:Load, Error, Warnings, Info & Verbose.}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialDebugLevel             | * | 0x01 | 0x03

  # !BSF NAME:{ISA Serial Base selection} TYPE:{Combo}
  # !BSF OPTION:{0:0x3F8, 1:0x2F8}
  # !BSF HELP:{Select ISA Serial Base address. Default is 0x3F8.}
  gPlatformFspPkgTokenSpaceGuid.PcdIsaSerialUartBase             | * | 0x01 | 0x00

  # !BSF NAME:{PcdLpssUart2ndEnable} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Enable and Initialize, 2:Enable without Initializing}
  # !BSF HELP:{Enable Additional SerialIo Uart device in FSP.}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUart2ndEnable                | * | 0x01 | 0x00

  # !BSF NAME:{PcdLpssUart2ndNumber} TYPE:{Combo}
  # !BSF OPTION:{0:SerialIoUart0, 1:SerialIoUart1, 2:SerialIoUart2}
  # !BSF HELP:{Select SerialIo Uart Controller Number}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUart2ndNumber                | * | 0x01 | 0x02

  # !BSF NAME:{PcdLpssUart2ndMode - FSPT} TYPE:{Combo}
  # !BSF OPTION:{0:SerialIoUartDisabled, 1:SerialIoUartPci, 2:SerialIoUartHidden, 3:SerialIoUartCom, 4:SerialIoUartSkipInit}
  # !BSF HELP:{Select SerialIo Uart Controller mode}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUart2ndMode                  | * | 0x01 | 0x02

  gPlatformFspPkgTokenSpaceGuid.FsptUpdRsvd196                 | * | 0x01 | 0x00

  # !BSF NAME:{PcdLpssUart2ndBaudRate - FSPT} TYPE:{EditNum, DEC, (0,6000000)}
  # !BSF HELP:{Set default BaudRate Supported from 0 - default to 6000000}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUart2ndBaudRate              | * | 0x4 | 115200

  # !BSF NAME:{PcdLpssUart2ndParity - FSPT} TYPE:{Combo}
  # !BSF OPTION:{0: DefaultParity, 1: NoParity, 2: EvenParity, 3: OddParity}
  # !BSF HELP:{Set default Parity.}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUart2ndParity                | * | 0x01 | 0x1

  # !BSF NAME:{PcdLpssUart2ndDataBits - FSPT} TYPE:{EditNum, HEX, (0x0,0x08)}
  # !BSF HELP:{Set default word length. 0: Default, 5,6,7,8}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUart2ndDataBits              | * | 0x01 | 0x08

  # !BSF NAME:{PcdLpssUart2ndStopBits - FSPT} TYPE:{Combo}
  # !BSF OPTION:{0: DefaultStopBits, 1: OneStopBit, 2: OneFiveStopBits, 3: TwoStopBits}
  # !BSF HELP:{Set default stop bits.}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUart2ndStopBits              | * | 0x01 | 0x01

  # !BSF NAME:{PcdLpssUart2ndAutoFlow - FSPT} TYPE:{Combo}
  # !BSF OPTION:{0: Disable, 1:Enable}
  # !BSF HELP:{Enables UART hardware flow control, CTS and RTS lines.}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUart2ndAutoFlow              | * | 0x01 | 0x0

  # !BSF NAME:{PcdLpssUart2ndRxPinMux - FSPT} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Select RX pin muxing for SerialIo UART}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUart2ndRxPinMux              | * | 0x04 | 0x0

  # !BSF NAME:{PcdLpssUart2ndTxPinMux - FSPT} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Select TX pin muxing for SerialIo UART}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUart2ndTxPinMux              | * | 0x04 | 0x0

  # !BSF NAME:{PcdLpssUart2ndRtsPinMux - FSPT} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select SerialIo Uart Rts pin muxing. Refer to GPIO_*_MUXING_SERIALIO_UARTx_RTS* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUart2ndRtsPinMux             | * | 0x04 | 0x0

  # !BSF NAME:{PcdLpssUart2ndCtsPinMux - FSPT} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select SerialIo Uart Cts pin muxing. Refer to GPIO_*_MUXING_SERIALIO_UARTx_CTS* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUart2ndCtsPinMux             | * | 0x04 | 0x0

  # !BSF NAME:{PcdLpssUart2ndMmioBase - FSPT} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select SerialIo Uart default MMIO resource in SEC/PEI phase when PcdLpssUart2ndMode = SerialIoUartPci.}
  gPlatformFspPkgTokenSpaceGuid.PcdLpssUart2ndMmioBase              | * | 0x04 | 0xFE030000

  # !BSF NAME:{PcdLpssUart2ndPciCfgBase - FSPT} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Specify PciCfgBase address. Allows for SerialIO UART functionality outside Bus 0}
  gSiPkgTokenSpaceGuid.PcdLpssUart2ndPciCfgBase                     | * | 0x04| 0x0

  #
  # NOTE: If offset for TopMemoryCacheSize is changed,
  # then please change FSPT_CFG_TOP_MEMORY_CACHE_SIZE_OFFSET in SecCpuLib.nasm
  #
  # Length of the TopMemory cacheable flash region.
  # This should only be used where the cache size is not able
  # to cover all pre-memory code with one continuous region.
  # NOTE: To simply the algorithm, the value must be power of 2.
  # When value is zero, the feature is disabled.
  # If the value is not zero, FSPT will configure CAR to cover
  # the region between 4G-TopMemoryCacheSize to 4G for the first region,
  # and the CodeRegionBase and CodeRegionSize is used to cover
  # the second region.
  gPlatformFspPkgTokenSpaceGuid.TopMemoryCacheSize                      | * | 0x04 | 0x00000000

  # !BSF NAME:{Serial Io SPI Chip Select Polarity} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Sets polarity for each chip Select. Available options: 0:SerialIoSpiCsActiveLow, 1:SerialIoSpiCsActiveHigh}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoSpiCsPolarity            | * | 0x02 | { 0x0, 0x0}

  # !BSF NAME:{Serial Io SPI Chip Select Enable} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{0:Disabled, 1:Enabled. Enables GPIO for CS0 or CS1 if it is Enabled}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoSpiCsEnable              | * | 0x02 | { 0x0, 0x0}

  # !BSF NAME:{Serial Io SPI Device Mode} TYPE:{EditNum, HEX, (0x0,0x1}
  # !BSF HELP:{When mode is set to Pci, controller is initalized in early stage. Available modes: 0:SerialIoSpiDisabled, 1:SerialIoSpiPci.}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoSpiMode                  | * | 0x01 | 0x0

  # !BSF NAME:{Serial Io SPI Default Chip Select Output} TYPE:{EditNum, HEX, (0x00,0x1)}
  # !BSF HELP:{Sets Default CS as Output. Available options: 0:CS0, 1:CS1}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoSpiDefaultCsOutput       | * | 0x01 | 0x0

  # !BSF NAME:{Serial Io SPI Default Chip Select Mode HW/SW} TYPE:{EditNum, HEX, (0x00,0x1)}
  # !BSF HELP:{Sets Default CS Mode Hardware or Software. Available options: 0:HW, 1:SW}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoSpiCsMode                | * | 0x01 | 0x0

  # !BSF NAME:{Serial Io SPI Default Chip Select State Low/High} TYPE:{EditNum, HEX, (0x00,0x1)}
  # !BSF HELP:{Sets Default CS State Low or High. Available options: 0:Low, 1:High}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoSpiCsState               | * | 0x1 | 0x0

  # !BSF NAME:{Serial Io SPI Device Number} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{Select which Serial Io SPI controller is initalized in early stage.}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoSpiNumber                | * | 0x01 | 0x0
  # Added reserved space  UnusedUpdSpace0[3]
  gPlatformFspPkgTokenSpaceGuid.FsptUpdRsvd0                        | * | 0x3 | {0x00}

  # !BSF NAME:{Serial Io SPI Device MMIO Base} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Assigns MMIO for Serial Io SPI controller usage in early stage.}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoSpiMmioBase              | * | 0x04 | 0x0

  # !BSF NAME:{Serial IO SPI CS Pin Muxing} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select SerialIo SPI CS pin muxing. Refer to GPIO_*_MUXING_SERIALIO_SPIx_CS* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoSpiCsPinMux     | * | 0x08 |  { 0, 0 }

  # !BSF NAME:{Serial IO SPI CLK Pin Muxing} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select SerialIo SPI CLK pin muxing. Refer to GPIO_*_MUXING_SERIALIO_SPIx_CLK* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoSpiClkPinMux     | * | 0x04 | 0x00

  # !BSF NAME:{Serial IO SPI MISO Pin Muxing} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select SerialIo SPI MISO pin muxing. Refer to GPIO_*_MUXING_SERIALIO_SPIx_MISO* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoSpiMisoPinMux     | * | 0x04 | 0x00

  # !BSF NAME:{Serial IO SPI MOSI Pin Muxing} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select SerialIo SPI MOSI pin muxing. Refer to GPIO_*_MUXING_SERIALIO_SPIx_MOSI* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoSpiMosiPinMux     | * | 0x04 | 0x00

  # !BSF NAME:{Serial Io I2C Device MMIO Base} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Assigns MMIO for Serial Io I2C controller usage in early stage.}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoI2cMmioBase              | * | 0x04 | 0x0

  # !BSF NAME:{Serial Io I2C Sda Gpio Pin} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select SerialIo I2C Rts pin. Refer to GPIO_*_MUXING_SERIALIO_I2C*_SDA* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoI2cSdaPin       | * | 0x04 | 0x0

  # !BSF NAME:{Serial Io I2C Scl Gpio Pin} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select SerialIo I2C Cts pin. Refer to GPIO_*_MUXING_SERIALIO_I2C*_SCL* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoI2cSclPin       | * | 0x04 | 0x0

  # !BSF NAME:{Serial Io I2C Gpio Pad termination} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo I2C0,I2C1,... pads termination respectively. One byte for each controller, byte0 for I2C0, byte1 for I2C1, and so on.}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoI2cPadsTerm | * | 0x01 | 0x00

  # !BSF NAME:{Serial Io I2c Controller Number} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF OPTION:{0:SerialIoI2c0, 1:SerialIoI2c1, 2:SerialIoI2c2, 0xFF:Disable}
  # !BSF HELP:{Select SerialIo I2C Controller number to be intilizaed during early boot. Default is 0xFF}
  gPlatformFspPkgTokenSpaceGuid.PcdSerialIoI2cNumber         | * | 0x01 | 0xFF

  # !BSF NAME:{Enable Secondary Data Cache Region} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Enable}
  # !BSF HELP:{Enable Secondary Data Cache Region}
  gPlatformFspPkgTokenSpaceGuid.EnableSecondaryDataCache       | * | 0x01 | 0x0

  # !BSF NAME:{Enable Signed FSP Code Cache Programming} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Enable}
  # !BSF HELP:{Enable Signed FSP Code Cache Programming}
!if gSiPkgTokenSpaceGuid.PcdSignedFspEnable == TRUE
  gPlatformFspPkgTokenSpaceGuid.ProgramWriteBackCodeCache       | * | 0x01 | 0x01
!else
  gPlatformFspPkgTokenSpaceGuid.ProgramWriteBackCodeCache       | * | 0x01 | 0x00
!endif

  # !HDR EMBED:{FSP_T_CONFIG:FsptConfig:END}
  gPlatformFspPkgTokenSpaceGuid.ReservedFsptUpd1                | * | 0x08 | {0x00}
  # Added reserved space  UnusedUpdSpace2[6]
  gPlatformFspPkgTokenSpaceGuid.FsptUpdRsvd2                    | * | 0x6 | {0x00}

  # Note please keep "UpdTerminator" at the end of each UPD region.
  # The tool will use this field to determine the actual end of the UPD data
  # structure.

  gPlatformFspPkgTokenSpaceGuid.UpdTerminator               | * | 0x02 | 0x55AA
