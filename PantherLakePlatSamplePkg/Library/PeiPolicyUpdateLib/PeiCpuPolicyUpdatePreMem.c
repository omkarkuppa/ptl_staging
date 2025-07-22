/** @file
  This file is SampleCode of the library for Intel CPU PEI Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#include "PeiCpuPolicyUpdate.h"
#include <Guid/SysFwUpdateProgress.h>
#include <Library/ConfigBlockLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/HobLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/Ptl/PcdInfoLib/PtlPcdInfoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/PmcLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/SiPolicyLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/SpiLib.h>
#include <Library/TmeInfoLib.h>
#include <TdxDataHob.h>
#include <Ppi/Spi.h>
#include <Register/GenerationMsr.h>
#include <Register/PchRegs.h>
#include <PlatformBoardConfig.h>
#include <Register/Cpuid.h>
#include <Library/EcMiscLib.h>
#include <TxtConfig.h>
#include <PolicyUpdateMacro.h>
#include <Library/PeiVrDomainLib.h>
#include <Library/Tpm2CommandLib.h>
#include <Register/PmcRegs.h>
#if FixedPcdGet8(PcdFspModeSelection) == 1
#include <FspmUpd.h>
#endif
#include <Library/PeiHostBridgeIpStatusLib.h>

#define GET_OCCUPIED_SIZE(ActualSize, Alignment) \
  ((ActualSize) + (((Alignment) - ((ActualSize) & ((Alignment) - 1))) & ((Alignment) - 1)))

#define EPOCFCLKFREQ_AUTO_VALUE  3

/**
  Get the BIOS Guard Module pointer.

  @param[in, out] ModulePtr  - Input is a NULL pointer,
                               and output points BIOS Guard module address if found.
  @param[out]     ModuleSize - UINT32 Input Output the BIOS Guard module size

  @retval EFI_SUCCESS        - BIOS Guard Module found.
  @retval EFI_NOT_FOUND      - BIOS Guard Module size and/or Address equal to 0.
  @retval Others             - BIOS Guard Module not found.
**/
EFI_STATUS
UpdateBiosGuardModulePtr (
  IN OUT EFI_PHYSICAL_ADDRESS   *ModulePtr,
  OUT    UINT32                 *ModuleSize
  )
{
  EFI_STATUS                    Status;
  EFI_FIRMWARE_VOLUME_HEADER    *FvHeader;
  EFI_PEI_FILE_HANDLE           *FfsHeader;
  EFI_PEI_FILE_HANDLE           *FfsRawSectionHeader;
  EFI_PEI_FILE_HANDLE           *FfsRawSectionData;
  UINTN                         BiosGuardModuleAddr;
  UINT32                        BiosGuardModuleSize;
  EFI_GUID                      *BiosGuardModuleGuidPtr;

  DEBUG ((DEBUG_INFO, "Update BIOS Guard Module Pointer and Size\n"));

  Status                 = EFI_SUCCESS;
  BiosGuardModuleAddr    = 0;
  BiosGuardModuleSize    = 0;
  BiosGuardModuleGuidPtr = &gBiosGuardModuleBinGuid;
  ///
  /// Locate Firmware Volume header
  ///
  FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) FixedPcdGet32 (PcdFlashFvFirmwareBinariesBase);
  Status = PeiServicesFfsFindFileByName (BiosGuardModuleGuidPtr, FvHeader, (VOID **)&FfsHeader);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error returned by PeiServicesFfsFindFileByName while retrieving BIOS Guard FFS File Handle\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = PeiServicesFfsFindSectionData (EFI_SECTION_RAW, FfsHeader, (VOID **)&FfsRawSectionData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error returned by PeiServicesFfsFindSectionData while retrieving BIOS Guard Module Address\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  BiosGuardModuleAddr = (UINTN)((UINT8 *) FfsRawSectionData);
  FfsRawSectionHeader = (VOID *)((UINT8 *) FfsRawSectionData - sizeof (EFI_COMMON_SECTION_HEADER));
  BiosGuardModuleSize = SECTION_SIZE(FfsRawSectionHeader) - sizeof (EFI_COMMON_SECTION_HEADER);


  DEBUG ((DEBUG_ERROR, "BIOS Guard Module Location: 0x%X\n", BiosGuardModuleAddr));
  DEBUG(( DEBUG_ERROR, "BIOS Guard Module size: 0x%X\n", BiosGuardModuleSize));

  if ((BiosGuardModuleAddr == 0) || (BiosGuardModuleSize == 0)) {
    DEBUG ((DEBUG_ERROR, "BiosGuardModuleSize = 0x%X and/or BiosGuardModule = 0x%X equal to zero\n", BiosGuardModuleAddr, BiosGuardModuleSize));
    ASSERT_EFI_ERROR (Status);
    return EFI_NOT_FOUND;
  }

  *ModulePtr  = BiosGuardModuleAddr;
  *ModuleSize = BiosGuardModuleSize;
  return Status;
}

/**
  Check if BIOS Guard Module is Debug Signed

  @param[in] ModulePtr - Input is the BIOS Guard module address.

  @retval TRUE  - BIOS Guard Module is Debug Signed
  @retval FALSE - BIOS Guard Module is Production Signed
**/
BOOLEAN
IsBiosGuardModuleDebugSigned2 (
  IN EFI_PHYSICAL_ADDRESS   ModulePtr
  )
{
  UINT16                  RevisionIdMajor;

  ///
  /// Offset from beginning 142 Bytes (Size of BIOS HEADER (128 Bytes) + Offset of Internal Header Major Revision ID (14 Bytes))
  ///
  RevisionIdMajor = *(UINT16*)(UINTN)(ModulePtr + BIOSGUARD_INTERNAL_HEADER_MODULE_ID_OFFSET);
  DEBUG ((DEBUG_INFO, "BIOS Guard Revision ID (Major) = %x\n", RevisionIdMajor));

  ///
  /// Check if Module is Debug Signed. Bit 15 being set means it is Debug Signed.
  ///
  return ((RevisionIdMajor & BIT15) != 0) ? TRUE : FALSE;
}

