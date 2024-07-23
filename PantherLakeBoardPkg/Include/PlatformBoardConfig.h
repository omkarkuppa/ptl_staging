/** @file
  Header file for Platform Boards Configurations.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#ifndef _PLATFORM_BOARD_CONFIG_H_
#define _PLATFORM_BOARD_CONFIG_H_

#include <ConfigBlock.h>
#include <Usb2PhyConfig.h>
#include <MemoryConfig.h>
#include <TcssInfo.h>
#include "../../PantherLakeOpenBoardPkg/Include/PlatformOpenBoardConfig.h" // @todo - Need to be corrected once no conflict between OpenBoardPkg.dec and BoardPkg.dec.

//
// VendorGuid: {89ED4FC1-4234-47BC-9081-7A6510A5E8A9}
// Name:       RmtRun
// Attributes: NV+BS+RT
// GetVariable value in Data parameter: 0x0 (Dont do any operation); 0x1 (update policy to Trigger Rmt Test)
// SetVariable value in Data parameter: 0x0 (Clear Rmt Test request );
//

#define RMT_RUN_REQUEST_CONTROL_GUID \
  { \
    0x89ED4FC1, 0x4234, 0x47BC, {0x90, 0x81, 0x7A, 0x65, 0x10, 0xA5, 0xE8, 0xA9} \
  }

#define RMT_RUN_REQUEST_CONTROL_NAME  L"RmtRun"

#define MAX_GPIO_PINS             150

#define MAX_PRE_MEM_GPIO_PINS     20

#define MAX_PMAX_DEVICE_STRING_LENGTH         31

#define BOARD_CONFIG_BLOCK_PEI_PREMEM_VERSION  0x00000001
#define BOARD_CONFIG_BLOCK_PEI_POSTMEM_VERSION 0x00000001
#define BOARD_CONFIG_BLOCK_DXE_VERSION 0x00000001
#define BOARD_NO_BATTERY_SUPPORT 0
#define BOARD_REAL_BATTERY_SUPPORTED BIT0
#define BOARD_VIRTUAL_BATTERY_SUPPORTED BIT1

//
// ACPI table information used to update tables thru PCD
//

#define ACPI_OEM_TABLE_ID_PTL                 0x4C5450           // PTL

//
// GPIO states mapping
//
#define PIN_GPIO_ACTIVE_LOW 0
#define PIN_GPIO_ACTIVE_HIGH 1

//
// USB Port
//
#define PORT1            0
#define PORT2            1
#define PORT3            2
#define PORT4            3
#define PORT5            4
#define PORT6            5
#define PORT7            6
#define PORT8            7
#define PORT9            8
#define PORT10           9
#define PORT11           10
#define PORT12           11
#define PORT13           12
#define PORT14           13
#define PORT15           14
#define PORT16           15

//
// USB2 Controller Type
//
#define PCH_USB2         1
#define DTBT1_USB2       3
#define DTBT2_USB2       4

//
// USB3 Controller Type
//
#define PCH_USB3         1
#define TCSS_USB3        2
#define DTBT1_USB3       3
#define DTBT2_USB3       4

//
// USB Connector Connectable
//
#define UNCONNECTABLE    0
#define CONNECTABLE      1

//
// USB Connector Type
//
#define USB_TYPE_A                0
#define USB_TYPE_C_USB2_ONLY      8
#define USB_TYPE_C                9
#define USB_TYPE_C_WITHOUT_SWITCH 10

//
// USB Connector Visibility
//
#define INVISIBLE        0
#define VISIBLE          1

//
// USB OC Pin Type
//
#define PHYSICAL_OC      1
#define VW_OC            2
#define USB_OC_MAX_TYPE  2

//
// USB OC Pin
//
#define OC_PIN0          0
#define OC_PIN1          1
#define OC_PIN2          2
#define OC_PIN3          3
#define OC_PIN4          4
#define OC_PIN5          5
#define OC_PIN6          6
#define OC_PIN7          7

//
// Modular IO supported
//
#define FIXED_IO         0
#define MODULAR_IO       1

//
// Retimer Count
//
#define RETIMERLESS      0
#define ONE_RETIMER      1
#define TWO_RETIMERS     2

//
// PCI Express Tunneling supported
//
#define PCIE_TUNNELING_UNSUPPORTED 0
#define PCIE_TUNNELING_SUPPORTED   1

//
// DisplayPort Alternative Mode (DP Alt Mode) supported
//
#define DP_ALT_MODE_UNSUPPORTED 0
#define DP_ALT_MODE_SUPPORTED   1

//
// USB4 supported
//
#define USB4_UNSUPPORTED 0
#define USB4_SUPPORTED   1

//
// Thunderbolt 3 Alternate Mode (TBT3) supported
//
#define TBT3_UNSUPPORTED 0
#define TBT3_SUPPORTED   1

//
// Thunderbolt Controller Type
//
#define ITBT_DMA         1
#define DTBT_NHI         2

//
// Thunderbolt Controller#
//
#define TBT_CONTROLLER1  0
#define TBT_CONTROLLER2  1

//
// PD#
//
#define USBC_PD1         1
#define USBC_PD2         2
#define USBC_PD3         3
#define USBC_PD4         4

//
// Port# on iTBT DMA controller
//
#define DMA_PORT1        0
#define DMA_PORT2        1

//
// PCIe Root Port Type for dTBT
//
#define PCH_RP           1
#define CPU_RP           2

//
// PCIe Root Port# for dTBT
//
#define PCIE_RP01        1
#define PCIE_RP02        2
#define PCIE_RP03        3
#define PCIE_RP04        4
#define PCIE_RP05        5
#define PCIE_RP06        6
#define PCIE_RP07        7
#define PCIE_RP08        8
#define PCIE_RP09        9
#define PCIE_RP10        10
#define PCIE_RP11        11
#define PCIE_RP12        12
#define PCIE_RP13        13
#define PCIE_RP14        14
#define PCIE_RP15        15
#define PCIE_RP16        16

typedef enum {
  BoardGpioTypePch,
  BoardGpioTypeExpander,
  BoardGpioTypeNotSupported = 0xFF
} BOARD_GPIO_TYPE;

#ifndef IO_EXPANDER_DISABLED
#define IO_EXPANDER_DISABLED      0xFF
#endif

#define HDAUDIO_VERB_TABLE_DATABASE_INIT(...) \
{ \
  (sizeof((HDAUDIO_VERB_TABLE*[]){__VA_ARGS__})/sizeof(HDAUDIO_VERB_TABLE*)), \
  { __VA_ARGS__ } \
}

// Do not change the encoding. It must correspond with PCH_PCIE_CLOCK_USAGE from PCH RC.
#define FREE_RUNNING 0x80
#define LAN_CLOCK    0x70
#define PCIE_PEG     0x40
#define PCIE_PCH     0x00

#pragma pack(1)
typedef struct {
  CONFIG_BLOCK_HEADER  Header;               ///< Offset 0-27 Config Block Header
} BOARD_CONFIG_BLOCK;

typedef struct {
  UINT8   GpioSupport;
  UINT32  WakeGpioNo;
  UINT8   HoldRstExpanderNo;
  UINT32  HoldRstGpioNo;
  BOOLEAN HoldRstActive;
  UINT8   PwrEnableExpanderNo;
  UINT32  PwrEnableGpioNo;
  BOOLEAN PwrEnableActive;
} SWITCH_GRAPHIC_GPIO;

/**
  This is a helper structure that's used to updating Config Block data according to board design
**/
typedef struct  {
  UINT8                 Size;
  USB2_PHY_PARAMETERS   Data[];
} USB2_PHY_TABLE;

