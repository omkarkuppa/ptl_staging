/** @file
  Intel One Click Recovery PET event support implementation

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

#include "OcrPetEvent.h"
#include <OneClickRecovery.h>
#include "OcrHttpSupport.h"
#include "OcrHttpBootCallbackSupport.h"
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeAsfLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AsfProtocol.h>
#include <Protocol/HttpCallback.h>
#include <Guid/StatusCodeDataTypeId.h>

///
/// HTTP Boot Callback Protocol instance
///
STATIC
EDKII_HTTP_CALLBACK_PROTOCOL  mHttpCallback = {
  PetHttpEventCallback
};

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
  )
{
  DEBUG ((DEBUG_INFO, "OCR: HttpCallback: %d - %r\n", Event, EventStatus));

  switch (Event) {
    case HttpEventDns:
      if (EFI_ERROR (EventStatus)) {
        DEBUG ((DEBUG_WARN, "OCR: Name Resolution of URI failed\n"));
        SendOcrPetEvent (ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_ERROR, ASF_OCR_EVENT_DATA3_NAME_RESOLUTION_URI_FAILED, 0);
      } else {
        DEBUG ((DEBUG_INFO, "OCR: HTTPS URI Name Resolved\n"));
        SendOcrPetEvent (ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_PROGRESS, ASF_OCR_EVENT_DATA3_HTTPS_URI_NAME_RESOLVED, 0);
      }
      break;

    case HttpEventConnectTcp:
      break;

    case HttpEventTlsConnectSession:
      if (EFI_ERROR (EventStatus)) {
        DEBUG ((DEBUG_WARN, "OCR: HTTPS TLS Auth failed\n"));
        SendOcrPetEvent (ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_ERROR, ASF_OCR_EVENT_DATA3_HTTPS_TLS_AUTH_FAILED, 0);
      }
      break;

    case HttpEventInitSession:
      if (!EFI_ERROR (EventStatus)) {
        DEBUG ((DEBUG_INFO, "OCR: HTTPS Connected Successfully\n"));
        SendOcrPetEvent (ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_PROGRESS, ASF_OCR_EVENT_DATA3_HTTPS_CONNECTED_SUCCESS, 0);
      } else {
        DEBUG ((DEBUG_WARN, "OCR: Connect to URI failed\n"));
        SendOcrPetEvent (ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_ERROR, ASF_OCR_EVENT_DATA3_CONNECT_TO_URI_FAILED, 0);
      }
      break;

    default:
      break;
  }
}

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
  )
{
  EFI_RETURN_STATUS_EXTENDED_DATA *ExtendedData;
  UINT32                          ErrorCode;

  //
  // Check whether status code is what we are interested in.
  //
  if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) != EFI_ERROR_CODE) ||
    (Value & (EFI_STATUS_CODE_CLASS_MASK | EFI_STATUS_CODE_SUBCLASS_MASK)) != EFI_SOFTWARE_DXE_BS_DRIVER) {
    return EFI_UNSUPPORTED;
  }

  ErrorCode = Value & EFI_STATUS_CODE_OPERATION_MASK;
  if ((ErrorCode == EFI_SW_DXE_BS_EC_BOOT_OPTION_LOAD_ERROR) || (ErrorCode == EFI_SW_DXE_BS_EC_BOOT_OPTION_FAILED)) {
    ExtendedData = (EFI_RETURN_STATUS_EXTENDED_DATA *)Data;
    DEBUG ((DEBUG_INFO, "OCR: CodeType %x, Value %x, Status %d - %r\n",
              CodeType, Value, ExtendedData->ReturnStatus, ExtendedData->ReturnStatus));

    switch (ErrorCode) {
      case EFI_SW_DXE_BS_EC_BOOT_OPTION_LOAD_ERROR:
        switch (ExtendedData->ReturnStatus) {
          case EFI_NOT_FOUND:
            if ((AsfGetSpecialCommand () == ASF_INTEL_OEM_FORCE_PBA_BOOT_CMD)) {
              DEBUG ((DEBUG_INFO, "OCR: OEM App not found at local URI\n"));
              SendOcrPetEvent (
                ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_ERROR,
                ASF_OCR_EVENT_DATA3_FILEPATH_NOT_FOUND,
                0
                );
            } else if ((AsfGetSpecialCommand () == ASF_INTEL_OEM_FORCE_HTTPS_BOOT_CMD)) {
              if (!mHttpBootCallbackInvoked) {
                //
                // HTTP Boot callback is not invoked, it means there is no network connection available.
                //
                DEBUG ((DEBUG_INFO, "OCR: No network connection available\n"));
                SendOcrPetEvent (
                  ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_ERROR,
                  ASF_OCR_EVENT_DATA3_NO_NETWORK_CONNECTION,
                  0
                  );
              } else if (mHttpBootDownloadComplete) {
                //
                // it is bad image because Http boot image is downloaded but can't find boot app.
                //
                DEBUG ((DEBUG_INFO, "OCR: Verified boot failed (bad image)\n"));
                SendOcrPetEvent (
                  ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_ERROR,
                  ASF_OCR_EVENT_DATA3_VERIFIED_BOOT_FAILED,
                  0
                  );
              }
            }
            break;
          case EFI_ACCESS_DENIED:
          case EFI_SECURITY_VIOLATION:
              DEBUG ((DEBUG_INFO, "OCR: Verified boot failed\n"));
              SendOcrPetEvent (
                ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_ERROR,
                ASF_OCR_EVENT_DATA3_VERIFIED_BOOT_FAILED,
                0
                );
              break;
        }
        break;

      case EFI_SW_DXE_BS_EC_BOOT_OPTION_FAILED:
        if (ExtendedData->ReturnStatus == EFI_SECURITY_VIOLATION) {
          DEBUG ((DEBUG_INFO, "OCR: Verified boot failed(boot failure)\n"));
          SendOcrPetEvent (
            ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_ERROR,
            ASF_OCR_EVENT_DATA3_VERIFIED_BOOT_FAILED,
            0
            );
        }
        break;
    }
  }

  return EFI_SUCCESS;
}

/**
  Install the HTTP PET event callback

  @param[in,out]  HttpCallbackHandle    HTTP callback handle

  @retval   EFI_SUCCESS             Fully installed the the callback event
  @retval   Other                   Failure occurred in installation of callback
**/
EFI_STATUS
InstallHttpPetCallback (
  IN OUT EFI_HANDLE  *HttpCallbackHandle
  )
{
  EFI_STATUS    Status;

  Status = gBS->InstallProtocolInterface (
                  HttpCallbackHandle,
                  &gEdkiiHttpCallbackProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mHttpCallback
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[%a] Http PET Callback installed with Status %r", __FUNCTION__, Status));
  }
  return Status;
}

/**
  Uninstalls the HTTP PET event callback

  @param[in]  HttpCallbackHandle    HTTP callback handle
**/
VOID
UninstallHttpPetCallback (
  IN EFI_HANDLE  *HttpCallbackHandle
  )
{
  EFI_STATUS    Status;

  Status = gBS->UninstallProtocolInterface (
             HttpCallbackHandle,
             &gEdkiiHttpCallbackProtocolGuid,
             &mHttpCallback
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[%a] Http PET Callback uninstalled with Status %r", __FUNCTION__, Status));
  }
}
