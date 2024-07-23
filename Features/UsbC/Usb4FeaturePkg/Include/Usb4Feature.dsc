## @file
# This is a build description file for the Discrete TBT feature.
# This file should be included into another package DSC file to build this feature.
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
#@par Specification
##

[Defines]
  #
  # Set TCSS specific package/folder name, same as passed from PREBUILD script.
  # PLATFORM_PACKAGE would be the same as PLATFORM_NAME as well as package build folder
  # DEFINE only takes effect at Usb4FeaturePkg. Intel\Features\Usb4FeaturePkg\Usb4FeaturePkg.dec
  #
  DEFINE      USB4CM_FEATURES_LIB_PATH              = Usb4FeaturePkg/Library/Usb4Cm
  DEFINE      USB4CM_FEATURES_PRIVATE_LIB_PATH      = Usb4FeaturePkg/LibraryPrivate/Usb4Cm
  DEFINE      TBT_FEATURES_LIB_PATH                 = Usb4FeaturePkg/Library/TbtInit

[Packages]
  Usb4FeaturePkg/Usb4FeaturePkg.dec

[LibraryClasses]

  #
  # Usb4Cm private library
  #
  CmUtilsLib|$(USB4CM_FEATURES_PRIVATE_LIB_PATH)/CmUtilsLib/CmUtilsLib.inf
  Usb4HrSrvLib|$(USB4CM_FEATURES_PRIVATE_LIB_PATH)/Usb4HrSrvLib/Usb4HrSrvLib.inf
  Usb4HiCoreLib|$(USB4CM_FEATURES_PRIVATE_LIB_PATH)/Usb4HiCoreLib/Usb4HiCoreLib.inf
  Usb4DomainLib|$(USB4CM_FEATURES_PRIVATE_LIB_PATH)/Usb4DomainLib/Usb4DomainLib.inf
  Usb4RtEnumLib|$(USB4CM_FEATURES_PRIVATE_LIB_PATH)/Usb4RtEnumLib/Usb4RtEnumLib.inf
  Usb4CsLib|$(USB4CM_FEATURES_PRIVATE_LIB_PATH)/Usb4CsLib/Usb4CsLib.inf
  CmEvtLib|$(USB4CM_FEATURES_PRIVATE_LIB_PATH)/CmEvtLib/CmEvtLib.inf
  Usb4ProtocolsLib|$(USB4CM_FEATURES_PRIVATE_LIB_PATH)/Usb4ProtocolsLib/Usb4ProtocolsLib.inf
  Tbt3SupportLib|$(USB4CM_FEATURES_PRIVATE_LIB_PATH)/Tbt3SupportLib/Tbt3SupportLib.inf

  #
  # Usb4Cm Common library
  #
  DUsb4HrPeiLib|$(USB4CM_FEATURES_LIB_PATH)/DUsb4HrPeiLib/DUsb4HrPeiLib.inf
  IUsb4HrLib|$(USB4CM_FEATURES_LIB_PATH)/IUsb4HrLib/IUsb4HrLib.inf

  #
  # Usb4Cm library
  #
  Usb4PeiLib|$(USB4CM_FEATURES_LIB_PATH)/Usb4PeiLib/Usb4PeiLib.inf
  Usb4DebugLib|$(USB4CM_FEATURES_LIB_PATH)/Usb4DebugLib/Usb4DebugLib.inf
  Usb4HobDumpLib|$(USB4CM_FEATURES_LIB_PATH)/Usb4HobDumpLib/Usb4HobDumpLib.inf

  #
  # TBT initialization library
  #
  DTbtCommonLib|$(TBT_FEATURES_LIB_PATH)/DTbtCommonLib/DTbtCommonLib.inf
  PeiDTbtInitLib|$(TBT_FEATURES_LIB_PATH)/PeiDTbtInitLib/PeiDTbtInitLib.inf
  PeiDTbtPolicyLib|$(TBT_FEATURES_LIB_PATH)/PeiDTbtPolicyLib/PeiDTbtPolicyLib.inf
  DxeDTbtPolicyLib|$(TBT_FEATURES_LIB_PATH)/DxeDTbtPolicyLib/DxeDTbtPolicyLib.inf
  PeiTbtTaskDispatchLib|$(TBT_FEATURES_LIB_PATH)/PeiTbtTaskDispatchLib/PeiTbtTaskDispatchLib.inf

[Components.X64]
  !if gUsb4FeaturePkgTokenSpaceGuid.PcdDTbtEnable == TRUE
  Usb4FeaturePkg/AcpiTables/DTbtDxe.inf {
    <BuildOptions>
      *_*_*_ASLPP_FLAGS = $(DTBT_ASLPP_FLAGS)
  }
  !endif

  #
  # USB4 Connection Manager
  #
  Usb4FeaturePkg/Usb4CmDxe/Usb4CmDxe.inf