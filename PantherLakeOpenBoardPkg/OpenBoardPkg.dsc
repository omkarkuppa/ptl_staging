## @file
#  The main build description file for the PantherLake board.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2023 Intel Corporation.
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
[Defines]
  DEFINE      PLATFORM_PACKAGE          = MinPlatformPkg
  DEFINE      PLATFORM_SI_PACKAGE       = OneSiliconPkg
  DEFINE      SILICON_PRODUCT_PATH      = OneSiliconPkg/Product/PantherLake
  DEFINE      PLATFORM_FSP_BIN_PACKAGE  = PantherLakeFspBinPkg
  DEFINE      PLATFORM_BOARD_PACKAGE    = PantherLakeOpenBoardPkg
  DEFINE      PLATFORM_BIN_PACKAGE      = PantherLakeBinPkg
  DEFINE      BOARD_PTL_BOARDS          = PantherLakeBoards
  DEFINE      PLATFORM_BSP_PATH         = $(PLATFORM_BOARD_PACKAGE)/BoardSupport
  DEFINE      PROJECT_PTL_BOARDS_BSP    = $(PLATFORM_BOARD_PACKAGE)/BoardSupport/$(BOARD_PTL_BOARDS)

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                         = $(PLATFORM_BOARD_PACKAGE)
  PLATFORM_GUID                         = 465B0A0B-7AC1-443b-8F67-7B8DEC145F90
  PLATFORM_VERSION                      = 0.1
  DSC_SPECIFICATION                     = 0x00010005
  OUTPUT_DIRECTORY                      = Build/$(PLATFORM_BOARD_PACKAGE)
  SUPPORTED_ARCHITECTURES               = $(FSP_ARCH)|X64
  BUILD_TARGETS                         = DEBUG|RELEASE
  SKUID_IDENTIFIER                      = ALL

  FLASH_DEFINITION                      = $(PLATFORM_BOARD_PACKAGE)/OpenBoardPkg.fdf
  VPD_TOOL_GUID                         = 8C3D856A-9BE6-468E-850A-24F7A8D38E08
  FIX_LOAD_TOP_MEMORY_ADDRESS           = 0x0
  DEFINE   TOP_MEMORY_ADDRESS           = 0x0

  #
  # Default value for BoardPkg.fdf use
  #
  DEFINE BIOS_SIZE_OPTION = SIZE_140

  PCD_DYNAMIC_AS_DYNAMICEX = TRUE

  #
  # VPD PCD initialization
  #
  !include $(BOARD_PTL_BOARDS)/BoardVpdPcdsInit/OpenBoardVpdPcdInit.dsc


  #
  # Include PCD configuration for this board.
  #
  !include OpenBoardPkgPcdInit.dsc


################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this board.
#
################################################################################
[SkuIds]
  0|DEFAULT              # 0|DEFAULT is reserved and always required.
  0x00000031|SkuIdPtlSimics


################################################################################
#
# Includes section - other DSC file contents included for this board build.
#
################################################################################


#######################################
# Library Includes
#######################################
!include $(PLATFORM_PACKAGE)/Include/Dsc/CoreCommonLib.dsc
!include $(PLATFORM_PACKAGE)/Include/Dsc/CorePeiLib.dsc
!include $(PLATFORM_PACKAGE)/Include/Dsc/CoreDxeLib.dsc
!include $(PLATFORM_BIN_PACKAGE)/Include/Dsc/BinCommonLib.dsc
!include $(PLATFORM_BIN_PACKAGE)/Include/Dsc/BinPeiLib.dsc
!include $(PLATFORM_BIN_PACKAGE)/Include/Dsc/BinDxeLib.dsc
!include $(PLATFORM_BOARD_PACKAGE)/Include/Dsc/SiPkgPeiLib.dsc

