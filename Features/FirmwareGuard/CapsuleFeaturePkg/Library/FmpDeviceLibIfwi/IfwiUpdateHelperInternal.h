/** @file
  Support FmpDeviceLibIfwi library.
  This file defined some internal macro and internal function.

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

#ifndef _IFWI_UPDATE_HELPER_INTERNAL_H_
#define _IFWI_UPDATE_HELPER_INTERNAL_H_

//
// EDK-II Foundation, used in this driver.
//
#include <Uefi.h>
//
// Advanced Feature Package Foundation itself, used in this driver.
//
#include <Library/ConfigProfileAccessLib.h>
#include <Library/PlatformFlashAccessLib.h>

/**
  This code is used to bypass the runtime service set variable.
  To prevent write the variable in Non-Volatile device.

  @param[in] VariableName  Name of Variable to be found.
  @param[in] VendorGuid    Variable vendor GUID.
  @param[in] Attributes    Attribute value of the variable found.
  @param[in] DataSize      Size of Data found. If size is less than the
                           data, this value contains the required size.
  @param[in] Data          Data pointer.

  @retval EFI_SUCCESS  Hook the service always return success.

**/
EFI_STATUS
EFIAPI
HookSetVariableFunction (
  IN CHAR16    *VariableName,
  IN EFI_GUID  *VendorGuid,
  IN UINT32    Attributes,
  IN UINTN     DataSize,
  IN VOID      *Data
  );

/**
  This code is used to bypass the runtime service get variable.
  To prevent the variable read error in Non-Volatile device.

  @param[in]      VariableName  Name of Variable to be found.
  @param[in]      VendorGuid    Variable vendor GUID.
  @param[out]     Attributes    Attribute value of the variable found.
  @param[in, out] DataSize      Size of Data found. If size is less than the
                                data, this value contains the required size.
  @param[out]     Data          The buffer to return the contents of the variable. May be NULL
                                with a zero DataSize in order to determine the size buffer needed.

  @retval EFI_SUCCESS  Hook the service always return success.

**/
EFI_STATUS
EFIAPI
HookGetVariableFunction (
  IN     CHAR16    *VariableName,
  IN     EFI_GUID  *VendorGuid,
     OUT UINT32    *Attributes,   OPTIONAL
  IN OUT UINTN     *DataSize,
     OUT VOID      *Data          OPTIONAL
  );

/**
  Support to halt the CSME for programming to SPI directly.

  @retval EFI_UNSUPPORTED       Current ME mode doesn't support this function.
  @retval EFI_SUCCESS           Command succeeded.
  @retval EFI_DEVICE_ERROR      HECI Device error, command aborts abnormally.
  @retval EFI_TIMEOUT           HECI does not return the buffer before timeout.
  @retval EFI_BUFFER_TOO_SMALL  Message Buffer is too small for the Acknowledge.

**/
EFI_STATUS
HaltMeBeforeFlash (
  VOID
  );

/**
  Get the IFWI flash request by config data.

  @param[in]      ConfigData       Update data within configuration INI file.
  @param[in]      UpdateImageBase  Input update image base address pointer.
  @param[in]      HelperImageBase  BGUP update image base address pointer.
                                   It would only be referenced when BiosGuard
                                   is enabled on platform.
  @param[in, out] FlashReq         Generic Flash ACCESS request to different
                                   flash storage mediaFlash request.

**/
VOID
GetIfwiUpdateRequest (
  IN     CONST UPDATE_CONFIG_DATA    ConfigData,
  IN     CONST VOID                  *UpdateImageBase,
  IN     CONST VOID                  *HelperImageBase,
  IN OUT       FLASH_ACCESS_REQUEST  *FlashReq
  );

#endif
