/** @file
  Platform hook to get BIOS information on platform or perfrom platform specific actions.

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
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PmcLib.h>
#include <Register/PmcRegs.h>
#include <Library/BiosIdLib.h>
#include <BootStateLib.h>
#include <SetupVariable.h>
#include <Library/PlatformBiosUpdateHookLib.h>

#define BIOS_ID_STRING_SIZE       (sizeof (BIOS_ID_STRING))

typedef struct {
  CHAR16                        *Name;
  EFI_GUID                      *Guid;
} USER_DATA_VARIABLE_ENTRY;

//
// Setup related variables that would be cleared after BIOS update if requested.
//
USER_DATA_VARIABLE_ENTRY mUserDataVariableList[] = {
  {L"EnableCustomDefaultsState", &gOsVendorGlobalVariableGuid },
  {L"Setup"               , &gSetupVariableGuid           },
  {L"MeInfoSetup"         , &gMeInfoSetupGuid             },
};

BOOLEAN                 mIsLoadSetupDefaultAfterBiosUpdate = FALSE;

/**
  Delete SETUP related Variables

  @retval     EFI_SUCCESS.             Operation is successful.

**/
EFI_STATUS
DeleteSetupVariables (
  VOID
  )
{
  EFI_STATUS        Status;
  UINTN             Index;

  DEBUG ((DEBUG_INFO, "Delete Setup User Data\n"));

  for (Index = 0; Index < sizeof (mUserDataVariableList) / sizeof (mUserDataVariableList[0]); Index++) {
     Status = gRT->SetVariable (
                     mUserDataVariableList[Index].Name,
                     mUserDataVariableList[Index].Guid,
                     0,
                     0,
                     NULL
                     );
     if (EFI_ERROR (Status)) {
       DEBUG ((DEBUG_INFO, "Failed to delete variable %s (%r)\n", mUserDataVariableList[Index].Name, mUserDataVariableList[Index].Guid));
     }
  }

  return EFI_SUCCESS;
}

/**
  Clear assigned non-volatile variables after BIOS update.

**/
VOID
ClearBiosNvData (
  VOID
  )
{
  if (mIsLoadSetupDefaultAfterBiosUpdate) {
    DeleteSetupVariables ();
  }

  //
  // Delete PdtExist non-volatile variable to update PDT after BIOS Capsule update.
  //
  gRT->SetVariable (
         L"PdtExist",
         &gPdtExistGuid,
         0,
         0,
         NULL
         );

  return;
}

/**
  This function will clear the DISB - DRAM Initialization Scratchpad Bit.
**/
VOID
ClearDramInitScratchpad (
  VOID
  )
{
  //
  // Clear B_PMC_PWRM_GEN_PMCON_A_DISB.
  //
  MmioAnd8 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 2, (UINT8) ~ ((B_PMC_PWRM_GEN_PMCON_A_DISB) >> 16));
}