/**
  Create and initialize BIOS Guard HOB

  @retval  EFI_SUCCESS  HOB Creating successful.
  @retval  Others       HOB Creating failed.
**/
EFI_STATUS
BiosGuardHobInit (
  VOID
  )
{
  EFI_STATUS                       Status;
  BIOSGUARD_HOB                    *BiosGuardHobPtr;
  EC_BGPDT                         EcBgpdt;
  STATIC UINT8                     BgpdtPkeySlot0[] = { 0xfe, 0xdc, 0x94, 0x2b, 0x0f, 0xf7, 0x50, 0x84, 0x52, 0xe8, 0x28, 0x7a, 0x1d, 0x01, 0xd5, 0x1e, 0x27, 0xd2, 0xec, 0xb1, 0xdc, 0x13, 0x3b, 0xb5, 0x4f, 0x59, 0xd7, 0xa4, 0x96, 0xc0, 0xc3, 0xd7, 0xa6, 0xa0, 0x83, 0xe7, 0x2b, 0x1b, 0xe9, 0x98, 0x3b, 0xa3, 0x33, 0xd4, 0x19, 0xbf, 0xfa, 0x03 };
  STATIC UINT8                     BgpdtPkeySlot1[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  STATIC UINT8                     BgpdtPkeySlot2[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
#if FixedPcdGetBool (PcdEcEnable) == 1
  UINT8                            EcStatus;
#endif
  UINT8                            PlatIdStr[] = "PANTHERLAKE";
  PCH_SPI_PPI                      *SpiPpi;
  UINT32                           BaseAddr;
  UINT32                           RegionSize;
  UINT32                           Index = 0;

  RegionSize = 0;
  BaseAddr   = 0;

  Status = PeiServicesLocatePpi (
             &gPchSpiPpiGuid,
             0,
             NULL,
             (VOID **)&SpiPpi
             );

  DEBUG ((DEBUG_INFO, "PlatformInit: LocatePpi gPchSpiPpiGuid = %r\n", Status));
  if (!EFI_ERROR (Status)) {
    //
    // Find the base address for flash
    //
    Status = SpiPpi->GetRegionAddress (SpiPpi, FlashRegionBios, &BaseAddr, &RegionSize);
    DEBUG ((DEBUG_INFO, "BIOS Region: %r - Start Address: 0x%x - Size: 0x%x\n", Status, BaseAddr, RegionSize));
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Unable to find base address for flash\n"));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }

  Status = PeiServicesCreateHob (EFI_HOB_TYPE_GUID_EXTENSION, sizeof (BIOSGUARD_HOB), (VOID **)&BiosGuardHobPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to create BIOS Guard Hob Pointer\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  ZeroMem (&(BiosGuardHobPtr->Bgpdt), (sizeof (BIOSGUARD_HOB) - sizeof (EFI_HOB_GUID_TYPE)));
  BiosGuardHobPtr->EfiHobGuidType.Name = gBiosGuardHobGuid;

  BiosGuardHobPtr->Bgpdt.BgpdtMajVer = BGPDT_MAJOR_VERSION;
  BiosGuardHobPtr->Bgpdt.BgpdtMinVer = BGPDT_MINOR_VERSION;
  BiosGuardHobPtr->Bgpdt.BiosSvn = 0x12700000;

#if FixedPcdGetBool(PcdExtendedBiosRegionSupport) == 0
  BiosGuardHobPtr->Bgpdt.LastSfam = 0;
  BiosGuardHobPtr->Bgpdt.SfamData[0].FirstByte = BaseAddr + FixedPcdGet32 (PcdFlashNonFitPayloadSize) + FixedPcdGet32 (PcdFlashNvStorageSize);
  BiosGuardHobPtr->Bgpdt.SfamData[0].LastByte  = (BaseAddr + RegionSize) - 1;
  BiosGuardHobPtr->Bgpdt.SfamData[1].FirstByte = BaseAddr;
  BiosGuardHobPtr->Bgpdt.SfamData[1].LastByte  = (BaseAddr + FixedPcdGet32 (PcdFlashNonFitPayloadSize)) - 1;
#else
  BiosGuardHobPtr->Bgpdt.LastSfam = 1;
  BiosGuardHobPtr->Bgpdt.SfamData[0].FirstByte = BaseAddr + FixedPcdGet32 (PcdFlashExtendRegionSizeInUse) + FixedPcdGet32(PcdFlashNonFitPayloadSize) + FixedPcdGet32 (PcdFlashNvStorageSize);
  BiosGuardHobPtr->Bgpdt.SfamData[0].LastByte  = (BaseAddr + RegionSize) - 1;
  BiosGuardHobPtr->Bgpdt.SfamData[1].FirstByte = BaseAddr + FixedPcdGet32 (PcdFlashExtendRegionSizeInUse);
  BiosGuardHobPtr->Bgpdt.SfamData[1].LastByte  = (BaseAddr + FixedPcdGet32 (PcdFlashExtendRegionSizeInUse) + FixedPcdGet32 (PcdFlashNonFitPayloadSize)) - 1;
  BiosGuardHobPtr->Bgpdt.SfamData[2].FirstByte = BaseAddr;
  BiosGuardHobPtr->Bgpdt.SfamData[2].LastByte  = (BaseAddr + FixedPcdGet32 (PcdFlashExtendRegionSizeInUse)) - 1;
#endif
  CopyMem (&BiosGuardHobPtr->Bgpdt.PkeySlot0[0], &BgpdtPkeySlot0[0], sizeof (BgpdtPkeySlot0));
  CopyMem (&BiosGuardHobPtr->Bgpdt.PkeySlot1[0], &BgpdtPkeySlot1[0], sizeof (BgpdtPkeySlot1));
  CopyMem (&BiosGuardHobPtr->Bgpdt.PkeySlot2[0], &BgpdtPkeySlot2[0], sizeof (BgpdtPkeySlot2));
  BiosGuardHobPtr->BiosGuardLog.LastPage       = MAX_BIOSGUARD_LOG_PAGE - 1;
  BiosGuardHobPtr->BiosGuardLog.LoggingOptions = BIOSGUARD_LOG_OPT_DEBUG | BIOSGUARD_LOG_OPT_FLASH_ERROR | BIOSGUARD_LOG_OPT_FLASH_ERASE | BIOSGUARD_LOG_OPT_FLASH_WRITE | BIOSGUARD_LOG_OPT_BRANCH_TRACE | BIOSGUARD_LOG_OPT_STEP_TRACE;
  BiosGuardHobPtr->BiosGuardLog.Version        = BIOSGUARD_LOG_VERSION;

  Status = GetBgpdtEcData (&EcBgpdt);
#if FixedPcdGetBool (PcdEcEnable) == 1
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC is Present\n"));
    BiosGuardHobPtr->Bgpdt.BiosGuardAttr      |= EnumEcPresent;
    BiosGuardHobPtr->Bgpdt.EcCmd              = EcBgpdt.EcCmd;
    BiosGuardHobPtr->Bgpdt.EcData             = EcBgpdt.EcData;
    Status = EcCommandDiscovery (&EcStatus);
    if (Status == EFI_SUCCESS) {
      if (((EcStatus & 0xF8) == 0) && (EcStatus & BIT0)) {
        if (((EcStatus & (BIT2 | BIT1)) == (BIT2 | BIT1)) || (((EcStatus & (BIT2 | BIT1)) == 0))) {
          DEBUG ((DEBUG_INFO, "EC FW supports BiosGuard\n"));
          BiosGuardHobPtr->Bgpdt.BiosGuardAttr |= EnumEcBiosGuardProtected;
          BiosGuardHobPtr->Bgpdt.EcCmdGetSvn   = EcBgpdt.EcCmdGetSvn;
          BiosGuardHobPtr->Bgpdt.EcCmdOpen     = EcBgpdt.EcCmdOpen;
          BiosGuardHobPtr->Bgpdt.EcCmdClose    = EcBgpdt.EcCmdClose;
          BiosGuardHobPtr->Bgpdt.EcCmdPortTest = EcBgpdt.EcCmdPortTest;
          BiosGuardHobPtr->BgupHeader.EcSvn    = 0x00010000;
        } else {
          DEBUG ((DEBUG_ERROR, "EC FW Error\n"));
        }
      } else {
        DEBUG ((DEBUG_WARN, "EC FW do not support BiosGuard\n"));
      }
    } else {
      DEBUG ((DEBUG_ERROR, "SendEcCommand (EcCmdDiscovery) Failed\n"));
    }
  }
#endif
  BiosGuardHobPtr->Bgpdt.BiosGuardAttr |= EnumFlashwearoutProtection;
  BiosGuardHobPtr->Bgpdt.BiosGuardAttr |= EnumFtuEnable;

  BiosGuardHobPtr->BiosGuardModulePtr  = 0;
  BiosGuardHobPtr->BiosGuardModuleSize = 0;

  Status = UpdateBiosGuardModulePtr (&BiosGuardHobPtr->BiosGuardModulePtr, &BiosGuardHobPtr->BiosGuardModuleSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UpdateBiosGuardModulePtr not found\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "------------------ BIOS Guard Module ------------------\n"));
  BiosGuardHobPtr->Bgpdt.BgModSvn = *(UINT32*)(UINTN)(BiosGuardHobPtr->BiosGuardModulePtr + BIOSGUARD_INTERNAL_HEADER_SVN_OFFSET);
  DEBUG ((DEBUG_INFO, " BIOS Guard Module:: SVN : 0x%X\n", *(UINT32*)(UINTN)(BiosGuardHobPtr->BiosGuardModulePtr + BIOSGUARD_INTERNAL_HEADER_SVN_OFFSET)));
  DEBUG ((DEBUG_INFO, " BIOS Guard Module:: Product ID : 0x%X\n", *(UINT16*)(UINTN)(BiosGuardHobPtr->BiosGuardModulePtr + BIOSGUARD_PRODUCT_ID_OFFSET)));
  DEBUG ((DEBUG_INFO, " BIOS Guard Module:: Build Number : 0x%X\n", *(UINT16*)(UINTN)(BiosGuardHobPtr->BiosGuardModulePtr + BIOSGUARD_BUILD_NUMBER_OFFSET)));

  BiosGuardHobPtr->Bgpdt.BgpdtSize = (sizeof (BGPDT) - sizeof (BiosGuardHobPtr->Bgpdt.SfamData) + ((BiosGuardHobPtr->Bgpdt.LastSfam + 1) * sizeof (SFAM_DATA)));
  CopyMem (&BiosGuardHobPtr->Bgpdt.PlatId[0], &PlatIdStr[0], sizeof (PlatIdStr));

  BiosGuardHobPtr->BgupHeader.Version        = BGUP_HDR_VERSION;
  CopyMem (&BiosGuardHobPtr->BgupHeader.PlatId[0], &PlatIdStr[0], sizeof (PlatIdStr));
  BiosGuardHobPtr->BgupHeader.PkgAttributes  = 0;
  BiosGuardHobPtr->BgupHeader.PslMajorVer    = PSL_MAJOR_VERSION;
  BiosGuardHobPtr->BgupHeader.PslMinorVer    = PSL_MINOR_VERSION;
  BiosGuardHobPtr->BgupHeader.BiosSvn        = BiosGuardHobPtr->Bgpdt.BiosSvn;
  BiosGuardHobPtr->BgupHeader.VendorSpecific = 0x808655AA;

  DEBUG ((DEBUG_INFO, "------------------ BIOS GUARD Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGUP_HEADER : Version : 0x%X\n", BiosGuardHobPtr->BgupHeader.Version));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGUP_HEADER : PlatId[16] :\n"));
  for (Index = 0; Index < 16; Index++) {
      DEBUG ((DEBUG_INFO, " 0x%X", BiosGuardHobPtr->BgupHeader.PlatId[Index]));
  }

  DEBUG ((DEBUG_INFO, " \n"));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGUP_HEADER : PkgAttributes : 0x%X\n", BiosGuardHobPtr->BgupHeader.PkgAttributes));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGUP_HEADER : PslMajorVer : 0x%X\n", BiosGuardHobPtr->BgupHeader.PslMajorVer));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGUP_HEADER : PslMinorVer : 0x%X\n", BiosGuardHobPtr->BgupHeader.PslMinorVer));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGUP_HEADER : ScriptSectionSize : 0x%X\n", BiosGuardHobPtr->BgupHeader.ScriptSectionSize));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGUP_HEADER : DataSectionSize : 0x%X\n", BiosGuardHobPtr->BgupHeader.DataSectionSize));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGUP_HEADER : BiosSvn : 0x%X\n", BiosGuardHobPtr->BgupHeader.BiosSvn));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGUP_HEADER : EcSvn : 0x%X\n", BiosGuardHobPtr->BgupHeader.EcSvn));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGUP_HEADER : VendorSpecific : 0x%X\n", BiosGuardHobPtr->BgupHeader.VendorSpecific));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : BgpdtSize : 0x%X\n", BiosGuardHobPtr->Bgpdt.BgpdtSize));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : BgpdtMajVer : 0x%X\n", BiosGuardHobPtr->Bgpdt.BgpdtMajVer));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : BgpdtMinVer : 0x%X\n", BiosGuardHobPtr->Bgpdt.BgpdtMinVer));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : PlatId[16] :\n"));
  for (Index = 0; Index < 16; Index++) {
      DEBUG ((DEBUG_INFO, " 0x%X", BiosGuardHobPtr->Bgpdt.PlatId[Index]));
  }

  DEBUG ((DEBUG_INFO, " \n"));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : PkeySlot0[48] :\n"));
  for (Index = 0; Index < BIOSGUARD_MAX_HASH_SIZE; Index++) {
    if ((Index == 15) || (Index == 31) || (Index == 47)) {
      DEBUG ((DEBUG_INFO, " 0x%X\n", BiosGuardHobPtr->Bgpdt.PkeySlot0[Index]));
    } else {
      DEBUG ((DEBUG_INFO, " 0x%X", BiosGuardHobPtr->Bgpdt.PkeySlot0[Index]));
    }
  }

  DEBUG ((DEBUG_INFO, " \n"));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : PkeySlot1[48] :\n"));
  for (Index = 0; Index < BIOSGUARD_MAX_HASH_SIZE; Index++) {
    if ((Index == 15) || (Index == 31) || (Index == 47)) {
      DEBUG ((DEBUG_INFO, " 0x%X\n", BiosGuardHobPtr->Bgpdt.PkeySlot1[Index]));
    } else {
      DEBUG ((DEBUG_INFO, " 0x%X", BiosGuardHobPtr->Bgpdt.PkeySlot1[Index]));
    }
  }

  DEBUG ((DEBUG_INFO, " \n"));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : PkeySlot2[48] :\n"));
  for (Index = 0; Index < BIOSGUARD_MAX_HASH_SIZE; Index++) {
    if ((Index == 15) || (Index == 31) || (Index == 47)) {
      DEBUG ((DEBUG_INFO, " 0x%X\n", BiosGuardHobPtr->Bgpdt.PkeySlot2[Index]));
    } else {
      DEBUG ((DEBUG_INFO, " 0x%X", BiosGuardHobPtr->Bgpdt.PkeySlot2[Index]));
    }
  }

  DEBUG ((DEBUG_INFO, " \n"));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : BgModSvn : 0x%X\n", BiosGuardHobPtr->Bgpdt.BgModSvn));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : BiosSvn : 0x%X\n", BiosGuardHobPtr->Bgpdt.BiosSvn));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : ExecLim : 0x%X\n", BiosGuardHobPtr->Bgpdt.ExecLim));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : BiosGuardAttr : 0x%X\n", BiosGuardHobPtr->Bgpdt.BiosGuardAttr));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : EcCmd : 0x%X\n", BiosGuardHobPtr->Bgpdt.EcCmd));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : EcData : 0x%X\n", BiosGuardHobPtr->Bgpdt.EcData));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : EcCmdGetSvn : 0x%X\n", BiosGuardHobPtr->Bgpdt.EcCmdGetSvn));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : EcCmdOpen : 0x%X\n", BiosGuardHobPtr->Bgpdt.EcCmdOpen));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : EcCmdClose : 0x%X\n", BiosGuardHobPtr->Bgpdt.EcCmdClose));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : EcCmdPortTest : 0x%X\n", BiosGuardHobPtr->Bgpdt.EcCmdPortTest));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : LastSfam : 0x%X\n", BiosGuardHobPtr->Bgpdt.LastSfam));
  DEBUG ((DEBUG_INFO, " BIOS_GUARD_CONFIG:: BGPDT : SfamData[64] :\n"));

  //
  // Change the array size according to MAX_SFAM_COUNT
  //
  for (Index = 0; Index <= BiosGuardHobPtr->Bgpdt.LastSfam; Index++) {
    if ((Index == 15) || (Index == 31) || (Index == 47) || (Index == 63) ) {
      DEBUG ((DEBUG_INFO, " 0x%X 0x%X\n", BiosGuardHobPtr->Bgpdt.SfamData[Index].FirstByte,
                                          BiosGuardHobPtr->Bgpdt.SfamData[Index].LastByte));
    } else {
      DEBUG ((DEBUG_INFO, " 0x%X 0x%X,", BiosGuardHobPtr->Bgpdt.SfamData[Index].FirstByte,
                                         BiosGuardHobPtr->Bgpdt.SfamData[Index].LastByte));
    }
  }
  DEBUG ((DEBUG_INFO, " \n"));
  BiosGuardHobPtr->BiosGuardMemAddress = 0;
  BiosGuardHobPtr->BiosGuardMemSize = 0;
  BiosGuardHobPtr->BiosGuardSwSmi = 0;

  return EFI_SUCCESS;
}

