## @file
#  This is the overall description file for the BoardPkg. This includes other scope-specific
#  description files such PCD scope or build options scope to help build the overall Board
#  support for the project.
#
#  Adding list of all Library Classes and Modules/Drivers needed to build this Platform.
#  For Example- Information file for BoardConfigLib.
#  BoardConfigLib|$(PLATFORM_BSP_PATH)/Library/PeiBoardConfigLib/PeiBoardConfigLib.inf
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
[Defines]
  #
  # Set platform specific package/folder name, same as passed from PREBUILD script.
  # PLATFORM_PACKAGE would be the same as PLATFORM_NAME as well as package build folder
  # DEFINE only takes effect at R9 DSC and FDF.
  #
  DEFINE      PLATFORM_PACKAGE                = MinPlatformPkg
  DEFINE      PLATFORM_FULL_PACKAGE           = PantherLakePlatSamplePkg
  DEFINE      PLATFORM_SI_PACKAGE             = OneSiliconPkg
  DEFINE      SILICON_PRODUCT_PATH            = OneSiliconPkg/Product/PantherLake
  DEFINE      PLATFORM_FSP_BIN_PACKAGE        = PantherLakeFspBinPkg
  DEFINE      PLATFORM_BOARD_PACKAGE          = PantherLakeBoardPkg
  DEFINE      PLATFORM_OPEN_BOARD_PACKAGE     = PantherLakeOpenBoardPkg
  DEFINE      PLATFORM_FEATURES_PATH          = $(PLATFORM_FULL_PACKAGE)/Features
  DEFINE      PLATFORM_BIN_PACKAGE            = PantherLakeBinPkg
  DEFINE      COMMON_BIN_PACKAGE              = CommonBinPkg
  DEFINE      PLATFORM_BSP_PATH               = PantherLakeBoardPkg/BoardSupport

#
# Define ESRT GUIDs for Firmware Management Protocol instances
#
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_TSN_MAC_ADDR  = 6FEE88FF-49ED-48F1-B77B-EAD15771ABE7 # gFmpDevicePlatformTsnMacAddrGuid
#
# Defines Boards paths
#
  DEFINE      BOARD_PTLUH_BOARDS              = PantherLakeBoards/PantherLakeMobileBoards
  DEFINE      PROJECT_PTLUH_BOARDS            = PantherLakeBoardPkg/$(BOARD_PTLUH_BOARDS)


  #
  # Silicon On/Off feature are defined here
  #
  !include $(SILICON_PRODUCT_PATH)/SiPkgPcdInit.dsc
  #
  # Platform On/Off features are defined here
  #
  !include $(COMMON_BIN_PACKAGE)/Include/Dsc/CommonBinPkgPcdInit.dsc
  !include $(PLATFORM_BIN_PACKAGE)/Include/Dsc/PantherLakeBinPkgPcdInit.dsc
  !include BoardPkgPcdInit.dsc
  !include BoardPkgPcdUpdate.dsc

  !include $(PLATFORM_BOARD_PACKAGE)/Include/Dsc/HiiStructurePcd.dsc

  !include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/BoardVpdPcdInit.dsc

!if gI2cFeaturePkgTokenSpaceGuid.PcdI2cTouchCommonEnable == TRUE
  !include $(PLATFORM_BOARD_PACKAGE)/VpdI2cRpTable.dsc
!endif

  #
  # @Todo: Need to check if the copies from PantherLakeSvRestrictedPkg are required or not
################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                       = $(PLATFORM_BOARD_PACKAGE)
  PLATFORM_GUID                       = EB89E595-7D9D-4422-A277-A50B5AFD3E16
  PLATFORM_VERSION                    = 0.1
  DSC_SPECIFICATION                   = 0x00010005
  OUTPUT_DIRECTORY                    = Build/$(PLATFORM_BOARD_PACKAGE)
  SUPPORTED_ARCHITECTURES             = $(FSP_ARCH)|X64
  BUILD_TARGETS                       = DEBUG|RELEASE
  SKUID_IDENTIFIER                    = ALL


  FLASH_DEFINITION                    = $(PLATFORM_BOARD_PACKAGE)/BoardPkg.fdf

  VPD_TOOL_GUID                       = 8C3D856A-9BE6-468E-850A-24F7A8D38E08
  FIX_LOAD_TOP_MEMORY_ADDRESS         = 0x0
  DEFINE TOP_MEMORY_ADDRESS           = 0x0

  #
  # Defines Boards paths
  #
  DEFINE      PROJECT_PTL_BOARDS_BSP        = PantherLakeBoardPkg/BoardSupport/$(BOARD_PTLUH_BOARDS)


  #
  # Default value for BoardPkg.fdf use
  #
  !if gPlatformModuleTokenSpaceGuid.PcdExtendedBiosRegionSupport == TRUE
    DEFINE BIOS_SIZE_OPTION = SIZE_170
  !else
    DEFINE BIOS_SIZE_OPTION = SIZE_140
  !endif

  PCD_DYNAMIC_AS_DYNAMICEX                  = TRUE

  #
  # Map Network Feature PCD to macro setting used in NetworkPkg/NetworkDefines.dsc.inc
  #
  !if gPlatformModuleTokenSpaceGuid.PcdNetworkEnable == TRUE
    DEFINE NETWORK_ENABLE                 = TRUE
  !else
    DEFINE NETWORK_ENABLE                 = FALSE
  !endif
  !if gPlatformModuleTokenSpaceGuid.PcdNetworkIp6Enable == TRUE
    DEFINE NETWORK_IP6_ENABLE             = TRUE
  !else
    DEFINE NETWORK_IP6_ENABLE             = FALSE
  !endif
  !if gPlatformModuleTokenSpaceGuid.PcdNetworkIscsiEnable == TRUE
    DEFINE NETWORK_ISCSI_ENABLE           = TRUE
  !else
    DEFINE NETWORK_ISCSI_ENABLE           = FALSE
  !endif
  !if gPlatformModuleTokenSpaceGuid.PcdNetworkVlanEnable == TRUE
    DEFINE NETWORK_VLAN_ENABLE            = TRUE
  !else
    DEFINE NETWORK_VLAN_ENABLE            = FALSE
  !endif
  DEFINE NETWORK_TLS_ENABLE               = TRUE
  DEFINE NETWORK_HTTP_BOOT_ENABLE         = TRUE
  DEFINE PLATFORMX64_ENABLE               = TRUE


################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this
#                              Platform.
#
################################################################################
[SkuIds]
  0|DEFAULT              # The entry: 0|DEFAULT is reserved and always required.
  0x00000031|SkuIdPtlSimics
  0x00010031|SkuIdPtlHfpgaBase|SkuIdPtlSimics
  0x00020031|SkuIdPtlHfpgaTcss|SkuIdPtlHfpgaBase
  0x00040031|SkuIdPtlHsle|SkuIdPtlSimics
  0x00050031|SkuIdPtlHsleNull|SkuIdPtlSimics
  0x00000009|SkuIdPtlUHHmpRvp
  0x00000001|SkuIdPtlUHLp5Rvp1
  0x00010001|SkuIdPtlUHLp5MemSdRvp1|SkuIdPtlUHLp5Rvp1
  0x00020001|SkuIdPtlUHLp5MemSktPnpRvp1|SkuIdPtlUHLp5Rvp1
  0x00030001|SkuIdPtlUHLp5MemSdEpsRvp1|SkuIdPtlUHLp5Rvp1
  0x00040001|SkuIdPtlUHLp5MemSktChromeRvp1|SkuIdPtlUHLp5Rvp1
  0x00050001|SkuIdPtlUHLp5MemSdChromeRvp1|SkuIdPtlUHLp5Rvp1
  0x00060001|SkuIdPtlUHLp5Rvp1Bom|SkuIdPtlUHLp5Rvp1
  0x00070001|SkuIdPtlUHLp5MemSdMeccRvp1|SkuIdPtlUHLp5Rvp1
  0x00000002|SkuIdPtlUHCammDTbTRvp2
  0x00010002|SkuIdPtlUHCammDTbTExtGfxRvp2|SkuIdPtlUHCammDTbTRvp2
  0x00060002|SkuIdPtlUHCammDTbTRvp2Bom|SkuIdPtlUHCammDTbTRvp2
  0x00000102|SkuIdPtlUHCammDTbTRvp2Rev1|SkuIdPtlUHCammDTbTRvp2
  0x00010102|SkuIdPtlUHCammDTbTExtGfxRvp2Rev1|SkuIdPtlUHCammDTbTExtGfxRvp2
  0x00060102|SkuIdPtlUHCammDTbTRvp2BomRev1|SkuIdPtlUHCammDTbTRvp2Bom
  0x00000003|SkuIdPtlUHLp5Rvp3
  0x00010003|SkuIdPtlUHLp5MemSdRvp3|SkuIdPtlUHLp5Rvp3
  0x00020003|SkuIdPtlUHLp5MemSktPnpRvp3|SkuIdPtlUHLp5Rvp3
  0x00050003|SkuIdPtlUHLp5Rvp3PpvBom|SkuIdPtlUHLp5Rvp3
  0x00060003|SkuIdPtlUHLp5Rvp3Bom|SkuIdPtlUHLp5Rvp3
  0x00000103|SkuIdPtlUHLp5Rvp3Rev1|SkuIdPtlUHLp5Rvp3
  0x00010103|SkuIdPtlUHLp5MemSdRvp3Rev1|SkuIdPtlUHLp5MemSdRvp3
  0x00020103|SkuIdPtlUHLp5MemSktPnpRvp3Rev1|SkuIdPtlUHLp5MemSktPnpRvp3
  0x00050103|SkuIdPtlUHLp5Rvp3PpvBomRev1|SkuIdPtlUHLp5Rvp3PpvBom
  0x00060103|SkuIdPtlUHLp5Rvp3BomRev1|SkuIdPtlUHLp5Rvp3Bom
  0x00000004|SkuIdPtlUHDdr5Rvp4
  0x00040004|SkuIdPtlUHDdr5Rvp4Chrome|SkuIdPtlUHDdr5Rvp4
  0x00050004|SkuIdPtlUHDdr5Rvp4PpvBom|SkuIdPtlUHDdr5Rvp4
  0x00060004|SkuIdPtlUHDdr5Rvp4Bom|SkuIdPtlUHDdr5Rvp4
  0x01000004|SkuIdPtlUHDdr5Rvp4Dp|SkuIdPtlUHDdr5Rvp4
  0x01040004|SkuIdPtlUHDdr5Rvp4DpChrome|SkuIdPtlUHDdr5Rvp4Chrome
  0x01050004|SkuIdPtlUHDdr5Rvp4DpPpvBom|SkuIdPtlUHDdr5Rvp4PpvBom
  0x01060004|SkuIdPtlUHDdr5Rvp4DpBom|SkuIdPtlUHDdr5Rvp4Bom
  0x00000005|SkuIdPtlUHLp5MemSktmRvp
  0x00000011|SkuIdPtlHLp5Gcs1
  0x00000012|SkuIdPtlHLp5Gcs2
  0x00000015|SkuIdPtlUHLp5Adk1
  0x00000016|SkuIdPtlUHLp5Adk2
  0x00010007|SkuIdPtlUHLp5AepBom
  0x00000007|SkuIdPtlUHLp5Aep|SkuIdPtlUHLp5AepBom
  0x00020007|SkuIdPtlUHLp5AepBom32Gb|SkuIdPtlUHLp5AepBom
  0x08000010|SkuIdWclNDdr5Rvp1
  0x0A000010|SkuIdWclNDdr5Rvp1HDMI|SkuIdWclNDdr5Rvp1

# Keeping this section as an example to define SKUs that can inherit parent
# properties and define a section by its own identifier as well.

################################################################################
#
# DefaultStores section - list of all default stores supported by this
#                              Platform.
#
################################################################################
[DefaultStores]
0|Standard
1|Manufacturing
2|Optimized


################################################################################
#
# Feature includes
# Features need to be included before platform specify it's own library classes
# so that platform choices are prioritized over feature libraries
#
################################################################################

# XmlCli: Include Feature based on Pcd Value
!if gXmlCliFeaturePkgTokenSpaceGuid.PcdXmlCliFeatureEnable == TRUE
  !include XmlCliFeaturePkg/Include/XmlCliFeature.dsc
!endif

!include NhltFeaturePkg/Include/NhltFeature.dsc
!include SndwDevTopologyFeaturePkg/Include/SndwDevTopologyFeature.dsc
!include DmarAcpiFeaturePkg/Include/DmarAcpiFeature.dsc
!include BeepPkg/Include/Beep.dsc
!include EcFeaturePkg/Include/EcFeatureLib.dsc
!include MultiIbbFeaturePkg/Include/MultiIbbFeature.dsc
!if gSiPkgTokenSpaceGuid.PcdAmtEnable == TRUE
  !include MebxFeaturePkg/Include/MebxFeature.dsc
  !include AmtPetAlertFeaturePkg/Include/AmtPetAlertFeature.dsc
  !include AmtMacPassThroughFeaturePkg/Include/AmtMacPassThroughFeature.dsc
  !include AmtPetInitFeaturePkg/Include/AmtPetInitFeature.dsc
