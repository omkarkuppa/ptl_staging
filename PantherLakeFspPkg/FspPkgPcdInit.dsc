## @file
#  FSP description file initializes configuration (PCD) settings for this project.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2020 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.
#
# @par Specification
##

#
# TRUE is ENABLE. FALSE is DISABLE.
#

#
# BIOS build switches configuration
#
[PcdsFixedAtBuild]
  #
  # Enable FV migration as default
  # Platform should build gEdkiiMigrationInfoGuid HOB to indicate which FVs
  # should be migrated
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdMigrateTemporaryRamFirmwareVolumes|TRUE
  #
  # CPU
  #
  gSiPkgTokenSpaceGuid.PcdTxtEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdOverclockEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdSpecialPoolEnable|FALSE
  #
  # SA
  #
  gSiPkgTokenSpaceGuid.PcdDtprEnable|TRUE
  #
  # ME
  #
  gSiPkgTokenSpaceGuid.PcdPttEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdAmtEnable|TRUE
  #
  # SI
  #
  gSiPkgTokenSpaceGuid.PcdTseEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdTdxEnable|FALSE # TDX is not supported, PCD should be set to FALSE
  gSiPkgTokenSpaceGuid.PcdFspBinaryEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdTraceHubCatalogEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdFspImageIdString|0x245053464C545024  #$PTLFSP$
  gSiPkgTokenSpaceGuid.PcdStatusCodeFlags|0x32

  gPantherLakeFspPkgTokenSpaceGuid.PcdMiniBiosEnable|FALSE
  gPantherLakeFspPkgTokenSpaceGuid.PcdCfgRebuild|FALSE
  gPantherLakeFspPkgTokenSpaceGuid.PcdFspPerformanceEnable|FALSE
  gPantherLakeFspPkgTokenSpaceGuid.PcdMonoStatusCode|FALSE
  gPantherLakeFspPkgTokenSpaceGuid.PcdAcpiInFspEnabled|FALSE
  #
  # Symbol in release build
  #
  gPantherLakeFspPkgTokenSpaceGuid.PcdSymbolInReleaseEnable|FALSE

!if $(TARGET) == RELEASE
!if gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x02
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0
!endif
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x27
!endif

  gIntelFsp2PkgTokenSpaceGuid.PcdTemporaryRamBase|0xFEF00000
  gIntelFsp2PkgTokenSpaceGuid.PcdTemporaryRamSize|0x000FF000
  gSiPkgTokenSpaceGuid.PcdSecondaryDataStackBase|0xFA000000
  gSiPkgTokenSpaceGuid.PcdSecondaryDataStackSize|0x00000000
gIntelFsp2PkgTokenSpaceGuid.PcdFspReservedMemoryLength|0x00500000
  gIntelFsp2PkgTokenSpaceGuid.PcdFspTemporaryRamSize|0x00020000
  ## Specifies the FSP Header Spec Version
  gIntelFsp2PkgTokenSpaceGuid.PcdFspHeaderSpecVersion|0x24
  # This defines how much space will be used for heap in FSP temporary memory
  # x % of FSP temporary memory will be used for heap
  # (100 - x) % of FSP temporary memory will be used for stack
  # 0 means FSP will share the same stack with bootloader
  #   In this case PcdFspTemporaryRamSize is used for Heap
  gIntelFsp2PkgTokenSpaceGuid.PcdFspHeapSizePercentage|0
  # This is a platform specific global pointer used by FSP
  gIntelFsp2PkgTokenSpaceGuid.PcdGlobalDataPointerAddress|0xFED00148
  # Override PcdFspMaxPatchEntry to match FspHeader.aslc
  gIntelFsp2PkgTokenSpaceGuid.PcdFspMaxPatchEntry|0x02

  ## Indicates if to shadow PEIM on S3 boot path after memory is ready.<BR><BR>
  #   TRUE  - Shadow PEIM on S3 boot path after memory is ready.<BR>
  #   FALSE - Not shadow PEIM on S3 boot path after memory is ready.<BR>
  # @Prompt Shadow Peim On S3 Boot.
  gEfiMdeModulePkgTokenSpaceGuid.PcdShadowPeimOnS3Boot|TRUE
  # Temp solution to avoid halt in PeiVariable->GetVariable (PeiGetVariable)
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableBase|0xFFF80000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableSize|0x10000
  # Use to override gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxPeiStackSize
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxPeiStackSize|0x80000

  ## Specifies the number of variable MTRRs reserved for OS use. The default number of
  #  MTRRs reserved for OS use is 0.
  # @Prompt Number of reserved variable MTRRs.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuNumberOfReservedVariableMtrrs|0x0
  ## Specifies timeout value in microseconds for the BSP to detect all APs for the first time.
  # @Prompt Timeout for the BSP to detect all APs for the first time.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApInitTimeOutInMicroSeconds|10000

  ## Specifies if CPU features will be initialized during S3 resume.
  # @Prompt If CPU features will be initialized during S3 resume.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuFeaturesInitOnS3Resume|TRUE

  #
  # Trace Hub Debug setting
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdTraceHubDebugLevel|0x3
  gEfiMdeModulePkgTokenSpaceGuid.PcdEnableTraceHubDebugMsg|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdTraceHubDebugMmioAddress|0xFE270380
  gSiPkgTokenSpaceGuid.PcdSafBarBaseAddress|0xFA000000


[PcdsPatchableInModule]
  gSiPkgTokenSpaceGuid.PcdRegBarBaseAddress|0xF0000000
  #
  # This entry will be patched during the build process
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdVpdBaseAddress|0x12345678

[PcdsDynamicExDefault]
  #
  # Include FSP PCD settings.
  #
  !include $(FSP_PACKAGE)/FspPkgPcdShare.dsc

  gEfiMdeModulePkgTokenSpaceGuid.PcdS3BootScriptTablePrivateDataPtr|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdS3BootScriptTablePrivateSmmDataPtr|0x0