/**
  This function locates the TDX ACTM module within the firmware volume and retrieves its address and size.

  @param[in, out] ModulePtr  - Input is a NULL pointer,
                               and output points to the ACTM module address if found.
  @param[out]     ModuleSize - Output the ACTM module size.

  @retval EFI_SUCCESS        - ACTM ACM Module found.
  @retval EFI_NOT_FOUND      - ACTM ACM Module size and/or Address equal to 0.
  @retval Others             - ACTM ACM Module not found.
**/
EFI_STATUS
UpdateTdxActmModulePtr (
  IN OUT EFI_PHYSICAL_ADDRESS   *ModulePtr,
  OUT    UINT32                 *ModuleSize
  )
{
  EFI_STATUS                    Status;
  EFI_FIRMWARE_VOLUME_HEADER    *FvHeader;
  EFI_PEI_FILE_HANDLE           *FfsHeader;
  EFI_PEI_FILE_HANDLE           *FfsRawSectionHeader;
  EFI_PEI_FILE_HANDLE           *FfsRawSectionData;
  EFI_GUID                      *TdxActmModuleGuidPtr;

  DEBUG ((DEBUG_INFO, "Update Tdx Actm Module Pointer and Size\n"));

  Status                 = EFI_SUCCESS;
  TdxActmModuleGuidPtr = &gTdxModuleBinGuid;

  // Locate Firmware Volume header
  FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) FixedPcdGet32 (PcdFlashFvFirmwareBinariesBase);

  // Find the TDX ACTM module file by its GUID
  Status = PeiServicesFfsFindFileByName (TdxActmModuleGuidPtr, FvHeader, (VOID **)&FfsHeader);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error returned by PeiServicesFfsFindFileByName while retrieving TDX ACTM FFS File Handle\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  // Find the raw section data within the TDX ACTM module file
  Status = PeiServicesFfsFindSectionData (EFI_SECTION_RAW, FfsHeader, (VOID **)&FfsRawSectionData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error returned by PeiServicesFfsFindSectionData while retrieving TDX ACTM Address\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  // Set the module pointer to the address of the raw section data
  *ModulePtr = (EFI_PHYSICAL_ADDRESS)((UINT8 *)FfsRawSectionData);

  // Calculate the size of the TDX ACTM module
  FfsRawSectionHeader = (VOID *)((UINT8 *)FfsRawSectionData - sizeof (EFI_COMMON_SECTION_HEADER));
  *ModuleSize = SECTION_SIZE (FfsRawSectionHeader) - sizeof (EFI_COMMON_SECTION_HEADER);

  DEBUG ((DEBUG_ERROR, "Tdx Actm Module Location: 0x%X\n", *ModulePtr ));
  DEBUG(( DEBUG_ERROR, "Tdx Actm Module size: 0x%X\n", *ModuleSize));

  if ((*ModulePtr == 0) || (*ModuleSize == 0)) {
    DEBUG ((DEBUG_ERROR, "Tdx Actm ModuleAddress = 0x%X and/or Tdx Actm Module Size = 0x%X equal to zero\n", *ModulePtr, *ModuleSize));
    ASSERT_EFI_ERROR (Status);
    return EFI_NOT_FOUND;
  }

  return Status;
}

/**
  Reports if detected reset type is warm or not
  @retval TRUE  Reset type is a warm reset
  @retval FALSE Other reset type
**/
BOOLEAN
IsWarmReset (
  VOID
  )
{
  UINT32 RegisterVal32 = MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A);

  if ((((RegisterVal32 & B_PMC_PWRM_GEN_PMCON_A_MEM_SR) != 0) &&
      ((RegisterVal32 & B_PMC_PWRM_GEN_PMCON_A_DISB) != 0))) {
    return TRUE;
  }

  return FALSE;
}