[LibraryClasses.common]
#
# Silicon Init Package
#
!include $(PLATFORM_BOARD_PACKAGE)/Include/Dsc/SiPkgCommonLib.dsc

  PlatformHookLib|$(PLATFORM_BOARD_PACKAGE)/Library/BasePlatformHookLib/BasePlatformHookLib.inf
  FspCommonLib|IntelFsp2Pkg/Library/BaseFspCommonLib/BaseFspCommonLib.inf
  FspSwitchStackLib|IntelFsp2Pkg/Library/BaseFspSwitchStackLib/BaseFspSwitchStackLib.inf

  PlatformSecLib|$(PLATFORM_BOARD_PACKAGE)/FspWrapper/Library/SecFspWrapperPlatformSecLib/SecFspWrapperPlatformSecLib.inf

  FspWrapperApiLib|IntelFsp2WrapperPkg/Library/BaseFspWrapperApiLib/BaseFspWrapperApiLib.inf
  FspWrapperApiTestLib|IntelFsp2WrapperPkg/Library/PeiFspWrapperApiTestLib/PeiFspWrapperApiTestLib.inf

  ConfigBlockLib|IntelSiliconPkg/Library/BaseConfigBlockLib/BaseConfigBlockLib.inf

  BoardInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/BoardInitLibNull/BoardInitLibNull.inf
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLibNull/TestPointCheckLibNull.inf

  PciLib|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf
  ReportCpuHobLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/ReportCpuHobLib/ReportCpuHobLib.inf
  PciSegmentInfoLib|$(PLATFORM_PACKAGE)/Pci/Library/PciSegmentInfoLibSimple/PciSegmentInfoLibSimple.inf
  PciHostBridgeLib|$(PLATFORM_PACKAGE)/Pci/Library/PciHostBridgeLibSimple/PciHostBridgeLibSimple.inf
  PlatformBootManagerLib|$(PLATFORM_PACKAGE)/Bds/Library/DxePlatformBootManagerLib/DxePlatformBootManagerLib.inf

  #
  # CPU
  #
  CpuPageTableLib|UefiCpuPkg/Library/CpuPageTableLib/CpuPageTableLib.inf
  FixedPointLib|$(PLATFORM_SI_PACKAGE)/Library/FixedPointLib/FixedPointLib.inf

  #
  # Shell
  #
  ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
  HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
  BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf

  #
  # S3
  #
  S3PciSegmentLib|MdePkg/Library/BaseS3PciSegmentLib/BaseS3PciSegmentLib.inf
  S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf

  TestPointLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointLib/PeiTestPointLib.inf

  SiPolicyLib|$(PLATFORM_BOARD_PACKAGE)/Library/PeiSiPolicyLib/PeiSiPolicyLib.inf

[LibraryClasses.IA32.SEC, LibraryClasses.X64.SEC]
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/SecTestPointCheckLib.inf
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLibNull/TestPointCheckLibNull.inf
  SecBoardInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/SecBoardInitLibNull/SecBoardInitLibNull.inf

[LibraryClasses.IA32.PEIM, LibraryClasses.X64.PEIM]
  #
  # SmmAccess
  #
  SmmAccessLib|IntelSiliconPkg/Feature/SmmAccess/Library/PeiSmmAccessLib/PeiSmmAccessLib.inf
  SetCacheMtrrLib|$(PLATFORM_PACKAGE)/Library/SetCacheMtrrLib/SetCacheMtrrLibNull.inf
  MultiBoardInitSupportLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/MultiBoardInitSupportLib/PeiMultiBoardInitSupportLib.inf
  BoardInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/MultiBoardInitSupportLib/PeiMultiBoardInitSupportLib.inf
  SiliconPolicyUpdateLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/SiliconPolicyUpdateLibNull/SiliconPolicyUpdateLibNull.inf

  PeiLib|$(PLATFORM_PACKAGE)/Library/PeiLib/PeiLib.inf
  ReportFvLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/PeiReportFvLib/PeiReportFvLib.inf

  FspWrapperApiTestLib|IntelFsp2WrapperPkg/Library/PeiFspWrapperApiTestLib/PeiFspWrapperApiTestLib.inf
  FspWrapperHobProcessLib|$(PLATFORM_PACKAGE)/FspWrapper/Library/PeiFspWrapperHobProcessLib/PeiFspWrapperHobProcessLib.inf

