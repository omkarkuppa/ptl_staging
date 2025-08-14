## @file
#  Board description file initializes configuration (PCD) settings for the project.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
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
[Defines]
  PIN_GPIO_ACTIVE_HIGH            = 1
  PIN_GPIO_ACTIVE_LOW             = 0
  BOARD_REAL_BATTERY_SUPPORTED    = 1
  BOARD_VIRTUAL_BATTERY_SUPPORTED = 2
  BOARD_NO_BATTERY_SUPPORT        = 0

[Packages]
  PantherLakeBoardPkg/BoardPkg.dec

#####################################################################
# PTL simics Board ID: 0x31 (BoardIdPtlSimics)
#              SKU ID: 0x00010031 (SkuIdPtlHfpgaBase)
#####################################################################
[PcdsDynamicExVpd.common.SkuIdPtlHfpgaBase]
  gBoardModuleTokenSpaceGuid.VpdPcdTcssEnable|*|0
  #
  # *** IMPORTANT ***
  #   For all of the USB ports or native display connector over TCSS lanes on platform.
  #   All of the USBC connectors with PD or native display connector over TCSS lanes MUST be described at
  #   the beginning of the 2 tables with same sequence. Suggest listed sequence: USBC ports (from TCSS lanes,
  #   dTBT or standalone USBC) or native display connector over TCSS lanes, Type-A connectors and others.
  #
  #   Example:
  #     gBoardModuleTokenSpaceGuid.VpdPcdUsbConnector| * |{CODE(
  #     {
  #     // Connectable,   Visibility, ConnectorType, UsbOcPinType, UsbOcPin, Usb2Controller, Usb2PortNum, Usb3Controller, Usb3PortNum
  #       {CONNECTABLE,   VISIBLE,    0x09,          VW_OC,        OC_PIN0,  PCH_USB2,       PORT1,       TCSS_USB3,      PORT1}, // TCP0
  #       {UNCONNECTABLE, VISIBLE,    0xFF,          0,            0,        0,              0,           TCSS_USB3,      PORT2}, // Native HDMI over TCP1
  #       {UNCONNECTABLE, VISIBLE,    0xFF,          VW_OC,        OC_PIN3,  PCH_USB2,       PORT4,       TCSS_USB3,      PORT4}, // TCP3, default is HDMI (Modular IO)
  #       {CONNECTABLE,   VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT3,       TCSS_USB3,      PORT3}, // TCP2 converted TAP (Type-A)
  #       {CONNECTABLE,   VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT5,       PCH_USB3,       PORT1}, // TAP1
  #       {CONNECTABLE,   VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT6,       PCH_USB3,       PORT2}, // TAP2
  #       {CONNECTABLE,   INVISIBLE,  0x00,          0,            0,        PCH_USB2,       PORT7,       0,              0    }, // FPS
  #       {CONNECTABLE,   INVISIBLE,  0x00,          0,            0,        PCH_USB2,       PORT8,       0,              0    }, // M.2 WLAN
  #       {0x0}  // terminator
  #     })}
  #
  #   For all of the USBC connectors with PD or native display connector over TCSS lanes.
  #   The sequence MUST align with the ones described in VpdPcdUsbConnector table.
  #
  #     gBoardModuleTokenSpaceGuid.VpdPcdUsbCConnector| * |{CODE(
  #     {
  #     // ModularIoSupported, EdpModularIoSupported, RetimerCount, PcieSupported, DpAltModeSupported, Usb4Supported, Tbt3Supported, PdNum,    AuxDpMode, TbtControllerType, TbtControllerNum, DmaPortNum, PcieType, PcieRootPortNum
  #       {FIXED_IO,           0,                     1,            1,             1,                  1,             1,             USBC_PD1, 0,         ITBT_DMA,          TBT_CONTROLLER1,  DMA_PORT1,  0,        0}, // TCP0
  #       {FIXED_IO,           0,                     0,            0,             0,                  0,             0,             0,        2,         0,                 0,                0,          0,        0}, // Native HDMI over TCP1
  #       {MODULAR_IO,         0,                     0,            0,             0,                  0,             0,             USBC_PD2, 2,         ITBT_DMA,          TBT_CONTROLLER2,  DMA_PORT2,  0,        0}, // TCP3, default is HDMI (Modular IO)
  #       {0x0}  // terminator
  #     })}
  #
  gBoardModuleTokenSpaceGuid.VpdPcdUsbConnector|*|{CODE(
  {
  // Connectable, Visibility, ConnectorType, UsbOcPinType, UsbOcPin, Usb2Controller, Usb2PortNum, Usb3Controller, Usb3PortNum
    {CONNECTABLE, VISIBLE,    0x00,          0,            0,        PCH_USB2,       PORT1,       PCH_USB3,       PORT1}, // TAP1
    {CONNECTABLE, VISIBLE,    0x00,          0,            0,        PCH_USB2,       PORT2,       0,              0    }, // TAP2
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdUsbCConnector| * |{CODE(
  {
  // ModularIoSupported, EdpModularIoSupported, RetimerCount, PcieSupported, DpAltModeSupported, Usb4Supported, Tbt3Supported, PdNum,    AuxDpMode, TbtControllerType, TbtControllerNum, DmaPortNum, PcieType, PcieRootPortNum
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdTcssCovUsbA|*|{CODE(
  {
    0, // Port1 Orientation
    0, // Port1 Physical OC pin
    0, // Port2 Orientation
    0, // Port2 Physical OC pin
    0, // Port3 Orientation
    0, // Port3 Physical OC pin
    0, // Port4 Orientation
    0  // Port4 Physical OC pin
  })}

  # Each DIMM Slot Mechanical present bit map, bit 0 -> DIMM 0, bit 1 -> DIMM1, ...
  # if the bit is 1, the related DIMM slot is present.
  # 0x1 - For 1DPC memory DIMM config, 0x3 - For 2DPC memory DIMM config.
  gBoardModuleTokenSpaceGuid.VpdPcdMrcNoOfDimmSlot|*|{CODE({
    {
      // Controller-0
      {
        0x1, // DIMM0
        0x1, // DIMM0
        0x1, // DIMM0
        0x1  // DIMM0
      },
      // Controller-1
      {
        0x1, // DIMM0
        0x1, // DIMM0
        0x1, // DIMM0
        0x1  // DIMM0
      }
    }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdData|*|{CODE(
  {
  //
  // Set SPD to support SMB in RTL
  //
  0x1,
  {
    0x23,0x10,0x13,0x0e,0x19,0x29,0x10,0x00,0x00,0x00,0x00,0x03,0x0a,0x01,0x00,0x00,
    0x00,0x00,0x14,0x28,0x80,0x7d,0x15,0x00,0xb4,0x00,0xa0,0xb4,0xa0,0xe0,0x0b,0xf0,
    0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
  }
  })}

#####################################################################
# PTL simics Board ID: 0x31 (BoardIdPtlSimics)
#              SKU ID: 0x00020031 (SkuIdPtlHfpgaTcss)
#####################################################################
[PcdsDynamicExVpd.common.SkuIdPtlHfpgaTcss]
  gBoardModuleTokenSpaceGuid.VpdPcdTcssEnable|*|1
  #
  # *** IMPORTANT ***
  #   All of the USBC connectors with PD or native display connector over TCSS lanes MUST be described at the beginning of the 2 tables.
  #
  #   Example:
  #     gBoardModuleTokenSpaceGuid.VpdPcdUsbConnector| * |{CODE(
  #     {
  #     // Connectable,   Visibility, ConnectorType, UsbOcPinType, UsbOcPin, Usb2Controller, Usb2PortNum, Usb3Controller, Usb3PortNum
  #       {CONNECTABLE,   VISIBLE,    0x09,          VW_OC,        OC_PIN0,  PCH_USB2,       PORT1,       TCSS_USB3,      PORT1}, // TCP0
  #       {UNCONNECTABLE, VISIBLE,    0xFF,          0,            0,        0,              0,           TCSS_USB3,      PORT2}, // Native HDMI over TCP1
  #       {UNCONNECTABLE, VISIBLE,    0xFF,          VW_OC,        OC_PIN3,  PCH_USB2,       PORT4,       TCSS_USB3,      PORT4}, // TCP3, default is HDMI (Modular IO)
  #       {CONNECTABLE,   VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT3,       TCSS_USB3,      PORT3}, // TCP2 converted TAP
  #       {CONNECTABLE,   VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT5,       PCH_USB3,       PORT1}, // TAP1
  #       {CONNECTABLE,   VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT6,       PCH_USB3,       PORT2}, // TAP2
  #       {CONNECTABLE,   INVISIBLE,  0x00,          0,            0,        PCH_USB2,       PORT7,       0,              0    }, // FPS
  #       {CONNECTABLE,   INVISIBLE,  0x00,          0,            0,        PCH_USB2,       PORT8,       0,              0    }, // M.2 WLAN
  #       {0x0}  // terminator
  #     })}
  #
  #     gBoardModuleTokenSpaceGuid.VpdPcdUsbCConnector| * |{CODE(
  #     {
  #     // ModularIoSupported, EdpModularIoSupported, RetimerCount, PcieSupported, DpAltModeSupported, Usb4Supported, Tbt3Supported, PdNum,    AuxDpMode, TbtControllerType, TbtControllerNum, DmaPortNum, PcieType, PcieRootPortNum
  #       {FIXED_IO,           0,                     1,            1,             1,                  1,             1,             USBC_PD1, 0,         ITBT_DMA,          TBT_CONTROLLER1,  DMA_PORT1,  0,        0}, // TCP0
  #       {FIXED_IO,           0,                     0,            0,             0,                  0,             0,             0,        2,         0,                 0,                0,          0,        0}, // Native HDMI over TCP1
  #       {MODULAR_IO,         0,                     0,            0,             0,                  0,             0,             USBC_PD2, 2,         ITBT_DMA,          TBT_CONTROLLER2,  DMA_PORT2,  0,        0}, // TCP3, default is HDMI
  #       {0x0}  // terminator
  #     })}
  #
  gBoardModuleTokenSpaceGuid.VpdPcdUsbConnector|*|{CODE(
  {
  // Connectable, Visibility, ConnectorType, UsbOcPinType, UsbOcPin, Usb2Controller, Usb2PortNum, Usb3Controller, Usb3PortNum
    {CONNECTABLE, VISIBLE,    0x09,          0,            0,        0,              0,           TCSS_USB3,      PORT1}, // TCP0
    {CONNECTABLE, VISIBLE,    0x09,          0,            0,        0,              0,           TCSS_USB3,      PORT2}, // TCP1
    {CONNECTABLE, VISIBLE,    0x09,          0,            0,        0,              0,           TCSS_USB3,      PORT3}, // TCP2
    {CONNECTABLE, VISIBLE,    0x09,          0,            0,        0,              0,           TCSS_USB3,      PORT4}, // TCP3
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdUsbCConnector| * |{CODE(
  {
  // ModularIoSupported, EdpModularIoSupported, RetimerCount, PcieSupported, DpAltModeSupported, Usb4Supported, Tbt3Supported, PdNum,    AuxDpMode, TbtControllerType, TbtControllerNum, DmaPortNum, PcieType, PcieRootPortNum
    {MODULAR_IO,         1,                     1,            1,             1,                  1,             1,             USBC_PD1, 0,         ITBT_DMA,          TBT_CONTROLLER1,  DMA_PORT1,  0,        0}, // TCP0
    {MODULAR_IO,         0,                     1,            1,             1,                  1,             1,             USBC_PD2, 0,         ITBT_DMA,          TBT_CONTROLLER1,  DMA_PORT2,  0,        0}, // TCP1
    {MODULAR_IO,         0,                     1,            1,             1,                  1,             1,             USBC_PD3, 0,         ITBT_DMA,          TBT_CONTROLLER2,  DMA_PORT1,  0,        0}, // TCP2
    {MODULAR_IO,         0,                     1,            1,             1,                  1,             1,             USBC_PD4, 0,         ITBT_DMA,          TBT_CONTROLLER2,  DMA_PORT2,  0,        0}, // TCP3
    {0x0}  // terminator
  })}

