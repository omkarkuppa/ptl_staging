/** @file
  ME BIOS Payload Data implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PerformanceLib.h>
#include <IntelRcStatusCode.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeiMeLib.h>
#include <Library/MeShowBufferLib.h>
#include <Library/MeUtilsLib.h>
#include <MeBiosPayloadHob.h>
#include "MbpDebugPrint.h"

/**
  Check if MBP items that might be missing on the first boot are available.
  If not, additional sync messages are executed in order to retrieve them.

  @param[in, out] MbpHob              Pointer to MBP Hob.
**/

VOID
SyncMissingMbpItems (
  IN OUT ME_BIOS_PAYLOAD_HOB *MbpHob
  )
{
  EFI_STATUS Status;

  if (MbpHob->MeBiosPayload.FwCapsSku.Available == FALSE) {
    Status = PeiHeciGetFwCapsSkuMsg (&MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities);
    if (!EFI_ERROR (Status)) {
      MbpHob->MeBiosPayload.FwCapsSku.Available = TRUE;
    }
  }
  if (MbpHob->MeBiosPayload.FwFeaturesState.Available == FALSE) {
    Status = PeiHeciGetFwFeatureStateMsg (&MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures);
    if (!EFI_ERROR (Status)) {
      MbpHob->MeBiosPayload.FwFeaturesState.Available = TRUE;
    }
  }
  if (MbpHob->MeBiosPayload.FwPlatType.Available == FALSE) {
    Status = PeiHeciGetPlatformTypeMsg (&MbpHob->MeBiosPayload.FwPlatType.RuleData);
    if (!EFI_ERROR (Status)) {
      MbpHob->MeBiosPayload.FwPlatType.Available = TRUE;
    }
  }
}