!if $(TARGET) == DEBUG
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/PeiTestPointCheckLib.inf
!endif
  GpioV2WrapperLib|$(PLATFORM_BOARD_PACKAGE)/Library/PeiDxeGpioV2WrapperLib/PeiDxeGpioV2WrapperLib.inf

[LibraryClasses.IA32.SEC, LibraryClasses.IA32.PEIM, LibraryClasses.X64.SEC, LibraryClasses.X64.PEIM]
  FspWrapperPlatformLib|$(PLATFORM_PACKAGE)/FspWrapper/Library/PeiFspWrapperPlatformLib/PeiFspWrapperPlatformLib.inf
  SiliconPolicyUpdateLib|$(PLATFORM_BOARD_PACKAGE)/BoardSupport/Policy/Library/PeiSiliconPolicyUpdateLib/PeiSiliconPolicyUpdateLib.inf
  PeiPolicyBoardConfigLib|$(PLATFORM_BOARD_PACKAGE)/BoardSupport/Library/PeiPolicyBoardConfigLib/PeiPolicyBoardConfigLib.inf
  PeiPolicyAdvancedConfigLib|$(PLATFORM_BOARD_PACKAGE)/BoardSupport/Library/PeiPolicyAdvancedConfigLibNull/PeiPolicyAdvancedConfigLibNull.inf

[LibraryClasses.X64.DXE_DRIVER]
  FspWrapperPlatformLib|$(PLATFORM_PACKAGE)/FspWrapper/Library/DxeFspWrapperPlatformLib/DxeFspWrapperPlatformLib.inf
!if $(TARGET) == DEBUG
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/DxeTestPointCheckLib.inf
!endif
  #
  # DXE phase common
  #
  TestPointLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointLib/DxeTestPointLib.inf
  MultiBoardInitSupportLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/MultiBoardInitSupportLib/DxeMultiBoardInitSupportLib.inf
  BoardInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/MultiBoardInitSupportLib/DxeMultiBoardInitSupportLib.inf
  MultiBoardAcpiSupportLib|$(PLATFORM_PACKAGE)/Acpi/Library/MultiBoardAcpiSupportLib/DxeMultiBoardAcpiSupportLib.inf
  BoardAcpiTableLib|$(PLATFORM_PACKAGE)/Acpi/Library/MultiBoardAcpiSupportLib/DxeMultiBoardAcpiSupportLib.inf
  AslUpdateLib|$(PLATFORM_PACKAGE)/Acpi/Library/DxeAslUpdateLib/DxeAslUpdateLib.inf

  SiliconPolicyInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/SiliconPolicyInitLibNull/SiliconPolicyInitLibNull.inf
  SiliconPolicyUpdateLib|$(PLATFORM_BOARD_PACKAGE)/Policy/Library/DxeSiliconPolicyUpdateLib/DxeSiliconPolicyUpdateLib.inf
  BoardBdsHookLib|BoardModulePkg/Library/BoardBdsHookLib/BoardBdsHookLib.inf
  BoardBootManagerLib|BoardModulePkg/Library/BoardBootManagerLib/BoardBootManagerLib.inf

  #
  # Silicon Init Package
  #
  !include $(PLATFORM_BOARD_PACKAGE)/Include/Dsc/SiPkgDxeLib.dsc

[LibraryClasses.X64.DXE_SMM_DRIVER]
  SpiFlashCommonLib|$(PLATFORM_BOARD_PACKAGE)/Library/SmmSpiFlashCommonLib/SmmSpiFlashCommonLib.inf
