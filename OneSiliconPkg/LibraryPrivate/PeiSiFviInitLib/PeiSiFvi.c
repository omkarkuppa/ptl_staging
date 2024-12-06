/** @file
  CPU Firmware Version Info implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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
#include <SiFvi.h>
#include <Library/MicrocodeLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HostBridgeInfoLib.h>
#include <Library/MeInfoLib.h>
#include <Defines/HostBridgeDefines.h>
#include <HostBridgeDataHob.h>
#include <MeBiosPayloadHob.h>
#include <MemInfoHob.h>
#include <TcssDataHob.h>
#include <TxtInfoHob.h>
#include <IndustryStandard/FirmwareVersionInfo.h>
#include <IndustryStandard/Pci30.h>
#include <Register/HeciRegs.h>
#include <Library/PrintLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED INTEL_FIRMWARE_VERSION_INFO    mRcFviData[] = {
                                                               DEFAULT_FVI_DATA (),
                                                             };

GLOBAL_REMOVE_IF_UNREFERENCED FVI_STRINGS                    mRcFviStrings[] = {
                                                               { RC_FVI_STRING, RC_FVI_DEFAULT_STRING },
                                                             };

GLOBAL_REMOVE_IF_UNREFERENCED INTEL_FIRMWARE_VERSION_INFO    mCpuFviData[] = {
                                                               FVI_VERSION (DEFAULT_FVI_VERSION),
                                                               FVI_VERSION (DEFAULT_FVI_VERSION)
                                                             };
GLOBAL_REMOVE_IF_UNREFERENCED FVI_STRINGS                    mCpuFviStrings[] = {
                                                               { UCODE_FVI_STRING, NULL },
                                                               { TXT_FVI_STRING,   NULL }
                                                             };

GLOBAL_REMOVE_IF_UNREFERENCED INTEL_FIRMWARE_VERSION_INFO    mMeFviData[] = {
                                                               FVI_VERSION (DEFAULT_FVI_VERSION),
                                                               FVI_VERSION (DEFAULT_FVI_VERSION),
                                                             };
GLOBAL_REMOVE_IF_UNREFERENCED FVI_STRINGS                    mMeFviStrings[] = {
                                                               {ME_FW_FVI_STRING, ME_FW_CONSUMER_SKU_STRING},
                                                               {SSE_FVI_STRING,   SSE_FVI_DEFAULT_STRING},
                                                             };

GLOBAL_REMOVE_IF_UNREFERENCED INTEL_FIRMWARE_VERSION_INFO    mPchFviData[] = {
    FVI_VERSION(DEFAULT_FVI_VERSION),   // PCH CRID status
    FVI_VERSION(DEFAULT_FVI_VERSION),   // PCH CRID origin
    FVI_VERSION(DEFAULT_FVI_VERSION),   // PCH CRID new
    FVI_VERSION(DEFAULT_FVI_VERSION),   // RAID RC version
    FVI_VERSION(FVI_PCH_ICL_LP_AX_HSIO_VERSION)
};
GLOBAL_REMOVE_IF_UNREFERENCED FVI_STRINGS                    mPchFviStrings[] = {
    { PCH_CRID_STATUS_STRING, PCH_CRID_DISABLED },
    { PCH_CRID_ORIGINAL_VALUE, NULL },
    { PCH_CRID_NEW_VALUE, NULL },
    { RAID_FVI_STRING, NULL },
    { PCH_HSIO_STRING, NULL }
};

GLOBAL_REMOVE_IF_UNREFERENCED INTEL_FIRMWARE_VERSION_INFO    mSaFviData[] = {
                                                               FVI_VERSION (DEFAULT_FVI_VERSION),   // Reference Code - MRC
                                                               FVI_VERSION (DEFAULT_FVI_VERSION),   // SA CRID Status
                                                               FVI_VERSION (DEFAULT_FVI_VERSION),   // SA CRID origin
                                                               FVI_VERSION (DEFAULT_FVI_VERSION),   // SA CRID new
                                                               FVI_VERSION (DEFAULT_FVI_VERSION),   // VBIOS RC version
                                                               FVI_VERSION (DEFAULT_FVI_VERSION),   // IOM FW version
                                                               FVI_VERSION (DEFAULT_FVI_VERSION),   // PHY version
                                                               FVI_VERSION (DEFAULT_FVI_VERSION),   // TBT FW version
                                                               FVI_VERSION (DEFAULT_FVI_VERSION)    // SAM FW version
                                                             };
GLOBAL_REMOVE_IF_UNREFERENCED FVI_STRINGS                    mSaFviStrings[] = {
                                                               { MEM_FVI_STRING,         NULL },
                                                               { SA_CRID_STATUS,         SA_CRID_DISABLED },
                                                               { SA_CRID_ORIGINAL_VALUE, NULL },
                                                               { SA_CRID_NEW_VALUE,      NULL },
                                                               { VBIOS_FVI_STRING,       NULL },
                                                               { IOM_FVI_STRING,         NULL },
                                                               { PHY_FVI_STRING,         NULL },
                                                               { TBT_FVI_STRING,         NULL },
                                                               { SAM_FVI_STRING,         NULL }
                                                             };


/**
  Build data Hob of the SMBIOS Firmware Version Info (FVI) OEM type based on the SMBIOS_TABLE_TYPE_OEM_INTEL_FVI

  @retval EFI_OUT_OF_RESOURCES - Failed to allocate memory for SMBIOS FVI OEM type.
  @retval EFI_SUCCESS          - Successfully built Hob
**/
EFI_STATUS
BuildFviHob (
  IN INTEL_FIRMWARE_VERSION_INFO    *Data,
  IN FVI_STRINGS                    *Strings,
  IN UINTN                          DataSize
  )
{
  EFI_STATUS                        Status;
  UINT8                             Index;
  UINTN                             SizeOfStrings;
  CHAR8                             *StringPtr;
  CHAR8                             **StringArray;
  UINT8                             StringCount;
  UINT8                             StringNumber;
  SMBIOS_TABLE_TYPE_OEM_INTEL_FVI   *Record;
  UINT8                             Count;
  UINT8                             *DataPtr;
  VOID                              *Hob;
  UINTN                             HobLength;

  SizeOfStrings = 0;
  HobLength = 0;
  StringCount = 0;                   ///< Number of non-null strings total.
  StringNumber = 0;                  ///< String number of the current string.
  Status = EFI_SUCCESS;

  DataPtr = (UINT8 *) Data;          ///< Points to the fixed data location for the current string.
  StringArray = (CHAR8 **) Strings;
  Count = (UINT8)(DataSize / sizeof (INTEL_FIRMWARE_VERSION_INFO));

  for (Index = 0; Index < (Count * FVI_NUMBER_OF_STRINGS); Index++) {
    if (StringArray[Index] != NULL) {
      SizeOfStrings += AsciiStrSize (StringArray[Index]);
      StringCount++;
      StringNumber = StringCount;
    } else {
      StringNumber = NO_STRING_AVAILABLE;
    }
    *DataPtr = StringNumber;
    DataPtr++;

    ///
    /// If this was the last string of this data block, adjust the DataPtr
    /// to the start of the string section of the next data block.
    ///
    if (((Index + 1) % FVI_NUMBER_OF_STRINGS) == 0) {
      DataPtr += (sizeof (INTEL_FIRMWARE_VERSION_INFO) - FVI_NUMBER_OF_STRINGS);
    }
  }

  ///
  /// Add the size of the terminating double null
  /// If there were any strings added, just add the second null
  ///
  if (SizeOfStrings == 0) {
    SizeOfStrings += 2;
  } else {
    SizeOfStrings += 1;
  }

  ///
  /// Initialize the full record
  /// The complete record layout is (SMBIOS_STRUCTURE + UINT8 + n * INTEL_FIRMWARE_VERSION_INFO + strings)
  ///
  HobLength = sizeof (SMBIOS_STRUCTURE) + sizeof (UINT8) + (Count * sizeof (INTEL_FIRMWARE_VERSION_INFO)) + SizeOfStrings;
  Record = (SMBIOS_TABLE_TYPE_OEM_INTEL_FVI *) AllocateZeroPool (HobLength);
  if (Record == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Record->Header.Type = INTEL_FVI_SMBIOS_TYPE;
  Record->Header.Length = sizeof (SMBIOS_STRUCTURE) + sizeof (UINT8) + (Count * sizeof (INTEL_FIRMWARE_VERSION_INFO));
  Record->Header.Handle = 0;
  Record->Count = Count;
  CopyMem (&(Record->Fvi[0]), Data, (Count * sizeof (INTEL_FIRMWARE_VERSION_INFO)));

  ///
  /// Copy the strings to the end of the record
  ///
  StringPtr = ((CHAR8 *) &(Record->Fvi[0])) + (Count * sizeof (INTEL_FIRMWARE_VERSION_INFO));
  for (Index = 0; Index < (Count * FVI_NUMBER_OF_STRINGS); Index++) {
    if (StringArray[Index] != NULL) {
      AsciiStrCpyS (StringPtr, SizeOfStrings, StringArray[Index]);
      StringPtr += AsciiStrSize (StringArray[Index]);
      SizeOfStrings -= AsciiStrSize (StringArray[Index]);
    }
  }

  Hob = BuildGuidDataHob (
          &gIntelSmbiosDataHobGuid,
          (VOID *) Record,
          HobLength
          );
  ASSERT (Hob != NULL);

  FreePool (Record);
  return Status;
}

/**
  This function builds the RC FVI Hob

  @retval EFI_SUCCESS          - Successfully built the Hob
**/
EFI_STATUS
BuildRcFviHob (
  VOID
  )
{
  CHAR8                           RcVersion[sizeof (RC_FVI_DEFAULT_STRING)];

  AsciiSPrint(RcVersion, sizeof(RcVersion), "%04X.%04X.%04X.%04X",
                                      ((UINT8)  PcdGet8 (PcdFamilyBranch)),\
                                      ((UINT8) (((PcdGet8 (PcdProgBranch) & 0x0F) << 4) | (PcdGet8 (PcdBranchNestedLevel) & 0x0F))),\
                                      ((UINT16) ((PcdGet8 (PcdSiliconSegment) & 0x0F) << 12) | (PcdGet8 (PcdFspYear) & 0xFF)),\
                                      ((UINT16) (PcdGet16 (PcdFspBuildVer) & 0xFFFF))\
                                      );

  mRcFviStrings->VersionString = (CHAR8 *) &RcVersion;
  mRcFviData->Version.MajorVersion = 0xFF;
  mRcFviData->Version.MinorVersion = 0xFF;
  mRcFviData->Version.Revision     = 0xFF;
  mRcFviData->Version.BuildNumber  = 0xFFFF;

  return BuildFviHob (mRcFviData, mRcFviStrings, sizeof (mRcFviData));
}

/**
  This function builds the CPU FVI Hob

  @retval EFI_SUCCESS          - Successfully built the Hob
**/
EFI_STATUS
BuildCpuFviHob (
  VOID
  )
{
  UINT32        uCodeRevision;
  TXT_INFO_HOB  *TxtInfoHob;

  uCodeRevision = GetProcessorMicrocodeSignature ();
  mCpuFviData[UCODE_VER].Version.MajorVersion  = (UINT8) ((uCodeRevision & 0xFF000000) >> 24);
  mCpuFviData[UCODE_VER].Version.MinorVersion  = (UINT8) ((uCodeRevision & 0x00FF0000) >> 16);
  mCpuFviData[UCODE_VER].Version.Revision      = (UINT8) ((uCodeRevision & 0x0000FF00) >> 8);
  mCpuFviData[UCODE_VER].Version.BuildNumber   = (UINT16) (uCodeRevision & 0x000000FF);

  //
  // Get TxtInfoHob
  //
  TxtInfoHob = (TXT_INFO_HOB *)GetFirstGuidHob (&gTxtInfoHobGuid);
  if (TxtInfoHob && !(0 == TxtInfoHob->Data.AcmMajorVersion && 0 == TxtInfoHob->Data.AcmMinorVersion)) {
    mCpuFviData[TXT_VER].Version.MajorVersion  = (UINT8) (TxtInfoHob->Data.AcmMajorVersion);
    mCpuFviData[TXT_VER].Version.MinorVersion  = (UINT8) (TxtInfoHob->Data.AcmMinorVersion);
    mCpuFviData[TXT_VER].Version.Revision      = (UINT8) (TxtInfoHob->Data.AcmRevision);
    mCpuFviData[TXT_VER].Version.BuildNumber   = 0;
  }

  return BuildFviHob (mCpuFviData, mCpuFviStrings, sizeof (mCpuFviData));
}

/**
  This function builds the ME FVI Hob

  @retval EFI_SUCCESS          - Successfully built the Hob
**/
EFI_STATUS
BuildMeFviHob (
  VOID
  )
{
  CONST UINT8                     StrCorporateSkuFw[sizeof (ME_FW_CORPORATE_SKU_STRING)] = ME_FW_CORPORATE_SKU_STRING;
  CONST UINT8                     StrLiteSkuFw[sizeof (ME_FW_LITE_SKU_STRING)] = ME_FW_LITE_SKU_STRING;
  ME_BIOS_PAYLOAD_HOB             *MbpHob;
  HECI_FW_STS3_REGISTER           MeFirmwareStatus3;

  MbpHob               = NULL;
  MeFirmwareStatus3.ul = 0;

  //
  // Get the MBP Data.
  //
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob != NULL) {
    mMeFviData[EnumMeFw].Version.MajorVersion = (UINT8)  MbpHob->MeBiosPayload.FwVersionName.MajorVersion;
    mMeFviData[EnumMeFw].Version.MinorVersion = (UINT8)  MbpHob->MeBiosPayload.FwVersionName.MinorVersion;
    mMeFviData[EnumMeFw].Version.Revision     = (UINT8)  MbpHob->MeBiosPayload.FwVersionName.HotfixVersion;
    mMeFviData[EnumMeFw].Version.BuildNumber  = (UINT16) MbpHob->MeBiosPayload.FwVersionName.BuildVersion;
    if (MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType == IntelMeCorporateFw) {
      mMeFviStrings[EnumMeFw].VersionString = (CHAR8 *) &StrCorporateSkuFw;
    } else if (MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType == IntelMeLiteFw) {
      mMeFviStrings[EnumMeFw].VersionString = (CHAR8 *) &StrLiteSkuFw;
    }
  } else {
    DEBUG ((DEBUG_INFO, "%a: No MBP Data Hob available\n", __FUNCTION__));
    MeFirmwareStatus3.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS_3);

    if (MeFirmwareStatus3.ul != 0xFFFFFFFF) {
      if (MeFirmwareStatus3.r.FwSku == ME_FWSTS3_FW_SKU_CORPORATE) {
        mMeFviStrings[EnumMeFw].VersionString = (CHAR8 *) &StrCorporateSkuFw;
      } else if (MeFirmwareStatus3.r.FwSku == ME_FWSTS3_FW_SKU_LITE) {
        mMeFviStrings[EnumMeFw].VersionString = (CHAR8 *) &StrLiteSkuFw;
      }
    }
  }

  return BuildFviHob (mMeFviData, mMeFviStrings, sizeof (mMeFviData));
}

