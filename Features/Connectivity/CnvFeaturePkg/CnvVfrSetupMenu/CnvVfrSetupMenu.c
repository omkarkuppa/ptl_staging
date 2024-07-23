/** @file
  This driver loads the CNV VFR Menu Setup Module

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

@par Specification
**/

#include <CnvVfrSetupMenu.h>
#include <Library/HiiLib.h>
#include <Protocol/CnvFormPlatformProtocol.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>

//
// Limited buffer size recommended by RFC4646 (4.3.  Length Considerations)
// (42 characters plus a NULL terminator)
//
#define RFC_3066_ENTRY_SIZE  (42 + 1)

GLOBAL_REMOVE_IF_UNREFERENCED EFI_HII_STRING_PROTOCOL  *gIfrLibHiiString = NULL;

EFI_HII_HANDLE                   mHiiHandle;
EFI_HII_DATABASE_PROTOCOL        *mHiiDatabase;
EFI_HII_CONFIG_ROUTING_PROTOCOL  *mConfigRouting;
EFI_HANDLE                       mDriverHandle;

EFI_HII_CONFIG_ACCESS_PROTOCOL  CnvVfrSetupMenuCallbackProtocol = {
  CnvVfrSetupMenuFormExtractConfig,
  CnvVfrSetupMenuFormRouteConfig,
  CnvVfrSetupMenuFormCallback
};

GLOBAL_REMOVE_IF_UNREFERENCED HII_VENDOR_DEVICE_PATH  mCnvVfrSetupMenuHiiVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    CNV_FEATURE_SETUP_GUID
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
  Returns a pointer to an allocated buffer that contains the best matching language
  from a set of supported languages.

  This function supports both ISO 639-2 and RFC 4646 language codes, but language
  code types may not be mixed in a single call to this function.  The language
  code returned is allocated using AllocatePool().  The caller is responsible for
  freeing the allocated buffer using FreePool().  This function supports a variable
  argument list that allows the caller to pass in a prioritized list of language
  codes to test against all the language codes in SupportedLanguages.

  If SupportedLanguages is NULL, then ASSERT()..

  @param[in] SupportedLanguages   -   A pointer to a Null-terminated ASCII string that
                                      contains a set of language codes in the format
                                      specified by Iso639Language.
  @param[in] Iso639Language       -   If TRUE, then all language codes are assumed to be
                                      in ISO 639-2 format.  If FALSE, then all language
                                      codes are assumed to be in RFC 4646 language format.
  ...                             -   A variable argument list that contains pointers to
                                      Null-terminated ASCII strings that contain one or more
                                      language codes in the format specified by Iso639Language.
                                      The first language code from each of these language
                                      code lists is used to determine if it is an exact or
                                      close match to any of the language codes in
                                      SupportedLanguages.  Close matches only apply to RFC 4646
                                      language codes, and the matching algorithm from RFC 4647
                                      is used to determine if a close match is present.  If
                                      an exact or close match is found, then the matching
                                      language code from SupportedLanguages is returned.  If
                                      no matches are found, then the next variable argument
                                      parameter is evaluated.  The variable argument list
                                      is terminated by a NULL

  @return NULL                    -   The best matching language could not be found in SupportedLanguages.
  @return NULL                    -   There are not enough resources available to return the best matching
                                      language.
  @return Other                   -   A pointer to a Null-terminated ASCII string that is the best matching
                                      language in SupportedLanguages.

