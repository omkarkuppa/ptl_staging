/** @file
  XmlCli Common Hii implementation.
  This file is responsible for registering relevant protocols

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

#include <XmlCliCommonHii.h>


EFI_GUID                          gXmlCliSetupConfigGuid  = XMLCLI_SETUP_GUID;
EFI_GUID                          gXmlCliSetupFormsetGuid = XMLCLI_FORM_SET_GUID;
XMLCLI_SETUP                      gXmlCliSetupConfig;
XMLCLI_PRIVATE_DATA               *gPrivate;
EFI_HII_CONFIG_ROUTING_PROTOCOL   *mHiiConfigRouting;


HII_VENDOR_DEVICE_PATH gXmlCliSetupHiiVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) (sizeof (VENDOR_DEVICE_PATH) >> 8)
      }
    },
    XMLCLI_FORM_SET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};


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
  )
{
  // Check for valid parameters
  if (Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Request;

  return EFI_NOT_FOUND;
}


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
  )
{

  if ((NULL == This) || (NULL == Progress)) {
    return EFI_INVALID_PARAMETER;
  }
  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Configuration;

  return EFI_NOT_FOUND;
}

/**
  This is the entrypoint of the XmlCliCommonHii driver.
  This initialize the XmlCli configuration form.

  @retval  EFI_SUCCESS           The XmlCli configuration form is initialized.
  @retval  EFI_OUT_OF_RESOURCES  Failed to allocate memory.
  @retval  Others                Other errors as indicated.
**/
EFI_STATUS
EFIAPI
InitXmlCliSetupEntry ()
{
  EFI_STATUS  Status;

  //
  //Locate required Hii protocols
  //
  Status = gBS->LocateProtocol (
                  &gEfiHiiConfigRoutingProtocolGuid,
                  NULL,
                  (VOID **) &mHiiConfigRouting
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  gPrivate = (XMLCLI_PRIVATE_DATA *) AllocateZeroPool (sizeof (XMLCLI_PRIVATE_DATA));
  if (gPrivate == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  gPrivate->ConfigAccess.ExtractConfig = XmlCliSetupFormExtractConfig;
  gPrivate->ConfigAccess.RouteConfig   = XmlCliSetupFormRouteConfig;

  // Install Device Path Protocol and Config Access protocol to driver handle
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &gPrivate->DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &gXmlCliSetupHiiVendorDevicePath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &gPrivate->ConfigAccess,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    FreePool(gPrivate);
    return Status;
  }

  // Publish our HII data
  gPrivate->HiiHandle = HiiAddPackages (
                          &gXmlCliSetupFormsetGuid,
                          gPrivate->DriverHandle,
                          XmlCliSetupVfrBin,
                          XmlCliCommonDxeStrings,
                          NULL
                          );

  if (gPrivate->HiiHandle == NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
           gPrivate->DriverHandle,
           &gEfiDevicePathProtocolGuid,
           &gXmlCliSetupHiiVendorDevicePath,
           &gEfiHiiConfigAccessProtocolGuid,
           &gPrivate->ConfigAccess,
           NULL
           );
    FreePool(gPrivate);
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}
