/** @file
  Library to handle specific reset requirements after firmware update.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef __CAPSULE_UPDATE_RESET_LIB_H__
#define __CAPSULE_UPDATE_RESET_LIB_H__

#define  CAPSULE_UPDATE_RESET_TYPE_GLOBAL_RESET    0x01
#define  CAPSULE_UPDATE_RESET_TYPE_EC_RESET        0x02

/**
  Register a ResetFilter notification function to change system reset type according
  to given CapsuleResetType.

  @param[in] CapsuleResetType   The platform specific reset type that the caller intends
                                to change to. This is different to EFI_RESET_TYPE.

  @retval EFI_SUCCESS           ResetFilter notification is registered successfully.
  @retval Other                 Failed to register ResetFilter notify function.

**/
EFI_STATUS
EFIAPI
CapsuleUpdateRegisterResetFilter (
  UINT8                CapsuleResetType
  );

/**
  Register ResetHandler notification function to handle specific platform reset functionality
  according to CapsuleResetType.

  @param[in] CapsuleResetType   The platform specific reset type that the caller intends to support.
                                This is different to EFI_RESET_TYPE.

  @retval EFI_SUCCESS           ResetHandler notification is registered successfully.
  @retval Other                 Failed to register ResetHandler notify function.

**/
EFI_STATUS
EFIAPI
CapsuleUpdateRegisterResetHandler (
  UINT8                CapsuleResetType
  );

/**
  Register a reset notification to arm platform WDT right before system reset.
  This WDT is intended to deal with the case when BIOS/ACM/uCode is broken
  and BIOS cannot even be loaded after capsule update.

  With the assistance of platform WDT, TopSwap would be set and system would have a chance to recover.

  @param[in] WdtTimeOut       The timeout value (seconds) set to WDT.

  @retval EFI_SUCCESS           ResetNotification is registered successfully.
  @retval Other                 Failed to register ResetNotification function.

**/
EFI_STATUS
EFIAPI
CapsuleUpdateArmWdtBeforeReset (
  UINT8                WdtTimeOut
  );

#endif