**/
CHAR8 *
EFIAPI
HiiLibGetBestLanguage (
  IN CHAR8    *SupportedLanguages,
  IN BOOLEAN  Iso639Language,
  ...
  )
{
  VA_LIST  Args;
  CHAR8    *Language;
  UINTN    CompareLength;
  UINTN    LanguageLength;
  CHAR8    *Supported;
  CHAR8    *BestLanguage;

  ASSERT (SupportedLanguages != NULL);
  if (SupportedLanguages == NULL) {
    return NULL;
  }

  VA_START (Args, Iso639Language);
  while ((Language = VA_ARG (Args, CHAR8 *)) != NULL) {
    //
    // Default to ISO 639-2 mode
    //
    CompareLength  = 3;
    LanguageLength = AsciiStrLen (Language);
    if (LanguageLength > 3) {
      LanguageLength = 3;
    }

    //
    // If in RFC 4646 mode, then determine the length of the first RFC 4646 language code in Language
    //
    if (!Iso639Language) {
      for (LanguageLength = 0; Language[LanguageLength] != 0 && Language[LanguageLength] != ';'; LanguageLength++) {
      }
    }

    //
    // Trim back the length of Language used until it is empty
    //
    while (LanguageLength > 0) {
      //
      // Loop through all language codes in SupportedLanguages
      //
      for (Supported = SupportedLanguages; *Supported != '\0'; Supported += CompareLength) {
        //
        // In RFC 4646 mode, then Loop through all language codes in SupportedLanguages
        //
        if (!Iso639Language) {
          //
          // Skip ';' characters in Supported
          //
          for ( ; *Supported != '\0' && *Supported == ';'; Supported++) {
          }

          //
          // Determine the length of the next language code in Supported
          //
          for (CompareLength = 0; Supported[CompareLength] != 0 && Supported[CompareLength] != ';'; CompareLength++) {
          }

          //
          // If Language is longer than the Supported, then skip to the next language
          //
          if (LanguageLength > CompareLength) {
            continue;
          }
        }

        //
        // See if the first LanguageLength characters in Supported match Language
        //
        if (AsciiStrnCmp (Supported, Language, LanguageLength) == 0) {
          VA_END (Args);
          //
          // Allocate, copy, and return the best matching language code from SupportedLanguages
          //
          BestLanguage = AllocateZeroPool (CompareLength + 1);
          if (BestLanguage == NULL) {
            return NULL;
          }

          CopyMem (BestLanguage, Supported, CompareLength);
          return BestLanguage;
        }
      }

      if (Iso639Language) {
        //
        // If ISO 639 mode, then each language can only be tested once
        //
        LanguageLength = 0;
      } else {
        //
        // If RFC 4646 mode, then trim Language from the right to the next '-' character
        //
        for (LanguageLength--; LanguageLength > 0 && Language[LanguageLength] != '-'; LanguageLength--) {
        }
      }
    }
  }

  VA_END (Args);

  //
  // No matches were found
  //
  return NULL;
}

VOID
EFIAPI
InitString (
  IN EFI_HII_HANDLE  HiiHandle,
  IN EFI_STRING_ID   StrRef,
  IN CHAR16          *StrFormat,
  ...
  )
{
  CHAR16   StringBuffer[1024];
  VA_LIST  ArgList;

  //
  // Construct string value.
  //
  VA_START (ArgList, StrFormat);
  UnicodeVSPrint (StringBuffer, sizeof (StringBuffer), StrFormat, ArgList);
  VA_END (ArgList);

  //
  // Set string
  //
  HiiSetString (HiiHandle, StrRef, StringBuffer, NULL);
}