/**
  This function builds the PCH FVI Hob

  @retval EFI_SUCCESS          - Successfully built the Hob
**/
EFI_STATUS
BuildPchFviHob (
  VOID
  )
{
  return BuildFviHob (mPchFviData, mPchFviStrings, sizeof (mPchFviData));
}

/**
  This function builds the SA FVI Hob

  @retval EFI_SUCCESS          - Successfully built the Hob
**/
EFI_STATUS
BuildSaFviHob (
  VOID
  )
{
  CONST UINT8                     StrEnabled[sizeof (SA_CRID_ENABLED)] = SA_CRID_ENABLED;
  CONST UINT8                     StrDisabled[sizeof (SA_CRID_DISABLED)] = SA_CRID_DISABLED;
  STATIC CONST SiMrcVersion       MemRcVersionConst = {0, 0, 0, 0};
  CONST SiMrcVersion              *MemRcVersion;
  MEMORY_INFO_DATA_HOB            *MemInfo;
  UINT64                          McBaseAddress;
  UINT8                           Data8;
  TCSS_DATA_HOB                   *TcssHob;
  EFI_HOB_GUID_TYPE               *GuidHob;
  UINT32                          SamFwVersion;

  McBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);
  ///
  /// Save CRID_ORIGINAL value before SaSecurityInit() executes CRID function.
  ///
  mSaFviData[CRID_ORIGINAL].Version.MajorVersion = 0;
  mSaFviData[CRID_ORIGINAL].Version.MinorVersion = 0;
  mSaFviData[CRID_ORIGINAL].Version.Revision     = 0;
  mSaFviData[CRID_ORIGINAL].Version.BuildNumber  = (UINT16) PciSegmentRead8 (McBaseAddress + PCI_REVISION_ID_OFFSET);
  ///
  /// Get TcssHob HOB
  ///
  TcssHob = NULL;
  TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);

  ///
  /// CRID configuration
  ///
  if (IsCridEnabled ()) {
    mSaFviStrings[CRID_STATUS].VersionString = (CHAR8 *) &StrEnabled;
    Data8 = CRID_DATA;
  } else {
    mSaFviStrings[CRID_STATUS].VersionString = (CHAR8 *) &StrDisabled;
    Data8 = CRID_LOCK;
  }
  PciSegmentWrite8 ((UINT64) (McBaseAddress + PCI_REVISION_ID_OFFSET), Data8);

  //
  // Search for the Memory Configuration GUID HOB. If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  GuidHob = NULL;
  MemInfo = NULL;
  GuidHob = GetFirstGuidHob (&gSiMemoryInfoDataGuid);
  if (GuidHob != NULL) {
    MemInfo = (MEMORY_INFO_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
  }
  if (MemInfo != NULL) {
    MemRcVersion = &MemInfo->Version;
  } else {
    MemRcVersion = &MemRcVersionConst;
  }

  mSaFviData[MEM_RC_VER].Version.MajorVersion = (UINT8)  MemRcVersion->Major;
  mSaFviData[MEM_RC_VER].Version.MinorVersion = (UINT8)  MemRcVersion->Minor;
  mSaFviData[MEM_RC_VER].Version.Revision     = (UINT8)  MemRcVersion->Rev;
  mSaFviData[MEM_RC_VER].Version.BuildNumber  = (UINT16) MemRcVersion->Build;
  mSaFviData[CRID_NEW].Version.MajorVersion   = 0;
  mSaFviData[CRID_NEW].Version.MinorVersion   = 0;
  mSaFviData[CRID_NEW].Version.Revision       = 0;
  mSaFviData[CRID_NEW].Version.BuildNumber    = (UINT16) PciSegmentRead8 (McBaseAddress + PCI_REVISION_ID_OFFSET);

  if (TcssHob != NULL) {
    DEBUG ((DEBUG_INFO, "IomFwVersion = %08X\n", TcssHob->TcssData.IomFwVersion));
    mSaFviData[IOM_FW_VER].Version.MajorVersion = (UINT8) (TcssHob->TcssData.IomFwVersion >> 24);
    mSaFviData[IOM_FW_VER].Version.MinorVersion = (UINT8) (TcssHob->TcssData.IomFwVersion >> 12);
    mSaFviData[IOM_FW_VER].Version.Revision = (UINT8) ((TcssHob->TcssData.IomFwVersion >> 8) & 0xF);
    mSaFviData[IOM_FW_VER].Version.BuildNumber = (UINT8) (TcssHob->TcssData.IomFwVersion);
    DEBUG ((DEBUG_INFO, "MgFwVersion = %04X\n", TcssHob->TcssData.MgImrStatus.Bits.MgFwVersion));
    mSaFviData[PHY_VER].Version.MajorVersion = 0;
    mSaFviData[PHY_VER].Version.MinorVersion = 0;
    mSaFviData[PHY_VER].Version.Revision = 0;
    mSaFviData[PHY_VER].Version.BuildNumber = (UINT16) (TcssHob->TcssData.MgImrStatus.Bits.MgFwVersion);
    DEBUG ((DEBUG_INFO, "TbtFwVersion = %04X\n", TcssHob->TcssData.TbtImrStatus.Bits.TbtFwVersion));
    mSaFviData[TBT_FW_VER].Version.MajorVersion = (UINT8) (DivU64x32 (TcssHob->TcssData.TbtImrStatus.Bits.TbtFwVersion, 100));
    mSaFviData[TBT_FW_VER].Version.MinorVersion = (UINT8) (ModU64x32 (TcssHob->TcssData.TbtImrStatus.Bits.TbtFwVersion, 100));
    mSaFviData[TBT_FW_VER].Version.Revision = 0;
    mSaFviData[TBT_FW_VER].Version.BuildNumber = 0;
  }

  if (IsSamEnabled ()) {
    SamFwVersion = GetSamFwInfo ();
    DEBUG ((DEBUG_INFO, "SamFwVersion = %08X\n", SamFwVersion));
    mSaFviData[SAM_FW_VER].Version.MajorVersion = (UINT8) (SamFwVersion >> 24);
    mSaFviData[SAM_FW_VER].Version.MinorVersion = (UINT8) (SamFwVersion >> 12);
    mSaFviData[SAM_FW_VER].Version.Revision = (UINT8) ((SamFwVersion >> 8) & 0xF);
    mSaFviData[SAM_FW_VER].Version.BuildNumber = (UINT8) (SamFwVersion);
  }

  return BuildFviHob (mSaFviData, mSaFviStrings, sizeof (mSaFviData));
}

/**
  Initialize all Smbios FVI OEM Type Data Hob
**/
VOID
BuildFviInfoHob (
  VOID
  )
{
  EFI_STATUS      Status;

  DEBUG ((DEBUG_INFO, "BuildFviInfoHob(): Build Smbios FVI Data Hob\n"));

  Status = BuildRcFviHob();
  ASSERT_EFI_ERROR (Status);

  Status = BuildCpuFviHob();
  ASSERT_EFI_ERROR (Status);

  Status = BuildMeFviHob();
  ASSERT_EFI_ERROR (Status);

  Status = BuildPchFviHob();
  ASSERT_EFI_ERROR (Status);

  Status = BuildSaFviHob();
  ASSERT_EFI_ERROR (Status);
}