// Sys G3 State
typedef struct _ {
  EFI_HOB_GUID_TYPE      EfiHobGuidType;
  UINT8                  SysG3Status;          ///< Indicate whether the previous system state is G3 or not.
} SYS_G3_STATE_HOB;

//SBC VPD GPIO PAD
typedef struct {
  UINT32  GpioPad;
} VPD_GPIO_PAD;

//SBC VPD helper structure that's used to updating Config Block data according to board design
typedef struct {
  UINT8                 Size;
  USB2_PHY_PARAMETERS   Data[16];
} VPD_USB2_PHY_TABLE;

//SBC VPD helper structure that's used to update TypeC Port Mapping
typedef struct {
  UINT8   Supported;
  UINT8   PchPort;
  UINT8   Properties;
} TYPE_C_PORT_MAPPING;

//SBC VPD helper structure for a CHILD_STRUCT (used for all the displays).
typedef struct {
  UINT8   Field;          ///< Specifies the display config field
  UINT8   Value;          ///< Specifies the value for config field
} DEVICE_CONFIG_DATA;

typedef struct {
  UINT8   Index;
  UINT16  DeviceClass;          ///< Indicates the class of display device.
  DEVICE_CONFIG_DATA DeviceConfigData[16];
} DISPLAY_DEVICE_CONFIG_TABLE;

typedef struct {
  UINT8   Size;
  DISPLAY_DEVICE_CONFIG_TABLE DisplayDeviceConfigTable[16];
} VPD_GOP_CONFIG;