/**
  Initialize CNV VFR Setup Menu configuration form.

  @retval EFI_SUCCESS              The HDD Password configuration form is initialized.
  @retval EFI_OUT_OF_RESOURCES     Failed to allocate memory.
  @retval Others                   Other errors as indicated.
**/
EFI_STATUS
EFIAPI
InstallCnvVfrSetupMenuForm (
  VOID
  )
{
  EFI_STATUS                  Status;
  CNV_VFR_CONFIG_SETUP        CnvSetup;
  UINTN                       DataSize;
  BOOLEAN                     CnvSetupNeedUpdate;
  CNV_FORM_PLATFORM_PROTOCOL  *CnvFormPlatformProtocol;

  CnvSetupNeedUpdate = FALSE;
  ZeroMem (&CnvSetup, sizeof (CNV_VFR_CONFIG_SETUP));

  Status = gBS->LocateProtocol (
                  &gEfiHiiDatabaseProtocolGuid,
                  NULL,
                  (VOID **)&mHiiDatabase
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (
                  &gEfiHiiConfigRoutingProtocolGuid,
                  NULL,
                  (VOID **)&mConfigRouting
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  mDriverHandle = NULL;
  //
  // Install Device Path Protocol and Config Access protocol to driver handle
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mDriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mCnvVfrSetupMenuHiiVendorDevicePath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &CnvVfrSetupMenuCallbackProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Publish our HII data
  //
  mHiiHandle = HiiAddPackages (
      &gCnvFeatureSetupGuid,
      mDriverHandle,
      CnvVfrSetupMenuStrings,
      CnvVfrSetupMenuVfrBin,
      NULL
  );
  ASSERT (mHiiHandle != NULL);

  Status = gBS->LocateProtocol (&gCnvFormPlatformProtocolGuid, NULL, (VOID **)&CnvFormPlatformProtocol);
  ASSERT_EFI_ERROR (Status);

  InitString (
    mHiiHandle,
    STRING_TOKEN (STR_CNVI_CURRENT_STATE),
    L"%a",
    CnvFormPlatformProtocol->CnvFormData.CnviIsPresent ? "Yes" : "No"
    );

  DataSize = sizeof (CNV_VFR_CONFIG_SETUP);
  Status   = gRT->GetVariable (
                  CNV_SETUP_VARIABLE_NAME,
                  &gCnvFeatureSetupGuid,
                  NULL,
                  &DataSize,
                  &CnvSetup
                  );
  ASSERT_EFI_ERROR (Status);

  if (CnvSetup.CnviIsPresent != CnvFormPlatformProtocol->CnvFormData.CnviIsPresent) {
    CnvSetup.CnviIsPresent = CnvFormPlatformProtocol->CnvFormData.CnviIsPresent;
    CnvSetupNeedUpdate     = TRUE;
  }

  if (CnvSetup.CnviBtInterfaceUsbOnly != CnvFormPlatformProtocol->CnvFormData.CnviBtInterfaceUsbOnly) {
    CnvSetup.CnviBtInterfaceUsbOnly = CnvFormPlatformProtocol->CnvFormData.CnviBtInterfaceUsbOnly;
    CnvSetupNeedUpdate              = TRUE;
  }

  if (CnvSetup.CnviBtAudioOffloadI2sInterfaceOnly != CnvFormPlatformProtocol->CnvFormData.CnviBtAudioOffloadI2sInterfaceOnly) {
    CnvSetup.CnviBtAudioOffloadI2sInterfaceOnly = CnvFormPlatformProtocol->CnvFormData.CnviBtAudioOffloadI2sInterfaceOnly;
    CnvSetupNeedUpdate                          = TRUE;
  }

  if (CnvFormPlatformProtocol->CnvFormData.CnviBtInterfaceUsbUpdate == 1) {
     DEBUG ((DEBUG_INFO, "CnviBtInterfaceUsbUpdate = 1\n"));
     CnvSetup.CnviBtInterface = 1;
     CnvSetupNeedUpdate = TRUE;
  } else if (CnvFormPlatformProtocol->CnvFormData.CnviBtInterfaceUsbUpdate == 2) {
     DEBUG ((DEBUG_INFO, "CnviBtInterfaceUsbUpdate = 2\n"));
     CnvSetup.CnviBtInterface = 2;
     CnvSetupNeedUpdate = TRUE;
  }

  if (CnvSetupNeedUpdate) {
    Status = gRT->SetVariable (
                    CNV_SETUP_VARIABLE_NAME,
                    &gCnvFeatureSetupGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    DataSize,
                    &CnvSetup
                    );
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**
  Initialize CNV VFR Menu setup Form

  @param[in] ImageHandle             Handle for the image of this driver
  @param[in] SystemTable             Pointer to the EFI System Table

  @retval EFI_SUCCESS                The function completed successfully
**/
EFI_STATUS
EFIAPI
CnvVfrSetupMenuInit (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "CnvVfrSetupMenuInit() start\n"));

  //
  // Install CNV VFR Menu Setup Form
  //
  Status = InstallCnvVfrSetupMenuForm ();
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "CnvVfrSetupMenuInit() completed\n"));

  return Status;
}
