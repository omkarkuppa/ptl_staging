/** @file
  ChipsetInit table update library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/Pci22.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SiScheduleResetLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/SocInfoLib.h>
#include <Library/PeiMeLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/ChipsetInitLib.h>
#include <PchResetPlatformSpecific.h>
#include <BupMsgs.h>
#include <MchiMsgs.h>
#include <ChipsetInfoHob.h>
#include <MeBiosPayloadHob.h>
#include <PcrDefine.h>

#if !defined (MDEPKG_NDEBUG)
GLOBAL_REMOVE_IF_UNREFERENCED STATIC CONST CHAR8 *OEM_TABLE_NAME[] = {
  "ChipsetInit",
  "Sphy",
  "Nphy"
};
#endif

STATIC CONST UINT32 OEM_TABLE_FILE_ID[] = {
  0,
  McaSphyFileId,
  McaNphyFileId
};

STATIC CONST UINT32 OEM_TABLE_MAX_SIZE[] = {
  HSIO_CHIPSETINIT_TBL_MAX_SIZE,
  HSIO_SPHY_TBL_MAX_SIZE,
  HSIO_NPHY_TBL_MAX_SIZE
};

/**
  Prints OEM Table Version Info.

  @param[in] CsmeOemTableVerInfo    CSME OEM Table Version Info
  @param[in] BiosOemTableVerInfo    BIOS OEM Table Version Info
  @param[in] OemTableType           OEM table type
**/
STATIC
VOID
PrintOemTableVersionInfo (
  IN OEM_TABLE_VER_INFO   *CsmeOemTableVerInfo,
  IN OEM_TABLE_VER_INFO   *BiosOemTableVerInfo,
  IN OEM_TABLE_TYPE       OemTableType
  )
{
  DEBUG_CODE_BEGIN ();

#if !defined (MDEPKG_NDEBUG)
  DEBUG ((DEBUG_INFO, "CSME OEM %a Version Information:\n", OEM_TABLE_NAME[OemTableType]));
#endif
  DEBUG ((DEBUG_INFO, "Major  = 0x%x\n", CsmeOemTableVerInfo->Major));
  DEBUG ((DEBUG_INFO, "Minor  = 0x%x\n", CsmeOemTableVerInfo->Minor));
  DEBUG ((DEBUG_INFO, "HotFix = 0x%x\n", CsmeOemTableVerInfo->Hotfix));
  DEBUG ((DEBUG_INFO, "Build  = 0x%x\n", CsmeOemTableVerInfo->Build));

#if !defined (MDEPKG_NDEBUG)
  DEBUG ((DEBUG_INFO, "BIOS OEM %a Version Information:\n", OEM_TABLE_NAME[OemTableType]));
#endif
  DEBUG ((DEBUG_INFO, "Major  = 0x%x\n", BiosOemTableVerInfo->Major));
  DEBUG ((DEBUG_INFO, "Minor  = 0x%x\n", BiosOemTableVerInfo->Minor));
  DEBUG ((DEBUG_INFO, "HotFix = 0x%x\n", BiosOemTableVerInfo->Hotfix));
  DEBUG ((DEBUG_INFO, "Build  = 0x%x\n", BiosOemTableVerInfo->Build));

  DEBUG_CODE_END ();
}

/**
  This function gets OEM Table version from CSME

  @param[in]  OemTableType          OEM table type
  @param[out] CsmeOemTableVerInfo   Version Info of OEM Table held by CSME

  @retval TRUE                      OEM version found in MBP
  @retval FALSE                     OEM version was not found in MBP
**/
STATIC
BOOLEAN
IsOemTableVersionPresentInMbp (
  IN  OEM_TABLE_TYPE        OemTableType,
  OUT OEM_TABLE_VER_INFO    *CsmeOemTableVerInfo
  )
{
  ME_BIOS_PAYLOAD_HOB *MbpHob;

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);

  if (NULL == MbpHob) {
    DEBUG ((DEBUG_ERROR, "MBP Hob doesn't exist!\n"));
    return FALSE;
  }

  switch (OemTableType) {
    case OemTableChipsetInit:
      if (MbpHob->MeBiosPayload.MphyData.Available) {
        ///
        /// CSME sends full NVAR table header in case of CSI item, first 4 bytes are API ID
        /// BIOS needs bytes [4:11] to compare the versions
        ///
        CopyMem (CsmeOemTableVerInfo, &MbpHob->MeBiosPayload.MphyData.ChipsetInitVer[1], sizeof (OEM_TABLE_VER_INFO));
        return TRUE;
      }
      break;
    case OemTableSphy:
      if (MbpHob->MeBiosPayload.OemSphyData.Available) {
        CopyMem (CsmeOemTableVerInfo, &MbpHob->MeBiosPayload.OemSphyData.Data, sizeof (OEM_TABLE_VER_INFO));
        return TRUE;
      }
      break;
    case OemTableNphy:
      if (MbpHob->MeBiosPayload.OemNphyData.Available) {
        CopyMem (CsmeOemTableVerInfo, &MbpHob->MeBiosPayload.OemNphyData.Data, sizeof (OEM_TABLE_VER_INFO));
        return TRUE;
      }
      break;
    default:
      break;
  }

  return FALSE;
}