!if $(TARGET) == DEBUG
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/SmmTestPointCheckLib.inf
!endif
  BoardAcpiEnableLib|$(PLATFORM_PACKAGE)/Acpi/Library/MultiBoardAcpiSupportLib/SmmMultiBoardAcpiSupportLib.inf
  MultiBoardAcpiSupportLib|$(PLATFORM_PACKAGE)/Acpi/Library/MultiBoardAcpiSupportLib/SmmMultiBoardAcpiSupportLib.inf
  TestPointLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointLib/SmmTestPointLib.inf


[Components.$(FSP_ARCH)]

  #
  # Common
  #
  !include $(PLATFORM_PACKAGE)/Include/Dsc/CorePeiInclude.dsc
  #
  # FSP wrapper SEC Core
  #
!if $(FSP64_BUILD) == TRUE
  UefiCpuPkg/SecCore/SecCoreNative.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
      SiliconPolicyInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/SiliconPolicyInitLibNull/SiliconPolicyInitLibNull.inf
      SiliconPolicyUpdateLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/SiliconPolicyUpdateLibNull/SiliconPolicyUpdateLibNull.inf
  }
  UefiCpuPkg/ResetVector/Vtf0/Vtf0.inf {
    <BuildOptions>
      *_*_X64_NASMB_FLAGS = -DPAGE_TABLE_1G -DARCH_X64
  }
!else
  UefiCpuPkg/SecCore/SecCore.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  }
!endif
  #
  # Platform
  #
  $(PLATFORM_PACKAGE)/PlatformInit/ReportFv/ReportFvPei.inf
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitPei/PlatformInitPreMem.inf {
    <LibraryClasses>
      NULL|$(PROJECT_PTL_BOARDS_BSP)/Library/BoardInitLib/Pei/PeiMultiBoardInitPreMemLib.inf
  }
  IntelFsp2WrapperPkg/FspmWrapperPeim/FspmWrapperPeim.inf {
    <LibraryClasses>
!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 0
      SiliconPolicyInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/SiliconPolicyInitLibNull/SiliconPolicyInitLibNull.inf
      SiliconPolicyUpdateLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/SiliconPolicyUpdateLibNull/SiliconPolicyUpdateLibNull.inf
!endif
  }

  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitPei/PlatformInitPostMem.inf {
    <LibraryClasses>
      NULL|$(PROJECT_PTL_BOARDS_BSP)/Library/BoardInitLib/Pei/PeiMultiBoardInitPostMemLib.inf
  }
!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 0
  $(PLATFORM_PACKAGE)/PlatformInit/SiliconPolicyPei/SiliconPolicyPeiPostMem.inf  {
    <LibraryClasses>
      SiliconPolicyInitLib|$(PLATFORM_BOARD_PACKAGE)/Library/PeiPostMemSiliconPolicyInitLib/PeiPostMemSiliconPolicyInitLib.inf
  }
  $(PLATFORM_PACKAGE)/PlatformInit/SiliconPolicyPei/SiliconPolicyPeiPreMem.inf {
    <LibraryClasses>
      SiliconPolicyInitLib|$(PLATFORM_BOARD_PACKAGE)/Library/PeiPreMemSiliconPolicyInitLib/PeiPreMemSiliconPolicyInitLib.inf
      #
      # In FSP Dispatch mode below dummy library should be linked to bootloader PEIM
      # to build all DynamicEx PCDs that FSP consumes into bootloader PCD database.
      #
      NULL|$(PLATFORM_FSP_BIN_PACKAGE)/Library/FspPcdListLib/FspPcdListLibNull.inf
  }
