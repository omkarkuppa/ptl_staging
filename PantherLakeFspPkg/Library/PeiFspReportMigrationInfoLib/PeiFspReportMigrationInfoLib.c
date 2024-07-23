/**@file

  Module Name:

    PeiFspMigrationInfoLib.c

  Abstract:

  Report which FVs should must be migrated by FSP when memory is ready.

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

@par Specification
**/

#include <Uefi.h>
#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BootGuardLib.h>
#include <Library/FspFbmSupportLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/MigratedFvInfo.h>
#include <Library/FspCommonLib.h>

/**
  In FSP Signing flow, FSP-M is runtime rebased on NEM and FSP should make sure it will be migrated. However,
  raw data copy is not needed, since the FSP-M loaded to NEM is not expected to be measured in PostMem phase
  In non FSP Signing flow, we don't need migrate FSP-M.

  @retval EFI_SUCCESS             Constructor returns with no error.
  @retval EFI_OUT_OF_RESOURCES    There is no memory to allocate.
**/
EFI_STATUS
EFIAPI
FspReportMigrationInfoConstructor (
  VOID
  )
{
  BSPM_ELEMENT                   *Bsss;
  BOOLEAN                        IsFspmNeedMigrate;
  BOOLEAN                        IsFspmCompressed;
  UINT32                         FspmBaseAddress;
  EFI_PEI_HOB_POINTERS           Hob;
  EDKII_MIGRATION_INFO           *MigrationInfo;
  TO_MIGRATE_FV_INFO             *ToMigrateFvInfo;
  UINTN                          Size;
  UINTN                          Index;

  IsFspmNeedMigrate = FALSE;
  IsFspmCompressed = FALSE;
  Bsss = LocateBspm ();
  if (Bsss != NULL && (Bsss->FspmLoadingPolicy & BIT0) != 0) {
    //
    // When FSPM is loaded by FSP-O, we need migrate FSPM.
    //
    IsFspmNeedMigrate = TRUE;
  }

  if (Bsss != NULL && Bsss->FspmLoadingPolicy & BIT1) {
    //
    // FSPM is compressed, need to migrate FSPM.
    //
    IsFspmCompressed = TRUE;
    IsFspmNeedMigrate = TRUE;
  }

  //
  // Check if gEdkiiMigrationInfoGuid HOB is already published.
  //
  Hob.Raw = GetFirstGuidHob (&gEdkiiMigrationInfoGuid);
  if (Hob.Raw != NULL) {
    MigrationInfo = GET_GUID_HOB_DATA (Hob);
    if (MigrationInfo->MigrateAll) {
      return EFI_SUCCESS;
    }
  }

  //
  // FSPM is loaded by FSP-O, FSP-M FV need be migrated:
  //   - If gEdkiiMigrationInfoGuid HOB is already published, add one entry for FSP-M (when needed);
  //   - If gEdkiiMigrationInfoGuid HOB is not published, create one with FSP-M entry.
  // FSPM is not loaded by FSP-O, no more FVs need migrate to optimize boot performance:
  //   - If gEdkiiMigrationInfoGuid HOB is already published, do nothing.
  //   - If gEdkiiMigrationInfoGuid HOB is not published, create one HOB to indicate no migration.
  //
  if (IsFspmNeedMigrate) {
    FspmBaseAddress = Bsss->FspmBaseAddress;
    if (IsFspmCompressed) {
      //
      // FSP-M was decompressed at PcdSecondaryDataStackBase + 4K to adjust decompression
      // header and align with 4K boundary.
      //
      FspmBaseAddress = PcdGet32 (PcdSecondaryDataStackBase) + SIZE_4KB;
    }
    if (Hob.Raw != NULL) {
      MigrationInfo = GET_GUID_HOB_DATA (Hob);
      for (Index = 0; Index < MigrationInfo->ToMigrateFvCount; Index++) {
        ToMigrateFvInfo = ((TO_MIGRATE_FV_INFO *)(MigrationInfo + 1)) + Index;
        if (ToMigrateFvInfo->FvOrgBaseOnTempRam == FspmBaseAddress) {
          //
          // Return since FSP-M is already set to migrate
          //
          return EFI_SUCCESS;
        }
      }
      //
      // Be careful of this usage!
      // Please ensure this hob producer allocate extra memory for this entry.
      //
      ToMigrateFvInfo = ((TO_MIGRATE_FV_INFO *)(MigrationInfo + 1)) + MigrationInfo->ToMigrateFvCount;
      ToMigrateFvInfo->FvOrgBaseOnTempRam = FspmBaseAddress;
      ToMigrateFvInfo->FvMigrationFlags   = 0;  // Skip raw data copy
      MigrationInfo->ToMigrateFvCount ++;

    } else {
      Size = sizeof (EDKII_MIGRATION_INFO) + sizeof (TO_MIGRATE_FV_INFO);
      MigrationInfo = AllocateZeroPool (Size);
      if (MigrationInfo == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      MigrationInfo->ToMigrateFvCount     = 1;  // We only migrate FSP-M
      ToMigrateFvInfo                     = (TO_MIGRATE_FV_INFO *) (MigrationInfo + 1);
      ToMigrateFvInfo->FvOrgBaseOnTempRam = FspmBaseAddress;
      ToMigrateFvInfo->FvMigrationFlags   = 0;  // Skip raw data copy
      MigrationInfo->MigrateAll           = FALSE;
      BuildGuidDataHob (&gEdkiiMigrationInfoGuid, MigrationInfo, Size);
      FreePool (MigrationInfo);
    }
    DEBUG ((DEBUG_INFO, "FSP-M FV is loaded by FSP-O, it needs be migrated.\n"));

  } else {
    if (Hob.Raw == NULL) {
      Size = sizeof (EDKII_MIGRATION_INFO);
      MigrationInfo = AllocateZeroPool (Size);
      if (MigrationInfo == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      MigrationInfo->ToMigrateFvCount     = 0;
      MigrationInfo->MigrateAll           = FALSE;
      BuildGuidDataHob (&gEdkiiMigrationInfoGuid, MigrationInfo, Size);
      FreePool (MigrationInfo);
      DEBUG ((DEBUG_INFO, "FSP-M FV is not loaded by FSP-O, skip any FV migration.\n"));

    }
  }

  return EFI_SUCCESS;
}