/**
  This routine returns ME-BIOS Payload information.

  @param[out] MbpPtr              ME-BIOS Payload information.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_DEVICE_ERROR        Failed to consume MBP
  @retval EFI_OUT_OF_RESOURCES    Out of resource
**/
EFI_STATUS
PrepareMeBiosPayload (
  OUT  ME_BIOS_PAYLOAD    *MbpPtr
  )
{
  EFI_STATUS          Status;
  MBP_ITEM_HEADER     *MbpItemHeader;
  UINTN               DestSize;
  UINT32              CurrentMbpItem;
  UINT32              MbpItemsCount;
  UINT32              MbpItemLength;
  UINT32              *MbpItems;
  UINT32              MbpItemId;
  VOID                *DestPtr;

  DEBUG ((DEBUG_INFO, "%a() - Start\n", __FUNCTION__));

  MbpItems = AllocateZeroPool (MAX_MBP_ITEMS_SIZE);
  if (MbpItems == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = PeiHeciGetMbpMsg (&MbpItemsCount, MbpItems, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HECI get MBP error: %r!\n", Status));
    FreePool (MbpItems);
    return Status;
  }

  ZeroMem (MbpPtr, sizeof (ME_BIOS_PAYLOAD));

  MbpItemHeader = (MBP_ITEM_HEADER *) MbpItems;

  for (CurrentMbpItem = 0; CurrentMbpItem < MbpItemsCount; CurrentMbpItem++) {
    MbpItemId = MBP_ITEM_ID (MbpItemHeader->Fields.AppId, MbpItemHeader->Fields.ItemId);
    //
    // Item length includes Item Header - need to substract it and convert length to bytes
    //
    MbpItemLength = (MbpItemHeader->Fields.Length - 1) * sizeof (UINT32);
    //
    // Copy data from HECI buffer per size of each MBP item
    //
    switch (MbpItemId) {
      //
      // Insensitive items
      //
      case MBP_ITEM_FW_VERSION_NAME:
        DestSize = sizeof (MbpPtr->FwVersionName);
        DestPtr = &MbpPtr->FwVersionName;
        break;

      case MBP_ITEM_FW_CAPABILITIES:
        DestSize = sizeof (MbpPtr->FwCapsSku.FwCapabilities);
        DestPtr = &MbpPtr->FwCapsSku.FwCapabilities;
        MbpPtr->FwCapsSku.Available = TRUE;
        break;

      case MBP_ITEM_CSE_PLATFORM_TYPE:
        DestSize = sizeof (MbpPtr->FwPlatType.RuleData);
        DestPtr = &MbpPtr->FwPlatType.RuleData;
        MbpPtr->FwPlatType.Available = TRUE;
        break;

      case MBP_ITEM_UNCONFIG_ON_RTC:
        DestSize = sizeof (MbpPtr->UnconfigOnRtcClearState.UnconfigOnRtcClearData);
        DestPtr = &MbpPtr->UnconfigOnRtcClearState.UnconfigOnRtcClearData;
        MbpPtr->UnconfigOnRtcClearState.Available = TRUE;
        break;

      case MBP_ITEM_SHIP_STATE:
        DestSize = sizeof (MbpPtr->FwFeaturesState.FwFeatures);
        DestPtr = &MbpPtr->FwFeaturesState.FwFeatures;
        MbpPtr->FwFeaturesState.Available = TRUE;
        break;

      case MBP_ITEM_ID_FW_ARB_SVN:
        DestSize = sizeof (MbpPtr->ArbSvnState.ArbSvnData);
        DestPtr = &MbpPtr->ArbSvnState.ArbSvnData;
        MbpPtr->ArbSvnState.Available = TRUE;
        break;

      case MBP_ITEM_ID_MEASURED_BOOT:
        DestSize = sizeof (MbpPtr->MeasuredBootSupport.MeasuredBootData);
        DestPtr = &MbpPtr->MeasuredBootSupport.MeasuredBootData;
        MbpPtr->MeasuredBootSupport.Available = TRUE;
        break;

      case MBP_ITEM_HWA_MTU:
        DestSize = sizeof (MbpPtr->HwaRequest.Data);
        DestPtr = &MbpPtr->HwaRequest.Data;
        MbpPtr->HwaRequest.Available = TRUE;
        break;

      case MBP_ITEM_ID_PSR:
        DestSize = sizeof (MbpPtr->Psr);
        DestPtr = &MbpPtr->Psr;
        MbpPtr->Psr.Available = TRUE;
        break;

      case MBP_ITEM_CHIPSET_INIT_VER:
        DestSize = sizeof (MbpPtr->MphyData);
        DestPtr = &MbpPtr->MphyData;
        MbpPtr->MphyData.Available = TRUE;
        break;

      case MBP_ITEM_OEM_SPHY_DATA:
        DestSize = sizeof (MbpPtr->OemSphyData);
        DestPtr = &MbpPtr->OemSphyData;
        MbpPtr->OemSphyData.Available = TRUE;
        break;

      case MBP_ITEM_OEM_NPHY_DATA:
        DestSize = sizeof (MbpPtr->OemNphyData);
        DestPtr = &MbpPtr->OemNphyData;
        MbpPtr->OemNphyData.Available = TRUE;
        break;

      default:
        DEBUG ((DEBUG_WARN, "Unknown MBP Item %d header: %08x\n", CurrentMbpItem, MbpItemHeader->Data));
        MbpItemHeader += MbpItemHeader->Fields.Length;
        continue;
    }

    DEBUG ((DEBUG_INFO, "MBP Item %d header: %08x\n", CurrentMbpItem, MbpItemHeader->Data));
    if (MbpItemLength <= DestSize) {
      DEBUG_CODE (
        ShowBuffer (((UINT8 *)(UINTN)(MbpItemHeader + 1)), MbpItemLength);
        );
      CopyMem (DestPtr, (MbpItemHeader + 1), MbpItemLength);
    } else {
      DEBUG ((DEBUG_INFO, "Data size is larger than destination buffer. This item has not been copied.\n"));
    }
    MbpItemHeader += MbpItemHeader->Fields.Length;
  }

  //
  // Clear buffers from sensitive data
  //
  ZeroMem (MbpItems, MAX_MBP_ITEMS_SIZE);
  FreePool (MbpItems);
  DEBUG ((DEBUG_INFO, "Clear the buffers from MBP Sensitive data.\n"));
  HeciWrapperReset ();

  return Status;
}

/**
  Install MbpData Hob.

  @retval     EFI_SUCCESS        The function completed successfully.
  @retval     EFI_UNSUPPORTED    MBP Hob creation skipped
  @retval     Others             Failed to create MBP Hob
**/
EFI_STATUS
InstallMbpHob (
  VOID
  )
{
  EFI_STATUS              Status;
  ME_BIOS_PAYLOAD         MbpData;
  ME_BIOS_PAYLOAD_HOB     *MbpHob;

  if (!MeSkuIsClient () || MeMbpHobSkipped () || (GetBootModeHob () == BOOT_ON_S3_RESUME)) {
    return EFI_UNSUPPORTED;
  }

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_MBP_ENTRY_START); //PostCode(0xE02)
  DEBUG ((DEBUG_INFO, "ME-BIOS: MBP Entry.\n"));

  PERF_INMODULE_BEGIN ("PrepareMeBiosPayload");
  Status = PrepareMeBiosPayload (&MbpData);
  PERF_INMODULE_END ("PrepareMeBiosPayload");
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: MBP Exit - Get Mbp message has failed, Status: %r\n", Status));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_MBP_EXIT_ERR_GETMBPMSGFAIL); //PostCode (0xE82)
    return EFI_DEVICE_ERROR;
  }

  ///
  /// Create MBP HOB
  ///
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (ME_BIOS_PAYLOAD_HOB),
             (VOID**) &MbpHob
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: MBP Exit - MeBiosPayloadHob creation has failed, Status: %r\n", Status));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_MBP_EXIT_ERR_PAYLOAD_FAIL); //PostCode (0xEA2)
    return Status;
  }

  //
  // Initialize MBP HOB
  //
  MbpHob->Header.Name = gMeBiosPayloadHobGuid;
  MbpHob->Revision    = MBP_DATA_REVISION;
  CopyMem (&MbpHob->MeBiosPayload, &MbpData, sizeof (ME_BIOS_PAYLOAD));
  SyncMissingMbpItems (MbpHob);

  DEBUG_CODE (
    PrintMbpData (MbpHob);
    );

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_MBP_EXIT_SUCCESS); //PostCode(0xE22)
  DEBUG ((DEBUG_INFO, "ME-BIOS: MBP Exit - Success.\n"));
  return EFI_SUCCESS;
}