/**
  This function checks if OEM table sync is required.

  @param[in]  HsioConfig              Pointer to HSIO Config
  @param[in]  OemTablePointer         Address of OEM table
  @param[in]  OemTableSize            Size of OEM table
  @param[in]  OemTableType            OEM table type

  @retval TRUE                        OEM table sync is required
  @retval FALSE                       OEM table sync not required
**/
BOOLEAN
IsOemTableSyncRequired (
  IN  HSIO_CONFIG       *HsioConfig,
  IN  VOID              *OemTablePointer,
  IN  UINT32            OemTableSize,
  IN  OEM_TABLE_TYPE    OemTableType
  )
{
  OEM_TABLE_VER_INFO  CsmeOemTableVerInfo;
  OEM_TABLE_VER_INFO  *BiosOemTableVerInfo;

  if (!IsOemTableVersionPresentInMbp (OemTableType, &CsmeOemTableVerInfo)) {
    #if !defined (MDEPKG_NDEBUG)
      DEBUG ((DEBUG_INFO, "No %a binary present in CSME\n", OEM_TABLE_NAME[OemTableType]));
    #endif
    return FALSE;
  }

  if ((OemTablePointer == 0) || (OemTableSize == 0)) {
  #if !defined (MDEPKG_NDEBUG)
    DEBUG ((DEBUG_INFO, "No OEM %a binary present in BIOS or binary size is 0\n", OEM_TABLE_NAME[OemTableType]));
  #endif
  } else if (OemTableSize > OEM_TABLE_MAX_SIZE[OemTableType]) {
#if !defined (MDEPKG_NDEBUG)
    DEBUG ((DEBUG_ERROR, "OEM %a binary length exceeds maximum size!\n", OEM_TABLE_NAME[OemTableType]));
#endif
  } else {
    BiosOemTableVerInfo = (OEM_TABLE_VER_INFO *) ((UINT8 *) OemTablePointer + HSIO_OEM_TABLE_VERSION_OFFSET);

    PrintOemTableVersionInfo (&CsmeOemTableVerInfo, BiosOemTableVerInfo, OemTableType);

    ///
    /// If CSME exposes OEM table version in MBP, but it's 0, push the table
    ///
    if (CsmeOemTableVerInfo.Major  == 0 &&
        CsmeOemTableVerInfo.Minor  == 0 &&
        CsmeOemTableVerInfo.Hotfix == 0 &&
        CsmeOemTableVerInfo.Build  == 0
        ) {
      return TRUE;
    }

    ///
    /// If any of the bytes [0:9] are different, don't push BIOS OEM table
    /// If bytes [0:9] are the same, but bytes [10:11] are different, push BIOS OEM table
    ///
    if (CsmeOemTableVerInfo.Major  == BiosOemTableVerInfo->Major  &&
        CsmeOemTableVerInfo.Minor  == BiosOemTableVerInfo->Minor  &&
        CsmeOemTableVerInfo.Hotfix == BiosOemTableVerInfo->Hotfix &&
        CsmeOemTableVerInfo.Build  != BiosOemTableVerInfo->Build
        ) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  This function performs OEM table sync if required.

  @param[in]  HsioConfig              Pointer to HSIO Config
  @param[in]  OemTableType            OEM table type

  @retval EFI_SUCCESS                 Synced successfully or sync not required
  @retval EFI_OUT_OF_RESOURCES        Couldn't allocate required memory
  @retval EFI_UNSUPPORTED             Current ME mode doesn't support ChipsetInit sync
  @retval EFI_DEVICE_ERROR            An error occurred during HECI communication
  @retval EFI_TIMEOUT                 CSME didn't answer before timeout
**/
EFI_STATUS
PerformOemTableSync (
  IN  HSIO_CONFIG       *HsioConfig,
  IN  OEM_TABLE_TYPE    OemTableType
  )
{
  EFI_STATUS        Status;
  PCH_RESET_DATA    ResetData;
  VOID              *OemTablePointer;
  UINT32            OemTableSize;
  UINT8             *OemTableData;

  DEBUG ((DEBUG_INFO, "%a() - Start\n", __FUNCTION__));

  OemTablePointer = 0;
  OemTableSize    = 0;

  switch (OemTableType) {
    case OemTableChipsetInit:
      OemTablePointer = HsioConfig->ChipsetInitBinPtr;
      OemTableSize    = HsioConfig->ChipsetInitBinLen;
      break;
    case OemTableSphy:
      OemTablePointer = HsioConfig->SynpsPhyBinPtr;
      OemTableSize    = HsioConfig->SynpsPhyBinLen;
      break;
    case OemTableNphy:
      OemTablePointer = HsioConfig->NphyBinPtr;
      OemTableSize    = HsioConfig->NphyBinLen;
      break;
    default:
      break;
  }

  if (IsOemTableSyncRequired (HsioConfig, OemTablePointer, OemTableSize, OemTableType)) {
#if !defined (MDEPKG_NDEBUG)
    DEBUG ((DEBUG_INFO, "BIOS <-> CSME OEM %a table sync required\n", OEM_TABLE_NAME[OemTableType]));
#endif

    ///
    /// Allocate memory for maximum OEM table size
    ///
    OemTableData = AllocateZeroPool (OEM_TABLE_MAX_SIZE[OemTableType]);

    if (OemTableData == NULL) {
      DEBUG ((DEBUG_ERROR, "%a(): Could not allocate required memory\n", __FUNCTION__));
      return EFI_OUT_OF_RESOURCES;
    }

    CopyMem (OemTableData, OemTablePointer, OemTableSize);

    switch (OemTableType) {
      case OemTableChipsetInit:
        Status = PeiHeciWriteChipsetInitMsg ((UINT8*) OemTableData, OemTableSize);
        FreePool (OemTableData);
        if (EFI_ERROR (Status)) {
          return Status;
        }
        break;
      case OemTableSphy:
      case OemTableNphy:
        Status = PeiHeciSetFileExMsg (OEM_TABLE_FILE_ID[OemTableType], 0, OemTableSize, 0, OemTableData);
        FreePool (OemTableData);
        if (EFI_ERROR (Status)) {
          return Status;
        }

        Status = PeiHeciCommitFilesMsg ();
        if (EFI_ERROR (Status)) {
          return Status;
        }
        break;
      default:
        break;
    }

#if !defined (MDEPKG_NDEBUG)
    DEBUG ((DEBUG_INFO, "OEM %a sync done - schedule global reset\n", OEM_TABLE_NAME[OemTableType]));
#endif
    CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
    StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
    SiScheduleResetSetType (EfiResetPlatformSpecific, &ResetData);
  } else {
#if !defined (MDEPKG_NDEBUG)
    DEBUG ((DEBUG_INFO, "BIOS <-> CSME OEM %a table sync not required\n", OEM_TABLE_NAME[OemTableType]));
#endif
  }

  DEBUG ((DEBUG_INFO, "%a() - End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  This function is used to ensure CSME has the latest OEM tables.

  A BIOS<->CSME ChipsetInit sync is performed if there is a mismatch.

  @param[in] PmcSsramPciCfgBase   PMC SSRAM controller PCI address

  @retval EFI_SUCCESS             BIOS and CSME ChipsetInit settings are in sync
  @retval EFI_UNSUPPORTED         OEM table sync not supported
  @retval EFI_OUT_OF_RESOURCES    BIOS is not able to allocate memory
**/
EFI_STATUS
ChipsetInitSync (
  VOID
  )
{
  EFI_STATUS                        Status;
  SI_POLICY_PPI                     *SiPolicyPpi;
  HSIO_CONFIG                       *HsioConfig;

  Status = EFI_SUCCESS;
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  if (IsChipsetInitSyncSupported ()) {
    ///
    /// Get Policy settings through the SiPolicy PPI
    ///
    HsioConfig = NULL;
    Status = PeiServicesLocatePpi (
              &gSiPolicyPpiGuid,
              0,
              NULL,
              (VOID **) &SiPolicyPpi
              );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "SiPolicyPpi is not located! Error: %r\n", Status));
      return EFI_UNSUPPORTED;
    }

    Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gHsioConfigGuid, (VOID *) &HsioConfig);
    ASSERT_EFI_ERROR (Status);

    Status = PerformOemTableSync (HsioConfig, OemTableChipsetInit);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "OEM ChipsetInit sync failed, status = %r\n", Status));
    }

    Status = PerformOemTableSync (HsioConfig, OemTableSphy);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "OEM Sphy sync failed, status = %r\n", Status));
    }

    Status = PerformOemTableSync (HsioConfig, OemTableNphy);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "OEM Nphy sync failed, status = %r\n", Status));
    }
  }

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));

  return Status;
}