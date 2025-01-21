/** @file
  Register Definitions for Me States

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2009 Intel Corporation.

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
#ifndef _ME_STATE_H_
#define _ME_STATE_H_
///
/// Ignore ME_FW_INIT_COMPLETE status Macro
///
#define ME_STATUS_ME_STATE_ONLY(a)  ((a) & (~(ME_FW_INIT_COMPLETE | ME_FW_BOOT_OPTIONS_PRESENT)))

///
/// Macro to check if ME FW INIT is completed
///
#define ME_STATUS_IS_ME_FW_INIT_COMPLETE(a) (((a) & ME_FW_INIT_COMPLETE) == ME_FW_INIT_COMPLETE)

///
/// Macro to combine the complete bit to status
///
#define ME_STATUS_WITH_ME_INIT_COMPLETE(a)  ((a) | ME_FW_INIT_COMPLETE)

///
/// Macro to check ME Boot Option Present
///
#define ME_STATUS_IS_ME_FW_BOOT_OPTIONS_PRESENT(a)  (((a) & ME_FW_BOOT_OPTIONS_PRESENT) == ME_FW_BOOT_OPTIONS_PRESENT)

//
// Abstract ME Mode Definitions
//
#define ME_MODE_NORMAL        0x00
#define ME_MODE_DEBUG         0x02
#define ME_MODE_TEMP_DISABLED 0x03
#define ME_MODE_SECOVER       0x04
#define ME_MODE_FAILED        0x06

//
// Abstract ME Status Definitions
//
#define ME_READY                    0x00
#define ME_INITIALIZING             0x01
#define ME_IN_RECOVERY_MODE         0x02
#define ME_DISABLE_WAIT             0x06
#define ME_TRANSITION               0x07
#define ME_NOT_READY                0x0F
#define ME_FW_INIT_COMPLETE         0x80
#define ME_FW_BOOT_OPTIONS_PRESENT  0x100
#define ME_FW_UPDATES_IN_PROGRESS   0x200

//
// HECI Software Progress Error Code definitions.
// Error Code Structure: (EFI_SOFTWARE_UNSPECIFIED | (EFI_OEM_SPECIFIC | 0x00000x00) | ERROR_STATUS) = 0x03008x00
//
#define EFI_STATUS_TO_STATUS_CODE_VALUE(EfiStatus)  ((EFI_STATUS_CODE_VALUE) (EfiStatus & 0x1F))

#define SW_HECI_ACK_ERROR           (EFI_SOFTWARE_UNSPECIFIED | (EFI_OEM_SPECIFIC | 0x00000100))
#define SW_HECI_RECEIVE_ERROR       (EFI_SOFTWARE_UNSPECIFIED | (EFI_OEM_SPECIFIC | 0x00000200))
#define SW_HECI_SEND_ERROR          (EFI_SOFTWARE_UNSPECIFIED | (EFI_OEM_SPECIFIC | 0x00000300))
#define SW_HECI_INIT_ERROR          (EFI_SOFTWARE_UNSPECIFIED | (EFI_OEM_SPECIFIC | 0x00000400))
#define SW_HECI_GET_STATUS_ERROR    (EFI_SOFTWARE_UNSPECIFIED | (EFI_OEM_SPECIFIC | 0x00000500))
#define SW_HECI_GET_MODE_ERROR      (EFI_SOFTWARE_UNSPECIFIED | (EFI_OEM_SPECIFIC | 0x00000600))
#define SW_HECI_RESET_HECI_ERROR    (EFI_SOFTWARE_UNSPECIFIED | (EFI_OEM_SPECIFIC | 0x00000700))
#define SW_HECI_REINIT_ERROR        (EFI_SOFTWARE_UNSPECIFIED | (EFI_OEM_SPECIFIC | 0x00000800))
#define SW_HECI_RESETWAIT_ERROR     (EFI_SOFTWARE_UNSPECIFIED | (EFI_OEM_SPECIFIC | 0x00000900))
#define SW_HECI_EOP_ERROR           (EFI_SOFTWARE_UNSPECIFIED | (EFI_OEM_SPECIFIC | 0x00000A00))
#endif // _ME_STATE_H_
