/**@file
  The file implements NVMe Based Recovery HII functions

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

#include "NvmeRecoveryHii.h"
//
// Character definitions
//
#define UPPER_LOWER_CASE_OFFSET 0x20

//
// This is the generated IFR binary Data for each formset defined in VFR.
// This Data array is ready to be used as input of HiiAddPackages() to
// create a packagelist (which contains Form packages, String packages, etc).
//
extern UINT8  NvmeRecoveryHiiFormBin[];
//
// This is the generated String package Data for all .UNI files.
// This Data array is ready to be used as input of HiiAddPackages() to
// create a packagelist (which contains Form packages, String packages, etc).
//
extern UINT8  NvmeRecoveryDxeStrings[];

CHAR16  mNvmeRecoveryStorageName[] = L"NvmeRecoverHii";

EFI_HII_CONFIG_ACCESS_PROTOCOL mHiiConfigAccessProtocol;

//
// Handle to the list of HII packages (forms and strings) for this driver
//
EFI_HII_HANDLE mHiiPackageListHandle = NULL;
EFI_HANDLE     mDriverHandle = NULL;

//
// Structure that contains state of the HII
// This structure is updated by Hii.cpp and its contents
// is rendered in the HII.
//
NVME_BASED_RECOVERY_HII_CONFIG mHiiConfiguration;

//
// The device path containing the VENDOR_DEVICE_PATH and EFI_DEVICE_PATH_PROTOCOL
//
HII_VENDOR_DEVICE_PATH mHiiVendorDevicePath = {
    {
        {
            HARDWARE_DEVICE_PATH,
            HW_VENDOR_DP,
            {
                (UINT8)(sizeof(VENDOR_DEVICE_PATH)),
                (UINT8)((sizeof(VENDOR_DEVICE_PATH)) >> 8)
            }
        },
        NVME_BASED_RECOVERY_HII_GUID
    },
    {
        END_DEVICE_PATH_TYPE,
        END_ENTIRE_DEVICE_PATH_SUBTYPE,
        {
            (UINT8)(END_DEVICE_PATH_LENGTH),
            (UINT8)((END_DEVICE_PATH_LENGTH) >> 8)
        }
    }
};

/**
  This function to set NvmeRecoverHii variable.

  @param[in]  NvmeHii  Points to the NVME_BASED_RECOVERY_HII_CONFIG.

  @retval  EFI_SUCCESS  Successfully set variable.
  @retval  Others       Failed set variable.

**/
EFI_STATUS
SetNvmeRecoveryHii (
  IN NVME_BASED_RECOVERY_HII_CONFIG  *NvmeHii
  )
{
  EFI_STATUS  Status;

  Status = gRT->SetVariable (
                  NVME_BASED_RECOVERY_VARIABLE_NAME,
                  &gNvmeBasedRecoveryHiiGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (NVME_BASED_RECOVERY_HII_CONFIG),
                  NvmeHii
                  );

  return Status;
}

/**
  This function to get NvmeRecoverHii value.

  @param[in, out]  NvmeHii      Points to the NVME_BASED_RECOVERY_HII_CONFIG.
  @param[in]       NvmeHiiSize  The size of NvmeHii.

  @retval  EFI_SUCCESS  Successfully get variable.
  @retval  Others       Failed get variable.
**/
EFI_STATUS
GetNvmeRecoveryHii (
  IN OUT NVME_BASED_RECOVERY_HII_CONFIG  *NvmeHii,
  IN UINTN                                NvmeHiiSize
  )
{
  EFI_STATUS  Status;

  Status  = gRT->GetVariable (
                   NVME_BASED_RECOVERY_VARIABLE_NAME,
                   &gNvmeBasedRecoveryHiiGuid,
                   NULL,
                   &NvmeHiiSize,
                   NvmeHii
                   );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get NVMe Recovery Hii variable fail, Status = %r\n", Status));
    if (Status != EFI_NOT_FOUND) {
      ASSERT_EFI_ERROR (Status);
    }
  }
  return Status;
}

