/** @file
  Intel One Click Recovery HTTP callback and HTTP boot callback functional implementation.

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

#include "OcrHttpBootCallbackSupport.h"
#include "OcrPetEvent.h"
#include <OneClickRecoveryConfig.h>
#include <Library/UefiLib.h>
#include <Library/HttpLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeAsfLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AsfProtocol.h>
#include <Protocol/Http.h>
#include <Protocol/HttpCallback.h>
#include <Protocol/HttpBootCallback.h>
#include <IndustryStandard/Http11.h>

///
/// HTTP Callback Protocol instance
///
STATIC
EFI_HTTP_BOOT_CALLBACK_PROTOCOL  mHttpBootCallback = {
  HttpBootCallback
};

//
//  Global variables for OCR HTTP boot
//
BOOLEAN      mHttpBootCallbackInvoked   = FALSE;
BOOLEAN      mHttpBootDownloadComplete  = FALSE;
EFI_HANDLE   mHttpBootCallbackHandle    = NULL;
EFI_HANDLE   mHttpCallbackHandle        = NULL;
CHAR8        *mAuthValue                = NULL;

/**
  Update the Authentication info to be sent to the HTTP Boot driver in HttpBootCallback()

  @param[in] Username   HTTP Boot Username string
  @param[in] Password   HTTP Boot Password string

  @retval EFI STATUS    Status of the updated authentication info process
  @retval Others        Error occurred
**/
EFI_STATUS
UpdateOcrHttpAuthInfo (
  IN CHAR8      *Username,
  IN CHAR8      *Password
  )
{
  CHAR8  RawAuthValue[HTTP_BOOT_AUTHENTICATION_INFO_MAX_LEN];
  UINT8  *BinData;
  UINTN  ReturnSize;

  if ((Username == NULL) || (Password == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  ReturnSize = HTTP_BOOT_AUTHENTICATION_INFO_MAX_LEN;

  AsciiSPrint (
    RawAuthValue,
    sizeof (RawAuthValue),
    "%a:%a",
    Username,
    Password
    );

  mAuthValue = AllocateZeroPool (ReturnSize);

  BinData  = (UINT8 *) RawAuthValue;
  Base64Encode (BinData, AsciiStrLen (RawAuthValue), mAuthValue, &ReturnSize);
  return EFI_SUCCESS;
}

/**
  Check whether Left and Right are the same without matching the specific
  device path data in IP device path and URI device path node.

  @retval TRUE  Left and Right are the same.
  @retval FALSE Left and Right are the different.
**/
BOOLEAN
MatchHttpBootDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *Left,
  IN EFI_DEVICE_PATH_PROTOCOL *Right
  )
{
  if ((Left == NULL) || (Right == NULL)) {
    return FALSE;
  }

  for (;  !IsDevicePathEnd (Left) && !IsDevicePathEnd (Right)
       ;  Left = NextDevicePathNode (Left), Right = NextDevicePathNode (Right)
       ) {
    if (CompareMem (Left, Right, DevicePathNodeLength (Left)) != 0) {
      if ((DevicePathType (Left) != MESSAGING_DEVICE_PATH) || (DevicePathType (Right) != MESSAGING_DEVICE_PATH)) {
        return FALSE;
      }

      if (DevicePathSubType (Left) == MSG_DNS_DP) {
        Left = NextDevicePathNode (Left);
      }

      if (DevicePathSubType (Right) == MSG_DNS_DP) {
        Right = NextDevicePathNode (Right);
      }

      if (((DevicePathSubType (Left) != MSG_IPv4_DP) || (DevicePathSubType (Right) != MSG_IPv4_DP)) &&
          ((DevicePathSubType (Left) != MSG_IPv6_DP) || (DevicePathSubType (Right) != MSG_IPv6_DP)) &&
          ((DevicePathSubType (Left) != MSG_URI_DP)  || (DevicePathSubType (Right) != MSG_URI_DP))
          ) {
        return FALSE;
      }
    }
  }
  return (BOOLEAN) (IsDevicePathEnd (Left) && IsDevicePathEnd (Right));
}

/**
  Indicate if the HTTP status code indicates a redirection.

  @param[in] StatusCode   HTTP status code from server.

  @return TRUE      If it's redirection.
  @retval FALSE     It's not redirection.
**/
BOOLEAN
IsHttpRedirectStatusCode (
  IN   EFI_HTTP_STATUS_CODE        StatusCode
  )
{
  switch (StatusCode) {
    case HTTP_STATUS_301_MOVED_PERMANENTLY:
    case HTTP_STATUS_302_FOUND:
    case HTTP_STATUS_307_TEMPORARY_REDIRECT:
    case HTTP_STATUS_308_PERMANENT_REDIRECT:
      return TRUE;
    default:
      return FALSE;
  }
}

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
  )
{
  EFI_HTTP_MESSAGE        *HttpMessage;
  EFI_HTTP_HEADER         *HttpHeader;
  UINT32                  Percentage;
  STATIC UINT64           mHttpFileSize;
  STATIC UINT64           mHttpReceivedSize;
  STATIC UINT32           mHttpPercentage;

  mHttpBootCallbackInvoked = TRUE;

  //
  // If Basic Authentication is not needed and Force Progress Events is not enabled, return.
  //
  if ((!AsfIsFwProgressEnabled ()) && (DataType != HttpBootHttpAuthInfo)) {
    return EFI_SUCCESS;
  }

  switch (DataType) {
    case HttpBootDhcp4:
    case HttpBootDhcp6:
      Print (L".");
      break;

    case HttpBootHttpRequest:
      if (Data != NULL) {
        HttpMessage = (EFI_HTTP_MESSAGE *) Data;
        if (HttpMessage->Data.Request->Method == HttpMethodGet &&
            HttpMessage->Data.Request->Url != NULL) {
          Print (L"\n  URI: %s\n", HttpMessage->Data.Request->Url);
        }
      }
      break;

    case HttpBootHttpResponse:
      if (Data != NULL) {
        mHttpFileSize = 0;
        mHttpPercentage = 0;
        HttpMessage = (EFI_HTTP_MESSAGE *) Data;

        if (HttpMessage->Data.Response != NULL) {
          if (IsHttpRedirectStatusCode (HttpMessage->Data.Response->StatusCode)) {
            //
            // Server indicates the resource has been redirected to a different URL
            // according to the section 6.4 of RFC 7231 and the RFC 7538.
            // Display the redirect information on the screen.
            //
            HttpHeader = HttpFindHeader (
                          HttpMessage->HeaderCount,
                          HttpMessage->Headers,
                          HTTP_HEADER_LOCATION
                          );
            if (HttpHeader != NULL) {
              Print (L"\n  HTTP ERROR: Resource Redirected.\n  New Location: %a\n", HttpHeader->FieldValue);
            }
            break;
          }

          if (HttpMessage->Data.Response->StatusCode == HTTP_STATUS_404_NOT_FOUND) {
            DEBUG ((DEBUG_INFO, "OCR: HTTPS Boot File not found\n"));
            SendOcrPetEvent (
              ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_ERROR,
              ASF_OCR_EVENT_DATA3_HTTPS_BOOT_FILE_NOT_FOUND,
              0
              );
          } else if (HttpMessage->Data.Response->StatusCode == HTTP_STATUS_401_UNAUTHORIZED) {
            DEBUG ((DEBUG_INFO, "OCR: HTTPS Digest Auth failed\n"));
            SendOcrPetEvent (
              ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_ERROR,
              ASF_OCR_EVENT_DATA3_HTTPS_DIGEST_AUTH_FAILED,
              0
              );
          }
        }

        HttpHeader = HttpFindHeader (
                       HttpMessage->HeaderCount,
                       HttpMessage->Headers,
                       HTTP_HEADER_CONTENT_LENGTH
                       );
        if (HttpHeader != NULL) {
          mHttpFileSize = AsciiStrDecimalToUintn (HttpHeader->FieldValue);
          mHttpReceivedSize = 0;
          mHttpPercentage   = 0;
        }
      }
      break;

    case HttpBootHttpEntityBody:
      if (DataLength != 0) {
        if (mHttpFileSize != 0) {
          //
          // We already know the file size, print in percentage format.
          //
          if (mHttpReceivedSize == 0) {
            Print (L"  File Size: %lu Bytes\n", mHttpFileSize);
          }
          mHttpReceivedSize += DataLength;
          Percentage = (UINT32) DivU64x64Remainder (MultU64x32 (mHttpReceivedSize, 100), mHttpFileSize, NULL);
          if (mHttpPercentage != Percentage) {
            mHttpPercentage = Percentage;
            Print (L"\r  Downloading...%d%%", Percentage);
            if (mHttpPercentage >= 100) {
              DEBUG ((DEBUG_INFO, "\nOCR: HTTPSBoot download is completed: %d/%d\n", mHttpReceivedSize, mHttpFileSize));
              mHttpBootDownloadComplete = TRUE;
              SendOcrPetEvent (
                ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_PROGRESS,
                ASF_OCR_EVENT_DATA3_HTTPS_BOOT_DOWNLOAD,
                0
                );
            }
          }
        } else {
          //
          // In some case we couldn't get the file size from the HTTP header, so we
          // just print the downloaded file size.
          //
          mHttpReceivedSize += DataLength;
          Print (L"\r  Downloading...%lu Bytes", mHttpReceivedSize);
        }
      }
      break;

    case HttpBootHttpAuthInfo :
      if (mAuthValue != NULL) {
        DEBUG ((DEBUG_INFO, "OCR : HTTPS Authentication information provided\n"));
        CopyMem (Data, (CHAR8 *) mAuthValue, HTTP_BOOT_AUTHENTICATION_INFO_MAX_LEN);
      }
      break;

    default:
      break;
  }
  return EFI_SUCCESS;
}