/**
  Get ACTM MAC from NVRAM and save in PCD.

  @param [in] VariableServices - Pointer to VariableServices

  @retval EFI_SUCCESS          - ACTM ACM Module found.
  @retval EFI_NOT_FOUND        - ACTM ACM Module size and/or Address equal to 0.
  @retval Others               - ACTM ACM Module not found.
**/
EFI_STATUS
SaveActmMacInPcd (
  IN EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8       *ActmMac = NULL;
  UINTN       VarSize;
  UINT64      Data;
  UINT32      Offset;

  DEBUG ((DEBUG_INFO, "[TDX] Get ACTM MAC from NVRAM and save in PCD.\n"));

  ActmMac = AllocateZeroPool (SIZE_OF_MANIFEST_MAC);
  if (ActmMac == NULL) {
    DEBUG ((DEBUG_ERROR, "[TDX] ERROR: AllocateZeroPool failure!\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  VarSize = (UINTN)SIZE_OF_MANIFEST_MAC;

  // 1. Get Actm Mac from Nv variable
  Status = VariableServices->GetVariable (
            VariableServices,
            L"TdxActmMacNvVar",
            &gTdxActmMacNvVarGuid,
            NULL,
            &VarSize,
            (void *)ActmMac);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[TDX] ERROR: GetVariable() return %r\n", Status));
    FreePool (ActmMac);
    return Status;
  }

  // 2. Set PCD's with recovered ACTM MAC
  CopyMem (&Data, ActmMac, sizeof (Data));
  Status = PcdSetEx64S (&gSiPkgTokenSpaceGuid, PcdTdxActmMac0, Data);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[TDX] ERROR: PcdSetEx64S() return %r\n", Status));
    FreePool (ActmMac);
    return Status;
  }

  Offset = sizeof (Data);
  CopyMem (&Data, &ActmMac[Offset], sizeof (Data));
  Status = PcdSetEx64S (&gSiPkgTokenSpaceGuid, PcdTdxActmMac1, Data);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[TDX] ERROR: PcdSetEx64S() return %r\n", Status));
    FreePool (ActmMac);
    return Status;
  }

  Offset += sizeof (Data);
  CopyMem (&Data, &ActmMac[Offset], sizeof (Data));
  Status = PcdSetEx64S (&gSiPkgTokenSpaceGuid, PcdTdxActmMac2, Data);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[TDX] ERROR: PcdSetEx64S() return %r\n", Status));
    FreePool (ActmMac);
    return Status;
  }

  Offset += sizeof (Data);
  CopyMem (&Data, &ActmMac[Offset], sizeof (Data));
  Status = PcdSetEx64S (&gSiPkgTokenSpaceGuid, PcdTdxActmMac3, Data);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[TDX] ERROR: PcdSetEx64S() return %r\n", Status));
    FreePool (ActmMac);
    return Status;
  }

  return Status;
}

/**
  Create G3 State Hob
**/
VOID
CreateSysG3StateHob (
  VOID
  )
{
  EFI_STATUS                        Status;
  SYS_G3_STATE_HOB                  *SysG3StateHob;

  Status = EFI_SUCCESS;
  SysG3StateHob = (SYS_G3_STATE_HOB *) GetFirstGuidHob (&gG3StateHobGuid);
  if (SysG3StateHob == NULL) {
    //
    // Create HOB for system G3 state
    //
    Status = PeiServicesCreateHob (
               EFI_HOB_TYPE_GUID_EXTENSION,
               sizeof (SYS_G3_STATE_HOB),
               (VOID **) &SysG3StateHob
               );

    if (EFI_ERROR (Status)) {
      return;
    }
    DEBUG ((DEBUG_INFO, "SysG3StateHob Created\n"));
    SysG3StateHob->EfiHobGuidType.Name = gG3StateHobGuid;
  }

  //
  // Record G3 status in SysG3StateHob
  //
  if (PmcIsPowerFailureDetected ()) {
    SysG3StateHob->SysG3Status = 1;
  } else {
    SysG3StateHob->SysG3Status = 0;
  }
}

/**
  This function gets VR Power Design.

  @param[in]  SelectedCtdpLevel      Selected Ctdp level in Setup knob
  @param[in]  CustomPowerLimit1Power PL1 set in BIOS knob
  @param[in]  CustomPowerLimit2Power PL2 set in BIOS knob

  @retval     return VR Power Design
**/
UINT8
EFIAPI
GetVrPowerDeliveryDesign (
  IN  UINT8  SelectedCtdpLevel,
  IN  UINT32 CustomPowerLimit1Power,
  IN  UINT32 CustomPowerLimit2Power
  )
{
  UINT16                            CtdpNominalTdp,CtdpLevel1Tdp,CtdpLevel2Tdp;
  UINT8                             VrPowerDeliveryDesign;
  UINT8                             MaxCtdpLevel;
  UINT8                             CpuConfigTdpLevels;
  UINT16                            CpuDid;
  UINT16                            BoardId;
  BOOLEAN                           IsRvpBoard;
  MSR_CONFIG_TDP_LEVEL1_REGISTER    ConfigTdpLevel1Msr;
  MSR_CONFIG_TDP_LEVEL2_REGISTER    ConfigTdpLevel2Msr;
  MSR_PACKAGE_POWER_SKU_REGISTER    PackagePowerSkuMsr;

  VrPowerDeliveryDesign = 0;
  MaxCtdpLevel          = 0;
  CpuConfigTdpLevels    = GetConfigTdpLevels ();
  BoardId               = PcdGet16 (PcdBoardId);
  IsRvpBoard            = FALSE;

  if ((BoardId == BoardIdPtlUHLp5Rvp1) ||
      (BoardId == BoardIdPtlUHLp5Rvp3) ||
      (BoardId == BoardIdPtlUHDdr5Rvp4) ||
      (BoardId == BoardIdPtlUHCammDTbTRvp2) ||
      (BoardId == BoardIdPtlUHLp5MemSktmRvp) ||
      (BoardId == BoardIdPtlUHHmpRvp)) {
    IsRvpBoard = TRUE;
  }

  ///
  /// Get Ratio and TDP for each cTDP level
  ///
  PackagePowerSkuMsr.Uint64 = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU);
  CtdpNominalTdp = (UINT16) PackagePowerSkuMsr.Bits.PkgTdp;
  ConfigTdpLevel1Msr.Uint64   = AsmReadMsr64 (MSR_CONFIG_TDP_LEVEL1);
  CtdpLevel1Tdp = (UINT16) ConfigTdpLevel1Msr.Bits.PkgTdp;
  ConfigTdpLevel2Msr.Uint64   = AsmReadMsr64 (MSR_CONFIG_TDP_LEVEL2);
  CtdpLevel2Tdp = (UINT16) ConfigTdpLevel2Msr.Bits.PkgTdp;

  CpuDid = (UINT16) GetHostBridgeRegisterData (HostBridgeDeviceId, HostBridgeDeviceIdData);
  DEBUG ((DEBUG_INFO, " CpuDID = 0x%X\n", CpuDid));
  DEBUG ((DEBUG_INFO, " BoardId = 0x%X\n", BoardId));
  DEBUG ((DEBUG_INFO, " IsRvpBoard = 0x%X\n", IsRvpBoard));

  ///
  /// All cases when BIOS shall override PD.
  ///
  /*
  --------------+---------------------+----------+
  | Ctdp level  |      Board          | PD       |
  +-------------+---------------------+----------+
  |  Nominal    |                     |          |
  |     or      |     NA(don't care)  | Official |
  |  cTDP Down  |                     |          |
  +-------------+---------------------+----------+
  |             |        RVP          | PD3      |
  |  cTDP Up    |---------------------+----------+
  | (4Xe/12Xe)  |       others        | PD2      |
  +-------------+---------------------+----------+

  An unique case:
    When cTDP level is nominal, Board is Intel RVP. Once PL2 is set to 80W and PL1 is set to 45W,
    then PD2 will be used to help PPG and PnP validation.
  */
  if (CpuConfigTdpLevels != 0) {
    DEBUG ((DEBUG_INFO, " CpuConfigTdpLevels = 0x%X\n", CpuConfigTdpLevels));
    ///
    /// Get the cTDP level with the highest TDP
    ///
    if ((CpuConfigTdpLevels == 1) && (CtdpLevel1Tdp >= CtdpNominalTdp)) {
      MaxCtdpLevel = 1;
    }
    if (CpuConfigTdpLevels == 2) {
      if ((CtdpLevel1Tdp >= CtdpNominalTdp) && (CtdpLevel1Tdp >= CtdpLevel2Tdp)) {
        MaxCtdpLevel = 1;
      } else if ((CtdpLevel2Tdp >= CtdpNominalTdp) && (CtdpLevel2Tdp >= CtdpLevel1Tdp)) {
        MaxCtdpLevel = 2;
      }
    }

    ///
    /// Confirm selected cTDP is the maximum assured power.
    ///
    if (SelectedCtdpLevel == MaxCtdpLevel) {
      DEBUG ((DEBUG_INFO, " SelectedCtdpLevel is 0x%X\n", SelectedCtdpLevel));
      if (IsRvpBoard) {
        ///
        /// If detected board is Intel RVP, BIOS set PD3 (95W PL2) as default.
        ///
        if (CpuDid == PTL_H_4XE_SA_DEVICE_ID_4C_8A) { // 4+8+4+4Xe
          VrPowerDeliveryDesign = 0x1C; // PD3: EnumPtlH4Xe65Watt48Pd3CpuId
        } else if (CpuDid == PTL_H_12XE_SA_DEVICE_ID_4C_8A) { // 4+8+4+12Xe
          VrPowerDeliveryDesign = 0x1D; // PD3: EnumPtlH12Xe65Watt48Pd3CpuId
        }
      } else {
        DEBUG ((DEBUG_INFO, "it is Not RVP\n"));
        ///
        /// If detected board is not Intel RVP, BIOS sets PD2 as default for H 4Xe/12Xe 4+8+4.
        ///
        if (CpuDid == PTL_H_4XE_SA_DEVICE_ID_4C_8A) { // 4+8+4+4Xe
          VrPowerDeliveryDesign = 0x1A; // PD2: EnumPtlH4Xe65Watt48Pd2CpuId
        } else if (CpuDid == PTL_H_12XE_SA_DEVICE_ID_4C_8A) { // 4+8+4+12Xe
          VrPowerDeliveryDesign = 0x1B; // PD2: EnumPtlH12Xe65Watt48Pd2CpuId
        }
      }
    }
  }

  ///
  /// The case includes:
  ///   1) Ctdp Up/Down supports but select Nominal as default.
  ///   2) Ctdp Up/Down unsupports. By default only Nominal supports.
  ///
  if (SelectedCtdpLevel == 0) {
    ///
    /// The unique case that RVP + PL1 45W + PL2 80W, BIOS sets PD2 as default which used for PPG and PnP validation.
    ///
    DEBUG ((DEBUG_INFO, "CustomPowerLimit1Power is - %x, CustomPowerLimit2Power is - %x \n", CustomPowerLimit1Power, CustomPowerLimit2Power));
    if ((IsRvpBoard) && (CustomPowerLimit2Power == 0x13880) && (CustomPowerLimit1Power == 0xAFC8)) {
      if (CpuDid == PTL_H_4XE_SA_DEVICE_ID_4C_8A) { // 4+8+4+4Xe
        VrPowerDeliveryDesign = 0x1A; // PD2: EnumPtlH4Xe65Watt48Pd2CpuId
      } else if (CpuDid == PTL_H_12XE_SA_DEVICE_ID_4C_8A) { // 4+8+4+12Xe
        VrPowerDeliveryDesign = 0x1B; // PD2: EnumPtlH12Xe65Watt48Pd2CpuId
      }
    }
  }

  DEBUG ((DEBUG_INFO, "VrPowerDeliveryDesign is - %x \n", VrPowerDeliveryDesign));
  return (UINT8) VrPowerDeliveryDesign;
}

