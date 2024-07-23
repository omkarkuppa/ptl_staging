/** @file
  This file contains DXE driver for testing and publishing HSTI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#include "HstiIhvDxe.h"

ADAPTER_INFO_PLATFORM_SECURITY_STRUCT  mHstiStruct = {
  PLATFORM_SECURITY_VERSION_VNEXTCS,
  PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
  {HSTI_PLATFORM_NAME},
  HSTI_SECURITY_FEATURE_SIZE,
  {0}, // SecurityFeaturesRequired
  {0}, // SecurityFeaturesImplemented
  {0}, // SecurityFeaturesVerified
  0,
};

UINT8  mFeatureRequired[HSTI_SECURITY_FEATURE_SIZE] = {
  //
  // Byte 0
  //
  HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY |
  HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION |
  HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE |
  HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT |
  HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION |
  HSTI_BYTE0_DEBUG_INTERFACE_SECURITY_CONFIGURATION |
  HSTI_BYTE0_SECURE_CPU_CONFIGURATION,
  //
  // Byte 1
  //
  HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION |
  HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION |
  HSTI_BYTE1_SECURE_PCH_CONFIGURATION,
  //
  // Byte 2
  //
  0,
};

UINT8  mFeatureImplemented[HSTI_SECURITY_FEATURE_SIZE] = {
  //
  // Byte 0
  //
  HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY |
  HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION |
  HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE |
  HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT |
  HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION |
  HSTI_BYTE0_DEBUG_INTERFACE_SECURITY_CONFIGURATION |
  HSTI_BYTE0_SECURE_CPU_CONFIGURATION,
  //
  // Byte 1
  //
  HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION |
  HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION |
  HSTI_BYTE1_SECURE_PCH_CONFIGURATION,
  //
  // Byte 2
  //
  0,
};

DXE_SI_POLICY_PROTOCOL *mSiPolicyData;

/**
  Initialize HSTI feature data
**/
VOID
InitData (
  VOID
  )
{
  EFI_STATUS  Status;

  ADAPTER_INFO_PLATFORM_SECURITY  *Hsti;
  UINT8                           *SecurityFeatures;
  UINTN                           Index;
  UINT16                          IgdVendorId;

  if ((mSiPolicyData != NULL) && (mSiPolicyData->Hsti != NULL)) {

    ///
    /// Take cached HSTI feature bitmap data pointed to by policy and publish to OS
    ///
    Hsti = mSiPolicyData->Hsti;

    SecurityFeatures = (UINT8 *) (Hsti + 1);
    DEBUG ((DEBUG_INFO, "  SecurityFeaturesRequired    - "));
    for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
      DEBUG ((DEBUG_INFO, "%02x ", SecurityFeatures[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
    CopyMem (
      mHstiStruct.SecurityFeaturesRequired,
      SecurityFeatures,
      sizeof (mFeatureRequired)
      );

    SecurityFeatures = (UINT8 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
    DEBUG ((DEBUG_INFO, "  SecurityFeaturesImplemented - "));
    for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
      DEBUG ((DEBUG_INFO, "%02x ", SecurityFeatures[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
    CopyMem (
      mHstiStruct.SecurityFeaturesImplemented,
      SecurityFeatures,
      sizeof (mFeatureImplemented)
      );

    SecurityFeatures = (UINT8 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
    DEBUG ((DEBUG_INFO, "  SecurityFeaturesVerified    - "));
    for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
      DEBUG ((DEBUG_INFO, "%02x ", SecurityFeatures[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
    CopyMem (
      mHstiStruct.SecurityFeaturesVerified,
      SecurityFeatures,
      sizeof (mFeatureImplemented)
      );
  } else {
    ///
    /// Set only the bitmaps not related to verified, those will get updated during test process
    ///
    CopyMem (
      mHstiStruct.SecurityFeaturesRequired,
      mFeatureRequired,
      sizeof (mFeatureRequired)
      );
    CopyMem (
      mHstiStruct.SecurityFeaturesImplemented,
      mFeatureImplemented,
      sizeof (mFeatureImplemented)
      );
  }

  //
  // Read Integrated Graphics Device
  //
  IgdVendorId = IGpuGetVendorId () & B_PCI_VENDOR_ID_MASK;

  if (IgdVendorId == B_PCI_VENDOR_ID_MASK) {
    //
    // Clear required & implemented HSTI flags since there is no IGD detected
    // We can skip this test.
    //
    mHstiStruct.SecurityFeaturesRequired[1] = (mHstiStruct.SecurityFeaturesRequired[1] & ~HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION);
    mHstiStruct.SecurityFeaturesImplemented[1] = (mHstiStruct.SecurityFeaturesImplemented[1] & ~HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION);
  }

  Status = HstiLibSetTable (
             &mHstiStruct,
             sizeof (mHstiStruct)
             );
  if (EFI_ERROR (Status)) {
    if (Status != EFI_ALREADY_STARTED) {
      DEBUG ((DEBUG_INFO, "  There is already HSTI table with Role and ImplementationID published in system\n"));
      ASSERT_EFI_ERROR (Status);
    }
  }
}

/**
  Check HSTI Library Set was Successful
**/
VOID
CheckStatusForHstiLibSet (
  IN EFI_STATUS   Status
  )
{
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "  There is no HSTI Table Published or ByteIndex is Invalid\n"));
    ASSERT_EFI_ERROR (Status);
  }
}

/**
  Check HSTI Library Set was Successful
**/
VOID
CheckStatusForHstiLibAppend (
  IN EFI_STATUS   Status
  )
{
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "  There is no HSTI Table Published or not enough system \
                           resources to update Status String\n"));
    ASSERT_EFI_ERROR (Status);
  }
}

/**
  Concatenate Status string.

  @param[in] StatusCodeString     - Status Code
  @param[in] StatusString         - Status Text
  @param[out] StringSizeOut       - Return String size for memory deallocation

  @retval CHAR16 - Concatenated string.
  Note: This function will always return a string. In case of error while
         trying to create the Concatenated String, it will as well return
         a string for Invalid output Parameter or out of resources error.
**/
CHAR16 *
EFIAPI
BuildHstiStatusStringAndSize (
  IN  CHAR16                   *StatusCodeString,
  IN  CHAR16                   *StatusString,
  OUT UINTN                    *StringSizeOut
  )
{
  UINTN                        Offset;
  UINTN                        TotalStringSize;
  UINTN                        StatusCodeStringSize;
  UINTN                        StatusStringSize;
  CHAR16                       *StatusStringOut;

  StatusStringOut          = NULL;
  Offset                   = 0;
  TotalStringSize          = 0;
  StatusCodeStringSize     = 0;
  StatusStringSize         = 0;

  if ((StatusCodeString == NULL) ||
      (StatusString == NULL)) {
    DEBUG ((DEBUG_INFO, "  %s", HSTI_BUILD_STRING_ERROR));
    return HSTI_BUILD_STRING_ERROR;
  }

  StatusCodeStringSize     = StrSize (StatusCodeString);
  StatusStringSize         = StrSize (StatusString);
  TotalStringSize          = StatusCodeStringSize +
                             StatusStringSize;
  StatusStringOut          = AllocateZeroPool (TotalStringSize);

  if (StatusStringOut == NULL) {
    DEBUG ((DEBUG_INFO, "  %s", HSTI_OUT_OF_RESOURCES_ERROR));
    return HSTI_OUT_OF_RESOURCES_ERROR;
  }

  CopyMem (
    StatusStringOut + Offset,
    StatusCodeString,
    (StatusCodeStringSize - 1)
    );
  Offset += StrLen (StatusCodeString);

  CopyMem (
    StatusStringOut + Offset,
    StatusString,
    StatusStringSize
    );
  Offset += StrLen (StatusString);

  *StringSizeOut = TotalStringSize;

  return  StatusStringOut;
}

/**
  Concatenate Status string and Append HSTI Status String if it hasn't been reported yet.
  The codes are tracked by externally provided bitmap.
  If CodeReportedBitmap[CodeIndexBit] is not set, append the string
  Otherwise do nothing

  @param[in] StatusCodeString     - Status Code
  @param[in] StatusString         - Status Text
  @param[in] CodeReportedBitmap   - Flags for which codes have been used
  @param[in] CodeIndexBit         - Flag index for reported code

  @retval EFI_SUCCESS            The Status String was successfully build and saved in HSTI Status Structure.
**/
EFI_STATUS
EFIAPI
BuildAndAppendHstiUniqueStatusString (
  IN      CHAR16  *StatusCodeString,
  IN      CHAR16  *StatusString,
  IN OUT  UINT64  *CodeReportedBitmap,
  IN      UINT64  CodeIndexBit
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (!(*CodeReportedBitmap & CodeIndexBit)) {
    Status = BuildAndAppendHstiStatusString (StatusCodeString, StatusString);
    if (!EFI_ERROR (Status)){
      *CodeReportedBitmap |= CodeIndexBit;
    }
  }
  return Status;
}

/**
  Concatenate Status string and Append HSTI Status String.

  @param[in] StatusCodeString     - Status Code
  @param[in] StatusString         - Status Text

  @retval EFI_SUCCESS            The Status String was successfully build and saved in HSTI Status Structure.
**/
EFI_STATUS
EFIAPI
BuildAndAppendHstiStatusString (
  IN  CHAR16                   *StatusCodeString,
  IN  CHAR16                   *StatusString
  )
{
  EFI_STATUS                   Status;
  UINTN                        TotalStringSize;
  CHAR16                       *StatusStringOut;

  StatusStringOut          = NULL;
  TotalStringSize          = 0;

  StatusStringOut = BuildHstiStatusStringAndSize (StatusCodeString,
                      StatusString,
                      &TotalStringSize
                      );

  Status = HstiLibAppendErrorString (
             PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
             NULL,
             StatusStringOut
             );
  CheckStatusForHstiLibAppend (Status);

  if (StatusStringOut != NULL) {
    ZeroMem (StatusStringOut, TotalStringSize);
    FreePool (StatusStringOut);
  }

  return Status;
}

/**
  Update HSTI feature data from cached results or rerun tests
**/
VOID
UpdateData (
  VOID
  )
{
  ADAPTER_INFO_PLATFORM_SECURITY *Hsti;
  UINT8                          *SecurityFeatures;
  CHAR16                         *StatusString;


  if ((mSiPolicyData != NULL) && (mSiPolicyData->Hsti != NULL)) {

    Hsti = mSiPolicyData->Hsti;

    SecurityFeatures = (UINT8 *) (Hsti + 1);
    SecurityFeatures = (UINT8 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
    SecurityFeatures = (UINT8 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
    StatusString     = (CHAR16 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);

    HstiLibSetErrorString (PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,NULL,StatusString);

  } else {
    DEBUG ((DEBUG_INFO, "HSTI - Intel Independent Hardware Vendor (Ihv) Tests\n"));


    DEBUG ((DEBUG_INFO, "Hardware Rooted Boot Integrity Configuration\n"));
    CheckHardwareRootedBootIntegrity ();

    DEBUG ((DEBUG_INFO, "Signed Firmware Update Configuration\n"));
    CheckSignedFirmwareUpdate ();

    DEBUG ((DEBUG_INFO, "Measured Boot Enforcement Configuration\n"));
    CheckMeasuredBootEnforcement ();

    DEBUG ((DEBUG_INFO, "Integrated Device DMA Protection Configuration\n"));
    CheckIntegratedDeviceDmaProtection ();

    DEBUG ((DEBUG_INFO, "Secure Debug Interface Configuration\n"));
    CheckSecureDebugInterfaceConfiguration ();

    DEBUG ((DEBUG_INFO, "Secure CPU Configuration\n"));
    CheckSecureCpuConfiguration ();

    DEBUG ((DEBUG_INFO, "Secure Memory Map Configuration\n"));
    CheckSecureMemoryMapConfiguration ();

    DEBUG ((DEBUG_INFO, "Secure Integrated Graphics Configuration\n"));
    CheckSecureIntegratedGraphicsConfiguration ();


    DEBUG ((DEBUG_INFO, "Boot Firmware Media Protection (SPI) Configuration\n"));
    CheckBootFirmwareMediaProtection ();

    DEBUG ((DEBUG_INFO, "Secure PCH Configuration\n"));
    CheckSecurePchConfiguration ();


  }
}

/**
  Dump HSTI info to serial

  @param[in] HstiData     - Pointer to HSTI data
**/
VOID
DumpHsti (
  IN VOID                     *HstiData
  )
{
  ADAPTER_INFO_PLATFORM_SECURITY *Hsti;
  UINT8                          *SecurityFeatures;
  CHAR16                         *StatusString;
  UINTN                          Index;
  CHAR16                         StatusChar;
  UINTN                          DebugIndex;
  CHAR16                         StatusDebugString [MAX_DEBUG_HSTI_MESSAGE_LENGTH];

  Hsti       = HstiData;
  DebugIndex = 0;

  DEBUG ((DEBUG_INFO, "\nHSTI\n"));
  DEBUG ((DEBUG_INFO, "  Version                     - 0x%08x\n", Hsti->Version));
  DEBUG ((DEBUG_INFO, "  Role                        - 0x%08x\n", Hsti->Role));
  DEBUG ((DEBUG_INFO, "  ImplementationID            - %S\n", Hsti->ImplementationID));
  DEBUG ((DEBUG_INFO, "  SecurityFeaturesSize        - 0x%08x\n", Hsti->SecurityFeaturesSize));

  SecurityFeatures = (UINT8 *) (Hsti + 1);
  if (Hsti->Role == PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE) {
    DEBUG ((DEBUG_INFO, "  SecurityFeaturesRequired    - "));
    for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
      DEBUG ((DEBUG_INFO, "%02x ", SecurityFeatures[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
    SecurityFeatures = (UINT8 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
  }

  DEBUG ((DEBUG_INFO, "  SecurityFeaturesImplemented - "));
  for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
    DEBUG ((DEBUG_INFO, "%02x ", SecurityFeatures[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  SecurityFeatures = (UINT8 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
  DEBUG ((DEBUG_INFO, "  SecurityFeaturesVerified    - "));
  for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
    DEBUG ((DEBUG_INFO, "%02x ", SecurityFeatures[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  StatusString = (CHAR16 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
  DEBUG ((DEBUG_INFO, "  UnexpectedStatusString        - \""));
  CopyMem (&StatusChar, StatusString, sizeof (StatusChar));

  ZeroMem (StatusDebugString, sizeof (StatusDebugString));
  // HSTI UEFI Structure is not 16-bit aligned for this reason
  // Status String may not be always 16-bit aligned (difficulting
  // Status String manipulation using string functions)
  // Due to this limitation, previously, code was printing to Serial Comm port
  // character by character, now with this workaround implementation, code
  // will print 128 characters to Serial Comm port instead of one by one
  // For this reason below code is implemented to working around
  // this string handling (require 16-bit alignment) limitation
  while (StatusChar != 0) {
    // Allocate two characters: one for last character and one for Null terminator
    if (DebugIndex < MAX_DEBUG_HSTI_MESSAGE_LENGTH - 2) {
      if (StatusChar == '\n') {
        StatusDebugString[DebugIndex] = StatusChar;
        StatusDebugString[DebugIndex + 1] = '\0';
        DEBUG((DEBUG_INFO, "%s", StatusDebugString));
        ZeroMem (StatusDebugString, sizeof (StatusDebugString));
        DebugIndex = 0;
      } else {
        StatusDebugString[DebugIndex] = StatusChar;
        DebugIndex++;
      }
    } else {
      StatusDebugString[DebugIndex] = StatusChar;
      StatusDebugString[DebugIndex + 1] = L'\0';

      DEBUG((DEBUG_INFO, "%s", StatusDebugString));
      ZeroMem (StatusDebugString, sizeof (StatusDebugString));
      DebugIndex = 0;
    }

    StatusString++;
    CopyMem (&StatusChar, StatusString, sizeof (StatusChar));
    // Make sure last message is completely sent prior leaving the loop
    if ((StatusChar == 0) && (DebugIndex < MAX_DEBUG_HSTI_MESSAGE_LENGTH - 1)) {
      StatusDebugString[DebugIndex] = StatusChar;
      DEBUG((DEBUG_INFO, "%s", StatusDebugString));
    }
  }
  DEBUG ((DEBUG_INFO, "\"\n"));

 ZeroMem (StatusDebugString, sizeof (StatusDebugString));
}

/**
  Retrieve HSTI Table from AIP
**/
VOID
DumpData (
  VOID
  )
{
  VOID        *Hsti;
  UINTN       HstiSize;
  EFI_STATUS  Status;

  Status = HstiLibGetTable (
             PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
             NULL,
             &Hsti,
             &HstiSize
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HSTI (Role - 0x%08x) not found!\n", PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE));
    return;
  }

  if (mSiPolicyData != NULL) {
    mSiPolicyData->Hsti = (ADAPTER_INFO_PLATFORM_SECURITY *) Hsti;
    mSiPolicyData->HstiSize = HstiSize;
  }

  DumpHsti (Hsti);
}

/**
  Handler to gather and publish HSTI results on ReadyToBootEvent

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context
**/
VOID
EFIAPI
OnReadyToBoot (
  EFI_EVENT                               Event,
  VOID                                    *Context
  )
{
  EFI_HANDLE                  Handle;
  EFI_STATUS                  Status;

  InitMp ();
  InitData ();
  UpdateData ();
  DumpData ();

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gHstiPublishCompleteProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
}

/**
  The driver's entry point.

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS     The entry point is executed successfully.
  @retval other           Some error occurs when executing this entry point.
**/
EFI_STATUS
EFIAPI
HstiIhvDxeEntrypoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;

  //
  // Locate DxeSiPolicyProtocolGuid protocol instance and assign it to a global variable
  //
  Status = gBS->LocateProtocol (
                  &gDxeSiPolicyProtocolGuid,
                  NULL,
                  (VOID **) &mSiPolicyData
                  );

  if (EFI_ERROR (Status)) {
    mSiPolicyData = NULL;
    DEBUG ((DEBUG_ERROR,"Failed to locate DxeSiPolicyProtocolGuid Protocol\n"));
    return Status;
  }

  Status = gBS->InstallProtocolInterface (
                  &gImageHandle,
                  &gHstiProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HSTI Protocol Installation Failed\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = EfiCreateEventReadyToBootEx (
    TPL_NOTIFY,
    OnReadyToBoot,
    NULL,
    &Event
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Status = %r\n", Status));
  }

  return EFI_SUCCESS;
}