/**
  This function to initialize NvmeRecoverHii value.

  @retval  EFI_SUCCESS  Successfully initialize variable.
  @retval  Others       Failed initialize variable.
**/
EFI_STATUS
InitNvmeRecoveryHiiDefault (
  VOID
  )
{
  EFI_STATUS                      Status;
  NVME_BASED_RECOVERY_HII_CONFIG  NvmeHii;
  UINTN                           NvmeHiiSize;

  NvmeHiiSize = sizeof (NVME_BASED_RECOVERY_HII_CONFIG);
  Status      = GetNvmeRecoveryHii (&NvmeHii, NvmeHiiSize);

  if (!EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Initialize default
  //
  DEBUG ((DEBUG_INFO, "Initialize the Nvme Recovery Hii Variable default.\n"));
  NvmeHii.Revision              = NVME_BASED_RECOVERY_HII_CONFIG_REVISION;
  NvmeHii.NvmeRecoverySupported = NVME_RECOVERY_FUNCTION_DISABLE_VALUE;
  NvmeHii.KeyMigrationAction    = NVME_RPMB_KEY_MIGRATION_VALUE_NO_ACTION;

  Status = SetNvmeRecoveryHii (&NvmeHii);

  return Status;
}

/**
  This function processes the results of changes in configuration.

  @param[in]   This           Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]   Configuration  A null-terminated Unicode string in <ConfigResp> format.
  @param[out]  Progress       A pointer to a string filled in with the offset of
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
RouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  CONST EFI_STRING                      Configuration,
  OUT       EFI_STRING                      *Progress
  )
{
  EFI_STATUS                      Status;
  NVME_BASED_RECOVERY_HII_CONFIG  *NvmeHii;
  UINTN                           BufferSize;

  if (Configuration == NULL || Progress == NULL) {
    return (EFI_INVALID_PARAMETER);
  }

  *Progress = Configuration;
  if (!HiiIsConfigHdrMatch (Configuration, &gNvmeBasedRecoveryHiiGuid, mNvmeRecoveryStorageName)) {
    return EFI_NOT_FOUND;
  }

  NvmeHii = AllocateZeroPool (sizeof (NVME_BASED_RECOVERY_HII_CONFIG));
  if (NvmeHii == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  BufferSize = 0;
  Status     = gHiiConfigRouting->ConfigToBlock (
                                    gHiiConfigRouting,
                                    Configuration,
                                    (UINT8 *)NvmeHii,
                                    &BufferSize,
                                    Progress
                                    );

  if (Status != EFI_BUFFER_TOO_SMALL) {
    FreePool (NvmeHii);
    return Status;
  }

  Status = gHiiConfigRouting->ConfigToBlock (
                                gHiiConfigRouting,
                                Configuration,
                                (UINT8 *)NvmeHii,
                                &BufferSize,
                                Progress
                                );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "NVMe Recovery RouteConfig Config to block fail, Status = %r\n", Status));
  } else {
    Status = SetNvmeRecoveryHii (NvmeHii);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Set NvmeRecoveryHii variable fail, Status = %r\n", Status));
    }
  }

  FreePool (NvmeHii);
  *Progress = Configuration + StrLen (Configuration);

  return Status;
}

/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.

  @param[in]   This      Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]   Request   A null-terminated Unicode string in <ConfigRequest> format.
  @param[out]  Progress  On return, points to a character in the Request
                         string. Points to the string's null terminator if
                         request was successful. Points to the most recent
                         '&' before the first failing name/value pair (or
                         the beginning of the string if the failure is in
                         the first name/value pair) if the request was not
                         successful.
  @param[out]  Results   A null-terminated Unicode string in
                         <ConfigAltResp> format which has all values filled
                         in for the names in the Request string. String to
                         be allocated by the called function.

  @retval  EFI_SUCCESS            The Results is filled with the requested values.
  @retval  EFI_OUT_OF_RESOURCES   Not enough memory to store the results.
  @retval  EFI_INVALID_PARAMETER  Request is illegal syntax, or unknown name.
  @retval  EFI_NOT_FOUND          Routing data doesn't match any storage in this
                                  driver.

**/
EFI_STATUS
EFIAPI
ExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  CONST EFI_STRING                      Request,
  OUT       EFI_STRING                      *Progress,
  OUT       EFI_STRING                      *Results
  )
{
  EFI_STATUS                      Status;
  EFI_STRING                      ConfigRequest;
  EFI_STRING                      ConfigRequestHdr;
  UINTN                           BufferSize;
  UINTN                           Size;
  BOOLEAN                         AllocatedRequest;
  NVME_BASED_RECOVERY_HII_CONFIG  *NvmeHii;

  //
  // Check for valid parameters
  //
  if (Progress == NULL || Results == NULL) {
    return (EFI_INVALID_PARAMETER);
  }

  *Progress = Request;
  if ((Request != NULL) &&
    !HiiIsConfigHdrMatch (Request, &gNvmeBasedRecoveryHiiGuid, mNvmeRecoveryStorageName)) {
    return EFI_NOT_FOUND;
  }

  AllocatedRequest = FALSE;
  BufferSize       = sizeof (NVME_BASED_RECOVERY_HII_CONFIG);
  ConfigRequest    = Request;
  NvmeHii          = AllocateZeroPool (BufferSize);

  Status = GetNvmeRecoveryHii (NvmeHii, BufferSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "NVMe Recovery ExtractConfig get variable fail.\n"));
  }

  if ((Request == NULL) || (StrStr (Request, L"OFFSET") == NULL)) {
    //
    // Request has no request element, construct full request string.
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
    //
    ConfigRequestHdr = HiiConstructConfigHdr (&gNvmeBasedRecoveryHiiGuid, mNvmeRecoveryStorageName, mDriverHandle);
    if (ConfigRequestHdr == NULL) {
      return EFI_NOT_FOUND;
    }
    Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Size);
    if (ConfigRequest == NULL) {
      FreePool (ConfigRequestHdr);
      return EFI_OUT_OF_RESOURCES;
    }
    AllocatedRequest = TRUE;
    UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", ConfigRequestHdr, (UINT64)BufferSize);
    FreePool (ConfigRequestHdr);
  }

  //
  // Convert Buffer Data to <ConfigResp> by helper function BlockToConfig( )
  //
  Status = gHiiConfigRouting->BlockToConfig (
               gHiiConfigRouting,
               ConfigRequest,
               (UINT8 *)NvmeHii,
               BufferSize,
               Results,
               Progress
               );
  FreePool (NvmeHii);
  //
  // Free the allocated config request string.
  //
  if (AllocatedRequest) {
    FreePool (ConfigRequest);
    ConfigRequest = NULL;
  }

  //
  // Set Progress string to the original request string.
  //
  if (Request == NULL) {
    *Progress = NULL;
  } else if (StrStr (Request, L"OFFSET") == NULL) {
    *Progress = Request + StrLen (Request);
  }

  return (Status);
}

