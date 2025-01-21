/** @file
  Usb Type-c Reitmer driver definitions.

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

#ifndef _USB_C_RETIMER_PROTOCOL_H_
#define _USB_C_RETIMER_PROTOCOL_H_

#include <Uefi.h>
#include <PiDxe.h>

// Macro Definition
#define    RETIMER_TO_PD_MAPPING_PCD_TOTAL_WIDTH    32
#define    RETIMER_TO_PD_MAPPING_PER_DEVICE_WIDTH   4
//
// Make Sure MAX_TBT_RETIMER_DEVICE must be equal to
// RETIMER_TO_PD_MAPPING_PCD_TOTAL_WIDTH / RETIMER_TO_PD_MAPPING_PER_DEVICE_WIDTH value.
// In this case it is 32/4 = 8.
//
#define    MAX_TBT_RETIMER_DEVICE                   8
#define    INVALID_DEVICE_MODE                      0xFF
#define    INVALID_PD_CONTROLLER_INDEX              0xFF
#define    ALL_PD_CONTROLLER                        0xFE
#define    RETIMER_DEVICE_AVAILABLE_BIT             BIT3
#define    PD_CONTROLLER_INDEX_BIT_MASK             (BIT0 | BIT1 | BIT2)

#define    RETIMER_FORCE_POWER_GPIO_HIGH            1
#define    RETIMER_FORCE_POWER_GPIO_LOW             0

//
// Define all PD Mode supported by EC
//
typedef enum {
  RetimerFwUpdateDisableMode = 0,
  RetimerFwUpdateEnableMode  = 1,
  InvalidRetimerFwUpdateMode
} PD_RETIMER_FW_UPDATE_MODE;

///
/// Forward declaration for the _USBC_RETIMER_PROTOCOL.
///
typedef struct _USBC_RETIMER_PROTOCOL              USBC_RETIMER_PROTOCOL;

///
/// Global ID for the ESRT_MANAGEMENT_PROTOCOL.  {B9AC2BD1-D450-4BEC-BD5B-192601119B84}
///

#define USBC_RETIMER_PROTOCOL_GUID \
{ \
    0xB9AC2BD1, 0xD450, 0x4BEC, { 0xBD, 0x5B, 0x19, 0x26, 0x1, 0x11, 0x9B, 0x84 } \
}
//EFI_GUID gUsbCRetimerProtocolGuid  = { 0xB9AC2BD1, 0xD450, 0x4BEC, { 0xBD, 0x5B, 0x19, 0x26, 0x1, 0x11, 0x9B, 0x84 } };

extern EFI_GUID gUsbCRetimerProtocolGuid;

//
// This variable is the indicator about ReadyToBoot() is signaled or not.
// The gUsbCRetimerProtocolGuid will not be used by any UEFI Application running in EDK Shell.
//
#define USBC_RETIMER_PROTOCOL_CAPABILITY_NAME  L"UsbCRetimerProtocolCapability"

typedef struct {
  BOOLEAN  RetimerSupport;
} USBC_RETIMER_PROTOCOL_CAPABILITY;

/**
  Get UsbC Retimer Controller info and Capability.

  @param[in]  This               The UsbC RETIMER PROTOCOL Instance.
  @param[in]  RetimerGuid        GUID from ESRT ACPI Table.

  @retval EFI_SUCCESS            Successfully Mode is Changed to FW Update Mode.
  @retval EFI_INVALID_PARAMETER  Invalid GUID from ESRT Table is Passed.
  @retval EFI_UNSUPPORTED        This driver does not support.
  @retval EFI_DEVICE_ERROR       This driver cannot be started due to device Error.
  @retval EFI_ALREADY_STARTED    This driver has been started.
  @retval EFI_TIMEOUT            UsbC Command timeout Happen.

**/
typedef
EFI_STATUS
(EFIAPI *USBC_RETIMER_INFO) (
  IN  USBC_RETIMER_PROTOCOL  *This,
  IN  EFI_GUID               RetimerGuid ///< GUID from ESRT ACPI Table
  );

/**
  Set PD Controller Mode

  @param[in]        PdControllerMode

  @retval           EFI_SUCCESS           Command success
  @retval           EFI_TIMEOUT           Command timeout
  @retval           EFI_DEVICE_ERROR      Command error
  @retval           EFI_INVALID_PARAMETER Command usage error.

**/
typedef
EFI_STATUS
(EFIAPI *SET_PD_CONTROLLER_MODE) (
  IN  UINT8  PdControllerMode
  );

/**
  Get PD Controller Mode

  @param[out]       PdControllerModeBuffer
  @param[out]       EcPdTempBuffer
  @param[in]        TotalCountOfPdController

  @retval           EFI_SUCCESS           Command success
  @retval           EFI_TIMEOUT           Command timeout
  @retval           EFI_DEVICE_ERROR      Command error
  @retval           EFI_INVALID_PARAMETER Command usage error.

**/
typedef
EFI_STATUS
(EFIAPI *GET_PD_CONTROLLER_MODE) (
  OUT  UINT8  *PdControllerModeBuffer,
  OUT  UINT8  *EcPdTempBuffer,
  IN   UINT8  TotalCountOfPdController
  );

/**
  Control Retimer FP GPIO by given output state

  @param[in] OutputState          RETIMER_FORCE_POWER_GPIO_LOW
                                  RETIMER_FORCE_POWER_GPIO_HIGH

  @retval  EFI_SUCCESS            Successfully assert/de-assert retimer FP GPIO
  @retval  EFI_INVALID_PARAMETER  Invalid group or pad number

**/
typedef
EFI_STATUS
(EFIAPI *CONTROL_RETIMER_FP_GPIO) (
  IN  UINT8  OutputState
  );

#pragma pack (push,1)
//
// Interface structure for the UsbC retimer Protocol
//
struct _USBC_RETIMER_PROTOCOL {
  GET_PD_CONTROLLER_MODE   GetPdControllerMode;   ///  Get PD controller Fw Update Mode.
  SET_PD_CONTROLLER_MODE   SetPdControllerMode;   ///  Set PD controller Fw Update Mode.
  CONTROL_RETIMER_FP_GPIO  RetimerFP;             ///  Control Retimer FP GPIO
};

#pragma pack (pop)

#endif