!endif

!if gPsrFeatureModuleTokenSpaceGuid.PcdPlatformServiceRecordSupport == TRUE
  !include PsrFeaturePkg/Include/PsrFeature.dsc
!endif
!if gUfsFeaturePkgTokenSpaceGuid.PcdUfsFeatureEnable == TRUE
  !include UfsFeaturePkg/Include/UfsFeature.dsc
!endif
!if gSiPkgTokenSpaceGuid.PcdSmbiosEnable == TRUE
  !include ClientSmbiosFeaturePkg/Include/ClientSmbiosFeature.dsc
!endif
!if gCnvFeaturePkgTokenSpaceGuid.PcdCnvFeatureEnable == TRUE
  [Defines]
    DEFINE CNV_WIFI_FLAGS = -DRP_PATH1=PC00 -DRP_PATH2=RP -DEP_DEV_NAME=PXSX -DEP_ADR_OBJ_TYPE=MethodObj -DRP_ADR_OBJ_TYPE=MethodObj
    DEFINE CNV_USB_BT_FLAGS = -DUSB2_PATH1=PC00 -DUSB2_PATH2=XHCI -DUSB2_PATH3=RHUB -DUSB2_PATH4=HS
    DEFINE CNV_UART_BT_FLAGS = -DUART_PATH1=PC00 -DUART_PATH2=UA
    DEFINE CNV_ASLPP_FLAGS = $(CNV_WIFI_FLAGS) $(CNV_USB_BT_FLAGS) $(CNV_UART_BT_FLAGS)
  !include CnvFeaturePkg/Include/CnvFeature.dsc
!endif
!if gSiPkgTokenSpaceGuid.PcdDtprEnable == TRUE
  !include DtprFeaturePkg/Include/DtprFeature.dsc
!endif
!if gDptfFeaturePkgTokenSpaceGuid.PcdDptfFeatureEnable == TRUE
  !include DptfFeaturePkg/Include/DptfFeature.dsc
!endif

!if gVtioFeaturePkgTokenSpaceGuid.PcdVtioFeatureEnable == TRUE
      !include VtioFeaturePkg/Include/VtioFeature.dsc
!endif

!if gTseFeaturePkgTokenSpaceGuid.PcdTseFeatureEnable == TRUE
  !include TseFeaturePkg/Include/TseFeature.dsc
!endif

#
# TDX
#
!if gTdxFeaturePkgTokenSpaceGuid.PcdTdxFeatureEnable == TRUE
  !include TdxFeaturePkg/Include/TdxFeature.dsc
!endif
!if gCapsuleFeaturePkgTokenSpaceGuid.PcdCapsuleFeatureEnable == TRUE
  [Defines]
    DEFINE FMP_CLIENT_PLATFORM_NAME = PantherLake

    #
    # Set the CSME_GENERATION as 0 since not product yet.
    # This would allow the CSME capsule which signed sample test key.
    # (3K test keys from platform sample code.)
    #
    DEFINE CSME_GENERATION          = 0

  [PcdsFixedAtBuild]
    #
    # Certificates used to authenticate capsule update image
    #
    # Using PKCS#7 3K test keys from platform sample code. This cannot be used in product.
    !include CapsuleFeaturePkg/SigningKeys/TestRoot3K.inc

  !include CapsuleFeaturePkg/Include/CapsuleFeature.dsc
!endif

!if gTelemetryFeaturePkgTokenSpaceGuid.PcdTelemetryFeatureEnable == TRUE
  !include TelemetryFeaturePkg/Include/TelemetryFeature.dsc
!endif

[Defines]
  #
  # Define Platform dependent macro for using dTBT AFP
  #
  DEFINE DTBT_IOE_RP_FLAGS = -DIOE_RP_PC=PC00
  DEFINE DTBT_PCH_RP_FLAGS = -DPCH_RP_PC=PC00
  DEFINE DTBT_ASLPP_FLAGS = $(DTBT_IOE_RP_FLAGS) $(DTBT_PCH_RP_FLAGS)
!include Usb4FeaturePkg/Include/Usb4Feature.dsc

#
# TCSS USB Type C
#
!if gUcsiFeaturePkgTokenSpaceGuid.PcdUcsiFeatureEnable == TRUE
  !include UcsiFeaturePkg/Include/UcsiFeature.dsc
!endif

!if gUsbPortMapPkgTokenSpaceGuid.PcdUsbPortMapEnable == TRUE
  !include UsbPortMapPkg/Include/UsbPortMap.dsc
!endif

!if gUsbCCapsuleFeaturePkgTokenSpaceGuid.PcdUsbCCapsuleFeatureEnable == TRUE
  !include UsbCCapsuleFeaturePkg/Include/UsbCCapsuleFeature.dsc
!endif

!if gMipiCamFeaturePkgTokenSpaceGuid.PcdMipiCamFeatureEnable == TRUE
  !include MipiCamFeaturePkg/Include/MipiCamFeature.dsc