/**
  Returns the Null-terminated Unicode string that is used to fill in the
  VersionName field of the EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is
  returned by the GetImageInfo() service of the Firmware Management Protocol.
  The returned string must be allocated using EFI_BOOT_SERVICES.AllocatePool().

  @note It is recommended that all firmware devices support a method to report
        the VersionName string from the currently stored firmware image.

  @param[out] VersionString  The version string retrieved from the currently
                             stored firmware image.

  @retval EFI_SUCCESS            The version string of currently stored
                                 firmware image was returned in Version.
  @retval EFI_INVALID_PARAMETER  VersionString is NULL.
  @retval EFI_UNSUPPORTED        The firmware device does not support a method
                                 to report the version string of the currently
                                 stored firmware image.
  @retval EFI_DEVICE_ERROR       An error occurred attempting to retrieve the
                                 version string of the currently stored
                                 firmware image.
  @retval EFI_OUT_OF_RESOURCES   There are not enough resources to allocate the
                                 buffer for the version string of the currently
                                 stored firmware image.

**/
EFI_STATUS
EFIAPI
PlatformFmpGetBiosVersionString (
  OUT CHAR16  **VersionString
  )
{
  if (VersionString == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *VersionString = (CHAR16 *) AllocateZeroPool (BIOS_ID_STRING_SIZE);

  if (*VersionString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  GetBiosVersionDateTime (*VersionString, NULL, NULL);

  return EFI_SUCCESS;
}

/**
  Returns BIOS FMP version on the platform

  Returns the value used to fill in the Version field of the
  EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is returned by the GetImageInfo()
  service of the Firmware Management Protocol.  If EFI_SUCCESS is returned, then
  the firmware device supports a method to report the Version value from the
  currently stored firmware image.  If the value can not be reported for the
  firmware image currently stored in the firmware device, then EFI_UNSUPPORTED
  must be returned.  EFI_DEVICE_ERROR is returned if an error occurs attempting
  to retrieve the LowestSupportedVersion value for the currently stored firmware
  image.

  @note It is recommended that all firmware devices support a method to report
        the Version value from the currently stored firmware image.

  @param[out] Version  The version value retrieved from the currently stored
                       firmware image.

  @retval EFI_SUCCESS       The version of currently stored firmware image was
                            returned in Version.
  @retval EFI_UNSUPPORTED   The firmware device does not support a method to
                            report the version of the currently stored firmware
                            image.
  @retval EFI_DEVICE_ERROR  An error occurred attempting to retrieve the version
                            of the currently stored firmware image.

**/
EFI_STATUS
EFIAPI
PlatformFmpGetBiosVersion (
  OUT UINT32  *Version
  )
{
  EFI_STATUS                        Status;
  BIOS_ID_IMAGE                     BiosIdImage;
  CHAR16                            MajorVersion[10];
  CHAR16                            MinorVersion[10];

  ZeroMem (&BiosIdImage, sizeof (BIOS_ID_IMAGE));
  ZeroMem (MajorVersion, sizeof (CHAR16) * 10);
  ZeroMem (MinorVersion, sizeof (CHAR16) * 10);

  Status = GetBiosId (&BiosIdImage);
  if (Status == EFI_SUCCESS) {
    StrnCpyS (MajorVersion, 5, BiosIdImage.BiosIdString.VersionMajor, 4);
    StrnCpyS (MinorVersion, 5, BiosIdImage.BiosIdString.VersionMinor, 4);
    *Version = (UINT32) ((StrDecimalToUintn (MajorVersion) * 100) + StrDecimalToUintn (MinorVersion));
  }

  return Status;
}

/**
  A function used to check if the BIOS image is compatible to current platform.

  @param[in]  Image           Points to a new BIOS image.
  @param[in]  ImageSize       Size, in bytes, of the new BIOS image.
  @param[out] ImageUpdatable  Indicates if a BIOS firmware image is valid for current platform.

  @retval EFI_SUCCESS         The fucntion is supported.
  @retval EFI_UNSUPPORTED     The function is not supported.

**/
EFI_STATUS
EFIAPI
PlatformFmpCheckBiosCompatibility (
  IN  CONST VOID  *BiosImage,
  IN  UINTN       BiosImageSize,
  OUT BOOLEAN     *BiosImageUpdatable
  )
{
  return EFI_UNSUPPORTED;
}

/**
  A function used to perform platform related task before BIOS update.

  @retval EFI_SUCCESS           Platform required actions are perfromed successfully.
  @retval Others                Platform required actions are failed to execute.

**/
EFI_STATUS
EFIAPI
PlatformFmpBiosUpdatePreHook (
  VOID
  )
{
  UINTN                      VariableSize = 0;
  SETUP_DATA                 SetupData;

  ZeroMem (&SetupData, sizeof (SetupData));

  //
  // BIOS and Monolithic FMP
  //
  VariableSize = sizeof (SETUP_DATA);
  gRT->GetVariable (
         L"Setup",
         &gSetupVariableGuid,
         NULL,
         &VariableSize,
         &SetupData
         );

  mIsLoadSetupDefaultAfterBiosUpdate = (SetupData.SysFwUpdateLoadDefault != 0) ? TRUE : FALSE;

  return EFI_SUCCESS;
}

/**
  A function used to perform platform related task after BIOS update.

  @param[in]   UpdateStatus     The status of BIOS update.

  @retval EFI_SUCCESS           Platform required actions are perfromed successfully.
  @retval Others                Platform required actions are failed to execute.

**/
EFI_STATUS
EFIAPI
PlatformFmpBiosUpdatePostHook (
  IN EFI_STATUS            UpdateStatus
  )
{
  if (UpdateStatus == EFI_SUCCESS) {
    ClearBiosNvData ();
    //
    // Enforce booting with full configuration after flash
    //
    UnsetBootState ();
    //
    // Clear the DRAM DISB bit
    //
    ClearDramInitScratchpad ();
  }

  return EFI_SUCCESS;
}

/**
  A function used to perform platform related task after uCode update.

  @param[in]  UpdateStatus  The status of uCode update.

  @retval  EFI_SUCCESS  Platform required actions are performed successfully.
  @retval  Others       Platform required actions are failed to execute.

**/
EFI_STATUS
EFIAPI
PlatformFmpUcodeUpdatePostHook (
  IN EFI_STATUS  UpdateStatus
  )
{
  if (!EFI_ERROR (UpdateStatus)) {
    //
    // Clear the DRAM DISB bit
    //
    ClearDramInitScratchpad ();
  }

  return EFI_SUCCESS;
}

/**
  A function used to perform platform related task after BtGAcm update.

  @param[in]  UpdateStatus  The status of BtGAcm update.

  @retval  EFI_SUCCESS  Platform required actions are performed successfully.
  @retval  Others       Platform required actions are failed to execute.

**/
EFI_STATUS
EFIAPI
PlatformFmpBtGAcmUpdatePostHook (
  IN EFI_STATUS  UpdateStatus
  )
{
  if (!EFI_ERROR (UpdateStatus)) {
    //
    // Clear the DRAM DISB bit
    //
    ClearDramInitScratchpad ();
  }

  return EFI_SUCCESS;
}
