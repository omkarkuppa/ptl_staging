/** @file
  Platform Sanitize Dxe driver supports Identifying RPE / LPE boot Header file.

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

#ifndef _PLATFORM_SANITIZE_H_
#define _PLATFORM_SANITIZE_H_

//
// Setting erase actions to execute in an order.
// Storage Erase is not listed here as its treated separately before attempting this list.
//
STATIC CONST UINT32 mOrderOfOperation[] = {
  PS_TPM_CLEAR,
  PS_OEM_CUSTOM_ACTION,
  PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION,
  PS_BIOS_RELOAD_GOLDEN_CONFIG,
  PS_CSME_UNCONFIGURE
};

/**
  Check if PS_BOOT_PARAMETERS exists in NVRAM from previous Sanitize Action Reboot.
  If 'PS_NVM_VARIABLE_BOOT_PARAM' received successfully means it was set in the
  previous boot and its continuation of sanitize process. So get boot parameter details.

  @retval TRUE       Previous Sanitize Continuation
  @retval FALSE      NVM variable doesn't exists, Sanitize requested freshly. (OR) Null Boot Parameter
**/
BOOLEAN
IsPreviousBootSanitizeContinuation (
  VOID
  );

/**
  Perform sanitize actions as per requested list from Parameter structure.
**/
VOID
PerformSanitizeActions (
  VOID
  );

/**
  Function to trigger TPM Clear operation by invoking Tpm2ClearControl and Tpm2Clear when TPM is present.
  TPM Clear affects Endorsement keys, NV Indexes and objects stored in the TPM's Owner or
  Endorsement hierarchies (like bitlocker key / virtual smart card / fingerprint data etc...)
  TPM clear does not affect TPM measurements.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_UNSUPPORTED       If TPM is not present/enabled.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.
**/
EFI_STATUS
TriggerTpmClear (
  VOID
  );

/**
  Clear All BIOS NVM Variable Region using SPI Protocols

  @retval EFI_SUCCESS           Cleared all BIOS NVM Variable region successfully
  @retval EFI_UNSUPPORTED       Flash update funtion failed
**/
EFI_STATUS
ClearAllBiosNvmVariableRegion (
  VOID
  );

/**
  Clear Boot option and all related NVM variables. So that next boot will be Normal Boot.
**/
VOID
PsClearBootOption (
  VOID
  );

/**
  Function to trigger Platform Sanitize Simulation.
**/
VOID
PsSimulation (
  VOID
  );

#endif  // _PLATFORM_SANITIZE_H_