// Power meter structure
#define MAX_PM_DEVICE_STRING_LENGTH               31
#define POWERMETER_MAX_DEVICES                     8
typedef struct  {
  CHAR8    RailName[MAX_PM_DEVICE_STRING_LENGTH];          // Rail Name
  UINT16   ResistorValue;                                  // Rail Resistence value
} RAIL_DATA_PKG;

typedef struct  {
  UINT8         DeviceAddress;   // 7-bit Device Address
  RAIL_DATA_PKG RailDataPkg[4];  // Package info
} POWER_METER_CONFIG_PKG;

typedef struct  {
  UINT8   DeviceSize;               // No. of Power Meter Device
  UINT8   I2cBusNumber;             // I2C Bus Number
  POWER_METER_CONFIG_PKG PmConfigPkgArray[POWERMETER_MAX_DEVICES];
} POWER_METER_PKG;

/**
  TCSS Covert to USBA Configuration Structure
**/
typedef struct {
  UINT16  Port1Orientation   : 1; // Port1 Orientation, 0 - non-flipped, 1 - flipped
  UINT16  Port1PhysicalOcPin : 3; // Port1 Physical OC Pin#
  UINT16  Port2Orientation   : 1; // Port2 Orientation, 0 - non-flipped, 1 - flipped
  UINT16  Port2PhysicalOcPin : 3; // Port2 Physical OC Pin#
  UINT16  Port3Orientation   : 1; // Port3 Orientation, 0 - non-flipped, 1 - flipped
  UINT16  Port3PhysicalOcPin : 3; // Port3 Physical OC Pin#
  UINT16  Port4Orientation   : 1; // Port4 Orientation, 0 - non-flipped, 1 - flipped
  UINT16  Port4PhysicalOcPin : 3; // Port4 Physical OC Pin#
} TCSS_COV_USBA_BOARD_CONFIG;

//
// The macros below transfer USB OC pin to USB OCM register.
//
#define GET_USB2_OCM_REG(OcPinType, OcPin) (OcPinType == PHYSICAL_OC ? OcPin : OcPin + USB2_OCM_REG_OFFSET)
#define GET_USB3_OCM_REG(OcPinType, OcPin) OcPin

/**
  Retimer Flash Configuration Structure
**/
typedef struct {
  /**
  Cascaded Retimer Flash Index per USBC connector
  0x0 - Unsupported
  **/
  UINT8  CascadedRetimerFlashIndex   : 2;
  /**
  The first USBC connector index supported by the retimer flash.
  0 - USBC Connector1, 1 - USBC Connector2, 2 - USBC Connector3, 3 - USBC Connector4, 7 - Unsupported
  **/
  UINT8  FirstUsbCConnectorIndex     : 3;
  /**
  The second USBC connector index supported by the retimer flash.
  0 - USBC Connector1, 1 - USBC Connector2, 2 - USBC Connector3, 3 - USBC Connector4, 7 - Unsupported
  **/
  UINT8  SecondUsbCConnectorIndex    : 3;
} USBC_RETIMER_FLASH_BOARD_CONFIG;

/**
  USB Connector Configuration Structure
**/
typedef struct {
  /**
  USB Connector Connectable
  0 - NOT connectable, 1 - connectable
  **/
  UINT32 ConnectorConnectable  : 1;
  /**
  Connector Visibility
  0 - invisible, 1 - visible
  **/
  UINT32 ConnectorVisibility   : 1;
  /**
  USB Connector Type
  0x00: Type A connector
  0x01: Mini-AB connector
  0x02: ExpressCard
  0x03: USB 3 Standard-A connector
  0x04: USB 3 Standard-B connector
  0x05: USB 3 Micro-B connector
  0x06: USB 3 Micro-AB connector
  0x07: USB 3 Power-B connector
  0x08: USBC connector - USB2-only
  0x09: USBC connector - USB2 and SS with Switch
  0x0A: USBC connector - USB2 and SS without Switch
  0x0B - 0xFE: Reserved
  0xFF: Proprietary connector
  **/
  UINT32 ConnectorType         : 8;
  /**
  USB Overcurrent Pin Type
  0 - Unsupported, 1 - Physical, 2 - Virtual Wire
  **/
  UINT32 UsbOcPinType          : 2;
  /**
  USB Overcurrent Pin
  PhysicalOc1, PhysicalOc2, ..., PhysicalOcM, VirtualOc1, VirtualOc2, ..., VirtualOcN
  M: The number of the supported physical OC pins
  N: The number of the supported virtual OC pins
  **/
  UINT32 UsbOcPin              : 3;
  /**
  USB2 controller type which is used by USB connector.
  0 - Unsupported, 1 - PCH, 3 - dTBT1, 4 - dTBT2
  **/
  UINT32 Usb2Controller        : 3;
  /**
  USB2 port# which is used by USB connector.
  0 - port1, 1 - port2, 2 - port3 and etc.
  **/
  UINT32 Usb2PortNum           : 5;
  /**
  USB3 controller type which is used by USB connector.
  0 - Unsupported, 1 - PCH, 2 - TCSS, 3 - dTBT1, 4 - dTBT2
  **/
  UINT32 Usb3Controller        : 3;
  /**
  USB3 port# which is used by USB connector.
  0 - port1, 1 - port2, 2 - port3 and etc.
  **/
  UINT32 Usb3PortNum           : 5;
  /**
  Reserved bits for future extension
  **/
  UINT32 RsvdBits              : 1;
} USB_CONNECTOR_BOARD_CONFIG;

