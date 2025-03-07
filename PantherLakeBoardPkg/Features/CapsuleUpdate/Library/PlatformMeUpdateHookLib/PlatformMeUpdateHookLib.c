/** @file
  Platform hook to get ME information on platform or perform platform
  specific actions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
//
// Silicon Foundation.
//
#include <MkhiMsgs.h>
#include <Library/PchInfoLib.h>
//
// AFP Foundation.
//
#include <Library/PlatformMeUpdateHookLib.h>
#include <Protocol/MeFirmwareUpdate.h>
//
// Specific CSME FMP device ImageTypeGuid.
//
extern EFI_GUID  gFmpDeviceMeGen21ConsGuid;
extern EFI_GUID  gFmpDeviceMeGen21CorpGuid;

typedef struct {
  PCH_GENERATION  PchGeneration;
  PCH_SERIES      SocSeries;
  UINT8           MeImageType;
  EFI_GUID        *ImageTypeIdGuid;
} ME_IMAGE_TYPE_ID_GUID_INFO;

ME_IMAGE_TYPE_ID_GUID_INFO  mMeFwImageTypeIdGuidList[] = {
  //
  // CSME Generation 21
  //
  { PTL_SOC, PCH_P, IntelMeConsumerFw,  &gFmpDeviceMeGen21ConsGuid },
  { PTL_SOC, PCH_P, IntelMeCorporateFw, &gFmpDeviceMeGen21CorpGuid },
  { PTL_SOC, PCH_H, IntelMeConsumerFw,  &gFmpDeviceMeGen21ConsGuid },
  { PTL_SOC, PCH_H, IntelMeCorporateFw, &gFmpDeviceMeGen21CorpGuid }
};

/**
  Return the first Protocol Interface that matches the Protocol GUID.

  Expect platform should only have one FWUpdateLib instance in a time.

  @param[in]  MeFwu  Pointer to firmware update protocol.

  @retval  EFI_SUCCESS            If a valid Interface is returned.
  @retval  EFI_INVALID_PARAMETER  Invalid parameter.
  @retval  EFI_NOT_FOUND          Protocol interface not found.

**/
EFI_STATUS
GetMeFwuProtocolInstance (
  IN ME_FIRMWARE_UPDATE_PROTOCOL  **MeFwu
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  //
  // It should be only one FWUpdate instance driver could be installed.
  //
  Status = gBS->LocateProtocol (&gMeFwuProtocol, NULL, (void **)MeFwu);
  DEBUG ((DEBUG_INFO, "Locate MeFwuProtocol Status: %r\n", Status));

  return Status;
}

/**
  Returns the GUID value used to fill in the ImageTypeId field of the
  EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is returned by the GetImageInfo ()
  service of the Firmware Management Protocol.

  @param[in, out]  Guid  Double pointer to a GUID value that is updated to point
                         to a GUID value. The GUID value is not allocated and must
                         not be modified or freed by the caller.

  @retval  EFI_SUCCESS            Found the matched platform configuration.
  @retval  EFI_INVALID_PARAMETER  Input Guid is NULL pointer.
  @retval  EFI_UNSUPPORTED        MeBiosPayloadHob is NULL.
  @retval  EFI_NOT_FOUND          Not found the matched platform configuration.

**/
EFI_STATUS
EFIAPI
PlatformFmpGetMeTypeIdGuidPtr (
  IN OUT EFI_GUID  **Guid
  )
{
  PCH_GENERATION               CurrentPchGen;
  PCH_SERIES                   CurrentSocSeries;
  UINT32                       CurrentMeImgType;
  UINT8                        Index;
  EFI_STATUS                   Status;
  ME_FIRMWARE_UPDATE_PROTOCOL  *MeFwu;

  CurrentPchGen    = PchGeneration ();
  CurrentSocSeries = PchSeries ();
  CurrentMeImgType = 0;
  Index            = 0;

  if (Guid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetMeFwuProtocolInstance (&MeFwu);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  if (MeFwu->FwType (&CurrentMeImgType) != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a: - Failed to get CSME FW image type.\n", __func__));
    return EFI_UNSUPPORTED;
  }

  //
  // Iteration all the possible platform configuration.
  //
  for (Index = 0; Index < sizeof (mMeFwImageTypeIdGuidList) / sizeof (ME_IMAGE_TYPE_ID_GUID_INFO); Index++) {
    if (   (CurrentPchGen    == mMeFwImageTypeIdGuidList[Index].PchGeneration) \
        && (CurrentSocSeries == mMeFwImageTypeIdGuidList[Index].SocSeries) \
        && (CurrentMeImgType == mMeFwImageTypeIdGuidList[Index].MeImageType)) {

      *Guid = mMeFwImageTypeIdGuidList[Index].ImageTypeIdGuid;

      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}
