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

  # !HDR COMMENT:{FSP_M_CONFIG:Fsp M Configuration}
  # !HDR EMBED:{FSP_M_CONFIG:FspmConfig:START}

  ## SERIAL_IO_UART_CONFIG Start

  # !BSF NAME:{Serial Io Uart Debug Mode} TYPE:{Combo}
  # !BSF OPTION:{0:SerialIoUartDisabled, 1:SerialIoUartPci, 2:SerialIoUartHidden, 3:SerialIoUartCom, 4:SerialIoUartSkipInit}
  # !BSF HELP:{Select SerialIo Uart Controller mode}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugMode               | * | 0x01 | 0x02

  # !BSF NAME:{Serial Io Uart Debug Auto Flow} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables UART hardware flow control, CTS and RTS lines.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugAutoFlow           | * | 0x01 | 0x0
  # Added reserved space UnusedUpdSpace0[2]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd0                        | * | 0x02 | {0x00}

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

  # !BSF NAME:{SerialIo Uart PowerGating} TYPE:{Combo}
  # !BSF OPTION:{0:Disabled, 1:Enabled, 2:Auto}
  # !BSF HELP:{Select SerialIo Uart Powergating mode}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartPowerGating             | * | 0x01 | 0x02

  ## SERIAL_IO_UART_CONFIG End

  #
  # Pch Pre-Mem Block End
  #

  ## PCH_DCI_PREMEM_CONFIG Start

  # !BSF NAME:{DCI Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Determine if to enable DCI debug from host}
  gPlatformFspPkgTokenSpaceGuid.DciEn                        | * | 0x01 | 0x00

  # !BSF NAME:{DCI DbC Mode} TYPE:{Combo} OPTION:{0:Disabled, 1:USB2 DbC, 2:USB3 DbC, 3:Both, 4:No Change}
  # !BSF HELP:{Disabled: Clear both USB2/3DBCEN; USB2: set USB2DBCEN; USB3: set USB3DBCEN; Both: Set both USB2/3DBCEN; No Change: Comply with HW value}
  gPlatformFspPkgTokenSpaceGuid.DciDbcMode                   | * | 0x01 | 0x04

  # !BSF NAME:{DCI Clock Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable DCI clock in lowest power state}
  gPlatformFspPkgTokenSpaceGuid.DciClkEnable                 | * | 0x01 | 0x01

  # !BSF NAME:{Keep Early Trace} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Trace is activated by default. When enable, keep early trace data and keep tracing, may block s0ix.\nWhen disabled will abandon trace data and stop tracing which allows enter s0ix\n\nnoted:enable this option will not enable TraceHub; When probe is connected, keep early trace will then be configured by tool, this option will not take effect.}
  gPlatformFspPkgTokenSpaceGuid.KeepEarlyTrace               | * | 0x01 | 0x00

  ## PCH_DCI_PREMEM_CONFIG End

  #
  # MRC Block Start
  #
  # !BSF PAGE:{MRC}

  # MEMORY Subsystem Start MemoryOverlap check Enable/Disable

  # !BSF NAME:{Enable/Disable MemoryOverlap check} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable(Default): Enable MemoryOverlap check, Disable: Disable MemoryOverlap check}
  gPlatformFspPkgTokenSpaceGuid.MemMapOverlapCheckSupport   | * | 0x01 | 0x0

  # MEMORY Subsystem Start

  ## MEMORY_CONFIG_NO_CRC Start

  # !BSF NAME:{Memory Test on Warm Boot}
  # !BSF TYPE:{Combo} OPTION:{0:Disable, 1:Enable}
  # !BSF HELP:{Run Base Memory Test on Warm Boot}
  gPlatformFspPkgTokenSpaceGuid.MemTestOnWarmBoot              | * | 0x01 | 0x01
  # Added reserved space UnusedUpdSpace1[5]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd1                | * | 0x05 | {0x00}

  # !BSF NAME:{Platform Reserved Memory Size} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{The minimum platform memory size required to pass control into DXE}
  gPlatformFspPkgTokenSpaceGuid.PlatformMemorySize          | * | 0x08 | 0x15000000

  # !BSF NAME:{SPD Data Length} TYPE:{Combo}
  # !BSF OPTION:{0x100:256 Bytes, 0x200:512 Bytes, 0x400:1024 Bytes}
  # !BSF HELP:{Length of SPD Data}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdDataLen            | * | 0x02 | 0x200
  # Added reserved space UnusedUpdSpace2[6]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd2                | * | 0x06 | {0x00}

  # !BSF NAME:{Memory SPD Pointer Controller 0 Channel 0 Dimm 0}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr000              | * | 0x08 | 0x00000000

  # !BSF NAME:{Memory SPD Pointer Controller 0 Channel 0 Dimm 1}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr001              | * | 0x08 | 0x00000000

  # !BSF NAME:{Memory SPD Pointer Controller 0 Channel 1 Dimm 0}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr010              | * | 0x08 | 0x00000000

  # !BSF NAME:{Memory SPD Pointer Controller 0 Channel 1 Dimm 1}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr011              | * | 0x08 | 0x00000000

  # !BSF NAME:{Memory SPD Pointer Controller 0 Channel 2 Dimm 0}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr020              | * | 0x08 | 0x00000000

  # !BSF NAME:{Memory SPD Pointer Controller 0 Channel 2 Dimm 1}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr021              | * | 0x08 | 0x00000000

  # !BSF NAME:{Memory SPD Pointer Controller 0 Channel 3 Dimm 0}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr030              | * | 0x08 | 0x00000000

  # !BSF NAME:{Memory SPD Pointer Controller 0 Channel 3 Dimm 1}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr031              | * | 0x08 | 0x00000000

  # !BSF NAME:{Memory SPD Pointer Controller 1 Channel 0 Dimm 0}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr100              | * | 0x08 | 0x00000000

  # !BSF NAME:{Memory SPD Pointer Controller 1 Channel 0 Dimm 1}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr101              | * | 0x08 | 0x00000000

  # !BSF NAME:{Memory SPD Pointer Controller 1 Channel 1 Dimm 0}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr110              | * | 0x08 | 0x00000000

  # !BSF NAME:{Memory SPD Pointer Controller 1 Channel 1 Dimm 1}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr111              | * | 0x08 | 0x00000000

  # !BSF NAME:{Memory SPD Pointer Controller 1 Channel 2 Dimm 0}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr120              | * | 0x08 | 0x00000000

  # !BSF NAME:{Memory SPD Pointer Controller 1 Channel 2 Dimm 1}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr121              | * | 0x08 | 0x00000000

  # !BSF NAME:{Memory SPD Pointer Controller 1 Channel 3 Dimm 0}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr130              | * | 0x08 | 0x00000000

  # !BSF NAME:{Memory SPD Pointer Controller 1 Channel 3 Dimm 1}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to SPD data, will be used only when SpdAddressTable SPD Address are marked as 00}
  gPlatformFspPkgTokenSpaceGuid.MemorySpdPtr131              | * | 0x08 | 0x00000000

  # !BSF NAME:{RcompResistor settings}  TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{Indicates  RcompResistor settings: Board-dependent}
  gPlatformFspPkgTokenSpaceGuid.RcompResistor                | * | 0x02 | 0

  # !BSF NAME:{RcompTarget settings} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{RcompTarget settings: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.RcompTarget                  | * | 0x0A | { 0, 0, 0, 0, 0 }

  # !BSF NAME:{Dqs Map CPU to DRAM MC 0 CH 0} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dqs mapping relationship between CPU and DRAM, Channel 0: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqsMapCpu2DramMc0Ch0           | * | 0x02 | { 0, 1 }

  # !BSF NAME:{Dqs Map CPU to DRAM MC 0 CH 1} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dqs mapping relationship between CPU and DRAM, Channel 1: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqsMapCpu2DramMc0Ch1           | * | 0x02 | { 0, 1 }

  # !BSF NAME:{Dqs Map CPU to DRAM MC 0 CH 2} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dqs mapping relationship between CPU and DRAM, Channel 2: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqsMapCpu2DramMc0Ch2           | * | 0x02 | { 0, 1 }

  # !BSF NAME:{Dqs Map CPU to DRAM MC 0 CH 3} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dqs mapping relationship between CPU and DRAM, Channel 3: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqsMapCpu2DramMc0Ch3           | * | 0x02 | { 0, 1 }

  # !BSF NAME:{Dqs Map CPU to DRAM MC 1 CH 0} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dqs mapping relationship between CPU and DRAM, Channel 0: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqsMapCpu2DramMc1Ch0           | * | 0x02 | { 0, 1 }

  # !BSF NAME:{Dqs Map CPU to DRAM MC 1 CH 1} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dqs mapping relationship between CPU and DRAM, Channel 1: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqsMapCpu2DramMc1Ch1           | * | 0x02 | { 0, 1 }

  # !BSF NAME:{Dqs Map CPU to DRAM MC 1 CH 2} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dqs mapping relationship between CPU and DRAM, Channel 2: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqsMapCpu2DramMc1Ch2           | * | 0x02 | { 0, 1 }

  # !BSF NAME:{Dqs Map CPU to DRAM MC 1 CH 3} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dqs mapping relationship between CPU and DRAM, Channel 3: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqsMapCpu2DramMc1Ch3           | * | 0x02 | { 0, 1 }

  # !BSF NAME:{Dq Map CPU to DRAM MC 0 CH 0} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dq mapping relationship between CPU and DRAM, Channel 0: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqMapCpu2DramMc0Ch0            | * | 0x10 | { 0, 1, 2, 3, 4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 15 }

  # !BSF NAME:{Dq Map CPU to DRAM MC 0 CH 1} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dq mapping relationship between CPU and DRAM, Channel 1: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqMapCpu2DramMc0Ch1            | * | 0x10 | { 0, 1, 2, 3, 4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 15 }

  # !BSF NAME:{Dq Map CPU to DRAM MC 0 CH 2} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dq mapping relationship between CPU and DRAM, Channel 2: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqMapCpu2DramMc0Ch2            | * | 0x10 | { 0, 1, 2, 3, 4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 15 }

  # !BSF NAME:{Dq Map CPU to DRAM MC 0 CH 3} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dq mapping relationship between CPU and DRAM, Channel 3: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqMapCpu2DramMc0Ch3            | * | 0x10 | { 0, 1, 2, 3, 4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 15 }

  # !BSF NAME:{Dq Map CPU to DRAM MC 1 CH 0} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dq mapping relationship between CPU and DRAM, Channel 0: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqMapCpu2DramMc1Ch0            | * | 0x10 | { 0, 1, 2, 3, 4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 15 }

  # !BSF NAME:{Dq Map CPU to DRAM MC 1 CH 1} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dq mapping relationship between CPU and DRAM, Channel 1: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqMapCpu2DramMc1Ch1            | * | 0x10 | { 0, 1, 2, 3, 4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 15 }

  # !BSF NAME:{Dq Map CPU to DRAM MC 1 CH 2} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dq mapping relationship between CPU and DRAM, Channel 2: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqMapCpu2DramMc1Ch2            | * | 0x10 | { 0, 1, 2, 3, 4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 15 }

  # !BSF NAME:{Dq Map CPU to DRAM MC 1 CH 3} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Set Dq mapping relationship between CPU and DRAM, Channel 3: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqMapCpu2DramMc1Ch3            | * | 0x10 | { 0, 1, 2, 3, 4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 15 }

  ## MEMORY_CONFIG_NO_CRC End

  ## MEMORY_CONFIGURATION Start

  # !BSF NAME:{LowerBasicMemTestSize}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Reduce BasicMemTest size. 0: Disabled (default), regular BasicMemTest. 1: Enabled, shorter BasicMemTest (faster boot)}
  gPlatformFspPkgTokenSpaceGuid.LowerBasicMemTestSize       | * | 0x01 | 0x00

  # !BSF NAME:{EccGranularity32BEn}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Reduce BasicMemTest size. 0: Disabled (default), regular BasicMemTest. 1: Enabled, shorter BasicMemTest (faster boot)}
  gPlatformFspPkgTokenSpaceGuid.EccGranularity32BEn         | * | 0x01 | 0x00

  # !BSF NAME:{EccCorrectionMode}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Reduce BasicMemTest size. 0: Disabled (default), regular BasicMemTest. 1: Enabled, shorter BasicMemTest (faster boot)}
  gPlatformFspPkgTokenSpaceGuid.EccCorrectionMode           | * | 0x01 | 0x00

  # !BSF NAME:{CaVrefHigh}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{DDR5 CA Sweep High Vref Value for DDR5 OC}
  gPlatformFspPkgTokenSpaceGuid.CaVrefHigh                  | * | 0x01 | 0x1D

  # !BSF NAME:{CaVrefLow}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{DDR5 CA Sweep Low Vref Value for DDR5 OC}
  gPlatformFspPkgTokenSpaceGuid.CaVrefLow                   | * | 0x01 | 0x45

  # !BSF NAME:{CsVrefHigh}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{DDR5 CS Sweep High Vref Value for DDR5 OC}
  gPlatformFspPkgTokenSpaceGuid.CsVrefHigh                  | * | 0x01 | 0x1D

  # !BSF NAME:{CsVrefLow}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{DDR5 CS Sweep Low Vref Value for DDR5 OC}
  gPlatformFspPkgTokenSpaceGuid.CsVrefLow                   | * | 0x01 | 0x45

  # !BSF NAME:{DIMM DFE Tap1 Step Size}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{DIMM DFE Tap1 Step Size for DDR5 OC}
  gPlatformFspPkgTokenSpaceGuid.DFETap1StepSize             | * | 0x01 | 0x00

  # !BSF NAME:{DIMM DFE Tap2 Step Size}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{DIMM DFE Tap2 Step Size for DDR5 OC}
  gPlatformFspPkgTokenSpaceGuid.DFETap2StepSize             | * | 0x01 | 0x00
  # Added reserved space UnusedUpdSpace3
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd3                | * | 0x01 | 0x00

  # !BSF NAME:{VDD2 override}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{VDD2 override for DDR5 OC; 0 - Auto}
  gPlatformFspPkgTokenSpaceGuid.Vdd2Mv                      | * | 0x02 | 0x00

  # !BSF NAME:{tRAS}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x40)}
  # !BSF HELP:{RAS Active Time, 0: AUTO, max: 65535. Only used if FspmUpd->FspmConfig.SpdProfileSelected == 1 (Custom Profile).}
  gPlatformFspPkgTokenSpaceGuid.tRAS                        | * | 0x02 | 0x0000

  # !BSF NAME:{tRCD/tRP}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x3F)}
  # !BSF HELP:{RAS to CAS delay time and Row Precharge delay time, 0: AUTO, max: 255. Only used if FspmUpd->FspmConfig.SpdProfileSelected == 1 (Custom Profile).}
  gPlatformFspPkgTokenSpaceGuid.tRCDtRP                     | * | 0x02 | 0x00

  # !BSF NAME:{tREFI}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Refresh Interval, 0: AUTO, max: 65535. Only used if FspmUpd->FspmConfig.SpdProfileSelected == 1 (Custom Profile).}
  gPlatformFspPkgTokenSpaceGuid.tREFI                       | * | 0x04 | 0x0000

  # !BSF NAME:{tCL}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x1F)}
  # !BSF HELP:{CAS Latency, 0: AUTO, max: 255. Only used if FspmUpd->FspmConfig.SpdProfileSelected == 1 (Custom Profile).}
  gPlatformFspPkgTokenSpaceGuid.tCL                         | * | 0x02 | 0x00

  # !BSF NAME:{tCWL}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x22)}
  # !BSF HELP:{Min CAS Write Latency Delay Time, 0: AUTO, max: 255. Only used if FspmUpd->FspmConfig.SpdProfileSelected == 1 (Custom Profile).}
  gPlatformFspPkgTokenSpaceGuid.tCWL                        | * | 0x02 | 0x00

  # !BSF NAME:{tFAW}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x3F)}
  # !BSF HELP:{Min Four Activate Window Delay Time, 0: AUTO, max: 65535. Only used if FspmUpd->FspmConfig.SpdProfileSelected == 1 (Custom Profile).}
  gPlatformFspPkgTokenSpaceGuid.tFAW                        | * | 0x02 | 0x0000

  # !BSF NAME:{tRFC}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x3FF)}
  # !BSF HELP:{Min Refresh Recovery Delay Time, 0: AUTO, max: 65535. Only used if FspmUpd->FspmConfig.SpdProfileSelected == 1 (Custom Profile).}
  gPlatformFspPkgTokenSpaceGuid.tRFC                        | * | 0x02 | 0x0000

  # !BSF NAME:{tRRD}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x0F)}
  # !BSF HELP:{Min Row Active to Row Active Delay Time, 0: AUTO, max: 255. Only used if FspmUpd->FspmConfig.SpdProfileSelected == 1 (Custom Profile).}
  gPlatformFspPkgTokenSpaceGuid.tRRD                        | * | 0x02 | 0x00

  # !BSF NAME:{tRTP}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x0F)}
  # !BSF HELP:{Min Internal Read to Precharge Command Delay Time, 0: AUTO, max: 255. Only used if FspmUpd->FspmConfig.SpdProfileSelected == 1 (Custom Profile).}
  gPlatformFspPkgTokenSpaceGuid.tRTP                        | * | 0x02 | 0x00

  # !BSF NAME:{tWR}
  # !BSF TYPE:{Combo} OPTION:{0:Auto, 5:5, 6:6, 7:7, 8:8, 10:10, 12:12, 14:14, 16:16, 18:18, 20:20, 24:24, 30:30, 34:34, 40:40}
  # !BSF HELP:{Min Write Recovery Time, 0: AUTO, legal values: 5, 6, 7, 8, 10, 12, 14, 16, 18, 20, 24, 30, 34, 40. Only used if FspmUpd->FspmConfig.SpdProfileSelected == 1 (Custom Profile).}
  gPlatformFspPkgTokenSpaceGuid.tWR                         | * | 0x02 | 0x00

  # !BSF NAME:{tWTR}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x1C)}
  # !BSF HELP:{Min Internal Write to Read Command Delay Time, 0: AUTO, max: 255. Only used if FspmUpd->FspmConfig.SpdProfileSelected == 1 (Custom Profile).}
  gPlatformFspPkgTokenSpaceGuid.tWTR                        | * | 0x02 | 0x00

  # !BSF NAME:{tWTR_S}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{tWTR_S value for OC Custom Profile, 0 - Auto}
  gPlatformFspPkgTokenSpaceGuid.tWTR_S                      | * | 0x02 | 0x00

  # !BSF NAME:{tWTR_L}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{tWTR_L value for OC Custom Profile, 0 - Auto}
  gPlatformFspPkgTokenSpaceGuid.tWTR_L                      | * | 0x02 | 0x00

  # !BSF NAME:{tCCD_L}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{tCCD_L value for OC Custom Profile, 0 - Auto}
  gPlatformFspPkgTokenSpaceGuid.tCCD_L                      | * | 0x02 | 0x00

  # !BSF NAME:{tRRD_S}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{tRRD_S value for OC Custom Profile, 0 - Auto}
  gPlatformFspPkgTokenSpaceGuid.tRRD_S                      | * | 0x02 | 0x00

  # !BSF NAME:{tRRD_L}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{tRRD_L value for OC Custom Profile, 0 - Auto}
  gPlatformFspPkgTokenSpaceGuid.tRRD_L                      | * | 0x02 | 0x00

  # !BSF NAME:{tRFC4}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{tRFC4 value for OC Custom Profile, 0 - Auto}
  gPlatformFspPkgTokenSpaceGuid.tRFC4                       | * | 0x02 | 0x00

  # !BSF NAME:{tRFC2}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{tRFC2 value for OC Custom Profile, 0 - Auto}
  gPlatformFspPkgTokenSpaceGuid.tRFC2                       | * | 0x02 | 0x00

  # !BSF NAME:{tRFCpb}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{tRFCpb value for OC Custom Profile, 0 - Auto}
  gPlatformFspPkgTokenSpaceGuid.tRFCpb                      | * | 0x02 | 0x00

  # !BSF NAME:{tCCD_L_WR}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Number of tCK cycles for the channel DIMM's minimum Write-to-Write delay for same bank groups, for OC Custom Profile, 0 - Auto}
  gPlatformFspPkgTokenSpaceGuid.tCCD_L_WR                   | * | 0x02 | 0x0

  # !BSF NAME:{Periodic COMP}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Periodic Compensation}
  gPlatformFspPkgTokenSpaceGuid.EnPeriodicComp              | * | 0x01 | 0x01

  # !BSF NAME:{LPMode4 Support}
  # !BSF TYPE:{Combo} OPTION:{0: Disable, 1:Enable, 2:Dynamic Threshold 2, 3:Dynamic Threshold 3}
  # !BSF HELP:{LPMode4 Options}
  gPlatformFspPkgTokenSpaceGuid.LpMode4                     | * | 0x01 | 0x01

  # !BSF NAME:{LPMode Support}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x7)}
  # !BSF HELP:{Bit[0]: Enable Lpmode0p5 (Idle_enable), Bit[1]: Enable Lpmode2 (Powerdown_enable), Bit[2]: Enable Lpmode3 (Selfrefresh_enable)}
  gPlatformFspPkgTokenSpaceGuid.LpMode                      | * | 0x01 | 0x07

  # !BSF NAME:{Opportunistic Read}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Opportunistic Read (Def= Enable)}
  gPlatformFspPkgTokenSpaceGuid.OpportunisticRead           | * | 0x01 | 0x01

  # !BSF NAME:{Cycle Bypass Support}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Cycle Bypass Support(Def=Disable)}
  gPlatformFspPkgTokenSpaceGuid.Disable2CycleBypass         | * | 0x01 | 0x00

  # !BSF NAME:{MRC OCSafeMode}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{OverClocking Safe Mode for tCL}
  gPlatformFspPkgTokenSpaceGuid.OCSafeMode                  | * | 0x01 | 0x01

  # !BSF NAME:{DQ Vref Ctrl Offset}
  # !BSF TYPE:{Combo} OPTION:{0xF4:-12,0xF5:-11, 0xF6:-10, 0xF7:-9, 0xF8:-8, 0xF9:-7, 0xFA:-6, 0xFB:-5, 0xFC:-4, 0xFD:-3, 0xFE:-2, 0xFF:-1, 0:0, 1:+1, 2:+2, 3:+3, 4:+4, 5:+5, 6:+6, 7:+7, 8:+8, 9:+9, 10:+10, 11:+11, 12:+12}
  # !BSF HELP:{Offset to be applied to DDRDATA7CH1_CR_DDRCRVREFADJUST1.Ch0VrefCtl}
  gPlatformFspPkgTokenSpaceGuid.VrefCtlOffset               | * | 0x01 | 0x00

  # !BSF NAME:{Dqs Pins Interleaved Setting} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Indicates DqPinsInterleaved setting: board-dependent}
  gPlatformFspPkgTokenSpaceGuid.DqPinsInterleaved           | * | 0x01 | 0x0

  # !BSF NAME:{Smram Mask} TYPE:{Combo}
  # !BSF OPTION:{0: Neither, 1:AB-SEG, 2:H-SEG, 3: Both}
  # !BSF HELP:{The SMM Regions AB-SEG and/or H-SEG reserved}
  gPlatformFspPkgTokenSpaceGuid.SmramMask                   | * | 0x01 | 0x0

  # !BSF NAME:{MRC Fast Boot} TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable the MRC fast path thru the MRC}
  gPlatformFspPkgTokenSpaceGuid.MrcFastBoot                 | * | 0x01 | 0x1

  # !BSF NAME:{Rank Margin Tool per Task}
  # !BSF TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{This option enables the user to execute Rank Margin Tool per major training step in the MRC.}
  gPlatformFspPkgTokenSpaceGuid.RmtPerTask                  | * | 0x01 | 0x0

  # !BSF NAME:{Training Trace}
  # !BSF TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{This option enables the trained state tracing feature in MRC.  This feature will print out the key training parameters state across major training steps.}
  gPlatformFspPkgTokenSpaceGuid.TrainTrace                  | * | 0x01 | 0x0

  # !BSF NAME:{Probeless Trace}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Probeless Trace: 0=Disabled, 1=Enable. Enabling Probeless Trace will reserve 128MB. This also requires IED to be enabled.}
  gPlatformFspPkgTokenSpaceGuid.ProbelessTrace              | * | 0x01 | 0x00
  # Added reserved space UnusedUpdSpace4
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd4                | * | 0x01 | 0x00

  # !BSF NAME:{DDR Frequency Limit}
  # !BSF TYPE:{Combo} OPTION:{1067:1067, 1333:1333, 1600:1600, 1867:1867, 2133:2133, 2400:2400, 2667:2667, 2933:2933, 0:Auto}
  # !BSF HELP:{Maximum Memory Frequency Selections in Mhz. Options are 1067, 1333, 1600, 1867, 2133, 2400, 2667, 2933 and 0 for Auto.}
  gPlatformFspPkgTokenSpaceGuid.DdrFreqLimit                | * | 0x02 | 0x0

  # !BSF NAME:{SAGV} TYPE:{Combo}
  # !BSF OPTION:{0:Disabled, 1:Enabled}
  # !BSF HELP:{System Agent dynamic frequency support.}
  gPlatformFspPkgTokenSpaceGuid.SaGv                        | * | 0x01 | 0x01

  # !BSF NAME:{SAGV WP Mask} TYPE:{Combo}
  # !BSF OPTION:{0x3:Points0_1, 0x7:Points0_1_2, 0xF:AllPoints0_1_2_3}
  # !BSF HELP:{System Agent dynamic frequency workpoints that memory will be training at the enabled frequencies.}
  gPlatformFspPkgTokenSpaceGuid.SaGvWpMask                  | * | 0x01 | 0x0F

  # !BSF NAME:{SAGV Gear Ratio} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF STRUCT:{UINT8}
  # !BSF HELP:{Gear Selection for SAGV points. 0 - Auto, 1-1 Gear 1, 2-Gear 2}
  gPlatformFspPkgTokenSpaceGuid.SaGvGear                    | * | 0x04 | {0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{SAGV Frequency} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{SAGV Frequency per point in Mhz. 0 for Auto and a ratio of 133/100MHz: 1333/1300.}
  gPlatformFspPkgTokenSpaceGuid.SaGvFreq                    | * | 0x08 | {0x0000, 0x0000, 0x0000, 0x0000}

  # !BSF NAME:{SAGV Disabled Gear Ratio} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF STRUCT:{UINT8}
  # !BSF HELP:{Gear Selection for SAGV Disabled. 0 - Auto, 1-1 Gear 1, 2-Gear 2}
  gPlatformFspPkgTokenSpaceGuid.GearRatio                   | * | 0x01 | 0x00

  # !BSF NAME:{Rank Margin Tool}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Rank Margin Tool.}
  gPlatformFspPkgTokenSpaceGuid.RMT                         | * | 0x01 | 0x00

  # !BSF NAME:{Controller 0 Channel 0 DIMM Control}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable / Disable DIMMs on Controller 0 Channel 0}
  gPlatformFspPkgTokenSpaceGuid.DisableMc0Ch0               | * | 0x01 | 0x00

  # !BSF NAME:{Controller 0 Channel 1 DIMM Control}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable / Disable DIMMs on Controller 0 Channel 1}
  gPlatformFspPkgTokenSpaceGuid.DisableMc0Ch1               | * | 0x01 | 0x00

  # !BSF NAME:{Controller 0 Channel 2 DIMM Control}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable / Disable DIMMs on Controller 0 Channel 2}
  gPlatformFspPkgTokenSpaceGuid.DisableMc0Ch2               | * | 0x01 | 0x00

  # !BSF NAME:{Controller 0 Channel 3 DIMM Control}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable / Disable DIMMs on Controller 0 Channel 3}
  gPlatformFspPkgTokenSpaceGuid.DisableMc0Ch3               | * | 0x01 | 0x00

  # !BSF NAME:{Controller 1 Channel 0 DIMM Control}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable / Disable DIMMs on Controller 1 Channel 0}
  gPlatformFspPkgTokenSpaceGuid.DisableMc1Ch0               | * | 0x01 | 0x00

  # !BSF NAME:{Controller 1 Channel 1 DIMM Control}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable / Disable DIMMs on Controller 1 Channel 1}
  gPlatformFspPkgTokenSpaceGuid.DisableMc1Ch1               | * | 0x01 | 0x00

  # !BSF NAME:{Controller 1 Channel 2 DIMM Control}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable / Disable DIMMs on Controller 1 Channel 2}
  gPlatformFspPkgTokenSpaceGuid.DisableMc1Ch2               | * | 0x01 | 0x00

  # !BSF NAME:{Controller 1 Channel 3 DIMM Control}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable / Disable DIMMs on Controller 1 Channel 3}
  gPlatformFspPkgTokenSpaceGuid.DisableMc1Ch3               | * | 0x01 | 0x00

  # !BSF NAME:{Scrambler Support}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{This option enables data scrambling in memory.}
  gPlatformFspPkgTokenSpaceGuid.ScramblerSupport            | * | 0x01 | 0x01
  # Added reserved space UnusedUpdSpace5
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd5             | * | 0x01 | 0x00

  # !BSF NAME:{Memory Voltage}
  # !BSF TYPE:{Combo} OPTION:{0:Default, 1200:1.20 Volts, 1250:1.25 Volts, 1300:1.30 Volts, 1350:1.35 Volts, 1400:1.40 Volts, 1450:1.45 Volts, 1500:1.50 Volts, 1550:1.55 Volts, 1600:1.60 Volts, 1650:1.65 Volts}
  # !BSF HELP:{DRAM voltage (Vdd) (supply voltage for input buffers and core logic of the DRAM chips) in millivolts. <b>0=Platform Default (no override)</b>, 1200=1.2V, 1350=1.35V etc.}
  gPlatformFspPkgTokenSpaceGuid.VddVoltage                  | * | 0x02 | 0x0000

  # !BSF NAME:{Memory Ratio}
  # !BSF TYPE:{Combo} OPTION:{0:Auto, 4:4, 5:5, 6:6, 7:7, 8:8, 9:9, 10:10, 11:11, 12:12, 13:13, 14:14, 15:15}
  # !BSF HELP:{Automatic or the frequency will equal ratio times reference clock. Set to Auto to recalculate memory timings listed below.}
  gPlatformFspPkgTokenSpaceGuid.Ratio                       | * | 0x02 | 0x00

  # !BSF NAME:{SPD Profile Selected}
  # !BSF TYPE:{Combo} OPTION:{0:Default Profile, 1:Custom Profile, 2:XMP Profile 1, 3:XMP Profile 2}
  # !BSF HELP:{Select DIMM timing profile. Options are 0=Default Profile, 1=Custom Profile, 2=XMP Profile 1, 3=XMP Profile 2}
  gPlatformFspPkgTokenSpaceGuid.SpdProfileSelected          | * | 0x01 | 0x00

  # !BSF NAME:{RxVref Per-Bit Training
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable RxVref Per-Bit Training}
  gPlatformFspPkgTokenSpaceGuid.RXVREFPERBIT                | * | 0x01 | 0x00

  # !BSF NAME:{Rx DQS Duty Cycle Correction}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Rx DQS Duty Cycle Correction}
  gPlatformFspPkgTokenSpaceGuid.RXDQSDCC                    | * | 0x01 | 0x01

  # !BSF NAME:{Ch Hash Override}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Select if Channel Hash setting values will be taken from input parameters or automatically taken from POR values depending on DRAM type detected. NOTE: ONLY if Memory interleaved Mode}
  gPlatformFspPkgTokenSpaceGuid.ChHashOverride              | * | 0x1 | 0x00

  # !BSF NAME:{Voltage Readout}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Voltage Readout for VCCClk and PBias}
  gPlatformFspPkgTokenSpaceGuid.VoltageReadout              | * | 0x01 | 0x00

  # !BSF NAME:{DQS Rise/Fall}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable DQS Rise/Fall}
  gPlatformFspPkgTokenSpaceGuid.DQSRF                       | * | 0x01 | 0x01

  # !BSF NAME:{DQS Rise/Fall}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable DQS Rise/Fall}
  gPlatformFspPkgTokenSpaceGuid.RDDQSODTT                   | * | 0x01 | 0x00

  # !BSF NAME:{PreTraining}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable PreTraining}
  gPlatformFspPkgTokenSpaceGuid.PRETRAIN                    | * | 0x01 | 0x01

  # !BSF NAME:{DUNIT Configuration}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Dunit Configuration}
  gPlatformFspPkgTokenSpaceGuid.DUNITC                      | * | 0x01 | 0x01

  # !BSF NAME:{Functional Duty Cycle Correction for DDR5 CLK}
  # !BSF TYPE:{Combo} OPTION:{0:Disable, 1:Enable}
  # !BSF HELP:{Enable/Disable Functional Duty Cycle Correction for DDR5 CLK}
  gPlatformFspPkgTokenSpaceGuid.FUNCDCCCLK                  | * | 0x01 | 0x00

  # !BSF NAME:{Functional Duty Cycle Correction for DDR5 DQS}
  # !BSF TYPE:{Combo} OPTION:{0:Disable, 1:Enable}
  # !BSF HELP:{Enable/Disable Functional Duty Cycle Correction for DDR5 DQS}
  gPlatformFspPkgTokenSpaceGuid.FUNCDCCDQS                  | * | 0x01 | 0x00

  # !BSF NAME:Functional Duty Cycle Correction for LP5 WCK}
  # !BSF TYPE:{Combo} OPTION:{0:Disable, 1:Enable}
  # !BSF HELP:{Enable/Disable Functional Duty Cycle Correction for LP5 WCK}
  gPlatformFspPkgTokenSpaceGuid.FUNCDCCWCK                  | * | 0x01 | 0x00

  # !BSF NAME:{Duty Cycle Correction for LP5 DCA}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Duty Cycle Correction for LP5 DCA}
  gPlatformFspPkgTokenSpaceGuid.DCCLP5WCKDCA                | * | 0x01 | 0x01

  # !BSF NAME:{DQ/DQS Swizzle Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable DQ/DQS Swizzle Training}
  gPlatformFspPkgTokenSpaceGuid.DQDQSSWZ                    | * | 0x01 | 0x01

  # !BSF NAME:{DQ/DQS Swizzle Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable DQ/DQS Swizzle Training}
  gPlatformFspPkgTokenSpaceGuid.DCCLP5READDCA               | * | 0x01 | 0x01

  # !BSF NAME:{Functional Duty Cycle Correction for Data DQ}
  # !BSF TYPE:{Combo} OPTION:{0:Disable, 1:Enable}
  # !BSF HELP:{Enable/Disable Functional Duty Cycle Correction for Data DQ}
  gPlatformFspPkgTokenSpaceGuid.FUNCDCCDQ                   | * | 0x01 | 0x00

  # !BSF NAME:{SubCh Hash Override}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Select if SubChannel Hash setting values will be taken from input parameters or automatically taken from POR values depending on DRAM type detected. NOTE: ONLY if Memory interleaved Mode}
  gPlatformFspPkgTokenSpaceGuid.SubChHashOverride           | * | 0x01 | 0x00

  # !BSF NAME:{DDR5 Auto Precharge Enable}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Auto Precharge Enable for DDR5: <b>O=Auto</b>, 1=Disable, 2=Enable}
  gPlatformFspPkgTokenSpaceGuid.Ddr5AutoPrechargeEnable     | * | 0x01 | 0x00

  # !BSF NAME:{Lp5 SplitACT Enable}
  # !BSF TYPE:{Combo} OPTION:{0:Auto, 1:Disable, 2:Enable}
  # !BSF HELP:{SplitACT enable for LP5}
  gPlatformFspPkgTokenSpaceGuid.Lp5SplitACTEnable           | * | 0x01 | 0x02

  # !BSF NAME:{CCC Half Frequency}
  # !BSF TYPE:{Combo} OPTION:{0: Auto, 1: Disable, 2: GroupGv0, 3: GroupGv1, 4: GroupGv2, 5: GroupGv3}
  # !BSF HELP:{CCC Half Frequency (CccGear4) Mode: 0 = Auto (Default), 1 = Disable, 2 = GroupGv0 (SaGv0 only), 3 = GroupGv1 (Up to SaGv1), 4 = GroupGv2 (Up to SaGv2), 5 = GroupGv3 (Up to SaGv3)}
  gPlatformFspPkgTokenSpaceGuid.CccHalfFrequency            | * | 0x01 | 0x00

  # !BSF NAME:{DIMM Non-Target ODT Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable DIMM Non-Target ODT Training}
  gPlatformFspPkgTokenSpaceGuid.DIMMNTODT                   | * | 0x01 | 0x00

  # !BSF NAME:{Unmatched Rx Calibration}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Rx Unmatched Calibration}
  gPlatformFspPkgTokenSpaceGuid.RXUNMATCHEDCAL              | * | 0x01 | 0x01

  # !BSF NAME:{Hard Post Package Repair}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Hard Post Package Repair}
  gPlatformFspPkgTokenSpaceGuid.PPR                         | * | 0x01 | 0x00

  # !BSF NAME:{PPR Test Type}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Select memory tests used in Post Package Repair flow. This option is only valid if PPR=1, otherwise ignored. Bit 0: WCMATS8 test, Bit 1: Data Retention test, Bit 2: X March test, Bit 3: X March G test, Bit 4: Y March Short test, Bit 5: Y March Long test. Default=0x3, WCMATS8 and data retention}
  gPlatformFspPkgTokenSpaceGuid.PprTestType                 | * | 0x1 | 0x6

  # !BSF NAME:{PPR Run Once}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When Eanble, PPR will run only once and then is disabled at next training cycle}
  gPlatformFspPkgTokenSpaceGuid.PprRunOnce                  | * | 0x1 | 0x1

  # !BSF NAME:{PPR Run During Fastboot}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When Eanble, PPR will run during fastboot}
  gPlatformFspPkgTokenSpaceGuid.PprRunAtFastboot            | * | 0x1 | 0x0

  # !BSF NAME:{PPR Repair Type}
  # !BSF TYPE:{Combo} OPTION:{0:Do not Repair (Default), 1:Soft Repair, 2:Hard Repair}
  # !BSF HELP:{PPR Repair Type: 0:Do not Repair (Default), 1:Soft Repair, 2:Hard Repair}
  gPlatformFspPkgTokenSpaceGuid.PprRepairType               | * | 0x01 | 0x0

  # !BSF NAME:{PPR Error Injection}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When Eanble, PPR will inject bad rows during testing}
  gPlatformFspPkgTokenSpaceGuid.PprErrorInjection           | * | 0x1 | 0x0

  # !BSF NAME:{PPR Repair Controller}  TYPE:{EditNum, HEX, (0x00,0x01)}
  # !BSF HELP:{PPR repair controller: User chooses to force repair specifc address }
  gPlatformFspPkgTokenSpaceGuid.PprRepairController         | * | 0x01 | 0x0

  # !BSF NAME:{PPR Repair Channel}  TYPE:{EditNum, HEX, (0x00,0x01)}
  # !BSF HELP:{PPR repair Channel: User chooses to force repair specifc address }
  gPlatformFspPkgTokenSpaceGuid.PprRepairChannel            | * | 0x01 | 0x0

  # !BSF NAME:{PPR Repair Dimm}  TYPE:{EditNum, HEX, (0x00,0x01)}
  # !BSF HELP:{PPR repair Dimm: User chooses to force repair specifc address }
  gPlatformFspPkgTokenSpaceGuid.PprRepairDimm               | * | 0x01 | 0x0

  # !BSF NAME:{PPR Repair Rank}  TYPE:{EditNum, HEX, (0x00,0x01)}
  # !BSF HELP:{PPR repair Rank: User chooses to force repair specifc address }
  gPlatformFspPkgTokenSpaceGuid.PprRepairRank               | * | 0x01 | 0x0

  # !BSF NAME:{PPR Repair Row}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{PPR repair Row: User chooses to force repair specifc address }
  gPlatformFspPkgTokenSpaceGuid.PprRepairRow                | * | 0x04 | 0x0

  # !BSF NAME:{PPR Repair Physical Address Low}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{PPR repair Physical Address Low: User chooses to force repair specifc address }
  gPlatformFspPkgTokenSpaceGuid.PprRepairPhysicalAddrLow    | * | 0x04 | 0x0

  # !BSF NAME:{PPR Repair Physical Address High}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{PPR repair Physical Address High: User chooses to force repair specifc address }
  gPlatformFspPkgTokenSpaceGuid.PprRepairPhysicalAddrHigh   | * | 0x04 | 0x0

  # !BSF NAME:{PPR Repair BankGroup}  TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{PPR repair BankGroup: User chooses to force repair specifc address }
  gPlatformFspPkgTokenSpaceGuid.PprRepairBankGroup          | * | 0x01 | 0x0

  # !BSF NAME:{LVR Auto Trim}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable LVR Auto Trim}
  gPlatformFspPkgTokenSpaceGuid.LVRAUTOTRIM                 | * | 0x01 | 0x00

  # !BSF NAME:{Compensation Optimization}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Compensation Optimization}
  gPlatformFspPkgTokenSpaceGuid.OPTIMIZECOMP                | * | 0x01 | 0x00

  # !BSF NAME:{Write DQ/DQS Retraining}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Write DQ/DQS Retraining}
  gPlatformFspPkgTokenSpaceGuid.WRTRETRAIN                  | * | 0x01 | 0x01

  # !BSF NAME:{DCC Phase Clk Calibration}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable DCC Phase Clk Calibration}
  gPlatformFspPkgTokenSpaceGuid.PHASECLKCAL                 | * | 0x01 | 0x00

  # !BSF NAME:{DCC Tline Clk Calibration}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable DCC Tline Clk Calibration}
  gPlatformFspPkgTokenSpaceGuid.TLINECLKCAL                 | * | 0x01 | 0x01

  # !BSF NAME:{DCC Tline Serializer Calibration}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable DCC PI Serializer Calibratio}
  gPlatformFspPkgTokenSpaceGuid.DCCPISERIALCAL              | * | 0x01 | 0x01

  # !BSF NAME:{RDDQODTT}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Read DQ ODT Training}
  gPlatformFspPkgTokenSpaceGuid.RDDQODTT                    | * | 0x01 | 0x01

  # !BSF NAME:{RDCTLET}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Read CTLE Training}
  gPlatformFspPkgTokenSpaceGuid.RDCTLET                     | * | 0x01 | 0x01

  # !BSF NAME:{RxVref Pre EMPHASIS Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Pre EMPHASIS Training}
  gPlatformFspPkgTokenSpaceGuid.EMPHASIS                    | * | 0x01 | 0x01

  # !BSF NAME:{RX DQS VOC Centring Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable RX DQS VOC Centring Training}
  gPlatformFspPkgTokenSpaceGuid.RXDQSVOCC              | * | 0x01 | 0x00

  # !BSF NAME:{NMode}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x02)}
  # !BSF HELP:{System command rate, range 0-2, 0 means auto, 1 = 1N, 2 = 2N}
  gPlatformFspPkgTokenSpaceGuid.NModeSupport                | * | 0x01 | 0x00

  # !BSF NAME:{LPDDR ODT RttWr}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Initial RttWr for LP4/5 in Ohms. 0x0 - Auto}
  gPlatformFspPkgTokenSpaceGuid.LpddrRttWr                  | * | 0x01 | 0x00

  # !BSF NAME:{Retrain on Fast flow Failure}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Restart MRC in Cold mode if SW MemTest fails during Fast flow.}
  gPlatformFspPkgTokenSpaceGuid.RetrainOnFastFail           | * | 0x01 | 0x01

  # !BSF NAME:{LPDDR ODT RttCa}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Initial RttCa for LP4/5 in Ohms. 0x0 - Auto}
  gPlatformFspPkgTokenSpaceGuid.LpddrRttCa                  | * | 0x01 | 0x00

  # !BSF NAME:{DIMM DFE Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable DIMM DFE Training}
  gPlatformFspPkgTokenSpaceGuid.WRTDIMMDFE                  | * | 0x01 | 0x01

  # !BSF NAME:{DDR5 ODT Timing Config}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable DDR5 ODT TIMING CONFIG}
  gPlatformFspPkgTokenSpaceGuid.DDR5ODTTIMING               | * | 0x01 | 0x00

  # !BSF NAME:{HobBufferSize} TYPE:{Combo}
  # !BSF OPTION:{0:Default, 1: 1 Byte, 2: 1 KB, 3: Max value}
  # !BSF HELP:{Size to set HOB Buffer. 0:Default, 1: 1 Byte, 2: 1 KB, 3: Max value(assuming 63KB total HOB size).}
  gPlatformFspPkgTokenSpaceGuid.HobBufferSize               | * | 0x01 | 0x00

  # !BSF NAME:{Early Command Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Early Command Training}
  gPlatformFspPkgTokenSpaceGuid.ECT                         | * | 0x01 | 0x01

  # !BSF NAME:{SenseAmp Offset Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable SenseAmp Offset Training}
  gPlatformFspPkgTokenSpaceGuid.SOT                         | * | 0x01 | 0x00

  # !BSF NAME:{Early ReadMPR Timing Centering 2D}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Early ReadMPR Timing Centering 2D}
  gPlatformFspPkgTokenSpaceGuid.ERDMPRTC2D                  | * | 0x01 | 0x01

  # !BSF NAME:{Read MPR Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Read MPR Training}
  gPlatformFspPkgTokenSpaceGuid.RDMPRT                      | * | 0x01 | 0x00

  # !BSF NAME:{Receive Enable Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Receive Enable Training}
  gPlatformFspPkgTokenSpaceGuid.RCVET                       | * | 0x01 | 0x01

  # !BSF NAME:{Jedec Write Leveling}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Jedec Write Leveling}
  gPlatformFspPkgTokenSpaceGuid.JWRL                        | * | 0x01 | 0x01

  # !BSF NAME:{Early Write Time Centering 2D}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Early Write Time Centering 2D}
  gPlatformFspPkgTokenSpaceGuid.EWRTC2D                     | * | 0x01 | 0x01

  # !BSF NAME:{Early Read Time Centering 2D}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Early Read Time Centering 2D}
  gPlatformFspPkgTokenSpaceGuid.ERDTC2D                     | * | 0x01 | 0x01

   # !BSF NAME:{Unmatched Write Time Centering 1D}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Unmatched Write Time Centering 1D}
  gPlatformFspPkgTokenSpaceGuid.UNMATCHEDWRTC1D             | * | 0x01 | 0x01

  # !BSF NAME:{Write Timing Centering 1D}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Write Timing Centering 1D}
  gPlatformFspPkgTokenSpaceGuid.WRTC1D                      | * | 0x01 | 0x01

  # !BSF NAME:{Write Voltage Centering 1D}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Write Voltage Centering 1D}
  gPlatformFspPkgTokenSpaceGuid.WRVC1D                      | * | 0x01 | 0x01

  # !BSF NAME:{Read Timing Centering 1D}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Read Timing Centering 1D}
  gPlatformFspPkgTokenSpaceGuid.RDTC1D                      | * | 0x01 | 0x01

  # !BSF NAME:{Dimm ODT Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Dimm ODT Training}
  gPlatformFspPkgTokenSpaceGuid.DIMMODTT                    | * | 0x01 | 0x01

  # !BSF NAME:{DIMM RON Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable DIMM RON Training}
  gPlatformFspPkgTokenSpaceGuid.DIMMRONT                    | * | 0x01 | 0x00

  # !BSF NAME:{Write Drive Strength/Equalization 2D}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Write Drive Strength/Equalization 2D}
  gPlatformFspPkgTokenSpaceGuid.WRDSEQT                     | * | 0x01 | 0x01

  # !BSF NAME:{Read Equalization Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Read Equalization Training}
  gPlatformFspPkgTokenSpaceGuid.RDEQT                       | * | 0x01 | 0x01

  # !BSF NAME:{Write Timing Centering 2D}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Write Timing Centering 2D}
  gPlatformFspPkgTokenSpaceGuid.WRTC2D                      | * | 0x01 | 0x00

  # !BSF NAME:{Read Timing Centering 2D}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Read Timing Centering 2D}
  gPlatformFspPkgTokenSpaceGuid.RDTC2D                      | * | 0x01 | 0x00

  # !BSF NAME:{Write Voltage Centering 2D}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Write Voltage Centering 2D}
  gPlatformFspPkgTokenSpaceGuid.WRVC2D                      | * | 0x01 | 0x00

  # !BSF NAME:{Read Voltage Centering 2D}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Read Voltage Centering 2D}
  gPlatformFspPkgTokenSpaceGuid.RDVC2D                      | * | 0x01 | 0x00

  # !BSF NAME:{Command Voltage Centering}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Command Voltage Centering}
  gPlatformFspPkgTokenSpaceGuid.CMDVC                       | * | 0x01 | 0x01

  # !BSF NAME:{Late Command Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Late Command Training}
  gPlatformFspPkgTokenSpaceGuid.LCT                         | * | 0x01 | 0x00

  # !BSF NAME:{Round Trip Latency Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Round Trip Latency Training}
  gPlatformFspPkgTokenSpaceGuid.RTL                         | * | 0x01 | 0x00

  # !BSF NAME:{Turn Around Timing Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Turn Around Timing Training}
  gPlatformFspPkgTokenSpaceGuid.TAT                         | * | 0x01 | 0x01

  # !BSF NAME:{Rmt Even Odd}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Rmt Even Odd}
  gPlatformFspPkgTokenSpaceGuid.RMTEVENODD                  | * | 0x01 | 0x00

  # !BSF NAME:{DIMM SPD Alias Test}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable DIMM SPD Alias Test}
  gPlatformFspPkgTokenSpaceGuid.ALIASCHK                    | * | 0x01 | 0x00

  # !BSF NAME:{Receive Enable Centering 1D}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Receive Enable Centering 1D}
  gPlatformFspPkgTokenSpaceGuid.RCVENC1D                    | * | 0x01 | 0x00

  # !BSF NAME:{Retrain Margin Check}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Retrain Margin Check}
  gPlatformFspPkgTokenSpaceGuid.RMC                         | * | 0x01 | 0x00

  # !BSF NAME:{ECC Support}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable ECC Support}
  gPlatformFspPkgTokenSpaceGuid.EccSupport                  | * | 0x01 | 0x00

  # !BSF NAME:{DLL DCC Calibration}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable DLL DCC Calibration}
  gPlatformFspPkgTokenSpaceGuid.DLLDCC                      | * | 0x01 | 0x01

  # !BSF NAME:{DLL BW Select Calibration}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable DLL BW Select Calibration}
  gPlatformFspPkgTokenSpaceGuid.DLLBWSEL                    | * | 0x01 | 0x01

  # !BSF NAME:{Ibecc}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{In-Band ECC Support}
  gPlatformFspPkgTokenSpaceGuid.Ibecc                       | * | 0x01 | 0x00

  # !BSF NAME:{IbeccParity}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{In-Band ECC Parity Control}
  gPlatformFspPkgTokenSpaceGuid.IbeccParity                 | * | 0x01 | 0x00

  # !BSF NAME:{MsHashEnable}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Controller Hash Enable: 0=Disable, <b>1=Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.MsHashEnable                | * | 0x01 | 0x00

  # !BSF NAME:{IbeccOperationMode}
  # !BSF TYPE:{Combo} OPTION:{0:Protect base on address range, 1: Non-protected, 2: All protected}
  # !BSF HELP:{In-Band ECC Operation Mode}
  gPlatformFspPkgTokenSpaceGuid.IbeccOperationMode          | * | 0x01 | 0x02

  # !BSF NAME:{IbeccProtectedRegionEnable}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{In-Band ECC Protected Region Enable }
  gPlatformFspPkgTokenSpaceGuid.IbeccProtectedRegionEnable  | * | 0x8 | { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
  # Added reserved space  UnusedUpdSpace45[1]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd45               | * | 0x1 | 0x00

  # !BSF NAME:{IbeccProtectedRegionBases} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{IBECC Protected Region Bases per IBECC instance}
  gPlatformFspPkgTokenSpaceGuid.IbeccProtectedRegionBase    | * | 0x10 | { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }

  # !BSF NAME:{IbeccProtectedRegionMasks} TYPE:{EditNum, HEX, (0x00,0x3FFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{IBECC Protected Region Masks}
  gPlatformFspPkgTokenSpaceGuid.IbeccProtectedRegionMask    | * | 0x10| { 0x3FFF, 0x3FFF, 0x3FFF, 0x3FFF, 0x3FFF, 0x3FFF, 0x3FFF, 0x3FFF }

  # !BSF NAME:{Memory Remap}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Memory Remap}
  gPlatformFspPkgTokenSpaceGuid.RemapEnable                 | * | 0x01 | 0x01

  # !BSF NAME:{Rank Interleave support}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Rank Interleave support. NOTE: RI and HORI can not be enabled at the same time.}
  gPlatformFspPkgTokenSpaceGuid.RankInterleave              | * | 0x01 | 0x01

  # !BSF NAME:{Enhanced Interleave support}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Enhanced Interleave support}
  gPlatformFspPkgTokenSpaceGuid.EnhancedInterleave          | * | 0x01 | 0x01

  # !BSF NAME:{Ch Hash Support}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Channel Hash Support. NOTE: ONLY if Memory interleaved Mode}
  gPlatformFspPkgTokenSpaceGuid.ChHashEnable                | * | 0x01 | 0x01

  # !BSF NAME:{DDR PowerDown and idle counter}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable DDR PowerDown and idle counter(For LPDDR Only)}
  gPlatformFspPkgTokenSpaceGuid.EnablePwrDn                 | * | 0x01 | 0x01

  # !BSF NAME:{DDR PowerDown and idle counter}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable DDR PowerDown and idle counter(For LPDDR Only)}
  gPlatformFspPkgTokenSpaceGuid.EnablePwrDnLpddr            | * | 0x01 | 0x01

  # !BSF NAME:{SelfRefresh Enable}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable SelfRefresh Enable}
  gPlatformFspPkgTokenSpaceGuid.SrefCfgEna                  | * | 0x01 | 0x01

  # !BSF NAME:{Throttler CKEMin Defeature}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Throttler CKEMin Defeature(For LPDDR Only)}
  gPlatformFspPkgTokenSpaceGuid.ThrtCkeMinDefeatLpddr       | * | 0x01 | 0x01

  # !BSF NAME:{Throttler CKEMin Defeature}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Throttler CKEMin Defeature}
  gPlatformFspPkgTokenSpaceGuid.ThrtCkeMinDefeat            | * | 0x01 | 0x01

  # !BSF NAME:{Exit On Failure (MRC)}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Exit On Failure (MRC)}
  gPlatformFspPkgTokenSpaceGuid.ExitOnFailure               | * | 0x01 | 0x01

  # !BSF NAME:{Wck Pad DCC Calibration}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Wck Pad DCC Calibration}
  gPlatformFspPkgTokenSpaceGuid.WCKPADDCCCAL                | * | 0x01 | 0x01

  # !BSF NAME:{DCC PI Code LUT Calibration}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable DCC PI Code LUT Calibration}
  gPlatformFspPkgTokenSpaceGuid.DCCPICODELUT                | * | 0x01 | 0x00

  # !BSF NAME:{Read Voltage Centering 1D}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Read Voltage Centering 1D}
  gPlatformFspPkgTokenSpaceGuid.RDVC1D                      | * | 0x01 | 0x01

  # !BSF NAME:{TxDqTCO Comp Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable TxDqTCO Comp Training}
  gPlatformFspPkgTokenSpaceGuid.TXTCO                       | * | 0x01 | 0x01

  # !BSF NAME:{ClkTCO Comp Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable ClkTCO Comp Training}
  gPlatformFspPkgTokenSpaceGuid.CLKTCO                      | * | 0x01 | 0x00

  # !BSF NAME:{CMD Slew Rate Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable CMD Slew Rate Training}
  gPlatformFspPkgTokenSpaceGuid.CMDSR                       | * | 0x01 | 0x00

  # !BSF NAME:{CMD Drive Strength and Tx Equalization}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable CMD Drive Strength and Tx Equalization}
  gPlatformFspPkgTokenSpaceGuid.CMDDSEQ                     | * | 0x01 | 0x00

  # !BSF NAME:{DIMM CA ODT Training}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable DIMM CA ODT Training}
  gPlatformFspPkgTokenSpaceGuid.DIMMODTCA                   | * | 0x01 | 0x00

  # !BSF NAME:{Read Vref Decap Training*}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Read Vref Decap Training*}
  gPlatformFspPkgTokenSpaceGuid.RDVREFDC                    | * | 0x01 | 0x00

  # !BSF NAME:{Rank Margin Tool Per Bit}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Rank Margin Tool Per Bit}
  gPlatformFspPkgTokenSpaceGuid.RMTBIT                      | * | 0x01 | 0x0

  # !BSF NAME:{Ref PI Calibration}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Ref PI Calibration}
  gPlatformFspPkgTokenSpaceGuid.REFPI                       | * | 0x01 | 0x01

  # !BSF NAME:{VccClk FF Offset Correction}
  # !BSF TYPE:{Combo} OPTION:{0:Disable, 1:Enable}
  # !BSF HELP:{Enable/Disable VccClk FF Offset Correction}
  gPlatformFspPkgTokenSpaceGuid.VCCCLKFF                    | * | 0x01 | 0x00

  # !BSF NAME:{Data PI Linearity Calibration}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable {Data PI Linearity Calibration}
  gPlatformFspPkgTokenSpaceGuid.DATAPILIN                   | * | 0x01 | 0x01

  # !BSF NAME:{Ddr5 Rx Cross-Talk Cancellation}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable {Ddr5 Rx Cross-Talk Cancellation}
  gPlatformFspPkgTokenSpaceGuid.DDR5XTALK                   | * | 0x01 | 0x00

  # !BSF NAME:{Retrain On Working Channel}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Retrain On Working Channel feature}
  gPlatformFspPkgTokenSpaceGuid.RetrainToWorkingChannel     | * | 0x01 | 0x00

  # !BSF NAME:{Row Press}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Row Press feature}
  gPlatformFspPkgTokenSpaceGuid.RowPressEn                  | * | 0x01 | 0x00

  # !BSF NAME:{DBI feature}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable DBI feature}
  gPlatformFspPkgTokenSpaceGuid.DBI                         | * | 0x01 | 0x00

  # !BSF NAME:{DDR5 MR7 WICA support}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable if DDR5 DRAM Device supports MR7 WICA 0.5 tCK offset alignment}
  gPlatformFspPkgTokenSpaceGuid.IsDdr5MR7WicaSupported      | * | 0x01 | 0x00

  # !BSF NAME:{Ch Hash Interleaved Bit}
  # !BSF TYPE:{Combo} OPTION:{0:BIT6, 1:BIT7, 2:BIT8, 3:BIT9, 4:BIT10, 5:BIT11, 6:BIT12, 7:BIT13}
  # !BSF HELP:{Select the BIT to be used for Channel Interleaved mode. NOTE: BIT7 will interlave the channels at a 2 cacheline granularity, BIT8 at 4 and BIT9 at 8. Default is BIT8}
  gPlatformFspPkgTokenSpaceGuid.ChHashInterleaveBit         | * | 0x01 | 0x02
  # Added reserved space UnusedUpdSpace6
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd6             | * | 0x01 | 0x00

  # !BSF NAME:{Ch Hash Mask}
  # !BSF TYPE:{EditNum, HEX, (0x0000, 0x3FFF)}
  # !BSF HELP:{Set the BIT(s) to be included in the XOR function. NOTE BIT mask corresponds to BITS [19:6] Default is 0x30CC}
  gPlatformFspPkgTokenSpaceGuid.ChHashMask                  | * | 0x02 | 0x830

  # !BSF NAME:{CccPinsInterleaved}
  # !BSF TYPE:{EditNum, HEX, (0x0000, 0x3FFF)}
  # !BSF HELP:{Interleaving mode of CCC pins which depends on board routing: <b>0=Disable</b>, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.CccPinsInterleaved          | * | 0x01 | 0x00

  # !BSF NAME:{Throttler CKEMin Timer}
  # !BSF TYPE:{EditNum, HEX, (0x0, 0xFF)}
  # !BSF HELP:{Timer value for CKEMin, range[255;0]. Req'd min of SC_ROUND_T + BYTE_LENGTH (4). Dfault is 0x00}
  gPlatformFspPkgTokenSpaceGuid.ThrtCkeMinTmr               | * | 0x01 | 0x00

  # !BSF NAME:{Allow Opp Ref Below Write Threhold} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Allow opportunistic refreshes while we don't exit power down.}
  gPlatformFspPkgTokenSpaceGuid.AllowOppRefBelowWriteThrehold    | * | 0x01 | 0x0

  # !BSF NAME:{Write Threshold} TYPE:{EditNum, HEX, (0x00, 0x3F)}
  # !BSF HELP:{Number of writes that can be accumulated while CKE is low before CKE is asserted.}
  gPlatformFspPkgTokenSpaceGuid.WriteThreshold              | * | 0x01 | 0x0

  # !BSF NAME:{MC_REFRESH_RATE}
  # !BSF TYPE:{Combo} OPTION:{0:NORMAL Refresh, 1:1x Refresh, 2:2x Refresh, 3:4x Refresh}
  # !BSF HELP:{Type of Refresh Rate used to prevent Row Hammer. Default is NORMAL Refresh}
  gPlatformFspPkgTokenSpaceGuid.McRefreshRate               | * | 0x01 | 0x00

  # !BSF NAME:{Refresh Watermarks}
  # !BSF TYPE:{Combo} OPTION:{0:Set Refresh Watermarks to Low, 1:Set Refresh Watermarks to High (Default)}
  # !BSF HELP:{Refresh Watermarks: 0-Low, 1-High (default)}
  gPlatformFspPkgTokenSpaceGuid.RefreshWm                   | * | 0x01 | 0x01

  # !BSF NAME:{User Manual Threshold}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Disabled: Predefined threshold will be used.\nEnabled: User Input will be used.}
  gPlatformFspPkgTokenSpaceGuid.UserThresholdEnable         | * | 0x01 | 0x00

  # !BSF NAME:{User Manual Budget}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Disabled: Configuration of memories will defined the Budget value.\nEnabled: User Input will be used.}
  gPlatformFspPkgTokenSpaceGuid.UserBudgetEnable            | * | 0x01 | 0x00

  # !BSF NAME:{Power Down Mode}
  # !BSF TYPE:{Combo} OPTION:{0x0:No Power Down, 0x1:APD, 0x6:PPD DLL OFF, 0xFF:Auto}
  # !BSF HELP:{This option controls command bus tristating during idle periods}
  gPlatformFspPkgTokenSpaceGuid.PowerDownMode               | * | 0x01 | 0xFF

  # !BSF NAME:{Pwr Down Idle Timer}
  # !BSF TYPE:{EditNum, HEX, (0x0, 0xFF)}
  # !BSF HELP:{The minimum value should = to the worst case Roundtrip delay + Burst_Length. 0 means AUTO: 64 for ULX/ULT, 128 for DT/Halo}
  gPlatformFspPkgTokenSpaceGuid.PwdwnIdleCounter            | * | 0x01 | 0x00

  # !BSF NAME:{Page Close Idle Timeout}
  # !BSF TYPE:{Combo} OPTION:{0:Enabled, 1:Disabled}
  # !BSF HELP:{This option controls Page Close Idle Timeout}
  gPlatformFspPkgTokenSpaceGuid.DisPgCloseIdleTimeout       | * | 0x01 | 0x0

  # !BSF NAME:{Bitmask of ranks that have CA bus terminated}
  # !BSF TYPE:{EditNum, HEX, (0x0, 0xFF)}
  # !BSF HELP:{LPDDR5: Bitmask of ranks that have CA bus terminated. <b>0x01=Default, Rank0 is terminating and Rank1 is non-terminating</b>}
  gPlatformFspPkgTokenSpaceGuid.CmdRanksTerminated          | * | 0x01 | 0x01

  # !BSF NAME:{MRC Safe Mode Override}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x7)}
  # !BSF HELP:{SafeModeOverride[0] Enable DdrSafeMode override, SafeModeOverride[1] Enable McSafeMode override, SafeModeOverride[2] Enable MrcSafeMode override, SafeModeOverride[3] Enable Training Algorithm (TrainingEnables) safe mode override, SafeModeOverride[4] Enable SaGv safe mode override}
  gPlatformFspPkgTokenSpaceGuid.SafeModeOverride            | * | 0x01 | 0xFF
  # Added reserved space UnusedUpdSpace7[5]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd7                | * | 0x05 | {0x00}

  # !BSF NAME:{DDR Phy Safe Mode Support}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{DdrSafeMode[0]: Basic PM Features, DdrSafeMode[1]: Spine Gating, DdrSafeMode[2]: Advanced DCC, DdrSafeMode[3]: R2R Training, DdrSafeMode[4]: Transformer Mode, DdrSafeMode[5]: PLL Operation, DdrSafeMode[6]: Safe ODT}
  gPlatformFspPkgTokenSpaceGuid.DdrSafeMode                 | * | 0x04 | 0x04

  # !BSF NAME:{Mc Safe Mode Support}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{McSafeMode[0]: Clk Gate / BGF, McSafeMode[1]: CKE Pdwn, McSafeMode[2]: Tristate, McSafeMode[3]: PHY Power States / Clock Spine, McSafeMode[4]: Same Rank TA, McSafeMode[5]: Different Rank TA, McSafeMode[6]: MR4_Period / ZQCAL_Period McSafeMode[7]: LP5 Wck Mode, SafeMode[8]: Self Refresh, McSafeMode[9]: WR/RD Retraining, McSafeMode[10]: Power Saving}
  gPlatformFspPkgTokenSpaceGuid.McSafeMode                  | * | 0x01 | 0x00

  # !BSF NAME:{Ask MRC to clear memory content}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Ask MRC to clear memory content <b>0: Do not Clear Memory;</b> 1: Clear Memory.}
  gPlatformFspPkgTokenSpaceGuid.CleanMemory                 | * | 0x01 | 0x00

  # !BSF NAME:{Tseg Retry Count} TYPE:{Combo}
  # !BSF OPTION:{0: Default, 1:3}
  # !BSF HELP:{Tseg Retry count will increase based on TSEG Region Fail count}
  gPlatformFspPkgTokenSpaceGuid.RetryCount                  | * | 0x01 | 0x0

  # !BSF NAME:{Mrc Ppr Status} TYPE:{Combo}
  # !BSF OPTION:{0: PASS, 1: FAIL(Default)}
  # !BSF HELP:{ Get Mrc PPR Status after PPR Recovery flow will get Trigger}
  gPlatformFspPkgTokenSpaceGuid.MrcPprStatus                | * | 0x01 | 0x0

  # !BSF NAME:{Tseg Memory Test Status} TYPE:{Combo}
  # !BSF OPTION:{0: PASS, 1: FAIL(Default)}
  # !BSF HELP:{ If enabled, PPR Recovery flow will get Trigger}
  gPlatformFspPkgTokenSpaceGuid.TsegMemoryTestStatus        | * | 0x01 | 0x0

  # !BSF NAME:{Ppr Recovery Status Enable} TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{0: Disabled(Default), 1: Enabled.  If enabled, PPR Recovery flow will get Trigger.}
  gPlatformFspPkgTokenSpaceGuid.PprRecoveryStatusEnable     | * | 0x01 | 0x0

  # !BSF NAME:{Safe Loading Bios Enable State} TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{0: Disabled(Default), 1: Enabled. If enabled, Memory diagnostic will perform for TSEG Region.}
  gPlatformFspPkgTokenSpaceGuid.SafeLoadingBiosEnableState  | * | 0x01 | 0x0

  # !BSF NAME:{BDAT test type} TYPE:{Combo}
  # !BSF OPTION:{0:RMT per Rank, 1:RMT per Bit, 2:Margin2D}
  # !BSF HELP:{When BdatEnable is set to TRUE, this option selects the type of data which will be populated in the BIOS Data ACPI Tables: <b>0=RMT</b>, 1=RMT Per Bit, 2=Margin 2D.}
  gPlatformFspPkgTokenSpaceGuid.MrcBdatTestType             | * | 0x01 | 0x0

  # !BSF NAME:{MrcBdatEnable} TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{0: Disabled(Default), 1: Enabled. This field enables the generation of the BIOS DATA ACPI Tables: <b>0=FALSE</b>, 1=TRUE.}
  gPlatformFspPkgTokenSpaceGuid.MrcBdatEnable               | * | 0x01 | 0x0

  # !BSF NAME:{DisableMrcRetraining} TYPE:{Combo}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{0: Disabled(Default), 1: Enabled. Enable/Disable DisableMrcRetraining}
  gPlatformFspPkgTokenSpaceGuid.DisableMrcRetraining        | * | 0x01 | 0x0

  # !BSF NAME:{RMTLoopCount} TYPE:{EditNum, HEX, (0, 0x20)}
  # !BSF HELP:{Specifies the Loop Count to be used during Rank Margin Tool Testing. 0 - AUTO}
  gPlatformFspPkgTokenSpaceGuid.RMTLoopCount                | * | 0x01 | 0x00

  # !BSF NAME:{DdrOneDpc} TYPE:{Combo}
  # !BSF OPTION:{0: Disabled, 1: Enabled on DIMM0 only, 2: Enabled on DIMM1 only, 3: Enabled}
  # !BSF HELP:{DDR 1DPC performance feature for 2R DIMMs. Can be enabled on DIMM0 or DIMM1 only, or on both (default)}
  gPlatformFspPkgTokenSpaceGuid.DdrOneDpc                   | * | 0x01 | 0x03

  # !BSF NAME:{Vddq Voltage Override}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{# is multiple of 1mV where 0 means Auto.}
  gPlatformFspPkgTokenSpaceGuid.VddqVoltageOverride         | * | 0x02 | 0x00

  # !BSF NAME:{VccIog Voltage Override}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{# is multiple of 1mV where 0 means Auto.}
  gPlatformFspPkgTokenSpaceGuid.VccIogVoltageOverride       | * | 0x02 | 0x00

  # !BSF NAME:{VccClk Voltage Override}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{# is multiple of 1mV where 0 means Auto.}
  gPlatformFspPkgTokenSpaceGuid.VccClkVoltageOverride       | * | 0x02 | 0x00

  # !BSF NAME:{ThrtCkeMinTmrLpddr}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Throttler CKE min timer for LPDDR: 0=Minimal, 0xFF=Maximum, <b>0x00=Default</b>}
  gPlatformFspPkgTokenSpaceGuid.ThrtCkeMinTmrLpddr          | * | 0x01 | 0x00
  # Added reserved space UnusedUpdSpace8
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd8                | * | 0x01 | 0x00

  # !BSF NAME:{Margin limit check L2}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{Margin limit check L2 threshold: <b>100=Default</b>}
  gPlatformFspPkgTokenSpaceGuid.MarginLimitL2               | * | 0x02 | 0x64

  # !BSF NAME:{Extended Bank Hashing}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Eanble/Disable ExtendedBankHashing}
  gPlatformFspPkgTokenSpaceGuid.ExtendedBankHashing         | * | 0x01 | 0x01

  # !BSF NAME:{DRFM Blast Radius Configuration}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}}
  # !BSF HELP:{Row Hammer DRFM Blast Radius Configuration determines number of victim rows around aggressor row targeted to send the DRFM sequence to: <b>2=BlastRadius 2</b>, 3=BlastRadius 3, 4=BlastRadius 4}
  gPlatformFspPkgTokenSpaceGuid.DrfmBrc                     | * | 0x01 | 0x02

  # !BSF NAME:{LP5 Command Pins Mapping}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{BitMask where bits [3:0] are Controller 0 Channel [3:0] and bits [7:4] are Controller 1 Channel [3:0]. 0 = CCC pin mapping is Ascending, 1 = CCC pin mapping is Descending.}
  gPlatformFspPkgTokenSpaceGuid.Lp5CccConfig                | * | 0x01 | 0xFF

  # !BSF NAME:{Command Pins Mirrored}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{BitMask where bits [3:0] are Controller 0 Channel [3:0] and bits [7:4] are Controller 1 Channel [3:0]. 0 = No Command Mirror and 1 = Command Mirror.}
  gPlatformFspPkgTokenSpaceGuid.CmdMirror                   | * | 0x01 | 0x00

  # !BSF NAME:{Time Measure} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Time Measure: 0(Default)=Disable, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.MrcTimeMeasure              | * | 0x01 | 0x00

  # !BSF NAME:{DVFSQ Enabled}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable DVFSQ}
  gPlatformFspPkgTokenSpaceGuid.DvfsqEnabled                | * | 0x01 | 0x00

  # !BSF NAME:{E-DVFSC Enabled}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Eanble/Disable DVFSC}
  gPlatformFspPkgTokenSpaceGuid.DvfscEnabled                | * | 0x01 | 0x00

  ## MEMORY_CONFIGURATION End

  # !BSF NAME:{MrcPreMemRsvd} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Reserved for MRC Pre-Mem}
  gPlatformFspPkgTokenSpaceGuid.MrcPreMemRsvd               | * | 64   | {0x00}

  #
  # MRC Block End
  #

  ## SA_MISC_PEI_PREMEM_CONFIG Start

  # !BSF NAME:{Board Type}
  # !BSF TYPE:{Combo} OPTION:{0:Mobile/Mobile Halo, 1:Desktop/DT Halo, 5:ULT/ULX/Mobile Halo, 7:UP Server}
  # !BSF HELP:{MrcBoardType, Options are 0=Mobile/Mobile Halo, 1=Desktop/DT Halo, 5=ULT/ULX/Mobile Halo, 7=UP Server}
  gPlatformFspPkgTokenSpaceGuid.UserBd                      | * | 0x01 | 0x00

  # !BSF NAME:{Spd Address Table} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !HDR STRUCT:{UINT8}
  # !BSF HELP:{Specify SPD Address table for CH0D0/CH0D1/CH1D0&CH1D1. MemorySpdPtr will be used if SPD Address is 00}
  gPlatformFspPkgTokenSpaceGuid.SpdAddressTable              | * | 0x10 | { 0x00, 0x0, 0x00, 0x0, 0x00, 0x0, 0x00, 0x0, 0x00, 0x0, 0x00, 0x0, 0x00, 0x0, 0x00, 0x0 }

  # !BSF NAME:{Enable/Disable MRC TXT dependency} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When enabled MRC execution will wait for TXT initialization to be done first. Disabled(0x0)(Default): MRC will not wait for TXT initialization, Enabled(0x1): MRC will wait for TXT initialization}
  gPlatformFspPkgTokenSpaceGuid.TxtImplemented              | * | 0x01 | 0x1

  # !BSF NAME:{PCIE Resizable BAR Support} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable PCIE Resizable BAR Support.0: Disable; 1: Enable; 2: Auto(Default).}
  gPlatformFspPkgTokenSpaceGuid.PcieResizableBarSupport          | * | 0x01 | 0x02


  # !BSF NAME:{Skip external display device scanning} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable: Do not scan for external display device, Disable (Default): Scan external display devices}
  gPlatformFspPkgTokenSpaceGuid.SkipExtGfxScan              | * | 0x01 | 0x0

  # !BSF NAME:{Generate BIOS Data ACPI Table} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable: Generate BDAT for MRC RMT or SA PCIe data. Disable (Default): Do not generate it}
  gPlatformFspPkgTokenSpaceGuid.BdatEnable                  | * | 0x01 | 0x00

  # !BSF NAME:{BdatTestType} TYPE:{Combo}
  # !BSF OPTION:{0:RMT per Rank, 1:RMT per Bit, 2:Margin2D}
  # !BSF HELP:{Indicates the type of Memory Training data to populate into the BDAT ACPI table.}
  gPlatformFspPkgTokenSpaceGuid.BdatTestType                | * | 0x1 | 0x00

  ## SA_MISC_PEI_PREMEM_CONFIG End


  ## PCH_HSIO_PCIE_PREMEM_CONFIG Start

  # !BSF NAME:{Enable PCH HSIO PCIE Rx Set Ctle} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable PCH PCIe Gen 3 Set CTLE Value.}
  gPlatformFspPkgTokenSpaceGuid.PchPcieHsioRxSetCtleEnable          | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCH HSIO PCIE Rx Set Ctle Value} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{PCH PCIe Gen 3 Set CTLE Value.}
  gPlatformFspPkgTokenSpaceGuid.PchPcieHsioRxSetCtle                | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Enble PCH HSIO PCIE TX Gen 1 Downscale Amplitude Adjustment value override} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PchPcieHsioTxGen1DownscaleAmpEnable | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCH HSIO PCIE Gen 2 TX Output Downscale Amplitude Adjustment value} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{PCH PCIe Gen 2 TX Output Downscale Amplitude Adjustment value.}
  gPlatformFspPkgTokenSpaceGuid.PchPcieHsioTxGen1DownscaleAmp       | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Enable PCH HSIO PCIE TX Gen 2 Downscale Amplitude Adjustment value override} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PchPcieHsioTxGen2DownscaleAmpEnable | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCH HSIO PCIE Gen 2 TX Output Downscale Amplitude Adjustment value} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{PCH PCIe Gen 2 TX Output Downscale Amplitude Adjustment value.}
  gPlatformFspPkgTokenSpaceGuid.PchPcieHsioTxGen2DownscaleAmp       | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Enable PCH HSIO PCIE TX Gen 3 Downscale Amplitude Adjustment value override} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PchPcieHsioTxGen3DownscaleAmpEnable | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCH HSIO PCIE Gen 3 TX Output Downscale Amplitude Adjustment value} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{PCH PCIe Gen 3 TX Output Downscale Amplitude Adjustment value.}
  gPlatformFspPkgTokenSpaceGuid.PchPcieHsioTxGen3DownscaleAmp       | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Enable PCH HSIO PCIE Gen 1 TX Output De-Emphasis Adjustment Setting value override} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PchPcieHsioTxGen1DeEmphEnable       | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCH HSIO PCIE Gen 1 TX Output De-Emphasis Adjustment value} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{PCH PCIe Gen 1 TX Output De-Emphasis Adjustment Setting.}
  gPlatformFspPkgTokenSpaceGuid.PchPcieHsioTxGen1DeEmph             | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Enable PCH HSIO PCIE Gen 2 TX Output -3.5dB De-Emphasis Adjustment Setting value override} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PchPcieHsioTxGen2DeEmph3p5Enable    | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCH HSIO PCIE Gen 2 TX Output -3.5dB De-Emphasis Adjustment value} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{PCH PCIe Gen 2 TX Output -3.5dB De-Emphasis Adjustment Setting.}
  gPlatformFspPkgTokenSpaceGuid.PchPcieHsioTxGen2DeEmph3p5          | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Enable PCH HSIO PCIE Gen 2 TX Output -6.0dB De-Emphasis Adjustment Setting value override} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PchPcieHsioTxGen2DeEmph6p0Enable    | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCH HSIO PCIE Gen 2 TX Output -6.0dB De-Emphasis Adjustment value} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{PCH PCIe Gen 2 TX Output -6.0dB De-Emphasis Adjustment Setting.}
  gPlatformFspPkgTokenSpaceGuid.PchPcieHsioTxGen2DeEmph6p0          | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  ## PCH_HSIO_PCIE_PREMEM_CONFIG End

  ## HDAUDIO_PREMEM_CONFIG Start

  # !BSF NAME:{HD Audio DMIC Link Clock Select} TYPE:{Combo} OPTION:{0: Both, 1: ClkA, 2: ClkB}
  # !BSF HELP:{Determines DMIC<N> Clock Source. 0: Both, 1: ClkA, 2: ClkB}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkDmicClockSelect  | * | 0x02 | { 0x01, 0x01 }

  # !BSF NAME:{Enable Intel HD Audio (Azalia)} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0: Disable, 1: Enable (Default) Azalia controller}
  gPlatformFspPkgTokenSpaceGuid.PchHdaEnable                | * | 0x01 | 0x01

  # !BSF NAME:{Universal Audio Architecture compliance for DSP enabled system} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0: Not-UAA Compliant (Intel SST driver supported only), 1: UAA Compliant (HDA Inbox driver or SST driver supported).}
  gPlatformFspPkgTokenSpaceGuid.PchHdaDspUaaCompliance      | * | 0x01 | 0x00

  # !BSF NAME:{Enable HD Audio Link} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable HD Audio Link. Muxed with SSP0/SSP1/SNDW1.}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkHdaEnable    | * | 0x01 | 0x01

  # !BSF NAME:{Enable HDA SDI lanes} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Enable/disable HDA SDI lanes.}
  gPlatformFspPkgTokenSpaceGuid.PchHdaSdiEnable            | * | 0x02 | { 0x01, 0x00}

  # !BSF NAME:{Enable HD Audio DMIC_N Link} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Enable/disable HD Audio DMIC1 link. Muxed with SNDW3.}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkDmicEnable  | * | 0x02 | { 0x00, 0x00}

  # !BSF NAME:{DMIC<N> ClkA Pin Muxing (N - DMIC number)} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Determines DMIC<N> ClkA Pin muxing. See  GPIO_*_MUXING_DMIC<N>_CLKA_*}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkDmicClkAPinMux | * | 0x8 | { 0x0, 0x0 }

  # !BSF NAME:{Enable HD Audio DSP} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable HD Audio DSP feature.}
  gPlatformFspPkgTokenSpaceGuid.PchHdaDspEnable             | * | 0x01 | 0x01
  # Added reserved space UnusedUpdSpace9[3]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd9                | * | 0x03 | {0x00}

  # !BSF NAME:{DMIC<N> Data Pin Muxing} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Determines DMIC<N> Data Pin muxing. See GPIO_*_MUXING_DMIC<N>_DATA_*}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkDmicDataPinMux | * | 0x8 | {0x00000000, 0x00000000}

  # !BSF NAME:{Enable HD Audio SSP0 Link} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/disable HD Audio SSP_N/I2S link. Muxed with HDA. N-number 0-5}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkSspEnable    | * | 0x07 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCH Hda Disc Bt Off Enabled} TYPE:{$EN_DIS}
  # !BSF HELP:{Hda Disc Bt Off Enabled}
  gPlatformFspPkgTokenSpaceGuid.PchHdaDiscBtOffEnabled | * | 0x01 | 0x00

  # !BSF NAME:{PCH HDA Discrete BT Offload Ssp Link} TYPE:{$EN_DIS}
  # !BSF HELP:{Discrete BT Offload Ssp Link}
  gPlatformFspPkgTokenSpaceGuid.PchHdaDiscBtOffSspLink | * | 0x04 | 0x00

  # !BSF NAME:{SSP<N> Sclk Pin Muxing (N - SSP Number)} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Determines SSP<N> Sclk Pin muxing. See GPIOV2_*_MUXING_I2S*_SCLK}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkSspSclkPinMux | * | 0x1C | { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }

  # !BSF NAME:{SSP<N> Sfmr Pin Muxing (N - SSP Number)} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Determines SSP<N> Sfmr Pin muxing. See GPIOV2_*_MUXING_I2S*_SFMR}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkSspSfmrPinMux | * | 0x1C | { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }

  # !BSF NAME:{SSP<N> Txd Pin Muxing (N - SSP Number)} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Determines SSP<N> Txd Pin muxing. See GPIOV2_*_MUXING_I2S*_TXD}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkSspTxdPinMux | * | 0x1C | { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }

  # !BSF NAME:{SSP<N> Rxd Pin Muxing (N - SSP Number)} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Determines SSP<N> Rxd Pin muxing. See GPIOV2_*_MUXING_I2S*_RXD}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkSspRxdPinMux | * | 0x1C | { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }

  # !BSF NAME:{Enable HD Audio SoundWire#N Link} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Enable/disable HD Audio SNDW#N link. Muxed with HDA.}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkSndwEnable  | * | 0x05 | {0x01, 0x00, 0x00, 0x00, 0x01}

  # !BSF NAME:{iDisp-Link Frequency} TYPE:{Combo} OPTION:{4: 96MHz, 3: 48MHz}
  # !BSF HELP:{iDisp-Link Freq (PCH_HDAUDIO_LINK_FREQUENCY enum): 4: 96MHz, 3: 48MHz.}
  gPlatformFspPkgTokenSpaceGuid.PchHdaIDispLinkFrequency    | * | 0x01 | 0x04

  # !BSF NAME:{iDisp-Link T-mode} TYPE:{Combo} OPTION:{0: 2T, 2: 4T, 3: 8T, 4: 16T}
  # !BSF HELP:{iDisp-Link T-Mode (PCH_HDAUDIO_IDISP_TMODE enum): 0: 2T, 2: 4T, 3: 8T, 4: 16T}
  gPlatformFspPkgTokenSpaceGuid.PchHdaIDispLinkTmode        | * | 0x01 | 0x02

  # !BSF NAME:{Sndw Multilane enablement} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{SoundWire Multiline enablement. Default is DISABLE. 0: DISABLE, 1: Two lines enabled, 2: Three lines enabled, 3: Four Lines enabled.}
  gPlatformFspPkgTokenSpaceGuid.PchHdAudioSndwMultilaneEnable    | * | 0x02 | { 0x00, 0x02 }
  # Added reserved space UnusedUpdSpace10[3]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd10                | * | 0x03 | {0x00}

  # !BSF NAME:{SoundWire<N> Clk Pin Muxing (N - SoundWire number)} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Determines SoundWire<N> Clk Pin muxing. See  GPIOV2_*_MUXING_SNDW<N>_CLK*}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkMultilaneClkPinMux | * | 0x8 | { 0x0, 0x0 }

  # !BSF NAME:{SoundWire<N> Multilane Data0 Pin Muxing (N - SoundWire number)} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Determines SoundWire<N> Multilane Clk Pin muxing. See  GPIOV2_*_MUXING_SNDW<N>_DATA0*}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkMultilaneData0PinMux | * | 0x8 | { 0x0, 0x0 }

  # !BSF NAME:{SoundWire<N> Multilane Data1 Pin Muxing (N - SoundWire number)} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Determines SoundWire<N> Multilane Clk Pin muxing. See  GPIOV2_*_MUXING_SNDW<N>_DATA1*}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkMultilaneData1PinMux | * | 0x8 | { 0x0, 0x0 }

  # !BSF NAME:{SoundWire<N> Multilane Data2 Pin Muxing (N - SoundWire number)} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Determines SoundWire<N> Multilane Clk Pin muxing. See  GPIOV2_*_MUXING_SNDW<N>_DATA2*}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkMultilaneData2PinMux | * | 0x8 | { 0x0, 0x0 }

  # !BSF NAME:{SoundWire<N> Multilane Data3 Pin Muxing (N - SoundWire number)} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Determines SoundWire<N> Multilane Clk Pin muxing. See  GPIOV2_*_MUXING_SNDW<N>_DATA3*}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.PchHdaAudioLinkMultilaneData3PinMux | * | 0x8 | { 0x0, 0x0 }

  # !BSF NAME:{iDisplay Audio Codec disconnection} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0: Not disconnected, enumerable, 1: Disconnected SDI, not enumerable.}
  gPlatformFspPkgTokenSpaceGuid.PchHdaIDispCodecDisconnect  | * | 0x01 | 0x00

  # !BSF NAME:{Sndw<N> Interface for Multilanes (N - SoundWire number)} OPTION:{0: Sndw0, 1: Sndw1, 2: Sndw2, 3: Sndw3, 4: Sndw4, 5: Sndw5}
  # !BSF HELP:{Determines Sndw<N> Interface Multilanes. 0: Sndw0, 1: Sndw1, 2: Sndw2, 3: Sndw3, 4: Sndw4, 5: Sndw5
  # !HDR STRUCT:{UINT8}
  gPlatformFspPkgTokenSpaceGuid.PchHdAudioSndwMultilaneSndwInterface | * | 0x2 | { 0x02, 0x03 }
  # Added reserved spaceUnusedUpdSpace11
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd11               | * | 0x01 | 0x00

  # !BSF NAME:{Audio Sub System IDs} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Set default Audio Sub System IDs. If its set to 0 then value from Strap is used.}
  gPlatformFspPkgTokenSpaceGuid.ResetWaitTimer           | * | 0x02 | 0x258

  # !BSF NAME:{HDA Power/Clock Gating (PGD/CGD)} TYPE:{Combo}
  # !BSF OPTION:{0: POR, 1: Force Enable, 2: Force Disable}
  # !BSF HELP:{Enable/Disable HD Audio Power and Clock Gating(POR: Enable). 0: PLATFORM_POR, 1: FORCE_ENABLE, 2: FORCE_DISABLE.}
  gPlatformFspPkgTokenSpaceGuid.PchHdaTestPowerClockGating    | * | 0x01 | 0x00

  # !BSF NAME:{Low Frequency Link Clock Source (LFLCS)} TYPE:{Combo} OPTION:{0: POR (Enable), 1: Enable (XTAL), 2: Disable (Audio PLL)}
  # !BSF HELP:{0: POR (Enable), 1: Enable (XTAL), 2: Disable (Audio PLL).}
  gPlatformFspPkgTokenSpaceGuid.PchHdaTestLowFreqLinkClkSrc   | * | 0x01 | 0x00

  # !BSF NAME:{Audio Sub System IDs} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Set default Audio Sub System IDs. If its set to 0 then value from Strap is used.}
  gPlatformFspPkgTokenSpaceGuid.PchHdaSubSystemIds           | * | 0x4 | 0x0

  # !BSF NAME:{SoundWire clock source select} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Select clock source for the SoundWire controllers. 0: XTAL, 1: Audio PLL.}
  gPlatformFspPkgTokenSpaceGuid.PchHdaSndwClockSourceSelect  | * | 0x1 | 0x0

  ## HDAUDIO_PREMEM_CONFIG End

  ## PCH_LPC_PREMEM_CONFIG Start

  # !BSF NAME:{PCH LPC Enhance the port 8xh decoding} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Original LPC only decodes one byte of port 80h.}
  gPlatformFspPkgTokenSpaceGuid.PchLpcEnhancePort8xhDecoding        | * | 0x01 | 0x01

  ## PCH_LPC_PREMEM_CONFIG End

  #
  # PCH_PCIE_RP_PREMEM_CONFIG Start
  #

  # !BSF NAME:{Usage type for ClkSrc} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{0-23: PCH rootport, 0x70:LAN, 0x80: unspecified but in use (free running), 0xFF: not used}
  gPlatformFspPkgTokenSpaceGuid.PcieClkSrcUsage             | * | 0x12 | { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }

  # !BSF NAME:{ClkReq-to-ClkSrc mapping} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Number of ClkReq signal assigned to ClkSrc}
  gPlatformFspPkgTokenSpaceGuid.PcieClkSrcClkReq            | * | 0x12 | { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, 0x01 }
  gPlatformFspPkgTokenSpaceGuid.PcieClkSrcClkReqRsvd        | * | 0x0E | {0x00}

  # !BSF NAME:{Clk Req GPIO Pin} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select Clk Req Pin. Refer to GPIO_*_MUXING_SRC_CLKREQ_x* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.PcieClkReqGpioMux     | * | 0x20 | { 0, 0, 0, 0, 0, 0, 0, 0 }

  # !BSF NAME:{Enable PCIE RP Mask} TYPE:{EditNum, HEX, (0x00,0x00FFFFFF)}
  # !BSF HELP:{Enable/disable PCIE Root Ports. 0: disable, 1: enable. One bit for each port, bit0 for port1, bit1 for port2, and so on.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpEnableMask            | * | 0x04 | 0x00FFFFFF

  ## PCH_PCIE_RP_PREMEM_CONFIG End

  #
  # Debug Pre-Mem Block Start
  #

  # !BSF PAGE:{DEBUG}
  # !BSF NAME:{Debug Interfaces} TYPE:{EditNum, HEX, (0x00,0x3F)}
  # !BSF HELP:{Debug Interfaces. BIT0-RAM, BIT1-UART, BIT3-USB3, BIT4-Serial IO, BIT5-TraceHub, BIT2 - Not used.}
  gPlatformFspPkgTokenSpaceGuid.PcdDebugInterfaceFlags                  | * | 0x01 | 0x32
  # Added reserved space UnusedUpdSpace12[3]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd12               | * | 0x03 | {0x00}

  # !BSF NAME:{Serial Io Uart Debug Mmio Base} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select SerialIo Uart default MMIO resource in SEC/PEI phase when PcdLpssUartMode = SerialIoUartPci.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDebugMmioBase           | * | 0x04 | 0xFE036000

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

  ## IPU_PREMEM_CONFIG Start

  # !BSF NAME:{IMGU CLKOUT Configuration}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{The configuration of IMGU CLKOUT, 0: Disable;<b>1: Enable</b>.}
  gPlatformFspPkgTokenSpaceGuid.ImguClkOutEn                | * | 0x06 | {0x01, 0x01, 0x01, 0x01, 0x00,0x00}

  # !BSF NAME:{Enable/Disable SA IPU} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable(Default): Enable SA IPU, Disable: Disable SA IPU}
  gPlatformFspPkgTokenSpaceGuid.SaIpuEnable                 | * | 0x01 | 0x1

  ## IPU_PREMEM_CONFIG End

  ## PCH_WDT_PREMEM_CONFIG Start

  # !BSF NAME:{Disable and Lock Watch Dog Register} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Set 1 to clear WDT status, then disable and lock WDT registers.}
  gPlatformFspPkgTokenSpaceGuid.WdtDisableAndLock           | * | 0x01 | 0x00
  # Added reserved space  UnusedUpdSpace13[2]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd13               | * | 0x02 | {0x00}

  ## PCH_WDT_PREMEM_CONFIG End

  # !BSF NAME:{HECI Timeouts} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0: Disable, 1: Enable (Default) timeout check for HECI}
  gPlatformFspPkgTokenSpaceGuid.HeciTimeouts                | * | 0x01 | 0x01

  # !BSF NAME:{HECI2 Interface Communication} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Test, 0: disable, 1: enable, Adds or Removes HECI2 Device from PCI space.}
  gPlatformFspPkgTokenSpaceGuid.HeciCommunication2          | * | 0x01 | 0x0

  # !BSF NAME:{Check HECI message before send} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Test, 0: disable, 1: enable, Enable/Disable message check.}
  gPlatformFspPkgTokenSpaceGuid.DisableMessageCheck         | * | 0x01 | 0x0

  # !BSF NAME:{Force ME DID Init Status} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Test, 0: disable, 1: Success, 2: No Memory in Channels, 3: Memory Init Error, Set ME DID init stat value}
  gPlatformFspPkgTokenSpaceGuid.DidInitStat                 | * | 0x01 | 0x0

  # !BSF NAME:{Enable KT device} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Test, 0: POR, 1: enable, 2: disable, Enable or Disable KT device.}
  gPlatformFspPkgTokenSpaceGuid.KtDeviceEnable              | * | 0x01 | 0x0

  # !BSF NAME:{CPU Replaced Polling Disable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Test, 0: disable, 1: enable, Setting this option disables CPU replacement polling loop}
  gPlatformFspPkgTokenSpaceGuid.DisableCpuReplacedPolling   | * | 0x01 | 0x0

  # !BSF NAME:{Skip CPU replacement check} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Test, 0: disable, 1: enable, Setting this option to skip CPU replacement check}
  gPlatformFspPkgTokenSpaceGuid.SkipCpuReplacementCheck     | * | 0x01 | 0x00

  # !BSF NAME:{Skip MBP HOB} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Test, 0: disable, 1: enable, Enable/Disable sending MBP message and creating MBP Hob.}
  gPlatformFspPkgTokenSpaceGuid.SkipMbpHob                  | * | 0x01 | 0x0

  ## ME_PEI_PREMEM_CONFIG End

  #
  # ME Pre-Mem Block Start
  #

  # !BSF NAME:{HECI Communication} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Test, 0: POR, 1: enable, 2: disable, Disables HECI communication causing ME to enter error state.}
  gPlatformFspPkgTokenSpaceGuid.HeciCommunication          | * | 0x01 | 0x0

  # !BSF NAME:{HECI3 Interface Communication} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Test, 0: POR, 1: enable, 2: disable, Adds or Removes HECI3 Device from PCI space.}
  gPlatformFspPkgTokenSpaceGuid.HeciCommunication3         | * | 0x01 | 0x0

  #
  # ME Pre-Mem Block End
  #

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
  # Added reserved space  UnusedUpdSpace14
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd14               | * | 0x01 | 0x00

  #
  # Debug Pre-Mem Block End
  #

  #
  # FspV Pre-Mem Test Config Start
  #

  # !BSF NAME:{FSPM Validation Pointer} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Point to FSPM Validation configuration structure}
  gPlatformFspPkgTokenSpaceGuid.FspmValidationPtr                      | * | 0x08 | 0x00

  #
  # FspV Pre-Mem Test Config End
  #

  ## SPI_INFO_HOB Start

  # !BSF NAME:{Extended BIOS Support} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Extended BIOS Region Support. Default is DISABLE. 0: DISABLE, 1: ENABLE}
  gPlatformFspPkgTokenSpaceGuid.ExtendedBiosDecodeRange     | * | 0x01 | 0x00

  ## SPI_INFO_HOB End

  ## SPI_CONFIG Start

  # !BSF NAME:{Extented BIOS Direct Read Decode enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable access to bigger than 16MB BIOS Region through Direct Memory Reads. 0: disabled (default), 1: enabled}
  gPlatformFspPkgTokenSpaceGuid.PchSpiExtendedBiosDecodeRangeEnable | * | 0x01 | 0x00
  # Added reserved space  UnusedUpdSpace15[2]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd15               | * | 0x02 | {0x00}

  # !BSF NAME:{Extended BIOS Direct Read Decode Range base} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Bits of 31:16 of a memory address that'll be a base for Extended BIOS Direct Read Decode.}
  gPlatformFspPkgTokenSpaceGuid.PchSpiExtendedBiosDecodeRangeBase | * | 0x04 | 0xF8000000

  # !BSF NAME:{Extended BIOS Direct Read Decode Range limit} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Bits of 31:16 of a memory address that'll be a limit for Extended BIOS Direct Read Decode.}
  gPlatformFspPkgTokenSpaceGuid.PchSpiExtendedBiosDecodeRangeLimit | * | 0x04 | 0xF9FFFFFF

  ## SPI_CONFIG End

  ## PCH_SMBUS_PREMEM_CONFIG Start

  # !BSF NAME:{Enable SMBus} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable SMBus controller.}
  gPlatformFspPkgTokenSpaceGuid.SmbusEnable                  | * | 0x01 | 0x01

  # !BSF NAME:{Enable SMBus ARP support} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable SMBus ARP support.}
  gPlatformFspPkgTokenSpaceGuid.SmbusArpEnable              | * | 0x01 | 0x00

  # !BSF NAME:{Number of RsvdSmbusAddressTable.} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{The number of elements in the RsvdSmbusAddressTable.}
  gPlatformFspPkgTokenSpaceGuid.PchNumRsvdSmbusAddresses    | * | 0x01 | 0x00
  # Added reserved space  UnusedUpdSpace16
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd16               | * | 0x01 | 0x00

  # !BSF NAME:{SMBUS Base Address} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{SMBUS Base Address (IO space).}
  gPlatformFspPkgTokenSpaceGuid.PchSmbusIoBase              | * | 0x02 | 0xEFA0

  # !BSF NAME:{Enable SMBus Alert Pin} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable SMBus Alert Pin.}
  gPlatformFspPkgTokenSpaceGuid.PchSmbAlertEnable           | * | 0x01 | 0x00
  # Added reserved space  UnusedUpdSpace17[5]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd17               | * | 0x05 | {0x00}

  # only need 4 bytes to use point for this policy
  # !BSF NAME:{Point of RsvdSmbusAddressTable} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Array of addresses reserved for non-ARP-capable SMBus devices.}
  gPlatformFspPkgTokenSpaceGuid.RsvdSmbusAddressTablePtr    | * | 0x08 | 0x00000000

  # !BSF NAME:{Smbus dynamic power gating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Disable or Enable Smbus dynamic power gating.}
  gPlatformFspPkgTokenSpaceGuid.SmbusDynamicPowerGating     | * | 0x01 | 0x01

  # !BSF NAME:{SMBUS SPD Write Disable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Set/Clear Smbus SPD Write Disable. 0: leave SPD Write Disable bit; 1: set SPD Write Disable bit. For security recommendations, SPD write disable bit must be set.}
  gPlatformFspPkgTokenSpaceGuid.SmbusSpdWriteDisable        | * | 0x01 | 0x01

  ## PCH_SMBUS_PREMEM_CONFIG End

  #
  # OC Pre-Mem Block Start
  #
  # !BSF PAGE:{OC}

  ## OVERCLOCKING_PREMEM_CONFIG Start

  # !BSF NAME:{Enable/Disable SA OcSupport} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable: Enable SA OcSupport, Disable(Default): Disable SA OcSupport}
  gPlatformFspPkgTokenSpaceGuid.SaOcSupport                 | * | 0x01 | 0x0

  # !BSF NAME:{VF Point Count}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Number of supported Voltage & Frequency Point Offset}
  gPlatformFspPkgTokenSpaceGuid.VfPointCount                     | * | 0x0A | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}


  # !BSF NAME:{ProcessVmaxLimit}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Disabling Process Vmax Limit will allow user to set any voltage}
  gPlatformFspPkgTokenSpaceGuid.ProcessVmaxLimit                     | * | 0x01 | 0x00

  # !BSF NAME:{CorePllCurrentRefTuningOffset}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Core PLL Current Reference Tuning Offset. <b>0: No offset</b>. Range 0-15}
  gPlatformFspPkgTokenSpaceGuid.CorePllCurrentRefTuningOffset                     | * | 0x01 | 0x00

  # !BSF NAME:{RingPllCurrentRefTuningOffset}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Ring PLL Current Reference Tuning Offset. <b>0: No offset</b>. Range 0-15}
  gPlatformFspPkgTokenSpaceGuid.RingPllCurrentRefTuningOffset                     | * | 0x01 | 0x00

  # !BSF NAME:{IaAtomPllCurrentRefTuningOffset}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{IaAtom PLL Current Reference Tuning Offset. <b>0: No offset</b>. Range 0-15}
  gPlatformFspPkgTokenSpaceGuid.IaAtomPllCurrentRefTuningOffset                     | * | 0x01 | 0x00

  # !BSF NAME:{CoreMinRatio}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{equest Core Min Ratio. Limit Core minimum ratio for extreme overclocking. Default 0 indicates no request}
  gPlatformFspPkgTokenSpaceGuid.CoreMinRatio                     | * | 0x01 | 0x00

  # !BSF NAME:{CoreMiNegativeTemperatureReportingnRatio}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Negative Temperature Reporting Enable. <b>0: Disable</b>, 1: enable}
  gPlatformFspPkgTokenSpaceGuid.NegativeTemperatureReporting                     | * | 0x01 | 0x00

  # !BSF NAME:{PcorePowerDensityThrottle}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Power Density Throttle control allows user to disable P-core}
  gPlatformFspPkgTokenSpaceGuid.PcorePowerDensityThrottle                     | * | 0x01 | 0x00

  # !BSF NAME:{EcorePowerDensityThrottle}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Power Density Throttle control allows user to disable P-core}
  gPlatformFspPkgTokenSpaceGuid.EcorePowerDensityThrottle                     | * | 0x01 | 0x00

  # !BSF NAME:{Over clocking support} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Over clocking support; <b>0: Disable</b>; 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.OcSupport                   | * | 0x01 | 0x00

  # !BSF NAME:{UnderVolt Protection} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When UnderVolt Protection is enabled, user will be not be able to program under voltage in OS runtime. 0: Disabled; <b>1: Enabled</b>}
  gPlatformFspPkgTokenSpaceGuid.UnderVoltProtection          | * | 0x01 | 0x01

  # !BSF NAME:{Realtime Memory Timing} TYPE:{Combo}
  # !BSF OPTION:{0: Disabled, 1: Enabled}
  # !BSF HELP:{0(Default): Disabled, 1: Enabled. When enabled, it will allow the system to perform realtime memory timing changes after MRC_DONE.}
  gPlatformFspPkgTokenSpaceGuid.RealtimeMemoryTiming        | * | 0x01 | 0x0

  # !BSF NAME:{core voltage override}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x7D0)}
  # !BSF HELP:{The core voltage override which is applied to the entire range of cpu core frequencies. Valid Range 0 to 2000}
  gPlatformFspPkgTokenSpaceGuid.CoreVoltageOverride         | * | 0x02 | 0x00

  # !BSF NAME:{Core Turbo voltage Offset}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x3E8)}
  # !BSF HELP:{The voltage offset applied to the core while operating in turbo mode.Valid Range 0 to 1000}
  gPlatformFspPkgTokenSpaceGuid.CoreVoltageOffset           | * | 0x02 | 0x00

  # !BSF NAME:{Core PLL voltage offset}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x0F)}
  # !BSF HELP:{Core PLL voltage offset. <b>0: No offset</b>. Range 0-15}
  gPlatformFspPkgTokenSpaceGuid.CorePllVoltageOffset        | * | 0x01 | 0x00

  # !BSF NAME:{AVX2 Ratio Offset} TYPE:{EditNum, HEX, (0x00, 0x1F)}
  # !BSF HELP:{0(Default)= No Offset. Range 0 - 31. Specifies number of bins to decrease AVX ratio vs. Core Ratio. Uses Mailbox MSR 0x150, cmd 0x1B.}
  gPlatformFspPkgTokenSpaceGuid.Avx2RatioOffset             | * | 0x1 | 0

  # !BSF NAME:{BCLK Adaptive Voltage Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When enabled, the CPU V/F curves are aware of BCLK frequency when calculated. </b>0: Disable;<b> 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.BclkAdaptiveVoltage     | * | 0x01 | 0x00

  # !BSF NAME:{Ring Downbin} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Ring Downbin enable/disable. When enabled, CPU will ensure the ring ratio is always lower than the core ratio.0: Disable; <b>1: Enable.</b>}
  gPlatformFspPkgTokenSpaceGuid.RingDownBin                 | * | 0x01 | 0x01

  # !BSF NAME:{Row Hammer pTRR LFSR0 Mask}
  # !BSF TYPE:{EditNum, HEX, (0x01, 0xF)}
  # !BSF HELP:{Row Hammer pTRR LFSR0 Mask, 1/2^(value)}
  gPlatformFspPkgTokenSpaceGuid.Lfsr0Mask         | * | 0x01 | 0xB

  # !BSF PAGE:{MRC}
  # !BSF NAME:{Margin Limit Check}
  # !BSF TYPE:{Combo} OPTION:{0:Disable, 1:L1, 2:L2, 3:Both}
  # !BSF HELP:{Margin Limit Check. Choose level of margin check}
  gPlatformFspPkgTokenSpaceGuid.MarginLimitCheck            | * | 0x01 | 0x00

  # !BSF NAME:{Row Hammer pTRR LFSR1 Mask}
  # !BSF TYPE:{EditNum, HEX, (0x01, 0xF)}
  # !BSF HELP:{Row Hammer pTRR LFSR1 Mask, 1/2^(value)}
  gPlatformFspPkgTokenSpaceGuid.Lfsr1Mask                   | * | 0x01 | 0xB

  # !BSF NAME:{Row Hammer DRAM Refresh Management Mode}
  # !BSF TYPE:{Combo} OPTION:{0: RFM, 1: ARFM Level A, 2: ARFM Level B, 3: ARFM Level C, 4: ARFM and RFM Disabled}
  # !BSF HELP:{Row Hammer Adaptive Refresh Management Level: 0-RFM (default), 1-ARFMLevel A, 2-ARFMLevel B, 3-ARFMLevel C, 4-Disable ARFM and RFM}
  gPlatformFspPkgTokenSpaceGuid.DramRfmMode         | * | 0x01 | 0x00

  # !BSF NAME:{Row Hammer Targeted Row Refresh Mode}
  # !BSF TYPE:{Combo} OPTION:{0: DRFM, 1: pTRR, 2: Targeted Row Refresh Disabled}
  # !BSF HELP:{Row Hammer Targeted Row Refresh: 0-DRFM, 1-pTRR (default), 2-Disable DRFM and pTRR}
  gPlatformFspPkgTokenSpaceGuid.TargetedRowRefreshMode         | * | 0x01 | 0x01

  # !BSF NAME:{TjMax Offset}
  # !BSF TYPE:{EditNum, HEX, (0x0A,0x3F)}
  # !BSF HELP:{TjMax offset.Specified value here is clipped by pCode (125 - TjMax Offset) to support TjMax in the range of 62 to 115 deg Celsius. Valid Range 10 - 63}
  gPlatformFspPkgTokenSpaceGuid.TjMaxOffset                 | * | 0x01 | 0x00

  # !BSF NAME:{Per-Atom-Cluster VF Offset} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Array used to specifies the selected Atom Core Cluster Offset Voltage. This voltage is specified in millivolts.}
  gPlatformFspPkgTokenSpaceGuid.PerAtomClusterVoltageOffset        | * | 0x10 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Per-Atom-Cluster VF Offset Prefix} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Sets the PerAtomCLusterVoltageOffset value as positive or negative for the selected Core; <b>0: Positive </b>; 1: Negative.}
  gPlatformFspPkgTokenSpaceGuid.PerAtomClusterVoltageOffsetPrefix  | * | 0x8 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Per-Atom-Cluster Voltage Mode} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT8}
  # !BSF HELP:{Array used to specifies the selected Atom Core ClusterVoltage Mode.}
  gPlatformFspPkgTokenSpaceGuid.PerAtomClusterVoltageMode          | * | 0x8 | { 0x0, 0x0, 0x0, 0x0, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Per-Atom-Cluster Voltage Override} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Array used to specifies the selected Atom Core Cluster Voltage Override.}
  gPlatformFspPkgTokenSpaceGuid.PerAtomClusterVoltageOverride      | * | 0x10 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Core VF Point Offset} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Array used to specifies the Core Voltage Offset applied to the each selected VF Point. This voltage is specified in millivolts.}
  gPlatformFspPkgTokenSpaceGuid.CoreVfPointOffset       | * | 0x1E | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Core VF Point Offset Prefix} TYPE:{Combo}  OPTION:{0:Positive, 1:Negative}
  # !BSF HELP:{Sets the CoreVfPointOffset value as positive or negative for corresponding core VF Point; <b>0: Positive </b>; 1: Negative.}
  gPlatformFspPkgTokenSpaceGuid.CoreVfPointOffsetPrefix | * | 0x0F | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Core VF Point Ratio} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Array for the each selected Core VF Point to display the ration.}
  gPlatformFspPkgTokenSpaceGuid.CoreVfPointRatio       | * | 0x0F | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Core VF Configuration Scope} TYPE:{Combo}  OPTION:{0:All-core, 1:Per-core}
  # !BSF HELP:{Alows both all-core VF curve or per-core VF curve configuration; <b>0: All-core</b>; 1: Per-core.}
  gPlatformFspPkgTokenSpaceGuid.CoreVfConfigScope           | * | 0x01 | 0x00
  # Added reserved space  UnusedUpdSpace18[1]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd18                | * | 0x01 | 0x00

  # !BSF NAME:{Per-core VF Offset} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Array used to specifies the selected Core Offset Voltage. This voltage is specified in millivolts.}
  gPlatformFspPkgTokenSpaceGuid.PerCoreVoltageOffset        | * | 0x10 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Per-core VF Offset Prefix} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Sets the PerCoreVoltageOffset value as positive or negative for the selected Core; <b>0: Positive </b>; 1: Negative.}
  gPlatformFspPkgTokenSpaceGuid.PerCoreVoltageOffsetPrefix  | * | 0x08 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Per Core Max Ratio override} TYPE:{Combo}  OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or disable Per Core PState OC supported by writing OCMB 0x1D to program new favored core ratio to each Core. <b>0: Disable</b>, 1: enable}
  gPlatformFspPkgTokenSpaceGuid.PerCoreRatioOverride        | * | 0x01 | 0x00

  # !BSF NAME:{Per-core Voltage Mode} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT8}
  # !BSF HELP:{Array used to specifies the selected Core Voltage Mode.}
  gPlatformFspPkgTokenSpaceGuid.PerCoreVoltageMode          | * | 0x8 | { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
  # Added reserved space  UnusedUpdSpace19[1]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd19               | * | 0x01 | 0x00

  # !BSF NAME:{Per-core Voltage Override} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Array used to specifies the selected Core Voltage Override.}
  gPlatformFspPkgTokenSpaceGuid.PerCoreVoltageOverride      | * | 0x10 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Per Core Current Max Ratio} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Array for the Per Core Max Ratio}
  gPlatformFspPkgTokenSpaceGuid.PerCoreRatio                | * | 0x08 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Atom Cluster Max Ratio} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Array for Atom Cluster Max Ratio, 4 ATOM cores are in the same Cluster and their max core ratio will be aligned.}
  gPlatformFspPkgTokenSpaceGuid.AtomClusterRatio            | * | 0x08 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Pvd Ratio Threshold for SOC/CPU die} TYPE:{EditNum, HEX, (0x0, 0x3F)}
  # !BSF HELP:{Array of Pvd Ratio Threshold for SOC/CPU die is the threshold value for input ratio (P0 to Pn) to select the multiplier so that the output is within the DCO frequency range. As per the die selected, this threshold is applied to SA and MC/CMI PLL for SOC die and SA, Ring and Atom PLL for CPU die. Range 0-63. When the threshold is 0, static PVD ratio is selected based on the PVD Mode for SOC. <b>0: Default</b>.}
  gPlatformFspPkgTokenSpaceGuid.PvdRatioThreshold           | * | 0x1 | 0x00

  # !BSF NAME:{Pvd Mode SOC/CPU die} TYPE:{EditNum, HEX, (0x0, 0x3)}
  # !BSF HELP:{Array of PVD Mode. Value from 0 to 3 for SOC/CPU. 0x0 = div-1 (VCO = Output clock), 0x1 = div-2 (VCO = 2x Output clock), 0x2 = div-4 (VCO = 4x Output clock), 0x3 = div-8 (VCO = 8x Output clock).}
  gPlatformFspPkgTokenSpaceGuid.PvdMode                     | * | 0x1 | 0x00

  # !BSF NAME:{FLL Overclock Mode} TYPE:{EditNum, HEX, (0x0, 0x3)}
  # !BSF HELP:{Select FLL Mode Value from 0 to 3. 0x0 = no overclocking, 0x1 = ratio overclocking with nominal (0.5-1x) reference clock frequency, 0x2 = BCLK overclocking with elevated (1-3x) reference clock frequency, 0x3 = BCLK overclocking with extreme elevated (3-5x) reference clock frequency and ratio limited to 63.}
  gPlatformFspPkgTokenSpaceGuid.FllOverclockMode            | * | 0x01 | 0x00
  # Added reserved space  UnusedUpdSpace20[1]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd20                 | * | 0x01 | 0x00

  # !BSF NAME:{Ring VF Point Offset} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Array used to specifies the Ring Voltage Offset applied to the each selected VF Point. This voltage is specified in millivolts.}
  gPlatformFspPkgTokenSpaceGuid.RingVfPointOffset       | * | 0x1E | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Ring VF Point Offset Prefix} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Sets the RingVfPointOffset value as positive or negative for corresponding core VF Point; <b>0: Positive </b>; 1: Negative.}
  gPlatformFspPkgTokenSpaceGuid.RingVfPointOffsetPrefix | * | 0x0F | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Ring VF Point Ratio} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Array for the each selected Ring VF Point to display the ration.}
  gPlatformFspPkgTokenSpaceGuid.RingVfPointRatio       | * | 0x0F | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Soc Die SSC enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Dsiable Soc-Die SSC Configuration. 0(Default)=Disable, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.SocDieSscEnable             | * | 0x01 | 0x00

  # !BSF NAME:{Core Operating Point Reporting}
  # !BSF TYPE:{Combo} OPTION:{0:Disable, 1:Enable}
  # !BSF HELP:{Enables Core Operating point reporting. <b>0: Disable</b>; 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.CoreOpPointReportingEn             | * | 0x01 | 0x00

  # !BSF NAME:{CPU BCLK OC Frequency} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{CPU BCLK OC Frequency in KHz units. 98000000Hz = 98MHz <b>0 - Auto</b>. Range is 40Mhz-1000Mhz.}
  gPlatformFspPkgTokenSpaceGuid.CpuBclkOcFrequency          | * | 0x4 | 0x00000000

  # !BSF NAME:{SOC BCLK OC Frequency} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{SOC BCLK OC Frequency in KHz units. 98000000Hz = 98MHz <b>0 - Auto</b>. Range is 40Mhz-1000Mhz.}
  gPlatformFspPkgTokenSpaceGuid.SocBclkOcFrequency          | * | 0x4 | 0x00000000

  # !BSF NAME:{Bitmask of disable cores} TYPE:{EditNum, HEX, (0x0, 0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Core mask is a bitwise indication of which core should be disabled. <b>0x00=Default</b>; Bit 0 - core 0, bit 7 - core 7.}
  gPlatformFspPkgTokenSpaceGuid.DisablePerCoreMask          | * | 0x08 | 0x00

  # !BSF NAME:{Granular Ratio Override} TYPE:{Combo}  OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or disable OC Granular Ratio Override. <b>0: Disable</b>, 1: enable}
  gPlatformFspPkgTokenSpaceGuid.GranularRatioOverride        | * | 0x01 | 0x00

  # !BSF NAME:{Avx2 Voltage Guardband Scaling Factor}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xC8)}
  # !BSF HELP:{AVX2 Voltage Guardband Scale factor applied to AVX2 workloads. Range is 0-200 in 1/100 units, where a value of 125 would apply a 1.25 scale factor.}
  gPlatformFspPkgTokenSpaceGuid.Avx2VoltageScaleFactor      | * | 0x01 | 0x00

  # !BSF NAME:{Ring PLL voltage offset}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x0F)}
  # !BSF HELP:{Core PLL voltage offset. <b>0: No offset</b>. Range 0-15}
  gPlatformFspPkgTokenSpaceGuid.RingPllVoltageOffset        | * | 0x01 | 0x00

  ## OVERCLOCKING_PREMEM_CONFIG End

  # !BSF NAME:{OcPreMemRsvd} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Reserved for OC Pre-Mem}
  gPlatformFspPkgTokenSpaceGuid.OcPreMemRsvd               | * | 0x05 | {0x00}

  #
  # OC Pre-Mem Block End
  #

  ## ISH_PREMEM_CONFIG Start

  # !BSF NAME:{Enable PCH ISH Controller} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0: Disable, 1: Enable (Default) ISH Controller}
  gPlatformFspPkgTokenSpaceGuid.PchIshEnable                | * | 0x01 | 0x01
  # Added reserved space  UnusedUpdSpace21[1]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd21                 | * | 0x01 | 0x00

  ## ISH_PREMEM_CONFIG End

  #
  # Security Pre-Mem Block Start
  #
  # !BSF PAGE:{SECURITY1}

  ## CPU_SECURITY_PREMEM_CONFIG Start

  # !BSF NAME:{BiosSize}  TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{The size of the BIOS region of the IFWI. Used if FspmUpd->FspmConfig.BiosGuard != 0. If BiosGuard is enabled, MRC will increase the size of the DPR (DMA Protected Range) so that a BIOS Update Script can be stored in the DPR.}
  gPlatformFspPkgTokenSpaceGuid.BiosSize                     | * | 0x02 | 0x2800

  # !BSF NAME:{BiosGuard} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable. 0: Disable, Enable/Disable BIOS Guard feature, 1: enable}
  gPlatformFspPkgTokenSpaceGuid.BiosGuard                   | * | 0x01 | 0x00

  # !BSF NAME :{BiosGuardToolsInterface} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable. 0: Disable, Enable/Disable BiosGuardToolsInterface feature, 1: enable}
  gPlatformFspPkgTokenSpaceGuid.BiosGuardToolsInterface     | * | 0x01 | 0x01

  # !BSF NAME:{Txt} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables utilization of additional hardware capabilities provided by Intel (R) Trusted Execution Technology. Changes require a full power cycle to take effect. <b>0: Disable</b>, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.Txt                         | * | 0x01 | 0x00

  # !BSF NAME:{Skip Stop PBET Timer Enable/Disable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Skip Stop PBET Timer; <b>0: Disable</b>; 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.SkipStopPbet                | * | 0x01 | 0x00

  # !BSF NAME:{Reset Auxiliary content} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Reset Auxiliary content, <b>0: Disabled</b>, 1: Enabled}
  gPlatformFspPkgTokenSpaceGuid.ResetAux                   | * | 0x01 | 0x00

  # !BSF NAME:{TseEnable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable. 0: Disable, Enable/Disable Tse feature, 1: enable}
  gPlatformFspPkgTokenSpaceGuid.TseEnable                   | * | 0x01 | 0x00

  # !BSF NAME:{Enable or Disable TDX} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Configure Trust Domain Extension (TDX) to isolate VMs from Virtual-Machine Manager (VMM)/hypervisor <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.TdxEnable                   | * | 0x01 | 0x00

  # Generate new TME key regardless of the type of reset.
  #- <b>0: do not generate</b> -    1: generate
  # !BSF NAME:{MKTME Key-Id Bits Override Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:MKTME Key-Id Bits Override Enable Enable; 0: disable, 1
  gPlatformFspPkgTokenSpaceGuid.GenerateNewTmeKey            | * | 0x01 | 0x00
  # Added reserved space  UnusedUpdSpace22[4]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd22                 | * | 0x4 | {0x00}

  # !BSF NAME:{TME Exclude Base Address} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{TME Exclude Base Address.}
  gPlatformFspPkgTokenSpaceGuid.TmeExcludeBase    | * | 0x08 | 0x0

  # !BSF NAME:{TME Exclude Size Value} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{TME Exclude Size Value.}
  gPlatformFspPkgTokenSpaceGuid.TmeExcludeSize    | * | 0x08 | 0x0

  # !BSF NAME:{TdxActmModuleAddr}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Base address of Tdx Actm module, used for launching the Actm}
  gPlatformFspPkgTokenSpaceGuid.TdxActmModuleAddr                   | * | 0x08 | 0x0000000000000000

  # !BSF NAME:{TdxActmModuleSize}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{size of Tdx Actm module, used for launching the Actm}
  gPlatformFspPkgTokenSpaceGuid.TdxActmModuleSize                   | * | 0x04 | 0x00000000

  # !BSF NAME:{TdxSeamldrSvn} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{TdxSeamldrSvn}
  gPlatformFspPkgTokenSpaceGuid.TdxSeamldrSvn                 | * | 0x01 | 0x0

  ## CPU_SECURITY_PREMEM_CONFIG End

  #
  # Security Pre-Mem Block End
  #

  #
  # CPU Pre-Mem Block Start
  #
  # !BSF PAGE:{CPU1}

  ## CPU_INIT_PREMEM_CONFIG Start
  # !BSF NAME:{Boot max frequency}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable Boot Maximum Frequency in CPU strap. 0: Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.BootMaxFrequency            | * | 0x01 | 0x01

  # !BSF NAME:{BIST on Reset} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable BIST (Built-In Self Test) on reset. <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.BistOnReset                 | * | 0x01 | 0x00

  # !BSF NAME:{Reduce XeCores} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Reduce XeCores. <b>0: Disable(strap=1) </b>; 1: Enable(strap=0.}

  gPlatformFspPkgTokenSpaceGuid.ReduceXecores                 | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable VMX} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable VMX, When enabled, a VMM can utilize the additional hardware capabilities provided by Vanderpool Technology. 0: Disable; <b>1: Enable</b>.}
  gPlatformFspPkgTokenSpaceGuid.VmxEnable                   | * | 0x01 | 0x01

  # !BSF NAME:{Processor Early Power On Configuration FCLK setting}
  # !BSF TYPE:{Combo} OPTION:{0:800 MHz, 1: 1 GHz, 2: 400 MHz, 3: Reserved }
  # !BSF HELP:{FCLK frequency can take values of 400MHz, 800MHz and 1GHz. <b>0: 800 MHz (ULT/ULX)</b>. <b>1: 1 GHz (DT/Halo)</b>. Not supported on ULT/ULX.- 2: 400 MHz. - 3: Reserved}
  gPlatformFspPkgTokenSpaceGuid.FClkFrequency               | * | 0x01 | 0x00

  # !BSF NAME:{Enable CPU CrashLog} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable CPU CrashLog; 0: Disable; <b>1: Enable</b>.}
  gPlatformFspPkgTokenSpaceGuid.CpuCrashLogEnable           | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable TME} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Configure Total Memory Encryption (TME) to protect DRAM data from physical attacks. <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.TmeEnable                   | * | 0x01 | 0x00

  # !BSF NAME:{CPU Run Control} TYPE:{Combo} OPTION:{0:Disabled, 1:Enabled, 2:No Change}
  # !BSF HELP:{Enable, Disable or Do not configure CPU Run Control; 0: Disable; 1: Enable ; <b>2: No Change</b>}
  gPlatformFspPkgTokenSpaceGuid.DebugInterfaceEnable        | * | 0x01 | 0x02

  # !BSF NAME:{CPU Run Control Lock} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Lock or Unlock CPU Run Control; 0: Disable; <b>1: Enable</b>.}
  gPlatformFspPkgTokenSpaceGuid.DebugInterfaceLockEnable    | * | 0x01 | 0x01

  # !BSF NAME:{Enable CPU CrashLog GPRs dump} TYPE:{Combo} OPTION:{0:Disabled, 1:Enabled, 2:Only Smm GPRs Disabled}
  # !BSF HELP:{Enable or Disable CPU CrashLog GPRs dump; <b>0: Disable</b>; 1: Enable; 2: Only disable Smm GPRs dump}
  gPlatformFspPkgTokenSpaceGuid.CrashLogGprs             | * | 0x01 | 0x00

  # !BSF NAME:{Over clocking Lock} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Lock Overclocking. 0: Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.OcLock                      | * | 0x01 | 0x01

  # !BSF NAME:{CPU ratio value}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x3F)}
  # !BSF HELP:{This value must be between Max Efficiency Ratio (LFM) and Maximum non-turbo ratio set by Hardware (HFM). Valid Range 0 to 63.}
  gPlatformFspPkgTokenSpaceGuid.CpuRatio                    | * | 0x01 | 0x1C

  # !BSF NAME:{Number of active big cores}
  # !BSF TYPE:{Combo} OPTION:{0:Disable all big cores, 1:1, 2:2, 3:3, 0xFF:Active all big cores}
  # !BSF HELP:{Number of P-cores to enable in each processor package. Note: Number of P-Cores and E-Cores are looked at together. When both are {0,0}, Pcode will enable all cores. Default 0xFF means to active all system supported big cores. <b>0xFF: Active all big cores</b>; 0: Disable all big cores; 1: 1; 2: 2; 3: 3;}
  gPlatformFspPkgTokenSpaceGuid.ActiveCoreCount             | * | 0x01 | 0xFF

  # !BSF NAME:{Number of active small cores}
  # !BSF TYPE:{Combo} OPTION:{0:Disable all small cores, 1:1, 2:2, 3:3, 0xFF:Active all small cores}
  # !BSF HELP:{Number of E-cores to enable in each processor package. Note: Number of P-Cores and E-Cores are looked at together. When both are {0,0}, Pcode will enable all cores. Default 0xFF means to active all system supported small cores. <b>0xFF: Active all small cores</b>; 0: Disable all small cores; 1: 1; 2: 2; 3: 3;}
  gPlatformFspPkgTokenSpaceGuid.ActiveSmallCoreCount        | * | 0x01 | 0xFF

  # !BSF NAME:{Number of LP Atom cores}
  # !BSF TYPE:{Combo} OPTION:{0:Disable all LP Atom cores, 1:1, 2:2, 0xFF:Active all cores}
  # !BSF HELP:{Number of LP E-cores to enable in LP. 0: Disable all LP Atom cores; 1: 1; 2: 2; <b>0xFF: Active all LP Atom cores</b>}
  gPlatformFspPkgTokenSpaceGuid.ActiveLpAtomCoreCount | * | 0x01 | 0xFF

  # !BSF NAME:{DFD Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable DFD. <b>0: Disable</b>, 1:Enable}
  gPlatformFspPkgTokenSpaceGuid.DfdEnable                   | * | 0x01 | 0x00
  # Added reserved space  UnusedUpdSpace23[3]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd23               | * | 0x3 | {0x00}

  # !BSF NAME:{PrmrrSize}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Enable/Disable. 0: Disable, define default value of PrmrrSize , 1: enable}
  # Supported values: 32MB: 0x2000000, 64MB: 0x4000000, 128 MB: 0x8000000, 256 MB: 0x10000000, 512 MB: 0x20000000
  gPlatformFspPkgTokenSpaceGuid.PrmrrSize                   | * | 0x04 | 0x00000000

  # !BSF NAME:{Tseg Size} TYPE:{Combo}
  # !BSF OPTION:{0x0400000:4MB, 0x01000000:16MB}
  # !BSF HELP:{Size of SMRAM memory reserved. 0x400000 for Release build and 0x1000000 for Debug build}
    !if $(TARGET) == DEBUG
      gPlatformFspPkgTokenSpaceGuid.TsegSize                    | * | 0x04 | 0x01000000
    !else
      gPlatformFspPkgTokenSpaceGuid.TsegSize                    | * | 0x04 | 0x00400000
    !endif

  # !BSF NAME:{SmmRelocationEnable Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable SmmRelocationEnable. <b>0: Disable</b>, 1:Enable}
  gPlatformFspPkgTokenSpaceGuid.SmmRelocationEnable                   | * | 0x01 | 0x01

  ## CPU_INIT_PREMEM_CONFIG End

  # !BSF NAME:{TCC Activation Offset}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{TCC Activation Offset. Offset from factory set TCC activation temperature at which the Thermal Control Circuit must be activated. TCC will be activated at TCC Activation Temperature, in volts. <b>Default = 0h</b>.}
  gPlatformFspPkgTokenSpaceGuid.TccActivationOffset         | * | 0x01 | 0x00
  # Added reserved space  UnusedUpdSpace24[2]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd24                 | * | 0x2 | {0x00}

  ## CPU_POWER_DELIVERY_CONFIG Start

  # !BSF NAME:{Platform PL1 power}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x7FFF)}
  # !BSF HELP:{Platform Power Limit 1 Power in Milli Watts. BIOS will round to the nearest 1/8W when programming. Value set 120 = 15W. Any value can be programmed between Max and Min Power Limits. This setting will act as the new PL1 value for the Package RAPL algorithm. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit. Valid Range 0 to 32767.}
  gPlatformFspPkgTokenSpaceGuid.PsysPowerLimit1Power        | * | 0x04 | 0x00

  # !BSF NAME:{PlatformAtxTelemetryUnit Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable PlatformAtxTelemetryUnit Mode.  <b>0: Disable</b> ; 1:Enable}
  gPlatformFspPkgTokenSpaceGuid.PlatformAtxTelemetryUnit   | * | 0x04 | 0x00

  # !BSF NAME:{Platform PL1 power}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x7FFF)}
  # !BSF HELP:{ Short term Power Limit value for custom cTDP level. Units are 125 milliwatt.}
  gPlatformFspPkgTokenSpaceGuid.CustomPowerLimit1      | * | 0x02 | 0x00

  # !BSF NAME:{Platform PL1 power}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x7FFF)}
  # !BSF HELP:{ Short term Power Limit value for custom cTDP level. Units are 125 milliwatt.}
  gPlatformFspPkgTokenSpaceGuid.CustomPowerLimit2      | * | 0x02 | 0x00

  # !BSF NAME:{Platform PL2 power}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x7FFF)}
  # !BSF HELP:{Platform Power Limit 2 Power in Milli Watts. BIOS will round to the nearest 1/8W when programming. Value set 120 = 15W. Any value can be programmed between Max and Min Power Limits. This setting will act as the new Max Turbo Power (PL2) value for the Package RAPL algorithm. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit. Valid Range 0 to 32767.}
  gPlatformFspPkgTokenSpaceGuid.PsysPowerLimit2Power        | * | 0x04 | 0x00

  # !BSF NAME:{Vsys Max System battery volatge}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{Vsys Max defined in 1/1000 increments. Range is 0-65535. For a 1.25 voltage, enter 1250. <b>Default =0xFF</b>.}
  gPlatformFspPkgTokenSpaceGuid.VsysMax                     | * | 0x02 | 0x00

  # !BSF NAME:{ThETA Ibatt Feature} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable ThETA Ibatt Feature. <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.ThETAIbattEnable         | * | 0x01 | 0x00
  # Added reserved space  UnusedUpdSpace25
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd25                 | * | 0x1 | 0x00

  # !BSF NAME:{ISYS Current Limit L1}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{This field indicated the current limitiation of L1. Indicate current limit for which dependency is on AC/DC mode before PSYS.Units of measurements are 1/8 A}
  gPlatformFspPkgTokenSpaceGuid.IsysCurrentLimitL1          | * | 0x02 | 0x0

  # !BSF NAME:{ISYS Current Limit L1 Tau}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{This Specifies the time window used to calculate average current for ISYS_L1. The units of measuremnts are specified in PACKAGE_POWER_SKU[TIME_UNIT]}
  gPlatformFspPkgTokenSpaceGuid.IsysCurrentL1Tau            | * | 0x01 | 0x00
  # Added reserved space  UnusedUpdSpace26
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd26                 | * | 0x1 | 0x00

  # !BSF NAME:{ISYS Current Limit L2}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{This bits enables disables ISYS_CURRENT_LIMIT_L2 algorithm.Indicate current limit for which dependency is on AC/DC mode before PSYS. Units of measurements are 1/8 A}
  gPlatformFspPkgTokenSpaceGuid.IsysCurrentLimitL2          | * | 0x02 | 0x00

  # !BSF NAME:{ISYS Current Limit L1 Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{This bits enables disables ISYS_CURRENT_LIMIT_L1 algorithm. It control loop based on the system power source AC or DC mode. <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.IsysCurrentLimitL1Enable    | * | 0x01 | 0x00

  # !BSF NAME:{ISYS Current Limit L2 Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{This bits enables disables ISYS_CURRENT_LIMIT_L2 algorithm. It control loop based on the system power source AC or DC mode. <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.IsysCurrentLimitL2Enable    | * | 0x01 | 0x00

  # !BSF NAME:{Package PL4 boost configuration}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xF618)}
  # !BSF HELP:{Configure Power Limit 4 Boost in Watts. Valid Range 0 to 63000 in step size of 125 mWatt. The value 0 means disable.}
  gPlatformFspPkgTokenSpaceGuid.PowerLimit4Boost            | * | 0x02 | 0x0

  # !BSF NAME:{Skin Temperature Target}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFFFFFF)}
  # !BSF HELP:{Target temperature is limit to which the control mechanism is regulating.It is defined in 1/2 C increments.Range is 0-255. Temperature Range is 0-122.5 C.<b>0: Auto</b>.}
  gPlatformFspPkgTokenSpaceGuid.SkinTargetTemp                      | * | 0x03 | {0x0, 0x0, 0x0}

  # !BSF NAME:{Skin Control Temperature Enable MMIO} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables the skin temperature control for MMIO register. <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.SkinTempControlEnable               | * | 0x03 | {0x0, 0x0, 0x0}

  # !BSF NAME:{Skin Temperature Loop Gain}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x777)}
  # !BSF HELP:{Sets the aggressiveness of control loop where 0 is graceful, favors performance on expense of temperature overshoots and 7 is for aggressive, favors tight regulation over performance. Range is 0-7.<b>0: Auto</b>.}
  gPlatformFspPkgTokenSpaceGuid.SkinControlLoopGain                 | * | 0x03 | {0x0,0x0,0x0}

  # !BSF NAME:{Skin Temperature Override Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When set, Pcode will use TEMPERATURE_OVERRIDE values instead of reading from corresponding sensor.. <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.SkinTempOverrideEnable              | * | 0x03 | {0x0, 0x0, 0x0}

  # !BSF NAME:{Skin Temperature Minimum Performance Level}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFFFFFF)}
  # !BSF HELP:{Minimum Performance level below which the STC limit will not throttle. 0 - all levels of throttling allowed incl. survivability actions. 256 - no throttling allowed.<b>0: Auto</b>.}
  gPlatformFspPkgTokenSpaceGuid.SkinMinPerformanceLevel             | * | 0x03 | {0x0,0x0,0x0}

  # !BSF NAME:{Skin Temperature Override}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFFFFFF)}
  # !BSF HELP:{Allows SW to override the input temperature. Pcode will use this value instead of the sensor temperature. EC control is not impacted. Units: 0.5C. Values are 0 to 255 which represents 0C-122.5C range.<b>0: Auto</b>.}
  gPlatformFspPkgTokenSpaceGuid.SkinTempOverride                    | * | 0x03 | {0x0,0x0,0x0}

  # !BSF NAME:{Skin Temperature Control Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables Skin Temperature Control Sensors Feature. <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.SkinTempControl                     | * | 0x01 | 0x0

  # !BSF NAME:{AC or DC Power State} TYPE:{Combo} OPTION:{0:DC, 1:AC}
  # !BSF HELP:{AC or DC power State; <b>0: DC</b>; 1: AC}
  gPlatformFspPkgTokenSpaceGuid.AcDcPowerState        | * | 0x01 | 0x00


  # !BSF NAME:{Enable or Disable VR Thermal Alert} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable VR Thermal Alert; <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.DisableVrThermalAlert       | * | 0x01 | 0x00

  # !BSF NAME:{Enable or Disable Thermal Monitor} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable Thermal Monitor; 0: Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.ThermalMonitor              | * | 0x01 | 0x01

  # !BSF NAME:{Configuration for boot TDP selection}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Assured Power (cTDP) Mode as Nominal/Level1/Level2/Deactivate Base Power (TDP) selection. Deactivate option will set MSR to Nominal and MMIO to Zero. <b>0: Base Power (TDP) Nominal</b>; 1: Base Power (TDP) Down; 2: Base Power (TDP) Up;0xFF : Deactivate}
  gPlatformFspPkgTokenSpaceGuid.ConfigTdpLevel              | * | 0x01 | 0x00

  # !BSF NAME:{ConfigTdp mode settings Lock} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Assured Power (cTDP) Mode Lock sets the Lock bits on TURBO_ACTIVATION_RATIO and CONFIG_TDP_CONTROL. Note: When CTDP (Assured Power) Lock is enabled Custom ConfigTDP Count will be forced to 1 and Custom ConfigTDP Boot Index will be forced to 0. <b>0: Disable</b>; 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.ConfigTdpLock               | * | 0x01 | 0x00

  # !BSF NAME:{Load Configurable TDP SSDT} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables Assured Power (cTDP) control via runtime ACPI BIOS methods. This 'BIOS only' feature does not require EC or driver support. <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.ConfigTdpBios               | * | 0x01 | 0x00

  # !BSF NAME:{CustomTurboActivationRatio} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Turbo Activation Ratio for custom cTDP level}
  gPlatformFspPkgTokenSpaceGuid.CustomTurboActivationRatio   | * | 0x01 | 0x00

  # !BSF NAME:{CustomPowerLimit1Time} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Short term Power Limit time window value for custom cTDP level.}
  gPlatformFspPkgTokenSpaceGuid.CustomPowerLimit1Time        | * | 0x01 | 0x50

  # !BSF NAME:{PL1 Enable value} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Platform Power Limit 1 programming. If this option is enabled, it activates the PL1 value to be used by the processor to limit the average power of given time window. <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PsysPowerLimit1             | * | 0x01 | 0x00

  # !BSF NAME:{PL1 timewindow}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x80)}
  # !BSF HELP:{Platform Power Limit 1 Time Window value in seconds. The value may vary from 0 to 128. 0 = default values. Indicates the time window over which Platform Processor Base Power (TDP) value should be maintained. Valid values(Unit in seconds) 0 to 8 , 10 , 12 ,14 , 16 , 20 , 24 , 28 , 32 , 40 , 48 , 56 , 64 , 80 , 96 , 112 , 128}
  gPlatformFspPkgTokenSpaceGuid.PsysPowerLimit1Time         | * | 0x01 | 0x00

  # !BSF NAME:{PL2 Enable Value} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Platform Power Limit 2 programming. If this option is disabled, BIOS will program the default values for Platform Power Limit 2. <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PsysPowerLimit2             | * | 0x01 | 0x00

  # !BSF NAME:{Package PL3 time window}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x40)}
  # !BSF HELP:{Power Limit 3 Time Window value in Milli seconds. Indicates the time window over which Power Limit 3 value should be maintained. If the value is 0, BIOS leaves the hardware default value. Valid value: <b>0</b>, 3-8, 10, 12, 14, 16, 20, 24, 28, 32, 40, 48, 56, 64.}
  gPlatformFspPkgTokenSpaceGuid.PowerLimit3Time             | * | 0x01 | 0x00

  # !BSF NAME:{Package Long duration turbo mode time}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x80)}
  # !BSF HELP:{Power Limit 1 Time Window value in seconds. The value may vary from 0 to 128. 0 = default value (28 sec for Mobile and 8 sec for Desktop). Defines time window which Processor Base Power (TDP) value should be maintained. Valid values(Unit in seconds) 0 to 8 , 10 , 12 ,14 , 16 , 20 , 24 , 28 , 32 , 40 , 48 , 56 , 64 , 80 , 96 , 112 , 128}
  gPlatformFspPkgTokenSpaceGuid.PowerLimit1Time             | * | 0x01 | 0x00
  # Added reserved space  UnusedUpdSpace27[2]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd27                 | * | 0x2 | {0x00}

  # !BSF NAME:{Package Long duration turbo mode power limit}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x7FFF)}
  # !BSF HELP:{Power Limit 1 in Milli Watts. BIOS will round to the nearest 1/8W when programming. Value set 120 = 15W. 0 = no custom override. Overclocking SKU: Value must be between Max and Min Power Limits. Other SKUs: This value must be between Min Power Limit and Processor Base Power (TDP) Limit. If value is 0, BIOS will program Processor Base Power (TDP) value. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit. Valid Range 0 to 32767.}
  gPlatformFspPkgTokenSpaceGuid.PowerLimit1                 | * | 0x04 | 0x0

  # !BSF NAME:{Package Short duration turbo mode power limit}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x7FFF)}
  # !BSF HELP:{Power Limit 2 in Milli Watts. BIOS will round to the nearest 1/8W when programming. Value set 120 = 15W. If the value is 0, BIOS will program this value as 1.25*Processor Base Power (TDP). Processor applies control policies such that the package power does not exceed this limit. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit. Valid Range 0 to 32767.}
  gPlatformFspPkgTokenSpaceGuid.PowerLimit2Power            | * | 0x04 | 0x0

  # !BSF NAME:{Package PL3 power limit}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x7FFF)}
  # !BSF HELP:{Power Limit 3 in Milli Watts. BIOS will round to the nearest 1/8W when programming. Value set 120 = 15W. XE SKU: Any value can be programmed. Overclocking SKU: Value must be between Max and Min Power Limits. Other SKUs: This value must be between Min Power Limit and Processor Base Power (TDP) Limit. If the value is 0, BIOS leaves the hardware default value. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit. Valid Range 0 to 32767.}
  gPlatformFspPkgTokenSpaceGuid.PowerLimit3                 | * | 0x04 | 0x0

  # !BSF NAME:{Package PL4 power limit}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x7FFF)}
  # !BSF HELP:{Power Limit 4 in Milli Watts. BIOS will round to the nearest 1/8W when programming. Value set 120 = 15W. If the value is 0, BIOS leaves default value. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit. Valid Range 0 to 32767.}
  gPlatformFspPkgTokenSpaceGuid.PowerLimit4                 | * | 0x04 | 0x0

  # !BSF NAME:{Short term Power Limit value for custom cTDP level 1}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x7FFF)}
  # !BSF HELP:{Power Limit 1 in Milli Watts. BIOS will round to the nearest 1/8W when programming. Value set 120 = 15W. 0 = no custom override. Overclocking SKU: Value must be between Max and Min Power Limits. Other SKUs: This value must be between Min Power Limit and Processor Base Power (TDP) Limit. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit. Valid Range 0 to 32767.}
  gPlatformFspPkgTokenSpaceGuid.Custom1PowerLimit1          | * | 0x04 | 0x0

  # !BSF NAME:{Long term Power Limit value for custom cTDP level 1}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x7FFF)}
  # !BSF HELP:{Power Limit 2 value in Milli Watts. BIOS will round to the nearest 1/8W when programming. Value set 120 = 15W. 0 = no custom override. Processor applies control policies such that the package power does not exceed this limit. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit. Valid Range 0 to 32767.}
  gPlatformFspPkgTokenSpaceGuid.Custom1PowerLimit2          | * | 0x04 | 0x0

  # !BSF NAME:{Enable Configurable TDP} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Applies Assured Power (cTDP) initialization settings based on non-Assured Power (cTDP) or Assured Power (cTDP). Default is 1: Applies to Assured Power (cTDP) ; if 0 then applies non-Assured Power (cTDP) and BIOS will bypass Assured Power (cTDP)  initialzation flow}
  gPlatformFspPkgTokenSpaceGuid.ApplyConfigTdp             | * | 0x01 | 0x01

  # !BSF NAME:{Dual Tau Boost} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable Dual Tau Boost feature. This is only applicable for Desktop 35W/65W/125W sku. When DPTF is enabled this feature is ignored. <b>0: Disable</b>; 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.DualTauBoost                | * | 0x01 | 0x00

  # !BSF NAME:{Tcc Offset Lock} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Tcc Offset Lock for Runtime Average Temperature Limit (RATL) to lock temperature target; <b>1:Enabled </b>; 0: Disabled.}
  gPlatformFspPkgTokenSpaceGuid.TccOffsetLock               | * | 0x01 | 0x01

  # !BSF NAME:{Package PL3 Duty Cycle}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x64)}
  # !BSF HELP:{Specify the duty cycle in percentage that the CPU is required to maintain over the configured time window. Range is 0-100.}
  gPlatformFspPkgTokenSpaceGuid.PowerLimit3DutyCycle        | * | 0x01 | 0x00

  # !BSF NAME:{Package PL3 Lock} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Power Limit 3 Lock. When enabled PL3 configurations are locked during OS. When disabled PL3 configuration can be changed during OS. <b>0: Disable</b> ; 1:Enable}
  gPlatformFspPkgTokenSpaceGuid.PowerLimit3Lock             | * | 0x01 | 0x00

  # !BSF NAME:{Package PL4 Lock} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Power Limit 4 Lock. When enabled PL4 configurations are locked during OS. When disabled PL4 configuration can be changed during OS. <b>0: Disable</b> ; 1:Enable}
  gPlatformFspPkgTokenSpaceGuid.PowerLimit4Lock             | * | 0x01 | 0x00

  # !BSF NAME:{Short Duration Turbo Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Power Limit 2 override. If this option is disabled, BIOS will program the default values for Power Limit 2. 0: Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.PowerLimit2                 | * | 0x01 | 0x01

  # !BSF NAME:{Response Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Response Mode.  <b>0: Disable</b> ; 1:Enable}
  gPlatformFspPkgTokenSpaceGuid.ResponseMode                 | * | 0x01 | 0x00

  ## CPU_POWER_DELIVERY_CONFIG End

  # CPU Pre-Mem Block End

  ## TXT_PREMEM_CONFIG Start

  # !BSF NAME:{SinitMemorySize}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Enable/Disable. 0: Disable, define default value of SinitMemorySize , 1: enable}
  gPlatformFspPkgTokenSpaceGuid.SinitMemorySize             | * | 0x04 | 0x50000

  # !BSF NAME:{TxtDprMemoryBase}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable. 0: Disable, define default value of TxtDprMemoryBase , 1: enable}
  # TODO: Check if default 0 is ok
  gPlatformFspPkgTokenSpaceGuid.TxtDprMemoryBase            | * | 0x08 | 0x0000000000000000

  # !BSF NAME:{TxtHeapMemorySize}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Enable/Disable. 0: Disable, define default value of TxtHeapMemorySize , 1: enable}
  # TODO: Check if default 0 is ok
  gPlatformFspPkgTokenSpaceGuid.TxtHeapMemorySize           | * | 0x04 | 0x00000000

  # !BSF NAME:{TxtDprMemorySize}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Reserve DPR memory size (0-255) MB. 0: Disable, define default value of TxtDprMemorySize , 1: enable}
  # TODO: Check if default 0 is ok
  gPlatformFspPkgTokenSpaceGuid.TxtDprMemorySize            | * | 0x04 | 0x00000000

  # !BSF NAME:{TxtLcpPdBase}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable. 0: Disable, define default value of TxtLcpPdBase , 1: enable}
  #TODO: Check if default 0 is ok
  gPlatformFspPkgTokenSpaceGuid.TxtLcpPdBase                | * | 0x08 | 0x0000000000000000

  # !BSF NAME:{TxtLcpPdSize}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable. 0: Disable, define default value of TxtLcpPdSize , 1: enable}
  #TODO: Check if default 0 is ok
  gPlatformFspPkgTokenSpaceGuid.TxtLcpPdSize                | * | 0x08 | 0x0000000000000000

  # !BSF NAME:{BiosAcmBase}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable. 0: Disable, define default value of BiosAcmBase , 1: enable}
  #TODO: This value is obtained from flash using GUID ApStartupBase- so keeping the default value as 0
  gPlatformFspPkgTokenSpaceGuid.BiosAcmBase                 | * | 0x08 | 0x00000000

  # !BSF NAME:{BiosAcmSize}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Enable/Disable. 0: Disable, define default value of BiosAcmSize , 1: enable}
  gPlatformFspPkgTokenSpaceGuid.BiosAcmSize                 | * | 0x04 | 0x00000000

  # !BSF NAME:{ApStartupBase}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Enable/Disable. 0: Disable, define default value of BiosAcmBase , 1: enable}
  #TODO: This value is obtained from flash using GUID - so keeping the default value as 0
  gPlatformFspPkgTokenSpaceGuid.ApStartupBase               | * | 0x04 | 0x00000000

  # !BSF NAME:{TgaSize}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Enable/Disable. 0: Disable, define default value of TgaSize , 1: enable}
  #TODO: Check if default 0 is ok
  gPlatformFspPkgTokenSpaceGuid.TgaSize                     | * | 0x04 | 0x00000000

  # !BSF NAME:{IsTPMPresence} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{IsTPMPresence default values}
  gPlatformFspPkgTokenSpaceGuid.IsTPMPresence               | * | 0x1 | 0x0

  ## TXT_PREMEM_CONFIG End

  ## CPU_POWER_MGMT_VR_CONFIG Start

  # !BSF NAME:{Acoustic Noise Mitigation feature}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enabling this option will help mitigate acoustic noise on certain SKUs when the CPU is in deeper C state. <b>0: Disabled</b>; 1: Enabled}
  gPlatformFspPkgTokenSpaceGuid.AcousticNoiseMitigation     | * | 0x01 | 0x00

  # !BSF NAME:{RfiMitigation}  TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable RFI Mitigation. <b>0: Disable - DCM is the IO_N default</b>; 1: Enable - Enable IO_N DCM/CCM switching as RFI mitigation.}
  gPlatformFspPkgTokenSpaceGuid.RfiMitigation                   | * | 0x01 | 0x00

  # !BSF NAME:{Platform Psys slope correction}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xC8)}
  # !BSF HELP:{PSYS Slope defined in 1/100 increments. <b>0 - Auto</b> Specified in 1/100 increment values. Range is 0-200. 125 = 1.25}
  gPlatformFspPkgTokenSpaceGuid.PsysSlope                   | * | 0x01 | 0x00

  # !BSF NAME:{Platform Power Pmax}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x400)}
  # !BSF HELP:{PSYS PMax power, defined in 1/8 Watt increments. <b>0 - Auto</b> Specified in 1/8 Watt increments. Range 0-1024 Watts(0-8191). Value of 800 = 100W}
  gPlatformFspPkgTokenSpaceGuid.PsysPmax                    | * | 0x02 | 0x00

  # !BSF NAME:{Thermal Design Current current limit} TYPE:{EditNum, HEX, (0x00,0x7FFF7FFF7FFF7FFF7FFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{TDC Current Limit, defined in 1/8A increments. Range 0-32767. For a TDC Current Limit of 125A, enter 1000. 0 = 0 Amps. <b>0: Auto</b>. [0] for IA, [1] for GT, [2] for SA, [3] for atom, [4]-[5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.TdcCurrentLimit             | * | 0xC | {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}

  # !BSF NAME:{AcLoadline} TYPE:{EditNum, HEX, (0x00,0x0x18691869186918691869)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{AC Loadline defined in 1/100 mOhms. A value of 100 = 1.00 mOhm, and 1255 = 12.55 mOhm. Range is 0-6249 (0-62.49 mOhms). 0 = AUTO/HW default. [0] for IA, [1] for GT, [2] for SA, [3] through [5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.AcLoadline                  | * | 0xC | {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}

  # !BSF NAME:{DcLoadline} TYPE:{EditNum, HEX, (0x00,0x18691869186918691869)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{DC Loadline defined in 1/100 mOhms. A value of 100 = 1.00 mOhm, and 1255 = 12.55 mOhm. Range is 0-6249 (0-62.49 mOhms). 0 = AUTO/HW default. [0] for IA, [1] for GT, [2] for SA, [3] through [5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.DcLoadline                  | * | 0xC | {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}

  # !BSF NAME:{Power State 1 Threshold current} TYPE:{EditNum, HEX, (0x00,0x02000200020002000200)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{PS Current Threshold1, defined in 1/4 A increments. A value of 400 = 100A. Range 0-152, which translates to 0-38A. 0 = AUTO. [0] for IA, [1] for GT, [2] for SA, [3] through [5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.Ps1Threshold               | * | 0xC | {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}

  # !BSF NAME:{Power State 2 Threshold current} TYPE:{EditNum, HEX, (0x00,0x02000200020002000200)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{PS Current Threshold2, defined in 1/4 A increments. A value of 400 = 100A. Range 0-48, which translates to 0-12A. 0 = AUTO. [0] for IA, [1] for GT, [2] for SA, [3] through [5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.Ps2Threshold               | * | 0xC | {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}

  # !BSF NAME:{Power State 3 Threshold current} TYPE:{EditNum, HEX, (0x00,0x02000200020002000200)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{PS Current Threshold3, defined in 1/4 A increments. A value of 400 = 100A. Range 0-16, which translates to 0-4A. 0 = AUTO. [0] for IA, [1] for GT, [2] for SA, [3] through [5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.Ps3Threshold               | * | 0xC | {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
  # Added reserved space  UnusedUpdSpace28[2]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd28              | * | 0x2 | {0x00}

  # !BSF NAME:{Imon offset correction}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{IMON Offset is an 32-bit signed value (2's complement). Units 1/1000, Range is [-128000, 127999]. For an offset of 25.348, enter 25348. <b>0: Auto</b>. [0] for IA, [1] for GT, [2] for SA, [3] through [5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.ImonOffset                  | * | 0x18 | {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}

  # !BSF NAME:{Icc Max limit} TYPE:{EditNum, HEX, (0x00,0x07FF07FF03FFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Voltage Regulator Current Limit (Icc Max). This value represents the Maximum instantaneous current allowed at any given time. The value is represented in 1/4 A increments. A value of 400 = 100A. 0 means AUTO. IA and GT, range 0-2047. SA range 0-1023. [0] for IA, [1] for GT, [2] for SA, [3] through [5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.IccMax                      | * | 0xC | {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}

  # !BSF NAME:{VR Fast Vmode VoltageLimit support} TYPE:{EditNum, HEX, (0x00,0x07F807F807F807F807F8)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Voltage Regulator Fast VoltageLimit .}
  gPlatformFspPkgTokenSpaceGuid.VrVoltageLimit         | * | 0xC | {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}

  # !BSF NAME:{Imon slope correction}  TYPE:{EditNum, HEX, (0x00,0x00C800C800C800C800C8)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{IMON Slope defined in 1/100 increments. Range is 0-200. For a 1.25 slope, enter 125. <b>0: Auto</b>. [0] for IA, [1] for GT, [2] for SA, [3] through [5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.ImonSlope                   | * | 0x0C | {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}

  # !BSF NAME:{Power State 3 enable/disable} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFF)}
  # !BSF HELP:{PS3 Enable/Disable. 0 - Disabled, 1 - Enabled. [0] for IA, [1] for GT, [2] for SA, [3] through [5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.Ps3Enable                  | * | 0x06 | {0x01, 0x01, 0x01, 0x01, 0x01, 0x01}

  # !BSF NAME:{Power State 4 enable/disable}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFF)}
  # !BSF HELP:{PS4 Enable/Disable. 0 - Disabled, 1 - Enabled. [0] for IA, [1] for GT, [2] for SA, [3] through [5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.Ps4Enable                  | * | 0x06 | {0x01, 0x01, 0x01, 0x01, 0x01, 0x01}

  # !BSF NAME:{Enable/Disable BIOS configuration of VR}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFF)}
  # !BSF HELP:{VR Config Enable. [0] for IA, [1] for GT, [2] for SA, [3] through [5] are Reserved. <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.VrConfigEnable              | * | 0x06 | {0x01, 0x01, 0x01, 0x01, 0x01, 0x01}

  # !BSF NAME:{Thermal Design Current enable/disable}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFF)}
  # !BSF HELP:{Thermal Design Current enable/disable; <b>0: Disable</b>; 1: Enable. [0] for IA, [1] for GT, [2] for SA, [3] for atom, [4]-[5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.TdcEnable                   | * | 0x06 | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{Thermal Design Current Lock}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFF)}
  # !BSF HELP:{Thermal Design Current Lock; <b>0: Disable</b>; 1: Enable. [0] for IA, [1] for GT, [2] for SA, [3] for atom, [4]-[5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.TdcLock                     | * | 0x06 | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{Disable Fast Slew Rate for Deep Package C States for VR domains}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{This option needs to be configured to reduce acoustic noise during deeper C states. False: Don't disable Fast ramp during deeper C states; True: Disable Fast ramp during deeper C state. [0] for IA, [1] for GT, [2] for SA, [3] through [5] are Reserved. <b>0: False</b>; 1: True}
  gPlatformFspPkgTokenSpaceGuid.FastPkgCRampDisable       | * | 0x06 | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{Slew Rate configuration for Deep Package C States for VR domains}
  # !BSF TYPE:{Combo} OPTION:{0: Fast/2, 1: Fast/4, 2: Fast/8, 3: Fast/16, 0xFF: Ignore the configuration}
  # !BSF HELP:{Set VR IA/GT/SA Slow Slew Rate for Deep Package C State ramp time; Slow slew rate equals to Fast divided by number, the number is 2, 4, 8, 16 to slow down the slew rate to help minimize acoustic noise; divide by 16 is disabled for GT/SA. <b>0: Fast/2</b>; 1: Fast/4; 2: Fast/8; 3: Fast/16; 0xFF: Ignore the configuration}
  gPlatformFspPkgTokenSpaceGuid.SlowSlewRate           | * | 0x06 | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  # Added reserved space  UnusedUpdSpace29[2]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd29                 | * | 0x2 | {0x00}

  # !BSF NAME:{Platform Psys offset correction}
  # !BSF TYPE:{EditNum, HEX, (0x0000,0xFFFFFFFF)}
  # !BSF HELP:{PSYS Offset defined in 1/1000 increments. <b>0 - Auto</b> This is an 32-bit signed value (2's complement). Units 1/1000, Range is [-128000, 127999]. For an offset of 25.348, enter 25348.}
  gPlatformFspPkgTokenSpaceGuid.PsysOffset                  | * | 0x04 | 0x00

  # !BSF NAME:{Thermal Design Current time window} TYPE:{EditNum, HEX, (0x00,0x0006D6000006D6000006D6000006D6000006D600)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Auto = 0 is default. Range is from 1ms to 448s. <b>0: Auto</b>. [0] for IA, [1] for GT, [2] for SA, [3] for atom, [4]-[5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.TdcTimeWindow               | * | 0x18 | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{TDC Mode}  TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFF)}
  # !BSF HELP:{TDC Mode based on IRMS supported bit from Mailbox. <b>0: iPL2</b>; 1: Irms. [0] for IA, [1] for GT, [2] for SA, [3] for atom [4]-[5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.TdcMode                        | * | 0x06 | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{DLVR RFI Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable DLVR RFI frequency hopping. 0: Disable; <b>1: Enable</b>.}
  gPlatformFspPkgTokenSpaceGuid.FivrSpectrumEnable                | * | 0x1 | 0x1

  # !BSF NAME:{DLVR RFI Spread Spectrum Percentage}
  # !BSF TYPE:{EditNum, HEX, (0x0,0x1F)}
  # !BSF HELP:{DLVR SSC in percentage with multiple of 0.25%. 0 = 0%, 10 = 4%. 0x00: 0% , <b>0x02: 0.5%</b>, 0x04: 1% , 0x08: 2% ,0x10: 4%; u3.2 value from 0% - 4%.}
  gPlatformFspPkgTokenSpaceGuid.DlvrSpreadSpectrumPercentage | * | 0x01 | 0x00

  # !BSF NAME:{DLVR RFI Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable DLVR RFI frequency hopping. 0: Disable; <b>1: Enable</b>.}
  gPlatformFspPkgTokenSpaceGuid.DlvrRfiEnable                | * | 0x1 | 0x1

  # !BSF NAME:{Pcore VR Hysteresis time window} TYPE:{EditNum, HEX, (0x00,0x32)} 0 is default . Range of PcoreHysteresisWindow from 1ms to 50ms.}
  gPlatformFspPkgTokenSpaceGuid.PcoreHysteresisWindow               | * | 0x1| 0x00

  # !BSF NAME:{Ecore VR Hysteresis time window} TYPE:{EditNum, HEX, (0x00,0x32)} 0 is default . Range of EcoreHysteresisWindow from 1ms to 50ms.}
  gPlatformFspPkgTokenSpaceGuid.EcoreHysteresisWindow               | * | 0x1| 0x00
  # Added reserved space  UnusedUpdSpace30
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd30                 | * | 0x1 | 0x00

  # !BSF NAME:{DLVR RFI Frequency}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFFFF)}
  # !BSF HELP:{DLVR RFI Frequency in MHz. <b>0: 2227 MHz</b> , 1: 2140MHZ.}
  gPlatformFspPkgTokenSpaceGuid.DlvrRfiFrequency             | * | 0x02 | 0x0

  # !BSF NAME:{DLVR PHASE_SSC Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable DLVR PHASE_SSC. <b>0: Disable.</b> 1:Enable.}
  gPlatformFspPkgTokenSpaceGuid.VrPowerDeliveryDesign        | * | 0x1 | 0x0

  # !BSF NAME:{DLVR PHASE_SSC Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable DLVR PHASE_SSC. <b>0: Disable.</b> 1:Enable.}
  gPlatformFspPkgTokenSpaceGuid.DlvrPhaseSsc                | * | 0x1 | 0x0

  # !BSF NAME:{Vsys Critical}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{PCODE MMIO Mailbox: Vsys Critical. <b>0: Disable</b>; 1: Enable Range is 0-255.}
  gPlatformFspPkgTokenSpaceGuid.EnableVsysCritical              | * | 0x01 | 0x00

  # !BSF NAME:{Assertion Deglitch Mantissa}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{Assertion Deglitch Mantissa, Range is 0-255}
  gPlatformFspPkgTokenSpaceGuid.VsysAssertionDeglitchMantissa   | * | 0x01 | 0x01

  # !BSF NAME:{Assertion Deglitch Exponent}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{Assertion Deglitch Exponent, Range is 0-255}
  gPlatformFspPkgTokenSpaceGuid.VsysAssertionDeglitchExponent   | * | 0x01 | 0x00

  # !BSF NAME:{De assertion Deglitch Mantissa}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{De assertion Deglitch Mantissa, Range is 0-255}
  gPlatformFspPkgTokenSpaceGuid.VsysDeassertionDeglitchMantissa | * | 0x01 | 0x0D

  # !BSF NAME:{De assertion Deglitch Exponent}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{De assertion Deglitch Exponent, Range is 0-255}
  gPlatformFspPkgTokenSpaceGuid.VsysDeassertionDeglitchExponent | * | 0x01 | 0x02
  # Added reserved space  UnusedUpdSpace31[1]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd31                 | * | 0x1 | 0x00

  # !BSF NAME:{VR Fast Vmode ICC Limit support} TYPE:{EditNum, HEX, (0x00,0x07F807F807F807F807F8)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Voltage Regulator Fast Vmode ICC Limit. A value of 400 = 100A. A value of 0 corresponds to feature disabled (no reactive protection). This value represents the current threshold where the VR would initiate reactive protection if Fast Vmode is enabled. The value is represented in 1/4 A increments. Range 0-2040. [0] for IA, [1] for GT, [2] for SA, [3] through [5] are Reserved.}
  gPlatformFspPkgTokenSpaceGuid.IccLimit                    | * | 0xC | {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}

  # !BSF NAME:{Enable/Disable VR FastVmode. The VR will initiate reactive protection if Fast Vmode is enabled.}
  # !BSF TYPE:{Combo} OPTION:{0: Disable, 1: Enable}
  # !BSF HELP:{Enable/Disable VR FastVmode; <b>0: Disable</b>; 1: Enable.For all VR by domain}
  gPlatformFspPkgTokenSpaceGuid.EnableFastVmode             | * | 0x6 | {0x1, 0x1, 0x1, 0x0, 0x0, 0x0}

  # !BSF NAME:{Enable/Disable CEP}
  # !BSF TYPE:{Combo} OPTION:{0: Disable, 1: Enable}
  # !BSF HELP:{Control for enabling/disabling CEP (Current Excursion Protection). <b>0: Disable</b>; 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.CepEnable                    | * | 0x6 | {0x01, 0x00, 0x01, 0x01, 0x00, 0x00}
  # Added reserved space  UnusedUpdSpace[6]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd48                  | * | 0x2 | {0x00}

  # !BSF NAME:{Vsys Full Scale}
  # !BSF TYPE:{EditNum, DEC, (0,255000)}
  # !BSF HELP:{Vsys Full Scale, Range is 0-255000mV}
  gPlatformFspPkgTokenSpaceGuid.VsysFullScale                   | * | 0x04 | 0x5DC0

  # !BSF NAME:{Vsys Critical Threshold}
  # !BSF TYPE:{EditNum, DEC, (0,255000)}
  # !BSF HELP:{Vsys Critical Threshold, Range is 0-255000mV }
  gPlatformFspPkgTokenSpaceGuid.VsysCriticalThreshold           | * | 0x04 | 0x1770

  # !BSF NAME:{Psys Full Scale}
  # !BSF TYPE:{EditNum, DEC, (0,255000)}
  # !BSF HELP:{Psys Full Scale, Range is 0-255000mV}
  gPlatformFspPkgTokenSpaceGuid.PsysFullScale                   | * | 0x04 | 0x30D40

  # !BSF NAME:{Psys Critical Threshold}
  # !BSF TYPE:{EditNum, DEC, (0,255000)}
  # !BSF HELP:{Psys Critical Threshold, Range is 0-255000mV }
  gPlatformFspPkgTokenSpaceGuid.PsysCriticalThreshold           | * | 0x04 | 0x1D4C0

  # !BSF NAME:{CpuPmVrRsvd} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Reserved for CPU Power Mgmt VR Config}
  gPlatformFspPkgTokenSpaceGuid.CpuPmVrRsvd                 | * | 0x08 | {0x00}

  ## CPU_POWER_MGMT_VR_CONFIG End

  ## PCH_GENERAL_PREMEM_CONFIG Start

  # !BSF NAME:{IOE Debug Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable IOE Debug. When enabled, IOE D2D Dfx link will keep up and clock is enabled}
  gPlatformFspPkgTokenSpaceGuid.IoeDebugEn                   | * | 0x01 | 0x00

  # !BSF NAME:{Pmode Clock Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable PMODE clock. When enabled, Pmode clock will toggle for XDP use}
  gPlatformFspPkgTokenSpaceGuid.PmodeClkEn                   | * | 0x01 | 0x00

  # !BSF NAME:{PCH Port80 Route} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Control where the Port 80h cycles are sent, 0: LPC; 1: PCI.}
  gPlatformFspPkgTokenSpaceGuid.PchPort80Route              | * | 0x01 | 0x00

  # !BSF NAME:{GPIO Override}  TYPE:{EditNum, HEX, (0x00, 0x7)}
  # !BSF HELP:{Gpio Override Level - FSP will not configure any GPIOs and rely on GPIO setings before moved to FSP. Available configurations 0: Disable; 1: Level 1 - Skips GPIO configuration in PEI/FSPM/FSPT phase;2: Level 2 - Reserved for future use}
  gPlatformFspPkgTokenSpaceGuid.GpioOverride                | * | 0x01  |  0x00

  # !BSF NAME:{Pmc Privacy Consent} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Pmc Privacy Consent}
  gPlatformFspPkgTokenSpaceGuid.PmcPrivacyConsent  | * | 0x01  |  0x00

  # !BSF NAME:{DMI ME UMA Root Space Check} TYPE:{Combo}
  # !BSF OPTION:{0: POR, 1: enable, 2: disable}
  # !BSF HELP:{DMI IOSF Root Space attribute check for RS3 for cycles targeting MEUMA.}
  gPlatformFspPkgTokenSpaceGuid.PchTestDmiMeUmaRootSpaceCheck | * | 0x1 | 0x00
  # Added reserved space   UnusedUpdSpace32[2]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd32                 | * | 0x2 | {0x00}

  ## PCH_GENERAL_PREMEM_CONFIG End

  #
  # MKTME Block Start
  #

  #
  # MKTME Block End
  #

  ## VTD_CONFIG Start

  # !BSF NAME:{PMR Size} TYPE:{Combo} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Size of PMR memory buffer. 0x400000 for normal boot and 0x200000 for S3 boot}
  gPlatformFspPkgTokenSpaceGuid.DmaBufferSize               | * | 0x04 | 0x0600000

  # !BSF NAME:{The policy for VTd driver behavior} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{BIT0: Enable IOMMU during boot, BIT1: Enable IOMMU when transfer control to OS}
  gPlatformFspPkgTokenSpaceGuid.PreBootDmaMask              | * | 0x1 | 0x2

  # !BSF NAME:{State of DMA_CONTROL_GUARANTEE bit in the DMAR table} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0=Disable/Clear, 1=Enable/Set}
  gPlatformFspPkgTokenSpaceGuid.DmaControlGuarantee         | * | 0x1 | 0x1

  # !BSF NAME:{Disable VT-d} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0=Enable/FALSE(VT-d enabled), 1=Disable/TRUE (VT-d disabled)}
  gPlatformFspPkgTokenSpaceGuid.VtdDisable                  | * | 0x1 | 0x0
  # Added reserved space   UnusedUpdSpace33[1]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd33               | * | 0x1 | 0x00

  # !BSF NAME:{Base addresses for VT-d function MMIO access} TYPE:{EditNum, HEX, (0, 0xFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Base addresses for VT-d MMIO access per VT-d engine}
  gPlatformFspPkgTokenSpaceGuid.VtdBaseAddress              | * | 0x24 | {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}
  # Added reserved space   UnusedUpdSpace34[4]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd34                 | * | 0x4 | {0x00}

  ## VTD_CONFIG End

  ## HOST_BRIDGE_PREMEM_CONFIG Start

  # !BSF NAME:{MMIO Size} TYPE:{EditNum, HEX, (0,0xC00)}
  # !BSF HELP:{Size of MMIO space reserved for devices. 0(Default)=Auto, non-Zero=size in MB}
  gPlatformFspPkgTokenSpaceGuid.MchBar                    | * | 0x08 | 0xFEDC0000

  # !BSF NAME:{MMIO Size} TYPE:{EditNum, HEX, (0,0xC00)}
  # !BSF HELP:{Size of MMIO space reserved for devices. 0(Default)=Auto, non-Zero=size in MB}
  gPlatformFspPkgTokenSpaceGuid.RegBar                    | * | 0x08 | 0xF0000000

  # !BSF NAME:{MMIO Size} TYPE:{EditNum, HEX, (0,0xC00)}
  # !BSF HELP:{Size of MMIO space reserved for devices. 0(Default)=Auto, non-Zero=size in MB}
  gPlatformFspPkgTokenSpaceGuid.MmioSize                    | * | 0x02 | 0x0

  # !BSF NAME:{MMIO size adjustment for AUTO mode} TYPE:{EditNum, HEX, (0, 0xFFFF)}
  # !BSF HELP:{Positive number means increasing MMIO size, Negative value means decreasing MMIO size: 0 (Default)=no change to AUTO mode MMIO size}
  gPlatformFspPkgTokenSpaceGuid.MmioSizeAdjustment          | * | 0x02 | 0x67

  # !BSF NAME:{Temporary address for ApicLocalAddress} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{The reference code will use this as Temporary address space}
  gPlatformFspPkgTokenSpaceGuid.ApicLocalAddress                         | * | 0x4 | 0xFEE00000

  # !BSF NAME:{Temporary address for NvmeHcPeiMmioBase} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{The reference code will use this as Temporary address space}
  gPlatformFspPkgTokenSpaceGuid.NvmeHcPeiMmioBase                            | * | 0x4 | 0xA0000000

  # !BSF NAME:{Temporary address for NvmeHcPeiMmioLimit} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{The reference code will use this as Temporary address space}
  gPlatformFspPkgTokenSpaceGuid.NvmeHcPeiMmioLimit                           | * | 0x4 | 0xA1000000

  # !BSF NAME:{Temporary address for AhciPeiMmioBase} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{The reference code will use this as Temporary address space}
  gPlatformFspPkgTokenSpaceGuid.AhciPeiMmioBase                              | * | 0x4 | 0x90000000

  # !BSF NAME:{Temporary address for AhciPeiMmioLimit} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{The reference code will use this as Temporary address space}
  gPlatformFspPkgTokenSpaceGuid.AhciPeiMmioLimit                             | * | 0x4 | 0x91000000

  # !BSF NAME:{Temporary address for EcExtraIoBase} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{The reference code will use this as Temporary address space}
  gPlatformFspPkgTokenSpaceGuid.EcExtraIoBase                                | * | 0x2 | 0x6A0

  # !BSF NAME:{Temporary address for SioBaseAddress} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{The reference code will use this as Temporary address space}
  gPlatformFspPkgTokenSpaceGuid.SioBaseAddress                               | * | 0x2 | 0x0680

  # !BSF NAME:{Temporary CfgBar address for VMD} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{The reference code will use this as Temporary Cfg Bar address space to access Cfg space of mapped devices. Platform should provide conflict free Temporary MMIO Range to (CfgBarAddr + 32MB MMIO).
  gPlatformFspPkgTokenSpaceGuid.VmdCfgBarBar                    | * | 0x4 | 0xA0000000

  # !BSF NAME:{System Agent SafBar} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{Address of System Agent SafBar}
  gPlatformFspPkgTokenSpaceGuid.SafBar                  | * | 0x08 | 0xFA000000

  # !BSF NAME:{Enable above 4GB MMIO resource support} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable above 4GB MMIO resource support}
  gPlatformFspPkgTokenSpaceGuid.EnableAbove4GBMmio          | * | 0x01 | 0x01

  # !BSF NAME:{Enable/Disable SA CRID} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable: SA CRID, Disable (Default): SA CRID}
  gPlatformFspPkgTokenSpaceGuid.CridEnable                  | * | 0x01 | 0x0
  # Added reserved space   UnusedUpdSpace35[2]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd35                | * | 0x2 | {0x00}

  ## HOST_BRIDGE_PREMEM_CONFIG End

  #
  # SA Pre-Mem Block Start
  #

  # !BSF NAME:{StreamTracer Mode} TYPE:{Combo}
  # !BSF OPTION:{0: Disable (Default), 524288: Advanced Tracing , 8192: Auto , 3: User input}
  # !BSF HELP:{Disable: Disable StreamTracer, Advanced Tracing: StreamTracer size 512MB - Recommended when all groups in high verbosity are traced in 'red', Auto: StreamTracer size 8MB - Recommended when using up to 8 groups red or up to 16 groups in green in med verbosity, User input: Allow User to enter a size in the range of 64KB-512MB}
  gPlatformFspPkgTokenSpaceGuid.StreamTracerMode             | * | 0x04 | 0x00

  # !BSF TYPE:{EditNum, HEX, (0x40, 0x20000000)}
  # !BSF HELP:{Size of StreamTracer in KB, Range 64KB - 512 MB, Update this upd only when StreamTracer is in User input mode}
  gPlatformFspPkgTokenSpaceGuid.StreamTracerSize             | * | 0x04 | 0x40

  # !BSF NAME:{Enable/Disable CrashLog Device} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable CrashLog/Telemetry Device  0- Disable, <b>1- Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.CpuCrashLogDevice   | * | 0x04 | 0x1

  # !BSF NAME:{StreamTracer physical address} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{StreamTracer physical address}
  gPlatformFspPkgTokenSpaceGuid.StreamTracerBase  | * | 0x08 | 0x0

  # !BSF NAME:{Temporary MemBar1 address for VMD} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{The reference code will use this as Temporary MemBar1 address space to access MMIO space of mapped devices. Platform should provide conflict free Temporary MMIO Range to (MemBar1Addr + 32MB MMIO).
  gPlatformFspPkgTokenSpaceGuid.VmdMemBar1Bar                    | * | 0x4 | 0xA2000000

  # !BSF NAME:{Temporary MemBar2 address for VMD} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{The reference code will use this as Temporary MemBar2 address space to access MMIO space of mapped devices. Platform should provide conflict free Temporary MMIO Range to (MemBar2Addr + 1MB MMIO).
  gPlatformFspPkgTokenSpaceGuid.VmdMemBar2Bar                    | * | 0x4 | 0xA4000000

  ## SI_PREMEM_CONFIG Start

  # !BSF NAME:{Skip override boot mode When Fw Update.} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When set to TRUE and boot mode is BOOT_ON_FLASH_UPDATE, skip setting boot mode to BOOT_WITH_FULL_CONFIGURATION in PEI memory init.}
  gPlatformFspPkgTokenSpaceGuid.SiSkipOverrideBootModeWhenFwUpdate  | * | 0x01 | 0x00
  # Added reserved space   UnusedUpdSpace36
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd36                 | * | 0x1 | 0x00


  # !BSF NAME:{Static Content at 4GB Location} TYPE:{Combo}
  # !BSF OPTION:{0: No Allocation, 0x20:32MB, 0x40:64MB, 0x80:128MB, 0x100:256MB, 0x200:512MB, 0x400:1GB, 0x800:2GB, 0xC00:3GB, 0x1000:4GB, 0x2000:8GB}
  # !BSF HELP:{0 (Default): No Allocation, 0x20:32MB, 0x40:64MB, 0x80:128MB, 0x100:256MB, 0x200:512MB, 0x400:1GB, 0x800:2GB, 0xC00:3GB, 0x1000:4GB, 0x2000:8GB}
  gPlatformFspPkgTokenSpaceGuid.StaticContentSizeAt4Gb            | * | 0x2 | 0x0

  # !BSF NAME:{Platform Debug Option} TYPE:{Combo} OPTION:{0:Disabled, 2:Enabled Trace Active, 4:Enabled Trace Ready, 6:Enable Trace Power-Off, 7:Manual}
  # !BSF HELP:{Enabled Trace active: TraceHub is enabled and trace is active, blocks s0ix.\n\nEnabled Trace ready: TraceHub is enabled and allowed S0ix.\n\nEnabled Trace power off: TraceHub is powergated, provide setting close to functional low power state\n\nManual: user needs to configure Advanced Debug Settings manually, aimed at advanced users}
  gPlatformFspPkgTokenSpaceGuid.PlatformDebugOption                 | * | 0x01 | 0x00

  ## SI_PREMEM_CONFIG End

  # !BSF NAME:{SiPreMemRsvd} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Reserved for SI Pre-Mem}
  gPlatformFspPkgTokenSpaceGuid.SiPreMemRsvd                        | * | 0x0E | {0x00}

  #
  # SI Pre-Mem Block End
  #

  ## GRAPHICS_PEI_PREMEM_CONFIG Start

  # !BSF NAME:{Program GPIOs for LFP on DDI port-A device} TYPE:{Combo}
  # !BSF OPTION:{0:Disabled, 1:eDP, 2:MIPI DSI}
  # !BSF HELP:{0=Disabled,1(Default)=eDP, 2=MIPI DSI}
  gPlatformFspPkgTokenSpaceGuid.DdiPortAConfig              | * | 0x1 | 0x1

  # !BSF NAME:{HgSubSystemId} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Hybrid Graphics SubSystemId}
  gPlatformFspPkgTokenSpaceGuid.HgSubSystemId                | * | 0x2 | 0x2112

  # !BSF NAME:{Program GPIOs for LFP on DDI port-B device} TYPE:{Combo}
  # !BSF OPTION:{0:Disabled, 1:eDP, 2:MIPI DSI}
  # !BSF HELP:{0(Default)=Disabled,1=eDP, 2=MIPI DSI}
  gPlatformFspPkgTokenSpaceGuid.DdiPortBConfig              | * | 0x1 | 0x0

  # !BSF NAME:{Enable or disable HPD of DDI port A} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0(Default)=Disable, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.DdiPortAHpd                 | * | 0x1 | 0x0

  # !BSF NAME:{Enable or disable HPD of DDI port B} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0=Disable, 1(Default)=Enable}
  gPlatformFspPkgTokenSpaceGuid.DdiPortBHpd                 | * | 0x1 | 0x1

  # !BSF NAME:{Enable or disable HPD of DDI port C} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0(Default)=Disable, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.DdiPortCHpd                 | * | 0x1 | 0x0

  # !BSF NAME:{Enable or disable HPD of DDI port 1} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0=Disable, 1(Default)=Enable}
  gPlatformFspPkgTokenSpaceGuid.DdiPort1Hpd                 | * | 0x1 | 0x0

  # !BSF NAME:{Enable or disable HPD of DDI port 2} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0(Default)=Disable, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.DdiPort2Hpd                 | * | 0x1 | 0x0

  # !BSF NAME:{Enable or disable HPD of DDI port 3} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0(Default)=Disable, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.DdiPort3Hpd                 | * | 0x1 | 0x0

  # !BSF NAME:{Enable or disable HPD of DDI port 4} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0(Default)=Disable, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.DdiPort4Hpd                 | * | 0x1 | 0x0

  # !BSF NAME:{Enable or disable DDC of DDI port A} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0(Default)=Disable, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.DdiPortADdc                 | * | 0x1 | 0x0

  # !BSF NAME:{Enable or disable DDC of DDI port B} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0=Disable, 1(Default)=Enable}
  gPlatformFspPkgTokenSpaceGuid.DdiPortBDdc                 | * | 0x1 | 0x1

  # !BSF NAME:{Enable or disable DDC of DDI port C} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0(Default)=Disable, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.DdiPortCDdc                 | * | 0x1 | 0x0

  # !BSF NAME:{Enable DDC setting of DDI Port 1} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0(Default)=Disable, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.DdiPort1Ddc                 | * | 0x1 | 0x0

  # !BSF NAME:{Enable DDC setting of DDI Port 2} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0(Default)=Disable, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.DdiPort2Ddc                 | * | 0x1 | 0x0

  # !BSF NAME:{Enable DDC setting of DDI Port 3} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0(Default)=Disable, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.DdiPort3Ddc                 | * | 0x1 | 0x0

  # !BSF NAME:{Enable DDC setting of DDI Port 4} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0(Default)=Disable, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.DdiPort4Ddc                 | * | 0x1 | 0x0

  # !BSF NAME:{Oem T12 Dealy Override} TYPE:{Combo}  OPTION:{$EN_DIS}
  # !BSF HELP:{Oem T12 Dealy Override. 0(Default)=Disable  1=Enable }
  gPlatformFspPkgTokenSpaceGuid.OemT12DelayOverride         | * | 0x1 | 0x0
  # Added reserved space   UnusedUpdSpace37[2]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd37                 | * | 0x2 | {0x00}

  # !BSF NAME:{Temporary MMIO address for GMADR} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{The reference code will use this as Temporary MMIO address space to access GMADR Registers.Platform should provide conflict free Temporary MMIO Range: GmAdr to (GmAdr + 256MB). Default is (PciExpressBaseAddress - 256MB) to (PciExpressBaseAddress - 0x1)}
  gPlatformFspPkgTokenSpaceGuid.LMemBar                     | * | 0x8 | 0xB0000000

  # !BSF NAME:{Temporary MMIO address for GTTMMADR} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{The reference code will use this as Temporary MMIO address space to access GTTMMADR Registers.Platform should provide conflict free Temporary MMIO Range: GttMmAdr to (GttMmAdr + 2MB MMIO + 6MB Reserved + GttSize). Default is (GmAdr - (2MB MMIO + 6MB Reserved + GttSize)) to (GmAdr - 0x1) (Where GttSize = 8MB)}
  gPlatformFspPkgTokenSpaceGuid.GttMmAdr                    | * | 0x8 | 0xAF000000

  # !BSF NAME:{Delta T12 Power Cycle Delay required in ms} TYPE:{Combo}
  # !BSF OPTION:{0 : No Delay, 0xFFFF : Auto Calulate T12 Delay}
  # !BSF HELP:{Select the value for delay required. 0= No delay, 0xFFFF(Default) = Auto calculate T12 Delay to max 500ms}
  gPlatformFspPkgTokenSpaceGuid.DeltaT12PowerCycleDelay     | * | 0x2 | 0x0

  # !BSF NAME:{Enable/Disable Memory Bandwidth Compression} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0=Disable, 1(Default)=Enable}
  gPlatformFspPkgTokenSpaceGuid.MemoryBandwidthCompression  | * | 0x1 | 0x1

  # !BSF NAME:{Panel Power Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Control for enabling/disabling VDD force bit (Required only for early enabling of eDP panel). 0=Disable, 1(Default)=Enable}
  gPlatformFspPkgTokenSpaceGuid.PanelPowerEnable            | * | 0x01 | 0x01

  # !BSF NAME:{Selection of the primary display device} TYPE:{Combo}
  # !BSF OPTION:{3:AUTO, 0:IGFX, 4:HG}
  # !BSF HELP:{3(Default)=AUTO, 0=IGFX, 4=Hybrid Graphics}
  gPlatformFspPkgTokenSpaceGuid.PrimaryDisplay              | * | 0x01 | 0x0

  # !BSF NAME:{Internal Graphics Data Stolen Memory GSM2} TYPE:{Combo}
  # !BSF OPTION:{0:2GB, 1:4GB, 2:6GB, 3:8GB, 4:10GB, 5:12GB, 6:14GB, 7:16GB, 8:18GB, 9:20GB, 10:22GB, 11:24GB, 12:26GB, 13:28GB, 14:30GB, 15:32GB, 0xFF:No Allocation}
  # !BSF HELP:{Size of memory preallocated for internal graphics GSM2.}
  gPlatformFspPkgTokenSpaceGuid.IGpuGsm2Size                | * | 0x01 | 0xFF
  # Added reserved space  UnusedUpdSpace46[2]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd46               | * | 0x2 | {0x00}

  # !BSF NAME:{Intel Graphics VBT (Video BIOS Table) Size}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Size of Internal Graphics VBT Image}
  gPlatformFspPkgTokenSpaceGuid.VbtSize                     | * | 0x04 | 0x00000000
  # Added reserved space  UnusedUpdSpace47[4]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd47                | * | 0x4 | {0x00}

  # !BSF NAME:{Graphics Configuration Ptr}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Points to VBT}
  gPlatformFspPkgTokenSpaceGuid.VbtPtr                      | * | 0x08 | 0x00000000

  # !BSF NAME:{SOL Training Message Pointer}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Points to SOL Message String}
  gPlatformFspPkgTokenSpaceGuid.VgaMessage                  | * | 0x08 | 0x00000000

  # !BSF NAME:{Platform LID Status for LFP Displays.} TYPE:{Combo}
  # !BSF OPTION:{0: LidClosed, 1: LidOpen}
  # !BSF HELP:{LFP Display Lid Status (LID_STATUS enum): 0 (Default): LidClosed, 1: LidOpen.}
  gPlatformFspPkgTokenSpaceGuid.LidStatus                   | * | 0x01 | 0x0

  # !BSF NAME:{Control SOL VGA Initialition sequence} TYPE:{Combo}
  # !BSF OPTION:{0x0: NO SOL VGA Init, 0x1: SOL VGA Init}
  # !BSF HELP:{Initialise SOL Init}
  gPlatformFspPkgTokenSpaceGuid.VgaInitControl              | * | 0x01 | 0x1

  ## GRAPHICS_PEI_PREMEM_CONFIG End

  ## TCSS_PEI_PREMEM_CONFIG Start

  # !BSF NAME:{TCSS USB HOST (xHCI) Enable}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{Set TCSS XHCI. 0:Disabled  1:Enabled - Must be enabled if xDCI is enabled below}
  gPlatformFspPkgTokenSpaceGuid.TcssXhciEn                | * | 0x01 | 0x01

  # !BSF NAME:{IomUsbCDpConfig}
  # !BSF OPTION:{$EN_DIS}
  # !BSF HELP:{Set IomUsbCDpConfig. 0:Disabled  1:Enabled - Must be enabled if xDCI is enabled below}
  gPlatformFspPkgTokenSpaceGuid.IomUsbCDpConfig                | * | 0x04 | {0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{TCSS Type C Port 0}
  # !BSF TYPE:{Combo} OPTION:{0:DISABLE, 1:DP_ONLY, 2:NO_TBT, 3: NO_PCIE, 7:FULL_FUN}
  # !BSF HELP:{Set TCSS Type C Port 0 Type, Options are 0=DISABLE, 1=DP_ONLY, 2=NO_TBT, 3=NO_PCIE, 7=FULL_FUN}
  # ToClean
  gPlatformFspPkgTokenSpaceGuid.TcssPort0                         | * | 0x01 | 0x07

  # !BSF NAME:{TCSS Type C Port 1}
  # !BSF TYPE:{Combo} OPTION:{0:DISABLE, 1:DP_ONLY, 2:NO_TBT, 3: NO_PCIE, 7:FULL_FUN}
  # !BSF HELP:{Set TCSS Type C Port 1 Type, Options are 0=DISABLE, 1=DP_ONLY, 2=NO_TBT, 3=NO_PCIE, 7=FULL_FUN}
  # ToClean
  gPlatformFspPkgTokenSpaceGuid.TcssPort1                         | * | 0x01 | 0x07

  # !BSF NAME:{TCSS Type C Port 2}
  # !BSF TYPE:{Combo} OPTION:{0:DISABLE, 1:DP_ONLY, 2:NO_TBT, 3: NO_PCIE, 7:FULL_FUN}
  # !BSF HELP:{Set TCSS Type C Port 2 Type, Options are 0=DISABLE, 1=DP_ONLY, 2=NO_TBT, 3=NO_PCIE, 7=FULL_FUN}
  # ToClean
  gPlatformFspPkgTokenSpaceGuid.TcssPort2                         | * | 0x01 | 0x07

  # !BSF NAME:{TCSS Type C Port 3}
  # !BSF TYPE:{Combo} OPTION:{0:DISABLE, 1:DP_ONLY, 2:NO_TBT, 3: NO_PCIE, 7:FULL_FUN}
  # !BSF HELP:{Set TCSS Type C Port 3 Type, Options are 0=DISABLE, 1=DP_ONLY, 2=NO_TBT, 3=NO_PCIE, 7=FULL_FUN}
  # ToClean
  gPlatformFspPkgTokenSpaceGuid.TcssPort3                         | * | 0x01 | 0x00
  # Added reserved space   UnusedUpdSpace38[1]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd38             | * | 0x1 | 0x00

  # !BSF NAME:{TypeC port GPIO setting} TYPE:{EditNum, HEX, (0, 0xFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{GPIO Pin number for Type C Aux orientation setting, use the GpioPad that is defined in GpioPinsXXX.h as argument.(XXX is platform name, Ex: Ptl = PantherLake)}
  gPlatformFspPkgTokenSpaceGuid.IomTypeCPortPadCfg        | * | 0x30 | {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}

  # !BSF NAME:{TCSS Aux Orientation Override Enable} TYPE:{EditNum, HEX, (0x0,0x0FFF)}
  # !BSF HELP:{Bits 0, 2, ... 10 control override enables, bits 1, 3, ... 11 control overrides}
  gPlatformFspPkgTokenSpaceGuid.TcssAuxOri                | * | 0x02 | 0x0000

  # !BSF NAME:{TCSS HSL Orientation Override Enable} TYPE:{EditNum, HEX, (0x0,0x0FFF)}
  # !BSF HELP:{Bits 0, 2, ... 10 control override enables, bits 1, 3, ... 11 control overrides}
  gPlatformFspPkgTokenSpaceGuid.TcssHslOri                | * | 0x02 | 0x0000

  ## TCSS_PEI_PREMEM_CONFIG End

  ## CNVI_PREMEM_CONFIG Start

  # !BSF NAME:{CNVi DDR RFI Mitigation} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable DDR RFI Mitigation. Default is ENABLE. 0: DISABLE, 1: ENABLE}
  gPlatformFspPkgTokenSpaceGuid.CnviDdrRfim                 | * | 0x01 | 0x01

  ## CNVI_PREMEM_CONFIG End

  ## TRACE_HUB_PREMEM_CONFIG Start

  # !BSF NAME:{SOC Trace Hub Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable SOC TraceHub}
  gPlatformFspPkgTokenSpaceGuid.SocTraceHubMode              | * | 0x01 | 0x00

  # !BSF NAME:{SOC Trace Hub Memory Region 0 buffer Size} TYPE:{Combo} OPTION:{0x00:1MB, 0x03:8MB, 0x06:64MB, 0x07:128MB, 0x08:256MB, 0x09:512MB, 0x0A:1GB, 0x0B:2GB, 0x0C:4GB, 0x0D:8GB, 0x0E:0MB}
  # !BSF HELP:{Select size of memory region 0 buffer. Memory allocated by BIOS only applies to ITH tool running on the host. For ITH tool running on the target, choose None/OS, memory shall be allocated by tool. User should be cautious to choose the amount of memory. If chosen size is larger than half of system memory, setup will automatically rollback to default value.}
  gPlatformFspPkgTokenSpaceGuid.SocTraceHubMemReg0Size       | * | 0x02 | 0x0E

  # !BSF NAME:{SOC Trace Hub Memory Region 0 buffer Size} TYPE:{Combo} OPTION:{0x00:1MB, 0x03:8MB, 0x06:64MB, 0x07:128MB, 0x08:256MB, 0x09:512MB, 0x0A:1GB, 0x0B:2GB, 0x0C:4GB, 0x0D:8GB, 0x0E:0MB}
  # !BSF HELP:{Select size of memory region 1 buffer. Memory allocated by BIOS only applies to ITH tool running on the host. For ITH tool running on the target, choose None/OS, memory shall be allocated by tool. User should be cautious to choose the amount of memory. If chosen size is larger than half of system memory, setup will automatically rollback to default value.}
  gPlatformFspPkgTokenSpaceGuid.SocTraceHubMemReg1Size       | * | 0x02 | 0x0E

  ## TRACE_HUB_PREMEM_CONFIG End

  ## IGPU_PEI_PREMEM_CONFIG Start

  # !BSF NAME:{Internal Graphics Pre-allocated Memory} TYPE:{Combo}
  # !BSF OPTION:{0x00:0MB, 0x01:32MB, 0x02:64MB, 0x03:96MB, 0x04:128MB, 0xF0:4MB, 0xF1:8MB, 0xF2:12MB, 0xF3:16MB, 0xF4:20MB, 0xF5:24MB, 0xF6:28MB, 0xF7:32MB, 0xF8:36MB, 0xF9:40MB, 0xFA:44MB, 0xFB:48MB, 0xFC:52MB, 0xFD:56MB, 0xFE:60MB}
  # !BSF HELP:{Size of memory preallocated for internal graphics.}
  gPlatformFspPkgTokenSpaceGuid.IgdDvmt50PreAlloc           | * | 0x02 | 0x2

  # !BSF NAME:{Internal Graphics} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable internal graphics.}
  gPlatformFspPkgTokenSpaceGuid.InternalGraphics                 | * | 0x01 | 0x02

  ## IGPU_PEI_PREMEM_CONFIG End

  #
  # MRC Block Start
  #

  # !BSF NAME:{Asynchronous ODT}
  # !BSF TYPE:{Combo} OPTION:{0:Enabled, 1:Disabled}
  # !BSF HELP:{This option configures the Memory Controler Asynchronous ODT control}
  gPlatformFspPkgTokenSpaceGuid.AsyncOdtDis              | * | 0x01 | 0x0

  # !BSF NAME:{DLL Weak Lock Support}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable DLL Weak Lock Support}
  gPlatformFspPkgTokenSpaceGuid.WeaklockEn               | * | 0x01 | 0x00
  # Added reserved space   UnusedUpdSpace39
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd39            | * | 0x1 | 0x00

  # !BSF NAME:{Fore Single Rank config}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables/Disable Fore Single Rank config}
  gPlatformFspPkgTokenSpaceGuid.ForceSingleRank          | * | 0x04 | 0x00

  # !BSF NAME:{DynamicMemoryBoost}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Dynamic Memory Boost Feature. Only valid if SpdProfileSelected is an XMP Profile; otherwise ignored. <b>0=Disabled</b>, 1=Enabled.}
  gPlatformFspPkgTokenSpaceGuid.DynamicMemoryBoost          | * | 0x04 | 0x00

  # !BSF NAME:{RealtimeMemoryFrequency}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Realtime Memory Frequency feature. Only valid if SpdProfileSelected is an XMP Profile; otherwise ignored. <b>0=Disabled</b>, 1=Enabled.}
  gPlatformFspPkgTokenSpaceGuid.RealtimeMemoryFrequency          | * | 0x04 | 0x00

  # !BSF NAME:{SelfRefresh IdleTimer}
  # !BSF TYPE:{EditNum, HEX, (0x100,0xFFFF)}
  # !BSF HELP:{SelfRefresh IdleTimer, Default is 256}
  gPlatformFspPkgTokenSpaceGuid.SrefCfgIdleTmr          | * | 0x02 | 0x640

  # !BSF NAME:{MC Register Offset}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Apply user offsets to select MC registers(Def=Disable)}
  gPlatformFspPkgTokenSpaceGuid.MCREGOFFSET     | * | 0x01 | 0x00

  # !BSF NAME:{CA Vref Ctl Offset}
  # !BSF TYPE:{Combo} OPTION:{0xF4:-12,0xF5:-11, 0xF6:-10, 0xF7:-9, 0xF8:-8, 0xF9:-7, 0xFA:-6, 0xFB:-5, 0xFC:-4, 0xFD:-3, 0xFE:-2, 0xFF:-1, 0:0, 1:+1, 2:+2, 3:+3, 4:+4, 5:+5, 6:+6, 7:+7, 8:+8, 9:+9, 10:+10, 11:+11, 12:+12}
  # !BSF HELP:{Offset to be applied to DDRDATA7CH1_CR_DDRCRVREFADJUST1.CAVref}
  gPlatformFspPkgTokenSpaceGuid.CAVrefCtlOffset          | * | 0x01 | 0x00

  # !BSF NAME:{Clk PI Code Offset}
  # !BSF TYPE:{Combo} OPTION:{0xFA:-6, 0xFB:-5, 0xFC:-4, 0xFD:-3, 0xFE:-2, 0xFF:-1, 0:0, 1:+1, 2:+2, 3:+3, 4:+4, 5:+5, 6:+6}
  # !BSF HELP:{Offset to be applied to DDRCLKCH0_CR_DDRCRCLKPICODE.PiSettingRank[0-3]}
  gPlatformFspPkgTokenSpaceGuid.ClkPiCodeOffset          | * | 0x01 | 0x00

  # !BSF NAME:{RcvEn Offset}
  # !BSF TYPE:{Combo} OPTION:{0xFD:-3, 0xFE:-2, 0xFF:-1, 0:0, 1:+1, 2:+2, 3:+3}
  # !BSF HELP:{Offset to be applied to DDRDATACH0_CR_DDRCRDATAOFFSETTRAIN.RcvEn}
  gPlatformFspPkgTokenSpaceGuid.RcvEnOffset          | * | 0x01 | 0x00

  # !BSF NAME:{Rx Dqs Offset}
  # !BSF TYPE:{Combo} OPTION:{0xFD:-3, 0xFE:-2, 0xFF:-1, 0:0, 1:+1, 2:+2, 3:+3}
  # !BSF HELP:{Offset to be applied to DDRDATACHX_CR_DDRCRDATAOFFSETTRAIN.RxDqsOffset}
  gPlatformFspPkgTokenSpaceGuid.RxDqsOffset          | * | 0x01 | 0x00

  # !BSF NAME:{Tx Dq Offset}
  # !BSF TYPE:{Combo} OPTION:{0xFD:-3, 0xFE:-2, 0xFF:-1, 0:0, 1:+1, 2:+2, 3:+3}
  # !BSF HELP:{Offset to be applied to DDRDATACH0_CR_DDRCRDATAOFFSETTRAIN.TxDqOffset}
  gPlatformFspPkgTokenSpaceGuid.TxDqOffset          | * | 0x01 | 0x00

  # !BSF NAME:{Tx Dqs Offset}
  # !BSF TYPE:{Combo} OPTION:{0xFD:-3, 0xFE:-2, 0xFF:-1, 0:0, 1:+1, 2:+2, 3:+3}
  # !BSF HELP:{Offset to be applied to DDRDATACH0_CR_DDRCRDATAOFFSETTRAIN.TxDqsOffset}
  gPlatformFspPkgTokenSpaceGuid.TxDqsOffset          | * | 0x01 | 0x00

  # !BSF NAME:{Vref Offset}
  # !BSF TYPE:{Combo} OPTION:{0xFA:-6, 0xFB:-5, 0xFC:-4, 0xFD:-3, 0xFE:-2, 0xFF:-1, 0:0, 1:+1, 2:+2, 3:+3, 4:+4, 5:+5, 6:+6}
  # !BSF HELP:{Offset to be applied to DDRDATACH0_CR_DDRCRDATAOFFSETTRAIN.VrefOffset}
  gPlatformFspPkgTokenSpaceGuid.VrefOffset          | * | 0x01 | 0x00

  # !BSF NAME:{Controller mask}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xff)}
  # !BSF HELP:{Controller mask to apply on parameter offset}
  gPlatformFspPkgTokenSpaceGuid.CntrlrMask          | * | 0x01 | 0x00

  # !BSF NAME:{Channel  mask}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xff)}
  # !BSF HELP:{Channel  mask to apply on parameter offset}
  gPlatformFspPkgTokenSpaceGuid.ChMask          | * | 0x01 | 0x00

  # !BSF NAME:{tRRSG Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Read-to-Read commands in the same Bank Group. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tRRSG          | * | 0x01 | 0x00


  # !BSF NAME:{tRRDG Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Read-to-Read commands in different Bank Group. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tRRDG          | * | 0x01 | 0x00

  # !BSF NAME:{tRRDR Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Read-to-Read commands in different Ranks. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tRRDR          | * | 0x01 | 0x00

  # !BSF NAME:{tRRDD Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Read-to-Read commands in different DIMMs. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tRRDD          | * | 0x01 | 0x00

  # !BSF NAME:{tWRSG Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Write-to-Read commands in the same Bank Group. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tWRSG          | * | 0x01 | 0x00

  # !BSF NAME:{tWRDG Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Write-to-Read commands in different Bank Group. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tWRDG          | * | 0x01 | 0x00

  # !BSF NAME:{tWRDR Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Write-to-Read commands in different Ranks. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tWRDR          | * | 0x01 | 0x00

  # !BSF NAME:{tWRDD Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Write-to-Read commands in different DIMMs. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tWRDD          | * | 0x01 | 0x00

  # !BSF NAME:{tWWSG Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Write-to-Write commands in the same Bank Group. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tWWSG          | * | 0x01 | 0x00

  # !BSF NAME:{tWWDG Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Write-to-Write commands in different Bank Group. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tWWDG          | * | 0x01 | 0x00

  # !BSF NAME:{tWWDR Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Write-to-Write commands in different Ranks. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tWWDR          | * | 0x01 | 0x00

  # !BSF NAME:{tWWDD Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Write-to-Write commands in different DIMMs. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tWWDD          | * | 0x01 | 0x00

  # !BSF NAME:{tRWSG Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Read-to-Write commands in the same Bank Group. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tRWSG          | * | 0x01 | 0x00

  # !BSF NAME:{tRWDG Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Read-to-Write commands in different Bank Group. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tRWDG          | * | 0x01 | 0x00

  # !BSF NAME:{tRWDR Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Read-to-Write commands in different Ranks. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tRWDR          | * | 0x01 | 0x00

  # !BSF NAME:{tRWDD Delta}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Delay between Read-to-Write commands in different DIMMs. 0 - Auto. Signed TAT delta is (Value - 128). Input value range of [1..255] will give a TAT delta range of [-127..127]}
  gPlatformFspPkgTokenSpaceGuid.tRWDD          | * | 0x01 | 0x00

  # !BSF NAME:{MRC Interpreter}
  # !BSF TYPE:{Combo} OPTION:{0:CMOS, 1:Break, 2:Force}
  # !BSF HELP:{Select CMOS location match of DD01 or Ctrl-Break key or force entry}
  gPlatformFspPkgTokenSpaceGuid.Interpreter          | * | 0x01 | 0x00

  # !BSF NAME:{ODT mode}
  # !BSF TYPE:{Combo} OPTION:{0:Default, 1:Vtt, 2:Vddq, 3:Vss, 4:Max}
  # !BSF HELP:{ODT mode}
  gPlatformFspPkgTokenSpaceGuid.IoOdtMode          | * | 0x01 | 0x00

  # !BSF NAME:{PerBankRefresh}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Control of Per Bank Refresh feature for LPDDR DRAMs}
  gPlatformFspPkgTokenSpaceGuid.PerBankRefresh           | * | 0x01 | 0x1

  # !BSF NAME:{Mimic WC display pattern in IPQ}
  # !BSF TYPE:{Combo} OPTION:{1:1, 3:3, 0xf:0xf, 0:Auto}
  # !BSF HELP:{Using for Disable/Enable Mimic WC display pattern in IPQ: 0:Disable, 1:Enable 1 ACT resources usage, 3:Enable 2 ACT resources usage, 3:Enable 3 ACT resources usage,0xf: Enable 4 ACT resources usage}
  gPlatformFspPkgTokenSpaceGuid.MimicWcDisaplayInIpq        | * | 0x01 | 0

  # !BSF NAME:{Fake SAGV}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Fake SAGV: 0:Disabled, 1:Enabled}
  gPlatformFspPkgTokenSpaceGuid.FakeSagv                  | * | 0x04 | 0

  # !BSF NAME:{Lock DPR register}
  # !BSF TYPE:{Combo} OPTION:{0:Platform POR, 1: Enable, 2: Disable}
  # !BSF HELP:{Lock DPR register. <b>0: Platform POR </b>; 1: Enable; 2: Disable}
  gPlatformFspPkgTokenSpaceGuid.DprLock                  | * | 0x04 | 0

  # !BSF NAME:{Board Stack Up}
  # !BSF TYPE:{Combo} OPTION:{0:Typical, 1:Freq Limited}
  # !BSF HELP:{Board Stack Up: <b>0=Typical</b>, 1=Freq Limited}
  gPlatformFspPkgTokenSpaceGuid.BoardStackUp                  | * | 0x01 | 0

  # !BSF NAME:{PPR ForceRepair}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When Eanble, PPR will force repair some rows many times (90)}
  gPlatformFspPkgTokenSpaceGuid.PprForceRepair                  | * | 0x01 | 0

  # !BSF NAME:{PPR Repair Bank}  TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{PPR repair Bank: User chooses to force repair specifc address }
  gPlatformFspPkgTokenSpaceGuid.PprRepairBank                  | * | 0x01 | 0

  # !BSF NAME:{Board Topology}
  # !BSF TYPE:{Combo} OPTION:{0:Daisy Chain, 1:Tee}
  # !BSF HELP:{Board Topology: <b>0=Daisy Chain</b>, 1=Tee.}
  gPlatformFspPkgTokenSpaceGuid.BoardTopology                  | * | 0x01 | 0

  # !BSF NAME:{SubCh Hash Interleaved Bit}
  # !BSF TYPE:{Combo} OPTION:{0:BIT6, 1:BIT7, 2:BIT8, 3:BIT9, 4:BIT10, 5:BIT11, 6:BIT12, 7:BIT13}
  # !BSF HELP:{Select the MC Enhanced Channel interleave bit, to set different address bit for sub channel selection than bit-6}
  gPlatformFspPkgTokenSpaceGuid.SubChHashInterleaveBit     | * | 0x01 | 0x03
  # Added reserved space   UnusedUpdSpace40
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd40                 | * | 0x1 | 0x00

  # !BSF NAME:{SubCh Hash Mask}
  # !BSF TYPE:{EditNum, HEX, (0x0000, 0x3FFF)}
  # !BSF HELP:{Set the BIT(s) to be included in the XOR function. NOTE BIT mask corresponds to BITS [19:6] Default is 0x834}
  gPlatformFspPkgTokenSpaceGuid.SubChHashMask              | * | 0x02 | 0x838

  # !BSF NAME:{Force CKD in Bypass Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Force CKD in Bypass Mode}
  gPlatformFspPkgTokenSpaceGuid.ForceCkdBypass           | * | 0x01 | 0x00

  # Added reserved space   UnusedUpdSpace41[3]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd41                 | * | 0x3 | {0x00}

  # !BSF NAME:{Disable Zq}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Zq Calibration: 0:Enabled, 1:Disabled}
  gPlatformFspPkgTokenSpaceGuid.DisableZq                 | * | 0x04 | 0

  # !BSF NAME:{Replicate SAGV}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Replicate SAGV: 0:Disabled, 1:Enabled}
  gPlatformFspPkgTokenSpaceGuid.ReplicateSagv             | * | 0x04 | 0

  # !BSF NAME:{Adjust wck mode}
  # !BSF TYPE:{Combo} OPTION:{0:safe mode, 1:manual mode, 2:dynamic mode, 3:Default}
  # !BSF HELP:{Adjust wck mode: 0:safe mode, 1:manual mode, 2:dynamic mode, 3:Default}
  gPlatformFspPkgTokenSpaceGuid.AdjustWckMode             | * | 0x01 | 0x03

  # !BSF NAME:{Control MC/PMA telemetry}
  # !BSF TYPE:{Combo} OPTION:{0: Default, 1: Enable, 2: Disable}
  # !BSF HELP:{Control MC/PMA telemetry: 0: Default, 1: Enable, 2: Disable}
  gPlatformFspPkgTokenSpaceGuid.TelemetryControl          | * | 0x01 | 0

  # !BSF NAME:{PHclk\Qclk SPINE gating Control}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{PHclk\Qclk SPINE gating Control: 0:Disabled, 1:Enabled}
  gPlatformFspPkgTokenSpaceGuid.SpineAndPhclkGateControl  | * | 0x01 | 0

  # !BSF NAME:{SpineGating per lpmode}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xF)}
  # !BSF HELP:{SpineGatePerLpmode[0]:Lpmode0.5, SpineGatePerLpmode[1]:Lpmode2, SpineGatePerLpmode[2]:Lpmode3, SpineGatePerLpmode[3]:Lpmode4}
  gPlatformFspPkgTokenSpaceGuid.SpineGatePerLpmode        | * | 0x01 | 0

  # !BSF NAME:{PhClkGating control per lpmode}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x1F)}
  # !BSF HELP:{PhclkGatePerLpmode[0]:Lpmode0.5, PhclkGatePerLpmode[1]:Lpmode1, PhclkGatePerLpmode[2]:Lpmode2, PhclkGatePerLpmode[3]:Lpmode3, PhclkGatePerLpmode[4]:Lpmode4}
  gPlatformFspPkgTokenSpaceGuid.PhclkGatePerLpmode        | * | 0x01 | 0

  # !BSF NAME:{DFI Control after cold boot} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Disable Switch DFI Control to MC after cold boot: 0(Default)=switch DFI to MC, 1=Keep with PHY/MPTU}
  gPlatformFspPkgTokenSpaceGuid.DisableSwitchDfiToMc      | * | 0x01 | 0x00

  # !BSF NAME:{Enable/Disable SmbusPostcode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Disable (Default): Postcode via Port80, Enable: Postcode via Smbus}
  gPlatformFspPkgTokenSpaceGuid.SmbusPostCodeEnable       | * | 0x01 | 0x0

  # !BSF NAME:{SmbusPostcode Address}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Slave address for Smbus postcode device}
  gPlatformFspPkgTokenSpaceGuid.SmbusPostCodeAddress      | * | 0x01 | 0x38

  # !BSF NAME:{SmbusPostcode Command}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Command value for Smbus postcode device}
  gPlatformFspPkgTokenSpaceGuid.SmbusPostCodeCommand      | * | 0x01 | 0x20

  # !BSF NAME:{Channel to CKD QCK Mapping} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !HDR STRUCT:{UINT8}
  # !BSF HELP:{Specify Channel to CKD QCK Mapping for CH0D0/CH0D1/CH1D0&CH1D1}
  gPlatformFspPkgTokenSpaceGuid.ChannelToCkdQckMapping       | * | 0x8 | { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }

  # !BSF NAME:{DDRIO Clock to CKD DIMM} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !HDR STRUCT:{UINT8}
  # !BSF HELP:{Specify DDRIO Clock to CKD DIMM for CH0D0/CH0D1/CH1D0&CH1D1}
  gPlatformFspPkgTokenSpaceGuid.PhyClockToCkdDimm            | * | 0x8 | { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
  # Added reserved space   UnusedUpdSpace42[8]
  gPlatformFspPkgTokenSpaceGuid.FspmUpdRsvd42                | * | 0x8 | {0x00}

  # !HDR EMBED:{FSP_M_CONFIG:FspmConfig:END}
  gPlatformFspPkgTokenSpaceGuid.ReservedFspmUpd             | * | 0x03 | {0x00}

  #
  # MRC Block End
  #

  #
  # SA Pre-Mem Block End
  #


  gPlatformFspPkgTokenSpaceGuid.FspmRsvd3834                | * | 0x06 | {0x00}

  # Note please keep "UpdTerminator" at the end of each UPD region.
  # The tool will use this field to determine the actual end of the UPD data
  # structure.

  gPlatformFspPkgTokenSpaceGuid.UpdTerminator               | * | 0x02 | 0x55AA
