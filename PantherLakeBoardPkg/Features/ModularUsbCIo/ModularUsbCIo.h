/** @file
  Modular TypeC IO Header

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

**/

#ifndef _MODULAR_TC_IO_H_
#define _MODULAR_TC_IO_H_

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/EcTcssLib.h>
#include <Library/PeiMeLib.h>
#include <Library/HobLib.h>
#include <Library/PlatformUsbConfigLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PmcLib.h>
#include <Library/PcdLib.h>
#include <MkhiMsgs.h>
#include <PlatformBoardConfig.h>
#include <PchResetPlatformSpecific.h>
#include <TcssPeiPreMemConfig.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <IGpuConfig.h>
#include <BoardSaConfigPreMem.h>

#define MODULAR_IO_GLOBAL_RESET_MASK      BIT0

#define MODULAR_IO_PORT1                  0
#define MODULAR_IO_PORT2                  1
#define MODULAR_IO_PORT3                  2
#define MODULAR_IO_PORT4                  3
#define MAX_MODULAR_IO_PORT_INDEX         4

#define DDI_PORT_B_CONFIG_OFFSET          1
#define TCP0_DDI_HPD_CONFIG_OFFSET        5

//
// GOP_CONFIG_DRIVER_HOB ModuleTypeCId
//
#define MODULE_IO_VBT_EDP                 (BIT3)
#define MODULE_IO_VBT_DP                  (BIT0 | BIT3)
#define MODULE_IO_VBT_HDMI                (BIT1 | BIT3)

#define MODULAR_IO_CONNECTOR_TYPE_USBA    0x1
#define MODULAR_IO_CONNECTOR_TYPE_USBC    0x2
#define MODULAR_IO_CONNECTOR_TYPE_HDMI    0x3
#define MODULAR_IO_CONNECTOR_TYPE_DP      0x4
#define MODULAR_IO_CONNECTOR_TYPE_EDP     0x5

#define CONVERT_EC_CONFIG_TO_CSE_CONFIG(PortNum)                                                                      \
{                                                                                                                     \
  CseModularIoConfigPtr->Port##PortNum##Change = EcModularIoConfig->PortConfig[PortIndex].Fields.PortPresent;         \
  switch (EcModularIoConfig->PortConfig[PortIndex].Fields.ConnectorType) {                                            \
    case MODULAR_IO_CONNECTOR_TYPE_USBC:                                                                              \
      CseModularIoConfigPtr->Port##PortNum##Config = (EcModularIoConfig->PortConfig[PortIndex].Fields.TbtStatus == 1) \
                                                      ? MODULAR_IO_USB4_40G : MODULAR_IO_USB_DP_ALT;                  \
      break;                                                                                                          \
    case MODULAR_IO_CONNECTOR_TYPE_USBA:                                                                              \
      CseModularIoConfigPtr->Port##PortNum##Config = MODULAR_IO_USB4_40G;                                             \
      break;                                                                                                          \
    case MODULAR_IO_CONNECTOR_TYPE_DP:                                                                                \
      CseModularIoConfigPtr->Port##PortNum##Config = MODULAR_IO_DP;                                                   \
      break;                                                                                                          \
    case MODULAR_IO_CONNECTOR_TYPE_HDMI:                                                                              \
      CseModularIoConfigPtr->Port##PortNum##Config = MODULAR_IO_HDMI;                                                 \
      break;                                                                                                          \
    case MODULAR_IO_CONNECTOR_TYPE_EDP:                                                                               \
      if (UsbCConnectorHobPtr->UsbCConnectorBoardConfig[ConnectorIndex].EdpModularIoSupported == 1) {                 \
        CseModularIoConfigPtr->Port##PortNum##Config = MODULAR_IO_EDP;                                                \
      } else {                                                                                                        \
        DEBUG ((DEBUG_ERROR, "%a: TCSS USBC port%d doesn't support eDP!!\n", __FUNCTION__, PortNum));                 \
        CseModularIoConfigPtr->Port##PortNum##Change = 0;                                                             \
      }                                                                                                               \
      break;                                                                                                          \
    default:                                                                                                          \
      DEBUG ((DEBUG_WARN, "%a: Unsupported modular IO port config (0x%04x) at connector%d!!\n", __FUNCTION__,         \
              EcModularIoConfig->PortConfig[PortIndex].Data16, ConnectorIndex));                                      \
      CseModularIoConfigPtr->Port##PortNum##Change = 0;                                                               \
      break;                                                                                                          \
  }                                                                                                                   \
}

#define UPDATE_USB_CONNECTOR_HOB(ConnectorConnectableValue, ConnectorTypeValue)                                 \
{                                                                                                               \
  UsbConnectorHobPtr->UsbConnectorBoardConfig[ConnectorIndex].ConnectorConnectable = ConnectorConnectableValue; \
  UsbConnectorHobPtr->UsbConnectorBoardConfig[ConnectorIndex].ConnectorType        = ConnectorTypeValue;        \
}

#define UPDATE_USBC_CONNECTOR_HOB(RetimerCountValue,                                                          \
                                  PcieSupportedValue,                                                         \
                                  DpAltModeSupportedValue,                                                    \
                                  Usb4SupportedValue,                                                         \
                                  Tbt3SupportedValue)                                                         \
{                                                                                                             \
  UsbCConnectorHobPtr->UsbCConnectorBoardConfig[ConnectorIndex].RetimerCount       = RetimerCountValue;       \
  UsbCConnectorHobPtr->UsbCConnectorBoardConfig[ConnectorIndex].PcieSupported      = PcieSupportedValue;      \
  UsbCConnectorHobPtr->UsbCConnectorBoardConfig[ConnectorIndex].DpAltModeSupported = DpAltModeSupportedValue; \
  UsbCConnectorHobPtr->UsbCConnectorBoardConfig[ConnectorIndex].Usb4Supported      = Usb4SupportedValue;      \
  UsbCConnectorHobPtr->UsbCConnectorBoardConfig[ConnectorIndex].Tbt3Supported      = Tbt3SupportedValue;      \
}

#define UPDATE_USBC_DP_ATTRIBUTE(DpType)                                                             \
{                                                                                                    \
  UsbCConnectorHobPtr->UsbCConnectorBoardConfig[ConnectorIndex].AuxDpMode          = (DpType & 0x3); \
}

#define UPDATE_USB_OC_PIN(OcType, OcPin)                                                     \
{                                                                                            \
  UsbConnectorHobPtr->UsbConnectorBoardConfig[ConnectorIndex].UsbOcPinType         = OcType; \
  UsbConnectorHobPtr->UsbConnectorBoardConfig[ConnectorIndex].UsbOcPin             = OcPin;  \
}

typedef union {
  struct {
    UINT16 PortPresent      : 1;
    UINT16 PortType         : 1;
    UINT16 ConnectorType    : 4;
    UINT16 RetimerStatus    : 2;
    UINT16 PdControllerType : 3;
    UINT16 TbtStatus        : 1;
    UINT16 DataSpeed        : 3;
    UINT16 Reserved         : 1;
  } Fields;
  UINT16 Data16;
} EC_MODULAR_IO_PORT_CONFIG;

typedef union {
  EC_MODULAR_IO_PORT_CONFIG PortConfig[4];
  UINT64                    Data64;
} EC_MODULAR_IO_CONFIG;

typedef struct {
  UINT8   DdiHpd;
  UINT8   DdiDdc;
} DDI_CONFIG;

#endif //_MODULAR_TC_IO_H_
