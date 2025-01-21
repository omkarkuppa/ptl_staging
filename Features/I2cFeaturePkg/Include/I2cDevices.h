/**@file
  I2C Devices ACPI Info driver

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

@par Specification Reference:
**/

#define I2C_ENABLE_BYTE_OFFSET                             0x00
#define END_POINT_PRESENT_BYTE_OFFSET                      0x01
#define TOUCH_PAD_INTERRUPT_GPIO_DWORD_OFFSET              0x02
#define TOUCH_PAD_INTERRUPT_POLARITY_BYTE_OFFSET           0x06
#define TOUCH_PANEL_INTERRUPT_GPIO_DWORD_OFFSET            0x07
#define TOUCH_PANEL_INTERRUPT_POLARITY_BYTE_OFFSET         0x0B
#define TOUCH_PANEL_POWER_GPIO_DWORD_OFFSET                0x0C
#define TOUCH_PANEL_POWER_POLARITY_BYTE_OFFSET             0x10
#define TOUCH_PANEL_RESET_GPIO_DWORD_OFFSET                0x11
#define TOUCH_PANEL_RESET_POLARITY_BYTE_OFFSET             0x15
#define I2C_BUFFER_SIZE                                    0x16
#define MAX_I2C_ROOTPORT_ENTRY_SUPPORT                        8
#define I2C_BUFFER_START_LOCATION                            10

typedef struct {
  UINT8    TouchPanelType;
  UINT8    TouchPanelIrqMode;
  UINT8    TouchPanelBusAddress;
  UINT8    TouchPanelSpeed;
  UINT16   TouchPanelHidAddress;
  UINT32   TouchpanelIrqGpio;
  UINT32   Touchpanel1IrqGpio;

}TOUCH_PANEL;

typedef struct {
  UINT8    TouchPadType;
  UINT8    TouchPadIrqMode;
  UINT8    TouchPadBusAddress;
  UINT8    TouchPadSpeed;
  UINT16   TouchPadHidAddress;
  UINT32   TouchpadIrqGpio;
}TOUCH_PAD;

typedef struct {
  TOUCH_PANEL  TouchPanelNvsVar;
  TOUCH_PAD    TouchPadNvsVar;
}I2C_DEVICES;

typedef struct  {
  UINT32     TouchpanelPwrGpio;
  UINT8      TouchpanelPwrGpioPolarity;
  UINT32     TouchPanelResetGpio;
  UINT8      TouchPanelResetGpioPolarity;
  UINT32     TouchPanelInterruptGpio;
  UINT8      TouchPanelInterruptGpioPolarity;
} I2C_ACPI_TOUCH_PANEL;

typedef struct  {
  UINT32     TouchPadInterruptGpio;
  UINT8      TouchPadInterruptGpioPolarity;
} I2C_ACPI_TOUCH_PAD;

typedef struct  {
  UINT8                 I2cControllerEnabled;
  UINT8                 I2cControllerTouchIndex;
  UINT8                 I2cDeviceType;
  I2C_ACPI_TOUCH_PAD    TouchPadInfo;
  I2C_ACPI_TOUCH_PANEL  TouchPanelInfo;
} I2C_ACPI_DEV;

typedef
EFI_STATUS
(EFIAPI *I2C_DEVICES_CONFIG) (
  UINT8                 Count,
  I2C_ACPI_DEV          *I2cBoardInfo,
  UINT8                  Rtd3Support,
  I2C_DEVICES            *I2cDevices
  );


struct _I2C_DEVICES_CONFIG_PROTOCOL {
  I2C_DEVICES_CONFIG   I2CBoardData;
  };

typedef struct _I2C_DEVICES_CONFIG_PROTOCOL I2C_DEVICES_CONFIG_PROTOCOL;

extern EFI_GUID gI2cDeviceInfoProtocolGuid;

#define GET_BUFFER_OFFSET_(INDEX_1, BYTE_OFFSET) \
  Add (Multiply (I2C_BUFFER_SIZE, INDEX_1), BYTE_OFFSET)

#define GET_BUFFER_OFFSET(INDEX_1, BYTE_OFFSET) GET_BUFFER_OFFSET_(INDEX_1, BYTE_OFFSET)

