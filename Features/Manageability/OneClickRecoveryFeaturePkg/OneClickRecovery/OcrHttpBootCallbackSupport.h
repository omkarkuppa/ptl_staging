/** @file
  Intel One Click Recovery HTTP callback and HTTP boot callback functional definiton.

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

#ifndef _HTTP_CALLBACK_SUPPORT_H_
#define _HTTP_CALLBACK_SUPPORT_H_

#include <PiDxe.h>
#include <Uefi.h>
#include <Protocol/HttpBootCallback.h>

//
// Flag to indicate if HTTP Boot callback is invoked.
// If current OCR boot is HTTP boot but HTTP Boot callback is not invoked,
// it means there is no network connection available.
//
extern BOOLEAN      mHttpBootCallbackInvoked;

//
// Flag to indicate if http boot download is complete.
//
extern BOOLEAN      mHttpBootDownloadComplete;

//
// Handle for EFI_HTTP_BOOT_CALLBACK_PROTOCOL
//
extern EFI_HANDLE   mHttpBootCallbackHandle;

//
// Handle for EDKII_HTTP_CALLBACK_PROTOCOL
//
extern EFI_HANDLE   mHttpCallbackHandle;

//
// HTTP Boot Authentication Information
//
extern CHAR8        *mAuthValue;

/**
  Callback function that is invoked when the HTTP Boot driver is about to transmit or has received a
  packet.

  This function is invoked when the HTTP Boot driver is about to transmit or has received a packet.
  Parameters DataType and Received specify the type of event and the format of the buffer pointed
  to by Data. Due to the polling nature of UEFI device drivers, this callback function should not
  execute for more than 5 ms.
  The returned status code determines the behavior of the HTTP Boot driver.

  @param[in]  This                Pointer to the EFI_HTTP_BOOT_CALLBACK_PROTOCOL instance.
  @param[in]  DataType            The event that occurs in the current state.
  @param[in]  Received            TRUE if the callback is being invoked due to a receive event.
                                  FALSE if the callback is being invoked due to a transmit event.
  @param[in]  DataLength          The length in bytes of the buffer pointed to by Data.
  @param[in]  Data                A pointer to the buffer of data, the data type is specified by
                                  DataType.

  @retval EFI_SUCCESS             Tells the HTTP Boot driver to continue the HTTP Boot process.
**/
EFI_STATUS
EFIAPI
HttpBootCallback (
  IN EFI_HTTP_BOOT_CALLBACK_PROTOCOL     *This,
  IN EFI_HTTP_BOOT_CALLBACK_DATA_TYPE    DataType,
  IN BOOLEAN                             Received,
  IN UINT32                              DataLength,
  IN VOID                                *Data     OPTIONAL
  );

/**
  Update the Authentication info to be sent to the HTTP Boot driver in HttpBootCallback()

  @param[in]     Username        HTTP Boot Username string
  @param[in]     Password        HTTP Boot Password string
**/
EFI_STATUS
UpdateOcrHttpAuthInfo (
  IN CHAR8                   *Username,
  IN CHAR8                   *Password
  );

/**
  Install the network callback, include gEfiHttpBootCallbackProtocolGuid
  and gEdkiiHttpCallbackProtocolGuid

  @param[in]  DevicePath                Device Path.
**/
VOID
InstallNetworkCallback (
  IN CHAR16                *DevicePath
  );

/**
  Uninstall the network callback, include gEfiHttpBootCallbackProtocolGuid
  and gEdkiiHttpCallbackProtocolGuid
**/
VOID
UninstallNetworkCallback (
  VOID
  );

#endif // _HTTP_CALLBACK_SUPPORT_H_