!endif
  $(PLATFORM_BSP_PATH)/BiosInfo/BiosInfo.inf
  $(PLATFORM_PACKAGE)/Services/StallServicePei/StallServicePei.inf


  IntelFsp2WrapperPkg/FspsWrapperPeim/FspsWrapperPeim.inf {
    <LibraryClasses>
!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 0
      SiliconPolicyInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/SiliconPolicyInitLibNull/SiliconPolicyInitLibNull.inf
      SiliconPolicyUpdateLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/SiliconPolicyUpdateLibNull/SiliconPolicyUpdateLibNull.inf
!endif
  }

  #
  # Security
  #
!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
  $(PLATFORM_PACKAGE)/Tcg/Tcg2PlatformPei/Tcg2PlatformPei.inf
!endif
 MdeModulePkg/Universal/ResetSystemPei/ResetSystemPei.inf

  #
  # UPL Wrapper
  #
  UefiPayloadPkg/PayloadLoaderPeim/PayloadLoaderPeim.inf {
    <LibraryClasses>
      NULL|$(PLATFORM_BOARD_PACKAGE)/Upl/Library/PeiPayloadHookLib/PeiPayloadHookLib.inf
  }

[Components.X64]

  #
  # Common
  #
  !include $(PLATFORM_BOARD_PACKAGE)/Include/Dsc/CoreDxeInclude.dsc


  BoardModulePkg/BoardBdsHookDxe/BoardBdsHookDxe.inf

  #
  # Silicon
  #
  !include $(PLATFORM_BOARD_PACKAGE)/Include/Dsc/SiPkgDxe.dsc

  #
  # SmmAccess
  #
  IntelSiliconPkg/Feature/SmmAccess/SmmAccessDxe/SmmAccess.inf

  #
  # Platform
  #
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitDxe/PlatformInitDxe.inf
!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  IntelFsp2WrapperPkg/FspWrapperNotifyDxe/FspWrapperNotifyDxe.inf
!endif
  $(PLATFORM_PACKAGE)/FspWrapper/SaveMemoryConfig/SaveMemoryConfig.inf

  $(PLATFORM_PACKAGE)/Test/TestPointStubDxe/TestPointStubDxe.inf
  $(PLATFORM_PACKAGE)/Test/TestPointDumpApp/TestPointDumpApp.inf

  #
  # OS Boot
  #
!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE
  $(PLATFORM_PACKAGE)/Acpi/AcpiTables/AcpiPlatform.inf
  $(PLATFORM_PACKAGE)/Acpi/MinDsdt/MinDsdt.inf
  $(PLATFORM_PACKAGE)/Acpi/AcpiSmm/AcpiSmm.inf
  IntelSiliconPkg/Feature/Flash/SpiFvbService/SpiFvbServiceSmm.inf
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitSmm/PlatformInitSmm.inf

  UefiCpuPkg/PiSmmCpuDxeSmm/PiSmmCpuDxeSmm.inf {
    <LibraryClasses>
      SmmCpuFeaturesLib|UefiCpuPkg/Library/SmmCpuFeaturesLib/SmmCpuFeaturesLib.inf
      MmSaveStateLib|UefiCpuPkg/Library/MmSaveStateLib/IntelMmSaveStateLib.inf
      SmmCpuSyncLib|UefiCpuPkg/Library/SmmCpuSyncLib/SmmCpuSyncLib.inf
  }
!endif


  #
  # Security
  #
  $(PLATFORM_PACKAGE)/Hsti/HstiIbvPlatformDxe/HstiIbvPlatformDxe.inf
!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
  $(PLATFORM_PACKAGE)/Tcg/Tcg2PlatformDxe/Tcg2PlatformDxe.inf
!endif

  #
  # This includes W/A for QemuVideoDxe driver otherwise Simics no screen
  #
  $(PLATFORM_BOARD_PACKAGE)/PciPlatform/Dxe/PciPlatform.inf

  !include $(PLATFORM_BOARD_PACKAGE)/Include/Dsc/SiPkgBuildOption.dsc
  !include OpenBoardPkgBuildOption.dsc