/**
  Install the network callback, include gEfiHttpBootCallbackProtocolGuid
  and gEdkiiHttpCallbackProtocolGuid

  @param[in]  DevicePath                Device Path.
**/
VOID
InstallNetworkCallback (
  IN CHAR16                *DevicePath
  )
{
  EFI_STATUS                      Status;
  EFI_HANDLE                      *Handles;
  UINTN                           Index;
  UINTN                           HandleCount;
  EFI_DEVICE_PATH_PROTOCOL        *FilePath;
  EFI_DEVICE_PATH_PROTOCOL        *NewDevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *HttpPath;
  URI_DEVICE_PATH                 *NullUriPath;

  NullUriPath = NULL;
  Handles = NULL;
  HttpPath = NULL;
  DEBUG ((DEBUG_INFO, "InstallNetworkCallback\n"));

  FilePath = ConvertTextToDevicePath (DevicePath);
  //
  // Use wide match algorithm to find the handle.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiLoadFileProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );
  if (EFI_ERROR (Status) || (Handles == NULL)) {
    return;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    NewDevicePath = DevicePathFromHandle (Handles[Index]);
    if (NewDevicePath == NULL) {
      continue;
    }
    if (MatchHttpBootDevicePath (NewDevicePath, FilePath)) {
      //
      // Matches HTTP Boot Device Path described as
      //   ....../Mac(...)[/Vlan(...)][/Wi-Fi(...)]/IPv4(...)[/Dns(...)]/Uri(...)
      //   ....../Mac(...)[/Vlan(...)][/Wi-Fi(...)]/IPv6(...)[/Dns(...)]/Uri(...)
      //
      mHttpBootCallbackHandle = Handles[Index];
      break;
    }
  }

  if (mHttpBootCallbackHandle == NULL) {
    NullUriPath = (URI_DEVICE_PATH *) CreateDeviceNode (
                                        MESSAGING_DEVICE_PATH,
                                        MSG_URI_DP,
                                        (UINT16)(sizeof (URI_DEVICE_PATH))
                                        );
    for (Index = 0; Index < HandleCount; Index++) {
      NewDevicePath = DevicePathFromHandle (Handles[Index]);
      if (NewDevicePath == NULL) {
        continue;
      }
      HttpPath = AppendDevicePathNode (NewDevicePath, (EFI_DEVICE_PATH_PROTOCOL *)NullUriPath);
      if (HttpPath == NULL) {
        continue;
      }
      if (MatchHttpBootDevicePath (HttpPath, FilePath)) {
        //
        // Matches HTTP Boot Device Path described as
        //   ....../Mac(...)[/Vlan(...)][/Wi-Fi(...)]/IPv4(...)[/Dns(...)]/Uri(...)/Uri(...)
        //   ....../Mac(...)[/Vlan(...)][/Wi-Fi(...)]/IPv6(...)[/Dns(...)]/Uri(...)/Uri(...)
        //
        mHttpBootCallbackHandle = Handles[Index];
        break;
      }
    }
    if (NullUriPath != NULL) {
      FreePool (NullUriPath);
    }
  }

  if (Handles != NULL) {
    FreePool (Handles);
  }

  //
  // Install a Http Boot callback
  //
  if (mHttpBootCallbackHandle != NULL) {
    DEBUG ((DEBUG_INFO, "Install gEfiHttpBootCallbackProtocolGuid\n"));
    Status = gBS->InstallProtocolInterface (
                    &mHttpBootCallbackHandle,
                    &gEfiHttpBootCallbackProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mHttpBootCallback
                    );
    if (EFI_ERROR (Status)) {
      mHttpBootCallbackHandle = NULL;
    }

    if (!AsfIsFwProgressEnabled ()) {
      return;
    }

    //
    // Install a Http callback for http boot only.
    //
    if (mHttpCallbackHandle == NULL) {
      DEBUG ((DEBUG_INFO, "Install gEdkiiHttpCallbackProtocolGuid\n"));
      mHttpBootCallbackInvoked = FALSE;
      mHttpBootDownloadComplete = FALSE;
      Status = InstallHttpPetCallback (&mHttpCallbackHandle);
      if (EFI_ERROR (Status)) {
        mHttpCallbackHandle = NULL;
      }
    }
  }

}

/**
  Uninstall the network callback, include gEfiHttpBootCallbackProtocolGuid
  and gEdkiiHttpCallbackProtocolGuid
**/
VOID
UninstallNetworkCallback (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "UninstallNetworkCallback\n"));
  if (mHttpBootCallbackHandle != NULL) {
    DEBUG ((DEBUG_INFO, "Uninstall gEfiHttpBootCallbackProtocolGuid\n"));
    gBS->UninstallProtocolInterface (
            mHttpBootCallbackHandle,
            &gEfiHttpBootCallbackProtocolGuid,
            &mHttpBootCallback
            );
    mHttpBootCallbackHandle = NULL;
  }

  if (mHttpCallbackHandle != NULL) {
    DEBUG ((DEBUG_INFO, "Uninstall gEdkiiHttpCallbackProtocolGuid\n"));
    UninstallHttpPetCallback (mHttpCallbackHandle);
    mHttpCallbackHandle = NULL;
  }
}
