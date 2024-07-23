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

#define    NO_VALID_GUID_FOUND                      0x00
#define    TBT_RETIMER_VALID_GUID_FOUND             BIT0

#define    RETIMER_FORCE_POWER_GPIO_HIGH            1
#define    RETIMER_FORCE_POWER_GPIO_LOW             0
///
/// Forward declaration for the _USBC_RETIMER_PROTOCOL.
///
typedef struct _USBC_RETIMER_PROTOCOL              USBC_RETIMER_PROTOCOL;

///
/// Global ID for the ESRT_MANAGEMENT_PROTOCOL.
///
// {B9AC2BD1-D450-4BEC-BD5B-192601119B84}
#define USBC_RETIMER_PROTOCOL_GUID \
{ \
    0xB9AC2BD1, 0xD450, 0x4BEC, { 0xBD, 0x5B, 0x19, 0x26, 0x1, 0x11, 0x9B, 0x84 } \
}
//EFI_GUID gUsbCRetimerProtocolGuid  = { 0xB9AC2BD1, 0xD450, 0x4BEC, { 0xBD, 0x5B, 0x19, 0x26, 0x1, 0x11, 0x9B, 0x84 } };

extern EFI_GUID gUsbCRetimerProtocolGuid;

// Structure Declaration

/**
  Get UsbC Retimer Controller info and Capability.

  @param[in]       This                 The UsbC RETIMER PROTOCOL Instance.
  @param[in]       RetimerGuid          GUID from ESRT ACPI Table.

  @retval EFI_SUCCESS                   Successfully Mode is Changed to FW Update Mode.
  @retval EFI_INVALID_PARAMETER         Invalid GUID from ESRT Table is Passed.
  @retval EFI_UNSUPPORTED               This driver does not support.
  @retval EFI_DEVICE_ERROR              This driver cannot be started due to device Error.
  @retval EFI_ALREADY_STARTED           This driver has been started.
  @retval EFI_TIMEOUT                   UsbC Command timeout Happen.

**/
typedef
EFI_STATUS
(EFIAPI *USBC_RETIMER_INFO) (
  IN  USBC_RETIMER_PROTOCOL                       *This,
  IN  EFI_GUID                                    RetimerGuid ///< GUID from ESRT ACPI Table
  );

/**
  Drive UsbC Retimer Controller into FW Update Mode.

  @param[in]       This                 The UsbC RETIMER PROTOCOL Instance.
  @param[in]       RetimerGuid          GUID from ESRT ACPI Table.

  @param[out]      *RetimerInfo         Out buffer containing Retimer info of guid passed.

  @retval EFI_SUCCESS                   Successfully Mode is Changed to FW Update Mode.
  @retval EFI_INVALID_PARAMETER         Invalid GUID from ESRT Table is Passed.
  @retval EFI_UNSUPPORTED               This driver does not support.
  @retval EFI_DEVICE_ERROR              This driver cannot be started due to device Error.
  @retval EFI_ALREADY_STARTED           This driver has been started.
  @retval EFI_TIMEOUT                   DriveToFwUpdateMode Command timeout Happen.

**/
typedef
EFI_STATUS
(EFIAPI *FW_UPDATE_MODE_ENTRY) (
  IN USBC_RETIMER_PROTOCOL      *This,
  IN EFI_GUID                   RetimerGuid ///< GUID from ESRT ACPI Table
  );

/**
  Restore TBT PD Controller into original mode.

  @param[in]       This                 The UsbC RETIMER PROTOCOL Instance.
  @param[in]       RetimerGuid          GUID from ESRT ACPI Table.

  @retval EFI_SUCCESS                   Successfully Mode is Restore.
  @retval EFI_INVALID_PARAMETER         Invalid GUID from ESRT Table is Passed.
  @retval EFI_UNSUPPORTED               This driver does not support.
  @retval EFI_DEVICE_ERROR              This driver cannot be started due to device Error.
  @retval EFI_NOT_STARTED               This driver has not been started.
  @retval EFI_TIMEOUT                   RestoreToOriginalMode Command timeout Happen.

**/
typedef
EFI_STATUS
(EFIAPI *FW_UPDATE_MODE_EXIT) (
  IN USBC_RETIMER_PROTOCOL      *This,
  IN EFI_GUID                   RetimerGuid
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
  FW_UPDATE_MODE_ENTRY                Drive;            ///  Drive UsbC retimer device into FW Update mode.
  FW_UPDATE_MODE_EXIT                 Restore;          ///  Restore UsbC retimer device into original mode.
  CONTROL_RETIMER_FP_GPIO             RetimerFP;        ///  Control Retimer FP GPIO
};

#pragma pack (pop)

#endif
