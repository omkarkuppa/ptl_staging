/** @file
  Intel One Click Recovery PET event functional definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _OCR_PET_EVENT_SUPPORT_H_
#define _OCR_PET_EVENT_SUPPORT_H_

#include <PiDxe.h>
#include <Uefi.h>
#include <Protocol/HttpCallback.h>

/**
  Callback function that is invoked when HTTP event occurs.

  @param[in]  This                Pointer to the EDKII_HTTP_CALLBACK_PROTOCOL instance.
  @param[in]  Event               The event that occurs in the current state.
  @param[in]  EventStatus         The Status of Event, EFI_SUCCESS or other errors.
**/
VOID
EFIAPI
PetHttpEventCallback (
  IN EDKII_HTTP_CALLBACK_PROTOCOL   *This,
  IN EDKII_HTTP_CALLBACK_EVENT      Event,
  IN EFI_STATUS                     EventStatus
  );

/**
  Report status code listener of FPDT. This is used to collect performance data
  for OsLoaderLoadImageStart and OsLoaderStartImageStart in FPDT.

  @param[in]  CodeType            Indicates the type of status code being reported.
  @param[in]  Value               Describes the current status of a hardware or software entity.
                                  This included information about the class and subclass that is used to
                                  classify the entity as well as an operation.
  @param[in]  Instance            The enumeration of a hardware or software entity within
                                  the system. Valid instance numbers start with 1.
  @param[in]  CallerId            This optional parameter may be used to identify the caller.
                                  This parameter allows the status code driver to apply different rules to
                                  different callers.
  @param[in]  Data                This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS             Status code is what we expected.
  @retval EFI_UNSUPPORTED         Status code not supported.

**/
EFI_STATUS
EFIAPI
OcrStatusCodeListener (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId,
  IN EFI_STATUS_CODE_DATA     *Data
  );

/**
  Install the HTTP PET event callback

  @param[in,out]  HttpCallbackHandle    HTTP callback handle

  @retval   EFI_SUCCESS             Fully installed the the callback event
  @retval   Other                   Failure occurred in installation of callback
**/
EFI_STATUS
InstallHttpPetCallback (
  IN OUT EFI_HANDLE  *HttpCallbackHandle
  );

/**
  Uninstalls the HTTP PET event callback

  @param[in]  HttpCallbackHandle    HTTP callback handle
**/
VOID
UninstallHttpPetCallback (
  IN EFI_HANDLE  *HttpCallbackHandle
  );

#endif // _OCR_PET_EVENT_SUPPORT_H_