/**
  This function performs CPU PEI Policy initialization in Pre-memory.

  @retval EFI_SUCCESS              The PPI is installed and initialized.
  @retval EFI ERRORS               The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES     Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicyPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINTN                           VariableSize;
  SETUP_DATA                      SetupData;
  CRASHLOG_VARIABLE               CrashLogVariable;
  CPU_SETUP                       CpuSetup;
  EFI_BOOT_MODE                   BootMode;

  UINT16                          Index;
  UINT32                          MaxLogicProcessors;
  BIOSGUARD_HOB                   *BiosGuardHobPtr;
  UINT16                          BiosSize;
  UINT16                          BiosMemSizeInMb;
  UINT8                           BiosGuardToolsInterface;
  MSR_CORE_THREAD_COUNT_REGISTER  MsrCoreThreadCount;
  UINT16                          MaxNumVrs;

  UINT8                           PowerSourceType;
  BOOLEAN                         IsAcPluggedIn;
  EFI_STATUS                      PowerSourceStatus;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  TXT_PREMEM_CONFIG               *TxtPreMemConfig;
  CPU_POWER_DELIVERY_CONFIG       *CpuPowerDeliveryConfig;
  CPU_INIT_PREMEM_CONFIG          *CpuInitPreMemConfig;
  CPU_SECURITY_PREMEM_CONFIG      *CpuSecurityPreMemConfig;
  CPU_POWER_MGMT_VR_CONFIG        *CpuPowerMgmtVrConfig;
#endif
  UINT8                            VrPowerDeliveryDesign;
  UINT16                           CpuDid;


  BiosGuardHobPtr             = NULL;
  BiosSize                    = 0;
  BiosMemSizeInMb             = 0;
  BiosGuardToolsInterface     = FALSE;

#if FixedPcdGetBool (PcdTdxEnable) == 1
  EFI_PHYSICAL_ADDRESS            ModulePtr  = 0;
  UINT32                          ModuleSize = 0;
#endif

  DEBUG ((DEBUG_INFO, "Update PeiCpuPolicyUpdate Pre-Mem Start\n"));

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  TxtPreMemConfig             = NULL;
  CpuPowerDeliveryConfig      = NULL;
  CpuInitPreMemConfig         = NULL;
  CpuSecurityPreMemConfig     = NULL;
  CpuPowerMgmtVrConfig        = NULL;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTxtPreMemConfigGuid, (VOID *) &TxtPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuInitPreMemConfigGuid, (VOID *) &CpuInitPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuPowerMgmtVrConfigGuid, (VOID *) &CpuPowerMgmtVrConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuPowerDeliveryConfigGuid, (VOID *) &CpuPowerDeliveryConfig);
  ASSERT_EFI_ERROR (Status);
#endif
  //
  // Make sure ReadOnlyVariablePpi is available
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Get Setup Variable
  //
  VariableSize = sizeof (CPU_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"CpuSetup",
                               &gCpuSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &CpuSetup
                               );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &SetupData
                               );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (CRASHLOG_VARIABLE);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"CrashLogVariable",
                               &gCrashLogVariableGuid,
                               NULL,
                               &VariableSize,
                               &CrashLogVariable
                               );
  ASSERT_EFI_ERROR (Status);

  //
  // Get current boot mode
  //
  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  #if FixedPcdGetBool (PcdTdxEnable) == 1
    //
    // Update TDx Policy
    //
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TdxEnable, CpuSecurityPreMemConfig->TdxEnable, CpuSetup.TdxEnable);

    //
    // Get the ACTM address and ACTM Base and update the policy
    //
    UpdateTdxActmModulePtr (&ModulePtr, &ModuleSize);

    //
    // Get ACTM MAC from NVRAM and save in PCD on WarmReset
    //
    if (IsWarmReset ()) {
      SaveActmMacInPcd (VariableServices);
    }

    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TdxActmModuleAddr, CpuSecurityPreMemConfig->TdxActmModuleAddr, ModulePtr);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TdxActmModuleSize, CpuSecurityPreMemConfig->TdxActmModuleSize, ModuleSize);
  #endif

  //
  // Set TME exclude base and size to 0
  // Set GenerateNewTmeKey to disable
  //
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TmeExcludeBase, CpuSecurityPreMemConfig->TmeExcludeBase, 0);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TmeExcludeSize, CpuSecurityPreMemConfig->TmeExcludeSize, 0);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.GenerateNewTmeKey, CpuSecurityPreMemConfig->GenerateNewTmeKey, CPU_FEATURE_DISABLE);
  //
  // Update TXT Platform Policy
  //
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TseEnable, CpuSecurityPreMemConfig->TseEnable, CpuSetup.TseEnable);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Txt, CpuSecurityPreMemConfig->Txt, CpuSetup.Txt);

  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TxtDprMemorySize, TxtPreMemConfig->TxtDprMemorySize, (UINTN)(CpuSetup.DprSize * 0x100000));
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TxtDprMemorySize, TxtPreMemConfig->TxtDprMemorySize, CpuSetup.DprSize * 0x100000);

#if FixedPcdGet8(PcdFspModeSelection) == 0
  TxtPreMemConfig->TxtImplemented = 0;
#endif

  //
  // Create G3 State Hob
  //
  CreateSysG3StateHob ();

  //
  // Check PWR FLR and clear it if it's set.
  // This will be handled by ModularUsbCIoPei driver when Modular USBC IO is supported.
  // The reason behind this is that ModularUsbCIoPei driver won't be able to detect G3
  // when there is reset happened prior to the G3 detection in ModularUsbCIoPei driver
  // during pre-mem.
  //
  if (((PcdGet64 (PcdPlatformModularUsbCIoConfig) == 0) || (BootMode == BOOT_ON_S3_RESUME)) && PmcIsPowerFailureDetected ()) {
    PmcClearPowerFailureStatus ();
  }

  //
  // When the SUS_PWR_FLR bit is set, it indicates VRMPWRGD signal from the CPU VRM went low.
  // Software must clear this bit if set.
  //
  if (PmcIsSusPowerFailureDetected ()) {
    PmcClearSusPowerFailureStatus ();
  }

  if (CpuSetup.SkipStopPbet == 1) {
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SkipStopPbet, CpuSecurityPreMemConfig->SkipStopPbet, TRUE);
  } else {
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SkipStopPbet, CpuSecurityPreMemConfig->SkipStopPbet, FALSE);
  }

  ///
  /// Set BIOS Guard Tools Interface policy
  ///
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.BiosGuardToolsInterface, CpuSecurityPreMemConfig->BiosGuardToolsInterface, CPU_FEATURE_ENABLE);

  SpiServiceInit ();

#if FixedPcdGetBool(PcdBiosGuardEnable) == 1
  //
  // Update BIOS Guard Policy

  if (IsSimicsEnvironment () || IsHfpgaEnvironment ()) {
    //
    // Todo: Enable Bios Guard when it is supported on SIMICS and HFPGA
    //
    DEBUG ((DEBUG_INFO, "BiG: Disable Bios Guard on SIMICS and HFPGA\n"));
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.BiosGuard, CpuSecurityPreMemConfig->BiosGuard, FALSE);
  } else {
    DEBUG ((DEBUG_INFO, "BiG: Enable Bios Guard\n"));
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.BiosGuard, CpuSecurityPreMemConfig->BiosGuard, CPU_FEATURE_ENABLE);
  }


#if FixedPcdGet8(PcdFspModeSelection) == 1
  if (((FSPM_UPD *) FspmUpd)->FspmConfig.BiosGuard== TRUE) {
#else
  if (CpuSecurityPreMemConfig->BiosGuard == TRUE) {
#endif
    Status = BiosGuardHobInit ();
    ASSERT_EFI_ERROR (Status);
  }

      BiosGuardHobPtr = GetFirstGuidHob (&gBiosGuardHobGuid);
      if (BiosGuardHobPtr == NULL) {
        DEBUG ((DEBUG_ERROR, "BIOS Guard HOB not available\n"));
      }else {
#if FixedPcdGet8(PcdFspModeSelection) == 1 // #if API Mode
        BiosSize = ((FSPM_UPD *) FspmUpd)->FspmConfig.BiosSize;
        BiosGuardToolsInterface |= (UINT8)(((FSPM_UPD *) FspmUpd)->FspmConfig.BiosGuardToolsInterface);
#else // Dispatch Mode
        BiosSize = CpuSecurityPreMemConfig->BiosSize;
        BiosGuardToolsInterface |= (UINT8)CpuSecurityPreMemConfig->BiosGuardToolsInterface;
#endif // #endif API Mode

        ///
        /// Check if BiosGuardToolsInterface policy is enabled
        /// If Enabled, BGUP data will use the memory allocated within DPR
        /// If Disabled, the size allocated to BIOS Guard in DPR is 0MB, BGUP data will be stored in TSEG
        ///
        if (BiosGuardToolsInterface == TRUE) {
          ///
          /// The size allocated for DPR needs to account for the size of the BIOS ROM plus the size of the
          /// BGUP data: BGUP Header, BGUP Script,and BGUP Certificate.
          /// The size allocated needs to aligned to 1MB due to 1MB granularity of DPR.
          ///
          if ((BiosSize % 1024) == 0) {
            BiosMemSizeInMb = (BiosSize / 1024) + 1;
          } else {
            ///
            /// For a BiosSize that is not exactly 1MB, additional space will need to be added
            /// With consideration of the BGUP data, we can add an additional 2MB for a BIOS
            /// with greater than X.5MB, where X represents whole number value of the BiosSize
            ///
            if ((BiosSize % 1024) < 512) {
              BiosMemSizeInMb = (BiosSize / 1024) + 1;
            } else {
              BiosMemSizeInMb = (BiosSize / 1024) + 2;
            }
          }
          ///
          /// Upon MTRR shortage, BiosMemSizeInMb can be 2MB aligned at the expense of losing memory.
          ///
          if ((BiosMemSizeInMb % 2) == 1) {
            BiosMemSizeInMb = BiosMemSizeInMb + 1;
          }
        }

        BiosGuardHobPtr->BiosGuardMemSize = (UINT8)BiosMemSizeInMb;
      }
#endif

  //
  // Update CpuConfigLibPreMem Config Block data
  //
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.BootMaxFrequency, CpuInitPreMemConfig->BootMaxFrequency, CpuSetup.BootMaxFrequency);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ActiveCoreCount, CpuInitPreMemConfig->ActiveCoreCount, CpuSetup.ActiveCoreCount);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ActiveSmallCoreCount, CpuInitPreMemConfig->ActiveSmallCoreCount, CpuSetup.ActiveSmallCoreCount);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ActiveLpAtomCoreCount, CpuInitPreMemConfig->ActiveLpAtomCoreCount, CpuSetup.ActiveLpAtomCoreCount);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TsegSize, CpuInitPreMemConfig->TsegSize, PcdGet32 (PcdTsegSize));

  //
  // If EpocFclkFreq is AUTO, then to load policy based on Silicon default
  //
  if (CpuSetup.EpocFclkFreq != EPOCFCLKFREQ_AUTO_VALUE) {
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.FClkFrequency, CpuInitPreMemConfig->FClkFrequency, CpuSetup.EpocFclkFreq);
  }
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.BistOnReset, CpuInitPreMemConfig->BistOnReset, CpuSetup.BistOnReset);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ReduceXecores, CpuInitPreMemConfig->ReduceXecores, CpuSetup.ReduceXecores);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VmxEnable, CpuInitPreMemConfig->VmxEnable, CpuSetup.VT);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TmeEnable, CpuInitPreMemConfig->TmeEnable, CpuSetup.TmeEnable);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CpuCrashLogEnable, CpuInitPreMemConfig->CrashLogEnable, CrashLogVariable.EnableCrashLog);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CrashLogGprs, CpuInitPreMemConfig->CrashLogGprs, CpuSetup.CrashLogGprs);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DebugInterfaceEnable, CpuInitPreMemConfig->DebugInterfaceEnable, CpuSetup.DebugInterfaceEnable);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DfdEnable, CpuInitPreMemConfig->DfdEnable, CpuSetup.DfdEnable);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.OcLock, CpuInitPreMemConfig->OcLock, CpuSetup.OverclockingLock);
#if FixedPcdGet8(PcdFspModeSelection) == 0
  COMPARE_AND_UPDATE_POLICY (NULL, CpuInitPreMemConfig->TmeBypassCapability, CpuSetup.TmeBypassCapability);
#endif
  if (BiosGuardHobPtr != NULL) {
    if (IsBiosGuardModuleDebugSigned2 (BiosGuardHobPtr->BiosGuardModulePtr)) {
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DebugInterfaceEnable, CpuInitPreMemConfig->DebugInterfaceEnable, CPU_FEATURE_ENABLE);
    }
  }
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DebugInterfaceLockEnable, CpuInitPreMemConfig->DebugInterfaceLockEnable, CpuSetup.DebugInterfaceLockEnable);
  if (CpuSetup.CpuRatioOverride) {
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CpuRatio, CpuInitPreMemConfig->CpuRatio, CpuSetup.CpuRatio);
  } else {
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CpuRatio, CpuInitPreMemConfig->CpuRatio, 0);
  }

  //
  // VR Configuration
  //
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PsysSlope, CpuPowerMgmtVrConfig->PsysSlope, CpuSetup.PsysSlope);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PsysPmax, CpuPowerMgmtVrConfig->PsysPmax,  CpuSetup.PsysPmax );
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PsysOffset, CpuPowerMgmtVrConfig->PsysOffset, (INT32) CpuSetup.PsysOffset *((CpuSetup.PsysOffsetPrefix == 1) ? -1 : 1));

  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DlvrSpreadSpectrumPercentage, CpuPowerMgmtVrConfig->DlvrSpreadSpectrumPercentage, CpuSetup.DlvrSpreadSpectrumPercentage);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DlvrRfiFrequency, CpuPowerMgmtVrConfig->DlvrRfiFrequency, CpuSetup.DlvrRfiFrequency);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DlvrRfiEnable, CpuPowerMgmtVrConfig->DlvrRfiEnable, CpuSetup.DlvrRfiEnable);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DlvrPhaseSsc, CpuPowerMgmtVrConfig->DlvrPhaseSsc, CpuSetup.DlvrPhaseSsc);

  //
  // Vsys Critical
  //
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.EnableVsysCritical, CpuPowerMgmtVrConfig->EnableVsysCritical, CpuSetup.EnableVsysCritical);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VsysFullScale, CpuPowerMgmtVrConfig->VsysFullScale, CpuSetup.VsysFullScale);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VsysCriticalThreshold, CpuPowerMgmtVrConfig->VsysCriticalThreshold, CpuSetup.VsysCriticalThreshold);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PsysFullScale, CpuPowerMgmtVrConfig->PsysFullScale, CpuSetup.PsysFullScale);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PsysCriticalThreshold, CpuPowerMgmtVrConfig->PsysCriticalThreshold, CpuSetup.PsysCriticalThreshold);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VsysAssertionDeglitchMantissa, CpuPowerMgmtVrConfig->VsysAssertionDeglitchMantissa, CpuSetup.VsysAssertionDeglitchMantissa);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VsysAssertionDeglitchExponent, CpuPowerMgmtVrConfig->VsysAssertionDeglitchExponent, CpuSetup.VsysAssertionDeglitchExponent);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VsysDeassertionDeglitchMantissa, CpuPowerMgmtVrConfig->VsysDeassertionDeglitchMantissa, CpuSetup.VsysDeassertionDeglitchMantissa);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VsysDeassertionDeglitchExponent, CpuPowerMgmtVrConfig->VsysDeassertionDeglitchExponent, CpuSetup.VsysDeassertionDeglitchExponent);

  ///
  /// Define Maximum Number of Voltage Regulator Domains.
  ///
  MaxNumVrs = GetMaxNumVrs ();
  
  for (Index = 0; Index < MaxNumVrs; Index++) {
    //
    // Only update if the user wants to override VR settings
    //
    if (CpuSetup.VrConfigEnable[Index] != 0) {
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.VrConfigEnable[Index], CpuPowerMgmtVrConfig->VrConfigEnable[Index], CpuSetup.VrConfigEnable[Index], Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.Ps1Threshold[Index], CpuPowerMgmtVrConfig->Ps1Threshold[Index], CpuSetup.Ps1Threshold[Index], Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.Ps2Threshold[Index], CpuPowerMgmtVrConfig->Ps2Threshold[Index], CpuSetup.Ps2Threshold[Index], Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.Ps3Threshold[Index], CpuPowerMgmtVrConfig->Ps3Threshold[Index], CpuSetup.Ps3Threshold[Index], Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.Ps3Enable[Index], CpuPowerMgmtVrConfig->Ps3Enable[Index], CpuSetup.Ps3Enable[Index], Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.Ps4Enable[Index], CpuPowerMgmtVrConfig->Ps4Enable[Index], CpuSetup.Ps4Enable[Index], Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.ImonSlope[Index], CpuPowerMgmtVrConfig->ImonSlope[Index], CpuSetup.ImonSlope[Index], Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.ImonOffset[Index], CpuPowerMgmtVrConfig->ImonOffset[Index], (INT32)CpuSetup.ImonOffset[Index] * ((CpuSetup.ImonOffsetPrefix[Index] == 1) ? -1 : 1), Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.EnableFastVmode[Index], CpuPowerMgmtVrConfig->EnableFastVmode[Index],CpuSetup.EnableFastVmode[Index], Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.CepEnable[Index], CpuPowerMgmtVrConfig->CepEnable[Index], CpuSetup.CepEnable[Index], Index);

      //
      // Only update if IccMax is non-zero. This is to distinguish between the default EDS override.
      //
      if (CpuSetup.IccMax[Index] != 0) {
        COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.IccMax[Index], CpuPowerMgmtVrConfig->IccMax[Index], CpuSetup.IccMax[Index], Index);
      }
      //
      // Only update if IccLimit is non-zero. This is to distinguish between the default EDS override.
      //
      if (CpuSetup.EnableFastVmode[Index] == 1 && CpuSetup.IccLimit[Index] != 0) {
        COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.IccLimit[Index], CpuPowerMgmtVrConfig->IccLimit[Index], CpuSetup.IccLimit[Index], Index);
      }
      //
      // Only update if Vr Voltage Limit is non-zero.
      //
      if (CpuSetup.VrVoltageLimit[Index] != 0) {
        COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.VrVoltageLimit[Index], CpuPowerMgmtVrConfig->VrVoltageLimit[Index], CpuSetup.VrVoltageLimit[Index], Index);
      }
      // Only update TDC if current limit is non-zero. TDC Lock should be separated to allow
      // locking of TDC feature.
      //
      if (CpuSetup.TdcCurrentLimit[Index] != 0) {
        COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.TdcCurrentLimit[Index], CpuPowerMgmtVrConfig->TdcCurrentLimit[Index], CpuSetup.TdcCurrentLimit[Index], Index);
      }
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.TdcTimeWindow[Index], CpuPowerMgmtVrConfig->TdcTimeWindow[Index], CpuSetup.TdcTimeWindow[Index], Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.TdcMode[Index], CpuPowerMgmtVrConfig->TdcMode[Index], CpuSetup.TdcMode[Index], Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.TdcEnable[Index], CpuPowerMgmtVrConfig->TdcEnable[Index], CpuSetup.TdcEnable[Index], Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.TdcLock[Index], CpuPowerMgmtVrConfig->TdcLock[Index], CpuSetup.TdcLock[Index], Index);
    }
    // Need to check if the user intends to override through CpuSetup to distinguish
    // between the default EDS override.
    if (CpuSetup.AcLoadline[Index] != 0) {
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.AcLoadline[Index], CpuPowerMgmtVrConfig->AcLoadline[Index], CpuSetup.AcLoadline[Index], Index);
    }
    if (CpuSetup.DcLoadline[Index] != 0) {
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.DcLoadline[Index], CpuPowerMgmtVrConfig->DcLoadline[Index], CpuSetup.DcLoadline[Index], Index);
    }
    //
    // VCCSA slew rate option in all the platforms from CML and future products was removed.
    //
    if (Index == 2) {
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SlowSlewRate[Index], CpuPowerMgmtVrConfig->SlowSlewRate[Index], 0xFF);
    } else {
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.SlowSlewRate[Index], CpuPowerMgmtVrConfig->SlowSlewRate[Index], CpuSetup.SlowSlewRate[Index], Index);
    }
    COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.FastPkgCRampDisable[Index], CpuPowerMgmtVrConfig->FastPkgCRampDisable[Index], CpuSetup.FastPkgCRampDisable[Index], Index);

  }

  //
  // VR Acoustic Noise Mitigation
  //
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.EcoreHysteresisWindow, CpuPowerMgmtVrConfig->EcoreHysteresisWindow,   CpuSetup.EcoreHysteresisWindow  );

  ///
  /// Update VrPowerDeliveryDesign when cTDP up selected
  ///
  VrPowerDeliveryDesign = GetVrPowerDeliveryDesign (CpuSetup.ConfigTdpLevel, CpuSetup.CustomPowerLimit1Power, CpuSetup.CustomPowerLimit2Power);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VrPowerDeliveryDesign,  CpuPowerMgmtVrConfig->VrPowerDeliveryDesign, VrPowerDeliveryDesign);

  //
  // When overclocking is enabled, we need to ensure the VR defaults are
  // optimized for overclocking
  //

#if FixedPcdGetBool(PcdOverclockEnable) == 1

      //
      // Disable VR TDC for all VR domains
      //
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TdcCurrentLimit[Index], CpuPowerMgmtVrConfig->TdcCurrentLimit[Index], 0);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TdcEnable[Index], CpuPowerMgmtVrConfig->TdcEnable[Index], 0);

      //
      // Disable noise mitigation and keep fast slew rate
      //
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.FastPkgCRampDisable[Index], CpuPowerMgmtVrConfig->FastPkgCRampDisable[Index], 0);
#endif
  //
  // Update Cpu Power Delivery policy
  //
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DisableVrThermalAlert, CpuPowerDeliveryConfig->DisableVrThermalAlert, CpuSetup.DisableVrThermalAlert);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ThermalMonitor, CpuPowerDeliveryConfig->ThermalMonitor, CpuSetup.EnableThermalMonitor);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ConfigTdpLevel, CpuPowerDeliveryConfig->ConfigTdpLevel, CpuSetup.ConfigTdpLevel);

  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TccActivationOffset, CpuPowerDeliveryConfig->TccActivationOffset, CpuSetup.TCCActivationOffset);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TccOffsetLock, CpuPowerDeliveryConfig->TccOffsetLock, CpuSetup.TccOffsetLock);

  if (CpuSetup.ConfigTdpLock == 1 || PcdGetBool (VpdPcdIpfEnable)) {
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ConfigTdpBios, CpuPowerDeliveryConfig->ConfigTdpBios, 0);
  } else {
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ConfigTdpBios, CpuPowerDeliveryConfig->ConfigTdpBios, CpuSetup.ConfigTdpBios);
  }
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PowerLimit2, CpuPowerDeliveryConfig->PowerLimit2, CpuSetup.PowerLimit2);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ConfigTdpLock, CpuPowerDeliveryConfig->ConfigTdpLock, CpuSetup.ConfigTdpLock);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Custom1PowerLimit1, CpuPowerDeliveryConfig->CustomPowerLimit1, (UINT16) (CpuSetup.CustomPowerLimit1Power / 125));
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Custom1PowerLimit2, CpuPowerDeliveryConfig->CustomPowerLimit2, (UINT16) (CpuSetup.CustomPowerLimit2Power / 125));
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CustomPowerLimit1Time, CpuPowerDeliveryConfig->CustomPowerLimit1Time, CpuSetup.CustomPowerLimit1Time);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CustomTurboActivationRatio, CpuPowerDeliveryConfig->CustomTurboActivationRatio, CpuSetup.CustomTurboActivationRatio);

  //
  // Turbo Mode setting
  //
  if (CpuSetup.LongDurationPwrLimitOverride) {
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PowerLimit1, CpuPowerDeliveryConfig->PowerLimit1, (UINT16) (CpuSetup.PowerLimit1 / 125));
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PowerLimit1Time, CpuPowerDeliveryConfig->PowerLimit1Time, CpuSetup.PowerLimit1Time);
  }

  if (CpuSetup.PowerLimit2) {
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PowerLimit2Power, CpuPowerDeliveryConfig->PowerLimit2Power, (UINT16) (CpuSetup.PowerLimit2Power / 125));
  }

  if (CpuSetup.PowerLimit3Override) {
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PowerLimit3, CpuPowerDeliveryConfig->PowerLimit3, (UINT16) (CpuSetup.PowerLimit3 / 125) );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PowerLimit3Time, CpuPowerDeliveryConfig->PowerLimit3Time, CpuSetup.PowerLimit3Time);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PowerLimit3DutyCycle, CpuPowerDeliveryConfig->PowerLimit3DutyCycle, CpuSetup.PowerLimit3DutyCycle);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PowerLimit3Lock, CpuPowerDeliveryConfig->PowerLimit3Lock, CpuSetup.PowerLimit3Lock);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ResponseMode, CpuPowerDeliveryConfig->ResponseMode, CpuSetup.ResponseMode);
  }

  if (CpuSetup.PowerLimit4Override) {
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PowerLimit4, CpuPowerDeliveryConfig->PowerLimit4, (UINT16) (CpuSetup.PowerLimit4 / 125));
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PowerLimit4Lock, CpuPowerDeliveryConfig->PowerLimit4Lock, CpuSetup.PowerLimit4Lock);
  }

  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PowerLimit4Boost, CpuPowerDeliveryConfig->PowerLimit4Boost, (UINT16) (CpuSetup.PowerLimit4Boost / 125));
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PsysPowerLimit1, CpuPowerDeliveryConfig->PsysPowerLimit1, CpuSetup.PlatformPowerLimit1Enable);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PsysPowerLimit2, CpuPowerDeliveryConfig->PsysPowerLimit2, CpuSetup.PlatformPowerLimit2Enable);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PsysPowerLimit1Power, CpuPowerDeliveryConfig->PsysPowerLimit1Power, (UINT16) (CpuSetup.PlatformPowerLimit1Power / 125));
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PsysPowerLimit1Time, CpuPowerDeliveryConfig->PsysPowerLimit1Time ,  CpuSetup.PlatformPowerLimit1Time                  );
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PsysPowerLimit2Power, CpuPowerDeliveryConfig->PsysPowerLimit2Power,  (UINT16) (CpuSetup.PlatformPowerLimit2Power / 125));
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PlatformAtxTelemetryUnit, CpuPowerDeliveryConfig->PlatformAtxTelemetryUnit, CpuSetup.PlatformAtxTelemetryUnit);
  //
  // Isys parameters
  //
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ThETAIbattEnable, CpuPowerDeliveryConfig->ThETAIbattEnable, CpuSetup.ThETAIbattEnable);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IsysCurrentLimitL1, CpuPowerDeliveryConfig->IsysCurrentLimitL1, CpuSetup.IsysCurrentLimitL1);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IsysCurrentLimitL1Enable, CpuPowerDeliveryConfig->IsysCurrentLimitL1Enable, CpuSetup.IsysCurrentLimitL1Enable);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IsysCurrentL1Tau, CpuPowerDeliveryConfig->IsysCurrentL1Tau, CpuSetup.IsysCurrentL1Tau);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IsysCurrentLimitL2, CpuPowerDeliveryConfig->IsysCurrentLimitL2, CpuSetup.IsysCurrentLimitL2);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IsysCurrentLimitL2Enable, CpuPowerDeliveryConfig->IsysCurrentLimitL2Enable, CpuSetup.IsysCurrentLimitL2Enable);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VsysMax, CpuPowerDeliveryConfig->VsysMax, CpuSetup.VsysMax);

  //
  // Check If power state is AC or DC.
  //
  PowerSourceStatus = EcGetPowerSource (&PowerSourceType);
  if (!EFI_ERROR (PowerSourceStatus)) {
    //
    // Power Source Type: BIT0    - AC
    //                    BIT1    - USB Type-C PD
    //                    BIT2    - USB PSS
    //                    Others  - Other usage
    //
    if ((PowerSourceType & (BIT0|BIT1|BIT2)) != 0) {
      DEBUG ((DEBUG_INFO, "AC/PD/PSS Adaptor plugin!\n"));
      IsAcPluggedIn = TRUE;
    } else {
      DEBUG ((DEBUG_INFO, "Power Source is unpluged!\n"));
      IsAcPluggedIn = FALSE;
    }
  }

  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD*) FspmUpd)->FspmConfig.AcDcPowerState, CpuPowerDeliveryConfig->AcDcPowerState, IsAcPluggedIn);

  //
  // Skin Temperature Control MMIO registers
  //
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD*) FspmUpd)->FspmConfig.SkinTempControl, CpuPowerDeliveryConfig->SkinTempControl, CpuSetup.SkinTempControl);

  if (CpuSetup.SkinTempControl) {
    for (Index = 0; Index < SKIN_TEMP_CONTROL_SENSOR; Index++) {
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD*) FspmUpd)->FspmConfig.SkinTargetTemp[Index], CpuPowerDeliveryConfig->SkinTargetTemp[Index],           CpuSetup.SkinTargetTemp[Index],          Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD*) FspmUpd)->FspmConfig.SkinTempControlEnable[Index], CpuPowerDeliveryConfig->SkinTempControlEnable[Index],    CpuSetup.SkinTempControlEnable[Index],   Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD*) FspmUpd)->FspmConfig.SkinControlLoopGain[Index], CpuPowerDeliveryConfig->SkinControlLoopGain[Index],      CpuSetup.SkinControlLoopGain[Index],     Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD*) FspmUpd)->FspmConfig.SkinTempOverrideEnable[Index], CpuPowerDeliveryConfig->SkinTempOverrideEnable[Index],   CpuSetup.SkinTempOverrideEnable[Index],  Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD*) FspmUpd)->FspmConfig.SkinMinPerformanceLevel[Index], CpuPowerDeliveryConfig->SkinMinPerformanceLevel[Index],  CpuSetup.SkinMinPerformanceLevel[Index], Index);
      COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD*) FspmUpd)->FspmConfig.SkinTempOverride[Index], CpuPowerDeliveryConfig->SkinTempOverride[Index],         CpuSetup.SkinTempOverride[Index],        Index);
    }
  }

  //
  // Applies TDP to non-cTDP or cTDP
  //
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD*) FspmUpd)->FspmConfig.ApplyConfigTdp, CpuPowerDeliveryConfig->ApplyConfigTdp, CpuSetup.ApplyConfigTdp);

  //
  // Dual Tau Boost
  //
  if (PcdGetBool (VpdPcdIpfEnable)) {
    UPDATE_POLICY (((FSPM_UPD*) FspmUpd)->FspmConfig.DualTauBoost, CpuPowerDeliveryConfig->DualTauBoost, 0);
  } else {
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD*) FspmUpd)->FspmConfig.DualTauBoost, CpuPowerDeliveryConfig->DualTauBoost, CpuSetup.DualTauBoost);
  }

  ///
  /// Set PcdCpuMaxLogicalProcessorNumber to max number of logical processors enabled
  /// Read MSR_CORE_THREAD_COUNT (0x35) to check the total active Threads
  ///
  MsrCoreThreadCount.Uint64 = AsmReadMsr64 (MSR_CORE_THREAD_COUNT);
  MaxLogicProcessors = (UINT32) MsrCoreThreadCount.Bits.Threadcount;
  DEBUG ((DEBUG_INFO, "MaxLogicProcessors = %d\n", MaxLogicProcessors));

#if FixedPcdGet8(PcdFspModeSelection) == 1
  PcdSet32S (PcdCpuMaxLogicalProcessorNumber, MaxLogicProcessors);
  PcdSet32S (PcdCpuProcTraceMemSize, CpuSetup.ProcessorTraceMemSize);
#else
  PcdSetEx32S (&gUefiCpuPkgTokenSpaceGuid, PcdCpuMaxLogicalProcessorNumber, MaxLogicProcessors);
  //
  // Point PCD to microcode location in flash.
  // CpuMp PEIM will shadow the microcode to memory.
  //
  PcdSetEx64S (
    &gUefiCpuPkgTokenSpaceGuid, PcdCpuMicrocodePatchAddress,
    FixedPcdGet32 (PcdFlashFvMicrocodeBase) + FixedPcdGet32 (PcdMicrocodeOffsetInFv)
    );
  PcdSetEx64S (
    &gUefiCpuPkgTokenSpaceGuid, PcdCpuMicrocodePatchRegionSize,
    FixedPcdGet32 (PcdFlashFvMicrocodeSize) - FixedPcdGet32 (PcdMicrocodeOffsetInFv)
    );
#endif

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    if ((BootMode == BOOT_IN_RECOVERY_MODE || BootMode == BOOT_WITH_DEFAULT_SETTINGS)) {  /// First Boot
      //
      // If it is frist boot, change the default value of FastPkgCRampDisable to 1 for some SKU.
      //
      CpuDid = (UINT16) GetHostBridgeRegisterData (HostBridgeDeviceId, HostBridgeDeviceIdData);
      switch (CpuDid) {
        case 0xB000:
        case 0xB002:
        case 0xB005:
        case 0xB003:
        case 0xB008:
        case 0xB00B:
        case 0xB00C:
        case 0xB014:
        case 0xB015:
        case 0xB016:
        case 0xB017:
        case 0xB01A:
          COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.AcousticNoiseMitigation, CpuPowerMgmtVrConfig->AcousticNoiseMitigation, 1);
          COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.FastPkgCRampDisable[1], CpuPowerMgmtVrConfig->FastPkgCRampDisable[1], 1, 1);
      }
      switch (CpuDid) {
        case 0xB003:
        case 0xB000:
          COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.EnableFastVmode[0], CpuPowerMgmtVrConfig->EnableFastVmode[0],0, 0);
      }
 
    }
  }
  return EFI_SUCCESS;
}