/**
  USBC Connector Configuration Structure
**/
typedef struct {
  /**
  Modular IO supported - Modular IO is supported or not
  0 - Modular IO is NOT supported (Fixed IO), 1 - Modular IO is supported
  **/
  UINT32 ModularIoSupported    : 1;
  /**
  eDP Modular IO supported - eDP Modular IO is supported or not
  0 - eDP Modular IO is NOT supported, 1 - eDP Modular IO is supported
  **/
  UINT32 EdpModularIoSupported : 1;
  /**
  Retimer Count - number of retimer devices present on the board
  between the Host Router and this Port (connector). The maximum value is 2
  (10b). If present, the retimer devices apply to USB4, USB3, and any Alternate
  Modes supported.
  **/
  UINT32 RetimerCount          : 2;
  /**
  PCI Express Tunneling supported. This bit is valid only for ports that
  support USB4 or TBT3. If PCI Express Tunneling for USB4 is not supported
  via the USB4 _OSC mechanism, then the value of this bit is indeterminate.
  **/
  UINT32 PcieSupported         : 1;
  /**
  DisplayPort Alternative Mode (DP Alt Mode) supported. This bit is
  required to be set for ports that support USB4 or Thunderbolt 3.
  **/
  UINT32 DpAltModeSupported    : 1;
  /**
  USB4 Supported.
  **/
  UINT32 Usb4Supported         : 1;
  /**
  Thunderbolt 3 Alternate Mode (TBT3) supported.
  **/
  UINT32 Tbt3Supported         : 1;
  /**
  PD#
  0 - Unsupported, 1 - PD1, 2 - PD2 and etc.
  **/
  UINT32 PdNum                 : 3;
  /**
  AUX DP Mode
  0 - Disabled, 1 - DP, 2 - HDMI, 3 - eDP
  **/
  UINT32 AuxDpMode             : 2;
  /**
  Thunderbolt Controller Type
  0 - Unsupported, 1 - iTBT DMA, 2 - dTBT NHI
  **/
  UINT32 TbtControllerType     : 2;
  /**
  Thunderbolt Controller#
  0 - Controller1, 1 - Controller2 and etc.
  **/
  UINT32 TbtControllerNum      : 2;
  /**
  Port# on iTBT DMA controller
  0 - port1, 1 - port2
  **/
  UINT32 DmaPortNum            : 1;
  /**
  PCIe Root Port Type for dTBT
  0 - Unsupported, 1 - PCH, 2 - CPU
  **/
  UINT32 PcieRootPortType      : 1;
  /**
  PCIe Root Port# for dTBT
  The RP# is same as the port number read from link capabilities register 0Ch
  of the root port controller.
  **/
  UINT32 PcieRootPortNum       : 8;
  /**
  Reserved bits for future extension
  **/
  UINT32 RsvdBits              : 5;
} USBC_CONNECTOR_BOARD_CONFIG;

#define MAX_USB_CONNECTOR_NUMBER   20
typedef struct {
  UINT8                       NumberOfUsbConnectors;
  USB_CONNECTOR_BOARD_CONFIG  UsbConnectorBoardConfig[MAX_USB_CONNECTOR_NUMBER];
} USB_CONNECTOR_HOB_DATA;

#define MAX_USBC_CONNECTOR_NUMBER  4
typedef struct {
  UINT8                       NumberOfUsbCConnectors;                               // The max number of USBC connectors supported by board including modular IO.
  UINT8                       NumberOfConnectableUsbCConnectors;                    // The number of connectable USBC connectors on board.
  UINT16                      ConnectableUsbCConnectorsMap;                         // Connectable USBC connectors map.
  USBC_CONNECTOR_BOARD_CONFIG UsbCConnectorBoardConfig[MAX_USBC_CONNECTOR_NUMBER];
} USBC_CONNECTOR_HOB_DATA;

#pragma pack()

#endif // _PLATFORM_BOARD_CONFIG_H_