#define PRINT_VAR_INDEX_( VAR, INDEX_2) \
  ADBG ( Concatenate (Concatenate ( #VAR, #INDEX_2), Concatenate ( "0x", ToHexString (VAR##INDEX_2))))

#define PRINT_VAR_INDEX( VAR, INDEX_2) PRINT_VAR_INDEX_ ( VAR, INDEX_2) \


#define I2C_ROOTPORT(I2C_INDEX,INDEX) \
  CreateByteField (I2BU, GET_BUFFER_OFFSET ( I2C_INDEX, I2C_ENABLE_BYTE_OFFSET),KN ## INDEX) \
  PRINT_VAR_INDEX ( KN, INDEX) \
  CreateByteField (I2BU, GET_BUFFER_OFFSET ( I2C_INDEX, END_POINT_PRESENT_BYTE_OFFSET),EE ## INDEX) \
  PRINT_VAR_INDEX ( EE, INDEX) \
  CreateDwordField (I2BU, GET_BUFFER_OFFSET ( I2C_INDEX, TOUCH_PAD_INTERRUPT_GPIO_DWORD_OFFSET),GG ## INDEX) \
  PRINT_VAR_INDEX ( GG, INDEX) \
  CreateByteField (I2BU, GET_BUFFER_OFFSET ( I2C_INDEX, TOUCH_PAD_INTERRUPT_POLARITY_BYTE_OFFSET),GP ## INDEX) \
  PRINT_VAR_INDEX ( GP, INDEX) \
  CreateDwordField (I2BU, GET_BUFFER_OFFSET ( I2C_INDEX, TOUCH_PANEL_POWER_GPIO_DWORD_OFFSET),HG ## INDEX) \
  PRINT_VAR_INDEX ( HG, INDEX) \
  CreateByteField (I2BU, GET_BUFFER_OFFSET ( I2C_INDEX, TOUCH_PANEL_POWER_POLARITY_BYTE_OFFSET),HP  ## INDEX ) \
  PRINT_VAR_INDEX ( HP, INDEX) \
  CreateDwordField (I2BU, GET_BUFFER_OFFSET ( I2C_INDEX, TOUCH_PANEL_RESET_GPIO_DWORD_OFFSET),IG ## INDEX) \
  PRINT_VAR_INDEX ( IG, INDEX) \
  CreateByteField (I2BU, GET_BUFFER_OFFSET ( I2C_INDEX, TOUCH_PANEL_RESET_POLARITY_BYTE_OFFSET),IP  ## INDEX ) \
  PRINT_VAR_INDEX ( IP, INDEX) \
  CreateDwordField (I2BU, GET_BUFFER_OFFSET ( I2C_INDEX, TOUCH_PANEL_INTERRUPT_GPIO_DWORD_OFFSET),JG ## INDEX) \
  PRINT_VAR_INDEX ( JG, INDEX) \
  CreateByteField (I2BU, GET_BUFFER_OFFSET ( I2C_INDEX, TOUCH_PANEL_INTERRUPT_POLARITY_BYTE_OFFSET),JP ## INDEX) \
  PRINT_VAR_INDEX ( JP, INDEX) \
  If (KN ## INDEX) { \
    If (CondRefOf ( SCOPE_I2C.I2C ## I2C_INDEX)) { \
      Scope (SCOPE_I2C.I2C ## I2C_INDEX) { \
        \
        Store (KN ## INDEX,I2EN) \
        Name (EPDO,0) \
        Store (EE ## INDEX,EPDO) \
        Name (PIRT, Package() {0, 0}) \
        Store (GG ## INDEX, Index(PIRT, 0)) \
        Store (GP ## INDEX, Index(PIRT, 1)) \
        Name (TPWR, Package() {0, 0}) \
        Store (HG ## INDEX, Index(TPWR, 0)) \
        Store (HP ## INDEX, Index(TPWR, 1)) \
        \
        Name (TRST, Package() {0, 0}) \
        Store (IG ## INDEX, Index(TRST, 0)) \
        Store (IP ## INDEX, Index(TRST, 1)) \
        \
        Name (TIRT, Package() {0, 0}) \
        Store (JG ## INDEX, Index(TIRT, 0)) \
        Store (JP ## INDEX, Index(TIRT, 1))

#define I2C_ROOTPORT_END \
      } \
    } \
  }