!endif
#
# Include FSP Wrapper Package Driver and Library Module
#
!if gFspWrapperFeaturePkgTokenSpaceGuid.PcdFspWrapperFeatureSupport == TRUE
  [Defines]
    DEFINE CHECK_FSP_ERROR_INFO_HOB_DXE_DEBUG_LIB = $(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
  !include FspWrapperFeaturePkg/Include/Dsc/FspWrapperFeature.dsc
!endif

!if gI2cFeaturePkgTokenSpaceGuid.PcdI2cFeatureEnable == TRUE
!include I2cFeaturePkg/Include/I2cFeature.dsc
  [Components.X64]
    I2cFeaturePkg/I2cPlatformDxe/I2cPlatformDxe.inf {
      <LibraryClasses>
      NULL|$(PLATFORM_BOARD_PACKAGE)/Library/I2cPlatformDxeHookLib/I2cPlatformDxeLibPtlPcd.inf
    }
!endif

!if gProtectedVariableFeaturePkgTokenSpaceGuid.PcdProtectedVariableEnable == TRUE
  !include ProtectedVariableFeaturePkg/Include/ProtectedVariableFeature.dsc
!endif

!if gPsFeatureModuleTokenSpaceGuid.PcdPlatformSanitizeFeatureEnable == TRUE
!include PlatformSanitizeFeaturePkg/Include/PlatformSanitizeFeature.dsc
!endif
!include NvmeFeaturePkg/Include/NvmeFeature.dsc

!if gWatchdogTimerFeaturePkgTokenSpaceGuid.PcdWatchdogTimerFeatureEnable == TRUE
  !include WatchdogTimerFeaturePkg/Include/WatchdogTimerFeature.dsc
!endif

#
# Memory Telemetry Feature
#
!if gMemoryTelemetryFeaturePkgTokenSpaceGuid.PcdMemoryTelemetryEnabled == TRUE
  !include  MemoryTelemetryFeaturePkg/Include/MemoryTelemetryFeaturePkg.dsc
!endif  # PcdMemoryTelemetryEnabled

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################

!include $(PLATFORM_PACKAGE)/Include/Dsc/CoreCommonLib.dsc
!include $(PLATFORM_PACKAGE)/Include/Dsc/CorePeiLib.dsc
!include $(PLATFORM_PACKAGE)/Include/Dsc/CoreDxeLib.dsc
#
# Silicon Init Package
#
!include $(SILICON_PRODUCT_PATH)/SiPkgCommonLib.dsc
!include $(SILICON_PRODUCT_PATH)/SiPkgPeiLib.dsc
!include $(SILICON_PRODUCT_PATH)/SiPkgDxeLib.dsc
!include $(PLATFORM_BIN_PACKAGE)/Include/Dsc/BinCommonLib.dsc
!include $(PLATFORM_BIN_PACKAGE)/Include/Dsc/BinPeiLib.dsc
!include $(PLATFORM_BIN_PACKAGE)/Include/Dsc/BinDxeLib.dsc


[LibraryClasses.IA32.PEI_CORE, LibraryClasses.X64.PEI_CORE]
!if $(TARGET) == DEBUG
DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
!endif
!if gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE
  DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
!endif

[LibraryClasses.common]
  PlatformHookLib|$(PLATFORM_BOARD_PACKAGE)/Library/BasePlatformHookLib/BasePlatformHookLib.inf
  BpmAccessLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseBpmAccessLib/BaseBpmAccessLib.inf
  FitAccessLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseFitAccessLib/BaseFitAccessLib.inf
  SiliconPolicyInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/SiliconPolicyInitLibNull/SiliconPolicyInitLibNull.inf
  SiliconPolicyUpdateLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/SiliconPolicyUpdateLibNull/SiliconPolicyUpdateLibNull.inf

PciHostBridgeLib|$(PLATFORM_PACKAGE)/Pci/Library/PciHostBridgeLibSimple/PciHostBridgeLibSimple.inf

!if gPlatformModuleTokenSpaceGuid.PcdSerialPortEnable == TRUE
  SerialPortLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseSerialPortLib/BaseSerialPortLib.inf
!else
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
!endif
  TimerLib|UefiCpuPkg/Library/CpuTimerLib/BaseCpuTimerLib.inf
  PeiGetVtdPmrAlignmentLib|IntelSiliconPkg/Library/PeiGetVtdPmrAlignmentLib/PeiGetVtdPmrAlignmentLib.inf

  FspCommonLib|IntelFsp2Pkg/Library/BaseFspCommonLib/BaseFspCommonLib.inf
  FspWrapperApiLib|IntelFsp2WrapperPkg/Library/BaseFspWrapperApiLib/BaseFspWrapperApiLib.inf
  FspSwitchStackLib|IntelFsp2Pkg/Library/BaseFspSwitchStackLib/BaseFspSwitchStackLib.inf

  ConfigBlockLib|IntelSiliconPkg/Library/BaseConfigBlockLib/BaseConfigBlockLib.inf
  SiliconInitLib|$(PLATFORM_BSP_PATH)/Library/SiliconInitLib/SiliconInitLib.inf
  MmioInitLib|$(PLATFORM_BOARD_PACKAGE)/Library/BaseMmioInitLib/BaseMmioInitLib.inf
  AmdSvsmLib|UefiCpuPkg/Library/AmdSvsmLibNull/AmdSvsmLibNull.inf

#
# NEM
#
  NemConfigLib|$(PLATFORM_SI_PACKAGE)/Library/BaseNemConfigLib/BaseNemConfigLib.inf
  BoardInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/BoardInitLibNull/BoardInitLibNull.inf
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLibNull/TestPointCheckLibNull.inf
  PciSegmentInfoLib|$(PLATFORM_PACKAGE)/Pci/Library/PciSegmentInfoLibSimple/PciSegmentInfoLibSimple.inf

  PciLib|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf
  BoardIdsLib|$(PLATFORM_BOARD_PACKAGE)/Library/PeiDxeBoardIdsLib/PeiDxeBoardIdsLib.inf
  WakeupEventLib|$(PLATFORM_BOARD_PACKAGE)/Library/WakeupEventLib/WakeupEventLib.inf
  DxeUpdatePlatformInfoLib|$(PLATFORM_BOARD_PACKAGE)/Library/DxeUpdatePlatformInfoLib/DxeUpdatePlatformInfoLib.inf

  HidI2cPlatformSupportLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeHidI2cPlatformSupportLib/DxeHidI2cPlatformSupportLib.inf
  UsbcRetimerConfigLib|$(PLATFORM_BOARD_PACKAGE)/Features/Tcss/Library/PeiUsbcRetimerConfigLib/PeiUsbcRetimerConfigLib.inf
  ItbtVariableUpdateLib|$(PLATFORM_BOARD_PACKAGE)/Features/Tcss/Library/DxeItbtVariableUpdateLib/DxeItbtVariableUpdateLib.inf
  PlatformPasswordLib|UserAuthFeaturePkg/Library/PlatformPasswordLibNull/PlatformPasswordLibNull.inf
  UserPasswordLib|UserAuthFeaturePkg/Library/UserPasswordLib/UserPasswordLib.inf
  UserPasswordUiLib|UserAuthFeaturePkg/Library/UserPasswordUiLib/UserPasswordUiLib.inf
!if gSiPkgTokenSpaceGuid.PcdFspVEnable == TRUE
  FspVPolicyLib|$(PLATFORM_SI_PACKAGE)/Fsp/Library/FspValidation/FspVPolicyLib/FspVPolicyLib.inf
!else
  FspVPolicyLib|$(PLATFORM_SI_PACKAGE)/Fsp/Library/FspValidation/FspVPolicyLib/FspVPolicyLibNull.inf
!endif
!if gPlatformModuleTokenSpaceGuid.PcdSkipFspTempRamInitAndExit == TRUE
  CacheAsRamLib|$(PLATFORM_SI_PACKAGE)/Library/BaseCacheAsRamLib/BaseCacheAsRamLib.inf
!else
  CacheAsRamLib|IntelFsp2Pkg/Library/BaseCacheAsRamLibNull/BaseCacheAsRamLibNull.inf
!endif

  SiPostCodeMapLib|$(PLATFORM_SI_PACKAGE)/Library/Rsc2PostCodeMapLib/SiPostCodeMapLib.inf
  PostCodeMapOverrideLib|$(PLATFORM_SI_PACKAGE)/Library/Rsc2PostCodeMapLib/PostCodeMapOverrideLib.inf
  CorePostCodeMapLib|$(PLATFORM_SI_PACKAGE)/Library/Rsc2PostCodeMapLib/CorePostCodeMapLib.inf
  PlatformPostCodeMapLib|$(PLATFORM_SI_PACKAGE)/Library/Rsc2PostCodeMapLib/PlatformPostCodeMapLib.inf
  Rsc2PostCodeMapLib|$(PLATFORM_SI_PACKAGE)/Library/Rsc2PostCodeMapLib/Rsc2PostCodeMapLib.inf

  PostCodeMapLib|$(PLATFORM_SI_PACKAGE)/Library/Rsc2PostCodeMapLib/PostCodeMapLib.inf

!if gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE
  DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
!endif
  BeepLib|$(PLATFORM_FULL_PACKAGE)/Library/BeepLib/BeepLib.inf
  BeepMapLib|BeepDebugFeaturePkg/Library/BeepMapLib/BeepMapLib.inf
  PostCodeToScratchPadLib|$(PLATFORM_SI_PACKAGE)/Library/BasePostCodeToScratchPadLibNull/BasePostCodeToScratchPadLibNull.inf
  Usb3DebugPortParameterLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseUsb3DebugPortParameterLibCmos/BaseUsb3DebugPortParameterLibCmos.inf
  BaseDciInfoLib|$(SILICON_PRODUCT_PATH)/Library/BaseDciInfoLib/BaseDciInfoLib.inf
  # TODO: This block of libraries is here because they are a dependency for PantherLakePlatSamplePkg/Setup/Setup.inf
  #       The Setup module needs to be refactored, these need to be refactored, and moved to PantherLakeBoardPkg
  #       over time.
  SerialPortParameterLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseSerialPortParameterLibPcd/BaseSerialPortParameterLibPcd.inf
  SerialIoUartDebugHelperLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseSerialIoUartDebugHelperLib/BaseSerialIoUartDebugHelperLib.inf
  SerialIoUartDebugPropertyLib|$(PLATFORM_FULL_PACKAGE)/Library/SerialIoUartDebugPropertyLib/BaseSerialIoUartDebugPropertyLib.inf
  PlatformNvRamHookLib|CmosFeaturePkg/Library/PlatformNvRamHookLibCmos/PlatformNvRamHookLibCmos.inf
  CmosAccessLib|BoardModulePkg/Library/CmosAccessLib/CmosAccessLib.inf
  PlatformCmosAccessLib|CmosFeaturePkg/Library/PlatformCmosAccessLib/PlatformCmosAccessLib.inf
  DimmInfoLib|$(PLATFORM_FULL_PACKAGE)/Setup/DimmInfoLib.inf

#
# Security
#
  TpmCommLib|SecurityPkg/Library/TpmCommLib/TpmCommLib.inf

#
# Silicon Init Package
#
!include $(SILICON_PRODUCT_PATH)/SiPkgCommonLib.dsc

#
# Features
#
!if gBoardModuleTokenSpaceGuid.PcdSetupEnable == TRUE
  !include $(PLATFORM_BOARD_PACKAGE)/Features/Setup/Include/Dsc/SetupBaseLib.dsc
!endif

CmosAccessLib|BoardModulePkg/Library/CmosAccessLib/CmosAccessLib.inf
!if gCmosFeaturePkgTokenSpaceGuid.PcdPlatformCmosAccessSupport == TRUE
  PlatformCmosAccessLib|CmosFeaturePkg/Library/PlatformCmosAccessLib/PlatformCmosAccessLib.inf
!else
  PlatformCmosAccessLibNull|CmosFeaturePkg/Library/PlatformCmosAccessLibNull/PlatformCmosAccessLibNull.inf
!endif

  ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
  HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
  BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf

  # Nvme feature
!if gNvmeFeaturePkgTokenSpaceGuid.PcdNvmePciScanEnable == TRUE
  NvmePcieRpLib|$(PLATFORM_BOARD_PACKAGE)/Features/Nvme/Library/NvmePcieRpLib/NvmePcieRpLib.inf
!endif

  # Sata feature
!if gSiPkgTokenSpaceGuid.PcdAtaEnable == TRUE
  AhciPcieRpLib|$(PLATFORM_BOARD_PACKAGE)/Features/Sata/Library/AhciPcieRpLib/AhciPcieRpLib.inf
!endif

#
# S3
#
!if gSiPkgTokenSpaceGuid.PcdS3Enable == TRUE
  S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
!endif
  S3PciSegmentLib|MdePkg/Library/BaseS3PciSegmentLib/BaseS3PciSegmentLib.inf

!if gCapsuleFeaturePkgTokenSpaceGuid.PcdCapsuleFeatureEnable == TRUE
  PlatformBiosUpdateHookLib|$(PLATFORM_BOARD_PACKAGE)/Features/CapsuleUpdate/Library/PlatformBiosUpdateHookLib/PlatformBiosUpdateHookLib.inf
  PlatformEcUpdateHookLib|$(PLATFORM_BOARD_PACKAGE)/Features/CapsuleUpdate/Library/PlatformEcUpdateHookLib/PlatformEcUpdateHookLib.inf
  PlatformWdtLib|$(PLATFORM_BOARD_PACKAGE)/Features/CapsuleUpdate/Library/PlatformWdtLib/PlatformWdtLib.inf
  PlatformEcResetLib|$(PLATFORM_BOARD_PACKAGE)/Features/CapsuleUpdate/Library/PlatformEcResetLib/PlatformEcResetLib.inf
  PlatformMeUpdateHookLib|CapsuleFeaturePkg/Library/PlatformMeUpdateHookLibNull/PlatformMeUpdateHookLibNull.inf
  PlatformDxeBootStateHookLib|$(PLATFORM_BOARD_PACKAGE)/Features/CapsuleUpdate/Library/PlatformDxeBootStateHookLib/PlatformDxeBootStateHookLib.inf
  PayloadResiliencySupportLib|$(PLATFORM_BOARD_PACKAGE)/Features/CapsuleUpdate/Library/PayloadResiliencySupportLib/PayloadResiliencySupportLib.inf
!else
  PlatformWdtLib|CapsuleFeaturePkg/Library/PlatformWdtLibNull/PlatformWdtLibNull.inf
  ResiliencySupportLib|$(PLATFORM_BOARD_PACKAGE)/Features/CapsuleUpdate/Library/PeiDxeSmmResiliencySupportLibNull/PeiDxeSmmResiliencySupportLibNull.inf
  PayloadResiliencySupportLib|CapsuleFeaturePkg/Library/PayloadResiliencySupportLibNull/PayloadResiliencySupportLibNull.inf
!endif
  TcgStorageCoreLib|SecurityPkg/Library/TcgStorageCoreLib/TcgStorageCoreLib.inf
  TcgStorageOpalLib|SecurityPkg/Library/TcgStorageOpalLib/TcgStorageOpalLib.inf

!if gPsFeatureModuleTokenSpaceGuid.PcdPlatformSanitizeFeatureEnable == TRUE
  EraseServiceLib|$(PLATFORM_BOARD_PACKAGE)/Features/PlatformSanitize/StorageSanitize/Library/EraseServiceLib/EraseServiceLib.inf
  StoragePlatformEventLib|$(PLATFORM_BOARD_PACKAGE)/Features/PlatformSanitize/StorageSanitize/Library/StoragePlatformEventLib/StoragePlatformEventLib.inf
  AhciCommandLib|SataFeaturePkg/Library/AhciCommandLib/AhciCommandLib.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == 0x1
#
# Sub Region FVs
#
  PeiSubRegionLib|$(PLATFORM_FEATURES_PATH)/SubRegion/PeiSubRegionLib/PeiSubRegionLib.inf

#
# TSN
#
  PeiTsnFvLib|$(PLATFORM_FEATURES_PATH)/Tsn/PeiTsnFvLib/PeiTsnFvLib.inf
!else
  PeiSubRegionLib|$(PLATFORM_FEATURES_PATH)/SubRegion/PeiSubRegionLib/PeiSubRegionLibNull.inf
  PeiTsnFvLib|$(PLATFORM_FEATURES_PATH)/Tsn/PeiTsnFvLib/PeiTsnFvLibNull.inf
!endif
  CcExitLib|UefiCpuPkg/Library/CcExitLibNull/CcExitLibNull.inf

#
# TelemetryFirmwareVersionHookLib
#
!if gTelemetryFeaturePkgTokenSpaceGuid.PcdTelemetryFeatureEnable == TRUE
  TelemetryFirmwareVersionHookLib|$(PLATFORM_BOARD_PACKAGE)/Features/Telemetry/TelemetryFirmwareVersionHookLib.inf
!endif

#
# Trace Hub
#
  MipiSysTLib|MdePkg/Library/MipiSysTLib/MipiSysTLib.inf
  TraceHubDebugLib|$(PLATFORM_SI_PACKAGE)/Library/TraceHubDebugLib/BaseTraceHubDebugLib.inf
  TraceHubDebugSysTLib|MdeModulePkg/Library/TraceHubDebugSysTLib/BaseTraceHubDebugSysTLib.inf

  CpuPageTableLib|UefiCpuPkg/Library/CpuPageTableLib/CpuPageTableLib.inf
  PlatformUsbConfigLib|$(PLATFORM_BOARD_PACKAGE)/Library/PlatformUsbConfigLib/PlatformUsbConfigLib.inf
  HeciControlFilterLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciControl/HeciControlFilterLib/HeciControlFilterLib.inf

#
# IntelVTdPeiDxeLib
#
  IntelVTdPeiDxeLib|IntelSiliconPkg/Library/IntelVTdPeiDxeLib/IntelVTdPeiDxeLib.inf

[LibraryClasses.IA32.PEIM, LibraryClasses.X64.PEIM]
!if gBoardModuleTokenSpaceGuid.PcdModularCryptoEnable == TRUE
  BaseCryptLib|CryptoPkg/Library/BaseCryptLibOnProtocolPpi/PeiCryptLib.inf
  TlsLib|CryptoPkg/Library/BaseCryptLibOnProtocolPpi/PeiCryptLib.inf
!endif

  FspWrapperPlatformMultiPhaseLib|IntelFsp2WrapperPkg/Library/BaseFspWrapperPlatformMultiPhaseLibNull/BaseFspWrapperPlatformMultiPhaseLibNull.inf

  SmmControlLib|IntelSiliconPkg/Feature/SmmControl/Library/PeiSmmControlLib/PeiSmmControlLib.inf

  #
  # This is library instance of Cpu Hob Library, replacing the library instance from MinPlatform.
  #
  ReportCpuHobLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/PeiReportCpuHob/Library/PeiReportCpuHobLib/PeiReportCpuHobLib.inf

  PeiLib|$(PLATFORM_PACKAGE)/Library/PeiLib/PeiLib.inf

  ReportFvLib|$(PLATFORM_BSP_PATH)/Library/PeiReportFvLib/PeiReportFvLib.inf
  BpdtLib|$(PLATFORM_FULL_PACKAGE)/Library/BpdtLib/BpdtLib.inf
  NemMapLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiNemMapLib/PeiNemMapLib.inf
  FspWrapperApiTestLib|IntelFsp2WrapperPkg/Library/PeiFspWrapperApiTestLib/PeiFspWrapperApiTestLib.inf
  PeiBootModeLib|$(PLATFORM_BSP_PATH)/Library/PeiBootModeLib/PeiBootModeLib.inf
  TxtPolicyLibPreMem|$(PLATFORM_SI_PACKAGE)/IpBlock/Txt/LibraryPrivate/PeiTxtPolicyLib/PeiTxtPolicyLibPreMem.inf
  TraceHubHookLib|$(SILICON_PRODUCT_PATH)/Library/TraceHubHookLib/PeiTraceHubHookLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLibBase.inf

#
# PEI phase common
#
  PeiGetFvInfoLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiGetFvInfoLib/PeiGetFvInfoLib.inf

!if $(TARGET) == DEBUG
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/PeiTestPointCheckLib.inf
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLibNull/TestPointCheckLibNull.inf
!endif
  TestPointLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointLib/PeiTestPointLib.inf
  MultiBoardInitSupportLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/MultiBoardInitSupportLib/PeiMultiBoardInitSupportLib.inf
  BoardInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/MultiBoardInitSupportLib/PeiMultiBoardInitSupportLib.inf

  BoardConfigLib|$(PLATFORM_BSP_PATH)/Library/PeiBoardConfigLib/PeiBoardConfigLib.inf
  GpioV2WrapperLib|$(PLATFORM_BOARD_PACKAGE)/Library/PeiDxeGpioV2WrapperLib/PeiDxeGpioV2WrapperLib.inf
  PciIoLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/PciIoLib/PciIoLib.inf

#
# Features
#
!if gBoardModuleTokenSpaceGuid.PcdSetupEnable == TRUE
  !include $(PLATFORM_BOARD_PACKAGE)/Features/Setup/Include/Dsc/SetupPeiLib.dsc
!endif
#
# SmmAccess
#
  SmmAccessLib|IntelSiliconPkg/Feature/SmmAccess/Library/PeiSmmAccessLib/PeiSmmAccessLib.inf

#
# S3
#
  S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf
  MpInitLib|UefiCpuPkg/Library/MpInitLib/PeiMpInitLib.inf
  BiosIdLib|BoardModulePkg/Library/BiosIdLib/PeiBiosIdLib.inf

#
# PEI Boot State Library Instance
#
  PeiBootStateLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiBootStateLib/PeiBootStateLib.inf

  FspSwitchStackLib|IntelFsp2Pkg/Library/BaseFspSwitchStackLib/BaseFspSwitchStackLib.inf

!if $(TARGET) == DEBUG
  SerialIoUartDebugPropertyLib|$(PLATFORM_FULL_PACKAGE)/Library/SerialIoUartDebugPropertyLib/PeiSerialIoUartDebugPropertyLib.inf
  !if gPlatformModuleTokenSpaceGuid.PcdSerialPortEnable == TRUE
    SerialPortLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseSerialPortLib/BaseSerialPortLib.inf
  !else
    SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
  !endif
  DebugLib|MdeModulePkg/Library/PeiDebugLibDebugPpi/PeiDebugLibDebugPpi.inf
  DebugPrintErrorLevelLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiDxeSmmDebugPrintErrorLevelLib/PeiDebugPrintErrorLevelLib.inf
!endif

!if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
  DebugLib|$(PLATFORM_SI_PACKAGE)/Library/TraceHubDebugLibPpi/CatalogTraceHubDebugLibPpi.inf
!endif

  #
  # Use Null library instance to skip MTRR initialization from MinPlatformPkg PlatformInit modules.
  # MTRR configuration will be done by FSP or PlatformInitAdvanced modules.
  #
  SetCacheMtrrLib|$(PLATFORM_PACKAGE)/Library/SetCacheMtrrLib/SetCacheMtrrLibNull.inf

!if gSiPkgTokenSpaceGuid.PcdOverclockEnable == TRUE
  PlatformVoltageControlLib|$(PLATFORM_BOARD_PACKAGE)/Library/PlatformVoltageControlLib/PlatformVoltageControlLib.inf
!else
  PlatformVoltageControlLib|$(PLATFORM_BOARD_PACKAGE)/Library/PlatformVoltageControlLibNull/PlatformVoltageControlLibNull.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdBiosGuardEnable == TRUE
  PeiBiosGuardInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/PeiBiosGuardInitFruLib/PeiBiosGuardInitFruLib.inf
!endif

#
# UEFI Variable Protection
#
!if gProtectedVariableFeaturePkgTokenSpaceGuid.PcdProtectedVariableEnable == TRUE
  ProtectedVariableRpmcLib|ProtectedVariableFeaturePkg/Library/PeiRpmcLib/PeiRpmcLib.inf
  HashApiLib|CryptoPkg/Library/BaseHashApiLib/BaseHashApiLib.inf
  ProtectedVariableKeyLib|ProtectedVariableFeaturePkg/Library/PeiProtectedVariableKeyLib/PeiProtectedVariableKeyLib.inf
  ProtectedVariableLib|ProtectedVariablePkg/Library/ProtectedVariableLib/PeiProtectedVariableLib.inf
  SortLib|MdeModulePkg/Library/BaseSortLib/BaseSortLib.inf
!endif


[LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.UEFI_DRIVER, LibraryClasses.common.UEFI_APPLICATION]
!if gBoardModuleTokenSpaceGuid.PcdModularCryptoEnable == TRUE
  BaseCryptLib|CryptoPkg/Library/BaseCryptLibOnProtocolPpi/DxeCryptLib.inf
  TlsLib|CryptoPkg/Library/BaseCryptLibOnProtocolPpi/DxeCryptLib.inf
!endif

!if gUsbCCapsuleFeaturePkgTokenSpaceGuid.PcdUsbCCapsuleFeatureEnable == FALSE
  TbtNvmRetimerUpdateLib|UsbCCapsuleFeaturePkg/Library/TbtNvmRetimerUpdateLib/TbtNvmRetimerUpdateLib.inf
  UsbcCapsuleDebugLib|UsbCCapsuleFeaturePkg/Library/UsbcCapsuleDebugLib/UsbcCapsuleDebugLib.inf
!endif

[LibraryClasses.common.DXE_SMM_DRIVER]
!if gBoardModuleTokenSpaceGuid.PcdModularCryptoEnable == TRUE
  BaseCryptLib|CryptoPkg/Library/BaseCryptLibMbedTls/SmmCryptLib.inf
  TlsLib|CryptoPkg/Library/TlsLibNull/TlsLibNull.inf
  MbedTlsLib|CryptoPkg/Library/MbedTlsLib/MbedTlsLib.inf
!endif

[LibraryClasses.IA32.PEIM, LibraryClasses.IA32.PEI_CORE, LibraryClasses.X64.PEIM, LibraryClasses.X64.PEI_CORE]
  ResetSystemLib|MdeModulePkg/Library/PeiResetSystemLib/PeiResetSystemLib.inf
  TraceHubDebugSysTLib|MdeModulePkg/Library/TraceHubDebugSysTLib/PeiTraceHubDebugSysTLib.inf
!if gPlatformModuleTokenSpaceGuid.PcdUsb3SerialStatusCodeEnable == TRUE
  Usb3DebugPortLib|Usb3DebugFeaturePkg/Library/Usb3DebugPortLib/Usb3DebugPortLibPei.inf
!else
  Usb3DebugPortLib|Usb3DebugFeaturePkg/Library/Usb3DebugPortLib/Usb3DebugPortLibNull.inf
!endif
  SerialPortParameterLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiSerialPortParameterLib/PeiSerialPortParameterLib.inf
  HeciUtilsLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciUtils/HeciUtilsPeiLib.inf
  HeciTransportCoreLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciTransport/HeciTransportCoreLib/HeciTransportCorePeiLib.inf

[LibraryClasses.IA32.SEC, LibraryClasses.IA32.PEIM, LibraryClasses.IA32.PEI_CORE, LibraryClasses.X64.SEC, LibraryClasses.X64.PEIM, LibraryClasses.X64.PEI_CORE]
  TimerLib|UefiCpuPkg/Library/CpuTimerLib/BaseCpuTimerLib.inf
  PeiPolicyBoardConfigLib|$(PLATFORM_BSP_PATH)/Library/PeiPolicyBoardConfigLib/PeiPolicyBoardConfigLib.inf
  PeiPolicyAdvancedConfigLib|$(PLATFORM_BSP_PATH)/Library/PeiPolicyAdvancedConfigLib/PeiPolicyAdvancedConfigLib.inf
!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  PeiPolicyUpdateLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiPolicyUpdateLib/PeiPolicyUpdateLibApi.inf
  PeiPolicyDebugLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiPolicyDebugLib/PeiPolicyDebugLibApi.inf
!else
  PeiPolicyUpdateLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiPolicyUpdateLib/PeiPolicyUpdateLib.inf
  PeiPolicyDebugLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiPolicyDebugLib/PeiPolicyDebugLib.inf
!endif


[LibraryClasses.IA32.SEC, LibraryClasses.X64.SEC]
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  Usb3DebugPortLib|Usb3DebugFeaturePkg/Library/Usb3DebugPortLib/Usb3DebugPortLibNull.inf
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/SecTestPointCheckLib.inf
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLibNull/TestPointCheckLibNull.inf
  SecBoardInitLib|$(PLATFORM_BSP_PATH)/Library/SecBoardInitLib/SecBoardInitLib.inf
  ResetSystemLib|MdeModulePkg/Library/BaseResetSystemLibNull/BaseResetSystemLibNull.inf
  SerialPortParameterLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseSerialPortParameterLibPcd/BaseSerialPortParameterLibPcd.inf
  SecGetFsptApiParameterLib|$(PLATFORM_SI_PACKAGE)/Library/SecGetFsptApiParameterLib/SecGetFsptApiParameterLib.inf
  GpioHelpersLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Gpio/LibraryPrivate/BaseGpioHelpersLibNull/BaseGpioHelpersLibNull.inf
  TraceHubHookLib|$(SILICON_PRODUCT_PATH)/Library/TraceHubHookLib/BaseTraceHubHookLib.inf
  PtlPcdSerialIoGpioAccessLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdSerialIoLib/PtlPcdSecSerialIoGpioAccessLib.inf

[LibraryClasses.X64.DXE_CORE, LibraryClasses.X64.DXE_DRIVER, LibraryClasses.X64.SMM_CORE, LibraryClasses.X64.DXE_SMM_DRIVER, LibraryClasses.X64.UEFI_DRIVER, LibraryClasses.X64.UEFI_APPLICATION]
  TraceHubDebugSysTLib|MdeModulePkg/Library/TraceHubDebugSysTLib/DxeSmmTraceHubDebugSysTLib.inf

#
# Libraries which are used by X64 drivers executed after PEI phase
#
[LibraryClasses.X64.DXE_CORE, LibraryClasses.X64.DXE_DRIVER, LibraryClasses.X64.DXE_RUNTIME_DRIVER, LibraryClasses.X64.SMM_CORE, LibraryClasses.X64.DXE_SMM_DRIVER, LibraryClasses.X64.UEFI_DRIVER, LibraryClasses.X64.UEFI_APPLICATION]
  TimerLib|UefiCpuPkg/Library/CpuTimerLib/BaseCpuTimerLib.inf
  FirmwareBootMediaLib|IntelSiliconPkg/Library/PeiDxeSmmBootMediaLib/DxeSmmFirmwareBootMediaLib.inf

!if gPlatformModuleTokenSpaceGuid.PcdUsb3SerialStatusCodeEnable == TRUE
  Usb3DebugPortLib|Usb3DebugFeaturePkg/Library/Usb3DebugPortLib/Usb3DebugPortLibDxe.inf
!else
  Usb3DebugPortLib|Usb3DebugFeaturePkg/Library/Usb3DebugPortLib/Usb3DebugPortLibNull.inf
!endif
!if gBoardModuleTokenSpaceGuid.PcdModularCryptoEnable == FALSE
  TlsLib|CryptoPkg/Library/TlsLib/TlsLib.inf
!endif

  TraceHubHookLib|$(PLATFORM_SI_PACKAGE)/IpBlock/TraceHub/Library/TraceHubHookLib/DxeSmmTraceHubHookLib.inf
  PostCodeLib|$(PLATFORM_SI_PACKAGE)/Library/BasePostCodeLibPort80TraceHub/BasePostCodeLibPort80TraceHub.inf
  HeciUtilsLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciUtils/HeciUtilsDxeSmmLib.inf
  HeciTransportCoreLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciTransport/HeciTransportCoreLib/HeciTransportCoreDxeSmmLib.inf


[LibraryClasses.X64.DXE_DRIVER, LibraryClasses.X64.DXE_SMM_DRIVER]
  BiosIdLib|BoardModulePkg/Library/BiosIdLib/DxeBiosIdLib.inf

[LibraryClasses.X64.DXE_DRIVER, LibraryClasses.X64.DXE_RUNTIME_DRIVER]
#
# DXE Boot State Library Instance
#
  DxeBootStateLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeBootStateLib/DxeBootStateLib.inf
  SpiFlashCommonLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeSpiFlashCommonLib/DxeSpiFlashCommonLib.inf

[LibraryClasses.X64.DXE_DRIVER]
  PciHostBridgeLib|$(PLATFORM_PACKAGE)/Pci/Library/PciHostBridgeLibSimple/PciHostBridgeLibSimple.inf
#
# Platform
#
!if gBoardModuleTokenSpaceGuid.PcdSetupEnable == TRUE
  PlatformBootManagerLib|$(PLATFORM_FULL_PACKAGE)/Library/DxePlatformBootManagerLib/DxePlatformBootManagerLib.inf
!else
  PlatformBootManagerLib|$(PLATFORM_FULL_PACKAGE)/Library/DxePlatformBootManagerLib/DxePlatformBootManagerHiiFreeLib.inf
!endif
  #
  # DXE phase common
  #
  SerialPortParameterLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeSmmSerialPortParameterLib/DxeSmmSerialPortParameterLib.inf

!if $(TARGET) == DEBUG
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/DxeTestPointCheckLib.inf
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLibNull/TestPointCheckLibNull.inf
!endif
  TestPointLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointLib/DxeTestPointLib.inf
  MultiBoardInitSupportLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/MultiBoardInitSupportLib/DxeMultiBoardInitSupportLib.inf
  BoardInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/MultiBoardInitSupportLib/DxeMultiBoardInitSupportLib.inf
  MultiBoardAcpiSupportLib|$(PLATFORM_PACKAGE)/Acpi/Library/MultiBoardAcpiSupportLib/DxeMultiBoardAcpiSupportLib.inf
  BoardAcpiTableLib|$(PLATFORM_PACKAGE)/Acpi/Library/MultiBoardAcpiSupportLib/DxeMultiBoardAcpiSupportLib.inf
  SiliconPolicyUpdateLib|$(PLATFORM_OPEN_BOARD_PACKAGE)/Policy/Library/DxeSiliconPolicyUpdateLib/DxeSiliconPolicyUpdateLib.inf
  DxeAcpiGnvsInitLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeAcpiGnvsInitLib/DxeAcpiGnvsInitLib.inf

  DxePolicyUpdateLib|$(PLATFORM_FULL_PACKAGE)/Library/DxePolicyUpdateLib/DxePolicyUpdateLib.inf

  BootLogoLib|MdeModulePkg/Library/BootLogoLib/BootLogoLib.inf
  TcgPhysicalPresenceLib|SecurityPkg/Library/DxeTcgPhysicalPresenceLib/DxeTcgPhysicalPresenceLib.inf
  TcgPpVendorLib|SecurityPkg/Library/TcgPpVendorLibNull/TcgPpVendorLibNull.inf

!if gSiPkgTokenSpaceGuid.PcdSmbiosEnable == TRUE
  DxeSmbiosProcessorLib|$(PLATFORM_SI_PACKAGE)/Library/DxeSmbiosProcessorLib/DxeSmbiosProcessorLib.inf
!else
  DxeSmbiosProcessorLib|$(PLATFORM_SI_PACKAGE)/Library/DxeSmbiosProcessorLibNull/DxeSmbiosProcessorLibNull.inf
!endif


  BoardConfigLib|$(PLATFORM_BOARD_PACKAGE)/Library/DxeBoardConfigLib/DxeBoardConfigLib.inf

  DxeOpromPatchLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeOpromPatchLib/DxeOpromPatchLib.inf
  MpInitLib|UefiCpuPkg/Library/MpInitLib/DxeMpInitLib.inf

  PdtUpdateLib|$(PLATFORM_FULL_PACKAGE)/Library/PdtUpdateLib/PdtUpdateLib.inf
  GpioV2WrapperLib|$(PLATFORM_BOARD_PACKAGE)/Library/PeiDxeGpioV2WrapperLib/PeiDxeGpioV2WrapperLib.inf

[LibraryClasses.X64]
!if gPlatformModuleTokenSpaceGuid.PcdSerialPortEnable == TRUE
  SerialPortLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseSerialPortLib/BaseSerialPortLib.inf
!else
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
!endif

#
# Silicon Init Package
#
!include $(SILICON_PRODUCT_PATH)/SiPkgDxeLib.dsc

  PciIoLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/PciIoLib/DxePciIoLib.inf


  BluetoothLib|BluetoothPkg/Library/BluetoothLib/BluetoothLib.inf
  HidLib|BluetoothPkg/Library/HidLib/HidLib.inf

  PostCodeToScratchPadLib|$(PLATFORM_SI_PACKAGE)/Library/BasePostCodeToScratchPadLibNull/BasePostCodeToScratchPadLibNull.inf

[LibraryClasses.X64.DXE_CORE]
  SerialPortParameterLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeCoreSerialPortParameterLib/DxeCoreSerialPortParameterLib.inf
  TraceHubHookLib|$(PLATFORM_SI_PACKAGE)/IpBlock/TraceHub/Library/TraceHubHookLib/DxeCoreTraceHubHookLib.inf
!if $(TARGET) == DEBUG
  SerialIoUartDebugPropertyLib|$(PLATFORM_FULL_PACKAGE)/Library/SerialIoUartDebugPropertyLib/DxeCoreSerialIoUartDebugPropertyLib.inf
  DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
!endif
!if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
  DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
!endif

[LibraryClasses.X64.DXE_SMM_DRIVER]
!if gSiPkgTokenSpaceGuid.PcdBiosGuardEnable == TRUE
  SpiFlashCommonLib|$(PLATFORM_FULL_PACKAGE)/Library/SmmSpiFlashCommonLib/SmmSpiFlashCommonLibBiosGuard.inf
!else
  SpiFlashCommonLib|$(PLATFORM_FULL_PACKAGE)/Library/SmmSpiFlashCommonLib/SmmSpiFlashCommonLib.inf
!endif

!if $(TARGET) == DEBUG
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/SmmTestPointCheckLib.inf
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLibNull/TestPointCheckLibNull.inf
  SerialIoUartDebugPropertyLib|$(PLATFORM_FULL_PACKAGE)/Library/SerialIoUartDebugPropertyLib/DxeSmmSerialIoUartDebugPropertyLib.inf
!endif
  TestPointLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointLib/SmmTestPointLib.inf
  MultiBoardAcpiSupportLib|$(PLATFORM_PACKAGE)/Acpi/Library/MultiBoardAcpiSupportLib/SmmMultiBoardAcpiSupportLib.inf
  BoardAcpiEnableLib|$(PLATFORM_PACKAGE)/Acpi/Library/MultiBoardAcpiSupportLib/SmmMultiBoardAcpiSupportLib.inf
  DxeSmmScriptLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeSmmScriptLib/DxeSmmScriptLib.inf
  SmmIoLib|MdePkg/Library/SmmIoLib/SmmIoLib.inf
  GpioV2WrapperLib|$(PLATFORM_BOARD_PACKAGE)/Library/SmmGpioV2WrapperLib/SmmGpioV2WrapperLib.inf

#
# UEFI Variable Protection
#
!if gProtectedVariableFeaturePkgTokenSpaceGuid.PcdProtectedVariableEnable == TRUE
  ProtectedVariableRpmcLib|ProtectedVariableFeaturePkg/Library/SmmRpmcLib/SmmRpmcLib.inf
  HashApiLib|CryptoPkg/Library/BaseHashApiLib/BaseHashApiLib.inf
  ProtectedVariableKeyLib|ProtectedVariablePkg/Library/ProtectedVariableKeyLibNull/ProtectedVariableKeyLibNull.inf
  ProtectedVariableLib|ProtectedVariablePkg/Library/ProtectedVariableLib/SmmProtectedVariableLib.inf
  SortLib|MdeModulePkg/Library/BaseSortLib/BaseSortLib.inf
!endif
  PostCodeLib|$(PLATFORM_SI_PACKAGE)/Library/BasePostCodeLibPort80TraceHubSmm/BasePostCodeLibPort80TraceHubSmm.inf

[LibraryClasses.X64.DXE_RUNTIME_DRIVER]
  ReportStatusCodeLib|MdeModulePkg/Library/RuntimeDxeReportStatusCodeLib/RuntimeDxeReportStatusCodeLib.inf
  DebugPrintErrorLevelLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiDxeSmmDebugPrintErrorLevelLib/DxeSmmDebugPrintErrorLevelLib.inf
  SerialPortParameterLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeSmmSerialPortParameterLib/DxeSmmSerialPortParameterLib.inf

!if $(TARGET) == DEBUG
  SerialIoUartDebugPropertyLib|$(PLATFORM_FULL_PACKAGE)/Library/SerialIoUartDebugPropertyLib/DxeSmmSerialIoUartDebugPropertyLib.inf
!endif

!if gPlatformModuleTokenSpaceGuid.PcdCapsuleEnable == TRUE
  CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibFmp/DxeRuntimeCapsuleLib.inf
!endif

  ResetSystemLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BaseResetSystemLib/BaseResetSystemLib.inf

!if gBoardModuleTokenSpaceGuid.PcdModularCryptoEnable == TRUE
  BaseCryptLib|CryptoPkg/Library/BaseCryptLibMbedTls/RuntimeCryptLib.inf
  TlsLib|CryptoPkg/Library/TlsLibNull/TlsLibNull.inf
  MbedTlsLib|CryptoPkg/Library/MbedTlsLib/MbedTlsLib.inf
!endif  # gBoardModuleTokenSpaceGuid.PcdModularCryptoEnable

#
# UEFI Variable Protection
#
!if gProtectedVariableFeaturePkgTokenSpaceGuid.PcdProtectedVariableEnable == TRUE
  ProtectedVariableRpmcLib|ProtectedVariablePkg/Library/ProtectedVariableRpmcLibNull/ProtectedVariableRpmcLibNull.inf
  HashApiLib|CryptoPkg/Library/BaseHashApiLib/BaseHashApiLib.inf
  ProtectedVariableKeyLib|ProtectedVariablePkg/Library/ProtectedVariableKeyLibNull/ProtectedVariableKeyLibNull.inf
  ProtectedVariableLib|ProtectedVariablePkg/Library/ProtectedVariableLib/DxeRuntimeProtectedVariableLib.inf
!endif

[LibraryClasses.X64.SMM_CORE]
!if $(TARGET) == DEBUG
  DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
  SerialIoUartDebugPropertyLib|$(PLATFORM_FULL_PACKAGE)/Library/SerialIoUartDebugPropertyLib/DxeSmmSerialIoUartDebugPropertyLib.inf
!endif
!if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
  DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
!endif

[Components.$(FSP_ARCH)]

#
# Common
#
!include $(PLATFORM_BOARD_PACKAGE)/Include/Dsc/CorePeiInclude.dsc

$(SILICON_PRODUCT_PATH)/EarlyDevices/EarlyDevices.inf
$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/SocInit/GpioV2ServicesInit/Pei/GpioV2PtlPcdPpiInitPei.inf

#
# HECI
#
!include $(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciDriverPei.dsc

!if gBoardModuleTokenSpaceGuid.PcdModularCryptoEnable == TRUE
  CryptoPkg/Driver/CryptoPei.inf {
    <LibraryClasses>
      BaseCryptLib|CryptoPkg/Library/BaseCryptLib/PeiCryptLib.inf
      TlsLib|CryptoPkg/Library/TlsLibNull/TlsLibNull.inf
  }
!endif

  MdeModulePkg/Universal/PCD/Pei/Pcd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }
  MdeModulePkg/Universal/StatusCodeHandler/Pei/StatusCodeHandlerPei.inf {
    <LibraryClasses>
!if $(TARGET) == DEBUG
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
!else
!if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
    DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
!endif
!endif
!if gPlatformModuleTokenSpaceGuid.PcdBeepStatusCodeEnable == TRUE
      NULL|BeepDebugFeaturePkg/Library/BeepStatusCodeHandlerLib/PeiBeepStatusCodeHandlerLib.inf
!endif
!if gPlatformModuleTokenSpaceGuid.PcdPostCodeStatusCodeEnable == TRUE && gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
      NULL|PostCodeDebugFeaturePkg/Library/PostCodeStatusCodeHandlerLib/PeiPostCodeStatusCodeHandlerLib.inf
!endif
!if gPlatformModuleTokenSpaceGuid.PcdSerialPortEnable == TRUE
      SerialPortLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiPlatformSerialPortLib/PeiPlatformSerialPortLib.inf
!else
      SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
!endif
      NULL|$(PLATFORM_SI_PACKAGE)/Library/TraceHubStatusCodeHandlerLib/PeiTraceHubStatusCodeHandlerLib.inf
  }

  #
  # FSP wrapper SEC Core
  #
!if $(FSP64_BUILD) == TRUE
  UefiCpuPkg/SecCore/SecCoreNative.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
!if $(TARGET) == DEBUG
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
      TraceHubDebugSysTLib|MdeModulePkg/Library/TraceHubDebugSysTLib/BaseTraceHubDebugSysTLib.inf
!endif
      NULL|MdePkg/Library/StackCheckLibNull/StackCheckLibNull.inf
  }
  UefiCpuPkg/ResetVector/Vtf0/Vtf0.inf {
    <BuildOptions>
      *_*_X64_NASMB_FLAGS = -DPAGE_TABLE_1G -DARCH_X64
  }
