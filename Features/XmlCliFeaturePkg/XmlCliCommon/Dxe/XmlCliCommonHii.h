/** @file
  Definitions for XmlCliCommonHii.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef __XMLCLICOMMONHII__H__
#define __XMLCLICOMMONHII__H__

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiLib.h>
#include <Library/UefiHiiServicesLib.h>
#include <Library/PrintLib.h>
#include <Library/IoLib.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiConfigRouting.h>
#include <Protocol/FormBrowser2.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiString.h>
#include <Protocol/FormBrowserEx.h>
#include <Protocol/HiiConfigKeyword.h>
#include <Protocol/HiiPopup.h>
#include <XmlCliSetup.h>


//
// These are the VFR compiler generated data representing our VFR data.
//
extern UINT8  XmlCliSetupVfrBin[];
extern UINT8  XmlCliCommonDxeStrings[];


typedef struct _XMLCLI_PRIVATE_DATA  XMLCLI_PRIVATE_DATA;


struct _XMLCLI_PRIVATE_DATA {
  EFI_HANDLE                      DriverHandle;
  EFI_HII_CONFIG_ACCESS_PROTOCOL  ConfigAccess;
  EFI_HII_HANDLE                  HiiHandle;
};


///
/// HII specific Vendor Device Path definition.
///
typedef struct {
  VENDOR_DEVICE_PATH        VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  End;
} HII_VENDOR_DEVICE_PATH;


/**
  This function applies changes in a driver's configuration.
  Input is a Configuration, which has the routing data for this
  driver followed by name / value configuration pairs. The driver
  must apply those pairs to its configurable storage. If the
  driver's configuration is stored in a linear block of data
  and the driver's name / value pairs are in <BlockConfig>
  format, it may use the ConfigToBlock helper function (above) to
  simplify the job.

  @param[in] This           Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Configuration  A null-terminated Unicode string in
                            <ConfigString> format.
  @param[out] Progress      A pointer to a string filled in with the
                            offset of the most recent '&' before the
                            first failing name / value pair (or the
                            beginning of the string if the failure
                            is in the first name / value pair) or
                            the terminating NULL if all was
                            successful.

  @retval  EFI_SUCCESS             The results have been distributed or are
                                   awaiting distribution.
  @retval  EFI_OUT_OF_RESOURCES    Not enough memory to store the
                                   parts of the results that must be
                                   stored awaiting possible future
                                   protocols.
  @retval  EFI_INVALID_PARAMETERS  Passing in a NULL for the
                                   Results parameter would result
                                   in this type of error.
  @retval  EFI_NOT_FOUND           Target for the specified routing data
                                   was not found.
**/
EFI_STATUS
EFIAPI
XmlCliSetupFormExtractConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST EFI_STRING                      Request,
  OUT EFI_STRING                           *Progress,
  OUT EFI_STRING                           *Results
  );


/**
  This function processes the results of changes in configuration.

  @param[in] This           Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Configuration  A null-terminated Unicode string in <ConfigResp>
                            format.
  @param[out] Progress      A pointer to a string filled in with the offset of
                            the most recent '&' before the first failing
                            name/value pair (or the beginning of the string if
                            the failure is in the first name/value pair) or
                            the terminating NULL if all was successful.

  @retval  EFI_SUCCESS            The Results is processed successfully.
  @retval  EFI_INVALID_PARAMETER  Configuration is NULL.
  @retval  EFI_NOT_FOUND          Routing data doesn't match any storage in this
                                  driver.

**/
EFI_STATUS
EFIAPI
XmlCliSetupFormRouteConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST EFI_STRING                      Configuration,
  OUT EFI_STRING                           *Progress
  );

#endif  // __XMLCLICOMMONHII__H__
