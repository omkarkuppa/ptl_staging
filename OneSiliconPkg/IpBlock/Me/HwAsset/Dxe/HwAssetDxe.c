/** @file
  Hardware Asset Initialization driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PerformanceLib.h>
#include <Library/PostCodeLib.h>
#include <Library/DxeMeLib.h>
#include <Library/MeUtilsLib.h>
#include <MeBiosPayloadHob.h>
#include <MeState.h>
#include <AsfMsgs.h>
#include "Inventory.h"
#include <Register/PmcRegs.h>

/**
  Detect Warm Reset.

  @retval TRUE         System is after warm reset.
  @retval FALSE        System is not after warm reset.
**/
BOOLEAN
IsWarmReset (
  VOID
  )
{
  VOID    *HobPtr;
  UINT32  PmconVal;

  //
  // CPU might trigger a warm reset in pre-mem phase after G3 to update CPU strap.
  // So use additional "DISB" bit to check if warm reset is triggered after G3
  //
  HobPtr = GetFirstGuidHob (&gMeSavedPmconHobGuid);
  if (HobPtr != NULL) {
    PmconVal = *(UINT32 *) GET_GUID_HOB_DATA (HobPtr);
    if (((PmconVal & B_PMC_PWRM_GEN_PMCON_A_MEM_SR) != 0) && ((PmconVal & B_PMC_PWRM_GEN_PMCON_A_DISB) != 0)) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Constructs all asset tables and send them to FW

  @param[in]  IsMediaTablePushRequested Determines if CSME has requested Media Table in this boot

  @retval EFI_SUCCESS           Table sent
  @retval EFI_OUT_OF_RESOURCES  Unable to allocate necessary AssetTableData data structure.
  @retval Others                BIOS-ME communication error has occured
**/
EFI_STATUS
SendAssetTables2Firmware (
  IN BOOLEAN IsMediaTablePushRequested
  )
{
  HWA_TABLE   *AssetTablesData;
  EFI_STATUS  Status;
  UINT16      TableSize;

  AssetTablesData = AllocateZeroPool (sizeof (HWA_TABLE) + MAX_ASSET_TABLE_ALLOCATED_SIZE);
  if (AssetTablesData == NULL) {
    DEBUG ((DEBUG_WARN, "%a() Error: Could not allocate Memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  TableSize = BuildHwaTable (AssetTablesData, IsMediaTablePushRequested);

  Status = HeciAssetUpdateFwMsg (AssetTablesData, TableSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a() Error: AssetUpdateFwMsg() returned Status %r\n", __FUNCTION__, Status));
  }

  FreePool (AssetTablesData);

  return Status;
}


/**
  Signal an event for ready to boot.
**/
VOID
EFIAPI
HwAssetReadyToBootEvent (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  )
{
  EFI_STATUS              Status;
  ME_BIOS_PAYLOAD_HOB     *MbpHob;
  UINT8                   PlatformBrand;
  BOOLEAN                 SendTables;
  BOOLEAN                 IsMediaTablePushRequested;

  DEBUG ((DEBUG_INFO, "%a() enter\n", __FUNCTION__));
  MbpHob                    = NULL;
  IsMediaTablePushRequested = FALSE;

  //
  // Get the MBP Data.
  //
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL) {
    DEBUG ((DEBUG_WARN, "HwAsset: No MBP Data HOB available\n"));
    return;
  } else {
    PlatformBrand = (UINT8) MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.PlatformBrand;
    IsMediaTablePushRequested = MbpHob->MeBiosPayload.HwaRequest.Available && (BOOLEAN) MbpHob->MeBiosPayload.HwaRequest.Data.Fields.MediaTablePush;
  }

  ///
  /// HW asset tables need to be sent when:
  /// - FW asked for it, or
  /// - FW is alive (brand!=0) and platform isn't booting from a warm reset.
  ///
  SendTables = IsMediaTablePushRequested || (PlatformBrand != NoBrand && !IsWarmReset ());

  if (SendTables) {
    Status = SendAssetTables2Firmware (IsMediaTablePushRequested);
    DEBUG ((DEBUG_INFO, "Send Asset Tables to AMT FW - Status = %r\n", Status));
  }
}

/**
  The driver entry point - Hardware Asset Driver.

  @param[in] ImageHandle          Handle for this drivers loaded image protocol.
  @param[in] SystemTable          EFI system table.

  @retval EFI_SUCCESS             The driver installed without error.
  @retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
**/
EFI_STATUS
EFIAPI
HwAssetDriverEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS            Status;
  UINT32                MeMode;
  EFI_EVENT             HwAssetEvent;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || MeMode != ME_MODE_NORMAL) {
    DEBUG ((DEBUG_INFO, "%a() - not supported\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  ///
  /// Create a Ready to Boot event.
  ///
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  HwAssetReadyToBootEvent,
                  (VOID *) &ImageHandle,
                  &gMePlatformReadyToBootGuid,
                  &HwAssetEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