!else
  UefiCpuPkg/SecCore/SecCore.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
!if $(TARGET) == DEBUG
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
      TraceHubDebugSysTLib|MdeModulePkg/Library/TraceHubDebugSysTLib/BaseTraceHubDebugSysTLib.inf
!endif
!if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
      TraceHubDebugSysTLib|MdeModulePkg/Library/TraceHubDebugSysTLib/BaseTraceHubDebugSysTLib.inf
!endif
  }
!endif

!if (gEfiMdeModulePkgTokenSpaceGuid.PcdMigrateTemporaryRamFirmwareVolumes == TRUE)
  UefiCpuPkg/SecMigrationPei/SecMigrationPei.inf
!endif

!if gPlatformModuleTokenSpaceGuid.PcdCapsuleEnable == TRUE
  MdeModulePkg/Universal/CapsulePei/CapsulePei.inf  {
    <LibraryClasses>
!if ($(TARGET) == RELEASE) AND (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
    DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
!endif
!if ($(TARGET) == DEBUG)
      # To support debugging capsule update within debug resiliency BIOS in PEI phase
      DebugLib|MdeModulePkg/Library/PeiDebugLibDebugPpi/PeiDebugLibDebugPpi.inf
!endif
    }
!endif

#
# CpuMp PEIM for MpService PPI
#
!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  UefiCpuPkg/CpuMpPei/CpuMpPei.inf {
    <LibraryClasses>
      NULL|$(PLATFORM_SI_PACKAGE)/Library/PeiReadyToInstallMpLib/PeiReadyToInstallMpLib.inf
  }
!endif #PcdFspModeSelection

#
# CpuFeatures PEIM
#
  UefiCpuPkg/CpuFeatures/CpuFeaturesPei.inf {
    <LibraryClasses>
      RegisterCpuFeaturesLib|UefiCpuPkg/Library/RegisterCpuFeaturesLib/PeiRegisterCpuFeaturesLib.inf
!if $(TARGET) == DEBUG
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!endif
!if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
!endif
      NULL|UefiCpuPkg/Library/CpuCommonFeaturesLib/CpuCommonFeaturesLib.inf
      NULL|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/LibraryPrivate/PeiCpuSpecificFeaturesLib/PeiCpuSpecificFeaturesLib.inf
  }
#
# This is for reducing NATIVE DEBUG binary size, replacing some library routines with PPI.
#
!if $(TARGET) == DEBUG
  MdeModulePkg/Universal/DebugServicePei/DebugServicePei.inf {
    <LibraryClasses>
      DebugLib|MdeModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
  }
!endif

!if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
  $(PLATFORM_FULL_PACKAGE)/Features/CatalogTrace/TraceHubDebugServicePei/CatalogTHDebugServicePei.inf {
    <LibraryClasses>
     DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
}
!endif

$(PLATFORM_FEATURES_PATH)/PlatformStatusCodeHandler/Pei/PlatformStatusCodeHandlerPei.inf {
    <LibraryClasses>
!if $(TARGET) == DEBUG
    DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
!else
!if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
    DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
!endif
!endif
}

#
# Multi-IBB support (Platform info configuration)
#
  $(PLATFORM_BOARD_PACKAGE)/Features/MultiIbbConfig/MultiIbbConfig.inf

#
# Platform
#
  $(PLATFORM_PACKAGE)/PlatformInit/ReportFv/ReportFvPei.inf
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitPei/PlatformInitPreMem.inf {
    <LibraryClasses>
        NULL|$(PROJECT_PTL_BOARDS_BSP)/Library/BoardInitLib/Pei/PeiMultiBoardInitPreMemLib.inf
    }
!if gSiPkgTokenSpaceGuid.PcdTsnSupport == TRUE
!if gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == TRUE
  $(PLATFORM_BOARD_PACKAGE)/Features/Tsn/TsnDeviceConfigPei/TsnDeviceConfigPei.inf
!endif
!endif
  $(PLATFORM_BOARD_PACKAGE)/Features/FastBoot/FastBootPei/FastBootPei.inf
  $(PLATFORM_BOARD_PACKAGE)/Features/Connectivity/ConnectivityPei/ConnectivityPreMem.inf
!if gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable == TRUE
  $(PLATFORM_BOARD_PACKAGE)/BoardInitAdvanced/BoardInitAdvancedPei/BoardInitAdvancedPreMem.inf {
    <LibraryClasses>
      NULL|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
      GuidForwardLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiGuidForwardLib/PeiGuidForwardLib.inf
  }
!endif  # PcdAdvancedFeatureEnable
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitPei/PlatformInitPostMem.inf {
    <LibraryClasses>
        NULL|$(PROJECT_PTL_BOARDS_BSP)/Library/BoardInitLib/Pei/PeiMultiBoardInitPostMemLib.inf

        #
        # include this dummy library to build all Hii Structure PCD into bootloader PCD database.
        #
        NULL|$(PLATFORM_BOARD_PACKAGE)/BoardSupport/Library/HiiStructurePcdListLibNull/HiiStructurePcdListLibNull.inf
  }

!if gMipiCamFeaturePkgTokenSpaceGuid.PcdMipiCamFeatureEnable == TRUE
  $(PLATFORM_BOARD_PACKAGE)/Features/MipiCam/MipiCamInitPei/MipiCamInitPei.inf
!endif

  $(PLATFORM_BOARD_PACKAGE)/Features/CacheMtrr/MtrrPei/SetCacheMtrrPei.inf

!if gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable == TRUE
  $(PLATFORM_BOARD_PACKAGE)/BoardInitAdvanced/BoardInitAdvancedPei/BoardInitAdvancedPostMem.inf

#
# UPL Support
#
!if gBoardModuleTokenSpaceGuid.PcdUplEnable == TRUE
  UefiPayloadPkg/PayloadLoaderPeim/PayloadLoaderPeim.inf {
    <LibraryClasses>
      NULL|$(PLATFORM_BOARD_PACKAGE)/Features/Upl/Library/PeiPayloadHookLib/PeiPayloadHookLib.inf
    <BuildOptions>
      GCC:*_*_*_CC_FLAGS = -Wno-error=unused-but-set-variable
  }
!endif

#
# GBE Firmware Update Enable Support
#
!if gBoardModuleTokenSpaceGuid.PcdGbeFwUpdateEnable == TRUE
  $(PLATFORM_FEATURES_PATH)/MacUpdate/GbeFwUpdate.inf
!endif

#
# Crash Log Support
#
!if gCrashLogFeaturePkgTokenSpaceGuid.PcdCrashLogFeatureEnable == TRUE
  $(PLATFORM_BOARD_PACKAGE)/Features/CrashLog/Agent/CpuCrashLogAgent/CpuCrashLogPei.inf
  $(PLATFORM_BOARD_PACKAGE)/Features/CrashLog/Agent/SocCrashLogAgent/SocCrashLogPei.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdS3Enable == TRUE
  S3FeaturePkg/S3Pei/S3Pei.inf
!endif
!endif  # PcdAdvancedFeatureEnable

  $(PLATFORM_BSP_PATH)/BiosInfo/BiosInfo.inf

  SecurityPkg/FvReportPei/FvReportPei.inf
!if gFvCopyPkgTokenSpaceGuid.PcdFreeSpaceSkipFvCopyEnable == TRUE
  !include FvCopyFeaturePkg/Include/FvCopyFeature.dsc
!endif
  $(PLATFORM_PACKAGE)/Services/StallServicePei/StallServicePei.inf

  $(PLATFORM_PACKAGE)/PlatformInit/SiliconPolicyPei/SiliconPolicyPeiPreMem.inf {
    <LibraryClasses>
    !if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1  # API mode
      SiliconPolicyInitLib|$(PLATFORM_SI_PACKAGE)/Library/PeiPreMemSiliconPolicyInitLib/PeiPreMemSiliconPolicyInitLibApi.inf
      SiliconPolicyUpdateLib|$(PLATFORM_BSP_PATH)/Policy/Library/PeiSiliconPolicyUpdateLib/PeiSiliconPolicyUpdateLibApi.inf
    !else
      SiliconPolicyInitLib|$(PLATFORM_SI_PACKAGE)/Library/PeiPreMemSiliconPolicyInitLib/PeiPreMemSiliconPolicyInitLib.inf
      SiliconPolicyUpdateLib|$(PLATFORM_BSP_PATH)/Policy/Library/PeiSiliconPolicyUpdateLib/PeiSiliconPolicyUpdateLib.inf
    !endif
#
# Memory Diagnostic Test Feature for TSEG Region
#
!if gBoardModuleTokenSpaceGuid.PcdMemoryDiagnosticTestEnable == TRUE
      NULL|$(PLATFORM_BOARD_PACKAGE)/Features/MemoryDiagnosticTest/PeiMemoryDiagnosticTestLib/PeiMemoryDiagnosticTestLib.inf
!endif
  }

  $(PLATFORM_PACKAGE)/PlatformInit/SiliconPolicyPei/SiliconPolicyPeiPostMem.inf {
    <LibraryClasses>
    !if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1  # API mode
      SiliconPolicyInitLib|$(PLATFORM_SI_PACKAGE)/Library/PeiPostMemSiliconPolicyInitLib/PeiPostMemSiliconPolicyInitLibApi.inf
      SiliconPolicyUpdateLib|$(PLATFORM_BSP_PATH)/Policy/Library/PeiSiliconPolicyUpdateLib/PeiSiliconPolicyUpdateLibApi.inf
    !else
      SiliconPolicyInitLib|$(PLATFORM_SI_PACKAGE)/Library/PeiPostMemSiliconPolicyInitLib/PeiPostMemSiliconPolicyInitLib.inf
      SiliconPolicyUpdateLib|$(PLATFORM_BSP_PATH)/Policy/Library/PeiSiliconPolicyUpdateLib/PeiSiliconPolicyUpdateLib.inf
    !endif
  }

  IntelFsp2WrapperPkg/FspmWrapperPeim/FspmWrapperPeim.inf {
    <LibraryClasses>
      !if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1  # API mode
        SiliconPolicyInitLib|$(PLATFORM_SI_PACKAGE)/Library/PeiSiliconPolicyInitLibDependency/PeiPreMemSiliconPolicyInitLibDependency.inf
      !endif
      !if (gPlatformModuleTokenSpaceGuid.PcdTpmEnable == TRUE) OR (gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE)
        !if gSiPkgTokenSpaceGuid.PcdTxtEnable == TRUE
          NULL|$(PLATFORM_FULL_PACKAGE)/Features/Txt/Library/TpmInitDoneDepLib/TpmInitDoneDepLib.inf
        !endif
      !endif
      !if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 0  # Dispatch mode
        #
        # In FSP Dispatch mode below dummy library should be linked to bootloader PEIM
        # to build all DynamicEx PCDs that FSP consumes into bootloader PCD database.
        #
        NULL|$(PLATFORM_FSP_BIN_PACKAGE)/Library/FspPcdListLib/FspPcdListLibNull.inf
      !endif  # PcdFspModeSelection
!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
!if gPlatformModuleTokenSpaceGuid.PcdDeprecatedCryptoRemove == FALSE
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
!endif
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha384/HashInstanceLibSha384.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha512/HashInstanceLibSha512.inf
      NULL|SecurityPkg/Library/HashInstanceLibSm3/HashInstanceLibSm3.inf
!endif
    }
  IntelFsp2WrapperPkg/FspsWrapperPeim/FspsWrapperPeim.inf {
    <LibraryClasses>
      !if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1  # API mode
        SiliconPolicyInitLib|$(PLATFORM_SI_PACKAGE)/Library/PeiSiliconPolicyInitLibDependency/PeiPostMemSiliconPolicyInitLibDependency.inf
      !endif
!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
!if gPlatformModuleTokenSpaceGuid.PcdDeprecatedCryptoRemove == FALSE
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
!endif
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha384/HashInstanceLibSha384.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha512/HashInstanceLibSha512.inf
      NULL|SecurityPkg/Library/HashInstanceLibSm3/HashInstanceLibSm3.inf
!endif
  }

!if gNvmeFeaturePkgTokenSpaceGuid.PcdNvmeFeatureEnable == TRUE
  MdeModulePkg/Bus/Pci/NvmExpressPei/NvmExpressPei.inf {
  <LibraryClasses>
    DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLibBase.inf
  }
!endif
!if gSiPkgTokenSpaceGuid.PcdAtaEnable == TRUE
  SataFeaturePkg/AhciPciHcPei/AhciPciHcPei.inf
  MdeModulePkg/Bus/Ata/AhciPei/AhciPei.inf
!endif
!if gPlatformModuleTokenSpaceGuid.PcdCapsuleEnable == TRUE
  # Recovery module
  FatPkg/FatPei/FatPei.inf
!endif

#
# Security
#
!if gPlatformModuleTokenSpaceGuid.PcdTpmEnable == TRUE
  SecurityPkg/Tcg/PhysicalPresencePei/PhysicalPresencePei.inf

!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
  SecurityPkg/Tcg/Tcg2Config/Tcg2ConfigPei.inf {
    <LibraryClasses>
      Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2DeviceLibDTpm.inf
      Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
      Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterPei.inf
      NULL|$(PLATFORM_FULL_PACKAGE)/Library/PeiTxtPowerdownReqLib/PeiTxtPowerdownReqLib.inf
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
      NULL|$(PLATFORM_FULL_PACKAGE)/Library/PeiBootGuardEventLogLib/PeiBootGuardEventLogLib.inf
!if gPlatformModuleTokenSpaceGuid.PcdDeprecatedCryptoRemove == FALSE
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
!endif
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha384/HashInstanceLibSha384.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha512/HashInstanceLibSha512.inf
      NULL|SecurityPkg/Library/HashInstanceLibSm3/HashInstanceLibSm3.inf
  }
  SecurityPkg/Tcg/Tcg2Pei/Tcg2Pei.inf {
    <LibraryClasses>
      Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
      Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterPei.inf
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
!if gPlatformModuleTokenSpaceGuid.PcdDeprecatedCryptoRemove == FALSE
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
!endif
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha384/HashInstanceLibSha384.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha512/HashInstanceLibSha512.inf
      NULL|SecurityPkg/Library/HashInstanceLibSm3/HashInstanceLibSm3.inf
      NULL|$(PLATFORM_FULL_PACKAGE)/Library/PeiTpmS3ErrorHandlerLib/PeiTpmS3ErrorHandlerLib.inf
  }

  $(PLATFORM_PACKAGE)/Tcg/Tcg2PlatformPei/Tcg2PlatformPei.inf  {
    <LibraryClasses>
      Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
      Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterPei.inf
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
      !if gSiPkgTokenSpaceGuid.PcdTxtEnable == TRUE
        TpmPlatformHierarchyLib|$(PLATFORM_FEATURES_PATH)/Txt/Library/PeiDxeTpmPlatformHierarchyLib/PeiDxeTpmPlatformHierarchyLib.inf
      !else
        TpmPlatformHierarchyLib|SecurityPkg/Library/PeiDxeTpmPlatformHierarchyLib/PeiDxeTpmPlatformHierarchyLib.inf
      !endif # gSiPkgTokenSpaceGuid.PcdTxtEnable
  }
!endif

!if gPlatformModuleTokenSpaceGuid.PcdDeprecatedCryptoRemove == FALSE
  SecurityPkg/Tcg/TcgPei/TcgPei.inf
!endif
!endif # gPlatformModuleTokenSpaceGuid.PcdTpmEnable


#
# Features
#
!if gBoardModuleTokenSpaceGuid.PcdSetupEnable == TRUE
  !include $(PLATFORM_BOARD_PACKAGE)/Features/Setup/Include/Dsc/SetupPei.dsc
!endif

 MdeModulePkg/Universal/ResetSystemPei/ResetSystemPei.inf {
   <LibraryClasses>
   ResetSystemLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BaseResetSystemLib/BaseResetSystemLib.inf
 }

#
# VTd Iommu Security for Pei
#
!if gSiPkgTokenSpaceGuid.PcdSocCoreBootEnable == FALSE
  $(PLATFORM_FEATURES_PATH)/VTd/PlatformVTdSamplePei/PlatformVTdSamplePei.inf
  IntelSiliconPkg/Feature/VTd/IntelVTdCorePei/IntelVTdCorePei.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdAmtEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/Amt/AmtInit/Pei/AmtStatusCodePei.inf
!endif

#
# USB4 Connection Manager for Pei
#
$(PLATFORM_FEATURES_PATH)/Usb4Cm/Usb4PlatformPei/Usb4PlatformPei.inf

#
# Modular USBC IO driver
#
$(PLATFORM_BOARD_PACKAGE)/Features/ModularUsbCIo/ModularUsbCIoPei.inf

#
# S3
#
!if gSiPkgTokenSpaceGuid.PcdS3Enable == TRUE
  UefiCpuPkg/Universal/Acpi/S3Resume2Pei/S3Resume2Pei.inf
  UefiCpuPkg/PiSmmCommunication/PiSmmCommunicationPei.inf
!if gPlatformModuleTokenSpaceGuid.PcdOpalPasswordEnable == TRUE
  SecurityPkg/Tcg/Opal/OpalPassword/OpalPasswordPei.inf
!endif
!if gSiPkgTokenSpaceGuid.PcdAtaEnable == TRUE
!if gPlatformModuleTokenSpaceGuid.PcdHddPasswordEnable == TRUE
  SecurityPkg/HddPassword/HddPasswordPei.inf
!endif
!endif
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
  MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTablePei/FirmwarePerformancePei.inf
!endif

$(PLATFORM_BOARD_PACKAGE)/Features/I2c/SensorI2c/SensorI2c.inf

#
# DGR
#
!if gSiPkgTokenSpaceGuid.PcdPpamEnable == TRUE
  $(PLATFORM_FULL_PACKAGE)/Features/NiftyRock/MsegSmramPei/MsegSmramPei.inf
!endif

#
# PCT PEIM Parser
#
  $(PLATFORM_FULL_PACKAGE)/Features/Pct/PlatformConfigTool.inf

#
# Extended BIOS Region validation PEIM
#
!if gPlatformModuleTokenSpaceGuid.PcdExtendedBiosRegionSupport == TRUE
  $(PLATFORM_FULL_PACKAGE)/Platform/ValidateExtendedBiosRegion/Pei/ValidateExtendedBiosRegionPostMem.inf
!endif

[Components.X64]
#
# Common
#
!include $(PLATFORM_BOARD_PACKAGE)/Include/Dsc/CoreDxeInclude.dsc
!include $(PLATFORM_BOARD_PACKAGE)/Include/Dsc/CoreBdsDrivers.dsc

!if gBoardModuleTokenSpaceGuid.PcdUplEnable == FALSE
# After UPL enable will include these Core Drivers
  !include $(PLATFORM_BOARD_PACKAGE)/Include/Dsc/CoreDxeDrivers.dsc
!if gPlatformModuleTokenSpaceGuid.PcdNetworkEnable == TRUE
  !include NetworkPkg/Network.dsc.inc
!endif
!endif

  BoardModulePkg/LegacySioDxe/LegacySioDxe.inf

!if gNvmeFeaturePkgTokenSpaceGuid.PcdNvmePlnEnable == TRUE
  $(PLATFORM_BOARD_PACKAGE)/Features/Nvme/PowerLossNotifySettingDxe/PowerLossNotifySettingDxe.inf
!endif

!if gPlatformModuleTokenSpaceGuid.PcdCapsuleEnable == TRUE
  MdeModulePkg/Universal/CapsulePei/CapsuleX64.inf {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
      SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
      HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
      MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  }
!endif

!if gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable == TRUE
!if gSiPkgTokenSpaceGuid.PcdAcpiEnable == TRUE
    $(PLATFORM_BOARD_PACKAGE)/BoardInitAdvanced/BoardInitAdvancedSmm/BoardInitAdvancedSmm.inf {
     <BuildOptions>
       *_*_X64_CC_FLAGS      = -DDOCK_ENABLE
  }
!endif
!endif

#
# Silicon
#
!include $(SILICON_PRODUCT_PATH)/SiPkgDxe.dsc

$(PLATFORM_SI_PACKAGE)/Product/PantherLake/EarlyDevices/EarlyDxeDevices.inf

!if gSiPkgTokenSpaceGuid.PcdAmtEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/Amt/AmtInit/Dxe/AmtInitDxe.inf {
    <LibraryClasses>
!if $(TARGET) == DEBUG
      # It can't use PeiDxeDebugLibReportStatusCode as DebugLib, otherwise some HECI log will not shown.
    DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
!endif
!if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
!endif
  }
!endif

#
# Network
#
!if gPlatformModuleTokenSpaceGuid.PcdNetworkEnable == TRUE

# Temporary workaround for GCC and Xcode build due to these drivers causes system hang.
!if gOneClickRecoveryFeaturePkgTokenSpaceGuid.PcdWifiProfileSyncEnable == TRUE
    OneClickRecoveryFeaturePkg/WifiProfileSync/WifiProfileSync.inf
!endif
  NetworkPkg/WifiConnectionManagerDxe/WifiConnectionManagerDxe.inf
  BluetoothPkg/BluetoothConnectionManagerDxe/BluetoothConnectionManagerDxe.inf
  BluetoothPkg/UsbBtHciDxe/UsbBtHciDxe.inf
  BluetoothPkg/BluetoothBusDxe/BluetoothBusDxe.inf
  BluetoothPkg/BluetoothHidDxe/BluetoothHidDxe.inf
  BluetoothPkg/HidKbDxe/HidKbDxe.inf
  BluetoothPkg/HidMouseDxe/HidMouseDxe.inf
  BluetoothPkg/BleConPlatformDxe/BleConPlatformDxe.inf
!endif # gPlatformModuleTokenSpaceGuid.PcdNetworkEnable

!if gPlatformModuleTokenSpaceGuid.PcdCapsuleEnable == TRUE
  FmpDevicePkg/CapsuleUpdatePolicyDxe/CapsuleUpdatePolicyDxe.inf {
    <LibraryClasses>
      CapsuleUpdatePolicyLib|$(PLATFORM_BOARD_PACKAGE)/Features/CapsuleUpdate/Library/CapsuleUpdatePolicyLib/CapsuleUpdatePolicyLib.inf
  }

!if ("MSFT" in $(FAMILY)) || ($(TOOL_CHAIN_TAG) == "CLANGPDB")
  #
  # FwUpdateEfiLib.lib is for CLANGPDB and MSFT used only, not for GCC.
  # Only include the DXE driver when not GCC build.
  #
  $(PLATFORM_BOARD_PACKAGE)/Features/CapsuleUpdate/MeFwuDxe/MeFwuDxe.inf
!endif
!endif

!if gPsrFeatureModuleTokenSpaceGuid.PcdPlatformServiceRecordSupport == TRUE
  $(PLATFORM_BOARD_PACKAGE)/Features/PsrControlDxe/PsrControlDxe.inf
!endif

!if gUsbCCapsuleFeaturePkgTokenSpaceGuid.PcdUsbCCapsuleFeatureEnable == TRUE
  $(PLATFORM_BOARD_PACKAGE)/Features/Tcss/UsbCRetimerSupport/UsbCRetimerSupport.inf
!endif

#
# SMBIOS
#
!if gSiPkgTokenSpaceGuid.PcdSmbiosEnable == TRUE
  $(PLATFORM_BOARD_PACKAGE)/SmbiosPlatformInfo/SmbiosPlatformInfo.inf
  $(PLATFORM_BOARD_PACKAGE)/Features/Me/MeSmbiosUpdateConfigDxe/MeSmbiosUpdateConfigDxe.inf
!endif

  $(PLATFORM_BOARD_PACKAGE)/Features/ModularUsbCIo/ModularUsbCIoDxe.inf

#
# AcpiTables
#
!if gUsb4FeaturePkgTokenSpaceGuid.PcdDTbtEnable == TRUE
  Usb4FeaturePkg/AcpiTables/DTbtDxe.inf {
    <BuildOptions>
      *_*_*_ASLPP_FLAGS = $(DTBT_ASLPP_FLAGS)
  }
!endif

!if gSiPkgTokenSpaceGuid.PcdVmdEnable == TRUE
  $(PLATFORM_FEATURES_PATH)/Rst/RstUefiDriverSupport/RstUefiDriverSupport.inf
!endif


#
# VTd Iommu Security
#
!if gSiPkgTokenSpaceGuid.PcdSocCoreBootEnable == FALSE
  $(PLATFORM_FEATURES_PATH)/VTd/PlatformVTdSampleDxe/PlatformVTdSampleDxe.inf
  IntelSiliconPkg/Feature/VTd/IntelVTdCoreDxe/IntelVTdCoreDxe.inf
!endif

#
# SmmAccess
#
  IntelSiliconPkg/Feature/SmmAccess/SmmAccessDxe/SmmAccess.inf

#
# Platform
#
  $(PLATFORM_PACKAGE)/PlatformInit/SiliconPolicyDxe/SiliconPolicyDxe.inf

  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitDxe/PlatformInitDxe.inf {
    <LibraryClasses>
      NULL|$(PROJECT_PTLUH_BOARDS)/Library/BoardInitLib/Dxe/DxeMultiBoardInitlib.inf
  }
  $(PLATFORM_BOARD_PACKAGE)/Features/CacheMtrr/MtrrDxe/SetCacheMtrrDxe.inf

  $(PLATFORM_BOARD_PACKAGE)/BoardInitAdvanced/BoardInitAdvancedDxe/BoardInitAdvancedDxe.inf {
    <LibraryClasses>
      BootGuardRevocationLib|$(PLATFORM_FULL_PACKAGE)/Library/BootGuardRevocationLib/BootGuardRevocationLib.inf
      DxeFirmwareVersionInfoLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeFirmwareVersionInfoLib/DxeFirmwareVersionInfoLib.inf
      DxeSmbiosDataHobLib|IntelSiliconPkg/Library/DxeSmbiosDataHobLib/DxeSmbiosDataHobLib.inf
!if $(TARGET) == DEBUG
      # It can't use PeiDxeDebugLibReportStatusCode as DebugLib, otherwise some Telemetry log will not shown.
    DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
!endif
!if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
!endif
  }

!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  IntelFsp2WrapperPkg/FspWrapperNotifyDxe/FspWrapperNotifyDxe.inf
!endif
  $(PLATFORM_PACKAGE)/FspWrapper/SaveMemoryConfig/SaveMemoryConfig.inf {
    <LibraryClasses>
      NULL|$(PLATFORM_FULL_PACKAGE)/Library/DxeSaveMemoryConfigLib/DxeSaveMemoryConfigLib.inf
  }

#
# Memory Address Decode Encode
#
Intel/OneSiliconPkg/IpBlock/MemoryInit/Library/PeiDxeSmmMemAddrEncodeLib/MemAddrDecodeEncodeLib.inf

#
# Bdat List: show Bdat info
#
$(PLATFORM_FULL_PACKAGE)/Library/DxeDriverBdat/BdatList.inf

#
# USB4 for Dxe
#
$(PLATFORM_FEATURES_PATH)/Usb4Cm/Usb4PlatformDxe/Usb4PlatformDxe.inf

  $(PLATFORM_PACKAGE)/Test/TestPointStubDxe/TestPointStubDxe.inf
  $(PLATFORM_PACKAGE)/Test/TestPointDumpApp/TestPointDumpApp.inf

!if gBoardModuleTokenSpaceGuid.PcdSetupEnable == TRUE
  !include $(PLATFORM_BOARD_PACKAGE)/Features/Setup/Include/Dsc/SetupDxe.dsc

!if gPlatformModuleTokenSpaceGuid.PcdUserAuthenticationEnable == TRUE
  UserAuthFeaturePkg/UserAuthenticationDxeSmm/UserAuthenticationDxe.inf {
    <LibraryClasses>
      PlatformPasswordLib|UserAuthFeaturePkg/Library/PlatformPasswordLibNull/PlatformPasswordLibNull.inf
!if $(TARGET) == DEBUG
      # If want to use PeiDxeDebugLibReportStatusCode as DebugLib to save size,
      # the log of 'Welcome Admin' or 'Admin password is not set' will not shown.
    DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
!endif
!if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
!endif
    }
  UserAuthFeaturePkg/UserAuthenticationDxeSmm/UserAuthenticationSmm.inf {
    <LibraryClasses>
      PlatformPasswordLib|UserAuthFeaturePkg/Library/PlatformPasswordLibNull/PlatformPasswordLibNull.inf
    }
!endif
!endif

#
# OS Boot
#
!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE
!if gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable == TRUE
  $(PLATFORM_BOARD_PACKAGE)/Acpi/AdvancedAcpiDxe/AdvancedAcpiDxe.inf
  $(PLATFORM_PACKAGE)/Acpi/AcpiTables/AcpiPlatform.inf
  $(PLATFORM_FEATURES_PATH)/Acpi/AcpiTables/AcpiFeatures.inf
  $(PLATFORM_FEATURES_PATH)/Pep/Dxe/PepOvrDxe.inf
  $(PLATFORM_BOARD_PACKAGE)/Acpi/AcpiTables/AcpiTables.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/CpuPowerManagement/AcpiTables/CpuAcpiTables.inf
  PmaxFeaturePkg/PmaxDxe/PmaxDxe.inf

#
# PlatformData
#
!if gSiPkgTokenSpaceGuid.PcdAcpiEnable == TRUE
  $(PLATFORM_BOARD_PACKAGE)/Features/DptfPlatformData/Dxe/DptfPlatformData.inf
  $(PLATFORM_BOARD_PACKAGE)/Features/DptfPlatformData/Acpi/DptfPlatformDataTables.inf
!endif
#
# DGR
#
!if gSiPkgTokenSpaceGuid.PcdPpamEnable == TRUE
  $(PLATFORM_FULL_PACKAGE)/Features/NiftyRock/PpamPlatformSmm/PpamPlatformSmm.inf
!endif
!else
  $(PLATFORM_PACKAGE)/Acpi/AcpiTables/AcpiPlatform.inf {
    <LibraryClasses>
      NULL|$(PROJECT_PTLUH_BOARDS)/Library/BoardAcpiLib/BaseMultiBoardAcpiSupportLib.inf
  }
  $(PLATFORM_BOARD_PACKAGE)/Acpi/BoardAcpiDxe/BoardAcpiDxe.inf {
    <LibraryClasses>
      NULL|$(PROJECT_PTLUH_BOARDS)/Library/BoardAcpiLib/BaseMultiBoardAcpiSupportLib.inf
  }
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitSmm/PlatformInitSmm.inf
  $(PLATFORM_PACKAGE)/Acpi/AcpiSmm/AcpiSmm.inf
!endif #PcdAdvancedFeatureEnable

  $(PLATFORM_FULL_PACKAGE)/Platform/SpiFvbService/SpiFvbServiceDxe.inf
!if gSiPkgTokenSpaceGuid.PcdSmmVariableEnable == TRUE
  IntelSiliconPkg/Feature/Flash/SpiFvbService/SpiFvbServiceSmm.inf
!endif # gSiPkgTokenSpaceGuid.PcdSmmVariableEnable
!endif #PcdBootToShellOnly

#
# Security
#

!if gPlatformModuleTokenSpaceGuid.PcdTpmEnable == TRUE
!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
  $(PLATFORM_PACKAGE)/Tcg/Tcg2PlatformDxe/Tcg2PlatformDxe.inf {
    <LibraryClasses>
      Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
        !if gSiPkgTokenSpaceGuid.PcdTxtEnable == TRUE
          TpmPlatformHierarchyLib|$(PLATFORM_FEATURES_PATH)/Txt/Library/PeiDxeTpmPlatformHierarchyLib/PeiDxeTpmPlatformHierarchyLib.inf
        !else
          TpmPlatformHierarchyLib|SecurityPkg/Library/PeiDxeTpmPlatformHierarchyLib/PeiDxeTpmPlatformHierarchyLib.inf
        !endif # gSiPkgTokenSpaceGuid.PcdTxtEnable
  }
!endif # gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable
!endif # gPlatformModuleTokenSpaceGuid.PcdTpmEnable

!if gSiPkgTokenSpaceGuid.PcdAmtEnable == TRUE
  $(PLATFORM_FEATURES_PATH)/Amt/AsfTable/AsfTable.inf
!if gOneClickRecoveryFeaturePkgTokenSpaceGuid.PcdOneClickRecoveryEnable == TRUE
  OneClickRecoveryFeaturePkg/OneClickRecovery/OneClickRecovery.inf
!endif

!if gPsFeatureModuleTokenSpaceGuid.PcdPlatformSanitizeFeatureEnable == TRUE
  $(PLATFORM_BOARD_PACKAGE)/Features/PlatformSanitize/PsBoardOemDxe/PsBoardOemDxe.inf
  !if gSiPkgTokenSpaceGuid.PcdAtaEnable == TRUE
    SataFeaturePkg/SataEraseDxe/SataEraseDxe.inf
  !endif
  !if gSiPkgTokenSpaceGuid.PcdVmdEnable == TRUE
    $(PLATFORM_BOARD_PACKAGE)/Features/PlatformSanitize/StorageSanitize/RstEraseDxe/RstEraseDxe.inf
  !endif
!endif
!endif

#
# ME
#
$(PLATFORM_FEATURES_PATH)/Me/MeExtMeasurement/Dxe/MeExtMeasurement.inf


#
# USB4 Connection Manager
#
Usb4FeaturePkg/Usb4CmDxe/Usb4CmDxe.inf

!if gI2cFeaturePkgTokenSpaceGuid.PcdI2cTouchCommonEnable == TRUE
  $(PLATFORM_BOARD_PACKAGE)/Acpi/I2cBoardData/I2cBoardData.inf
!endif

$(PLATFORM_BOARD_PACKAGE)/Acpi/I2cPmData/I2cPmData.inf
#
# Crash Log Support
#
!if gCrashLogFeaturePkgTokenSpaceGuid.PcdCrashLogFeatureEnable == TRUE
  $(PLATFORM_BOARD_PACKAGE)/Features/CrashLog/CrashLogVariableUpdateDxe/CrashLogVariableUpdateDxe.inf
  CrashLogFeaturePkg/CrashLogDxe/CrashLogDxe.inf
!endif

$(PLATFORM_BOARD_PACKAGE)/Features/FastBoot/FastBootHandlerDxe/FastBootHandlerDxe.inf
$(PLATFORM_BOARD_PACKAGE)/Features/FastBoot/FastBootSetupDxe/FastBootSetupDxe.inf


#
# Hsti advanced feature containing HstiResultDxe driver
#
!include HstiFeaturePkg/Include/HstiFeature.dsc

#
# Hsti
#
$(PLATFORM_FEATURES_PATH)/Hsti/HstiIhvDxe/HstiIhvDxe.inf {
  <LibraryClasses>
    HstiLib|MdePkg/Library/DxeHstiLib/DxeHstiLib.inf
    Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
}

!if gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable == TRUE
!if gBoardModuleTokenSpaceGuid.PcdSetupEnable == TRUE
!endif

$(PLATFORM_FULL_PACKAGE)/Platform/PciPlatform/Dxe/PciPlatform.inf {
  <LibraryClasses>
    DxeOpromPatchLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeOpromPatchLib/DxeOpromPatchLib.inf
}

!if gPlatformModuleTokenSpaceGuid.PcdPciHotplugEnable == TRUE
  $(PLATFORM_FULL_PACKAGE)/Platform/PciHotPlug/PciHotPlug.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdAcpiEnable == TRUE
!if gBoardModuleTokenSpaceGuid.PcdAcpiDebugFeatureEnable == TRUE
  AcpiDebugFeaturePkg/AcpiDebugDxeSmm/AcpiDebugDxe.inf
!endif
!endif

!if gPlatformModuleTokenSpaceGuid.PcdVirtualKeyboardEnable == TRUE
  VirtualKeyboardFeaturePkg/VirtualKeyboardDxe/VirtualKeyboardDxe.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == 0x1
  $(PLATFORM_FEATURES_PATH)/LifeCycleState/LifeCycleState.inf
!endif

!endif  #PcdAdvancedFeatureEnable

!if gEcFeaturePkgTokenSpaceGuid.PcdEcEnable == TRUE
  EcFeaturePkg/EcDxe/EcDxe.inf
  EcFeaturePkg/AcpiTables/EcAcpiTables.inf
  EcFeaturePkg/BatteryDxe/BatteryDxe.inf
!endif

!if gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable == TRUE
  $(PLATFORM_FEATURES_PATH)/DashG/DxeDgOpregionInit.inf
!endif

  $(PLATFORM_FEATURES_PATH)/PlatformStatusCodeHandler/Smm/PlatformStatusCodeHandlerSmm.inf
  $(PLATFORM_FEATURES_PATH)/GuidForwardHandler/Smm/GuidForwardHandlerSmm.inf {
    <LibraryClasses>
!if $(TARGET) == DEBUG
      # It can't use PeiDxeDebugLibReportStatusCode as DebugLib, otherwise some forward SMM log will not shown.
    DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
!endif
!if gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
!endif
  }
  $(PLATFORM_FEATURES_PATH)/GuidForwardHandler/RuntimeDxe/GuidForwardHandlerRuntimeDxe.inf {
    <LibraryClasses>
!if $(TARGET) == DEBUG
      # It can't use PeiDxeDebugLibReportStatusCode as DebugLib, otherwise some forward DXE log will not shown.
      DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
      LpssUartLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssUart/Library/LpssUartLib/DxeRuntimeLpssUartLib.inf
!endif
!if gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE
    DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
!endif
  }

!if gBoardModuleTokenSpaceGuid.PcdTerminalEnable == TRUE
!if gSiPkgTokenSpaceGuid.PcdSourceDebugEnable == FALSE
  SpcrFeaturePkg/SpcrAcpiDxe/SpcrAcpiDxe.inf {
    <LibraryClasses>
      SpcrDeviceLib|$(PLATFORM_FEATURES_PATH)/Amt/Library/AmtSpcrDeviceLib/AmtSpcrDeviceLib.inf
  }
!endif
!endif

$(PLATFORM_FEATURES_PATH)/PlatformStatusCodeHandler/RuntimeDxe/PlatformStatusCodeHandlerRuntimeDxe.inf {
    <LibraryClasses>
      LpssUartLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssUart/Library/LpssUartLib/DxeRuntimeLpssUartLib.inf
!if $(TARGET) == DEBUG
    DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
    LpssUartDebugPropertyPcdLib|$(PLATFORM_SI_PACKAGE)/IpBlock/LpssUart/Library/LpssUartDebugPropertyPcdLib/LpssUartDebugPropertyPcdLib.inf
!else
!if gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE
    DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibTraceHubCatalog/BaseDebugLibTraceHubCatalog.inf
!endif
!endif
!if gPlatformModuleTokenSpaceGuid.PcdBeepStatusCodeEnable == TRUE
  NULL|BeepDebugFeaturePkg/Library/BeepStatusCodeHandlerLib/RuntimeDxeBeepStatusCodeHandlerLib.inf
!endif
!if gPlatformModuleTokenSpaceGuid.PcdPostCodeStatusCodeEnable == TRUE
  NULL|PostCodeDebugFeaturePkg/Library/PostCodeStatusCodeHandlerLib/RuntimeDxePostCodeStatusCodeHandlerLib.inf
!endif
  NULL|$(PLATFORM_SI_PACKAGE)/Library/TraceHubStatusCodeHandlerLib/RuntimeDxeTraceHubStatusCodeHandlerLib.inf
}

  LogoFeaturePkg/LogoDxe/LogoDxe.inf

#
# Platform Pdt feature support Driver
#
$(PLATFORM_FEATURES_PATH)/Pdt/PlatformPdtFeatureDxe.inf {
  <LibraryClasses>
    PdtUpdateLib|$(PLATFORM_FULL_PACKAGE)/Library/PdtUpdateLib/PdtUpdateLib.inf
}

!if gTmeFeaturePkgTokenSpaceGuid.PcdTmeFeatureEnable == TRUE
  !include TmeFeaturePkg/Include/TmeFeature.dsc
!endif

!if gPlatformModuleTokenSpaceGuid.PcdTerminalOverDebugEnable == TRUE
  $(PLATFORM_FEATURES_PATH)/TerminalOverDebug/TerminalOverDebug.inf
!endif
#
# Extended BIOS Region validation driver
#
!if gPlatformModuleTokenSpaceGuid.PcdExtendedBiosRegionSupport == TRUE
  $(PLATFORM_FULL_PACKAGE)/Platform/ValidateExtendedBiosRegion/Dxe/ValidateExtendedBiosRegionDxe.inf
!endif

#
# Build Options
#
!include $(SILICON_PRODUCT_PATH)/SiPkgBuildOption.dsc
!include BoardPkgBuildOption.dsc