/**
  This function processes the results of changes in configuration.

  @param[in]   This           Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]   Action         Specifies the type of action taken by the browser.
  @param[in]   QuestionId     A unique value which is sent to the original
                              exporting driver so that it can identify the type
                              of data to expect.
  @param[in]   Type           The type of value for the question.
  @param[in]   Value          A pointer to the data being sent to the original
                              exporting driver.
  @param[out]  ActionRequest  On return, points to the action requested by the
                              callback function.

  @retval  EFI_SUCCESS           The callback successfully handled the action.
  @retval  EFI_OUT_OF_RESOURCES  Not enough storage is available to hold the
                                 variable and its data.
  @retval  EFI_DEVICE_ERROR      The variable could not be saved.
  @retval  EFI_UNSUPPORTED       The specified Action is not supported by the callback.

**/
EFI_STATUS
EFIAPI
DriverCallback(
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN        EFI_BROWSER_ACTION              Action,
  IN        EFI_QUESTION_ID                 QuestionId,
  IN        UINT8                           Type,
  IN        EFI_IFR_TYPE_VALUE              *Value,
  OUT       EFI_BROWSER_ACTION_REQUEST      *ActionRequest
  )
{
  EFI_STATUS  Status;
  Status = EFI_SUCCESS;

  if ((Action == EFI_BROWSER_ACTION_SUBMITTED) && (QuestionId == NVME_RECOVERY_SUPPORT_QUESTION_ID)) {

    if (Value->u8 == NVME_RECOVERY_FUNCTION_DISABLE_VALUE) {
      Status = gRT->SetVariable (
                      NVME_BASED_RECOVERY_INFO_VARIABLE,
                      &gNvmeBasedRecoveryVarGuid,
                      0,
                      0,
                      NULL
                      );
      if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "NvmeRecoverySupported to be disabled, clear the backup status.\n"));
      }
    }
    return Status;
  }
  return EFI_UNSUPPORTED;
}

/**
  Install the HII related resources.

  @retval  EFI_SUCCESS  Install all the resources success.
  @retval  other        Error occur when install the resources.
**/
EFI_STATUS
NvmeRecoveryHiiInstall (
  VOID
  )
{
  EFI_STATUS  Status;

  //
  // Populate the config access protocol with the three functions we are publishing
  //
  mHiiConfigAccessProtocol.ExtractConfig = ExtractConfig;
  mHiiConfigAccessProtocol.RouteConfig   = RouteConfig;
  mHiiConfigAccessProtocol.Callback      = DriverCallback;

  //
  // Associate the required protocols with our driver handle
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mDriverHandle,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mHiiConfigAccessProtocol,      // HII callback
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath,          // required for HII callback allow all disks to be shown in same hii
                  NULL
                  );

  DEBUG ((DEBUG_INFO, "Install Nvme Based Recovery HII protocol: %r\n", Status));

  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Publish the HII form and HII string packages
  //
  mHiiPackageListHandle = HiiAddPackages(
                            &gNvmeBasedRecoveryHiiGuid,
                            mDriverHandle,
                            NvmeRecoveryDxeStrings,
                            NvmeRecoveryHiiFormBin,
                            (VOID*)NULL
                            );

  //
  // Make sure the packages installed successfully
  //
  if (mHiiPackageListHandle == NULL) {
    DEBUG ((DEBUG_ERROR, "Add Nvme Based Recovery HII package failed\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  return Status;
}
