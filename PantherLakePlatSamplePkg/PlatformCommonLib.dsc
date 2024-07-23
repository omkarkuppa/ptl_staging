## @file
#  platform build option configuration file.
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
#  @par Specification
##

[LibraryClasses.common]
  SiliconPolicyInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/SiliconPolicyInitLibNull/SiliconPolicyInitLibNull.inf
  SiliconPolicyUpdateLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/SiliconPolicyUpdateLibNull/SiliconPolicyUpdateLibNull.inf

  CmosAccessLib|CmosFeaturePkg/Library/CmosAccessLib/CmosAccessLib.inf
  PlatformNvRamHookLib|CmosFeaturePkg/Library/PlatformNvRamHookLibCmos/PlatformNvRamHookLibCmos.inf

  BeepLib|$(PLATFORM_FULL_PACKAGE)/Library/BeepLib/BeepLib.inf
  BootGuardRevocationLib|$(PLATFORM_FULL_PACKAGE)/Library/BootGuardRevocationLib/BootGuardRevocationLib.inf
  HidI2cPlatformSupportLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeHidI2cPlatformSupportLib/DxeHidI2cPlatformSupportLib.inf
  PeiBootGuardEventLogLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiBootGuardEventLogLib/PeiBootGuardEventLogLib.inf
  SerialIoUartDebugHelperLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseSerialIoUartDebugHelperLib/BaseSerialIoUartDebugHelperLib.inf
  SerialIoUartDebugHelperLibNull|$(PLATFORM_FULL_PACKAGE)/Library/BaseSerialIoUartDebugHelperLib/BaseSerialIoUartDebugHelperLibNull.inf
  SerialIoUartDebugPropertyLib|$(PLATFORM_FULL_PACKAGE)/Library/SerialIoUartDebugPropertyLib/BaseSerialIoUartDebugPropertyLib.inf

  DimmInfoLib|$(PLATFORM_FULL_PACKAGE)/Setup/DimmInfoLib.inf

!include EcFeaturePkg/Include/EcFeatureLib.dsc

!if gBoardModuleTokenSpaceGuid.PcdSetupEnable == TRUE
  PlatformBootManagerLib|$(PLATFORM_FULL_PACKAGE)/Library/DxePlatformBootManagerLib/DxePlatformBootManagerLib.inf
!else
  PlatformBootManagerLib|$(PLATFORM_FULL_PACKAGE)/Library/DxePlatformBootManagerLib/DxePlatformBootManagerHiiFreeLib.inf
!endif


!if gCmosFeaturePkgTokenSpaceGuid.PcdPlatformCmosAccessSupport == TRUE
  PlatformCmosAccessLib|CmosFeaturePkg/Library/PlatformCmosAccessLib/PlatformCmosAccessLib.inf
!else
  PlatformCmosAccessLibNull|CmosFeaturePkg/Library/PlatformCmosAccessLibNull/PlatformCmosAccessLibNull.inf
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable == TRUE
  PlatformSecureLib|$(PLATFORM_FULL_PACKAGE)/Library/PlatformSecureLibTest/PlatformSecureLibTest.inf
!else
  PlatformSecureLib|SecurityPkg/Library/PlatformSecureLibNull/PlatformSecureLibNull.inf
!endif

!if gPlatformModuleTokenSpaceGuid.PcdSerialPortEnable == TRUE
  SerialPortLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseSerialPortLib/BaseSerialPortLib.inf
!else
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == 0x1
  PeiSubRegionLib|$(PLATFORM_FEATURES_PATH)/SubRegion/PeiSubRegionLib/PeiSubRegionLib.inf
  PeiTsnFvLib|$(PLATFORM_FEATURES_PATH)/Tsn/PeiTsnFvLib/PeiTsnFvLib.inf
!else
  PeiSubRegionLib|$(PLATFORM_FEATURES_PATH)/SubRegion/PeiSubRegionLib/PeiSubRegionLibNull.inf
  PeiTsnFvLib|$(PLATFORM_FEATURES_PATH)/Tsn/PeiTsnFvLib/PeiTsnFvLibNull.inf
!endif

  FspWrapperPlatformLib|$(PLATFORM_FULL_PACKAGE)/FspWrapper/Library/PeiFspWrapperPlatformLib/PeiFspWrapperPlatformLib.inf
  FspWrapperPlatformResetLib|$(PLATFORM_FULL_PACKAGE)/FspWrapper/Library/PeiFspWrapperPlatformResetLib/PeiFspWrapperPlatformResetLib.inf
  FspWrapperHobProcessLib|$(PLATFORM_FULL_PACKAGE)/FspWrapper/Library/PeiFspWrapperHobProcessLib/PeiFspWrapperHobProcessLib.inf
  PlatformInitLib|$(PLATFORM_FULL_PACKAGE)/Library/PlatformInitLib/PlatformInitLib.inf
  #PlatformSecLib|$(PLATFORM_FULL_PACKAGE)/FspWrapper/Library/SecFspWrapperPlatformSecLib/SecFspWrapperPlatformSecLib.inf

!if gSiPkgTokenSpaceGuid.PcdTxtEnable == TRUE
  TpmInitDoneDepLib|$(PLATFORM_FEATURES_PATH)/Txt/Library/TpmInitDoneDepLib/TpmInitDoneDepLib.inf
  TpmPlatformHierarchyLib|$(PLATFORM_FEATURES_PATH)/Txt/Library/TpmPlatformHierarchyLib/TpmPlatformHierarchyLib.inf
!else
  TpmPlatformHierarchyLib|$(PLATFORM_FULL_PACKAGE)/Tcg/Library/TpmPlatformHierarchyLib/TpmPlatformHierarchyLib.inf
!endif
